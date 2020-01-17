/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl http document

	@module	nethttp.c | implement file

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

#include "nethttp.h"
#include "xdlinit.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdldoc.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)

typedef struct _xhttp_t{
	xhand_head head;		//head for xhand_t
	int type;

	int secu;
	
	if_bio_t inf;

	stream_t send_stream;
	stream_t recv_stream;

	link_t_ptr st_request,st_response;
	bool_t b_request,b_response;

	unsigned short port;
	tchar_t addr[ADDR_LEN + 1];

	tchar_t method[RES_LEN + 1];
	tchar_t poto[RES_LEN + 1];
	tchar_t version[NUM_LEN + 1];
	tchar_t host[RES_LEN + 1];
	tchar_t object[PATH_LEN + 1];

	byte_t *query;
	dword_t len_query;

	tchar_t code[NUM_LEN + 1];
	tchar_t text[ERR_LEN + 1];
}xhttp_t;

#define IS_XHTTP_METHOD(token)		(a_xsnicmp(token,"GET",3) == 0 ||  a_xsnicmp(token,"PUT",3) == 0 || a_xsnicmp(token,"POST",4) == 0 || a_xsnicmp(token,"DELETE",6) == 0 || a_xsnicmp(token,"HEAD",4) == 0 || a_xsnicmp(token,"LIST",4) == 0 || a_xsnicmp(token,"GRANT",5) == 0) 

/***********************************************************************************************/
static void _xhttp_parse_request(xhttp_t* phttp, byte_t* data, int len)
{
	byte_t *token = data;
	byte_t *key, *val;
	int klen, vlen;
	tchar_t *tkey, *tval;
	int tlen;

	//get method
	klen = 0;
	key = token;
	while (*token != ' ' && *token != '\0')
	{
		token++;
		klen++;
	}
#ifdef _UNICODE
	utf8_to_ucs(key, klen, phttp->method, RES_LEN);
#else
	utf8_to_mbs(key, klen, phttp->method, RES_LEN);
#endif

	if (data + len < token)
		return;

	//skip black
	while (*token == ' ')
		token++;

	//get object
	klen = 0;
	key = token;
	while (*token != ' ' && *token != '?' && *token != '\0')
	{
		token++;
		klen++;
	}
#ifdef _UNICODE
	utf8_to_ucs(key, klen, phttp->object, RES_LEN);
#else
	utf8_to_mbs(key, klen, phttp->object, RES_LEN);
#endif

	if (data + len < token)
		return;

	//get query
	if (*token == _T('?'))
	{
		//skip '?'
		token++;

		key = token;
		klen = 0;
		while (*token != ' ' && *token != '\0')
		{
			token++;
			klen++;
		}

		phttp->len_query = klen;
		phttp->query = (byte_t*)xmem_realloc(phttp->query, phttp->len_query + 1);
		xmem_copy((void*)phttp->query, (void*)key, phttp->len_query);
	}

	//skip black
	while (*token == ' ')
		token++;

	//get proto
	klen = 0;
	key = token;
	while (*token != '/' && *token != '\0')
	{
		token++;
		klen++;
	}
#ifdef _UNICODE
	utf8_to_ucs(key, klen, phttp->poto, RES_LEN);
#else
	utf8_to_mbs(key, klen, phttp->poto, RES_LEN);
#endif

	if (data + len < token)
		return;

	if (*token != '\0')
		token++;

	//get version
	klen = 0;
	key = token;
	while (*token != '\r' && *token != '\0')
	{
		token++;
		klen++;
	}
#ifdef _UNICODE
	utf8_to_ucs(key, klen, phttp->version, NUM_LEN);
#else
	utf8_to_mbs(key, klen, phttp->version, NUM_LEN);
#endif

	if (data + len < token)
		return;

	//skip '\r\n'
	if (*token == '\r')
		token++;
	if (*token == '\n')
		token++;

	//parse key and val
	while (1)
	{
		if (*token == '\r' || *token == '\n' || *token == '\0')
			break;

		key = token;
		klen = 0;
		while (*token != ':' && *token != '\0')
		{
			token++;
			klen++;
		}
		//skip ':'
		if (*token != '\0')
			token++;

		//skip black
		while (*token == ' ')
			token++;

		val = token;
		vlen = 0;
		while (*token != '\r' && *token != '\n' && *token != '\0')
		{
			token++;
			vlen++;
		}
#ifdef _UNICODE
		tlen = utf8_to_ucs(key, klen, NULL, MAX_LONG);
		tkey = xsalloc(tlen + 1);
		utf8_to_ucs(key, klen, tkey, tlen);

		tlen = utf8_to_ucs(val, vlen, NULL, MAX_LONG);
		tval = xsalloc(tlen + 1);
		utf8_to_ucs(val, vlen, tval, tlen);

		write_string_entity(phttp->st_request, tkey, -1, tval, -1);

		xsfree(tkey);
		xsfree(tval);
#else
		tlen = utf8_to_mbs(key, klen, NULL, MAX_LONG);
		tkey = xsalloc(tlen + 1);
		utf8_to_mbs(key, klen, tkey, tlen);

		tlen = utf8_to_mbs(val, vlen, NULL, MAX_LONG);
		tval = xsalloc(tlen + 1);
		utf8_to_mbs(val, vlen, tval, tlen);

		write_string_entity(phttp->st_request, tkey, -1, tval, -1);

		xsfree(tkey);
		xsfree(tval);
#endif
		//skip '\r\n'
		if (*token == '\r')
			token++;
		if (*token == '\n')
			token++;

		if (data + len < token)
			return;
	}
}

static dword_t _xhttp_format_request(xhttp_t* phttp, byte_t* buf, dword_t max)
{
	dword_t total = 0;
	link_t_ptr ent;
	const tchar_t *key, *val;

	//method
#ifdef _UNICODE
	total += ucs_to_utf8(phttp->method, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->method, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = ' ';
	}
	total++;
	if (total >= max)
		return total;

	//object
#ifdef _UNICODE
	total += ucs_to_utf8(phttp->object, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->object, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	//query
	if (phttp->len_query)
	{
		if (buf)
		{
			buf[total] = '?';
		}
		total++;
		if (total >= max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)phttp->query, phttp->len_query);
		}
		total += phttp->len_query;
		if (total >= max)
			return total;
	}

	if (buf)
	{
		buf[total] = ' ';
	}
	total++;
	if (total >= max)
		return total;

	//proto
#ifdef _UNICODE
	total += ucs_to_utf8(phttp->poto, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->poto, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = '/';
	}
	total++;
	if (total >= max)
		return total;

	//version
