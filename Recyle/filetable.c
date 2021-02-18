/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc filetable document

	@module	filetable.c | implement file

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
#include "xdlmath.h"

#include "xdlinit.h"

#define TABLEGUID_MAPBITS		1
#define TABLEGUID_INCRES		(16 * 1024)

#define TABLEGUID_ALLOCED		0x01
#define TABLEGUID_UNALLOCED		0x00

//#define TABLEGUID_POSED			(1 << 15)
//#define TABLEGUID_UNPOS			(~(1 << 15))

typedef struct _map_table_t{
	int pos;
	map_t map;
}map_table_t;

typedef struct _file_table_context{
	link_t lk;

	xhand_t block;

	dword_t file_guid;
	dword_t file_root;

	sword_t file_incr;
	sword_t page_size;
	sword_t map_size;
	sword_t map_bits;
	sword_t map_count;

	map_table_t* map_table;
}file_table_context;

#define PageTableFromLink(p) TypePtrFromLink(file_table_context,p)

static void _default_file_head(file_table_context* ppt)
{
	ppt->file_guid = 0;
	ppt->file_root = 0;

	ppt->file_incr = TABLEGUID_INCRES;
	ppt->page_size = PAGE_SIZE;
	ppt->map_size = (sword_t)map_calc_size(TABLEGUID_INCRES, TABLEGUID_MAPBITS);
	ppt->map_bits = TABLEGUID_MAPBITS;
	ppt->map_count = 1;
	ppt->map_table = (map_table_t*)xmem_alloc((ppt->map_count) * sizeof(map_table_t));
	ppt->map_table[0].map = map_alloc(TABLEGUID_INCRES, TABLEGUID_MAPBITS);
	map_set_bit(ppt->map_table[0].map, 0, TABLEGUID_ALLOCED);
	ppt->map_table[0].pos = 1;
}

static bool_t _flush_file_head(file_table_context* ppt, bool_t b_save)
{
	byte_t head[PAGE_SIZE] = { 0 };

	if (b_save)
	{
		PUT_DWORD_LOC(head, 0, ppt->file_guid);
		PUT_DWORD_LOC(head, 4, ppt->file_root);

		PUT_SWORD_LOC(head, 8, ppt->file_incr);
		PUT_SWORD_LOC(head, 10, ppt->page_size);
		PUT_SWORD_LOC(head, 12, ppt->map_size);
		PUT_SWORD_LOC(head, 14, ppt->map_bits);
		PUT_SWORD_LOC(head, 16, ppt->map_count);

		return xuncf_write_file_range(ppt->block, 0, 0, head, ppt->page_size - ppt->map_size);
	}
	else
	{
		if (!xuncf_read_file_range(ppt->block, 0, 0, head, PAGE_SIZE))
			return 0;

		ppt->file_guid = GET_DWORD_LOC(head, 0);
		ppt->file_root = GET_DWORD_LOC(head, 4);

		ppt->file_incr = GET_SWORD_LOC(head, 8);
		ppt->page_size = GET_DWORD_LOC(head, 10);
		ppt->map_size = GET_SWORD_LOC(head, 12);
		ppt->map_bits = GET_SWORD_LOC(head, 14);
		ppt->map_count = GET_SWORD_LOC(head, 16);

		return 1;
	}
}

static bool_t _flush_file_map(file_table_context* ppt, int i, bool_t b_save)
{
	byte_t head[PAGE_SIZE] = { 0 };
	lword_t ll;
	dword_t hoff, loff;
	byte_t* buff;

	XDL_ASSERT(i >= 0 && i < ppt->map_count);

	ll = (ppt->file_incr * ppt->page_size) * i + ppt->page_size - ppt->map_size;

	hoff = GETHDWORD(ll);
	loff = GETLDWORD(ll);

	if (b_save)
	{
		buff = map_data(ppt->map_table[i].map);
		xmem_copy((void*)(head + ppt->page_size - ppt->map_size), (void*)(buff), ppt->map_size);

		return xuncf_write_file_range(ppt->block, hoff, loff, (head + ppt->page_size - ppt->map_size), ppt->map_size);
	}
	else
	{
		if (!xuncf_read_file_range(ppt->block, hoff, loff, (head + ppt->page_size - ppt->map_size), ppt->map_size))
			return 0;

		buff = map_data(ppt->map_table[i].map);
		xmem_copy((void*)(buff), (void*)(head + ppt->page_size - ppt->map_size), ppt->map_size);
		return 1;
	}
}

