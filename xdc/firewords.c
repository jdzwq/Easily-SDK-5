/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc fire control document

	@module	firewords.c | implement file

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
	res_win_t ctrl;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;

	if (nKey == KEY_TAB)
	{
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		return 1;
	}
	else if (nKey == KEY_ENTER)
	{
		widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		return 1;
	}
	else if (nKey == KEY_ESC)
	{
		widget_post_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, (var_long)NULL);
		return 1;
	}
	else if (nKey == VK_DOWN)
	{
		if (widget_is_valid(ctrl))
		{
			wordsbox_tabskip(ctrl,WD_TAB_DOWN); 
			return 1;
		}
	}
	else if (nKey == VK_UP)
	{
		if (widget_is_valid(widget))
		{
			wordsbox_tabskip(ctrl,WD_TAB_UP); 
			return 1;
		}
	}

	return 0;
}

static int sub_editbox_char(res_win_t widget, tchar_t ch, uid_t subid, var_long delta)
{
	res_win_t ctrl;
	int index;
	link_t_ptr plk;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;

	if (widget_is_valid(ctrl))
	{
		if (ch >= _T('1') && ch <= _T('9'))
		{
			index = (int)(ch - _T('1'));
			plk = wordsbox_seek(ctrl, index);
			if (plk)
			{
				widget_post_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
				return 1;
			}
		}
	}

	return 0;
}

static int sub_editbox_scroll(res_win_t widget, bool_t bHorz, long nLine, uid_t subid, var_long delta)
{
	res_win_t ctrl;

	if (subid != IDS_EDITBOX)
		return 0;

	if (bHorz)
		return 1;

	ctrl = (res_win_t)delta;

	if (widget_is_valid(ctrl))
	{
		if (nLine < 0)
		{
			wordsbox_tabskip(ctrl,WD_TAB_DOWN); 
		}
		else
		{
			wordsbox_tabskip(ctrl,WD_TAB_UP); 
		}
	}

	return 1;
}

