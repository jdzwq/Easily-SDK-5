/*************************************************************
	EasySoft xds v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc xds.rtf: slots

	@module	xmlhttp.h | xml http interface file

	@devnote 张文权 2003.01 - 2008.12 v3.0
	@devnote 张文权 2009.01 - 2009.12 v4.0
*************************************************************/

#ifndef XDLINET_H
#define XDLINET_H

#include "xdldef.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API handle_t xhttp_open(const wchar_t* app,const wchar_t* method,const wchar_t* url);

XDL_API void xhttp_close(handle_t xhttp);

XDL_API bool_t xhttp_send(handle_t xhttp,void* data,int len);

XDL_API int xhttp_recv_size(handle_t xhttp);

XDL_API bool_t xhttp_recv(handle_t xhttp,void* buf,int max);

#ifdef __cplusplus
}
#endif

#endif /*XDLINET_H*/