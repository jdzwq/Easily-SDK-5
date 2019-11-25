/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc interface document

	@module	xdlinf.c | implement file

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

#include "gdiinf.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlview.h"

#if defined(XDK_SUPPORT_CONTEXT)

if_canvas_t* create_canvas_interface(canvas_t canv)
{
	if_canvas_t* pic;
	
	pic = (if_canvas_t*)xmem_alloc(sizeof(if_canvas_t));

	pic->canvas = (canvas_t)canv;

	pic->pf_canvas_type = get_canvas_type;

	pic->pf_draw_3dline = draw_3dline;
	pic->pf_draw_3drect = draw_3drect;
	pic->pf_draw_arrow = draw_arrow;
	pic->pf_draw_image = draw_image;
	pic->pf_draw_icon = draw_icon;
	pic->pf_draw_line = draw_line;
	pic->pf_draw_pie = draw_pie;
	pic->pf_draw_arc = draw_arc;
	pic->pf_draw_polygon = draw_polygon;
	pic->pf_draw_polyline = draw_polyline;
	pic->pf_draw_bezier = draw_bezier;
	pic->pf_draw_curve = draw_curve;
	pic->pf_draw_rect = draw_rect;
	pic->pf_draw_round = draw_round;
	pic->pf_draw_ellipse = draw_ellipse;
	pic->pf_draw_shape = draw_shape;
	pic->pf_draw_shadow = draw_shadow;
	pic->pf_multi_line = multi_line;
	pic->pf_gradient_rect = gradient_rect;
	pic->pf_alphablend_rect = alphablend_rect;
	pic->pf_draw_text = draw_text;
	pic->pf_text_out = text_out;
	pic->pf_color_out = color_out;
	pic->pf_draw_password = draw_password;
	pic->pf_draw_data = draw_data;
	pic->pf_draw_var_text = draw_var_text;

	pic->pf_draw_tag_text = draw_tag_text;
	pic->pf_calc_tag_pages = calc_tag_pages;
	pic->pf_draw_memo_text = draw_memo_text;
	pic->pf_calc_memo_pages = calc_memo_pages;
	pic->pf_draw_rich_text = draw_rich_text;
	pic->pf_calc_rich_pages = calc_rich_pages;

	pic->pf_measure_metric = text_metric;
	pic->pf_measure_rect = text_rect;
	pic->pf_measure_size = text_size;


	return pic;
}

void destroy_canvas_interface(if_canvas_t* pic)
{
	xmem_free(pic);
}

#endif /*XDK_SUPPORT_CONTEXT*/
