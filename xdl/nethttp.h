/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl http document

	@module	nethttp.h | interface file

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

#define DEF_HTTP_PORT		80
#define DEF_HTTPS_PORT		443

#define XHTTP_HEADER_SIZE	1024
#define XHTTP_HEADER_MAX	(64 * 1024)
#define XHTTP_ZIPED_SIZE	(4 * 1024 * 1024)

#if defined(XDK_SUPPORT_SOCK)

typedef enum{
	_XHTTP_TYPE_LIS = 0,
	_XHTTP_TYPE_CLI = 1,
	_XHTTP_TYPE_SRV = 2
}XHTTP_TYPE;

#define IS_XHTTP_SUCCEED(code)		((compare_text(code,1,_T("2"),1,0) == 0 || compare_text(code,1,_T("3"),1,0) == 0)? 1 : 0)
#define IS_XHTTP_CONTINUE(code)		((compare_text(code,-1,_T("100"),-1,0) == 0)? 1 : 0)
#define IS_XHTTP_REDIRECT(code)		((compare_text(code,-1,_T("302"),-1,0) == 0)? 1 : 0)

#define CONTENTTYPE_IS_JSON(token)	((compare_text(token, xslen(HTTP_HEADER_CONTENTTYPE_APPJSON), HTTP_HEADER_CONTENTTYPE_APPJSON, -1, 1) == 0)? 1 : 0)
#define CONTENTTYPE_IS_XML(token)	((compare_text(token, xslen(HTTP_HEADER_CONTENTTYPE_APPXML), HTTP_HEADER_CONTENTTYPE_APPXML, -1, 1) == 0)? 1 : 0)
#define CONTENTTYPE_IS_SVG(token)	((compare_text(token, xslen(HTTP_HEADER_CONTENTTYPE_IMAGESVG), HTTP_HEADER_CONTENTTYPE_IMAGESVG, -1, 1) == 0)? 1 : 0)
#define CONTENTTYPE_IS_TEXT(token)	((compare_text(token, 5, _T("text/"), 5 ,1) == 0)? 1 : 0)
#define CONTENTTYPE_IS_ANY(token)	((is_null(token) || compare_text(token, 3, _T("*/*"), -1, 1) == 0)? 1 : 0)
#define CHARSET_NEED_UTFBOM(token)	((compare_text(charset,6,_T("utf-16"),6,1) == 0)? 1 : 0)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xhttp_client: create a http client.
@INPUT const tchar_t* method: the http method, it can be GET, PUT, POST, HEAD, DELETE.
@INPUT const tchar_t* url: the service url token.
@RETURN xhand_t: if succeeds return http client handle, fails return NULL.
*/
EXP_API xhand_t		xhttp_client(const tchar_t* method,const tchar_t* url);

/*
@FUNCTION xhttp_server: create a http server.
@INPUT xhand_t bio: the background io handle, it can be tcp, ssl handle.
@RETURN xhand_t: if succeeds return http server handle, fails return NULL.
*/
EXP_API xhand_t		xhttp_server(xhand_t bio);

/*
@FUNCTION xhttp_close: close a http client or server handle.
@INPUT xhand_t xhttp: the http handle.
@RETURN void: none.
*/
EXP_API void		xhttp_close(xhand_t xhttp);

/*
@FUNCTION xhttp_get_send_stream: get a http stream object for writing.
@INPUT xhand_t xhttp: the http handle.
@RETURN stream_t: return the stream object.
*/
EXP_API stream_t	xhttp_get_send_stream(xhand_t xhttp);

/*
@FUNCTION xhttp_get_recv_stream: get a http stream object for reading.
@INPUT xhand_t xhttp: the http handle.
@RETURN stream_t: return the stream object.
*/
EXP_API stream_t	xhttp_get_recv_stream(xhand_t xhttp);

/*
@FUNCTION xhttp_type: get the http type, it can be _XHTTP_TYPE_CLI, _XHTTP_TYPE_SRV.
@INPUT xhand_t xhttp: the http handle.
@RETURN int: return the http type.
*/
EXP_API int			xhttp_type(xhand_t xhttp);

