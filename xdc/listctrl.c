/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc list control document

	@module	listctrl.c | list control implement file

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
#include "xdcfire.h"
#include "xdcbox.h"

#define MAX_HELP	6

typedef struct _list_delta_t{
	link_t_ptr list;
	link_t_ptr parent;
	link_t_ptr item;
	link_t_ptr hover;

	res_win_t editor;

	bool_t b_drag;
	bool_t b_lock;

	tchar_t help[MAX_HELP + 1];
}list_delta_t;

#define GETLISTDELTA(widget) 	((list_delta_t*)widget_get_user_delta(widget))
#define SETLISTDELTA(widget,ptd) widget_set_user_delta(widget,(var_long)ptd)

/******************************************************************************************************/

static void _listctrl_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_list_item_rect(&cb, ptd->list, ptd->parent, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _listctrl_item_text_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_list_item_text_rect(&cb, ptd->list, ptd->parent, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _listctrl_reset_page(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;
	canvbox_t cb = { 0 };
	bool_t b_horz;

	b_horz = (compare_text(get_list_layer_ptr(ptd->list), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.cx = pw;
	xs.cy = ph;
	widget_size_to_tm(widget, &xs);

	cb.fw = xs.fx;
	cb.fh = xs.fy;
	if (b_horz)
	{
		xs.fx = calc_list_width(&cb, ptd->list, ptd->parent);
	}
	else
	{
		xs.fy = calc_list_height(&cb, ptd->list, ptd->parent);
	}
	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = get_list_item_width(ptd->list);
	xs.fy = get_list_item_height(ptd->list);
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	if (b_horz)
		widget_reset_scroll(widget, 1);
	else
		widget_reset_scroll(widget, 0);
}

void _listctrl_ensure_visible(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	xrect_t xr = { 0 };

	if (!ptd->item)
		return;

	_listctrl_item_rect(widget, ptd->item, &xr);
	
	widget_ensure_visible(widget, &xr, 1);
}

void _listctrl_find(res_win_t widget, link_t_ptr pos, const tchar_t* token)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	link_t_ptr elk;
	int tlen;
	tchar_t* help;
	int hlen;

	tlen = xslen(token);

	help = xsalloc(tlen + 1);

	if (pos == LINK_FIRST || pos == ptd->parent)
		elk = get_list_first_child_item(ptd->parent);
	else if (pos == LINK_LAST)
		elk = NULL;
	else
		elk = get_list_next_sibling_item(pos);

	while (elk)
	{
		if (xsnicmp(get_list_item_title_ptr(elk), token, tlen) == 0)
			break;

		hlen = xslen(get_list_item_title_ptr(elk));
		if (hlen)
		{
			tlen = help_code(get_list_item_title_ptr(elk), hlen, help, tlen);
			if (xsnicmp(help, token, tlen) == 0)
				break;
		}

		elk = get_list_next_sibling_item(elk);
	}

	xsfree(help);

	if (elk)
		listctrl_set_focus_item(widget, elk);
}

/************************************control event**********************************************/
int noti_list_owner(res_win_t widget, unsigned long code, link_t_ptr list, link_t_ptr ilk, void* data)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	NOTICE_LIST nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.list = list;
	nf.item = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_list_reset_check(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	link_t_ptr ilk;
	int count = 0;

	ilk = get_list_first_child_item(ptd->parent);
	while (ilk)
	{
		if (get_list_item_checked(ilk))
		{
			set_list_item_checked(ilk, 0);
			noti_list_owner(widget, NC_LISTITEMCHECKED, ptd->list, ilk, NULL);

			count++;
		}
		ilk = get_list_next_sibling_item(ilk);
	}

	if (!count)
		return;

	widget_update(widget, NULL, 0);
}

bool_t noti_list_item_changing(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	if (noti_list_owner(widget, NC_LISTITEMCHANGING, ptd->list, ptd->item, NULL))
		return 0;

	_listctrl_item_rect(widget, ptd->item, &xr);

	ptd->item = NULL;

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	return 1;
}

void noti_list_item_changed(res_win_t widget, link_t_ptr plk)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = plk;

	_listctrl_item_rect(widget, plk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	noti_list_owner(widget, NC_LISTITEMCHANGED, ptd->list, ptd->item, NULL);
}

void noti_list_item_enter(res_win_t widget, link_t_ptr plk)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(plk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = plk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_list_item_leave(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_list_item_hover(res_win_t widget, long x, long y)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_list_owner(widget, NC_LABELITEMHOVER, ptd->list, ptd->hover, (void*)&xp);
}

void noti_list_item_check(res_win_t widget, link_t_ptr plk)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	xrect_t xr;
	bool_t b_check;

	XDL_ASSERT(plk);

	b_check = get_list_item_checked(plk);
	set_list_item_checked(plk, ((b_check) ? 0 : 1));

	noti_list_owner(widget, NC_LISTITEMCHECKED, ptd->list, plk, NULL);

	_listctrl_item_rect(widget, plk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

void noti_list_item_collapse(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	link_t_ptr plk;

	XDL_ASSERT(ptd->parent);

	if (ptd->parent == ptd->list)
		return;

	if (ptd->item)
	{
		if (!noti_list_item_changing(widget))
			return;
	}

	plk = ptd->parent;
	ptd->parent = get_list_parent_item(plk);

	listctrl_redraw(widget);

	noti_list_owner(widget, NC_LISTITEMCOLLAPSE, ptd->list, plk, NULL);

	listctrl_set_focus_item(widget, plk);
}

void noti_list_item_expand(res_win_t widget, link_t_ptr plk)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(plk);

	if (ptd->item)
	{
		if (!noti_list_item_changing(widget))
			return;
	}

	ptd->parent = plk;

	listctrl_redraw(widget);

	noti_list_owner(widget, NC_LISTITEMEXPAND, ptd->list, plk, NULL);
}

void noti_list_item_drag(res_win_t widget, long x, long y)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item);

	ptd->b_drag = 1;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_list_owner(widget, NC_LISTITEMDRAG, ptd->list, ptd->item, (void*)&pt);
}

void noti_list_item_drop(res_win_t widget, long x, long y)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item);

	ptd->b_drag = 0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	pt.x = x;
	pt.y = y;
	noti_list_owner(widget, NC_LISTITEMDROP,ptd->list, ptd->item, (void*)&pt);
}

