
/***********************************************************************
	Easily pacs service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc pacs service document

	@module	xpacss.cc | pacs service implement file

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

#include "xpacss.h"
#include "srvlog.h"

static void _xpacss_get_config(const tchar_t* root, const tchar_t* site, tchar_t* sz_path, tchar_t* sz_trace, tchar_t* sz_level, tchar_t* sz_proc)
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
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("level"), -1, 1) == 0 && sz_level)
				{
					get_dom_node_text(nlk, sz_level, INT_LEN);
				}
				
				nlk = get_dom_next_sibling_node(nlk);
			}
		}
		nlk_site = get_dom_next_sibling_node(nlk_site);
	}

	destroy_xml_doc(ptr_cfg);
}

static void _xpacss_log_request(xhand_t pacs)
{
	tchar_t token[RES_LEN + 1] = { 0 };
	int len;

	xscpy(token, _T("PACS-SCU: ["));
	len = xslen(token);
	xpacs_addr_port(pacs, token + len);
	len = xslen(token);
	len += xsprintf(token + len, _T(": %d]\r\n"), xthread_get_id());

	xhttpm_log_info(token, len);
}

static void _xpacss_log_response(xhand_t pacs)
{
	tchar_t token[RES_LEN + 1] = { 0 };
	int len;

	xscpy(token, _T("PACS-SCP: ["));
	len = xslen(token);
	xpacs_addr_port(pacs, token + len);
	len = xslen(token);
	len += xsprintf(token + len, _T(": %d]\r\n"), xthread_get_id());

	xhttpm_log_info(token, len);
}

static void _invoke_error(xhand_t pacs)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_method[INT_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttpm_log_error(sz_code, sz_error);

	//xpacs_send_error(pacs, NULL, NULL, sz_code, sz_error, -1);
}

/**********************************************************************************************************************/
void _xpacss_dispatch(xhand_t pacs, void* p)
{
	tchar_t sz_virtual[PATH_LEN] = { 0 };
	tchar_t sz_proc[PATH_LEN] = { 0 };
	tchar_t sz_track[PATH_LEN] = { 0 };
	tchar_t sz_trace[NUM_LEN] = { 0 };
	tchar_t sz_level[INT_LEN] = { 0 };
	tchar_t sz_site[RES_LEN] = { 0 };
	tchar_t sz_cert[RES_LEN] = { 0 };

	int n_state = 0;

	xpacss_param_t* pxp = (xpacss_param_t*)p;
	pacss_block_t *pb = NULL;
	res_modu_t api = NULL;
	PF_PACSS_INVOKE pf_invoke = NULL;

	xdate_t xdt = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };
	tchar_t sz_hmac[NAME_LEN + 1] = { 0 };
	byte_t textbom[4] = { 0 };

	xhand_t bio;

	TRY_CATCH;

	bio = xpacs_bio(pacs);

	_xpacss_log_request(pacs);

	get_param_item(pxp->sz_param, _T("SITE"), sz_site, RES_LEN);

	_xpacss_get_config(pxp->sz_root, sz_site, sz_virtual, sz_track, sz_level, sz_proc);

	if (is_null(sz_virtual))
	{
		raise_user_error(_T("_pacss_invoke"), _T("website not define service entry\n"));
	}

	if (is_null(sz_proc))
	{
		raise_user_error(_T("_pacss_invoke"), _T("website not define service module\n"));
	}

	if (pxp->n_secu == _SECU_SSL)
	{
		set_certs(_SECU_SSL, bio);

		get_param_item(pxp->sz_param, _T("CERT"), sz_cert, RES_LEN);

		if (compare_text(sz_cert, 5, _T("SSL_2"), 5, 1) == 0)
			xssl_set_auth(bio, SSL_VERIFY_REQUIRED);
		else if (compare_text(sz_cert, 5, _T("SSL_1"), 5, 1) == 0)
			xssl_set_auth(bio, SSL_VERIFY_OPTIONAL);
		else
			xssl_set_auth(bio, SSL_VERIFY_NONE);
	}
	else if (pxp->n_secu == _SECU_TLS)
	{
		set_certs(_SECU_TLS, bio);
	}

	pb = (pacss_block_t*)xmem_alloc(sizeof(pacss_block_t));
	pb->cbs = sizeof(pacss_block_t);
	pb->pacs = pacs;
	pb->is_thread = IS_THREAD_MODE(pxp->sz_mode);

	pb->pf_log_title = _write_log_title;
	pb->pf_log_error = _write_log_error;
	pb->pf_log_data = _write_log_data;
	pb->pf_log_xml = _write_log_xml;

	xsncpy(pb->site, sz_site, RES_LEN);
	xsncpy(pb->path, sz_virtual, PATH_LEN);

	api = load_library(sz_proc);
	if (!api)
	{
		raise_user_error(_T("_pacss_invoke"), _T("website load service module failed\n"));
	}

	pf_invoke = (PF_PACSS_INVOKE)get_address(api, _T("pacss_invoke"));
	if (!pf_invoke)
	{
		raise_user_error(_T("_pacss_invoke"), _T("website load invoke function failed\n"));
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
			xpacs_addr_port(pacs, sz_trace + 1);
			xscat(sz_trace, _T("]"));

			(*pb->pf_log_title)(pb->log, sz_trace, -1);
		}
	}

	n_state = (*pf_invoke)(pb);

	if (pb->log)
	{
		xfile_close(pb->log);
		pb->log = NULL;

		if (n_state < xstol(sz_level))
		{
			xfile_delete(NULL, sz_track);
		}
	}

	xmem_free(pb);
	pb = NULL;

	free_library(api);
	api = NULL;

	_xpacss_log_response(pacs);

	END_CATCH;

	return;

