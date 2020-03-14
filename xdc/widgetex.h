/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widgetex extension document

	@module	widgetex.h | interface file

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

#ifndef _WIDGETEX_H
#define _WIDGETEX_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION widget_get_canvas: get the canvas from widget.
@INPUT res_win_t wt: the widget resource handle.
@RETURN canvas_t: return canvas handle if exists, otherwise return zero.
*/
EXP_API canvas_t widget_get_canvas(res_win_t wt);

/*
@FUNCTION widget_attach_splitor: attach a splitor document to widget.
@INPUT res_win_t wt: windowd resource handle.
@INPUT link_t_ptr split: the splitor link component.
@RETURN void: none.
*/
EXP_API void	widget_attach_splitor(res_win_t wt, link_t_ptr split);

/*
@FUNCTION widget_get_splitor: get the splitor document from widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN link_t_ptr: return the splitor link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr	widget_get_splitor(res_win_t wt);

/*
@FUNCTION widget_detach_splitor: detach the splitor document from widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN link_t_ptr: return the splitor link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr	widget_detach_splitor(res_win_t wt);

/*
@FUNCTION widget_layout_splitor: recalc and redraw the splitor in widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN void: none.
*/
EXP_API void	widget_layout_splitor(res_win_t wt);

/*
@FUNCTION widget_dock: add a dock to widget.
@INPUT res_win_t wt: windowd resource handle.
@INPUT dword_t style: the dock style, it can be WS_DOCK_TOP, WS_DOCK_BOTTOM, WS_DOCK_LEFT, WS_DOCK_RIGHT, WS_DOCK_DYNA.
@INPUT int cx: the dock width.
@INPUT int cy: the dock height.
@RETURN bool_t: return nonzero if added, return zero if exists.
*/
EXP_API bool_t	widget_dock(res_win_t wt, dword_t style, int cx, int cy);

/*
@FUNCTION widget_undock: remove a dock from widget.
@INPUT res_win_t wt: windowd resource handle.
@INPUT dword_t style: the dock style, it can be WS_DOCK_TOP, WS_DOCK_BOTTOM, WS_DOCK_LEFT, WS_DOCK_RIGHT, WS_DOCK_DYNA.
@RETURN bool_t: return nonzero if added, return zero if exists.
*/
EXP_API void	widget_undock(res_win_t wt, dword_t style);

/*
@FUNCTION widget_get_dock_rect: get the dock rect.
@INPUT res_win_t wt: windowd resource handle.
@INPUT dword_t style: the dock style, it can be WS_DOCK_TOP, WS_DOCK_BOTTOM, WS_DOCK_LEFT, WS_DOCK_RIGHT, WS_DOCK_DYNA.
@OUTPUT xrect_t* pxr: the rect struct.
@RETURN void: none.
*/
EXP_API void	widget_get_dock_rect(res_win_t wt, dword_t style, xrect_t* pxr);

/*
@FUNCTION widget_rect_to_pt: mapping rect millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xrect_t* pxr: the rect struct for inputing millimeter value and outping points value.
@RETURN void: none.
*/
EXP_API void	widget_rect_to_pt(res_win_t wt, xrect_t* pxr);

/*
@FUNCTION widget_rect_to_tm: mapping rect points value to millimeter value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xrect_t* pxr: the rect struct for inputing points value and outping millimeter value.
@RETURN void: none.
*/
EXP_API void	widget_rect_to_tm(res_win_t wt, xrect_t* pxr);

/*
@FUNCTION widget_size_to_pt: mapping size millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xsize_t* pxs: the size struct for inputing millimeter value and outping points value.
@RETURN void: none.
*/
EXP_API void	widget_size_to_pt(res_win_t wt, xsize_t* pxs);

/*
@FUNCTION widget_size_to_tm: mapping rect millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xsize_t* pxs: the size struct for inputing points value and outping millimeter value.
@RETURN void: none.
*/
EXP_API void	widget_size_to_tm(res_win_t wt, xsize_t* pxs);

/*
@FUNCTION widget_point_to_pt: mapping point millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xpoint_t* ppt: the point struct for inputing millimeter value and outping points value.
@RETURN void: none.
*/
EXP_API void	widget_point_to_pt(res_win_t wt, xpoint_t* ppt);

/*
@FUNCTION widget_point_to_tm: mapping point millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xpoint_t* ppt: the point struct for inputing points value and outping millimeter value.
@RETURN void: none.
*/
EXP_API void	widget_point_to_tm(res_win_t wt, xpoint_t* ppt);

/*
@FUNCTION widget_attach_menu: attach a menu document to widget.
@INPUT res_win_t wt: windowd resource handle.
@INPUT link_t_ptr menu: the menu link component.
@RETURN void: none.
*/
EXP_API void	widget_attach_menu(res_win_t wt, link_t_ptr menu);

