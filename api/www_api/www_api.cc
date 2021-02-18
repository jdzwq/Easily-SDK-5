/***********************************************************************
	Easily www web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc www web api document

	@module	www_api.cc | www web api implement file

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

#include "www_api.h"

bool_t _invoke_get(const https_block_t* pb)
{
	tchar_t sz_object[PATH_LEN + 1] = { 0 };
	tchar_t sz_filter[RES_LEN + 1] = { 0 };
	tchar_t sz_content[RES_LEN + 1] = { 0 };

	tchar_t fsince[DATE_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t fetag[ETAG_LEN + 1] = { 0 };
	tchar_t not_etag[ETAG_LEN + 1] = { 0 };
	tchar_t yes_etag[ETAG_LEN + 1] = { 0 };
	tchar_t fencode[INT_LEN + 1] = { 0 };
	tchar_t frange[RES_LEN + 1] = { 0 };
	
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

    xdate_t dt_since = {0};
    xdate_t dt_time = {0};

	file_t xf = NULL;
	byte_t *sz_buf = NULL;
	byte_t *sz_zip = NULL;
	dword_t n_zip,n_size = 0;
	dword_t n_hoff, n_loff;
	long long n_total;

	bool_t b_rt = 0;
	bool_t b_range = 0;
	bool_t b_zip = 0;

	TRY_CATCH;
	
	xhttp_get_request_header(pb->http, HTTP_HEADER_RANGE, -1, frange, RES_LEN);
	xhttp_get_request_header(pb->http, HTTP_HEADER_IFMODIFIEDSINCE, -1, fsince, DATE_LEN);
	xhttp_get_request_header(pb->http, HTTP_HEADER_IFMATCH, -1, yes_etag, ETAG_LEN);
	xhttp_get_request_header(pb->http, HTTP_HEADER_IFNONEMATCH, -1, not_etag, ETAG_LEN);

	if (is_null(pb->object))
	{
		xsprintf(sz_object, _T("%s%s"), pb->path, _T("/index.html"));
	}
	else if ((pb->object[0] == _T('/') || pb->object[0] == _T('\\')) && pb->object[1] == _T('\0'))
	{
		xsprintf(sz_object, _T("%s%s"), pb->path, _T("/index.html"));
	}
	else
	{
		xsprintf(sz_object, _T("%s%s"), pb->path, pb->object);
	}

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[WWW: GET]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, _T(""), sz_object, -1);
	}

	if (!xfile_info(NULL, sz_object, ftime, fsize, fetag, NULL))
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize) && is_null(frange))
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_403);
		xhttp_set_response_message(pb->http, HTTP_CODE_403_TEXT, -1);

		raise_user_error(_T("www_api._invoke_get"), _T("not support large file\n"));
	}

	if (!is_null(frange))
	{
		n_hoff = n_loff = n_size = 0;
		n_total = 0;
		parse_bytes_range(frange, &n_hoff, &n_loff, &n_size, &n_total);

		if (!n_total)
			n_total = xstoll(fsize);

		b_range = 1;
	}
	else
	{
		n_size = xstol(fsize);
		n_hoff = 0;
		n_loff = 0;
		n_total = n_size;

		b_range = 0;
	}

	/*if (!is_null(yes_etag))
	{
		if (compare_text(yes_etag, -1, fetag, -1, 0) != 0)
		{
			xhttp_set_response_code(pb->http, HTTP_CODE_304);
			xhttp_set_response_message(pb->http, HTTP_CODE_304_TEXT, -1);

			raise_user_error(_T("www_api._invoke_get"), _T("file not modified\n"));
		}
	}

	if (!is_null(not_etag))
	{
		if (compare_text(not_etag, -1, fetag, -1, 0) == 0)
		{
			xhttp_set_response_code(pb->http, HTTP_CODE_304);
			xhttp_set_response_message(pb->http, HTTP_CODE_304_TEXT, -1);

			raise_user_error(_T("www_api._invoke_get"), _T("file not modified\n"));
		}
	}

	if (!is_null(fsince))
	{
		parse_gmttime(&dt_since, fsince);
		parse_gmttime(&dt_time, ftime);

		if (compare_datetime(&dt_since, &dt_time) >= 0)
		{
			xhttp_set_response_code(pb->http, HTTP_CODE_304);
			xhttp_set_response_message(pb->http, HTTP_CODE_304_TEXT, -1);

			raise_user_error(_T("www_api._invoke_get"), _T("file not modified\n"));
		}
	}*/

	xf = xfile_open(NULL, sz_object, 0);
	if (!xf)
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_403);
		xhttp_set_response_message(pb->http, HTTP_CODE_403_TEXT, -1);

		raise_user_error(_T("www_api._invoke_get"), _T("open file failed\n"));
	}

	sz_buf = (byte_t*)xmem_alloc(n_size);

	if (b_range)
		b_rt = xfile_read_range(xf, n_hoff, n_loff, sz_buf, n_size);
	else
		b_rt = xfile_read(xf, sz_buf, n_size);

	xfile_close(xf);
	xf = NULL;

	if (!b_rt)
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_403);
		xhttp_set_response_message(pb->http, HTTP_CODE_403_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	if (n_size > XHTTP_ZIPED_SIZE)
	{
		xhttp_get_request_header(pb->http, HTTP_HEADER_ACCEPTENCODING, -1, fencode, INT_LEN);

		if (compare_text(fencode, -1, HTTP_HEADER_CONTENTENCODING_DEFLATE, -1, 1) == 0)
		{
			sz_zip = (byte_t*)xmem_alloc(n_size);

			n_zip = n_size;
			if (!xzlib_compress_bytes(sz_buf, n_size, sz_zip, &n_zip))
			{
				xmem_free(sz_zip);
				sz_zip = NULL;

				xsprintf(fsize, _T("%d"), n_size);
				xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
				b_zip = 0;
			}
			else
			{
				xsprintf(fsize, _T("%d"), n_zip);
				xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
				xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTENCODING, -1, HTTP_HEADER_CONTENTENCODING_DEFLATE, -1);
				b_zip = 1;
			}
		}else if (compare_text(fencode, -1, HTTP_HEADER_CONTENTENCODING_GZIP, -1, 1) == 0)
		{
			sz_zip = (byte_t*)xmem_alloc(n_size);

			n_zip = n_size;
			if (!xgzip_compress_bytes(sz_buf, n_size, sz_zip, &n_zip))
			{
				xmem_free(sz_zip);
				sz_zip = NULL;

				xsprintf(fsize, _T("%d"), n_size);
				xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
				b_zip = 0;
			}
			else
			{
				xsprintf(fsize, _T("%d"), n_zip);
				xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
				xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTENCODING, -1, HTTP_HEADER_CONTENTENCODING_GZIP, -1);
				b_zip = 1;
			}
		}
		else
		{
			xsprintf(fsize, _T("%d"), n_size);
			xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
			b_zip = 0;
		}
	}
	else
	{
		xsprintf(fsize, _T("%d"), n_size);
		xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
		b_zip = 0;
	}

	split_path(sz_object, NULL, NULL, sz_filter);
	xhttp_format_content_type(sz_filter, sz_content, RES_LEN);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, sz_content, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_LASTMODIFIED, -1, ftime, -1);
	if (b_range)
	{
		xscpy(frange, _T("bytes "));
		format_bytes_range(frange + xslen(frange), n_hoff, n_loff, n_size, n_total);

		xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, frange, -1);
		xhttp_set_response_header(pb->http, HTTP_HEADER_ACCEPTRANGES, -1, HTTP_HEADER_ACCEPTRANGES_BYTES, -1);
	}

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	if (b_zip)
		xhttp_send_full(pb->http, sz_zip, n_zip);
	else
		xhttp_send_full(pb->http, sz_buf, n_size);

	if (b_zip)
	{
		xmem_free(sz_zip);
		sz_zip = NULL;
	}

	xmem_free(sz_buf);
	sz_buf = NULL;

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	if (xf)
		xfile_close(xf);

	if (sz_zip)
		xmem_free(sz_zip);

	if (sz_buf)
		xmem_free(sz_buf);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[WWW: GET]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}

	return 0;
}


void _invoke_error(const https_block_t* pb)
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
		(*pb->plg->pf_log_title)(pb->plg->unc, _T("[WWW: ERROR]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->unc, sz_code, sz_error, -1);
	}
}

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	bool_t rt = 1;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	rt = _invoke_get(pb);

	END_CATCH;

	return (rt) ? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb);

	return HTTPS_INVOKE_WITHINFO;
}