/*
@FUNCTION xhttp_secu: get the http security type, it can be _SECU_SSL, _SECU_XSL or zero.
@INPUT xhand_t xhttp: the http handle.
@RETURN int: return the http security type, zero indicate no security.
*/
EXP_API int			xhttp_secu(xhand_t xhttp);

/*
@FUNCTION xhttp_bio: get the http background io handle, it can be tcp, ssl handle.
@INPUT xhand_t xhttp: the http handle.
@RETURN xhand_t: return background io handle.
*/
EXP_API xhand_t		xhttp_bio(xhand_t xhttp);

/*
@FUNCTION xhttp_format_request: format http request to bytes buffer.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return the bytes formated.
*/
EXP_API dword_t	xhttp_format_request(xhand_t xhttp, byte_t* buf, dword_t max);

EXP_API int xhttp_format_content_type(const tchar_t* filter, tchar_t* buf, int max);

/*
@FUNCTION xhttp_split_object: split and copy http query url site and file token,
if the url like "http://127.0.0.1:8080/mysite/mydir/myfile?mykey=myval", then the site is "/mysite", the file is "/mydir/myfile".
@INPUT const tchar_t* sz_url: the url token.
@OUTPUT tchar_t* sz_site: the string buffer for returning site token.
@OUTPUT tchar_t* sz_file: the string buffer for returning file token.
@RETURN void: none.
*/
EXP_API void		xhttp_split_object(const tchar_t* sz_url, tchar_t* sz_site, tchar_t* sz_file);

/*
@FUNCTION xhttp_url_encoding: encoding the url query token.
@INPUT const tchar_t* url: the url token.
@INPUT int len: the url token length in characters.
@INPUT byte_t* buf: the buffer for holding query token.
@INPUT int max: the buffer max size.
@RETURN int: return bytes encoded.
*/
EXP_API dword_t		xhttp_url_encoding(const tchar_t* url, int len, byte_t* buf, dword_t max);

/*
@FUNCTION xhttp_url_decoding: decoding the url query token.
@INPUT const byte_t* url: the url encoded token.
@INPUT dword_t len: the url encoded token size in bytes.
@INPUT tchar_t* buf: the buffer for holding query token.
@INPUT int max: the buffer max size.
@RETURN int: return characters decoded.
*/
EXP_API int			xhttp_url_decoding(const byte_t* url, dword_t len, tchar_t* buf, int max);

/*
@FUNCTION xhttp_get_encoded_query: get http query token.
@INPUT xhand_t xhttp: the http handle.
@INPUT byte_t* buf: the buffer for holding query token.
@INPUT int max: the buffer max size.
@RETURN int: return bytes copyed.
*/
EXP_API dword_t		xhttp_get_encoded_query(xhand_t xhttp, byte_t* buf, dword_t max);

/*
@FUNCTION xhttp_set_encoded_query: set http encoded query.
@INPUT xhand_t xhttp: the http handle.
@INPUT const byte_t* query: the query encoded token.
@INPUT dword_t len: the encoded query token length in bytes.
@RETURN void: none.
*/
EXP_API void		xhttp_set_encoded_query(xhand_t xhttp, const byte_t* query, dword_t len);

/*
@FUNCTION xhttp_get_url_method: get http method.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* buf: the string buffer for returning value.
@INPUT int max: the string buffer size in characters, not include terminate character.
@RETURN int: return method token length in characters.
*/
EXP_API int			xhttp_get_url_method(xhand_t xhttp, tchar_t* buf, int max);

/*
@FUNCTION xhttp_get_url_host: get http url host.
if the url like "http://127.0.0.1:8080/mysite/mydir/myfile?mykey=myval", then the host is "127.0.0.1".
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* buf: the string buffer for returning value.
@INPUT int max: the string buffer size in characters, not include terminate character.
@RETURN int: return method token length in characters.
*/
EXP_API int			xhttp_get_url_host(xhand_t xhttp, tchar_t* buf, int max);

