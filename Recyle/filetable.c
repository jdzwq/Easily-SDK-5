/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc filetable document

	@module	filetable.c | filetable implement file

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

#include "filetable.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlinit.h"

#define TABLEGUID_FILEMAX		(16 * 1024 * PAGE_SIZE)
#define TABLEGUID_HEADER		PAGE_SIZE

#define TABLEGUID_OWNED			0x01
#define TABLEGUID_DIRTY			0x02
#define TABLEGUID_SAVED			0x04
#define TABLEGUID_TAGED			0x30

#define TABLEGUID_POSED			(1 << 15)
#define TABLEGUID_UNPOS			(~(1 << 15))

typedef struct _file_table_t{
	link_t lk;

	xhand_t block;
	dword_t head_size;
	dword_t file_guid;
	int file_root;

	int map_pos;
	
	dword_t map_size;
	map_t* map;

	dword_t cata_size;
	byte_t* cata;
}file_table_t;

#define PageTableFromLink(p) TypePtrFromLink(file_table_t,p)

static bool_t _flush_file_table(file_table_t* ppt, bool_t b_save)
{
	byte_t head[TABLEGUID_HEADER] = { 0 };

	if (b_save)
	{
		PUT_DWORD_LOC(head, 0, ppt->head_size);
		PUT_DWORD_LOC(head, 4, ppt->map_size);
		PUT_DWORD_LOC(head, 8, ppt->file_guid);
		PUT_DWORD_LOC(head, 12, ppt->file_root);

		xmem_copy(head + ppt->head_size - ppt->map_size, ppt->map->data, ppt->map_size);

		if (!xblock_write(ppt->block, 0, 0, head, ppt->head_size))
			return 0;

		return xblock_write(ppt->block, 0, ppt->head_size, ppt->cata, ppt->cata_size);
	}
	else
	{
		if (!xblock_read(ppt->block, 0, 0, head, TABLEGUID_HEADER))
			return 0;

		ppt->head_size = GET_DWORD_LOC(head, 0);
		ppt->map_size = GET_DWORD_LOC(head, 4);
		ppt->file_guid = GET_DWORD_LOC(head, 8);
		ppt->file_root = GET_DWORD_LOC(head, 12);

		if (ppt->map)
		{
			map_free(ppt->map);
		}

		ppt->map = map_alloc(ppt->map_size * 8);
		xmem_copy(ppt->map->data, head + ppt->head_size - ppt->map_size, ppt->map_size);

		if (ppt->cata)
		{
			page_free(ppt->cata);
		}

		ppt->cata = (byte_t*)page_alloc(ppt->map_size * 8);
		ppt->cata_size = page_size(ppt->cata);

		return xblock_read(ppt->block, 0, ppt->head_size, ppt->cata, ppt->cata_size);
	}
}

link_t_ptr create_file_table(const tchar_t* fname, dword_t fmode)
{
	file_table_t* ppt = NULL;

	dword_t dwh = 0,dwl = 0;
	byte_t head[PAGE_SIZE] = { 0 };

	TRY_CATCH;

	ppt = (file_table_t*)xmem_alloc(sizeof(file_table_t));
	ppt->lk.tag = lkFileTable;

	ppt->block = xblock_open(fname, fmode);
	if (!ppt->block)
	{
		raise_user_error(NULL, NULL);
	}

	xblock_size(ppt->block, &dwh, &dwl);

	if (dwl && dwl < TABLEGUID_HEADER)
	{
		raise_user_error(_T("open_file_table"), _T("invalid file size"));
	}

	if (dwl)
	{
		//load file table header
		if (!_flush_file_table(ppt, 0))
		{
			raise_user_error(_T("open_file_table"), _T("invalid file header"));
		}

		if (!ppt->map_size || ppt->map_size > TABLEGUID_HEADER / 2)
		{
			raise_user_error(_T("open_file_table"), _T("invalid map size"));
		}
	}
	else
	{
		ppt->head_size = TABLEGUID_HEADER;
		ppt->map_size = TABLEGUID_FILEMAX / PAGE_SIZE / 8;
		ppt->file_guid = 0;
		ppt->file_root = 0;

		ppt->map = map_alloc(ppt->map_size * 8);

		ppt->cata = (byte_t*)page_alloc(ppt->map_size * 8);
		ppt->cata_size = page_size(ppt->cata);

		//save file table header
		if (!_flush_file_table(ppt, 1))
		{
			raise_user_error(_T("open_file_table"), _T("save file header failed"));
		}
	}

	END_CATCH;

	return &ppt->lk;
ONERROR:

	if (ppt->block)
		xblock_close(ppt->block);

	if (ppt->map)
		map_free(ppt->map);

	if (ppt->cata)
		page_free(ppt->cata);

	if (ppt)
		xmem_free(ppt);

	return NULL;
}

