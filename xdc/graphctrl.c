/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc graph control document

	@module	graphctrl.c | graph control implement file

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

typedef struct _graph_delta_t{
	link_t_ptr graph;
	link_t_ptr gax;
	link_t_ptr yax;
	link_t_ptr xax;
	link_t_ptr hover;

	long org_x, org_y;
	long cur_x, cur_y;
	long cur_page;

	res_win_t editor;
	tchar_t pch[CHS_LEN + 1];

	bool_t b_drag_xax, b_drag_yax;
	bool_t b_size_xax, b_size_yax;

	bool_t b_auto;
	bool_t b_lock;

	link_t_ptr stack;
}graph_delta_t;

#define GETGRAPHDELTA(ph) 	(graph_delta_t*)widget_get_user_delta(ph)
#define SETGRAPHDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/******************************************************************************************/
static void _graphctrl_done(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->graph);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->graph, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->graph, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->graph, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->graph, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _graphctrl_undo(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->graph);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_graph_doc(ptd->graph);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->graph, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->graph, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		graphctrl_redraw(widget, 1);
	}
}

static void _graphctrl_discard(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _graphctrl_clean(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _graphctrl_copy(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->graph);

	if (!graph_is_design(ptd->graph))
		return 0;

	if (!get_yax_selected_count(ptd->graph) && !ptd->yax)
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_graph_doc();
	ilk = get_next_yax(ptd->graph, LINK_FIRST);
	while (ilk)
	{
		if (get_yax_selected(ilk) || ilk == ptd->yax)
		{
			nlk = insert_yax(dom, LINK_LAST);
			copy_dom_node(nlk, ilk);
		}

		ilk = get_next_yax(ptd->graph, ilk);
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

		destroy_graph_doc(dom);
		return 0;
	}

	clipboard_close();

	destroy_graph_doc(dom);
	return 1;
}

static bool_t _graphctrl_cut(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->graph);

	if (!_graphctrl_copy(widget))
		return 0;

	ilk = get_next_yax(ptd->graph, LINK_FIRST);
	while (ilk)
	{
		nxt = get_next_yax(ptd->graph, ilk);

		if (get_yax_selected(ilk) || ilk == ptd->yax)
		{
			delete_yax(ilk);
		}
		ilk = nxt;
	}

	graphctrl_redraw(widget, 1);

	return 1;
}

static bool_t _graphctrl_paste(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, plk;

	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->graph);

	if (!graph_is_design(ptd->graph))
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

	if (!is_graph_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	plk = (ptd->yax) ? ptd->yax : LINK_LAST;

	while (nlk = get_next_yax(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_graph_yaxset(dom), nlk);
		attach_dom_node(get_graph_yaxset(ptd->graph), plk, nlk);

		xsprintf(sz_name, _T("%s%d"), get_yax_count(ptd->graph));
		set_yax_name(nlk, sz_name);
		set_yax_title(nlk, sz_name);

	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);

	graphctrl_redraw(widget, 1);

	return 1;
}

static void _graphctrl_xaxbar_rect(res_win_t widget, link_t_ptr xlk, xrect_t* pxr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_graph_coor_rect(&cb, ptd->graph, ptd->cur_page, xlk, NULL, pxr);
	
	widget_rect_to_pt(widget, pxr);
}

