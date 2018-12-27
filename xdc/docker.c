/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc docker document

	@module	docker.c | frame docker implement file

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

#include "docker.h"
#include "handler.h"


#define DOCKER_SPLIT_SPAN		(long)10 //pt


static int _docker_calc_hint(docker_t* ptd, const xpoint_t* pxp)
{
	xrect_t xr,xr_cli;
	long top, bottom, left, right;
	long span;
	int i;

	widget_get_client_rect(ptd->widget, &xr_cli);

	span = DOCKER_SPLIT_SPAN;
	top = bottom = left = right = 0;

	for (i = 0; i < 4; i++)
	{
		if (ptd->dock[i].style & WD_DOCK_LEFT)
		{
			xr.x = xr_cli.x;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if ((ptd->dock[i].style & WD_DOCK_DYNA) && pt_inside(pxp->x, pxp->y, xr.x + xr.w, xr.y, xr.x + xr.w + span, xr.y + xr.h))
				return i;

			if (ptd->dock[i].style & WD_DOCK_DYNA)
				left += (ptd->dock[i].cx + span);
			else
				left += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & WD_DOCK_TOP)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y;
			xr.h = ptd->dock[i].cy;

			if ((ptd->dock[i].style & WD_DOCK_DYNA) && pt_inside(pxp->x, pxp->y, xr.x, xr.y + xr.h, xr.x + xr.w, xr.y + xr.h + span))
				return i;

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				top += (ptd->dock[i].cy + span);
			else
				top += ptd->dock[i].cy;
		}
		else if (ptd->dock[i].style & WD_DOCK_RIGHT)
		{
			xr.x = xr_cli.x + xr_cli.w - ptd->dock[i].cx;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if ((ptd->dock[i].style & WD_DOCK_DYNA) && pt_inside(pxp->x, pxp->y, xr.x - span, xr.y, xr.x, xr.y + xr.h))
				return i;

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				right += (ptd->dock[i].cx + span);
			else
				right += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & WD_DOCK_BOTTOM)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y + xr_cli.h - ptd->dock[i].cy;
			xr.h = ptd->dock[i].cy;

			if ((ptd->dock[i].style & WD_DOCK_DYNA)  && pt_inside(pxp->x, pxp->y, xr.x, xr.y - span, xr.x + xr.w, xr.y))
				return i;

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				bottom += (ptd->dock[i].cy + span);
			else
				bottom += ptd->dock[i].cy;
		}
	}

	return -1;
}

void _docker_calc_rect(docker_t* ptd, dword_t style, xrect_t* pxr)
{
	xrect_t xr, xr_cli;
	long top, bottom, left, right, span;
	int i;

	if (!ptd)
		return;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	if (widget_is_minimized(ptd->widget))
		return;

	widget_get_client_rect(ptd->widget, &xr_cli);

	span = DOCKER_SPLIT_SPAN;
	top = bottom = left = right = 0;

	for (i = 0; i < 4; i++)
	{
		if (ptd->dock[i].style & WD_DOCK_LEFT)
		{
			xr.x = xr_cli.x;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if (ptd->dock[i].style & style)
			{
				xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
				return;
			}

			if (ptd->dock[i].style & WD_DOCK_DYNA)
				left += (ptd->dock[i].cx + span);
			else
				left += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & WD_DOCK_TOP)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y;
			xr.h = ptd->dock[i].cy;

			if (ptd->dock[i].style & style)
			{
				xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
				return;
			}

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				top += (ptd->dock[i].cy + span);
			else
				top += ptd->dock[i].cy;
		}
		else if (ptd->dock[i].style & WD_DOCK_RIGHT)
		{
			xr.x = xr_cli.x + xr_cli.w - ptd->dock[i].cx;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if (ptd->dock[i].style & style)
			{
				xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
				return;
			}

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				right += (ptd->dock[i].cx + span);
			else
				right += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & WD_DOCK_BOTTOM)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y + xr_cli.h - ptd->dock[i].cy;
			xr.h = ptd->dock[i].cy;

			if (ptd->dock[i].style & style)
			{
				xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
				return;
			}

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				bottom += (ptd->dock[i].cy + span);
			else
				bottom += ptd->dock[i].cy;
		}
	}

	pxr->x = xr_cli.x + left;
	pxr->y = xr_cli.y + top ;
	pxr->w = xr_cli.w - left - right;
	pxr->h = xr_cli.h - top - bottom ;
}

/*************************************************************************************************/

void hand_docker_mouse_move(docker_t* ptd, dword_t dw, const xpoint_t* pxp)
{
	int hint;

	if (ptd->drag)
		return;

	hint = _docker_calc_hint(ptd, pxp);

	if (hint >= 0)
	{
		widget_set_cursor(ptd->widget, CURSOR_HAND);
	}
}

void hand_docker_lbutton_down(docker_t* ptd, const xpoint_t* pxp)
{
	ptd->ind = _docker_calc_hint(ptd, pxp);

	if (ptd->ind >= 0)
	{
		ptd->drag = 1;
		ptd->x = pxp->x;
		ptd->y = pxp->y;

		switch (ptd->dock[ptd->ind].style & 0x0000FFFF)
		{
		case WD_DOCK_LEFT:
		case WD_DOCK_RIGHT:
			widget_set_cursor(ptd->widget, CURSOR_SIZEWE);
			break;
		case WD_DOCK_TOP:
		case WD_DOCK_BOTTOM:
			widget_set_cursor(ptd->widget, CURSOR_SIZENS);
			break;
		}

		widget_set_capture(ptd->widget, 1);
	}

}

