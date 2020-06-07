/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc socket document

	@module	if_socket_win.c | windows implement file

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

#include "xdkiml.h"

#ifdef XDK_SUPPORT_SOCK

#include <mswsock.h>
#ifdef WINCE
#pragma comment(lib,"ws2.lib")
#else
#pragma comment(lib,"ws2_32.lib")
#endif


bool_t _host_addr(const schar_t* host, schar_t* addr)
{
#ifdef _WIN64
	struct addrinfo *p, *paddr = NULL;
	struct addrinfo hints = { 0 };
	struct sockaddr_in* pin;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(host, 0, &hints, &paddr))
		return 0;

	for (p = paddr; p != NULL; p = p->ai_next)
	{
		switch (p->ai_family)
		{
		case AF_UNSPEC:
			break;
		case AF_INET:
			pin = (struct sockaddr_in *) p->ai_addr;
			inet_ntop(AF_INET, &pin->sin_addr, addr, ADDR_LEN);
			return 1;
		case AF_INET6:
			pin = (struct sockaddr_in *) p->ai_addr;
			inet_ntop(AF_INET6, &pin->sin_addr, addr, ADDR_LEN);
			break;
		case AF_NETBIOS:
			break;
		default:
			break;
		}
	}

	freeaddrinfo(paddr);
	return 0;
#else
	int i;
	struct hostent *hos;
	struct in_addr *addr_list;

	hos = gethostbyname((char*)host);
	if (!hos)
		return 0;

	i = 0;
	if (hos->h_addrtype == AF_INET)
	{
		while (hos->h_addr_list[i] != 0) {
			addr_list = (struct in_addr *)hos->h_addr_list[i++];
			lstrcpyA(addr, inet_ntoa(*addr_list));
			return 1;
		}
	}

	return 0;
#endif
}


void _fill_addr(net_addr_t* paddr, unsigned short port, const schar_t* saddr)
{
	paddr->sin_family = AF_INET;
	paddr->sin_port = htons((u_short)port);
	if (!saddr || !saddr[0])
		paddr->sin_addr.s_addr = INADDR_ANY;
	else
#ifdef _WIN64
		inet_pton(AF_INET, saddr, &paddr->sin_addr);
#else
		paddr->sin_addr.s_addr = inet_addr(saddr);
#endif
}

void _conv_addr(net_addr_t* paddr, unsigned short* port, schar_t* saddr)
{
	*port = ntohs(paddr->sin_port);
#ifdef _WIN64
	inet_ntop(AF_INET, &paddr->sin_addr, saddr, ADDR_LEN);
#else
	lstrcpyA(saddr, inet_ntoa(paddr->sin_addr));
#endif
}

void _socket_addr(res_file_t so, net_addr_t* paddr)
{
	int ns;

	ns = sizeof(net_addr_t);
	getsockname((SOCKET)so, (SOCKADDR *)paddr, &ns);
}

void _socket_peer(res_file_t so, net_addr_t* paddr)
{
	int ns;

	ns = sizeof(net_addr_t);
	getpeername((SOCKET)so, (SOCKADDR *)paddr, &ns);
}

bool_t _socket_startup(void)
{
	WSADATA wsa = { 0 };

	return (WSAStartup(MAKEWORD(2, 2), &wsa) == 0) ? 1 : 0;
}

void _socket_cleanup(void)
{
	WSACleanup();
}

int _socket_error(tchar_t* buf, int max)
{
	DWORD dwErr,dwLen;
	void *token;

	dwErr = WSAGetLastError();

	dwLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&token, 0, NULL);

	if (token)
	{
		dwLen = ((int)dwLen < max) ? dwLen : max;
		if (buf)
		{
			CopyMemory((void*)buf, token, dwLen * sizeof(tchar_t));
			buf[dwLen] = _T('\0');
		}
		LocalFree(token);
	}
	else
	{
		if (buf)
		{
			buf[0] = _T('\0');
		}
	}

	return dwLen;
}

res_file_t _socket_tcp(int ver, dword_t flag)
{
	SOCKET so;
	DWORD dw = 0;

	if(flag & FILE_OPEN_OVERLAP)
		dw |= WSA_FLAG_OVERLAPPED;

	so = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, dw);

	return (so == INVALID_SOCKET) ? INVALID_FILE : (res_file_t)so;
}