static void _graphctrl_xax_rect(res_win_t widget, link_t_ptr xlk, xrect_t* pxr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_graph_xax_rect(&cb, ptd->graph, ptd->cur_page, xlk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _graphctrl_gaxbar_rect(res_win_t widget, link_t_ptr glk, xrect_t* pxr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_graph_gax_rect(&cb, ptd->graph, glk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _graphctrl_yax_rect(res_win_t widget, link_t_ptr ylk, xrect_t* pxr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_graph_yax_rect(&cb, ptd->graph, ylk, pxr);

	pxr->fw = get_graph_width(ptd->graph);

	widget_rect_to_pt(widget, pxr);
}

static void _graphctrl_yaxbar_rect(res_win_t widget, link_t_ptr ylk, xrect_t* pxr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_graph_yax_rect(&cb, ptd->graph, ylk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _graphctrl_coor_rect(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_graph_coor_rect(&cb, ptd->graph, ptd->cur_page, xlk, ylk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _graphctrl_reset_page(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_graph_width(ptd->graph);
	xs.fy = get_graph_height(ptd->graph);

	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = get_graph_yaxbar_height(ptd->graph);
	xs.fy = get_graph_xaxbar_width(ptd->graph);

	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

void _graphctrl_ensure_visible(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;
	canvbox_t cb;

	int page;

	if (!ptd->xax || !ptd->yax)
		return;

	widget_get_canv_rect(widget, &cb);

	page = calc_graph_xax_page(&cb, ptd->graph, ptd->xax);
	if (page != ptd->cur_page)
	{
		ptd->cur_page = page;
		widget_update(widget, NULL, 0);
	}

	_graphctrl_coor_rect(widget, ptd->xax, ptd->yax, &xr);

	widget_ensure_visible(widget, &xr, 1);
}

/***********************************************************************************************************************/
int noti_graph_owner(res_win_t widget, unsigned long code, link_t_ptr graph, link_t_ptr xlk, link_t_ptr ylk, link_t_ptr glk, void* data)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	NOTICE_GRAPH nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;
	nf.ret = 0;

	nf.graph = graph;
	nf.xax = xlk;
	nf.yax = ylk;
	nf.gax = glk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_graph_reset_select(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr ylk;
	int count = 0;

	ylk = get_next_yax(ptd->graph, LINK_FIRST);
	while (ylk)
	{
		if (get_yax_selected(ylk))
		{
			set_yax_selected(ylk, 0);
			noti_graph_owner(widget, NC_YAXSELECTED, ptd->graph, NULL, ylk, NULL, NULL);

			count++;
		}
		ylk = get_next_yax(ptd->graph, ylk);
	}

	if (count)
		widget_update(widget, NULL, 0);
}

void noti_graph_reset_check(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr xlk;
	int count = 0;

	if (!get_graph_showcheck(ptd->graph))
		return;

	xlk = get_next_xax(ptd->graph, LINK_FIRST);
	while (xlk)
	{
		if (get_xax_checked(xlk))
		{
			set_xax_checked(xlk, 0);
			noti_graph_owner(widget, NC_XAXCHECKED, ptd->graph, xlk, NULL, NULL, NULL);

			count++;
		}
		xlk = get_next_xax(ptd->graph, xlk);
	}

	if (count)
		widget_update(widget, NULL, 0);
}

void noti_graph_xax_sizing(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	ptd->b_size_xax = (bool_t)1;
	ptd->org_x = x;
	ptd->org_y = y;

	widget_set_capture(widget, 1);
	widget_set_cursor(widget,CURSOR_SIZEWE);
}

void noti_graph_xax_sized(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	float mw;
	xsize_t xs = { 0 };

	widget_set_capture(widget, 0);
	widget_set_cursor(widget, CURSOR_ARROW);

	ptd->b_size_xax = (bool_t)0;

	xs.cx = x - ptd->org_x;
	if (!xs.cx)
		return;

	widget_size_to_tm(widget, &xs);

	mw = get_graph_xaxbar_width(ptd->graph);
	mw += xs.fx;
	if (mw < 2 * DEF_SPLIT_FEED)
		mw = 2 * DEF_SPLIT_FEED;

	_graphctrl_done(widget);

	mw = (float)(long)mw;
	set_graph_xaxbar_width(ptd->graph, mw);

	widget_update(widget, NULL, 0);

	noti_graph_owner(widget, NC_XAXSIZED, ptd->graph, ptd->xax, NULL, NULL, NULL);
}

void noti_graph_yax_sizing(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	ptd->b_size_yax = (bool_t)1;
	ptd->org_y = x;
	ptd->org_y = y;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_SIZENS);
}

void noti_graph_yax_sized(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	float mh;
	xsize_t xs = { 0 };

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	ptd->b_size_yax = (bool_t)0;

	xs.cy = y - ptd->org_y;
	if (!xs.cy)
		return;

	widget_size_to_tm(widget, &xs);

	mh = get_graph_yaxbar_height(ptd->graph);
	mh += xs.fy;
	if (mh <  2 * DEF_SPLIT_FEED)
		mh = 2 * DEF_SPLIT_FEED;

	_graphctrl_done(widget);

	mh = (float)(long)mh;
	set_graph_yaxbar_height(ptd->graph, mh);

	widget_update(widget, NULL, 0);

	noti_graph_owner(widget, NC_YAXSIZED, ptd->graph, NULL, ptd->yax, NULL, NULL);
}

void noti_graph_yax_drag(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->yax);

	ptd->b_drag_yax = (bool_t)1;
	ptd->org_x = x;
	ptd->org_y = y;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_graph_owner(widget, NC_YAXDRAG, ptd->graph, NULL, ptd->yax, NULL, (void*)&pt);
}

void noti_graph_yax_drop(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	int nHint;
	link_t_ptr root, xlk, ylk, glk;
	xpoint_t pt;
	canvbox_t cb;

	XDL_ASSERT(ptd->yax);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	ptd->b_drag_yax = (bool_t)0;

	widget_get_canv_rect(widget, &cb);

	pt.x = x;
	pt.y = y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_graph_hint(&cb, &pt, ptd->graph, ptd->cur_page, &xlk, &ylk, &glk);
	if (ylk != ptd->yax)
	{
		root = get_dom_child_node_root(get_graph_yaxset(ptd->graph));

		_graphctrl_done(widget);

		if (ylk)
		{
			if (y < ptd->org_y)
				switch_link_before(root, ylk, ptd->yax);
			else
				switch_link_after(root, ylk, ptd->yax);
		}
		else
		{
			if (y < ptd->org_y)
				switch_link_first(root, ptd->yax);
			else
				switch_link_last(root, ptd->yax);
		}
	}

	widget_update(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_graph_owner(widget, NC_YAXDROP, ptd->graph, NULL, ptd->yax, NULL, (void*)&pt);
}

void noti_graph_xax_drag(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->xax);

	ptd->b_drag_xax = (bool_t)1;
	ptd->org_x = x;
	ptd->org_y = y;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_graph_owner(widget, NC_XAXDRAG, ptd->graph, ptd->xax, NULL, NULL, (void*)&pt);
}

void noti_graph_xax_drop(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	int nHint;
	link_t_ptr root, xlk, ylk, glk, xlk_first, xlk_last;
	xpoint_t pt;
	canvbox_t cb;

	XDL_ASSERT(ptd->xax);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	ptd->b_drag_xax = (bool_t)0;

	widget_get_canv_rect(widget, &cb);

	pt.x = x;
	pt.y = y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_graph_hint(&cb, &pt, ptd->graph, ptd->cur_page, &xlk, &ylk, &glk);
	if (xlk != ptd->xax)
	{
		root = get_dom_child_node_root(get_graph_xaxset(ptd->graph));

		if (xlk)
		{
			if (x < ptd->org_x)
				switch_link_before(root, xlk, ptd->xax);
			else
				switch_link_after(root, xlk, ptd->xax);
		}
		else
		{
			calc_graph_xax_scope(&cb, ptd->graph, ptd->cur_page, &xlk_first, &xlk_last);

			if (x < ptd->org_x)
				switch_link_before(root, xlk_first, ptd->xax);
			else
				switch_link_after(root, xlk_last, ptd->xax);
		}
	}

	widget_update(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_graph_owner(widget, NC_XAXDROP, ptd->graph, ptd->xax, NULL, NULL, (void*)&pt);
}

void noti_graph_yax_selected(res_win_t widget, link_t_ptr ylk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ylk);

	if (!graph_is_design(ptd->graph))
		return;

	set_yax_selected(ylk, ((get_yax_selected(ylk)) ? 0 : 1));

	noti_graph_owner(widget, NC_YAXSELECTED, ptd->graph, NULL, ylk, NULL, NULL);

	_graphctrl_yax_rect(widget, ylk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t noti_graph_xax_insert(res_win_t widget, link_t_ptr xlk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(xlk);

	if (noti_graph_owner(widget, NC_XAXINSERT, ptd->graph, xlk, NULL, NULL, NULL))
		return 0;

	return 1;
}

bool_t noti_graph_xax_delete(res_win_t widget, link_t_ptr xlk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(xlk);

	if (noti_graph_owner(widget, NC_XAXDELETE, ptd->graph, xlk, NULL, NULL, NULL))
		return 0;

	return 1;
}

bool_t noti_graph_xax_changing(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->xax);

	if (noti_graph_owner(widget, NC_XAXCHANGING, ptd->graph, ptd->xax, NULL, NULL, NULL))
		return (bool_t)0;

	_graphctrl_xax_rect(widget, ptd->xax, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->xax = NULL;

	widget_update(widget, &xr, 0);

	return (bool_t)1;
}

void noti_graph_xax_changed(res_win_t widget, link_t_ptr xlk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(xlk && !ptd->xax);

	_graphctrl_xax_rect(widget, xlk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->xax = xlk;

	widget_update(widget, &xr, 0);

	noti_graph_owner(widget, NC_XAXCHANGED, ptd->graph, ptd->xax, NULL, NULL, NULL);
}

void noti_graph_xax_checked(res_win_t widget, link_t_ptr xlk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(xlk);

	set_xax_checked(xlk, ((get_xax_checked(xlk)) ? 0 : 1));

	noti_graph_owner(widget, NC_XAXCHECKED, ptd->graph, xlk, NULL, NULL, NULL);

	_graphctrl_xax_rect(widget, xlk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

void noti_graph_gax_changing(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->gax);

	noti_graph_owner(widget, NC_GAXCHANGING, ptd->graph, NULL, NULL, ptd->gax, NULL);

	_graphctrl_gaxbar_rect(widget, ptd->gax, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->gax = NULL;

	widget_update(widget, &xr, 0);
}

void noti_graph_gax_changed(res_win_t widget, link_t_ptr glk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(glk && !ptd->gax);

	_graphctrl_gaxbar_rect(widget, glk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->gax = glk;

	widget_update(widget, &xr, 0);

	noti_graph_owner(widget, NC_GAXCHANGED, ptd->graph, NULL, NULL, ptd->gax, NULL);
}

void noti_graph_yax_changing(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->yax);

	noti_graph_owner(widget, NC_YAXCHANGING, ptd->graph, NULL, ptd->yax, NULL, NULL);

	_graphctrl_yax_rect(widget, ptd->yax, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->yax = NULL;

	widget_update(widget, &xr, 0);
}

void noti_graph_yax_changed(res_win_t widget, link_t_ptr ylk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ylk && !ptd->yax);

	_graphctrl_yax_rect(widget, ylk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->yax = ylk;

	widget_update(widget, &xr, 0);

	noti_graph_owner(widget, NC_YAXCHANGED, ptd->graph, NULL, ptd->yax, NULL, NULL);
}

void noti_graph_yax_enter(res_win_t widget, link_t_ptr yax)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(yax);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = yax;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
}

void noti_graph_yax_leave(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
}

void noti_graph_yax_hover(res_win_t widget, long x, long y)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_graph_owner(widget, NC_YAXHOVER, ptd->graph, NULL, ptd->hover, NULL, (void*)&xp);
}

void noti_graph_begin_edit(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	const tchar_t* text;
	xrect_t xr = { 0 };

	clr_mod_t ob = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd->xax);
	
	if (widget_is_valid(ptd->editor))
		return;

	if (ptd->b_lock)
		return;

	if (get_xax_locked(ptd->xax))
		return;

	widget_get_xfont(widget, &xf);
	parse_xfont_from_style(&xf, get_graph_style_ptr(ptd->graph));
	widget_get_color_mode(widget, &ob);

	_graphctrl_coor_rect(widget, ptd->xax, ptd->yax, &xr);
	pt_expand_rect(&xr, -1, -1);

	if (noti_graph_owner(widget, NC_COOREDITING, ptd->graph, ptd->xax, ptd->yax, NULL, NULL))
		return;

	if (ptd->yax)
	{
		ptd->editor = firenum_create(widget, &xr);
		widget_set_user_id(ptd->editor, IDC_FIRENUM);
	}
	else
	{
		ptd->editor = fireedit_create(widget, &xr);
		widget_set_user_id(ptd->editor, IDC_FIREEDIT);
	}
	XDL_ASSERT(ptd->editor);
	
	widget_set_owner(ptd->editor, widget);

	widget_set_xfont(ptd->editor, &xf);
	widget_set_color_mode(ptd->editor, &ob);
	widget_show(ptd->editor, WD_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	if (ptd->pch[0])
	{
		editbox_set_text(ptd->editor, ptd->pch);
		ptd->pch[0] = _T('\0');
	}
	else
	{
		if (ptd->yax)
			text = get_coor_text_ptr(ptd->xax, ptd->yax);
		else
			text = get_xax_text_ptr(ptd->xax);

		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
}

void noti_graph_commit_edit(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	const tchar_t* text;
	res_win_t editctrl;
	link_t_ptr xlk_new;
	bool_t b_accept = 0;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->xax);

	text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
	b_accept = (noti_graph_owner(widget, NC_COORCOMMIT, ptd->graph, ptd->xax, ptd->yax, NULL, (void*)text) == 0) ? 1 : 0;
	if (b_accept)
	{
		graphctrl_set_coor_text(widget, ptd->xax, ptd->yax, text);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);

	if (!b_accept)
		return;

	if (ptd->b_auto && (ptd->xax == get_prev_xax(ptd->graph, LINK_LAST)) && (ptd->yax == get_prev_yax(ptd->graph, LINK_LAST)))
	{
		xlk_new = insert_xax(ptd->graph, LINK_LAST);
		set_xax_state(xlk_new, dsNewClean);

		if (!noti_graph_xax_insert(widget, xlk_new))
		{
			delete_xax(xlk_new);
			return;
		}

		graphctrl_set_focus_coor(widget, xlk_new, get_next_yax(ptd->graph, LINK_FIRST));
	}
	else
	{
		if (ptd->yax == get_prev_yax(ptd->graph, LINK_LAST))
		{
			xlk_new = get_next_xax(ptd->graph, ptd->xax);
			if (xlk_new)
			{
				graphctrl_set_focus_coor(widget, xlk_new, get_next_xax(ptd->graph, LINK_FIRST));
			}
		}
		else
		{
			graphctrl_tabskip(widget,WD_TAB_DOWN);
		}
	}
}

void noti_graph_rollback_edit(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->xax);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_graph_reset_editor(res_win_t widget, bool_t bCommit)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_graph_commit_edit(widget);
		else
			noti_graph_rollback_edit(widget);
	}
}

/*******************************************************************************************/

int hand_graph_create(res_win_t widget, void* data)
{
	graph_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (graph_delta_t*)xmem_alloc(sizeof(graph_delta_t));

	SETGRAPHDELTA(widget, ptd);

	ptd->b_lock = 1;
	ptd->stack = create_stack_table();

	return 0;
}

void hand_graph_destroy(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_graph_reset_editor(widget, 0);

	_graphctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETGRAPHDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_graph_size(res_win_t widget, int code, const xsize_t* psc)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	graphctrl_redraw(widget, 1);
}

void hand_graph_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_graph_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr xax, yax, gax;
	int nHint;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->graph)
		return;

	if (ptd->b_size_xax || ptd->b_size_yax)
	{
		ptd->cur_x = pxp->x;
		ptd->cur_y = pxp->y;
		return;
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_graph_hint(&cb, &pt,ptd->graph, ptd->cur_page,  &xax, &yax, &gax);

	if (nHint == GRAPH_HINT_HORZ_SPLIT && yax == ptd->yax && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_graph_yax_sizing(widget, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget,CURSOR_SIZENS);
	}
	else if (nHint == GRAPH_HINT_VERT_SPLIT && xax == ptd->xax && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_graph_xax_sizing(widget, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget,CURSOR_SIZEWE);
	}
	else if (nHint == GRAPH_HINT_YAXBAR && yax == ptd->yax && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_graph_yax_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == GRAPH_HINT_XAXBAR && xax == ptd->xax && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_graph_xax_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == GRAPH_HINT_NONE)
	{
		widget_set_cursor(widget, CURSOR_ARROW);
	}
}

void hand_graph_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	if (ptd->hover)
		noti_graph_yax_hover(widget, pxp->x, pxp->y);
}

void hand_graph_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	if (ptd->hover)
		noti_graph_yax_leave(widget);
}

void hand_graph_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	if (ptd->yax && !ptd->xax)
	{
		if (get_yax_sortable(ptd->yax))
		{
			//sort_graph_yax(ptd->graph,ptd->yax);
			//widget_update(widget, 0, 0, 0, 0);
		}
	}

	noti_graph_owner(widget, NC_GRAPHDBCLK, ptd->graph, ptd->xax, ptd->yax, NULL, (void*)pxp);
}

void hand_graph_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr xlk, ylk, glk;
	int nHint;
	bool_t bReYax, bReXax;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_graph_hint(&cb, &pt,ptd->graph, ptd->cur_page,  &xlk, &ylk, &glk);

	bReXax = (xlk == ptd->xax) ? (bool_t)1 : (bool_t)0;
	bReYax = (ylk == ptd->yax) ? (bool_t)1 : (bool_t)0;

	if (nHint == GRAPH_HINT_NULBAR)
	{
		noti_graph_reset_check(widget);
	}
	else if (nHint == GRAPH_HINT_XAXBAR)
	{
		noti_graph_xax_checked(widget, xlk);
	}
	else if (nHint == GRAPH_HINT_YAXBAR)
	{
		if (widget_key_state(widget, KEY_CONTROL))
			noti_graph_yax_selected(widget, ylk);
	}
	else if (nHint == GRAPH_HINT_NONE)
	{
		if (!widget_key_state(widget, KEY_CONTROL))
		{
			if (graph_is_design(ptd->graph))
				noti_graph_reset_select(widget);
		}
	}
}

void hand_graph_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr xlk, ylk, glk;
	int nHint;
	bool_t bReGax, bReYax, bReXax;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->graph)
		return;

	if (ptd->b_size_xax && ptd->xax)
	{
		noti_graph_xax_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_size_yax && ptd->yax)
	{
		noti_graph_yax_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag_xax && ptd->xax)
	{
		noti_graph_xax_drop(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag_yax && ptd->yax)
	{
		noti_graph_yax_drop(widget, pxp->x, pxp->y);
		return;
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_graph_hint(&cb, &pt, ptd->graph, ptd->cur_page, &xlk, &ylk, &glk);

	bReXax = (xlk == ptd->xax) ? 1 : 0;
	bReYax = (ylk == ptd->yax) ? 1 : 0;
	bReGax = (glk == ptd->gax) ? 1 : 0;

	if (bReXax && bReYax && bReGax)
	{
		if (ptd->xax && !ptd->b_lock && !get_xax_locked(ptd->xax))
		{
			widget_post_key(widget, KEY_ENTER);
		}
		return;
	}

	if (!bReXax && ptd->xax)
	{
		if (!noti_graph_xax_changing(widget))
			bReXax = 1;
	}

	if (!bReGax && ptd->gax)
	{
		noti_graph_gax_changing(widget);
	}

	if (!bReYax && ptd->yax)
	{
		noti_graph_yax_changing(widget);
	}

	if (!bReYax && ylk)
		noti_graph_yax_changed(widget, ylk);

	if (!bReGax && glk)
		noti_graph_gax_changed(widget, glk);

	if (!bReXax && xlk)
		noti_graph_xax_changed(widget, xlk);

	if (!bReYax || !bReXax)
	{
		if (ptd->xax && ptd->yax)
		{
			_graphctrl_ensure_visible(widget);
			noti_graph_owner(widget, NC_COORSETFOCUS, ptd->graph, ptd->xax, ptd->yax, NULL, NULL);
		}
	}

	noti_graph_owner(widget, NC_GRAPHLBCLK, ptd->graph, ptd->xax, ptd->yax, ptd->gax, (void*)pxp);
}

void hand_graph_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);
}

void hand_graph_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	noti_graph_owner(widget, NC_GRAPHRBCLK, ptd->graph, ptd->xax, ptd->yax, ptd->gax, (void*)pxp);
}

void hand_graph_keydown(res_win_t widget, int nKey)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	if (graph_is_design(ptd->graph))
	{
		if ((nKey == _T('z') || nKey == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_undo(widget);
			return;
		}
		else if ((nKey == _T('c') || nKey == _T('C')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_copy(widget);
			return;
		}
		else if ((nKey == _T('x') || nKey == _T('X')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_cut(widget);
			return;
		}
		else if ((nKey == _T('v') || nKey == _T('V')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_paste(widget);
			return;
		}
	}

	switch (nKey)
	{
	case KEY_ENTER:
		if (ptd->xax)
		{
			noti_graph_begin_edit(widget);
		}
		break;
	case KEY_LEFT:
		graphctrl_tabskip(widget, WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		graphctrl_tabskip(widget, WD_TAB_RIGHT);
		break;
	case KEY_UP:
		graphctrl_tabskip(widget, WD_TAB_UP);
		break;
	case KEY_DOWN:
		graphctrl_tabskip(widget, WD_TAB_DOWN);
		break;
	case KEY_END:
		graphctrl_tabskip(widget, WD_TAB_END);
		break;
	case KEY_HOME:
		graphctrl_tabskip(widget, WD_TAB_HOME);
		break;
	case KEY_PAGEUP:
		graphctrl_tabskip(widget, WD_TAB_PAGEUP);
		break;
	case KEY_PAGEDOWN:
		graphctrl_tabskip(widget, WD_TAB_PAGEDOWN);
		break;
	}
}

void hand_graph_char(res_win_t widget, tchar_t nChar)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	if (IS_VISIBLE_CHAR(nChar) && !widget_is_valid(ptd->editor))
	{
		ptd->pch[0] = nChar;
		hand_graph_keydown(widget, KEY_ENTER);
	}

	if (IS_VISIBLE_CHAR(nChar) && widget_is_valid(ptd->editor))
	{
		//widget_post_char(NULL, nChar);
	}
}

void hand_graph_copy(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	_graphctrl_copy(widget);
}

void hand_graph_cut(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	_graphctrl_done(widget);

	if (!_graphctrl_cut(widget))
	{
		_graphctrl_discard(widget);
	}
}

void hand_graph_paste(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	_graphctrl_done(widget);

	if (!_graphctrl_paste(widget))
	{
		_graphctrl_discard(widget);
	}
}

void hand_graph_undo(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;

	_graphctrl_undo(widget);
}

void hand_graph_child_command(res_win_t widget, int code, var_long data)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_graph_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_graph_rollback_edit(widget);
		break;
	}
}

void hand_graph_erase(res_win_t widget, res_ctx_t dc)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	if (!ptd->graph)
		return;
}

void hand_graph_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->graph)
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

	draw_graph_page(pif, &cb, ptd->graph, ptd->cur_page);

	//draw focus
	if (ptd->xax && ptd->yax)
	{
		_graphctrl_coor_rect(widget, ptd->xax, ptd->yax, &xr);

		parse_xcolor(&xc, DEF_FOCUS_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}
	else if (ptd->yax)
	{
		_graphctrl_yaxbar_rect(widget, ptd->yax, &xr);

		parse_xcolor(&xc, DEF_DISABLE_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}
	else if (ptd->xax)
	{
		_graphctrl_xaxbar_rect(widget, ptd->xax, &xr);

		parse_xcolor(&xc, DEF_FOCUS_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}
	else if (ptd->gax)
	{
		_graphctrl_gaxbar_rect(widget, ptd->gax, &xr);

		parse_xcolor(&xc, DEF_DISABLE_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/*********************************************************************************/

res_win_t graphctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_graph_create)
		EVENT_ON_DESTROY(hand_graph_destroy)

		EVENT_ON_ERASE(hand_graph_erase)
		EVENT_ON_PAINT(hand_graph_paint)

		EVENT_ON_SIZE(hand_graph_size)

		EVENT_ON_SCROLL(hand_graph_scroll)

		EVENT_ON_KEYDOWN(hand_graph_keydown)
		EVENT_ON_CHAR(hand_graph_char)

		EVENT_ON_MOUSE_MOVE(hand_graph_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_graph_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_graph_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_graph_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_graph_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_graph_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_graph_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_graph_rbutton_up)

		EVENT_ON_CHILD_COMMAND(hand_graph_child_command)
		EVENT_ON_COPY(hand_graph_copy)
		EVENT_ON_CUT(hand_graph_cut)
		EVENT_ON_PASTE(hand_graph_paste)
		EVENT_ON_UNDO(hand_graph_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void graphctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_graph_doc(ptr));

	noti_graph_reset_editor(widget, 0);

	ptd->graph = ptr;
	ptd->yax = NULL;
	ptd->xax = NULL;

	ptd->cur_page = 1;

	graphctrl_redraw(widget, 1);

	if (!ptd->b_lock && ptd->b_auto && !get_next_xax(ptr, LINK_FIRST))
	{
		graphctrl_insert_xax(widget, LINK_LAST);
	}
}

link_t_ptr graphctrl_detach(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	noti_graph_reset_editor(widget, 0);

	data = ptd->graph;
	ptd->graph = NULL;

	ptd->cur_page = 0;

	widget_update(widget, NULL, 0);

	return data;
}

link_t_ptr graphctrl_fetch(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->graph;
}

void graphctrl_accept(res_win_t widget, bool_t bAccept)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr xlk, nxt;
	int ds;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, bAccept);

	xlk = get_next_xax(ptd->graph, LINK_FIRST);
	while (xlk)
	{
		nxt = get_next_xax(ptd->graph, xlk);

		ds = get_xax_state(xlk);
		if (ds == dsNewClean || ds == dsNewDelete)
			delete_xax(xlk);

		xlk = nxt;
	}

	graphctrl_redraw(widget, 0);
}

bool_t graphctrl_is_update(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return 0;

	return (get_update_xax_count(ptd->graph)) ? 1 : 0;
}

void graphctrl_auto_insert(res_win_t widget, bool_t bAuto)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_auto = bAuto;
}

