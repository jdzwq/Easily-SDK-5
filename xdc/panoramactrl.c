/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc panorama control document

	@module	panoramactrl.c | implement file

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

#define PANORAMA_LINE_FEED		(float)50
#define PANORAMA_PLOT_MIN_WIDTH	(float)10
#define PANORAMA_PLOT_MIN_HEIGHT	(float)10

typedef struct _panorama_delta_t{
	link_t_ptr panorama;
	link_t_ptr plot;
	link_t_ptr hover;

	int org_hint;
	long org_x, org_y;
	long cur_x, cur_y;
	short cur_page;

	int opera;

	bool_t b_drag;
	bool_t b_size;

	link_t_ptr stack;
}panorama_delta_t;

#define GETPANORAMADELTA(ph) 	(panorama_delta_t*)widget_get_user_delta(ph)
#define SETPANORAMADELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/******************************************panorama event********************************************************/
static void _panoramactrl_done(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->panorama);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->panorama, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->panorama, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->panorama, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->panorama, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _panoramactrl_undo(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->panorama);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_panorama_doc(ptd->panorama);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->panorama, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->panorama, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		panoramactrl_redraw(widget);
	}
}

static void _panoramactrl_discard(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _panoramactrl_clean(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _panoramactrl_copy(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->panorama);

	if (!get_panorama_plot_selected_count(ptd->panorama))
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_panorama_doc();
	ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
	while (ilk)
	{
		if (get_field_selected(ilk))
		{
			nlk = insert_panorama_plot(dom, get_panorama_plot_class_ptr(ilk));
			copy_dom_node(nlk, ilk);
		}

		ilk = get_panorama_next_plot(ptd->panorama, ilk);
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

		destroy_panorama_doc(dom);
		return 0;
	}

	clipboard_close();

	destroy_panorama_doc(dom);
	return 1;
}

static bool_t _panoramactrl_cut(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->panorama);

	if (!_panoramactrl_copy(widget))
		return 0;

	ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
	while (ilk)
	{
		nxt = get_panorama_next_plot(ptd->panorama, ilk);

		if (get_panorama_plot_selected(ilk))
		{
			delete_panorama_plot(ilk);
		}
		ilk = nxt;
	}

	panoramactrl_redraw(widget);

	return 1;
}

static bool_t _panoramactrl_paste(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk;

	float y;
	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->panorama);

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

	if (!is_panorama_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	while (nlk = get_panorama_next_plot(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_panorama_plotset(dom), nlk);
		attach_dom_node(get_panorama_plotset(ptd->panorama), LINK_LAST, nlk);

		y = get_panorama_plot_y(nlk);
		y += get_panorama_plot_height(nlk);
		set_panorama_plot_y(nlk, y);

		xsprintf(sz_name, _T("%s%d"), get_panorama_plot_class_ptr(nlk), get_panorama_plot_count_by_class(ptd->panorama, get_panorama_plot_class_ptr(nlk)));
		set_panorama_plot_name(nlk, sz_name);
	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);
	panoramactrl_redraw(widget);

	return 1;
}

