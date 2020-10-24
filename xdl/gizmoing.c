/***********************************************************************
	Easily xdl v5.5

	(c) 2013-20SMALL_GIZMO JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gizmo document

	@module	gizmoing.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 20SMALL_GIZMO.01 - 20SMALL_GIZMO.12	v4.5
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

#include "gizmoing.h"

#include "xdlimp.h"
#include "xdlstd.h"

#define SMALL_GIZMO		12 //(float)(12.0f / PDPERMM)
#define MIDD_GIZMO		24 //(float)(24.0f / PDPERMM)
#define LARGE_GIZMO		36 //(float)(32.0f / PDPERMM)

#define SINGLE_SPAN		1 //(float)(1.0f / PDPERMM)
#define DOUBLE_SPAN		2 //(float)(2.0f / PDPERMM)
#define THREE_SPAN		3 //(float)(3.0f / PDPERMM)
#define FOUR_SPAN		4 //(float)(4.0f / PDPERMM)

#if defined(XDL_SUPPORT_VIEW)

void draw_logo_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2 - DOUBLE_SPAN;
	xr.h = prt->h / 2 - DOUBLE_SPAN;
	(*pif->pf_draw_round)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 + SINGLE_SPAN;
	xr.y = prt->y;
	xr.w = prt->w / 2 - DOUBLE_SPAN;
	xr.h = prt->h / 2 - DOUBLE_SPAN;
	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 + SINGLE_SPAN;
	xr.w = prt->w / 2 - DOUBLE_SPAN;
	xr.h = prt->h / 2 - DOUBLE_SPAN;
	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 + SINGLE_SPAN;
	xr.y = prt->y + prt->h / 2 + SINGLE_SPAN;
	xr.w = prt->w / 2 - DOUBLE_SPAN;
	xr.h = prt->h / 2 - DOUBLE_SPAN;
	(*pif->pf_draw_round)(pif->ctx, &xp, &xb, &xr);
}

void draw_plus_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt;
	xpoint_t pt[2];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xp.adorn.feed = 1;
	xp.adorn.size = 1;

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &(pt[0]), &(pt[1]));

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &(pt[0]), &(pt[1]));
}

void draw_minus_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt;
	xpoint_t pt[2];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	xp.adorn.feed = 1;
	xp.adorn.size = 1;
	(*pif->pf_draw_line)(pif->ctx, &xp, &(pt[0]), &(pt[1]));
}

void draw_collapse_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_expand_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_begin_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 + ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_up_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 + ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_down_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3 - ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_end_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3 - ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_first_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 + ps;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_prev_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_next_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_last_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 * 3 - ps;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_zoomin_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 3 * 2 + SINGLE_SPAN;
	xr.h = prt->h / 3 * 2 + SINGLE_SPAN;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = prt->x + prt->w / 3 * 2;
	pt1.y = prt->y + prt->h / 3 * 2;
	pt2.x = prt->x + prt->w;
	pt2.y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_zoomout_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 3 * 2 + SINGLE_SPAN;
	xr.h = prt->h / 3 * 2 + SINGLE_SPAN;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + ps + SINGLE_SPAN;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h - ps - SINGLE_SPAN;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = prt->x + prt->w / 3 * 2;
	pt1.y = prt->y + prt->h / 3 * 2;
	pt2.x = prt->x + prt->w;
	pt2.y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_keybox_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	lighten_xpen(&xp, DEF_HARD_DARKEN);

	xr.x = prt->x;
	xr.y = prt->y + ps;
	xr.w = prt->w;
	xr.h = prt->h - 2 * ps;

	(*pif->pf_draw_round)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w - 2 * ps;
	xr.h = ps * 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_keyboxed_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	lighten_xpen(&xp, -20);

	xr.x = prt->x - SINGLE_SPAN;
	xr.y = prt->y + SINGLE_SPAN;
	xr.w = prt->w + DOUBLE_SPAN;
	xr.h = prt->h - DOUBLE_SPAN;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	pt1.x = xr.x;
	pt1.y = xr.y + DOUBLE_SPAN;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + DOUBLE_SPAN;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x;
	pt1.y = xr.y + FOUR_SPAN;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + FOUR_SPAN;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + THREE_SPAN;
	pt1.y = xr.y + FOUR_SPAN;
	pt2.x = xr.x + THREE_SPAN;
	pt2.y = xr.y + xr.h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w - THREE_SPAN;
	pt1.y = xr.y + FOUR_SPAN;
	pt2.x = xr.x + xr.w - THREE_SPAN;
	pt2.y = xr.y + xr.h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_touch_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	lighten_xpen(&xp, -20);

	xr.x = prt->x - SINGLE_SPAN;
	xr.y = prt->y - SINGLE_SPAN;
	xr.w = prt->w + DOUBLE_SPAN;
	xr.h = prt->h + DOUBLE_SPAN;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	pt1.x = xr.x + xr.w / 2 - THREE_SPAN;
	pt1.y = xr.y + xr.h / 2 - THREE_SPAN;
	pt2.x = xr.x + xr.w / 2 + THREE_SPAN;
	pt2.y = xr.y + xr.h / 2 + THREE_SPAN;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2 - THREE_SPAN;
	pt1.y = xr.y + xr.h / 2 + THREE_SPAN; 
	pt2.x = xr.x + xr.w / 2 + THREE_SPAN;
	pt2.y = xr.y + xr.h / 2 - THREE_SPAN;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_touched_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	lighten_xpen(&xp, -20);

	xr.x = prt->x - SINGLE_SPAN;
	xr.y = prt->y - SINGLE_SPAN;
	xr.w = prt->w + DOUBLE_SPAN;
	xr.h = prt->h + DOUBLE_SPAN;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	pt_expand_rect(&xr, -DOUBLE_SPAN, -DOUBLE_SPAN);

	lighten_xbrush(&xb, 20);

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);
}

void draw_close_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	pt1.x = xr.x + ps;
	pt1.y = xr.y + ps;
	pt2.x = xr.x + xr.w - ps;
	pt2.y = xr.y + xr.h - ps;

	xp.adorn.feed = 1;
	xp.adorn.size = 1;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps;
	pt1.y = xr.y + xr.h - ps;
	pt2.x = xr.x + xr.w - ps;
	pt2.y = xr.y + ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_minimize_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y + prt->h - DOUBLE_SPAN;
	xr.w = 4;
	xr.h = 2;
	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_maximize_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.w = prt->w - DOUBLE_SPAN;
	xr.y = prt->y;
	xr.h = prt->h - DOUBLE_SPAN;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + DOUBLE_SPAN;
	xr.w = prt->w - DOUBLE_SPAN;
	xr.y = prt->y + DOUBLE_SPAN;
	xr.h = prt->h - DOUBLE_SPAN;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_restore_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_sum_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5] = { 0 };
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	pt[0].x = xr.x + xr.w - ps;
	pt[0].y = xr.y + ps;
	pt[1].x = xr.x + ps;
	pt[1].y = xr.y + ps;
	pt[2].x = xr.x + xr.w / 2;
	pt[2].y = xr.y + xr.h / 2;
	pt[3].x = xr.x + ps;
	pt[3].y = xr.y + xr.h - ps;
	pt[4].x = xr.x + xr.w - 2 * ps;
	pt[4].y = xr.y + xr.h - ps;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 5);
}

void draw_checkbox_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_checked_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	lighten_xbrush(&xb, DEF_SOFT_LIGHTEN);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	//(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + DOUBLE_SPAN;
	pt2.y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + DOUBLE_SPAN;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h - SINGLE_SPAN;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h - SINGLE_SPAN;
	pt2.x = xr.x + xr.w + DOUBLE_SPAN;
	pt2.y = xr.y + SINGLE_SPAN;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_radiobox_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h - 2 * ps;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);
}

void draw_radioed_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h - 2 * ps;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);
}

void draw_selected_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + DOUBLE_SPAN;
	pt2.y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + DOUBLE_SPAN;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_folder_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	xscpy(xp.size, _T("1"));

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h / 4;

	pt[1].x = xr.x;
	pt[1].y = xr.y;

	pt[2].x = xr.x + xr.w / 2;
	pt[2].y = xr.y;

	pt[3].x = xr.x + 3 * xr.w / 4;
	pt[3].y = xr.y + xr.h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	xr.y += xr.h / 4;
	xr.h -= xr.h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_guider_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h / 4;

	pt[1].x = xr.x + xr.w / 2;
	pt[1].y = xr.y + xr.h / 4;

	pt[2].x = xr.x + xr.w / 4 * 3;
	pt[2].y = xr.y + xr.h / 2;

	pt[3].x = xr.x + xr.w / 2;
	pt[3].y = xr.y + xr.h / 4 * 3;

	pt[4].x = xr.x;
	pt[4].y = xr.y + xr.h / 4 * 3;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 5);
}

void draw_fixed_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	pt1.x = prt->x + 2 * ps;
	pt1.y = prt->y + prt->h / 2;
	pt2.x = prt->x + 4 * ps;
	pt2.y = prt->y + prt->h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = prt->x + 4 * ps;
	pt1.y = prt->y + prt->h / 4;
	pt2.x = prt->x + 4 * ps;
	pt2.y = prt->y + prt->h / 4 * 3;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
	
	xr.x = prt->x + 4 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w - 6 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_numeric_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	float fs;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, prt);

	font_metric_by_px((float)prt->h, &fs, NULL);

	if (fs < 8.0f)
		fs = 8.0f;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ftoxs(fs, xf.size, INT_LEN);
	xscpy(xf.color, GDI_ATTR_RGB_WHITE);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	//(*pif->pf_draw_text)(pif->ctx, &xf, &xa, prt, no, -1);
}

void draw_omit_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	float fs;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	font_metric_by_px((float)prt->h, &fs, NULL);

	if (fs < 8.0f)
		fs = 8.0f;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ftoxs(fs, xf.size, INT_LEN);
	xscpy(xf.color, GDI_ATTR_RGB_WHITE);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, prt, _T("..."), -1);
}

void draw_doc_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 4;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 4 * 3;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_new(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	pt[0].x = xr.x;
	pt[0].y = xr.y;
	pt[1].x = xr.x;
	pt[1].y = xr.y + xr.h;
	pt[2].x = xr.x + xr.w / 4 * 3;
	pt[2].y = xr.y + xr.h;
	pt[3].x = xr.x + xr.w;
	pt[3].y = xr.y + xr.h / 4 * 3;
	pt[4].x = xr.x + xr.w;
	pt[4].y = xr.y;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 5);

	pt[0].x = xr.x + xr.w / 4 * 3;
	pt[0].y = xr.y + xr.h - SINGLE_SPAN;
	pt[1].x = xr.x + xr.w / 4 * 3;
	pt[1].y = xr.y + xr.h / 4 * 3;
	pt[2].x = xr.x + xr.w - SINGLE_SPAN;
	pt[2].y = xr.y + xr.h / 4 * 3;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 4;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 4 * 3;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_open_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + ps;
	xr.w = prt->w - 3 * ps;
	xr.h = prt->h - ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h;
	pt[1].x = xr.x + 3 * ps;
	pt[1].y = xr.y + xr.h / 2;
	pt[2].x = xr.x + xr.w + 3 * ps;
	pt[2].y = xr.y + xr.h / 2;
	pt[3].x = xr.x + xr.w;
	pt[3].y = xr.y + xr.h;

	format_xcolor(pxc, xb.color);
	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 4);
}

void draw_save_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[8];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + ps;
	pt[4].x = prt->x + prt->w - ps;
	pt[4].y = prt->y;
	pt[5].x = prt->x + prt->w - ps;
	pt[5].y = prt->y + prt->h / 4 + DOUBLE_SPAN;
	pt[6].x = prt->x + prt->w / 4;
	pt[6].y = prt->y + prt->h / 4 + DOUBLE_SPAN;
	pt[7].x = prt->x + prt->w / 4;
	pt[7].y = prt->y;

	format_xcolor(pxc, xb.color);
	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 8);
}

void draw_saveas_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[9];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	lighten_xpen(&xp, DEF_HARD_DARKEN);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h / 2;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + ps;
	pt[5].x = prt->x + prt->w - ps;
	pt[5].y = prt->y;
	pt[6].x = prt->x + prt->w - ps;
	pt[6].y = prt->y + prt->h / 4 + DOUBLE_SPAN;
	pt[7].x = prt->x + prt->w / 4;
	pt[7].y = prt->y + prt->h / 4 + DOUBLE_SPAN;
	pt[8].x = prt->x + prt->w / 4;
	pt[8].y = prt->y;

	format_xcolor(pxc, xb.color);
	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 9);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w / 2 + FOUR_SPAN;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 2 + FOUR_SPAN;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_schema_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w / 2 - ps - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 4;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w / 2 + ps;
	pt[1].y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - ps - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 4 * 3;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + xr.w / 2 + ps;
	pt[0].y = xr.y + xr.h / 2 - ps;
	pt[1].x = xr.x + xr.w / 4 * 3;
	pt[1].y = xr.y + xr.h / 2;
	pt[2].x = xr.x + xr.w / 2 + ps;
	pt[2].y = xr.y + xr.h / 2 + ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_output_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;
	
	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_input_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[7];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x;
	pt[2].y = prt->y;
	pt[3].x = prt->x;
	pt[3].y = prt->y + prt->h;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + prt->h;
	pt[5].x = prt->x + prt->w;
	pt[5].y = prt->y + prt->h / 2;
	pt[6].x = prt->x + prt->w / 4 * 3;
	pt[6].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 7);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_print_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	lighten_xpen(&xp, DEF_HARD_DARKEN);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + prt->w / 4 + ps;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4;
	pt[3].x = prt->x + prt->w / 4 * 3 + ps;
	pt[3].y = prt->y;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 4);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w;
	xr.h = prt->h / 2 + ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - ps;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 4 * 3 - ps;
	pt[2].y = prt->y + prt->h - ps;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 4);
}

void draw_preview_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	lighten_xbrush(&xb, DEF_HARD_LIGHTEN);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_round)(pif->ctx, &xp, &xb, &xr);
}

void draw_screen_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	lighten_xpen(&xp, DEF_HARD_DARKEN);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	(*pif->pf_draw_round)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = ps;
	xr.h = ps;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	(*pif->pf_draw_round)(pif->ctx, &xp, &xb, &xr);

	lighten_xbrush(&xb, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_execute_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[4];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + prt->w / 2 - 2 * ps;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h / 4;
	pt[3].x = prt->x + prt->w / 2;
	pt[3].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 + 3 * ps;
	pt[2].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2 + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 * 3 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 * 3 - 3 * ps;
	pt[2].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_selectall_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[4];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 4;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	pt[0].x = prt->x + prt->w / 2 + ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h - ps - SINGLE_SPAN;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 * 3 - 2 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_delete_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + ps;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h - ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h - ps;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_insert_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - 1 * ps;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - DOUBLE_SPAN;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + DOUBLE_SPAN;
	pt[2].x = prt->x + prt->w - 2 * ps - FOUR_SPAN;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_remove_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w /2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 4 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 1 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_clear_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 4 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 1 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h - 1 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h - 4 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_append_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 2 + 3 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_copy_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 4 * 3;
	xr.h = prt->h / 4 * 3;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 2 - ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 + SINGLE_SPAN;
	pt[1].x = prt->x + prt->w - SINGLE_SPAN;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 3);
}

void draw_cut_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);
	xscpy(xp.size, _T("1"));
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_paste_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_undo_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4 * 3;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 - 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 + 2 * ps;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 4 + 3 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_redo_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4 * 3;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 + 3 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_fontname_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("8"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("7"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("A"), 1);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("A"), 1);
}

void draw_fontsize_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[3];
	int ps;

	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xf.size, _T("8"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xf.size, _T("7"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xf.size, _T("6"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xscpy(xp.size, _T("1"));
	ps = DOUBLE_SPAN;

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + 2 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + ps;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;;
	pt[1].y = prt->y + 2 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("A"), 1);
}

void draw_fontweight_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("9"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("8"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("B"), 1);
}

void draw_fontcolor_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xf.size, _T("8"));
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xf.size, _T("7"));
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xf.size, _T("6"));
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("A"), 1);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 4 * 3 + ps;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h / 4 - ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_fontstyle_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xf.size, _T("9"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xf.size, _T("8"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.style, GDI_ATTR_FONT_STYLE_UNDERLINE);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("U"), 1);
}

void draw_foreground_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4 * 3 - 2 * ps;
	xr.y = prt->y;
	xr.w = 4 * ps;
	xr.h = prt->h / 2 - ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 3);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_background_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);

	prt = &rt;

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y;
	xr.w = 4 * ps;
	xr.h = prt->h / 2 - ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 2 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + prt->w / 2 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 3);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h - 2 * ps;
	xr.w = prt->w - 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_alignnear_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_aligncenter_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_alignfar_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);

	pt[0].x = prt->x + prt->w / 2 + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2 - ps;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_arrangelept_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);

	xr.x = prt->x + ps + SINGLE_SPAN;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + ps + SINGLE_SPAN;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 2;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + ps + SINGLE_SPAN;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w / 4;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_arrangecenter_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 2;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 4 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + 2 * ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w - 4 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_arrangeright_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);

	xr.x = prt->x + prt->w - 6 * ps - SINGLE_SPAN;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 5 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w - 8 * ps - SINGLE_SPAN;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 7 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w - 4 * ps - SINGLE_SPAN;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = 3 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_sizehorz_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 2 + prt->h / 8;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);
}

void draw_sizevert_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 4;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 8;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 8;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);
}

void draw_spacehorz_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = 2 * ps;
	xr.h = prt->h / 4 * 3;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 2 * ps;
	xr.h = prt->h / 4 * 3;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w - 2 * ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 2 * ps;
	xr.h = prt->h / 4 * 3;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + 2 * ps;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 2 - ps;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w / 2 - ps;
	pt[3].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 + ps;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w - 2 * ps;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w - 2 * ps;
	pt[3].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);
}

void draw_spacevert_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h - 2 * ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + 2 * ps;
	pt[1].x = prt->x;
	pt[1].y = prt->y + 2 * ps;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h / 2 - ps;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h / 2 - ps;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h - 2 * ps;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h - 2 * ps;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);
}

void draw_edit_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y;
	xr.w = 4 * ps;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 2 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + 1 * ps + SINGLE_SPAN;
	pt[1].x = prt->x + prt->w / 2 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps + SINGLE_SPAN;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 3);
}

void draw_group_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 + 2 * ps;
	xr.y = prt->y + prt->h / 4 + 2 * ps;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	ltoxs(xstol(xp.size) - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_order_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4 - (int)(1.5 * (float)ps);
	xr.y = prt->y + prt->h / 4 - (int)(1.5 * (float)ps);
	xr.w = 3 * ps;
	xr.h = 3 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - (int)(1.5 * (float)ps);
	xr.y = prt->y + prt->h / 4 * 3 - (int)(1.5 * (float)ps);
	xr.w = 3 * ps;
	xr.h = 3 * ps;

	lighten_xpen(&xp, 0 - DEF_HARD_DARKEN);
	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_border_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;
	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_shape_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 2 - 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 2 - 2 * ps;

	(*pif->pf_draw_round)(pif->ctx, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + ps;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 2 - ps;
	pt[2].y = prt->y + prt->h - ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	xr.x = prt->x + prt->w / 2 + ps;
	xr.y = prt->y + prt->h / 2 + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 2 - 2 * ps;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);
}

void draw_label_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	xrect_t rt, xr;

	default_xfont(&xf);
	format_xcolor(pxc, xf.color);
	xscpy(xf.style, GDI_ATTR_FONT_STYLE_ITALIC);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xf.size, _T("9"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xf.size, _T("8"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("Aa"), -1);
}

void draw_check_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4 - ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w - ps;
	pt[3].y = prt->y + ps;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);
}

void draw_singletext_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 - 3 * ps;
	xr.w = prt->w;
	xr.h = 6 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 + 4 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_multitext_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 4;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 4 * 3;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_tag_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("12"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("10"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("</>"), -1);
}

void draw_memo_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 4;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + SINGLE_SPAN;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - SINGLE_SPAN;
	pt2.y = xr.y + xr.h / 4 * 3;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt1, &pt2);
}

void draw_photo_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + ps + SINGLE_SPAN;
	xr.y = prt->y + ps + SINGLE_SPAN;
	xr.w = prt->w / 4 - DOUBLE_SPAN;
	xr.h = prt->h / 4 - DOUBLE_SPAN;

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);
}

void draw_herf_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xscpy(xp.size, _T("1"));

	xr.x = prt->x + prt->w / 4 - ps - SINGLE_SPAN;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = (int)(2.5 * (float)ps) + SINGLE_SPAN;
	xr.h = (int)(2.5 * (float)ps) - SINGLE_SPAN;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps - SINGLE_SPAN;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = (int)(2.5 * (float)ps) + SINGLE_SPAN;
	xr.h = (int)(2.5 * (float)ps) - SINGLE_SPAN;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - ps - SINGLE_SPAN;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = (int)(2.5 * (float)ps) + SINGLE_SPAN;
	xr.h = (int)(2.5 * (float)ps) - SINGLE_SPAN;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);
}

void draw_code_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 3 - ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 3;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 3 - ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 3 * 2;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 3 - ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_table_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xscpy(xp.size, _T("1"));

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_rich_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h - 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 4;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w - ps - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + SINGLE_SPAN;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - SINGLE_SPAN;
	pt[1].y = xr.y + xr.h / 4 * 3;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + xr.w / 4;
	pt[0].y = xr.y + xr.h;
	pt[1].x = xr.x + xr.w / 4;
	pt[1].y = xr.y + xr.h + 2 * ps;
	pt[2].x = xr.x + xr.w + 2 * ps;
	pt[2].y = xr.y + xr.h + 2 * ps;
	pt[3].x = xr.x + xr.w + 2 * ps;
	pt[3].y = xr.y + xr.h / 4;
	pt[4].x = xr.x + xr.w;
	pt[4].y = xr.y + xr.h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 5);
}

void draw_grid_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_graph_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 + ps;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4 * 3;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + prt->h / 2;

	(*pif->pf_draw_curve)(pif->ctx, &xp, pt, 5);
}

void draw_images_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h - 2 * ps;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + ps + SINGLE_SPAN;
	xr.y = prt->y + ps + SINGLE_SPAN;
	xr.w = prt->w / 4 - DOUBLE_SPAN;
	xr.h = prt->h / 4 - DOUBLE_SPAN;

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);
}

void draw_update_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xsize_t xs;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 2 - prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	pt[0].x = xr.x + xr.w;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w / 2;
	pt[1].y = xr.y;

	xs.w = xr.w / 2;
	xs.h = xr.h / 2;

	//XPI/2 ~2*XPI
	(*pif->pf_draw_arc)(pif->ctx, &xp, &pt[0], &pt[1], &xs, 1, 1);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4 + 1 * ps;
	pt[2].x = prt->x + prt->w / 2 + 2 * ps;
	pt[2].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_fetch_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xsize_t xs;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 2 - prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w / 2;
	pt[1].y = xr.y + xr.h;

	xs.w = xr.w / 2;
	xs.h = xr.h / 2;

	//XPI ~ XPI/2
	(*pif->pf_draw_arc)(pif->ctx, &xp, &pt[0], &pt[1], &xs, 1, 1);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4 * 3 + 1 * ps;
	pt[2].x = prt->x + prt->w / 2 - 2 * ps;
	pt[2].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_fresh_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[8];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + prt->w / 2 + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h / 4 * 3;
	pt[4].x = prt->x + prt->w / 2 - 2 * ps;
	pt[4].y = prt->y + prt->h / 4 * 3;
	pt[5].x = prt->x;
	pt[5].y = prt->y + prt->h / 4 * 3;
	pt[6].x = prt->x;
	pt[6].y = prt->y + prt->h / 4;
	pt[7].x = prt->x + prt->w / 2;
	pt[7].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 8);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 + 3 * ps;
	pt[2].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_helpc_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("9"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("8"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("C"), 1);
}

void draw_helpp_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	xfont_t xf;
	xface_t xa;

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("9"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("8"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("P"), 1);
}

void draw_start_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2 - 1 * ps;
	pt[2].y = prt->y + prt->h / 2 + 1 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_stop_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);
}

void draw_pause_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 - 1 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
	
	pt[0].x = prt->x + prt->w / 2 + 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_find_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 4 * 3;
	xr.h = prt->h / 4 * 3;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;
	
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_proper_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xsize_t xs;
	xpoint_t pt[2];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w;
	pt[1].y = xr.y + xr.h / 2;

	xs.w = xr.w / 2;
	xs.h = xr.h / 2;

	//XPI ~ 0
	(*pif->pf_draw_arc)(pif->ctx, &xp, &pt[0], &pt[1], &xs, 1, 0);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	(*pif->pf_draw_round)(pif->ctx, &xp, NULL, &xr);
}

void draw_style_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -DOUBLE_SPAN, -DOUBLE_SPAN);

	prt = &rt;

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 5);

	xr.x = prt->x + prt->w / 4 - 1 * ps;
	xr.y = prt->y + prt->h / 4 * 3 - 1 * ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	lighten_xpen(&xp, -20);
	lighten_xbrush(&xb, -20);

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);
}

void draw_note_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[7];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h;
	pt[4].x = prt->x + prt->w / 2;
	pt[4].y = prt->y + prt->h / 4 * 3;
	pt[5].x = prt->x + prt->w;
	pt[5].y = prt->y + prt->h / 4 * 3;
	pt[6].x = prt->x + prt->w;
	pt[6].y = prt->y;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 7);
}

void draw_book_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[10];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x;
	pt[0].y = prt->y + 1 * ps;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + +1 * ps;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + 1 * ps;
	pt[5].x = prt->x + prt->w;
	pt[5].y = prt->y + prt->h;
	pt[6].x = prt->x + prt->w / 4 * 3;
	pt[6].y = prt->y + prt->h - 1 * ps;
	pt[7].x = prt->x + prt->w / 2;
	pt[7].y = prt->y + prt->h;
	pt[8].x = prt->x + prt->w / 4;
	pt[8].y = prt->y + prt->h - 1 * ps;
	pt[9].x = prt->x;
	pt[9].y = prt->y + prt->h;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 10);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_import_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 2;
	xr.h = prt->h / 4;

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 4 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - 1 * ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[3].y = prt->y + prt->h / 2;
	
	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 1 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4;
	
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_export_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[10];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 2;
	xr.h = prt->h / 4;

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 4 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - 1 * ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[3].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 - 1 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_dialog_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[10];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 + ps;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 + ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	xr.x = prt->x + prt->w - 2 * ps - SINGLE_SPAN;
	xr.y = prt->y + ps + SINGLE_SPAN;
	xr.w = ps;
	xr.h = ps;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_calendar_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_diagram_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);
}

void draw_static_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	xrect_t rt, xr;

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xf.size, _T("9"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xf.size, _T("8"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("T"), -1);
}

void draw_list_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = (int)(1.5 * (float)ps);
	xr.h = (int)(1.5 * (float)ps);

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = (int)(1.5 * (float)ps);

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 2;
	xr.w = (int)(1.5 * (float)ps);
	xr.h = (int)(1.5 * (float)ps);

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = (int)(1.5 * (float)ps);

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = (int)(1.5 * (float)ps);
	xr.h = (int)(1.5 * (float)ps);

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 2;
	xr.h = (int)(1.5 * (float)ps);

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
}

void draw_navi_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4 - ps;
	pt[0].y = prt->y + prt->h / 2 - ps;
	pt[1].x = prt->x + prt->w / 4 - ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 4 + ps;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 - ps;
	pt[1].x = prt->x + prt->w / 4 * 3 - ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 4 * 3 + ps;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_spin_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[3];
	int ps;

	xface_t xa;
	xfont_t xf;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("8"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xscpy(xp.size, _T("1"));

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2 - SINGLE_SPAN;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("2"), -1);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 - SINGLE_SPAN;
	pt[1].x = prt->x + prt->w / 4 * 3 + ps;
	pt[1].y = prt->y + prt->h / 2 - SINGLE_SPAN;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 - 2 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 + SINGLE_SPAN;
	pt[1].x = prt->x + prt->w / 4 * 3 + ps;
	pt[1].y = prt->y + prt->h / 2 + SINGLE_SPAN;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 + 2 * ps;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);
}

void draw_slide_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[3];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h / 2;

	xp.adorn.feed = 2;
	xp.adorn.size = 2;
	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 + ps;
	pt[0].y = prt->y + prt->h / 2 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4 + ps;
	pt[1].y = prt->y + prt->h / 2 + 2 * ps;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_radio_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);
}

void draw_date_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	xface_t xa;
	xfont_t xf;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("8"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("23"), -1);
}

void draw_time_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[3];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 3);
}

void draw_push_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2 - 2 * ps;
	xr.w = prt->w / 2;
	xr.h = 4 * ps;

	xp.adorn.feed = 2;
	xp.adorn.size = 2;
	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_person_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;
	tchar_t ta[10] = { 0 };
	xpoint_t pa[12] = { 0 };

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	ta[0] = _T('M');
	pa[0].x = prt->x;
	pa[0].y = prt->y + prt->h;

	ta[1] = _T('L');
	pa[1].x = prt->x;
	pa[1].y = prt->y + prt->h / 2 + 2 * ps;

	ta[2] = _T('A');
	pa[2].x = 1;
	pa[2].y = 0;
	pa[3].x = 2 * ps;
	pa[3].y = 2 * ps;
	pa[4].x = prt->x + 2 * ps;
	pa[4].y = prt->y + prt->h / 2;

	ta[3] = _T('L');
	pa[5].x = prt->x + prt->w - 2 * ps;
	pa[5].y = prt->y + prt->h / 2;

	ta[4] = _T('A');
	pa[6].x = 1;
	pa[6].y = 0;
	pa[7].x = 2 * ps;
	pa[7].y = 2 * ps;
	pa[8].x = prt->x + prt->w;
	pa[8].y = prt->y + prt->h / 2 + 2 * ps;

	ta[5] = _T('L');
	pa[9].x = prt->x + prt->w;
	pa[9].y = prt->y + prt->h;

	ta[6] = _T('L');
	pa[10].x = prt->x;
	pa[10].y = prt->y + prt->h;

	(*pif->pf_draw_path)(pif->ctx, &xp, NULL, ta, pa, 11);
}

void draw_user_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xsize_t xs;
	xpoint_t pt[3];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 4 * ps;
	xr.h = prt->h / 4;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w;
	xr.h = prt->h;

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w;
	pt[1].y = xr.y + xr.h / 2;

	xs.w = xr.w / 2;
	xs.h = xr.h / 2;

	//XPI ~ 0
	(*pif->pf_draw_arc)(pif->ctx, &xp, &pt[0], &pt[1], &xs, 1, 0);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_location_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xsize_t xs;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w;
	pt[1].y = xr.y + xr.h / 2;

	xs.w = xr.w / 2;
	xs.h = xr.h / 2;

	//XPI ~ 0
	(*pif->pf_draw_arc)(pif->ctx, &xp, &pt[0], &pt[1], &xs, 1, 0);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h - ps;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	(*pif->pf_draw_bezier)(pif->ctx, &xp, &pt[0], &pt[1], &pt[2], &pt[3]);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + 2 * ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);
}

void draw_trends_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[4];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h / 4;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);
}

void draw_panto_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_pie)(pif->ctx, &xp, &xb, RECTPOINT(&xr), RECTSIZE(&xr), 0, XPI / 2);

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	(*pif->pf_draw_pie)(pif->ctx, &xp, &xb, RECTPOINT(&xr), RECTSIZE(&xr), XPI, XPI / 2 * 3);
}

void draw_scatter_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	xscpy(xp.size, _T("1"));

	prt = &rt;

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w / 2 - DOUBLE_SPAN;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 2 + DOUBLE_SPAN;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h;

	(*pif->pf_draw_bezier)(pif->ctx, &xp, &pt[0], &pt[1], &pt[2], &pt[3]);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = 2;
	xr.h = 2;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = 2;
	xr.h = 2;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = 2;
	xr.h = 2;

	(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);
}

void draw_density_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	xscpy(xp.size, _T("1"));

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);
	(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);
}

void draw_counter_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	xface_t xa;
	xfont_t xf;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("8"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("0"), -1);
}

void draw_process_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[2];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 2;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_judge_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	xface_t xa;
	xfont_t xf;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xfont(&xf);
	default_xface(&xa);
	format_xcolor(pxc, xf.color);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("8"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, NULL, pt, 4);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, _T("?"), -1);
}

void draw_lock_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h / 2;

	(*pif->pf_draw_round)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4;

	(*pif->pf_draw_bezier)(pif->ctx, &xp, &pt[0], &pt[1], &pt[1], &pt[2]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_unlock_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h / 2;

	(*pif->pf_draw_round)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4;

	(*pif->pf_draw_bezier)(pif->ctx, &xp, &pt[0], &pt[1], &pt[1], &pt[2]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;

	(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
}

void draw_home_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > MIDD_GIZMO && prt->h > MIDD_GIZMO)
	{
		xscpy(xp.size, _T("3"));
		rt.w = LARGE_GIZMO;
		rt.h = LARGE_GIZMO;
	}
	else if (prt->w > SMALL_GIZMO && prt->h > SMALL_GIZMO)
	{
		xscpy(xp.size, _T("2"));
		rt.w = MIDD_GIZMO;
		rt.h = MIDD_GIZMO;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = SMALL_GIZMO;
		rt.h = SMALL_GIZMO;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, pt, 3);

	xr.x = prt->x + prt->w / 4 + ps;
	xr.y = prt->y + prt->h / 2 + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 4;

	(*pif->pf_draw_rect)(pif->ctx, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w - ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w - ps;
	pt[3].y = prt->y + prt->h / 2;

	(*pif->pf_draw_polyline)(pif->ctx, &xp, pt, 4);
}

/********************************************************************************************/
typedef void(*PF_GIZMO_MAKE)(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* prt);

