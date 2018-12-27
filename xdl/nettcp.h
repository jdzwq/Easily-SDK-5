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
#define TCP_BASE_TIMO	5000

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t xtcp_cli(unsigned short port, const tchar_t* addr);

XDL_API xhand_t xtcp_srv(res_file_t so);

XDL_API res_file_t xtcp_socket(xhand_t tcp);

XDL_API int  xtcp_type(xhand_t tcp);

XDL_API void xtcp_set_send_buff(xhand_t tcp, dword_t dw);

XDL_API void xtcp_set_recv_buff(xhand_t tcp, dword_t dw);

XDL_API void xtcp_set_send_timeout(xhand_t tcp, int sec);

XDL_API void xtcp_set_recv_timeout(xhand_t tcp, int sec);

XDL_API void xtcp_set_linger(xhand_t tcp, bool_t b_wait, int n_sec);

XDL_API void  xtcp_close(xhand_t tcp);

XDL_API bool_t  xtcp_write(xhand_t tcp, const byte_t* data, dword_t* pb);

XDL_API bool_t  xtcp_read(xhand_t tcp, byte_t* data, dword_t* pb);

XDL_API unsigned short xtcp_addr_port(xhand_t tcp, tchar_t* addr);

XDL_API unsigned short xtcp_peer_port(xhand_t tcp, tchar_t* addr);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/