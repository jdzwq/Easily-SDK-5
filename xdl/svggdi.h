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

#if defined(XDK_SUPPORT_CONTEXT)

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
XDL_API void	svg_draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

/*
@FUNCTION svg_draw_3dline: draw 3d line in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the from point struct using float member.
@INPUT const xpoint_t* ppt2: the to point struct using float member.
@RETURN void: none.
*/
XDL_API void	svg_draw_3dline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

/*
@FUNCTION svg_draw_3drect: draw 3d rect in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@RETURN void: none.
*/
XDL_API void	svg_draw_3drect(canvas_t canv, const xpen_t* pxp, const xrect_t* prt);

/*
@FUNCTION svg_draw_polyline: draw polyline in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point struct array using float member.
@INPUT int n: the point entity count.
@RETURN void: none.
*/
XDL_API void	svg_draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION svg_draw_polygon: draw polygon in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct array using float member.
@INPUT int n: the point entity count.
@RETURN void: none.
*/
XDL_API void	svg_draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

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
XDL_API void	svg_draw_bezier(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4);

/*
@FUNCTION svg_draw_curve: draw curve in svg canvas using millimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point array.
@INPUT int n: the point array size.
@RETURN void: none.
*/
XDL_API void	svg_draw_curve(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION draw_rect: draw svg_draw_rect in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
XDL_API void	svg_draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION svg_gradient_rect: gradient a rect in svg canvas using milimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xgradi_t* pxg the gradient struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
XDL_API void	svg_gradient_rect(canvas_t canv, const xgradi_t* pxg, const xrect_t* pxr);

/*
@FUNCTION svg_alphablend_rect: alphablend a rect in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT int opacity: the alphablend level: 0~255, or use predefined value: ALPHA_SOLID(250), ALPHA_SOFT(128), ALPHA_TRANS(64).
@RETURN void: none.
*/
XDL_API void	svg_alphablend_rect(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int opacity);

/*
@FUNCTION svg_draw_round: draw round rect in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
XDL_API void	svg_draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION svg_draw_ellipse: draw ellipse in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
XDL_API void	svg_draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

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
XDL_API void	svg_draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float rx, float ry, double fang, double tang);

/*
@FUNCTION svg_draw_arc: draw arc in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point struct using millimeter member.
@INPUT float rx: the x-radius
@INPUT float ry: the y-radius
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@RETURN void: none.
*/
XDL_API void	svg_draw_arc(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, float rx, float ry, double fang, double tang);

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
XDL_API void	svg_draw_arrow(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, float alen, double arc);

/*
@FUNCTION svg_draw_shape: draw shape in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* shape: the shape name, eg: ATTR_SHAPE_*.
@RETURN void: none.
*/
XDL_API void	svg_draw_shape(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* shape);

/*
@FUNCTION svg_multi_line: draw multiple base line in svg canvas using millimeter coordinate, the line separated by line height of font and face.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
XDL_API void	svg_multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr);

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
XDL_API void	svg_draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

/*
@FUNCTION svg_text_out: output text in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xpoint* ppt: the start point struct using float member.
@INPUT const tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
XDL_API void	svg_text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);

/*
@FUNCTION svg_color_out: output color sequence in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT bool_t horz: nonzero for horizon drawing, zero for vertical drawing.
@INPUT const tchar_t* rgbstr: the rgb tokens, every rgb token separated by ';'.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
XDL_API void	svg_color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len);

/*
@FUNCTION svg_draw_var_text: draw string object in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT string_t var: the string object.
@RETURN void: none.
*/
XDL_API void	svg_draw_var_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var);

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
XDL_API void	svg_draw_tag_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

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
XDL_API void	svg_draw_memo_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

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
XDL_API void	svg_draw_rich_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page);

/*
@FUNCTION svg_draw_image: draw image in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const ximage_t* pxi: the image struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
XDL_API void	svg_draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr);

/*
@FUNCTION svg_draw_icon: draw icon in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* iname: the icon name.
@RETURN void: none.
*/
XDL_API void	svg_draw_icon(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* iname);

/*
@FUNCTION svg_draw_password: draw the password in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT const tchar_t* txt: the password text.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
XDL_API void	svg_draw_password(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

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
XDL_API void	svg_draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap);

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
XDL_API void	svg_text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr);

/*
@FUNCTION svg_text_size: calc the text suitable size in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate zero character terminated.
@OUTPUT xsize_t* pxs: the size struct for returning float member.
@RETURN void: none.
*/
XDL_API void	svg_text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

/*
@FUNCTION svg_text_metric: calc the image size in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas svg object.
@INPUT cont ximage_t* pxi: the image struct.
@OUTPUT xsize_t* pxs: the size struct for returning float member.
@RETURN void: none.
*/
XDL_API void	svg_text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs);

/*
@FUNCTION svg_draw_fix_text: draw multiple line text in svg canvas using millimeter coordinate.
@INPUT canvas_t canv: the svg canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
XDL_API void	svg_draw_fix_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* text, int len);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*SVGGDI_H*/