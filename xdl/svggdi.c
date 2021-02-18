/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg gdi document

	@module	svggdi.c | implement file

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

#include "svggdi.h"
#include "svgcanv.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_VIEW)

void svg_get_measure_raw(visual_t view, measure_interface* pim)
{
	pim->ctx = (void*)view;

	pim->pf_measure_pixel = svg_pixel_metric_raw;
	pim->pf_measure_font = svg_text_metric_raw;
	pim->pf_measure_size = svg_text_size_raw;
	pim->pf_measure_rect = svg_text_rect_raw;
}

void svg_get_measure(canvas_t canv, measure_interface* pim)
{
	pim->ctx = (void*)canv;

	pim->pf_measure_pixel = svg_pixel_metric;
	pim->pf_measure_font = svg_text_metric;
	pim->pf_measure_size = svg_text_size;
	pim->pf_measure_rect = svg_text_rect;
}

/***************************************************************************************************************/

void svg_draw_line_raw(visual_t view, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_line_to_svg_node(nlk, pxp, ppt1, ppt2);
}

void svg_draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	visual_t view;
	xpoint_t pt[2];

	view = svg_get_canvas_visual(canv);

	pt[0].fx = ppt1->fx;
	pt[0].fy = ppt1->fy;
	pt[1].fx = ppt2->fx;
	pt[1].fy = ppt2->fy;

	svg_point_tm_to_pt(canv, &pt[0]);
	svg_point_tm_to_pt(canv, &pt[1]);

	svg_draw_line_raw(view, pxp, &pt[0], &pt[1]);
}

void svg_draw_polyline_raw(visual_t view, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_polyline_to_svg_node(nlk, pxp, ppt, n);
}

void svg_draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	visual_t view;
	xpoint_t* pa;
	int i;

	view = svg_get_canvas_visual(canv);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	for (i = 0; i < n; i++)
	{
		pa[i].fx = ppt[i].fx;
		pa[i].fy = ppt[i].fy;
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	svg_draw_polyline_raw(view, pxp, pa, n);

	xmem_free(pa);
}

void svg_draw_bezier_raw(visual_t view, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_bezier_to_svg_node(nlk, pxp, ppt1, ppt2, ppt3, ppt4);
}

void svg_draw_bezier(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	visual_t view;
	xpoint_t pt[4];

	view = svg_get_canvas_visual(canv);

	pt[0].fx = ppt1->fx;
	pt[0].fy = ppt1->fy;
	pt[1].fx = ppt2->fx;
	pt[1].fy = ppt2->fy;
	pt[2].fx = ppt3->fx;
	pt[2].fy = ppt3->fy;
	pt[3].fx = ppt4->fx;
	pt[3].fy = ppt4->fy;

	svg_point_tm_to_pt(canv, &pt[0]);
	svg_point_tm_to_pt(canv, &pt[1]);
	svg_point_tm_to_pt(canv, &pt[2]);
	svg_point_tm_to_pt(canv, &pt[3]);

	svg_draw_bezier_raw(view, pxp, &pt[0], &pt[1], &pt[2], &pt[3]);
}

void svg_draw_curve_raw(visual_t view, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_curve_to_svg_node(nlk, pxp, ppt, n);
}

void svg_draw_curve(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	visual_t view;
	xpoint_t* pa;
	int i;

	view = svg_get_canvas_visual(canv);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	
	for (i = 0; i < n; i++)
	{
		xmem_copy((void*)&pa[i], (void*)&ppt[i], sizeof(xpoint_t));
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	svg_draw_curve_raw(view, pxp, pa, n);

	xmem_free(pa);
}


void svg_draw_arc_raw(visual_t view, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xsize_t* pxs, bool_t sflag, bool_t lflag)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_arc_to_svg_node(nlk, pxp, ppt1, ppt2, pxs->w, pxs->h, sflag, lflag);
}

