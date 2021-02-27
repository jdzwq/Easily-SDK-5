/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc status document

	@module	statusdoc.h | interface file

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

#ifndef _STATUSDOC_H
#define _STATUSDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_status_name_ptr: get the status name.
*/
#define get_status_name_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_status_name: set the status name.
*/
#define set_status_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER title: string.
@SET set_status_title: set the status title.
*/
#define set_status_title(ptr,token)						set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)
/*
@PROPER title: string.
@GET get_status_title_ptr: get the status title.
*/
#define get_status_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)
/*
@PROPER style: string.
@SET set_status_style: set the status style.
*/
#define set_status_style(ptr,lay)						set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)
/*
@PROPER style: string.
@GET get_status_style_ptr: get the status style.
*/
#define get_status_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER width: numeric.
@SET set_status_width: set the status width.
*/
#define set_status_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER width: numeric.
@GET get_status_width: get the status width.
*/
#define get_status_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER height: numeric.
@GET get_status_height: get the status height.
*/
#define get_status_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: numeric.
@SET set_status_height: set the status height.
*/
#define set_status_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER iconSpan: numeric.
@SET set_status_icon_span: set the status icon span.
*/
#define set_status_icon_span(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ICON_SPAN,n)
/*
@PROPER iconSpan: numeric.
@GET get_status_icon_span: get the status icon span.
*/
#define get_status_icon_span(ptr)						get_dom_node_attr_float(ptr,ATTR_ICON_SPAN)
/*
@PROPER alignment: string.
@GET get_status_alignment_ptr: get the status alignment.
*/
#define get_status_alignment_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_ALIGNMENT,-1)
/*
@PROPER alignment: string.
@SET set_status_alignment: set the status alignment.
*/
#define set_status_alignment(ptr,token)					set_dom_node_attr(ptr,ATTR_ALIGNMENT,-1,token,-1)
/*
@PROPER delta: var int.
@SET set_status_item_delta: set the status item extract data.
*/
#define set_status_item_delta(ilk,ul)					set_dom_node_delta(ilk,(vword_t)ul)
/*
@PROPER alignment: var int.
@GET get_status_item_delta: get the status item extract data.
*/
#define get_status_item_delta(ilk)						get_dom_node_delta(ilk)
/*
@PROPER name: string.
@SET set_status_item_name: set the status item name.
*/
#define set_status_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER name: string.
@GET get_status_item_name_ptr: get the status item name.
*/
#define get_status_item_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER title: string.
@SET set_status_item_title: set the status item title.
*/
#define set_status_item_title(ilk,token)				set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)
/*
@PROPER title: string.
@GET get_status_item_title_ptr: get the status item title.
*/
#define get_status_item_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)
/*
@PROPER id: string.
@SET set_status_item_id: set the status item identifier.
*/
#define set_status_item_id(ilk,token)					set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)
/*
@PROPER id: string.
@GET get_status_item_id_ptr: get the status item identifier.
*/
#define get_status_item_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)
/*
@PROPER icon: string.
@GET get_status_item_icon_ptr: get the status item icon.
*/
#define get_status_item_icon_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ICON,-1)
/*
@PROPER icon: string.
@SET set_status_item_icon: set the status item icon.
*/
#define set_status_item_icon(ilk,token)				set_dom_node_attr(ilk,ATTR_ICON,-1,token,-1)
/*
@PROPER imagePosition: string.
@GET get_status_item_image_position_ptr: get the status item image position.
*/
#define get_status_item_image_position_ptr(ilk)			get_dom_node_attr_ptr(ilk,ATTR_IMAGE_POSITION,-1)
/*
@PROPER imagePosition: string.
@SET set_status_item_image_position: set the status item image position.
*/
#define set_status_item_image_position(ilk,token)		set_dom_node_attr(ilk,ATTR_IMAGE_POSITION,-1,token,-1)
/*
@PROPER width: numeric.
@SET set_status_item_width: set the status item width.
*/
#define set_status_item_width(ilk,n)					set_dom_node_attr_float(ilk,ATTR_ITEM_WIDTH,n)
/*
@PROPER width: numeric.
@GET get_status_item_width: get the status item width.
*/
#define get_status_item_width(ilk)						get_dom_node_attr_float(ilk,ATTR_ITEM_WIDTH)

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_status_doc: create a status document.
@RETURN link_t_ptr: return the status document link component.
*/
EXP_API link_t_ptr create_status_doc(void);

/*
@FUNCTION destroy_status_doc: destroy a status document.
@INPUT link_t_ptr ptr: the status link component.
@RETURN void: none.
*/
EXP_API void destroy_status_doc(link_t_ptr ptr);

/*
@FUNCTION get_status_itemset: get the status item set.
@INPUT link_t_ptr ptr: the status link component.
@RETURN link_t_ptr: return the status item set link component.
*/
EXP_API link_t_ptr get_status_itemset(link_t_ptr ptr);

/*
@FUNCTION is_status_doc: test is status document.
@INPUT link_t_ptr ptr: the status link component.
@RETURN bool_t: return nonzero for being a status document, otherwise return zero.
*/
EXP_API bool_t is_status_doc(link_t_ptr ptr);

/*
@FUNCTION is_status_item: test is status item node.
@INPUT link_t_ptr ptr: the status link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a group node, otherwise return zero.
*/
EXP_API bool_t is_status_item(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION clear_status_doc: clear the status document.
@INPUT link_t_ptr ptr: the status link component.
@RETURN void: none.
*/
EXP_API void clear_status_doc(link_t_ptr ptr);

/*
@FUNCTION insert_status_item: add a new item to status document.
@INPUT link_t_ptr ptr: the status link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
EXP_API link_t_ptr insert_status_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_status_next_item: get the next item.
@INPUT link_t_ptr ptr: the status link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_status_next_item(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_status_prev_item: get the previous item.
@INPUT link_t_ptr ptr: the status link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_status_prev_item(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_status_item: find the status item by name.
@INPUT link_t_ptr ptr: the status link component.
@INPUT const tchar_t* sz_name: the item name token.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_status_item(link_t_ptr ptr, const tchar_t* szName);

/*
@FUNCTION get_status_item_count: counting the items in status document.
@INPUT link_t_ptr ptr: the status link component.
@RETURN int: return the number of items.
*/
EXP_API int get_status_item_count(link_t_ptr ptr);

/*
@FUNCTION delete_status_item: delete the status item.
@INPUT link_t_ptr ptr: the item link component.
@RETURN void: none.
*/
EXP_API void delete_status_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*STATUSDOC_H*/