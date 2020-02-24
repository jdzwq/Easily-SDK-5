/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg canvas document

	@module	svgcanv.h | interface file

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

#ifndef _SVGCANV_H
#define _SVGCANV_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_CONTEXT)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_svg_canvas: create a svg canvas.
@INPUT link_t_ptr svg: the svg document.
@RETURN canvas_t: if succeeds return canvas object, fails return NULL.
*/
XDL_API canvas_t create_svg_canvas(link_t_ptr svg);

/*
@FUNCTION destroy_svg_canvas: destroy the svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@RETURN void: none.
*/
XDL_API void destroy_svg_canvas(canvas_t canv);

/*
@FUNCTION svg_get_canvas_ctx: get the svg canvas context resource handle.
@INPUT canvas_t canv: the svg canvas object.
@RETURN res_ctx_t: return the context resource handle if exists, otherwise return NULL.
*/
XDL_API res_ctx_t svg_get_canvas_ctx(canvas_t canv);

/*
@FUNCTION svg_get_canvas_doc: get the svg canvas document.
@INPUT canvas_t canv: the svg canvas object.
@RETURN link_t_ptr: return the svg document if exists, otherwise return NULL.
*/
XDL_API link_t_ptr svg_get_canvas_doc(canvas_t canv);

/*
@FUNCTION svg_pt_per_mm: get points per millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in millimeter.
*/
XDL_API float svg_pt_per_mm(canvas_t canv, bool_t horz);

/*
@FUNCTION svg_pt_to_tm: mapping points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INPUT int pt: the points value.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in millimeter.
*/
XDL_API float svg_pt_to_tm(canvas_t canv, int pt, bool_t horz);

/*
@FUNCTION svg_tm_to_pt: mapping millimeter to points in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INPUT float tm: the millimeter value.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in points.
*/
XDL_API int svg_tm_to_pt(canvas_t canv, float tm, bool_t horz);

/*
@FUNCTION svg_rect_tm_to_pt: mapping rectangle points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xrect_t* pxr: the rect struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void svg_rect_tm_to_pt(canvas_t canv, xrect_t* pxr);

/*
@FUNCTION svg_rect_pt_to_tm: mapping rectangle millimeter to points in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xrect_t* pxr: the rect struct for inputing float member and outputing integer member.
@RETURN void: none.
*/
XDL_API void svg_rect_pt_to_tm(canvas_t canv, xrect_t* pxr);

/*
@FUNCTION svg_size_tm_to_pt: mapping size points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xsize_t* pxs: the size struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void svg_size_tm_to_pt(canvas_t canv, xsize_t* pxs);

/*
@FUNCTION svg_size_pt_to_tm: mapping size points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xsize_t* pxs: the size struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void svg_size_pt_to_tm(canvas_t canv, xsize_t* pxs);

/*
@FUNCTION svg_point_tm_to_pt: mapping point points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xpoint_t* ppt: the point struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void svg_point_tm_to_pt(canvas_t canv, xpoint_t* ppt);

/*
@FUNCTION svg_point_pt_to_tm: mapping point points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xpoint_t* ppt: the point struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
XDL_API void svg_point_pt_to_tm(canvas_t canv, xpoint_t* ppt);

/*
@FUNCTION svg_get_canvas_measure: fill the svg canvas measure functions.
@INPUT canvas_t canv: the svg canvas object.
@OUTPUT if_measure_t pif: the measure struct for returning inner function.
@RETURN void: none.
*/
XDL_API void svg_get_canvas_measure(canvas_t canv, if_measure_t* pif);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*SVGCANV_H*/