/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc edit control document

	@module	editbox.c | edit control widnows implement file

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
#include "xdcmenu.h"
#include "textor.h"

typedef struct _editbox_delta_t{
	textor_t textor;

	int chs;
	tchar_t pch[CHS_LEN + 1];

	bool_t b_lock;
	bool_t b_auto;
}editbox_delta_t;

#define GETEDITBOXDELTA(ph) 	(editbox_delta_t*)widget_get_user_delta(ph)
#define SETEDITBOXDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

int _editbox_get_text(void* data, tchar_t* buf, int max)
{
	string_t vs = (string_t)data;
	int len;

	len = varstr_len(vs);
	len = (len < max) ? len : max;

	if (buf)
		xsncpy(buf, varstr_ptr(vs), len);

	return len;
}

void _editbox_set_text(void* data, const tchar_t* buf, int len)
{
	string_t vs = (string_t)data;

	varstr_cpy(vs, buf, len);
}

static bool_t _editbox_get_paging(res_win_t widget, xsize_t* pse)
{
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	pse->cx = xr.w;
	pse->cy = xr.h;

	return 0;
}

void _editbox_auto_resize(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	string_t vs;
	xsize_t xs;
	xrect_t xr;
	long cx;
	const xfont_t* pxf;

	pxf = widget_get_xfont_ptr(widget);

	XDL_ASSERT(ptd != NULL);

	widget_get_window_rect(widget, &xr);

	vs = (string_t)ptd->textor.data;

	text_metric_raw(ptd->textor.dc, pxf, &xs);
	cx = xs.cx;
	text_size_raw(ptd->textor.dc, pxf, varstr_ptr(vs), varstr_len(vs), &xs);

	if (xs.cx + cx > xr.w)
	{
		xs.cx += (cx + 4) / 2;
		xs.cy = xr.h;

		widget_size(widget, &xs);
	}
}

/*****************************************************************************/
void noti_editbox_command(res_win_t widget, int code, var_long data)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}

/**********************************************************************************/
int hand_editbox_create(res_win_t widget, void* data)
{
	editbox_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (editbox_delta_t*)xmem_alloc(sizeof(editbox_delta_t));
	xmem_zero((void*)ptd, sizeof(editbox_delta_t));

	ptd->textor.widget = widget;
	ptd->textor.dc = widget_client_ctx(widget);
	ptd->textor.data = (void*)varstr_alloc();
	ptd->textor.pf_scan_text = (PF_SCAN_TEXT)scan_var_text;
	ptd->textor.pf_get_text = _editbox_get_text;
	ptd->textor.pf_set_text = _editbox_set_text;
	ptd->textor.pf_get_paging = _editbox_get_paging;
	ptd->textor.max_undo = 1024;
	ptd->textor.page = 1;

	SETEDITBOXDELTA(widget, ptd);

	return 0;
}

void hand_editbox_destroy(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	res_win_t keybox;

	XDL_ASSERT(ptd != NULL);

	hand_textor_clean(&ptd->textor);

	widget_release_ctx(widget, ptd->textor.dc);
	varstr_free((string_t)ptd->textor.data);

	xmem_free(ptd);

	SETEDITBOXDELTA(widget, 0);

	widget_hand_destroy(widget);

	keybox = (res_win_t)widget_get_user_prop(widget, XDCKEYBOX);

	if (widget_is_valid(keybox))
		widget_destroy(keybox);
}

void hand_editbox_set_focus(res_win_t widget, res_win_t wt)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	hand_textor_set_focus(&ptd->textor, wt);
}

void hand_editbox_kill_focus(res_win_t widget, res_win_t wt)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	hand_textor_kill_focus(&ptd->textor, wt);

	if (widget_is_editor(widget))
	{
		noti_editbox_command(widget, COMMAND_COMMIT, (var_long)NULL);
	}
}

