/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tool document

	@module	tooldoc.h | tool document interface file

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

#ifndef _TOOLDOC_H
#define _TOOLDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/*******************************************serial enable attributes********************************************/
#define get_tool_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)

#define set_tool_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)

#define set_tool_style(ptr,lay)							set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)

#define get_tool_style_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

#define set_tool_icon_span(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)

#define get_tool_icon_span(ptr)							get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)

#define get_tool_title_height(ptr)						get_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT)

#define set_tool_title_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT,n)

#define get_tool_group_name_ptr(glk)					get_dom_node_attr_ptr(glk,ATTR_NAME,-1)

#define set_tool_group_name(glk,token)					set_dom_node_attr(glk,ATTR_NAME,-1,token,-1)

#define get_tool_group_title_ptr(glk)					get_dom_node_attr_ptr(glk,ATTR_TITLE,-1)

#define set_tool_group_title(glk,token)					set_dom_node_attr(glk,ATTR_TITLE,-1,token,-1)

#define get_tool_group_image_ptr(glk)					get_dom_node_attr_ptr(glk,ATTR_IMAGE,-1)

#define set_tool_group_image(glk,token)					set_dom_node_attr(glk,ATTR_IMAGE,-1,token,-1)

#define set_tool_group_show(glk,token)					set_dom_node_attr(glk,ATTR_SHOW,-1,token,-1)

#define get_tool_group_show_ptr(glk)					get_dom_node_attr_ptr(glk,ATTR_SHOW,-1)

#define set_tool_group_item_width(glk,n)				set_dom_node_attr_float(glk,ATTR_ITEM_WIDTH,n)

#define get_tool_group_item_width(glk)					get_dom_node_attr_float(glk,ATTR_ITEM_WIDTH)

#define get_tool_group_item_height(glk)					get_dom_node_attr_float(glk,ATTR_ITEM_HEIGHT)

#define set_tool_group_item_height(glk,n)				set_dom_node_attr_float(glk,ATTR_ITEM_HEIGHT,n)

#define set_tool_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_tool_item_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_tool_item_id(ilk,token)						set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)

#define get_tool_item_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)

#define set_tool_item_title(ilk,token)					set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)

#define get_tool_item_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)

#define get_tool_item_image_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_IMAGE,-1)

#define set_tool_item_image(ilk,token)					set_dom_node_attr(ilk,ATTR_IMAGE,-1,token,-1)

/***************************************************runtime attributes************************************/
#define set_tool_images(ptr,images)						set_dom_node_images(ptr,images)

#define get_tool_images(ptr)							get_dom_node_images(ptr)

#define set_tool_item_delta(ilk,ul)						set_dom_node_delta(ilk,(var_long)ul)

#define get_tool_item_delta(ilk)						get_dom_node_delta(ilk)
/*********************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_tool_doc(void);

XDL_API void destroy_tool_doc(link_t_ptr ptr);

XDL_API bool_t is_tool_doc(link_t_ptr ptr);

XDL_API bool_t is_tool_group_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void clear_tool_doc(link_t_ptr ptr);

XDL_API void merge_tool_doc(link_t_ptr ptr1, link_t_ptr ptr2);

XDL_API link_t_ptr insert_tool_group(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_tool_group(link_t_ptr ptr, const tchar_t* sz_name);

XDL_API link_t_ptr get_tool_next_group(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_tool_prev_group(link_t_ptr ptr, link_t_ptr pos);

XDL_API int get_tool_group_count(link_t_ptr ptr);

XDL_API void delete_tool_group(link_t_ptr glk);

XDL_API link_t_ptr insert_tool_group_item(link_t_ptr glk,link_t_ptr pos);

XDL_API link_t_ptr get_tool_group_next_item(link_t_ptr glk,link_t_ptr pos);

XDL_API link_t_ptr get_tool_group_prev_item(link_t_ptr glk,link_t_ptr pos);

XDL_API link_t_ptr get_tool_group_item(link_t_ptr glk, const tchar_t* sz_name);

XDL_API int get_tool_group_item_count(link_t_ptr glk);

XDL_API void delete_tool_group_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*TOOLDOC_H*/