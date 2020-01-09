/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl set document

	@module	set.h | xdl set interface file

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

#ifndef _SET_H
#define _SET_H

#include "xdldef.h"



#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API object_t set_alloc(void);

	XDL_API void set_free(object_t set);

	XDL_API void set_copy(object_t dst, object_t src);

	XDL_API int set_comp(object_t s1, object_t s2);

	XDL_API void set_empty(object_t set);

	XDL_API void set_add(object_t set, object_t sub);

	XDL_API void set_del(object_t set, object_t sub);

	XDL_API bool_t set_in(object_t set, object_t sub);

	XDL_API void set_get(object_t set, int idex, object_t sub);

	XDL_API void set_parse(object_t set, const tchar_t* token, int len);

	XDL_API int set_format(object_t set, tchar_t* buf, int max);

#ifdef _DEBUG
	XDL_API void test_set(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*SET_H*/