/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc Aho-Corasick automaton document

	@module	actable.h | interface file

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

#ifndef _ACTABLE_H
#define _ACTABLE_H

#include "xdldef.h"

typedef bool_t(*PF_ENUM_AC_TABLE)(const tchar_t* key, int len, vword_t delta, void* p);

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_ac_table: create a ac table.
@RETURN link_t_ptr: return the ac table link component.
*/
EXP_API link_t_ptr create_ac_table(void);

/*
@FUNCTION destroy_ac_table: destroy a ac table.
@INPUT link_t_ptr ptr: the ac table link component.
@RETURN void: none.
*/
EXP_API void destroy_ac_table(link_t_ptr ptr);

/*
@FUNCTION insert_ac_table: insert a key and value into ac table.
@INPUT link_t_ptr ptr: the ac table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int len: the key string token length in characters.
@INPUT vword_t val: the int val.
@RETURN void: none.
*/
EXP_API void insert_ac_table(link_t_ptr ptr, const tchar_t* key, int len, vword_t val);

/*
@FUNCTION build_ac_table: build failure transfer table of the ac table .
@INPUT link_t_ptr ptr: the ac table link component.
@RETURN void: none.
*/
EXP_API void build_ac_table(link_t_ptr ptr);

/*
@FUNCTION find_ac_table: find and return data in ac table by the key.
@INPUT link_t_ptr ptr: the ac table link component.
@INPUT const tchar_t* key: the key string token.
@INPUT int len: the key string token length in characters.
@RETURN vword_t: return the data if finded, otherwise return zero.
*/
EXP_API vword_t find_ac_table(link_t_ptr ptr, const tchar_t* key, int len);

/*
@FUNCTION enum_ac_table: enum ac table key and value.
@INPUT link_t_ptr ptr: the ac table link component.
@INPUT PF_ENUM_AC_TABLE pf: the enum callback function.
@INPUT void* pa: the parameter tanslate to callback function.
@RETURN void: none.
*/
EXP_API void enum_ac_table(link_t_ptr ptr, PF_ENUM_AC_TABLE pf, void* pa);

#if defined(XDL_SUPPORT_TEST)
	EXP_API void test_ac_table();
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_ACTABLE_H*/
