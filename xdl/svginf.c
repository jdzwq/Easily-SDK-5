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

#if defined(XDL_SUPPORT_GDI)

void svg_get_canvas_interface(canvas_t canv, if_drawing_t* pif)
{
	pif->ctx = (void*)canv;

	pif->pf_get_measure = svg_get_measure;

	pif->pf_rect_tm_to_pt = svg_rect_tm_to_pt;
	pif->pf_rect_pt_to_tm = svg_rect_pt_to_tm;
	pif->pf_size_tm_to_pt = svg_size_tm_to_pt;
	pif->pf_size_pt_to_tm = svg_size_pt_to_tm;
	pif->pf_point_tm_to_pt = svg_point_tm_to_pt;
	pif->pf_point_pt_to_tm = svg_point_pt_to_tm;

	pif->pf_draw_line = svg_draw_line;
	pif->pf_draw_bezier = svg_draw_bezier;
	pif->pf_draw_curve = svg_draw_curve;
	pif->pf_draw_arc = svg_draw_arc;
	pif->pf_draw_polyline = svg_draw_polyline;

	pif->pf_calc_fan = svg_calc_fan;
	pif->pf_draw_fan = svg_draw_fan;
	pif->pf_draw_pie = svg_draw_pie;
	pif->pf_draw_arrow = svg_draw_arrow;
	pif->pf_draw_triangle = svg_draw_triangle;
	pif->pf_draw_rect = svg_draw_rect;
	pif->pf_draw_round = svg_draw_round;
	pif->pf_draw_ellipse = svg_draw_ellipse;
	pif->pf_draw_polygon = svg_draw_polygon;
	pif->pf_calc_equalgon = svg_calc_equalgon;
	pif->pf_draw_equalgon = svg_draw_equalgon;
	pif->pf_draw_path = svg_draw_path;

	pif->pf_text_metric = svg_text_metric;
	pif->pf_text_size = svg_text_size;
	pif->pf_draw_text = svg_draw_text;
	pif->pf_text_out = svg_text_out;
	pif->pf_multi_line = svg_multi_line;

	pif->pf_color_out = svg_color_out;
	pif->pf_draw_image = svg_draw_image;
	pif->pf_draw_icon = svg_draw_icon;
	pif->pf_draw_thumb = svg_draw_thumb;

	pif->pf_get_visual_interface = svg_get_visual_interface;
	pif->pf_get_visual_handle = svg_get_canvas_visual;
}

void svg_get_visual_interface(visual_t visual, if_drawing_t* pif)
{
	pif->ctx = (void*)visual;

	pif->pf_get_measure = svg_get_measure_raw;

	pif->pf_draw_line = svg_draw_line_raw;
	pif->pf_draw_bezier = svg_draw_bezier_raw;
	pif->pf_draw_curve = svg_draw_curve_raw;
	pif->pf_draw_arc = svg_draw_arc_raw;
	pif->pf_draw_polyline = svg_draw_polyline_raw;

	pif->pf_draw_rect = svg_draw_rect_raw;
	pif->pf_draw_triangle = svg_draw_triangle_raw;
	pif->pf_draw_round = svg_draw_round_raw;
	pif->pf_draw_ellipse = svg_draw_ellipse_raw;
	pif->pf_draw_pie = svg_draw_pie_raw;
	pif->pf_draw_fan = svg_draw_fan_raw;
	pif->pf_calc_fan = svg_calc_fan_raw;
	pif->pf_draw_arrow = svg_draw_arrow_raw;
	pif->pf_draw_polygon = svg_draw_polygon_raw;
	pif->pf_draw_equalgon = svg_draw_equalgon_raw;
	pif->pf_calc_equalgon = svg_calc_equalgon_raw;
	pif->pf_draw_path = svg_draw_path_raw;

	pif->pf_text_metric = svg_text_metric_raw;
	pif->pf_text_size = svg_text_size_raw;
	pif->pf_draw_text = svg_draw_text_raw;
	pif->pf_text_out = svg_text_out_raw;
	pif->pf_multi_line = svg_multi_line_raw;

	pif->pf_color_out = svg_color_out_raw;
	pif->pf_draw_image = svg_draw_image_raw;
	pif->pf_draw_icon = svg_draw_icon_raw;
}

#endif /*XDL_SUPPORT_VIEW*/