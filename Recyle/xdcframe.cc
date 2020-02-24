/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc mainframe document

	@module	mainframe.c | main frame implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#include "xdcfrm.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcgdi.h"
#include "xdccanv.h"
#include "xdcres.h"
#include "xdcwidg.h"
#include "xdcctrl.h"


#define FRAME_SPLIT_SPAN		(long)10 //pt

typedef struct _DOCKINFO{
	dword_t style;
	long cx, cy;
}DOCKINFO;

typedef struct _frame_delta_t{
	DOCKINFO dock[4];

	long org_x, org_y;
	int org_ind;

	bool_t b_size;
}frame_delta_t;

#define GETFRAMEDELTA(ph) 	(frame_delta_t*)widget_get_user_delta(ph)
#define SETFRAMEDELTA(ph,ptd) widget_set_user_delta(ph,ptd)

/************************************************************************************/

static int _frame_calc_hint(res_win_t widget, const xpoint_t* pxp)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);
	xrect_t xr,xr_cli;
	long top, bottom, left, right;
	long span;
	int i;

	widget_get_client_rect(widget, &xr_cli);

	span = FRAME_SPLIT_SPAN;
	top = bottom = left = right = 0;

	for (i = 0; i < 4; i++)
	{
		if (ptd->dock[i].style & DOCK_AT_LEFT)
		{
			xr.x = xr_cli.x;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if ((ptd->dock[i].style & DOCK_IS_DYNA) && pt_inside(pxp->x, pxp->y, xr.x + xr.w, xr.y, xr.x + xr.w + span, xr.y + xr.h))
				return i;

			if (ptd->dock[i].style & DOCK_IS_DYNA)
				left += (ptd->dock[i].cx + span);
			else
				left += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & DOCK_AT_TOP)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y;
			xr.h = ptd->dock[i].cy;

			if ((ptd->dock[i].style & DOCK_IS_DYNA) && pt_inside(pxp->x, pxp->y, xr.x, xr.y + xr.h, xr.x + xr.w, xr.y + xr.h + span))
				return i;

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				top += (ptd->dock[i].cy + span);
			else
				top += ptd->dock[i].cy;
		}
		else if (ptd->dock[i].style & DOCK_AT_RIGHT)
		{
			xr.x = xr_cli.x + xr_cli.w - ptd->dock[i].cx;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if ((ptd->dock[i].style & DOCK_IS_DYNA) && pt_inside(pxp->x, pxp->y, xr.x - span, xr.y, xr.x, xr.y + xr.h))
				return i;

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				right += (ptd->dock[i].cx + span);
			else
				right += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & DOCK_AT_BOTTOM)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y + xr_cli.h - ptd->dock[i].cy;
			xr.h = ptd->dock[i].cy;

			if ((ptd->dock[i].style & DOCK_IS_DYNA)  && pt_inside(pxp->x, pxp->y, xr.x, xr.y - span, xr.x + xr.w, xr.y))
				return i;

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				bottom += (ptd->dock[i].cy + span);
			else
				bottom += ptd->dock[i].cy;
		}
	}

	return -1;
}

void _frame_calc_dock_rect(res_win_t widget, dword_t style, xrect_t* pxr)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);

	xrect_t xr, xr_cli;
	long top, bottom, left, right, span;
	int i;

	if (!ptd)
		return;

	xmem_zero((void*)pxr, sizeof(xrect_t));

#ifdef XDC_SUPPORT_WIDGET_EX
	if (widget_is_minimized(widget))
		return;
#endif

	widget_get_client_rect(widget, &xr_cli);

	span = FRAME_SPLIT_SPAN;
	top = bottom = left = right = 0;

	for (i = 0; i < 4; i++)
	{
		if (ptd->dock[i].style & DOCK_AT_LEFT)
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

			if (ptd->dock[i].style & DOCK_IS_DYNA)
				left += (ptd->dock[i].cx + span);
			else
				left += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & DOCK_AT_TOP)
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

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				top += (ptd->dock[i].cy + span);
			else
				top += ptd->dock[i].cy;
		}
		else if (ptd->dock[i].style & DOCK_AT_RIGHT)
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

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				right += (ptd->dock[i].cx + span);
			else
				right += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & DOCK_AT_BOTTOM)
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

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
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
int hand_frame_create(res_win_t widget, void* data)
{
	frame_delta_t* ptd;
	res_dc_t rdc;
	XHANDLE canv;

	ptd = (frame_delta_t*)xmem_alloc(sizeof(frame_delta_t));
	xmem_zero((void*)ptd, sizeof(frame_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_attach_canvas(widget, canv);

	ptd->org_ind = -1;

	SETFRAMEDELTA(widget, ptd);

	return 0;
}

void hand_frame_destroy(res_win_t widget)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	destroy_display_canvas(widget_detach_canvas(widget));

	SETFRAMEDELTA(widget, 0);

	xmem_free(ptd);
}

void hand_frame_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);
	int hint;

	if (ptd->b_size)
		return;

	hint = _frame_calc_hint(widget, pxp);

	if (hint >= 0)
		widget_set_cursor(widget, CURS_HAND);
}

