/***********************************************************************
	Easily slot service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc slot service defination document

	@module	xslots.h | slot service definition interface file

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


#ifndef _XSLOTS_H
#define _XSLOTS_H

#include "srvdef.h"
#include "srvcert.h"

typedef struct _xslots_param_t{
	slot_listen_t* lis_slot;

	int			n_secu;
	cert_t*		p_certs;

	tchar_t		sz_mode[INT_LEN];
	tchar_t		sz_port[INT_LEN];
	tchar_t		sz_root[PATH_LEN];
	tchar_t		sz_module[PATH_LEN];
	tchar_t		sz_param[PATH_LEN];
}xslots_param_t;

void	_xslots_start(xslots_param_t* pxs);

void	_xslots_stop(xslots_param_t* pxs);

void	_xslots_dispatch(xhand_t slot, void* pxp);


#endif //_XSLOTS_H