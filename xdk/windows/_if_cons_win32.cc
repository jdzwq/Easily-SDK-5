/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc console system call document

	@module	_if_console.c | console system call windows implement file

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

#ifdef XDK_SUPPORT_CONS

res_file_t _cons_alloc(tchar_t* cname, int max)
{
	HANDLE hCon;
	BOOL rt;

	if (!AllocConsole())
		return INVALID_FILE;

	hCon = GetStdHandle(STD_INPUT_HANDLE);
	if (hCon)
		rt = SetHandleInformation(hCon, HANDLE_FLAG_INHERIT | HANDLE_FLAG_PROTECT_FROM_CLOSE, 0);

	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hCon)
		rt = SetHandleInformation(hCon, HANDLE_FLAG_INHERIT | HANDLE_FLAG_PROTECT_FROM_CLOSE, 0);

	hCon = GetStdHandle(STD_ERROR_HANDLE);
	if (hCon)
		rt = SetHandleInformation(hCon, HANDLE_FLAG_INHERIT | HANDLE_FLAG_PROTECT_FROM_CLOSE, 0);

	//DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);
	DrawMenuBar(GetConsoleWindow());

	return (res_file_t)1;
}

void _cons_free(res_file_t con)
{
	FreeConsole();
}

bool_t _cons_sigaction(res_file_t con, PF_SIGHANDLER pf)
{
	return (SetConsoleCtrlHandler((PHANDLER_ROUTINE)pf, TRUE)) ? 1 : 0;
}

res_file_t _cons_stdin(res_file_t con)
{
	HANDLE hCon;

	hCon = GetStdHandle(STD_INPUT_HANDLE);

	if (!hCon || hCon == INVALID_HANDLE_VALUE)
		return (res_file_t)0;
	else
		return (res_file_t)hCon;
}

res_file_t _cons_stdout(res_file_t con)
{
	HANDLE hCon;

	hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!hCon || hCon == INVALID_HANDLE_VALUE)
		return (res_file_t)0;
	else
		return (res_file_t)hCon;
}

bool_t _cons_write(res_file_t con, void* buf, size_t len, size_t* pb)
{
	DWORD dw = 0;
	HANDLE hCon;

	hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!hCon || hCon == INVALID_HANDLE_VALUE)
	{
		if (pb) *pb = 0;
		return 0;
	}

	if (!WriteConsoleA(hCon, (void*)buf, (DWORD)len, &dw, NULL))
	{
		if (pb) *pb = 0;
		return 0;
	}

	if (pb) *pb = dw;

	return 1;
}

bool_t _cons_flush(res_file_t con)
{
	HANDLE hCon;

	hCon =  GetStdHandle(STD_OUTPUT_HANDLE);

	if (!hCon || hCon == INVALID_HANDLE_VALUE)
		return 0;

	return (FlushConsoleInputBuffer(hCon)) ? 1 : 0;
}

bool_t _cons_read(res_file_t con, void* buf, size_t size, size_t* pb)
{
	DWORD dw = 0;
	HANDLE hCon;

	hCon = GetStdHandle(STD_INPUT_HANDLE);

	if (!hCon || hCon == INVALID_HANDLE_VALUE)
	{
		if (pb) *pb = 0;
		return 0;
	}

	if (!ReadConsoleA(hCon, (void*)buf, (DWORD)size, &dw, NULL))
	{
		if (pb) *pb = 0;
		return 0;
	}

	if (pb) *pb = dw;

	return 1;	
}

#endif //XDK_SUPPORT_CONS