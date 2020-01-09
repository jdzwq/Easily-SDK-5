/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc plot gdi document

	@module	plotgdi.c | implement file

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

#include "plotgdi.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDK_SUPPORT_CONTEXT)

void plot_geogram(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, const tchar_t* title, const plot_t* plt, const vector_t* pvc)
{
	
}

void plot_trendgram(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, const tchar_t* title, const plot_t* plt, const vector_t* pvc)
{
	int i, j;
	int dark;
	xpoint_t pt1, pt2;
	xrect_t xr;
	xbrush_t xb_line;
	xpen_t xp_dot, xp_line;
	xfont_t xf_dot;
	xface_t xa;
	float midy, midx;
	float dx, dy;
	tchar_t token[NUM_LEN];
	double dbl, vx, vy;

	default_xface(&xa);

	default_xbrush(&xb_line);
	format_xcolor(&(plt->clr_argv[0]), xb_line.color);

	default_xpen(&xp_line);
	format_xcolor(&(plt->clr_argv[0]), xp_line.color);

	default_xfont(&xf_dot);
	xscpy(xf_dot.color, pxf->color);
	xscpy(xf_dot.size, _T("8"));
	
	default_xpen(&xp_dot);
	xscpy(xp_dot.color, pxp->color);

	dy = prt->fh / 10;
	if (dy > 10)
		dy = 10;
	if (dy < 5)
		dy = 5;

	dx = prt->fw / 10;
	if (dx > 10)
		dx = 10;
	if (dx < 5)
		dx = 5;

	midy = prt->fy + prt->fh / 2;
	midx = prt->fx + dx;

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	xr.fx = prt->fx + dx;
	xr.fw = prt->fw - 2 * dx;
	xr.fy = prt->fy;
	xr.fh = dy;
	draw_text(canv, pxf, &xa, &xr, title, -1);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);

	dbl = plt->y_base;
	pt1.fx = midx;
	pt1.fy = midy;
	while (pt1.fy > prt->fy + 2 * dy)
	{
		pt2.fx = pt1.fx - 2;
		pt2.fy = pt1.fy;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx;
		xr.fw = dx - 2;
		xr.fy = pt1.fy - dy / 2;
		xr.fh = dy;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl += plt->y_step;
		pt1.fy -= dy;
	}

	dbl = plt->y_base - plt->y_step;
	pt1.fx = midx;
	pt1.fy = midy + dy;
	while (pt1.fy < prt->fy + prt->fh - 2 * dy)
	{
		pt2.fx = pt1.fx - 2;
		pt2.fy = pt1.fy;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx;
		xr.fw = dx - 2;
		xr.fy = pt1.fy - dy / 2;
		xr.fh = dy;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl -= plt->y_step;
		pt1.fy += dy;
	}

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	xr.fx = prt->fx;
	xr.fw = dx;
	xr.fy = prt->fy + dy / 2;
	xr.fh = dy;
	draw_text(canv, &xf_dot, &xa, &xr, plt->y_unit, -1);

	pt1.fx = midx;
	pt1.fy = prt->fy + dy;
	pt2.fx = prt->fx + dx;
	pt2.fy = prt->fy + prt->fh - dy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	dbl = plt->x_step;
	pt1.fx = midx + dx;
	pt1.fy = prt->fy + prt->fh - dy;
	while (pt1.fx < prt->fx + prt->fw - 2 * dx)
	{
		pt2.fx = pt1.fx;
		pt2.fy = pt1.fy + 2;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx / 2;
		xr.fw = dx;
		xr.fy = pt1.fy + 2;
		xr.fh = dy - 2;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl += plt->x_step;
		pt1.fx += dx;
	}

	xr.fx = prt->fx + prt->fw - 2 * dx;
	xr.fw = 2 * dx;
	xr.fy = prt->fy + prt->fh - dy;
	xr.fh = dy;
	draw_text(canv, &xf_dot, &xa, &xr, plt->x_unit, -1);

	pt1.fx = midx;
	pt1.fy = prt->fy + prt->fh - dy;
	pt2.fx = prt->fx + prt->fw - dx;
	pt2.fy = prt->fy + prt->fh - dy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	pt1.fx = midx;
	pt1.fy = midy;
	pt2.fx = prt->fx + prt->fw - dx;
	pt2.fy = midy;

	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);
	draw_line(canv, &xp_dot, &pt1, &pt2);

	for (i = 0; i < pvc->size; i ++)
	{
		if (i)
		{
			pt2.fx = pt1.fx;
			pt2.fy = pt1.fy;
		}

		vector_get_value(pvc, i, &vx, &vy);

		pt1.fx = midx + (float)(vx / plt->x_step) * dx;
		pt1.fy = midy - (float)((vy - plt->y_base) / plt->y_step) * dy;

		xr.fx = pt1.fx - 1;
		xr.fy = pt1.fy - 1;
		xr.fw = 2;
		xr.fh = 2;

		draw_ellipse(canv, &xp_line, &xb_line, &xr);

		if (i)
		{
			draw_line(canv, &xp_line, &pt1, &pt2);
		}
	}
}

