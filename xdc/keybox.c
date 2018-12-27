/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc key control document

	@module	keybox.c | key control widnows implement file

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

#include "xdcbox.h"
#include "handler.h"
#include "winnc.h"

typedef struct _keybox_delta_t{
	int index;
	int ca;
	long bw, bh;
	long org_x, org_y;
}keybox_delta_t;


#define KEYBOX_COUNT	46
#define KEYBOX_ROWS		5
#define KEYBOX_COLS		10

#define _HCA		0
#define _LCA		1
#define _SCA		2

#define KEY_CLOSE	0x05
#define KEY_MOVE	0x06

static tchar_t KEYBOX_LCA[] = _T("1234567890qwertyuiop\0asdfghjkl\x1zxcv bnm\b\x2\a\r\f\v\n");
static tchar_t KEYBOX_HCA[] = _T("1234567890QWERTYUIOP\0ASDFGHJKL\x1ZXCV BNM\b\x2\a\r\f\v\n");
static tchar_t KEYBOX_SCA[] = _T("1234567890~!@#$%^-*_\0()={}[]:;\x3\'\"<> ,.?\b\x4\a\r\f\v\n");

typedef struct _KEYBOX_RECT{
	long x, y, w, h;
}KEYBOX_RECT;

static KEYBOX_RECT KEYBOX_POS[KEYBOX_COUNT] = {
	{ 0, 1, 1, 1 },
	{ 1, 1, 1, 1 },
	{ 2, 1, 1, 1 },
	{ 3, 1, 1, 1 },
	{ 4, 1, 1, 1 },
	{ 5, 1, 1, 1 },
	{ 6, 1, 1, 1 },
	{ 7, 1, 1, 1 },
	{ 8, 1, 1, 1 },
	{ 9, 1, 1, 1 },

	{ 0, 2, 1, 1 },
	{ 1, 2, 1, 1 },
	{ 2, 2, 1, 1 },
	{ 3, 2, 1, 1 },
	{ 4, 2, 1, 1 },
	{ 5, 2, 1, 1 },
	{ 6, 2, 1, 1 },
	{ 7, 2, 1, 1 },
	{ 8, 2, 1, 1 },
	{ 9, 2, 1, 1 },

	{ 0, 3, 1, 1 },
	{ 1, 3, 1, 1 },
	{ 2, 3, 1, 1 },
	{ 3, 3, 1, 1 },
	{ 4, 3, 1, 1 },
	{ 5, 3, 1, 1 },
	{ 6, 3, 1, 1 },
	{ 7, 3, 1, 1 },
	{ 8, 3, 1, 1 },
	{ 9, 3, 1, 1 },

	{ 0, 4, 1, 1 },
	{ 1, 4, 1, 1 },
	{ 2, 4, 1, 1 },
	{ 3, 4, 1, 1 },
	{ 4, 4, 1, 1 },
	{ 5, 4, 1, 1 },
	{ 6, 4, 1, 1 },
	{ 7, 4, 1, 1 },
	{ 8, 4, 1, 1 },
	{ 9, 4, 1, 1 },

	{ 0, 5, 1, 1 },
	{ 1, 5, 2, 1 },
	{ 3, 5, 2, 1 },
	{ 5, 5, 2, 1 },
	{ 7, 5, 2, 1 },
	{ 9, 5, 1, 1 },
};


#define GETKEYBOXDELTA(ph) 	(keybox_delta_t*)widget_get_user_delta(ph)
#define SETKEYBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/***********************************************************************************/
static int _keybox_calc_hint(res_win_t widget, long x, long y, tchar_t* pch)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);
	xpoint_t xp;
	xrect_t xr;
	int i;

	xp.x = x;
	xp.y = y;

	widget_get_client_rect(widget, &xr);

	xr.x = xr.w - ptd->bw;
	xr.w = ptd->bw;
	xr.y = 0;
	xr.h = ptd->bh;
	if (pt_in_rect(&xp, &xr))
	{
		*pch = KEY_CLOSE;
		return KEYBOX_COUNT;
	}

	xr.w = xr.x - ptd->bw;
	xr.x = ptd->bw;
	xr.y = 0;
	xr.h = ptd->bh;
	if (pt_in_rect(&xp, &xr))
	{
		*pch = KEY_MOVE;
		return KEYBOX_COUNT;
	}

	xr.x = 0;
	xr.w = ptd->bw;
	xr.y = 0;
	xr.h = ptd->bh;
	if (pt_in_rect(&xp, &xr))
	{
		*pch = KEY_ESC;
		return KEYBOX_COUNT;
	}

	for (i = 0; i < KEYBOX_COUNT; i++)
	{
		xr.x = ptd->bw * KEYBOX_POS[i].x;
		xr.w = ptd->bw * KEYBOX_POS[i].w;
		xr.y = ptd->bh * KEYBOX_POS[i].y;
		xr.h = ptd->bh * KEYBOX_POS[i].h;

		if (pt_in_rect(&xp, &xr))
		{
			switch (ptd->ca)
			{
			case _HCA:
				*pch = KEYBOX_HCA[i];
				break;
			case _LCA:
				*pch = KEYBOX_LCA[i];
				break;
			case _SCA:
				*pch = KEYBOX_SCA[i];
				break;
			}

			return i;
		}
	}

	*pch = 0;
	return -1;
}

