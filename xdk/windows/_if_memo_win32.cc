/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc global/heap/virtual memory system call document

	@module	_if_memo.c | memory system call windows implement file

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
	return GetProcessHeap();
}

res_heap_t _heapo_create(void)
{
	return HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
}

void  _heapo_destroy(res_heap_t heap)
{
	BOOL b;
	b = HeapDestroy(heap);
}

void* _heapo_alloc(res_heap_t heap, size_t size)
{
	return HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
}

void* _heapo_realloc(res_heap_t heap, void* p, size_t size)
{
	if(!p)
		return HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
	else
		return HeapReAlloc(heap, HEAP_ZERO_MEMORY, p, size);
}

void _heapo_zero(res_heap_t heap, void* p, size_t size)
{
	if (p)
	{
#ifdef WINCE
		ZeroMemory(p, size);	
#else
		SecureZeroMemory(p, size);
#endif
	}
}

void _heapo_free(res_heap_t heap, void* p)
{
	BOOL b;
	b = HeapFree(heap, 0, p);
}

void _heapo_clean(res_heap_t heap)
{
	//PROCESS_HEAP_ENTRY Entry = { 0 };
	//BOOL b;
	SIZE_T s = 0;

	/*Entry.lpData = NULL;
	while (HeapWalk(heap, &Entry) != FALSE) 
	{
		if ((Entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) != 0) 
		{
			if ((Entry.wFlags & PROCESS_HEAP_ENTRY_MOVEABLE) != 0) 
			{
			}
			if ((Entry.wFlags & PROCESS_HEAP_ENTRY_DDESHARE) != 0) 
			{
			}
		}
		else if ((Entry.wFlags & PROCESS_HEAP_REGION) != 0) 
		{
			b = HeapFree(heap, HEAP_NO_SERIALIZE, Entry.Region.lpFirstBlock);
		}
		else if ((Entry.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE) != 0) 
		{
		}
	}*/
#ifdef WINCE
	return;
#else
	s = HeapCompact(heap, HEAP_NO_SERIALIZE);
#endif
}
#endif
/*****************************************************************************************/
#ifdef XDK_SUPPORT_MEMO_LOCAL
void* _local_alloc(size_t size)
{
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
}

void* _local_realloc(void* p, size_t size)
{
	if (!p)
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	else
		return HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, p, size);
}

void _local_free(void* p)
{
	BOOL b;
	b = HeapFree(GetProcessHeap(), 0, p);
}

void _async_alloc_lapp(async_t* pas)
{
	LPOVERLAPPED lp;

	lp = (LPOVERLAPPED)_local_alloc(sizeof(OVERLAPPED));

	lp->hEvent = CreateEvent(NULL, 1, 0, NULL);
	pas->lapp = (void*)lp;
	pas->type = ASYNC_EVENT;
}

void _async_free_lapp(async_t* pas)
{
	LPOVERLAPPED lp;

	if (!pas->lapp)
		return;

	lp = (LPOVERLAPPED)pas->lapp;

	if (lp->hEvent)
		CloseHandle(lp->hEvent);

	_local_free(pas->lapp);
	pas->lapp = NULL;
}
#endif
/******************************************************************************************/
#ifdef XDK_SUPPORT_MEMO_GLOB
res_glob_t _globo_alloc(size_t size)
{
	return GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT, size);
}