void graphctrl_redraw(res_win_t widget, bool_t bCalc)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr xlk, ylk, glk;
	bool_t b_valid;
	bool_t b;
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 0);

	ptd->hover = NULL;

	b_valid = 0;
	glk = get_next_gax(ptd->graph, LINK_FIRST);
	while (glk)
	{
		if (glk == ptd->gax)
			b_valid = 1;

		glk = get_next_gax(ptd->graph, glk);
	}

	if (!b_valid)
		ptd->gax = NULL;

	b_valid = 0;
	ylk = get_next_yax(ptd->graph, LINK_FIRST);
	while (ylk)
	{
		if (ylk == ptd->yax)
			b_valid = 1;

		noti_graph_owner(widget, NC_YAXCALCED, ptd->graph, NULL, ylk, NULL, NULL);

		ylk = get_next_yax(ptd->graph, ylk);
	}

	if (!b_valid)
		ptd->yax = NULL;

	b_valid = 0;
	xlk = get_next_xax(ptd->graph, LINK_FIRST);
	while (xlk)
	{
		if (xlk == ptd->xax)
			b_valid = 1;

		noti_graph_owner(widget, NC_XAXCALCED, ptd->graph, xlk, NULL, NULL, NULL);

		xlk = get_next_xax(ptd->graph, xlk);
	}

	if (!b_valid)
		ptd->xax = NULL;

	noti_graph_owner(widget, NC_GRAPHCALCED, ptd->graph, NULL, NULL, NULL, NULL);

	b = (widget_get_style(widget) & WD_STYLE_PAGING) ? 1 : 0;
	if (!b)
	{
		widget_get_client_rect(widget, &xr);
		widget_rect_to_tm(widget, &xr);
		set_graph_height(ptd->graph, xr.fh);
	}

	_graphctrl_reset_page(widget);

	widget_update_window(widget);

	widget_update(widget, NULL, 0);
}

