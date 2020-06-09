/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl udp document

	@module	netudp.c | implement file

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

#include "netudp.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_RAND)

typedef struct _udp_t{
	xhand_head head;		//head for xhand_t

	int type;
	res_file_t so;
	async_t* pov;

	unsigned short port;
	tchar_t addr[ADDR_LEN];

	dword_t pkg_size;

	byte_t* snd_pdu;
	dword_t snd_bys;

	byte_t* rcv_pdu;
	dword_t rcv_bys;
	dword_t rcv_ret;
}udp_t;

/*********************************************************************************************/

xhand_t xudp_cli(unsigned short port, const tchar_t* addr)
{
	udp_t* pudp = NULL;
	int zo;

	pudp = (udp_t*)xmem_alloc(sizeof(udp_t));
	pudp->head.tag = _HANDLE_UDP;
	pudp->type = _XUDP_TYPE_CLI;

	pudp->so = INVALID_FILE;
	pudp->port = port;
	xscpy(pudp->addr, addr);

	pudp->pkg_size = UDP_PDU_SIZE;

	pudp->so = socket_udp(0, FILE_OPEN_OVERLAP);
	if (pudp->so == INVALID_FILE)
	{
		xmem_free(pudp);
		return NULL;
	}
	zo = 1;
	socket_setopt(pudp->so, SO_REUSEADDR, (const char*)&zo, sizeof(int));

	pudp->pov = async_alloc_lapp(ASYNC_EVENT, UDP_BASE_TIMO, INVALID_FILE);

	pudp->snd_pdu = (byte_t*)xmem_alloc(pudp->pkg_size);
	pudp->snd_bys = 0;

	pudp->rcv_pdu = (byte_t*)xmem_alloc(pudp->pkg_size);
	pudp->rcv_bys = 0;
	pudp->rcv_ret = 0;

	return &(pudp->head);
}

xhand_t xudp_srv(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size)
{
	udp_t* pudp = NULL;
	int zo;

	if (size > MTU_MAX_SIZE)
	{
		set_last_error(_T("xudp_srv"), _T("too large package size"), -1);
		return NULL;
	}

	pudp = (udp_t*)xmem_alloc(sizeof(udp_t));
	pudp->head.tag = _HANDLE_UDP;
	pudp->type = _XUDP_TYPE_SRV;

	pudp->so = INVALID_FILE;
	pudp->port = port;
	xscpy(pudp->addr, addr);

	pudp->pkg_size = (size > UDP_PDU_SIZE)? size : UDP_PDU_SIZE;

	pudp->so = socket_udp(0, FILE_OPEN_OVERLAP);
	if (pudp->so == INVALID_FILE)
	{
		xmem_free(pudp);
		return NULL;
	}
	zo = 1;
	socket_setopt(pudp->so, SO_REUSEADDR, (const char*)&zo, sizeof(int));

	pudp->pov = async_alloc_lapp(ASYNC_EVENT, UDP_BASE_TIMO, INVALID_FILE);

	pudp->snd_pdu = (byte_t*)xmem_alloc(pudp->pkg_size);
	pudp->snd_bys = 0;

	pudp->rcv_pdu = (byte_t*)xmem_alloc(pudp->pkg_size);
	pudp->rcv_bys = 0;
	pudp->rcv_ret = 0;

	if (size)
	{
		xmem_copy((void*)(pudp->rcv_pdu), (void*)pack, size);
		pudp->rcv_bys = size;
		pudp->rcv_ret = 0;
	}

	return &pudp->head;
}

bool_t  xudp_bind(xhand_t udp, unsigned short bind)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);
	net_addr_t sin = { 0 };

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	fill_addr(&sin, bind, NULL);

	return socket_bind(pudp->so, (res_addr_t)&sin, sizeof(sin));
}

void  xudp_close(xhand_t udp)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	if (pudp->rcv_pdu)
		xmem_free(pudp->rcv_pdu);

	if (pudp->snd_pdu)
		xmem_free(pudp->snd_pdu);

	if (pudp->so)
	{
		socket_close(pudp->so);
	}

	if (pudp->pov)
	{
		async_free_lapp(pudp->pov);
	}

	xmem_free(pudp);
}

int xudp_type(xhand_t udp)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	return pudp->type;
}

res_file_t xudp_socket(xhand_t udp)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	return pudp->so;
}

void xudp_set_package(xhand_t udp, dword_t size)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	pudp->pkg_size = (size > UDP_PDU_SIZE) ? size : UDP_PDU_SIZE;

	pudp->snd_pdu = (byte_t*)xmem_realloc(pudp->snd_pdu, pudp->pkg_size);

	pudp->rcv_pdu = (byte_t*)xmem_realloc(pudp->rcv_pdu, pudp->pkg_size);

	socket_set_rcvbuf(pudp->so, pudp->pkg_size);
	socket_set_sndbuf(pudp->so, pudp->pkg_size);
}

