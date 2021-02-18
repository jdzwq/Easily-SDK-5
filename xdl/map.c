/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc map document

	@module	map.c | implement file

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

#include "map.h"
#include "varbytes.h"
#include "xdlimp.h"

typedef struct _map_context{
	memobj_head head;

	int bits;
	int nums;
	void* data;
}map_context;

#define MAP_CALC_COLS(bits)	(sizeof(dword_t) * 8 / bits)

int map_calc_rows(int nums, int bits)
{
	int rows, cols;

	cols = MAP_CALC_COLS(bits);
	rows = nums / cols;
	if (nums % cols)
		rows++;

	return rows;
}

int map_calc_size(int nums, int bits)
{
	int rows, cols;

	cols = MAP_CALC_COLS(bits);
	rows = nums / cols;
	if (nums % cols)
		rows++;

	return (rows * cols * bits / 8);
}

map_t map_alloc(int nums, int bits)
{
	map_context* pmm;
	int n;

	if (nums <= 0)
		return NULL;

	if (bits != 1 && bits != 2 && bits != 4 && bits != 8)
		return NULL;

	pmm = (map_context*)xmem_alloc(sizeof(map_context));
	pmm->head.tag = MEM_MAP;
	PUT_THREEBYTE_LOC(pmm->head.len, 0, (sizeof(map_context) - 4));

	pmm->bits = bits;
	pmm->nums = nums;
	
	n = map_calc_size(nums, bits);
	pmm->data = xmem_alloc(n);
	
	return (map_t)&(pmm->head);
}

void map_free(map_t map)
{
	map_context* pmm = TypePtrFromHead(map_context, map);

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);
	
	if (pmm->data)
		xmem_free(pmm->data);

	xmem_free(pmm);
}

map_t map_clone(matrix_t map)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	map_context* pnew;
	int n;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	pnew = (map_context*)map_alloc(pmm->nums, pmm->bits);
	
	n = map_calc_size(pmm->nums, pmm->bits);
	pnew->data = xmem_alloc(n);
	xmem_copy((void*)pnew->data, (void*)pmm->data, n);

	return (map_t)&(pnew->head);
}

void map_zero(map_t map)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	int n;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	n = map_calc_size(pmm->nums, pmm->bits);

	if (pmm->data)
	{
		xmem_zero((void*)pmm->data, n);
	}
}

int map_size(map_t map)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	int n;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	n = map_calc_size(pmm->nums, pmm->bits);

	return n;
}

int map_bits(map_t map)
{
	map_context* pmm = TypePtrFromHead(map_context, map);

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	return pmm->bits;
}

void* map_data(map_t map)
{
	map_context* pmm = TypePtrFromHead(map_context, map);

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	return pmm->data;
}

void map_attach(map_t map, void* data)
{
	map_context* pmm = TypePtrFromHead(map_context, map);

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	if (pmm->data)
		xmem_free(pmm->data);

	pmm->data = data;
}

void* map_detach(map_t map)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	void* d;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	d = pmm->data;
	pmm->data = NULL;

	return d;
}

void map_set_bit(map_t map, int i, byte_t b)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	dword_t* pd;
	int rows, cols;
	int row, col;
	int j;
	dword_t bit, msk;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	pd = (dword_t*)pmm->data;
	cols = MAP_CALC_COLS(pmm->bits);
	rows = map_calc_rows(pmm->nums, pmm->bits);

	row = i / cols;

	XDL_ASSERT(pd != NULL && row < rows);

	msk = 1;
	for (j = 1; j < pmm->bits; j++)
	{
		msk <<= 1;
		msk += 1;
	}

	bit = (b & msk);

	col = i % cols;
	while (col--)
	{
		msk <<= pmm->bits;
		bit <<= pmm->bits;
	}

	pd[row] &= (~msk);
	pd[row] |= bit;
}

byte_t map_get_bit(map_t map, int i)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	dword_t* pd;
	int rows, cols;
	int row, col;
	int j;
	dword_t msk;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	pd = (dword_t*)pmm->data;
	cols = MAP_CALC_COLS(pmm->bits);
	rows = map_calc_rows(pmm->nums, pmm->bits);

	row = i / cols;

	XDL_ASSERT(pd != NULL && row < rows);

	msk = 1;
	for (j = 1; j < pmm->bits; j++)
	{
		msk <<= 1;
		msk += 1;
	}

	col = i % cols;
	for (j = 0; j < col; j++)
		msk <<= pmm->bits;

	msk &= pd[row];
	for (j = 0; j < col; j++)
		msk >>= pmm->bits;

	return (byte_t)msk;
}