static void _panoramactrl_plot_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	calc_panorama_plot_rect(ptd->panorama, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _panoramactrl_reset_page(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_panorama_width(ptd->panorama);
	xs.fy = get_panorama_height(ptd->panorama);

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

static void _panoramactrl_ensure_visible(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	xrect_t xr = { 0 };

	if (!ptd->plot)
		return;

	_panoramactrl_plot_rect(widget, ptd->plot, &xr);

	widget_ensure_visible(widget, &xr, 1);
}
/*********************************************************************************************************/
int noti_panorama_owner(res_win_t widget, unsigned long code, link_t_ptr ptr, link_t_ptr ilk, void* data)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	NOTICE_PANORAMA nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;

	nf.panorama = ptr;
	nf.plot = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_panorama_reset_select(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	link_t_ptr ilk;
	int count = 0;

	ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
	while (ilk)
	{
		if (get_panorama_plot_selected(ilk))
		{
			set_panorama_plot_selected(ilk, 0);
			noti_panorama_owner(widget, NC_PANORAMAPLOTSELECTED, ptd->panorama, ilk, NULL);

			count++;
		}

		ilk = get_panorama_next_plot(ptd->panorama, ilk);
	}

	if (count)
	{
		widget_redraw(widget, NULL, 0);
	}
}

void noti_panorama_plot_selected(res_win_t widget, link_t_ptr ilk)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	xrect_t xr;
	bool_t b_check;

	b_check = get_panorama_plot_selected(ilk);

	if (b_check)
		set_panorama_plot_selected(ilk, 0);
	else
		set_panorama_plot_selected(ilk, 1);

	noti_panorama_owner(widget, NC_PANORAMAPLOTSELECTED, ptd->panorama, ilk, NULL);

	_panoramactrl_plot_rect(widget, ilk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

bool_t noti_panorama_plot_changing(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->plot);

	if (noti_panorama_owner(widget, NC_PANORAMAPLOTCHANGING, ptd->panorama, ptd->plot, NULL))
		return (bool_t)0;

	_panoramactrl_plot_rect(widget, ptd->plot, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->plot = NULL;

	widget_redraw(widget, &xr, 0);

	return (bool_t)1;
}

void noti_panorama_plot_changed(res_win_t widget, link_t_ptr ilk)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->plot);

	ptd->plot = ilk;

	_panoramactrl_plot_rect(widget, ptd->plot, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	noti_panorama_owner(widget, NC_PANORAMAPLOTCHANGED, ptd->panorama, ilk, NULL);
}

void noti_panorama_plot_enter(res_win_t widget, link_t_ptr ilk)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = ilk;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
}

void noti_panorama_plot_leave(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
}

void noti_panorama_plot_hover(res_win_t widget, long x, long y)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->hover != NULL);

	pt.x = x;
	pt.y = y;
	noti_panorama_owner(widget, NC_PANORAMAPLOTHOVER, ptd->panorama, ptd->hover, (void*)&pt);
}

void noti_panorama_plot_drag(res_win_t widget, long x, long y)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->plot);

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
	noti_panorama_owner(widget, NC_PANORAMAPLOTDRAG, ptd->panorama, ptd->plot, (void*)&pt);
}

void noti_panorama_plot_drop(res_win_t widget, long x, long y)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	
	xpoint_t pt;
	xrect_t xr;
	long cx, cy;

	XDL_ASSERT(ptd->plot);

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

	calc_panorama_plot_rect(ptd->panorama, ptd->plot, &xr);

	widget_rect_to_pt(widget, &xr);

	if (xr.x + cx < 0 || xr.y + cy < 0)
		return;

	_panoramactrl_done(widget);

	pt.x = xr.x + cx;
	pt.y = xr.y + cy;

	widget_point_to_tm(widget, &pt);

	pt.fx = (float)((long)(pt.fx));
	pt.fy = (float)((long)(pt.fy));

	set_panorama_plot_x(ptd->plot, pt.fx);
	set_panorama_plot_y(ptd->plot, pt.fy);

	widget_redraw(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_panorama_owner(widget, NC_PANORAMAPLOTDROP, ptd->panorama, ptd->plot, (void*)&pt);
}

void noti_panorama_plot_sizing(res_win_t widget, int hint, long x, long y)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->plot);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	if (hint == PANORAMA_HINT_HORZ_SPLIT)
	{
		widget_set_cursor(widget,CURSOR_SIZENS);
	}
	else if (hint == PANORAMA_HINT_VERT_SPLIT)
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

	_panoramactrl_plot_rect(widget, ptd->plot, &xr);

	noti_panorama_owner(widget, NC_PANORAMAPLOTSIZING, ptd->panorama, ptd->plot, (void*)&xr);
}

void noti_panorama_plot_sized(res_win_t widget, long x, long y)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	float minw, minh, fw, fh;
	int hint;
	xrect_t xr;
	xsize_t xs;

	XDL_ASSERT(ptd->plot);

	ptd->cur_x = x;
	ptd->cur_y = y;

	ptd->b_size = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	hint = ptd->org_hint;

	minw = PANORAMA_PLOT_MIN_WIDTH;
	minh = PANORAMA_PLOT_MIN_HEIGHT;

	xs.cx = ptd->cur_x - ptd->org_x;
	xs.cy = ptd->cur_y - ptd->org_y;

	if (!xs.cx && !xs.cy)
		return;

	widget_size_to_tm(widget, &xs);

	_panoramactrl_plot_rect(widget, ptd->plot, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	fw = get_panorama_plot_width(ptd->plot);
	fh = get_panorama_plot_height(ptd->plot);

	fw += xs.fx;
	fh += xs.fy;

	if (fw < minw)
		fw = minw;

	if (fh < minh)
		fh = minh;

	fw = (float)((long)fw);
	fh = (float)((long)fh);

	_panoramactrl_done(widget);

	if (hint == PANORAMA_HINT_HORZ_SPLIT)
	{
		set_panorama_plot_height(ptd->plot, fh);
	}
	else if (hint == PANORAMA_HINT_VERT_SPLIT)
	{
		set_panorama_plot_width(ptd->plot, fw);
	}
	else
	{
		set_panorama_plot_width(ptd->plot, fw);
		set_panorama_plot_height(ptd->plot, fh);
	}

	_panoramactrl_plot_rect(widget, ptd->plot, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	_panoramactrl_plot_rect(widget, ptd->plot, &xr);

	noti_panorama_owner(widget, NC_PANORAMAPLOTSIZED, ptd->panorama, ptd->plot, (void*)&xr);
}

void noti_panorama_calc(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
}

/*******************************************************************************/
int hand_panorama_create(res_win_t widget, void* data)
{
	panorama_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (panorama_delta_t*)xmem_alloc(sizeof(panorama_delta_t));
	xmem_zero((void*)ptd, sizeof(panorama_delta_t));

	ptd->stack = create_stack_table();

	SETPANORAMADELTA(widget, ptd);

	return 0;
}

void hand_panorama_destroy(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd != NULL);

	_panoramactrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETPANORAMADELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_panorama_size(res_win_t widget, int code, const xsize_t* prs)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	panoramactrl_redraw(widget);
}

