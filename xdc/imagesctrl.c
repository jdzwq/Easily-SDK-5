/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc image control document

	@module	imagesctrl.c | implement file

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

typedef struct _images_delta_t{
	link_t_ptr images;
	link_t_ptr item;
	link_t_ptr hover;

	res_win_t editor;
	res_win_t hsc;
	res_win_t vsc;

	int opera;
	bool_t b_drag;
	bool_t b_lock;
}images_delta_t;

#define GETIMAGESDELTA(widget) 	((images_delta_t*)widget_get_user_delta(widget))
#define SETIMAGESDELTA(widget,ptd) widget_set_user_delta(widget,(var_long)ptd)

/******************************************************************************************************/
static bool_t _imagesctrl_copy(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	res_glob_t gb;
	int len;
	tchar_t* buf;

	XDL_ASSERT(ptd && ptd->images);

	if (!ptd->item)
		return 0;

	if (!clipboard_open())
		return 0;

	len = xslen(get_images_item_src_ptr(ptd->item));

	gb = gmem_alloc((len + 1) * sizeof(tchar_t));
	buf = (tchar_t*)gmem_lock(gb);

	xsncpy(buf, get_images_item_src_ptr(ptd->item), len);

	gmem_unlock(gb);

	if (!clipboard_put(DEF_CB_FORMAT, gb))
	{
		gmem_free(gb);
		clipboard_close();

		return 0;
	}

	clipboard_close();
	return 1;
}

static bool_t _imagesctrl_cut(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	link_t_ptr ilk;

	XDL_ASSERT(ptd && ptd->images);

	if (!ptd->item)
		return 0;

	if (!_imagesctrl_copy(widget))
		return 0;

	ilk = ptd->item;
	ptd->item = NULL;

	delete_images_item(ilk);

	imagesctrl_redraw(widget);

	return 1;
}

static bool_t _imagesctrl_paste(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	res_glob_t gb;
	tchar_t* buf;
	link_t_ptr ilk;

	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->images);

	if (!clipboard_open())
		return 0;

	gb = clipboard_get(DEF_CB_FORMAT);
	if (!gb)
	{
		clipboard_close();
		return 0;
	}

	buf = (tchar_t*)gmem_lock(gb);

	if (compare_text(buf, xslen(GDI_ATTR_IMAGE_HEAD), GDI_ATTR_IMAGE_HEAD, -1, 1) != 0)
	{
		gmem_unlock(gb);
		clipboard_close();
		return 0;
	}

	ilk = insert_images_item(ptd->images, LINK_LAST);
	set_images_item_src(ilk, buf);

	xsprintf(sz_name, _T("image%d"), get_images_item_count(ptd->images));
	set_images_item_alt(ilk, sz_name);

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	imagesctrl_redraw(widget);

	return 1;
}

