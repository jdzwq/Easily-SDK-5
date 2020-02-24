/*************************************************************
	Easily xdl v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	xdlmem.cc | memory implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

#include "xdlmem.h"

#ifdef _DEBUG
#include "xdllink.h"
#endif

#ifdef _DEBUG
link_t memo_trace = {0};
#endif

void*	xmem_alloc(size_t size)
{
	void* p;
	if(size <= 0)
		return NULL;

#ifdef _DEBUG
	p = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size + sizeof(size_t) + sizeof(link_t));
	if(memo_trace.tag)
	{
		((link_t_ptr)p)->tag = lkDebug;
		insert_link_after(&memo_trace,LINK_LAST,(link_t_ptr)p);
	}
	*((size_t*)((BYTE*)p + sizeof(link_t))) = size;
	p = (void*)((BYTE*)p + sizeof(size_t) + sizeof(link_t));
#else
	p = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size + sizeof(size_t));
	*((size_t*)p) = size;
	p = (void*)((BYTE*)p + sizeof(size_t));
#endif
	
	return p;
}

void*	xmem_realloc(void* p,size_t size)
{
	if(size <= 0)
	{
		xmem_free(p);
		return NULL;
	}

	if(p)
#ifdef _DEBUG
	{
		p = (void*)((BYTE*)p - sizeof(size_t) - sizeof(link_t));
		if(memo_trace.tag)
		{
			delete_link(&memo_trace,(link_t_ptr)p);
		}
	}
#else
		p = (void*)((BYTE*)p - sizeof(size_t));
#endif

	if(p)
#ifdef _DEBUG
		p = HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,p,size + sizeof(size_t) + sizeof(link_t));
#else
		p = HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,p,size + sizeof(size_t));
#endif
	else
#ifdef _DEBUG
		p = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size + sizeof(size_t) + sizeof(link_t));
#else
		p = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size + sizeof(size_t));
#endif

#ifdef _DEBUG
	if(memo_trace.tag)
	{
		((link_t_ptr)p)->tag = lkDebug;
		insert_link_after(&memo_trace,LINK_LAST,(link_t_ptr)p);
	}
	*((size_t*)((BYTE*)p + sizeof(link_t))) = size;
	p = (void*)((BYTE*)p + sizeof(size_t) + sizeof(link_t));
#else
	*((size_t*)p) = size;
	p = (void*)((BYTE*)p + sizeof(size_t));
#endif

	return p;
}

void xmem_zero(void* p)
{
	size_t size;

	if(!p)
		return;

	size = *(size_t*)((BYTE*)p - sizeof(size_t));

	ZeroMemory(p,size);
}

void* xmem_clone(const void* src,size_t bytes)
{
	void* p;

	if(!src || bytes <= 0)
		return NULL;

	p = xmem_alloc(bytes);

	memcpy(p,src,bytes);

	return p;
}

size_t	xmem_size(void* p)
{
	if(!p)
		return 0;

	p = (void*)((BYTE*)p - sizeof(size_t));
	return *((size_t*)p);
}

void	xmem_free(void* p)
{
	if(p == NULL)
		return;

#ifdef _DEBUG
	p = (void*)((BYTE*)p - sizeof(size_t) - sizeof(link_t));
	if(memo_trace.tag)
	{
		delete_link(&memo_trace,(link_t_ptr)p);
	}
#else
	p = (void*)((BYTE*)p - sizeof(size_t));
#endif

	HeapFree(GetProcessHeap(),0,p);
}

void xmem_swap(void* p,size_t n)
{
	wchar_t *pw;

	pw = (wchar_t*)p;
	n /= 2;
	while (n--)
	{
		*pw = SWAPWCHAR(*pw);
		pw ++;
	}
}

#ifdef _DEBUG
void xmem_debug()
{
	if(!memo_trace.tag)
		init_root_link(&memo_trace);
}

void xmem_dump()
{
	link_t_ptr plk;
	size_t size;

	if(!memo_trace.tag)
		return;

	plk = get_first_link(&memo_trace);
	while(plk)
	{
		size = *((size_t*)((BYTE*)plk + sizeof(link_t)));

		printf("xmem_dump[%d %s]\n",size,(BYTE*)((BYTE*)plk + sizeof(link_t) + sizeof(size_t)));
		plk = get_next_link(plk);
	}
}
#endif