/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl internet document

	@module	internet.c | implement file

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

#include "xdlinet.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdlnet.h"
#include "xdldoc.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)

typedef struct _xinet_t{
	xhand_head head;		//reserved for xhand_t
	
	byte_t proto;
	int fmode, fsince;
	tchar_t path[PATH_LEN];
	tchar_t ftime[DATE_LEN];
	secu_desc_t fsecu;
}xinet_t;

/*************************************************************************************************/
static void _xinet_set_filetime(xhand_t inet, const tchar_t* ftime)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xsncpy(pfn->ftime, ftime, DATE_LEN);
}

static void _xinet_get_filetime(xhand_t inet, tchar_t* ftime)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xsncpy(ftime, pfn->ftime, DATE_LEN);
}

static void _xinet_set_filesince(xhand_t inet, int since)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	pfn->fsince = since;
}

static void _xinet_get_filesince(xhand_t inet, int* psince)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	*psince = pfn->fsince;
}
/*************************************************************************************************/

static void _parse_fileinfo_from_line(file_info_t* pfi, string_t vs)
{
	const tchar_t* token = NULL;
	int pos = 0;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	memset((void*)pfi, 0, sizeof(file_info_t));

	token = string_ptr(vs);
	while (*token != TXT_LINEFEED && *token != _T('\0'))
	{
		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}
		pfi->is_dir = (compare_text(token - pos, pos, DOC_LIST_DIRECTORY, -1, 1) == 0) ? 1 : 0;

		if (*token == TXT_ITEMFEED)
			token++;

		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}
		xsncpy(pfi->file_name, token - pos, pos);

		if (*token == TXT_ITEMFEED)
			token++;

		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}
		xsncpy(sz_date, token - pos, pos);
		parse_gmttime(&pfi->create_time, sz_date);

		if (*token == TXT_ITEMFEED)
			token++;

		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}
		xsncpy(sz_date, token - pos, pos);
		parse_gmttime(&pfi->access_time, sz_date);

		if (*token == TXT_ITEMFEED)
			token++;

		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}
		xsncpy(sz_date, token - pos, pos);
		parse_gmttime(&pfi->write_time, sz_date);

		if (*token == TXT_ITEMFEED)
			token++;

		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}
		xsncpy(sz_size, token - pos, pos);
		parse_long(&pfi->high_size, &pfi->low_size, sz_size);

		if (*token == TXT_ITEMFEED)
			token++;

		pos = 0;
		while (*token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0'))
		{
			token++;
			pos++;
		}
		xsncpy(pfi->file_etag, token - pos, pos);

		if (*token == TXT_ITEMFEED)
			token++;
	}
}