void svg_draw_arc(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xsize_t* pxs, bool_t sflag, bool_t lflag)
{
	visual_t view;
	xpoint_t pt1, pt2;
	xsize_t xs;

	view = svg_get_canvas_visual(canv);

	pt1.fx = ppt1->fx;
	pt1.fy = ppt1->fy;
	svg_point_tm_to_pt(canv, &pt1);

	pt2.fx = ppt2->fx;
	pt2.fy = ppt2->fy;
	svg_point_tm_to_pt(canv, &pt2);

	xs.fw = pxs->fw;
	xs.fh = pxs->fh;
	svg_size_tm_to_pt(canv, &xs);

	svg_draw_arc_raw(view, pxp, &pt1, &pt2, &xs, sflag, lflag);
}

void svg_draw_triangle_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, const tchar_t* orient)
{
	xpoint_t pt[4] = { 0 };
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	if (xsicmp(orient, GDI_ATTR_ORIENT_TOP) == 0)
	{
		pt[0].x = prt->x + prt->w / 2;
		pt[0].y = prt->y;
		pt[1].x = prt->x + prt->w;
		pt[1].y = prt->y + prt->h;
		pt[2].x = prt->x;
		pt[2].y = prt->y + prt->h;
		pt[3].x = prt->x + prt->w / 2;
		pt[3].y = prt->y;

		write_polygon_to_svg_node(nlk, pxp, pxb, pt, 3);
	}
	else if (xsicmp(orient, GDI_ATTR_ORIENT_BOTTOM) == 0)
	{
		pt[0].x = prt->x + prt->w / 2;
		pt[0].y = prt->y + prt->h;
		pt[1].x = prt->x;
		pt[1].y = prt->y;
		pt[2].x = prt->x + prt->w;
		pt[2].y = prt->y;
		pt[3].x = prt->x + prt->w / 2;
		pt[3].y = prt->y + prt->h;

		write_polygon_to_svg_node(nlk, pxp, pxb, pt, 3);
	}
	else if (xsicmp(orient, GDI_ATTR_ORIENT_LEFT) == 0)
	{
		pt[0].x = prt->x;
		pt[0].y = prt->y + prt->h / 2;
		pt[1].x = prt->x + prt->w;
		pt[1].y = prt->y;
		pt[2].x = prt->x + prt->w;
		pt[2].y = prt->y + prt->h;
		pt[3].x = prt->x;
		pt[3].y = prt->y + prt->h / 2;

		write_polygon_to_svg_node(nlk, pxp, pxb, pt, 3);
	}
	else if (xsicmp(orient, GDI_ATTR_ORIENT_RIGHT) == 0)
	{
		pt[0].x = prt->x + prt->w;
		pt[0].y = prt->y + prt->h / 2;
		pt[1].x = prt->x;
		pt[1].y = prt->y + prt->h;
		pt[2].x = prt->x;
		pt[2].y = prt->y;
		pt[3].x = prt->x + prt->w;
		pt[3].y = prt->y + prt->h / 2;

		write_polygon_to_svg_node(nlk, pxp, pxb, pt, 3);
	}
}

void svg_draw_triangle(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, const tchar_t* orient)
{
	visual_t view;
	xrect_t xr;

	view = svg_get_canvas_visual(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_triangle_raw(view, pxp, pxb, &xr, orient);
}

void svg_draw_rect_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_rect_to_svg_node(nlk, pxp, pxb, pxr);
}

void svg_draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	visual_t view;
	xrect_t xr;

	view = svg_get_canvas_visual(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_rect_raw(view, pxp, pxb, &xr);
}

void svg_draw_round_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_round_to_svg_node(nlk, pxp, pxb, pxr, pxr->w / 10, pxr->h / 10);
}

void svg_draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	visual_t view;
	xrect_t xr;

	view = svg_get_canvas_visual(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_round_raw(view, pxp, pxb, &xr);
}

void svg_draw_ellipse_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_ellipse_to_svg_node(nlk, pxp, pxb, pxr);
}