typedef struct _GIZMO_DRAW_TABLE{
	tchar_t gizmo_name[32];
	PF_GIZMO_MAKE gizmo_func;
}GIZMO_DRAW_TABLE;

GIZMO_DRAW_TABLE g_gizmo_table[] = {
	{ GDI_ATTR_GIZMO_ALIGNCENTER, draw_aligncenter_gizmo },
	{ GDI_ATTR_GIZMO_ALIGNFAR, draw_alignfar_gizmo },
	{ GDI_ATTR_GIZMO_ALIGNNEAR, draw_alignnear_gizmo },
	{ GDI_ATTR_GIZMO_APPEND, draw_append_gizmo },
	{ GDI_ATTR_GIZMO_ARRANGECENTER, draw_arrangecenter_gizmo },
	{ GDI_ATTR_GIZMO_ARRANGELEFT, draw_arrangelept_gizmo },
	{ GDI_ATTR_GIZMO_ARRANGERIGHT, draw_arrangeright_gizmo },
	{ GDI_ATTR_GIZMO_BACKGROUND, draw_background_gizmo },
	{ GDI_ATTR_GIZMO_BEGIN, draw_begin_gizmo },
	{ GDI_ATTR_GIZMO_BOOK, draw_book_gizmo },
	{ GDI_ATTR_GIZMO_BORDER, draw_border_gizmo },
	{ GDI_ATTR_GIZMO_CALENDAR, draw_calendar_gizmo },
	{ GDI_ATTR_GIZMO_CHECK, draw_check_gizmo },
	{ GDI_ATTR_GIZMO_CHECKBOX, draw_checkbox_gizmo },
	{ GDI_ATTR_GIZMO_CHECKED, draw_checked_gizmo },
	{ GDI_ATTR_GIZMO_CLEAR, draw_clear_gizmo },
	{ GDI_ATTR_GIZMO_CLOSE, draw_close_gizmo },
	{ GDI_ATTR_GIZMO_CODE, draw_code_gizmo },
	{ GDI_ATTR_GIZMO_COLLAPSE, draw_collapse_gizmo },
	{ GDI_ATTR_GIZMO_COPY, draw_copy_gizmo },
	{ GDI_ATTR_GIZMO_COUNTER, draw_counter_gizmo },
	{ GDI_ATTR_GIZMO_CUT, draw_cut_gizmo },
	{ GDI_ATTR_GIZMO_DATE, draw_date_gizmo },
	{ GDI_ATTR_GIZMO_DELETE, draw_delete_gizmo },
	{ GDI_ATTR_GIZMO_DENSITY, draw_density_gizmo },
	{ GDI_ATTR_GIZMO_DIAGRAM, draw_diagram_gizmo },
	{ GDI_ATTR_GIZMO_DIALOG, draw_dialog_gizmo },
	{ GDI_ATTR_GIZMO_DOC, draw_doc_gizmo },
	{ GDI_ATTR_GIZMO_DOWN, draw_down_gizmo },
	{ GDI_ATTR_GIZMO_EDIT, draw_edit_gizmo },
	{ GDI_ATTR_GIZMO_END, draw_end_gizmo },
	{ GDI_ATTR_GIZMO_EXECUTE, draw_execute_gizmo },
	{ GDI_ATTR_GIZMO_EXPAND, draw_expand_gizmo },
	{ GDI_ATTR_GIZMO_EXPORT, draw_export_gizmo },
	{ GDI_ATTR_GIZMO_FETCH, draw_fetch_gizmo },
	{ GDI_ATTR_GIZMO_FIND, draw_find_gizmo },
	{ GDI_ATTR_GIZMO_FIRST, draw_first_gizmo },
	{ GDI_ATTR_GIZMO_FIXED, draw_fixed_gizmo },
	{ GDI_ATTR_GIZMO_FOLDER, draw_folder_gizmo },
	{ GDI_ATTR_GIZMO_FONTCOLOR, draw_fontcolor_gizmo },
	{ GDI_ATTR_GIZMO_FONTNAME, draw_fontname_gizmo },
	{ GDI_ATTR_GIZMO_FONTSIZE, draw_fontsize_gizmo },
	{ GDI_ATTR_GIZMO_FONTSTYLE, draw_fontstyle_gizmo },
	{ GDI_ATTR_GIZMO_FONTWEIGHT, draw_fontweight_gizmo },
	{ GDI_ATTR_GIZMO_FOREGROUND, draw_foreground_gizmo },
	{ GDI_ATTR_GIZMO_FRESH, draw_fresh_gizmo },
	{ GDI_ATTR_GIZMO_GRAPH, draw_graph_gizmo },
	{ GDI_ATTR_GIZMO_GRID, draw_grid_gizmo },
	{ GDI_ATTR_GIZMO_GROUP, draw_group_gizmo },
	{ GDI_ATTR_GIZMO_GUIDER, draw_guider_gizmo },
	{ GDI_ATTR_GIZMO_HELPC, draw_helpc_gizmo },
	{ GDI_ATTR_GIZMO_HELPP, draw_helpp_gizmo },
	{ GDI_ATTR_GIZMO_HERF, draw_herf_gizmo },
	{ GDI_ATTR_GIZMO_HOME, draw_home_gizmo },
	{ GDI_ATTR_GIZMO_IMAGES, draw_images_gizmo },
	{ GDI_ATTR_GIZMO_IMPORT, draw_import_gizmo },
	{ GDI_ATTR_GIZMO_INPUT, draw_input_gizmo },
	{ GDI_ATTR_GIZMO_INSERT, draw_insert_gizmo },
	{ GDI_ATTR_GIZMO_JUDGE, draw_judge_gizmo },
	{ GDI_ATTR_GIZMO_KEYBOX, draw_keybox_gizmo },
	{ GDI_ATTR_GIZMO_KEYBOXED, draw_keyboxed_gizmo },
	{ GDI_ATTR_GIZMO_LABEL, draw_label_gizmo },
	{ GDI_ATTR_GIZMO_LAST, draw_last_gizmo },
	{ GDI_ATTR_GIZMO_LIST, draw_list_gizmo },
	{ GDI_ATTR_GIZMO_LOCATION, draw_location_gizmo },
	{ GDI_ATTR_GIZMO_LOCK, draw_lock_gizmo },
	{ GDI_ATTR_GIZMO_LOGO, draw_logo_gizmo },
	{ GDI_ATTR_GIZMO_MAXIMIZE, draw_maximize_gizmo },
	{ GDI_ATTR_GIZMO_MEMO, draw_memo_gizmo },
	{ GDI_ATTR_GIZMO_MINIMIZE, draw_minimize_gizmo },
	{ GDI_ATTR_GIZMO_MINUS, draw_minus_gizmo },
	{ GDI_ATTR_GIZMO_MULTITEXT, draw_multitext_gizmo },
	{ GDI_ATTR_GIZMO_NAVI, draw_navi_gizmo },
	{ GDI_ATTR_GIZMO_NEW, draw_new },
	{ GDI_ATTR_GIZMO_NEXT, draw_next_gizmo },
	{ GDI_ATTR_GIZMO_NOTE, draw_note_gizmo },
	{ GDI_ATTR_GIZMO_NUMERIC, draw_numeric_gizmo },
	{ GDI_ATTR_GIZMO_OMIT, draw_omit_gizmo },
	{ GDI_ATTR_GIZMO_OPEN, draw_open_gizmo },
	{ GDI_ATTR_GIZMO_ORDER, draw_order_gizmo },
	{ GDI_ATTR_GIZMO_OUTPUT, draw_output_gizmo },
	{ GDI_ATTR_GIZMO_PANTO, draw_panto_gizmo },
	{ GDI_ATTR_GIZMO_PASTE, draw_paste_gizmo },
	{ GDI_ATTR_GIZMO_PAUSE, draw_pause_gizmo },
	{ GDI_ATTR_GIZMO_PERSON, draw_person_gizmo },
	{ GDI_ATTR_GIZMO_PHOTO, draw_photo_gizmo },
	{ GDI_ATTR_GIZMO_PLUS, draw_plus_gizmo },
	{ GDI_ATTR_GIZMO_PREV, draw_prev_gizmo },
	{ GDI_ATTR_GIZMO_PREVIEW, draw_preview_gizmo },
	{ GDI_ATTR_GIZMO_PRINT, draw_print_gizmo },
	{ GDI_ATTR_GIZMO_PROCESS, draw_process_gizmo },
	{ GDI_ATTR_GIZMO_PROPER, draw_proper_gizmo },
	{ GDI_ATTR_GIZMO_PUSH, draw_push_gizmo },
	{ GDI_ATTR_GIZMO_RADIO, draw_radio_gizmo },
	{ GDI_ATTR_GIZMO_RADIOBOX, draw_radiobox_gizmo },
	{ GDI_ATTR_GIZMO_RADIOED, draw_radioed_gizmo },
	{ GDI_ATTR_GIZMO_REDO, draw_redo_gizmo },
	{ GDI_ATTR_GIZMO_REMOVE, draw_remove_gizmo },
	{ GDI_ATTR_GIZMO_RESTORE, draw_restore_gizmo },
	{ GDI_ATTR_GIZMO_RICH, draw_rich_gizmo },
	{ GDI_ATTR_GIZMO_SAVE, draw_save_gizmo },
	{ GDI_ATTR_GIZMO_SAVEAS, draw_saveas_gizmo },
	{ GDI_ATTR_GIZMO_SCATTER, draw_scatter_gizmo },
	{ GDI_ATTR_GIZMO_SCHEMA, draw_schema_gizmo },
	{ GDI_ATTR_GIZMO_SCREEN, draw_screen_gizmo },
	{ GDI_ATTR_GIZMO_SELECTALL, draw_selectall_gizmo },
	{ GDI_ATTR_GIZMO_SELECTED, draw_selected_gizmo },
	{ GDI_ATTR_GIZMO_SHAPE, draw_shape_gizmo },
	{ GDI_ATTR_GIZMO_SINGLETEXT, draw_singletext_gizmo },
	{ GDI_ATTR_GIZMO_SIZEHORZ, draw_sizehorz_gizmo },
	{ GDI_ATTR_GIZMO_SIZEVERT, draw_sizevert_gizmo },
	{ GDI_ATTR_GIZMO_SLIDE, draw_slide_gizmo },
	{ GDI_ATTR_GIZMO_SPACEHORZ, draw_spacehorz_gizmo },
	{ GDI_ATTR_GIZMO_SPACEVERT, draw_spacevert_gizmo },
	{ GDI_ATTR_GIZMO_SPIN, draw_spin_gizmo },
	{ GDI_ATTR_GIZMO_START, draw_start_gizmo },
	{ GDI_ATTR_GIZMO_STATIC, draw_static_gizmo },
	{ GDI_ATTR_GIZMO_STOP, draw_stop_gizmo },
	{ GDI_ATTR_GIZMO_STYLE, draw_style_gizmo },
	{ GDI_ATTR_GIZMO_SUM, draw_sum_gizmo },
	{ GDI_ATTR_GIZMO_TABLE, draw_table_gizmo },
	{ GDI_ATTR_GIZMO_TAG, draw_tag_gizmo },
	{ GDI_ATTR_GIZMO_TIME, draw_time_gizmo },
	{ GDI_ATTR_GIZMO_TOUCH, draw_touch_gizmo },
	{ GDI_ATTR_GIZMO_TOUCHED, draw_touched_gizmo },
	{ GDI_ATTR_GIZMO_TRENDS, draw_trends_gizmo },
	{ GDI_ATTR_GIZMO_UNDO, draw_undo_gizmo },
	{ GDI_ATTR_GIZMO_UNLOCK, draw_unlock_gizmo },
	{ GDI_ATTR_GIZMO_UP, draw_up_gizmo },
	{ GDI_ATTR_GIZMO_UPDATE, draw_update_gizmo },
	{ GDI_ATTR_GIZMO_USER, draw_user_gizmo },
	{ GDI_ATTR_GIZMO_ZOOMIN, draw_zoomin_gizmo },
	{ GDI_ATTR_GIZMO_ZOOMOUT, draw_zoomout_gizmo },
};

