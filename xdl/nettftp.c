/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl tftp document

	@module	nettftp.c | implement file

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

#include "nettftp.h"
#include "netudp.h"
#include "bioinf.h"
#include "stream.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK)

typedef struct _tftp_pdu_t{
	int type;

	tchar_t method[INT_LEN];
	tchar_t file[PATH_LEN];
	tchar_t mode[NUM_LEN];

	dword_t size;
	sword_t isdir;
	tchar_t ftime[DATE_LEN];

	int errcode;
	tchar_t errtext[ERR_LEN + 1];

	int pdv_num;
	int pdv_off;
	int pdv_len;
	int pdv_eof;
	byte_t payload[TFTP_PDV_SIZE];
}tftp_pdu_t;

typedef struct _xtftp_t{
	xhand_head head;		//head for xhand_t

	if_bio_t* pif;

	int type;
	havege_state havs;

	tftp_pdu_t snd_pdu;
	tftp_pdu_t rcv_pdu;

	int serial;

	sword_t errcode;
	tchar_t errtext[ERR_LEN];
}xtftp_t;

/***********************************************************************************************/

static unsigned short _tftp_port(xtftp_t* pftp)
{
	unsigned short port = 0;

	while (port < UDP_MIN_PORT || port > UDP_MAX_PORT)
	{
		port = (unsigned short)havege_rand(&pftp->havs);
	}

	return port;
}

void _tftp_error(int errcode, tchar_t* errtext)
{
	switch (errcode)
	{
	case TFTP_CODE_NOTDEF:
		xscpy(errtext, TFTP_CODE_NOTDEF_TEXT);
		break;
	case TFTP_CODE_NOTFIND:
		xscpy(errtext, TFTP_CODE_NOTFIND_TEXT);
		break;
	case TFTP_CODE_REJECT:
		xscpy(errtext, TFTP_CODE_REJECT_TEXT);
		break;
	case TFTP_CODE_DSKFULL:
		xscpy(errtext, TFTP_CODE_DSKFULL_TEXT);
		break;
	case TFTP_CODE_UNTID:
		xscpy(errtext, TFTP_CODE_UNTID_TEXT);
		break;
	case TFTP_CODE_EXISTS:
		xscpy(errtext, TFTP_CODE_EXISTS_TEXT);
		break;
	case TFTP_CODE_NOUSER:
		xscpy(errtext, TFTP_CODE_NOUSER_TEXT);
		break;
	}
}

