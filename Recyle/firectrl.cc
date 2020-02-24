/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc fire control document

	@module	firectrl.c | fire control implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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
#include "xdcbox.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcgdi.h"
#include "xdcres.h"
#include "xdcwidg.h"


/*************************************************************************************/

static int sub_editbox_keydown(res_win_t widget, int nKey, dword_t subid, void* delta)
{
	res_win_t ctrl;
	int uid;
	LINKPTR plk;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;
	uid = widget_get_user_id(ctrl);

	if (nKey == KEY_TAB)
	{
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
		return 1;
	}
	else if (nKey == KEY_ENTER)
	{
		plk = NULL;
		if (widget_is_valid(ctrl))
		{
			if (uid == IDC_WORDSBOX)
				plk = wordsctrl_get_focus_item(ctrl); 
			else if (uid == IDC_GRIDBOX)
				plk = gridctrl_get_focus_row(ctrl);
		}
		if (plk)
			widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
		else
			widget_post_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, NULL);
		return 1;
	}
	else if (nKey == KEY_ESC)
	{
		widget_post_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, NULL);
		return 1;
	}
	else if (nKey == KEY_SPACE)
	{
		if (widget_is_valid(ctrl))
		{
			if (uid == IDC_WORDSBOX)
				wordsctrl_set_focus_item(ctrl, NULL); 
			else if (uid == IDC_GRIDBOX)
				gridctrl_set_focus_cell(ctrl, NULL, NULL);
		}
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
		return 1;
	}
	else if (nKey == VK_DOWN)
	{
		if (widget_is_valid(ctrl))
		{
			if (uid == IDC_WORDSBOX)
				wordsctrl_tabskip(ctrl, NF_DOWN); 
			else if (uid == IDC_GRIDBOX)
				gridctrl_tabskip(ctrl, NF_DOWN);
			return 1;
		}
	}
	else if (nKey == VK_UP)
	{
		if (widget_is_valid(widget))
		{
			if (uid == IDC_WORDSBOX)
				wordsctrl_tabskip(ctrl, NF_UP); 
			else if (uid == IDC_GRIDBOX)
				gridctrl_tabskip(ctrl, NF_UP);
			return 1;
		}
	}

	return 0;
}

static int sub_editbox_scroll(res_win_t widget, bool_t bHorz, long nLine, dword_t subid, void* delta)
{
	res_win_t ctrl;
	int uid;

	if (subid != IDS_EDITBOX)
		return 0;

	if (bHorz)
		return 0;

	ctrl = (res_win_t)delta;
	uid = widget_get_user_id(ctrl);

	if (widget_is_valid(ctrl))
	{
		if (nLine < 0)
		{
			if (uid == IDC_WORDSBOX)
				wordsctrl_tabskip(ctrl, NF_DOWN); 
			else if (uid == IDC_GRIDBOX)
				gridctrl_tabskip(ctrl, NF_DOWN);
			return 1;
		}
		else
		{
			if (uid == IDC_WORDSBOX)
				wordsctrl_tabskip(ctrl, NF_UP); 
			else if (uid == IDC_GRIDBOX)
				gridctrl_tabskip(ctrl, NF_UP);
		}
	}

	return 1;
}

static int sub_editbox_command_color(res_win_t widget, void* data, dword_t subid, void* delta)
{
	res_win_t ctrl;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;

	if (widget_is_valid(ctrl))
	{
		widget_send_command(ctrl, COMMAND_COLOR, IDC_PARENT, data);
	}
	return 0;
}

static int sub_editbox_parent_command(res_win_t widget, int code, void* data, dword_t subid, void* delta)
{
	res_win_t ctrl;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;

	switch (code)
	{
	case COMMAND_COMMIT:
		widget_send_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
		return 1;
	case COMMAND_ROLLBACK:
		widget_send_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, NULL);
		return 1;
	}

	return 0;
}

static int sub_editbox_self_command(res_win_t widget, int code, void* data, dword_t subid, void* delta)
{
	res_win_t ctrl;
	int uid;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;

	uid = widget_get_user_id(ctrl);

	switch (code)
	{
	case COMMAND_UPDATE:
		if (widget_is_valid(ctrl))
		{
			if (uid == IDC_WORDSBOX)
				wordsctrl_filter(ctrl, editbox_get_text_ptr(widget)); 
			else if (uid == IDC_GRIDBOX)
				gridctrl_filter(ctrl, editbox_get_text_ptr(widget));
		}
		return 1;
	}

	return 0;
}

static int sub_editbox_killfocus(res_win_t widget, res_win_t wnext, dword_t subid, void* delta)
{
	if (subid != IDS_EDITBOX)
		return 0;

	widget_send_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, NULL);

	return 0;
}

static int sub_editbox_notice(res_win_t widget, NOTICE* pnt, dword_t subid, void* delta)
{
	res_win_t ctrl;
	int uid;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;

	uid = widget_get_user_id(ctrl);

	if (uid == IDC_WORDSBOX)
	{
		switch (pnt->code)
		{
		case NC_WORDSLBCLK:
			widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
			return 1;
		}
	}
	else if (uid == IDC_GRIDBOX)
	{
		switch (pnt->code)
		{
		case NC_GRIDLBCLK:
			widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
			return 1;
		}
	}

	return 0;
}

