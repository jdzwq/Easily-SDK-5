/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc proper control document

	@module	properctrl.c | proper control implement file

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

typedef struct _proper_delta_t{
	link_t_ptr proper;
	link_t_ptr entity;
	link_t_ptr hover;

	res_win_t editor;
	tchar_t pch[CHS_LEN + 1];

	long org_x, org_y;

	bool_t b_size;
	bool_t b_lock;
}proper_delta_t;

#define GETPROPERDELTA(ph) 	(proper_delta_t*)widget_get_user_delta(ph)
#define SETPROPERDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/*****************************************************************************************************/
static void _properctrl_section_rect(res_win_t widget, link_t_ptr sec, xrect_t* pxr)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_proper_section_rect( &cb, ptd->proper,sec, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _properctrl_entity_rect(res_win_t widget, link_t_ptr ent, xrect_t* pxr)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_proper_entity_rect(&cb, ptd->proper, ent, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _properctrl_entity_text_rect(res_win_t widget, link_t_ptr ent, xrect_t* pxr)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_proper_entity_text_rect(&cb, ptd->proper, ent, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _properctrl_reset_page(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;
	canvbox_t cb;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	widget_get_canv_rect(widget, &cb);

	xs.fx = calc_proper_width(&cb, ptd->proper);
	xs.fy = calc_proper_height(&cb, ptd->proper);

	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	if (fw < pw)
		fw = pw;
	fh = xs.cy;

	xs.fx = get_proper_item_height(ptd->proper);
	xs.fy = get_proper_item_height(ptd->proper);
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

static void _properctrl_ensure_visible(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	xrect_t xr = { 0 };

	if (!ptd->entity)
		return;

	_properctrl_entity_rect(widget, ptd->entity, &xr);

	widget_ensure_visible(widget, &xr, 1);
}

/**********************************************************************************************************/

int noti_proper_owner(res_win_t widget, unsigned long code, link_t_ptr proper, link_t_ptr slk, link_t_ptr elk, void* data)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	NOTICE_PROPER nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.proper = proper;
	nf.section = slk;
	nf.entity = elk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);

	return nf.ret;
}

void noti_proper_begin_size(res_win_t widget, long x, long y)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	ptd->org_x = x;
	ptd->org_y = y;
	ptd->b_size = 1;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_SIZEWE);
}

void noti_proper_end_size(res_win_t widget, long x, long y)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	float pw, iw, ew;
	canvbox_t vb;
	xsize_t xs = { 0 };

	ptd->b_size = 0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	widget_get_canv_rect(widget, &vb);

	pw = vb.fw;
	ew = get_proper_item_span(ptd->proper);
	iw = get_proper_icon_span(ptd->proper);

	xs.cx = x - ptd->org_x;
	widget_size_to_tm(widget, &xs);

	ew += xs.fx;
	if (ew < iw)
		ew = iw;
	else if (ew > pw)
		ew = pw;

	set_proper_item_span(ptd->proper, ew);

	widget_update(widget, NULL, 0);
}

bool_t noti_proper_entity_changing(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->entity);

	if (noti_proper_owner(widget, NC_ENTITYCHANGING, ptd->proper, section_from_entity(ptd->entity), ptd->entity, NULL))
		return 0;

	_properctrl_entity_rect(widget, ptd->entity, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->entity = NULL;

	widget_update(widget, &xr, 0);

	return 1;
}

void noti_proper_entity_changed(res_win_t widget, link_t_ptr elk)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->entity);

	ptd->entity = elk;

	_properctrl_entity_rect(widget, ptd->entity, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	noti_proper_owner(widget, NC_ENTITYCHANGED, ptd->proper, section_from_entity(ptd->entity), ptd->entity, NULL);
}

