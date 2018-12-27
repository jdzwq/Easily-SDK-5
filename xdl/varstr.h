/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc variant string document

	@module	varstr.h | xdl variant memory interface file

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

	XDL_API string_t varstr_alloc(void);

	XDL_API void varstr_free(string_t vs);

	XDL_API void varstr_incre(string_t vs, int len);

	XDL_API int	varstr_cat(string_t vs, const tchar_t* str, int len);

	XDL_API int	varstr_cpy(string_t vs, const tchar_t* str, int len);

	XDL_API int varstr_printf(string_t vs, const tchar_t* fmt, ...);

	XDL_API int varstr_append(string_t vs, const tchar_t* fmt, ...);

	XDL_API const tchar_t* varstr_ptr(string_t vs);

	XDL_API int varstr_len(string_t vs);

	XDL_API void varstr_empty(string_t vs);

	XDL_API string_t varstr_clone(string_t vs);

	XDL_API int varstr_resize(string_t vs, int len);

	XDL_API int varstr_encode(string_t vs, int encode, byte_t* buf, dword_t max);

	XDL_API int varstr_decode(string_t vs, int encode, const byte_t* buf, dword_t size);

	XDL_API tchar_t* varstr_ensure_buf(string_t vs, int len);

	XDL_API void varstr_attach_buf(string_t vs, tchar_t* buf, int size);

	XDL_API tchar_t* varstr_detach_buf(string_t vs);

	XDL_API tchar_t varstr_get_char(string_t vs, int pos);

	XDL_API bool_t varstr_set_char(string_t vs, int pos, tchar_t ch);

	XDL_API int varstr_get_chars(string_t vs, int pos, tchar_t* pch, int n);

	XDL_API void varstr_set_chars(string_t vs, int pos, const tchar_t* pch, int n);

	XDL_API void varstr_ins_chars(string_t vs, int pos, const tchar_t* pch, int n);

	XDL_API void varstr_del_chars(string_t vs, int pos, int n);

#ifdef	__cplusplus
}
#endif

#endif /*VARSTR_H*/
