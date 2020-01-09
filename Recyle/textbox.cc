/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text control document

	@module	textbox.c | text control widnows implement file

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

#include "xdcbox.h"
#include "xdctextor.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcgdi.h"

typedef struct _textbox_delta_t{
	textor_t text;
	link_t_ptr line;
}textbox_delta_t;

#define GETTEXTBOXDELTA(ph) 	(textbox_delta_t*)widget_get_user_delta(ph)
#define SETTEXTBOXDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

static int _textbox_get_text(void* data, tchar_t* buf, int max)
{
	link_t_ptr text = (link_t_ptr)data;

	return format_text_doc(text, buf, max);
}

static void _textbox_set_text(void* data, const tchar_t* buf, int len)
{
	link_t_ptr text = (link_t_ptr)data;

	parse_text_doc(text, buf, len);
}

/********************************************************************************************/
int noti_textbox_owner(res_win_t widget, unsigned long code, link_t_ptr ptr, link_t_ptr nlk, void* data)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);
	NOTICE_TEXT nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.text = ptr;
	nf.line = nlk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);

	return nf.ret;
}

/********************************************************************************************/
int hand_textbox_create(res_win_t widget, void* data)
{
	textbox_delta_t* ptd;
	xhand_t canv;
	res_dc_t rdc;

	ptd = (textbox_delta_t*)xmem_alloc(sizeof(textbox_delta_t));
	memset((void*)ptd, 0, sizeof(textbox_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_set_canvas(widget, canv);

	ptd->text.widget = widget;
	ptd->text.data = NULL;
	ptd->text.pf_scan_text = scan_text_doc;
	ptd->text.pf_get_text = _textbox_get_text;
	ptd->text.pf_set_text = _textbox_set_text;

	SETTEXTBOXDELTA(widget, ptd);

	return 0;
}

void hand_textbox_destroy(res_win_t widget)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	hand_textor_destroy(&ptd->text);

	destroy_display_canvas(widget_get_canvas(widget));

	xmem_free(ptd);

	SETTEXTBOXDELTA(widget, 0);
}

void hand_textbox_set_focus(res_win_t widget, res_win_t wt)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_set_focus(&ptd->text, wt);
}

void hand_textbox_kill_focus(res_win_t widget, res_win_t wt)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_kill_focus(&ptd->text, wt);
}

void hand_textbox_keydown(res_win_t widget, int key)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);
	xface_t xa;

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	widget_get_xface(widget, &xa);

	switch (key)
	{
	case KEY_BACK:
		if (PRESS_ACCEPT == hand_textor_back(&ptd->text))
		{
			widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);

			if (ptd->line != (link_t_ptr)ptd->text.object)
			{
				ptd->line = (link_t_ptr)ptd->text.object;
				widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
			}
		}
		break;
	case KEY_DELETE:
		if (PRESS_ACCEPT == hand_textor_delete(&ptd->text))
		{
			widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);

			if (ptd->line != (link_t_ptr)ptd->text.object)
			{
				ptd->line = (link_t_ptr)ptd->text.object;
				widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
			}
		}
		break;
	case KEY_TAB:
		break;
	case KEY_ENTER:
		break;
	case KEY_ESC:
		hand_textor_escape(&ptd->text);
		break;
	case KEY_LEFT:
		hand_textor_left(&ptd->text);

		if (ptd->line != (link_t_ptr)ptd->text.object)
		{
			ptd->line = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
		}
		break;
	case KEY_RIGHT:
		hand_textor_right(&ptd->text);

		if (ptd->line != (link_t_ptr)ptd->text.object)
		{
			ptd->line = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
		}
		break;
	case KEY_UP:
		hand_textor_up(&ptd->text);

		if (ptd->line != (link_t_ptr)ptd->text.object)
		{
			ptd->line = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
		}
		break;
	case KEY_DOWN:
		hand_textor_down(&ptd->text);

		if (ptd->line != (link_t_ptr)ptd->text.object)
		{
			ptd->line = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
		}
		break;
	case _T('c'):
	case _T('C'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			hand_textor_copy(&ptd->text);
		}
		break;
	case _T('x'):
	case _T('X'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			if (PRESS_ACCEPT == hand_textor_cut(&ptd->text))
			{
				widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);

				if (ptd->line != (link_t_ptr)ptd->text.object)
				{
					ptd->line = (link_t_ptr)ptd->text.object;
					widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
				}
			}
		}
		break;
	case _T('v'):
	case _T('V'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			if (PRESS_ACCEPT == hand_textor_paste(&ptd->text))
			{
				widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);

				if (ptd->line != (link_t_ptr)ptd->text.object)
				{
					ptd->line = (link_t_ptr)ptd->text.object;
					widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
				}
			}
		}
		break;
	case _T('z'):
	case _T('Z'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			if (PRESS_ACCEPT == hand_textor_undo(&ptd->text))
			{
				widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);

				if (ptd->line != (link_t_ptr)ptd->text.object)
				{
					ptd->line = (link_t_ptr)ptd->text.object;
					widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
				}
			}
		}
		break;
	}
}

void hand_textbox_char(res_win_t widget, tchar_t ch)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	if (ch == KEY_BACK || ch == KEY_TAB || ch == KEY_ENTER || ch < 32)
		return;

	if (PRESS_ACCEPT == hand_textor_char(&ptd->text, ch))
	{
		widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);
	}
}

