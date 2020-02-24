/***********************************************************************
	Easily http service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc ssl service defination document

	@module	xssls.h | ssl service definition interface file

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


#ifndef _XSSLS_H
#define _XSSLS_H

#include "srvdef.h"

typedef struct _xssls_param_t{
	ssl_listen_t*	lis_ssl;

	cert_t*		p_certs;

	tchar_t		sz_mode[INT_LEN];
	tchar_t		sz_port[INT_LEN];
	tchar_t		sz_root[PATH_LEN];
	tchar_t		sz_module[PATH_LEN];
	tchar_t		sz_param[PATH_LEN];
}xssls_param_t;

void	_xssls_start(xssls_param_t* pxs);

void	_xssls_stop(xssls_param_t* pxs);

void	_xssls_dispatch(xhand_t ssl, void* pxp);

#endif //_XSSLS_H