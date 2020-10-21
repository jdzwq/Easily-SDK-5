/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc interface document

	@module	xdcinf.c | implement file

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
#include "xdcimp.h"

#if defined(XDU_SUPPORT_CONTEXT)

if_canvas_t* create_canvas_interface(canvas_t canv)
{
	if_canvas_t* pic;
	
	pic = (if_canvas_t*)xmem_alloc(sizeof(if_canvas_t));

	pic->canvas = (canvas_t)canv;

	pic->pf_get_visual_interface = get_visual_interface;
	pic->pf_get_measure = get_measure;
	pic->pf_get_canvas_visual = get_canvas_visual;

	pic->pf_rect_tm_to_pt = rect_tm_to_pt;
	pic->pf_rect_pt_to_tm = rect_pt_to_tm;
	pic->pf_size_tm_to_pt = size_tm_to_pt;
	pic->pf_size_pt_to_tm = size_pt_to_tm;
	pic->pf_point_tm_to_pt = point_tm_to_pt;
	pic->pf_point_pt_to_tm = point_pt_to_tm;

	pic->pf_draw_line = draw_line;
	pic->pf_draw_bezier = draw_bezier;
	pic->pf_draw_curve = draw_curve;
	pic->pf_draw_arc = draw_arc;
	pic->pf_draw_polyline = draw_polyline;

	pic->pf_draw_rect = draw_rect;
	pic->pf_draw_triangle = draw_triangle;
	pic->pf_draw_round = draw_round;
	pic->pf_draw_ellipse = draw_ellipse;
	pic->pf_draw_pie = draw_pie;
	pic->pf_draw_fan = draw_fan;
	pic->pf_calc_fan = calc_fan;
	pic->pf_draw_arrow = draw_arrow;
	pic->pf_draw_polygon = draw_polygon;
	pic->pf_draw_equalgon = draw_equalgon;
	pic->pf_calc_equalgon = calc_equalgon;
	pic->pf_draw_path = draw_path;

	pic->pf_text_metric = text_metric;
	pic->pf_text_size = text_size;
	pic->pf_text_rect = text_rect;
	pic->pf_draw_text = draw_text;
	pic->pf_text_out = text_out;
	pic->pf_multi_line = multi_line;

	pic->pf_color_out = color_out;
	pic->pf_draw_image = draw_image;
	pic->pf_draw_icon = draw_icon;

	return pic;
}

void destroy_canvas_interface(if_canvas_t* pic)
{
	xmem_free(pic);
}

void get_visual_interface(canvas_t canv, if_visual_t* piv)
{
	piv->visual = get_canvas_visual(canv);

	piv->pf_get_measure_raw = get_measure_raw;

	piv->pf_draw_line_raw = draw_line_raw;
	piv->pf_draw_bezier_raw = draw_bezier_raw;
	piv->pf_draw_curve_raw = draw_curve_raw;
	piv->pf_draw_arc_raw = draw_arc_raw;
	piv->pf_draw_polyline_raw = draw_polyline_raw;

	piv->pf_draw_rect_raw = draw_rect_raw;
	piv->pf_draw_triangle_raw = draw_triangle_raw;
	piv->pf_draw_round_raw = draw_round_raw;
	piv->pf_draw_ellipse_raw = draw_ellipse_raw;
	piv->pf_draw_pie_raw = draw_pie_raw;
	piv->pf_draw_fan_raw = draw_fan_raw;
	piv->pf_calc_fan_raw = calc_fan_raw;
	piv->pf_draw_arrow_raw = draw_arrow_raw;
	piv->pf_draw_polygon_raw = draw_polygon_raw;
	piv->pf_draw_equalgon_raw = draw_equalgon_raw;
	piv->pf_calc_equalgon_raw = calc_equalgon_raw;
	piv->pf_draw_path_raw = draw_path_raw;

	piv->pf_text_metric_raw = text_metric_raw;
	piv->pf_text_size_raw = text_size_raw;
	piv->pf_text_rect_raw = text_rect_raw;
	piv->pf_draw_text_raw = draw_text_raw;
	piv->pf_text_out_raw = text_out_raw;
	piv->pf_multi_line_raw = multi_line_raw;

	piv->pf_color_out_raw = color_out_raw;
	piv->pf_draw_image_raw = draw_image_raw;
	piv->pf_draw_icon_raw = draw_icon_raw;
}

if_visual_t* create_visual_interface(visual_t rdc)
{
	if_visual_t* pic;

	pic = (if_visual_t*)xmem_alloc(sizeof(if_visual_t));

	pic->visual = rdc;

	pic->pf_get_measure_raw = get_measure_raw;

	pic->pf_draw_line_raw = draw_line_raw;
	pic->pf_draw_bezier_raw = draw_bezier_raw;
	pic->pf_draw_curve_raw = draw_curve_raw;
	pic->pf_draw_arc_raw = draw_arc_raw;
	pic->pf_draw_polyline_raw = draw_polyline_raw;

	pic->pf_draw_rect_raw = draw_rect_raw;
	pic->pf_draw_triangle_raw = draw_triangle_raw;
	pic->pf_draw_round_raw = draw_round_raw;
	pic->pf_draw_ellipse_raw = draw_ellipse_raw;
	pic->pf_draw_pie_raw = draw_pie_raw;
	pic->pf_draw_fan_raw = draw_fan_raw;
	pic->pf_calc_fan_raw = calc_fan_raw;
	pic->pf_draw_arrow_raw = draw_arrow_raw;
	pic->pf_draw_polygon_raw = draw_polygon_raw;
	pic->pf_draw_equalgon_raw = draw_equalgon_raw;
	pic->pf_calc_equalgon_raw = calc_equalgon_raw;
	pic->pf_draw_path_raw = draw_path_raw;

	pic->pf_text_metric_raw = text_metric_raw;
	pic->pf_text_size_raw = text_size_raw;
	pic->pf_text_rect_raw = text_rect_raw;
	pic->pf_draw_text_raw = draw_text_raw;
	pic->pf_text_out_raw = text_out_raw;

	pic->pf_color_out_raw = color_out_raw;
	pic->pf_draw_image_raw = draw_image_raw;
	pic->pf_draw_icon_raw = draw_icon_raw;

	return pic;
}

void destroy_visual_interface(if_visual_t* pic)
{
	xmem_free(pic);
}

#endif /*XDU_SUPPORT_CONTEXT*/
