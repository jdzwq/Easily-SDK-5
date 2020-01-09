/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tag text control document

	@module	tagctrl.c | tag text control widnows implement file

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
#include "xdctextor.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcgdi.h"
#include "xdccanv.h"
#include "xdcbox.h"
#include "xdceditor.h"

#include "xdcdlg.h"

typedef struct _tagctrl_delta_t{
	textor_t text;
	link_t_ptr node;
	res_win_t editor;
	res_win_t menu;
}tagctrl_delta_t;

#define GETTAGCTRLDELTA(ph) 	(tagctrl_delta_t*)widget_get_user_delta(ph)
#define SETTAGCTRLDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

/********************************************************************************************/
static int _tagctrl_get_text(void* data, tchar_t* buf, int max)
{
	link_t_ptr tag = (link_t_ptr)data;

	return format_tag_text(tag, buf, max);
}

static void _tagctrl_set_text(void* data, const tchar_t* buf, int len)
{
	link_t_ptr tag = (link_t_ptr)data;

	parse_tag_text(tag, buf, len);
}

/********************************************************************************************/
int noti_tagctrl_owner(res_win_t widget, unsigned long code, link_t_ptr tag, link_t_ptr nlk, void* data)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	NOTICE_TAG nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.tag = tag;
	nf.node = nlk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);

	return nf.ret;
}

void noti_tagctrl_begin_edit(res_win_t widget, const xrect_t* pxr)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	clr_mod_t ob = { 0 };
	link_t_ptr data;
	FIREDELTA fd = { 0 };

	XDC_ASSERT(!ptd->editor);

	if (widget_get_lock(widget))
		return;

	if (noti_tagctrl_owner(widget, NC_TAGNODEEDITING, (link_t_ptr)ptd->text.data, ptd->node, (void*)&fd))
		return;

	data = (link_t_ptr)fd.data;
	if (!data)
	{
		ptd->editor = fireedit_create(widget, pxr);
		widget_set_user_id(ptd->editor, IDC_FIREEDIT);

		if (!is_null(fd.text))
		{
			editbox_set_text(ptd->editor, fd.text);
			editbox_selectall(ptd->editor);
		}
	}
	else
	{
		if (is_words_table(data))
		{
			ptd->editor = firewords_create(widget, pxr, data);
			widget_set_user_id(ptd->editor, IDC_FIREWORDS);

			if (!is_null(fd.text))
			{
				editbox_set_text(ptd->editor, fd.text);
				editbox_selectall(ptd->editor);
			}
		}
		else if (is_grid_doc(data))
		{
			ptd->editor = firegrid_create(widget, pxr, data);
			widget_set_user_id(ptd->editor, IDC_FIREGRID);
		}
		else
		{
			return;
		}
	}

	XDC_ASSERT(ptd->editor != NULL);

	widget_get_color_mode(widget, &ob);

	widget_set_owner(ptd->editor, widget);
	widget_send_command(ptd->editor, COMMAND_COLOR, IDC_PARENT, (void*)&ob);
	editbox_auto_size(ptd->editor, 1);

	widget_show(ptd->editor, WD_SHOW_NORMAL);
	widget_set_focus(ptd->editor);
}

void noti_tagctrl_commit_edit(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	FIREDELTA fd = { 0 };
	res_win_t editctrl;
	int eid;

	if (!widget_is_valid(ptd->editor))
		return;

	eid = widget_get_user_id(ptd->editor);
	if (eid == IDC_FIREEDIT)
	{
		fd.data = NULL;
		fd.item = NULL;
		fd.text = editbox_get_text_ptr(ptd->editor);
	}
	else if (eid == IDC_FIREWORDS)
	{
		fd.data = firewords_get_data(ptd->editor);
		fd.item = firewords_get_item(ptd->editor);
		fd.text = editbox_get_text_ptr(ptd->editor);
	}
	else if (eid == IDC_FIREGRID)
	{
		fd.data = firegrid_get_data(ptd->editor);
		fd.item = firegrid_get_item(ptd->editor);
		fd.text = NULL;
	}

	hand_textor_done(&ptd->text);

	noti_tagctrl_owner(widget, NC_TAGNODECOMMIT, (link_t_ptr)ptd->text.data, ptd->node, (void*)&fd);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);

	widget_post_key(widget, KEY_RIGHT);

	tagctrl_redraw(widget, 1);
}

