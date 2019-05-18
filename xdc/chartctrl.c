/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc chart control document

	@module	chartctrl.c | chart control implement file

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

#define CHART_LINE_FEED		(float)50
#define CHART_TABLE_MIN_WIDTH	(float)10
#define CHART_TABLE_MIN_HEIGHT	(float)10

typedef struct _chart_delta_t{
	link_t_ptr chart;
	link_t_ptr table;
	link_t_ptr hover;

	int org_hint;
	long org_x, org_y;
	long cur_x, cur_y;
	short cur_page;

	int opera;

	bool_t b_drag;
	bool_t b_size;

	link_t_ptr stack;
}chart_delta_t;

#define GETCHARTDELTA(ph) 	(chart_delta_t*)widget_get_user_delta(ph)
#define SETCHARTDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/******************************************chart event********************************************************/
static void _chartctrl_done(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->chart);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->chart, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->chart, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->chart, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->chart, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _chartctrl_undo(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->chart);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_chart_doc(ptd->chart);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->chart, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->chart, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		chartctrl_redraw(widget);
	}
}

static void _chartctrl_discard(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _chartctrl_clean(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _chartctrl_copy(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->chart);

	if (!get_chart_table_selected_count(ptd->chart))
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_chart_doc();
	ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
	while (ilk)
	{
		if (get_field_selected(ilk))
		{
			nlk = insert_chart_table(dom, get_chart_table_class_ptr(ilk));
			copy_dom_node(nlk, ilk);
		}

		ilk = get_chart_next_table(ptd->chart, ilk);
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

		destroy_chart_doc(dom);
		return 0;
	}

	clipboard_close();

	destroy_chart_doc(dom);
	return 1;
}

static bool_t _chartctrl_cut(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->chart);

	if (!_chartctrl_copy(widget))
		return 0;

	ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
	while (ilk)
	{
		nxt = get_chart_next_table(ptd->chart, ilk);

		if (get_chart_table_selected(ilk))
		{
			delete_chart_table(ilk);
		}
		ilk = nxt;
	}

	chartctrl_redraw(widget);

	return 1;
}

static bool_t _chartctrl_paste(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk;

	float y;
	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->chart);

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

	if (!is_chart_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	while (nlk = get_chart_next_table(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_chart_tableset(dom), nlk);
		attach_dom_node(get_chart_tableset(ptd->chart), LINK_LAST, nlk);

		y = get_chart_table_y(nlk);
		y += get_chart_table_height(nlk);
		set_chart_table_y(nlk, y);

		xsprintf(sz_name, _T("%s%d"), get_chart_table_class_ptr(nlk), get_chart_table_count_by_class(ptd->chart, get_chart_table_class_ptr(nlk)));
		set_chart_table_name(nlk, sz_name);
	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);
	chartctrl_redraw(widget);

	return 1;
}

static void _chartctrl_table_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	calc_chart_table_rect(ptd->chart, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _chartctrl_reset_page(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_chart_width(ptd->chart);
	xs.fy = get_chart_height(ptd->chart);

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

static void _chartctrl_ensure_visible(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	xrect_t xr = { 0 };

	if (!ptd->table)
		return;

	_chartctrl_table_rect(widget, ptd->table, &xr);

	widget_ensure_visible(widget, &xr, 1);
}
/*********************************************************************************************************/
int noti_chart_owner(res_win_t widget, unsigned long code, link_t_ptr ptr, link_t_ptr ilk, void* data)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	NOTICE_CHART nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;

	nf.chart = ptr;
	nf.table = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_chart_reset_select(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	link_t_ptr ilk;
	int count = 0;

	ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
	while (ilk)
	{
		if (get_chart_table_selected(ilk))
		{
			set_chart_table_selected(ilk, 0);
			noti_chart_owner(widget, NC_CHARTTABLESELECTED, ptd->chart, ilk, NULL);

			count++;
		}

		ilk = get_chart_next_table(ptd->chart, ilk);
	}

	if (count)
	{
		widget_update(widget, NULL, 0);
	}
}

void noti_chart_table_selected(res_win_t widget, link_t_ptr ilk)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	xrect_t xr;
	bool_t b_check;

	b_check = get_chart_table_selected(ilk);

	if (b_check)
		set_chart_table_selected(ilk, 0);
	else
		set_chart_table_selected(ilk, 1);

	noti_chart_owner(widget, NC_CHARTTABLESELECTED, ptd->chart, ilk, NULL);

	_chartctrl_table_rect(widget, ilk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t noti_chart_table_changing(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->table);

	if (noti_chart_owner(widget, NC_CHARTTABLECHANGING, ptd->chart, ptd->table, NULL))
		return (bool_t)0;

	_chartctrl_table_rect(widget, ptd->table, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->table = NULL;

	widget_update(widget, &xr, 0);

	return (bool_t)1;
}

void noti_chart_table_changed(res_win_t widget, link_t_ptr ilk)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->table);

	ptd->table = ilk;

	_chartctrl_table_rect(widget, ptd->table, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	noti_chart_owner(widget, NC_CHARTTABLECHANGED, ptd->chart, ilk, NULL);
}

void noti_chart_table_enter(res_win_t widget, link_t_ptr ilk)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = ilk;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
}

void noti_chart_table_leave(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
}

void noti_chart_table_hover(res_win_t widget, long x, long y)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->hover != NULL);

	pt.x = x;
	pt.y = y;
	noti_chart_owner(widget, NC_CHARTTABLEHOVER, ptd->chart, ptd->hover, (void*)&pt);
}