void hand_editbox_keydown(res_win_t widget, int key)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	const xface_t* pxa;

	switch (key)
	{
	case KEY_BACK:
		if (ptd->b_lock)
			break;
		if (_TEXTOR_PRESS_ACCEPT == hand_textor_back(&ptd->textor))
		{
			noti_editbox_command(widget, COMMAND_UPDATE, (var_long)NULL);
		}
		break;
	case KEY_DELETE:
		if (ptd->b_lock)
			break;
		if (_TEXTOR_PRESS_ACCEPT == hand_textor_delete(&ptd->textor))
		{
			noti_editbox_command(widget, COMMAND_UPDATE, (var_long)NULL);
		}
		break;
	case KEY_TAB:
		break;
	case KEY_ENTER:
		if (widget_is_editor(widget))
		{
			pxa = widget_get_xface_ptr(widget);
			if (is_null(pxa->text_wrap))
			{
				noti_editbox_command(widget, COMMAND_COMMIT, (var_long)NULL);
			}
		}
		break;
	case KEY_ESC:
		if (ptd->b_lock)
			break;
		hand_textor_escape(&ptd->textor);
		break;
	case KEY_LEFT:
		hand_textor_left(&ptd->textor);
		break;
	case KEY_RIGHT:
		hand_textor_right(&ptd->textor);
		break;
	case KEY_UP:
		hand_textor_up(&ptd->textor);
		break;
	case KEY_DOWN:
		hand_textor_down(&ptd->textor);
		break;
	case _T('c'):
	case _T('C'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			widget_copy(widget);
		}
		break;
	case _T('x'):
	case _T('X'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			widget_cut(widget);
		}
		break;
	case _T('v'):
	case _T('V'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			widget_paste(widget);
		}
		break;
	case _T('z'):
	case _T('Z'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			widget_undo(widget);
		}
		break;
	}
}

void hand_editbox_char(res_win_t widget, tchar_t ch)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (ptd->b_lock)
		return;

	if (ch == KEY_BACK)
		return;

	if (ch != KEY_ENTER && ch != KEY_TAB && ch > 0 && ch < 32)
		return;

	if (!ptd->chs)
	{
		ptd->chs = xschs(&ch);

		xsncpy(ptd->pch, &ch, 1);
		ptd->chs--;

		if (ptd->chs)
			return;
	}
	else
	{
		xsncat(ptd->pch, &ch, 1);
		ptd->chs--;

		if (ptd->chs)
			return;
	}

	if (_TEXTOR_PRESS_ACCEPT == hand_textor_word(&ptd->textor, ptd->pch))
	{
		noti_editbox_command(widget, COMMAND_UPDATE, (var_long)NULL);

		if (ptd->b_auto)
		{
			_editbox_auto_resize(widget);
		}
	}
}

void hand_editbox_copy(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	hand_textor_copy(&ptd->textor);
}

void hand_editbox_cut(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (ptd->b_lock)
		return;

	if (_TEXTOR_PRESS_ACCEPT == hand_textor_cut(&ptd->textor))
	{
		noti_editbox_command(widget, COMMAND_UPDATE, (var_long)NULL);

		if (ptd->b_auto)
		{
			_editbox_auto_resize(widget);
		}
	}
}

void hand_editbox_paste(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (ptd->b_lock)
		return;

	if (_TEXTOR_PRESS_ACCEPT == hand_textor_paste(&ptd->textor))
	{
		noti_editbox_command(widget, COMMAND_UPDATE, (var_long)NULL);

		if (ptd->b_auto)
		{
			_editbox_auto_resize(widget);
		}
	}
}

void hand_editbox_undo(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (ptd->b_lock)
		return;

	if (_TEXTOR_PRESS_ACCEPT == hand_textor_undo(&ptd->textor))
	{
		noti_editbox_command(widget, COMMAND_UPDATE, (var_long)NULL);

		if (ptd->b_auto)
		{
			_editbox_auto_resize(widget);
		}
	}
}

void hand_editbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	hand_textor_lbutton_down(&ptd->textor, pxp);
}

void hand_editbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	hand_textor_lbutton_up(&ptd->textor, pxp);
}

void hand_editbox_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

}

void hand_editbox_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (editbox_is_multiline(widget))
		hand_textor_selectline(&ptd->textor);
	else
		hand_textor_selectall(&ptd->textor);
}

void hand_editbox_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xpoint_t xp;

	xp.x = pxp->x;
	xp.y = pxp->y;
	widget_client_to_screen(widget, &xp);

	textor_menu(widget, &xp, WD_LAYOUT_LEFTBOTTOM);
}

void hand_editbox_mousemove(res_win_t widget, dword_t mk, const xpoint_t* ppt)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	hand_textor_mousemove(&ptd->textor, mk, ppt);
}

void hand_editbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	hand_textor_size(&ptd->textor, code, prs);
}

void hand_editbox_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	hand_textor_scroll(&ptd->textor, bHorz, nLine);
}

void hand_editbox_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (cid == IDC_EDITMENU)
	{
		switch (code)
		{
		case COMMAND_COPY:
			widget_copy(widget);
			break;
		case COMMAND_CUT:
			widget_cut(widget);
			break;
		case COMMAND_PASTE:
			widget_paste(widget);
			break;
		case COMMAND_UNDO:
			widget_undo(widget);
			break;
		}

		widget_close((res_win_t)data, 1);
	}
}

