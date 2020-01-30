/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tcp service document

	@module	xdstcp.c | implement file

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

#include "xdstcp.h"

typedef struct _tcp_accept_t{
	res_file_t so;

	res_even_t ev;

	void* pf_param;
	union
	{
		PF_TCPS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};
}tcp_accept_t;

/************************************************************************************************/
static tcp_listen_t*  _xtcp_listen(unsigned short port)
{
	tcp_listen_t* plis;
	net_addr_t sin;
	res_file_t so;
	sys_info_t si = { 0 };
	
	so = socket_tcp(0, FILE_OPEN_OVERLAP);
	if (so == INVALID_FILE)
	{
		return NULL;
	}

	xmem_zero((void*)&sin, sizeof(sin));

	fill_addr(&sin, port, NULL);

	if (!socket_bind(so, (res_addr_t)&sin, sizeof(sin)))
	{
		socket_close(so);
		return NULL; //bind sock error
	}

	if (!socket_listen(so, SOMAXCONN))
	{
		socket_close(so);
		return NULL; //listen error
	}

	plis = (tcp_listen_t*)xmem_alloc(sizeof(tcp_listen_t));

	plis->so = so;
	plis->act = 1;

	system_info(&si);
	plis->res = si.processor_number;

	return plis;
}

static unsigned STDCALL thread_dispatch(void* param)
{
	tcp_accept_t* pxa = (tcp_accept_t*)param;

	PF_TCPS_DISPATCH pf_dispatch = NULL;
	void* pf_param = NULL;
	xhand_t tcp = NULL;
	res_file_t so = 0;
	res_even_t ev = NULL;

	xdl_thread_init(0);

	so = pxa->so;
	pxa->so = 0;
	pf_dispatch = pxa->pf_dispatch;
	pf_param = pxa->pf_param;
	ev = pxa->ev;

	event_sign(ev, 1);
    
    tcp = xtcp_srv(so);

	(*pf_dispatch)(tcp, pf_param);

	xtcp_close(tcp);

	socket_close(so);

	xdl_thread_uninit(0);

	thread_stop();

	return 0;
}

static unsigned STDCALL process_dispatch(void* param)
{
	tcp_accept_t* pxa = (tcp_accept_t*)param;

	res_file_t so;
	const tchar_t* sz_module = NULL;
	const tchar_t* sz_param = NULL;
	res_even_t ev;

	proc_info_t pi = { 0 };

	xdl_thread_init(0);

	so = pxa->so;
	pxa->so = 0;
	sz_module = pxa->sz_module;
	sz_param = (const tchar_t*)pxa->pf_param;
	ev = pxa->ev;

	event_sign(ev, 1);

	if (create_process(sz_module, (tchar_t*)sz_param, SHARE_SOCK, &pi))
	{
		socket_share(pi.process_id, pi.pip_write, so, NULL, 0);

        thread_yield();
        
        release_process(&pi);
	}

	socket_close(so);

	xdl_thread_uninit(0);

	thread_stop();

	return 0;
}

static unsigned STDCALL wait_accept(void* param)
{
	tcp_listen_t* plis = (tcp_listen_t*)param;

	res_file_t so;
	net_addr_t locaddr, rmtaddr;
	int addr_len;
	tcp_accept_t xa = { 0 };
	async_t* pov = NULL;

	xdl_thread_init(0);

	if (plis->res == 1)
	{
		pov = async_alloc_lapp(ASYNC_QUEUE, TCP_BASE_TIMO, plis->so);
	}
	else
	{
		pov = async_alloc_lapp(ASYNC_EVENT, TCP_BASE_TIMO, INVALID_FILE);
	}

	socket_addr(plis->so, &locaddr);

	while (plis->act)
	{
		addr_len = sizeof(net_addr_t);
		so = socket_accept(plis->so, (res_addr_t)&rmtaddr, &addr_len, pov);
		if (so == INVALID_FILE)
		{
			thread_yield();
			continue;
		}

		xa.so = so;
		xa.pf_param = plis->pf_param;
		if (plis->is_thread)
			xa.pf_dispatch = plis->pf_dispatch;
		else
			xa.sz_module = plis->sz_module;

		xa.pf_param = plis->pf_param;
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

			event_wait(xa.ev, TCP_BASE_TIMO);
			event_destroy(xa.ev);
		}

		if (xa.so)
		{
			socket_close(xa.so);
		}

		xmem_zero((void*)&xa, sizeof(tcp_accept_t));
	}

	async_free_lapp(pov);

	xdl_thread_uninit(0);

	thread_stop();

	return 0;
}

tcp_listen_t* xtcp_start_thread(unsigned short port, PF_TCPS_DISPATCH pf_dispatch, void* param)
{
	tcp_listen_t* plis;
	int i;

	plis = _xtcp_listen(port);
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

tcp_listen_t* xtcp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	tcp_listen_t* plis;
	int i;

	plis = _xtcp_listen(port);
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

void xtcp_stop(tcp_listen_t* plis)
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
    
    thread_yield();

	xmem_free(plis->thr);

	xmem_free(plis);
}