void noti_tagctrl_rollback_edit(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	FIREDELTA fd = { 0 };
	res_win_t editctrl;
	int eid;

	if (!widget_is_valid(ptd->editor))
		return;

	eid = widget_get_user_id(ptd->editor);
	if (eid == IDC_FIREEDIT)
	{
		fd.data = NULL;
		fd.item = NULL;
		fd.text = NULL;
	}
	else if (eid == IDC_FIREWORDS)
	{
		fd.data = firewords_get_data(ptd->editor);
		fd.item = NULL;
		fd.text = NULL;
	}
	else if (eid == IDC_FIREGRID)
	{
		fd.data = firegrid_get_data(ptd->editor);
		fd.item = NULL;
		fd.text = NULL;
	}

	noti_tagctrl_owner(widget, NC_TAGNODEROLLBACK, (link_t_ptr)ptd->text.data, ptd->node, (void*)&fd);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
}

bool_t tagctrl_is_editing(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	return (widget_is_valid(ptd->editor)) ? 1 : 0;
}

void tagctrl_cancel_editing(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (widget_is_valid(ptd->editor))
		widget_post_key(ptd->editor, KEY_ESC);
}

/********************************************************************************************/
int hand_tagctrl_create(res_win_t widget, void* data)
{
	tagctrl_delta_t* ptd;
	xhand_t canv;
	res_dc_t rdc;

	ptd = (tagctrl_delta_t*)xmem_alloc(sizeof(tagctrl_delta_t));
	xmem_zero((void*)ptd, sizeof(tagctrl_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_attach_canvas(widget, canv);

	ptd->text.widget = widget;
	ptd->text.data = NULL;
	ptd->text.pf_scan_text = (PF_SCAN_TEXT)scan_tag_doc;
	ptd->text.pf_get_text = _tagctrl_get_text;
	ptd->text.pf_set_text = _tagctrl_set_text;
	ptd->text.max_undo = 1024;

	SETTAGCTRLDELTA(widget, ptd);

	return 0;
}

void hand_tagctrl_destroy(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (widget_is_valid(ptd->menu))
	{
		editmenu_destroy(ptd->menu);
	}

	hand_textor_destroy(&ptd->text);

	destroy_display_canvas(widget_detach_canvas(widget));

	xmem_free(ptd);

	SETTAGCTRLDELTA(widget, 0);
}

void hand_tagctrl_set_focus(res_win_t widget, res_win_t wt)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_set_focus(&ptd->text, wt);
}

void hand_tagctrl_kill_focus(res_win_t widget, res_win_t wt)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	if (widget_is_valid(ptd->menu))
	{
		editmenu_destroy(ptd->menu);
	}

	hand_textor_kill_focus(&ptd->text, wt);
}