static dword_t _tftp_parse_pdu(const byte_t* buf, dword_t size, tftp_pdu_t* pdu)
{
	dword_t total = 0;
	int n, len;

	pdu->type = GET_SWORD_LIT(buf, total);
	total += 2;

	switch (pdu->type)
	{
	case TFTP_PDU_HEAD:
		pdu->size = GET_DWORD_LIT(buf, total);
		total += 4;

		pdu->isdir = GET_SWORD_LIT(buf, total);
		total += 2;

		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = utf8_to_ucs((buf + total), len, pdu->ftime, DATE_LEN);
#else
		n = utf8_to_mbs((buf + total), len, pdu->ftime, DATE_LEN);
#endif
		pdu->ftime[n] = _T('\0');
		total += (len + 1);

		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = utf8_to_ucs((buf + total), len, pdu->file, PATH_LEN);
#else
		n = utf8_to_mbs((buf + total), len, pdu->file, PATH_LEN);
#endif
		pdu->file[n] = _T('\0');
		total += (len + 1);

		break;
	case TFTP_PDU_DEL:
		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = utf8_to_ucs((buf + total), len, pdu->file, PATH_LEN);
#else
		n = utf8_to_mbs((buf + total), len, pdu->file, PATH_LEN);
#endif
		pdu->file[n] = _T('\0');
		total += (len + 1);

		break;
	case TFTP_PDU_RRQ:
		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = utf8_to_ucs((buf + total), len, pdu->file, PATH_LEN);
#else
		n = utf8_to_mbs((buf + total), len, pdu->file, PATH_LEN);
#endif
		pdu->file[n] = _T('\0');
		total += (len + 1);

		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = utf8_to_ucs((buf + total), len, pdu->mode, NUM_LEN);
#else
		n = utf8_to_mbs((buf + total), len, pdu->mode, NUM_LEN);
#endif
		pdu->mode[n] = _T('\0');
		total += (len + 1);

		break;
	case TFTP_PDU_WRQ:
		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = utf8_to_ucs((buf + total), len, pdu->file, PATH_LEN);
#else
		n = utf8_to_mbs((buf + total), len, pdu->file, PATH_LEN);
#endif
		pdu->file[n] = _T('\0');
		total += (len + 1);

		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = utf8_to_ucs((buf + total), len, pdu->mode, NUM_LEN);
#else
		n = utf8_to_mbs((buf + total), len, pdu->mode, NUM_LEN);
#endif
		pdu->mode[n] = _T('\0');
		total += (len + 1);

		break;
	case TFTP_PDU_DATA:
		pdu->pdv_num = GET_SWORD_LIT(buf, total);
		total += 2;

		//payload
		pdu->pdv_off = 0;
		pdu->pdv_len = size - total;
		pdu->pdv_eof = (pdu->pdv_len == TFTP_PDV_SIZE) ? 0 : 1;
		xmem_copy((void*)(pdu->payload), (void*)(buf + total), pdu->pdv_len);
		total += pdu->pdv_len;
		break;
	case TFTP_PDU_ACK:
		pdu->pdv_num = GET_SWORD_LIT(buf, total);
		total += 2;

		break;
	case TFTP_PDU_ERR:
		pdu->errcode = GET_SWORD_LIT(buf, total);
		total += 2;

		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = utf8_to_ucs((buf + total), len, pdu->errtext, ERR_LEN);
#else
		n = utf8_to_mbs((buf + total), len, pdu->errtext, ERR_LEN);
#endif
		pdu->errtext[n] = _T('\0');
		total += (len + 1);

		break;
	}

	return total;
}

static dword_t _tftp_format_pdu(byte_t* buf, dword_t size, tftp_pdu_t* pdu)
{
	dword_t dw, total = 0;

	PUT_SWORD_LIT(buf, total, pdu->type);
	total += 2;

	switch (pdu->type)
	{
	case TFTP_PDU_HEAD:
		PUT_DWORD_LIT(buf, total, pdu->size);
		total += 4;

		PUT_SWORD_LIT(buf, total, pdu->isdir);
		total += 2;

#ifdef _UNICODE
		dw = ucs_to_utf8(pdu->ftime, -1, (buf + total), TFTP_PDV_SIZE - total);
#else
		dw = mbs_to_utf8(pdu->ftime, -1, (buf + total), TFTP_PDV_SIZE - total);
#endif
		total += dw;

		buf[total] = '\0';
		total++;

#ifdef _UNICODE
		dw = ucs_to_utf8(pdu->file, -1, (buf + total), TFTP_PDV_SIZE - total);
#else
		dw = mbs_to_utf8(pdu->file, -1, (buf + total), TFTP_PDV_SIZE - total);
#endif
		total += dw;

		buf[total] = '\0';
		total++;

		break;
	case TFTP_PDU_DEL:
#ifdef _UNICODE
		dw = ucs_to_utf8(pdu->file, -1, (buf + total), TFTP_PDV_SIZE - total);
#else
		dw = mbs_to_utf8(pdu->file, -1, (buf + total), TFTP_PDV_SIZE - total);
#endif
		total += dw;

		buf[total] = '\0';
		total++;

		break;
	case TFTP_PDU_RRQ:
#ifdef _UNICODE
		dw = ucs_to_utf8(pdu->file, -1, (buf + total), TFTP_PDV_SIZE - total);
#else
		dw = mbs_to_utf8(pdu->file, -1, (buf + total), TFTP_PDV_SIZE - total);
#endif
		total += dw;
		
		buf[total] = '\0';
		total++;

#ifdef _UNICODE
		dw = ucs_to_utf8(pdu->mode, -1, (buf + total), TFTP_PDV_SIZE - total);
#else
		dw = mbs_to_utf8(pdu->mode, -1, (buf + total), TFTP_PDV_SIZE - total);
#endif
		total += dw;

		buf[total] = '\0';
		total++;

		break;
	case TFTP_PDU_WRQ:
#ifdef _UNICODE
		dw = ucs_to_utf8(pdu->file, -1, (buf + total), TFTP_PDV_SIZE - total);
#else
		dw = mbs_to_utf8(pdu->file, -1, (buf + total), TFTP_PDV_SIZE - total);
#endif
		total += dw;

		buf[total] = '\0';
		total++;

#ifdef _UNICODE
		dw = ucs_to_utf8(pdu->mode, -1, (buf + total), TFTP_PDV_SIZE - total);
#else
		dw = mbs_to_utf8(pdu->mode, -1, (buf + total), TFTP_PDV_SIZE - total);
#endif
		total += dw;

		buf[total] = '\0';
		total++;

		break;
	case TFTP_PDU_DATA:
		PUT_SWORD_LIT(buf, total, pdu->pdv_num);
		total += 2;

		xmem_copy((void*)(buf + total), (void*)pdu->payload, pdu->pdv_off);
		total += pdu->pdv_off;
		break;
	case TFTP_PDU_ACK:
		PUT_SWORD_LIT(buf, total, pdu->pdv_num);
		total += 2;

		break;
	case TFTP_PDU_ERR:
		PUT_SWORD_LIT(buf, total, pdu->errcode);
		total += 2;

#ifdef _UNICODE
		dw = ucs_to_utf8(pdu->errtext, -1, (buf + total), TFTP_PDV_SIZE - total);
#else
		dw = mbs_to_utf8(pdu->errtext, -1, (buf + total), TFTP_PDV_SIZE - total);
#endif
		total += dw;

		buf[total] = '\0';
		total++;

		break;
	}

	return total;
}

