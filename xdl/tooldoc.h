/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tool document

	@module	tooldoc.h | interface file

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

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_tool_name_ptr: get the tool name.
*/
#define get_tool_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_tool_name: set the tool name.
*/
#define set_tool_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER style: string.
@SET set_tool_style: set the tool style.
*/
#define set_tool_style(ptr,lay)							set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)
/*
@PROPER style: string.
@GET get_tool_style_ptr: get the tool style.
*/
#define get_tool_style_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)

/*
@PROPER iconSpan: numeric.
@SET set_tool_icon_span: set the tool icon span.
*/
#define set_tool_icon_span(ptr,n)						set_dom_node_attr_float(ptr,GDI_ATTR_GIZMO_SPAN,n)
/*
@PROPER iconSpan: numeric.
@GET get_tool_icon_span: get the tool icon span.
*/
#define get_tool_icon_span(ptr)							get_dom_node_attr_float(ptr,GDI_ATTR_GIZMO_SPAN)
/*
@PROPER titleHeight: numeric.
@GET get_tool_title_height: get the tool title height.
*/
#define get_tool_title_height(ptr)						get_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT)
/*
@PROPER titleHeight: numeric.
@SET set_tool_title_height: set the tool title height.
*/
#define set_tool_title_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_TITLE_HEIGHT,n)
/*
@PROPER collapsed: boolean.
@SET set_tool_group_collapsed: set the tool group collapsed or expanded.
*/
#define set_tool_group_collapsed(glk,b)					set_dom_node_mask_check(glk,MSK_COLLAPSED,b)
/*
@PROPER collapsed: document.
@GET get_tool_group_collapsed: get the tool group collapsed or expanded.
*/
#define get_tool_group_collapsed(glk)					get_dom_node_mask_check(glk,MSK_COLLAPSED)
/*
@PROPER groupName: string.
@GET get_tool_group_name_ptr: get the tool group name.
*/
#define get_tool_group_name_ptr(glk)					get_dom_node_attr_ptr(glk,ATTR_NAME,-1)
/*
@PROPER groupName: string.
@SET set_tool_group_name: set the tool group name.
*/
#define set_tool_group_name(glk,token)					set_dom_node_attr(glk,ATTR_NAME,-1,token,-1)
/*
@PROPER groupTitle: string.
@GET get_tool_group_title_ptr: get the tool group title.
*/
#define get_tool_group_title_ptr(glk)					get_dom_node_attr_ptr(glk,ATTR_TITLE,-1)
/*
@PROPER groupTitle: string.
@SET set_tool_group_title: set the tool group title.
*/
#define set_tool_group_title(glk,token)					set_dom_node_attr(glk,ATTR_TITLE,-1,token,-1)
/*
@PROPER groupShow: string.
@SET set_tool_group_show: set the tool group show style.
*/
#define set_tool_group_show(glk,token)					set_dom_node_attr(glk,ATTR_SHOW,-1,token,-1)
/*
@PROPER groupShow: string.
@GET get_tool_group_show_ptr: get the tool group show style.
*/
#define get_tool_group_show_ptr(glk)					get_dom_node_attr_ptr(glk,ATTR_SHOW,-1)
/*
@PROPER groupItemWidth: numeric.
@SET set_tool_group_item_width: set the tool group item width.
*/
#define set_tool_group_item_width(glk,n)				set_dom_node_attr_float(glk,ATTR_ITEM_WIDTH,n)
/*
@PROPER groupItemWidth: numeric.
@GET get_tool_group_item_width: get the tool group item width.
*/
#define get_tool_group_item_width(glk)					get_dom_node_attr_float(glk,ATTR_ITEM_WIDTH)
/*
@PROPER groupItemHeight: numeric.
@GET get_tool_group_item_height: get the tool group item height.
*/
#define get_tool_group_item_height(glk)					get_dom_node_attr_float(glk,ATTR_ITEM_HEIGHT)
/*
@PROPER groupItemHeight: numeric.
@SET set_tool_group_item_height: set the tool group item height.
*/
#define set_tool_group_item_height(glk,n)				set_dom_node_attr_float(glk,ATTR_ITEM_HEIGHT,n)
/*
@PROPER name: string.
@SET set_tool_item_name: set the tool item name.
*/
#define set_tool_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER name: string.
@GET get_tool_item_name_ptr: get the tool item item.
*/
#define get_tool_item_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_tool_item_id: set the tool item identifier.
*/
#define set_tool_item_id(ilk,token)						set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)
/*
@PROPER id: string.
@GET get_tool_item_id_ptr: get the tool item identifier.
*/
#define get_tool_item_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)
/*
@PROPER title: string.
@SET set_tool_item_title: set the tool item title.
*/
#define set_tool_item_title(ilk,token)					set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)
/*
@PROPER title: string.
@GET get_tool_item_title_ptr: get the tool item title.
*/
#define get_tool_item_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)
/*
@PROPER icon: string.
@GET get_tool_item_icon_ptr: get the tool item icon.
*/
#define get_tool_item_icon_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ICON,-1)
/*
@PROPER icon: string.
@SET set_tool_item_icon: set the tool item icon.
*/
#define set_tool_item_icon(ilk,token)					set_dom_node_attr(ilk,ATTR_ICON,-1,token,-1)
/*
@PROPER delta: var int.
@SET set_tool_item_delta: set the tool item extract data.
*/
#define set_tool_item_delta(ilk,ul)						set_dom_node_delta(ilk,(var_long)ul)
/*
@PROPER delta: var int.
@GET get_tool_item_delta: get the tool item extract data.
*/
#define get_tool_item_delta(ilk)						get_dom_node_delta(ilk)

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_tool_doc: create a tool document.
@RETURN link_t_ptr: return the tool document link component.
*/
EXP_API link_t_ptr create_tool_doc(void);

