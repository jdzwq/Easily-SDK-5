// ossapi.cpp : 定义 DLL 应用程序的导出函数。
//

#include <xdl.h>

#include "oss_ali.h"


static int _oss_hash(
	const wchar_t* access_key,
	const wchar_t* verb,
	const wchar_t* content_md5,
	const wchar_t* content_type,
	const wchar_t* gmt_time,
	const wchar_t* oss_head,
	const wchar_t* oss_res,
	wchar_t* auth, 
	int max)
{
	int acc_len = utf8_encode(access_key,-1,NULL,MAX_LONG);

	byte_t* acc_buf = (byte_t*)xmem_alloc(acc_len + 1);
	utf8_encode(access_key,-1,acc_buf,acc_len);

	int utf8_len = utf8_encode(verb,-1,NULL,MAX_LONG) + 1 \
		+ utf8_encode(content_md5,-1,NULL,MAX_LONG) + 1 \
		+ utf8_encode(content_type,-1,NULL,MAX_LONG) + 1 \
		+ utf8_encode(gmt_time,-1,NULL,MAX_LONG) + 1 \
		+ utf8_encode(oss_head,-1,NULL,MAX_LONG) \
		+ utf8_encode(oss_res,-1,NULL,MAX_LONG) + 1;
	
	byte_t* utf8_buf = (byte_t*)xmem_alloc(utf8_len);

	int pos = 0;
	
	pos += utf8_encode(verb,-1,utf8_buf + pos,utf8_len - pos);
	utf8_buf[pos++] = '\n';

	pos += utf8_encode(content_md5,-1,utf8_buf + pos,utf8_len - pos);
	utf8_buf[pos++] = '\n';

	pos += utf8_encode(content_type,-1,utf8_buf + pos,utf8_len - pos);
	utf8_buf[pos++] = '\n';

	pos += utf8_encode(gmt_time,-1,utf8_buf + pos,utf8_len - pos);
	utf8_buf[pos++] = '\n';

	pos += utf8_encode(oss_head,-1,utf8_buf + pos,utf8_len - pos);

	pos += utf8_encode(oss_res,-1,utf8_buf + pos,utf8_len - pos);

	unsigned char dig[20] = {0};
	xsha1_hash_bytes((unsigned char*)acc_buf,acc_len,(unsigned char*)utf8_buf,pos,dig);

	int bas_len = w_base64_encode(dig,20,auth,max);

	xmem_free(acc_buf);
	xmem_free(utf8_buf);

	return bas_len;
}

static void _oss_bucket(wchar_t* host)
{
	int dot_count = 0;
	wchar_t* sz = (wchar_t*)host;
	while(*sz != '\0')
	{
		if(*sz == L'.')
			dot_count ++;
		sz ++;
	}

	if(dot_count != 3)
	{
		host[0] = L'\0';
		return ;
	}

	sz = (wchar_t*)host;
	while(*sz != L'.' && *sz != L'\0')
	{
		sz ++;
	}

	*sz = L'\0';
}

static void _oss_object(const wchar_t* obj, int* res_at, int* res_len, int* ext_at, int* ext_len)
{
	*res_at = *res_len = 0;
	*ext_at = *ext_len = 0;

	int len = xslen(obj);
	if (!len)
		return;

	wchar_t* token = (wchar_t*)(obj + len);
	len = 0;
	while (*token != L'.' && *token != L'/' && token != obj)
	{
		token--;
		len++;
	}

	if (*token == L'.')
	{
		*ext_at = token - obj;
		*ext_len = len;
	}

	while (*token != L'/' && token != obj)
	{
		token--;
	}

	len = 0;
	while (token != obj)
	{
		token--;
		len++;
	}

	*res_at = token - obj + 1;
	*res_len = len;
}

