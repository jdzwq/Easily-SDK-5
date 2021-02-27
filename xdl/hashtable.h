/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hashtable document

	@module	hashtable.h | interface file

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

/*
@FUNCTION create_hash_table: create a hash table.
@RETURN link_t_ptr: return the hash table link component.
*/
EXP_API link_t_ptr create_hash_table(void);

/*
@FUNCTION destroy_hash_table: destroy a hash table.
@INPUT link_t_ptr ptr: the hash table link component.
@RETURN void: none.
*/
EXP_API void destroy_hash_table(link_t_ptr ptr);

/*
@FUNCTION copy_hash_table: copy source hash table to destination hash table.
@INPUT link_t_ptr ptr_dst: the destination hash table link component.
@INPUT link_t_ptr ptr_src: the source hash table link component.
@RETURN void: none.
*/
EXP_API void copy_hash_table(link_t_ptr ptr_dst, link_t_ptr ptr_src);

/*
@FUNCTION clear_hash_table: clear a hash table, all entities will be deleted.
@INPUT link_t_ptr ptr: the hash table link component.
@RETURN void: none.
*/
EXP_API void clear_hash_table(link_t_ptr ptr);

/*
@FUNCTION delete_hash_entity: delete a hash entity.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN void: none.
*/
EXP_API void delete_hash_entity(link_t_ptr elk);

/*
@FUNCTION get_hash_entity: get a hash entity by key.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key token length in characters.
@RETURN link_t_ptr: return the hash entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_hash_entity(link_t_ptr ptr, const tchar_t* key, int keylen);

/*
@FUNCTION get_hash_entity_key_ptr: get a hash entity key string pointer.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN const tchar_t*: return the hash entity key token pointer.
*/
EXP_API const tchar_t* get_hash_entity_key_ptr(link_t_ptr elk);

/*
@FUNCTION get_hash_entity_val_ptr: get a hash entity value string pointer.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN const tchar_t*: return the hash entity value token pointer.
*/
EXP_API const tchar_t* get_hash_entity_val_ptr(link_t_ptr elk);

/*
@FUNCTION get_hash_entity_key: copy the hash entity key string.
@INPUT link_t_ptr elk: the hash entity link component.
@INPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer maximized size, not inlude terminate character.
@RETURN int: return the hash entity key token length in characters.
*/
EXP_API int get_hash_entity_key(link_t_ptr elk, tchar_t* buf, int max);

/*
@FUNCTION get_hash_entity_val: copy the hash entity value string.
@INPUT link_t_ptr elk: the hash entity link component.
@INPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer maximized size, not inlude terminate character.
@RETURN int: return the hash entity value token length in characters.
*/
EXP_API int get_hash_entity_val(link_t_ptr elk, tchar_t* buf, int max);

/*
@FUNCTION set_hash_entity_delta: set the hash entity extract data.
@INPUT link_t_ptr elk: the hash entity link component.
@INPUT vword_t data: the extract data.
@RETURN void: none.
*/
EXP_API void set_hash_entity_delta(link_t_ptr elk, vword_t data);

/*
@FUNCTION get_hash_entity_delta: get the hash entity extract data.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN vword_t: return the hash entity extract data.
*/
EXP_API vword_t get_hash_entity_delta(link_t_ptr elk);

/*
@FUNCTION attach_hash_entity_val: attach a value buffer to the hash entity.
@INPUT link_t_ptr elk: the hash entity link component.
@INPUT tchar_t* val: the value string buffer.
@RETURN void: none.
*/
EXP_API void attach_hash_entity_val(link_t_ptr elk, tchar_t* val);

/*
@FUNCTION detach_hash_entity_val: detach a value buffer from the hash entity.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN tchar_t*: return the value string buffer if exists, otherwise return NULL.
*/
EXP_API tchar_t* detach_hash_entity_val(link_t_ptr elk);

/*
@FUNCTION enum_hash_entity: enum hash entities.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT CALLBACK_ENUMLINK pf: the callback function, if the function return zero, the enumeration will be breaked.
@INPUT void* pv: the param translated into callback function.
@RETURN int: return the count of enumerated hash entity.
*/
EXP_API int	enum_hash_entity(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* pv);

