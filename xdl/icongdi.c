/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc icon document

	@module	icongdi.c | implement file

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

#include "icongdi.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#if defined(XDK_SUPPORT_CONTEXT) && defined(XDK_SUPPORT_CONTEXT_GRAPHIC)

void draw_logo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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
	draw_round_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 + 1;
	xr.y = prt->y;
	xr.w = prt->w / 2 - 2;
	xr.h = prt->h / 2 - 2;
	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 + 1;
	xr.w = prt->w / 2 - 2;
	xr.h = prt->h / 2 - 2;
	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 + 1;
	xr.y = prt->y + prt->h / 2 + 1;
	xr.w = prt->w / 2 - 2;
	xr.h = prt->h / 2 - 2;
	draw_round_raw(rdc, &xp, &xb, &xr);
}

void draw_plus_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_line_raw(rdc, &xp, &(pt[0]), &(pt[1]));

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, &xp, &(pt[0]), &(pt[1]));
}

void draw_minus_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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
	draw_line_raw(rdc, &xp, &(pt[0]), &(pt[1]));
}

void draw_collapse_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_expand_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_begin_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_up_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_down_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_end_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_first_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_prev_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_next_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_last_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_zoomin_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;

	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = prt->x + prt->w / 3 * 2;
	pt1.y = prt->y + prt->h / 3 * 2;
	pt2.x = prt->x + prt->w;
	pt2.y = prt->y + prt->h;

	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_zoomout_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;

	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + ps + 1;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h - ps - 1;

	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = prt->x + prt->w / 3 * 2;
	pt1.y = prt->y + prt->h / 3 * 2;
	pt2.x = prt->x + prt->w;
	pt2.y = prt->y + prt->h;

	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_keybox_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	draw_round_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w - 2 * ps;
	xr.h = ps * 2;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_keyboxed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	pt1.x = xr.x;
	pt1.y = xr.y + 2;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + 2;

	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x;
	pt1.y = xr.y + 4;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + 4;

	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + 3;
	pt1.y = xr.y + 4;
	pt2.x = xr.x + 3;
	pt2.y = xr.y + xr.h;

	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w - 3;
	pt1.y = xr.y + 4;
	pt2.x = xr.x + xr.w - 3;
	pt2.y = xr.y + xr.h;

	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_touch_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	pt1.x = xr.x + xr.w / 2 - 3;
	pt1.y = xr.y + xr.h / 2 - 3;
	pt2.x = xr.x + xr.w / 2 + 3;
	pt2.y = xr.y + xr.h / 2 + 3;

	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2 - 3;
	pt1.y = xr.y + xr.h / 2 + 3; 
	pt2.x = xr.x + xr.w / 2 + 3;
	pt2.y = xr.y + xr.h / 2 - 3;

	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_touched_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	lighten_xpen(&xp, -20);

	xr.x = prt->x - 1;
	xr.y = prt->y - 1;
	xr.w = prt->w + 2;
	xr.h = prt->h + 2;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	pt_expand_rect(&xr, -2, -2);

	lighten_xbrush(&xb, 20);

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_close_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	long ps;

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

	pt1.x = xr.x + ps;
	pt1.y = xr.y + ps;
	pt2.x = xr.x + xr.w - ps;
	pt2.y = xr.y + xr.h - ps;

	xp.adorn.feed = 1;
	xp.adorn.size = 1;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps;
	pt1.y = xr.y + xr.h - ps;
	pt2.x = xr.x + xr.w - ps;
	pt2.y = xr.y + ps;

	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_minimize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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
	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_maximize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + 2;
	xr.w = prt->w - 2;
	xr.y = prt->y + 2;
	xr.h = prt->h - 2;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_restore_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_sum_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5] = { 0 };
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 5);
}

void draw_checkbox_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_checked_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;

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

	prt = &rt;

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	//draw_rect_raw(rdc, &xp, &xb, &xr);

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + 2;
	pt2.y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + 2;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h - 1;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h - 1;
	pt2.x = xr.x + xr.w + 2;
	pt2.y = xr.y + 1;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_radiobox_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);
}

void draw_radioed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_selected_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + 2;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_folder_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 4);

	xr.y += xr.h / 4;
	xr.h -= xr.h / 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_guider_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_polygon_raw(rdc, &xp, &xb, pt, 5);
}

