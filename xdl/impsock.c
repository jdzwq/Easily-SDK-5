/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc socket document

	@module	impsock.c | implement file

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

#include "impsock.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdlinit.h"

#ifdef XDK_SUPPORT_SOCK

void set_network_error(const tchar_t* code)
{
	tchar_t err[ERR_LEN + 1] = { 0 };
	if_socket_t* pif_so;
	int len;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	len = (*pif_so->pf_socket_error)(err, ERR_LEN);

	set_last_error(code, err, len);
}

res_file_t socket_tcp(int ver, dword_t flag)
{
	if_socket_t* pif_so;
	res_file_t sd;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	sd = (*pif_so->pf_socket_tcp)(ver, flag);
	if (sd == INVALID_FILE)
	{
		set_network_error(_T("socket_tcp"));
	}

	return sd;
}

res_file_t socket_udp(int ver, dword_t flag)
{
	if_socket_t* pif_so;
	res_file_t sd;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	sd = (*pif_so->pf_socket_udp)(ver, flag);
	if (sd == INVALID_FILE)
	{
		set_network_error(_T("socket_udp"));
	}

	return sd;
}

res_file_t socket_icmp(int ver, dword_t flag)
{
	if_socket_t* pif_so;
	res_file_t sd;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	sd = (*pif_so->pf_socket_icmp)(ver, flag);
	if (sd == INVALID_FILE)
	{
		set_network_error(_T("socket_icmp"));
	}

	return sd;
}

void socket_shutdown(res_file_t so, int how)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	(*pif_so->pf_socket_shutdown)(so, how);
}

void socket_close(res_file_t so)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	(*pif_so->pf_socket_close)(so);
}

dword_t socket_wait(res_file_t so, dword_t msk, int ms)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	return (*pif_so->pf_socket_wait)(so, msk, ms);
}

bool_t socket_bind(res_file_t so, res_addr_t saddr, int slen)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_bind)(so, saddr, slen))
	{
		set_network_error(_T("socket_bind"));
		return 0;
	}

	return 1;
}

bool_t socket_listen(res_file_t so, int max)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_listen)(so, max))
	{
		set_network_error(_T("socket_listen"));
		return 0;
	}

	return 1;
}

bool_t	socket_connect(res_file_t so, res_addr_t saddr, int slen)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_connect)(so, saddr, slen))
	{
		set_network_error(_T("socket_connect"));
		return 0;
	}

	return 1;
}

res_file_t socket_accept(res_file_t so, res_addr_t saddr, int *plen, async_t* pb)
{
	if_socket_t* pif_so;
	res_file_t sd;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	sd = (*pif_so->pf_socket_accept)(so, saddr, plen, pb);
	if (sd == INVALID_FILE)
	{
		set_network_error(_T("socket_accept"));
	}

	return sd;
}

bool_t socket_sendto(res_file_t so, res_addr_t saddr, int alen, const byte_t* buf, dword_t size, async_t* pov)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	pov->size = 0;
	if (!(*pif_so->pf_socket_sendto)(so, saddr, alen, (void*)(buf), size, pov))
	{
		set_network_error(_T("socket_sendto"));

		pov->size = 0;
		return 0;
	}

	return 1;
}

bool_t socket_recvfrom(res_file_t so, res_addr_t saddr, int *plen, byte_t* buf, dword_t size, async_t* pov)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	pov->size = 0;
	if (!(*pif_so->pf_socket_recvfrom)(so, saddr, plen, (void*)(buf), size, pov))
	{
		set_network_error(_T("socket_recvfrom"));

		pov->size = 0;
		return 0;
	}

	return 1;
}

bool_t	socket_send(res_file_t so, const byte_t* buf, dword_t size, async_t* pov)
{
	if_socket_t* pif_so;
	dword_t pos = 0;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	while (pos < size)
	{
		pov->size = 0;
		if (!(*pif_so->pf_socket_send)(so, (void*)(buf + pos), size - pos, pov))
		{
			set_network_error(_T("socket_send"));

			pov->size = pos;
			return 0;
		}
		
		if (!(pov->size)) break;

		pos += pov->size;
	}

	pov->size = pos;

	return 1;
}

bool_t	socket_recv(res_file_t so, byte_t* buf, dword_t size, async_t* pov)
{
	if_socket_t* pif_so;
	dword_t pos = 0;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	while (pos < size)
	{
		pov->size = 0;
		if (!(*pif_so->pf_socket_recv)(so, (void*)(buf + pos), size - pos, pov))
		{
			set_network_error(_T("socket_recv"));

			pov->size = pos;
			return 0;
		}
		
		if (!(pov->size)) break;
		
		pos += pov->size;
	}

	pov->size = pos;
	return 1;
}

int	socket_write(void* pso, unsigned char* buf, int len)
{
	if_socket_t* pif_so;
	
	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (C_ERR == (*pif_so->pf_socket_write)(pso, buf, len))
	{
		set_network_error(_T("socket_write"));
		return C_ERR;
	}

	return C_OK;
}

