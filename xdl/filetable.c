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
#include "nuid.h"
#include "locktable.h"
#include "xdlimp.h"
#include "xdlmath.h"

#include "xdlinit.h"

#define FILETABLE_MAPBITS		1

#define FILETABLE_ALLOCED		0x01
#define FILETABLE_RELEASED		0x00


typedef struct _block_map_t{
	int tag; //map is loaded
	int pos; //map index
	int inc; //page counter
	map_t map;
}block_map_t;

typedef struct _file_table_context{
	link_t lk;

	xhand_t block;
	link_t_ptr lock_table;

	dword_t file_root;
	sword_t page_size;
	sword_t mask_bits;
	sword_t file_maps;

	tchar_t file_guid[NUID_TOKEN_SIZE + 1];

	block_map_t* block_table;
}file_table_context;

#define PageTableFromLink(p) TypePtrFromLink(file_table_context,p)

#define CALC_MAP_ITEMS(pagesize, maskbits)	(pagesize * (8 / maskbits))

static void _alloc_block_map(block_map_t* pmt, int page_size, int mask_bits, int i)
{
	pmt->tag = 0;
	pmt->inc = CALC_MAP_ITEMS(page_size, mask_bits);
	pmt->map = map_alloc(pmt->inc, mask_bits);
	map_set_bit(pmt->map, 0, FILETABLE_ALLOCED);
	pmt->pos = 1;
}

static void _default_file_head(file_table_context* ppt)
{
	lword_t tms;
	nuid_t nuid = { 0 };
	
	ppt->file_root = 0;
	ppt->page_size = PAGE_SIZE;
	ppt->mask_bits = FILETABLE_MAPBITS;
	ppt->file_maps = 1;

	tms = get_timestamp();
	nuid_from_timestamp(&nuid, tms);
	nuid_format_string(&nuid, ppt->file_guid);

	ppt->block_table = (block_map_t*)xmem_alloc((ppt->file_maps) * sizeof(block_map_t));
	_alloc_block_map(&(ppt->block_table[0]), ppt->page_size, ppt->mask_bits, 0);
}

static bool_t _flush_file_head(file_table_context* ppt, bool_t b_save)
{
	byte_t head[PAGE_SIZE] = { 0 };

	if (b_save)
	{
		PUT_DWORD_LOC(head, 0, ppt->file_root);
		PUT_SWORD_LOC(head, 4, ppt->page_size);
		PUT_SWORD_LOC(head, 6, ppt->mask_bits);
		PUT_SWORD_LOC(head, 8, ppt->file_maps);
#if defined(_UNICODE) || defined(UNICODE)
		ucs_to_utf8(ppt->file_guid, NUID_TOKEN_SIZE, (head + 10), NUID_TOKEN_SIZE);
#else
		mbs_to_utf8(ppt->file_guid, NUID_TOKEN_SIZE, (head + 10), NUID_TOKEN_SIZE);
#endif

		return xuncf_write_file_range(ppt->block, 0, 0, head, ppt->page_size);
	}
	else
	{
		if (!xuncf_read_file_range(ppt->block, 0, 0, head, PAGE_SIZE))
			return 0;

		ppt->file_root = GET_DWORD_LOC(head, 0);
		ppt->page_size = GET_DWORD_LOC(head, 4);
		ppt->mask_bits = GET_SWORD_LOC(head, 6);
		ppt->file_maps = GET_SWORD_LOC(head, 8);
#if defined(_UNICODE) || defined(UNICODE)
		utf8_to_ucs((head + 10), NUID_TOKEN_SIZE, ppt->file_guid, NUID_TOKEN_SIZE);
#else
		utf8_to_mbs((head + 10), NUID_TOKEN_SIZE, ppt->file_guid, NUID_TOKEN_SIZE);
#endif

		return (ppt->page_size <= PAGE_SIZE && (ppt->mask_bits == 1 || ppt->mask_bits == 2 || ppt->mask_bits == 4 || ppt->mask_bits == 8)) ? 1 : 0;
	}
}

