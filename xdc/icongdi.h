/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc icon document

	@module	icongdi.h | interface file

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

#ifndef _ICONGDI_H
#define _ICONGDI_H

#include "xdcdef.h"

#if defined(XDU_SUPPORT_CONTEXT_GRAPHIC)

typedef void(*PF_GDI_ICON_MAKE)(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

typedef struct _GDI_ICON_DRAW_TABLE{
	tchar_t icon_name[32];
	PF_GDI_ICON_MAKE icon_func;
}GDI_ICON_DRAW_TABLE;

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API PF_GDI_ICON_MAKE find_icon_maker(const tchar_t* iname);

XDC_API void draw_logo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDC_API void draw_close_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDC_API void draw_maximize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDC_API void draw_minimize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDC_API void draw_restore_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

#if defined(_DEBUG) || defined(DEBUG)

XDC_API void test_icon_draw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

#endif 


#ifdef	__cplusplus
}
#endif

#endif

#endif /*ICONGDI_H*/