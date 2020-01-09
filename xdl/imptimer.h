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
@FUNCTION timer_create_queue: create timer queue, every queue can manage multiple timer.
@RETURN res_hand_t: if succeeds return queue resource handle, fails return NULL.
*/
XDL_API res_hand_t timer_create_queue(void);

/*
@FUNCTION timer_destroy_queue: destroy timer queue and free all timer in queue.
@INPUT res_hand_t rq: the timer queue resource handle.
@RETURN void: none.
*/
XDL_API void timer_destroy_queue(res_hand_t rq);

/*
@FUNCTION timer_create: create timer and add into queue.
@INPUT res_hand_t rq: the timer queue resource handle.
@INPUT dword_t duetime: amount of time to elapse before the timer is signed for the first time in milliseconds.
@INPUT dword_t period: period of the timer, in milliseconds. 
if zero, the timer is signed once. the timer is periodic by the period value.
@INPUT PF_TIMERFUNC pf: the worker timer function.
@INPUT void* pa: the worker function parameter.
@RETURN res_timer_t: if succeeds return timer resource hanle, fails return NULL.
*/
XDL_API res_timer_t timer_create(res_hand_t rq, dword_t duetime, dword_t period, PF_TIMERFUNC pf, void* pa);

/*
@FUNCTION timer_destroy: destroy timer and remove from queue.
@INPUT res_hand_t rq: the timer queue resource handle.
@INPUT res_timer_t rt: the timer resource handle.
@RETURN void: none.
*/
XDL_API void timer_destroy(res_hand_t rq, res_timer_t rt);

/*
@FUNCTION timer_alter: change timer time setting.
@INPUT res_hand_t rq: the timer queue resource handle.
@INPUT res_timer_t rt: the timer resource handle.
@INPUT dword_t duetime: amount of time to elapse before the timer is signed for the first time in milliseconds.
@INPUT dword_t period: period of the timer, in milliseconds.
if zero, the timer is signed once. the timer is periodic by the period value.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t timer_alter(res_hand_t rq, res_timer_t rt, dword_t duetime, dword_t period);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_TIMER*/

#endif /*_IMPTIMER_H*/