int map_find_bit(map_t map, int i, byte_t b)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	dword_t* pd;
	int rows, cols;
	int row, col;
	int j;
	dword_t fix, bit, msk;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	pd = (dword_t*)pmm->data;
	cols = MAP_CALC_COLS(pmm->bits);
	rows = map_calc_rows(pmm->nums, pmm->bits);

	row = i / cols;
	if (!pd || row >= rows)
		return C_ERR;

	fix = 1;
	for (j = 1; j < pmm->bits; j++)
	{
		fix <<= 1;
		fix += 1;
	}

	msk = fix;
	bit = (b & fix);

	col = i % cols;
	while (col--)
	{
		msk <<= pmm->bits;
		bit <<= pmm->bits;
	}

	while (row < rows)
	{
		if (bit == (pd[row] & msk))
			return i;

		i++;
		msk <<= pmm->bits;
		bit <<= pmm->bits;

		if (!(i % cols))
		{
			row++;
			msk = fix;
			bit = (b & fix);
		}
	}

	return C_ERR;
}

int map_test_bit(map_t map, int i, byte_t b, int n)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	dword_t* pd;
	int rows, cols;
	int row, col, k = 0;
	int j;
	dword_t fix, bit, msk;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	pd = (dword_t*)pmm->data;
	cols = MAP_CALC_COLS(pmm->bits);
	rows = map_calc_rows(pmm->nums, pmm->bits);

	row = i / cols;
	if (!pd || row >= rows)
		return C_ERR;

	fix = 1;
	for (j = 1; j < pmm->bits; j++)
	{
		fix <<= 1;
		fix += 1;
	}

	msk = fix;
	bit = (b & fix);

	col = i % cols;
	while (col--)
	{
		msk <<= pmm->bits;
		bit <<= pmm->bits;
	}

	while (k < n && row < rows)
	{
		if (bit != (pd[row] & msk))
			break;

		k++;
		i++;
		msk <<= pmm->bits;
		bit <<= pmm->bits;

		if (!(i % cols))
		{
			row++;
			msk = fix;
			bit = (b & fix);
		}
	}

	return k;
}

void map_parse(map_t map, const tchar_t* str, int len)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	dword_t* pd;
	int rows;
	const tchar_t* token;
	int j, n;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	pd = (dword_t*)pmm->data;
	rows = map_calc_rows(pmm->nums, pmm->bits);

	XDL_ASSERT(pd != NULL);

	if (is_null(str) || !len)
		return;

	if (len < 0)
		len = xslen(str);

	token = str;

	while (*token != _T('{') && *token != _T('\0'))
		token++;

	if (*token == _T('\0'))
		return;

	if (*token == _T('{'))
		token++;

	for (j = 0; j < rows; j++)
	{
		n = 0;
		while (*token != _T(',') && *token != _T('}') && *token != _T('\0'))
		{
			n++;
			token++;
		}

		pd[j] = xsntol(token - n, n);

		if (*token == _T(','))
			token++;

		if (*token == _T('}') || *token == _T('\0'))
			break;
	}
}

int map_format(map_t map, tchar_t* buf, int max)
{
	map_context* pmm = TypePtrFromHead(map_context, map);
	dword_t* pd;
	int rows;
	int j, n;
	int total = 0;

	XDL_ASSERT(map != NULL && map->tag == MEM_MAP);

	pd = (dword_t*)pmm->data;
	rows = map_calc_rows(pmm->nums, pmm->bits);

	XDL_ASSERT(pd != NULL);

	if (total + 1 > max)
		return total;

	if (buf)
	{
		buf[total] = _T('{');
	}
	total++;

	for (j = 0; j < rows; j++)
	{
		n = ltoxs(pd[j], ((buf) ? (buf + total) : NULL), NUM_LEN);
		if (total + n > max)
			return total;

		total += n;

		if (j < rows - 1)
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = _T(',');
			}
			total++;
		}
	}

	if (total + 1 > max)
		return total;

	if (buf)
	{
		xsncat(buf + total, _T("}"), 1);
	}
	total++;

	return total;
}

