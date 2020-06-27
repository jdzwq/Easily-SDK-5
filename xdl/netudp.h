/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl udp document

	@module	netudp.h | interface file

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

#ifndef _NETUDP_H
#define _NETUDP_H

#include "xdldef.h"
#include "httpattr.h"

#if defined(XDK_SUPPORT_SOCK)

typedef enum{
	_XUDP_TYPE_LIS = 0,
	_XUDP_TYPE_CLI = 1,
	_XUDP_TYPE_SRV = 2
}XUDP_TYPE;


#define UDP_BASE_TIMO		(5000) //millionsecond
#define UDP_BASE_BUFF		(65535)
#define UDP_MIN_PORT		(49152)
#define UDP_MAX_PORT		(65535)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xudp_cli: create a UDP client.
@INPUT unsigned short port: the network port to connect.
@INPUT const tchar_t* addr: the network address to connect.
@RETURN xhand_t: if succeeds return UDP client handle, fails return NULL.
*/
EXP_API xhand_t xudp_cli(unsigned short port, const tchar_t* addr);

/*
@FUNCTION xudp_srv: create a UDP server.
@INPUT unsigned short port: the network port from client.
@INPUT const tchar_t* addr: the network address from client.
@INPUT const byte_t* pack: the network package from client.
@INPUT dword_t size: the network package size in bytes.
@RETURN xhand_t: if succeeds return UDP server handle, fails return NULL.
*/
EXP_API xhand_t xudp_srv(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size);

/*
@FUNCTION xudp_bind: bind the local port.
@INPUT xhand_t udp: the UDP xhandle.
@INPUT unsigned short port: the network port from binding.
@RETURN bool_t: if succeeds return nonzero.
*/
EXP_API bool_t xudp_bind(xhand_t udp, unsigned short bind);

/*
@FUNCTION xudp_cli: conenct to some addr.
@INPUT xhand_t udp: the UDP xhandle.
@INPUT unsigned short port: the network port to connect.
@INPUT const tchar_t* addr: the network address to connect.
@RETURN bool_t: if succeeds return nonzero.
*/
EXP_API bool_t xudp_connect(xhand_t udp, unsigned short port, const tchar_t* addr);

/*
@FUNCTION xudp_close: close a UDP client or server handle.
@INPUT xhand_t udp: the UDP xhandle.
@RETURN void: none.
*/
EXP_API void xudp_close(xhand_t udp);

/*
@FUNCTION xudp_type: get UDP type, it can be _XUDP_TYPE_CLI, _XUDP_TYPE_SRV.
@INPUT xhand_t udp: the UDP xhandle.
@RETURN int: the UDP type.
*/
EXP_API int xudp_type(xhand_t udp);

/*
@FUNCTION xudp_socket: get socket resource handle.
@INPUT xhand_t udp: the TCP handle.
@RETURN res_file_t: return the socket resource handle.
*/
EXP_API res_file_t xudp_socket(xhand_t udp);

/*
@FUNCTION xudp_setopt: set the socket options.
@INPUT xhand_t udp: the udp handle.
@INPUT int oid: the option id, eg: SOCK_OPTION_SNDBUF, SOCK_OPTION_RCVBUF, SOCK_OPTION_NONBLK.
@INPUT void* opt: the option value pointer
@INPUT int len: the value length in bytes, string value must be a zero terminated token and set len to zero.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xudp_setopt(xhand_t udp, int oid, void* opt, int len);

/*
@FUNCTION xudp_settmo: set the socket timeout.
@INPUT xhand_t udp: the udp handle.
@INPUT dword_t tmo: the tmieout in millsecoend.
@RETURN void: none
*/
EXP_API void xudp_settmo(xhand_t ucp, dword_t tmo);

/*
@FUNCTION xudp_set_package: set UDP package size.
@INPUT xhand_t udp: the UDP xhandle.
@INPUT dword_t size: the package size in bytes
@RETURN void: none.
*/
EXP_API void xudp_set_package(xhand_t udp, dword_t size);

/*
@FUNCTION xudp_get_package: get UDP package size.
@INPUT xhand_t udp: the UDP xhandle.
@RETURN dword_t: return the package size in bytes.
*/
EXP_API dword_t xudp_get_package(xhand_t udp);

/*
@FUNCTION xudp_write: write UDP data.
@INPUT xhand_t udp: the UDP xhandle.
@INPUT const byte_t* data: the data bytes buffer.
@INOUTPUT dword_t* pb: input the size for writing, and return the bytes writed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xudp_write(xhand_t udp, const byte_t* data, dword_t* pb);

/*
@FUNCTION xudp_flush: ensure UDP data writed.
@INPUT xhand_t udp: the UDP xhandle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xudp_flush(xhand_t udp);

/*
@FUNCTION xudp_read: read UDP data.
@INPUT xhand_t udp: the UDP xhandle.
@OUTPUT byte_t* data: the bytes buffer for reading.
@INOUTPUT dword_t* pb: input the size for reading, and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xudp_read(xhand_t udp, byte_t* data, dword_t* pb);

/*
@FUNCTION xudp_addr_port: get UDP local address and port.
@INPUT xhand_t udp: the UDP handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the local port.
*/
EXP_API unsigned short xudp_addr_port(xhand_t udp, tchar_t* addr);

/*
@FUNCTION xudp_peer_port: get UDP remote address and port.
@INPUT xhand_t udp: the UDP handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the remote port.
*/
EXP_API unsigned short xudp_peer_port(xhand_t udp, tchar_t* addr);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*XDLINET_H*/