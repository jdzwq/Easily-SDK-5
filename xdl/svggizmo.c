/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc gizmo document

	@module	gizmogdi.c | implement file

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

#include "svggizmo.h"
#include "svggdi.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_SVG)

void svg_draw_logo_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2 - 2;
	xr.h = prt->h / 2 - 2;
	svg_draw_round_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 + 1;
	xr.y = prt->y;
	xr.w = prt->w / 2 - 2;
	xr.h = prt->h / 2 - 2;
	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 + 1;
	xr.w = prt->w / 2 - 2;
	xr.h = prt->h / 2 - 2;
	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 + 1;
	xr.y = prt->y + prt->h / 2 + 1;
	xr.w = prt->w / 2 - 2;
	xr.h = prt->h / 2 - 2;
	svg_draw_round_raw(g, &xp, &xb, &xr);
}

void svg_draw_plus_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt;
	xpoint_t pt[2];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xp.adorn.feed = 1;
	xp.adorn.size = 1;

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &(pt[0]), &(pt[1]));

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &(pt[0]), &(pt[1]));
}

void svg_draw_minus_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt;
	xpoint_t pt[2];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	xp.adorn.feed = 1;
	xp.adorn.size = 1;
	svg_draw_line_raw(g, &xp, &(pt[0]), &(pt[1]));
}

void svg_draw_collapse_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_expand_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_begin_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 + ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_up_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 + ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_down_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3 - ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_end_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3 - ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_first_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 + ps;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_prev_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_next_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_last_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 * 3 - ps;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_zoomin_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 3 * 2 + 1;
	xr.h = prt->h / 3 * 2 + 1;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = prt->x + prt->w / 3 * 2;
	pt1.y = prt->y + prt->h / 3 * 2;
	pt2.x = prt->x + prt->w;
	pt2.y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_zoomout_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 3 * 2 + 1;
	xr.h = prt->h / 3 * 2 + 1;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + ps + 1;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h - ps - 1;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = prt->x + prt->w / 3 * 2;
	pt1.y = prt->y + prt->h / 3 * 2;
	pt2.x = prt->x + prt->w;
	pt2.y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_keybox_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	lighten_xpen(&xp, DEF_HARD_DARKEN);

	xr.x = prt->x;
	xr.y = prt->y + ps;
	xr.w = prt->w;
	xr.h = prt->h - 2 * ps;

	svg_draw_round_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w - 2 * ps;
	xr.h = ps * 2;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_keyboxed_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	lighten_xpen(&xp, -20);

	xr.x = prt->x - 1;
	xr.y = prt->y + 1;
	xr.w = prt->w + 2;
	xr.h = prt->h - 2;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	pt1.x = xr.x;
	pt1.y = xr.y + 2;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + 2;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x;
	pt1.y = xr.y + 4;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + 4;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + 3;
	pt1.y = xr.y + 4;
	pt2.x = xr.x + 3;
	pt2.y = xr.y + xr.h;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w - 3;
	pt1.y = xr.y + 4;
	pt2.x = xr.x + xr.w - 3;
	pt2.y = xr.y + xr.h;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_touch_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	lighten_xpen(&xp, -20);

	xr.x = prt->x - 1;
	xr.y = prt->y - 1;
	xr.w = prt->w + 2;
	xr.h = prt->h + 2;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	pt1.x = xr.x + xr.w / 2 - 3;
	pt1.y = xr.y + xr.h / 2 - 3;
	pt2.x = xr.x + xr.w / 2 + 3;
	pt2.y = xr.y + xr.h / 2 + 3;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2 - 3;
	pt1.y = xr.y + xr.h / 2 + 3; 
	pt2.x = xr.x + xr.w / 2 + 3;
	pt2.y = xr.y + xr.h / 2 - 3;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_touched_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	lighten_xpen(&xp, -20);

	xr.x = prt->x - 1;
	xr.y = prt->y - 1;
	xr.w = prt->w + 2;
	xr.h = prt->h + 2;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	pt_expand_rect(&xr, -2, -2);

	lighten_xbrush(&xb, 20);

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);
}

void svg_draw_close_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps;
	pt1.y = xr.y + xr.h - ps;
	pt2.x = xr.x + xr.w - ps;
	pt2.y = xr.y + ps;

	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_minimize_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y + prt->h - 2;
	xr.w = 4;
	xr.h = 2;
	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_maximize_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.w = prt->w - 2;
	xr.y = prt->y;
	xr.h = prt->h - 2;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + 2;
	xr.w = prt->w - 2;
	xr.y = prt->y + 2;
	xr.h = prt->h - 2;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_restore_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_sum_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5] = { 0 };
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	if (xr.w % 2 == 0)
		xr.w -= 1;
	if (xr.h % 2 == 0)
		xr.h -= 1;

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

	svg_draw_polyline_raw(g, &xp, pt, 5);
}

void svg_draw_checkbox_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_checked_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	//svg_draw_rect_raw(g, &xp, &xb, &xr);

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + 2;
	pt2.y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + 2;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h - 1;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h - 1;
	pt2.x = xr.x + xr.w + 2;
	pt2.y = xr.y + 1;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_radiobox_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h - 2 * ps;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);
}

void svg_draw_radioed_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);

	prt = &rt;

	xr.x = prt->x + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h - 2 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);
}