void draw_fixed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt1, pt2;
	long ps;

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
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = prt->x + 4 * ps;
	pt1.y = prt->y + prt->h / 4;
	pt2.x = prt->x + 4 * ps;
	pt2.y = prt->y + prt->h / 4 * 3;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
	
	xr.x = prt->x + 4 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w - 6 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_numeric_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt;
	int fs;

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

	draw_ellipse_raw(rdc, &xp, &xb, prt);

	fs = font_size(rdc, prt->h) - 2;
	if (fs < 8)
		fs = 8;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ltoxs(fs, xf.size, INT_LEN);
	xscpy(xf.color, GDI_ATTR_RGB_WHITE);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	//draw_text_raw(rdc, &xf, &xa, prt, no, -1);
}

void draw_doc_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 4;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - 1;
	pt2.y = xr.y + xr.h / 4 * 3;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_new_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt, xr;
	long ps;

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

	draw_polygon_raw(rdc, &xp, NULL, pt, 5);

	pt[0].x = xr.x + xr.w / 4 * 3;
	pt[0].y = xr.y + xr.h - 1;
	pt[1].x = xr.x + xr.w / 4 * 3;
	pt[1].y = xr.y + xr.h / 4 * 3;
	pt[2].x = xr.x + xr.w - 1;
	pt[2].y = xr.y + xr.h / 4 * 3;

	draw_polyline_raw(rdc, &xp, pt, 3);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 4;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - 1;
	pt[1].y = xr.y + xr.h / 4 * 3;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_open_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h;
	pt[1].x = xr.x + 3 * ps;
	pt[1].y = xr.y + xr.h / 2;
	pt[2].x = xr.x + xr.w + 3 * ps;
	pt[2].y = xr.y + xr.h / 2;
	pt[3].x = xr.x + xr.w;
	pt[3].y = xr.y + xr.h;

	format_xcolor(pxc, xb.color);
	draw_polygon_raw(rdc, &xp, &xb, pt, 4);
}

void draw_save_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[8];
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

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
	draw_polygon_raw(rdc, &xp, &xb, pt, 8);
}

void draw_saveas_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[9];
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w;
	pt[3].y = prt->y + prt->h / 4 * 3;
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
	draw_polygon_raw(rdc, &xp, &xb, pt, 9);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4 * 3;

	ltoxs(ps, xp.size, INT_LEN);
	draw_polyline_raw(rdc, &xp, pt, 3);
}

void draw_schema_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w / 2 - ps - 1;
	pt[1].y = xr.y + xr.h / 4;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w / 2 + ps;
	pt[1].y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - ps - 1;
	pt[1].y = xr.y + xr.h / 4 * 3;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + xr.w / 2 + ps;
	pt[0].y = xr.y + xr.h / 2 - ps;
	pt[1].x = xr.x + xr.w / 4 * 3;
	pt[1].y = xr.y + xr.h / 2;
	pt[2].x = xr.x + xr.w / 2 + ps;
	pt[2].y = xr.y + xr.h / 2 + ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_output_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt;
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;
	
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_input_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[7];
	xrect_t rt;
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 7);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 * 3;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_print_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, NULL, pt, 4);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w;
	xr.h = prt->h / 2 + ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - ps;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 4 * 3 - ps;
	pt[2].y = prt->y + prt->h - ps;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 2;

	draw_polygon_raw(rdc, &xp, NULL, pt, 4);
}

void draw_preview_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_round_raw(rdc, &xp, &xb, &xr);
}

void draw_screen_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_round_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = ps;
	xr.h = ps;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	draw_round_raw(rdc, &xp, &xb, &xr);

	lighten_xbrush(&xb, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_execute_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[4];
	xrect_t rt;
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 + 3 * ps;
	pt[2].y = prt->y + prt->h / 4;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2 + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4 * 3;

	draw_polyline_raw(rdc, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 * 3 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 * 3 - 3 * ps;
	pt[2].y = prt->y + prt->h / 4 * 3;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_selectall_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[4];
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 4;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	pt[0].x = prt->x + prt->w / 2 + ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h - ps - 1;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 * 3 - 2 * ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_delete_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	long ps;

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

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h - ps;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_insert_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 4;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - 1 * ps;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 2;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + 2;
	pt[2].x = prt->x + prt->w - 2 * ps - 4;
	pt[2].y = prt->y + prt->h / 2;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + 2 * ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w - 2 * ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_remove_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w /2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 4 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 1 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_clear_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 4 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 - 1 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h - 4 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h - 1 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h - 4 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_append_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w - ps;
	pt[1].y = prt->y + prt->h / 4;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 4 * 3 + ps;
	pt[1].x = prt->x + prt->w / 2 - ps;
	pt[1].y = prt->y + prt->h / 4 * 3 + ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 2 + 3 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_copy_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

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

	draw_polyline_raw(rdc, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 + 1;
	pt[1].x = prt->x + prt->w - 1;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;

	draw_polygon_raw(rdc, &xp, NULL, pt, 3);
}

void draw_cut_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt;
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 5);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 2;

	draw_polygon_raw(rdc, &xp, NULL, pt, 3);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);
	xscpy(xp.size, _T("1"));
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_paste_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_undo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h / 2;

	draw_polyline_raw(rdc, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 - 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 + 2 * ps;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 4 + 3 * ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_redo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	draw_polyline_raw(rdc, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 4;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4 + 3 * ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_fontname_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_text_raw(rdc, &xf, &xa, &xr, _T("A"), 1);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	draw_text_raw(rdc, &xf, &xa, &xr, _T("A"), 1);
}

void draw_fontsize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[3];
	long ps;

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

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + ps;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;;
	pt[1].y = prt->y + 2 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	draw_text_raw(rdc, &xf, &xa, &xr, _T("A"), 1);
}

