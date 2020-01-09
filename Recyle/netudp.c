/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc udp document

	@module	netudp.c | udp implement file

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

#ifdef XDK_SUPPORT_SOCK

typedef struct _udp_t{
	xhand_head head;		//reserved for xhand_t

	int type;
	res_file_t so;
	async_t ov;
	
	PDU* snd_pdu;
	PDU* rcv_pdu;
	dword_t snd_bys;
	dword_t rcv_bys;
	dword_t rcv_ret;
}udp_t;

/**************************************************************************************************************/
xhand_t xudp_cli(unsigned short bind, unsigned short port,const tchar_t* addr)
{
	net_addr_t sin = { 0 };
	struct linger li = { 1,10 };
	int zo ;

	udp_t* pso = NULL;
	res_file_t so = 0;

	TRY_CATCH;

	so = xsocket_udp(0, 0);
	if (so == INVALID_FILE)
	{
		raise_user_error(NULL, NULL);
	}
	
	zo = 1;
	xsocket_setopt(so, SOL_SOCKET, SO_REUSEADDR, (const char*)&zo, sizeof(int));

	if (bind)
	{
		fill_addr(&sin, bind, NULL);
		if (!xsocket_bind(so, (res_addr_t)&sin, sizeof(sin)))
		{
			raise_user_error(NULL, NULL);
		}
	}

	if (port && !is_null(addr))
	{
		xmem_zero((void*)&sin, sizeof(sin));
		fill_addr(&sin, port, addr);
		if (!xsocket_connect(so, (res_addr_t)&sin, sizeof(sin)))
		{
			raise_user_error(NULL, NULL);
		}
	}

	zo = UDP_SEG_SIZE;
	xsocket_setopt(so, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(struct linger));
	xsocket_setopt(so, SOL_SOCKET, SO_SNDBUF, (const char*)&zo, sizeof(int));
	xsocket_setopt(so, SOL_SOCKET, SO_RCVBUF, (const char*)&zo, sizeof(int));

	pso = (udp_t*)xmem_alloc(sizeof(udp_t));
	pso->head.tag = _HANDLE_UDP;

	pso->type = _XUDP_TYPE_CLI;
	pso->so = so;

	pso->snd_pdu = pdu_alloc(PDU_TYPE_UDP);
	pso->snd_bys = 0;
	pso->rcv_pdu = pdu_alloc(PDU_TYPE_UDP);
	pso->rcv_bys = 0;

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

xhand_t xudp_srv(unsigned short bind, unsigned short port, const tchar_t* addr, const byte_t* pack)
{
	net_addr_t sin = { 0 };
	struct linger li = { 1, 10 };
	int zo;

	udp_t* pso = NULL;
	res_file_t so = 0;

	TRY_CATCH;

	so = xsocket_udp(0, FILE_OPEN_OVERLAP);
	if (so == INVALID_FILE)
	{
		raise_user_error(NULL, NULL);
	}

	zo = 1;
	xsocket_setopt(so, SOL_SOCKET, SO_REUSEADDR, (const char*)&zo, sizeof(int));

	if (bind)
	{
		fill_addr(&sin, bind, NULL);
		if (!xsocket_bind(so, (res_addr_t)&sin, sizeof(sin)))
		{
			raise_user_error(NULL, NULL);
		}
	}

	if (port)
	{
		xmem_zero((void*)&sin, sizeof(sin));
		fill_addr(&sin, port, addr);
		if (!xsocket_connect(so, (res_addr_t)&sin, sizeof(sin)))
		{
			raise_user_error(NULL, NULL);
		}
	}

	zo = UDP_SEG_SIZE;
	xsocket_setopt(so, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(struct linger));
	xsocket_setopt(so, SOL_SOCKET, SO_SNDBUF, (const char*)&zo, sizeof(int));
	xsocket_setopt(so, SOL_SOCKET, SO_RCVBUF, (const char*)&zo, sizeof(int));

	pso = (udp_t*)xmem_alloc(sizeof(udp_t));
	pso->head.tag = _HANDLE_UDP;

	pso->type = _XUDP_TYPE_SRV;
	pso->so = so;

	async_alloc_lapp(&pso->ov);

	pso->snd_pdu = pdu_alloc(PDU_TYPE_UDP);
	pso->snd_bys = 0;
	pso->rcv_pdu = pdu_alloc(PDU_TYPE_UDP);
	pso->rcv_bys = 0;

	if (pack)
	{
		xmem_copy((void*)(pso->rcv_pdu->seg), (void*)pack, UDP_SEG_SIZE);
	}

	END_CATCH;

	return (xhand_t)pso;
ONERROR:

	if (so)
		xsocket_close(so);

	if (pso)
		async_free_lapp(&pso->ov);

	if (pso)
		xmem_free(pso);

	XDL_TRACE_LAST;

	return NULL;
}

void xudp_set_recv_timeout(xhand_t udp, int ms)
{
	udp_t* pso = (udp_t*)udp;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	xsocket_set_rcvtmo(pso->so, ms);
}

void xudp_set_send_timeout(xhand_t udp, int ms)
{
	udp_t* pso = (udp_t*)udp;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	xsocket_set_sndtmo(pso->so, ms);
}

void xudp_set_linger(xhand_t udp, bool_t b_wait, int n_sec)
{
	udp_t* pso = (udp_t*)udp;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	xsocket_set_linger(pso->so, b_wait, n_sec);
}

res_file_t xudp_socket(xhand_t udp)
{
	udp_t* pso = (udp_t*)udp;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	return pso->so;
}

int xudp_type(xhand_t udp)
{
	udp_t* pso = (udp_t*)udp;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	return pso->type;
}

void  xudp_close(xhand_t udp)
{
	udp_t* pso = (udp_t*)udp;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	xsocket_shutdown(pso->so, 0);

#if defined(_DEBUG) && defined(XDK_SUPPORT_THREAD)
	//wait data sended
	xthread_sleep(10);
#endif

	xsocket_close(pso->so);

	if (pso->rcv_pdu)
		pdu_free(pso->rcv_pdu);

	if (pso->snd_pdu)
		pdu_free(pso->snd_pdu);

	async_free_lapp(&pso->ov);

	xmem_free(pso);
}

bool_t xudp_write(xhand_t udp, const byte_t* buf, dword_t* pb)
{
	udp_t* pso = (udp_t*)udp;
	dword_t bys, dw;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	dw = *pb;
	while (dw)
	{
		bys = (pso->snd_bys + dw > UDP_SEG_SIZE) ? (UDP_SEG_SIZE - pso->snd_bys) : dw;
		xmem_copy((void*)(pso->snd_pdu->seg + pso->snd_bys), (void*)buf, bys);
		pso->snd_bys += bys;

		if (pso->snd_bys == UDP_SEG_SIZE)
		{
			bys = pso->snd_bys;
			if (!xsocket_send(pso->so, (void*)(pso->snd_pdu->seg), bys, &pso->ov))
			{
				*pb = 0;
				return 0;
			}
			bys = (dword_t)pso->ov.size;
			pso->snd_bys = 0;
		}
		dw -= bys;
	}
	
	return 1;
}

bool_t xudp_flush(xhand_t udp)
{
	udp_t* pso = (udp_t*)udp;
	dword_t dw;
	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	if (pso->snd_bys)
	{
		dw = pso->snd_bys;
		if (!xsocket_send(pso->so, (void*)(pso->snd_pdu->seg), dw, &pso->ov))
			return 0;

		pso->snd_bys = 0;
	}
	
	return 1;
}

bool_t xudp_read(xhand_t udp, byte_t* buf, dword_t* pb)
{
	udp_t* pso = (udp_t*)udp;
	dword_t bys, dw;

	XDL_ASSERT(udp && udp->tag == _HANDLE_UDP);

	dw = *pb;
	*pb = 0;
	while (dw)
	{
		if (!pso->rcv_bys)
		{
			bys = UDP_SEG_SIZE;
			if (!xsocket_recv(pso->so, (void*)(pso->rcv_pdu->seg), bys, &pso->ov))
			{
				*pb = 0;
				return 0;
			}
			bys = (dword_t)pso->ov.size;

			pso->rcv_bys = bys;
			pso->rcv_ret = 0;

			if (!bys)
			{
				return 1;
			}
		}

		bys = (pso->rcv_bys < dw) ? pso->rcv_bys : dw;
		xmem_copy((void*)(buf + *pb), (void*)(pso->rcv_pdu->seg + pso->rcv_ret), bys);

		pso->rcv_ret += bys;
		pso->rcv_bys -= bys;
		dw -= bys;
		*pb = *pb + bys;
	}

	return 1;
}

#endif //XDK_SUPPORT_SOCK
