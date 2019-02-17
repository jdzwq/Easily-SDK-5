/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc topog control document

	@module	topogctrl.c | topog control widnows implement file

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

typedef struct _topog_delta_t{
	link_t_ptr topog;
	link_t_ptr spot;
	link_t_ptr hover;

	bool_t b_drag;
	long org_x, org_y;
	int row, col;

	ximage_t img;

	link_t_ptr stack;
}topog_delta_t;


#define GETTOPOGDELTA(ph) 	(topog_delta_t*)widget_get_user_delta(ph)
#define SETTOPOGDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/*****************************************************************************/
static void _topogctrl_done(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->topog);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->topog, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->topog, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->topog, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->topog, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _topogctrl_undo(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->topog);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_topog_doc(ptd->topog);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->topog, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->topog, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		topogctrl_redraw(widget);
	}
}

static void _topogctrl_discard(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _topogctrl_clean(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _topogctrl_copy(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	res_glob_t gb;
	dword_t len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->topog);

	if (!get_topog_spot_selected_count(ptd->topog))
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_topog_doc();
	ilk = get_topog_next_spot(ptd->topog, LINK_FIRST);
	while (ilk)
	{
		if (get_topog_spot_selected(ilk))
		{
			nlk = insert_topog_spot(dom, LINK_LAST);
			copy_dom_node(nlk, ilk);
		}

		ilk = get_topog_next_spot(ptd->topog, ilk);
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

		destroy_topog_doc(dom);

		return 0;
	}

	clipboard_close();

	destroy_topog_doc(dom);
	return 1;
}

static bool_t _topogctrl_cut(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->topog);

	if (!_topogctrl_copy(widget))
		return 0;

	ilk = get_topog_next_spot(ptd->topog, LINK_FIRST);
	while (ilk)
	{
		nxt = get_topog_next_spot(ptd->topog, ilk);

		if (get_topog_spot_selected(ilk))
		{
			delete_topog_spot(ilk);
		}
		ilk = nxt;
	}

	topogctrl_redraw(widget);

	return 1;
}

static bool_t _topogctrl_paste(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk;

	int row, col;
	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->topog);

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

	if (!is_topog_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	while (nlk = get_topog_next_spot(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_topog_spotset(dom), nlk);
		attach_dom_node(get_topog_spotset(ptd->topog), LINK_LAST, nlk);

		xsprintf(sz_name, _T("spot%d"), get_topog_spot_count(ptd->topog));
		set_topog_spot_name(nlk, sz_name);

		col = get_topog_spot_col(nlk);
		row = get_topog_spot_row(nlk) + 2;

		set_topog_spot_col(nlk, col);
		set_topog_spot_row(nlk, row);
	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);
	return 1;
}

