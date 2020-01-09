/***********************************************************************
	Easily oss aliyun

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc oss aliyun document

	@module	oss_aliyun.c | oss aliyun implement file

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

#include "osspro.h"

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
	ucs_to_utf8(access_key, -1, acc_buf, acc_len);
#else
	mbs_to_utf8(access_key, -1, acc_buf, acc_len);
#endif

#ifdef _UNICODE
	utf8_len = ucs_to_utf8(verb, -1, NULL, MAX_LONG) + 1 \
		+ ucs_to_utf8(content_md5, -1, NULL, MAX_LONG) + 1 \
		+ ucs_to_utf8(content_type, -1, NULL, MAX_LONG) + 1 \
		+ ucs_to_utf8(gmt_time, -1, NULL, MAX_LONG) + 1 \
		+ ucs_to_utf8(oss_head, -1, NULL, MAX_LONG) \
		+ ucs_to_utf8(oss_res, -1, NULL, MAX_LONG) + 1;
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
	pos += ucs_to_utf8(verb, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(verb, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(content_md5, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(content_md5, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(content_type, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(content_type, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(gmt_time, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(gmt_time, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(oss_head, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(oss_head, -1, utf8_buf + pos, utf8_len - pos);
#endif

#ifdef _UNICODE
	pos += ucs_to_utf8(oss_res, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(oss_res, -1, utf8_buf + pos, utf8_len - pos);
#endif

	sha1_hmac((unsigned char*)acc_buf, acc_len, (unsigned char*)utf8_buf, pos, dig);

	bas_len = xbas_encode(dig, 20, auth, max);

	xmem_free(acc_buf);
	xmem_free(utf8_buf);

	return bas_len;
}


static void _oss_bucket(tchar_t* host)
{
	int dot_count = 0;
	tchar_t* sz = (tchar_t*)host;

	while (*sz != _T('\0'))
	{
		if (*sz == _T('.'))
			dot_count++;
		sz++;
	}

	if (dot_count != 3)
	{
		host[0] = _T('\0');
		return;
	}

	sz = (tchar_t*)host;
	while (*sz != _T('.') && *sz != _T('\0'))
	{
		sz++;
	}

	*sz = _T('\0');
}

static bool_t _oss_get(const tchar_t* url,
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
	xhand_t xh = NULL;

	tchar_t gmt[DATE_LEN + 1] = { 0 };
	tchar_t bucket[PATH_LEN] = { 0 };
	tchar_t oss_res[PATH_LEN] = { 0 };
	tchar_t auth[PATH_LEN] = { 0 };

	xdate_t dt = { 0 };

	int res_len;

	TRY_CATCH;

	xh = xhttp_client(_T("GET"), url);
	if (!xh)
	{
		raise_user_error(NULL, NULL);
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

	xscat(auth, _T("OSS "));
	xscat(auth, acc_id);
	xscat(auth, _T(":"));

	_oss_hash(acc_key, _T("GET"), NULL, NULL, gmt, NULL, oss_res, auth + xslen(auth), 30);

	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, auth, -1);

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_get_response_code(xh, out_code);
	xhttp_get_response_message(xh, out_msg, ERR_LEN);

	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTTYPE, -1, out_type, RES_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, out_size, NUM_LEN);

	if (!xhttp_recv_full(xh, out_buffer, out_bytes))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_close(xh);
	xh = NULL;

	END_CATCH;

	return 1;
ONERROR:
	get_last_error(out_code, out_msg, ERR_LEN);

	if (xh)
		xhttp_close(xh);

	return 0;
}

static bool_t _oss_put(const tchar_t* url,
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
	xhand_t xh = NULL;

	tchar_t gmt[DATE_LEN + 1] = { 0 };
	tchar_t bucket[PATH_LEN] = { 0 };
	tchar_t oss_res[PATH_LEN] = { 0 };
	tchar_t auth[PATH_LEN] = { 0 };

	dword_t res_len;

	xdate_t dt = { 0 };
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	TRY_CATCH;

	xh = xhttp_client(_T("PUT"), url);
	if (!xh)
	{
		raise_user_error(NULL, NULL);
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

	xscat(auth, _T("OSS "));
	xscat(auth, acc_id);
	xscat(auth, _T(":"));
	_oss_hash(acc_key, _T("PUT"), NULL, NULL, gmt, NULL, oss_res, auth + xslen(auth), 30);

	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, auth, -1);

	xsprintf(sz_size, _T("%d"), in_size);

	xhttp_set_request_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, sz_size, -1);

	if (!xhttp_send_full(xh, (byte_t*)in_data, in_size))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_get_response_code(xh, out_code);
	xhttp_get_response_message(xh, out_msg, ERR_LEN);

	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTTYPE, -1, out_type, RES_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, out_size, NUM_LEN);

	if (!xhttp_recv_full(xh, out_buffer, out_bytes))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_close(xh);
	xh = NULL;

	END_CATCH;

	return 1;
ONERROR:
	get_last_error(out_code, out_msg, ERR_LEN);

	if (xh)
		xhttp_close(xh);

	return 0;
}

static bool_t _oss_delete(const tchar_t* url,
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
	xhand_t xh = NULL;

	tchar_t gmt[DATE_LEN + 1] = { 0 };
	tchar_t bucket[PATH_LEN] = { 0 };
	tchar_t oss_res[PATH_LEN] = { 0 };
	tchar_t auth[PATH_LEN] = { 0 };

	dword_t res_len;

	xdate_t dt = { 0 };

	TRY_CATCH;

	xh = xhttp_client(_T("DELETE"), url);
	if (!xh)
	{
		raise_user_error(NULL, NULL);
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

	xscat(auth, _T("OSS "));
	xscat(auth, acc_id);
	xscat(auth, _T(":"));

	_oss_hash(acc_key, _T("DELETE"), NULL, NULL, gmt, NULL, oss_res, auth + xslen(auth), 30);

	xhttp_set_request_header(xh, HTTP_HEADER_AUTHORIZATION, -1, auth, -1);

	if (!xhttp_send_request(xh))
	{
		raise_user_error(NULL, NULL);
	}

	if (!xhttp_recv_response(xh))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_get_response_code(xh, out_code);
	xhttp_get_response_message(xh, out_msg, ERR_LEN);

	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTTYPE, -1, out_type, RES_LEN);
	xhttp_get_response_header(xh, HTTP_HEADER_CONTENTLENGTH, -1, out_size, NUM_LEN);

	if (!xhttp_recv_full(xh, out_buffer, out_bytes))
	{
		raise_user_error(NULL, NULL);
	}

	xhttp_close(xh);
	xh = NULL;

	END_CATCH;

	return 1;
ONERROR:
	get_last_error(out_code, out_msg, ERR_LEN);

	if (xh)
		xhttp_close(xh);

	return 0;
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

/******************************************************************************************************/
typedef struct _oss_t{
	xhand_head head;		//reserved for xhand_t

	int fmode;
	tchar_t path[PATH_LEN];
	secu_desc_t fsecu;
}oss_t;

