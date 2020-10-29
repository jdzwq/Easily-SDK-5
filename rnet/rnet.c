/***********************************************************************
	Easily RadNet 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc rnet document

	@module	rnet.c | rad network document implement file

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

#include "rnet.h"

static dword_t _rnet_format_text(rnet_t* rnet, const tchar_t* token, int len, byte_t* buf, dword_t max)
{
	dword_t n;

	switch (rnet->encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = ucs_to_gb2312(token, len, buf, max);
#else
		n = mbs_to_gb2312(token, len, buf, max);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = ucs_to_utf8(token, len, buf, max);
#else
		n = mbs_to_utf8(token, len, buf, max);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = ucs_to_utf16lit(token, len, buf, max);
#else
		n = mbs_to_utf16lit(token, len, buf, max);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = ucs_to_utf16big(token, len, buf, max);
#else
		n = mbs_to_utf16big(token, len, buf, max);
#endif
		break;
	}

	return n;
}

static int _rnet_parse_text(rnet_t* rnet, const byte_t* buf, dword_t len, tchar_t* token, int max)
{
	int n;

	switch (rnet->encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = gb2312_to_ucs(buf, len, token, max);
#else
		n = gb2312_to_mbs(buf, len, token, max);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = utf8_to_ucs(buf, len, token, max);
#else
		n = utf8_to_mbs(buf, len,  token, max);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = utf16lit_to_ucs(buf, len, token, max);
#else
		n = utf16lit_to_mbs(buf, len,  token, max);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = utf16big_to_ucs(buf, len, token, max);
#else
		n = utf16big_to_mbs(buf, len,  token, max);
#endif
		break;
	}

	return n;
}

static bool_t _rnet_write_pdu(rnet_t* rnet, byte_t pdu_type, byte_t pdv_type, dword_t pdv_size)
{
	byte_t hdr[INT_LEN + 1] = { 0 };
	byte_t* buf = NULL;
	dword_t len;

	stream_t stm = NULL;

	dword_t total = 0;
	dword_t pdu_size, n;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)
	//PDU: {HDR {PDV}}

	TRY_CATCH;

	stm = stream_alloc(rnet->pbo);

	//HDR: {ENCODE:3 PDUTYPE:1 PDUSIZE:4}
	n = 3;
	PUT_ENCODE(hdr, total, rnet->encode);
	total += n;

	n = 1;
	PUT_BYTE(hdr, total, pdu_type);
	total += n;

	//{PDV}
	switch (pdu_type)
	{
	case RNET_PDU_CONNECT:
		len = _rnet_format_text(rnet, rnet->dbname, -1, NULL, MAX_LONG);
		pdu_size = 2 + len;
		n = 4;
		PUT_DWORD_NET(hdr, total, pdu_size);
		total += n;

		if (!stream_write_bytes(stm, hdr, total))
		{
			raise_user_error(NULL, NULL);
		}

		//CONNECT-PDV: {DBNAME-LENGTH:2 DBNAME:n}
		total = 0;
		buf = (byte_t*)xmem_alloc(2 + len);

		n = 2;
		PUT_SWORD_NET(buf, total, len);
		total += n;

		n = _rnet_format_text(rnet, rnet->dbname, -1, buf + total, len);
		total += n;

		if (!stream_write_bytes(stm, buf, total))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	case RNET_PDU_DISCONN:
		pdu_size = 0;
		n = 4;
		PUT_DWORD_NET(hdr, total, pdu_size);
		total += n;
		//DISCONN-PDV: {}
		if (!stream_write_bytes(stm, hdr, total))
		{
			raise_user_error(NULL, NULL);
		}
		break;
	case RNET_PDU_COMMAND:
		pdu_size = RNET_COMMAND_HDR_SIZE + pdv_size;
		n = 4;
		PUT_DWORD_NET(hdr, total, pdu_size);
		total += n;

		//COMMAND-PDV: {COMMAND-TYPE:1 RESERVE:3 GROUP-SIZE:4}
		n = 1;
		PUT_BYTE(hdr, total, pdv_type);
		total += n;

		n = 3;
		PUT_BYTE(hdr, total, 0);
		PUT_BYTE(hdr, total + 1, 0);
		PUT_BYTE(hdr, total + 2, 0);
		total += n;

		n = 4;
		PUT_DWORD_NET(hdr, total, pdv_size);
		total += n;

		if (!stream_write_bytes(stm, hdr, total))
		{
			raise_user_error(NULL, NULL);
		}

		break;
	case RNET_PDU_DATASET:
		pdu_size = RNET_DATASET_HDR_SIZE + pdv_size;
		n = 4;
		PUT_DWORD_NET(hdr, total, pdu_size);
		total += n;

		//DATA-PDV: {DATASET-TYPE:1 RESERVE:3 GROUP-SIZE:4}
		n = 1;
		PUT_BYTE(hdr, total, 0);
		total += n;

		n = 3;
		PUT_BYTE(hdr, total, 0);
		PUT_BYTE(hdr, total + 1, 0);
		PUT_BYTE(hdr, total + 2, 0);
		total += n;

		n = 4;
		PUT_DWORD_NET(hdr, total, pdv_size);
		total += n;

		if (!stream_write_bytes(stm, hdr, total))
		{
			raise_user_error(NULL, NULL);
		}

		break;
	case RNET_PDU_ERR:
		len = _rnet_format_text(rnet, rnet->errtext, -1, NULL, MAX_LONG);
		pdu_size = 4 + 4 + len;
		n = 4;
		PUT_DWORD_NET(hdr, total, pdu_size);
		total += n;

		if (!stream_write_bytes(stm, hdr, total))
		{
			raise_user_error(NULL, NULL);
		}

		//ERR-PDV: {ERRCODE:4 ERRTEXT-LENGTH:4 ERRTEXT:n}
		total = 0;
		buf = (byte_t*)xmem_alloc(4 + 4 + len);

		n = 4;
		PUT_DWORD_NET(buf, total, rnet->errcode);
		total += n;

		n = 4;
		PUT_DWORD_NET(buf, total, len);
		total += n;

		n = _rnet_format_text(rnet, rnet->errtext, -1, buf + total, len);
		total += n;

		if (!stream_write_bytes(stm, buf, total))
		{
			raise_user_error(NULL, NULL);
		}

		xmem_free(buf);
		buf = NULL;
		break;
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _rnet_read_pdu(rnet_t* rnet, byte_t* pdu_type, byte_t* pdv_type, dword_t* pdv_size)
{
	byte_t hdr[INT_LEN + 1] = { 0 };
	byte_t* buf = NULL;
	dword_t len;
	dword_t pdu_size, n;

	stream_t stm = NULL;

	//PDU USE NETWORK BYTES ORDERS (BigEndian)
	//PDU: {HDR {PDV}}

	TRY_CATCH;

	stm = stream_alloc(rnet->pbo);

	//HDR: {ENCODE:3 PDUTYPE:1 PDUSIZE:4}
	*pdv_type = 0;
	*pdv_size = 0;

	n = RNET_HDR_SIZE;
	if (!stream_read_bytes(stm, hdr, &n))
	{
		raise_user_error(NULL, NULL);
	}
	
	//pdu encode
	rnet->encode = GET_ENCODE(hdr, 0);
	//pdu type
	*pdu_type = GET_BYTE(hdr, 3);
	//pdu size
	pdu_size = GET_DWORD_NET(hdr, 4);

	//{PDV}
	switch (*pdu_type)
	{
	case RNET_PDU_CONNECT:
		//CONNECT-PDV: {DBNAME-LENGTH:2 DBNAME:n}
		buf = (byte_t*)xmem_alloc(pdu_size);
		n = pdu_size;
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}

		len = GET_SWORD_NET(buf, 0);
		_rnet_parse_text(rnet, buf + 2, len, rnet->dbname, PATH_LEN);

		xmem_free(buf);
		buf = NULL;
		break;
	case RNET_PDU_DISCONN:
		//DISCONN-PDV: {}
		break;
	case RNET_PDU_COMMAND:
		//COMMAND-PDV: {COMMAND-TYPE:1 RESERVE:3 GROUP-SIZE:4}
		n = RNET_COMMAND_HDR_SIZE;
		if (!stream_read_bytes(stm, hdr, &n))
		{
			raise_user_error(NULL, NULL);
		}

		*pdv_type = GET_BYTE(hdr, 0);
		*pdv_size = GET_DWORD_NET(hdr, 4);
		break;
	case RNET_PDU_DATASET:
		//DATA-PDV: {DATASET-TYPE:1 RESERVE:3 GROUP-SIZE:4}
		n = RNET_DATASET_HDR_SIZE;
		if (!stream_read_bytes(stm, hdr, &n))
		{
			raise_user_error(NULL, NULL);
		}

		*pdv_type = GET_BYTE(hdr, 0);
		*pdv_size = GET_DWORD_NET(hdr, 4);
		break;
	case RNET_PDU_ERR:
		//ERR-PDV: {ERRCODE:4 ERRTEXT-LENGTH:4 ERRTEXT:n}
		if (!pdu_size)
			break;
		buf = (byte_t*)xmem_alloc(pdu_size);
		n = pdu_size;
		if (!stream_read_bytes(stm, buf, &n))
		{
			raise_user_error(NULL, NULL);
		}

		rnet->errcode = GET_DWORD_NET(buf, 0);

		len = GET_DWORD_NET(buf, 4);
		_rnet_parse_text(rnet, buf + 8, len, rnet->errtext, ERR_LEN);

		xmem_free(buf);
		buf = NULL;
		break;
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

/****************************************************************************************************************/

