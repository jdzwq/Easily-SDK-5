/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc svg document

	@module	svgdoc.h | svg document interface file

	@devnote 张文权 2003.01 - 2007.12 v3.0
	@devnote 张文权 2008.01 - 2009.12 v4.0
*************************************************************/

#ifndef _SVGDRAW_H
#define _SVGDRAW_H

#include "xdldef.h"
#include "xdlctx.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API void write_line_to_svg_node(link_t_ptr glk,const xpoint_t* prt1,const xpoint_t* prt2);

XDL_API void read_line_from_svg_node(link_t_ptr glk,xpoint_t* prt1,xpoint_t* prt2);

XDL_API void write_line_to_svg_node(link_t_ptr glk,const xpoint_t* prt1,const xpoint_t* prt2);

XDL_API void read_line_from_svg_node(link_t_ptr glk,xpoint_t* prt1,xpoint_t* prt2);

XDL_API void write_polyline_to_svg_node(link_t_ptr glk,const xpoint_t* ppt,int n);

XDL_API int read_polyline_from_svg_node(link_t_ptr glk,xpoint_t* ppt,int n);

XDL_API void write_polygon_to_svg_node(link_t_ptr glk,const xpoint_t* ppt,int n);

XDL_API int read_polygon_from_svg_node(link_t_ptr glk,xpoint_t* ppt,int n);

XDL_API void write_rect_to_svg_node(link_t_ptr glk,const xrect_t* prt);

XDL_API void read_rect_from_svg_node(link_t_ptr glk,xrect_t* prt);

XDL_API void write_ellipse_to_svg_node(link_t_ptr glk,const xrect_t* prt);

XDL_API void read_ellipse_from_svg_node(link_t_ptr glk,xrect_t* prt);

XDL_API void write_round_to_svg_node(link_t_ptr glk,const xrect_t* prt);

XDL_API void read_round_from_svg_node(link_t_ptr glk,xrect_t* prt);

XDL_API void write_text_to_svg_node(link_t_ptr glk,const xrect_t* prt,const wchar_t* text,int len);

XDL_API const wchar_t* read_text_from_svg_node(link_t_ptr glk,xrect_t* prt);

XDL_API void write_shape_to_svg_node(link_t_ptr glk,const xrect_t* prt,const wchar_t* shape);

XDL_API void draw_svg_node(const context_t* pct,link_t_ptr glk);

XDL_API void draw_svg_doc(const context_t* pct,link_t_ptr ptr);

#ifdef __cplusplus
}
#endif

#endif /*_SVGDRAW_H*/