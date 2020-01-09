/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl http document

	@module	xdlinet.h | xdl http interface file

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

#ifndef _XDLINET_H
#define _XDLINET_H

#include "xdldef.h"
#include "xdlattr.h"

#ifdef XDL_SUPPOTR_HTTP

/*******************************************************************************************/
#define XHTTP_HEADER_TYPE_IMAGEJPG			_T("image/jpeg")
#define XHTTP_HEADER_TYPE_IMAGEPNG			_T("image/png")
#define XHTTP_HEADER_TYPE_IMAGEGIF			_T("image/gif")
#define XHTTP_HEADER_TYPE_IMAGEBMP			_T("image/bmp")
#define XHTTP_HEADER_TYPE_IMAGESVG			_T("image/svg+xml")
#define XHTTP_HEADER_TYPE_VIDEOMPG			_T("video/mpeg")
#define XHTTP_HEADER_TYPE_VIDEOAVI			_T("video/avi")
#define XHTTP_HEADER_TYPE_AUDIOMP3			_T("audio/mp3")
#define XHTTP_HEADER_TYPE_APPPDF			_T("application/pdf")
#define XHTTP_HEADER_TYPE_APPXML			_T("application/xml")
#define XHTTP_HEADER_TYPE_APPSOAP			_T("application/soap+xml")
#define XHTTP_HEADER_TYPE_TEXTXML			_T("text/xml")
#define XHTTP_HEADER_TYPE_TEXTXML_GB2312	_T("text/xml; charset=gb2312;")
#define XHTTP_HEADER_TYPE_TEXTXML_UTF8		_T("text/xml; charset=utf-8;")
#define XHTTP_HEADER_TYPE_TEXTHTML			_T("text/html")
#define XHTTP_HEADER_TYPE_TEXTPLAIN			_T("text/plain")
#define XHTTP_HEADER_TYPE_TEXTPLAIN_GB2312		_T("text/plain; charset=gb2312;")
#define XHTTP_HEADER_TYPE_TEXTPLAIN_UTF8		_T("text/plain; charset=utf-8;")

#define XHTTP_HEADER_TYPE_ENTITY_CHARSET	_T("charset")

#define XHTTP_HEADER_ENCODING_GZIP			_T("gzip")
#define XHTTP_HEADER_ENCODING_DEFLATE		_T("deflate")

#define XHTTP_HEADER_AUTHORIZATION			_T("Authorization")
#define XHTTP_HEADER_ACCEPT					_T("Accept")
#define XHTTP_HEADER_ACCEPTENCODING			_T("Accept-Encoding")
#define XHTTP_HEADER_ACCEPTCHARSET			_T("Accept-Charset")
#define XHTTP_HEADER_ACCEPTRANGES			_T("Accept-Ranges")
#define XHTTP_HEADER_ACCEPTRANGES_BYTES		_T("bytes")
#define XHTTP_HEADER_ACCEPTRANGES_NONE		_T("none")
#define XHTTP_HEADER_EXPECT					_T("Expect")
#define XHTTP_HEADER_EXPECT_CONTINUE		_T("100-continue")
#define XHTTP_HEADER_CONTENTENCODING		_T("Content-Encoding")
#define XHTTP_HEADER_CONTENTLENGTH			_T("Content-Length")
#define XHTTP_HEADER_CONTENTTYPE			_T("Content-Type")
#define XHTTP_HEADER_CONTENTRANGE			_T("Content-Range")
#define XHTTP_HEADER_RANGE					_T("Range")
#define XHTTP_HEADER_TRANSFERENCODING				_T("Transfer-Encoding")
#define XHTTP_HEADER_TRANSFERENCODING_CHUNKED		_T("chunked")
#define XHTTP_HEADER_CONTENTMD5				_T("Content-MD5")
#define XHTTP_HEADER_DATE					_T("Date")
#define XHTTP_HEADER_HOST					_T("Host")
#define XHTTP_HEADER_SERVER					_T("Server")
#define XHTTP_HEADER_SERVER_XSERVICE		_T("xService")
#define XHTTP_HEADER_ETAG					_T("ETag")
#define XHTTP_HEADER_LASTMODIFIED			_T("Last-Modified")
#define XHTTP_HEADER_IFMODIFIEDSINCE		_T("If-Modified-Since")
#define XHTTP_HEADER_IFMATCH				_T("If-Match")
#define XHTTP_HEADER_IFNONEMATCH			_T("If-None-Match")
#define XHTTP_HEADER_CONNECTION				_T("Connection")
#define XHTTP_HEADER_CONNECTION_CLOSE		_T("Close")
#define XHTTP_HEADER_CONNECTION_KEEPALIVE	_T("Keep-Alive")
#define XHTTP_HEADER_SOAPACTION				_T("SOAPAction")
#define XHTTP_HEADER_LOCATION				_T("Location")

