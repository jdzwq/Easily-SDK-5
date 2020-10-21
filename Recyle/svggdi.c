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
#include "svggizmo.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_SVG)

static float svg_pt_to_tm_raw(link_t_ptr g, int pt, bool_t horz)
{
	xrect_t vb;
	float htpermm, vtpermm;

	get_svg_viewbox(g, &vb);

	if (horz)
	{
		htpermm = (float)((float)vb.w / get_svg_width(g));

		return (float)((float)pt / htpermm);
	}
	else
	{
		vtpermm = (float)((float)vb.h / get_svg_height(g));

		return (float)((float)pt / vtpermm);
	}
}

static int svg_tm_to_pt_raw(link_t_ptr g, float tm, bool_t horz)
{
	xrect_t vb;
	float htpermm, vtpermm;

	get_svg_viewbox(g, &vb);

	if (horz)
	{
		htpermm = (float)((float)vb.w / get_svg_width(g));

		return (int)(tm * htpermm + 0.5);
	}
	else
	{
		vtpermm = (float)((float)vb.h / get_svg_height(g));

		return (int)(tm * vtpermm + 0.5);
	}
}

void svg_text_metric_raw(link_t_ptr g, const xfont_t* pxf, xsize_t* pxs)
{
	float mm;

	mm = font_metric(xstof(pxf->size));

	pxs->cx = svg_tm_to_pt_raw(g, mm, 1) - svg_tm_to_pt_raw(g, 0, 1);
	pxs->cy = svg_tm_to_pt_raw(g, mm, 0) - svg_tm_to_pt_raw(g, 0, 0);
}

void svg_text_size_raw(link_t_ptr g, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	float mm;

	mm = font_metric(xstof(pxf->size));

	if (len < 0) len = xslen(txt);

	pxs->cx = svg_tm_to_pt_raw(g, mm * len, 1) - svg_tm_to_pt_raw(g, 0, 1);
	pxs->cy = svg_tm_to_pt_raw(g, mm, 0) - svg_tm_to_pt_raw(g, 0, 0);
}

/*static void svg_text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs)
{
	link_t_ptr g;
	float mp = 0.0f;

	g = svg_get_canvas_doc(canv);

	font_metric_by_point(xstof(pxf->size), NULL, &mp);

	pxs->fx = svg_pt_to_tm_raw(g, mp, 1) - svg_pt_to_tm_raw(g, 0, 1);
	pxs->fy = svg_pt_to_tm_raw(g, mp, 0) - svg_pt_to_tm_raw(g, 0, 0);
}

static void svg_text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	link_t_ptr g;
	float mp = 0.0f;

	g = svg_get_canvas_doc(canv);

	font_metric_by_point(xstof(pxf->size), NULL, &mp);

	if (len < 0) len = xslen(txt);

	pxs->fx = svg_pt_to_tm(canv, mp * len, 1) - svg_pt_to_tm(canv, 0, 1);
	pxs->fy = svg_pt_to_tm(canv, mp, 0) - svg_pt_to_tm(canv, 0, 0);
}*/

void svg_text_size(canvas_t canv, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	float mm = 0.0f;

	mm = (float)font_points(xstof(pxf->size));

	if (len < 0) len = xslen(txt);

	pxs->fx = mm * len;
	pxs->fy = mm;
}

void svg_text_metric(canvas_t canv, const xfont_t* pxf, xsize_t* pxs)
{
	float mm = 0.0f;

	mm = (float)font_points(xstof(pxf->size));

	pxs->fx = mm;
	pxs->fy = mm;
}

/***************************************************************************************************************/

void svg_draw_line_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_line_to_svg_node(nlk, pxp, ppt1, ppt2);
}

void svg_draw_line(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2)
{
	link_t_ptr g;
	xpoint_t pt[2];

	g = svg_get_canvas_doc(canv);

	pt[0].fx = ppt1->fx;
	pt[0].fy = ppt1->fy;
	pt[1].fx = ppt2->fx;
	pt[1].fy = ppt2->fy;

	svg_point_tm_to_pt(canv, &pt[0]);
	svg_point_tm_to_pt(canv, &pt[1]);

	svg_draw_line_raw(g, pxp, &pt[0], &pt[1]);
}

void svg_draw_polyline_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_polyline_to_svg_node(nlk, pxp, ppt, n);
}

void svg_draw_polyline(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	link_t_ptr g;
	xpoint_t* pa;
	int i;

	g = svg_get_canvas_doc(canv);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	for (i = 0; i < n; i++)
	{
		pa[i].fx = ppt[i].fx;
		pa[i].fy = ppt[i].fy;
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	svg_draw_polyline_raw(g, pxp, pa, n);

	xmem_free(pa);
}

void svg_draw_bezier_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_bezier_to_svg_node(nlk, pxp, ppt1, ppt2, ppt3, ppt4);
}

