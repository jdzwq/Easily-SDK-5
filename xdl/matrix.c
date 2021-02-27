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
#include "varbytes.h"
#include "xdlimp.h"


typedef struct _matrix_context{
	memobj_head head;

	int rows;
	int cols;
	void* data;
}matrix_context;


#define MATRIX_CALC_SIZE(rows, cols)	(rows * cols * sizeof(double))

matrix_t matrix_alloc(int rows, int cols)
{
	matrix_context* pmt;

	XDL_ASSERT(rows >= 0 && cols >= 0);

	pmt = (matrix_context*)xmem_alloc(sizeof(matrix_context));
	pmt->head.tag = MEM_MATRIX;
	PUT_THREEBYTE_LOC((pmt->head.len), 0, (sizeof(matrix_context) - 4));

	pmt->rows = rows;
	pmt->cols = cols;
	pmt->data = NULL;

	return (matrix_t)&(pmt->head);
}

void matrix_free(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);
	
	if (pmt->data)
		xmem_free(pmt->data);

	xmem_free(pmt);
}

matrix_t matrix_clone(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	int n;
	matrix_context* pnew;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	pnew = (matrix_context*)matrix_alloc(pmt->rows, pmt->cols);
	
	if (pmt->data)
	{
		n = MATRIX_CALC_SIZE(pmt->rows, pmt->cols);
		pnew->data = xmem_realloc(pnew->data, n);
		xmem_copy((void*)pnew->data, (void*)pmt->data, n);
	}

	return (matrix_t)&(pnew->head);
}

void matrix_reset(matrix_t mat, int rows, int cols)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	
	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (pmt->data)
		xmem_free(pmt->data);

	pmt->data = NULL;
	pmt->rows = rows;
	pmt->cols = cols;
}

void matrix_copy(matrix_t dst, matrix_t src)
{
	matrix_context* psrc = (matrix_context*)src;
	matrix_context* pdst = (matrix_context*)dst;
	int n;

	XDL_ASSERT(psrc && psrc->head.tag == MEM_MATRIX && pdst && pdst->head.tag == MEM_MATRIX);

	matrix_reset(dst, psrc->rows, psrc->cols);

	if (psrc->data)
	{
		n = MATRIX_CALC_SIZE(psrc->rows, psrc->cols);
		pdst->data = xmem_realloc(pdst->data, n);
		xmem_copy(pdst->data, psrc->data, n);
	}
}

int matrix_get_rows(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	return pmt->rows;
}

int matrix_get_cols(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	return pmt->cols;
}

const void* matrix_data(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	return pmt->data;
}

void matrix_attach(matrix_t mat, void* data)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (pmt->data)
		xmem_free(pmt->data);

	pmt->data = data;
}

void* matrix_detach(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	void* d;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	d = pmt->data;
	pmt->data = NULL;

	return d;
}

void matrix_zero(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	int n;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	n = MATRIX_CALC_SIZE(pmt->rows, pmt->cols);

	if (!pmt->data)
	{
		pmt->data = xmem_alloc(n);
	}

	xmem_zero((void*)pmt->data, n);
}

void matrix_set_value(matrix_t mat, int i, int j, double db)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	double* pd;
	int n;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (!pmt->data)
	{
		n = MATRIX_CALC_SIZE(pmt->rows, pmt->cols);
		pmt->data = xmem_alloc(n);
	}

	pd = (double*)pmt->data;

	XDL_ASSERT(i >= 0 && i < pmt->rows && j >= 0 && j < pmt->cols);

	pd[i * pmt->cols + j] = db;
}

double matrix_get_value(matrix_t mat, int i, int j)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	double* pd;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (!pmt->data)
	{
		return MAXDBL;
	}

	pd = (double*)pmt->data;

	if (i < 0 || i >= pmt->rows || j < 0 || j >= pmt->cols)
		return MAXDBL;

	return pd[i * pmt->cols + j];
}

void matrix_unit(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	double* pd;
	int n, i;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (!pmt->data)
	{
		n = MATRIX_CALC_SIZE(pmt->rows, pmt->cols);
		pmt->data = xmem_alloc(n);
	}

	pd = (double*)pmt->data;

	XDL_ASSERT(pd != NULL);

	i = pmt->rows * pmt->cols;
	while (i--)
	{
		*pd++ = 1.0;
	}
}