void hand_panorama_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_panorama_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->panorama)
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

void hand_panorama_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	int nHint;
	link_t_ptr ilk;
	xpoint_t pt;

	if (!ptd->panorama)
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
	nHint = calc_panorama_hint(ptd->panorama, &pt, &ilk);

	if (nHint == PANORAMA_HINT_HORZ_SPLIT && ilk == ptd->plot && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_panorama_plot_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZENS);
	}
	else if (nHint == PANORAMA_HINT_VERT_SPLIT && ilk == ptd->plot && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_panorama_plot_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZEWE);
	}
	else if (nHint == PANORAMA_HINT_CROSS_SPLIT && ilk == ptd->plot && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_panorama_plot_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZEALL);
	}
	else if (nHint == PANORAMA_HINT_PLOT && ilk == ptd->plot && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_panorama_plot_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == PANORAMA_HINT_NONE)
	{
		if (ptd->opera == WD_OPERA_CONTROL)
			widget_set_cursor(widget, CURSOR_IBEAM);
		else
			widget_set_cursor(widget, CURSOR_ARROW);
	}

	if (widget_is_hotvoer(widget))
	{
		if (nHint == PANORAMA_HINT_PLOT && !ptd->hover && ilk)
		{
			noti_panorama_plot_enter(widget, ilk);
			return;
		}

		if (nHint == PANORAMA_HINT_PLOT && ptd->hover && ptd->hover != ilk)
		{
			noti_panorama_plot_leave(widget);
			return;
		}

		if (nHint != PANORAMA_HINT_PLOT && ptd->hover)
		{
			noti_panorama_plot_leave(widget);
		}
	}
}

void hand_panorama_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	if (ptd->hover)
		noti_panorama_plot_hover(widget, pxp->x, pxp->y);
}

void hand_panorama_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	if (ptd->hover)
		noti_panorama_plot_leave(widget);
}

void hand_panorama_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->panorama)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_panorama_hint(ptd->panorama, &pt, &ilk);
	bRe = (ilk == ptd->plot) ? 1 : 0;

	switch (nHint)
	{
	case PANORAMA_HINT_PLOT:
		if (ptd->opera == WD_OPERA_CONTROL || widget_key_state(widget, KEY_CONTROL))
		{
			noti_panorama_plot_selected(widget, ilk);
		}
		break;
	case PANORAMA_HINT_NONE:
		noti_panorama_reset_select(widget);
		break;
	}
}

void hand_panorama_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->panorama)
		return;

	if (ptd->b_size)
	{
		noti_panorama_plot_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag)
	{
		noti_panorama_plot_drop(widget, pxp->x, pxp->y);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_panorama_hint(ptd->panorama, &pt, &ilk);

	noti_panorama_owner(widget, NC_PANORAMALBCLK, ptd->panorama, ilk, (void*)pxp);

	bRe = (ilk == ptd->plot) ? 1 : 0;

	if (ptd->plot && !bRe)
	{
		if (!noti_panorama_plot_changing(widget))
			return;
	}

	if (ilk && !bRe)
	{
		noti_panorama_plot_changed(widget, ilk);
	}
}

void hand_panorama_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	noti_panorama_owner(widget, NC_PANORAMADBCLK, ptd->panorama, ptd->plot, (void*)pxp);
}

