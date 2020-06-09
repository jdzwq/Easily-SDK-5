/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl coap document

	@module	netcoap.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#include "netcoap.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_RAND)

#define GET_THREEBYTE(buf,off)		((buf[off] << 16) | (buf[off+1] << 8) | buf[off+2])
#define PUT_THREEBYTE(buf,off,n)	{buf[off] = (byte_t)((n) >> 16);buf[off+1] = (byte_t)((n) >> 8);buf[off+2] = (byte_t)((n));}

#define OPT_TOKEN_SIZE	20
static int OPT_TOKEN[OPT_TOKEN_SIZE] = {
	COAP_OPTION_IF_MATCH , 
	COAP_OPTION_URI_HOST,
	COAP_OPTION_ETAG,
	COAP_OPTION_IF_NONE_MATCH,
	COAP_OPTION_OBSERVE,
	COAP_OPTION_URI_PORT,
	COAP_OPTION_LOCATION_PATH,
	COAP_OPTION_URI_PATH,
	COAP_OPTION_CONTENT_FORMAT,
	COAP_OPTION_MAXAGE,
	COAP_OPTION_URI_QUERY,
	COAP_OPTION_ACCEPT,
	COAP_OPTION_LOCATION_QUERY,
	COAP_OPTION_BLOCK2,
	COAP_OPTION_BLOCK1,
	COAP_OPTION_SIZE2,
	COAP_OPTION_PROXY_URI,
	COAP_OPTION_PROXY_SCHEME,
	COAP_OPTION_SIZE1,
	COAP_OPTION_NORESPONSE
};

typedef struct _coap_opt_t{
	int opd;
	int len;
	byte_t* opt;
}coap_opt_t;

typedef struct _coap_pdu_t{
	int ver;
	int type;
	int code;
	sword_t tid;

	int tok_len;
	byte_t token[COAP_MAX_TKL];

	coap_opt_t opt_table[OPT_TOKEN_SIZE + 1];

	int pdv_off;
	int pdv_len;
	byte_t payload[COAP_BLK_SIZE];
}coap_pdu_t;

typedef struct _xcoap_t{
	xhand_head head;		//head for xhand_t

	int type;
	xhand_t bio;

	int proto;

	havege_state havs;

	coap_pdu_t snd_pdu;
	coap_pdu_t rcv_pdu;

	int blk_szx;
	int blk_nxt;
	int blk_num;
	int blk_eof;

	sword_t errcode;
	tchar_t errtext[ERR_LEN];
}xcoap_t;

/***********************************************************************************************/

void _coap_error(int errcode, tchar_t* errtext)
{
	switch (errcode)
	{
	case COAP_RESPONSE_200_CODE:
		xscpy(errtext, COAP_RESPONSE_200_TEXT);
		break;
	case COAP_RESPONSE_201_CODE:
		xscpy(errtext, COAP_RESPONSE_201_TEXT);
		break;
	case COAP_RESPONSE_203_CODE:
		xscpy(errtext, COAP_RESPONSE_203_TEXT);
		break;
	case COAP_RESPONSE_204_CODE:
		xscpy(errtext, COAP_RESPONSE_204_TEXT);
		break;
	case COAP_RESPONSE_205_CODE:
		xscpy(errtext, COAP_RESPONSE_205_TEXT);
		break;
	case COAP_RESPONSE_231_CODE:
		xscpy(errtext, COAP_RESPONSE_231_TEXT);
		break;
	case COAP_RESPONSE_400_CODE:
		xscpy(errtext, COAP_RESPONSE_400_TEXT);
		break;
	case COAP_RESPONSE_404_CODE:
		xscpy(errtext, COAP_RESPONSE_404_TEXT);
		break;
	case COAP_RESPONSE_405_CODE:
		xscpy(errtext, COAP_RESPONSE_405_TEXT);
		break;
	case COAP_RESPONSE_415_CODE:
		xscpy(errtext, COAP_RESPONSE_415_TEXT);
		break;
	case COAP_RESPONSE_500_CODE:
		xscpy(errtext, COAP_RESPONSE_500_TEXT);
		break;
	case COAP_RESPONSE_501_CODE:
		xscpy(errtext, COAP_RESPONSE_501_TEXT);
		break;
	case COAP_RESPONSE_503_CODE:
		xscpy(errtext, COAP_RESPONSE_503_TEXT);
		break;
	case COAP_RESPONSE_504_CODE:
		xscpy(errtext, COAP_RESPONSE_504_TEXT);
		break;
	default:
		xscpy(errtext, COAP_RESPONSE_X_242_TEXT);
		break;
	}
}

static unsigned short _coap_dynamic_port(havege_state* phs)
{
	unsigned short port = 0;

	while (port < UDP_MIN_PORT || port > UDP_MAX_PORT)
	{
		port = (unsigned short)havege_rand(phs);
	}

	return port;
}

static dword_t _coap_parse_opt(byte_t* buf, dword_t max, int* popd, byte_t* opt, dword_t* plen)
{
	dword_t total = 0;
	int delta, length;

	if (!max) return 0;

	delta = (buf[0] & 0xf0) >> 4;
	length = (buf[0] & 0x0f);
	total += 1;

	if (delta == 15) //is payload start tag
	{
		if(plen) *plen = 0;
		*popd = -1;
		return total;
	}
	else if (delta == 14) //two bytes extended delta
	{
		delta = GET_SWORD_NET(buf, total) + 269;
		total += 2;
	}
	else if (delta == 13) //one byte extended delta
	{
		delta = GET_BYTE(buf, total) + 13;
		total++;
	}

	if (length == 14) //two bytes extended length
	{
		length = GET_SWORD_NET(buf, total) + 269;
		total += 2;
	}
	else if (length == 13) //one byte extended length
	{
		length = GET_BYTE(buf, total) + 13;
		total++;
	}

	*popd = delta;

	if (opt)
	{
		xmem_copy((void*)opt, (void*)(buf + total), length);
	}
	if (plen)
	{
		*plen = length;
	}

	total += length;

	return total;
}

