/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dialog control document

	@module	dialogctrl.c | implement file

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
#include "xdcimp.h"

#define DIALOG_LINE_FEED		(float)50
#define DIALOG_ITEM_MIN_WIDTH	(float)10
#define DIALOG_ITEM_MIN_HEIGHT	(float)10

typedef struct _dialog_delta_t{
	link_t_ptr dialog;
	link_t_ptr item;
	link_t_ptr hover;

	int org_hint;
	int org_x, org_y;
	int cur_x, cur_y;
	short cur_page;

	int opera;

	bool_t b_drag;
	bool_t b_size;

	link_t_ptr stack;
}dialog_delta_t;

#define GETDIALOGDELTA(ph) 	(dialog_delta_t*)widget_get_user_delta(ph)
#define SETDIALOGDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/******************************************dialog event********************************************************/
static void _dialogctrl_done(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->dialog);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->dialog, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->dialog, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->dialog, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->dialog, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _dialogctrl_undo(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->dialog);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_dialog_doc(ptd->dialog);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->dialog, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->dialog, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		dialogctrl_redraw(widget);
	}
}

static void _dialogctrl_discard(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _dialogctrl_clean(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _dialogctrl_copy(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->dialog);

	if (!get_dialog_item_selected_count(ptd->dialog))
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_dialog_doc();
	ilk = get_dialog_next_item(ptd->dialog, LINK_FIRST);
	while (ilk)
	{
		if (get_field_selected(ilk))
		{
			nlk = insert_dialog_item(dom, get_dialog_item_class_ptr(ilk));
			copy_dom_node(nlk, ilk);
		}

		ilk = get_dialog_next_item(ptd->dialog, ilk);
	}

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, DEF_MBS);
#endif

	gb = gmem_alloc(len + sizeof(tchar_t));
	buf = (byte_t*)gmem_lock(gb);

#ifdef _UNICODE
	format_dom_doc_to_bytes(dom, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(dom, buf, len, DEF_MBS);
#endif

	gmem_unlock(gb);

	if (!clipboard_put(DEF_CB_FORMAT, gb))
	{
		gmem_free(gb);
		clipboard_close();

		destroy_dialog_doc(dom);
		return 0;
	}

	clipboard_close();

	destroy_dialog_doc(dom);
	return 1;
}

static bool_t _dialogctrl_cut(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->dialog);

	if (!_dialogctrl_copy(widget))
		return 0;

	ilk = get_dialog_next_item(ptd->dialog, LINK_FIRST);
	while (ilk)
	{
		nxt = get_dialog_next_item(ptd->dialog, ilk);

		if (get_dialog_item_selected(ilk))
		{
			delete_dialog_item(ilk);
		}
		ilk = nxt;
	}

	dialogctrl_redraw(widget);

	return 1;
}

static bool_t _dialogctrl_paste(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk;

	float y;
	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->dialog);

	if (!clipboard_open())
		return 0;

	gb = clipboard_get(DEF_CB_FORMAT);
	if (!gb)
	{
		clipboard_close();
		return 0;
	}

	len = gmem_size(gb);
	buf = (byte_t*)gmem_lock(gb);

	dom = create_dom_doc();
#ifdef _UNICODE
	if (!parse_dom_doc_from_bytes(dom, buf, len, DEF_UCS))
#else
	if (!parse_dom_doc_from_bytes(dom, buf, len, DEF_MBS))
#endif
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	if (!is_dialog_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	while (nlk = get_dialog_next_item(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_dialog_itemset(dom), nlk);
		attach_dom_node(get_dialog_itemset(ptd->dialog), LINK_LAST, nlk);

		y = get_dialog_item_y(nlk);
		y += get_dialog_item_height(nlk);
		set_dialog_item_y(nlk, y);

		xsprintf(sz_name, _T("%s%d"), get_dialog_item_class_ptr(nlk), get_dialog_item_count_by_class(ptd->dialog, get_dialog_item_class_ptr(nlk)));
		set_dialog_item_name(nlk, sz_name);
	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);
	dialogctrl_redraw(widget);

	return 1;
}

