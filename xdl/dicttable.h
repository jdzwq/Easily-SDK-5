/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicttable document

	@module	dicttable.h | dicttable dword_terface file

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
#include "variant.h"

typedef struct _dict_enum_t{
	int index;
	link_t_ptr dict;
	link_t_ptr entity;
}dict_enum_t;

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_dict_table(void);

	XDL_API void destroy_dict_table(link_t_ptr ptr);

	XDL_API void copy_dict_table(link_t_ptr ptr_dest, link_t_ptr ptr_src);

	XDL_API void clear_dict_table(link_t_ptr ptr);

	XDL_API void delete_dict_entity(link_t_ptr elk);

	XDL_API link_t_ptr get_dict_entity(link_t_ptr ptr, variant_t key);

	XDL_API const variant_t* get_dict_entity_key_ptr(link_t_ptr elk);

	XDL_API void get_dict_entity_key(link_t_ptr elk, variant_t* pkey);

	XDL_API void set_dict_entity_key(link_t_ptr elk, variant_t key);

	XDL_API const object_t* get_dict_entity_val_ptr(link_t_ptr elk);

	XDL_API void get_dict_entity_val(link_t_ptr elk, object_t val);

	XDL_API void set_dict_entity_val(link_t_ptr elk, object_t val);

	XDL_API void attach_dict_entity_val(link_t_ptr elk, object_t val);

	XDL_API object_t detach_dict_entity_val(link_t_ptr elk);

	XDL_API var_long get_dict_entity_delta(link_t_ptr elk);

	XDL_API void set_dict_entity_delta(link_t_ptr elk, var_long mask);

	XDL_API int	enum_dict_entity(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* pv);

	XDL_API int get_dict_entity_count(link_t_ptr ptr);

	XDL_API link_t_ptr get_dict_entity_at(link_t_ptr ptr, int index);

	XDL_API int get_dict_entity_index(link_t_ptr ptr, link_t_ptr elk);

	XDL_API link_t_ptr get_dict_next_entity(dict_enum_t* pea);

	XDL_API link_t_ptr get_dict_prev_entity(dict_enum_t* pea);

	XDL_API void delete_dict_item(link_t_ptr ptr, variant_t key);

	XDL_API link_t_ptr write_dict_item(link_t_ptr ptr, variant_t key, object_t val);

	XDL_API bool_t read_dict_item(link_t_ptr ptr, variant_t key, object_t val);

	XDL_API const object_t* get_dict_item_ptr(link_t_ptr ptr, variant_t key);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_API void test_dict_table();
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_DICTTABLE_H*/