static bool_t _flush_file_map(file_table_context* ppt, int map_ind, bool_t b_save)
{
	lword_t ll;
	dword_t hoff, loff;
	byte_t* buff;
	sword_t bytes;
	int j;
	dword_t incr;
	bool_t rt;

	XDL_ASSERT(map_ind >= 0 && map_ind < ppt->file_maps);

	incr = 1;
	for (j = 0; j < map_ind; j++)
	{
		incr += ppt->block_table[j].inc;
	}

	buff = map_data(ppt->block_table[map_ind].map);
	bytes = map_size(ppt->block_table[map_ind].map);

	ll = incr * ppt->page_size + (ppt->page_size - bytes);

	hoff = GETHDWORD(ll);
	loff = GETLDWORD(ll);

	if (ppt->lock_table)
	{
		while (!enter_lock_table(ppt->lock_table, map_ind, 0))
		{
			thread_yield();
		}
	}

	if (b_save)
	{
		rt = xuncf_write_file_range(ppt->block, hoff, loff, buff, bytes);
	}
	else
	{
		rt = xuncf_read_file_range(ppt->block, hoff, loff, buff, bytes);
	}

	if (ppt->lock_table)
	{
		leave_lock_table(ppt->lock_table, map_ind, 0);
	}

	if (rt)
	{
		ppt->block_table[map_ind].tag = 1;
	}

	return rt;
}

static bool_t _flush_file_block(file_table_context* ppt, int map_ind, int map_pos, byte_t* buf, dword_t size, bool_t b_save)
{
	lword_t ll;
	dword_t hoff, loff;
	dword_t incr;
	int j;
	bool_t rt;

	incr = 1;
	for (j = 0; j < map_ind; j++)
	{
		incr += ppt->block_table[j].inc;
	}

	ll = (incr + map_pos) * ppt->page_size;
	hoff = GETHDWORD(ll);
	loff = GETLDWORD(ll);

	if (ppt->lock_table)
	{
		while (!enter_lock_table(ppt->lock_table, map_ind, map_pos))
		{
			thread_yield();
		}
	}

	if (b_save)
	{
		rt = xuncf_write_file_range(ppt->block, hoff, loff, buf, size);
	}
	else
	{
		rt = xuncf_read_file_range(ppt->block, hoff, loff, buf, size);
	}

	if (ppt->lock_table)
	{
		leave_lock_table(ppt->lock_table, map_ind, map_pos);
	}

	return rt;
}

/************************************************************************************/

link_t_ptr create_file_table(const tchar_t* fname, bool_t share)
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

		if (!ppt->page_size || !ppt->file_maps || ppt->mask_bits > 8)
		{
			raise_user_error(_T("open_file_table"), _T("invalid file header"));
		}

		if (share)
		{
			ppt->lock_table = create_lock_table(ppt->file_guid, CALC_MAP_ITEMS(ppt->page_size, ppt->mask_bits));
			if (!ppt->lock_table)
			{
				raise_user_error(_T("open_file_table"), _T("create lock table failed"));
			}
		}

		ppt->block_table = (block_map_t*)xmem_alloc((ppt->file_maps) * sizeof(block_map_t));
		
		for (i = 0; i < ppt->file_maps; i++)
		{
			_alloc_block_map(&(ppt->block_table[i]), ppt->page_size, ppt->mask_bits, i);

			_flush_file_map(ppt, i, 0);
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

		if (share)
		{
			ppt->lock_table = create_lock_table(ppt->file_guid, CALC_MAP_ITEMS(ppt->page_size, ppt->mask_bits));
			if (!ppt->lock_table)
			{
				raise_user_error(_T("open_file_table"), _T("create lock table failed"));
			}
		}

		if (!_flush_file_map(ppt, 0, 1))
		{
			raise_user_error(_T("open_file_table"), _T("save file map failed"));
		}
	}

	END_CATCH;

	return &ppt->lk;