static void _imagesctrl_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_images_item_rect(&cb, ptd->images, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _imagesctrl_image_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_images_image_rect(&cb, ptd->images, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _imagesctrl_text_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_images_text_rect(&cb, ptd->images, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _imagesctrl_reset_page(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	int pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;
	canvbox_t cb = { 0 };
	bool_t b_horz;

	b_horz = (compare_text(get_images_layer_ptr(ptd->images), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.cx = pw;
	xs.cy = ph;
	widget_size_to_tm(widget, &xs);

	cb.fw = xs.fx;
	cb.fh = xs.fy;
	if (b_horz)
	{
		xs.fx = calc_images_width(&cb, ptd->images);
	}
	else
	{
		xs.fy = calc_images_height(&cb, ptd->images);
	}
	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = get_images_item_width(ptd->images);
	xs.fy = get_images_item_height(ptd->images);
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	if (b_horz)
		widget_reset_scroll(widget, 1);
	else
		widget_reset_scroll(widget, 0);
}

void _imagesctrl_ensure_visible(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	xrect_t xr = { 0 };

	if (!ptd->item)
		return;

	_imagesctrl_item_rect(widget, ptd->item, &xr);
	
	widget_ensure_visible(widget, &xr, 1);
}

/************************************control event**********************************************/
int noti_images_owner(res_win_t widget, unsigned int code, link_t_ptr image, link_t_ptr ilk, void* data)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	NOTICE_IMAGES nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.images = image;
	nf.item = ilk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_images_reset_check(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	link_t_ptr ilk;
	int count = 0;

	ilk = get_images_next_item(ptd->images, LINK_FIRST);
	while (ilk)
	{
		if (get_images_item_checked(ilk))
		{
			set_images_item_checked(ilk, 0);
			noti_images_owner(widget, NC_IMAGEITEMCHECKED, ptd->images, ilk, NULL);

			count++;
		}
		ilk = get_images_next_item(ptd->images, ilk);
	}

	if (!count)
		return;

	widget_redraw(widget, NULL, 0);
}

bool_t noti_images_item_changing(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	if (noti_images_owner(widget, NC_IMAGEITEMCHANGING, ptd->images, ptd->item, NULL))
		return 0;

	_imagesctrl_item_rect(widget, ptd->item, &xr);

	ptd->item = NULL;

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	return 1;
}

void noti_images_item_changed(res_win_t widget, link_t_ptr plk)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = plk;

	_imagesctrl_item_rect(widget, plk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);

	noti_images_owner(widget, NC_IMAGEITEMCHANGED, ptd->images, ptd->item, NULL);
}

void noti_images_item_enter(res_win_t widget, link_t_ptr plk)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(plk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = plk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_images_item_leave(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_images_item_hover(res_win_t widget, int x, int y)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_images_owner(widget, NC_IMAGEITEMHOVER, ptd->images, ptd->hover, (void*)&xp);
}

void noti_images_item_check(res_win_t widget, link_t_ptr plk)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	xrect_t xr;
	bool_t b_check;

	XDL_ASSERT(plk);

	b_check = get_images_item_checked(plk);

	set_images_item_checked(plk, ((b_check) ? 0 : 1));

	noti_images_owner(widget, NC_IMAGEITEMCHECKED, ptd->images, plk, NULL);

	_imagesctrl_item_rect(widget, plk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

void noti_images_item_drag(res_win_t widget, int x, int y)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item);

	ptd->b_drag = 1;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_images_owner(widget, NC_IMAGEITEMDRAG, ptd->images, ptd->item, (void*)&pt);
}

void noti_images_item_drop(res_win_t widget, int x, int y)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->item);

	ptd->b_drag = 0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	pt.x = x;
	pt.y = y;
	noti_images_owner(widget, NC_IMAGEITEMDROP,ptd->images, ptd->item, (void*)&pt);
}

void noti_images_begin_edit(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	const tchar_t* text;
	xrect_t xr = { 0 };

	clr_mod_t ob = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd->item);
	
	if (widget_is_valid(ptd->editor))
		return;

	if (ptd->b_lock)
		return;

	widget_get_xfont(widget, &xf);

	widget_get_color_mode(widget, &ob);

	_imagesctrl_text_rect(widget, ptd->item, &xr);

	if (noti_images_owner(widget, NC_IMAGEITEMEDITING, ptd->images, ptd->item, NULL))
		return;

	ptd->editor = fireedit_create(widget, &xr);
	XDL_ASSERT(ptd->editor);
	widget_set_user_id(ptd->editor, IDC_FIREEDIT);
	widget_set_owner(ptd->editor, widget);

	widget_set_xfont(ptd->editor, &xf);
	widget_set_color_mode(ptd->editor, &ob);

	widget_show(ptd->editor, WD_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	text = get_images_item_alt_ptr(ptd->item);
	editbox_set_text(ptd->editor, text);
	editbox_selectall(ptd->editor);
}

void noti_images_commit_edit(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	const tchar_t* text;
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->item);

	text = (tchar_t*)editbox_get_text_ptr(ptd->editor);

	if (!noti_images_owner(widget, NC_IMAGEITEMCOMMIT, ptd->images, ptd->item, (void*)text))
	{
		imagesctrl_set_item_title(widget, ptd->item, text);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_images_rollback_edit(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->item);

	noti_images_owner(widget, NC_IMAGEITEMROLLBACK, ptd->images, ptd->item, NULL);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_images_reset_editor(res_win_t widget, bool_t bCommit)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_images_commit_edit(widget);
		else
			noti_images_rollback_edit(widget);
	}
}

/********************************************************************************************************/

int hand_images_create(res_win_t widget, void* data)
{
	images_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (images_delta_t*)xmem_alloc(sizeof(images_delta_t));
	xmem_zero((void*)ptd, sizeof(images_delta_t));

	SETIMAGESDELTA(widget, ptd);

	ptd->b_lock = 1;

	return 0;
}

void hand_images_destroy(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_images_reset_editor(widget, 0);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	xmem_free(ptd);

	SETIMAGESDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_images_size(res_win_t widget, int code, const xsize_t* prs)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	imagesctrl_redraw(widget);
}

void hand_images_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	noti_images_reset_editor(widget, 1);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_images_wheel(res_win_t widget, bool_t bHorz, int nDelta)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	scroll_t scr = { 0 };
	int nLine;
	res_win_t win;
	bool_t b_horz;

	if (!ptd->images)
		return;

	noti_images_reset_editor(widget, 1);

	widget_get_scroll(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widget_hand_scroll(widget, bHorz, nLine))
	{
		b_horz = (compare_text(get_images_layer_ptr(ptd->images), -1, ATTR_LAYER_HORZ, -1, 0) == 0) ? 1 : 0;

		if (!b_horz && !bHorz && !(widget_get_style(widget) & WD_STYLE_VSCROLL))
		{
			if (!widget_is_valid(ptd->vsc))
			{
				ptd->vsc = show_vertbox(widget);
			}
		}

		if (b_horz && bHorz && !(widget_get_style(widget) & WD_STYLE_HSCROLL))
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

void hand_images_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	int nHint;
	link_t_ptr plk;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->images)
		return;

	if (ptd->b_drag)
		return;

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_images_hint(&cb, &pt, ptd->images, &plk);

	if (nHint == IMAGE_HINT_ITEM && plk == ptd->item && !(dw & MS_WITH_CONTROL))
	{
		if (dw & MS_WITH_LBUTTON)
		{
			noti_images_item_drag(widget, pxp->x, pxp->y);
			return;
		}
	}

	if (nHint == IMAGE_HINT_ITEM && !ptd->hover && plk)
	{
		noti_images_item_enter(widget, plk);
	}
	else if (nHint == IMAGE_HINT_ITEM && ptd->hover && ptd->hover != plk)
	{
		noti_images_item_leave(widget);
	}
	else if (nHint == IMAGE_HINT_NONE && ptd->hover)
	{
		noti_images_item_leave(widget);
	}
}