static void _topogctrl_spot_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_topog_spot_rect(&cb, ptd->topog, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _topogctrl_ensure_visible(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	xrect_t xr = { 0 };

	if (!ptd->spot)
		return;

	_topogctrl_spot_rect(widget, ptd->spot, &xr);

	widget_ensure_visible(widget, &xr, 1);
}

static void _topogctrl_reset_matrix(res_win_t widget, int row, int col)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	int rows, cols;
	matrix_t* mt = NULL;
	bool_t b;
	tchar_t* buf;
	int len;

	if (!ptd->topog)
		return;

	rows = get_topog_rows(ptd->topog);
	cols = get_topog_cols(ptd->topog);

	if (row < 0 || row >= rows)
		return;

	if (col < 0 || col >= cols)
		return;

	mt = matrix_alloc(rows, cols);

	matrix_parse(mt, get_topog_matrix_ptr(ptd->topog), -1);

	b = ((int)matrix_get_value(mt, row, col)) ? 0 : 1;
	matrix_set_value(mt, row, col, (double)b);

	len = matrix_format(mt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	len = matrix_format(mt, buf, len);

	matrix_free(mt);

	set_topog_matrix(ptd->topog, buf, len);
	xsfree(buf);

	widget_update(widget, NULL, 0);
}

static void _topogctrl_reset_page(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_topog_cols(ptd->topog) * get_topog_rx(ptd->topog);
	xs.fy = get_topog_rows(ptd->topog) * get_topog_ry(ptd->topog);
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


/*****************************************************************************/
int noti_topog_owner(res_win_t widget, unsigned long code, link_t_ptr topog, link_t_ptr spot, int row, int col, void* data)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	NOTICE_TOPOG nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.topog = topog;
	nf.spot = spot;
	nf.row = row;
	nf.col = col;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_topog_reset_select(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	link_t_ptr ilk;
	int count = 0;

	ilk = get_topog_next_spot(ptd->topog, LINK_FIRST);
	while (ilk)
	{
		if (get_topog_spot_selected(ilk))
		{
			set_topog_spot_selected(ilk, 0);
			noti_topog_owner(widget, NC_TOPOGSPOTSELECTED, ptd->topog, ilk, get_topog_spot_row(ilk), get_topog_spot_col(ilk), NULL);

			count++;
		}

		ilk = get_topog_next_spot(ptd->topog, ilk);
	}

	if (count)
	{
		widget_update(widget, NULL, 0);
	}
}

void noti_topog_spot_selected(res_win_t widget, link_t_ptr ilk)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	xrect_t xr;
	bool_t b_check;

	b_check = get_topog_spot_selected(ilk);

	if (b_check)
		set_topog_spot_selected(ilk, 0);
	else
		set_topog_spot_selected(ilk, 1);

	noti_topog_owner(widget, NC_TOPOGSPOTSELECTED, ptd->topog, ilk, get_topog_spot_row(ilk), get_topog_spot_col(ilk), NULL);

	_topogctrl_spot_rect(widget, ilk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t noti_topog_spot_changing(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->spot);

	if (noti_topog_owner(widget, NC_TOPOGSPOTCHANGING, ptd->topog, ptd->spot, ptd->row, ptd->col, NULL))
		return (bool_t)0;

	_topogctrl_spot_rect(widget, ptd->spot, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->spot = NULL;
	ptd->row = -1;
	ptd->col = -1;

	widget_update(widget, &xr, 0);

	return (bool_t)1;
}

void noti_topog_spot_changed(res_win_t widget, link_t_ptr ilk)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->spot);

	ptd->spot = ilk;
	ptd->row = get_topog_spot_row(ilk);
	ptd->col = get_topog_spot_col(ilk);

	_topogctrl_spot_rect(widget, ptd->spot, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	noti_topog_owner(widget, NC_TOPOGSPOTCHANGED, ptd->topog, ptd->spot, ptd->row, ptd->col, NULL);
}

void noti_topog_spot_enter(res_win_t widget, link_t_ptr ilk)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = ilk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_topog_spot_leave(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_topog_spot_hover(res_win_t widget, long x, long y)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_topog_owner(widget, NC_TOPOGSPOTHOVER, ptd->topog, ptd->hover, get_topog_spot_row(ptd->hover), get_topog_spot_col(ptd->hover), (void*)&xp);
}

void noti_topog_spot_drag(res_win_t widget, long x, long y)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->spot);

	ptd->b_drag = (bool_t)1;
	ptd->org_x = x;
	ptd->org_y = y;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget, CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_topog_owner(widget, NC_TOPOGSPOTDRAG, ptd->topog, ptd->spot, ptd->row, ptd->col, (void*)&pt);
}

