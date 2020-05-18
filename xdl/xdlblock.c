/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory block document

	@module	xdlblock.c | xdllement file

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
but WITHOUT ANY WARRANTY; without even the xdllied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "xdlblock.h"
#include "impmem.h"
#include "imperr.h"
#include "impassert.h"
#include "varbytes.h"


typedef struct _block_t{
	xhand_head head;		//reserved for xhand_t

	byte_t** block;
	dword_t write_bytes;
	dword_t read_bytes;
}block_t;


xhand_t xblock_open(byte_t** pp)
{
	block_t* ppi;

	XDL_ASSERT(pp != NULL);

	ppi = (block_t*)xmem_alloc(sizeof(block_t));
	ppi->head.tag = _HANDLE_BLOCK;
	ppi->block = pp;
	ppi->read_bytes = 0;
	ppi->write_bytes = bytes_size(pp);

	return &ppi->head;
}

byte_t** xblock_handle(xhand_t block)
{
	block_t* ppi = TypePtrFromHead(block_t, block);

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	return ppi->block;
}

byte_t** xblock_close(xhand_t block)
{
	block_t* ppi = TypePtrFromHead(block_t, block);
	byte_t** pp;

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	pp = ppi->block;

	xmem_free(ppi);

	return pp;
}

bool_t xblock_read(xhand_t block, byte_t* buf, dword_t* pb)
{
	block_t* ppt = TypePtrFromHead(block_t, block);
	dword_t size;

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	size = (*pb < (ppt->write_bytes - ppt->read_bytes)) ? (*pb) : (ppt->write_bytes - ppt->read_bytes);

	bytes_copy((byte_t**)ppt->block, ppt->read_bytes, buf, &size);
	ppt->read_bytes += (dword_t)size;

	*pb = (dword_t)size;

	return 1;
}

bool_t xblock_write(xhand_t block, const byte_t* buf, dword_t* pb)
{
	block_t* ppt = TypePtrFromHead(block_t, block);
	dword_t size;

	XDL_ASSERT(block && block->tag == _HANDLE_BLOCK);

	size = *pb;

	bytes_insert((byte_t**)ppt->block, ppt->write_bytes, buf, size);
	ppt->write_bytes += (dword_t)size;

	*pb = (dword_t)size;
	
	return 1;
}

