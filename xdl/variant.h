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

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION variant_alloc: alloc variant object.
@INPUT int encode: the variant encode, it can be _UTF8, _UTF16_BIG, _UTF16_LIT, _GB2312.
@RETURN variant_t*: return variant object.
*/
XDL_API variant_t* variant_alloc(int encode);

/*
@FUNCTION variant_clone: clone a new variant object.
@INPUT const variant_t* pv: the variant object.
@RETURN variant_t*: return new variant object.
*/
XDL_API variant_t* variant_clone(const variant_t* pv);

/*
@FUNCTION variant_free: free a variant object.
@INPUT variant_t* pv: the variant object.
@RETURN void: none.
*/
XDL_API void variant_free(variant_t* pv);

/*
@FUNCTION variant_copy: copy a variant object.
@INPUT variant_t* pdst: the destination variant object.
@INPUT const variant_t* psrc: the source variant object.
@RETURN void: none.
*/
XDL_API void variant_copy(variant_t* pdst, const variant_t* psrc);

/*
@FUNCTION variant_is_null: test variant object is empty.
@INPUT variant_t* pv: the variant object.
@RETURN bool_t: return nonzero for empty variant object, otherwise return zero.
*/
XDL_API bool_t variant_is_null(variant_t* pv);

/*
@FUNCTION variant_to_null: set variant object to empty.
@INPUT variant_t* pv: the variant object.
@RETURN void: none.
*/
XDL_API void variant_to_null(variant_t* pv);

/*
@FUNCTION variant_to_string: format variant object to string buffer.
@INPUT variant_t* pv: the variant object.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the buffer size in characters, not include terminate character.
@RETURN int: return the string token formated length in characters.
*/
XDL_API int variant_to_string(variant_t* pv, tchar_t* buf, int max);

/*
@FUNCTION variant_from_string: parse variant object from string buffer.
@INPUT variant_t* pv: the variant object.
@INPUT const tchar_t* str: the string token.
@INPUT int len: the string token length in characters, not include terminate character.
@RETURN void: none.
*/
XDL_API void variant_from_string(variant_t* pv, const tchar_t* str, int len);

/*
@FUNCTION variant_encode: encode variant object to bytes buffer.
@INPUT variant_t* pv: the variant object.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return encoded bytes.
*/
XDL_API dword_t variant_encode(variant_t* pv, byte_t* buf, dword_t max);

/*
@FUNCTION variant_decode: decode variant object from bytes buffer.
@INPUT variant_t* pv: the variant object.
@INPUT const byte_t* buf: the bytes buffer.
@RETURN dword_t: return decoded bytes.
*/
XDL_API dword_t variant_decode(variant_t* pv, const byte_t* buf);

/*
@FUNCTION variant_comp: compare two variant object, 
null variant object equal to null variant object and less than other type variant object.
@INPUT variant_t* pv1: the variant object.
@INPUT variant_t* pv2: the variant object.
@RETURN int: 0 for equal to, 1 for greater than, -1 for less than.
*/
XDL_API int variant_comp(const variant_t* pv1, const variant_t* pv2);

/*
@FUNCTION variant_hash32: get variant object 32bits hash code.
@INPUT variant_t* pv: the variant object.
@OUTPUT key32_t* pkey: the 32bits key buffer.
@RETURN void: none.
*/
XDL_API void variant_hash32(variant_t* pv, key32_t* pkey);

/*
@FUNCTION variant_hash64: get variant object 64bits hash code.
@INPUT variant_t* pv: the variant object.
@OUTPUT key64_t* pkey: the 64bits key buffer.
@RETURN void: none.
*/
XDL_API void variant_hash64(variant_t* pv, key64_t* pkey);

/*
@FUNCTION variant_hash128: get variant object 128bits hash code.
@INPUT variant_t* pv: the variant object.
@OUTPUT key128_t* pkey: the 128bits key buffer.
@RETURN void: none.
*/
XDL_API void variant_hash128(variant_t* pv, key128_t* pkey);

#if defined(_DEBUG) || defined(DEBUG)
XDL_API void test_variant(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif //VARIANT_H
