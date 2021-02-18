/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tree control document

	@module	treectrl.c | implement file

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

typedef struct _tree_delta_t{
	link_t_ptr tree;
	link_t_ptr item;
	link_t_ptr hover;

	res_win_t editor;
	res_win_t vsc;

	int org_x, org_y, cur_x, cur_y;

	bool_t b_drag;
	bool_t b_lock;
}tree_delta_t;

typedef struct _tree_redraw_param{
	res_win_t wt;
	bool_t calc;
	bool_t valid;
}tree_redraw_param;

#define GETTREEDELTA(ph) 		(tree_delta_t*)widget_get_user_delta(ph)
#define SETTREEDELTA(ph,ptd)	widget_set_user_delta(ph,(var_long)ptd)


/*********************************************control event**************************************/


static void _treectrl_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	
	calc_tree_item_entity_rect(ptd->tree, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _treectrl_item_text_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	calc_tree_item_text_rect(ptd->tree, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _treectrl_item_expand_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	calc_tree_item_expand_rect(ptd->tree, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _treectrl_reset_page(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	int pw, ph, fw, fh, lh;
	xrect_t xr;
	xsize_t xs;
	canvas_t canv;
	measure_interface im = { 0 };

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	canv = widget_get_canvas(widget);
	get_canvas_measure(canv, &im);
	widget_get_canv_rect(widget, (canvbox_t*)&(im.rect));

	xs.fw = calc_tree_width(&im, ptd->tree);
	xs.fh = calc_tree_height(ptd->tree);

	widget_size_to_pt(widget, &xs);
	fw = xs.w;
	if (fw < pw)
		fw = pw;
	fh = xs.h;

	xs.fw = ZERO_WIDTH;
	xs.fh = get_tree_item_height(ptd->tree);

	widget_size_to_pt(widget, &xs);
	lh = xs.h;

	widget_reset_paging(widget, pw, ph, fw, fh, lh, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

static void _treectrl_ensure_visible(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	bool_t b_tag = 0;
	xrect_t xr;
	link_t_ptr parent;

	if (!ptd->item)
	{
		return;
	}

	parent = get_tree_parent_item(ptd->item);
	while (parent)
	{
		if (get_tree_item_collapsed(parent))
		{
			set_tree_item_collapsed(parent, 0);
			b_tag = 1;
		}
		parent = get_tree_parent_item(parent);
	}

	if (b_tag)
	{
		_treectrl_reset_page(widget);
	}

	_treectrl_item_rect(widget, ptd->item, &xr);
	widget_ensure_visible(widget, &xr, 1);
}

/******************************************************************************************/
int noti_tree_owner(res_win_t widget, unsigned int code, link_t_ptr tree, link_t_ptr ilk, void* data)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	NOTICE_TREE nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.tree = tree;
	nf.item = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_tree_item_enter(res_win_t widget, link_t_ptr plk)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(plk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = plk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_tree_item_leave(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_tree_item_hover(res_win_t widget, int x, int y)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_tree_owner(widget, NC_TREEITEMHOVER, ptd->tree, ptd->hover, (void*)&xp);
}

bool_t noti_tree_item_changing(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	link_t_ptr tlk;
	xrect_t xr;

	XDL_ASSERT(ptd->item != NULL);

	if (noti_tree_owner(widget, NC_TREEITEMCHANGING, ptd->tree, ptd->item, NULL))
		return 0;

	tlk = ptd->item;
	ptd->item = NULL;

	_treectrl_item_rect(widget, tlk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_erase(widget, &xr);

	return 1;
}

void noti_tree_item_changed(res_win_t widget, link_t_ptr ilk)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(NULL == ptd->item);

	ptd->item = ilk;
	_treectrl_item_rect(widget, ptd->item, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_erase(widget, &xr);

	noti_tree_owner(widget, NC_TREEITEMCHANGED, ptd->tree, ptd->item, NULL);
}

void noti_tree_item_checked(res_win_t widget, link_t_ptr ilk)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	bool_t bCheck;
	xrect_t xr;
	link_t_ptr tlk;

	XDL_ASSERT(ilk != NULL);

	bCheck = get_tree_item_checked(ilk);

	set_tree_item_checked(ilk, ((bCheck) ? 0 : 1));

	noti_tree_owner(widget, NC_TREEITEMCHECKED, ptd->tree, ilk, NULL);

	tlk = get_tree_first_child_item(ilk);
	while (tlk)
	{
		if (get_tree_item_showcheck(tlk))
		{
			set_tree_item_checked(tlk, ((bCheck) ? 0 : 1));

			noti_tree_owner(widget, NC_TREEITEMCHECKED, ptd->tree, tlk, NULL);
		}

		tlk = get_tree_next_sibling_item(tlk);
	}

	_treectrl_item_expand_rect(widget, ilk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_erase(widget, &xr);
}

void noti_tree_item_expand(res_win_t widget, link_t_ptr ilk)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ilk != NULL);

	set_tree_item_collapsed(ilk, 0);

	noti_tree_owner(widget, NC_TREEITEMEXPAND, ptd->tree, ilk, NULL);

	treectrl_redraw(widget);
}

void noti_tree_item_collapse(res_win_t widget, link_t_ptr ilk)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ilk != NULL);

	set_tree_item_collapsed(ilk, 1);

	noti_tree_owner(widget, NC_TREEITEMCOLLAPSE, ptd->tree, ilk, NULL);

	treectrl_redraw(widget);
}

void noti_tree_item_drag(res_win_t widget, int x, int y)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item != NULL);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	widget_set_cursor(widget, CURSOR_HAND);

	ptd->b_drag = 1;
	pt.x = x;
	pt.y = y;
	noti_tree_owner(widget, NC_TREEITEMDRAG, ptd->tree, ptd->item, (void*)&pt);
}

void noti_tree_item_drop(res_win_t widget, int x, int y)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item != NULL);

	widget_set_cursor(widget, CURSOR_ARROW);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}

	ptd->b_drag = 0;
	pt.x = x;
	pt.y = y;
	noti_tree_owner(widget, NC_TREEITEMDROP, ptd->tree, ptd->item, (void*)&pt);
}

