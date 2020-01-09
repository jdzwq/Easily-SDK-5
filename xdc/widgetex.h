/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget extension document

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
@FUNCTION widgetex_has_struct: test is a xdc widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN bool_t: return nonzero for being a xdc widget.
*/
XDC_API	bool_t	widgetex_has_struct(res_win_t wt);

/*
@FUNCTION widgetex_set_xfont: set the widget font.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xfont_t* pxf: the font struct.
@RETURN void: none.
*/
XDC_API void	widgetex_set_xfont(res_win_t wt, const xfont_t* pxf);

/*
@FUNCTION widgetex_get_xfont: copy the widget font.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xfont_t* pxf: the font struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_xfont(res_win_t wt, xfont_t* pxf);

/*
@FUNCTION widgetex_get_xfont_ptr: get the widget font.
@INPUT res_win_t wt: windowd resource handle.
@RETURN const xfont_t*: return the widget font struct if exists, otherwise return NULL.
*/
XDC_API const xfont_t*	widgetex_get_xfont_ptr(res_win_t wt);

/*
@FUNCTION widgetex_set_xface: set the widget face.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xface_t* pxa: the face struct.
@RETURN void: none.
*/
XDC_API void	widgetex_set_xface(res_win_t wt, const xface_t* pxa);

/*
@FUNCTION widgetex_get_xface: copy the widget face.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xface_t* pxa: the face struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_xface(res_win_t wt, xface_t* pxa);

/*
@FUNCTION widgetex_get_xface_ptr: get the widget face.
@INPUT res_win_t wt: windowd resource handle.
@RETURN const xface_t*: return the widget face struct if exists, otherwise return NULL.
*/
XDC_API const xface_t*	widgetex_get_xface_ptr(res_win_t wt);

/*
@FUNCTION widgetex_set_xbrush: set the widget brush.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xbrush_t* pxb: the brush struct.
@RETURN void: none.
*/
XDC_API void	widgetex_set_xbrush(res_win_t wt, const xbrush_t* pxb);

/*
@FUNCTION widgetex_get_xbrush: copy the widget brush.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xbrush_t* pxb: the brush struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_xbrush(res_win_t wt, xbrush_t* pxb);

/*
@FUNCTION widgetex_get_xbrush_ptr: get the widget brush.
@INPUT res_win_t wt: windowd resource handle.
@RETURN const xbrush_t*: return the widget brush struct if exists, otherwise return NULL.
*/
XDC_API const xbrush_t*	widgetex_get_xbrush_ptr(res_win_t wt);

/*
@FUNCTION widgetex_set_xpen: set the widget pen.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xpen_t* pxp: the pen struct.
@RETURN void: none.
*/
XDC_API void	widgetex_set_xpen(res_win_t wt, const xpen_t* pxp);

/*
@FUNCTION widgetex_get_xpen: copy the widget pen.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xpen_t* pxp: the pen struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_xpen(res_win_t wt, xpen_t* pxp);

/*
@FUNCTION widgetex_get_xpen_ptr: get the widget pen.
@INPUT res_win_t wt: windowd resource handle.
@RETURN const xpen_t*: return the widget pen struct if exists, otherwise return NULL.
*/
XDC_API const xpen_t*	widgetex_get_xpen_ptr(res_win_t wt);

/*
@FUNCTION widgetex_set_mask: set the widget mask color.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xcolor_t* pxc: the color struct.
@RETURN void: none.
*/
XDC_API void	widgetex_set_mask(res_win_t wt, const xcolor_t* pxc);

/*
@FUNCTION widgetex_get_mask: copy the widget mask color.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xcolor_t* pxc: the color struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_mask(res_win_t wt, xcolor_t* pxc);

/*
@FUNCTION widgetex_get_mask_ptr: get the widget mask color.
@INPUT res_win_t wt: windowd resource handle.
@RETURN const xcolor_t*: return the widget color struct if exists, otherwise return NULL.
*/
XDC_API const xcolor_t*	widgetex_get_mask_ptr(res_win_t wt);

/*
@FUNCTION widgetex_set_iconic: set the widget icon color.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xcolor_t* pxc: the color struct.
@RETURN void: none.
*/
XDC_API void	widgetex_set_iconic(res_win_t wt, const xcolor_t* pxc);

/*
@FUNCTION widgetex_get_iconic: copy the widget icon color.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xcolor_t* pxc: the color struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_iconic(res_win_t wt, xcolor_t* pxc);

/*
@FUNCTION widgetex_get_iconic_ptr: get the widget mask color.
@INPUT res_win_t wt: windowd resource handle.
@RETURN const xcolor_t*: return the widget color struct if exists, otherwise return NULL.
*/
XDC_API const xcolor_t*	widgetex_get_iconic_ptr(res_win_t wt);

