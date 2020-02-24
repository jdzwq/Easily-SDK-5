/***********************************************************************
	Easily radhex web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc radhex defination document

	@module	rad_api.h | radhex web api definition interface file
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

#ifndef _RADAPI_H
#define _RADAPI_H

#include <xdl.h>
#include <xds.h>
#include <radhex.h>
#include <hexdfs.h>

#ifdef	__cplusplus
extern "C" {
#endif

	extern int STDCALL slots_invoke(const slots_block_t* pb);

#ifdef	__cplusplus
}
#endif


#endif