/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tcp document

	@module	nettcp.h | tcp interface file

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

#ifndef _NETTCP_H
#define _NETTCP_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_SOCK

typedef enum{
	_XTCP_TYPE_LIS = 0,
	_XTCP_TYPE_CLI = 1,
	_XTCP_TYPE_SRV = 2
}XTCP_TYPE;

#define TCP_ZERO_BUFF	0
#define TCP_MAX_BUFF	(64 * 1024)
//#define TCP_MTU_BUFF	(1500 - 20 - 20)
#define TCP_MTU_BUFF	(576 - 20 - 20)
#if defined(DEBUG) || defined(_DEBUG)
#define TCP_BASE_TIMO	(-1)
#else
#define TCP_BASE_TIMO	(10000)
#endif

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xtcp_cli: create a TCP client.
@INPUT unsigned short port: the network port to connect.
@INPUT const tchar_t* addr: the network address to connect.
@RETURN xhand_t: if succeeds return TCP client handle, fails return NULL.
*/
XDL_API xhand_t xtcp_cli(unsigned short port, const tchar_t* addr);

/*
@FUNCTION xtcp_srv: create a TCP server.
@INPUT res_file_t so: the network io resource handle, it must be a socket resource handle.
@RETURN xhand_t: if succeeds return TCP server handle, fails return NULL.
*/
XDL_API xhand_t xtcp_srv(res_file_t so);

/*
@FUNCTION xtcp_socket: get socket resource handle.
@INPUT xhand_t tcp: the TCP handle.
@RETURN res_file_t: return the socket resource handle.
*/
XDL_API res_file_t xtcp_socket(xhand_t tcp);

/*
@FUNCTION xtcp_type: get socket type, it can be _XTCP_TYPE_CLI, _XTCP_TYPE_SRV.
@INPUT xhand_t tcp: the TCP handle.
@RETURN int: return the socket type.
*/
XDL_API int  xtcp_type(xhand_t tcp);

/*
@FUNCTION xtcp_set_send_buff: set socket send buffer size.
@INPUT xhand_t tcp: the TCP handle.
@INPUT dword_t dw: the size in bytes.
@RETURN void: none.
*/
XDL_API void xtcp_set_send_buff(xhand_t tcp, dword_t dw);

/*
@FUNCTION xtcp_set_recv_buff: set socket recv buffer size.
@INPUT xhand_t tcp: the TCP handle.
@INPUT dword_t dw: the size in bytes.
@RETURN void: none.
*/
XDL_API void xtcp_set_recv_buff(xhand_t tcp, dword_t dw);

/*
@FUNCTION xtcp_set_linger: set socket linger on close action.
@INPUT xhand_t tcp: the TCP handle.
@INPUT bool_t b_wait: nonzero for waiting unsent data to compelete, zero for immediately closing.
@INPUT int n_sec: the wait time in second.
@RETURN void: none.
*/
XDL_API void xtcp_set_linger(xhand_t tcp, bool_t b_wait, int n_sec);

/*
@FUNCTION xtcp_close: close TCP handle.
@INPUT xhand_t tcp: the TCP handle.
@RETURN void: none.
*/
XDL_API void  xtcp_close(xhand_t tcp);

/*
@FUNCTION xtcp_write: write TCP data.
@INPUT xhand_t tcp: the TCP handle.
@INPUT const byte_t* data: the data buffer.
@INOUTPUT dword_t* pb: indicate the bytes to write and return the bytes writed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t  xtcp_write(xhand_t tcp, const byte_t* data, dword_t* pb);

/*
@FUNCTION xtcp_read: read TCP data.
@INPUT xhand_t tcp: the TCP handle.
@OUTPUT byte_t* data: the data buffer.
@INOUTPUT dword_t* pb: indicate the bytes to read and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t  xtcp_read(xhand_t tcp, byte_t* data, dword_t* pb);

/*
@FUNCTION xtcp_addr_port: get TCP local address and port.
@INPUT xhand_t tcp: the TCP handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the local port.
*/
XDL_API unsigned short xtcp_addr_port(xhand_t tcp, tchar_t* addr);

/*
@FUNCTION xtcp_peer_port: get TCP remote address and port.
@INPUT xhand_t tcp: the TCP handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the remote port.
*/
XDL_API unsigned short xtcp_peer_port(xhand_t tcp, tchar_t* addr);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/