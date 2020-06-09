/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc port listen defination document

	@module	xportd.h | port listen definition interface file

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


#ifndef _XPORTD_H
#define _XPORTD_H

#include "srvdef.h"

#define XPORTD_PORT_TYPE_PACS		_T("PACS")
#define XPORTD_PORT_TYPE_HTTP		_T("HTTP")
#define XPORTD_PORT_TYPE_TCP		_T("TCP")
#define XPORTD_PORT_TYPE_UDP		_T("UDP")
#define XPORTD_PORT_TYPE_PNP		_T("PNP")

#define IS_THREAD_MODE(mode)		(compare_text(mode,-1,_T("thread"),-1,1) == 0)

#define XPORTD_STATE_STOPPED	0
#define XPORTD_STATE_RUNNING	1
#define XPORTD_STATE_PAUSED		2

typedef void(*PF_PORT_START)(void* pxp);
typedef void(*PF_PORT_STOP)(void* pxp);

typedef struct _xportd_param_t{
	void* param;

	PF_PORT_START	pf_start;
	PF_PORT_STOP	pf_stop;
	
}xportd_param_t;

void	xportd_start(void);

void	xportd_stop(void);

int		xportd_state(void);

#endif //_XPORTD_H