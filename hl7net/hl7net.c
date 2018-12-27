/***********************************************************************
	Easily HL7 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hl7 document

	@module	hl7net.c | redhex network document implement file

	@devnote 张文权 2018.01 - 2018.12	v1.0
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

#include "hl7net.h"

static dword_t _hl7net_format_session(hl7net_t* hl7net, byte_t* pdv_data, dword_t pdv_size)
{
	dword_t len, pos = 0;

	//sesscion pdv: {session_ver:2 session_len:2 session_id:n username_len:2 username:n password_len:2 password:n}

	if (pos + 2 > pdv_size) return pos;
	if (pdv_data)
	{
		PUT_SWORD_NET(pdv_data, pos, hl7net->session_ver);
	}
	pos += 2;

	if (pos + 2 > pdv_size) return pos;
	if (pdv_data)
	{
		PUT_SWORD_NET(pdv_data, pos, hl7net->session_len);
	}
	pos += 2;

	if (pos + hl7net->session_len > pdv_size) return pos;
	if (pdv_data)
	{
		xmem_copy((void*)(pdv_data + pos), hl7net->session_cid, hl7net->session_len);
	}
	pos += hl7net->session_len;

	switch (hl7net->encode)
	{
	case _GB2312:
		len = ucs_to_gb2312(hl7net->username, hl7net->username_length, NULL, MAX_LONG);
		break;
	case _UTF8:
		len = ucs_to_utf8(hl7net->username, hl7net->username_length, NULL, MAX_LONG);
		break;
	case _UTF16_LIT:
		len = ucs_to_utf16lit(hl7net->username, hl7net->username_length, NULL, MAX_LONG);
		break;
	case _UTF16_BIG:
		len = ucs_to_utf16big(hl7net->username, hl7net->username_length, NULL, MAX_LONG);
		break;
	}

	if (pos + 2 > pdv_size) return pos;
	if (pdv_data)
	{
		PUT_SWORD_NET(pdv_data, pos, len);
	}
	pos += 2;

	if (pos + len > pdv_size) return pos;
	if (pdv_data)
	{
		switch (hl7net->encode)
		{
		case _GB2312:
			len = ucs_to_gb2312(hl7net->username, hl7net->username_length, pdv_data + pos, len);
			break;
		case _UTF8:
			len = ucs_to_utf8(hl7net->username, hl7net->username_length, pdv_data + pos, len);
			break;
		case _UTF16_LIT:
			len = ucs_to_utf16lit(hl7net->username, hl7net->username_length, pdv_data + pos, len);
			break;
		case _UTF16_BIG:
			len = ucs_to_utf16big(hl7net->username, hl7net->username_length, pdv_data + pos, len);
			break;
		}
	}
	pos += len;

	switch (hl7net->encode)
	{
	case _GB2312:
		len = ucs_to_gb2312(hl7net->password, hl7net->password_length, NULL, MAX_LONG);
		break;
	case _UTF8:
		len = ucs_to_utf8(hl7net->password, hl7net->password_length, NULL, MAX_LONG);
		break;
	case _UTF16_LIT:
		len = ucs_to_utf16lit(hl7net->password, hl7net->password_length, NULL, MAX_LONG);
		break;
	case _UTF16_BIG:
		len = ucs_to_utf16big(hl7net->password, hl7net->password_length, NULL, MAX_LONG);
		break;
	}

	if (pos + 2 > pdv_size) return pos;
	if (pdv_data)
	{
		PUT_SWORD_NET(pdv_data, pos, len);
	}
	pos += 2;

	if (pos + len > pdv_size) return pos;
	if (pdv_data)
	{
		switch (hl7net->encode)
		{
		case _GB2312:
			len = ucs_to_gb2312(hl7net->password, hl7net->password_length, pdv_data + pos, len);
			break;
		case _UTF8:
			len = ucs_to_utf8(hl7net->password, hl7net->password_length, pdv_data + pos, len);
			break;
		case _UTF16_LIT:
			len = ucs_to_utf16lit(hl7net->password, hl7net->password_length, pdv_data + pos, len);
			break;
		case _UTF16_BIG:
			len = ucs_to_utf16big(hl7net->password, hl7net->password_length, pdv_data + pos, len);
			break;
		}
	}
	pos += len;

	return pos;
}

static bool_t _hl7net_parse_session(hl7net_t* hl7net, const byte_t* pdv_data, dword_t pdv_size)
{
	dword_t len, pos = 0;

	//sesscion pdv: {session_ver:2 session_len:2 session_id:n username_len:2 username:n password_len:2 password:n}

	if (pos + 2 > pdv_size) return 0;
	hl7net->session_ver = GET_SWORD_NET(pdv_data, pos);
	pos += 2;

	if (pos + 2 > pdv_size) return 0;
	hl7net->session_len = GET_SWORD_NET(pdv_data, pos);
	pos += 2;

	if (pos + hl7net->session_len > pdv_size) return 0;

	hl7net->session_cid = (byte_t*)xmem_realloc(hl7net->session_cid, hl7net->session_len);
	xmem_copy((void*)(hl7net->session_cid), (void*)(pdv_data + pos), hl7net->session_len);
	pos += hl7net->session_len;

	if (pos + 2 > pdv_size) return 0;

	len = GET_SWORD_NET(pdv_data, pos);
	pos += 2;

	if (pos + len > pdv_size) return 0;

	switch (hl7net->encode)
	{
	case _GB2312:
		hl7net->username_length = gb2312_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF8:
		hl7net->username_length = utf8_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF16_LIT:
		hl7net->username_length = utf16lit_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF16_BIG:
		hl7net->username_length = utf16big_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	}

	hl7net->username = xsrealloc(hl7net->username, hl7net->username_length + 1);

	switch (hl7net->encode)
	{
	case _GB2312:
		gb2312_to_ucs(pdv_data + pos, len, hl7net->username, hl7net->username_length);
		break;
	case _UTF8:
		utf8_to_ucs(pdv_data + pos, len, hl7net->username, hl7net->username_length);
		break;
	case _UTF16_LIT:
		utf16lit_to_ucs(pdv_data + pos, len, hl7net->username, hl7net->username_length);
		break;
	case _UTF16_BIG:
		utf16big_to_ucs(pdv_data + pos, len, hl7net->username, hl7net->username_length);
		break;
	}

	pos += len;

	if (pos + 2 > pdv_size) return 0;

	len = GET_SWORD_NET(pdv_data, pos);
	pos += 2;

	if (pos + len > pdv_size) return 0;

	switch (hl7net->encode)
	{
	case _GB2312:
		hl7net->password_length = gb2312_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF8:
		hl7net->password_length = utf8_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF16_LIT:
		hl7net->password_length = utf16lit_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF16_BIG:
		hl7net->password_length = utf16big_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	}

	hl7net->password = xsrealloc(hl7net->password, hl7net->password_length + 1);

	switch (hl7net->encode)
	{
	case _GB2312:
		gb2312_to_ucs(pdv_data + pos, len, hl7net->password, hl7net->password_length);
		break;
	case _UTF8:
		utf8_to_ucs(pdv_data + pos, len, hl7net->password, hl7net->password_length);
		break;
	case _UTF16_LIT:
		utf16lit_to_ucs(pdv_data + pos, len, hl7net->password, hl7net->password_length);
		break;
	case _UTF16_BIG:
		utf16big_to_ucs(pdv_data + pos, len, hl7net->password, hl7net->password_length);
		break;
	}

	pos += len;

	return 1;
}

static void _hl7net_clean_session(hl7net_t* hl7net)
{
	if (hl7net->session_cid)
	{
		xmem_free(hl7net->session_cid);
		hl7net->session_len = 0;
	}

	if (hl7net->username)
	{
		xmem_free(hl7net->username);
		hl7net->username_length = 0;
	}

	if (hl7net->password)
	{
		xmem_free(hl7net->password);
		hl7net->password_length = 0;
	}

	if (hl7net->topic_name)
	{
		xmem_free(hl7net->topic_name);
		hl7net->topic_length = 0;
	}

	if (hl7net->error_text)
	{
		xmem_free(hl7net->error_text);
		hl7net->error_length = 0;
	}
}

static dword_t _hl7net_format_topic(hl7net_t* hl7net, byte_t* pdv_data, dword_t pdv_size)
{
	dword_t len, pos = 0;

	//subscribe pdv: {topic_size:2 topic_name:n}

	switch (hl7net->encode)
	{
	case _GB2312:
		len = ucs_to_gb2312(hl7net->topic_name, hl7net->topic_length, NULL, MAX_LONG);
		break;
	case _UTF8:
		len = ucs_to_utf8(hl7net->topic_name, hl7net->topic_length, NULL, MAX_LONG);
		break;
	case _UTF16_LIT:
		len = ucs_to_utf16lit(hl7net->topic_name, hl7net->topic_length, NULL, MAX_LONG);
		break;
	case _UTF16_BIG:
		len = ucs_to_utf16big(hl7net->topic_name, hl7net->topic_length, NULL, MAX_LONG);
		break;
	}

	if (pos + 2 > pdv_size) return pos;
	if (pdv_data)
	{
		PUT_SWORD_NET(pdv_data, pos, len);
	}
	pos += 2;

	if (pos + len > pdv_size) return pos;
	if (pdv_data)
	{
		switch (hl7net->encode)
		{
		case _GB2312:
			len = ucs_to_gb2312(hl7net->topic_name, hl7net->topic_length, pdv_data + pos, len);
			break;
		case _UTF8:
			len = ucs_to_utf8(hl7net->topic_name, hl7net->topic_length, pdv_data + pos, len);
			break;
		case _UTF16_LIT:
			len = ucs_to_utf16lit(hl7net->topic_name, hl7net->topic_length, pdv_data + pos, len);
			break;
		case _UTF16_BIG:
			len = ucs_to_utf16big(hl7net->topic_name, hl7net->topic_length, pdv_data + pos, len);
			break;
		}
	}
	pos += len;

	return pos;
}

static bool_t _hl7net_parse_topic(hl7net_t* hl7net, const byte_t* pdv_data, dword_t pdv_size)
{
	dword_t len, pos = 0;

	//subscribe pdv: {topic_size:2 topic_name:n}

	len = GET_SWORD_NET(pdv_data, pos);
	pos += 2;

	switch (hl7net->encode)
	{
	case _GB2312:
		hl7net->topic_length = gb2312_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF8:
		hl7net->topic_length = utf8_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF16_LIT:
		hl7net->topic_length = utf16lit_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF16_BIG:
		hl7net->topic_length = utf16big_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	}

	hl7net->topic_name = xsrealloc(hl7net->topic_name, hl7net->topic_length + 1);

	switch (hl7net->encode)
	{
	case _GB2312:
		gb2312_to_ucs(pdv_data + pos, len, hl7net->topic_name, hl7net->topic_length);
		break;
	case _UTF8:
		utf8_to_ucs(pdv_data + pos, len, hl7net->topic_name, hl7net->topic_length);
		break;
	case _UTF16_LIT:
		utf16lit_to_ucs(pdv_data + pos, len, hl7net->topic_name, hl7net->topic_length);
		break;
	case _UTF16_BIG:
		utf16big_to_ucs(pdv_data + pos, len, hl7net->topic_name, hl7net->topic_length);
		break;
	}

	pos += len;

	return 1;
}

static dword_t _hl7net_format_error(hl7net_t* hl7net, byte_t* pdv_data, dword_t pdv_size)
{
	dword_t len, pos = 0;

	//subscribe pdv: {topic_size:2 topic_name:n}

	switch (hl7net->encode)
	{
	case _GB2312:
		len = ucs_to_gb2312(hl7net->error_text, hl7net->error_length, NULL, MAX_LONG);
		break;
	case _UTF8:
		len = ucs_to_utf8(hl7net->error_text, hl7net->error_length, NULL, MAX_LONG);
		break;
	case _UTF16_LIT:
		len = ucs_to_utf16lit(hl7net->error_text, hl7net->error_length, NULL, MAX_LONG);
		break;
	case _UTF16_BIG:
		len = ucs_to_utf16big(hl7net->error_text, hl7net->error_length, NULL, MAX_LONG);
		break;
	}

	if (pos + 2 > pdv_size) return pos;
	if (pdv_data)
	{
		PUT_SWORD_NET(pdv_data, pos, len);
	}
	pos += 2;

	if (pos + len > pdv_size) return pos;
	if (pdv_data)
	{
		switch (hl7net->encode)
		{
		case _GB2312:
			len = ucs_to_gb2312(hl7net->error_text, hl7net->error_length, pdv_data + pos, len);
			break;
		case _UTF8:
			len = ucs_to_utf8(hl7net->error_text, hl7net->error_length, pdv_data + pos, len);
			break;
		case _UTF16_LIT:
			len = ucs_to_utf16lit(hl7net->error_text, hl7net->error_length, pdv_data + pos, len);
			break;
		case _UTF16_BIG:
			len = ucs_to_utf16big(hl7net->error_text, hl7net->error_length, pdv_data + pos, len);
			break;
		}
	}
	pos += len;

	return pos;
}

static bool_t _hl7net_parse_error(hl7net_t* hl7net, const byte_t* pdv_data, dword_t pdv_size)
{
	dword_t len, pos = 0;

	//subscribe pdv: {topic_size:2 topic_name:n}

	len = GET_SWORD_NET(pdv_data, pos);
	pos += 2;

	switch (hl7net->encode)
	{
	case _GB2312:
		hl7net->error_length = gb2312_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF8:
		hl7net->error_length = utf8_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF16_LIT:
		hl7net->error_length = utf16lit_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	case _UTF16_BIG:
		hl7net->error_length = utf16big_to_ucs(pdv_data + pos, len, NULL, MAX_LONG);
		break;
	}

	hl7net->error_text = xsrealloc(hl7net->error_text, hl7net->error_length + 1);

	switch (hl7net->encode)
	{
	case _GB2312:
		gb2312_to_ucs(pdv_data + pos, len, hl7net->error_text, hl7net->error_length);
		break;
	case _UTF8:
		utf8_to_ucs(pdv_data + pos, len, hl7net->error_text, hl7net->error_length);
		break;
	case _UTF16_LIT:
		utf16lit_to_ucs(pdv_data + pos, len, hl7net->error_text, hl7net->error_length);
		break;
	case _UTF16_BIG:
		utf16big_to_ucs(pdv_data + pos, len, hl7net->error_text, hl7net->error_length);
		break;
	}

	pos += len;

	return 1;
}

static bool_t _hl7net_write_pdu(hl7net_t* hl7net, byte_t pdu_type, dword_t pdu_size)
{
	dword_t n, total = 0;
	byte_t ctl[HL7NET_PDU_HDR_SIZE] = { 0 };

	stream_t stm = NULL;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)
	//PDU: {CONTROL | PDV | ...}

	TRY_CATCH;

	stm = stream_alloc(hl7net->bio);

	//CONTROL:{ encode:3 | pdu tpye:1 | pdu size:4 }

	//encode
	n = 3;
	PUT_ENCODE(ctl, total, hl7net->encode);
	total += n;

	//type
	n = 1;
	PUT_BYTE(ctl, total, pdu_type);
	total += n;

	//size
	n = 4;
	PUT_DWORD_NET(ctl, total, pdu_size);
	total += n;

	if (!stream_write_bytes(stm, ctl, total))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	if (stm)
		stream_free(stm);

	return 0;
}

static bool_t _hl7net_read_pdu(hl7net_t* hl7net, byte_t* pdu_type, dword_t* pdu_size)
{
	byte_t ctl[HL7NET_PDU_HDR_SIZE] = { 0 };
	dword_t n, pos = 0;

	stream_t stm = NULL;

	TRY_CATCH;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)
	//PDU: {CONTROL | PDV | ...}

	stm = stream_alloc(hl7net->bio);

	//CONTROL:{ encode:3 | pdu tpye:1 | pdu size:4 }
	n = HL7NET_PDU_HDR_SIZE;
	if (!stream_read_bytes(stm, ctl, &n))
	{
		raise_user_error(NULL, NULL);
	}
	
	//encode
	hl7net->encode = GET_ENCODE(ctl, pos);
	pos += 3;

	//pdu type
	*pdu_type = GET_BYTE(ctl, pos);
	pos += 1;

	//pdu_size
	*pdu_size = GET_DWORD_NET(ctl, pos);
	pos += 4;

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _hl7net_write_pdv(hl7net_t* hl7net, const hl7net_pdv_head_t* pdv)
{
	dword_t n, total = 0;
	byte_t hdr[HL7NET_PDV_HDR_SIZE] = { 0 };

	stream_t stm = NULL;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)
	//PDV: {type:4 | size:4 | dataset}

	TRY_CATCH;

	stm = stream_alloc(hl7net->bio);

	n = 4;
	xmem_copy((void*)(hdr + total), (void*)pdv->type, n);
	total += n;

	n = 4;
	PUT_DWORD_NET(hdr, total, pdv->size);
	total += n;

	if (!stream_write_bytes(stm, hdr, total))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _hl7net_read_pdv(hl7net_t* hl7net, hl7net_pdv_head_t* pdv)
{
	dword_t n, total = 0;
	byte_t hdr[HL7NET_PDV_HDR_SIZE] = { 0 };

	stream_t stm = NULL;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)
	//PDV: {type:4 | size:4 | dataset}

	TRY_CATCH;

	stm = stream_alloc(hl7net->bio);

	n = HL7NET_PDV_HDR_SIZE;
	if (!stream_read_bytes(stm, hdr, &n))
	{
		raise_user_error(NULL, NULL);
	}

	n = 4;
	xmem_copy((void*)pdv->type, (void*)(hdr + total), n);
	total += n;

	n = 4;
	pdv->size = GET_DWORD_NET(hdr, total);
	total += n;

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}
/************************************************************************************************************/