void svg_draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	visual_t view;
	xrect_t xr;

	view = svg_get_canvas_visual(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_ellipse_raw(view, pxp, pxb, &xr);
}

void svg_draw_pie_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, double fang, double tang)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_pie_to_svg_node(nlk, pxp, pxb, prt, fang, tang);
}

void svg_draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, double fang, double tang)
{
	visual_t view;
	xrect_t xr;

	view = svg_get_canvas_visual(canv);

	xr.fx = prt->fx;
	xr.fy = prt->fy;
	xr.fw = prt->fw;
	xr.fh = prt->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_pie_raw(view, pxp, pxb, &xr, fang, tang);
}

void svg_calc_fan_raw(visual_t view, const xpoint_t* ppt, const xsize_t* pxs, double fang, double tang, xpoint_t* pa, int n)
{
	int r1, r2;
	r1 = pxs->w;
	r2 = pxs->w - pxs->h;

	if (n > 0)
	{
		pa[0].x = ppt->x + (int)((float)(r1)* cos(fang));
		pa[0].y = ppt->y + (int)((float)(r1)* sin(fang));
	}

	if (n > 1)
	{
		pa[1].x = ppt->x + (int)((float)(r1)* cos(tang));
		pa[1].y = ppt->y + (int)((float)(r1)* sin(tang));
	}

	if (n > 2)
	{
		pa[2].x = ppt->x + (int)((float)(r2)* cos(tang));
		pa[2].y = ppt->y + (int)((float)(r2)* sin(tang));
	}

	if (n > 3)
	{
		pa[3].x = ppt->x + (int)((float)(r2)* cos(fang));
		pa[3].y = ppt->y + (int)((float)(r2)* sin(fang));
	}
}

void svg_calc_fan(canvas_t canv, const xpoint_t* ppt, const xsize_t* pxs, double fang, double tang, xpoint_t* pa, int n)
{
	float r1, r2;
	r1 = pxs->fw;
	r2 = pxs->fw - pxs->fh;

	if (n > 0)
	{
		pa[0].fx = ppt->fx + (float)(r1 * cos(fang));
		pa[0].fy = ppt->fy + (float)(r1 * sin(fang));
	}

	if (n > 1)
	{
		pa[1].fx = ppt->fx + (float)(r1 * cos(tang));
		pa[1].fy = ppt->fy + (float)(r1 * sin(tang));
	}

	if (n > 2)
	{
		pa[2].fx = ppt->fx + (float)(r2 * cos(tang));
		pa[2].fy = ppt->fy + (float)(r2 * sin(tang));
	}

	if (n > 3)
	{
		pa[3].fx = ppt->fx + (float)(r2 * cos(fang));
		pa[3].fy = ppt->fy + (float)(r2 * sin(fang));
	}
}

void svg_draw_fan_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, const xsize_t* pxs, double fang, double tang)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_fan_to_svg_node(nlk, pxp, pxb, ppt, pxs->w, pxs->h, fang, tang);
}

void svg_draw_fan(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, const xsize_t* pxs, double fang, double tang)
{
	visual_t view;
	xrect_t xr;

	view = svg_get_canvas_visual(canv);

	xr.fx = ppt->fx;
	xr.fy = ppt->fy;
	xr.fw = pxs->fw;
	xr.fh = pxs->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_fan_raw(view, pxp, pxb, RECTPOINT(&xr), RECTSIZE(&xr), fang, tang);
}

void svg_draw_polygon_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_polygon_to_svg_node(nlk, pxp, pxb, ppt, n);
}