void svg_draw_bezier(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4)
{
	link_t_ptr g;
	xpoint_t pt[4];

	g = svg_get_canvas_doc(canv);

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

	svg_draw_bezier_raw(g, pxp, &pt[0], &pt[1], &pt[2], &pt[3]);
}

void svg_draw_curve_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_curve_to_svg_node(nlk, pxp, ppt, n);
}

void svg_draw_curve(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt, int n)
{
	link_t_ptr g;
	xpoint_t* pa;
	int i;

	g = svg_get_canvas_doc(canv);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	
	for (i = 0; i < n; i++)
	{
		xmem_copy((void*)&pa[i], (void*)&ppt[i], sizeof(xpoint_t));
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	svg_draw_curve_raw(g, pxp, pa, n);

	xmem_free(pa);
}


void svg_draw_arc_raw(link_t_ptr g, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, int rx, int ry, bool_t sflag, bool_t lflag)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_arc_to_svg_node(nlk, pxp, ppt1, ppt2, rx, ry, sflag, lflag);
}

void svg_draw_arc(canvas_t canv, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, float rx, float ry, bool_t sflag, bool_t lflag)
{
	link_t_ptr g;
	xpoint_t pt1, pt2;
	xsize_t xs;

	g = svg_get_canvas_doc(canv);

	pt1.fx = ppt1->fx;
	pt1.fy = ppt1->fy;
	svg_point_tm_to_pt(canv, &pt1);

	pt2.fx = ppt2->fx;
	pt2.fy = ppt2->fy;
	svg_point_tm_to_pt(canv, &pt2);

	xs.fx = rx;
	xs.fy = ry;
	svg_size_tm_to_pt(canv, &xs);

	svg_draw_arc_raw(g, pxp, &pt1, &pt2, xs.cx, xs.cy, sflag, lflag);
}

void svg_draw_arrow_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, float alen, double arc)
{

}

void svg_draw_arrow(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, float alen, double arc)
{

}

void svg_draw_shape_raw(link_t_ptr g, const xpen_t* pxp, const xrect_t* pxr, const tchar_t* shape)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_shape_to_svg_node(nlk, pxp, pxr, shape);
}

void svg_draw_shape(canvas_t canv, const xpen_t* pxp, const xrect_t* pxr, const tchar_t* shape)
{
	link_t_ptr g;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_shape_raw(g, pxp, &xr, shape);
}

void svg_draw_triangle_raw(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, const tchar_t* orient)
{
	xpoint_t pt[4] = { 0 };
	link_t_ptr nlk;

	nlk = insert_svg_node(glk);

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
	link_t_ptr g;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_triangle_raw(g, pxp, pxb, &xr, orient);
}

void svg_draw_rect_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_rect_to_svg_node(nlk, pxp, pxb, pxr);
}

void svg_draw_rect(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_rect_raw(g, pxp, pxb, &xr);
}

void svg_draw_round_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_round_to_svg_node(nlk, pxp, pxb, pxr, pxr->w / 10, pxr->h / 10);
}

void svg_draw_round(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_round_raw(g, pxp, pxb, &xr);
}

void svg_draw_ellipse_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_ellipse_to_svg_node(nlk, pxp, pxb, pxr);
}

void svg_draw_ellipse(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr)
{
	link_t_ptr g;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_ellipse_raw(g, pxp, pxb, &xr);
}

void svg_draw_pie_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int rx, int ry, double fang, double tang)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_pie_to_svg_node(nlk, pxp, pxb, ppt, rx, ry, fang, tang);
}

void svg_draw_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float rx, float ry, double fang, double tang)
{
	link_t_ptr g;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	xr.fx = ppt->fx;
	xr.fy = ppt->fy;
	xr.fw = rx;
	xr.fh = ry;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_pie_raw(g, pxp, pxb, RECTPOINT(&xr), xr.w, xr.h, fang, tang);
}

void svg_calc_fan_raw(link_t_ptr g, const xpoint_t* ppt, int r, int s, double fang, double tang, xpoint_t* pa, int n)
{
	int r1, r2;
	r1 = r;
	r2 = r - s;

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

void svg_calc_fan(canvas_t canv, const xpoint_t* ppt, float r, float s, double fang, double tang, xpoint_t* pa, int n)
{
	float r1, r2;
	r1 = r;
	r2 = r - s;

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

void svg_draw_fan_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int r, int s, double fang, double tang)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_fan_to_svg_node(nlk, pxp, pxb, ppt, r, s, fang, tang);
}

void svg_draw_fan(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float r, float s, double fang, double tang)
{
	link_t_ptr g;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	xr.fx = ppt->fx;
	xr.fy = ppt->fy;
	xr.fw = r;
	xr.fh = s;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_fan_raw(g, pxp, pxb, RECTPOINT(&xr), xr.w, xr.h, fang, tang);
}