void graphctrl_redraw_xax(res_win_t widget, link_t_ptr xlk, bool_t bCalc)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 0);

#ifdef _DEBUG
	XDL_ASSERT(is_graph_xax(ptd->graph, xlk));
#endif

	noti_graph_owner(widget, NC_XAXCALCED, ptd->graph, xlk, NULL, NULL, NULL);

	_graphctrl_xax_rect(widget, xlk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

void graphctrl_redraw_yax(res_win_t widget, link_t_ptr ylk, bool_t bCalc)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 0);

#ifdef _DEBUG
	XDL_ASSERT(is_graph_yax(ptd->graph, ylk));
#endif

	noti_graph_owner(widget, NC_YAXCALCED, ptd->graph, ylk, NULL, NULL, NULL);

	_graphctrl_yax_rect(widget, ylk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

void graphctrl_tabskip(res_win_t widget, int dir)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr xlk, ylk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	xlk = ptd->xax;
	ylk = ptd->yax;

	switch (dir)
	{
	case WD_TAB_UP:
		if (ylk == NULL)
			ylk = get_prev_yax(ptd->graph, LINK_LAST);
		else
			ylk = get_prev_yax(ptd->graph, ylk);

		if (ylk)
			graphctrl_set_focus_coor(widget, xlk, ylk);
		break;
	case WD_TAB_DOWN:
		if (ylk == NULL)
			ylk = get_next_yax(ptd->graph, LINK_FIRST);
		else
			ylk = get_next_yax(ptd->graph, ylk);

		if (ylk)
			graphctrl_set_focus_coor(widget, xlk, ylk);
		break;
	case WD_TAB_LEFT:
		if (xlk != NULL)
		{
			xlk = get_prev_xax(ptd->graph, xlk);
			if (xlk)
				graphctrl_set_focus_coor(widget, xlk, ylk);
		}
		break;
	case WD_TAB_RIGHT:
		if (xlk != NULL)
		{
			xlk = get_next_xax(ptd->graph, xlk);
			if (xlk)
				graphctrl_set_focus_coor(widget, xlk, ylk);
		}
		break;
	case WD_TAB_HOME:
		graphctrl_move_first_page(widget);
		break;
	case WD_TAB_END:
		graphctrl_move_last_page(widget);
		break;
	case WD_TAB_PAGEUP:
		graphctrl_move_prev_page(widget);
		break;
	case WD_TAB_PAGEDOWN:
		graphctrl_move_next_page(widget);
		break;
	}
}