res_file_t _socket_udp(int ver, dword_t flag)
{
	SOCKET so;
	DWORD dw = 0;

	if (flag & FILE_OPEN_OVERLAP)
		dw |= WSA_FLAG_OVERLAPPED;

	so = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, dw);

	return (so == INVALID_SOCKET) ? INVALID_FILE : (res_file_t)so;
}

res_file_t _socket_icmp(int ver, dword_t flag)
{
	SOCKET so;
	DWORD dw = 0;

	if (flag & FILE_OPEN_OVERLAP)
		dw |= WSA_FLAG_OVERLAPPED;

	so = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, dw);

	return (so == INVALID_SOCKET) ? INVALID_FILE : (res_file_t)so;
}

res_file_t _socket_open(int af, int type, int protocol, dword_t flag)
{
	SOCKET so;
	DWORD dw = 0;

	if (flag & FILE_OPEN_OVERLAP)
		dw |= WSA_FLAG_OVERLAPPED;

	so = WSASocket(af, type, protocol, NULL, 0, dw);

	return (so == INVALID_SOCKET) ? INVALID_FILE : (res_file_t)so;
}

void _socket_shutdown(res_file_t so, int how)
{
	shutdown((SOCKET)so, how);
}

void _socket_close(res_file_t so)
{
	closesocket((SOCKET)so);
}

/*wait_t _socket_wait(res_file_t so, int ms)
{
	fd_set fr;
	struct timeval tv = { 0 };

	FD_ZERO(&fr);
	FD_SET((SOCKET)so, &fr);

	tv.tv_sec = 0;
	tv.tv_usec = ms * 1000;

	if(!select(0, &fr, NULL, NULL, &tv))
		return WAIT_TMO;

	return (FD_ISSET(so, &fr)) ? WAIT_RET : WAIT_ERR;
}*/

dword_t _socket_wait(res_file_t so, dword_t msk, int ms)
{
	HANDLE ev;
	WSANETWORKEVENTS  ns = { 0 };
	DWORD dw;

	ev = WSACreateEvent();
	if (!ev)
		return 0;

	if (0 != WSAEventSelect((SOCKET)so, ev, msk))
	{
		WSACloseEvent(ev);
		return 0;
	}

	if (ms < 0)
		dw = INFINITE;
	else
		dw = ms;

	dw = WSAWaitForMultipleEvents(1, &ev, 0, dw, 0);
	if (dw == WSA_WAIT_TIMEOUT || dw == WSA_WAIT_FAILED)
	{
		WSACloseEvent(ev);
		return 0;
	}

	WSAEnumNetworkEvents((SOCKET)so, ev, &ns);

	WSACloseEvent(ev);

	return ns.lNetworkEvents;
}

bool_t _socket_bind(res_file_t so, res_addr_t saddr, int slen)
{
	return (bind((SOCKET)so, saddr, slen) == 0) ? 1 : 0;
}

bool_t _socket_connect(res_file_t so, res_addr_t saddr, int slen)
{
	return (WSAConnect((SOCKET)so, saddr, slen, NULL, NULL, NULL, NULL) == 0) ? 1 : 0;
}

bool_t _socket_sendto(res_file_t so, res_addr_t saddr, int alen, void* buf, dword_t size, async_t* pb)
{
	LPOVERLAPPED pov = (pb) ? (LPOVERLAPPED)pb->lapp : NULL;
	dword_t* pcb = (pb) ? &(pb->size) : NULL;

	ULONG_PTR up = NULL;
	LPOVERLAPPED ul = NULL;

	int rt;
	WSABUF wb;
	DWORD fd, dw = 0, fg = 0;

	wb.buf = (char*)buf;
	wb.len = (ULONG)size;

	rt = WSASendTo((SOCKET)so, &wb, 1, &dw, fg, (SOCKADDR*)saddr, alen, (LPWSAOVERLAPPED)pov, NULL);
	if (!dw && rt == SOCKET_ERROR)
	{
		if (!pov)
		{
			if (pcb) *pcb = 0;
			return 0;
		}

		if (WSA_IO_PENDING != WSAGetLastError())
		{
			*pcb = 0;
			return 0;
		}

		if (pb->type == ASYNC_QUEUE)
		{
			dw = 0;
			if (!GetQueuedCompletionStatus((HANDLE)pb->port, &dw, &up, &ul, pb->timo))
			{
				*pcb = dw;
				return (dw)? 1 : 0;
			}
		}
		if (pb->type == ASYNC_EVENT)
		{
			if (WAIT_OBJECT_0 != WaitForSingleObject(pov->hEvent, pb->timo))
			{
				*pcb = 0;
				return 0;
			}

			dw = 0;
			WSAGetOverlappedResult((SOCKET)so, (LPWSAOVERLAPPED)pov, &dw, FALSE, &fd);
			*pcb = dw;
			return (dw) ? 1 : 0;
		}
	}

	if (pov) WSAResetEvent(pov->hEvent);

	if (pcb) *pcb = dw;
	
	return (dw) ? 1 : 0;
}

