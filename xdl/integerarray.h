/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc integerarray document

	@module	integerarray.h | interface file

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

#ifndef _INTARRAY_H
#define _INTARRAY_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION alloc_integer_array: create a integer array.
@RETURN int**: return the integer array address.
*/
EXP_API int** alloc_integer_array(void);

/*
@FUNCTION destroy_integer_array: destroy a integer array.
@INPUT int** ptr: the integer array address.
@RETURN void: none.
*/
EXP_API void free_integer_array(int** sa);

/*
@FUNCTION clear_integer_array: delete all integer tokens.
@INPUT int** sa: the integer array address.
@RETURN void: none.
*/
EXP_API void clear_integer_array(int** sa);

/*
@FUNCTION get_integer_array_size: get integer array tokenes.
@INPUT int** sa: the integer array address.
@RETURN int: return the count of integer tokens.
*/
EXP_API int get_integer_array_size(int** sa);

/*
@FUNCTION get_integer: copy the integer token by index.
@INPUT int** sa: the integer array address.
@INPUT int index: the zero based array index.
@INPUT tchar_t* buf: the integer buf for copying.
@INPUT int max: the size of buffer.
@RETURN int: return characters copyed.
*/
EXP_API int get_integer(int** sa, int index);

/*
@FUNCTION insert_integer: insert the integer token before the position.
@INPUT int** sa: the integer array address.
@INPUT int index: the zero based array index, -1 indicate insertting at last.
@INPUT const tchar_t* token: the integer token to be inserted.
@INPUT int len: the length of token.
@RETURN void: none.
*/
EXP_API void insert_integer(int** sa, int index, int val);

/*
@FUNCTION delete_integer_token: delete the integer token.
@INPUT int** sa: the integer array address.
@INPUT int index: the zero based array index, -1 indicate delete the last token.
@RETURN void: none.
*/
EXP_API void delete_integer(int** sa, int index);


#if defined(XDL_SUPPORT_TEST)

EXP_API void test_integer_array(void);

#endif

#ifdef	__cplusplus
}
#endif

#endif //_INTARRAY_H