static void _dialogctrl_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	calc_dialog_item_rect(ptd->dialog, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _dialogctrl_reset_page(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	int pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_dialog_width(ptd->dialog);
	xs.fy = get_dialog_height(ptd->dialog);

	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = (float)10;
	xs.fy = (float)10;
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

static void _dialogctrl_ensure_visible(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	xrect_t xr = { 0 };

	if (!ptd->item)
		return;

	_dialogctrl_item_rect(widget, ptd->item, &xr);

	widget_ensure_visible(widget, &xr, 1);
}
/*********************************************************************************************************/
int noti_dialog_owner(res_win_t widget, unsigned int code, link_t_ptr ptr, link_t_ptr ilk, void* data)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	NOTICE_DIALOG nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;

	nf.dialog = ptr;
	nf.item = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_dialog_reset_select(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	link_t_ptr ilk;
	int count = 0;

	ilk = get_dialog_next_item(ptd->dialog, LINK_FIRST);
	while (ilk)
	{
		if (get_dialog_item_selected(ilk))
		{
			set_dialog_item_selected(ilk, 0);
			noti_dialog_owner(widget, NC_DIALOGITEMSELECTED, ptd->dialog, ilk, NULL);

			count++;
		}

		ilk = get_dialog_next_item(ptd->dialog, ilk);
	}

	if (count)
	{
		widget_redraw(widget, NULL, 0);
	}
}

void noti_dialog_item_selected(res_win_t widget, link_t_ptr ilk)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	xrect_t xr;
	bool_t b_check;

	b_check = get_dialog_item_selected(ilk);

	if (b_check)
		set_dialog_item_selected(ilk, 0);
	else
		set_dialog_item_selected(ilk, 1);

	noti_dialog_owner(widget, NC_DIALOGITEMSELECTED, ptd->dialog, ilk, NULL);

	_dialogctrl_item_rect(widget, ilk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

bool_t noti_dialog_item_changing(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	if (noti_dialog_owner(widget, NC_DIALOGITEMCHANGING, ptd->dialog, ptd->item, NULL))
		return (bool_t)0;

	_dialogctrl_item_rect(widget, ptd->item, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->item = NULL;

	widget_redraw(widget, &xr, 0);

	return (bool_t)1;
}

void noti_dialog_item_changed(res_win_t widget, link_t_ptr ilk)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->item);

	ptd->item = ilk;

	_dialogctrl_item_rect(widget, ptd->item, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	noti_dialog_owner(widget, NC_DIALOGITEMCHANGED, ptd->dialog, ilk, NULL);
}

void noti_dialog_item_enter(res_win_t widget, link_t_ptr ilk)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = ilk;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
}

void noti_dialog_item_leave(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
}

void noti_dialog_item_hover(res_win_t widget, int x, int y)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->hover != NULL);

	pt.x = x;
	pt.y = y;
	noti_dialog_owner(widget, NC_DIALOGITEMHOVER, ptd->dialog, ptd->hover, (void*)&pt);
}

void noti_dialog_item_drag(res_win_t widget, int x, int y)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item);

	ptd->b_drag = (bool_t)1;
	ptd->org_x = x;
	ptd->org_y = y;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_dialog_owner(widget, NC_DIALOGITEMDRAG, ptd->dialog, ptd->item, (void*)&pt);
}

void noti_dialog_item_drop(res_win_t widget, int x, int y)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	
	xpoint_t pt;
	xrect_t xr;
	int cx, cy;

	XDL_ASSERT(ptd->item);

	ptd->cur_x = x;
	ptd->cur_y = y;

	ptd->b_drag = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	cx = x - ptd->org_x;
	cy = y - ptd->org_y;

	if (!cx && !cy)
		return;

	calc_dialog_item_rect(ptd->dialog, ptd->item, &xr);

	widget_rect_to_pt(widget, &xr);

	if (xr.x + cx < 0 || xr.y + cy < 0)
		return;

	_dialogctrl_done(widget);

	pt.x = xr.x + cx;
	pt.y = xr.y + cy;

	widget_point_to_tm(widget, &pt);

	pt.fx = (float)((int)(pt.fx));
	pt.fy = (float)((int)(pt.fy));

	set_dialog_item_x(ptd->item, pt.fx);
	set_dialog_item_y(ptd->item, pt.fy);

	widget_redraw(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_dialog_owner(widget, NC_DIALOGITEMDROP, ptd->dialog, ptd->item, (void*)&pt);
}

