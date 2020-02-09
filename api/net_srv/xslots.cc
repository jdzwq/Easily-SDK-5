
/***********************************************************************
	Easily slot service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc slot service document

	@module	xslots.cc | slot service implement file

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

#include "xslots.h"
#include "srvlog.h"
#include "srvcrt.h"

static void _xslots_get_config(const tchar_t* site, tchar_t* sz_path, tchar_t* sz_trace, tchar_t* sz_level, tchar_t* sz_proc)
{
	tchar_t sz_root[PATH_LEN] = { 0 };
	tchar_t sz_file[PATH_LEN] = { 0 };

	get_envvar(XSERVICE_ROOT, sz_root, PATH_LEN);

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

static void _xslots_log_request(xhand_t slot)
{
	tchar_t token[RES_LEN + 1] = { 0 };
	int len;

	xscpy(token, _T("SLOT-SCU: ["));
	len = xslen(token);

	switch (slot->tag)
	{
	case _HANDLE_SSL:
		xssl_peer_port(slot, token + len);
		break;
	case _HANDLE_SSH:
		xssh_peer_port(slot, token + len);
		break;
	case _HANDLE_TCP:
		xtcp_peer_port(slot, token + len);
		break;
	}

	len = xslen(token);
	len += xsprintf(token + len, _T(": %d]\r\n"), thread_get_id());

	xportm_log_info(token, len);
}

static void _xslots_log_response(xhand_t slot)
{
	tchar_t token[RES_LEN + 1] = { 0 };
	int len;

	xscpy(token, _T("SLOT-SCP: ["));
	len = xslen(token);

	switch (slot->tag)
	{
	case _HANDLE_SSL:
		xssl_peer_port(slot, token + len);
		break;
	case _HANDLE_SSH:
		xssh_peer_port(slot, token + len);
		break;
	case _HANDLE_TCP:
		xtcp_peer_port(slot, token + len);
		break;
	}

	len = xslen(token);
	len += xsprintf(token + len, _T(": %d]\r\n"), thread_get_id());

	xportm_log_info(token, len);
}

static void _invoke_error(xhand_t slot)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_method[INT_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	xportm_log_error(sz_code, sz_error);
}

/**********************************************************************************************************************/
void _xslots_dispatch(xhand_t slot, void* p)
{
	tchar_t sz_path[PATH_LEN] = { 0 };
	tchar_t sz_proc[PATH_LEN] = { 0 };
	tchar_t sz_track[PATH_LEN] = { 0 };
	tchar_t sz_trace[NUM_LEN] = { 0 };
	tchar_t sz_level[INT_LEN] = { 0 };
	tchar_t sz_site[RES_LEN] = { 0 };
    tchar_t sz_name[RES_LEN] = { 0 };
	tchar_t sz_cert[RES_LEN] = { 0 };
	tchar_t sz_pass[NUM_LEN] = { 0 };

	int n_state = 0;

	xslots_param_t* pxp = (xslots_param_t*)p;
	slots_block_t *pb = NULL;
	res_modu_t api = NULL;
	PF_SLOTS_INVOKE pf_invoke = NULL;

	xdate_t xdt = { 0 };

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };
	tchar_t sz_hmac[KEY_LEN + 1] = { 0 };
	byte_t textbom[4] = { 0 };

	byte_t buf_crt[X509_CERT_SIZE] = { 0 };
	byte_t buf_key[RSA_KEY_SIZE] = { 0 };
	dword_t dw;

	TRY_CATCH;

	_xslots_log_request(slot);

	get_envvar(XSERVICE_ROOT, sz_path, PATH_LEN);

	if (pxp->n_secu == _SECU_SSL)
	{
        get_param_item(pxp->sz_param, _T("CERT"), sz_cert, RES_LEN);
        get_param_item(pxp->sz_param, _T("NAME"), sz_name, RES_LEN);
		get_param_item(pxp->sz_param, _T("PASS"), sz_pass, NUM_LEN);

		dw = X509_CERT_SIZE;
		if (!get_ssl_crt(sz_path, sz_name, buf_crt, &dw))
		{
			raise_user_error(_T("_slots_invoke"), _T("http get ssl certif failed"));
		}

		xssl_set_cert(slot, buf_crt, dw);

		dw = RSA_KEY_SIZE;
		if (!get_ssl_key(sz_path, sz_name, buf_key, &dw))
		{
			raise_user_error(_T("_slots_invoke"), _T("http get ssl rsa key failed"));
		}

		xssl_set_rsa(slot, buf_key, dw, sz_pass, -1);

		if (compare_text(sz_cert, 5, _T("SSL_2"), 5, 1) == 0)
			xssl_set_auth(slot, SSL_VERIFY_REQUIRED);
		else if (compare_text(sz_cert, 5, _T("SSL_1"), 5, 1) == 0)
			xssl_set_auth(slot, SSL_VERIFY_OPTIONAL);
		else
			xssl_set_auth(slot, SSL_VERIFY_NONE);
	}
	else if (pxp->n_secu == _SECU_SSH)
	{
        get_param_item(pxp->sz_param, _T("NAME"), sz_name, RES_LEN);
		get_param_item(pxp->sz_param, _T("PASS"), sz_pass, NUM_LEN);

		dw = RSA_KEY_SIZE;
		if (!get_ssh_key(sz_path, sz_name, buf_key, &dw))
		{
			raise_user_error(_T("_slots_invoke"), _T("http get ssh rsa key failed"));
		}

		//xssh_set_rsa(slot, buf_key, dw, sz_pass, -1);
	}

	get_param_item(pxp->sz_param, _T("SITE"), sz_site, RES_LEN);

	xszero(sz_path, PATH_LEN);

	_xslots_get_config(sz_site, sz_path, sz_track, sz_level, sz_proc);

	if (is_null(sz_path))
	{
		raise_user_error(_T("_slots_invoke"), _T("website not define service entry\n"));
	}

	if (is_null(sz_proc))
	{
		raise_user_error(_T("_slots_invoke"), _T("website not define service module\n"));
	}

	pb = (slots_block_t*)xmem_alloc(sizeof(slots_block_t));
	pb->cbs = sizeof(slots_block_t);
	pb->slot = slot;
	pb->is_thread = IS_THREAD_MODE(pxp->sz_mode);

	pb->pf_log_title = _write_log_title;
	pb->pf_log_error = _write_log_error;
	pb->pf_log_data = _write_log_data;
	printf_path(pb->path, sz_path);
	xsncpy(pb->site, sz_site, RES_LEN);
	
	xszero(sz_path, PATH_LEN);
	printf_path(sz_path, sz_proc);

	api = load_library(sz_path);
	if (!api)
	{
		raise_user_error(_T("_slots_invoke"), _T("website load service module failed\n"));
	}

	pf_invoke = (PF_SLOTS_INVOKE)get_address(api, "slots_invoke");
	if (!pf_invoke)
	{
		raise_user_error(_T("_slots_invoke"), _T("website load invoke function failed\n"));
	}

	get_loc_date(&xdt);
	xsprintf(sz_trace, _T("%02d%02d%02d%02d%02d%08d"), xdt.year - 200, xdt.mon, xdt.day, xdt.hour, xdt.min, thread_get_id());

	xszero(sz_path, PATH_LEN);

	if (!is_null(sz_track))
	{
		printf_path(sz_path, sz_track);
		xsappend(sz_path, _T("/%s.log"), sz_trace);

		pb->log = xfile_open(NULL, sz_track, FILE_OPEN_CREATE);
	}

	n_state = (*pf_invoke)(pb);

	if (pb->log)
	{
		xfile_close(pb->log);
		pb->log = NULL;

		if (n_state < xstol(sz_level))
		{
			xfile_delete(NULL, sz_path);
		}
	}

	xmem_free(pb);
	pb = NULL;

	free_library(api);
	api = NULL;

	_xslots_log_response(slot);

	END_CATCH;

	return;

