/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc rich control document

	@module	richctrl.c | rich control widnows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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
#include "xdcinf.h"
#include "xdcmenu.h"
#include "xdcbox.h"
#include "xdceditor.h"

typedef struct _richctrl_delta_t{
	textor_t textor;
	LINKPTR node;

	res_win_t editor;

	int chs;
	tchar_t pch[MAX_CHS + 1];
}richctrl_delta_t;

#define GETRICHCTRLDELTA(ph) 	(richctrl_delta_t*)widget_get_user_delta(ph)
#define SETRICHCTRLDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

static int _richctrl_get_text(void* data, tchar_t* buf, int max)
{
	LINKPTR text = (LINKPTR)data;

	return format_rich_doc(text, buf, max);
}

static void _richctrl_set_text(void* data, const tchar_t* buf, int len)
{
	LINKPTR text = (LINKPTR)data;

	parse_rich_doc(text, buf, len);
}

static bool_t _richctrl_get_paging(res_win_t widget, xsize_t* pse)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	xrect_t xr;

	XDC_ASSERT(ptd && ptd->textor.data);

	if (widget_get_style(widget) & WD_STYLE_PAGING)
	{
		pse->fx = get_rich_width((LINKPTR)(ptd->textor.data));
		pse->fy = get_rich_height((LINKPTR)(ptd->textor.data));

		widget_size_to_pt(widget, pse);

		return 1;
	}
	else
	{
		widget_get_client_rect(widget, &xr);

		pse->cx = xr.w;
		pse->cy = xr.h;

		return 0;
	}
}
/********************************************************************************************/
int noti_richctrl_owner(res_win_t widget, unsigned long code, LINKPTR ptr, LINKPTR nlk, void* data)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	NOTICE_RICH nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.rich = ptr;
	nf.node = nlk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);

	return nf.ret;
}

void noti_richctrl_begin_edit(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	const tchar_t* text;
	xrect_t xr = { 0 };

	clr_mod_t ob = { 0 };
	xfont_t xf = { 0 };

	XDC_ASSERT(ptd->node);
	XDC_ASSERT(!ptd->editor);

	if (widget_get_lock(widget))
		return;

	widget_get_color_mode(widget, &ob);
	widget_get_xfont(widget, &xf);

	textor_cursor_rect(&ptd->textor, &xr);
	pt_expand_rect(&xr, 0, 2);

	if (noti_richctrl_owner(widget, NC_RICHATOMEDITING, (LINKPTR)ptd->textor.data, ptd->node, NULL))
		return;

	ptd->editor = fireedit_create(widget, &xr);
	XDC_ASSERT(ptd->editor);
	widget_set_user_id(ptd->editor, IDC_FIREEDIT);
	widget_set_owner(ptd->editor, widget);

	widget_set_color_mode(ptd->editor, &ob, 0);
	widget_set_xfont(ptd->editor, &xf);
	widget_show(ptd->editor, WD_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	text = get_rich_node_title_ptr(ptd->node);
	editbox_set_text(ptd->editor, text);
	editbox_auto_size(ptd->editor, 1);
}

void noti_richctrl_commit_edit(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	const tchar_t* text;
	res_win_t editctrl;
	xrect_t xr;

	if (!widget_is_valid(ptd->editor))
		return;

	XDC_ASSERT(ptd->node);

	text = (tchar_t*)editbox_get_text_ptr(ptd->editor);

	if (!noti_richctrl_owner(widget, NC_RICHATOMCOMMIT, (LINKPTR)ptd->textor.data, ptd->node, (void*)text))
	{
		if (compare_text(text, -1, get_rich_node_title_ptr(ptd->node), -1, 0) != 0)
		{
			set_rich_node_title(ptd->node, text);
			noti_richctrl_owner(widget, NC_RICHATOMUPDATE, (LINKPTR)ptd->textor.data, ptd->node, NULL);

			textor_object_rect(&ptd->textor, ptd->node, &xr);
			widget_invalid(widget, &xr, 0);
		}
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_richctrl_rollback_edit(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDC_ASSERT(ptd->node);

	noti_richctrl_owner(widget, NC_RICHATOMROLLBACK, (LINKPTR)ptd->textor.data, ptd->node, NULL);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_richctrl_reset_editor(res_win_t widget, bool_t bCommit)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_richctrl_commit_edit(widget);
		else
			noti_richctrl_rollback_edit(widget);
	}
}

/********************************************************************************************/
int hand_richctrl_create(res_win_t widget, void* data)
{
	richctrl_delta_t* ptd;
	XHANDLE canv;

	ptd = (richctrl_delta_t*)xmem_alloc(sizeof(richctrl_delta_t));
	xmem_zero((void*)ptd, sizeof(richctrl_delta_t));

	canv = create_display_canvas(NULL);

	widget_attach_canvas(widget, canv);

	ptd->textor.widget = widget;
	ptd->textor.data = NULL;
	ptd->textor.pf_scan_text = (PF_SCAN_TEXT)scan_rich_text;
	ptd->textor.pf_get_text = _richctrl_get_text;
	ptd->textor.pf_set_text = _richctrl_set_text;
	ptd->textor.pf_get_paging = _richctrl_get_paging;
	ptd->textor.max_undo = 1024;

	SETRICHCTRLDELTA(widget, ptd);

	return 0;
}

void hand_richctrl_destroy(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	XHANDLE canv;

	XDC_ASSERT(ptd != NULL);

	noti_richctrl_reset_editor(widget, 0);

	hand_textor_destroy(&ptd->textor);

	canv = widget_detach_canvas(widget);

	XDC_ASSERT(canv != NULL);

	destroy_display_canvas(canv);

	xmem_free(ptd);

	SETRICHCTRLDELTA(widget, 0);
}

void hand_richctrl_set_focus(res_win_t widget, res_win_t wt)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_set_focus(&ptd->textor, wt);
}

void hand_richctrl_kill_focus(res_win_t widget, res_win_t wt)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_kill_focus(&ptd->textor, wt);

	if (widget_is_editor(widget))
	{
		if (richctrl_can_undo(widget))
			widget_send_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
		else
			widget_send_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, NULL);
	}
}