void hand_panorama_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;
}

void hand_panorama_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	noti_panorama_owner(widget, NC_PANORAMARBCLK, ptd->panorama, ptd->plot, (void*)pxp);
}

void hand_panorama_keydown(res_win_t widget, int nKey)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	float x, y, w, h, m;
	bool_t ks;
	link_t_ptr ilk;

	if (!ptd->panorama)
		return;

	if (!ptd->plot)
		return;

	if ((nKey == KEY_UP || nKey == KEY_DOWN || nKey == KEY_LEFT || nKey == KEY_RIGHT))
	{
		ks = widget_key_state(widget, KEY_SHIFT);
		m = 1;

		if (ks)
			noti_panorama_owner(widget, NC_PANORAMAPLOTSIZING, ptd->panorama, ptd->plot, NULL);
		else
			noti_panorama_owner(widget, NC_PANORAMAPLOTDRAG, ptd->panorama, ptd->plot, NULL);

		_panoramactrl_done(widget);

		ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
		while (ilk)
		{
			if (ilk != ptd->plot && !get_panorama_plot_selected(ilk))
			{
				ilk = get_panorama_next_plot(ptd->panorama, ilk);
				continue;
			}

			x = get_panorama_plot_x(ilk);
			y = get_panorama_plot_y(ilk);
			w = get_panorama_plot_width(ilk);
			h = get_panorama_plot_height(ilk);

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

			set_panorama_plot_x(ilk, x);
			set_panorama_plot_y(ilk, y);
			set_panorama_plot_width(ilk, w);
			set_panorama_plot_height(ilk, h);

			ilk = get_panorama_next_plot(ptd->panorama, ilk);
		}

		widget_redraw(widget, NULL, 0);

		if (ks)
			noti_panorama_owner(widget, NC_PANORAMAPLOTSIZED, ptd->panorama, ptd->plot, NULL);
		else
			noti_panorama_owner(widget, NC_PANORAMAPLOTDROP, ptd->panorama, ptd->plot, NULL);
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

void hand_panorama_char(res_win_t widget, tchar_t nChar)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;
}

void hand_panorama_copy(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	_panoramactrl_copy(widget);
}

void hand_panorama_cut(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	_panoramactrl_done(widget);

	if (!_panoramactrl_cut(widget))
	{
		_panoramactrl_discard(widget);
	}
}

void hand_panorama_paste(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	_panoramactrl_done(widget);

	if (!_panoramactrl_paste(widget))
	{
		_panoramactrl_discard(widget);
	}
}

void hand_panorama_undo(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;

	_panoramactrl_undo(widget);
}

void hand_panorama_notice(res_win_t widget, NOTICE* pnt)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;
}


void hand_panorama_erase(res_win_t widget, res_ctx_t dc)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	if (!ptd->panorama)
		return;
}

void hand_panorama_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
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

	if (!ptd->panorama)
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

	draw_panorama(pif, &cb, ptd->panorama);

	//draw focus
	if (ptd->plot)
	{
		_panoramactrl_plot_rect(widget, ptd->plot, &xr);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);

		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	//draw check
	parse_xcolor(&xc, DEF_ALPHA_COLOR);

	ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
	while (ilk)
	{
		if (get_panorama_plot_selected(ilk))
		{
			_panoramactrl_plot_rect(widget, ilk, &xr);
			alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
		}
		ilk = get_panorama_next_plot(ptd->panorama, ilk);
	}

	if (ptd->b_drag)
	{
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		_panoramactrl_plot_rect(widget, ptd->plot, &xr);

		xr.x += (ptd->cur_x - ptd->org_x);
		xr.y += (ptd->cur_y - ptd->org_y);

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}
	else if (ptd->b_size)
	{
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		_panoramactrl_plot_rect(widget, ptd->plot, &xr);

		if (ptd->org_hint == PANORAMA_HINT_VERT_SPLIT)
		{
			xr.w = (ptd->cur_x - xr.x);
		}
		else if (ptd->org_hint == PANORAMA_HINT_HORZ_SPLIT)
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

res_win_t panoramactrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_panorama_create)
		EVENT_ON_DESTROY(hand_panorama_destroy)

		EVENT_ON_ERASE(hand_panorama_erase)
		EVENT_ON_PAINT(hand_panorama_paint)

		EVENT_ON_SIZE(hand_panorama_size)

		EVENT_ON_SCROLL(hand_panorama_scroll)
		EVENT_ON_WHEEL(hand_panorama_wheel)

		EVENT_ON_KEYDOWN(hand_panorama_keydown)
		EVENT_ON_CHAR(hand_panorama_char)

		EVENT_ON_MOUSE_MOVE(hand_panorama_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_panorama_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_panorama_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_panorama_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_panorama_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_panorama_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_panorama_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_panorama_rbutton_up)

		EVENT_ON_NOTICE(hand_panorama_notice)

		EVENT_ON_COPY(hand_panorama_copy)
		EVENT_ON_CUT(hand_panorama_cut)
		EVENT_ON_PASTE(hand_panorama_paste)
		EVENT_ON_UNDO(hand_panorama_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void panoramactrl_attach(res_win_t widget, link_t_ptr ptr)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_panorama_doc(ptr));

	ptd->panorama = ptr;
	ptd->plot = NULL;

	panoramactrl_redraw(widget);
}

