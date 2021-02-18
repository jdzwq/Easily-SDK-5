/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory file system call document

	@module	_if_share.c | windows implement file

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

#ifdef XDK_SUPPORT_SHARE

res_file_t _share_srv(const tchar_t* fname, const tchar_t* fpath, dword_t hoff, dword_t loff, dword_t size)
{
	HANDLE hShare,hFile;
	DWORD dwBys = 0;
	void* buf = NULL;

	WIN32_FILE_ATTRIBUTE_DATA ad = { 0 };

	if (!GetFileAttributesEx(fpath, GetFileExInfoStandard, &ad))
	{
		return INVALID_FILE;
	}

	if (MAKESIZE(loff, hoff) > MAKESIZE(ad.nFileSizeLow, ad.nFileSizeHigh))
	{
		return INVALID_FILE;
	}

	hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, fname);
	if (!hShare)
	{
		return INVALID_FILE;
	}

	hFile = CreateFile(fpath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ((HANDLE)hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	dwBys = hoff;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, loff, (PLONG)&dwBys, FILE_BEGIN))
	{
		CloseHandle(hFile);
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	buf = HeapAlloc(GetProcessHeap(), 0, (SIZE_T)size);
	if (!buf)
	{
		CloseHandle(hFile);
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	dwBys = 0;
	if (!ReadFile(hFile, buf, (DWORD)size, &dwBys, NULL))
	{
		HeapFree(GetProcessHeap(), 0, buf);
		CloseHandle(hFile);
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	CloseHandle(hFile);

	if (!_share_write(hShare, 0, buf, size, NULL))
	{
		HeapFree(GetProcessHeap(), 0, buf);
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	HeapFree(GetProcessHeap(), 0, buf);

	return hShare;
}

void _share_close(const tchar_t* fname, res_file_t fh)
{
	CloseHandle(fh);
}

res_file_t _share_cli(const tchar_t* fname, dword_t size, dword_t fmode)
{
	HANDLE hp;
	dword_t dwAccess = 0;

	if (fmode & FILE_OPEN_APPEND)
	{
		dwAccess = FILE_MAP_READ | FILE_MAP_WRITE;
	}
	else if (fmode & FILE_OPEN_CREATE)
	{
		dwAccess = FILE_MAP_READ | FILE_MAP_WRITE;
	}
	else if (fmode & FILE_OPEN_WRITE)
	{
		dwAccess = FILE_MAP_READ | FILE_MAP_WRITE;
	}
	else
	{
		dwAccess = FILE_MAP_READ;
	}

	hp = OpenFileMapping(dwAccess, FALSE, fname);

	if (!hp && ((fmode & FILE_OPEN_CREATE) || (fmode & FILE_OPEN_APPEND)))
	{
		hp = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_RESERVE, 0, size, fname);
	}

	return (hp) ? hp : INVALID_FILE;
}

bool_t _share_write(res_file_t fh, dword_t off, void* buf, dword_t size, dword_t* pcb)
{
	void* pBase = NULL;
	DWORD poff, loff;
	SIZE_T dlen;
	DWORD page_gran;

	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);
	page_gran = si.dwAllocationGranularity;

	poff = (off % page_gran);
	loff = (off / page_gran) * page_gran;
	dlen = poff + size;

	pBase = MapViewOfFile(fh, FILE_MAP_WRITE, 0, loff, dlen);
	if (!pBase)
	{
		if (pcb) *pcb = 0;

		return 0;
	}

	VirtualAlloc(pBase, dlen, MEM_COMMIT, PAGE_READWRITE);

	CopyMemory((void*)((char*)pBase + poff), buf, (SIZE_T)size);

	VirtualFree(pBase, dlen, MEM_DECOMMIT);

	UnmapViewOfFile(pBase);

	if (pcb) *pcb = size;

	return 1;
}

bool_t _share_read(res_file_t fh, dword_t off, void* buf, dword_t size, dword_t* pcb)
{
	void* pBase = NULL;
	DWORD poff, loff;
	SIZE_T dlen;
	DWORD page_gran;

	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);
	page_gran = si.dwAllocationGranularity;

	poff = (off % page_gran);
	loff = (off / page_gran) * page_gran;
	dlen = poff + size;

	pBase = MapViewOfFile(fh, FILE_MAP_READ, 0, loff, dlen);
	if (!pBase)
	{
		if (pcb) *pcb = 0;

		return 0;
	}

	VirtualAlloc(pBase, dlen, MEM_COMMIT, PAGE_READWRITE);

	CopyMemory(buf, (void*)((char*)pBase + poff), (SIZE_T)size);

	UnmapViewOfFile(pBase);

	VirtualFree(pBase, dlen, MEM_DECOMMIT);

	if (pcb) *pcb = size;

	return 1;
}

void* _share_lock(res_file_t fh, dword_t off, dword_t size)
{
	void* pBase = NULL;
	DWORD poff, loff;
	SIZE_T dlen;
	DWORD page_gran;

	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);
	page_gran = si.dwAllocationGranularity;

	poff = (off % page_gran);
	loff = (off / page_gran) * page_gran;
	dlen = poff + size;

	pBase = MapViewOfFile(fh, FILE_MAP_WRITE, 0, loff, dlen);
	if (!pBase)
		return NULL;

	VirtualAlloc(pBase, dlen, MEM_COMMIT, PAGE_READWRITE);

	return (pBase) ? (void*)((char*)pBase + poff) : NULL;
}

void _share_unlock(res_file_t fh, dword_t off, dword_t size, void* p)
{
	void* pBase = NULL;
	DWORD poff,loff;
	SIZE_T dlen;
	DWORD page_gran;

	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);
	page_gran = si.dwAllocationGranularity;

	poff = (off % page_gran);
	loff = (off / page_gran) * page_gran;
	dlen = poff + size;
	pBase = (void*)((char*)p - poff);

	VirtualFree(pBase, dlen, MEM_DECOMMIT);

	UnmapViewOfFile(pBase);
}

#endif //XDK_SUPPORT_SHARE