static bool_t _http_read_file(xinet_t* pfn, byte_t* buf, dword_t* pb, const tchar_t* range)
{
	tchar_t code[NUM_LEN + 1] = { 0 };
	tchar_t text[ERR_LEN + 1] = { 0 };

	xhand_t xh = NULL;

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t fcode[INT_LEN + 1] = { 0 };
	tchar_t frange[RES_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };

	dword_t n_zip, n_size = 0;
	byte_t* sz_zip = NULL;
	dword_t hoff, loff;
	long long n_all;

	xh = xhttp_client(_T("GET"), pfn->path);
	if (!xh)
	{
		*pb = 0;
		return 0;
	}

	xhttp_set_request_default_header(xh);

	if (pfn->fsince)
	{
		xhttp_set_request_header(xh, HTTP_HEADER_IFMODIFIEDSINCE, -1, pfn->ftime, DATE_LEN);
	}

	if (!is_null(range))
	{
		xhttp_set_request_header(xh, HTTP_HEADER_RANGE, -1, range, -1);
	}

	xhttp_set_request_header(xh, HTTP_HEADER_ACCEPTENCODING, -1, HTTP_HEADER_CONTENTENCODING_DEFLATE, -1);

	if (!is_null(pfn->fsecu.scr_key))
	{
		xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, pfn->fsecu.scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pfn->fsecu.scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}

	if (!xhttp_send_request(xh))
	{
		*pb = 0;

		xhttp_close(xh);
		return 0;
	}

	if (!xhttp_recv_response(xh))
	{
		*pb = 0;

		xhttp_close(xh);
		return 0;
	}

	if (!xhttp_get_response_state(xh))
	{
		xhttp_recv_error(xh, NULL, NULL, code, text, ERR_LEN);
		set_last_error(code, text, -1);

		*pb = 0;

		xhttp_close(xh);
		return 0;
	}

	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTENCODING, -1, fcode, INT_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTRANGE, -1, frange, RES_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);

	n_size = xstol(fsize);

	if (!is_null(frange))
	{
		parse_bytes_range(frange, &hoff, &loff, &n_zip, &n_all);
	}
	else
	{
		n_zip = n_size;
	}

	n_zip = (n_zip < *pb) ? n_zip : *pb;

	if (compare_text(fcode, -1, HTTP_HEADER_CONTENTENCODING_DEFLATE, -1, 1) == 0)
	{
		sz_zip = (byte_t*)xmem_alloc(n_size);
		if (!xhttp_recv(xh, sz_zip, &n_size))
		{
			xmem_free(sz_zip);
			xhttp_close(xh);

			*pb = 0;
			return 0;
		}
#ifdef GPL_SUPPORT_ZIP
		if (!xzlib_uncompress_bytes(sz_zip, n_size, buf, &n_zip))
		{
			xmem_free(sz_zip);
			xhttp_close(xh);

			*pb = 0;
			return 0;
		}
		xmem_free(sz_zip);
		*pb = n_zip;
#else
		xmem_free(sz_zip);
		xhttp_close(xh);

		*pb = 0;
		set_last_error(_T("-1"), _T("not support deflate"), -1);
		return 0;
#endif
	}
	else if (compare_text(fcode, -1, HTTP_HEADER_CONTENTENCODING_GZIP, -1, 1) == 0)
	{
		sz_zip = (byte_t*)xmem_alloc(n_size);
		if (!xhttp_recv(xh, sz_zip, &n_size))
		{
			xmem_free(sz_zip);
			xhttp_close(xh);

			*pb = 0;
			return 0;
		}
#ifdef GPL_SUPPORT_ZIP
		if (!xgzip_uncompress_bytes(sz_zip, n_size, buf, &n_zip))
		{
			xmem_free(sz_zip);
			xhttp_close(xh);

			*pb = 0;
			return 0;
		}
		xmem_free(sz_zip);
		*pb = n_zip;
#else
		xmem_free(sz_zip);
		xhttp_close(xh);

		*pb = 0;
		set_last_error(_T("-1"), _T("not support gzip"), -1);
		return 0;
#endif
	}
	else
	{
		if (!xhttp_recv(xh, buf, &n_zip))
		{
			xhttp_close(xh);

			*pb = 0;
			return 0;
		}
	}

	xhttp_close(xh);

	*pb = n_zip;
	return 1;
}