static dword_t _coap_format_opt(byte_t* buf, dword_t max, int opd, const byte_t* opt, dword_t len)
{
	int delta, length;
	dword_t total = 0;

	if (opd < 0) //is payload start tag
	{
		buf[0] = (byte_t)(15 << 4);
		total++;
		return total;
	}

	delta = opd;
	length = len;

	if (delta < 13) 
	{
		buf[0] = (byte_t)(delta << 4);
		total++;
	}
	else if (delta < 269) 
	{
		buf[0] = (byte_t)(13 << 4);
		total++;

		PUT_BYTE(buf, total, (delta - 13));
		total++;
	}
	else 
	{
		buf[0] = (byte_t)(14 << 4);
		total++;

		PUT_SWORD_NET(buf, 1, delta - 269);
		total += 2;
	}

	if (length < 13) 
	{
		buf[0] |= (byte_t)(length & 0x0f);
	}
	else if (length < 269) 
	{
		buf[0] |= 13;

		PUT_BYTE(buf, total, (byte_t)(length - 13));
		total++;
	}
	else 
	{
		buf[0] |= 14;

		PUT_SWORD_NET(buf, 1, length - 269);
		total += 2;
	}

	xmem_copy((void*)(buf + total), (void*)opt, length);

	total += length;

	return total;
}

static bool_t _coap_write_opt(coap_pdu_t* pdu, int opd, const byte_t* opt, dword_t len)
{
	int i = 0;

	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != opd)
	{
		i++;
	}

	pdu->opt_table[i].opd = opd;
	pdu->opt_table[i].len = len;

	if (pdu->opt_table[i].opt)
	{
		xmem_free(pdu->opt_table[i].opt);
		pdu->opt_table[i].opt = NULL;
	}

	if (len)
	{
		pdu->opt_table[i].opt = (byte_t*)xmem_alloc(pdu->opt_table[i].len);
		xmem_copy((void*)(pdu->opt_table[i].opt), (void*)(opt), len);
	}

	return 1;
}

static bool_t _coap_read_opt(coap_pdu_t* pdu, int opd, byte_t* opt, int *pdw)
{
	int i;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != opd)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == 0)
		return 0;

	*pdw = (*pdw < pdu->opt_table[i].len)? *pdw : pdu->opt_table[i].len;
	xmem_copy((void*)(opt), (void*)(pdu->opt_table[i].opt), *pdw);

	return 1;
}

static void _coap_clear_opt(coap_pdu_t* pdu)
{
	int i = 0;

	while (pdu->opt_table[i].opd)
	{
		if (pdu->opt_table[i].opt)
			xmem_free(pdu->opt_table[i].opt);

		pdu->opt_table[i].opt = NULL;
		pdu->opt_table[i].len = 0;
		i++;
	}
}

static void _coap_clear_pdv(coap_pdu_t* pdu)
{
	xmem_zero((void*)pdu->payload, COAP_BLK_SIZE);
	pdu->pdv_off = 0;
}

static dword_t _coap_parse_pdu(const byte_t* buf, dword_t size, coap_pdu_t* pdu)
{
	dword_t dw, total = 0;
	int opd, tmp, len;
	byte_t opt[COAP_MAX_OPT] = { 0 };
	//header
	pdu->ver = (int)(buf[total] >> 6);
	pdu->type = (int)((buf[total] >> 4) & 0x03);
	pdu->tok_len = (int)(buf[total] & 0x0f);
	pdu->code = (int)(buf[total + 1]);
	pdu->tid = GET_SWORD_NET(buf, total + 2);

	total += COAP_HDR_SIZE;

	if (pdu->ver != COAP_MAJOR_VERSION)
	{
		set_last_error(_T("_xcoap_read_pdu"), _T("coap version invalid"), -1);
		return 0;
	}

	//empty message
	if (pdu->code == 0 && pdu->tok_len)
	{
		set_last_error(_T("_xcoap_read_pdu"), _T("invalid coap empty message"), -1);
		return 0;
	}

	//token length
	if(pdu->tok_len > 8)
	{
		set_last_error(_T("_xcoap_read_pdu"), _T("invalid coap token length"), -1);
		return 0;
	}

	//token
	xmem_copy((void*)(pdu->token), (void*)(buf + total), pdu->tok_len);
	total += pdu->tok_len;

	//options
	opd = tmp = 0;
	len = 0;
	while ((dw = _coap_parse_opt((buf + total), (size - total), &tmp, opt, &len)) > 0)
	{
		total += dw;
		if (tmp < 0)
			break;

		opd += tmp;
		_coap_write_opt(pdu, opd, opt, len);

		tmp = 0;
		len = 0;
	}

	if (size - total > 0 && tmp >= 0)
	{
		set_last_error(_T("_xcoap_read_pdu"), _T("invalid coap payload"), -1);
		return 0;
	}

	//payload
	pdu->pdv_off = 0;
	pdu->pdv_len = size - total;
	xmem_copy((void*)(pdu->payload), (void*)(buf + total), pdu->pdv_len);
	total += pdu->pdv_len;

	return total;
}

