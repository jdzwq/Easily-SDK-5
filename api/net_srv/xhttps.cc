
/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc http service document

	@module	xhttps.cc | http service implement file

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

#include "xhttps.h"
#include "srvlog.h"
#include "srvcrt.h"

static void _xhttps_get_config(const tchar_t* site, tchar_t* sz_space, tchar_t* sz_path, tchar_t* sz_track, tchar_t* sz_level, tchar_t* sz_proc)
{
	tchar_t sz_root[PATH_LEN] = { 0 };
	tchar_t sz_file[PATH_LEN] = { 0 };

	get_envvar(XSERVICE_ROOT, sz_root, PATH_LEN);
	if(is_null(sz_root))
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
				if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("namespace"), -1, 1) == 0 && sz_path)
				{
					get_dom_node_text(nlk, sz_space, RES_LEN);
				}
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("path"), -1, 1) == 0 && sz_path)
				{
					get_dom_node_text(nlk, sz_path, PATH_LEN);
				}
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("proc"), -1, 1) == 0 && sz_proc)
				{
					get_dom_node_text(nlk, sz_proc, PATH_LEN);
				}
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("track"), -1, 1) == 0 && sz_track)
				{
					get_dom_node_text(nlk, sz_track, PATH_LEN);
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

static void _xhttps_log_request(xhand_t http)
{
	tchar_t token[RES_LEN + 1] = { 0 };
	int len;
	byte_t* sz_buf = NULL;
	dword_t size = 0;

	xscpy(token, _T("["));
	xhttp_peer_port(http, token + 1);
	len = xslen(token);
	len += xsprintf(token + len, _T(" :%d]\r\n"), thread_get_id());

	xportm_log_info(token, len);

	size = xhttp_format_request(http, NULL, MAX_LONG);
	sz_buf = (byte_t*)xmem_alloc(size + 1);
	xhttp_format_request(http, sz_buf, size);

	xportm_log_data(sz_buf, size);

	xmem_free(sz_buf);
	sz_buf = NULL;
}

static void _xhttps_log_response(xhand_t http)
{
	tchar_t token[RES_LEN + 1] = { 0 };
	int len;
	byte_t* sz_buf = NULL;
	dword_t size = 0;

	xscpy(token, _T("["));
	xhttp_peer_port(http, token + 1);
	len = xslen(token);
	len += xsprintf(token + len, _T(" :%d]\r\n"), thread_get_id());

	xportm_log_info(token, len);

	size = xhttp_format_response(http, NULL, MAX_LONG);
	sz_buf = (byte_t*)xmem_alloc(size + 1);
	xhttp_format_response(http, sz_buf, size);

	xportm_log_data(sz_buf, size);

	xmem_free(sz_buf);
	sz_buf = NULL;
}

static bool_t _xhttps_licence(xhand_t http, const tchar_t* site, tchar_t* hmac)
{
	tchar_t sz_auth[INT_LEN + 1] = { 0 };
	tchar_t sz_sid[RES_LEN + 1] = { 0 };
	tchar_t sz_key[MD5_LEN + 1] = { 0 };
	tchar_t sz_sign[KEY_LEN + 1] = { 0 };
	tchar_t sz_hmac[KEY_LEN + 1] = { 0 };
	int count = 0;

	tchar_t sz_root[PATH_LEN] = { 0 };
	tchar_t sz_file[PATH_LEN] = { 0 };
	link_t_ptr nlk_root,nlk_lic, nlk_node, ptr_xml = NULL;

	xhttp_get_authorization(http, sz_auth, sz_sid, RES_LEN, sz_sign, KEY_LEN);

	get_envvar(XSERVICE_ROOT, sz_root, PATH_LEN);
	if(is_null(sz_root))
	{
		//xscpy(sz_root,_T("."));
		get_runpath((res_modu_t)0, sz_root, PATH_LEN);
	}
	
	xsprintf(sz_file, _T("%s/lic/%s/%s.lic"), sz_root, site, sz_sid);

	ptr_xml = create_xml_doc();
	if (!load_xml_doc_from_file(ptr_xml, NULL, sz_file))
	{
		destroy_xml_doc(ptr_xml);
		return 0;
	}

	nlk_root = get_xml_dom_node(ptr_xml);
	nlk_lic = get_dom_first_child_node(nlk_root);
	while (nlk_lic)
	{
		nlk_node = get_dom_first_child_node(nlk_lic);
		while (nlk_node)
		{
			if (compare_text(get_dom_node_name_ptr(nlk_node), -1, _T("PRIVATE-KEY"), -1, 1) == 0)
			{
				get_dom_node_text(nlk_node, sz_key, MD5_LEN);

				xhttp_request_signature(http, sz_auth, sz_key, sz_hmac, KEY_LEN);

				if (xscmp(sz_sign, sz_hmac) == 0)
				{
					destroy_xml_doc(ptr_xml);
					return 1;
				}
				else
				{
					xscpy(hmac, sz_hmac);
				}
			}

			nlk_node = get_dom_next_sibling_node(nlk_node);
		}

		count++;
		nlk_lic = get_dom_next_sibling_node(nlk_lic);
	}

	destroy_xml_doc(ptr_xml);

	if (count > 1)
	{
		xscpy(hmac, _T(""));
	}

	return 0;
}

static void _invoke_error(xhand_t http)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_method[INT_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	xportm_log_error(sz_code, sz_error);

	xhttp_get_url_method(http, sz_method, INT_LEN);

	if (!xhttp_need_expect(http) && (compare_text(sz_method, -1, HTTP_METHOD_PUT, -1, 1) == 0 || compare_text(sz_method, -1, HTTP_METHOD_POST, -1, 1) == 0))
	{
		d_recv = bytes_alloc();
		n_size = 0;

		xhttp_recv_full(http, d_recv, &n_size);

		bytes_free(d_recv);
		d_recv = NULL;
	}

	xhttp_set_response_default_header(http);

	xhttp_send_error(http, NULL, NULL, sz_code, sz_error, -1);
}

/**********************************************************************************************************************/
void _xhttps_dispatch(xhand_t http, void* p)
{
	tchar_t sz_method[RES_LEN + 1] = { 0 };
	tchar_t sz_object[PATH_LEN] = { 0 };
	tchar_t sz_site[RES_LEN] = { 0 };
	tchar_t sz_res[PATH_LEN] = { 0 };
	tchar_t sz_path[PATH_LEN] = { 0 };
	tchar_t sz_proc[PATH_LEN] = { 0 };
	tchar_t sz_track[PATH_LEN] = { 0 };
	tchar_t sz_trace[NUM_LEN] = { 0 };
	tchar_t sz_space[RES_LEN] = { 0 };
	tchar_t sz_level[INT_LEN] = { 0 };
	tchar_t sz_auth[INT_LEN] = { 0 };
	tchar_t sz_cert[RES_LEN] = { 0 };
    tchar_t sz_name[RES_LEN] = { 0 };
	tchar_t sz_pass[NUM_LEN] = { 0 };
	int n_state = 0;

	xhttps_param_t* pxp = (xhttps_param_t*)p;
	https_block_t *pb = NULL;
	res_modu_t api = NULL;
	PF_HTTPS_INVOKE pf_invoke = NULL;

	xdate_t xdt = { 0 };

	tchar_t errtext[ERR_LEN + 1] = { 0 };
	tchar_t signature[KEY_LEN + 1] = { 0 };

    byte_t* buf_crt = NULL;
    byte_t* buf_key = NULL;
	dword_t dw;

	xhand_t bio = NULL;

	TRY_CATCH;

	bio = xhttp_bio(http);

	get_envvar(XSERVICE_ROOT, sz_path, PATH_LEN);

	if (pxp->n_secu == _SECU_SSL)
	{
        get_param_item(pxp->sz_param, _T("CERT"), sz_cert, RES_LEN);
        get_param_item(pxp->sz_param, _T("NAME"), sz_name, RES_LEN);
		get_param_item(pxp->sz_param, _T("PASS"), sz_pass, NUM_LEN);

		dw = X509_CERT_SIZE;
        buf_crt = (byte_t*)xmem_alloc(dw);
        
		if (!get_ssl_crt(sz_path, sz_name, buf_crt, &dw))
		{
			raise_user_error(_T("_https_invoke"), _T("http get ssl certif failed"));
		}

		xssl_set_cert(bio, buf_crt, dw);
        
        xmem_free(buf_crt);
        buf_crt = NULL;

		dw = RSA_KEY_SIZE;
        buf_key = (byte_t*)xmem_alloc(dw);
        
		if (!get_ssl_key(sz_path, sz_name, buf_key, &dw))
		{
			raise_user_error(_T("_https_invoke"), _T("http get ssl rsa key failed"));
		}

		xssl_set_rsa(bio, buf_key, dw, sz_pass, -1);
        
        xmem_free(buf_key);
        buf_key = NULL;

		if (compare_text(sz_cert, 5, _T("SSL_2"), 5, 1) == 0)
			xssl_set_auth(bio, SSL_VERIFY_REQUIRED);
		else if (compare_text(sz_cert, 5, _T("SSL_1"), 5, 1) == 0)
			xssl_set_auth(bio, SSL_VERIFY_OPTIONAL);
		else
			xssl_set_auth(bio, SSL_VERIFY_NONE);
	}
	else if (pxp->n_secu == _SECU_SSH)
	{
        get_param_item(pxp->sz_param, _T("NAME"), sz_name, RES_LEN);
		get_param_item(pxp->sz_param, _T("PASS"), sz_pass, NUM_LEN);

		dw = RSA_KEY_SIZE;
        buf_key = (byte_t*)xmem_alloc(dw);
        
		if (!get_ssh_key(sz_path, sz_name, buf_key, &dw))
		{
			raise_user_error(_T("_https_invoke"), _T("http get ssh rsa key failed"));
		}

		//xssh_set_rsa(bio, buf_key, dw, sz_pass, -1);
        
        xmem_free(buf_key);
        buf_key = NULL;
	}

	xszero(sz_path, PATH_LEN);
	
	if (!xhttp_recv_request(http))
	{
		xhttp_set_response_code(http, HTTP_CODE_400);
		xhttp_set_response_message(http, HTTP_CODE_400_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	xhttp_get_url_method(http, sz_method, RES_LEN);
	xhttp_get_url_object(http, sz_object, PATH_LEN);

	if (is_null(sz_object))
	{
		xhttp_set_response_code(http, HTTP_CODE_404);
		xhttp_set_response_message(http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(_T("_https_invoke"), _T("request header object is empty"));
	}

	xhttp_split_object(sz_object, sz_site, sz_res);

	if (is_null(sz_site))
	{
		xscat(sz_site, _T("/"));
		xscat(sz_site, XHTTPS_DEFAULT_SITE);
	}
	else if ((sz_site[0] == _T('/') || sz_site[0] == _T('\\')) && sz_site[1] == _T('\0'))
	{
		xscat(sz_site, XHTTPS_DEFAULT_SITE);
	}

	_xhttps_get_config(sz_site + 1, sz_space, sz_path, sz_track, sz_level, sz_proc);

	if (is_null(sz_path))
	{
		xhttp_set_response_code(http, HTTP_CODE_404);
		xhttp_set_response_message(http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(sz_site, _T("website not define service entry\n"));
	}

	if (is_null(sz_proc))
	{
		xhttp_set_response_code(http, HTTP_CODE_404);
		xhttp_set_response_message(http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(sz_site, _T("website not define service module\n"));
	}

	get_param_item(pxp->sz_param, _T("AUTH"), sz_auth, INT_LEN);

	if (compare_text(sz_auth,-1,HTTP_HEADER_AUTHORIZATION_XDS,-1,1) == 0)
	{
		if (!_xhttps_licence(http, sz_site + 1, signature))
		{
			xhttp_set_response_code(http, HTTP_CODE_401);
			xhttp_set_response_message(http, HTTP_CODE_401_TEXT, -1);
			 
			if (is_null(signature))
				xscpy(errtext, _T("request header signature is invalid"));
			else
				xsprintf(errtext, _T("request header signature is %s"), signature);

			raise_user_error(sz_site, errtext);
		}
	}

	pb = (https_block_t*)xmem_alloc(sizeof(https_block_t));
	pb->cbs = sizeof(https_block_t);
	pb->http = http;
	pb->is_thread = IS_THREAD_MODE(pxp->sz_mode);

	pb->pf_log_title = _write_log_title;
	pb->pf_log_error = _write_log_error;
	pb->pf_log_data = _write_log_data;
	pb->pf_log_xml = _write_log_xml;
	pb->pf_log_json = _write_log_json;

	xsncpy(pb->site, sz_site + 1, RES_LEN);
	xsncpy(pb->space, sz_space, RES_LEN);
	xsncpy(pb->object, sz_res, PATH_LEN);
	printf_path(pb->path, sz_path);

	xszero(sz_path, PATH_LEN);
	printf_path(sz_path, sz_proc);

	api = load_library(sz_path);
	if (!api)
	{
		xhttp_set_response_code(http, HTTP_CODE_404);
		xhttp_set_response_message(http, HTTP_CODE_404_TEXT, -1);
		raise_user_error(_T("_https_invoke"), _T("website load service module failed\n"));
	}

	pf_invoke = (PF_HTTPS_INVOKE)get_address(api, "https_invoke");
	if (!pf_invoke)
	{
		xhttp_set_response_code(http, HTTP_CODE_404);
		xhttp_set_response_message(http, HTTP_CODE_404_TEXT, -1);
		raise_user_error(_T("_https_invoke"), _T("website load module function failed\n"));
	}

	xhttp_set_response_default_header(http);

	get_loc_date(&xdt);
	xsprintf(sz_trace, _T("%02d%02d%02d%02d%02d%08d"), xdt.year - 200, xdt.mon, xdt.day, xdt.hour, xdt.min, thread_get_id());

	xhttp_set_response_header(http, HTTP_HEADER_TRACE, -1, sz_trace, -1);

    xszero(sz_path, PATH_LEN);
    
	if (!is_null(sz_track))
	{
        printf_path(sz_path, sz_track);
		xsappend(sz_path, _T("/%s.log"), sz_trace);

		pb->log = xfile_open(NULL, sz_path, FILE_OPEN_CREATE);

		if (pb->log)
		{
			xscpy(sz_res, _T("["));
			xhttp_addr_port(http, sz_res + 1);
			xscat(sz_res, _T("]"));

			(*pb->pf_log_title)(pb->log, sz_res, -1);
		}
	}

	n_state = (*pf_invoke)(sz_method, pb);

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

	_xhttps_log_request(http);

	_xhttps_log_response(http);

	END_CATCH;

	return;

ONERROR:

	_invoke_error(http);

	if (pb)
	{
		if (pb->log)
		{
			xfile_close(pb->log);
			pb->log = NULL;
		}

		xmem_free(pb);
	}
    
    if(buf_crt)
        xmem_free(buf_crt);
    
    if(buf_key)
        xmem_free(buf_key);

	if (api)
		free_library(api);

	_xhttps_log_request(http);

	_xhttps_log_response(http);

	return;
}

void _xhttps_start(xhttps_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN] = { 0 };
	tchar_t sz_auth[INT_LEN] = { 0 };
	unsigned short port;

	if (pxp->lis_http)
	{
		return;
	}

	get_param_item(pxp->sz_param, _T("CERT"), sz_file, RES_LEN);
	get_param_item(pxp->sz_param, _T("AUTH"), sz_auth, INT_LEN);

	if (compare_text(sz_file, 3, _T("SSL"), 3, 1) == 0)
		pxp->n_secu = _SECU_SSL;
	else if (compare_text(sz_file, 3, _T("SSH"), 3, 1) == 0)
		pxp->n_secu = _SECU_SSH;
	else
		pxp->n_secu = _SECU_NONE;
    
	port = xstous(pxp->sz_port);

	if (IS_THREAD_MODE(pxp->sz_mode))
		pxp->lis_http = xhttp_start_thread(pxp->n_secu, port, _xhttps_dispatch, pxp);
	else
		pxp->lis_http = xhttp_start_process(pxp->n_secu, port, pxp->sz_module, pxp->sz_param);

	//start failed, clean the resource
	if (!pxp->lis_http)
	{
		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("HTTP/ssl service started at port: %s  ...failed!\r\n"), pxp->sz_port);
		else if (pxp->n_secu == _SECU_SSH)
			xsprintf(sz_file, _T("HTTP/ssh service started at port: %s ...failed!\r\n"), pxp->sz_port);
		else
			xsprintf(sz_file, _T("HTTP service at port: %s ...failed!\r\n"), pxp->sz_port);

		xportm_log_info(sz_file, -1);

		_xhttps_stop(pxp);
	}
	else
	{
		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("HTTP/ssl service started at port: %s  mode: %s authorization: %s ...succeed!\r\n"), pxp->sz_port, pxp->sz_mode, sz_auth);
		else if (pxp->n_secu == _SECU_SSH)
			xsprintf(sz_file, _T("HTTP/ssh service started at port: %s  mode: %s authorization: %s ...succeed!\r\n"), pxp->sz_port, pxp->sz_mode, sz_auth);
		else
			xsprintf(sz_file, _T("HTTP service started at port: %s  mode: %s authorization: %s ...succeed!\r\n"), pxp->sz_port, pxp->sz_mode, sz_auth);

		xportm_log_info(sz_file, -1);
	}
}

void _xhttps_stop(xhttps_param_t* pxp)
{
	tchar_t sz_file[PATH_LEN] = { 0 };

	if (pxp->lis_http)
	{
		xhttp_stop(pxp->lis_http);
		pxp->lis_http = NULL;

		if (pxp->n_secu == _SECU_SSL)
			xsprintf(sz_file, _T("HTTP/ssl service at port: %s ...stoped!\r\n"), pxp->sz_port);
		else if (pxp->n_secu == _SECU_SSH)
			xsprintf(sz_file, _T("HTTP/ssh service at port: %s ...stoped!\r\n"), pxp->sz_port);
		else
			xsprintf(sz_file, _T("HTTP service at port: %s ...stoped!\r\n"), pxp->sz_port);

		xportm_log_info(sz_file, -1);
	}
}
