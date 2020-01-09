/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl http document

	@module	xdlinet.c | xdl http implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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
#include "stringtable.h"
#include "xdlutil.h"
#include "xdlmem.h"
#include "xdlerr.h"
#include "xdlstr.h"
#include "xdlxml.h"
#include "xdlcode.h"
#include "xdlsock.h"
#include "xdlstm.h"
#include "xdlvar.h"
#include "xdltime.h"
#include "xdlesc.h"
#include "xdlbuf.h"
#include "xmldoc.h"
#include "domdoc.h"
#include "xdlbas.h"
#include "oemmd5.h"
#include "oemsha.h"
#include "oemzlib.h"

#ifdef XDL_SUPPOTR_HTTP

static tchar_t url_asc[] = { _T(' '), _T('"'),_T('#'), _T('%'), _T('&'), _T('('), _T(')'), _T('+'), _T(','), _T('/'), _T(':'), _T(';'), _T('<'), _T('='), _T('>'), _T('?'), _T('@'), _T('\\'), _T('|'), _T('\0') };

//#define ASC_LEN	16
//static tchar_t url_asc[ASC_LEN] = { _T(' '), _T('"'), _T('#'), _T('%'), _T('('), _T(')'), _T('+'), _T(','), _T(':'), _T(';'), _T('<'), _T('>'), _T('?'), _T('@'), _T('\\'), _T('|'), _T('\0')  };

typedef struct _xhttp_t{
	xhand_head head;		//head for xhand_t

	int type;
	bool_t secu;
	xhand_t socket;
	xhand_t send_stream;
	xhand_t recv_stream;
	link_t_ptr st_request,st_response;
	bool_t b_request,b_response;

	tchar_t method[RES_LEN + 1];
	tchar_t poto[RES_LEN + 1];
	tchar_t version[NUM_LEN + 1];
	tchar_t host[RES_LEN + 1];
	tchar_t addr[RES_LEN + 1];
	tchar_t port[NUM_LEN + 1];
	tchar_t object[PATH_LEN + 1];
	tchar_t *query;

	tchar_t code[NUM_LEN + 1];
	tchar_t text[MAX_ERR + 1];
}xhttp_t;

#ifdef XDL_SUPPOTR_HTTP_SRV

typedef struct _xhttp_listen_t{
	xhand_head head;			//reserved for xhand_t

	int type;
	xhand_t socket;

	void *pf_param;
	PF_XHTTP_ACCEPT pf_accept;
}xhttp_listen_t;

#endif

/////////////////////////////////////////////////////////////////////////////////////////////

void xhttp_parse_request(xhand_t xhttp, byte_t* data, int len)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	byte_t *token = data;
	byte_t *key, *val;
	int klen, vlen;
	tchar_t *tkey, *tval;
	int tlen;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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
#ifdef _UNICODE
		tlen = utf8_to_ucs(key, klen, NULL, MAX_LONG);
#else
		tlen = utf8_to_mbs(key, klen, NULL, MAX_LONG);
#endif

		phttp->query = xsalloc(tlen + 1);

#ifdef _UNICODE
		tlen = utf8_to_ucs(key, klen, phttp->query, tlen);
#else
		tlen = utf8_to_mbs(key, klen, phttp->query, tlen);
#endif
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

