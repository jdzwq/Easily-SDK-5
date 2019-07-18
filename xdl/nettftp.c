/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl tftp document

	@module	nettftp.c | xdl tftp implement file

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
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_RAND)

typedef struct _xtftp_t{
	xhand_head head;		//head for xhand_t

	int type;
	xhand_t bio;

	havege_state havs;

	int status;
	sword_t block;

	dword_t pdv_limit;
	dword_t pdv_count;

	tchar_t method[INT_LEN];
	tchar_t file[PATH_LEN];
	tchar_t mode[NUM_LEN];

	dword_t size;
	sword_t isdir;
	tchar_t ftime[DATE_LEN];

	sword_t errcode;
	tchar_t errtext[ERR_LEN];
}xtftp_t;

/***********************************************************************************************/

static unsigned short _dynet_port(xtftp_t* pftp)
{
	unsigned short port;

	port = (unsigned short)havege_rand(&pftp->havs);

	while (port < UDP_MIN_PORT)
	{
		port = (unsigned short)havege_rand(&pftp->havs);
	}

	return port;
}

static bool_t _xtftp_read_pdu(xtftp_t* pftp, sword_t* pdu_type)
{
	byte_t buf[TFTP_PKG_SIZE + 1] = { 0 };
	dword_t dw, total = 0;
	int n,len;

	TRY_CATCH;

	pftp->pdv_limit = 0;
	pftp->pdv_count = 0;

	dw = TFTP_HDR_SIZE;
	if (!xudp_read(pftp->bio, buf, &dw))
	{
		raise_user_error(NULL, NULL);
	}
	*pdu_type = GET_SWORD_LIT(buf, total);
	total += dw;

	switch (*pdu_type)
	{
	case TFTP_PDU_HEAD:
		dw = TFTP_PDU_SIZE;
		if (!xudp_read(pftp->bio, buf + total, &dw))
		{
			raise_user_error(NULL, NULL);
		}
		pftp->size = GET_DWORD_LIT(buf, total);
		total += 4;

		pftp->isdir = GET_SWORD_LIT(buf, total);
		total += 2;

		len = a_xslen((schar_t*)(buf + total));
		if (len > TFTP_PKG_SIZE - total)
		{
			raise_user_error(_T("TFTP HEAD"), _T("invalid file head"));
		}
#ifdef _UNICODE
		n = mbs_to_ucs((schar_t*)(buf + total), len, pftp->ftime, DATE_LEN);
#else
		n = mbs_to_mbs((schar_t*)(buf + total), len, pftp->ftime, DATE_LEN);
#endif
		pftp->ftime[n] = _T('\0');
		total += (len + 1);

		len = a_xslen((schar_t*)(buf + total));
		if (len > TFTP_PKG_SIZE - total)
		{
			raise_user_error(_T("TFTP HEAD"), _T("invalid file name"));
		}
#ifdef _UNICODE
		n = mbs_to_ucs((schar_t*)(buf + total), len, pftp->file, PATH_LEN);
#else
		n = mbs_to_mbs((schar_t*)(buf + total), len, pftp->file, PATH_LEN);
#endif
		pftp->file[n] = _T('\0');
		total += (len + 1);

		break;
	case TFTP_PDU_DEL:
		dw = TFTP_PDU_SIZE;
		if (!xudp_read(pftp->bio, buf + total, &dw))
		{
			raise_user_error(NULL, NULL);
		}
		len = a_xslen((schar_t*)(buf + total));
		if (len >= TFTP_PDU_SIZE)
		{
			raise_user_error(_T("TFTP DEL"), _T("invalid file name"));
		}
#ifdef _UNICODE
		n = mbs_to_ucs((schar_t*)(buf + total), len, pftp->file, PATH_LEN);
#else
		n = mbs_to_mbs((schar_t*)(buf + total), len, pftp->file, PATH_LEN);
#endif
		pftp->file[n] = _T('\0');
		total += (len + 1);

		break;
	case TFTP_PDU_RRQ:
		dw = TFTP_PDU_SIZE;
		if (!xudp_read(pftp->bio, buf + total, &dw))
		{
			raise_user_error(NULL, NULL);
		}
		len = a_xslen((schar_t*)(buf + total));
		if (len >= TFTP_PDU_SIZE)
		{
			raise_user_error(_T("TFTP RRQ"), _T("invalid file name"));
		}
#ifdef _UNICODE
		n = mbs_to_ucs((schar_t*)(buf + total), len, pftp->file, PATH_LEN);
#else
		n = mbs_to_mbs((schar_t*)(buf + total), len, pftp->file, PATH_LEN);
#endif
		pftp->file[n] = _T('\0');
		total += (len + 1);

		if (total < TFTP_PDU_SIZE)
		{
			len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
			n = mbs_to_ucs((schar_t*)(buf + total), len, pftp->mode, NUM_LEN);
#else
			n = mbs_to_mbs((schar_t*)(buf + total), len, pftp->mode, NUM_LEN);
#endif
			pftp->mode[n] = _T('\0');
			total += (len + 1);
		}

		break;
	case TFTP_PDU_WRQ:
		dw = TFTP_PDU_SIZE;
		if (!xudp_read(pftp->bio, buf + total, &dw))
		{
			raise_user_error(NULL, NULL);
		}
		len = a_xslen((schar_t*)(buf + total));
		if (len > TFTP_PDU_SIZE)
		{
			raise_user_error(_T("TFTP WRQ"), _T("invalid file name"));
		}
#ifdef _UNICODE
		n = mbs_to_ucs((schar_t*)(buf + total), len, pftp->file, PATH_LEN);
#else
		n = mbs_to_mbs((schar_t*)(buf + total), len, pftp->file, PATH_LEN);
#endif
		pftp->file[n] = _T('\0');
		total += (len + 1);

		if (total < TFTP_PDU_SIZE)
		{
			len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
			n = mbs_to_ucs((schar_t*)(buf + total), len, pftp->mode, NUM_LEN);
#else
			n = mbs_to_mbs((schar_t*)(buf + total), len, pftp->mode, NUM_LEN);
#endif
			pftp->mode[n] = _T('\0');
			total += (len + 1);
		}

		break;
	case TFTP_PDU_DATA:
		dw = 2;
		if (!xudp_read(pftp->bio, buf + total, &dw))
		{
			raise_user_error(NULL, NULL);
		}
		if (pftp->block + 1 != GET_SWORD_LIT(buf, total))
		{
			raise_user_error(_T("TFTP DATA"), _T("invalid block sequence"));
		}
		total += dw;

		pftp->block++;
		pftp->pdv_limit = TFTP_PDV_SIZE;

		break;
	case TFTP_PDU_ACK:
		dw = 2;
		if (!xudp_read(pftp->bio, buf + total, &dw))
		{
			raise_user_error(NULL, NULL);
		}
		if (pftp->block != GET_SWORD_LIT(buf, total))
		{
			raise_user_error(_T("TFTP ACK"), _T("invalid block sequence"));
		}
		total += dw;

		break;
	case TFTP_PDU_ERR:
		dw = TFTP_PDU_SIZE;
		if (!xudp_read(pftp->bio, buf + total, &dw))
		{
			raise_user_error(NULL, NULL);
		}
		pftp->errcode = GET_SWORD_LIT(buf, total);
		total += 2;

		len = a_xslen((schar_t*)(buf + total));
#ifdef _UNICODE
		n = mbs_to_ucs((schar_t*)(buf + total), len, pftp->errtext, ERR_LEN);
#else
		n = mbs_to_mbs((schar_t*)(buf + total), len, pftp->errtext, ERR_LEN);
#endif
		pftp->errtext[n] = _T('\0');
		total += (len + 1);

		break;
	}

	END_CATCH;

	return 1;
ONERROR:

	return 0;
}