static void _tftp_clear_pdv(tftp_pdu_t* pdu)
{
	xmem_zero((void*)pdu->payload, TFTP_PDV_SIZE);
	pdu->pdv_off = 0;
}

static void _tftp_clear_pdu(tftp_pdu_t* pdu)
{
	_tftp_clear_pdv(pdu);

	xmem_zero((void*)pdu, sizeof(tftp_pdu_t));
}

/****************************************************************************************************/

bool_t _tftp_send_request(xtftp_t* ppt)
{
	tftp_pdu_t* pdu;

	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t len;

	XDL_ASSERT(ppt->type == _XTFTP_TYPE_CLI);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	if (pdu->type == TFTP_PDU_DATA)
	{
		pdu->pdv_num = ppt->serial;
	}

	len = _tftp_format_pdu(pkg_buf, TFTP_PKG_SIZE, pdu);

	if (!(*ppt->pif->pf_write)(ppt->pif->fd, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	(*ppt->pif->pf_flush)(ppt->pif->fd);

	if (pdu->type == TFTP_PDU_DATA)
	{
		ppt->serial ++;
		pdu->pdv_off = 0;
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t _tftp_recv_request(xtftp_t* ppt)
{
	tftp_pdu_t* pdu;

	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t len;

	XDL_ASSERT(ppt->type == _XTFTP_TYPE_SRV);

	TRY_CATCH;

	len = TFTP_PKG_SIZE;
	if (!(*ppt->pif->pf_read)(ppt->pif->fd, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	pdu = &ppt->rcv_pdu;

	_tftp_clear_pdu(pdu);

	len = _tftp_parse_pdu(pkg_buf, len, pdu);

	if (!len)
	{
		raise_user_error(NULL, NULL);
	}

	switch (pdu->type)
	{
	case TFTP_PDU_WRQ:
		xscpy(pdu->method, TFTP_METHOD_PUT);
		break;
	case TFTP_PDU_RRQ:
		xscpy(pdu->method, TFTP_METHOD_GET);
		break;
	case TFTP_PDU_HEAD:
		xscpy(pdu->method, TFTP_METHOD_HEAD);
		break;
	case TFTP_PDU_DEL:
		xscpy(pdu->method, TFTP_METHOD_DELETE);
		break;
	default:
		break;
	}

	if (pdu->type == TFTP_PDU_ERR)
	{
		raise_user_error(_T("_tftp_recv_request"), pdu->errtext);
	}
	
	if (pdu->type == TFTP_PDU_DATA)
	{
		if (pdu->pdv_num != ppt->serial)
		{
			raise_user_error(_T("_tftp_recv_request"), _T("invalid serial number"));
		}
		ppt->serial++;
	}

	END_CATCH;

	return 1;
ONERROR:

	return 0;
}

bool_t _tftp_send_response(xtftp_t* ppt)
{
	tftp_pdu_t* pdu;

	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t len;

	XDL_ASSERT(ppt->type == _XTFTP_TYPE_SRV);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	if (pdu->type == TFTP_PDU_DATA)
	{
		pdu->pdv_num = ppt->serial;
	}

	len = _tftp_format_pdu(pkg_buf, TFTP_PKG_SIZE, pdu);

	if (!(*ppt->pif->pf_write)(ppt->pif->fd, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	(*ppt->pif->pf_flush)(ppt->pif->fd);

	if (pdu->type == TFTP_PDU_DATA)
	{
		ppt->serial++;
		pdu->pdv_off = 0;
	}


	END_CATCH;

	return 1;
ONERROR:

	return 0;
}

bool_t _tftp_recv_response(xtftp_t* ppt)
{
	tftp_pdu_t* pdu;

	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t len;

	XDL_ASSERT(ppt->type == _XTFTP_TYPE_CLI);

	TRY_CATCH;

	len = TFTP_PKG_SIZE;
	if (!(*ppt->pif->pf_read)(ppt->pif->fd, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	pdu = &ppt->rcv_pdu;

	_tftp_clear_pdu(pdu);

	len = _tftp_parse_pdu(pkg_buf, len, pdu);

	if (!len)
	{
		raise_user_error(NULL, NULL);
	}

	if (pdu->type == TFTP_PDU_ERR)
	{
		raise_user_error(_T("_tftp_recv_response"), pdu->errtext);
	}

	if (pdu->type == TFTP_PDU_DATA)
	{
		if (pdu->pdv_num != ppt->serial)
		{
			raise_user_error(_T("_tftp_recv_request"), _T("invalid serial number"));
		}
		ppt->serial++;
	}

	END_CATCH;

	return 1;
ONERROR:

	return 0;
}

/***********************************************************************************************/

xhand_t xtftp_client(const tchar_t* method, const tchar_t* url)
{
	xtftp_t* pftp = NULL;

	tchar_t *potoat, *hostat, *portat, *objat, *qryat;
	int potolen, hostlen, portlen, objlen, qrylen;

	tchar_t host[META_LEN] = { 0 };
	tchar_t addr[ADDR_LEN] = { 0 };
	unsigned short port, bind;

	tftp_pdu_t* pdu = NULL;
	xhand_t bio = NULL;

	TRY_CATCH;

	pftp = (xtftp_t*)xmem_alloc(sizeof(xtftp_t));
	pftp->head.tag = _HANDLE_TFTP;

	havege_init(&pftp->havs);
	bind = _tftp_port(pftp);

	pftp->type = _XTFTP_TYPE_CLI;

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
		port = DEF_TFTP_PORT;

	bio = xudp_cli(port, addr);
	if (!bio)
	{
		raise_user_error(NULL, NULL);
	}

	if (!xudp_bind(bio, bind))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_set_package(bio, TFTP_PKG_SIZE);

	pftp->pif = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));
	get_bio_interface(bio, pftp->pif);
	bio = NULL;

	pftp->serial = 0;

	pdu = &pftp->snd_pdu;

	xscpy(pdu->method, method);
	xsncpy(pdu->file, objat, objlen);
	xscpy(pdu->mode, _T("octet"));

	END_CATCH;

	return &pftp->head;

ONERROR:
	XDL_TRACE_LAST;

	if (bio)
		xudp_close(bio);

	if (pftp)
	{
		if (pftp->pif)
			xmem_free(pftp->pif);

		xmem_free(pftp);
	}

	return NULL;
}

xhand_t	xtftp_server(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size)
{
	xtftp_t* pftp = NULL;
	unsigned short bind;
	xhand_t bio = NULL;

	TRY_CATCH;

	pftp = (xtftp_t*)xmem_alloc(sizeof(xtftp_t));
	pftp->head.tag = _HANDLE_TFTP;

	pftp->type = _XTFTP_TYPE_SRV;

	havege_init(&pftp->havs);
	bind = _tftp_port(pftp);

	bio = xudp_srv(port, addr, pack, size);
	if (!bio)
	{
		raise_user_error(NULL, NULL);
	}

	if (!xudp_bind(bio, bind))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xudp_connect(bio, port, addr))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_set_package(bio, TFTP_PKG_SIZE);

	pftp->pif = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));
	get_bio_interface(bio, pftp->pif);
	bio = NULL;

	END_CATCH;

	return &pftp->head;
ONERROR:
	XDL_TRACE_LAST;

	if (bio)
		xudp_close(bio);

	if (pftp)
	{
		if (pftp->pif)
			xmem_free(pftp->pif);

		xmem_free(pftp);
	}

	return NULL;
}

xhand_t xtftp_bio(xhand_t tftp)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	return (pftp->pif)? pftp->pif->fd : NULL;
}

void xtftp_close(xhand_t tftp)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->pif)
	{
		xudp_close(pftp->pif->fd);
		xmem_free(pftp->pif);
	}

	xmem_free(pftp);
}

int xtftp_method(xhand_t tftp, tchar_t* buf, int max)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	int len;
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->type == _XTFTP_TYPE_SRV)
		pdu = &pftp->rcv_pdu;
	else
		pdu = &pftp->snd_pdu;

	len = xslen(pdu->method);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, pdu->method, len);
	}

	return len;
}