#ifdef _UNICODE
	total += ucs_to_utf8(phttp->version, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->version, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = '\r';
		buf[total + 1] = '\n';
	}
	total += 2;
	if (total >= max)
		return total;

	//entity
	ent = get_string_next_entity(phttp->st_request, LINK_FIRST);
	while (ent)
	{
		key = get_string_entity_key_ptr(ent);
		val = get_string_entity_val_ptr(ent);

#ifdef _UNICODE
		total += ucs_to_utf8(key, -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf8(key, -1, ((buf) ? buf + total : NULL), max - total);
#endif
		if (total >= max)
			return total;

		if (buf)
		{
			buf[total] = ':';
			buf[total + 1] = ' ';
		}
		total += 2;
		if (total >= max)
			return total;

#ifdef _UNICODE
		total += ucs_to_utf8(val, -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf8(val, -1, ((buf) ? buf + total : NULL), max - total);
#endif
		if (total >= max)
			return total;

		if (buf)
		{
			buf[total] = '\r';
			buf[total + 1] = '\n';
		}
		total += 2;
		if (total >= max)
			return total;

		ent = get_string_next_entity(phttp->st_request, ent);
	}

	if (buf)
	{
		buf[total] = '\r';
		buf[total + 1] = '\n';
	}
	total += 2;

	return total;
}

static void _xhttp_parse_response(xhttp_t* phttp, byte_t* data, dword_t len)
{
	byte_t *token = data;
	byte_t *key, *val;
	int klen, vlen;
	tchar_t *tkey, *tval;
	int tlen;

	//get proto
	klen = 0;
	key = token;
	while (*token != '/' && *token != '\0')
	{
		token++;
		klen++;
	}
#ifdef _UNICODE
	utf8_to_ucs(key, klen, phttp->poto, RES_LEN);
#else
	utf8_to_mbs(key, klen, phttp->poto, RES_LEN);
#endif

	if (data + len < token)
		return;

	//skip '/'
	if (*token != '\0')
		token++;

	//get version
	klen = 0;
	key = token;
	while (*token != ' ' && *token != '\0')
	{
		token++;
		klen++;
	}
#ifdef _UNICODE
	utf8_to_ucs(key, klen, phttp->version, NUM_LEN);
#else
	utf8_to_mbs(key, klen, phttp->version, NUM_LEN);
#endif

	if (data + len < token)
		return;

	//skip black
	while (*token == ' ')
		token++;

	//get code
	klen = 0;
	key = token;
	while (*token != ' ' && *token != '\0')
	{
		token++;
		klen++;
	}
#ifdef _UNICODE
	utf8_to_ucs(key, klen, phttp->code, NUM_LEN);
#else
	utf8_to_mbs(key, klen, phttp->code, NUM_LEN);
#endif

	if (data + len < token)
		return;

	//skip black
	while (*token == ' ')
		token++;

	//get text
	if (*token != '\r' && *token != '\0')
	{
		vlen = 0;
		val = token;
		while (*token != '\r' && *token != '\0')
		{
			token++;
			vlen++;
		}
#ifdef _UNICODE
		utf8_to_ucs(val, vlen, phttp->text, ERR_LEN);
#else
		utf8_to_mbs(val, vlen, phttp->text, ERR_LEN);
#endif
	}

	//skip '\r\n'
	if (*token == '\r')
		token++;
	if (*token == '\n')
		token++;

	if (data + len < token)
		return;

	//parse key and val
	while (1)
	{
		if (*token == '\r' || *token == '\n' || *token == '\0')
			break;

		key = token;
		klen = 0;
		while (*token != ':' && *token != '\0')
		{
			token++;
			klen++;
		}
		//skip ':'
		if (*token != '\0')
			token++;

		//skip black
		while (*token == ' ')
			token++;

		val = token;
		vlen = 0;
		while (*token != '\r' && *token != '\n' && *token != '\0')
		{
			token++;
			vlen++;
		}

#ifdef _UNICODE
		tlen = utf8_to_ucs(key, klen, NULL, MAX_LONG);
		tkey = xsalloc(tlen + 1);
		utf8_to_ucs(key, klen, tkey, tlen);

		tlen = utf8_to_ucs(val, vlen, NULL, MAX_LONG);
		tval = xsalloc(tlen + 1);
		utf8_to_ucs(val, vlen, tval, tlen);

		write_string_entity(phttp->st_response, tkey, -1, tval, -1);

		xsfree(tkey);
		xsfree(tval);
#else
		tlen = utf8_to_mbs(key, klen, NULL, MAX_LONG);
		tkey = xsalloc(tlen + 1);
		utf8_to_mbs(key, klen, tkey, tlen);

		tlen = utf8_to_mbs(val, vlen, NULL, MAX_LONG);
		tval = xsalloc(tlen + 1);
		utf8_to_mbs(val, vlen, tval, tlen);

		write_string_entity(phttp->st_response, tkey, -1, tval, -1);

		xsfree(tkey);
		xsfree(tval);
#endif
		//skip '\r\n'
		if (*token == '\r')
			token++;
		if (*token == '\n')
			token++;

		if (data + len < token)
			return;
	}
}

static dword_t _xhttp_format_response(xhttp_t* phttp, byte_t* buf, dword_t max)
{
	dword_t total = 0;
	link_t_ptr ent;
	const tchar_t *key, *val;

#ifdef _UNICODE
	total += ucs_to_utf8(phttp->poto, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->poto, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = '/';
	}
	total++;
	if (total >= max)
		return total;

#ifdef _UNICODE
	total += ucs_to_utf8(phttp->version, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->version, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = ' ';
	}
	total++;
	if (total >= max)
		return total;

#ifdef _UNICODE
	total += ucs_to_utf8(phttp->code, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->code, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = ' ';
	}
	total++;
	if (total >= max)
		return total;

#ifdef _UNICODE
	total += ucs_to_utf8(phttp->text, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->text, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = '\r';
		buf[total + 1] = '\n';
	}
	total += 2;
	if (total >= max)
		return total;

	ent = get_string_next_entity(phttp->st_response, LINK_FIRST);
	while (ent)
	{
		key = get_string_entity_key_ptr(ent);
		val = get_string_entity_val_ptr(ent);

#ifdef _UNICODE
		total += ucs_to_utf8(key, -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf8(key, -1, ((buf) ? buf + total : NULL), max - total);
#endif
		if (total >= max)
			return total;

		if (buf)
		{
			buf[total] = ':';
			buf[total + 1] = ' ';
		}
		total += 2;
		if (total >= max)
			return total;

#ifdef _UNICODE
		total += ucs_to_utf8(val, -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf8(val, -1, ((buf) ? buf + total : NULL), max - total);
#endif
		if (total >= max)
			return total;

		if (buf)
		{
			buf[total] = '\r';
			buf[total + 1] = '\n';
		}
		total += 2;
		if (total >= max)
			return total;

		ent = get_string_next_entity(phttp->st_response, ent);
	}

	if (buf)
	{
		buf[total] = '\r';
		buf[total + 1] = '\n';
	}
	total += 2;

	return total;
}

static int _xhttp_format_continue(xhttp_t* phttp, byte_t* buf, dword_t max)
{
	dword_t total = 0;

#ifdef _UNICODE
	total += ucs_to_utf8(phttp->poto, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->poto, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = '/';
	}
	total++;
	if (total >= max)
		return total;

#ifdef _UNICODE
	total += ucs_to_utf8(phttp->version, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(phttp->version, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = ' ';
	}
	total++;
	if (total >= max)
		return total;

#ifdef _UNICODE
	total += ucs_to_utf8(HTTP_CODE_100, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(HTTP_CODE_100, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = ' ';
	}
	total++;
	if (total >= max)
		return total;

#ifdef _UNICODE
	total += ucs_to_utf8(HTTP_CODE_100_TEXT, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(HTTP_CODE_100_TEXT, -1, ((buf) ? buf + total : NULL), max - total);
#endif
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = '\r';
		buf[total + 1] = '\n';
	}
	total += 2;
	if (total >= max)
		return total;

	if (buf)
	{
		buf[total] = '\r';
		buf[total + 1] = '\n';
	}
	total += 2;

	return total;
}

/*****************************************************************************************************/

void xhttp_split_object(const tchar_t* sz_object, tchar_t* sz_site, tchar_t* sz_file)
{
	int fat = 0;
	int lat = 0;
	int len;
	tchar_t* token;
	
	token = (tchar_t*)sz_object;
	len = 0;

	if (*token == _T('/') || *token == _T('\\'))
	{
		token++;
		len++;
	}

	while (*token != _T('/') && *token != _T('\\') && *token != _T('\0'))
	{
		token++;
		len++;
	}

	if (*token == _T('\0'))
	{
		if (sz_site)
		{
			xscpy(sz_site, _T("/"));
		}

		if (sz_file)
		{
			xscpy(sz_file, sz_object);
			if (sz_file[0] == _T('\\'))
				sz_file[0] = _T('/');
		}
		return;
	}
	
	if (sz_site)
	{
		xsncpy(sz_site, sz_object, len);
		if (sz_site[0] == _T('\\'))
			sz_site[0] = _T('/');
	}

	if (sz_file)
	{
		xsncpy(sz_file, token, PATH_LEN);
		token = sz_file;
		while (*token != _T('\0'))
		{
			if (*token == _T('\\'))
				*token = _T('/');
			token++;
		}
	}
}

int xhttp_format_content_type(const tchar_t* filter, tchar_t* buf, int max)
{
	int len;

	if (compare_text(filter, -1, _T("html"), -1, 1) == 0)
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_TEXTHTML);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_TEXTHTML, len);
		}
		return len;
	}
	else if (compare_text(filter, -1, _T("css"), -1, 1) == 0)
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_TEXTCSS);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_TEXTCSS, len);
		}
		return len;
	}
	else if (compare_text(filter, -1, _T("js"), -1, 1) == 0)
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_JAVASCRIPT);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_JAVASCRIPT, len);
		}
		return len;
	}
	else if (compare_text(filter, -1, _T("jpg"), -1, 1) == 0)
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_IMAGEJPG);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_IMAGEJPG, len);
		}
		return len;
	}
	else if (compare_text(filter, -1, _T("jpeg"), -1, 1) == 0)
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_IMAGEJPG);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_IMAGEJPG, len);
		}
		return len;
	}
	else if (compare_text(filter, -1, _T("png"), -1, 1) == 0)
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_IMAGEPNG);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_IMAGEPNG, len);
		}
		return len;
	}
	else if (compare_text(filter, -1, _T("gif"), -1, 1) == 0)
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_IMAGEGIF);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_IMAGEGIF, len);
		}
		return len;
	}
	else if (compare_text(filter, -1, _T("tiff"), -1, 1) == 0)
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_IMAGETIFF);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_IMAGETIFF, len);
		}
		return len;
	}
	else
	{
		len = xslen(HTTP_HEADER_CONTENTTYPE_TEXTPLAIN);
		len = (len < max) ? len : max;
		if (buf)
		{
			xsncpy(buf, HTTP_HEADER_CONTENTTYPE_TEXTPLAIN, len);
		}
		return len;
	}
}

xhand_t xhttp_client(const tchar_t* method,const tchar_t* url)
{
	tchar_t *potoat,*hostat,*portat,*objat,*qryat;
	int potolen,hostlen,portlen,objlen,qrylen;

	xhttp_t* phttp = NULL;

	TRY_CATCH;

	if (is_null(url))
	{
		raise_user_error(_T("xhttp_client"), _T("invalid url"));
	}

	phttp = (xhttp_t*)xmem_alloc(sizeof(xhttp_t));
	phttp->head.tag = _HANDLE_INET;

	phttp->type = _XHTTP_TYPE_CLI;
	xsncpy(phttp->method, method, RES_LEN);
	xsupr(phttp->method);

	parse_url(url,&potoat,&potolen,&hostat,&hostlen,&portat,&portlen,&objat,&objlen,&qryat,&qrylen);

	if (potolen)
	{
		xsncpy(phttp->poto, potoat, potolen);
		xsupr(phttp->poto);

		if (compare_text(phttp->poto, -1, _T("HTTPS"), -1, 0) == 0)
		{
			phttp->secu = _SECU_SSL;
		}
		else if (compare_text(phttp->poto, -1, _T("SSH"), -1, 0) == 0)
		{
			phttp->secu = _SECU_SSH;
		}
	}

	xscpy(phttp->poto, _T("HTTP"));
	xscpy(phttp->version, _T("1.1"));

	if(hostlen)
	{
		xsncpy(phttp->host, hostat, hostlen);

		if(is_ip(phttp->host))
		{
			xscpy(phttp->addr,phttp->host);
		}else
		{
			host_addr(phttp->host,phttp->addr);
		}
	}

	if(is_null(phttp->addr))
	{
		raise_user_error(_T("xhttp_client"), _T("invalid host address"));
	}

	if (portlen)
	{
		phttp->port = xsntos(portat, portlen);
	}
	else
	{
		switch (phttp->secu)
		{
		case _SECU_SSL:
			phttp->port = DEF_HTTPS_PORT;
			break;
		case _SECU_SSH:
			phttp->port = DEF_SSH_PORT;
			break;
		default:
			phttp->port = DEF_HTTP_PORT;
			break;
		}
	}

	if (objlen)
		xsncpy(phttp->object, objat, objlen);
	else
		xscpy(phttp->object,_T("/"));

	if(qrylen)
	{
		phttp->len_query = xhttp_url_encoding(qryat, qrylen, NULL, MAX_LONG);
		phttp->query = (byte_t*)xmem_alloc(phttp->len_query + 1);
		xhttp_url_encoding(qryat, qrylen, phttp->query, phttp->len_query);
	}

	switch (phttp->secu)
	{
#ifdef XDL_SUPPORT_CRYPT
	case _SECU_SSL:
		phttp->inf.bio = xssl_cli(phttp->port, phttp->addr);

		phttp->inf.pf_write = xssl_write;
		phttp->inf.pf_flush = xssl_flush;
		phttp->inf.pf_read = xssl_read;
		phttp->inf.pf_close = xssl_close;
		phttp->inf.pf_setopt = xssl_setopt;
		break;
	case _SECU_SSH:
		phttp->inf.bio = xssh_cli(phttp->port, phttp->addr);

		phttp->inf.pf_write = xssh_write;
		phttp->inf.pf_read = xssh_read;
		phttp->inf.pf_flush = xssh_flush;
		phttp->inf.pf_close = xssh_close;
		phttp->inf.pf_setopt = xssh_setopt;
		break;
#endif
	default:
		phttp->inf.bio = xtcp_cli(phttp->port, phttp->addr);

		phttp->inf.pf_write = xtcp_write;
		phttp->inf.pf_read = xtcp_read;
		phttp->inf.pf_close = xtcp_close;
		phttp->inf.pf_setopt = xtcp_set_option;
		break;
	}
	
	if(!phttp->inf.bio)
	{
		raise_user_error(_T("xhttp_client"), _T("create bio failed"));
	}

	phttp->st_request = create_string_table(ORDER_ASCEND);
	phttp->st_response = create_string_table(ORDER_ASCEND);

	END_CATCH;

	return &phttp->head;
ONERROR:
	XDL_TRACE_LAST;

	if (phttp)
	{
		xhttp_close(&phttp->head);
	}

	return NULL;
}