void hand_richctrl_keydown(res_win_t widget, int key)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	xface_t xa;

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	widget_get_xface(widget, &xa);

	switch (key)
	{
	case KEY_BACK:
		if (widget_get_lock(widget))
			break;

		if (PRESS_ACCEPT == hand_textor_back(&ptd->textor))
		{
			widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

			if (ptd->node != (LINKPTR)ptd->textor.object)
			{
				ptd->node = (LINKPTR)ptd->textor.object;
				widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, NULL);
			}
		}
		break;
	case KEY_DELETE:
		if (widget_get_lock(widget))
			break;

		if (PRESS_ACCEPT == hand_textor_delete(&ptd->textor))
		{
			widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

			if (ptd->node != (LINKPTR)ptd->textor.object)
			{
				ptd->node = (LINKPTR)ptd->textor.object;
				widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, NULL);
			}
		}
		break;
	case KEY_TAB:
		break;
	case KEY_ENTER:
		break;
	case KEY_ESC:
		if (widget_get_lock(widget))
			break;

		hand_textor_escape(&ptd->textor);
		break;
	case KEY_LEFT:
		hand_textor_left(&ptd->textor);

		if (ptd->node != (LINKPTR)ptd->textor.object)
		{
			ptd->node = (LINKPTR)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
		break;
	case KEY_RIGHT:
		hand_textor_right(&ptd->textor);

		if (ptd->node != (LINKPTR)ptd->textor.object)
		{
			ptd->node = (LINKPTR)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
		break;
	case KEY_UP:
		hand_textor_up(&ptd->textor);

		if (ptd->node != (LINKPTR)ptd->textor.object)
		{
			ptd->node = (LINKPTR)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
		break;
	case KEY_DOWN:
		hand_textor_down(&ptd->textor);

		if (ptd->node != (LINKPTR)ptd->textor.object)
		{
			ptd->node = (LINKPTR)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
		}
		break;
	case KEY_PAGEDOWN:
		hand_textor_move_next_page(&ptd->textor);
		break;
	case KEY_PAGEUP:
		hand_textor_move_prev_page(&ptd->textor);
		break;
	case KEY_HOME:
		hand_textor_move_first_page(&ptd->textor);
		break;
	case KEY_END:
		hand_textor_move_last_page(&ptd->textor);
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

void hand_richctrl_char(res_win_t widget, tchar_t ch)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (widget_get_lock(widget))
		return;

	if (!ptd->textor.data)
		return;

	if (ch == KEY_BACK)
		return;

	if (ch != KEY_ENTER && ch != KEY_TAB && ch > 0 && ch < 32)
		return;

	if (is_rich_text_reserve(ch))
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

	if (PRESS_ACCEPT == hand_textor_word(&ptd->textor, ptd->pch))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);
	}
}

#ifdef XDC_SUPPORT_CLIPBOARD
void hand_richctrl_copy(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_copy(&ptd->textor);
}

void hand_richctrl_cut(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (widget_get_lock(widget))
		return;

	if (!ptd->textor.data)
		return;

	if (PRESS_ACCEPT == hand_textor_cut(&ptd->textor))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

		if (ptd->node != (LINKPTR)ptd->textor.object)
		{
			ptd->node = (LINKPTR)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, NULL);
		}
	}
}

