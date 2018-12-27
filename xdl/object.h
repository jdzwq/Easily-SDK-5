/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl object document

	@module	object.h | xdl object interface file

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

#ifndef _OBJECT_H
#define _OBJECT_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API object_t object_alloc(int encode);

	XDL_API void object_free(object_t obj);

	XDL_API object_t object_clone(object_t obj);

	XDL_API void object_copy(object_t dst, object_t src);

	XDL_API void object_empty(object_t obj);

	XDL_API int object_get_type(object_t obj);

	XDL_API int object_get_encode(object_t obj);

	XDL_API bool_t object_get_commpress(object_t obj);

	XDL_API void object_set_commpress(object_t obj, bool_t b);

	XDL_API void object_set_string(object_t obj, const tchar_t* str, int len);

	XDL_API int object_get_string(object_t obj, tchar_t* str, int max);

	XDL_API void object_set_variant(object_t obj, variant_t val);

	XDL_API bool_t object_get_variant(object_t obj, variant_t* pval);

	XDL_API void object_set_domdoc(object_t obj, link_t_ptr dom);

	XDL_API bool_t object_get_domdoc(object_t obj, link_t_ptr dom);

	XDL_API void object_set_bytes(object_t obj, int encode, const byte_t* buf, dword_t len);

	XDL_API dword_t object_get_bytes(object_t obj, byte_t* buf, dword_t max);

	XDL_API dword_t object_encode(object_t obj, byte_t* buf, dword_t max);

	XDL_API dword_t object_decode(object_t obj, const byte_t* data);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_API void test_object(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_OBJECT_H*/