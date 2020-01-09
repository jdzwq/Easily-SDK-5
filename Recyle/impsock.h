/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc socket document

	@module	impsock.h | socket interface file

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

#ifndef _IMPSOCK_H
#define _IMPSOCK_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_SOCK

#define SOCKET_ADDR_LEN		16
#define SOCKET_TYPE_CLIENT	0
#define SOCKET_TYPE_SERVER	1
#define SOCKET_TYPE_LISTEN	2

#define ERR_SOCKET_INIT		-1
#define ERR_SOCKET_OPEN		-2
#define ERR_SOCKET_CLOSE	-3
#define ERR_SOCKET_WRITE	-4
#define ERR_SOCKET_READ		-5

#define SOCKET_ZERO_BUFF	0
#define SOCKET_BASE_BUFF	65536
#define SOCKET_PAGE_BUFF	4096
#define SOCKET_HEAD_BUFF	1024
#define SOCKET_MIN_MULTI	1 
#define SOCKET_MAX_MULTI	8
#define SOCKET_BASE_TIMO	10

typedef int(*PF_SOCKETACCEPT)(xhand_t socket, void* param);

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API bool_t host_addr(const tchar_t* host,tchar_t* addr);

XDL_API xhand_t  xsocket_cli(bool_t sec, short port, const tchar_t* addr);

XDL_API int  xsocket_type(xhand_t so);

XDL_API void xsocket_set_send_buff(xhand_t so, dword_t dw);

XDL_API void xsocket_set_recv_buff(xhand_t so, dword_t dw);

XDL_API void xsocket_set_send_timeout(xhand_t sok, int s);

XDL_API void xsocket_set_recv_timeout(xhand_t sok, int s);

XDL_API void xsocket_set_linger(xhand_t sok, bool_t b_wait, int n_sec);

XDL_API void  xsocket_close(xhand_t so);

XDL_API bool_t  xsocket_write(xhand_t so, const byte_t* data, dword_t* pb);

XDL_API bool_t  xsocket_read(xhand_t so, byte_t* data, dword_t* pb);

XDL_API bool_t  xsocket_peek(xhand_t so, byte_t* data, dword_t size);

XDL_API void  xsocket_addr(xhand_t so, tchar_t* addr);

XDL_API void  xsocket_peer(xhand_t so, tchar_t* addr);

XDL_API dword_t xsocket_share_size(void);

XDL_API dword_t xsocket_share(xhand_t sok, dword_t procid, byte_t* buf);

XDL_API xhand_t xsocket_dupli(const byte_t* pb, dword_t dw);

XDL_API xhand_t  xsocket_thread_srv(bool_t secu, short port, PF_SOCKETACCEPT pf_accept, void* param);

XDL_API xhand_t  xsocket_process_srv(bool_t secu, short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

XDL_API void  xsocket_stop(xhand_t sok);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/