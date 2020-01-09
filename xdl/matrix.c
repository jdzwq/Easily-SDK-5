/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc matrix document

	@module	matrix.c | implement file

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


matrix_t* matrix_alloc(int rows, int cols)
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

matrix_t* matrix_clone(const matrix_t* src)
{
	matrix_t* pmt;

	XDL_ASSERT(src != NULL);

	pmt = (matrix_t*)xmem_alloc(sizeof(matrix_t));

	pmt->rows = src->rows;
	pmt->cols = src->cols;
	pmt->data = (double*)xmem_alloc(pmt->rows * pmt->cols * sizeof(double));
	xmem_copy((void*)pmt->data, (void*)src->data, pmt->rows * pmt->cols * sizeof(double));

	return pmt;
}

void matrix_zero(matrix_t* pmt)
{
	xmem_zero(pmt->data, pmt->rows * pmt->cols * sizeof(double));
}

void matrix_unit(matrix_t* pmt)
{
	int i;
	double* p;

	i = pmt->rows * pmt->cols;
	p = pmt->data;

	while (i--)
	{
		*p++ = 1.0;
	}
}

void matrix_empty(matrix_t* pmt)
{
	xmem_free(pmt->data);

	pmt->data = NULL;
	pmt->rows = 0;
	pmt->cols = 0;
}

void matrix_copy(matrix_t* dest, const matrix_t* src)
{
	XDL_ASSERT(dest != NULL && src != NULL);

	dest->rows = src->rows;
	dest->cols = src->cols;
	dest->data = (double*)xmem_realloc(dest->data, dest->rows * dest->cols * sizeof(double));
	xmem_copy((void*)dest->data, (void*)src->data, dest->rows * dest->cols * sizeof(double));
}

void matrix_set_value(matrix_t* pmt, int i, int j, double db)
{
	XDL_ASSERT(i >= 0 && i < pmt->rows && j >= 0 && j < pmt->cols);

	(pmt->data)[i * pmt->cols + j] = db;
}

double matrix_get_value(matrix_t* pmt, int i, int j)
{
	XDL_ASSERT(i >= 0 && i < pmt->rows && j >= 0 && j < pmt->cols);

	return (pmt->data)[i * pmt->cols + j];
}

matrix_t* matrix_trans(matrix_t mt)
{
	matrix_t* pmt;
	int i, j;
	double dbl;

	XDL_ASSERT(mt.rows && mt.cols);

	pmt = matrix_alloc(mt.cols, mt.rows);

	for (i = 0; i < pmt->rows; i++)
	{
		for (j = 0; j < pmt->cols; j++)
		{
			dbl = matrix_get_value(&mt, j, i);
			matrix_set_value(pmt, i, j, dbl);
		}
	}

	return pmt;
}

matrix_t* matrix_plus(matrix_t mt, double n)
{
	matrix_t* pmt;
	int i, j;
	double dbl;

	XDL_ASSERT(mt.rows && mt.cols);

	pmt = matrix_alloc(mt.rows, mt.cols);

	for (i = 0; i < pmt->rows; i++)
	{
		for (j = 0; j < pmt->cols; j++)
		{
			dbl = matrix_get_value(&mt, i, j) * n;
			matrix_set_value(pmt, i, j, dbl);
		}
	}

	return pmt;
}

matrix_t* matrix_add(matrix_t mt1, matrix_t mt2)
{
	matrix_t* pmt;
	int i, j;
	double dbl;

	XDL_ASSERT(mt1.rows == mt2.rows && mt1.cols == mt2.cols);

	pmt = matrix_alloc(mt1.rows, mt2.cols);

	for (i = 0; i < pmt->rows; i++)
	{
		for (j = 0; j < pmt->cols; j++)
		{
			dbl = matrix_get_value(&mt1, i, j) + matrix_get_value(&mt2, i, j);
			matrix_set_value(pmt, i, j, dbl);
		}
	}

	return pmt;
}

matrix_t* matrix_mul(matrix_t mt1, matrix_t mt2)
{
	matrix_t* pmt;
	int i, j, k;
	double dbl;

	XDL_ASSERT(mt1.cols == mt2.rows);

	pmt = matrix_alloc(mt1.rows, mt2.cols);

	for (i = 0; i < pmt->rows; i++)
	{
		for (j = 0; j < pmt->cols; j++)
		{
			dbl = 0.0;
			for (k = 0; k < mt1.cols; k++)
			{
				dbl += matrix_get_value(&mt1, i, k) * matrix_get_value(&mt2, k, j);
			}
			matrix_set_value(pmt, i, j, dbl);
		}
	}

	return pmt;
}

double matrix_det(matrix_t mt)
{
	int i, j, k;
	double dbl, n1, n2;

	n1 = 0.0;
	for (k = 0; k < mt.cols; k++)
	{
		dbl = 1.0;
		for (i = 0, j = k; i < mt.rows; i++, j++)
		{
			dbl *= matrix_get_value(&mt, i, j);

			j %= mt.cols;
		}
		n1 += dbl;
	}

	n2 = 0.0;
	for (k = mt.cols - 1; k >= 0; k--)
	{
		dbl = 1.0;
		for (i = 0, j = k; i < mt.rows; i++, j--)
		{
			dbl *= matrix_get_value(&mt, i, j);

			j = (j + mt.cols) % mt.cols;
		}
		n2 += dbl;
	}

	return (n1 - n2);
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
		buf[total] = _T('[');
	}
	total++;

	for (i = 0; i < pmt->rows; i++)
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('[');
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
					buf[total] = _T(' ');
				}
				total++;
			}
		}

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T(']');
		}
		total++;

		if (i < pmt->rows - 1)
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