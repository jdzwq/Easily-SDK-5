/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc json document set

	@module	jsondoc.h | interface file

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

#ifndef _JSONDOC_H
#define _JSONDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/*******************************************Properties***************************************************************/

/*
@PROPER name: string.
@GET get_json_item_name_ptr: get the josn item name.
*/
#define get_json_item_name_ptr(ilk)						get_dom_node_name_ptr(ilk)
/*
@PROPER name: string.
@SET set_json_item_name: set the josn item name.
*/
#define set_json_item_name(ilk,token)					set_dom_node_name(ilk,token,-1)
/*
@PROPER value: string.
@GET get_json_item_value_ptr: get the josn text value.
*/
#define get_json_item_value_ptr(ilk)					get_dom_node_text_ptr(ilk)
/*
@PROPER value: string.
@GET get_json_item_value: get the josn text value.
*/
#define get_json_item_value(ilk,token,max)				get_dom_node_text(ilk,token,max)
/*
@PROPER value: string.
@SET set_json_item_value: set the josn item value.
*/
#define set_json_item_value(ilk,token)					set_dom_node_text(ilk,token,-1)
/*
@PROPER array: boolean.
@SET set_json_item_array: make the josn item is array.
*/
#define set_json_item_array(ilk,b)						set_dom_node_mask_check(ilk,MSK_ARRAY,b)
/*
@PROPER array: boolean.
@SET get_json_item_array: test the josn item is array.
*/
#define get_json_item_array(ilk)						get_dom_node_mask_check(ilk,MSK_ARRAY)

#ifdef	__cplusplus
extern "C" {
#endif
	
/*******************************************Functions****************************************************************/

/*
@FUNCTION create_json_doc: create a json document.
@RETURN link_t_ptr: return the json document link component.
*/
EXP_API link_t_ptr create_json_doc(void);

/*
@FUNCTION destroy_json_doc: destroy a json document.
@INPUT link_t_ptr ptr: the json document link component.
@RETURN void: none.
*/
EXP_API void destroy_json_doc(link_t_ptr ptr);

/*
@FUNCTION clear_json_doc: clear a json document, all of child nodes in dom body will be emptied.
@INPUT link_t_ptr ptr: the json document link component.
@RETURN void: none.
*/
EXP_API void clear_json_doc(link_t_ptr ptr);

/*
@FUNCTION is_json_doc: test is json document.
@INPUT link_t_ptr ptr: the json document link component.
@RETURN bool_t: return nonzero for json document.
*/
EXP_API bool_t is_json_doc(link_t_ptr ptr);

/*
@FUNCTION is_json_item: test is json child item.
@INPUT link_t_ptr ptr: the json document link component.
@INPUT link_t_ptr ilk: the json item link component.
@RETURN bool_t: return nonzero for json child item.
*/
EXP_API bool_t is_json_item(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION get_json_item: find json child item by name.
@INPUT link_t_ptr ptr: the json document link component.
@INPUT const tchar_t* iname: the name string token.
@RETURN link_t_ptr: return json item link component.
*/
EXP_API link_t_ptr get_json_item(link_t_ptr ptr, const tchar_t* iname);

/*
@FUNCTION insert_json_item: insert new json child item at the position.
@INPUT link_t_ptr ptr: the json document link component.
@INPUT link_t_ptr pos: the json item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new json item link component.
*/
EXP_API link_t_ptr insert_json_item(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_json_item: delete json child item.
@INPUT link_t_ptr ilk: the json iem link component.
@RETURN void: none.
*/
EXP_API void delete_json_item(link_t_ptr ilk);

/*
@FUNCTION get_json_item_count: counting the json child items.
@INPUT link_t_ptr ilk: the json iem link component.
@RETURN int: return the number of child items.
*/
EXP_API int get_json_item_count(link_t_ptr ilk);

/*
@FUNCTION get_json_first_child_item: get json first child item.
@INPUT link_t_ptr ilk: the parent json item link component.
@RETURN link_t_ptr: return item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_json_first_child_item(link_t_ptr ilk);

/*
@FUNCTION get_json_last_child_item: get json last child item.
@INPUT link_t_ptr ilk: the parent json item link component.
@RETURN link_t_ptr: return item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_json_last_child_item(link_t_ptr ilk);

/*
@FUNCTION get_json_parent_item: get json parent item.
@INPUT link_t_ptr ilk: the json item link component.
@RETURN link_t_ptr: return parent item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_json_parent_item(link_t_ptr ilk);

/*
@FUNCTION get_json_next_sibling_item: get json next sibling item.
@INPUT link_t_ptr ilk: the json item link component.
@RETURN link_t_ptr: return next sibling item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_json_next_sibling_item(link_t_ptr ilk);

/*
@FUNCTION get_json_prev_sibling_item: get json previous sibling item.
@INPUT link_t_ptr ilk: the json item link component.
@RETURN link_t_ptr: return previous sibling item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_json_prev_sibling_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif //XDL_SUPPORT_DOC

#endif	/*_JSONDOC_H*/