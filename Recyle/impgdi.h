﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdi document

	@module	impgdi.h | interface file

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

#include "xdcdef.h"

#if defined(XDU_SUPPORT_CONTEXT)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION draw_line_raw: draw line in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the from point struct using integer member.
@INPUT const xpoint_t* ppt2: the to point struct using integer member.
@RETURN void: none.
*/
EXP_API void	draw_line_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

/*
@FUNCTION draw_line: draw line in canvas using milimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the from point struct using float member.
@INPUT const xpoint_t* ppt2: the to point struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

/*
@FUNCTION draw_bezier_raw: draw bezier in memory or device context using points coordinate
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the start point.
@INPUT const xpoint_t* ppt2: the control point.
@INPUT const xpoint_t* ppt3: the control point.
@INPUT const xpoint_t* ppt4: the end point.
@RETURN void: none.
*/
EXP_API void	draw_bezier_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4);

/*
@FUNCTION draw_bezier: draw bezier in canvas using millimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the start point using float member.
@INPUT const xpoint_t* ppt2: the control point using float member.
@INPUT const xpoint_t* ppt3: the control point using float member.
@INPUT const xpoint_t* ppt4: the end point using float member.
@RETURN void: none.
*/
EXP_API void	draw_bezier(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4);

/*
@FUNCTION draw_curve_raw: draw curve in memory or device context using points array
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point array.
@INPUT int n: the point array size.
@RETURN void: none.
*/
EXP_API void	draw_curve_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION draw_curve: draw curve in canvas using millimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point array.
@INPUT int n: the point array size.
@RETURN void: none.
*/
EXP_API void	draw_curve(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION draw_arc_raw: draw arc in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the start point struct using integer member.
@INPUT const xpoint_t* ppt2: the end point struct using integer member.
@INPUT int rx: the x-radius
@INPUT int ry: the y-radius
@INPUT bool_t sflag: is clock-wise drawing.
@INPUT bool_t lflag: is large arc.
@RETURN void: none.
*/
EXP_API void	draw_arc_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, int rx, int ry, bool_t sflag, bool_t lflag);

/*
@FUNCTION draw_arc: draw arc in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the start point struct using float member.
@INPUT const xpoint_t* ppt2: the end point struct using float member.
@INPUT float rx: the x-radius
@INPUT float ry: the y-radius
@INPUT bool_t sflag: is clock-wise drawing.
@INPUT bool_t lflag: is large arc.
@RETURN void: none.
*/
EXP_API void	draw_arc(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, float rx, float ry, bool_t sflag, bool_t lflag);

/*
@FUNCTION draw_polyline_raw: draw polyline in memory or device context using points coordinate
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point struct array using integer member.
@INPUT int n: the point entity count.
@RETURN void: none.
*/
EXP_API void	draw_polyline_raw(res_ctx_t rdc, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION draw_polyline: draw polyline in canvas using milimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point struct array using float member.
@INPUT int n: the point entity count.
@RETURN void: none.
*/
EXP_API void	draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION draw_trangle_raw: draw triangle in memory or device context using points coordinate
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT const tchar_t* orient: the triangle orientation
@RETURN void: none.
*/
EXP_API void	draw_triangle_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* orient);

/*
@FUNCTION draw_rect: draw triangle in canvas using milimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* orient: the triangle orientation
@RETURN void: none.
*/
EXP_API void	draw_triangle(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* orient);

/*
@FUNCTION draw_rect_raw: draw rect in memory or device context using points coordinate
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	draw_rect_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION draw_rect: draw rect in canvas using milimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION draw_round_raw: draw round rect in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	draw_round_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION draw_round: draw round rect in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION draw_ellipse_raw: draw ellipse in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	draw_ellipse_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION draw_ellipse: draw ellipse in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr);

/*
@FUNCTION draw_pie_raw: draw pie in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* pxr: the center point struct using integer member.
@INPUT int rx: the x-radius
@INPUT int ry: the y-radius
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@RETURN void: none.
*/
EXP_API void	draw_pie_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int rx, int ry, double fang, double tang);

/*
@FUNCTION draw_pie: draw pie in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* pxr: the center point struct using float member.
@INPUT int rx: the x-radius
@INPUT int ry: the y-radius
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@RETURN void: none.
*/
EXP_API void	draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float rx, float ry, double fang, double tang);