void svg_draw_selected_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + 2;
	pt2.y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + 2;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_folder_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polyline_raw(g, &xp, pt, 4);

	xr.y += xr.h / 4;
	xr.h -= xr.h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_guider_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polygon_raw(g, &xp, &xb, pt, 5);
}

void svg_draw_fixed_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);

	prt = &rt;

	pt1.x = prt->x + 2 * ps;
	pt1.y = prt->y + prt->h / 2;
	pt2.x = prt->x + 4 * ps;
	pt2.y = prt->y + prt->h / 2;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = prt->x + 4 * ps;
	pt1.y = prt->y + prt->h / 4;
	pt2.x = prt->x + 4 * ps;
	pt2.y = prt->y + prt->h / 4 * 3;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);
	
	xr.x = prt->x + 4 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w - 6 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_numeric_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	svg_draw_ellipse_raw(g, &xp, &xb, prt);

	fs = font_size(prt->h);
	if (fs < 8.0f)
		fs = 8.0f;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ftoxs(fs, xf.size, INT_LEN);
	xscpy(xf.color, GDI_ATTR_RGB_WHITE);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	//svg_draw_text_raw(g, &xf, &xa, prt, no, -1);
}

void svg_draw_omit_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	fs = font_size(prt->h);
	if (fs < 8.0f)
		fs = 8.0f;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ftoxs(fs, xf.size, INT_LEN);
	xscpy(xf.color, GDI_ATTR_RGB_WHITE);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	svg_draw_text_raw(g, &xf, &xa, prt, _T("..."), -1);
}

void svg_draw_doc_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 4;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - 1;
	pt2.y = xr.y + xr.h / 4 * 3;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_new_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polygon_raw(g, &xp, NULL, pt, 5);

	pt[0].x = xr.x + xr.w / 4 * 3;
	pt[0].y = xr.y + xr.h - 1;
	pt[1].x = xr.x + xr.w / 4 * 3;
	pt[1].y = xr.y + xr.h / 4 * 3;
	pt[2].x = xr.x + xr.w - 1;
	pt[2].y = xr.y + xr.h / 4 * 3;

	svg_draw_polyline_raw(g, &xp, pt, 3);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 4;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - 1;
	pt[1].y = xr.y + xr.h / 4 * 3;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_open_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + ps;
	xr.w = prt->w - 3 * ps;
	xr.h = prt->h - ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h;
	pt[1].x = xr.x + 3 * ps;
	pt[1].y = xr.y + xr.h / 2;
	pt[2].x = xr.x + xr.w + 3 * ps;
	pt[2].y = xr.y + xr.h / 2;
	pt[3].x = xr.x + xr.w;
	pt[3].y = xr.y + xr.h;

	format_xcolor(pxc, xb.color);
	svg_draw_polygon_raw(g, &xp, &xb, pt, 4);
}

void svg_draw_save_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		ps = 3;
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		ps = 2;
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		ps = 1;
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

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
	pt[5].y = prt->y + prt->h / 4 + 2;
	pt[6].x = prt->x + prt->w / 4;
	pt[6].y = prt->y + prt->h / 4 + 2;
	pt[7].x = prt->x + prt->w / 4;
	pt[7].y = prt->y;

	format_xcolor(pxc, xb.color);
	svg_draw_polygon_raw(g, &xp, &xb, pt, 8);
}

void svg_draw_saveas_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		ps = 3;
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		ps = 2;
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		ps = 1;
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

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
	pt[6].y = prt->y + prt->h / 4 + 2;
	pt[7].x = prt->x + prt->w / 4;
	pt[7].y = prt->y + prt->h / 4 + 2;
	pt[8].x = prt->x + prt->w / 4;
	pt[8].y = prt->y;

	format_xcolor(pxc, xb.color);
	svg_draw_polygon_raw(g, &xp, &xb, pt, 9);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w / 2 + 4;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 2 + 4;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_schema_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w / 2 - ps - 1;
	pt[1].y = xr.y + xr.h / 4;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w / 2 + ps;
	pt[1].y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - ps - 1;
	pt[1].y = xr.y + xr.h / 4 * 3;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + xr.w / 2 + ps;
	pt[0].y = xr.y + xr.h / 2 - ps;
	pt[1].x = xr.x + xr.w / 4 * 3;
	pt[1].y = xr.y + xr.h / 2;
	pt[2].x = xr.x + xr.w / 2 + ps;
	pt[2].y = xr.y + xr.h / 2 + ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_output_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polyline_raw(g, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;
	
	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_input_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polyline_raw(g, &xp, pt, 7);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 * 3;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_print_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polygon_raw(g, &xp, NULL, pt, 4);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w;
	xr.h = prt->h / 2 + ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - ps;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 4 * 3 - ps;
	pt[2].y = prt->y + prt->h - ps;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, NULL, pt, 4);
}

void svg_draw_preview_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_round_raw(g, &xp, &xb, &xr);
}

