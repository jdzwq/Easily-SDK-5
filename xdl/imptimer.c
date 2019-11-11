/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc timer document

	@module	imptimer.c | implement file

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

#include "imptimer.h"
#include "xdlinit.h"
#include "xdlimp.h"

#ifdef XDK_SUPPORT_TIMER

res_hand_t xtimer_create_queue(void)
{
	if_timer_t* pif;

	pif = PROCESS_TIMER_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_create_timer_queue)();
}

void xtimer_destroy_queue(res_hand_t rq)
{
	if_timer_t* pif;

	pif = PROCESS_TIMER_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_destroy_timer_queue)(rq);
}

res_timer_t xtimer_create(res_hand_t rq, dword_t duetime, dword_t period, PF_TIMERFUNC pf, void* pa)
{
	if_timer_t* pif;

	pif = PROCESS_TIMER_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_create_timer)(rq, duetime, period, pf, pa);
}

void xtimer_destroy(res_hand_t rq, res_timer_t rt)
{
	if_timer_t* pif;

	pif = PROCESS_TIMER_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_destroy_timer)(rq, rt);
}

bool_t xtimer_alter(res_hand_t rq, res_timer_t rt, dword_t duetime, dword_t period)
{
	if_timer_t* pif;

	pif = PROCESS_TIMER_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_alter_timer)(rq, rt, duetime, period);
}

#endif /*XDK_SUPPORT_TIMER*/
