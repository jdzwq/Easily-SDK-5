/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc module system call document

	@module	_if_module.c | module system call windows implement file

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

#ifdef XDK_SUPPORT_PROCESS

res_modu_t _load_library(const tchar_t* lname)
{
	return LoadLibrary(lname);
}

void _free_library(res_modu_t lib)
{
	FreeLibrary(lib);
}

void* _get_address(res_modu_t lib, const schar_t* fname)
{
#ifdef WINCE
	wchar_t* token;
	int len;
	void* p;

	len = WideCharToMultiByte(CP_ACP, 0, fname, -1, NULL, 0, 0, 0);
	token = (char*)LocalAlloc(LPTR, (len + 1) * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, fname, -1, token, len, 0, 0);

	p = (void*)GetProcAddress(lib, token);
	LocalFree(token);
	return p;
#else
	return (void*)GetProcAddress(lib, fname);
#endif //WINCE
}

void _get_runpath(res_modu_t ins, tchar_t* buf, int max)
{
	tchar_t* token;

	GetModuleFileName(ins, buf, max);

	token = buf + _tcslen(buf);
	while (*token != _T('\\') && *token != _T('/') && token != buf)
		token--;

	*token = _T('\0');
}

bool_t _create_process(const tchar_t* exename, const tchar_t* cmdline, int share, proc_info_t* ppi)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL rt;
	DWORD dw;
	SECURITY_ATTRIBUTES sa;

	tchar_t sz_cmd[MAX_PATH] = { 0 };

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	if (exename)
	{
		_tstrcpy(sz_cmd, exename);
	}

	if (cmdline)
	{
		if (sz_cmd[0] != _T('\0') && cmdline[0] != _T('\0'))
		{
			_tstrcat(sz_cmd, _T(" "));
		}
		_tstrcat(sz_cmd, cmdline);
	}

	if (sz_cmd[0] == _T('\0'))
	{
		return 0;
	}

	if (share)
	{
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;

		CreatePipe(&ppi->pip_read, &ppi->std_write, &sa, 0);

		if (ppi->pip_read)
			SetHandleInformation(ppi->pip_read, HANDLE_FLAG_INHERIT, 0);

		CreatePipe(&ppi->std_read, &ppi->pip_write, &sa, 0);

		if (ppi->pip_write)
			SetHandleInformation(ppi->pip_write, HANDLE_FLAG_INHERIT, 0);

		if (!ppi->pip_read || !ppi->pip_write)
		{
			if (ppi->pip_read)
				CloseHandle(ppi->pip_read);
			if (ppi->std_write)
				CloseHandle(ppi->std_write);
			if (ppi->std_read)
				CloseHandle(ppi->std_read);
			if (ppi->pip_write)
				CloseHandle(ppi->pip_write);

			ZeroMemory((void*)ppi, sizeof(proc_info_t));
			return 0;
		}

		si.hStdOutput = ppi->std_write;
		si.hStdInput = ppi->std_read;
		si.hStdError = ppi->std_write;
		si.dwFlags |= STARTF_USESTDHANDLES;
	}

	rt = CreateProcess(NULL, sz_cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	if (!rt)
	{
		dw = GetLastError();

		if (ppi->pip_read)
			CloseHandle(ppi->pip_read);
		if (ppi->std_write)
			CloseHandle(ppi->std_write);
		if (ppi->std_read)
			CloseHandle(ppi->std_read);
		if (ppi->pip_write)
			CloseHandle(ppi->pip_write);

		ZeroMemory((void*)ppi, sizeof(proc_info_t));
		return 0;
	}

	ppi->process_handle = pi.hProcess;
	ppi->thread_handle = pi.hThread;
	ppi->process_id = pi.dwProcessId;
	ppi->thread_id = pi.dwThreadId;

	return 1;
}

void _release_process(proc_info_t* ppi)
{
	if (ppi->pip_read)
		CloseHandle(ppi->pip_read);
	if (ppi->pip_write)
		CloseHandle(ppi->pip_write);

	if (ppi->std_write)
		CloseHandle(ppi->std_write);
	if (ppi->std_read)
		CloseHandle(ppi->std_read);

	if (ppi->thread_handle)
		CloseHandle(ppi->thread_handle);
	if (ppi->process_handle)
		CloseHandle(ppi->process_handle);

	ZeroMemory((void*)ppi, sizeof(proc_info_t));
}

void _process_waitrun(res_hand_t ph)
{
	WaitForInputIdle(ph, 500);
}

res_hand_t _process_dupli(res_hand_t ph, res_hand_t vh)
{
	HANDLE hh = NULL;

	DuplicateHandle(GetCurrentProcess(), vh, ph, &hh, 0, FALSE, DUPLICATE_SAME_ACCESS);

	return hh;
}

void* _process_alloc(res_hand_t ph, size_t dw)
{
	return VirtualAllocEx(ph, NULL, dw, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void _process_free(res_hand_t ph, void* p)
{
	VirtualFreeEx(ph, p, 0, MEM_RELEASE);
}

bool_t _process_write(res_hand_t ph, void* p, void* data, size_t dw)
{
	return (bool_t)WriteProcessMemory(ph, p, data, dw, 0);
}

bool_t _process_read(res_hand_t ph, void* p, void* data, size_t dw)
{
	return ReadProcessMemory(ph, p, data, dw, NULL);
}

bool_t _inherit_handle(res_hand_t hh, bool_t b)
{
	return SetHandleInformation((HANDLE)hh, HANDLE_FLAG_INHERIT, ((b) ? HANDLE_FLAG_INHERIT : 0));
}

void _release_handle(res_hand_t hh)
{
	CloseHandle((HANDLE)hh);
}

void _read_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, tchar_t* buf, int max)
{
	GetPrivateProfileString(sec, key, _T(""), buf, max, fname);
}

void _write_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, const tchar_t* val)
{
	WritePrivateProfileString(sec, key, val, fname);
}

void _system_info(sys_info_t* psi)
{
	SYSTEM_INFO si = { 0 };

	GetSystemInfo(&si);

	switch (si.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		_tcscpy(psi->architecture, _T("x64"));
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		_tcscpy(psi->architecture, _T("IA64"));
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		_tcscpy(psi->architecture, _T("x86"));
		break;
	}

	psi->processor_type = si.dwProcessorType;
	psi->processor_number = si.dwNumberOfProcessors;
	psi->page_size = si.dwPageSize;
	psi->page_gran = si.dwAllocationGranularity;
}

#endif //XDK_SUPPORT_PROCESS