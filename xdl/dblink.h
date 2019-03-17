/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc double link list document

	@module	dblink.h | double link list interface file

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

#ifndef _DBLINK_H
#define _DBLINK_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION init_root_link: initialize a empty link.
@INPUT link_t_ptr root: root link component.
@RETURN void: none.
*/
XDL_API void init_root_link(link_t_ptr root);

/*
@FUNCTION is_empty_link: test is a empty link.
@INPUT link_t_ptr root: root link component.
@RETURN bool_t: return nonzero for being a empty link.
*/
XDL_API bool_t is_empty_link(link_t_ptr root);

/*
@FUNCTION is_root_link: test the link is root component.
@INPUT link_t_ptr plk: the link component.
@RETURN bool_t: return nonzero for root link component.
*/
XDL_API bool_t is_root_link(link_t_ptr plk);

/*
@FUNCTION is_first_link: test the link is first link component.
@INPUT link_t_ptr plk: the link component.
@RETURN bool_t: return nonzero for being a first link component.
*/
XDL_API bool_t is_first_link(link_t_ptr plk);

/*
@FUNCTION is_last_link: test the link is last link component.
@INPUT link_t_ptr plk: the link component.
@RETURN bool_t: return nonzero for being a last link component.
*/
XDL_API bool_t is_last_link(link_t_ptr plk);

/*
@FUNCTION is_child_link: test the link is a child link component of root.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk: the link component.
@RETURN bool_t: return nonzero for being a child link component.
*/
XDL_API bool_t is_child_link(link_t_ptr root, link_t_ptr plk);

/*
@FUNCTION insert_link_before: insert a link component into root as child befor the position, 
the position must be one child in the root or indicant.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr pos: the link component in root, or indicant LINK_FIRST and LINK_LAST.
if LINK_FIRST, the new link component will be inserted as first component of root,
if LINK_LAST, the new link component will be inserted as last component of root.
@INPUT link_t_ptr pnew: the new link component to be insert.
@RETURN void: none.
*/
XDL_API void insert_link_before(link_t_ptr root,link_t_ptr pos,link_t_ptr pnew);

/*
@FUNCTION insert_link_after: insert a link component into root as child after the position,
the position must be one child in the root or indicant.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr pos: the link component in root, or indicant LINK_FIRST and LINK_LAST.
if LINK_FIRST, the new link component will be inserted as first component of root,
if LINK_LAST, the new link component will be inserted as last component of root.
@INPUT link_t_ptr pnew: the new link component to be insert.
@RETURN void: none.
*/
XDL_API void insert_link_after(link_t_ptr root,link_t_ptr pos,link_t_ptr pnew);

/*
@FUNCTION delete_link: remove the link component in root.
the position must be one child in the root or indicant.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk: the link component in root, or indicant LINK_FIRST and LINK_LAST.
if LINK_FIRST, the first link component in root will be removed.
if LINK_LAST, the last link component will be removed.
@RETURN link_t_ptr: return the removed link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr delete_link(link_t_ptr root, link_t_ptr plk);

/*
@FUNCTION get_link_count: get the link component count in root.
@INPUT link_t_ptr root: the root link component.
@RETURN int: return link component count, empty root return zero.
*/
XDL_API int get_link_count(link_t_ptr root);

/*
@FUNCTION get_link_at: get the link component by position.
@INPUT link_t_ptr root: the root link component.
@INPUT int index: the position, zero indicate the first link, negative indicate the last link.
@RETURN link_t_ptr: return link if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_link_at(link_t_ptr root,int index);

/*
@FUNCTION insert_link_at: insert the the link component at position.
@INPUT link_t_ptr root: the root link component.
@INPUT int index: the position, zero based, negative indicate inserting at last.
@RETURN void: none.
*/
XDL_API void insert_link_at(link_t_ptr root, int index, link_t_ptr pnew);

/*
@FUNCTION delete_link_at: remove the the link component at position.
@INPUT link_t_ptr root: the root link component.
@INPUT int index: the position, zero based, negative indicate inserting at last.
@RETURN link_t_ptr: return the link component removed if exists, otherwise return NULL.
*/
XDL_API link_t_ptr delete_link_at(link_t_ptr root, int index);

/*
@FUNCTION get_link_index: calc the link component position in root.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk: the link component.
@RETURN link_t_ptr: return zero based position if the link component exists, otherwise return -1.
*/
XDL_API int get_link_index(link_t_ptr root,link_t_ptr plk);

/*
@FUNCTION get_first_link: get the first link component in root.
@INPUT link_t_ptr root: the root link component.
@RETURN link_t_ptr: return the first link if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_first_link(link_t_ptr root);

/*
@FUNCTION link_t_ptr: get the next link component.
@INPUT link_t_ptr plk: the current link component.
@RETURN link_t_ptr: return the next link if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_next_link(link_t_ptr plk);

/*
@FUNCTION get_prev_link: get the previous link component.
@INPUT link_t_ptr plk: the current link component.
@RETURN link_t_ptr: return the previous link if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_prev_link(link_t_ptr plk);

/*
@FUNCTION get_last_link: get the last link component in root.
@INPUT link_t_ptr root: the root link component.
@RETURN link_t_ptr: return the last link if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_last_link(link_t_ptr root);

/*
@FUNCTION get_root_link: get the root link component.
@INPUT link_t_ptr plk: the link component in root.
@RETURN link_t_ptr: return the root link if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_root_link(link_t_ptr plk);

/*
@FUNCTION merge_link: merge the source root link children into destination root, then the source root is empty.
@INPUT link_t_ptr root_dst: the destination root link component.
@INPUT link_t_ptr root_src: the source root link component.
@RETURN void: none.
*/
XDL_API void merge_link(link_t_ptr root_dst, link_t_ptr root_src);

