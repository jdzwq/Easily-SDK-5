/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc stringdoc document

	@module	stringdoc.h | stringdoc interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#ifndef _STRDOC_H
#define _STRDOC_H

#include "xdldef.h"
#include "xdlattr.h"

#define set_string_style(ptr,token)					set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)

#define get_string_style_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_string_item_height(ptr,n)				set_dom_node_attr_integer(ptr,ATTR_ITEM_HEIGHT,n)

#define get_string_item_height(ptr)					get_dom_node_attr_integer(ptr,ATTR_ITEM_HEIGHT)

#define set_string_item_width(ptr,n)				set_dom_node_attr_integer(ptr,ATTR_ITEM_WIDTH,n)

#define get_string_item_width(ptr)					get_dom_node_attr_integer(ptr,ATTR_ITEM_WIDTH)

#define set_string_item_delta(ilk,ul)				set_dom_node_delta(ilk,ul)

#define get_string_item_delta(ilk)					get_dom_node_delta(ilk)

#define set_string_item_name(ilk,token,len)			set_dom_node_attr(ilk,ATTR_NAME,-1,token,len)

#define get_string_item_name_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_string_item_text(ilk,token,len)			set_dom_node_text(ilk,token,len)

#define get_string_item_text_ptr(ilk)				get_dom_node_text_ptr(ilk)

#define get_string_item_text(ilk,buf,max)			get_dom_node_text(ilk,buf,max)

#define set_string_item_span(ilk,n)					set_dom_node_attr_integer(ilk,ATTR_ITEM_SPAN,n)

#define get_string_item_span(ilk)					get_dom_node_attr_integer(ilk,ATTR_ITEM_SPAN)

/***********************************************************************************************************************/
#define set_string_imagelist(ptr,images)			set_dom_node_imagelist(ptr,images)

#define get_string_imagelist(ptr)					get_dom_node_imagelist(ptr)

#define set_string_item_expanded(ilk,expand)		set_dom_node_mask_check(ilk,MSK_EXPANDED,expand)

#define get_string_item_expanded(ilk)				get_dom_node_mask_check(ilk,MSK_EXPANDED)

#define set_string_item_hidden(ilk,c)				set_dom_node_mask_check(ilk,MSK_HIDDEN,c) 

#define get_string_item_hidden(ilk)					get_dom_node_mask_check(ilk,MSK_HIDDEN)

/*********************************************************************************************************************/
XDL_API link_t_ptr create_string_doc(void);

XDL_API void destroy_string_doc(link_t_ptr ptr);

XDL_API bool_t is_string_doc(link_t_ptr ptr);

XDL_API bool_t is_string_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void clear_string_doc(link_t_ptr ptr);

XDL_API void sort_string_doc(link_t_ptr ptr,int desc);

XDL_API int get_string_item_count(link_t_ptr ptr);

XDL_API int get_string_item_visible_count(link_t_ptr ptr);

XDL_API link_t_ptr insert_string_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_string_item(link_t_ptr ptr,const tchar_t* iname);

XDL_API void delete_string_item(link_t_ptr tlk);

XDL_API link_t_ptr get_string_next_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_string_prev_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_string_next_visible_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_string_prev_visible_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_string_item_at(link_t_ptr ptr, int index);

XDL_API int get_string_item_index(link_t_ptr ptr, link_t_ptr tlk);


#endif /*_STRDOC_H*/
