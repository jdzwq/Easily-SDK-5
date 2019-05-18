/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc stringtable document

	@module	stringtable.h | stringtable interface file

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

#ifndef _STRTABLE_H
#define _STRTABLE_H

#include "xdldef.h"

/**********************************************************************************************/
#define set_string_entity_dirty(elk,c)		set_string_entity_mask(elk,(get_string_entity_mask(elk) & 0xFFFFFFF0) | c)

#define get_string_entity_dirty(elk)		(get_string_entity_mask(elk) & 0x0000000F)
/**********************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_string_table: create a string table.
@INPUT int order: sort rule 1: ascend,-1: descend, 0: none.
@RETURN link_t_ptr: return the string table link component.
*/
XDL_API link_t_ptr create_string_table(int order);

/*
@FUNCTION destroy_string_table: destroy a string table.
@INPUT link_t_ptr ptr: the string table link component.
@RETURN void: none.
*/
XDL_API void destroy_string_table(link_t_ptr ptr);

/*
@FUNCTION clear_string_table: delete all string entities.
@INPUT link_t_ptr ptr: the string table link component.
@RETURN void: none.
*/
XDL_API void clear_string_table(link_t_ptr ptr);

/*
@FUNCTION is_string_table: test is string table.
@INPUT link_t_ptr ptr: the string table link component.
@RETURN bool_t: return nonzero for being a string table, otherwise return zero.
*/
XDL_API bool_t is_string_table(link_t_ptr ptr);

/*
@FUNCTION is_string_entity: test is string table child entity.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT link_t_ptr ilk: the string entity link component.
@RETURN bool_t: return nonzero for being a string entity.
*/
XDL_API bool_t is_string_entity(link_t_ptr ptr, link_t_ptr ilk);

/*
@FUNCTION get_string_entity_count: get string table entities.
@INPUT link_t_ptr ptr: the string table link component.
@RETURN int: return the count of string entities.
*/
XDL_API int get_string_entity_count(link_t_ptr ptr);

/*
@FUNCTION write_string_entity: add string entity into string table, if the key exists in string table, the value will be replaced.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT const tchar_t* key: the key stirng token.
@INPUT int keylen: the key token length in characters.
@INPUT const tchar_t* val: the value stirng token.
@INPUT int vallen: the value token length in characters.
@RETURN link_t_ptr: return string entity link component.
*/
XDL_API link_t_ptr write_string_entity(link_t_ptr ptr, const tchar_t* key, int keylen, const tchar_t* val, int vallen);

/*
@FUNCTION read_string_entity: read string entity value from string table by key.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT const tchar_t* key: the key stirng token.
@INPUT int keylen: the key token length in characters.
@OUTPUT tchar_t* buf: the stirng buffer for returning value.
@INPUT int max: the value string buffer size in characters.
@RETURN int: return entity value length in characters.
*/
XDL_API int read_string_entity(link_t_ptr ptr, const tchar_t* key, int keylen, tchar_t* buf, int max);

/*
@FUNCTION get_string_entity_ptr: get string entity value string pointer.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT const tchar_t* key: the key stirng token.
@INPUT int keylen: the key token length in characters.
@RETURN const tchar_t*: return entity value string pointer if exists, otherwise return NULL.
*/
XDL_API const tchar_t* get_string_entity_ptr(link_t_ptr ptr, const tchar_t* key, int keylen);

