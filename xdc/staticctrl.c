/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc statis control document

	@module	statisctrl.c | implement file

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
#include "xdcfire.h"
#include "xdcbox.h"

typedef struct _statis_delta_t{
	link_t_ptr statis;
	link_t_ptr gax;
	link_t_ptr yax;
	link_t_ptr xax;
	link_t_ptr hover;

	int org_x, org_y;
	int cur_x, cur_y;
	int cur_page;

	res_win_t editor;
	res_win_t hsc;
	res_win_t vsc;

	bool_t b_drag_xax, b_drag_yax;
	bool_t b_size_xax, b_size_yax;

	bool_t b_auto;
	bool_t b_lock;

	link_t_ptr stack;
}statis_delta_t;

#define GETSTATISDELTA(ph) 	(statis_delta_t*)widget_get_user_delta(ph)
#define SETSTATISDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/******************************************************************************************/
static void _statisctrl_done(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->statis);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->statis, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->statis, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->statis, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->statis, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _statisctrl_undo(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->statis);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_statis_doc(ptd->statis);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->statis, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->statis, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		statisctrl_redraw(widget, 1);
	}
}

static void _statisctrl_discard(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _statisctrl_clean(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _statisctrl_copy(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->statis);

	if (!statis_is_design(ptd->statis))
		return 0;

	if (!get_yax_selected_count(ptd->statis) && !ptd->yax)
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_statis_doc();
	ilk = get_next_yax(ptd->statis, LINK_FIRST);
	while (ilk)
	{
		if (get_yax_selected(ilk) || ilk == ptd->yax)
		{
			nlk = insert_yax(dom, LINK_LAST);
			copy_dom_node(nlk, ilk);
		}

		ilk = get_next_yax(ptd->statis, ilk);
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

		destroy_statis_doc(dom);
		return 0;
	}

	clipboard_close();

	destroy_statis_doc(dom);
	return 1;
}

static bool_t _statisctrl_cut(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->statis);

	if (!_statisctrl_copy(widget))
		return 0;

	ilk = get_next_yax(ptd->statis, LINK_FIRST);
	while (ilk)
	{
		nxt = get_next_yax(ptd->statis, ilk);

		if (get_yax_selected(ilk) || ilk == ptd->yax)
		{
			delete_yax(ilk);
		}
		ilk = nxt;
	}

	statisctrl_redraw(widget, 1);

	return 1;
}

static bool_t _statisctrl_paste(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, plk;

	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->statis);

	if (!statis_is_design(ptd->statis))
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

	if (!is_statis_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	plk = (ptd->yax) ? ptd->yax : LINK_LAST;

	while (nlk = get_next_yax(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_statis_yaxset(dom), nlk);
		attach_dom_node(get_statis_yaxset(ptd->statis), plk, nlk);

		xsprintf(sz_name, _T("%s%d"), get_yax_count(ptd->statis));
		set_yax_name(nlk, sz_name);
		set_yax_title(nlk, sz_name);

	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);

	statisctrl_redraw(widget, 1);

	return 1;
}

static void _statisctrl_xaxbar_rect(res_win_t widget, link_t_ptr xlk, xrect_t* pxr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_statis_coor_rect(&cb, ptd->statis, ptd->cur_page, xlk, NULL, pxr);
	
	widget_rect_to_pt(widget, pxr);
}