int xhttp_format_request(xhand_t xhttp, byte_t* buf, dword_t max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	dword_t total = 0;
	link_t_ptr ent;
	const tchar_t *key, *val;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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
	if (!is_null(phttp->query))
	{
		if (buf)
		{
			buf[total] = '?';
		}
		total++;
		if (total >= max)
			return total;

#ifdef _UNICODE
		total += ucs_to_utf8(phttp->query, -1, ((buf) ? buf + total : NULL), max - total);
#else
		total += mbs_to_utf8(phttp->query, -1, ((buf) ? buf + total : NULL), max - total);
#endif
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

void xhttp_parse_response(xhand_t xhttp, byte_t* data, dword_t len)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	byte_t *token = data;
	byte_t *key, *val;
	int klen, vlen;
	tchar_t *tkey, *tval;
	int tlen;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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
		utf8_to_ucs(val, vlen, phttp->text, MAX_ERR);
#else
		utf8_to_mbs(val, vlen, phttp->text, MAX_ERR);
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

int xhttp_format_response(xhand_t xhttp, byte_t* buf, dword_t max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	dword_t total = 0;
	link_t_ptr ent;
	const tchar_t *key, *val;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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

int xhttp_format_continue(xhand_t xhttp, byte_t* buf, dword_t max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	dword_t total = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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
	total += ucs_to_utf8(XHTTP_CODE_100, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(XHTTP_CODE_100, -1, ((buf) ? buf + total : NULL), max - total);
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
	total += ucs_to_utf8(XHTTP_CODE_100_TEXT, -1, ((buf) ? buf + total : NULL), max - total);
#else
	total += mbs_to_utf8(XHTTP_CODE_100_TEXT, -1, ((buf) ? buf + total : NULL), max - total);
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
////////////////////////////////////////////////////////////////////////////////////////////
bool_t _is_ip(const tchar_t* addr)
{
	if(addr[0] >= _T('0') && addr[0] <= _T('9'))
	{
		if((addr[1] >= _T('0') && addr[1] <= _T('9')) || addr[1] == _T('.'))
		{
			if((addr[2] >= _T('0') && addr[2] <= _T('9')) || addr[2] == _T('.'))
				return 1;
		}
	}

	return 0;
}

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
			xscpy(sz_site, sz_object);
			if (sz_site[0] == _T('\\'))
				sz_site[0] = _T('/');
		}

		if (sz_file)
		{
			xscpy(sz_file, token);
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

xhand_t  xhttp_open(const tchar_t* app,const tchar_t* method,const tchar_t* url)
{
	tchar_t *potoat,*hostat,*portat,*objat,*qryat;
	int potolen,hostlen,portlen,objlen,qrylen;
	xhttp_t* phttp;

	if (is_null(url))
	{
		set_last_error(_T("xhttp_open"), _T("url is empty"), -1);
		return 0;
	}

	phttp = (xhttp_t*)xmem_alloc(sizeof(xhttp_t));
	phttp->head.tag = _HANDLE_HTTP;
	phttp->head.cbs = sizeof(xhttp_t);

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
			phttp->secu = 1;
			xscpy(phttp->poto, _T("HTTP"));
		}
	}
	else
	{
		xscpy(phttp->poto, _T("HTTP"));
	}

	xscpy(phttp->version, _T("1.1"));

	if(hostlen)
	{
		xsncpy(phttp->host, hostat, hostlen);

		if(_is_ip(phttp->host))
		{
			xscpy(phttp->addr,phttp->host);
		}else
		{
			host_addr(phttp->host,phttp->addr);
		}
	}

	if(is_null(phttp->addr))
	{
		xmem_free(phttp);

		set_last_error(_T("xhttp_open"), _T("host address is empty"), -1);
		return 0;
	}

	if (portlen)
	{
		xsncpy(phttp->port, portat, portlen);
	}
	else
	{
		if(phttp->secu)
			xscpy(phttp->port, _T("443"));
		else
			xscpy(phttp->port, _T("80"));
	}

	if (objlen)
		xsncpy(phttp->object, objat, objlen);
	else
		xscpy(phttp->object,_T("/"));

	if(qrylen)
	{
		phttp->query = xsalloc(qrylen + 1);
		xsncpy(phttp->query, qryat, qrylen);
	}

	phttp->socket = xsocket_cli(phttp->secu, xstos(phttp->port), phttp->addr);
	if(!phttp->socket)
	{
		xmem_free(phttp);
		return 0;
	}

	phttp->st_request = create_string_table();
	phttp->st_response = create_string_table();

	return (xhand_t)phttp;
}

void xhttp_set_query_entity(xhand_t xhttp, const tchar_t* key, int klen, const tchar_t* val, int vlen)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int len,tklen = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_CLI);

	if (klen < 0)
		klen = xslen(key);
	if (!klen)
		return;

	if (vlen < 0)
		vlen = xslen(val);

	tklen += encode_escape(url_asc, key, klen, NULL, MAX_LONG);
	tklen++; //=
	tklen += encode_escape(url_asc, val, vlen, NULL, MAX_LONG);

	len = xslen(phttp->query);
	if (len)
	{
		phttp->query = xsrealloc(phttp->query, len + tklen + 2);
		xsncpy(phttp->query, _T("&"), 1);
		tklen = len + 1;
	}
	else
	{
		phttp->query = xsrealloc(phttp->query, tklen + 1);
		tklen = 0;
	}
	
	tklen = encode_escape(url_asc, key, klen, phttp->query + tklen, MAX_LONG);

	xsncpy(phttp->query + tklen, _T("="), 1);
	tklen++;

	encode_escape(url_asc, val, vlen, phttp->query + tklen, MAX_LONG);
}

int xhttp_get_query_entity(xhand_t xhttp, const tchar_t* key, int len, tchar_t* buf, int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	const tchar_t* token = NULL;
	tchar_t *kstr, *vstr;
	int klen, vlen;
	tchar_t *tmp = NULL;
	int tlen = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	tlen = encode_escape(url_asc, key, len, NULL, MAX_LONG);
	tmp = xsalloc(tlen + 1);
	encode_escape(url_asc, key, len, tmp, tlen); 

	token = phttp->query;
	while (token)
	{
		token = parse_options_token(token, -1, _T('='), _T('&'), &kstr, &klen, &vstr, &vlen);
		if (compare_text(tmp, tlen, kstr, klen, 1) == 0)
		{
			vlen = decode_escape(vstr, vlen, buf, max);
			xsfree(tmp);
			return vlen;
		}
	}

	xsfree(tmp);

	if (buf)
	{
		buf[0] = _T('\0');
	}

	return 0;
}

void xhttp_close(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);
	XDL_ASSERT(phttp->type == _XHTTP_TYPE_CLI);

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

	if (phttp->socket)
		xsocket_close(phttp->socket);

	xmem_free(phttp);
}

xhand_t	xhttp_get_send_stream(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	return phttp->send_stream;
}

xhand_t	xhttp_get_recv_stream(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	return phttp->recv_stream;
}

int xhttp_type(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	return phttp->type;
}

int	xhttp_get_url_method(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	len = xslen(phttp->method);
	len = (len < max) ? len : max;
	xsncpy(buf, phttp->method, len);
	return len;
}

int	xhttp_get_url_host(xhand_t xhttp,tchar_t* buf,int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	len = xslen(phttp->host);
	len = (len < max) ? len : max;
	xsncpy(buf, phttp->host, len);
	return len;
}

