/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc draw document

	@module	xdcdraw.h | draw interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#ifndef _XDCDRAW_H
#define _XDCDRAW_H

#include "xdcdef.h"

XDC_API void	draw_line(xhand_t canv, const xpen_t* pxp, long x1, long y1, long x2, long y2);

XDC_API void	draw_3dline(xhand_t canv, const xpen_t* pxp, long x1, long y1, long x2, long y2);

XDC_API void	draw_polyline(xhand_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);

XDC_API void	draw_polygon(xhand_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);

XDC_API void	fill_polygon(xhand_t canv, const xbrush_t* pxb, const xpoint_t* ppt, int n);

XDC_API void	draw_3drect(xhand_t canv, const xpen_t* pxp, const xrect_t* prt);

XDC_API void	draw_rect(xhand_t canv, const xpen_t* pxp, const xrect_t* pxr);

XDC_API void	fill_rect(xhand_t canv, const xbrush_t* pxb, const xrect_t* pxr);

XDC_API void	gradient_rect(xhand_t canv, const xgradi_t* pxg, const xrect_t* pxr);

XDC_API void	draw_round(xhand_t canv, const xpen_t* pxp, const xrect_t* pxr);

XDC_API void	fill_round(xhand_t canv, const xbrush_t* pxb, const xrect_t* pxr);

XDC_API void	draw_ellipse(xhand_t canv, const xpen_t* pxp, const xrect_t* pxr);

XDC_API void	fill_ellipse(xhand_t canv, const xbrush_t* pxb, const xrect_t* pxr);

XDC_API void	draw_pie(xhand_t canv, const xpen_t* pxp, const xrect_t* pxr, double fang, double tang);

XDC_API void	fill_pie(xhand_t canv, const xbrush_t* pxb, const xrect_t* pxr, double fang, double tang);

XDC_API void	draw_arrow(xhand_t canv, const xpen_t* pxp, const xrect_t* pxr, int alen, double arc);

XDC_API void	fill_arrow(xhand_t canv, const xbrush_t* pxb, const xrect_t* pxr, int alen, double arc);

XDC_API void	draw_shape(xhand_t canv, const xpen_t* pxp, const xrect_t* pxr, const wchar_t* shape);

XDC_API void	fill_shape(xhand_t canv, const xbrush_t* pxb, const xrect_t* pxr, const wchar_t* shape);

XDC_API void	draw_text(xhand_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const wchar_t* txt, int len);

XDC_API void	calc_text(xhand_t canv, const xfont_t* pxf, const xface_t* pxa, const wchar_t* txt, int len, xrect_t* pxr);

XDC_API void	draw_image(xhand_t canv, const ximage_t* pxi, const xrect_t* pxr);

XDC_API void	draw_collapse(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_expand(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_insert(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_delete(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_up(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_down(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_guider(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_folder(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_check(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_close(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_sum(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size);

XDC_API void	draw_select(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, byte_t opa);

XDC_API void	draw_focus(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, byte_t opa);

XDC_API void	draw_alpha(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, byte_t opa);

#endif