void svg_draw_screen_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	svg_draw_round_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = ps;
	xr.h = ps;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	svg_draw_round_raw(g, &xp, &xb, &xr);

	lighten_xbrush(&xb, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_execute_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polyline_raw(g, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 + 3 * ps;
	pt[2].y = prt->y + prt->h / 4;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2 + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4 * 3;

	svg_draw_polyline_raw(g, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 * 3 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 * 3 - 3 * ps;
	pt[2].y = prt->y + prt->h / 4 * 3;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_selectall_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 4;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	pt[0].x = prt->x + prt->w / 2 + ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h - ps - 1;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 * 3 - 2 * ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_delete_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + ps;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h - ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h - ps;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_insert_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - 1 * ps;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 2;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + 2;
	pt[2].x = prt->x + prt->w - 2 * ps - 4;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_remove_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w /2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 4 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 1 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_clear_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 4 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 1 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h - 1 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h - 4 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_append_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 2 + 3 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_copy_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 4 * 3;
	xr.h = prt->h / 4 * 3;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

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

	svg_draw_polyline_raw(g, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 + 1;
	pt[1].x = prt->x + prt->w - 1;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, NULL, pt, 3);
}

void svg_draw_cut_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polyline_raw(g, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, NULL, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);
	xscpy(xp.size, _T("1"));
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_paste_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_undo_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4 * 3;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h / 2;

	svg_draw_polyline_raw(g, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 - 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 + 2 * ps;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 4 + 3 * ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_redo_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4 * 3;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	svg_draw_polyline_raw(g, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 + 3 * ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_fontname_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("8"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("7"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("A"), 1);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("A"), 1);
}

void svg_draw_fontsize_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xf.size, _T("8"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xf.size, _T("7"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xf.size, _T("6"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xscpy(xp.size, _T("1"));

	ps = 2;

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + 2 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + ps;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;;
	pt[1].y = prt->y + 2 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	svg_draw_text_raw(g, &xf, &xa, &xr, _T("A"), 1);
}

void svg_draw_fontweight_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("9"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("8"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("B"), 1);
}

void svg_draw_fontcolor_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xf.size, _T("8"));
		ps = 3;
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xf.size, _T("7"));
		ps = 2;
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xf.size, _T("6"));
		ps = 1;
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("A"), 1);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 4 * 3 + ps;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h / 4 - ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_fontstyle_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xf.size, _T("9"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xf.size, _T("8"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.style, GDI_ATTR_FONT_STYLE_UNDERLINE);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("U"), 1);
}

void svg_draw_foreground_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4 * 3 - 2 * ps;
	xr.y = prt->y;
	xr.w = 4 * ps;
	xr.h = prt->h / 2 - ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	svg_draw_polygon_raw(g, &xp, NULL, pt, 3);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_background_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y;
	xr.w = 4 * ps;
	xr.h = prt->h / 2 - ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 2 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + prt->w / 2 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	svg_draw_polygon_raw(g, &xp, NULL, pt, 3);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h - 2 * ps;
	xr.w = prt->w - 2 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_alignnear_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	ltoxs(ps - 1, xp.size, INT_LEN);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_aligncenter_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	ltoxs(ps - 1, xp.size, INT_LEN);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_alignfar_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	ltoxs(ps - 1, xp.size, INT_LEN);

	pt[0].x = prt->x + prt->w / 2 + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2 - ps;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_arrangeleft_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	ltoxs(ps - 1, xp.size, INT_LEN);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 2;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w / 4;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_arrangecenter_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	ltoxs(ps - 1, xp.size, INT_LEN);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 2;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 4 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + 2 * ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w - 4 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_arrangeright_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	ltoxs(ps - 1, xp.size, INT_LEN);
	if (!ps)
		ps = 1;

	xr.x = prt->x + prt->w - 6 * ps - 1;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 5 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w - 8 * ps - 1;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 7 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w - 4 * ps - 1;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = 3 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_sizehorz_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	ltoxs(ps - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 2 + prt->h / 8;

	svg_draw_polyline_raw(g, &xp, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4;

	svg_draw_polyline_raw(g, &xp, pt, 3);
}

void svg_draw_sizevert_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 4;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	ltoxs(ps - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 8;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y;

	svg_draw_polyline_raw(g, &xp, pt, 3);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 8;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h;

	svg_draw_polyline_raw(g, &xp, pt, 3);
}

void svg_draw_spacehorz_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = 2 * ps;
	xr.h = prt->h / 4 * 3;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 2 * ps;
	xr.h = prt->h / 4 * 3;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w - 2 * ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 2 * ps;
	xr.h = prt->h / 4 * 3;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	ltoxs(ps - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + 2 * ps;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 2 - ps;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w / 2 - ps;
	pt[3].y = prt->y + prt->h / 4;

	svg_draw_polyline_raw(g, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 + ps;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w - 2 * ps;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w - 2 * ps;
	pt[3].y = prt->y + prt->h / 4;

	svg_draw_polyline_raw(g, &xp, pt, 4);
}

void svg_draw_spacevert_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h - 2 * ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	ltoxs(ps - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + 2 * ps;
	pt[1].x = prt->x;
	pt[1].y = prt->y + 2 * ps;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h / 2 - ps;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h / 2 - ps;

	svg_draw_polyline_raw(g, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h - 2 * ps;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h - 2 * ps;

	svg_draw_polyline_raw(g, &xp, pt, 4);
}

void svg_draw_edit_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y;
	xr.w = 4 * ps;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 2 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + 1 * ps + 1;
	pt[1].x = prt->x + prt->w / 2 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps + 1;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	svg_draw_polygon_raw(g, &xp, NULL, pt, 3);
}

void svg_draw_group_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 + 2 * ps;
	xr.y = prt->y + prt->h / 4 + 2 * ps;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	ltoxs(ps - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_order_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 4 - 1.5 * ps;
	xr.y = prt->y + prt->h / 4 - 1.5 * ps;
	xr.w = 3 * ps;
	xr.h = 3 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - 1.5 * ps;
	xr.y = prt->y + prt->h / 4 * 3 - 1.5 * ps;
	xr.w = 3 * ps;
	xr.h = 3 * ps;

	lighten_xpen(&xp, 0 - DEF_HARD_DARKEN);
	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_border_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;
	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_shape_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 2 - 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 2 - 2 * ps;

	svg_draw_round_raw(g, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + ps;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 2 - ps;
	pt[2].y = prt->y + prt->h - ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	xr.x = prt->x + prt->w / 2 + ps;
	xr.y = prt->y + prt->h / 2 + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 2 - 2 * ps;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);
}

void svg_draw_label_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xf.size, _T("9"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xf.size, _T("8"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("Aa"), -1);
}

void svg_draw_check_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4 - ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w - ps;
	pt[3].y = prt->y + ps;

	svg_draw_polyline_raw(g, &xp, pt, 4);
}

void svg_draw_singletext_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 - 3 * ps;
	xr.w = prt->w;
	xr.h = 6 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 + 4 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_multitext_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 4;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - 1;
	pt2.y = xr.y + xr.h / 4 * 3;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_tag_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("12"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("10"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("</>"), -1);
}

void svg_draw_memo_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 4;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - 1;
	pt2.y = xr.y + xr.h / 4 * 3;
	svg_draw_line_raw(g, &xp, &pt1, &pt2);
}