/*
@FUNCTION widgetex_set_point: set the child widget position in client coordinate.
@INPUT res_win_t wt: windowd resource handle.
@RETURN const xcolor_t*: return the widget color struct if exists, otherwise return NULL.
*/
XDC_API void	widgetex_set_point(res_win_t wt, const xpoint_t* ppt);

/*
@FUNCTION widgetex_get_point: get the child widget position in client coordinate.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xpoint_t* ppt: the point struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_point(res_win_t wt, xpoint_t* ppt);

/*
@FUNCTION widgetex_set_color_mode: set the widget color mode.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const clr_mod_t* pclr: the color mode struct.
@RETURN void: none.
*/
XDC_API void	widgetex_set_color_mode(res_win_t wt, const clr_mod_t* pclr);

/*
@FUNCTION widgetex_get_color_mode: copy the widget color mode.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT clr_mod_t* pclr: the color mode struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_color_mode(res_win_t wt, clr_mod_t* pclr);

/*
@FUNCTION widgetex_attach_splitor: attach a splitor document to widget.
@INPUT res_win_t wt: windowd resource handle.
@INPUT link_t_ptr split: the splitor link component.
@RETURN void: none.
*/
XDC_API void	widgetex_attach_splitor(res_win_t wt, link_t_ptr split);

/*
@FUNCTION widgetex_get_splitor: get the splitor document from widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN link_t_ptr: return the splitor link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr	widgetex_get_splitor(res_win_t wt);

/*
@FUNCTION widgetex_detach_splitor: detach the splitor document from widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN link_t_ptr: return the splitor link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr	widgetex_detach_splitor(res_win_t wt);

/*
@FUNCTION widgetex_layout_splitor: recalc and redraw the splitor in widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN void: none.
*/
XDC_API void	widgetex_layout_splitor(res_win_t wt);

/*
@FUNCTION widgetex_dock: add a dock to widget.
@INPUT res_win_t wt: windowd resource handle.
@INPUT dword_t style: the dock style, it can be WD_DOCK_TOP, WD_DOCK_BOTTOM, WD_DOCK_LEFT, WD_DOCK_RIGHT, WD_DOCK_DYNA.
@INPUT long cx: the dock width.
@INPUT long cy: the dock height.
@RETURN bool_t: return nonzero if added, return zero if exists.
*/
XDC_API bool_t	widgetex_dock(res_win_t wt, dword_t style, long cx, long cy);

/*
@FUNCTION widgetex_undock: remove a dock from widget.
@INPUT res_win_t wt: windowd resource handle.
@INPUT dword_t style: the dock style, it can be WD_DOCK_TOP, WD_DOCK_BOTTOM, WD_DOCK_LEFT, WD_DOCK_RIGHT, WD_DOCK_DYNA.
@RETURN bool_t: return nonzero if added, return zero if exists.
*/
XDC_API void	widgetex_undock(res_win_t wt, dword_t style);

/*
@FUNCTION widgetex_get_dock_rect: get the dock rect.
@INPUT res_win_t wt: windowd resource handle.
@INPUT dword_t style: the dock style, it can be WD_DOCK_TOP, WD_DOCK_BOTTOM, WD_DOCK_LEFT, WD_DOCK_RIGHT, WD_DOCK_DYNA.
@OUTPUT xrect_t* pxr: the rect struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_dock_rect(res_win_t wt, dword_t style, xrect_t* pxr);

/*
@FUNCTION widgetex_rect_to_pt: mapping rect millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xrect_t* pxr: the rect struct for inputing millimeter value and outping points value.
@RETURN void: none.
*/
XDC_API void	widgetex_rect_to_pt(res_win_t wt, xrect_t* pxr);

/*
@FUNCTION widgetex_rect_to_tm: mapping rect points value to millimeter value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xrect_t* pxr: the rect struct for inputing points value and outping millimeter value.
@RETURN void: none.
*/
XDC_API void	widgetex_rect_to_tm(res_win_t wt, xrect_t* pxr);

/*
@FUNCTION widgetex_size_to_pt: mapping size millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xsize_t* pxs: the size struct for inputing millimeter value and outping points value.
@RETURN void: none.
*/
XDC_API void	widgetex_size_to_pt(res_win_t wt, xsize_t* pxs);

/*
@FUNCTION widgetex_size_to_tm: mapping rect millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xsize_t* pxs: the size struct for inputing points value and outping millimeter value.
@RETURN void: none.
*/
XDC_API void	widgetex_size_to_tm(res_win_t wt, xsize_t* pxs);