static dword_t _coap_format_pdu(byte_t* buf, dword_t size, coap_pdu_t* pdu)
{
	dword_t dw, total = 0;
	int i, opd;

	//header
	if (total + COAP_HDR_SIZE > size)
	{
		set_last_error(_T("_xcoap_format_pdu"), _T("invalid buffer length"), -1);
		return 0;
	}
	if (buf)
	{
		buf[total] = (byte_t)(COAP_MAJOR_VERSION << 6) | (byte_t)(pdu->type << 4) | (byte_t)pdu->tok_len;
		buf[total + 1] = pdu->code;
		PUT_SWORD_NET(buf, (total + 2), pdu->tid);
	}
	total += COAP_HDR_SIZE;
	
	//token
	if (total + pdu->tok_len > size)
	{
		set_last_error(_T("_xcoap_format_pdu"), _T("invalid buffer length"), -1);
		return 0;
	}
	if (buf)
	{
		xmem_copy((void*)(buf + total), pdu->token, pdu->tok_len);
	}
	total += pdu->tok_len;

	//options
	opd = 0;
	i = 0;
	while (pdu->opt_table[i].opd)
	{
		if (pdu->opt_table[i].len)
		{
			dw = _coap_format_opt((buf + total), (size - total), (pdu->opt_table[i].opd - opd), pdu->opt_table[i].opt, pdu->opt_table[i].len);
			opd += (pdu->opt_table[i].opd - opd);
			total += dw;

			if (total + pdu->pdv_off > size)
				break;
		}
		i++;
	}

	//payload start tag
	if (pdu->pdv_off)
	{
		dw = _coap_format_opt((buf + total), (size - total), -1, NULL, 0);
		total += dw;
	}

	//payload
	if (total + pdu->pdv_off > size)
	{
		set_last_error(_T("_xcoap_format_pdu"), _T("invalid buffer length"), -1);
		return 0;
	}
	if (buf)
	{
		xmem_copy((void*)(buf + total), pdu->payload, pdu->pdv_off);
	}
	total += pdu->pdv_off;

	pdu->pdv_off = 0;

	return total;
}

static void _coap_clear_pdu(coap_pdu_t* pdu)
{
	_coap_clear_opt(pdu);

	_coap_clear_pdv(pdu);

	xmem_zero((void*)pdu, sizeof(coap_pdu_t));
}