int	xhttp_get_url_port(xhand_t xhttp,tchar_t* buf,int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	len = xslen(phttp->port);
	len = (len < max) ? len : max;
	xsncpy(buf, phttp->port, len);
	return len;
}

int	xhttp_get_url_object(xhand_t xhttp,tchar_t* buf,int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	len = xslen(phttp->object);
	len = (len < max) ? len : max;
	xsncpy(buf, phttp->object, len);
	return len;
}

int	xhttp_get_url_query(xhand_t xhttp,tchar_t* buf,int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	len = xslen(phttp->query);
	len = (len < max) ? len : max;
	xsncpy(buf, phttp->query, len);
	return len;
}

const tchar_t* xhttp_get_url_query_ptr(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	return phttp->query;
}

void xhttp_set_request_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	write_string_entity(phttp->st_request,hname,nlen,hvalue,vlen);
}

int xhttp_get_request_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* hvalue, int hmax)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	return read_string_entity(phttp->st_request, hname, nlen, hvalue, hmax);
}

void xhttp_set_request_default_header(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	tchar_t szHost[PATH_LEN + 1] = { 0 };
	tchar_t szPort[NUM_LEN + 1] = { 0 };
	tchar_t szDate[NUM_LEN + 1] = { 0 };
	xdate_t dt = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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

	xhttp_set_request_header(xhttp, XHTTP_HEADER_HOST, -1, szHost, -1);
	xhttp_set_request_header(xhttp, XHTTP_HEADER_DATE, -1, szDate, -1);
	xhttp_set_request_header(xhttp, XHTTP_HEADER_CONNECTION, -1, XHTTP_HEADER_CONNECTION_CLOSE, -1);
}

void xhttp_get_response_code(xhand_t xhttp, tchar_t* buf)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	
	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	xsncpy(buf,phttp->code,NUM_LEN);
}

void xhttp_set_response_code(xhand_t xhttp, const tchar_t* code)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	xsncpy(phttp->code, code, NUM_LEN);
}

int	xhttp_get_response_message(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	len = xslen(phttp->text);
	len = (len < max) ? len : max;
	if (buf)
		xsncpy(buf, phttp->text, len);

	return len;
}

void xhttp_set_response_message(xhand_t xhttp, const tchar_t* msg, int len)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (len < 0)
		len = xslen(msg);
	len = (len < MAX_ERR) ? len : MAX_ERR;
	xsncpy(phttp->text, msg, len);
}

int xhttp_get_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* hvalue, int vmax)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	return read_string_entity(phttp->st_response, hname, nlen, hvalue, vmax);
}

void xhttp_set_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	write_string_entity(phttp->st_response, hname, nlen, hvalue, vlen);
}

void xhttp_set_response_default_header(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	tchar_t szDate[DATE_LEN + 1] = { 0 };
	xdate_t dt = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	get_utc_date(&dt);
	format_gmttime(&dt, szDate);

	clear_string_table(phttp->st_response);

	xhttp_set_response_header(xhttp, XHTTP_HEADER_CONNECTION, -1, XHTTP_HEADER_CONNECTION_CLOSE, -1);
	xhttp_set_response_header(xhttp, XHTTP_HEADER_DATE, -1, szDate, -1);
	xhttp_set_response_header(xhttp, XHTTP_HEADER_SERVER, -1, XHTTP_HEADER_SERVER_XSERVICE, -1);
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

	xhttp_set_response_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, sz_size, -1);
}

dword_t xhttp_get_response_content_length(xhand_t xhttp)
{
	tchar_t sz_size[NUM_LEN + 1] = { 0 };
	long n_size;

	xhttp_get_response_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, sz_size, NUM_LEN);

	n_size = xstol(sz_size);
	return (n_size < 0) ? 0 : n_size;
}

void xhttp_set_request_content_length(xhand_t xhttp, dword_t len)
{
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	xsprintf(sz_size, _T("%d"), len);

	xhttp_set_request_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, sz_size, -1);
}

dword_t xhttp_get_request_content_length(xhand_t xhttp)
{
	tchar_t sz_size[NUM_LEN + 1] = { 0 };

	xhttp_get_request_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, sz_size, NUM_LEN);

	return xstol(sz_size);
}

int xhttp_get_response_content_type_charset(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	const tchar_t* str;
	tchar_t *key, *val;
	int klen, vlen;
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (buf)
	{
		xszero(buf, max);
	}

	ent = get_string_entity(phttp->st_response, XHTTP_HEADER_CONTENTTYPE, -1);
	if (!ent)
		return 0;

	str = get_string_entity_val_ptr(ent);
	if (is_null(str))
		return 0;

	str = xsistr(str, XHTTP_HEADER_TYPE_ENTITY_CHARSET);
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
	xhttp_t* phttp = (xhttp_t*)xhttp;
	tchar_t* str;
	tchar_t ctype[NAME_LEN + 1] = { 0 };
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	ent = get_string_entity(phttp->st_response, XHTTP_HEADER_CONTENTTYPE, -1);

	if (ent)
	{
		get_string_entity_val(ent, ctype, NAME_LEN);
	}
	else
	{
		xscpy(ctype, XHTTP_HEADER_TYPE_APPXML);
	}

	str = (tchar_t*)xsistr(ctype, XHTTP_HEADER_TYPE_ENTITY_CHARSET);
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
		xscat(ctype, XHTTP_HEADER_TYPE_ENTITY_CHARSET);
		xscat(ctype, _T("="));
		xsncat(ctype, token, len);
	}

	set_string_entity_val(ent, ctype, -1);
}

