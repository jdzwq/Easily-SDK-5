/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc timer document

	@module	imptimer.h | interface file

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

#ifndef _IMPTIMER_H
#define _IMPTIMER_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_TIMER

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_timer_queue: create timer queue, every queue can manage multiple timer.
@RETURN res_queue_t: if succeeds return queue resource handle, fails return NULL.
*/
EXP_API res_queue_t create_timer_queue(void);

/*
@FUNCTION destroy_timer_queue: destroy timer queue and free all timer in queue.
@INPUT res_queue_t rq: the timer queue resource handle.
@RETURN void: none.
*/
EXP_API void destroy_timer_queue(res_queue_t rq);

/*
@FUNCTION create_timer: create timer and add into queue.
@INPUT res_queue_t rq: the timer queue resource handle.
@INPUT dword_t duetime: amount of time to elapse before the timer is signed for the first time in milliseconds.
@INPUT dword_t period: period of the timer, in milliseconds. if zero, the timer is signed once or the timer is periodic by the period value.
@INPUT PF_TIMERFUNC pf: the worker timer function.
@INPUT void* pa: the worker function parameter.
@RETURN res_timer_t: if succeeds return timer resource hanle, fails return NULL.
*/
EXP_API res_timer_t create_timer(res_queue_t rq, dword_t duetime, dword_t period, PF_TIMERFUNC pf, void* pa);

/*
@FUNCTION destroy_timer: destroy timer and remove from queue.
@INPUT res_queue_t rq: the timer queue resource handle.
@INPUT res_timer_t rt: the timer resource handle.
@RETURN void: none.
*/
EXP_API void destroy_timer(res_queue_t rq, res_timer_t rt);

/*
@FUNCTION alter_timer: change timer time setting.
@INPUT res_queue_t rq: the timer queue resource handle.
@INPUT res_timer_t rt: the timer resource handle.
@INPUT dword_t duetime: amount of time to elapse before the timer is signed for the first time in milliseconds.
@INPUT dword_t period: period of the timer, in milliseconds.
if zero, the timer is signed once. the timer is periodic by the period value.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t alter_timer(res_queue_t rq, res_timer_t rt, dword_t duetime, dword_t period);

#ifdef	__cplusplus
}
#endif

typedef struct _timer_block_t{
	trace_interface* ptk;

	tchar_t name[RES_LEN + 1];
	tchar_t path[PATH_LEN + 1];
}timer_block_t;

typedef enum{
	TIMER_INVOKE_SUCCEED = 0,
	TIMER_INVOKE_WITHINFO = 1,
	TIMER_INVOKE_WITHERROR = 2,
	TIMER_INVOKE_PENDING = 100
}TIMER_INVOKE_STATE;

typedef int(STDCALL *PF_TIMER_INVOKE)(const timer_block_t* pt);


#endif /*XDK_SUPPORT_TIMER*/

#endif /*_IMPTIMER_H*/