bool_t _socket_recvfrom(res_file_t so, res_addr_t saddr, int* plen, void* buf, dword_t size, async_t* pb)
{
	LPOVERLAPPED pov = (pb) ? (LPOVERLAPPED)pb->lapp : NULL;
	dword_t* pcb = (pb) ? &(pb->size) : NULL;

	ULONG_PTR up = NULL;
	LPOVERLAPPED ul = NULL;

	int rt, alen = 0;
	WSABUF wb;
	DWORD fd, dw = 0, fg = 0;

	wb.buf = (char*)buf;
	wb.len = (ULONG)size;

	alen = *plen;
	rt = WSARecvFrom((SOCKET)so, &wb, 1, &dw, &fg, (SOCKADDR*)saddr, &alen, (LPWSAOVERLAPPED)pov, NULL);
	*plen = alen;

	if (!dw && rt == SOCKET_ERROR)
	{
		if (!pov)
		{
			if (pcb) *pcb = 0;
			return 0;
		}

		if (WSA_IO_PENDING != WSAGetLastError())
		{
			*pcb = 0;
			return 0;
		}

		if (pb->type == ASYNC_QUEUE)
		{
			dw = 0;
			if (!GetQueuedCompletionStatus((HANDLE)pb->port, &dw, &up, &ul, pb->timo))
			{
				*pcb = dw;
				return (dw) ? 1 : 0;
			}
		}
		if (pb->type == ASYNC_EVENT)
		{
			if (WAIT_OBJECT_0 != WaitForSingleObject(pov->hEvent, pb->timo))
			{
				*pcb = 0;
				return 0;
			}

			dw = 0;
			WSAGetOverlappedResult((SOCKET)so, (LPWSAOVERLAPPED)pov, &dw, FALSE, &fd);
			*pcb = dw;
			return (dw) ? 1 : 0;
		}
	}

	if (pov) WSAResetEvent(pov->hEvent);

	if (pcb) *pcb = dw;

	return (dw) ? 1 : 0;
}

bool_t _socket_send(res_file_t so, void* buf, dword_t len, async_t* pb)
{
	LPOVERLAPPED pov = (pb) ? (LPOVERLAPPED)pb->lapp : NULL;
	dword_t* pcb = (pb) ? &(pb->size) : NULL;

	ULONG_PTR up = NULL;
	LPOVERLAPPED ul = NULL;

	int rt;
	WSABUF wb;
	DWORD fd, dw = 0, fg = 0;

	wb.buf = (char*)buf;
	wb.len = (ULONG)len;

	rt = WSASend((SOCKET)so, &wb, 1, &dw, fg, (LPWSAOVERLAPPED)pov, NULL);
	if (rt == SOCKET_ERROR)
	{
		if (!pov)
		{
			if (pcb) *pcb = 0;
			return 0;
		}

		if (WSA_IO_PENDING != WSAGetLastError())
		{
			*pcb = 0;
			return 0;
		}
		if (pb->type == ASYNC_QUEUE)
		{
			dw = 0;
			if (!GetQueuedCompletionStatus((HANDLE)pb->port, &dw, &up, &ul, pb->timo))
			{
				*pcb = 0;
				return 0;
			}
		}
		if (pb->type == ASYNC_EVENT)
		{
			if (WAIT_OBJECT_0 != WaitForSingleObject(pov->hEvent, pb->timo))
			{
				*pcb = 0;
				return 0;
			}

			dw = 0;
			if (!WSAGetOverlappedResult((SOCKET)so, (LPWSAOVERLAPPED)pov, &dw, FALSE, &fd))
			{
				*pcb = 0;
				return 0;
			}
		}
	}

	if (pov) WSAResetEvent(pov->hEvent);

	if (pcb) *pcb = dw;

	return 1;
}

