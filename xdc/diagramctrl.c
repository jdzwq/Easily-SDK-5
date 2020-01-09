/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc diagram control document

	@module	diagramctrl.c | implement file

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
#include "widgetnc.h"
#include "widgetex.h"
#include "xdcbox.h"

#define DIAGRAM_LINE_FEED		(float)50
#define DIAGRAM_ENTITY_MIN_WIDTH	(float)10
#define DIAGRAM_ENTITY_MIN_HEIGHT	(float)10

typedef struct _diagram_delta_t{
	link_t_ptr diagram;
	link_t_ptr entity;
	link_t_ptr hover;

	int org_hint;
	long org_x, org_y;
	long cur_x, cur_y;
	short cur_page;

	int opera;

	bool_t b_drag;
	bool_t b_size;

	res_win_t hsc;
	res_win_t vsc;

	link_t_ptr stack;
}diagram_delta_t;

#define GETDIAGRAMDELTA(ph) 	(diagram_delta_t*)widget_get_user_delta(ph)
#define SETDIAGRAMDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/******************************************diagram event********************************************************/
static void _diagramctrl_done(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->diagram);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->diagram, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->diagram, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->diagram, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->diagram, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _diagramctrl_undo(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->diagram);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_diagram_doc(ptd->diagram);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->diagram, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->diagram, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		diagramctrl_redraw(widget);
	}
}

static void _diagramctrl_discard(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _diagramctrl_clean(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _diagramctrl_copy(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->diagram);

	if (!get_diagram_entity_selected_count(ptd->diagram))
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_diagram_doc();
	ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
	while (ilk)
	{
		if (get_field_selected(ilk))
		{
			nlk = insert_diagram_entity(dom, get_diagram_entity_class_ptr(ilk));
			copy_dom_node(nlk, ilk);
		}

		ilk = get_diagram_next_entity(ptd->diagram, ilk);
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

		destroy_diagram_doc(dom);
		return 0;
	}

	clipboard_close();

	destroy_diagram_doc(dom);
	return 1;
}

static bool_t _diagramctrl_cut(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->diagram);

	if (!_diagramctrl_copy(widget))
		return 0;

	ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
	while (ilk)
	{
		nxt = get_diagram_next_entity(ptd->diagram, ilk);

		if (get_diagram_entity_selected(ilk))
		{
			delete_diagram_entity(ilk);
		}
		ilk = nxt;
	}

	diagramctrl_redraw(widget);

	return 1;
}

static bool_t _diagramctrl_paste(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk;

	float y;
	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->diagram);

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

	if (!is_diagram_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	while (nlk = get_diagram_next_entity(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_diagram_entityset(dom), nlk);
		attach_dom_node(get_diagram_entityset(ptd->diagram), LINK_LAST, nlk);

		y = get_diagram_entity_y(nlk);
		y += get_diagram_entity_height(nlk);
		set_diagram_entity_y(nlk, y);

		xsprintf(sz_name, _T("%s%d"), get_diagram_entity_class_ptr(nlk), get_diagram_entity_count_by_class(ptd->diagram, get_diagram_entity_class_ptr(nlk)));
		set_diagram_entity_name(nlk, sz_name);
	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);
	diagramctrl_redraw(widget);

	return 1;
}

static void _diagramctrl_entity_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	calc_diagram_entity_rect(ptd->diagram, ilk, pxr);

	widgetex_rect_to_pt(widget, pxr);
}

