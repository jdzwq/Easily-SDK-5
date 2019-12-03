/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc grid control document

	@module	gridctrl.c | implement file

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

typedef struct _grid_delta_t{
	link_t_ptr grid;
	link_t_ptr row;
	link_t_ptr col;
	link_t_ptr hover;
	link_t_ptr fix;

	long org_x, org_y;
	long cur_x, cur_y;
	short cur_page;

	res_win_t editor;
	res_win_t hsc;
	res_win_t vsc;

	bool_t b_drag_row, b_drag_col;
	bool_t b_size_row, b_size_col;
	bool_t b_alarm;

	bool_t b_auto;
	bool_t b_lock;

	link_t_ptr stack;
}grid_delta_t;

#define GETGRIDDELTA(ph) 	(grid_delta_t*)widget_get_user_delta(ph)
#define SETGRIDDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/**********************************************************************************************/
static void _gridctrl_done(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->grid);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->grid, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->grid, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->grid, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->grid, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _gridctrl_undo(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->grid);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_grid_doc(ptd->grid);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->grid, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->grid, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		gridctrl_redraw(widget, 1);
	}
}

static void _gridctrl_discard(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _gridctrl_clean(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _gridctrl_copy(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->grid);

	if (!grid_is_design(ptd->grid))
		return 0;

	if (!get_col_selected_count(ptd->grid) && !ptd->col)
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_grid_doc();
	ilk = get_next_col(ptd->grid, LINK_FIRST);
	while (ilk)
	{
		if (get_col_selected(ilk) || ilk == ptd->col)
		{
			nlk = insert_col(dom, LINK_LAST);
			copy_dom_node(nlk, ilk);
		}

		ilk = get_next_col(ptd->grid, ilk);
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

		destroy_grid_doc(dom);
		return 0;
	}

	clipboard_close();

	destroy_grid_doc(dom);
	return 1;
}

static bool_t _gridctrl_cut(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->grid);

	if (!_gridctrl_copy(widget))
		return 0;

	ilk = get_next_col(ptd->grid, LINK_FIRST);
	while (ilk)
	{
		nxt = get_next_col(ptd->grid, ilk);

		if (get_col_selected(ilk) || ilk == ptd->col)
		{
			delete_col(ilk);
		}
		ilk = nxt;
	}

	gridctrl_redraw(widget, 1);

	return 1;
}

static bool_t _gridctrl_paste(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, plk;

	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->grid);

	if (!grid_is_design(ptd->grid))
		return 0;

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

	if (!is_grid_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	plk = (ptd->col) ? ptd->col : LINK_LAST;
	while (nlk = get_next_col(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_grid_colset(dom), nlk);
		attach_dom_node(get_grid_colset(ptd->grid), plk, nlk);

		xsprintf(sz_name, _T("col%d"), get_col_count(ptd->grid));
		set_col_name(nlk, sz_name);
		set_col_title(nlk, sz_name);
	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);

	gridctrl_redraw(widget, 1);

	return 1;
}

