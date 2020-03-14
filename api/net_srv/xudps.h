/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc udp service defination document

	@module	xudps.h | udp service definition interface file

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


#ifndef _XUDPS_H
#define _XUDPS_H

#include "srvdef.h"

typedef struct _xudps_param_t{
	udp_listen_t*	lis_udp;

	tchar_t		sz_mode[INT_LEN];
	tchar_t		sz_port[INT_LEN];
	tchar_t		sz_module[PATH_LEN];
	tchar_t		sz_param[PATH_LEN];
}xudps_param_t;

void	_xudps_start(xudps_param_t* pxs);

void	_xudps_stop(xudps_param_t* pxs);

void	_xudps_dispatch(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size, void* p);

#endif //_XUDPS_H