/*
@FUNCTION xhttp_get_url_port: get http url port.
if the url like "http://127.0.0.1:8080/mysite/mydir/myfile?mykey=myval", then the port is "8080".
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* buf: the string buffer for returning value.
@INPUT int max: the string buffer size in characters, not include terminate character.
@RETURN int: return port token length in characters.
*/
EXP_API int			xhttp_get_url_port(xhand_t xhttp, tchar_t* buf, int max);

/*
@FUNCTION xhttp_get_url_object: get http url object.
if the url like "http://127.0.0.1:8080/mysite/mydir/myfile?mykey=myval", then the object is "/mysite/mydir/myfile".
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* buf: the string buffer for returning value.
@INPUT int max: the string buffer size in characters, not include terminate character.
@RETURN int: return object token length in characters.
*/
EXP_API int			xhttp_get_url_object(xhand_t xhttp, tchar_t* buf, int max);

/*
@FUNCTION xhttp_get_url_query: get http url encoded query.
if the url like "http://127.0.0.1:8080/mysite/mydir/myfile?mykey=myval", then the query is "mykey=myval".
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* buf: the string buffer for returning value.
@INPUT int max: the string buffer size in characters, not include terminate character.
@RETURN int: return object token length in characters.
*/
EXP_API int			xhttp_get_url_query(xhand_t xhttp, tchar_t* buf, int max);

/*
@FUNCTION xhttp_set_query: set http query token.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* query: the query string token.
@INPUT int len: the query token length in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_set_url_query(xhand_t xhttp, const tchar_t* query, int len);

/*
@FUNCTION xhttp_set_url_query_entity: set http query entity key and value.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* key: the query entity key token.
@INPUT int klen: the key length in characters.
@INPUT const tchar_t* val: the query entity value token.
@INPUT int vlen: the value length in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_set_url_query_entity(xhand_t xhttp, const tchar_t* key, int klen, const tchar_t* val, int vlen);

/*
@FUNCTION xhttp_get_url_query_entity: get http query entity value by key.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* key: the query entity key token.
@INPUT int klen: the key length in characters.
@OUTPUT tchar_t* val: the string buffer for returning value.
@INPUT int max: the string buffer size in characters, not include terminate character.
@RETURN int: return value token length in characters.
*/
EXP_API int			xhttp_get_url_query_entity(xhand_t xhttp, const tchar_t* key, int klen, tchar_t* buf, int max);

/*
@FUNCTION xhttp_get_request_header: get http request header value.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* hname: the header name token.
@INPUT int nlen: the header name length in characters.
@OUTPUT tchar_t* hvalue: the string buffer for returning header value.
@INPUT int vmax: the value string buffer size in characters, not include terminate character.
@RETURN int: return the header value length in characters if exists, otherwise return zero.
*/
EXP_API int			xhttp_get_request_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* hvalue, int vmax);

/*
@FUNCTION xhttp_set_request_header: set http request header value.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* hname: the header name token.
@INPUT int nlen: the header name length in characters.
@INPUT const tchar_t* hvalue: the header value token.
@INPUT int vlen: the header value length in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_set_request_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen);

/*
@FUNCTION xhttp_set_request_default_header: set http default request header.
@INPUT xhand_t xhttp: the http handle.
@RETURN void: none.
*/
EXP_API void		xhttp_set_request_default_header(xhand_t xhttp);

/*
@FUNCTION xhttp_format_response: format http response header to bytes buffer.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return the response formated size in bytes.
*/
EXP_API dword_t		xhttp_format_response(xhand_t xhttp, byte_t* buf, dword_t max);

/*
@FUNCTION xhttp_get_response_code: get http response code.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* token: the string buffer for returning code.
@RETURN void: none.
*/
EXP_API void		xhttp_get_response_code(xhand_t xhttp, tchar_t* token);

/*
@FUNCTION xhttp_set_response_code: set http response code.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* token: the response code string token. 
@RETURN void: none.
*/
EXP_API void		xhttp_set_response_code(xhand_t xhttp, const tchar_t* token);