void svg_draw_photo_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + ps + 1;
	xr.w = prt->w / 4 - 2;
	xr.h = prt->h / 4 - 2;

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	svg_draw_polyline_raw(g, &xp, pt, 4);
}

void svg_draw_herf_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xscpy(xp.size, _T("1"));

	xr.x = prt->x + prt->w / 4 - ps - 1;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = 2.5 * ps + 1;
	xr.h = 2.5 * ps - 1;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps - 1;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = 2.5 * ps + 1;
	xr.h = 2.5 * ps - 1;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - ps - 1;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = 2.5 * ps + 1;
	xr.h = 2.5 * ps - 1;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);
}

void svg_draw_code_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 3 - ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 3;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 3 - ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 3 * 2;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 3 - ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_table_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xscpy(xp.size, _T("1"));

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_rich_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h - 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 4;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 2;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - 1;
	pt[1].y = xr.y + xr.h / 4 * 3;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

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

	svg_draw_polyline_raw(g, &xp, pt, 5);
}

void svg_draw_grid_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_graph_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h;

	svg_draw_polyline_raw(g, &xp, pt, 3);

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

	svg_draw_curve_raw(g, &xp, pt, 5);
}

void svg_draw_images_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h - 2 * ps;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + ps + 1;
	xr.w = prt->w / 4 - 2;
	xr.h = prt->h / 4 - 2;

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	svg_draw_polyline_raw(g, &xp, pt, 4);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4;

	svg_draw_polyline_raw(g, &xp, pt, 3);
}

void svg_draw_update_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_arc_raw(g, &xp, RECTPOINT(&xr), xr.w, xr.h, XPI / 2, 2 * XPI);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4 + 1 * ps;
	pt[2].x = prt->x + prt->w / 2 + 2 * ps;
	pt[2].y = prt->y + prt->h / 4;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_fetch_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_arc_raw(g, &xp, RECTPOINT(&xr), xr.w, xr.h, XPI, - XPI / 2);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4 * 3 + 1 * ps;
	pt[2].x = prt->x + prt->w / 2 - 2 * ps;
	pt[2].y = prt->y + prt->h / 4 * 3;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_fresh_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polyline_raw(g, &xp, pt, 8);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 + 3 * ps;
	pt[2].y = prt->y + prt->h / 4;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_helpc_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("9"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("8"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("C"), 1);
}

void svg_draw_helpp_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("9"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("8"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("P"), 1);
}

void svg_draw_start_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2 - 1 * ps;
	pt[2].y = prt->y + prt->h / 2 + 1 * ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_stop_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);
}

void svg_draw_pause_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 - 1 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
	
	pt[0].x = prt->x + prt->w / 2 + 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_find_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 4 * 3;
	xr.h = prt->h / 4 * 3;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;
	
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_proper_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_arc_raw(g, &xp, RECTPOINT(&xr), xr.w, xr.h, 0, XPI);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	svg_draw_round_raw(g, &xp, NULL, &xr);
}

void svg_draw_style_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -2, -2);

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

	svg_draw_polygon_raw(g, &xp, &xb, pt, 5);

	xr.x = prt->x + prt->w / 4 - 1 * ps;
	xr.y = prt->y + prt->h / 4 * 3 - 1 * ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	lighten_xpen(&xp, -20);
	lighten_xbrush(&xb, -20);

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);
}

void svg_draw_note_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polygon_raw(g, &xp, NULL, pt, 7);
}

