/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc focus document

	@module	focusing.c | implement file

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

#include "focusing.h"

#if defined(XDL_SUPPORT_GDI)

void draw_select_raw(const drawing_interface* piv, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, prt);
}

void draw_focus_raw(const drawing_interface* piv, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);

	(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, prt);
}

void draw_sizing_raw(const drawing_interface* piv, const xcolor_t* pxc, const xrect_t* prt, int deep, dword_t pos)
{
	xrect_t xr;
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, _T("%d"), deep);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

	if (pos & SIZING_TOPLEFT)
	{
		xr.x = prt->x;
		xr.y = prt->y;
		xr.w = 4;
		xr.h = 4;

		(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, &xr);
	}

	if (pos & SIZING_TOPCENTER)
	{
		xr.x = prt->x + prt->w / 2 - 2;
		xr.y = prt->y;
		xr.w = 4;
		xr.h = 4;

		(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, &xr);
	}

	if (pos & SIZING_TOPRIGHT)
	{
		xr.x = prt->x + prt->w - 4;
		xr.y = prt->y;
		xr.w = 4;
		xr.h = 4;

		(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, &xr);
	}

	if (pos & SIZING_BOTTOMLEFT)
	{
		xr.x = prt->x;
		xr.y = prt->y + prt->h - 2;
		xr.w = 4;
		xr.h = 4;

		(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, &xr);
	}

	if (pos & SIZING_BOTTOMCENTER)
	{
		xr.x = prt->x + prt->w / 2 - 2;
		xr.y = prt->y + prt->h - 4;
		xr.w = 4;
		xr.h = 4;

		(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, &xr);
	}

	if (pos & SIZING_BOTTOMRIGHT)
	{
		xr.x = prt->x + prt->w - 4;
		xr.y = prt->y + prt->h - 4;
		xr.w = 4;
		xr.h = 4;

		(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, &xr);
	}

	if (pos & SIZING_LEFTCENTER)
	{
		xr.x = prt->x;
		xr.y = prt->y + prt->h / 2 - 2;
		xr.w = 4;
		xr.h = 4;

		(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, &xr);
	}

	if (pos & SIZING_RIGHTCENTER)
	{
		xr.x = prt->x + prt->w - 4;
		xr.y = prt->y + prt->h / 2 - 2;
		xr.w = 4;
		xr.h = 4;

		(*piv->pf_draw_rect)(piv->ctx, &xp, NULL, &xr);
	}
}

void draw_feed_raw(const drawing_interface* piv, const xcolor_t* pxc, const xrect_t* prt, int deep)
{
	xpoint_t pt[2];
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	xsprintf(xp.opacity, _T("%d"), deep);
	//xp.adorn.feed = 2;
	//xp.adorn.size = 2;

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x;
	pt[1].y = prt->y + 5;
	(*piv->pf_draw_line)(piv->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x;
	pt[0].y = prt->y;
	pt[1].x = prt->x + 5;
	pt[1].y = prt->y;
	(*piv->pf_draw_line)(piv->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w - 5;
	pt[1].y = prt->y + prt->h;
	(*piv->pf_draw_line)(piv->ctx, &xp, &pt[0], &pt[1]);

	pt[0].x = prt->x + prt->w;
	pt[0].y = prt->y + prt->h;
	pt[1].x = prt->x + prt->w;
	pt[1].y = prt->y + prt->h - 5;
	(*piv->pf_draw_line)(piv->ctx, &xp, &pt[0], &pt[1]);
}

#endif /*XDL_SUPPORT_VIEW*/
