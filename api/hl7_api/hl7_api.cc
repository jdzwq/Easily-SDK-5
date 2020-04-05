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
	hex_obj_t hdb;

	tchar_t topic_name[PATH_LEN];

	secu_desc_t sd;
	tchar_t local[PATH_LEN];
}hl7_block_t;


//typedef struct _hl7_head_t{
	//byte_t timestamp[24];
	//byte_t longitude[10];
	//byte_t latitude[10];
	//byte_t altitude[16];
	//byte_t identify[36];
//};


bool_t _invoke_head(const https_block_t* pb, hl7_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	variant_t key = { 0 };
	object_t val = NULL;

	byte_t* buf = NULL;
	dword_t len;
	dword_t total = 0;

	dword_t msg_len;
	sword_t han_len;
	byte_t* hdr_buf = NULL;
	dword_t hdr_len;
	byte_t* hl7_buf = NULL;
	dword_t hl7_len;

	tchar_t timestamp[TIMESTAMP_SIZE + 1] = {0};
	tchar_t longitude[LONGITUDE_SIZE + 1] = {0};
	tchar_t latitude[LATITUDE_SIZE + 1] = {0};
	tchar_t altitude[ALTITUDE_SIZE + 1] = {0};
	tchar_t identify[IDENTIFY_SIZE + 1] = { 0 };

	hex_obj_t hkv = NULL;

	xdate_t dt,dt_last = {0};
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };

	TRY_CATCH;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: HEAD]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), pd->topic_name, -1);
	}

	hkv = hexkv_create(pd->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_head"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	len = object_get_bytes(val, NULL, MAX_LONG);
	xsprintf(fsize, _T("%d"), len);

	buf = (byte_t*)xmem_alloc(len);

	object_get_bytes(val, buf, len);

	while (total < len)
	{
		//the message total length
		msg_len = GET_DWORD_NET(buf, total);
		if (msg_len > MAX_LONG)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage packet size"));
		}
		total += 4;

		//the message handler length
		han_len = GET_SWORD_NET(buf, total);
		if (han_len > MAX_SHORT)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage handler size"));
		}
		total += 2;

		//skip handler
		total += han_len;
		//the message header length
		hdr_len = GET_SWORD_NET(buf, total);
		if (hdr_len > MAX_SHORT)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage header size"));
		}
		total += 2;

		hdr_buf = buf + total;

		if(hdr_len >= TIMESTAMP_SIZE)
		{
	#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs(hdr_buf, TIMESTAMP_SIZE, timestamp, TIMESTAMP_SIZE);
	#else
			utf8_to_mbs(hdr_buf, TIMESTAMP_SIZE, timestamp, TIMESTAMP_SIZE);
	#endif
		}else
		{
			xszero(timestamp, TIMESTAMP_SIZE);
		}

		if(hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE)
		{
	#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE), LONGITUDE_SIZE, longitude, LONGITUDE_SIZE);
	#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE), LONGITUDE_SIZE, longitude, LONGITUDE_SIZE);
	#endif
		}else
		{
			xszero(longitude, LONGITUDE_SIZE);
		}

		if(hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE)
		{
	#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE), LATITUDE_SIZE, latitude, LATITUDE_SIZE);
	#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE), LATITUDE_SIZE, latitude, LATITUDE_SIZE);
	#endif
		}else
		{
			xszero(latitude, LATITUDE_SIZE);
		}

		if(hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE)
		{
	#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE), ALTITUDE_SIZE, altitude, ALTITUDE_SIZE);
	#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE), ALTITUDE_SIZE, altitude, ALTITUDE_SIZE);
	#endif
		}else
		{
			xszero(altitude, ALTITUDE_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE + IDENTIFY_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE), IDENTIFY_SIZE, identify, IDENTIFY_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE), IDENTIFY_SIZE, identify, IDENTIFY_SIZE);
