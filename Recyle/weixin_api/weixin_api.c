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

#define CONENTYPE_IS_JSON(token)	((compare_text(token, xslen(HTTP_HEADER_CONTENTTYPE_APPJSON), HTTP_HEADER_CONTENTTYPE_APPJSON, -1, 1) == 0)? 1 : 0)
#define CONENTYPE_IS_UNKNOWN(token)	((is_null(token) || compare_text(token, 3, _T("*/*"), -1, 1) == 0)? 1 : 0)

typedef struct _weixin_block_t{
	tchar_t local_url[PATH_LEN];

	tchar_t token[KEY_LEN];
	tchar_t aeskey[KEY_LEN];
}weixin_block_t;

typedef struct _weixin_message_t{
	tchar_t ToUserName[KEY_LEN];
	tchar_t FromUserName[KEY_LEN];
	tchar_t CreateTime[NUM_LEN];
	tchar_t MsgType[INT_LEN];
	tchar_t MsgId[KEY_LEN];
	tchar_t Event[KEY_LEN];
}weixin_message_t;

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

static bool_t _invoke_message_text(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_message_image(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_message_voice(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_message_video(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_message_shortvideo(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_message_location(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_message_link(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_event_subcribe(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_event_unsubcribe(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_event_scan(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_event_location(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_event_click(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}

static bool_t _invoke_event_view(weixin_block_t* pd, link_t_ptr st)
{
	return 1;
}


bool_t _invoke_weixin_message(const https_block_t* pb, weixin_block_t* pd)
{
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_num[NUM_LEN + 1] = { 0 };
	tchar_t sz_encoding[RES_LEN] = { 0 };
	bool_t b_json;

	link_t_ptr ptr_json = NULL;
	link_t_ptr ptr_xml = NULL;
	link_t_ptr nlk_rowset, nlk_row, nlk;
	link_t_ptr ptr_table = NULL;
	bool_t rt;

	tchar_t MsgType[RES_LEN] = { 0 };
	tchar_t Event[RES_LEN] = { 0 }; 

	TRY_CATCH;

	//获取请求参数
	xhttp_get_request_accept_type(pb->http, sz_encoding, RES_LEN);
	if (CONENTYPE_IS_UNKNOWN(sz_encoding))
	{
		xhttp_get_request_content_type(pb->http, sz_encoding, RES_LEN);
	}
	b_json = CONENTYPE_IS_JSON(sz_encoding);

	if (b_json)
	{
		ptr_json = create_json_doc();

		rt = xhttp_recv_json(pb->http, ptr_json);
		if (!rt)
		{
			get_last_error(sz_num, sz_err, ERR_LEN);
		}

		if (pb->log)
		{
			(*pb->pf_log_title)(pb->log, _T("[交易请求]"), -1);

			(*pb->pf_log_json)(pb->log, ptr_json);
		}

		if (!rt)
		{
			raise_user_error(sz_num, sz_err);
		}
	}
	else
	{
		ptr_xml = create_xml_doc();

		rt = xhttp_recv_xml(pb->http, ptr_xml);
		if (!rt)
		{
			get_last_error(sz_num, sz_err, ERR_LEN);
		}

		if (pb->log)
		{
			(*pb->pf_log_title)(pb->log, _T("[交易请求]"), -1);

			(*pb->pf_log_xml)(pb->log, ptr_xml);
		}

		if (!rt)
		{
			raise_user_error(sz_num, sz_err);
		}
	}

	if (b_json)
	{
		nlk_rowset = ptr_json;
	}
	else
	{
		nlk_rowset = get_xml_dom_node(ptr_xml);
	}
	nlk_row = (nlk_rowset) ? get_dom_first_child_node(nlk_rowset) : NULL;

	if (!nlk_row)
	{
		raise_user_error(_T("-1"), _T("交易数据为空\n"));
	}

	ptr_table = create_string_table(1);

	nlk = get_dom_first_child_node(nlk_row);
	while (nlk)
	{
		write_string_entity(ptr_table, get_dom_node_name_ptr(nlk), -1, get_dom_node_text_ptr(nlk), -1);

		nlk = get_dom_next_sibling_node(nlk);
	}

	if (b_json)
	{
		destroy_json_doc(ptr_json);
		ptr_json = NULL;
	}
	else
	{
		destroy_xml_doc(ptr_xml);
		ptr_xml = NULL;
	}

	read_string_entity(ptr_table, _T("MsgType"), -1, MsgType, RES_LEN);

	if (compare_text(MsgType, -1, _T("text"), -1, 1) == 0)
	{
		rt = _invoke_message_text(pd, ptr_table);
	}
	else if (compare_text(MsgType, -1, _T("image"), -1, 1) == 0)
	{
		rt = _invoke_message_image(pd, ptr_table);
	}
	else if (compare_text(MsgType, -1, _T("voice"), -1, 1) == 0)
	{
		rt = _invoke_message_voice(pd, ptr_table);
	}
	else if (compare_text(MsgType, -1, _T("video"), -1, 1) == 0)
	{
		rt = _invoke_message_video(pd, ptr_table);
	}
	else if (compare_text(MsgType, -1, _T("shortvideo"), -1, 1) == 0)
	{
		rt = _invoke_message_shortvideo(pd, ptr_table);
	}
	else if (compare_text(MsgType, -1, _T("location"), -1, 1) == 0)
	{
		rt = _invoke_message_location(pd, ptr_table);
	}
	else if (compare_text(MsgType, -1, _T("link"), -1, 1) == 0)
	{
		rt = _invoke_message_link(pd, ptr_table);
	}
	else if (compare_text(MsgType, -1, _T("event"), -1, 1) == 0)
	{
		read_string_entity(ptr_table, _T("Event"), -1, Event, RES_LEN);
		
		if (compare_text(Event, -1, _T("subscribe"), -1, 1) == 0)
		{
			rt = _invoke_event_subcribe(pd, ptr_table);
		}
		else if (compare_text(Event, -1, _T("unsubscribe"), -1, 1) == 0)
		{
			rt = _invoke_event_unsubcribe(pd, ptr_table);
		}
		else if (compare_text(Event, -1, _T("SCAN"), -1, 1) == 0)
		{
			rt = _invoke_event_scan(pd, ptr_table);
		}
		else if (compare_text(Event, -1, _T("LOCATION"), -1, 1) == 0)
		{
			rt = _invoke_event_location(pd, ptr_table);
		}
		else if (compare_text(Event, -1, _T("CLICK"), -1, 1) == 0)
		{
			rt = _invoke_event_click(pd, ptr_table);
		}
		else if (compare_text(Event, -1, _T("VIEW"), -1, 1) == 0)
		{
			rt = _invoke_event_view(pd, ptr_table);
		}
	}
	else
	{
		raise_user_error(_T("_invoke_weixin_message"), _T("unknown MsgType"));
	}

	destroy_string_table(ptr_table);
	ptr_table = NULL;

	xhttp_set_response_default_header(pb->http);
	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN_UTF8, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[交易应答]"), -1);

		(*pb->pf_log_title)(pb->log, _T(""), -1);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_num, sz_err, ERR_LEN);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[交易应答]"), -1);

		(*pb->pf_log_title)(pb->log, sz_err, -1);
	}

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (ptr_xml)
		destroy_xml_doc(ptr_xml);

	if (ptr_table)
		destroy_string_table(ptr_table);

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
		rt = _invoke_weixin_message(pb, pd);
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