static void _gridctrl_row_rect(res_win_t widget, link_t_ptr rlk, xrect_t* pxr)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_grid_row_rect(&cb, ptd->grid, ptd->cur_page, rlk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _gridctrl_col_rect(res_win_t widget, link_t_ptr rlk, link_t_ptr clk, xrect_t* pxr)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_grid_col_rect(&cb, ptd->grid, ptd->cur_page, rlk, clk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _gridctrl_cell_rect(res_win_t widget, link_t_ptr rlk, link_t_ptr clk, xrect_t* pxr)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_grid_cell_rect(&cb, ptd->grid, ptd->cur_page, rlk, clk, pxr);

	widget_rect_to_pt(widget, pxr);
}

float _gridctrl_page_width(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr clk;
	bool_t b_design;
	float w;

	b_design = grid_is_design(ptd->grid);

	w = get_grid_rowbar_width(ptd->grid);

	clk = get_prev_col(ptd->grid, LINK_LAST);
	while (clk)
	{
		if (!get_col_visible(clk) && !b_design)
			clk = get_prev_col(ptd->grid, clk);
		else
			break;
	}

	while (clk)
	{
		w += get_col_width(clk);
		clk = get_prev_col(ptd->grid, clk);
	}

	return w;
}

static void _gridctrl_reset_page(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = _gridctrl_page_width(widget);

	if (xs.fx < get_grid_width(ptd->grid))
	{
		xs.fx = get_grid_width(ptd->grid);
	}
	xs.fy = get_grid_height(ptd->grid);

	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = get_grid_rowbar_height(ptd->grid);
	xs.fy = get_grid_rowbar_height(ptd->grid);
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

void _gridctrl_ensure_visible(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	int page;
	xrect_t xr = { 0 };
	canvbox_t cb;

	if (ptd->row)
	{
		widget_get_canv_rect(widget, &cb);

		page = calc_grid_row_page( &cb, ptd->grid,ptd->row);
		if (page && page != ptd->cur_page)
		{
			ptd->cur_page = page;
			widget_redraw(widget, NULL, 0);
		}
	}

	_gridctrl_cell_rect(widget, ptd->row, ptd->col, &xr);
	widget_ensure_visible(widget, &xr, 1);
}

/*************************************************************************************************/
int noti_grid_owner(res_win_t widget, unsigned long code, link_t_ptr grid, link_t_ptr rlk, link_t_ptr clk, void* data)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	NOTICE_GRID nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.grid = grid;
	nf.row = rlk;
	nf.col = clk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_grid_reset_check(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr rlk;
	int count = 0;
	bool_t b;

	b = get_rowset_checked(ptd->grid);

	rlk = get_next_row(ptd->grid, LINK_FIRST);
	while (rlk)
	{
		set_row_checked(rlk, b);
		noti_grid_owner(widget, NC_ROWCHECKED, ptd->grid, rlk, NULL, NULL);

		rlk = get_next_row(ptd->grid, rlk);
	}

	widget_redraw(widget, NULL, 0);
}

void noti_grid_reset_select(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr clk;
	int count = 0;

	clk = get_next_col(ptd->grid, LINK_FIRST);
	while (clk)
	{
		if (get_col_selected(clk))
		{
			set_col_selected(clk, 0);
			noti_grid_owner(widget, NC_COLSELECTED, ptd->grid, NULL, clk, NULL);

			count++;
		}
		clk = get_next_col(ptd->grid, clk);
	}

	if (!count)
		return;

	widget_redraw(widget, NULL, 0);
}

void noti_grid_col_sizing(res_win_t widget, long x, long y)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	ptd->b_size_col = 1;
	ptd->org_x = x;
	ptd->org_y = y;

	if (widget_is_hotvoer(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget, CURSOR_SIZEWE);

	noti_grid_owner(widget, NC_COLSIZING, ptd->grid, NULL, ptd->col, NULL);
}

void noti_grid_col_sized(res_win_t widget, long x, long y)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	float mw;
	xrect_t xrCol, xrCli;
	xsize_t xs = { 0 };

	if (widget_is_hotvoer(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	xs.cx = x - ptd->org_x;
	if (!xs.cx)
		return;

	widget_size_to_tm(widget, &xs);

	mw = get_col_width(ptd->col);
	mw += xs.fx;
	mw = (float)(long)mw;
	if (mw < 2 * DEF_SPLIT_FEED)
		mw = 2 * DEF_SPLIT_FEED;

	_gridctrl_done(widget);

	mw = (float)(long)mw;

	if (ptd->col)
	{
		set_col_width(ptd->col, mw);

		_gridctrl_col_rect(widget, NULL, ptd->col, &xrCol);

		widget_get_client_rect(widget, &xrCli);
		xrCli.x = xrCol.x;
		xrCli.w -= xrCol.x;
	}
	else
	{
		set_grid_rowbar_width(ptd->grid, mw);

		widget_get_client_rect(widget, &xrCli);
	}

	widget_redraw(widget, &xrCli, 0);

	ptd->b_size_col = 0;
	noti_grid_owner(widget, NC_COLSIZED, ptd->grid, NULL, ptd->col, NULL);
}

void noti_grid_row_sizing(res_win_t widget, long x, long y)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	ptd->b_size_row = 1;
	ptd->org_x = x;
	ptd->org_y = y;

	if (widget_is_hotvoer(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_SIZENS);

	noti_grid_owner(widget, NC_ROWSIZING, ptd->grid, ptd->row, NULL, NULL);
}

void noti_grid_row_sized(res_win_t widget, long x, long y)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	float mh;
	xsize_t xs = { 0 };

	if (widget_is_hotvoer(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	xs.cy = y - ptd->org_y;
	if (!xs.cy)
		return;

	widget_size_to_tm(widget, &xs);

	mh = get_grid_rowbar_height(ptd->grid);
	mh += xs.fy;
	mh = (float)(long)mh;
	if (mh < 2 * DEF_SPLIT_FEED)
		mh = 2 * DEF_SPLIT_FEED;

	_gridctrl_done(widget);

	mh = (float)(long)mh;
	set_grid_rowbar_height(ptd->grid, mh);

	widget_redraw(widget, NULL, 0);

	ptd->b_size_row = 0;
	noti_grid_owner(widget, NC_ROWSIZED, ptd->grid, ptd->row, NULL, NULL);
}

void noti_grid_col_drag(res_win_t widget, long x, long y)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->col);

	if (widget_is_hotvoer(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	ptd->b_drag_col = 1;
	ptd->org_x = x;
	ptd->org_y = y;

	pt.x = x;
	pt.y = y;
	noti_grid_owner(widget, NC_COLDRAG, ptd->grid, NULL, ptd->col, (void*)&pt);
}

void noti_grid_col_drop(res_win_t widget, long x, long y)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	int nHint;
	link_t_ptr root, rlk, clk;
	xpoint_t pt;
	canvbox_t cb;

	XDL_ASSERT(ptd->col);

	if (widget_is_hotvoer(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	ptd->b_drag_col = 0;

	widget_get_canv_rect(widget, &cb);

	pt.x = x;
	pt.y = y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_grid_hint(&cb, &pt, ptd->grid, ptd->cur_page, &rlk, &clk);
	if (clk != ptd->col)
	{
		root = get_dom_child_node_root(get_grid_colset(ptd->grid));

		_gridctrl_done(widget);

		if (clk)
		{
			switch_link_before(root, clk, ptd->col);
		}
		else
		{
			if (x < ptd->org_x)
				switch_link_first(root, ptd->col);
			else
				switch_link_last(root, ptd->col);
		}
	}

	widget_redraw(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_grid_owner(widget, NC_COLDROP, ptd->grid, NULL, ptd->col, (void*)&pt);
}

void noti_grid_row_drag(res_win_t widget, long x, long y)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->row);

	if (widget_is_hotvoer(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	ptd->b_drag_row = 1;
	ptd->org_x = x;
	ptd->org_y = y;

	pt.x = x;
	pt.y = y;
	noti_grid_owner(widget, NC_ROWDRAG, ptd->grid, ptd->row, NULL, (void*)&pt);
}

void noti_grid_row_drop(res_win_t widget, long x, long y)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xpoint_t pt;
	xrect_t xrRow, xrCli;

	XDL_ASSERT(ptd->row);

	if (widget_is_hotvoer(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	ptd->b_drag_row = 0;

	_gridctrl_row_rect(widget, ptd->row, &xrRow);
	widget_get_client_rect(widget, &xrCli);
	ft_inter_rect(&xrCli, &xrRow);

	widget_redraw(widget, &xrCli, 0);

	pt.x = x;
	pt.y = y;
	noti_grid_owner(widget, NC_ROWDROP, ptd->grid, ptd->row, NULL, (void*)&pt);
}

void noti_grid_row_checked(res_win_t widget, link_t_ptr rlk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xrect_t xr;

	if (!get_grid_showcheck(ptd->grid))
		return;

	XDL_ASSERT(rlk);

	set_row_checked(rlk, ((get_row_checked(rlk)) ? 0 : 1));

	noti_grid_owner(widget, NC_ROWCHECKED, ptd->grid, rlk, NULL, NULL);

	_gridctrl_row_rect(widget, rlk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

void noti_grid_col_selected(res_win_t widget, link_t_ptr clk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xrect_t xr;

	if (!grid_is_design(ptd->grid))
		return;

	XDL_ASSERT(clk);

	set_col_selected(clk, ((get_col_selected(clk)) ? 0 : 1));

	noti_grid_owner(widget, NC_COLSELECTED, ptd->grid, NULL, clk, NULL);

	_gridctrl_col_rect(widget, ptd->row, clk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

bool_t noti_grid_row_insert(res_win_t widget, link_t_ptr rlk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(rlk);

	if (noti_grid_owner(widget, NC_ROWINSERT, ptd->grid, rlk, NULL, NULL))
		return 0;

	return 1;
}

bool_t noti_grid_row_delete(res_win_t widget, link_t_ptr rlk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(rlk);

	if (noti_grid_owner(widget, NC_ROWDELETE, ptd->grid, rlk, NULL, NULL))
		return 0;

	return 1;
}

bool_t noti_grid_row_changing(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->row);

	if (noti_grid_owner(widget, NC_ROWCHANGING, ptd->grid, ptd->row, NULL, NULL))
		return 0;

	_gridctrl_row_rect(widget, ptd->row, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->row = NULL;

	widget_redraw(widget, &xr, 0);

	return 1;
}

void noti_grid_row_changed(res_win_t widget, link_t_ptr rlk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(rlk && !ptd->row);

	_gridctrl_row_rect(widget, rlk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->row = rlk;

	widget_redraw(widget, &xr, 0);

	noti_grid_owner(widget, NC_ROWCHANGED, ptd->grid, ptd->row, NULL, NULL);
}

void noti_grid_col_changing(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->col);

	noti_grid_owner(widget, NC_COLCHANGING, ptd->grid, NULL, ptd->col, NULL);

	_gridctrl_col_rect(widget, ptd->row, ptd->col, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->col = NULL;

	widget_redraw(widget, &xr, 0);
}

void noti_grid_col_changed(res_win_t widget, link_t_ptr clk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(clk && !ptd->col);

	_gridctrl_col_rect(widget, ptd->row, clk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->col = clk;

	widget_redraw(widget, &xr, 0);

	noti_grid_owner(widget, NC_COLCHANGED, ptd->grid, NULL, ptd->col, NULL);
}

void noti_grid_col_enter(res_win_t widget, link_t_ptr clk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(clk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = clk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_grid_col_leave(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_grid_col_hover(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	noti_grid_owner(widget, NC_COLHOVER, ptd->grid, NULL, ptd->hover, NULL);
}

void noti_grid_begin_edit(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	const tchar_t* editor;
	const tchar_t* text;
	xrect_t xr = { 0 };
	link_t_ptr data;

	EDITDELTA fd = { 0 };

	clr_mod_t ob = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd->row && ptd->col);
	
	if (widget_is_valid(ptd->editor))
		return;

	if (ptd->b_lock)
		return;

	if (grid_is_design(ptd->grid) || !get_col_focusable(ptd->col) || get_row_locked(ptd->row))
	{
		return;
	}

	widget_get_xfont(widget, &xf);
	parse_xfont_from_style(&xf, get_col_style_ptr(ptd->col));
	widget_get_color_mode(widget, &ob);

	_gridctrl_cell_rect(widget, ptd->row, ptd->col, &xr);
	pt_expand_rect(&xr, -1, -1);

	editor = get_col_editor_ptr(ptd->col);

	if (compare_text(editor, -1, ATTR_EDITOR_FIREEDIT, -1, 0) == 0)
	{
		if (noti_grid_owner(widget, NC_CELLEDITING, ptd->grid, ptd->row, ptd->col, NULL))
			return;

		ptd->editor = fireedit_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREEDIT);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_cell_text_ptr(ptd->row, ptd->col);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRENUM, -1, 0) == 0)
	{
		if (noti_grid_owner(widget, NC_CELLEDITING, ptd->grid, ptd->row, ptd->col, NULL))
			return;

		ptd->editor = firenum_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRENUM);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_cell_text_ptr(ptd->row, ptd->col);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREDATE, -1, 0) == 0)
	{
		if (noti_grid_owner(widget, NC_CELLEDITING, ptd->grid, ptd->row, ptd->col, NULL))
			return;

		ptd->editor = firedate_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREDATE);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_cell_text_ptr(ptd->row, ptd->col);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRETIME, -1, 0) == 0)
	{
		if (noti_grid_owner(widget, NC_CELLEDITING, ptd->grid, ptd->row, ptd->col, NULL))
			return;

		ptd->editor = firetime_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRETIME);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_cell_text_ptr(ptd->row, ptd->col);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRELIST, -1, 0) == 0)
	{
		if (noti_grid_owner(widget, NC_CELLEDITING, ptd->grid, ptd->row, ptd->col, NULL))
			return;

		data = get_col_options_table(ptd->col);
		if (!data)
			return;

		ptd->editor = firelist_create(widget, &xr, data);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRELIST);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_cell_text_ptr(ptd->row, ptd->col);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREWORDS, -1, 0) == 0)
	{
		if (noti_grid_owner(widget, NC_CELLEDITING, ptd->grid, ptd->row, ptd->col, (void*)&fd))
			return;

		data = (link_t_ptr)fd.data;
		ptd->editor = firewords_create(widget, &xr, data);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREWORDS);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_cell_text_ptr(ptd->row, ptd->col);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREGRID, -1, 0) == 0)
	{
		if (noti_grid_owner(widget, NC_CELLEDITING, ptd->grid, ptd->row, ptd->col, (void*)&fd))
			return;

		data = (link_t_ptr)fd.data;
		if (!data)
			return;

		ptd->editor = firegrid_create(widget, &xr, data);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREGRID);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
}

void noti_grid_commit_edit(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	const tchar_t* editor;
	tchar_t* text;
	res_win_t editctrl;
	link_t_ptr ilk,rlk_new;
	int n_ret = 0;

	EDITDELTA fd = { 0 };

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->row && ptd->col);

	editor = get_col_editor_ptr(ptd->col);

	if (compare_text(editor, -1, ATTR_EDITOR_FIREEDIT, -1, 0) == 0)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		n_ret = noti_grid_owner(widget, NC_CELLCOMMIT, ptd->grid, ptd->row, ptd->col, (void*)text);
		if (n_ret == GRID_NOTICE_ACCEPT)
		{
			gridctrl_set_cell_text(widget, ptd->row, ptd->col, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRENUM, -1, 0) == 0)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		n_ret = noti_grid_owner(widget, NC_CELLCOMMIT, ptd->grid, ptd->row, ptd->col, (void*)text);
		if (n_ret == GRID_NOTICE_ACCEPT)
		{
			gridctrl_set_cell_text(widget, ptd->row, ptd->col, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREDATE, -1, 0) == 0)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		n_ret = noti_grid_owner(widget, NC_CELLCOMMIT, ptd->grid, ptd->row, ptd->col, (void*)text);
		if (n_ret == GRID_NOTICE_ACCEPT)
		{
			gridctrl_set_cell_text(widget, ptd->row, ptd->col, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRETIME, -1, 0) == 0)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		n_ret = noti_grid_owner(widget, NC_CELLCOMMIT, ptd->grid, ptd->row, ptd->col, (void*)text);
		if (n_ret == GRID_NOTICE_ACCEPT)
		{
			gridctrl_set_cell_text(widget, ptd->row, ptd->col, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRELIST, -1, 0) == 0)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		n_ret = noti_grid_owner(widget, NC_CELLCOMMIT, ptd->grid, ptd->row, ptd->col, (void*)text);
		if (n_ret == GRID_NOTICE_ACCEPT)
		{
			gridctrl_set_cell_text(widget, ptd->row, ptd->col, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREWORDS, -1, 0) == 0)
	{
		fd.data = firewords_get_data(ptd->editor);
		ilk = firewords_get_item(ptd->editor);
		if (ilk)
		{
			editbox_set_text(ptd->editor, get_words_item_text_ptr(ilk));
		}

		fd.text = editbox_get_text_ptr(ptd->editor);
		n_ret = noti_grid_owner(widget, NC_CELLCOMMIT, ptd->grid, ptd->row, ptd->col, (void*)&fd);
		if (n_ret == GRID_NOTICE_ACCEPT)
		{
			gridctrl_set_cell_text(widget, ptd->row, ptd->col, fd.text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREGRID, -1, 0) == 0)
	{
		fd.data = firegrid_get_data(ptd->editor);
		fd.item = firegrid_get_item(ptd->editor);
		n_ret = noti_grid_owner(widget, NC_CELLCOMMIT, ptd->grid, ptd->row, ptd->col, (void*)&fd);
		if (n_ret == GRID_NOTICE_ACCEPT)
		{
			gridctrl_redraw_row(widget, ptd->row, 1);
		}
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);

	if (n_ret == GRID_NOTICE_REJECT)
	{
		return;
	}
	else if (n_ret == GRID_NOTICE_DELETE)
	{
		gridctrl_delete_row(widget, ptd->row);
		return;
	}

	if (ptd->b_auto && (ptd->row == get_prev_visible_row(ptd->grid, LINK_LAST)) && (ptd->col == ptd->fix || ptd->col == get_prev_focusable_col(ptd->grid, LINK_LAST)))
	{
		rlk_new = insert_row(ptd->grid, LINK_LAST);
		set_row_state(rlk_new, dsNewClean);

		if (!noti_grid_row_insert(widget, rlk_new))
		{
			delete_row(rlk_new);
			return;
		}

		gridctrl_redraw_row(widget, rlk_new, 1);
		gridctrl_set_focus_cell(widget, rlk_new, get_next_focusable_col(ptd->grid, LINK_FIRST));
	}
	else
	{
		if (ptd->col == ptd->fix)
		{
			gridctrl_tabskip(widget, WD_TAB_DOWN);
		}
		else if (ptd->col == get_prev_focusable_col(ptd->grid, LINK_LAST))
		{
			rlk_new = get_next_visible_row(ptd->grid, ptd->row);
			if (rlk_new)
			{
				gridctrl_set_focus_cell(widget, rlk_new, get_next_focusable_col(ptd->grid, LINK_FIRST));
			}
		}
		else
		{
			gridctrl_tabskip(widget, WD_TAB_RIGHT);
		}
	}
}

void noti_grid_rollback_edit(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	res_win_t editctrl;
	const tchar_t* editor;
	EDITDELTA fd = { 0 };

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->row && ptd->col);

	editor = get_col_editor_ptr(ptd->col);

	if (compare_text(editor, -1, ATTR_EDITOR_FIREWORDS, -1, 0) == 0)
	{
		fd.data = firewords_get_data(ptd->editor);
		fd.text = NULL;
		noti_grid_owner(widget, NC_CELLROLLBACK, ptd->grid, ptd->row, ptd->col, (void*)&fd);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREGRID, -1, 0) == 0)
	{
		fd.data = firegrid_get_data(ptd->editor);
		fd.item = NULL;
		noti_grid_owner(widget, NC_CELLROLLBACK, ptd->grid, ptd->row, ptd->col, (void*)&fd);
	}
	else
	{
		noti_grid_owner(widget, NC_CELLROLLBACK, ptd->grid, ptd->row, ptd->col, NULL);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_grid_reset_editor(res_win_t widget, bool_t bCommit)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_grid_commit_edit(widget);
		else
			noti_grid_rollback_edit(widget);
	}
}
/*******************************************************************************************/

int hand_grid_create(res_win_t widget, void* data)
{
	grid_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (grid_delta_t*)xmem_alloc(sizeof(grid_delta_t));
	ptd->stack = create_stack_table();

	SETGRIDDELTA(widget, ptd);

	return 0;
}

void hand_grid_destroy(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_grid_reset_editor(widget, 0);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	_gridctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETGRIDDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_grid_size(res_win_t widget, int code, const xsize_t* prs)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	gridctrl_redraw(widget, 1);
}

void hand_grid_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_grid_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	widget_get_scroll(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widget_hand_scroll(widget, bHorz, nLine))
	{
		if (!bHorz && !(widget_get_style(widget) & WD_STYLE_VSCROLL))
		{
			if (!widget_is_valid(ptd->vsc))
			{
				ptd->vsc = show_vertbox(widget);
			}
		}

		if (bHorz && !(widget_get_style(widget) & WD_STYLE_HSCROLL))
		{
			if (!widget_is_valid(ptd->hsc))
			{
				ptd->hsc = show_horzbox(widget);
			}
		}

		return;
	}

	win = widget_get_parent(widget);

	if (widget_is_valid(win))
	{
		widget_scroll(win, bHorz, nLine);
	}
}

