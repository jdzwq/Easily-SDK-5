
/***********************************************************************
	Easily tcp service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tcp service document

	@module	xtcps.cc | tcp service implement file

***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "xtcps.h"
#include "srvlog.h"
#include "srvcrt.h"

static void _xtcps_get_config(const tchar_t* site, tchar_t* sz_path, tchar_t* sz_proc)
{
	tchar_t sz_root[PATH_LEN + 1] = { 0 };
	tchar_t sz_file[PATH_LEN + 1] = { 0 };

	get_envvar(XSERVICE_ROOT, sz_root, PATH_LEN);
	if (is_null(sz_root))
	{
		//xscpy(sz_root,_T("."));
		get_runpath((res_modu_t)0, sz_root, PATH_LEN);
	}
	xsprintf(sz_file, _T("%s/cfg/%s.config"), sz_root, site);

	LINKPTR ptr_cfg = create_xml_doc();
	if (!load_xml_doc_from_file(ptr_cfg, NULL, sz_file))
	{
		destroy_xml_doc(ptr_cfg);
		return;
	}

	LINKPTR nlk_config = get_xml_dom_node(ptr_cfg);

	LINKPTR nlk_site = get_dom_first_child_node(nlk_config);
	while (nlk_site)
	{
		if (compare_text(get_dom_node_name_ptr(nlk_site), -1, site, -1, 1) == 0)
		{
			LINKPTR nlk = get_dom_first_child_node(nlk_site);
			while (nlk)
			{
				if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("path"), -1, 1) == 0 && sz_path)
				{
					get_dom_node_text(nlk, sz_path, PATH_LEN);
				}
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("proc"), -1, 1) == 0 && sz_proc)
				{
					get_dom_node_text(nlk, sz_proc, PATH_LEN);
				}
				
				nlk = get_dom_next_sibling_node(nlk);
			}
		}
		nlk_site = get_dom_next_sibling_node(nlk_site);
	}

	destroy_xml_doc(ptr_cfg);
}

static void _xtcps_track_error(void* hand, const tchar_t* code, const tchar_t* text)
{
	tcps_block_t* pb = (tcps_block_t*)hand;

	tchar_t addr[ADDR_LEN + 1] = { 0 };
	tchar_t token[PATH_LEN + 1] = { 0 };
	int len;
	int port;

	port = (int)xtcp_peer_port(pb->tcp, addr);

	len = xsprintf(token, _T("TCP-SCP: [%s : %d] %s %s\r\n"), addr, port, code, text);

	xportm_log_info(token, len);
}

/**********************************************************************************************************************/
void _xtcps_dispatch(xhand_t tcp, void* p)
{
	tchar_t sz_path[PATH_LEN + 1] = { 0 };
	tchar_t sz_proc[PATH_LEN + 1] = { 0 };
	tchar_t sz_site[RES_LEN + 1] = { 0 };
	tchar_t sz_timo[INT_LEN + 1] = { 0 };
    tchar_t sz_name[RES_LEN + 1] = { 0 };

	int n_state = 0;

	xtcps_param_t* pxp = (xtcps_param_t*)p;
	tcps_block_t *pb = NULL;
	res_modu_t api = NULL;
	PF_TCPS_INVOKE pf_invoke = NULL;

	xdate_t xdt = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };
	tchar_t sz_hmac[KEY_LEN + 1] = { 0 };
	byte_t textbom[4] = { 0 };

	TRY_CATCH;

	get_envvar(XSERVICE_ROOT, sz_path, PATH_LEN);

	if (is_null(sz_path))
	{
		//xscpy(sz_path,_T("."));
		get_runpath((res_modu_t)0, sz_path, PATH_LEN);
	}

	get_param_item(pxp->sz_param, _T("TIMO"), sz_timo, INT_LEN);
	get_param_item(pxp->sz_param, _T("SITE"), sz_site, RES_LEN);

	xszero(sz_path, PATH_LEN);

	_xtcps_get_config(sz_site, sz_path, sz_proc);

	if (is_null(sz_path))
	{
		raise_user_error(_T("_tcps_invoke"), _T("website not define service entry\n"));
	}

	if (is_null(sz_proc))
	{
		raise_user_error(_T("_tcps_invoke"), _T("website not define service module\n"));
	}

	pb = (tcps_block_t*)xmem_alloc(sizeof(tcps_block_t));
	pb->cbs = sizeof(tcps_block_t);

	pb->tcp = tcp;
	pb->is_thread = IS_THREAD_MODE(pxp->sz_mode);
	pb->timo = xstol(sz_timo);

	printf_path(pb->path, sz_path);
	xsncpy(pb->site, sz_site, RES_LEN);

	xszero(sz_path, PATH_LEN);
	printf_path(sz_path, sz_proc);

	api = load_library(sz_path);
	if (!api)
	{
		raise_user_error(_T("_tcps_invoke"), _T("website load service module failed\n"));
	}

	pf_invoke = (PF_TCPS_INVOKE)get_address(api, "tcps_invoke");
	if (!pf_invoke)
	{
		raise_user_error(_T("_tcps_invoke"), _T("website load invoke function failed\n"));
	}

	pb->ptk = (if_track_t*)xmem_alloc(sizeof(if_track_t));
	pb->ptk->param = (void*)pb;
	pb->ptk->pf_track_error = (PF_TRACK_ERROR)_xtcps_track_error;

	n_state = (*pf_invoke)(pb);

	free_library(api);
	api = NULL;

	xmem_free(pb->ptk);
	pb->ptk = NULL;

	xmem_free(pb);
	pb = NULL;

	END_CATCH;

	return;

ONERROR:
	get_last_error(errcode, errtext, ERR_LEN);

	if (api)
	{
		free_library(api);
	}

	if (pb)
	{
		_xtcps_track_error((void*)pb, errcode, errtext);

		if (pb->ptk)
			xmem_free(pb->ptk);

		xmem_free(pb);
	}

	return;
}

void _xtcps_start(xtcps_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN + 1] = { 0 };
	tchar_t sz_token[RES_LEN + 1] = { 0 };
	unsigned short port;

	if (pxp->lis_tcp)
	{
		return;
	}

	port = xstous(pxp->sz_port);

	if (IS_THREAD_MODE(pxp->sz_mode))
		pxp->lis_tcp = xtcp_start_thread(port, _xtcps_dispatch, pxp);
	else
		pxp->lis_tcp = xtcp_start_process(port, pxp->sz_module, pxp->sz_param);

	get_param_item(pxp->sz_param, _T("SITE"), sz_token, RES_LEN);

	//start failed, clean the resource
	if (!pxp->lis_tcp)
	{
		xsprintf(sz_file, _T("TCP %s service started at port: %s  mode: %s ...failed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);

		xportm_log_info(sz_file, -1);

		_xtcps_stop(pxp);
	}
	else
	{
		xsprintf(sz_file, _T("TCP %s service started at port: %s  mode: %s ...succeed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);

		xportm_log_info(sz_file, -1);
	}
}

void _xtcps_stop(xtcps_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN + 1] = { 0 };
	tchar_t sz_token[RES_LEN + 1] = { 0 };

	if (pxp->lis_tcp)
	{
		xtcp_stop(pxp->lis_tcp);
		pxp->lis_tcp = NULL;

		get_param_item(pxp->sz_param, _T("SITE"), sz_token, RES_LEN);

		xsprintf(sz_file, _T("TCP %s service at port: %s ...stoped!\r\n"), sz_token, pxp->sz_port);

		xportm_log_info(sz_file, -1);
	}
}