int xhttp_get_request_content_type_charset(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	const tchar_t* str;
	tchar_t *key, *val;
	int klen, vlen;
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (buf)
	{
		xszero(buf, max);
	}

	ent = get_string_entity(phttp->st_request, XHTTP_HEADER_CONTENTTYPE, -1);
	if (!ent)
		return 0;

	str = get_string_entity_val_ptr(ent);
	if (is_null(str))
		return 0;

	str = xsistr(str, XHTTP_HEADER_TYPE_ENTITY_CHARSET);
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
	xhttp_t* phttp = (xhttp_t*)xhttp;
	tchar_t* str;
	tchar_t ctype[NAME_LEN + 1] = { 0 };
	link_t_ptr ent;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	ent = get_string_entity(phttp->st_request, XHTTP_HEADER_CONTENTTYPE, -1);

	if (ent)
	{
		get_string_entity_val(ent, ctype, NAME_LEN);
	}
	else
	{
		xscpy(ctype, XHTTP_HEADER_TYPE_APPXML);
	}

	str = (tchar_t*)xsistr(ctype, XHTTP_HEADER_TYPE_ENTITY_CHARSET);
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
		xscat(ctype, XHTTP_HEADER_TYPE_ENTITY_CHARSET);
		xscat(ctype, _T("="));
		xsncat(ctype, token, len);
	}

	set_string_entity_val(ent, ctype, -1);
}

int	xhttp_get_addr(xhand_t xhttp, tchar_t* buf, int max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int len;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	len = xslen(phttp->addr);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, phttp->addr, len);
	}

	return len;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
int xhttp_format_error(const tchar_t* encoding, const tchar_t* errcode, const tchar_t* errtext, int slen, byte_t* buf, dword_t max)
{
	link_t_ptr ptr_xml, ptr_dom;
	link_t_ptr nlk;

	int nlen = 0;
	byte_t* sz_buf = NULL;

	ptr_xml = create_xml_doc();

	if (!is_null(encoding))
		set_xml_encoding(ptr_xml, encoding, -1);

	ptr_dom = get_xml_dom_node(ptr_xml);
	set_dom_node_name(ptr_dom, XHTTP_FAULT, -1);

	nlk = insert_dom_node(ptr_dom, LINK_LAST);
	set_dom_node_name(nlk, XHTTP_FAULT_CODE, -1);
	set_dom_node_text(nlk, errcode, -1);

	nlk = insert_dom_node(ptr_dom, LINK_LAST);
	set_dom_node_name(nlk, XHTTP_FAULT_STRING, -1);
	set_dom_node_text(nlk, errtext, slen);

	nlen = format_xml_doc_to_buffer(ptr_xml, buf, max);

	destroy_xml_doc(ptr_xml);

	return nlen;
}

bool_t xhttp_parse_error(const byte_t* buf, dword_t len, tchar_t* errcode, tchar_t* errtext, int max)
{
	link_t_ptr nlk,ptr_dom,ptr_xml;
	bool_t b_rt;

	ptr_xml = create_xml_doc();
	b_rt = parse_xml_doc_from_buffer(ptr_xml, buf, len);
	if (!b_rt)
	{
		destroy_xml_doc(ptr_xml);
		return 0;
	}

	ptr_dom = get_xml_dom_node(ptr_xml);
	if (compare_text(get_dom_node_name_ptr(ptr_dom), -1, XHTTP_FAULT, -1, 1) != 0)
	{
		destroy_xml_doc(ptr_xml);
		return 0;
	}

	nlk = get_dom_first_child_node(ptr_dom);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, XHTTP_FAULT_CODE, -1, 1) == 0)
		{
			if (errcode)
			{
				get_dom_node_text(nlk, errcode, NUM_LEN);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, XHTTP_FAULT_STRING, -1, 1) == 0)
		{
			if (errtext)
			{
				get_dom_node_text(nlk, errtext, max);
			}
		}
		nlk = get_dom_next_sibling_node(nlk);
	}

	destroy_xml_doc(ptr_xml);

	return 1;
}

bool_t	xhttp_is_requested(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	return  phttp->b_request;
}

bool_t	xhttp_is_responsed(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	return phttp->b_response;
}

void xhttp_reset_request(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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
	xhttp_t* phttp = (xhttp_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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
	xszero(phttp->text, MAX_ERR);
}

void xhttp_send_continue(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	byte_t *buf_response;
	dword_t len_response;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_SRV)
	{
		len_response = xhttp_format_continue(xhttp, NULL, MAX_LONG);
		buf_response = (byte_t*)xmem_alloc(len_response + 1);

		len_response = xhttp_format_continue(xhttp, buf_response, len_response);

		xsocket_set_send_buff(phttp->socket, 0);

		xsocket_write(phttp->socket, buf_response, &len_response);

		xmem_free(buf_response);
	}
}

