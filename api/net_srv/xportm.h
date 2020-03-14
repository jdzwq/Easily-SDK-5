/***********************************************************************
	Easily http service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc http monitor defination document

	@module	xportm.h | http monitor definition interface file

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


#ifndef _XPORTM_H
#define _XPORTM_H

#include <xdl.h>

typedef struct _xportm_param_t
{
	res_crit_t cr;
	res_even_t ev;
	bool_t act;

	xhand_t std;
	stream_t stm;
	xhand_t pip;
	tchar_t pname[RES_LEN + 1];
}xportm_param_t;

void _xportm_start(xportm_param_t* pxm);
void _xportm_stop(xportm_param_t* pxm);

#endif //_XPORTM_H