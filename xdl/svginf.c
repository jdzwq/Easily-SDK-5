/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg interface document

	@module	svginf.c | implement file

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

#include "svginf.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_SVG)

if_canvas_t* create_svg_interface(canvas_t canv)
{
	if_canvas_t* pic;
	
	pic = (if_canvas_t*)xmem_alloc(sizeof(if_canvas_t));

	pic->canvas = canv;

	pic->pf_draw_line = svg_draw_line;
	pic->pf_gradient_rect = svg_gradient_rect;
	pic->pf_alphablend_rect = svg_alphablend_rect;
	pic->pf_draw_arrow = svg_draw_arrow;
	pic->pf_draw_pie = svg_draw_pie;
	pic->pf_draw_polygon = svg_draw_polygon;
	pic->pf_draw_polyline = svg_draw_polyline;
	pic->pf_draw_bezier = svg_draw_bezier;
	pic->pf_draw_curve = svg_draw_curve;
	pic->pf_draw_rect = svg_draw_rect;
	pic->pf_draw_round = svg_draw_round;
	pic->pf_draw_ellipse = svg_draw_ellipse;
	pic->pf_draw_text = svg_draw_text;
	pic->pf_text_out = svg_text_out;
	pic->pf_color_out = svg_color_out;
	pic->pf_draw_pass = svg_draw_pass;
	pic->pf_draw_data = svg_draw_data;

	pic->pf_draw_shape = svg_draw_shape;
	pic->pf_multi_line = svg_multi_line;

	pic->pf_draw_var_text = svg_draw_var_text;
	pic->pf_draw_tag_text = svg_draw_tag_text;
	pic->pf_draw_rich_text = svg_draw_rich_text;
	pic->pf_draw_memo_text = svg_draw_memo_text;

	pic->pf_draw_image = svg_draw_image;
	pic->pf_draw_gizmo = svg_draw_gizmo;
	pic->pf_draw_code128 = svg_draw_code128;
	pic->pf_draw_pdf417 = svg_draw_pdf417;
	pic->pf_draw_qrcode = svg_draw_qrcode;

	pic->pf_draw_icon = svg_draw_icon;
	pic->pf_draw_thumb = svg_draw_thumb;

	pic->pf_measure_metric = svg_text_metric;
	pic->pf_measure_size = svg_text_size;
	pic->pf_measure_rect = svg_text_rect;

	return pic;
}

void destroy_svg_interface(if_canvas_t* pic)
{
	xmem_free(pic);
}

#endif /*XDL_SUPPORT_SVG*/