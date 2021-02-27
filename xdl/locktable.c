/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc locktable document

	@module	locktable.c | implement lock

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

#include "locktable.h"
#include "nuid.h"
#include "xdlimp.h"
#include "xdlmath.h"

#include "xdlinit.h"

#define GUID_MEM_PREFIX		_T("mem-")

#define LOCKTABLE_MAPBITS		2

#define LOCKTABLE_TAG_WAIT		0x01
#define LOCKTABLE_TAG_LOCK		0x02
#define LOCKTABLE_TAG_BITMASK	0x03

typedef struct _lock_table_context{
	link_t lk;

	xhand_t share;

	map_t map;
	dword_t map_size;
	tchar_t guid[NUID_TOKEN_SIZE + 1];
}lock_table_context;

#define LockTableFromLink(p) TypePtrFromLink(lock_table_context,p)

/************************************************************************************/

link_t_ptr create_lock_table(const tchar_t* guid, int map_nums)
{
	lock_table_context* ppt = NULL;

	tchar_t token[NUID_TOKEN_SIZE + 5] = { 0 };

	TRY_CATCH;

	ppt = (lock_table_context*)xmem_alloc(sizeof(lock_table_context));
	ppt->lk.tag = lkLockTable;

	xsncpy(ppt->guid, guid, NUID_TOKEN_SIZE);

	ppt->map = map_alloc(map_nums, LOCKTABLE_MAPBITS);
	if (!ppt->map)
	{
		raise_user_error(_T("create_lock_table"), _T("alloc map object failed"));
	}
	ppt->map_size = map_size(ppt->map);

	xsprintf(token, _T("%s%s"), GUID_MEM_PREFIX, ppt->guid);
	ppt->share = xshare_cli(token, MAX_LONG, FILE_OPEN_CREATE);
	if (!ppt->share)
	{
		raise_user_error(_T("create_lock_table"), _T("alloc share object failed"));
	}

	END_CATCH;

	return &ppt->lk;
ONERROR:

	if (ppt->share)
		xshare_close(ppt->share);

	if (ppt->map)
		map_free(ppt->map);

	if (ppt)
		xmem_free(ppt);

	return NULL;
}

void destroy_lock_table(link_t_ptr pt)
{
	lock_table_context* ppt = LockTableFromLink(pt);

	tchar_t token[NUID_TOKEN_SIZE + 5] = { 0 };

	XDL_ASSERT(pt && pt->tag == lkLockTable);

	xshare_close(ppt->share);

	map_free(ppt->map);

	xmem_free(ppt);
}

bool_t enter_lock_table(link_t_ptr pt, int map_ind, int map_pos)
{
	lock_table_context* ppt = LockTableFromLink(pt);

	dword_t offs;
	byte_t *buff;
	byte_t tag;
	bool_t b;
	void* data;

	XDL_ASSERT(pt && pt->tag == lkLockTable);

	offs = map_ind * ppt->map_size;
	buff = xshare_lock(ppt->share, offs, ppt->map_size);

	XDL_ASSERT(buff != NULL);

	data = map_detach(ppt->map);
	map_attach(ppt->map, (void*)buff);

	tag = map_get_bit(ppt->map, map_pos);
	tag &= LOCKTABLE_TAG_BITMASK;
	
	b = (tag & LOCKTABLE_TAG_WAIT) ? 0 : 1;

	tag >>= 1;
	map_set_bit(ppt->map, map_pos, tag);

	if (b)
	{
		while ((tag & LOCKTABLE_TAG_WAIT))
		{
			tag = map_get_bit(ppt->map, map_pos);
			tag &= LOCKTABLE_TAG_BITMASK;
		}

		map_set_bit(ppt->map, map_pos, LOCKTABLE_TAG_LOCK);
	}

	buff = (byte_t*)map_detach(ppt->map);
	map_attach(ppt->map, data);

	xshare_unlock(ppt->share, offs, ppt->map_size, buff);

	return b;
}

void leave_lock_table(link_t_ptr pt, int map_ind, int map_pos)
{
	lock_table_context* ppt = LockTableFromLink(pt);

	dword_t offs;
	byte_t *buff;
	byte_t tag;
	void* data;

	XDL_ASSERT(pt && pt->tag == lkLockTable);

	offs = map_ind * ppt->map_size;
	buff = xshare_lock(ppt->share, offs, ppt->map_size);

	XDL_ASSERT(buff != NULL);

	data = map_detach(ppt->map);
	map_attach(ppt->map, (void*)buff);

	tag = map_get_bit(ppt->map, map_pos);
	tag &= LOCKTABLE_TAG_BITMASK;

	if (tag)
	{
		tag = 0;
		map_set_bit(ppt->map, map_pos, tag);
	}

	buff = (byte_t*)map_detach(ppt->map);
	map_attach(ppt->map, data);

	xshare_unlock(ppt->share, offs, ppt->map_size, buff);
}

#if defined(XDL_SUPPORT_TEST)

void test_lock_table()
{
	lword_t tms;
	nuid_t nuid = { 0 };
	tchar_t token[NUID_TOKEN_SIZE + 1] = { 0 };

	tms = get_timestamp();
	nuid_from_timestamp(&nuid, tms);
	nuid_format_string(&nuid, token);

	int nums = 4096;
	link_t_ptr lt = create_lock_table(token, nums);
	bool_t rt;
	int i, k, j;

	for (k = 0; k < 1024; k++)
	{
		for (i = 0; i < nums; i++)
		{
			for (j = 0; j < 2; j++)
			{
				rt = enter_lock_table(lt, k, i);
				_tprintf(_T("map:%d pos:%d return:%d\n"), k, i, rt);
				//if (j % 2 && rt)
					//goto err;
				leave_lock_table(lt, k, i);
			}
		}
	}

//err:

	destroy_lock_table(lt);
}
#endif