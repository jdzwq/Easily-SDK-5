﻿/***********************************************************************
	Easily coap web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc coap defination document

	@module	coapapi.h | coap web api definition interface file

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

#ifndef _COAPAPI_H
#define _COAPAPI_H

#include <xdl.h>
#include <hdfs.h>

#ifdef	__cplusplus
extern "C" {
#endif

	extern int STDCALL udps_invoke(const udps_block_t* pb);

#ifdef	__cplusplus
}
#endif

#endif