void draw_fontweight_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	long ps;

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

	draw_text_raw(rdc, &xf, &xa, &xr, _T("B"), 1);
}

void draw_fontcolor_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_text_raw(rdc, &xf, &xa, &xr, _T("A"), 1);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 * 3 + ps;
	xr.w = prt->w;
	xr.h = prt->h / 4 - ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_fontstyle_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.style, GDI_ATTR_FONT_STYLE_UNDERLINE);

	draw_text_raw(rdc, &xf, &xa, &xr, _T("U"), 1);
}

void draw_foreground_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4 * 3 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + prt->w / 4 * 3 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	draw_polygon_raw(rdc, &xp, NULL, pt, 3);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_background_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 2 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + prt->w / 2 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	draw_polygon_raw(rdc, &xp, NULL, pt, 3);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_alignnear_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	long ps;

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

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_aligncenter_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	long ps;

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

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_alignfar_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[2];
	xrect_t rt;
	long ps;

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

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2 - ps;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_arrangeleft_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	ltoxs(ps - 1, xp.size, INT_LEN);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 2;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w / 4;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_arrangecenter_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	ltoxs(ps - 1, xp.size, INT_LEN);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 2;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - 2 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 4 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + 2 * ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w - 4 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_arrangeright_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	ltoxs(ps - 1, xp.size, INT_LEN);
	if (!ps)
		ps = 1;

	xr.x = prt->x + prt->w - 6 * ps - 1;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 5 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w - 8 * ps - 1;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 7 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w - 4 * ps - 1;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = 3 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_sizehorz_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	ltoxs(ps - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[2].x = prt->x + prt->w / 4;
	pt[2].y = prt->y + prt->h / 2 + prt->h / 8;

	draw_polyline_raw(rdc, &xp, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2 + prt->h / 8;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4;

	draw_polyline_raw(rdc, &xp, pt, 3);
}

void draw_sizevert_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 4;
	xr.h = prt->h / 2;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	ltoxs(ps - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 8;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y;

	draw_polyline_raw(rdc, &xp, pt, 3);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 8;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h;

	draw_polyline_raw(rdc, &xp, pt, 3);
}

void draw_spacehorz_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 2 * ps;
	xr.h = prt->h / 4 * 3;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w - 2 * ps;
	xr.y = prt->y + prt->h / 4;
	xr.w = 2 * ps;
	xr.h = prt->h / 4 * 3;

	draw_rect_raw(rdc, &xp, &xb, &xr);

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

	draw_polyline_raw(rdc, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 + ps;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2 + ps;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w - 2 * ps;
	pt[2].y = prt->y;
	pt[3].x = prt->x + prt->w - 2 * ps;
	pt[3].y = prt->y + prt->h / 4;

	draw_polyline_raw(rdc, &xp, pt, 4);
}

void draw_spacevert_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h - 2 * ps;
	xr.w = prt->w / 4 * 3;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);

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

	draw_polyline_raw(rdc, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h - 2 * ps;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h - 2 * ps;

	draw_polyline_raw(rdc, &xp, pt, 4);
}

void draw_edit_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 2 - 2 * ps;
	pt[0].y = prt->y + prt->h / 2 + 1 * ps + 1;
	pt[1].x = prt->x + prt->w / 2 + 2 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps + 1;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 4 * ps;

	draw_polygon_raw(rdc, &xp, NULL, pt, 3);
}

