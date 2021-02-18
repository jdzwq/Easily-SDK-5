/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc jion document

	@module	jioning.c | implement file

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

#include "jioning.h"
#include "miscella.h"

#if defined(XDL_SUPPORT_GDI)

static void _draw_round(const drawing_interface* pif, const xpen_t* pxp, const xpoint_t* pps, const xpoint_t* ppt, double angle)
{
	xbrush_t xb;
	xrect_t xr;
	float rr;

	default_xbrush(&xb);
	xscpy(xb.color, pxp->color);

	rr = (float)(xstof(pxp->size) * 0.8);
	
	xr.fx = ppt->fx - rr;
	xr.fy = ppt->fy - rr;
	xr.fw = 2 * rr;
	xr.fh = 2 * rr;

	(*pif->pf_draw_ellipse)(pif->ctx, pxp, &xb, &xr);
}

static void _draw_arrow(const drawing_interface* pif, const xpen_t* pxp, const xpoint_t* pps, const xpoint_t* ppt, double angle)
{
	xbrush_t xb;

	xsize_t xs;
	xpoint_t pt, pk, tk1, tk2;
	xpoint_t pa[10] = { 0 };
	tchar_t sa[10] = { 0 };

	float rr, rm;
	double arc, fang, tang;

	default_xbrush(&xb);
	xscpy(xb.color, pxp->color);

	rr = (float)(xstof(pxp->size) * 1.6);
	rm = (float)(rr * sin(XPI / 4));

	arc = atan2((float)(ppt->fy - pps->fy), (float)(ppt->fx - pps->fx));
	arc += XPI;

	fang = arc - angle / 2;
	tang = arc + angle / 2;

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;
	xs.fw = rr;
	xs.fh = rr;
	(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, tang, pa, 2);
	tk1.fx = pa[0].fx;
	tk1.fy = pa[0].fy;
	tk2.fx = pa[1].fx;
	tk2.fy = pa[1].fy;

	fang = arc;
	tang = arc;

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;
	xs.fw = rm;
	xs.fh = rm;
	(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, tang, pa, 2);
	pk.fx = pa[0].fx;
	pk.fy = pa[0].fy;

	sa[0] = _T('M');
	pa[0].fx = pt.fx;
	pa[0].fy = pt.fy;
	
	sa[1] = _T('L');
	pa[1].fx = tk1.fx;
	pa[1].fy = tk1.fy;

	sa[2] = _T('L');
	pa[2].fx = pk.fx;
	pa[2].fy = pk.fy;

	sa[3] = _T('L');
	pa[3].fx = tk2.fx;
	pa[3].fy = tk2.fy;

	sa[4] = _T('L');
	pa[4].fx = pt.fx;
	pa[4].fy = pt.fy;

	sa[5] = _T('Z');

	(*pif->pf_draw_path)(pif->ctx, pxp, &xb, sa, pa, 5);
}

static void _draw_square(const drawing_interface* pif, const xpen_t* pxp, const xpoint_t* pps, const xpoint_t* ppt, double angle)
{
	xbrush_t xb;

	xsize_t xs;
	xpoint_t pt, pk, tk1, tk2, tk3, tk4;
	xpoint_t pa[10] = { 0 };
	tchar_t sa[10] = { 0 };

	float rr;
	double arc, fang, tang;

	default_xbrush(&xb);
	xscpy(xb.color, pxp->color);

	rr = (float)(xstof(pxp->size) * 0.8);

	arc = atan2((float)(ppt->fy - pps->fy), (float)(ppt->fx - pps->fx));
	arc += XPI;

	fang = arc - XPI / 2;
	tang = arc + XPI / 2;

	pt.fx = ppt->fx;
	pt.fy = ppt->fy;
	xs.fw = rr;
	xs.fh = rr;
	(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, tang, pa, 2);
	tk1.fx = pa[0].fx;
	tk1.fy = pa[0].fy;
	tk2.fx = pa[1].fx;
	tk2.fy = pa[1].fy;

	fang = arc;
	tang = arc;

	xs.fw = 2 * rr;
	xs.fh = 2 * rr;
	(*pif->pf_calc_fan)(pif->ctx, &pt, &xs, fang, tang, pa, 2);
	pk.fx = pa[0].fx;
	pk.fy = pa[0].fy;

	fang = arc - XPI / 2;
	tang = arc + XPI / 2;

	xs.fw = rr;
	xs.fh = rr;
	(*pif->pf_calc_fan)(pif->ctx, &pk, &xs, fang, tang, pa, 2);
	tk3.fx = pa[0].fx;
	tk3.fy = pa[0].fy;
	tk4.fx = pa[1].fx;
	tk4.fy = pa[1].fy;

	sa[0] = _T('M');
	pa[0].fx = tk1.fx;
	pa[0].fy = tk1.fy;

	sa[1] = _T('L');
	pa[1].fx = tk3.fx;
	pa[1].fy = tk3.fy;

	sa[2] = _T('L');
	pa[2].fx = tk4.fx;
	pa[2].fy = tk4.fy;

	sa[3] = _T('L');
	pa[3].fx = tk2.fx;
	pa[3].fy = tk2.fy;

	sa[4] = _T('Z');

	(*pif->pf_draw_path)(pif->ctx, pxp, &xb, sa, pa, 4);
}

void draw_linecap(const drawing_interface* pif, const xpen_t* pxp, const xpoint_t* pps, const xpoint_t* ppt, double angle, const tchar_t* cap)
{
	if (compare_text(cap, -1, GDI_ATTR_STROKE_LINECAP_ARROW, -1, 1) == 0)
	{
		_draw_arrow(pif, pxp, pps, ppt, angle);
	}
	else if (compare_text(cap, -1, GDI_ATTR_STROKE_LINECAP_SQUARE, -1, 1) == 0)
	{
		_draw_square(pif, pxp, pps, ppt, angle);
	}
	else if (compare_text(cap, -1, GDI_ATTR_STROKE_LINECAP_ROUND, -1, 1) == 0)
	{
		_draw_round(pif, pxp, pps, ppt, angle);
	}
}

#endif /*XDL_SUPPORT_GDI*/
