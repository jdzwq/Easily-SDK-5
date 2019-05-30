/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc fire date control document

	@module	firedate.c | fire date control implement file

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
	res_win_t datebox;
	xdate_t dt;
	const tchar_t* text;

	if (subid != IDS_EDITBOX)
		return 0;

	datebox = (res_win_t)delta;

	switch (code)
	{
	case COMMAND_UPDATE:
		if (widget_is_valid(datebox))
		{
			text = editbox_get_text_ptr(widget);
			if (is_null(text))
				get_loc_date(&dt);
			else
				parse_date(&dt, text);

			datebox_set_date(datebox, &dt);
		}
		return 1;
	case COMMAND_COLOR:
		if (widget_is_valid(datebox))
		{
			widget_set_color_mode(datebox, (clr_mod_t*)data);
			widget_update_window(datebox);
			widget_update_client(datebox);
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
	res_win_t datebox;

	if (subid != IDS_EDITBOX)
		return 0;

	datebox = (res_win_t)delta;

	if (widget_is_valid(datebox))
	{
		if (show)
			widget_show(datebox, WD_SHOW_NORMAL);
		else
			widget_show(datebox, WD_SHOW_HIDE);
	}

	return 1;
}

static void sub_editbox_unsubbing(res_win_t widget, uid_t subid, var_long delta)
{
	res_win_t datebox;

	if (subid != IDS_EDITBOX)
		return;

	datebox = (res_win_t)delta;
	if (widget_is_valid(datebox))
	{
		widget_destroy(datebox);
	}

	widget_del_subproc(widget, IDS_EDITBOX);
}

/////////////////////////////////////////////////////////////////////////////
static int sub_datebox_self_command(res_win_t widget, int code, var_long data, uid_t subid, var_long delta)
{
	res_win_t editbox;
	tchar_t token[DATE_LEN + 1] = { 0 };
	xdate_t dt = { 0 };

	if (subid != IDS_DATEBOX)
		return 0;

	editbox = (res_win_t)delta;

	switch (code)
	{
	case COMMAND_UPDATE:
		if (widget_is_valid(editbox))
		{
			datebox_get_date(widget, &dt);

			format_date(&dt, token);
			editbox_set_text(editbox, token);
		}
		return 1;
	case COMMAND_CHANGE:
		if (widget_is_valid(editbox))
		{
			widget_post_key(editbox, KEY_ENTER);
		}
		break;
	}

	return 0;
}

static void sub_datebox_unsubbing(res_win_t widget, uid_t subid, var_long delta)
{
	if (subid != IDS_DATEBOX)
		return;

	widget_del_subproc(widget, IDS_DATEBOX);
}

/*************************************************************************************/
res_win_t firedate_create(res_win_t widget, const xrect_t* pxr)
{
	res_win_t editor, datebox;
	xrect_t xr_ed, xr = { 0 };
	xsize_t xs;

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
	datebox = datebox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_NOACTIVE, &xr);

	widget_set_subproc_delta(editor, IDS_EDITBOX, (var_long)datebox);

	widget_set_user_id(datebox, IDC_DATEBOX);
	widget_set_owner(datebox, editor);

	xmem_zero((void*)&ev, sizeof(if_subproc_t));

	ev.sub_on_self_command = sub_datebox_self_command;
	ev.sub_on_unsubbing = sub_datebox_unsubbing;

	widget_set_subproc(datebox, IDS_DATEBOX, &ev);
	widget_set_subproc_delta(datebox, IDS_DATEBOX, (var_long)editor);

	widget_get_window_rect(datebox, &xr);
	datebox_popup_size(datebox, RECTSIZE(&xr));

	get_desktop_size(&xs);

	if (xr.x + xr.w > xs.cx)
	{
		xr.x = xs.cx - xr.w;
	}

	if (xr.y + xr.h > xs.cy)
	{
		xr.y = xr_ed.y - xr.h;
	}

	widget_move(datebox, RECTPOINT(&xr));
	widget_size(datebox, RECTSIZE(&xr));

	return editor;
}