/*
@FUNCTION get_hash_entity_count: counting hash entities.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT CALLBACK_ENUMLINK pf: the callback function, if the function return zero, the enumeration will be breaked.
@INPUT void* pv: the param translated into callback function.
@RETURN int: return the count of enumerated hash entity.
*/
EXP_API int get_hash_entity_count(link_t_ptr ptr);

/*
@FUNCTION get_hash_entity_at: get hash entity at posotion.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return hash entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_hash_entity_at(link_t_ptr ptr, int index);

/*
@FUNCTION get_hash_entity_index: get hash entity posotion.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN int: return zero based position if entity inside table, otherwise return -1.
*/
EXP_API int get_hash_entity_index(link_t_ptr ptr, link_t_ptr elk);

/*
@FUNCTION get_hash_next_entity: get next hash entity.
@INPUT hash_enum_t* pea: the hash enumertor struct.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_hash_next_entity(hash_enum_t* pea);

/*
@FUNCTION get_hash_prev_entity: get previous hash entity.
@INPUT hash_enum_t* pea: the hash enumertor struct.
@RETURN link_t_ptr: return entity link component if exists, otherwise return NULL.
*/
EXP_API link_t_ptr get_hash_prev_entity(hash_enum_t* pea);

/*
@FUNCTION delete_hash_attr: delete the hash entity by key.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@RETURN void: none.
*/
EXP_API void delete_hash_attr(link_t_ptr ptr, const tchar_t* key, int keylen);

/*
@FUNCTION write_hash_attr: write a hash key and value into hash table,
if the key exists in hash table, the entity will be overwrited, else a new entity will be inserted.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@INPUT const tchar_t* val: the value string token.
@INPUT int vallen: the value string token length in characters.
@RETURN link_t_ptr: the hash entity link componet.
*/
EXP_API link_t_ptr write_hash_attr(link_t_ptr ptr, const tchar_t* key, int keylen, const tchar_t* val, int vallen);

/*
@FUNCTION read_hash_attr: read hash value from hash table by key.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@OUTPUT tchar_t* buf: the string buffer for returning value.
@INPUT int max: the string buffer length in characters, not include terminate character.
@RETURN int: return the hash entity value length in characters if exists, otherwise return zero.
*/
EXP_API int read_hash_attr(link_t_ptr ptr, const tchar_t* key, int keylen, tchar_t* buf, int max);

/*
@FUNCTION get_hash_attr_ptr: get hash value string pointer from hash table by key.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@RETURN const tchar_t*: return the hash entity value string pointer if exists, otherwise return NULL.
*/
EXP_API const tchar_t* get_hash_attr_ptr(link_t_ptr ptr, const tchar_t* key, int keylen);

/*
@FUNCTION get_hash_attr_len: get hash value token length from hash table by key.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@RETURN int: return the hash entity value token length in characters if exists, otherwise return zero.
*/
EXP_API int get_hash_attr_len(link_t_ptr ptr, const tchar_t* key, int keylen);

/*
@FUNCTION set_hash_attr_boolean: set hash entity a boolean value.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT bool_t b: the boolean value.
@RETURN link_t_ptr: the hash entity link componet.
*/
EXP_API link_t_ptr set_hash_attr_boolean(link_t_ptr ptr, const tchar_t* key, bool_t b);

/*
@FUNCTION get_hash_attr_boolean: get hash entity boolean value.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@RETURN bool_t: return the hash entity boolean value if exists, otherwise return zero.
*/
EXP_API bool_t get_hash_attr_boolean(link_t_ptr ptr, const tchar_t* key);

/*
@FUNCTION set_hash_attr_integer: set hash entity a integer value.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int ul: the integer value.
@RETURN link_t_ptr: the hash entity link componet.
*/
EXP_API link_t_ptr set_hash_attr_integer(link_t_ptr ptr, const tchar_t* key, int ul);

/*
@FUNCTION get_hash_attr_integer: get hash entity integer value.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@RETURN int: return the hash entity integer value if exists, otherwise return zero.
*/
EXP_API int get_hash_attr_integer(link_t_ptr ptr, const tchar_t* key);