#endif
		}
		else
		{
			xszero(identify, IDENTIFY_SIZE);
		}

		parse_datetime(&dt, timestamp);

		if(compare_datetime(&dt, &dt_last) > 0)
		{
			xmem_copy((void*)&dt_last, (void*)&dt, sizeof(xdate_t));
		}

		total += hdr_len;

		hl7_len = GET_DWORD_NET(buf, total);
		if (hl7_len > (len - 4))
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage data size"));
		}
		total += 4;

		//the message data
		hl7_buf = buf + total;

		if (msg_len != (2 + han_len + 2 + hdr_len + 4 + hl7_len))
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage total size"));
		}
		//skip message data
		total += hl7_len;
	}

	xmem_free(buf);
	buf = NULL;

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	hexkv_destroy(hkv);
	hkv = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

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
		hexkv_destroy(hkv);

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

	byte_t* buf = NULL;
	dword_t len;
	dword_t total = 0;

	dword_t msg_len;
	sword_t han_len;
	byte_t* hdr_buf = NULL;
	dword_t hdr_len;
	byte_t* hl7_buf = NULL;
	dword_t hl7_len;

	tchar_t timestamp[TIMESTAMP_SIZE + 1] = {0};
	tchar_t longitude[LONGITUDE_SIZE + 1] = {0};
	tchar_t latitude[LATITUDE_SIZE + 1] = {0};
	tchar_t altitude[ALTITUDE_SIZE + 1] = {0};
	tchar_t identify[IDENTIFY_SIZE + 1] = { 0 };

	hex_obj_t hkv = NULL;

	tchar_t sz_encoding[RES_LEN] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	bool_t b_json = 0;

	LINKPTR ptr_json = NULL;
	LINKPTR ptr_xml = NULL;
	LINKPTR nlk_rowset, nlk_row, nlk;

	TRY_CATCH;

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

	hkv = hexkv_create(pd->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_head"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	len = object_get_bytes(val, NULL, MAX_LONG);

	buf = (byte_t*)xmem_alloc(len);

	object_get_bytes(val, buf, len);

	while (total < len)
	{
		//the message packet length
		msg_len = GET_DWORD_NET(buf, total);
		if (msg_len > MAX_LONG)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage packet size"));
		}
		total += 4;

		//the message handler length
		han_len = GET_SWORD_NET(buf, total);
		if (han_len > MAX_SHORT)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage handler size"));
		}
		total += 2;

		//skip handler
		total += han_len;
		//the message header length
		hdr_len = GET_SWORD_NET(buf, total);
		if (hdr_len > MAX_SHORT)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage header size"));
		}
		total += 2;

		hdr_buf = buf + total;

		if (hdr_len >= TIMESTAMP_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs(hdr_buf, TIMESTAMP_SIZE, timestamp, TIMESTAMP_SIZE);
#else
			utf8_to_mbs(hdr_buf, TIMESTAMP_SIZE, timestamp, TIMESTAMP_SIZE);
#endif
		}
		else
		{
			xszero(timestamp, TIMESTAMP_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE), LONGITUDE_SIZE, longitude, LONGITUDE_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE), LONGITUDE_SIZE, longitude, LONGITUDE_SIZE);
#endif
		}
		else
		{
			xszero(longitude, LONGITUDE_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE), LATITUDE_SIZE, latitude, LATITUDE_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE), LATITUDE_SIZE, latitude, LATITUDE_SIZE);
#endif
		}
		else
		{
			xszero(latitude, LATITUDE_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE), ALTITUDE_SIZE, altitude, ALTITUDE_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE), ALTITUDE_SIZE, altitude, ALTITUDE_SIZE);
#endif
		}
		else
		{
			xszero(altitude, ALTITUDE_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE + IDENTIFY_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE), IDENTIFY_SIZE, identify, IDENTIFY_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE), IDENTIFY_SIZE, identify, IDENTIFY_SIZE);
