/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc socket document

	@module	xdlsock.c | socket implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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

#include "xdlsock.h"
#include "xdlinit.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlthr.h"
#include "xdlmodu.h"
#include "xdlcode.h"
#include "xdlerr.h"
#include "xdlutil.h"
#include "xdlpipe.h"
#include "xdlcons.h"

#ifdef XDL_SUPPORT_SSL
#include "oemssl.h"
#endif

#ifdef XDL_SUPPOTR_HTTP

unsigned int __stdcall wait_accept(void* param);

unsigned int __stdcall thread_accept(void* param);
unsigned int __stdcall process_accept(void* param);

#define MAX_LISTEN		5
#define TRY_MAX			5
#define HEAP_MUL		10


typedef struct _socket_t{
	xhand_head head;		//reserved for xhand_t

	res_sock_t soc;
#ifdef XDL_SUPPORT_SSL
	ssl_sock_t ssl;
	bool_t secu;
#endif

	int type;
	net_addr_t addr;

	int act;
	xhand_t ev;

	void* pf_param;
	bool_t is_thread;
	union
	{
		PF_SOCKETACCEPT pf_accept;
		const tchar_t* sz_module;
	};
}socket_t;

typedef struct _xaccept_t{

	res_sock_t soc;
#ifdef XDL_SUPPORT_SSL
	bool_t secu;
#endif

	net_addr_t addr;
	xhand_t ev;

	void* pf_param;
	union
	{
		PF_SOCKETACCEPT pf_accept;
		const tchar_t* sz_module;
	};
}xaccept_t;

bool_t host_addr(const tchar_t* host,tchar_t* addr)
{
	if_socket_t* pif_so;
	bool_t b;
	schar_t taddr[MAX_ADDR + 1] = { 0 };
	schar_t thost[PATH_LEN + 1] = { 0 };

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

#ifdef _UNICODE
	ucs_to_mbs(host, -1, thost, PATH_LEN);
#else
	a_xsncpy(thost, host, PATH_LEN);
#endif

	b = (*pif_so->pf_host_addr)(thost, taddr);

#ifdef _UNICODE
	mbs_to_ucs(taddr, a_xslen(taddr), addr, MAX_ADDR);
#else
	a_xsncpy(addr, taddr, MAX_ADDR);
#endif

	return b;
}

void  xsocket_addr(xhand_t sok, tchar_t* addr)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;
	schar_t taddr[MAX_ADDR + 1] = { 0 };

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	(*pif_so->pf_socket_addr)(&pso->addr, taddr);

#ifdef _UNICODE
	mbs_to_ucs(taddr, a_xslen(taddr), addr, MAX_ADDR);
#else
	a_xsncpy(addr,taddr, MAX_ADDR);
#endif
}

void  xsocket_peer(xhand_t sok, tchar_t* addr)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;
	net_addr_t na = { 0 };
	schar_t taddr[MAX_ADDR + 1] = { 0 };

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	(*pif_so->pf_socket_peer)(pso->soc, &na);

	(*pif_so->pf_socket_addr)(&na, taddr);

#ifdef _UNICODE
	mbs_to_ucs(taddr, a_xslen(taddr), addr, MAX_ADDR);
#else
	a_xsncpy(addr,taddr, MAX_ADDR);
#endif
}

xhand_t xsocket_cli(bool_t secu, short port,const tchar_t* addr)
{
	socket_t* pso;
	net_addr_t sin;
	res_sock_t so;
	int n;
	bool_t rt;
	if_socket_t* pif_so;
	schar_t saddr[MAX_ADDR + 1] = { 0 };
	struct linger li = { 1,10 };

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	so = (*pif_so->pf_socket_open)(AF_INET,SOCK_STREAM,0);

	if (!so)
	{
		set_system_error(_T("pf_socket_open"));
		return NULL;
	}

#ifdef _UNICODE
	ucs_to_mbs(addr, -1, saddr, MAX_ADDR);
#else
	a_xsncpy(saddr, addr, MAX_ADDR);
#endif

	xmem_zero((void*)&sin, sizeof(sin));

	(*pif_so->pf_fill_addr)(&sin, AF_INET, port, saddr);
	
	n = TRY_MAX;
	rt = 0;
	while(n && !(*pif_so->pf_socket_connect)(so,(res_addr_t)&sin,sizeof(sin)))
	{
		n--;
#ifdef _DEBUG
		xthread_sleep(10);
#endif
	}

	if (!n)
	{
		set_system_error(_T("pf_socket_connect"));

		(*pif_so->pf_socket_close)(so);
		return NULL;
	}

	(*pif_so->pf_socket_setopt)(so, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(struct linger));

	pso = (socket_t*)xmem_alloc(sizeof(socket_t));
	pso->head.tag = _HANDLE_SOCKET;
	pso->head.cbs = sizeof(socket_t);

	pso->soc = so;
	pso->type = SOCKET_TYPE_CLIENT;
	xmem_copy((void*)&pso->addr,(void*)&sin,sizeof(net_addr_t));

#ifdef XDL_SUPPORT_SSL
	if (secu)
	{
		pso->ssl = xssl_alloc(pso->soc, pif_so->pf_socket_write, pif_so->pf_socket_read, 0);
		pso->secu = (pso->ssl) ? 1 : 0;
	}
#endif

	return (xhand_t)pso;
}

