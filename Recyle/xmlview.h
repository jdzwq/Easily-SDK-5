/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xml view document

	@module	xmlview.h | xml view interface file

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

#ifndef _XMLVIEW_H
#define _XMLVIEW_H

#include "xdcdef.h"


/***************************************runtime page size***********************************/
#define get_xml_page_width(xml)						get_dom_page_width(get_xml_dom_node(xml))

#define set_xml_page_width(xml,n)					set_dom_page_width(get_xml_dom_node(xml),n)

#define set_xml_page_height(xml,n)					set_dom_page_height(get_xml_dom_node(xml),n)

#define get_xml_page_height(xml)					get_dom_page_height(get_xml_dom_node(xml))

#define get_xml_page_xoff(xml)						get_dom_page_xoff(get_xml_dom_node(xml))

#define set_xml_page_xoff(xml,n)					set_dom_page_xoff(get_xml_dom_node(xml),n)

#define get_xml_page_yoff(xml)						get_dom_page_yoff(get_xml_dom_node(xml))

#define set_xml_page_yoff(xml,n)					set_dom_page_yoff(get_xml_dom_node(xml),n)

/********************************************************************************************/
long	calc_xml_height(link_t_ptr xml);

bool_t	calc_xml_node_rect(link_t_ptr xml,link_t_ptr nlk,xrect_t* pxr);

bool_t	calc_xml_node_text_rect(link_t_ptr xml,link_t_ptr nlk,xrect_t* pxr);

int		calc_xml_point_hint(link_t_ptr xml,const xpoint_t* ppt,link_t_ptr* pnlk);

bool_t	ensure_xml_node_visible(link_t_ptr xml,link_t_ptr nlk);

void	draw_xml(xhand_t canv,link_t_ptr xml,const xpaint_t* pxn);

/*********************************************************************************************/
XDC_API res_win_t xml_view_create(const wchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	xml_view_set_owner(res_win_t widget, res_win_t owner);

XDC_API res_win_t xml_view_get_owner(res_win_t widget);

XDC_API xhand_t xml_view_get_canvas(res_win_t widget);

XDC_API void	xml_view_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr xml_view_detach(res_win_t widget);

XDC_API link_t_ptr xml_view_fetch(res_win_t widget);

XDC_API void	xml_view_redraw(res_win_t widget);

XDC_API void	xml_view_redraw_node(res_win_t widget, link_t_ptr ilk);

XDC_API bool_t	xml_view_set_focus_node(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr xml_view_get_focus_node(res_win_t widget);

XDC_API void	xml_view_tabskip(res_win_t widget, int nSkip);

XDC_API void	xml_view_get_node_rect(res_win_t widget, link_t_ptr nlk, xrect_t* prt);

XDC_API void	xml_view_tm_to_pt(res_win_t widget, xpoint_t* ppt);

XDC_API void	xml_view_pt_to_tm(res_win_t widget, xpoint_t* ppt);

#endif