/************************************************************************************/
link_t_ptr create_file_table(const tchar_t* fname)
{
	file_table_context* ppt = NULL;

	dword_t dwh = 0,dwl = 0;
	int i;

	TRY_CATCH;

	ppt = (file_table_context*)xmem_alloc(sizeof(file_table_context));
	ppt->lk.tag = lkFileTable;

	ppt->block = xuncf_open_file(NULL, fname, FILE_OPEN_APPEND | FILE_OPEN_RANDOM);
	if (!ppt->block)
	{
		raise_user_error(NULL, NULL);
	}

	xuncf_file_size(ppt->block, &dwh, &dwl);

	if (dwl && dwl < PAGE_SIZE)
	{
		raise_user_error(_T("open_file_table"), _T("invalid file size"));
	}

	if (dwl)
	{
		//load file table header
		if (!_flush_file_head(ppt, 0))
		{
			raise_user_error(_T("open_file_table"), _T("invalid file header"));
		}

		if (!ppt->map_size || ppt->map_size > ppt->page_size / 2)
		{
			raise_user_error(_T("open_file_table"), _T("invalid map size"));
		}

		ppt->map_table = (map_table_t*)xmem_alloc((ppt->map_count) * sizeof(map_table_t));
		
		for (i = 0; i < ppt->map_count; i++)
		{
			ppt->map_table[i].map = map_alloc(ppt->file_incr, ppt->map_bits);

			if (!_flush_file_map(ppt, i, 0))
			{
				raise_user_error(_T("open_file_table"), _T("invalid file maps"));
			}

			map_set_bit(ppt->map_table[i].map, 0, TABLEGUID_ALLOCED);
			ppt->map_table[i].pos = 1;
		}
	}
	else
	{
		//set default header
		_default_file_head(ppt);

		//save file table header
		if (!_flush_file_head(ppt, 1))
		{
			raise_user_error(_T("open_file_table"), _T("save file header failed"));
		}

		if (!_flush_file_map(ppt, 0, 1))
		{
			raise_user_error(_T("open_file_table"), _T("save file maps failed"));
		}
	}

	END_CATCH;

	return &ppt->lk;
ONERROR:

	if (ppt->block)
		xuncf_close_file(ppt->block);

	if (ppt->map_table)
	{
		map_free(ppt->map_table[0].map);
		xmem_free(ppt->map_table);
	}

	if (ppt)
		xmem_free(ppt);

	return NULL;
}

void destroy_file_table(link_t_ptr pt)
{
	file_table_context* ppt = PageTableFromLink(pt);
	int i;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	xuncf_close_file(ppt->block);

	for (i = 0; i < ppt->map_count; i++)
	{
		map_free(ppt->map_table[i].map);
	}

	xmem_free(ppt->map_table);

	xmem_free(ppt);
}

void set_file_table_root(link_t_ptr pt, dword_t pos)
{
	file_table_context* ppt = PageTableFromLink(pt);
	bool_t rt;

	ppt->file_root = pos;

	if (pos)
	{
		rt = get_file_table_block_alloced(pt, pos);

		XDL_ASSERT(rt);
	}

	_flush_file_head(ppt, 1);
}

dword_t get_file_table_root(link_t_ptr pt)
{
	file_table_context* ppt = PageTableFromLink(pt);
	bool_t rt;

	if (ppt->file_root)
	{
		rt = get_file_table_block_alloced(pt, ppt->file_root);

		XDL_ASSERT(rt);
	}

	return ppt->file_root;
}

dword_t alloc_file_table_block(link_t_ptr pt, dword_t size)
{
	file_table_context* ppt = PageTableFromLink(pt);
	int k, n, pos;
	sword_t i;
	bool_t tag = 0;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	n = size / ppt->page_size;
	if (size % ppt->page_size)
		n++;

	for (i = 0; i < ppt->map_count; i++)
	{
		pos = (n > 1) ? ppt->map_table[i].pos : 1;

		while (n)
		{
			pos = map_find_bit(ppt->map_table[i].map, pos, TABLEGUID_UNALLOCED);
			if (pos == C_ERR)
				break;

			if (n == 1)
			{
				tag = 1;
				break;
			}

			k = map_test_bit(ppt->map_table[i].map, pos, TABLEGUID_UNALLOCED, n);
			if (k == n)
			{
				tag = 1;
				break;
			}

			pos += (k + 1);
		}

		if (tag) break;
	}

	if (i == ppt->map_count)
	{
		ppt->map_count++;
		ppt->map_table = (map_table_t*)xmem_realloc(ppt->map_table, (ppt->map_count) * sizeof(map_table_t));
		ppt->map_table[i].map = map_alloc(ppt->file_incr, ppt->map_bits);
		map_set_bit(ppt->map_table[i].map, 0, TABLEGUID_ALLOCED);
		ppt->map_table[i].pos = 1;
		pos = 1;

		_flush_file_head(ppt, 1);
	}

	XDL_ASSERT(pos > 0);

	//cache the map pos
	ppt->map_table[i].pos = (pos + n) % ppt->map_size;

	while (n--)
	{
		map_set_bit(ppt->map_table[i].map, pos + n, TABLEGUID_ALLOCED);
	}

	_flush_file_map(ppt, i, 1);

	return (int)(pos | (i << 16));
}

