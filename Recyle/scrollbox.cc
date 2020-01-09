/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc scroll control document

	@module	scrollbox.c | scroll control widnows implement file

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

#include "xdcbox.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcres.h"
#include "xdcgdi.h"
#include "xdccanv.h"

typedef enum{
	_HINT_SCROLL_NONE = 0,
	_HINT_SCROLL_NEXT = 1,
	_HINT_SCROLL_PREV = 2,
	_HINT_SCROLL_HOME = 3,
	_HINT_SCROLL_END = 4,
	_HINT_SCROLL_BAR = 5
}_HINT_SCROLL;

typedef struct _scrollbox_delta_t{
	long span;
	long orgx, orgy;
	bool_t b_drag;
	short max, min, pos;
	bool_t b_horz;
	bool_t b_page;
}scrollbox_delta_t;

#define GETSCROLLBOXDELTA(ph) 	(scrollbox_delta_t*)widget_get_user_delta(ph)
#define SETSCROLLBOXDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

int _scrollbox_calc_hint(res_win_t widget, long x, long y)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);
	xrect_t xr,xr_pos;
	xpoint_t xp;
	long n_pos;

	xp.x = x;
	xp.y = y;

	widget_get_client_rect(widget, &xr);

	ptd->span = (ptd->b_horz) ? xr.h : xr.w;

	if (ptd->b_horz)
	{
		xr_pos.x = xr.x;
		xr_pos.y = xr.y;
		xr_pos.w = ptd->span;
		xr_pos.h = xr.h;
		if (pt_in_rect(&xp, &xr_pos))
		{
			return (ptd->b_page)? _HINT_SCROLL_HOME : _HINT_SCROLL_PREV;
		}

		if (ptd->b_page)
		{
			xr_pos.x = xr.x + ptd->span;
			xr_pos.y = xr.y;
			xr_pos.w = ptd->span;
			xr_pos.h = xr.h;
			if (pt_in_rect(&xp, &xr_pos))
			{
				return _HINT_SCROLL_PREV;
			}
		}

		xr_pos.x = xr.x + xr.w - ptd->span;
		xr_pos.y = xr.y;
		xr_pos.w = ptd->span;
		xr_pos.h = xr.h;
		if (pt_in_rect(&xp, &xr_pos))
		{
			return (ptd->b_page) ? _HINT_SCROLL_END : _HINT_SCROLL_NEXT;
		}

		if (ptd->b_page)
		{
			xr_pos.x = xr.x + xr.w - 2 * ptd->span;
			xr_pos.y = xr.y;
			xr_pos.w = ptd->span;
			xr_pos.h = xr.h;
			if (pt_in_rect(&xp, &xr_pos))
			{
				return _HINT_SCROLL_NEXT;
			}
		}

		if (ptd->max > 0)
		{
			n_pos = (ptd->b_page) ? (xr.w - 4 * ptd->span) : (xr.w - 2 * ptd->span);
			n_pos = (long)((float)ptd->min / (float)ptd->max * (float)n_pos);

			xr_pos.x = xr.x + n_pos - ptd->span / 2;
			xr_pos.y = xr.y;
			xr_pos.w = ptd->span;
			xr_pos.h = xr.h;
			if (pt_in_rect(&xp, &xr_pos))
			{
				return _HINT_SCROLL_BAR;
			}
		}
	}
	else
	{
		xr_pos.x = xr.x;
		xr_pos.y = xr.y;
		xr_pos.w = xr.w;
		xr_pos.h = ptd->span;
		if (pt_in_rect(&xp, &xr_pos))
		{
			return (ptd->b_page) ? _HINT_SCROLL_HOME : _HINT_SCROLL_PREV;
		}

		if (ptd->b_page)
		{
			xr_pos.x = xr.x;
			xr_pos.y = xr.y + ptd->span;
			xr_pos.w = xr.w;
			xr_pos.h = ptd->span;
			if (pt_in_rect(&xp, &xr_pos))
			{
				return _HINT_SCROLL_PREV;
			}
		}

		xr_pos.x = xr.x;
		xr_pos.y = xr.y + xr.h - ptd->span;
		xr_pos.w = xr.w;
		xr_pos.h = ptd->span;
		if (pt_in_rect(&xp, &xr_pos))
		{
			return (ptd->b_page) ? _HINT_SCROLL_END : _HINT_SCROLL_NEXT;
		}

		if (ptd->b_page)
		{
			xr_pos.x = xr.x;
			xr_pos.y = xr.y + xr.h - 2 * ptd->span;
			xr_pos.w = xr.w;
			xr_pos.h = ptd->span;
			if (pt_in_rect(&xp, &xr_pos))
			{
				return _HINT_SCROLL_NEXT;
			}
		}

		if (ptd->max > 0)
		{
			n_pos = (ptd->b_page) ? (xr.h - 4 * ptd->span) : (xr.h - 2 * ptd->span);
			n_pos = (long)((float)ptd->min / (float)ptd->max * (float)n_pos);

			xr_pos.x = xr.x;
			xr_pos.y = xr.y + n_pos - ptd->span / 2;
			xr_pos.w = xr.w;
			xr_pos.h = ptd->span;
			if (pt_in_rect(&xp, &xr_pos))
			{
				return _HINT_SCROLL_BAR;
			}
		}
	}

	return _HINT_SCROLL_NONE;
}

