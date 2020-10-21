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
#include "svggdi.h"
#include "svgcanv.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_SVG)

if_canvas_t* svg_create_canvas_interface(canvas_t canv)
{
	if_canvas_t* pic;
	
	pic = (if_canvas_t*)xmem_alloc(sizeof(if_canvas_t));

	pic->canvas = canv;

	pic->pf_get_visual_interface = svg_get_visual_interface;
	pic->pf_get_measure = svg_get_measure;
	pic->pf_get_canvas_visual = svg_get_canvas_visual;

	pic->pf_draw_line = svg_draw_line;
	pic->pf_draw_bezier = svg_draw_bezier;
	pic->pf_draw_curve = svg_draw_curve;
	pic->pf_draw_arc = svg_draw_arc;
	pic->pf_draw_polyline = svg_draw_polyline;

	pic->pf_calc_fan = svg_calc_fan;
	pic->pf_draw_fan = svg_draw_fan;
	pic->pf_draw_pie = svg_draw_pie;
	pic->pf_draw_arrow = svg_draw_arrow;
	pic->pf_draw_triangle = svg_draw_triangle;
	pic->pf_draw_rect = svg_draw_rect;
	pic->pf_draw_round = svg_draw_round;
	pic->pf_draw_ellipse = svg_draw_ellipse;
	pic->pf_draw_polygon = svg_draw_polygon;
	pic->pf_calc_equalgon = svg_calc_equalgon;
	pic->pf_draw_equalgon = svg_draw_equalgon;
	pic->pf_draw_path = svg_draw_path;

	pic->pf_text_metric = svg_text_metric;
	pic->pf_text_size = svg_text_size;
	pic->pf_draw_text = svg_draw_text;
	pic->pf_text_out = svg_text_out;
	pic->pf_multi_line = svg_multi_line;

	pic->pf_color_out = svg_color_out;
	pic->pf_draw_image = svg_draw_image;
	pic->pf_draw_icon = svg_draw_icon;
	pic->pf_draw_thumb = svg_draw_thumb;

	return pic;
}

void svg_destroy_canvas_interface(if_canvas_t* pic)
{
	xmem_free(pic);
}


void svg_get_visual_interface(canvas_t canv, if_visual_t* piv)
{
	piv->visual = svg_get_canvas_visual(canv);

	piv->pf_get_measure_raw = svg_get_measure_raw;

	piv->pf_draw_line_raw = svg_draw_line_raw;
	piv->pf_draw_bezier_raw = svg_draw_bezier_raw;
	piv->pf_draw_curve_raw = svg_draw_curve_raw;
	piv->pf_draw_arc_raw = svg_draw_arc_raw;
	piv->pf_draw_polyline_raw = svg_draw_polyline_raw;

	piv->pf_draw_rect_raw = svg_draw_rect_raw;
	piv->pf_draw_triangle_raw = svg_draw_triangle_raw;
	piv->pf_draw_round_raw = svg_draw_round_raw;
	piv->pf_draw_ellipse_raw = svg_draw_ellipse_raw;
	piv->pf_draw_pie_raw = svg_draw_pie_raw;
	piv->pf_draw_fan_raw = svg_draw_fan_raw;
	piv->pf_calc_fan_raw = svg_calc_fan_raw;
	piv->pf_draw_arrow_raw = svg_draw_arrow_raw;
	piv->pf_draw_polygon_raw = svg_draw_polygon_raw;
	piv->pf_draw_equalgon_raw = svg_draw_equalgon_raw;
	piv->pf_calc_equalgon_raw = svg_calc_equalgon_raw;
	piv->pf_draw_path_raw = svg_draw_path_raw;

	piv->pf_text_metric_raw = svg_text_metric_raw;
	piv->pf_text_size_raw = svg_text_size_raw;
	piv->pf_draw_text_raw = svg_draw_text_raw;
	piv->pf_text_out_raw = svg_text_out_raw;
	piv->pf_multi_line_raw = svg_multi_line_raw;

	piv->pf_color_out_raw = svg_color_out_raw;
	piv->pf_draw_image_raw = svg_draw_image_raw;
	piv->pf_draw_icon_raw = svg_draw_icon_raw;
}

if_visual_t* svg_create_visual_interface(visual_t view)
{
	if_visual_t* piv;

	piv = (if_visual_t*)xmem_alloc(sizeof(if_visual_t));

	piv->visual = view;

	piv->pf_get_measure_raw = svg_get_measure_raw;

	piv->pf_draw_line_raw = svg_draw_line_raw;
	piv->pf_draw_bezier_raw = svg_draw_bezier_raw;
	piv->pf_draw_curve_raw = svg_draw_curve_raw;
	piv->pf_draw_arc_raw = svg_draw_arc_raw;
	piv->pf_draw_polyline_raw = svg_draw_polyline_raw;

	piv->pf_draw_rect_raw = svg_draw_rect_raw;
	piv->pf_draw_triangle_raw = svg_draw_triangle_raw;
	piv->pf_draw_round_raw = svg_draw_round_raw;
	piv->pf_draw_ellipse_raw = svg_draw_ellipse_raw;
	piv->pf_draw_pie_raw = svg_draw_pie_raw;
	piv->pf_draw_fan_raw = svg_draw_fan_raw;
	piv->pf_calc_fan_raw = svg_calc_fan_raw;
	piv->pf_draw_arrow_raw = svg_draw_arrow_raw;
	piv->pf_draw_polygon_raw = svg_draw_polygon_raw;
	piv->pf_draw_equalgon_raw = svg_draw_equalgon_raw;
	piv->pf_calc_equalgon_raw = svg_calc_equalgon_raw;
	piv->pf_draw_path_raw = svg_draw_path_raw;

	piv->pf_text_metric_raw = svg_text_metric_raw;
	piv->pf_text_size_raw = svg_text_size_raw;
	piv->pf_draw_text_raw = svg_draw_text_raw;
	piv->pf_text_out_raw = svg_text_out_raw;
	piv->pf_multi_line_raw = svg_multi_line_raw;

	piv->pf_color_out_raw = svg_color_out_raw;
	piv->pf_draw_image_raw = svg_draw_image_raw;
	piv->pf_draw_icon_raw = svg_draw_icon_raw;

	return piv;
}

void svg_destroy_visual_interface(if_visual_t* pic)
{
	xmem_free(pic);
}

#endif /*XDL_SUPPORT_SVG*/