/*
@FUNCTION set_hash_attr_float: set hash entity a float value.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT float f: the float value.
@RETURN link_t_ptr: the hash entity link componet.
*/
EXP_API link_t_ptr set_hash_attr_float(link_t_ptr ptr, const tchar_t* key, float f);

/*
@FUNCTION get_hash_attr_float: get hash entity float value.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@RETURN float: return the hash entity float value if exists, otherwise return zero.
*/
EXP_API float get_hash_attr_float(link_t_ptr ptr, const tchar_t* key);

/*
@FUNCTION set_hash_attr_numeric: set hash entity a double value.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT double db: the double value.
@RETURN link_t_ptr: the hash entity link componet.
*/
EXP_API link_t_ptr set_hash_attr_numeric(link_t_ptr ptr, const tchar_t* key, double db);

/*
@FUNCTION get_hash_attr_numeric: get hash entity double value.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@RETURN double: return the hash entity double value if exists, otherwise return zero.
*/
EXP_API double get_hash_attr_numeric(link_t_ptr ptr, const tchar_t* key);

/*
@FUNCTION write_hash_bytes: write a hash key and bytes value into hash table,
if the key exists in hash table, the entity will be overwrited, else a new entity will be inserted.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@INPUT const byte_t* val: the bytes value buffer.
@INPUT dword_t vallen: the buffer size in bytes.
@RETURN link_t_ptr: the hash entity link componet.
*/
EXP_API link_t_ptr write_hash_bytes(link_t_ptr ptr, const tchar_t* key, int keylen, const byte_t* val, dword_t vallen);

/*
@FUNCTION read_hash_bytes: read hash bytes value from hash table by key.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@OUTPUT byte_t* buf: the bytes buffer for returning value.
@INPUT int max: the buffer size in bytes.
@RETURN dword_t: return the hash entity value size in bytes if exists, otherwise return zero.
*/
EXP_API dword_t read_hash_bytes(link_t_ptr ptr, const tchar_t* key, int keylen, byte_t* buf, dword_t max);

/*
@FUNCTION get_hash_bytes_ptr: get hash value bytes buffer pointer from hash table by key.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@RETURN const byte_t*: return the hash entity value bytes buffer pointer if exists, otherwise return NULL.
*/
EXP_API const byte_t* get_hash_bytes_ptr(link_t_ptr ptr, const tchar_t* key, int keylen);

/*
@FUNCTION get_hash_bytes_size: get hash value bytes size from hash table by key.
@INPUT link_t_ptr ptr: the hash table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key string token length in characters.
@RETURN dword_t: return the hash entity value size in bytes if exists, otherwise return zero.
*/
EXP_API dword_t get_hash_bytes_size(link_t_ptr ptr, const tchar_t* key, int keylen);

/*
@FUNCTION get_hash_entity_val_ptr: get a hash entity value bytes buffer pointer.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN const byte_t*: return the hash entity value bytes buffer pointer.
*/
EXP_API const byte_t* get_hash_entity_bytes_ptr(link_t_ptr elk);

/*
@FUNCTION get_hash_entity_bytes_size: get a hash entity value size in bytes.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN dword_t: return the hash entity size in bytes.
*/
EXP_API dword_t get_hash_entity_bytes_size(link_t_ptr elk);

/*
@FUNCTION attach_hash_entity_bytes: attach bytes buffer to hash entity.
@INPUT link_t_ptr elk: the hash entity link component.
@INPUT byte_t* val: the bytes buffer.
@INPUT dword_t len: the buffer value size in bytes.
@RETURN void: none.
*/
EXP_API void attach_hash_entity_bytes(link_t_ptr elk, byte_t* val, dword_t len);

/*
@FUNCTION detach_hash_entity_bytes: detach bytes buffer from hash entity.
@INPUT link_t_ptr elk: the hash entity link component.
@RETURN byte_t*: return the bytes buffer if exists, otherwise return NULL.
*/
EXP_API byte_t* detach_hash_entity_bytes(link_t_ptr elk);