void hand_grid_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr row, col;
	int nHint;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->grid)
		return;

	if (ptd->b_size_row || ptd->b_size_col || ptd->b_drag_row || ptd->b_drag_col)
	{
		ptd->cur_x = pxp->x;
		ptd->cur_y = pxp->y;
		return;
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_grid_hint(&cb, &pt, ptd->grid, ptd->cur_page, &row, &col);

	if (nHint == GRID_HINT_HORZ_SPLIT && row == ptd->row && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_grid_row_sizing(widget, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget,CURSOR_SIZENS);
	}
	else if (nHint == GRID_HINT_VERT_SPLIT && col == ptd->col && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_grid_col_sizing(widget, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget,CURSOR_SIZEWE);
	}
	else if (nHint == GRID_HINT_COLBAR && col == ptd->col && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_grid_col_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == GRID_HINT_ROWBAR && row == ptd->row && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_grid_row_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == GRID_HINT_NONE)
	{
		widget_set_cursor(widget, CURSOR_ARROW);
	}
}

void hand_grid_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	if (ptd->hover)
		noti_grid_col_hover(widget);
}

void hand_grid_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	if (ptd->hover)
		noti_grid_col_leave(widget);
}

void hand_grid_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	if (!grid_is_design(ptd->grid))
	{
		if (!ptd->row && ptd->col && get_col_sortable(ptd->col))
		{
			sort_grid_col(ptd->grid, ptd->col);

			widget_redraw(widget, NULL, 0);
		}
	}

	noti_grid_owner(widget, NC_GRIDDBCLK, ptd->grid, ptd->row, ptd->col, (void*)pxp);
}

