/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget document

	@module	impwin.h | widget interface file

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

#ifndef _IMPWIN_H
#define _IMPWIN_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_WIDGET


#ifdef	__cplusplus
extern "C" {
#endif

XDL_API res_win_t widget_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev);

XDL_API	void	widget_destroy(res_win_t wt);

XDL_API	void	widget_close(res_win_t wt, int ret);

XDL_API if_event_t* widget_get_dispatch(res_win_t wt);

XDL_API void	widget_set_style(res_win_t wt, dword_t ws);

XDL_API dword_t widget_get_style(res_win_t wt);

XDL_API void	widget_set_core_delta(res_win_t wt, var_long pd);

XDL_API var_long	widget_get_core_delta(res_win_t wt);

XDL_API void	widget_set_user_delta(res_win_t wt, var_long pd);

XDL_API var_long	widget_get_user_delta(res_win_t wt);

XDL_API void	widget_set_user_id(res_win_t wt, dword_t uid);

XDL_API dword_t widget_get_user_id(res_win_t wt);

XDL_API int		widget_get_return(res_win_t wt);

XDL_API void	widget_set_owner(res_win_t wt, res_win_t owner);

XDL_API res_win_t widget_get_owner(res_win_t wt);

XDL_API res_win_t widget_get_child(res_win_t wt, dword_t uid);

XDL_API res_win_t widget_get_parent(res_win_t wt);

XDL_API void	widget_set_user_prop(res_win_t wt, const tchar_t* pname, var_long pval);

XDL_API var_long	widget_get_user_prop(res_win_t wt, const tchar_t* pname);

XDL_API var_long	widget_del_user_prop(res_win_t wt, const tchar_t* pname);

XDL_API res_ctx_t widget_client_ctx(res_win_t wt);

XDL_API res_ctx_t widget_window_ctx(res_win_t wt);

XDL_API void	widget_release_ctx(res_win_t wt, res_ctx_t dc);

XDL_API void	widget_get_client_rect(res_win_t wt, xrect_t* prt);

XDL_API void	widget_get_window_rect(res_win_t wt, xrect_t* prt);

XDL_API void	widget_get_window_edge(res_win_t wt, xsize_t* pxs);

XDL_API void	widget_center_window(res_win_t wt, res_win_t owner);

XDL_API void	widget_client_to_screen(res_win_t wt, xpoint_t* pst);

XDL_API void	widget_screen_to_client(res_win_t wt, xpoint_t* pst);

XDL_API void	widget_client_to_window(res_win_t wt, xpoint_t* pst);

XDL_API void	widget_window_to_client(res_win_t wt, xpoint_t* pst);

XDL_API void	widget_calc_border(dword_t ws, border_t* pbd);

XDL_API void	widget_adjust_size(dword_t ws, xsize_t* pxs);

XDL_API void	widget_size(res_win_t wt, const xsize_t* pxs);

XDL_API void	widget_move(res_win_t wt, const xpoint_t* ppt);

XDL_API void	widget_take(res_win_t wt, int zor);

XDL_API void	widget_show(res_win_t wt, dword_t sw);

XDL_API void	widget_invalid(res_win_t wt, const xrect_t* prt, bool_t b_erase);

XDL_API void	widget_update(res_win_t wt, const xrect_t* prt, bool_t b_erase);

XDL_API void	widget_update_window(res_win_t wt);

XDL_API void	widget_update_client(res_win_t wt);

XDL_API void	widget_enable(res_win_t wt, bool_t b);

XDL_API void	widget_set_cursor(res_win_t wt,int curs);

XDL_API void	widget_set_capture(res_win_t wt,bool_t b);

XDL_API void	widget_create_caret(res_win_t wt, int w, int h);

XDL_API void	widget_destroy_caret(res_win_t wt);

XDL_API void	widget_show_caret(res_win_t wt, int x, int y, bool_t b);

XDL_API void	widget_set_imm(res_win_t wt, bool_t b);

XDL_API bool_t	widget_get_imm(res_win_t wt);

XDL_API void	widget_set_focus(res_win_t wt);