void noti_chart_table_drag(res_win_t widget, long x, long y)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->table);

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
	noti_chart_owner(widget, NC_CHARTTABLEDRAG, ptd->chart, ptd->table, (void*)&pt);
}

void noti_chart_table_drop(res_win_t widget, long x, long y)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	
	xpoint_t pt;
	xrect_t xr;
	long cx, cy;

	XDL_ASSERT(ptd->table);

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

	calc_chart_table_rect(ptd->chart, ptd->table, &xr);

	widget_rect_to_pt(widget, &xr);

	if (xr.x + cx < 0 || xr.y + cy < 0)
		return;

	_chartctrl_done(widget);

	pt.x = xr.x + cx;
	pt.y = xr.y + cy;

	widget_point_to_tm(widget, &pt);

	pt.fx = (float)((long)(pt.fx));
	pt.fy = (float)((long)(pt.fy));

	set_chart_table_x(ptd->table, pt.fx);
	set_chart_table_y(ptd->table, pt.fy);

	widget_update(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_chart_owner(widget, NC_CHARTTABLEDROP, ptd->chart, ptd->table, (void*)&pt);
}

void noti_chart_table_sizing(res_win_t widget, int hint, long x, long y)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->table);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	if (hint == CHART_HINT_HORZ_SPLIT)
	{
		widget_set_cursor(widget,CURSOR_SIZENS);
	}
	else if (hint == CHART_HINT_VERT_SPLIT)
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

	_chartctrl_table_rect(widget, ptd->table, &xr);

	noti_chart_owner(widget, NC_CHARTTABLESIZING, ptd->chart, ptd->table, (void*)&xr);
}

void noti_chart_table_sized(res_win_t widget, long x, long y)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	float minw, minh, fw, fh;
	int hint;
	xrect_t xr;
	xsize_t xs;

	XDL_ASSERT(ptd->table);

	ptd->cur_x = x;
	ptd->cur_y = y;

	ptd->b_size = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	hint = ptd->org_hint;

	minw = CHART_TABLE_MIN_WIDTH;
	minh = CHART_TABLE_MIN_HEIGHT;

	xs.cx = ptd->cur_x - ptd->org_x;
	xs.cy = ptd->cur_y - ptd->org_y;

	if (!xs.cx && !xs.cy)
		return;

	widget_size_to_tm(widget, &xs);

	_chartctrl_table_rect(widget, ptd->table, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	fw = get_chart_table_width(ptd->table);
	fh = get_chart_table_height(ptd->table);

	fw += xs.fx;
	fh += xs.fy;

	if (fw < minw)
		fw = minw;

	if (fh < minh)
		fh = minh;

	fw = (float)((long)fw);
	fh = (float)((long)fh);

	_chartctrl_done(widget);

	if (hint == CHART_HINT_HORZ_SPLIT)
	{
		set_chart_table_height(ptd->table, fh);
	}
	else if (hint == CHART_HINT_VERT_SPLIT)
	{
		set_chart_table_width(ptd->table, fw);
	}
	else
	{
		set_chart_table_width(ptd->table, fw);
		set_chart_table_height(ptd->table, fh);
	}

	_chartctrl_table_rect(widget, ptd->table, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	_chartctrl_table_rect(widget, ptd->table, &xr);

	noti_chart_owner(widget, NC_CHARTTABLESIZED, ptd->chart, ptd->table, (void*)&xr);
}

void noti_chart_calc(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
}

/*******************************************************************************/
int hand_chart_create(res_win_t widget, void* data)
{
	chart_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (chart_delta_t*)xmem_alloc(sizeof(chart_delta_t));
	xmem_zero((void*)ptd, sizeof(chart_delta_t));

	ptd->stack = create_stack_table();

	SETCHARTDELTA(widget, ptd);

	return 0;
}

void hand_chart_destroy(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	_chartctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETCHARTDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_chart_size(res_win_t widget, int code, const xsize_t* prs)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	chartctrl_redraw(widget);
}

void hand_chart_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_chart_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->chart)
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

