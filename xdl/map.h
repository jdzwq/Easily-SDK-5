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

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION map_alloc: alloc a map.
@INPUT int nums: count of number.
@INPUT int bits: the flag bits.
@RETURN map_t: return map object.
*/
EXP_API map_t map_alloc(int nums, int bits);

/*
@FUNCTION map_free: free a map.
@INPUT map_t map: the map object.
@RETURN void: none.
*/
EXP_API void map_free(map_t map);

/*
@FUNCTION map_clone: clone a map from souce.
@INPUT const map_t: the source map struct.
@RETURN map_t: return map struct.
*/
EXP_API map_t map_clone(map_t map);

/*
@FUNCTION map_zero: set flag to zero in map.
@INPUT map_t map: the map object.
@RETURN void: none.
*/
EXP_API void map_zero(map_t map);

/*
@FUNCTION map_calc_size: calc map bytes needed.
@INPUT int nums: count of number.
@INPUT int bits: the flag bits.
@RETURN int: return the bytes needed.
*/
EXP_API int map_calc_size(int nums, int bits);

/*
@FUNCTION map_get_size: get map bytes.
@INPUT map_t map: the map object.
@RETURN int: size in bytes.
*/
EXP_API int map_size(map_t map);

/*
@FUNCTION map_get_bits: get map bits.
@INPUT map_t map: the map object.
@RETURN int: bits.
*/
EXP_API int map_bits(map_t map);

/*
@FUNCTION map_data: get map data buffer.
@INPUT map_t map: the map object.
@RETURN void*: the data buffer.
*/
EXP_API void* map_data(map_t map);

/*
@FUNCTION map_attach: attach map data buffer.
@INPUT map_t map: the map object.
@INPUT void* data: the data buffer.
@RETURN void*: the data buffer.
*/
EXP_API void map_attach(map_t map, void* data);

/*
@FUNCTION map_detach: detach map data buffer.
@INPUT map_t map: the map object.
@RETURN void*: the data buffer.
*/
EXP_API void* map_detach(map_t map);

/*
@FUNCTION map_set_bits: set flag.
@INPUT map_t map: the map struct.
@INPUT int i: the index of number.
@INPUT byte_t tag: the flag value.
@RETURN void: none.
*/
EXP_API void map_set_bit(map_t map, int i, byte_t tag);

/*
@FUNCTION map_get_bit: get flag.
@INPUT map_t map: the map struct.
@INPUT int i: the index of number.
@RETURN byte_t: return the flag value if exists, otherwise return zero.
*/
EXP_API byte_t map_get_bit(map_t map, int i);

/*
@FUNCTION map_find_bit: find the flag in map.
@INPUT map_t map: the map struct.
@INPUT int i: the start position.
@INPUT byte tag: the tag value to seek.
@RETURN int: return the index if exists, otherwise return C_ERR(-1).
*/
EXP_API int map_find_bit(map_t map, int i, byte_t tag);

/*
@FUNCTION map_test_bit: find the flag not setted in map.
@INPUT map_t map: the map struct.
@INPUT int i: the start position.
@INPUT byte_t tag: the flag value.
@INPUT int n: the find step.
@RETURN int: return the index if exists, otherwise return C_ERR(-1).
*/
EXP_API int map_test_bit(map_t map, int i, byte_t tag, int n);

/*
@FUNCTION map_parse: parse map element value from string.
@INPUT map_t map: the map struct.
@INPUT const tchar_t* str: string token, number separated by space.
@INPUT int len: length of string token.
@RETURN void: none.
*/
EXP_API void map_parse(map_t map, const tchar_t* str, int len);

/*
@FUNCTION map_formap: formap map element to string.
@INPUT map_t map: the map struct.
@OUTPUT tchar_t* buf: buffer for formaping.
@INPUT int max: the buffer size in characters, not include terminate character.
@RETURN int: return the formaped string token length.
*/
EXP_API int map_formap(map_t map, tchar_t* buf, int max);

/*
@FUNCTION map_encode: encode map object to bytes buffer.
@INPUT map map: the map object.
@INPUT int encode: the encoding type eg: _UTF8, _GB2312, _UTF16_LIT, _UTF16_BIG.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return encoded bytes.
*/
EXP_API dword_t map_encode(map_t map, int encode, byte_t* buf, dword_t max);

/*
@FUNCTION map_decode: decode map object from bytes buffer.
@INPUT map map: the map object.
@INPUT int encode: the encoding type eg: _UTF8, _GB2312, _UTF16_LIT, _UTF16_BIG.
@INPUT const byte_t* buf: the data buffer.
@INPUT dword_t n: the data size in bytes.
*/
EXP_API void map_decode(map_t map, int encode, const byte_t* buf, dword_t n);

#if defined(XDL_SUPPORT_TEST)
EXP_API void test_map(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_MAP_H*/