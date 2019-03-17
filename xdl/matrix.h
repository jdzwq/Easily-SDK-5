/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc matrix document

	@module	matrix.h | matrix interface file

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

typedef struct _matrix_t{	
	sword_t rows, cols;
	double* data;
}matrix_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION matrix_alloc: alloc matrix.
@INPUT sword_t rows: rows of matrix.
@INPUT sword_t cols: cols of matrix.
@RETURN matrix_t*: return matrix struct.
*/
XDL_API matrix_t* matrix_alloc(sword_t rows, sword_t cols);

/*
@FUNCTION matrix_free: free matrix.
@INPUT matrix_t* pmt: the matrix struct.
@RETURN void: none.
*/
XDL_API void matrix_free(matrix_t* pmt);

/*
@FUNCTION matrix_zero: set the matrix elements value to zero.
@INPUT matrix_t* pmt: the matrix struct.
@RETURN void: none.
*/
XDL_API void matrix_zero(matrix_t* pmt);

/*
@FUNCTION matrix_set_value: set the matrix element value.
@INPUT matrix_t* pmt: the matrix struct.
@INPUT sword_t i: zero based row index.
@INPUT sword_t j: zero based col index.
@INPUT double db: the value to set.
@RETURN void: none.
*/
XDL_API void matrix_set_value(matrix_t* pmt, sword_t i, sword_t j, double db);

/*
@FUNCTION matrix_get_value: get the matrix element value.
@INPUT matrix_t* pmt: the matrix struct.
@INPUT sword_t i: zero based row index.
@INPUT sword_t j: zero based col index.
@RETURN double: return the element value if exists, otherwise return zero.
*/
XDL_API double matrix_get_value(matrix_t* pmt, sword_t i, sword_t j);

/*
@FUNCTION matrix_parse: parse matrix element value from string.
@INPUT matrix_t* pmt: the matrix struct.
@INPUT const tchar_t* str: string token, number separated by space.
@INPUT int len: length of string token.
@RETURN void: none.
*/
XDL_API void matrix_parse(matrix_t* pmt, const tchar_t* str, int len);

/*
@FUNCTION matrix_format: format matrix element to string.
@INPUT matrix_t* pmt: the matrix struct.
@OUTPUT tchar_t* buf: buffer for formating.
@INPUT int max: the buffer size in characters, not include terminate character.
@RETURN int: return the formated string token length.
*/
XDL_API int matrix_format(matrix_t* pmt, tchar_t* buf, int max);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_API void test_matrix(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_MATRIX_H*/