void noti_dialog_item_sizing(res_win_t widget, int hint, int x, int y)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	if (hint == DIALOG_HINT_HORZ_SPLIT)
	{
		widget_set_cursor(widget,CURSOR_SIZENS);
	}
	else if (hint == DIALOG_HINT_VERT_SPLIT)
	{
		widget_set_cursor(widget,CURSOR_SIZEWE);
	}
	else
	{
		widget_set_cursor(widget,CURSOR_SIZEALL);
	}

	ptd->org_hint = hint;
	ptd->org_x = x;
	ptd->org_y = y;

	ptd->b_size = (bool_t)1;

	_dialogctrl_item_rect(widget, ptd->item, &xr);

	noti_dialog_owner(widget, NC_DIALOGITEMSIZING, ptd->dialog, ptd->item, (void*)&xr);
}

void noti_dialog_item_sized(res_win_t widget, int x, int y)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	float minw, minh, fw, fh;
	int hint;
	xrect_t xr;
	xsize_t xs;

	XDL_ASSERT(ptd->item);

	ptd->cur_x = x;
	ptd->cur_y = y;

	ptd->b_size = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	hint = ptd->org_hint;

	minw = DIALOG_ITEM_MIN_WIDTH;
	minh = DIALOG_ITEM_MIN_HEIGHT;

	xs.cx = ptd->cur_x - ptd->org_x;
	xs.cy = ptd->cur_y - ptd->org_y;

	if (!xs.cx && !xs.cy)
		return;

	widget_size_to_tm(widget, &xs);

	_dialogctrl_item_rect(widget, ptd->item, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	fw = get_dialog_item_width(ptd->item);
	fh = get_dialog_item_height(ptd->item);

	fw += xs.fx;
	fh += xs.fy;

	if (fw < minw)
		fw = minw;

	if (fh < minh)
		fh = minh;

	fw = (float)((int)fw);
	fh = (float)((int)fh);

	_dialogctrl_done(widget);

	if (hint == DIALOG_HINT_HORZ_SPLIT)
	{
		set_dialog_item_height(ptd->item, fh);
	}
	else if (hint == DIALOG_HINT_VERT_SPLIT)
	{
		set_dialog_item_width(ptd->item, fw);
	}
	else
	{
		set_dialog_item_width(ptd->item, fw);
		set_dialog_item_height(ptd->item, fh);
	}

	_dialogctrl_item_rect(widget, ptd->item, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	_dialogctrl_item_rect(widget, ptd->item, &xr);

	noti_dialog_owner(widget, NC_DIALOGITEMSIZED, ptd->dialog, ptd->item, (void*)&xr);
}

void noti_dialog_calc(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
}

/*******************************************************************************/
int hand_dialog_create(res_win_t widget, void* data)
{
	dialog_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (dialog_delta_t*)xmem_alloc(sizeof(dialog_delta_t));
	xmem_zero((void*)ptd, sizeof(dialog_delta_t));

	ptd->stack = create_stack_table();

	SETDIALOGDELTA(widget, ptd);

	return 0;
}

void hand_dialog_destroy(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	_dialogctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETDIALOGDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_dialog_size(res_win_t widget, int code, const xsize_t* prs)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;

	dialogctrl_redraw(widget);
}

void hand_dialog_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_dialog_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	xpoint_t pt;

	if (!ptd->dialog)
		return;

	if (ptd->b_size || ptd->b_drag)
	{
		ptd->cur_x = pxp->x;
		ptd->cur_y = pxp->y;
		widget_redraw(widget, NULL, 0);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_dialog_hint(ptd->dialog, &pt, &ilk);

	if (nHint == DIALOG_HINT_HORZ_SPLIT && ilk == ptd->item && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_dialog_item_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZENS);
	}
	else if (nHint == DIALOG_HINT_VERT_SPLIT && ilk == ptd->item && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_dialog_item_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZEWE);
	}
	else if (nHint == DIALOG_HINT_CROSS_SPLIT && ilk == ptd->item && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_dialog_item_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZEALL);
	}
	else if (nHint == DIALOG_HINT_ITEM && ilk == ptd->item && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_dialog_item_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == DIALOG_HINT_NONE)
	{
		if (ptd->opera == WD_OPERA_CONTROL)
			widget_set_cursor(widget, CURSOR_IBEAM);
		else
			widget_set_cursor(widget, CURSOR_ARROW);
	}

	if (widget_is_hotvoer(widget))
	{
		if (nHint == DIALOG_HINT_ITEM && !ptd->hover && ilk)
		{
			noti_dialog_item_enter(widget, ilk);
			return;
		}

		if (nHint == DIALOG_HINT_ITEM && ptd->hover && ptd->hover != ilk)
		{
			noti_dialog_item_leave(widget);
			return;
		}

		if (nHint != DIALOG_HINT_ITEM && ptd->hover)
		{
			noti_dialog_item_leave(widget);
		}
	}
}

