/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdi canvas document

	@module	gdicanv.h | canvas interface file

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

XDL_API canvas_t create_display_canvas(res_ctx_t rdc);

XDL_API void	destroy_display_canvas(canvas_t canv);

XDL_API void	set_canvas_ratio(canvas_t canv, float htpermm, float vtpermm);

XDL_API float	get_canvas_horz_size(canvas_t canv);

XDL_API float	get_canvas_vert_size(canvas_t canv);

XDL_API void	set_canvas_horz_feed(canvas_t canv, float cx);

XDL_API float	get_canvas_horz_feed(canvas_t canv);

XDL_API void	set_canvas_vert_feed(canvas_t canv, float cy);

XDL_API float	get_canvas_vert_feed(canvas_t canv);

XDL_API void	set_canvas_scale(canvas_t canv, float sca);

XDL_API float	get_canvas_scale(canvas_t canv);

XDL_API res_ctx_t get_canvas_ctx(canvas_t canv);

XDL_API int get_canvas_type(canvas_t canv);

XDL_API res_ctx_t begin_canvas_paint(canvas_t canv, res_ctx_t rdc, long width, long height);

XDL_API void	end_canvas_paint(canvas_t canv, res_ctx_t rdc, const xrect_t* pxr);

XDL_API void get_canvas_measure(canvas_t canv, if_measure_t* pif);

#ifdef XDK_SUPPORT_CONTEXT_PRINTER

XDL_API canvas_t create_printer_canvas(res_ctx_t rdc);

XDL_API void	destroy_printer_canvas(canvas_t canv);

#endif //XDK_SUPPORT_CONTEXT_PRINTER


XDL_API float pt_to_tm(canvas_t canv, long pt, bool_t horz);

XDL_API long tm_to_pt(canvas_t canv, float tm, bool_t horz);

XDL_API void rect_tm_to_pt(canvas_t canv, xrect_t* pxr);

XDL_API void rect_pt_to_tm(canvas_t canv, xrect_t* pxr);

XDL_API void size_tm_to_pt(canvas_t canv, xsize_t* pxs);

XDL_API void size_pt_to_tm(canvas_t canv, xsize_t* pxs);

XDL_API void point_tm_to_pt(canvas_t canv, xpoint_t* ppt);

XDL_API void point_pt_to_tm(canvas_t canv, xpoint_t* ppt);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*GDICANV_H*/