void hand_chart_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	xpoint_t pt;

	if (!ptd->chart)
		return;

	if (ptd->b_size || ptd->b_drag)
	{
		ptd->cur_x = pxp->x;
		ptd->cur_y = pxp->y;
		widget_update(widget, NULL, 0);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_chart_hint(ptd->chart, &pt, &ilk);

	if (nHint == CHART_HINT_HORZ_SPLIT && ilk == ptd->table && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_chart_table_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZENS);
	}
	else if (nHint == CHART_HINT_VERT_SPLIT && ilk == ptd->table && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_chart_table_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZEWE);
	}
	else if (nHint == CHART_HINT_CROSS_SPLIT && ilk == ptd->table && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_chart_table_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZEALL);
	}
	else if (nHint == CHART_HINT_TABLE && ilk == ptd->table && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_chart_table_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == CHART_HINT_NONE)
	{
		if (ptd->opera == WD_OPERA_CONTROL)
			widget_set_cursor(widget, CURSOR_IBEAM);
		else
			widget_set_cursor(widget, CURSOR_ARROW);
	}

	if (widget_is_hotvoer(widget))
	{
		if (nHint == CHART_HINT_TABLE && !ptd->hover && ilk)
		{
			noti_chart_table_enter(widget, ilk);
			return;
		}

		if (nHint == CHART_HINT_TABLE && ptd->hover && ptd->hover != ilk)
		{
			noti_chart_table_leave(widget);
			return;
		}

		if (nHint != CHART_HINT_TABLE && ptd->hover)
		{
			noti_chart_table_leave(widget);
		}
	}
}

void hand_chart_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	if (ptd->hover)
		noti_chart_table_hover(widget, pxp->x, pxp->y);
}

void hand_chart_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	if (ptd->hover)
		noti_chart_table_leave(widget);
}

void hand_chart_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->chart)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_chart_hint(ptd->chart, &pt, &ilk);
	bRe = (ilk == ptd->table) ? 1 : 0;

	switch (nHint)
	{
	case CHART_HINT_TABLE:
		if (ptd->opera == WD_OPERA_CONTROL || widget_key_state(widget, KEY_CONTROL))
		{
			noti_chart_table_selected(widget, ilk);
		}
		break;
	case CHART_HINT_NONE:
		noti_chart_reset_select(widget);
		break;
	}
}

void hand_chart_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->chart)
		return;

	if (ptd->b_size)
	{
		noti_chart_table_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag)
	{
		noti_chart_table_drop(widget, pxp->x, pxp->y);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_chart_hint(ptd->chart, &pt, &ilk);

	noti_chart_owner(widget, NC_CHARTLBCLK, ptd->chart, ilk, (void*)pxp);

	bRe = (ilk == ptd->table) ? 1 : 0;

	if (ptd->table && !bRe)
	{
		if (!noti_chart_table_changing(widget))
			return;
	}

	if (ilk && !bRe)
	{
		noti_chart_table_changed(widget, ilk);
	}
}

void hand_chart_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	noti_chart_owner(widget, NC_CHARTDBCLK, ptd->chart, ptd->table, (void*)pxp);
}

void hand_chart_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;
}