void hand_grid_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr rlk, clk;
	int nHint;
	bool_t bReCol, bReRow;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_grid_hint(&cb, &pt, ptd->grid, ptd->cur_page, &rlk, &clk);

	bReRow = (rlk == ptd->row) ? 1 : 0;
	bReCol = (clk == ptd->col) ? 1 : 0;

	if (nHint == GRID_HINT_NULBAR)
	{
		if (get_grid_showcheck(ptd->grid))
		{
			if (get_rowset_checked(ptd->grid))
				set_rowset_checked(ptd->grid, 0);
			else
				set_rowset_checked(ptd->grid, 1);

			noti_grid_reset_check(widget);
		}
	}
	else if (nHint == GRID_HINT_ROWBAR)
	{
		noti_grid_row_checked(widget, rlk);
	}
	else if (nHint == GRID_HINT_COLBAR)
	{
		if (widget_key_state(widget, KEY_CONTROL))
		{
			if (grid_is_design(ptd->grid))
				noti_grid_col_selected(widget, clk);
			else
				ptd->fix = ptd->col;
		}
	}
	else if (nHint == GRID_HINT_NONE)
	{
		if (!widget_key_state(widget, KEY_CONTROL))
		{
			if(grid_is_design(ptd->grid))
				noti_grid_reset_select(widget);
		}
	}
}

