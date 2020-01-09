/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc split view document

	@module	splitview.h | split view interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#ifndef _SPLITVIEW_H
#define _SPLITVIEW_H

#include "xdcdef.h"

/***************************************runtime page size***********************************/
#define get_split_page_width(ptr)					get_dom_page_width(ptr)

#define set_split_page_width(ptr,n)					set_dom_page_width(ptr,n)

#define set_split_page_height(ptr,n)				set_dom_page_height(ptr,n)

#define get_split_page_height(ptr)					get_dom_page_height(ptr)

/********************************************************************************************/
void	resize_split_item(link_t_ptr ilk);

void	calc_split_item_rect(link_t_ptr ptr,link_t_ptr ilk,xrect_t* pxr);

int		calc_split_point_hint(link_t_ptr ptr,const xpoint_t* ppt,link_t_ptr* pilk);

void	draw_split(xhand_t canv,link_t_ptr ptr,const xpaint_t* pxn);

/*********************************************************************************************/
XDC_API void	hand_split_create(res_win_t widget);

XDC_API void	hand_split_destroy(res_win_t widget);

XDC_API void	hand_split_erase(res_win_t widget, res_dc_t dc, const xrect_t* pxr);

XDC_API void	hand_split_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr);

XDC_API void	hand_split_size(res_win_t widget, const resize_t* prs);

XDC_API void	hand_split_mouse_move(res_win_t widget, dword_t dw, short x, short y);

XDC_API void	hand_split_lbutton_down(res_win_t widget, short x, short y);

XDC_API void	hand_split_lbutton_up(res_win_t widget, short x, short y);

/*********************************************************************************************/
XDC_API res_win_t	split_view_create(const wchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void		split_view_set_owner(res_win_t widget, res_win_t owner);

XDC_API res_win_t	split_view_get_owner(res_win_t widget);

XDC_API xhand_t		split_view_get_canvas(res_win_t widget);

XDC_API void		split_view_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr	split_view_detach(res_win_t widget);

XDC_API link_t_ptr	split_view_fetch(res_win_t widget);

XDC_API void		split_view_redraw(res_win_t widget);

XDC_API void		split_view_tm_to_pt(res_win_t widget, xpoint_t *ppt);

XDC_API void		split_view_pt_to_tm(res_win_t widget, xpoint_t *ppt);

#endif