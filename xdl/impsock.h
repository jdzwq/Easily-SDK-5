/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc socket document

	@module	impsock.h | socket interface file

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

#ifndef _IMPSOCK_H
#define _IMPSOCK_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_SOCK


#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API res_file_t xsocket_tcp(int ver, dword_t flag);

	XDL_API res_file_t xsocket_udp(int ver, dword_t flag);

	XDL_API res_file_t xsocket_icmp(int ver, dword_t flag);

	XDL_API void xsocket_shutdown(res_file_t so, int how);

	XDL_API void xsocket_close(res_file_t so);

	XDL_API dword_t xsocket_wait(res_file_t so, dword_t, int ms);

	XDL_API bool_t xsocket_bind(res_file_t so, res_addr_t saddr, int slen);

	XDL_API bool_t xsocket_listen(res_file_t so, int max);

	XDL_API bool_t	xsocket_connect(res_file_t so, res_addr_t saddr, int slen);

	XDL_API res_file_t xsocket_accept(res_file_t so, res_addr_t saddr, int *plen, async_t* pov);

	XDL_API bool_t xsocket_sendto(res_file_t so, res_addr_t saddr, int alen, void* buf, dword_t size, async_t* pov);

	XDL_API bool_t xsocket_recvfrom(res_file_t so, res_addr_t saddr, int *plen, void* buf, dword_t size, async_t* pov);

	XDL_API bool_t	xsocket_send(res_file_t so, const byte_t* buf, dword_t size, async_t* pov);

	XDL_API bool_t	xsocket_recv(res_file_t so, byte_t* buf, dword_t size, async_t* pov);

	XDL_API bool_t	xsocket_setopt(res_file_t so, int level, int optname, const char* optval, int optlen);

	XDL_API bool_t	xsocket_getopt(res_file_t so, int level, int optname, char* pval, int* plen);

	XDL_API bool_t	xsocket_set_linger(res_file_t so, bool_t wait, int sec);

	XDL_API bool_t	xsocket_set_sndbuf(res_file_t so, int size);

	XDL_API bool_t	xsocket_set_rcvbuf(res_file_t so, int size);

	XDL_API bool_t	xsocket_set_sndtmo(res_file_t so, int ms);

	XDL_API bool_t	xsocket_set_rcvtmo(res_file_t so, int ms);

	XDL_API bool_t	host_addr(const tchar_t* host, tchar_t* addr);

	XDL_API void	fill_addr(net_addr_t* paddr, short port, const tchar_t* addr);

	XDL_API void	conv_addr(net_addr_t* paddr, short* port, tchar_t* addr);

	XDL_API void	xsocket_addr(res_file_t so, net_addr_t* paddr);

	XDL_API void	xsocket_peer(res_file_t so, net_addr_t* paddr);

	XDL_API bool_t xsocket_share(dword_t procid, res_file_t procfd, res_file_t so, const byte_t* data, dword_t size);

	XDL_API res_file_t xsocket_dupli(res_file_t procfd, u32_t flag, byte_t* data, dword_t* pb);

	int	xsocket_write(void* pso, unsigned char* buf, int len);

	int	xsocket_read(void* pso, unsigned char* buf, int len);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/