bool_t graphctrl_delete_xax(res_win_t widget, link_t_ptr xlk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr nlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return 0;

	noti_graph_reset_editor(widget, 0);

	if (ptd->b_lock)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_graph_xax(ptd->graph, xlk));
#endif

	if (!noti_graph_xax_delete(widget, xlk))
		return 0;

	if (xlk == ptd->xax)
	{
		noti_graph_xax_changing(widget);
	}

	nlk = get_next_xax(ptd->graph, xlk);
	if (!nlk)
		nlk = get_prev_xax(ptd->graph, xlk);

	delete_xax(xlk);

	graphctrl_redraw(widget, 1);

	if (nlk)
		graphctrl_set_focus_coor(widget, nlk, ptd->yax);

	return 1;
}

link_t_ptr graphctrl_insert_xax(res_win_t widget, link_t_ptr pre)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	link_t_ptr xlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return NULL;

	noti_graph_reset_editor(widget, 0);

	if (ptd->b_lock)
		return NULL;

	if (pre == LINK_FIRST)
		pre = get_next_xax(ptd->graph, LINK_FIRST);
	else if (pre == LINK_LAST)
		pre = get_prev_xax(ptd->graph, LINK_LAST);
	else
	{
#ifdef _DEBUG
		XDL_ASSERT(is_graph_xax(ptd->graph, pre));
#endif
	}

	xlk = insert_xax(ptd->graph, pre);
	set_xax_state(xlk, dsNewClean);

	if (!noti_graph_xax_insert(widget, xlk))
	{
		delete_xax(xlk);
		return NULL;
	}

	graphctrl_redraw(widget, 1);

	graphctrl_set_focus_coor(widget, xlk, get_next_yax(ptd->graph, LINK_FIRST));

	return xlk;
}

