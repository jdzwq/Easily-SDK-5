/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tree view

	@module	treeview.h | interface file

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

#ifndef _TREEVIEW_H
#define _TREEVIEW_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_VIEW

typedef enum{
	TREE_HINT_NONE,
	TREE_HINT_EXPAND,
	TREE_HINT_CHECK,
	TREE_HINT_ITEM,
	TREE_HINT_TITLE,
}TREE_HINT_CODE;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API float calc_tree_height(link_t_ptr ptr);

	EXP_API float calc_tree_width(const if_measure_t* pif, link_t_ptr ptr);

	EXP_API bool_t calc_tree_item_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr);

	EXP_API bool_t calc_tree_item_text_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr);

	EXP_API bool_t calc_tree_item_entity_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr);

	EXP_API bool_t calc_tree_item_expand_rect(link_t_ptr ptr, link_t_ptr cur, xrect_t* pxr);

	EXP_API int calc_tree_hint(const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pilk);

	EXP_API void draw_tree(const if_canvas_t* pcanv, link_t_ptr ptr);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_VIEW*/

#endif /*_TREEVIEW_H*/
