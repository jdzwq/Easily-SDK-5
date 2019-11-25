/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc listbox control document

	@module	listbox.c | implement file

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

#include "xdcbox.h"
#include "handler.h"
#include "winnc.h"

typedef struct _listbox_delta_t{
	link_t_ptr string;
	link_t_ptr entity;
}listbox_delta_t;

#define GETLISTBOXDELTA(ph) 	(listbox_delta_t*)widget_get_user_delta(ph)
#define SETLISTBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/***************************************************************************************/
void _listbox_item_rect(res_win_t widget, link_t_ptr ent, xrect_t* pxr)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	xrect_t xr;
	xfont_t xf;
	if_measure_t im = { 0 };

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	widget_get_xfont(widget, &xf);

	calc_listbox_item_rect(&im, &xf, ptd->string, ent, pxr);
	widget_rect_to_pt(widget, pxr);

	widget_get_client_rect(widget, &xr);
	pxr->w = xr.w;
}

void _listbox_reset_page(res_win_t widget)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	long vw, vh, lw, lh;
	xrect_t xr;
	xsize_t xs;
	xfont_t xf;
	if_measure_t im = { 0 };

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	widget_get_xfont(widget, &xf);

	text_metric((canvas_t)im.ctx, &xf, &xs);
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	calc_listbox_size(&im, &xf, ptd->string, &xs);
	widget_size_to_pt(widget, &xs);
	vw = xs.cx;
	vh = xs.cy;

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, vw, vh, lw, lh);

	widget_reset_scroll(widget, 0);
}

void _listbox_reset_visible(res_win_t widget)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	xrect_t xr;
	
	if (!ptd->entity)
		return;

	_listbox_item_rect(widget, ptd->entity, &xr);

	widget_ensure_visible(widget, &xr, 1);
}

static link_t_ptr _listbox_get_next_entity(res_win_t widget, link_t_ptr pos)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	link_t_ptr ent;

	if (pos == LINK_LAST)
		return NULL;

	ent = get_string_next_entity(ptd->string, LINK_FIRST);
	while (ent)
	{
		if (get_string_entity_delta(ent))
		{
			ent = get_string_next_entity(ptd->string, ent);
			continue;
		}

		if (pos == LINK_FIRST)
			return ent;
		else if (pos == ent)
			pos = LINK_FIRST;

		ent = get_string_next_entity(ptd->string, ent);
	}

	return NULL;
}

static link_t_ptr _listbox_get_prev_entity(res_win_t widget, link_t_ptr pos)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	link_t_ptr ent;

	if (pos == LINK_LAST)
		return NULL;

	ent = get_string_prev_entity(ptd->string, LINK_LAST);
	while (ent)
	{
		if (get_string_entity_delta(ent))
		{
			ent = get_string_prev_entity(ptd->string, ent);
			continue;
		}

		if (pos == LINK_LAST)
			return ent;
		else if (pos == ent)
			pos = LINK_LAST;

		ent = get_string_prev_entity(ptd->string, ent);
	}

	return NULL;
}
/*************************************************************************/

void noti_listbox_command(res_win_t widget, int code, var_long data)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}

void listbox_on_item_changing(res_win_t widget)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->entity != NULL);

	_listbox_item_rect(widget, ptd->entity, &xr);

	ptd->entity = NULL;

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 1);
}

void listbox_on_item_changed(res_win_t widget, link_t_ptr ent)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->entity == NULL);

	ptd->entity = ent;

	_listbox_item_rect(widget, ptd->entity, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 1);

	noti_listbox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

/********************************************************************************************/
int hand_listbox_create(res_win_t widget, void* data)
{
	listbox_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (listbox_delta_t*)xmem_alloc(sizeof(listbox_delta_t));

	ptd->string = create_string_table(0);
	ptd->entity = NULL;

	SETLISTBOXDELTA(widget, ptd);

	return 0;
}

void hand_listbox_destroy(res_win_t widget)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	destroy_string_table(ptd->string);

	xmem_free(ptd);

	SETLISTBOXDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_listbox_keydown(res_win_t widget, int key)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	if (!ptd->string)
		return;

	switch (key)
	{
	case KEY_ENTER:
		noti_listbox_command(widget, COMMAND_CHANGE, (var_long)NULL);
		break;
	case KEY_SPACE:
		noti_listbox_command(widget, COMMAND_CHANGE, (var_long)NULL);
		break;
	case KEY_LEFT:
		listbox_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		listbox_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_HOME:
		listbox_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		listbox_tabskip(widget,WD_TAB_END);
		break;
	}
}

void hand_listbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	if (!ptd)
		return;

}

void hand_listbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	xfont_t xf = { 0 };
	if_measure_t im = { 0 };
	link_t_ptr ilk = NULL;
	int hint;
	xpoint_t pt;

	if (!ptd->string)
		return;

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	widget_get_xfont(widget, &xf);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	hint = calc_listbox_hint(&im, &xf, &pt, ptd->string, &ilk);

	if (ilk != ptd->entity)
	{
		if (ptd->entity)
			listbox_on_item_changing(widget);

		if (ilk)
			listbox_on_item_changed(widget, ilk);
	}

	noti_listbox_command(widget, COMMAND_CHANGE, (var_long)NULL);
}