void destroy_file_table(link_t_ptr pt)
{
	file_table_t* ppt = PageTableFromLink(pt);

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	xblock_close(ppt->block);

	map_free(ppt->map);
	page_free(ppt->cata);

	xmem_free(ppt);
}

void set_file_table_guid(link_t_ptr pt, dword_t guid)
{
	file_table_t* ppt = PageTableFromLink(pt);

	ppt->file_guid = guid;

	_flush_file_table(ppt, 1);
}

dword_t get_file_table_guid(link_t_ptr pt)
{
	file_table_t* ppt = PageTableFromLink(pt);

	return ppt->file_guid;
}

void set_file_table_root(link_t_ptr pt, int pos)
{
	file_table_t* ppt = PageTableFromLink(pt);

	ppt->file_root = pos;

	_flush_file_table(ppt, 1);
}

int get_file_table_root(link_t_ptr pt)
{
	file_table_t* ppt = PageTableFromLink(pt);

	return ppt->file_root;
}

int alloc_file_table_block(link_t_ptr pt, dword_t size)
{
	file_table_t* ppt = PageTableFromLink(pt);
	int k, n, pos;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	n = size / PAGE_SIZE;
	if (size % PAGE_SIZE)
		n++;

	pos = (n > 1) ? 0 : ppt->map_pos;

	while (n)
	{
		pos = map_find_mask(ppt->map, pos, 0);
		if (pos == C_ERR)
		{
			set_last_error(_T("file_block_alloc"), _T("no block to be alloced"), -1);
			return C_ERR;
		}

		if (n == 1)
			break;

		k = map_test_mask(ppt->map, pos, 0, n);
		if (k == n)
			break;

		pos += (k + 1);
	}

	if (pos >= (int)ppt->cata_size)
		return C_ERR;

	//cache the map pos
	ppt->map_pos = (pos + n) % ppt->map_size;

	while (n--)
	{
		ppt->cata[pos + n] |= TABLEGUID_OWNED;

		map_set_mask(ppt->map, pos + n, 1);
	}

	_flush_file_table(ppt, 1);

	return (int)(pos | TABLEGUID_POSED);
}

void free_file_table_block(link_t_ptr pt, int pos, dword_t size)
{
	file_table_t* ppt = PageTableFromLink(pt);
	int n;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	pos &= TABLEGUID_UNPOS;

	XDL_ASSERT(pos >= 0 && pos < (int)ppt->cata_size);

	n = size / PAGE_SIZE;
	if (size % PAGE_SIZE)
		n++;

	while (n--)
	{
		XDL_ASSERT(ppt->cata[pos + n] & TABLEGUID_OWNED);

		ppt->cata[pos + n] = 0;

		map_set_mask(ppt->map, pos + n, 0);
	}

	//cache the map pos
	ppt->map_pos = pos;

	_flush_file_table(ppt, 1);
}

bool_t get_file_table_block_alloced(link_t_ptr pt, int pos)
{
	file_table_t* ppt = PageTableFromLink(pt);
	byte_t tag;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	pos &= TABLEGUID_UNPOS;

	XDL_ASSERT(pos >= 0 && pos < (int)ppt->cata_size);

	tag = ppt->cata[pos];
	return (tag & TABLEGUID_OWNED) ? 1 : 0;
}

void set_file_table_block_dirty(link_t_ptr pt, int pos, bool_t b_dirty)
{
	file_table_t* ppt = PageTableFromLink(pt);
	byte_t tag;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	pos &= TABLEGUID_UNPOS;

	XDL_ASSERT(pos >= 0 && pos < (int)ppt->cata_size);
	XDL_ASSERT(ppt->cata[pos] & TABLEGUID_OWNED);

	tag = ppt->cata[pos];
	if (b_dirty)
		ppt->cata[pos] = tag | TABLEGUID_DIRTY;
	else
		ppt->cata[pos] = tag & (~TABLEGUID_DIRTY);

	_flush_file_table(ppt, 1);
}

