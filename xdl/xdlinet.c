/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl inet document

	@module	xdlinet.c | xdl inet implement file

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
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdldoc.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)

typedef struct _inet_t{
	xhand_head head;		//reserved for xhand_t
	
	int proto;
	int fmode, fsince;
	tchar_t path[PATH_LEN];
	tchar_t ftime[DATE_LEN];
	secu_desc_t fsecu;
}inet_t;

/*************************************************************************************************/
static void _inet_set_filetime(xhand_t inet, const tchar_t* ftime)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xsncpy(pfn->ftime, ftime, DATE_LEN);
}

static void _inet_set_filesince(xhand_t inet, int since)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	pfn->fsince = since;
}
/*************************************************************************************************/

static void _parse_fileinfo_from_line(file_info_t* pfi, string_t vs)
{
	const tchar_t* token = NULL;
	int pos = 0;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	memset((void*)pfi, 0, sizeof(file_info_t));

	token = varstr_ptr(vs);
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
		parse_longlong(&pfi->high_size, &pfi->low_size, sz_size);

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

static bool_t _http_read_file(inet_t* pfn, byte_t* buf, dword_t* pb, const tchar_t* range)
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

	xhttp_set_request_header(xh, HTTP_HEADER_ACCEPTENCODING, -1, HTTP_HEADER_ENCODING_DEFLATE, -1);

#ifdef XDL_SUPPORT_CRYPT
	if (!is_null(pfn->fsecu.scr_key))
	{
		xhttp_request_signature(xh, pfn->fsecu.scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("XDS %s:%s"), pfn->fsecu.scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}
#endif

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
		parse_long_range(frange, &hoff, &loff, &n_zip, &n_all);
	}
	else
	{
		n_zip = n_size;
	}

	n_zip = (n_zip < *pb) ? n_zip : *pb;

	if (compare_text(fcode, -1, HTTP_HEADER_ENCODING_DEFLATE, -1, 1) == 0)
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
	else if (compare_text(fcode, -1, HTTP_HEADER_ENCODING_GZIP, -1, 1) == 0)
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

static bool_t _http_write_file(inet_t* pfn, const byte_t* buf, dword_t* pb, const tchar_t* range)
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
				format_long_range(fsize, 0, 0, n_size, n_size);
				xhttp_set_request_header(xh, HTTP_HEADER_CONTENTRANGE, -1, fsize, -1);
			}
			else
			{
				xhttp_set_request_header(xh, HTTP_HEADER_CONTENTRANGE, -1, range, -1);
			}

			xhttp_set_request_header(xh, HTTP_HEADER_CONTENTENCODING, -1, HTTP_HEADER_ENCODING_DEFLATE, -1);

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

#ifdef XDL_SUPPORT_CRYPT
	if (!is_null(pfn->fsecu.scr_uid))
	{
		xhttp_request_signature(xh, pfn->fsecu.scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("XDS %s:%s"), pfn->fsecu.scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}
#endif

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
	inet_t* pfn;

	if (is_null(fname))
		return NULL;

	pfn = (inet_t*)xmem_alloc(sizeof(inet_t));
	pfn->head.tag = _HANDLE_INET;
	pfn->proto = _PROTO_HTTP;

	pfn->fmode = mode;
	xsncpy(pfn->path, fname, PATH_LEN);
	if (psd)
	{
		xsncpy(pfn->fsecu.scr_uid, psd->scr_uid, META_LEN);
		xsncpy(pfn->fsecu.scr_key, psd->scr_key, KEY_LEN);
	}

	return (xhand_t)pfn;
}

static void http_close_file(xhand_t inet)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xmem_free(pfn);
}

static bool_t http_read_file(xhand_t inet, byte_t* buf, dword_t* pb)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	return _http_read_file(pfn, buf, pb, NULL);
}

static bool_t http_write_file(xhand_t inet, const byte_t* buf, dword_t* pb)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	return _http_write_file(pfn, buf, pb, NULL);
}

