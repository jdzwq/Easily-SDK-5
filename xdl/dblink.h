/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc double link list document

	@module	dblink.h | double link list interface file

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

#ifndef _DBLINK_H
#define _DBLINK_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void init_root_link(link_t_ptr root);

XDL_API bool_t is_empty_link(link_t_ptr root);

XDL_API bool_t is_root_link(link_t_ptr plk);

XDL_API bool_t is_first_link(link_t_ptr plk);

XDL_API bool_t is_last_link(link_t_ptr plk);

XDL_API bool_t is_child_link(link_t_ptr root, link_t_ptr plk);

XDL_API void insert_link_before(link_t_ptr root,link_t_ptr pos,link_t_ptr pnew);

XDL_API void insert_link_after(link_t_ptr root,link_t_ptr pos,link_t_ptr pnew);

XDL_API link_t_ptr delete_link(link_t_ptr root, link_t_ptr plk);

XDL_API int get_link_count(link_t_ptr root);

XDL_API link_t_ptr get_link_at(link_t_ptr root,int index);

XDL_API void insert_link_at(link_t_ptr root, int index, link_t_ptr pnew);

XDL_API link_t_ptr delete_link_at(link_t_ptr root, int index);

XDL_API int get_link_index(link_t_ptr root,link_t_ptr plk);

XDL_API link_t_ptr get_first_link(link_t_ptr root);

XDL_API link_t_ptr get_next_link(link_t_ptr plk);

XDL_API link_t_ptr get_prev_link(link_t_ptr plk);

XDL_API link_t_ptr get_last_link(link_t_ptr root);

XDL_API link_t_ptr get_root_link(link_t_ptr plk);

XDL_API void move_link(link_t_ptr root, link_t_ptr root_src);

XDL_API void push_link(link_t_ptr root,link_t_ptr ptr);

XDL_API link_t_ptr pop_link(link_t_ptr root);

XDL_API link_t_ptr peek_link(link_t_ptr root);

XDL_API void switch_link(link_t_ptr root,link_t_ptr plk1,link_t_ptr plk2);

XDL_API void switch_link_next(link_t_ptr root, link_t_ptr plk);

XDL_API void switch_link_prev(link_t_ptr root, link_t_ptr plk);

XDL_API void switch_link_first(link_t_ptr root, link_t_ptr plk);

XDL_API void switch_link_last(link_t_ptr root, link_t_ptr plk);

XDL_API void switch_link_before(link_t_ptr root, link_t_ptr next, link_t_ptr plk);

XDL_API void switch_link_after(link_t_ptr root, link_t_ptr prev, link_t_ptr plk);

XDL_API void bubble_sort_link(link_t_ptr root,CALLBACK_SORTLINK pf,int desc,void* parm);

XDL_API void merge_sort_link(link_t_ptr root, CALLBACK_SORTLINK pf, int desc, void* parm);

XDL_API void heap_sort_link(link_t_ptr root, CALLBACK_SORTLINK pf, int desc, void* parm);

XDL_API void quick_sort_link(link_t_ptr root, CALLBACK_SORTLINK pf, int desc, void* parm);

XDL_API void enum_link(link_t_ptr root,CALLBACK_ENUMLINK pf,void* parm);

#ifdef	__cplusplus
}
#endif

#endif /*_DBLINK_H*/