void hand_tagctrl_keydown(res_win_t widget, int key)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	xface_t xa;
	xrect_t xr;

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	widget_get_xface(widget, &xa);

	switch (key)
	{
	case KEY_BACK:
		if (widget_get_lock(widget))
			break;

		if (PRESS_ACCEPT == hand_textor_back(&ptd->text))
		{
			widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

			if (ptd->node != (link_t_ptr)ptd->text.object)
			{
				ptd->node = (link_t_ptr)ptd->text.object;
				widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
			}
		}
		break;
	case KEY_DELETE:
		if (widget_get_lock(widget))
			break;

		if (PRESS_ACCEPT == hand_textor_delete(&ptd->text))
		{
			widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

			if (ptd->node != (link_t_ptr)ptd->text.object)
			{
				ptd->node = (link_t_ptr)ptd->text.object;
				widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
			}
		}
		break;
	case KEY_TAB:
		break;
	case KEY_ENTER:
		if (widget_get_lock(widget))
			break;

		if (ptd->node && !is_tag_text_node(ptd->node))
		{
			if (!tagctrl_is_editing(widget))
			{
				tagctrl_get_node_rect(widget, ptd->node, &xr);
				noti_tagctrl_begin_edit(widget, &xr);
			}
			return;
		}
		else
		{
			if (PRESS_ACCEPT == hand_textor_char(&ptd->text, key))
			{
				widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);
				return;
			}
		}
		break;
	case KEY_ESC:
		if (widget_is_valid(ptd->menu))
		{
			editmenu_destroy(ptd->menu);
		}

		if (widget_get_lock(widget))
			break;

		hand_textor_escape(&ptd->text);
		break;
	case KEY_LEFT:
		hand_textor_left(&ptd->text);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
		break;
	case KEY_RIGHT:
		hand_textor_right(&ptd->text);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
		break;
	case KEY_UP:
		hand_textor_up(&ptd->text);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
		break;
	case KEY_DOWN:
		hand_textor_down(&ptd->text);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
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

void hand_tagctrl_char(res_win_t widget, tchar_t ch)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	xrect_t xr;

	if (!ptd)
		return;

	if (widget_get_lock(widget))
		return;

	if (!ptd->text.data)
		return;

	if (ch == KEY_BACK || ch == KEY_TAB || ch == KEY_ENTER || ch < 32)
		return;

	if (ptd->node && !is_tag_text_node(ptd->node))
	{
		if (!tagctrl_is_editing(widget))
		{
			tagctrl_get_node_rect(widget, ptd->node, &xr);
			noti_tagctrl_begin_edit(widget, &xr);
		}
		return;
	}

	if (PRESS_ACCEPT == hand_textor_char(&ptd->text, ch))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);
	}
}

void hand_tagctrl_copy(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_copy(&ptd->text);
}

void hand_tagctrl_cut(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (widget_get_lock(widget))
		return;

	if (!ptd->text.data)
		return;

	if (PRESS_ACCEPT == hand_textor_cut(&ptd->text))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
	}
}

void hand_tagctrl_paste(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (widget_get_lock(widget))
		return;

	if (!ptd->text.data)
		return;

	if (PRESS_ACCEPT == hand_textor_paste(&ptd->text))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
	}
}

void hand_tagctrl_undo(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (widget_get_lock(widget))
		return;

	if (!ptd->text.data)
		return;

	if (PRESS_ACCEPT == hand_textor_undo(&ptd->text))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
	}
}

void hand_tagctrl_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_down(&ptd->text, pxp);

	if (ptd->node != (link_t_ptr)ptd->text.object)
	{
		ptd->node = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
	}
}

void hand_tagctrl_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_up(&ptd->text, pxp);

	if (ptd->node && !is_tag_text_node(ptd->node))
	{
		hand_textor_selectcur(&ptd->text);
	}
}

void hand_tagctrl_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_dbclick(&ptd->text, pxp);
}

void hand_tagctrl_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

}

void hand_tagctrl_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	xpoint_t xp;

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	if (widget_is_valid(ptd->menu))
	{
		editmenu_destroy(ptd->menu);
	}
	else
	{
		//get_cursor_point(&ptd->text, &xp);
		xp.x = pxp->x;
		xp.y = pxp->y;
		widget_client_to_screen(widget, &xp);

		ptd->menu = editmenu_create(widget, &xp);
	}
}

void hand_tagctrl_mousemove(res_win_t widget, dword_t mk, const xpoint_t* ppt)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_mousemove(&ptd->text, mk, ppt);
}

void hand_tagctrl_size(res_win_t widget, int code, const xsize_t* prs)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_size(&ptd->text, code, prs);
}

void hand_tagctrl_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_scroll(&ptd->text, bHorz, nLine);
}