void svg_draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n)
{
	visual_t view;
	xpoint_t* pa;
	int i;

	view = svg_get_canvas_visual(canv);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	for (i = 0; i < n; i++)
	{
		pa[i].fx = ppt[i].fx;
		pa[i].fy = ppt[i].fy;
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	svg_draw_polygon_raw(view, pxp, pxb, pa, n);

	xmem_free(pa);
}

void svg_calc_equalgon_raw(visual_t view, const xpoint_t* ppt, const xspan_t* pxn, int n, xpoint_t* pa)
{
	double a;
	int i, j;

	if (n < 3) return;

	a = 2 * XPI / n;

	if (n % 2)
	{
		pa[0].x = ppt->x;
		pa[0].y = ppt->y - pxn->r;

		for (i = 1; i <= n / 2; i++)
		{
			pa[i].x = ppt->x + (int)(pxn->r * cos(a * i - XPI / 2));
			pa[i].y = ppt->y + (int)(pxn->r * sin(a * i - XPI / 2));
		}

		for (j = i; j < n; j++)
		{
			pa[j].x = ppt->x - (pa[i - 1].x - ppt->x);
			pa[j].y = pa[i - 1].y;
			i--;
		}
	}
	else
	{
		for (i = 0; i < n / 2; i++)
		{
			pa[i].x = ppt->x + (int)(pxn->r * cos(a * i + a / 2 - XPI / 2));
			pa[i].y = ppt->y + (int)(pxn->r * sin(a * i + a / 2 - XPI / 2));
		}

		for (j = i; j < n; j++)
		{
			pa[j].x = ppt->x - (pa[i - 1].x - ppt->x);
			pa[j].y = pa[i - 1].y;
			i--;
		}
	}
}

void svg_calc_equalgon(canvas_t canv, const xpoint_t* ppt, const xspan_t* pxn, int n, xpoint_t* pa)
{
	visual_t view;

	xpoint_t pt;
	xspan_t xn;
	int i;

	view = svg_get_canvas_visual(canv);

	if (n < 3) return;

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;

	svg_point_tm_to_pt(canv, &pt);

	xn.fr = pxn->fr;
	svg_span_tm_to_pt(canv, &xn);

	svg_calc_equalgon_raw(view, &pt, &xn, n, pa);

	for (i = 0; i < n; i++)
	{
		svg_point_pt_to_tm(canv, &(pa[i]));
	}
}

void svg_draw_equalgon_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, const xspan_t* pxn, int n)
{
	xpoint_t* pa;

	if (n < 3) return;

	pa = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)* n);

	svg_calc_equalgon_raw(view, ppt, pxn, n, pa);

	svg_draw_polygon_raw(view, pxp, pxb, pa, n);

	xmem_free(pa);
}

void svg_draw_equalgon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, const xspan_t* pxn, int n)
{
	visual_t view;

	xpoint_t pt;
	xspan_t xn;

	view = svg_get_canvas_visual(canv);

	if (n < 3) return;

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;

	svg_point_tm_to_pt(canv, &pt);

	xn.fr = pxn->fr;
	svg_span_tm_to_pt(canv, &xn);

	svg_draw_equalgon_raw(view, pxp, pxb, &pt, &xn, n);
}


void svg_draw_path_raw(visual_t view, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_path_to_svg_node(nlk, pxp, pxb, aa, -1, pa, n);
}