void noti_proper_entity_enter(res_win_t widget, link_t_ptr plk)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(plk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = plk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_proper_entity_leave(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_proper_entity_hover(res_win_t widget, long x, long y)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_proper_owner(widget, NC_ENTITYHOVER, ptd->proper, NULL, ptd->hover, (void*)&xp);
}

void noti_proper_section_expand(res_win_t widget, link_t_ptr slk)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	xrect_t xr_sec, xr;

	if (get_section_expanded(slk))
	{
		set_section_expanded(slk, 0);
	}
	else
	{
		set_section_expanded(slk, 1);
	}

	_properctrl_section_rect(widget, slk, &xr_sec);
	widget_get_client_rect(widget, &xr);

	pt_inter_rect(&xr, &xr_sec);
	widget_update(widget, &xr, 0);
}

void noti_proper_begin_edit(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	const tchar_t* editor;
	const tchar_t* text;
	xrect_t xr = { 0 };

	link_t_ptr data;
	EDITDELTA fd = { 0 };

	clr_mod_t ob = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd->entity);

	if (widget_is_valid(ptd->editor))
		return;

	if (ptd->b_lock)
		return;

	if (!get_entity_editable(ptd->entity))
		return;

	widget_get_xfont(widget, &xf);
	parse_xfont_from_style(&xf, get_proper_style_ptr(ptd->proper));
	widget_get_color_mode(widget, &ob);

	_properctrl_entity_text_rect(widget, ptd->entity, &xr);
	pt_expand_rect(&xr, -1, -1);

	editor = get_entity_editor_ptr(ptd->entity);

	if (compare_text(editor, -1, ATTR_EDITOR_FIREEDIT, -1, 0) == 0)
	{
		if (noti_proper_owner(widget, NC_ENTITYEDITING, ptd->proper, section_from_entity(ptd->entity), ptd->entity, NULL))
			return;

		ptd->editor = fireedit_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREEDIT);
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
			text = get_entity_value_ptr(ptd->entity);
			editbox_set_text(ptd->editor, text);
			editbox_selectall(ptd->editor);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRENUM, -1, 0) == 0)
	{
		if (noti_proper_owner(widget, NC_ENTITYEDITING, ptd->proper, section_from_entity(ptd->entity), ptd->entity, NULL))
			return;

		ptd->editor = firenum_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRENUM);
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
			text = get_entity_value_ptr(ptd->entity);
			editbox_set_text(ptd->editor, text);
			editbox_selectall(ptd->editor);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREDATE, -1, 0) == 0)
	{
		if (noti_proper_owner(widget, NC_ENTITYEDITING, ptd->proper, section_from_entity(ptd->entity), ptd->entity, NULL))
			return;

		ptd->editor = firedate_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREDATE);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_entity_value_ptr(ptd->entity);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRETIME, -1, 0) == 0)
	{
		if (noti_proper_owner(widget, NC_ENTITYEDITING, ptd->proper, section_from_entity(ptd->entity), ptd->entity, NULL))
			return;

		ptd->editor = firetime_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRETIME);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);
		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_entity_value_ptr(ptd->entity);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRELIST, -1, 0) == 0)
	{
		if (noti_proper_owner(widget, NC_ENTITYEDITING, ptd->proper, section_from_entity(ptd->entity), ptd->entity, NULL))
			return;

		data = get_entity_options_table(ptd->entity);
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

		text = get_entity_value_ptr(ptd->entity);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREWORDS, -1, 0) == 0)
	{
		if (noti_proper_owner(widget, NC_ENTITYEDITING, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)&fd))
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

		text = get_entity_value_ptr(ptd->entity);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREGRID, -1, 0) == 0)
	{
		if (noti_proper_owner(widget, NC_ENTITYEDITING, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)&fd))
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

