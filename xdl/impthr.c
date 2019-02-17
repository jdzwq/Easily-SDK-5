/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc thread document

	@module	impthr.c | thread implement file

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

#include "impthr.h"
#include "xdlinit.h"
#include "xdlimp.h"

#ifdef XDK_SUPPORT_THREAD

void  xthread_begin(res_hand_t* ph_hand, PF_THREADFUNC pf_worker, void* param)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_thread_begin)(ph_hand, pf_worker,param);
}

void xthread_end(void)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_thread_end)();
}

void xthread_sleep(int ms)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_thread_sleep)(ms);
}

dword_t xthread_get_id(void)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	return (*pit->pf_thread_get_id)();
}

void xthread_join(res_hand_t th)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_thread_join)(th);
}
/////////////////////////////////////////////////////////////////////
#ifdef XDK_SUPPORT_THREAD_EVENT
res_even_t xevent_create()
{
	res_even_t ev;
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	ev = (*pit->pf_event_create)();
	if (!ev)
	{
		set_system_error(_T("pf_event_create"));
		return NULL;
	}

	return ev;
}

void xevent_destroy(res_even_t eh)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_event_destroy)(eh);
}

wait_t xevent_wait(res_even_t eh, int ms)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	return (*pit->pf_event_wait)(eh, ms);
}

void xevent_sign(res_even_t eh, bool_t b_sign)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_event_sign)(eh, b_sign);
}

#endif

////////////////////////////////////////////////////////////////////////
#ifdef XDK_SUPPORT_THREAD_CRITI

res_crit_t xcriti_create()
{
	if_thread_t* pit;
	res_crit_t cr;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	cr = (*pit->pf_criti_create)();
	if (!cr)
	{
		set_system_error(_T("pf_criti_create"));
		return NULL;
	}

	return cr;
}

void xcriti_destroy(res_crit_t cr)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_criti_destroy)(cr);
}

void xcriti_enter(res_crit_t cr)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_criti_enter)(cr);
}

void xcriti_leave(res_crit_t cr)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_criti_leave)(cr);
}

bool_t xcriti_query(res_crit_t cr)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	return (*pit->pf_criti_query)(cr);
}

#endif

/***********************************************************************************************/
#ifdef XDK_SUPPORT_THREAD_MUTEX

res_mutx_t	xmutex_create(const tchar_t* mname)
{
	if_thread_t* pit;
	res_mutx_t mx;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	mx = (*pit->pf_mutex_create)(mname);
	if (!mx)
	{
		set_system_error(_T("pf_mutex_create"));
		return NULL;
	}

	return mx;
}

void xmutex_destroy(const tchar_t* mname, res_mutx_t mx)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_mutex_destroy)(mname, mx);
}

res_mutx_t	xmutex_open(const tchar_t* mname)
{
	if_thread_t* pit;
	res_mutx_t mx;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	mx = (void*)(*pit->pf_mutex_open)(mname);
	if (!mx)
	{
		set_system_error(_T("pf_mutex_open"));
		return NULL;
	}

	return mx;
}

wait_t xmutex_lock(res_mutx_t mx, int ms)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	return (*pit->pf_mutex_lock)(mx, ms);
}

void xmutex_unlock(res_mutx_t mx)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_mutex_unlock)(mx);
}

#endif
////////////////////////////////////////////////////////////////////////////////////
#ifdef XDK_SUPPORT_THREAD_SEMAP

res_sema_t	xsemap_create(const tchar_t* mname, int max)
{
	if_thread_t* pit;
	res_sema_t mx;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	mx = (*pit->pf_semap_create)(mname, max);
	if (!mx)
	{
		set_system_error(_T("pf_semap_create"));
		return NULL;
	}

	return mx;
}

void xsemap_destroy(const tchar_t* mname, res_sema_t mx)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_semap_destroy)(mname, mx);
}

res_sema_t	xsemap_open(const tchar_t* mname)
{
	if_thread_t* pit;
	res_sema_t mx;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	mx = (void*)(*pit->pf_semap_open)(mname);
	if (!mx)
	{
		set_system_error(_T("pf_semap_open"));
		return NULL;
	}

	return mx;
}

wait_t xsemap_lock(res_sema_t mx, int ms)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	return (*pit->pf_semap_lock)(mx, ms);
}

void xsemap_unlock(res_sema_t mx)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_semap_unlock)(mx);
}

#endif

#ifdef XDK_SUPPORT_THREAD_QUEUE
res_queue_t xqueue_create(res_queue_t ep, res_file_t fd, int max)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	return (*pit->pf_queue_create)(ep, fd, max);
}

void xqueue_destroy(res_queue_t ep)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	(*pit->pf_queue_destroy)(ep);
}

wait_t xqueue_wait(res_queue_t ep, int ms)
{
	if_thread_t* pit;

	pit = PROCESS_THREAD_INTERFACE;

	XDL_ASSERT(pit != NULL);

	return (*pit->pf_queue_wait)(ep, ms);
}
#endif

#endif //XDK_SUPPORT_THREAD
