/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc title document

	@module	svgdoc.h | svg document interface file

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

#ifndef _SVGDOC_H
#define _SVGDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#define set_svg_node_name(nlk,token)						set_dom_node_name(nlk,token,-1)
#define get_svg_node_name_ptr(nlk)							get_dom_node_name_ptr(nlk)

#define set_svg_node_attr(nlk,key,val)						set_dom_node_attr(nlk,key,-1,val,-1)
#define get_svg_node_attr_ptr(nlk,key)						get_dom_node_attr_ptr(nlk,key)

#define set_svg_node_text(nlk,token,len)					set_dom_node_text(nlk,token,len)
#define get_svg_node_text_ptr(nlk)							get_dom_node_text_ptr(nlk)

/***************************************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_svg_doc(void);

XDL_API void destroy_svg_doc(link_t_ptr ptr);

XDL_API void clear_svg_doc(link_t_ptr ptr);

XDL_API bool_t is_svg_doc(link_t_ptr ptr);

XDL_API bool_t is_svg_node(link_t_ptr ptr, link_t_ptr nlk);

XDL_API link_t_ptr insert_svg_node(link_t_ptr nlk);

XDL_API void delete_svg_node(link_t_ptr nlk);

XDL_API link_t_ptr get_svg_child_node(link_t_ptr nlk);

XDL_API link_t_ptr get_svg_parent_node(link_t_ptr nlk);

XDL_API link_t_ptr get_svg_next_sibling_node(link_t_ptr nlk);

XDL_API link_t_ptr get_svg_prev_sibling_node(link_t_ptr nlk);

XDL_API void set_svg_width(link_t_ptr ptr, float width);

XDL_API float get_svg_width(link_t_ptr ptr);

XDL_API void set_svg_height(link_t_ptr ptr, float height);

XDL_API float get_svg_height(link_t_ptr ptr);

XDL_API void set_svg_viewbox(link_t_ptr ptr, const xrect_t* pbox);

XDL_API void get_svg_viewbox(link_t_ptr ptr, xrect_t* pbox);

XDL_API void write_line_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

XDL_API void read_line_from_svg_node(link_t_ptr glk, xpen_t* pxp, xpoint_t* ppt1, xpoint_t* ppt2);

XDL_API void write_polyline_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xpoint_t* ppt, int n);

XDL_API int read_polyline_from_svg_node(link_t_ptr glk, xpen_t* pxp, xpoint_t* ppt, int n);

XDL_API void write_polygon_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

XDL_API int read_polygon_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xpoint_t* ppt, int n);

XDL_API void write_rect_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, bool_t b_round);

XDL_API bool_t read_rect_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* prt);

XDL_API void write_ellipse_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt);

XDL_API void read_ellipse_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* prt);

XDL_API bool_t svg_node_is_pie(link_t_ptr glk);

XDL_API void write_pie_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, double fang, double tang);

XDL_API void read_pie_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* pxr, double* pfang, double* ptang);

XDL_API void write_text_to_svg_node(link_t_ptr glk, const xfont_t* pxf, const xface_t* pxa, const xrect_t* prt, const tchar_t* text, int len);

XDL_API const tchar_t* read_text_from_svg_node(link_t_ptr glk, xfont_t* pxf, xface_t* pxa, xrect_t* prt);

XDL_API void write_shape_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, const tchar_t* shape);

XDL_API void write_path_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, link_t_ptr path);

XDL_API void read_path_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, link_t_ptr path);

XDL_API void write_ximage_to_svg_node(link_t_ptr glk, const ximage_t* pxi, const xrect_t* prt);

XDL_API void read_ximage_from_svg_node(link_t_ptr glk, ximage_t* pxi, xrect_t* prt);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_SVGDOC_H*/