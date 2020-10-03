/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg gdi document

	@module	svggdi.h | interface file

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

#if defined(XDL_SUPPORT_SVG)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION svg_draw_line: draw line in svg canvas using milimeter coordinate
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the from point struct using float member.
@INPUT const xpoint_t* ppt2: the to point struct using float member.
@RETURN void: none.
*/
EXP_API void	svg_draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);
EXP_API void	svg_draw_line_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

/*
@FUNCTION svg_draw_bezier: draw bezier in svg canvas using millimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the start point using float member.
@INPUT const xpoint_t* ppt2: the control point using float member.
@INPUT const xpoint_t* ppt3: the control point using float member.
@INPUT const xpoint_t* ppt4: the end point using float member.
@RETURN void: none.
*/
EXP_API void	svg_draw_bezier(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4);
EXP_API void	svg_draw_bezier_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4);

/*
@FUNCTION svg_draw_curve: draw curve in svg canvas using millimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point array.
@INPUT int n: the point array size.
@RETURN void: none.
*/
EXP_API void	svg_draw_curve(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);
EXP_API void	svg_draw_curve_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION svg_draw_arc : draw arc in svg canvas using millimeter coordinate.
@INPUT canvas_t canv : the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point struct using millimeter member.
@INPUT float rx : the x - radius
@INPUT float ry : the y - radius
@INPUT double fang : the from angle PI value.
@INPUT double tang : the to angle PI value.
@RETURN void: none.
*/
EXP_API void	svg_draw_arc(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, float rx, float ry, double fang, double tang);
EXP_API void	svg_draw_arc_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt, int rx, int ry, double fang, double tang);

/*
@FUNCTION svg_draw_polyline: draw polyline in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point struct array using float member.
@INPUT int n: the point entity count.
@RETURN void: none.
*/
EXP_API void	svg_draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);
EXP_API void	svg_draw_polyline_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION svg_draw_shape: draw shape in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* shape: the shape name, eg: ATTR_SHAPE_*.
@RETURN void: none.
*/
EXP_API void	svg_draw_shape(canvas_t canv, const xpen_t* pxp, const xrect_t* pxr, const tchar_t* shape);
EXP_API void	svg_draw_shape_raw(link_t_ptr g, const xpen_t* pxp, const xrect_t* pxr, const tchar_t* shape);

/*
@FUNCTION svg_draw_triangle: draw triangle in canvas using milimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* orient: the triangle orientation
@RETURN void: none.
*/
EXP_API void	svg_draw_triangle(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* orient);
EXP_API void	svg_draw_triangle_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* orient);

/*
@FUNCTION draw_rect: draw svg_draw_rect in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	svg_draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);
EXP_API void	svg_draw_rect_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION svg_draw_round: draw round rect in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	svg_draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);
EXP_API void	svg_draw_round_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION svg_draw_ellipse: draw ellipse in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	svg_draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);
EXP_API void	svg_draw_ellipse_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION svg_draw_pie: draw pie in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct using millimeter member.
@INPUT float rx: the x-radius
@INPUT float ry: the y-radius
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@RETURN void: none.
*/
EXP_API void	svg_draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float rx, float ry, double fang, double tang);
EXP_API void	svg_draw_pie_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int rx, int ry, double fang, double tang);

/*
@FUNCTION svg_draw_arrow: draw arrrow in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT float alen: the arrow length in millimeter.
@INPUT double arc: the arrow angle PI value.
@RETURN void: none.
*/
EXP_API void	svg_draw_arrow(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, float alen, double arc);
EXP_API void	svg_draw_arrow_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, float alen, double arc);

/*
@FUNCTION svg_calc_fan: draw fan in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpoint_t* pxr: the point struct using float member.
@INPUT float r: the radius
@INPUT float s: the span
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@OUTPUT xpoint_t* pa: return the fan points.
@INPUT int n: the point array size.
@RETURN void: none.
*/
EXP_API void	svg_calc_fan(canvas_t canv, const xpoint_t* ppt, float r, float s, double fang, double tang, xpoint_t* pa, int n);
EXP_API void	svg_calc_fan_raw(link_t_ptr g, const xpoint_t* ppt, int r, int s, double fang, double tang, xpoint_t* pa, int n);

/*
@FUNCTION svg_draw_fan: draw fan in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct using float member.
@INPUT float r: the radius
@INPUT float s: the span
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@RETURN void: none.
*/
EXP_API void	svg_draw_fan(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float r, float s, double fang, double tang);
EXP_API void	svg_draw_fan_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int r, int s, double fang, double tang);

/*
@FUNCTION svg_draw_polygon: draw polygon in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct array using float member.
@INPUT int n: the point entity count.
@RETURN void: none.
*/
EXP_API void	svg_draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);
EXP_API void	svg_draw_polygon_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

/*
@FUNCTION svg_draw_equalgon: draw equal polygon using milimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the center point using float member.
@INPUT float fr: the radius.
@INPUT int n: the edge count.
@RETURN void: none.
*/
EXP_API void	svg_draw_equalgon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float fr, int n);
EXP_API void	svg_draw_equalgon_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int r, int n);

/*
@FUNCTION svg_calc_equalgon: calc equal polygon vertex using milimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpoint_t* ppt: the center point using float member.
@INPUT float fr: the radius.
@INPUT int n: the edge count.
@OUTPUT int n: the point array for vertex.
@RETURN void: none.
*/
EXP_API void	svg_calc_equalgon(canvas_t canv, const xpoint_t* ppt, float fr, int n, xpoint_t* pa);
EXP_API void	svg_calc_equalgon_raw(link_t_ptr g, const xpoint_t* ppt, int r, int n, xpoint_t* pa);

