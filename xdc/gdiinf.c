/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gdi document

	@module	impgdi.c | implement file

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
#include "impgdi.h"

#include "xdcimp.h"
#include "xdcinit.h"

#if defined(XDU_SUPPORT_CONTEXT)

void get_canvas_interface(canvas_t canv, if_drawing_t* pif)
{
	pif->ctx = (void*)canv;

	pif->pf_get_visual_interface = get_visual_interface;
	pif->pf_get_visual_handle = get_canvas_visual;

	pif->pf_get_measure = get_canvas_measure;

	pif->pf_rect_tm_to_pt = rect_tm_to_pt;
	pif->pf_rect_pt_to_tm = rect_pt_to_tm;
	pif->pf_size_tm_to_pt = size_tm_to_pt;
	pif->pf_size_pt_to_tm = size_pt_to_tm;
	pif->pf_point_tm_to_pt = point_tm_to_pt;
	pif->pf_point_pt_to_tm = point_pt_to_tm;

	pif->pf_draw_line = draw_line;
	pif->pf_draw_bezier = draw_bezier;
	pif->pf_draw_curve = draw_curve;
	pif->pf_draw_arc = draw_arc;
	pif->pf_draw_polyline = draw_polyline;

	pif->pf_draw_rect = draw_rect;
	pif->pf_draw_triangle = draw_triangle;
	pif->pf_draw_round = draw_round;
	pif->pf_draw_ellipse = draw_ellipse;
	pif->pf_draw_pie = draw_pie;
	pif->pf_draw_fan = draw_fan;
	pif->pf_calc_fan = calc_fan;
	pif->pf_draw_polygon = draw_polygon;
	pif->pf_draw_equalgon = draw_equalgon;
	pif->pf_calc_equalgon = calc_equalgon;
	pif->pf_draw_path = draw_path;

	pif->pf_text_metric = text_metric;
	pif->pf_text_size = text_size;
	pif->pf_text_rect = text_rect;
	pif->pf_draw_text = draw_text;
	pif->pf_text_out = text_out;
	pif->pf_multi_line = multi_line;

	pif->pf_color_out = color_out;
	pif->pf_draw_image = draw_image;
	pif->pf_draw_icon = draw_icon;
	pif->pf_draw_thumb = draw_thumb;
	pif->pf_draw_bitmap = draw_bitmap;
}

void get_visual_interface(visual_t visu, if_drawing_t* piv)
{
	piv->ctx = (void*)visu;

	piv->pf_get_measure = get_visual_measure;

	piv->pf_draw_line = draw_line_raw;
	piv->pf_draw_bezier = draw_bezier_raw;
	piv->pf_draw_curve = draw_curve_raw;
	piv->pf_draw_arc = draw_arc_raw;
	piv->pf_draw_polyline = draw_polyline_raw;

	piv->pf_draw_rect = draw_rect_raw;
	piv->pf_draw_triangle = draw_triangle_raw;
	piv->pf_draw_round = draw_round_raw;
	piv->pf_draw_ellipse = draw_ellipse_raw;
	piv->pf_draw_pie = draw_pie_raw;
	piv->pf_draw_fan = draw_fan_raw;
	piv->pf_calc_fan = calc_fan_raw;
	piv->pf_draw_polygon = draw_polygon_raw;
	piv->pf_draw_equalgon = draw_equalgon_raw;
	piv->pf_calc_equalgon = calc_equalgon_raw;
	piv->pf_draw_path = draw_path_raw;

	piv->pf_text_metric = text_metric_raw;
	piv->pf_text_size = text_size_raw;
	piv->pf_text_rect = text_rect_raw;
	piv->pf_draw_text = draw_text_raw;
	piv->pf_text_out = text_out_raw;

	piv->pf_color_out = color_out_raw;
	piv->pf_draw_image = draw_image_raw;
	piv->pf_draw_icon = draw_icon_raw;
	piv->pf_draw_thumb = draw_thumb_raw;
	piv->pf_draw_bitmap = draw_bitmap_raw;

	piv->pf_inclip_rect = inclip_rect_raw;
	piv->pf_exclip_rect = exclip_rect_raw;
	piv->pf_gradient_rect = gradient_rect_raw;
	piv->pf_alphablend_rect = alphablend_rect_raw;
}

void get_visual_measure(visual_t view, if_measure_t* pim)
{
	pim->ctx = (void*)view;

	pim->pf_measure_pixel = pixel_metric_raw;
	pim->pf_measure_font = text_metric_raw;
	pim->pf_measure_size = text_size_raw;
	pim->pf_measure_rect = text_rect_raw;
}

void get_canvas_measure(canvas_t canv, if_measure_t* pim)
{
	pim->ctx = (void*)canv;

	pim->pf_measure_pixel = pixel_metric;
	pim->pf_measure_font = text_metric;
	pim->pf_measure_size = text_size;
	pim->pf_measure_rect = text_rect;
}

#endif /*XDU_SUPPORT_CONTEXT*/