void hand_images_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	if (ptd->hover)
		noti_images_item_hover(widget, pxp->x, pxp->y);
}

void hand_images_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	if (ptd->hover)
		noti_images_item_leave(widget);
}

void hand_images_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	if (widget_is_valid(ptd->editor))
		return;

	noti_images_owner(widget, NC_IMAGESDBCLK, ptd->images, ptd->item, (void*)pxp);
}

void hand_images_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	int nHint;
	link_t_ptr plk;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->images)
		return;

	noti_images_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_images_hint(&cb, &pt, ptd->images, &plk);
	bRe = (plk == ptd->item) ? 1 : 0;

	if (nHint == IMAGE_HINT_CHECK)
	{
		noti_images_item_check(widget, plk);
	}
	else if (nHint == IMAGE_HINT_NONE)
	{
		if (ptd->opera != WD_OPERA_CONTROL && !widget_key_state(widget, KEY_CONTROL))
		{
			noti_images_reset_check(widget);
		}
	}
}

void hand_images_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	int nHint;
	link_t_ptr plk;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->images)
		return;

	if (ptd->b_drag)
	{
		noti_images_item_drop(widget, pxp->x, pxp->y);
		return;
	}

	widget_get_canv_rect(widget, &cb);

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	plk = NULL;
	nHint = calc_images_hint(&cb, &pt, ptd->images, &plk);
	
	if (nHint == IMAGE_HINT_CHECK)
	{
		return;
	}

	bRe = (plk == ptd->item) ? 1 : 0;

	if (bRe && ptd->item && !ptd->b_lock)
	{
		widget_post_key(widget, KEY_ENTER);
		return;
	}

	if (ptd->item && !bRe)
	{
		if (!noti_images_item_changing(widget))
			bRe = 1;
	}

	if (plk && !bRe)
	{
		noti_images_item_changed(widget, plk);

		_imagesctrl_ensure_visible(widget);
	}

	noti_images_owner(widget, NC_IMAGESLBCLK, ptd->images, ptd->item, (void*)pxp);
}