bool_t _oss_head(
	const wchar_t* url,
	const wchar_t* acc_id,
	const wchar_t* acc_key,
	wchar_t* code,
	wchar_t* msg,
	wchar_t* out_type,
	byte_t* buf,
	int* max)
{
	xhand_t xh = xhttp_open(NULL, L"GET", url);
	if (!xh)
	{
		return 0;
	}

	wchar_t sz_object[PATH_LEN] = { 0 };
	xhttp_get_url_object(xh, sz_object, PATH_LEN);

	wchar_t sz_res[PATH_LEN] = { 0 };
	wchar_t sz_ext[RES_LEN] = { 0 };
	int res_at, res_len, ext_at, ext_len;

	_oss_object(sz_object, &res_at, &res_len, &ext_at, &ext_len);

	xsncpy(sz_res, sz_object + res_at, res_len);
	xsncpy(sz_ext, sz_object + ext_at, ext_len);

	wchar_t oss_res[1024] = { 0 };
	if (is_null(sz_res))
		xscpy(oss_res, L"/");
	else
		xsprintf(oss_res, L"/?prefix=%s&delimiter=/", sz_res);

	wchar_t auth[1024] = { 0 };
	xscat(auth, L"OSS ");
	xscat(auth, acc_id);
	xscat(auth, L":");

	wchar_t gmt[50];
	get_gmt_time(gmt);

	_oss_hash(acc_key, L"GET", NULL, NULL, gmt, NULL, oss_res, auth + xslen(auth), 30);

	xhttp_set_request_default_header(xh);
	xhttp_set_request_header(xh, L"Date", -1, gmt, -1);
	xhttp_set_request_header(xh, L"Authorization", -1, auth, -1);

	int bytes = 0;

	bytes = xhttp_send(xh, NULL, 0);
	if (bytes < 0)
	{
		xhttp_close(xh);
		return 0;
	}

	bytes = xhttp_recv(xh, NULL, 0);
	if (bytes < 0)
	{
		xhttp_close(xh);
		return 0;
	}

	xhttp_get_response_code(xh, code);
	xhttp_get_response_message(xh, msg, MAX_ERR);

	xhttp_get_response_header(xh, L"Content-Type", -1, out_type, RES_LEN);

	wchar_t sz_len[NUM_LEN + 1] = { 0 };
	xhttp_get_response_header(xh, L"Content-Length", -1, sz_len, NUM_LEN);
	int slen = xstol(sz_len);
	slen = (slen < *max) ? slen : *max;

	int len, pos = 0;
	do{
		len = xhttp_recv(xh, buf + pos, slen - pos);
		if (len <= 0)
			break;

		pos += len;

		if (pos == slen)
		{
			break;
		}
	} while (1);

	xhttp_close(xh);

	*max = pos;

	return 1;
}

bool_t _oss_get(
	const wchar_t* url,
	const wchar_t* acc_id,
	const wchar_t* acc_key,
	wchar_t* code,
	wchar_t* msg,
	wchar_t* out_type,
	byte_t* buf,
	int* max)
{
	xhand_t xh = xhttp_open(NULL,L"GET",url);
	if (!xh)
	{
		return 0;
	}

	wchar_t gmt[50];
	get_gmt_time(gmt);

	wchar_t bucket[1024] = {0};
	xhttp_get_url_host(xh,bucket,1024);
	
	_oss_bucket(bucket);

	wchar_t oss_res[1024] = {0};
	if(!is_null(bucket))
	{
		xscat(oss_res,L"/");
		xscat(oss_res,bucket);
	}

	int res_len = xslen(oss_res);
	xhttp_get_url_object(xh,oss_res + res_len,1024 - res_len);

	wchar_t auth[1024] = {0};
	xscat(auth,L"OSS ");
	xscat(auth,acc_id);
	xscat(auth,L":");

	_oss_hash(acc_key,L"GET",NULL,NULL,gmt,NULL,oss_res,auth + xslen(auth),30);

	wchar_t host[1024] = {0};
	wchar_t port[10] = {0};
	xhttp_get_url_host(xh,host,1024);
	xhttp_get_url_port(xh,port,10);

	if(!is_null(port) && xstos(port) != 80)
	{
		xscat(host,L":");
		xscat(host,port);
	}

	xhttp_set_request_header(xh,L"Host",-1,host,-1);
	xhttp_set_request_header(xh,L"Date",-1,gmt,-1);
	xhttp_set_request_header(xh,L"Authorization",-1,auth,-1);
	
	int bytes = 0;

	bytes = xhttp_send(xh,NULL,0);
	if (bytes < 0)
	{
		xhttp_close(xh);
		return 0;
	}

	bytes = xhttp_recv(xh, NULL, 0);
	if (bytes < 0)
	{
		xhttp_close(xh);
		return 0;
	}

	xhttp_get_response_code(xh,code);
	xhttp_get_response_message(xh,msg,MAX_ERR);

	xhttp_get_response_header(xh, L"Content-Type", -1, out_type, RES_LEN);

	wchar_t sz_len[NUM_LEN + 1] = {0};
	xhttp_get_response_header(xh,L"Content-Length",-1,sz_len,NUM_LEN);
	int slen = xstol(sz_len);
	slen = (slen < *max) ? slen : *max;

	int len,pos = 0;
	do{
		len = xhttp_recv(xh,buf + pos,slen - pos);
		if(len <= 0)
			break;

		pos += len;

		if(pos == slen)
		{
			break;
		}
	}while(1);

	xhttp_close(xh);

	*max = pos;

	return 1;
}

