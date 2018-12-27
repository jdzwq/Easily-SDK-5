/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory document

	@module	impmem.h | memory interface file

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

#ifndef _IMPMEM_H
#define	_IMPMEM_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void*	xmem_alloc(dword_t size);

XDL_API void*	xmem_realloc(void* p, dword_t size);

XDL_API void	xmem_free(void* p);

XDL_API void	xmem_zero(void* p, dword_t size);

XDL_API void	xmem_set(void* p, byte_t c, dword_t size);

XDL_API void*	xmem_clone(void* p,dword_t size);

XDL_API void	xmem_copy(void* dst, void* src, dword_t size);

XDL_API void	xmem_move(void* p, dword_t len, int off);

XDL_API int xmem_comp(void* mem1, dword_t len1, void* mem2, dword_t len2);

#ifdef XDL_SUPPORT_MEMO_DUMP

XDL_API dword_t	xmem_size(void* p);

XDL_API	void	xmem_assert(void* p);

#endif

#ifdef XDK_SUPPORT_MEMO_LOCAL
XDL_API void async_alloc_lapp(async_t* pas);

XDL_API void async_free_lapp(async_t* pas);
#endif

#ifdef XDK_SUPPORT_MEMO_GLOB

XDL_API res_glob_t	system_alloc(dword_t size);

XDL_API res_glob_t	system_realloc(res_glob_t glob, dword_t size);

XDL_API void		system_free(res_glob_t glob);

XDL_API dword_t		system_size(res_glob_t glob);

XDL_API void*		system_lock(res_glob_t glob);

XDL_API bool_t		system_unlock(res_glob_t glob);

XDL_API res_glob_t	system_alloc(dword_t size);

#endif

#ifdef XDK_SUPPORT_MEMO_PAGE

XDL_API void*		page_alloc(dword_t size);

XDL_API void*		page_realloc(void* p, dword_t size);

XDL_API void		page_free(void* p);

XDL_API dword_t		page_size(void* p);

XDL_API void*		page_lock(void* p);

XDL_API void		page_unlock(void* p);

XDL_API bool_t		page_protect(void* p);

#endif

#ifdef	__cplusplus
}
#endif

#endif	/*IMPMEM_H */

