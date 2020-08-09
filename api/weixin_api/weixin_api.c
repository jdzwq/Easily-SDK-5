/***********************************************************************
	Easily weixin weixin

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc weixin weixin document

	@module	weixin_weixin.c | weixin weixin implement file

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

#include "weixin_api.h"

typedef struct _weixin_block_t{
	tchar_t local_url[PATH_LEN];

	tchar_t token[KEY_LEN];
	tchar_t aeskey[KEY_LEN];
}weixin_block_t;

static void _calc_signature(const tchar_t* token, const tchar_t* timestamp, const tchar_t* nonce, tchar_t* signature)
{
	link_t_ptr st;
	link_t_ptr elk;
	string_t vs;
	byte_t* buf;
	int n;
	byte_t out[20] = { 0 };

	st = create_string_table(1);
	write_string_entity(st, token, -1, NULL, 0);
	write_string_entity(st, timestamp, -1, NULL, 0);
	write_string_entity(st, nonce, -1, NULL, 0);

	vs = string_alloc();

	elk = get_string_next_entity(st, LINK_FIRST);
	while (elk)
	{
		string_cat(vs, get_string_entity_key_ptr(elk), -1);

		elk = get_string_next_entity(st, elk);
	}
	destroy_string_table(st);

#if defined(_UNICODE) || defined(UNICODE)
	n = ucs_to_utf8(string_ptr(vs), string_len(vs), NULL, MAX_LONG);
#else
	n = mbs_to_utf8(string_ptr(vs), string_len(vs), NULL, MAX_LONG);
#endif

	buf = (byte_t*)xmem_alloc(n);

#if defined(_UNICODE) || defined(UNICODE)
	n = ucs_to_utf8(string_ptr(vs), string_len(vs), buf, n);
#else
	n = mbs_to_utf8(string_ptr(vs), string_len(vs), buf, n);
#endif

	string_free(vs);

	sha1(buf, n, out);

	xmem_free(buf);

	for (n = 0; n < 20; n++)
	{
		xsprintf(&(signature[n * 2]), _T("%02x"), (out[n] & 0x00FF));
	}
}

bool_t _invoke_weixin_echo(const https_block_t* pb, weixin_block_t* pd)
{
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };
	tchar_t sz_method[RES_LEN] = { 0 };
	tchar_t* sz_query = NULL;
	int len;
	byte_t* sz_buf = NULL;
	dword_t n;

	tchar_t signature[KEY_LEN] = { 0 };
	tchar_t timestamp[KEY_LEN] = { 0 };
	tchar_t nonce[KEY_LEN] = { 0 };
	tchar_t echostr[1024] = { 0 };

	tchar_t result[KEY_LEN] = { 0 };

	TRY_CATCH;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[交易应答]"), -1);

		len = xhttp_get_url_query(pb->http, NULL, MAX_LONG);
		sz_query = xsalloc(len + 1);
		xhttp_get_url_query(pb->http, sz_query, len);

		(*pb->pf_log_title)(pb->log, sz_query, len);
		xsfree(sz_query);
		sz_query = NULL;
	}

	xhttp_get_url_query_entity(pb->http, _T("signature"), -1, signature, KEY_LEN);
	xhttp_get_url_query_entity(pb->http, _T("timestamp"), -1, timestamp, KEY_LEN);
	xhttp_get_url_query_entity(pb->http, _T("nonce"), -1, nonce, KEY_LEN);
	xhttp_get_url_query_entity(pb->http, _T("echostr"), -1, echostr, 1024);

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN_UTF8, -1);

#if defined(_UNICODE) || defined(UNICODE)
	n = ucs_to_utf8(echostr, -1, NULL, MAX_LONG);
#else
	n = mbs_to_utf8(echostr, -1, NULL, MAX_LONG);
#endif

	sz_buf = (byte_t*)xmem_alloc(n);

#if defined(_UNICODE) || defined(UNICODE)
	n = ucs_to_utf8(echostr, -1, sz_buf, n);
#else
	n = mbs_to_utf8(echostr, -1, sz_buf, n);
#endif

	xhttp_set_response_content_length(pb->http, n);

	if (!xhttp_send_full(pb->http, sz_buf, n))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(sz_buf);
	sz_buf = NULL;

	_calc_signature(pd->token, timestamp, nonce, result);

	if (compare_text(signature, -1, result, -1, 1) != 0)
	{
		raise_user_error(_T("_invoke_weixin_echo"), _T("invalid signature"));
	}

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[交易应答]"), -1);

		(*pb->pf_log_title)(pb->log, echostr, -1);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_num, sz_err, ERR_LEN);

	if (sz_buf)
		xmem_free(sz_buf);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[交易错误]"), -1);

		(*pb->pf_log_title)(pb->log, sz_err, -1);
	}

	return 0;
}

void _invoke_error(const https_block_t* pb, weixin_block_t* pd)
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
		(*pb->pf_log_title)(pb->log, _T("[WEIXIN: ERROR]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

/****************************************************************************************/

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	weixin_block_t* pd = NULL;

	tchar_t token[RES_LEN] = { 0 };
	tchar_t file[PATH_LEN] = { 0 };

	bool_t rt = 1;

	link_t_ptr ptr_prop = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pd = (weixin_block_t*)xmem_alloc(sizeof(weixin_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s%s"), pb->path, pb->object);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(_T("-1"), _T("load weixin config falied\n"));
	}

	xszero(file, PATH_LEN);
	read_proper(ptr_prop, _T("ISP"), -1, _T("LocalUrl"), -1, pd->local_url, PATH_LEN);
	read_proper(ptr_prop, _T("ISP"), -1, _T("Token"), -1, pd->token, KEY_LEN);
	read_proper(ptr_prop, _T("ISP"), -1, _T("EncodingAESKey"), -1, pd->aeskey, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	if (compare_text(method, -1, _T("GET"), -1, 1) == 0)
	{
		rt = _invoke_weixin_echo(pb, pd);
	}
	else
	{
		raise_user_error(_T("weixin_api"), _T("unknown weixin function\n"));
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
