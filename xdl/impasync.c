/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc async document

	@module	impasync.c | implement file

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

#include "impasync.h"
#include "imperr.h"
#include "impassert.h"
#include "xdlinit.h"


#ifdef XDK_SUPPORT_ASYNC

async_t* async_alloc_lapp(int type, int ms, res_file_t fd)
{
	if_async_t *pif;

	pif = PROCESS_ASYNC_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_async_alloc_lapp)(type, ms, fd);
}

void async_free_lapp(async_t* pas)
{
	if_async_t *pif;

	pif = PROCESS_ASYNC_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_async_free_lapp)(pas);
}


#endif //XDK_SUPPORT_ASYNC