void svg_draw_polygon_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_polygon_to_svg_node(nlk, pxp, pxb, ppt, n);
}

void svg_draw_polygon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n)
{
	link_t_ptr g;
	xpoint_t* pa;
	int i;

	g = svg_get_canvas_doc(canv);

	pa = (xpoint_t*)xmem_alloc(n * sizeof(xpoint_t));
	for (i = 0; i < n; i++)
	{
		pa[i].fx = ppt[i].fx;
		pa[i].fy = ppt[i].fy;
		svg_point_tm_to_pt(canv, &pa[i]);
	}

	svg_draw_polygon_raw(g, pxp, pxb, pa, n);

	xmem_free(pa);
}

void svg_calc_equalgon_raw(link_t_ptr g, const xpoint_t* ppt, int r, int n, xpoint_t* pa)
{
	double a;
	int i, j;

	if (n < 3) return;

	a = 2 * XPI / n;

	if (n % 2)
	{
		pa[0].x = ppt->x;
		pa[0].y = ppt->y - r;

		for (i = 1; i <= n / 2; i++)
		{
			pa[i].x = ppt->x + (int)(r * cos(a * i - XPI / 2));
			pa[i].y = ppt->y + (int)(r * sin(a * i - XPI / 2));
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
			pa[i].x = ppt->x + (int)(r * cos(a * i + a / 2 - XPI / 2));
			pa[i].y = ppt->y + (int)(r * sin(a * i + a / 2 - XPI / 2));
		}

		for (j = i; j < n; j++)
		{
			pa[j].x = ppt->x - (pa[i - 1].x - ppt->x);
			pa[j].y = pa[i - 1].y;
			i--;
		}
	}
}

void svg_calc_equalgon(canvas_t canv, const xpoint_t* ppt, float fr, int n, xpoint_t* pa)
{
	link_t_ptr g;

	xpoint_t pt;
	xsize_t xs;
	int i;

	g = svg_get_canvas_doc(canv);

	if (n < 3) return;

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;

	svg_point_tm_to_pt(canv, &pt);

	xs.fx = fr;
	xs.fy = fr;
	svg_size_tm_to_pt(canv, &xs);

	svg_calc_equalgon_raw(g, &pt, xs.cx, n, pa);

	for (i = 0; i < n; i++)
	{
		svg_point_pt_to_tm(canv, &(pa[i]));
	}
}

void svg_draw_equalgon_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int r, int n)
{
	xpoint_t* pa;
	double a;
	int i, j;

	if (n < 3) return;

	pa = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)* n);

	svg_calc_equalgon_raw(g, ppt, r, n, pa);

	svg_draw_polygon_raw(g, pxp, pxb, pa, n);

	xmem_free(pa);
}

void svg_draw_equalgon(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, float fr, int n)
{
	link_t_ptr g;

	xpoint_t pt;
	xsize_t xs;

	g = svg_get_canvas_doc(canv);

	if (n < 3) return;

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;

	svg_point_tm_to_pt(canv, &pt);

	xs.fx = fr;
	xs.fy = fr;

	svg_size_tm_to_pt(canv, &xs);

	svg_draw_equalgon_raw(g, pxp, pxb, &pt, xs.cy, n);
}


void svg_draw_path_raw(link_t_ptr g, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_path_to_svg_node(nlk, pxp, pxb, aa, -1, pa, n);
}

void svg_draw_path(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const tchar_t* aa, const xpoint_t* pa, int n)
{
	link_t_ptr g;
	xpoint_t* ppt;
	int i,j;

	g = svg_get_canvas_doc(canv);

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

	svg_draw_path_raw(g, pxp, pxb, aa, ppt, n);

	xmem_free(ppt);
}

void svg_draw_image_raw(link_t_ptr g, const ximage_t* pxi, const xrect_t* pxr)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_ximage_to_svg_node(nlk, pxi, pxr);
}

void svg_draw_image(canvas_t canv, const ximage_t* pxi, const xrect_t* pxr)
{
	link_t_ptr g;
	xrect_t xr;

	if (is_null(pxi->source))
		return;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	svg_draw_image_raw(g, pxi, &xr);
}

void svg_multi_line_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xpen_t* pxp, const xrect_t* pxr)
{
	link_t_ptr nlk;

	float line_rati;
	int th, lh;
	int i, rows;
	xpoint_t pt1, pt2;
	xsize_t xs;

	if (is_null(pxa->line_height))
		line_rati = xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = xstof(pxa->line_height);

	if (line_rati < 1.0)
		line_rati = 1.0;

	svg_text_metric_raw(g, pxf, &xs);

	th = xs.cy;
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
	link_t_ptr g;
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	svg_rect_tm_to_pt(canv, &xr);

	g = svg_get_canvas_doc(canv);

	svg_multi_line_raw(g, pxf, pxa, pxp, &xr);
}