void hand_frame_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);

	ptd->org_ind = _frame_calc_hint(widget, pxp);

	if (ptd->org_ind >= 0)
	{
		ptd->b_size = 1;
		ptd->org_x = pxp->x;
		ptd->org_y = pxp->y;

		switch (ptd->dock[ptd->org_ind].style & 0x0000FFFF)
		{
		case DOCK_AT_LEFT:
		case DOCK_AT_RIGHT:
			widget_set_cursor(widget, CURS_SIZEWE);
			break;
		case DOCK_AT_TOP:
		case DOCK_AT_BOTTOM:
			widget_set_cursor(widget, CURS_SIZENS);
			break;
		}

		widget_set_capture(widget, 1);
	}

}

void hand_frame_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);
	xrect_t xr;
	long span = FRAME_SPLIT_SPAN;

	if (ptd->b_size)
	{
		widget_set_capture(widget, 0);

		switch (ptd->dock[ptd->org_ind].style & 0x0000FFFF)
		{
		case DOCK_AT_LEFT:
			ptd->dock[ptd->org_ind].cx += (pxp->x - ptd->org_x);
			break;
		case DOCK_AT_RIGHT:
			ptd->dock[ptd->org_ind].cx -= (pxp->x - ptd->org_x);
			break;
		case DOCK_AT_TOP:
			ptd->dock[ptd->org_ind].cy += (pxp->y - ptd->org_y);
			break;
		case DOCK_AT_BOTTOM:
			ptd->dock[ptd->org_ind].cy -= (pxp->y - ptd->org_y);
			break;
		}

		widget_get_client_rect(widget, &xr);

		if (ptd->dock[ptd->org_ind].cx < 0)
			ptd->dock[ptd->org_ind].cx = 0;
		if (ptd->dock[ptd->org_ind].cx > xr.w - span)
			ptd->dock[ptd->org_ind].cx = xr.w - span;

		if (ptd->dock[ptd->org_ind].cy < 0)
			ptd->dock[ptd->org_ind].cy = 0;
		if (ptd->dock[ptd->org_ind].cy > xr.h - span)
			ptd->dock[ptd->org_ind].cy = xr.h - span;

		ptd->b_size = 0;
		ptd->org_x = pxp->x;
		ptd->org_y = pxp->y;
		ptd->org_ind = -1;

		widget_update_client(widget);
	}
}

void hand_frame_size(res_win_t widget, int code, const xsize_t* pxs)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);
	xsize_t xs;

	if (code == RESIZE_MAXIMIZED)
	{
		get_desktop_size(&xs);
		widget_size(widget, &xs);
		widget_update_window(widget);
		return;
	}
	else if (code == RESIZE_MINIMIZED)
	{
		return;
	}

	widget_invalid(widget, NULL, 0);
}

void hand_frame_erase(res_win_t widget, res_dc_t dc)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);

}