bool_t xhttp_send_response(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	byte_t *buf_response;
	dword_t len_response;
	tchar_t charset[INT_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_SRV)
	{
		if (!phttp->b_response)
		{
			sort_string_table(phttp->st_response, 0);

			len_response = xhttp_format_response(xhttp, NULL, MAX_LONG);
			buf_response = (byte_t*)xmem_alloc(len_response);

			len_response = xhttp_format_response(xhttp, buf_response, len_response);

			xsocket_set_send_buff(phttp->socket, 0);

			if (xsocket_write(phttp->socket, buf_response, &len_response))
			{
				phttp->b_response = 1;
			}

			if (phttp->b_response)
			{
				phttp->send_stream = stream_alloc(phttp->socket);

				xhttp_get_response_content_type_charset(xhttp, charset, INT_LEN);
				if (!is_null(charset))
				{
					stream_set_encode(phttp->send_stream, parse_charset(charset));
				}

				if (xhttp_is_chunked_send(xhttp))
				{
					stream_set_mode(phttp->send_stream, CHUNK_OPERA);
					stream_opera_reset(phttp->send_stream);
					xsocket_set_send_buff(phttp->socket, SOCKET_BASE_BUFF);
#ifndef _DEBUG
					xsocket_set_send_timeout(phttp->socket, SOCKET_BASE_TIMO);
#endif
				}
				else
				{
					len_response = xhttp_get_response_content_length(xhttp);
					stream_set_size(phttp->send_stream, len_response);
					xsocket_set_send_buff(phttp->socket, ((len_response) ? len_response : SOCKET_BASE_BUFF));
#ifndef _DEBUG
					xsocket_set_send_timeout(phttp->socket, (len_response / SOCKET_BASE_BUFF + 1) * SOCKET_BASE_TIMO);
#endif
				}
			}

			xmem_free(buf_response);
		}

		return phttp->b_response;
	}

	return 0;
}