int xsocket_type(xhand_t sok)
{
	socket_t* pso = (socket_t*)sok;

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	return pso->type;
}

void xsocket_set_send_buff(xhand_t sok, dword_t dw)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;
	int zero = 0;
	int size = sizeof(int);

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (dw == SOCKET_ZERO_BUFF)
	{
		zero = 0;
	}
	else if (dw == SOCKET_BASE_BUFF)
	{
		zero = SOCKET_BASE_BUFF;
	}
	else if (dw < SOCKET_BASE_BUFF)
	{
		zero = dw / SOCKET_PAGE_BUFF;
		if (zero < 1)
			zero = 1;
		else if (zero > 16)
			zero = 16;

		zero *= SOCKET_PAGE_BUFF;
	}
	else
	{
		zero = dw / SOCKET_BASE_BUFF;

		if (zero < SOCKET_MIN_MULTI)
			zero = SOCKET_MIN_MULTI;
		else if (zero > SOCKET_MAX_MULTI)
			zero = SOCKET_MAX_MULTI;

		zero *= SOCKET_BASE_BUFF;
	}

	(*pif_so->pf_socket_setopt)(pso->soc, SOL_SOCKET, SO_SNDBUF, (const char*)&zero, sizeof(int));

	//(*pif_so->pf_socket_getopt)(pso->soc, SOL_SOCKET, SO_SNDBUF, (char*)&zero, &size);
}

void xsocket_set_recv_buff(xhand_t sok, dword_t dw)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;
	int zero = 0;
	int size = sizeof(int);

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	if (dw == SOCKET_ZERO_BUFF)
	{
		zero = 0;
	}
	else if (dw == SOCKET_BASE_BUFF)
	{
		zero = SOCKET_BASE_BUFF;
	}
	else if (dw < SOCKET_BASE_BUFF)
	{
		zero = dw / SOCKET_PAGE_BUFF;
		if (zero < 1)
			zero = 1;
		else if (zero > 16)
			zero = 16;

		zero *= SOCKET_PAGE_BUFF;
	}
	else
	{
		zero = dw / SOCKET_BASE_BUFF;

		if (zero < SOCKET_MIN_MULTI)
			zero = SOCKET_MIN_MULTI;
		else if (zero > SOCKET_MAX_MULTI)
			zero = SOCKET_MAX_MULTI;

		zero *= SOCKET_BASE_BUFF;
	}

	(*pif_so->pf_socket_setopt)(pso->soc, SOL_SOCKET, SO_RCVBUF, (const char*)&zero, sizeof(int));

	//(*pif_so->pf_socket_getopt)(pso->soc, SOL_SOCKET, SO_RCVBUF, (char*)&zero, &size);
}

void xsocket_set_send_timeout(xhand_t sok, int s)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;
#ifdef _OS_WINDOWS
	int zero = 0;
#else
	struct timeval tv = { 0 };
#endif

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

#ifdef _OS_WINDOWS
	zero = s * 1000;
	(*pif_so->pf_socket_setopt)(pso->soc, SOL_SOCKET, SO_SNDTIMEO, (const char*)&zero, sizeof(int));
#else
	tv.sec = s;
	(*pif_so->pf_socket_setopt)(pso->soc, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(struct timeval));
#endif
}

void xsocket_set_recv_timeout(xhand_t sok, int s)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;
#ifdef _OS_WINDOWS
	int zero = 0;
#else
	struct timeval tv = { 0 };
#endif

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

#ifdef _OS_WINDOWS
	zero = s * 1000;
	(*pif_so->pf_socket_setopt)(pso->soc, SOL_SOCKET, SO_RCVTIMEO, (const char*)&zero, sizeof(int));
#else
	tv.sec = s;
	(*pif_so->pf_socket_setopt)(pso->soc, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));
#endif
}

