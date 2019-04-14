/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc notes control documilk

	@module	notesctrl.c | notes control widnows implemilk file

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

#include "xdcctrl.h"
#include "handler.h"
#include "winnc.h"

typedef struct _notes_delta_t{
	link_t_ptr arch;
	link_t_ptr item;
	link_t_ptr hover;
	long tw, th;

	bool_t b_delete;
}notes_delta_t;

#define GETNOTESDELTA(ph) 	(notes_delta_t*)widget_get_user_delta(ph)
#define SETNOTESDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

#define NOTESCTRL_GUID_SPAN		(float)6
#define NOTESCTRL_SPAN_PLUS		(long)10

typedef enum{
	_NOTES_HINT_NONE = 0,
	_NOTES_HINT_TITLE = 1,
	_NOTES_HINT_ITEM = 2,
	_NOTES_HINT_TIME = 3,
	_NOTES_HINT_CLOSE = 5
}NOTES_HINT;
/***************************************************************************************/

static long _notesctrl_calc_width(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	link_t_ptr ilk, doc;
	xfont_t xf;
	xface_t xa;
	res_ctx_t rdc;
	xsize_t xs;
	long pw;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);

	rdc = widget_client_ctx(widget);

	pw = 0;
	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		XDL_ASSERT(is_notes_doc(doc));

		if (compare_text(get_notes_type_ptr(doc),-1,ATTR_NOTES_TEXT,-1,0) == 0)
		{
			text_size_raw(rdc, &xf, get_notes_text_ptr(doc), -1, &xs);
			if (pw < xs.cx)
				pw = xs.cx;
		}
		else
		{
			if (pw < ptd->tw * NOTESCTRL_SPAN_PLUS)
				pw = ptd->tw * NOTESCTRL_SPAN_PLUS;
		}

		ilk = get_arch_next_sibling_item(ilk);
	}

	widget_release_ctx(widget, rdc);

	return pw + ptd->tw + ptd->th;
}

static long _notesctrl_calc_height(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	link_t_ptr ilk,doc;
	xfont_t xf;
	xface_t xa;
	res_ctx_t rdc;
	xrect_t xr;
	long pw,ph;
	int n;

	widget_get_client_rect(widget, &xr);
	pw = xr.w - ptd->tw;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);

	rdc = widget_client_ctx(widget);

	ph = 0;
	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		XDL_ASSERT(is_notes_doc(doc));

		if (compare_text(get_notes_type_ptr(doc), -1, ATTR_NOTES_TEXT, -1, 0) == 0)
		{
			xr.x = xr.y = 0;
			xr.w = pw;
			xr.h = ptd->tw;
			text_rect_raw(rdc, &xf, &xa, get_notes_text_ptr(doc), -1, &xr);

			n = xr.h / ptd->tw;
			if (xr.h % ptd->tw)
				n++;
		}
		else
		{
			n = NOTESCTRL_SPAN_PLUS;
		}

		ph += (ptd->th + n * ptd->tw);

		ilk = get_arch_next_sibling_item(ilk);
	}

	widget_release_ctx(widget, rdc);

	return ph;
}