#define XHTTP_HEADER_XDSACTION				_T("xds-action")
#define XHTTP_HEADER_XDSTRACE				_T("xds-trace")
#define XHTTP_HEADER_XDSCONTEXT				_T("xds-context")

#define XHTTP_VERSION			_T("1.1")

#define XHTTP_CODE_100			_T("100")
#define XHTTP_CODE_100_TEXT		_T("Continue")

#define XHTTP_CODE_101			_T("101")
#define XHTTP_CODE_101_TEXT		_T("Switching Protocols")

#define XHTTP_CODE_200			_T("200")
#define XHTTP_CODE_200_TEXT		_T("OK")

#define XHTTP_CODE_201			_T("201")
#define XHTTP_CODE_201_TEXT		_T("Created")

#define XHTTP_CODE_202			_T("202")
#define XHTTP_CODE_202_TEXT		_T("Accepted")

#define XHTTP_CODE_300			_T("300")
#define XHTTP_CODE_300_TEXT		_T("Multiple Choices")

#define XHTTP_CODE_301			_T("301")
#define XHTTP_CODE_301_TEXT		_T("Moved Permanently")

#define XHTTP_CODE_302			_T("302")
#define XHTTP_CODE_302_TEXT		_T("Moved Temporarily")

#define XHTTP_CODE_303			_T("303")
#define XHTTP_CODE_303_TEXT		_T("See Other")

#define XHTTP_CODE_304			_T("304")
#define XHTTP_CODE_304_TEXT		_T("Not Modified")

#define XHTTP_CODE_305			_T("305")
#define XHTTP_CODE_305_TEXT		_T("Use Proxy")

#define XHTTP_CODE_400			_T("400")
#define XHTTP_CODE_400_TEXT		_T("Bad Request")

#define XHTTP_CODE_401			_T("401")
#define XHTTP_CODE_401_TEXT		_T("Unauthorized")

#define XHTTP_CODE_402			_T("402")
#define XHTTP_CODE_402_TEXT		_T("Payment Required")

#define XHTTP_CODE_403			_T("403")
#define XHTTP_CODE_403_TEXT		_T("Forbidden")

#define XHTTP_CODE_404			_T("404")
#define XHTTP_CODE_404_TEXT		_T("Not Found")

#define XHTTP_CODE_405			_T("405")
#define XHTTP_CODE_405_TEXT		_T("Method Not Allowed")

#define XHTTP_CODE_406			_T("406")
#define XHTTP_CODE_406_TEXT		_T("Not Acceptable")

#define XHTTP_CODE_407			_T("407")
#define XHTTP_CODE_407_TEXT		_T("Proxy Authentication")

#define XHTTP_CODE_408			_T("408")
#define XHTTP_CODE_408_TEXT		_T("Request Timed-Out")

#define XHTTP_CODE_409			_T("409")
#define XHTTP_CODE_409_TEXT		_T("Conflict")

#define XHTTP_CODE_410			_T("410")
#define XHTTP_CODE_410_TEXT		_T("Gone")

#define XHTTP_CODE_411			_T("411")
#define XHTTP_CODE_411_TEXT		_T("Length Required")

#define XHTTP_CODE_412			_T("412")
#define XHTTP_CODE_412_TEXT		_T("Precondition Failed")

#define XHTTP_CODE_413			_T("413")
#define XHTTP_CODE_413_TEXT		_T("Request Entity Too Large")

#define XHTTP_CODE_414			_T("414")
#define XHTTP_CODE_414_TEXT		_T("Request-URI Too Large")

#define XHTTP_CODE_415			_T("415")
#define XHTTP_CODE_415_TEXT		_T("Unsupported Media Type")

#define XHTTP_CODE_417			_T("417")
#define XHTTP_CODE_417_TEXT		_T("Expectation Failed")

#define XHTTP_CODE_500			_T("500")
#define XHTTP_CODE_500_TEXT		_T("Internal Server Error")

#define XHTTP_CODE_501			_T("501")
#define XHTTP_CODE_501_TEXT		_T("Not Implemented")

