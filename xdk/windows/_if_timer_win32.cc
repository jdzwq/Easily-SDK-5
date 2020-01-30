/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc timer system call document

	@module	_if_timer.c | windows implement file

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

#ifdef XDK_SUPPORT_TIMER

res_queue_t _create_timer_queue()
{
	return CreateTimerQueue();
}

void _destroy_timer_queue(res_queue_t rq)
{
	DeleteTimerQueue(rq);
}

res_timer_t _create_timer(res_queue_t rq, clock_t duetime, clock_t period, PF_TIMERFUNC pf, void* pa)
{
	res_timer_t hTimer = NULL;

	if (CreateTimerQueueTimer(&hTimer, rq, (WAITORTIMERCALLBACK)pf, (void*)pa, duetime, period, 0))
		return hTimer;
	else
		return NULL;
}

void _destroy_timer(res_queue_t rq, res_timer_t rt)
{
	DeleteTimerQueueTimer(rq, rt, INVALID_HANDLE_VALUE);
}

bool_t _alter_timer(res_queue_t rq, res_timer_t rt, clock_t duetime, clock_t period)
{
	return (bool_t)ChangeTimerQueueTimer(rq, rt, duetime, period);
}

#endif //XDK_SUPPORT_TIMER