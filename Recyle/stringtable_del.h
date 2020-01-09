/*************************************************************
	Easily xdl v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@doc stringtable document

	@module	stringtable.h | string table interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

#ifndef _STRTABLE_H
#define _STRTABLE_H

#include "xdllink.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_string_table(void);

XDL_API void destroy_string_table(link_t_ptr ptr);

XDL_API void clear_string_table(link_t_ptr ptr);

XDL_API void sort_string_table(link_t_ptr ptr,int desc);

XDL_API int get_string_entity_count(link_t_ptr ptr);

XDL_API link_t_ptr write_string_entity_attr(link_t_ptr ptr,const wchar_t* key,int keylen,const wchar_t* val,int vallen);

XDL_API int read_string_entity_attr(link_t_ptr ptr,const wchar_t* key,int keylen,wchar_t* buf,int max);

XDL_API const wchar_t* get_string_entity_attr_ptr(link_t_ptr ptr,const wchar_t* key,int keylen);

XDL_API link_t_ptr get_string_entity(link_t_ptr ptr,const wchar_t* key,int keylen);

XDL_API void delete_string_entity(link_t_ptr ptr,link_t_ptr tlk);

XDL_API const wchar_t* get_string_entity_key_ptr(link_t_ptr tlk);

XDL_API const wchar_t* get_string_entity_val_ptr(link_t_ptr tlk);

XDL_API int get_string_entity_val_len(link_t_ptr tlk);

XDL_API int get_string_entity_key(link_t_ptr tlk,wchar_t* buf,int max);

XDL_API int get_string_entity_val(link_t_ptr tlk,wchar_t* buf,int max);

XDL_API void set_string_entity_val(link_t_ptr tlk,const wchar_t* val,int vallen);

XDL_API void set_string_entity_data(link_t_ptr tlk,unsigned long data);

XDL_API unsigned long get_string_entity_data(link_t_ptr tlk);

XDL_API link_t_ptr get_string_next_entity(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_string_prev_entity(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_string_entity_at(link_t_ptr ptr, int index);

XDL_API int get_string_entity_index(link_t_ptr ptr, link_t_ptr tlk);

XDL_API void string_table_parse_options(link_t_ptr ptr,const wchar_t* options,int len,wchar_t itemfeed,wchar_t linefeed);

XDL_API int string_table_format_options(link_t_ptr ptr,wchar_t* buf,int max,wchar_t itemfeed,wchar_t linefeed);

XDL_API int string_table_format_options_length(link_t_ptr ptr,wchar_t itemfeed,wchar_t linefeed);

XDL_API void string_table_parse_attrset(link_t_ptr ptr,const wchar_t* attrset,int len);

XDL_API int string_table_format_attrset(link_t_ptr ptr,wchar_t* buf,int max);

XDL_API int string_table_format_attrset_length(link_t_ptr ptr);

#ifdef __cplusplus
}
#endif

#endif /*_STRTABLE_H*/
