/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc string table control document

	@module	tablectrl.c | implement file

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

typedef struct _tablectrl_delta_t{
	link_t_ptr table;
	link_t_ptr item;
	link_t_ptr hover;
	float ratio;
	bool_t onkey;

	res_win_t editor;

	long org_x, org_y;
	bool_t b_size;
	bool_t b_auto;
	bool_t b_lock;
}tablectrl_delta_t;

#define GETTABLECTRLDELTA(ph) 	(tablectrl_delta_t*)widget_get_user_delta(ph)
#define SETTABLECTRLDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

#define TABLECTRL_MIN_SPLIT		10
/***************************************************************************************/

static void _tablectrl_item_rect(res_win_t widget, link_t_ptr plk, xrect_t* pxr)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	xfont_t xf;
	xface_t xa;

	canvbox_t cb;
	if_measure_t im = { 0 };

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	get_canvas_measure(widget_get_canvas(widget), &im);

	widget_get_canv_rect(widget, &cb);

	calc_table_item_rect(&im, &xf, &xa, &cb, ptd->table, plk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _tablectrl_item_key_rect(res_win_t widget, link_t_ptr plk, xrect_t* pxr)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	xfont_t xf;
	xface_t xa;

	canvbox_t cb;
	if_measure_t im = { 0 };

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	get_canvas_measure(widget_get_canvas(widget), &im);

	widget_get_canv_rect(widget, &cb);

	calc_table_item_key_rect(&im, &xf, &xa, &cb, ptd->table, ptd->ratio, plk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _tablectrl_item_val_rect(res_win_t widget, link_t_ptr plk, xrect_t* pxr)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	xfont_t xf;
	xface_t xa;

	canvbox_t cb;
	if_measure_t im = { 0 };

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	get_canvas_measure(widget_get_canvas(widget), &im);

	widget_get_canv_rect(widget, &cb);

	calc_table_item_val_rect(&im, &xf, &xa, &cb, ptd->table, ptd->ratio, plk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _tablectrl_reset_page(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xrect_t xr;
	xsize_t xs;

	xfont_t xf;
	xface_t xa;

	canvbox_t cb;
	if_measure_t im = { 0 };

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	get_canvas_measure(widget_get_canvas(widget), &im);

	widget_get_canv_rect(widget, &cb);

	(*im.pf_text_metric)(im.ctx, &xf, &xs);
	xs.fx = xs.fy;
	xs.fx = calc_table_height(&im, &xf, &xa, &cb, ptd->table);

	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, xr.w, xs.cy, xs.cx, xs.cx);
}

static void _tablectrl_ensure_visible(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xrect_t xr;

	if (!ptd->item)
		return;

	_tablectrl_item_rect(widget, ptd->item, &xr);
	widget_ensure_visible(widget, &xr, 1);
}

/*****************************************************************************************************************/
int noti_tablectrl_owner(res_win_t widget, unsigned long code, link_t_ptr table, link_t_ptr ilk, bool_t onkey, void* data)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	NOTICE_TABLE nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.table = table;
	nf.item = ilk;
	nf.onkey = onkey;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_tablectrl_begin_size(res_win_t widget, long x, long y)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	ptd->org_x = x;
	ptd->org_y = y;
	ptd->b_size = 1;

	widget_set_capture(widget, 1);
	widget_set_cursor(widget, CURSOR_SIZEWE);
}

void noti_tablectrl_end_size(res_win_t widget, long x, long y)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	canvbox_t cb;
	xsize_t xs;

	ptd->b_size = 0;

	widget_set_capture(widget, 0);
	widget_set_cursor(widget, CURSOR_ARROW);

	widget_get_canv_rect(widget, &cb);

	xs.cy = 0;
	xs.cx = (x - ptd->org_x);
	widget_size_to_tm(widget, &xs);
	xs.fx += cb.fw * ptd->ratio;

	if (xs.fx < DEF_TOUCH_SPAN)
		xs.fx = DEF_TOUCH_SPAN;

	ptd->ratio = (float)(xs.fx / cb.fw);

	widget_update(widget, NULL, 0);
}

bool_t noti_tablectrl_item_insert(res_win_t widget, link_t_ptr ilk)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ilk);

	if (noti_tablectrl_owner(widget, NC_TABLEITEMINSERT, ptd->table, ilk, ptd->onkey, NULL))
		return 0;

	return 1;
}

bool_t noti_tablectrl_item_delete(res_win_t widget, link_t_ptr ilk)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ilk);

	if (noti_tablectrl_owner(widget, NC_TABLEITEMDELETE, ptd->table, ilk, ptd->onkey, NULL))
		return 0;

	return 1;
}

