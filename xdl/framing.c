/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc frame document

	@module	framing.c | implement file

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

#include "framing.h"
#include "xdlimp.h"
#include "xdlstd.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_VIEW)


void draw_progress(const if_canvas_t* pif, const xcolor_t* pxc, const xrect_t* prt, int steps)
{
	int i,index;
	float iw;
	xrect_t xr;
	xpen_t xp;
	xbrush_t xb;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	//iw = prt->fw / 10;
	iw = DEF_SMALL_ICON * 2;
	
	index = steps % 10;
	if (!index && steps)
		index = 10;

	for (i = 0; i < 10; i++)
	{
		xr.fx = prt->fx + iw * i;
		xr.fw = iw;
		xr.fy = prt->fy;
		xr.fh = prt->fh;

		ft_center_rect(&xr, DEF_SMALL_ICON, DEF_SMALL_ICON);

		if (index > i)
			(*pif->pf_draw_rect)(pif->canvas, &xp, &xb, &xr);
		else
			(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
	}
}

void draw_corner(const if_canvas_t* pif, const xcolor_t* pxc, const xrect_t* pxr)
{
	xrect_t xr;
	xpoint_t pt1, pt2;
	xpen_t xp = { 0 };
	float SPAN = DEF_CORNER_FEED;

	ft_expand_rect(&xr, 1.0f, 1.0f);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_SOLID);
	xscpy(xp.size, _T("2"));
	xp.adorn.feed = 2;
	xp.adorn.size = 2;

	pt1.fx = xr.fx;
	pt1.fy = xr.fy;
	pt2.fx = xr.fx;
	pt2.fy = xr.fy - SPAN;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	pt1.fx = xr.fx;
	pt1.fy = xr.fy;
	pt2.fx = xr.fx - SPAN;
	pt2.fy = xr.fy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//right,top
	pt1.fx = xr.fx + xr.fw;
	pt1.fy = xr.fy;
	pt2.fx = xr.fx + xr.fw;
	pt2.fy = xr.fy - SPAN;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	pt1.fx = xr.fx + xr.fw;
	pt1.fy = xr.fy;
	pt2.fx = xr.fx + xr.fw + SPAN;
	pt2.fy = xr.fy;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//left,bottom
	pt1.fx = xr.fx;
	pt1.fy = xr.fy + xr.fh;
	pt2.fx = xr.fx;
	pt2.fy = xr.fy + xr.fh + SPAN;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	pt1.fx = xr.fx;
	pt1.fy = xr.fy + xr.fh;
	pt2.fx = xr.fx - SPAN;
	pt2.fy = xr.fy + xr.fh;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	//right,bottom
	pt1.fx = xr.fx + xr.fw;
	pt1.fy = xr.fy + xr.fh;
	pt2.fx = xr.fx + xr.fw;
	pt2.fy = xr.fy + xr.fh + SPAN;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

	pt1.fx = xr.fx + xr.fw;
	pt1.fy = xr.fy + xr.fh;
	pt2.fx = xr.fx + xr.fw + SPAN;
	pt2.fy = xr.fy + xr.fh;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
}