bool_t _coap_send_request(xcoap_t* ppt)
{
	coap_pdu_t* pdu;

	byte_t pkg_buf[COAP_PKG_SIZE] = { 0 };
	dword_t len;

	int blk;
	int blk_num, blk_nxt, blk_szx;
	byte_t blk_opt[3] = { 0 };

	XDL_ASSERT(ppt->type == _XCOAP_TYPE_CLI);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	pdu->type = COAP_MESSAGE_CON;
	pdu->tid = (sword_t)havege_rand(&ppt->havs);

	if (pdu->code == COAP_REQUEST_GET)
	{
		blk_num = ppt->blk_num;
		blk_nxt = 0;
		blk_szx = ppt->blk_szx;

		blk = (blk_num << 4) | (blk_nxt << 3) | (blk_szx);
		PUT_THREEBYTE(blk_opt, 0, blk);
		_coap_write_opt(pdu, COAP_OPTION_BLOCK2, blk_opt, 3);

		_coap_clear_pdv(pdu);
	}
	else if (pdu->code == COAP_REQUEST_PUT || pdu->code == COAP_REQUEST_POST)
	{
		blk_num = ppt->blk_num;
		blk_nxt = ppt->blk_nxt;
		blk_szx = ppt->blk_szx;

		blk = (blk_num << 4) | (blk_nxt << 3) | (blk_szx);
		PUT_THREEBYTE(blk_opt, 0, blk);
		_coap_write_opt(pdu, COAP_OPTION_BLOCK1, blk_opt, 3);

		pdu->pdv_len = (1 << (blk_szx + 4));
	}

	len = _coap_format_pdu(pkg_buf, COAP_PKG_SIZE, pdu);

	if (!len)
	{
		raise_user_error(NULL, NULL);
	}

	if (!xudp_write(ppt->bio, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_flush(ppt->bio);


	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t _coap_recv_request(xcoap_t* ppt)
{
	coap_pdu_t* pdu;

	byte_t pkg_buf[COAP_PKG_SIZE] = { 0 };
	dword_t len;

	int blk;
	int blk_num, blk_nxt, blk_szx;
	byte_t blk_opt[3] = { 0 };
	int n;

	XDL_ASSERT(ppt->type == _XCOAP_TYPE_SRV);

	TRY_CATCH;

	len = COAP_PKG_SIZE;
	if (!xudp_read(ppt->bio, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	pdu = &ppt->rcv_pdu;

	_coap_clear_opt(pdu);

	len = _coap_parse_pdu(pkg_buf, len, pdu);

	if (!len)
	{
		raise_user_error(NULL, NULL);
	}

	if (pdu->type != COAP_MESSAGE_CON && pdu->type != COAP_MESSAGE_NON)
	{
		raise_user_error(_T("xcoap_recv_request"), _T("message type invalid"));
	}

	if (pdu->code == COAP_REQUEST_GET)
	{
		n = 3;
		if (_coap_read_opt(pdu, COAP_OPTION_BLOCK2, blk_opt, &n))
		{
			blk = GET_THREEBYTE(blk_opt, 0);
			if (blk)
			{
				blk_num = (blk >> 4);
				blk_szx = (blk & 0x07);

				if (blk_num != ppt->blk_num)
				{
					raise_user_error(_T("xcoap_recv_request"), _T("block serial invalid"));
				}

				ppt->blk_szx = blk_szx;
			}
		}
	}
	else if (pdu->code == COAP_REQUEST_PUT || pdu->code == COAP_REQUEST_POST)
	{
		n = 3;
		if (_coap_read_opt(pdu, COAP_OPTION_BLOCK1, blk_opt, &n))
		{
			blk = GET_THREEBYTE(blk_opt, 0);

			blk_num = (blk >> 4);
			blk_nxt = (blk & 0x08) >> 3;
			blk_szx = (blk & 0x07);

			if (blk_num != ppt->blk_num)
			{
				raise_user_error(_T("xcoap_recv_request"), _T("block serial invalid"));
			}

			ppt->blk_nxt = blk_nxt;
			ppt->blk_szx = blk_szx;
			ppt->blk_eof = (blk_nxt) ? 0 : 1;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t _coap_send_response(xcoap_t* ppt)
{
	coap_pdu_t* pdu;

	byte_t pkg_buf[COAP_PKG_SIZE] = { 0 };
	dword_t len;

	int blk;
	int blk_num, blk_nxt, blk_szx;
	byte_t blk_opt[3] = { 0 };

	XDL_ASSERT(ppt->type == _XCOAP_TYPE_SRV);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	pdu->type = COAP_MESSAGE_ACK;
	pdu->code = COAP_RESPONSE_200_CODE;
	pdu->tid = ppt->rcv_pdu.tid;
	pdu->tok_len = ppt->rcv_pdu.tok_len;
	xmem_copy((void*)(pdu->token), (void*)(ppt->rcv_pdu.token), pdu->tok_len);

	if (ppt->rcv_pdu.code == COAP_REQUEST_PUT || ppt->rcv_pdu.code == COAP_REQUEST_POST)
	{
		blk_num = ppt->blk_num;
		blk_nxt = 0;
		blk_szx = ppt->blk_szx;

		blk = (blk_num << 4) | (blk_nxt << 3) | (blk_szx);
		PUT_THREEBYTE(blk_opt, 0, blk);
		_coap_write_opt(pdu, COAP_OPTION_BLOCK1, blk_opt, 3);

		_coap_clear_pdv(pdu);

		ppt->blk_num++;
	}
	else if (ppt->rcv_pdu.code == COAP_REQUEST_GET)
	{
		blk_num = ppt->blk_num;
		blk_nxt = ppt->blk_nxt;
		blk_szx = ppt->blk_szx;

		blk = (blk_num << 4) | (blk_nxt << 3) | (blk_szx);
		PUT_THREEBYTE(blk_opt, 0, blk);
		_coap_write_opt(pdu, COAP_OPTION_BLOCK2, blk_opt, 3);

		pdu->pdv_len = (1 << (blk_szx + 4));

		ppt->blk_num++;
	}

	len = _coap_format_pdu(pkg_buf, COAP_PKG_SIZE, pdu);

	if (!len)
	{
		raise_user_error(NULL, NULL);
	}

	if (!xudp_write(ppt->bio, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_flush(ppt->bio);


	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t _coap_recv_response(xcoap_t* ppt)
{
	coap_pdu_t* pdu;

	byte_t pkg_buf[COAP_PKG_SIZE] = { 0 };
	dword_t len;

	int blk;
	int blk_num, blk_nxt, blk_szx;
	byte_t blk_opt[3] = { 0 };
	int n;

	XDL_ASSERT(ppt->type == _XCOAP_TYPE_CLI);

	TRY_CATCH;

	len = COAP_PKG_SIZE;
	if (!xudp_read(ppt->bio, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	pdu = &ppt->rcv_pdu;

	_coap_clear_pdu(pdu);

	len = _coap_parse_pdu(pkg_buf, len, pdu);

	if (!len)
	{
		raise_user_error(NULL, NULL);
	}

	ppt->errcode = pdu->code;
	_coap_error(pdu->code, ppt->errtext);

	if (!COAP_IS_SUCCEED(ppt->errcode))
	{
		raise_user_error(_T("xcoap_recv_response"), ppt->errtext);
	}

	if (pdu->tid != ppt->snd_pdu.tid)
	{
		raise_user_error(_T("xcoap_recv_response"), _T("message ID invalid"));
	}

	if (pdu->type != COAP_MESSAGE_ACK)
	{
		raise_user_error(_T("xcoap_recv_response"), _T("message type invalid"));
	}

	if (ppt->snd_pdu.code == COAP_REQUEST_GET)
	{
		n = 3;
		if (_coap_read_opt(pdu, COAP_OPTION_BLOCK2, blk_opt, &n))
		{
			blk = GET_THREEBYTE(blk_opt, 0);

			blk_num = (blk >> 4);
			blk_nxt = (blk & 0x08) >> 3;
			blk_szx = (blk & 0x07);

			if (blk_num != ppt->blk_num)
			{
				raise_user_error(_T("xcoap_recv_response"), _T("block serial invalid"));
			}

			ppt->blk_szx = blk_szx;
			ppt->blk_eof = (blk_nxt) ? 0 : 1;

			ppt->blk_num++;
		}
	}
	else if (ppt->snd_pdu.code == COAP_REQUEST_PUT || ppt->snd_pdu.code == COAP_REQUEST_POST)
	{
		n = 3;
		if (_coap_read_opt(pdu, COAP_OPTION_BLOCK1, blk_opt, &n))
		{
			blk = GET_THREEBYTE(blk_opt, 0);

			blk_num = (blk >> 4);

			if (blk_num != ppt->blk_num)
			{
				raise_user_error(_T("xcoap_recv_response"), _T("block serial invalid"));
			}

			ppt->blk_num++;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

/***********************************************************************************************/

xhand_t xcoap_client(const tchar_t* method, const tchar_t* url)
{
	xcoap_t* ppt = NULL;

	tchar_t *potoat, *hostat, *portat, *objat, *qryat;
	int potolen, hostlen, portlen, objlen, qrylen;

	tchar_t host[META_LEN] = { 0 };
	tchar_t addr[ADDR_LEN] = { 0 };
	tchar_t sport[INT_LEN] = { 0 };
	unsigned short port, bind;

	int i, code = 0;
	coap_pdu_t* pdu = NULL;

	byte_t opt[COAP_MAX_OPT];
	dword_t dw;

	TRY_CATCH;

	ppt = (xcoap_t*)xmem_alloc(sizeof(xcoap_t));
	ppt->head.tag = _HANDLE_COAP;

	ppt->type = _XCOAP_TYPE_CLI;

	havege_init(&ppt->havs);

	parse_url(url, &potoat, &potolen, &hostat, &hostlen, &portat, &portlen, &objat, &objlen, &qryat, &qrylen);

	xsncpy(host, hostat, hostlen);
	if (is_ip(host))
	{
		xscpy(addr, host);
	}
	else
	{
		host_addr(host, addr);
	}

	port = xsntos(portat, portlen);
	if (!port)
		port = DEF_COAP_PORT;

	stoxs(port, sport, INT_LEN);

	ppt->bio = xudp_cli(port, addr);
	if (!ppt->bio)
	{
		raise_user_error(NULL, NULL);
	}

	bind = _coap_dynamic_port(&ppt->havs);

	if (!xudp_bind(ppt->bio, bind))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_set_package(ppt->bio, COAP_PKG_SIZE);

	pdu = &ppt->snd_pdu;

	pdu->tok_len = COAP_MAX_TKL;
	for (i = 0; i < pdu->tok_len; i++)
	{
		pdu->token[i] = (byte_t)havege_rand(&ppt->havs);
	}

	for (i = 0; i < sizeof(OPT_TOKEN)/sizeof(int); i++)
	{
		_coap_write_opt(&(ppt->snd_pdu), OPT_TOKEN[i], NULL, 0);
		_coap_write_opt(&(ppt->rcv_pdu), OPT_TOKEN[i], NULL, 0);
	}

	if (compare_text(method, -1, HTTP_METHOD_GET, -1, 1) == 0)
		pdu->code = COAP_REQUEST_GET;
	else if (compare_text(method, -1, HTTP_METHOD_PUT, -1, 1) == 0)
		pdu->code = COAP_REQUEST_PUT;
	else if (compare_text(method, -1, HTTP_METHOD_POST, -1, 1) == 0)
		pdu->code = COAP_REQUEST_POST;
	else if (compare_text(method, -1, HTTP_METHOD_DELETE, -1, 1) == 0)
		pdu->code = COAP_REQUEST_DELETE;
	else
	{
		raise_user_error(_T("xcoap_client"), _T("invalid method"));
	}

#if defined(_UNICODE) || defined(UNICODE)
	dw = ucs_to_utf8(addr, -1, opt, COAP_MAX_OPT);
#else
	dw = mbs_to_utf8(addr, -1, opt, COAP_MAX_OPT);
#endif
	_coap_write_opt(pdu, COAP_OPTION_URI_HOST, opt, dw);


	PUT_SWORD_NET(opt, 0, port);
	dw = 2;
	_coap_write_opt(pdu, COAP_OPTION_URI_PORT, opt, dw);

	if (objlen)
	{
#if defined(_UNICODE) || defined(UNICODE)
		dw = ucs_to_utf8(objat, objlen, opt, COAP_MAX_OPT);
#else
		dw = mbs_to_utf8(objat, objlen, opt, COAP_MAX_OPT);
#endif
		_coap_write_opt(pdu, COAP_OPTION_URI_PATH, opt, dw);
	}

	if (qrylen)
	{
#if defined(_UNICODE) || defined(UNICODE)
		dw = ucs_to_utf8(qryat, qrylen, opt, COAP_MAX_OPT);
#else
		dw = mbs_to_utf8(qryat, qrylen, opt, COAP_MAX_OPT);
#endif
		_coap_write_opt(pdu, COAP_OPTION_URI_QUERY, opt, dw);
	}

	pdu->pdv_len = COAP_PDV_SIZE;

	END_CATCH;

	return &ppt->head;

ONERROR:

	if (ppt)
	{
		if (ppt->bio)
			xudp_close(ppt->bio);

		xmem_free(ppt);
	}

	XDL_TRACE_LAST;

	return NULL;
}

xhand_t	xcoap_server(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size)
{
	xcoap_t* ppt = NULL;
	unsigned short bind;

	int i = 0;

	TRY_CATCH;

	ppt = (xcoap_t*)xmem_alloc(sizeof(xcoap_t));
	ppt->head.tag = _HANDLE_COAP;

	ppt->type = _XCOAP_TYPE_SRV;

	havege_init(&ppt->havs);

	ppt->bio = xudp_srv(port, addr, pack, size);
	if (!ppt->bio)
	{
		raise_user_error(NULL, NULL);
	}

	bind = _coap_dynamic_port(&ppt->havs);

	if (!xudp_bind(ppt->bio, bind))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_set_package(ppt->bio, COAP_PKG_SIZE);

	for (i = 0; i < sizeof(OPT_TOKEN) / sizeof(int); i++)
	{
		_coap_write_opt(&(ppt->snd_pdu), OPT_TOKEN[i], NULL, 0);
		_coap_write_opt(&(ppt->rcv_pdu), OPT_TOKEN[i], NULL, 0);
	}

	END_CATCH;

	return &ppt->head;
ONERROR:

	if (ppt)
	{
		if (ppt->bio)
			xudp_close(ppt->bio);

		xmem_free(ppt);
	}

	XDL_TRACE_LAST;

	return NULL;
}

xhand_t xcoap_bio(xhand_t coap)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	return ppt->bio;
}

void xcoap_close(xhand_t coap)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	_coap_clear_pdu(&ppt->snd_pdu);

	_coap_clear_pdu(&ppt->rcv_pdu);

	if (ppt->bio)
		xudp_close(ppt->bio);

	xmem_free(ppt);
}

int xcoap_method(xhand_t coap, tchar_t* buf, int max)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int len;
	coap_pdu_t* pdu;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	if (ppt->type == _XCOAP_TYPE_CLI)
		pdu = &ppt->snd_pdu;
	else
		pdu = &ppt->rcv_pdu;

	switch (pdu->code)
	{
	case COAP_REQUEST_GET:
		len = xslen(HTTP_METHOD_GET);
		len = (len < max) ? len : max;
		xsncpy(buf, HTTP_METHOD_GET, len);
		break;
	case COAP_REQUEST_PUT:
		len = xslen(HTTP_METHOD_PUT);
		len = (len < max) ? len : max;
		xsncpy(buf, HTTP_METHOD_PUT, len);
		break;
	case COAP_REQUEST_POST:
		len = xslen(HTTP_METHOD_POST);
		len = (len < max) ? len : max;
		xsncpy(buf, HTTP_METHOD_POST, len);
		break;
	case COAP_REQUEST_DELETE:
		len = xslen(HTTP_METHOD_DELETE);
		len = (len < max) ? len : max;
		xsncpy(buf, HTTP_METHOD_DELETE, len);
		break;
	default:
		xscpy(buf, _T(""));
		len = 0;
		break;
	}

	return len;
}

sword_t xcoap_msgid(xhand_t coap)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	coap_pdu_t* pdu;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	if (ppt->type == _XCOAP_TYPE_CLI)
		pdu = &ppt->snd_pdu;
	else
		pdu = &ppt->rcv_pdu;

	return pdu->tid;
}

int xcoap_get_object(xhand_t coap, tchar_t* buf, int max)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i, len = 0;
	coap_pdu_t* pdu;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	XDL_ASSERT(ppt->type == _XCOAP_TYPE_SRV);

	pdu = &ppt->rcv_pdu;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != COAP_OPTION_URI_PATH)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == COAP_OPTION_URI_PATH)
	{
#if defined(_UNICODE) || defined(UNICODE)
		len = utf8_to_ucs((pdu->opt_table[i].opt), (pdu->opt_table[i].len), buf, max);
#else
		len = utf8_to_mbs((pdu->opt_table[i].opt), (pdu->opt_table[i].len), buf, max);
#endif
	}

	return len;
}

void xcoap_set_object(xhand_t coap, const tchar_t* obj, int len)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i = 0;
	coap_pdu_t* pdu;
	int opd = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	XDL_ASSERT(ppt->type == _XCOAP_TYPE_CLI);

	pdu = &ppt->snd_pdu;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != COAP_OPTION_URI_PATH)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == COAP_OPTION_URI_PATH)
	{
		if (pdu->opt_table[i].opt)
			xmem_free(pdu->opt_table[i].opt);
		pdu->opt_table[i].len = 0;

		if (!is_null(obj) || !len)
		{
#if defined(_UNICODE) || defined(UNICODE)
			pdu->opt_table[i].len = ucs_to_utf8(obj, len, NULL, MAX_LONG);
#else
			pdu->opt_table[i].len = mbs_to_utf8(obj, len, NULL, MAX_LONG);
#endif
			pdu->opt_table[i].opt = (byte_t*)xmem_alloc(pdu->opt_table[i].len);
#if defined(_UNICODE) || defined(UNICODE)
			pdu->opt_table[i].len = ucs_to_utf8(obj, len, (pdu->opt_table[i].opt), (pdu->opt_table[i].len));
#else
			pdu->opt_table[i].len = mbs_to_utf8(obj, len, (pdu->opt_table[i].opt), (pdu->opt_table[i].len));
#endif
		}
	}
}

int xcoap_get_query(xhand_t coap, tchar_t* buf, int max)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i, len = 0;
	coap_pdu_t* pdu;
	int opd = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	XDL_ASSERT(ppt->type == _XCOAP_TYPE_SRV);

	pdu = &ppt->rcv_pdu;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != COAP_OPTION_URI_QUERY)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == COAP_OPTION_URI_QUERY)
	{
#if defined(_UNICODE) || defined(UNICODE)
		len = utf8_to_ucs((pdu->opt_table[i].opt), (pdu->opt_table[i].len), buf, max);
#else
		len = utf8_to_mbs((pdu->opt_table[i].opt), (pdu->opt_table[i].len), buf, max);
#endif
	}

	return len;
}

void xcoap_set_query(xhand_t coap, const tchar_t* qry, int len)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i = 0;
	coap_pdu_t* pdu;
	int opd = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	XDL_ASSERT(ppt->type == _XCOAP_TYPE_CLI);

	pdu = &ppt->snd_pdu;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != COAP_OPTION_URI_QUERY)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == COAP_OPTION_URI_QUERY)
	{
		if (pdu->opt_table[i].opt)
			xmem_free(pdu->opt_table[i].opt);
		pdu->opt_table[i].len = 0;

		if (!is_null(qry) || !len)
		{
#if defined(_UNICODE) || defined(UNICODE)
			pdu->opt_table[i].len = ucs_to_utf8(qry, len, NULL, MAX_LONG);
#else
			pdu->opt_table[i].len = mbs_to_utf8(qry, len, NULL, MAX_LONG);
#endif
			pdu->opt_table[i].opt = (byte_t*)xmem_alloc(pdu->opt_table[i].len);
#if defined(_UNICODE) || defined(UNICODE)
			pdu->opt_table[i].len = ucs_to_utf8(qry, len, (pdu->opt_table[i].opt), (pdu->opt_table[i].len));
#else
			pdu->opt_table[i].len = mbs_to_utf8(qry, len, (pdu->opt_table[i].opt), (pdu->opt_table[i].len));
#endif
		}
	}
}