rnet_t* rnet_scu(xhand_t bio)
{
	rnet_t* rnet;

	if (!bio)
		return NULL;

	rnet = (rnet_t*)xmem_alloc(sizeof(rnet_t));
	rnet->type = _RNET_TYPE_SCU;

	rnet->pbo = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));
	get_bio_interface(bio, rnet->pbo);

	rnet->encode = DEF_MBS;
	rnet->status = _RNET_STATUS_ASSOCIATE;

	return rnet;
}

rnet_t* rnet_scp(xhand_t bio)
{
	rnet_t* rnet;

	if (!bio)
		return NULL;

	rnet = (rnet_t*)xmem_alloc(sizeof(rnet_t));
	rnet->type = _RNET_TYPE_SCP;

	rnet->pbo = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));
	get_bio_interface(bio, rnet->pbo);

	rnet->encode = DEF_MBS;
	rnet->status = _RNET_STATUS_ASSOCIATE;

	return rnet;
}

bool_t rnet_connect(rnet_t* rnet, const tchar_t* dbname)
{
	byte_t pdu_type, pdv_type;
	dword_t pdv_size;

	XDL_ASSERT(rnet && rnet->type == _RNET_TYPE_SCU);

	TRY_CATCH;

	xsncpy(rnet->dbname, dbname, PATH_LEN);

	if (!_rnet_write_pdu(rnet, RNET_PDU_CONNECT, 0, 0))
	{
		raise_user_error(NULL, NULL);
	}

	if (!_rnet_read_pdu(rnet, &pdu_type, &pdv_type, &pdv_size))
	{
		raise_user_error(NULL, NULL);
	}
	if (pdu_type == RNET_PDU_ERR && rnet->errcode)
	{
		raise_user_error(_T("rnet_connect"), rnet->errtext);
	}

	rnet->status = _RNET_STATUS_WAITING;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	rnet->status = _RNET_STATUS_RELEASE;

	return 0;
}