hl7net_t* hl7net_scu(xhand_t bio, int scu)
{
	hl7net_t* hl7net = NULL;

	XDL_ASSERT(scu == _HL7NET_TYPE_SCU_PUB || scu == _HL7NET_TYPE_SCU_SUB);

	if (!bio)
		return NULL;

	hl7net = (hl7net_t*)xmem_alloc(sizeof(hl7net_t));

	hl7net->type = scu;
	hl7net->bio = bio;

	hl7net->encode = DEF_MBS;
	hl7net->request_bytes = MAX_LONG;
	hl7net->response_bytes = MAX_LONG;

	hl7net->session_ver = HL7NET_VER;

	return (hl7net_t*)hl7net;
}

hl7net_t* hl7net_scp(xhand_t bio, int scp)
{
	hl7net_t* hl7net = NULL;

	XDL_ASSERT(scp == _HL7NET_TYPE_SCP_PUB || scp == _HL7NET_TYPE_SCP_SUB);

	if (!bio)
		return NULL;

	hl7net = (hl7net_t*)xmem_alloc(sizeof(hl7net_t));

	hl7net->type = scp;
	hl7net->bio = bio;

	hl7net->encode = DEF_MBS;
	hl7net->request_bytes = MAX_LONG;
	hl7net->response_bytes = MAX_LONG;

	hl7net->session_ver = HL7NET_VER;

	return (hl7net_t*)hl7net;
}

