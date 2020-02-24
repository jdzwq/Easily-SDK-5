/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc shell document

	@module	_if_shell_win.c | shell api macos implement file

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

#ifdef XDK_SUPPORT_SHELL

bool_t _shell_get_curpath(tchar_t* pathbuf, int pathlen)
{
	tchar_t* p;

	p = realpath("./", pathbuf);
	if(p == NULL)
        return 0;

	p = strrchr(pathbuf,  '/');
    if(p != NULL)
    {
    	*p = '\0';
	}

	return 1;
}

bool_t _shell_get_runpath(tchar_t* pathbuf, int pathlen)
{
	size_t size;
	tchar_t* p;

	size = readlink("/proc/self/exe", pathbuf, pathlen);

	pathbuf[size] = '\0';
    
    p = strrchr(pathbuf,  '/');
    if(p != NULL)
    {
   	 *p = '\0';
	}

	return 1;
}

bool_t _shell_get_apppath(tchar_t* pathbuf, int pathlen)
{
	
	return 0;
}

bool_t _shell_get_docpath(tchar_t* pathbuf, int pathlen)
{
	
	return 0;
}

bool_t _shell_get_tmppath(tchar_t* pathbuf, int pathlen)
{
	
	return 0;
}

#endif