#define XHTTP_CODE_502			_T("502")
#define XHTTP_CODE_502_TEXT		_T("Bad Gateway")

#define XHTTP_CODE_503			_T("503")
#define XHTTP_CODE_503_TEXT		_T("Server Unavailable")

#define XHTTP_CODE_504			_T("504")
#define XHTTP_CODE_504_TEXT		_T("Gateway Timed-Out")

#define XHTTP_CODE_505			_T("505")
#define XHTTP_CODE_505_TEXT		_T("HTTP Version not support")

#define XHTTP_FAULT				_T("fault")
#define XHTTP_FAULT_CODE		_T("fault-code")
#define XHTTP_FAULT_STRING		_T("fault-string")

#define XHTTP_METHOD_GET		_T("GET")
#define XHTTP_METHOD_PUT		_T("PUT")
#define XHTTP_METHOD_POST		_T("POST")
#define XHTTP_METHOD_HEAD		_T("HEAD")
#define XHTTP_METHOD_DELETE		_T("DELETE")
#define XHTTP_METHOD_LIST		_T("LIST")
#define XHTTP_METHOD_GRANT		_T("GRANT")


#define XHTTP_MAX_HEADER	4096
#define XHTTP_INC_HEADER	1024
#define XHTTP_MAX_BUFFER	8688
#define XHTTP_MAX_UDPDATA	65527
#define XHTTP_ZIPED_SIZE	(4 * 1024 * 1024)

#define IS_XHTTP_METHOD(token)		(compare_text(token,-1,XHTTP_METHOD_GET,-1,1) == 0 ||  compare_text(token,-1,XHTTP_METHOD_PUT,-1,1) == 0 || compare_text(token,-1,XHTTP_METHOD_POST,-1,1) == 0 || compare_text(token,-1,XHTTP_METHOD_DELETE,-1,1) == 0 || compare_text(token,-1,XHTTP_METHOD_HEAD,-1,1) == 0 || compare_text(token,-1,XHTTP_METHOD_LIST,-1,1) == 0 || compare_text(token,-1,XHTTP_METHOD_GRANT,-1,1) == 0) 
#define IS_XHTTP_SUCCEED(code)		((compare_text(code,1,_T("2"),1,0) == 0)? 1 : 0)
#define IS_XHTTP_CONTINUE(code)		((compare_text(code,-1,_T("100"),-1,0) == 0)? 1 : 0)

/**********************************************************************************************************************/
XDL_API xhand_t		xhttp_open(const tchar_t* app,const tchar_t* method,const tchar_t* url);

XDL_API void		xhttp_close(xhand_t xhttp);

XDL_API xhand_t		xhttp_get_send_stream(xhand_t xhttp);

XDL_API xhand_t		xhttp_get_recv_stream(xhand_t xhttp);

XDL_API int			xhttp_type(xhand_t xhttp);

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

XDL_API void		xhttp_get_response_code(xhand_t xhttp, tchar_t* token);

XDL_API void		xhttp_set_response_code(xhand_t xhttp, const tchar_t* token);

XDL_API int			xhttp_get_response_message(xhand_t xhttp, tchar_t* buf, int max);

XDL_API void		xhttp_set_response_message(xhand_t xhttp, const tchar_t* buf, int len);

XDL_API int			xhttp_get_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, tchar_t* buf, int max);

XDL_API void		xhttp_set_response_header(xhand_t xhttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen);

XDL_API void		xhttp_set_response_default_header(xhand_t xhttp);

XDL_API bool_t		xhttp_get_response_state(xhand_t xhttp);

XDL_API int			xhttp_get_addr(xhand_t xhttp, tchar_t* buf, int max);

XDL_API void		xhttp_get_authorization(xhand_t xhttp, tchar_t* sz_sid, int slen, tchar_t* sz_sign, int max);

XDL_API void		xhttp_set_authorization(xhand_t xhttp, const tchar_t* sz_sid, int slen, const tchar_t* sz_sign,int max);

XDL_API int			xhttp_format_request(xhand_t xhttp, byte_t* buf, dword_t max);

XDL_API int			xhttp_format_response(xhand_t xhttp, byte_t* buf, dword_t max);

XDL_API int			xhttp_format_continue(xhand_t xhttp, byte_t* buf, dword_t max);

XDL_API int			xhttp_format_error(const tchar_t* encoding, const tchar_t* errcode, const tchar_t* errtext, int len, byte_t* buf, dword_t max);