static bool_t http_read_file_range(xhand_t inet, dword_t hoff, dword_t loff, byte_t* buf, dword_t size)
{
	inet_t* pfn = (inet_t*)inet;

	tchar_t sz_range[RES_LEN + 1] = { 0 };
	tchar_t sz_from[NUM_LEN + 1] = { 0 };
	tchar_t sz_to[NUM_LEN + 1] = { 0 };
	long long ll = 0;
	dword_t dw = size;

	format_longlong(hoff, loff, sz_from);

	ll = MAKELONGLONG(loff, hoff) + size - 1;
	hoff = GETHDWORD(ll);
	loff = GETLDWORD(ll);

	format_longlong(hoff, loff, sz_to);

	xsprintf(sz_range, _T("bytes=%s-%s"), sz_from, sz_to);

	return _http_read_file(pfn, buf, &dw, sz_range);
}

static bool_t http_write_file_range(xhand_t inet, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size)
{
	inet_t* pfn = (inet_t*)inet;

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
		format_longlong(hoff, loff, sz_from);

		ll = MAKELONGLONG(loff, hoff) + size - 1;
		hoff = GETHDWORD(ll);
		loff = GETLDWORD(ll);
		format_longlong(hoff, loff, sz_to);

		ll += 1;
		hoff = GETHDWORD(ll);
		loff = GETLDWORD(ll);

		format_longlong(hoff, loff, sz_total);

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

#ifdef XDL_SUPPORT_CRYPT
	if (psd && !is_null(psd->scr_uid))
	{
		xhttp_request_signature(xh, psd->scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("XDS %s:%s"), psd->scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}
#endif

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
	xhttp_set_request_accept_type(xh, HTTP_HEADER_TYPE_APPXML, -1);

#ifdef XDL_SUPPORT_CRYPT
	if (psd && !is_null(psd->scr_uid))
	{
		xhttp_request_signature(xh, psd->scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("XDS %s:%s"), psd->scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}
#endif

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

	vs = varstr_alloc();

	while (1)
	{
		dw = 0;
		if (!stream_read_line(stream, vs, &dw))
		{
			rt = 0;
			break;
		}

		if (!dw)
			break;

		xmem_zero((void*)&fi, sizeof(file_info_t));

		_parse_fileinfo_from_line(&fi, vs);

		if (pf)
		{
			(*pf)(&fi, pa);
		}

		varstr_empty(vs);
	}

	varstr_free(vs);
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

#ifdef XDL_SUPPORT_CRYPT
	if (psd && !is_null(psd->scr_uid))
	{
		xhttp_request_signature(xh, psd->scr_key, sz_hmac, HMAC_LEN);
		xsprintf(sz_auth, _T("XDS %s:%s"), psd->scr_uid, sz_hmac);
		xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
	}
#endif

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

/******************************************************************************************************/

static void _split_oss_url(const tchar_t* obj, tchar_t** addrat, int* addrlen, tchar_t** buckat, int* bucklen, tchar_t** pathat, int* pathlen, tchar_t** fileat, int* filelen)
{
	tchar_t* token = (tchar_t*)obj;
	int len;

	*addrat = *buckat = *pathat = *fileat = NULL;
	*addrlen = *bucklen = *pathlen = *filelen = 0;

	while (*token != _T(':') && *token != _T('\0'))
		token++;

	if (*token == _T(':'))
		token++;

	while (*token == _T('/'))
		token++;

	len = 0;
	*addrat = token;
	while (*token != _T('/') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	*addrlen = len;

	if (*token == _T('/'))
		token++;

	len = 0;
	*buckat = token;
	while (*token != _T('/') && *token != _T('\0'))
	{
		token++;
		len++;
	}
	*bucklen = len;

	if (*token == _T('/'))
		token++;

	len = 0;
	*pathat = token;
	while (*token != _T('\0'))
	{
		token++;
		len++;
	}
	while (*token != _T('/') && token != *pathat)
	{
		token--;
		len--;
	}
	*pathlen = len;

	if (*token == _T('/'))
		token++;

	len = 0;
	*fileat = token;
	while (*token != _T('\0'))
	{
		token++;
		len++;
	}
	*filelen = len;
}

//<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Error>\n  <Code>InvalidBucketName</Code>\n  <Message>The specified bucket is not valid.</Message>\n  <BucketName>myfile.txt</BucketName>\n  <RequestId>547B4B4F44ABF...	

static bool_t _parse_oss_error(byte_t* buf, dword_t size, tchar_t* errcode, tchar_t* errtext)
{
	link_t_ptr root, nlk, xml = NULL;

	xml = create_xml_doc();

	if (!parse_xml_doc_from_bytes(xml, buf, size))
	{
		destroy_xml_doc(xml);

		get_last_error(errcode, errtext, ERR_LEN);
		return 0;
	}

	root = get_xml_dom_node(xml);

	nlk = get_dom_first_child_node(root);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Code"), -1, 1) == 0)
		{
			get_dom_node_text(nlk, errcode, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Message"), -1, 1) == 0)
		{
			get_dom_node_text(nlk, errtext, ERR_LEN);
		}

		nlk = get_dom_next_sibling_node(nlk);
	}

	destroy_xml_doc(xml);

	return 1;
}

static bool_t _parse_oss_result(byte_t* buf, int size, CALLBACK_LISTFILE pf, void* pa)
{
	link_t_ptr root, nlk, clk, xml = NULL;
	file_info_t fi = { 0 };

	xml = create_xml_doc();

	if (!parse_xml_doc_from_bytes(xml, buf, size))
	{
		destroy_xml_doc(xml);
		return 0;
	}

	root = get_xml_dom_node(xml);

	nlk = get_dom_first_child_node(root);
	while (nlk)
	{
		xmem_zero((void*)&fi, sizeof(file_info_t));

		if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Contents"), -1, 1) == 0)
		{
			clk = get_dom_first_child_node(nlk);
			while (clk)
			{
				if (compare_text(get_dom_node_name_ptr(clk), -1, _T("Key"), -1, 1) == 0)
				{
					xsncpy(fi.file_name, get_dom_node_text_ptr(clk), META_LEN);
				}
				else if (compare_text(get_dom_node_name_ptr(clk), -1, _T("LastModified"), -1, 1) == 0)
				{
					parse_gmttime(&fi.write_time, get_dom_node_text_ptr(clk));
				}
				else if (compare_text(get_dom_node_name_ptr(clk), -1, _T("ETag"), -1, 1) == 0)
				{
					xsncpy(fi.file_etag, get_dom_node_text_ptr(clk), ETAG_LEN);
				}
				else if (compare_text(get_dom_node_name_ptr(clk), -1, _T("Size"), -1, 1) == 0)
				{
					parse_longlong(&fi.high_size, &fi.low_size, get_dom_node_text_ptr(clk));
				}

				clk = get_dom_next_sibling_node(clk);
			}

			fi.is_dir = 0;
			if (pf)
			{
				(*pf)(&fi, pa);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("CommonPrefixes"), -1, 1) == 0)
		{
			clk = get_dom_first_child_node(nlk);
			while (clk)
			{
				if (compare_text(get_dom_node_name_ptr(clk), -1, _T("Prefix"), -1, 1) == 0)
				{
					xsncpy(fi.file_name, get_dom_node_text_ptr(clk), META_LEN);
				}

				clk = get_dom_next_sibling_node(clk);
			}

			fi.is_dir = 1;
			if (pf)
			{
				(*pf)(&fi, pa);
			}
		}

		nlk = get_dom_next_sibling_node(nlk);
	}

	destroy_xml_doc(xml);

	return 1;
}

static xhand_t xoss_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode)
{
	inet_t* pfn;

	if (is_null(fname))
		return NULL;

	pfn = (inet_t*)xmem_alloc(sizeof(inet_t));
	pfn->head.tag = _HANDLE_INET;
	pfn->proto = _PROTO_OSS;

	pfn->fmode = mode;
	xsncpy(pfn->path, fname, PATH_LEN);
	if (psd)
	{
		xsncpy(pfn->fsecu.scr_uid, psd->scr_uid, META_LEN);
		xsncpy(pfn->fsecu.scr_key, psd->scr_key, KEY_LEN);
	}

	return (xhand_t)pfn;
}

static bool_t xoss_read_file(xhand_t inet, byte_t* buf, dword_t* pb)
{
	inet_t* pfn = (inet_t*)inet;

	tchar_t sz_url[PATH_LEN + 1] = { 0 };
	tchar_t *addrat, *buckat, *pathat, *fileat;
	int addrlen, bucklen, pathlen, filelen;

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_type[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	byte_t** pbuf = NULL;
	dword_t size = 0;

	bool_t rt = 0;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	_split_oss_url(pfn->path, &addrat, &addrlen, &buckat, &bucklen, &pathat, &pathlen, &fileat, &filelen);

	xscpy(sz_url, _T("http://"));

	if (addrlen)
	{
		if (bucklen)
		{
			xsncat(sz_url, buckat, bucklen);
			xsncat(sz_url, _T("."), 1);
		}
		xsncat(sz_url, addrat, addrlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (pathlen)
	{
		xsncat(sz_url, pathat, pathlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (filelen)
	{
		xsncat(sz_url, fileat, filelen);
	}

	pbuf = buffer_alloc();

	if (!oss_ioctl(_T("GET"), sz_url, pfn->fsecu.scr_uid, pfn->fsecu.scr_key, NULL, 0, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	size = (size < *pb) ? size : *pb;
	xmem_copy((void*)buf, (void*)*pbuf, size);
	*pb = size;

	buffer_free(pbuf);

	return 1;
}

static bool_t xoss_write_file(xhand_t inet, const byte_t* buf, dword_t* pb)
{
	inet_t* pfn = (inet_t*)inet;

	tchar_t sz_url[PATH_LEN + 1] = { 0 };
	tchar_t *addrat, *buckat, *pathat, *fileat;
	int addrlen, bucklen, pathlen, filelen;

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_type[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	byte_t** pbuf = NULL;
	dword_t size = 0;

	bool_t rt = 0;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	_split_oss_url(pfn->path, &addrat, &addrlen, &buckat, &bucklen, &pathat, &pathlen, &fileat, &filelen);

	xscpy(sz_url, _T("http://"));

	if (addrlen)
	{
		if (bucklen)
		{
			xsncat(sz_url, buckat, bucklen);
			xsncat(sz_url, _T("."), 1);
		}
		xsncat(sz_url, addrat, addrlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (pathlen)
	{
		xsncat(sz_url, pathat, pathlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (filelen)
	{
		xsncat(sz_url, fileat, filelen);
	}

	pbuf = buffer_alloc();

	if (!oss_ioctl(_T("PUT"), sz_url, pfn->fsecu.scr_uid, pfn->fsecu.scr_key, buf, *pb, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	buffer_free(pbuf);

	return 1;
}

static void xoss_close_file(xhand_t inet)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xmem_free(pfn);
}

static bool_t xoss_delete_file(const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t sz_url[PATH_LEN + 1] = { 0 };
	tchar_t *addrat, *buckat, *pathat, *fileat;
	int addrlen, bucklen, pathlen, filelen;

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_type[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	byte_t** pbuf = NULL;
	dword_t size = 0;

	bool_t rt = 0;

	_split_oss_url(fname, &addrat, &addrlen, &buckat, &bucklen, &pathat, &pathlen, &fileat, &filelen);

	xscpy(sz_url, _T("http://"));

	if (addrlen)
	{
		if (bucklen)
		{
			xsncat(sz_url, buckat, bucklen);
			xsncat(sz_url, _T("."), 1);
		}
		xsncat(sz_url, addrat, addrlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (pathlen)
	{
		xsncat(sz_url, pathat, pathlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (filelen)
	{
		xsncat(sz_url, fileat, filelen);
	}

	pbuf = buffer_alloc();

	if (!oss_ioctl(_T("DELETE"), sz_url, ((psd)? psd->scr_uid : NULL), ((psd)? psd->scr_key : NULL), NULL, 0, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	buffer_free(pbuf);

	return 1;
}

static void _oss_file_info(const file_info_t* pfi, void* pa)
{
	xmem_copy(pa, (void*)pfi, sizeof(file_info_t));
}

static bool_t xoss_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
{
	tchar_t sz_url[PATH_LEN + 1] = { 0 };
	tchar_t *addrat, *buckat, *pathat, *fileat;
	int addrlen, bucklen, pathlen, filelen;

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_type[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	byte_t** pbuf = NULL;
	dword_t size = 0;

	file_info_t fi = { 0 };

	bool_t rt = 0;

	_split_oss_url(fname, &addrat, &addrlen, &buckat, &bucklen, &pathat, &pathlen, &fileat, &filelen);

	xscpy(sz_url, _T("http://"));

	if (addrlen)
	{
		if (bucklen)
		{
			xsncat(sz_url, buckat, bucklen);
			xsncat(sz_url, _T("."), 1);
		}
		xsncat(sz_url, addrat, addrlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (pathlen)
	{
		xsncat(sz_url, pathat, pathlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (filelen)
	{
		xscat(sz_url, _T("?prefix="));
		xsncat(sz_url, fileat, filelen);
	}

	pbuf = buffer_alloc();

	if (!oss_ioctl(_T("GET"), sz_url, ((psd)? psd->scr_uid : NULL), ((psd)? psd->scr_key : NULL), NULL, 0, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	rt = _parse_oss_result(*pbuf, size, _oss_file_info, (void*)&fi);

	buffer_free(pbuf);

	if (ftime)
		format_gmttime(&fi.write_time, ftime);
	if (fsize)
		format_longlong(fi.high_size, fi.low_size, fsize);
	if (fetag)
		xsncpy(fetag, fi.file_etag, ETAG_LEN);

	return rt;
}

static bool_t xoss_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa)
{
	tchar_t sz_url[PATH_LEN + 1] = { 0 };
	tchar_t *addrat, *buckat, *pathat, *fileat;
	int addrlen, bucklen, pathlen, filelen;

	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_type[RES_LEN + 1] = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	byte_t** pbuf = NULL;
	dword_t size = 0;

	bool_t rt = 0;

	_split_oss_url(path, &addrat, &addrlen, &buckat, &bucklen, &pathat, &pathlen, &fileat, &filelen);

	xscpy(sz_url, _T("http://"));

	if (addrlen)
	{
		if (bucklen)
		{
			xsncat(sz_url, buckat, bucklen);
			xsncat(sz_url, _T("."), 1);
		}
		xsncat(sz_url, addrat, addrlen);
		xsncat(sz_url, _T("/"), 1);
	}

	if (filelen && is_suffix(fileat, _T(".*")))
	{
		if (pathlen)
		{
			xscat(sz_url, _T("?prefix="));
			xsncat(sz_url, pathat, pathlen);
			xsncat(sz_url, _T("/"), 1);
			xscat(sz_url, _T("&delimiter=/"));
		}
	}
	else
	{
		if (pathlen)
		{
			xsncat(sz_url, pathat, pathlen);
			xsncat(sz_url, _T("/"), 1);
		}

		if (filelen)
		{
			xscat(sz_url, _T("?prefix="));
			xsncat(sz_url, fileat, filelen);
		}
	}

	pbuf = buffer_alloc();

	if (!oss_ioctl(_T("GET"), sz_url, ((psd)? psd->scr_uid : NULL), ((psd)? psd->scr_key : NULL), NULL, 0, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		buffer_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	rt = _parse_oss_result(*pbuf, size, pf, pa);

	buffer_free(pbuf);

	return rt;
}

/***********************************************************************************************************************************/

static xhand_t tftp_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode)
{
	inet_t* pfn;

	if (is_null(fname))
		return NULL;

	pfn = (inet_t*)xmem_alloc(sizeof(inet_t));
	pfn->head.tag = _HANDLE_INET;

	pfn->proto = _PROTO_TFTP;
	pfn->fmode = mode;
	xsncpy(pfn->path, fname, PATH_LEN);
	if (psd)
	{
		xsncpy(pfn->fsecu.scr_uid, psd->scr_uid, META_LEN);
		xsncpy(pfn->fsecu.scr_key, psd->scr_key, KEY_LEN);
	}

	return (xhand_t)pfn;
}

static bool_t tftp_write_file(xhand_t inet, const byte_t* buf, dword_t* pb)
{
	inet_t* pfn = (inet_t*)inet;
	xhand_t xh;
	bool_t rt = 0;
	dword_t bys, size, pos = 0;

	xh = xtftp_client(_T("PUT"), pfn->path);
	if (!xh)
		return 0;

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
	inet_t* pfn = (inet_t*)inet;
	xhand_t xh;
	bool_t rt;
	dword_t bys, size, pos = 0;

	xh = xtftp_client(_T("GET"), pfn->path);
	if (!xh)
		return 0;

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
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xmem_free(pfn);
}

static bool_t tftp_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa)
{
	xhand_t xh;
	tchar_t ftime[DATE_LEN] = { 0 };
	file_info_t fi = { 0 };

	xh = xtftp_client(_T("HEAD"), path);
	if (!xh)
		return 0;

	while (xtftp_fetch(xh))
	{
		fi.is_dir = xtftp_get_isdir(xh);
		xtftp_get_filename(xh, fi.file_name);

		if (!fi.is_dir)
		{
			xtftp_get_filetime(xh, ftime);
			parse_gmttime(&fi.write_time, ftime);
			fi.low_size = xtftp_get_filesize(xh);
		}
		
		(*pf)(&fi, pa);
	}

	xtftp_close(xh);

	return 0;
}

static bool_t tftp_delete_file(const secu_desc_t* psd, const tchar_t* fname)
{
	xhand_t xh;

	xh = xtftp_client(_T("DELETE"), fname);
	if (!xh)
		return 0;

	xtftp_close(xh);

	return 1;
}

static bool_t tftp_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
{
	xhand_t xh;

	xh = xtftp_client(_T("HEAD"), fname);
	if (!xh)
		return 0;

	if (xtftp_fetch(xh))
	{
		if (ftime)
			xtftp_get_filetime(xh, ftime);
		if (fsize)
			ltoxs(xtftp_get_filesize(xh), fsize, INT_LEN);
	}

	xtftp_close(xh);

	return 1;
}
/*************************************************************************************************/

xhand_t inet_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode)
{
	int proto;

	proto = parse_proto(fname);

	if (proto == _PROTO_HTTP)
		return http_open_file(psd, fname, mode);
	else if (proto == _PROTO_TFTP)
		return tftp_open_file(psd, fname, mode);
	if (proto == _PROTO_OSS)
		return xoss_open_file(psd, fname, mode);
	else
		return NULL;
}

void inet_close_file(xhand_t inet)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		http_close_file(inet);
	else if (pfn->proto == _PROTO_TFTP)
		tftp_close_file(inet);
	else if (pfn->proto == _PROTO_OSS)
		xoss_close_file(inet);
}

bool_t inet_read_file(xhand_t inet, byte_t* buf, dword_t* pb)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		return http_read_file(inet, buf, pb);
	else if (pfn->proto == _PROTO_TFTP)
		return tftp_read_file(inet, buf, pb);
	else if (pfn->proto == _PROTO_OSS)
		return xoss_read_file(inet, buf, pb);
	else
		return 0;
}

bool_t inet_write_file(xhand_t inet, const byte_t* buf, dword_t* pb)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		return http_write_file(inet, buf, pb);
	else if (pfn->proto == _PROTO_TFTP)
		return tftp_write_file(inet, buf, pb);
	else if (pfn->proto == _PROTO_OSS)
		return xoss_write_file(inet, buf, pb);
	else
		return 0;
}

bool_t inet_read_file_range(xhand_t inet, dword_t hoff, dword_t loff, byte_t* buf, dword_t size)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		return http_read_file_range(inet, hoff, loff, buf, size);
	else
		return 0;
}

bool_t inet_write_file_range(xhand_t inet, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		return http_write_file_range(inet, hoff, loff, buf, size);
	else
		return 0;
}

void inet_set_filetime(xhand_t inet, const tchar_t* ftime)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		_inet_set_filetime(inet, ftime);
}

void inet_set_filesince(xhand_t inet, int since)
{
	inet_t* pfn = (inet_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	if (pfn->proto == _PROTO_HTTP)
		_inet_set_filesince(inet, since);
}

bool_t inet_delete_file(const secu_desc_t* psd, const tchar_t* fname)
{
	int proto;

	proto = parse_proto(fname);

	if (proto == _PROTO_HTTP)
		return http_delete_file(psd, fname);
	else if (proto == _PROTO_OSS)
		return xoss_delete_file(psd, fname);
	else if (proto == _PROTO_TFTP)
		return tftp_delete_file(psd, fname);
	else
		return 0;
}

bool_t inet_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa)
{
	int proto;

	proto = parse_proto(path);

	if (proto == _PROTO_HTTP)
		return http_list_file(psd, path, pf, pa);
	else if (proto == _PROTO_OSS)
		return xoss_list_file(psd, path, pf, pa);
	else if (proto == _PROTO_TFTP)
		return tftp_list_file(psd, path, pf, pa);
	else
		return 0;
}

bool_t inet_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
{
	int proto;

	proto = parse_proto(fname);

	if (proto == _PROTO_HTTP)
		return http_file_info(psd, fname, ftime, fsize, fetag, fencode);
	else if (proto == _PROTO_OSS)
		return xoss_file_info(psd, fname, ftime, fsize, fetag, fencode);
	else if (proto == _PROTO_TFTP)
		return tftp_file_info(psd, fname, ftime, fsize, fetag, fencode);
	else
		return 0;
}


#endif /*XDK_SUPPORT_SOCK*/
