/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc canvas document

	@module	xdccanv.h | canvas interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#ifndef _XDCCANV_H
#define _XDCCANV_H

#include "xdcdef.h"


/****************************************************************************************************************/
XDC_API void	initial_canvas();

XDC_API void	uninitial_canvas();

XDC_API void	screen_point_size(long* pw, long* ph);

XDC_API xhand_t create_display_canvas(res_dc_t rdc);

XDC_API void	destroy_display_canvas(xhand_t canv);

XDC_API res_dc_t begin_display_paint(xhand_t canv, res_dc_t rdc, long width, long height);

XDC_API void	end_display_paint(xhand_t canv,res_dc_t rdc,const xrect_t* pxr);

XDC_API float	get_canvas_scale(xhand_t canv);

XDC_API void	set_canvas_scale(xhand_t canv,float scale);

XDC_API long	pt_to_tm(xhand_t canv,long pt,bool_t horz);

XDC_API long	tm_to_pt(xhand_t canv,long mm,bool_t horz);

XDC_API void	rect_tm_to_pt(xhand_t canv, xrect_t* pxr);

XDC_API void	rect_pt_to_tm(xhand_t canv, xrect_t* pxr);

XDC_API void	point_tm_to_pt(xhand_t canv, xpoint_t* ppt, int n);

XDC_API void	point_pt_to_tm(xhand_t canv, xpoint_t* ppt, int n);

XDC_API void	set_canvas_horz_feed(xhand_t canv,long feed);

XDC_API long	get_canvas_horz_feed(xhand_t canv);

XDC_API void	set_canvas_vert_feed(xhand_t canv,long feed);

XDC_API long	get_canvas_vert_feed(xhand_t canv);

XDC_API res_dc_t	get_canvas_dc(xhand_t canv);

/******************************************************************************************************************/


#endif