bool_t rnet_accept(rnet_t* rnet)
{
	byte_t pdu_type, pdv_type;
	dword_t pdv_size;

	XDL_ASSERT(rnet && rnet->type == _RNET_TYPE_SCP);

	TRY_CATCH;

	if (!_rnet_read_pdu(rnet, &pdu_type, &pdv_type, &pdv_size))
	{
		raise_user_error(NULL, NULL);
	}
	if (pdu_type != RNET_PDU_CONNECT)
	{
		raise_user_error(_T("rnet_accept"), _T("invalid operator"));
	}

	rnet->errcode = 0;
	xscpy(rnet->errtext, _T("connect accepted"));

	if (!_rnet_write_pdu(rnet, RNET_PDU_ERR, 0, 0))
	{
		raise_user_error(NULL, NULL);
	}

	rnet->status = _RNET_STATUS_WAITING;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	rnet->status = _RNET_STATUS_RELEASE;

	return 0;
}

void rnet_abort(rnet_t* rnet, int errcode)
{
	XDL_ASSERT(rnet != NULL);

	switch (errcode)
	{
	case RNET_ERROR_SUCCEED:
		xsncpy(rnet->errtext, RNET_ERROR_SUCCEED_TEXT, ERR_LEN);
		break;
	case RNET_ERROR_OPEN:
		xsncpy(rnet->errtext, RNET_ERROR_OPEN_TEXT, ERR_LEN);
		break;
	case RNET_ERROR_INSERT:
		xsncpy(rnet->errtext, RNET_ERROR_INSERT_TEXT, ERR_LEN);
		break;
	case RNET_ERROR_UPDATE:
		xsncpy(rnet->errtext, RNET_ERROR_UPDATE_TEXT, ERR_LEN);
		break;
	case RNET_ERROR_TRUNCA:
		xsncpy(rnet->errtext, RNET_ERROR_TRUNCA_TEXT, ERR_LEN);
		break;
	case RNET_ERROR_SELECT:
		xsncpy(rnet->errtext, RNET_ERROR_SELECT_TEXT, ERR_LEN);
		break;
	default:
		xsncpy(rnet->errtext, _T("unknown error"), ERR_LEN);
		break;
	}

	rnet->errcode = errcode;

	if (rnet->type == _RNET_TYPE_SCP && rnet->status == _RNET_STATUS_WAITING)
	{
		_rnet_write_pdu(rnet, RNET_PDU_ERR, 0, 0);
	}
}

