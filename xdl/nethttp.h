/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl http document

	@module	nethttp.h | xdl http interface file

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

#ifndef _NETHTTP_H
#define _NETHTTP_H

#include "xdldef.h"
#include "httpattr.h"

#define XHTTP_HEADER_SIZE	1024
#define XHTTP_ZIPED_SIZE	(4 * 1024 * 1024)

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)

typedef enum{
	_XHTTP_TYPE_LIS = 0,
	_XHTTP_TYPE_CLI = 1,
	_XHTTP_TYPE_SRV = 2
}XHTTP_TYPE;

#define IS_XHTTP_METHOD(token)		(compare_text(token,-1,HTTP_METHOD_GET,-1,1) == 0 ||  compare_text(token,-1,HTTP_METHOD_PUT,-1,1) == 0 || compare_text(token,-1,HTTP_METHOD_POST,-1,1) == 0 || compare_text(token,-1,HTTP_METHOD_DELETE,-1,1) == 0 || compare_text(token,-1,HTTP_METHOD_HEAD,-1,1) == 0 || compare_text(token,-1,HTTP_METHOD_LIST,-1,1) == 0 || compare_text(token,-1,HTTP_METHOD_GRANT,-1,1) == 0) 
#define IS_XHTTP_SUCCEED(code)		((compare_text(code,1,_T("2"),1,0) == 0 || compare_text(code,1,_T("3"),1,0) == 0)? 1 : 0)
#define IS_XHTTP_CONTINUE(code)		((compare_text(code,-1,_T("100"),-1,0) == 0)? 1 : 0)
#define IS_XHTTP_REDIRECT(code)		((compare_text(code,-1,_T("302"),-1,0) == 0)? 1 : 0)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t		xhttp_client(const tchar_t* method,const tchar_t* url);

XDL_API xhand_t		xhttp_server(xhand_t bio);

XDL_API void		xhttp_close(xhand_t xhttp);

XDL_API stream_t	xhttp_get_send_stream(xhand_t xhttp);

XDL_API stream_t	xhttp_get_recv_stream(xhand_t xhttp);

XDL_API int			xhttp_type(xhand_t xhttp);

XDL_API int			xhttp_secu(xhand_t xhttp);

XDL_API xhand_t		xhttp_bio(xhand_t xhttp);

XDL_API dword_t		xhttp_format_request(xhand_t xhttp, byte_t* buf, dword_t max);

XDL_API void		xhttp_set_query(xhand_t xhttp, const tchar_t* query, int len);

XDL_API const tchar_t* xhttp_get_query_ptr(xhand_t xhttp);

XDL_API void		xhttp_set_query_entity(xhand_t xhttp, const tchar_t* key, int klen, const tchar_t* val, int vlen);

XDL_API int			xhttp_get_query_entity(xhand_t xhttp, const tchar_t* key, int klen, tchar_t* buf, int max);

XDL_API void		xhttp_split_object(const tchar_t* sz_url, tchar_t* sz_site, tchar_t* sz_file);

XDL_API int			xhttp_get_url_method(xhand_t xhttp, tchar_t* buf, int max);

XDL_API int			xhttp_get_url_host(xhand_t xhttp,tchar_t* buf,int max);

XDL_API int			xhttp_get_url_port(xhand_t xhttp,tchar_t* buf,int max);

XDL_API int			xhttp_get_url_object(xhand_t xhttp,tchar_t* buf,int max);

XDL_API int			xhttp_get_url_query(xhand_t xhttp,tchar_t* buf,int max);

XDL_API const tchar_t* xhttp_get_url_query_ptr(xhand_t http);

XDL_API int			xhttp_get_request_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* hvalue, int vmax);

XDL_API void		xhttp_set_request_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen);

XDL_API void		xhttp_set_request_default_header(xhand_t xhttp);

XDL_API dword_t		xhttp_format_response(xhand_t xhttp, byte_t* buf, dword_t max);

XDL_API void		xhttp_get_response_code(xhand_t xhttp, tchar_t* token);

XDL_API void		xhttp_set_response_code(xhand_t xhttp, const tchar_t* token);

XDL_API int			xhttp_get_response_message(xhand_t xhttp, tchar_t* buf, int max);

XDL_API void		xhttp_set_response_message(xhand_t xhttp, const tchar_t* buf, int len);

XDL_API int			xhttp_get_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* buf, int max);

XDL_API void		xhttp_set_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen);

XDL_API void		xhttp_set_response_default_header(xhand_t xhttp);

XDL_API bool_t		xhttp_get_response_state(xhand_t xhttp);

XDL_API unsigned short xhttp_addr_port(xhand_t xhttp, tchar_t* addr);

XDL_API unsigned short xhttp_peer_port(xhand_t xhttp, tchar_t* addr);

XDL_API void		xhttp_get_authorization(xhand_t xhttp, tchar_t* sz_sid, int slen, tchar_t* sz_sign, int max);

XDL_API void		xhttp_set_authorization(xhand_t xhttp, const tchar_t* sz_sid, int slen, const tchar_t* sz_sign,int max);

XDL_API int			xhttp_format_error(const tchar_t* encoding, const tchar_t* errcode, const tchar_t* errtext, int len, byte_t* buf, dword_t max);

XDL_API bool_t		xhttp_parse_error(const byte_t* buf, dword_t len, tchar_t* errcode, tchar_t* errtext, int max);

#ifdef XDL_SUPPORT_CRYPT
XDL_API int			xhttp_request_signature(xhand_t xhttp, const tchar_t* skey, tchar_t* buf, int max);
#endif

XDL_API bool_t		xhttp_is_requested(xhand_t xhttp);