/***********************************************************************************/
int hand_keybox_create(res_win_t widget, void* data)
{
	keybox_delta_t* ptd;
	xsize_t xs;

	widget_hand_create(widget);

	ptd = (keybox_delta_t*)xmem_alloc(sizeof(keybox_delta_t));
	xmem_zero((void*)ptd, sizeof(keybox_delta_t));

	xs.fx = DEF_TOUCH_SPAN;
	xs.fy = DEF_TOUCH_SPAN;

	widget_size_to_pt(widget, &xs);

	ptd->bw = xs.cx;
	ptd->bh = xs.cy;

	ptd->index = -1;

	SETKEYBOXDELTA(widget, ptd);

	return 0;
}

void hand_keybox_destroy(res_win_t widget)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETKEYBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_keybox_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);
	int hint;
	tchar_t ch = 0;
	long cx, cy;
	xrect_t xr;

	hint = _keybox_calc_hint(widget, pxp->x, pxp->y, &ch);
	if (hint < 0)
		return;

	if (ch == KEY_MOVE && (dw & MS_WITH_LBUTTON))
	{
		widget_set_cursor(widget, CURSOR_HAND);

		cx = pxp->x - ptd->org_x;
		cy = pxp->y - ptd->org_y;
		if (!cx && !cy)
			return;

		widget_get_window_rect(widget, &xr);
		xr.x += cx;
		xr.y += cy;
		widget_move(widget, RECTPOINT(&xr));
	}
	else
	{
		widget_set_cursor(widget, CURSOR_ARROW);
	}
}

void hand_keybox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);
	tchar_t ch = 0;

	ptd->index = _keybox_calc_hint(widget, pxp->x, pxp->y, &ch);

	if (ptd->index < 0)
		return;

	if (ch == KEY_MOVE)
	{
		ptd->org_x = pxp->x;
		ptd->org_y = pxp->y;
		widget_set_cursor(widget, CURSOR_HAND);
		return;
	}

	if (ch == KEY_ESC)
	{
		widget_post_key(NULL, KEY_ESC);
	}
	else if (ch == _T('\b'))
	{
		widget_post_key(NULL, KEY_BACK);
	}
	else if (ch == _T('\n'))
	{
		widget_post_key(NULL, KEY_ENTER);
	}
	else if (ch == _T('\0'))
	{
		switch (ptd->ca)
		{
		case _HCA:
			ptd->ca = _LCA;
			break;
		case _LCA:
			ptd->ca = _SCA;
			break;
		case _SCA:
			ptd->ca = _HCA;
		}

		widget_invalid(widget, NULL, 1);
	}
	else if (ch == 0x1)
	{
		widget_post_key(NULL, KEY_UP);
	}
	else if (ch == 0x3)
	{
		widget_post_key(NULL, KEY_LEFT);
	}
	else if (ch == KEY_ESC)
	{
		widget_post_key(NULL, KEY_LEFT);
	}
	else if (ch == _T('\a'))
	{
		widget_copy(NULL);
	}
	else if (ch == _T('\r'))
	{
		widget_cut(NULL);
	}
	else if (ch == _T('\f'))
	{
		widget_paste(NULL);
	}
	else if (ch == _T('\v'))
	{
		widget_undo(NULL);
	}
	else if (ch == 0x2)
	{
		widget_post_key(NULL, KEY_DOWN);
	}
	else if (ch == 0x4)
	{
		widget_post_key(NULL, KEY_RIGHT);
	}
	else if (ch && ch != 0x5)
	{
		widget_post_char(NULL, ch);
	}

	widget_invalid(widget, NULL, 0);
}

void hand_keybox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);
	int hint;
	tchar_t ch = 0;

	hint = _keybox_calc_hint(widget, pxp->x, pxp->y, &ch);
	if (hint < 0)
		return;

	if (ch == 0x5)
	{
		widget_close(widget, 0);
		return;
	}

	ptd->index = -1;
	widget_invalid(widget, NULL, 0);
}

void hand_keybox_size(res_win_t widget, int code, const xsize_t* prs)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);
	
	widget_invalid(widget, NULL, 0);
}

void hand_keybox_erase(res_win_t widget, res_ctx_t dc)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);
}