static void _diagramctrl_reset_page(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_diagram_width(ptd->diagram);
	xs.fy = get_diagram_height(ptd->diagram);

	widgetex_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = (float)10;
	xs.fy = (float)10;
	widgetex_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widgetex_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

static void _diagramctrl_ensure_visible(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	xrect_t xr = { 0 };

	if (!ptd->entity)
		return;

	_diagramctrl_entity_rect(widget, ptd->entity, &xr);

	widgetex_ensure_visible(widget, &xr, 1);
}
/*********************************************************************************************************/
int noti_diagram_owner(res_win_t widget, unsigned long code, link_t_ptr ptr, link_t_ptr ilk, void* data)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	NOTICE_DIAGRAM nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;

	nf.diagram = ptr;
	nf.entity = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_diagram_reset_select(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	link_t_ptr ilk;
	int count = 0;

	ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
	while (ilk)
	{
		if (get_diagram_entity_selected(ilk))
		{
			set_diagram_entity_selected(ilk, 0);
			noti_diagram_owner(widget, NC_DIAGRAMENTITYSELECTED, ptd->diagram, ilk, NULL);

			count++;
		}

		ilk = get_diagram_next_entity(ptd->diagram, ilk);
	}

	if (count)
	{
		widget_redraw(widget, NULL, 0);
	}
}

void noti_diagram_entity_selected(res_win_t widget, link_t_ptr ilk)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	xrect_t xr;
	bool_t b_check;

	b_check = get_diagram_entity_selected(ilk);

	if (b_check)
		set_diagram_entity_selected(ilk, 0);
	else
		set_diagram_entity_selected(ilk, 1);

	noti_diagram_owner(widget, NC_DIAGRAMENTITYSELECTED, ptd->diagram, ilk, NULL);

	_diagramctrl_entity_rect(widget, ilk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

bool_t noti_diagram_entity_changing(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->entity);

	if (noti_diagram_owner(widget, NC_DIAGRAMENTITYCHANGING, ptd->diagram, ptd->entity, NULL))
		return (bool_t)0;

	_diagramctrl_entity_rect(widget, ptd->entity, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->entity = NULL;

	widget_redraw(widget, &xr, 0);

	return (bool_t)1;
}

void noti_diagram_entity_changed(res_win_t widget, link_t_ptr ilk)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->entity);

	ptd->entity = ilk;

	_diagramctrl_entity_rect(widget, ptd->entity, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	noti_diagram_owner(widget, NC_DIAGRAMENTITYCHANGED, ptd->diagram, ilk, NULL);
}

void noti_diagram_entity_enter(res_win_t widget, link_t_ptr ilk)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ilk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = ilk;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
}

void noti_diagram_entity_leave(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
}

void noti_diagram_entity_hover(res_win_t widget, long x, long y)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->hover != NULL);

	pt.x = x;
	pt.y = y;
	noti_diagram_owner(widget, NC_DIAGRAMENTITYHOVER, ptd->diagram, ptd->hover, (void*)&pt);
}

void noti_diagram_entity_drag(res_win_t widget, long x, long y)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->entity);

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
	noti_diagram_owner(widget, NC_DIAGRAMENTITYDRAG, ptd->diagram, ptd->entity, (void*)&pt);
}

void noti_diagram_entity_drop(res_win_t widget, long x, long y)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	
	xpoint_t pt;
	xrect_t xr;
	long cx, cy;

	XDL_ASSERT(ptd->entity);

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

	calc_diagram_entity_rect(ptd->diagram, ptd->entity, &xr);

	widgetex_rect_to_pt(widget, &xr);

	if (xr.x + cx < 0 || xr.y + cy < 0)
		return;

	_diagramctrl_done(widget);

	pt.x = xr.x + cx;
	pt.y = xr.y + cy;

	widgetex_point_to_tm(widget, &pt);

	pt.fx = (float)((long)(pt.fx));
	pt.fy = (float)((long)(pt.fy));

	set_diagram_entity_x(ptd->entity, pt.fx);
	set_diagram_entity_y(ptd->entity, pt.fy);

	widget_redraw(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_diagram_owner(widget, NC_DIAGRAMENTITYDROP, ptd->diagram, ptd->entity, (void*)&pt);
}

void noti_diagram_entity_sizing(res_win_t widget, int hint, long x, long y)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->entity);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	if (hint == DIAGRAM_HINT_HORZ_SPLIT)
	{
		widget_set_cursor(widget,CURSOR_SIZENS);
	}
	else if (hint == DIAGRAM_HINT_VERT_SPLIT)
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

	_diagramctrl_entity_rect(widget, ptd->entity, &xr);

	noti_diagram_owner(widget, NC_DIAGRAMENTITYSIZING, ptd->diagram, ptd->entity, (void*)&xr);
}

