/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory document

	@module	impmem.c | implement file

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

#include "impmem.h"
#include "imperr.h"
#include "impassert.h"
#include "xdlinit.h"

void* xmem_alloc(dword_t size)
{
	void* p;

#ifdef XDK_SUPPORT_MEMO_HEAP
	if_zone_t* pif;
	if_memo_t* piv;

	if(!size)
		return NULL;

	pif = THREAD_ZONE_INTERFACE;
	piv = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL && piv != NULL);

#ifdef XDL_SUPPORT_MEMO_DUMP
	p = (*piv->pf_heap_alloc)(pif->if_heap, size + sizeof(dword_t) + sizeof(link_t));

	XDL_ASSERT(p != NULL);

	((link_t_ptr)p)->tag = lkDebug;
	insert_link_after(&pif->if_dump, LINK_LAST, (link_t_ptr)p);

	*((dword_t*)((byte_t*)p + sizeof(link_t))) = size;
	p = (void*)((byte_t*)p + sizeof(dword_t) + sizeof(link_t));
#else
	p = (*piv->pf_heap_alloc)(pif->if_heap, size);
	
	XDL_ASSERT(p != NULL);

#endif //XDL_SUPPORT_MEMO_DUMP

#else

#ifdef XDK_SUPPORT_MEMO_LOCAL
	if_memo_t* piv;

	piv = PROCESS_MEMO_INTERFACE;
	XDL_ASSERT(piv != NULL);

	p = (*piv->pf_local_alloc)(size);

	XDL_ASSERT(p != NULL);

#else
	p = calloc(size, 1);

	XDL_ASSERT(p != NULL);
#endif //XDK_SUPPORT_MEMO_LOCAL

#endif //XDK_SUPPORT_MEMO_HEAP

	return p;
}

void* xmem_realloc(void* p, dword_t size)
{
	if (!size)
	{
		xmem_free(p);
		return NULL;
	}

	if (!p)
	{
		return xmem_alloc(size);
	}

#ifdef XDK_SUPPORT_MEMO_HEAP
	if_zone_t* pif;
	if_memo_t* piv;

	pif = THREAD_ZONE_INTERFACE;
	piv = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL && piv != NULL);

#ifdef XDL_SUPPORT_MEMO_DUMP
	p = (void*)((byte_t*)p - sizeof(dword_t) - sizeof(link_t));
	delete_link(&pif->if_dump, (link_t_ptr)p);

	p = (*piv->pf_heap_realloc)(pif->if_heap, p, size + sizeof(dword_t) + sizeof(link_t));
	XDL_ASSERT(p != NULL);

	((link_t_ptr)p)->tag = lkDebug;
	insert_link_after(&pif->if_dump, LINK_LAST, (link_t_ptr)p);

	*((dword_t*)((byte_t*)p + sizeof(link_t))) = size;
	p = (void*)((byte_t*)p + sizeof(dword_t) + sizeof(link_t));
#else
	p = (*piv->pf_heap_realloc)(pif->if_heap, p, size);
#endif

#else

#ifdef XDK_SUPPORT_MEMO_LOCAL
	if_memo_t* piv;

	piv = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(piv != NULL);

	p = (*piv->pf_local_realloc)(p, size);
	XDL_ASSERT(p != NULL);
#else
	p = (p)? realloc(p, size) : calloc(size, 1);
	XDL_ASSERT(p != NULL);
#endif //XDK_SUPPORT_MEMO_LOCAL

#endif //XDK_SUPPORT_MEMO_HEAP

	return p;
}

void xmem_free(void* p)
{
#ifdef XDK_SUPPORT_MEMO_HEAP

	if_zone_t* pif;
	if_memo_t* piv;

	pif = THREAD_ZONE_INTERFACE;
	piv = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL && piv != NULL);

	if (!p) return;

#ifdef XDL_SUPPORT_MEMO_DUMP
	p = (void*)((byte_t*)p - sizeof(dword_t) - sizeof(link_t));
	XDL_ASSERT(((link_t_ptr)p)->tag == lkDebug);

	delete_link(&pif->if_dump, (link_t_ptr)p);
