/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc map document

	@module	map.c | map implement file

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
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#define MAP_SIZE(ppt)	(pmm->rows * pmm->cols * pmm->bits / 8)

dword_t map_size(int nums, int bits)
{
	int rows, cols;

	cols = sizeof(dword_t) * 8 / bits;
	rows = nums / cols;
	if (nums % cols)
		rows++;

	return (rows * cols * bits / 8);
}

map_t* map_alloc(int nums, int bits)
{
	map_t* pmm;

	if (nums <= 0)
		return NULL;

	if (bits <= 0 || bits > 8)
		return NULL;

	pmm = (map_t*)xmem_alloc(sizeof(map_t));
	
	pmm->bits = bits;
	pmm->cols = sizeof(dword_t) * 8 / pmm->bits;
	pmm->rows = nums / pmm->cols;
	if (nums % pmm->cols)
		pmm->rows++;

	pmm->data = (dword_t*)xmem_alloc(MAP_SIZE(ppt));

	return pmm;
}

void map_free(map_t* pmm)
{
	xmem_free(pmm->data);
	xmem_free(pmm);
}

void map_zero(map_t* pmm)
{
	xmem_zero(pmm->data, MAP_SIZE(pmm));
}

void map_set_bits(map_t* pmm, int i, byte_t b)
{
	int row, col;
	int j;
	dword_t bit, msk;

	XDL_ASSERT(i >= 0);

	row = i / pmm->cols;
	if (row >= pmm->rows)
		return;

	msk = 1;
	for (j = 1; j < pmm->bits; j++)
	{
		msk <<= 1;
		msk += 1;
	}

	bit = (b & msk);

	col = i % pmm->cols;
	while (col--)
	{
		msk <<= pmm->bits;
		bit <<= pmm->bits;
	}

	(pmm->data)[row] &= (~msk);
	(pmm->data)[row] |= bit;
}

byte_t map_get_bits(map_t* pmm, int i)
{
	int row, col;
	int j;
	dword_t msk;

	XDL_ASSERT(i >= 0);

	row = i / pmm->cols;
	if (row >= pmm->rows)
		return 0;

	msk = 1;
	for (j = 1; j < pmm->bits; j++)
	{
		msk <<= 1;
		msk += 1;
	}

	col = i % pmm->cols;
	for (j = 0; j < col;j++)
		msk <<= pmm->bits;

	msk &= (pmm->data)[row];
	for (j = 0; j < col; j++)
		msk >>= pmm->bits;

	return (byte_t)msk;
}

int map_find_bits(map_t* pmm, int i, byte_t b)
{
	int row, col;
	int j;
	dword_t fix, bit, msk;

	XDL_ASSERT(i >= 0);

	row = i / pmm->cols;
	if (row >= pmm->rows)
		return C_ERR;

	fix = 1;
	for (j = 1; j < pmm->bits; j++)
	{
		fix <<= 1;
		fix += 1;
	}

	msk = fix;
	bit = (b & fix);

	col = i % pmm->cols;
	while (col--)
	{
		msk <<= pmm->bits;
		bit <<= pmm->bits;
	}

	while (row < pmm->rows)
	{
		if (bit == ((pmm->data)[row] & msk))
			return i;

		i++;
		msk <<= pmm->bits;
		bit <<= pmm->bits;

		if (!(i % pmm->cols))
		{
			row++;
			msk = fix;
			bit = (b & fix);
		}
	}

	return C_ERR;
}

int map_test_bits(map_t* pmm, int i, byte_t b, int n)
{
	int row, col, k = 0;
	int j;
	dword_t fix, bit, msk;

	XDL_ASSERT(i >= 0 && n >= 0);

	row = i / pmm->cols;
	if (row >= pmm->rows)
		return C_ERR;

	fix = 1;
	for (j = 1; j < pmm->bits; j++)
	{
		fix <<= 1;
		fix += 1;
	}

	msk = fix;
	bit = (b & fix);

	col = i % pmm->cols;
	while (col--)
	{
		msk <<= pmm->bits;
		bit <<= pmm->bits;
	}

	while (k < n && row < pmm->rows)
	{
		if (bit != ((pmm->data)[row] & msk))
			break;

		k++;
		i++;
		msk <<= pmm->bits;
		bit <<= pmm->bits;

		if (!(i % pmm->cols))
		{
			row++;
			msk = fix;
			bit = (b & fix);
		}
	}

	return k;
}

#if defined(_DEBUG) || defined(DEBUG)
void test_map(void)
{
	int k = 2;
	byte_t bits = 0x03;
	map_t* pvt = map_alloc(256, k);

	for (int i = 0; i < 256; i++)
		map_set_bits(pvt, i, bits);

	for (int i = 0; i < 256; i++)
	{
		if (map_get_bits(pvt, i) == bits)
			_tprintf(_T("1"));
		else
			_tprintf(_T("0"));

		if (!((i + 1) % (32/k)))
			_tprintf(_T("\n"));
	}

	for (int i = 0; i < 8; i++)
		map_set_bits(pvt, i * (32/k) + i, 0);

	for (int i = 0; i < 256; i++)
	{
		if (map_get_bits(pvt, i) == bits)
			_tprintf(_T("1"));
		else
			_tprintf(_T("0"));

		if (!((i + 1) % (32/k)))
			_tprintf(_T("\n"));
	}

	map_free(pvt);
}
#endif