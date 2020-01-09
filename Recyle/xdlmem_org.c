/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc memory document

	@module	xdlmem.c | memory implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#include "xdlmem.h"
#include "xdlinit.h"
#include "xdlerr.h"

void* xmem_alloc(dword_t size)
{
	if_heap_t* pif;
	void* p;

	if(size <= 0)
		return NULL;

	pif = THREAD_HEAP_INTERFACE;

#ifdef _DEBUG
	if (pif)
	{
		p = (*pif->pf_alloc)(pif->if_hand, size + sizeof(dword_t) + sizeof(link_t));

		XDL_ASSERT(p != NULL);

		((link_t_ptr)p)->tag = lkDebug;
		insert_link_after(&pif->if_dump, LINK_LAST, (link_t_ptr)p);
	}
	else
	{
		p = calloc(size + sizeof(dword_t) + sizeof(link_t), 1);

		XDL_ASSERT(p != NULL);

		((link_t_ptr)p)->tag = lkDebug;
	}
	
	*((dword_t*)((byte_t*)p + sizeof(link_t))) = size;
	p = (void*)((byte_t*)p + sizeof(dword_t) + sizeof(link_t));
#else
	if (pif)
	{
		p = (*pif->pf_alloc)(pif->if_hand, size + sizeof(dword_t));
	}else
	{
		p = calloc(size + sizeof(dword_t),1);
	}

	XDL_ASSERT(p != NULL);

	*((dword_t*)p) = size;
	p = (void*)((byte_t*)p + sizeof(dword_t));
#endif

	return p;
}

void* xmem_realloc(void* p,dword_t size)
{
	if_heap_t* pif;

	if(size <= 0)
	{
		xmem_free(p);
		return NULL;
	}

	pif = THREAD_HEAP_INTERFACE;

	if (p)
	{
#ifdef _DEBUG
		p = (void*)((byte_t*)p - sizeof(dword_t) - sizeof(link_t));

		if (pif)
		{
			delete_link(&pif->if_dump, (link_t_ptr)p);
		}
#else
		p = (void*)((byte_t*)p - sizeof(dword_t));
#endif
	}

	if (p)
	{
#ifdef _DEBUG
		if (pif)
		{
			p = (*pif->pf_realloc)(pif->if_hand, p, size + sizeof(dword_t) + sizeof(link_t));
		}
		else
		{
			p = realloc(p, size + sizeof(dword_t) + sizeof(link_t));
		}
		XDL_ASSERT(p != NULL);
#else
		if (pif)
		{
			p = (*pif->pf_realloc)(pif->if_hand, p, size + sizeof(dword_t));
		}
		else
		{
			p = realloc(p, size + sizeof(dword_t));
		}
		XDL_ASSERT(p != NULL);
#endif
	}
	else
	{
#ifdef _DEBUG
		if (pif)
		{
			p = (*pif->pf_alloc)(pif->if_hand, size + sizeof(dword_t) + sizeof(link_t));
		}
		else
		{
			p = calloc(size + sizeof(dword_t) + sizeof(link_t), 1);
		}
		XDL_ASSERT(p != NULL);
#else
		if (pif)
		{
			p = (*pif->pf_alloc)(pif->if_hand, size + sizeof(dword_t));
		}
		else
		{
			p = calloc(size + sizeof(dword_t), 1);
		}
		XDL_ASSERT(p != NULL);
#endif
	}

#ifdef _DEBUG
	if (pif)
	{
		((link_t_ptr)p)->tag = lkDebug;
		insert_link_after(&pif->if_dump, LINK_LAST, (link_t_ptr)p);
	}
	else
	{
		((link_t_ptr)p)->tag = lkDebug;
	}

	*((dword_t*)((byte_t*)p + sizeof(link_t))) = size;
	p = (void*)((byte_t*)p + sizeof(dword_t) + sizeof(link_t));
#else
	*((dword_t*)p) = size;
	p = (void*)((byte_t*)p + sizeof(dword_t));
#endif

	return p;
}

void xmem_zero(void* p)
{
	dword_t size;

	if(!p)
		return;

#ifdef _DEBUG
	XDL_ASSERT(((link_t_ptr)((byte_t*)p - sizeof(dword_t) - sizeof(link_t)))->tag == lkDebug);
#endif

	size = *(dword_t*)((byte_t*)p - sizeof(dword_t));
	memset(p,0,size);
}

void* xmem_clone(void* src,dword_t bytes)
{
	void* p;

	if(!src || bytes <= 0)
		return NULL;

	p = xmem_alloc(bytes);

	memcpy(p,src,bytes);

	return p;
}

void xmem_copy(void* dest, void* src, dword_t n)
{
	dword_t dw;

	dw = xmem_size(dest);
	n = (n < dw) ? n : dw;

	memcpy(dest, src, n);
}

dword_t	xmem_size(void* p)
{
	if(!p)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(((link_t_ptr)((byte_t*)p - sizeof(dword_t) - sizeof(link_t)))->tag == lkDebug);
#endif

	p = (void*)((byte_t*)p - sizeof(dword_t));
	return *((dword_t*)p);
}

void xmem_free(void* p)
{
	if_heap_t* pif;

	if(p == NULL)
		return;

	pif = THREAD_HEAP_INTERFACE;

#ifdef _DEBUG
	p = (void*)((byte_t*)p - sizeof(dword_t) - sizeof(link_t));
	XDL_ASSERT(((link_t_ptr)p)->tag == lkDebug);

	if (pif)
	{
		delete_link(&pif->if_dump, (link_t_ptr)p);
	}
#else
	p = (void*)((byte_t*)p - sizeof(dword_t));
#endif

	if (pif)
	{
		(*pif->pf_free)(pif->if_hand, p);
	}
	else
	{
		free(p);
	}
}

#ifdef _DEBUG
void xmem_assert(void* p)
{
	if (!p)
		return;

	p = (void*)((byte_t*)p - sizeof(dword_t) - sizeof(link_t));

	XDL_ASSERT(((link_t_ptr)p)->tag == lkDebug);
}
#endif

res_glob_t	system_alloc(dword_t size)
{
	if_virmem_t *pif;
	res_glob_t gb;

	pif = PROCESS_VIRMEM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	gb = (*pif->pf_glob_alloc)(size);

	XDL_ASSERT(gb != NULL);

	return gb;
}

res_glob_t	system_realloc(res_glob_t glob, dword_t size)
{
	if_virmem_t *pif;
	res_glob_t gb;

	pif = PROCESS_VIRMEM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	gb = (*pif->pf_glob_realloc)(glob, size);

	XDL_ASSERT(gb != NULL);

	return gb;
}

void system_free(res_glob_t glob)
{
	if_virmem_t *pif;

	pif = PROCESS_VIRMEM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_glob_free)(glob);
}

dword_t system_size(res_glob_t glob)
{
	if_virmem_t *pif;

	pif = PROCESS_VIRMEM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (dword_t)(*pif->pf_glob_size)(glob);
}

void* system_lock(res_glob_t glob)
{
	if_virmem_t *pif;

	pif = PROCESS_VIRMEM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_glob_lock)(glob);
}

bool_t system_unlock(res_glob_t glob)
{
	if_virmem_t *pif;

	pif = PROCESS_VIRMEM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_glob_unlock)(glob);
}