bool_t noti_tablectrl_item_changing(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	if (noti_tablectrl_owner(widget, NC_TABLEITEMCHANGING, ptd->table, ptd->item, ptd->onkey, NULL))
		return 0;

	_tablectrl_item_rect(widget,ptd->item, &xr);

	ptd->item = NULL;
	ptd->onkey = 0;

	widget_update(widget, &xr, 0);

	return 1;
}

void noti_tablectrl_item_changed(res_win_t widget, link_t_ptr elk, bool_t onkey)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = elk;
	ptd->onkey = onkey;

	_tablectrl_item_rect(widget, ptd->item, &xr);
	
	widget_update(widget, &xr, 0);
}

void noti_tablectrl_begin_edit(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	const tchar_t* text;
	xrect_t xr = { 0 };

	clr_mod_t ob = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd->item);

	if (widget_is_valid(ptd->editor))
		return;

	if (ptd->b_lock)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_color_mode(widget, &ob);

	if (ptd->onkey)
		_tablectrl_item_key_rect(widget, ptd->item, &xr);
	else
		_tablectrl_item_val_rect(widget, ptd->item, &xr);

	pt_expand_rect(&xr, -1, -1);

	if (noti_tablectrl_owner(widget, NC_TABLEITEMEDITING, ptd->table, ptd->item, ptd->onkey, NULL))
		return;

	ptd->editor = fireedit_create(widget, &xr);
	XDL_ASSERT(ptd->editor);
	widget_set_user_id(ptd->editor, IDC_FIREEDIT);
	widget_set_owner(ptd->editor, widget);

	widget_set_xfont(ptd->editor, &xf);
	widget_set_color_mode(ptd->editor, &ob);
	widget_show(ptd->editor, WD_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	if (ptd->onkey)
		text = get_string_entity_key_ptr(ptd->item);
	else
		text = get_string_entity_val_ptr(ptd->item);

	editbox_set_text(ptd->editor, text);
	editbox_selectall(ptd->editor);
}

void noti_tablectrl_commit_edit(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	const tchar_t* text;
	res_win_t editctrl;
	link_t_ptr ilk_new;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->item);

	text = (const tchar_t*)editbox_get_text_ptr(ptd->editor);

	if (!noti_tablectrl_owner(widget, NC_TABLEITEMCOMMIT, ptd->table, ptd->item, ptd->onkey, (void*)text))
	{
		if (ptd->onkey)
			tablectrl_set_item_key_text(widget, ptd->item, text);
		else
			tablectrl_set_item_val_text(widget, ptd->item, text);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);

	if (ptd->b_auto && (ptd->item == get_string_prev_entity(ptd->table, LINK_LAST)) && !ptd->onkey)
	{
		ilk_new = insert_string_entity(ptd->table, LINK_LAST);
		set_string_entity_dirty(ilk_new, 0);

		if (!noti_tablectrl_item_insert(widget, ilk_new))
		{
			delete_string_entity(ptd->table, ilk_new);
			return;
		}

		ptd->onkey = 1;
		tablectrl_set_focus_item(widget, ilk_new);
	}
	else
	{
		if (ptd->onkey)
		{
			tablectrl_tabskip(widget,WD_TAB_RIGHT);
		}
		else
		{
			tablectrl_tabskip(widget,WD_TAB_DOWN);
		}
	}
}

void noti_tablectrl_rollback_edit(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->item);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_tablectrl_reset_editor(res_win_t widget, bool_t bCommit)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_tablectrl_commit_edit(widget);
		else
			noti_tablectrl_rollback_edit(widget);
	}
}

/********************************************************************************************/

int hand_tablectrl_create(res_win_t widget, void* data)
{
	tablectrl_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (tablectrl_delta_t*)xmem_alloc(sizeof(tablectrl_delta_t));

	SETTABLECTRLDELTA(widget, ptd);

	ptd->ratio = 0.5;
	ptd->b_lock = 1;

	return 0;
}

void hand_tablectrl_destroy(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETTABLECTRLDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_tablectrl_keydown(res_win_t widget, int key)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;

	switch (key)
	{
	case KEY_ENTER:
		if (ptd->item && !widget_is_valid(ptd->editor))
		{
			noti_tablectrl_begin_edit(widget);
		}
		break;
	case KEY_SPACE:
		break;
	case KEY_LEFT:
		tablectrl_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		tablectrl_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_UP:
		tablectrl_tabskip(widget,WD_TAB_UP);
		break;
	case KEY_DOWN:
		tablectrl_tabskip(widget,WD_TAB_DOWN);
		break;
	case KEY_HOME:
		tablectrl_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		tablectrl_tabskip(widget,WD_TAB_END);
		break;
	}
}

