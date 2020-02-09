/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc thread system call document

	@module	_if_thread.c | windows implement file

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


#include "xdkiml.h"

#ifdef XDK_SUPPORT_THREAD

#ifndef WINCE
#include <process.h>
#endif

void _thread_create_tls(tls_key_t* pkey)
{
	*pkey = TlsAlloc();
}

void _thread_destroy_tls(tls_key_t key)
{
	TlsFree(key);
}

void* _thread_get_tls(tls_key_t key)
{
	return TlsGetValue(key);
}

void _thread_set_tls(tls_key_t key, void* pv)
{
	TlsSetValue(key, pv);
}

pid_t _thread_get_id(void)
{
	return (pid_t)GetCurrentThreadId();
}

void _thread_safe()
{
	return;
}

void _thread_begin(res_thread_t* ph_thread, PF_THREADFUNC pf_func, void* param)
{
	res_thread_t th;
#ifdef WINCE
	th = (res_thread_t)CreateThread(NULL,0,pf_func,param,0,NULL);
#else
	th = (res_thread_t)_beginthreadex(NULL, 0, (PF_THREADFUNC)pf_func, param, 0, NULL);
#endif

	if (ph_thread) *ph_thread = th;
}

void _thread_end(void)
{
#ifdef WINCE
	ExitThread(0);
#else
	_endthreadex(0);
#endif
}

void _thread_sleep(int ms)
{
	Sleep(((ms < 0)? INFINITE : ms));
}

void _thread_yield()
{
	SwitchToThread();
}

void _thread_join(res_thread_t th)
{
	DWORD dw = 0;

	if (!GetHandleInformation((HANDLE)th, &dw))
		return;

	WaitForSingleObject((HANDLE)th, INFINITE);
}

/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_EVENT
res_even_t _event_create()
{
	return CreateEvent(NULL, 1, 0, NULL);
}

void _event_destroy(res_even_t ev)
{
	CloseHandle(ev);
}

wait_t _event_wait(res_even_t ev, int milsec)
{
	DWORD dw;

	if (!GetHandleInformation(ev, &dw))
	{
		return WAIT_ERR;
	}

	if (milsec < 0)
		dw = INFINITE;
	else
		dw = milsec;

	switch (WaitForSingleObject(ev, dw))
	{
	case WAIT_OBJECT_0:
		return WAIT_RET;
	case WAIT_TIMEOUT:
		return WAIT_TMO;
	default:
		return WAIT_ERR;
	}
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

void _mutex_destroy(const tchar_t* mname, res_mutx_t mtx)
{
	CloseHandle(mtx);
}

res_mutx_t _mutex_open(const tchar_t* mname)
{
	return OpenMutex(MUTEX_ALL_ACCESS, FALSE, mname);
}

wait_t _mutex_lock(res_mutx_t mtx, int milsec)
{
	DWORD dw;

	if (!GetHandleInformation(mtx, &dw))
	{
		return WAIT_ERR;
	}

	if (milsec < 0)
		dw = INFINITE;
	else
		dw = milsec;

	switch (WaitForSingleObject(mtx, dw))
	{
	case WAIT_OBJECT_0:
		return WAIT_RET;
	case WAIT_TIMEOUT:
		return WAIT_TMO;
	default:
		return WAIT_ERR;
	}
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

void _semap_destroy(const tchar_t* sname, res_sema_t sem)
{
	CloseHandle(sem);
}

res_sema_t _semap_open(const tchar_t* sname)
{
	return OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, sname);
}

wait_t _semap_lock(res_sema_t sem, int milsec)
{
	DWORD dw;

	if (!GetHandleInformation(sem, &dw))
	{
		return WAIT_ERR;
	}

	if (milsec < 0)
		dw = INFINITE;
	else
		dw = milsec;

	switch (WaitForSingleObject(sem, dw))
	{
	case WAIT_OBJECT_0:
		return WAIT_RET;
	case WAIT_TIMEOUT:
		return WAIT_TMO;
	default:
		return WAIT_ERR;
	}
}

void _semap_unlock(res_sema_t sem)
{
	ReleaseSemaphore(sem, 1, NULL);
}
#endif

/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_QUEUE
res_queue_t _queue_create(res_queue_t ep, res_file_t fd, int max)
{
	return (res_queue_t)CreateIoCompletionPort(((fd == INVALID_FILE)? INVALID_HANDLE_VALUE : (HANDLE)fd), ep, (ULONG_PTR)fd, (DWORD)max);
}

void _queue_destroy(res_queue_t ep)
{
	CloseHandle(ep);
}

wait_t _queue_wait(res_queue_t ep, int ms)
{
	DWORD dw = 0;
	ULONG_PTR up = NULL;
	LPOVERLAPPED ul = NULL;
	DWORD tmo;

	if (ms < 0)
		tmo = INFINITE;
	else
		tmo = ms;

	if (!GetQueuedCompletionStatus(ep, &dw, &up, &ul, tmo))
	{
		if (GetLastError() == WAIT_TIMEOUT)
			return WAIT_TMO;

		return WAIT_ERR;
	}

	return WAIT_RET;
}
#endif

#endif //XDK_SUPPORT_THREAD