static int _notesctrl_calc_hint(res_win_t widget, const xpoint_t* ppt, link_t_ptr* pplk)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	res_ctx_t rdc;
	link_t_ptr ilk,doc;
	int hint, n;
	long total = 0;

	xrect_t xr;
	xfont_t xf;
	xface_t xa;
	viewbox_t vb;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);

	widget_get_view_rect(widget, &vb);

	rdc = widget_client_ctx(widget);

	*pplk = NULL;
	hint = _NOTES_HINT_NONE;

	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		XDL_ASSERT(is_notes_doc(doc));

		xr.x = vb.px + vb.pw - ptd->th;
		xr.y = vb.py + total;
		xr.w = ptd->th;
		xr.h = ptd->th;
		if (pt_in_rect(ppt, &xr))
		{
			*pplk = ilk;
			hint = _NOTES_HINT_CLOSE;
			break;
		}

		xr.x = vb.px;
		xr.y = vb.py + total;
		xr.w = ptd->tw;
		xr.h = ptd->th;
		if (pt_in_rect(ppt, &xr))
		{
			*pplk = ilk;
			hint = _NOTES_HINT_TIME;
			break;
		}

		xr.x = vb.px + ptd->tw;
		xr.y = vb.py + total;
		xr.w = vb.pw - ptd->tw;
		xr.h = ptd->th;
		if (pt_in_rect(ppt, &xr))
		{
			*pplk = ilk;
			hint = _NOTES_HINT_TITLE;
			break;
		}

		if (compare_text(get_notes_type_ptr(doc), -1, ATTR_NOTES_TEXT, -1, 0) == 0)
		{
			xr.x = xr.y = 0;
			xr.w = vb.pw - ptd->tw;
			xr.h = ptd->tw;
			text_rect_raw(rdc, &xf, &xa, get_notes_text_ptr(doc), -1, &xr);

			n = xr.h / ptd->tw;
			if (xr.h % ptd->tw)
				n++;
		}
		else
		{
			n = NOTESCTRL_SPAN_PLUS;
		}

		xr.x = vb.px + ptd->tw;
		xr.y = vb.py + total + ptd->th;
		xr.w = vb.pw - ptd->tw;
		xr.h = n * ptd->tw;
		if (pt_in_rect(ppt, &xr))
		{
			*pplk = ilk;
			hint = _NOTES_HINT_ITEM;
			break;
		}

		total += (ptd->th + n * ptd->tw);

		ilk = get_arch_next_sibling_item(ilk);
	}

	widget_release_ctx(widget, rdc);

	return hint;
}

static void _notesctrl_item_rect(res_win_t widget, link_t_ptr plk, xrect_t* pxr)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	res_ctx_t rdc;
	link_t_ptr ilk,doc;

	xrect_t xr;
	xfont_t xf;
	xface_t xa;
	viewbox_t vb;
	long n,total = 0;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);

	widget_get_view_rect(widget, &vb);

	rdc = widget_client_ctx(widget);

	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		XDL_ASSERT(is_notes_doc(doc));

		if (compare_text(get_notes_type_ptr(doc), -1, ATTR_NOTES_TEXT, -1, 0) == 0)
		{
			xr.x = xr.y = 0;
			xr.w = vb.pw - ptd->tw;
			xr.h = ptd->tw;
			text_rect_raw(rdc, &xf, &xa, get_notes_text_ptr(doc), -1, &xr);

			n = xr.h / ptd->tw;
			if (xr.h % ptd->tw)
				n++;
		}
		else
		{
			n = NOTESCTRL_SPAN_PLUS;
		}

		if (ilk == plk)
		{
			pxr->x = vb.px;
			pxr->y = vb.py + total;
			pxr->w = vb.pw;
			pxr->h = ptd->th + n * ptd->tw;
			break;
		}

		total += (ptd->th + n * ptd->tw);

		ilk = get_arch_next_sibling_item(ilk);
	}

	widget_release_ctx(widget, rdc);
}

static void _notesctrl_reset_page(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	xrect_t xr;
	long mh;

	mh = _notesctrl_calc_height(widget);

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, xr.w, mh, ptd->th, ptd->th);
}

static void _notesctrl_ensure_visible(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	xrect_t xr;

	if (!ptd->item)
		return;

	_notesctrl_item_rect(widget, ptd->item, &xr);

	widget_ensure_visible(widget, &xr, 1);
}

/*************************************************************************/
int noti_notes_owner(res_win_t widget, unsigned long code, link_t_ptr arch, link_t_ptr item, void* data)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	NOTICE_NOTES nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.arch = arch;
	nf.item = item;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

bool_t noti_notes_item_changing(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	if (noti_notes_owner(widget, NC_NOTESITEMCHANGING, ptd->arch, ptd->item, NULL))
		return 0;

	_notesctrl_item_rect(widget,ptd->item, &xr);

	ptd->item = NULL;

	widget_update(widget, &xr, 0);

	return 1;
}

void noti_notes_item_changed(res_win_t widget, link_t_ptr elk)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = elk;

	_notesctrl_item_rect(widget, ptd->item, &xr);
	
	widget_update(widget, &xr, 0);
}

bool_t noti_notes_item_delete(res_win_t widget, link_t_ptr ilk)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (noti_notes_owner(widget, NC_NOTESITEMDELETE, ptd->arch, ilk, NULL))
		return 0;

	return 1;
}