void hand_richctrl_paste(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (widget_get_lock(widget))
		return;

	if (!ptd->textor.data)
		return;

	if (PRESS_ACCEPT == hand_textor_paste(&ptd->textor))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

		if (ptd->node != (LINKPTR)ptd->textor.object)
		{
			ptd->node = (LINKPTR)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, NULL);
		}
	}
}
#endif //XDC_SUPPORT_CLIPBOARD

void hand_richctrl_undo(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (widget_get_lock(widget))
		return;

	if (!ptd->textor.data)
		return;

	if (PRESS_ACCEPT == hand_textor_undo(&ptd->textor))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, NULL);

		if (ptd->node != (LINKPTR)ptd->textor.object)
		{
			ptd->node = (LINKPTR)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, NULL);
		}
	}
}

void hand_richctrl_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 1);

	hand_textor_lbutton_down(&ptd->textor, pxp);
}

void hand_richctrl_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_lbutton_up(&ptd->textor, pxp);

	if (ptd->node != (LINKPTR)ptd->textor.object)
	{
		ptd->node = (LINKPTR)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
	}
	else if (ptd->textor.atom && ptd->textor.object && !get_rich_node_fixed((LINKPTR)ptd->textor.object))
	{
		noti_richctrl_begin_edit(widget);
	}
}

void hand_richctrl_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	if (ptd->textor.object)
	{
		hand_textor_selectobj(&ptd->textor);
	}
}


void hand_richctrl_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 1);
}

void hand_richctrl_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	xpoint_t xp;

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	xp.x = pxp->x;
	xp.y = pxp->y;
	widget_client_to_screen(widget, &xp);

	textor_menu(widget, &xp, WD_LAYOUT_LEFTBOTTOM);
}

void hand_richctrl_mousemove(res_win_t widget, dword_t mk, const xpoint_t* ppt)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	if (widget_is_valid(ptd->editor))
		return;

	hand_textor_mousemove(&ptd->textor, mk, ppt);
}

void hand_richctrl_size(res_win_t widget, int code, const xsize_t* prs)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 0);

	hand_textor_size(&ptd->textor, code, prs);
}

void hand_richctrl_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 0);

	hand_textor_scroll(&ptd->textor, bHorz, nLine);
}

void hand_richctrl_self_command(res_win_t widget, int code, void* data)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	if (code == COMMAND_UPDATE)
	{
		noti_richctrl_owner(widget, NC_RICHNODEUPDATE, (LINKPTR)ptd->textor.data, (LINKPTR)ptd->node, NULL);
	}
	else if (code == COMMAND_CHANGE)
	{
		noti_richctrl_owner(widget, NC_RICHNODECHANGED, (LINKPTR)ptd->textor.data, (LINKPTR)ptd->node, NULL);
	}
}

void hand_richctrl_child_command(res_win_t widget, int code, void* data)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_richctrl_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_richctrl_rollback_edit(widget);
		break;
	}
}

void hand_richctrl_menu_command(res_win_t widget, int code, int cid, void* data)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

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

void hand_richctrl_erase(res_win_t widget, res_ctx_t dc)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;
}

void hand_richctrl_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_paint(&ptd->textor, dc, pxr);
}

/************************************************************************************************/
res_win_t richctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };
	res_win_t wt;
	xface_t xa;

	ev.pf_on_create = hand_richctrl_create;
	ev.pf_on_destroy = hand_richctrl_destroy;
	ev.pf_on_set_focus = hand_richctrl_set_focus;
	ev.pf_on_kill_focus = hand_richctrl_kill_focus;

	ev.pf_on_self_command = hand_richctrl_self_command;
	ev.pf_on_child_command = hand_richctrl_child_command;
	ev.pf_on_menu_command = hand_richctrl_menu_command;

	ev.pf_on_erase = hand_richctrl_erase;
	ev.pf_on_paint = hand_richctrl_paint;

	ev.pf_on_size = hand_richctrl_size;

	ev.pf_on_scroll = hand_richctrl_scroll;

	ev.pf_on_keydown = hand_richctrl_keydown;
	ev.pf_on_char = hand_richctrl_char;

#ifdef XDC_SUPPORT_CLIPBOARD
	ev.pf_on_copy = hand_richctrl_copy;
	ev.pf_on_cut = hand_richctrl_cut;
	ev.pf_on_paste = hand_richctrl_paste;
#endif
	ev.pf_on_undo = hand_richctrl_undo;

	ev.pf_on_mouse_move = hand_richctrl_mousemove;
	ev.pf_on_lbutton_down = hand_richctrl_lbutton_down;
	ev.pf_on_lbutton_up = hand_richctrl_lbutton_up;
	ev.pf_on_lbutton_dbclick = hand_richctrl_lbutton_dbclick;

	ev.pf_on_rbutton_down = hand_richctrl_rbutton_down;
	ev.pf_on_rbutton_up = hand_richctrl_rbutton_up;

	wt = widget_create(wname, wstyle, pxr, wparent, &ev);
	if (!wt)
		return NULL;

	widget_get_xface(wt, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
	widget_set_xface(wt, &xa);

	return wt;
}