void hand_tablectrl_char(res_win_t widget, tchar_t nChar)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;

	if (IS_VISIBLE_CHAR(nChar) && !widget_is_valid(ptd->editor))
	{
		hand_tablectrl_keydown(widget, KEY_ENTER);
	}

	if (IS_VISIBLE_CHAR(nChar) && widget_is_valid(ptd->editor))
	{
		widget_post_char(ptd->editor, nChar);
	}
}

void hand_tablectrl_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;
}

void hand_tablectrl_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;
}

void hand_tablectrl_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;
}

void hand_tablectrl_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xpoint_t pt;
	int hint;
	link_t_ptr ilk = NULL;
	if_measure_t im;
	canvbox_t cb;
	xfont_t xf;
	xface_t xa;

	if (!ptd->table)
		return;

	noti_tablectrl_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	get_canvas_measure(widget_get_canvas(widget), &im);

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	hint = calc_table_hint(&im, &xf, &xa, &cb, &pt, ptd->table, ptd->ratio, &ilk);

	if (hint == TABLE_HINT_SPLIT)
	{
		noti_tablectrl_begin_size(widget, pxp->x, pxp->y);
		return;
	}
}

void hand_tablectrl_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xpoint_t pt;
	int hint;
	link_t_ptr ilk = NULL;
	if_measure_t im;
	canvbox_t cb;
	xfont_t xf;
	xface_t xa;
	bool_t onkey;

	if (!ptd->table)
		return;

	if (ptd->b_size)
	{
		noti_tablectrl_end_size(widget, pxp->x, pxp->y);
		return;
	}

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	get_canvas_measure(widget_get_canvas(widget), &im);

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	hint = calc_table_hint(&im, &xf, &xa, &cb, &pt, ptd->table, ptd->ratio, &ilk);

	onkey = (hint == TABLE_HINT_KEY) ? 1 : 0;

	if (ptd->item && ilk == ptd->item && onkey == ptd->onkey && !ptd->b_lock)
	{
		widget_post_key(widget, KEY_ENTER);
		return;
	}

	if (ilk != ptd->item || onkey != ptd->onkey)
	{
		if (ptd->item)
			noti_tablectrl_item_changing(widget);

		if (ilk)
			noti_tablectrl_item_changed(widget, ilk, onkey);
	}

	noti_tablectrl_owner(widget, NC_TABLELBCLK, ptd->table, ptd->item, ptd->onkey, (void*)pxp);
}

void hand_tablectrl_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;

	noti_tablectrl_owner(widget, NC_TABLEDBCLK, ptd->table, ptd->item, 0, (void*)pxp);
}

void hand_tablectrl_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;

	noti_tablectrl_reset_editor(widget, 1);
}

void hand_tablectrl_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;

	noti_tablectrl_owner(widget, NC_TABLERBCLK, ptd->table, ptd->item, 0, (void*)pxp);
}

void hand_tablectrl_size(res_win_t widget, int code, const xsize_t* prs)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;

	tablectrl_redraw(widget);
}

void hand_tablectrl_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	if (!ptd->table)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_tablectrl_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->table)
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

void hand_tablectrl_child_command(res_win_t widget, int code, var_long data)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_tablectrl_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_tablectrl_rollback_edit(widget);
		break;
	}
}

void hand_tablectrl_erase(res_win_t widget, res_ctx_t rdc)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	
	if (!ptd->table)
		return;
}