void noti_list_begin_edit(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	const tchar_t* text;
	xrect_t xr = { 0 };

	clr_mod_t ob = { 0 };

	XDL_ASSERT(ptd->item);
	XDL_ASSERT(!ptd->editor);

	if (ptd->b_lock)
		return;

	if (get_list_item_locked(ptd->item))
		return;

	widget_get_color_mode(widget, &ob);

	_listctrl_item_text_rect(widget, ptd->item, &xr);

	if (noti_list_owner(widget, NC_LISTITEMEDITING, ptd->list, ptd->item, NULL))
		return;

	ptd->editor = fireedit_create(widget, &xr);
	XDL_ASSERT(ptd->editor);
	widget_set_user_id(ptd->editor, IDC_FIREEDIT);
	widget_set_owner(ptd->editor, widget);

	widget_set_color_mode(ptd->editor, &ob);
	widget_show(ptd->editor, WD_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	text = get_list_item_title_ptr(ptd->item);
	editbox_set_text(ptd->editor, text);
	editbox_selectall(ptd->editor);
}

void noti_list_commit_edit(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	const tchar_t* text;
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->item);

	text = (tchar_t*)editbox_get_text_ptr(ptd->editor);

	if (!noti_list_owner(widget, NC_LISTITEMCOMMIT, ptd->list, ptd->item, (void*)text))
	{
		listctrl_set_item_title(widget, ptd->item, text);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_list_rollback_edit(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->item);

	noti_list_owner(widget, NC_LISTITEMROLLBACK, ptd->list, ptd->item, NULL);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_list_reset_editor(res_win_t widget, bool_t bCommit)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_list_commit_edit(widget);
		else
			noti_list_rollback_edit(widget);
	}
}