#endif

	(*piv->pf_heap_free)(pif->if_heap, p);
#else

#ifdef XDK_SUPPORT_MEMO_LOCAL
	if_memo_t* piv;

	piv = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(piv != NULL);

	if (!p) return;

	(*piv->pf_local_free)(p);

#else

	if (!p) return;

	free(p);
#endif //XDK_SUPPORT_MEMO_LOCAL

#endif //XDK_SUPPORT_MEMO_HEAP
}

void xmem_zero(void* p, dword_t size)
{
	if(!p || !size)
		return;

//#ifdef XDL_SUPPORT_MEMO_DUMP
//	XDL_ASSERT(((link_t_ptr)((byte_t*)p - sizeof(dword_t) - sizeof(link_t)))->tag == lkDebug);
//#endif

	while (size--)
	{
		*(byte_t*)p = 0;
		p = (byte_t*)p + 1;
	}
}

void xmem_set(void* p, byte_t c, dword_t size)
{
	if (!p || !size)
		return;

	while (size--)
	{
		*(byte_t*)p = c;
		p = (byte_t*)p + 1;
	}
}

void* xmem_clone(void* src,dword_t bytes)
{
	void* p;

	if(!src || bytes <= 0)
		return NULL;

	p = xmem_alloc(bytes);

	xmem_copy(p,src,bytes);

	return p;
}

void xmem_copy(void* dest, void* src, dword_t size)
{
	if (!dest || !src)
		return;

	while (size--)
	{
		*(byte_t*)dest = *(byte_t*)src;
		dest = (byte_t*)dest + 1;
		src = (byte_t*)src + 1;
	}
}

int xmem_comp(void* mem1, dword_t len1, void* mem2, dword_t len2)
{
	byte_t *p1, *p2;
	dword_t len;

	if ((!mem1 || !len1) && (!mem2 || !len2))
		return 0;
	else if ((!mem1 || !len1) && mem2)
		return -1;
	else if (mem1 && (!mem2 || !len2))
		return 1;

	p1 = (byte_t*)mem1;
	p2 = (byte_t*)mem2;

	len = (len1 < len2) ? len1 : len2;
	while (len--)
	{
		if (*p1 > *p2)
			return 1;
		else if (*p1 < *p2)
			return -1;

		p1++; p2++;
	}
	
	if (len1 == len2)
		return 0;
	else
		return (len1 < len2) ? -1 : 1;
}

void xmem_move(void* p, dword_t len, int off)
{
	byte_t *p1, *p2;

	if (!p || !off)
		return;

	if (off > 0)
	{
		p1 = (byte_t*)p + len - 1 + off;
		p2 = (byte_t*)p + len - 1;

		while (len--)
		{
			*p1 = *p2;
			p1--;
			p2--;
		}
	}
	else
	{
		p1 = (byte_t*)p + off;
		p2 = (byte_t*)p;

		while (len--)
		{
			*p1 = *p2;
			p1++;
			p2++;
		}
	}
}

#ifdef XDL_SUPPORT_MEMO_DUMP
void xmem_assert(void* p)
{
	if (!p)
		return;

	p = (void*)((byte_t*)p - sizeof(dword_t) - sizeof(link_t));

	XDL_ASSERT(((link_t_ptr)p)->tag == lkDebug);
}

dword_t	xmem_size(void* p)
{
	if (!p)
		return 0;

	XDL_ASSERT(((link_t_ptr)((byte_t*)p - sizeof(dword_t) - sizeof(link_t)))->tag == lkDebug);

	p = (void*)((byte_t*)p - sizeof(dword_t));
	return *((dword_t*)p);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef XDK_SUPPORT_MEMO_GLOB

res_glob_t	gmem_alloc(dword_t size)
{
	if_memo_t *pif;
	res_glob_t gb;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	gb = (*pif->pf_glob_alloc)(size);

	XDL_ASSERT(gb != NULL);

	return gb;
}

res_glob_t	gmem_realloc(res_glob_t glob, dword_t size)
{
	if_memo_t *pif;
	res_glob_t gb;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	gb = (*pif->pf_glob_realloc)(glob, size);

	XDL_ASSERT(gb != NULL);

	return gb;
}

void gmem_free(res_glob_t glob)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_glob_free)(glob);
}