void hand_grid_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr rlk, clk;
	int nHint;
	bool_t bReCol, bReRow;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->grid)
		return;

	if (ptd->b_size_row)
	{
		noti_grid_row_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_size_col)
	{
		noti_grid_col_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag_row)
	{
		noti_grid_row_drop(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag_col)
	{
		noti_grid_col_drop(widget, pxp->x, pxp->y);
		return;
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_grid_hint(&cb, &pt, ptd->grid, ptd->cur_page, &rlk, &clk);

	if (nHint == GRID_HINT_NULBAR || nHint == GRID_HINT_ROWBAR)
	{
		return;
	}

	bReRow = (rlk == ptd->row) ? 1 : 0;
	bReCol = (clk == ptd->col) ? 1 : 0;

	if (bReRow && bReCol)
	{
		if (ptd->row && ptd->col && !ptd->b_lock && !grid_is_design(ptd->grid) && get_col_editable(ptd->col) && !get_row_locked(ptd->row))
		{
			widget_post_key(widget, KEY_ENTER);
		}
		return;
	}

	if (!bReRow && ptd->row)
	{
		if (!noti_grid_row_changing(widget))
			bReRow = 1;
	}

	if (!bReCol && ptd->col)
	{
		noti_grid_col_changing(widget);
	}

	if (!bReCol && clk)
		noti_grid_col_changed(widget, clk);

	if (!bReRow && rlk)
		noti_grid_row_changed(widget, rlk);

	if (!bReRow || !bReCol)
	{
		if (ptd->row && ptd->col)
		{
			_gridctrl_ensure_visible(widget);
			noti_grid_owner(widget, NC_CELLSETFOCUS, ptd->grid, ptd->row, ptd->col, NULL);
		}
	}

	noti_grid_owner(widget, NC_GRIDLBCLK, ptd->grid, ptd->row, ptd->col, (void*)pxp);
}

void hand_grid_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);
}

void hand_grid_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	noti_grid_owner(widget, NC_GRIDRBCLK, ptd->grid, ptd->row, ptd->col, (void*)pxp);
}

void hand_grid_keydown(res_win_t widget, int nKey)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	if (grid_is_design(ptd->grid))
	{
		if ((nKey == _T('z') || nKey == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
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
		switch (nKey)
		{
		case KEY_ENTER:
			if (ptd->row && ptd->col)
			{
				noti_grid_begin_edit(widget);
			}
			break;
		case KEY_SPACE:
			if (ptd->row)
			{
				noti_grid_row_checked(widget, ptd->row);
			}
			break;
		case KEY_TAB:
			gridctrl_tabskip(widget, WD_TAB_RIGHT);
			break;
		case KEY_LEFT:
			gridctrl_tabskip(widget, WD_TAB_LEFT);
			break;
		case KEY_RIGHT:
			gridctrl_tabskip(widget, WD_TAB_RIGHT);
			break;
		case KEY_UP:
			gridctrl_tabskip(widget, WD_TAB_UP);
			break;
		case KEY_DOWN:
			gridctrl_tabskip(widget, WD_TAB_DOWN);
			break;
		case KEY_END:
			gridctrl_tabskip(widget, WD_TAB_END);
			break;
		case VK_HOME:
			gridctrl_tabskip(widget, WD_TAB_HOME);
			break;
		case KEY_PAGEUP:
			gridctrl_tabskip(widget, WD_TAB_PAGEUP);
			break;
		case KEY_PAGEDOWN:
			gridctrl_tabskip(widget, WD_TAB_PAGEDOWN);
			break;
		}
	}
}

void hand_grid_char(res_win_t widget, tchar_t nChar)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	if (IS_VISIBLE_CHAR(nChar) && !widget_is_valid(ptd->editor))
	{
		hand_grid_keydown(widget, KEY_ENTER);
	}

	if (IS_VISIBLE_CHAR(nChar) && widget_is_valid(ptd->editor))
	{
		widget_post_char(ptd->editor, nChar);
	}
}

void hand_grid_copy(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	_gridctrl_copy(widget);
}

void hand_grid_cut(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	_gridctrl_done(widget);

	if (!_gridctrl_cut(widget))
	{
		_gridctrl_discard(widget);
	}
}

void hand_grid_paste(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	_gridctrl_done(widget);

	if (!_gridctrl_paste(widget))
	{
		_gridctrl_discard(widget);
	}
}

void hand_grid_undo(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;

	_gridctrl_undo(widget);
}

void hand_grid_child_command(res_win_t widget, int code, var_long data)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_grid_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_grid_rollback_edit(widget);
		break;
	}
}

void hand_grid_notice(res_win_t widget, NOTICE* pnt)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;
}

void hand_grid_erase(res_win_t widget, res_ctx_t dc)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	if (!ptd->grid)
		return;
}