#endif
		}
		else
		{
			xszero(identify, IDENTIFY_SIZE);
		}

		total += hdr_len;

		hl7_len = GET_DWORD_NET(buf, total);
		if (hl7_len > (len - 4))
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage data size"));
		}
		total += 4;

		nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);
		set_dom_node_name(nlk_row, _T("row"), -1);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_TIMESTAMP, -1);
		set_dom_node_text(nlk, timestamp, TIMESTAMP_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_LONGITUDE, -1);
		set_dom_node_text(nlk, longitude, LONGITUDE_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_LATITUDE, -1);
		set_dom_node_text(nlk, latitude, LATITUDE_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_ALTITUDE, -1);
		set_dom_node_text(nlk, altitude, ALTITUDE_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_IDENTIFY, -1);
		set_dom_node_text(nlk, identify, IDENTIFY_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_PACKSIZE, -1);
		ltoxs(hl7_len, fsize, NUM_LEN);
		set_dom_node_text(nlk, fsize, -1);

		//the message data
		hl7_buf = buf + total;

		if (msg_len != (2 + han_len + 2 + hdr_len + 4 + hl7_len))
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage total size"));
		}
		//skip message data
		total += hl7_len;
	}

	xmem_free(buf);
	buf = NULL;

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	hexkv_destroy(hkv);
	hkv = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	if(b_json)
		len = format_json_doc_to_bytes(ptr_json, NULL, MAX_LONG, _UTF8);
	else
		len = format_xml_doc_to_bytes(ptr_xml, NULL, MAX_LONG);

	xsprintf(fsize, _T("%d"), len);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);

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

	if (buf)
		xmem_free(buf);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		hexkv_destroy(hkv);

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
	dword_t len;
	dword_t total = 0;

	dword_t msg_len;
	sword_t han_len;
	byte_t* hdr_buf = NULL;
	dword_t hdr_len;
	byte_t* hl7_buf = NULL;
	dword_t hl7_len;

	tchar_t timestamp[TIMESTAMP_SIZE + 1] = {0};
	tchar_t longitude[LONGITUDE_SIZE + 1] = {0};
	tchar_t latitude[LATITUDE_SIZE + 1] = {0};
	tchar_t altitude[ALTITUDE_SIZE + 1] = {0};
	tchar_t identify[IDENTIFY_SIZE + 1] = { 0 };
	tchar_t *package = NULL;
	int pkg_len;

	hex_obj_t hkv = NULL;

	tchar_t sz_encoding[RES_LEN] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	bool_t b_json = 0;

	LINKPTR ptr_json = NULL;
	LINKPTR ptr_xml = NULL;
	LINKPTR nlk_rowset, nlk_row, nlk;

	TRY_CATCH;

	xhttp_get_request_accept_type(pb->http, sz_encoding, RES_LEN);
	
	b_json = CONTENTTYPE_IS_JSON(sz_encoding);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: GET]"), -1);

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

	hkv = hexkv_create(pd->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_get"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	len = object_get_bytes(val, NULL, MAX_LONG);

	buf = (byte_t*)xmem_alloc(len);

	object_get_bytes(val, buf, len);

	while (total < len)
	{
		//the message packet length
		msg_len = GET_DWORD_NET(buf, total);
		if (msg_len > MAX_LONG)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage packet size"));
		}
		total += 4;

		//the message handler length
		han_len = GET_SWORD_NET(buf, total);
		if (han_len > MAX_SHORT)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage handler size"));
		}
		total += 2;

		//skip handler
		total += han_len;
		//the message header length
		hdr_len = GET_SWORD_NET(buf, total);
		if (hdr_len > MAX_SHORT)
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage header size"));
		}
		total += 2;

		hdr_buf = buf + total;

		if (hdr_len >= TIMESTAMP_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs(hdr_buf, TIMESTAMP_SIZE, timestamp, TIMESTAMP_SIZE);
#else
			utf8_to_mbs(hdr_buf, TIMESTAMP_SIZE, timestamp, TIMESTAMP_SIZE);
#endif
	}
		else
		{
			xszero(timestamp, TIMESTAMP_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE), LONGITUDE_SIZE, longitude, LONGITUDE_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE), LONGITUDE_SIZE, longitude, LONGITUDE_SIZE);
