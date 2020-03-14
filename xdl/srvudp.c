/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc udp service document

	@module	srvudp.c | implement file

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

#include "srvudp.h"
#include "xdlinit.h"
#include "xdloem.h"
#include "xdlimp.h"
#include "xdlstd.h"
#include "xdlnet.h"

typedef struct _udp_accept_t{
	unsigned short port;
	tchar_t addr[ADDR_LEN];

	res_even_t ev;

	res_file_t so;
	void* pf_param;
	union
	{
		PF_UDPS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};

	byte_t pack[UDP_PDU_SIZE];
	dword_t size;
}udp_accept_t;

/************************************************************************************************/

static udp_listen_t*  _xudp_listen(unsigned short port)
{
	udp_listen_t* plis;
	res_file_t so;
	net_addr_t locaddr;
	sys_info_t si = { 0 };

	so = socket_udp(0, FILE_OPEN_OVERLAP);
	if (so == INVALID_FILE)
	{
		return NULL;
	}

	xmem_zero((void*)&locaddr, sizeof(locaddr));
	fill_addr(&locaddr, port, NULL);

	if (!socket_bind(so, (res_addr_t)&locaddr, sizeof(locaddr)))
	{
		socket_close(so);
		return NULL;
	}

	plis = (udp_listen_t*)xmem_alloc(sizeof(udp_listen_t));

	plis->so = so;
	plis->port = port;
	plis->act = 1;
	
	system_info(&si);
	plis->res = si.processor_number;

	return plis;
}

static unsigned STDCALL thread_dispatch(void* param)
{
	udp_accept_t* pxa = (udp_accept_t*)param;

	PF_UDPS_DISPATCH pf_dispatch = NULL;
	void* pf_param = NULL;
	res_even_t ev = NULL;
	unsigned short port;
	tchar_t addr[ADDR_LEN];
	byte_t pack[UDP_PDU_SIZE];
	dword_t size;

	xdl_thread_init(0);

	pf_dispatch = pxa->pf_dispatch;
	pf_param = pxa->pf_param;
	ev = pxa->ev;

	event_sign(ev, 1);

	port = pxa->port;
	xsncpy(addr, pxa->addr, ADDR_LEN);
	xmem_copy((void*)pack, (void*)pxa->pack, pxa->size);
	size = pxa->size;

	(*pf_dispatch)(port, addr, pack, size, pf_param);

	xdl_thread_uninit(0);

	thread_stop();

	return 0;
}

static unsigned STDCALL process_dispatch(void* param)
{
	udp_accept_t* pxa = (udp_accept_t*)param;

	unsigned short port;
	byte_t addr[ADDR_LEN];
	dword_t alen;
	byte_t pack[UDP_PDU_SIZE];
	dword_t size;

	const tchar_t* sz_module = NULL;
	const void* pf_param = NULL;
	res_even_t ev = NULL;

	proc_info_t pi = { 0 };

	xhand_t pipe = NULL;
	stream_t stm = NULL;
	byte_t num[2];

	xdl_thread_init(0);

	port = pxa->port;
#ifdef _UNICODE
	alen = ucs_to_mbs(pxa->addr, -1, (schar_t*)addr, ADDR_LEN);
#else
	alen = mbs_to_mbs(pxa->addr, -1, (schar_t*)addr, ADDR_LEN);
#endif
	xmem_copy((void*)pack, (void*)pxa->pack, pxa->size);
	size = pxa->size;

	sz_module = pxa->sz_module;
	pf_param = pxa->pf_param;
	ev = pxa->ev;

	event_sign(ev, 1);

	if (create_process(sz_module, (tchar_t*)pf_param, SHARE_SOCK, &pi))
	{
		if (pi.pip_write)
		{
			pipe = xpipe_attach(pi.pip_write);
			if (pipe)
			{
				stm = stream_alloc(pipe);
				if (stm)
				{
					stream_set_mode(stm, CHUNK_OPERA);

					//write port
					stream_write_chunk_size(stm, 2);
					PUT_SWORD_NET(num, 0, port);
					stream_write_bytes(stm, num, 2);

					//write addr
					stream_write_chunk_size(stm, alen);
					stream_write_bytes(stm, addr, alen);

					//write data
					stream_write_chunk_size(stm, size);
					stream_write_bytes(stm, pack, size);

					//terminate
					stream_write_chunk_size(stm, 0);

					stream_flush(stm);

					stream_free(stm);
					stm = NULL;
				}

				xpipe_detach(pipe);
				pipe = NULL;
			}
		}
        
        thread_yield();
        
		release_process(&pi);
	}

	xdl_thread_uninit(0);

	thread_stop();

	return 0;
}