void hand_grid_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	res_ctx_t rdc;
	xrect_t xr;
	xsize_t xs;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	link_t_ptr clk;
	bool_t b_design;

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->grid)
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

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	b_design = grid_is_design(ptd->grid);

	if (widget_can_paging(widget))
	{
		xmem_copy((void*)&xc, (void*)&pif->clr_ico, sizeof(xcolor_t));
		draw_corner(canv, &xc, (const xrect_t*)&cb);

		if (b_design)
		{
			parse_xcolor(&xc, xp.color);
			lighten_xcolor(&xc, DEF_SOFT_DARKEN);
			draw_ruler(pif->canvas, &xc, (const xrect_t*)&cb);
		}
	}

	draw_grid_page(pif, &cb, ptd->grid, ptd->cur_page);

	if (b_design)
	{
		clk = get_next_col(ptd->grid, LINK_FIRST);
		while (clk)
		{
			if (clk == ptd->col)
			{
				parse_xcolor(&xc, DEF_ENABLE_COLOR);

				_gridctrl_cell_rect(widget, NULL, ptd->col, &xr);

				draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
			}

			if (get_col_selected(clk))
			{
				_gridctrl_cell_rect(widget, NULL, clk, &xr);
				pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

				alphablend_rect_raw(rdc, &xc, &xr, ALPHA_SOFT);
			}
			clk = get_next_col(ptd->grid, clk);
		}
	}
	else
	{
		//draw focus
		if (ptd->row)
		{
			if (ptd->b_lock)
			{
				_gridctrl_row_rect(widget, ptd->row, &xr);
				pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

				parse_xcolor(&xc, DEF_ALPHA_COLOR);
				alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
			}
			else if (ptd->col)
			{
				_gridctrl_cell_rect(widget, ptd->row, ptd->col, &xr);

				if (ptd->b_alarm)
				{
					parse_xcolor(&xc, DEF_ALARM_COLOR);
				}
				else
				{
					if (get_col_editable(ptd->col))
						parse_xcolor(&xc, DEF_ENABLE_COLOR);
					else
						parse_xcolor(&xc, DEF_DISABLE_COLOR);
				}

				draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
			}
		}
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/******************************************************************************************/
res_win_t gridctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_grid_create)
		EVENT_ON_DESTROY(hand_grid_destroy)

		EVENT_ON_ERASE(hand_grid_erase)
		EVENT_ON_PAINT(hand_grid_paint)

		EVENT_ON_SIZE(hand_grid_size)

		EVENT_ON_SCROLL(hand_grid_scroll)
		EVENT_ON_WHEEL(hand_grid_wheel)

		EVENT_ON_KEYDOWN(hand_grid_keydown)
		EVENT_ON_CHAR(hand_grid_char)

		EVENT_ON_MOUSE_MOVE(hand_grid_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_grid_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_grid_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_grid_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_grid_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_grid_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_grid_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_grid_rbutton_up)

		EVENT_ON_NOTICE(hand_grid_notice)
		EVENT_ON_CHILD_COMMAND(hand_grid_child_command)

		EVENT_ON_COPY(hand_grid_copy)
		EVENT_ON_CUT(hand_grid_cut)
		EVENT_ON_PASTE(hand_grid_paste)
		EVENT_ON_UNDO(hand_grid_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void gridctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_grid_doc(ptr));

	ptd->grid = ptr;
	ptd->row = NULL;
	ptd->col = NULL;

	ptd->cur_page = 1;

	gridctrl_redraw(widget, 1);

	if (!ptd->b_lock && ptd->b_auto && !get_next_visible_row(ptr, LINK_FIRST))
	{
		gridctrl_insert_row(widget, LINK_LAST);
	}
}

link_t_ptr gridctrl_detach(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	noti_grid_reset_editor(widget, 0);

	data = ptd->grid;
	ptd->grid = NULL;

	ptd->cur_page = 0;

	widget_redraw(widget, NULL, 0);

	return data;
}

link_t_ptr gridctrl_fetch(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->grid;
}

void gridctrl_redraw(res_win_t widget, bool_t bCalc)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr rlk, clk;
	bool_t bValid;
	int bSum, sumcols;
	bool_t b;
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 0);

	bValid = 0;
	rlk = get_next_visible_row(ptd->grid, LINK_FIRST);
	while (rlk)
	{
		if (rlk == ptd->row)
			bValid = 1;

		if (bCalc)
		{
			calc_grid_row(ptd->grid, rlk);
		}
			
		noti_grid_owner(widget, NC_ROWCALCED,ptd->grid, rlk, NULL, NULL);

		rlk = get_next_visible_row(ptd->grid, rlk);
	}

	if (!bValid)
		ptd->row = NULL;

	bValid = 0;
	bSum = get_grid_showsum(ptd->grid);
	sumcols = 0;
	clk = get_next_visible_col(ptd->grid, LINK_FIRST);
	while (clk)
	{
		if (clk == ptd->col)
			bValid = 1;

		if (bSum && get_col_sum_mode_ptr(clk))
		{
			sum_grid_col(ptd->grid, clk);
			sumcols++;
		}

		noti_grid_owner(widget, NC_COLCALCED, ptd->grid, NULL, clk, NULL);

		clk = get_next_visible_col(ptd->grid, clk);
	}

	if (!bValid)
		ptd->col = NULL;

	ptd->hover = NULL;

	noti_grid_owner(widget, NC_GRIDCALCED,ptd->grid, NULL, NULL, NULL);

	b = (widget_get_style(widget) & WD_STYLE_PAGING) ? 1 : 0;
	if (!b)
	{
		widget_get_client_rect(widget, &xr);
		widget_rect_to_tm(widget, &xr);
		set_grid_height(ptd->grid, xr.fh);
	}

	_gridctrl_reset_page(widget);

	widget_update(widget);
}

void gridctrl_tabskip(res_win_t widget, int dir)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr rlk, clk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	rlk = ptd->row;
	clk = ptd->col;

	switch (dir)
	{
	case WD_TAB_LEFT:
		if (clk == NULL)
			clk = get_prev_focusable_col(ptd->grid, LINK_LAST);
		else
			clk = get_prev_focusable_col(ptd->grid, clk);

		if (clk)
		{
			gridctrl_set_focus_cell(widget, rlk, clk);
		}
		break;
	case WD_TAB_RIGHT:
		if (clk == NULL)
			clk = get_next_focusable_col(ptd->grid, LINK_FIRST);
		else
			clk = get_next_focusable_col(ptd->grid, clk);

		if (clk)
		{
			gridctrl_set_focus_cell(widget, rlk, clk);
		}
		break;
	case WD_TAB_UP:
		if (rlk)
		{
			rlk = get_prev_visible_row(ptd->grid, rlk);

			if (rlk)
			{
				gridctrl_set_focus_cell(widget, rlk, clk);
			}
		}
		break;
	case WD_TAB_DOWN:
		if (rlk == NULL)
			rlk = get_next_visible_row(ptd->grid, LINK_FIRST);
		else
			rlk = get_next_visible_row(ptd->grid, rlk);

		if (rlk)
		{
			gridctrl_set_focus_cell(widget, rlk, clk);
		}
		break;
	case WD_TAB_HOME:
		gridctrl_move_first_page(widget);
		break;
	case WD_TAB_END:
		gridctrl_move_last_page(widget);
		break;
	case WD_TAB_PAGEUP:
		gridctrl_move_prev_page(widget);
		break;
	case WD_TAB_PAGEDOWN:
		gridctrl_move_next_page(widget);
		break;
	}
}