void hand_chart_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	noti_chart_owner(widget, NC_CHARTRBCLK, ptd->chart, ptd->table, (void*)pxp);
}

void hand_chart_keydown(res_win_t widget, int nKey)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	float x, y, w, h, m;
	bool_t ks;
	link_t_ptr ilk;

	if (!ptd->chart)
		return;

	if (!ptd->table)
		return;

	if ((nKey == KEY_UP || nKey == KEY_DOWN || nKey == KEY_LEFT || nKey == KEY_RIGHT))
	{
		ks = widget_key_state(widget, KEY_SHIFT);
		m = 1;

		if (ks)
			noti_chart_owner(widget, NC_CHARTTABLESIZING, ptd->chart, ptd->table, NULL);
		else
			noti_chart_owner(widget, NC_CHARTTABLEDRAG, ptd->chart, ptd->table, NULL);

		_chartctrl_done(widget);

		ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
		while (ilk)
		{
			if (ilk != ptd->table && !get_chart_table_selected(ilk))
			{
				ilk = get_chart_next_table(ptd->chart, ilk);
				continue;
			}

			x = get_chart_table_x(ilk);
			y = get_chart_table_y(ilk);
			w = get_chart_table_width(ilk);
			h = get_chart_table_height(ilk);

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

			set_chart_table_x(ilk, x);
			set_chart_table_y(ilk, y);
			set_chart_table_width(ilk, w);
			set_chart_table_height(ilk, h);

			ilk = get_chart_next_table(ptd->chart, ilk);
		}

		widget_update(widget, NULL, 0);

		if (ks)
			noti_chart_owner(widget, NC_CHARTTABLESIZED, ptd->chart, ptd->table, NULL);
		else
			noti_chart_owner(widget, NC_CHARTTABLEDROP, ptd->chart, ptd->table, NULL);
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

void hand_chart_char(res_win_t widget, tchar_t nChar)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;
}

void hand_chart_copy(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	_chartctrl_copy(widget);
}

void hand_chart_cut(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	_chartctrl_done(widget);

	if (!_chartctrl_cut(widget))
	{
		_chartctrl_discard(widget);
	}
}

void hand_chart_paste(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	_chartctrl_done(widget);

	if (!_chartctrl_paste(widget))
	{
		_chartctrl_discard(widget);
	}
}

void hand_chart_undo(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;

	_chartctrl_undo(widget);
}

void hand_chart_notice(res_win_t widget, NOTICE* pnt)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;
}


void hand_chart_erase(res_win_t widget, res_ctx_t dc)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	if (!ptd->chart)
		return;
}

void hand_chart_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
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

	if (!ptd->chart)
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

	widget_get_xbrush(widget, &xb);

	widget_get_xpen(widget, &xp);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_chart(pif, &cb, ptd->chart);

	//draw focus
	if (ptd->table)
	{
		_chartctrl_table_rect(widget, ptd->table, &xr);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);

		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	//draw check
	parse_xcolor(&xc, DEF_ALPHA_COLOR);

	ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
	while (ilk)
	{
		if (get_chart_table_selected(ilk))
		{
			_chartctrl_table_rect(widget, ilk, &xr);
			alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
		}
		ilk = get_chart_next_table(ptd->chart, ilk);
	}

	if (ptd->b_drag)
	{
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		_chartctrl_table_rect(widget, ptd->table, &xr);

		xr.x += (ptd->cur_x - ptd->org_x);
		xr.y += (ptd->cur_y - ptd->org_y);

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}
	else if (ptd->b_size)
	{
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		_chartctrl_table_rect(widget, ptd->table, &xr);

		if (ptd->org_hint == CHART_HINT_VERT_SPLIT)
		{
			xr.w = (ptd->cur_x - xr.x);
		}
		else if (ptd->org_hint == CHART_HINT_HORZ_SPLIT)
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

res_win_t chartctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_chart_create)
		EVENT_ON_DESTROY(hand_chart_destroy)

		EVENT_ON_ERASE(hand_chart_erase)
		EVENT_ON_PAINT(hand_chart_paint)

		EVENT_ON_SIZE(hand_chart_size)

		EVENT_ON_SCROLL(hand_chart_scroll)
		EVENT_ON_WHEEL(hand_chart_wheel)

		EVENT_ON_KEYDOWN(hand_chart_keydown)
		EVENT_ON_CHAR(hand_chart_char)

		EVENT_ON_MOUSE_MOVE(hand_chart_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_chart_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_chart_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_chart_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_chart_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_chart_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_chart_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_chart_rbutton_up)

		EVENT_ON_NOTICE(hand_chart_notice)

		EVENT_ON_COPY(hand_chart_copy)
		EVENT_ON_CUT(hand_chart_cut)
		EVENT_ON_PASTE(hand_chart_paste)
		EVENT_ON_UNDO(hand_chart_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void chartctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_chart_doc(ptr));

	ptd->chart = ptr;
	ptd->table = NULL;

	chartctrl_redraw(widget);
}