ONERROR:

	_invoke_error(slot);

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

	_xslots_log_response(slot);

	return;
}

void _xslots_start(xslots_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN] = { 0 };
	tchar_t sz_token[RES_LEN] = { 0 };
	unsigned short port;

	if (pxp->lis_slot)
	{
		return;
	}

	get_param_item(pxp->sz_param, _T("CERT"), sz_token, RES_LEN);

	if (compare_text(sz_token, 3, _T("SSL"), 3, 1) == 0)
		pxp->n_secu = _SECU_SSL;
	else if (compare_text(sz_token, 3, _T("SSH"), 3, 1) == 0)
		pxp->n_secu = _SECU_SSH;
	else
		pxp->n_secu = _SECU_NONE;

	port = xstous(pxp->sz_port);

	if (IS_THREAD_MODE(pxp->sz_mode))
		pxp->lis_slot = xslot_start_thread(pxp->n_secu, port, _xslots_dispatch, pxp);
	else
		pxp->lis_slot = xslot_start_process(pxp->n_secu, port, pxp->sz_module, pxp->sz_param);

	get_param_item(pxp->sz_param, _T("SITE"), sz_token, RES_LEN);

	//start failed, clean the resource
	if (!pxp->lis_slot)
	{
		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("SLOT/SSL %s service started at port: %s  mode: %s ...failed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);
		else if (pxp->n_secu == _SECU_SSH)
			xsprintf(sz_file, _T("SLOT/SSH %s service started at port: %s  mode: %s ...failed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);
		else
			xsprintf(sz_file, _T("SLOT %s service started at port: %s  mode: %s ...failed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);

		xportm_log_info(sz_file, -1);

		_xslots_stop(pxp);
	}
	else
	{
		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("SLOT/SSL %s service started at port: %s  mode: %s ...succeed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);
		else if (pxp->n_secu == _SECU_SSH)
			xsprintf(sz_file, _T("SLOT/SSH %s service started at port: %s  mode: %s ...succeed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);
		else
			xsprintf(sz_file, _T("SLOT %s service started at port: %s  mode: %s ...succeed!\r\n"), sz_token, pxp->sz_port, pxp->sz_mode);

		xportm_log_info(sz_file, -1);
	}
}

void _xslots_stop(xslots_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN] = { 0 };
	tchar_t sz_token[RES_LEN] = { 0 };

	if (pxp->lis_slot)
	{
		xslot_stop(pxp->lis_slot);
		pxp->lis_slot = NULL;

		get_param_item(pxp->sz_param, _T("SITE"), sz_token, RES_LEN);

		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("SLOT/SSL %s service at port: %s ...stoped!\r\n"), sz_token, pxp->sz_port);
		else if (pxp->n_secu == _SECU_SSH)
			xsprintf(sz_file, _T("SLOT/SSH %s service at port: %s ...stoped!\r\n"), sz_token, pxp->sz_port);
		else
			xsprintf(sz_file, _T("SLOT %s service at port: %s ...stoped!\r\n"), sz_token, pxp->sz_port);

		xportm_log_info(sz_file, -1);
	}
}
