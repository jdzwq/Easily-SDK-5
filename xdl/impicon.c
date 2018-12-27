/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc icon document

	@module	impicon.c | icon implement file

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

#include "impicon.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#if defined(XDK_SUPPORT_CONTEXT) && defined(XDK_SUPPORT_CONTEXT_GRAPHIC)

void draw_logo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	
}

void draw_insert_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 - 1;
	xr.w = prt->w;
	xr.h = 2;

	if (xr.w % 2 == 0)
		xr.w -= 1;
	
	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.x = prt->x + prt->w / 2 - 1;
	xr.y = prt->y;
	xr.w = 2;
	xr.h = prt->h;

	if (xr.h % 2 == 0)
		xr.h -= 1;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_delete_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h / 2 - 1;
	xr.w = prt->w;
	xr.h = 2;

	if (xr.w % 2 == 0)
		xr.w -= 1;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_plus_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w / 2;
	xr.h = prt->h / 2;

	if (xr.w % 2 == 0)
		xr.w -= 1;
	if (xr.h % 2 == 0)
		xr.h -= 1;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	draw_rect_raw(rdc, &xp, NULL, &xr);

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

	draw_rect_raw(rdc, &xp, NULL, prt);
}

void draw_minus_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

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

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + xr.h / 2;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_collapse_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t xr;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	if (!(xr.w % 2 == 0))
		xr.w -= 1;
	if (!(xr.h % 2 == 0))
		xr.h -= 1;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

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
	xrect_t xr;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	if (!(xr.w % 2 == 0))
		xr.w -= 1;
	if (!(xr.h % 2 == 0))
		xr.h -= 1;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

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

void draw_home_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + 1;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = 2;
	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_up_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + 1;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_down_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h - 1;
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_end_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w / 2;
	pt[2].y = prt->y + prt->h - 1;
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	xr.x = prt->x;
	xr.y = prt->y + prt->h - 2;
	xr.w = prt->w;
	xr.h = 2;
	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_first_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + 1;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h;
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = 2;
	xr.h = prt->h;
	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_prev_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x + 1;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h;
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_next_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w - 1;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);
}

void draw_last_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt[3];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w - 1;
	pt[1].y = prt->y + prt->h / 2;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	draw_polygon_raw(rdc, &xp, &xb, pt, 3);

	xr.x = prt->x + prt->w - 2;
	xr.y = prt->y;
	xr.w = 2;
	xr.h = prt->h;
	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_zoomin_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	
	xr.x = prt->x - 1;
	xr.y = prt->y - 1;
	xr.w = prt->w + 2;
	xr.h = prt->h + 2;

	if (xr.w % 2 == 0)
		xr.w -= 1;
	if (xr.h % 2 == 0)
		xr.h -= 1;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt1.x = xr.x + 3;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - 3;
	pt2.y = xr.y + xr.h / 2;

	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x + xr.w / 2;
	pt1.y = xr.y + 3;
	pt2.x = xr.x + xr.w / 2;
	pt2.y = xr.y + xr.h - 3;

	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_zoomout_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xr.x = prt->x - 1;
	xr.y = prt->y - 1;
	xr.w = prt->w + 2;
	xr.h = prt->h + 2;

	if (xr.w % 2 == 0)
		xr.w -= 1;
	if (xr.h % 2 == 0)
		xr.h -= 1;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	draw_ellipse_raw(rdc, &xp, NULL, &xr);

	pt1.x = xr.x + 3;
	pt1.y = xr.y + xr.h / 2;
	pt2.x = xr.x + xr.w - 3;
	pt2.y = xr.y + xr.h / 2;

	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_keybox_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	lighten_xpen(&xp, -20);

	xr.x = prt->x - 1;
	xr.y = prt->y + 1;
	xr.w = prt->w + 2;
	xr.h = prt->h - 2;

	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_keyboxed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

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
	xrect_t xr;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	lighten_xpen(&xp, -20);

	xr.x = prt->x - 1;
	xr.y = prt->y - 1;
	xr.w = prt->w + 2;
	xr.h = prt->h + 2;

	if (xr.w % 2 == 0)
		xr.w -= 1;
	if (xr.h % 2 == 0)
		xr.h -= 1;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

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
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	lighten_xpen(&xp, -20);

	xr.x = prt->x - 1;
	xr.y = prt->y - 1;
	xr.w = prt->w + 2;
	xr.h = prt->h + 2;

	if (xr.w % 2 == 0)
		xr.w -= 1;
	if (xr.h % 2 == 0)
		xr.h -= 1;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	pt_expand_rect(&xr, -2, -2);

	lighten_xbrush(&xb, 20);

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_close_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t xr;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

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

	pt1.x = xr.x;
	pt1.y = xr.y;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y + xr.h;
	draw_line_raw(rdc, &xp, &pt1, &pt2);

	pt1.x = xr.x;
	pt1.y = xr.y + xr.h;
	pt2.x = xr.x + xr.w;
	pt2.y = xr.y;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
}

void draw_minimize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y + prt->h - 2;
	xr.w = 4;
	xr.h = 2;
	draw_rect_raw(rdc, &xp, &xb, &xr);
}

