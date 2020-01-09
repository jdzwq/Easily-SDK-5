﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl cache document

	@module	impcache.h | xdl cache interface file

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

#ifndef _IMPCACHE_H
#define _IMPCACHE_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_MEMO_CACHE

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION: open a memory cache.
@RETURN: if succeeds return cache handle, fails return NULL.
*/
XDL_API xhand_t vmem_open(void);

/*
@FUNCTION: close a memory cache.
@INPUT cache: the cache handle.
@RETURN: none.
*/
XDL_API void vmem_close(xhand_t vmem);

/*
@FUNCTION: get a cache handle, the handle is buffer pointer.
@INPUT vmem: the cache handle.
@RETURN: if succeeds return buffer pointer, fails return NULL.
*/
XDL_API void* vmem_handle(xhand_t vmem);

/*
@FUNCTION: write data into cache.
@INPUT vmem: the cache handle.
@INPUT data: the data buffer pointer.
@INPUT pb: the integer buffer pointer holding byte count for writing, and return the actually byte count writed.
@RETURN: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t vmem_write(xhand_t vmem, const byte_t* data, dword_t* pb);

/*
@FUNCTION: read data from cache.
@INPUT vmem: the cache handle.
@INPUT data: the data buffer pointer.
@INPUT pb: the integer buffer pointer for holding byte count for reading, and return the actually byte count readed.
@RETURN: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t vmem_read(xhand_t vmem, byte_t* buf, dword_t* pb);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_MEMO_CACHE*/

#endif /*_IMPCACHE_H*/