/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl object document

	@module	object.h | interface file

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

#ifndef _VAROBJ_H
#define _VAROBJ_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION object_alloc: alloc a object.
@INPUT int encode: the encode, it can be _UTF8, _UTF16_BIG, _UTF16_LIT, _GB2312.
@RETURN object_t: return the object.
*/
EXP_API object_t object_alloc(int encode);

/*
@FUNCTION object_free: free a object.
@INPUT object_t obj: the object.
@RETURN void: none.
*/
EXP_API void object_free(object_t obj);

/*
@FUNCTION object_clone: clone a new object.
@INPUT object_t obj: the object for copying.
@RETURN object_t: the new object.
*/
EXP_API object_t object_clone(object_t obj);

/*
@FUNCTION object_copy: copy source object to destination object.
@INPUT object_t dst: the destination object.
@INPUT object_t src: the source object.
@RETURN void: none.
*/
EXP_API void object_copy(object_t dst, object_t src);

/*
@FUNCTION object_empty: empty the object.
@INPUT object_t obj: the object.
@RETURN void: none.
*/
EXP_API void object_empty(object_t obj);

/*
@FUNCTION object_get_type: get the object type, it can be _OBJECT_UNKNOWN, _OBJECT_STRING, _OBJECT_VARIANT, _OBJECT_DOMDOC, _OBJECT_BINARY.
@INPUT object_t obj: the object.
@RETURN int: return the object type, default is _OBJECT_UNKNOWN.
*/
EXP_API int object_get_type(object_t obj);

/*
@FUNCTION object_get_encode: get the object encode, it can be it can be _UTF8, _UTF16_BIG, _UTF16_LIT, _GB2312.
@INPUT object_t obj: the object.
@RETURN int: return the object encode.
*/
EXP_API int object_get_encode(object_t obj);

/*
@FUNCTION object_get_commpress: test the object is compressed.
@INPUT object_t obj: the object.
@RETURN boo_t: return nonzero if compressed, otherwise return zero.
*/
EXP_API bool_t object_get_commpress(object_t obj);

/*
@FUNCTION object_set_commpress: compress or decompress object.
@INPUT object_t obj: the object.
@INPUT bool_t b: nonzero for compressing, zero for decompressing.
@RETURN void: none.
*/
EXP_API void object_set_commpress(object_t obj, bool_t b);

/*
@FUNCTION object_set_string: save string token to object.
@INPUT object_t obj: the object.
@INPUT const tchar_t* str: the string token.
@INPUT int len: the string token length in characters.
@RETURN void: none.
*/
EXP_API void object_set_string(object_t obj, const tchar_t* str, int len);

/*
@FUNCTION object_get_string: get string token from object.
@INPUT object_t obj: the object.
@OUTPUT tchar_t* str: the string buffer.
@INPUT int max: the string buffer size in characters, not include terminate character.
@RETURN int: return the length of string token.
*/
EXP_API int object_get_string(object_t obj, tchar_t* str, int max);

/*
@FUNCTION object_set_variant: save variant to object.
@INPUT object_t obj: the object.
@INPUT variant_t val: the variant object.
@RETURN void: none.
*/
EXP_API void object_set_variant(object_t obj, variant_t val);

/*
@FUNCTION object_get_variant: get variant from object.
@INPUT object_t obj: the object.
@OUTPUT variant_t* pval: the variant object.
@RETURN bool_t: return nonzero if the object type is _OBJECT_VARIANT, otherwise return zero.
*/
EXP_API bool_t object_get_variant(object_t obj, variant_t* pval);

/*
@FUNCTION object_set_domdoc: save dom document to object.
@INPUT object_t obj: the object.
@INPUT link_t_ptr dom: the dom document.
@RETURN void: none.
*/
EXP_API void object_set_domdoc(object_t obj, link_t_ptr dom);

/*
@FUNCTION object_get_domdoc: get dom document from object.
@INPUT object_t obj: the object.
@OUTPUT link_t_ptr dom: the dom document.
@RETURN bool_t: return nonzero if the object type is _OBJECT_DOMDOC, otherwise return zero.
*/
EXP_API bool_t object_get_domdoc(object_t obj, link_t_ptr dom);

/*
@FUNCTION object_set_bytes: save bytes to object.
@INPUT object_t obj: the object.
@INPUT int encode: the encode it can be _UTF8, _UTF16_BIG, _UTF16_LIT, _GB2312.
@INPUT const byte_t* buf: the bytes buffer.
@INPUT dword_t len: the buffer size in bytes.
@RETURN void: none.
*/
EXP_API void object_set_bytes(object_t obj, int encode, const byte_t* buf, dword_t len);

/*
@FUNCTION object_add_bytes: append bytes to object.
@INPUT object_t obj: the object.
@INPUT const byte_t* buf: the bytes buffer.
@INPUT dword_t len: the buffer size in bytes.
@RETURN void: none.
*/
EXP_API void object_add_bytes(object_t obj, const byte_t* buf, dword_t len);

/*
@FUNCTION object_add_bytes: delete followed bytes from object.
@INPUT object_t obj: the object.
@INPUT dword_t off: the zero based posotin.
@RETURN void: none.
*/
EXP_API void object_del_bytes(object_t obj, dword_t off);

/*
@FUNCTION object_get_bytes: load bytes from object.
@INPUT object_t obj: the object.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return the bytes loaded.
*/
EXP_API dword_t object_get_bytes(object_t obj, byte_t* buf, dword_t max);

/*
@FUNCTION object_encode: encode a object to bytes buffer.
@INPUT object_t obj: the object.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return the bytes encoded.
*/
EXP_API dword_t object_encode(object_t obj, byte_t* buf, dword_t max);

/*
@FUNCTION object_decode: decode a object from bytes buffer.
@INPUT object_t obj: the object.
@INPUT const byte_t* data: the bytes buffer.
@RETURN dword_t: return the bytes decoded.
*/
EXP_API dword_t object_decode(object_t obj, const byte_t* data);

#if defined(_DEBUG) || defined(DEBUG)
	EXP_API void test_object(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_OBJECT_H*/