xhand_t hl7net_bio(hl7net_t* hl7net)
{
	return hl7net->bio;
}

int hl7net_status(hl7net_t* hl7net)
{
	return hl7net->status;
}

bool_t hl7net_recv(hl7net_t* hl7net, hl7net_pdv_head_t* pdv)
{
	byte_t pdu_type = 0;

	byte_t* buf = NULL;
	dword_t len;

	stream_t stm = NULL;

	XDL_ASSERT(hl7net != NULL);

	TRY_CATCH;

	stm = stream_alloc(hl7net->bio);

	if (hl7net->type == _HL7NET_TYPE_SCP_SUB)
	{
		switch (hl7net->status)
		{
		case _HL7NET_STATUS_ASSOCIATE:
			//read CONNECT PDU
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != HL7NET_PDU_CONNECT)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid connect pdu"));
			}
			if (!len)
			{
				raise_user_error(_T("hl7net_recv"), _T("connect must has session pdv"));
			}

			//continue read CONNECT session
			buf = (byte_t*)xmem_alloc(len);
			if (!stream_read_bytes(stm, buf, &len))
			{
				raise_user_error(NULL, NULL);
			}

			//initialize or restore session
			if (!_hl7net_parse_session(hl7net, buf, len))
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid session"));
			}

			xmem_free(buf);
			buf = NULL;
			break;
		case _HL7NET_STATUS_SLEEPING:
			//read SUBCRIBE PDU
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != HL7NET_PDU_SUBSCRIBE)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid message type"));
			}
			if (!len)
			{
				raise_user_error(_T("hl7net_recv"), _T("subcribe must has pdv"));
			}

			//continue read SUBCRIBE topic
			buf = (byte_t*)xmem_alloc(len);
			if (!stream_read_bytes(stm, buf, &len))
			{
				raise_user_error(NULL, NULL);
			}

			if (!_hl7net_parse_topic(hl7net, buf, len))
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid subscribe topic"));
			}

			xmem_free(buf);
			buf = NULL;
			break;
		case _HL7NET_STATUS_WAITING:
			raise_user_error(_T("hl7net_recv"), _T("invalid server status"));
			break;
		case _HL7NET_STATUS_PENDING:
			//read MESSAGE ACK
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != HL7NET_PDU_MSGACK)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid message type"));
			}
			if (len)
			{
				raise_user_error(_T("hl7net_recv"), _T("message ack has no pdv"));
			}

			//MESSAGE END
			hl7net->status = _HL7NET_STATUS_RELEASE;
			break;
		case _HL7NET_STATUS_RELEASE:
			raise_user_error(_T("hl7net_recv"), _T("server released"));
			break;
		}
	}
	else if (hl7net->type == _HL7NET_TYPE_SCP_PUB)
	{
		switch (hl7net->status)
		{
		case _HL7NET_STATUS_ASSOCIATE:
			//read CONNECT PDU
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != HL7NET_PDU_CONNECT)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid connect pdu"));
			}
			if (!len)
			{
				raise_user_error(_T("hl7net_recv"), _T("connect must has pdv"));
			}
			
			//continue read CONNECT
			buf = (byte_t*)xmem_alloc(len);
			if (!stream_read_bytes(stm, buf, &len))
			{
				raise_user_error(NULL, NULL);
			}

			//initialize or restore session
			if (!_hl7net_parse_session(hl7net, buf, len))
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid session"));
			}

			xmem_free(buf);
			buf = NULL;
			break;
		case _HL7NET_STATUS_SLEEPING:
			//read PUBLISH PDU
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != HL7NET_PDU_PUBLISH)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid message type"));
			}
			if (!len)
			{
				raise_user_error(_T("hl7net_recv"), _T("publish must has pdv"));
			}

			//continue read PUBLISH topic
			buf = (byte_t*)xmem_alloc(len);
			if (!stream_read_bytes(stm, buf, &len))
			{
				raise_user_error(NULL, NULL);
			}

			if (!_hl7net_parse_topic(hl7net, buf, len))
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid publish topic"));
			}

			xmem_free(buf);
			buf = NULL;
			break;
		case _HL7NET_STATUS_WAITING:
			//read MESSAGE PDU
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &hl7net->request_bytes))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type != HL7NET_PDU_MESSAGE)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid message type"));
			}

			//continue read MESSAGE PDV
			hl7net->status = _HL7NET_STATUS_PENDING;
		case _HL7NET_STATUS_PENDING:
			if (hl7net->request_bytes)
			{
				if (!_hl7net_read_pdv(hl7net, pdv))
				{
					raise_user_error(NULL, NULL);
				}

				hl7net->request_bytes -= pdv->size;
			}
			else
			{
				xmem_zero((void*)hl7net->type, 4);
				pdv->size = 0;
			}
			break;
		case _HL7NET_STATUS_RELEASE:
			raise_user_error(_T("hl7net_recv"), _T("server released"));
			break;
		}
	}
	else if (hl7net->type == _HL7NET_TYPE_SCU_SUB)
	{
		switch (hl7net->status)
		{
		case _HL7NET_STATUS_ASSOCIATE:
			//read CONNACK PDU
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == HL7NET_PDU_ERROR)
			{
				//then to read error
				buf = (byte_t*)xmem_alloc(len);
				if (!stream_read_bytes(stm, buf, &len))
				{
					raise_user_error(NULL, NULL);
				}

				_hl7net_parse_error(hl7net, buf, len);
				
				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			if (pdu_type != HL7NET_PDU_CONNACK)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid connack pdu"));
			}
			if (!len)
			{
				raise_user_error(_T("hl7net_recv"), _T("connack must has pdv"));
			}

			//then to read session
			buf = (byte_t*)xmem_alloc(len);
			if (!stream_read_bytes(stm, buf, &len))
			{
				raise_user_error(NULL, NULL);
			}

			if (!_hl7net_parse_session(hl7net, buf, len))
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid session"));
			}

			xmem_free(buf);
			buf = NULL;

			hl7net->status = _HL7NET_STATUS_SLEEPING;
			break;
		case _HL7NET_STATUS_SLEEPING:
			//read SUBSCRIBE ACK
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == HL7NET_PDU_ERROR)
			{
				//then to read error
				buf = (byte_t*)xmem_alloc(len);
				if (!stream_read_bytes(stm, buf, &len))
				{
					raise_user_error(NULL, NULL);
				}

				_hl7net_parse_error(hl7net, buf, len);

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			if (pdu_type != HL7NET_PDU_SUBACK)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid message type"));
			}
			if (len)
			{
				raise_user_error(_T("hl7net_recv"), _T("subscribe ack has no pdv"));
			}

			// then to read MESSAGE PDV
			hl7net->status = _HL7NET_STATUS_WAITING;
			break;
		case _HL7NET_STATUS_WAITING:
			//read MESSAGE PDU
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &hl7net->response_bytes))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == HL7NET_PDU_ERROR)
			{
				//then to read error
				buf = (byte_t*)xmem_alloc(len);
				if (!stream_read_bytes(stm, buf, &len))
				{
					raise_user_error(NULL, NULL);
				}

				_hl7net_parse_error(hl7net, buf, len);

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			if (pdu_type != HL7NET_PDU_MESSAGE)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid message type"));
			}
			
			// continue to read MESSAGE PDV
			hl7net->status = _HL7NET_STATUS_PENDING;
		case _HL7NET_STATUS_PENDING:
			if (hl7net->response_bytes)
			{
				if (!_hl7net_read_pdv(hl7net, pdv))
				{
					raise_user_error(NULL, NULL);
				}

				hl7net->response_bytes -= pdv->size;
			}
			else
			{
				xmem_zero((void*)hl7net->type, 4);
				pdv->size = 0;
			}
			break;
		case _HL7NET_STATUS_RELEASE:
			raise_user_error(_T("hl7net_recv"), _T("client released"));
			break;
		}
	}
	else if (hl7net->type == _HL7NET_TYPE_SCU_PUB)
	{
		switch (hl7net->status)
		{
		case _HL7NET_STATUS_ASSOCIATE:
			//read CONNACK PDU
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == HL7NET_PDU_ERROR)
			{
				//then to read error
				buf = (byte_t*)xmem_alloc(len);
				if (!stream_read_bytes(stm, buf, &len))
				{
					raise_user_error(NULL, NULL);
				}

				_hl7net_parse_error(hl7net, buf, len);

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			if (pdu_type != HL7NET_PDU_CONNACK)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid connack pdu"));
			}
			if (!len)
			{
				raise_user_error(_T("hl7net_recv"), _T("connack must has pdv"));
			}

			//then to read session
			buf = (byte_t*)xmem_alloc(len);
			if (!stream_read_bytes(stm, buf, &len))
			{
				raise_user_error(NULL, NULL);
			}

			if (!_hl7net_parse_session(hl7net, buf, len))
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid session"));
			}

			xmem_free(buf);
			buf = NULL;

			hl7net->status = _HL7NET_STATUS_SLEEPING;
			break;
		case _HL7NET_STATUS_SLEEPING:
			//read PUBLISH ACK
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == HL7NET_PDU_ERROR)
			{
				//then to read error
				buf = (byte_t*)xmem_alloc(len);
				if (!stream_read_bytes(stm, buf, &len))
				{
					raise_user_error(NULL, NULL);
				}

				_hl7net_parse_error(hl7net, buf, len);

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			if (pdu_type != HL7NET_PDU_PUBACK)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid message type"));
			}
			if (len)
			{
				raise_user_error(_T("hl7net_recv"), _T("publish ack has no pdv"));
			}

			// then to send MESSAGE PDV
			hl7net->status = _HL7NET_STATUS_WAITING;
			break;
		case _HL7NET_STATUS_WAITING:
			raise_user_error(_T("hl7net_recv"), _T("invalid client status"));
			break;
		case _HL7NET_STATUS_PENDING:
			//read MESSAGE ACK
			if (!_hl7net_read_pdu(hl7net, &pdu_type, &len))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == HL7NET_PDU_ERROR)
			{
				//then to read error
				buf = (byte_t*)xmem_alloc(len);
				if (!stream_read_bytes(stm, buf, &len))
				{
					raise_user_error(NULL, NULL);
				}

				_hl7net_parse_error(hl7net, buf, len);

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			if (pdu_type != HL7NET_PDU_MSGACK)
			{
				raise_user_error(_T("hl7net_recv"), _T("invalid message type"));
			}
			if (len)
			{
				raise_user_error(_T("hl7net_recv"), _T("MSGACK has no pdv"));
			}

			//MESSAGE END
			hl7net->status = _HL7NET_STATUS_RELEASE;
			break;
		case _HL7NET_STATUS_RELEASE:
			raise_user_error(_T("hl7net_recv"), _T("client released"));
			break;
		}
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	hl7net->status = _HL7NET_STATUS_RELEASE;

	return 0;
}

