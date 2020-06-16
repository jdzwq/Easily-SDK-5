/***********************************************************************
	Easily oau web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc oauth function document

	@module	oaudef.h | oauth interface file

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

#ifndef _OAUDEF_H
#define _OAUDEF_H

#include <xdl.h>

typedef struct _oau_head{
	sword_t dbt;
	sword_t cbs;
}oau_head, *oau_t;

typedef oau_t	OAU;

#ifdef	__cplusplus
extern "C" {
#endif

	extern oau_t STDCALL oau_open_isp(const tchar_t* ispfile);

	extern void STDCALL oau_close(oau_t oau);

	extern bool_t STDCALL oau_session(oau_t oau, const tchar_t* code, tchar_t* skey, tchar_t* oid, tchar_t* uid);

	extern int STDCALL oau_error(oau_t oau, tchar_t* buf, int max);

#ifdef	__cplusplus
}
#endif
/*********************************oau export def***************************************************/

typedef oau_t(STDCALL *PF_OAU_OPEN_ISP)(const tchar_t*);
typedef void(STDCALL *PF_OAU_CLOSE)(oau_t);
typedef bool_t(STDCALL *PF_OAU_SESSION)(oau_t, const tchar_t*, tchar_t*, tchar_t*, tchar_t*);
typedef int(STDCALL *PF_OAU_ERROR)(oau_t, tchar_t*, int);


#endif