bool_t xhttp_recv_response(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	dword_t len_header, len_response,len_one;
	byte_t* buf_response;
	tchar_t charset[INT_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_CLI)
	{
		if (!phttp->b_response)
		{
			xsocket_set_recv_buff(phttp->socket, 0);

			len_header = XHTTP_MAX_HEADER;
			buf_response = (byte_t*)xmem_alloc(len_header);
			len_response = 0;

			while (1)
			{
				len_one = 1;
				if (!xsocket_read(phttp->socket, buf_response + len_response, &len_one))
					break;

				if (!len_one)
					break;

				len_response++;

				if (len_response > len_header)
				{
					len_header += XHTTP_INC_HEADER;
					buf_response = (byte_t*)xmem_realloc(buf_response, len_header);
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
				phttp->recv_stream = stream_alloc(phttp->socket);

				xhttp_parse_response(xhttp, buf_response, len_response);

				xhttp_get_response_content_type_charset(xhttp, charset, INT_LEN);
				if (!is_null(charset))
				{
					stream_set_encode(phttp->recv_stream, parse_charset(charset));
				}

				if (xhttp_is_chunked_recv(xhttp))
				{
					stream_set_mode(phttp->recv_stream, CHUNK_OPERA);
					stream_opera_reset(phttp->recv_stream);
					xsocket_set_recv_buff(phttp->socket, SOCKET_BASE_BUFF);
#ifndef _DEBUG
					xsocket_set_recv_timeout(phttp->socket, SOCKET_BASE_TIMO);
#endif
				}
				else
				{
					len_one = xhttp_get_response_content_length(xhttp);
					stream_set_size(phttp->recv_stream, len_one);
					xsocket_set_recv_buff(phttp->socket, ((len_one) ? len_one : SOCKET_BASE_BUFF));
#ifndef _DEBUG
					xsocket_set_recv_timeout(phttp->socket, (len_one / SOCKET_BASE_BUFF + 1) * SOCKET_BASE_TIMO);
#endif
				}
			}

			xmem_free(buf_response);
		}

		return phttp->b_response;
	}

	return 0;
}

bool_t xhttp_send_request(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	byte_t *buf_request;
	dword_t len_request;
	tchar_t charset[INT_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_CLI)
	{
		if (!phttp->b_request)
		{
			sort_string_table(phttp->st_request, 0);

			len_request = xhttp_format_request(xhttp, NULL, MAX_LONG);
			buf_request = (byte_t*)xmem_alloc(len_request);

			len_request = xhttp_format_request(xhttp, buf_request, len_request);

			xsocket_set_send_buff(phttp->socket, 0);

			if (xsocket_write(phttp->socket, buf_request, &len_request))
			{
				phttp->b_request = 1;
			}

			if (phttp->b_request)
			{
				phttp->send_stream = stream_alloc(phttp->socket);

				xhttp_get_request_content_type_charset(xhttp, charset, INT_LEN);
				if (!is_null(charset))
				{
					stream_set_encode(phttp->send_stream, parse_charset(charset));
				}

				if (xhttp_is_chunked_send(xhttp))
				{
					stream_set_mode(phttp->send_stream, CHUNK_OPERA);
					stream_opera_reset(phttp->send_stream);
					xsocket_set_send_buff(phttp->socket, SOCKET_BASE_BUFF);
#ifndef _DEBUG
					xsocket_set_send_timeout(phttp->socket, SOCKET_BASE_TIMO);
#endif
				}
				else
				{
					len_request = xhttp_get_request_content_length(xhttp);
					stream_set_size(phttp->send_stream, len_request);
					xsocket_set_send_buff(phttp->socket, ((len_request) ? len_request : SOCKET_BASE_BUFF));
#ifndef _DEBUG
					xsocket_set_send_timeout(phttp->socket, (len_request / SOCKET_BASE_BUFF + 1) * SOCKET_BASE_TIMO);
#endif
				}
			}

			xmem_free(buf_request);
		}

		return  phttp->b_request;
	}

	return 0;
}

bool_t xhttp_recv_request(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	dword_t len_header, len_request,len_one;
	byte_t* buf_request;
	tchar_t charset[INT_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_SRV)
	{
		if (!phttp->b_request)
		{
			xsocket_set_recv_buff(phttp->socket, 0);

			len_header = XHTTP_MAX_HEADER;
			buf_request = (byte_t*)xmem_alloc(len_header);
			len_request = 0;

			while (1)
			{
				len_one = 1;
				if (!xsocket_read(phttp->socket, buf_request + len_request, &len_one))
					break;

				if (!len_one)
					break;

				len_request++;

				if (len_request > len_header)
				{
					len_header += XHTTP_INC_HEADER;
					buf_request = (byte_t*)xmem_realloc(buf_request, len_header);
				}

				if (len_request >= 4)
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
				phttp->recv_stream = stream_alloc(phttp->socket);

				xhttp_parse_request((xhand_t)phttp, buf_request, len_request);

				xhttp_get_request_content_type_charset(xhttp, charset, INT_LEN);
				if (!is_null(charset))
				{
					stream_set_encode(phttp->recv_stream, parse_charset(charset));
				}

				if (xhttp_is_chunked_recv(xhttp))
				{
					stream_set_mode(phttp->recv_stream, CHUNK_OPERA);
					stream_opera_reset(phttp->recv_stream);
					xsocket_set_recv_buff(phttp->socket, SOCKET_BASE_BUFF);
#ifndef _DEBUG
					xsocket_set_recv_timeout(phttp->socket, SOCKET_BASE_TIMO);
#endif
				}
				else
				{
					len_one = xhttp_get_request_content_length(xhttp);
					stream_set_size(phttp->recv_stream, len_one);
					xsocket_set_recv_buff(phttp->socket, ((len_one) ? len_one : SOCKET_BASE_BUFF));
#ifndef _DEBUG
					xsocket_set_recv_timeout(phttp->socket, (len_one / SOCKET_BASE_BUFF + 1) * SOCKET_BASE_TIMO);
#endif
				}
			}

			xmem_free(buf_request);
		}

		return phttp->b_request;
	}

	return 0;
}

bool_t xhttp_is_chunked_recv(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_response)
		return 0;

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_request)
		return 0;

	if (phttp->type == _XHTTP_TYPE_CLI)
		xhttp_get_response_header(xhttp, XHTTP_HEADER_TRANSFERENCODING, -1, token, RES_LEN);
	else
		xhttp_get_request_header(xhttp, XHTTP_HEADER_TRANSFERENCODING, -1, token, RES_LEN);

	return (compare_text(token, -1, XHTTP_HEADER_TRANSFERENCODING_CHUNKED, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_is_chunked_send(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_request)
		return 0;

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_response)
		return 0;

	if (phttp->type == _XHTTP_TYPE_CLI)
		xhttp_get_request_header(xhttp, XHTTP_HEADER_TRANSFERENCODING, -1, token, RES_LEN);
	else
		xhttp_get_response_header(xhttp, XHTTP_HEADER_TRANSFERENCODING, -1, token, RES_LEN);

	return (compare_text(token, -1, XHTTP_HEADER_TRANSFERENCODING_CHUNKED, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_is_zipped_recv(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_response)
		return 0;

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_request)
		return 0;

	if (phttp->type == _XHTTP_TYPE_CLI)
		xhttp_get_response_header(xhttp, XHTTP_HEADER_CONTENTENCODING, -1, token, RES_LEN);
	else
		xhttp_get_request_header(xhttp, XHTTP_HEADER_CONTENTENCODING, -1, token, RES_LEN);

	return (compare_text(token, -1, XHTTP_HEADER_ENCODING_DEFLATE, -1, 1) == 0 || compare_text(token, -1, XHTTP_HEADER_ENCODING_GZIP, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_is_zipped_send(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_request)
		return 0;

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_response)
		return 0;

	if (phttp->type == _XHTTP_TYPE_CLI)
		xhttp_get_request_header(xhttp, XHTTP_HEADER_CONTENTENCODING, -1, token, RES_LEN);
	else
		xhttp_get_response_header(xhttp, XHTTP_HEADER_CONTENTENCODING, -1, token, RES_LEN);

	return (compare_text(token, -1, XHTTP_HEADER_ENCODING_DEFLATE, -1, 1) == 0 || compare_text(token, -1, XHTTP_HEADER_ENCODING_GZIP, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_need_expect(xhand_t xhttp)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	tchar_t token[RES_LEN + 1] = { 0 };

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	xhttp_get_request_header(xhttp, XHTTP_HEADER_EXPECT, -1, token, RES_LEN);
	
	return (compare_text(phttp->method, -1, XHTTP_METHOD_POST,-1,1) == 0 && compare_text(token, -1, XHTTP_HEADER_EXPECT_CONTINUE, -1, 1) == 0) ? 1 : 0;
}

bool_t xhttp_send(xhand_t xhttp, const byte_t* data, dword_t len)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	dword_t dw = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);
	XDL_ASSERT(phttp->send_stream != NULL);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_request)
	{
		set_last_error(_T("xhttp_send"), _T("xhttp not send request"), -1);
		return 0;
	}

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_response)
	{
		set_last_error(_T("xhttp_send"), _T("xhttp not recv response"), -1);
		return 0;
	}

	return stream_write_bytes(phttp->send_stream, data, len, &dw);
}