bool_t hl7net_send(hl7net_t* hl7net, const hl7net_pdv_head_t* pdv)
{
	stream_t stm = NULL;

	byte_t* buf = NULL;
	dword_t len;

	XDL_ASSERT(hl7net != NULL);

	TRY_CATCH;

	stm = stream_alloc(hl7net->bio);

	if (hl7net->type == _HL7NET_TYPE_SCP_SUB)
	{
		switch (hl7net->status)
		{
		case _HL7NET_STATUS_ASSOCIATE:
			if (hl7net->error_code)
			{
				//write ERROR PDU
				len = _hl7net_format_error(hl7net, NULL, MAX_LONG);

				if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_ERROR, len))
				{
					raise_user_error(NULL, NULL);
				}

				buf = (byte_t*)xmem_alloc(len);
				_hl7net_format_error(hl7net, buf, len);

				if (!stream_write_bytes(stm, buf, len))
				{
					raise_user_error(NULL, NULL);
				}

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			//write CONNACK PDU
			len = _hl7net_format_session(hl7net, NULL, MAX_LONG);

			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_CONNACK, len))
			{
				raise_user_error(NULL, NULL);
			}
			
			buf = (byte_t*)xmem_alloc(len);
			_hl7net_format_session(hl7net, buf, len);
			
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}

			xmem_free(buf);
			buf = NULL;

			hl7net->status = _HL7NET_STATUS_SLEEPING;
			break;
		case _HL7NET_STATUS_SLEEPING:
			if (hl7net->error_code)
			{
				//write ERROR PDU
				len = _hl7net_format_error(hl7net, NULL, MAX_LONG);

				if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_ERROR, len))
				{
					raise_user_error(NULL, NULL);
				}

				buf = (byte_t*)xmem_alloc(len);
				_hl7net_format_error(hl7net, buf, len);

				if (!stream_write_bytes(stm, buf, len))
				{
					raise_user_error(NULL, NULL);
				}

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			//write SUBACK PDU
			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_SUBACK, 0))
			{
				raise_user_error(NULL, NULL);
			}
			hl7net->status = _HL7NET_STATUS_WAITING;
			break;
		case _HL7NET_STATUS_WAITING:
			if (hl7net->error_code)
			{
				//write ERROR PDU
				len = _hl7net_format_error(hl7net, NULL, MAX_LONG);

				if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_ERROR, len))
				{
					raise_user_error(NULL, NULL);
				}

				buf = (byte_t*)xmem_alloc(len);
				_hl7net_format_error(hl7net, buf, len);

				if (!stream_write_bytes(stm, buf, len))
				{
					raise_user_error(NULL, NULL);
				}

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			//write MESSAGE PDU
			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_MESSAGE, hl7net->response_bytes))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to send MESSAGE pdv
			hl7net->status = _HL7NET_STATUS_PENDING;
		case _HL7NET_STATUS_PENDING:
			if (hl7net->response_bytes)
			{
				if (!_hl7net_write_pdv(hl7net, pdv))
				{
					raise_user_error(NULL, NULL);
				}

				hl7net->response_bytes -= pdv->size;
			}
			else
			{
				raise_user_error(_T("hl7net_send"), _T("invalid response size"));
			}
			break;
		case _HL7NET_STATUS_RELEASE:
			raise_user_error(_T("hl7net_send"), _T("server released"));
			break;
		}
	}
	else if (hl7net->type == _HL7NET_TYPE_SCP_PUB)
	{
		switch (hl7net->status)
		{
		case _HL7NET_STATUS_ASSOCIATE:
			if (hl7net->error_code)
			{
				//write ERROR PDU
				len = _hl7net_format_error(hl7net, NULL, MAX_LONG);

				if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_ERROR, len))
				{
					raise_user_error(NULL, NULL);
				}

				buf = (byte_t*)xmem_alloc(len);
				_hl7net_format_error(hl7net, buf, len);

				if (!stream_write_bytes(stm, buf, len))
				{
					raise_user_error(NULL, NULL);
				}

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			//write CONNACK PDU
			len = _hl7net_format_session(hl7net, NULL, MAX_LONG);
			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_CONNACK, len))
			{
				raise_user_error(NULL, NULL);
			}

			buf = (byte_t*)xmem_alloc(len);
			_hl7net_format_session(hl7net, buf, len);

			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}

			xmem_free(buf);
			buf = NULL;

			hl7net->status = _HL7NET_STATUS_SLEEPING;
			break;
		case _HL7NET_STATUS_SLEEPING:
			if (hl7net->error_code)
			{
				//write ERROR PDU
				len = _hl7net_format_error(hl7net, NULL, MAX_LONG);

				if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_ERROR, len))
				{
					raise_user_error(NULL, NULL);
				}

				buf = (byte_t*)xmem_alloc(len);
				_hl7net_format_error(hl7net, buf, len);

				if (!stream_write_bytes(stm, buf, len))
				{
					raise_user_error(NULL, NULL);
				}

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			//write PUBACK PDU
			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_PUBACK, 0))
			{
				raise_user_error(NULL, NULL);
			}
			hl7net->status = _HL7NET_STATUS_WAITING;
			break;
		case _HL7NET_STATUS_WAITING:
			raise_user_error(_T("hl7net_send"), _T("invalid server status"));
			break;
		case _HL7NET_STATUS_PENDING:
			if (hl7net->error_code)
			{
				//write ERROR PDU
				len = _hl7net_format_error(hl7net, NULL, MAX_LONG);

				if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_ERROR, len))
				{
					raise_user_error(NULL, NULL);
				}

				buf = (byte_t*)xmem_alloc(len);
				_hl7net_format_error(hl7net, buf, len);

				if (!stream_write_bytes(stm, buf, len))
				{
					raise_user_error(NULL, NULL);
				}

				xmem_free(buf);
				buf = NULL;

				hl7net->status = _HL7NET_STATUS_RELEASE;
				break;
			}

			//write MESSAGE ACK
			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_MSGACK, 0))
			{
				raise_user_error(NULL, NULL);
			}
			//MESSAGE END 
			hl7net->status = _HL7NET_STATUS_RELEASE;
			break;
		case _HL7NET_STATUS_RELEASE:
			raise_user_error(_T("hl7net_send"), _T("server released"));
			break;
		}
	}
	else if (hl7net->type == _HL7NET_TYPE_SCU_SUB)
	{
		switch (hl7net->status)
		{
		case _HL7NET_STATUS_ASSOCIATE:
			//write CONNECT PDU
			len = _hl7net_format_session(hl7net, NULL, MAX_LONG);

			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_CONNECT, len))
			{
				raise_user_error(NULL, NULL);
			}
			
			buf = (byte_t*)xmem_alloc(len);
			_hl7net_format_session(hl7net, buf, len);

			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}

			xmem_free(buf);
			buf = NULL;

			break;
		case _HL7NET_STATUS_SLEEPING:
			//write SUBSCRIBE PDU
			len = _hl7net_format_topic(hl7net, NULL, MAX_LONG);

			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_SUBSCRIBE, len))
			{
				raise_user_error(NULL, NULL);
			}
			
			buf = (byte_t*)xmem_alloc(len);
			_hl7net_format_topic(hl7net, buf, len);

			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}

			xmem_free(buf);
			buf = NULL;
			break;
		case _HL7NET_STATUS_WAITING:
			raise_user_error(_T("hl7net_send"), _T("invalid client status"));
			break;
		case _HL7NET_STATUS_PENDING:
			//write MESSAGE ACK
			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_MSGACK, 0))
			{
				raise_user_error(NULL, NULL);
			}

			//MESSAGE END
			hl7net->status = _HL7NET_STATUS_RELEASE;
			break;
		case _HL7NET_STATUS_RELEASE:
			raise_user_error(_T("hl7net_send"), _T("client released"));
			break;
		}
	}
	else if (hl7net->type == _HL7NET_TYPE_SCU_PUB)
	{
		switch (hl7net->status)
		{
		case _HL7NET_STATUS_ASSOCIATE:
			//write CONNECT PDU
			len = _hl7net_format_session(hl7net, NULL, MAX_LONG);

			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_CONNECT, len))
			{
				raise_user_error(NULL, NULL);
			}
			
			buf = (byte_t*)xmem_alloc(len);
			_hl7net_format_session(hl7net, buf, len);

			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}

			xmem_free(buf);
			buf = NULL;
			break;
		case _HL7NET_STATUS_SLEEPING:
			//write PUBLISH PDU
			len = _hl7net_format_topic(hl7net, NULL, MAX_LONG);

			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_PUBLISH, len))
			{
				raise_user_error(NULL, NULL);
			}

			buf = (byte_t*)xmem_alloc(len);
			_hl7net_format_topic(hl7net, buf, len);

			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}

			xmem_free(buf);
			buf = NULL;
			break;
		case _HL7NET_STATUS_WAITING:
			//write MESSAGE PDU
			if (!_hl7net_write_pdu(hl7net, HL7NET_PDU_MESSAGE, hl7net->request_bytes))
			{
				raise_user_error(NULL, NULL);
			}
			//continue to send pdv
			hl7net->status = _HL7NET_STATUS_PENDING;
		case _HL7NET_STATUS_PENDING:
			if (hl7net->request_bytes)
			{
				if (!_hl7net_write_pdv(hl7net, pdv))
				{
					raise_user_error(NULL, NULL);
				}

				hl7net->request_bytes -= pdv->size;
			}
			else
			{
				raise_user_error(_T("hl7net_send"), _T("invalid request size"));
			}
			break;
		case _HL7NET_STATUS_RELEASE:
			raise_user_error(_T("hl7net_send"), _T("client released"));
			break;
		}
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	hl7net->status = _HL7NET_STATUS_RELEASE;

	return 0;
}

