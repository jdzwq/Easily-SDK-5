/***********************************************************************
	Easily oau weiapp

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc oau weiapp document

	@module	oau_weiapp.c | oau weiapp implement file

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

#include "oaudef.h"


typedef struct _weiapp_t{
	oau_head head;

	tchar_t session[PATH_LEN];
	tchar_t access[PATH_LEN];
	tchar_t appid[RES_LEN];
	tchar_t secret[KEY_LEN];
  
	tchar_t err_code[NUM_LEN + 1];
	tchar_t err_text[ERR_LEN + 1];
}weiapp_t;


oau_t STDCALL oau_open_isp(const tchar_t* ispfile)
{
	weiapp_t* pal = NULL;
	LINKPTR d_ptr = NULL;

	TRY_CATCH;

	d_ptr = create_proper_doc();

	if (!load_proper_from_ini_file(d_ptr, NULL, ispfile))
	{
		raise_user_error(NULL, NULL);
	}

	pal = (weiapp_t*)xmem_alloc(sizeof(weiapp_t));

	read_proper(d_ptr, _T("ISP"), -1, _T("SessionUrl"), -1, pal->session, PATH_LEN);
	read_proper(d_ptr, _T("ISP"), -1, _T("AccessUrl"), -1, pal->access, PATH_LEN);
	read_proper(d_ptr, _T("ISP"), -1, _T("AppId"), -1, pal->appid, RES_LEN);
	read_proper(d_ptr, _T("ISP"), -1, _T("Secret"), -1, pal->secret, KEY_LEN);

	destroy_proper_doc(d_ptr);
	d_ptr = NULL;

	END_CATCH;

	return (oau_t)(&pal->head);

ONERROR:

	if (d_ptr)
		destroy_proper_doc(d_ptr);

	if (pal)
		xmem_free(pal);

	return NULL;
}

void STDCALL oau_close(oau_t oau)
{
    weiapp_t* pal = (weiapp_t*)oau;
	
	XDL_ASSERT(pal != NULL);

	xmem_free(pal);
}

bool_t STDCALL oau_session(oau_t oau, const tchar_t* jscode, tchar_t* skey, tchar_t* oid, tchar_t* uid)
{
	weiapp_t* pal = (weiapp_t*)oau;

	xhand_t xh_weiapp = NULL;

	tchar_t sz_openid[KEY_LEN] = { 0 };
	tchar_t sz_session_key[KEY_LEN] = { 0 };
	tchar_t sz_unionid[KEY_LEN] = { 0 };

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk;

	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	TRY_CATCH;

	xh_weiapp = xhttp_client(HTTP_METHOD_GET, pal->session);
	if (!xh_weiapp)
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_set_request_default_header(xh_weiapp);
	xhttp_set_request_content_type(xh_weiapp, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_url_query_entity(xh_weiapp, _T("appid"), -1, pal->appid, -1);
	xhttp_set_url_query_entity(xh_weiapp, _T("secret"), -1, pal->secret, -1);
	xhttp_set_url_query_entity(xh_weiapp, _T("js_code"), -1, jscode, -1);
	xhttp_set_url_query_entity(xh_weiapp, _T("grant_type"), -1, _T("authorization_code"), -1);

	if (!xhttp_send_request(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	ptr_json = create_json_doc();

	if (!xhttp_recv_json(xh_weiapp, ptr_json))
	{
		raise_user_error(_T("_invoke_weiapp_access"), _T("parse json doc failed"));
	}

	xhttp_close(xh_weiapp);
	xh_weiapp = NULL;

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
		raise_user_error(_T("_invoke_weiapp_access"), sz_err);
	}

	xscpy(skey, sz_session_key);
	xscpy(oid, sz_openid);
	xscpy(uid, sz_unionid);

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(pal->err_code, pal->err_text, ERR_LEN);

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (xh_weiapp)
		xhttp_close(xh_weiapp);

	return 0;
}

bool_t STDCALL oau_access(oau_t oau, tchar_t* token)
{
	weiapp_t* pal = (weiapp_t*)oau;

	xhand_t xh_weiapp = NULL;

	tchar_t sz_token[KEY_LEN] = { 0 };
	tchar_t sz_expires[INT_LEN] = { 0 };

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk;

	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	TRY_CATCH;

	xh_weiapp = xhttp_client(HTTP_METHOD_GET, pal->access);
	if (!xh_weiapp)
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_set_request_default_header(xh_weiapp);
	xhttp_set_request_content_type(xh_weiapp, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_url_query_entity(xh_weiapp, _T("grant_type"), -1, _T("client_credential"), -1);
	xhttp_set_url_query_entity(xh_weiapp, _T("appid"), -1, pal->appid, -1);
	xhttp_set_url_query_entity(xh_weiapp, _T("secret"), -1, pal->secret, -1);

	if (!xhttp_send_request(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	ptr_json = create_json_doc();

	if (!xhttp_recv_json(xh_weiapp, ptr_json))
	{
		raise_user_error(_T("_invoke_weiapp_access"), _T("parse json doc failed"));
	}

	xhttp_close(xh_weiapp);
	xh_weiapp = NULL;

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
		raise_user_error(_T("_invoke_weiapp_access"), sz_err);
	}

	xscpy(token, sz_token);

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(pal->err_code, pal->err_text, ERR_LEN);

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (xh_weiapp)
		xhttp_close(xh_weiapp);

	return 0;
}

bool_t STDCALL oau_phone(oau_t oau, const tchar_t* jscode, const tchar_t* encryptedData, const tchar_t* iv, tchar_t* phoneNumber, tchar_t* purePhoneNumber, tchar_t* countryCode)
{
	weiapp_t* pal = (weiapp_t*)oau;

	xhand_t xh_weiapp = NULL;

	tchar_t sz_openid[KEY_LEN] = { 0 };
	tchar_t sz_session_key[KEY_LEN] = { 0 };
	tchar_t sz_unionid[KEY_LEN] = { 0 };

	link_t_ptr ptr_json = NULL;
	link_t_ptr nlk;

	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	TRY_CATCH;

	xh_weiapp = xhttp_client(HTTP_METHOD_GET, pal->session);
	if (!xh_weiapp)
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_set_request_default_header(xh_weiapp);
	xhttp_set_request_content_type(xh_weiapp, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

	xhttp_set_url_query_entity(xh_weiapp, _T("appid"), -1, pal->appid, -1);
	xhttp_set_url_query_entity(xh_weiapp, _T("secret"), -1, pal->secret, -1);
	xhttp_set_url_query_entity(xh_weiapp, _T("js_code"), -1, jscode, -1);
	xhttp_set_url_query_entity(xh_weiapp, _T("grant_type"), -1, _T("authorization_code"), -1);

	if (!xhttp_send_request(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_get_response_state(xh_weiapp))
	{
		raise_user_error(NULL, NULL);
	}

	ptr_json = create_json_doc();

	if (!xhttp_recv_json(xh_weiapp, ptr_json))
	{
		raise_user_error(_T("_invoke_weiapp_access"), _T("parse json doc failed"));
	}

	xhttp_close(xh_weiapp);
	xh_weiapp = NULL;

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
		raise_user_error(_T("_invoke_weiapp_access"), sz_err);
	}

	xscpy(phoneNumber, _T("13500000000"));
	xscpy(purePhoneNumber, _T("13500000000"));
	xscpy(countryCode, _T("86"));

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(pal->err_code, pal->err_text, ERR_LEN);

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (xh_weiapp)
		xhttp_close(xh_weiapp);

	return 0;
}

int STDCALL oau_error(oau_t oau, tchar_t* buf, int max)
{
	weiapp_t* pal = (weiapp_t*)oau;

	XDL_ASSERT(pal != NULL);

	max = (max < ERR_LEN) ? max : ERR_LEN;
	if (buf)
	{
		xsncpy(buf, pal->err_text, max);
	}

	return -1;
}