/*
@FUNCTION destroy_tool_doc: destroy a tool document.
@INPUT link_t_ptr ptr: the tool link component.
@RETURN void: none.
*/
EXP_API void destroy_tool_doc(link_t_ptr ptr);

/*
@FUNCTION is_tool_doc: test is tool document.
@INPUT link_t_ptr ptr: the tool link component.
@RETURN bool_t: return nonzero for being a tool document, otherwise return zero.
*/
EXP_API bool_t is_tool_doc(link_t_ptr ptr);

/*
@FUNCTION is_tool_group_item: test is tool group node.
@INPUT link_t_ptr ptr: the tool link component.
@INPUT link_t_ptr ilk: the group link component.
@RETURN bool_t: return nonzero for being a group node, otherwise return zero.
*/
EXP_API bool_t is_tool_group_item(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION clear_tool_doc: clear the tool document.
@INPUT link_t_ptr ptr: the tool link component.
@RETURN void: none.
*/
EXP_API void clear_tool_doc(link_t_ptr ptr);

/*
@FUNCTION merge_tool_doc: merge source tool child items into destination tool document.
@INPUT link_t_ptr ptr_dst: the tool link component.
@RETURN void: none.
*/
EXP_API void merge_tool_doc(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION insert_tool_group: add a new group to tool document.
@INPUT link_t_ptr ptr: the tool link component.
@INPUT link_t_ptr pos: the group link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new group link component.
*/
EXP_API link_t_ptr insert_tool_group(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_tool_group: find the group by name.
@INPUT link_t_ptr ptr: the tool link component.
@INPUT const tchar_t* sz_name: the group name token.
@RETURN link_t_ptr: return the group link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_tool_group(link_t_ptr ptr, const tchar_t* sz_name);

/*
@FUNCTION get_tool_next_group: get the next group.
@INPUT link_t_ptr ptr: the tool link component.
@INPUT link_t_ptr pos: the group link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the group link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_tool_next_group(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_tool_prev_group: get the previous group.
@INPUT link_t_ptr ptr: the tool link component.
@INPUT link_t_ptr pos: the group link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the group link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_tool_prev_group(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_tool_group_count: counting the groups in annotation document.
@INPUT link_t_ptr ptr: the tool link component.
@RETURN int: return the number of groups.
*/
EXP_API int get_tool_group_count(link_t_ptr ptr);

/*
@FUNCTION delete_tool_group: delete the tool group.
@INPUT link_t_ptr glk: the group link component.
@RETURN void: none.
*/
EXP_API void delete_tool_group(link_t_ptr glk);

/*
@FUNCTION insert_tool_group_item: add a new item to tool group.
@INPUT link_t_ptr glk: the group link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
EXP_API link_t_ptr insert_tool_group_item(link_t_ptr glk, link_t_ptr pos);

/*
@FUNCTION get_tool_group_next_item: get the next item.
@INPUT link_t_ptr glk: the group link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_tool_group_next_item(link_t_ptr glk,link_t_ptr pos);

/*
@FUNCTION get_tool_group_prev_item: get the previous item.
@INPUT link_t_ptr glk: the group link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_tool_group_prev_item(link_t_ptr glk,link_t_ptr pos);

/*
@FUNCTION get_tool_group_item: find the item by name.
@INPUT link_t_ptr glk: the group link component.
@INPUT const tchar_t* sz_name: the item name token.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_tool_group_item(link_t_ptr glk, const tchar_t* sz_name);

/*
@FUNCTION get_tool_group_item_count: counting the items in group.
@INPUT link_t_ptr glk: the group link component.
@RETURN int: return the number of items.
*/
EXP_API int get_tool_group_item_count(link_t_ptr glk);

/*
@FUNCTION delete_tool_group_item: delete the group item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
EXP_API void delete_tool_group_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*TOOLDOC_H*/