static bool_t _http_write_file(xinet_t* pfn, const byte_t* buf, dword_t* pb, const tchar_t* range)
{
	tchar_t code[NUM_LEN + 1] = { 0 };
	tchar_t text[ERR_LEN + 1] = { 0 };

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };

	xhand_t xh = NULL;
	byte_t* sz_zip = NULL;
	dword_t n_zip, n_size = 0;
	bool_t b_rt, b_zip = 0;

	xh = xhttp_client(_T("PUT"), pfn->path);
	if (!xh)
	{
		*pb = 0;
		return 0;
	}

	n_size = *pb;

	xhttp_set_request_default_header(xh);

	if (n_size > XHTTP_ZIPED_SIZE)
	{
		sz_zip = (byte_t*)xmem_alloc(n_size);
		n_zip = n_size;

#ifdef GPL_SUPPORT_ZIP
		b_rt = xzlib_compress_bytes(buf, n_size, sz_zip, &n_zip);
#else
		b_rt = 0;
#endif
		if (!b_rt || ((double)n_zip / (double)n_size > 0.9))
		{
			xmem_free(sz_zip);
			sz_zip = NULL;

			xsprintf(fsize, _T("%d"), n_size);
			xhttp_set_request_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
			b_zip = 0;
		}
		else
		{
			xsprintf(fsize, _T("%d"), n_zip);
			xhttp_set_request_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);

			if (is_null(range))
			{
				format_bytes_range(fsize, 0, 0, n_size, n_size);
				xhttp_set_request_header(xh, HTTP_HEADER_CONTENTRANGE, -1, fsize, -1);
			}
			else
			{
				xhttp_set_request_header(xh, HTTP_HEADER_CONTENTRANGE, -1, range, -1);
			}

			xhttp_set_request_header(xh, HTTP_HEADER_CONTENTENCODING, -1, HTTP_HEADER_CONTENTENCODING_DEFLATE, -1);

			b_zip = 1;
		}
	}
	else
	{
		xsprintf(fsize, _T("%d"), n_size);
		xhttp_set_request_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
		b_zip = 0;
	}

	if (!is_null(pfn->ftime))
	{
		xhttp_set_request_header(xh, HTTP_HEADER_LASTMODIFIED, -1, pfn->ftime, -1);
	}

	if (pfn->fsince)
	{
		xhttp_set_request_header(xh, HTTP_HEADER_IFMODIFIEDSINCE, -1, pfn->ftime, -1);
	}

	if (!is_null(range))
	{
		xhttp_set_request_header(xh, HTTP_HEADER_CONTENTRANGE, -1, range, -1);
	}

	if (!is_null(pfn->fsecu.scr_uid))
	{
		xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, pfn->fsecu.scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, pfn->fsecu.scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}

	if (b_zip)
		b_rt = xhttp_send_full(xh, sz_zip, n_zip);
	else
		b_rt = xhttp_send_full(xh, buf, n_size);

	if (b_zip)
	{
		xmem_free(sz_zip);
		sz_zip = NULL;
	}

	if (!b_rt)
	{
		xhttp_close(xh);

		*pb = 0;
		return 0;
	}

	b_rt = xhttp_recv_response(xh);

	if (!b_rt)
	{
		xhttp_close(xh);

		*pb = 0;
		return 0;
	}

	b_rt = xhttp_get_response_state(xh);

	xhttp_recv_error(xh, NULL, NULL, code, text, ERR_LEN);

	if (!b_rt)
	{
		set_last_error(code, text, -1);
	}

	xhttp_close(xh);

	return b_rt;
}

static xhand_t http_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode)
{
	xinet_t* pfn;

	if (is_null(fname))
		return NULL;

	pfn = (xinet_t*)xmem_alloc(sizeof(xinet_t));
	pfn->head.tag = _HANDLE_INET;
	pfn->proto = _PROTO_HTTP;

	pfn->fmode = mode;
	xsncpy(pfn->path, fname, PATH_LEN);
	if (psd)
	{
		xsncpy(pfn->fsecu.scr_uid, psd->scr_uid, META_LEN);
		xsncpy(pfn->fsecu.scr_key, psd->scr_key, KEY_LEN);
	}

	return &pfn->head;
}

static void http_close_file(xhand_t inet)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xmem_free(pfn);
}

static bool_t http_read_file(xhand_t inet, byte_t* buf, dword_t* pb)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	return _http_read_file(pfn, buf, pb, NULL);
}

static bool_t http_write_file(xhand_t inet, const byte_t* buf, dword_t* pb)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	return _http_write_file(pfn, buf, pb, NULL);
}

static bool_t http_read_file_range(xhand_t inet, dword_t hoff, dword_t loff, byte_t* buf, dword_t size)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	tchar_t sz_range[RES_LEN + 1] = { 0 };
	tchar_t sz_from[NUM_LEN + 1] = { 0 };
	tchar_t sz_to[NUM_LEN + 1] = { 0 };
	long long ll = 0;
	dword_t dw = size;

	format_long(hoff, loff, sz_from);

	ll = MAKELWORD(loff, hoff) + size - 1;
	hoff = GETHDWORD(ll);
	loff = GETLDWORD(ll);

	format_long(hoff, loff, sz_to);

	xsprintf(sz_range, _T("bytes=%s-%s"), sz_from, sz_to);

	return _http_read_file(pfn, buf, &dw, sz_range);
}