void noti_proper_commit_edit(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	const tchar_t* editor;
	const tchar_t* text;
	res_win_t editctrl;
	link_t_ptr item;

	EDITDELTA fd = { 0 };

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->entity);

	editor = get_entity_editor_ptr(ptd->entity);

	if (compare_text(editor, -1, ATTR_EDITOR_FIREEDIT, -1, 0) == 0)
	{
		text = (const tchar_t*)editbox_get_text_ptr(ptd->editor);

		if (!noti_proper_owner(widget, NC_ENTITYCOMMIT, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)text))
		{
			properctrl_set_entity_value(widget, ptd->entity, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRENUM, -1, 0) == 0)
	{
		text = (const tchar_t*)editbox_get_text_ptr(ptd->editor);
		
		if (!noti_proper_owner(widget, NC_ENTITYCOMMIT, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)text))
		{
			properctrl_set_entity_value(widget, ptd->entity, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREDATE, -1, 0) == 0)
	{
		text = (const tchar_t*)editbox_get_text_ptr(ptd->editor);

		if (!noti_proper_owner(widget, NC_ENTITYCOMMIT, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)text))
		{
			properctrl_set_entity_value(widget, ptd->entity, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRETIME, -1, 0) == 0)
	{
		text = (const tchar_t*)editbox_get_text_ptr(ptd->editor);

		if (!noti_proper_owner(widget, NC_ENTITYCOMMIT, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)text))
		{
			properctrl_set_entity_value(widget, ptd->entity, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRELIST, -1, 0) == 0)
	{
		text = editbox_get_text_ptr(ptd->editor);

		if (!noti_proper_owner(widget, NC_ENTITYCOMMIT, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)text))
		{
			properctrl_set_entity_value(widget, ptd->entity, text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREWORDS, -1, 0) == 0)
	{
		fd.data = firewords_get_data(ptd->editor);
		item = firewords_get_item(ptd->editor);
		if (item)
		{
			editbox_set_text(ptd->editor, get_words_item_text_ptr(item));
		}

		fd.text = editbox_get_text_ptr(ptd->editor);

		if (!noti_proper_owner(widget, NC_ENTITYCOMMIT, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)&fd))
		{
			properctrl_set_entity_value(widget, ptd->entity, fd.text);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREGRID, -1, 0) == 0)
	{
		fd.data = firegrid_get_data(ptd->editor);
		fd.item = (link_t_ptr)firegrid_get_item(ptd->editor);
		noti_proper_owner(widget, NC_ENTITYCOMMIT, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)&fd);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_proper_rollback_edit(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	res_win_t editctrl;
	const tchar_t* editor;

	EDITDELTA fd = { 0 };

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->entity);

	editor = get_entity_editor_ptr(ptd->entity);

	if (compare_text(editor, -1, ATTR_EDITOR_FIREWORDS, -1, 0) == 0)
	{
		fd.data = firewords_get_data(ptd->editor);
		fd.text = NULL;
		noti_proper_owner(widget, NC_ENTITYROLLBACK, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)&fd);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREGRID, -1, 0) == 0)
	{
		fd.data = firegrid_get_data(ptd->editor);
		fd.item = NULL;
		noti_proper_owner(widget, NC_ENTITYROLLBACK, ptd->proper, section_from_entity(ptd->entity), ptd->entity, (void*)&fd);
	}
	else
	{
		noti_proper_owner(widget, NC_ENTITYROLLBACK, ptd->proper, section_from_entity(ptd->entity), ptd->entity, NULL);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_proper_reset_editor(res_win_t widget, bool_t bCommit)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_proper_commit_edit(widget);
		else
			noti_proper_rollback_edit(widget);
	}
}

/************************************************************************************************/
int hand_proper_create(res_win_t widget, void* data)
{
	proper_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (proper_delta_t*)xmem_alloc(sizeof(proper_delta_t));

	SETPROPERDELTA(widget, ptd);

	return 0;
}

void hand_proper_destroy(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETPROPERDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_proper_size(res_win_t widget, int code, const xsize_t* prs)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;

	properctrl_redraw(widget);
}

void hand_proper_notice(res_win_t widget, NOTICE* pnt)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;
}

void hand_proper_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_proper_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd->proper)
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

