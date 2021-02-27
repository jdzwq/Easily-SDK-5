/***********************************************************************
	Easily event web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc event web api document

	@module	event_api.cc | event web api implement file

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

#include "event_api.h"

typedef struct _event_block_t{
	tchar_t topic[RES_LEN + 1];
	tchar_t path[PATH_LEN + 1];
}event_block_t;

bool_t _invoke_event_pubs(const https_block_t* pb, event_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_method[RES_LEN + 1] = { 0 };
	tchar_t sz_encoding[RES_LEN + 1] = { 0 };
	bool_t b_json = 0;

	tchar_t sz_name[RES_LEN + 1] = { 0 };
	tchar_t sz_file[PATH_LEN + 1] = { 0 };
	tchar_t sz_token[STYLE_LEN + 1] = { 0 };

	link_t_ptr ptr_xml = NULL;
	link_t_ptr ptr_json = NULL;
	link_t_ptr ptr_event = NULL;

	byte_t* msg_buf = NULL;
	dword_t msg_len = 0;

	t_kb_t hkb = NULL;

	variant_t key = NULL;
	object_t val = NULL;
	queue_t que = NULL;
	message_t msg = NULL;
	msg_hdr_t hdr = { 0 };

	bool_t rt;

	TRY_CATCH;

	xhttp_get_url_method(pb->http, sz_method, RES_LEN);

	if (compare_text(sz_method, -1, HTTP_METHOD_GET, -1, 1) == 0)
	{
		ptr_event = create_event_doc();

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_ID, -1, sz_token, RES_LEN);
		if (!is_null(sz_token))
		{
			set_event_id(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_FROM, -1, sz_token, PATH_LEN);
		if (!is_null(sz_token))
		{
			set_event_from(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_SENDER, -1, sz_token, RES_LEN);
		if (!is_null(sz_token))
		{
			set_event_sender(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_SENDTIME, -1, sz_token, DATE_LEN);
		if (!is_null(sz_token))
		{
			set_event_send_time(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_CODE, -1, sz_token, RES_LEN);
		if (!is_null(sz_token))
		{
			set_event_code(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_DESCRIBE, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_event_describe(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_MEDIUM, -1, sz_token, RES_LEN);
		if (!is_null(sz_token))
		{
			set_event_medium(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_CONTENT, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_event_content(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_TO, -1, sz_token, PATH_LEN);
		if (!is_null(sz_token))
		{
			set_event_to(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_RECEIPTOR, -1, sz_token, RES_LEN);
		if (!is_null(sz_token))
		{
			set_event_receiptor(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_RECEIPTTIME, -1, sz_token, DATE_LEN);
		if (!is_null(sz_token))
		{
			set_event_receipt_time(ptr_event, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_EVENT_RECEIPTSTATE, -1, sz_token, RES_LEN);
		if (!is_null(sz_token))
		{
			set_event_receipt_state(ptr_event, sz_token, -1);
		}
	}
	else
	{
		xhttp_get_request_accept_type(pb->http, sz_encoding, RES_LEN);
		if (CONTENTTYPE_IS_ANY(sz_encoding))
		{
			xhttp_get_request_content_type(pb->http, sz_encoding, RES_LEN);
		}
		b_json = CONTENTTYPE_IS_JSON(sz_encoding);

		if (b_json)
		{
			ptr_json = create_json_doc();

			rt = xhttp_recv_json(pb->http, ptr_json);
			if (!rt)
			{
				get_last_error(sz_code, sz_error, ERR_LEN);
			}

			if (pb->plg)
			{
				(*pb->plg->pf_log_title)(pb->plg->unc, _T("[EVENT]"), -1);

				(*pb->plg->pf_log_json)(pb->plg->unc, ptr_json);
			}

			if (!rt)
			{
				raise_user_error(sz_code, sz_error);
			}
		}
		else
		{
			ptr_xml = create_xml_doc();

			rt = xhttp_recv_xml(pb->http, ptr_xml);
			if (!rt)
			{
				get_last_error(sz_code, sz_error, ERR_LEN);
			}

			if (pb->plg)
			{
				(*pb->plg->pf_log_title)(pb->plg->unc, _T("[EVENT]"), -1);

				(*pb->plg->pf_log_xml)(pb->plg->unc, ptr_xml);
			}

			if (!rt)
			{
				raise_user_error(sz_code, sz_error);
			}
		}

		if (b_json)
		{
			ptr_event = ptr_json;
		}
		else
		{
			ptr_event = get_xml_dom_node(ptr_xml);
		}
	}

	get_event_id(ptr_event, sz_name, RES_LEN);
	if (is_null(sz_name))
	{
		raise_user_error(_T("_invoke_event_pubs"), _T("empty event id"));
	}

	msg_len = format_json_doc_to_bytes(ptr_event, NULL, MAX_LONG, _UTF8);
	msg_buf = (byte_t*)xmem_alloc(msg_len + 1);
	format_json_doc_to_bytes(ptr_event, msg_buf, msg_len, _UTF8);

	if (compare_text(sz_method, -1, HTTP_METHOD_GET, -1, 1) == 0)
	{
		destroy_event_doc(ptr_event);
		ptr_event = NULL;
	}
	else
	{
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
		ptr_event = NULL;
	}

	hkb = tkb_create(pd->path, pd->topic, FILETABLE_SHARE | FILETABLE_DIRECT);
	if (!hkb)
	{
		raise_user_error(_T("_invoke_event_pubs"), _T("open kv database failed"));
	}

	key = variant_alloc(VV_STRING_UTF8);
	variant_from_string(key, sz_name, -1);

	val = object_alloc();

	tkb_load(hkb, key, val);

	que = queue_alloc();

	if (object_size(val) > 0)
	{
		if (!object_get_queue(val, que))
		{
			raise_user_error(_T("_invoke_put"), _T("read queue falied"));
		}
	}

	hdr.ver = MSGVER_VENDOR;
	hdr.qos = 0;
	hdr.seq = 0;
	hdr.utc = get_timestamp();

	msg = message_alloc();
	message_write(msg, &hdr, msg_buf, msg_len);

	queue_write(que, msg);

	message_free(msg);
	msg = NULL;

	object_set_queue(val, que);
	queue_free(que);
	que = NULL;

	object_set_commpress(val, 1);
	tkb_save(hkb, key, val);

	variant_free(key);
	key = NULL;

	object_free(val);
	val = NULL;

	tkb_destroy(hkb);
	hkb = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_202, HTTP_CODE_202_TEXT, _T("0"), _T("send event success"), -1);

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (compare_text(sz_method, -1, HTTP_METHOD_GET, -1, 1) == 0)
	{
		if (ptr_event)
			destroy_event_doc(ptr_event);
	}
	else
	{
		if (ptr_json)
			destroy_json_doc(ptr_json);

		if (ptr_xml)
			destroy_xml_doc(ptr_xml);
	}

	if (hkb)
		tkb_destroy(hkb);

	if (key)
		variant_free(key);

	if (val)
		object_free(val);

	if (que)
		queue_free(que);

	if (msg)
		message_free(msg);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[EVENT]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

bool_t _invoke_event_subs(const https_block_t* pb, event_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_encoding[RES_LEN + 1] = { 0 };
	bool_t b_json = 0;

	tchar_t sz_name[RES_LEN + 1] = { 0 };

	link_t_ptr ptr_xml;
	link_t_ptr ptr_event = NULL;

	byte_t* msg_buf = NULL;
	dword_t msg_len = 0;

	t_kb_t hkb = NULL;

	variant_t key = NULL;
	object_t val = NULL;
	queue_t que = NULL;
	message_t msg = NULL;
	msg_hdr_t hdr = { 0 };

	bool_t rt;

	TRY_CATCH;

	xhttp_get_url_query_entity(pb->http, DOC_EVENT_ID, -1, sz_name, RES_LEN);
	if (is_null(sz_name))
	{
		raise_user_error(_T("_invoke_event_subs"), _T("unknown event id"));
	}

	xhttp_get_request_accept_type(pb->http, sz_encoding, RES_LEN);
	if (CONTENTTYPE_IS_ANY(sz_encoding))
	{
		xhttp_get_request_content_type(pb->http, sz_encoding, RES_LEN);
	}
	b_json = CONTENTTYPE_IS_JSON(sz_encoding);

	hkb = tkb_create(pd->path, pd->topic, FILETABLE_SHARE | FILETABLE_DIRECT);
	if (!hkb)
	{
		raise_user_error(_T("_invoke_event_subs"), _T("open kv database failed"));
	}

	key = variant_alloc(VV_STRING_UTF8);
	variant_from_string(key, sz_name, -1);

	val = object_alloc();

	tkb_load(hkb, key, val);

	que = queue_alloc();

	if (object_size(val) > 0)
	{
		if (!object_get_queue(val, que))
		{
			raise_user_error(_T("_invoke_event_subs"), _T("get queue failed"));
		}
	}

	msg = message_alloc();
	queue_read(que, msg);

	object_set_queue(val, que);
	queue_free(que);
	que = NULL;

	object_set_commpress(val, 1);
	tkb_save(hkb, key, val);

	variant_free(key);
	key = NULL;

	object_free(val);
	val = NULL;

	tkb_destroy(hkb);
	hkb = NULL;

	msg_len = message_read(msg, &hdr, NULL, MAX_LONG);
	msg_buf = (byte_t*)xmem_alloc(msg_len + 1);
	message_read(msg, NULL, msg_buf, msg_len);

	message_free(msg);
	msg = NULL;

	ptr_event = create_event_doc();

	if (!parse_json_doc_from_bytes(ptr_event, msg_buf, msg_len, _UTF8))
	{
		raise_user_error(_T("_invoke_event_subs"), _T("parse event failed"));
	}

	xmem_free(msg_buf);
	msg_buf = NULL;

	if (b_json)
	{
		xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);

		rt = xhttp_send_json(pb->http, ptr_event);
		if (!rt)
		{
			raise_user_error(NULL, NULL);
		}

		if (pb->plg)
		{
			(*pb->plg->pf_log_title)(pb->plg->unc, _T("[response]"), -1);

			(*pb->plg->pf_log_json)(pb->plg->unc, ptr_event);
		}
	}
	else
	{
		xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

		ptr_xml = upcast_dom_to_xml(ptr_event);
		rt = xhttp_send_xml(pb->http, ptr_xml);
		ptr_event = downcast_xml_to_dom(ptr_xml);
		ptr_xml = NULL;
		if (!rt)
		{
			raise_user_error(NULL, NULL);
		}

		if (pb->plg)
		{
			(*pb->plg->pf_log_title)(pb->plg->unc, _T("[response]"), -1);

			(*pb->plg->pf_log_json)(pb->plg->unc, ptr_event);
		}
	}

	destroy_event_doc(ptr_event);
	ptr_event = NULL;

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (ptr_event)
		destroy_event_doc(ptr_event);

	if (msg_buf)
		xmem_free(msg_buf);

	if (hkb)
		tkb_destroy(hkb);

	if (key)
		variant_free(key);

	if (val)
		object_free(val);

	if (que)
		queue_free(que);

	if (msg)
		message_free(msg);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[EVENT]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;

	return 0;
}

void _invoke_event_error(const https_block_t* pb, event_block_t* pd)
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

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[EVENT]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	XDL_TRACE_LAST;
}

/**********************************************************************************************/

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	event_block_t* pd = NULL;

	tchar_t file[PATH_LEN + 1] = { 0 };
	tchar_t token[RES_LEN + 1] = { 0 };

	bool_t rt = 1;

	link_t_ptr ptr_prop = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pd = (event_block_t*)xmem_alloc(sizeof(event_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/event.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(_T("-1"), _T("load loc config falied\n"));
	}

	read_proper(ptr_prop, _T("EVENT"), -1, _T("LOCATION"), -1, file, PATH_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->path, file);
	
	if (compare_text((pb->object + 1), 4, _T("pubs"), 4, 1) == 0)
	{
		xsncpy(pd->topic, (pb->object + 1 + 4 + 1), RES_LEN);
		if (is_null(pd->topic))
		{
			raise_user_error(_T("-1"), _T("unknown event topic\n"));
		}
		rt = _invoke_event_pubs(pb, pd);
	}
	else if (compare_text((pb->object + 1), 5, _T("subs"), 5, 1) == 0)
	{
		xsncpy(pd->topic, (pb->object + 1 + 5 + 1), RES_LEN);
		if (is_null(pd->topic))
		{
			raise_user_error(_T("-1"), _T("unknown event topic\n"));
		}
		rt = _invoke_event_subs(pb, pd);
	}
	else
	{
		raise_user_error(_T("-1"), _T("unknown event method\n"));
	}

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return (rt) ? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	_invoke_event_error(pb, pd);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	xmem_free(pd);

	return HTTPS_INVOKE_WITHINFO;
}