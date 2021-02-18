/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc shape document

	@module	shaping.c | implement file

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

#include "shaping.h"
#include "docattr.h"

#include "xdlimp.h"
#include "xdlstd.h"

#if defined(XDL_SUPPORT_GDI)


void _draw_forwardslash_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].fx = prt->fx + prt->fw;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx;
	pt[1].fy = prt->fy + prt->fh;

	(*pif->pf_draw_line)(pif->ctx, pxp, &pt[0], &pt[1]);
}

void _draw_backslash_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy + prt->fh;

	(*pif->pf_draw_line)(pif->ctx, pxp, &pt[0], &pt[1]);
}

void _draw_leftline_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx;
	pt[1].fy = prt->fy + prt->fh;

	(*pif->pf_draw_line)(pif->ctx, pxp, &pt[0], &pt[1]);
}

void _draw_rightline_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].fx = prt->fx + prt->fw;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy + prt->fh;

	(*pif->pf_draw_line)(pif->ctx, pxp, &pt[0], &pt[1]);
}

void _draw_topline_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy;

	(*pif->pf_draw_line)(pif->ctx, pxp, &pt[0], &pt[1]);
}

void _draw_bottomline_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[2];

	pt[0].fx = prt->fx;
	pt[0].fy = prt->fy + prt->fh;
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy + prt->fh;

	(*pif->pf_draw_line)(pif->ctx, pxp, &pt[0], &pt[1]);
}

void _draw_lefttriangle_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[4];

	pt[0].fx = prt->fx + 1;
	pt[0].fy = prt->fy + prt->fh / 2;
	pt[1].fx = prt->fx + prt->fw;
	pt[1].fy = prt->fy;;
	pt[2].fx = prt->fx + prt->fw;
	pt[2].fy = prt->fy + prt->fh;
	pt[3].fx = pt[0].fx;
	pt[3].fy = pt[0].fy;

	(*pif->pf_draw_polygon)(pif->ctx, pxp, pxb, pt, 4);
}

void _draw_righttriangle_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[4];

	pt[0].fx = prt->fx + prt->fw;
	pt[0].fy = prt->fy + prt->fh / 2;
	pt[1].fx = prt->fx;
	pt[1].fy = prt->fy;
	pt[2].fx = prt->fx;
	pt[2].fy = prt->fy + prt->fh;
	pt[3].fx = pt[0].fx;
	pt[3].fy = pt[0].fy;

	(*pif->pf_draw_polygon)(pif->ctx, pxp, pxb, pt, 4);
}

void _draw_toptriangle_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[4];

	pt[0].fx = prt->fx + prt->fw / 2;
	pt[0].fy = prt->fy;
	pt[1].fx = prt->fx;
	pt[1].fy = prt->fy + prt->fh;
	pt[2].fx = prt->fx + prt->fw;
	pt[2].fy = prt->fy + prt->fh;
	pt[3].fx = pt[0].fx;
	pt[3].fy = pt[0].fy;

	(*pif->pf_draw_polygon)(pif->ctx, pxp, pxb, pt, 4);
}

void _draw_bottomtriangle_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	xpoint_t pt[4];

	pt[0].fx = prt->fx + prt->fw / 2;
	pt[0].fy = prt->fy + prt->fh;
	pt[1].fx = prt->fx;
	pt[1].fy = prt->fy;
	pt[2].fx = prt->fx + prt->fw;
	pt[2].fy = prt->fy;
	pt[3].fx = pt[0].fx;
	pt[3].fy = pt[0].fy;

	(*pif->pf_draw_polygon)(pif->ctx, pxp, pxb, pt, 4);
}

void _draw_rect_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	(*pif->pf_draw_rect)(pif->ctx, pxp, pxb, prt);
}

void _draw_round_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	(*pif->pf_draw_round)(pif->ctx, pxp, pxb, prt);
}

void _draw_ellipse_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt)
{
	(*pif->pf_draw_ellipse)(pif->ctx, pxp, pxb, prt);
}


/*******************************************************************************************************/

typedef void(*PF_SHAPE_MAKE)(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt);

typedef struct _SHAPE_DRAW_TABLE{
	tchar_t shape_name[32];
	PF_SHAPE_MAKE shape_func;
}SHAPE_DRAW_TABLE;

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


void draw_shape(const drawing_interface* pif, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, const tchar_t* shape)
{
	PF_SHAPE_MAKE pf;

	pf = find_shape_maker(shape);
	if (pf)
	{
		(*pf)(pif, pxp, pxb, prt);
	}
}

#endif


