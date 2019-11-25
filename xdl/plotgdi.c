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

void plot_map(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, float rx, float ry, const matrix_t* pma)
{
	int i,j;
	int dark;
	xrect_t xr;
	xbrush_t xb_dot;

	for (i = 0; i < pma->rows; i++)
	{
		for (j = 0; j < pma->cols; j++)
		{
			xr.fx = prt->fx + j * rx;
			xr.fy = prt->fy + i * ry;
			xr.fw = rx;
			xr.fh = ry;

			ft_expand_rect(&xr, -0.5, -0.5);

			dark = (int)matrix_get_value(pma, i, j);
			//if (dark)
			{
				xmem_copy((void*)&xb_dot, (void*)pxb, sizeof(xbrush_t));
				lighten_xbrush(&xb_dot, dark + DEF_SOFT_LIGHTEN);
				draw_rect(canv, pxp, &xb_dot, &xr);
			}
		}
	}
}

void plot_dot(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, float rx, float ry, const matrix_t* pma)
{
	int i, j;
	int dark;
	xrect_t xr;
	xbrush_t xb_dot;

	for (i = 0; i < pma->rows; i++)
	{
		for (j = 0; j < pma->cols; j++)
		{
			xr.fx = prt->fx + j * rx;
			xr.fy = prt->fy + i * ry;
			xr.fw = rx;
			xr.fh = ry;

			ft_expand_rect(&xr, -0.5, -0.5);

			dark = (int)matrix_get_value(pma, i, j);
			//if (dark)
			{
				xmem_copy((void*)&xb_dot, (void*)pxb, sizeof(xbrush_t));
				lighten_xbrush(&xb_dot, dark + DEF_SOFT_LIGHTEN);
				draw_rect(canv, pxp, &xb_dot, &xr);
			}
		}
	}
}

void plot_line(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, double base, double span, const vector_t* pvc)
{
	int i, j;
	int dark;
	xpoint_t pt1, pt2;
	xrect_t xr;
	xbrush_t xb_dot;
	xpen_t xp_dot;
	float midy;

	if (!pvc->size)
		return;

	default_xbrush(&xb_dot);
	xscpy(xb_dot.color, pxp->color);
	lighten_xbrush(&xb_dot, DEF_SOFT_LIGHTEN);

	xmem_copy((void*)&xp_dot, (void*)pxp, sizeof(xpen_t));
	lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);
	xscpy(xp_dot.style, GDI_ATTR_STROKE_STYLE_DASHED);

	midy = prt->fy + prt->fh / 2;

	pt1.fx = prt->fx;
	pt1.fy = midy;
	pt2.fx = prt->fx + (pvc->size + 1) * 5;
	pt2.fy = midy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	for (i = 0; i < pvc->size; i++)
	{
		if (i)
		{
			pt2.fx = pt1.fx;
			pt2.fy = pt1.fy;
		}

		pt1.fx = prt->fx + (i + 1) * 5;
		pt1.fy = midy + (float)((pvc->data[i] - base) / span);

		xr.fx = pt1.fx - 1;
		xr.fy = pt1.fy - 1;
		xr.fw = 2;
		xr.fh = 2;

		draw_ellipse(canv, pxp, &xb_dot, &xr);

		if (i)
		{
			draw_line(canv, pxp, &pt1, &pt2);
		}
	}
}

void plot_bar(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, double base, double span, const vector_t* pvc)
{
	int i, j;
	int dark;
	xpoint_t pt1, pt2;
	xrect_t xr;
	xpen_t xp_dot;
	float midy;

	if (!pvc->size)
		return;

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, pxb->color);
	lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);

	midy = prt->fy + prt->fh / 2;

	pt1.fx = prt->fx;
	pt1.fy = midy;
	pt2.fx = prt->fx + (pvc->size + 1) * 5;
	pt2.fy = midy;

	draw_line(canv, &xp_dot, &pt1, &pt2);

	for (i = 0; i < pvc->size; i++)
	{
		xr.fx = prt->fx + i * 5;
		xr.fy = midy - (float)((pvc->data[i] - base) / span);
		xr.fw = 5;
		xr.fh = (float)((pvc->data[i] - base) / span);

		ft_expand_rect(&xr, -1, 0);
		draw_rect(canv, NULL, pxb, &xr);
	}
}

void plot_pie(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt,  const vector_t* pvc)
{
	int i, j;
	xrect_t xr;
	xpen_t xp_dot;
	float dby, midy;

	double pred, nxtd, total = 0.0;

	if (!pvc->size)
		return;

	default_xpen(&xp_dot);
	xscpy(xp_dot.color, pxb->color);
	lighten_xpen(&xp_dot, DEF_SOFT_DARKEN);

	midy = prt->fy + prt->fh / 2;

	for (i = 0; i < pvc->size; i++)
	{
		total += pvc->data[i];
	}

	nxtd = 0.0;

	for (i = 0; i < pvc->size; i++)
	{
		pred = nxtd;

		if ((int)total == 0)
			dby = 0;
		else
			dby = (float)(pvc->data[i] / total);

		nxtd = pred + (float)(2 * XPI *  dby);
		if (pred != nxtd)
		{
			draw_pie(canv, pxp, pxb, prt, pred, nxtd);
		}
	}
}

void plot_num(canvas_t canv, const xpen_t* pxp, const xbrush_t* pxb, const xfont_t* pxf, const xrect_t* prt, const tchar_t* layer, const tchar_t* num, int max)
{
	int j;
	int cols;
	xrect_t xr;

	cols = xslen(num);

	for (j = 0; j < max; j++)
	{
		if (compare_text(layer, -1, ATTR_LAYER_VERT, -1, 0) == 0)
		{
			xr.fx = prt->fx;
			xr.fy = prt->fy + j * prt->fw;
			xr.fw = prt->fw;
			xr.fh = prt->fw;
		}
		else
		{
			xr.fx = prt->fx + j * prt->fh;
			xr.fy = prt->fy;
			xr.fw = prt->fh;
			xr.fh = prt->fh;
		}

		ft_expand_rect(&xr, -0.5, -0.5);

		draw_rect(canv, pxp, NULL, &xr);
	}
}

#endif /*XDK_SUPPORT_CONTEXT*/