xhand_t xhttp_server(xhand_t bio)
{
	net_addr_t na;
	unsigned short port;
	
	xhttp_t* phttp = NULL;

	TRY_CATCH;

	if (!bio)
	{
		raise_user_error(_T("xhttp_server"), _T("invalid bio"));
	}

	phttp = (xhttp_t*)xmem_alloc(sizeof(xhttp_t));
	phttp->head.tag = _HANDLE_INET;
	phttp->type = _XHTTP_TYPE_SRV;

	phttp->inf.bio = bio;

	switch (bio->tag)
	{
	case _HANDLE_SSL:
		phttp->secu = _SECU_SSL;
		break;
	case _HANDLE_SSH:
		phttp->secu = _SECU_SSH;
		break;
	default:
		phttp->secu = 0;
		break;
	}

	switch(phttp->secu)
	{
#ifdef XDL_SUPPORT_CRYPT
	case _SECU_SSL:
		phttp->inf.pf_write = xssl_write;
		phttp->inf.pf_flush = xssl_flush;
		phttp->inf.pf_read = xssl_read;
		phttp->inf.pf_setopt = xssl_setopt;

		socket_peer(xssl_socket(bio), &na);
		break;
	case _SECU_SSH:
		phttp->inf.pf_write = xssh_write;
		phttp->inf.pf_read = xssh_read;
		phttp->inf.pf_flush = xssh_flush;
		phttp->inf.pf_setopt = xssh_setopt;

		socket_peer(xssh_socket(bio), &na);
		break;
#endif
	default:
		phttp->inf.pf_write = xtcp_write;
		phttp->inf.pf_read = xtcp_read;
		phttp->inf.pf_setopt = xtcp_set_option;

		socket_peer(xtcp_socket(bio), &na);
		break;
	}
	
	conv_addr(&na, &port, phttp->addr);
	xscpy(phttp->poto, _T("HTTP"));
	xscpy(phttp->version, _T("1.1"));

	phttp->st_request = create_string_table(ORDER_ASCEND);
	phttp->st_response = create_string_table(ORDER_ASCEND);

	END_CATCH;

	return &phttp->head;
ONERROR:
	XDL_TRACE_LAST;

	if (phttp)
	{
		xhttp_close(&phttp->head);
	}

	return NULL;
}

void xhttp_close(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_CLI || phttp->type == _XHTTP_TYPE_SRV);

	if (phttp->st_request)
		destroy_string_table(phttp->st_request);

	if (phttp->st_response)
		destroy_string_table(phttp->st_response);

	if (phttp->query)
		xmem_free(phttp->query);

	if (phttp->send_stream)
	{
		stream_free(phttp->send_stream);
	}

	if (phttp->recv_stream)
	{
		stream_free(phttp->recv_stream);
	}

	if (phttp->inf.bio && phttp->type == _XHTTP_TYPE_CLI)
	{
		if (phttp->inf.pf_close)
		{
			(*phttp->inf.pf_close)(phttp->inf.bio);
			phttp->inf.bio = NULL;
		}
	}

	xmem_free(phttp);
}


int xhttp_type(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return phttp->type;
}

int xhttp_secu(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return phttp->secu;
}

xhand_t xhttp_bio(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return phttp->inf.bio;
}

unsigned short xhttp_addr_port(xhand_t xhttp, tchar_t* addr)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	res_file_t so;
	net_addr_t na = { 0 };
	unsigned short port = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	switch (phttp->secu)
	{
#ifdef XDL_SUPPORT_CRYPT
	case _SECU_SSL:
		so = xssl_socket(phttp->inf.bio);
		break;
	case _SECU_SSH:
		so = xssh_socket(phttp->inf.bio);
		break;
#endif
	default:
		so = xtcp_socket(phttp->inf.bio);
		break;
	}

	if (so == INVALID_FILE)
		return 0;

	socket_addr(so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

unsigned short xhttp_peer_port(xhand_t xhttp, tchar_t* addr)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	res_file_t so;
	net_addr_t na = { 0 };
	unsigned short port = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	switch (phttp->secu)
	{
#ifdef XDL_SUPPORT_CRYPT
	case _SECU_SSL:
		so = xssl_socket(phttp->inf.bio);
		break;
	case _SECU_SSH:
		so = xssh_socket(phttp->inf.bio);
		break;
#endif
	default:
		so = xtcp_socket(phttp->inf.bio);
		break;
	}

	if (so == INVALID_FILE)
		return 0;

	socket_peer(so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

dword_t xhttp_format_request(xhand_t xhttp, byte_t* buf, dword_t max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return _xhttp_format_request(phttp, buf, max);
}

stream_t xhttp_get_send_stream(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return phttp->send_stream;
}

stream_t xhttp_get_recv_stream(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return phttp->recv_stream;
}

int	xhttp_get_url_method(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	len = xslen(phttp->method);
	len = (len < max) ? len : max;
	xsncpy(buf, phttp->method, len);
	return len;
}

int	xhttp_get_url_host(xhand_t xhttp,tchar_t* buf,int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	len = xslen(phttp->host);
	len = (len < max) ? len : max;
	xsncpy(buf, phttp->host, len);
	return len;
}

int	xhttp_get_url_port(xhand_t xhttp,tchar_t* buf,int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return stoxs(phttp->port, buf, max);
}

int	xhttp_get_url_object(xhand_t xhttp,tchar_t* buf,int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	len = xslen(phttp->object);
	len = (len < max) ? len : max;
	xsncpy(buf, phttp->object, len);
	return len;
}

dword_t xhttp_get_encoded_query(xhand_t xhttp, byte_t* buf, dword_t max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	max = (max < phttp->len_query) ? max : phttp->len_query;
	if (buf)
	{
		xmem_copy((void*)buf, (void*)phttp->query, max);
	}

	return max;
}

void xhttp_set_encoded_query(xhand_t xhttp, const byte_t* query, dword_t len)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (!query || !len)
	{
		xmem_free(phttp->query);
		phttp->query = NULL;
		phttp->len_query = 0;
		return;
	}

	phttp->len_query = len;
	phttp->query = (byte_t*)xmem_realloc(phttp->query, phttp->len_query + 1);
	xmem_copy((void*)phttp->query, (void*)query, phttp->len_query);
}

dword_t xhttp_url_encoding(const tchar_t* url, int len, byte_t* buf, dword_t max)
{
	dword_t total = 0;
	tchar_t *pre, *token = (tchar_t*)url;
	tchar_t *kstr, *vstr;
	int klen, vlen;

	byte_t* utf;
	dword_t ulen;

	byte_t* esc;
	dword_t elen;

	if (len < 0)
		len = xslen(url);

	while (token && len)
	{
		pre = token;
		token = parse_options_token(token, len, _T('='), _T('&'), &kstr, &klen, &vstr, &vlen);
		len -= (token - pre);

		//encoding the key
#ifdef _UNICODE
		ulen = ucs_to_utf8(kstr, klen, NULL, MAX_LONG);
		utf = (byte_t*)xmem_alloc(ulen);
		ucs_to_utf8(kstr, klen, utf, ulen);
#else
		ulen = mbs_to_utf8(kstr, klen, NULL, MAX_LONG);
		utf = (byte_t*)xmem_alloc(ulen);
		mbs_to_utf8(kstr, klen, utf, ulen);
#endif

		elen = url_byte_encode(utf, ulen, NULL, MAX_LONG);
		esc = (byte_t*)xmem_alloc(elen + 1);
		url_byte_encode(utf, ulen, esc, elen);

		xmem_free(utf);

		if (total + elen > max)
		{
			xmem_free(esc);
			return total;
		}
		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)esc, elen);
		}
		total += elen;

		xmem_free(esc);

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = ('=');
		}
		total++;

		//encoding the value
#ifdef _UNICODE
		ulen = ucs_to_utf8(vstr, vlen, NULL, MAX_LONG);
		utf = (byte_t*)xmem_alloc(ulen);
		ucs_to_utf8(vstr, vlen, utf, ulen);
#else
		ulen = mbs_to_utf8(vstr, vlen, NULL, MAX_LONG);
		utf = (byte_t*)xmem_alloc(ulen);
		mbs_to_utf8(vstr, vlen, utf, ulen);
#endif

		elen = url_byte_encode(utf, ulen, NULL, MAX_LONG);
		esc = (byte_t*)xmem_alloc(elen + 1);
		url_byte_encode(utf, ulen, esc, elen);

		xmem_free(utf);

		if (total + elen > max)
		{
			xmem_free(esc);
			return total;
		}
		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)esc, elen);
		}
		total += elen;

		xmem_free(esc);

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = ('&');
		}
		total++;
	}

	//remove last '&'
	if (total)
	{
		total--;

		if (buf)
		{
			buf[total] = ('\0');
		}
	}

	return total;
}