/********************************************************************************************************/

int hand_list_create(res_win_t widget, void* data)
{
	list_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (list_delta_t*)xmem_alloc(sizeof(list_delta_t));
	xmem_zero((void*)ptd, sizeof(list_delta_t));

	SETLISTDELTA(widget, ptd);

	ptd->b_lock = 1;

	return 0;
}

void hand_list_destroy(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_list_reset_editor(widget, 0);

	xmem_free(ptd);

	SETLISTDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_list_size(res_win_t widget, int code, const xsize_t* prs)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

	listctrl_redraw(widget);
}

void hand_list_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

	noti_list_reset_editor(widget, 1);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_list_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	int nHint;
	link_t_ptr plk;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->list)
		return;

	if (ptd->b_drag)
		return;

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_list_hint(&cb, &pt, ptd->list, ptd->parent, &plk);

	if (nHint == LIST_HINT_ITEM && plk == ptd->item && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_list_item_drag(widget, pxp->x, pxp->y);
			return;
		}
	}

	if (nHint == LIST_HINT_ITEM && !ptd->hover && plk)
	{
		noti_list_item_enter(widget, plk);
	}
	else if (nHint == LIST_HINT_ITEM && ptd->hover && ptd->hover != plk)
	{
		noti_list_item_leave(widget);
	}
	else if (nHint == LIST_HINT_NONE && ptd->hover)
	{
		noti_list_item_leave(widget);
	}
}

void hand_list_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

	if (ptd->hover)
		noti_list_item_hover(widget, pxp->x, pxp->y);
}

void hand_list_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

	if (ptd->hover)
		noti_list_item_leave(widget);
}

void hand_list_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	int nHint;
	link_t_ptr plk;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->list)
		return;

	if (widget_is_valid(ptd->editor))
		return;

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_list_hint(&cb, &pt, ptd->list, ptd->parent, &plk);

	if (nHint == LIST_HINT_ITEM && get_list_first_child_item(plk))
	{
		noti_list_item_expand(widget, plk);
	}
	else if (nHint == LIST_HINT_PARENT && ptd->parent && ptd->parent != ptd->list)
	{
		noti_list_item_collapse(widget);
	}

	noti_list_owner(widget, NC_LISTDBCLK, ptd->list, ptd->item, (void*)pxp);
}

void hand_list_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	int nHint;
	link_t_ptr plk;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->list)
		return;

	noti_list_reset_editor(widget, 1);

	xscpy(ptd->help, _T(""));

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_list_hint(&cb, &pt, ptd->list, ptd->parent, &plk);
	bRe = (plk == ptd->item) ? 1 : 0;

	if (nHint == LIST_HINT_CHECK)
	{
		noti_list_item_check(widget, plk);
		return;
	}
	else if (nHint == LIST_HINT_NONE)
	{
		if (!widget_key_state(widget, KEY_CONTROL))
		{
			noti_list_reset_check(widget);
		}
		return;
	}
}

void hand_list_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	int nHint;
	link_t_ptr plk;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->list)
		return;

	if (ptd->b_drag)
	{
		noti_list_item_drop(widget, pxp->x, pxp->y);
		return;
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_list_hint(&cb, &pt, ptd->list, ptd->parent, &plk);

	noti_list_owner(widget, NC_LISTLBCLK, ptd->list, plk, (void*)pxp);

	bRe = (plk == ptd->item) ? 1 : 0;

	if (bRe && ptd->item)
	{
		widget_post_key(widget, KEY_ENTER);
		return;
	}

	if (ptd->item && !bRe)
	{
		if (!noti_list_item_changing(widget))
			return;
	}

	if (plk && !bRe)
	{
		noti_list_item_changed(widget, plk);
	}

	if (!bRe && ptd->item)
	{
		_listctrl_ensure_visible(widget);
	}
}