int xcoap_get_content_type(xhand_t coap)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i, len = 0;
	coap_pdu_t* pdu;
	int opd = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	pdu = &ppt->rcv_pdu;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != COAP_OPTION_CONTENT_FORMAT)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == COAP_OPTION_CONTENT_FORMAT)
	{
		if (pdu->opt_table[i].len)
			return (int)GET_SWORD_NET(pdu->opt_table[i].opt, 0);
		else
			return 0;
	}

	return 0;
}

void xcoap_set_content_type(xhand_t coap, int type)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i = 0;
	coap_pdu_t* pdu;
	int opd = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	pdu = &ppt->snd_pdu;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != COAP_OPTION_CONTENT_FORMAT)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == COAP_OPTION_CONTENT_FORMAT)
	{
		pdu->opt_table[i].len = sizeof(sword_t);

		if (!(pdu->opt_table[i].opt))
			pdu->opt_table[i].opt = (byte_t*)xmem_alloc(pdu->opt_table[i].len);

		PUT_SWORD_NET((pdu->opt_table[i].opt), 0, (sword_t)type);
	}
}

int xcoap_get_content_size(xhand_t coap)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i, len = 0;
	coap_pdu_t* pdu;
	int opd = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	pdu = &ppt->rcv_pdu;

	if (ppt->type == _XCOAP_TYPE_CLI)
	{
		opd = COAP_OPTION_SIZE2;
	}
	else
	{
		opd = COAP_OPTION_SIZE1;
	}

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != opd)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == opd)
	{
		if (pdu->opt_table[i].len)
			return (int)GET_SWORD_NET(pdu->opt_table[i].opt, 0);
		else
			return 0;
	}

	return 0;
}

