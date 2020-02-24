/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pipe system call document

	@module	_if_pipe.c | pipe system call windows implement file

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

#ifdef XDK_SUPPORT_PIPE

#define PIPE_HEAD		_T("\\\\.\\pipe\\")

void _pipe_srv(const tchar_t* pname, res_file_t* pread, res_file_t* pwrite)
{
	tchar_t path[PATH_LEN] = { 0 };
	SYSTEM_INFO si = { 0 };
	HANDLE hp = NULL;
	BOOL bc = 0;
	SECURITY_ATTRIBUTES saAttr;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);

	if (pread)
		*pread = NULL;
	if (pwrite)
		*pwrite = NULL;

	if (!pname)
		return;

	if (_tstrcmp(pname, PIPE_STD) == 0)
	{
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		if (!CreatePipe(pread, pwrite, &saAttr, 0))
			return;

		if (pread)
			SetHandleInformation(*pread, HANDLE_FLAG_INHERIT, 0);

		if (pwrite)
			SetHandleInformation(*pwrite, HANDLE_FLAG_INHERIT, 0);
	}
	else
	{
		saAttr.bInheritHandle = FALSE;
		saAttr.lpSecurityDescriptor = NULL;

		GetSystemInfo(&si);

		_tsprintf(path, _T("%s%s"), PIPE_HEAD, pname);
		hp = CreateNamedPipe(path, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, si.dwPageSize, si.dwPageSize, 0, &saAttr);
		if (hp == INVALID_HANDLE_VALUE)
		{
			return;
		}
		
		bc = ConnectNamedPipe(hp, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		if (!bc)
		{
			CloseHandle(hp);
			return;
		}

		if (pread)
			*pread = hp;
		if (pwrite)
			*pwrite = hp;
	}
}

void _pipe_stop(const tchar_t* pname, res_file_t pip)
{
	FlushFileBuffers(pip);
	DisconnectNamedPipe(pip);
}

void _pipe_close(res_file_t pip)
{
	CloseHandle(pip);
}

void _pipe_cli(const tchar_t* pname, res_file_t* pread, res_file_t* pwrite)
{
	tchar_t path[PATH_LEN] = { 0 };
	HANDLE hp = NULL;

	if (pread)
		*pread = NULL;
	if (pwrite)
		*pwrite = NULL;

	if (!pname)
		return;

	if (_tstrcmp(pname, PIPE_STD) == 0)
	{
		if (pread)
			*pread = GetStdHandle(STD_INPUT_HANDLE);
		if (pwrite)
			*pwrite = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	else
	{
		_tsprintf(path, _T("%s%s"), PIPE_HEAD, pname);
		hp = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hp == INVALID_HANDLE_VALUE)
		{
			return;
		}
		
		if (pread)
			*pread = hp;
		if (pwrite)
			*pwrite = hp;
	}
}

bool_t _pipe_wait(const tchar_t* pname, int ms)
{
	tchar_t path[PATH_LEN] = { 0 };
	DWORD dw;

	if (_tstrcmp(pname, PIPE_STD) == 0)
	{
		return 1;
	}
	else
	{
		_tsprintf(path, _T("%s%s"), PIPE_HEAD, pname);
		if (ms < 0)
			dw = NMPWAIT_WAIT_FOREVER;
		else if (!ms)
			dw = NMPWAIT_USE_DEFAULT_WAIT;
		else
			dw = ms;

		return (WaitNamedPipe(path, dw)) ? 1 : 0;
	}
}

bool_t _pipe_write(res_file_t pipe, void* buf, dword_t len, dword_t* pb)
{
	DWORD dw = 0;

	if (WriteFile(pipe, buf, len, &dw, NULL))
	{
		if (pb)
			*pb = dw;

		return 1;
	}
	else
	{
		if (pb)
			*pb = 0;

		return 0;
	}
}

bool_t _pipe_flush(res_file_t pipe)
{
	if (!pipe)
		return 0;

	return (FlushFileBuffers(pipe)) ? 1 : 0;
}

bool_t _pipe_read(res_file_t pipe, void* buf, dword_t max, dword_t* pb)
{
	DWORD dw = 0;

	if (ReadFile(pipe, buf, max, &dw, NULL))
	{
		if (pb)
			*pb = dw;

		return 1;
	}
	else
	{
		if (pb)
			*pb = 0;

		return 0;
	}
}

#endif //XDK_SUPPORT_PIPE
