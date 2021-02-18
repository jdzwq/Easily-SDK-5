/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc async system call document

	@module	_if_async.c | windows implement file

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

void _async_init(async_t* pas, int type, int ms, res_file_t fd)
{
	LPOVERLAPPED lp = (LPOVERLAPPED)(pas->lapp);

	if (type == ASYNC_EVENT || type == ASYNC_QUEUE)
	{
		lp->hEvent = CreateEvent(NULL, 1, 0, NULL);
	}
	else
	{
		ZeroMemory((void*)lp, sizeof(OVERLAPPED));
	}

	pas->type = type;
	pas->timo = (ms < 0) ? INFINITE : ms;

#ifdef XDK_SUPPORT_THREAD_QUEUE
	if (type == ASYNC_QUEUE)
	{
		pas->port = _queue_create(NULL, fd, 0);
	}
#endif
}

void _async_uninit(async_t* pas)
{
	LPOVERLAPPED lp = (LPOVERLAPPED)pas->lapp;

	if (lp->hEvent)
		CloseHandle(lp->hEvent);

#ifdef XDK_SUPPORT_THREAD_QUEUE
	if (pas->port)
	{
		_queue_destroy(pas->port);
	}
#endif
}

#endif //XDK_SUPPORT_ASYNC

