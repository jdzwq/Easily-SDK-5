/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory cache document

	@module	impcache.c | implement file

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

#include "impcache.h"
#include "impmem.h"
#include "imperr.h"
#include "impassert.h"
#include "xdlinit.h"


/////////////////////////////////////////////////////////////////////////////////////////
#ifdef XDK_SUPPORT_MEMO_CACHE

typedef struct _cache_t{
	xhand_head head;		//reserved for xhand_t

	void* cache;
	dword_t write_bytes;
	dword_t read_bytes;
}cache_t;


xhand_t xcache_open()
{
	cache_t* ppi;
	if_memo_t* pif;
	void* bh;

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	bh = (*pif->pf_cache_open)();

	if (!bh)
	{
		set_system_error(_T("pf_cache_open"));
		return NULL;
	}

	ppi = (cache_t*)xmem_alloc(sizeof(cache_t));
	ppi->head.tag = _HANDLE_CACHE;
	ppi->cache = bh;
	ppi->read_bytes = 0;
	ppi->write_bytes = 0;

	return &ppi->head;
}

void* xcache_handle(xhand_t cache)
{
	cache_t* ppi = TypePtrFromHead(cache_t, cache);

	XDL_ASSERT(cache && cache->tag == _HANDLE_CACHE);

	return ppi->cache;
}

void xcache_close(xhand_t cache)
{
	cache_t* ppi = TypePtrFromHead(cache_t, cache);
	if_memo_t* pif;

	XDL_ASSERT(cache && cache->tag == _HANDLE_CACHE);

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_cache_close)(ppi->cache);

	xmem_free(ppi);
}

bool_t xcache_read(xhand_t cache, byte_t* buf, dword_t* pb)
{
	cache_t* ppt = TypePtrFromHead(cache_t, cache);
	if_memo_t* pif;
	dword_t size;
	bool_t rt;

	XDL_ASSERT(cache && cache->tag == _HANDLE_CACHE);

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = (*pb < (ppt->write_bytes - ppt->read_bytes)) ? (*pb) : (ppt->write_bytes - ppt->read_bytes);

	if (size)
		rt = (*pif->pf_cache_read)(ppt->cache, 0, ppt->read_bytes, buf, size, &size);
	else
		rt = 1;

	if (rt)
	{
		ppt->read_bytes += (dword_t)size;
		*pb = (dword_t)size;
	}
	else
	{
		set_system_error(_T("pf_cache_read"));
		*pb = 0;
	}

	return rt;
}

bool_t xcache_write(xhand_t cache, const byte_t* buf, dword_t* pb)
{
	cache_t* ppt = TypePtrFromHead(cache_t, cache);
	if_memo_t* pif;
	dword_t size;
	bool_t rt;

	XDL_ASSERT(cache && cache->tag == _HANDLE_CACHE);

	pif = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pb;
	rt = (*pif->pf_cache_write)(ppt->cache, 0, ppt->write_bytes, (void*)buf, size, &size);
	if (rt)
	{
		ppt->write_bytes += (dword_t)size;
		*pb = (dword_t)size;
	}
	else
	{
		set_system_error(_T("pf_cache_write"));
		*pb = 0;
	}

	return rt;
}

#endif //XDK_SUPPORT_MEMO_CACHE
