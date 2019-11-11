/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdi canvas document

	@module	gdicanv.h | interface file

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

#ifndef _GDICANV_H
#define _GDICANV_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_CONTEXT)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_display_canvas: create a display context canvas.
@INPUT res_dc_t rdc: the device context resource handle.
@RETURN canvas_t: if succeeds return canvas object, fails return NULL.
*/
XDL_API canvas_t create_display_canvas(res_ctx_t rdc);

/*
@FUNCTION destroy_display_canvas: destroy the display context canvas.
@INPUT canvas_t canv: the canvas object.
@RETURN void: none.
*/
XDL_API void	destroy_display_canvas(canvas_t canv);

/*
@FUNCTION set_canvas_ratio: set the canvas horizon and vertical ratio.
@INPUT canvas_t canv: the canvas object.
@INPUT float htpermm: the horizon points per millimeter.
@INPUT float vtpermm: the vertical points per millimeter.
@RETURN void: none.
*/
XDL_API void	set_canvas_ratio(canvas_t canv, float htpermm, float vtpermm);

/*
@FUNCTION get_canvas_horz_size: get the canvas horizon size in millimeter.
@INPUT canvas_t canv: the canvas object.
@RETURN float: return the canvas horizon size.
*/
XDL_API float	get_canvas_horz_size(canvas_t canv);

/*
@FUNCTION get_canvas_horz_size: get the canvas vertical size in millimeter.
@INPUT canvas_t canv: the canvas object.
@RETURN float: return the canvas vertical size.
*/
XDL_API float	get_canvas_vert_size(canvas_t canv);

/*
@FUNCTION set_canvas_horz_feed: set the canvas horizon feed in millimeter, the feed span is not drawable.
@INPUT canvas_t canv: the canvas object.
@INPUT float cx: the feed value.
@RETURN void: none.
*/
XDL_API void	set_canvas_horz_feed(canvas_t canv, float cx);

/*
@FUNCTION get_canvas_horz_feed: get the canvas horizon feed in millimeter, the feed span is not drawable.
@INPUT canvas_t canv: the canvas object.
@RETURN float: return the canvas horizon feed.
*/
XDL_API float	get_canvas_horz_feed(canvas_t canv);

/*
@FUNCTION set_canvas_vert_feed: set the canvas vertical feed in millimeter, the feed span is not drawable.
@INPUT canvas_t canv: the canvas object.
@INPUT float cx: the feed value.
@RETURN void: none.
*/
XDL_API void	set_canvas_vert_feed(canvas_t canv, float cy);

/*
@FUNCTION get_canvas_vert_feed: get the canvas vertical feed in millimeter, the feed span is not drawable.
@INPUT canvas_t canv: the canvas object.
@RETURN float: return the canvas vertical feed.
*/
XDL_API float	get_canvas_vert_feed(canvas_t canv);

/*
@FUNCTION set_canvas_scale: set the canvas scale ratio in millimeter.
@INPUT canvas_t canv: the canvas object.
@INPUT float sca: the scale ratio value.
@RETURN void: none.
*/
XDL_API void	set_canvas_scale(canvas_t canv, float sca);

/*
@FUNCTION get_canvas_scale: get the canvas scale ratio in millimeter.
@INPUT canvas_t canv: the canvas object.
@RETURN float: return the canvas scale ratio.
*/
XDL_API float	get_canvas_scale(canvas_t canv);

/*
@FUNCTION get_canvas_ctx: get the canvas context resource handle.
@INPUT canvas_t canv: the canvas object.
@RETURN res_ctx_t: return the context resource handle if exists, otherwise return NULL.
*/
XDL_API res_ctx_t get_canvas_ctx(canvas_t canv);

/*
@FUNCTION get_canvas_type: get the canvas type, it can be _CANV_DISPLAY, _CANV_PRINTER.
@INPUT canvas_t canv: the canvas object.
@RETURN int: return the canvas type.
*/
XDL_API int get_canvas_type(canvas_t canv);