#endif
		}
		else
		{
			xszero(longitude, LONGITUDE_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE), LATITUDE_SIZE, latitude, LATITUDE_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE), LATITUDE_SIZE, latitude, LATITUDE_SIZE);
#endif
		}
		else
		{
			xszero(latitude, LATITUDE_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE), ALTITUDE_SIZE, altitude, ALTITUDE_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE), ALTITUDE_SIZE, altitude, ALTITUDE_SIZE);
#endif
		}
		else
		{
			xszero(altitude, ALTITUDE_SIZE);
		}

		if (hdr_len >= TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE + IDENTIFY_SIZE)
		{
#if defined(_UNICODE) || defined(UNICODE)
			utf8_to_ucs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE), IDENTIFY_SIZE, identify, IDENTIFY_SIZE);
#else
			utf8_to_mbs((hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE), IDENTIFY_SIZE, identify, IDENTIFY_SIZE);
#endif
		}
		else
		{
			xszero(identify, IDENTIFY_SIZE);
		}

		total += hdr_len;

		hl7_len = GET_DWORD_NET(buf, total);
		if (hl7_len > (len - 4))
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage data size"));
		}
		total += 4;

		//the message data
		hl7_buf = buf + total;

	#if defined(_UNICODE) || defined(UNICODE)
		pkg_len = utf8_to_ucs(hl7_buf, hl7_len, NULL, MAX_LONG);
	#else
		pkg_len = utf8_to_mbs(hl7_buf, hl7_len, NULL, MAX_LONG);
	#endif

		package = xsalloc(pkg_len + 1);

	#if defined(_UNICODE) || defined(UNICODE)
		pkg_len = utf8_to_ucs(hl7_buf, hl7_len, package, pkg_len);
	#else
		pkg_len = utf8_to_mbs(hl7_buf, hl7_len, package, pkg_len);
	#endif

		nlk_row = insert_dom_node(nlk_rowset, LINK_LAST);
		set_dom_node_name(nlk_row, _T("row"), -1);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_TIMESTAMP, -1);
		set_dom_node_text(nlk, timestamp, TIMESTAMP_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_LONGITUDE, -1);
		set_dom_node_text(nlk, longitude, LONGITUDE_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_LATITUDE, -1);
		set_dom_node_text(nlk, latitude, LATITUDE_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_ALTITUDE, -1);
		set_dom_node_text(nlk, altitude, ALTITUDE_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_IDENTIFY, -1);
		set_dom_node_text(nlk, identify, IDENTIFY_SIZE);

		nlk = insert_dom_node(nlk_row, LINK_LAST);
		set_dom_node_name(nlk, HL7_ELEMENT_PACKAGE, -1);
		set_dom_node_text(nlk, package, pkg_len);

		xsfree(package);
		package = NULL;
		pkg_len = 0;

		if (msg_len != (2 + han_len + 2 + hdr_len + 4 + hl7_len))
		{
			raise_user_error(_T("_invoke_head"), _T("invalid mesage total size"));
		}
		//skip message data
		total += hl7_len;
	}

	xmem_free(buf);
	buf = NULL;

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	hexkv_destroy(hkv);
	hkv = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	if(b_json)
		len = format_json_doc_to_bytes(ptr_json, NULL, MAX_LONG, _UTF8);
	else
		len = format_xml_doc_to_bytes(ptr_xml, NULL, MAX_LONG);

	xsprintf(fsize, _T("%d"), len);
	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);

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

	if (buf)
		xmem_free(buf);

	if(package)
		xsfree(package);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		hexkv_destroy(hkv);

	if(ptr_json)
		destroy_json_doc(ptr_json);

	if(ptr_xml)
		destroy_xml_doc(ptr_xml);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: GET]"), -1);

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
	dword_t len;
	dword_t total = 0;

	byte_t** bb = NULL;
	dword_t bs = 0;

	byte_t* msg_buf = NULL;
	dword_t msg_len;
	byte_t* han_buf = NULL;
	sword_t han_len;
	byte_t* hdr_buf = NULL;
	dword_t hdr_len;
	byte_t* hl7_buf = NULL;
	dword_t hl7_len;

	tchar_t timestamp[TIMESTAMP_SIZE + 1] = {0};
	tchar_t longitude[LONGITUDE_SIZE + 1] = {0};
	tchar_t latitude[LATITUDE_SIZE + 1] = {0};
	tchar_t altitude[ALTITUDE_SIZE + 1] = {0};
	tchar_t identify[IDENTIFY_SIZE + 1] = { 0 };
	const tchar_t* package = NULL;
	int pkg_len = 0;

	hex_obj_t hkv = NULL;

	tchar_t sz_encoding[RES_LEN] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	bool_t b_json = 0;

	xdate_t dt1, dt2;
	int tag = 0;

	LINKPTR ptr_json = NULL;
	LINKPTR ptr_xml = NULL;
	LINKPTR nlk_rowset, nlk_row, nlk;

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

	nlk_row = get_dom_first_child_node(nlk_rowset);

	hkv = hexkv_create(pd->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_head"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	len = object_get_bytes(val, NULL, MAX_LONG);

	bb = bytes_alloc();

	bytes_realloc(bb, len);

	object_get_bytes(val, *bb, len);

	while(nlk_row)
	{
		nlk = get_dom_first_child_node(nlk_row);
		
		xszero(timestamp, TIMESTAMP_SIZE);
		xszero(longitude, LONGITUDE_SIZE);
		xszero(latitude, LATITUDE_SIZE);
		xszero(altitude, ALTITUDE_SIZE);
		xszero(identify, IDENTIFY_SIZE);

		package = NULL;
		pkg_len = 0;

		while(nlk)
		{
			if(compare_text(get_dom_node_name_ptr(nlk),-1,HL7_ELEMENT_TIMESTAMP,-1,1) == 0)
			{
				get_dom_node_text(nlk, timestamp, TIMESTAMP_SIZE);
			}else if(compare_text(get_dom_node_name_ptr(nlk),-1,HL7_ELEMENT_LONGITUDE,-1,1) == 0)
			{
				get_dom_node_text(nlk, longitude, LONGITUDE_SIZE);
			}else if(compare_text(get_dom_node_name_ptr(nlk),-1,HL7_ELEMENT_LATITUDE,-1,1) == 0)
			{
				get_dom_node_text(nlk, latitude, LATITUDE_SIZE);
			}else if(compare_text(get_dom_node_name_ptr(nlk),-1,HL7_ELEMENT_ALTITUDE,-1,1) == 0)
			{
				get_dom_node_text(nlk, altitude, ALTITUDE_SIZE);
			}
			else if (compare_text(get_dom_node_name_ptr(nlk), -1, HL7_ELEMENT_ALTITUDE, -1, 1) == 0)
			{
				get_dom_node_text(nlk, altitude, ALTITUDE_SIZE);
			}
			else if (compare_text(get_dom_node_name_ptr(nlk), -1, HL7_ELEMENT_IDENTIFY, -1, 1) == 0)
			{
				get_dom_node_text(nlk, identify, IDENTIFY_SIZE);
			}else if(compare_text(get_dom_node_name_ptr(nlk),-1,HL7_ELEMENT_PACKAGE,-1,1) == 0)
			{
				package = get_dom_node_text_ptr(nlk);
				pkg_len = xslen(package);
			}

			nlk = get_dom_next_sibling_node(nlk);
		}

#if defined(_UNICODE) || defined(UNICODE)
		len = ucs_to_utf8(package, pkg_len, NULL, MAX_LONG);
#else
		len = mbs_to_utf8(package, pkg_len, NULL, MAX_LONG);
#endif

		msg_len = 2 + HL7HAND_SIZE + 2 + HL7HEADER_SIZE + 4 + len;
		buf = (byte_t*)xmem_alloc(4 + msg_len);
		msg_buf = buf + 4;
		PUT_DWORD_NET((msg_buf - 4), 0, msg_len);

		han_buf = msg_buf + 2;
		PUT_SWORD_NET((han_buf - 2), 0, HL7HAND_SIZE);
		xmem_copy((void*)han_buf, (void*)HL7VER, HL7HAND_SIZE);

		hdr_buf = han_buf + HL7HAND_SIZE + 2;
		PUT_SWORD_NET((hdr_buf - 2), 0, HL7HEADER_SIZE);

		hl7_buf = hdr_buf + HL7HEADER_SIZE + 4;
		PUT_DWORD_NET((hl7_buf - 4), 0, len);

#if defined(_UNICODE) || defined(UNICODE)
		ucs_to_utf8(timestamp, TIMESTAMP_SIZE, hdr_buf, TIMESTAMP_SIZE);
		ucs_to_utf8(longitude, LONGITUDE_SIZE, (hdr_buf + TIMESTAMP_SIZE), LONGITUDE_SIZE);
		ucs_to_utf8(latitude, LATITUDE_SIZE, (hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE), LATITUDE_SIZE);
		ucs_to_utf8(altitude, ALTITUDE_SIZE, (hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE), ALTITUDE_SIZE);
		ucs_to_utf8(identify, IDENTIFY_SIZE, (hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE), IDENTIFY_SIZE);
		ucs_to_utf8(package, pkg_len, hl7_buf, len);
#else
		mbs_to_utf8(timestamp, TIMESTAMP_SIZE, hdr_buf, TIMESTAMP_SIZE);
		mbs_to_utf8(longitude, LONGITUDE_SIZE, (hdr_buf + TIMESTAMP_SIZE), LONGITUDE_SIZE);
		mbs_to_utf8(latitude, LATITUDE_SIZE, (hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE), LATITUDE_SIZE);
		mbs_to_utf8(altitude, ALTITUDE_SIZE, (hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE), ALTITUDE_SIZE);
		mbs_to_utf8(identify, IDENTIFY_SIZE, (hdr_buf + TIMESTAMP_SIZE + LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE), IDENTIFY_SIZE);
		mbs_to_utf8(package, pkg_len, hl7_buf, len);
#endif

		bs = bytes_size(bb);
		bytes_insert(bb, bs, buf, msg_len + 4);

		xmem_free(buf);
		buf = NULL;

		nlk_row = get_dom_next_sibling_node(nlk_row);
	}

	object_empty(val);
	bs = bytes_size(bb);
	object_set_bytes(val, _UTF8, *bb, bs);

	bytes_free(bb);
	bb = NULL;

	hexkv_write(hkv, key, val);

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	hexkv_destroy(hkv);
	hkv = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	if(b_json)
	{
		destroy_json_doc(ptr_json);
		ptr_json = NULL;
	}
	else
	{
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

	if(bb)
		bytes_free(bb);

	variant_to_null(&key);

	if (val)
		object_free(val);

	if (hkv)
		hexkv_destroy(hkv);

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

	hex_obj_t hkv = NULL;

	TRY_CATCH;

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[HL7: DELETE]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), pd->topic_name, -1);
	}

	hkv = hexkv_create(pd->hdb);
	if (!hkv)
	{
		raise_user_error(_T("_invoke_delete"), _T("create hexdb kv entity falied"));
	}

	key.vv = VV_STRING;
	variant_from_string(&key, pd->topic_name, -1);

	val = object_alloc(_UTF8);

	hexkv_read(hkv, key, val);

	object_empty(val);

	hexkv_write(hkv, key, val);

	variant_to_null(&key);
	object_free(val);
	val = NULL;

	hexkv_destroy(hkv);
	hkv = NULL;

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
		hexkv_destroy(hkv);

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

	tchar_t token[RES_LEN] = {0};
	tchar_t file[PATH_LEN] = { 0 };

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
	
	pd->hdb = hexdb_create(pd->local, token);
	if (!pd->hdb)
	{
		raise_user_error(_T("-1"), _T("open database failed"));
	}

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

	hexdb_destroy(pd->hdb);
	pd->hdb = NULL;

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
	{
		if (pd->hdb)
			hexdb_destroy(pd->hdb);

		xmem_free(pd);
	}

	return HTTPS_INVOKE_WITHINFO;
}