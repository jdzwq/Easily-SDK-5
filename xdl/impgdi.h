/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdi document

	@module	impgdi.h | gdi interface file

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

#ifndef _IMPGDI_H
#define _IMPGDI_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_CONTEXT_GRAPHIC)

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void	draw_line_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

XDL_API void	draw_3dline_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

XDL_API void	draw_3drect_raw(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt);

XDL_API void	draw_polyline_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n);

XDL_API void	draw_polygon_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

XDL_API void	draw_rect_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	gradient_rect_raw(res_ctx_t rdc, const xgradi_t* pxg, const xrect_t* pxr);

XDL_API void	alpha_rect_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity);

XDL_API void	draw_round_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	draw_ellipse_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	draw_pie_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, double fang, double tang);

XDL_API void	draw_arc_raw(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* pxr, double fang, double tang);

XDL_API void	draw_arrow_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, int alen, double arc);

XDL_API void	draw_shape_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* shape);

XDL_API void	multi_line_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr);

XDL_API void	draw_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

XDL_API void	text_out_raw(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);

XDL_API void	color_out_raw(res_ctx_t rdc, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len);

XDL_API void	draw_var_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var, int page);

XDL_API void	draw_tag_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag, int page);

XDL_API void	draw_memo_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo, int page);

XDL_API int		calc_memo_pages_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo);

XDL_API void	draw_rich_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich, int page);

XDL_API int		calc_rich_pages_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich);

XDL_API void	draw_image_raw(res_ctx_t rdc, const ximage_t* pxi, const xrect_t* pxr);

XDL_API void	draw_bitmap_raw(res_ctx_t rdc, res_bmp_t bmp, const xrect_t* pxr);

XDL_API void	draw_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr, const tchar_t* iname);

XDL_API void	fill_region_raw(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn);

XDL_API void	draw_select_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep);

XDL_API void	draw_focus_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep);

XDL_API void	draw_feed_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep);

XDL_API void	exclip_rect_raw(res_ctx_t rdc, const xrect_t* pxr);

XDL_API void	inclip_rect_raw(res_ctx_t rdc, const xrect_t* pxr);

XDL_API void	text_rect_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr);

XDL_API void	text_size_raw(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

XDL_API void	text_metric_raw(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs);

XDL_API void	image_size_raw(res_ctx_t rdc, const ximage_t* pxi, xsize_t* pxs);

/*******************************************************************************************************************/

XDL_API void	draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

XDL_API void	draw_3dline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

XDL_API void	draw_3drect(canvas_t canv, const xpen_t* pxp, const xrect_t* prt);

XDL_API void	draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);

XDL_API void	draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

XDL_API void	draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	gradient_rect(canvas_t canv, const xgradi_t* pxg, const xrect_t* pxr);

XDL_API void	alpha_rect(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int opa);

XDL_API void	draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

XDL_API void	draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, double fang, double tang);

XDL_API void	draw_arc(canvas_t canv, const xpen_t* pxp, const xrect_t* pxr, double fang, double tang);

XDL_API void	draw_arrow(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, int alen, double arc);

XDL_API void	draw_shape(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* shape);

XDL_API void	multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr);

XDL_API void	draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

XDL_API void	text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);

XDL_API void	color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len);

XDL_API void	draw_password(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

XDL_API void	draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap);

XDL_API void	draw_var_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var, int page);

XDL_API void	draw_tag_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag, int page);

XDL_API void	draw_memo_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo, int page);

XDL_API int		calc_memo_pages(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo);

XDL_API void	draw_rich_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich, int page);

XDL_API int		calc_rich_pages(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich);

XDL_API void	draw_button(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* iname);

XDL_API void	draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr);

XDL_API void	draw_bitmap(canvas_t canv, res_bmp_t bmp, const xrect_t* pxr);

XDL_API void	draw_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* pxr, const tchar_t* iname);

XDL_API void	draw_progress(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int steps);

XDL_API void	draw_ruler(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_svg(canvas_t canv, const xrect_t* prt, link_t_ptr svg);

XDL_API void	draw_corner(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_shadow(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void	draw_select(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep);

XDL_API void	draw_focus(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep);

XDL_API void	draw_feed(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep);

XDL_API void	exclip_rect(canvas_t canv, const xrect_t* pxr);

XDL_API void	inclip_rect(canvas_t canv, const xrect_t* pxr);

XDL_API void	text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr);

XDL_API void	text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxr);

XDL_API void	text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs);

XDL_API void	image_size(canvas_t canv, const ximage_t* pxi, xsize_t* pxs);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT_GRAPHIC*/

#endif /*IMPGDI_H*/