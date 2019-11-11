/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc map document

	@module	map.h | interface file

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

#ifndef _MAP_H
#define _MAP_H

#include "xdldef.h"

typedef struct _map_t{	
	int bits, rows, cols;
	dword_t* data;
}map_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION map_size: calc map bytes needed.
@INPUT int nums: count of number.
@INPUT int bits: the flag bits.
@RETURN dword_t: return the bytes needed.
*/
XDL_API dword_t map_size(int nums, int bits);

/*
@FUNCTION map_alloc: alloc a map.
@INPUT int nums: count of number.
@INPUT int bits: the flag bits.
@RETURN map_t*: return map struct.
*/
XDL_API map_t* map_alloc(int nums, int bits);

/*
@FUNCTION map_free: free a map.
@INPUT map_t* pmm: the map struct.
@RETURN void: none.
*/
XDL_API void map_free(map_t* pmm);

/*
@FUNCTION map_zero: set flag to zero in map.
@INPUT map_t* pmm: the map struct.
@RETURN void: none.
*/
XDL_API void map_zero(map_t* pmm);

/*
@FUNCTION map_set_bits: set flag.
@INPUT map_t* pmm: the map struct.
@INPUT int i: the index of number.
@INPUT byte_t b: the flag value.
@RETURN void: none.
*/
XDL_API void map_set_bits(map_t* pmm, int i, byte_t b);

/*
@FUNCTION map_get_bits: get flag.
@INPUT map_t* pmm: the map struct.
@INPUT int i: the index of number.
@RETURN byte_t: return the flag value if exists, otherwise return zero.
*/
XDL_API byte_t map_get_bits(map_t* pmm, int i);

/*
@FUNCTION map_find_bits: find the flag in map.
@INPUT map_t* pmm: the map struct.
@INPUT int i: the start position.
@RETURN int: return the index if exists, otherwise return C_ERR(-1).
*/
XDL_API int map_find_bits(map_t* pmm, int i, byte_t b);

/*
@FUNCTION map_test_bits: find the flag not setted in map.
@INPUT map_t* pmm: the map struct.
@INPUT int i: the start position.
@INPUT byte_t b: the flag value.
@INPUT int n: the find step.
@RETURN int: return the index if exists, otherwise return C_ERR(-1).
*/
XDL_API int map_test_bits(map_t* pmm, int i, byte_t b, int n);

#if defined(_DEBUG) || defined(DEBUG)
XDL_API void test_map(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_MAP_H*/