void noti_tree_begin_edit(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	const tchar_t* text;
	xrect_t xr = { 0 };

	clr_mod_t ob = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd->item);

	if (widget_is_valid(ptd->editor))
		return;

	if (ptd->b_lock)
		return;

	if (get_tree_item_locked(ptd->item))
		return;

	widget_get_xfont(widget, &xf);
	parse_xfont_from_style(&xf, get_tree_style_ptr(ptd->tree));

	widget_get_color_mode(widget, &ob);

	_treectrl_item_text_rect(widget, ptd->item, &xr);
	pt_expand_rect(&xr, 0, DEF_INNER_FEED);

	if (noti_tree_owner(widget, NC_TREEITEMEDITING, ptd->tree, ptd->item, NULL))
		return;

	xr.w = 0;
	ptd->editor = fireedit_create(widget, &xr);
	XDL_ASSERT(ptd->editor);
	widget_set_user_id(ptd->editor, IDC_FIREEDIT);
	widget_set_owner(ptd->editor, widget);
	editbox_auto_size(ptd->editor, 1);

	widget_set_xfont(ptd->editor, &xf);
	widget_set_color_mode(ptd->editor, &ob);

	widget_show(ptd->editor, WS_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	text = get_tree_item_title_ptr(ptd->item);
	editbox_set_text(ptd->editor, text);
	editbox_selectall(ptd->editor);
}

void noti_tree_commit_edit(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	const tchar_t* text;
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->item);

	text = (tchar_t*)editbox_get_text_ptr(ptd->editor);

	if (!noti_tree_owner(widget, NC_TREEITEMCOMMIT, ptd->tree, ptd->item, (void*)text))
	{
		treectrl_set_item_title(widget, ptd->item, text);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_tree_rollback_edit(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->item);

	noti_tree_owner(widget, NC_TREEITEMROLLBACK, ptd->tree, ptd->item, NULL);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_tree_reset_editor(res_win_t widget, bool_t bCommit)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_tree_commit_edit(widget);
		else
			noti_tree_rollback_edit(widget);
	}
}

/********************************************************************************************/

int hand_tree_create(res_win_t widget, void* data)
{
	tree_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (tree_delta_t*)xmem_alloc(sizeof(tree_delta_t));
	xmem_zero((void*)ptd, sizeof(tree_delta_t));

	SETTREEDELTA(widget, ptd);

	ptd->b_lock = 1;

	return 0;
}

void hand_tree_destroy(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_tree_reset_editor(widget, 0);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	xmem_free(ptd);

	SETTREEDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_tree_size(res_win_t widget, int code, const xsize_t* pxs)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	xrect_t xr;

	if (!ptd->tree)
		return;

	widget_get_client_rect(widget, &xr);
	widget_rect_to_tm(widget, &xr);

	set_tree_width(ptd->tree, xr.fw);
	set_tree_height(ptd->tree, xr.fh);

	treectrl_redraw(widget);
}