void svg_draw_path(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n)
{
	visual_t view;
	xpoint_t* ppt;
	int i,j;

	view = svg_get_canvas_visual(canv);

	if (is_null(aa))
		return;

	ppt = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));

	xmem_copy((void*)ppt, (void*)pa, n * sizeof(xpoint_t));

	i = j = 0;
	while (*(aa + j))
	{
		if (*(aa + j) == _T('M') || *(aa + j) == _T('m'))
		{
			svg_point_tm_to_pt(canv, &ppt[i]);
			i += 1;
		}
		else if (*(aa + j) == _T('L') || *(aa + j) == _T('l'))
		{
			svg_point_tm_to_pt(canv, &ppt[i]);
			i += 1;
		}
		else if (*(aa + j) == _T('Q') || *(aa + j) == _T('q'))
		{
			svg_point_tm_to_pt(canv, &ppt[i]);
			svg_point_tm_to_pt(canv, &ppt[i + 1]);
			i += 2;
		}
		else if (*(aa + j) == _T('T') || *(aa + j) == _T('t'))
		{
			svg_point_tm_to_pt(canv, &ppt[i]);
			i += 1;
		}
		else if (*(aa + j) == _T('C') || *(aa + j) == _T('c'))
		{
			svg_point_tm_to_pt(canv, &ppt[i]);
			svg_point_tm_to_pt(canv, &ppt[i + 1]);
			svg_point_tm_to_pt(canv, &ppt[i + 2]);
			i += 3;
		}
		else if (*(aa + j) == _T('S') || *(aa + j) == _T('s'))
		{
			svg_point_tm_to_pt(canv, &ppt[i]);
			svg_point_tm_to_pt(canv, &ppt[i + 1]);
			i += 2;
		}
		else if (*(aa + j) == _T('A') || *(aa + j) == _T('a'))
		{
			svg_size_tm_to_pt(canv, (xsize_t*)(&ppt[i]));
			svg_size_tm_to_pt(canv, (xsize_t*)(&ppt[i + 1]));
			svg_point_tm_to_pt(canv, &ppt[i + 2]);
			i += 3;
		}
		else if (*(aa + j) == _T('Z') || *(aa + j) == _T('z'))
		{
			break;
		}

		j++;
	}

	svg_draw_path_raw(view, pxp, pxb, aa, ppt, n);

	xmem_free(ppt);
}

void svg_multi_line_raw(visual_t view, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr)
{
	link_t_ptr g, nlk;

	float line_rati;
	int th, lh;
	int i, rows;
	xpoint_t pt1, pt2;
	xsize_t xs;

	g = svg_get_visual_doc(view);

	if (is_null(pxa->line_height))
		line_rati = xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = xstof(pxa->line_height);

	if (line_rati < 1.0)
		line_rati = 1.0;

	svg_text_metric_raw(view, pxf, &xs);

	th = xs.h;
	lh = (int)((float)th * (line_rati - 1.0));

	rows = pxr->h / (th + lh);

	pt1.x = pxr->x;
	pt1.y = pxr->y + th + lh;
	pt2.x = pxr->x + pxr->w;
	pt2.y = pxr->y + th + lh;

	for (i = 0; i < rows; i++)
	{
		nlk = insert_svg_node(g);

		write_line_to_svg_node(nlk, pxp, &pt1, &pt2);

		pt1.y += (th + lh);
		pt2.y += (th + lh);
	}
}

void svg_multi_line(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr)
{
	visual_t view;
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	svg_rect_tm_to_pt(canv, &xr);

	view = svg_get_canvas_visual(canv);

	svg_multi_line_raw(view, pxf, pxa, pxp, &xr);
}

void svg_text_out_raw(visual_t view, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	xrect_t xr;
	xface_t xa;
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = 0;
	xr.h = 0;

	svg_text_size_raw(view, pxf, txt, len, RECTSIZE(&xr));
	default_xface(&xa);

	write_text_to_svg_node(nlk, pxf, &xa, &xr, txt, len);
}

void svg_text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	xpoint_t pt = { 0 };
	visual_t view;

	view = svg_get_canvas_visual(canv);

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;

	svg_point_tm_to_pt(canv, &pt);

	svg_text_out_raw(view, pxf, &pt, txt, len);
}

static void _draw_single_text_raw(visual_t view, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	link_t_ptr g, nlk;
	
	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_text_to_svg_node(nlk, pxf, pxa, pxr, txt, len);
}

static void _draw_multi_text_raw(visual_t view, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	link_t_ptr g, nlk;
	int n, m = 0, total = 0, w = 0;
	float px, pm;
	xrect_t xr;

	if (len < 0)
		len = xslen(txt);

	if (is_null(txt) || !len)
		return;

	font_metric_by_pt(xstof(pxf->size), &pm, &px);

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	xr.h = (int)(px + 0.5);

	g = svg_get_visual_doc(view);
	m = 0;
	while (total < len)
	{
		n = next_word((txt + total), (len - total));
		m += n;
		total += n;
		w += (int)(px);
		if ((w >= pxr->w) || (total == len && w))
		{
			nlk = insert_svg_node(g);
			write_text_to_svg_node(nlk, pxf, pxa, &xr, (txt + total - m), m);

			m = 0;
			w = 0;
			xr.y += xr.h;
		}
	}
}

