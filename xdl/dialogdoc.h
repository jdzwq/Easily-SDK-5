/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dialog document

	@module	dialogdoc.h | dialog document interface file

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

#ifndef _DIALOGDOC_H
#define _DIALOGDOC_H

#include "xdldef.h"
#include "docattr.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@SET set_dialog_name: set the dialog name.
*/
#define set_dialog_name(ptr,token)						set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER name: string.
@GET get_dialog_name_ptr: get the dialog name.
*/
#define get_dialog_name_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_dialog_id: set the dialog identifier.
*/
#define set_dialog_id(ptr,token)						set_dom_node_attr(ptr,ATTR_ID,-1,token,-1)
/*
@PROPER id: string.
@GET get_dialog_id_ptr: get the dialog identifier.
*/
#define get_dialog_id_ptr(ptr)							get_dom_node_attr_ptr(ptr,ATTR_ID,-1)
/*
@PROPER title: string.
@SET set_dialog_title: set the dialog title.
*/
#define set_dialog_title(ptr,token)						set_dom_node_attr(ptr,ATTR_TITLE,-1,token,-1)
/*
@PROPER title: string.
@GET get_dialog_title_ptr: get the dialog title.
*/
#define get_dialog_title_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_TITLE,-1)
/*
@PROPER style: string.
@SET set_dialog_style: set the dialog style.
*/
#define set_dialog_style(ptr,token)						set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)
/*
@PROPER style: string.
@GET get_dialog_style_ptr: get the dialog style.
*/
#define get_dialog_style_ptr(ptr)						get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER width: numeric.
@GET get_dialog_width: get the dialog width.
*/
#define get_dialog_width(ptr)							get_dom_node_attr_float(ptr,ATTR_WIDTH)
/*
@PROPER width: numeric.
@SET set_dialog_width: set the dialog width.
*/
#define set_dialog_width(ptr,n)							set_dom_node_attr_float(ptr,ATTR_WIDTH,n)
/*
@PROPER height: numeric.
@GET get_dialog_height: get the dialog height.
*/
#define get_dialog_height(ptr)							get_dom_node_attr_float(ptr,ATTR_HEIGHT)
/*
@PROPER height: numeric.
@SET set_dialog_height: set the dialog height.
*/
#define set_dialog_height(ptr,n)						set_dom_node_attr_float(ptr,ATTR_HEIGHT,n)
/*
@PROPER class: string.
@GET get_dialog_item_class_ptr: get the dialog item class.
*/
#define get_dialog_item_class_ptr(ilk)					get_dom_node_name_ptr(ilk)
/*
@PROPER name: string.
@SET set_dialog_item_name: set the dialog item name.
*/
#define set_dialog_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER name: string.
@GET get_dialog_item_name_ptr: get the dialog item name.
*/
#define get_dialog_item_name_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER id: string.
@SET set_dialog_item_id: set the dialog item id.
*/
#define set_dialog_item_id(ilk,token)					set_dom_node_attr(ilk,ATTR_ID,-1,token,-1)
/*
@PROPER id: string.
@GET get_dialog_item_id_ptr: get the dialog item id.
*/
#define get_dialog_item_id_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ID,-1)
/*
@PROPER text: string.
@SET set_dialog_item_text: set the dialog item text.
*/
#define set_dialog_item_text(ilk,token,len)				set_dom_node_text(ilk,token,len)
/*
@PROPER text: string.
@GET get_dialog_item_text_ptr: get the dialog item text.
*/
#define get_dialog_item_text_ptr(ilk)					get_dom_node_text_ptr(ilk)
/*
@PROPER style: string.
@SET set_dialog_item_style: set the dialog item style.
*/
#define set_dialog_item_style(ilk,token)				set_dom_node_attr(ilk,ATTR_STYLE,-1,token,-1)
/*
@PROPER style: string.
@GET get_dialog_item_style_ptr: get the dialog item style.
*/
#define get_dialog_item_style_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_STYLE,-1)
/*
@PROPER x: numeric.
@GET get_dialog_item_x: get the dialog item x point.
*/
#define get_dialog_item_x(ilk)							get_dom_node_attr_float(ilk,ATTR_X)
/*
@PROPER x: numeric.
@SET set_dialog_item_x: set the dialog item x point.
*/
#define set_dialog_item_x(ilk,xm)						set_dom_node_attr_float(ilk,ATTR_X,xm)
/*
@PROPER y: numeric.
@GET get_dialog_item_y: get the dialog item y point.
*/
#define get_dialog_item_y(ilk)							get_dom_node_attr_float(ilk,ATTR_Y)
/*
@PROPER y: numeric.
@SET set_dialog_item_y: set the dialog item y point.
*/
#define set_dialog_item_y(ilk,ym)						set_dom_node_attr_float(ilk,ATTR_Y,ym)
/*
@PROPER width: numeric.
@SET set_dialog_item_width: set the dialog item width.
*/
#define set_dialog_item_width(ilk,n)					set_dom_node_attr_float(ilk,ATTR_ITEM_WIDTH,n)
/*
@PROPER width: numeric.
@GET get_dialog_item_width: get the dialog item width.
*/
#define get_dialog_item_width(ilk)						get_dom_node_attr_float(ilk,ATTR_ITEM_WIDTH)
/*
@PROPER height: numeric.
@SET set_dialog_item_height: set the dialog item height.
*/
#define set_dialog_item_height(ilk,n)					set_dom_node_attr_float(ilk,ATTR_ITEM_HEIGHT,n)
/*
@PROPER height: numeric.
@GET get_dialog_item_height: get the dialog item height.
*/
#define get_dialog_item_height(ilk)						get_dom_node_attr_float(ilk,ATTR_ITEM_HEIGHT)
/*
@PROPER tabOrder: integer.
@GET get_dialog_item_taborder: get the dialog item tab order.
*/
#define get_dialog_item_taborder(ilk)					get_dom_node_attr_integer(ilk,ATTR_TABORDER)
/*
@PROPER tabOrder: integer.
@SET set_dialog_item_taborder: set the dialog item tab order.
*/
#define set_dialog_item_taborder(ilk,n)					set_dom_node_attr_integer(ilk,ATTR_TABORDER,n)

