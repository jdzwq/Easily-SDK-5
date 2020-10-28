/***********************************************************************
	Easily hl7 web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hl7 web api document

	@module	hl7_api.cc | hl7 web api implement file

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

#include "hl7_api.h"

typedef struct _hl7_block_t{
	tchar_t topic_name[PATH_LEN + 1];

	secu_desc_t sd;
	tchar_t local[PATH_LEN + 1];
}hl7_block_t;

static void split_topic(const tchar_t* topic, tchar_t* cid, tchar_t* did, tchar_t* pid)
{
	const tchar_t* token = topic;
	int len;

	len = 0;
	while (*token != _T('/') && *token != _T('\\') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	xsncpy(cid, token - len, len);

	if (*token != _T('\0'))
		token++;

	len = 0;
	while (*token != _T('/') && *token != _T('\\') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	xsncpy(did, token - len, len);

	if (*token != _T('\0'))
		token++;

	len = 0;
	while (*token != _T('/') && *token != _T('\\') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	xsncpy(pid, token - len, len);
}

bool_t _invoke_head(const https_block_t* pb, hl7_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	rad_hdr_t hdr = { 0 };
	dword_t dw, total = 0;
	byte_t* buf = NULL;

	tk_db_t hdb = NULL;
	tk_kv_t hkv = NULL;

	xdate_t dt,dt_last = {0};
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };

	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	TRY_CATCH;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: HEAD]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), pd->topic_name, -1);
	}

	split_topic(pd->topic_name, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_head"), _T("unknown kv database"));
	}
	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hdb = tkdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_head"), _T("open kv database failed"));
	}

	hkv = tkkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_head"), _T("create tkdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	tkkv_read(hkv, key, val);

	total = 0;
	while ((dw = radobj_read(val, &hdr, NULL, MAX_LONG)) > 0)
	{
		buf = (byte_t*)xmem_alloc(dw);
		radobj_read(val, &hdr, buf, dw);

		parse_datetime(&dt, hdr.utc);

		if(compare_datetime(&dt, &dt_last) > 0)
		{
			xmem_copy((void*)&dt_last, (void*)&dt, sizeof(xdate_t));
		}

		xmem_free(buf);
		buf = NULL;

		total += dw;

		plus_millseconds(&dt, 1);
		format_utctime(&dt, hdr.utc);
	}

	variant_to_null(&key);

	object_free(val);
	val = NULL;

	tkkv_destroy(hkv);
	hkv = NULL;

	tkdb_destroy(hdb);
	hdb = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	ltoxs(total, fsize, INT_LEN);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
	format_utctime(&dt_last, ftime);
	xhttp_set_response_header(pb->http, HTTP_HEADER_LASTMODIFIED, -1, ftime, -1);
	xhttp_set_response_content_type(pb->http, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN_UTF8, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		tkkv_destroy(hkv);

	if (hdb)
		tkdb_destroy(hdb);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: HEAD]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_list(const https_block_t* pb, hl7_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	dword_t dw;

	tk_db_t hdb = NULL;
	tk_kv_t hkv = NULL;

	rad_hdr_t hdr = { 0 };
	tchar_t fsince[DATE_LEN + 1] = { 0 };
	xdate_t dt_first, dt = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };

	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	tchar_t sz_encoding[RES_LEN + 1] = { 0 };
	bool_t b_json = 0;

	LINKPTR ptr_json = NULL;
	LINKPTR ptr_xml = NULL;
	LINKPTR nlk_rowset, nlk_row, nlk;

	TRY_CATCH;

	xhttp_get_request_header(pb->http, HTTP_HEADER_IFMODIFIEDSINCE, -1, fsince, DATE_LEN);
	parse_gmttime(&dt_first, fsince);

	xhttp_get_request_accept_type(pb->http, sz_encoding, RES_LEN);
	b_json = CONTENTTYPE_IS_JSON(sz_encoding);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: LIST]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), pd->topic_name, -1);
	}

	if(b_json)
	{
		ptr_json = create_json_doc();
		nlk_rowset = ptr_json;
	}
	else
	{
		ptr_xml = create_xml_doc();
		set_xml_encoding(ptr_xml, DEF_XML_ENCODING, -1);
		nlk_rowset = get_xml_dom_node(ptr_xml);
	}
	set_dom_node_name(nlk_rowset, _T("rowset"), -1);

	split_topic(pd->topic_name, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_list"), _T("unknown kv database"));
	}
	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hdb = tkdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_list"), _T("open kv database failed"));
	}

	hkv = tkkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_list"), _T("create tkdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	tkkv_read(hkv, key, val);

	while ((dw = radobj_read(val, &hdr, NULL, MAX_LONG)) > 0)
	{
		parse_datetime(&dt, hdr.utc);

		if (compare_datetime(&dt, &dt_first) > 0)
		{
			nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);
			set_dom_node_name(nlk_row, _T("row"), -1);

			nlk = insert_dom_node(nlk_row, LINK_LAST);
			set_dom_node_name(nlk, _T("MessageName"), -1);
			set_dom_node_text(nlk, pid, -1);

			xsprintf(fsize, _T("%d"), dw);
			nlk = insert_dom_node(nlk_row, LINK_LAST);
			set_dom_node_name(nlk, _T("MessageSize"), -1);
			set_dom_node_text(nlk, fsize, -1);

			format_datetime(&dt, ftime);
			nlk = insert_dom_node(nlk_row, LINK_LAST);
			set_dom_node_name(nlk, _T("MessageTime"), -1);
			set_dom_node_text(nlk, ftime, -1);
		}

		plus_millseconds(&dt, 1);
		format_utctime(&dt, hdr.utc);
	}

	variant_to_null(&key);

	object_free(val);
	val = NULL;

	tkkv_destroy(hkv);
	hkv = NULL;

	tkdb_destroy(hdb);
	hdb = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	if(b_json)
		xhttp_set_response_content_type(pb->http, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	else
		xhttp_set_response_content_type(pb->http, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	if(b_json)
	{
		if (!xhttp_send_json(pb->http, ptr_json))
		{
			raise_user_error(NULL, NULL);
		}

		destroy_json_doc(ptr_json);
		ptr_json = NULL;
	}else
	{
		if (!xhttp_send_xml(pb->http, ptr_xml))
		{
			raise_user_error(NULL, NULL);
		}

		destroy_xml_doc(ptr_xml);
		ptr_xml = NULL;
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		tkkv_destroy(hkv);

	if (hdb)
		tkdb_destroy(hdb);

	if(ptr_json)
		destroy_json_doc(ptr_json);

	if(ptr_xml)
		destroy_xml_doc(ptr_xml);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: LIST]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_get(const https_block_t* pb, hl7_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	byte_t* buf = NULL;
	dword_t dw;

	tk_db_t hdb = NULL;
	tk_kv_t hkv = NULL;

	rad_hdr_t hdr = { 0 };
	tchar_t fsince[DATE_LEN + 1] = { 0 };
	xdate_t dt_first, dt = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };

	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	byte_t** pp = NULL;

	tchar_t sz_encoding[RES_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	bool_t b_json = 0;

	LINKPTR ptr_json = NULL;
	LINKPTR ptr_xml = NULL;
	LINKPTR nlk_rowset;

	TRY_CATCH;

	xhttp_get_request_header(pb->http, HTTP_HEADER_IFMODIFIEDSINCE, -1, fsince, DATE_LEN);
	parse_gmttime(&dt_first, fsince);

	xhttp_get_request_accept_type(pb->http, sz_encoding, RES_LEN);
	b_json = CONTENTTYPE_IS_JSON(sz_encoding);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: LIST]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), pd->topic_name, -1);
	}

	if (b_json)
	{
		ptr_json = create_json_doc();
		nlk_rowset = ptr_json;
	}
	else
	{
		ptr_xml = create_xml_doc();
		set_xml_encoding(ptr_xml, DEF_XML_ENCODING, -1);
		nlk_rowset = get_xml_dom_node(ptr_xml);
	}
	set_dom_node_name(nlk_rowset, _T("rowset"), -1);

	split_topic(pd->topic_name, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_list"), _T("unknown kv database"));
	}
	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hdb = tkdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_list"), _T("open kv database failed"));
	}

	hkv = tkkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_list"), _T("create tkdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	tkkv_read(hkv, key, val);

	while ((dw = radobj_read(val, &hdr, NULL, MAX_LONG)) > 0)
	{
		buf = (byte_t*)xmem_alloc(dw);
		radobj_read(val, &hdr, buf, dw);

		parse_datetime(&dt, hdr.utc);

		if (compare_datetime(&dt, &dt_first) > 0)
		{
			pp = bytes_alloc();

			bytes_insert(pp, 0, buf, dw);

			xmem_free(buf);
			buf = NULL;

			if (!load_mded_doc_from_bytes(nlk_rowset, pp))
			{
				raise_user_error(NULL, NULL);
			}

			bytes_free(pp);
			pp = NULL;

			break;
		}

		xmem_free(buf);
		buf = NULL;

		plus_millseconds(&dt, 1);
		format_utctime(&dt, hdr.utc);
	}

	variant_to_null(&key);

	object_free(val);
	val = NULL;

	tkkv_destroy(hkv);
	hkv = NULL;

	tkdb_destroy(hdb);
	hdb = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	if (b_json)
		xhttp_set_response_content_type(pb->http, HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8, -1);
	else
		xhttp_set_response_content_type(pb->http, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	if (b_json)
	{
		if (!xhttp_send_json(pb->http, ptr_json))
		{
			raise_user_error(NULL, NULL);
		}

		destroy_json_doc(ptr_json);
		ptr_json = NULL;
	}
	else
	{
		if (!xhttp_send_xml(pb->http, ptr_xml))
		{
			raise_user_error(NULL, NULL);
		}

		destroy_xml_doc(ptr_xml);
		ptr_xml = NULL;
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		tkkv_destroy(hkv);

	if (hdb)
		tkdb_destroy(hdb);

	if (pp)
		bytes_free(pp);

	if (ptr_json)
		destroy_json_doc(ptr_json);

	if (ptr_xml)
		destroy_xml_doc(ptr_xml);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: LIST]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_put(const https_block_t* pb, hl7_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	byte_t* buf = NULL;

	tk_db_t hdb = NULL;
	tk_kv_t hkv = NULL;

	rad_hdr_t hdr = { 0 };
	xdate_t dt = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };

	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	byte_t** pp = NULL;
	dword_t size;

	tchar_t sz_encoding[RES_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	bool_t b_json = 0;

	LINKPTR ptr_json = NULL;
	LINKPTR ptr_xml = NULL;
	LINKPTR nlk_rowset;

	TRY_CATCH;

	xhttp_get_request_content_type(pb->http, sz_encoding, RES_LEN);
	
	b_json = CONTENTTYPE_IS_JSON(sz_encoding);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: PUT]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), pd->topic_name, -1);
	}

	if(b_json)
	{
		ptr_json = create_json_doc();
		
		if(!xhttp_recv_json(pb->http, ptr_json))
		{
			raise_user_error(NULL, NULL);
		}

		nlk_rowset = ptr_json;
	}
	else
	{
		ptr_xml = create_xml_doc();

		if(!xhttp_recv_xml(pb->http, ptr_xml))
		{
			raise_user_error(NULL, NULL);
		}

		nlk_rowset = get_xml_dom_node(ptr_xml);
	}

	pp = bytes_alloc();

	if (!save_mded_doc_to_bytes(nlk_rowset, pp))
	{
		raise_user_error(NULL, NULL);
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

	size = bytes_size(pp);

	split_topic(pd->topic_name, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_put"), _T("unknown kv database"));
	}
	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hdb = tkdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_put"), _T("open kv database failed"));
	}

	hkv = tkkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_put"), _T("create tkdb kv entity falied"));
	}

	get_utc_date(&dt);
	format_utctime(&dt, ftime);

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	tkkv_read(hkv, key, val);

	xmem_copy((void*)hdr.ver, (void*)MSGVER_APPLICATION, MSGVER_SIZE);
	format_utctime(&dt, hdr.utc);

	radobj_write(val, &hdr, *pp, size);

	tkkv_attach(hkv, key, val);
	val = NULL;

	bytes_free(pp);
	pp = NULL;

	variant_to_null(&key);

	tkkv_destroy(hkv);
	hkv = NULL;

	tkdb_destroy(hdb);
	hdb = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (buf)
		xmem_free(buf);

	if (pp)
		bytes_free(pp);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		tkkv_destroy(hkv);

	if (hdb)
		tkdb_destroy(hdb);

	if(ptr_json)
		destroy_json_doc(ptr_json);

	if(ptr_xml)
		destroy_xml_doc(ptr_xml);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: PUT]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_delete(const https_block_t* pb, hl7_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	tk_db_t hdb = NULL;
	tk_kv_t hkv = NULL;

	tchar_t path[PATH_LEN + 1] = { 0 };
	tchar_t cid[UUID_LEN + 1] = { 0 };
	tchar_t did[UUID_LEN + 1] = { 0 };
	tchar_t pid[UUID_LEN + 1] = { 0 };

	TRY_CATCH;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: DELETE]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), pd->topic_name, -1);
	}

	split_topic(pd->topic_name, cid, did, pid);

	if (is_null(cid) || is_null(did))
	{
		raise_user_error(_T("_invoke_put"), _T("unknown kv database"));
	}
	xsprintf(path, _T("%s/%s"), pd->local, cid);

	hdb = tkdb_create(path, did);
	if (!hdb)
	{
		raise_user_error(_T("_invoke_put"), _T("open kv database failed"));
	}

	hkv = tkkv_create(hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_put"), _T("create tkdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pid, -1);

	val = object_alloc(_UTF8);

	tkkv_read(hkv, key, val);

	object_empty(val);

	tkkv_write(hkv, key, val);

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	tkkv_destroy(hkv);
	hkv = NULL;

	tkdb_destroy(hdb);
	hdb = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		tkkv_destroy(hkv);

	if (hdb)
		tkdb_destroy(hdb);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: DELETE]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

void _invoke_error(const https_block_t* pb, hl7_block_t* pd)
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
		(*pb->pf_log_title)(pb->log, _T("[HL7: ERROR]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	hl7_block_t* pd = NULL;

	tchar_t token[RES_LEN + 1] = {0};
	tchar_t file[PATH_LEN + 1] = { 0 };

	bool_t rt = 1;

	link_t_ptr ptr_prop = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pd = (hl7_block_t*)xmem_alloc(sizeof(hl7_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(file, _T("%s/hl7.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, file))
	{
		raise_user_error(_T("-1"), _T("load hl7 config falied\n"));
	}

	xszero(file, PATH_LEN);
	read_proper(ptr_prop, _T("HL7"), -1, _T("DATABASE"), -1, token, RES_LEN);
	read_proper(ptr_prop, _T("HL7"), -1, _T("LOCATION"), -1, file, PATH_LEN);
	read_proper(ptr_prop, _T("HL7"), -1, _T("PUBLICKEY"), -1, pd->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("HL7"), -1, _T("PRIVATEKEY"), -1, pd->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->local, file);

	xscpy(pd->topic_name, pb->object + 1);

	if (compare_text(method, -1, _T("HEAD"), -1, 1) == 0)
		rt = _invoke_head(pb, pd);
	else if (compare_text(method, -1, _T("LIST"), -1, 1) == 0)
		rt = _invoke_list(pb, pd);
	else if (compare_text(method, -1, _T("GET"), -1, 1) == 0)
		rt = _invoke_get(pb, pd);
	else if (compare_text(method, -1, _T("PUT"), -1, 1) == 0)
		rt = _invoke_put(pb, pd);
	else if (compare_text(method, -1, _T("DELETE"), -1, 1) == 0)
		rt = _invoke_delete(pb, pd);
	else
	{
		raise_user_error(_T("hl7_api"), _T("unknown hl7 method"));
	}

	xmem_free(pd);

	END_CATCH;

	return (rt) ? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	if(pd)
	{
		_invoke_error(pb, pd);
	}

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	if(pd)
		xmem_free(pd);

	return HTTPS_INVOKE_WITHINFO;
}