/*
@FUNCTION begin_canvas_paint: begin canvas painting and return a memory context for drawing buffer.
@INPUT canvas_t canv: the canvas object.
@INPUT res_ctx_t rdc: the display or printer context resource handle.
@INPUT long width: the client width in points.
@INPUT long height: the client height in points.
@RETURN res_ctx_t: if succeeds return memory context resource handle, fails return NULL.
*/
XDL_API res_ctx_t begin_canvas_paint(canvas_t canv, res_ctx_t rdc, long width, long height);

/*
@FUNCTION end_canvas_paint: end canvas painting and free drawing buffer.
@INPUT canvas_t canv: the canvas object.
@INPUT res_ctx_t rdc: the display or printer context resource handle.
@INPUT const xrect_t: the client rectangle for rendering context from buffer.
@RETURN void: none.
*/
XDL_API void	end_canvas_paint(canvas_t canv, res_ctx_t rdc, const xrect_t* pxr);

/*
@FUNCTION get_canvas_measure: fill the canvas measure functions.
@INPUT canvas_t canv: the canvas object.
@OUTPUT if_measure_t pif: the measure struct for returning inner function.
@RETURN void: none.
*/
XDL_API void get_canvas_measure(canvas_t canv, if_measure_t* pif);

#ifdef XDK_SUPPORT_CONTEXT_PRINTER
/*
@FUNCTION create_printer_canvas: create a printer context canvas.
@INPUT res_dc_t rdc: the display context resource handle.
@RETURN canvas_t: if succeeds return canvas object, fails return NULL.
*/
XDL_API canvas_t create_printer_canvas(res_ctx_t rdc);

/*
@FUNCTION destroy_printer_canvas: destroy the printer context canvas.
@INPUT canvas_t canv: the canvas object.
@RETURN void: none.
*/
XDL_API void	destroy_printer_canvas(canvas_t canv);

#endif //XDK_SUPPORT_CONTEXT_PRINTER

/*
@FUNCTION pt_to_tm: mapping points to millimeter in canvas.
@INPUT canvas_t canv: the canvas object.
@INPUT long pt: the points value.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in millimeter.
*/
XDL_API float pt_to_tm(canvas_t canv, long pt, bool_t horz);

/*
@FUNCTION tm_to_pt: mapping millimeter to points in canvas.
@INPUT canvas_t canv: the canvas object.
@INPUT float tm: the millimeter value.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in points.
*/
XDL_API long tm_to_pt(canvas_t canv, float tm, bool_t horz);

/*
@FUNCTION rect_tm_to_pt: mapping rectangle points to millimeter in canvas.
@INPUT canvas_t canv: the canvas object.
@INOUTPUT xrect_t* pxr: the rect struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void rect_tm_to_pt(canvas_t canv, xrect_t* pxr);

/*
@FUNCTION rect_pt_to_tm: mapping rectangle millimeter to points in canvas.
@INPUT canvas_t canv: the canvas object.
@INOUTPUT xrect_t* pxr: the rect struct for inputing float member and outputing integer member.
@RETURN void: none.
*/
XDL_API void rect_pt_to_tm(canvas_t canv, xrect_t* pxr);

/*
@FUNCTION size_tm_to_pt: mapping size points to millimeter in canvas.
@INPUT canvas_t canv: the canvas object.
@INOUTPUT xsize_t* pxs: the size struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void size_tm_to_pt(canvas_t canv, xsize_t* pxs);

/*
@FUNCTION size_pt_to_tm: mapping size points to millimeter in canvas.
@INPUT canvas_t canv: the canvas object.
@INOUTPUT xsize_t* pxs: the size struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void size_pt_to_tm(canvas_t canv, xsize_t* pxs);

/*
@FUNCTION point_tm_to_pt: mapping point points to millimeter in canvas.
@INPUT canvas_t canv: the canvas object.
@INOUTPUT xpoint_t* ppt: the point struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void point_tm_to_pt(canvas_t canv, xpoint_t* ppt);

/*
@FUNCTION point_pt_to_tm: mapping point points to millimeter in canvas.
@INPUT canvas_t canv: the canvas object.
@INOUTPUT xpoint_t* ppt: the point struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void point_pt_to_tm(canvas_t canv, xpoint_t* ppt);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*GDICANV_H*/