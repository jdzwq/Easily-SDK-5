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
#include "matrix.h"

typedef struct _vector_t{
	int size;
	int dimens;
	double* data;
}vector_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION vector_alloc: alloc a vector.
@INPUT int size: the vector size.
@INPUT int dimens: the vector dimens, 1-dimens like: (x), 2-dimens like: (x,y), 3-dimens like (x,y,z).
@RETURN vector_t*: return vector struct.
*/
EXP_API vector_t* vector_alloc(int size, int dimens);

/*
@FUNCTION vector_free: free the vector.
@INPUT vector_t*: the vector struct.
@RETURN void: none.
*/
EXP_API void vector_free(vector_t* pvt);

/*
@FUNCTION vector_clone: clone a vector from souce.
@INPUT const vector_t*: the source vector struct.
@RETURN vector_t*: return vector struct.
*/
EXP_API vector_t* vector_clone(const vector_t* pvt);

/*
@FUNCTION vector_clear: clear vector elements.
@INPUT vector_t*: the vector struct.
@RETURN void: none.
*/
EXP_API void vector_empty(vector_t* pvt);

/*
@FUNCTION vector_zero: set vector elements value to zero.
@INPUT vector_t*: the vector struct.
@RETURN void: none.
*/
EXP_API void vector_zero(vector_t* pvt);

/*
@FUNCTION vector_unit: set vector elements value to 1.
@INPUT vector_t*: the vector struct.
@RETURN void: none.
*/
EXP_API void vector_unit(vector_t* pvt);

/*
@FUNCTION vector_copy: copy the vector.
@INPUT vector_t*: the destent vector struct.
@INPUT const vector_t*: the srource vector struct.
@RETURN void: none.
*/
EXP_API void vector_copy(vector_t* dest, const vector_t* src);

/*
@FUNCTION vector_set_value: set vector element value.
@INPUT vector_t*: the vector struct.
@INPUT int i: the vector element zero based index.
@INPUT double A: the A value to set.
@INPUT ...: variant double value to set according to vector dimens.
@RETURN void: none.
*/
EXP_API void vector_set_value(vector_t* pvt, int i, ...);

/*
@FUNCTION vector_get_value: get vector element value.
@INPUT vector_t*: the vector struct.
@INPUT int i: the vector element zero based index.
@INPUT double* pA: the buffer for return A value.
@INPUT ...: variant double value buffer for return more, according to vector dimens.
@RETURN void: none.
*/
EXP_API void vector_get_value(vector_t* pvt, int i, ...);

/*
@FUNCTION vector_parse: parse vector element value from string.
@INPUT vector_t* pvt: the vector struct.
@INPUT const tchar_t* str: string token, number separated by space.
@INPUT int len: length of string token.
@RETURN void: none.
*/
EXP_API void vector_parse(vector_t* pvt, const tchar_t* str, int len);

/*
@FUNCTION vector_format: format vector element to string.
@INPUT vector_t* pvt: the vector struct.
@OUTPUT tchar_t* buf: buffer for formating.
@INPUT int max: the buffer size in characters, not include terminate character.
@RETURN int: return the formated string token length.
*/
EXP_API int vector_format(vector_t* pvt, tchar_t* buf, int max);

EXP_API vector_t* vector_shift(vector_t vt, ...);

EXP_API vector_t* vector_rotate(vector_t vt, double ang);

EXP_API vector_t* vector_scale(vector_t vt, ...);

EXP_API vector_t* vector_shear(vector_t vt, double sx, double sy);

EXP_API vector_t* vector_trans(vector_t vt, matrix_t mt);

#if defined(_DEBUG) || defined(DEBUG)
	EXP_API void test_vector(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*SET_H*/