void hand_list_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

	noti_list_reset_editor(widget, 1);

	xscpy(ptd->help, _T(""));
}

void hand_list_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

	noti_list_owner(widget, NC_LISTRBCLK, ptd->list, ptd->item, (void*)pxp);
}

void hand_list_keydown(res_win_t widget, int nKey)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

	switch (nKey)
	{
	case KEY_ENTER:
		if (ptd->item)
		{
			noti_list_begin_edit(widget);
		}
		break;
	case KEY_SPACE:
		if (ptd->item)
		{
			noti_list_item_check(widget, ptd->item);
		}
		break;
	case KEY_LEFT:
		listctrl_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		listctrl_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_HOME:
		listctrl_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		listctrl_tabskip(widget,WD_TAB_END);
		break;
	}
}

void hand_list_char(res_win_t widget, tchar_t ch)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

	if (ch == _T('\n') || ch == _T('\t') || ch == _T('\r'))
		return;

	if (IS_ASCII_CHAR(ch))
	{
		if (xslen(ptd->help) < MAX_HELP)
		{
			xsncat(ptd->help, &ch, 1);
			_listctrl_find(widget, ptd->item, ptd->help);
		}
	}
}

void hand_list_child_command(res_win_t widget, int code, var_long data)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_list_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_list_rollback_edit(widget);
		break;
	}
}

void hand_list_erase(res_win_t widget, res_ctx_t dc)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	if (!ptd->list)
		return;

}

void hand_list_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->list)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);

	parse_xcolor(&pif->clr_bkg, xb.color);
	parse_xcolor(&pif->clr_frg, xp.color);
	parse_xcolor(&pif->clr_txt, xf.color);
	widget_get_xcolor(widget, &pif->clr_msk);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_list_child(pif, &cb, ptd->list, ptd->parent);

	if (ptd->item)
	{
		_listctrl_item_rect(widget, ptd->item, &xr);

		pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

		parse_xcolor(&xc, DEF_FOCUS_COLOR);

		draw_focus_raw(rdc, &xc, &xr, ALPHA_TRANS);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/**************************************************************************************************/

res_win_t listctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_list_create)
		EVENT_ON_DESTROY(hand_list_destroy)

		EVENT_ON_ERASE(hand_list_erase)
		EVENT_ON_PAINT(hand_list_paint)

		EVENT_ON_SIZE(hand_list_size)

		EVENT_ON_SCROLL(hand_list_scroll)

		EVENT_ON_KEYDOWN(hand_list_keydown)
		EVENT_ON_CHAR(hand_list_char)

		EVENT_ON_MOUSE_MOVE(hand_list_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_list_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_list_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_list_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_list_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_list_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_list_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_list_rbutton_up)

		EVENT_ON_CHILD_COMMAND(hand_list_child_command)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void listctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_list_doc(ptr));

	noti_list_reset_editor(widget, 0);

	ptd->list = ptr;
	ptd->parent = ptr;
	listctrl_redraw(widget);
}

link_t_ptr listctrl_detach(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	noti_list_reset_editor(widget, 0);

	if (ptd->list)
	{
		data = ptd->list;
		ptd->list = NULL;
		ptd->parent = NULL;
	}
	else
	{
		data = NULL;
	}
	return data;
}

link_t_ptr listctrl_fetch(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->list;
}

void listctrl_accept(res_win_t widget, bool_t bAccept)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return;

	noti_list_reset_editor(widget, bAccept);
}

void listctrl_redraw(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return;

	noti_list_reset_editor(widget, 0);

	b_valid = 0;
	ilk = get_list_first_child_item(ptd->parent);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		noti_list_owner(widget, NC_LISTITEMCALCED, ptd->parent, ilk, NULL);

		ilk = get_list_next_sibling_item(ilk);
	}

	noti_list_owner(widget, NC_LISTCALCED, ptd->parent, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}
	ptd->hover = NULL;

	_listctrl_reset_page(widget);

	widget_update_window(widget);
	
	widget_update(widget, NULL, 0);
}

