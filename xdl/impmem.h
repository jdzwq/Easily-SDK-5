﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory document

	@module	impmem.h | interface file

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

/*
@FUNCTION xmem_alloc: alloc memory by size, if XDL start in thread appartment, then memory alloced from thread private heap,
otherwise, memory alloced from process default heap. 
if XDL reduced to not support heap, then use c library to alloc memory.
@INPUT dword_t size: great then or equal to zero
@RETURN void*: if size equal to zero then return NULL, otherwise return the memory buffer pointer
*/
EXP_API void*	xmem_alloc(dword_t size);

/*
@FUNCTION xmem_realloc: realloc memory by request size. the alloc mode same as xmem_alloc function.
@INPUT void* p: the original memoy pointer, can be NULL.
@INPUT dword_t size: great then or equal to zero
@RETURN void*: if size equal to zero then return NULL, otherwise return the memory buffer pointer
*/
EXP_API void*	xmem_realloc(void* p, dword_t size);

/*
@FUNCTION xmem_free: free the memory by pointer.
@INPUT void* p: the memoy pointer, can be NULL.
@RETURN void: none
*/
EXP_API void	xmem_free(void* p);

/*
@FUNCTION xmem_zero: set the memory content to zero.
@INPUT void* p: the memoy pointer, can not be NULL.
@INPUT dword_t size: total bytes to set zero.
@RETURN void: none
*/
EXP_API void	xmem_zero(void* p, dword_t size);

/*
@FUNCTION xmem_set: fill the memory with a byte value.
@INPUT void* p: the memoy pointer, can not be NULL.
@INPUT byte_t byte: the value to fill.
@INPUT dword_t size: total bytes to fill.
@RETURN void: none
*/
EXP_API void	xmem_set(void* p, byte_t c, dword_t size);

/*
@FUNCTION xmem_clone: clone the memory segment.
@INPUT void* p: the memoy pointer, can not be NULL.
@INPUT dword_t size: total bytes to copy.
@RETURN void*: the new memory pointer.
*/
EXP_API void*	xmem_clone(void* p,dword_t size);

/*
@FUNCTION xmem_copy: copy the source memoy content to destent.
@INPUT void* dst: the destent memoy pointer, can not be NULL.
@INPUT void* src: the source memoy pointer, can not be NULL.
@INPUT dword_t size: the source memory content size in bytes.
@RETURN void: none
*/
EXP_API void	xmem_copy(void* dst, void* src, dword_t size);

/*
@FUNCTION xmem_move: move the memoy content.
@INPUT void* p: the memoy pointer, can not be NULL.
@INPUT dword_t len: total bytes to move
@INPUT int off: offset to move, if off great then zero, content move to higher address, 
else if less then zero,then move to lower address.
@RETURN void: none
*/
EXP_API void	xmem_move(void* p, dword_t len, int off);

/*
@FUNCTION xmem_comp: compare two memoy content one by one byte.
@INPUT void* mem1: the first memoy pointer, can be NULL.
@INPUT dword_t len1: mem1 total bytes to compare
@INPUT void* mem2: the second memoy pointer, can be NULL.
@INPUT dword_t len2: mem2 total bytes to compare
@RETURN int: the same content and length will return 0, else if mem1 great then mem2 then return 1, otherwise return -1.
*/
EXP_API int xmem_comp(void* mem1, dword_t len1, void* mem2, dword_t len2);

#ifdef XDL_SUPPORT_MEMO_DUMP

EXP_API dword_t	xmem_size(void* p);

EXP_API	void	xmem_assert(void* p);

#endif

#ifdef XDK_SUPPORT_MEMO_GLOB

/*
@FUNCTION gmem_alloc: alloc global memory by size
@INPUT dword_t size: total bytes to alloc.
@RETURN res_glob_t: the global memory handle.
*/
EXP_API res_glob_t	gmem_alloc(dword_t size);

/*
@FUNCTION gmem_realloc: realloc global memory.
@INPUT res_glob_t glob: the original global handle
@INPUT dword_t size: total bytes to alloc.
@RETURN res_glob_t: the new global memory handle.
*/
EXP_API res_glob_t	gmem_realloc(res_glob_t glob, dword_t size);

/*
@FUNCTION gmem_free: free global memory.
@INPUT res_glob_t glob: the original global handle
@RETURN void: none.
*/
EXP_API void		gmem_free(res_glob_t glob);

/*
@FUNCTION gmem_size: get global memory space size.
@INPUT res_glob_t glob: the global handle
@RETURN dword_t: bytes.
*/
EXP_API dword_t		gmem_size(res_glob_t glob);

/*
@FUNCTION gmem_lock: reference the global memory pointer
@INPUT res_glob_t glob: the global handle
@RETURN void*: memory pointer.
*/
EXP_API void*		gmem_lock(res_glob_t glob);

/*
@FUNCTION gmem_unlock: discard the global memory pointer
@INPUT res_glob_t glob: the global handle
@RETURN bool_t: if succeed then return none zero, else return zero.
*/
EXP_API bool_t		gmem_unlock(res_glob_t glob);

#endif

#ifdef XDK_SUPPORT_MEMO_PAGE

/*
@FUNCTION pmem_alloc: alloc page based memoy by page size granularity.
@INPUT dword_t size: the bytes want to alloc. eg: page size is 4096, if request size is 4097, then 4096 * 2 size memory will alloced.
@RETURN void*: the page based memory pointer.
*/
EXP_API void*		pmem_alloc(dword_t size);

/*
@FUNCTION pmem_realloc: realloc page based memoy by page size granularity.
@INPUT void* p: the original page based memory pointer.
@INPUT dword_t size: the bytes want to alloc.
@RETURN void*: the new page based memory pointer.
*/
EXP_API void*		pmem_realloc(void* p, dword_t size);

/*
@FUNCTION pmem_free: free the page based memory.
@INPUT void* p: the page based memory pointer.
@RETURN void: none.
*/
EXP_API void		pmem_free(void* p);

/*
@FUNCTION pmem_size: get the page based memory size.
@INPUT void* p: the page based memory pointer.
@RETURN dword_t: total space size in bytes.
*/
EXP_API dword_t		pmem_size(void* p);

/*
@FUNCTION pmem_lock: lock the page based memory into physical memory
@INPUT void* p: the page based memory pointer.
@RETURN void*: if succeed return memory pointer, failed return NULL.
*/
EXP_API void*		pmem_lock(void* p);

/*
@FUNCTION pmem_unlock: unlock the page based memory, then it can be switched out of physical memory
@INPUT void* p: the page based memory pointer.
@RETURN void: none
*/
EXP_API void		pmem_unlock(void* p);

/*
@FUNCTION pmem_protect: protect or unprotect the page based memory, let it be READONLY or READWRITE
@INPUT void* p: the page based memory pointer.
@INPUT bool_t b: if nonezero protect it, zero unprotect it.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		pmem_protect(void* p, bool_t b);

#endif


#ifdef	__cplusplus
}
#endif

#endif	/*IMPMEM_H */