bool_t _oss_put(
	const wchar_t* url,
	const wchar_t* acc_id,
	const wchar_t* acc_key,
	const wchar_t* in_type,
	const byte_t* data,
	int size,
	wchar_t* code,
	wchar_t* msg,
	wchar_t* out_type,
	byte_t* buf,
	int* max)
{
	xhand_t xh = xhttp_open(NULL,L"PUT",url);
	if (!xh)
	{
		return 0;
	}

	wchar_t gmt[50];
	get_gmt_time(gmt);

	wchar_t bucket[1024] = {0};
	xhttp_get_url_host(xh,bucket,1024);
	_oss_bucket(bucket);

	wchar_t oss_res[1024] = {0};
	if(!is_null(bucket))
	{
		xscat(oss_res,L"/");
		xscat(oss_res,bucket);
	}

	int res_len = xslen(oss_res);
	xhttp_get_url_object(xh,oss_res + res_len,1024 - res_len);

	wchar_t auth[1024] = {0};
	xscat(auth,L"OSS ");
	xscat(auth,acc_id);
	xscat(auth,L":");
	_oss_hash(acc_key,L"PUT",NULL,in_type,gmt,NULL,oss_res,auth + xslen(auth),30);

	wchar_t host[1024] = {0};
	wchar_t port[10] = {0};
	xhttp_get_url_host(xh,host,1024);
	xhttp_get_url_port(xh,port,10);

	if(!is_null(port) && xstos(port) != 80)
	{
		xscat(host,L":");
		xscat(host,port);
	}

	wchar_t sz_size[NUM_LEN + 1] = {0};
	xsprintf(sz_size,L"%d",size);

	xhttp_set_request_header(xh,L"Host",-1,host,-1);
	xhttp_set_request_header(xh,L"Date",-1,gmt,-1);
	if (!is_null(in_type))
		xhttp_set_request_header(xh,L"Content-Type",-1,in_type,-1);
	if (data && size > 0)
		xhttp_set_request_header(xh,L"Content-Length",-1,sz_size,-1);
	xhttp_set_request_header(xh,L"Authorization",-1,auth,-1);
	
	int bytes = xhttp_send(xh,(byte_t*)data,size);
	if(bytes < size)
	{
		xhttp_close(xh);
		return 0;
	}

	bytes = xhttp_recv(xh, NULL, 0);
	if (bytes < 0)
	{
		xhttp_close(xh);
		return 0;
	}

	xhttp_get_response_code(xh,code);
	xhttp_get_response_message(xh,msg,MAX_ERR);

	xhttp_get_response_header(xh, L"Content-Type", -1, out_type, RES_LEN);

	wchar_t sz_len[NUM_LEN + 1] = {0};
	xhttp_get_response_header(xh,L"Content-Length",-1,sz_len,NUM_LEN);
	int slen = xstol(sz_len);
	slen = (slen < *max) ? slen : *max;

	int len,pos = 0;
	do{
		len = xhttp_recv(xh,buf + pos,slen - pos);
		if(len <= 0)
			break;

		pos += len;

		if(pos == slen)
		{
			break;
		}
	}while(1);

	xhttp_close(xh);

	*max = pos;

	return 1;
}

