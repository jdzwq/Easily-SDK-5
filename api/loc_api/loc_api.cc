/***********************************************************************
	Easily loc web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc loc web api document

	@module	loc_api.cc | loc web api implement file

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

#include "loc_api.h"

typedef struct _loc_block_t{
	secu_desc_t sd;
	tchar_t local[PATH_LEN];
}loc_block_t;


bool_t _invoke_head(const https_block_t* pb, loc_block_t* pos)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_object[PATH_LEN] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t fetag[ETAG_LEN + 1] = { 0 };
	tchar_t fencode[INT_LEN + 1] = { 0 };

	TRY_CATCH;

	xsprintf(sz_object, _T("%s%s"), pos->local, pb->object);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: HEAD]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), sz_object, -1);
	}

	if (!xfile_info(&pos->sd, sz_object, ftime, fsize, fetag, fencode))
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_LASTMODIFIED, -1, ftime, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_ETAG, -1, fetag, -1);
	if (!is_null(fencode))
	{
		xhttp_set_response_content_type(pb->http, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
		xhttp_set_response_content_type_charset(pb->http, fencode, -1);
	}

	xhttp_send_response(pb->http);

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: HEAD]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_list(const https_block_t* pb, loc_block_t* pos)
{
	tchar_t sz_object[PATH_LEN] = { 0 };
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_enc[RES_LEN + 1] = { 0 };

	tchar_t feed[3] = { TXT_ITEMFEED, TXT_LINEFEED, _T('\n') };
	
	stream_t stream = NULL;
	int encode = 0;

	TRY_CATCH;

	xsprintf(sz_object, _T("%s%s"), pos->local, pb->object);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: LIST]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), sz_object, -1);
	}

	xhttp_get_request_accept_charset(pb->http, sz_enc, RES_LEN);
	encode = parse_charset(sz_enc);
	if (!encode)
	{
		encode = _UTF8;
	}
	format_charset(encode, sz_enc);

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, -1);
	xhttp_set_response_content_type_charset(pb->http, sz_enc, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_TRANSFERENCODING, -1, HTTP_HEADER_TRANSFERENCODING_CHUNKED, -1);

	if (!xhttp_send_response(pb->http))
	{
		raise_user_error(NULL, NULL);
	}

	stream = xhttp_get_send_stream(pb->http);

	if (!xfile_dump(NULL, sz_object, stream))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (!xhttp_is_responsed(pb->http))
	{
		xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);
	}

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: LIST]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_get(const https_block_t* pb, loc_block_t* pos)
{
	tchar_t sz_object[PATH_LEN] = { 0 };
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

	xdate_t dt_since, dt_time;

	if_fio_t* xf = NULL;
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

	xsprintf(sz_object, _T("%s%s"), pos->local, pb->object);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: GET]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), sz_object, -1);
	}

	if (!xfile_info(&pos->sd, sz_object, ftime, fsize, fetag, NULL))
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize) && is_null(frange))
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_403);
		xhttp_set_response_message(pb->http, HTTP_CODE_403_TEXT, -1);

		raise_user_error(_T("loc_api._invoke_gut"), _T("not support large file\n"));
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

	xscpy(frange, _T("bytes "));
	format_bytes_range(frange + xslen(frange), n_hoff, n_loff, n_size, n_total);

	if (!is_null(yes_etag))
	{
		if (compare_text(yes_etag, -1, fetag, -1, 0) != 0)
		{
			xhttp_set_response_code(pb->http, HTTP_CODE_304);
			xhttp_set_response_message(pb->http, HTTP_CODE_304_TEXT, -1);

			raise_user_error(_T("loc_api._invoke_gut"), _T("file not modified\n"));
		}
	}

	if (!is_null(not_etag))
	{
		if (compare_text(not_etag, -1, fetag, -1, 0) == 0)
		{
			xhttp_set_response_code(pb->http, HTTP_CODE_304);
			xhttp_set_response_message(pb->http, HTTP_CODE_304_TEXT, -1);

			raise_user_error(_T("loc_api._invoke_gut"), _T("file not modified\n"));
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

			raise_user_error(_T("loc_api._invoke_gut"), _T("file not modified\n"));
		}
	}

	xf = xfile_open(&pos->sd, sz_object, 0);
	if (!xf)
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_403);
		xhttp_set_response_message(pb->http, HTTP_CODE_403_TEXT, -1);

		raise_user_error(_T("loc_api._invoke_gut"), _T("open file failed\n"));
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

	xhttp_set_response_header(pb->http, HTTP_HEADER_LASTMODIFIED, -1, ftime, -1);
	if (b_range)
	{
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

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: GET]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_put(const https_block_t* pb, loc_block_t* pos)
{
	tchar_t sz_object[PATH_LEN] = { 0 };
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t frange[RES_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t fetag[ETAG_LEN + 1] = { 0 };
	tchar_t fsince[DATE_LEN + 1] = { 0 };
	tchar_t fencode[INT_LEN + 1] = { 0 };
	tchar_t yes_etag[ETAG_LEN + 1] = { 0 };
	tchar_t not_etag[ETAG_LEN + 1] = { 0 };

	dword_t n_hoff, n_loff, n_bys;
	long long n_all;

	xdate_t dt_since, dt_time;

	if_fio_t* xf = NULL;
	byte_t** pbuf = NULL;
	byte_t* sz_zip = NULL;
	dword_t n_zip,n_size = 0;
	bool_t b_rt = 0;
	bool_t b_zip = 0;

	TRY_CATCH;

	pbuf = bytes_alloc();
	n_size = 0;

	b_rt = xhttp_recv_full(pb->http, pbuf, &n_size);
	if (!b_rt)
	{
		raise_user_error(NULL, NULL);
	}

	xsprintf(sz_object, _T("%s%s"), pos->local, pb->object);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: PUT]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), sz_object, -1);
	}

	xhttp_get_request_header(pb->http, HTTP_HEADER_IFMODIFIEDSINCE, -1, fsince, DATE_LEN);
	xhttp_get_request_header(pb->http, HTTP_HEADER_IFMATCH, -1, yes_etag, ETAG_LEN);
	xhttp_get_request_header(pb->http, HTTP_HEADER_IFNONEMATCH, -1, not_etag, ETAG_LEN);

	if (!is_null(fsince))
	{
		xfile_info(&pos->sd, sz_object, ftime, NULL, NULL, NULL);
		if (!is_null(ftime))
		{
			parse_gmttime(&dt_since, fsince);
			parse_gmttime(&dt_time, ftime);

			if (compare_datetime(&dt_since, &dt_time) <= 0)
			{
				xhttp_set_response_code(pb->http, HTTP_CODE_304);
				xhttp_set_response_message(pb->http, HTTP_CODE_304_TEXT, -1);

				raise_user_error(_T("loc_api._invoke_put"), _T("file not modified\n"));
			}
		}
	}

	if (!is_null(yes_etag))
	{
		xfile_info(&pos->sd, sz_object, NULL, NULL, fetag, NULL);
		if (!is_null(fetag))
		{
			if (compare_text(yes_etag, -1, fetag, -1, 1) != 0)
			{
				xhttp_set_response_code(pb->http, HTTP_CODE_304);
				xhttp_set_response_message(pb->http, HTTP_CODE_304_TEXT, -1);

				raise_user_error(_T("loc_api._invoke_put"), _T("file not modified\n"));
			}
		}
	}

	if (!is_null(not_etag))
	{
		xfile_info(&pos->sd, sz_object, NULL, NULL, fetag, NULL);
		if (!is_null(fetag))
		{
			if (compare_text(not_etag, -1, fetag, -1, 1) == 0)
			{
				xhttp_set_response_code(pb->http, HTTP_CODE_304);
				xhttp_set_response_message(pb->http, HTTP_CODE_304_TEXT, -1);

				raise_user_error(_T("loc_api._invoke_put"), _T("file not modified\n"));
			}
		}
	}

	xhttp_get_request_header(pb->http, HTTP_HEADER_LASTMODIFIED, -1, ftime, DATE_LEN);
	xhttp_get_request_header(pb->http, HTTP_HEADER_CONTENTRANGE, -1, frange, RES_LEN);
	xhttp_get_request_header(pb->http, HTTP_HEADER_CONTENTENCODING, -1, fencode, INT_LEN);

	if (is_null(frange))
	{
		n_hoff = 0;
		n_loff = 0;
		n_bys = n_size;
		n_all = n_size;
	}else
	{
		n_hoff = n_loff = n_bys = 0;
		n_all = 0;
		parse_bytes_range(frange, &n_hoff, &n_loff, &n_bys, &n_all);
	}

	if (compare_text(fencode, -1, HTTP_HEADER_CONTENTENCODING_DEFLATE, -1, 1) == 0)
	{
		sz_zip = (byte_t*)xmem_alloc(n_bys);
		n_zip = n_bys;
		if (!xzlib_uncompress_bytes(*pbuf, n_size, sz_zip, &n_zip))
		{
			raise_user_error(_T("loc_api._invoke_put"), _T("uncompress data failed\n"));
		}
		b_zip = 1;
	}
	else if (compare_text(fencode, -1, HTTP_HEADER_CONTENTENCODING_GZIP, -1, 1) == 0)
	{
		sz_zip = (byte_t*)xmem_alloc(n_bys);
		n_zip = n_bys;
		if (!xgzip_uncompress_bytes(*pbuf, n_size, sz_zip, &n_zip))
		{
			raise_user_error(_T("loc_api._invoke_put"), _T("uncompress data failed\n"));
		}
		b_zip = 1;
	}
	else
	{
		b_zip = 0;
	}

	if (MAKELWORD(n_loff, n_hoff) == 0 && MAKELWORD(n_bys, 0) == n_all)
		xf = xfile_open(&pos->sd, sz_object, FILE_OPEN_CREATE);
	else
		xf = xfile_open(&pos->sd, sz_object, FILE_OPEN_APPEND);

	if (!xf)
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_403);
		xhttp_set_response_message(pb->http, HTTP_CODE_403_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	if (!is_null(ftime))
	{
		xfile_settime(xf, ftime);
	}

	if (MAKELWORD(n_loff, n_hoff) == 0 && MAKELWORD(n_bys, 0) == n_all)
	{
		if (b_zip)
			b_rt = xfile_write(xf, sz_zip, n_zip);
		else
			b_rt = xfile_write(xf, *pbuf, n_size);	
	}
	else
	{
		if (b_zip)
			b_rt = xfile_write_range(xf, n_hoff, n_loff, sz_zip, n_zip);
		else
			b_rt = xfile_write_range(xf, n_hoff, n_loff, *pbuf, n_size);
	}

	xfile_close(xf);
	xf = NULL;

	if (b_zip)
	{
		xmem_free(sz_zip);
		sz_zip = NULL;
	}

	bytes_free(pbuf);
	pbuf = NULL;

	if (!b_rt)
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_403);
		xhttp_set_response_message(pb->http, HTTP_CODE_403_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	xhttp_send_error(pb->http, HTTP_CODE_201, HTTP_CODE_201_TEXT, _T("0"), _T("xhttp write file succeeded\n"), -1);

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (xf)
		xfile_close(xf);

	if (sz_zip)
		xmem_free(sz_zip);

	if (pbuf)
		bytes_free(pbuf);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: PUT]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_delete(const https_block_t* pb, loc_block_t* pos)
{
	tchar_t sz_object[PATH_LEN] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	xsprintf(sz_object, _T("%s%s"), pos->local, pb->object);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: DELETE]"), -1);

		(*pb->pf_log_error)(pb->log, _T(""), sz_object, -1);
	}

	if (!xfile_info(&pos->sd, sz_object, ftime, NULL, NULL, NULL))
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	if (!xfile_delete(&pos->sd, sz_object))
	{
		xhttp_set_response_code(pb->http, HTTP_CODE_403);
		xhttp_set_response_message(pb->http, HTTP_CODE_403_TEXT, -1);

		raise_user_error(NULL, NULL);
	}

	xhttp_send_error(pb->http, HTTP_CODE_202, HTTP_CODE_202_TEXT,_T("0"), _T("xhttp delete file succeeded\n"), -1);

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[LOC: DELETE]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

void _invoke_error(const https_block_t* pb, loc_block_t* pos)
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
		(*pb->pf_log_title)(pb->log, _T("[LOC: ERROR]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	loc_block_t* pos = NULL;

	tchar_t sz_local[PATH_LEN] = { 0 };

	bool_t rt = 1;

	link_t_ptr ptr_prop = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pos = (loc_block_t*)xmem_alloc(sizeof(loc_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(sz_local, _T("%s/loc.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, sz_local))
	{
		raise_user_error(_T("-1"), _T("load loc config falied\n"));
	}

	read_proper(ptr_prop, _T("LOC"), -1, _T("LOCATION"), -1, sz_local, PATH_LEN);
	read_proper(ptr_prop, _T("LOC"), -1, _T("PUBLICKEY"), -1, pos->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("LOC"), -1, _T("PRIVATEKEY"), -1, pos->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pos->local, sz_local);

	if (compare_text(method, -1, _T("HEAD"), -1, 1) == 0)
		rt = _invoke_head(pb, pos);
	else if (compare_text(method, -1, _T("LIST"), -1, 1) == 0)
		rt = _invoke_list(pb, pos);
	else if (compare_text(method, -1, _T("GET"), -1, 1) == 0)
		rt = _invoke_get(pb, pos);
	else if (compare_text(method, -1, _T("PUT"), -1, 1) == 0)
		rt = _invoke_put(pb, pos);
	else if (compare_text(method, -1, _T("DELETE"), -1, 1) == 0)
		rt = _invoke_delete(pb, pos);
	else
	{
		raise_user_error(_T("loc_api"), _T("unknown loc method"));
	}

	xmem_free(pos);

	END_CATCH;

	return (rt) ? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb, pos);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	xmem_free(pos);

	return HTTPS_INVOKE_WITHINFO;
}