void draw_ruler(const if_canvas_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xrect_t xr;
	xpoint_t pt1, pt2;
	int i, j;
	int vm, hm;
	xpen_t xp;
	xfont_t xf;
	xface_t xa;
	tchar_t sz[NUM_LEN + 1];

	float x1, x2, y1, y2;
	float fw, fh, fx, fy;
	float MAX_SPAN = 5.0f;
	float MIN_SPAN = 1.0f;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xfont(&xf);
	xscpy(xf.size, _T("7"));
	xscpy(xf.weight, _T("100"));
	xscpy(xf.color, xp.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, prt);

	fx = prt->fx;
	fy = prt->fy;
	fh = prt->fh;
	fw = prt->fw;

	//left edge
	if ((int)fh % 10)
		vm = (int)fh / 10;
	else
		vm = (int)fh / 10 - 1;

	x1 = fx;
	x2 = x1 - MAX_SPAN;
	y1 = fy;
	y2 = y1 + 2 * MAX_SPAN;
	for (i = 1; i < vm; i++)
	{
		y1 = y2;
		y2 = y1 + 2 * MAX_SPAN;

		pt1.fx = x1;
		pt1.fy = y1;
		pt2.fx = x2;
		pt2.fy = y1;
		(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

		for (j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				pt1.fx = x1;
				pt1.fy = y1 + j * MIN_SPAN;
				pt2.fx = x2 + 2 * MIN_SPAN;
				pt2.fy = y1 + j * MIN_SPAN;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}
		}

		xsprintf(sz, _T("%d"), i);

		xr.fx = x2;
		xr.fy = y1;
		xr.fw = MAX_SPAN;
		xr.fh = (float)(MAX_SPAN / 2 + 0.5);
		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz, -1);

		if (i == vm - 1)
		{
			pt1.fx = x1;
			pt1.fy = y2;
			pt2.fx = x2;
			pt2.fy = y2;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

			xsprintf(sz, _T("%d"), i + 1);

			xr.fx = x2;
			xr.fy = y2;
			xr.fw = MAX_SPAN;
			xr.fh = (float)(MAX_SPAN / 2 + 0.5);
			xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz, -1);
		}
	}

	//right edge
	if ((int)fh % 10)
		vm = (int)fh / 10;
	else
		vm = (int)fh / 10 - 1;

	x1 = fx + fw;
	x2 = x1 + MAX_SPAN;
	y1 = fy;
	y2 = y1 + 2 * MAX_SPAN;
	for (i = 1; i < vm; i++)
	{
		y1 = y2;
		y2 = y1 + 2 * MAX_SPAN;

		pt1.fx = x1;
		pt1.fy = y1;
		pt2.fx = x2;
		pt2.fy = y1;
		(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

		for (j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				pt1.fx = x1;
				pt1.fy = y1 + j * MIN_SPAN;
				pt2.fx = x2 - 2 * MIN_SPAN;
				pt2.fy = y1 + j * MIN_SPAN;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}
		}

		xsprintf(sz, _T("%d"), i);

		xr.fx = x1;
		xr.fy = y1;
		xr.fw = MAX_SPAN;
		xr.fh = (float)(MAX_SPAN / 2 + 0.5);
		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);
		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz, -1);

		if (i == vm - 1)
		{
			pt1.fx = x1;
			pt1.fy = y2;
			pt2.fx = x2;
			pt2.fy = y2;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

			xsprintf(sz, _T("%d"), i + 1);

			xr.fx = x1;
			xr.fy = y2;
			xr.fw = MAX_SPAN;
			xr.fh = (float)(MAX_SPAN / 2 + 0.5);
			xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);
			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz, -1);
		}
	}

	//top edge
	if ((int)fw % 10)
		hm = (int)fw / 10;
	else
		hm = (int)fw / 10 - 1;

	x1 = fx;
	x2 = x1 + 2 * MAX_SPAN;
	y1 = fy;
	y2 = y1 - MAX_SPAN;
	for (i = 1; i < hm; i++)
	{
		x1 = x2;
		x2 = x1 + 2 * MAX_SPAN;

		pt1.fx = x1;
		pt1.fy = y1;
		pt2.fx = x1;
		pt2.fy = y2;
		(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

		for (j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				pt1.fx = x1 + j * MIN_SPAN;
				pt1.fy = y1;
				pt2.fx = x1 + j * MIN_SPAN;
				pt2.fy = y2 + 2 * MIN_SPAN;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}
		}

		xsprintf(sz, _T("%d"), i);
		xr.fx = x1;
		xr.fy = y2;
		xr.fw = (float)(MAX_SPAN - 0.5);
		xr.fh = (float)(MAX_SPAN / 2 + 0.5);
		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz, -1);

		if (i == hm - 1)
		{
			pt1.fx = x2;
			pt1.fy = y1;
			pt2.fx = x2;
			pt2.fy = y2;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

			xsprintf(sz, _T("%d"), i + 1);

			xr.fx = x2;
			xr.fy = y2;
			xr.fw = (float)(MAX_SPAN - 0.5);
			xr.fh = (float)(MAX_SPAN / 2 + 0.5);
			xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz, -1);
		}
	}

	//bottom edge
	if ((int)fw % 10)
		hm = (int)fw / 10;
	else
		hm = (int)fw / 10 - 1;

	x1 = fx;
	x2 = x1 + 2 * MAX_SPAN;
	y1 = fy + fh;
	y2 = y1 + MAX_SPAN;
	for (i = 1; i < hm; i++)
	{
		x1 = x2;
		x2 = x1 + 2 * MAX_SPAN;

		pt1.fx = x1;
		pt1.fy = y1;
		pt2.fx = x1;
		pt2.fy = y2;
		(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

		for (j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				pt1.fx = x1 + j * MIN_SPAN;
				pt1.fy = y1;
				pt2.fx = x1 + j * MIN_SPAN;
				pt2.fy = y2 - 2 * MIN_SPAN;
				(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
			}
		}

		xsprintf(sz, _T("%d"), i);
		xr.fx = x1;
		xr.fy = (float)(y1 + MAX_SPAN / 2 - 0.5);
		xr.fw = MAX_SPAN;
		xr.fh = (float)(MAX_SPAN / 2 + 0.5);
		xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz, -1);

		if (i == hm - 1)
		{
			pt1.fx = x2;
			pt1.fy = y1;
			pt2.fx = x2;
			pt2.fy = y2;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

			xsprintf(sz, _T("%d"), i + 1);

			xr.fx = x2;
			xr.fy = (float)(y1 + MAX_SPAN / 2 - 0.5);
			xr.fw = MAX_SPAN;
			xr.fh = (float)(MAX_SPAN / 2 + 0.5);
			xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_NEAR);
			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz, -1);
		}
	}
}

#endif /*XDL_SUPPORT_VIEW*/