int xhttp_url_decoding(const byte_t* url, dword_t len, tchar_t* buf, int max)
{
	int total = 0;
	schar_t *pre, *token = (schar_t*)url;
	schar_t *kstr, *vstr;
	int klen, vlen;

	wchar_t* ucs;
	int ulen;

	byte_t* esc;
	dword_t elen;

	while (token && len)
	{
		pre = token;
		token = a_parse_options_token(token, len, ('='), ('&'), &kstr, &klen, &vstr, &vlen);
		len -= (token - pre);

		//decoding the key
		elen = url_byte_decode((byte_t*)kstr, klen, NULL, MAX_LONG);
		esc = (byte_t*)xmem_alloc(elen + 1);
		url_byte_decode(kstr, klen, esc, elen);

#ifdef _UNICODE
		ulen = utf8_to_ucs(esc, elen, NULL, MAX_LONG);
		ucs = xsalloc(ulen + 1);
		utf8_to_ucs(esc, elen, ucs, ulen);
#else
		ulen = mbs_to_ucs(esc, elen, NULL, MAX_LONG);
		ucs = xsalloc(ulen + 1);
		mbs_to_ucs(esc, elen, ucs, ulen);
#endif

		xmem_free(esc);

		if (total + ulen > max)
		{
			xmem_free(ucs);
			return total;
		}
		if (buf)
		{
			xsncpy(buf + total, ucs, ulen);
		}
		total += ulen;

		xmem_free(ucs);

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = ('=');
		}
		total++;

		//decoding the value
		elen = url_byte_decode((byte_t*)vstr, vlen, NULL, MAX_LONG);
		esc = (byte_t*)xmem_alloc(elen + 1);
		url_byte_decode(vstr, vlen, esc, elen);

#ifdef _UNICODE
		ulen = utf8_to_ucs(esc, elen, NULL, MAX_LONG);
		ucs = xsalloc(ulen + 1);
		utf8_to_ucs(esc, elen, ucs, ulen);
#else
		ulen = mbs_to_ucs(esc, elen, NULL, MAX_LONG);
		ucs = xsalloc(ulen + 1);
		mbs_to_ucs(esc, elen, ucs, ulen);
#endif

		xmem_free(esc);

		if (total + ulen > max)
		{
			xmem_free(ucs);
			return total;
		}
		if (buf)
		{
			xsncpy(buf + total, ucs, ulen);
		}
		total += ulen;

		xmem_free(ucs);

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = ('&');
		}
		total++;
	}

	//remove last '&'
	if (total)
	{
		total--;

		if (buf)
		{
			buf[total] = ('\0');
		}
	}

	return total;
}

void xhttp_set_url_query(xhand_t xhttp, const tchar_t* query, int len)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (len < 0) len = xslen(query);

	phttp->len_query = xhttp_url_encoding(query, len, NULL, MAX_LONG);
	phttp->query = (byte_t*)xmem_realloc(phttp->query, phttp->len_query + 1);
	xhttp_url_encoding(query, len, phttp->query, phttp->len_query);
}

int xhttp_get_url_query(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return xhttp_url_decoding(phttp->query, phttp->len_query, buf, max);
}

void xhttp_set_url_query_entity(xhand_t xhttp, const tchar_t* key, int klen, const tchar_t* val, int vlen)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	byte_t *utf_key, *utf_val;
	dword_t ulen_key, ulen_val;

	byte_t *esc_key, *esc_val;
	dword_t elen_key, elen_val;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_CLI);

	if (klen < 0) klen = xslen(key);

	if (!klen)
		return;

	if (vlen < 0) vlen = xslen(val);

	//encoding the key
#ifdef _UNICODE
	ulen_key = ucs_to_utf8(key, klen, NULL, MAX_LONG);
	utf_key = (byte_t*)xmem_alloc(ulen_key);
	ucs_to_utf8(key, klen, utf_key, ulen_key);
#else
	ulen_key = mbs_to_utf8(key, klen, NULL, MAX_LONG);
	utf_key = (byte_t*)xmem_alloc(ulen_key);
	mbs_to_utf8(key, klen, utf_key, ulen_key);
#endif

	elen_key = url_byte_encode(utf_key, ulen_key, NULL, MAX_LONG);
	esc_key = (byte_t*)xmem_alloc(elen_key + 1);
	url_byte_encode(utf_key, ulen_key, esc_key, elen_key);

	xmem_free(utf_key);

	//encoding the value
#ifdef _UNICODE
	ulen_val = ucs_to_utf8(val, vlen, NULL, MAX_LONG);
	utf_val = (byte_t*)xmem_alloc(ulen_val);
	ucs_to_utf8(val, vlen, utf_val, ulen_val);
#else
	ulen_val = mbs_to_utf8(val, vlen, NULL, MAX_LONG);
	utf_val = (byte_t*)xmem_alloc(ulen_val);
	mbs_to_utf8(val, vlen, utf_val, ulen_val);
#endif

	elen_val = url_byte_encode(utf_val, ulen_val, NULL, MAX_LONG);
	esc_val = (byte_t*)xmem_alloc(elen_val + 1);
	url_byte_encode(utf_val, ulen_val, esc_val, elen_val);

	xmem_free(utf_val);

	//append '&' key '=' val
	phttp->query = (byte_t*)xmem_realloc(phttp->query, phttp->len_query + 1 + elen_key + 1 + elen_val + 1);

	//if not the first entity
	if (phttp->len_query)
	{
		phttp->query[phttp->len_query] = ('&');
		phttp->len_query++;
	}

	xmem_copy((void*)(phttp->query + phttp->len_query), (void*)esc_key, elen_key);
	phttp->len_query += elen_key;

	phttp->query[phttp->len_query] = ('=');
	phttp->len_query++;

	xmem_copy((void*)(phttp->query + phttp->len_query), (void*)esc_val, elen_val);
	phttp->len_query += elen_val;

	xmem_free(esc_key);
	xmem_free(esc_val);
}

int xhttp_get_url_query_entity(xhand_t xhttp, const tchar_t* key, int len, tchar_t* buf, int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	tchar_t* url;
	int url_len;

	tchar_t* token;
	tchar_t *kstr, *vstr;
	int klen, vlen;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (!phttp->len_query)
	{
		if (buf) buf[0] = _T('\0');
		return 0;
	}

	url_len = xhttp_url_decoding(phttp->query, phttp->len_query, NULL, MAX_LONG);
	url = xsalloc(url_len + 1);
	xhttp_url_decoding(phttp->query, phttp->len_query, url, url_len);

	kstr = vstr = NULL;
	klen = vlen = 0;

	token = url;
	while (token)
	{
		kstr = vstr = NULL;
		klen = vlen = 0;
		token = parse_options_token(token, -1, _T('='), _T('&'), &kstr, &klen, &vstr, &vlen);

		if (compare_text(key, len, kstr, klen, 1) == 0)
		{
			break;
		}
	}

	if (buf)
	{
		vlen = (vlen < max) ? vlen : max;
		xsncpy(buf, vstr, vlen);
	}

	xsfree(url);

	return vlen;
}

void xhttp_set_request_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	write_string_entity(phttp->st_request,hname,nlen,hvalue,vlen);
}

int xhttp_get_request_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* hvalue, int hmax)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	ent = find_string_entity(phttp->st_request, hname, nlen);
	if (ent)
	{
		return get_string_entity_val(ent, hvalue, hmax);
	}
	else
	{
		if (hvalue) hvalue[0] = _T('\0');
		return 0;
	}
}

void xhttp_set_request_default_header(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t szHost[PATH_LEN + 1] = { 0 };
	tchar_t szPort[NUM_LEN + 1] = { 0 };
	tchar_t szDate[NUM_LEN + 1] = { 0 };
	xdate_t dt = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	get_utc_date(&dt);
	format_gmttime(&dt, szDate);

	xhttp_get_url_host(xhttp, szHost, PATH_LEN);
	xhttp_get_url_port(xhttp, szPort, NUM_LEN);
	
	if (!is_null(szPort) && xstos(szPort) != 80)
	{
		xscat(szHost, _T(":"));
		xscat(szHost, szPort);
	}

	clear_string_table(phttp->st_request);

	xhttp_set_request_header(xhttp, HTTP_HEADER_HOST, -1, szHost, -1);
	xhttp_set_request_header(xhttp, HTTP_HEADER_DATE, -1, szDate, -1);
	xhttp_set_request_header(xhttp, HTTP_HEADER_CONNECTION, -1, HTTP_HEADER_CONNECTION_CLOSE, -1);
}

dword_t xhttp_format_response(xhand_t xhttp, byte_t* buf, dword_t max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return _xhttp_format_response(phttp, buf, max);
}

void xhttp_get_response_code(xhand_t xhttp, tchar_t* buf)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	
	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	xsncpy(buf,phttp->code,NUM_LEN);
}

void xhttp_set_response_code(xhand_t xhttp, const tchar_t* code)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	xsncpy(phttp->code, code, NUM_LEN);
}

int	xhttp_get_response_message(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	len = xslen(phttp->text);
	len = (len < max) ? len : max;
	if (buf)
		xsncpy(buf, phttp->text, len);

	return len;
}

void xhttp_set_response_message(xhand_t xhttp, const tchar_t* msg, int len)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (len < 0)
		len = xslen(msg);
	len = (len < ERR_LEN) ? len : ERR_LEN;
	xsncpy(phttp->text, msg, len);
}

int xhttp_get_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* hvalue, int vmax)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	ent = find_string_entity(phttp->st_response, hname, nlen);
	if (ent)
	{
		return get_string_entity_val(ent, hvalue, vmax);
	}
	else
	{
		if (hvalue) hvalue[0] = _T('\0');
		return 0;
	}
}

void xhttp_set_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	write_string_entity(phttp->st_response, hname, nlen, hvalue, vlen);
}

void xhttp_set_response_default_header(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t szDate[DATE_LEN + 1] = { 0 };
	xdate_t dt = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	get_utc_date(&dt);
	format_gmttime(&dt, szDate);

	clear_string_table(phttp->st_response);

	xhttp_set_response_header(xhttp, HTTP_HEADER_CONNECTION, -1, HTTP_HEADER_CONNECTION_CLOSE, -1);
	xhttp_set_response_header(xhttp, HTTP_HEADER_DATE, -1, szDate, -1);
	xhttp_set_response_header(xhttp, HTTP_HEADER_SERVER, -1, HTTP_HEADER_SERVER_XSERVICE, -1);
}

