/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc chart gdi document

	@module	chartgdi.h | chart gdi interface file

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

#ifndef _CHARTGDI_H
#define _CHARTGDI_H

#include "xdldef.h"
#include "matrix.h"
#include "vector.h"

#if defined(XDK_SUPPORT_CONTEXT)

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API void draw_map_table(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, float rx, float ry, const matrix_t* pma);

	XDL_API void draw_dot_table(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, float rx, float ry, const matrix_t* pma);

	XDL_API void draw_line_table(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, double base, double span, const vector_t* pvc);

	XDL_API void draw_bar_table(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, double base, double span, const vector_t* pvc);

	XDL_API void draw_pie_table(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, const vector_t* pvc);

	XDL_API void draw_counter_table(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, const tchar_t* layer, const tchar_t* num, int max);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*CHARTGDI_H*/