void hand_proper_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	link_t_ptr slk, elk;
	int nHint;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->proper)
		return;

	if (ptd->b_size)
		return;

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	slk = elk = NULL;
	nHint = calc_proper_hint(&cb, &pt, ptd->proper, &slk, &elk);

	if (nHint == PROPER_HINT_VERT_SPLIT && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_proper_begin_size(widget, pxp->x, pxp->y);
			return;
		}else
			widget_set_cursor(widget, CURSOR_SIZEWE);
	}

	if (nHint == PROPER_HINT_ENTITY && !ptd->hover && elk)
	{
		noti_proper_entity_enter(widget, elk);
	}
	else if (nHint == PROPER_HINT_ENTITY && ptd->hover && ptd->hover != elk)
	{
		noti_proper_entity_leave(widget);
	}
	else if (nHint != PROPER_HINT_ENTITY && ptd->hover)
	{
		noti_proper_entity_leave(widget);
	}
}

void hand_proper_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;

	if (ptd->hover)
	{
		noti_proper_entity_hover(widget, pxp->x, pxp->y);
	}
}

void hand_proper_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;

	if (ptd->hover)
		noti_proper_entity_leave(widget);
}

void hand_proper_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return;

	noti_proper_owner(widget, NC_PROPERDBCLK, ptd->proper, ((ptd->entity) ? section_from_entity(ptd->entity) : NULL), ptd->entity, (void*)pxp);
}

void hand_proper_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	link_t_ptr slk, elk;
	int nHint;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->proper)
		return;

	noti_proper_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	slk = elk = NULL;
	nHint = calc_proper_hint(&cb, &pt, ptd->proper, &slk, &elk);

	if (nHint == PROPER_HINT_SECTION)
	{
		noti_proper_section_expand(widget, slk);
		return;
	}
}

void hand_proper_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	link_t_ptr slk, elk;
	xpoint_t pt;
	canvbox_t cb;
	int nHint;
	bool_t bRe;

	if (!ptd->proper)
		return;

	if (ptd->b_size)
	{
		noti_proper_end_size(widget, pxp->x, pxp->y);
		return;
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	slk = elk = NULL;
	nHint = calc_proper_hint(&cb, &pt, ptd->proper, &slk, &elk);

	bRe = (elk == ptd->entity) ? 1 : 0;

	if (bRe && !ptd->b_lock && ptd->entity && get_entity_editable(ptd->entity))
	{
		widget_post_key(widget, KEY_ENTER);
		return;
	}

	if (!bRe && ptd->entity)
	{
		if (!noti_proper_entity_changing(widget))
			bRe = 1;
	}

	if (!bRe && elk)
	{
		noti_proper_entity_changed(widget, elk);
	}

	noti_proper_owner(widget, NC_PROPERLBCLK, ptd->proper, ((ptd->entity) ? section_from_entity(ptd->entity) : NULL), ptd->entity, (void*)pxp);
}

void hand_proper_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;

	noti_proper_reset_editor(widget, 1);
}

void hand_proper_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;

	noti_proper_owner(widget, NC_PROPERRBCLK, ptd->proper, ((ptd->editor)? section_from_entity(ptd->entity) : NULL), ptd->entity, (void*)pxp);
}

void hand_proper_keydown(res_win_t widget, int nKey)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;

	switch (nKey)
	{
	case KEY_ENTER:
		if (ptd->entity && !widget_is_valid(ptd->editor))
		{
			noti_proper_begin_edit(widget);
		}
		break;
	case KEY_LEFT:
		properctrl_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		properctrl_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_UP:
		properctrl_tabskip(widget,WD_TAB_UP);
		break;
	case KEY_DOWN:
		properctrl_tabskip(widget,WD_TAB_DOWN);
		break;
	case KEY_END:
		properctrl_tabskip(widget,WD_TAB_END);
		break;
	case KEY_HOME:
		properctrl_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_PAGEUP:
		properctrl_tabskip(widget,WD_TAB_PAGEUP);
		break;
	case KEY_PAGEDOWN:
		properctrl_tabskip(widget,WD_TAB_PAGEDOWN);
		break;
	}
}