/*
@FUNCTION widgetex_point_to_pt: mapping point millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xpoint_t* ppt: the point struct for inputing millimeter value and outping points value.
@RETURN void: none.
*/
XDC_API void	widgetex_point_to_pt(res_win_t wt, xpoint_t* ppt);

/*
@FUNCTION widgetex_point_to_tm: mapping point millimeter value to points value.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xpoint_t* ppt: the point struct for inputing points value and outping millimeter value.
@RETURN void: none.
*/
XDC_API void	widgetex_point_to_tm(res_win_t wt, xpoint_t* ppt);

/*
@FUNCTION widgetex_attach_menu: attach a menu document to widget.
@INPUT res_win_t wt: windowd resource handle.
@INPUT link_t_ptr menu: the menu link component.
@RETURN void: none.
*/
XDC_API void	widgetex_attach_menu(res_win_t wt, link_t_ptr menu);

/*
@FUNCTION widgetex_get_menu: get a menu document from widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN link_t_ptr: return the menu link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr	widgetex_get_menu(res_win_t wt);

/*
@FUNCTION widgetex_detach_menu: detach the menu document from widget.
@INPUT res_win_t wt: windowd resource handle.
@RETURN link_t_ptr: return the menu link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr	widgetex_detach_menu(res_win_t wt);

/*
@FUNCTION widgetex_menu_item_rect: find a menu item by iid and get the item rect.
@INPUT res_win_t wt: windowd resource handle.
@INPUT int iid: the menu item iid value.
@RETURN void: none.
*/
XDC_API void	widgetex_menu_item_rect(res_win_t wt, int iid, xrect_t* pxr);

/*
@FUNCTION widgetex_get_view_rect: get the widget view rect.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT viewbox_t* pbox: the view box struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_view_rect(res_win_t wt, viewbox_t* pbox);

/*
@FUNCTION widgetex_client_to_viewbox: mapping widget client coordinate to viewbox coordinate.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xpoint_t* ppt: the point struct.
@RETURN void: none.
*/
XDC_API void	widgetex_client_to_viewbox(res_win_t wt, xpoint_t* ppt);

/*
@FUNCTION widgetex_viewbox_to_client: mapping widget viewbox coordinate to client coordinate.
@INPUT res_win_t wt: windowd resource handle.
@INOUTPUT xpoint_t* ppt: the point struct.
@RETURN void: none.
*/
XDC_API void	widgetex_viewbox_to_client(res_win_t wt, xpoint_t* pst);

/*
@FUNCTION widgetex_get_canv_rect: get the widget canvas rect.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT canvbox_t* pbox: the canvas box struct.
@RETURN void: none.
*/
XDC_API void	widgetex_get_canv_rect(res_win_t wt, canvbox_t* pbox);

/*
@FUNCTION widgetex_get_canv_size: get the widget canvas size.
@INPUT res_win_t wt: windowd resource handle.
@OUTPUT xsize_t* pxs: the size struct using millimeter member.
@RETURN void: none.
*/
XDC_API void	widgetex_get_canv_size(res_win_t wt, xsize_t* pxs);

/*
@FUNCTION widgetex_reset_paging: reset the widget page face.
@INPUT res_win_t wt: windowd resource handle.
@INPUT long ww: the widget client width in points.
@INPUT long wh: the widget client height in points.
@INPUT long vw: the widget viewbox width in points.
@INPUT long vh: the widget viewbox height in points.
@INPUT long lw: the widget line width in points.
@INPUT long lh: the widget line height in points.
@RETURN void: none.
*/
XDC_API void	widgetex_reset_paging(res_win_t wt, long ww, long wh, long vw, long vh, long lw, long lh);

/*
@FUNCTION widgetex_ensure_visible: ensure the rect scrolling into widget visual area.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xrect_t* pxr: the rect in widget client.
@INPUT bool_t scroll: nonzero for redrawing scroll bar.
@RETURN void: none.
*/
XDC_API void	widgetex_ensure_visible(res_win_t wt, const xrect_t* pxr, bool_t scroll);

/*
@FUNCTION widgetex_point_corner: test point is in widget corner.
@INPUT res_win_t wt: windowd resource handle.
@INPUT const xpoint_t* ppt: the point struct.
@RETURN bool_t: return nonzero if the point in widget corner.
*/
XDC_API bool_t	widgetex_point_corner(res_win_t wt, const xpoint_t* ppt);



#ifdef	__cplusplus
}
#endif


#endif /*WIDGETEX_H*/