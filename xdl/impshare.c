/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc block document

	@module	impshare.c | implement file

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

#include "impshare.h"
#include "xdlinit.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDK_SUPPORT_SHARE

typedef struct _share_t{
	xhand_head head;

	res_file_t block;
	dword_t write_bytes;
	dword_t read_bytes;
	tchar_t* sname;
}share_t;

xhand_t xshare_srv(const tchar_t* pname, const tchar_t* fpath, dword_t size)
{
	share_t* ppi;
	if_share_t* pif;
	res_file_t bh;

	pif = PROCESS_SHARE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	bh = (*pif->pf_share_srv)(pname, fpath, size);

	if (bh == INVALID_FILE)
	{
		set_system_error(_T("pf_share_open"));
		return NULL;
	}

	ppi = (share_t*)xmem_alloc(sizeof(share_t));
	ppi->head.tag = _HANDLE_SHARE;
	ppi->block = bh;
	ppi->read_bytes = 0;
	ppi->write_bytes = 0;
	ppi->sname = xsclone(pname);

	return &ppi->head;
}

xhand_t xshare_cli(const tchar_t* pname, dword_t size)
{
	share_t* ppi;
	if_share_t* pif;
	res_file_t bh;

	pif = PROCESS_SHARE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	bh = (*pif->pf_share_cli)(pname, size);

	if (bh == INVALID_FILE)
	{
		set_system_error(_T("pf_share_open"));
		return NULL;
	}

	ppi = (share_t*)xmem_alloc(sizeof(share_t));
	ppi->head.tag = _HANDLE_SHARE;
	ppi->block = bh;
	ppi->read_bytes = 0;
	ppi->write_bytes = 0;
	ppi->sname = xsclone(pname);

	return &ppi->head;
}

res_file_t xshare_handle(xhand_t block)
{
	share_t* ppi = TypePtrFromHead(share_t, block);

	XDL_ASSERT(block && block->tag == _HANDLE_SHARE);

	return ppi->block;
}

void xshare_close(xhand_t block)
{
	share_t* ppi = TypePtrFromHead(share_t, block);
	if_share_t* pif;

	XDL_ASSERT(block && block->tag == _HANDLE_SHARE);

	pif = PROCESS_SHARE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_share_close)(ppi->sname, ppi->block);

	xsfree(ppi->sname);

	xmem_free(ppi);
}

bool_t xshare_read(xhand_t block, byte_t* buf, dword_t* pcb)
{
	share_t* ppt = TypePtrFromHead(share_t, block);
	if_share_t* pif;
	size_t size;
	bool_t rt;

	XDL_ASSERT(block && block->tag == _HANDLE_SHARE);

	pif = PROCESS_SHARE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pcb;
	rt = (*pif->pf_share_read)(ppt->block, ppt->read_bytes, buf, size, &size);
	if (rt)
	{
		ppt->read_bytes += (dword_t)size;
		*pcb = (dword_t)size;
	}
	else
	{
		set_system_error(_T("pf_share_read"));
		*pcb = 0;
	}
	
	return rt;
}

bool_t xshare_write(xhand_t block, const byte_t* buf, dword_t* pcb)
{
	share_t* ppt = TypePtrFromHead(share_t, block);
	if_share_t* pif;
	size_t size;
	bool_t rt;

	XDL_ASSERT(block && block->tag == _HANDLE_SHARE);

	pif = PROCESS_SHARE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pcb;
	rt = (*pif->pf_share_write)(ppt->block, ppt->write_bytes, (void*)buf, size, &size);
	if (rt)
	{
		ppt->write_bytes += (dword_t)size;
		*pcb = (dword_t)size;
	}
	else
	{
		set_system_error(_T("pf_share_write"));
		*pcb = 0;
	}

	return rt;
}

void* xshare_lock(xhand_t block, dword_t offset, dword_t size)
{
	share_t* ppt = TypePtrFromHead(share_t, block);
	if_share_t* pif;

	XDL_ASSERT(block && block->tag == _HANDLE_SHARE);

	pif = PROCESS_SHARE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_share_lock)(ppt->block, offset, size);
}

void xshare_unlock(xhand_t block, dword_t offset, dword_t size, void* p)
{
	share_t* ppt = TypePtrFromHead(share_t, block);
	if_share_t* pif;

	XDL_ASSERT(block && block->tag == _HANDLE_SHARE);

	pif = PROCESS_SHARE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_share_unlock)(ppt->block, offset, size, p);
}

#endif //XDK_SUPPORT_SHARE
