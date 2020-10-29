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


#ifndef _XTIMERS_H
#define _XTIMERS_H

#include "srvdef.h"

typedef struct _xtimers_param_t{
	tchar_t	task[RES_LEN + 1];
}xtimers_param_t;

void	_calc_duretime(timer_hint* pth, xdate_t* pdate, dword_t* pdure);

void	xtimers_dispatch(xtimers_param_t* pts);


#endif //_XTIMERS_H