/*
@FUNCTION widget_get_menu: get a menu document from widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN link_t_ptr: return the menu link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr	widget_get_menu(res_win_t wt);

/*
@FUNCTION widget_detach_menu: detach the menu document from widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN link_t_ptr: return the menu link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr	widget_detach_menu(res_win_t wt);

/*
@FUNCTION widget_menu_item_rect: find a menu item by iid and get the item rect.
@INPUT res_win_t wt: windowd resource handle.
@INPUT int iid: the menu item iid value.
@RETURN void: none.
*/
EXP_API void	widget_menu_item_rect(res_win_t wt, int iid, xrect_t* pxr);

/*
@FUNCTION widget_get_view_rect: get the widget view rect.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT viewbox_t* pbox: the view box struct.
@RETURN void: none.
*/
EXP_API void	widget_get_view_rect(res_win_t wt, viewbox_t* pbox);

/*
@FUNCTION widget_client_to_viewbox: mapping widget client coordinate to viewbox coordinate.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xpoint_t* ppt: the point struct.
@RETURN void: none.
*/
EXP_API void	widget_client_to_viewbox(res_win_t wt, xpoint_t* ppt);

/*
@FUNCTION widget_viewbox_to_client: mapping widget viewbox coordinate to client coordinate.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xpoint_t* ppt: the point struct.
@RETURN void: none.
*/
EXP_API void	widget_viewbox_to_client(res_win_t wt, xpoint_t* pst);

/*
@FUNCTION widget_get_canv_rect: get the widget canvas rect.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT canvbox_t* pbox: the canvas box struct.
@RETURN void: none.
*/
EXP_API void	widget_get_canv_rect(res_win_t wt, canvbox_t* pbox);

/*
@FUNCTION widget_get_canv_size: get the widget canvas size.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xsize_t* pxs: the size struct using millimeter member.
@RETURN void: none.
*/
EXP_API void	widget_get_canv_size(res_win_t wt, xsize_t* pxs);

/*
@FUNCTION widget_reset_scroll: reset widget scroll.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT bool_t horz: zero for vert scroll, zero for horz scroll.
@RETURN void: none.
*/
EXP_API	void	widget_reset_scroll(res_win_t wt, bool_t horz);

/*
@FUNCTION widget_reset_paging: reset the widget page face.
@INPUT res_win_t wt: windowd resource handle.
@INPUT int ww: the widget client width in points.
@INPUT int wh: the widget client height in points.
@INPUT int vw: the widget viewbox width in points.
@INPUT int vh: the widget viewbox height in points.
@INPUT int lw: the widget line width in points.
@INPUT int lh: the widget line height in points.
@RETURN void: none.
*/
EXP_API void	widget_reset_paging(res_win_t wt, int ww, int wh, int vw, int vh, int lw, int lh);

/*
@FUNCTION widget_ensure_visible: ensure the rect scrolling into widget visual area.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xrect_t* pxr: the rect in widget client.
@INPUT bool_t scroll: nonzero for redrawing scroll bar.
@RETURN void: none.
*/
EXP_API void	widget_ensure_visible(res_win_t wt, const xrect_t* pxr, bool_t scroll);

/*
@FUNCTION widget_point_corner: test point is in widget corner.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xpoint_t* ppt: the point struct.
@RETURN bool_t: return nonzero if the point in widget corner.
*/
EXP_API bool_t	widget_point_corner(res_win_t wt, const xpoint_t* ppt);

/******************************widget event implement*****************************************************/

EXP_API void	widget_hand_create(res_win_t wt);

EXP_API void	widget_hand_destroy(res_win_t wt);

EXP_API bool_t	widget_hand_scroll(res_win_t wt, bool_t b_horz, int pos);

/******************************widget splitor implement*****************************************************/

EXP_API void widget_splitor_on_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp);

EXP_API void widget_splitor_on_lbuttondown(res_win_t widget, const xpoint_t* pxp);

EXP_API void widget_splitor_on_lbuttonup(res_win_t widget, const xpoint_t* pxp);

EXP_API void widget_splitor_on_size(res_win_t widget, int code, const xsize_t* pxs);

EXP_API void widget_splitor_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt);

EXP_API int widget_splitor_sub_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp, uid_t sid, var_long delta);

EXP_API int widget_splitor_sub_lbuttondown(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

EXP_API int widget_splitor_sub_lbuttonup(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

EXP_API int widget_splitor_sub_size(res_win_t widget, int code, const xsize_t* pxs, uid_t sid, var_long delta);

EXP_API int widget_splitor_sub_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_long delta);

/******************************widget docker implement*****************************************************/

EXP_API void widget_docker_on_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp);

EXP_API void widget_docker_on_lbuttondown(res_win_t widget, const xpoint_t* pxp);

EXP_API void widget_docker_on_lbuttonup(res_win_t widget, const xpoint_t* pxp);

EXP_API void widget_docker_on_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt);

EXP_API int widget_docker_sub_mousemove(res_win_t widget, dword_t dw, const xpoint_t* pxp, uid_t sid, var_long delta);

EXP_API int widget_docker_sub_lbuttondown(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

EXP_API int widget_docker_sub_lbuttonup(res_win_t widget, const xpoint_t* pxp, uid_t sid, var_long delta);

EXP_API int widget_docker_sub_paint(res_win_t widget, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_long delta);

#ifdef	__cplusplus
}
#endif


#endif /*WIDGETEX_H*/