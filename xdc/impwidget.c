/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget document

	@module	impwin.c | implement file

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

#include "impwidget.h"
#include "xdcinit.h"

#ifdef XDU_SUPPORT_WIDGET


void send_quit_message(int code)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_send_quit_message)(code);
}

void message_fetch(msg_t* pmsg, res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_message_fetch)(pmsg, wt);
}

bool_t message_peek(msg_t* pmsg)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_message_peek)(pmsg);
}


bool_t	message_translate(const msg_t* pmsg)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_message_translate)(pmsg);
}

result_t message_dispatch(const msg_t* pmsg)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_message_dispatch)(pmsg);
}

bool_t	message_is_quit(const msg_t* pmsg)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_message_is_quit)(pmsg);
}

void message_position(xpoint_t* ppt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_message_position)(ppt);
}

void widget_set_alpha(res_win_t wt, byte_t b)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_alpha)(wt, b);
}

byte_t widget_get_alpha(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_alpha)(wt);
}

void widget_track_mouse(res_win_t wt, dword_t mask)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_track_mouse)(wt, mask);
}

#ifdef XDU_SUPPORT_WIDGET_REGION
void widget_set_region(res_win_t wt, res_rgn_t rgn)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_region)(wt, rgn);
}

#endif //XDU_SUPPORT_WIDGET_EX

void get_screen_size(xsize_t* pxs)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_get_screen_size)(pxs);
}

void get_desktop_size(xsize_t* pxs)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_get_desktop_size)(pxs);
}

void screen_size_to_pt(xsize_t* pxs)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_screen_size_to_pt)(pxs);
}

void screen_size_to_tm(xsize_t* pxs)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_screen_size_to_tm)(pxs);
}

res_acl_t create_accel_table(const accel_t* pac, int n)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_create_accel_table)(pac, n);
}

void destroy_accel_table(res_acl_t hac)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_destroy_accel_table)(hac);
}

bool_t	widget_is_maximized(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_is_maximized)(wt);
}

bool_t	widget_is_minimized(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_is_minimized)(wt);
}

bool_t	widget_enum_child(res_win_t wt, PF_ENUM_WINDOW_PROC pf, var_long pv)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_enum_child)(wt, pf, pv);
}

if_event_t* widget_get_dispatch(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_dispatch)(wt);
}

res_win_t widget_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev)
{
	if_widget_t* pif;
	
	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_create)(wname, wstyle, pxr, wparent, pev);
}

void widget_set_style(res_win_t wt, dword_t ws)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_style)(wt, ws);
}

dword_t widget_get_style(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_style)(wt);
}

void widget_set_owner(res_win_t wt, res_win_t owner)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_owner)(wt, owner);
}

res_win_t widget_get_owner(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_owner)(wt);
}

void widget_attach_accel(res_win_t wt, res_acl_t acl)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_accel)(wt, acl);
}

res_acl_t widget_get_accel(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_accel)(wt);
}

res_acl_t widget_detach_accel(res_win_t wt)
{
	res_acl_t acl;
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	acl = (*pif->pf_widget_get_accel)(wt);

	(*pif->pf_widget_set_accel)(wt, NULL);

	return acl;
}

void widget_set_core_delta(res_win_t wt, var_long pd)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_core_delta)(wt, pd);
}

var_long widget_get_core_delta(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_core_delta)(wt);
}

void widget_set_user_delta(res_win_t wt, var_long pd)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;
	
	(*pif->pf_widget_set_user_delta)(wt, pd);
}

var_long widget_get_user_delta(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_user_delta)(wt);
}

void widget_set_user_id(res_win_t wt, dword_t uid)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_user_id)(wt, uid);
}

dword_t widget_get_user_id(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_user_id)(wt);
}

res_win_t widget_get_child(res_win_t wt, dword_t uid)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_child)(wt, uid);
}

res_win_t widget_get_parent(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_parent)(wt);
}

void widget_set_user_prop(res_win_t wt, const tchar_t* pname,var_long pval)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_user_prop)(wt, pname, pval);
}

var_long widget_get_user_prop(res_win_t wt, const tchar_t* pname)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_user_prop)(wt, pname);
}

int widget_get_return(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_user_result)(wt);
}

var_long widget_del_user_prop(res_win_t wt, const tchar_t* pname)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_del_user_prop)(wt, pname);
}

res_ctx_t widget_client_ctx(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_client_ctx)(wt);
}

res_ctx_t widget_window_ctx(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_window_ctx)(wt);
}

void widget_release_ctx(res_win_t wt, res_ctx_t dc)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_release_ctx)(wt, dc);
}

void widget_get_client_rect(res_win_t wt, xrect_t* prt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_client_rect)(wt, prt);
}

void widget_get_window_rect(res_win_t wt, xrect_t* prt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_window_rect)(wt, prt);
}

