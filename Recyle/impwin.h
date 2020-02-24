/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget document

	@module	impwin.h | widget interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#ifdef XDL_SUPPORT_WIDGET

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API res_win_t widget_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev);

XDL_API	void	widget_destroy(res_win_t wt);

XDL_API	void	widget_close(res_win_t wt, int ret);

XDL_API if_event_t* widget_get_dispatch(res_win_t wt);

XDL_API	bool_t	widget_has_struct(res_win_t wt);

XDL_API void	widget_set_style(res_win_t wt, dword_t ws);

XDL_API dword_t widget_get_style(res_win_t wt);

XDL_API void	widget_set_core_delta(res_win_t wt, void* pd);

XDL_API void*	widget_get_core_delta(res_win_t wt);

XDL_API void	widget_set_user_delta(res_win_t wt, void* pd);

XDL_API void*	widget_get_user_delta(res_win_t wt);

XDL_API void	widget_set_user_id(res_win_t wt, dword_t uid);

XDL_API dword_t widget_get_user_id(res_win_t wt);

XDL_API int		widget_get_return(res_win_t wt);

XDL_API void	widget_set_owner(res_win_t wt, res_win_t owner);

XDL_API res_win_t widget_get_owner(res_win_t wt);

XDL_API res_win_t widget_get_child(res_win_t wt, dword_t uid);

XDL_API res_win_t widget_get_parent(res_win_t wt);

XDL_API void	widget_set_user_prop(res_win_t wt, const tchar_t* pname, void* pval);

XDL_API void*	widget_get_user_prop(res_win_t wt, const tchar_t* pname);

XDL_API void*	widget_del_user_prop(res_win_t wt, const tchar_t* pname);

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

XDL_API void	widget_client_to_viewbox(res_win_t wt, xpoint_t* pst);

XDL_API void	widget_viewbox_to_client(res_win_t wt, xpoint_t* pst);

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

XDL_API void	widget_reset_scroll(res_win_t wt, bool_t bHorz);

XDL_API bool_t	widget_has_border(res_win_t wt);

XDL_API int		widget_send_notice(res_win_t wt, NOTICE* pnt);

XDL_API void	widget_post_command(res_win_t wt, int code, int cid, void* data);

XDL_API int		widget_send_command(res_win_t wt, int code, int cid, void* data);

XDL_API void	widget_set_title(res_win_t wt, const tchar_t* token);

XDL_API int		widget_get_title(res_win_t wt, tchar_t* buf, int max);

XDL_API bool_t	widget_set_subproc(res_win_t wt, dword_t sid, if_subproc_t* sub);

XDL_API void	widget_del_subproc(res_win_t wt, dword_t sid);

XDL_API bool_t	widget_set_subproc_delta(res_win_t wt, dword_t sid, void*);

XDL_API void*	widget_get_subproc_delta(res_win_t wt, dword_t sid);

XDL_API bool_t	widget_has_subproc(res_win_t wt);

XDL_API void	widget_copy(res_win_t wt);

XDL_API void	widget_cut(res_win_t wt);

XDL_API void	widget_paste(res_win_t wt);

XDL_API void	widget_undo(res_win_t wt);

XDL_API void	widget_reset_paging(res_win_t wt, long ww, long wh, long vw, long vh, long lw, long lh);

XDL_API void	widget_ensure_visible(res_win_t wt, const xrect_t* pxr, bool_t scroll);

XDL_API void	widget_hand_command_switch(res_win_t wt, bool_t bkey);

XDL_API void	widget_set_xfont(res_win_t wt, const xfont_t* pxf);

XDL_API void	widget_get_xfont(res_win_t wt, xfont_t* pxf);

XDL_API const xfont_t*	widget_get_xfont_ptr(res_win_t wt);

XDL_API void	widget_set_xface(res_win_t wt, const xface_t* pxa);

XDL_API void	widget_get_xface(res_win_t wt, xface_t* pxa);

XDL_API const xface_t*	widget_get_xface_ptr(res_win_t wt);

XDL_API void	widget_set_xbrush(res_win_t wt, const xbrush_t* pxb);

XDL_API void	widget_get_xbrush(res_win_t wt, xbrush_t* pxb);

XDL_API const xbrush_t*	widget_get_xbrush_ptr(res_win_t wt);

XDL_API void	widget_set_xpen(res_win_t wt, const xpen_t* pxp);

XDL_API void	widget_get_xpen(res_win_t wt, xpen_t* pxp);

XDL_API const xpen_t*	widget_get_xpen_ptr(res_win_t wt);

XDL_API void	widget_set_xcolor(res_win_t wt, const xcolor_t* pxc);

XDL_API void	widget_get_xcolor(res_win_t wt, xcolor_t* pxc);

XDL_API const xcolor_t*	widget_get_xcolor_ptr(res_win_t wt);

XDL_API void	widget_set_point(res_win_t wt, const xpoint_t* ppt);

XDL_API void	widget_get_point(res_win_t wt, xpoint_t* ppt);

XDL_API void	widget_set_color_mode(res_win_t wt, const clr_mod_t* pclr);

XDL_API void	widget_get_color_mode(res_win_t wt, clr_mod_t* pclr);

XDL_API void	widget_get_color_mode(res_win_t wt, clr_mod_t* pclr);

XDL_API void	widget_attach_logo(res_win_t wt, res_bmp_t logo);