XDL_API bool_t		xhttp_is_responsed(xhand_t xhttp);

XDL_API void		xhttp_reset_request(xhand_t xhttp);

XDL_API void		xhttp_reset_response(xhand_t xhttp);

XDL_API bool_t		xhttp_send_request(xhand_t xhttp);

XDL_API bool_t		xhttp_send_response(xhand_t xhttp);

XDL_API bool_t		xhttp_recv_request(xhand_t xhttp);

XDL_API bool_t		xhttp_recv_response(xhand_t xhttp);

XDL_API void		xhttp_send_continue(xhand_t xhttp);

XDL_API bool_t		xhttp_send(xhand_t xhttp, const byte_t* data, dword_t len);

XDL_API bool_t		xhttp_fush(xhand_t xhttp);

XDL_API bool_t		xhttp_recv(xhand_t xhttp, byte_t* buf, dword_t *pb);

XDL_API bool_t		xhttp_send_full(xhand_t xhttp, const byte_t* buf, dword_t len);

XDL_API bool_t		xhttp_recv_full(xhand_t xhttp, byte_t** pbuf, dword_t* plen);

XDL_API bool_t		xhttp_send_error(xhand_t xhttp, const tchar_t* http_code, const tchar_t* http_info, const tchar_t* errcode, const tchar_t* errtext, int len);

XDL_API bool_t		xhttp_recv_error(xhand_t xhttp, tchar_t* http_code, tchar_t* http_info, tchar_t* errcode, tchar_t* errtext, int max);

XDL_API bool_t		xhttp_send_xml(xhand_t xhttp,link_t_ptr xml);

XDL_API bool_t		xhttp_recv_xml(xhand_t xhttp,link_t_ptr xml);

XDL_API bool_t		xhttp_send_json(xhand_t xhttp, link_t_ptr json);

XDL_API bool_t		xhttp_recv_json(xhand_t xhttp, link_t_ptr json);

XDL_API bool_t		xhttp_send_var(xhand_t xhttp, string_t var);

XDL_API bool_t		xhttp_recv_var(xhand_t xhttp, string_t var);

XDL_API dword_t		xhttp_get_response_content_length(xhand_t xhttp);

XDL_API void		xhttp_set_response_content_length(xhand_t xhttp, dword_t len);

XDL_API dword_t		xhttp_get_request_content_length(xhand_t xhttp);

XDL_API void		xhttp_set_request_content_length(xhand_t xhttp, dword_t len);

XDL_API int			xhttp_get_response_content_type_charset(xhand_t xhttp, tchar_t* buf, int max);

XDL_API void		xhttp_set_response_content_type_charset(xhand_t xhttp, const tchar_t* token, int len);

XDL_API int			xhttp_get_request_content_type_charset(xhand_t xhttp, tchar_t* buf, int max);

XDL_API void		xhttp_set_request_content_type_charset(xhand_t xhttp, const tchar_t* token, int len);

XDL_API void		xhttp_set_request_cookie(xhand_t xhttp, const tchar_t* key, const tchar_t* val, int len);

XDL_API int			xhttp_get_request_cookie(xhand_t xhttp, const tchar_t* key, tchar_t* val, int max);

XDL_API bool_t		xhttp_is_chunked_send(xhand_t xhttp);

XDL_API bool_t		xhttp_is_chunked_recv(xhand_t xhttp);

XDL_API bool_t		xhttp_is_zipped_send(xhand_t xhttp);

XDL_API bool_t		xhttp_is_zipped_recv(xhand_t xhttp);

XDL_API bool_t		xhttp_need_expect(xhand_t xhttp);

#define xhttp_get_request_content_type(xhttp, buf, len) xhttp_get_request_header(xhttp, HTTP_HEADER_CONTENTTYPE, -1, buf, len)

#define xhttp_set_request_content_type(xhttp, token, len) xhttp_set_request_header(xhttp, HTTP_HEADER_CONTENTTYPE, -1, token, len)

#define xhttp_get_request_accept_type(xhttp, buf, len) xhttp_get_request_header(xhttp, HTTP_HEADER_ACCEPT, -1, buf, len)

#define xhttp_set_request_accept_type(xhttp, token, len) xhttp_set_request_header(xhttp, HTTP_HEADER_ACCEPT, -1, token, len)

#define xhttp_get_request_accept_encoding(xhttp, buf, len) xhttp_get_request_header(xhttp, HTTP_HEADER_ACCEPTENCODING, -1, buf, len)

#define xhttp_set_request_accept_encoding(xhttp, token, len) xhttp_set_request_header(xhttp, HTTP_HEADER_ACCEPTENCODING, -1, token, len)

#define xhttp_get_request_accept_charset(xhttp, buf, len) xhttp_get_request_header(xhttp, HTTP_HEADER_ACCEPTCHARSET, -1, buf, len)

#define xhttp_set_request_accept_charset(xhttp, token, len) xhttp_set_request_header(xhttp, HTTP_HEADER_ACCEPTCHARSET, -1, token, len)

#define xhttp_get_response_content_type(xhttp, buf, len) xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTTYPE, -1, buf, len)

#define xhttp_set_response_content_type(xhttp, token, len) xhttp_set_response_header(xhttp, HTTP_HEADER_CONTENTTYPE, -1, token, len)

#define xhttp_get_response_content_encoding(xhttp, buf, len) xhttp_get_response_header(xhttp, HTTP_HEADER_CONTENTENCODING, -1, buf, len)

#define xhttp_set_response_content_encoding(xhttp, token, len) xhttp_set_response_header(xhttp, HTTP_HEADER_CONTENTENCODING, -1, token, len);


#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*NETHTTP_H*/