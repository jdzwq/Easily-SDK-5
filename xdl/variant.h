/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc variant value document

	@module	variant.h | interface file

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

#ifndef _VARIANT_H
#define _VARIANT_H

#include "xdldef.h"

/*define variant type*/
#define VV_NULL			0x00
#define VV_BOOL			0x01
#define VV_BYTE			0x02
#define VV_SHORT		0x03
#define VV_INT			0x04
#define VV_LONG			0x05
#define VV_FLOAT		0x06
#define VV_DOUBLE		0x07
#define VV_STRING_GB2312	0x0A
#define VV_STRING_UTF8		0x0B
#define VV_STRING_UTF16LIT	0x0C
#define VV_STRING_UTF16BIG	0x0D

#define VV_BOOL_ARRAY	(VV_BOOL | 0x10)
#define VV_BYTE_ARRAY	(VV_BYTE | 0x10)
#define VV_SHORT_ARRAY	(VV_SHORT | 0x10)
#define VV_INT_ARRAY	(VV_INT | 0x10)
#define VV_LONG_ARRAY	(VV_LONG | 0x10)
#define VV_FLOAT_ARRAY	(VV_FLOAT | 0x10)
#define VV_DOUBLE_ARRAY	(VV_DOUBLE | 0x10)


#define IS_VARIANT_TYPE(tag) (((tag | 0x10) >= 0x10 && (tag | 0x10) <= 0x1D)? 1 : 0)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION variant_alloc: alloc variant object.
@INPUT int type: the variant type, it can be VV_NULL, VV_BYTE....
@RETURN variant_t: return variant object.
*/
EXP_API variant_t variant_alloc(int type);

/*
@FUNCTION variant_clone: clone a new variant object.
@INPUT const variant var: the variant object.
@RETURN variant_t: return new variant object.
*/
EXP_API variant_t variant_clone(variant_t var);

/*
@FUNCTION variant_free: free a variant object.
@INPUT variant var: the variant object.
@RETURN void: none.
*/
EXP_API void variant_free(variant_t var);

/*
@FUNCTION variant_get_type: get the variant type, it can be VV_NULL, VV_BYTE....
@INPUT variant_t var: the variant.
@RETURN int: return the variant type.
*/
EXP_API int variant_get_type(variant_t var);

/*
@FUNCTION variant_data: get variant data buffer.
@INPUT variant_t var: the variant object.
@RETURN void*: the data buffer.
*/
EXP_API void* variant_data(variant_t var);

/*
@FUNCTION variant_attach: attach variant data buffer.
@INPUT variant_t var: the variant object.
@INPUT void* data: the data buffer.
@RETURN void*: the data buffer.
*/
EXP_API void variant_attach(variant_t var, void* data);

/*
@FUNCTION variant_detach: detach variant data buffer.
@INPUT variant_t var: the variant object.
@RETURN void*: the data buffer.
*/
EXP_API const void* variant_detach(variant_t var);

/*
@FUNCTION variant_copy: copy a variant object.
@INPUT variant_t pdst: the destination variant object.
@INPUT const variant_t psrc: the source variant object.
@RETURN void: none.
*/
EXP_API void variant_copy(variant_t pdst, variant_t psrc);

/*
@FUNCTION variant_is_null: test variant object is empty.
@INPUT variant var: the variant object.
@RETURN bool_t: return nonzero for empty variant object, otherwise return zero.
*/
EXP_API bool_t variant_is_null(variant_t var);

/*
@FUNCTION variant_to_null: set variant object to empty and change type.
@INPUT variant var: the variant object.
@INPUT int type: the variant type, it can be VV_NULL, VV_BYTE....
@RETURN void: none.
*/
EXP_API void variant_to_null(variant_t var, int type);

/*
@FUNCTION variant_to_string: format variant object to string buffer.
@INPUT variant var: the variant object.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the buffer size in characters, not include terminate character.
@RETURN int: return the string token formated length in characters.
*/
EXP_API int variant_to_string(variant_t var, tchar_t* buf, int max);

/*
@FUNCTION variant_from_string: parse variant object from string buffer.
@INPUT variant var: the variant object.
@INPUT const tchar_t* str: the string token.
@INPUT int len: the string token length in characters, not include terminate character.
@RETURN void: none.
*/
EXP_API void variant_from_string(variant_t var, const tchar_t* str, int len);

/*
@FUNCTION variant_set_bool: set variant bool value, the variant type must be VV_BOOL.
@INPUT variant var: the variant object.
@INPUT bool_t c: the bool value.
@RETURN void: none.
*/
EXP_API void variant_set_bool(variant_t var, bool_t c);

