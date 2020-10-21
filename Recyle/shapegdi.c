/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc shape document

	@module	shapegdi.c | implement file

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

#include "shapegdi.h"
#include "xdcimp.h"

#if defined(XDU_SUPPORT_CONTEXT)


void _draw_forwardslash_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, pxp, &pt[0], &pt[1]);
}

void _draw_backslash_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, pxp, &pt[0], &pt[1]);
}

void _draw_leftline_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, pxp, &pt[0], &pt[1]);
}

void _draw_rightline_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, pxp, &pt[0], &pt[1]);
}

void _draw_topline_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;

	draw_line_raw(rdc, pxp, &pt[0], &pt[1]);
}

void _draw_bottomline_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].x = prt->x;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h;

	draw_line_raw(rdc, pxp, &pt[0], &pt[1]);
}

void _draw_lefttriangle_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[4];

	pt[0].x = prt->x + 1;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y;;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h;
	pt[3].x = pt[0].x;
	pt[3].y = pt[0].y;

	draw_polyline_raw(rdc, pxp, pt, 4);
}

void _draw_righttriangle_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[3];

	pt[0].x = prt->x + prt->w - 1;
	pt[0].y = prt->y + prt->h / 2;
	pt[1].x = prt->x;
	pt[1].y = prt->y;
	pt[2].x = prt->x;
	pt[2].y = prt->y + prt->h;
	pt[3].x = pt[0].x;
	pt[3].y = pt[0].y;

	draw_polyline_raw(rdc, pxp, pt, 3);
}

void _draw_toptriangle_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[3];

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + 1;
	pt[1].x = prt->x;
	pt[1].y = prt->y + prt->h;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y + prt->h;
	pt[3].x = pt[0].x;
	pt[3].y = pt[0].y;

	draw_polyline_raw(rdc, pxp, pt, 3);
}

void _draw_bottomtriangle_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	xpoint_t pt[4];

	pt[0].x = prt->x + prt->w / 2;
	pt[0].y = prt->y + prt->h - 1;
	pt[1].x = prt->x;
	pt[1].y = prt->y;
	pt[2].x = prt->x + prt->w;
	pt[2].y = prt->y;
	pt[3].x = pt[0].x;
	pt[3].y = pt[0].y;

	draw_polyline_raw(rdc, pxp, pt, 4);
}

void _draw_rect_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	draw_rect_raw(rdc, pxp, NULL, prt);
}

void _draw_round_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	draw_round_raw(rdc, pxp, NULL, prt);
}

void _draw_ellipse_shape(res_ctx_t rdc, const xpen_t* pxp, const xrect_t* prt)
{
	draw_ellipse_raw(rdc, pxp, NULL, prt);
}

SHAPE_DRAW_TABLE g_shape_table[] = {
	{ ATTR_SHAPE_BACKSLASH, _draw_backslash_shape },
	{ ATTR_SHAPE_BOTTOMLINE, _draw_bottomline_shape },
	{ ATTR_SHAPE_BOTTOMTRIANGLE, _draw_bottomtriangle_shape },
	{ ATTR_SHAPE_ELLIPSE, _draw_ellipse_shape },
	{ ATTR_SHAPE_FORWARDSLASH, _draw_forwardslash_shape },
	{ ATTR_SHAPE_LEFTLINE, _draw_leftline_shape },
	{ ATTR_SHAPE_LEFTTRIANGLE, _draw_lefttriangle_shape },
	{ ATTR_SHAPE_RECT, _draw_rect_shape },
	{ ATTR_SHAPE_RIGHTLINE, _draw_rightline_shape },
	{ ATTR_SHAPE_RIGHTTRIANGLE, _draw_righttriangle_shape },
	{ ATTR_SHAPE_ROUND, _draw_round_shape },
	{ ATTR_SHAPE_TOPLINE, _draw_topline_shape },
	{ ATTR_SHAPE_TOPTRIANGLE, _draw_toptriangle_shape },
};

static PF_SHAPE_MAKE _find_shape_maker(int i, int j, const tchar_t* iname)
{
	int rt, k;

	if (i == j)
	{
		rt = compare_text(iname, -1, g_shape_table[i].shape_name, -1, 1);
		if (!rt)
			return g_shape_table[i].shape_func;
		else
			return NULL;
	}
	else if (i < j)
	{
		k = (i + j) / 2;
		rt = compare_text(iname, -1, g_shape_table[k].shape_name, -1, 1);
		if (!rt)
			return g_shape_table[k].shape_func;
		else if (rt > 0)
			return _find_shape_maker(k + 1, j, iname);
		else
			return _find_shape_maker(i, k - 1, iname);
	}
	else
	{
		return NULL;
	}
}

PF_SHAPE_MAKE find_shape_maker(const tchar_t* iname)
{
	int k;

	if (is_null(iname))
		return NULL;

	k = sizeof(g_shape_table) / sizeof(SHAPE_DRAW_TABLE);

	return _find_shape_maker(0, k - 1, iname);
}


#endif