static void _statisctrl_xax_rect(res_win_t widget, link_t_ptr xlk, xrect_t* pxr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_statis_xax_rect(&cb, ptd->statis, ptd->cur_page, xlk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _statisctrl_gaxbar_rect(res_win_t widget, link_t_ptr glk, xrect_t* pxr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_statis_gax_rect(&cb, ptd->statis, glk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _statisctrl_yax_rect(res_win_t widget, link_t_ptr ylk, xrect_t* pxr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_statis_yax_rect(&cb, ptd->statis, ylk, pxr);

	pxr->fw = get_statis_width(ptd->statis);

	widget_rect_to_pt(widget, pxr);
}

static void _statisctrl_yaxbar_rect(res_win_t widget, link_t_ptr ylk, xrect_t* pxr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_statis_yax_rect(&cb, ptd->statis, ylk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _statisctrl_coor_rect(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_statis_coor_rect(&cb, ptd->statis, ptd->cur_page, xlk, ylk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _statisctrl_reset_page(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	int pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_statis_width(ptd->statis);
	xs.fy = get_statis_height(ptd->statis);

	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = get_statis_yaxbar_height(ptd->statis);
	xs.fy = get_statis_xaxbar_width(ptd->statis);

	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

void _statisctrl_ensure_visible(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;
	canvbox_t cb;

	int page;

	if (!ptd->xax || !ptd->yax)
		return;

	widget_get_canv_rect(widget, &cb);

	page = calc_statis_xax_page(&cb, ptd->statis, ptd->xax);
	if (page != ptd->cur_page)
	{
		ptd->cur_page = page;
		widget_erase(widget, NULL);
	}

	_statisctrl_coor_rect(widget, ptd->xax, ptd->yax, &xr);

	widget_ensure_visible(widget, &xr, 1);
}

/***********************************************************************************************************************/
int noti_statis_owner(res_win_t widget, unsigned int code, link_t_ptr statis, link_t_ptr xlk, link_t_ptr ylk, link_t_ptr glk, void* data)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	NOTICE_STATIS nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;
	nf.ret = 0;

	nf.statis = statis;
	nf.xax = xlk;
	nf.yax = ylk;
	nf.gax = glk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_statis_reset_select(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr ylk;
	int count = 0;

	ylk = get_next_yax(ptd->statis, LINK_FIRST);
	while (ylk)
	{
		if (get_yax_selected(ylk))
		{
			set_yax_selected(ylk, 0);
			noti_statis_owner(widget, NC_YAXSELECTED, ptd->statis, NULL, ylk, NULL, NULL);

			count++;
		}
		ylk = get_next_yax(ptd->statis, ylk);
	}

	if (count)
		widget_erase(widget, NULL);
}

void noti_statis_reset_check(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr xlk;
	int count = 0;

	if (!get_statis_showcheck(ptd->statis))
		return;

	xlk = get_next_xax(ptd->statis, LINK_FIRST);
	while (xlk)
	{
		if (get_xax_checked(xlk))
		{
			set_xax_checked(xlk, 0);
			noti_statis_owner(widget, NC_XAXCHECKED, ptd->statis, xlk, NULL, NULL, NULL);

			count++;
		}
		xlk = get_next_xax(ptd->statis, xlk);
	}

	if (count)
		widget_erase(widget, NULL);
}

void noti_statis_xax_sizing(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	ptd->b_size_xax = (bool_t)1;
	ptd->org_x = x;
	ptd->org_y = y;

	widget_set_capture(widget, 1);
	widget_set_cursor(widget,CURSOR_SIZEWE);
}

void noti_statis_xax_sized(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	float mw;
	xsize_t xs = { 0 };

	widget_set_capture(widget, 0);
	widget_set_cursor(widget, CURSOR_ARROW);

	ptd->b_size_xax = (bool_t)0;

	xs.cx = x - ptd->org_x;
	if (!xs.cx)
		return;

	widget_size_to_tm(widget, &xs);

	mw = get_statis_xaxbar_width(ptd->statis);
	mw += xs.fx;
	if (mw < 2 * DEF_SPLIT_FEED)
		mw = 2 * DEF_SPLIT_FEED;

	_statisctrl_done(widget);

	mw = (float)(int)mw;
	set_statis_xaxbar_width(ptd->statis, mw);

	widget_erase(widget, NULL);

	noti_statis_owner(widget, NC_XAXSIZED, ptd->statis, ptd->xax, NULL, NULL, NULL);
}

void noti_statis_yax_sizing(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	ptd->b_size_yax = (bool_t)1;
	ptd->org_y = x;
	ptd->org_y = y;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_SIZENS);
}

void noti_statis_yax_sized(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
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

	mh = get_statis_yaxbar_height(ptd->statis);
	mh += xs.fy;
	if (mh <  2 * DEF_SPLIT_FEED)
		mh = 2 * DEF_SPLIT_FEED;

	_statisctrl_done(widget);

	mh = (float)(int)mh;
	set_statis_yaxbar_height(ptd->statis, mh);

	widget_erase(widget, NULL);

	noti_statis_owner(widget, NC_YAXSIZED, ptd->statis, NULL, ptd->yax, NULL, NULL);
}

void noti_statis_yax_drag(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
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
	noti_statis_owner(widget, NC_YAXDRAG, ptd->statis, NULL, ptd->yax, NULL, (void*)&pt);
}

void noti_statis_yax_drop(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
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

	nHint = calc_statis_hint(&cb, &pt, ptd->statis, ptd->cur_page, &xlk, &ylk, &glk);
	if (ylk != ptd->yax)
	{
		root = get_dom_child_node_root(get_statis_yaxset(ptd->statis));

		_statisctrl_done(widget);

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

	widget_erase(widget, NULL);

	pt.x = x;
	pt.y = y;
	noti_statis_owner(widget, NC_YAXDROP, ptd->statis, NULL, ptd->yax, NULL, (void*)&pt);
}

void noti_statis_xax_drag(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
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
	noti_statis_owner(widget, NC_XAXDRAG, ptd->statis, ptd->xax, NULL, NULL, (void*)&pt);
}

void noti_statis_xax_drop(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
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

	nHint = calc_statis_hint(&cb, &pt, ptd->statis, ptd->cur_page, &xlk, &ylk, &glk);
	if (xlk != ptd->xax)
	{
		root = get_dom_child_node_root(get_statis_xaxset(ptd->statis));

		if (xlk)
		{
			if (x < ptd->org_x)
				switch_link_before(root, xlk, ptd->xax);
			else
				switch_link_after(root, xlk, ptd->xax);
		}
		else
		{
			calc_statis_xax_scope(&cb, ptd->statis, ptd->cur_page, &xlk_first, &xlk_last);

			if (x < ptd->org_x)
				switch_link_before(root, xlk_first, ptd->xax);
			else
				switch_link_after(root, xlk_last, ptd->xax);
		}
	}

	widget_erase(widget, NULL);

	pt.x = x;
	pt.y = y;
	noti_statis_owner(widget, NC_XAXDROP, ptd->statis, ptd->xax, NULL, NULL, (void*)&pt);
}

void noti_statis_yax_selected(res_win_t widget, link_t_ptr ylk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ylk);

	if (!statis_is_design(ptd->statis))
		return;

	set_yax_selected(ylk, ((get_yax_selected(ylk)) ? 0 : 1));

	noti_statis_owner(widget, NC_YAXSELECTED, ptd->statis, NULL, ylk, NULL, NULL);

	_statisctrl_yax_rect(widget, ylk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_erase(widget, &xr);
}

bool_t noti_statis_xax_insert(res_win_t widget, link_t_ptr xlk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(xlk);

	if (noti_statis_owner(widget, NC_XAXINSERT, ptd->statis, xlk, NULL, NULL, NULL))
		return 0;

	return 1;
}

bool_t noti_statis_xax_delete(res_win_t widget, link_t_ptr xlk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(xlk);

	if (noti_statis_owner(widget, NC_XAXDELETE, ptd->statis, xlk, NULL, NULL, NULL))
		return 0;

	return 1;
}

bool_t noti_statis_xax_changing(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->xax);

	if (noti_statis_owner(widget, NC_XAXCHANGING, ptd->statis, ptd->xax, NULL, NULL, NULL))
		return (bool_t)0;

	_statisctrl_xax_rect(widget, ptd->xax, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->xax = NULL;

	widget_erase(widget, &xr);

	return (bool_t)1;
}

void noti_statis_xax_changed(res_win_t widget, link_t_ptr xlk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(xlk && !ptd->xax);

	_statisctrl_xax_rect(widget, xlk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->xax = xlk;

	widget_erase(widget, &xr);

	noti_statis_owner(widget, NC_XAXCHANGED, ptd->statis, ptd->xax, NULL, NULL, NULL);
}

void noti_statis_xax_checked(res_win_t widget, link_t_ptr xlk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(xlk);

	set_xax_checked(xlk, ((get_xax_checked(xlk)) ? 0 : 1));

	noti_statis_owner(widget, NC_XAXCHECKED, ptd->statis, xlk, NULL, NULL, NULL);

	_statisctrl_xax_rect(widget, xlk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_erase(widget, &xr);
}

void noti_statis_gax_changing(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->gax);

	noti_statis_owner(widget, NC_GAXCHANGING, ptd->statis, NULL, NULL, ptd->gax, NULL);

	_statisctrl_gaxbar_rect(widget, ptd->gax, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->gax = NULL;

	widget_erase(widget, &xr);
}

void noti_statis_gax_changed(res_win_t widget, link_t_ptr glk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(glk && !ptd->gax);

	_statisctrl_gaxbar_rect(widget, glk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->gax = glk;

	widget_erase(widget, &xr);

	noti_statis_owner(widget, NC_GAXCHANGED, ptd->statis, NULL, NULL, ptd->gax, NULL);
}

void noti_statis_yax_changing(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->yax);

	noti_statis_owner(widget, NC_YAXCHANGING, ptd->statis, NULL, ptd->yax, NULL, NULL);

	_statisctrl_yax_rect(widget, ptd->yax, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->yax = NULL;

	widget_erase(widget, &xr);
}

void noti_statis_yax_changed(res_win_t widget, link_t_ptr ylk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ylk && !ptd->yax);

	_statisctrl_yax_rect(widget, ylk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->yax = ylk;

	widget_erase(widget, &xr);

	noti_statis_owner(widget, NC_YAXCHANGED, ptd->statis, NULL, ptd->yax, NULL, NULL);
}

void noti_statis_yax_enter(res_win_t widget, link_t_ptr yax)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(yax);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = yax;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
}

void noti_statis_yax_leave(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
}

void noti_statis_yax_hover(res_win_t widget, int x, int y)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_statis_owner(widget, NC_YAXHOVER, ptd->statis, NULL, ptd->hover, NULL, (void*)&xp);
}

void noti_statis_begin_edit(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
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
	parse_xfont_from_style(&xf, get_statis_style_ptr(ptd->statis));
	widget_get_color_mode(widget, &ob);

	_statisctrl_coor_rect(widget, ptd->xax, ptd->yax, &xr);
	pt_expand_rect(&xr, -1, -1);

	if (noti_statis_owner(widget, NC_COOREDITING, ptd->statis, ptd->xax, ptd->yax, NULL, NULL))
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
	widget_show(ptd->editor, WS_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	if (ptd->yax)
		text = get_coor_text_ptr(ptd->xax, ptd->yax);
	else
		text = get_xax_text_ptr(ptd->xax);

	editbox_set_text(ptd->editor, text);
	editbox_selectall(ptd->editor);
}

void noti_statis_commit_edit(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	const tchar_t* text;
	res_win_t editctrl;
	link_t_ptr xlk_new;
	bool_t b_accept = 0;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->xax);

	text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
	b_accept = (noti_statis_owner(widget, NC_COORCOMMIT, ptd->statis, ptd->xax, ptd->yax, NULL, (void*)text) == 0) ? 1 : 0;
	if (b_accept)
	{
		statisctrl_set_coor_text(widget, ptd->xax, ptd->yax, text);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);

	if (!b_accept)
		return;

	if (ptd->b_auto && (ptd->xax == get_prev_xax(ptd->statis, LINK_LAST)) && (ptd->yax == get_prev_yax(ptd->statis, LINK_LAST)))
	{
		xlk_new = insert_xax(ptd->statis, LINK_LAST);
		set_xax_state(xlk_new, dsNewClean);

		if (!noti_statis_xax_insert(widget, xlk_new))
		{
			delete_xax(xlk_new);
			return;
		}

		statisctrl_set_focus_coor(widget, xlk_new, get_next_yax(ptd->statis, LINK_FIRST));
	}
	else
	{
		if (ptd->yax == get_prev_yax(ptd->statis, LINK_LAST))
		{
			xlk_new = get_next_xax(ptd->statis, ptd->xax);
			if (xlk_new)
			{
				statisctrl_set_focus_coor(widget, xlk_new, get_next_xax(ptd->statis, LINK_FIRST));
			}
		}
		else
		{
			statisctrl_tabskip(widget,TABORDER_DOWN);
		}
	}
}

void noti_statis_rollback_edit(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->xax);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_statis_reset_editor(res_win_t widget, bool_t bCommit)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_statis_commit_edit(widget);
		else
			noti_statis_rollback_edit(widget);
	}
}

/*******************************************************************************************/

int hand_statis_create(res_win_t widget, void* data)
{
	statis_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (statis_delta_t*)xmem_alloc(sizeof(statis_delta_t));

	SETSTATISDELTA(widget, ptd);

	ptd->b_lock = 1;
	ptd->stack = create_stack_table();

	return 0;
}

void hand_statis_destroy(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_statis_reset_editor(widget, 0);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	_statisctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETSTATISDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_statis_undo(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	_statisctrl_undo(widget);
}

void hand_statis_copy(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	_statisctrl_copy(widget);
}

void hand_statis_cut(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	_statisctrl_done(widget);

	if (!_statisctrl_cut(widget))
	{
		_statisctrl_discard(widget);
	}
}

void hand_statis_paste(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	_statisctrl_done(widget);

	if (!_statisctrl_paste(widget))
	{
		_statisctrl_discard(widget);
	}
}

void hand_statis_size(res_win_t widget, int code, const xsize_t* psc)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	statisctrl_redraw(widget, 1);
}

void hand_statis_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_statis_wheel(res_win_t widget, bool_t bHorz, int nDelta)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	scroll_t scr = { 0 };
	int nLine;
	res_win_t win;

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	widget_get_scroll_info(widget, bHorz, &scr);

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

void hand_statis_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr xax, yax, gax;
	int nHint;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->statis)
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

	nHint = calc_statis_hint(&cb, &pt,ptd->statis, ptd->cur_page,  &xax, &yax, &gax);

	if (nHint == STATIS_HINT_HORZ_SPLIT && yax == ptd->yax && !(dw & KS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_statis_yax_sizing(widget, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget,CURSOR_SIZENS);
	}
	else if (nHint == STATIS_HINT_VERT_SPLIT && xax == ptd->xax && !(dw & KS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_statis_xax_sizing(widget, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget,CURSOR_SIZEWE);
	}
	else if (nHint == STATIS_HINT_YAXBAR && yax == ptd->yax && !(dw & KS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_statis_yax_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == STATIS_HINT_XAXBAR && xax == ptd->xax && !(dw & KS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_statis_xax_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == STATIS_HINT_NONE)
	{
		widget_set_cursor(widget, CURSOR_ARROW);
	}
}

void hand_statis_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	if (ptd->hover)
		noti_statis_yax_hover(widget, pxp->x, pxp->y);
}

void hand_statis_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	if (ptd->hover)
		noti_statis_yax_leave(widget);
}

void hand_statis_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	if (ptd->yax && !ptd->xax)
	{
		if (get_yax_sortable(ptd->yax))
		{
			//sort_statis_yax(ptd->statis,ptd->yax);
			//widget_erase(widget, 0, 0, 0, 0);
		}
	}

	noti_statis_owner(widget, NC_STATISDBCLK, ptd->statis, ptd->xax, ptd->yax, NULL, (void*)pxp);
}

void hand_statis_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr xlk, ylk, glk;
	int nHint;
	bool_t bReYax, bReXax;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_statis_hint(&cb, &pt,ptd->statis, ptd->cur_page,  &xlk, &ylk, &glk);

	bReXax = (xlk == ptd->xax) ? (bool_t)1 : (bool_t)0;
	bReYax = (ylk == ptd->yax) ? (bool_t)1 : (bool_t)0;

	if (nHint == STATIS_HINT_NULBAR)
	{
		noti_statis_reset_check(widget);
	}
	else if (nHint == STATIS_HINT_XAXBAR)
	{
		noti_statis_xax_checked(widget, xlk);
	}
	else if (nHint == STATIS_HINT_YAXBAR)
	{
		if (widget_key_state(widget, KEY_CONTROL))
			noti_statis_yax_selected(widget, ylk);
	}
	else if (nHint == STATIS_HINT_NONE)
	{
		if (!widget_key_state(widget, KEY_CONTROL))
		{
			if (statis_is_design(ptd->statis))
				noti_statis_reset_select(widget);
		}
	}
}

void hand_statis_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr xlk, ylk, glk;
	int nHint;
	bool_t bReGax, bReYax, bReXax;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->statis)
		return;

	if (ptd->b_size_xax && ptd->xax)
	{
		noti_statis_xax_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_size_yax && ptd->yax)
	{
		noti_statis_yax_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag_xax && ptd->xax)
	{
		noti_statis_xax_drop(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag_yax && ptd->yax)
	{
		noti_statis_yax_drop(widget, pxp->x, pxp->y);
		return;
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	nHint = calc_statis_hint(&cb, &pt, ptd->statis, ptd->cur_page, &xlk, &ylk, &glk);

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
		if (!noti_statis_xax_changing(widget))
			bReXax = 1;
	}

	if (!bReGax && ptd->gax)
	{
		noti_statis_gax_changing(widget);
	}

	if (!bReYax && ptd->yax)
	{
		noti_statis_yax_changing(widget);
	}

	if (!bReYax && ylk)
		noti_statis_yax_changed(widget, ylk);

	if (!bReGax && glk)
		noti_statis_gax_changed(widget, glk);

	if (!bReXax && xlk)
		noti_statis_xax_changed(widget, xlk);

	if (!bReYax || !bReXax)
	{
		if (ptd->xax && ptd->yax)
		{
			_statisctrl_ensure_visible(widget);
			noti_statis_owner(widget, NC_COORSETFOCUS, ptd->statis, ptd->xax, ptd->yax, NULL, NULL);
		}
	}

	noti_statis_owner(widget, NC_STATISLBCLK, ptd->statis, ptd->xax, ptd->yax, ptd->gax, (void*)pxp);
}

void hand_statis_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);
}

void hand_statis_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	noti_statis_owner(widget, NC_STATISRBCLK, ptd->statis, ptd->xax, ptd->yax, ptd->gax, (void*)pxp);
}

void hand_statis_keydown(res_win_t widget, dword_t ks, int nKey)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	if (statis_is_design(ptd->statis))
	{
		if ((nKey == _T('z') || nKey == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
		{
			hand_statis_undo(widget);
			return;
		}
		else if ((nKey == _T('c') || nKey == _T('C')) && widget_key_state(widget, KEY_CONTROL))
		{
			hand_statis_copy(widget);
			return;
		}
		else if ((nKey == _T('x') || nKey == _T('X')) && widget_key_state(widget, KEY_CONTROL))
		{
			hand_statis_cut(widget);
			return;
		}
		else if ((nKey == _T('v') || nKey == _T('V')) && widget_key_state(widget, KEY_CONTROL))
		{
			hand_statis_paste(widget);
			return;
		}
	}

	switch (nKey)
	{
	case KEY_ENTER:
		if (ptd->xax)
		{
			noti_statis_begin_edit(widget);
		}
		break;
	case KEY_LEFT:
		statisctrl_tabskip(widget, TABORDER_LEFT);
		break;
	case KEY_RIGHT:
		statisctrl_tabskip(widget, TABORDER_RIGHT);
		break;
	case KEY_UP:
		statisctrl_tabskip(widget, TABORDER_UP);
		break;
	case KEY_DOWN:
		statisctrl_tabskip(widget, TABORDER_DOWN);
		break;
	case KEY_END:
		statisctrl_tabskip(widget, TABORDER_END);
		break;
	case KEY_HOME:
		statisctrl_tabskip(widget, TABORDER_HOME);
		break;
	case KEY_PAGEUP:
		statisctrl_tabskip(widget, TABORDER_PAGEUP);
		break;
	case KEY_PAGEDOWN:
		statisctrl_tabskip(widget, TABORDER_PAGEDOWN);
		break;
	}
}

void hand_statis_char(res_win_t widget, tchar_t nChar)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (!ptd->statis)
		return;

	if (IS_VISIBLE_CHAR(nChar) && !widget_is_valid(ptd->editor))
	{
		hand_statis_keydown(widget, 0, KEY_ENTER);
	}

	if (IS_VISIBLE_CHAR(nChar) && widget_is_valid(ptd->editor))
	{
		widget_post_char(ptd->editor, nChar);
	}
}

void hand_statis_child_command(res_win_t widget, int code, var_long data)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_statis_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_statis_rollback_edit(widget);
		break;
	}
}