void hand_docker_lbutton_up(docker_t* ptd, const xpoint_t* pxp)
{
	xrect_t xr;
	long span = DOCKER_SPLIT_SPAN;

	if (ptd->drag)
	{
		widget_set_capture(ptd->widget, 0);

		switch (ptd->dock[ptd->ind].style & 0x0000FFFF)
		{
		case WD_DOCK_LEFT:
			ptd->dock[ptd->ind].cx += (pxp->x - ptd->x);
			break;
		case WD_DOCK_RIGHT:
			ptd->dock[ptd->ind].cx -= (pxp->x - ptd->x);
			break;
		case WD_DOCK_TOP:
			ptd->dock[ptd->ind].cy += (pxp->y - ptd->y);
			break;
		case WD_DOCK_BOTTOM:
			ptd->dock[ptd->ind].cy -= (pxp->y - ptd->y);
			break;
		}

		widget_get_client_rect(ptd->widget, &xr);

		if (ptd->dock[ptd->ind].cx < 0)
			ptd->dock[ptd->ind].cx = 0;
		if (ptd->dock[ptd->ind].cx > xr.w - span)
			ptd->dock[ptd->ind].cx = xr.w - span;

		if (ptd->dock[ptd->ind].cy < 0)
			ptd->dock[ptd->ind].cy = 0;
		if (ptd->dock[ptd->ind].cy > xr.h - span)
			ptd->dock[ptd->ind].cy = xr.h - span;

		ptd->drag = 0;
		ptd->x = pxp->x;
		ptd->y = pxp->y;
		ptd->ind = -1;

		widget_update_client(ptd->widget);
	}
}

void hand_docker_paint(docker_t* ptd, res_ctx_t dc, const xrect_t* pxr)
{
	res_ctx_t rdc;
	canvas_t canv;
	xrect_t xr, xr_cli, xr_bar;
	long top, bottom, left, right, span;
	xbrush_t xb = { 0 };
	xgradi_t gi = { 0 };
	xcolor_t xc = { 0 };
	int i;

	widget_get_xbrush(ptd->widget, &xb);
	parse_xcolor(&xc, xb.color);

	format_xcolor(&xc, gi.brim_color);
	lighten_xcolor(&xc, DEF_SOFT_DARKEN);
	format_xcolor(&xc, gi.core_color);

	widget_get_client_rect(ptd->widget, &xr_cli);

	canv = widget_get_canvas(ptd->widget);

	XDL_ASSERT(canv != NULL);

	rdc = begin_canvas_paint(canv, dc, xr_cli.w, xr_cli.h);

	draw_rect_raw(rdc, NULL, &xb, &xr_cli);

	span = DOCKER_SPLIT_SPAN;
	top = bottom = left = right = 0;

	for (i = 0; i < 4; i++)
	{
		if (ptd->dock[i].style & WD_DOCK_LEFT)
		{
			xr.x = xr_cli.x;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
			{
				xr_bar.x = xr.x + xr.w;
				xr_bar.w = span;
				xr_bar.y = xr.y;
				xr_bar.h = xr.h;
				xscpy(gi.type, GDI_ATTR_GRADIENT_TYPE_VERT);
				gradient_rect_raw(rdc, &gi, &xr_bar);
			}

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				left += (ptd->dock[i].cx + span);
			else
				left += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & WD_DOCK_TOP)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y;
			xr.h = ptd->dock[i].cy;

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
			{
				xr_bar.x = xr.x;
				xr_bar.w = xr.w;
				xr_bar.y = xr.y + xr.h;
				xr_bar.h = span;
				xscpy(gi.type, GDI_ATTR_GRADIENT_TYPE_HORZ);
				gradient_rect_raw(rdc, &gi, &xr_bar);
			}

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				top += (ptd->dock[i].cy + span);
			else
				top += ptd->dock[i].cy;
		}
		else if (ptd->dock[i].style & WD_DOCK_RIGHT)
		{
			xr.x = xr_cli.x + xr_cli.w - ptd->dock[i].cx;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
			{
				xr_bar.x = xr.x - span;
				xr_bar.w = span;
				xr_bar.y = xr.y;
				xr_bar.h = xr.h;
				xscpy(gi.type, GDI_ATTR_GRADIENT_TYPE_VERT);
				gradient_rect_raw(rdc, &gi, &xr_bar);
			}

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				right += (ptd->dock[i].cx + span);
			else
				right += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & WD_DOCK_BOTTOM)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y + xr_cli.h - ptd->dock[i].cy;
			xr.h = ptd->dock[i].cy;

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
			{
				xr_bar.x = xr.x;
				xr_bar.w = xr.w;
				xr_bar.y = xr.y - span;
				xr_bar.h = span;
				xscpy(gi.type, GDI_ATTR_GRADIENT_TYPE_HORZ);
				gradient_rect_raw(rdc, &gi, &xr_bar);
			}

			if ((ptd->dock[i].style & WD_DOCK_DYNA))
				bottom += (ptd->dock[i].cy + span);
			else
				bottom += ptd->dock[i].cy;
		}
	}

	end_canvas_paint(canv, dc, pxr);
}

void hand_docker_calc_rect(docker_t* ptd, dword_t style, xrect_t* pxr)
{
	_docker_calc_rect(ptd, style, pxr);
}