matrix_t matrix_trans(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	double dbl;
	int i, j;
	matrix_t pnew;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (!pmt->data)
	{
		return NULL;
	}

	pnew = matrix_alloc(pmt->cols, pmt->rows);

	for (i = 0; i < pmt->cols; i++)
	{
		for (j = 0; j < pmt->rows; j++)
		{
			dbl = matrix_get_value(mat, j, i);
			matrix_set_value(pnew, i, j, dbl);
		}
	}

	return pnew;
}

matrix_t matrix_plus(matrix_t mat, double n)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	double dbl;
	int i, j;
	matrix_t pnew;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (!pmt->data)
	{
		return NULL;
	}

	pnew = matrix_alloc(pmt->rows, pmt->cols);

	for (i = 0; i < pmt->rows; i++)
	{
		for (j = 0; j < pmt->cols; j++)
		{
			dbl = matrix_get_value(mat, i, j) * n;
			matrix_set_value(pnew, i, j, dbl);
		}
	}

	return pnew;
}

matrix_t matrix_add(matrix_t mat1, matrix_t mat2)
{
	matrix_context* pmt1 = TypePtrFromHead(matrix_context, mat1);
	matrix_context* pmt2 = TypePtrFromHead(matrix_context, mat2);
	double dbl;
	int i, j;
	matrix_t pnew;

	XDL_ASSERT(pmt1 && pmt1->head.tag == MEM_MATRIX && pmt2 && pmt2->head.tag == MEM_MATRIX);

	XDL_ASSERT(pmt1->rows == pmt2->rows && pmt1->cols == pmt2->cols);

	if (!pmt1->data || !pmt2->data)
	{
		return NULL;
	}

	pnew = matrix_alloc(pmt1->rows, pmt2->cols);

	for (i = 0; i < pmt1->rows; i++)
	{
		for (j = 0; j < pmt2->cols; j++)
		{
			dbl = matrix_get_value(mat1, i, j) + matrix_get_value(mat2, i, j);
			matrix_set_value(pnew, i, j, dbl);
		}
	}

	return pnew;
}

matrix_t matrix_mul(matrix_t mat1, matrix_t mat2)
{
	matrix_context* pmt1 = TypePtrFromHead(matrix_context, mat1);
	matrix_context* pmt2 = TypePtrFromHead(matrix_context, mat2);
	double dbl;
	int i, j, k;
	matrix_t pnew;

	XDL_ASSERT(pmt1 && pmt1->head.tag == MEM_MATRIX && pmt2 && pmt2->head.tag == MEM_MATRIX);

	XDL_ASSERT(pmt1->cols == pmt2->rows);

	if (!pmt1->data || !pmt2->data)
	{
		return NULL;
	}

	pnew = matrix_alloc(pmt1->rows, pmt2->cols);

	for (i = 0; i < pmt1->rows; i++)
	{
		for (j = 0; j < pmt2->cols; j++)
		{
			dbl = 0.0;
			for (k = 0; k < pmt1->cols; k++)
			{
				dbl += matrix_get_value(mat1, i, k) * matrix_get_value(mat2, k, j);
			}
			matrix_set_value(pnew, i, j, dbl);
		}
	}

	return pnew;
}

double matrix_det(matrix_t mat)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	double dbl, n1, n2;
	int i, j, k;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (!pmt->data)
	{
		return MAXDBL;
	}

	n1 = 0.0;
	for (k = 0; k < pmt->cols; k++)
	{
		dbl = 1.0;
		for (i = 0, j = k; i < pmt->rows; i++, j++)
		{
			dbl *= matrix_get_value(mat, i, j);

			j %= pmt->cols;
		}
		n1 += dbl;
	}

	n2 = 0.0;
	for (k = pmt->cols - 1; k >= 0; k--)
	{
		dbl = 1.0;
		for (i = 0, j = k; i < pmt->rows; i++, j--)
		{
			dbl *= matrix_get_value(mat, i, j);

			j = (j + pmt->cols) % pmt->cols;
		}
		n2 += dbl;
	}

	return (n1 - n2);
}