/*
@FUNCTION xhttp_get_response_message: get http response message.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* token: the string buffer for returning message.
@RETURN void: none.
*/
EXP_API int			xhttp_get_response_message(xhand_t xhttp, tchar_t* buf, int max);

/*
@FUNCTION xhttp_set_response_message: set http response message.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* token: the response message string token.
@RETURN void: none.
*/
EXP_API void		xhttp_set_response_message(xhand_t xhttp, const tchar_t* buf, int len);

/*
@FUNCTION xhttp_get_response_header: get http response header value.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* hname: the header name token.
@INPUT int nlen: the header name length in characters.
@OUTPUT tchar_t* hvalue: the string buffer for returning header value.
@INPUT int vmax: the value string buffer size in characters, not include terminate character.
@RETURN int: return the header value length in characters if exists, otherwise return zero.
*/
EXP_API int			xhttp_get_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* buf, int max);

/*
@FUNCTION xhttp_set_response_header: set http response header value.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* hname: the header name token.
@INPUT int nlen: the header name length in characters.
@INPUT const tchar_t* hvalue: the header value token.
@INPUT int vlen: the header value length in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_set_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen);

/*
@FUNCTION xhttp_set_response_default_header: set http default response header.
@INPUT xhand_t xhttp: the http handle.
@RETURN void: none.
*/
EXP_API void		xhttp_set_response_default_header(xhand_t xhttp);

/*
@FUNCTION xhttp_get_response_state: get http default response state.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for succeed response, zero for failed response.
*/
EXP_API bool_t		xhttp_get_response_state(xhand_t xhttp);

/*
@FUNCTION xhttp_addr_port: get http local address and port.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* addr: the string buffer for returning address.
@RETURN unsigned short: return the local port.
*/
EXP_API unsigned short xhttp_addr_port(xhand_t xhttp, tchar_t* addr);

/*
@FUNCTION xhttp_peer_port: get http remote address and port.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* addr: the string buffer for returning address.
@RETURN unsigned short: return the remote port.
*/
EXP_API unsigned short xhttp_peer_port(xhand_t xhttp, tchar_t* addr);

/*
@FUNCTION xhttp_get_authorization: get http authorization key and sign.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* sz_auth: the string buffer for returning authorization mode, it may be "Basic", "XDS", "OSS".
@OUTPUT tchar_t* sz_sid: the string buffer for returning authorization key.
@INPUT int slen: the key string buffer size in characters.
@OUTPUT tchar_t* sz_sign: the string buffer for returning authorization sign.
@INPUT int nlen: the sign string buffer size in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_get_authorization(xhand_t xhttp, tchar_t* sz_auth, tchar_t* sz_sid, int slen, tchar_t* sz_sign, int nlen);

/*
@FUNCTION xhttp_set_authorization: set http authorization key and sign.
@INPUT xhand_t xhttp: the http handle.
@INPUT tchar_t* sz_auth: the authorization mode, it can be "Basic", "XDS", "OSS".
@INPUT const tchar_t* sz_sid: the authorization key.
@INPUT int slen: the key string token length in characters.
@INPUT const tchar_t* sz_sign: the authorization sign.
@INPUT int nlen: the sign string token length in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_set_authorization(xhand_t xhttp, const tchar_t* sz_auth, const tchar_t* sz_sid, int slen, const tchar_t* sz_sign, int nlen);

/*
@FUNCTION xhttp_format_error: format http error information into bytes buffer.
@INPUT bool_t b_json: nonzero for formating json error, zero for formating xml error.
@INPUT const tchar_t* encoding: the xml document encode, it can be "utf-8", "gb2312".
@INPUT const tchar_t* errcode: the error code.
@INPUT const tchar_t* errtext: the error text.
@INPUT int len: the error text length in characters.
@OUTPUT byte_t* buf: the bytes buffer for returning error document.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return the formated bytes.
*/
EXP_API dword_t		xhttp_format_error(bool_t b_json, const tchar_t* encoding, const tchar_t* errcode, const tchar_t* errtext, int len, byte_t* buf, dword_t max);

