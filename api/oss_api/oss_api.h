/***********************************************************************
	Easily ali oss web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc oss defination document

	@module	ossapi.h | ali oss web api definition interface file

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

#ifndef _OSSAPI_H
#define _OSSAPI_H

#include <xdl.h>
#include <xds.h>
#include <oss.h>

extern int STDCALL https_invoke(const wchar_t* method, const https_block_t* pb);


#define OSS_IS_SUCCEED(code)	(xscmp(code,_T("0")) == 0)

#endif