void xtftp_set_isdir(xhand_t tftp, bool_t isdir)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	pdu = &pftp->snd_pdu;

	pdu->isdir = (sword_t)isdir;
}

bool_t xtftp_get_isdir(xhand_t tftp)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->type == _XTFTP_TYPE_SRV)
		pdu = &pftp->rcv_pdu;
	else
		pdu = &pftp->snd_pdu;

	return (bool_t)pdu->isdir;
}

void xtftp_set_filesize(xhand_t tftp, dword_t size)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	pdu = &pftp->snd_pdu;

	pdu->size = size;
}

dword_t xtftp_get_filesize(xhand_t tftp)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->type == _XTFTP_TYPE_SRV)
		pdu = &pftp->rcv_pdu;
	else
		pdu = &pftp->snd_pdu;

	return pdu->size;
}

void xtftp_set_filetime(xhand_t tftp, const tchar_t* ftime)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	pdu = &pftp->snd_pdu;

	xscpy(pdu->ftime, ftime);
}

void xtftp_get_filetime(xhand_t tftp, tchar_t* ftime)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->type == _XTFTP_TYPE_SRV)
		pdu = &pftp->rcv_pdu;
	else
		pdu = &pftp->snd_pdu;

	xscpy(ftime, pdu->ftime);
}

