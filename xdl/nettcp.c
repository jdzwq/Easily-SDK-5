/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tcp document

	@module	nettcp.c | tcp implement file

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

#include "nettcp.h"
#include "xdlinit.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDK_SUPPORT_SOCK

#define MAX_LISTEN		5
#define TRY_MAX			3

typedef struct _tcp_t{
	xhand_head head;		//reserved for xhand_t

	int type;
	res_file_t so;
	async_t ov;
}tcp_t;

/****************************************************************************************************/

xhand_t xtcp_cli(unsigned short port, const tchar_t* addr)
{
	net_addr_t sin;
	int n;
	bool_t rt;
	struct linger li = { 1,10 };

	tcp_t* pso = NULL;
	res_file_t so = 0;

	TRY_CATCH;

	so = xsocket_tcp(0, 0);
	if (so == INVALID_FILE)
	{
		raise_user_error(NULL, NULL);
	}

	xmem_zero((void*)&sin, sizeof(sin));

	fill_addr(&sin, port, addr);
	
	n = TRY_MAX;
	rt = 0;
	while(n && !xsocket_connect(so,(res_addr_t)&sin,sizeof(sin)))
	{
		n--;
#if defined(_DEBUG) && defined(XDK_SUPPORT_THREAD)
		xthread_sleep(10);
#endif
	}

	if (!n)
	{
		raise_user_error(NULL, NULL);
	}

	xsocket_setopt(so, SO_LINGER, (const char*)&li, sizeof(struct linger));

	pso = (tcp_t*)xmem_alloc(sizeof(tcp_t));
	pso->head.tag = _HANDLE_TCP;

	pso->so = so;
	pso->type = _XTCP_TYPE_CLI;

	END_CATCH;

	return (xhand_t)pso;
ONERROR:

	if (so)
		xsocket_close(so);

	if (pso)
		xmem_free(pso);

	XDL_TRACE_LAST;

	return NULL;
}

xhand_t xtcp_srv(res_file_t so)
{
	struct linger li = { 1, 10 };
	int zo = 0;

	tcp_t* pso = NULL;

	TRY_CATCH;

	if (so == INVALID_FILE)
	{
		raise_user_error(_T("xssl_srv"), _T("invalid socket handle"));
	}

	pso = (tcp_t*)xmem_alloc(sizeof(tcp_t));
	pso->head.tag = _HANDLE_TCP;

	pso->so = so;
	pso->type = _XTCP_TYPE_SRV;

	xsocket_setopt(pso->so, SO_LINGER, (const char*)&li, sizeof(struct linger));
	xsocket_setopt(pso->so, SO_SNDBUF, (const char*)&zo, sizeof(int));
	xsocket_setopt(pso->so, SO_RCVBUF, (const char*)&zo, sizeof(int));

	async_alloc_lapp(&pso->ov, TCP_BASE_TIMO);

	if (pso->ov.type == ASYNC_BLOCK)
	{
		xsocket_set_nonblk(pso->so, 0);
	}

	END_CATCH;

	return (xhand_t)pso;
ONERROR:

	if (pso)
		async_release_lapp(&pso->ov);

	if (pso)
		xmem_free(pso);

	XDL_TRACE_LAST;

	return NULL;
}

void  xtcp_close(xhand_t tcp)
{
	tcp_t* pso = (tcp_t*)tcp;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	XDL_ASSERT(pso->type == _XTCP_TYPE_CLI || pso->type == _XTCP_TYPE_SRV);

	if (pso->type == _XTCP_TYPE_CLI)
	{
		//disable recive
		xsocket_shutdown(pso->so, 0);
	}
	else if (pso->type == _XTCP_TYPE_SRV)
	{
		//disable send
		xsocket_shutdown(pso->so, 1);
		xsocket_wait(pso->so, FD_READ, TCP_BASE_TIMO);
	}

#if defined(_DEBUG) && defined(XDK_SUPPORT_THREAD)
	//wait data sended
	xthread_sleep(THREAD_BASE_TMO);
#endif

	if (pso->type == _XTCP_TYPE_CLI)
	{
		xsocket_close(pso->so);
	}

	async_release_lapp(&pso->ov);

	xmem_free(pso);
}

