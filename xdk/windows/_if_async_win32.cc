﻿/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc async system call document

	@module	_if_async.c | async system call windows implement file

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

#ifdef XDK_SUPPORT_ASYNC

void _async_alloc_lapp(async_t* pas)
{
	LPOVERLAPPED lp;

	lp = (LPOVERLAPPED)LocalAlloc(LPTR, sizeof(OVERLAPPED));

	lp->hEvent = CreateEvent(NULL, 1, 0, NULL);
	pas->lapp = (void*)lp;
	pas->type = ASYNC_EVENT;
}

void _async_release_lapp(async_t* pas)
{
	LPOVERLAPPED lp;

	if (!pas->lapp)
		return;

	lp = (LPOVERLAPPED)pas->lapp;

	if (lp->hEvent)
		CloseHandle(lp->hEvent);

	LocalFree(pas->lapp);
	pas->lapp = NULL;
}

#endif //XDK_SUPPORT_ASYNC