void svg_text_out_raw(link_t_ptr g, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	xrect_t xr;
	xface_t xa;
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	xr.x = ppt->x;
	xr.y = ppt->y;
	xr.w = 0;
	xr.h = 0;

	//svg_text_size_raw(g, pxf, txt, len, RECTSIZE(&xr));

	default_xface(&xa);

	write_text_to_svg_node(nlk, pxf, &xa, &xr, txt, len);
}

void svg_text_out(canvas_t canv, const xfont_t* pxf, const xpoint_t* ppt, const tchar_t* txt, int len)
{
	xpoint_t pt = { 0 };
	link_t_ptr g;

	g = svg_get_canvas_doc(canv);

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;

	svg_point_tm_to_pt(canv, &pt);

	svg_text_out_raw(g, pxf, &pt, txt, len);
}

void svg_color_out_raw(link_t_ptr g, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len)
{
	const tchar_t *pre, *nxt;
	xrect_t xr;
	xcolor_t xc;
	xbrush_t xb;
	tchar_t* val;
	int vlen;
	tchar_t clr[CLR_LEN + 1];

	if (len < 0)
		len = xslen(rgbstr);

	default_xbrush(&xb);

	pre = rgbstr;
	while (nxt = parse_string_token(pre, len, _T(';'), &val, &vlen))
	{
		xsncpy(clr, val, CLR_LEN);
		parse_xcolor(&xc, clr);
		format_xcolor(&xc, xb.color);

		svg_draw_rect_raw(g, NULL, &xb, &xr);

		len -= (nxt - pre);

		if (horz)
			xr.x += xr.w;
		else
			xr.y += xr.h;

		pre = nxt;
	}
}

void svg_color_out(canvas_t canv, const xrect_t* pxr, bool_t horz, const tchar_t* rgbstr, int len)
{
	const tchar_t *pre, *nxt;
	xrect_t xr;
	xcolor_t xc;
	xbrush_t xb;
	tchar_t* val;
	int vlen;
	tchar_t clr[CLR_LEN + 1];

	if (len < 0)
		len = xslen(rgbstr);

	default_xbrush(&xb);

	pre = rgbstr;
	while (nxt = parse_string_token(pre, len, _T(';'), &val, &vlen))
	{
		xsncpy(clr, val, CLR_LEN);
		parse_xcolor(&xc, clr);
		format_xcolor(&xc, xb.color);

		svg_draw_rect(canv, NULL, &xb, &xr);

		len -= (nxt - pre);

		if (horz)
			xr.fx += xr.fw;
		else
			xr.fy += xr.fh;

		pre = nxt;
	}
}

void svg_draw_pass_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	tchar_t sz_pass[INT_LEN + 1] = { 0 };

	len = format_password(txt, sz_pass, INT_LEN);

	svg_draw_text_raw(g, pxf, pxa, pxr, sz_pass, len);
}

void svg_draw_pass(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	tchar_t sz_pass[INT_LEN + 1] = { 0 };

	len = format_password(txt, sz_pass, INT_LEN);

	svg_draw_text(canv, pxf, pxa, pxr, sz_pass, len);
}