bool_t graphctrl_set_coor_text(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, const tchar_t* szText)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	const tchar_t* text;
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_graph_xax(ptd->graph, xlk));
#endif

	if (ylk)
	{
		text = get_coor_text_ptr(xlk, ylk);
		if (compare_numeric(szText, text, get_yax_data_dig(ylk)) != 0)
		{
			set_coor_text(xlk, ylk, szText, -1);
			set_coor_dirty(xlk, ylk, 1);
			set_xax_dirty(xlk);

			noti_graph_owner(widget, NC_COORUPDATE, ptd->graph, xlk, ylk, NULL, NULL);

			graphctrl_get_coor_rect(widget, xlk, ylk, &xr);
			pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
			widget_update(widget, &xr, 1);

			return 1;
		}
	}
	else
	{
		text = get_xax_text_ptr(xlk);
		if (compare_text(szText, -1, text, -1, 0) != 0)
		{
			set_xax_text(xlk, szText);
			set_xax_dirty(xlk);

			noti_graph_owner(widget, NC_COORUPDATE, ptd->graph, xlk, ylk, NULL, NULL);

			_graphctrl_xaxbar_rect(widget, xlk, &xr);
			pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
			widget_update(widget, &xr, 1);

			return 1;
		}
	}

	return 0;
}