void xcoap_set_content_size(xhand_t coap, int type)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i = 0;
	coap_pdu_t* pdu;
	int opd = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	pdu = &ppt->snd_pdu;

	if (ppt->type == _XCOAP_TYPE_CLI)
	{
		opd = COAP_OPTION_SIZE1;
	}
	else
	{
		opd = COAP_OPTION_SIZE2;
	}

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != COAP_OPTION_CONTENT_FORMAT)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == COAP_OPTION_CONTENT_FORMAT)
	{
		pdu->opt_table[i].len = sizeof(sword_t);

		if (!(pdu->opt_table[i].opt))
			pdu->opt_table[i].opt = (byte_t*)xmem_alloc(pdu->opt_table[i].len);

		PUT_SWORD_NET((pdu->opt_table[i].opt), 0, (sword_t)type);
	}
}

int xcoap_get_option(xhand_t coap, int opd, tchar_t* buf, int max)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i, len = 0;
	coap_pdu_t* pdu;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	pdu = &ppt->rcv_pdu;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != opd)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == opd)
	{
#if defined(_UNICODE) || defined(UNICODE)
		len = utf8_to_ucs((pdu->opt_table[i].opt), (pdu->opt_table[i].len), buf, max);
#else
		len = utf8_to_mbs((pdu->opt_table[i].opt), (pdu->opt_table[i].len), buf, max);
#endif
	}

	return len;
}

