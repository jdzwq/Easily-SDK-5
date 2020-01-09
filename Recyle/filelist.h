/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc file list document

	@module	filelist.h | list document interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#ifndef _FILELIST_H
#define _FILELIST_H

#include "xdldef.h"

#define LIST_ITEM_FILE			_T("file")
#define LIST_ITEM_DIRECTORY		_T("directory")

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_file_list(void);

XDL_API void destroy_file_list(link_t_ptr ptr);

XDL_API void clear_file_list(link_t_ptr ptr);

XDL_API int get_file_item_count(link_t_ptr ilk);

XDL_API bool_t is_file_list(link_t_ptr ilk);

XDL_API link_t_ptr insert_file_item(link_t_ptr ilk, link_t_ptr pos);

XDL_API link_t_ptr insert_file_item_before(link_t_ptr ilk, link_t_ptr pos);

XDL_API link_t_ptr get_file_first_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_file_last_child_item(link_t_ptr ilk);

XDL_API link_t_ptr get_file_parent_item(link_t_ptr ilk);

XDL_API link_t_ptr get_file_next_sibling_item(link_t_ptr ilk);

XDL_API link_t_ptr get_file_prev_sibling_item(link_t_ptr ilk);

XDL_API int get_file_child_item_count(link_t_ptr ilk);

XDL_API link_t_ptr enum_file_item(link_t_ptr ptr, PF_LINKENUM pf, void* param);

XDL_API void delete_file_child_items(link_t_ptr ilk);

XDL_API void delete_file_item(link_t_ptr ilk);

XDL_API bool_t is_file_item(link_t_ptr ilk);

XDL_API bool_t is_directory_item(link_t_ptr ilk);

XDL_API void set_file_item_info(link_t_ptr ilk, const file_info_t* pif);

XDL_API void get_file_item_info(link_t_ptr ilk, file_info_t* pif);

XDL_API void set_file_item_mask(link_t_ptr ilk, dword_t mask);

XDL_API dword_t get_file_item_mask(link_t_ptr ilk);

XDL_API void set_file_item_mask_check(link_t_ptr ilk, dword_t ul, bool_t b);

XDL_API bool_t get_file_item_mask_check(link_t_ptr ilk, dword_t ul);

XDL_API int get_file_child_item_mask_check_count(link_t_ptr ilk, dword_t ul);

XDL_API int set_file_child_item_mask_check(link_t_ptr ilk, dword_t ul, bool_t b);

XDL_API void sort_file_child_item_by_name(link_t_ptr ilk, int desc);

XDL_API void sort_file_child_item_by_time(link_t_ptr ilk, int desc);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/