bool_t hl7net_accept(hl7net_t* hl7net)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCP_SUB || hl7net->type == _HL7NET_TYPE_SCP_PUB));

	//read CONNECT PDU
	if (!hl7net_recv(hl7net, NULL))
	{
		return 0;
	}

	//write CONNECT ACK	
	if (!hl7net_send(hl7net, NULL))
	{
		return 0;
	}

	return 1;
}

bool_t hl7net_connect(hl7net_t* hl7net)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCU_SUB || hl7net->type == _HL7NET_TYPE_SCU_PUB));

	//write CONNECT PDU
	if (!hl7net_send(hl7net, NULL))
	{
		return 0;
	}

	//read CONNECT ACK
	if (!hl7net_recv(hl7net, NULL))
	{
		return 0;
	}

	return 1;
}

bool_t hl7net_subscribe(hl7net_t* hl7net, const tchar_t* topic, int len)
{
	XDL_ASSERT(hl7net != NULL && hl7net->type == _HL7NET_TYPE_SCU_SUB);

	hl7net->topic_length = (len < 0) ? xslen(topic) : len;
	hl7net->topic_name = xsrealloc(hl7net->topic_name, hl7net->topic_length + 1);
	xscpy(hl7net->topic_name, topic);
	
	//write SUBSCRIBE PDU
	if (!hl7net_send(hl7net, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	//read SUBSCRIBE ACK
	if (!hl7net_recv(hl7net, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	return 1;
}

bool_t hl7net_poll_subscribe(hl7net_t* hl7net, tchar_t* topic, int len)
{
	XDL_ASSERT(hl7net != NULL && hl7net->type == _HL7NET_TYPE_SCP_SUB);

	//read SUBSCRIBE PDU
	if (!hl7net_recv(hl7net, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	//write SUBSCRIBE ACK
	if (!hl7net_send(hl7net, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	xsncpy(topic, hl7net->topic_name, len);

	return 1;
}

bool_t hl7net_publish(hl7net_t* hl7net, const tchar_t* topic, int len)
{
	XDL_ASSERT(hl7net != NULL && hl7net->type == _HL7NET_TYPE_SCU_PUB);

	hl7net->topic_length = (len < 0) ? xslen(topic) : len;
	hl7net->topic_name = xsrealloc(hl7net->topic_name, hl7net->topic_length + 1);
	xscpy(hl7net->topic_name, topic);

	//write PUBLISH PDU
	if (!hl7net_send(hl7net, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	//read PUBLISH ACK
	if (!hl7net_recv(hl7net, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	return 1;
}

bool_t hl7net_poll_publish(hl7net_t* hl7net, tchar_t* topic, int len)
{
	XDL_ASSERT(hl7net != NULL && hl7net->type == _HL7NET_TYPE_SCP_PUB);

	//read PUBLISH PDU
	if (!hl7net_recv(hl7net, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	//write PUBLISH ACK
	if (!hl7net_send(hl7net, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	xsncpy(topic, hl7net->topic_name, len);

	return 1;
}

bool_t hl7net_poll_message(hl7net_t* hl7net, hl7net_pdv_head_t* pdv)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCU_SUB || hl7net->type == _HL7NET_TYPE_SCP_PUB));

	//read MESSAGE PDU
	if (!hl7net_recv(hl7net, pdv))
	{
		return 0;
	}

	if ((!hl7net->response_bytes && hl7net->type == _HL7NET_TYPE_SCU_SUB) || (!hl7net->request_bytes && hl7net->type == _HL7NET_TYPE_SCP_PUB))
	{
		//write MESSAGE ACK
		if (!hl7net_send(hl7net, 0))
		{
			return 0;
		}
	}

	return 1;
}

bool_t hl7net_push_message(hl7net_t* hl7net, const hl7net_pdv_head_t* pdv)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCU_PUB || hl7net->type == _HL7NET_TYPE_SCP_SUB));

	//write MESSAGE PDU
	if (!hl7net_send(hl7net, pdv))
	{
		return 0;
	}

	if ((!hl7net->request_bytes && hl7net->type == _HL7NET_TYPE_SCU_PUB) || (!hl7net->response_bytes && hl7net->type == _HL7NET_TYPE_SCP_SUB))
	{
		//read MESSAGE ACK
		if (!hl7net_recv(hl7net, NULL))
		{
			return 0;
		}
	}

	return 1;
}

void hl7net_abort(hl7net_t* hl7net, int errcode)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCP_PUB || hl7net->type == _HL7NET_TYPE_SCP_SUB));

	hl7net->error_code = errcode;

	xsfree(hl7net->error_text);
	hl7net->error_length = 0;

	switch (hl7net->error_code)
	{
	case HL7NET_ERROR_SUCCEED:
		hl7net->error_text = xsclone(HL7NET_ERROR_SUCCEED_TEXT);
		break;
	case HL7NET_ERROR_INVALIDUSER:
		hl7net->error_text = xsclone(HL7NET_ERROR_INVALIDUSER_TEXT);
		break;
	case HL7NET_ERROR_INVALIDTOPIC:
		hl7net->error_text = xsclone(HL7NET_ERROR_INVALIDTOPIC_TEXT);
		break;
	}

	hl7net->error_length = xslen(hl7net->error_text);

	hl7net_send(hl7net, NULL);
}

void hl7net_close(hl7net_t* hl7net)
{
	_hl7net_clean_session(hl7net);

	xmem_free(hl7net);
}

void hl7net_set_response_bytes(hl7net_t* hl7net, dword_t size)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCP_PUB || hl7net->type == _HL7NET_TYPE_SCP_SUB));

	hl7net->response_bytes = size;
}

dword_t hl7net_get_response_bytes(hl7net_t* hl7net)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCU_PUB || hl7net->type == _HL7NET_TYPE_SCU_SUB));

	return hl7net->response_bytes;
}

dword_t hl7net_get_request_bytes(hl7net_t* hl7net)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCP_PUB || hl7net->type == _HL7NET_TYPE_SCP_SUB));

	return hl7net->request_bytes;
}

void hl7net_set_request_bytes(hl7net_t* hl7net, dword_t size)
{
	XDL_ASSERT(hl7net != NULL && (hl7net->type == _HL7NET_TYPE_SCU_PUB || hl7net->type == _HL7NET_TYPE_SCU_SUB));

	hl7net->request_bytes = size;
}

dword_t hl7net_pdv_group_size(hl7net_pdv_head_t* pdv, int n)
{
	dword_t total = 0;
	int i;

	for (i = 0; i < n; i++)
	{
		total += (HL7NET_PDV_HDR_SIZE + pdv->size);
	}

	return total;
}