
/***********************************************************************
	Easily http service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc TCP service document

	@module	xtcps.cc | TCP service implement file

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

static void _xtcps_get_config(const tchar_t* root, const tchar_t* site, tchar_t* sz_path, tchar_t* sz_trace, tchar_t* sz_proc)
{
	tchar_t sz_file[PATH_LEN] = { 0 };

	xsprintf(sz_file, _T("%s/cfg/%s.config"), root, site);

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
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("trace"), -1, 1) == 0 && sz_trace)
				{
					get_dom_node_text(nlk, sz_trace, PATH_LEN);
				}
				
				nlk = get_dom_next_sibling_node(nlk);
			}
		}
		nlk_site = get_dom_next_sibling_node(nlk_site);
	}

	destroy_xml_doc(ptr_cfg);
}

/***************************************************************************************************************/
void _xtcps_dispatch(xhand_t tcp, void* p)
{
	tchar_t sz_site[RES_LEN] = { 0 };
	tchar_t sz_virtual[PATH_LEN] = { 0 };
	tchar_t sz_trace[RES_LEN] = { 0 };
	tchar_t sz_track[PATH_LEN] = { 0 };
	tchar_t sz_proc[PATH_LEN] = { 0 };
	int n_state = 0;

	xtcps_param_t* pxp = (xtcps_param_t*)p;
	tcps_block_t *pb = NULL;
	res_modu_t api = NULL;
	PF_TCPS_INVOKE pf_invoke = NULL;

	xdate_t xdt = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };
	byte_t textbom[4] = { 0 };
	net_addr_t na = { 0 };
	short port;

	TRY_CATCH;

	get_param_item(pxp->sz_param, _T("SITE"), sz_site, RES_LEN);

	_xtcps_get_config(pxp->sz_root, sz_site, sz_virtual, sz_track, sz_proc);

	if (is_null(sz_virtual))
	{
		raise_user_error(_T("_xtcps_invoke"), _T("website not define service entry\n"));
	}

	if (is_null(sz_proc))
	{
		raise_user_error(_T("_xtcps_invoke"), _T("website not define service module\n"));
	}

	pb = (tcps_block_t*)xmem_alloc(sizeof(tcps_block_t));
	pb->cbs = sizeof(tcps_block_t);
	pb->tcp = tcp;
	pb->is_thread = IS_THREAD_MODE(pxp->sz_mode);

	pb->pf_log_title = _write_log_title;
	pb->pf_log_error = _write_log_error;
	pb->pf_log_data = _write_log_data;

	xsncpy(pb->site, sz_site, RES_LEN);
	xsncpy(pb->path, sz_virtual, PATH_LEN);

	api = load_library(sz_proc);
	if (!api)
	{
		raise_user_error(_T("_tcps_invoke"), _T("website load service module failed\n"));
	}

	pf_invoke = (PF_TCPS_INVOKE)get_address(api, _T("tcps_invoke"));
	if (!pf_invoke)
	{
		raise_user_error(_T("_tcps_invoke"), _T("website invoke module function failed\n"));
	}

	get_loc_date(&xdt);
	xsprintf(sz_trace, _T("%02d%02d%02d%02d%02d%08d"), xdt.year - 200, xdt.mon, xdt.day, xdt.hour, xdt.min, xthread_get_id());

	if (!is_null(sz_track))
	{
		xsappend(sz_track, _T("/%s.log"), sz_trace);

		pb->log = xfile_open(NULL, sz_track, FILE_OPEN_CREATE);

		if (pb->log)
		{
			xscpy(sz_trace, _T("["));
			xsocket_addr(xtcp_socket(tcp), &na);
			conv_addr(&na, &port, sz_trace + 1);
			xscat(sz_trace, _T(": "));

			(*pb->pf_log_title)(pb->log, sz_trace, -1);
		}
	}

	n_state = (*pf_invoke)(pb);

	if (pb->log)
	{
		xfile_close(pb->log);
		pb->log = NULL;

		if (!n_state)
		{
			xfile_delete(NULL, sz_track);
		}
	}

	xmem_free(pb);
	pb = NULL;

	free_library(api);
	api = NULL;

	END_CATCH;

	return;

ONERROR:

	if (pb)
	{
		if (pb->log)
		{
			xfile_close(pb->log);
			pb->log = NULL;
		}

		xmem_free(pb);
	}

	if (api)
		free_library(api);

	return;
}

void _xtcps_start(xtcps_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN] = { 0 };

	if (pxp->lis_tcp)
	{
		return;
	}

	if (xstos(pxp->sz_port) == 0)
	{
		_xtcps_stop(pxp);

		xhttpm_log_info(_T("xtcps undefined port...\r\n"), -1);
	}

	if (IS_THREAD_MODE(pxp->sz_mode))
		pxp->lis_tcp = xtcp_start_thread(xstos(pxp->sz_port), _xtcps_dispatch, pxp);
	else
		pxp->lis_tcp = xtcp_start_process(xstos(pxp->sz_port), pxp->sz_module, pxp->sz_param);

	//start failed, clean the resource
	if (!pxp->lis_tcp)
	{
		_xtcps_stop(pxp);

		xhttpm_log_info(_T("xtcp started failed...\r\n"), -1);
	}
	else
	{
		xhttpm_log_info(_T("xtcp started succeed...\r\n"), -1);
	}
}

void _xtcps_stop(xtcps_param_t* pxp)
{
	if (pxp->lis_tcp)
	{
		xtcp_stop(pxp->lis_tcp);
		pxp->lis_tcp = NULL;

		xhttpm_log_info(_T("xtcp stoped...\r\n"), -1);
	}
}