static PF_GIZMO_MAKE _find_gizmo_maker(int i, int j, const tchar_t* iname)
{
	int rt, k;

	if (i == j)
	{
		rt = compare_text(iname, -1, g_gizmo_table[i].gizmo_name, -1, 1);
		if (!rt)
			return g_gizmo_table[i].gizmo_func;
		else
			return NULL;
	}
	else if (i < j)
	{
		k = (i + j) / 2;
		rt = compare_text(iname, -1, g_gizmo_table[k].gizmo_name, -1, 1);
		if (!rt)
			return g_gizmo_table[k].gizmo_func;
		else if (rt > 0)
			return _find_gizmo_maker(k + 1, j, iname);
		else
			return _find_gizmo_maker(i, k - 1, iname);
	}
	else
	{
		return NULL;
	}
}

PF_GIZMO_MAKE find_gizmo_maker(const tchar_t* iname)
{
	int k;

	if (is_null(iname))
		return NULL;

	k = sizeof(g_gizmo_table) / sizeof(GIZMO_DRAW_TABLE);

	return _find_gizmo_maker(0, k - 1, iname);
}


void draw_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* pxr, const tchar_t* iname)
{
	PF_GIZMO_MAKE pf;
	if_drawing_t it = { 0 };
	visual_t visu;
	xrect_t xr;

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	(*pif->pf_rect_tm_to_pt)(pif->ctx, &xr);

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &it);

	pf = find_gizmo_maker(iname);
	if (pf)
	{
		(*pf)(&it, pxc, &xr);
	}
}

