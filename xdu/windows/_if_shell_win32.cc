/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc shell document

	@module	_if_shell_win.c | widnows implement file

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

#include "xduiml.h"

#ifdef XDU_SUPPORT_SHELL

#pragma comment(lib,"shell32.lib")

bool_t _shell_get_filename(res_win_t owner, const tchar_t* defpath, const tchar_t* filter, const tchar_t* defext, bool_t saveit, tchar_t* pathbuf, int pathlen, tchar_t* filebuf, int filelen)
{
	OPENFILENAME ofn = { 0 };

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.hInstance = 0;
	ofn.lpstrFilter = filter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nFilterIndex = 1L;
	ofn.lpstrFile = pathbuf;
	ofn.nMaxFile = pathlen;
	ofn.lpstrFileTitle = filebuf;
	ofn.nMaxFileTitle = filelen;
	ofn.lpstrInitialDir = defpath;
	ofn.lpstrTitle = NULL;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = defext;
	ofn.lCustData = 0;

	if (saveit)
	{
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		if (!GetSaveFileName(&ofn))
			return 0;
	}
	else
	{
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;

		if (!GetOpenFileName(&ofn))
			return 0;
	}

	if (pathbuf[ofn.nFileOffset - 1] == _T('\\') || pathbuf[ofn.nFileOffset - 1] == _T('/'))
		pathbuf[ofn.nFileOffset - 1] = _T('\0');

	if (filebuf)
	{
		CopyMemory(filebuf, pathbuf + ofn.nFileOffset, filelen * sizeof(tchar_t));
	}

	return 1;
}

bool_t _shell_get_pathname(res_win_t owner, const tchar_t* defpath, bool_t createit, tchar_t* pathbuf, int pathlen)
{
	BROWSEINFO brw = { 0 };
	LPITEMIDLIST pidl = NULL;
	IShellFolder* pisf = NULL;
	DWORD len,atr;
	WCHAR wPath[MAX_PATH] = { 0 };

	brw.hwndOwner = owner;
	brw.pszDisplayName = NULL;
	brw.lpszTitle = NULL;

	if (defpath)
	{
		if (SUCCEEDED(SHGetDesktopFolder(&pisf)))
		{
			CopyMemory((void*)wPath, (void*)defpath, MAX_PATH * sizeof(WCHAR));
			pisf->ParseDisplayName(NULL, NULL, wPath, &len, &pidl, &atr);
			pisf->Release();

			brw.pidlRoot = pidl;
		}
		else
		{
			brw.pidlRoot = 0;
		}
	}
	else
	{
		brw.pidlRoot = 0;
	}

	if (createit)
		brw.ulFlags = 0x0040 | BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
	else
		brw.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX;

	pidl = SHBrowseForFolder(&brw);
	if (!pidl)
	{
		return 0;
	}

	return (SHGetPathFromIDList(pidl, pathbuf) == TRUE)? 1 : 0;
}

bool_t _shell_get_curpath(tchar_t* pathbuf, int pathlen)
{
	return (GetCurrentDirectory(pathlen, pathbuf))? 1 :  0;
}

bool_t _shell_get_runpath(tchar_t* pathbuf, int pathlen)
{
	tchar_t* token;
	tchar_t fullpath[MAX_PATH] = { 0 };

	if (!GetModuleFileName(NULL, fullpath, MAX_PATH))
		return 0;

	GetLongPathName(fullpath, pathbuf, pathlen);

	token = pathbuf;
	while (*token != _T('\0'))
		token++;

	while (*token != _T('\\') && *token != _T('/') && token != pathbuf)
		token--;

	if (*token != _T('\\') || *token != _T('/'))
		*token = _T('\0');

	return 1;
}

bool_t _shell_get_apppath(tchar_t* pathbuf, int pathlen)
{
	LPITEMIDLIST pidl = NULL;
	tchar_t fullpath[MAX_PATH] = { 0 };

	SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES, &pidl);

	if (!pidl)
		return 0;

	if (SHGetPathFromIDList(pidl, fullpath))
	{
		return (GetLongPathName(fullpath, pathbuf, pathlen) > 0)? 1 : 0;
	}

	return 0;
}

bool_t _shell_get_docpath(tchar_t* pathbuf, int pathlen)
{
	LPITEMIDLIST pidl = NULL;
	tchar_t fullpath[MAX_PATH] = { 0 };

	SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);

	if (!pidl)
		return 0;

	if (SHGetPathFromIDList(pidl, fullpath))
	{
		return (GetLongPathName(fullpath, pathbuf, pathlen) > 0)? 1 : 0;
	}

	return 0;
}

bool_t _shell_get_tmppath(tchar_t* pathbuf, int pathlen)
{
	LPITEMIDLIST pidl = NULL;
	tchar_t fullpath[MAX_PATH] = { 0 };

	SHGetSpecialFolderLocation(NULL, CSIDL_TEMPLATES, &pidl);

	if (!pidl)
		return 0;

	if (SHGetPathFromIDList(pidl, fullpath))
	{
		return (GetLongPathName(fullpath, pathbuf, pathlen) > 0)? 1 : 0;
	}

	return 0;
	//return (GetWindowsDirectory(pathbuf, pathlen)) ? 1 : 0;
}

#endif //XDU_SUPPORT_SHELL