link_t_ptr panoramactrl_detach(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->panorama;
	ptd->panorama = NULL;
	ptd->plot = NULL;

	widget_redraw(widget, NULL, 0);

	return data;
}

link_t_ptr panoramactrl_fetch(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->panorama;
}

void panoramactrl_redraw(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->panorama)
		return;

	b_valid = 0;
	ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->plot)
			b_valid = 1;

		noti_panorama_owner(widget, NC_PANORAMAPLOTCALCED, ptd->panorama, ilk, NULL);

		ilk = get_panorama_next_plot(ptd->panorama, ilk);
	}
	
	noti_panorama_owner(widget, NC_PANORAMACALCED, ptd->panorama, NULL, NULL);

	if (!b_valid)
	{
		ptd->plot = NULL;
	}
	ptd->hover = NULL;

	_panoramactrl_reset_page(widget);

	widget_update(widget);
}

void panoramactrl_redraw_plot(res_win_t widget, link_t_ptr ilk)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->panorama)
		return;

#ifdef _DEBUG
	if(!is_panorama_plot(ptd->panorama, ilk))
		return;
#endif

	noti_panorama_owner(widget, NC_PANORAMAPLOTCALCED, ptd->panorama, ilk, NULL);

	_panoramactrl_plot_rect(widget, ilk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

void panoramactrl_tabskip(res_win_t widget, int nSkip)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	link_t_ptr ilk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->panorama)
		return;

	switch (nSkip)
	{
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (ptd->plot == NULL)
			ilk = get_panorama_prev_plot(ptd->panorama, LINK_LAST);
		else
			ilk = get_panorama_prev_plot(ptd->panorama, ptd->plot);
		break;
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (ptd->plot == NULL)
			ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
		else
			ilk = get_panorama_next_plot(ptd->panorama, ptd->plot);
		break;
	case WD_TAB_HOME:
		ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
		break;
	case WD_TAB_END:
		ilk = get_panorama_prev_plot(ptd->panorama, LINK_LAST);
		break;
	}

	panoramactrl_set_focus_plot(widget, ilk);
}

bool_t panoramactrl_set_focus_plot(res_win_t widget, link_t_ptr ilk)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->panorama)
		return 0;

	if (ilk == LINK_FIRST)
		ilk = get_panorama_next_plot(ptd->panorama, LINK_FIRST);
	else if (ilk == LINK_LAST)
		ilk = get_panorama_prev_plot(ptd->panorama, LINK_LAST);

	bRe = (ilk == ptd->plot) ? (bool_t)1 : (bool_t)0;
	if (bRe)
		return (bool_t)1;

	if (ptd->plot && !bRe)
	{
		if (!noti_panorama_plot_changing(widget))
			return (bool_t)0;
	}

	if (ilk && !bRe)
	{
		noti_panorama_plot_changed(widget, ilk);

		_panoramactrl_ensure_visible(widget);
	}

	return (bool_t)1;
}

link_t_ptr panoramactrl_get_focus_plot(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->panorama)
		return NULL;

	return ptd->plot;
}

void panoramactrl_set_opera(res_win_t widget, int opera)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->opera = opera;
}

int panoramactrl_get_opera(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->opera;
}

void panoramactrl_get_panorama_plot_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->panorama)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_panorama_plot(ptd->panorama, ilk));
#endif

	_panoramactrl_plot_rect(widget, ilk, pxr);
}

bool_t panoramactrl_get_dirty(res_win_t widget)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->panorama)
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void panoramactrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	panorama_delta_t* ptd = GETPANORAMADELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->panorama)
		return;

	if (bDirty)
		_panoramactrl_done(widget);
	else
		_panoramactrl_clean(widget);
}