void plot_histogram(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, const tchar_t* title, const plot_t* plt, const vector_t* pvc)
{
	int i, j;
	int dark;
	xpoint_t pt1, pt2;
	xrect_t xr;
	xbrush_t xb_line;
	xpen_t xp_dot, xp_line;
	xfont_t xf_dot;
	xface_t xa;
	float midy, midx;
	float dx, dy;
	tchar_t token[NUM_LEN];
	double dbl, vx, vy;

	default_xface(&xa);
	default_xbrush(&xb_line);
	default_xpen(&xp_line);

	default_xfont(&xf_dot);
	xscpy(xf_dot.color, pxf->color);
	xscpy(xf_dot.size, _T("8"));

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, pxp->color);

	dy = prt->fh / 10;
	if (dy > 10)
		dy = 10;
	if (dy < 5)
		dy = 5;

	dx = prt->fw / 10;
	if (dx > 10)
		dx = 10;
	if (dx < 5)
		dx = 5;

	midy = prt->fy + prt->fh / 2;
	midx = prt->fx + dx;

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	xr.fx = prt->fx + dx;
	xr.fw = prt->fw - 2 * dx;
	xr.fy = prt->fy;
	xr.fh = dy;
	draw_text(canv, pxf, &xa, &xr, title, -1);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);

	dbl = plt->y_base;
	pt1.fx = midx;
	pt1.fy = midy;
	while (pt1.fy > prt->fy + 2 * dy)
	{
		pt2.fx = pt1.fx - 2;
		pt2.fy = pt1.fy;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx;
		xr.fw = dx - 2;
		xr.fy = pt1.fy - dy / 2;
		xr.fh = dy;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl += plt->y_step;
		pt1.fy -= dy;
	}

	dbl = plt->y_base - plt->y_step;
	pt1.fx = midx;
	pt1.fy = midy + dy;
	while (pt1.fy < prt->fy + prt->fh - 2 * dy)
	{
		pt2.fx = pt1.fx - 2;
		pt2.fy = pt1.fy;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx;
		xr.fw = dx - 2;
		xr.fy = pt1.fy - dy / 2;
		xr.fh = dy;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl -= plt->y_step;
		pt1.fy += dy;
	}

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	xr.fx = prt->fx;
	xr.fw = dx;
	xr.fy = prt->fy + dy / 2;
	xr.fh = dy;
	draw_text(canv, &xf_dot, &xa, &xr, plt->y_unit, -1);

	pt1.fx = midx;
	pt1.fy = prt->fy + dy;
	pt2.fx = prt->fx + dx;
	pt2.fy = prt->fy + prt->fh - dy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	dbl = plt->x_step;
	pt1.fx = midx + dx;
	pt1.fy = prt->fy + prt->fh - dy;
	while (pt1.fx < prt->fx + prt->fw - 2 * dx)
	{
		pt2.fx = pt1.fx;
		pt2.fy = pt1.fy + 2;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx / 2;
		xr.fw = dx;
		xr.fy = pt1.fy + 2;
		xr.fh = dy - 2;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl += plt->x_step;
		pt1.fx += dx;
	}

	xr.fx = prt->fx + prt->fw - 2 * dx;
	xr.fw = 2 * dx;
	xr.fy = prt->fy + prt->fh - dy;
	xr.fh = dy;
	draw_text(canv, &xf_dot, &xa, &xr, plt->x_unit, -1);

	pt1.fx = midx;
	pt1.fy = prt->fy + prt->fh - dy;
	pt2.fx = prt->fx + prt->fw - dx;
	pt2.fy = prt->fy + prt->fh - dy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	pt1.fx = midx;
	pt1.fy = midy;
	pt2.fx = prt->fx + prt->fw - dx;
	pt2.fy = midy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	for (i = 0; i < pvc->size; i ++)
	{
		if (i)
		{
			pt2.fx = pt1.fx;
			pt2.fy = pt1.fy;
		}

		vector_get_value(pvc, i, &vx, &vy);

		pt1.fx = midx + (float)(vx / plt->x_step) * dx;
		pt1.fy = midy - (float)((vy - plt->y_base) / plt->y_step) * dy;

		if (pt1.fy < midy)
		{
			xr.fy = pt1.fy;
			xr.fh = midy - pt1.fy;

			format_xcolor(&(plt->clr_argv[0]), xp_line.color);
			format_xcolor(&(plt->clr_argv[0]), xb_line.color);
		}
		else
		{
			xr.fy = midy;
			xr.fh = pt1.fy - midy;

			format_xcolor(&(plt->clr_argv[1]), xp_line.color);
			format_xcolor(&(plt->clr_argv[1]), xb_line.color);
		}
		xr.fx = pt1.fx;
		xr.fw = dx / 2;

		draw_rect(canv, &xp_line, &xb_line, &xr);

		if (i)
		{
			if (pt1.fy > midy)
			{
				pt1.fx += dx / 2;
			}
			if (pt2.fy > midy)
			{
				pt2.fx += dx / 2;
			}

			format_xcolor(&(plt->clr_argv[2]), xp_line.color);
			//draw_line(canv, &xp_line, &pt1, &pt2);

			if (pt1.fy > midy)
			{
				pt1.fx -= dx / 2;
			}
			if (pt2.fy > midy)
			{
				pt2.fx -= dx / 2;
			}
		}
	}
}