res_glob_t _globo_realloc(res_glob_t glob, size_t size)
{
	return GlobalReAlloc(glob, (size_t)size, GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
}

void _globo_free(res_glob_t glob)
{
	GlobalFree(glob);
}

size_t _globo_size(res_glob_t glob)
{
	return (size_t)GlobalSize(glob);
}

void* _globo_lock(res_glob_t glob)
{
	return GlobalLock(glob);
}

bool_t _globo_unlock(res_glob_t glob)
{
	return (bool_t)GlobalUnlock(glob);
}
#endif
/******************************************************************************/
#ifdef XDK_SUPPORT_MEMO_PAGE
void* _paged_alloc(size_t size)
{
	DWORD dw;
	void* p;

	dw = (DWORD)(size / PAGE_SIZE);
	if (size % PAGE_SIZE)
		dw++;
	
	p = VirtualAlloc(NULL, dw * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (p)
	{
		ZeroMemory(p, dw * PAGE_SIZE);
	}

	return p;
}

void* _paged_realloc(void* p, size_t size)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	void* pnew;
	size_t n;
	bool_t b;

	if (!p)
		return _paged_alloc(size);

	VirtualQuery(p, &mbi, sizeof(mbi));

	if (mbi.RegionSize < size)
	{
		pnew = _paged_alloc(size);

		n = _paged_size(p);
		n = (n < size) ? n : size;

		CopyMemory(pnew, p, n);

		_paged_free(p);
		return pnew;
	}
	else
	{
		n = size / PAGE_SIZE;
		if (size % PAGE_SIZE)
			n++;

		if (mbi.RegionSize > n * PAGE_SIZE)
		{
			b = VirtualFree((LPBYTE)mbi.AllocationBase + n * PAGE_SIZE, mbi.RegionSize - n * PAGE_SIZE, MEM_DECOMMIT);
		}
		return mbi.AllocationBase;
	}
}

void _paged_free(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	bool_t b;

	if (!p)
		return;

	VirtualQuery(p, &mbi, sizeof(mbi));

	if (mbi.State == MEM_FREE)
	{
		return;
	}
	else if (mbi.State == MEM_COMMIT)
	{
		b = VirtualFree(mbi.AllocationBase, mbi.RegionSize, MEM_DECOMMIT);
		b = VirtualFree(mbi.AllocationBase, 0, MEM_RELEASE);
	}
	else if (mbi.State == MEM_RESERVE)
	{
		b = VirtualFree(mbi.AllocationBase, 0, MEM_RELEASE);
	}
}

size_t _paged_size(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	if (!p)
		return 0;

	VirtualQuery(p, &mbi, sizeof(mbi));
	
	return mbi.RegionSize;
}

void* _paged_lock(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	if (!p)
		return NULL;

	VirtualQuery(p, &mbi, sizeof(mbi));

	if (mbi.State == MEM_FREE)
	{
		return NULL;
	}
	else if (mbi.State == MEM_RESERVE)
	{
		VirtualAlloc(p, mbi.RegionSize, MEM_COMMIT, PAGE_READWRITE);
		ZeroMemory(p, mbi.RegionSize);
		return VirtualLock(p, mbi.RegionSize) ? p : NULL;
	}
	else if (mbi.State == MEM_COMMIT)
	{
		return VirtualLock(p, mbi.RegionSize) ? p : NULL;
	}

	return NULL;
}

void _paged_unlock(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	if (!p)
		return;

	VirtualQuery(p, &mbi, sizeof(mbi));

	if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_READONLY))
	{
		VirtualUnlock(p, mbi.RegionSize);
	}
}

bool_t _paged_protect(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	DWORD dw = 0;

	if (!p)
		return 0;

	VirtualQuery(p, &mbi, sizeof(mbi));

	if (mbi.State == MEM_FREE)
	{
		return 0;
	}
	else if (mbi.State == MEM_RESERVE)
	{
		VirtualAlloc(p, mbi.RegionSize, MEM_COMMIT, PAGE_READWRITE);
		ZeroMemory(p, mbi.RegionSize);
		return (bool_t)VirtualProtect(p, mbi.RegionSize, PAGE_READONLY, &dw);
	}
	else if (mbi.State == MEM_COMMIT)
	{
		return (bool_t)VirtualProtect(p, mbi.RegionSize, PAGE_READONLY, &dw);
	}

	return 0;
}
#endif

/*****************************************************************************************/
#ifdef XDK_SUPPORT_MEMO_CACHE

void* _cache_open()
{
	size_t psize = 0;

	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);

	return VirtualAlloc(NULL, MAX_SIZE, MEM_RESERVE, 0);
}

void _cache_close(void* fh)
{
	VirtualFree(fh, 0, MEM_RELEASE);
}

bool_t _cache_write(void* fh, size_t off, void* buf, size_t size, size_t* pb)
{
	void* pBase = NULL;
	DWORD loff, hoff, poff;
	DWORD dwh, dwl;
	size_t dlen, flen;

	hoff = GETSIZEH(off);
	loff = GETSIZEL(off);

	poff = (loff % PAGE_GRAN);
	loff = (loff / PAGE_GRAN) * PAGE_GRAN;
	dlen = poff + size;

	flen = MAKESIZE(loff, hoff) + dlen;
	dwh = GETSIZEH(flen);
	dwl = GETSIZEL(flen);

	pBase = VirtualAlloc((LPBYTE)fh + (flen - dlen), dlen, MEM_COMMIT, PAGE_READWRITE);
	if (!pBase)
	{
		if (pb) *pb = 0;

		return 0;
	}

	CopyMemory((void*)((char*)pBase + poff), buf, size);

	VirtualFree(pBase, dlen, MEM_DECOMMIT);

	if (pb) *pb = size;

	return 1;
}

bool_t _cache_read(void* fh, size_t off, void* buf, size_t size, size_t* pb)
{
	void* pBase = NULL;
	DWORD loff, hoff, poff;
	DWORD dwh, dwl;
	size_t dlen, flen;

	hoff = GETSIZEH(off);
	loff = GETSIZEL(off);

	poff = (loff % PAGE_GRAN);
	loff = (loff / PAGE_GRAN) * PAGE_GRAN;
	dlen = poff + size;

	flen = MAKESIZE(loff, hoff) + dlen;
	dwh = GETSIZEH(flen);
	dwl = GETSIZEL(flen);

	pBase = VirtualAlloc((LPBYTE)fh + (flen - dlen), dlen, MEM_COMMIT, PAGE_READONLY);
	if (!pBase)
	{
		if (pb) *pb = 0;

		return 0;
	}

	CopyMemory(buf, (void*)((char*)pBase + poff), size);

	VirtualFree(pBase, dlen, MEM_DECOMMIT);

	if (pb) *pb = size;

	return 1;
}
#endif

#endif //XDK_SUPPORT_MEMO