void noti_diagram_entity_sized(res_win_t widget, long x, long y)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	float minw, minh, fw, fh;
	int hint;
	xrect_t xr;
	xsize_t xs;

	XDL_ASSERT(ptd->entity);

	ptd->cur_x = x;
	ptd->cur_y = y;

	ptd->b_size = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	hint = ptd->org_hint;

	minw = DIAGRAM_ENTITY_MIN_WIDTH;
	minh = DIAGRAM_ENTITY_MIN_HEIGHT;

	xs.cx = ptd->cur_x - ptd->org_x;
	xs.cy = ptd->cur_y - ptd->org_y;

	if (!xs.cx && !xs.cy)
		return;

	widgetex_size_to_tm(widget, &xs);

	_diagramctrl_entity_rect(widget, ptd->entity, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	fw = get_diagram_entity_width(ptd->entity);
	fh = get_diagram_entity_height(ptd->entity);

	fw += xs.fx;
	fh += xs.fy;

	if (fw < minw)
		fw = minw;

	if (fh < minh)
		fh = minh;

	fw = (float)((long)fw);
	fh = (float)((long)fh);

	_diagramctrl_done(widget);

	if (hint == DIAGRAM_HINT_HORZ_SPLIT)
	{
		set_diagram_entity_height(ptd->entity, fh);
	}
	else if (hint == DIAGRAM_HINT_VERT_SPLIT)
	{
		set_diagram_entity_width(ptd->entity, fw);
	}
	else
	{
		set_diagram_entity_width(ptd->entity, fw);
		set_diagram_entity_height(ptd->entity, fh);
	}

	_diagramctrl_entity_rect(widget, ptd->entity, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	_diagramctrl_entity_rect(widget, ptd->entity, &xr);

	noti_diagram_owner(widget, NC_DIAGRAMENTITYSIZED, ptd->diagram, ptd->entity, (void*)&xr);
}

void noti_diagram_calc(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
}

/*******************************************************************************/
int hand_diagram_create(res_win_t widget, void* data)
{
	diagram_delta_t* ptd;

	widgetex_hand_create(widget);

	ptd = (diagram_delta_t*)xmem_alloc(sizeof(diagram_delta_t));
	xmem_zero((void*)ptd, sizeof(diagram_delta_t));

	ptd->stack = create_stack_table();

	SETDIAGRAMDELTA(widget, ptd);

	return 0;
}

void hand_diagram_destroy(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	_diagramctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETDIAGRAMDELTA(widget, 0);

	widgetex_hand_destroy(widget);
}

void hand_diagram_size(res_win_t widget, int code, const xsize_t* prs)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	diagramctrl_redraw(widget);
}

void hand_diagram_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	widgetex_hand_scroll(widget, bHorz, nLine);
}

void hand_diagram_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->diagram)
		return;

	widget_get_scroll(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widgetex_hand_scroll(widget, bHorz, nLine))
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

void hand_diagram_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	xpoint_t pt;

	if (!ptd->diagram)
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
	widgetex_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_diagram_hint(ptd->diagram, &pt, &ilk);

	if (nHint == DIAGRAM_HINT_HORZ_SPLIT && ilk == ptd->entity && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_diagram_entity_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZENS);
	}
	else if (nHint == DIAGRAM_HINT_VERT_SPLIT && ilk == ptd->entity && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_diagram_entity_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZEWE);
	}
	else if (nHint == DIAGRAM_HINT_CROSS_SPLIT && ilk == ptd->entity && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_diagram_entity_sizing(widget, nHint, pxp->x, pxp->y);
			return;
		}
		else
			widget_set_cursor(widget, CURSOR_SIZEALL);
	}
	else if (nHint == DIAGRAM_HINT_ENTITY && ilk == ptd->entity && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_diagram_entity_drag(widget, pxp->x, pxp->y);
			return;
		}
	}
	else if (nHint == DIAGRAM_HINT_NONE)
	{
		if (ptd->opera == WD_OPERA_CONTROL)
			widget_set_cursor(widget, CURSOR_IBEAM);
		else
			widget_set_cursor(widget, CURSOR_ARROW);
	}

	if (widget_is_hotvoer(widget))
	{
		if (nHint == DIAGRAM_HINT_ENTITY && !ptd->hover && ilk)
		{
			noti_diagram_entity_enter(widget, ilk);
			return;
		}

		if (nHint == DIAGRAM_HINT_ENTITY && ptd->hover && ptd->hover != ilk)
		{
			noti_diagram_entity_leave(widget);
			return;
		}

		if (nHint != DIAGRAM_HINT_ENTITY && ptd->hover)
		{
			noti_diagram_entity_leave(widget);
		}
	}
}

