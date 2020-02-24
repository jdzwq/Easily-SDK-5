/***********************************************************************
	Easily http service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tcp service defination document

	@module	xtcps.h | tcp service definition interface file

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


#ifndef _XTCPS_H
#define _XTCPS_H

#include "srvdef.h"

typedef struct _xtcps_param_t{
	tcp_listen_t*	lis_tcp;

	tchar_t		sz_mode[INT_LEN];
	tchar_t		sz_port[INT_LEN];
	tchar_t		sz_root[PATH_LEN];
	tchar_t		sz_module[PATH_LEN];
	tchar_t		sz_param[PATH_LEN];
}xtcps_param_t;

void	_xtcps_start(xtcps_param_t* pxs);

void	_xtcps_stop(xtcps_param_t* pxs);

void	_xtcps_dispatch(xhand_t tcp, void* pxp);

#endif //_XTCPS_H