void hand_statis_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	if (ptd->statis && statis_is_design(ptd->statis))
	{
		if (cid == IDC_EDITMENU)
		{
			switch (code)
			{
			case COMMAND_COPY:
				hand_statis_copy(widget);
				break;
			case COMMAND_CUT:
				hand_statis_cut(widget);
				break;
			case COMMAND_PASTE:
				hand_statis_paste(widget);
				break;
			case COMMAND_UNDO:
				hand_statis_undo(widget);
				break;
			}

			if (widget_is_valid((res_win_t)data))
			{
				widget_close((res_win_t)data, 1);
			}
		}
	}
}

void hand_statis_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->statis)
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

	draw_statis_page(pif, &cb, ptd->statis, ptd->cur_page);

	//draw focus
	if (ptd->xax && ptd->yax)
	{
		_statisctrl_coor_rect(widget, ptd->xax, ptd->yax, &xr);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}
	else if (ptd->yax)
	{
		_statisctrl_yaxbar_rect(widget, ptd->yax, &xr);

		parse_xcolor(&xc, DEF_DISABLE_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}
	else if (ptd->xax)
	{
		_statisctrl_xaxbar_rect(widget, ptd->xax, &xr);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}
	else if (ptd->gax)
	{
		_statisctrl_gaxbar_rect(widget, ptd->gax, &xr);

		parse_xcolor(&xc, DEF_DISABLE_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/*********************************************************************************/

res_win_t statisctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_statis_create)
		EVENT_ON_DESTROY(hand_statis_destroy)

		EVENT_ON_PAINT(hand_statis_paint)

		EVENT_ON_SIZE(hand_statis_size)

		EVENT_ON_SCROLL(hand_statis_scroll)
		EVENT_ON_WHEEL(hand_statis_wheel)

		EVENT_ON_KEYDOWN(hand_statis_keydown)
		EVENT_ON_CHAR(hand_statis_char)

		EVENT_ON_MOUSE_MOVE(hand_statis_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_statis_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_statis_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_statis_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_statis_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_statis_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_statis_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_statis_rbutton_up)

		EVENT_ON_CHILD_COMMAND(hand_statis_child_command)
		EVENT_ON_MENU_COMMAND(hand_statis_menu_command)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void statisctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_statis_doc(ptr));

	noti_statis_reset_editor(widget, 0);

	ptd->statis = ptr;
	ptd->yax = NULL;
	ptd->xax = NULL;

	ptd->cur_page = 1;

	statisctrl_redraw(widget, 1);

	if (!ptd->b_lock && ptd->b_auto && !get_next_xax(ptr, LINK_FIRST))
	{
		statisctrl_insert_xax(widget, LINK_LAST);
	}
}