static int sub_editbox_self_command(res_win_t widget, int code, var_long data, uid_t subid, var_long delta)
{
	res_win_t ctrl;
	const tchar_t* text;

	if (subid != IDS_EDITBOX)
		return 0;

	ctrl = (res_win_t)delta;

	switch (code)
	{
	case COMMAND_UPDATE:
		if (widget_is_valid(ctrl) && !editbox_is_select(widget))
		{
			text = editbox_get_text_ptr(widget);
			wordsbox_filter(ctrl, text);

			if (!is_null(text))
			{
				wordsbox_set_focus_item(ctrl, LINK_FIRST);
			}
		}
		return 1;
	case COMMAND_COLOR:
		if (widget_is_valid(ctrl))
		{
			widget_set_color_mode(ctrl, (clr_mod_t*)data);
			widget_update(ctrl);
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
	res_win_t ctrl;
	//res_win_t keybox;

	if (subid != IDS_EDITBOX)
		return 0;

	/*keybox = editbox_get_keybox(widget);
	if (widget_is_valid(keybox))
	{
		if (show)
			widget_show(keybox, WD_SHOW_NORMAL);
		else
			widget_show(keybox, WD_SHOW_HIDE);
	}*/

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

static void sub_editbox_unsubbing(res_win_t widget, uid_t subid, var_long delta)
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
/***************************************************************************************************/
static int sub_wordsbox_self_command(res_win_t widget, int code, var_long data, uid_t subid, var_long delta)
{
	res_win_t editbox;

	if (subid != IDS_WORDSBOX)
		return 0;

	editbox = (res_win_t)delta;

	switch (code)
	{
	case COMMAND_UPDATE:
		return 1;
	case COMMAND_CHANGE:
		if (widget_is_valid(editbox))
		{
			widget_post_key(editbox, KEY_ENTER);
		}
		return 1;
	}

	return 0;
}

static void sub_wordsbox_unsubbing(res_win_t widget, uid_t subid, var_long delta)
{
	if (subid != IDS_WORDSBOX)
		return;

	widget_del_subproc(widget, IDS_WORDSBOX);
}
/****************************************************************************************************/

res_win_t firewords_create(res_win_t widget, const xrect_t* pxr, link_t_ptr data)
{
	res_win_t editor,ctrlbox = NULL;
	xsize_t xs;
	xrect_t xr_ed, xr = { 0 };

	if_subproc_t ev = { 0 };
	xface_t xa = { 0 };

	ev.sub_on_scroll = sub_editbox_scroll;
	ev.sub_on_keydown = sub_editbox_keydown;
	ev.sub_on_char = sub_editbox_char;
	ev.sub_on_self_command = sub_editbox_self_command;
	ev.sub_on_unsubbing = sub_editbox_unsubbing;
	ev.sub_on_show = sub_editbox_show;

	/*if (widget_get_touch_mode(widget))
	{
		editor = editbox_create_keybox(widget, WD_STYLE_CONTROL | WD_STYLE_EDITOR, pxr);
	}
	else*/
	{
		editor = editbox_create(widget, WD_STYLE_CONTROL | WD_STYLE_EDITOR, pxr);
	}
	widget_set_user_id(editor, IDC_EDITBOX);
	widget_set_subproc(editor, IDS_EDITBOX, &ev);
	widget_set_imm(editor, 0);

	widget_get_xface(editor, &xa);
	xscpy(xa.text_wrap, NULL);
	widget_set_xface(editor, &xa);

	if (data)
	{
		widget_get_window_rect(editor, &xr_ed);

		xr.x = xr_ed.x;
		xr.y = xr_ed.y + xr_ed.h;

		XDL_ASSERT(is_words_table(data));

		ctrlbox = wordsbox_create(widget, WD_STYLE_POPUP | WD_STYLE_BORDER | WD_STYLE_VSCROLL | WD_STYLE_NOACTIVE, &xr);

		widget_set_user_id(ctrlbox, IDC_WORDSBOX);
		widget_set_owner(ctrlbox, editor);
		wordsbox_set_data(ctrlbox, data);

		xmem_zero((void*)&ev, sizeof(if_subproc_t));

		ev.sub_on_self_command = sub_wordsbox_self_command;
		ev.sub_on_unsubbing = sub_wordsbox_unsubbing;

		widget_set_subproc(ctrlbox, IDS_WORDSBOX, &ev);
		widget_set_subproc_delta(ctrlbox, IDS_WORDSBOX, (var_long)editor);
		widget_set_subproc_delta(editor, IDS_EDITBOX, (var_long)ctrlbox);

		widget_get_window_rect(ctrlbox, &xr);
		wordsbox_popup_size(ctrlbox, RECTSIZE(&xr));

		get_desktop_size(&xs);

		if (xr.x + xr.w > xs.cx)
		{
			xr.x = xs.cx - xr.w;
		}

		if (xr.y + xr.h > xs.cy)
		{
			xr.y = xr_ed.y - xr.h;
		}

		widget_move(ctrlbox, RECTPOINT(&xr));
		widget_size(ctrlbox, RECTSIZE(&xr));
		widget_update(ctrlbox);
	}

	return editor;
}

link_t_ptr firewords_get_data(res_win_t widget)
{
	res_win_t ctrl;

	ctrl = (res_win_t)widget_get_subproc_delta(widget, IDS_EDITBOX);

	if (!widget_is_valid(ctrl))
		return NULL;

	return wordsbox_get_data(ctrl);
}

link_t_ptr firewords_get_item(res_win_t widget)
{
	res_win_t ctrl;

	ctrl = (res_win_t)widget_get_subproc_delta(widget, IDS_EDITBOX);

	if (!widget_is_valid(ctrl))
		return NULL;

	return wordsbox_get_focus_item(ctrl);
}
