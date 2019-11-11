/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc error system call document

	@module	_if_error.c | windows implement file

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

#ifdef XDK_SUPPORT_ERROR

int _error_text(tchar_t* buf, int max)
{
	DWORD dwLen, dwErr;
	void *token;

	dwErr = GetLastError();
	dwLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&token, 0, NULL);

	if (token)
	{
		dwLen = ((int)dwLen < max) ? dwLen : max;
		if (buf)
		{
			CopyMemory((void*)buf, token, dwLen * sizeof(tchar_t));
			buf[dwLen] = _T('\0');
		}
		LocalFree(token);
	}
	else
	{
		if (buf)
		{
			buf[0] = _T('\0');
		}
	}

	return (int)dwLen;
}

void _error_exit(void)
{
	ExitProcess(-1);
}

void _error_debug(const tchar_t* str)
{
	OutputDebugString(str);
}

void _error_print(const char* str)
{
	OutputDebugStringA(str);
	OutputDebugStringA("\n");
}

#endif //XDK_SUPPORT_ERROR