void noti_notes_item_enter(res_win_t widget, link_t_ptr plk)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	XDL_ASSERT(plk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = plk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_notes_item_leave(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_notes_item_hover(res_win_t widget, long x, long y)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_notes_owner(widget, NC_NOTESITEMHOVER, ptd->arch, ptd->hover, (void*)&xp);
}
/********************************************************************************************/
int hand_notes_create(res_win_t widget, void* data)
{
	notes_delta_t* ptd;
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xsize_t xs;

	widget_hand_create(widget);

	ptd = (notes_delta_t*)xmem_alloc(sizeof(notes_delta_t));

	widget_get_xfont(widget, &xf);

	rdc = widget_client_ctx(widget);
	text_metric_raw(rdc, &xf, &xs);
	widget_release_ctx(widget, rdc);

	ptd->tw = (long)((float)xs.cx * 8);
	ptd->th = (long)((float)xs.cy * 1.25);

	SETNOTESDELTA(widget, ptd);

	return 0;
}

void hand_notes_destroy(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETNOTESDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_notes_keydown(res_win_t widget, int key)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;

	switch (key)
	{
	case KEY_ENTER:
		break;
	case KEY_SPACE:
		break;
	case KEY_LEFT:
		notesctrl_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		notesctrl_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_HOME:
		notesctrl_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		notesctrl_tabskip(widget,WD_TAB_END);
		break;
	}
}

void hand_notes_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	link_t_ptr plk = NULL;
	int hint;

	if (!ptd->arch)
		return;

	hint = _notesctrl_calc_hint(widget, pxp, &plk);

	if (!ptd->hover && plk)
	{
		noti_notes_item_enter(widget, plk);
	}
	else if (ptd->hover && ptd->hover != plk)
	{
		noti_notes_item_leave(widget);
	}
}

void hand_notes_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;

	if (ptd->hover)
		noti_notes_item_hover(widget, pxp->x, pxp->y);
}

void hand_notes_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;

	if (ptd->hover)
		noti_notes_item_leave(widget);
}

void hand_notes_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}
}

void hand_notes_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	link_t_ptr ilk = NULL;
	int hint;
	bool_t bRe;

	if (!ptd->arch)
		return;

	hint = _notesctrl_calc_hint(widget, pxp, &ilk);

	if (ptd->b_delete && hint == _NOTES_HINT_CLOSE)
	{
		if (noti_notes_item_delete(widget, ilk))
		{
			delete_arch_item(ilk);

			notesctrl_redraw(widget);
			return;
		}
	}

	bRe = (ilk == ptd->item) ? 1 : 0;

	if (!bRe && ptd->item)
	{
		if (!noti_notes_item_changing(widget))
			bRe = 1;
	}

	if (ilk && !bRe)
	{
		noti_notes_item_changed(widget, ilk);
	}

	noti_notes_owner(widget, NC_NOTESLBCLK, ptd->arch, ptd->item, (void*)pxp);
}

void hand_notes_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;

	noti_notes_owner(widget, NC_NOTESDBCLK, ptd->arch, ptd->item, (void*)pxp);
}

void hand_notes_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;
}

void hand_notes_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;

	noti_notes_owner(widget, NC_NOTESRBCLK, ptd->arch, ptd->item, (void*)pxp);
}

void hand_notes_size(res_win_t widget, int code, const xsize_t* prs)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;

	notesctrl_redraw(widget);
}

void hand_notes_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	if (!ptd->arch)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_notes_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->arch)
		return;

	widget_get_scroll(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widget_hand_scroll(widget, bHorz, nLine))
		return;

	win = widget_get_parent(widget);

	if (widget_is_valid(win))
	{
		widget_scroll(win, bHorz, nLine);
	}
}

void hand_notes_erase(res_win_t widget, res_ctx_t rdc)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	
	if (!ptd->arch)
		return;
}