void richctrl_attach(res_win_t widget, LINKPTR data)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	XDC_ASSERT(data && is_rich_doc(data));

	ptd->textor.data = (void*)data;
	ptd->textor.object = NULL;
	ptd->textor.page = 1;

	ptd->node = NULL;
	richctrl_redraw(widget);

	widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, NULL);
}

LINKPTR richctrl_fetch(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	return (LINKPTR)ptd->textor.data;
}

LINKPTR richctrl_detach(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	LINKPTR ptr;

	XDC_ASSERT(ptd != NULL);

	ptr = (LINKPTR)ptd->textor.data;
	ptd->textor.data = NULL;
	ptd->textor.object = NULL;
	ptd->textor.page = 0;

	ptd->node = NULL;

	return ptr;
}

LINKPTR richctrl_get_focus_node(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	LINKPTR nlk = NULL;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return NULL;

	return ptd->node;
}

void richctrl_set_focus_node(res_win_t widget, LINKPTR nlk)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_findobj(&ptd->textor, nlk);

	if (ptd->node != (LINKPTR)ptd->textor.object)
	{
		ptd->node = (LINKPTR)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
	}
}

void richctrl_delete_node(res_win_t widget, LINKPTR nlk)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	if (widget_get_lock(widget))
		return;

	hand_textor_done(&ptd->textor);

	delete_rich_node(nlk);
	
	richctrl_redraw(widget);
}

LINKPTR richctrl_insert_node(res_win_t widget, LINKPTR pos)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);
	LINKPTR nlk;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return NULL;

	if (widget_get_lock(widget))
		return NULL;

	hand_textor_done(&ptd->textor);

	nlk = insert_rich_node((LINKPTR)ptd->textor.data, pos);

	richctrl_redraw(widget);

	return nlk;
}

void richctrl_get_node_rect(res_win_t widget, LINKPTR nlk, xrect_t* pxr)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	textor_object_rect(&ptd->textor, nlk, pxr);
}

void richctrl_redraw(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_redraw(&ptd->textor);

	if (ptd->node != (LINKPTR)ptd->textor.object)
	{
		ptd->node = (LINKPTR)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
	}
}

void richctrl_select_cur(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_selectobj(&ptd->textor);

	if (ptd->node != (LINKPTR)ptd->textor.object)
	{
		ptd->node = (LINKPTR)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
	}
}

void richctrl_select_all(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_selectall(&ptd->textor);

	if (ptd->node != (LINKPTR)ptd->textor.object)
	{
		ptd->node = (LINKPTR)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (void*)ptd->node);
	}
}

bool_t richctrl_can_undo(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return (ptd->textor.ptu != NULL) ? 1 : 0;
}

#ifdef XDC_SUPPORT_CLIPBOARD
bool_t richctrl_copy(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return (hand_textor_copy(&ptd->textor) == PRESS_ACCEPT) ? 1 : 0;
}

bool_t richctrl_cut(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	if (widget_get_lock(widget))
		return 0;

	return (hand_textor_cut(&ptd->textor) == PRESS_ACCEPT) ? 1 : 0;
}

bool_t richctrl_paste(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	if (widget_get_lock(widget))
		return 0;

	return (hand_textor_paste(&ptd->textor) == PRESS_ACCEPT) ? 1 : 0;
}
#endif //XDC_SUPPORT_CLIPBOARD

bool_t richctrl_undo(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	if (widget_get_lock(widget))
		return 0;

	return (hand_textor_undo(&ptd->textor) == PRESS_ACCEPT) ? 1 : 0;
}

void richctrl_set_undo(res_win_t widget, bool_t bUndo)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return ;

	if (bUndo)
		hand_textor_done(&ptd->textor);
	else
		hand_textor_clean(&ptd->textor);
}

void richctrl_move_to_page(res_win_t widget, int page)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 0);

	hand_textor_move_to_page(&ptd->textor, page);
}

void richctrl_move_first_page(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 0);

	hand_textor_move_first_page(&ptd->textor);
}

void richctrl_move_last_page(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 0);

	hand_textor_move_last_page(&ptd->textor);
}

void richctrl_move_next_page(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 0);

	hand_textor_move_next_page(&ptd->textor);
}

void richctrl_move_prev_page(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	noti_richctrl_reset_editor(widget, 0);

	hand_textor_move_prev_page(&ptd->textor);
}

int richctrl_get_cur_page(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return textor_cur_page(&ptd->textor);
}

int richctrl_get_max_page(res_win_t widget)
{
	richctrl_delta_t* ptd = GETRICHCTRLDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return textor_max_page(&ptd->textor);
}
