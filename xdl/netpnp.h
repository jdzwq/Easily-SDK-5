/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl pnp document

	@module	netpnp.h | xdl pnp interface file

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

#define PNP_BASE_TIMO		15000
#define PNP_BASE_BUFF		65535
#define PNP_MIN_PORT		49152
#define PNP_MAX_PORT		65535

#define PNP_PKG_SIZE	516
#define PNP_HDR_SIZE	0
#define PNP_PDU_SIZE    (PNP_PKG_SIZE - PNP_HDR_SIZE)

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API xhand_t xpnp_cli(unsigned short port, const tchar_t* addr);

	XDL_API xhand_t xpnp_srv(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size);

	XDL_API void xpnp_close(xhand_t pnp);

	XDL_API int xpnp_type(xhand_t pnp);;

	XDL_API void xpnp_set_send_timeout(xhand_t pnp, int ms);

	XDL_API void xpnp_set_recv_timeout(xhand_t pnp, int ms);

	XDL_API bool_t xpnp_write(xhand_t pnp, const byte_t* data, dword_t* pb);

	XDL_API bool_t xpnp_flush(xhand_t pnp);

	XDL_API bool_t xpnp_read(xhand_t pnp, byte_t* data, dword_t* pb);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*XDLINET_H*/