void svg_draw_book_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polygon_raw(g, &xp, NULL, pt, 10);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_import_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 2;
	xr.h = prt->h / 4;

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 4 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - 1 * ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[3].y = prt->y + prt->h / 2;
	
	svg_draw_polyline_raw(g, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 1 * ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4;
	
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_export_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 2;
	xr.h = prt->h / 4;

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 4 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - 1 * ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[3].y = prt->y + prt->h / 2;

	svg_draw_polyline_raw(g, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 - 1 * ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_dialog_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 + ps;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 + ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	xr.x = prt->x + prt->w - 2 * ps - 1;
	xr.y = prt->y + ps + 1;
	xr.w = ps;
	xr.h = ps;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_calendar_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_diagram_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;

	svg_draw_polyline_raw(g, &xp, pt, 3);
}

void svg_draw_static_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xf.size, _T("9"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xf.size, _T("8"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("T"), -1);
}

void svg_draw_list_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 1.5 * ps;
	xr.h = 1.5 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = 1.5 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 2;
	xr.w = 1.5 * ps;
	xr.h = 1.5 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = 1.5 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = 1.5 * ps;
	xr.h = 1.5 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 2;
	xr.h = 1.5 * ps;

	svg_draw_rect_raw(g, &xp, &xb, &xr);
}

void svg_draw_navi_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 4 - ps;
	pt[0].y = prt->y + prt->h / 2 - ps;
	pt[1].x = prt->x + prt->w / 4 - ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 4 + ps;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 - ps;
	pt[1].x = prt->x + prt->w / 4 * 3 - ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 4 * 3 + ps;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_spin_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("8"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xscpy(xp.size, _T("1"));

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2 - 1;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("2"), -1);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 - 1;
	pt[1].x = prt->x + prt->w / 4 * 3 + ps;
	pt[1].y = prt->y + prt->h / 2 - 1;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 - 2 * ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 + 1;
	pt[1].x = prt->x + prt->w / 4 * 3 + ps;
	pt[1].y = prt->y + prt->h / 2 + 1;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 + 2 * ps;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);
}

void svg_draw_slide_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[3];
	xrect_t rt;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h / 2;

	xp.adorn.feed = 2;
	xp.adorn.size = 2;
	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 + ps;
	pt[0].y = prt->y + prt->h / 2 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4 + ps;
	pt[1].y = prt->y + prt->h / 2 + 2 * ps;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_radio_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);
}

void svg_draw_date_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("8"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("23"), -1);
}

void svg_draw_time_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[3];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polyline_raw(g, &xp, pt, 3);
}

void svg_draw_push_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2 - 2 * ps;
	xr.w = prt->w / 2;
	xr.h = 4 * ps;

	xp.adorn.feed = 2;
	xp.adorn.size = 2;
	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_person_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	int ps;
	tchar_t ta[10] = { 0 };
	xpoint_t pa[10] = { 0 };

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	ps = xstol(xp.size);
	pt_expand_rect(&rt, -ps, -ps);

	prt = &rt;

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	ta[0] = _T('M');
	pa[0].x = prt->x;
	pa[0].y = prt->y + prt->h;

	ta[1] = _T('L');
	pa[1].x = prt->x;
	pa[1].y = prt->y + prt->h / 2 + 2 * ps;

	ta[2] = _T('A');
	pa[2].x = 2 * ps;
	pa[2].y = 2 * ps;
	pa[3].x = prt->x + 2 * ps;
	pa[3].y = prt->y + prt->h / 2;

	ta[3] = _T('L');
	pa[4].x = prt->x + prt->w - 2 * ps;
	pa[4].y = prt->y + prt->h / 2;

	ta[4] = _T('A');
	pa[5].x = 2 * ps;
	pa[5].y = 2 * ps;
	pa[6].x = prt->x + prt->w;
	pa[6].y = prt->y + prt->h / 2 + 2 * ps;

	ta[5] = _T('L');
	pa[7].x = prt->x + prt->w;
	pa[7].y = prt->y + prt->h;

	ta[6] = _T('L');
	pa[8].x = prt->x;
	pa[8].y = prt->y + prt->h;

	svg_draw_path_raw(g, &xp, NULL, ta, pa, 9);
}

void svg_draw_user_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[3];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 4 * ps;
	xr.h = prt->h / 4;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_arc_raw(g, &xp, RECTPOINT(&xr), xr.w, xr.h, 0, XPI);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_location_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	svg_draw_arc_raw(g, &xp, RECTPOINT(&xr),xr.w, xr.h, 0, XPI);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h - ps;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	svg_draw_bezier_raw(g, &xp, &pt[0], &pt[1], &pt[2], &pt[3]);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + 2 * ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);
}

void svg_draw_trends_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polyline_raw(g, &xp, pt, 4);
}

void svg_draw_panto_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_ellipse_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_pie_raw(g, &xp, &xb, RECTPOINT(&xr), xr.w, xr.h, 0, XPI / 2);

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	svg_draw_pie_raw(g, &xp, &xb, RECTPOINT(&xr), xr.w, xr.h, XPI, XPI / 2 * 3);
}

void svg_draw_scatter_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	xscpy(xp.size, _T("1"));

	prt = &rt;

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w / 2 - 2;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 2 + 2;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h;

	svg_draw_bezier_raw(g, &xp, &pt[0], &pt[1], &pt[2], &pt[3]);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = 2;
	xr.h = 2;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = 2;
	xr.h = 2;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = 2;
	xr.h = 2;

	svg_draw_ellipse_raw(g, &xp, &xb, &xr);
}

void svg_draw_density_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	xscpy(xp.size, _T("1"));

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);
	svg_draw_rect_raw(g, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, NULL, &xr);
}

void svg_draw_counter_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("8"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("0"), -1);
}