dword_t map_encode(map_t map, int encode, byte_t* buf, dword_t max)
{
	dword_t n = 0;
	tchar_t* str;
	int len;

	XDL_ASSERT(map != NULL);

	len = map_format(map, NULL, MAX_LONG);
	str = xsalloc(len + 1);
	map_format(map, str, len);

	switch (encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = ucs_to_gb2312(str, len, ((buf) ? buf : NULL), max);
#else
		n = mbs_to_gb2312(str, len, ((buf) ? buf : NULL), max);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = ucs_to_utf8(str, len, ((buf) ? buf : NULL), max);
#else
		n = mbs_to_utf8(str, len, ((buf) ? buf : NULL), max);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = ucs_to_utf16lit(str, len, ((buf) ? buf : NULL), max);
#else
		n = mbs_to_utf16lit(str, len, ((buf) ? buf : NULL), max);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = ucs_to_utf16big(str, len, ((buf) ? buf : NULL), max);
#else
		n = mbs_to_utf16big(str, len, ((buf) ? buf : NULL), max);
#endif
		break;
	}

	xsfree(str);

	return (n);
}

void map_decode(map_t map, int encode, const byte_t* buf, dword_t len)
{
	int n;
	tchar_t* str;

	if (!buf || !len)
	{
		if (map)
		{
			map_zero(map);
		}
		return;
	}

	switch (encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = gb2312_to_ucs(buf, len, NULL, MAX_LONG);
#else
		n = gb2312_to_mbs(buf, len, NULL, MAX_LONG);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = utf8_to_ucs(buf, len, NULL, MAX_LONG);
#else
		n = utf8_to_mbs(buf, len, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = utf16lit_to_ucs(buf, len, NULL, MAX_LONG);
#else
		n = utf16lit_to_mbs(buf, len, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = utf16big_to_ucs(buf, len, NULL, MAX_LONG);
#else
		n = utf16big_to_mbs(buf, len, NULL, MAX_LONG);
#endif
		break;
	}

	str = xsalloc(n + 1);

	switch (encode)
	{
	case _GB2312:
#ifdef _UNICODE
		n = gb2312_to_ucs(buf, len, str, n);
#else
		n = gb2312_to_mbs(buf, len, str, n);
#endif
		break;
	case _UTF8:
#ifdef _UNICODE
		n = utf8_to_ucs(buf, len, str, n);
#else
		n = utf8_to_mbs(buf, len, str, n);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		n = utf16lit_to_ucs(buf, len, str, n);
#else
		n = utf16lit_to_mbs(buf, len, str, n);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		n = utf16big_to_ucs(buf, len, str, n);
#else
		n = utf16big_to_mbs(buf, len, str, n);
#endif
		break;
	}

	if (map)
	{
		map_parse(map, str, n);
	}

	xsfree(str);
}

#if defined(XDL_SUPPORT_TEST)
void test_map(void)
{
	int items = 128;
	int b = 0x01;

	for (int k = 1; k <= 8; k <<= 1)
	{
		map_t pvt = map_alloc(items, k);
		int size = map_size(pvt);

		_tprintf(_T("items:%d bits:%d size:%d mask:%d\n"), items, k, size, b);

		for (int i = 0; i < items; i++)
			map_set_bit(pvt, i, b);

		int rows = items / (32 / k);

		for (int i = 0; i < rows; i++)
			map_set_bit(pvt, i * (32 / k) + i % (32 / k), 0);

		tchar_t* buf;
		int len = map_format(pvt, NULL, MAX_LONG);
		buf = xsalloc(len + 1);
		map_format(pvt, buf, len);

		map_zero(pvt);
		map_parse(pvt, buf, len);
		xsfree(buf);

		for (int i = 0; i < items; i++)
		{
			if (map_get_bit(pvt, i) == b)
				_tprintf(_T("1"));
			else
				_tprintf(_T("0"));

			if (!((i + 1) % (32 / k)))
				_tprintf(_T("\n"));
		}

		b <<= 1;
		map_free(pvt);
	}
}
#endif