int hand_scrollbox_create(res_win_t widget, void* data)
{
	scrollbox_delta_t* ptd;
	res_dc_t rdc;
	xsize_t xs;
	xhand_t canv;

	ptd = (scrollbox_delta_t*)xmem_alloc(sizeof(scrollbox_delta_t));
	xmem_zero((void*)ptd, sizeof(scrollbox_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_attach_canvas(widget, canv);

	xs.fx = DEF_TOUCH_SPAN;
	xs.fy = DEF_TOUCH_SPAN;

	screen_size_to_pt(&xs);

	

	SETSCROLLBOXDELTA(widget, ptd);

	return 0;
}

void hand_scrollbox_destroy(res_win_t widget)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);

	destroy_display_canvas(widget_detach_canvas(widget));

	SETSCROLLBOXDELTA(widget, 0);

	xmem_free(ptd);
}

void hand_scrollbox_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);
	int hint;

	if (ptd->b_drag)
	{
		widget_set_cursor(widget, CURS_HAND);
		return;
	}

	hint = _scrollbox_calc_hint(widget, pxp->x, pxp->y);

	if (hint == _HINT_SCROLL_BAR)
		widget_set_cursor(widget, CURS_HAND);
	else
		widget_set_cursor(widget, CURS_ARROW);
}

void hand_scrollbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);
	int hint;

	hint = _scrollbox_calc_hint(widget, pxp->x, pxp->y);

	if (hint == _HINT_SCROLL_BAR)
	{
		ptd->b_drag = 1;

		ptd->orgx = pxp->x;
		ptd->orgy = pxp->y;

		widget_set_capture(widget, 1);
	}

	widget_invalid(widget, NULL, 1);
}

void hand_scrollbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);
	long cx, cy;
	int hint;

	if (ptd->b_drag)
	{
		widget_set_capture(widget, 0);

		cx = pxp->x - ptd->orgx;
		cy = pxp->y - ptd->orgy;

		ptd->b_drag = 0;
	}
	else
	{
		hint = _scrollbox_calc_hint(widget, pxp->x, pxp->y);
		switch (hint)
		{
		case _HINT_SCROLL_HOME:
			//widget_post_key(NULL, KEY_ENTER);
			break;
		case _HINT_SCROLL_PREV:
			//widget_post_key(NULL, KEY_PAGEUP);
			break;
		case _HINT_SCROLL_NEXT:
			//widget_post_key(NULL, KEY_DOWN);
			break;
		case _HINT_SCROLL_END:
			//widget_post_key(NULL, KEY_HOME);
			break;
		}
		//widget_invalid(widget, NULL, 1);
	}
}