void svg_draw_data_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap)
{
	tchar_t sz_format[RES_LEN] = { 0 };
	xdate_t dt = { 0 };
	int lt;
	double db;

	xsize_t xs;
	xface_t xa;

	if (len < 0)
		len = xslen(data);

	xmem_copy((void*)&xa, (void*)pxa, sizeof(xface_t));

	if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
	{
		lt = xsntol(data, len);

		if (zeronull && !lt)
		{
			xscpy(sz_format, _T(""));
		}
		else if (!is_null(fmt))
		{
			format_integer_ex(lt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && pxa && is_null(xa.text_wrap))
		{
			svg_text_size_raw(g, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text_raw(g, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
	{
		db = xsntonum(data, len);

		if (zeronull && is_zero_numeric(db, dig))
		{
			xscpy(sz_format, _T(""));
		}
		else if (!is_null(fmt))
		{
			format_numeric(db, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size_raw(g, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text_raw(g, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
	{
		if (!is_null(fmt) && !is_null(data))
		{
			parse_datetime(&dt, data);
			format_datetime_ex(&dt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size_raw(g, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text_raw(g, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0)
	{
		if (!is_null(fmt) && !is_null(data))
		{
			parse_date(&dt, data);
			format_datetime_ex(&dt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size_raw(g, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text_raw(g, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
	{
		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size_raw(g, pxf, data, len, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text_raw(g, pxf, &xa, pxr, data, len);
	}
}

void svg_draw_data(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* data, int len, int dig, const tchar_t* type, const tchar_t* fmt, bool_t zeronull, bool_t autowrap)
{
	tchar_t sz_format[RES_LEN] = { 0 };
	xdate_t dt = { 0 };
	int lt;
	double db;

	xsize_t xs;
	xface_t xa;

	if (len < 0)
		len = xslen(data);

	xmem_copy((void*)&xa, (void*)pxa, sizeof(xface_t));

	if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
	{
		lt = xsntol(data, len);

		if (zeronull && !lt)
		{
			xscpy(sz_format, _T(""));
		}
		else if (!is_null(fmt))
		{
			format_integer_ex(lt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && pxa && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
	{
		db = xsntonum(data, len);

		if (zeronull && is_zero_numeric(db, dig))
		{
			xscpy(sz_format, _T(""));
		}
		else if (!is_null(fmt))
		{
			format_numeric(db, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
	{
		if (!is_null(fmt) && !is_null(data))
		{
			parse_datetime(&dt, data);
			format_datetime_ex(&dt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0)
	{
		if (!is_null(fmt) && !is_null(data))
		{
			parse_date(&dt, data);
			format_datetime_ex(&dt, fmt, sz_format, RES_LEN);
		}
		else
		{
			xsncpy(sz_format, data, len);
		}

		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, sz_format, -1, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, sz_format, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_STRING, -1, 0) == 0)
	{
		if (autowrap && is_null(xa.text_wrap))
		{
			svg_text_size(canv, pxf, data, len, &xs);
			if (xs.fx > pxr->fw)
			{
				xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
			}
		}

		svg_draw_text(canv, pxf, &xa, pxr, data, len);
	}
}

void svg_draw_code128_raw(link_t_ptr g, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black, span;
	dword_t i;
	int unit = 2;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

	unit = svg_tm_to_pt_raw(g, 0.3f, 1);

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = code128_encode(buf, buf_len, NULL, MAX_LONG);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = code128_encode(buf, buf_len, bar_buf, bar_len);

	xmem_free(buf);

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
		default_xpen(&xp);
		format_xcolor(pxc, xp.color);
	}

	rt.x = prt->x + unit;
	rt.y = prt->y + unit;
	rt.w = unit;
	rt.h = prt->h - 2 * unit;

	black = 0;
	for (i = 0; i < bar_len; i++)
	{
		span = (bar_buf[i] - '0');
		rt.w = span * unit;

		black = (black) ? 0 : 1;

		if (black && pxc)
		{
			svg_draw_rect_raw(g, &xp, &xb, &rt);
		}

		rt.x += rt.w;
	}

	xmem_free(bar_buf);

	prt->w = rt.x + unit - prt->x;
}

void svg_draw_code128(canvas_t canv, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black,span;
	dword_t i;
	float unit = 0.3f;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = code128_encode(buf, buf_len, NULL, MAX_LONG);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = code128_encode(buf, buf_len, bar_buf, bar_len);

	xmem_free(buf);

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
		default_xpen(&xp);
		format_xcolor(pxc, xp.color);
	}

	rt.fx = prt->fx + unit;
	rt.fy = prt->fy + unit;
	rt.fw = unit;
	rt.fh = prt->fh - 2 * unit;

	black = 0;
	for (i = 0; i < bar_len; i++)
	{
		span = (bar_buf[i] - '0');
		rt.fw = span * unit;

		black = (black) ? 0 : 1;

		if (black && pxc)
		{
			svg_draw_rect(canv, &xp, &xb, &rt);
		}
		
		rt.fx += rt.fw;
	}

	xmem_free(bar_buf);

	prt->fw = rt.fx + unit - prt->fx;
}

void svg_draw_pdf417_raw(link_t_ptr g, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black;
	int rows, cols;
	unsigned char b, c;
	int i, j;

	int unit = 4;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

	unit = svg_tm_to_pt_raw(g, 0.5f, 1);

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = pdf417_encode(buf, buf_len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = pdf417_encode(buf, buf_len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
		default_xpen(&xp);
		format_xcolor(pxc, xp.color);
	}

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.x = prt->x + unit;
		rt.w = unit;
		rt.y = prt->y + unit + i * 2 * unit;
		rt.h = 2 * unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.x += rt.w;

				black = (c & b) ? 0 : 1;

				if (black && pxc)
				{
					svg_draw_rect_raw(g, &xp, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);

	prt->w = rt.x + unit - prt->x;
	prt->h = rt.y + rt.h + unit - prt->y;
}

void svg_draw_pdf417(canvas_t canv, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black;
	int rows,cols;
	unsigned char b, c;
	int i,j;

	float unit = 0.5f;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = pdf417_encode(buf, buf_len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = pdf417_encode(buf, buf_len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);
	
	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
		default_xpen(&xp);
		format_xcolor(pxc, xp.color);
	}

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.fx = prt->fx + unit;
		rt.fw = unit;
		rt.fy = prt->fy + unit + i * 2 * unit;
		rt.fh = 2 * unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.fx += rt.fw;

				black = (c & b) ? 0 : 1;

				if (black && pxc)
				{
					svg_draw_rect(canv, &xp, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);

	prt->fw = rt.fx + unit - prt->fx;
	prt->fh = rt.fy + rt.fh + unit - prt->fy;
}

#ifdef GPL_SUPPORT_QRCODE
void svg_draw_qrcode_raw(link_t_ptr g, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black;
	int rows, cols;
	unsigned char b, c;
	int i, j;

	int unit = 4;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

	unit = svg_tm_to_pt_raw(g, 0.5f, 1);

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = qr_encode(buf, buf_len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = qr_encode(buf, buf_len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
		default_xpen(&xp);
		format_xcolor(pxc, xp.color);
	}

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.x = prt->x + unit;
		rt.w = unit;
		rt.y = prt->y + unit + i * unit;
		rt.h = unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.x += rt.w;

				black = (c & b) ? 1 : 0;

				if (black && pxc)
				{
					svg_draw_rect_raw(g, &xp, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);

	prt->w = rt.x + unit - prt->x;
	prt->h = rt.y + rt.h + unit - prt->y;
}

void svg_draw_qrcode(canvas_t canv, const xcolor_t* pxc, xrect_t* prt, const tchar_t* text, int len)
{
	int black;
	int rows,cols;
	unsigned char b, c;
	int i,j;

	float unit = 0.5f;
	xrect_t rt;
	xbrush_t xb;
	xpen_t xp;

	byte_t* buf;
	dword_t buf_len;

	byte_t* bar_buf;
	dword_t bar_len;

#ifdef _UNICODE
	buf_len = ucs_to_utf8(text, len, NULL, MAX_LONG);
#else
	buf_len = mbs_to_utf8(text, len, NULL, MAX_LONG);
#endif

	if (!buf_len) return;

	buf = (byte_t*)xmem_alloc(buf_len + 1);
#ifdef _UNICODE
	ucs_to_utf8(text, len, buf, buf_len);
#else
	mbs_to_utf8(text, len, buf, buf_len);
#endif

	bar_len = qr_encode(buf, buf_len, NULL, MAX_LONG, NULL, NULL);
	if (bar_len <= 0)
	{
		xmem_free(buf);
		return;
	}

	bar_buf = (byte_t*)xmem_alloc(bar_len + 1);
	bar_len = qr_encode(buf, buf_len, bar_buf, bar_len, &rows, &cols);

	xmem_free(buf);

	if (pxc)
	{
		default_xbrush(&xb);
		format_xcolor(pxc, xb.color);
		default_xpen(&xp);
		format_xcolor(pxc, xp.color);
	}

	len = 0;
	black = 0;
	for (i = 0; i < rows; i++)
	{
		rt.fx = prt->fx + unit;
		rt.fw = unit;
		rt.fy = prt->fy + unit + i * unit;
		rt.fh = unit;

		for (j = 0; j < cols; j++)
		{
			c = *(bar_buf + i * cols + j);
			b = 0x80;

			while (b)
			{
				rt.fx += rt.fw;

				black = (c & b) ? 1 : 0;

				if (black && pxc)
				{
					svg_draw_rect(canv, &xp, &xb, &rt);
				}

				b = b >> 1;
			}
		}
	}

	xmem_free(bar_buf);

	prt->fw = rt.fx + unit - prt->fx;
	prt->fh = rt.fy + rt.fh + unit - prt->fy;
}
#endif

void svg_draw_gizmo(canvas_t canv, const xcolor_t* pxc, const xrect_t* pxr, const tchar_t* gname)
{
	xrect_t xr;
	PF_SVG_GIZMO_MAKE pf;
	link_t_ptr g;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	g = svg_get_canvas_doc(canv);

	pf = svg_find_gizmo_maker(gname);
	if (pf)
	{
		(*pf)(g, pxc, &xr);
	}
}

void svg_draw_icon(canvas_t canv, const tchar_t* iname, const xrect_t* prt)
{
}

void svg_draw_thumb(canvas_t canv, const tchar_t* fname, const xrect_t* prt)
{
}

typedef struct _FIXTEXT_SCAN{
	link_t_ptr g;
	xrect_t xr;
}FIXTEXT_SCAN;

static int _fix_text_calc_rect(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	FIXTEXT_SCAN* ptt = (FIXTEXT_SCAN*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->xr.w = ptm->cur_x + ptm->cur_w - ptm->min_x + ptm->char_w;
		ptt->xr.h = ptm->cur_y + ptm->cur_h - ptm->min_y + ptm->line_h;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_text_rect_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr)
{
	FIXTEXT_SCAN tt = { 0 };
	if_measure_t it = { 0 };

	tt.g = g;
	xmem_copy((void*)(&tt.xr), (void*)pxr, sizeof(xrect_t));
	
	it.ctx = (void*)g;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	if (sz_len < 0)
		sz_len = xslen(sz_text);
	if (sz_text)
		sz_len++;

	scan_fix_text((tchar_t*)sz_text, sz_len, &it, pxf, pxa, tt.xr.x, tt.xr.y, tt.xr.w, tt.xr.h, 0, _fix_text_calc_rect, (void*)&tt);

}

void svg_text_rect(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr)
{
	xrect_t xr;
	link_t_ptr g;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = PAPER_MAX_HEIGHT;
	svg_rect_tm_to_pt(canv, &xr);

	g = svg_get_canvas_doc(canv);

	svg_text_rect_raw(g, pxf, pxa, sz_text, sz_len, &xr);

	pxr->fw = svg_pt_to_tm(canv, xr.w, 1) - svg_pt_to_tm(canv, 0, 1);
	pxr->fh = svg_pt_to_tm(canv, xr.h, 0) - svg_pt_to_tm(canv, 0, 0);
}

void _svg_draw_single_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	link_t_ptr nlk;

	nlk = insert_svg_node(g);

	write_text_to_svg_node(nlk, pxf, pxa, pxr, txt, len);
}

void _svg_draw_single_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	link_t_ptr g;
	xrect_t xr;

	g = svg_get_canvas_doc(canv);

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;

	svg_rect_tm_to_pt(canv, &xr);

	_svg_draw_single_text_raw(g, pxf, pxa, &xr, txt, len);
}

typedef struct _FIXTEXT_DRAW{
	link_t_ptr g;
	string_t vs;
}FIXTEXT_DRAW;

static int _fix_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	FIXTEXT_DRAW* ptt = (FIXTEXT_DRAW*)pp;
	xrect_t xr;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_END:
		xr.x = (ptm->min_x + ptm->char_w);
		xr.y = (ptm->cur_y);
		xr.w = (ptm->max_x - ptm->min_x);
		xr.h = (ptm->cur_h);

		_svg_draw_single_text_raw(ptt->g, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void _svg_draw_fix_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* text, int len)
{
	FIXTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.g = g;
	tt.vs = string_alloc();

	it.ctx = (void*)g;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	if (len < 0)
		len = xslen(text);
	if (text)
		len++;

	scan_fix_text((tchar_t*)text, len, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 0, _fix_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

void _svg_draw_fix_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* text, int len)
{
	xrect_t xr;
	link_t_ptr g;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = PAPER_MAX_HEIGHT;
	svg_rect_tm_to_pt(canv, &xr);

	g = svg_get_canvas_doc(canv);

	_svg_draw_fix_text_raw(g, pxf, pxa, &xr, text, len);
}

void svg_draw_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	xrect_t xr = { 0 };

	if (pxa && !is_null(pxa->text_wrap))
	{
		if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_FAR, -1, 0) == 0)
		{
			xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
			svg_text_rect_raw(g, pxf, pxa, txt, len, &xr);

			xr.y = pxr->y + pxr->h - xr.h;
		}
		else if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_CENTER, -1, 0) == 0)
		{
			xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
			svg_text_rect_raw(g, pxf, pxa, txt, len, &xr);

			xr.y = pxr->y + (pxr->h - xr.h) / 2;
		}
		else
		{
			xr.y = pxr->y;
		}

		xr.x = pxr->x;
		xr.w = pxr->w;

		_svg_draw_fix_text_raw(g, pxf, pxa, &xr, txt, len);
	}
	else
	{
		_svg_draw_single_text_raw(g, pxf, pxa, pxr, txt, len);
	}
}

void svg_draw_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, const tchar_t* txt, int len)
{
	xrect_t xr = { 0 };

	if (pxa && !is_null(pxa->text_wrap))
	{
		if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_FAR, -1, 0) == 0)
		{
			xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
			svg_text_rect(canv, pxf, pxa, txt, len, &xr);

			xr.fy = pxr->fy + pxr->fh - xr.fh;
		}
		else if (compare_text(pxa->line_align, -1, GDI_ATTR_TEXT_ALIGN_CENTER, -1, 0) == 0)
		{
			xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
			svg_text_rect(canv, pxf, pxa, txt, len, &xr);

			xr.fy = pxr->fy + (pxr->fh - xr.fh) / 2;
		}
		else
		{
			xr.fy = pxr->fy;
		}

		xr.fx = pxr->fx;
		xr.fw = pxr->fw;

		_svg_draw_fix_text(canv, pxf, pxa, &xr, txt, len);
	}
	else
	{
		_svg_draw_single_text(canv, pxf, pxa, pxr, txt, len);
	}
}

typedef struct _VARTEXT_DRAW{
	link_t_ptr g;
	int page;
	string_t vs;
}VARTEXT_DRAW;

static int _var_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	VARTEXT_DRAW* ptt = (VARTEXT_DRAW*)pp;
	xrect_t xr;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_END:
		xr.x = (ptm->min_x + ptm->char_w);
		xr.y = (ptm->cur_y);
		xr.w = (ptm->max_x - ptm->min_x);
		xr.h = (ptm->cur_h);

		_svg_draw_single_text_raw(ptt->g, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_draw_var_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var)
{
	VARTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.g = g;
	tt.page = 0;
	tt.vs = string_alloc();

	it.ctx = (void*)g;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	scan_var_text(var, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 0, _var_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

void svg_draw_var_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var)
{
	xrect_t xr;
	link_t_ptr g;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;
	svg_rect_tm_to_pt(canv, &xr);

	g = svg_get_canvas_doc(canv);

	svg_draw_var_text_raw(g, pxf, pxa, &xr, var);
}

typedef struct _TAGTEXT_DRAW{
	link_t_ptr g;
	int page;
	string_t vs;
}TAGTEXT_DRAW;

static int _tag_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TAGTEXT_DRAW* ptt = (TAGTEXT_DRAW*)pp;
	xrect_t xr;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_END:
		xr.x = ptm->min_x + ptm->char_w;
		xr.y = ptm->cur_y;
		xr.w = ptm->max_x - ptm->min_x;
		xr.h = ptm->cur_h;

		_svg_draw_single_text_raw(ptt->g, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_draw_tag_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	TAGTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.g = g;
	tt.page = page;
	tt.vs = string_alloc();

	it.ctx = (void*)g;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	scan_tag_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _tag_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

void svg_draw_tag_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	xrect_t xr;
	link_t_ptr g;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;
	svg_rect_tm_to_pt(canv, &xr);

	g = svg_get_canvas_doc(canv);

	svg_draw_tag_text_raw(g, pxf, pxa, &xr, data, page);
}

typedef struct _MEMOTEXT_DRAW{
	link_t_ptr g;
	int page;
	string_t vs;
}MEMOTEXT_DRAW;

static int _memo_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	MEMOTEXT_DRAW* ptt = (MEMOTEXT_DRAW*)pp;
	xrect_t xr;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		xr.x = (ptm->min_x + ptm->char_w);
		xr.y = (ptm->cur_y);
		xr.w = (ptm->max_x - ptm->min_x);
		xr.h = (ptm->cur_h);

		_svg_draw_single_text_raw(ptt->g, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}


void svg_draw_memo_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	MEMOTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.g = g;
	tt.page = page;
	tt.vs = string_alloc();

	it.ctx = (void*)g;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	scan_memo_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _memo_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

void svg_draw_memo_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	xrect_t xr;
	link_t_ptr g;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;
	svg_rect_tm_to_pt(canv, &xr);

	g = svg_get_canvas_doc(canv);

	svg_draw_memo_text_raw(g, pxf, pxa, &xr, data, page);
}

typedef struct _RICHTEXT_DRAW{
	link_t_ptr g;
	int page;
	string_t vs;
}RICHTEXT_DRAW;

static int _rich_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	RICHTEXT_DRAW* ptt = (RICHTEXT_DRAW*)pp;
	xrect_t xr;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		string_cat(ptt->vs, cur_char, -1);
		break;
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
	case _SCANNER_STATE_NEWLINE:
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		xr.x = (ptm->min_x + ptm->char_w);
		xr.y = (ptm->cur_y);
		xr.w = (ptm->max_x - ptm->min_x);
		xr.h = (ptm->cur_h);

		_svg_draw_single_text_raw(ptt->g, pxf, pxa, &xr, string_ptr(ptt->vs), string_len(ptt->vs));
		string_empty(ptt->vs);
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

void svg_draw_rich_text_raw(link_t_ptr g, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	RICHTEXT_DRAW tt = { 0 };
	if_measure_t it = { 0 };

	tt.g = g;
	tt.page = page;
	tt.vs = string_alloc();

	it.ctx = (void*)g;
	it.pf_mm_points = (PF_MM_POINTS)svg_pt_per_mm;
	it.pf_text_metric = (PF_TEXT_METRIC)svg_text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)svg_text_size_raw;

	scan_rich_text(data, &it, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _rich_text_calc_draw, (void*)&tt);

	string_free(tt.vs);
}

void svg_draw_rich_text(canvas_t canv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	xrect_t xr;
	link_t_ptr g;

	xr.fx = pxr->fx;
	xr.fy = pxr->fy;
	xr.fw = pxr->fw;
	xr.fh = pxr->fh;
	svg_rect_tm_to_pt(canv, &xr);

	g = svg_get_canvas_doc(canv);

	svg_draw_rich_text_raw(g, pxf, pxa, &xr, data, page);
}


#endif /*XDL_SUPPORT_SVG*/

