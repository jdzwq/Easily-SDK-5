/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc block document

	@module	impblock.c | block implement file

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

#include "impblock.h"
#include "xdlinit.h"
#include "xdlimp.h"

#ifdef XDK_SUPPORT_FILE_BLOCK

typedef struct _block_t{
	xhand_head head;		//reserved for xhand_t

	res_file_t block;
}block_t;


xhand_t xblock_open(const tchar_t* fname, dword_t fmode)
{
	block_t* ppi;
	if_file_t* pif;
	res_file_t bh;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	bh = (*pif->pf_block_open)(fname, fmode);
	if (bh == INVALID_FILE)
	{
		set_system_error(_T("xblock_open"));
		return NULL;
	}

	ppi = (block_t*)xmem_alloc(sizeof(block_t));
	ppi->head.tag = _HANDLE_BLOCK;
	ppi->block = bh;

	return (xhand_t)ppi;
}

res_file_t xblock_handle(xhand_t block)
{
	block_t* ppi = (block_t*)block;

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	return ppi->block;
}

void xblock_close(xhand_t block)
{
	block_t* ppt = (block_t*)block;
	if_file_t* pif;

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_block_close)(ppt->block);

	xmem_free(ppt);
}


bool_t xblock_size(xhand_t block, dword_t* ph, dword_t* pl)
{
	block_t* ppt = (block_t*)block;
	if_file_t* pif;

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if(!(*pif->pf_block_size)(ppt->block, ph, pl))
	{
		set_system_error(_T("xblock_size"));
		return 0;
	}

	return 1;
}

bool_t xblock_read(xhand_t block, dword_t hoff, dword_t loff, byte_t* buf, dword_t size)
{
	block_t* ppt = (block_t*)block;
	if_file_t* pif;

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_block_read)(ppt->block, hoff, loff, buf, size, NULL))
	{
		set_system_error(_T("xblock_read"));
		return 0;
	}

	return 1;
}

bool_t xblock_write(xhand_t block, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size)
{
	block_t* ppt = (block_t*)block;
	if_file_t* pif;

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_block_write)(ppt->block, hoff, loff, (void*)buf, size, NULL))
	{
		set_system_error(_T("xblock_write"));
		return 0;
	}

	return 1;
}

bool_t xblock_truncate(xhand_t block, dword_t hoff, dword_t loff)
{
	block_t* ppt = (block_t*)block;
	if_file_t* pif;

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	pif = PROCESS_FILE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_block_truncate)(ppt->block, hoff, loff))
	{
		set_system_error(_T("xblock_truncate"));
		return 0;
	}

	return 1;
}

#endif //XDK_SUPPORT_FILE_BLOCK