link_t_ptr statisctrl_detach(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	noti_statis_reset_editor(widget, 0);

	data = ptd->statis;
	ptd->statis = NULL;

	ptd->cur_page = 0;

	widget_erase(widget, NULL);

	return data;
}

link_t_ptr statisctrl_fetch(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->statis;
}

void statisctrl_accept(res_win_t widget, bool_t bAccept)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr xlk, nxt;
	int ds;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, bAccept);

	xlk = get_next_xax(ptd->statis, LINK_FIRST);
	while (xlk)
	{
		nxt = get_next_xax(ptd->statis, xlk);

		ds = get_xax_state(xlk);
		if (ds == dsNewClean)
			delete_xax(xlk);

		xlk = nxt;
	}

	statisctrl_redraw(widget, 0);
}

bool_t statisctrl_is_update(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return 0;

	return (get_update_xax_count(ptd->statis)) ? 1 : 0;
}

void statisctrl_auto_insert(res_win_t widget, bool_t bAuto)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_auto = bAuto;
}

void statisctrl_redraw(res_win_t widget, bool_t bCalc)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr xlk, ylk, glk;
	bool_t b_valid;
	bool_t b;
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 0);

	ptd->hover = NULL;

	b_valid = 0;
	glk = get_next_gax(ptd->statis, LINK_FIRST);
	while (glk)
	{
		if (glk == ptd->gax)
			b_valid = 1;

		glk = get_next_gax(ptd->statis, glk);
	}

	if (!b_valid)
		ptd->gax = NULL;

	b_valid = 0;
	ylk = get_next_yax(ptd->statis, LINK_FIRST);
	while (ylk)
	{
		if (ylk == ptd->yax)
			b_valid = 1;

		noti_statis_owner(widget, NC_YAXCALCED, ptd->statis, NULL, ylk, NULL, NULL);

		ylk = get_next_yax(ptd->statis, ylk);
	}

	if (!b_valid)
		ptd->yax = NULL;

	b_valid = 0;
	xlk = get_next_xax(ptd->statis, LINK_FIRST);
	while (xlk)
	{
		if (xlk == ptd->xax)
			b_valid = 1;

		noti_statis_owner(widget, NC_XAXCALCED, ptd->statis, xlk, NULL, NULL, NULL);

		xlk = get_next_xax(ptd->statis, xlk);
	}

	if (!b_valid)
		ptd->xax = NULL;

	noti_statis_owner(widget, NC_STATISCALCED, ptd->statis, NULL, NULL, NULL, NULL);

	b = (widget_get_style(widget) & WD_STYLE_PAGING) ? 1 : 0;
	if (!b)
	{
		widget_get_client_rect(widget, &xr);
		widget_rect_to_tm(widget, &xr);
		set_statis_height(ptd->statis, xr.fh);
	}

	_statisctrl_reset_page(widget);

	widget_update(widget);
}