xhand_t oss_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode)
{
	oss_t* pfn;

	if (is_null(fname))
		return NULL;

	pfn = (oss_t*)xmem_alloc(sizeof(oss_t));
	pfn->head.tag = _HANDLE_INET;

	pfn->fmode = mode;
	xsncpy(pfn->path, fname, PATH_LEN);

	if (psd)
	{
		xsncpy(pfn->fsecu.scr_uid, psd->scr_uid, META_LEN);
		xsncpy(pfn->fsecu.scr_key, psd->scr_key, KEY_LEN);
	}

	return (xhand_t)pfn;
}

bool_t oss_read_file(xhand_t inet, byte_t* buf, dword_t* pb)
{
	oss_t* pfn = (oss_t*)inet;

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

	pbuf = bytes_alloc();

	if (!_oss_get(sz_url, pfn->fsecu.scr_uid, pfn->fsecu.scr_key, NULL, 0, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	size = (size < *pb) ? size : *pb;
	xmem_copy((void*)buf, (void*)*pbuf, size);
	*pb = size;

	bytes_free(pbuf);

	return 1;
}

bool_t oss_write_file(xhand_t inet, const byte_t* buf, dword_t* pb)
{
	oss_t* pfn = (oss_t*)inet;

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

	pbuf = bytes_alloc();

	if (!_oss_put(sz_url, pfn->fsecu.scr_uid, pfn->fsecu.scr_key, buf, *pb, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	bytes_free(pbuf);

	return 1;
}

void oss_close_file(xhand_t inet)
{
	oss_t* pfn = (oss_t*)inet;

	XDL_ASSERT(pfn && pfn->head.tag == _HANDLE_INET);

	xmem_free(pfn);
}

bool_t oss_delete_file(const secu_desc_t* psd, const tchar_t* fname)
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

	pbuf = bytes_alloc();

	if (!_oss_delete(sz_url, ((psd) ? psd->scr_uid : NULL), ((psd) ? psd->scr_key : NULL), NULL, 0, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	bytes_free(pbuf);

	return 1;
}

static void _oss_file_info(const file_info_t* pfi, void* pa)
{
	xmem_copy(pa, (void*)pfi, sizeof(file_info_t));
}

bool_t oss_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
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

	pbuf = bytes_alloc();

	if (!_oss_get(sz_url, ((psd) ? psd->scr_uid : NULL), ((psd) ? psd->scr_key : NULL), NULL, 0, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	rt = _parse_oss_result(*pbuf, size, _oss_file_info, (void*)&fi);

	bytes_free(pbuf);

	if (ftime)
		format_gmttime(&fi.write_time, ftime);
	if (fsize)
		format_longlong(fi.high_size, fi.low_size, fsize);
	if (fetag)
		xsncpy(fetag, fi.file_etag, ETAG_LEN);

	return rt;
}

bool_t oss_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa)
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

	pbuf = bytes_alloc();

	if (!_oss_get(sz_url, ((psd) ? psd->scr_uid : NULL), ((psd) ? psd->scr_key : NULL), NULL, 0, sz_code, sz_err, sz_type, sz_size, pbuf, &size))
	{
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);
		return 0;
	}

	if (!IS_XHTTP_SUCCEED(sz_code))
	{
		if (size)
		{
			_parse_oss_error(*pbuf, size, sz_code, sz_err);
		}
		bytes_free(pbuf);

		set_last_error(sz_code, sz_err, -1);

		return 0;
	}

	rt = _parse_oss_result(*pbuf, size, pf, pa);

	bytes_free(pbuf);

	return rt;
}

static void _list_file(const file_info_t* pfi, void* pa)
{
	link_t_ptr ptr = (link_t_ptr)pa;
	link_t_ptr nlk;

	if (pfi->is_dir)
		nlk = insert_list_directory_item(ptr, LINK_LAST);
	else
		nlk = insert_list_file_item(ptr, LINK_LAST);

	set_list_item_file_info(nlk, pfi);
}

bool_t oss_list(const secu_desc_t* psd, const tchar_t* path, link_t_ptr ptr)
{
	if (is_null(path))
		return 0;

	return oss_list_file(psd, path, _list_file, (void*)ptr);
}