/*
@FUNCTION variant_get_bool: get variant bool value, the variant type must be VV_BYTE.
@INPUT variant var: the variant object.
@RETURN bool_t: the bool value..
*/
EXP_API bool_t variant_get_bool(variant_t var);

/*
@FUNCTION variant_set_short: set variant short value, the variant type must be VV_SHORT.
@INPUT variant var: the variant object.
@INPUT short c: the short value.
@RETURN void: none.
*/
EXP_API void variant_set_short(variant_t var, short c);

/*
@FUNCTION variant_get_short: get variant short value, the variant type must be VV_SHORT.
@INPUT variant var: the variant object.
@RETURN short: the short value..
*/
EXP_API short variant_get_short(variant_t var);

/*
@FUNCTION variant_set_int: set variant int value, the variant type must be VV_INT.
@INPUT variant var: the variant object.
@INPUT int c: the int value.
@RETURN void: none.
*/
EXP_API void variant_set_int(variant_t var, int c);

/*
@FUNCTION variant_get_int: get variant int value, the variant type must be VV_INT.
@INPUT variant var: the variant object.
@RETURN int: the int value..
*/
EXP_API int variant_get_int(variant_t var);

/*
@FUNCTION variant_set_long long: set variant long long value, the variant type must be VV_LONG.
@INPUT variant var: the variant object.
@INPUT long long c: the long long value.
@RETURN void: none.
*/
EXP_API void variant_set_long(variant_t var, long long c);

/*
@FUNCTION variant_get_long long: get variant long long value, the variant type must be VV_LONG.
@INPUT variant var: the variant object.
@RETURN long long: the long long value..
*/
EXP_API long long variant_get_long(variant_t var);

/*
@FUNCTION variant_set_float: set variant float value, the variant type must be VV_FLOAT.
@INPUT variant var: the variant object.
@INPUT float c: the float value.
@RETURN void: none.
*/
EXP_API void variant_set_float(variant_t var, float c);

/*
@FUNCTION variant_get_float: get variant float value, the variant type must be VV_FLOAT.
@INPUT variant var: the variant object.
@RETURN float: the float value..
*/
EXP_API float variant_get_float(variant_t var);

/*
@FUNCTION variant_set_double: set variant double value, the variant type must be VV_DOUBLE.
@INPUT variant var: the variant object.
@INPUT double c: the double value.
@RETURN void: none.
*/
EXP_API void variant_set_double(variant_t var, double c);

/*
@FUNCTION variant_get_double: get variant double value, the variant type must be VV_DOUBLE.
@INPUT variant var: the variant object.
@RETURN double: the double value..
*/
EXP_API double variant_get_double(variant_t var);

/*
@FUNCTION variant_encode: encode variant object to bytes buffer.
@INPUT variant var: the variant object.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return encoded bytes.
*/
EXP_API dword_t variant_encode(variant_t var, byte_t* buf, dword_t max);

/*
@FUNCTION variant_decode: decode variant object from bytes buffer.
@INPUT variant var: the variant object.
@INPUT const byte_t* buf: the data buffer.
@RETURN dword_t: the decoded size in bytes.
*/
EXP_API dword_t variant_decode(variant_t var, const byte_t* buf);

/*
@FUNCTION variant_comp: compare two variant object, 
null variant object equal to null variant object and less than other type variant object.
@INPUT variant var1: the variant object.
@INPUT variant var2: the variant object.
@RETURN int: 0 for equal to, 1 for greater than, -1 for less than.
*/
EXP_API int variant_comp(variant_t var1, variant_t var2);

/*
@FUNCTION variant_hash32: get variant object 32bits hash code.
@INPUT variant var: the variant object.
@OUTPUT key32_t* pkey: the 32bits key buffer.
@RETURN void: none.
*/
EXP_API void variant_hash32(variant_t var, key32_t* pkey);

/*
@FUNCTION variant_hash64: get variant object 64bits hash code.
@INPUT variant var: the variant object.
@OUTPUT key64_t* pkey: the 64bits key buffer.
@RETURN void: none.
*/
EXP_API void variant_hash64(variant_t var, key64_t* pkey);

/*
@FUNCTION variant_hash128: get variant object 128bits hash code.
@INPUT variant var: the variant object.
@OUTPUT key128_t* pkey: the 128bits key buffer.
@RETURN void: none.
*/
EXP_API void variant_hash128(variant_t var, key128_t* pkey);

#if defined(XDL_SUPPORT_TEST)
EXP_API void test_variant(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif //VARIANT_H