static unsigned STDCALL wait_accept(void* param)
{
	udp_listen_t* plis = (udp_listen_t*)param;

	net_addr_t locaddr, rmtaddr;
	int addr_len;
	udp_accept_t xa = { 0 };
	dword_t dw;

	async_t* pov = NULL;

	xdl_thread_init(0);

	if (plis->res == 1)
	{
		pov = async_alloc_lapp(ASYNC_QUEUE, UDP_BASE_TIMO, plis->so);
	}else
	{
		pov = async_alloc_lapp(ASYNC_EVENT, UDP_BASE_TIMO, INVALID_FILE);
	}

	socket_addr(plis->so, &locaddr);

	while (plis->act)
	{
		addr_len = sizeof(net_addr_t);
		dw = UDP_PDU_SIZE;
		if (!socket_recvfrom(plis->so, (res_addr_t)&rmtaddr, &addr_len, xa.pack, dw, pov))
		{
            thread_yield();
			continue;
		}

		conv_addr(&rmtaddr, &xa.port, xa.addr);
		xa.size = (dword_t)pov->size;

		xa.pf_param = plis->pf_param;

		if (plis->is_thread)
			xa.pf_dispatch = plis->pf_dispatch;
		else
			xa.sz_module = plis->sz_module;

		xa.ev = event_create();

		if (xa.ev)
		{
			if (plis->is_thread)
			{
				thread_start(NULL, (PF_THREADFUNC)thread_dispatch, (void*)&xa);
			}
			else
			{
				thread_start(NULL, (PF_THREADFUNC)process_dispatch, (void*)&xa);
			}
            
			event_wait(xa.ev, UDP_BASE_TIMO);
			event_destroy(xa.ev);
		}

		xmem_zero((void*)&xa, sizeof(udp_accept_t));
	}

	async_free_lapp(pov);

	xdl_thread_uninit(0);

	thread_stop();

	return 0;
}

udp_listen_t* xudp_start_thread(unsigned short port, PF_UDPS_DISPATCH pf_dispatch, void* param)
{
	udp_listen_t* plis;
	int i;

	plis = _xudp_listen(port);
	if (!plis)
		return NULL;

	plis->is_thread = 1;
	plis->pf_dispatch = pf_dispatch;
	plis->pf_param = param;

	plis->thr = (res_thread_t*)xmem_alloc(sizeof(res_thread_t) * plis->res);

	for (i = 0; i < plis->res; i++)
	{
		thread_start(&(plis->thr[i]), (PF_THREADFUNC)wait_accept, (void*)plis);
	}

	return plis;
}

udp_listen_t* xudp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	udp_listen_t* plis;
	int i;

	plis = _xudp_listen(port);
	if (!plis)
		return NULL;

	plis->is_thread = 0;
	plis->sz_module = sz_module;
	plis->pf_param = (void*)sz_cmdline;

	plis->thr = (res_thread_t*)xmem_alloc(sizeof(res_thread_t) * plis->res);

	for (i = 0; i < plis->res; i++)
	{
		thread_start(&(plis->thr[i]), (PF_THREADFUNC)wait_accept, (void*)plis);
	}

	return plis;
}

void  xudp_stop(udp_listen_t* plis)
{
	int i;

	//indicate listen to be stoping
	plis->act = 0;

	//disiable recive and send
	socket_shutdown(plis->so, 2);

	socket_close(plis->so);

	for (i = 0; i < plis->res; i++)
	{
		thread_join(plis->thr[i]);
	}

	xmem_free(plis->thr);

	xmem_free(plis);
}