void hand_proper_char(res_win_t widget, tchar_t nChar)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;

	if (IS_VISIBLE_CHAR(nChar) && !widget_is_valid(ptd->editor))
	{
		ptd->pch[0] = nChar;
		hand_proper_keydown(widget, KEY_ENTER);
	}

	if (IS_VISIBLE_CHAR(nChar) && widget_is_valid(ptd->editor))
	{
		//widget_post_char(NULL, nChar);
	}

}

void hand_proper_child_command(res_win_t widget, int code, var_long data)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_proper_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_proper_rollback_edit(widget);
		break;
	}
}

void hand_proper_erase(res_win_t widget, res_ctx_t dc)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	if (!ptd->proper)
		return;
}

void hand_proper_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xrect_t xr = { 0 };
	xcolor_t xc = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->proper)
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

	draw_proper(pif, &cb, ptd->proper);

	//draw focus
	if (ptd->entity)
	{
		_properctrl_entity_rect(widget, ptd->entity, &xr);
		pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

		parse_xcolor(&xc, DEF_ALPHA_COLOR);
		alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/*************************************************************************************************/

res_win_t properctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_proper_create)
		EVENT_ON_DESTROY(hand_proper_destroy)

		EVENT_ON_ERASE(hand_proper_erase)
		EVENT_ON_PAINT(hand_proper_paint)

		EVENT_ON_SIZE(hand_proper_size)

		EVENT_ON_SCROLL(hand_proper_scroll)
		EVENT_ON_WHEEL(hand_proper_wheel)

		EVENT_ON_KEYDOWN(hand_proper_keydown)
		EVENT_ON_CHAR(hand_proper_char)

		EVENT_ON_MOUSE_MOVE(hand_proper_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_proper_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_proper_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_proper_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_proper_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_proper_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_proper_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_proper_rbutton_up)

		EVENT_ON_NOTICE(hand_proper_notice)
		EVENT_ON_CHILD_COMMAND(hand_proper_child_command)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void properctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_proper_doc(ptr));

	ptd->proper = ptr;
	properctrl_redraw(widget);
}

link_t_ptr properctrl_detach(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	if (ptd->proper == NULL)
		return NULL;

	noti_proper_reset_editor(widget, 0);

	data = ptd->proper;
	ptd->proper = NULL;

	widget_update(widget, NULL, 0);

	return data;
}

link_t_ptr properctrl_fetch(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->proper;
}

void properctrl_accept(res_win_t widget, bool_t bAccept)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return;

	noti_proper_reset_editor(widget, bAccept);
}

void properctrl_redraw(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	link_t_ptr sec, ent;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return;

	noti_proper_reset_editor(widget, 0);

	b_valid = 0;
	sec = get_next_section(ptd->proper, LINK_FIRST);
	while (sec)
	{
		noti_proper_owner(widget, NC_SECTIONCALCED, ptd->proper, sec, NULL, NULL);

		ent = get_next_entity(sec, LINK_FIRST);
		while (ent)
		{
			if (ent == ptd->entity)
				b_valid = 1;

			noti_proper_owner(widget, NC_ENTITYCALCED, ptd->proper, sec, ent, NULL);

			ent = get_next_entity(sec, ent);
		}

		sec = get_next_section(ptd->proper, sec);
	}

	noti_proper_owner(widget, NC_PROPERCALCED, ptd->proper, NULL, NULL, NULL);

	if (!b_valid)
	{
		ptd->entity = NULL;
	}
	ptd->hover = NULL;

	_properctrl_reset_page(widget);

	widget_update_window(widget);

	widget_update(widget, NULL, 0);
}

void properctrl_redraw_entity(res_win_t widget, link_t_ptr elk)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_proper_entity(ptd->proper, elk));
#endif

	noti_proper_owner(widget, NC_ENTITYCALCED, ptd->proper, section_from_entity(elk), elk, NULL);

	_properctrl_entity_rect(widget, ptd->entity, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

void properctrl_redraw_section(res_win_t widget, link_t_ptr slk)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	xrect_t xr;
	link_t_ptr ent;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_proper_section(ptd->proper, slk));
