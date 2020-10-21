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

#if defined(XDL_SUPPORT_SVG)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_svg_canvas: create a svg canvas.
@INPUT visual_t view: the visual object.
@RETURN canvas_t: if succeeds return canvas object, fails return NULL.
*/
EXP_API canvas_t create_svg_canvas(visual_t view);

/*
@FUNCTION destroy_svg_canvas: destroy the svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@RETURN void: none.
*/
EXP_API void destroy_svg_canvas(canvas_t canv);

/*
@FUNCTION svg_get_canvas_visual: get the svg visual object.
@INPUT canvas_t canv: the svg canvas object.
@RETURN visual_t: return the visual object if exists, otherwise return NULL.
*/
EXP_API visual_t svg_get_canvas_visual(canvas_t canv);

/*
@FUNCTION svg_pt_per_in: get points per inch in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in millimeter.
*/
EXP_API float svg_pt_per_in(canvas_t canv, bool_t horz);

/*
@FUNCTION svg_pt_per_mm: get points per millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in millimeter.
*/
EXP_API float svg_pt_per_mm(canvas_t canv, bool_t horz);

/*
@FUNCTION svg_pt_to_tm: mapping points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INPUT int pt: the points value.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in millimeter.
*/
EXP_API float svg_pt_to_tm(canvas_t canv, int pt, bool_t horz);

/*
@FUNCTION svg_tm_to_pt: mapping millimeter to points in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INPUT float tm: the millimeter value.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in points.
*/
EXP_API int svg_tm_to_pt(canvas_t canv, float tm, bool_t horz);

/*
@FUNCTION svg_rect_tm_to_pt: mapping rectangle points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xrect_t* pxr: the rect struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
EXP_API void svg_rect_tm_to_pt(canvas_t canv, xrect_t* pxr);

/*
@FUNCTION svg_rect_pt_to_tm: mapping rectangle millimeter to points in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xrect_t* pxr: the rect struct for inputing float member and outputing integer member.
@RETURN void: none.
*/
EXP_API void svg_rect_pt_to_tm(canvas_t canv, xrect_t* pxr);

/*
@FUNCTION svg_size_tm_to_pt: mapping size points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xsize_t* pxs: the size struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
EXP_API void svg_size_tm_to_pt(canvas_t canv, xsize_t* pxs);

/*
@FUNCTION svg_size_pt_to_tm: mapping size points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xsize_t* pxs: the size struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
EXP_API void svg_size_pt_to_tm(canvas_t canv, xsize_t* pxs);

/*
@FUNCTION svg_point_tm_to_pt: mapping point points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xpoint_t* ppt: the point struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
EXP_API void svg_point_tm_to_pt(canvas_t canv, xpoint_t* ppt);

/*
@FUNCTION svg_point_pt_to_tm: mapping point points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xpoint_t* ppt: the point struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
EXP_API void svg_point_pt_to_tm(canvas_t canv, xpoint_t* ppt);

/*
@FUNCTION svg_point_tm_to_pt: mapping point points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xspan_t* ppt: the span struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
EXP_API void svg_span_tm_to_pt(canvas_t canv, xspan_t* ppn);

/*
@FUNCTION svg_point_pt_to_tm: mapping point points to millimeter in svg canvas.
@INPUT canvas_t canv: the svg canvas object.
@INOUTPUT xspan_t* ppn: the span struct for inputing integer member and outputing float member.
@RETURN void: none.
*/
EXP_API void svg_span_pt_to_tm(canvas_t canv, xspan_t* ppn);

/*
@FUNCTION create_svg_visual: create a svg visual.
@INPUT link_t_ptr svg: the svg doucment.
@RETURN visual_t: if succeeds return visual object, fails return NULL.
*/
EXP_API visual_t create_svg_visual(link_t_ptr svg);

/*
@FUNCTION destroy_svg_visual: destroy the svg visual.
@INPUT visual_t view: the svg visual object.
@RETURN void: none.
*/
EXP_API void destroy_svg_visual(visual_t view);

/*
@FUNCTION svg_get_visual_node: get the svg visual node.
@INPUT visual_t view: the svg visual object.
@RETURN link_t_ptr: return the svg document if exists, otherwise return NULL.
*/
EXP_API link_t_ptr svg_get_visual_doc(visual_t view);

/*
@FUNCTION svg_pt_to_tm: mapping points to millimeter in svg canvas.
@INPUT visual_t view: the svg visual object.
@INPUT int pt: the points value.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in millimeter.
*/
EXP_API float svg_pt_to_tm_raw(visual_t view, int pt, bool_t horz);

/*
@FUNCTION svg_tm_to_pt_raw: mapping millimeter to points in svg canvas.
@INPUT visual_t view: the svg visual object.
@INPUT float tm: the millimeter value.
@INPUT bool_t horz: nonzero fro horizon mapping, zero for vertical mapping.
@RETURN float: return the value in points.
*/
EXP_API int svg_tm_to_pt_raw(visual_t view, float tm, bool_t horz);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_SVG*/

#endif /*SVGCANV_H*/