/*
@FUNCTION hash_attr_set_chars: replace characters in hash entity string value at position.
@INPUT link_t_ptr elk: the hash entity link component.
@INPUT const tchar_t* key: the key string token used to find entity.
@INPUT int pos: the zero based position for string buffer replacing.
@INPUT const tchar_t* pch: the character array.
@INPUT int n: the count of characters.
@RETURN const tchar_t*: return hash entity string token if exists, otherwise return NULL.
*/
EXP_API const tchar_t* hash_attr_set_chars(link_t_ptr elk, const tchar_t* key, int pos, const tchar_t* pch, int n);

/*
@FUNCTION hash_attr_ins_chars: insert characters into hash entity string value at position.
@INPUT link_t_ptr elk: the hash entity link component.
@INPUT const tchar_t* key: the key string token used to find entity.
@INPUT int pos: the zero based position for string buffer inserting.
@INPUT const tchar_t* pch: the character array.
@INPUT int n: the count of characters.
@RETURN const tchar_t*: return hash entity string token if exists, otherwise return NULL.
*/
EXP_API const tchar_t* hash_attr_ins_chars(link_t_ptr elk, const tchar_t* key, int pos, const tchar_t* pch, int n);

/*
@FUNCTION hash_attr_del_chars: delete some characters from hash entity string value at position.
@INPUT link_t_ptr elk: the hash entity link component.
@INPUT const tchar_t* key: the key string token used to find entity.
@INPUT int pos: the zero based position for string buffer inserting.
@INPUT int n: the count of characters.
@RETURN const tchar_t*: return hash entity string token if exists, otherwise return NULL.
*/
EXP_API const tchar_t* hash_attr_del_chars(link_t_ptr elk, const tchar_t* key, int pos, int n);

/*
@FUNCTION hash_table_parse_attrset: pare hash table from attributes set.
@INPUT link_t_ptr ptr: the hash entity link component.
@INPUT const tchar_t* attrset: the attributes set. it like "key1=val1;key2=val2;...".
@INPUT int n: the length of attributes token in characters.
@RETURN void: none.
*/
EXP_API void hash_table_parse_attrset(link_t_ptr ptr, const tchar_t* attrset, int len);

/*
@FUNCTION hash_table_format_attrset: format hash table to attributes set.
@INPUT link_t_ptr ptr: the hash entity link component.
@OUTPUT tchar_t* buf: the attributes set buffer, the output string token like: "key1=val1;key2=val2;...".
@INPUT int max: the size of attributes set buffer in characters, not include terminate character.
@RETURN int: return the length of formated token.
*/
EXP_API int hash_table_format_attrset(link_t_ptr ptr, tchar_t* buf, int max);

EXP_API int hash_table_compare_attr(const tchar_t* key, const tchar_t* sign, const tchar_t* val, void* param);

/*
@FUNCTION hash_table_parse_options: pare hash table from options set.
@INPUT link_t_ptr ptr: the hash entity link component.
@INPUT const tchar_t* str: the options string token. it like "key1~val1;key2~val2;...".
@INPUT int n: the length of options token in characters.
@INPUT tchar_t itemfeed: the item seperator character. it like "~".
@INPUT tchar_t linefeed: the line seperator character. it like ";".
@RETURN int: return count of item parsed.
*/
EXP_API int hash_table_parse_options(link_t_ptr ptr, const tchar_t* str, int len, tchar_t itemfeed, tchar_t linefeed);

/*
@FUNCTION hash_table_format_options: format hash table to options set.
@INPUT link_t_ptr ptr: the hash entity link component.
@INPUT tchar_t* buf: the options string buffer, the output token will like "key1~val1;key2~val2;...".
@INPUT int max: the size of options buffer in characters.
@INPUT tchar_t itemfeed: the item seperator character. it like "~".
@INPUT tchar_t linefeed: the line seperator character. it like ";".
@RETURN int: return length of string formated in characters.
*/
EXP_API int hash_table_format_options(link_t_ptr ptr, tchar_t* buf, int max, tchar_t itemfeed, tchar_t linefeed);

#if defined(XDL_SUPPORT_TEST)
	EXP_API void test_hash_table();
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_HASHTABLE_H*/