void hand_dialog_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;

	if (ptd->hover)
		noti_dialog_item_hover(widget, pxp->x, pxp->y);
}

void hand_dialog_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;

	if (ptd->hover)
		noti_dialog_item_leave(widget);
}

void hand_dialog_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->dialog)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_dialog_hint(ptd->dialog, &pt, &ilk);
	bRe = (ilk == ptd->item) ? 1 : 0;

	switch (nHint)
	{
	case DIALOG_HINT_ITEM:
		if (ptd->opera == WD_OPERA_CONTROL || widget_key_state(widget, KEY_CONTROL))
		{
			noti_dialog_item_selected(widget, ilk);
		}
		break;
	case DIALOG_HINT_NONE:
		noti_dialog_reset_select(widget);
		break;
	}
}

void hand_dialog_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->dialog)
		return;

	if (ptd->b_size)
	{
		noti_dialog_item_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag)
	{
		noti_dialog_item_drop(widget, pxp->x, pxp->y);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_dialog_hint(ptd->dialog, &pt, &ilk);

	bRe = (ilk == ptd->item) ? 1 : 0;

	if (ptd->item && !bRe)
	{
		if (!noti_dialog_item_changing(widget))
			bRe = 1;
	}

	if (ilk && !bRe)
	{
		noti_dialog_item_changed(widget, ilk);
	}

	noti_dialog_owner(widget, NC_DIALOGLBCLK, ptd->dialog, ptd->item, (void*)pxp);
}

void hand_dialog_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;

	noti_dialog_owner(widget, NC_DIALOGDBCLK, ptd->dialog, ptd->item, (void*)pxp);
}

void hand_dialog_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;
}

void hand_dialog_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;

	noti_dialog_owner(widget, NC_DIALOGRBCLK, ptd->dialog, ptd->item, (void*)pxp);
}