void hand_tablectrl_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	res_ctx_t rdc;
	canvas_t canv;
	if_canvas_t* pif;
	xrect_t xr;

	canvbox_t cb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };

	if (!ptd->table)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	pif = create_canvas_interface(canv);

	widget_get_canv_rect(widget, &cb);

	draw_table(pif, &xf, &xa, &xp, &xb, &cb, ptd->table, ptd->ratio);

	//draw focus
	if (ptd->item)
	{
		if (ptd->onkey)
			_tablectrl_item_key_rect(widget, ptd->item, &xr);
		else
			_tablectrl_item_val_rect(widget, ptd->item, &xr);

		pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);
		alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
	}

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/************************************************************************************************/
res_win_t tablectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_tablectrl_create)
		EVENT_ON_DESTROY(hand_tablectrl_destroy)

		EVENT_ON_ERASE(hand_tablectrl_erase)
		EVENT_ON_PAINT(hand_tablectrl_paint)

		EVENT_ON_SIZE(hand_tablectrl_size)

		EVENT_ON_SCROLL(hand_tablectrl_scroll)
		EVENT_ON_WHEEL(hand_tablectrl_wheel)

		EVENT_ON_KEYDOWN(hand_tablectrl_keydown)

		EVENT_ON_MOUSE_MOVE(hand_tablectrl_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_tablectrl_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_tablectrl_mouse_leave)

		EVENT_ON_LBUTTON_DOWN(hand_tablectrl_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_tablectrl_lbutton_up)
		EVENT_ON_LBUTTON_DBCLICK(hand_tablectrl_lbutton_dbclick)
		EVENT_ON_RBUTTON_DOWN(hand_tablectrl_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_tablectrl_rbutton_up)

		EVENT_ON_CHILD_COMMAND(hand_tablectrl_child_command)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void tablectrl_attach(res_win_t widget, link_t_ptr ptr)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);
	XDL_ASSERT(ptr != NULL);

	ptd->table = ptr;

	tablectrl_redraw(widget);

	if (!ptd->b_lock && ptd->b_auto && !get_string_next_entity(ptr, LINK_FIRST))
	{
		tablectrl_insert_item(widget, LINK_LAST);
	}
}

link_t_ptr tablectrl_detach(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->table;
	ptd->table = NULL;

	tablectrl_redraw(widget);

	return data;
}

link_t_ptr tablectrl_fetch(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->table;
}

void tablectrl_redraw(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return;

	noti_tablectrl_reset_editor(widget, 1);

	b_valid = 0;
	ilk = get_string_next_entity(ptd->table, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		noti_tablectrl_owner(widget, NC_TABLEITEMCALCED, ptd->table, ilk, 0, NULL);

		ilk = get_string_next_entity(ptd->table, ilk);
	}

	noti_tablectrl_owner(widget, NC_TABLECALCED, ptd->table, NULL, 0, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}

	_tablectrl_reset_page(widget);

	widget_update_window(widget);

	widget_update(widget, NULL, 0);
}

void tablectrl_redraw_item(res_win_t widget, link_t_ptr ent)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return;

	noti_tablectrl_reset_editor(widget, 1);

#ifdef _DEBUG
	XDL_ASSERT(is_string_entity(ptd->table, ent));
#endif

	noti_tablectrl_owner(widget, NC_TABLEITEMCALCED, ptd->table, ent, 0, NULL);

	_tablectrl_item_rect(widget, ent, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t tablectrl_set_focus_item(res_win_t widget, link_t_ptr ent)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return 0;

	if (ent == LINK_FIRST)
		ent = get_string_next_entity(ptd->table, LINK_FIRST);
	else if (ent == LINK_LAST)
		ent = get_string_prev_entity(ptd->table, LINK_LAST);

	bRe = (ent == ptd->item) ? 1 : 0;

	if (!bRe && ptd->item)
	{
		if (!noti_tablectrl_item_changing(widget))
			return 0;
	}

	if (!bRe && ent)
	{
		noti_tablectrl_item_changed(widget, ent, 0);

		_tablectrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr tablectrl_get_focus_item(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return NULL;

	return ptd->item;
}

void tablectrl_get_item_rect(res_win_t widget, link_t_ptr elk, xrect_t* prt)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_string_entity(ptd->table, elk));
#endif

	_tablectrl_item_rect(widget, elk, prt);
}

void tablectrl_tabskip(res_win_t widget, int nSkip)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	link_t_ptr plk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return;

	noti_tablectrl_reset_editor(widget, 1);

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
		if (ptd->onkey && ptd->item)
		{
			ptd->onkey = 0;
			tablectrl_redraw_item(widget, ptd->item);
		}
		break;
	case WD_TAB_LEFT:
		if (!ptd->onkey && ptd->item)
		{
			ptd->onkey = 1;
			tablectrl_redraw_item(widget, ptd->item);
		}
		break;
	case WD_TAB_DOWN:
		if (ptd->item)
		{
			plk = get_string_next_entity(ptd->table, ptd->item);

			if (plk)
			{
				ptd->onkey = 1;
				tablectrl_set_focus_item(widget, plk);
			}
		}
		break;
	case WD_TAB_UP:
		if (ptd->item)
		{
			plk = get_string_prev_entity(ptd->table, ptd->item);

			if (plk)
			{
				ptd->onkey = 1;
				tablectrl_set_focus_item(widget, plk);
			}
		}
		break;
	case WD_TAB_HOME:
		plk = get_string_next_entity(ptd->table, LINK_FIRST);

		if (plk)
		{
			ptd->onkey = 1;
			tablectrl_set_focus_item(widget, plk);
		}
		break;
	case WD_TAB_END:
		plk = get_string_prev_entity(ptd->table, LINK_LAST);

		if (plk)
		{
			ptd->onkey = 1;
			tablectrl_set_focus_item(widget, plk);
		}
		break;
	}
}