void xsocket_set_linger(xhand_t sok, bool_t b_wait, int n_sec)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;
	struct linger li;

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	li.l_onoff = (b_wait) ? 1 : 0;
	li.l_linger = (u_short)n_sec;

	(*pif_so->pf_socket_setopt)(pso->soc, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(struct linger));
}

void  xsocket_close(xhand_t sok)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	XDL_ASSERT(pso->type == SOCKET_TYPE_CLIENT || pso->type == SOCKET_TYPE_SERVER);

	if (pso->type == SOCKET_TYPE_CLIENT)
	{
		//disable recive
		(*pif_so->pf_socket_shutdown)(pso->soc, 0);
	}
	else if (pso->type == SOCKET_TYPE_SERVER)
	{
		//disable send
		(*pif_so->pf_socket_shutdown)(pso->soc, 1);
		(*pif_so->pf_socket_wait)(pso->soc, 3);
	}

#ifdef _DEBUG
	//wait data sended
	xthread_sleep(10);
#endif

	(*pif_so->pf_socket_close)(pso->soc);

#ifdef XDL_SUPPORT_SSL
	if (pso->secu)
	{
		xssl_free(pso->ssl);
	}
#endif

	xmem_free(pso);
}

bool_t xsocket_write(xhand_t sok, const byte_t* buf, dword_t* pb)
{
	socket_t* pso = (socket_t*)sok;

	if_socket_t* pif;

	dword_t size, pos = 0;

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

#ifdef XDL_SUPPORT_SSL
	if (pso->secu)
	{
		size = *pb;

		if (INTEGER_ERROR != xssl_write(pso->ssl, buf, size))
			return 1;

		*pb = 0;
		return 0;
	}
#endif

	pif = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pb;

	while (pos < size)
	{
		if (!(*pif->pf_socket_send)(pso->soc, (char*)(buf + pos), size - pos, pb, 0))
		{
			set_system_error(_T("pf_socket_send"));
			return 0;
		}

		if (!(*pb))
			break;

		pos += *pb;
	}

	*pb = pos;

	return 1;
}

bool_t xsocket_read(xhand_t sok, byte_t* buf, dword_t* pb)
{
	socket_t* pso = (socket_t*)sok;

	if_socket_t* pif;

	dword_t size, pos = 0;

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

#ifdef XDL_SUPPORT_SSL
	if (pso->secu)
	{
		size = *pb;

		if (INTEGER_ERROR != xssl_read(pso->ssl, buf, size))
			return 1;

		*pb = 0;
		return 0;
	}
#endif

	pif = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pb;

	while (pos < size)
	{
		if (!(*pif->pf_socket_recv)(pso->soc, (char*)(buf + pos), size - pos, pb, 0))
		{
			set_system_error(_T("pf_socket_recv"));
			return 0;
		}

		if (!(*pb))
			break;

		pos += *pb;
	}

	*pb = pos;

	return 1;
}

bool_t xsocket_peek(xhand_t sok, byte_t* buf, dword_t max)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;
	dword_t dw, pos = 0;

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	while (pos < max)
	{
		if (!(*pif_so->pf_socket_recv)(pso->soc, (char*)(buf + pos), max - pos, &dw, 2))
		{
			set_system_error(_T("pf_socket_recv"));
			return 0;
		}

		if (!dw)
			break;

		pos += dw;
	}

	return (pos == max)? 1 : 0;
}

#ifdef XDL_SUPPOTR_HTTP_SRV

dword_t xsocket_share(xhand_t sok, dword_t procid, byte_t* buf)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	return (*pif_so->pf_socket_share)(pso->soc, procid, buf);
}

dword_t xsocket_share_size()
{
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	return (*pif_so->pf_socket_share_size)();
}

xhand_t xsocket_dupli(const byte_t* pb, dword_t dw)
{
	socket_t* pso;
	res_sock_t so;

	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	so = (*pif_so->pf_socket_dupli)(pb, dw);
	if (!so)
	{
		set_system_error(_T("pf_socket_dupli"));
		return NULL;
	}

	pso = (socket_t*)xmem_alloc(sizeof(socket_t));
	pso->head.tag = _HANDLE_SOCKET;
	pso->head.cbs = sizeof(socket_t);

	pso->soc = so;
	pso->act = 1;
	pso->type = SOCKET_TYPE_SERVER;
	//xmem_copy((void*)&(pso->addr), (void*)&(pxa->addr), sizeof(net_addr_t));

	return (xhand_t)pso;
}


////////////////////////////////////////////////////////////////////////////////

