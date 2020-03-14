/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl pnp document

	@module	netpnp.h | interface file

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

#ifndef _NETPNP_H
#define _NETPNP_H

#include "xdldef.h"
#include "httpattr.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_RAND)

typedef enum{
	_XPNP_TYPE_LIS = 0,
	_XPNP_TYPE_CLI = 1,
	_XPNP_TYPE_SRV = 2
}XPNP_TYPE;

#define PNP_BASE_TIMO		(2000) //millionsecond
#define PNP_MIN_PORT		(49152)
#define PNP_MAX_PORT		(65535)

#define PNP_PKG_SIZE	516
#define PNP_HDR_SIZE	0
#define PNP_PDU_SIZE    (PNP_PKG_SIZE - PNP_HDR_SIZE)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xpnp_cli: create a PNP client.
@INPUT unsigned short port: the network port to connect.
@INPUT const tchar_t* addr: the network address to connect.
@RETURN xhand_t: if succeeds return PNP client handle, fails return NULL.
*/
EXP_API xhand_t xpnp_cli(unsigned short port, const tchar_t* addr);

/*
@FUNCTION xpnp_srv: create a PNP server.
@INPUT unsigned short port: the network port from client.
@INPUT const tchar_t* addr: the network address from client.
@INPUT const byte_t* pack: the network package from client.
@INPUT dword_t size: the network package size in bytes.
@RETURN xhand_t: if succeeds return PNP server handle, fails return NULL.
*/
EXP_API xhand_t xpnp_srv(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size);

/*
@FUNCTION xpnp_close: close a PNP client or server handle.
@INPUT xhand_t pnp: the PNP xhandle.
@RETURN void: none.
*/
EXP_API void xpnp_close(xhand_t pnp);

/*
@FUNCTION xpnp_type: get PNP type, it can be _XPNP_TYPE_CLI, _XPNP_TYPE_SRV.
@INPUT xhand_t pnp: the PNP xhandle.
@RETURN int: the PNP type.
*/
EXP_API int xpnp_type(xhand_t pnp);;

/*
@FUNCTION xpnp_write: write PNP data.
@INPUT xhand_t pnp: the PNP xhandle.
@INPUT const byte_t* data: the data bytes buffer.
@INOUTPUT dword_t* pb: input the size for writing, and return the bytes writed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xpnp_write(xhand_t pnp, const byte_t* data, dword_t* pb);

/*
@FUNCTION xpnp_flush: ensure PNP data writed.
@INPUT xhand_t pnp: the PNP xhandle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xpnp_flush(xhand_t pnp);

/*
@FUNCTION xpnp_read: read PNP data.
@INPUT xhand_t pnp: the PNP xhandle.
@OUTPUT byte_t* data: the bytes buffer for reading.
@INOUTPUT dword_t* pb: input the size for reading, and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xpnp_read(xhand_t pnp, byte_t* data, dword_t* pb);

/*
@FUNCTION xpnp_addr_port: get PNP local address and port.
@INPUT xhand_t pnp: the PNP handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the local port.
*/
EXP_API unsigned short xpnp_addr_port(xhand_t pnp, tchar_t* addr);

/*
@FUNCTION xpnp_peer_port: get PNP remote address and port.
@INPUT xhand_t pnp: the PNP handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the remote port.
*/
EXP_API unsigned short xpnp_peer_port(xhand_t pnp, tchar_t* addr);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*XDLINET_H*/