/*
@FUNCTION xhttp_parse_error: parse http error information from bytes buffer.
@INPUT bool_t b_json: nonzero for parsing json error, zero for parsing xml error.
@INPUT const tchar_t* encoding: the xml document encode, it can be "utf-8", "gb2312".
@INPUT const byte_t* buf: the bytes buffer.
@INPUT dword_t len: the buffer data size in bytes.
@OUTPUT tchar_t* errcode: the string buffer for returning error code.
@OUTPUT tchar_t* errtext: the string buffer for returning error text.
@INPUT int max: the error string buffer size in characters.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_parse_error(bool_t b_json, const tchar_t* encoding, const byte_t* buf, dword_t len, tchar_t* errcode, tchar_t* errtext, int max);

/*
@FUNCTION xhttp_request_signature: calc http request header signature.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* auth: the authorization mode token, it can be "Basic", "XDS", "OSS".
@INPUT const tchar_t* skey: the signature key token.
@OUTPUT tchar_t* buf: the string buffer for returning signature.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the signature length in characters.
*/
EXP_API int			xhttp_request_signature(xhand_t xhttp, const tchar_t* auth, const tchar_t* skey, tchar_t* buf, int max);

/*
@FUNCTION xhttp_is_requested: test http request header has been sended.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for been sended, otherwise return zero.
*/
EXP_API bool_t		xhttp_is_requested(xhand_t xhttp);

/*
@FUNCTION xhttp_is_responsed: test http response header has been sended.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for been sended, otherwise return zero.
*/
EXP_API bool_t		xhttp_is_responsed(xhand_t xhttp);

/*
@FUNCTION xhttp_reset_request: clear http request header send status, then the http request header can send again.
@INPUT xhand_t xhttp: the http handle.
@RETURN void: none.
*/
EXP_API void		xhttp_reset_request(xhand_t xhttp);

/*
@FUNCTION xhttp_reset_response: clear http response header send status, then the http response header can send again.
@INPUT xhand_t xhttp: the http handle.
@RETURN void: none.
*/
EXP_API void		xhttp_reset_response(xhand_t xhttp);

