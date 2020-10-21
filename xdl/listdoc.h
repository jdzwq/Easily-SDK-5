/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc list document

	@module	listdoc.h | interface file

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

#ifndef _LISTDOC_H
#define _LISTDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER style: string.
@GET get_list_style_ptr: get the list style.
*/
#define get_list_style_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER style: string.
@SET set_list_style: set the list style.
*/
#define set_list_style(ptr,lay)								set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)
/*
@PROPER name: string.
@GET get_list_name_ptr: get the list name.
*/
#define get_list_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_list_name: set the list name.
*/
#define set_list_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER layer: string.
@SET set_list_layer: set the list layer.
*/
#define set_list_layer(ptr,lay)								set_dom_node_attr(ptr,ATTR_LAYER,-1,lay,-1)
/*
@PROPER layer: string.
@GET get_list_layer_ptr: get the list layer.
*/
#define get_list_layer_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_LAYER,-1)
/*
@PROPER width: numeric.
@SET set_list_width: set the list width.
*/
#define set_list_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER width: numeric.
@GET get_list_width: get the list width.
*/
#define get_list_width(ptr)								get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER height: numeric.
@GET get_list_height: get the list height.
*/
#define get_list_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: numeric.
@SET set_list_height: set the list height.
*/
#define set_list_height(ptr,n)							set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER iconSpan: numeric.
@SET set_list_icon_span: set the list icon span.
*/
#define set_list_icon_span(ptr,n)							set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)
/*
@PROPER iconSpan: numeric.
@GET get_list_icon_span: get the list icon span.
*/
#define get_list_icon_span(ptr)								get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)
/*
@PROPER itemWidth: numeric.
@SET set_list_item_width: set the list item width.
*/
#define set_list_item_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH,n)
/*
@PROPER itemWidth: numeric.
@GET get_list_item_width: get the list item width.
*/
#define get_list_item_width(ptr)							get_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH)
/*
@PROPER itemHeight: numeric.
@GET get_list_item_height: get the list item height.
*/
#define get_list_item_height(ptr)							get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)
/*
@PROPER itemHeight: numeric.
@SET set_list_item_height: set the list item height.
*/
#define set_list_item_height(ptr,n)							set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)
/*
@PROPER showCheck: boolean.
@GET get_list_showcheck: get the list show check bar.
*/
#define get_list_showcheck(ptr)								get_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK)
/*
@PROPER showCheck: boolean.
@SET set_list_showcheck: set the list show check bar.
*/
#define set_list_showcheck(ptr,b)							set_dom_node_attr_boolean(ptr,ATTR_SHOWCHECK,b)
/*
@PROPER id: string.
@SET set_list_item_id: set the list item identifier
*/
#define set_list_item_id(ilk,token)							set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)
/*
@PROPER id: string.
@GET get_list_item_id_ptr: get the list item identifier.
*/
#define get_list_item_id_ptr(ilk)							get_dom_node_attr_ptr(ilk,ATTR_ID,-1)
/*
@PROPER name: string.
@GET get_list_item_name_ptr: get the list item name.
*/
#define get_list_item_name_ptr(ilk)							get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_list_item_name: set the list item name
*/
#define set_list_item_name(ilk,token)						set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER title: string.
@GET get_list_item_title_ptr: get the list item title.
*/
#define get_list_item_title_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_list_item_title: set the list item title
*/
#define set_list_item_title(ilk,token)						set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)
/*
@PROPER icon: string.
@GET get_list_item_icon_ptr: get the list item icon.
*/
#define get_list_item_icon_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ICON,-1)
/*
@PROPER icon: string.
@SET set_list_item_icon: set the list item icon
*/
#define set_list_item_icon(ilk,token)						set_dom_node_attr(ilk,ATTR_ICON,-1,token,-1)
/*
@PROPER locked: boolean.
@SET set_list_item_locked: set the list item is locked.
*/
#define set_list_item_locked(ilk,c)							set_dom_node_mask_check(ilk,MSK_LOCKED,c) 
/*
@PROPER locked: boolean.
@GET get_list_item_locked: get the list item is locked.
*/
#define get_list_item_locked(ilk)							get_dom_node_mask_check(ilk,MSK_LOCKED)
/*
@PROPER checked: boolean.
@SET set_list_item_checked: set the list item is checked.
*/
#define set_list_item_checked(ilk,b)						set_dom_node_mask_check(ilk,MSK_CHECKED,b)
/*
@PROPER checked: boolean.
@GET get_list_item_checked: get the list item is checked.
*/
#define get_list_item_checked(ilk)							get_dom_node_mask_check(ilk,MSK_CHECKED)

#define set_list_child_item_checked(ilk,b)					set_dom_child_node_mask_check(ilk,MSK_CHECKED,b)

#define get_list_item_checked_count(ilk)					get_dom_child_node_mask_check_count(ilk,MSK_CHECKED)


#define get_list_item_file_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_list_item_file_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_list_item_file_createtime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_CREATETIME,-1)

#define set_list_item_file_createtime(ilk,token)			set_dom_node_attr(ilk,ATTR_CREATETIME,-1,token,-1)

#define get_list_item_file_writetime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_WRITETIME,-1)

#define set_list_item_file_writetime(ilk,token)				set_dom_node_attr(ilk,ATTR_WRITETIME,-1,token,-1)

#define get_list_item_file_accesstime_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_ACCESSTIME,-1)

