/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc multi tree document

	@module	multitree.h | multi tree interface file

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

#ifndef _MULTITREE_H
#define _MULTITREE_H

#include "xdldef.h"

/****************************************************************************************************************************/
#define set_multi_node_attr(ptr,key,keylen,val,vallen)	write_hash_attr(get_multi_node_attr_table(ptr),key,keylen,val,vallen)

#define get_multi_node_attr(ptr,key,keylen,buf,max)		read_hash_attr(get_multi_node_attr_table(ptr),key,keylen,buf,max)

#define get_multi_node_attr_ptr(ptr,key,keylen)			get_hash_attr_ptr(get_multi_node_attr_table(ptr),key,keylen)

#define get_multi_node_attr_len(ptr,key,keylen)			get_hash_attr_len(get_multi_node_attr_table(ptr),key,keylen)
/*************************************************************************************************************************/

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_multi_tree(void);

	XDL_API void destroy_multi_tree(link_t_ptr ptr);

	XDL_API bool_t is_multi_tree(link_t_ptr ptr);

	XDL_API bool_t is_multi_child_node(link_t_ptr ilk, link_t_ptr plk);

	XDL_API link_t_ptr enum_multi_tree(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param);

	XDL_API link_t_ptr enum_multi_tree_deep(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param);

	XDL_API link_t_ptr insert_multi_node_before(link_t_ptr ilk, link_t_ptr pos);

	XDL_API link_t_ptr insert_multi_node(link_t_ptr ilk, link_t_ptr pos);

	XDL_API void delete_multi_node(link_t_ptr ilk);

	XDL_API void delete_multi_child_nodes(link_t_ptr ilk);

	XDL_API int get_multi_child_node_count(link_t_ptr ilk);

	XDL_API link_t_ptr get_multi_first_child_node(link_t_ptr ilk);

	XDL_API link_t_ptr get_multi_last_child_node(link_t_ptr ilk);

	XDL_API link_t_ptr get_multi_parent_node(link_t_ptr ilk);

	XDL_API link_t_ptr get_multi_next_sibling_node(link_t_ptr ilk);

	XDL_API link_t_ptr get_multi_prev_sibling_node(link_t_ptr ilk);

	XDL_API link_t_ptr get_multi_node_attr_table(link_t_ptr ilk);

	XDL_API var_long get_multi_node_delta(link_t_ptr ilk);

	XDL_API void set_multi_node_delta(link_t_ptr ilk,var_long delta);

	XDL_API dword_t get_multi_node_mask(link_t_ptr ilk);

	XDL_API void set_multi_node_mask(link_t_ptr ilk, dword_t ul);

	XDL_API void set_multi_node_mask_check(link_t_ptr ilk, dword_t ul, bool_t b);

	XDL_API bool_t get_multi_node_mask_check(link_t_ptr ilk, dword_t ul);

	XDL_API int get_multi_child_node_mask_check_count(link_t_ptr ilk, dword_t ul);

	XDL_API int set_multi_child_node_mask_check(link_t_ptr ilk, dword_t ul, bool_t b);



#ifdef	__cplusplus
}
#endif


#endif /*_MULTITREE_H*/