bool_t gridctrl_set_cell_text(res_win_t widget, link_t_ptr rlk, link_t_ptr clk, const tchar_t* szText)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	const tchar_t* text;
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_grid_row(ptd->grid, rlk));
	XDL_ASSERT(is_grid_col(ptd->grid, clk));
#endif

	text = get_cell_text_ptr(rlk,clk);
	if (compare_data(szText, text, get_col_data_type_ptr(clk)) == 0)
		return 1;

	if (veValid != verify_text(szText, get_col_data_type_ptr(clk), get_col_nullable(clk), get_col_data_len(clk), get_col_data_min_ptr(clk), get_col_data_max_ptr(clk)))
		return 0;

	set_cell_text(rlk, clk, szText, -1);
	set_cell_dirty(rlk, clk, 1);
	set_row_dirty(rlk);

	noti_grid_owner(widget, NC_CELLUPDATE, ptd->grid, rlk, clk, NULL);

	if (get_col_fireable(clk))
	{
		if (calc_grid_row(ptd->grid, rlk))
		{
			widget_redraw(widget, NULL, 0);
		}
	}

	_gridctrl_row_rect(widget, rlk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
	widget_redraw(widget, &xr, 0);

	return 1;
}

bool_t gridctrl_delete_row(res_win_t widget, link_t_ptr rlk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr nlk;

	XDL_ASSERT(ptd != NULL);
	
	if (!ptd->grid)
		return 0;

	noti_grid_reset_editor(widget, 0);

	if (ptd->b_lock)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_grid_row(ptd->grid, rlk));
#endif

	if (!noti_grid_row_delete(widget, rlk))
		return 0;

	if (rlk == ptd->row)
	{
		noti_grid_row_changing(widget);
	}

	nlk = get_next_visible_row(ptd->grid, rlk);
	if (!nlk)
		nlk = get_prev_visible_row(ptd->grid, rlk);

	set_row_delete(rlk);

	gridctrl_redraw(widget, 1);

	if (nlk)
		gridctrl_set_focus_cell(widget, nlk, ptd->col);

	return 1;
}

link_t_ptr gridctrl_insert_row(res_win_t widget, link_t_ptr pre)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr rlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return NULL;

	noti_grid_reset_editor(widget, 0);

	if (ptd->b_lock)
		return NULL;

	if (pre == LINK_FIRST)
		pre = get_next_visible_row(ptd->grid, LINK_FIRST);
	else if (pre == LINK_LAST)
		pre = get_prev_visible_row(ptd->grid, LINK_LAST);
	else
	{
#ifdef _DEBUG
		XDL_ASSERT(is_grid_row(ptd->grid, pre));
#endif
	}

	if (!pre) pre = LINK_FIRST;

	rlk = insert_row(ptd->grid, pre);
	set_row_state(rlk, dsNewClean);

	if (!noti_grid_row_insert(widget, rlk))
	{
		delete_row(rlk);
		return NULL;
	}

	gridctrl_redraw_row(widget, rlk, 0);

	gridctrl_set_focus_cell(widget, rlk, LINK_FIRST);

	return rlk;
}

bool_t gridctrl_copy_row(res_win_t widget, link_t_ptr srcGrid, link_t_ptr srcRow)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr destGrid = ptd->grid;
	link_t_ptr destRow;
	link_t_ptr clk_dest, clk_src;
	const tchar_t* cname;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return 0;

	destRow = ptd->row;

	if (!destRow)
		return 0;

	if (get_row_locked(destRow))
		return 0;

	clk_dest = get_next_col(destGrid, LINK_FIRST);
	while (clk_dest)
	{
		cname = get_col_name_ptr(clk_dest);
		clk_src = get_col(srcGrid, cname);
		if (clk_src)
		{
			set_cell_text(destRow, clk_dest, get_cell_text_ptr(srcRow, clk_src), -1);
			set_cell_dirty(destRow, clk_dest, 1);
			set_row_dirty(destRow);

			sum_grid_col(ptd->grid, clk_dest);
		}

		clk_dest = get_next_col(destGrid, clk_dest);
	}
	set_row_dirty(destRow);

	gridctrl_redraw_row(widget, destRow, 1);

	return 1;
}

void gridctrl_redraw_row(res_win_t widget, link_t_ptr rlk, bool_t bCalc)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xrect_t xr, xrCli;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_grid_row(ptd->grid, rlk));
#endif

	if (bCalc)
	{
		calc_grid_row(ptd->grid, rlk);
	}
		
	noti_grid_owner(widget, NC_ROWCALCED,ptd->grid, rlk, NULL, NULL);

	_gridctrl_row_rect(widget, rlk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_get_client_rect(widget, &xrCli);
	xrCli.y = xr.y;
	xrCli.h -= xr.y;

	widget_redraw(widget, &xrCli, 0);
}

void gridctrl_redraw_col(res_win_t widget, link_t_ptr clk, bool_t bCalc)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	xrect_t xr,xrCli;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_grid_col(ptd->grid, clk));
#endif

	if (bCalc && get_grid_showsum(ptd->grid) && get_col_sum_mode_ptr(clk))
	{
		sum_grid_col(ptd->grid, clk);
	}

	noti_grid_owner(widget, NC_COLCALCED, ptd->grid, NULL, clk, NULL);

	_gridctrl_col_rect(widget, ptd->row, clk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_get_client_rect(widget, &xrCli);
	xrCli.x = xr.x;
	xrCli.w -= xr.x;

	widget_redraw(widget, &xr, 0);
}

bool_t gridctrl_set_focus_cell(res_win_t widget, link_t_ptr rlk, link_t_ptr clk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	bool_t bReRow, bReCol;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return 0;

	if (rlk == LINK_FIRST)
		rlk = get_next_visible_row(ptd->grid, LINK_FIRST);
	else if (rlk == LINK_LAST)
		rlk = get_prev_visible_row(ptd->grid, LINK_LAST);

	if (clk == LINK_FIRST)
		clk = get_next_focusable_col(ptd->grid, LINK_FIRST);
	else if (clk == LINK_LAST)
		clk = get_prev_focusable_col(ptd->grid, LINK_LAST);

	bReRow = (rlk == ptd->row) ? 1 : 0;
	bReCol = (clk == ptd->col) ? 1 : 0;

	if (bReRow && bReCol)
	{
		return 1;
	}

	if (!bReRow && ptd->row)
	{
		if (!noti_grid_row_changing(widget))
			return 0;
	}

	if (!bReCol && ptd->col)
	{
		noti_grid_col_changing(widget);
	}

	if (!bReRow || !bReCol)
	{
		if (ptd->row && ptd->col)
			noti_grid_owner(widget, NC_CELLKILLFOCUS,ptd->grid, ptd->row, ptd->col, NULL);
	}

	if (!bReCol && clk)
		noti_grid_col_changed(widget, clk);

	if (!bReRow && rlk)
		noti_grid_row_changed(widget, rlk);

	if (!bReRow || !bReCol)
	{
		if (ptd->row && ptd->col)
			noti_grid_owner(widget, NC_CELLSETFOCUS,ptd->grid, ptd->row, ptd->col, NULL);
	}

	_gridctrl_ensure_visible(widget);

	return 1;
}