void xtftp_set_filename(xhand_t tftp, const tchar_t* fname)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	pdu = &pftp->snd_pdu;

	xscpy(pdu->file, fname);
}

void xtftp_get_filename(xhand_t tftp, tchar_t* fname)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->type == _XTFTP_TYPE_SRV)
		pdu = &pftp->rcv_pdu;
	else
		pdu = &pftp->snd_pdu;

	xscpy(fname, pdu->file);
}

bool_t xtftp_connect(xhand_t tftp)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	tftp_pdu_t* pdu;

	TRY_CATCH;

	pftp->serial = 0;

	pdu = &pftp->snd_pdu;

	if (compare_text(pdu->method, -1, TFTP_METHOD_PUT, -1, 1) == 0)
	{
		pdu->type = TFTP_PDU_WRQ;
		if (!_tftp_send_request(pftp))
		{
			raise_user_error(NULL, NULL);
		}

		if (!_tftp_recv_response(pftp))
		{
			raise_user_error(NULL, NULL);
		}

		if (pftp->rcv_pdu.type != TFTP_PDU_ACK)
		{
			raise_user_error(_T("xtftp_connect"), _T("not ack package"));
		}
	}
	else if (compare_text(pdu->method, -1, TFTP_METHOD_GET, -1, 1) == 0)
	{
		pdu->type = TFTP_PDU_RRQ;
		if (!_tftp_send_request(pftp))
		{
			raise_user_error(NULL, NULL);
		}
	}
	else if (compare_text(pdu->method, -1, TFTP_METHOD_HEAD, -1, 1) == 0)
	{
		pdu->type = TFTP_PDU_HEAD;
		if (!_tftp_send_request(pftp))
		{
			raise_user_error(NULL, NULL);
		}
	}
	else if (compare_text(pdu->method, -1, TFTP_METHOD_DELETE, -1, 1) == 0)
	{
		pdu->type = TFTP_PDU_DEL;
		if (!_tftp_send_request(pftp))
		{
			raise_user_error(NULL, NULL);
		}
	}
	else
	{
		raise_user_error(_T("xtftp_connect"), _T("invalid operator"));
	}

	END_CATCH;

	return 1;