void matrix_parse(matrix_t mat, const tchar_t* str, int len)
{	
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	double* pd;
	const tchar_t* token;
	int i, j, n;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (len < 0)
		len = xslen(str);

	if (!len)
		return;

	if (!pmt->data)
	{
		n = MATRIX_CALC_SIZE(pmt->rows, pmt->cols);
		pmt->data = xmem_alloc(n);
	}

	pd = (double*)pmt->data;

	token = str;

	while (*token != _T('{') && *token != _T('\0'))
		token++;

	if (*token == _T('\0'))
		return;

	if (*token == _T('{'))
		token++;

	for (i = 0; i < pmt->rows; i++)
	{
		while (*token != _T('[') && *token != _T('}') && *token != _T('\0'))
			token++;

		if (*token == _T('}') || *token == _T('\0'))
			break;

		//skip '['
		token++;

		for (j = 0; j < pmt->cols; j++)
		{
			n = 0;
			while (*token != _T(',') && *token != _T(']') && *token != _T('}') && *token != _T('\0'))
			{
				n++;
				token++;
			}

			pd[i * pmt->cols + j] = xsntonum(token - n, n);

			if (*token == _T(','))
				token++;

			if (*token == _T(']') || *token == _T('}') || *token == _T('\0'))
				break;
		}

		if (*token == _T('}') || *token == _T('\0'))
			break;

		//skip ']'
		token++;
	}
}

int matrix_format(matrix_t mat, tchar_t* buf, int max)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	double* pd;
	int i, j, n;
	int total = 0;

	XDL_ASSERT(pmt && pmt->head.tag == MEM_MATRIX);

	if (!pmt->data)
		return 0;

	pd = (double*)pmt->data;

	if (total + 1 > max)
		return total;

	if (buf)
	{
		buf[total] = _T('{');
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
			n = numtoxs(pd[i * pmt->cols + j], ((buf)? (buf + total) : NULL), NUM_LEN);
			if (total + n > max)
				return total;

			total += n;

			if (j < pmt->cols - 1)
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
		xsncat(buf + total, _T("}"), 1);
	}
	total++;

	return total;
}

/*
struct{
	byte[2]: rows
	byte[2]: cols
	byte[]: data
}matrix_dump
*/
dword_t matrix_encode(matrix_t mat, byte_t* buf, dword_t max)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	dword_t n = 0;
	
	XDL_ASSERT(mat != NULL && mat->tag == MEM_MATRIX);

	if (buf)
	{
		PUT_SWORD_LOC(buf, 0, pmt->rows);
		PUT_SWORD_LOC(buf, 2, pmt->cols);
	}

	n = MATRIX_CALC_SIZE(pmt->rows, pmt->cols);
	n = (n < max) ? n : max;
	if (buf && pmt->data)
	{
		xmem_copy((void*)(buf + 4), (void*)pmt->data, n);
	}

	return (n + 4);
}

dword_t matrix_decode(matrix_t mat, const byte_t* buf)
{
	matrix_context* pmt = TypePtrFromHead(matrix_context, mat);
	dword_t n = 0;
	int rows, cols;

	if (!buf)
	{
		return 0;
	}

	rows = GET_SWORD_LOC(buf, 0);
	cols = GET_SWORD_LOC(buf, 2);

	n = rows * cols * sizeof(double);
	if (mat)
	{
		matrix_reset(mat, rows, cols);
		pmt->data = xmem_realloc(pmt->data, n);
		xmem_copy((void*)pmt->data, (void*)(buf + 4), n);
	}

	return (n + 4);
}

#if defined(XDL_SUPPORT_TEST)
void test_matrix(void)
{
	tchar_t* buf;
	int len;

	matrix_t pvt = matrix_alloc(2, 10);

	matrix_parse(pvt, _T("{ [0, 1, 2,3, 4, 5, 6, 7, 8,9 ],[9,8,7,6,5,4,3,2,1,0] }"), -1);

	len = matrix_format(pvt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	matrix_format(pvt, buf, len);

	_tprintf(_T("%s\n"), buf);

	xsfree(buf);

	matrix_free(pvt);
}
#endif