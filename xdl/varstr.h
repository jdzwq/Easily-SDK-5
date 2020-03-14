/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc variant string document

	@module	string.h | interface file

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

#ifndef _VARSTR_H
#define _VARSTR_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION string_alloc: alloc a string object.
@RETURN string_t: return the string object
*/
EXP_API string_t string_alloc(void);

/*
@FUNCTION string_free: free a string object.
@INPUT string_t vs: the string object.
@RETURN void: none.
*/
EXP_API void string_free(string_t vs);

/*
@FUNCTION string_incre: increase string object buffer size.
@INPUT string_t vs: the string object.
@INPUT int len: length in characters to increased.
@RETURN void: none.
*/
EXP_API void string_incre(string_t vs, int len);

/*
@FUNCTION string_cat: cat a string token to string object.
@INPUT string_t vs: the string object.
@INPUT const tchar_t* str: the string token.
@INPUT int len: length in characters of string token.
@RETURN int: return total string length in characters in string object.
*/
EXP_API int	string_cat(string_t vs, const tchar_t* str, int len);

/*
@FUNCTION string_cpy: copy a string token to string object.
@INPUT string_t vs: the string object.
@INPUT const tchar_t* str: the string token.
@INPUT int len: length in characters of string token.
@RETURN int: return total string length in characters in the string object.
*/
EXP_API int	string_cpy(string_t vs, const tchar_t* str, int len);

/*
@FUNCTION string_printf: fill string object using printf.
@INPUT string_t vs: the string object.
@INPUT const tchar_t* fmt: the format token.
@INPUT ...: variable parameters.
@RETURN int: return total string length in characters in the string object.
*/
EXP_API int string_printf(string_t vs, const tchar_t* fmt, ...);

/*
@FUNCTION string_append: append string object using printf.
@INPUT string_t vs: the string object.
@INPUT const tchar_t* fmt: the format token.
@INPUT ...: variable parameters.
@RETURN int: return total string length in characters in the string object.
*/
EXP_API int string_append(string_t vs, const tchar_t* fmt, ...);

/*
@FUNCTION string_ptr: get string object buffer pointer.
@INPUT string_t vs: the string object.
@RETURN const tchar_t*: return the buffer pointer.
*/
EXP_API const tchar_t* string_ptr(string_t vs);

/*
@FUNCTION string_len: get string token length of the string object.
@INPUT string_t vs: the string object.
@RETURN int*: return the string token length in characters.
*/
EXP_API int string_len(string_t vs);

/*
@FUNCTION string_empty: empty string buffer of the string object.
@INPUT string_t vs: the string object.
@RETURN void: none.
*/
EXP_API void string_empty(string_t vs);

/*
@FUNCTION string_is_empty: test string buffer is empty.
@INPUT string_t vs: the string object.
@RETURN bool_t: nonezero for the buffer of string object is empty, otherwise return zero.
*/
EXP_API bool_t string_is_empty(string_t vs);

/*
@FUNCTION string_clone: clone a new string object.
@INPUT string_t vs: the string object.
@RETURN string_t: the new string object.
*/
EXP_API string_t string_clone(string_t vs);

/*
@FUNCTION string_resize: resize string buffer size of the string object.
@INPUT string_t vs: the string object.
@INPUT len: the new size in characters, not include terminate character.
@RETURN int: the new size.
*/
EXP_API int string_resize(string_t vs, int len);

/*
@FUNCTION string_encode: encode the string object and output to buffer.
@INPUT string_t vs: the string object.
@INPUT int encode: the code type, it can be _UTF8, _UTF16_BIG, _UTF16_LIT, _GB2312.
@OUTPUT buf: the bytes buffer for outputing.
@INPUT dword_t max: the buffer size.
@RETURN dword_t: return the outputed byte size.
*/
EXP_API dword_t string_encode(string_t vs, int encode, byte_t* buf, dword_t max);

/*
@FUNCTION string_decode: decode buffer into the string object.
@INPUT string_t vs: the string object.
@INPUT int encode: the code type, it can be _UTF8, _UTF16_BIG, _UTF16_LIT, _GB2312.
@INPUT const buf: the bytes buffer to decoding.
@INPUT dword_t size: the buffer content size.
@RETURN int: return string token length in characters in string object.
*/
EXP_API int string_decode(string_t vs, int encode, const byte_t* buf, dword_t size);

/*
@FUNCTION string_ensure_buf: expand buffer size of the string object 
@INPUT string_t vs: the string object.
@INPUT int len: the string buffer size in characters, not include terminate character.
@RETURN tchar_t: return string buffer pointer.
*/
EXP_API tchar_t* string_ensure_buf(string_t vs, int len);

/*
@FUNCTION string_attach_buf: repleace buffer of the string object
@INPUT string_t vs: the string object.
@INPUT tchar_t* buf: the buffer pointer.
@INPUT int size: the buffer size, not include terminate character.
@RETURN void: none.
*/
EXP_API void string_attach_buf(string_t vs, tchar_t* buf, int size);

/*
@FUNCTION string_detach_buf: detach buffer of the string object, then the string object is empty.
@INPUT string_t vs: the string object.
@RETURN tchar_t*: the buffer pointer if exists, otherwise return NULL.
*/
EXP_API tchar_t* string_detach_buf(string_t vs);

/*
@FUNCTION string_get_char: get a character at position of the string object.
@INPUT string_t vs: the string object.
@INPUT int pos: the zero based position.
@RETURN tchar_t: return the character if exists, otherwise return zero.
*/
EXP_API tchar_t string_get_char(string_t vs, int pos);

/*
@FUNCTION string_set_char: set a character at position of the string object.
@INPUT string_t vs: the string object.
@INPUT int pos: the zero based position.
@INPUT tchar_t ch: the character.
@RETURN bool_t: if position in the buffer size range return nonzero, otherwise return zero.
*/
EXP_API bool_t string_set_char(string_t vs, int pos, tchar_t ch);

/*
@FUNCTION string_get_chars: get some characters from the position in string object.
@INPUT string_t vs: the string object.
@INPUT int pos: the zero based start position.
@OUTPUT tchar_t* pch: the characters buffer.
@INPUT int n: the characters count want to get.
@RETURN int: return the characters count copyed.
*/
EXP_API int string_get_chars(string_t vs, int pos, tchar_t* pch, int n);

/*
@FUNCTION string_set_chars: set some characters from the position in string object.
@INPUT string_t vs: the string object.
@INPUT int pos: the zero based start position.
@INPUT cont tchar_t* pch: the characters buffer.
@INPUT int n: the characters count want to set.
@RETURN void: none.
*/
EXP_API void string_set_chars(string_t vs, int pos, const tchar_t* pch, int n);

/*
@FUNCTION string_ins_chars: insert some characters from the position in string object.
@INPUT string_t vs: the string object.
@INPUT int pos: the zero based start position.
@INPUT cont tchar_t* pch: the characters buffer.
@INPUT int n: the characters count want to set.
@RETURN void: none.
*/
EXP_API void string_ins_chars(string_t vs, int pos, const tchar_t* pch, int n);

/*
@FUNCTION string_del_chars: delete some characters from the position in string object.
@INPUT string_t vs: the string object.
@INPUT int pos: the zero based start position.
@INPUT cont tchar_t* pch: the characters buffer.
@INPUT int n: the characters count want to delete.
@RETURN void: none.
*/
EXP_API void string_del_chars(string_t vs, int pos, int n);

#ifdef	__cplusplus
}
#endif

#endif /*STRING_H*/
