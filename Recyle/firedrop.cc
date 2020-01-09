/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc firedrop editor document

	@module	firedrop.c | firedrop editor widnows implement file

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

#include "xdceditor.h"
#include "xdcbox.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcgdi.h"
#include "xdcres.h"
#include "xdcwidg.h"


/*********************************************************************************/
static int sub_editbox_keydown(res_win_t widget, int nKey, dword_t subid, void* delta)
{
	res_win_t dropbox;
	LINKPTR ent;

	if (subid != IDS_EDITBOX)
		return 0;

	dropbox = (res_win_t)delta;

	switch (nKey)
	{
	case KEY_TAB:
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
		return 1;
	case KEY_ENTER:
		if (widget_is_valid(dropbox))
		{
			ent = dropbox_get_focus_item(dropbox);
			if (ent)
			{
				editbox_set_text(widget, get_hash_entity_val_ptr(ent));
			}
		}

		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
		return 1;
	case KEY_ESC:
		widget_post_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, NULL);
		return 1;
	case KEY_DOWN:
		dropbox_tabskip((res_win_t)delta, NF_DOWN);
		return 1;
	case KEY_UP:
		dropbox_tabskip((res_win_t)delta, NF_UP);
		return 1;
	case KEY_SPACE:
		editbox_set_text(widget, NULL);
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
		return 1;
	}

	return 0;
}

static int sub_editbox_scroll(res_win_t widget, bool_t bHorz, long nLine, dword_t subid, void* delta)
{
	res_win_t dropbox;

	if (subid != IDS_EDITBOX)
		return 0;

	if (bHorz)
		return 0;

	dropbox = (res_win_t)delta;

	if (widget_is_valid(dropbox))
	{
		if (nLine < 0)
			dropbox_tabskip((res_win_t)delta, NF_DOWN);
		else
			dropbox_tabskip((res_win_t)delta, NF_UP);
	}

	return 1;
}

static int sub_editbox_command_color(res_win_t widget, void* data, dword_t subid, void* delta)
{
	res_win_t dropbox;

	if (subid != IDS_EDITBOX)
		return 0;

	dropbox = (res_win_t)delta;

	if (widget_is_valid(dropbox))
	{
		widget_send_command(dropbox, COMMAND_COLOR, IDC_PARENT, data);
	}

	return 0;
}

static int sub_editbox_self_command(res_win_t widget, int code, void* data, dword_t subid, void* delta)
{
	res_win_t dropbox;

	if (subid != IDS_EDITBOX)
		return 0;

	dropbox = (res_win_t)delta;

	switch (code)
	{
	case COMMAND_UPDATE:
		if (widget_is_valid(dropbox))
		{
			dropbox_filter(dropbox, editbox_get_text_ptr(widget));
		}
		return 1;
	}

	return 0;
}

static int sub_editbox_show(res_win_t widget, bool_t show, dword_t subid, void* delta)
{
	res_win_t dropbox;

	if (subid != IDS_EDITBOX)
		return 0;

	dropbox = (res_win_t)delta;

	if (widget_is_valid(dropbox))
	{
		if (show)
			widget_show(dropbox, WD_SHOW_NORMAL);
		else
			widget_show(dropbox, WD_SHOW_HIDE);
	}

	return 1;
}

static void sub_editbox_destroy(res_win_t widget, dword_t subid, void* delta)
{
	res_win_t dropbox;

	if (subid != IDS_EDITBOX)
		return;

	dropbox = (res_win_t)delta;
	if (widget_is_valid(dropbox))
	{
		widget_destroy(dropbox);
	}

	widget_del_subproc(widget, IDS_EDITBOX);
}

//////////////////////////////////////////////////////////////////////////////////////

static int sub_dropbox_self_command(res_win_t widget, int code, void* data, dword_t subid, void* delta)
{
	res_win_t editbox;

	if (subid != IDS_DROPBOX)
		return 0;

	editbox = (res_win_t)delta;

	switch (code)
	{
	case COMMAND_UPDATE:
		return 1;
	case COMMAND_SELECT:
		if (widget_is_valid(editbox))
		{
			widget_post_key(editbox, KEY_ENTER);
		}
		return 1;
	}

	return 0;
}

static void sub_dropbox_destroy(res_win_t widget, dword_t subid, void* delta)
{
	if (subid != IDS_DROPBOX)
		return;

	widget_del_subproc(widget, IDS_DROPBOX);
}

/*********************************************************************************************************/

res_win_t firedrop_create(res_win_t widget, const xrect_t* pxr, LINKPTR data)
{
	res_win_t editor,dropbox;
	xrect_t xr;
	xsize_t xs, xs_scr;
	dword_t ws;

	if_subproc_t ev = { 0 };
	xface_t xa = { 0 };

	ev.sub_on_scroll = sub_editbox_scroll;
	ev.sub_on_keydown = sub_editbox_keydown;
	ev.sub_on_self_command = sub_editbox_self_command;
	ev.sub_on_command_color = sub_editbox_command_color;
	ev.sub_on_show = sub_editbox_show;
	ev.sub_on_destroy = sub_editbox_destroy;

	editor = editbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_EDITOR, pxr);
	widget_set_user_id(editor, IDC_EDITBOX);
	widget_set_subproc(editor, IDS_EDITBOX, &ev);

	widget_get_xface(editor, &xa);
	xscpy(xa.text_wrap, NULL);
	widget_set_xface(editor, &xa);

	widget_get_window_rect(editor, &xr);

	ws = WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_VSCROLL | WD_STYLE_NOACTIVE;

	dropbox = dropbox_create(widget, ws, &xr);
	if (!widget_is_valid(dropbox))
		return editor;

	dropbox_set_data(dropbox, data);

	widget_set_user_id(dropbox, IDC_DROPBOX);
	widget_set_owner(dropbox, editor);

	xmem_zero((void*)&ev, sizeof(if_subproc_t));
	
	ev.sub_on_self_command = sub_dropbox_self_command;
	ev.sub_on_destroy = sub_dropbox_destroy;

	widget_set_subproc(dropbox, IDS_DROPBOX, &ev);

	dropbox_popup_size(dropbox, &xs);

	get_screen_size(&xs_scr);

	if (xr.x + xs.cx > xs_scr.cx)
	{
		xr.x = (xr.x + xr.w) - xs.cx - 2;
		xr.w = xs.cx;
	}

	if (xr.y + xs.cy > xs_scr.cy)
	{
		xr.y = xr.y - xs.cy - 2;
		xr.h = xs.cy;
	}
	else
	{
		xr.y = xr.y + xr.h + 1;
		xr.h = xs.cy;
	}

	widget_move(dropbox, RECTPOINT(&xr));
	widget_size(dropbox, RECTSIZE(&xr));

	widget_set_subproc_delta(editor, IDS_EDITBOX, (void*)dropbox);
	widget_set_subproc_delta(dropbox, IDS_DROPBOX, (void*)editor);

	return editor;
}

LINKPTR firedrop_get_data(res_win_t widget)
{
	res_win_t dropbox;

	dropbox = (res_win_t)widget_get_subproc_delta(widget, IDS_EDITBOX);

	if (!widget_is_valid(dropbox))
		return NULL;

	return dropbox_get_data(dropbox);
}

LINKPTR firedrop_get_item(res_win_t widget)
{
	res_win_t dropbox;

	dropbox = (res_win_t)widget_get_subproc_delta(widget, IDS_EDITBOX);

	if (!widget_is_valid(dropbox))
		return NULL;

	return dropbox_get_focus_item(dropbox);
}