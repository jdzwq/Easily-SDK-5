﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc B+ tree document

	@module	bplustree.h | interface file

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

#ifndef _BPLUSTREE_H
#define _BPLUSTREE_H

#include "xdldef.h"
#include "variant.h"

/*enum bplus node callback function*/
typedef bool_t(*CALLBACK_ENUMBPLUSENTITY)(variant_t key, object_t val, void* pv);

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_bplus_tree: create a bplus tree.
@RETURN link_t_ptr: return the bplus tree link component.
*/
EXP_API link_t_ptr create_bplus_tree(void);

/*
@FUNCTION create_bplus_file_table: create a bplus tree with index and data file table.
@INPUT link_t_ptr index_table: the index file table.
@INPUT link_t_ptr data_table: the data file table.
@RETURN link_t_ptr: return the bplus tree link component.
*/
EXP_API link_t_ptr create_bplus_file_table(link_t_ptr index_table, link_t_ptr data_table);

/*
@FUNCTION destroy_bplus_tree: destroy a bplus tree.
@INPUT link_t_ptr ptr: the bplus tree link component.
@RETURN void: none.
*/
EXP_API void destroy_bplus_tree(link_t_ptr ptr);

/*
@FUNCTION clear_bplus_tree: clear all child entity in the bplus tree.
@INPUT link_t_ptr ptr: the bplus tree link component.
@RETURN void: none.
*/
EXP_API void clear_bplus_tree(link_t_ptr ptr);

/*
@FUNCTION is_bplus_tree: test is the bplus tree.
@INPUT link_t_ptr ptr: the bplus tree link component.
@RETURN boo_t: return nonzero for bplus tree, otherwise return zero.
*/
EXP_API bool_t is_bplus_tree(link_t_ptr ptr);

/*
@FUNCTION insert_bplus_entity: insert a entity with the key and value.
@INPUT link_t_ptr ptr: the bplus tree link component.
@INPUT variant_t var: the variant key.
@INPUT object_t val: the object value.
@RETURN boo_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t insert_bplus_entity(link_t_ptr ptr, variant_t key, object_t val);

/*
@FUNCTION delete_bplus_entity: delete a entity by the key.
@INPUT link_t_ptr ptr: the bplus tree link component.
@INPUT variant_t key: the variant key.
@RETURN boo_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t delete_bplus_entity(link_t_ptr ptr, variant_t key);

/*
@FUNCTION find_bplus_entity: find a entity by the key and get the entity value.
@INPUT link_t_ptr ptr: the bplus tree link component.
@INPUT variant_t key: the variant key.
@OUTPUT object_t val: the object for returning value.
@RETURN boo_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t find_bplus_entity(link_t_ptr ptr, variant_t var, object_t val);

/*
@FUNCTION enum_bplus_entity: enum bplus tree entities.
@INPUT link_t_ptr ptr: the bplus tree link component.
@INPUT CALLBACK_ENUMLINK pf: the callback function.
@INPUT void* param: the parameter translate into callback function.
@RETURN void: none.
*/
EXP_API void enum_bplus_entity(link_t_ptr ptr, CALLBACK_ENUMBPLUSENTITY pf, void* param);


#if defined(XDL_SUPPORT_TEST)
	EXP_API void test_bplus_tree_none_table();
	EXP_API void test_bplus_tree_file_table(const tchar_t* tname, dword_t tmask);
#endif

#ifdef	__cplusplus
}
#endif


#endif /*_BPLUSTREE_H*/
