/***********************************************************************
	Easily timer service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc timer listen defination document

	@module	xtimerd.h | timer listen definition interface file

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


#ifndef _XTIMERD_H
#define _XTIMERD_H

#include "srvdef.h"

typedef struct _xtimerd_param_t{
	res_timer_t timer;
	
	timer_hint hint;
	tchar_t mode[INT_LEN + 1];
	tchar_t module[PATH_LEN + 1];
	tchar_t task[RES_LEN + 1];
}xtimerd_param_t;

EXP_API res_queue_t g_queue;

void	xtimerd_start(void);

void	xtimerd_stop(void);

int		xtimerd_state(void);

#endif //_XTIMERD_H