void draw_group_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 + 1.5 * ps;
	xr.y = prt->y + prt->h / 4 + 1.5 * ps;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	ltoxs(ps - 1, xp.size, INT_LEN);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_order_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	lighten_xpen(&xp, DEF_HARD_DARKEN);
	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - 1.5 * ps;
	xr.y = prt->y + prt->h / 4 * 3 - 1.5 * ps;
	xr.w = 3 * ps;
	xr.h = 3 * ps;

	lighten_xpen(&xp, 0 - DEF_HARD_DARKEN);
	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_border_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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
	draw_rect_raw(rdc, &xp, NULL, &xr);

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_shape_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 + ps;
	xr.y = prt->y + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 2 - 2 * ps;

	draw_round_raw(rdc, &xp, &xb, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2 + ps;
	pt[1].x = prt->x + ps;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 2 - ps;
	pt[2].y = prt->y + prt->h - ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	xr.x = prt->x + prt->w / 2 + ps;
	xr.y = prt->y + prt->h / 2 + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 2 - 2 * ps;

	draw_ellipse_raw(rdc, &xp, NULL, &xr);
}

void draw_label_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_text_raw(rdc, &xf, &xa, &xr, _T("Aa"), -1);
}

void draw_check_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4 - ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w - ps;
	pt[3].y = prt->y + ps;

	draw_polyline_raw(rdc, &xp, pt, 4);
}

void draw_singletext_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 + 4 * ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_multitext_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 4;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - 1;
	pt2.y = xr.y + xr.h / 4 * 3;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_tag_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	draw_text_raw(rdc, &xf, &xa, &xr, _T("</>"), -1);
}

void draw_memo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 4;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - ps - 1;
	pt2.y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + ps + 1;
	pt1.y = xr.y + xr.h / 4 * 3;
	pt2.x = xr.x + xr.w / 2 - 1;
	pt2.y = xr.y + xr.h / 4 * 3;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_photo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + ps + 1;
	xr.w = prt->w / 4 - 2;
	xr.h = prt->h / 4 - 2;

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	draw_polyline_raw(rdc, &xp, pt, 4);
}

void draw_herf_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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
	xr.w = prt->w;
	xr.h = prt->h / 2;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xscpy(xp.size, _T("1"));

	xr.x = prt->x + prt->w / 4 - ps - 1;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = 2.5 * ps + 1;
	xr.h = 2.5 * ps - 1;

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps - 1;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = 2.5 * ps + 1;
	xr.h = 2.5 * ps - 1;

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - ps - 1;
	xr.y = prt->y + prt->h / 2 - 1 * ps;
	xr.w = 2.5 * ps + 1;
	xr.h = 2.5 * ps - 1;

	draw_ellipse_raw(rdc, &xp, NULL, &xr);
}

void draw_table_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xscpy(xp.size, _T("1"));

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 4 * 3 - ps;
	xr.w = prt->w / 2 - ps;
	xr.h = 2 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_rich_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 4;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 2;
	pt[1].x = xr.x + xr.w - ps - 1;
	pt[1].y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = xr.x + ps + 1;
	pt[0].y = xr.y + xr.h / 4 * 3;
	pt[1].x = xr.x + xr.w / 2 - 1;
	pt[1].y = xr.y + xr.h / 4 * 3;
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

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

	draw_polyline_raw(rdc, &xp, pt, 5);
}

void draw_grid_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_graph_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[5];
	xrect_t rt;
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 3);

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

	draw_curve_raw(rdc, &xp, pt, 5);
}

void draw_images_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[5];
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + ps + 1;
	xr.y = prt->y + ps + 1;
	xr.w = prt->w / 4 - 2;
	xr.h = prt->h / 4 - 2;

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 8;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	draw_polyline_raw(rdc, &xp, pt, 4);

	lighten_xpen(&xp, DEF_SOFT_DARKEN);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h / 4;

	draw_polyline_raw(rdc, &xp, pt, 3);
}

void draw_update_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_arc_raw(rdc, &xp, RECTPOINT(&xr), xr.w, xr.h, XPI / 2, 2 * XPI);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4 + 1 * ps;
	pt[2].x = prt->x + prt->w / 2 + 2 * ps;
	pt[2].y = prt->y + prt->h / 4;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_fetch_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_arc_raw(rdc, &xp, RECTPOINT(&xr), xr.w, xr.h, XPI, - XPI / 2);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4 * 3 + 1 * ps;
	pt[2].x = prt->x + prt->w / 2 - 2 * ps;
	pt[2].y = prt->y + prt->h / 4 * 3;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_fresh_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[8];
	xrect_t rt;
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 8);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 + 2 * ps;
	pt[2].x = prt->x + prt->w / 4 + 3 * ps;
	pt[2].y = prt->y + prt->h / 4;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_helpc_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	draw_text_raw(rdc, &xf, &xa, &xr, _T("C"), 1);
}

void draw_helpp_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xf.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	draw_text_raw(rdc, &xf, &xa, &xr, _T("P"), 1);
}