dword_t xudp_get_package(xhand_t udp)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	return pudp->pkg_size;
}

bool_t xudp_write(xhand_t udp, const byte_t* buf, dword_t* pb)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);
	dword_t bys, dw;
	net_addr_t sin = { 0 };
	int addr_len;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	TRY_CATCH;

	dw = *pb;
	while (dw)
	{
		bys = (pudp->snd_bys + dw > pudp->pkg_size) ? (pudp->pkg_size - pudp->snd_bys) : dw;
		xmem_copy((void*)(pudp->snd_pdu + pudp->snd_bys), (void*)buf, bys);
		pudp->snd_bys += bys;
		dw -= bys;

		if (pudp->snd_bys == pudp->pkg_size)
		{
			fill_addr(&sin, pudp->port, pudp->addr);
			addr_len = sizeof(net_addr_t);

			bys = pudp->snd_bys;
			if (!socket_sendto(pudp->so, (res_addr_t)&sin, addr_len, (void*)(pudp->snd_pdu), bys, pudp->pov))
			{
				raise_user_error(NULL, NULL);
			}

			pudp->snd_bys = bys - (dword_t)(pudp->pov->size);
			if (pudp->snd_bys)
			{
				xmem_copy((void*)(pudp->snd_pdu), (void*)(pudp->snd_pdu + pudp->pkg_size - pudp->snd_bys), pudp->snd_bys);
			}
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	*pb = 0;

	return 0;
}

bool_t xudp_flush(xhand_t udp)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);
	dword_t dw;
	net_addr_t sin = { 0 };
	int addr_len;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	TRY_CATCH;

	if (pudp->snd_bys)
	{
		fill_addr(&sin, pudp->port, pudp->addr);
		addr_len = sizeof(net_addr_t);

		dw = pudp->snd_bys;
		pudp->pov->size = 0;
		if (!socket_sendto(pudp->so, (res_addr_t)&sin, addr_len, (void*)(pudp->snd_pdu), dw, pudp->pov))
		{
			raise_user_error(NULL, NULL);
		}

		dw = (dword_t)(pudp->pov->size);

		pudp->snd_bys = 0;
	}

	END_CATCH;

	return 1;
ONERROR:

	return 0;
}

bool_t xudp_read(xhand_t udp, byte_t* buf, dword_t* pb)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);
	dword_t bys, dw;
	net_addr_t na = { 0 };
	int addr_len;
	bool_t b = 0;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	TRY_CATCH;

	dw = *pb;
	*pb = 0;
	while (dw && !b)
	{
		if (!pudp->rcv_bys)
		{
			bys = pudp->pkg_size;
			addr_len = sizeof(net_addr_t);
			pudp->pov->size = 0;
			if (!socket_recvfrom(pudp->so, (res_addr_t)&na, &addr_len, (void*)(pudp->rcv_pdu), bys, pudp->pov))
			{
				if (!(pudp->pov->size))
				{
					raise_user_error(NULL, NULL);
				}
				else
				{
					b = 1;
				}
			}
			conv_addr(&na, &pudp->port, pudp->addr);

			pudp->rcv_bys = (dword_t)(pudp->pov->size);
			pudp->rcv_ret = 0;

			if (pudp->rcv_bys < pudp->pkg_size)
			{
				b = 1;
			}
		}
		else
		{
			b = 1;
		}

		bys = (pudp->rcv_bys < dw) ? pudp->rcv_bys : dw;
		xmem_copy((void*)(buf + *pb), (void*)(pudp->rcv_pdu + pudp->rcv_ret), bys);

		pudp->rcv_ret += bys;
		pudp->rcv_bys -= bys;
		dw -= bys;
		*pb = *pb + bys;
	}

	END_CATCH;

	return 1;
ONERROR:

	*pb = 0;

	return 0;
}

unsigned short xudp_addr_port(xhand_t udp, tchar_t* addr)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	socket_addr(pudp->so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

unsigned short xudp_peer_port(xhand_t udp, tchar_t* addr)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	socket_peer(pudp->so, &na);
	conv_addr(&na, &port, addr);

	return port;
}

bool_t xudp_setopt(xhand_t udp, int oid, void* opt, int len)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	switch (oid)
	{
	case SOCK_OPTION_SNDBUF:
		socket_set_sndbuf(pudp->so, *(int*)opt);
		return 1;
	case SOCK_OPTION_RCVBUF:
		socket_set_rcvbuf(pudp->so, *(int*)opt);
		return 1;
	case SOCK_OPTION_NONBLK:
		socket_set_nonblk(pudp->so, *(bool_t*)opt);
		return 1;
	}

	return 0;
}

void xudp_settmo(xhand_t udp, dword_t tmo)
{
	udp_t* pudp = TypePtrFromHead(udp_t, udp);

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	if (pudp->pov)
	{
		pudp->pov->timo = tmo;
	}
}


#endif /*XDK_SUPPORT_SOCK*/