socket_t*  _xsocket_srv(short port)
{
	socket_t* pso;
	net_addr_t sin;
	res_sock_t so;
	if_socket_t* pif_so;

	pif_so = PROCESS_SOCKET_INTERFACE;
	XDL_ASSERT(pif_so != NULL);

	so = (*pif_so->pf_socket_open)(AF_INET, SOCK_STREAM, 0);
	if ((int)so < 0)
		return NULL;

	xmem_zero((void*)&sin, sizeof(sin));

	(*pif_so->pf_fill_addr)(&sin, AF_INET, port, NULL);

	if (!(*pif_so->pf_socket_bind)(so, (res_addr_t)&sin, sizeof(sin)))
	{
		set_system_error(_T("pf_socket_bind"));

		(*pif_so->pf_socket_close)(so);
		return NULL; //bind sock error
	}

	if (!(*pif_so->pf_socket_listen)(so, SOMAXCONN))
	{
		set_system_error(_T("pf_socket_listen"));

		(*pif_so->pf_socket_close)(so);
		return NULL; //listen error
	}

	pso = (socket_t*)xmem_alloc(sizeof(socket_t));
	pso->head.tag = _HANDLE_SOCKET;
	pso->head.cbs = sizeof(socket_t);

	pso->soc = so;
	pso->type = SOCKET_TYPE_LISTEN;
	pso->act = 1;
	xmem_copy((void*)&pso->addr, (void*)&sin, sizeof(net_addr_t));
	pso->ev = xevent_create(NULL);

	return pso;
}

xhand_t  xsocket_thread_srv(bool_t secu, short port, PF_SOCKETACCEPT pf_accept, void* param)
{
	socket_t* pso;
	
	pso = _xsocket_srv(port);
	if (!pso)
		return NULL;

	pso->is_thread = 1;
	pso->pf_accept = pf_accept;
	pso->pf_param = param;
	pso->secu = secu;

	xthread_begin((PF_THREADFUNC)wait_accept, (void*)pso);

#ifdef _DEBUG
	xthread_sleep(10);
#endif

	return (xhand_t)pso;
}

xhand_t  xsocket_process_srv(bool_t secu, short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	socket_t* pso;

	pso = _xsocket_srv(port);
	if (!pso)
		return NULL;

	pso->is_thread = 0;
	pso->sz_module = sz_module;
	pso->pf_param = (void*)sz_cmdline;
	pso->secu = secu;

	xthread_begin((PF_THREADFUNC)wait_accept, (void*)pso);

#ifdef _DEBUG
	xthread_sleep(10);
#endif

	return (xhand_t)pso;
}

void  xsocket_stop(xhand_t sok)
{
	socket_t* pso = (socket_t*)sok;
	if_socket_t* pif_so;

	XDL_ASSERT(sok && sok->tag == _HANDLE_SOCKET);

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	XDL_ASSERT(pso->type == SOCKET_TYPE_LISTEN);

	//disiable recive and send
	(*pif_so->pf_socket_shutdown)(pso->soc, 2);

	//indicate listen to be stoping
	pso->act = 0;

	//wait listen stoped
	xthread_sleep(100);

	(*pif_so->pf_socket_close)(pso->soc);

	//wait listen stopped
	xevent_wait(pso->ev, -1);
	xevent_destroy(pso->ev);

	xmem_free(pso);
}

unsigned int __stdcall wait_accept(void* param)
{
	socket_t* pso = (socket_t*)param;
	socket_t* psn = NULL;
	res_sock_t so;
	net_addr_t tcpaddr;
	int addr_len;
	xaccept_t xa = { 0 };
	if_socket_t* pif_so;

	xdl_thread_init();

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	while(pso->act)
	{
		addr_len = sizeof(tcpaddr);
		so = (*pif_so->pf_socket_accept)(pso->soc, (res_addr_t)&tcpaddr, &addr_len);
		if((int)so > 0)
		{
			xa.soc = so;
			xa.pf_param = pso->pf_param;
			if (pso->is_thread)
				xa.pf_accept = pso->pf_accept;
			else
				xa.sz_module = pso->sz_module;

			xa.pf_param = pso->pf_param;
			xa.secu = pso->secu;

			xmem_copy((void*)&(xa.addr), (void*)&tcpaddr, sizeof(tcpaddr));

			xa.ev = xevent_create(NULL);

			if (pso->is_thread)
			{
				xthread_begin((PF_THREADFUNC)thread_accept, (void*)&xa);
			}
			else
			{
				xthread_begin((PF_THREADFUNC)process_accept, (void*)&xa);
			}

			xevent_wait(xa.ev, -1);
			xevent_destroy(xa.ev);

			xmem_zero((void*)&xa, sizeof(xaccept_t));
		}
	}

	xdl_thread_uninit(0);

	xevent_sign(pso->ev, 1);

	xthread_end();

	return 0;
}

