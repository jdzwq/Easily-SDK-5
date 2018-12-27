/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg gdi document

	@module	svggdi.h | svg gdi interface file

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

#ifndef _SVGGDI_H
#define _SVGGDI_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_CONTEXT)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void	svg_draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

XDL_API void	svg_draw_3dline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

XDL_API void	svg_draw_3drect(canvas_t canv, const xpen_t* pxp, const xrect_t* prt);

XDL_API void	svg_draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);

XDL_API void	svg_draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

XDL_API void	svg_draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	svg_gradient_rect(canvas_t canv, const xgradi_t* pxg, const xrect_t* pxr);

XDL_API void	svg_alphablend_rect(canvas_t canv, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	svg_draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	svg_draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	svg_draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, double fang, double tang);

XDL_API void	svg_draw_arrow(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, int alen, double arc);

XDL_API void	svg_draw_shape(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* shape);

XDL_API void	svg_multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr);

XDL_API void	svg_draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

XDL_API void	svg_draw_password(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

XDL_API void	svg_draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap);

XDL_API void	svg_draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr);

XDL_API void	svg_color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len);

XDL_API void	svg_text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);

XDL_API void	svg_draw_fix_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* text, int len);

XDL_API void	svg_draw_var_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t data, int page);

XDL_API void	svg_draw_tag_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

XDL_API void	svg_draw_memo_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

XDL_API void	svg_draw_rich_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

XDL_API void	svg_draw_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* iname);

XDL_API void	svg_text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr);

XDL_API void	svg_text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

XDL_API void	svg_text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*SVGGDI_H*/