link_t_ptr chartctrl_detach(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->chart;
	ptd->chart = NULL;
	ptd->table = NULL;

	widget_update(widget, NULL, 0);

	return data;
}

link_t_ptr chartctrl_fetch(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->chart;
}

void chartctrl_redraw(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->chart)
		return;

	b_valid = 0;
	ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->table)
			b_valid = 1;

		noti_chart_owner(widget, NC_CHARTTABLECALCED, ptd->chart, ilk, NULL);

		ilk = get_chart_next_table(ptd->chart, ilk);
	}
	
	noti_chart_owner(widget, NC_CHARTCALCED, ptd->chart, NULL, NULL);

	if (!b_valid)
	{
		ptd->table = NULL;
	}
	ptd->hover = NULL;

	_chartctrl_reset_page(widget);

	widget_update_window(widget);

	widget_update(widget, NULL, 0);
}

void chartctrl_redraw_table(res_win_t widget, link_t_ptr ilk)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->chart)
		return;

#ifdef _DEBUG
	if(!is_chart_table(ptd->chart, ilk))
		return;
#endif

	noti_chart_owner(widget, NC_CHARTTABLECALCED, ptd->chart, ilk, NULL);

	_chartctrl_table_rect(widget, ilk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

void chartctrl_tabskip(res_win_t widget, int nSkip)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	link_t_ptr ilk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->chart)
		return;

	switch (nSkip)
	{
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (ptd->table == NULL)
			ilk = get_chart_prev_table(ptd->chart, LINK_LAST);
		else
			ilk = get_chart_prev_table(ptd->chart, ptd->table);
		break;
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (ptd->table == NULL)
			ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
		else
			ilk = get_chart_next_table(ptd->chart, ptd->table);
		break;
	case WD_TAB_HOME:
		ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
		break;
	case WD_TAB_END:
		ilk = get_chart_prev_table(ptd->chart, LINK_LAST);
		break;
	}

	chartctrl_set_focus_table(widget, ilk);
}

bool_t chartctrl_set_focus_table(res_win_t widget, link_t_ptr ilk)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->chart)
		return 0;

	if (ilk == LINK_FIRST)
		ilk = get_chart_next_table(ptd->chart, LINK_FIRST);
	else if (ilk == LINK_LAST)
		ilk = get_chart_prev_table(ptd->chart, LINK_LAST);

	bRe = (ilk == ptd->table) ? (bool_t)1 : (bool_t)0;
	if (bRe)
		return (bool_t)1;

	if (ptd->table && !bRe)
	{
		if (!noti_chart_table_changing(widget))
			return (bool_t)0;
	}

	if (ilk && !bRe)
	{
		noti_chart_table_changed(widget, ilk);

		_chartctrl_ensure_visible(widget);
	}

	return (bool_t)1;
}

link_t_ptr chartctrl_get_focus_table(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->chart)
		return NULL;

	return ptd->table;
}

void chartctrl_set_opera(res_win_t widget, int opera)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->opera = opera;
}

int chartctrl_get_opera(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->opera;
}

void chartctrl_get_chart_table_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->chart)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_chart_table(ptd->chart, ilk));
#endif

	_chartctrl_table_rect(widget, ilk, pxr);
}

bool_t chartctrl_get_dirty(res_win_t widget)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->chart)
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void chartctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	chart_delta_t* ptd = GETCHARTDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->chart)
		return;

	if (bDirty)
		_chartctrl_done(widget);
	else
		_chartctrl_clean(widget);
}