void hand_listbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	if (!ptd->string)
		return;

	listbox_redraw(widget);
}

void hand_listbox_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	if (!ptd->string)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_listbox_erase(res_win_t widget, res_ctx_t rdc)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	
	if (!ptd->string)
		return;
}

void hand_listbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	res_ctx_t rdc;
	xrect_t xr;
	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	xfont_t xf;
	xbrush_t xb;
	xpen_t xp;
	xcolor_t xc;

	if (!ptd->string)
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

	draw_listbox(pif, &cb, &xf, ptd->string);

	//draw focus
	if (ptd->entity)
	{
		_listbox_item_rect(widget, ptd->entity, &xr);

		parse_xcolor(&xc, DEF_ALPHA_COLOR);
		alphablend_rect_raw(rdc, &xc, &xr, ALPHA_SOFT);
	}

	end_canvas_paint(canv, dc, pxr);
}

/************************************************************************************************/
res_win_t listbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_listbox_create)
		EVENT_ON_DESTROY(hand_listbox_destroy)

		EVENT_ON_ERASE(hand_listbox_erase)
		EVENT_ON_PAINT(hand_listbox_paint)

		EVENT_ON_SIZE(hand_listbox_size)

		EVENT_ON_SCROLL(hand_listbox_scroll)

		EVENT_ON_KEYDOWN(hand_listbox_keydown)

		EVENT_ON_LBUTTON_DOWN(hand_listbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_listbox_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL,style, pxr, widget, &ev);
}

void listbox_set_options(res_win_t widget, const tchar_t* options, int len)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	clear_string_table(ptd->string);
	string_table_parse_options(ptd->string, options, len, OPT_ITEMFEED, OPT_LINEFEED);

	listbox_redraw(widget);
}

const tchar_t* listbox_get_cur_key(res_win_t widget)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->entity)
		return NULL;

	return get_string_entity_key_ptr(ptd->entity);
}

int listbox_get_cur_val(res_win_t widget, tchar_t* val, int max)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->entity)
		return 0;

	return get_string_entity_val(ptd->entity, val, max);
}

void listbox_redraw(res_win_t widget)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	link_t_ptr ent;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

	ent = get_string_next_entity(ptd->string, LINK_FIRST);
	while (ent)
	{
		if (get_string_entity_delta(ent))
		{
			ent = get_string_next_entity(ptd->string, ent);
			continue;
		}

		if (ent == ptd->entity)
			break;

		ent = get_string_next_entity(ptd->string, ent);
	}

	ptd->entity = ent;
	_listbox_reset_page(widget);

	widget_redraw(widget, NULL, 1);
}

void listbox_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

	if (ilk != ptd->entity)
	{
		if (ptd->entity)
			listbox_on_item_changing(widget);

		if (ilk)
			listbox_on_item_changed(widget, ilk);

		_listbox_reset_visible(widget);
	}
}

void listbox_tabskip(res_win_t widget, int nSkip)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	link_t_ptr ilk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->string)
		return;

	ilk = (ptd->entity)? ptd->entity : NULL;

	switch (nSkip)
	{
	case WD_TAB_LEFT:
	case WD_TAB_DOWN:
		ilk = _listbox_get_next_entity(widget, ilk);
		break;
	case WD_TAB_RIGHT:
	case WD_TAB_UP:
		ilk = _listbox_get_prev_entity(widget, ilk);
		break;
	}

	if (ilk)
		listbox_set_focus_item(widget, ilk);
}

void listbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);
	xfont_t xf;
	if_measure_t im = { 0 };

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	widget_get_xfont(widget, &xf);

	calc_listbox_size(&im, &xf, ptd->string, pxs);

	if (pxs->fy > 7 * DEF_TOUCH_SPAN)
		pxs->fy = 7 * DEF_TOUCH_SPAN;

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

void listbox_find(res_win_t widget, const tchar_t* token)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	link_t_ptr ent;
	int tlen;

	if (is_null(token))
	{
		listbox_set_focus_item(widget, NULL);
		return;
	}

	tlen = xslen(token);

	ent = get_string_next_entity(ptd->string, LINK_FIRST);
	while (ent)
	{
		if (get_string_entity_delta(ent))
		{
			ent = get_string_next_entity(ptd->string, ent);
			continue;
		}

		if (xsnicmp(get_string_entity_key_ptr(ent), token, tlen) == 0)
			break;

		ent = get_string_next_entity(ptd->string, ent);
	}

	listbox_set_focus_item(widget, ent);
}

void listbox_filter(res_win_t widget, const tchar_t* token)
{
	listbox_delta_t* ptd = GETLISTBOXDELTA(widget);

	link_t_ptr ent;
	int tlen;

	tlen = xslen(token);

	ent = get_string_next_entity(ptd->string, LINK_FIRST);
	while (ent)
	{
		if (xsnicmp(get_string_entity_key_ptr(ent), token, tlen) == 0)
			set_string_entity_delta(ent, 0);
		else
			set_string_entity_delta(ent, 1);

		ent = get_string_next_entity(ptd->string, ent);
	}

	ptd->entity = NULL;
	listbox_redraw(widget);
}
