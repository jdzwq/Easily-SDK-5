/***********************************************************************
	Easily weiisp weiisp

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc weiisp weiisp document

	@module	weiisp_weiisp.c | weiisp weiisp implement file

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

#include "weiisp_api.h"

typedef struct _weiisp_block_t{
	tchar_t session_url[PATH_LEN];
	tchar_t access_url[PATH_LEN];

	tchar_t appid[RES_LEN];
	tchar_t secret[KEY_LEN];
}weiisp_block_t;

bool_t _invoke_weiisp_session(const https_block_t* pb, weiisp_block_t* pd)
{
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };
	tchar_t sz_method[RES_LEN] = { 0 };

	xhand_t xh_weiisp = NULL;

	tchar_t sz_jscode[KEY_LEN] = { 0 };

	tchar_t sz_openid[KEY_LEN] = { 0 };
	tchar_t sz_session_key[KEY_LEN] = { 0 };
	tchar_t sz_unionid[KEY_LEN] = { 0 };

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk;

	TRY_CATCH;

	xhttp_get_url_method(pb->http, sz_method, INT_LEN);

	if (compare_text(sz_method, -1, HTTP_METHOD_GET, -1, 1) == 0)
	{
		xhttp_get_url_query_entity(pb->http, _T("jscode"), -1, sz_jscode, KEY_LEN);
	}
	else
	{
		ptr_json = create_json_doc();

		if (!xhttp_recv_json(pb->http, ptr_json))
		{
			raise_user_error(NULL, NULL);
		}

		nlk = get_dom_first_child_node(ptr_json);
		while (nlk)
		{
			if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("jscode"), -1, 1) == 0)
				get_dom_node_text(nlk, sz_jscode, KEY_LEN);

			nlk = get_dom_next_sibling_node(nlk);
		}

		destroy_json_doc(ptr_json);
		ptr_json = NULL;
	}

	if (is_null(sz_jscode))
	{
		raise_user_error(_T("_invoke_weiisp_session"), _T("weiisp login code not allow empty"));
	}

	xh_weiisp = xhttp_client(HTTP_METHOD_GET, pd->session_url);
	if (!xh_weiisp)
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_set_request_default_header(xh_weiisp);
	xhttp_set_request_content_type(xh_weiisp, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_url_query_entity(xh_weiisp, _T("appid"), -1, pd->appid, -1);
	xhttp_set_url_query_entity(xh_weiisp, _T("secret"), -1, pd->secret, -1);
	xhttp_set_url_query_entity(xh_weiisp, _T("js_code"), -1, sz_jscode, -1);
	xhttp_set_url_query_entity(xh_weiisp, _T("grant_type"), -1, _T("authorization_code"), -1);

	if (!xhttp_send_request(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	ptr_json = create_json_doc();

	if (!xhttp_recv_json(xh_weiisp, ptr_json))
	{
		raise_user_error(_T("_invoke_weiisp_access"), _T("parse weiisp json failed"));
	}

	xhttp_close(xh_weiisp);
	xh_weiisp = NULL;

	nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		if (compare_text(get_json_item_name_ptr(nlk), -1, _T("openid"), -1, 1) == 0)
			get_json_item_value(nlk, sz_openid, KEY_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("session_key"), -1, 1) == 0)
			get_json_item_value(nlk, sz_session_key, KEY_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("unionid"), -1, 1) == 0)
			get_json_item_value(nlk, sz_unionid, KEY_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("errcode"), -1, 1) == 0)
			get_json_item_value(nlk, sz_num, NUM_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("errmsg"), -1, 1) == 0)
			get_json_item_value(nlk, sz_err, ERR_LEN);

		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	if (!is_null(sz_num) && xstol(sz_num) != 0)
	{
		raise_user_error(_T("_invoke_weiisp_access"), sz_err);
	}

	//返回应答数据包
	ptr_json = create_json_doc();

	nlk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(nlk, _T("session_key"));
	set_json_item_value(nlk, sz_session_key);

	nlk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(nlk, _T("openid"));
	set_json_item_value(nlk, sz_openid);

	nlk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(nlk, _T("unionid"));
	set_json_item_value(nlk, sz_unionid);

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CACHECONTROL, -1, HTTP_HEADER_CACHECONTROL_NOSTORE, -1);

	if (!xhttp_send_json(pb->http, ptr_json))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[交易应答]"), -1);

		(*pb->pf_log_json)(pb->log, ptr_json);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	END_CATCH;

	return 1;

ONERROR:
	XDL_TRACE_LAST;

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (xh_weiisp)
		xhttp_close(xh_weiisp);

	return 0;
}

bool_t _invoke_weiisp_access(const https_block_t* pb, weiisp_block_t* pd)
{
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };
	tchar_t sz_method[RES_LEN] = { 0 };

	xhand_t xh_weiisp = NULL;

	tchar_t sz_token[KEY_LEN] = { 0 };
	tchar_t sz_expires[INT_LEN] = { 0 };

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk;

	TRY_CATCH;

	xhttp_get_url_method(pb->http, sz_method, INT_LEN);

	xh_weiisp = xhttp_client(HTTP_METHOD_GET, pd->access_url);
	if (!xh_weiisp)
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_set_request_default_header(xh_weiisp);
	xhttp_set_request_content_type(xh_weiisp, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_url_query_entity(xh_weiisp, _T("grant_type"), -1, _T("client_credential"), -1);
	xhttp_set_url_query_entity(xh_weiisp, _T("appid"), -1, pd->appid, -1);
	xhttp_set_url_query_entity(xh_weiisp, _T("secret"), -1, pd->secret, -1);

	if (!xhttp_send_request(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	ptr_json = create_json_doc();

	if (!xhttp_recv_json(xh_weiisp, ptr_json))
	{
		raise_user_error(_T("_invoke_weiisp_access"), _T("parse weiisp json failed"));
	}

	xhttp_close(xh_weiisp);
	xh_weiisp = NULL;

	nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		if (compare_text(get_json_item_name_ptr(nlk), -1, _T("access_token"), -1, 1) == 0)
			get_json_item_value(nlk, sz_token, KEY_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("expires_in"), -1, 1) == 0)
			get_json_item_value(nlk, sz_expires, KEY_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("errcode"), -1, 1) == 0)
			get_json_item_value(nlk, sz_num, NUM_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("errmsg"), -1, 1) == 0)
			get_json_item_value(nlk, sz_err, ERR_LEN);

		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	if (!is_null(sz_num) && xstol(sz_num) != 0)
	{
		raise_user_error(_T("_invoke_weiisp_access"), sz_err);
	}

	//返回应答数据包
	ptr_json = create_json_doc();

	nlk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(nlk, _T("access_token"));
	set_json_item_value(nlk, sz_token);

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CACHECONTROL, -1, HTTP_HEADER_CACHECONTROL_NOSTORE, -1);

	if (!xhttp_send_json(pb->http, ptr_json))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[交易应答]"), -1);

		(*pb->pf_log_json)(pb->log, ptr_json);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	END_CATCH;

	return 1;

ONERROR:
	XDL_TRACE_LAST;

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (xh_weiisp)
		xhttp_close(xh_weiisp);

	return 0;
}

bool_t _invoke_weiisp_phone(const https_block_t* pb, weiisp_block_t* pd)
{
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };
	tchar_t sz_method[RES_LEN] = { 0 };

	tchar_t sz_code[KEY_LEN] = { 0 };
	tchar_t sz_data[1024] = { 0 };
	tchar_t sz_iv[KEY_LEN] = { 0 };
	tchar_t sz_cloud[KEY_LEN] = { 0 };

	xhand_t xh_weiisp = NULL;

	tchar_t sz_openid[KEY_LEN] = { 0 };
	tchar_t sz_session_key[KEY_LEN] = { 0 };
	tchar_t sz_unionid[KEY_LEN] = { 0 };

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk;

	tchar_t sz_phone[INT_LEN] = { 0 };
	tchar_t sz_pure[INT_LEN] = { 0 };
	tchar_t sz_country[INT_LEN] = { 0 };

	TRY_CATCH;

	xhttp_get_url_method(pb->http, sz_method, INT_LEN);

	if (compare_text(sz_method, -1, HTTP_METHOD_GET, -1, 1) == 0)
	{
		xhttp_get_url_query_entity(pb->http, _T("jscode"), -1, sz_code, KEY_LEN);
		xhttp_get_url_query_entity(pb->http, _T("encryptedData"), -1, sz_data, 1024);
		xhttp_get_url_query_entity(pb->http, _T("iv"), -1, sz_iv, KEY_LEN);
		xhttp_get_url_query_entity(pb->http, _T("cloudID"), -1, sz_cloud, KEY_LEN);
	}
	else
	{
		ptr_json = create_json_doc();

		if (!xhttp_recv_json(pb->http, ptr_json))
		{
			raise_user_error(NULL, NULL);
		}

		nlk = get_dom_first_child_node(ptr_json);
		while (nlk)
		{
			if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("jscode"), -1, 1) == 0)
				get_dom_node_text(nlk, sz_code, KEY_LEN);
			else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("encryptedData"), -1, 1) == 0)
				get_dom_node_text(nlk, sz_data, 1024);
			else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("iv"), -1, 1) == 0)
				get_dom_node_text(nlk, sz_iv, KEY_LEN);
			else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("cloudID"), -1, 1) == 0)
				get_dom_node_text(nlk, sz_cloud, KEY_LEN);

			nlk = get_dom_next_sibling_node(nlk);
		}

		destroy_json_doc(ptr_json);
		ptr_json = NULL;
	}

	if (is_null(sz_code))
	{
		raise_user_error(_T("_invoke_weiisp_access"), _T("weiisp login code not allow empty"));
	}

	xh_weiisp = xhttp_client(HTTP_METHOD_GET, pd->session_url);
	if (!xh_weiisp)
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_set_request_default_header(xh_weiisp);
	xhttp_set_request_content_type(xh_weiisp, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_url_query_entity(xh_weiisp, _T("appid"), -1, pd->appid, -1);
	xhttp_set_url_query_entity(xh_weiisp, _T("secret"), -1, pd->secret, -1);
	xhttp_set_url_query_entity(xh_weiisp, _T("js_code"), -1, sz_code, -1);
	xhttp_set_url_query_entity(xh_weiisp, _T("grant_type"), -1, _T("authorization_code"), -1);

	if (!xhttp_send_request(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh_weiisp))
	{
		raise_user_error(NULL, NULL);
	}

	ptr_json = create_json_doc();

	if (!xhttp_recv_json(xh_weiisp, ptr_json))
	{
		raise_user_error(_T("_invoke_weiisp_access"), _T("parse json doc failed"));
	}

	xhttp_close(xh_weiisp);
	xh_weiisp = NULL;

	nlk = get_json_first_child_item(ptr_json);
	while (nlk)
	{
		if (compare_text(get_json_item_name_ptr(nlk), -1, _T("openid"), -1, 1) == 0)
			get_json_item_value(nlk, sz_openid, KEY_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("session_key"), -1, 1) == 0)
			get_json_item_value(nlk, sz_session_key, KEY_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("unionid"), -1, 1) == 0)
			get_json_item_value(nlk, sz_unionid, KEY_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("errcode"), -1, 1) == 0)
			get_json_item_value(nlk, sz_num, NUM_LEN);
		else if (compare_text(get_json_item_name_ptr(nlk), -1, _T("errmsg"), -1, 1) == 0)
			get_json_item_value(nlk, sz_err, ERR_LEN);

		nlk = get_json_next_sibling_item(nlk);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	if (!is_null(sz_num) && xstol(sz_num) != 0)
	{
		raise_user_error(_T("_invoke_weiisp_access"), sz_err);
	}

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CACHECONTROL, -1, HTTP_HEADER_CACHECONTROL_NOSTORE, -1);

	ptr_json = create_json_doc();

	nlk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(nlk, _T("phoneNumber"));
	set_json_item_value(nlk, sz_phone);

	nlk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(nlk, _T("purePhoneNumber"));
	set_json_item_value(nlk, sz_pure);

	nlk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(nlk, _T("countryCode"));
	set_json_item_value(nlk, sz_country);

	if (!xhttp_send_json(pb->http, ptr_json))
	{
		raise_user_error(NULL, NULL);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	END_CATCH;

	return 1;

ONERROR:
	XDL_TRACE_LAST;

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (xh_weiisp)
		xhttp_close(xh_weiisp);

	return 0;
}

void _invoke_error(const https_block_t* pb, weiisp_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	d_recv = bytes_alloc();

	xhttp_recv_full(pb->http, d_recv, &n_size);

	bytes_free(d_recv);
	d_recv = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[WEIISP: ERROR]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

/****************************************************************************************/

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	weiisp_block_t* pd = NULL;

	tchar_t token[RES_LEN] = { 0 };
	tchar_t file[PATH_LEN] = { 0 };

	bool_t rt = 1;

	link_t_ptr ptr_prop = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pd = (weiisp_block_t*)xmem_alloc(sizeof(weiisp_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s%s"), pb->path, pb->object);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(_T("-1"), _T("load weiisp config falied\n"));
	}

	xszero(file, PATH_LEN);
	read_proper(ptr_prop, _T("ISP"), -1, _T("SessionUrl"), -1, pd->session_url, PATH_LEN);
	read_proper(ptr_prop, _T("ISP"), -1, _T("AccessUrl"), -1, pd->access_url, PATH_LEN);
	read_proper(ptr_prop, _T("ISP"), -1, _T("AppId"), -1, pd->appid, RES_LEN);
	read_proper(ptr_prop, _T("ISP"), -1, _T("Secret"), -1, pd->secret, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	xhttp_get_url_query_entity(pb->http, _T("action"), -1, token, RES_LEN);

	if (compare_text(token, xslen(_T("session")), _T("session"), -1, 1) == 0)
	{
		_invoke_weiisp_session(pb, pd);
	}
	else if (compare_text(token, xslen(_T("access")), _T("access"), -1, 1) == 0)
	{
		_invoke_weiisp_access(pb, pd);
	}
	else if (compare_text(token, xslen(_T("phone")), _T("phone"), -1, 1) == 0)
	{
		_invoke_weiisp_phone(pb, pd);
	}
	else
	{
		raise_user_error(_T("weiisp_api"), _T("unknown weiisp function\n"));
	}

	xmem_free(pd);

	END_CATCH;

	return (rt) ? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	if (pd)
	{
		_invoke_error(pb, pd);
	}

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if (pd)
		xmem_free(pd);

	return HTTPS_INVOKE_WITHINFO;
}