void hand_notes_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	res_ctx_t rdc;
	canvas_t canv;

	link_t_ptr ilk,doc;
	xrect_t xr_btn,xr_txt,xr;
	xpoint_t pt_org, pt_cur;
	xsize_t xs;
	long n;

	viewbox_t vb = { 0 };
	xfont_t xf_top,xf = { 0 };
	xface_t xa_top,xa = { 0 };
	xbrush_t xb_bar, xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };

	xdate_t dt,td;
	tchar_t token[DATE_LEN + 1];

	if (!ptd->arch)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	parse_xcolor(&xc, xp.color);

	memcpy((void*)&xf_top, (void*)&xf, sizeof(xfont_t));
	xscpy(xf_top.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	memcpy((void*)&xa_top, (void*)&xa, sizeof(xface_t));

	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_NEAR);

	xscpy(xa_top.text_wrap, _T(""));

	memcpy((void*)&xb_bar, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_bar, DEF_SOFT_DARKEN);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_view_rect(widget, &vb);

	xr.x = vb.px;
	xr.y = vb.py;
	xr.w = vb.pw;

	get_loc_date(&td);

	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		xr_btn.x = xr.x;
		xr_btn.y = xr.y;
		xr_btn.w = vb.pw;
		xr_btn.h = ptd->th;

		draw_rect_raw(rdc, NULL, &xb_bar, &xr_btn);

		xr_btn.x = xr.x;
		xr_btn.y = xr.y;
		xr_btn.w = ptd->th;
		xr_btn.h = ptd->th;

		if (ilk == ptd->item)
		{
			pt_center_rect(&xr_btn, 12, 8);
			draw_icon_raw(rdc, &xc, &xr_btn, ATTR_ICON_GUIDER);
		}
		else
		{
			pt_center_rect(&xr_btn, 8, 8);
			draw_icon_raw(rdc, &xc, &xr_btn, ATTR_ICON_RADIOED);
		}

		if (!is_null(get_notes_time_ptr(doc)))
		{
			parse_datetime(&dt, get_notes_time_ptr(doc));
			if (compare_date(&dt, &td) == 0)
			{
				xsprintf(token, _T("今天 %02d:%02d"), dt.hour, dt.min);
			}
			else
			{
				xsprintf(token, _T("%d/%d %02d:%02d"), dt.day,dt.mon,dt.hour, dt.min);
			}

			text_size_raw(rdc, &xf_top, token, -1, &xs);

			xr_txt.x = xr.x + ptd->th;
			xr_txt.y = xr.y;
			xr_txt.w = vb.pw - 2 * ptd->th;
			xr_txt.h = ptd->th;

			draw_text_raw(rdc, &xf_top, &xa_top, &xr_txt, token, -1);
		}
		else
		{
			xsprintf(token, _T("今天 %02d:%02d"), td.hour, td.min);
			text_size_raw(rdc, &xf_top, token, -1, &xs);
		}

		if (compare_text(get_notes_type_ptr(doc), -1, ATTR_NOTES_TEXT, -1, 0) == 0)
		{
			xr_txt.x = xr.x + xs.cx + 2 * ptd->th;
			xr_txt.y = xr.y;
			xr_txt.w = vb.pw - xs.cx - 3 * ptd->th;
			xr_txt.h = ptd->th;

			draw_text_raw(rdc, &xf, &xa, &xr_txt, get_notes_text_ptr(doc), -1);
		}
		else
		{
			xr_btn.x = xr.x + ptd->th;
			xr_btn.y = xr.y + ptd->th;
			xr_btn.w = ptd->tw - ptd->th;
			xr_btn.h = NOTESCTRL_SPAN_PLUS * ptd->tw;

			pt_center_rect(&xr_btn, 8, 8);
			draw_icon_raw(rdc, &xc, &xr_btn, ATTR_ICON_FIXED);
		}

		pt_cur.x = xr.x + ptd->th / 2;
		pt_cur.y = xr.y + ptd->th / 2;

		if (!is_first_link(ilk))
		{
			draw_line_raw(rdc, &xp, &pt_cur, &pt_org);
		}

		if (ptd->b_delete)
		{
			xr_btn.x = xr.x + vb.pw - ptd->th;
			xr_btn.y = xr.y;
			xr_btn.w = ptd->th;
			xr_btn.h = ptd->th;

			pt_center_rect(&xr_btn, 8, 8);
			draw_icon_raw(rdc, &xc, &xr_btn, ATTR_ICON_CLOSE);
		}

		if (compare_text(get_notes_type_ptr(doc), -1, ATTR_NOTES_TEXT, -1, 0) == 0)
		{
			xr_txt.x = 0;
			xr_txt.y = 0;
			xr_txt.w = vb.pw - ptd->tw;
			xr_txt.h = ptd->tw;
			text_rect_raw(rdc, &xf, &xa, get_notes_text_ptr(doc), -1, &xr_txt);

			n = xr_txt.h / ptd->tw;
			if (xr_txt.h % ptd->tw)
				n++;
		}
		else
		{
			n = NOTESCTRL_SPAN_PLUS;
		}

		xr_txt.x = xr.x + ptd->tw;
		xr_txt.y = xr.y + ptd->th;
		xr_txt.w = vb.pw - ptd->tw;
		xr_txt.h = n * ptd->tw;

		draw_text_raw(rdc, &xf, &xa, &xr_txt, get_notes_text_ptr(doc), -1);

		pt_org.x = pt_cur.x;
		pt_org.y = pt_cur.y;

		xr.y += (ptd->th + n * ptd->tw);

		ilk = get_arch_next_sibling_item(ilk);
	}

	end_canvas_paint(canv, dc, pxr);
}

