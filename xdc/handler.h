/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget default handler document

	@module	handler.h | widget default handler interface file

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

#ifndef _HANDLER_H
#define _HANDLER_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API	bool_t	widget_has_struct(res_win_t wt);

XDC_API void	widget_set_xfont(res_win_t wt, const xfont_t* pxf);

XDC_API void	widget_get_xfont(res_win_t wt, xfont_t* pxf);

XDC_API const xfont_t*	widget_get_xfont_ptr(res_win_t wt);

XDC_API void	widget_set_xface(res_win_t wt, const xface_t* pxa);

XDC_API void	widget_get_xface(res_win_t wt, xface_t* pxa);

XDC_API const xface_t*	widget_get_xface_ptr(res_win_t wt);

XDC_API void	widget_set_xbrush(res_win_t wt, const xbrush_t* pxb);

XDC_API void	widget_get_xbrush(res_win_t wt, xbrush_t* pxb);

XDC_API const xbrush_t*	widget_get_xbrush_ptr(res_win_t wt);

XDC_API void	widget_set_xpen(res_win_t wt, const xpen_t* pxp);

XDC_API void	widget_get_xpen(res_win_t wt, xpen_t* pxp);

XDC_API const xpen_t*	widget_get_xpen_ptr(res_win_t wt);

XDC_API void	widget_set_xcolor(res_win_t wt, const xcolor_t* pxc);

XDC_API void	widget_get_xcolor(res_win_t wt, xcolor_t* pxc);

XDC_API const xcolor_t*	widget_get_xcolor_ptr(res_win_t wt);


XDC_API void	widget_set_point(res_win_t wt, const xpoint_t* ppt);

XDC_API void	widget_get_point(res_win_t wt, xpoint_t* ppt);

XDC_API void	widget_set_color_mode(res_win_t wt, const clr_mod_t* pclr);

XDC_API void	widget_get_color_mode(res_win_t wt, clr_mod_t* pclr);

XDC_API void	widget_get_color_mode(res_win_t wt, clr_mod_t* pclr);

XDC_API void widget_attach_splitor(res_win_t wt, link_t_ptr split);

XDC_API link_t_ptr	widget_get_splitor(res_win_t wt);

XDC_API link_t_ptr	widget_detach_splitor(res_win_t wt);

XDC_API void widget_layout_splitor(res_win_t widget);

XDC_API bool_t widget_dock(res_win_t wt, dword_t style, long cx, long cy);

XDC_API void widget_undock(res_win_t wt, dword_t style);

XDC_API void widget_get_dock_rect(res_win_t wt, dword_t style, xrect_t* pxr);

XDC_API void	widget_rect_to_pt(res_win_t wt, xrect_t* pxr);

XDC_API void	widget_rect_to_tm(res_win_t wt, xrect_t* pxr);

XDC_API void	widget_size_to_pt(res_win_t wt, xsize_t* pxs);

XDC_API void	widget_size_to_tm(res_win_t wt, xsize_t* pxs);

XDC_API void	widget_point_to_pt(res_win_t wt, xpoint_t* ppt);

XDC_API void	widget_point_to_tm(res_win_t wt, xpoint_t* ppt);

XDC_API void	widget_attach_menu(res_win_t wt, link_t_ptr menu);

XDC_API link_t_ptr	widget_get_menu(res_win_t wt);

XDC_API link_t_ptr	widget_detach_menu(res_win_t wt);

XDC_API void	widget_menu_item_rect(res_win_t wt, int iid, xrect_t* pxr);

XDC_API void	widget_get_view_rect(res_win_t wt, viewbox_t* pbox);

XDC_API void	widget_client_to_viewbox(res_win_t wt, xpoint_t* pst);

XDC_API void	widget_viewbox_to_client(res_win_t wt, xpoint_t* pst);

XDC_API void	widget_get_canv_rect(res_win_t wt, canvbox_t* pbox);

XDC_API void	widget_get_canv_size(res_win_t wt, xsize_t* pps);

XDC_API void	widget_reset_paging(res_win_t wt, long ww, long wh, long vw, long vh, long lw, long lh);

XDC_API void	widget_ensure_visible(res_win_t wt, const xrect_t* pxr, bool_t scroll);

XDC_API bool_t	widget_point_corner(res_win_t wt, const xpoint_t* ppt);


XDC_API void	widget_hand_create(res_win_t wt);

XDC_API void	widget_hand_destroy(res_win_t wt);

XDC_API void	widget_hand_scroll(res_win_t wt, bool_t b_horz, long pos);


/******************************widget splitor implement*****************************************************/
XDC_API void widget_splitor_on_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp);

XDC_API void widget_splitor_on_lbuttondown(res_win_t widget, const xpoint_t* pxp);

XDC_API void widget_splitor_on_lbuttonup(res_win_t widget, const xpoint_t* pxp);

XDC_API void widget_splitor_on_size(res_win_t widget, int code, const xsize_t* pxs);

XDC_API void widget_splitor_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt);

XDC_API int widget_splitor_sub_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widget_splitor_sub_lbuttondown(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widget_splitor_sub_lbuttonup(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widget_splitor_sub_size(res_win_t widget, int code, const xsize_t* pxs, uid_t sid, var_long delta);

XDC_API int widget_splitor_sub_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_long delta);

/******************************widget docker implement*****************************************************/

XDC_API void widget_docker_on_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp);

XDC_API void widget_docker_on_lbuttondown(res_win_t widget, const xpoint_t* pxp);

XDC_API void widget_docker_on_lbuttonup(res_win_t widget, const xpoint_t* pxp);

XDC_API void widget_docker_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt);

XDC_API int widget_docker_sub_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widget_docker_sub_lbuttondown(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widget_docker_sub_lbuttonup(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

XDC_API int widget_docker_sub_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_long delta);

#ifdef	__cplusplus
}
#endif


#endif /*HANDLER_H*/