void hand_scrollbox_command_color(res_win_t widget, void* data)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);

	widget_set_color_mode(widget, (clr_mod_t*)data);
}

void hand_scrollbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	widget_invalid(widget, NULL, 0);
}

void hand_scrollbox_erase(res_win_t widget, res_dc_t dc)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);
}

void hand_scrollbox_paint(res_win_t widget, res_dc_t rdc, const xrect_t* pxr)
{
	scrollbox_delta_t* ptd = GETSCROLLBOXDELTA(widget);
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xrect_t xr, xr_pos;
	xbrush_t xb_bar, xb_pos;
	long n_pos, FEED = 1;

	widget_get_xbrush(widget, &xb);
	xmem_copy((void*)&xb_bar, (void*)&xb, sizeof(xbrush_t));
	xmem_copy((void*)&xb_pos, (void*)&xb, sizeof(xbrush_t));

	dword_t ws = widget_get_style(widget);

	if (ws & WD_STYLE_HSCROLL)
	{
		ptd->b_horz = 1;
		if (ws & WD_STYLE_HORZPAGE)
			ptd->b_page = 1;
	}
	else if (ws & WD_STYLE_VSCROLL)
	{
		ptd->b_horz = 0;
		if (ws & WD_STYLE_VERTPAGE)
			ptd->b_page = 1;
	}

	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	draw_rect_raw(rdc, NULL, &xb, &xr);
	
	widget_get_client_rect(widget, &xr);

	ptd->span = (ptd->b_horz) ? xr.h : xr.w;

	if (ptd->b_horz)
	{
		xr_pos.x = xr.x + FEED;
		xr_pos.y = xr.y + FEED;
		xr_pos.w = ptd->span - 2 * FEED;
		xr_pos.h = xr.h - 2 * FEED;

		draw_rect_raw(rdc, &xp, &xb_bar, &xr_pos);
		pt_center_rect(&xr_pos, 6, 8);
		draw_shape_raw(rdc, &xp, NULL, &xr_pos, ATTR_SHAPE_LEFTTRIANGLE);

		if (ptd->b_page)
		{
			xr_pos.x = xr.x + ptd->span + FEED;
			xr_pos.y = xr.y + FEED;
			xr_pos.w = ptd->span - 2 * FEED;
			xr_pos.h = xr.h - 2 * FEED;
			
			draw_rect_raw(rdc, &xp, &xb_bar, &xr_pos);
			pt_center_rect(&xr_pos, 6, 8);
			draw_shape_raw(rdc, &xp, NULL, &xr_pos, ATTR_SHAPE_LEFTTRIANGLE);
		}

		xr_pos.x = xr.x + xr.w - ptd->span + FEED;
		xr_pos.y = xr.y + FEED;
		xr_pos.w = ptd->span - 2 * FEED;
		xr_pos.h = xr.h - 2 * FEED;

		draw_rect_raw(rdc, &xp, &xb_bar, &xr_pos);
		pt_center_rect(&xr_pos, 6, 8);
		draw_shape_raw(rdc, &xp, NULL, &xr_pos, ATTR_SHAPE_RIGHTTRIANGLE);

		if (ptd->b_page)
		{
			xr_pos.x = xr.x + xr.w - 2 * ptd->span + FEED;
			xr_pos.y = xr.y + FEED;
			xr_pos.w = ptd->span - 2 * FEED;
			xr_pos.h = xr.h - 2 * FEED;

			draw_rect_raw(rdc, &xp, &xb_bar, &xr_pos);
			pt_center_rect(&xr_pos, 6, 8);
			draw_shape_raw(rdc, &xp, NULL, &xr_pos, ATTR_SHAPE_RIGHTTRIANGLE);
		}

		if (ptd->max > 0)
		{
			n_pos = (ptd->b_page) ? (xr.w - 4 * ptd->span) : (xr.w - 2 * ptd->span);
			n_pos = (long)((float)ptd->min / (float)ptd->max * (float)n_pos);

			xr_pos.x = xr.x + n_pos - ptd->span / 2 + FEED;
			xr_pos.y = xr.y + FEED;
			xr_pos.w = ptd->span - 2 * FEED;
			xr_pos.h = xr.h - 2 * FEED;

			draw_rect_raw(rdc, &xp, &xb_pos, &xr_pos);
		}
	}
	else
	{
		xr_pos.x = xr.x + FEED;
		xr_pos.y = xr.y + FEED;
		xr_pos.w = xr.w - 2 * FEED;
		xr_pos.h = ptd->span - 2 * FEED;

		draw_rect_raw(rdc, &xp, &xb_bar, &xr_pos);
		pt_center_rect(&xr_pos, 8, 6);
		draw_shape_raw(rdc, &xp, NULL, &xr_pos, ATTR_SHAPE_TOPTRIANGLE);

		if (ptd->b_page)
		{
			xr_pos.x = xr.x + FEED;
			xr_pos.y = xr.y + ptd->span - 2 * FEED;
			xr_pos.w = xr.w - 2 * FEED;
			xr_pos.h = ptd->span - 2 * FEED;

			draw_rect_raw(rdc, &xp, &xb_bar, &xr_pos);
			pt_center_rect(&xr_pos, 8, 6);
			draw_shape_raw(rdc, &xp, NULL, &xr_pos, ATTR_SHAPE_TOPTRIANGLE);
		}

		xr_pos.x = xr.x + FEED;
		xr_pos.y = xr.y + xr.h - ptd->span + FEED;
		xr_pos.w = xr.w - 2 * FEED;
		xr_pos.h = ptd->span - 2 * FEED;

		draw_rect_raw(rdc, &xp, &xb_bar, &xr_pos);
		pt_center_rect(&xr_pos, 8, 6);
		draw_shape_raw(rdc, &xp, NULL, &xr_pos, ATTR_SHAPE_BOTTOMTRIANGLE);

		if (ptd->b_page)
		{
			xr_pos.x = xr.x + FEED;
			xr_pos.y = xr.y + xr.h - 2 * ptd->span + FEED;
			xr_pos.w = xr.w - 2 * FEED;
			xr_pos.h = ptd->span - 2 * FEED;

			draw_rect_raw(rdc, &xp, &xb_bar, &xr_pos);
			pt_center_rect(&xr_pos, 8, 6);
			draw_shape_raw(rdc, &xp, NULL, &xr_pos, ATTR_SHAPE_BOTTOMTRIANGLE);
		}

		if (ptd->max > 0)
		{
			n_pos = (ptd->b_page) ? (xr.h - 4 * ptd->span) : (xr.h - 2 * ptd->span);
			n_pos = (long)((float)ptd->min / (float)ptd->max * (float)n_pos);

			xr_pos.x = xr.x + FEED;
			xr_pos.y = xr.y + n_pos - ptd->span / 2 + FEED;
			xr_pos.w = xr.w - 2 * FEED;
			xr_pos.h = ptd->span - 2 * FEED;

			draw_rect_raw(rdc, &xp, &xb_pos, &xr_pos);
		}
	}
}

/**************************************************************************************/
res_win_t scrollbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_scrollbox_create;
	ev.pf_on_destroy = hand_scrollbox_destroy;

	ev.pf_on_erase = hand_scrollbox_erase;
	ev.pf_on_paint = hand_scrollbox_paint;

	ev.pf_on_size = hand_scrollbox_size;

	ev.pf_on_mouse_move = hand_scrollbox_mouse_move;
	ev.pf_on_lbutton_down = hand_scrollbox_lbutton_down;
	ev.pf_on_lbutton_up = hand_scrollbox_lbutton_up;

	ev.pf_on_command_color = hand_scrollbox_command_color;

	return widget_create(NULL, style, pxr, widget, &ev);
}