void plot_pantogram(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, const tchar_t* title, const plot_t* plt, const vector_t* pvc)
{
	int i, j,k;
	int dark;
	xpoint_t pt;
	xrect_t xr;
	xbrush_t xb_line;
	xpen_t xp_dot, xp_line;
	xfont_t xf_dot;
	xface_t xa;
	float midy, midx;
	float dx, dy;
	tchar_t token[NUM_LEN];
	double dbl, total = 0.0;
	double pred, nxtd;
	double vx, vy;
	float dby;

	default_xface(&xa);
	default_xbrush(&xb_line);
	xscpy(xb_line.color, pxp->color);

	default_xfont(&xf_dot);
	xscpy(xf_dot.color, pxf->color);
	xscpy(xf_dot.size, _T("8"));

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, pxp->color);
	xscpy(xp_dot.size, _T("1"));
	lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);

	dy = prt->fh / 10;
	if (dy > 10)
		dy = 10;
	if (dy < 5)
		dy = 5;

	dx = prt->fw / 10;
	if (dx > 10)
		dx = 10;
	if (dx < 5)
		dx = 5;

	midy = prt->fy + prt->fh / 2;
	midx = prt->fx + dx;

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	xr.fx = prt->fx + dx;
	xr.fw = prt->fw - 2 * dx;
	xr.fy = prt->fy;
	xr.fh = dy;
	draw_text(canv, pxf, &xa, &xr, title, -1);

	pt.fx = prt->fx + prt->fw / 2;
	pt.fy = prt->fy + prt->fh / 2;

	for (i = 0; i < pvc->order * pvc->size; i += pvc->order)
	{
		total += pvc->data[i];
	}

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);

	nxtd = 0.0;

	for (i = 0; i < pvc->size; i++)
	{
		k = i % 8;
		format_xcolor(&(plt->clr_argv[k]), xp_line.color);
		format_xcolor(&(plt->clr_argv[k]), xb_line.color);

		pred = nxtd;

		vector_get_value(pvc, i, &vx, &vy);

		if ((int)total == 0)
			dby = 0;
		else
			dby = (float)(vx / total);

		nxtd = pred + (float)(2 * XPI *  dby);
		if (pred != nxtd)
		{
			draw_pie(canv, &xp_line, &xb_line, &pt, prt->fw / 4, prt->fh / 4, pred, nxtd);
		}

		xr.fx = midx;
		xr.fy = prt->fy + dy + i * 4;
		xr.fw = 3;
		xr.fh = 3;
		ft_center_rect(&xr, 3, 3);
		draw_rect(canv, &xp_line, &xb_line, &xr);

		xsprintf(token, _T("%.2f%c"), dby * 100, _T('%'));
		xr.fx = midx + 2;
		xr.fy = prt->fy + dy + i * 4;
		xr.fw = 10;
		xr.fh = 4;
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);
	}
}