void hand_dialog_keydown(res_win_t widget, int nKey)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	float x, y, w, h, m;
	bool_t ks;
	link_t_ptr ilk;

	if (!ptd->dialog)
		return;

	if (!ptd->item)
		return;

	if ((nKey == KEY_UP || nKey == KEY_DOWN || nKey == KEY_LEFT || nKey == KEY_RIGHT))
	{
		ks = widget_key_state(widget, KEY_SHIFT);
		m = 1;

		if (ks)
			noti_dialog_owner(widget, NC_DIALOGITEMSIZING, ptd->dialog, ptd->item, NULL);
		else
			noti_dialog_owner(widget, NC_DIALOGITEMDRAG, ptd->dialog, ptd->item, NULL);

		_dialogctrl_done(widget);

		ilk = get_dialog_next_item(ptd->dialog, LINK_FIRST);
		while (ilk)
		{
			if (ilk != ptd->item && !get_dialog_item_selected(ilk))
			{
				ilk = get_dialog_next_item(ptd->dialog, ilk);
				continue;
			}

			x = get_dialog_item_x(ilk);
			y = get_dialog_item_y(ilk);
			w = get_dialog_item_width(ilk);
			h = get_dialog_item_height(ilk);

			switch (nKey)
			{
			case KEY_DOWN:
				if (ks)
					h += m;
				else
					y += m;
				break;
			case KEY_UP:
				if (ks)
					h = (h - m < 0) ? h : h - m;
				else
					y = (y - m < 0) ? y : y - m;
				break;
			case KEY_LEFT:
				if (ks)
					w = (w - m < 0) ? w : w - m;
				else
					x = (x - m < 0) ? x : x - m;
				break;
			case KEY_RIGHT:
				if (ks)
					w += m;
				else
					x += m;
				break;
			}

			set_dialog_item_x(ilk, x);
			set_dialog_item_y(ilk, y);
			set_dialog_item_width(ilk, w);
			set_dialog_item_height(ilk, h);

			ilk = get_dialog_next_item(ptd->dialog, ilk);
		}

		widget_redraw(widget, NULL, 0);

		if (ks)
			noti_dialog_owner(widget, NC_DIALOGITEMSIZED, ptd->dialog, ptd->item, NULL);
		else
			noti_dialog_owner(widget, NC_DIALOGITEMDROP, ptd->dialog, ptd->item, NULL);
	}
	else if ((nKey == _T('z') || nKey == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
	{
		_dialogctrl_undo(widget);
	}
	else if ((nKey == _T('c') || nKey == _T('C')) && widget_key_state(widget, KEY_CONTROL))
	{
		_dialogctrl_copy(widget);
	}
	else if ((nKey == _T('x') || nKey == _T('X')) && widget_key_state(widget, KEY_CONTROL))
	{
		_dialogctrl_done(widget);

		if (!_dialogctrl_cut(widget))
		{
			_dialogctrl_discard(widget);
		}
	}
	else if ((nKey == _T('v') || nKey == _T('V')) && widget_key_state(widget, KEY_CONTROL))
	{
		_dialogctrl_done(widget);

		if (!_dialogctrl_paste(widget))
		{
			_dialogctrl_discard(widget);
		}
	}
}

void hand_dialog_char(res_win_t widget, tchar_t nChar)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;
}

void hand_dialog_notice(res_win_t widget, NOTICE* pnt)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;
}


void hand_dialog_erase(res_win_t widget, res_ctx_t dc)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	if (!ptd->dialog)
		return;
}

void hand_dialog_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	xrect_t xr = { 0 };
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	res_ctx_t rdc;
	link_t_ptr ilk;

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->dialog)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);

	parse_xcolor(&pif->clr_bkg, xb.color);
	parse_xcolor(&pif->clr_frg, xp.color);
	parse_xcolor(&pif->clr_txt, xf.color);
	widget_get_mask(widget, &pif->clr_msk);
	widget_get_iconic(widget, &pif->clr_ico);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	widget_get_xbrush(widget, &xb);

	widget_get_xpen(widget, &xp);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_dialog(pif, &cb, ptd->dialog);

	//draw focus
	if (ptd->item)
	{
		_dialogctrl_item_rect(widget, ptd->item, &xr);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);

		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	//draw check
	parse_xcolor(&xc, DEF_ALPHA_COLOR);

	ilk = get_dialog_next_item(ptd->dialog, LINK_FIRST);
	while (ilk)
	{
		if (get_dialog_item_selected(ilk))
		{
			_dialogctrl_item_rect(widget, ilk, &xr);
			alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
		}
		ilk = get_dialog_next_item(ptd->dialog, ilk);
	}

	if (ptd->b_drag)
	{
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		_dialogctrl_item_rect(widget, ptd->item, &xr);

		xr.x += (ptd->cur_x - ptd->org_x);
		xr.y += (ptd->cur_y - ptd->org_y);

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}
	else if (ptd->b_size)
	{
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		_dialogctrl_item_rect(widget, ptd->item, &xr);

		if (ptd->org_hint == DIALOG_HINT_VERT_SPLIT)
		{
			xr.w = (ptd->cur_x - xr.x);
		}
		else if (ptd->org_hint == DIALOG_HINT_HORZ_SPLIT)
		{
			xr.h = (ptd->cur_y - xr.y);
		}
		else
		{
			xr.w = (ptd->cur_x - xr.x);
			xr.h = (ptd->cur_y - xr.y);
		}

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/***********************************************function********************************************************/

res_win_t dialogctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_dialog_create)
		EVENT_ON_DESTROY(hand_dialog_destroy)

		EVENT_ON_ERASE(hand_dialog_erase)
		EVENT_ON_PAINT(hand_dialog_paint)

		EVENT_ON_SIZE(hand_dialog_size)

		EVENT_ON_SCROLL(hand_dialog_scroll)

		EVENT_ON_KEYDOWN(hand_dialog_keydown)
		EVENT_ON_CHAR(hand_dialog_char)

		EVENT_ON_MOUSE_MOVE(hand_dialog_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_dialog_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_dialog_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_dialog_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_dialog_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_dialog_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_dialog_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_dialog_rbutton_up)

		EVENT_ON_NOTICE(hand_dialog_notice)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void dialogctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_dialog_doc(ptr));

	ptd->dialog = ptr;
	ptd->item = NULL;

	dialogctrl_redraw(widget);
}