bool_t _socket_recv(res_file_t so, void* buf, dword_t len, async_t* pb)
{
	LPOVERLAPPED pov = (pb) ? (LPOVERLAPPED)pb->lapp : NULL;
	dword_t* pcb = (pb) ? &(pb->size) : NULL;

	ULONG_PTR up = NULL;
	LPOVERLAPPED ul = NULL;

	int rt;
	WSABUF wb;
	DWORD fd, dw = 0, fg = 0;

	wb.buf = (char*)buf;
	wb.len = (ULONG)len;

	rt = WSARecv((SOCKET)so, &wb, 1, &dw, &fg, (LPWSAOVERLAPPED)pov, NULL);
	if (rt == SOCKET_ERROR)
	{
		if (!pov)
		{
			if (pcb) *pcb = 0;
			return 0;
		}

		if (WSA_IO_PENDING != WSAGetLastError())
		{
			*pcb = 0;
			return 0;
		}

		if (pb->type == ASYNC_QUEUE)
		{
			if (!GetQueuedCompletionStatus((HANDLE)pb->port, &dw, &up, &ul, pb->timo))
			{
				*pcb = 0;
				return 0;
			}
		}
		if (pb->type == ASYNC_EVENT)
		{
			if (WAIT_OBJECT_0 != WaitForSingleObject(pov->hEvent, pb->timo))
			{
				*pcb = 0;
				return 0;
			}

			dw = 0;
			if (!WSAGetOverlappedResult((SOCKET)so, (LPWSAOVERLAPPED)pov, &dw, FALSE, &fd))
			{
				*pcb = 0;
				return 0;
			}
		}
	}

	if (pov) WSAResetEvent(pov->hEvent);

	if (pcb) *pcb = dw;

	return 1;
}

/*res_file_t _socket_accept(res_file_t so, res_addr_t saddr, int *plen)
{
SOCKET po;

po = accept((SOCKET)so, saddr, plen);

return (po == INVALID_SOCKET) ? INVALID_FILE : (res_file_t)po;
}*/

res_file_t _socket_accept(res_file_t ls, res_addr_t saddr, int *plen, async_t* pb)
{
	LPOVERLAPPED pov = (pb) ? (LPOVERLAPPED)pb->lapp : NULL;

	res_file_t so;

	LPFN_ACCEPTEX pf_acceptex = NULL;
	GUID id_acceptex = WSAID_ACCEPTEX;

	LPFN_GETACCEPTEXSOCKADDRS pf_acceptex_addr = NULL;
	GUID id_acceptex_addr = WSAID_GETACCEPTEXSOCKADDRS;

	DWORD fd, dw;
	ULONG_PTR up = NULL;
	LPOVERLAPPED ul = NULL;

	unsigned char buf[64] = { 0 };
	res_addr_t loc_addr = NULL;
	int loc_len = 0;
	res_addr_t rmt_addr = NULL;
	int rmt_len = 0;

	so = _socket_tcp(0, FILE_OPEN_OVERLAP);
	if (so == INVALID_FILE)
		return INVALID_FILE;

	WSAIoctl((SOCKET)so, SIO_GET_EXTENSION_FUNCTION_POINTER, &id_acceptex, sizeof(id_acceptex), &pf_acceptex, sizeof(pf_acceptex), &dw, NULL, NULL);
	if (!pf_acceptex)
	{
		closesocket((SOCKET)so);

		*plen = 0;
		return INVALID_FILE;
	}

	WSAIoctl((SOCKET)so, SIO_GET_EXTENSION_FUNCTION_POINTER, &id_acceptex_addr, sizeof(id_acceptex_addr), &pf_acceptex_addr, sizeof(pf_acceptex_addr), &dw, NULL, NULL);
	if (!pf_acceptex_addr)
	{
		closesocket((SOCKET)so);

		*plen = 0;
		return INVALID_FILE;
	}
	
	if (!pf_acceptex((SOCKET)ls, (SOCKET)so, buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dw, (LPOVERLAPPED)pov))
	{
		if (!pov)
		{
			closesocket((SOCKET)so);

			*plen = 0;
			return INVALID_FILE;
		}

		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			closesocket((SOCKET)so);

			*plen = 0;
			return INVALID_FILE;
		}

		if (pb->type == ASYNC_QUEUE)
		{
			if (!GetQueuedCompletionStatus((HANDLE)pb->port, &dw, &up, &ul, pb->timo))
			{
				closesocket((SOCKET)so);

				*plen = 0;
				return INVALID_FILE;
			}
		}
		if (pb->type == ASYNC_EVENT)
		{
			if (WAIT_OBJECT_0 != WaitForSingleObject(pov->hEvent, pb->timo))
			{
				closesocket((SOCKET)so);

				*plen = 0;
				return INVALID_FILE;
			}

			if (!WSAGetOverlappedResult((SOCKET)ls, (LPWSAOVERLAPPED)pov, &dw, FALSE, &fd))
			{
				closesocket((SOCKET)so);

				*plen = 0;
				return INVALID_FILE;
			}
		}
	}

	if(pov) ResetEvent(pov->hEvent);

	setsockopt((SOCKET)so, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&ls, sizeof(SOCKET));

	pf_acceptex_addr(buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &loc_addr, &loc_len, &rmt_addr, &rmt_len);
	*plen = rmt_len;
	if (*plen)
	{
		CopyMemory((void*)saddr, (void*)rmt_addr, rmt_len);
	}

	return so;
}