void hand_keybox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);
	canvas_t canv;
	res_ctx_t rdc;
	xbrush_t xb,xb_bark, xb_focus;
	xrect_t xr, xr_focus;
	xfont_t xf;
	xface_t xa;
	int i;
	tchar_t tk[2] = { 0 };

	widget_get_xbrush(widget, &xb);
	xmem_copy((void*)&xb_focus, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_focus, DEF_SOFT_LIGHTEN);

	xmem_copy((void*)&xb_bark, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_bark, DEF_SOFT_DARKEN);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	canv = widget_get_canvas(widget);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb_bark, &xr);

	xr.x = xr.w - ptd->bw;
	xr.w = ptd->bw;
	xr.y = 0;
	xr.h = ptd->bh;
	draw_text_raw(rdc, &xf, &xa, &xr, _T("×"), -1);

	xr.x = 0;
	xr.w = ptd->bw;
	xr.y = 0;
	xr.h = ptd->bh;
	draw_text_raw(rdc, &xf, &xa, &xr, _T("Esc"), -1);

	for (i = 0; i < KEYBOX_COUNT; i++)
	{
		xr.x = ptd->bw * KEYBOX_POS[i].x;
		xr.w = ptd->bw * KEYBOX_POS[i].w;
		xr.y = ptd->bh * KEYBOX_POS[i].y;
		xr.h = ptd->bh * KEYBOX_POS[i].h;

		xr_focus.x = xr.x + 2;
		xr_focus.y = xr.y + 2;
		xr_focus.w = xr.w - 4;
		xr_focus.h = xr.h - 4;

		if (ptd->index == i)
			draw_rect_raw(rdc, NULL, &xb_focus, &xr_focus);
		else
			draw_rect_raw(rdc, NULL, &xb, &xr_focus);

		if (ptd->ca == _HCA)
			tk[0] = KEYBOX_HCA[i];
		else if (ptd->ca == _LCA)
			tk[0] = KEYBOX_LCA[i];
		else if (ptd->ca == _SCA)
			tk[0] = KEYBOX_SCA[i];

		if (tk[0] == _T('\n'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("√"), -1);
		else if (tk[0] == _T('\b'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("CE"), -1);
		else if (tk[0] == _T('\0'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("FN"), -1);
		else if (tk[0] == 0x1)
			draw_text_raw(rdc, &xf, &xa, &xr, _T("↑"), -1);
		else if (tk[0] == 0x3)
			draw_text_raw(rdc, &xf, &xa, &xr, _T("←"), -1);
		else if (tk[0] == _T('\a'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("复制"), -1);
		else if (tk[0] == _T('\r'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("剪切"), -1);
		else if (tk[0] == _T('\f'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("粘贴"), -1);
		else if (tk[0] == _T('\v'))
			draw_text_raw(rdc, &xf, &xa, &xr, _T("撤销"), -1);
		else if (tk[0] == 0x2)
			draw_text_raw(rdc, &xf, &xa, &xr, _T("↓"), -1);
		else if (tk[0] == 0x4)
			draw_text_raw(rdc, &xf, &xa, &xr, _T("→"), -1);
		else
			draw_text_raw(rdc, &xf, &xa, &xr, tk, -1);
	}

	end_canvas_paint(canv, dc, pxr);
}

/**********************************************************************************************/
res_win_t keybox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_keybox_create)
		EVENT_ON_DESTROY(hand_keybox_destroy)

		EVENT_ON_ERASE(hand_keybox_erase)
		EVENT_ON_PAINT(hand_keybox_paint)

		EVENT_ON_SIZE(hand_keybox_size)

		EVENT_ON_MOUSE_MOVE(hand_keybox_mouse_move)

		EVENT_ON_LBUTTON_DOWN(hand_keybox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_keybox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void keybox_popup_size(res_win_t widget, xsize_t* pxs)
{
	keybox_delta_t* ptd = GETKEYBOXDELTA(widget);

	pxs->cx = ptd->bw * KEYBOX_COLS;
	pxs->cy = ptd->bh * (KEYBOX_ROWS + 1);
	
	widget_adjust_size(widget_get_style(widget), pxs);
}

res_win_t show_keybox(const xpoint_t* ppt)
{
	res_win_t wt;
	xrect_t xr = { 0 };
	xsize_t xs = { 0 };

	wt = keybox_create(NULL, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_NOACTIVE, &xr);
	if (!wt)
		return NULL;

	keybox_popup_size(wt, RECTSIZE(&xr));

	if (ppt)
	{
		xr.x = ppt->x;
		xr.y = ppt->y;
	}
	else
	{
		get_desktop_size(&xs);
		xr.x = xs.cx - xr.w - 1;
		xr.y = xs.cy - xr.h;
	}

	widget_move(wt, RECTPOINT(&xr));
	widget_size(wt, RECTSIZE(&xr));
	widget_take(wt, (int)WD_ZORDER_TOPMOST);
	widget_show(wt, WD_SHOW_NORMAL);

	return wt;
}

