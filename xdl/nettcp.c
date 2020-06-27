/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tcp document

	@module	nettcp.c | implement file

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

#include "xdlstd.h"

#ifdef XDK_SUPPORT_SOCK

#define MAX_LISTEN		5
#define TRY_MAX			3

typedef struct _tcp_t{
	xhand_head head;		//reserved for xhand_t

	int type;
	res_file_t so;

	async_t* pov;
}tcp_t;

/****************************************************************************************************/

xhand_t xtcp_cli(unsigned short port, const tchar_t* addr)
{
	net_addr_t sin;
	struct linger li = { 1,10 };

	tcp_t* pso = NULL;
	res_file_t so = 0;
    int n;

	TRY_CATCH;

	so = socket_tcp(0, 0);
	if (so == INVALID_FILE)
	{
		raise_user_error(NULL, NULL);
	}

	xmem_zero((void*)&sin, sizeof(sin));

	fill_addr(&sin, port, addr);
	
    n = TRY_MAX;
	while(n && !socket_connect(so,(res_addr_t)&sin,sizeof(sin)))
	{
        n--;
	}
    
    if(!n)
    {
        raise_user_error(NULL, NULL);
    }

	socket_setopt(so, SO_LINGER, (const char*)&li, sizeof(struct linger));

	pso = (tcp_t*)xmem_alloc(sizeof(tcp_t));
	pso->head.tag = _HANDLE_TCP;

	pso->so = so;
	pso->type = _XTCP_TYPE_CLI;

#if defined(_DEBUG) || defined(DEBUG)
	pso->pov = async_alloc_lapp(ASYNC_BLOCK, -1, INVALID_FILE);
#else
	pso->pov = async_alloc_lapp(ASYNC_BLOCK, TCP_BASE_TIMO, INVALID_FILE);
#endif

	END_CATCH;

	return &pso->head;
ONERROR:

	if (so)
		socket_close(so);

	if (pso)
	{
		if (pso->pov)
			async_free_lapp(pso->pov);

		xmem_free(pso);
	}

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

	socket_setopt(pso->so, SO_LINGER, (const char*)&li, sizeof(struct linger));
	socket_setopt(pso->so, SO_SNDBUF, (const char*)&zo, sizeof(int));
	socket_setopt(pso->so, SO_RCVBUF, (const char*)&zo, sizeof(int));

#if defined(_DEBUG) || defined(DEBUG)
	pso->pov = async_alloc_lapp(ASYNC_BLOCK, -1, INVALID_FILE);
#else
	pso->pov = async_alloc_lapp(ASYNC_EVENT, TCP_BASE_TIMO, INVALID_FILE);
#endif

	END_CATCH;

	return &pso->head;
ONERROR:

	if (pso)
	{
		if (pso->pov)
			async_free_lapp(pso->pov);

		xmem_free(pso);
	}

	XDL_TRACE_LAST;

	return NULL;
}

void  xtcp_close(xhand_t tcp)
{
	tcp_t* pso = TypePtrFromHead(tcp_t, tcp);

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	XDL_ASSERT(pso->type == _XTCP_TYPE_CLI || pso->type == _XTCP_TYPE_SRV);

	if (pso->type == _XTCP_TYPE_CLI)
	{
		//disable recive
		socket_shutdown(pso->so, 0);
	}
	else if (pso->type == _XTCP_TYPE_SRV)
	{
		//disable send
		socket_shutdown(pso->so, 1);
	}

	if (pso->type == _XTCP_TYPE_CLI)
	{
		socket_close(pso->so);
	}

	if (pso->pov)
	{
		async_free_lapp(pso->pov);
	}

	xmem_free(pso);
}

res_file_t xtcp_socket(xhand_t tcp)
{
	tcp_t* pso = TypePtrFromHead(tcp_t, tcp);

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	return pso->so;
}

int xtcp_type(xhand_t tcp)
{
	tcp_t* pso = TypePtrFromHead(tcp_t, tcp);

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	return pso->type;
}

bool_t xtcp_write(xhand_t tcp, const byte_t* buf, dword_t* pcb)
{
	tcp_t* pso = TypePtrFromHead(tcp_t, tcp);
	dword_t size;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	size = *pcb;

	if (!socket_send(pso->so, (void*)buf, size, pso->pov))
	{
		*pcb = 0;
		return 0;
	}

	*pcb = (dword_t)(pso->pov->size);

	return 1;
}

bool_t xtcp_read(xhand_t tcp, byte_t* buf, dword_t* pcb)
{
	tcp_t* pso = TypePtrFromHead(tcp_t, tcp);
	dword_t size;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	size = *pcb;

	if (!socket_recv(pso->so, buf, size, pso->pov))
	{
		*pcb = 0;
		return 0;
	}

	*pcb = (dword_t)(pso->pov->size);

	return 1;
}

unsigned short xtcp_addr_port(xhand_t tcp, tchar_t* addr)
{
	tcp_t* pso = TypePtrFromHead(tcp_t, tcp);
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	socket_addr(pso->so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

unsigned short xtcp_peer_port(xhand_t tcp, tchar_t* addr)
{
	tcp_t* pso = TypePtrFromHead(tcp_t, tcp);
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	socket_peer(pso->so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

bool_t xtcp_setopt(xhand_t tcp, int oid, void* opt, int len)
{
	tcp_t* ptcp = TypePtrFromHead(tcp_t, tcp);

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	switch (oid)
	{
	case SOCK_OPTION_SNDBUF:
		socket_set_sndbuf(xtcp_socket(tcp), *(int*)opt);
		return 1;
	case SOCK_OPTION_RCVBUF:
		socket_set_rcvbuf(xtcp_socket(tcp), *(int*)opt);
		return 1;
	case SOCK_OPTION_NONBLK:
		socket_set_nonblk(xtcp_socket(tcp), *(bool_t*)opt);
		return 1;
	}

	return 0;
}

void xtcp_settmo(xhand_t tcp, dword_t tmo)
{
	tcp_t* ptcp = TypePtrFromHead(tcp_t, tcp);

	XDL_ASSERT(tcp && tcp->tag == _HANDLE_TCP);

	if (ptcp->pov)
	{
		ptcp->pov->timo = tmo;
	}
}

#endif //XDK_SUPPORT_SOCK