/************************************************************************************************/
res_win_t notesctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparilk)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_notes_create)
		EVENT_ON_DESTROY(hand_notes_destroy)

		EVENT_ON_ERASE(hand_notes_erase)
		EVENT_ON_PAINT(hand_notes_paint)

		EVENT_ON_SIZE(hand_notes_size)

		EVENT_ON_SCROLL(hand_notes_scroll)
		EVENT_ON_WHEEL(hand_notes_wheel)

		EVENT_ON_KEYDOWN(hand_notes_keydown)

		EVENT_ON_MOUSE_MOVE(hand_notes_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_notes_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_notes_mouse_leave)

		EVENT_ON_LBUTTON_DOWN(hand_notes_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_notes_lbutton_up)
		EVENT_ON_LBUTTON_DBCLICK(hand_notes_lbutton_dbclick)
		EVENT_ON_RBUTTON_DOWN(hand_notes_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_notes_rbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparilk, &ev);
}

void notesctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_arch_doc(ptr));

	ptd->arch = ptr;

	notesctrl_redraw(widget);
}

link_t_ptr notesctrl_detach(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->arch;
	ptd->arch = NULL;

	notesctrl_redraw(widget);

	return data;
}

link_t_ptr notesctrl_fetch(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->arch;
}

void notesctrl_enable_delete(res_win_t widget, bool_t bDelete)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_delete = bDelete;
}

void notesctrl_redraw(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	link_t_ptr ilk, doc;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return;

	b_valid = 0;
	ilk = get_arch_first_child_item(ptd->arch);
	while (ilk)
	{
		doc = fetch_arch_document(ilk);

		if (ilk == ptd->item)
			b_valid = 1;

		noti_notes_owner(widget, NC_NOTESITEMCALCED, ptd->arch, ilk, NULL);

		ilk = get_arch_next_sibling_item(ilk);
	}

	noti_notes_owner(widget, NC_NOTESCALCED, ptd->arch, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}

	_notesctrl_reset_page(widget);

	widget_update_window(widget);

	widget_update(widget, NULL, 0);
}

void notesctrl_redraw_item(res_win_t widget, link_t_ptr ilk)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return;

	noti_notes_owner(widget, NC_NOTESITEMCALCED, ptd->arch, ilk, NULL);

	_notesctrl_item_rect(widget, ilk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t notesctrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return 0;

	if (ilk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_arch_document(ptd->arch, ilk) || is_arch_catalog(ptd->arch, ilk));
#endif
	}

	bRe = (ilk == ptd->item) ? 1 : 0;

	if (!bRe && ptd->item)
	{
		if (!noti_notes_item_changing(widget))
			return 0;
	}

	if (!bRe && ilk)
	{
		noti_notes_item_changed(widget, ilk);

		_notesctrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr notesctrl_get_focus_item(res_win_t widget)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return NULL;

	return ptd->item;
}

void notesctrl_get_item_rect(res_win_t widget, link_t_ptr elk, xrect_t* prt)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return;

	_notesctrl_item_rect(widget, elk, prt);
}

void notesctrl_tabskip(res_win_t widget, int nSkip)
{
	notes_delta_t* ptd = GETNOTESDELTA(widget);
	link_t_ptr plk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arch)
		return;

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (ptd->item)
			plk = get_arch_next_sibling_item(ptd->item);
		else
			plk = get_arch_first_child_item(ptd->arch);

		if (plk)
			notesctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (ptd->item)
			plk = get_arch_prev_sibling_item(ptd->item);
		else
			plk = get_arch_last_child_item(ptd->arch);

		if (plk)
			notesctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_HOME:
		plk = get_arch_first_child_item(ptd->arch);

		if (plk)
			notesctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_END:
		plk = get_arch_last_child_item(ptd->arch);

		if (plk)
			notesctrl_set_focus_item(widget, plk);
		break;
	}
}