void noti_topog_spot_drop(res_win_t widget, long x, long y)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	xpoint_t pt;
	xsize_t xs;
	long cx, cy;

	XDL_ASSERT(ptd->spot);

	ptd->b_drag = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	xs.fx = get_topog_rx(ptd->topog);
	xs.fy = get_topog_ry(ptd->topog);

	widget_size_to_pt(widget, &xs);

	if (!xs.cx || !xs.cy)
		return;

	cx = x - ptd->org_x;
	cy = y - ptd->org_y;

	cx /= xs.cx;
	cy /= xs.cy;

	if (!cx && !cy)
		return;

	cx += get_topog_spot_col(ptd->spot);
	cy += get_topog_spot_row(ptd->spot);

	if (cx < 0 || cy < 0)
		return;

	if (cx >= get_topog_cols(ptd->topog) || cy >= get_topog_rows(ptd->topog))
		return;

	_topogctrl_done(widget);

	set_topog_spot_col(ptd->spot, cx);
	set_topog_spot_row(ptd->spot, cy);
	ptd->row = cy;
	ptd->col = cx;

	widget_update(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_topog_owner(widget, NC_TOPOGSPOTDROP, ptd->topog, ptd->spot, ptd->row, ptd->col, (void*)&pt);
}
/*****************************************************************************/
int hand_topogctrl_create(res_win_t widget, void* data)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	widget_hand_create(widget);

	ptd = (topog_delta_t*)xmem_alloc(sizeof(topog_delta_t));
	xmem_zero((void*)ptd, sizeof(topog_delta_t));

	xmem_zero((void*)&ptd->img, sizeof(ximage_t));

	ptd->stack = create_stack_table();

	SETTOPOGDELTA(widget, ptd);

	return 0;
}

void hand_topogctrl_destroy(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	_topogctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	if (ptd->img.source)
		xsfree(ptd->img.source);

	xmem_free(ptd);

	SETTOPOGDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_topogctrl_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	xpoint_t pt;
	canvbox_t cb;
	int row, col;

	if (!ptd->topog)
		return;

	if (ptd->b_drag)
		return;

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	ilk = NULL;
	row = col = -1;
	nHint = calc_topog_hint(&cb, &pt, ptd->topog, &ilk, &row, &col);

	if (nHint == TOPOG_HINT_SPOT && !ptd->hover && ilk)
	{
		noti_topog_spot_enter(widget, ilk);
	}
	else if (nHint == TOPOG_HINT_SPOT && ptd->hover && ptd->hover != ilk)
	{
		noti_topog_spot_leave(widget);
	}
	else if (nHint != TOPOG_HINT_SPOT && ptd->hover)
	{
		noti_topog_spot_leave(widget);
	}

	if (topog_is_design(ptd->topog) && nHint == TOPOG_HINT_SPOT && ilk == ptd->spot && (dw & MS_WITH_LBUTTON) && !(dw & MS_WITH_CONTROL))
	{
		noti_topog_spot_drag(widget, pxp->x, pxp->y);
	}
}

void hand_topogctrl_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	if (ptd->hover)
		noti_topog_spot_hover(widget, pxp->x, pxp->y);
}

void hand_topogctrl_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	if (ptd->hover)
		noti_topog_spot_leave(widget);
}

void hand_topogctrl_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;
	int row, col;

	if (!ptd->topog)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	if (!topog_is_design(ptd->topog))
		return;

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	ilk = NULL;
	row = col = -1;
	nHint = calc_topog_hint(&cb, &pt, ptd->topog, &ilk, &row, &col);
	bRe = (ilk == ptd->spot) ? 1 : 0;

	if (nHint == TOPOG_HINT_SPOT)
	{
		if (widget_key_state(widget, KEY_CONTROL))
		{
			noti_topog_spot_selected(widget, ilk);
		}
	}
	else if (nHint == TOPOG_HINT_NONE)
	{
		if (!widget_key_state(widget, KEY_CONTROL))
		{
			noti_topog_reset_select(widget);
		}
	}
}