void hand_textbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_down(&ptd->text, pxp);

	if (ptd->line != (link_t_ptr)ptd->text.object)
	{
		ptd->line = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
	}
}

void hand_textbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_up(&ptd->text, pxp);
}

void hand_textbox_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_dbclick(&ptd->text, pxp);
}

void hand_textbox_mousemove(res_win_t widget, dword_t mk, const xpoint_t* ppt)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_mousemove(&ptd->text, mk, ppt);
}

void hand_textbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_size(&ptd->text, code, prs);
}

void hand_textbox_vert_scroll(res_win_t widget, const scroll_t* psc)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_vert_scroll(&ptd->text, psc);
}

void hand_textbox_horz_scroll(res_win_t widget, const scroll_t* psc)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_horz_scroll(&ptd->text, psc);
}

void hand_textbox_vert_wheel(res_win_t widget, dword_t dw, short delta)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_vert_wheel(&ptd->text, dw, delta);
}

void hand_textbox_horz_wheel(res_win_t widget, dword_t dw, short delta)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_horz_wheel(&ptd->text, dw, delta);
}

void hand_textbox_command(res_win_t widget, int code, int cid, void* data)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);
	xbrush_t xb;
	xpen_t xp;
	xface_t xa;
	xcolor_t* pxc;

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	if (cid == IDC_PARENT)
	{
		if (code == COMMAND_COLOR)
		{
			pxc = (xcolor_t*)data;

			widget_get_xbrush(widget, &xb);
			format_xcolor(&pxc[0], xb.color);
			widget_set_xbrush(widget, &xb);

			widget_get_xpen(widget, &xp);
			format_xcolor(&pxc[1], xp.color);
			widget_set_xpen(widget, &xp);

			widget_get_xface(widget, &xa);
			format_xcolor(&pxc[2], xa.text_color);
			widget_set_xface(widget, &xa);
		}
	}
	else if (cid == widget_get_user_id(widget))
	{
		if (code == COMMAND_UPDATE)
		{
			noti_textbox_owner(widget, NC_TEXTLINEUPDATE, (link_t_ptr)ptd->text.data, ptd->line, NULL);
		}
		else if (code == COMMAND_CHANGE)
		{
			noti_textbox_owner(widget, NC_TEXTLINECHANGED, (link_t_ptr)ptd->text.data, ptd->line, NULL);
		}
	}
}

void hand_textbox_erase(res_win_t widget, res_dc_t dc)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;
}

void hand_textbox_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	ptd->text.b_paging = widget_can_paging(widget);
	hand_textor_paint(&ptd->text, dc, pxr);
}

/************************************************************************************************/
res_win_t textbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	default_xfont(&ev.xf);
	default_xface(&ev.xa);
	xscpy(ev.xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
	default_xpen(&ev.xp);
	default_xbrush(&ev.xb);

	ev.pf_on_create = hand_textbox_create;
	ev.pf_on_destroy = hand_textbox_destroy;
	ev.pf_on_set_focus = hand_textbox_set_focus;
	ev.pf_on_kill_focus = hand_textbox_kill_focus;
	ev.pf_on_command = hand_textbox_command;

	ev.pf_on_erase = hand_textbox_erase;
	ev.pf_on_paint = hand_textbox_paint;

	ev.pf_on_size = hand_textbox_size;

	ev.pf_on_horz_scroll = hand_textbox_horz_scroll;
	ev.pf_on_horz_wheel = hand_textbox_horz_wheel;
	ev.pf_on_vert_scroll = hand_textbox_vert_scroll;
	ev.pf_on_vert_wheel = hand_textbox_vert_wheel;

	ev.pf_on_keydown = hand_textbox_keydown;
	ev.pf_on_char = hand_textbox_char;

	ev.pf_on_mouse_move = hand_textbox_mousemove;
	ev.pf_on_lbutton_down = hand_textbox_lbutton_down;
	ev.pf_on_lbutton_up = hand_textbox_lbutton_up;
	ev.pf_on_lbutton_dbclick = hand_textbox_lbutton_dbclick;

	return widget_create(NULL, style, pxr, widget, &ev);
}

void textbox_redraw(res_win_t widget)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	hand_textor_redraw(&ptd->text);

	if (ptd->line != (link_t_ptr)ptd->text.object)
	{
		ptd->line = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
	}
}

void textbox_selectall(res_win_t widget)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	hand_textor_selectall(&ptd->text);

	if (ptd->line != (link_t_ptr)ptd->text.object)
	{
		ptd->line = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->line);
	}
}

void textbox_set_data(res_win_t widget, link_t_ptr data)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	ptd->text.data = (void*)data;

	textbox_redraw(widget);
}

link_t_ptr textbox_get_data(res_win_t widget)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	return (link_t_ptr)ptd->text.data;
}

link_t_ptr textbox_get_focus_line(res_win_t widget)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);
	link_t_ptr nlk = NULL;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return NULL;

	return ptd->line;
}

void textbox_get_line_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	get_object_rect(&ptd->text, nlk, pxr);
}

void textbox_set_text(res_win_t widget, const tchar_t* text)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	parse_text_doc((link_t_ptr)ptd->text.data, text, -1);

	tagbox_redraw(widget);
}

int textbox_get_text(res_win_t widget, tchar_t* buf, int max)
{
	textbox_delta_t* ptd = GETTEXTBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return 0;

	return format_text_doc((link_t_ptr)ptd->text.data, buf, max);
}