dword_t gmem_size(res_glob_t glob)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (dword_t)(*pif->pf_glob_size)(glob);
}

void* gmem_lock(res_glob_t glob)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_glob_lock)(glob);
}

bool_t gmem_unlock(res_glob_t glob)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_glob_unlock)(glob);
}

#endif 

//////////////////////////////////////////////////////////////////////////////////////////
#ifdef XDK_SUPPORT_MEMO_PAGE

void* pmem_alloc(dword_t size)
{
	if_memo_t *pif;
	void* p;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	p = (*pif->pf_page_alloc)(size);

	XDL_ASSERT(p != NULL);

	return p;
}

void* pmem_realloc(void* p, dword_t size)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	p = (*pif->pf_page_realloc)(p, size);

	XDL_ASSERT(p != NULL);

	return p;
}

void pmem_free(void* p)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_page_free)(p);
}

dword_t	pmem_size(void* p)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (dword_t)(*pif->pf_page_size)(p);
}

void* pmem_lock(void* p)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_page_lock)(p);
}

void pmem_unlock(void* p)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_page_unlock)(p);
}

bool_t	pmem_protect(void* p, bool_t b)
{
	if_memo_t *pif;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_page_protect)(p, b);
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////
#ifdef XDK_SUPPORT_MEMO_CACHE

typedef struct _cache_t{
	xhand_head head;		//reserved for xhand_t

	void* cache;
	dword_t write_bytes;
	dword_t read_bytes;
}cache_t;


xhand_t vmem_open()
{
	cache_t* ppi;
	if_memo_t* pif;
	void* bh;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	bh = (*pif->pf_cache_open)();

	if (!bh)
	{
		set_system_error(_T("pf_cache_open"));
		return NULL;
	}

	ppi = (cache_t*)xmem_alloc(sizeof(cache_t));
	ppi->head.tag = _HANDLE_CACHE;
	ppi->cache = bh;
	ppi->read_bytes = 0;
	ppi->write_bytes = 0;

	return (xhand_t)ppi;
}

void* vmem_handle(xhand_t cache)
{
	cache_t* ppi = (cache_t*)cache;

	XDL_ASSERT(cache && cache->tag == _HANDLE_CACHE);

	return ppi->cache;
}

void vmem_close(xhand_t cache)
{
	cache_t* ppi = (cache_t*)cache;
	if_memo_t* pif;

	XDL_ASSERT(cache && cache->tag == _HANDLE_CACHE);

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_cache_close)(ppi->cache);

	xmem_free(ppi);
}

bool_t vmem_read(xhand_t cache, byte_t* buf, dword_t* pb)
{
	cache_t* ppt = (cache_t*)cache;
	if_memo_t* pif;
	size_t size;
	bool_t rt;

	XDL_ASSERT(cache && cache->tag == _HANDLE_CACHE);

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pb;
	rt = (*pif->pf_cache_read)(ppt->cache, ppt->read_bytes, buf, size, &size);
	if (rt)
	{
		ppt->read_bytes += (dword_t)size;
		*pb = (dword_t)size;
	}
	else
	{
		set_system_error(_T("pf_cache_read"));
		*pb = 0;
	}

	return rt;
}

bool_t vmem_write(xhand_t cache, const byte_t* buf, dword_t* pb)
{
	cache_t* ppt = (cache_t*)cache;
	if_memo_t* pif;
	size_t size;
	bool_t rt;

	XDL_ASSERT(cache && cache->tag == _HANDLE_CACHE);

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pb;
	rt = (*pif->pf_cache_write)(ppt->cache, ppt->write_bytes, (void*)buf, size, &size);
	if (rt)
	{
		ppt->write_bytes += (dword_t)size;
		*pb = (dword_t)size;
	}
	else
	{
		set_system_error(_T("pf_cache_write"));
		*pb = 0;
	}

	return rt;
}

#endif //XDK_SUPPORT_MEMO_CACHE
