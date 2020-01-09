/***********************************************************************
	Easily loc web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc loc web api document

	@module	oau_api.cc | loc web api implement file

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

#include "oau_api.h"

typedef struct _oau_block_t{
	secu_desc_t sd;
	tchar_t location[PATH_LEN];
}oau_block_t;


bool_t _invoke_auth_request(const https_block_t* pb, oau_block_t* pos)
{
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	tchar_t sz_response_type[INT_LEN] = { 0 };
	tchar_t sz_redirect_uri[PATH_LEN] = { 0 };
	tchar_t sz_scope[PATH_LEN] = { 0 };
	tchar_t sz_client_id[RES_LEN] = { 0 };
	tchar_t sz_state[NUM_LEN + 1] = { 0 };

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	dword_t n;

	tchar_t* sz_qry = NULL;
	int len;

	TRY_CATCH;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OAUTH: AUTH]"), -1);

		len = xhttp_get_url_query(pb->http, NULL, MAX_LONG);
		sz_qry = xsalloc(len + 1);
		len = xhttp_get_url_query(pb->http, sz_qry, len);

		(*pb->pf_log_error)(pb->log, _T(""), sz_qry, -1);

		xsfree(sz_qry);
		sz_qry = NULL;
	}

	xhttp_get_url_query_entity(pb->http, OAUTH_RESPONSE_TYPE, -1, sz_response_type, INT_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_REDIRECT_URL, -1, sz_redirect_uri, PATH_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_SCOPE, -1, sz_scope, PATH_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_CLIENT_ID, -1, sz_client_id, RES_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_STATE, -1, sz_state, NUM_LEN);

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_302);
	xhttp_set_response_message(pb->http, HTTP_CODE_302_TEXT, -1);

	xscat(sz_redirect_uri, _T("?"));
	if (compare_text(sz_response_type, -1, OAUTH_RESPONSE_TYPE_CODE, -1, 1) == 0)
	{
		Srand48(get_times());
		n = Lrand48();
		xsprintf(sz_code, _T("%d"), n % 10000);

		xscat(sz_redirect_uri, OAUTH_CODE);
		xscat(sz_redirect_uri, _T("="));
		xscat(sz_redirect_uri, sz_code);
	}
	else if (compare_text(sz_response_type, -1, OAUTH_RESPONSE_TYPE_TOKEN, -1, 1) == 0)
	{
		Srand48(get_times());
		n = Lrand48();
		xsprintf(sz_code, _T("%d"), n % 100000);

		xscat(sz_redirect_uri, OAUTH_ACCESS_TOKEN);
		xscat(sz_redirect_uri, _T("="));
		xscat(sz_redirect_uri, sz_code);
	}
	else
	{
		raise_user_error(_T("_invoke_auth_request"), _T("unknown oauth response type"));
	}

	xscat(sz_redirect_uri, _T("&"));

	xscat(sz_redirect_uri, OAUTH_STATE);
	xscat(sz_redirect_uri, _T("="));
	xscat(sz_redirect_uri, sz_state);

	xhttp_set_response_header(pb->http, HTTP_HEADER_LOCATION , -1, sz_redirect_uri, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_num, sz_err, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_num, sz_err, -1);

	if (sz_qry)
		xsfree(sz_qry);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OAUTH: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_num, sz_err, -1);
	}

	return 0;
}

bool_t _invoke_auth_access(const https_block_t* pb, oau_block_t* pos)
{
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	tchar_t sz_grant_type[RES_LEN] = { 0 };
	tchar_t sz_redirect_uri[PATH_LEN] = { 0 };
	tchar_t sz_client_id[RES_LEN] = { 0 };
	tchar_t sz_code[NUM_LEN + 1] = { 0 };

	tchar_t sz_token[NUM_LEN + 1] = { 0 };
	dword_t n;

	link_t_ptr ptr_json = NULL;
	link_t_ptr ilk;

	tchar_t* sz_qry = NULL;
	int len;

	TRY_CATCH;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OAUTH: AUTH]"), -1);

		len = xhttp_get_url_query(pb->http, NULL, MAX_LONG);
		sz_qry = xsalloc(len + 1);
		len = xhttp_get_url_query(pb->http, sz_qry, len);

		(*pb->pf_log_error)(pb->log, _T(""), sz_qry, -1);

		xsfree(sz_qry);
		sz_qry = NULL;
	}

	xhttp_get_url_query_entity(pb->http, OAUTH_GRANT_TYPE, -1, sz_grant_type, RES_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_REDIRECT_URL, -1, sz_redirect_uri, PATH_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_CODE, -1, sz_code, NUM_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_CLIENT_ID, -1, sz_client_id, RES_LEN);

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	Srand48(get_times());
	n = Lrand48();
	xsprintf(sz_token, _T("%d"), n % 100000);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CACHECONTROL, -1, HTTP_HEADER_CACHECONTROL_NOSTORE, -1);

	ptr_json = create_json_doc();

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_ACCESS_TOKEN);
	set_json_item_value(ilk, sz_token);

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_TOKEN_TYPE);
	set_json_item_value(ilk, OAUTH_TOKEN_TYPE_MD5);

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_EXPIRES_IN);
	set_json_item_value(ilk, _T("3600"));

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_REFRESH_TOKEN);
	set_json_item_value(ilk, sz_code);

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_SCOPE);
	set_json_item_value(ilk, NULL);

	if (!xhttp_send_json(pb->http, ptr_json))
	{
		raise_user_error(NULL, NULL);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (ptr_json)
		destroy_json_doc(ptr_json);

	get_last_error(sz_num, sz_err, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_num, sz_err, -1);

	if (sz_qry)
		xsfree(sz_qry);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OAUTH: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_num, sz_err, -1);
	}

	return 0;
}

bool_t _invoke_auth_refresh(const https_block_t* pb, oau_block_t* pos)
{
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	tchar_t sz_grant_type[RES_LEN] = { 0 };
	tchar_t sz_refresh_token[RES_LEN] = { 0 };
	tchar_t sz_scope[PATH_LEN + 1] = { 0 };

	tchar_t sz_token[NUM_LEN + 1] = { 0 };
	dword_t n;

	link_t_ptr ptr_json = NULL;
	link_t_ptr ilk;

	tchar_t* sz_qry = NULL;
	int len;

	TRY_CATCH;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OAUTH: AUTH]"), -1);

		len = xhttp_get_url_query(pb->http, NULL, MAX_LONG);
		sz_qry = xsalloc(len + 1);
		len = xhttp_get_url_query(pb->http, sz_qry, len);

		(*pb->pf_log_error)(pb->log, _T(""), sz_qry, -1);

		xsfree(sz_qry);
		sz_qry = NULL;
	}

	xhttp_get_url_query_entity(pb->http, OAUTH_GRANT_TYPE, -1, sz_grant_type, RES_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_REFRESH_TOKEN, -1, sz_refresh_token, RES_LEN);
	xhttp_get_url_query_entity(pb->http, OAUTH_SCOPE, -1, sz_scope, PATH_LEN);

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	Srand48(get_times());
	n = Lrand48();
	xsprintf(sz_token, _T("%d"), n % 100000);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CACHECONTROL, -1, HTTP_HEADER_CACHECONTROL_NOSTORE, -1);

	ptr_json = create_json_doc();

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_ACCESS_TOKEN);
	set_json_item_value(ilk, sz_token);

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_TOKEN_TYPE);
	set_json_item_value(ilk, OAUTH_TOKEN_TYPE_MD5);

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_EXPIRES_IN);
	set_json_item_value(ilk, _T("3600"));

	ilk = insert_json_item(ptr_json, LINK_LAST);
	set_json_item_name(ilk, OAUTH_REFRESH_TOKEN);
	set_json_item_value(ilk, sz_refresh_token);

	if (!xhttp_send_json(pb->http, ptr_json))
	{
		raise_user_error(NULL, NULL);
	}

	destroy_json_doc(ptr_json);
	ptr_json = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (ptr_json)
		destroy_json_doc(ptr_json);

	get_last_error(sz_num, sz_err, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_num, sz_err, -1);

	if (sz_qry)
		xsfree(sz_qry);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OAUTH: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_num, sz_err, -1);
	}

	return 0;
}

void _invoke_error(const https_block_t* pb, oau_block_t* pos)
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
		(*pb->pf_log_title)(pb->log, _T("[OAUTH: ´íÎó]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	oau_block_t* pos = NULL;

	tchar_t token[PATH_LEN + 1] = { 0 };

	bool_t rt = 1;

	link_t_ptr ptr_prop = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pos = (oau_block_t*)xmem_alloc(sizeof(oau_block_t));

	/*ptr_prop = create_proper_doc();

	xsprintf(token, _T("%s/oau.ini"), pb->path);

	if (!load_proper_doc_from_ini_file(ptr_prop, NULL, token))
	{
		raise_user_error(_T("-1"), _T("load loc config falied\n"));
	}

	read_proper(ptr_prop, _T("OAU"), -1, _T("OAUATION"), -1, pos->local, PATH_LEN);
	read_proper(ptr_prop, _T("OAU"), -1, _T("PUBLICKEY"), -1, pos->sd.public_key, KEY_LEN);
	read_proper(ptr_prop, _T("OAU"), -1, _T("PRIVATEKEY"), -1, pos->sd.private_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;*/

	if (compare_text(pb->object, -1, _T("/auth_request"), -1, 1) == 0)
		rt = _invoke_auth_request(pb, pos);
	else if (compare_text(pb->object, -1, _T("/auth_access"), -1, 1) == 0)
		rt = _invoke_auth_access(pb, pos);
	else if (compare_text(pb->object, -1, _T("/auth_refresh"), -1, 1) == 0)
		rt = _invoke_auth_refresh(pb, pos);
	else
	{
		raise_user_error(_T("oau_api"), _T("unknown loc method"));
	}

	xmem_free(pos);
	pos = NULL;

	END_CATCH;

	return (rt) ? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb, pos);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	xmem_free(pos);

	return HTTPS_INVOKE_WITHINFO;
}