static bool_t _xtftp_write_pdu(xtftp_t* pftp, sword_t pdu_type)
{
	byte_t buf[TFTP_PKG_SIZE + 1] = { 0 };
	dword_t dw, total = 0;

	TRY_CATCH;

	pftp->pdv_limit = 0;
	pftp->pdv_count = 0;

	dw = TFTP_HDR_SIZE;
	PUT_SWORD_LIT(buf, total, pdu_type);
	total += dw;

	switch (pdu_type)
	{
	case TFTP_PDU_HEAD:
		PUT_DWORD_LIT(buf, total, pftp->size);
		total += 4;

		PUT_SWORD_LIT(buf, total, pftp->isdir);
		total += 2;

#ifdef _UNICODE
		dw = ucs_to_mbs(pftp->ftime, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#else
		dw = mbs_to_mbs(pftp->ftime, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#endif
		if (dw + total >= TFTP_PKG_SIZE)
		{
			raise_user_error(_T("TFTP HEAD"), _T("invalid file head"));
		}
		total += dw;

		buf[total] = '\0';
		total++;

#ifdef _UNICODE
		dw = ucs_to_mbs(pftp->file, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#else
		dw = mbs_to_mbs(pftp->file, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#endif
		if (dw + total >= TFTP_PKG_SIZE)
		{
			raise_user_error(_T("TFTP HEAD"), _T("invalid file name"));
		}
		total += dw;

		buf[total] = '\0';
		total++;

		if (!xudp_write(pftp->bio, buf, &total))
		{
			raise_user_error(NULL, NULL);
		}
		xudp_flush(pftp->bio);

		break;
	case TFTP_PDU_DEL:
#ifdef _UNICODE
		dw = ucs_to_mbs(pftp->file, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#else
		dw = mbs_to_mbs(pftp->file, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#endif
		if (dw + total >= TFTP_PKG_SIZE)
		{
			raise_user_error(_T("TFTP DEL"), _T("invalid file name"));
		}
		total += dw;

		buf[total] = '\0';
		total++;

		if (!xudp_write(pftp->bio, buf, &total))
		{
			raise_user_error(NULL, NULL);
		}
		xudp_flush(pftp->bio);

		break;
	case TFTP_PDU_RRQ:
#ifdef _UNICODE
		dw = ucs_to_mbs(pftp->file, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#else
		dw = mbs_to_mbs(pftp->file, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#endif
		if (dw + total >= TFTP_PKG_SIZE)
		{
			raise_user_error(_T("TFTP RRQ"), _T("invalid file name"));
		}
		total += dw;
		
		buf[total] = '\0';
		total++;

#ifdef _UNICODE
		dw = ucs_to_mbs(pftp->mode, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#else
		dw = mbs_to_mbs(pftp->mode, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#endif
		if (dw + total >= TFTP_PKG_SIZE)
		{
			raise_user_error(_T("TFTP RRQ"), _T("invalid file mode"));
		}
		total += dw;

		buf[total] = '\0';
		total++;

		if (!xudp_write(pftp->bio, buf, &total))
		{
			raise_user_error(NULL, NULL);
		}
		xudp_flush(pftp->bio);

		break;
	case TFTP_PDU_WRQ:
#ifdef _UNICODE
		dw = ucs_to_mbs(pftp->file, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#else
		dw = mbs_to_mbs(pftp->file, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#endif
		if (dw + total >= TFTP_PKG_SIZE)
		{
			raise_user_error(_T("TFTP RRQ"), _T("invalid file name"));
		}
		total += dw;

		buf[total] = '\0';
		total++;

#ifdef _UNICODE
		dw = ucs_to_mbs(pftp->mode, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#else
		dw = mbs_to_mbs(pftp->mode, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#endif
		if (dw + total >= TFTP_PKG_SIZE)
		{
			raise_user_error(_T("TFTP RRQ"), _T("invalid file mode"));
		}
		total += dw;

		buf[total] = '\0';
		total++;

		if (!xudp_write(pftp->bio, buf, &total))
		{
			raise_user_error(NULL, NULL);
		}
		xudp_flush(pftp->bio);

		break;
	case TFTP_PDU_DATA:
		pftp->block++;

		dw = 2;
		PUT_SWORD_LIT(buf, total, pftp->block);
		total += dw;

		if (!xudp_write(pftp->bio, buf, &total))
		{
			raise_user_error(NULL, NULL);
		}

		pftp->pdv_limit = TFTP_PDV_SIZE;

		break;
	case TFTP_PDU_ACK:
		dw = 2;
		PUT_SWORD_LIT(buf, total, pftp->block);
		total += dw;

		if (!xudp_write(pftp->bio, buf, &total))
		{
			raise_user_error(NULL, NULL);
		}
		xudp_flush(pftp->bio);

		break;
	case TFTP_PDU_ERR:
		dw = 2;
		PUT_SWORD_LIT(buf, total, pftp->errcode);
		total += dw;

#ifdef _UNICODE
		dw = ucs_to_mbs(pftp->errtext, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#else
		dw = mbs_to_mbs(pftp->errtext, -1, (schar_t*)(buf + total), TFTP_PKG_SIZE - total);
#endif
		if (dw + total >= TFTP_PKG_SIZE)
		{
			raise_user_error(_T("TFTP RRQ"), _T("invalid file name"));
		}
		total += dw;

		buf[total] = '\0';
		total++;

		if (!xudp_write(pftp->bio, buf, &total))
		{
			raise_user_error(NULL, NULL);
		}
		xudp_flush(pftp->bio);
		break;
	}

	END_CATCH;

	return 1;
ONERROR:

	return 0;
}

static bool_t _xtftp_read_pdv(xtftp_t* pftp, byte_t* buf, dword_t* pch)
{
	dword_t org = *pch;

	*pch = (*pch < pftp->pdv_limit - pftp->pdv_count) ? *pch : (pftp->pdv_limit - pftp->pdv_count);
	if (!xudp_read(pftp->bio, buf, pch))
	{
		return 0;
	}
	pftp->pdv_count += *pch;

	if ((*pch) < org)
	{
		pftp->pdv_limit = pftp->pdv_count;
	}

	return 1;
}

static bool_t _xtftp_write_pdv(xtftp_t* pftp, const byte_t* buf, dword_t *pch)
{
	*pch = (*pch < pftp->pdv_limit - pftp->pdv_count) ? *pch : (pftp->pdv_limit - pftp->pdv_count);
	if (!xudp_write(pftp->bio, buf, pch))
	{
		return 0;
	}
	pftp->pdv_count += *pch;

	return 1;
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

	sword_t pdu_type = 0;

	TRY_CATCH;

	pftp = (xtftp_t*)xmem_alloc(sizeof(xtftp_t));
	pftp->head.tag = _HANDLE_TFTP;

	havege_init(&pftp->havs);
	bind = _dynet_port(pftp);

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

	pftp->bio = xudp_cli(port, addr);
	if (!pftp->bio)
	{
		raise_user_error(NULL, NULL);
	}

	if (!xudp_bind(pftp->bio, bind))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_set_package(pftp->bio, TFTP_PKG_SIZE);

	pftp->block = 0;

	xscpy(pftp->method, method);
	xsncpy(pftp->file, objat, objlen);
	xscpy(pftp->mode, _T("octet"));

#if defined(DEBUG) || defined(_DEBUG)
	xdl_trace(pftp->method, pftp->file);
#endif

	if (compare_text(pftp->method, -1, TFTP_METHOD_PUT, -1, 1) == 0)
	{
#if defined(DEBUG) || defined(_DEBUG)
		xdl_trace(_T("WRQ"), _T("==>"));
#endif
		if (!_xtftp_write_pdu(pftp, TFTP_PDU_WRQ))
		{
			raise_user_error(NULL, NULL);
		}
#if defined(DEBUG) || defined(_DEBUG)
		xdl_trace(_T("ACK"), _T("<=="));
#endif
		if (!_xtftp_read_pdu(pftp, &pdu_type))
		{
			raise_user_error(NULL, NULL);
		}
		if (pdu_type == TFTP_PDU_ERR)
		{
			raise_user_error(_T("tftp_read"), pftp->errtext);
		}
		if (pdu_type != TFTP_PDU_ACK)
		{
			raise_user_error(_T("tftp_read"), _T("invalid operator"));
		}
	}
	else if (compare_text(pftp->method, -1, TFTP_METHOD_GET, -1, 1) == 0)
	{
#if defined(DEBUG) || defined(_DEBUG)
		xdl_trace(_T("RRQ"), _T("==>"));
#endif
		if (!_xtftp_write_pdu(pftp, TFTP_PDU_RRQ))
		{
			raise_user_error(NULL, NULL);
		}
	}
	else if (compare_text(pftp->method, -1, TFTP_METHOD_HEAD, -1, 1) == 0)
	{
#if defined(DEBUG) || defined(_DEBUG)
		xdl_trace(_T("HEAD"), _T("==>"));
#endif
		if (!_xtftp_write_pdu(pftp, TFTP_PDU_HEAD))
		{
			raise_user_error(NULL, NULL);
		}
	}
	else if (compare_text(pftp->method, -1, TFTP_METHOD_DELETE, -1, 1) == 0)
	{
#if defined(DEBUG) || defined(_DEBUG)
		xdl_trace(_T("DELETE"), _T("==>"));
#endif
		if (!_xtftp_write_pdu(pftp, TFTP_PDU_DEL))
		{
			raise_user_error(NULL, NULL);
		}
#if defined(DEBUG) || defined(_DEBUG)
		xdl_trace(_T("ERR"), _T("<=="));
#endif
		if (!_xtftp_read_pdu(pftp, &pdu_type))
		{
			raise_user_error(NULL, NULL);
		}
		if (pftp->errcode)
		{
			raise_user_error(_T("tftp_delete"), pftp->errtext);
		}
	}
	else
	{
		raise_user_error(_T("tftp_client"), _T("invalid operator"));
	}

	if (compare_text(pftp->method, -1, TFTP_METHOD_HEAD, -1, 1) == 0)
		pftp->status = _TFTP_STATUS_NOTIFY;
	else if (compare_text(pftp->method, -1, TFTP_METHOD_DELETE, -1, 1) == 0)
		pftp->status = _TFTP_STATUS_RELEASE;
	else
		pftp->status = _TFTP_STATUS_ASSOCIATE;

	END_CATCH;

	return (xhand_t)pftp;

ONERROR:

	if (pftp)
	{
		if (pftp->bio)
			xudp_close(pftp->bio);

		xmem_free(pftp);
	}

	XDL_TRACE_LAST;

	return NULL;
}

xhand_t	xtftp_server(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size)
{
	xtftp_t* pftp = NULL;
	sword_t pdu_type;
	unsigned short bind;

	TRY_CATCH;

	pftp = (xtftp_t*)xmem_alloc(sizeof(xtftp_t));
	pftp->head.tag = _HANDLE_TFTP;

	pftp->type = _XTFTP_TYPE_SRV;

	havege_init(&pftp->havs);
	bind = _dynet_port(pftp);

	pftp->bio = xudp_srv(port, addr, pack, size);
	if (!pftp->bio)
	{
		raise_user_error(NULL, NULL);
	}

	if (!xudp_bind(pftp->bio, bind))
	{
		raise_user_error(NULL, NULL);
	}

	xudp_set_package(pftp->bio, TFTP_PKG_SIZE);

	pftp->block = 0;

	if (!_xtftp_read_pdu(pftp, &pdu_type))
	{
		raise_user_error(NULL, NULL);
	}

	if (pdu_type == TFTP_PDU_WRQ)
		xscpy(pftp->method, TFTP_METHOD_PUT);
	else if (pdu_type == TFTP_PDU_RRQ)
		xscpy(pftp->method, TFTP_METHOD_GET);
	else if (pdu_type == TFTP_PDU_HEAD)
		xscpy(pftp->method, TFTP_METHOD_HEAD);
	else if (pdu_type == TFTP_PDU_DEL)
		xscpy(pftp->method, TFTP_METHOD_DELETE);
	else
	{
		raise_user_error(_T("tftp_server"), _T("invalid operator"));
	}

#if defined(DEBUG) || defined(_DEBUG)
	xdl_trace(pftp->method, pftp->file);
#endif

	if (compare_text(pftp->method,-1,TFTP_METHOD_HEAD,-1,0) == 0)
		pftp->status = _TFTP_STATUS_NOTIFY;
	else if (compare_text(pftp->method, -1, TFTP_METHOD_DELETE, -1, 0) == 0)
		pftp->status = _TFTP_STATUS_RELEASE;
	else
		pftp->status = _TFTP_STATUS_ASSOCIATE;

	END_CATCH;

	return (xhand_t)pftp;
ONERROR:

	if (pftp)
	{
		if (pftp->bio)
			xudp_close(pftp->bio);

		xmem_free(pftp);
	}

	XDL_TRACE_LAST;

	return NULL;
}

void xtftp_close(xhand_t tftp)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->bio)
		xudp_close(pftp->bio);

	xmem_free(pftp);
}

bool_t xtftp_recv(xhand_t tftp, byte_t* buf, dword_t* pch)
{
	xtftp_t* pftp = (xtftp_t*)tftp;
	sword_t pdu_type = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	TRY_CATCH;

	if (pftp->type == _XTFTP_TYPE_SRV)
	{
		switch (pftp->status)
		{
		case _TFTP_STATUS_ASSOCIATE:
#if defined(DEBUG) || defined(_DEBUG)
			xdl_trace(_T("ACK"), _T("==>"));
#endif
			if (!_xtftp_write_pdu(pftp, TFTP_PDU_ACK))
			{
				raise_user_error(NULL, NULL);
			}

			pftp->status = _TFTP_STATUS_WAITING;
		case _TFTP_STATUS_WAITING:
#if defined(DEBUG) || defined(_DEBUG)
			xdl_trace(_T("DATA"), _T("<=="));
#endif
			if (!_xtftp_read_pdu(pftp, &pdu_type))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == TFTP_PDU_ERR)
			{
				raise_user_error(_T("tftp_read"), pftp->errtext);
			}
			if (pdu_type != TFTP_PDU_DATA)
			{
				raise_user_error(_T("tftp_read"), _T("invalid operator"));
			}

			pftp->status = _TFTP_STATUS_PENDING;
		case _TFTP_STATUS_PENDING:
			if (!_xtftp_read_pdv(pftp, buf, pch))
			{
				raise_user_error(NULL, NULL);
			}

			if (pftp->pdv_count == pftp->pdv_limit)
			{
#if defined(DEBUG) || defined(_DEBUG)
				xdl_trace(_T("ACK"), _T("==>"));
#endif
				if (!_xtftp_write_pdu(pftp, TFTP_PDU_ACK))
				{
					raise_user_error(NULL, NULL);
				}
				pftp->status = (pftp->pdv_count < TFTP_PDV_SIZE) ? _TFTP_STATUS_WAITING : _TFTP_STATUS_EOF;
			}
			break;
		case _TFTP_STATUS_EOF:
			*pch = 0;
			pftp->status = _TFTP_STATUS_RELEASE;
			break;
		case _TFTP_STATUS_RELEASE:
			raise_user_error(_T("tftp_read"), _T("tftp released"));
			break;
		}
	}
	else if (pftp->type == _XTFTP_TYPE_CLI)
	{
		switch (pftp->status)
		{
		case _TFTP_STATUS_ASSOCIATE:

			pftp->status = _TFTP_STATUS_WAITING;
		case _TFTP_STATUS_WAITING:
#if defined(DEBUG) || defined(_DEBUG)
			xdl_trace(_T("DATA"), _T("<=="));
#endif
			if (!_xtftp_read_pdu(pftp, &pdu_type))
			{
				raise_user_error(NULL, NULL);
			}
			if (pdu_type == TFTP_PDU_ERR)
			{
				raise_user_error(_T("tftp_read"), pftp->errtext);
			}
			if (pdu_type != TFTP_PDU_DATA)
			{
				raise_user_error(_T("tftp_read"), _T("invalid operator"));
			}

			pftp->status = _TFTP_STATUS_PENDING;
		case _TFTP_STATUS_PENDING:
			if (!_xtftp_read_pdv(pftp, buf, pch))
			{
				raise_user_error(NULL, NULL);
			}

			if (pftp->pdv_count == pftp->pdv_limit)
			{
#if defined(DEBUG) || defined(_DEBUG)
				xdl_trace(_T("ACK"), _T("==>"));
#endif
				if (!_xtftp_write_pdu(pftp, TFTP_PDU_ACK))
				{
					raise_user_error(NULL, NULL);
				}
				pftp->status = (pftp->pdv_count < TFTP_PDV_SIZE) ? _TFTP_STATUS_WAITING : _TFTP_STATUS_EOF;
			}
			break;
		case _TFTP_STATUS_EOF:
			*pch = 0;
			pftp->status = _TFTP_STATUS_RELEASE;
			break;
		case _TFTP_STATUS_RELEASE:
			raise_user_error(_T("tftp_read"), _T("tftp released"));
			break;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	pftp->status = _TFTP_STATUS_RELEASE;

	return 0;
}

bool_t xtftp_send(xhand_t tftp, const byte_t* buf, dword_t *pch)
{
	xtftp_t* pftp = (xtftp_t*)tftp;
	sword_t pdu_type;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	TRY_CATCH;

	if (pftp->type == _XTFTP_TYPE_SRV)
	{
		switch (pftp->status)
		{
		case _TFTP_STATUS_ASSOCIATE:

			pftp->status = _TFTP_STATUS_WAITING;
		case _TFTP_STATUS_WAITING:
#if defined(DEBUG) || defined(_DEBUG)
			xdl_trace(_T("DATA"), _T("==>"));
#endif
			if (!_xtftp_write_pdu(pftp, TFTP_PDU_DATA))
			{
				raise_user_error(NULL, NULL);
			}
			
			pftp->status = _TFTP_STATUS_PENDING;
		case _TFTP_STATUS_PENDING:
			if (!_xtftp_write_pdv(pftp, buf, pch))
			{
				raise_user_error(NULL, NULL);
			}
			if (pftp->pdv_count == pftp->pdv_limit)
			{
#if defined(DEBUG) || defined(_DEBUG)
				xdl_trace(_T("ACK"), _T("<=="));
#endif
				if (!_xtftp_read_pdu(pftp, &pdu_type))
				{
					raise_user_error(NULL, NULL);
				}
				if (pdu_type == TFTP_PDU_ERR)
				{
					raise_user_error(_T("tftp_read"), pftp->errtext);
				}
				if (pdu_type != TFTP_PDU_ACK)
				{
					raise_user_error(_T("tftp_read"), _T("invalid operator"));
				}
				pftp->status = _TFTP_STATUS_WAITING;
			}
			break;
		case _TFTP_STATUS_EOF:
			*pch = 0;
			pftp->status = _TFTP_STATUS_RELEASE;
			break;
		case _TFTP_STATUS_RELEASE:
			raise_user_error(_T("tftp_read"), _T("tftp released"));
			break;
		}
	}
	else if (pftp->type == _XTFTP_TYPE_CLI)
	{
		switch (pftp->status)
		{
		case _TFTP_STATUS_ASSOCIATE:

			pftp->status = _TFTP_STATUS_WAITING;
		case _TFTP_STATUS_WAITING:
#if defined(DEBUG) || defined(_DEBUG)
			xdl_trace(_T("DATA"), _T("==>"));
#endif
			if (!_xtftp_write_pdu(pftp, TFTP_PDU_DATA))
			{
				raise_user_error(NULL, NULL);
			}

			pftp->status = _TFTP_STATUS_PENDING;
		case _TFTP_STATUS_PENDING:
			if (!_xtftp_write_pdv(pftp, buf, pch))
			{
				raise_user_error(NULL, NULL);
			}

			if (pftp->pdv_count == pftp->pdv_limit)
			{
#if defined(DEBUG) || defined(_DEBUG)
				xdl_trace(_T("ACK"), _T("<=="));
#endif
				if (!_xtftp_read_pdu(pftp, &pdu_type))
				{
					raise_user_error(NULL, NULL);
				}
				if (pdu_type == TFTP_PDU_ERR)
				{
					raise_user_error(_T("tftp_read"), pftp->errtext);
				}
				if (pdu_type != TFTP_PDU_ACK)
				{
					raise_user_error(_T("tftp_read"), _T("invalid operator"));
				}
				pftp->status = _TFTP_STATUS_WAITING;
			}
			break;
		case _TFTP_STATUS_EOF:
			*pch = 0;
			pftp->status = _TFTP_STATUS_RELEASE;
			break;
		case _TFTP_STATUS_RELEASE:
			raise_user_error(_T("tftp_read"), _T("tftp released"));
			break;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	pftp->status = _TFTP_STATUS_RELEASE;

	return 0;
}

bool_t xtftp_flush(xhand_t tftp)
{
	xtftp_t* pftp = (xtftp_t*)tftp;
	sword_t pdu_type = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->status == _TFTP_STATUS_PENDING)
	{
		if (!xudp_flush(pftp->bio))
			return 0;

#if defined(DEBUG) || defined(_DEBUG)
		xdl_trace(_T("ACK"), _T("<=="));
#endif
		_xtftp_read_pdu(pftp, &pdu_type);
	}

	pftp->pdv_limit = pftp->pdv_count;
	pftp->status = _TFTP_STATUS_EOF;

	return 1;
}

bool_t xtftp_head(xhand_t tftp)
{
	xtftp_t* pftp = (xtftp_t*)tftp;
	sword_t pdu_type = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->status != _TFTP_STATUS_NOTIFY)
		return 0;

	if (!_xtftp_write_pdu(pftp, TFTP_PDU_HEAD))
	{
		return 0;
	}

	if (!_xtftp_read_pdu(pftp, &pdu_type))
	{
		return 0;
	}

	if (pdu_type == TFTP_PDU_ERR)
	{
		set_last_error(_T("tftp_head"), pftp->errtext, -1);
		return 0;
	}

	if (pdu_type != TFTP_PDU_ACK)
	{
		set_last_error(_T("tftp_head"), _T("invalid operator"), -1);
		return 0;
	}

	return 1;
}

bool_t xtftp_fetch(xhand_t tftp)
{
	xtftp_t* pftp = (xtftp_t*)tftp;
	sword_t pdu_type = 0;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	if (pftp->status != _TFTP_STATUS_NOTIFY)
		return 0;

	if (!_xtftp_read_pdu(pftp, &pdu_type))
	{
		return 0;
	}

	if (pdu_type == TFTP_PDU_ERR)
	{
		set_last_error(_T("tftp_fetch"), pftp->errtext, -1);
		return 0;
	}

	if (pdu_type != TFTP_PDU_HEAD)
	{
		set_last_error(_T("tftp_fetch"), _T("invalid operator"), -1);
		return 0;
	}

	if (!_xtftp_write_pdu(pftp, TFTP_PDU_ACK))
	{
		return 0;
	}

	return 1;
}

void xtftp_abort(xhand_t tftp, int errcode)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	pftp->errcode = errcode;

	switch (errcode)
	{
	case TFTP_CODE_NOTDEF:
		xscpy(pftp->errtext, TFTP_CODE_NOTDEF_TEXT);
		break;
	case TFTP_CODE_NOTFIND:
		xscpy(pftp->errtext, TFTP_CODE_NOTFIND_TEXT);
		break;
	case TFTP_CODE_REJECT:
		xscpy(pftp->errtext, TFTP_CODE_REJECT_TEXT);
		break;
	case TFTP_CODE_DSKFULL:
		xscpy(pftp->errtext, TFTP_CODE_DSKFULL_TEXT);
		break;
	case TFTP_CODE_UNTID:
		xscpy(pftp->errtext, TFTP_CODE_UNTID_TEXT);
		break;
	case TFTP_CODE_EXISTS:
		xscpy(pftp->errtext, TFTP_CODE_EXISTS_TEXT);
		break;
	case TFTP_CODE_NOUSER:
		xscpy(pftp->errtext, TFTP_CODE_NOUSER_TEXT);
		break;
	}
#if defined(DEBUG) || defined(_DEBUG)
	xdl_trace(_T("ERR"), _T("==>"));
#endif
	_xtftp_write_pdu(pftp, TFTP_PDU_ERR);
}

int xtftp_object(xhand_t tftp, tchar_t* buf, int max)
{
	xtftp_t* pftp = (xtftp_t*)tftp;
	int len;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	len = xslen(pftp->file);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, pftp->file, len);
	}

	return len;
}

int xtftp_method(xhand_t tftp, tchar_t* buf, int max)
{
	xtftp_t* pftp = (xtftp_t*)tftp;
	int len;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	len = xslen(pftp->method);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, pftp->method, len);
	}

	return len;
}

void xtftp_set_isdir(xhand_t tftp, bool_t isdir)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	pftp->isdir = isdir;
}

bool_t xtftp_get_isdir(xhand_t tftp)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	return (bool_t)pftp->isdir;
}

void xtftp_set_filesize(xhand_t tftp, dword_t size)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	pftp->size = size;
}

dword_t xtftp_get_filesize(xhand_t tftp)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	return pftp->size;
}

void xtftp_set_filetime(xhand_t tftp, const tchar_t* ftime)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	xscpy(pftp->ftime, ftime);
}

void xtftp_get_filetime(xhand_t tftp, tchar_t* ftime)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	xscpy(ftime, pftp->ftime);
}

void xtftp_set_filename(xhand_t tftp, const tchar_t* fname)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	xscpy(pftp->file, fname);
}

void xtftp_get_filename(xhand_t tftp, tchar_t* fname)
{
	xtftp_t* pftp = (xtftp_t*)tftp;

	XDL_ASSERT(tftp && tftp->tag == _HANDLE_TFTP);

	xscpy(fname, pftp->file);
}

#endif /*XDK_SUPPORT_SOCK*/
