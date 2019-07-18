﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl pnp document

	@module	netpnp.c | xdl pnp implement file

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

#include "netpnp.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_RAND)

typedef struct _pnp_t{
	xhand_head head;

	int type;
	res_file_t so;
	async_t ov;

	havege_state havs;

	unsigned short bind;
	unsigned short port;
	tchar_t addr[ADDR_LEN];

	byte_t* snd_pkg;
	dword_t snd_bys;

	byte_t* rcv_pkg;
	dword_t rcv_bys;
	dword_t rcv_ret;
}pnp_t;

/***********************************************************************************************/
static unsigned short _dynet_port(pnp_t* ppnp)
{
	unsigned short port;

	port = (unsigned short)havege_rand(&ppnp->havs);

	while (port < PNP_MIN_PORT)
	{
		port = (unsigned short)havege_rand(&ppnp->havs);
	}

	return port;
}

/*********************************************************************************************/

xhand_t xpnp_cli(unsigned short port, const tchar_t* addr)
{
	pnp_t* ppnp = NULL;
	int zo;
	net_addr_t sin = { 0 };

	ppnp = (pnp_t*)xmem_alloc(sizeof(pnp_t));
	ppnp->head.tag = _HANDLE_PNP;
	ppnp->type = _XPNP_TYPE_CLI;

	ppnp->so = INVALID_FILE;
	ppnp->port = port;
	xscpy(ppnp->addr, addr);

	havege_init(&ppnp->havs);
	ppnp->bind = _dynet_port(ppnp);

	ppnp->so = xsocket_udp(0, FILE_OPEN_OVERLAP);
	if (ppnp->so == INVALID_FILE)
	{
		xmem_free(ppnp);
		return NULL;
	}
	zo = 1;
	xsocket_setopt(ppnp->so, SO_REUSEADDR, (const char*)&zo, sizeof(int));

	fill_addr(&sin, ppnp->bind, NULL);
	if (!xsocket_bind(ppnp->so, (res_addr_t)&sin, sizeof(sin)))
	{
		xsocket_close(ppnp->so);
		xmem_free(ppnp);
		return NULL;
	}

	xsocket_set_rcvbuf(ppnp->so, PNP_PKG_SIZE);
	xsocket_set_sndbuf(ppnp->so, PNP_PKG_SIZE);

	async_alloc_lapp(&ppnp->ov, PNP_BASE_TIMO);

	if (ppnp->ov.type == ASYNC_BLOCK)
	{
		xsocket_set_nonblk(ppnp->so, 0);
	}

	ppnp->snd_pkg = (byte_t*)xmem_alloc(PNP_PKG_SIZE);
	ppnp->snd_bys = 0;

	ppnp->rcv_pkg = (byte_t*)xmem_alloc(PNP_PKG_SIZE);
	ppnp->rcv_bys = 0;
	ppnp->rcv_ret = 0;

	return (xhand_t)&(ppnp->head);
}

xhand_t xpnp_srv(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size)
{
	pnp_t* ppnp = NULL;
	int zo;
	net_addr_t sin = { 0 };

	if (size > PNP_PKG_SIZE)
	{
		set_last_error(_T("xpnp_srv"), _T("too large package size"), -1);
		return NULL;
	}

	ppnp = (pnp_t*)xmem_alloc(sizeof(pnp_t));
	ppnp->head.tag = _HANDLE_PNP;
	ppnp->type = _XPNP_TYPE_SRV;

	ppnp->so = INVALID_FILE;
	ppnp->port = port;
	xscpy(ppnp->addr, addr);

	havege_init(&ppnp->havs);
	ppnp->bind = _dynet_port(ppnp);

	ppnp->so = xsocket_udp(0, FILE_OPEN_OVERLAP);
	if (ppnp->so == INVALID_FILE)
	{
		xmem_free(ppnp);
		return NULL;
	}
	zo = 1;
	xsocket_setopt(ppnp->so, SO_REUSEADDR, (const char*)&zo, sizeof(int));

	fill_addr(&sin, ppnp->bind, NULL);
	if (!xsocket_bind(ppnp->so, (res_addr_t)&sin, sizeof(sin)))
	{
		xsocket_close(ppnp->so);
		xmem_free(ppnp);
		return NULL;
	}

	xsocket_set_rcvbuf(ppnp->so, PNP_PKG_SIZE);
	xsocket_set_sndbuf(ppnp->so, PNP_PKG_SIZE);

	async_alloc_lapp(&ppnp->ov, PNP_BASE_TIMO);

	ppnp->snd_pkg = (byte_t*)xmem_alloc(PNP_PKG_SIZE);
	ppnp->snd_bys = 0;

	ppnp->rcv_pkg = (byte_t*)xmem_alloc(PNP_PKG_SIZE);
	ppnp->rcv_bys = 0;
	ppnp->rcv_ret = 0;

	if (size)
	{
		xmem_copy((void*)(ppnp->rcv_pkg), (void*)pack, size);
		ppnp->rcv_bys = size;
		ppnp->rcv_ret = 0;
	}

	return (xhand_t)&ppnp->head;
}

