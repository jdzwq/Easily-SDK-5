/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc tag text control document

	@module	tagbox.c | tag text control widnows implement file

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

typedef struct _tagbox_delta_t{
	textor_t text;
	link_t_ptr node;
}tagbox_delta_t;

#define GETTAGBOXDELTA(ph) 	(tagbox_delta_t*)widget_get_user_delta(ph)
#define SETTAGBOXDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

static int _tagbox_get_text(void* data, tchar_t* buf, int max)
{
	link_t_ptr tag = (link_t_ptr)data;

	return format_tag_text(tag, buf, max);
}

static void _tagbox_set_text(void* data, const tchar_t* buf, int len)
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

	dev_obj_t ob = { 0 };
	link_t_ptr data;
	FIREDELTA fd = { 0 };

	XDC_ASSERT(!ptd->editor);

	widget_get_xfont(ptd->tagbox, &ob.xf);
	widget_get_xface(ptd->tagbox, &ob.xa);
	widget_get_xbrush(ptd->tagbox, &ob.xb);
	widget_get_xpen(ptd->tagbox, &ob.xp);

	if (noti_tagctrl_owner(widget, NC_TAGNODEEDITING, tagbox_get_data(ptd->tagbox), tagbox_get_focus_node(ptd->tagbox), (void*)&fd))
		return;

	data = (link_t_ptr)fd.data;
	if (!data)
		return;

	ptd->editor = firectrl_create(ptd->tagbox, pxr, data, &ob);
	XDC_ASSERT(ptd->editor);
	widget_set_user_id(ptd->editor, IDC_FIRECTRL);
	widget_set_owner(ptd->editor, widget);

	widget_show(ptd->editor, WD_SHOW_NORMAL);
	widget_set_focus(ptd->editor);
}

void noti_tagctrl_commit_edit(res_win_t widget, NOTICE_EDITOR* pne)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	FIREDELTA fd = { 0 };
	link_t_ptr tag, node;

	XDC_ASSERT(ptd->editor);

	fd.data = firectrl_get_data(ptd->editor);
	fd.item = (link_t_ptr)pne->data;

	tag = tagbox_get_data(ptd->tagbox);
	node = tagbox_get_focus_node(ptd->tagbox);
	noti_tagctrl_owner(widget, NC_TAGNODECOMMIT, tag, node, (void*)&fd);

	widget_destroy(ptd->editor);
	ptd->editor = NULL;

	tagbox_redraw(ptd->tagbox);
}

void noti_tagctrl_rollback_edit(res_win_t widget, NOTICE_EDITOR* pne)
{
	tagctrl_delta_t* ptd = GETTAGCTRLDELTA(widget);

	FIREDELTA fd = { 0 };
	link_t_ptr tag, node;

	XDC_ASSERT(ptd->editor);

	fd.data = firectrl_get_data(ptd->editor);
	fd.item = (link_t_ptr)pne->data;

	tag = tagbox_get_data(ptd->tagbox);
	node = tagbox_get_focus_node(ptd->tagbox);
	noti_tagctrl_owner(widget, NC_TAGNODEROLLBACK, tag, node, (void*)&fd);

	widget_destroy(ptd->editor);
	ptd->editor = NULL;
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
int hand_tagbox_create(res_win_t widget, void* data)
{
	tagbox_delta_t* ptd;
	xhand_t canv;
	res_dc_t rdc;

	ptd = (tagbox_delta_t*)xmem_alloc(sizeof(tagbox_delta_t));
	memset((void*)ptd, 0, sizeof(tagbox_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_set_canvas(widget, canv);

	ptd->text.widget = widget;
	ptd->text.data = NULL;
	ptd->text.pf_scan_text = scan_tag_text;
	ptd->text.pf_get_text = _tagbox_get_text;
	ptd->text.pf_set_text = _tagbox_set_text;

	SETTAGBOXDELTA(widget, ptd);

	return 0;
}

void hand_tagbox_destroy(res_win_t widget)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	hand_textor_destroy(&ptd->text);

	destroy_display_canvas(widget_get_canvas(widget));

	xmem_free(ptd);

	SETTAGBOXDELTA(widget, 0);
}

void hand_tagbox_set_focus(res_win_t widget, res_win_t wt)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_set_focus(&ptd->text, wt);
}

void hand_tagbox_kill_focus(res_win_t widget, res_win_t wt)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_kill_focus(&ptd->text, wt);
}

void hand_tagbox_keydown(res_win_t widget, int key)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);
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

			if (ptd->node != (link_t_ptr)ptd->text.object)
			{
				ptd->node = (link_t_ptr)ptd->text.object;
				widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
			}
		}
		break;
	case KEY_DELETE:
		if (PRESS_ACCEPT == hand_textor_delete(&ptd->text))
		{
			widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);

			if (ptd->node != (link_t_ptr)ptd->text.object)
			{
				ptd->node = (link_t_ptr)ptd->text.object;
				widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
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

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
		}
		break;
	case KEY_RIGHT:
		hand_textor_right(&ptd->text);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
		}
		break;
	case KEY_UP:
		hand_textor_up(&ptd->text);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
		}
		break;
	case KEY_DOWN:
		hand_textor_down(&ptd->text);

		if (ptd->node != (link_t_ptr)ptd->text.object)
		{
			ptd->node = (link_t_ptr)ptd->text.object;
			widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
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

				if (ptd->node != (link_t_ptr)ptd->text.object)
				{
					ptd->node = (link_t_ptr)ptd->text.object;
					widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
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

				if (ptd->node != (link_t_ptr)ptd->text.object)
				{
					ptd->node = (link_t_ptr)ptd->text.object;
					widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
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

				if (ptd->node != (link_t_ptr)ptd->text.object)
				{
					ptd->node = (link_t_ptr)ptd->text.object;
					widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
				}
			}
		}
		break;
	}
}