void draw_start_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2 - 1 * ps;
	pt[2].y = prt->y + prt->h / 2 + 1 * ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_stop_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_pause_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 - 1 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
	
	pt[0].x = prt->x + prt->w / 2 + 1 * ps;
	pt[0].y = prt->y + prt->h / 2 - 1 * ps;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2 + 1 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_find_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;
	
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_proper_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_arc_raw(rdc, &xp, RECTPOINT(&xr), xr.w, xr.h, 0, XPI);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	draw_round_raw(rdc, &xp, NULL, &xr);
}

void draw_style_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w / 4;
	pt[3].y = prt->y + prt->h;
	pt[4].x = prt->x + prt->w;
	pt[4].y = prt->y + prt->h / 4;

	draw_polygon_raw(rdc, &xp, &xb, pt, 5);

	xr.x = prt->x + prt->w / 4 - 1 * ps;
	xr.y = prt->y + prt->h / 4 * 3 - 1 * ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	lighten_xpen(&xp, -20);
	lighten_xbrush(&xb, -20);

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_note_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[7];
	xrect_t rt;
	long ps;

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

	draw_polygon_raw(rdc, &xp, NULL, pt, 7);
}

void draw_book_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[10];
	xrect_t rt;
	long ps;

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

	draw_polygon_raw(rdc, &xp, NULL, pt, 10);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + 1 * ps;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_import_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 4 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - 1 * ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[3].y = prt->y + prt->h / 2;
	
	draw_polyline_raw(rdc, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 + 1 * ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 4;
	
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_export_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[10];
	long ps;

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

	draw_rect_raw(rdc, &xp, &xb, &xr);

	lighten_xpen(&xp, DEF_HARD_LIGHTEN);

	pt[0].x = prt->x + prt->w / 4 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4 - 1 * ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w / 4 * 3 + 1 * ps;
	pt[3].y = prt->y + prt->h / 2;

	draw_polyline_raw(rdc, &xp, pt, 4);

	pt[0].x = prt->x + prt->w / 2 - 1 * ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 2 + 1 * ps;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 2 - 1 * ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4 * 3;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_dialog_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[10];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h / 4 + ps;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h / 4 + ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	xr.x = prt->x + prt->w - 2 * ps - 1;
	xr.y = prt->y + ps + 1;
	xr.w = ps;
	xr.h = ps;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_calendar_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 4;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_diagram_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[5];
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h / 4 * 3;

	draw_polyline_raw(rdc, &xp, pt, 3);
}

void draw_static_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_text_raw(rdc, &xf, &xa, &xr, _T("T"), -1);
}

void draw_list_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w / 2;
	xr.h = 1.5 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 2;
	xr.w = 1.5 * ps;
	xr.h = 1.5 * ps;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = 1.5 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = 1.5 * ps;
	xr.h = 1.5 * ps;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = prt->w / 2;
	xr.h = 1.5 * ps;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_navi_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];
	xrect_t rt;
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 - ps;
	pt[1].x = prt->x + prt->w / 4 * 3 - ps;
	pt[1].y = prt->y + prt->h / 2 + ps;
	pt[2].x = prt->x + prt->w / 4 * 3 + ps;
	pt[2].y = prt->y + prt->h / 2;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_spin_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[3];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	draw_text_raw(rdc, &xf, &xa, &xr, _T("2"), -1);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 - 1;
	pt[1].x = prt->x + prt->w / 4 * 3 + ps;
	pt[1].y = prt->y + prt->h / 2 - 1;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 - 2 * ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	pt[0].x = prt->x + prt->w / 4 * 3 - ps;
	pt[0].y = prt->y + prt->h / 2 + 1;
	pt[1].x = prt->x + prt->w / 4 * 3 + ps;
	pt[1].y = prt->y + prt->h / 2 + 1;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2 + 2 * ps;

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_slide_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt[3];
	xrect_t rt;
	long ps;

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
	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 + ps;
	pt[0].y = prt->y + prt->h / 2 - 2 * ps;
	pt[1].x = prt->x + prt->w / 4 + ps;
	pt[1].y = prt->y + prt->h / 2 + 2 * ps;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_radio_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + prt->h / 2 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_date_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 4 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3 - ps;
	xr.y = prt->y + prt->h / 4 - ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 4;
	xr.w = prt->w;
	xr.h = prt->h / 4 * 3;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	draw_text_raw(rdc, &xf, &xa, &xr, _T("23"), -1);
}

void draw_time_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[3];
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 2;

	draw_polyline_raw(rdc, &xp, pt, 3);
}