void hand_tree_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	link_t_ptr tlk;
	int nHint;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	tlk = NULL;
	nHint = calc_tree_hint(&pt, ptd->tree, &tlk);

	bRe = (tlk == ptd->item) ? 1 : 0;

	if (nHint == TREE_HINT_EXPAND)
	{
		if (get_tree_item_collapsed(tlk))
			noti_tree_item_expand(widget, tlk);
		else
			noti_tree_item_collapse(widget, tlk);
		return;
	}

	if (nHint == TREE_HINT_CHECK)
	{
		noti_tree_item_checked(widget, tlk);
		return;
	}
}

void hand_tree_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	link_t_ptr tlk;
	int nHint;
	bool_t bRe;
	xpoint_t pt;

	if (!ptd->tree)
		return;

	if (ptd->b_drag)
	{
		noti_tree_item_drop(widget, pxp->x, pxp->y);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	tlk = NULL;
	nHint = calc_tree_hint(&pt, ptd->tree, &tlk);

	if (nHint == TREE_HINT_EXPAND || nHint == TREE_HINT_CHECK)
	{
		return;
	}

	bRe = (tlk == ptd->item) ? 1 : 0;

	if (bRe && ptd->item && !ptd->b_lock && !get_tree_item_locked(ptd->item))
	{
		widget_post_key(widget, KEY_ENTER);
		return;
	}

	if (!bRe && ptd->item)
	{
		if (!noti_tree_item_changing(widget))
			bRe = 1;
	}

	if (!bRe && tlk)
	{
		noti_tree_item_changed(widget, tlk);
	}

	if (!bRe && ptd->item)
	{
		_treectrl_ensure_visible(widget);
	}

	noti_tree_owner(widget, NC_TREELBCLK, ptd->tree, ptd->item, (void*)pxp);
}

void hand_tree_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (!ptd->tree)
		return;

	if (widget_is_valid(ptd->editor))
		return;

	noti_tree_owner(widget, NC_TREEDBCLK, ptd->tree, ptd->item, (void*)pxp);
}

void hand_tree_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, 1);
}

void hand_tree_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (!ptd->tree)
		return;

	noti_tree_owner(widget, NC_TREERBCLK, ptd->tree, ptd->item, (void*)pxp);
}

void hand_tree_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	link_t_ptr tlk;
	int nHint;
	xpoint_t pt;

	if (!ptd->tree)
		return;

	if (ptd->b_drag)
		return;

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	tlk = NULL;
	nHint = calc_tree_hint(&pt, ptd->tree, &tlk);

	if (nHint == TREE_HINT_ITEM && tlk == ptd->item && !(dw & KS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_tree_item_drag(widget, pxp->x, pxp->y);
			return;
		}
	}

	if (nHint == TREE_HINT_ITEM && !ptd->hover && tlk)
	{
		noti_tree_item_enter(widget, tlk);
	}
	else if (nHint == TREE_HINT_ITEM && ptd->hover && ptd->hover != tlk)
	{
		noti_tree_item_leave(widget);
	}
	else if (nHint != TREE_HINT_ITEM && ptd->hover)
	{
		noti_tree_item_leave(widget);
	}
}

void hand_tree_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (!ptd->tree)
		return;

	if (ptd->hover)
		noti_tree_item_hover(widget, pxp->x, pxp->y);
}

void hand_tree_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (!ptd->tree)
		return;

	if (ptd->hover)
		noti_tree_item_leave(widget);
}

void hand_tree_keydown(res_win_t widget, dword_t ks, int nKey)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (!ptd->tree)
		return;

	switch (nKey)
	{
	case KEY_ENTER:
		if (ptd->item)
		{
			noti_tree_begin_edit(widget);
		}
		break;
	case KEY_SPACE:
		if (ptd->item && get_tree_item_showcheck(ptd->item))
		{
			noti_tree_item_checked(widget, ptd->item);
		}
		break;
	case KEY_DOWN:
		treectrl_tabskip(widget,TABORDER_DOWN);
		break;
	case KEY_RIGHT:
		treectrl_tabskip(widget,TABORDER_RIGHT);
		break;
		break;
	case KEY_UP:
		treectrl_tabskip(widget,TABORDER_UP);
		break;
	case KEY_LEFT:
		treectrl_tabskip(widget,TABORDER_LEFT);
		break;
	}
}

void hand_tree_char(res_win_t widget, tchar_t nChar)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (!ptd->tree)
		return;
}

