/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc fire num control document

	@module	firenum.c | fire numeric control implement file

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

#include "xdcfire.h"
#include "handler.h"
#include "winnc.h"
#include "xdcbox.h"


static int sub_editbox_keydown(res_win_t widget, int nKey, uid_t subid, var_long delta)
{
	if (subid != IDS_EDITBOX)
		return 0;

	switch (nKey)
	{
	case KEY_TAB:
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		return 1;
	case KEY_ENTER:
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		return 1;
	case KEY_ESC:
		widget_post_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, (var_long)NULL);
		return 1;
	case KEY_SPACE:
		editbox_set_text(widget, NULL);
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		return 1;
	}

	return 0;
}

static int sub_editbox_self_command(res_win_t widget, int code, var_long data, uid_t subid, var_long delta)
{
	res_win_t numbox;

	if (subid != IDS_EDITBOX)
		return 0;

	numbox = (res_win_t)delta;

	switch (code)
	{
	case COMMAND_UPDATE:
		return 1;
	case COMMAND_COLOR:
		if (widget_is_valid(numbox))
		{
			widget_set_color_mode(numbox, (clr_mod_t*)data);
			widget_update_window(numbox);
			widget_update_client(numbox);
		}
		return 1;
	case COMMAND_COMMIT:
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		return 1;
	case COMMAND_ROLLBACK:
		widget_post_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, (var_long)NULL);
		return 1;
	}

	return 0;
}

static int sub_editbox_show(res_win_t widget, bool_t show, uid_t subid, var_long delta)
{
	res_win_t numbox;

	if (subid != IDS_EDITBOX)
		return 0;

	numbox = (res_win_t)delta;

	if (widget_is_valid(numbox))
	{
		if (show)
			widget_show(numbox, WD_SHOW_NORMAL);
		else
			widget_show(numbox, WD_SHOW_HIDE);
	}

	return 1;
}

static void sub_editbox_unsubbing(res_win_t widget, uid_t subid, var_long delta)
{
	res_win_t numbox;

	if (subid != IDS_EDITBOX)
		return;

	numbox = (res_win_t)delta;
	if (widget_is_valid(numbox))
	{
		widget_destroy(numbox);
	}

	widget_del_subproc(widget, IDS_EDITBOX);
}

/*************************************************************************************/
res_win_t firenum_create(res_win_t widget, const xrect_t* pxr)
{
	res_win_t editor,numbox;
	xsize_t xs;
	xrect_t xr_ed, xr = { 0 };

	if_subproc_t ev = { 0 };
	xface_t xa = { 0 };

	ev.sub_on_keydown = sub_editbox_keydown;
	ev.sub_on_unsubbing = sub_editbox_unsubbing;
	ev.sub_on_self_command = sub_editbox_self_command;
	ev.sub_on_show = sub_editbox_show;

	editor = editbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_EDITOR, pxr);

	widget_set_user_id(editor, IDC_EDITBOX);
	widget_set_subproc(editor, IDS_EDITBOX, &ev);
	widget_set_imm(editor, 0);

	widget_get_xface(editor, &xa);
	xscpy(xa.text_wrap, NULL);
	widget_set_xface(editor, &xa);

	widget_get_window_rect(editor, &xr_ed);

	xr.x = xr_ed.x;
	xr.y = xr_ed.y + xr_ed.h;

	numbox = numbox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_NOACTIVE, &xr);
	
	widget_set_user_id(numbox, IDC_NUMBOX);
	widget_set_owner(numbox, editor);

	widget_set_subproc_delta(editor, IDS_EDITBOX, (var_long)numbox);

	widget_get_window_rect(numbox, &xr);
	numbox_popup_size(numbox, RECTSIZE(&xr));

	get_desktop_size(&xs);

	if (xr.x + xr.w > xs.cx)
	{
		xr.x = xs.cx - xr.w;
	}

	if (xr.y + xr.h > xs.cy)
	{
		xr.y = xr_ed.y - xr.h;
	}

	widget_move(numbox, RECTPOINT(&xr));
	widget_size(numbox, RECTSIZE(&xr));

	return editor;
}
