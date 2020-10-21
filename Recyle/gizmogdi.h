/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gizmo document

	@module	gizmogdi.h | interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#ifndef _GIZMOGDI_H
#define _GIZMOGDI_H

#include "xdcdef.h"

#if defined(XDU_SUPPORT_CONTEXT)

typedef void(*PF_GIZMO_MAKE)(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

typedef struct _GIZMO_DRAW_TABLE{
	tchar_t gizmo_name[32];
	PF_GIZMO_MAKE gizmo_func;
}GIZMO_DRAW_TABLE;

#ifdef	__cplusplus
extern "C" {
#endif

EXP_API PF_GIZMO_MAKE find_gizmo_maker(const tchar_t* iname);

EXP_API void draw_logo_gizmo_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

EXP_API void draw_close_gizmo_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

EXP_API void draw_maximize_gizmo_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

EXP_API void draw_minimize_gizmo_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

EXP_API void draw_restore_gizmo_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

#if defined(_DEBUG) || defined(DEBUG)

EXP_API void test_gizmo_draw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

#endif 


#ifdef	__cplusplus
}
#endif

#endif

#endif /*GIZMOGDI_H*/