void draw_maximize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xr.x = prt->x;
	xr.w = prt->w - 2;
	xr.y = prt->y;
	xr.h = prt->h - 2;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

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
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	if (xr.w < xr.h)
		xr.w = xr.h;
	else if (xr.w > xr.h)
		xr.h = xr.w;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_sum_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;
	xpoint_t pt[5] = { 0 };

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

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

	pt[0].x = xr.x + xr.w;
	pt[0].y = xr.y;
	pt[1].x = xr.x;
	pt[1].y = xr.y;
	pt[2].x = xr.x + xr.w / 2;
	pt[2].y = xr.y + xr.h / 2;
	pt[3].x = xr.x;
	pt[3].y = xr.y + xr.h;
	pt[4].x = xr.x + xr.w;
	pt[4].y = xr.y + xr.h;

	draw_polyline_raw(rdc, &xp, pt, 5);
}

void draw_checkbox_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

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

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_checked_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xpoint_t pt1, pt2;
	xrect_t xr;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	lighten_xbrush(&xb, DEF_SOFT_LIGHTEN);

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
	xrect_t xr;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

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

	draw_ellipse_raw(rdc, &xp, NULL, &xr);
}

void draw_radioed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xbrush_t xb;
	xrect_t xr;

	default_xpen(&xp);
	xscpy(xp.size, _T("2"));
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

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

	draw_ellipse_raw(rdc, &xp, &xb, &xr);
}

void draw_selected_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xpoint_t pt1, pt2;
	xrect_t xr;

	default_xpen(&xp);
	xscpy(xp.size, _T("1"));
	format_xcolor(pxc, xp.color);

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
	xrect_t xr;
	xpoint_t pt[4];

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

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
	xrect_t xr;
	xpoint_t pt[5];
	int feed;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	xr.x = prt->x;
	xr.y = prt->y;
	xr.w = prt->w;
	xr.h = prt->h;

	feed = xr.w / 3;
	if (feed < 4)
		feed = 4;

	pt[0].x = xr.x;
	pt[0].y = xr.y;

	pt[1].x = xr.x + xr.w - feed;
	pt[1].y = xr.y;

	pt[2].x = xr.x + xr.w;
	pt[2].y = xr.y + xr.h / 2;

	pt[3].x = xr.x + xr.w - feed;
	pt[3].y = xr.y + xr.h;

	pt[4].x = xr.x;
	pt[4].y = xr.y + xr.h;

	draw_polygon_raw(rdc, &xp, &xb, pt, 5);
}

void draw_fixed_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xpen_t xp;
	xrect_t xr;
	xpoint_t pt1, pt2;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

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
	pt1.y = xr.y;
	pt2.x = xr.x + 2;
	pt2.y = xr.y + xr.h;
	draw_line_raw(rdc, &xp, &pt1, &pt2);
	
	xr.x += 2;
	xr.w -= 2;
	xr.y += 2;
	xr.h -= 4;

	draw_rect_raw(rdc, &xp, NULL, &xr);
}

void draw_point_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	int fs;

	fs = font_size(rdc, prt->h) - 1;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ltoxs(fs, xf.size, INT_LEN);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	draw_text_raw(rdc, &xf, &xa, prt, ICON_TEXT_POINT, -1);
}

void draw_cross_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	int fs;

	fs = font_size(rdc, prt->h);

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ltoxs(fs, xf.size, INT_LEN);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	draw_text_raw(rdc, &xf, &xa, prt, ICON_TEXT_CROSS, -1);
}

void draw_rect_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	int fs;

	fs = font_size(rdc, prt->h);

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ltoxs(fs, xf.size, INT_LEN);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	draw_text_raw(rdc, &xf, &xa, prt, ICON_TEXT_RECT, -1);
}

void draw_ellipse_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	int fs;

	fs = font_size(rdc, prt->h);

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ltoxs(fs, xf.size, INT_LEN);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	draw_text_raw(rdc, &xf, &xa, prt, ICON_TEXT_ELLIPSE, -1);
}

void draw_star_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	int fs;

	fs = font_size(rdc, prt->h) - 1;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ltoxs(fs, xf.size, INT_LEN);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	draw_text_raw(rdc, &xf, &xa, prt, ICON_TEXT_STAR, -1);
}

void draw_diamond_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt)
{
	xfont_t xf;
	xface_t xa;
	int fs;

	fs = font_size(rdc, prt->h) - 1;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ltoxs(fs, xf.size, INT_LEN);
	format_xcolor(pxc, xf.color);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	draw_text_raw(rdc, &xf, &xa, prt, ICON_TEXT_DIAMOND, -1);
}

void draw_numeric_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt, const tchar_t* no)
{
	xfont_t xf;
	xface_t xa;
	xpen_t xp;
	xbrush_t xb;
	int fs;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);

	draw_ellipse_raw(rdc, &xp, &xb, prt);

	fs = font_size(rdc, prt->h) - 1;

	default_xfont(&xf);
	xscpy(xf.family, GDI_ATTR_FONT_FAMILY_ARIA);
	ltoxs(fs, xf.size, INT_LEN);
	format_xcolor(pxc, xf.color);
	lighten_xfont(&xf, DEF_HARD_LIGHTEN);

	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	draw_text_raw(rdc, &xf, &xa, prt, no, -1);
}

#endif /*XDK_SUPPORT_CONTEXT*/