/*
@FUNCTION draw_fan_raw: draw fan in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct using integer member.
@INPUT int r: the radius
@INPUT int s: the span
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@RETURN void: none.
*/
EXP_API void	draw_fan_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int r, int s, double fang, double tang);

/*
@FUNCTION draw_fan: draw fan in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct using float member.
@INPUT float r: the radius
@INPUT float s: the span
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@RETURN void: none.
*/
EXP_API void	draw_fan(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float r, float s, double fang, double tang);

/*
@FUNCTION calc_fan_raw: calc fan points in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpont_t* ppt: the point struct using integer member.
@INPUT int r: the radius
@INPUT int s: the span
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@OUTPUT xpoint_t* pa: return the fan points.
@INPUT int n: the point array size.
@RETURN void: none.
*/
EXP_API void	calc_fan_raw(res_ctx_t rdc, const xpoint_t* ppt, int r, int s, double fang, double tang, xpoint_t* pa, int n);

/*
@FUNCTION calc_fan: draw fan in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const rect_t* ppt: the point struct using float member.
@INPUT float r: the radius
@INPUT float s: the span
@INPUT double fang: the from angle PI value.
@INPUT double tang: the to angle PI value.
@OUTPUT xpoint_t* pa: return the fan points.
@INPUT int n: the point array size.
@RETURN void: none.
*/
EXP_API void	calc_fan(canvas_t canv, const xpoint_t* ppt, float r, float s, double fang, double tang, xpoint_t* pa, int n);

/*
@FUNCTION draw_arrow_raw: draw arrrow in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int alen: the arrow length in points.
@INPUT double arc: the arrow angle PI value.
@RETURN void: none.
*/
EXP_API void	draw_arrow_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, int alen, double arc);

/*
@FUNCTION draw_arrow: draw arrrow in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT float alen: the arrow length in millimeter.
@INPUT double arc: the arrow angle PI value.
@RETURN void: none.
*/
EXP_API void	draw_arrow(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, float alen, double arc);

/*
@FUNCTION draw_polygon_raw: draw polygon in memory or device context using points coordinate
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct array using integer member.
@INPUT int n: the point entity count.
@RETURN void: none.
*/
EXP_API void	draw_polygon_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

/*
@FUNCTION draw_polygon: draw polygon in canvas using milimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct array using float member.
@INPUT int n: the point entity count.
@RETURN void: none.
*/
EXP_API void	draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

/*
@FUNCTION draw_equalgon_raw: draw equal polygon in memory or device context using points coordinate
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the center point using integer member.
@INPUT int r: the radius.
@INPUT int n: the edge count.
@RETURN void: none.
*/
EXP_API void	draw_equalgon_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int r, int n);

/*
@FUNCTION draw_equalgon: draw equal polygon using milimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the center point using float member.
@INPUT float fr: the radius.
@INPUT int n: the edge count.
@RETURN void: none.
*/
EXP_API void	draw_equalgon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float fr, int n);

/*
@FUNCTION calc_equalgon_raw: calc equal polygon in memory or device context using points coordinate
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpoint_t* ppt: the center point using integer member.
@INPUT int r: the radius.
@INPUT int n: the edge count.
@OUTPUT int n: the point array for vertex.
@RETURN void: none.
*/
EXP_API void	calc_equalgon_raw(res_ctx_t rdc, const xpoint_t* ppt, int r, int n, xpoint_t* pa);

/*
@FUNCTION calc_equalgon: calc equal polygon vertex using milimeter coordinate
@INPUT canvas_t canv: the canvas object.
@INPUT const xpoint_t* ppt: the center point using float member.
@INPUT float fr: the radius.
@INPUT int n: the edge count.
@OUTPUT int n: the point array for vertex.
@RETURN void: none.
*/
EXP_API void	calc_equalgon(canvas_t canv, const xpoint_t* ppt, float fr, int n, xpoint_t* pa);

/*
@FUNCTION draw_path_raw: draw path in memory or device context using points coordinate
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const tchar_t* aa: the action stack.
@INPUT const xpoint_t* pa: the points stack.
@INPUT int n: the points stack size.
@RETURN void: none.
*/
EXP_API void	draw_path_raw(res_ctx_t rdc, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n);

