/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc matrix document

	@module	matrix.c | matrix implement file

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

#include "matrix.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"


matrix_t* matrix_alloc(sword_t rows, sword_t cols)
{
	matrix_t* pmt;

	pmt = (matrix_t*)xmem_alloc(sizeof(matrix_t));
	
	pmt->rows = rows;
	pmt->cols = cols;

	pmt->data = (double*)xmem_alloc(rows * cols * sizeof(double));

	return pmt;
}

void matrix_free(matrix_t* pmt)
{
	xmem_free(pmt->data);
	xmem_free(pmt);
}

void matrix_zero(matrix_t* pmt)
{
	xmem_zero(pmt->data, pmt->rows * pmt->cols * sizeof(double));
}

void matrix_set_value(matrix_t* pmt, sword_t i, sword_t j, double db)
{
	(pmt->data)[i * pmt->cols + j] = db;
}

double matrix_get_value(matrix_t* pmt, sword_t i, sword_t j)
{
	return (pmt->data)[i * pmt->cols + j];
}

void matrix_parse(matrix_t* pmt, const tchar_t* str, int len)
{		
	const tchar_t* token;
	int i,j,n;

	if (is_null(str) || !len)
		return;

	if (len < 0)
		len = xslen(str);

	token = str;

	while (*token != _T('[') && *token != _T('\0'))
		token++;

	if (*token == _T('\0'))
		return;

	if (*token == _T('['))
		token++;

	for (i = 0; i < pmt->rows; i++)
	{
		while (*token != _T('[') && *token != _T('\0'))
			token++;

		if (*token == _T('\0'))
			break;

		if (*token == _T('['))
			token++;

		for (j = 0; j < pmt->cols; j++)
		{
			n = 0;
			while (*token != _T(' ') && *token != _T(']') && *token != _T('\0'))
			{
				n++;
				token++;
			}

			(pmt->data)[i * pmt->cols + j] = xsntonum(token - n, n);

			if (*token == _T(' '))
				token++;

			if (*token == _T(']') || *token == _T('\0'))
				break;
		}

		if (*token == _T('\0'))
			break;

		if (*token == _T(']'))
			token++;
	}
}

int matrix_format(matrix_t* pmt, tchar_t* buf, int max)
{
	int i, j, n;
	int total = 0;

	if (total + 1 > max)
		return total;

	if (buf)
	{
		xsncat(buf + total, _T("["), 1);
	}
	total++;

	for (i = 0; i < pmt->rows; i++)
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			xsncat(buf + total, _T("["), 1);
		}
		total++;

		for (j = 0; j < pmt->cols; j++)
		{
			n = numtoxs((pmt->data)[i * pmt->cols + j], ((buf)? (buf + total) : NULL), NUM_LEN);
			if (total + n > max)
				return total;

			total += n;

			if (j < pmt->cols - 1)
			{
				if (total + 1 > max)
					return total;

				if (buf)
				{
					xsncat(buf + total, _T(" "), 1);
				}
				total++;
			}
		}

		if (total + 1 > max)
			return total;

		if (buf)
		{
			xsncat(buf + total, _T("]"), 1);
		}
		total++;
	}

	if (total + 1 > max)
		return total;

	if (buf)
	{
		xsncat(buf + total, _T("]"), 1);
	}
	total++;

	return total;
}

#if defined(_DEBUG) || defined(DEBUG)
void test_matrix(void)
{
	tchar_t* buf;
	int len;

	matrix_t* pvt = matrix_alloc(2, 10);

	matrix_parse(pvt, _T("[ [0 1 2 3 4 5 6 7 8 9],[9 8 7 6 5 4 3 2 1 0] ]"), -1);

	len = matrix_format(pvt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	matrix_format(pvt, buf, len);

	_tprintf(_T("%s\n"), buf);

	xsfree(buf);

	matrix_free(pvt);
}
#endif