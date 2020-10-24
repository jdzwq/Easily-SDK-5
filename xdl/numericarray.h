/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc numericarray document

	@module	numericarray.h | interface file

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

#ifndef _NUMARRAY_H
#define _NUMARRAY_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION alloc_numeric_array: create a numeric array.
@RETURN double**: return the numeric array address.
*/
EXP_API double** alloc_numeric_array(void);

/*
@FUNCTION destroy_numeric_array: destroy a numeric array.
@INPUT double** ptr: the numeric array address.
@RETURN void: none.
*/
EXP_API void free_numeric_array(double** sa);

/*
@FUNCTION clear_numeric_array: delete all numeric tokens.
@INPUT double** sa: the numeric array address.
@RETURN void: none.
*/
EXP_API void clear_numeric_array(double** sa);

/*
@FUNCTION get_numeric_array_size: get numeric array tokenes.
@INPUT double** sa: the numeric array address.
@RETURN int: return the count of numeric tokens.
*/
EXP_API int get_numeric_array_size(double** sa);

/*
@FUNCTION get_numeric: copy the numeric token by index.
@INPUT double** sa: the numeric array address.
@INPUT int index: the zero based array index.
@INPUT tchar_t* buf: the numeric buf for copying.
@INPUT int max: the size of buffer.
@RETURN int: return characters copyed.
*/
EXP_API double get_numeric(double** sa, int index);

/*
@FUNCTION insert_numeric: insert the numeric token before the position.
@INPUT double** sa: the numeric array address.
@INPUT int index: the zero based array index, -1 indicate insertting at last.
@INPUT const tchar_t* token: the numeric token to be inserted.
@INPUT int len: the length of token.
@RETURN void: none.
*/
EXP_API void insert_numeric(double** sa, int index, double val);

/*
@FUNCTION delete_numeric_token: delete the numeric token.
@INPUT double** sa: the numeric array address.
@INPUT int index: the zero based array index, -1 indicate delete the last token.
@RETURN void: none.
*/
EXP_API void delete_numeric(double** sa, int index);


#if defined(_DEBUG) || defined(DEBUG)

EXP_API void test_numeric_array(void);

#endif

#ifdef	__cplusplus
}
#endif

#endif //_NUMARRAY_H
