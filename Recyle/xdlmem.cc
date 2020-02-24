
/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc xdl memory function

	@module	xdlmem.c | xdl memory implement file

	@devnote 张文�?2003.01 - 2007.12 v3.0
			 张文�?2008.01 - 2009.12 v4.0
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlmem.h"


void*	xmem_alloc(size_t size)
{
	void* p;
	if(size <= 0)
		return NULL;

#ifdef _WINDOWS
	p = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size + sizeof(size_t));
#else
	p = calloc(1,size + sizeof(size_t));
#endif
	*((size_t*)p) = size;
	p = (void*)((char*)p + sizeof(size_t));
	return p;
}

void*	xmem_realloc(void* p,size_t size)
{
	assert(size >= 0);
	if(!size)
		return NULL;

	if(p)
		p = (void*)((char*)p - sizeof(size_t));

#ifdef _WINDOWS
	if(!p)
		p = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size + sizeof(size_t));
	else
		p = HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,p,size + sizeof(size_t));
#else
	if(!p)
		p = calloc(1,size + sizeof(size_t));
	else
		p = realloc(p,size + sizeof(size_t));
#endif
	*((size_t*)p) = size;
	p = (void*)((char*)p + sizeof(size_t));
	
	return p;
}

void xmem_zero(void* p)
{
	size_t size;

	if(!p)
		return;

	size = *(size_t*)((char*)p - sizeof(size_t));

#ifdef _WINDOWS
	ZeroMemory(p,size);
#else
	memset(p,0,size);
#endif
}

size_t	xmem_size(void* p)
{
	if(!p)
		return 0;

	p = (void*)((char*)p - sizeof(size_t));
	return *((size_t*)p);
}

void	xmem_free(void* p)
{
	if(p == NULL)
		return;

	p = (void*)((char*)p - sizeof(size_t));
#ifdef _WINDOWS
	HeapFree(GetProcessHeap(),0,p);
#else
	free(p);
#endif
}