ONERROR:

	_invoke_error(pacs);

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

	_xpacss_log_response(pacs);

	return;
}

void _xpacss_start(xpacss_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN] = { 0 };

	if (pxp->lis_pacs)
	{
		return;
	}

	get_param_item(pxp->sz_param, _T("CERT"), sz_file, RES_LEN);

	if (compare_text(sz_file, 3, _T("SSL"), 3, 1) == 0)
		pxp->n_secu = _SECU_SSL;
	else if (compare_text(sz_file, 3, _T("TLS"), 3, 1) == 0)
		pxp->n_secu = _SECU_TLS;
	else
		pxp->n_secu = _SECU_NONE;

	pxp->p_certs = alloc_certs(pxp->n_secu, pxp->sz_root);

	if (IS_THREAD_MODE(pxp->sz_mode))
		pxp->lis_pacs = xpacs_start_thread(pxp->n_secu, xstos(pxp->sz_port), _xpacss_dispatch, pxp);
	else
		pxp->lis_pacs = xpacs_start_process(pxp->n_secu, xstos(pxp->sz_port), pxp->sz_module, pxp->sz_param);

	//start failed, clean the resource
	if (!pxp->lis_pacs)
	{
		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("PACS/SSL service started at port: %s  mode: %s root: %s ...failed!\r\n"), pxp->sz_port, pxp->sz_mode, pxp->sz_root);
		else if (pxp->n_secu == _SECU_TLS)
			xsprintf(sz_file, _T("PACS/TLS service started at port: %s  mode: %s root: %s ...failed!\r\n"), pxp->sz_port, pxp->sz_mode, pxp->sz_root);
		else
			xsprintf(sz_file, _T("PACS service started at port: %s  mode: %s root: %s ...failed!\r\n"), pxp->sz_port, pxp->sz_mode, pxp->sz_root);

		xhttpm_log_info(sz_file, -1);

		_xpacss_stop(pxp);
	}
	else
	{
		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("PACS/SSL service started at port: %s  mode: %s root: %s ...succeed!\r\n"), pxp->sz_port, pxp->sz_mode, pxp->sz_root);
		else if (pxp->n_secu == _SECU_TLS)
			xsprintf(sz_file, _T("PACS/TLS service started at port: %s  mode: %s root: %s ...succeed!\r\n"), pxp->sz_port, pxp->sz_mode, pxp->sz_root);
		else
			xsprintf(sz_file, _T("PACS service started at port: %s  mode: %s root: %s ...succeed!\r\n"), pxp->sz_port, pxp->sz_mode, pxp->sz_root);

		xhttpm_log_info(sz_file, -1);
	}
}

void _xpacss_stop(xpacss_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN] = { 0 };

	if (pxp->lis_pacs)
	{
		xpacs_stop(pxp->lis_pacs);
		pxp->lis_pacs = NULL;

		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("PACS/SSL at port: %s ...stoped!\r\n"), pxp->sz_port);
		else if (pxp->n_secu == _SECU_TLS)
			xsprintf(sz_file, _T("PACS/TLS at port: %s ...stoped!\r\n"), pxp->sz_port);
		else
			xsprintf(sz_file, _T("PACS at port: %s ...stoped!\r\n"), pxp->sz_port);

		xhttpm_log_info(sz_file, -1);
	}

	if (pxp->p_certs)
	{
		free_certs(pxp->p_certs);
		pxp->p_certs = NULL;
	}
}
