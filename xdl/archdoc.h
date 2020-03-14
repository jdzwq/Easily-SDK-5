/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc arch document

	@module	archdoc.h | interface file

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

#ifndef _ARCHDOC_H
#define _ARCHDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@GET get_arch_item_name_ptr: get the archive name.
*/
#define get_arch_item_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)
/*
@PROPER name: string.
@SET set_arch_item_name: set the archive name.
*/
#define set_arch_item_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)
/*
@PROPER title: string.
@GET get_arch_item_title_ptr: get the archive title.
*/
#define get_arch_item_title_ptr(ilk)					get_dom_node_attr_ptr(ilk,ATTR_TITLE,-1)
/*
@PROPER title: string.
@SET set_arch_item_title: set the archive title.
*/
#define set_arch_item_title(ilk,token)					set_dom_node_attr(ilk,ATTR_TITLE,-1,token,-1)

/**********************************************************************************************************/

/*@PROPER expanded : boolean.
@SET set_arch_item_expanded: set the arch item expanded.
*/
#define set_arch_item_expanded(ilk,expand)				set_dom_node_mask_check(ilk,MSK_EXPANDED,expand)
/*
@PROPER expanded: document.
@GET get_arch_item_expanded: get the tree item expanded.
*/
#define get_arch_item_expanded(ilk)						get_dom_node_mask_check(ilk,MSK_EXPANDED)

/**********************************************************************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Fonctions***************************************************************************/

/*
@FUNCTION create_arch_doc: create a archive document.
@RETURN link_t_ptr: return the archive document link component.
*/
EXP_API link_t_ptr create_arch_doc(void);

/*
@FUNCTION destroy_arch_doc: destroy a archive document.
@INPUT link_t_ptr ptr: the archive link component.
@RETURN void: none.
*/
EXP_API void destroy_arch_doc(link_t_ptr ptr);

/*
@FUNCTION clear_arch_doc: clear the archive document.
@INPUT link_t_ptr ptr: the archive link component.
@RETURN void: none.
*/
EXP_API void clear_arch_doc(link_t_ptr ptr);

/*
@FUNCTION is_arch_doc: test is archive document.
@INPUT link_t_ptr ptr: the archive link component.
@RETURN bool_t: return nonzero for being a archive document, otherwise return zero.
*/
EXP_API bool_t is_arch_doc(link_t_ptr ptr);

/*
@FUNCTION is_arch_catalog: test is archive catalog node.
@INPUT link_t_ptr ptr: the archive link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a catalog, otherwise return zero.
*/
EXP_API bool_t is_arch_catalog(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION is_arch_document: test is archive document node.
@INPUT link_t_ptr ptr: the archive link component.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being a document, otherwise return zero.
*/
EXP_API bool_t is_arch_document(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION get_arch_item_count: counting the items in archive document.
@INPUT link_t_ptr ptr: the archive link component.
@RETURN int: return the number of nodes.
*/
EXP_API int get_arch_item_count(link_t_ptr ptr);

/*
@FUNCTION get_arch_item: find the item by name.
@INPUT link_t_ptr ptr: the archive link component.
@INPUT const tchar_t* iname: the name token.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_arch_item(link_t_ptr ptr, const tchar_t* iname);

/*
@FUNCTION insert_arch_catalog: add a new catalog item to archive document.
@INPUT link_t_ptr ptr: the archive link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the new item link component.
*/
EXP_API link_t_ptr insert_arch_catalog(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION insert_arch_document: add a new document item to archive document.
@INPUT link_t_ptr ptr: the archive link component.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@INPUT link_t_ptr doc: the document link component to attaching.
@RETURN link_t_ptr: return the new document item link component.
*/
EXP_API link_t_ptr insert_arch_document(link_t_ptr ptr, link_t_ptr pos, link_t_ptr doc);

/*
@FUNCTION delete_arch_item: delete the item in archive document.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
EXP_API void delete_arch_item(link_t_ptr ilk);

/*
@FUNCTION attach_arch_document: attach a document to item node.
@INPUT link_t_ptr ilk: the item link component.
@INPUT link_t_ptr doc: the document link component.
@RETURN link_t_ptr: return the original document link component attached.
*/
EXP_API link_t_ptr attach_arch_document(link_t_ptr ilk, link_t_ptr doc);

/*
@FUNCTION detach_arch_document: detach the document from item node.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the original document link component attached.
*/
EXP_API link_t_ptr detach_arch_document(link_t_ptr ilk);

/*
@FUNCTION fetch_arch_document: get the document from item node.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the document link component attached.
*/
EXP_API link_t_ptr fetch_arch_document(link_t_ptr ilk);

/*
@FUNCTION get_arch_first_child_item: get the first child item.
@INPUT link_t_ptr ilk: the parent link component.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_arch_first_child_item(link_t_ptr ilk);

/*
@FUNCTION get_arch_last_child_item: get the last child item.
@INPUT link_t_ptr ilk: the parent link component.
@RETURN link_t_ptr: return the item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_arch_last_child_item(link_t_ptr ilk);

/*
@FUNCTION get_arch_parent_item: get the last parent item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the parent link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_arch_parent_item(link_t_ptr ilk);

/*
@FUNCTION get_arch_next_sibling_item: get the next sibling item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the next sibling item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_arch_next_sibling_item(link_t_ptr ilk);

/*
@FUNCTION get_arch_prev_sibling_item: get the previous sibling item.
@INPUT link_t_ptr ilk: the item link component.
@RETURN link_t_ptr: return the previous sibling item link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_arch_prev_sibling_item(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif //XDL_SUPPORT_DOC

#endif	/*_ARCHDOC_H*/