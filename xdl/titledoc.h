/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc title document

	@module	titledoc.h | interface file

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

#ifndef _TITLEDOC_H
#define _TITLEDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_title_name_ptr: get the title name.
*/
#define get_title_name_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_title_name: set the title name.
*/
#define set_title_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER title: string.
@GET get_title_title_ptr: get the title document title.
*/
#define get_title_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_title_title: set the title document title.
*/
#define set_title_title(ptr,token)						set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)
/*
@PROPER style: string.
@SET set_title_style: set the title style.
*/
#define set_title_style(ptr,lay)						set_dom_node_attr(ptr,ATTR_STYLE,-1,lay,-1)
/*
@PROPER style: string.
@GET get_title_style_ptr: get the title style.
*/
#define get_title_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER oritation: string.
@SET set_title_oritation: set the title oritation.
*/
#define set_title_oritation(ptr,lay)					set_dom_node_attr(ptr,ATTR_ORITATION,-1,lay,-1)
/*
@PROPER oritation: string.
@GET get_title_oritation_ptr: get the title oritation.
*/
#define get_title_oritation_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_ORITATION,-1)
/*
@PROPER iconSpan: numeric.
@SET set_title_icon_span: set the title icon span.
*/
#define set_title_icon_span(ptr,n)						set_dom_node_attr_float(ptr,ICON_SPAN,n)
/*
@PROPER iconSpan: numeric.
@GET get_title_icon_span: get the title icon span.
*/
#define get_title_icon_span(ptr)						get_dom_node_attr_float(ptr,ICON_SPAN)
/*
@PROPER itemHeight: numeric.
@SET set_title_item_height: set the title item height.
*/
#define set_title_item_height(ptr,n)					set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)
/*
@PROPER itemHeight: numeric.
@GET get_title_item_height: get the title item height.
*/
#define get_title_item_height(ptr)						get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)
/*
@PROPER itemWidth: numeric.
@SET set_title_item_width: set the title item width.
*/
#define set_title_item_width(ptr,n)						set_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH,n)
/*
@PROPER itemWidth: numeric.
@GET get_title_item_width: get the title item width.
*/
#define get_title_item_width(ptr)						get_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH)
/*
@PROPER name: string.
@SET set_title_item_name: set the title item name.
*/
#define set_title_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER name: string.
@GET get_title_item_name_ptr: get the title item name.
*/
#define get_title_item_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER title: string.
@SET set_title_item_title: set the item title.
*/
#define set_title_item_title(ilk,token)					set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)
/*
@PROPER title: string.
@GET get_title_item_title_ptr: get the item title.
*/
#define get_title_item_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)
/*
@PROPER id: string.
@SET set_title_item_id: set the item identifier.
*/
#define set_title_item_id(ilk,token)					set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)
/*
@PROPER id: string.
@GET get_title_item_id_ptr: get the item identifier.
*/
#define get_title_item_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)
/*
@PROPER icon: string.
@GET get_title_item_icon_ptr: get the item icon.
*/
#define get_title_item_icon_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ICON,-1)
/*
@PROPER icon: string.
@SET set_title_item_icon: set the item icon.
*/
#define set_title_item_icon(ilk,token)					set_dom_node_attr(ilk,ATTR_ICON,-1,token,-1)
/*
@PROPER icon: string.
@GET get_title_item_icon_ptr: get the item icon.
*/
#define get_title_item_icon_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ICON,-1)
/*
@PROPER icon: string.
@SET set_title_item_icon: set the item image.
*/
#define set_title_item_icon(ilk,token)					set_dom_node_attr(ilk,ATTR_ICON,-1,token,-1)
/*
@PROPER delta: var int.
@SET set_title_item_delta: set the item extract data.
*/
#define set_title_item_delta(ilk,ul)					set_dom_node_delta(ilk,(var_long)ul)
/*
@PROPER delta: var int.
@GET get_title_item_delta: get the item extract data.
*/
#define get_title_item_delta(ilk)						get_dom_node_delta(ilk)
/*
@PROPER locked: boolean.
@SET set_title_item_locked: set the item is locked.
*/
#define set_title_item_locked(ilk,b)					set_dom_node_mask_check(ilk,MSK_LOCKED,b) 
/*
@PROPER locked: boolean.
@GET get_title_item_locked: get the item is locked.
*/
#define get_title_item_locked(ilk)						get_dom_node_mask_check(ilk,MSK_LOCKED)

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_title_doc: create a title document.
@RETURN link_t_ptr: return the title document link component.
*/
XDL_API link_t_ptr create_title_doc(void);

/*
@FUNCTION destroy_title_doc: destroy a title document.
@INPUT link_t_ptr ptr: the title link component.
@RETURN void: none.
*/
XDL_API void destroy_title_doc(link_t_ptr ptr);

/*
@FUNCTION get_title_itemset: get the title item set.
@INPUT link_t_ptr ptr: the title link component.
@RETURN link_t_ptr: return the title item set link component.
*/
XDL_API link_t_ptr get_title_itemset(link_t_ptr ptr);

/*
@FUNCTION is_title_doc: test is title document.
@INPUT link_t_ptr ptr: the title link component.
@RETURN bool_t: return nonzero for being a title document, otherwise return zero.
*/
XDL_API bool_t is_title_doc(link_t_ptr ptr);

/*
@FUNCTION is_title_item: test is title item node.
@INPUT link_t_ptr ptr: the title link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a group node, otherwise return zero.
*/
XDL_API bool_t is_title_item(link_t_ptr ptr,link_t_ptr ilk);

/*
@FUNCTION clear_title_doc: clear the title document.
@INPUT link_t_ptr ptr: the title link component.
@RETURN void: none.
*/
XDL_API void clear_title_doc(link_t_ptr ptr);

/*
@FUNCTION insert_title_item: add a new item to title document.
@INPUT link_t_ptr ptr: the tool link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
XDL_API link_t_ptr insert_title_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_title_item: find the title item by name.
@INPUT link_t_ptr ptr: the title link component.
@INPUT const tchar_t* sz_name: the item name token.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_title_item(link_t_ptr ptr,const tchar_t* szName);

/*
@FUNCTION get_title_item_by_id: find the title item by id.
@INPUT link_t_ptr ptr: the title link component.
@INPUT const tchar_t* sz_id: the item id token.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_title_item_by_id(link_t_ptr ptr, const tchar_t* sz_id);

/*
@FUNCTION get_title_next_item: get the next item.
@INPUT link_t_ptr ptr: the title link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_title_next_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_title_prev_item: get the previous item.
@INPUT link_t_ptr ptr: the title link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_title_prev_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_title_item_count: counting the items in title document.
@INPUT link_t_ptr ptr: the title link component.
@RETURN int: return the number of items.
*/
XDL_API int get_title_item_count(link_t_ptr ptr);

/*
@FUNCTION delete_title_item: delete the title item.
@INPUT link_t_ptr ptr: the item link component.
@RETURN void: none.
*/
XDL_API void delete_title_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*TITLEDOC_H*/