/*
@FUNCTION svg_multi_line: draw multiple base line in svg canvas using millimeter coordinate, the line separated by line height of font and face.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	svg_multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr);
EXP_API void	svg_multi_line_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr);

/*
@FUNCTION svg_draw_path: draw path in canvas using milimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const tchar_t* aa: the action stack.
@INPUT const xpoint_t* pa: the points stack.
@INPUT int n: the points stack size.
@RETURN void: none.
*/
EXP_API void	svg_draw_path(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n);
EXP_API void	svg_draw_path_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n);

/*
@FUNCTION svg_draw_text: draw text in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	svg_draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);
EXP_API void	svg_draw_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);
/*
@FUNCTION svg_text_out: output text in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xpoint* ppt: the start point struct using float member.
@INPUT const tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	svg_text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);
EXP_API void	svg_text_out_raw(link_t_ptr g, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);

/*
@FUNCTION svg_draw_var_text: draw string object in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT string_t var: the string object.
@RETURN void: none.
*/
EXP_API void	svg_draw_var_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var);
EXP_API void	svg_draw_var_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var);

/*
@FUNCTION svg_draw_tag_text: draw tag document in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr tag: the tag document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	svg_draw_tag_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);
EXP_API void	svg_draw_tag_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

/*
@FUNCTION svg_draw_memo_text: draw memo document in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr memo: the memo document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	svg_draw_memo_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);
EXP_API void	svg_draw_memo_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

/*
@FUNCTION svg_draw_rich_text: draw rich document in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr rich: the rich document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	svg_draw_rich_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);
EXP_API void	svg_draw_rich_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

/*
@FUNCTION svg_draw_pass: draw the password in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT const tchar_t* txt: the password text.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	svg_draw_pass(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

/*
@FUNCTION svg_draw_data: draw the formated data in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT const tchar_t* data: the data text.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@INPUT int dig: the digits of numeric text.
@INPUT const tchar_t* type: the data type, it can be ATTR_DATATYPE_*.
@INPUT const tchar_t* fmt: the text format token.
@INPUT bool_t zeronull: if nonzero, the numeric equal to zero, the text will not to be drawed.
@INPUT bool_t autowap: if nonzero, the text will auto
@RETURN void: none.
*/
EXP_API void	svg_draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap);

/*
@FUNCTION svg_text_rect: calc the text suitable rectangle in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT cont tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate zero character terminated.
@OUTPUT xrect_t* pxr: the rect struct for returning float member.
@RETURN void: none.
*/
EXP_API void	svg_text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr);
EXP_API void	svg_text_rect_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr);
/*
@FUNCTION svg_text_size: calc the text suitable size in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate zero character terminated.
@OUTPUT xsize_t* pxs: the size struct for returning float member.
@RETURN void: none.
*/
EXP_API void	svg_text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);
EXP_API void	svg_text_size_raw(link_t_ptr g, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

/*
@FUNCTION svg_text_metric: calc the image size in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas svg object.
@INPUT cont ximage_t* pxi: the image struct.
@OUTPUT xsize_t* pxs: the size struct for returning float member.
@RETURN void: none.
*/
EXP_API void	svg_text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs);
EXP_API void	svg_text_metric_raw(link_t_ptr g, const xfont_t* pxf, xsize_t* pxs);

/*
@FUNCTION svg_color_out: output color sequence in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT bool_t horz: nonzero for horizon drawing, zero for vertical drawing.
@INPUT const tchar_t* rgbstr: the rgb tokens, every rgb token separated by ';'.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	svg_color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len);

/*
@FUNCTION svg_draw_image: draw image in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const ximage_t* pxi: the image struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	svg_draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr);
EXP_API void	svg_draw_image_raw(link_t_ptr g, const ximage_t* pxi, const xrect_t* pxr);

/*
@FUNCTION svg_draw_code128: draw CODE128 bar in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xcolor_t* pxc: the foreground color.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* text: the text for PDF417 encoding.
@INPUT int len: text length in characters, -1 indicate zero terminated.
@RETURN void: none.
*/
EXP_API void 	svg_draw_code128(canvas_t canv, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len);
EXP_API void	svg_draw_code128_raw(link_t_ptr g, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len);

/*
@FUNCTION svg_draw_pdf417: draw PDF427 bar in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xcolor_t* pxc: the foreground color.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* text: the text for PDF417 encoding.
@INPUT int len: text length in characters, -1 indicate zero terminated.
@RETURN void: none.
*/
EXP_API void 	svg_draw_pdf417(canvas_t canv, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len);
EXP_API void	svg_draw_pdf417_raw(link_t_ptr g, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len);

#ifdef GPL_SUPPORT_QRCODE
/*
@FUNCTION svg_draw_qrcode: draw QR bar in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xcolor_t* pxc: the foreground color.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* text: the text for PDF417 encoding.
@INPUT int len: text length in characters, -1 indicate zero terminated.
@RETURN void: none.
*/
EXP_API void 	svg_draw_qrcode(canvas_t canv, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len);
EXP_API void	svg_draw_qrcode_raw(link_t_ptr g, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len);
#endif

/*
@FUNCTION svg_draw_gizmo: draw gizmo icon in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* iname: the icon name.
@RETURN void: none.
*/
EXP_API void	svg_draw_gizmo(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* iname);

EXP_API void	svg_draw_icon(canvas_t canv, const tchar_t* iname, const xrect_t* prt);

EXP_API void	svg_draw_thumb(canvas_t canv, const tchar_t* fname, const xrect_t* prt);


#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_SVG*/

#endif /*SVGGDI_H*/