void xcoap_set_option(xhand_t coap, int opd, const tchar_t* opt, int len)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	int i = 0;
	coap_pdu_t* pdu;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	pdu = &ppt->snd_pdu;

	i = 0;
	while (pdu->opt_table[i].opd && pdu->opt_table[i].opd != opd)
	{
		i++;
	}

	if (pdu->opt_table[i].opd == opd)
	{
		if (pdu->opt_table[i].opt)
			xmem_free(pdu->opt_table[i].opt);
		pdu->opt_table[i].len = 0;

		if (!is_null(opt) || !len)
		{
#if defined(_UNICODE) || defined(UNICODE)
			pdu->opt_table[i].len = ucs_to_utf8(opt, len, NULL, MAX_LONG);
#else
			pdu->opt_table[i].len = mbs_to_utf8(opt, len, NULL, MAX_LONG);
#endif
			pdu->opt_table[i].opt = (byte_t*)xmem_alloc(pdu->opt_table[i].len);
#if defined(_UNICODE) || defined(UNICODE)
			pdu->opt_table[i].len = ucs_to_utf8(opt, len, (pdu->opt_table[i].opt), (pdu->opt_table[i].len));
#else
			pdu->opt_table[i].len = mbs_to_utf8(opt, len, (pdu->opt_table[i].opt), (pdu->opt_table[i].len));
#endif
		}
	}
}

void xcoap_set_blockwise(xhand_t coap, int szx)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	XDL_ASSERT(szx >= 0 && szx <= 6);

	ppt->blk_nxt = 1;
	ppt->blk_szx = szx;
}