void statisctrl_redraw_xax(res_win_t widget, link_t_ptr xlk, bool_t bCalc)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 0);

#ifdef _DEBUG
	XDL_ASSERT(is_statis_xax(ptd->statis, xlk));
#endif

	noti_statis_owner(widget, NC_XAXCALCED, ptd->statis, xlk, NULL, NULL, NULL);

	_statisctrl_xax_rect(widget, xlk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_erase(widget, &xr);
}

void statisctrl_redraw_yax(res_win_t widget, link_t_ptr ylk, bool_t bCalc)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 0);

#ifdef _DEBUG
	XDL_ASSERT(is_statis_yax(ptd->statis, ylk));
#endif

	noti_statis_owner(widget, NC_YAXCALCED, ptd->statis, ylk, NULL, NULL, NULL);

	_statisctrl_yax_rect(widget, ylk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_erase(widget, &xr);
}

void statisctrl_tabskip(res_win_t widget, int dir)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr xlk, ylk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	xlk = ptd->xax;
	ylk = ptd->yax;

	switch (dir)
	{
	case TABORDER_UP:
		if (ylk == NULL)
			ylk = get_prev_yax(ptd->statis, LINK_LAST);
		else
			ylk = get_prev_yax(ptd->statis, ylk);

		if (ylk)
			statisctrl_set_focus_coor(widget, xlk, ylk);
		break;
	case TABORDER_DOWN:
		if (ylk == NULL)
			ylk = get_next_yax(ptd->statis, LINK_FIRST);
		else
			ylk = get_next_yax(ptd->statis, ylk);

		if (ylk)
			statisctrl_set_focus_coor(widget, xlk, ylk);
		break;
	case TABORDER_LEFT:
		if (xlk != NULL)
		{
			xlk = get_prev_xax(ptd->statis, xlk);
			if (xlk)
				statisctrl_set_focus_coor(widget, xlk, ylk);
		}
		break;
	case TABORDER_RIGHT:
		if (xlk != NULL)
		{
			xlk = get_next_xax(ptd->statis, xlk);
			if (xlk)
				statisctrl_set_focus_coor(widget, xlk, ylk);
		}
		break;
	case TABORDER_HOME:
		statisctrl_move_first_page(widget);
		break;
	case TABORDER_END:
		statisctrl_move_last_page(widget);
		break;
	case TABORDER_PAGEUP:
		statisctrl_move_prev_page(widget);
		break;
	case TABORDER_PAGEDOWN:
		statisctrl_move_next_page(widget);
		break;
	}
}

