/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pnp service document

	@module	xdspnp.c | pnp service implement file

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

#include "xdspnp.h"

typedef struct _pnp_accept_t{
	unsigned short port;
	tchar_t addr[ADDR_LEN];

	xhand_t ev;

	res_file_t so;
	void* pf_param;
	union
	{
		PF_PNPS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};

	byte_t pack[PNP_PKG_SIZE];
	dword_t size;
}pnp_accept_t;

/************************************************************************************************/

static pnp_listen_t*  _xpnp_listen(unsigned short port)
{
	pnp_listen_t* plis;
	res_file_t so;
	net_addr_t locaddr;
	sys_info_t si = { 0 };

	so = xsocket_udp(0, FILE_OPEN_OVERLAP);
	if (so == INVALID_FILE)
	{
		return NULL;
	}

	xmem_zero((void*)&locaddr, sizeof(locaddr));
	fill_addr(&locaddr, port, NULL);

	if (!xsocket_bind(so, (res_addr_t)&locaddr, sizeof(locaddr)))
	{
		xsocket_close(so);
		return NULL;
	}

	plis = (pnp_listen_t*)xmem_alloc(sizeof(pnp_listen_t));

	plis->so = so;
	plis->port = port;
	plis->act = 1;
	
	system_info(&si);
	plis->res = si.processor_number;

	return plis;
}

static unsigned int STDCALL thread_dispatch(void* param)
{
	pnp_accept_t* pxa = (pnp_accept_t*)param;

	PF_PNPS_DISPATCH pf_dispatch = NULL;
	void* pf_param = NULL;
	unsigned short port;
	tchar_t addr[ADDR_LEN];
	byte_t pack[PNP_PKG_SIZE];

	xdl_thread_init();

	pf_dispatch = pxa->pf_dispatch;
	pf_param = pxa->pf_param;

	port = pxa->port;
	xsncpy(addr, pxa->addr, ADDR_LEN);
	xmem_copy((void*)pack, (void*)pxa->pack, PNP_PKG_SIZE);

	xevent_sign(pxa->ev, 1);

	(*pf_dispatch)(port, addr, pack, PNP_PKG_SIZE, pf_param);

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

static unsigned int STDCALL process_dispatch(void* param)
{
	pnp_accept_t* pxa = (pnp_accept_t*)param;

	unsigned short port;
	byte_t addr[ADDR_LEN];
	dword_t alen;
	byte_t pack[PNP_PKG_SIZE];
	dword_t size;

	const tchar_t* sz_module = NULL;
	const void* pf_param = NULL;

	proc_info_t pi = { 0 };

	xhand_t pipe = NULL;
	stream_t stm = NULL;
	byte_t num[2];

	xdl_thread_init();

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

	xevent_sign(pxa->ev, 1);

	if (create_process(sz_module, (tchar_t*)pf_param, 1, &pi))
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

					PUT_SWORD_NET(num, 0, port);
					stream_write_chunk_size(stm, 2);
					stream_write_bytes(stm, num, 2);

					stream_write_chunk_size(stm, alen);
					stream_write_bytes(stm, addr, alen);

					stream_write_chunk_size(stm, size);
					stream_write_bytes(stm, pack, size);

					stream_write_chunk_size(stm, 0);
				}

				stream_free(stm);
				stm = NULL;
			}

			xpipe_flush(pipe);

			xpipe_free(pipe);
			pipe = NULL;
		}

		release_process(&pi);

#if defined(_DEBUG) || defined(DEBUG)
		//wait process run
		xthread_sleep(10);
#endif
	}

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

static unsigned int STDCALL wait_accept(void* param)
{
	pnp_listen_t* plis = (pnp_listen_t*)param;

	net_addr_t locaddr, rmtaddr;
	int addr_len;
	pnp_accept_t xa = { 0 };
	dword_t dw;

	async_t over = { 0 };

	xdl_thread_init();

	async_alloc_lapp(&over);

#ifdef XDK_SUPPORT_THREAD_QUEUE
	over.type = ASYNC_QUEUE;
	over.port = plis->epo;
#endif

	xsocket_addr(plis->so, &locaddr);

	while (plis->act)
	{
		addr_len = sizeof(net_addr_t);
		dw = PNP_PKG_SIZE;
		if (xsocket_recvfrom(plis->so, (res_addr_t)&rmtaddr, &addr_len, xa.pack, dw, &over))
		{
			conv_addr(&rmtaddr, &xa.port, xa.addr);
			xa.size = (dword_t)over.size;

			xa.pf_param = plis->pf_param;

			if (plis->is_thread)
				xa.pf_dispatch = plis->pf_dispatch;
			else
				xa.sz_module = plis->sz_module;

			xa.ev = xevent_create();

			if (plis->is_thread)
			{
				xthread_begin(NULL, (PF_THREADFUNC)thread_dispatch, (void*)&xa);
			}
			else
			{
				xthread_begin(NULL, (PF_THREADFUNC)process_dispatch, (void*)&xa);
			}

			xevent_wait(xa.ev, -1);
			xevent_destroy(xa.ev);

			xmem_zero((void*)&xa, sizeof(pnp_accept_t));
		}
	}

	async_release_lapp(&over);

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

pnp_listen_t* xpnp_start_thread(unsigned short port, PF_PNPS_DISPATCH pf_dispatch, void* param)
{
	pnp_listen_t* plis;
	int i;

	plis = _xpnp_listen(port);
	if (!plis)
		return NULL;

	plis->is_thread = 1;
	plis->pf_dispatch = pf_dispatch;
	plis->pf_param = param;

#ifdef XDK_SUPPORT_THREAD_QUEUE
	plis->epo = xqueue_create((res_hand_t)NULL, plis->so, plis->res);
#endif

	for (i = 0; i < plis->res; i++)
	{
		xthread_begin(NULL, (PF_THREADFUNC)wait_accept, (void*)plis);
#if defined(_DEBUG) || defined(DEBUG)
		xthread_sleep(10);
#endif
	}

	return plis;
}

pnp_listen_t* xpnp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	pnp_listen_t* plis;
	int i;

	plis = _xpnp_listen(port);
	if (!plis)
		return NULL;

	plis->is_thread = 0;
	plis->sz_module = sz_module;
	plis->pf_param = (void*)sz_cmdline;

#ifdef XDK_SUPPORT_THREAD_QUEUE
	plis->epo = xqueue_create((res_hand_t)NULL, plis->so, plis->res);
#endif

	for (i = 0; i < plis->res; i++)
	{
		xthread_begin(NULL, (PF_THREADFUNC)wait_accept, (void*)plis);
#if defined(_DEBUG) || defined(DEBUG)
		xthread_sleep(10);
#endif
	}

	return plis;
}

void  xpnp_stop(pnp_listen_t* plis)
{
	//indicate listen to be stoping
	plis->act = 0;

	//disiable recive and send
	xsocket_shutdown(plis->so, 2);

	//wait listen stoped
	xthread_sleep(THREAD_BASE_TMO);

	xsocket_close(plis->so);

#ifdef XDK_SUPPORT_THREAD_QUEUE
	if (plis->epo)
	{
		xqueue_destroy(plis->epo);
		plis->epo = (res_hand_t)NULL;
	}
#endif

	xmem_free(plis);
}


