/***********************************************************************
	Easily oss web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc oss web api document

	@module	oss_api.cc | oss web api implement file

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

#include "oss_api.h"

typedef struct _oss_block_t{
	PF_OSS_OPEN_ISP pf_open_isp;
	PF_OSS_CLOSE pf_close;
	PF_OSS_IOCTL pf_ioctl;
	PF_OSS_ERROR pf_error;

	tchar_t isp_file[PATH_LEN];
}oss_block_t;


bool_t _invoke_head(const https_block_t* pb, oss_block_t* pos)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_object[PATH_LEN] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };

	oss_t oss = NULL;

	file_info_t fi = { 0 };
	dword_t olen;

	TRY_CATCH;

	xhttp_get_url_query_entity(pb->http, _T("object"), -1, sz_object, PATH_LEN);
	if (is_null(sz_object))
	{
		raise_user_error(_T("_invoke_head"), _T("empty object"));
	}

	oss = (*pos->pf_open_isp)(pos->isp_file);

	if (!oss)
	{
		raise_user_error(_T("_invoke_head"), _T("open isp file failed"));
	}

	olen = sizeof(file_info_t);

	if (!(*pos->pf_ioctl)(oss, _T("HEAD"), sz_object, NULL, 0, (void*)&fi, &olen))
	{
		(*pos->pf_error)(oss, sz_error, ERR_LEN);

		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(_T("_invoke_head"), sz_error);
	}

	(*pos->pf_close)(oss);
	oss = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	lltoxs(fi.file_size, fsize, NUM_LEN);
	format_gmttime(&fi.write_time, ftime);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
	xhttp_set_response_header(pb->http, HTTP_HEADER_LASTMODIFIED, -1, ftime, -1);

	xhttp_send_response(pb->http);

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (oss)
		(*pos->pf_close)(oss);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OSS错误]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_list(const https_block_t* pb, oss_block_t* pos)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_object[PATH_LEN] = { 0 };

	oss_t oss = NULL;

	byte_t* obuf = NULL;
	dword_t olen;

	TRY_CATCH;

	xhttp_get_url_query_entity(pb->http, _T("object"), -1, sz_object, PATH_LEN);
	if (is_null(sz_object))
	{
		raise_user_error(_T("_invoke_list"), _T("empty object"));
	}

	oss = (*pos->pf_open_isp)(pos->isp_file);

	if (!oss)
	{
		raise_user_error(_T("_invoke_list"), _T("open isp file failed"));
	}

	olen = MAX_LONG;

	if (!(*pos->pf_ioctl)(oss, _T("LIST"), sz_object, NULL, 0, NULL, &olen))
	{
		(*pos->pf_error)(oss, sz_error, ERR_LEN);

		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(_T("_invoke_list"), sz_error);
	}

	obuf = (byte_t*)xmem_alloc(olen);

	if (!(*pos->pf_ioctl)(oss, _T("LIST"), pb->object, NULL, 0, (void*)obuf, &olen))
	{
		(*pos->pf_error)(oss, sz_error, ERR_LEN);

		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(_T("_invoke_list"), sz_error);
	}

	(*pos->pf_close)(oss);
	oss = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_APPXML, -1);

	xhttp_send_full(pb->http, obuf, olen);

	xmem_free(obuf);
	obuf = NULL;

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (obuf)
		xmem_free(obuf);

	if (oss)
		(*pos->pf_close)(oss);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OSS错误]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_get(const https_block_t* pb, oss_block_t* pos)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_object[PATH_LEN] = { 0 };

	oss_t oss = NULL;

	file_info_t fi = { 0 };

	byte_t* obuf = NULL;
	dword_t olen;

	TRY_CATCH;

	xhttp_get_url_query_entity(pb->http, _T("object"), -1, sz_object, PATH_LEN);
	if (is_null(sz_object))
	{
		raise_user_error(_T("_invoke_get"), _T("empty object"));
	}

	oss = (*pos->pf_open_isp)(pos->isp_file);

	if (!oss)
	{
		raise_user_error(_T("_invoke_get"), _T("open isp file failed"));
	}

	olen = sizeof(file_info_t);

	if (!(*pos->pf_ioctl)(oss, _T("HEAD"), pb->object, NULL, 0, (void*)&fi, &olen))
	{
		(*pos->pf_error)(oss, sz_error, ERR_LEN);

		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(_T("_invoke_get"), sz_error);
	}

	olen = (dword_t)fi.file_size;
	obuf = (byte_t*)xmem_alloc(olen);

	if (!(*pos->pf_ioctl)(oss, _T("GET"), pb->object, NULL, 0, (void*)obuf, &olen))
	{
		(*pos->pf_error)(oss, sz_error, ERR_LEN);

		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(_T("_invoke_get"), sz_error);
	}

	(*pos->pf_close)(oss);
	oss = NULL;

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_send_full(pb->http, obuf, olen);

	xmem_free(obuf);
	obuf = NULL;

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (obuf)
		xmem_free(obuf);

	if (oss)
		(*pos->pf_close)(oss);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OSS错误]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_put(const https_block_t* pb, oss_block_t* pos)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_object[PATH_LEN] = { 0 };

	oss_t oss = NULL;

	byte_t** pbuf = NULL;
	dword_t ilen;

	dword_t olen;

	TRY_CATCH;

	pbuf = bytes_alloc();
	ilen = 0;

	if(!xhttp_recv_full(pb->http, pbuf, &ilen))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_get_url_query_entity(pb->http, _T("object"), -1, sz_object, PATH_LEN);
	if (is_null(sz_object))
	{
		raise_user_error(_T("_invoke_put"), _T("empty object"));
	}

	oss = (*pos->pf_open_isp)(pos->isp_file);

	if (!oss)
	{
		raise_user_error(_T("_invoke_put"), _T("open isp file failed"));
	}

	olen = 0;

	if (!(*pos->pf_ioctl)(oss, _T("PUT"), sz_object, (void*)(*pbuf), ilen, NULL, &olen))
	{
		(*pos->pf_error)(oss, sz_error, ERR_LEN);

		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(_T("_invoke_put"), sz_error);
	}

	bytes_free(pbuf);
	pbuf = NULL;

	(*pos->pf_close)(oss);
	oss = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_202, HTTP_CODE_202_TEXT, _T("0"), _T("xhttp put file succeeded\n"), -1);

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (pbuf)
		bytes_free(pbuf);

	if (oss)
		(*pos->pf_close)(oss);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OSS错误]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

bool_t _invoke_delete(const https_block_t* pb, oss_block_t* pos)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	tchar_t sz_object[PATH_LEN] = { 0 };

	oss_t oss = NULL;
	dword_t olen = 0;

	TRY_CATCH;

	xhttp_get_url_query_entity(pb->http, _T("object"), -1, sz_object, PATH_LEN);
	if (is_null(sz_object))
	{
		raise_user_error(_T("_invoke_delete"), _T("empty object"));
	}

	oss = (*pos->pf_open_isp)(pos->isp_file);

	if (!oss)
	{
		raise_user_error(_T("_invoke_delete"), _T("open isp file failed"));
	}

	if (!(*pos->pf_ioctl)(oss, _T("DELETE"), sz_object, NULL, 0, NULL, &olen))
	{
		(*pos->pf_error)(oss, sz_error, ERR_LEN);

		xhttp_set_response_code(pb->http, HTTP_CODE_404);
		xhttp_set_response_message(pb->http, HTTP_CODE_404_TEXT, -1);

		raise_user_error(_T("_invoke_delete"), sz_error);
	}

	(*pos->pf_close)(oss);
	oss = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_202, HTTP_CODE_202_TEXT, _T("0"), _T("xhttp delete file succeeded\n"), -1);

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(sz_code, sz_error, ERR_LEN);

	if (oss)
		(*pos->pf_close)(oss);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (pb->log)
	{
		(*pb->pf_log_title)(pb->log, _T("[OSS错误]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}

	return 0;
}

void _invoke_error(const https_block_t* pb, oss_block_t* pos)
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
		(*pb->pf_log_title)(pb->log, _T("[OSS错误]"), -1);

		(*pb->pf_log_error)(pb->log, sz_code, sz_error, -1);
	}
}

/********************************************************************************************/

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	oss_block_t* pos = NULL;

	bool_t rt = 1;

	res_modu_t oss_lib = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pos = (oss_block_t*)xmem_alloc(sizeof(oss_block_t));

	if (xsnicmp(_T("aliyun"), pb->object + 1, xslen(_T("aliyun"))) == 0)
	{
#if defined(_OS_WINDOWS)
		oss_lib = load_library(_T("oss_aliyun.dll"));
#elif defined(_OS_MACOS)
		oss_lib = load_library(_T("liboss_aliyun.dylib"));
#elif defined(_OS_LINUX)
		oss_lib = load_library(_T("liboss_aliyun.so"));
#endif
	}
	else
	{
		raise_user_error(_T("oss_api"), _T("unknown oss library"));
	}

	if (!oss_lib)
	{
		raise_user_error(_T("oss_api"), _T("load oss library falied\n"));
	}

	pos->pf_open_isp = (PF_OSS_OPEN_ISP)get_address(oss_lib, "oss_open_isp");
	pos->pf_close = (PF_OSS_CLOSE)get_address(oss_lib, "oss_close");
	pos->pf_ioctl = (PF_OSS_IOCTL)get_address(oss_lib, "oss_ioctl");
	pos->pf_error = (PF_OSS_ERROR)get_address(oss_lib, "oss_error");

	if (!pos->pf_open_isp || !pos->pf_close || !pos->pf_ioctl || !pos->pf_error)
	{
		raise_user_error(_T("oss_api"), _T("get open/close functon falied"));
	}

	xsprintf(pos->isp_file, _T("%s%s"), pb->path, pb->object);

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
		raise_user_error(_T("oss_api"), _T("unknown oss method"));
	}

	free_library(oss_lib);

	xmem_free(pos);

	END_CATCH;

	return (rt)? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb, pos);

	if (oss_lib)
		free_library(oss_lib);

	xmem_free(pos);

	return HTTPS_INVOKE_WITHINFO;
}