void hand_tagctrl_command_color(res_win_t widget, void* data)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	widget_set_color_mode(widget, (clr_mod_t*)data);

	widget_update_window(widget);
	widget_update_client(widget);
}

void hand_tagctrl_command_switch(res_win_t widget, bool_t bkey)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	dword_t ws;

	ws = widget_get_style(widget);

	if (bkey)
		ws |= (WD_STYLE_TOUCH);
	else
		ws &= ~(WD_STYLE_TOUCH);

	widget_set_style(widget, ws);

	widget_update_window(widget);
}

void hand_tagctrl_self_command(res_win_t widget, int code, void* data)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	switch (code)
	{
	case COMMAND_CHANGE:
		tagctrl_cancel_editing(widget);
		break;
	}
}

void hand_tagctrl_child_command(res_win_t widget, int code, void* data)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_tagctrl_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_tagctrl_rollback_edit(widget);
		break;
	}
}


void hand_tagctrl_notice(res_win_t widget, LPNOTICE pnt)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	NOTICE_MENU* pnm;
	int nCmd;

	if (pnt->id == IDC_EDITMENU)
	{
		pnm = (NOTICE_MENU*)pnt;

		switch (pnm->code)
		{
		case NC_MENULBCLK:
			if (pnm->item)
			{
				nCmd = xstol(get_menu_item_id_ptr(pnm->item));
				if (nCmd == COMMAND_COPY)
				{
					widget_copy(widget);
				}
				else if (nCmd == COMMAND_CUT)
				{
					widget_cut(widget);
				}
				else if (nCmd == COMMAND_PASTE)
				{
					widget_paste(widget);
				}
				else if (nCmd == COMMAND_UNDO)
				{
					widget_undo(widget);
				}
			}

			editmenu_destroy(ptd->menu);
			ptd->menu = NULL;
			break;
		}
	}
}

void hand_tagctrl_erase(res_win_t widget, res_dc_t dc)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;
}

void hand_tagctrl_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_paint(&ptd->text, dc, pxr);
}

/************************************************************************************************/
res_win_t tagctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_tagctrl_create;
	ev.pf_on_destroy = hand_tagctrl_destroy;
	ev.pf_on_set_focus = hand_tagctrl_set_focus;
	ev.pf_on_kill_focus = hand_tagctrl_kill_focus;
	ev.pf_on_command_color = hand_tagctrl_command_color;
	ev.pf_on_command_switch = hand_tagctrl_command_switch;
	ev.pf_on_self_command = hand_tagctrl_self_command;
	ev.pf_on_child_command = hand_tagctrl_child_command;

	ev.pf_on_erase = hand_tagctrl_erase;
	ev.pf_on_paint = hand_tagctrl_paint;

	ev.pf_on_size = hand_tagctrl_size;

	ev.pf_on_scroll = hand_tagctrl_scroll;

	ev.pf_on_keydown = hand_tagctrl_keydown;
	ev.pf_on_char = hand_tagctrl_char;

	ev.pf_on_copy = hand_tagctrl_copy;
	ev.pf_on_cut = hand_tagctrl_cut;
	ev.pf_on_paste = hand_tagctrl_paste;
	ev.pf_on_undo = hand_tagctrl_undo;

	ev.pf_on_mouse_move = hand_tagctrl_mousemove;
	ev.pf_on_lbutton_down = hand_tagctrl_lbutton_down;
	ev.pf_on_lbutton_up = hand_tagctrl_lbutton_up;
	ev.pf_on_lbutton_dbclick = hand_tagctrl_lbutton_dbclick;

	ev.pf_on_rbutton_down = hand_tagctrl_rbutton_down;
	ev.pf_on_rbutton_up = hand_tagctrl_rbutton_up;

	ev.pf_on_notice = hand_tagctrl_notice;

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void tagctrl_redraw(res_win_t widget, bool_t b_calc)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	hand_textor_redraw(&ptd->text);

	if (ptd->node != (link_t_ptr)ptd->text.object)
	{
		ptd->node = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
	}
}