ONERROR:
	XDL_TRACE_LAST;

	return 0;
}

bool_t	xtftp_accept(xhand_t tftp)
{
	xtftp_t* pftp = TypePtrFromHead(xtftp_t, tftp);

	tftp_pdu_t* pdu;
	int pdu_type;

	TRY_CATCH;

	pftp->serial = 0;

	if (!_tftp_recv_request(pftp))
	{
		raise_user_error(NULL, NULL);
	}

	pdu_type = pftp->rcv_pdu.type;

	pdu = &pftp->snd_pdu;

	if (pdu_type == TFTP_PDU_WRQ)
	{
		pdu->type = TFTP_PDU_ACK;

		if (!_tftp_send_response(pftp))
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


bool_t xtftp_recv(xhand_t tftp, byte_t* buf, dword_t* pch)
{
	xtftp_t* ppt = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t len, pos = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	TRY_CATCH;

	pdu = &ppt->rcv_pdu;

	pdu->pdv_eof = 0;

	while (pos < *pch)
	{
		len = ((*pch - pos) < (pdu->pdv_len - pdu->pdv_off)) ? (*pch - pos) : (pdu->pdv_len - pdu->pdv_off);
		xmem_copy((void*)(buf + pos), (void*)(pdu->payload + pdu->pdv_off), len);
		pdu->pdv_off += len;
		pos += len;

		if (pos == *pch)
			break;

		if ((pdu->pdv_len == pdu->pdv_off) && !pdu->pdv_eof)
		{
			if (ppt->type == _XTFTP_TYPE_SRV)
			{
				if (!_tftp_recv_request(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (pdu->type != TFTP_PDU_DATA)
				{
					raise_user_error(_T("xtftp_recv"), _T("invalid data package type"));
				}

				ppt->snd_pdu.type = TFTP_PDU_ACK;

				if (!_tftp_send_response(ppt))
				{
					raise_user_error(NULL, NULL);
				}
			}
			else if (ppt->type == _XTFTP_TYPE_CLI)
			{
				if (!_tftp_recv_response(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (pdu->type != TFTP_PDU_DATA)
				{
					raise_user_error(_T("xtftp_recv"), _T("invalid data package type"));
				}

				ppt->snd_pdu.type = TFTP_PDU_ACK;

				if (!_tftp_send_request(ppt))
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

bool_t xtftp_send(xhand_t tftp, const byte_t* buf, dword_t *pch)
{
	xtftp_t* ppt = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;

	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t len, pos = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	pdu->pdv_len = TFTP_PDV_SIZE;

	while (pos < *pch)
	{
		len = ((*pch - pos) < (pdu->pdv_len - pdu->pdv_off)) ? (*pch - pos) : (pdu->pdv_len - pdu->pdv_off);
		xmem_copy((void*)(pdu->payload + pdu->pdv_off), (void*)(buf + pos), len);
		pdu->pdv_off += len;
		pos += len;

		if (pdu->pdv_off == pdu->pdv_len)
		{
			if (ppt->type == _XTFTP_TYPE_SRV)
			{
				pdu->type = TFTP_PDU_DATA;
				if (!_tftp_send_response(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (!_tftp_recv_request(ppt))
				{
					raise_user_error(NULL, NULL);
				}
				if (ppt->rcv_pdu.type != TFTP_PDU_ACK)
				{
					raise_user_error(_T("xtftp_recv"), _T("invalid ack package type"));
				}
			}
			else if (ppt->type == _XTFTP_TYPE_CLI)
			{
				pdu->type = TFTP_PDU_DATA;
				if (!_tftp_send_request(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (!_tftp_recv_response(ppt))
				{
					raise_user_error(NULL, NULL);
				}

				if (ppt->rcv_pdu.type != TFTP_PDU_ACK)
				{
					raise_user_error(_T("xtftp_recv"), _T("invalid ack package type"));
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

bool_t xtftp_flush(xhand_t tftp)
{
	xtftp_t* ppt = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;
	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t pos = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	pdu->pdv_eof = 1;

	if (ppt->type == _XTFTP_TYPE_SRV)
	{
		pdu->type = TFTP_PDU_DATA;
		if (!_tftp_send_response(ppt))
		{
			raise_user_error(NULL, NULL);
		}

		if (!_tftp_recv_request(ppt))
		{
			raise_user_error(NULL, NULL);
		}

		if (ppt->rcv_pdu.type != TFTP_PDU_ACK)
		{
			raise_user_error(_T("xtftp_recv"), _T("invalid ack package type"));
		}
	}
	else if (ppt->type == _XTFTP_TYPE_CLI)
	{
		pdu->type = TFTP_PDU_DATA;
		if (!_tftp_send_request(ppt))
		{
			raise_user_error(NULL, NULL);
		}

		if (!_tftp_recv_response(ppt))
		{
			raise_user_error(NULL, NULL);
		}

		if (ppt->rcv_pdu.type != TFTP_PDU_ACK)
		{
			raise_user_error(_T("xtftp_recv"), _T("invalid ack package type"));
		}
	}

	pdu->pdv_eof = 0;

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

void xtftp_abort(xhand_t tftp, int errcode)
{
	xtftp_t* ppt = TypePtrFromHead(xtftp_t, tftp);

	tftp_pdu_t* pdu;

	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t len;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	pdu->errcode = errcode;

	_tftp_error(pdu->errcode, pdu->errtext);

	_tftp_clear_pdv(pdu);

	pdu->type = TFTP_PDU_ERR;

	len = _tftp_format_pdu(pkg_buf, TFTP_PKG_SIZE, pdu);

	if (!(*ppt->pif->pf_write)(ppt->pif->fd, pkg_buf, &len))
	{
		raise_user_error(NULL, NULL);
	}

	(*ppt->pif->pf_flush)(ppt->pif->fd);

	END_CATCH;

	return;
ONERROR:

	return;
}

bool_t xtftp_head(xhand_t tftp)
{
	xtftp_t* ppt = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;
	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t pos = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	if (ppt->type == _XTFTP_TYPE_SRV)
	{
		pdu->type = TFTP_PDU_HEAD;
		if (!_tftp_send_response(ppt))
		{
			raise_user_error(NULL, NULL);
		}
	}
	else if (ppt->type == _XTFTP_TYPE_CLI)
	{
		if (!_tftp_recv_response(ppt))
		{
			raise_user_error(NULL, NULL);
		}

		if (ppt->rcv_pdu.type != TFTP_PDU_HEAD)
		{
			raise_user_error(_T("xtftp_recv"), _T("invalid ack package type"));
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t xtftp_delete(xhand_t tftp)
{
	xtftp_t* ppt = TypePtrFromHead(xtftp_t, tftp);
	tftp_pdu_t* pdu;
	byte_t pkg_buf[TFTP_PKG_SIZE] = { 0 };
	dword_t pos = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	TRY_CATCH;

	pdu = &ppt->snd_pdu;

	if (ppt->type == _XTFTP_TYPE_SRV)
	{
		pdu->type = TFTP_PDU_ACK;
		if (!_tftp_send_response(ppt))
		{
			raise_user_error(NULL, NULL);
		}
	}
	else if (ppt->type == _XTFTP_TYPE_CLI)
	{
		if (!_tftp_recv_response(ppt))
		{
			raise_user_error(NULL, NULL);
		}

		if (ppt->rcv_pdu.type != TFTP_PDU_ACK)
		{
			raise_user_error(_T("xtftp_recv"), _T("invalid ack package type"));
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

#endif /*XDK_SUPPORT_SOCK*/