void hand_tree_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, 1);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_tree_wheel(res_win_t widget, bool_t bHorz, int nDelta)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	scroll_t scr = { 0 };
	int nLine;
	res_win_t win;

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, 1);

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

		return;
	}

	win = widget_get_parent(widget);

	if (widget_is_valid(win))
	{
		widget_scroll(win, bHorz, nLine);
	}
}

void hand_tree_child_command(res_win_t widget, int code, var_long data)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_tree_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_tree_rollback_edit(widget);
		break;
	}
}

void hand_tree_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	visual_t rdc;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	const drawing_interface* pif = NULL;
	drawing_interface ifv = {0};

	if (!ptd->tree)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);

	pif = widget_get_canvas_interface(widget);
	

	
	
	
	
	

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);
		
	get_visual_interface(rdc, &ifv);

	(*ifv.pf_draw_rect)(ifv.ctx, NULL, &xb, &xr);

	draw_tree(pif, ptd->tree);

	//draw focus
	if (ptd->item)
	{
		widget_get_view_rect(widget, (viewbox_t*)(&ifv.rect));

		parse_xcolor(&xc, DEF_ENABLE_COLOR);

		_treectrl_item_rect(widget, ptd->item, &xr);
		pt_expand_rect(&xr, DEF_INNER_FEED, 0);

		draw_select_raw(&ifv, &xc, &xr, ALPHA_SOFT);

	}

	
	
	end_canvas_paint(canv, dc, pxr);
	
}

/*****************************************************************************************************/

res_win_t treectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_tree_create)
		EVENT_ON_DESTROY(hand_tree_destroy)

		EVENT_ON_PAINT(hand_tree_paint)

		EVENT_ON_SIZE(hand_tree_size)

		EVENT_ON_SCROLL(hand_tree_scroll)
		EVENT_ON_WHEEL(hand_tree_wheel)

		EVENT_ON_KEYDOWN(hand_tree_keydown)
		EVENT_ON_CHAR(hand_tree_char)

		EVENT_ON_MOUSE_MOVE(hand_tree_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_tree_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_tree_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_tree_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_tree_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_tree_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_tree_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_tree_rbutton_up)

		EVENT_ON_CHILD_COMMAND(hand_tree_child_command)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void treectrl_attach(res_win_t widget, link_t_ptr ptr)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_tree_doc(ptr));

	noti_tree_reset_editor(widget, 0);

	ptd->tree = ptr;
	ptd->item = NULL;

	widget_get_client_rect(widget, &xr);
	widget_rect_to_tm(widget, &xr);

	set_tree_width(ptd->tree, xr.fw);
	set_tree_height(ptd->tree, xr.fh);

	treectrl_redraw(widget);
}

link_t_ptr treectrl_detach(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	link_t_ptr ptr;

	XDL_ASSERT(ptd != NULL);

	noti_tree_reset_editor(widget, 0);

	ptr = ptd->tree;
	ptd->tree = NULL;

	widget_erase(widget, NULL);
	return ptr;
}

link_t_ptr treectrl_fetch(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->tree;
}

void treectrl_accept(res_win_t widget, bool_t bAccept)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, bAccept);
}

bool_t _redraw_tree_node(link_t_ptr plk, void* pv)
{
	tree_redraw_param* ptp = (tree_redraw_param*)pv;

	tree_delta_t* ptd = GETTREEDELTA(ptp->wt);

	if (plk == ptd->item)
		ptp->valid = 1;

	if (ptp->calc)
		noti_tree_owner(ptp->wt, NC_TREEITEMCALCED, ptd->tree, plk, NULL);

	return 1;
}

void treectrl_redraw(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	tree_redraw_param tp = { 0 };

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, 0);

	tp.wt = widget;
	tp.calc = 1;
	tp.valid = 0;

	enum_dom_node(ptd->tree, _redraw_tree_node, (void*)&tp);

	noti_tree_owner(widget, NC_TREECALCED, ptd->tree, NULL, NULL);

	if (!tp.valid)
	{
		ptd->item = NULL;
	}
	ptd->hover = NULL;

	_treectrl_reset_page(widget);

	widget_update(widget);
}

void treectrl_redraw_item(res_win_t widget, link_t_ptr ilk)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, 0);

#ifdef _DEBUG
	XDL_ASSERT(is_tree_item(ptd->tree, ilk));
#endif

	noti_tree_owner(widget, NC_TREEITEMCALCED, ptd->tree, ilk, NULL);

	_treectrl_item_rect(widget, ilk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_erase(widget, &xr);
}