ONERROR:

	if (ppt->block)
		xuncf_close_file(ppt->block);

	if (ppt->lock_table)
		destroy_lock_table(ppt->lock_table);

	if (ppt->block_table)
	{
		map_free(ppt->block_table[0].map);
		xmem_free(ppt->block_table);
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

	if (ppt->lock_table)
	{
		destroy_lock_table(ppt->lock_table);
	}

	for (i = 0; i < ppt->file_maps; i++)
	{
		map_free(ppt->block_table[i].map);
	}

	xmem_free(ppt->block_table);

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
	int k, n, pos = 0;
	sword_t i;
	bool_t tag = 0;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	n = size / ppt->page_size;
	if (size % ppt->page_size)
		n++;

	for (i = 0; i < ppt->file_maps; i++)
	{
		if (!_flush_file_map(ppt, i, 0))
			continue;

		pos = (n > 1) ? ppt->block_table[i].pos : 1;

		while (n)
		{
			pos = map_find_bit(ppt->block_table[i].map, pos, FILETABLE_RELEASED);
			if (pos == C_ERR)
				break;

			if (n == 1)
			{
				tag = 1;
				break;
			}

			k = map_test_bit(ppt->block_table[i].map, pos, FILETABLE_RELEASED, n);
			if (k == n)
			{
				tag = 1;
				break;
			}

			pos += (k + 1);
		}

		if (tag) break;
	}

	if (i == ppt->file_maps)
	{
		ppt->file_maps++;
		ppt->block_table = (block_map_t*)xmem_realloc(ppt->block_table, (ppt->file_maps) * sizeof(block_map_t));
		_alloc_block_map(&(ppt->block_table[i]), ppt->page_size, ppt->mask_bits, i);
		
		pos = 1;

		_flush_file_head(ppt, 1);
	}

	XDL_ASSERT(pos > 0);

	//cache the map pos
	ppt->block_table[i].pos = (pos + n) % ppt->file_maps;

	while (n--)
	{
		map_set_bit(ppt->block_table[i].map, pos + n, FILETABLE_ALLOCED);
	}

	_flush_file_map(ppt, i, 1);

	return (dword_t)(pos | (i << 16));
}

void free_file_table_block(link_t_ptr pt, dword_t pos, dword_t size)
{
	file_table_context* ppt = PageTableFromLink(pt);
	int n;
	sword_t i;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	i = (sword_t)((pos & 0xFFFF0000) >> 16);
	pos &= 0x0000FFFF;

	XDL_ASSERT(pos > 0 && i < ppt->file_maps);

	n = size / ppt->page_size;
	if (size % ppt->page_size)
		n++;

	while (n--)
	{
		XDL_ASSERT(map_get_bit(ppt->block_table[i].map, pos + n) != FILETABLE_RELEASED);
		map_set_bit(ppt->block_table[i].map, pos + n, FILETABLE_RELEASED);
	}

	//cache the map pos
	ppt->block_table[i].pos = pos;

	_flush_file_map(ppt, i, 1);
}

bool_t get_file_table_block_alloced(link_t_ptr pt, dword_t pos)
{
	file_table_context* ppt = PageTableFromLink(pt);
	sword_t i;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	i = (sword_t)((pos & 0xFFFF0000) >> 16);
	pos &= 0x0000FFFF;

	XDL_ASSERT(pos > 0 && i < ppt->file_maps);

	_flush_file_map(ppt, i, 0);

	return (map_get_bit(ppt->block_table[i].map, pos) == FILETABLE_RELEASED)? 0 : 1;
}

bool_t read_file_table_block(link_t_ptr pt, dword_t pos, byte_t* buf, dword_t size)
{
	file_table_context* ppt = PageTableFromLink(pt);
	int i, j;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	i = (int)((pos & 0xFFFF0000) >> 16);
	j = (int)(pos & 0x0000FFFF);

	XDL_ASSERT(j > 0 && i < ppt->file_maps);
	XDL_ASSERT((ppt->block_table[i].tag) != 0);
	XDL_ASSERT(map_get_bit(ppt->block_table[i].map, j) != FILETABLE_RELEASED);
	
	return _flush_file_block(ppt, i, j, buf, size, 0);
}

bool_t write_file_table_block(link_t_ptr pt, dword_t pos, byte_t* buf, dword_t size)
{
	file_table_context* ppt = PageTableFromLink(pt);
	int i, j;

	XDL_ASSERT(pt && pt->tag == lkFileTable);

	i = (int)((pos & 0xFFFF0000) >> 16);
	j = (int)(pos & 0x0000FFFF);

	XDL_ASSERT(j > 0 && i < ppt->file_maps);
	XDL_ASSERT((ppt->block_table[i].tag) != 0);
	XDL_ASSERT(map_get_bit(ppt->block_table[i].map, j) != FILETABLE_RELEASED);

	return _flush_file_block(ppt, i, j, buf, size, 1);
}

#if defined(XDL_SUPPORT_TEST)
#include <time.h>

void test_file_table(const tchar_t* fname, bool_t share)
{
	link_t_ptr pt = create_file_table(fname, share);

	file_table_context* ppt = PageTableFromLink(pt);

	Srand48(time(NULL));

	#define ARS 1000

	for (int k = 0; k < 10; k++)
	{
		int ind[ARS];
		int ext[ARS];
		for (int i = 0; i < ARS; i++)
		{
			ext[i] = Lrand48() % 1000;
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
	}

	destroy_file_table(pt);
}
#endif