/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc oss invoke document

	@module	netoss.c | implement file

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

#include "netoss.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdldoc.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)

#ifdef XDL_SUPPORT_CRYPT

static int _oss_hash(
	const tchar_t* access_key,
	const tchar_t* verb,
	const tchar_t* content_md5,
	const tchar_t* content_type,
	const tchar_t* gmt_time,
	const tchar_t* oss_head,
	const tchar_t* oss_res,
	tchar_t* auth, 
	int max)
{
	int acc_len, utf8_len, bas_len;
	int pos;

	byte_t* acc_buf = NULL;
	byte_t* utf8_buf = NULL;

	unsigned char dig[20] = { 0 };

#ifdef _UNICODE
	acc_len = ucs_to_utf8(access_key, -1, NULL, MAX_LONG);
#else
	acc_len = mbs_to_utf8(access_key, -1, NULL, MAX_LONG);
#endif

	acc_buf = (byte_t*)xmem_alloc(acc_len + 1);

#ifdef _UNICODE
	ucs_to_utf8(access_key,-1,acc_buf,acc_len);
#else
	mbs_to_utf8(access_key, -1, acc_buf, acc_len);
#endif

#ifdef _UNICODE
	utf8_len = ucs_to_utf8(verb,-1,NULL,MAX_LONG) + 1 \
		+ ucs_to_utf8(content_md5,-1,NULL,MAX_LONG) + 1 \
		+ ucs_to_utf8(content_type,-1,NULL,MAX_LONG) + 1 \
		+ ucs_to_utf8(gmt_time,-1,NULL,MAX_LONG) + 1 \
		+ ucs_to_utf8(oss_head,-1,NULL,MAX_LONG) \
		+ ucs_to_utf8(oss_res,-1,NULL,MAX_LONG) + 1;
#else
	utf8_len = mbs_to_utf8(verb, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(content_md5, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(content_type, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(gmt_time, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(oss_head, -1, NULL, MAX_LONG) \
		+ mbs_to_utf8(oss_res, -1, NULL, MAX_LONG) + 1;
#endif
	
	utf8_buf = (byte_t*)xmem_alloc(utf8_len);

	pos = 0;
#ifdef _UNICODE
	pos += ucs_to_utf8(verb,-1,utf8_buf + pos,utf8_len - pos);
#else
	pos += mbs_to_utf8(verb, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(content_md5,-1,utf8_buf + pos,utf8_len - pos);
#else
	pos += mbs_to_utf8(content_md5, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(content_type,-1,utf8_buf + pos,utf8_len - pos);
#else
	pos += mbs_to_utf8(content_type, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(gmt_time,-1,utf8_buf + pos,utf8_len - pos);
#else
	pos += mbs_to_utf8(gmt_time, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(oss_head,-1,utf8_buf + pos,utf8_len - pos);
#else
	pos += mbs_to_utf8(oss_head, -1, utf8_buf + pos, utf8_len - pos);
#endif

#ifdef _UNICODE
	pos += ucs_to_utf8(oss_res,-1,utf8_buf + pos,utf8_len - pos);
#else
	pos += mbs_to_utf8(oss_res, -1, utf8_buf + pos, utf8_len - pos);
#endif

	sha1_hmac((unsigned char*)acc_buf,acc_len,(unsigned char*)utf8_buf,pos,dig);

	bas_len = xbas_encode(dig, 20, auth, max);

	xmem_free(acc_buf);
	xmem_free(utf8_buf);

	return bas_len;
}
#endif //XDL_SUPPORT_CRYPT

static void _oss_bucket(tchar_t* host)
{
	int dot_count = 0;
	tchar_t* sz = (tchar_t*)host;

	while(*sz != _T('\0'))
	{
		if(*sz == _T('.'))
			dot_count ++;
		sz ++;
	}

	if(dot_count != 3)
	{
		host[0] = _T('\0');
		return ;
	}

	sz = (tchar_t*)host;
	while(*sz != _T('.') && *sz != _T('\0'))
	{
		sz ++;
	}

	*sz = _T('\0');
}


xhand_t _oss_get_open(
	const tchar_t* url,
	const tchar_t* acc_id,
	const tchar_t* acc_key,
	tchar_t* code,
	tchar_t* msg)
{
	xhand_t xh = NULL;

	tchar_t gmt[DATE_LEN + 1] = { 0 };
	tchar_t bucket[PATH_LEN] = { 0 };
	tchar_t oss_res[PATH_LEN] = { 0 };
	tchar_t auth[PATH_LEN] = { 0 };

	xdate_t dt = { 0 };

	int res_len;
	
	xh = xhttp_client(_T("GET"), url);
	if (!xh)
	{
		get_last_error(code, msg, ERR_LEN);
		return NULL;
	}

	get_utc_date(&dt);
	format_gmttime(&dt, gmt);

	xhttp_get_url_host(xh, bucket, PATH_LEN);

	_oss_bucket(bucket);

	if (!is_null(bucket))
	{
		xscat(oss_res, _T("/"));
		xscat(oss_res, bucket);
	}

	res_len = xslen(oss_res);
	xhttp_get_url_object(xh, oss_res + res_len, PATH_LEN - res_len);

	
	xhttp_set_request_default_header(xh);
	xhttp_set_request_header(xh, HTTP_HEADER_DATE, -1, gmt, -1);

#ifdef XDL_SUPPORT_CRYPT
	xscat(auth, _T("OSS "));
	xscat(auth, acc_id);
	xscat(auth, _T(":"));

	_oss_hash(acc_key, _T("GET"), NULL, NULL, gmt, NULL, oss_res, auth + xslen(auth), 30);

	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, auth, -1);
#endif

	return xh;
}

bool_t _oss_get_send(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg)
{
	if(!xhttp_send_request(xh))
	{
		get_last_error(code, msg, ERR_LEN);
		return 0;
	}

	return 1;
}

bool_t _oss_get_response(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg,
	tchar_t* out_type,
	tchar_t* out_size)
{
	if(!xhttp_recv_response(xh))
	{
		get_last_error(code, msg, ERR_LEN);
		return 0;
	}

	xhttp_get_response_code(xh, code);
	xhttp_get_response_message(xh, msg, ERR_LEN);

	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTTYPE, -1, out_type, RES_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, out_size, NUM_LEN);

	return 1;
}

bool_t _oss_get_recv(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg,
	byte_t** pbuf,
	dword_t* pb)
{
	if(!xhttp_recv_full(xh, pbuf, pb))
	{
		get_last_error(code, msg, ERR_LEN);

		return 0;
	}

	return 1;
}

void _oss_get_close(xhand_t xh)
{
	xhttp_close(xh);
}

xhand_t _oss_put_open(
	const tchar_t* url,
	const tchar_t* acc_id,
	const tchar_t* acc_key,
	tchar_t* code,
	tchar_t* msg)
{
	xhand_t xh = NULL;
	
	tchar_t gmt[DATE_LEN + 1] = { 0 };
	tchar_t bucket[PATH_LEN] = { 0 };
	tchar_t oss_res[PATH_LEN] = { 0 };
	tchar_t auth[PATH_LEN] = { 0 };

	dword_t res_len;

	xdate_t dt = { 0 };

	xh = xhttp_client(_T("PUT"), url);
	if (!xh)
	{
		get_last_error(code, msg, ERR_LEN);
		return 0;
	}

	get_utc_date(&dt);
	format_gmttime(&dt, gmt);

	xhttp_get_url_host(xh, bucket, PATH_LEN);
	_oss_bucket(bucket);

	if (!is_null(bucket))
	{
		xscat(oss_res, _T("/"));
		xscat(oss_res, bucket);
	}

	res_len = xslen(oss_res);
	xhttp_get_url_object(xh, oss_res + res_len, PATH_LEN - res_len);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_header(xh, HTTP_HEADER_DATE, -1, gmt, -1);

#ifdef XDL_SUPPORT_CRYPT
	xscat(auth, _T("OSS "));
	xscat(auth, acc_id);
	xscat(auth, _T(":"));
	_oss_hash(acc_key, _T("PUT"), NULL, NULL, gmt, NULL, oss_res, auth + xslen(auth), 30);

	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, auth, -1);
#endif

	return xh;
}

bool_t _oss_put_send(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg,
	const byte_t* data,
	dword_t size)
{
	tchar_t in_size[NUM_LEN + 1] = { 0 };
	xsprintf(in_size, _T("%d"), size);

	xhttp_set_request_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, in_size, -1);

	if(!xhttp_send_full(xh, (byte_t*)data, size))
	{
		get_last_error(code, msg, ERR_LEN);
		return 0;
	}

	return 1;
}

bool_t _oss_put_response(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg,
	tchar_t* out_type,
	tchar_t* out_size)
{
	if(!xhttp_recv_response(xh))
	{
		get_last_error(code, msg, ERR_LEN);
		return 0;
	}

	xhttp_get_response_code(xh, code);
	xhttp_get_response_message(xh, msg, ERR_LEN);

	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTTYPE, -1, out_type, RES_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, out_size, NUM_LEN);

	return 1;
}

bool_t _oss_put_recv(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg,
	byte_t** pbuf,
	dword_t* pb)
{
	if(!xhttp_recv_full(xh, pbuf, pb))
	{
		get_last_error(code, msg, ERR_LEN);

		return 0;
	}
	
	return 1;
}

void _oss_put_close(xhand_t xh)
{
	xhttp_close(xh);
}

xhand_t _oss_delete_open(
	const tchar_t* url,
	const tchar_t* acc_id,
	const tchar_t* acc_key,
	tchar_t* code,
	tchar_t* msg)
{
	xhand_t xh = NULL;
	
	tchar_t gmt[DATE_LEN + 1] = { 0 };
	tchar_t bucket[PATH_LEN] = { 0 };
	tchar_t oss_res[PATH_LEN] = { 0 };
	tchar_t auth[PATH_LEN] = { 0 };

	dword_t res_len;

	xdate_t dt = { 0 };

	xh = xhttp_client(_T("DELETE"), url);
	if (!xh)
	{
		get_last_error(code, msg, ERR_LEN);
		return 0;
	}

	get_utc_date(&dt);
	format_gmttime(&dt, gmt);

	xhttp_get_url_host(xh, bucket, PATH_LEN);
	_oss_bucket(bucket);

	if (!is_null(bucket))
	{
		xscat(oss_res, _T("/"));
		xscat(oss_res, bucket);
	}

	res_len = xslen(oss_res);
	xhttp_get_url_object(xh, oss_res + res_len, PATH_LEN - res_len);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_header(xh, HTTP_HEADER_DATE, -1, gmt, -1);

#ifdef XDL_SUPPORT_CRYPT
	xscat(auth, _T("OSS "));
	xscat(auth, acc_id);
	xscat(auth, _T(":"));

	_oss_hash(acc_key, _T("DELETE"), NULL, NULL, gmt, NULL, oss_res, auth + xslen(auth), 30);

	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, auth, -1);
#endif

	return xh;
}

bool_t _oss_delete_send(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg)
{
	if(!xhttp_send_request(xh))
	{
		get_last_error(code, msg, ERR_LEN);
		return 0;
	}

	return 1;
}

bool_t _oss_delete_response(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg,
	tchar_t* out_type,
	tchar_t* out_size)
{
	if(!xhttp_recv_response(xh))
	{
		get_last_error(code, msg, ERR_LEN);
		return 0;
	}

	xhttp_get_response_code(xh, code);
	xhttp_get_response_message(xh, msg, ERR_LEN);

	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTTYPE, -1, out_type, RES_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, out_size, NUM_LEN);

	return 1;
}

bool_t _oss_delete_recv(
	xhand_t xh,
	tchar_t* code,
	tchar_t* msg,
	byte_t** pbuf,
	dword_t* pb)
{
	if(!xhttp_recv_full(xh, pbuf, pb))
	{
		get_last_error(code, msg, ERR_LEN);

		return 0;
	}
	
	return 1;
}

void _oss_delete_close(xhand_t xh)
{
	xhttp_close(xh);
}

bool_t oss_ioctl(
	const tchar_t* ioop,

	const tchar_t* url,
	const tchar_t* acc_id,
	const tchar_t* acc_key,

	const byte_t* in_data, 
	dword_t in_size, 

	tchar_t* out_code,
	tchar_t* out_msg,

	tchar_t* out_type,
	tchar_t* out_size,

	byte_t** out_buffer,
	dword_t* out_bytes)
{
	xhand_t xh;
	bool_t rt;

	if(compare_text(ioop,-1,_T("GET"),-1,0) == 0)
	{
		xh = _oss_get_open(url, acc_id, acc_key, out_code, out_msg);
		if (!xh)
		{
			return 0;
		}

		rt = _oss_get_send(xh, out_code, out_msg);
		if (!rt)
		{
			_oss_get_close(xh);
			return 0;
		}

		rt = _oss_get_response(xh, out_code, out_msg, out_type, out_size);
		if (!rt)
		{
			_oss_get_close(xh);
			return 0;
		}

		rt = _oss_get_recv(xh, out_code, out_msg, out_buffer, out_bytes);

		_oss_get_close(xh);

		return rt;
	}
	else if(compare_text(ioop,-1,_T("PUT"),-1,0) == 0)
	{
		xh = _oss_put_open(url, acc_id, acc_key, out_code, out_msg);
		if (!xh)
		{
			return 0;
		}

		rt = _oss_put_send(xh, out_code, out_msg, in_data, in_size);
		if (!rt)
		{
			_oss_put_close(xh);
			return 0;
		}

		rt = _oss_put_response(xh, out_code, out_msg, out_type, out_size);
		
		_oss_put_recv(xh, out_code, out_msg, out_buffer, out_bytes);
		_oss_put_close(xh);

		return rt;
	}
	else if(compare_text(ioop,-1,_T("DELETE"),-1,0) == 0)
	{
		xh = _oss_delete_open(url, acc_id, acc_key, out_code, out_msg);
		if (!xh)
		{
			return 0;
		}

		rt = _oss_delete_send(xh, out_code, out_msg);
		if (!rt)
		{
			_oss_delete_close(xh);
			return 0;
		}

		rt = _oss_delete_response(xh, out_code, out_msg, out_type, out_size);
		
		_oss_delete_recv(xh, out_code, out_msg, out_buffer, out_bytes);
		_oss_delete_close(xh);

		return rt;
	}

	return 0;
}


#endif //XDK_SUPPORT_SOCK