bool_t statisctrl_delete_xax(res_win_t widget, link_t_ptr xlk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr nlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return 0;

	noti_statis_reset_editor(widget, 0);

	if (ptd->b_lock)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_statis_xax(ptd->statis, xlk));
#endif

	if (!noti_statis_xax_delete(widget, xlk))
		return 0;

	if (xlk == ptd->xax)
	{
		noti_statis_xax_changing(widget);
	}

	nlk = get_next_xax(ptd->statis, xlk);
	if (!nlk)
		nlk = get_prev_xax(ptd->statis, xlk);

	delete_xax(xlk);

	statisctrl_redraw(widget, 1);

	if (nlk)
		statisctrl_set_focus_coor(widget, nlk, ptd->yax);

	return 1;
}

link_t_ptr statisctrl_insert_xax(res_win_t widget, link_t_ptr pre)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	link_t_ptr xlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return NULL;

	noti_statis_reset_editor(widget, 0);

	if (ptd->b_lock)
		return NULL;

	if (pre == LINK_FIRST)
		pre = get_next_xax(ptd->statis, LINK_FIRST);
	else if (pre == LINK_LAST)
		pre = get_prev_xax(ptd->statis, LINK_LAST);
	else
	{
#ifdef _DEBUG
		XDL_ASSERT(is_statis_xax(ptd->statis, pre));
#endif
	}

	if (!pre) pre = LINK_FIRST;

	xlk = insert_xax(ptd->statis, pre);
	set_xax_state(xlk, dsNewClean);

	if (!noti_statis_xax_insert(widget, xlk))
	{
		delete_xax(xlk);
		return NULL;
	}

	statisctrl_redraw(widget, 1);

	statisctrl_set_focus_coor(widget, xlk, LINK_FIRST);

	return xlk;
}

