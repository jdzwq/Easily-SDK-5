/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc http service defination document

	@module	xhttps.h | http service definition interface file

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


#ifndef _XHTTPS_H
#define _XHTTPS_H

#include "srvdef.h"


#define XHTTPS_DEFAULT_SITE		_T("www")

typedef struct _xhttps_param_t{
	http_listen_t* lis_http;

	int			n_secu;

	tchar_t		sz_mode[INT_LEN];
	tchar_t		sz_port[INT_LEN];
	tchar_t		sz_module[PATH_LEN];
	tchar_t		sz_param[PATH_LEN];
}xhttps_param_t;

void	_xhttps_start(xhttps_param_t* pxs);

void	_xhttps_stop(xhttps_param_t* pxs);

void	_xhttps_dispatch(xhand_t http, void* pxp);


#endif //_XHTTPS_H