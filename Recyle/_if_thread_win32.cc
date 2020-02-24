/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc thread system call document

	@module	_if_thread.c | thread system call windows implement file

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

res_sock_t xsocket_epoll(res_epoll_t ep, res_sock_t lso, res_addr_t saddr, int *plen)
{
	res_sock_t aso;
	int len, zero = 0;
	byte_t addrbuf[64] = { 0 };
	int addr_len;
	net_addr_t rmtaddr;
	async_t ov = { 0 };

	aso = xsocket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (aso <= 0)
		return 0;

	ov.obj = (void*)aso;

	len = 64;
	xmem_zero(addrbuf, len);
	zero = xsocket_accept_async(lso, aso, &ov, addrbuf, &len);
	switch (zero)
	{
	case IO_SUCCEED:
		addr_len = sizeof(net_addr_t);
		xsocket_peer_async(aso, addrbuf, (res_addr_t)&rmtaddr, &addr_len);
		xsocket_update_context(aso, lso);
		break;
	case IO_PENDING:
		if(&ov == xepoll_wait(ep, -1))
		{
			addr_len = sizeof(net_addr_t);
			xsocket_peer_async(aso, addrbuf, (res_addr_t)&rmtaddr, &addr_len);
			xsocket_update_context(aso, lso);
		}
		else
		{
			xsocket_close(aso);
			aso = 0;
		}
		break;
	default:
		xsocket_close(aso);
		aso = 0;
		break;
	}

	return aso;
}

#include "xdkiml.h"

#ifdef XDK_SUPPORT_THREAD

#ifndef WINCE
#include <process.h>
#endif

dword_t	_thread_create_tls(void)
{
	return (dword_t)TlsAlloc();
}

void _thread_destroy_tls(dword_t dw)
{
	TlsFree(dw);
}

void* _thread_get_tls(dword_t dw)
{
	return TlsGetValue(dw);
}

void _thread_set_tls(dword_t dw, void* pv)
{
	TlsSetValue(dw, pv);
}

dword_t _thread_get_id(void)
{
	return (dword_t)GetCurrentThreadId();
}

void _thread_begin(PF_THREADFUNC pf_func, void* param)
{
#ifdef WINCE
	CreateThread(NULL,0,pf_func,param,0,NULL);
#else
	_beginthreadex(NULL, 0, pf_func, param, 0, NULL);
#endif
}

void _thread_end(void)
{
#ifdef WINCE
	ExitThread(0);
#else
	_endthreadex(0);
#endif
}

void _thread_sleep(dword_t n)
{
	Sleep(n);
}

/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_EVENT
res_even_t _event_create(const tchar_t* ename)
{
	return CreateEvent(NULL, 1, 0, ename);
}

res_even_t _event_open(const tchar_t* ename)
{
	return OpenEvent(EVENT_ALL_ACCESS, FALSE, ename);
}

void _event_destroy(res_even_t ev)
{
	CloseHandle(ev);
}

int _event_wait(res_even_t ev, int milsec)
{
	DWORD dw;

	if (milsec < 0)
		dw = INFINITE;
	else
		dw = milsec;

	if (WaitForSingleObject(ev, dw) == WAIT_OBJECT_0)
		return 1;
	else
		return -1;
}

void _event_sign(res_even_t ev, bool_t b_sign)
{
	if (b_sign)
		SetEvent(ev);
	else
		ResetEvent(ev);
}
#endif //XDK_SUPPORT_THREAD_EVENT
/********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_CRITI
res_crit_t _criti_create()
{
	LPCRITICAL_SECTION pcr;

	pcr = (LPCRITICAL_SECTION)GlobalAlloc(GPTR, sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(pcr);

	return pcr;
}

void _criti_destroy(res_crit_t cr)
{
	DeleteCriticalSection(cr);

	GlobalFree((HGLOBAL)cr);
}

void _criti_enter(res_crit_t cr)
{
	EnterCriticalSection(cr);
}

void _criti_leave(res_crit_t cr)
{
	LeaveCriticalSection(cr);
}

bool_t _criti_query(res_crit_t cr)
{
	return (bool_t)TryEnterCriticalSection(cr);
}
#endif
/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_MUTEX
res_mutx_t _mutex_create(const tchar_t* mname)
{
	return CreateMutex(NULL, FALSE, mname);
}

void _mutex_destroy(res_mutx_t mtx)
{
	CloseHandle(mtx);
}

res_mutx_t _mutex_open(const tchar_t* mname)
{
	return OpenMutex(MUTEX_ALL_ACCESS, FALSE, mname);
}

bool_t _mutex_lock(res_mutx_t mtx, int milsec)
{
	if (milsec < 0)
		milsec = INFINITE;

	return (WaitForSingleObject(mtx, milsec) == WAIT_OBJECT_0) ? 1 : 0;
}

void _mutex_unlock(res_mutx_t mtx)
{
	ReleaseMutex(mtx);
}
#endif
/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_SEMAP
res_sema_t _semap_create(const tchar_t* sname, int max)
{
	return CreateSemaphore(NULL, max, max, sname);
}

void _semap_destroy(res_sema_t sem)
{
	CloseHandle(sem);
}

res_sema_t _semap_open(const tchar_t* sname)
{
	return OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, sname);
}

bool_t _semap_lock(res_sema_t sem, int milsec)
{
	if (milsec < 0)
		milsec = INFINITE;

	return (WaitForSingleObject(sem, milsec) == WAIT_OBJECT_0) ? 1 : 0;
}

void _semap_unlock(res_sema_t sem)
{
	ReleaseSemaphore(sem, 1, NULL);
}
#endif
/******************************************************************************/
#ifdef XDK_SUPPORT_THREAD_EPOLL
res_epoll_t _epoll_create(int max)
{
	res_epoll_t ep;

	ep = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, max);
	if (!ep)
		return NULL;

	return (res_epoll_t)ep;
}

bool_t _epoll_attach(res_epoll_t ep, res_file_t io, res_void_t key)
{
	return (NULL == CreateIoCompletionPort((HANDLE)io, ep, (ULONG_PTR)key, 0)) ? 0 : 1;
}

async_t* _epoll_wait(res_epoll_t ep, int tmo)
{
	DWORD dw = 0;
	ULONG_PTR up = 0;
	LPOVERLAPPED lp = NULL;

	if (!GetQueuedCompletionStatus(ep, &dw, &up, (LPOVERLAPPED*)&lp, ((tmo < 0) ? INFINITE : tmo)))
		return NULL;
	else
		return (async_t*)lp;
}

bool_t _epoll_sign(res_epoll_t ep, res_void_t key)
{
	return (PostQueuedCompletionStatus(ep, 0, (ULONG_PTR)key, NULL)) ? 1 : 0;
}

void _epoll_destroy(res_epoll_t ep)
{
	CloseHandle(ep);
}

#endif

#endif //XDK_SUPPORT_THREAD