void hand_topogctrl_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;
	int row, col;

	if (!ptd->topog)
		return;

	if (ptd->b_drag)
	{
		noti_topog_spot_drop(widget, pxp->x, pxp->y);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	ilk = NULL;
	row = col = -1;
	nHint = calc_topog_hint(&cb, &pt, ptd->topog, &ilk, &row, &col);

	bRe = (ilk == ptd->spot) ? 1 : 0;

	if (ptd->spot && !bRe)
	{
		if (!noti_topog_spot_changing(widget))
			return;
	}

	if (ilk && !bRe)
	{
		noti_topog_spot_changed(widget, ilk);
	}

	if (topog_is_design(ptd->topog))
	{
		bRe = (row == ptd->row && ptd->col == col) ? 1 : 0;

		if (widget_key_state(widget, KEY_CONTROL))
		{
			_topogctrl_reset_matrix(widget, row, col);
		}
	}

	ptd->row = row;
	ptd->col = col;

	noti_topog_owner(widget, NC_TOPOGLBCLK, ptd->topog, ptd->spot, ptd->row, ptd->col, (void*)pxp);
}

void hand_topogctrl_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	noti_topog_owner(widget, NC_TOPOGDBCLK, ptd->topog, ptd->spot, ptd->row, ptd->col, (void*)pxp);
}

void hand_topogctrl_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;
}

void hand_topogctrl_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	noti_topog_owner(widget, NC_TOPOGRBCLK, ptd->topog, ptd->spot, ptd->row, ptd->col, (void*)pxp);
}