void draw_push_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	long ps;

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
	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_user_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[3];
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	draw_arc_raw(rdc, &xp, RECTPOINT(&xr), xr.w, xr.h, 0, XPI);

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_location_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_arc_raw(rdc, &xp, RECTPOINT(&xr),xr.w, xr.h, 0, XPI);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y + prt->h - ps;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h - ps;
	pt[3].x = prt->x + prt->w / 4 * 3;
	pt[3].y = prt->y + prt->h / 4;

	draw_bezier_raw(rdc, &xp, &pt[0], &pt[1], &pt[2], &pt[3]);

	xr.x = prt->x + prt->w / 2 - ps;
	xr.y = prt->y + 2 * ps;
	xr.w = 2 * ps;
	xr.h = 2 * ps;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_trends_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[4];
	xrect_t rt;
	long ps;

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

	draw_polyline_raw(rdc, &xp, pt, 4);
}

void draw_panto_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	long ps;

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	draw_pie_raw(rdc, &xp, &xb, RECTPOINT(&xr), xr.w, xr.h, 0, XPI / 2);

	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	draw_pie_raw(rdc, &xp, &xb, RECTPOINT(&xr), xr.w, xr.h, XPI, XPI / 2 * 3);
}

void draw_scatter_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_bezier_raw(rdc, &xp, &pt[0], &pt[1], &pt[2], &pt[3]);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = 2;
	xr.h = 2;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = 2;
	xr.h = 2;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 4 * 3;
	xr.y = prt->y + prt->h / 4 * 3;
	xr.w = 2;
	xr.h = 2;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_density_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	lighten_xbrush(&xb, DEF_HARD_DARKEN);
	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2;
	xr.y = prt->y + prt->h / 2;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_counter_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	draw_text_raw(rdc, &xf, &xa, &xr, _T("0"), -1);
}

void draw_process_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[2];
	long ps;

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

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 4 * 3;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_judge_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, NULL, pt, 4);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	draw_text_raw(rdc, &xf, &xa, &xr, _T("?"), -1);
}

void draw_lock_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_round_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4;

	draw_bezier_raw(rdc, &xp, &pt[0], &pt[1], &pt[1], &pt[2]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w / 4 * 3;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4 * 3;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_unlock_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_round_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 2;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 4 * 3;
	pt[2].y = prt->y + prt->h / 4;

	draw_bezier_raw(rdc, &xp, &pt[0], &pt[1], &pt[1], &pt[2]);

	pt[0].x = prt->x + prt->w / 4;
	pt[0].y = prt->y + prt->h / 4;
	pt[1].x = prt->x + prt->w / 4;
	pt[1].y = prt->y + prt->h / 2;

	draw_line_raw(rdc, &xp, &pt[0], &pt[1]);
}

void draw_home_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t rt, xr;
	xpoint_t pt[4];
	long ps;

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

	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	xr.x = prt->x + prt->w / 4 + ps;
	xr.y = prt->y + prt->h / 2 + ps;
	xr.w = prt->w / 2 - 2 * ps;
	xr.h = prt->h / 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);

	pt[0].x = prt->x + ps;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + ps;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w - ps;
	pt[2].y = prt->y + prt->h;
	pt[3].x = prt->x + prt->w - ps;
	pt[3].y = prt->y + prt->h / 2;

	draw_polyline_raw(rdc, &xp, pt, 4);
}

