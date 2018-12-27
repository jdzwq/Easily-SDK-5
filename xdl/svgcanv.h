/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg canvas document

	@module	svgcanv.h | svg canvas interface file

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

	XDL_API canvas_t create_svg_canvas(link_t_ptr svg);

	XDL_API void destroy_svg_canvas(canvas_t canv);

	XDL_API res_ctx_t svg_get_canvas_ctx(canvas_t canv);

	XDL_API link_t_ptr svg_get_canvas_doc(canvas_t canv);

	XDL_API int svg_get_canvas_type(canvas_t canv);

	XDL_API float svg_pt_to_tm(canvas_t canv, long pt, bool_t horz);

	XDL_API long svg_tm_to_pt(canvas_t canv, float tm, bool_t horz);

	XDL_API void svg_rect_tm_to_pt(canvas_t canv, xrect_t* pxr);

	XDL_API void svg_rect_pt_to_tm(canvas_t canv, xrect_t* pxr);

	XDL_API void svg_size_tm_to_pt(canvas_t canv, xsize_t* pxs);

	XDL_API void svg_size_pt_to_tm(canvas_t canv, xsize_t* pxs);

	XDL_API void svg_point_tm_to_pt(canvas_t canv, xpoint_t* ppt);

	XDL_API void svg_point_pt_to_tm(canvas_t canv, xpoint_t* ppt);

	XDL_API void svg_get_canvas_measure(canvas_t canv, if_measure_t* pif);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*SVGCANV_H*/