void hand_diagram_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	if (ptd->hover)
		noti_diagram_entity_hover(widget, pxp->x, pxp->y);
}

void hand_diagram_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	if (ptd->hover)
		noti_diagram_entity_leave(widget);
}

void hand_diagram_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->diagram)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widgetex_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_diagram_hint(ptd->diagram, &pt, &ilk);
	bRe = (ilk == ptd->entity) ? 1 : 0;

	switch (nHint)
	{
	case DIAGRAM_HINT_ENTITY:
		if (ptd->opera == WD_OPERA_CONTROL || widget_key_state(widget, KEY_CONTROL))
		{
			noti_diagram_entity_selected(widget, ilk);
		}
		break;
	case DIAGRAM_HINT_NONE:
		noti_diagram_reset_select(widget);
		break;
	}
}

void hand_diagram_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	int nHint;
	link_t_ptr ilk;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->diagram)
		return;

	if (ptd->b_size)
	{
		noti_diagram_entity_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag)
	{
		noti_diagram_entity_drop(widget, pxp->x, pxp->y);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widgetex_point_to_tm(widget, &pt);

	ilk = NULL;
	nHint = calc_diagram_hint(ptd->diagram, &pt, &ilk);

	noti_diagram_owner(widget, NC_DIAGRAMLBCLK, ptd->diagram, ilk, (void*)pxp);

	bRe = (ilk == ptd->entity) ? 1 : 0;

	if (ptd->entity && !bRe)
	{
		if (!noti_diagram_entity_changing(widget))
			return;
	}

	if (ilk && !bRe)
	{
		noti_diagram_entity_changed(widget, ilk);
	}
}

void hand_diagram_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	noti_diagram_owner(widget, NC_DIAGRAMDBCLK, ptd->diagram, ptd->entity, (void*)pxp);
}

void hand_diagram_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;
}

void hand_diagram_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	noti_diagram_owner(widget, NC_DIAGRAMRBCLK, ptd->diagram, ptd->entity, (void*)pxp);
}

void hand_diagram_keydown(res_win_t widget, int nKey)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	float x, y, w, h, m;
	bool_t ks;
	link_t_ptr ilk;

	if (!ptd->diagram)
		return;

	if (!ptd->entity)
		return;

	if ((nKey == KEY_UP || nKey == KEY_DOWN || nKey == KEY_LEFT || nKey == KEY_RIGHT))
	{
		ks = widget_key_state(widget, KEY_SHIFT);
		m = 1;

		if (ks)
			noti_diagram_owner(widget, NC_DIAGRAMENTITYSIZING, ptd->diagram, ptd->entity, NULL);
		else
			noti_diagram_owner(widget, NC_DIAGRAMENTITYDRAG, ptd->diagram, ptd->entity, NULL);

		_diagramctrl_done(widget);

		ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
		while (ilk)
		{
			if (ilk != ptd->entity && !get_diagram_entity_selected(ilk))
			{
				ilk = get_diagram_next_entity(ptd->diagram, ilk);
				continue;
			}

			x = get_diagram_entity_x(ilk);
			y = get_diagram_entity_y(ilk);
			w = get_diagram_entity_width(ilk);
			h = get_diagram_entity_height(ilk);

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

			set_diagram_entity_x(ilk, x);
			set_diagram_entity_y(ilk, y);
			set_diagram_entity_width(ilk, w);
			set_diagram_entity_height(ilk, h);

			ilk = get_diagram_next_entity(ptd->diagram, ilk);
		}

		widget_redraw(widget, NULL, 0);

		if (ks)
			noti_diagram_owner(widget, NC_DIAGRAMENTITYSIZED, ptd->diagram, ptd->entity, NULL);
		else
			noti_diagram_owner(widget, NC_DIAGRAMENTITYDROP, ptd->diagram, ptd->entity, NULL);
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

void hand_diagram_char(res_win_t widget, tchar_t nChar)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;
}