bool_t get_file_table_block_dirty(link_t_ptr pt, int pos)
{
	file_table_t* ppt = PageTableFromLink(pt);

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	pos &= TABLEGUID_UNPOS;

	XDL_ASSERT(pos >= 0 && pos < (int)ppt->cata_size);
	XDL_ASSERT(ppt->cata[pos] & TABLEGUID_OWNED);

	return (ppt->cata[pos] & TABLEGUID_DIRTY) ? 1 : 0;
}

void set_file_table_block_tag(link_t_ptr pt, int pos, byte_t tag)
{
	file_table_t* ppt = PageTableFromLink(pt);

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	pos &= TABLEGUID_UNPOS;

	XDL_ASSERT(pos >= 0 && pos < (int)ppt->cata_size);

	XDL_ASSERT(ppt->cata[pos] & TABLEGUID_OWNED);

	tag = tag % 4;
	tag <<= 4;

	ppt->cata[pos] &= (~TABLEGUID_TAGED);
	ppt->cata[pos] |= tag ;

	_flush_file_table(ppt, 1);
}

byte_t get_file_table_block_tag(link_t_ptr pt, int pos)
{
	file_table_t* ppt = PageTableFromLink(pt);

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	pos &= TABLEGUID_UNPOS;

	XDL_ASSERT(pos >= 0 && pos < (int)ppt->cata_size);

	XDL_ASSERT(ppt->cata[pos] & TABLEGUID_OWNED);

	return (ppt->cata[pos] & TABLEGUID_TAGED) >> 4;
}

bool_t read_file_table_block(link_t_ptr pt, int pos, byte_t* buf, dword_t size)
{
	file_table_t* ppt = PageTableFromLink(pt);

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	pos &= TABLEGUID_UNPOS;

	XDL_ASSERT(pos >= 0 && pos < (int)ppt->cata_size);

	XDL_ASSERT(ppt->cata[pos] & TABLEGUID_OWNED);

	if (!(ppt->cata[pos] & TABLEGUID_SAVED))
	{
		xmem_zero(buf, size);
		return 1;
	}

	return xblock_read(ppt->block, 0, TABLEGUID_HEADER + ppt->cata_size + pos * PAGE_SIZE, buf, size);
}

bool_t write_file_table_block(link_t_ptr pt, int pos, byte_t* buf, dword_t size)
{
	file_table_t* ppt = PageTableFromLink(pt);

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	pos &= TABLEGUID_UNPOS;

	XDL_ASSERT(pos >= 0 && pos < (int)ppt->cata_size);

	XDL_ASSERT(ppt->cata[pos] & TABLEGUID_OWNED);

	if (xblock_write(ppt->block, 0, TABLEGUID_HEADER + ppt->cata_size + pos * PAGE_SIZE, buf, size))
	{
		ppt->cata[pos] |= TABLEGUID_SAVED;

		_flush_file_table(ppt, 1);
		return 1;
	}

	return 0;
}

#if defined(_DEBUG) || defined(DEBUG)
#include <time.h>
#include "oemrand.h"

void test_file_table(const tchar_t* fname)
{
	link_t_ptr pt = create_file_table(fname, FILE_OPEN_APPEND);

	file_table_t* ppt = PageTableFromLink(pt);

	Srand48(time(NULL));

	int ind[100];
	int ext[100];
	for (int i = 0; i < 100; i++)
	{
		ext[i] = Lrand48() % 100;
		ind[i] = alloc_file_table_block(pt, ext[i] * PAGE_SIZE);
	}

	for (int i = 0; i < 100; i++)
	{
		_tprintf(_T("%d-%d\t"), ind[i] & TABLEGUID_UNPOS, ext[i]);
	}

	_tprintf(_T("\n"));

	for (int i = 0; i < 100; i++)
	{
		free_file_table_block(pt, ind[i], ext[i] * PAGE_SIZE);
	}

	for (int i = 0; i < ppt->cata_size; i++)
	{
		if (ppt->cata[i])
			_tprintf(_T("%d\t"), i);
	}

	_tprintf(_T("\n"));

	destroy_file_table(pt);
}
#endif