void hand_topogctrl_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_topogctrl_keydown(res_win_t widget, int nKey)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	int row, col, m;
	int rows, cols;
	bool_t b_design;
	link_t_ptr slk;

	if (!ptd->topog)
		return;

	b_design = topog_is_design(ptd->topog);

	if (b_design)
	{
		if (ptd->spot && (nKey == KEY_UP || nKey == KEY_DOWN || nKey == KEY_LEFT || nKey == KEY_RIGHT))
		{
			m = 1;

			noti_topog_owner(widget, NC_FIELDDRAG, ptd->topog, ptd->spot, ptd->row, ptd->col, NULL);

			_topogctrl_done(widget);

			rows = get_topog_rows(ptd->topog);
			cols = get_topog_cols(ptd->topog);

			slk = get_topog_next_spot(ptd->topog, LINK_FIRST);
			while (slk)
			{
				if (slk != ptd->spot && !get_topog_spot_selected(slk))
				{
					slk = get_topog_next_spot(ptd->topog, slk);
					continue;
				}

				col = get_topog_spot_col(slk);
				row = get_topog_spot_row(slk);

				switch (nKey)
				{
				case KEY_DOWN:
					row = (row + m < rows) ? row + m : row;
					break;
				case KEY_UP:
					row = (row - m < 0) ? row : row - m;
					break;
				case KEY_LEFT:
					col = (col - m < 0) ? col : col - m;
					break;
				case KEY_RIGHT:
					col = (col + m < cols)? col + m : col;
					break;
				}

				set_topog_spot_col(slk, col);
				set_topog_spot_row(slk, row);
				ptd->row = row;
				ptd->col = col;

				slk = get_topog_next_spot(ptd->spot, slk);
			}

			widget_update(widget, NULL, 0);

			noti_topog_owner(widget, NC_TOPOGSPOTDROP, ptd->topog, ptd->spot, ptd->row, ptd->col, NULL);
		}
		else if ((nKey == _T('z') || nKey == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_undo(widget);
		}
		else if ((nKey == _T('c') || nKey == _T('C')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_copy(widget);
		}
		else if ((nKey == _T('x') || nKey == _T('X')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_cut(widget);
		}
		else if ((nKey == _T('v') || nKey == _T('V')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_paste(widget);
		}
	}
	else
	{
		if (nKey == KEY_TAB)
		{
			topogctrl_tabskip(widget,WD_TAB_RIGHT);
		}
		else if (nKey == KEY_LEFT || nKey == KEY_UP) //VK_LEFT VK_UP
		{
			topogctrl_tabskip(widget,WD_TAB_LEFT);
		}
		else if (nKey == KEY_RIGHT || nKey == KEY_DOWN) //VK_RIGHT VK_DOWN
		{
			topogctrl_tabskip(widget,WD_TAB_RIGHT);
		}
	}
}

void hand_topogctrl_copy(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	_topogctrl_copy(widget);
}

void hand_topogctrl_cut(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	_topogctrl_done(widget);

	if (!_topogctrl_cut(widget))
	{
		_topogctrl_discard(widget);
	}
}

void hand_topogctrl_paste(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	_topogctrl_done(widget);

	if (!_topogctrl_paste(widget))
	{
		_topogctrl_discard(widget);
	}
}

void hand_topogctrl_undo(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	_topogctrl_undo(widget);
}

void hand_topogctrl_size(res_win_t widget, int code, const xsize_t* prs)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	if (!ptd->topog)
		return;

	_topogctrl_reset_page(widget);

	widget_update(widget, NULL, 0);
}

void hand_topogctrl_erase(res_win_t widget, res_ctx_t dc)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
}

void hand_topogctrl_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr;
	if_canvas_t* pif;
	canvbox_t cb;

	canvas_t canv;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);

	parse_xcolor(&pif->clr_bkg, xb.color);
	parse_xcolor(&pif->clr_frg, xp.color);
	parse_xcolor(&pif->clr_txt, xf.color);
	widget_get_xcolor(widget, &pif->clr_msk);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);
	
	if (ptd->img.source)
	{
		format_xcolor(&pif->clr_msk, ptd->img.color);

		draw_image(pif->canvas, &(ptd->img), (xrect_t*)&cb);
	}

	if (ptd->topog)
	{
		draw_topog(pif, &cb, ptd->topog);

		if (topog_is_design(ptd->topog) && ptd->spot)
		{
			_topogctrl_spot_rect(widget, ptd->spot, &xr);

			pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

			if (get_topog_spot_selected(ptd->spot))
			{
				parse_xcolor(&xc, DEF_FOCUS_BRUSH);
				alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
			}
			else
			{
				parse_xcolor(&xc, DEF_FOCUS_COLOR);
				draw_focus_raw(rdc, &xc, &xr, ALPHA_TRANS);
			}
		}
	}

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t topogctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_topogctrl_create)
		EVENT_ON_DESTROY(hand_topogctrl_destroy)

		EVENT_ON_ERASE(hand_topogctrl_erase)
		EVENT_ON_PAINT(hand_topogctrl_paint)

		EVENT_ON_SIZE(hand_topogctrl_size)

		EVENT_ON_SCROLL(hand_topogctrl_scroll)

		EVENT_ON_KEYDOWN(hand_topogctrl_keydown)

		EVENT_ON_MOUSE_MOVE(hand_topogctrl_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_topogctrl_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_topogctrl_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_topogctrl_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_topogctrl_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_topogctrl_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_topogctrl_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_topogctrl_rbutton_up)

		EVENT_ON_COPY(hand_topogctrl_copy)
		EVENT_ON_CUT(hand_topogctrl_cut)
		EVENT_ON_PASTE(hand_topogctrl_paste)
		EVENT_ON_UNDO(hand_topogctrl_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void topogctrl_attach(res_win_t widget, link_t_ptr data)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(data && is_topog_doc(data));

	ptd->topog = data;
	ptd->spot = NULL;
	ptd->row = -1;
	ptd->col = -1;

	topogctrl_redraw(widget);
}

link_t_ptr topogctrl_detach(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->topog;
	ptd->topog = NULL;
	ptd->spot = NULL;
	ptd->row = -1;
	ptd->col = -1;

	widget_update(widget, NULL, 0);

	return data;
}

link_t_ptr topogctrl_fetch(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->topog;
}