void svg_draw_process_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[2];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 2;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_judge_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		xscpy(xf.size, _T("10"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		xscpy(xf.size, _T("8"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		xscpy(xf.size, _T("6"));
		rt.w = 16;
		rt.h = 16;
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

	svg_draw_polygon_raw(g, &xp, NULL, pt, 4);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	svg_draw_text_raw(g, &xf, &xa, &xr, _T("?"), -1);
}

void svg_draw_lock_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h / 2;

	svg_draw_round_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4;

	svg_draw_bezier_raw(g, &xp, &pt[0], &pt[1], &pt[1], &pt[2]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_unlock_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	int ps;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	rt.x = prt->x;
	rt.y = prt->y;

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w - 2 * ps;
	xr.h = prt->h / 2;

	svg_draw_round_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4;

	svg_draw_bezier_raw(g, &xp, &pt[0], &pt[1], &pt[1], &pt[2]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;

	svg_draw_line_raw(g, &xp, &pt[0], &pt[1]);
}

void svg_draw_home_gizmo(link_t_ptr g, const xcolor_t* pxc, const xrect_t* prt)
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

	if (prt->w > 24 && prt->h > 24)
	{
		xscpy(xp.size, _T("3"));
		rt.w = 36;
		rt.h = 36;
	}
	else if (prt->w > 16 && prt->h > 16)
	{
		xscpy(xp.size, _T("2"));
		rt.w = 24;
		rt.h = 24;
	}
	else
	{
		xscpy(xp.size, _T("1"));
		rt.w = 16;
		rt.h = 16;
	}

	prt = &rt;

	ps = xstol(xp.size);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 2;

	svg_draw_polygon_raw(g, &xp, &xb, pt, 3);

	xr.x = prt->x + prt->w / 4 + ps;
	xr.y = prt->y + prt->h / 2 + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 4;

	svg_draw_rect_raw(g, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w - ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w - ps;
	pt[3].y = prt->y + prt->h / 2;

	svg_draw_polyline_raw(g, &xp, pt, 4);
}

SVG_GIZMO_DRAW_TABLE g_svg_gizmo_table[] = {
	{ GDI_ATTR_GIZMO_ALIGNCENTER, svg_draw_aligncenter_gizmo },
	{ GDI_ATTR_GIZMO_ALIGNFAR, svg_draw_alignfar_gizmo },
	{ GDI_ATTR_GIZMO_ALIGNNEAR, svg_draw_alignnear_gizmo },
	{ GDI_ATTR_GIZMO_APPEND, svg_draw_append_gizmo },
	{ GDI_ATTR_GIZMO_ARRANGECENTER, svg_draw_arrangecenter_gizmo },
	{ GDI_ATTR_GIZMO_ARRANGELEFT, svg_draw_arrangeleft_gizmo },
	{ GDI_ATTR_GIZMO_ARRANGERIGHT, svg_draw_arrangeright_gizmo },
	{ GDI_ATTR_GIZMO_BACKGROUND, svg_draw_background_gizmo },
	{ GDI_ATTR_GIZMO_BEGIN, svg_draw_begin_gizmo },
	{ GDI_ATTR_GIZMO_BOOK, svg_draw_book_gizmo },
	{ GDI_ATTR_GIZMO_BORDER, svg_draw_border_gizmo },
	{ GDI_ATTR_GIZMO_CALENDAR, svg_draw_calendar_gizmo },
	{ GDI_ATTR_GIZMO_CHECK, svg_draw_check_gizmo },
	{ GDI_ATTR_GIZMO_CHECKBOX, svg_draw_checkbox_gizmo },
	{ GDI_ATTR_GIZMO_CHECKED, svg_draw_checked_gizmo },
	{ GDI_ATTR_GIZMO_CLEAR, svg_draw_clear_gizmo },
	{ GDI_ATTR_GIZMO_CLOSE, svg_draw_close_gizmo },
	{ GDI_ATTR_GIZMO_CODE, svg_draw_code_gizmo },
	{ GDI_ATTR_GIZMO_COLLAPSE, svg_draw_collapse_gizmo },
	{ GDI_ATTR_GIZMO_COPY, svg_draw_copy_gizmo },
	{ GDI_ATTR_GIZMO_COUNTER, svg_draw_counter_gizmo },
	{ GDI_ATTR_GIZMO_CUT, svg_draw_cut_gizmo },
	{ GDI_ATTR_GIZMO_DATE, svg_draw_date_gizmo },
	{ GDI_ATTR_GIZMO_DELETE, svg_draw_delete_gizmo },
	{ GDI_ATTR_GIZMO_DENSITY, svg_draw_density_gizmo },
	{ GDI_ATTR_GIZMO_DIAGRAM, svg_draw_diagram_gizmo },
	{ GDI_ATTR_GIZMO_DIALOG, svg_draw_dialog_gizmo },
	{ GDI_ATTR_GIZMO_DOC, svg_draw_doc_gizmo },
	{ GDI_ATTR_GIZMO_DOWN, svg_draw_down_gizmo },
	{ GDI_ATTR_GIZMO_EDIT, svg_draw_edit_gizmo },
	{ GDI_ATTR_GIZMO_END, svg_draw_end_gizmo },
	{ GDI_ATTR_GIZMO_EXECUTE, svg_draw_execute_gizmo },
	{ GDI_ATTR_GIZMO_EXPAND, svg_draw_expand_gizmo },
	{ GDI_ATTR_GIZMO_EXPORT, svg_draw_export_gizmo },
	{ GDI_ATTR_GIZMO_FETCH, svg_draw_fetch_gizmo },
	{ GDI_ATTR_GIZMO_FIND, svg_draw_find_gizmo },
	{ GDI_ATTR_GIZMO_FIRST, svg_draw_first_gizmo },
	{ GDI_ATTR_GIZMO_FIXED, svg_draw_fixed_gizmo },
	{ GDI_ATTR_GIZMO_FOLDER, svg_draw_folder_gizmo },
	{ GDI_ATTR_GIZMO_FONTCOLOR, svg_draw_fontcolor_gizmo },
	{ GDI_ATTR_GIZMO_FONTNAME, svg_draw_fontname_gizmo },
	{ GDI_ATTR_GIZMO_FONTSIZE, svg_draw_fontsize_gizmo },
	{ GDI_ATTR_GIZMO_FONTSTYLE, svg_draw_fontstyle_gizmo },
	{ GDI_ATTR_GIZMO_FONTWEIGHT, svg_draw_fontweight_gizmo },
	{ GDI_ATTR_GIZMO_FOREGROUND, svg_draw_foreground_gizmo },
	{ GDI_ATTR_GIZMO_FRESH, svg_draw_fresh_gizmo },
	{ GDI_ATTR_GIZMO_GRAPH, svg_draw_graph_gizmo },
	{ GDI_ATTR_GIZMO_GRID, svg_draw_grid_gizmo },
	{ GDI_ATTR_GIZMO_GROUP, svg_draw_group_gizmo },
	{ GDI_ATTR_GIZMO_GUIDER, svg_draw_guider_gizmo },
	{ GDI_ATTR_GIZMO_HELPC, svg_draw_helpc_gizmo },
	{ GDI_ATTR_GIZMO_HELPP, svg_draw_helpp_gizmo },
	{ GDI_ATTR_GIZMO_HERF, svg_draw_herf_gizmo },
	{ GDI_ATTR_GIZMO_HOME, svg_draw_home_gizmo },
	{ GDI_ATTR_GIZMO_IMAGES, svg_draw_images_gizmo },
	{ GDI_ATTR_GIZMO_IMPORT, svg_draw_import_gizmo },
	{ GDI_ATTR_GIZMO_INPUT, svg_draw_input_gizmo },
	{ GDI_ATTR_GIZMO_INSERT, svg_draw_insert_gizmo },
	{ GDI_ATTR_GIZMO_JUDGE, svg_draw_judge_gizmo },
	{ GDI_ATTR_GIZMO_KEYBOX, svg_draw_keybox_gizmo },
	{ GDI_ATTR_GIZMO_KEYBOXED, svg_draw_keyboxed_gizmo },
	{ GDI_ATTR_GIZMO_LABEL, svg_draw_label_gizmo },
	{ GDI_ATTR_GIZMO_LAST, svg_draw_last_gizmo },
	{ GDI_ATTR_GIZMO_LIST, svg_draw_list_gizmo },
	{ GDI_ATTR_GIZMO_LOCATION, svg_draw_location_gizmo },
	{ GDI_ATTR_GIZMO_LOCK, svg_draw_lock_gizmo },
	{ GDI_ATTR_GIZMO_LOGO, svg_draw_logo_gizmo },
	{ GDI_ATTR_GIZMO_MAXIMIZE, svg_draw_maximize_gizmo },
	{ GDI_ATTR_GIZMO_MEMO, svg_draw_memo_gizmo },
	{ GDI_ATTR_GIZMO_MINIMIZE, svg_draw_minimize_gizmo },
	{ GDI_ATTR_GIZMO_MINUS, svg_draw_minus_gizmo },
	{ GDI_ATTR_GIZMO_MULTITEXT, svg_draw_multitext_gizmo },
	{ GDI_ATTR_GIZMO_NAVI, svg_draw_navi_gizmo },
	{ GDI_ATTR_GIZMO_NEW, svg_draw_new_gizmo },
	{ GDI_ATTR_GIZMO_NEXT, svg_draw_next_gizmo },
	{ GDI_ATTR_GIZMO_NOTE, svg_draw_note_gizmo },
	{ GDI_ATTR_GIZMO_NUMERIC, svg_draw_numeric_gizmo },
	{ GDI_ATTR_GIZMO_OMIT, svg_draw_omit_gizmo },
	{ GDI_ATTR_GIZMO_OPEN, svg_draw_open_gizmo },
	{ GDI_ATTR_GIZMO_ORDER, svg_draw_order_gizmo },
	{ GDI_ATTR_GIZMO_OUTPUT, svg_draw_output_gizmo },
	{ GDI_ATTR_GIZMO_PANTO, svg_draw_panto_gizmo },
	{ GDI_ATTR_GIZMO_PASTE, svg_draw_paste_gizmo },
	{ GDI_ATTR_GIZMO_PAUSE, svg_draw_pause_gizmo },
	{ GDI_ATTR_GIZMO_PERSON, svg_draw_person_gizmo },
	{ GDI_ATTR_GIZMO_PHOTO, svg_draw_photo_gizmo },
	{ GDI_ATTR_GIZMO_PLUS, svg_draw_plus_gizmo },
	{ GDI_ATTR_GIZMO_PREV, svg_draw_prev_gizmo },
	{ GDI_ATTR_GIZMO_PREVIEW, svg_draw_preview_gizmo },
	{ GDI_ATTR_GIZMO_PRINT, svg_draw_print_gizmo },
	{ GDI_ATTR_GIZMO_PROCESS, svg_draw_process_gizmo },
	{ GDI_ATTR_GIZMO_PROPER, svg_draw_proper_gizmo },
	{ GDI_ATTR_GIZMO_PUSH, svg_draw_push_gizmo },
	{ GDI_ATTR_GIZMO_RADIO, svg_draw_radio_gizmo },
	{ GDI_ATTR_GIZMO_RADIOBOX, svg_draw_radiobox_gizmo },
	{ GDI_ATTR_GIZMO_RADIOED, svg_draw_radioed_gizmo },
	{ GDI_ATTR_GIZMO_REDO, svg_draw_redo_gizmo },
	{ GDI_ATTR_GIZMO_REMOVE, svg_draw_remove_gizmo },
	{ GDI_ATTR_GIZMO_RESTORE, svg_draw_restore_gizmo },
	{ GDI_ATTR_GIZMO_RICH, svg_draw_rich_gizmo },
	{ GDI_ATTR_GIZMO_SAVE, svg_draw_save_gizmo },
	{ GDI_ATTR_GIZMO_SAVEAS, svg_draw_saveas_gizmo },
	{ GDI_ATTR_GIZMO_SCATTER, svg_draw_scatter_gizmo },
	{ GDI_ATTR_GIZMO_SCHEMA, svg_draw_schema_gizmo },
	{ GDI_ATTR_GIZMO_SCREEN, svg_draw_screen_gizmo },
	{ GDI_ATTR_GIZMO_SELECTALL, svg_draw_selectall_gizmo },
	{ GDI_ATTR_GIZMO_SELECTED, svg_draw_selected_gizmo },
	{ GDI_ATTR_GIZMO_SHAPE, svg_draw_shape_gizmo },
	{ GDI_ATTR_GIZMO_SINGLETEXT, svg_draw_singletext_gizmo },
	{ GDI_ATTR_GIZMO_SIZEHORZ, svg_draw_sizehorz_gizmo },
	{ GDI_ATTR_GIZMO_SIZEVERT, svg_draw_sizevert_gizmo },
	{ GDI_ATTR_GIZMO_SLIDE, svg_draw_slide_gizmo },
	{ GDI_ATTR_GIZMO_SPACEHORZ, svg_draw_spacehorz_gizmo },
	{ GDI_ATTR_GIZMO_SPACEVERT, svg_draw_spacevert_gizmo },
	{ GDI_ATTR_GIZMO_SPIN, svg_draw_spin_gizmo },
	{ GDI_ATTR_GIZMO_START, svg_draw_start_gizmo },
	{ GDI_ATTR_GIZMO_STATIC, svg_draw_static_gizmo },
	{ GDI_ATTR_GIZMO_STOP, svg_draw_stop_gizmo },
	{ GDI_ATTR_GIZMO_STYLE, svg_draw_style_gizmo },
	{ GDI_ATTR_GIZMO_SUM, svg_draw_sum_gizmo },
	{ GDI_ATTR_GIZMO_TABLE, svg_draw_table_gizmo },
	{ GDI_ATTR_GIZMO_TAG, svg_draw_tag_gizmo },
	{ GDI_ATTR_GIZMO_TIME, svg_draw_time_gizmo },
	{ GDI_ATTR_GIZMO_TOUCH, svg_draw_touch_gizmo },
	{ GDI_ATTR_GIZMO_TOUCHED, svg_draw_touched_gizmo },
	{ GDI_ATTR_GIZMO_TRENDS, svg_draw_trends_gizmo },
	{ GDI_ATTR_GIZMO_UNDO, svg_draw_undo_gizmo },
	{ GDI_ATTR_GIZMO_UNLOCK, svg_draw_unlock_gizmo },
	{ GDI_ATTR_GIZMO_UP, svg_draw_up_gizmo },
	{ GDI_ATTR_GIZMO_UPDATE, svg_draw_update_gizmo },
	{ GDI_ATTR_GIZMO_USER, svg_draw_user_gizmo },
	{ GDI_ATTR_GIZMO_ZOOMIN, svg_draw_zoomin_gizmo },
	{ GDI_ATTR_GIZMO_ZOOMOUT, svg_draw_zoomout_gizmo },
};

static PF_SVG_GIZMO_MAKE _find_gizmo_maker(int i, int j, const tchar_t* iname)
{
	int rt, k;

	if (i == j)
	{
		rt = compare_text(iname, -1, g_svg_gizmo_table[i].gizmo_name, -1, 1);
		if (!rt)
			return g_svg_gizmo_table[i].gizmo_func;
		else
			return NULL;
	}
	else if (i < j)
	{
		k = (i + j) / 2;
		rt = compare_text(iname, -1, g_svg_gizmo_table[k].gizmo_name, -1, 1);
		if (!rt)
			return g_svg_gizmo_table[k].gizmo_func;
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

PF_SVG_GIZMO_MAKE svg_find_gizmo_maker(const tchar_t* iname)
{
	int k;

	if (is_null(iname))
		return NULL;

	k = sizeof(g_svg_gizmo_table) / sizeof(SVG_GIZMO_DRAW_TABLE);

	return _find_gizmo_maker(0, k - 1, iname);
}


#endif


