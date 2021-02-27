/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc matrix document

	@module	matrix.h | interface file

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

#ifndef _MATRIX_H
#define _MATRIX_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION matrix_alloc: alloc matrix.
@INPUT int rows: rows of matrix.
@INPUT int cols: cols of matrix.
@RETURN matrix_t: return matrix struct.
*/
EXP_API matrix_t matrix_alloc(int rows, int cols);

/*
@FUNCTION matrix_free: free matrix.
@INPUT matrix_t mat: the matrix struct.
@RETURN void: none.
*/
EXP_API void matrix_free(matrix_t mat);

/*
@FUNCTION matrix_clone: clone a matrix from souce.
@INPUT const matrix_t: the source matrix struct.
@RETURN matrix_t: return matrix struct.
*/
EXP_API matrix_t matrix_clone(matrix_t mat);

/*
@FUNCTION matrix_reset: realloc matrix elements.
@INPUT matrix_t: the matrix struct.
@INPUT int rows: rows of matrix.
@INPUT int cols: cols of matrix.
@RETURN void: none.
*/
EXP_API void matrix_reset(matrix_t mat, int rows, int cols);

/*
@FUNCTION matrix_data: get matrix data buffer.
@INPUT matrix_t mat: the matrix object.
@RETURN void*: the data buffer.
*/
EXP_API const void* matrix_data(matrix_t mat);

/*
@FUNCTION matrix_attach: attach matrix data buffer.
@INPUT matrix_t mat: the matrix object.
@INPUT void* data: the data buffer.
@RETURN void*: the data buffer.
*/
EXP_API void matrix_attach(matrix_t mat, void* data);

/*
@FUNCTION matrix_detach: detach matrix data buffer.
@INPUT matrix_t mat: the matrix object.
@RETURN void*: the data buffer.
*/
EXP_API void* matrix_detach(matrix_t mat);

/*
@FUNCTION matrix_copy: copy the matrix.
@INPUT matrix_t dst: the destent matrix struct.
@INPUT matrix_t src: the srource matrix struct.
@RETURN void: none.
*/
EXP_API void matrix_copy(matrix_t dst, matrix_t src);

/*
@FUNCTION matrix_get_rows: get the matrix rows.
@INPUT matrix_t mat: the matrix struct.
@RETURN int: row count.
*/
EXP_API int matrix_get_rows(matrix_t mat);

/*
@FUNCTION matrix_get_cols: get the matrix cols.
@INPUT matrix_t mat: the matrix struct.
@RETURN int: col count.
*/
EXP_API int matrix_get_cols(matrix_t mat);

/*
@FUNCTION matrix_zero: set the matrix elements value to zero.
@INPUT matrix_t mat: the matrix struct.
@RETURN void: none.
*/
EXP_API void matrix_zero(matrix_t mat);

/*
@FUNCTION matrix_set_value: set the matrix element value.
@INPUT matrix_t mat: the matrix struct.
@INPUT int i: zero based row index.
@INPUT int j: zero based col index.
@INPUT double db: the value to set.
@RETURN void: none.
*/
EXP_API void matrix_set_value(matrix_t mat, int i, int j, double db);

/*
@FUNCTION matrix_get_value: get the matrix element value.
@INPUT matrix_t mat: the matrix struct.
@INPUT int i: zero based row index.
@INPUT int j: zero based col index.
@RETURN double: return the element value if exists, otherwise return zero.
*/
EXP_API double matrix_get_value(matrix_t mat, int i, int j);

/*
@FUNCTION matrix_parse: parse matrix element value from string.
@INPUT matrix_t mat: the matrix struct.
@INPUT const tchar_t* str: string token, number separated by space.
@INPUT int len: length of string token.
@RETURN void: none.
*/
EXP_API void matrix_parse(matrix_t mat, const tchar_t* str, int len);

/*
@FUNCTION matrix_format: format matrix element to string.
@INPUT matrix_t mat: the matrix struct.
@OUTPUT tchar_t* buf: buffer for formating.
@INPUT int max: the buffer size in characters, not include terminate character.
@RETURN int: return the formated string token length.
*/
EXP_API int matrix_format(matrix_t mat, tchar_t* buf, int max);

/*
@FUNCTION matrix_encode: encode matrix object to bytes buffer.
@INPUT matrix mat: the matrix object.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return encoded bytes.
*/
EXP_API dword_t matrix_encode(matrix_t mat, byte_t* buf, dword_t max);

/*
@FUNCTION matrix_decode: decode matrix object from bytes buffer.
@INPUT matrix mat: the matrix object.
@INPUT const byte_t* buf: the data buffer.
@RETURN dword_t: return decoded bytes.
*/
EXP_API dword_t matrix_decode(matrix_t mat, const byte_t* buf);

/*
@FUNCTION matrix_unit: set the matrix elements value to 1.
@INPUT matrix_t mat: the matrix struct.
@RETURN void: none.
*/
EXP_API void matrix_unit(matrix_t mat);

EXP_API matrix_t matrix_trans(matrix_t mt);

EXP_API matrix_t matrix_plus(matrix_t mt, double dbl);

EXP_API matrix_t matrix_add(matrix_t mt1, matrix_t mt2);

EXP_API matrix_t matrix_mul(matrix_t mt1, matrix_t mt2);

EXP_API double matrix_det(matrix_t mt);

#if defined(XDL_SUPPORT_TEST)
	EXP_API void test_matrix(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_MATRIX_H*/