void rnet_close(rnet_t* rnet)
{
	XDL_ASSERT(rnet != NULL);

	if (rnet->type == _RNET_TYPE_SCU && rnet->status == _RNET_STATUS_WAITING)
	{
		_rnet_write_pdu(rnet, RNET_PDU_DISCONN, 0, 0);
	}

	if (rnet->pbo)
		xmem_free(rnet->pbo);

	xmem_free(rnet);
}

bool_t rnet_recv(rnet_t* rnet, rnet_pdv_head_t* pdv)
{
	byte_t pdu_type = 0;

	XDL_ASSERT(rnet != NULL);

	TRY_CATCH;

	if (rnet->type == _RNET_TYPE_SCP)
	{
		switch (rnet->status)
		{
		case _RNET_STATUS_ASSOCIATE:
			raise_user_error(_T("rnet_read"), _T("client not connected"));
			break;
		case _RNET_STATUS_WAITING:
			if (!_rnet_read_pdu(rnet, &pdu_type, &pdv->type, &pdv->size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == RNET_PDU_DISCONN)
			{
				rnet->status = _RNET_STATUS_RELEASE;
				break;
			}
			if (pdu_type != RNET_PDU_COMMAND)
			{
				raise_user_error(_T("rnet_read"), _T("invalid operator"));
			}
			break;
		case _RNET_STATUS_RELEASE:
			raise_user_error(_T("rnet_read"), _T("rnet released"));
			break;
		}
	}
	else if (rnet->type == _RNET_TYPE_SCU)
	{
		switch (rnet->status)
		{
		case _RNET_STATUS_ASSOCIATE:
			raise_user_error(_T("rnet_read"), _T("server not connected"));
			break;
		case _RNET_STATUS_WAITING:
			if (!_rnet_read_pdu(rnet, &pdu_type, &pdv->type, &pdv->size))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == RNET_PDU_ERR)
			{
				if (rnet->errcode)
				{
					raise_user_error(_T("rnet_read"), rnet->errtext);
				}
				else
				{
					break;
				}
			}
			if (pdu_type != RNET_PDU_DATASET)
			{
				raise_user_error(_T("rnet_read"), _T("invalid operator"));
			}
			break;
		case _RNET_STATUS_RELEASE:
			raise_user_error(_T("rnet_read"), _T("rnet released"));
			break;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	rnet->status = _RNET_STATUS_RELEASE;

	return 0;
}

bool_t rnet_send(rnet_t* rnet, const rnet_pdv_head_t* pdv)
{
	byte_t pdu_type = 0;

	XDL_ASSERT(rnet != NULL);

	TRY_CATCH;

	if (rnet->type == _RNET_TYPE_SCP)
	{
		switch (rnet->status)
		{
		case _RNET_STATUS_ASSOCIATE:
			raise_user_error(_T("rnet_write"), _T("client not connected"));
			break;
		case _RNET_STATUS_WAITING:
			if (!_rnet_write_pdu(rnet, RNET_PDU_DATASET, pdv->type, pdv->size))
			{
				raise_user_error(NULL, NULL);
			}
			break;
		case _RNET_STATUS_RELEASE:
			raise_user_error(_T("rnet_write"), _T("rnet released"));
			break;
		}
	}
	else if (rnet->type == _RNET_TYPE_SCU)
	{
		switch (rnet->status)
		{
		case _RNET_STATUS_ASSOCIATE:
			raise_user_error(_T("rnet_write"), _T("server not connected"));
			break;
		case _RNET_STATUS_WAITING:
			if (!_rnet_write_pdu(rnet, RNET_PDU_COMMAND, pdv->type, pdv->size))
			{
				raise_user_error(NULL, NULL);
			}
			break;
		case _RNET_STATUS_RELEASE:
			raise_user_error(_T("rnet_write"), _T("rnet released"));
			break;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	rnet->status = _RNET_STATUS_RELEASE;

	return 0;
}

int rnet_status(rnet_t* rnet)
{
	XDL_ASSERT(rnet != NULL);

	return rnet->status;
}
/*******************************************************************************************/

bool_t rnet_set(rnet_t* rnet, variant_t key, object_t val)
{
	rnet_pdv_head_t pdv = { 0 };
	dword_t n, total = 0;
	byte_t* buf = NULL;

	stream_t stm = NULL;

	XDL_ASSERT(rnet && rnet->type == _RNET_TYPE_SCU);

	TRY_CATCH;

	pdv.type = RNET_COMMAND_INSERT;
	pdv.size = variant_encode(&key, NULL, MAX_LONG) + object_encode(val, NULL, MAX_LONG);

	if (!rnet_send(rnet, &pdv))
	{
		raise_user_error(NULL, NULL);
	}

	buf = (byte_t*)xmem_alloc(pdv.size);

	n = variant_encode(&key, buf + total, MAX_LONG);
	total += n;

	n = object_encode(val, buf + total, MAX_LONG);
	total += n;

	stm = stream_alloc(rnet->pbo);

	if (!stream_write_bytes(stm, buf, total))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(buf);
	buf = NULL;

	if (!rnet_recv(rnet, &pdv))
	{
		raise_user_error(NULL, NULL);
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

	return 0;
}

bool_t rnet_get(rnet_t* rnet, variant_t key, object_t val)
{
	rnet_pdv_head_t pdv = { 0 };
	dword_t n, total = 0;
	byte_t* buf = NULL;

	stream_t stm = NULL;

	XDL_ASSERT(rnet && rnet->type == _RNET_TYPE_SCU);

	TRY_CATCH;

	pdv.type = RNET_COMMAND_SELECT;
	pdv.size = variant_encode(&key, NULL, MAX_LONG);

	if (!rnet_send(rnet, &pdv))
	{
		raise_user_error(NULL, NULL);
	}

	buf = (byte_t*)xmem_alloc(pdv.size);

	n = variant_encode(&key, buf + total, MAX_LONG);
	total += n;

	stm = stream_alloc(rnet->pbo);

	if (!stream_write_bytes(stm, buf, total))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(buf);
	buf = NULL;

	if (!rnet_recv(rnet, &pdv))
	{
		raise_user_error(NULL, NULL);
	}

	buf = (byte_t*)xmem_alloc(pdv.size);
	if (!stream_read_bytes(stm, buf, &pdv.size))
	{
		raise_user_error(NULL, NULL);
	}

	n = variant_decode(NULL, buf);
	if (n > pdv.size)
	{
		raise_user_error(_T("rnet_read"), _T("invalid dataset"));
	}

	n = object_decode(val, buf + n);

	xmem_free(buf);
	buf = NULL;

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

	return 0;
}

bool_t rnet_del(rnet_t* rnet, variant_t key)
{
	rnet_pdv_head_t pdv = { 0 };
	dword_t n, total = 0;
	byte_t* buf = NULL;

	stream_t stm = NULL;

	XDL_ASSERT(rnet && rnet->type == _RNET_TYPE_SCU);

	TRY_CATCH;

	pdv.type = RNET_COMMAND_DELETE;
	pdv.size = variant_encode(&key, NULL, MAX_LONG);

	if (!rnet_send(rnet, &pdv))
	{
		raise_user_error(NULL, NULL);
	}

	buf = (byte_t*)xmem_alloc(pdv.size);

	n = variant_encode(&key, buf + total, MAX_LONG);
	total += n;

	stm = stream_alloc(rnet->pbo);

	if (!stream_write_bytes(stm, buf, total))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(buf);
	buf = NULL;

	if (!rnet_recv(rnet, &pdv))
	{
		raise_user_error(NULL, NULL);
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

	return 0;
}