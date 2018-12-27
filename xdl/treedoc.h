/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tree document

	@module	treedoc.h | tree document interface file

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

#ifndef _TREEDOC_H
#define _TREEDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**************************************serial enable attributes**************************************/
#define get_tree_style_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_tree_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_tree_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_tree_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define set_tree_title(ptr,token)						set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)

#define get_tree_title_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)

#define get_tree_title_height(ptr)						get_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT)

#define set_tree_title_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT,n)

#define get_tree_title_image_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_IMAGE,-1)

#define set_tree_title_image(ptr,token)					set_dom_node_attr(ptr,ATTR_IMAGE,-1,token,-1)

#define set_tree_icon_span(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)

#define get_tree_icon_span(ptr)							get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)

#define get_tree_item_height(ptr)						get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)

#define set_tree_item_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)

#define set_tree_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_tree_item_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_tree_item_title(ilk,token)					set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)

#define get_tree_item_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)

#define get_tree_item_showcheck(ilk)					get_dom_node_attr_boolean(ilk,ATTR_SHOWCHECK)

#define set_tree_item_showcheck(ilk,b)					set_dom_node_attr_boolean(ilk,ATTR_SHOWCHECK,b)

#define get_tree_item_image_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_IMAGE,-1)

#define set_tree_item_image(ilk,token)					set_dom_node_attr(ilk,ATTR_IMAGE,-1,token,-1)

/********************************************runtime enable attributes***********************************/
#define set_tree_images(ptr,images)						set_dom_node_images(ptr,images)

#define get_tree_images(ptr)							get_dom_node_images(ptr)

#define set_tree_item_locked(ilk,c)						set_dom_node_mask_check(ilk,MSK_LOCKED,c) 

#define get_tree_item_locked(ilk)						get_dom_node_mask_check(ilk,MSK_LOCKED)

#define set_tree_item_expanded(ilk,expand)				set_dom_node_mask_check(ilk,MSK_EXPANDED,expand)

#define get_tree_item_expanded(ilk)						get_dom_node_mask_check(ilk,MSK_EXPANDED)

#define set_tree_item_checked(ilk,check)				set_dom_node_mask_check(ilk,MSK_CHECKED,check) 

#define get_tree_item_checked(ilk)						get_dom_node_mask_check(ilk,MSK_CHECKED)

#define set_tree_item_delta(ilk,ul)						set_dom_node_delta(ilk,(var_long)ul)

#define get_tree_item_delta(ilk)						get_dom_node_delta(ilk)
/*******************************************runtime function*******************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_tree_doc();

XDL_API void destroy_tree_doc(link_t_ptr ptr);

XDL_API bool_t is_tree_doc(link_t_ptr ptr);

XDL_API void clear_tree_doc(link_t_ptr ptr);

XDL_API bool_t is_tree_item(link_t_ptr ptr,link_t_ptr ilk);

XDL_API link_t_ptr insert_tree_item(link_t_ptr ilk,link_t_ptr pos);

XDL_API void delete_tree_item(link_t_ptr ilk);

XDL_API void delete_tree_child_items(link_t_ptr ilk);

XDL_API int get_tree_child_item_count(link_t_ptr ilk);

XDL_API link_t_ptr get_tree_first_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_tree_last_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_tree_parent_item(link_t_ptr ilk);

XDL_API link_t_ptr get_tree_next_sibling_item(link_t_ptr ilk);

XDL_API link_t_ptr get_tree_prev_sibling_item(link_t_ptr ilk);

XDL_API link_t_ptr get_tree_next_visible_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_tree_prev_visible_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr find_tree_item_by_name(link_t_ptr ptr, const tchar_t* sz_name);

XDL_API link_t_ptr find_tree_item_by_title(link_t_ptr ptr, const tchar_t* sz_title);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_TREEDOC_H*/
