/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc imagesdoc document

	@module	imagesdoc.h | interface file

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

#ifndef _IMAGESDOC_H
#define _IMAGESDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER layer: string.
@SET set_images_layer: set the images layer.
*/
#define set_images_layer(ptr,lay)					set_dom_node_attr(ptr,ATTR_LAYER,-1,lay,-1)
/*
@PROPER layer: string.
@GET get_images_layer_ptr: get the images layer.
*/
#define get_images_layer_ptr(ptr)					get_dom_node_attr_ptr(ptr,ATTR_LAYER,-1)
/*
@PROPER width: numeric.
@SET set_images_item_width: set the images width.
*/
#define set_images_item_width(ptr,n)				set_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH,n)
/*
@PROPER width: numeric.
@GET get_images_item_width: get the images width.
*/
#define get_images_item_width(ptr)					get_dom_node_attr_float(ptr,ATTR_ITEM_WIDTH)
/*
@PROPER height: numeric.
@GET get_images_item_height: get the images height.
*/
#define get_images_item_height(ptr)					get_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT)
/*
@PROPER height: numeric.
@SET set_images_item_height: set the images height.
*/
#define set_images_item_height(ptr,n)				set_dom_node_attr_float(ptr,ATTR_ITEM_HEIGHT,n)
/*
@PROPER iconSpan: numeric.
@SET set_images_icon_span: set the images icon span.
*/
#define set_images_icon_span(ptr,n)					set_dom_node_attr_float(ptr,ICON_SPAN,n)
/*
@PROPER iconSpan: numeric.
@GET get_images_icon_span: get the images icon span.
*/
#define get_images_icon_span(ptr)					get_dom_node_attr_float(ptr,ICON_SPAN)
/*
@PROPER src: string.
@SET set_images_item_src: set the images item source.
*/
#define set_images_item_src(ilk,token)				set_dom_node_attr(ilk,ATTR_SRC,-1,token,-1)
/*
@PROPER src: string.
@GET get_images_item_src_ptr: get the images item source.
*/
#define get_images_item_src_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_SRC,-1)
/*
@PROPER alt: string.
@SET set_images_item_alt: set the images item alter text.
*/
#define set_images_item_alt(ilk,token)				set_dom_node_attr(ilk,ATTR_ALT,-1,token,-1)
/*
@PROPER alt: string.
@GET get_images_item_alt_ptr: get the images item alter text.
*/
#define get_images_item_alt_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_ALT,-1)
/*
@PROPER id: string.
@SET set_images_item_id: set the images item identifier.
*/
#define set_images_item_id(ilk,token)				set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)
/*
@PROPER id: string.
@GET get_images_item_id_ptr: get the images item identifier.
*/
#define get_images_item_id_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_ID,-1)

/*
@PROPER checked: boolean.
@SET set_images_item_checked: set the images item checked.
*/
#define set_images_item_checked(ilk,b)				set_dom_node_mask_check(ilk,MSK_CHECKED,b)
/*
@PROPER checked: boolean.
@GET get_images_item_checked: get the images item checked.
*/
#define get_images_item_checked(ilk)				get_dom_node_mask_check(ilk,MSK_CHECKED)


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_images_doc: create a images document.
@RETURN link_t_ptr: return the images document link component.
*/
XDL_API link_t_ptr create_images_doc(void);

/*
@FUNCTION destroy_images_doc: destroy a images document.
@INPUT link_t_ptr ptr: the images link component.
@RETURN void: none.
*/
XDL_API void destroy_images_doc(link_t_ptr ptr);

/*
@FUNCTION get_images_itemset: get images item set.
@INPUT link_t_ptr ptr: the imaes link component.
@RETURN link_t_ptr: the item set link component.
*/
XDL_API link_t_ptr get_images_itemset(link_t_ptr ptr);

/*
@FUNCTION clear_images_doc: clear the images document.
@INPUT link_t_ptr ptr: the images link component.
@RETURN void: none.
*/
XDL_API void clear_images_doc(link_t_ptr ptr);

/*
@FUNCTION is_images_doc: test is images document.
@INPUT link_t_ptr ptr: the images link component.
@RETURN bool_t: return nonzero for being a images document, otherwise return zero.
*/
XDL_API bool_t is_images_doc(link_t_ptr ptr);

/*
@FUNCTION is_images_item: test is images item node.
@INPUT link_t_ptr ptr: the images link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a item, otherwise return zero.
*/
XDL_API bool_t is_images_item(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION sort_images_doc: sorting images items by name order.
@INPUT link_t_ptr ptr: the images link component.
@INPUT bool_t desc: nonzero for descend sorting, zero for abscend sorting.
@RETURN void: none.
*/
XDL_API void sort_images_doc(link_t_ptr ptr, bool_t desc);

/*
@FUNCTION get_images_item_count: counting the items in images document.
@INPUT link_t_ptr ptr: the images link component.
@RETURN int: return the number of items.
*/
XDL_API int get_images_item_count(link_t_ptr ptr);

/*
@FUNCTION insert_images_item: add a new item to images document.
@INPUT link_t_ptr ptr: the images link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
XDL_API link_t_ptr insert_images_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_images_item: delete the images item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDL_API void delete_images_item(link_t_ptr ilk);

/*
@FUNCTION get_images_next_item: get the next images item.
@INPUT link_t_ptr ptr: the images link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_images_next_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_images_prev_item: get the previous images item.
@INPUT link_t_ptr ptr: the images link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_images_prev_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_images_item: get the images item by name.
@INPUT link_t_ptr ptr: the images link component.
@INPUT const tchar_t* alt: the item name token.
@INPUT int altlen: the key token length in characters.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_images_item(link_t_ptr ptr, const tchar_t* alt, int altlen);

/*
@FUNCTION get_ximage: get the ximage content by key.
@INPUT link_t_ptr ptr: the images link component.
@INTPUT const tchar_t* alt: the key string token.
@OUTPUT ximage_t* the ximage struct buffer.
@RETURN bool_t: return nonzero if item exists, otherwise return zero.
*/
XDL_API bool_t get_ximage(link_t_ptr ptr, const tchar_t* alt, ximage_t* pxi);

#ifdef	__cplusplus
}
#endif

#endif //XDL_SUPPORT_DOC

#endif /*_IMAGELIST_H*/
