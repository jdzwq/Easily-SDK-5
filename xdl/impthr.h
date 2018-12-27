/***********************************************************************
Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc thread document

	@module	impthr.h | thread interface file

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

#ifndef _IMPTHR_H
#define _IMPTHR_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_THREAD

#define THREAD_BASE_TMO		500

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void xthread_begin(res_hand_t* ph_hand, PF_THREADFUNC pf_worker, void* param);

XDL_API void xthread_end(void);

XDL_API void xthread_sleep(int ms);

XDL_API dword_t xthread_get_id(void);

XDL_API void xthread_join(res_hand_t th);

#ifdef XDK_SUPPORT_THREAD_EVENT

XDL_API res_even_t xevent_create(void);

XDL_API void xevent_destroy(res_even_t eh);

XDL_API wait_t xevent_wait(res_even_t eh, int milsec);

XDL_API void xevent_sign(res_even_t eh, bool_t b_sign);

#endif

#ifdef XDK_SUPPORT_THREAD_CRITI

XDL_API res_crit_t xcriti_create();

XDL_API void xcriti_destroy(res_crit_t cr);

XDL_API void xcriti_enter(res_crit_t cr);

XDL_API void xcriti_leave(res_crit_t cr);

XDL_API bool_t xcriti_query(res_crit_t cr);

#endif

#ifdef XDK_SUPPORT_THREAD_MUTEX

XDL_API res_mutx_t	xmutex_create(const tchar_t* mname);

XDL_API void xmutex_destroy(const tchar_t* mname, res_mutx_t mx);

XDL_API res_mutx_t	xmutex_open(const tchar_t* mname);

XDL_API wait_t xmutex_lock(res_mutx_t mx, int milsec);

XDL_API void xmutex_unlock(res_mutx_t mx);

#endif

#ifdef XDK_SUPPORT_THREAD_SEMAP

XDL_API res_sema_t	xsemap_create(const tchar_t* mname, int max);

XDL_API void xsemap_destroy(const tchar_t* mname, res_sema_t mx);

XDL_API res_sema_t	xsemap_open(const tchar_t* mname);

XDL_API wait_t xsemap_lock(res_sema_t mx, int milsec);

XDL_API void xsemap_unlock(res_sema_t mx);

#endif

#ifdef XDK_SUPPORT_THREAD_QUEUE
XDL_API res_hand_t xqueue_create(res_hand_t ep, res_file_t fd, int max);

XDL_API void xqueue_destroy(res_hand_t ep);

XDL_API wait_t queue_wait(res_hand_t ep, int ms);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_THREAD*/

#endif /*IMPTHR_H*/