#endif

	ent = get_next_entity(slk, LINK_FIRST);
	while (ent)
	{
		noti_proper_owner(widget, NC_ENTITYCALCED, ptd->proper, slk, ent, NULL);
		ent = get_next_entity(slk, ent);
	}

	noti_proper_owner(widget, NC_SECTIONCALCED, ptd->proper, slk, NULL, NULL);

	_properctrl_section_rect(widget, slk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t properctrl_set_focus_entity(res_win_t widget, link_t_ptr elk)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return 0;

	if (elk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_proper_entity(ptd->proper, elk));
#endif
	}

	bRe = (elk == ptd->entity) ? 1 : 0;

	if (bRe)
		return 1;

	if (ptd->entity && !bRe)
	{
		if (!noti_proper_entity_changing(widget))
			return 0;
	}

	if (elk && !bRe)
	{
		noti_proper_entity_changed(widget, elk);

		_properctrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr properctrl_get_focus_entity(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return NULL;

	return ptd->entity;
}

void properctrl_tabskip(res_win_t widget, int dir)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	link_t_ptr slk, elk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return;

	switch (dir)
	{
	case WD_TAB_DOWN:
	case WD_TAB_RIGHT:
		if (ptd->entity)
		{
			slk = section_from_entity(ptd->entity);
			elk = get_next_entity(slk, ptd->entity);
			if (!elk)
			{
				slk = get_next_section(ptd->proper, slk);
				if (!slk)
					break;
				elk = get_next_entity(slk, LINK_FIRST);
				if (!elk)
					break;
			}

			if (!get_section_expanded(slk))
				noti_proper_section_expand(widget, slk);

			if (elk)
				properctrl_set_focus_entity(widget, elk);
		}
		else
		{
			slk = get_next_section(ptd->proper, LINK_FIRST);
			if (!slk)
				break;

			elk = get_next_entity(slk, LINK_FIRST);
			if (!elk)
				break;

			if (!get_section_expanded(slk))
				noti_proper_section_expand(widget, slk);

			if (elk)
				properctrl_set_focus_entity(widget, elk);
		}
		break;
	case WD_TAB_UP:
	case WD_TAB_LEFT:
		if (ptd->entity)
		{
			slk = section_from_entity(ptd->entity);
			elk = get_prev_entity(slk, ptd->entity);
			if (!elk)
			{
				slk = get_prev_section(ptd->proper, slk);
				if (!slk)
					break;
				elk = get_prev_entity(slk, LINK_LAST);
				if (!elk)
					break;
			}

			if (!get_section_expanded(slk))
				noti_proper_section_expand(widget, slk);

			if (elk)
				properctrl_set_focus_entity(widget, elk);
		}
		else
		{
			slk = get_prev_section(ptd->proper, LINK_LAST);
			if (!slk)
				break;

			elk = get_prev_entity(slk, LINK_LAST);
			if (!elk)
				break;

			if (!get_section_expanded(slk))
				noti_proper_section_expand(widget, slk);

			if (elk)
				properctrl_set_focus_entity(widget, elk);
		}
		break;
	}
}

bool_t	properctrl_set_entity_value(res_win_t widget, link_t_ptr elk, const tchar_t* token)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_proper_entity(ptd->proper, elk));
#endif

	if (compare_text(get_entity_value_ptr(elk), -1, token, -1, 0) == 0)
		return 0;

	set_entity_value(elk, token, -1);

	noti_proper_owner(widget, NC_ENTITYUPDATE, ptd->proper, section_from_entity(elk), elk, NULL);

	properctrl_redraw_entity(widget, elk);

	return 1;
}

void properctrl_get_entity_rect(res_win_t widget, link_t_ptr ent, xrect_t* pxr)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->proper)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_proper_entity(ptd->proper, ent));
#endif

	_properctrl_entity_rect(widget, ent, pxr);
}

void properctrl_set_lock(res_win_t widget, bool_t bLock)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t properctrl_get_lock(res_win_t widget)
{
	proper_delta_t* ptd = GETPROPERDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}