int _socket_type(res_file_t so)
{
	int type = 0;
	int len = sizeof(int);

	getsockopt((SOCKET)so, SOL_SOCKET, SO_TYPE, (char*)&type, &len);

	return type;
}

bool_t	_socket_setopt(res_file_t so, int optname, const char* optval, int optlen)
{
	return (setsockopt((SOCKET)so, SOL_SOCKET, optname, optval, optlen) == SOCKET_ERROR) ? 0 : 1;
}

bool_t	_socket_getopt(res_file_t so, int optname, char* pval, int* plen)
{
	return (getsockopt((SOCKET)so, SOL_SOCKET, optname, pval, plen) == SOCKET_ERROR) ? 0 : 1;
}

bool_t _socket_set_sndbuf(res_file_t so, int size)
{
	return (setsockopt((SOCKET)so, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(int)) == SOCKET_ERROR) ? 0 : 1;
}

bool_t _socket_set_rcvbuf(res_file_t so, int size)
{
	return (setsockopt((SOCKET)so, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(int)) == SOCKET_ERROR) ? 0 : 1;
}

bool_t _socket_set_linger(res_file_t so, bool_t wait, int sec)
{
	struct linger li;

	li.l_onoff = (wait) ? 1 : 0;
	li.l_linger = (unsigned short)sec;

	return (setsockopt((SOCKET)so, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(struct linger)) == SOCKET_ERROR) ? 0 : 1;
}

bool_t _socket_set_nonblk(res_file_t so, bool_t none)
{
	//set socket to blocking or none blocking mode
	unsigned long dw = (none)? 1 : 0;
	
	return (ioctlsocket((SOCKET)so, FIONBIO, &dw) == SOCKET_ERROR) ? 0 : 1;
}

bool_t _socket_get_nonblk(res_file_t so)
{
	return 0;
}

bool_t _socket_listen(res_file_t so, int max)
{
	return (listen((SOCKET)so, max) == 0) ? 1 : 0;
}

int _socket_write(void* pso, unsigned char* buf, int len)
{
	int rt;
	int pos = 0;

	while (pos < len)
	{
		rt = send(*((SOCKET*)pso), (char*)(buf + pos), len - pos, 0);
		if (rt == SOCKET_ERROR)
		{
			return (-1);
		}
		if (!rt) continue;

		pos += rt;
	}

	return pos;
}

int _socket_read(void* pso, unsigned char* buf, int len)
{
	int rt;
	int pos = 0;

	while (pos < len)
	{
		rt = recv(*((SOCKET*)pso), (char*)(buf + pos), len - pos, 0);
		if (rt == SOCKET_ERROR)
		{
			return (-1);
		}
		if (!rt) continue;

		pos += rt;
	}

	return pos;
}