void hand_diagram_copy(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	_diagramctrl_copy(widget);
}

void hand_diagram_cut(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	_diagramctrl_done(widget);

	if (!_diagramctrl_cut(widget))
	{
		_diagramctrl_discard(widget);
	}
}

void hand_diagram_paste(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	_diagramctrl_done(widget);

	if (!_diagramctrl_paste(widget))
	{
		_diagramctrl_discard(widget);
	}
}

void hand_diagram_undo(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;

	_diagramctrl_undo(widget);
}

void hand_diagram_notice(res_win_t widget, NOTICE* pnt)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;
}


void hand_diagram_erase(res_win_t widget, res_ctx_t dc)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	if (!ptd->diagram)
		return;
}

void hand_diagram_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
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

	if (!ptd->diagram)
		return;

	widgetex_get_xfont(widget, &xf);
	widgetex_get_xbrush(widget, &xb);
	widgetex_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);

	parse_xcolor(&pif->clr_bkg, xb.color);
	parse_xcolor(&pif->clr_frg, xp.color);
	parse_xcolor(&pif->clr_txt, xf.color);
	widgetex_get_mask(widget, &pif->clr_msk);
	widgetex_get_iconic(widget, &pif->clr_ico);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	widgetex_get_xbrush(widget, &xb);

	widgetex_get_xpen(widget, &xp);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widgetex_get_canv_rect(widget, &cb);

	draw_diagram(pif, &cb, ptd->diagram);

	//draw focus
	if (ptd->entity)
	{
		_diagramctrl_entity_rect(widget, ptd->entity, &xr);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);

		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	//draw check
	parse_xcolor(&xc, DEF_ALPHA_COLOR);

	ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
	while (ilk)
	{
		if (get_diagram_entity_selected(ilk))
		{
			_diagramctrl_entity_rect(widget, ilk, &xr);
			alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
		}
		ilk = get_diagram_next_entity(ptd->diagram, ilk);
	}

	if (ptd->b_drag)
	{
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		_diagramctrl_entity_rect(widget, ptd->entity, &xr);

		xr.x += (ptd->cur_x - ptd->org_x);
		xr.y += (ptd->cur_y - ptd->org_y);

		draw_rect_raw(rdc, &xp, NULL, &xr);
	}
	else if (ptd->b_size)
	{
		xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

		_diagramctrl_entity_rect(widget, ptd->entity, &xr);

		if (ptd->org_hint == DIAGRAM_HINT_VERT_SPLIT)
		{
			xr.w = (ptd->cur_x - xr.x);
		}
		else if (ptd->org_hint == DIAGRAM_HINT_HORZ_SPLIT)
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

res_win_t diagramctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_diagram_create)
		EVENT_ON_DESTROY(hand_diagram_destroy)

		EVENT_ON_ERASE(hand_diagram_erase)
		EVENT_ON_PAINT(hand_diagram_paint)

		EVENT_ON_SIZE(hand_diagram_size)

		EVENT_ON_SCROLL(hand_diagram_scroll)
		EVENT_ON_WHEEL(hand_diagram_wheel)

		EVENT_ON_KEYDOWN(hand_diagram_keydown)
		EVENT_ON_CHAR(hand_diagram_char)

		EVENT_ON_MOUSE_MOVE(hand_diagram_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_diagram_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_diagram_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_diagram_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_diagram_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_diagram_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_diagram_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_diagram_rbutton_up)

		EVENT_ON_NOTICE(hand_diagram_notice)

		EVENT_ON_COPY(hand_diagram_copy)
		EVENT_ON_CUT(hand_diagram_cut)
		EVENT_ON_PASTE(hand_diagram_paste)
		EVENT_ON_UNDO(hand_diagram_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void diagramctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_diagram_doc(ptr));

	ptd->diagram = ptr;
	ptd->entity = NULL;

	diagramctrl_redraw(widget);
}