/*
@FUNCTION push_link: insert the link component into root at last.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk: the link component.
@RETURN void: none.
*/
XDL_API void push_link(link_t_ptr root,link_t_ptr plk);

/*
@FUNCTION pop_link: remove the last link component in root.
@INPUT link_t_ptr root: the root link component.
@RETURN link_t_ptr: return the link if exists, otherwise return NULL.
*/
XDL_API link_t_ptr pop_link(link_t_ptr root);

/*
@FUNCTION pop_link: get the last link component in root.
@INPUT link_t_ptr root: the root link component.
@RETURN link_t_ptr: return the link if exists, otherwise return NULL.
*/
XDL_API link_t_ptr peek_link(link_t_ptr root);

/*
@FUNCTION switch_link: switch two link component position in root.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk1: the link component.
@INPUT link_t_ptr plk2: the link component.
@RETURN void: none.
*/
XDL_API void switch_link(link_t_ptr root,link_t_ptr plk1,link_t_ptr plk2);

/*
@FUNCTION switch_link_next: switch the link component to next position.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk: the link component.
@RETURN void: none.
*/
XDL_API void switch_link_next(link_t_ptr root, link_t_ptr plk);

/*
@FUNCTION switch_link_next: switch the link component to previous position.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk: the link component.
@RETURN void: none.
*/
XDL_API void switch_link_prev(link_t_ptr root, link_t_ptr plk);

/*
@FUNCTION switch_link_first: switch the link component to first position.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk: the link component.
@RETURN void: none.
*/
XDL_API void switch_link_first(link_t_ptr root, link_t_ptr plk);

/*
@FUNCTION switch_link_last: switch the link component to last position.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr plk: the link component.
@RETURN void: none.
*/
XDL_API void switch_link_last(link_t_ptr root, link_t_ptr plk);

/*
@FUNCTION switch_link_before: switch the link component to the position before some link component.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr next: the link component, it can be indocator, LINK_FIRST or LINK_LAST.
@INPUT link_t_ptr plk: the link component be to switched.
@RETURN void: none.
*/
XDL_API void switch_link_before(link_t_ptr root, link_t_ptr next, link_t_ptr plk);

/*
@FUNCTION switch_link_after: switch the link component to the position after some link component.
@INPUT link_t_ptr root: the root link component.
@INPUT link_t_ptr next: the link component, it can be indocator, LINK_FIRST or LINK_LAST.
@INPUT link_t_ptr plk: the link component be to switched.
@RETURN void: none.
*/
XDL_API void switch_link_after(link_t_ptr root, link_t_ptr prev, link_t_ptr plk);

/*
@FUNCTION bubble_sort_link: bubble sorting the root link component.
@INPUT link_t_ptr root: the root link component.
@INPUT CALLBACK_SORTLINK pf: the compare callback function.
@INPUT bool_t dest: nonzero for descending, zero for ascending.
@INPUT void* parm: the parameter translated into compare callback function.
@RETURN void: none.
*/
XDL_API void bubble_sort_link(link_t_ptr root,CALLBACK_SORTLINK pf, bool_t desc,void* parm);

/*
@FUNCTION merge_sort_link: merge sorting the root link component.
@INPUT link_t_ptr root: the root link component.
@INPUT CALLBACK_SORTLINK pf: the compare callback function.
@INPUT bool_t dest: nonzero for descending, zero for ascending.
@INPUT void* parm: the parameter translated into compare callback function.
@RETURN void: none.
*/
XDL_API void merge_sort_link(link_t_ptr root, CALLBACK_SORTLINK pf, bool_t desc, void* parm);

/*
@FUNCTION heap_sort_link: heap sorting the root link component.
@INPUT link_t_ptr root: the root link component.
@INPUT CALLBACK_SORTLINK pf: the compare callback function.
@INPUT bool_t dest: nonzero for descending, zero for ascending.
@INPUT void* parm: the parameter translated into compare callback function.
@RETURN void: none.
*/
XDL_API void heap_sort_link(link_t_ptr root, CALLBACK_SORTLINK pf, bool_t desc, void* parm);

/*
@FUNCTION quick_sort_link: quick sorting the root link component.
@INPUT link_t_ptr root: the root link component.
@INPUT CALLBACK_SORTLINK pf: the compare callback function.
@INPUT bool_t dest: nonzero for descending, zero for ascending.
@INPUT void* parm: the parameter translated into compare callback function.
@RETURN void: none.
*/
XDL_API void quick_sort_link(link_t_ptr root, CALLBACK_SORTLINK pf, bool_t desc, void* parm);

/*
@FUNCTION enum_link: enum link component in root.
@INPUT link_t_ptr root: the root link component.
@INPUT CALLBACK_ENUMLINK pf: the enum callback function.
@INPUT void* parm: the parameter translated into enum callback function.
@RETURN void: none.
*/
XDL_API void enum_link(link_t_ptr root,CALLBACK_ENUMLINK pf,void* parm);

#ifdef	__cplusplus
}
#endif

#endif /*_DBLINK_H*/