void  xpnp_close(xhand_t pnp)
{
	pnp_t* ppnp = (pnp_t*)pnp;

	XDL_ASSERT(pnp && pnp->tag == _HANDLE_PNP);

	if (ppnp->rcv_pkg)
		xmem_free(ppnp->rcv_pkg);

	if (ppnp->snd_pkg)
		xmem_free(ppnp->snd_pkg);

	if (ppnp->so)
	{
		xthread_sleep(THREAD_BASE_TMO);
		xsocket_close(ppnp->so);
	}

	async_release_lapp(&ppnp->ov);

	xmem_free(ppnp);
}

int xpnp_type(xhand_t pnp)
{
	pnp_t* ppnp = (pnp_t*)pnp;

	XDL_ASSERT(pnp && pnp->tag == _HANDLE_PNP);

	return ppnp->type;
}

bool_t xpnp_write(xhand_t pnp, const byte_t* buf, dword_t* pb)
{
	pnp_t* ppnp = (pnp_t*)pnp;
	dword_t bys, dw;
	net_addr_t sin = { 0 };
	int addr_len;

	XDL_ASSERT(pnp && pnp->tag == _HANDLE_PNP);

	TRY_CATCH;

	dw = *pb;
	while (dw)
	{
		bys = (ppnp->snd_bys + dw > PNP_PDU_SIZE) ? (PNP_PDU_SIZE - ppnp->snd_bys) : dw;
		xmem_copy((void*)(ppnp->snd_pkg + ppnp->snd_bys), (void*)buf, bys);
		ppnp->snd_bys += bys;
		dw -= bys;

		if (ppnp->snd_bys == PNP_PDU_SIZE)
		{
			fill_addr(&sin, ppnp->port, ppnp->addr);
			addr_len = sizeof(net_addr_t);

			bys = ppnp->snd_bys;
			if (!xsocket_sendto(ppnp->so, (res_addr_t)&sin, addr_len, (void*)(ppnp->snd_pkg), bys, &ppnp->ov))
			{
				raise_user_error(NULL, NULL);
			}

			ppnp->snd_bys = bys - (dword_t)ppnp->ov.size;
			if (ppnp->snd_bys)
			{
				xmem_copy((void*)(ppnp->snd_pkg), (void*)(ppnp->snd_pkg + PNP_PDU_SIZE - ppnp->snd_bys), ppnp->snd_bys);
			}
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	*pb = 0;

	return 0;
}

bool_t xpnp_flush(xhand_t pnp)
{
	pnp_t* ppnp = (pnp_t*)pnp;
	dword_t dw;
	net_addr_t sin = { 0 };
	int addr_len;

	XDL_ASSERT(pnp && pnp->tag == _HANDLE_PNP);

	TRY_CATCH;

	if (ppnp->snd_bys)
	{
		fill_addr(&sin, ppnp->port, ppnp->addr);
		addr_len = sizeof(net_addr_t);

		dw = ppnp->snd_bys;
		if (!xsocket_sendto(ppnp->so, (res_addr_t)&sin, addr_len, (void*)(ppnp->snd_pkg), dw, &ppnp->ov))
		{
			raise_user_error(NULL, NULL);
		}

		dw = (dword_t)ppnp->ov.size;

		ppnp->snd_bys = 0;
	}

	END_CATCH;

	return 1;
ONERROR:

	return 0;
}

bool_t xpnp_read(xhand_t pnp, byte_t* buf, dword_t* pb)
{
	pnp_t* ppnp = (pnp_t*)pnp;
	dword_t bys, dw;
	net_addr_t na = { 0 };
	int addr_len;
	bool_t b = 0;

	XDL_ASSERT(pnp && pnp->tag == _HANDLE_PNP);

	TRY_CATCH;

	dw = *pb;
	*pb = 0;
	while (dw && !b)
	{
		if (!ppnp->rcv_bys)
		{
			bys = PNP_PDU_SIZE;
			addr_len = sizeof(net_addr_t);
			if (!xsocket_recvfrom(ppnp->so, (res_addr_t)&na, &addr_len, (void*)(ppnp->rcv_pkg), bys, &ppnp->ov))
			{
				raise_user_error(NULL, NULL);
			}
			conv_addr(&na, &ppnp->port, ppnp->addr);

			ppnp->rcv_bys = (dword_t)ppnp->ov.size;
			ppnp->rcv_ret = 0;

			if (ppnp->rcv_bys < PNP_PDU_SIZE)
			{
				b = 1;
			}
		}
		else
		{
			b = 1;
		}

		bys = (ppnp->rcv_bys < dw) ? ppnp->rcv_bys : dw;
		xmem_copy((void*)(buf + *pb), (void*)(ppnp->rcv_pkg + ppnp->rcv_ret), bys);

		ppnp->rcv_ret += bys;
		ppnp->rcv_bys -= bys;
		dw -= bys;
		*pb = *pb + bys;
	}

	END_CATCH;

	return 1;
ONERROR:

	*pb = 0;

	return 0;
}


unsigned short xpnp_addr_port(xhand_t pnp, tchar_t* addr)
{
	pnp_t* pso = (pnp_t*)pnp;
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(pnp && pnp->tag == _HANDLE_PNP);

	xsocket_addr(pso->so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

unsigned short xpnp_peer_port(xhand_t pnp, tchar_t* addr)
{
	pnp_t* pso = (pnp_t*)pnp;
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(pnp && pnp->tag == _HANDLE_PNP);

	xsocket_peer(pso->so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

#endif /*XDK_SUPPORT_SOCK*/