#define set_list_item_file_accesstime(ilk,token)			set_dom_node_attr(ilk,ATTR_ACCESSTIME,-1,token,-1)

#define get_list_item_file_etag_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ETAG,-1)

#define set_list_item_file_etag(ilk,token)					set_dom_node_attr(ilk,ATTR_ETAG,-1,token,-1)

#define get_list_item_file_size_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_SIZE,-1)

#define set_list_item_file_size(ilk,token)					set_dom_node_attr(ilk,ATTR_SIZE,-1,token,-1)

#define get_list_item_file_archive_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ARCHIVE,-1)

#define set_list_item_file_archive(ilk,token)				set_dom_node_attr(ilk,ATTR_ARCHIVE,-1,token,-1)

#ifdef	__cplusplus
extern "C" {
#endif


/************************************************Functions***************************************************************************/

/*
@FUNCTION create_list_doc: create a list document.
@RETURN link_t_ptr: return the list document link component.
*/
EXP_API link_t_ptr create_list_doc(void);

/*
@FUNCTION destroy_list_doc: destroy a list document.
@INPUT link_t_ptr ptr: the list link component.
@RETURN void: none.
*/
EXP_API void destroy_list_doc(link_t_ptr ptr);

/*
@FUNCTION clear_list_doc: clear the list document.
@INPUT link_t_ptr ptr: the list link component.
@RETURN void: none.
*/
EXP_API void clear_list_doc(link_t_ptr ptr);

/*
@FUNCTION get_list_item_level: get the item level.
@INPUT link_t_ptr ilk: the item link component.
@RETURN int: return the zero based level index.
*/
EXP_API int get_list_item_level(link_t_ptr ilk);

/*
@FUNCTION get_list_child_item_count: counting the list child items.
@INPUT link_t_ptr ptr: the parent item link component.
@RETURN int: return the number of items.
*/
EXP_API int get_list_child_item_count(link_t_ptr ilk);

/*
@FUNCTION is_list_doc: test is list document.
@INPUT link_t_ptr ptr: the list link component.
@RETURN bool_t: return nonzero for being a list document, otherwise return zero.
*/
EXP_API bool_t is_list_doc(link_t_ptr ptr);

/*
@FUNCTION is_list_item: test is list item node.
@INPUT link_t_ptr ptr: the list link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a list item, otherwise return zero.
*/
EXP_API bool_t is_list_item(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION list_doc_from_item: trackback to list document from child item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the list link component.
*/
EXP_API link_t_ptr list_doc_from_item(link_t_ptr ilk);

/*
@FUNCTION insert_list_item: add a new item to list document.
@INPUT link_t_ptr ilk: the parent item link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
EXP_API link_t_ptr insert_list_item(link_t_ptr ilk, link_t_ptr pos);

/*
@FUNCTION get_list_first_child_item: get the first child item.
@INPUT link_t_ptr ilk: the parent item link component.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_list_first_child_item(link_t_ptr ilk);

/*
@FUNCTION get_list_last_child_item: get the last child item.
@INPUT link_t_ptr ilk: the parent item link component.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_list_last_child_item(link_t_ptr ilk);

/*
@FUNCTION get_list_item_at: get the child item at position.
@INPUT link_t_ptr ilk: the parent item link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_list_item_at(link_t_ptr ilk, int index);

/*
@FUNCTION get_list_parent_item: get the parent item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the parent item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_list_parent_item(link_t_ptr ilk);

/*
@FUNCTION get_list_next_sibling_item: get the next sibling item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the next sibling item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_list_next_sibling_item(link_t_ptr ilk);

/*
@FUNCTION get_list_prev_sibling_item: get the previous sibling item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the previous sibling item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_list_prev_sibling_item(link_t_ptr ilk);

/*
@FUNCTION delete_list_item: delete the list item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
EXP_API void delete_list_item(link_t_ptr ilk);

/*
@FUNCTION insert_list_file_item: add a new file item to list document.
@INPUT link_t_ptr ilk: the parent item link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
EXP_API link_t_ptr insert_list_file_item(link_t_ptr ilk, link_t_ptr pos);

/*
@FUNCTION insert_list_directory_item: add a new directory item to list document.
@INPUT link_t_ptr ilk: the parent item link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
EXP_API link_t_ptr insert_list_directory_item(link_t_ptr ilk, link_t_ptr pos);

/*
@FUNCTION is_list_file_item: test is a file item.
@INPUT link_t_ptr ilk: the list link component.
@RETURN bool_t: return nonzero being a file item, otherwise return zero.
*/
EXP_API bool_t is_list_file_item(link_t_ptr ilk);

/*
@FUNCTION is_list_directory_item: test is a directory item.
@INPUT link_t_ptr ilk: the list link component.
@RETURN bool_t: return nonzero being a directory item, otherwise return zero.
*/
EXP_API bool_t is_list_directory_item(link_t_ptr ilk);

/*
@FUNCTION set_list_item_file_info: set file information to item.
@INPUT link_t_ptr ilk: the item link component.
@INPUT const file_info_t* pfi: the file information struct.
@RETURN void: none.
*/
EXP_API void set_list_item_file_info(link_t_ptr ilk, const file_info_t* pfi);

/*
@FUNCTION get_list_item_file_info: get file information from item.
@INPUT link_t_ptr ilk: the item link component.
@OUTPUT file_info_t* pfi: the file information struct.
@RETURN void: none.
*/
EXP_API void get_list_item_file_info(link_t_ptr ilk, file_info_t* pfi);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*LISTDOC_H*/