link_t_ptr diagramctrl_detach(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	data = ptd->diagram;
	ptd->diagram = NULL;
	ptd->entity = NULL;

	widget_redraw(widget, NULL, 0);

	return data;
}

link_t_ptr diagramctrl_fetch(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->diagram;
}

void diagramctrl_redraw(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->diagram)
		return;

	b_valid = 0;
	ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->entity)
			b_valid = 1;

		noti_diagram_owner(widget, NC_DIAGRAMENTITYCALCED, ptd->diagram, ilk, NULL);

		ilk = get_diagram_next_entity(ptd->diagram, ilk);
	}
	
	noti_diagram_owner(widget, NC_DIAGRAMCALCED, ptd->diagram, NULL, NULL);

	if (!b_valid)
	{
		ptd->entity = NULL;
	}
	ptd->hover = NULL;

	_diagramctrl_reset_page(widget);

	widget_update(widget);
}

void diagramctrl_redraw_entity(res_win_t widget, link_t_ptr ilk)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->diagram)
		return;

#ifdef _DEBUG
	if(!is_diagram_entity(ptd->diagram, ilk))
		return;
#endif

	noti_diagram_owner(widget, NC_DIAGRAMENTITYCALCED, ptd->diagram, ilk, NULL);

	_diagramctrl_entity_rect(widget, ilk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

void diagramctrl_tabskip(res_win_t widget, int nSkip)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	link_t_ptr ilk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->diagram)
		return;

	switch (nSkip)
	{
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (ptd->entity == NULL)
			ilk = get_diagram_prev_entity(ptd->diagram, LINK_LAST);
		else
			ilk = get_diagram_prev_entity(ptd->diagram, ptd->entity);
		break;
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (ptd->entity == NULL)
			ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
		else
			ilk = get_diagram_next_entity(ptd->diagram, ptd->entity);
		break;
	case WD_TAB_HOME:
		ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
		break;
	case WD_TAB_END:
		ilk = get_diagram_prev_entity(ptd->diagram, LINK_LAST);
		break;
	}

	diagramctrl_set_focus_entity(widget, ilk);
}

bool_t diagramctrl_set_focus_entity(res_win_t widget, link_t_ptr ilk)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->diagram)
		return 0;

	if (ilk == LINK_FIRST)
		ilk = get_diagram_next_entity(ptd->diagram, LINK_FIRST);
	else if (ilk == LINK_LAST)
		ilk = get_diagram_prev_entity(ptd->diagram, LINK_LAST);

	bRe = (ilk == ptd->entity) ? (bool_t)1 : (bool_t)0;
	if (bRe)
		return (bool_t)1;

	if (ptd->entity && !bRe)
	{
		if (!noti_diagram_entity_changing(widget))
			return (bool_t)0;
	}

	if (ilk && !bRe)
	{
		noti_diagram_entity_changed(widget, ilk);

		_diagramctrl_ensure_visible(widget);
	}

	return (bool_t)1;
}

link_t_ptr diagramctrl_get_focus_entity(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->diagram)
		return NULL;

	return ptd->entity;
}

void diagramctrl_set_opera(res_win_t widget, int opera)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->opera = opera;
}

int diagramctrl_get_opera(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->opera;
}

void diagramctrl_get_diagram_entity_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->diagram)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_diagram_entity(ptd->diagram, ilk));
#endif

	_diagramctrl_entity_rect(widget, ilk, pxr);
}

bool_t diagramctrl_get_dirty(res_win_t widget)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->diagram)
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void diagramctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	diagram_delta_t* ptd = GETDIAGRAMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->diagram)
		return;

	if (bDirty)
		_diagramctrl_done(widget);
	else
		_diagramctrl_clean(widget);
}
