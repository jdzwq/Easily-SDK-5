/***********************************************************************
	Easily pacs service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc pacs service defination document

	@module	xpacss.h | pacs service definition interface file

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


#ifndef _XPACSS_H
#define _XPACSS_H

#include "srvdef.h"

typedef struct _xpacss_param_t{
	pacs_listen_t* lis_pacs;

	int			n_secu;
	cert_t*		p_certs;

	tchar_t		sz_mode[INT_LEN];
	tchar_t		sz_port[INT_LEN];
	tchar_t		sz_root[PATH_LEN];
	tchar_t		sz_module[PATH_LEN];
	tchar_t		sz_param[PATH_LEN];
}xpacss_param_t;

void	_xpacss_start(xpacss_param_t* pxs);

void	_xpacss_stop(xpacss_param_t* pxs);

void	_xpacss_dispatch(xhand_t pacs, void* pxp);


#endif //_XPACSS_H