bool_t statisctrl_set_coor_text(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, const tchar_t* szText)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	const tchar_t* text;
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_statis_xax(ptd->statis, xlk));
#endif

	if (ylk)
	{
		text = get_coor_text_ptr(xlk, ylk);
		if (compare_numeric(szText, text, get_yax_data_dig(ylk)) != 0)
		{
			set_coor_text(xlk, ylk, szText, -1);
			set_coor_dirty(xlk, ylk, 1);
			set_xax_dirty(xlk);

			noti_statis_owner(widget, NC_COORUPDATE, ptd->statis, xlk, ylk, NULL, NULL);

			statisctrl_get_coor_rect(widget, xlk, ylk, &xr);
			pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
			widget_erase(widget, &xr);
		}
	}
	else
	{
		text = get_xax_text_ptr(xlk);
		if (compare_text(szText, -1, text, -1, 0) != 0)
		{
			set_xax_text(xlk, szText);
			set_xax_dirty(xlk);

			noti_statis_owner(widget, NC_COORUPDATE, ptd->statis, xlk, ylk, NULL, NULL);

			_statisctrl_xaxbar_rect(widget, xlk, &xr);
			pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
			widget_erase(widget, &xr);
		}
	}

	return 1;
}

bool_t statisctrl_set_focus_coor(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	bool_t bReXax, bReYax;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return 0;

	if (xlk == LINK_FIRST)
		xlk = get_next_xax(ptd->statis, LINK_FIRST);
	else if (xlk == LINK_LAST)
		xlk = get_prev_xax(ptd->statis, LINK_LAST);

	if (ylk == LINK_FIRST)
		ylk = get_next_yax(ptd->statis, LINK_FIRST);
	else if (ylk == LINK_LAST)
		ylk = get_prev_yax(ptd->statis, LINK_LAST);

	bReXax = (xlk == ptd->xax) ? 1 : 0;
	bReYax = (ylk == ptd->yax) ? 1 : 0;

	if (bReXax && bReYax)
	{
		return 1;
	}

	if (!bReXax && ptd->xax)
	{
		if (!noti_statis_xax_changing(widget))
			return 0;
	}

	if (!bReYax && ptd->yax)
	{
		noti_statis_yax_changing(widget);
	}

	if (!bReXax || !bReYax)
	{
		if (ptd->xax && ptd->yax)
			noti_statis_owner(widget, NC_COORKILLFOCUS, ptd->statis, ptd->xax, ptd->yax, NULL, NULL);
	}

	if (!bReYax && ylk)
		noti_statis_yax_changed(widget, ylk);

	if (!bReXax && xlk)
		noti_statis_xax_changed(widget, xlk);

	if (!bReXax || !bReYax)
	{
		if (ptd->xax && ptd->yax)
			noti_statis_owner(widget, NC_COORSETFOCUS, ptd->statis, ptd->xax, ptd->yax, NULL, NULL);
	}

	_statisctrl_ensure_visible(widget);

	return 1;
}

