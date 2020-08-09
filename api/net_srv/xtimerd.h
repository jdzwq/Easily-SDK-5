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

#include <xdl.h>

#define XSERVICE_ROOT			_T("XSERVICE_ROOT")
#define XSERVICE_DATA			_T("XSERVICE_DATA")

#define XTIMERD_STATE_STOPPED	0
#define XTIMERD_STATE_RUNNING	1
#define XTIMERD_STATE_PAUSED	2

#define XTIMERD_ATTR_SCHEDULE	_T("schedule")
#define XTIMERD_ATTR_MODULE		_T("module")
#define XTIMERD_ATTR_PARAM		_T("param")
#define XTIMERD_ATTR_DUETIME	_T("duetime")
#define XTIMERD_ATTR_PERIOD		_T("period")

typedef struct _xtimerd_param_t{
	res_modu_t module;
	dword_t duetime;
	dword_t period;
	res_timer_t timer;
	
	tchar_t param[PATH_LEN];
}xtimerd_param_t;

void	xtimerd_start(void);

void	xtimerd_stop(void);

int		xtimerd_state(void);

#endif //_XTIMERD_H