bool_t treectrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return 0;

	noti_tree_reset_editor(widget, 0);

	if (ilk)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_tree_item(ptd->tree, ilk));
#endif
	}

	bRe = (ilk == ptd->item) ? 1 : 0;
	if (bRe)
		return 1;

	if (ptd->item && !bRe)
	{
		if (!noti_tree_item_changing(widget))
			return 0;
	}

	if (ilk && !bRe)
	{
		noti_tree_item_changed(widget, ilk);

		_treectrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr treectrl_get_focus_item(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return NULL;

	return ptd->item;
}

bool_t treectrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* szText)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	const tchar_t* text;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_tree_item(ptd->tree, ilk));
#endif

	text = get_tree_item_title_ptr(ilk);
	if (compare_text(szText, -1, text, -1, 0) != 0)
	{
		set_tree_item_title(ilk, szText);

		noti_tree_owner(widget, NC_TREEITEMUPDATE, ptd->tree, ilk, NULL);

		treectrl_redraw_item(widget, ilk);
	}

	return 1;
}

void treectrl_tabskip(res_win_t widget, int nSkip)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	link_t_ptr plk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, 1);

	switch (nSkip)
	{
	case TABORDER_RIGHT:
		if (ptd->item && get_tree_child_item_count(ptd->item))
		{
			if (get_tree_item_collapsed(ptd->item))
			{
				noti_tree_item_expand(widget, ptd->item);
			}
			plk = get_tree_first_child_item(ptd->item);
		}
		else
		{
			plk = ptd->item;
		}
		break;
	case TABORDER_DOWN:
		if (ptd->item)
		{
			plk = get_tree_next_visible_item(ptd->tree, ptd->item);
		}
		else
		{
			plk = get_tree_first_child_item(ptd->tree);
		}
		break;
	case TABORDER_LEFT:
		if (ptd->item && get_tree_parent_item(ptd->item))
		{
			plk = get_tree_parent_item(ptd->item);
			if (!get_tree_item_collapsed(plk))
			{
				noti_tree_item_expand(widget, plk);
			}
		}
		else
		{
			plk = ptd->item;
		}
		break;
	case TABORDER_UP:
		if (ptd->item)
		{
			plk = get_tree_prev_visible_item(ptd->tree, ptd->item);
		}
		else
		{
			plk = get_tree_last_child_item(ptd->tree);
		}
		break;
	case TABORDER_HOME:
		plk = get_tree_first_child_item(ptd->tree);
		break;
	case TABORDER_END:
		plk = get_tree_last_child_item(ptd->tree);
		break;
	}

	if (plk && is_tree_doc(plk))
		plk = NULL;

	if (plk)
		treectrl_set_focus_item(widget, plk);
}

void treectrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* prt)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_tree_item(ptd->tree, ilk));
#endif

	_treectrl_item_rect(widget, ilk, prt);
}

void treectrl_find(res_win_t widget, const tchar_t* token)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	link_t_ptr elk;
	int tlen;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return;

	noti_tree_reset_editor(widget, 1);

	tlen = xslen(token);

	if (tlen)
	{
		elk = (ptd->item) ? get_tree_next_sibling_item(ptd->item) : get_tree_first_child_item(ptd->tree);

		while (elk)
		{
			if (xsnicmp(get_tree_item_title_ptr(elk), token, tlen) == 0)
				break;

			elk = get_tree_next_sibling_item(elk);
		}
	}
	else
	{
		elk = NULL;
	}

	if (elk)
		treectrl_set_focus_item(widget, elk);
	else
		treectrl_set_focus_item(widget, NULL);
}

void treectrl_popup_size(res_win_t widget, xsize_t* pse)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);
	int count;
	canvas_t canv;
	measure_interface im = { 0 };

	XDL_ASSERT(ptd != NULL);

	if (!ptd->tree)
		return;

	count = get_tree_child_item_count(ptd->tree);
	if (count > 7)
	{
		count = 7;
	}

	canv = widget_get_canvas(widget);
	get_canvas_measure(canv, &im);
	widget_get_canv_rect(widget, (canvbox_t*)&(im.rect));

	pse->fw = calc_tree_width(&im, ptd->tree);
	pse->fh = count * get_tree_item_height(ptd->tree);

	widget_size_to_pt(widget, pse);

	widget_adjust_size(widget_get_style(widget), pse);
}

void treectrl_set_lock(res_win_t widget, bool_t bLock)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t treectrl_get_lock(res_win_t widget)
{
	tree_delta_t* ptd = GETTREEDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}