/*
@FUNCTION get_string_entity: find the string entity by key.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT const tchar_t* key: the key stirng token.
@INPUT int keylen: the key token length in characters.
@RETURN link_t_ptr: return string entity link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_string_entity(link_t_ptr ptr, const tchar_t* key, int keylen);

/*
@FUNCTION insert_string_entity: insert the string entity after the position.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT link_t_ptr pos: the position link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return new string entity link component.
*/
XDL_API link_t_ptr insert_string_entity(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION delete_string_entity: delete the string entity.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT link_t_ptr pos: the link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN void: none.
*/
XDL_API void delete_string_entity(link_t_ptr ptr, link_t_ptr pos);

/*
@FUNCTION get_string_entity_key_ptr: get the string entity key string pointer.
@INPUT link_t_ptr elk: the string entity link component.
@RETURN const tchar_t*: the key string pointer.
*/
XDL_API const tchar_t* get_string_entity_key_ptr(link_t_ptr elk);

/*
@FUNCTION get_string_entity_key: copy the string entity key.
@INPUT link_t_ptr elk: the string entity link component.
@OUTPUT tchar_t* buf: the string buffer for returning key token.
@INPUT int max: the string buffer size in characters.
@RETURN int: return characters copyed.
*/
XDL_API int get_string_entity_key(link_t_ptr elk, tchar_t* buf, int max);

/*
@FUNCTION set_string_entity_key: set the string entity key.
@INPUT link_t_ptr elk: the string entity link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int keylen: the key token length in characters.
@RETURN void: none.
*/
XDL_API void set_string_entity_key(link_t_ptr elk, const tchar_t* key, int keylen);

/*
@FUNCTION get_string_entity_val_ptr: get the string entity value string pointer.
@INPUT link_t_ptr elk: the string entity link component.
@RETURN const tchar_t*: the value string pointer.
*/
XDL_API const tchar_t* get_string_entity_val_ptr(link_t_ptr elk);

/*
@FUNCTION get_string_entity_val: copy the string entity value.
@INPUT link_t_ptr elk: the string entity link component.
@OUTPUT tchar_t* buf: the string buffer for returning value token.
@INPUT int max: the string buffer size in characters.
@RETURN int: return characters copyed.
*/
XDL_API int get_string_entity_val(link_t_ptr elk, tchar_t* buf, int max);

/*
@FUNCTION set_string_entity_val: set the string entity value.
@INPUT link_t_ptr elk: the string entity link component.
@INPUT const tchar_t* val: the value string token.
@INPUT int keylen: the value token length in characters.
@RETURN void: none.
*/
XDL_API void set_string_entity_val(link_t_ptr elk,const tchar_t* val,int vallen);

/*
@FUNCTION set_string_entity_delta: set the string entity extract data.
@INPUT link_t_ptr elk: the string entity link component.
@INPUT var_long data: the extract data.
@RETURN void: none.
*/
XDL_API void set_string_entity_delta(link_t_ptr elk, var_long data);

/*
@FUNCTION get_string_entity_delta: get the string entity extract data.
@INPUT link_t_ptr elk: the string entity link component.
@RETURN var_long: return the extract data.
*/
XDL_API var_long get_string_entity_delta(link_t_ptr elk);

/*
@FUNCTION set_string_entity_mask: set the string entity mask.
@INPUT link_t_ptr elk: the string entity link component.
@INPUT dword_t mask: the mask value.
@RETURN void: none.
*/
XDL_API void set_string_entity_mask(link_t_ptr elk, dword_t mask);

/*
@FUNCTION get_string_entity_mask: get the string entity mask.
@INPUT link_t_ptr elk: the string entity link component.
@RETURN dword_t: return the mask value.
*/
XDL_API dword_t get_string_entity_mask(link_t_ptr elk);

/*
@FUNCTION get_string_next_entity: get the next string entity.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT link_t_ptr pos: the entity link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the entity link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_string_next_entity(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_string_prev_entity: get the previous string entity.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT link_t_ptr pos: the entity link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: return the entity link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_string_prev_entity(link_t_ptr ptr,link_t_ptr pos);

/*
@FUNCTION get_string_entity_at: get the previous string entity at position.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT int index: the zero based position.
@RETURN link_t_ptr: return the entity link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_string_entity_at(link_t_ptr ptr, int index);

/*
@FUNCTION get_string_entity_index: calc the string entity position.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT link_t_ptr elk: the string entity link component.
@RETURN int: return zero based position.
*/
XDL_API int get_string_entity_index(link_t_ptr ptr, link_t_ptr elk);

/*
@FUNCTION get_string_updated_entity_count: get the dirty string entity count.
@INPUT link_t_ptr ptr: the string table link component.
@RETURN int: return count of entities.
*/
XDL_API int get_string_updated_entity_count(link_t_ptr ptr);

/*
@FUNCTION string_table_parse_attrset: parse string table from attributes set.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT const tchar_t* attrset: the attributes set string token.
@INPUT int len: the length of attribytes string token.
@RETURN void: none.
*/
XDL_API void string_table_parse_attrset(link_t_ptr ptr,const tchar_t* attrset,int len);

/*
@FUNCTION string_table_format_attrset: format string table to attributes set.
@INPUT link_t_ptr ptr: the string table link component.
@OUTPUT tchar_t* buf: the string buffer for returning attributes set token.
@INPUT int max: the string buffer size in characters.
@RETURN int: return the characters formated.
*/
XDL_API int string_table_format_attrset(link_t_ptr ptr,tchar_t* buf,int max);

/*
@FUNCTION string_table_format_key_tokens: format string entities key to set.
@INPUT link_t_ptr ptr: the string table link component.
@OUTPUT tchar_t* buf: the string buffer for returning keys set token.
@INPUT int max: the string buffer size in characters.
@INPUT tchar_t feed: the seperator character.
@RETURN int: return the characters formated.
*/
XDL_API int string_table_format_key_tokens(link_t_ptr ptr, tchar_t* buf, int max, tchar_t feed);

/*
@FUNCTION string_table_format_val_tokens: format string entities value to set.
@INPUT link_t_ptr ptr: the string table link component.
@OUTPUT tchar_t* buf: the string buffer for returning values set token.
@INPUT int max: the string buffer size in characters.
@INPUT tchar_t feed: the seperator character.
@RETURN int: return the characters formated.
*/
XDL_API int string_table_format_val_tokens(link_t_ptr ptr, tchar_t* buf, int max, tchar_t feed);

/*
@FUNCTION string_table_parse_options: parse string table entities from options set.
@INPUT link_t_ptr ptr: the string table link component.
@INPUT const tchar_t* str: the options set string token.
@INPUT int len: the length of options string token.
@INPUT tchar_t itemfeed: the item seperator character.
@INPUT tchar_t linefeed: the line seperator character.
@RETURN int: return count of entities parsed.
*/
XDL_API int string_table_parse_options(link_t_ptr ptr, const tchar_t* str, int len, tchar_t itemfeed, tchar_t linefeed);

/*
@FUNCTION string_table_format_options: format string table entities to options set.
@INPUT link_t_ptr ptr: the string table link component.
@OUTPUT tchar_t* buf: the string buffer for return options set.
@INPUT int max: the string buffer size in characters.
@INPUT tchar_t itemfeed: the item seperator character.
@INPUT tchar_t linefeed: the line seperator character.
@RETURN int: return characters formated.
*/
XDL_API int string_table_format_options(link_t_ptr ptr, tchar_t* buf, int max, tchar_t itemfeed, tchar_t linefeed);

#ifdef	__cplusplus
}
#endif

#endif //_STRTABLE_H