/*
@FUNCTION xhttp_send_request: send the http request header.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_send_request(xhand_t xhttp);

/*
@FUNCTION xhttp_send_response: send the http response header.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_send_response(xhand_t xhttp);

/*
@FUNCTION xhttp_recv_request: recv the http request header.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_recv_request(xhand_t xhttp);

/*
@FUNCTION xhttp_recv_response: recv the http response header.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_recv_response(xhand_t xhttp);

/*
@FUNCTION xhttp_send_continue: send a http continue header.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API void		xhttp_send_continue(xhand_t xhttp);

/*
@FUNCTION xhttp_send: send http data.
@INPUT xhand_t xhttp: the http handle.
@INPUT const byte_t* data: the data bytes buffer.
@INPUT dword_t len: the data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_send(xhand_t xhttp, const byte_t* data, dword_t len);

/*
@FUNCTION xhttp_fush: ensure http data sending compeleted.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_fush(xhand_t xhttp);

/*
@FUNCTION xhttp_recv: recv http data.
@INPUT xhand_t xhttp: the http handle.
@INPUT byte_t* data: the bytes buffer for reading data.
@INOUTPUT dword_t* pb: indicate the bytes wanted for reading and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_recv(xhand_t xhttp, byte_t* buf, dword_t *pb);

/*
@FUNCTION xhttp_send_full: send http full data.
@INPUT xhand_t xhttp: the http handle.
@INPUT const byte_t* buf: the bytes buffer for sending.
@INPUT dword_t len: the data size in bytes
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_send_full(xhand_t xhttp, const byte_t* buf, dword_t len);

/*
@FUNCTION xhttp_recv_full: recv http full data.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT byte_t** pbuf: the bytes buffer, it must be alloced by bytes_alloc.
@OUTPUT dword_t* plen: for return bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_recv_full(xhand_t xhttp, byte_t** pbuf, dword_t* plen);

/*
@FUNCTION xhttp_send_error: send a http error.
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* http_code: the http code token.
@INPUT const tchar_t* http_info: the http code information.
@INPUT const tchar_t* errcode: the user error code.
@INPUT const tchar_t* errtext: the user error text.
@INPUT int len: the user error text length in characters.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_send_error(xhand_t xhttp, const tchar_t* http_code, const tchar_t* http_info, const tchar_t* errcode, const tchar_t* errtext, int len);

/*
@FUNCTION xhttp_recv_error: recv a http error.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* http_code: the string buffer for returning http code.
@OUTPUT tchar_t* http_info: the string buffer for returning http code information.
@OUTPUT tchar_t* errcode: the string buffer for returning user error code.
@OUTPUT tchar_t* errtext: the string buffer for returning user error text.
@INPUT int max: the user error text string buffer size in characters.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_recv_error(xhand_t xhttp, tchar_t* http_code, tchar_t* http_info, tchar_t* errcode, tchar_t* errtext, int max);

#if defined(XDL_SUPPORT_DOC)
/*
@FUNCTION xhttp_send_xml: send a http xml document.
@INPUT xhand_t xhttp: the http handle.
@INPUT link_t_ptr xml: the xml document.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_send_xml(xhand_t xhttp, link_t_ptr xml);

/*
@FUNCTION xhttp_recv_xml: recv a http xml document.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT link_t_ptr xml: the xml document for receiving.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_recv_xml(xhand_t xhttp, link_t_ptr xml);

/*
@FUNCTION xhttp_send_json: send a http json document.
@INPUT xhand_t xhttp: the http handle.
@INPUT link_t_ptr json: the json document.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_send_json(xhand_t xhttp, link_t_ptr json);

/*
@FUNCTION xhttp_recv_json: recv a http json document.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT link_t_ptr json: the json document for receiving.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_recv_json(xhand_t xhttp, link_t_ptr json);

#endif

/*
@FUNCTION xhttp_send_string: send a http string object.
@INPUT xhand_t xhttp: the http handle.
@INPUT string_t var: the string object.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_send_string(xhand_t xhttp, string_t var);

/*
@FUNCTION xhttp_recv_string: recv a http string object.
@INPUT xhand_t xhttp: the http handle.
@OUTPUT string_t var: the string object for receiving.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xhttp_recv_string(xhand_t xhttp, string_t var);

/*
@FUNCTION xhttp_get_response_content_length: get http response content length in bytes, the header token is "Content-Length".
@INPUT xhand_t xhttp: the http handle.
@RETURN dword_t: return the content length in bytes if header exists, otherwise return zero.
*/
EXP_API dword_t		xhttp_get_response_content_length(xhand_t xhttp);

/*
@FUNCTION xhttp_set_response_content_length: set http response content length in bytes, the header token is "Content-Length".
@INPUT xhand_t xhttp: the http handle.
@INPUT dword_t len: the content size in bytes.
@RETURN void: none.
*/
EXP_API void		xhttp_set_response_content_length(xhand_t xhttp, dword_t len);

/*
@FUNCTION xhttp_get_request_content_length: get http request content length in bytes, the header token is "Content-Length".
@INPUT xhand_t xhttp: the http handle.
@RETURN dword_t: return the content length in bytes if header exists, otherwise return zero.
*/
EXP_API dword_t		xhttp_get_request_content_length(xhand_t xhttp);

/*
@FUNCTION xhttp_set_request_content_length: set http request content length in bytes, the header token is "Content-Length".
@INPUT xhand_t xhttp: the http handle.
@INPUT dword_t len: the content size in bytes.
@RETURN void: none.
*/
EXP_API void		xhttp_set_request_content_length(xhand_t xhttp, dword_t len);

/*
@FUNCTION xhttp_get_response_content_type_charset: get http response content type charset, the header token is "charset".
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* buf: the string buffer for return charset, the output can be "utf-8", "gb2312".
@INPUT int max: the string buffer size in characters.
@RETURN int: return the charset length in characters if header exists, otherwise return zero.
*/
EXP_API int			xhttp_get_response_content_type_charset(xhand_t xhttp, tchar_t* buf, int max);