XDL_API bool_t	widget_key_state(res_win_t wt,byte_t key);

XDL_API bool_t	widget_is_hotvoer(res_win_t wt);

XDL_API bool_t	widget_is_editor(res_win_t wt);

XDL_API bool_t	widget_is_valid(res_win_t wt);

XDL_API bool_t	widget_is_child(res_win_t wt);

XDL_API bool_t	widget_is_focus(res_win_t wt);

XDL_API bool_t	widget_can_focus(res_win_t wt);

XDL_API void	widget_post_char(res_win_t wt, tchar_t ch);

XDL_API void	widget_post_key(res_win_t wt, int key);

XDL_API bool_t	widget_can_close(res_win_t wt);

XDL_API bool_t	widget_can_size(res_win_t wt);

XDL_API bool_t	widget_can_paging(res_win_t wt);

XDL_API void	widget_get_scroll(res_win_t wt, bool_t bHorz, scroll_t* psl);

XDL_API void	widget_set_scroll(res_win_t wt, bool_t horz, const scroll_t* psc);

XDL_API void	widget_reset_scroll(res_win_t wt, bool_t bHorz);

XDL_API bool_t	widget_has_border(res_win_t wt);

XDL_API int		widget_send_notice(res_win_t wt, NOTICE* pnt);

XDL_API void	widget_post_command(res_win_t wt, int code, int cid, var_long data);

XDL_API int		widget_send_command(res_win_t wt, int code, int cid, var_long data);

XDL_API void	widget_set_title(res_win_t wt, const tchar_t* token);

XDL_API int		widget_get_title(res_win_t wt, tchar_t* buf, int max);

XDL_API bool_t	widget_is_maximized(res_win_t wt);

XDL_API bool_t	widget_is_minimized(res_win_t wt);

XDL_API bool_t	widget_set_subproc(res_win_t wt, dword_t sid, if_subproc_t* sub);

XDL_API void	widget_del_subproc(res_win_t wt, dword_t sid);

XDL_API bool_t	widget_set_subproc_delta(res_win_t wt, dword_t sid, var_long delta);

XDL_API var_long widget_get_subproc_delta(res_win_t wt, dword_t sid);

XDL_API bool_t	widget_has_subproc(res_win_t wt);

XDL_API var_long widget_set_timer(res_win_t wt, int ms);

XDL_API void	widget_kill_timer(res_win_t wt, var_long tid);

XDL_API void	widget_attach_accel(res_win_t wt, res_acl_t acl);

XDL_API res_acl_t widget_get_accel(res_win_t wt);

XDL_API res_acl_t widget_detach_accel(res_win_t wt);

XDL_API canvas_t widget_get_canvas(res_win_t wt);

XDL_API void	widget_copy(res_win_t wt);

XDL_API void	widget_cut(res_win_t wt);

XDL_API void	widget_paste(res_win_t wt);

XDL_API void	widget_undo(res_win_t wt);

XDL_API bool_t	widget_enum_child(res_win_t wt, PF_ENUM_WINDOW_PROC pf, var_long pv);

XDL_API res_acl_t create_accel_table(const accel_t* pac, int n);

XDL_API void	destroy_accel_table(res_acl_t hac);

XDL_API void	get_screen_size(xsize_t* pxs);

XDL_API void	get_desktop_size(xsize_t* pxs);

XDL_API void	screen_size_to_pt(xsize_t* pxs);

XDL_API void	screen_size_to_tm(xsize_t* pxs);

XDL_API int		widget_do_modal(res_win_t wt);

XDL_API void	widget_do_trace(res_win_t wt);

#ifdef XDK_SUPPORT_WIDGET_EX
XDL_API void	widget_track_mouse(res_win_t wt, dword_t mask);

XDL_API void	widget_set_alpha(res_win_t wt, byte_t b);

XDL_API byte_t	widget_get_alpha(res_win_t wt);

XDL_API void	widget_set_region(res_win_t wt, res_rgn_t rgn);
#endif


#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_WIDGET*/

#endif /*IMPWIN_H*/