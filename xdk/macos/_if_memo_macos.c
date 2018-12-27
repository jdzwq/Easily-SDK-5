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
    malloc_zone_t* pmz;
    
    pmz = malloc_default_zone();
    
	return (res_heap_t)pmz;
}

res_heap_t _heapo_create(void)
{
    malloc_zone_t* pmz;
    
    pmz = malloc_create_zone(PAGE_SIZE, 0);
    
    return (res_heap_t)pmz;
}

void  _heapo_destroy(res_heap_t heap)
{
    malloc_zone_t* pmz = (malloc_zone_t*)heap;
    
    malloc_destroy_zone(pmz);
}

void* _heapo_alloc(res_heap_t heap, size_t size)
{
    malloc_zone_t* pmz = (malloc_zone_t*)heap;

    return malloc_zone_calloc(pmz, 1, size);
}

void* _heapo_realloc(res_heap_t heap, void* p, size_t size)
{
    malloc_zone_t* pmz = (malloc_zone_t*)heap;
    
    return malloc_zone_realloc(pmz, p, size);
}

void _heapo_zero(res_heap_t heap, void* p, size_t size)
{
    //malloc_zone_t* pmz = (malloc_zone_t*)heap;
    
    memset(p,0,size);
}

void _heapo_free(res_heap_t heap, void* p)
{
    malloc_zone_t* pmz = (malloc_zone_t*)heap;

    malloc_zone_free(pmz, p);
}

void _heapo_clean(res_heap_t heap)
{
    malloc_zone_t* pmz = (malloc_zone_t*)heap;
    
    malloc_zone_pressure_relief(pmz, 0);
}
#endif
/******************************************************************************************/
#ifdef XDK_SUPPORT_MEMO_GLOB
res_glob_t _globo_alloc(size_t size)
{
    SDK_UNSUPPORT_ERROR;
    
	return NULL;
}

res_glob_t _globo_realloc(res_glob_t glob, size_t size)
{
    SDK_UNSUPPORT_ERROR;
    
	return NULL;
}

void _globo_free(res_glob_t glob)
{
    SDK_UNSUPPORT_ERROR;
    
	return;
}

size_t _globo_size(res_glob_t glob)
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
void* _local_alloc(size_t size)
{
    return calloc(1, size);
}

void* _local_realloc(void* p, size_t size)
{
    return realloc(p, size);
}

void _local_free(void* p)
{
    free(p);
}

void _async_alloc_lapp(async_t* pas)
{
    pas->type = ASYNC_EVENT;
    pas->lapp = (void*)_local_alloc(sizeof(OVERLAPPED));
}

void _async_free_lapp(async_t* pas)
{
    if(pas->lapp)
        _local_free(pas->lapp);
    
    pas->lapp = NULL;
}
#endif
/******************************************************************************/
#ifdef XDK_SUPPORT_MEMO_PAGE
void* _paged_alloc(size_t size)
{
    void* p = NULL;
    size_t dw;
    
    dw = size / PAGE_SIZE;
    if (size % PAGE_SIZE)
        dw++;
    
    return (posix_memalign(&p, PAGE_SIZE, dw * PAGE_SIZE) < 0)? NULL : p;
}

void* _paged_realloc(void* p, size_t size)
{
    void* pn;
    size_t n;
    
    if (!p)
        return _paged_alloc(size);
    
    n = malloc_size(p);
    
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

size_t _paged_size(void* p)
{
    return (size_t)malloc_size(p);
}

void* _paged_lock(void* p)
{
    size_t n;
    
    n = malloc_size(p);

    mlock(p, n);
    
	return p;
}

void _paged_unlock(void* p)
{
    size_t n;
    
    n = malloc_size(p);
    
    munlock(p, n);
}

bool_t _paged_protect(void* p)
{
    size_t n;
    
    n = malloc_size(p);

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

bool_t _cache_write(void* fh, size_t offset, void* buf, size_t size, size_t* pb)
{
    memcpy((void*)((char*)fh + offset), buf, size);
    if(pb)
        *pb = size;
    
    return 1;
}

bool_t _cache_read(void* fh, size_t offset, void* buf, size_t size, size_t* pb)
{
    memcpy(buf, (void*)((char*)fh + offset), size);
    if(pb)
        *pb = size;
    
    return 1;
}
#endif

#endif //XDK_SUPPORT_MEMO