XDL_API res_bmp_t	widget_get_logo(res_win_t wt);

XDL_API res_bmp_t widget_detach_logo(res_win_t wt);

XDL_API void	widget_attach_accel(res_win_t wt, res_acl_t acl);

XDL_API res_acl_t widget_get_accel(res_win_t wt);

XDL_API res_acl_t widget_detach_accel(res_win_t wt);

XDL_API object_t widget_get_canvas(res_win_t wt);

XDL_API void	widget_attach_splitor(res_win_t wt, link_t_ptr split);

XDL_API link_t_ptr	widget_get_splitor(res_win_t wt);

XDL_API link_t_ptr	widget_detach_splitor(res_win_t wt);

XDL_API void widget_layout_splitor(res_win_t widget);

XDL_API bool_t widget_dock(res_win_t wt, dword_t style, long cx, long cy);

XDL_API void widget_undock(res_win_t wt, dword_t style);

XDL_API void widget_get_dock_rect(res_win_t wt, dword_t style, xrect_t* pxr);

XDL_API void	widget_rect_to_pt(res_win_t wt, xrect_t* pxr);

XDL_API void	widget_rect_to_tm(res_win_t wt, xrect_t* pxr);

XDL_API void	widget_size_to_pt(res_win_t wt, xsize_t* pxs);

XDL_API void	widget_size_to_tm(res_win_t wt, xsize_t* pxs);

XDL_API void	widget_point_to_pt(res_win_t wt, xpoint_t* ppt);

XDL_API void	widget_point_to_tm(res_win_t wt, xpoint_t* ppt);

XDL_API void	widget_get_view_rect(res_win_t wt, viewbox_t* pbox);

XDL_API void	widget_get_canv_rect(res_win_t wt, canvbox_t* pbox);

XDL_API void	widget_get_canv_size(res_win_t wt, xsize_t* pps);

XDL_API long_ptr widget_set_timer(res_win_t wt, dword_t ms);

XDL_API void	widget_kill_timer(res_win_t wt, long_ptr tid);

XDL_API bool_t	widget_point_corner(res_win_t wt, const xpoint_t* ppt);

XDL_API int		widget_do_modal(res_win_t wt);

XDL_API void	widget_do_trace(res_win_t wt);

XDL_API bool_t	widget_enum_child(res_win_t wt, PF_ENUM_WINDOW_PROC pf, long_ptr pv);

#ifdef XDL_SUPPORT_WIDGET_NC
XDL_API void	widget_attach_menu(res_win_t wt, link_t_ptr menu);

XDL_API link_t_ptr	widget_get_menu(res_win_t wt);

XDL_API link_t_ptr	widget_detach_menu(res_win_t wt);

XDL_API void	widget_menu_item_rect(res_win_t wt, int iid, xrect_t* pxr);
#endif

#ifdef XDL_SUPPORT_WIDGET_EX
XDL_API void	widget_set_alpha(res_win_t wt, byte_t b);

XDL_API byte_t	widget_get_alpha(res_win_t wt);

XDL_API void	widget_track_mouse(res_win_t wt, dword_t mask);

XDL_API bool_t	widget_is_maximized(res_win_t wt);

XDL_API bool_t	widget_is_minimized(res_win_t wt);

XDL_API void	widget_set_region(res_win_t wt, res_rgn_t rgn);

#endif

XDL_API void	widget_hand_create(res_win_t wt);

XDL_API void	widget_hand_destroy(res_win_t wt);

XDL_API void	widget_hand_scroll(res_win_t wt, bool_t b_horz, long pos);


/******************************widget splitor implement*****************************************************/
XDL_API void widget_splitor_on_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp);

XDL_API void widget_splitor_on_lbuttondown(res_win_t widget, const xpoint_t* pxp);

XDL_API void widget_splitor_on_lbuttonup(res_win_t widget, const xpoint_t* pxp);

XDL_API void widget_splitor_on_size(res_win_t widget, int code, const xsize_t* pxs);

XDL_API void widget_splitor_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt);

XDL_API int widget_splitor_sub_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp, dword_t sid, void* delta);

XDL_API int widget_splitor_sub_lbuttondown(res_win_t widget, const xpoint_t* pxp, dword_t sid, void* delta);

XDL_API int widget_splitor_sub_lbuttonup(res_win_t widget, const xpoint_t* pxp, dword_t sid, void* delta);

XDL_API int widget_splitor_sub_size(res_win_t widget, int code, const xsize_t* pxs, dword_t sid, void* delta);

XDL_API int widget_splitor_sub_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, dword_t sid, void* delta);

/******************************widget docker implement*****************************************************/

XDL_API void widget_docker_on_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp);

XDL_API void widget_docker_on_lbuttondown(res_win_t widget, const xpoint_t* pxp);

XDL_API void widget_docker_on_lbuttonup(res_win_t widget, const xpoint_t* pxp);

XDL_API void widget_docker_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt);

XDL_API int widget_docker_sub_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp, dword_t sid, void* delta);

XDL_API int widget_docker_sub_lbuttondown(res_win_t widget, const xpoint_t* pxp, dword_t sid, void* delta);

XDL_API int widget_docker_sub_lbuttonup(res_win_t widget, const xpoint_t* pxp, dword_t sid, void* delta);

XDL_API int widget_docker_sub_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, dword_t sid, void* delta);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_WIDGET*/

#endif /*IMPWIN_H*/