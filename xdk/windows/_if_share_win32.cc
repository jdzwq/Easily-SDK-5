/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory file system call document

	@module	_if_share.c | memory file system call windows implement file

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

res_file_t _share_srv(const tchar_t* fname, const tchar_t* fpath, size_t size)
{
	HANDLE hShare,hFile;
	DWORD dwh, dwl,dwSize,dwPos = 0;
	void* buf = NULL;

	WIN32_FILE_ATTRIBUTE_DATA ad = { 0 };

	dwh = GETSIZEH(size);
	dwl = GETSIZEL(size);

	hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, dwh, dwl, fname);
	if (!hShare)
	{
		return INVALID_FILE;
	}

	if (!GetFileAttributesEx(fpath, GetFileExInfoStandard, &ad))
	{
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	if (ad.nFileSizeHigh)
	{
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	dwSize = ad.nFileSizeLow;

	hFile = CreateFile(fpath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ((HANDLE)hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	buf = HeapAlloc(GetProcessHeap(), 0, dwSize);

	while (dwPos < dwSize)
	{
		dwl = 0;
		if (!ReadFile(hFile, (void*)((BYTE*)buf + dwPos), dwSize - dwPos, &dwl, NULL))
			break;

		if (!dwl)
			break;

		dwPos += dwl;
	}

	CloseHandle(hFile);

	if (dwPos < dwSize)
	{
		HeapFree(GetProcessHeap(), 0, buf);
		CloseHandle(hShare);
		return INVALID_FILE;
	}

	dwSize = (dwSize < (DWORD)size) ? dwSize : (DWORD)size;

	if (!_share_write(hShare, 0, buf, dwSize, NULL))
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

res_file_t _share_cli(const tchar_t* fname, size_t size)
{
	HANDLE hp;
	DWORD dwh, dwl;

	dwh = GETSIZEH(size);
	dwl = GETSIZEL(size);

	hp = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, dwh, dwl, fname);

	return (hp) ? hp : INVALID_FILE;
}

bool_t _share_write(res_file_t fh, size_t off, void* buf, size_t size, size_t* pcb)
{
	void* pBase = NULL;
	DWORD loff, hoff, poff;
	size_t dlen;

	hoff = GETSIZEH(off);
	loff = GETSIZEL(off);

	poff = (loff % PAGE_GRAN);
	loff = (loff / PAGE_GRAN) * PAGE_GRAN;
	dlen = poff + size;

	pBase = MapViewOfFile(fh, FILE_MAP_WRITE, hoff, loff, dlen);
	if (!pBase)
	{
		if (pcb) *pcb = 0;

		return 0;
	}

	CopyMemory((void*)((char*)pBase + poff), buf, size);

	UnmapViewOfFile(pBase);

	if (pcb) *pcb = size;

	return 1;
}

bool_t _share_read(res_file_t fh, size_t off, void* buf, size_t size, size_t* pcb)
{
	void* pBase = NULL;
	DWORD loff, hoff, poff;
	size_t dlen;

	hoff = GETSIZEH(off);
	loff = GETSIZEL(off);

	poff = (loff % PAGE_GRAN);
	loff = (loff / PAGE_GRAN) * PAGE_GRAN;
	dlen = poff + size;

	pBase = MapViewOfFile(fh, FILE_MAP_READ, hoff, loff, dlen);
	if (!pBase)
	{
		if (pcb) *pcb = 0;

		return 0;
	}

	CopyMemory(buf, (void*)((char*)pBase + poff), size);

	UnmapViewOfFile(pBase);

	if (pcb) *pcb = size;

	return 1;
}

void* _share_lock(res_file_t fh, size_t off, size_t size)
{
	void* pBase = NULL;
	DWORD loff, hoff, poff;
	size_t dlen;

	hoff = GETSIZEH(off);
	loff = GETSIZEL(off);

	poff = (loff % PAGE_GRAN);
	loff = (loff / PAGE_GRAN) * PAGE_GRAN;
	dlen = poff + size;

	pBase = MapViewOfFile(fh, FILE_MAP_READ, hoff, loff, dlen);

	return (pBase) ? (void*)((char*)pBase + poff) : NULL;
}

void _share_unlock(res_file_t fh, size_t off, size_t size, void* p)
{
	DWORD loff, hoff, poff;
	size_t dlen;

	hoff = GETSIZEH(off);
	loff = GETSIZEL(off);

	poff = (loff % PAGE_GRAN);
	loff = (loff / PAGE_GRAN) * PAGE_GRAN;
	dlen = poff + size;

	UnmapViewOfFile((void*)((char*)p - poff));
}

#endif //XDK_SUPPORT_SHARE