link_t_ptr gridctrl_get_focus_col(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return NULL;

	return ptd->col;
}

link_t_ptr gridctrl_get_focus_row(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return NULL;

	return ptd->row;
}

void gridctrl_auto_insert(res_win_t widget, bool_t bAuto)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_auto = bAuto;
}

void gridctrl_set_fixed(res_win_t widget, link_t_ptr clk)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->fix = clk;
}

link_t_ptr	gridctrl_get_fixed(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->fix;
}

bool_t gridctrl_verify(res_win_t widget, bool_t bAlarm)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr rlk, clk;
	int code;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return 1;

	noti_grid_reset_editor(widget, 0);

	code = verify_grid_doc(ptd->grid, &rlk, &clk);
	if (veValid != code)
	{
		gridctrl_set_focus_cell(widget, rlk, clk);
		ptd->b_alarm = 1;
		return 0;
	}
	else
		return 1;
}

void gridctrl_accept(res_win_t widget, bool_t bAccept)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr nxt, rlk;
	int rs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, bAccept);

	rlk = get_next_row(ptd->grid, LINK_FIRST);
	while (rlk)
	{
		nxt = get_next_row(ptd->grid, rlk);

		rs = get_row_state(rlk);
		if (rs == dsNewClean)
			delete_row(rlk);

		rlk = nxt;
	}

	gridctrl_redraw(widget, 0);
}

bool_t gridctrl_is_update(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return 0;

	return (get_update_row_count(ptd->grid)) ? 1 : 0;
}

void gridctrl_move_first_page(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	nCurPage = ptd->cur_page;

	if (nCurPage != 1)
	{
		nCurPage = 1;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

void gridctrl_move_prev_page(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	nCurPage = ptd->cur_page;

	if (nCurPage > 1)
	{
		nCurPage--;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

void gridctrl_move_next_page(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_grid_pages(&cb, ptd->grid);

	if (nCurPage < nMaxPage)
	{
		nCurPage++;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

void gridctrl_move_last_page(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_grid_pages(&cb, ptd->grid);

	if (nCurPage != nMaxPage)
	{
		nCurPage = nMaxPage;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

void gridctrl_move_to_page(res_win_t widget, int page)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_grid_pages(&cb, ptd->grid);

	if (page > 0 && page != nCurPage && page <= nMaxPage)
	{
		nCurPage = page;
		ptd->cur_page = nCurPage;

		widget_redraw(widget, NULL, 0);
	}
}

int gridctrl_get_max_page(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return 0;

	widget_get_canv_rect(widget, &cb);

	return calc_grid_pages(&cb, ptd->grid);
}

int gridctrl_get_cur_page(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return 0;

	return ptd->cur_page;
}

void gridctrl_find(res_win_t widget, const tchar_t* token)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	link_t_ptr clk,rlk;
	int tlen;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 1);

	clk = get_next_visible_col(ptd->grid, LINK_FIRST);
	if (!clk)
		return;

	tlen = xslen(token);

	
	if (tlen)
		rlk = (ptd->row) ? get_next_visible_row(ptd->grid, ptd->row) : get_next_visible_row(ptd->grid, LINK_FIRST);
	else
		rlk = NULL;
	
	while (rlk)
	{
		if (xsnicmp(get_cell_text_ptr(rlk, clk), token, tlen) == 0)
			break;

		rlk = get_next_visible_row(ptd->grid, rlk);
	}

	if (rlk)
		gridctrl_set_focus_cell(widget, rlk, clk);
	else
		gridctrl_set_focus_cell(widget, NULL, NULL);
}

void gridctrl_filter(res_win_t widget, const tchar_t* token)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	link_t_ptr rlk,clk;
	int tlen;
	const tchar_t* text;
	bool_t b_redraw, b_hidden;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	noti_grid_reset_editor(widget, 0);

	gridctrl_set_focus_cell(widget, NULL, NULL);

	clk = get_next_visible_col(ptd->grid, LINK_FIRST);
	if (!clk)
		return;

	tlen = xslen(token);

	b_redraw = 0;
	rlk = get_next_row(ptd->grid, LINK_FIRST);
	while (rlk)
	{
		b_hidden = 1;

		if (is_null(token))
		{
			b_hidden = 0;
		}
		else
		{
			text = get_cell_text_ptr(rlk,clk);

			if (xsnicmp(text, token, tlen) == 0)
				b_hidden = 0;
		}

		if (b_hidden != get_row_hidden(rlk))
		{
			set_row_hidden(rlk, b_hidden);
			b_redraw = 1;
		}

		rlk = get_next_row(ptd->grid, rlk);
	}

	if (b_redraw)
		gridctrl_redraw(widget, 1);
}

void gridctrl_popup_size(res_win_t widget, xsize_t* pse)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	int count;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	count = get_visible_row_count(ptd->grid);
	if (count < 5)
		count = 5;
	if (count > 7)
		count = 7;

	pse->fx = _gridctrl_page_width(widget);
	pse->fy = get_grid_title_height(ptd->grid) + get_grid_rowbar_height(ptd->grid) * count;

	widget_size_to_pt(widget, pse);

	widget_adjust_size(widget_get_style(widget), pse);
}

void gridctrl_get_cell_rect(res_win_t widget, link_t_ptr rlk, link_t_ptr clk, xrect_t* pxr)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	_gridctrl_cell_rect(widget, rlk, clk, pxr);
}

bool_t gridctrl_get_lock(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

void gridctrl_set_lock(res_win_t widget, bool_t bLock)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t gridctrl_get_dirty(res_win_t widget)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return 0;

	if (!grid_is_design(ptd->grid))
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void gridctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	grid_delta_t* ptd = GETGRIDDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->grid)
		return;

	if (!grid_is_design(ptd->grid))
		return;

	if (bDirty)
		_gridctrl_done(widget);
	else
		_gridctrl_clean(widget);
}