res_file_t xtcp_socket(xhand_t tcp)
{
	tcp_t* pso = (tcp_t*)tcp;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	return pso->so;
}

int xtcp_type(xhand_t tcp)
{
	tcp_t* pso = (tcp_t*)tcp;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	return pso->type;
}

void xtcp_set_send_buff(xhand_t tcp, dword_t dw)
{
	tcp_t* pso = (tcp_t*)tcp;

	int zero = 0;
	int size = sizeof(int);

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	if (dw == TCP_ZERO_BUFF)
	{
		zero = 0;
	}
	else if (dw < TCP_MAX_BUFF)
	{
		zero = dw / TCP_MTU_BUFF;
		if (zero < 1)
			zero = 1;
		else if (zero > 32)
			zero = 32;

		zero *= TCP_MTU_BUFF;
	}
	else
	{
		zero = TCP_MAX_BUFF - 1;
	}

	xsocket_set_sndbuf(pso->so, zero);
}

void xtcp_set_recv_buff(xhand_t tcp, dword_t dw)
{
	tcp_t* pso = (tcp_t*)tcp;

	int zero = 0;
	int size = sizeof(int);

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	if (dw == TCP_ZERO_BUFF)
	{
		zero = 0;
	}
	else if (dw < TCP_MAX_BUFF)
	{
		zero = dw / TCP_MTU_BUFF;
		if (zero < 1)
			zero = 1;
		else if (zero > 32)
			zero = 32;

		zero *= TCP_MTU_BUFF;
	}
	else
	{
		zero = TCP_MAX_BUFF - 1;
	}

	xsocket_set_rcvbuf(pso->so, zero);
}

void xtcp_set_linger(xhand_t tcp, bool_t b_wait, int n_sec)
{
	tcp_t* pso = (tcp_t*)tcp;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	xsocket_set_linger(pso->so, b_wait, n_sec);
}

bool_t xtcp_write(xhand_t tcp, const byte_t* buf, dword_t* pcb)
{
	tcp_t* pso = (tcp_t*)tcp;
	dword_t size;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	size = *pcb;

	if (!xsocket_send(pso->so, (void*)buf, size, &pso->ov))
	{
		*pcb = 0;
		return 0;
	}

	*pcb = (dword_t)pso->ov.size;

	return 1;
}

bool_t xtcp_read(xhand_t tcp, byte_t* buf, dword_t* pcb)
{
	tcp_t* pso = (tcp_t*)tcp;
	dword_t size;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	size = *pcb;

	if (!xsocket_recv(pso->so, buf, size, &pso->ov))
	{
		*pcb = 0;
		return 0;
	}

	*pcb = (dword_t)pso->ov.size;

	return 1;
}

void xtcp_set_option(xhand_t tcp, int oid, void* opt, int len)
{
	switch (oid)
	{
	case SOCKET_OPTION_SNDBUF:
		xsocket_set_sndbuf(xtcp_socket(tcp), *(int*)opt);
		break;
	case SOCKET_OPTION_RCVBUF:
		xsocket_set_rcvbuf(xtcp_socket(tcp), *(int*)opt);
		break;
	case SOCKET_OPTION_NONBLK:
		xsocket_set_nonblk(xtcp_socket(tcp), *(bool_t*)opt);
		break;
	}
}

unsigned short xtcp_addr_port(xhand_t tcp, tchar_t* addr)
{
	tcp_t* pso = (tcp_t*)tcp;
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	xsocket_addr(pso->so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

unsigned short xtcp_peer_port(xhand_t tcp, tchar_t* addr)
{
	tcp_t* pso = (tcp_t*)tcp;
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	xsocket_peer(pso->so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

#endif //XDK_SUPPORT_SOCK