void widget_get_window_edge(res_win_t wt, xsize_t* pxs)
{
	xrect_t xr1, xr2;
	xpoint_t xp1, xp2;

	widget_get_window_rect(wt, &xr1);
	widget_get_client_rect(wt, &xr2);

	xp1.x = xr2.x;
	xp1.y = xr2.y;
	widget_client_to_screen(wt, &xp1);

	xp2.x = xr2.x + xr2.w;
	xp2.y = xr2.y + xr2.h;
	widget_client_to_screen(wt, &xp2);

	xr2.x = xp1.x;
	xr2.y = xp1.y;
	xr2.w = xp2.x - xp1.x;
	xr2.h = xp2.y - xp1.y;

	pxs->cx = xr1.w - xr2.w;
	pxs->cy = xr1.h - xr2.h;
}

void widget_client_to_screen(res_win_t wt, xpoint_t* ppt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_client_to_screen)(wt, ppt);
}

void widget_screen_to_client(res_win_t wt, xpoint_t* ppt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_screen_to_client)(wt, ppt);
}

void widget_client_to_window(res_win_t wt, xpoint_t* ppt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_client_to_window)(wt, ppt);
}

void widget_window_to_client(res_win_t wt, xpoint_t* ppt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_window_to_client)(wt, ppt);
}

void widget_center_window(res_win_t wt, res_win_t owner)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_center_window)(wt, owner);
}

void widget_calc_border(dword_t ws, border_t* pbd)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_calc_border)(ws, pbd);
}

void widget_adjust_size(dword_t ws, xsize_t* pxs)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_adjust_size)(ws, pxs);
}

void widget_set_cursor(res_win_t wt, int curs)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_cursor)(wt, curs);
}

void widget_set_capture(res_win_t wt, bool_t b)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_capture)(wt, b);
}

void widget_create_caret(res_win_t wt, int w, int h)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_create_caret)(wt, w, h);
}

void widget_destroy_caret(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_destroy_caret)(wt);
}

void widget_show_caret(res_win_t wt, int x, int y, bool_t b)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_show_caret)(wt, x, y, b);
}

void widget_set_focus(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_focus)(wt);
}

bool_t widget_key_state(res_win_t wt, byte_t key)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_key_state)(wt, key);
}

bool_t widget_is_hotvoer(res_win_t wt)
{
#ifdef XDU_SUPPORT_WIDGET_EX
	return (widget_get_style(wt) & WD_STYLE_HOTOVER) ? 1 : 0;
#else
	return 0;
#endif
}

bool_t widget_is_editor(res_win_t wt)
{
	return (widget_get_style(wt) & WD_STYLE_EDITOR) ? 1 : 0;
}

bool_t	widget_is_valid(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_is_valid)(wt);
}

bool_t	widget_is_child(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_is_child)(wt);
}

bool_t	widget_is_focus(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_is_focus)(wt);
}

bool_t	widget_can_focus(res_win_t wt)
{
	if_widget_t* pif;
	dword_t dw;

	pif = PROCESS_WIDGET_INTERFACE;

	dw = (*pif->pf_widget_get_style)(wt);

	return (dw & WD_STYLE_NOACTIVE) ? 0 : 1;
}

bool_t	widget_can_paging(res_win_t wt)
{
	if_widget_t* pif;
	dword_t dw;

	pif = PROCESS_WIDGET_INTERFACE;

	dw = (*pif->pf_widget_get_style)(wt);

	return (dw & WD_STYLE_PAGING) ? 1 : 0;
}

bool_t widget_has_close(res_win_t wt)
{
	if_widget_t* pif;
	dword_t dw;

	pif = PROCESS_WIDGET_INTERFACE;

	dw = (*pif->pf_widget_get_style)(wt);

	return (dw & WD_STYLE_CLOSEBOX) ? 1 : 0;
}

bool_t widget_has_size(res_win_t wt)
{
	if_widget_t* pif;
	dword_t dw;

	pif = PROCESS_WIDGET_INTERFACE;

	dw = (*pif->pf_widget_get_style)(wt);

	return (dw & WD_STYLE_SIZEBOX) ? 1 : 0;
}

bool_t widget_has_border(res_win_t wt)
{
	if_widget_t* pif;
	dword_t dw;

	pif = PROCESS_WIDGET_INTERFACE;

	dw = (*pif->pf_widget_get_style)(wt);

	return (dw & WD_STYLE_BORDER) ? 1 : 0;
}

void widget_get_scroll_info(res_win_t wt, bool_t horz, scroll_t* psc)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_scroll_info)(wt, horz, psc);
}

void widget_set_scroll_info(res_win_t wt, bool_t horz, const scroll_t* psc)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_scroll_info)(wt, horz, psc);
}

void widget_scroll(res_win_t wt, bool_t horz, int line)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_scroll)(wt, horz, line);
}

void widget_post_char(res_win_t wt, tchar_t ch)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_post_char)(wt, ch);
}

void widget_post_key(res_win_t wt, int key)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_post_key)(wt, key);
}

void widget_post_notice(res_win_t wt, NOTICE* pnt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_post_notice)(wt, pnt);
}

int widget_send_notice(res_win_t wt, NOTICE* pnt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_send_notice)(wt, pnt);
}

void widget_post_command(res_win_t wt, int code, int cid, var_long data)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_post_command)(wt, code, cid, data);
}