void hand_images_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	noti_images_reset_editor(widget, 1);
}

void hand_images_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	noti_images_owner(widget, NC_IMAGESRBCLK, ptd->images, ptd->item, (void*)pxp);
}

void hand_images_keydown(res_win_t widget, int nKey)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	switch (nKey)
	{
	case KEY_ENTER:
		if (ptd->item)
			noti_images_begin_edit(widget);
		break;
	case KEY_SPACE:
		if (ptd->item)
			noti_images_item_check(widget, ptd->item);
		break;
	case KEY_LEFT:
		imagesctrl_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		imagesctrl_tabskip(widget,WD_TAB_RIGHT);
		break;
	case VK_UP:
		imagesctrl_tabskip(widget,WD_TAB_UP);
		break;
	case VK_DOWN:
		imagesctrl_tabskip(widget,WD_TAB_DOWN);
		break;
	case KEY_HOME:
		imagesctrl_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		imagesctrl_tabskip(widget,WD_TAB_END);
		break;
	case _T('c'):
	case _T('C'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_imagesctrl_copy(widget);
		}
		break;
	case _T('x'):
	case _T('X'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_imagesctrl_cut(widget);
		}
		break;
	case _T('v'):
	case _T('V'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_imagesctrl_paste(widget);
		}
		break;
	}
}

void hand_images_char(res_win_t widget, tchar_t ch)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;
}

void hand_images_set_focus(res_win_t widget, res_win_t wt)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;
}

void hand_images_kill_focus(res_win_t widget, res_win_t wt)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

	if (widget_is_editor(widget))
	{
		widget_send_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
	}
}

void hand_images_notice(res_win_t widget, NOTICE* pnt)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

}

void hand_images_child_command(res_win_t widget, int code, var_long data)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_images_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_images_rollback_edit(widget);
		break;
	}
}

void hand_images_erase(res_win_t widget, res_ctx_t dc)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	if (!ptd->images)
		return;

}

void hand_images_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr = { 0 };

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->images)
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

	draw_images(pif, &cb, ptd->images);

	if (ptd->item)
	{
		_imagesctrl_image_rect(widget, ptd->item, &xr);

		pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

		parse_xcolor(&xc, DEF_ENABLE_COLOR);

		draw_focus_raw(rdc, &xc, &xr, ALPHA_TRANS);
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/**************************************************************************************************/

res_win_t imagesctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_images_create)
		EVENT_ON_DESTROY(hand_images_destroy)

		EVENT_ON_ERASE(hand_images_erase)
		EVENT_ON_PAINT(hand_images_paint)

		EVENT_ON_SIZE(hand_images_size)

		EVENT_ON_SCROLL(hand_images_scroll)
		EVENT_ON_WHEEL(hand_images_wheel)

		EVENT_ON_KEYDOWN(hand_images_keydown)
		EVENT_ON_CHAR(hand_images_char)

		EVENT_ON_MOUSE_MOVE(hand_images_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_images_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_images_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_images_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_images_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_images_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_images_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_images_rbutton_up)

		EVENT_ON_NOTICE(hand_images_notice)
		EVENT_ON_CHILD_COMMAND(hand_images_child_command)

		EVENT_ON_SET_FOCUS(hand_images_set_focus)
		EVENT_ON_KILL_FOCUS(hand_images_kill_focus)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void imagesctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_images_doc(ptr));

	ptd->images = ptr;

	imagesctrl_redraw(widget);
}

link_t_ptr imagesctrl_detach(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	noti_images_reset_editor(widget, 0);

	if (ptd->images)
	{
		data = ptd->images;
		ptd->images = NULL;
	}
	else
	{
		data = NULL;
	}
	return data;
}

link_t_ptr imagesctrl_fetch(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->images;
}

void imagesctrl_redraw(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->images)
		return;

	noti_images_reset_editor(widget, 0);

	b_valid = 0;
	ilk = get_images_next_item(ptd->images, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		noti_images_owner(widget, NC_IMAGEITEMCALCED, ptd->images, ilk, NULL);

		ilk = get_images_next_item(ptd->images, ilk);
	}

	noti_images_owner(widget, NC_IMAGESCALCED, ptd->images, NULL, NULL);

	if (!b_valid)
	{
		ptd->item = NULL;
	}
	ptd->hover = NULL;

	_imagesctrl_reset_page(widget);

	widget_update(widget);
}

