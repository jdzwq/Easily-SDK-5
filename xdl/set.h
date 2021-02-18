/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl set document

	@module	set.h | interface file

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

#ifndef _SET_H
#define _SET_H

#include "xdldef.h"

typedef enum{
	_SET_SET = 0,
	_SET_ELE = 1
}SET_TYPE;

typedef struct _set_t* set_t_ptr;

typedef struct _set_t{
	int type;
	int size;
	union{
		double data;
		set_t_ptr pset;
	};
}set_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION set_alloc: alloc a set.
@RETURN set_t*: return the set struct
*/
EXP_API set_t* set_alloc(void);

/*
@FUNCTION set_free: free a set.
@INPUT set_t* pset: the set struct
@RETURN void: none.
*/
EXP_API void set_free(set_t* pset);

/*
@FUNCTION set_copy: copy source set elements to destination.
@OUTPUT set_t* pdst: the destination set struct.
@INPUT const set_t* psrc: the source set struct.
@RETURN void: none.
*/
EXP_API void set_copy(set_t* pdst, const set_t* psrc);

/*
@FUNCTION set_comp: compare two set.
@INPUT const set_t* p1: the set struct.
@INPUT const set_t* p2: the set struct.
@RETURN int: if p1 equla to p2 return zero, else if p1 less than p2 return -1, else return 1.
*/
EXP_API int set_comp(const set_t* p1, const set_t* p2);

/*
@FUNCTION set_reset: empty the set elements.
@OUTPUT set_t* pset: the set struct.
@RETURN void: none.
*/
EXP_API void set_reset(set_t* pset);

/*
@FUNCTION set_add: append set to a set as its sub element.
@INOUTPUT set_t* pset: the set struct.
@INPUT const set_t* psub: the sub set struct.
@RETURN void: none.
*/
EXP_API void set_add(set_t* pset, const set_t* psub);

/*
@FUNCTION set_del: delete sub set from a set.
@INOUTPUT set_t* pset: the set struct.
@INPUT const set_t* psub: the sub set struct.
@RETURN void: none.
*/
EXP_API void set_del(set_t* pset, const set_t* psub);

/*
@FUNCTION set_in: test a sub set is in the set.
@INPUT set_t* pset: the set struct.
@INPUT const set_t* psub: the sub set struct.
@RETURN bool_t: return nonzero if exists, otherwise return zero.
*/
EXP_API bool_t set_in(set_t* pset, const set_t* psub);

/*
@FUNCTION set_get: get a sub from the set by position.
@INPUT set_t* pset: the set struct.
@INPUT int index: the zero based position.
@OUTPUT set_t* psub: the sub set struct.
@RETURN bool_t: return nonzero if exists, otherwise return zero.
*/
EXP_API void set_get(set_t* pset, int index, set_t* psub);

/*
@FUNCTION set_parse: parse set element value from string.
@INOUTPUT set_t* pset: the set struct.
@INPUT const tchar_t* str: string token, eg: "{a, b, {c, d}}".
@INPUT int len: length of string token.
@RETURN void: none.
*/
EXP_API void set_parse(set_t* pset, const tchar_t* token, int len);

/*
@FUNCTION set_format: format set element to string.
@INPUT const set_t* pset: the set struct.
@OUTPUT tchar_t* buf: buffer for formating, eg: "{1, 2, {3, 4}}".
@INPUT int max: the buffer size in characters, not include terminate character.
@RETURN int: return the formated string token length.
*/
EXP_API int set_format(const set_t* pset, tchar_t* buf, int max);

#if defined(XDL_SUPPORT_TEST)
	EXP_API void test_set(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*SET_H*/