XDL_API bool_t		xhttp_parse_error(const byte_t* buf, dword_t len, tchar_t* errcode, tchar_t* errtext, int max);

#ifdef XDL_SUPPORT_SHA
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

XDL_API bool_t		xhttp_recv(xhand_t xhttp, byte_t* buf, dword_t max);

XDL_API bool_t		xhttp_send_full(xhand_t xhttp, const byte_t* buf, dword_t len);

XDL_API bool_t		xhttp_recv_full(xhand_t xhttp, byte_t** pbuf, dword_t* plen);

XDL_API bool_t		xhttp_send_error(xhand_t xhttp, const tchar_t* http_code, const tchar_t* http_info, const tchar_t* errcode, const tchar_t* errtext, int len);

XDL_API bool_t		xhttp_recv_error(xhand_t xhttp, tchar_t* http_code, tchar_t* http_info, tchar_t* errcode, tchar_t* errtext, int max);

XDL_API bool_t		xhttp_send_xml(xhand_t xhttp,link_t_ptr xml);

XDL_API bool_t		xhttp_recv_xml(xhand_t xhttp,link_t_ptr xml);

XDL_API bool_t		xhttp_send_var(xhand_t xhttp, xhand_t var);

XDL_API bool_t		xhttp_recv_var(xhand_t xhttp, xhand_t var);

XDL_API dword_t		xhttp_get_response_content_length(xhand_t xhttp);

XDL_API void		xhttp_set_response_content_length(xhand_t xhttp, dword_t len);

XDL_API dword_t		xhttp_get_request_content_length(xhand_t xhttp);

XDL_API void		xhttp_set_request_content_length(xhand_t xhttp, dword_t len);

XDL_API int			xhttp_get_response_content_type_charset(xhand_t xhttp, tchar_t* buf, int max);

XDL_API void		xhttp_set_response_content_type_charset(xhand_t xhttp, const tchar_t* token, int len);

XDL_API int			xhttp_get_request_content_type_charset(xhand_t xhttp, tchar_t* buf, int max);

XDL_API void		xhttp_set_request_content_type_charset(xhand_t xhttp, const tchar_t* token, int len);

XDL_API bool_t		xhttp_is_chunked_send(xhand_t xhttp);

XDL_API bool_t		xhttp_is_chunked_recv(xhand_t xhttp);

XDL_API bool_t		xhttp_is_zipped_send(xhand_t xhttp);

XDL_API bool_t		xhttp_is_zipped_recv(xhand_t xhttp);

XDL_API bool_t		xhttp_need_expect(xhand_t xhttp);

#define xhttp_get_request_content_type(xhttp, buf, max) xhttp_get_request_header(xhttp, XHTTP_HEADER_CONTENTTYPE, -1, buf, max)

#define xhttp_set_request_content_type(xhttp, token, len) xhttp_set_request_header(xhttp, XHTTP_HEADER_CONTENTTYPE, -1, token, len)

#define xhttp_get_request_accept_type(xhttp, buf, max) xhttp_get_request_header(xhttp, XHTTP_HEADER_ACCEPT, -1, buf, max)

#define xhttp_set_request_accept_type(xhttp, token, len) xhttp_set_request_header(xhttp, XHTTP_HEADER_ACCEPT, -1, token, len)

#define xhttp_get_request_accept_encoding(xhttp, buf, max) xhttp_get_request_header(xhttp, XHTTP_HEADER_ACCEPTENCODING, -1, buf, max)

#define xhttp_set_request_accept_encoding(xhttp, token, len) xhttp_set_request_header(xhttp, XHTTP_HEADER_ACCEPTENCODING, -1, token, len)

#define xhttp_get_request_accept_charset(xhttp, buf, max) xhttp_get_request_header(xhttp, XHTTP_HEADER_ACCEPTCHARSET, -1, buf, max)

#define xhttp_set_request_accept_charset(xhttp, token, len) xhttp_set_request_header(xhttp, XHTTP_HEADER_ACCEPTCHARSET, -1, token, len)

#define xhttp_get_response_content_type(xhttp, buf, max) xhttp_get_response_header(xhttp, XHTTP_HEADER_CONTENTTYPE, -1, buf, max)

#define xhttp_set_response_content_type(xhttp, token, len) xhttp_set_response_header(xhttp, XHTTP_HEADER_CONTENTTYPE, -1, token, len)

