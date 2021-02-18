/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc locktable document

	@module	locktable.h | interface lock

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

#ifndef _LOCKTABLE_H
#define _LOCKTABLE_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_lock_table: create a lock table.
@INPUT const tchar_t* guid: the lock global id.
@INPUT int map_nums: the single map items count.
@RETURN link_t_ptr: return the lock table link component.
*/
EXP_API link_t_ptr create_lock_table(const tchar_t* guid, int map_nums);

/*
@FUNCTION destroy_lock_table: destroy a lock table.
@INPUT link_t_ptr pt: the lock table link component.
@RETURN void: none.
*/
EXP_API void destroy_lock_table(link_t_ptr pt);

/*
@FUNCTION enter_lock_table: enter the lock table.
@INPUT link_t_ptr pt: the lock table link component.
@INPUT int map_ind: the zero based map index.
@INPUT int map_pos: the zero based map item index.
@RETURN bool_t: if succeeded return none zero.
*/
EXP_API bool_t enter_lock_table(link_t_ptr pt, int map_ind, int map_pos);

/*
@FUNCTION leave_lock_table: leave the lock table.
@INPUT link_t_ptr pt: the lock table link component.
@INPUT int map_ind: the zero based map index.
@INPUT int map_pos: the zero based map item index.
@RETURN bool_t: if succeeded return none zero.
*/
EXP_API void leave_lock_table(link_t_ptr pt, int map_ind, int map_pos);

#if defined(XDL_SUPPORT_TEST)
EXP_API void test_lock_table(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_LOCKTABLE_H*/