/*
@FUNCTION draw_path: draw path in canvas using milimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const tchar_t* aa: the action stack.
@INPUT const xpoint_t* pa: the points stack.
@INPUT int n: the points stack size.
@RETURN void: none.
*/
EXP_API void	draw_path(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n);

/*
@FUNCTION multi_line_raw: draw multiple base line in memory or device context using points coordinate, the line separated by line height of font and face.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	multi_line_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr);

/*
@FUNCTION multi_line: draw multiple base line in canvas using millimeter coordinate, the line separated by line height of font and face.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr);

/*
@FUNCTION draw_text_raw: draw text in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT const tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	draw_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

/*
@FUNCTION draw_text: draw text in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT const tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

/*
@FUNCTION text_out_raw: output text in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xpoint* ppt: the start point struct using integer member.
@INPUT const tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	text_out_raw(res_ctx_t rdc, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);

/*
@FUNCTION text_out: output text in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xpoint* ppt: the start point struct using float member.
@INPUT const tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len);


/*
@FUNCTION text_rect_raw: calc the text suitable rectangle in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT cont tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate zero character terminated.
@OUTPUT xrect_t* pxr: the rect struct for returning integer member.
@RETURN void: none.
*/
EXP_API void	text_rect_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr);

/*
@FUNCTION text_rect: calc the text suitable rectangle in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT cont tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate zero character terminated.
@OUTPUT xrect_t* pxr: the rect struct for returning float member.
@RETURN void: none.
*/
EXP_API void	text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr);

/*
@FUNCTION text_size_raw: calc the text suitable size in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate zero character terminated.
@OUTPUT xsize_t* pxs: the size struct for returning integer member.
@RETURN void: none.
*/
EXP_API void	text_size_raw(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

/*
@FUNCTION text_size: calc the text suitable size in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont tchar_t* txt: the text token.
@INPUT int len: the text length in characters, -1 indicate zero character terminated.
@OUTPUT xsize_t* pxs: the size struct for returning float member.
@RETURN void: none.
*/
EXP_API void	text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxr);

/*
@FUNCTION text_metric_raw: calc the character metric in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT cont xfont_t* pxf: the font struct.
@OUTPUT xsize_t* pxs: the size struct for returning integer member.
@RETURN void: none.
*/
EXP_API void	text_metric_raw(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs);

/*
@FUNCTION text_metric: calc the character metric in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@OUTPUT xsize_t* pxs: the size struct for returning float member.
@RETURN void: none.
*/
EXP_API void	text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs);

/*
@FUNCTION draw_var_text_raw: draw string object in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT string_t var: the string object.
@RETURN void: none.
*/
EXP_API void	draw_var_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var);

/*
@FUNCTION draw_var_text: draw string object in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT string_t var: the string object.
@RETURN void: none.
*/
EXP_API void	draw_var_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var);

/*
@FUNCTION draw_tag_text_raw: draw tag document in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT link_t_ptr tag: the tag document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	draw_tag_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag, int page);

/*
@FUNCTION draw_tag_text: draw tag document in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr tag: the tag document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	draw_tag_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag, int page);

/*
@FUNCTION calc_tag_pages_raw: calc tag document pages in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT link_t_ptr tag: the tag document.
@RETURN int: return total pages.
*/
EXP_API int		calc_tag_pages_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag);

/*
@FUNCTION calc_tag_pages: calc tag document pages in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr tag: the tag document.
@RETURN int: return total pages.
*/
EXP_API int		calc_tag_pages(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo);

/*
@FUNCTION draw_memo_text_raw: draw memo document in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT link_t_ptr memo: the memo document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	draw_memo_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo, int page);

/*
@FUNCTION draw_memo_text: draw memo document in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr memo: the memo document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	draw_memo_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo, int page);

/*
@FUNCTION calc_memo_pages_raw: calc memo document pages in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT link_t_ptr memo: the memo document.
@RETURN int: return total pages.
*/
EXP_API int		calc_memo_pages_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo);

/*
@FUNCTION calc_memo_pages: calc memo document pages in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr memo: the memo document.
@RETURN int: return total pages.
*/
EXP_API int		calc_memo_pages(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo);