#define xhttp_get_response_content_encoding(xhttp, buf, max) xhttp_get_response_header(xhttp, XHTTP_HEADER_CONTENTENCODING, -1, buf, max)

#define xhttp_set_response_content_encoding(xhttp, token, len) xhttp_set_response_header(xhttp, XHTTP_HEADER_CONTENTENCODING, -1, token, len);


/*********************************************************************************************/

#ifdef XDL_SUPPOTR_HTTP_SRV

typedef byte_t** (*PF_XMEM_ALLOC_BUFFER)(void);

typedef void(*PF_XMEM_FREE_BUFFER)(byte_t**);

typedef byte_t* (*PF_XMEM_REALLOC_BUFFER)(byte_t**,dword_t);

typedef void* (*PF_XMEM_ALLOC)(dword_t);

typedef void* (*PF_XMEM_REALLOC)(void*, dword_t);

typedef void (*PF_XMEM_FREE)(void*);

typedef void(*PF_WRITE_LOG_TITLE)(xhand_t, const tchar_t*, int);

typedef void (__cdecl* PF_WRITE_LOG_ERROR)(xhand_t, const tchar_t*, const tchar_t*, int);

typedef void (__cdecl* PF_WRITE_LOG_DATA)(xhand_t, const byte_t*, dword_t);

typedef void(__cdecl* PF_WRITE_LOG_XML)(xhand_t, link_t_ptr);

typedef struct _xhttp_block_t{
	int cbs;

	bool_t is_thread;
	xhand_t xh;

	PF_XMEM_ALLOC_BUFFER			pf_xmem_alloc_buffer;
	PF_XMEM_FREE_BUFFER				pf_xmem_free_buffer;
	PF_XMEM_REALLOC_BUFFER			pf_xmem_realloc_buffer;
	PF_XMEM_ALLOC					pf_xmem_alloc;
	PF_XMEM_REALLOC					pf_xmem_realloc;
	PF_XMEM_FREE					pf_xmem_free;

	xhand_t log;
	PF_WRITE_LOG_TITLE				pf_write_log_title;
	PF_WRITE_LOG_ERROR				pf_write_log_error;
	PF_WRITE_LOG_DATA				pf_write_log_data;
	PF_WRITE_LOG_XML				pf_write_log_xml;

	tchar_t ctxid[NUM_LEN];
	tchar_t site[RES_LEN];
	tchar_t space[PATH_LEN];
	tchar_t path[PATH_LEN];
	tchar_t file[PATH_LEN];
	tchar_t param[PATH_LEN];

#ifdef _OS_64
	unsigned long long	wrecv;
#else
	unsigned long		wrecv;
#endif
	void*	precv;

}xhttp_block_t;

typedef enum{
	XHTTP_INVOKE_SUCCEED = 0,
	XHTTP_INVOKE_WITHINFO = 1,
	XHTTP_INVOKE_WITHERROR = 2,
	XHTTP_INVOKE_PENDING = 100
}XHTTP_INVOKE_STATE;

typedef int(_stdcall *PF_XHTTP_INVOKE)(const tchar_t* method, const xhttp_block_t* pb);

typedef void(*PF_XHTTP_ACCEPT)(xhand_t xhttp, void* param);

XDL_API xhand_t		xhttp_start_thread(bool_t secu, short port, PF_XHTTP_ACCEPT pf_accept, void* pf_param);

XDL_API xhand_t		xhttp_start_process(bool_t secu, short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

XDL_API void		xhttp_stop(xhand_t xhttp);

XDL_API void		xhttp_accept(xhand_t sok, PF_XHTTP_ACCEPT pf_accept, void* pf_param);

typedef struct _xsoap_block_t{
	link_t_ptr soap_request;
	link_t_ptr soap_response;

	tchar_t soap_action[PATH_LEN];
	tchar_t soap_path[PATH_LEN];
}xsoap_block_t;

typedef int(_stdcall *PF_XSOAP_INVOKE)(const xsoap_block_t* pb);

typedef struct _xwsdl_block_t{
	link_t_ptr soap_wsdl;

	tchar_t soap_path[PATH_LEN];
}xwsdl_block_t;

typedef int(_stdcall *PF_XWSDL_INVOKE)(const xwsdl_block_t* pb);

#endif //XDL_SUPPOTR_HTTP_SRV

#endif //XDL_SUPPOTR_HTTP

#endif /*XDLINET_H*/