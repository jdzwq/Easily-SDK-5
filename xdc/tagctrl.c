/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tag text control document

	@module	tagctrl.c | tag text control widnows implement file

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

#include "xdcctrl.h"
#include "handler.h"
#include "winnc.h"
#include "xdcmenu.h"
#include "textor.h"

typedef struct _tagctrl_delta_t{
	textor_t textor;
	link_t_ptr joint;
	
	bool_t b_lock;

	int chs;
	tchar_t pch[CHS_LEN + 1];
}tagctrl_delta_t;

#define GETTAGCTRLDELTA(ph) 	(tagctrl_delta_t*)widget_get_user_delta(ph)
#define SETTAGCTRLDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/********************************************************************************************/
static int _tagctrl_get_text(void* data, tchar_t* buf, int max)
{
	link_t_ptr tag = (link_t_ptr)data;

	return format_tag_doc(tag, buf, max);
}

static void _tagctrl_set_text(void* data, const tchar_t* buf, int len)
{
	link_t_ptr tag = (link_t_ptr)data;

	parse_tag_doc(tag, buf, len);
}

static bool_t _tagctrl_get_paging(res_win_t widget, xsize_t* pse)
{
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	pse->cx = xr.w;
	pse->cy = xr.h;

	return 0;
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
	nf.joint = nlk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);

	return nf.ret;
}

/********************************************************************************************/
int hand_tagctrl_create(res_win_t widget, void* data)
{
	tagctrl_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (tagctrl_delta_t*)xmem_alloc(sizeof(tagctrl_delta_t));
	xmem_zero((void*)ptd, sizeof(tagctrl_delta_t));

	SETTAGCTRLDELTA(widget, ptd);

	ptd->textor.widget = widget;
	ptd->textor.dc = widget_client_ctx(widget);
	ptd->textor.data = NULL;
	ptd->textor.pf_scan_text = (PF_SCAN_TEXT)scan_tag_text;
	ptd->textor.pf_get_text = _tagctrl_get_text;
	ptd->textor.pf_set_text = _tagctrl_set_text;
	ptd->textor.pf_get_paging = _tagctrl_get_paging;
	ptd->textor.max_undo = 1024;

	ptd->b_lock = 1;

	return 0;
}

void hand_tagctrl_destroy(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	hand_textor_clean(&ptd->textor);

	widget_release_ctx(widget, ptd->textor.dc);

	xmem_free(ptd);

	SETTAGCTRLDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_tagctrl_set_focus(res_win_t widget, res_win_t wt)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_set_focus(&ptd->textor, wt);
}

void hand_tagctrl_kill_focus(res_win_t widget, res_win_t wt)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_kill_focus(&ptd->textor, wt);

	if (widget_is_editor(widget))
	{
		if (tagctrl_get_dirty(widget))
			widget_send_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		else
			widget_send_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, (var_long)NULL);
	}
}

void hand_tagctrl_keydown(res_win_t widget, int key)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	switch (key)
	{
	case KEY_BACK:
		if (ptd->b_lock)
			break;

		if (_TEXTOR_PRESS_ACCEPT == hand_textor_back(&ptd->textor))
		{
			widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

			if (ptd->joint != (link_t_ptr)ptd->textor.object)
			{
				ptd->joint = (link_t_ptr)ptd->textor.object;
				widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
			}
		}
		break;
	case KEY_DELETE:
		if (ptd->b_lock)
			break;

		if (_TEXTOR_PRESS_ACCEPT == hand_textor_delete(&ptd->textor))
		{
			widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

			if (ptd->joint != (link_t_ptr)ptd->textor.object)
			{
				ptd->joint = (link_t_ptr)ptd->textor.object;
				widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
			}
		}
		break;
	case KEY_ENTER:
		break;
	case KEY_ESC:
		if (ptd->b_lock)
			break;

		hand_textor_escape(&ptd->textor);
		break;
	case KEY_LEFT:
		hand_textor_left(&ptd->textor);

		if (ptd->joint != (link_t_ptr)ptd->textor.object)
		{
			ptd->joint = (link_t_ptr)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
		}
		break;
	case KEY_RIGHT:
		hand_textor_right(&ptd->textor);

		if (ptd->joint != (link_t_ptr)ptd->textor.object)
		{
			ptd->joint = (link_t_ptr)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
		}
		break;
	case KEY_UP:
		hand_textor_up(&ptd->textor);

		if (ptd->joint != (link_t_ptr)ptd->textor.object)
		{
			ptd->joint = (link_t_ptr)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
		}
		break;
	case KEY_DOWN:
		hand_textor_down(&ptd->textor);

		if (ptd->joint != (link_t_ptr)ptd->textor.object)
		{
			ptd->joint = (link_t_ptr)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
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

	if (!ptd)
		return;

	if (ptd->b_lock)
		return;

	if (!ptd->textor.data)
		return;

	if (ch == KEY_BACK)
		return;

	if (ch != KEY_ENTER && ch != KEY_TAB && ch > 0 && ch < 32)
		return;

	if (is_tag_text_reserve(ch))
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
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);
	}
}