/*
@FUNCTION draw_rich_text_raw: draw rich document in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT link_t_ptr rich: the rich document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	draw_rich_text_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich, int page);

/*
@FUNCTION draw_rich_text: draw rich document in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr rich: the rich document.
@INPUT int page: the page will be drawed, the page index is 1-based.
@RETURN void: none.
*/
EXP_API void	draw_rich_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich, int page);

/*
@FUNCTION calc_rich_pages_raw: calc rich document pages in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT link_t_ptr rich: the rich document.
@RETURN int: return total pages.
*/
EXP_API int		calc_rich_pages_raw(res_ctx_t rdc, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich);

/*
@FUNCTION calc_rich_pages: calc rich document pages in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT link_t_ptr rich: the rich document.
@RETURN int: return total pages.
*/
EXP_API int		calc_rich_pages(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich);

/*
@FUNCTION color_out_raw: output color sequence in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT bool_t horz: nonzero for horizon drawing, zero for vertical drawing.
@INPUT const tchar_t* rgbstr: the rgb tokens, every rgb token separated by ';'.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	color_out_raw(res_ctx_t rdc, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len);

/*
@FUNCTION color_out: output color sequence in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT bool_t horz: nonzero for horizon drawing, zero for vertical drawing.
@INPUT const tchar_t* rgbstr: the rgb tokens, every rgb token separated by ';'.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len);

/*
@FUNCTION draw_thumn_raw: draw thumn image in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const tchar_t* fname: the file name.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	draw_thumn_raw(res_ctx_t rdc, const tchar_t* fname, const xrect_t* prt);

/*
@FUNCTION draw_thumb: draw file thumb in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const tchar_t* fname: the file name.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_thumb(canvas_t canv, const tchar_t* fname, const xrect_t* prt);

/*
@FUNCTION draw_icon_raw: draw icon in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const tchar_t* iname: the icon name.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	draw_icon_raw(res_ctx_t rdc, const tchar_t* iname, const xrect_t* prt);

/*
@FUNCTION draw_icon: draw system icon in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const tchar_t* iname: the icon name.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_icon(canvas_t canv, const tchar_t* iname, const xrect_t* prt);

/*
@FUNCTION image_size_raw: calc the image size in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT cont ximage_t* pxi: the image struct.
@OUTPUT xsize_t* pxs: the size struct for returning integer member.
@RETURN void: none.
*/
EXP_API void	image_size_raw(res_ctx_t rdc, const ximage_t* pxi, xsize_t* pxs);

/*
@FUNCTION image_size: calc the image size in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT cont ximage_t* pxi: the image struct.
@OUTPUT xsize_t* pxs: the size struct for returning float member.
@RETURN void: none.
*/
EXP_API void	image_size(canvas_t canv, const ximage_t* pxi, xsize_t* pxs);

/*
@FUNCTION draw_image_raw: draw image in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const ximage_t* pxi: the image struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	draw_image_raw(res_ctx_t rdc, const ximage_t* pxi, const xrect_t* pxr);

/*
@FUNCTION draw_image: draw image in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const ximage_t* pxi: the image struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr);

/*
@FUNCTION draw_bitmap_raw: draw bitmap in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT res_bmp_t bmp: the bitmap resource handle.
@INPUT const xpoint_t* ppt: the point struct using integer member.
@RETURN void: none.
*/
EXP_API void	draw_bitmap_raw(res_ctx_t rdc, res_bmp_t bmp, const xpoint_t* ppt);

/*
@FUNCTION draw_bitmap: draw bitmap in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT res_bmp_t bmp: the bitmap resource handle.
@INPUT const xpoint_t* ppt: the point struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_bitmap(canvas_t canv, res_bmp_t bmp, const xpoint_t* ppt);

/*
@FUNCTION gradient_rect_raw: gradient a rect in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xcolor_t* xc_brim: the brim color.
@INPUT const xcolor_t* xc_core: the core color.
@INPUT const tchar_t* gradient: the gradient type.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	gradient_rect_raw(res_ctx_t rdc, const xcolor_t* xc_brim, const xcolor_t* xc_core, const tchar_t* gradient, const xrect_t* pxr);

/*
@FUNCTION gradient_rect: gradient a rect in canvas using milimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xgradi_t* pxg the gradient struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	gradient_rect(canvas_t canv, const xcolor_t* xc_brim, const xcolor_t* xc_core, const tchar_t* gradient, const xrect_t* pxr);

/*
@FUNCTION alphablend_rect_raw: alphablend a rect in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int opacity: the alphablend level: 0~255, or use predefined value: ALPHA_SOLID(250), ALPHA_SOFT(128), ALPHA_TRANS(64).
@RETURN void: none.
*/
EXP_API void	alphablend_rect_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int opacity);

