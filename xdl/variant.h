/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc variant value document

	@module	variant.h | xdl variant memory interface file

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

#ifndef _VARIANT_H
#define _VARIANT_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API variant_t* variant_alloc(int encode);

XDL_API variant_t* variant_clone(const variant_t* pv);

XDL_API void variant_free(variant_t* pv);

XDL_API void variant_copy(variant_t* pv1, const variant_t* pv2);

XDL_API bool_t variant_is_null(variant_t* pv);

XDL_API void variant_to_null(variant_t* pv);

XDL_API int variant_to_string(variant_t* pv, tchar_t* buf, int max);

XDL_API void variant_from_string(variant_t* pv, const tchar_t* buf, int len);

XDL_API dword_t variant_encode(variant_t* pv, byte_t* buf, dword_t max);

XDL_API dword_t variant_decode(variant_t* pv, const byte_t* buf);

XDL_API int variant_comp(const variant_t* pv1, const variant_t* pv2);

XDL_API void variant_hash32(variant_t* pv, key32_t* pkey);

XDL_API void variant_hash64(variant_t* pv, key64_t* pkey);

XDL_API void variant_hash128(variant_t* pv, key128_t* pkey);

#if defined(_DEBUG) || defined(DEBUG)
XDL_API void test_variant(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif //VARIANT_H