bool_t xhttp_get_response_state(xhand_t xhttp)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };

	xhttp_get_response_code(xhttp, sz_code);

	return IS_XHTTP_SUCCEED(sz_code);
}

void xhttp_set_response_content_length(xhand_t xhttp, dword_t len)
{
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	xsprintf(sz_size, _T("%d"), len);

	xhttp_set_response_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, sz_size, -1);
}

dword_t xhttp_get_response_content_length(xhand_t xhttp)
{
	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	long n_size;

	xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, sz_size, NUM_LEN);

	n_size = xstol(sz_size);
	return (n_size < 0) ? 0 : n_size;
}

void xhttp_set_request_content_length(xhand_t xhttp, dword_t len)
{
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	xsprintf(sz_size, _T("%d"), len);

	xhttp_set_request_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, sz_size, -1);
}

dword_t xhttp_get_request_content_length(xhand_t xhttp)
{
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	xhttp_get_request_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, sz_size, NUM_LEN);

	return xstol(sz_size);
}

int xhttp_get_response_content_type_charset(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	const tchar_t* str;
	tchar_t *key, *val;
	int klen, vlen;
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (buf)
	{
		xszero(buf, max);
	}

	ent = find_string_entity(phttp->st_response, HTTP_HEADER_CONTENTTYPE, -1);
	if (!ent)
		return 0;

	str = get_string_entity_val_ptr(ent);
	if (is_null(str))
		return 0;

	str = xsistr(str, HTTP_HEADER_CONTENTTYPE_ENTITY_CHARSET);
	if (is_null(str))
		return 0;

	parse_options_token(str, -1, _T('='), _T(';'), &key, &klen, &val, &vlen);

	if (!vlen)
		return 0;

	vlen = (vlen < max) ? vlen : max;

	if (buf)
	{
		xsncpy(buf, val, vlen);
	}

	return vlen;
}

void xhttp_set_response_content_type_charset(xhand_t xhttp, const tchar_t* token, int len)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	tchar_t* str;
	tchar_t ctype[META_LEN + 1] = { 0 };
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	ent = find_string_entity(phttp->st_response, HTTP_HEADER_CONTENTTYPE, -1);

	if (ent)
	{
		get_string_entity_val(ent, ctype, META_LEN);
	}
	else
	{
		xscpy(ctype, HTTP_HEADER_CONTENTTYPE_APPXML);
	}

	str = (tchar_t*)xsistr(ctype, HTTP_HEADER_CONTENTTYPE_ENTITY_CHARSET);
	if (str)
	{
		*str = _T('\0');
	}

	str = (tchar_t*)xsistr(ctype, _T(";"));
	if (str)
	{
		*str = _T('\0');
	}

	if (len < 0)
		len = xslen(token);

	if (len)
	{
		xscat(ctype, _T("; "));
		xscat(ctype, HTTP_HEADER_CONTENTTYPE_ENTITY_CHARSET);
		xscat(ctype, _T("="));
		xsncat(ctype, token, len);
	}

	set_string_entity_val(ent, ctype, -1);
}

int xhttp_get_request_content_type_charset(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	const tchar_t* str;
	tchar_t *key, *val;
	int klen, vlen;
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (buf)
	{
		xszero(buf, max);
	}

	ent = find_string_entity(phttp->st_request, HTTP_HEADER_CONTENTTYPE, -1);
	if (!ent)
		return 0;

	str = get_string_entity_val_ptr(ent);
	if (is_null(str))
		return 0;

	str = xsistr(str, HTTP_HEADER_CONTENTTYPE_ENTITY_CHARSET);
	if (is_null(str))
		return 0;

	parse_options_token(str, -1, _T('='), _T(';'), &key, &klen, &val, &vlen);

	if (!vlen)
		return 0;

	vlen = (vlen < max) ? vlen : max;

	if (buf)
	{
		xsncpy(buf, val, vlen);
	}

	return vlen;
}

void xhttp_set_request_content_type_charset(xhand_t xhttp, const tchar_t* token, int len)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	tchar_t* str;
	tchar_t ctype[META_LEN + 1] = { 0 };
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	ent = find_string_entity(phttp->st_request, HTTP_HEADER_CONTENTTYPE, -1);

	if (ent)
	{
		get_string_entity_val(ent, ctype, META_LEN);
	}
	else
	{
		xscpy(ctype, HTTP_HEADER_CONTENTTYPE_APPXML);
	}

	str = (tchar_t*)xsistr(ctype, HTTP_HEADER_CONTENTTYPE_ENTITY_CHARSET);
	if (str)
	{
		*str = _T('\0');
	}

	str = (tchar_t*)xsistr(ctype, _T(";"));
	if (str)
	{
		*str = _T('\0');
	}

	if (len < 0)
		len = xslen(token);

	if (len)
	{
		xscat(ctype, _T("; "));
		xscat(ctype, HTTP_HEADER_CONTENTTYPE_ENTITY_CHARSET);
		xscat(ctype, _T("="));
		xsncat(ctype, token, len);
	}

	set_string_entity_val(ent, ctype, -1);
}

void xhttp_set_request_cookie(xhand_t xhttp, const tchar_t* key, const tchar_t* val, int len)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t cookies[PATH_LEN + 1] = { 0 };
	link_t_ptr ent;
	link_t_ptr st, ilk;
	int total = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	ent = find_string_entity(phttp->st_request, HTTP_HEADER_COOKIE, -1);

	if (ent)
	{
		get_string_entity_val(ent, cookies, PATH_LEN);
	}

	if (len < 0)
		len = xslen(val);

	if (xslen(cookies) + xslen(key) + len + 3 >= PATH_LEN)
	{
		return;
	}

	st = create_string_table(0);

	string_table_format_options(st, cookies, -1, _T('='), _T(';'));

	ilk = get_string_next_entity(st, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(get_string_entity_key_ptr(ilk), -1, key, -1, 1) == 0)
		{
			set_string_entity_val(ilk, val, len);
			break;
		}

		ilk = get_string_next_entity(st, ilk);
	}

	if (!ilk)
	{
		write_string_entity(st, key, -1, val, len);
	}

	xszero(cookies, PATH_LEN);

	total = 0;
	ilk = get_string_next_entity(st, LINK_FIRST);
	while (ilk)
	{
		total += xsprintf(cookies + total, _T("%s=%s; "), get_string_entity_key_ptr(ilk), get_string_entity_val_ptr(ilk));

		ilk = get_string_next_entity(st, ilk);
	}

	destroy_string_table(st);

	set_string_entity_val(ent, cookies, total);
}

int xhttp_get_request_cookie(xhand_t xhttp, const tchar_t* key, tchar_t* val, int max)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t cookies[PATH_LEN + 1] = { 0 };
	link_t_ptr ent;
	link_t_ptr st, ilk;
	int n;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	ent = find_string_entity(phttp->st_request, HTTP_HEADER_COOKIE, -1);

	if (ent)
	{
		get_string_entity_val(ent, cookies, PATH_LEN);
	}

	st = create_string_table(0);

	string_table_format_options(st, cookies, -1, _T('='), _T(';'));

	n = 0;
	ilk = get_string_next_entity(st, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(get_string_entity_key_ptr(ilk), -1, key, -1, 1) == 0)
		{
			n = get_string_entity_val(ilk, val, max);
			break;
		}

		ilk = get_string_next_entity(st, ilk);
	}

	destroy_string_table(st);

	return n;
}

void xhttp_get_authorization(xhand_t xhttp, tchar_t* sz_mode, tchar_t* sz_sid, int slen, tchar_t* sz_sign, int max)
{
	tchar_t sz_auth[META_LEN + 1] = { 0 };
	int len;
	tchar_t *token;

	xhttp_get_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, META_LEN);

	if (compare_text(sz_auth, 3, _T("XDS"), 3, 1) != 0)
		return;

	token = sz_auth + 3;
	while (*token == _T(' ') && *token != _T('\0'))
		token++;

	len = 0;
	while (*token != _T(':') && *token != _T('\0'))
	{
		token++;
		len++;
	}

	slen = (slen < len) ? slen : len;
	if (sz_sid)
	{
		xsncpy(sz_sid, token - len, slen);
	}

	if (*token == _T(':'))
		token++;

	len = 0;
	while (*token != _T('\0'))
	{
		token++;
		len++;
	}

	max = (max < len) ? max : len;
	if (sz_sign)
	{
		xsncpy(sz_sign, token - len, max);
	}
}

