/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl vector document

	@module	vector.h | interface file

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

#ifndef _VECTOR_H
#define _VECTOR_H

#include "xdldef.h"

typedef struct _vector_t{
	dword_t size;
	double* data;
}vector_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION vector_alloc: alloc vector.
@INPUT sword_t size: the vector size.
@RETURN vector_t*: return vector struct.
*/
XDL_API vector_t* vector_alloc(sword_t size);

/*
@FUNCTION vector_free: free vector.
@INPUT vector_t*: the vector struct.
@RETURN void: none.
*/
XDL_API void vector_free(vector_t* pvt);

/*
@FUNCTION vector_zero: set vector elements value to zero.
@INPUT vector_t*: the vector struct.
@RETURN void: none.
*/
XDL_API void vector_zero(vector_t* pvt);

/*
@FUNCTION vector_set_value: set vector element value.
@INPUT vector_t*: the vector struct.
@INPUT sword_t i: the vector element zero based index.
@INPUT double db: the value to set.
@RETURN void: none.
*/
XDL_API void vector_set_value(vector_t* pvt, sword_t i, double db);

/*
@FUNCTION vector_get_value: get vector element value.
@INPUT vector_t*: the vector struct.
@INPUT sword_t i: the vector element zero based index.
@RETURN double: return the element value if exists, otherwise return zero.
*/
XDL_API double vector_get_value(vector_t* pvt, sword_t i);

/*
@FUNCTION vector_parse: parse vector element value from string.
@INPUT vector_t* pvt: the vector struct.
@INPUT const tchar_t* str: string token, number separated by space.
@INPUT int len: length of string token.
@RETURN void: none.
*/
XDL_API void vector_parse(vector_t* pvt, const tchar_t* str, int len);

/*
@FUNCTION vector_format: format vector element to string.
@INPUT vector_t* pvt: the vector struct.
@OUTPUT tchar_t* buf: buffer for formating.
@INPUT int max: the buffer size in characters, not include terminate character.
@RETURN int: return the formated string token length.
*/
XDL_API int vector_format(vector_t* pvt, tchar_t* buf, int max);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_API void test_vector(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*SET_H*/