void tagctrl_selectall(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	hand_textor_selectall(&ptd->text);

	if (ptd->node != (link_t_ptr)ptd->text.object)
	{
		ptd->node = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
	}
}

void tagctrl_settag(res_win_t widget, const tchar_t* tname)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	int len;
	tchar_t* buf;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	if (is_null(tname))
		return;

	len = hand_textor_selected_text(&ptd->text, NULL, MAX_LONG);
	len += (xslen(_T("[][/]")) + 2 * xslen(tname));

	buf = xsalloc(len + 1);
	xsprintf(buf, _T("[%s]"), tname);
	len = xslen(buf);

	hand_textor_selected_text(&ptd->text, buf + len, MAX_LONG);
	len = xslen(buf);

	xsprintf(buf + len, _T("[/%s]"), tname);

	if (PRESS_ACCEPT != hand_textor_replace_text(&ptd->text, buf, -1))
	{
		xsfree(buf);
		return;
	}

	xsfree(buf);

	len = format_tag_text((link_t_ptr)ptd->text.data, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	format_tag_text((link_t_ptr)ptd->text.data, buf, len);

	parse_tag_text((link_t_ptr)ptd->text.data, buf, len);
	xsfree(buf);

	tagctrl_redraw(widget, 1);
}

void tagctrl_cleartag(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	int len;
	tchar_t* buf;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	if (!ptd->text.object)
		return;

	if (is_tag_text_node((link_t_ptr)ptd->text.object))
		return;

	len = xslen(get_tag_node_text_ptr((link_t_ptr)ptd->text.object));

	buf = xsalloc(len + 1);
	xsncpy(buf, get_tag_node_text_ptr((link_t_ptr)ptd->text.object),len);

	if (PRESS_ACCEPT != hand_textor_replace_text(&ptd->text, buf, -1))
	{
		xsfree(buf);
		return;
	}

	xsfree(buf);

	len = format_tag_text((link_t_ptr)ptd->text.data, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	format_tag_text((link_t_ptr)ptd->text.data, buf, len);

	parse_tag_text((link_t_ptr)ptd->text.data, buf, len);
	xsfree(buf);

	tagctrl_redraw(widget, 1);
}

void tagctrl_attach(res_win_t widget, link_t_ptr data)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	XDC_ASSERT(data && is_tag_doc(data));

	ptd->text.data = (void*)data;

	tagctrl_redraw(widget, 1);
}

link_t_ptr tagctrl_fetch(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	return (link_t_ptr)ptd->text.data;
}

link_t_ptr tagctrl_detach(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	link_t_ptr ptr;

	XDC_ASSERT(ptd != NULL);

	ptr = (link_t_ptr)ptd->text.data;
	ptd->text.data = NULL;

	return ptr;
}

link_t_ptr tagctrl_get_focus_node(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	link_t_ptr nlk = NULL;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return NULL;

	return ptd->node;
}

void tagctrl_get_node_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	get_object_rect(&ptd->text, nlk, pxr);
}

bool_t tagctrl_can_undo(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return 0;

	return (ptd->text.ptu != NULL) ? 1 : 0;
}

bool_t tagctrl_copy(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return 0;

	return (hand_textor_copy(&ptd->text) == PRESS_ACCEPT) ? 1 : 0;
}

bool_t tagctrl_cut(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return 0;

	return (hand_textor_cut(&ptd->text) == PRESS_ACCEPT) ? 1 : 0;
}

bool_t tagctrl_paste(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return 0;

	return (hand_textor_paste(&ptd->text) == PRESS_ACCEPT) ? 1 : 0;
}

bool_t tagctrl_undo(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return 0;

	return (hand_textor_undo(&ptd->text) == PRESS_ACCEPT) ? 1 : 0;
}

void tagctrl_clean(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return ;

	hand_textor_clean(&ptd->text);
}