ICON_DRAW_TABLE g_icon_table[] = {
	{ ICON_ALIGNCENTER, draw_aligncenter_icon_raw },
	{ ICON_ALIGNFAR, draw_alignfar_icon_raw },
	{ ICON_ALIGNNEAR, draw_alignnear_icon_raw },
	{ ICON_APPEND, draw_append_icon_raw },
	{ ICON_ARRANGECENTER, draw_arrangecenter_icon_raw },
	{ ICON_ARRANGELEFT, draw_arrangeleft_icon_raw },
	{ ICON_ARRANGERIGHT, draw_arrangeright_icon_raw },
	{ ICON_BACKGROUND, draw_background_icon_raw },
	{ ICON_BEGIN, draw_begin_icon_raw },
	{ ICON_BOOK, draw_book_icon_raw },
	{ ICON_BORDER, draw_border_icon_raw },
	{ ICON_CALENDAR, draw_calendar_icon_raw },
	{ ICON_CHECK, draw_check_icon_raw },
	{ ICON_CHECKBOX, draw_checkbox_icon_raw },
	{ ICON_CHECKED, draw_checked_icon_raw },
	{ ICON_CLEAR, draw_clear_icon_raw },
	{ ICON_CLOSE, draw_close_icon_raw },
	{ ICON_COLLAPSE, draw_collapse_icon_raw },
	{ ICON_COPY, draw_copy_icon_raw },
	{ ICON_COUNTER, draw_counter_icon_raw },
	{ ICON_CUT, draw_cut_icon_raw },
	{ ICON_DATE, draw_date_icon_raw },
	{ ICON_DELETE, draw_delete_icon_raw },
	{ ICON_DENSITY, draw_density_icon_raw },
	{ ICON_DIAGRAM, draw_diagram_icon_raw },
	{ ICON_DIALOG, draw_dialog_icon_raw },
	{ ICON_DOC, draw_doc_icon_raw },
	{ ICON_DOWN, draw_down_icon_raw },
	{ ICON_EDIT, draw_edit_icon_raw },
	{ ICON_END, draw_end_icon_raw },
	{ ICON_EXECUTE, draw_execute_icon_raw },
	{ ICON_EXPAND, draw_expand_icon_raw },
	{ ICON_EXPORT, draw_export_icon_raw },
	{ ICON_FETCH, draw_fetch_icon_raw },
	{ ICON_FIND, draw_find_icon_raw },
	{ ICON_FIRST, draw_first_icon_raw },
	{ ICON_FIXED, draw_fixed_icon_raw },
	{ ICON_FOLDER, draw_folder_icon_raw },
	{ ICON_FONTCOLOR, draw_fontcolor_icon_raw },
	{ ICON_FONTNAME, draw_fontname_icon_raw },
	{ ICON_FONTSIZE, draw_fontsize_icon_raw },
	{ ICON_FONTSTYLE, draw_fontstyle_icon_raw },
	{ ICON_FONTWEIGHT, draw_fontweight_icon_raw },
	{ ICON_FOREGROUND, draw_foreground_icon_raw },
	{ ICON_FRESH, draw_fresh_icon_raw },
	{ ICON_GRAPH, draw_graph_icon_raw },
	{ ICON_GRID, draw_grid_icon_raw },
	{ ICON_GROUP, draw_group_icon_raw },
	{ ICON_GUIDER, draw_guider_icon_raw },
	{ ICON_HELPC, draw_helpc_icon_raw },
	{ ICON_HELPP, draw_helpp_icon_raw },
	{ ICON_HERF, draw_herf_icon_raw },
	{ ICON_HOME, draw_home_icon_raw },
	{ ICON_IMAGES, draw_images_icon_raw },
	{ ICON_IMPORT, draw_import_icon_raw },
	{ ICON_INPUT, draw_input_icon_raw },
	{ ICON_INSERT, draw_insert_icon_raw },
	{ ICON_JUDGE, draw_judge_icon_raw },
	{ ICON_KEYBOX, draw_keybox_icon_raw },
	{ ICON_KEYBOXED, draw_keyboxed_icon_raw },
	{ ICON_LABEL, draw_label_icon_raw },
	{ ICON_LAST, draw_last_icon_raw },
	{ ICON_LIST, draw_list_icon_raw },
	{ ICON_LOCATION, draw_location_icon_raw },
	{ ICON_LOCK, draw_lock_icon_raw },
	{ ICON_LOGO, draw_logo_icon_raw },
	{ ICON_MAXIMIZE, draw_maximize_icon_raw },
	{ ICON_MEMO, draw_memo_icon_raw },
	{ ICON_MINIMIZE, draw_minimize_icon_raw },
	{ ICON_MINUS, draw_minus_icon_raw },
	{ ICON_MULTITEXT, draw_multitext_icon_raw },
	{ ICON_NAVI, draw_navi_icon_raw },
	{ ICON_NEW, draw_new_raw },
	{ ICON_NEXT, draw_next_icon_raw },
	{ ICON_NOTE, draw_note_icon_raw },
	{ ICON_NUMERIC, draw_numeric_icon_raw },
	{ ICON_OPEN, draw_open_icon_raw },
	{ ICON_ORDER, draw_order_icon_raw },
	{ ICON_OUTPUT, draw_output_icon_raw },
	{ ICON_PANTO, draw_panto_icon_raw },
	{ ICON_PASTE, draw_paste_icon_raw },
	{ ICON_PAUSE, draw_pause_icon_raw },
	{ ICON_PHOTO, draw_photo_icon_raw },
	{ ICON_PLUS, draw_plus_icon_raw },
	{ ICON_PREV, draw_prev_icon_raw },
	{ ICON_PREVIEW, draw_preview_icon_raw },
	{ ICON_PRINT, draw_print_icon_raw },
	{ ICON_PROCESS, draw_process_icon_raw },
	{ ICON_PROPER, draw_proper_icon_raw },
	{ ICON_PUSH, draw_push_icon_raw },
	{ ICON_RADIO, draw_radio_icon_raw },
	{ ICON_RADIOBOX, draw_radiobox_icon_raw },
	{ ICON_RADIOED, draw_radioed_icon_raw },
	{ ICON_REDO, draw_redo_icon_raw },
	{ ICON_REMOVE, draw_remove_icon_raw },
	{ ICON_RESTORE, draw_restore_icon_raw },
	{ ICON_RICH, draw_rich_icon_raw },
	{ ICON_SAVE, draw_save_icon_raw },
	{ ICON_SAVEAS, draw_saveas_icon_raw },
	{ ICON_SCATTER, draw_scatter_icon_raw },
	{ ICON_SCHEMA, draw_schema_icon_raw },
	{ ICON_SCREEN, draw_screen_icon_raw },
	{ ICON_SELECTALL, draw_selectall_icon_raw },
	{ ICON_SELECTED, draw_selected_icon_raw },
	{ ICON_SHAPE, draw_shape_icon_raw },
	{ ICON_SINGLETEXT, draw_singletext_icon_raw },
	{ ICON_SIZEHORZ, draw_sizehorz_icon_raw },
	{ ICON_SIZEVERT, draw_sizevert_icon_raw },
	{ ICON_SLIDE, draw_slide_icon_raw },
	{ ICON_SPACEHORZ, draw_spacehorz_icon_raw },
	{ ICON_SPACEVERT, draw_spacevert_icon_raw },
	{ ICON_SPIN, draw_spin_icon_raw },
	{ ICON_START, draw_start_icon_raw },
	{ ICON_STATIC, draw_static_icon_raw },
	{ ICON_STOP, draw_stop_icon_raw },
	{ ICON_STYLE, draw_style_icon_raw },
	{ ICON_SUM, draw_sum_icon_raw },
	{ ICON_TABLE, draw_table_icon_raw },
	{ ICON_TAG, draw_tag_icon_raw },
	{ ICON_TIME, draw_time_icon_raw },
	{ ICON_TOUCH, draw_touch_icon_raw },
	{ ICON_TOUCHED, draw_touched_icon_raw },
	{ ICON_TRENDS, draw_trends_icon_raw },
	{ ICON_UNDO, draw_undo_icon_raw },
	{ ICON_UNLOCK, draw_unlock_icon_raw },
	{ ICON_UP, draw_up_icon_raw },
	{ ICON_UPDATE, draw_update_icon_raw },
	{ ICON_USER, draw_user_icon_raw },
	{ ICON_ZOOMIN, draw_zoomin_icon_raw },
	{ ICON_ZOOMOUT, draw_zoomout_icon_raw },
};

