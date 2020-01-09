/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl shttp document

	@module	xdlsnet.h | xdl http interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#ifndef _XDLSNET_H
#define _XDLSNET_H

#include "xdldef.h"
#include "httpattr.h"

#ifdef XDL_SUPPORT_SOCK

#define TLS_CONTROL_SIZE		8
#define TLS_SESSION_SIZE		32

typedef enum{
	_SHTTP_TYPE_LIS = 0,
	_SHTTP_TYPE_CLI = 1,
	_SHTTP_TYPE_SRV = 2
}SHTTP_TYPE;

#define SHTTP_ZIPED_SIZE	(4 * 1024 * 1024)

#define IS_SHTTP_METHOD(token)		(compare_text(token,-1,SHTTP_METHOD_GET,-1,1) == 0 ||  compare_text(token,-1,SHTTP_METHOD_PUT,-1,1) == 0 || compare_text(token,-1,SHTTP_METHOD_POST,-1,1) == 0 || compare_text(token,-1,SHTTP_METHOD_DELETE,-1,1) == 0 || compare_text(token,-1,SHTTP_METHOD_HEAD,-1,1) == 0 || compare_text(token,-1,SHTTP_METHOD_LIST,-1,1) == 0 || compare_text(token,-1,SHTTP_METHOD_GRANT,-1,1) == 0) 
#define IS_SHTTP_SUCCEED(code)		((compare_text(code,1,_T("2"),1,0) == 0)? 1 : 0)
#define IS_SHTTP_CONTINUE(code)		((compare_text(code,-1,_T("100"),-1,0) == 0)? 1 : 0)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t		shttp_open(const tchar_t* app,const tchar_t* method,const tchar_t* url);

XDL_API void		shttp_close(xhand_t shttp);

XDL_API object_t	shttp_get_send_stream(xhand_t shttp);

XDL_API object_t	shttp_get_recv_stream(xhand_t shttp);

XDL_API int			shttp_type(xhand_t shttp);

XDL_API void		shttp_set_query_entity(xhand_t shttp, const tchar_t* key, int klen, const tchar_t* val, int vlen);

XDL_API int			shttp_get_query_entity(xhand_t shttp, const tchar_t* key, int klen, tchar_t* buf, int max);

XDL_API void		shttp_split_object(const tchar_t* sz_url, tchar_t* sz_site, tchar_t* sz_file);

XDL_API int			shttp_get_url_method(xhand_t shttp, tchar_t* buf, int max);

XDL_API int			shttp_get_url_host(xhand_t shttp,tchar_t* buf,int max);

XDL_API int			shttp_get_url_port(xhand_t shttp,tchar_t* buf,int max);

XDL_API int			shttp_get_url_object(xhand_t shttp,tchar_t* buf,int max);

XDL_API int			shttp_get_url_query(xhand_t shttp,tchar_t* buf,int max);

XDL_API const tchar_t* shttp_get_url_query_ptr(xhand_t http);

XDL_API int			shttp_get_request_header(xhand_t shttp, const tchar_t* hname, int nlen, tchar_t* hvalue, int vmax);

XDL_API void		shttp_set_request_header(xhand_t shttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen);

XDL_API void		shttp_set_request_default_header(xhand_t shttp);

XDL_API void		shttp_get_response_code(xhand_t shttp, tchar_t* token);

XDL_API void		shttp_set_response_code(xhand_t shttp, const tchar_t* token);

XDL_API int			shttp_get_response_message(xhand_t shttp, tchar_t* buf, int max);

XDL_API void		shttp_set_response_message(xhand_t shttp, const tchar_t* buf, int len);

XDL_API int			shttp_get_response_header(xhand_t shttp, const tchar_t* hname, int nlen, tchar_t* buf, int max);

XDL_API void		shttp_set_response_header(xhand_t shttp, const tchar_t* hname, int nlen, const tchar_t* hvalue, int vlen);

XDL_API void		shttp_set_response_default_header(xhand_t shttp);

XDL_API bool_t		shttp_get_response_state(xhand_t shttp);

XDL_API int			shttp_get_addr(xhand_t shttp, tchar_t* buf, int max);

XDL_API int			shttp_format_request(xhand_t shttp, byte_t* buf, dword_t max);

XDL_API int			shttp_format_response(xhand_t shttp, byte_t* buf, dword_t max);

XDL_API int			shttp_format_continue(xhand_t shttp, byte_t* buf, dword_t max);

XDL_API int			shttp_format_error(const tchar_t* encoding, const tchar_t* errcode, const tchar_t* errtext, int len, byte_t* buf, dword_t max);

XDL_API bool_t		shttp_parse_error(const byte_t* buf, dword_t len, tchar_t* errcode, tchar_t* errtext, int max);

XDL_API bool_t		shttp_is_requested(xhand_t shttp);

XDL_API bool_t		shttp_is_responsed(xhand_t shttp);

XDL_API void		shttp_reset_request(xhand_t shttp);

XDL_API void		shttp_reset_response(xhand_t shttp);

XDL_API bool_t		shttp_send_request(xhand_t shttp);

XDL_API bool_t		shttp_send_response(xhand_t shttp);

XDL_API bool_t		shttp_recv_request(xhand_t shttp);

XDL_API bool_t		shttp_recv_response(xhand_t shttp);

XDL_API void		shttp_send_continue(xhand_t shttp);

XDL_API bool_t		shttp_send(xhand_t shttp, const byte_t* data, dword_t len);

