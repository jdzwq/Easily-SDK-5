/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc virtual memory system call document

	@module	_if_cache.c | virtual memory system call windows implement file

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

#ifdef XDK_SUPPORT_MEMO_CACHE

#ifdef _OS_64
#define MAX_SPACE		0x7FFFFFFFFFF
#else
#define MAX_SPACE		0x7FFFFFFF
#endif

res_void_t _cache_open()
{
	dword_t psize = 0;

	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);
	
	return VirtualAlloc(NULL, MAX_SPACE, MEM_RESERVE, 0);
}

void _cache_close(res_void_t fh)
{
	VirtualFree(fh, 0, MEM_RELEASE);
}

bool_t _cache_write(res_void_t fh, dword_t offset, void* buf, dword_t size, dword_t* pb)
{
	void* pBase = NULL;
	DWORD dwStart, dwSize, dwIndex, dwGran = 0;
	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);
	dwGran = si.dwAllocationGranularity;

	dwStart = (offset / dwGran) * dwGran;
	dwSize = (offset % dwGran) + size;
	dwIndex = (offset % dwGran);

	pBase = VirtualAlloc((LPBYTE)fh + dwStart, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (pBase)
	{
		CopyMemory((void*)((char*)pBase + dwIndex), buf, size);

		VirtualFree(pBase, dwSize, MEM_DECOMMIT);

		if (pb)
			*pb = size;

		return 1;
	}
	else
	{
		if (pb)
			*pb = 0;

		return 0;
	}
}

bool_t _cache_read(res_void_t fh, dword_t offset, void* buf, dword_t size, dword_t* pb)
{
	void* pBase = NULL;
	DWORD dwStart, dwSize, dwIndex, dwGran = 0;
	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);
	dwGran = si.dwAllocationGranularity;

	dwStart = (offset / dwGran) * dwGran;
	dwSize = (offset % dwGran) + size;
	dwIndex = (offset % dwGran);

	pBase = VirtualAlloc((LPBYTE)fh + dwStart, dwSize, MEM_COMMIT, PAGE_READONLY);
	if (pBase)
	{
		CopyMemory(buf, (void*)((char*)pBase + dwIndex), size);

		VirtualFree(pBase, dwSize, MEM_DECOMMIT);

		if (pb)
			*pb = size;

		return 1;
	}
	else
	{
		if (pb)
			*pb = 0;

		return 0;
	}
}

#endif //XDK_SUPPORT_MEMO_CACHE