void hand_tagctrl_copy(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_copy(&ptd->textor);
}

void hand_tagctrl_cut(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (ptd->b_lock)
		return;

	if (!ptd->textor.data)
		return;

	if (_TEXTOR_PRESS_ACCEPT != hand_textor_cut(&ptd->textor))
		return;

	widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

	if (ptd->joint != (link_t_ptr)ptd->textor.object)
	{
		ptd->joint = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void hand_tagctrl_paste(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (ptd->b_lock)
		return;

	if (!ptd->textor.data)
		return;

	if (_TEXTOR_PRESS_ACCEPT != hand_textor_paste(&ptd->textor))
		return;

	widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

	if (ptd->joint != (link_t_ptr)ptd->textor.object)
	{
		ptd->joint = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void hand_tagctrl_undo(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (ptd->b_lock)
		return;

	if (!ptd->textor.data)
		return;

	if (_TEXTOR_PRESS_ACCEPT != hand_textor_undo(&ptd->textor))
		return;

	widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

	if (ptd->joint != (link_t_ptr)ptd->textor.object)
	{
		ptd->joint = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void hand_tagctrl_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_lbutton_down(&ptd->textor, pxp);
}

void hand_tagctrl_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_lbutton_up(&ptd->textor, pxp);

	if (ptd->joint != (link_t_ptr)ptd->textor.object)
	{
		ptd->joint = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void hand_tagctrl_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_selectobj(&ptd->textor);
}

void hand_tagctrl_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

}

void hand_tagctrl_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
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

void hand_tagctrl_mousemove(res_win_t widget, dword_t mk, const xpoint_t* ppt)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_mousemove(&ptd->textor, mk, ppt);
}

void hand_tagctrl_size(res_win_t widget, int code, const xsize_t* prs)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_size(&ptd->textor, code, prs);
}

void hand_tagctrl_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_scroll(&ptd->textor, bHorz, nLine);
}

void hand_tagctrl_self_command(res_win_t widget, int code, var_long data)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	if (code == COMMAND_UPDATE)
	{
		noti_tagctrl_owner(widget, NC_TAGJOINTUPDATE, (link_t_ptr)ptd->textor.data, ptd->joint, NULL);
	}
	else if (code == COMMAND_CHANGE)
	{
		noti_tagctrl_owner(widget, NC_TAGJOINTCHANGED, (link_t_ptr)ptd->textor.data, ptd->joint, NULL);
	}
}

void hand_tagctrl_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	
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

void hand_tagctrl_erase(res_win_t widget, res_ctx_t dc)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;
}

void hand_tagctrl_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_paint(&ptd->textor, dc, pxr);
}

