/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc udp document

	@module	netudp.h | udp interface file

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

#ifdef XDK_SUPPORT_SOCK

typedef enum{
	_XUDP_TYPE_LIS = 0,
	_XUDP_TYPE_CLI = 1,
	_XUDP_TYPE_SRV = 2
}XUDP_TYPE;


#define UDP_BASE_TIMO		3000


#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t xudp_cli(unsigned short bind, unsigned short port, const tchar_t* addr);

XDL_API xhand_t xudp_srv(unsigned short bind, unsigned short port, const tchar_t* addr, const byte_t* pack);

XDL_API bool_t xudp_lis(unsigned short bind, unsigned short* port, tchar_t* addr, byte_t* pack, dword_t* pb);

XDL_API void xudp_close(xhand_t udp);

XDL_API int xudp_type(xhand_t udp);;

XDL_API res_file_t xudp_socket(xhand_t udp);

XDL_API int  xudp_type(xhand_t udp);

XDL_API void xudp_set_send_timeout(xhand_t udp, int s);

XDL_API void xudp_set_recv_timeout(xhand_t udp, int s);

XDL_API void xudp_set_linger(xhand_t udp, bool_t b_wait, int n_sec);

XDL_API bool_t xudp_write(xhand_t udp, const byte_t* data, dword_t* pb);

XDL_API bool_t xudp_flush(xhand_t udp);

XDL_API bool_t xudp_read(xhand_t udp, byte_t* data, dword_t* pb);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/