link_t_ptr statisctrl_get_focus_xax(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return NULL;

	return ptd->xax;
}

link_t_ptr statisctrl_get_focus_yax(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return NULL;

	return ptd->yax;
}

link_t_ptr statisctrl_get_focus_gax(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return NULL;

	return ptd->gax;
}

void statisctrl_move_first_page(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	nCurPage = ptd->cur_page;

	if (nCurPage != 1)
	{
		nCurPage = 1;
		ptd->cur_page = nCurPage;

		widget_erase(widget, NULL);
	}
}

void statisctrl_move_prev_page(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	nCurPage = ptd->cur_page;

	if (nCurPage > 1)
	{
		nCurPage--;
		ptd->cur_page = nCurPage;

		widget_erase(widget, NULL);
	}
}

void statisctrl_move_next_page(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_statis_pages(&cb, ptd->statis);

	if (nCurPage < nMaxPage)
	{
		nCurPage++;
		ptd->cur_page = nCurPage;

		widget_erase(widget, NULL);
	}
}

void statisctrl_move_last_page(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_statis_pages(&cb, ptd->statis);

	if (nCurPage != nMaxPage)
	{
		nCurPage = nMaxPage;
		ptd->cur_page = nCurPage;

		widget_erase(widget, NULL);
	}
}

void statisctrl_move_to_page(res_win_t widget, int page)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	int nCurPage, nMaxPage;
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	noti_statis_reset_editor(widget, 1);

	widget_get_canv_rect(widget, &cb);

	nCurPage = ptd->cur_page;
	nMaxPage = calc_statis_pages(&cb, ptd->statis);

	if (page > 0 && page != nCurPage && page <= nMaxPage)
	{
		nCurPage = page;
		ptd->cur_page = nCurPage;

		widget_erase(widget, NULL);
	}
}

int statisctrl_get_cur_page(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return 0;

	return ptd->cur_page;
}

int statisctrl_get_max_page(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	canvbox_t cb;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return 0;

	widget_get_canv_rect(widget, &cb);

	return calc_statis_pages(&cb, ptd->statis);
}

void statisctrl_get_coor_rect(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	_statisctrl_coor_rect(widget, xlk, ylk, pxr);
}

bool_t statisctrl_get_lock(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

void statisctrl_set_lock(res_win_t widget, bool_t bLock)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t statisctrl_get_dirty(res_win_t widget)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return 0;

	if (!statis_is_design(ptd->statis))
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void statisctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	statis_delta_t* ptd = GETSTATISDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->statis)
		return;

	if (!statis_is_design(ptd->statis))
		return;

	if (bDirty)
		_statisctrl_done(widget);
	else
		_statisctrl_clean(widget);
}