unsigned int __stdcall thread_accept(void* param)
{
	xaccept_t* pxa = (xaccept_t*)param;
	socket_t* pso;
	if_socket_t* pif_so;
	struct linger li = { 1, 10 };

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	xdl_thread_init();
	
	pso = (socket_t*)xmem_alloc(sizeof(socket_t));
	pso->head.tag = _HANDLE_SOCKET;
	pso->head.cbs = sizeof(socket_t);

	pso->soc = pxa->soc;
	pso->pf_param = pxa->pf_param;
	pso->pf_accept = pxa->pf_accept;
	pso->act = 1;
	pso->type = SOCKET_TYPE_SERVER;
	pso->secu = pxa->secu;
	xmem_copy((void*)&(pso->addr), (void*)&(pxa->addr), sizeof(net_addr_t));

	xevent_sign(pxa->ev, 1);

	(*pif_so->pf_socket_setopt)(pso->soc, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(struct linger));

#ifdef XDL_SUPPORT_SSL
	if (pso->secu)
	{
		pso->ssl = xssl_alloc(pso->soc, pif_so->pf_socket_write, pif_so->pf_socket_read, 1);
		pso->secu = (pso->ssl) ? 1 : 0;
	}
#endif

	(*(pso->pf_accept))((xhand_t)pso, pso->pf_param);

	//disable send
	(*pif_so->pf_socket_shutdown)(pso->soc, 1);
	(*pif_so->pf_socket_wait)(pso->soc, 3);

#ifdef _DEBUG
	//wait data sended
	xthread_sleep(10);
#endif

	(*pif_so->pf_socket_close)(pso->soc);

#ifdef XDL_SUPPORT_SSL
	if (pso->secu)
	{
		xssl_free(pso->ssl);
	}
#endif

	xmem_free(pso);

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

unsigned int __stdcall process_accept(void* param)
{
	xaccept_t* pxa = (xaccept_t*)param;
	socket_t* pso;
	if_socket_t* pif_so;

	proc_info_t pi = { 0 };
	byte_t* sz_buf = NULL;
	dword_t dw;
	xhand_t pip = NULL;

	pif_so = PROCESS_SOCKET_INTERFACE;

	XDL_ASSERT(pif_so != NULL);

	xdl_thread_init();

	pso = (socket_t*)xmem_alloc(sizeof(socket_t));
	pso->head.tag = _HANDLE_SOCKET;
	pso->head.cbs = sizeof(socket_t);

	pso->soc = pxa->soc;
	pso->act = 1;
	pso->type = SOCKET_TYPE_SERVER;
	pso->sz_module = pxa->sz_module;
	pso->pf_param = pxa->pf_param;
	xmem_copy((void*)&(pso->addr), (void*)&(pxa->addr), sizeof(net_addr_t));

	xevent_sign(pxa->ev, 1);

	if (create_process(pso->sz_module, (tchar_t*)pso->pf_param, 1, &pi))
	{
		dw = xsocket_share_size();
		sz_buf = (byte_t*)xmem_alloc(dw);

		dw = xsocket_share((xhand_t)pso, pi.process_id, sz_buf);

		if (pi.std_in_write)
		{
			pip = xpipe_attach(NULL, pi.std_in_write);

			if (pip)
			{
				xpipe_write(pip, sz_buf, &dw);

				xpipe_detach(pip, NULL, NULL);
				pip = NULL;
			}
		}

		if (pi.std_in_read)
			release_handle(pi.std_in_read);

		if (pi.std_in_write)
			release_handle(pi.std_in_write);

		xmem_free(sz_buf);
		sz_buf = NULL;

		if (pi.std_out_read)
			release_handle(pi.std_out_read);

		if (pi.std_out_write)
			release_handle(pi.std_out_write);

		release_handle(pi.process_handle);
		release_handle(pi.thread_handle);

#ifdef _DEBUG
		//wait process run
		xthread_sleep(10);
#endif

		(*pif_so->pf_socket_close)(pso->soc);
	}
	else
	{
		//disable recive
		(*pif_so->pf_socket_shutdown)(pso->soc, 0);
		(*pif_so->pf_socket_close)(pso->soc);
	}

	xmem_free(pso);

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

#endif //XDL_SUPPOTR_HTTP_SRV

#endif //XDL_SUPPOTR_HTTP