/************************************************************************************************/
res_win_t tagctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_tagctrl_create)
		EVENT_ON_DESTROY(hand_tagctrl_destroy)

		EVENT_ON_ERASE(hand_tagctrl_erase)
		EVENT_ON_PAINT(hand_tagctrl_paint)

		EVENT_ON_SIZE(hand_tagctrl_size)

		EVENT_ON_SCROLL(hand_tagctrl_scroll)

		EVENT_ON_KEYDOWN(hand_tagctrl_keydown)
		EVENT_ON_CHAR(hand_tagctrl_char)

		EVENT_ON_LBUTTON_DBCLICK(hand_tagctrl_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_tagctrl_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_tagctrl_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_tagctrl_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_tagctrl_rbutton_up)

		EVENT_ON_SET_FOCUS(hand_tagctrl_set_focus)
		EVENT_ON_KILL_FOCUS(hand_tagctrl_kill_focus)
		EVENT_ON_SELF_COMMAND(hand_tagctrl_self_command)
		EVENT_ON_MENU_COMMAND(hand_tagctrl_menu_command)

		EVENT_ON_COPY(hand_tagctrl_copy)
		EVENT_ON_CUT(hand_tagctrl_cut)
		EVENT_ON_PASTE(hand_tagctrl_paste)
		EVENT_ON_UNDO(hand_tagctrl_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void tagctrl_redraw(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_redraw(&ptd->textor);

	if (ptd->joint != (link_t_ptr)ptd->textor.object)
	{
		ptd->joint = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void tagctrl_select_all(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_selectall(&ptd->textor);

	if (ptd->joint != (link_t_ptr)ptd->textor.object)
	{
		ptd->joint = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void tagctrl_select_cur(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_selectcur(&ptd->textor);

	if (ptd->joint != (link_t_ptr)ptd->textor.object)
	{
		ptd->joint = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

int tagctrl_get_selected_text(res_win_t widget, tchar_t* buf, int max)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return hand_textor_selected_text(&ptd->textor, buf, max);
}

void tagctrl_setup_tag(res_win_t widget, const tchar_t* tname)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	int len;
	tchar_t* buf;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	if (is_null(tname))
		return;

	len = hand_textor_selected_text(&ptd->textor, NULL, MAX_LONG);
	
	len += (xslen(_T("[][/]")) + 2 * xslen(tname));

	buf = xsalloc(len + 1);
	xsprintf(buf, _T("[%s]"), tname);
	len = xslen(buf);

	hand_textor_selected_text(&ptd->textor, buf + len, MAX_LONG);
	len = xslen(buf);

	xsprintf(buf + len, _T("[/%s]"), tname);

	if (_TEXTOR_PRESS_ACCEPT != hand_textor_replace_text(&ptd->textor, buf, -1))
	{
		xsfree(buf);
		return;
	}

	xsfree(buf);

	len = format_tag_doc((link_t_ptr)ptd->textor.data, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	format_tag_doc((link_t_ptr)ptd->textor.data, buf, len);

	parse_tag_doc((link_t_ptr)ptd->textor.data, buf, len);
	xsfree(buf);

	tagctrl_redraw(widget);
}

void tagctrl_clean_tag(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	int len;
	tchar_t* buf;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	if (!ptd->textor.object)
		return;

	if (is_tag_text_joint((link_t_ptr)ptd->textor.object))
		return;

	len = xslen(get_tag_joint_text_ptr((link_t_ptr)ptd->textor.object));

	buf = xsalloc(len + 1);
	xsncpy(buf, get_tag_joint_text_ptr((link_t_ptr)ptd->textor.object),len);

	if (_TEXTOR_PRESS_ACCEPT != hand_textor_replace_text(&ptd->textor, buf, -1))
	{
		xsfree(buf);
		return;
	}

	xsfree(buf);

	len = format_tag_doc((link_t_ptr)ptd->textor.data, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	format_tag_doc((link_t_ptr)ptd->textor.data, buf, len);

	parse_tag_doc((link_t_ptr)ptd->textor.data, buf, len);
	xsfree(buf);

	tagctrl_redraw(widget);
}

void tagctrl_attach(res_win_t widget, link_t_ptr data)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(data && is_tag_doc(data));

	ptd->textor.data = (void*)data;
	ptd->textor.object = NULL;
	ptd->textor.page = 1;
	
	ptd->joint = NULL;

	tagctrl_redraw(widget);

	widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
}

link_t_ptr tagctrl_fetch(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return (link_t_ptr)ptd->textor.data;
}

link_t_ptr tagctrl_detach(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	link_t_ptr ptr;

	XDL_ASSERT(ptd != NULL);

	ptr = (link_t_ptr)ptd->textor.data;
	ptd->textor.data = NULL;
	ptd->textor.object = NULL;
	ptd->textor.page = 0;

	ptd->joint = NULL;

	return ptr;
}

link_t_ptr tagctrl_get_focus_joint(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	link_t_ptr nlk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return NULL;

	return ptd->joint;
}

void tagctrl_set_focus_joint(res_win_t widget, link_t_ptr nlk)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_findobj(&ptd->textor, nlk);

	if (ptd->joint != (link_t_ptr)ptd->textor.object)
	{
		ptd->joint = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void tagctrl_delete_joint(res_win_t widget, link_t_ptr nlk)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_done(&ptd->textor);

	delete_tag_joint(nlk);

	tagctrl_redraw(widget);
}

link_t_ptr tagctrl_insert_joint(res_win_t widget, link_t_ptr pos)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);
	link_t_ptr nlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return NULL;

	hand_textor_done(&ptd->textor);

	nlk = insert_tag_joint((link_t_ptr)ptd->textor.data, pos);
	set_tag_joint_name(nlk, _T("TAG"));
	set_tag_joint_text(nlk, _T("TEXT"), -1);

	tagctrl_redraw(widget);

	return nlk;
}

void tagctrl_set_joint_text(res_win_t widget, link_t_ptr nlk, const tchar_t* token)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	if (ptd->b_lock)
		return;

	hand_textor_done(&ptd->textor);

	set_tag_joint_text(nlk, token, -1);

	tagctrl_redraw(widget);
}


void tagctrl_get_joint_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	textor_object_rect(&ptd->textor, nlk, pxr);
}

bool_t tagctrl_get_dirty(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return (ptd->textor.ptu != NULL) ? 1 : 0;
}

void tagctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	if (bDirty)
		hand_textor_done(&ptd->textor);
	else
		hand_textor_clean(&ptd->textor);
}

void tagctrl_move_to_page(res_win_t widget, int page)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_to_page(&ptd->textor, page);
}

void tagctrl_move_first_page(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_first_page(&ptd->textor);
}

void tagctrl_move_last_page(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_last_page(&ptd->textor);
}

void tagctrl_move_next_page(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_next_page(&ptd->textor);
}

void tagctrl_move_prev_page(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_prev_page(&ptd->textor);
}

int tagctrl_get_cur_page(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return textor_cur_page(&ptd->textor);
}

int tagctrl_get_max_page(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return textor_max_page(&ptd->textor);
}

bool_t tagctrl_get_lock(res_win_t widget)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

void tagctrl_set_lock(res_win_t widget, bool_t bLock)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}