void plot_scattergram(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, const tchar_t* title, const plot_t* plt, const vector_t* pvc)
{
	int i, j;
	int dark;
	xpoint_t pt1, pt2;
	xrect_t xr;
	xbrush_t xb_line;
	xpen_t xp_dot, xp_line;
	xfont_t xf_dot;
	xface_t xa;
	float midy, midx;
	float dx, dy;
	tchar_t token[NUM_LEN];
	double dbl, vx, vy;

	default_xface(&xa);

	default_xbrush(&xb_line);
	format_xcolor(&(plt->clr_argv[0]), xb_line.color);

	default_xpen(&xp_line);
	format_xcolor(&(plt->clr_argv[0]), xp_line.color);

	default_xfont(&xf_dot);
	xscpy(xf_dot.color, pxf->color);
	xscpy(xf_dot.size, _T("8"));

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, pxp->color);

	dy = prt->fh / 10;
	if (dy > 10)
		dy = 10;
	if (dy < 5)
		dy = 5;

	dx = prt->fw / 10;
	if (dx > 10)
		dx = 10;
	if (dx < 5)
		dx = 5;

	midy = prt->fy + prt->fh / 2;
	midx = prt->fx + dx;

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	xr.fx = prt->fx + dx;
	xr.fw = prt->fw - 2 * dx;
	xr.fy = prt->fy;
	xr.fh = dy;
	draw_text(canv, pxf, &xa, &xr, title, -1);

	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	pt1.fx = midx;
	pt1.fy = midy;
	pt2.fx = prt->fx + prt->fw - dx;
	pt2.fy = midy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_SOLID);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);

	dbl = plt->y_base;
	pt1.fx = midx;
	pt1.fy = midy;
	while (pt1.fy > prt->fy + 2 * dy)
	{
		pt2.fx = pt1.fx - 2;
		pt2.fy = pt1.fy;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx;
		xr.fw = dx - 2;
		xr.fy = pt1.fy - dy / 2;
		xr.fh = dy;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl += plt->y_step;
		pt1.fy -= dy;
	}

	dbl = plt->y_base - plt->y_step;
	pt1.fx = midx;
	pt1.fy = midy + dy;
	while (pt1.fy < prt->fy + prt->fh - 2 * dy)
	{
		pt2.fx = pt1.fx - 2;
		pt2.fy = pt1.fy;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx;
		xr.fw = dx - 2;
		xr.fy = pt1.fy - dy / 2;
		xr.fh = dy;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl -= plt->y_step;
		pt1.fy += dy;
	}

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	xr.fx = prt->fx;
	xr.fw = dx;
	xr.fy = prt->fy + dy / 2;
	xr.fh = dy;
	draw_text(canv, &xf_dot, &xa, &xr, plt->y_unit, -1);

	pt1.fx = prt->fx + dx;
	pt1.fy = prt->fy + dy;
	pt2.fx = prt->fx + dx;
	pt2.fy = prt->fy + prt->fh - dy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	pt1.fx = prt->fx + dx;
	pt1.fy = prt->fy + dy;
	pt2.fx = prt->fx + prt->fw - dx;
	pt2.fy = prt->fy + dy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	dbl = plt->x_step;
	pt1.fx = midx + dx;
	pt1.fy = prt->fy + prt->fh - dy;
	while (pt1.fx < prt->fx + prt->fw - 2 * dx)
	{
		pt2.fx = pt1.fx;
		pt2.fy = pt1.fy + 2;
		draw_line(canv, &xp_dot, &pt1, &pt2);

		xr.fx = pt1.fx - dx / 2;
		xr.fw = dx;
		xr.fy = pt1.fy + 2;
		xr.fh = dy - 2;
		xsprintf(token, _T("%.2f"), dbl);
		draw_text(canv, &xf_dot, &xa, &xr, token, -1);

		dbl += plt->x_step;
		pt1.fx += dx;
	}

	xr.fx = prt->fx + prt->fw - 2 * dx;
	xr.fw = 2 * dx;
	xr.fy = prt->fy + prt->fh - dy;
	xr.fh = dy;
	draw_text(canv, &xf_dot, &xa, &xr, plt->x_unit, -1);

	pt1.fx = midx;
	pt1.fy = prt->fy + prt->fh - dy;
	pt2.fx = prt->fx + prt->fw - dx;
	pt2.fy = prt->fy + prt->fh - dy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	pt1.fx = prt->fx + prt->fw - dx;
	pt1.fy = prt->fy + dy;
	pt2.fx = prt->fx + prt->fw - dx;
	pt2.fy = prt->fy + prt->fh - dy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	for (i = 0; i < pvc->size; i ++)
	{
		vector_get_value(pvc, i, &vx, &vy);

		pt1.fx = midx + (float)((vx) / plt->x_step) * dx;
		pt1.fy = midy - (float)((vy - plt->y_base) / plt->y_step) * dy;

		xr.fx = pt1.fx - 0.5;
		xr.fy = pt1.fy - 0.5;
		xr.fw = 1;
		xr.fh = 1;

		draw_ellipse(canv, &xp_line, &xb_line, &xr);
	}
}

#endif /*XDK_SUPPORT_CONTEXT*/

