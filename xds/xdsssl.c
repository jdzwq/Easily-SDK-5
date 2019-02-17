/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssl service document

	@module	xdsssl.c | ssl service implement file

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

#include "xdsssl.h"

typedef struct _ssl_accept_t{
	res_file_t so;

	xhand_t ev;

	void* pf_param;
	union
	{
		PF_SSLS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};
}ssl_accept_t;

/************************************************************************************************/
static ssl_listen_t*  _xssl_listen(unsigned short port)
{
	ssl_listen_t* plis;
	net_addr_t sin;
	res_file_t so;
	sys_info_t si = { 0 };

	so = xsocket_tcp(0, FILE_OPEN_OVERLAP);
	if (so == INVALID_FILE)
	{
		return NULL;
	}

	xmem_zero((void*)&sin, sizeof(sin));

	fill_addr(&sin, port, NULL);

	if (!xsocket_bind(so, (res_addr_t)&sin, sizeof(sin)))
	{
		xsocket_close(so);
		return NULL; //bind sock error
	}

	if (!xsocket_listen(so, SOMAXCONN))
	{
		xsocket_close(so);
		return NULL; //listen error
	}

	plis = (ssl_listen_t*)xmem_alloc(sizeof(ssl_listen_t));

	plis->so = so;
	plis->act = 1;
	
	system_info(&si);
	plis->res = si.processor_number;
	
	return plis;
}

static unsigned int STDCALL thread_dispatch(void* param)
{
	ssl_accept_t* pxa = (ssl_accept_t*)param;

	PF_SSLS_DISPATCH pf_dispatch = NULL;
	void* pf_param = NULL;
	xhand_t ssl = NULL;
	res_file_t so = 0;

	xdl_thread_init();

	so = pxa->so;
	pf_dispatch = pxa->pf_dispatch;
	pf_param = pxa->pf_param;

	ssl = xssl_srv(so);

	xevent_sign(pxa->ev, 1);

	(*pf_dispatch)(ssl, pf_param);

	xssl_close(ssl);

	xsocket_close(so);

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

static unsigned int STDCALL process_dispatch(void* param)
{
	ssl_accept_t* pxa = (ssl_accept_t*)param;

	res_file_t so;
	const tchar_t* sz_module = NULL;
	const void* pf_param = NULL;

	proc_info_t pi = { 0 };

	xdl_thread_init();

	so = pxa->so;
	sz_module = pxa->sz_module;
	pf_param = pxa->pf_param;

	xevent_sign(pxa->ev, 1);

	if (create_process(sz_module, (tchar_t*)pf_param, 1, &pi))
	{
		if (pi.pip_write)
		{
			xsocket_share(pi.process_id, pi.pip_write, so, NULL, 0);
		}

#if defined(_DEBUG) || defined(DEBUG)
		//wait process run
		xthread_sleep(10);
#endif
        release_process(&pi);

		xsocket_close(so);
	}
	else
	{
		//disable recive
		xsocket_shutdown(so, 0);
		xsocket_close(so);
	}

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

static unsigned int STDCALL wait_accept(void* param)
{
	ssl_listen_t* plis = (ssl_listen_t*)param;

	res_file_t so;
	net_addr_t locaddr, rmtaddr;
	int addr_len;
	ssl_accept_t xa = { 0 };
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
		so = xsocket_accept(plis->so, (res_addr_t)&rmtaddr, &addr_len, &over);
		if (so == INVALID_FILE)
		{
			continue;
		}

		xa.so = so;
		xa.pf_param = plis->pf_param;
		if (plis->is_thread)
			xa.pf_dispatch = plis->pf_dispatch;
		else
			xa.sz_module = plis->sz_module;

		xa.pf_param = plis->pf_param;
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

		xmem_zero((void*)&xa, sizeof(ssl_accept_t));
	}

	async_release_lapp(&over);

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

ssl_listen_t* xssl_start_thread(unsigned short port, PF_SSLS_DISPATCH pf_dispatch, void* param)
{
	ssl_listen_t* plis;
	int i;

	plis = _xssl_listen(port);
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

ssl_listen_t* xssl_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	ssl_listen_t* plis;
	int i;

	plis = _xssl_listen(port);
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

void xssl_stop(ssl_listen_t* plis)
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


