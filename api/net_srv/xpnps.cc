
/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc PNP service document

	@module	xpnps.cc | PNP service implement file

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

#include "xpnps.h"
#include "srvlog.h"

static void _xpnps_track_error(void* hand, const tchar_t* code, const tchar_t* text)
{
	pnps_block_t* pb = (pnps_block_t*)hand;

	tchar_t token[PATH_LEN + 1] = { 0 };
	int len;

	len = xsprintf(token, _T("PNP-SCP: [%s: %d] %s %s\r\n"), pb->addr, pb->port, code, text);

	xportm_log_info(token, len);
}

static void _xpnps_get_config(const tchar_t* site, tchar_t* sz_path, tchar_t* sz_proc)
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

/***************************************************************************************************************/
void _xpnps_dispatch(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size, void* p)
{
	int n_state = 0;

	xpnps_param_t* pxp = (xpnps_param_t*)p;
	pnps_block_t *pb = NULL;
	res_modu_t api = NULL;
	PF_PNPS_INVOKE pf_invoke = NULL;

	xdate_t xdt = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	tchar_t sz_site[RES_LEN + 1] = { 0 };
	tchar_t sz_proc[PATH_LEN + 1] = { 0 };
	tchar_t sz_path[PATH_LEN + 1] = { 0 };

	trace_interface tra = { 0 };

	TRY_CATCH;

	get_param_item(pxp->sz_param, _T("SITE"), sz_site, RES_LEN);

	_xpnps_get_config(sz_site, sz_path, sz_proc);
	
	if (is_null(sz_path))
	{
		raise_user_error(_T("_xpnps_invoke"), _T("site not define service entry\n"));
	}

	if (is_null(sz_proc))
	{
		raise_user_error(_T("_xpnps_invoke"), _T("site not define service module\n"));
	}

	pb = (pnps_block_t*)xmem_alloc(sizeof(pnps_block_t));
	pb->cbs = sizeof(pnps_block_t);
	pb->is_thread = IS_THREAD_MODE(pxp->sz_mode);
	pb->port = port;
	xsncpy(pb->addr, addr, ADDR_LEN);
	if (pack)
	{
		xmem_copy((void*)pb->pack, (void*)pack, size);
		pb->size = size;
	}
	printf_path(pb->path, sz_path);

	xszero(sz_path, PATH_LEN);
	printf_path(sz_path, sz_proc);

	api = load_library(sz_path);
	if (!api)
	{
		raise_user_error(_T("_pnps_invoke"), _T("site load service module failed\n"));
	}

	pf_invoke = (PF_PNPS_INVOKE)get_address(api, "pnps_invoke");
	if (!pf_invoke)
	{
		raise_user_error(_T("_pnps_invoke"), _T("site invoke module function failed\n"));
	}

	tra.param = (void*)pb;
	tra.pf_track_error = (PF_TRACK_ERROR)_xpnps_track_error;
	pb->ptk = &tra;

	n_state = (*pf_invoke)(pb);

	free_library(api);
	api = NULL;

	xmem_free(pb);
	pb = NULL;

	END_CATCH;

	return;

ONERROR:

	get_last_error(errcode, errtext, ERR_LEN);

	if (api)
		free_library(api);

	if (pb)
	{
		_xpnps_track_error((void*)pb, errcode, errtext);

		xmem_free(pb);
	}

	return;
}

void _xpnps_start(xpnps_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN + 1] = { 0 };
	tchar_t sz_token[RES_LEN + 1] = { 0 };

	unsigned short port;

	if (pxp->lis_pnp)
	{
		return;
	}

	if (xstos(pxp->sz_port) == 0)
	{
		_xpnps_stop(pxp);

		xportm_log_info(_T("xpnp undefined port...\r\n"), -1);
	}

	port = xstous(pxp->sz_port);

	if (IS_THREAD_MODE(pxp->sz_mode))
		pxp->lis_pnp = xpnp_start_thread(port, _xpnps_dispatch, pxp);
	else
		pxp->lis_pnp = xpnp_start_process(port, pxp->sz_module, pxp->sz_param);

	get_param_item(pxp->sz_param, _T("SITE"), sz_token, RES_LEN);

	//start failed, clean the resource
	if (!pxp->lis_pnp)
	{
		_xpnps_stop(pxp);

		xsprintf(sz_file, _T("PNP %s service started at port: %s  mode: %s ...failed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);
		xportm_log_info(sz_file, -1);
	}
	else
	{
		xsprintf(sz_file, _T("PNP %s service started at port: %s  mode: %s ...succeed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);
		xportm_log_info(sz_file, -1);
	}
}

void _xpnps_stop(xpnps_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN + 1] = { 0 };
	tchar_t sz_token[RES_LEN + 1] = { 0 };

	if (pxp->lis_pnp)
	{
		get_param_item(pxp->sz_param, _T("SITE"), sz_token, RES_LEN);

		xpnp_stop(pxp->lis_pnp);
		pxp->lis_pnp = NULL;

		xsprintf(sz_file, _T("PNP %s service at port: %s site: %s stoped...\r\n"), sz_token, pxp->sz_port);

		xportm_log_info(sz_file, -1);
	}
}