link_t_ptr dialogctrl_detach(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->dialog;
	ptd->dialog = NULL;
	ptd->item = NULL;

	widget_redraw(widget, NULL, 0);

	return data;
}

link_t_ptr dialogctrl_fetch(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->dialog;
}

void dialogctrl_redraw(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->dialog)
		return;

	b_valid = 0;
	ilk = get_dialog_next_item(ptd->dialog, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		noti_dialog_owner(widget, NC_DIALOGITEMCALCED, ptd->dialog, ilk, NULL);

		ilk = get_dialog_next_item(ptd->dialog, ilk);
	}
	
	noti_dialog_owner(widget, NC_DIALOGCALCED, ptd->dialog, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}
	ptd->hover = NULL;

	_dialogctrl_reset_page(widget);

	widget_update(widget);
}

void dialogctrl_redraw_item(res_win_t widget, link_t_ptr ilk)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->dialog)
		return;

#ifdef _DEBUG
	if(!is_dialog_item(ptd->dialog, ilk))
		return;
#endif

	noti_dialog_owner(widget, NC_DIALOGITEMCALCED, ptd->dialog, ilk, NULL);

	_dialogctrl_item_rect(widget, ilk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

void dialogctrl_tabskip(res_win_t widget, int nSkip)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	link_t_ptr ilk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->dialog)
		return;

	switch (nSkip)
	{
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (ptd->item == NULL)
			ilk = get_dialog_prev_item(ptd->dialog, LINK_LAST);
		else
			ilk = get_dialog_prev_item(ptd->dialog, ptd->item);
		break;
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (ptd->item == NULL)
			ilk = get_dialog_next_item(ptd->dialog, LINK_FIRST);
		else
			ilk = get_dialog_next_item(ptd->dialog, ptd->item);
		break;
	case WD_TAB_HOME:
		ilk = get_dialog_next_item(ptd->dialog, LINK_FIRST);
		break;
	case WD_TAB_END:
		ilk = get_dialog_prev_item(ptd->dialog, LINK_LAST);
		break;
	}

	dialogctrl_set_focus_item(widget, ilk);
}

bool_t dialogctrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->dialog)
		return 0;

	if (ilk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_dialog_item(ptd->dialog, ilk));
#endif
	}

	bRe = (ilk == ptd->item) ? (bool_t)1 : (bool_t)0;
	if (bRe)
		return (bool_t)1;

	if (ptd->item && !bRe)
	{
		if (!noti_dialog_item_changing(widget))
			return (bool_t)0;
	}

	if (ilk && !bRe)
	{
		noti_dialog_item_changed(widget, ilk);

		_dialogctrl_ensure_visible(widget);
	}

	return (bool_t)1;
}

link_t_ptr dialogctrl_get_focus_item(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->dialog)
		return NULL;

	return ptd->item;
}

void dialogctrl_set_opera(res_win_t widget, int opera)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->opera = opera;
}

int dialogctrl_get_opera(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->opera;
}

void dialogctrl_get_dialog_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->dialog)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_dialog_item(ptd->dialog, ilk));
#endif

	_dialogctrl_item_rect(widget, ilk, pxr);
}

bool_t dialogctrl_get_dirty(res_win_t widget)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->dialog)
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void dialogctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	dialog_delta_t* ptd = GETDIALOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->dialog)
		return;

	if (bDirty)
		_dialogctrl_done(widget);
	else
		_dialogctrl_clean(widget);
}