static PF_ICON_MAKE _find_icon_maker(int i, int j, const tchar_t* iname)
{
	int rt, k;

	if (i == j)
	{
		rt = compare_text(iname, -1, g_icon_table[i].icon_name, -1, 1);
		if (!rt)
			return g_icon_table[i].icon_func;
		else
			return NULL;
	}
	else if (i < j)
	{
		k = (i + j) / 2;
		rt = compare_text(iname, -1, g_icon_table[k].icon_name, -1, 1);
		if (!rt)
			return g_icon_table[k].icon_func;
		else if (rt > 0)
			return _find_icon_maker(k + 1, j, iname);
		else
			return _find_icon_maker(i, k - 1, iname);
	}
	else
	{
		return NULL;
	}
}

PF_ICON_MAKE find_icon_maker(const tchar_t* iname)
{
	int k;

	if (is_null(iname))
		return NULL;

	k = sizeof(g_icon_table) / sizeof(ICON_DRAW_TABLE);

	return _find_icon_maker(0, k - 1, iname);
}

#if defined(_DEBUG) || defined(DEBUG)

void test_icon_draw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	int i, k;
	long feed = 5;

	k = sizeof(g_icon_table) / sizeof(ICON_DRAW_TABLE);

	default_xfont(&xf);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);

	for (i = 0; i < k; i++)
	{
		xr.x = pxr->x + feed;
		xr.y = pxr->y + i * (32 + feed);
		xr.w = 16;
		xr.h = 16;

		(*(g_icon_table[i].icon_func))(rdc, pxc, &xr);

		xr.x = pxr->x + 16 + 2 * feed;
		xr.y = pxr->y + i * (32 + feed);
		xr.w = 24;
		xr.h = 24;

		(*(g_icon_table[i].icon_func))(rdc, pxc, &xr);

		xr.x = pxr->x + 16 + 24 + 3 * feed;
		xr.y = pxr->y + i * (32 + feed);
		xr.w = 32;
		xr.h = 32;

		(*(g_icon_table[i].icon_func))(rdc, pxc, &xr);

		xr.x = pxr->x + 16 + 24 + 32 + 4 * feed;
		xr.y = pxr->y + i * (32 + feed);
		xr.w = 100;
		xr.h = 32;

		draw_text_raw(rdc, &xf, &xa, &xr, g_icon_table[i].icon_name, -1);
	}
}

#endif


#endif /*XDK_SUPPORT_CONTEXT*/