void hand_editbox_erase(res_win_t widget, res_ctx_t dc)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

}

void hand_editbox_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xrect_t xr;
	xcolor_t xc;

	hand_textor_paint(&ptd->textor, dc, pxr);

	if (ptd->b_auto)
	{
		widget_get_client_rect(widget, &xr);

		parse_xcolor(&xc, DEF_DISABLE_COLOR);
		draw_feed_raw(dc, &xc, &xr, ALPHA_SOLID);
	}
}

/************************************************************************************************/

res_win_t editbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };
	res_win_t wt;

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_editbox_create)
		EVENT_ON_DESTROY(hand_editbox_destroy)

		EVENT_ON_ERASE(hand_editbox_erase)
		EVENT_ON_PAINT(hand_editbox_paint)

		EVENT_ON_SIZE(hand_editbox_size)
		EVENT_ON_SCROLL(hand_editbox_scroll)

		EVENT_ON_KEYDOWN(hand_editbox_keydown)
		EVENT_ON_CHAR(hand_editbox_char)

		EVENT_ON_MOUSE_MOVE(hand_editbox_mousemove)
		EVENT_ON_LBUTTON_DBCLICK(hand_editbox_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_editbox_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_editbox_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_editbox_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_editbox_rbutton_up)

		EVENT_ON_MENU_COMMAND(hand_editbox_menu_command)

		EVENT_ON_SET_FOCUS(hand_editbox_set_focus)
		EVENT_ON_KILL_FOCUS(hand_editbox_kill_focus)

		EVENT_ON_COPY(hand_editbox_copy)
		EVENT_ON_CUT(hand_editbox_cut)
		EVENT_ON_PASTE(hand_editbox_paste)
		EVENT_ON_UNDO(hand_editbox_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	wt = widget_create(NULL, style, pxr, widget, &ev);
	if (!wt)
		return NULL;

	/*widget_get_xface(wt, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_LINEBREAK);
	widget_set_xface(wt, &xa);*/

	return wt;
}

void editbox_redraw(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	hand_textor_redraw(&ptd->textor);
}

void editbox_selectall(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	hand_textor_selectall(&ptd->textor);
}

void editbox_set_text(res_win_t widget, const tchar_t* text)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	varstr_cpy((string_t)ptd->textor.data, text, -1);

	if (ptd->b_auto)
	{
		_editbox_auto_resize(widget);
		hand_textor_end(&ptd->textor);
	}
	else
	{
		hand_textor_end(&ptd->textor);
		editbox_redraw(widget);
	}

	noti_editbox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

int editbox_get_text(res_win_t widget, tchar_t* buf, int max)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	string_t vs;
	int len;

	XDL_ASSERT(ptd != NULL);

	vs = (string_t)ptd->textor.data;

	len = varstr_len(vs);
	if (buf)
	{
		len = (len < max) ? len : max;
		xsncpy(buf, varstr_ptr(vs), len);
	}

	return len;
}

const tchar_t* editbox_get_text_ptr(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	string_t vs;

	XDL_ASSERT(ptd != NULL);

	vs = (string_t)ptd->textor.data;

	return varstr_ptr(vs);
}

bool_t editbox_is_select(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	return textor_is_select(&ptd->textor);
}

bool_t editbox_is_multiline(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xface_t xa = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xface(widget, &xa);

	return is_null(xa.text_wrap) ? 0 : 1;
}

void editbox_auto_size(res_win_t widget, bool_t bSize)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_auto = bSize;

	if (bSize)
	{
		_editbox_auto_resize(widget);
	}
}

void editbox_set_lock(res_win_t widget, bool_t bLock)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t editbox_get_lock(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

res_win_t editbox_create_keybox(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	res_win_t editbox, keybox;
	xrect_t xr = { 0 };

	editbox = editbox_create(widget, style, pxr);

	widget_get_window_rect(editbox, &xr);
	xr.y += xr.h;

	keybox = keybox_create(editbox, WD_STYLE_POPUP | WD_STYLE_NOACTIVE, &xr);

	keybox_popup_size(keybox, RECTSIZE(&xr));

	widget_size(keybox, RECTSIZE(&xr));
	widget_take(keybox, (int)WD_ZORDER_TOP);
	widget_show(keybox, WD_SHOW_NORMAL);

	widget_set_user_prop(editbox, XDCKEYBOX, (var_long)keybox);

	return editbox;
}

res_win_t editbox_get_keybox(res_win_t widget)
{
	return (res_win_t)widget_get_user_prop(widget, XDCKEYBOX);
}