void xhttp_set_authorization(xhand_t xhttp, const tchar_t* sz_mode, const tchar_t* sz_sid, int slen, const tchar_t* sz_sign, int max)
{
	tchar_t sz_auth[META_LEN + 1] = { 0 };

	xscat(sz_auth, _T("XDS"));

	if (slen < 0)
		slen = xslen(sz_sid);

	if (!slen)
		return;

	xscat(sz_auth, _T(" "));
	xsncat(sz_auth, sz_sid, slen);

	if (max < 0)
		max = xslen(sz_sign);

	if (!max)
		return;

	xscat(sz_auth, _T(":"));
	xsncat(sz_auth, sz_sign, max);

	xhttp_set_request_header(xhttp, HTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
}

#ifdef XDL_SUPPORT_CRYPT
int xhttp_request_signature(xhand_t xhttp, const tchar_t* auth, const tchar_t* skey, tchar_t* buf, int max)
{
	tchar_t sz_verb[RES_LEN + 1] = { 0 };
	tchar_t sz_type[RES_LEN + 1] = { 0 };
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	tchar_t sz_object[PATH_LEN] = { 0 };
	tchar_t sz_signature[HMAC_LEN + 1] = { 0 };

	int acc_len, utf8_len, bas_len;
	int pos;

	byte_t* acc_buf = NULL;
	byte_t* utf8_buf = NULL;

	unsigned char dig[20] = { 0 };

	xhttp_get_url_method(xhttp, sz_verb, RES_LEN);
	xhttp_get_url_object(xhttp, sz_object, PATH_LEN);
	xhttp_get_request_header(xhttp, HTTP_HEADER_CONTENTTYPE, -1, sz_type, RES_LEN);
	xhttp_get_request_header(xhttp, HTTP_HEADER_DATE, -1, sz_date, DATE_LEN);

#ifdef _UNICODE
	acc_len = ucs_to_utf8(skey, -1, NULL, MAX_LONG);
#else
	acc_len = mbs_to_utf8(skey, -1, NULL, MAX_LONG);
#endif

	acc_buf = (byte_t*)xmem_alloc(acc_len + 1);

#ifdef _UNICODE
	ucs_to_utf8(skey, -1, acc_buf, acc_len);
#else
	mbs_to_utf8(skey, -1, acc_buf, acc_len);
#endif

#ifdef _UNICODE
	utf8_len = ucs_to_utf8(sz_verb, -1, NULL, MAX_LONG) + 1 \
		+ ucs_to_utf8(sz_type, -1, NULL, MAX_LONG) + 1 \
		+ ucs_to_utf8(sz_date, -1, NULL, MAX_LONG) + 1 \
		+ ucs_to_utf8(sz_object, -1, NULL, MAX_LONG);
#else
	utf8_len = mbs_to_utf8(sz_verb, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(sz_type, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(sz_date, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(sz_object, -1, NULL, MAX_LONG);
#endif

	utf8_buf = (byte_t*)xmem_alloc(utf8_len + 1);

	pos = 0;
#ifdef _UNICODE
	pos += ucs_to_utf8(sz_verb, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(sz_verb, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(sz_type, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(sz_type, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(sz_date, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(sz_date, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(sz_object, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(sz_object, -1, utf8_buf + pos, utf8_len - pos);
#endif

	sha1_hmac((unsigned char*)acc_buf, acc_len, (unsigned char*)utf8_buf, pos, dig);

	bas_len = xbas_encode(dig, 20, sz_signature, RES_LEN);

	xmem_free(acc_buf);
	xmem_free(utf8_buf);

	bas_len = (bas_len < max) ? bas_len : max;
	if (buf)
	{
		xsncpy(buf, sz_signature, bas_len);
	}

	return bas_len;
}
#endif

dword_t xhttp_format_error(bool_t b_json, const tchar_t* encoding, const tchar_t* errcode, const tchar_t* errtext, int slen, byte_t* buf, dword_t max)
{
	link_t_ptr ptr_xml, ptr_dom;
	link_t_ptr nlk;

	dword_t nlen = 0;
	byte_t* sz_buf = NULL;

	if (b_json)
	{
		ptr_dom = create_json_doc();
	}
	else
	{
		ptr_xml = create_xml_doc();

		if (!is_null(encoding))
			set_xml_encoding(ptr_xml, encoding, -1);

		ptr_dom = get_xml_dom_node(ptr_xml);
		set_dom_node_name(ptr_dom, HTTP_FAULT, -1);
	}

	nlk = insert_dom_node(ptr_dom, LINK_LAST);
	set_dom_node_name(nlk, HTTP_FAULT_CODE, -1);
	set_dom_node_text(nlk, errcode, -1);

	nlk = insert_dom_node(ptr_dom, LINK_LAST);
	set_dom_node_name(nlk, HTTP_FAULT_STRING, -1);
	set_dom_node_text(nlk, errtext, slen);

	if (b_json)
	{
		nlen = format_json_doc_to_bytes(ptr_dom, buf, max, parse_charset(encoding));

		destroy_json_doc(ptr_dom);
	}
	else
	{
		nlen = format_xml_doc_to_bytes(ptr_xml, buf, max);

		destroy_xml_doc(ptr_xml);
	}

	return nlen;
}

bool_t xhttp_parse_error(bool_t b_json, const tchar_t* encoding, const byte_t* buf, dword_t len, tchar_t* errcode, tchar_t* errtext, int max)
{
	link_t_ptr nlk,ptr_dom,ptr_xml;
	bool_t b_rt;

	if (b_json)
	{
		ptr_dom = create_json_doc();
		b_rt = parse_json_doc_from_bytes(ptr_dom, buf, len, parse_charset(encoding));
		if (!b_rt)
		{
			destroy_json_doc(ptr_dom);
			return 0;
		}
	}
	else
	{
		ptr_xml = create_xml_doc();
		b_rt = parse_xml_doc_from_bytes(ptr_xml, buf, len);
		if (!b_rt)
		{
			destroy_xml_doc(ptr_xml);
			return 0;
		}

		ptr_dom = get_xml_dom_node(ptr_xml);
		if (compare_text(get_dom_node_name_ptr(ptr_dom), -1, HTTP_FAULT, -1, 1) != 0)
		{
			destroy_xml_doc(ptr_xml);
			return 0;
		}
	}

	nlk = get_dom_first_child_node(ptr_dom);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, HTTP_FAULT_CODE, -1, 1) == 0)
		{
			if (errcode)
			{
				get_dom_node_text(nlk, errcode, NUM_LEN);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, HTTP_FAULT_STRING, -1, 1) == 0)
		{
			if (errtext)
			{
				get_dom_node_text(nlk, errtext, max);
			}
		}
		nlk = get_dom_next_sibling_node(nlk);
	}

	if (b_json)
		destroy_json_doc(ptr_dom);
	else
		destroy_xml_doc(ptr_xml);

	return 1;
}

bool_t	xhttp_is_requested(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return  phttp->b_request;
}

bool_t	xhttp_is_responsed(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	return phttp->b_response;
}

void xhttp_reset_request(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	clear_string_table(phttp->st_request);

	if (phttp->type == _XHTTP_TYPE_CLI)
	{
		if (phttp->send_stream)
		{
			stream_free(phttp->send_stream);
			phttp->send_stream = NULL;
		}
	}

	phttp->b_request = 0;
}

void xhttp_reset_response(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	clear_string_table(phttp->st_response);

	if (phttp->type == _XHTTP_TYPE_SRV)
	{
		if (phttp->send_stream)
		{
			stream_free(phttp->send_stream);
			phttp->send_stream = NULL;
		}
	}

	phttp->b_response = 0;

	xszero(phttp->code, NUM_LEN);
	xszero(phttp->text, ERR_LEN);
}

void xhttp_send_continue(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	byte_t *buf_response = NULL;
	dword_t len_response = 0;
	int n;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_SRV);
	
	TRY_CATCH;

	len_response = _xhttp_format_continue(phttp, NULL, MAX_LONG);
	buf_response = (byte_t*)xmem_alloc(len_response + 1);

	len_response = _xhttp_format_continue(phttp, buf_response, len_response);

	if (phttp->inf.pf_setopt)
	{
		n = 0;
		(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_SNDBUF, &n, sizeof(n));
	}

	if (phttp->inf.pf_write)
	{
		if (!(*phttp->inf.pf_write)(phttp->inf.bio, buf_response, &len_response))
		{
			raise_user_error(NULL, NULL);
		}
	}

	if (phttp->inf.pf_flush)
	{
		if (!(*phttp->inf.pf_flush)(phttp->inf.bio))
		{
			raise_user_error(NULL, NULL);
		}
	}

	xmem_free(buf_response);
	buf_response = NULL;

	END_CATCH;

	return;
ONERROR:

	if (buf_response)
		xmem_free(buf_response);

	XDL_TRACE_LAST;

	return;
}

bool_t xhttp_send_response(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t charset[INT_LEN + 1] = { 0 };
	byte_t *buf_response = NULL;
	dword_t len_response = 0;
	int opt = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_SRV);
	
	TRY_CATCH;

	if (!phttp->b_response)
	{
		len_response = _xhttp_format_response(phttp, NULL, MAX_LONG);
		buf_response = (byte_t*)xmem_alloc(len_response + 1);

		len_response = _xhttp_format_response(phttp, buf_response, len_response);

		if (phttp->inf.pf_setopt)
		{
			opt = 0;
			(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_SNDBUF, &opt, sizeof(opt));
		}

		if (!(*phttp->inf.pf_write)(phttp->inf.bio, buf_response, &len_response))
		{
			raise_user_error(NULL, NULL);
		}

		if (phttp->inf.pf_flush)
		{
			if (!(*phttp->inf.pf_flush)(phttp->inf.bio))
			{
				raise_user_error(NULL, NULL);
			}
		}

		phttp->b_response = 1;

		if (phttp->b_response)
		{
			phttp->send_stream = stream_alloc(phttp->inf.bio);

			xhttp_get_response_content_type_charset(xhttp, charset, INT_LEN);
			if (!is_null(charset))
			{
				stream_set_encode(phttp->send_stream, parse_charset(charset));
			}

			if (xhttp_is_chunked_send(xhttp))
			{
				stream_set_mode(phttp->send_stream, CHUNK_OPERA);
				stream_opera_reset(phttp->send_stream);
				if (phttp->inf.pf_setopt)
				{
					opt = TCP_MAX_BUFF;
					(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_SNDBUF, &opt, sizeof(opt));
				}
			}
			else
			{
				len_response = xhttp_get_response_content_length(xhttp);
				stream_set_size(phttp->send_stream, len_response);
				if (phttp->inf.pf_setopt)
				{
					opt = ((len_response) ? len_response : TCP_MAX_BUFF);
					(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_SNDBUF, &opt, sizeof(opt));
				}
			}
		}

		xmem_free(buf_response);
		buf_response = NULL;
	}

	END_CATCH;

	return phttp->b_response;
ONERROR:

	if (buf_response)
		xmem_free(buf_response);

	XDL_TRACE_LAST;

	return phttp->b_response;
}

bool_t xhttp_recv_response(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	dword_t len_header, len_response,len_one;
	tchar_t charset[INT_LEN + 1] = { 0 };
	byte_t* buf_response = NULL;
	int opt;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_CLI);
	
	TRY_CATCH;

	if (!phttp->b_response)
	{
		if (phttp->inf.pf_setopt)
		{
			opt = 0;
			(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_RCVBUF, &opt, sizeof(opt));
		}

		len_header = XHTTP_HEADER_SIZE;
		buf_response = (byte_t*)xmem_alloc(len_header + 1);
		len_response = 0;

		while (1)
		{
			len_one = 1;
			if (!(*phttp->inf.pf_read)(phttp->inf.bio, buf_response + len_response, &len_one))
			{
				raise_user_error(NULL, NULL);
			}

			if (!len_one)
				break;

			len_response++;

			if (len_response > len_header)
			{
				len_header += XHTTP_HEADER_SIZE;
				buf_response = (byte_t*)xmem_realloc(buf_response, len_header + 1);
			}

			if (len_response >= 4)
			{
				if (buf_response[len_response - 1] == '\n' && buf_response[len_response - 2] == '\r' && buf_response[len_response - 3] == '\n' && buf_response[len_response - 4] == '\r')
				{
					phttp->b_response = 1;
					break;
				}
			}
		}

		if (phttp->b_response)
		{
			phttp->recv_stream = stream_alloc(phttp->inf.bio);

			_xhttp_parse_response(phttp, buf_response, len_response);

			xhttp_get_response_content_type_charset(xhttp, charset, INT_LEN);
			if (!is_null(charset))
			{
				stream_set_encode(phttp->recv_stream, parse_charset(charset));
			}

			if (xhttp_is_chunked_recv(xhttp))
			{
				stream_set_mode(phttp->recv_stream, CHUNK_OPERA);
				stream_opera_reset(phttp->recv_stream);
				if (phttp->inf.pf_setopt)
				{
					opt = TCP_MAX_BUFF;
					(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_RCVBUF, &opt, sizeof(opt));
				}
			}
			else
			{
				len_one = xhttp_get_response_content_length(xhttp);
				stream_set_size(phttp->recv_stream, len_one);
				if (phttp->inf.pf_setopt)
				{
					opt = ((len_one) ? len_one : TCP_MAX_BUFF);
					(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_RCVBUF, &opt, sizeof(opt));
				}
			}
		}

		xmem_free(buf_response);
		buf_response = NULL;
	}

	END_CATCH;

	return phttp->b_response;
ONERROR:

	if (buf_response)
		xmem_free(buf_response);

	XDL_TRACE_LAST;

	return phttp->b_response;
}

bool_t xhttp_send_request(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t charset[INT_LEN + 1] = { 0 };
	byte_t *buf_request = NULL;
	dword_t len_request = 0;
	int opt;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_CLI);

	TRY_CATCH;

	if (!phttp->b_request)
	{
		len_request = _xhttp_format_request(phttp, NULL, MAX_LONG);
		buf_request = (byte_t*)xmem_alloc(len_request + 1);

		len_request = _xhttp_format_request(phttp, buf_request, len_request);

		if (phttp->inf.pf_setopt)
		{
			opt = 0;
			(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_SNDBUF, &opt, sizeof(opt));
		}

		if (!(*phttp->inf.pf_write)(phttp->inf.bio, buf_request, &len_request))
		{
			raise_user_error(NULL, NULL);
		}

		if (phttp->inf.pf_flush)
		{
			if (!(*phttp->inf.pf_flush)(phttp->inf.bio))
			{
				raise_user_error(NULL, NULL);
			}
		}

		phttp->b_request = 1;

		if (phttp->b_request)
		{
			phttp->send_stream = stream_alloc(phttp->inf.bio);

			xhttp_get_request_content_type_charset(xhttp, charset, INT_LEN);
			if (!is_null(charset))
			{
				stream_set_encode(phttp->send_stream, parse_charset(charset));
			}

			if (xhttp_is_chunked_send(xhttp))
			{
				stream_set_mode(phttp->send_stream, CHUNK_OPERA);
				stream_opera_reset(phttp->send_stream);
				if (phttp->inf.pf_setopt)
				{
					opt = TCP_MAX_BUFF;
					(*phttp->inf.pf_setopt)(phttp->inf.bio,SOCKET_OPTION_SNDBUF, &opt, sizeof(opt));
				}
			}
			else
			{
				len_request = xhttp_get_request_content_length(xhttp);
				stream_set_size(phttp->send_stream, len_request);
				if (phttp->inf.pf_setopt)
				{
					opt = ((len_request) ? len_request : TCP_MAX_BUFF);
					(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_SNDBUF, &opt, sizeof(opt));
				}
			}
		}

		xmem_free(buf_request);
		buf_request = NULL;
	}

	END_CATCH;

	return phttp->b_request;
ONERROR:

	if (buf_request)
		xmem_free(buf_request);

	XDL_TRACE_LAST;

	return phttp->b_request;
}

bool_t xhttp_recv_request(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	dword_t len_header, len_request,len_one;
	tchar_t charset[INT_LEN + 1] = { 0 };
	byte_t* buf_request = NULL;
	int opt;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_SRV);

	TRY_CATCH;

	if (!phttp->b_request)
	{
		if (phttp->inf.pf_setopt)
		{
			opt = 0;
			(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_RCVBUF, &opt, sizeof(opt));
		}

		len_header = XHTTP_HEADER_SIZE;
		buf_request = (byte_t*)xmem_alloc(len_header + 1);
		len_request = 0;

		while (1)
		{
			len_one = 1;
			if (!(*phttp->inf.pf_read)(phttp->inf.bio, buf_request + len_request, &len_one))
			{
				raise_user_error(NULL, NULL);
			}

			if (!len_one)
				break;

			len_request++;

			if (len_request > XHTTP_HEADER_MAX)
			{
				raise_user_error(_T("0"), _T("invalid http header size"));
			}

			if (len_request > len_header)
			{
				len_header += XHTTP_HEADER_SIZE;
				buf_request = (byte_t*)xmem_realloc(buf_request, len_header + 1);
			}

			if (len_request == 6)
			{
				if (!IS_XHTTP_METHOD((schar_t*)buf_request))
				{
					raise_user_error(_T("0"), _T("invalid http method"));
				}
			}

			if (len_request > 6)
			{
				if (buf_request[len_request - 1] == '\n' && buf_request[len_request - 2] == '\r' && buf_request[len_request - 3] == '\n' && buf_request[len_request - 4] == '\r')
				{
					phttp->b_request = 1;
					break;
				}
			}
		}

		if (phttp->b_request)
		{
			phttp->recv_stream = stream_alloc(phttp->inf.bio);

			_xhttp_parse_request(phttp, buf_request, len_request);

			xhttp_get_request_content_type_charset(xhttp, charset, INT_LEN);
			if (!is_null(charset))
			{
				stream_set_encode(phttp->recv_stream, parse_charset(charset));
			}

			if (xhttp_is_chunked_recv(xhttp))
			{
				stream_set_mode(phttp->recv_stream, CHUNK_OPERA);
				stream_opera_reset(phttp->recv_stream);
				if (phttp->inf.pf_setopt)
				{
					opt = TCP_MAX_BUFF;
					(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_RCVBUF, &opt, sizeof(opt));
				}
			}
			else
			{
				len_one = xhttp_get_request_content_length(xhttp);
				stream_set_size(phttp->recv_stream, len_one);
				if (phttp->inf.pf_setopt)
				{
					opt = ((len_one) ? len_one : TCP_MAX_BUFF);
					(*phttp->inf.pf_setopt)(phttp->inf.bio, SOCKET_OPTION_RCVBUF, &opt, sizeof(opt));
				}
			}
		}

		xmem_free(buf_request);
		buf_request = NULL;
	}

	END_CATCH;

	return phttp->b_request;
ONERROR:

	if (buf_request)
		xmem_free(buf_request);

	XDL_TRACE_LAST;

	return phttp->b_request;
}

bool_t xhttp_is_chunked_recv(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_response)
		return 0;

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_request)
		return 0;

	if (phttp->type == _XHTTP_TYPE_CLI)
		xhttp_get_response_header(xhttp, HTTP_HEADER_TRANSFERENCODING, -1, token, RES_LEN);
	else
		xhttp_get_request_header(xhttp, HTTP_HEADER_TRANSFERENCODING, -1, token, RES_LEN);

	return (compare_text(token, -1, HTTP_HEADER_TRANSFERENCODING_CHUNKED, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_is_chunked_send(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_request)
		return 0;

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_response)
		return 0;

	if (phttp->type == _XHTTP_TYPE_CLI)
		xhttp_get_request_header(xhttp, HTTP_HEADER_TRANSFERENCODING, -1, token, RES_LEN);
	else
		xhttp_get_response_header(xhttp, HTTP_HEADER_TRANSFERENCODING, -1, token, RES_LEN);

	return (compare_text(token, -1, HTTP_HEADER_TRANSFERENCODING_CHUNKED, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_is_zipped_recv(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_response)
		return 0;

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_request)
		return 0;

	if (phttp->type == _XHTTP_TYPE_CLI)
		xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTENCODING, -1, token, RES_LEN);
	else
		xhttp_get_request_header(xhttp, HTTP_HEADER_CONTENTENCODING, -1, token, RES_LEN);

	return (compare_text(token, -1, HTTP_HEADER_CONTENTENCODING_DEFLATE, -1, 1) == 0 || compare_text(token, -1, HTTP_HEADER_CONTENTENCODING_GZIP, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_is_zipped_send(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_request)
		return 0;

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_response)
		return 0;

	if (phttp->type == _XHTTP_TYPE_CLI)
		xhttp_get_request_header(xhttp, HTTP_HEADER_CONTENTENCODING, -1, token, RES_LEN);
	else
		xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTENCODING, -1, token, RES_LEN);

	return (compare_text(token, -1, HTTP_HEADER_CONTENTENCODING_DEFLATE, -1, 1) == 0 || compare_text(token, -1, HTTP_HEADER_CONTENTENCODING_GZIP, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_need_expect(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	xhttp_get_request_header(xhttp, HTTP_HEADER_EXPECT, -1, token, RES_LEN);
	
	return (compare_text(phttp->method, -1, HTTP_METHOD_POST,-1,1) == 0 && compare_text(token, -1, HTTP_HEADER_EXPECT_CONTINUE, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_send(xhand_t xhttp, const byte_t* data, dword_t len)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->send_stream != NULL);

	TRY_CATCH;

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_request)
	{
		raise_user_error(_T("xhttp_send"), _T("xhttp not send request"));
	}

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_response)
	{
		raise_user_error(_T("xhttp_send"), _T("xhttp not recv response"));
	}

	if (!stream_write_bytes(phttp->send_stream, data, len))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t xhttp_fush(xhand_t xhttp)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->send_stream != NULL);

	return stream_flush(phttp->send_stream);
}

bool_t xhttp_recv(xhand_t xhttp,byte_t* buf,dword_t *pb)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);
	XDL_ASSERT(phttp->recv_stream != NULL);

	TRY_CATCH;

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_response)
	{
		raise_user_error(_T("xhttp_recv"), _T("http not recv response"));
	}

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_request)
	{
		raise_user_error(_T("xhttp_recv"), _T("http not send request"));
	}

	if (!stream_read_bytes(phttp->recv_stream, buf, pb))
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return 1;
ONERROR:

	XDL_TRACE_LAST;

	return 0;
}