#if defined(_DEBUG) || defined(DEBUG)

void test_gizmo(const if_drawing_t* pif, const xcolor_t* pxc, const xrect_t* pxr)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	int i, j, k;
	int feed = 10;
	int split = 200;

	if_drawing_t it = { 0 };
	visual_t visu;

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &it);

	k = sizeof(g_gizmo_table) / sizeof(GIZMO_DRAW_TABLE);

	default_xfont(&xf);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);

	j = 0;
	for (i = 0; k--; i++)
	{
		xr.x = pxr->x + j * split + feed;
		xr.y = pxr->y + i * (LARGE_GIZMO + feed);
		xr.w = SMALL_GIZMO;
		xr.h = SMALL_GIZMO;

		(*(g_gizmo_table[k].gizmo_func))(&it, pxc, &xr);

		xr.x = pxr->x + j * split + SMALL_GIZMO + 2 * feed;
		xr.y = pxr->y + i * (LARGE_GIZMO + feed);
		xr.w = MIDD_GIZMO;
		xr.h = MIDD_GIZMO;

		(*(g_gizmo_table[k].gizmo_func))(&it, pxc, &xr);

		xr.x = pxr->x + j * split + SMALL_GIZMO + MIDD_GIZMO + 3 * feed;
		xr.y = pxr->y + i * (LARGE_GIZMO + feed);
		xr.w = LARGE_GIZMO;
		xr.h = LARGE_GIZMO;

		(*(g_gizmo_table[k].gizmo_func))(&it, pxc, &xr);

		xr.x = pxr->x + j * split + SMALL_GIZMO + MIDD_GIZMO + LARGE_GIZMO + 4 * feed;
		xr.y = pxr->y + i * (LARGE_GIZMO + feed);
		xr.w = LARGE_GIZMO;
		xr.h = LARGE_GIZMO;

		(*it.pf_text_out)(it.ctx, &xf, RECTPOINT(&xr), g_gizmo_table[k].gizmo_name, -1);

		if (i > 15)
		{
			i = 0;
			j++;
		}
	}
}

#endif

#endif


