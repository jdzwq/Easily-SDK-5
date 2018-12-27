/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hashtable document

	@module	hashtable.h | hashtable interface file

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

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include "xdldef.h"

typedef struct _hash_enum_t{
	int index;
	link_t_ptr hash;
	link_t_ptr entity;
}hash_enum_t;

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_hash_table(void);

	XDL_API void destroy_hash_table(link_t_ptr ptr);

	XDL_API void copy_hash_table(link_t_ptr ptr_dest, link_t_ptr ptr_src);

	XDL_API void clear_hash_table(link_t_ptr ptr);

	XDL_API void delete_hash_entity(link_t_ptr elk);

	XDL_API link_t_ptr get_hash_entity(link_t_ptr ptr, const tchar_t* key, int keylen);

	XDL_API const tchar_t* get_hash_entity_key_ptr(link_t_ptr elk);

	XDL_API const tchar_t* get_hash_entity_val_ptr(link_t_ptr elk);

	XDL_API int get_hash_entity_val_len(link_t_ptr elk);

	XDL_API int get_hash_entity_key(link_t_ptr elk, tchar_t* buf, int max);

	XDL_API int get_hash_entity_val(link_t_ptr elk, tchar_t* buf, int max);

	XDL_API void set_hash_entity_delta(link_t_ptr elk, var_long data);

	XDL_API var_long get_hash_entity_delta(link_t_ptr elk);

	XDL_API void attach_hash_entity_val(link_t_ptr elk, tchar_t* val);

	XDL_API tchar_t* detach_hash_entity_val(link_t_ptr elk);

	XDL_API int	enum_hash_entity(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* pv);

	XDL_API int get_hash_entity_count(link_t_ptr ptr);

	XDL_API link_t_ptr get_hash_entity_at(link_t_ptr ptr, int index);

	XDL_API int get_hash_entity_index(link_t_ptr ptr, link_t_ptr elk);

	XDL_API link_t_ptr get_hash_next_entity(hash_enum_t* pea);

	XDL_API link_t_ptr get_hash_prev_entity(hash_enum_t* pea);

	XDL_API void delete_hash_attr(link_t_ptr ptr, const tchar_t* key, int keylen);

	XDL_API link_t_ptr write_hash_attr(link_t_ptr ptr, const tchar_t* key, int keylen, const tchar_t* val, int vallen);

	XDL_API int read_hash_attr(link_t_ptr ptr, const tchar_t* key, int keylen, tchar_t* buf, int max);

	XDL_API const tchar_t* get_hash_attr_ptr(link_t_ptr ptr, const tchar_t* key, int keylen);

	XDL_API int get_hash_attr_len(link_t_ptr ptr, const tchar_t* key, int keylen);

	XDL_API link_t_ptr set_hash_attr_boolean(link_t_ptr ptr, const tchar_t* key, bool_t b);

	XDL_API bool_t get_hash_attr_boolean(link_t_ptr ptr, const tchar_t* key);

	XDL_API link_t_ptr set_hash_attr_integer(link_t_ptr ptr, const tchar_t* key, long ul);

	XDL_API long get_hash_attr_integer(link_t_ptr ptr, const tchar_t* key);

	XDL_API link_t_ptr set_hash_attr_float(link_t_ptr ptr, const tchar_t* key, float f);

	XDL_API float get_hash_attr_float(link_t_ptr ptr, const tchar_t* key);

	XDL_API link_t_ptr set_hash_attr_numeric(link_t_ptr ptr, const tchar_t* key, double db);

	XDL_API double get_hash_attr_numeric(link_t_ptr ptr, const tchar_t* key);

	XDL_API link_t_ptr write_hash_bytes(link_t_ptr ptr, const tchar_t* key, int keylen, const byte_t* val, dword_t vallen);

	XDL_API dword_t read_hash_bytes(link_t_ptr ptr, const tchar_t* key, int keylen, byte_t* buf, dword_t max);

	XDL_API const byte_t* get_hash_bytes_ptr(link_t_ptr ptr, const tchar_t* key, int keylen);

	XDL_API dword_t get_hash_bytes_size(link_t_ptr ptr, const tchar_t* key, int keylen);

	XDL_API const byte_t* get_hash_entity_bytes_ptr(link_t_ptr elk);

	XDL_API dword_t get_hash_entity_bytes_size(link_t_ptr elk);

	XDL_API void attach_hash_entity_bytes(link_t_ptr elk, byte_t* val, dword_t len);

	XDL_API byte_t* detach_hash_entity_bytes(link_t_ptr elk);

	XDL_API const tchar_t* hash_attr_set_chars(link_t_ptr ilk, const tchar_t* key, int pos, const tchar_t* pch, int n);

	XDL_API const tchar_t* hash_attr_ins_chars(link_t_ptr ilk, const tchar_t* key, int pos, const tchar_t* pch, int n);

	XDL_API const tchar_t* hash_attr_del_chars(link_t_ptr ilk, const tchar_t* key, int pos, int n);

	XDL_API void hash_table_parse_attrset(link_t_ptr ptr, const tchar_t* attrset, int len);

	XDL_API int hash_table_format_attrset(link_t_ptr ptr, tchar_t* buf, int max);

	XDL_API int hash_table_compare_attr(const tchar_t* key, const tchar_t* sign, const tchar_t* val, void* param);

	XDL_API int hash_table_parse_options(link_t_ptr ptr, const tchar_t* str, int len, tchar_t itemfeed, tchar_t linefeed);

	XDL_API int hash_table_format_options(link_t_ptr ptr, tchar_t* buf, int max, tchar_t itemfeed, tchar_t linefeed);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_API void test_hash_table();
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_HASHTABLE_H*/