bool_t xhttp_send_full(xhand_t xhttp, const byte_t* buf, dword_t len)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	dword_t n_size = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (phttp->type == _XHTTP_TYPE_CLI && !xhttp_is_requested(xhttp))
	{
		xhttp_get_request_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);
		if (is_null(fsize))
		{
			ltoxs(len, fsize, NUM_LEN);
			xhttp_set_request_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
		}

		if (!xhttp_send_request(xhttp))
			return 0;
	}
	
	if (phttp->type == _XHTTP_TYPE_SRV && !xhttp_is_responsed(xhttp))
	{
		xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);
		if (is_null(fsize))
		{
			ltoxs(len, fsize, NUM_LEN);
			xhttp_set_response_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
		}

		if (!xhttp_send_response(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->send_stream != NULL);

	if (!stream_write_bytes(phttp->send_stream, buf, len))
	{
		return 0;
	}

	return stream_flush(phttp->send_stream);
}

bool_t xhttp_recv_full(xhand_t xhttp, byte_t** pbuf, dword_t* plen)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	dword_t nlen, npos, nbys;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	if (phttp->type == _XHTTP_TYPE_CLI && !xhttp_is_responsed(xhttp))
	{
		if (!xhttp_recv_response(xhttp))
			return 0;
	}
	
	if (phttp->type == _XHTTP_TYPE_SRV && !xhttp_is_requested(xhttp))
	{
		if (!xhttp_recv_request(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->recv_stream != NULL);

	if (xhttp_is_chunked_recv(xhttp))
	{
		nlen = npos = 0;

		do{
			if (!stream_read_chunk_size(phttp->recv_stream, &nbys))
				return 0;

			if (!nbys)
				break;

			nlen += nbys;
			bytes_realloc(pbuf, nlen);

			if (!stream_read_bytes(phttp->recv_stream, *pbuf + npos, &nbys))
				break;

			npos += nbys;
		} while (nbys);

		*plen = npos;
		return (!nbys) ? 1 : 0;
	}
	else
	{
		if (phttp->type == _XHTTP_TYPE_CLI)
			nlen = xhttp_get_response_content_length(xhttp);
		else
			nlen = xhttp_get_request_content_length(xhttp);

		bytes_realloc(pbuf, nlen);

		if (!stream_read_bytes(phttp->recv_stream, *pbuf, &nlen))
		{
			bytes_realloc(pbuf, 0);
			*plen = 0;
			return 0;
		}

		*plen = nlen;
		return 1;
	}
}

bool_t xhttp_send_error(xhand_t xhttp, const tchar_t* http_code, const tchar_t* http_info, const tchar_t* errcode, const tchar_t* errtext, int slen)
{
	int type;
	byte_t* sz_buf = NULL;
	dword_t nlen;
	bool_t b_rt;
	tchar_t encoding[RES_LEN + 1] = { 0 };
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	int encode = 0;
	bool_t b_json = 0;

	type = xhttp_type(xhttp);

	XDL_ASSERT(type == _XHTTP_TYPE_SRV);

	xhttp_get_request_content_type(xhttp, encoding, RES_LEN);

	b_json = (compare_text(encoding, xslen(HTTP_HEADER_CONTENTTYPE_APPJSON), HTTP_HEADER_CONTENTTYPE_APPJSON, -1, 1) == 0) ? 1 : 0;

	xhttp_get_request_accept_charset(xhttp, encoding, RES_LEN);
	if (is_null(encoding))
	{
		xhttp_get_request_content_type_charset(xhttp, encoding, RES_LEN);
	}

	encode = parse_charset(encoding);
	if (encode)
	{
		format_charset(encode, encoding);
	}
	else
	{
		xscpy(encoding, CHARSET_UTF8);
	}

	nlen = xhttp_format_error(b_json, encoding, errcode, errtext, slen, NULL, MAX_LONG);
	sz_buf = (byte_t*)xmem_alloc(nlen + 1);
	nlen = xhttp_format_error(b_json, encoding, errcode, errtext, slen, sz_buf, nlen);

	xsprintf(fsize, _T("%d"), nlen);
	xhttp_set_response_header(xhttp, HTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
	if (b_json)
		xhttp_set_response_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPJSON, -1);
	else
		xhttp_set_response_content_type(xhttp, HTTP_HEADER_CONTENTTYPE_APPXML,-1);
	xhttp_set_response_content_type_charset(xhttp, encoding,-1);

	if (!is_null(http_code))
		xhttp_set_response_code(xhttp, http_code);
	if (!is_null(http_info))
		xhttp_set_response_message(xhttp, http_info, -1);

	b_rt = xhttp_send_full(xhttp, sz_buf, nlen);

	xmem_free(sz_buf);
	sz_buf = NULL;

	return b_rt;
}

bool_t xhttp_recv_error(xhand_t xhttp, tchar_t* http_code, tchar_t* http_info, tchar_t* errcode, tchar_t* errtext, int max)
{
	int type;
	byte_t** pbuf = NULL;
	tchar_t encoding[RES_LEN + 1] = { 0 };
	dword_t nlen = 0;
	bool_t b_rt;
	bool_t b_json = 0;

	type = xhttp_type(xhttp);

	XDL_ASSERT(type == _XHTTP_TYPE_CLI);

	xhttp_get_response_content_type(xhttp, encoding, RES_LEN);

	b_json = (compare_text(encoding, xslen(HTTP_HEADER_CONTENTTYPE_APPJSON), HTTP_HEADER_CONTENTTYPE_APPJSON, -1, 1) == 0) ? 1 : 0;

	xhttp_get_response_content_type_charset(xhttp, encoding, RES_LEN);
	if (is_null(encoding))
	{
		xscpy(encoding, CHARSET_UTF8);
	}

	if (http_code)
	{
		xhttp_get_response_code(xhttp, http_code);
	}
	if (http_info)
	{
		xhttp_get_response_message(xhttp, http_info, META_LEN);
	}

	pbuf = bytes_alloc();

	b_rt = xhttp_recv_full(xhttp, pbuf, &nlen);

	if (b_rt)
	{
		b_rt = xhttp_parse_error(b_json, encoding, *pbuf, nlen, errcode, errtext, max);
	}

	bytes_free(pbuf);
	pbuf = NULL;

	return b_rt;
}

bool_t xhttp_send_xml(xhand_t xhttp,link_t_ptr xml)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	type = xhttp_type(xhttp);

	if (type == _XHTTP_TYPE_CLI && !xhttp_is_requested(xhttp))
	{
		if (!xhttp_send_request(xhttp))
			return 0;
	}

	if (type == _XHTTP_TYPE_SRV && !xhttp_is_responsed(xhttp))
	{
		if (!xhttp_send_response(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->send_stream != NULL);

	if (!format_xml_doc_to_stream(xml, phttp->send_stream))
		return 0;

	return stream_flush(phttp->send_stream);
}

bool_t xhttp_recv_xml(xhand_t xhttp,link_t_ptr xml)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	type = xhttp_type(xhttp);

	if (type == _XHTTP_TYPE_CLI && !xhttp_is_responsed(xhttp))
	{
		if (!xhttp_recv_response(xhttp))
			return 0;
	}

	if (type == _XHTTP_TYPE_SRV && !xhttp_is_requested(xhttp))
	{
		if (!xhttp_recv_request(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->recv_stream != NULL);

	return parse_xml_doc_from_stream(xml, phttp->recv_stream);
}

bool_t xhttp_send_json(xhand_t xhttp, link_t_ptr json)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	type = xhttp_type(xhttp);

	if (type == _XHTTP_TYPE_CLI && !xhttp_is_requested(xhttp))
	{
		if (!xhttp_send_request(xhttp))
			return 0;
	}

	if (type == _XHTTP_TYPE_SRV && !xhttp_is_responsed(xhttp))
	{
		if (!xhttp_send_response(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->send_stream != NULL);

	if (!format_json_doc_to_stream(json, phttp->send_stream))
		return 0;

	return stream_flush(phttp->send_stream);
}

bool_t xhttp_recv_json(xhand_t xhttp, link_t_ptr json)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);
	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	type = xhttp_type(xhttp);

	if (type == _XHTTP_TYPE_CLI && !xhttp_is_responsed(xhttp))
	{
		if (!xhttp_recv_response(xhttp))
			return 0;
	}

	if (type == _XHTTP_TYPE_SRV && !xhttp_is_requested(xhttp))
	{
		if (!xhttp_recv_request(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->recv_stream != NULL);

	return parse_json_doc_from_stream(json, phttp->recv_stream);
}

bool_t xhttp_send_string(xhand_t xhttp, string_t var)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	type = xhttp_type(xhttp);

	if (type == _XHTTP_TYPE_CLI && !xhttp_is_requested(xhttp))
	{
		if (!xhttp_send_request(xhttp))
			return 0;
	}

	if (type == _XHTTP_TYPE_SRV && !xhttp_is_responsed(xhttp))
	{
		if (!xhttp_send_response(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->send_stream != NULL);

	return stream_write_line(phttp->send_stream, var, NULL);
}

bool_t xhttp_recv_string(xhand_t xhttp, string_t var)
{
	xhttp_t* phttp = TypePtrFromHead(xhttp_t, xhttp);

	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_INET);

	type = xhttp_type(xhttp);

	if (type == _XHTTP_TYPE_CLI && !xhttp_is_responsed(xhttp))
	{
		if (!xhttp_recv_response(xhttp))
			return 0;
	}

	if (type == _XHTTP_TYPE_SRV && !xhttp_is_requested(xhttp))
	{
		if (!xhttp_recv_request(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->recv_stream != NULL);

	return stream_read_line(phttp->recv_stream, var, NULL);
}



#endif /*XDK_SUPPORT_SOCK*/
