/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc fire text control document

	@module	firetext.c | fire text control implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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
#include "xdcwidg.h"

/*************************************************************************************/
void _firetext_send_notice(res_win_t widget, int code, void* data)
{
	NOTICE_EDITOR ne = { 0 };

	ne.widget = widget;
	ne.id = widget_get_user_id(widget);
	ne.code = code;
	ne.data = data;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&ne);
}
/*************************************************************************************/

void hand_firetext_destroy(res_win_t widget, dword_t subid, void* delta)
{
	if (subid != IDS_FIRETEXT)
		return ;

	widget_del_subproc(widget, IDS_FIRETEXT);
}

int hand_firetext_command(res_win_t widget, int code, int cid, void* data, dword_t subid, void* delta)
{
	tchar_t* text;
	int len;

	if (subid != IDS_FIRETEXT)
		return 0;

	switch (code)
	{
	case COMMAND_COMMIT:
		len = textbox_get_text(widget, NULL, MAX_LONG);
		text = xsalloc(len + 1);
		textbox_get_text(widget, text, len);

		_firetext_send_notice(widget, FE_COMMIT, (void*)text);
		xsfree(text);
		return 1;
	case COMMAND_ROLLBACK:
		_firetext_send_notice(widget, FE_ROLLBACK, NULL);
		return 1;
	case COMMAND_UPDATE:

		return 1;
	}

	return 0;
}

int hand_firetext_killfocus(res_win_t widget, res_win_t org, dword_t subid, void* delta)
{
	int cid;

	if (subid != IDS_FIRETEXT)
		return 0;

	cid = widget_get_user_id(widget);
	widget_post_command(widget, COMMAND_COMMIT, cid, NULL);

	return 0;
}
/*************************************************************************************/

res_win_t firetext_create(res_win_t widget, const xrect_t* pxr, const tchar_t* text, const dev_obj_t* pob)
{
	res_win_t editor;
	if_subproc_t ev = { 0 };

	ev.sub_on_destroy = hand_firetext_destroy;
	ev.sub_on_command = hand_firetext_command;
	ev.sub_on_kill_focus = hand_firetext_killfocus;

	editor = textbox_create(widget, WD_STYLE_TOOLBOX, pxr);
	widget_set_user_id(editor, IDC_TEXTBOX);
	widget_set_subproc(editor, IDS_FIRETEXT, &ev);
	widget_update_window(editor);

	if (pob)
	{
		widget_set_xfont(editor, &pob->xf);
		widget_set_xface(editor, &pob->xa);
		widget_set_xbrush(editor, &pob->xb);
		widget_set_xpen(editor, &pob->xp);
	}

	textbox_set_text(editor, text);

	return editor;
}
