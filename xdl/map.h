/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc map document

	@module	map.h | map interface file

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

#ifndef _MAP_H
#define _MAP_H

#include "xdldef.h"

typedef struct _map_t{	
	int bits, rows, cols;
	dword_t* data;
}map_t;

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API dword_t map_size(int nums, int bits);

	XDL_API map_t* map_alloc(int nums, int bits);

	XDL_API void map_free(map_t* pmm);

	XDL_API void map_zero(map_t* pmm);

	XDL_API void map_set_bits(map_t* pmm, int i, byte_t b);

	XDL_API byte_t map_get_bits(map_t* pmm, int i);

	XDL_API int map_find_bits(map_t* pmm, int i, byte_t b);

	XDL_API int map_test_bits(map_t* pmm, int i, byte_t b, int n);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_API void test_map(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_MAP_H*/