/*
@FUNCTION alphablend_rect: alphablend a rect in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT int opacity: the alphablend level: 0~255, or use predefined value: ALPHA_SOLID(250), ALPHA_SOFT(128), ALPHA_TRANS(64).
@RETURN void: none.
*/
EXP_API void	alphablend_rect(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int opa);


/*
@FUNCTION exclip_rect_raw: clip the rect outside in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	exclip_rect_raw(res_ctx_t rdc, const xrect_t* pxr);

/*
@FUNCTION exclip_rect: clip the rect outside in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	exclip_rect(canvas_t canv, const xrect_t* pxr);

/*
@FUNCTION inclip_rect_raw: clip the rect inside in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@RETURN void: none.
*/
EXP_API void	inclip_rect_raw(res_ctx_t rdc, const xrect_t* pxr);

/*
@FUNCTION inclip_rect: clip the rect inside in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xrect_t* pxr: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	inclip_rect(canvas_t canv, const xrect_t* pxr);

/****************************************************************************************************************************/

/*
@FUNCTION fill_region_raw: fill the windows region in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT res_rgn_t rgn: the resion resource handle.
@RETURN void: none.
*/
EXP_API void	fill_region_raw(res_ctx_t rdc, const xbrush_t* pxb, res_rgn_t rgn);

/*
@FUNCTION draw_select_raw: draw select frame in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
EXP_API void	draw_select_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_select: draw select frame in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
//EXP_API void	draw_select(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_focus_raw: draw focus frame in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
EXP_API void	draw_focus_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_focus: draw focus frame in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
//EXP_API void	draw_focus(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_feed_raw: draw feed frame in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
EXP_API void	draw_feed_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_feed: draw feed frame in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using float member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
//EXP_API void	draw_feed(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int deep);

/*
@FUNCTION draw_sizing_raw: draw feed frame in memory or device context using points coordinate.
@INPUT res_ctx_t rdc: the context resource handle.
@INPUT const xcolor_t* pxc: the color struct.
@INPUT const xrect_t* pxr: the rect struct using integer member.
@INPUT int deep: the alphablend level: 0~255, the predefined value is ALPHA_SOLID, ALPHA_SOFT, ALPHA_TRANS.
@RETURN void: none.
*/
EXP_API void draw_sizing_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, int deep, dword_t pos);

/*******************************************************************************************************************/

/*
@FUNCTION draw_pass: draw the password in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT cont xfont_t* pxf: the font struct.
@INPUT cont xface_t* pxa: the face struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT const tchar_t* txt: the password text.
@INPUT int len: the text length in characters, -1 indicate the text is terminated by zero.
@RETURN void: none.
*/
EXP_API void	draw_pass(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len);

/*
@FUNCTION draw_data: draw the formated data in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
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
EXP_API void	draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap);

/*
@FUNCTION draw_progress: draw the progress in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT cont xcolor_t* pxc: the color struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT int steps: the steps of progress.
@RETURN void: none.
*/
EXP_API void	draw_progress(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt, int steps);

/*
@FUNCTION draw_ruler: draw the ruler in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT cont xcolor_t* pxc: the color struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_ruler(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt);

/*
@FUNCTION draw_corner: draw the conner in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT cont xcolor_t* pxc: the color struct.
@INPUT const xrect_t* prt: the rect struct using float member.
@RETURN void: none.
*/
EXP_API void	draw_corner(canvas_t canv, const xcolor_t* pxc, const xrect_t* prt);

/*
@FUNCTION draw_svg: draw the svg document in canvas using millimeter coordinate.
@INPUT canvas_t canv: the canvas object.
@INPUT const xrect_t* prt: the rect struct using float member.
@INPUT link_t_ptr svg: the svg document.
@RETURN void: none.
*/
EXP_API void	draw_svg(canvas_t canv, const xrect_t* prt, link_t_ptr svg);

#ifdef	__cplusplus
}
#endif

#endif /*XDU_SUPPORT_CONTEXT*/

#endif /*IMPGDI_H*/