void hand_frame_paint(res_win_t widget, res_dc_t dc, const xrect_t* prt)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);
	res_dc_t rdc;
	XHANDLE canv;
	xrect_t xr, xr_cli, xr_bar;
	long top, bottom, left, right, span;
	xbrush_t xb = { 0 };
	xgradi_t gi = { 0 };
	xcolor_t xc = { 0 };
	int i;

	widget_get_xbrush(widget, &xb);
	parse_xcolor(&xc, xb.color);

	format_xcolor(&xc, gi.brim_color);
	lighten_xcolor(&xc, DEF_SOFT_DARKEN);
	format_xcolor(&xc, gi.core_color);

	widget_get_client_rect(widget, &xr_cli);

	canv = widget_get_canvas(widget);

	rdc = begin_display_paint(canv, dc, xr_cli.w, xr_cli.h);

	draw_rect_raw(rdc, NULL, &xb, &xr_cli);

	span = FRAME_SPLIT_SPAN;
	top = bottom = left = right = 0;

	for (i = 0; i < 4; i++)
	{
		if (ptd->dock[i].style & DOCK_AT_LEFT)
		{
			xr.x = xr_cli.x;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
			{
				xr_bar.x = xr.x + xr.w;
				xr_bar.w = span;
				xr_bar.y = xr.y;
				xr_bar.h = xr.h;
				xscpy(gi.type, GDI_ATTR_GRADIENT_TYPE_VERT);
				gradient_rect_raw(rdc, &gi, &xr_bar);
			}

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				left += (ptd->dock[i].cx + span);
			else
				left += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & DOCK_AT_TOP)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y;
			xr.h = ptd->dock[i].cy;

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
			{
				xr_bar.x = xr.x;
				xr_bar.w = xr.w;
				xr_bar.y = xr.y + xr.h;
				xr_bar.h = span;
				xscpy(gi.type, GDI_ATTR_GRADIENT_TYPE_HORZ);
				gradient_rect_raw(rdc, &gi, &xr_bar);
			}

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				top += (ptd->dock[i].cy + span);
			else
				top += ptd->dock[i].cy;
		}
		else if (ptd->dock[i].style & DOCK_AT_RIGHT)
		{
			xr.x = xr_cli.x + xr_cli.w - ptd->dock[i].cx;
			xr.w = ptd->dock[i].cx;
			xr.y = xr_cli.y + top;
			xr.h = xr_cli.h - top - bottom;

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
			{
				xr_bar.x = xr.x - span;
				xr_bar.w = span;
				xr_bar.y = xr.y;
				xr_bar.h = xr.h;
				xscpy(gi.type, GDI_ATTR_GRADIENT_TYPE_VERT);
				gradient_rect_raw(rdc, &gi, &xr_bar);
			}

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				right += (ptd->dock[i].cx + span);
			else
				right += ptd->dock[i].cx;
		}
		else if (ptd->dock[i].style & DOCK_AT_BOTTOM)
		{
			xr.x = xr_cli.x + left;
			xr.w = xr_cli.w - left - right;
			xr.y = xr_cli.y + xr_cli.h - ptd->dock[i].cy;
			xr.h = ptd->dock[i].cy;

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
			{
				xr_bar.x = xr.x;
				xr_bar.w = xr.w;
				xr_bar.y = xr.y - span;
				xr_bar.h = span;
				xscpy(gi.type, GDI_ATTR_GRADIENT_TYPE_HORZ);
				gradient_rect_raw(rdc, &gi, &xr_bar);
			}

			if ((ptd->dock[i].style & DOCK_IS_DYNA))
				bottom += (ptd->dock[i].cy + span);
			else
				bottom += ptd->dock[i].cy;
		}
	}

	end_display_paint(canv, dc, prt);
}

/*************************************************************************************************************/
res_win_t frame_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_frame_create;
	ev.pf_on_destroy = hand_frame_destroy;

	ev.pf_on_erase = hand_frame_erase;
	ev.pf_on_paint = hand_frame_paint;
	ev.pf_on_size = hand_frame_size;
	ev.pf_on_mouse_move = hand_frame_mouse_move;
	ev.pf_on_lbutton_down = hand_frame_lbutton_down;
	ev.pf_on_lbutton_up = hand_frame_lbutton_up;

	return widget_create(wname, wstyle, pxr, NULL, &ev);
}

bool_t frame_dock(res_win_t widget, dword_t style, long cx, long cy)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);
	int i;

	XDC_ASSERT(ptd != NULL);

	for (i = 0; i < 4; i++)
	{
		if (!ptd->dock[i].style)
			break;

		if ((ptd->dock[i].style & 0x0000ffff) == (style & 0x0000ffff))
			return 0;
	}

	if (i == 4)
		return 0;

	ptd->dock[i].style = style;
	ptd->dock[i].cx = cx;
	ptd->dock[i].cy = cy;

	return 1;
}

void frame_undock(res_win_t widget, dword_t style)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);
	int i;
	
	XDC_ASSERT(ptd != NULL);

	for (i = 0; i < 4; i++)
	{
		if ((ptd->dock[i].style & 0x0000ffff) == (style & 0x0000ffff))
			break;
	}

	if (i == 4)
		return;

	for (; i < 3; i++)
	{
		ptd->dock[i].style = ptd->dock[i + 1].style;
		ptd->dock[i].cx = ptd->dock[i + 1].cx;
		ptd->dock[i].cy = ptd->dock[i + 1].cy;

		ptd->dock[i+1].style = 0;
		ptd->dock[i+1].cx = 0;
		ptd->dock[i+1].cy = 0;
	}
}

void frame_get_dock_rect(res_win_t widget, dword_t style, xrect_t* pxr)
{
	frame_delta_t* ptd = GETFRAMEDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	_frame_calc_dock_rect(widget, style, pxr);
}
