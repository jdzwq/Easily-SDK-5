/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc menu document

	@module	menudoc.h | interface file

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

#ifndef _MENUDOC_H
#define _MENUDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_menu_name_ptr: get the munu name.
*/
#define get_menu_name_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_menu_name: set the menu name.
*/
#define set_menu_name(ptr,token)							set_dom_node_attr(ptr,ATTR_NAME,-1,token,-1)
/*
@PROPER style: string.
@GET get_menu_style_ptr: get the menu style.
*/
#define get_menu_style_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_STYLE,-1)
/*
@PROPER style: string.
@SET set_menu_style: set the menu style.
*/
#define set_menu_style(ptr,token)							set_dom_node_attr(ptr,ATTR_STYLE,-1,token,-1)
/*
@PROPER show: string.
@GET get_menu_show_ptr: get the menu show type.
*/
#define get_menu_show_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_SHOW,-1)
/*
@PROPER show: string.
@SET set_menu_show: set the menu show type.
*/
#define set_menu_show(ptr,token)							set_dom_node_attr(ptr,ATTR_SHOW,-1,token,-1)
/*
@PROPER layer: string.
@SET set_menu_layer: set the menu layer.
*/
#define set_menu_layer(ptr,lay)								set_dom_node_attr(ptr,ATTR_LAYER,-1,lay,-1)
/*
@PROPER layer: string.
@GET get_menu_layer_ptr: get the menu layer.
*/
#define get_menu_layer_ptr(ptr)								get_dom_node_attr_ptr(ptr,ATTR_LAYER,-1)
/*
@PROPER iconSpan: numeric.
@SET set_menu_icon_span: set the menu icon span.
*/
#define set_menu_icon_span(ptr,n)							set_dom_node_attr_float(ptr,GDI_ATTR_GIZMO_SPAN,n)
/*
@PROPER iconSpan: numeric.
@GET get_menu_icon_span: get the menu icon span.
*/
#define get_menu_icon_span(ptr)								get_dom_node_attr_float(ptr,GDI_ATTR_GIZMO_SPAN)
/*
@PROPER name: string.
@GET get_menu_item_name_ptr: get the menu name.
*/
#define get_menu_item_name_ptr(ilk)							get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_menu_item_name: set the menu name.
*/
#define set_menu_item_name(ilk,token)						set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER iid: string.
@GET get_menu_item_iid: get the menu iid.
*/
#define get_menu_item_iid(ilk)								get_dom_node_attr_integer(ilk,ATTR_ID)
/*
@PROPER iid: string.
@SET set_menu_item_iid: set the menu iid.
*/
#define set_menu_item_iid(ilk,n)							set_dom_node_attr_integer(ilk,ATTR_ID,n)
/*
@PROPER title: string.
@SET set_menu_item_title: set the menu title.
*/
#define set_menu_item_title(ilk,token)						set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)
/*
@PROPER title: string.
@GET get_menu_item_title_ptr: get the menu title.
*/
#define get_menu_item_title_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)
/*
@PROPER icon: string.
@GET get_menu_item_icon_ptr: get the menu icon.
*/
#define get_menu_item_icon_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_ICON,-1)
/*
@PROPER icon: string.
@SET set_menu_item_icon: set the menu icon.
*/
#define set_menu_item_icon(ilk,token)						set_dom_node_attr(ilk,ATTR_ICON,-1,token,-1)
/*
@PROPER checked: boolean.
@SET set_menu_item_checked: set the menu item is checked.
*/
#define set_menu_item_checked(ilk,n)						set_dom_node_mask_check(ilk,MSK_CHECKED,n)
/*
@PROPER checked: boolean.
@GET get_menu_item_checked: get the menu item is checked.
*/
#define get_menu_item_checked(ilk)							get_dom_node_mask_check(ilk,MSK_CHECKED)


#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_menu_doc: create a menu document.
@RETURN link_t_ptr: return the menu document link component.
*/
EXP_API link_t_ptr create_menu_doc(void);

/*
@FUNCTION destroy_menu_doc: destroy a menu document.
@INPUT link_t_ptr ptr: the menu link component.
@RETURN void: none.
*/
EXP_API void destroy_menu_doc(link_t_ptr ptr);

/*
@FUNCTION get_menu_itemset: get menu item set.
@INPUT link_t_ptr ptr: the menu link component.
@RETURN link_t_ptr: the item set link component.
*/
EXP_API link_t_ptr get_menu_itemset(link_t_ptr ptr);

/*
@FUNCTION clear_menu_doc: clear the menu document.
@INPUT link_t_ptr ptr: the menu link component.
@RETURN void: none.
*/
EXP_API void clear_menu_doc(link_t_ptr ptr);

/*
@FUNCTION is_menu_doc: test is menu document.
@INPUT link_t_ptr ptr: the menu link component.
@RETURN bool_t: return nonzero for being a menu document, otherwise return zero.
*/
EXP_API bool_t is_menu_doc(link_t_ptr ptr);

/*
@FUNCTION is_menu_item: test is menu item node.
@INPUT link_t_ptr ptr: the menu link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a item, otherwise return zero.
*/
EXP_API bool_t is_menu_item(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION get_menu_item_count: counting the items in menu document.
@INPUT link_t_ptr ptr: the menu link component.
@RETURN int: return the number of items.
*/
EXP_API int get_menu_item_count(link_t_ptr ptr);

/*
@FUNCTION get_menu_next_item: get the next menu item.
@INPUT link_t_ptr ptr: the menu link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_menu_next_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_menu_prev_item: get the previous menu item.
@INPUT link_t_ptr ptr: the menu link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_menu_prev_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_menu_item: find the item by name.
@INPUT link_t_ptr ptr: the menu link component.
@INPUT const tchar_t* sz_name: the menu item name.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_menu_item(link_t_ptr ptr, const tchar_t* sz_name);

/*
@FUNCTION get_menu_item_by_iid: find the item by iid.
@INPUT link_t_ptr ptr: the menu link component.
@INPUT const tchar_t* sz_iid: the menu item iid.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_menu_item_by_iid(link_t_ptr ptr, int sz_iid);

/*
@FUNCTION insert_menu_item: add a new item to menu document.
@INPUT link_t_ptr ptr: the menu link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
EXP_API link_t_ptr insert_menu_item(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION delete_menu_item: delete the menu item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
EXP_API void delete_menu_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif	/*_MENUDOC_H*/