/*
@PROPER delta: var long.
@SET set_dialog_item_delta: set the dialog item extract data.
*/
#define set_dialog_item_delta(ilk,ul)					set_dom_node_delta(ilk,(var_long)ul)
/*
@PROPER delta: var long.
@GET get_dialog_item_delta: get the dialog item extract data.
*/
#define get_dialog_item_delta(ilk)						get_dom_node_delta(ilk)
/*
@PROPER selected: boolean.
@SET set_dialog_item_selected: set the dialog item is selected.
*/
#define set_dialog_item_selected(flk,b)					set_dom_node_mask_check(flk,MSK_CHECKED,b)
/*
@PROPER selected: boolean.
@GET get_dialog_item_selected: get the dialog item is selected.
*/
#define get_dialog_item_selected(flk)					get_dom_node_mask_check(flk,MSK_CHECKED) 


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_dialog_doc: create a dialog document.
@RETURN link_t_ptr: return the dialog document link component.
*/
XDL_API link_t_ptr create_dialog_doc(void);

/*
@FUNCTION destroy_dialog_doc: destroy a dialog document.
@INPUT link_t_ptr ptr: the dialog link component.
@RETURN void: none.
*/
XDL_API void destroy_dialog_doc(link_t_ptr ptr);

/*
@FUNCTION clear_dialog_doc: clear the dialog document.
@INPUT link_t_ptr ptr: the dialog link component.
@RETURN void: none.
*/
XDL_API void clear_dialog_doc(link_t_ptr ptr);

/*
@FUNCTION is_dialog_doc: test is dialog document.
@INPUT link_t_ptr ptr: the dialog link component.
@RETURN bool_t: return nonzero for being a dialog document, otherwise return zero.
*/
XDL_API bool_t is_dialog_doc(link_t_ptr ptr);

/*
@FUNCTION is_dialog_item: test is dialog item node.
@INPUT link_t_ptr ptr: the dialog link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a item, otherwise return zero.
*/
XDL_API bool_t is_dialog_item(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION reset_dialog_taborder: reset the dialog item tab order.
@INPUT link_t_ptr ptr: the dialog link component.
@RETURN void: none.
*/
XDL_API void reset_dialog_taborder(link_t_ptr ptr);

/*
@FUNCTION get_dialog_itemset: get dialog item set.
@INPUT link_t_ptr ptr: the dialog link component.
@RETURN link_t_ptr: the item set link component.
*/
XDL_API link_t_ptr get_dialog_itemset(link_t_ptr ptr);

/*
@FUNCTION insert_dialog_item: add a new item to dialog document.
@INPUT link_t_ptr ptr: the dialog link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
XDL_API link_t_ptr insert_dialog_item(link_t_ptr ptr,const tchar_t* sz_class);

/*
@FUNCTION get_dialog_next_item: get the next dialog item.
@INPUT link_t_ptr ptr: the dialog link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dialog_next_item(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_dialog_prev_item: get the previous dialog item.
@INPUT link_t_ptr ptr: the dialog link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dialog_prev_item(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_dialog_item: find the item by name.
@INPUT link_t_ptr ptr: the dialog link component.
@INPUT const tchar_t* sz_name: the dialog item name.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_dialog_item(link_t_ptr ptr, const tchar_t* sz_name);

/*
@FUNCTION get_dialog_item_count: counting the items in dialog document.
@INPUT link_t_ptr ptr: the dialog link component.
@RETURN int: return the number of items.
*/
XDL_API int get_dialog_item_count(link_t_ptr ptr);

/*
@FUNCTION get_dialog_item_count_by_class: counting the items according to class.
@INPUT link_t_ptr ptr: the dialog link component.
@INPUT const tchar_t* sz_class: the class token.
@RETURN int: return the number of items.
*/
XDL_API int get_dialog_item_count_by_class(link_t_ptr ptr, const tchar_t* sz_class);

/*
@FUNCTION get_dialog_item_selected_count: counting the selected items in dialog document.
@INPUT link_t_ptr ptr: the dialog link component.
@RETURN int: return the number of items.
*/
XDL_API int get_dialog_item_selected_count(link_t_ptr ptr);

/*
@FUNCTION delete_dialog_item: delete the dialog item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDL_API void delete_dialog_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //DIALOGDOC_H