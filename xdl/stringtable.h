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

XDL_API link_t_ptr create_string_table(void);

XDL_API void destroy_string_table(link_t_ptr ptr);

XDL_API void clear_string_table(link_t_ptr ptr);

XDL_API void sort_string_table(link_t_ptr ptr,int desc);

XDL_API bool_t is_string_table(link_t_ptr ptr);

XDL_API bool_t is_string_entity(link_t_ptr ptr, link_t_ptr ilk);

XDL_API int get_string_entity_count(link_t_ptr ptr);

XDL_API link_t_ptr write_string_entity(link_t_ptr ptr,const tchar_t* key,int keylen,const tchar_t* val,int vallen);

XDL_API int read_string_entity(link_t_ptr ptr,const tchar_t* key,int keylen,tchar_t* buf,int max);

XDL_API const tchar_t* get_string_entity_ptr(link_t_ptr ptr,const tchar_t* key,int keylen);

XDL_API link_t_ptr get_string_entity(link_t_ptr ptr,const tchar_t* key,int keylen);

XDL_API link_t_ptr insert_string_entity(link_t_ptr ptr, link_t_ptr pos);

XDL_API void delete_string_entity(link_t_ptr ptr, link_t_ptr pos);

XDL_API const tchar_t* get_string_entity_key_ptr(link_t_ptr elk);

XDL_API int get_string_entity_key(link_t_ptr elk, tchar_t* buf, int max);

XDL_API void set_string_entity_key(link_t_ptr elk, const tchar_t* key, int keylen);

XDL_API const tchar_t* get_string_entity_val_ptr(link_t_ptr elk);

XDL_API int get_string_entity_val(link_t_ptr elk,tchar_t* buf,int max);

XDL_API void set_string_entity_val(link_t_ptr elk,const tchar_t* val,int vallen);

XDL_API void set_string_entity_delta(link_t_ptr elk, var_long data);

XDL_API var_long get_string_entity_delta(link_t_ptr elk);

XDL_API void set_string_entity_mask(link_t_ptr elk, unsigned long mask);

XDL_API unsigned long get_string_entity_mask(link_t_ptr elk);

XDL_API link_t_ptr get_string_next_entity(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_string_prev_entity(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_string_entity_at(link_t_ptr ptr, int index);

XDL_API int get_string_entity_index(link_t_ptr ptr, link_t_ptr elk);

XDL_API int get_string_updated_entity_count(link_t_ptr ptr);

XDL_API void string_table_parse_attrset(link_t_ptr ptr,const tchar_t* attrset,int len);

XDL_API int string_table_format_attrset(link_t_ptr ptr,tchar_t* buf,int max);

XDL_API int string_table_format_key_tokens(link_t_ptr ptr, tchar_t* buf, int max, tchar_t feed);

XDL_API int string_table_format_val_tokens(link_t_ptr ptr, tchar_t* buf, int max, tchar_t feed);

XDL_API int string_table_parse_options(link_t_ptr ptr, const tchar_t* str, int len, tchar_t itemfeed, tchar_t linefeed);

XDL_API int string_table_format_options(link_t_ptr ptr, tchar_t* buf, int max, tchar_t itemfeed, tchar_t linefeed);

#ifdef	__cplusplus
}
#endif

#endif //_STRTABLE_H