int widget_send_command(res_win_t wt, int code, int cid, var_long data)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_send_command)(wt, code, cid, data);
}

void widget_size(res_win_t wt, const xsize_t* pxs)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_size)(wt, pxs);
}

void widget_move(res_win_t wt, const xpoint_t* ppt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_move)(wt, ppt);
}

void widget_take(res_win_t wt, int zor)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_take)(wt, zor);
}

void widget_show(res_win_t wt, dword_t sw)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_show)(wt, sw);
}

void widget_layout(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_layout)(wt);
}

void widget_paint(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_paint)(wt);
}

void widget_update(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_update)(wt);
}

void widget_erase(res_win_t wt, const xrect_t* prt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_erase)(wt, prt);
}

void widget_enable(res_win_t wt, bool_t b)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_enable)(wt, b);
}

void widget_active(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_active)(wt);
}

void widget_close(res_win_t wt, int ret)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_close)(wt, ret);
}

void widget_destroy(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_destroy)(wt);
}

void widget_set_title(res_win_t wt, const tchar_t* token)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_title)(wt, token);
}

int	widget_get_title(res_win_t wt, tchar_t* buf, int max)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_title)(wt, buf, max);
}

bool_t	widget_set_subproc(res_win_t wt, dword_t sid, if_subproc_t* sub)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_set_subproc)(wt, sid, sub);
}

void widget_del_subproc(res_win_t wt, dword_t sid)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_del_subproc)(wt, sid);
}

var_long widget_get_subproc_delta(res_win_t wt, dword_t sid)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_subproc_delta)(wt, sid);
}

bool_t widget_set_subproc_delta(res_win_t wt, dword_t sid, var_long delta)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_set_subproc_delta)(wt, sid, delta);
}

bool_t widget_has_subproc(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_has_subproc)(wt);
}

var_long widget_set_timer(res_win_t wt, int ms)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_set_timer)(wt, ms);
}

void widget_kill_timer(res_win_t wt, var_long tid)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_kill_timer)(wt, tid);
}


bool_t	widget_has_struct(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_has_struct)(wt);
}

void  widget_set_xfont(res_win_t wt, const xfont_t* pxf)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_xfont)(wt, pxf);
}

void  widget_get_xfont(res_win_t wt, xfont_t* pxf)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_xfont)(wt, pxf);
}

const xfont_t*  widget_get_xfont_ptr(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_xfont_ptr)(wt);
}

void widget_set_xface(res_win_t wt, const xface_t* pxa)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_xface)(wt, pxa);
}

void  widget_get_xface(res_win_t wt, xface_t* pxa)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_xface)(wt, pxa);
}

const xface_t*  widget_get_xface_ptr(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_xface_ptr)(wt);
}

void  widget_set_xbrush(res_win_t wt, const xbrush_t* pxb)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_xbrush)(wt, pxb);
}

void  widget_get_xbrush(res_win_t wt, xbrush_t* pxb)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_xbrush)(wt, pxb);
}

const xbrush_t*  widget_get_xbrush_ptr(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_xbrush_ptr)(wt);
}

void  widget_set_xpen(res_win_t wt, const xpen_t* pxp)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_xpen)(wt, pxp);
}

void  widget_get_xpen(res_win_t wt, xpen_t* pxp)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_xpen)(wt, pxp);
}

const xpen_t*  widget_get_xpen_ptr(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_xpen_ptr)(wt);
}

void  widget_set_mask(res_win_t wt, const xcolor_t* pxc)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_mask)(wt, pxc);
}

void  widget_get_mask(res_win_t wt, xcolor_t* pxc)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_mask)(wt, pxc);
}

const xcolor_t*  widget_get_mask_ptr(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_mask_ptr)(wt);
}

void  widget_set_iconic(res_win_t wt, const xcolor_t* pxc)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_iconic)(wt, pxc);
}

void  widget_get_iconic(res_win_t wt, xcolor_t* pxc)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_iconic)(wt, pxc);
}

const xcolor_t*  widget_get_iconic_ptr(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_iconic_ptr)(wt);
}

void  widget_set_point(res_win_t wt, const xpoint_t* ppt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_point)(wt, ppt);
}

void  widget_get_point(res_win_t wt, xpoint_t* ppt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_point)(wt, ppt);
}

void widget_set_color_mode(res_win_t wt, const clr_mod_t* pclr)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_set_color_mode)(wt, pclr);
}

void widget_get_color_mode(res_win_t wt, clr_mod_t* pclr)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_get_color_mode)(wt, pclr);
}


int widget_do_modal(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_do_modal)(wt);
}

void widget_do_trace(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	(*pif->pf_widget_do_trace)(wt);
}

#ifdef XDU_SUPPORT_CONTEXT_OPENGL
res_glc_t widget_get_glctx(res_win_t wt)
{
	if_widget_t* pif;

	pif = PROCESS_WIDGET_INTERFACE;

	return (*pif->pf_widget_get_glctx)(wt);
}
#endif

#endif /*XDU_SUPPORT_WIDGET*/