static bool_t http_write_file_range(xhand_t inet, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	tchar_t sz_range[RES_LEN + 1] = { 0 };
	tchar_t sz_from[NUM_LEN + 1] = { 0 };
	tchar_t sz_to[NUM_LEN + 1] = { 0 };
	tchar_t sz_total[NUM_LEN + 1] = { 0 };
	long long ll = 0;
	dword_t dw = size;

	if (!size)
	{
		return _http_write_file(pfn, buf, &dw, NULL);
	}
	else
	{
		format_long(hoff, loff, sz_from);

		ll = MAKELWORD(loff, hoff) + size - 1;
		hoff = GETHDWORD(ll);
		loff = GETLDWORD(ll);
		format_long(hoff, loff, sz_to);

		ll += 1;
		hoff = GETHDWORD(ll);
		loff = GETLDWORD(ll);

		format_long(hoff, loff, sz_total);

		xsprintf(sz_range, _T("%s-%s/%s"), sz_from, sz_to, sz_total);

		return _http_write_file(pfn, buf, &dw, sz_range);
	}
}

static bool_t http_delete_file(const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t code[NUM_LEN + 1] = { 0 };
	tchar_t text[ERR_LEN + 1] = { 0 };

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	xhand_t xh = NULL;

	xh = xhttp_client(_T("DELETE"), fname);

	if (!xh)
		return 0;

	xhttp_set_request_default_header(xh);

	if (psd && !is_null(psd->scr_uid))
	{
		xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, psd->scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, psd->scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}

	if (!xhttp_send_request(xh))
	{
		xhttp_close(xh);
		return 0;
	}

	if (!xhttp_recv_response(xh))
	{
		xhttp_close(xh);
		return 0;
	}

	if (!xhttp_get_response_state(xh))
	{
		xhttp_recv_error(xh, NULL, NULL, code, text, ERR_LEN);
		set_last_error(code, text, -1);

		xhttp_close(xh);
		return 0;
	}

	xhttp_close(xh);

	return 1;
}

static bool_t http_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa)
{
	tchar_t code[NUM_LEN + 1] = { 0 };
	tchar_t text[ERR_LEN + 1] = { 0 };

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	xhand_t xh = NULL;
	bool_t rt;
	dword_t dw = 0;

	stream_t stream;
	string_t vs = NULL;
	file_info_t fi = { 0 };

	xh = xhttp_client(_T("LIST"), path);
	if (!xh)
		return 0;

	xhttp_set_request_default_header(xh);
	xhttp_set_request_accept_type(xh, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	if (psd && !is_null(psd->scr_uid))
	{
		xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, psd->scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, psd->scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}

	if (!xhttp_send_request(xh))
	{
		xhttp_close(xh);
		return 0;
	}

	if (!xhttp_recv_response(xh))
	{
		xhttp_close(xh);
		return 0;
	}

	if (!xhttp_get_response_state(xh))
	{
		xhttp_recv_error(xh, NULL, NULL, code, text, ERR_LEN);
		set_last_error(code, text, -1);

		xhttp_close(xh);
		return 0;
	}

	stream = xhttp_get_recv_stream(xh);

	stream_read_utfbom(stream, NULL);

	vs = string_alloc();

	while (1)
	{
		string_empty(vs);
		dw = 0;
		if (!stream_read_line(stream, vs, &dw))
		{
			rt = 0;
			break;
		}

		if (string_len(vs) == 0)
			break;

		xmem_zero((void*)&fi, sizeof(file_info_t));

		_parse_fileinfo_from_line(&fi, vs);

		if (pf)
		{
			(*pf)(&fi, pa);
		}
	}

	string_free(vs);
	vs = NULL;

	xhttp_close(xh);

	return rt;
}

static bool_t http_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
{
	tchar_t code[NUM_LEN + 1] = { 0 };
	tchar_t text[ERR_LEN + 1] = { 0 };

	tchar_t sz_auth[META_LEN + 1] = { 0 };
	tchar_t sz_hmac[HMAC_LEN + 1] = { 0 };

	xhand_t xh = NULL;

	xh = xhttp_client(_T("HEAD"), fname);
	if (!xh)
		return 0;

	xhttp_set_request_default_header(xh);

	if (psd && !is_null(psd->scr_uid))
	{
		xhttp_request_signature(xh, HTTP_HEADER_AUTHORIZATION_XDS, psd->scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("%s %s:%s"), HTTP_HEADER_AUTHORIZATION_XDS, psd->scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}

	if (!xhttp_send_request(xh))
	{
		xhttp_close(xh);
		return 0;
	}

	if (!xhttp_recv_response(xh))
	{
		xhttp_close(xh);
		return 0;
	}

	if (!xhttp_get_response_state(xh))
	{
		xhttp_recv_error(xh, NULL, NULL, code, text, ERR_LEN);
		set_last_error(code, text, -1);

		xhttp_close(xh);
		return 0;
	}

	if (fsize)
		xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);
	if (ftime)
		xhttp_get_response_header(xh, HTTP_HEADER_LASTMODIFIED, -1, ftime, DATE_LEN);
	if (fetag)
		xhttp_get_response_header(xh, HTTP_HEADER_ETAG, -1, fetag, RES_LEN);
	if (fencode)
		xhttp_get_response_content_type_charset(xh, fencode, INT_LEN);

	xhttp_close(xh);

	return 1;
}

/***********************************************************************************************************************************/

static xhand_t tftp_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode)
{
	xinet_t* pfn;

	if (is_null(fname))
		return NULL;

	pfn = (xinet_t*)xmem_alloc(sizeof(xinet_t));
	pfn->head.tag = _HANDLE_INET;

	pfn->proto = _PROTO_TFTP;
	pfn->fmode = mode;
	xsncpy(pfn->path, fname, PATH_LEN);
	if (psd)
	{
		xsncpy(pfn->fsecu.scr_uid, psd->scr_uid, META_LEN);
		xsncpy(pfn->fsecu.scr_key, psd->scr_key, KEY_LEN);
	}

	return &pfn->head;
}

