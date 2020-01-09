/***********************************************************************
	Easily http service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tftps service defination document

	@module	xtftps.h | tftps service definition interface file

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


#ifndef _XTFTPSS_H
#define _XTFTPSS_H

#include "srvdef.h"

typedef struct _xtftps_param_t{
	tftp_listen_t*	lis_tftp;

	tchar_t		sz_mode[INT_LEN];
	tchar_t		sz_port[INT_LEN];
	tchar_t		sz_root[PATH_LEN];
	tchar_t		sz_module[PATH_LEN];
	tchar_t		sz_param[PATH_LEN];
}xtftps_param_t;

void	_xtftps_start(xtftps_param_t* pxs);

void	_xtftps_stop(xtftps_param_t* pxs);

void	_xtftps_invoke(xhand_t xh, void* pxp);

#endif //_XTFTPSS_H