bool_t xcoap_connect(xhand_t coap)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	coap_pdu_t* pdu;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	pdu = &ppt->snd_pdu;

	switch (pdu->code)
	{
	case COAP_REQUEST_GET:
		return _coap_send_request(ppt);
	case COAP_REQUEST_DELETE:
		if (!_coap_send_request(ppt))
			return 0;

		return _coap_recv_response(ppt);
	default:
		return 1;
	}
}

bool_t xcoap_accept(xhand_t coap)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	coap_pdu_t* pdu;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	if (!_coap_recv_request(ppt))
		return 0;

	pdu = &ppt->rcv_pdu;

	if (pdu->code == COAP_REQUEST_PUT || pdu->code == COAP_REQUEST_POST)
		return _coap_send_response(ppt);

	return 1;
}

void xcoap_abort(xhand_t coap, int errcode)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);

	coap_pdu_t* pdu;

	byte_t pkg_buf[COAP_PKG_SIZE] = { 0 };
	dword_t len;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	TRY_CATCH;

	ppt->errcode = errcode;

	_coap_error(ppt->errcode, ppt->errtext);

	pdu = &ppt->snd_pdu;

	_coap_clear_pdv(pdu);

	if (COAP_IS_SUCCEED(ppt->errcode))
		pdu->type = COAP_MESSAGE_ACK;
	else
		pdu->type = COAP_MESSAGE_RST;
	pdu->code = errcode;
	pdu->tid = ppt->rcv_pdu.tid;
	pdu->tok_len = ppt->rcv_pdu.tok_len;
	xmem_copy((void*)(pdu->token), (void*)(ppt->rcv_pdu.token), pdu->tok_len);

	len = _coap_format_pdu(pkg_buf, COAP_PKG_SIZE, pdu);

	if (!xudp_write(ppt->bio, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_flush(ppt->bio);

	END_CATCH;

	return;
ONERROR:

	XDL_TRACE_LAST;

	return;
}

bool_t xcoap_recv(xhand_t coap, byte_t* buf, dword_t* pch)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	coap_pdu_t* pdu;

	byte_t pkg_buf[COAP_PKG_SIZE] = { 0 };
	dword_t len, pos = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	TRY_CATCH;

	pdu = &ppt->rcv_pdu;

	while (pos < *pch)
	{
		len = ((*pch - pos) < (pdu->pdv_len - pdu->pdv_off)) ? (*pch - pos) : (pdu->pdv_len - pdu->pdv_off);
		xmem_copy((void*)(buf + pos), (void*)(pdu->payload + pdu->pdv_off), len);
		pdu->pdv_off += len;
		pos += len;

		if (pos == *pch)
			break;

		if ((pdu->pdv_len == pdu->pdv_off) && !ppt->blk_eof)
		{
			if (ppt->type == _XCOAP_TYPE_SRV)
			{
				if (!_coap_recv_request(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (!_coap_send_response(ppt))
				{
					raise_user_error(NULL, NULL);
				}
			}
			else
			{
				if (!_coap_recv_response(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (!_coap_send_request(ppt))
				{
					raise_user_error(NULL, NULL);
				}
			}
		}
		else if (!len)
		{
			break;
		}
	}

	END_CATCH;

	*pch = pos;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	*pch = 0;

	return 0;
}

bool_t xcoap_send(xhand_t coap, const byte_t* buf, dword_t *pch)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	coap_pdu_t* pdu;

	byte_t pkg_buf[COAP_PKG_SIZE] = { 0 };
	dword_t len, pos = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;
	pdu->pdv_len = 1 << (ppt->blk_szx + 4);

	while (pos < *pch)
	{
		len = ((*pch - pos) < (pdu->pdv_len - pdu->pdv_off)) ? (*pch - pos) : (pdu->pdv_len - pdu->pdv_off);
		xmem_copy((void*)(pdu->payload + pdu->pdv_off), (void*)(buf + pos), len);
		pdu->pdv_off += len;
		pos += len;

		if (pdu->pdv_off == pdu->pdv_len)
		{
			if (ppt->type == _XCOAP_TYPE_SRV)
			{
				if (!_coap_send_response(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (!_coap_recv_request(ppt))
				{
					raise_user_error(NULL, NULL);
				}
			}
			else
			{
				if (!_coap_send_request(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (!_coap_recv_response(ppt))
				{
					raise_user_error(NULL, NULL);
				}
			}
		}
	}

	END_CATCH;

	*pch = pos;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	*pch = 0;

	return 0;
}

bool_t xcoap_flush(xhand_t coap)
{
	xcoap_t* ppt = TypePtrFromHead(xcoap_t, coap);
	coap_pdu_t* pdu;
	byte_t pkg_buf[COAP_PKG_SIZE] = { 0 };
	dword_t pos = 0;

	XDL_ASSERT(coap && coap->tag == _HANDLE_COAP);

	TRY_CATCH;

	ppt->blk_nxt = 0;

	pdu = &ppt->snd_pdu;
	pdu->pdv_len = 1 << (ppt->blk_szx + 4);

	if (ppt->type == _XCOAP_TYPE_SRV)
	{
		if (!_coap_send_response(ppt))
		{
			raise_user_error(NULL, NULL);
		}

		if (!_coap_recv_request(ppt))
		{
			raise_user_error(NULL, NULL);
		}
	}
	else
	{
		if (!_coap_send_request(ppt))
		{
			raise_user_error(NULL, NULL);
		}

		if (!_coap_recv_response(ppt))
		{
			raise_user_error(NULL, NULL);
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}


#endif /*XDK_SUPPORT_SOCK*/
