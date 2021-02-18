/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicttable document

	@module	dicttable.h | interface file

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

#ifndef _DICTTABLE_H
#define _DICTTABLE_H

#include "xdldef.h"

typedef struct _dict_enum_t{
	int index;
	link_t_ptr dict;
	link_t_ptr entity;
}dict_enum_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_dict_table: create a dictionary table.
@RETURN link_t_ptr: return the dictionary table link component.
*/
EXP_API link_t_ptr create_dict_table(void);

/*
@FUNCTION destroy_dict_table: destroy the dictionary table.
@INPUT link_t_ptr ptr: the dictionary table link component.
@RETURN void: none.
*/
EXP_API void destroy_dict_table(link_t_ptr ptr);

/*
@FUNCTION copy_dict_table: copy source dictionary table to destination dictionary table.
@INPUT link_t_ptr ptr_dst: the destination dictionary table link component.
@INPUT link_t_ptr ptr_src: the source dictionary table link component.
@RETURN void: none.
*/
EXP_API void copy_dict_table(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION clear_dict_table: clear the dictionary table.
@INPUT link_t_ptr ptr: the dictionary table link component.
@RETURN void: none.
*/
EXP_API void clear_dict_table(link_t_ptr ptr);

/*
@FUNCTION delete_dict_entity: delete a dictionary entity.
@INPUT link_t_ptr elk: the dictionary entity link component.
@RETURN void: none.
*/
EXP_API void delete_dict_entity(link_t_ptr elk);

/*
@FUNCTION get_dict_entity: get the dictionary entity by key.
@INPUT link_t_ptr ptr: the dictionary table link component.
@INPUT variant_t key: the variant key.
@RETURN link_t_ptr: return the entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_dict_entity(link_t_ptr ptr, variant_t key);

/*
@FUNCTION get_dict_entity_key_ptr: get the dictionary entity key variant struct.
@INPUT link_t_ptr elk: the dictionary entity link component.
@RETURN const variant_t*: return the variant key struct.
*/
EXP_API const variant_t* get_dict_entity_key_ptr(link_t_ptr elk);

/*
@FUNCTION get_dict_entity_key: get the dictionary entity key.
@INPUT link_t_ptr elk: the dictionary entity link component.
@OUTPUT variant_t key: the variant struct for returning key.
@RETURN void: none.
*/
EXP_API void get_dict_entity_key(link_t_ptr elk, variant_t key);

/*
@FUNCTION set_dict_entity_key: set the dictionary entity key.
@INPUT link_t_ptr elk: the dictionary entity link component.
@INPUT variant_t key: the variant key.
@RETURN void: none.
*/
EXP_API void set_dict_entity_key(link_t_ptr elk, variant_t key);

/*
@FUNCTION get_dict_entity_val_ptr: get the dictionary entity value object struct.
@INPUT link_t_ptr elk: the dictionary entity link component.
@RETURN const object_t*: return the value object struct if exists, otherwise return NULL.
*/
EXP_API const object_t* get_dict_entity_val_ptr(link_t_ptr elk);

/*
@FUNCTION get_dict_entity_val: get the dictionary entity value.
@INPUT link_t_ptr elk: the dictionary entity link component.
@OUTPUT object_t val: the object for returning value.
@RETURN void: none.
*/
EXP_API void get_dict_entity_val(link_t_ptr elk, object_t val);

/*
@FUNCTION set_dict_entity_val: set the dictionary entity value.
@INPUT link_t_ptr elk: the dictionary entity link component.
@INPUT object_t val: the object value.
@RETURN void: none.
*/
EXP_API void set_dict_entity_val(link_t_ptr elk, object_t val);

/*
@FUNCTION attach_dict_entity_val: attach the dictionary entity value, then the original entity value will be freed and replaced.
@INPUT link_t_ptr elk: the dictionary entity link component.
@INPUT object_t val: the object value.
@RETURN void: none.
*/
EXP_API void attach_dict_entity_val(link_t_ptr elk, object_t val);

/*
@FUNCTION detach_dict_entity_val: detach the dictionary entity value, then the entity value will be empty.
@INPUT link_t_ptr elk: the dictionary entity link component.
@RETURN object_t: return entity object value detached if exists, otherwise return NULL.
*/
EXP_API object_t detach_dict_entity_val(link_t_ptr elk);

/*
@FUNCTION get_dict_entity_delta: get the dictionary entity extract value.
@INPUT link_t_ptr elk: the dictionary entity link component.
@RETURN var_long: return extract value if exists, otherwise return zero.
*/
EXP_API var_long get_dict_entity_delta(link_t_ptr elk);

/*
@FUNCTION set_dict_entity_delta: set the dictionary entity extract value.
@INPUT link_t_ptr elk: the dictionary entity link component.
@INPUT var_long val: the extract value.
@RETURN void: none.
*/
EXP_API void set_dict_entity_delta(link_t_ptr elk, var_long val);

/*
@FUNCTION enum_dict_entity: enum the dictionary entites.
@INPUT link_t_ptr ptr: the dictionary table link component.
@INPUT CALLBACK_ENUMLINK pf: the callback function.
@INPUT void* pv: the parameter translate into callback function.
@RETURN int: return the count of entites enumerated.
*/
EXP_API int	enum_dict_entity(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* pv);

/*
@FUNCTION get_dict_entity_count: get the dictionary entites count.
@INPUT link_t_ptr ptr: the dictionary table link component.
@RETURN int: return the count of entites.
*/
EXP_API int get_dict_entity_count(link_t_ptr ptr);

/*
@FUNCTION get_dict_entity_at: get the dictionary entity by position.
@INPUT link_t_ptr ptr: the dictionary table link component.
@INPUT int index: zero based position.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_dict_entity_at(link_t_ptr ptr, int index);

/*
@FUNCTION get_dict_entity_index: get the dictionary entity position.
@INPUT link_t_ptr ptr: the dictionary table link component.
@INPUT link_t_ptr elk: the dictionary entity link component.
@RETURN int: return the zero based position.
*/
EXP_API int get_dict_entity_index(link_t_ptr ptr, link_t_ptr elk);

/*
@FUNCTION get_dict_next_entity: get the dictionary next entity.
@INOUTPUT dict_enum_t* pea: the dictionary eumeration struct.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_dict_next_entity(dict_enum_t* pea);

/*
@FUNCTION get_dict_prev_entity: get the dictionary previous entity.
@INOUTPUT dict_enum_t* pea: the dictionary eumeration struct.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_dict_prev_entity(dict_enum_t* pea);

/*
@FUNCTION delete_dict_item: delete the dictionary entity by key.
@INPUT link_t_ptr ptr: the dictionary table link component.
@INPUT variant_t key: the variant key.
@RETURN void: none.
*/
EXP_API void delete_dict_item(link_t_ptr ptr, variant_t key);

/*
@FUNCTION write_dict_item: write key and value into the dictionary table,
if the entity indicated by key is exists, the value of entity will be overwrited, otherwise insert a new entity with the key and value.
@INPUT link_t_ptr ptr: the dictionary table link component.
@INPUT variant_t key: the variant key.
@INPUT object_t val: the object value.
@RETURN link_t_ptr: return the entity link component.
*/
EXP_API link_t_ptr write_dict_item(link_t_ptr ptr, variant_t key, object_t val);

/*
@FUNCTION read_dict_item: read entity value by the key.
@INPUT link_t_ptr ptr: the dictionary table link component.
@INPUT variant_t key: the variant key.
@OUTPUT object_t val: the object for return value.
@RETURN bool_t: return nonzero if entity exists, otherwise return zero.
*/
EXP_API bool_t read_dict_item(link_t_ptr ptr, variant_t key, object_t val);

/*
@FUNCTION get_dict_item_ptr: get entity value pointer by the key.
@INPUT link_t_ptr ptr: the dictionary table link component.
@INPUT variant_t key: the variant key.
@RETURN const object_t: return entity object pointer if exists, otherwise return NULL.
*/
EXP_API const object_t* get_dict_item_ptr(link_t_ptr ptr, variant_t key);

#if defined(XDL_SUPPORT_TEST)
	EXP_API void test_dict_table();
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_DICTTABLE_H*/