bool_t _oss_delete(
	const wchar_t* url,
	const wchar_t* acc_id,
	const wchar_t* acc_key,
	wchar_t* code,
	wchar_t* msg,
	wchar_t* out_type,
	byte_t* buf,
	int* max)
{
	xhand_t xh = xhttp_open(NULL,L"DELETE",url);
	if (!xh)
	{
		return 0;
	}

	wchar_t gmt[50];
	get_gmt_time(gmt);

	wchar_t bucket[1024] = {0};
	xhttp_get_url_host(xh,bucket,1024);
	_oss_bucket(bucket);

	wchar_t oss_res[1024] = {0};
	if(!is_null(bucket))
	{
		xscat(oss_res,L"/");
		xscat(oss_res,bucket);
	}

	int res_len = xslen(oss_res);
	xhttp_get_url_object(xh,oss_res + res_len,1024 - res_len);

	wchar_t auth[1024] = {0};
	xscat(auth,L"OSS ");
	xscat(auth,acc_id);
	xscat(auth,L":");

	_oss_hash(acc_key,L"DELETE",NULL,NULL,gmt,NULL,oss_res,auth + xslen(auth),30);

	wchar_t host[1024] = {0};
	wchar_t port[10] = {0};
	xhttp_get_url_host(xh,host,1024);
	xhttp_get_url_port(xh,port,10);

	if(!is_null(port) && xstos(port) != 80)
	{
		xscat(host,L":");
		xscat(host,port);
	}

	xhttp_set_request_header(xh,L"Host",-1,host,-1);
	xhttp_set_request_header(xh,L"Date",-1,gmt,-1);
	xhttp_set_request_header(xh,L"Authorization",-1,auth,-1);
	
	int bytes = 0;

	bytes = xhttp_send(xh,NULL,0);
	if (bytes < 0)
	{
		xhttp_close(xh);
		return 0;
	}

	bytes = xhttp_recv(xh, NULL, 0);
	if (bytes < 0)
	{
		xhttp_close(xh);
		return 0;
	}

	xhttp_get_response_code(xh,code);
	xhttp_get_response_message(xh,msg,MAX_ERR);

	xhttp_get_response_header(xh, L"Content-Type", -1, out_type, RES_LEN);

	wchar_t sz_len[NUM_LEN + 1] = {0};
	xhttp_get_response_header(xh,L"Content-Length",-1,sz_len,NUM_LEN);
	int slen = xstol(sz_len);
	slen = (slen < *max) ? slen : *max;

	int len,pos = 0;
	do{
		len = xhttp_recv(xh,buf + pos,slen - pos);
		if(len <= 0)
			break;

		pos += len;

		if(pos == slen)
		{
			break;
		}
	}while(1);

	xhttp_close(xh);

	*max = pos;

	return 1;
}


bool_t __stdcall oss_ioctl(
	const wchar_t* ioop,
	const wchar_t* url,
	const wchar_t* acc_id,
	const wchar_t* acc_key,
	const wchar_t* in_type,
	const byte_t* in_data, 
	int in_size, 
	wchar_t* out_code,
	wchar_t* out_msg,
	wchar_t* out_type,
	byte_t* out_buf,
	int* out_max)
{
	if(compare_text(ioop, -1, L"HEAD", -1, 0) == 0)
		return _oss_head(url, acc_id, acc_key, out_code, out_msg, out_type, out_buf, out_max);
	else if(compare_text(ioop,-1,L"GET",-1,0) == 0)
		return _oss_get(url,acc_id,acc_key,out_code,out_msg,out_type,out_buf,out_max);
	else if(compare_text(ioop,-1,L"PUT",-1,0) == 0)
		return _oss_put(url, acc_id, acc_key, in_type, in_data, in_size, out_code, out_msg, out_type,out_buf, out_max);
	else if(compare_text(ioop,-1,L"DELETE",-1,0) == 0)
		return _oss_delete(url,acc_id,acc_key,out_code,out_msg,out_type,out_buf,out_max);

	return 0;
}