void svg_draw_text_raw(visual_t view, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	if (is_null(pxa->text_wrap))
		_draw_single_text_raw(view, pxf, pxa, pxr, txt, len);
	else
		_draw_multi_text_raw(view, pxf, pxa, pxr, txt, len);
}

void svg_draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	visual_t view;
	xrect_t xr;

	view = svg_get_canvas_visual(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_text_raw(view, pxf, pxa, &xr, txt, len);
}

void svg_text_rect_raw(visual_t view, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr)
{
	int n, m = 0, total = 0, w = 0, h = 0;
	float px, pm;

	if (len < 0)
		len = xslen(txt);

	if (is_null(txt) || !len)
		return;

	font_metric_by_pt(xstof(pxf->size), &pm, &px);
	h = (int)(px + 0.5);
	m = 0;
	while (total < len)
	{
		n = next_word((txt + total), (len - total));
		m += n;
		total += n;
		w += (int)(px);
		if ((w >= pxr->w) || (total == len && w))
		{
			m = 0;
			w = 0;

			if (n)
				h += (int)(px + 0.5);
		}
	}

	pxr->h = h;
}

void svg_text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr)
{
	int n, m = 0, total = 0;
	float fw = 0.0f, fh = 0.0f;
	float px, pm;

	if (len < 0)
		len = xslen(txt);

	if (is_null(txt) || !len)
		return;

	font_metric_by_pt(xstof(pxf->size), &pm, &px);
	fh = px;
	m = 0;
	while (total < len)
	{
		n = next_word((txt + total), (len - total));
		m += n;
		total += n;
		fw += px;
		if ((fw >= pxr->fw) || (total == len && (int)fw))
		{
			m = 0;
			fw = 0.0f;

			if (n)
				fh += px;
		}
	}

	pxr->fh = fh;
}

void svg_text_size_raw(visual_t view, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	float pm, mm = 0.0f;
	int n, total = 0;
	byte_t chs[5];

	font_metric_by_pt(xstof(pxf->size), &pm, NULL);

	if (len < 0)
		len = xslen(txt);
	if (is_null(txt) || !len)
	{
		pxs->w = 0;
		pxs->h = 0;
		return ;
	}

	while (n = next_word((txt + total), (len - total)))
	{
		if (n > 1)
		{
			mm += pm;
		}
		else if (n > 0)
		{
#if defined(_UNICODE) || defined(UNICODE)
			if(ucs_byte_to_utf(*(txt + total), chs) > 1)
				mm += pm;
			else
				mm += (float)(pm * 0.75);
#else
			mm += (float)(pm * 0.75);
#endif
		}

		total += n;
	}

	pxs->w = svg_tm_to_pt_raw(view, mm, 1) - svg_tm_to_pt_raw(view, 0, 1);
	pxs->h = svg_tm_to_pt_raw(view, pm * 1.2f, 0) - svg_tm_to_pt_raw(view, 0, 0);
}

void svg_text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	float pm, mm = 0.0f;
	int n, total = 0;
	byte_t chs[5];

	font_metric_by_pt(xstof(pxf->size), &pm, NULL);

	if (len < 0)
		len = xslen(txt);
	if (is_null(txt) || !len)
	{
		pxs->w = 0;
		pxs->h = 0;
		return;
	}

	while (n = next_word((txt + total), (len - total)))
	{
		if (n > 1)
		{
			mm += pm;
		}
		else if (n > 0)
		{
#if defined(_UNICODE) || defined(UNICODE)
			if (ucs_byte_to_utf(*(txt + total), chs) > 1)
				mm += pm;
			else
				mm += (float)(pm * 0.75);
#else
			mm += (float)(pm * 0.75);
#endif
		}

		total += n;
	}

	pxs->fw = mm;
	pxs->fh = pm * 1.2f;
}