void imagesctrl_tabskip(res_win_t widget, int nSkip)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	link_t_ptr plk = ptd->item;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->images)
		return;

	noti_images_reset_editor(widget, 0);

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (plk == NULL)
			plk = get_images_next_item(ptd->images, LINK_FIRST);
		else
			plk = get_images_next_item(ptd->images, ptd->item);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (plk == NULL)
			plk = get_images_prev_item(ptd->images, LINK_LAST);
		else
			plk = get_images_prev_item(ptd->images, ptd->item);
		break;
	case WD_TAB_HOME:
		plk = get_images_next_item(ptd->images, LINK_FIRST);
		break;
	case WD_TAB_END:
		plk = get_images_prev_item(ptd->images, LINK_LAST);
		break;
	}

	imagesctrl_set_focus_item(widget, plk);
}

void imagesctrl_redraw_item(res_win_t widget, link_t_ptr plk)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->images)
		return;

	noti_images_reset_editor(widget, 0);

#ifdef _DEBUG
	XDL_ASSERT(is_images_item(ptd->images, plk));
#endif

	noti_images_owner(widget, NC_IMAGEITEMCALCED, ptd->images, plk, NULL);

	_imagesctrl_item_rect(widget, plk, &xr);
	
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_redraw(widget, &xr, 0);
}

bool_t imagesctrl_set_focus_item(res_win_t widget, link_t_ptr ilk)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->images)
		return 0;

	noti_images_reset_editor(widget, 0);

	if (ilk == LINK_FIRST)
		ilk = get_images_next_item(ptd->images, LINK_FIRST);
	else if (ilk == LINK_LAST)
		ilk = get_images_prev_item(ptd->images, LINK_LAST);

	bRe = (ilk == ptd->item) ? 1 : 0;
	if (bRe)
		return 1;

	if (ptd->item && !bRe)
	{
		if (!noti_images_item_changing(widget))
			return 0;
	}

	if (ilk && !bRe)
	{
		noti_images_item_changed(widget, ilk);

		_imagesctrl_ensure_visible(widget);
	}

	return 1;
}

link_t_ptr imagesctrl_get_focus_item(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->images)
		return NULL;

	return ptd->item;
}

bool_t imagesctrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* szText)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	const tchar_t* text;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->images)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_images_item(ptd->images, ilk));
#endif

	text = get_images_item_alt_ptr(ilk);
	if (compare_text(szText, -1, text, -1, 0) != 0)
	{
		set_images_item_alt(ilk, szText);

		noti_images_owner(widget, NC_IMAGEITEMUPDATE, ptd->images, ilk, NULL);

		imagesctrl_redraw_item(widget, ilk);

		return 1;
	}

	return 0;
}

void imagesctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->images)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_images_item(ptd->images, ilk));
#endif

	_imagesctrl_item_rect(widget, ilk, pxr);
}

void imagesctrl_set_opera(res_win_t widget, int opera)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->opera = opera;
}

int imagesctrl_get_opera(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->opera;
}

void imagesctrl_popup_size(res_win_t widget, xsize_t* pse)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);
	float ih, iw;
	int count;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->images)
		return;

	ih = get_images_item_height(ptd->images);
	iw = get_images_item_width(ptd->images);

	count = get_images_item_count(ptd->images);
	if (count % 3)
		count = count / 3 + 1;
	else
		count = count / 3;

	if (count > 5)
		count = 5;

	if (compare_text(get_images_layer_ptr(ptd->images), -1, ATTR_LAYER_HORZ, -1, 0) == 0)
	{
		pse->fx = iw * count;
		pse->fy = 3 * ih;
	}
	else
	{
		pse->fx = iw * 3;
		pse->fy = ih * count;
	}

	widget_size_to_pt(widget, pse);

	widget_adjust_size(widget_get_style(widget), pse);
}

bool_t imagesctrl_get_lock(res_win_t widget)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

void imagesctrl_set_lock(res_win_t widget, bool_t bLock)
{
	images_delta_t* ptd = GETIMAGESDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

