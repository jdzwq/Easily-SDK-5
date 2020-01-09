/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc stringarray document

	@module	stringarray.h | stringarray interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#ifndef _STRARRAY_H
#define _STRARRAY_H

#include "xdldef.h"

XDL_API link_t_ptr create_string_array(void);

XDL_API void destroy_string_array(link_t_ptr ptr);

XDL_API void clear_string_array(link_t_ptr ptr);

XDL_API void sort_string_array(link_t_ptr ptr,int desc);

XDL_API bool_t is_string_array(link_t_ptr ptr);

XDL_API bool_t is_string_item(link_t_ptr ptr, link_t_ptr ilk);

XDL_API int get_string_item_count(link_t_ptr ptr);

XDL_API link_t_ptr insert_string_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API void delete_string_item(link_t_ptr ptr, link_t_ptr pos);

XDL_API const tchar_t* get_string_item_text_ptr(link_t_ptr ilk);

XDL_API void set_string_item_text(link_t_ptr ilk,const tchar_t* val,int len);

XDL_API void set_string_item_delta(link_t_ptr ilk, void* data);

XDL_API void* get_string_item_delta(link_t_ptr ilk);

XDL_API link_t_ptr get_string_next_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_string_prev_item(link_t_ptr ptr,link_t_ptr pos);

XDL_API link_t_ptr get_string_item_at(link_t_ptr ptr, int index);

XDL_API int get_string_item_index(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void string_array_parse_tokens(link_t_ptr ptr,const tchar_t* tokens,int len,tchar_t feed);

XDL_API int string_array_format_tokens(link_t_ptr ptr,tchar_t* buf,int max,tchar_t feed);

#endif //_STRTABLE_H
