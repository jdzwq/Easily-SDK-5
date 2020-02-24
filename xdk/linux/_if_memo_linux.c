/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc global/heap/virtual memory system call document

	@module	_if_memo.c | memory system call macos implement file

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


#include "xdkiml.h"


#ifdef XDK_SUPPORT_MEMO
/******************************************************************************/
#ifdef XDK_SUPPORT_MEMO_HEAP
res_heap_t _process_heapo(void)
{
    SDK_UNSUPPORT_ERROR;
    
	return (res_heap_t)NULL;
}

res_heap_t _heapo_create(void)
{
    SDK_UNSUPPORT_ERROR;
    
	return (res_heap_t)NULL;
}

void  _heapo_destroy(res_heap_t heap)
{
    SDK_UNSUPPORT_ERROR;
}

void* _heapo_alloc(res_heap_t heap, dword_t size)
{
    SDK_UNSUPPORT_ERROR;
    
	return (res_heap_t)NULL;
}

void* _heapo_realloc(res_heap_t heap, void* p, dword_t size)
{
    SDK_UNSUPPORT_ERROR;
    
	return (res_heap_t)NULL;
}

void _heapo_zero(res_heap_t heap, void* p, dword_t size)
{
    SDK_UNSUPPORT_ERROR;
}

void _heapo_free(res_heap_t heap, void* p)
{
    SDK_UNSUPPORT_ERROR;
}

void _heapo_clean(res_heap_t heap)
{
    SDK_UNSUPPORT_ERROR;
}
#endif
/******************************************************************************************/
#ifdef XDK_SUPPORT_MEMO_GLOB

res_glob_t _globo_alloc(dword_t size)
{
    SDK_UNSUPPORT_ERROR;
    
	return NULL;
}

res_glob_t _globo_realloc(res_glob_t glob, dword_t size)
{
    SDK_UNSUPPORT_ERROR;
    
	return NULL;
}

void _globo_free(res_glob_t glob)
{
    SDK_UNSUPPORT_ERROR;
    
	return;
}

dword_t _globo_size(res_glob_t glob)
{
    SDK_UNSUPPORT_ERROR;
    
	return 0;
}

void* _globo_lock(res_glob_t glob)
{
    SDK_UNSUPPORT_ERROR;
    
	return NULL;
}

bool_t _globo_unlock(res_glob_t glob)
{
    SDK_UNSUPPORT_ERROR;
    
	return 0;
}
#endif
/*****************************************************************************************/
#ifdef XDK_SUPPORT_MEMO_LOCAL
void* _local_alloc(dword_t size)
{
    return calloc(1, (size_t)size);
}

void* _local_realloc(void* p, dword_t size)
{
    return realloc(p, (size_t)size);
}

void _local_free(void* p)
{
    free(p);
}

#endif
/******************************************************************************/
#ifdef XDK_SUPPORT_MEMO_PAGE
void* _paged_alloc(dword_t size)
{
    void* p = NULL;
    dword_t dw;
    
    dw = size / PAGE_SIZE;
    if (size % PAGE_SIZE)
        dw++;
    
    return (posix_memalign(&p, PAGE_SIZE, (size_t)(dw * PAGE_SIZE)) < 0)? NULL : p;
}

void* _paged_realloc(void* p, dword_t size)
{
    void* pn;
    dword_t n;
    
    if (!p)
        return _paged_alloc(size);
    
    n = (dword_t)malloc_usable_size(p);
    
    if (n < size)
    {
        pn = _paged_alloc(size);
        
        n = _paged_size(p);
        n = (n < size) ? n : size;
        
        memcpy(pn, p, n);
        
        _paged_free(p);
        return pn;
    }
    else
    {
        memset((void*)((char*)p + size), 0, n - size);
        return p;
    }
}

void _paged_free(void* p)
{
    free(p);
}

dword_t _paged_size(void* p)
{
    return (dword_t)malloc_usable_size(p);
}

void* _paged_lock(void* p)
{
    size_t n;
    
    n = malloc_usable_size(p);

    mlock(p, n);
    
	return p;
}

void _paged_unlock(void* p)
{
    size_t n;
    
    n = malloc_usable_size(p);
    
    munlock(p, n);
}

bool_t _paged_protect(void* p, bool_t b)
{
    size_t n;
    
    n = malloc_usable_size(p);

    mprotect(p, n, PROT_READ);
	
	return 0;
}
#endif

/*****************************************************************************************/
#ifdef XDK_SUPPORT_MEMO_CACHE

void* _cache_open()
{
    void* p;
    
    p = mmap(NULL, PAGE_SPACE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    return (p == MAP_FAILED)? NULL : p;
}

void _cache_close(void* fh)
{
	munmap(fh, PAGE_SPACE);
}

bool_t _cache_write(void* fh, dword_t hoff, dword_t loff, void* buf, dword_t size, dword_t* pb)
{
    size_t off;

    off = MAKESIZE(loff, hoff);

    memcpy((void*)((char*)fh + off), buf, (size_t)size);

    if(pb)
        *pb = size;
    
    return 1;
}

bool_t _cache_read(void* fh, dword_t hoff, dword_t loff, void* buf, dword_t size, dword_t* pb)
{
    size_t off;

    off = MAKESIZE(loff, hoff);

    memcpy(buf, (void*)((char*)fh + off), (size_t)size);
    
    if(pb)
        *pb = size;
    
    return 1;
}
#endif

#endif //XDK_SUPPORT_MEMO