void topogctrl_redraw(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;


	XDL_ASSERT(ptd != NULL);

	if (!ptd->topog)
		return;

	b_valid = 0;
	ilk = get_topog_next_spot(ptd->topog, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->spot)
			b_valid = 1;

		noti_topog_owner(widget, NC_TOPOGSPOTCALCED, ptd->topog, ilk, get_topog_spot_row(ilk), get_topog_spot_col(ilk), NULL);

		ilk = get_topog_next_spot(ptd->topog, ilk);
	}

	noti_topog_owner(widget, NC_TOPOGCALCED, ptd->topog,  NULL, -1, -1,NULL);

	if (!b_valid)
	{
		ptd->spot = NULL;
		ptd->row = -1;
		ptd->col = -1;
	}
	ptd->hover = NULL;

	_topogctrl_reset_page(widget);

	widget_update_window(widget);

	widget_update(widget, NULL, 0);
}

void topogctrl_tabskip(res_win_t widget, int nSkip)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	link_t_ptr plk = ptd->spot;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->topog)
		return;

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (plk == NULL)
			plk = get_topog_next_spot(ptd->topog, LINK_FIRST);
		else
			plk = get_topog_next_spot(ptd->topog, plk);

		if (plk)
			topogctrl_set_focus_spot(widget, plk);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (plk == NULL)
			plk = get_topog_prev_spot(ptd->topog, LINK_LAST);
		else
			plk = get_topog_prev_spot(ptd->topog, plk);

		if (plk)
			topogctrl_set_focus_spot(widget, plk);
		break;
	}
}

void topogctrl_redraw_spot(res_win_t widget, link_t_ptr plk)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->topog)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_topog_spot(ptd->topog, plk));
#endif

	noti_topog_owner(widget, NC_TOPOGSPOTCALCED, ptd->topog, plk, get_topog_spot_row(plk), get_topog_spot_col(plk), NULL);

	_topogctrl_spot_rect(widget, plk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t topogctrl_set_focus_spot(res_win_t widget, link_t_ptr ilk)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->topog)
		return 0;

	if (ilk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_topog_spot(ptd->topog, ilk));
#endif
	}

	bRe = (ilk == ptd->spot) ? 1 : 0;
	if (bRe)
		return 1;

	if (ptd->spot && !bRe)
	{
		if (!noti_topog_spot_changing(widget))
			return 0;
	}

	if (ilk && !bRe)
	{
		noti_topog_spot_changed(widget, ilk);

		_topogctrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr topogctrl_get_focus_spot(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->spot;
}

void topogctrl_get_spot_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->topog)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_topog_spot(ptd->topog, ilk));
#endif

	_topogctrl_spot_rect(widget, ilk, pxr);
}

void topogctrl_get_focus_dot(res_win_t widget, int* prow, int* pcol)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	*prow = ptd->row;
	*pcol = ptd->col;
}

bool_t topogctrl_get_dirty(res_win_t widget)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->topog)
		return 0;

	if (!topog_is_design(ptd->topog))
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void topogctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->topog)
		return;

	if (!topog_is_design(ptd->topog))
		return;

	if (bDirty)
		_topogctrl_done(widget);
	else
		_topogctrl_clean(widget);
}

bool_t topogctrl_set_bitmap(res_win_t widget, res_bmp_t bmp)
{
	topog_delta_t* ptd = GETTOPOGDELTA(widget);
	bool_t rt;
	res_ctx_t rdc;

	XDL_ASSERT(ptd != NULL);

	rdc = widget_client_ctx(widget);

	if (ptd->img.source)
		xsfree(ptd->img.source);

	xmem_zero((void*)&ptd->img, sizeof(ximage_t));

	if (bmp)
		rt = save_bitmap_to_ximage(rdc, bmp, &ptd->img);
	else
		rt = 1;

	widget_release_ctx(widget, rdc);

	topogctrl_redraw(widget);

	return rt;
}
