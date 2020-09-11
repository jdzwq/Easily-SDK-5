/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg gizmo document

	@module	svggizmo.h | interface file

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

#ifndef _SVGGIZMO_H
#define _SVGIZMO_H

#include "xdldef.h"

#if defined(XDL_SUPPORT_SVG)

typedef void(*PF_SVG_GIZMO_MAKE)(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt);

typedef struct _SVG_GIZMO_DRAW_TABLE{
	tchar_t gizmo_name[32];
	PF_SVG_GIZMO_MAKE gizmo_func;
}SVG_GIZMO_DRAW_TABLE;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API PF_SVG_GIZMO_MAKE svg_find_gizmo_maker(const tchar_t* iname);

	/*
	@FUNCTION svg draw_gizmo: svg draw gizmo icon in canvas using millimeter coordinate.
	@INPUT canvas_t canv: the canvas object.
	@INPUT const xcolor_t* pxc: the color struct.
	@INPUT const xrect_t* pxr: the rect struct using float member.
	@INPUT const tchar_t* iname: the icon name.
	@RETURN void: none.
	*/
	EXP_API void	svg_draw_gizmo(canvas_t canv, const xcolor_t* pxc, const xrect_t* pxr, const tchar_t* iname);


#ifdef	__cplusplus
}
#endif


#endif /*XDL_SUPPORT_SVG*/

#endif /*SVGGDI_H*/