static bool_t tftp_write_file(xhand_t inet, const byte_t* buf, dword_t* pb)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);
	xhand_t xh;
	bool_t rt = 0;
	dword_t bys, size, pos = 0;

	xh = xtftp_client(_T("PUT"), pfn->path);
	if (!xh)
		return 0;

	if (!xtftp_connect(xh))
	{
		xtftp_close(xh);
		return 0;
	}

	size = *pb;
	while (pos < size)
	{
		bys = size - pos;
		rt = xtftp_send(xh, buf + pos, &bys);
		if (!rt)
		{
			break;
		}
		pos += bys;
	}

	if (rt)
	{
		xtftp_flush(xh);
	}

	xtftp_close(xh);

	return rt;
}

static bool_t tftp_read_file(xhand_t inet, byte_t* buf, dword_t* pb)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);
	xhand_t xh;
	bool_t rt;
	dword_t bys, size, pos = 0;

	xh = xtftp_client(_T("GET"), pfn->path);
	if (!xh)
		return 0;

	if (!xtftp_connect(xh))
	{
		xtftp_close(xh);
		return 0;
	}

	size = *pb;
	while (pos < size)
	{
		bys = size - pos;
		rt = xtftp_recv(xh, buf + pos, &bys);
		pos += bys;

		if (!bys)
			break;
	}

	xtftp_close(xh);

	*pb = pos;

	return rt;
}

static void tftp_close_file(xhand_t inet)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xmem_free(pfn);
}

static bool_t tftp_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa)
{
	return 0;
}

static bool_t tftp_delete_file(const secu_desc_t* psd, const tchar_t* fname)
{
	xhand_t xh;

	xh = xtftp_client(_T("DELETE"), fname);
	if (!xh)
		return 0;

	if (!xtftp_connect(xh))
	{
		xtftp_close(xh);
		return 0;
	}

	xtftp_close(xh);

	return 1;
}

static bool_t tftp_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
{
	xhand_t xh;

	xh = xtftp_client(_T("HEAD"), fname);
	if (!xh)
		return 0;

	if (!xtftp_connect(xh))
	{
		xtftp_close(xh);
		return 0;
	}

	if (!xtftp_head(xh))
	{
		xtftp_close(xh);
		return 0;
	}

	if (ftime)
		xtftp_get_filetime(xh, ftime);
	if (fsize)
		ltoxs(xtftp_get_filesize(xh), fsize, INT_LEN);

	xtftp_close(xh);

	return 1;
}