/*
@FUNCTION xhttp_set_response_content_type_charset: set http response content type charset, the header token is "charset".
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* token: the charset string token, it can be "utf-8", "gb2312".
@INPUT int len: the charset token length in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_set_response_content_type_charset(xhand_t xhttp, const tchar_t* token, int len);

/*
@FUNCTION xhttp_get_request_content_type_charset: get http request content type charset, the header token is "charset".
@INPUT xhand_t xhttp: the http handle.
@OUTPUT tchar_t* buf: the string buffer for return charset, the output can be "utf-8", "gb2312".
@INPUT int max: the string buffer size in characters.
@RETURN int: return the charset length in characters if header exists, otherwise return zero.
*/
EXP_API int			xhttp_get_request_content_type_charset(xhand_t xhttp, tchar_t* buf, int max);

/*
@FUNCTION xhttp_set_request_content_type_charset: set http request content type charset, the header token is "charset".
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* token: the charset string token, it can be "utf-8", "gb2312".
@INPUT int len: the charset token length in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_set_request_content_type_charset(xhand_t xhttp, const tchar_t* token, int len);

/*
@FUNCTION xhttp_set_request_cookie: set http request cookies, the header token is "Cookie".
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* key: the cookie key token.
@INPUT const tchar_t* val: the cookie value token.
@INPUT int len: the cookie value token length in characters.
@RETURN void: none.
*/
EXP_API void		xhttp_set_request_cookie(xhand_t xhttp, const tchar_t* key, const tchar_t* val, int len);

/*
@FUNCTION xhttp_get_request_cookie: get http request cookie, the header token is "Cookie".
@INPUT xhand_t xhttp: the http handle.
@INPUT const tchar_t* key: the cookie key token.
@OUTPUT tchar_t* val: the string buffer for returning cookie.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the cookie length in characters if header exists, otherwise return zero.
*/
EXP_API int			xhttp_get_request_cookie(xhand_t xhttp, const tchar_t* key, tchar_t* val, int max);

/*
@FUNCTION xhttp_is_chunked_send: test http is chunked sending, the header token is "Transfer-Encoding".
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for chunked sending, otherwise return zero.
*/
EXP_API bool_t		xhttp_is_chunked_send(xhand_t xhttp);

/*
@FUNCTION xhttp_is_chunked_recv: test http is chunked receiving, the header token is "Transfer-Encoding".
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for chunked receiving, otherwise return zero.
*/
EXP_API bool_t		xhttp_is_chunked_recv(xhand_t xhttp);

/*
@FUNCTION xhttp_is_lined_send: test http is lined sending, the content type must b "text***" and content length must bu zero.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for lined sending, otherwise return zero.
*/
EXP_API bool_t		xhttp_is_lined_send(xhand_t xhttp);

/*
@FUNCTION xhttp_is_lined_recv: test http is lined receiving, the content type must b "text***" and content length must bu zero.
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for lined receiving, otherwise return zero.
*/
EXP_API bool_t		xhttp_is_lined_recv(xhand_t xhttp);

/*
@FUNCTION xhttp_is_zipped_send: test http is zipped sending, the header token is "Content-Encoding".
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for zipped sending, otherwise return zero.
*/
EXP_API bool_t		xhttp_is_zipped_send(xhand_t xhttp);

/*
@FUNCTION xhttp_is_zipped_send: test http is zipped receiving, the header token is "Content-Encoding".
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for zipped receiving, otherwise return zero.
*/
EXP_API bool_t		xhttp_is_zipped_recv(xhand_t xhttp);

/*
@FUNCTION xhttp_need_expect: test http is need expect, the header token is "Expect".
@INPUT xhand_t xhttp: the http handle.
@RETURN bool_t: return nonzero for need expect, otherwise return zero.
*/
EXP_API bool_t		xhttp_need_expect(xhand_t xhttp);


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