void free_file_table_block(link_t_ptr pt, dword_t pos, dword_t size)
{
	file_table_context* ppt = PageTableFromLink(pt);
	int n;
	sword_t i;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	i = (sword_t)((pos & 0xFFFF0000) >> 16);
	pos &= 0x0000FFFF;

	XDL_ASSERT(pos > 0 && i < ppt->map_count);

	n = size / ppt->page_size;
	if (size % ppt->page_size)
		n++;

	while (n--)
	{
		XDL_ASSERT(map_get_bit(ppt->map_table[i].map, pos + n) != TABLEGUID_UNALLOCED);
		map_set_bit(ppt->map_table[i].map, pos + n, TABLEGUID_UNALLOCED);
	}

	//cache the map pos
	ppt->map_table[i].pos = pos;

	_flush_file_map(ppt, i, 1);
}

bool_t get_file_table_block_alloced(link_t_ptr pt, dword_t pos)
{
	file_table_context* ppt = PageTableFromLink(pt);
	sword_t i;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	i = (sword_t)((pos & 0xFFFF0000) >> 16);
	pos &= 0x0000FFFF;

	XDL_ASSERT(pos > 0 && i < ppt->map_count);

	return (map_get_bit(ppt->map_table[i].map, pos) == TABLEGUID_UNALLOCED)? 0 : 1;
}

bool_t read_file_table_block(link_t_ptr pt, dword_t pos, byte_t* buf, dword_t size)
{
	file_table_context* ppt = PageTableFromLink(pt);
	sword_t i;
	lword_t ll;
	dword_t hoff, loff;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	i = (sword_t)((pos & 0xFFFF0000) >> 16);
	pos &= 0x0000FFFF;

	XDL_ASSERT(pos > 0 && i < ppt->map_count);

	XDL_ASSERT(map_get_bit(ppt->map_table[i].map, pos) != TABLEGUID_UNALLOCED);
	
	ll = i * ppt->file_incr * PAGE_SIZE + pos * PAGE_SIZE;
	hoff = GETHDWORD(ll);
	loff = GETLDWORD(ll);

	return xuncf_read_file_range(ppt->block, hoff, loff, buf, size);
}

bool_t write_file_table_block(link_t_ptr pt, dword_t pos, byte_t* buf, dword_t size)
{
	file_table_context* ppt = PageTableFromLink(pt);
	sword_t i;
	lword_t ll;
	dword_t hoff, loff;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	i = (sword_t)((pos & 0xFFFF0000) >> 16);
	pos &= 0x0000FFFF;

	XDL_ASSERT(pos > 0 && i < ppt->map_count);

	XDL_ASSERT(map_get_bit(ppt->map_table[i].map, pos) != TABLEGUID_UNALLOCED);

	ll = i * ppt->file_incr * PAGE_SIZE + pos * PAGE_SIZE;
	hoff = GETHDWORD(ll);
	loff = GETLDWORD(ll);

	return xuncf_write_file_range(ppt->block, hoff, loff, buf, size);
}

#if defined(_DEBUG) || defined(DEBUG)
#include <time.h>

void test_file_table(const tchar_t* fname)
{
	link_t_ptr pt = create_file_table(fname);

	file_table_context* ppt = PageTableFromLink(pt);

	Srand48(time(NULL));

	#define ARS 1000

	int ind[ARS];
	int ext[ARS];
	for (int i = 0; i < ARS; i++)
	{
		ext[i] = Lrand48() % 100;
		ind[i] = alloc_file_table_block(pt, ext[i] * PAGE_SIZE);

		_tprintf(_T("%d-%d-%d\t"), (ind[i] & 0x0000FFFF), ext[i], (int)get_file_table_block_alloced(pt, ind[i]));
	}

	_tprintf(_T("\n"));

	for (int i = 0; i < ARS; i++)
	{
		free_file_table_block(pt, ind[i], ext[i] * PAGE_SIZE);

		_tprintf(_T("%d-%d-%d\t"), (ind[i] & 0x0000FFFF), ext[i], (int)get_file_table_block_alloced(pt, ind[i]));
	}

	_tprintf(_T("\n"));

	destroy_file_table(pt);
}
#endif