static int sub_editbox_show(res_win_t widget, bool_t show, dword_t subid, void* delta)
{
	res_win_t ctrl;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;

	if (widget_is_valid(ctrl))
	{
		if (show)
			widget_show(ctrl, WD_SHOW_NORMAL);
		else
			widget_show(ctrl, WD_SHOW_HIDE);
	}

	return 1;
}

static void sub_editbox_destroy(res_win_t widget, dword_t subid, void* delta)
{
	res_win_t ctrl;

	if (subid != IDS_EDITBOX)
		return;

	ctrl = (res_win_t)delta;

	if (widget_is_valid(ctrl))
	{
		widget_destroy(ctrl);
	}

	widget_del_subproc(widget, IDS_EDITBOX);
}

/****************************************************************************************************/

res_win_t firectrl_create(res_win_t widget, const xrect_t* pxr, link_t_ptr data)
{
	res_win_t editor,ctrlbox = NULL;
	xsize_t xs, xs_scr;
	xrect_t xr;
	dword_t ws;

	if_subproc_t ev = { 0 };
	xface_t xa = { 0 };

	ev.sub_on_scroll = sub_editbox_scroll;
	ev.sub_on_keydown = sub_editbox_keydown;
	ev.sub_on_kill_focus = sub_editbox_killfocus;
	ev.sub_on_destroy = sub_editbox_destroy;
	ev.sub_on_command_color = sub_editbox_command_color;
	ev.sub_on_parent_command = sub_editbox_parent_command;
	ev.sub_on_self_command = sub_editbox_self_command;
	ev.sub_on_notice = sub_editbox_notice;
	ev.sub_on_show = sub_editbox_show;

	editor = editbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_NOSCROLL, pxr);
	widget_set_user_id(editor, IDC_EDITBOX);
	widget_set_subproc(editor, IDS_EDITBOX, &ev);
	widget_set_imm(editor, 0);

	widget_get_xface(editor, &xa);
	xscpy(xa.text_wrap, NULL);
	widget_set_xface(editor, &xa);

	widget_get_window_rect(editor, &xr);

	ws = WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_NOACTIVE;

	if (is_words_table(data))
	{
		ctrlbox = wordsctrl_create(NULL, ws, &xr, widget);
		widget_set_user_id(ctrlbox, IDC_WORDSBOX);
		widget_set_owner(ctrlbox, editor);
		wordsctrl_attach(ctrlbox, data);
		wordsctrl_popup_size(ctrlbox, &xs);
	}
	else if (is_grid_doc(data))
	{
		ctrlbox = gridctrl_create(NULL, ws, &xr, widget);
		widget_set_user_id(ctrlbox, IDC_GRIDBOX);
		widget_set_owner(ctrlbox, editor);
		gridctrl_attach(ctrlbox, data);
		gridctrl_popup_size(ctrlbox, &xs);
	}

	if (!widget_is_valid(ctrlbox))
		return editor;

	get_screen_size(&xs_scr);

	if (xs.cx > xs_scr.cx)
		xs.cx = xs_scr.cx;
	if (xs.cy > xs_scr.cy)
		xs.cy = xs_scr.cy;

	if (xr.x + xs.cx > xs_scr.cx)
	{
		xr.x = (xr.x + xr.w) - xs.cx;
		xr.w = xs.cx;
	}
	else
	{
		xr.w = xs.cx;
	}

	if (xr.y + xr.h + xs.cy > xs_scr.cy)
	{
		xr.y = xr.y + xr.h;
		xr.h = xs_scr.cy - xr.y;
	}
	else
	{
		xr.y = xr.y + xr.h;
		xr.h = xs.cy;
	}

	widget_move(ctrlbox, RECTPOINT(&xr));
	widget_size(ctrlbox, RECTSIZE(&xr));

	widget_set_subproc_delta(editor, IDS_EDITBOX, (void*)ctrlbox);

	return editor;
}

link_t_ptr firectrl_get_data(res_win_t widget)
{
	res_win_t ctrl;
	dword_t uid;

	ctrl = (res_win_t)widget_get_subproc_delta(widget, IDS_EDITBOX);

	if (!widget_is_valid(ctrl))
		return NULL;

	uid = widget_get_user_id(ctrl);

	if (uid == IDC_WORDSBOX)
		return wordsctrl_fetch(ctrl);
	else if (uid == IDC_GRIDBOX)
		return gridctrl_fetch(ctrl);
	else
		return NULL;
}

link_t_ptr firectrl_get_item(res_win_t widget)
{
	res_win_t ctrl;
	dword_t uid;

	ctrl = (res_win_t)widget_get_subproc_delta(widget, IDS_EDITBOX);

	if (!widget_is_valid(ctrl))
		return NULL;

	uid = widget_get_user_id(ctrl);

	if (uid == IDC_WORDSBOX)
		return wordsctrl_get_focus_item(ctrl);
	else if (uid == IDC_GRIDBOX)
		return gridctrl_get_focus_row(ctrl);
	else
		return NULL;
}