void svg_text_metric_raw(visual_t view, const xfont_t* pxf, xsize_t* pxs)
{
	float mm;

	font_metric_by_pt(xstof(pxf->size), &mm, NULL);

	pxs->w = svg_tm_to_pt_raw(view, mm, 1) - svg_tm_to_pt_raw(view, 0, 1);
	pxs->h = svg_tm_to_pt_raw(view, mm, 0) - svg_tm_to_pt_raw(view, 0, 0);
}

void svg_text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs)
{
	float mm;

	font_metric_by_pt(xstof(pxf->size), &mm, NULL);

	pxs->fw = mm;
	pxs->fh = mm;
}

float svg_pixel_metric_raw(visual_t view, bool_t horz)
{
	link_t_ptr svg;
	xrect_t vb;

	svg = svg_get_visual_doc(view);

	get_svg_viewbox(svg, &vb);

	if (horz)
	{
		return (float)((float)vb.w / get_svg_width(svg));
	}
	else
	{
		return (float)((float)vb.h / get_svg_height(svg));
	}
}

float svg_pixel_metric(canvas_t canv, bool_t horz)
{
	visual_t view;

	view = svg_get_canvas_visual(canv);

	return svg_pixel_metric_raw(view, horz);
}

void svg_color_out_raw(visual_t view, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len)
{
	xrect_t xr;
	xcolor_t xc;
	xbrush_t xb;
	tchar_t* val;
	int vlen;
	tchar_t clr[CLR_LEN + 1];
	int n, total = 0;

	if (len < 0)
		len = xslen(rgbstr);

	default_xbrush(&xb);

	while (n = parse_string_token((rgbstr + total), (len - total), _T(';'), &val, &vlen))
	{
		total += n;

		xsncpy(clr, val, CLR_LEN);
		parse_xcolor(&xc, clr);
		format_xcolor(&xc, xb.color);

		svg_draw_rect_raw(view, NULL, &xb, &xr);

		if (horz)
			xr.x += xr.w;
		else
			xr.y += xr.h;
	}
}

void svg_color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len)
{
	xrect_t xr;
	xcolor_t xc;
	xbrush_t xb;
	tchar_t* val;
	int vlen;
	tchar_t clr[CLR_LEN + 1];
	int n, total = 0;

	if (len < 0)
		len = xslen(rgbstr);

	default_xbrush(&xb);

	while (n = parse_string_token((rgbstr + total), (len - total), _T(';'), &val, &vlen))
	{
		total += n;

		xsncpy(clr, val, CLR_LEN);
		parse_xcolor(&xc, clr);
		format_xcolor(&xc, xb.color);

		svg_draw_rect(canv, NULL, &xb, &xr);

		if (horz)
			xr.fx += xr.fw;
		else
			xr.fy += xr.fh;
	}
}

void svg_draw_image_raw(visual_t view, const ximage_t* pxi, const xrect_t* pxr)
{
	link_t_ptr g, nlk;

	g = svg_get_visual_doc(view);

	nlk = insert_svg_node(g);

	write_ximage_to_svg_node(nlk, pxi, pxr);
}

void svg_draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr)
{
	visual_t view;
	xrect_t xr;

	if (is_null(pxi->source))
		return;

	view = svg_get_canvas_visual(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_image_raw(view, pxi, &xr);
}

void svg_draw_icon_raw(visual_t view, const tchar_t* iname, const xrect_t* prt)
{
}

void svg_draw_icon(canvas_t canv, const tchar_t* iname, const xrect_t* prt)
{
}

void svg_draw_thumb_raw(visual_t view, const tchar_t* fname, const xrect_t* prt)
{
}

void svg_draw_thumb(canvas_t canv, const tchar_t* fname, const xrect_t* prt)
{
}


#endif /*XDL_SUPPORT_VIEW*/