void hand_tagbox_char(res_win_t widget, tchar_t ch)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	if (ch == KEY_BACK || ch == KEY_TAB || ch == KEY_ENTER || ch < 32)
		return;

	tagctrl = (res_win_t)delta;

	nlk = tagbox_get_focus_node(widget);
	if (!nlk)
		return 0;

	if (is_tag_code_node(nlk))
	{
		if (!tagctrl_is_editing(tagctrl))
		{
			tagbox_get_node_rect(widget, nlk, &xr);
			noti_tagctrl_begin_edit(tagctrl, &xr);

			widget_post_char(NULL, ch);

			return 1;
		}
	}

	if (PRESS_ACCEPT == hand_textor_char(&ptd->text, ch))
	{
		widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);
	}
}

void hand_tagbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_down(&ptd->text, pxp);

	if (ptd->node != (link_t_ptr)ptd->text.object)
	{
		ptd->node = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
	}
}

void hand_tagbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_up(&ptd->text, pxp);
}

void hand_tagbox_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_lbutton_dbclick(&ptd->text, pxp);
}

void hand_tagbox_mousemove(res_win_t widget, dword_t mk, const xpoint_t* ppt)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_mousemove(&ptd->text, mk, ppt);
}

void hand_tagbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_size(&ptd->text, code, prs);
}

void hand_tagbox_vert_scroll(res_win_t widget, const scroll_t* psc)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_vert_scroll(&ptd->text, psc);
}

void hand_tagbox_horz_scroll(res_win_t widget, const scroll_t* psc)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_horz_scroll(&ptd->text, psc);
}

void hand_tagbox_vert_wheel(res_win_t widget, dword_t dw, short delta)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_vert_wheel(&ptd->text, dw, delta);
}

void hand_tagbox_horz_wheel(res_win_t widget, dword_t dw, short delta)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_horz_wheel(&ptd->text, dw, delta);
}

void hand_tagbox_command(res_win_t widget, int code, int cid, void* data)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);
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
		if (code == COMMAND_CHANGE)
		{
			tagctrl_cancel_editing(tagctrl);
		}
	}
}

void hand_tagbox_erase(res_win_t widget, res_dc_t dc)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;
}

void hand_tagbox_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->text.data)
		return;

	hand_textor_paint(&ptd->text, dc, pxr);
}

/************************************************************************************************/
res_win_t tagbox_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	default_xfont(&ev.xf);
	default_xface(&ev.xa);
	default_xpen(&ev.xp);
	default_xbrush(&ev.xb);

	ev.pf_on_create = hand_tagbox_create;
	ev.pf_on_destroy = hand_tagbox_destroy;
	ev.pf_on_set_focus = hand_tagbox_set_focus;
	ev.pf_on_kill_focus = hand_tagbox_kill_focus;
	ev.pf_on_command = hand_tagbox_command;

	ev.pf_on_erase = hand_tagbox_erase;
	ev.pf_on_paint = hand_tagbox_paint;

	ev.pf_on_size = hand_tagbox_size;

	ev.pf_on_horz_scroll = hand_tagbox_horz_scroll;
	ev.pf_on_horz_wheel = hand_tagbox_horz_wheel;
	ev.pf_on_vert_scroll = hand_tagbox_vert_scroll;
	ev.pf_on_vert_wheel = hand_tagbox_vert_wheel;

	ev.pf_on_keydown = hand_tagbox_keydown;
	ev.pf_on_char = hand_tagbox_char;

	ev.pf_on_mouse_move = hand_tagbox_mousemove;
	ev.pf_on_lbutton_down = hand_tagbox_lbutton_down;
	ev.pf_on_lbutton_up = hand_tagbox_lbutton_up;
	ev.pf_on_lbutton_dbclick = hand_tagbox_lbutton_dbclick;

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void tagbox_redraw(res_win_t widget, bool_t b_calc)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	hand_textor_redraw(&ptd->text);

	if (ptd->node != (link_t_ptr)ptd->text.object)
	{
		ptd->node = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
	}
}

void tagbox_selectall(res_win_t widget)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	hand_textor_selectall(&ptd->text);

	if (ptd->node != (link_t_ptr)ptd->text.object)
	{
		ptd->node = (link_t_ptr)ptd->text.object;
		widget_post_command(widget, COMMAND_CHANGE, widget_get_user_id(widget), (void*)ptd->node);
	}
}

void tagbox_attach(res_win_t widget, link_t_ptr data)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	ptd->text.data = (void*)data;

	tagbox_redraw(widget, 1);
}

link_t_ptr tagbox_fetch(res_win_t widget)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	return (link_t_ptr)ptd->text.data;
}

link_t_ptr tagbox_detach(res_win_t widget)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);
	link_t_ptr ptr;

	XDC_ASSERT(ptd != NULL);

	ptr = (link_t_ptr)ptd->text.data;
	ptd->text.data = NULL;

	return ptr;
}

link_t_ptr tagbox_get_focus_node(res_win_t widget)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);
	link_t_ptr nlk = NULL;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return NULL;

	return ptd->node;
}

void tagbox_get_node_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr)
{
	tagbox_delta_t* ptd = GETTAGBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->text.data)
		return;

	get_object_rect(&ptd->text, nlk, pxr);
}