bool_t graphctrl_set_focus_coor(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	bool_t bReXax, bReYax;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return 0;

	if (xlk == LINK_FIRST)
		xlk = get_next_xax(ptd->graph, LINK_FIRST);
	else if (xlk == LINK_LAST)
		xlk = get_prev_xax(ptd->graph, LINK_LAST);

	if (ylk == LINK_FIRST)
		ylk = get_next_yax(ptd->graph, LINK_FIRST);
	else if (ylk == LINK_LAST)
		ylk = get_prev_yax(ptd->graph, LINK_LAST);

	bReXax = (xlk == ptd->xax) ? 1 : 0;
	bReYax = (ylk == ptd->yax) ? 1 : 0;

	if (bReXax && bReYax)
	{
		return 1;
	}

	if (!bReXax && ptd->xax)
	{
		if (!noti_graph_xax_changing(widget))
			return 0;
	}

	if (!bReYax && ptd->yax)
	{
		noti_graph_yax_changing(widget);
	}

	if (!bReXax || !bReYax)
	{
		if (ptd->xax && ptd->yax)
			noti_graph_owner(widget, NC_COORKILLFOCUS, ptd->graph, ptd->xax, ptd->yax, NULL, NULL);
	}

	if (!bReYax && ylk)
		noti_graph_yax_changed(widget, ylk);

	if (!bReXax && xlk)
		noti_graph_xax_changed(widget, xlk);

	if (!bReXax || !bReYax)
	{
		if (ptd->xax && ptd->yax)
			noti_graph_owner(widget, NC_COORSETFOCUS, ptd->graph, ptd->xax, ptd->yax, NULL, NULL);
	}

	_graphctrl_ensure_visible(widget);

	return 1;
}