XDL_API bool_t		shttp_recv(xhand_t shttp, byte_t* buf, dword_t max);

XDL_API bool_t		shttp_send_full(xhand_t shttp, const byte_t* buf, dword_t len);

XDL_API bool_t		shttp_recv_full(xhand_t shttp, byte_t** pbuf, dword_t* plen);

XDL_API bool_t		shttp_send_error(xhand_t shttp, const tchar_t* http_code, const tchar_t* http_info, const tchar_t* errcode, const tchar_t* errtext, int len);

XDL_API bool_t		shttp_recv_error(xhand_t shttp, tchar_t* http_code, tchar_t* http_info, tchar_t* errcode, tchar_t* errtext, int max);

XDL_API bool_t		shttp_send_xml(xhand_t shttp,link_t_ptr xml);

XDL_API bool_t		shttp_recv_xml(xhand_t shttp,link_t_ptr xml);

XDL_API bool_t		shttp_send_var(xhand_t shttp, object_t var);

XDL_API bool_t		shttp_recv_var(xhand_t shttp, object_t var);

XDL_API dword_t		shttp_get_response_content_length(xhand_t shttp);

XDL_API void		shttp_set_response_content_length(xhand_t shttp, dword_t len);

XDL_API dword_t		shttp_get_request_content_length(xhand_t shttp);

XDL_API void		shttp_set_request_content_length(xhand_t shttp, dword_t len);

XDL_API int			shttp_get_response_content_type_charset(xhand_t shttp, tchar_t* buf, int max);

XDL_API void		shttp_set_response_content_type_charset(xhand_t shttp, const tchar_t* token, int len);

XDL_API int			shttp_get_request_content_type_charset(xhand_t shttp, tchar_t* buf, int max);

XDL_API void		shttp_set_request_content_type_charset(xhand_t shttp, const tchar_t* token, int len);

XDL_API bool_t		shttp_is_chunked_send(xhand_t shttp);

XDL_API bool_t		shttp_is_chunked_recv(xhand_t shttp);

XDL_API bool_t		shttp_is_zipped_send(xhand_t shttp);

XDL_API bool_t		shttp_is_zipped_recv(xhand_t shttp);

XDL_API bool_t		shttp_need_expect(xhand_t shttp);

#define shttp_get_request_content_type(shttp, buf, max) shttp_get_request_header(shttp, HTTP_HEADER_CONTENTTYPE, -1, buf, max)

#define shttp_set_request_content_type(shttp, token, len) shttp_set_request_header(shttp, HTTP_HEADER_CONTENTTYPE, -1, token, len)

#define shttp_get_request_accept_type(shttp, buf, max) shttp_get_request_header(shttp, HTTP_HEADER_ACCEPT, -1, buf, max)

#define shttp_set_request_accept_type(shttp, token, len) shttp_set_request_header(shttp, HTTP_HEADER_ACCEPT, -1, token, len)

#define shttp_get_request_accept_encoding(shttp, buf, max) shttp_get_request_header(shttp, HTTP_HEADER_ACCEPTENCODING, -1, buf, max)

#define shttp_set_request_accept_encoding(shttp, token, len) shttp_set_request_header(shttp, HTTP_HEADER_ACCEPTENCODING, -1, token, len)

#define shttp_get_request_accept_charset(shttp, buf, max) shttp_get_request_header(shttp, HTTP_HEADER_ACCEPTCHARSET, -1, buf, max)

#define shttp_set_request_accept_charset(shttp, token, len) shttp_set_request_header(shttp, HTTP_HEADER_ACCEPTCHARSET, -1, token, len)

#define shttp_get_response_content_type(shttp, buf, max) shttp_get_response_header(shttp, HTTP_HEADER_CONTENTTYPE, -1, buf, max)

#define shttp_set_response_content_type(shttp, token, len) shttp_set_response_header(shttp, HTTP_HEADER_CONTENTTYPE, -1, token, len)

#define shttp_get_response_content_encoding(shttp, buf, max) shttp_get_response_header(shttp, HTTP_HEADER_CONTENTENCODING, -1, buf, max)

#define shttp_set_response_content_encoding(shttp, token, len) shttp_set_response_header(shttp, HTTP_HEADER_CONTENTENCODING, -1, token, len);


/*********************************************************************************************/
#ifdef XDL_SUPPORT_SOCK_SRV

typedef struct _shttp_block_t{
	int cbs;

	bool_t is_thread;
	xhand_t xh;

	object_t log;
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

}shttp_block_t;

typedef enum{
	SHTTP_INVOKE_SUCCEED = 0,
	SHTTP_INVOKE_WITHINFO = 1,
	SHTTP_INVOKE_WITHERROR = 2,
	SHTTP_INVOKE_PENDING = 100
}SHTTP_INVOKE_STATE;

typedef int(_stdcall *PF_SHTTP_INVOKE)(const tchar_t* method, const shttp_block_t* pb);

typedef void(*PF_SHTTP_ACCEPT)(xhand_t shttp, void* param);

XDL_API xhand_t		shttp_start_thread(bool_t secu, short port, PF_SHTTP_ACCEPT pf_accept, void* pf_param);

XDL_API xhand_t		shttp_start_process(bool_t secu, short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

XDL_API void		shttp_stop(xhand_t shttp);

XDL_API void		shttp_accept(xhand_t sok, PF_SHTTP_ACCEPT pf_accept, void* pf_param);


#endif /*XDL_SUPPORT_SOCK_SRV*/

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_SOCK*/

#endif /*XDLSNET_H*/