void listctrl_tabskip(res_win_t widget, int nSkip)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	link_t_ptr plk = ptd->item;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return;

	noti_list_reset_editor(widget, 1);

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (plk == NULL)
			plk = get_list_first_child_item(ptd->parent);
		else
			plk = get_list_next_sibling_item(ptd->item);

		if (plk)
			listctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (plk == NULL)
			plk = get_list_last_child_item(ptd->parent);
		else
			plk = get_list_prev_sibling_item(ptd->item);

		if (plk)
			listctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_HOME:
		plk = get_list_first_child_item(ptd->parent);

		if (plk)
			listctrl_set_focus_item(widget, plk);
		break;
	case WD_TAB_END:
		plk = get_list_last_child_item(ptd->parent);

		if (plk)
			listctrl_set_focus_item(widget, plk);
		break;
	}
}

void listctrl_redraw_item(res_win_t widget, link_t_ptr plk)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return;

	noti_list_reset_editor(widget, 1);

#ifdef _DEBUG
	XDL_ASSERT(is_list_item(ptd->list, plk));
#endif

	noti_list_owner(widget, NC_LISTITEMCALCED, ptd->parent, plk, NULL);

	_listctrl_item_rect(widget, plk, &xr);
	
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t listctrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return 0;

	noti_list_reset_editor(widget, 0);

	if (ilk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_list_item(ptd->parent, ilk));
#endif
	}

	bRe = (ilk == ptd->item) ? 1 : 0;
	if (bRe)
		return 1;

	if (ptd->item && !bRe)
	{
		if (!noti_list_item_changing(widget))
			return 0;
	}

	if (ilk && !bRe)
	{
		noti_list_item_changed(widget, ilk);

		_listctrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr listctrl_get_focus_item(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return NULL;

	return ptd->item;
}

bool_t listctrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* szText)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	const tchar_t* text;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_list_item(ptd->list, ilk));
#endif

	text = get_list_item_title_ptr(ilk);
	if (compare_text(szText, -1, text, -1, 0) != 0)
	{
		set_list_item_title(ilk, szText);

		noti_list_owner(widget, NC_LISTITEMUPDATE, ptd->list, ilk, NULL);

		listctrl_redraw_item(widget, ilk);

		return 1;
	}

	return 0;
}

void listctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_list_item(ptd->parent, ilk));
#endif

	_listctrl_item_rect(widget, ilk, pxr);
}

bool_t listctrl_get_lock(res_win_t widget)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

void listctrl_set_lock(res_win_t widget, bool_t bLock)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

void listctrl_find(res_win_t widget, link_t_ptr pos, const tchar_t* token)
{
	list_delta_t* ptd = GETLISTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return;

	noti_list_reset_editor(widget, 1);

	_listctrl_find(widget, pos, token);
}

void listctrl_popup_size(res_win_t widget, xsize_t* pse)
{
	list_delta_t* ptd = GETLISTDELTA(widget);
	float ih, iw;
	int count;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->list)
		return;

	ih = get_list_item_height(ptd->list);
	iw = get_list_item_width(ptd->list);

	count = get_list_child_item_count(ptd->list);
	if (count % 3)
		count = count / 3 + 1;
	else
		count = count / 3;

	if (count > 5)
		count = 5;

	if (compare_text(get_list_layer_ptr(ptd->list), -1, ATTR_LAYER_HORZ, -1, 0) == 0)
	{
		pse->fx = iw * count;
		pse->fy = 3 * ih;
	}
	else
	{
		pse->fx = iw * 3;
		pse->fy = ih * count;
	}

	widget_size_to_pt(widget, pse);

	widget_adjust_size(widget_get_style(widget), pse);
}