link_t_ptr graphctrl_get_focus_xax(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return NULL;

	return ptd->xax;
}

link_t_ptr graphctrl_get_focus_yax(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return NULL;

	return ptd->yax;
}

link_t_ptr graphctrl_get_focus_gax(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return NULL;

	return ptd->gax;
}

void graphctrl_move_first_page(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	nCurPage = ptd->cur_page;

	if (nCurPage != 1)
	{
		nCurPage = 1;
		ptd->cur_page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

void graphctrl_move_prev_page(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	nCurPage = ptd->cur_page;

	if (nCurPage > 1)
	{
		nCurPage--;
		ptd->cur_page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

void graphctrl_move_next_page(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_graph_pages(&cb, ptd->graph);

	if (nCurPage < nMaxPage)
	{
		nCurPage++;
		ptd->cur_page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

void graphctrl_move_last_page(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_graph_pages(&cb, ptd->graph);

	if (nCurPage != nMaxPage)
	{
		nCurPage = nMaxPage;
		ptd->cur_page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

void graphctrl_move_to_page(res_win_t widget, int page)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	noti_graph_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_graph_pages(&cb, ptd->graph);

	if (page > 0 && page != nCurPage && page <= nMaxPage)
	{
		nCurPage = page;
		ptd->cur_page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

int graphctrl_get_cur_page(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return 0;

	return ptd->cur_page;
}

int graphctrl_get_max_page(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return 0;

	widget_get_canv_rect(widget, &cb);

	return calc_graph_pages(&cb, ptd->graph);
}

void graphctrl_get_coor_rect(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	_graphctrl_coor_rect(widget, xlk, ylk, pxr);
}

bool_t graphctrl_get_lock(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

void graphctrl_set_lock(res_win_t widget, bool_t bLock)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t graphctrl_get_dirty(res_win_t widget)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return 0;

	if (!graph_is_design(ptd->graph))
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void graphctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	graph_delta_t* ptd = GETGRAPHDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->graph)
		return;

	if (!graph_is_design(ptd->graph))
		return;

	if (bDirty)
		_graphctrl_done(widget);
	else
		_graphctrl_clean(widget);
}