int	socket_read(void* pso, unsigned char* buf, int len)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (C_ERR == (*pif_so->pf_socket_read)(pso, buf, len))
	{
		set_network_error(_T("socket_read"));
		return C_ERR;
	}

	return C_OK;
}

bool_t	socket_setopt(res_file_t so, int optname, const char* optval, int optlen)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_setopt)(so, optname, optval, optlen))
	{
		set_network_error(_T("socket_setopt"));
		return 0;
	}

	return 1;
}

bool_t	socket_getopt(res_file_t so, int optname, char* pval, int* plen)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_getopt)(so, optname, pval, plen))
	{
		set_network_error(_T("socket_getopt"));
		return 0;
	}

	return 1;
}

bool_t	socket_set_linger(res_file_t so, bool_t wait, int sec)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_set_linger)(so, wait, sec))
	{
		set_network_error(_T("socket_set_linger"));
		return 0;
	}

	return 1;
}

bool_t	socket_set_sndbuf(res_file_t so, int size)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_set_sndbuf)(so, size))
	{
		set_network_error(_T("socket_set_sndbuf"));
		return 0;
	}

	return 1;
}

bool_t	socket_set_rcvbuf(res_file_t so, int size)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_set_rcvbuf)(so, size))
	{
		set_network_error(_T("socket_set_rcvbuf"));
		return 0;
	}

	return 1;
}

bool_t	socket_set_nonblk(res_file_t so, bool_t none)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (!(*pif_so->pf_socket_set_nonblk)(so, none))
	{
		set_network_error(_T("socket_set_nonblk"));
		return 0;
	}

	return 1;
}

bool_t	socket_get_nonblk(res_file_t so)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	return (*pif_so->pf_socket_get_nonblk)(so);
}

bool_t host_addr(const tchar_t* host, tchar_t* addr)
{
	if_socket_t* pif_so;
	schar_t shost[META_LEN] = { 0 };
	schar_t saddr[ADDR_LEN] = { 0 };

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

#ifdef _UNICODE
	ucs_to_mbs(host, w_xslen(host), shost, META_LEN);
#else
	a_xsncpy(shost, host, META_LEN);
#endif

	if (!(*pif_so->pf_host_addr)(shost, saddr))
	{
		set_network_error(_T("host_addr"));
		return 0;
	}

#ifdef _UNICODE
	mbs_to_ucs(saddr, a_xslen(saddr), addr, ADDR_LEN);
#else
	a_xsncpy(addr, saddr, ADDR_LEN);
#endif

	return 1;
}

void fill_addr(net_addr_t* paddr, unsigned short port, const tchar_t* addr)
{
	if_socket_t* pif_so;
	schar_t saddr[ADDR_LEN] = { 0 };

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

#ifdef _UNICODE
	ucs_to_mbs(addr, w_xslen(addr), saddr, ADDR_LEN);
#else
	a_xsncpy(saddr, addr, ADDR_LEN);
#endif

	(*pif_so->pf_fill_addr)(paddr, port, saddr);
}

void conv_addr(net_addr_t* paddr, unsigned short* port, tchar_t* addr)
{
	if_socket_t* pif_so;
	schar_t saddr[ADDR_LEN + 1] = { 0 };
	int len;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	(*pif_so->pf_conv_addr)(paddr, port, saddr);

#ifdef _UNICODE
	len = mbs_to_ucs(saddr, a_xslen(saddr), addr, ADDR_LEN);
	addr[len] = _T('\0');
#else
	a_xsncpy(addr, saddr, ADDR_LEN);
#endif
}

void socket_addr(res_file_t so, net_addr_t* paddr)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	(*pif_so->pf_socket_addr)(so, paddr);
}

void socket_peer(res_file_t so, net_addr_t* paddr)
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	(*pif_so->pf_socket_peer)(so, paddr);
}

bool_t socket_share(pid_t procid, res_file_t procfd, res_file_t so, const byte_t* data, dword_t size)
{
	if_socket_t* pif_so;
	bool_t rt = 0;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	rt = (*pif_so->pf_socket_share)(procid, procfd, so, (void*)data, size);
	if (!rt)
	{
		set_network_error(_T("socket_share"));
		return 0;
	}

	return rt;
}

res_file_t socket_dupli(res_file_t procfd, byte_t* data, dword_t* pb)
{
	if_socket_t* pif_so;
	res_file_t sd;
	dword_t size;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

    size = (pb)? (*pb) : 0;
    
	sd = (*pif_so->pf_socket_dupli)(procfd, (void*)data, &size);
	if (sd == INVALID_FILE)
	{
		set_network_error(_T("socket_dupli"));
	}
    
	if(pb) *pb = (dword_t)size;

	return sd;
}

#endif //XDK_SUPPORT_SOCK