bool_t xhttp_recv(xhand_t xhttp,byte_t* buf,dword_t max)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	dword_t dw = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);
	XDL_ASSERT(phttp->recv_stream != NULL);

	if (phttp->type == _XHTTP_TYPE_CLI && !phttp->b_response)
	{
		set_last_error(_T("xhttp_recv"), _T("http not recv response"), -1);
		return 0;
	}

	if (phttp->type == _XHTTP_TYPE_SRV && !phttp->b_request)
	{
		set_last_error(_T("xhttp_recv"), _T("http not send request"), -1);
		return 0;
	}

	return stream_read_bytes(phttp->recv_stream, buf, max, &dw);
}

bool_t xhttp_send_full(xhand_t xhttp, const byte_t* buf, dword_t len)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	dword_t n_size = 0;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

	if (phttp->type == _XHTTP_TYPE_CLI && !xhttp_is_requested(xhttp))
	{
		xhttp_get_request_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);
		if (is_null(fsize))
		{
			ltoxs(len, fsize, NUM_LEN);
			xhttp_set_request_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
		}

		if (!xhttp_send_request(xhttp))
			return 0;
	}
	
	if (phttp->type == _XHTTP_TYPE_SRV && !xhttp_is_responsed(xhttp))
	{
		xhttp_get_response_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, fsize, NUM_LEN);
		if (is_null(fsize))
		{
			ltoxs(len, fsize, NUM_LEN);
			xhttp_set_response_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
		}

		if (!xhttp_send_response(xhttp))
			return 0;
	}

	XDL_ASSERT(phttp->send_stream != NULL);

	return stream_write_bytes(phttp->send_stream, buf, len, &n_size);
}

bool_t xhttp_recv_full(xhand_t xhttp, byte_t** pbuf, dword_t* plen)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	dword_t nlen, npos, nbys;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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
			xmem_realloc_buffer(pbuf, nlen);

			if (!stream_read_bytes(phttp->recv_stream, *pbuf + npos, nbys, &nbys))
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

		xmem_realloc_buffer(pbuf, nlen);

		if (!stream_read_bytes(phttp->recv_stream, *pbuf, nlen, plen))
		{
			xmem_realloc_buffer(pbuf, 0);
			*plen = 0;
			return 0;
		}

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

	type = xhttp_type(xhttp);

	XDL_ASSERT(type == _XHTTP_TYPE_SRV);

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
		xscpy(encoding, XML_ATTR_ENCODING_UTF8);
	}

	nlen = xhttp_format_error(encoding, errcode, errtext, slen, NULL, MAX_LONG);
	sz_buf = (byte_t*)xmem_alloc(nlen + 1);
	nlen = xhttp_format_error(encoding, errcode, errtext, slen, sz_buf, nlen);

	xsprintf(fsize, _T("%d"), nlen);
	xhttp_set_response_header(xhttp, XHTTP_HEADER_CONTENTLENGTH, -1, fsize, -1);
	xhttp_set_response_content_type(xhttp, XHTTP_HEADER_TYPE_APPXML,-1);
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
	dword_t nlen = 0;
	bool_t b_rt;

	type = xhttp_type(xhttp);

	XDL_ASSERT(type == _XHTTP_TYPE_CLI);

	if (http_code)
	{
		xhttp_get_response_code(xhttp, http_code);
	}
	if (http_info)
	{
		xhttp_get_response_message(xhttp, http_info, NAME_LEN);
	}

	pbuf = xmem_alloc_buffer();

	b_rt = xhttp_recv_full(xhttp, pbuf, &nlen);

	if (b_rt)
	{
		b_rt = xhttp_parse_error(*pbuf, nlen, errcode, errtext, max);
	}

	xmem_free_buffer(pbuf);
	pbuf = NULL;

	return b_rt;
}

bool_t xhttp_send_xml(xhand_t xhttp,link_t_ptr xml)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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

	return format_xml_doc_to_stream(xml, phttp->send_stream);
}

bool_t xhttp_recv_xml(xhand_t xhttp,link_t_ptr xml)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;
	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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

bool_t xhttp_send_var(xhand_t xhttp, xhand_t var)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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

bool_t xhttp_recv_var(xhand_t xhttp, xhand_t var)
{
	xhttp_t* phttp = (xhttp_t*)xhttp;

	int type;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);

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