/*************************************************************************************************/

xhand_t xinet_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode)
{
	byte_t proto;

	proto = parse_proto(fname);

	if (proto == _PROTO_HTTP)
		return http_open_file(psd, fname, mode);
	else if (proto == _PROTO_TFTP)
		return tftp_open_file(psd, fname, mode);
	else
		return NULL;
}

void xinet_close_file(xhand_t inet)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		http_close_file(inet);
	else if (pfn->proto == _PROTO_TFTP)
		tftp_close_file(inet);
}

bool_t xinet_read_file(xhand_t inet, byte_t* buf, dword_t* pb)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		return http_read_file(inet, buf, pb);
	else if (pfn->proto == _PROTO_TFTP)
		return tftp_read_file(inet, buf, pb);
	else
		return 0;
}

bool_t xinet_write_file(xhand_t inet, const byte_t* buf, dword_t* pb)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		return http_write_file(inet, buf, pb);
	else if (pfn->proto == _PROTO_TFTP)
		return tftp_write_file(inet, buf, pb);
	else
		return 0;
}

bool_t xinet_read_file_range(xhand_t inet, dword_t hoff, dword_t loff, byte_t* buf, dword_t size)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		return http_read_file_range(inet, hoff, loff, buf, size);
	else
		return 0;
}

bool_t xinet_write_file_range(xhand_t inet, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		return http_write_file_range(inet, hoff, loff, buf, size);
	else
		return 0;
}

void xinet_set_filetime(xhand_t inet, const tchar_t* ftime)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		_xinet_set_filetime(inet, ftime);
}

void xinet_get_filetime(xhand_t inet, tchar_t* ftime)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		_xinet_get_filetime(inet, ftime);
}

void xinet_set_filesince(xhand_t inet, int since)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		_xinet_set_filesince(inet, since);
}

void xinet_get_filesince(xhand_t inet, int* psince)
{
	xinet_t* pfn = TypePtrFromHead(xinet_t, inet);

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		_xinet_get_filesince(inet, psince);
}

bool_t xinet_delete_file(const secu_desc_t* psd, const tchar_t* fname)
{
	byte_t proto;

	proto = parse_proto(fname);

	if (proto == _PROTO_HTTP)
		return http_delete_file(psd, fname);
	else if (proto == _PROTO_TFTP)
		return tftp_delete_file(psd, fname);
	else
		return 0;
}

bool_t xinet_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa)
{
	byte_t proto;

	proto = parse_proto(path);

	if (proto == _PROTO_HTTP)
		return http_list_file(psd, path, pf, pa);
	else if (proto == _PROTO_TFTP)
		return tftp_list_file(psd, path, pf, pa);
	else
		return 0;
}

bool_t xinet_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
{
	byte_t proto;

	proto = parse_proto(fname);

	if (proto == _PROTO_HTTP)
		return http_file_info(psd, fname, ftime, fsize, fetag, fencode);
	else if (proto == _PROTO_TFTP)
		return tftp_file_info(psd, fname, ftime, fsize, fetag, fencode);
	else
		return 0;
}

bool_t xinet_setopt(xhand_t inet, int oid, void* opt, int len)
{
	switch (oid)
	{
	case FILE_OPTION_SINCE:
		xinet_set_filesince(inet, *(int*)(opt));
		return 1;
	case FILE_OPTION_TIME:
		xinet_set_filetime(inet, (tchar_t*)opt);
		return 1;
	}

	return 0;
}

bool_t xinet_getopt(xhand_t inet, int oid, void* opt, int len)
{
	switch (oid)
	{
	case FILE_OPTION_SINCE:
		xinet_get_filesince(inet, (int*)(opt));
		return 1;
	case FILE_OPTION_TIME:
		xinet_get_filetime(inet, (tchar_t*)opt);
		return 1;
	}

	return 0;
}

#endif /*XDK_SUPPORT_SOCK*/