bool_t _socket_share(pid_t procid, res_file_t procfd, res_file_t so, void* data, dword_t size)
{
	WSAPROTOCOL_INFO wi = { 0 };
	DWORD dw;
	BYTE num[4] = { 0 };

	dw = (DWORD)sizeof(wi);

	if (WSADuplicateSocket((SOCKET)so, procid, &wi))
	{
#ifdef _DEBUG
		dw = WSAGetLastError();
#endif
		return 0;
	}

	if (!WriteFile(procfd, (void*)&wi, dw, &dw, NULL))
	{
#ifdef _DEBUG
		dw = WSAGetLastError();
#endif
		return 0;
	}

	dw = (DWORD)size;
	num[0] = (unsigned char)(((dw) >> 24) & 0xFF);
	num[1] = (unsigned char)(((dw) >> 16) & 0xFF);
	num[2] = (unsigned char)(((dw) >> 8) & 0xFF);
	num[3] = (unsigned char)((dw)& 0xFF);

	dw = sizeof(DWORD);
	if (!WriteFile(procfd, (void*)num, dw, &dw, NULL))
	{
#ifdef _DEBUG
		dw = WSAGetLastError();
#endif
		return 0;
	}

	dw = (DWORD)size;
	if (!dw)
		return 1;

	if (!WriteFile(procfd, data, dw, &dw, NULL))
	{
		return 0;
	}

	return 1;
}

res_file_t _socket_dupli(res_file_t procfd, void* data, dword_t* pcb)
{
	WSAPROTOCOL_INFO wi = { 0 };
	DWORD dw;
	SOCKET so;
	BYTE num[4] = { 0 };
	void *buf = NULL;
	BOOL rt;

	dw = (DWORD)sizeof(wi);

	if (!ReadFile(procfd, (void*)&wi, dw, &dw, NULL))
	{
		return INVALID_FILE;
	}

	so = WSASocket(0, 0, 0, &wi, 0, 0);
	if (so == INVALID_SOCKET)
		return INVALID_FILE;

	dw = sizeof(DWORD);
	if (!ReadFile(procfd, (void*)num, dw, &dw, NULL))
	{
		if(pcb) *pcb = 0;

		return (res_file_t)so;
	}

	dw = (((unsigned int)(num[0]) << 24) & 0xFF000000) |
		(((unsigned int)(num[1]) << 16) & 0x00FF0000) |
		(((unsigned int)(num[2]) << 8) & 0x0000FF00) |
		((unsigned int)(num[3]) & 0x000000FF);

	if (!dw)
	{
		if (pcb) *pcb = 0;

		return (res_file_t)so;
	}

	buf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dw);

	rt = ReadFile(procfd, buf, dw, &dw, NULL);
	
	if (rt && data && pcb)
	{
		dw = (dw < (DWORD)(*pcb)) ? dw : (DWORD)(*pcb);
		CopyMemory(data, buf, dw);
	}
	else
	{
		dw = 0;
	}

	HeapFree(GetProcessHeap(), 0, buf);

	if (pcb) *pcb = dw;

	return (res_file_t)so;
}


/*res_file_t _socket_share(res_file_t so, dword_t procid, const tchar_t* sname)
{
	WSAPROTOCOL_INFO wi = { 0 };
	HANDLE mf;
	void* pb;
	DWORD dw;

	if (WSADuplicateSocket(so, procid, &wi))
	{
		dw = WSAGetLastError();
		return 0;
	}

	mf = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, sizeof(wi), sname);
	if (!mf)
		return 0;

	pb = MapViewOfFile(mf, FILE_MAP_WRITE, 0, 0, 0);
	if (!pb)
	{
		CloseHandle(mf);
		return 0;
	}

	CopyMemory(pb, (void*)&wi, sizeof(wi));
	UnmapViewOfFile(pb);

	return mf;
}

res_file_t _socket_dupli(const tchar_t* sname)
{
	WSAPROTOCOL_INFO wi = { 0 };
	HANDLE mf;
	void* pb;

	mf = OpenFileMapping(FILE_MAP_READ, 0, sname);
	if (!mf)
		return 0;

	pb = MapViewOfFile(mf, FILE_MAP_READ, 0, 0, 0);
	if (!pb)
	{
		CloseHandle(mf);
		return 0;
	}

	CopyMemory((void*)&wi, pb, sizeof(wi));
	UnmapViewOfFile(pb);
	CloseHandle(mf);

	return WSASocket(0, 0, 0, &wi, 0, 0);
}*/

#endif //XDK_SUPPORT_SOCK