void xhttp_get_authorization(xhand_t xhttp, tchar_t* sz_sid, int slen, tchar_t* sz_sign, int max)
{
	tchar_t sz_auth[NAME_LEN + 1] = { 0 };
	int len;
	tchar_t *token;

	xhttp_get_request_header(xhttp, XHTTP_HEADER_AUTHORIZATION, -1, sz_auth, NAME_LEN);

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

void xhttp_set_authorization(xhand_t xhttp, const tchar_t* sz_sid, int slen, const tchar_t* sz_sign, int max)
{
	tchar_t sz_auth[NAME_LEN + 1] = { 0 };

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

	xhttp_set_request_header(xhttp, XHTTP_HEADER_AUTHORIZATION, -1, sz_auth, -1);
}

#ifdef XDL_SUPPORT_SHA
int xhttp_request_signature(xhand_t xhttp, const tchar_t* skey, tchar_t* buf, int max)
{
	tchar_t sz_verb[RES_LEN + 1] = { 0 };
	tchar_t sz_type[RES_LEN + 1] = { 0 };
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	tchar_t sz_action[PATH_LEN] = { 0 };
	tchar_t sz_object[PATH_LEN] = { 0 };
	tchar_t sz_signature[HMAC_LEN + 1] = { 0 };

	int acc_len, utf8_len, bas_len;
	int pos;

	byte_t* acc_buf = NULL;
	byte_t* utf8_buf = NULL;

	unsigned char dig[20] = { 0 };

	xhttp_get_url_method(xhttp, sz_verb, RES_LEN);
	xhttp_get_url_object(xhttp, sz_object, PATH_LEN);
	xhttp_get_request_header(xhttp, XHTTP_HEADER_CONTENTTYPE, -1, sz_type, RES_LEN);
	xhttp_get_request_header(xhttp, XHTTP_HEADER_DATE, -1, sz_date, DATE_LEN);
	xhttp_get_request_header(xhttp, XHTTP_HEADER_XDSACTION, -1, sz_action, PATH_LEN);

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
		+ ucs_to_utf8(sz_action, -1, NULL, MAX_LONG) + 1\
		+ ucs_to_utf8(sz_object, -1, NULL, MAX_LONG);
#else
	utf8_len = mbs_to_utf8(sz_verb, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(sz_type, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(sz_date, -1, NULL, MAX_LONG) + 1 \
		+ mbs_to_utf8(sz_action, -1, NULL, MAX_LONG) + 1 \
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
	pos += ucs_to_utf8(sz_action, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(sz_action, -1, utf8_buf + pos, utf8_len - pos);
#endif
	utf8_buf[pos++] = '\n';

#ifdef _UNICODE
	pos += ucs_to_utf8(sz_object, -1, utf8_buf + pos, utf8_len - pos);
#else
	pos += mbs_to_utf8(sz_object, -1, utf8_buf + pos, utf8_len - pos);
#endif

	xsha1_hmac_bytes((unsigned char*)acc_buf, acc_len, (unsigned char*)utf8_buf, pos, dig);

#ifdef _UNICODE
	bas_len = w_base64_encode(dig, 20, sz_signature, RES_LEN);
#else
	bas_len = a_base64_encode(dig, 20, sz_signature, RES_LEN);
#endif

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

/*****************************************************************************************************/
#ifdef XDL_SUPPOTR_HTTP_SRV

void xhttp_accept(xhand_t socket, PF_XHTTP_ACCEPT pf_accept, void* pf_param)
{
	xhttp_t* phttp = NULL;

	phttp = (xhttp_t*)xmem_alloc(sizeof(xhttp_t));
	phttp->head.tag = _HANDLE_HTTP;
	phttp->head.cbs = sizeof(xhttp_t);
	phttp->type = _XHTTP_TYPE_SRV;

	phttp->socket = socket;
	phttp->st_request = create_string_table();
	phttp->st_response = create_string_table();
	xsocket_peer(socket, phttp->addr);
	xscpy(phttp->poto, _T("HTTP"));
	xscpy(phttp->version, _T("1.1"));

	if (pf_accept)
	{
		(*pf_accept)((xhand_t)phttp, pf_param);
	}

	destroy_string_table(phttp->st_request);

	destroy_string_table(phttp->st_response);

	xmem_free(phttp);
}

int socket_accept(xhand_t socket, void* param)
{
	xhttp_listen_t* plis = (xhttp_listen_t*)param;

	XDL_ASSERT(plis != NULL);
	
	xhttp_accept(socket, plis->pf_accept, plis->pf_param);

	return 0;
}

xhand_t xhttp_start_thread(bool_t secu, short port, PF_XHTTP_ACCEPT pf_accept, void* pf_param)
{
	xhttp_listen_t* plis;

	plis = (xhttp_listen_t*)xmem_alloc(sizeof(xhttp_listen_t));
	plis->head.tag = _HANDLE_HTTP;
	plis->head.cbs = sizeof(xhttp_listen_t);

	plis->type = _XHTTP_TYPE_LIS;

	plis->pf_accept = pf_accept;
	plis->pf_param = (void*)pf_param;

	plis->socket = xsocket_thread_srv(secu, port, socket_accept, (void*)plis);
	
	if (!plis->socket)
	{
		xmem_free(plis);
		return 0;
	}

	return (xhand_t)plis;
}

xhand_t xhttp_start_process(bool_t secu, short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	xhttp_listen_t* plis;

	plis = (xhttp_listen_t*)xmem_alloc(sizeof(xhttp_listen_t));
	plis->head.tag = _HANDLE_HTTP;
	plis->head.cbs = sizeof(xhttp_listen_t);

	plis->type = _XHTTP_TYPE_LIS;

	plis->socket = xsocket_process_srv(secu, port, sz_module, sz_cmdline);

	if (!plis->socket)
	{
		xmem_free(plis);
		return 0;
	}

	return (xhand_t)plis;
}

void xhttp_stop(xhand_t xhttp)
{
	xhttp_listen_t* plis = (xhttp_listen_t*)xhttp;

	XDL_ASSERT(xhttp && xhttp->tag == _HANDLE_HTTP);
	XDL_ASSERT(plis->type == _XHTTP_TYPE_LIS);

	if (plis->socket)
		xsocket_stop(plis->socket);

	xmem_free(plis);
}

#endif //XDL_SUPPOTR_HTTP_SRV

#endif //XDL_SUPPOTR_HTTP
