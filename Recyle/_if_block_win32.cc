/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc block device system call document

	@module	_if_block.c | block device system call windows implement file

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

#ifdef XDK_SUPPORT_FILE_BLOCK

res_file_t _block_open(const tchar_t* fname, u32_t fmode)
{
	HANDLE hFile = 0;
	DWORD dwAccess, dwCreate;

	if (fmode & FILE_OPEN_APPEND)
	{
		dwCreate = OPEN_ALWAYS;
		dwAccess = GENERIC_READ | GENERIC_WRITE;
	}
	else if (fmode & FILE_OPEN_CREATE)
	{
		dwCreate = CREATE_ALWAYS;
		dwAccess = GENERIC_READ | GENERIC_WRITE;
	}
	else
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_READ;
	}

	hFile = CreateFile(fname, dwAccess, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, dwCreate, FILE_FLAG_NO_BUFFERING, NULL);
	if ((HANDLE)hFile == INVALID_HANDLE_VALUE)
	{
		return INVALID_FILE;
	}

	return hFile;
}

bool_t _block_size(res_file_t fh, u32_t* ph, u32_t* pl)
{
	bool_t rt;

	*pl = GetFileSize(fh, (LPDWORD)ph);

	rt = (*pl == INVALID_FILE_SIZE)? 0 : 1;

	return rt;
}

void _block_close(res_file_t fh)
{
	CloseHandle(fh);
}

bool_t _block_write(res_file_t fh, u32_t hoff, u32_t loff, void* buf, size_t size, size_t* pcb)
{
	return _file_write_range(fh, hoff, loff, buf, size, pcb);
}

bool_t _block_read(res_file_t fh, u32_t hoff, u32_t loff, void* buf, size_t size, size_t* pcb)
{
	return _file_read_range(fh, hoff, loff, buf, size, pcb);
}

bool_t _block_truncate(res_file_t fh, u32_t hoff, u32_t loff)
{
	HANDLE hMap = NULL;

	hMap = CreateFileMapping(fh, NULL, PAGE_READWRITE, hoff, loff, NULL);
	if (!hMap)
	{
		return 0;
	}

	CloseHandle(hMap);

	return 1;
}

#endif //XDK_SUPPORT_FILE_BLOCK