void tablectrl_set_item_key_text(res_win_t widget, link_t_ptr elk, const tchar_t* token)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xrect_t xr;
	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_string_entity(ptd->table, elk));
#endif

	if (is_null(token))
		return;

	if (compare_text(get_string_entity_key_ptr(elk), -1, token, -1, 0) == 0)
		return;

	set_string_entity_key(elk, token, -1);

	noti_tablectrl_owner(widget, NC_TABLEITEMUPDATE, ptd->table, elk, 1, NULL);

	tablectrl_get_item_rect(widget, elk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
	widget_update(widget, &xr, 0);
}

void tablectrl_set_item_val_text(res_win_t widget, link_t_ptr elk, const tchar_t* token)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_string_entity(ptd->table, elk));
#endif

	if (compare_text(get_string_entity_val_ptr(elk), -1, token, -1, 0) == 0)
		return;

	set_string_entity_val(elk, token, -1);
	set_string_entity_dirty(elk, 1);

	noti_tablectrl_owner(widget, NC_TABLEITEMUPDATE, ptd->table, elk, 0, NULL);

	tablectrl_get_item_rect(widget, elk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
	widget_update(widget, &xr, 0);
}

void tablectrl_accept(res_win_t widget, bool_t bAccept)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return;

	noti_tablectrl_reset_editor(widget, bAccept);

	ilk = get_string_next_entity(ptd->table, LINK_FIRST);
	while (ilk)
	{
		nxt = get_string_next_entity(ptd->table, ilk);

		if (!get_string_entity_dirty(ilk))
			delete_string_entity(ptd->table, ilk);

		ilk = nxt;
	}

	tablectrl_redraw(widget);
}

void tablectrl_auto_insert(res_win_t widget, bool_t bAuto)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_auto = bAuto;
}

bool_t tablectrl_delete_item(res_win_t widget, link_t_ptr ilk)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	link_t_ptr nlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return 0;


	noti_tablectrl_reset_editor(widget, 0);

#ifdef _DEBUG
	XDL_ASSERT(is_string_entity(ptd->table, ilk));
#endif

	if (!noti_tablectrl_item_delete(widget, ilk))
		return 0;

	if (ilk == ptd->item)
	{
		noti_tablectrl_item_changing(widget);
	}

	nlk = get_string_next_entity(ptd->table, ilk);
	if (!nlk)
		nlk = get_string_prev_entity(ptd->table, ilk);

	delete_string_entity(ptd->table, ilk);

	tablectrl_redraw(widget);

	if (nlk)
		tablectrl_set_focus_item(widget, nlk);

	return 1;
}

link_t_ptr tablectrl_insert_item(res_win_t widget, link_t_ptr pre)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);
	link_t_ptr ilk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return NULL;

	noti_tablectrl_reset_editor(widget, 0);

	if (pre == LINK_FIRST)
		pre = get_string_next_entity(ptd->table, LINK_FIRST);
	else if (pre == LINK_LAST)
		pre = get_string_prev_entity(ptd->table, LINK_LAST);
	else
	{
#ifdef _DEBUG
		XDL_ASSERT(is_string_entity(ptd->table, pre));
#endif
	}

	if (!pre) pre = LINK_FIRST;

	ilk = insert_string_entity(ptd->table, pre);
	set_string_entity_dirty(ilk, 0);

	if (!noti_tablectrl_item_insert(widget, ilk))
	{
		delete_string_entity(ptd->table, ilk);
		return NULL;
	}

	tablectrl_redraw(widget);

	ptd->onkey = 1;
	tablectrl_set_focus_item(widget, ilk);

	return ilk;
}

bool_t tablectrl_is_update(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->table)
		return 0;

	return (get_string_updated_entity_count(ptd->table)) ? 1 : 0;
}

void tablectrl_set_ratio(res_win_t widget, float r)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->ratio = r;
}

float tablectrl_get_ratio(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->ratio;
}

void tablectrl_set_lock(res_win_t widget, bool_t b)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = b;
}

bool_t tablectrl_get_lock(res_win_t widget)
{
	tablectrl_delta_t* ptd = GETTABLECTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}
