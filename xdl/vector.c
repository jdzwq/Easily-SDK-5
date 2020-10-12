/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc vector document

	@module	vector.c | implement file

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

#include "vector.h"
#include "xdlimp.h"

#include "xdlstd.h"

vector_t* vector_alloc(int size, int order)
{
	vector_t* pvt;

	XDL_ASSERT(order > 0);

	pvt = (vector_t*)xmem_alloc(sizeof(vector_t));

	pvt->size = size;
	pvt->order = order;
	pvt->data = (double*)xmem_alloc(size * order * sizeof(double));

	return pvt;
}

void vector_free(vector_t* pvt)
{
	if (pvt->data)
		xmem_free(pvt->data);
	xmem_free(pvt);
}

vector_t* vector_clone(const vector_t* src)
{
	vector_t* pvt;

	XDL_ASSERT(src != NULL);

	pvt = (vector_t*)xmem_alloc(sizeof(vector_t));

	pvt->size = src->size;
	pvt->order = src->order;
	pvt->data = (double*)xmem_alloc(pvt->size * pvt->order * sizeof(double));

	xmem_copy((void*)pvt->data, (void*)src->data, pvt->size * pvt->order * sizeof(double));

	return pvt;
}

void vector_empty(vector_t* pvt)
{
	xmem_free(pvt->data);
	
	pvt->data = NULL;
	pvt->size = 0;
}

void vector_zero(vector_t* pvt)
{
	xmem_zero(pvt->data, pvt->size * pvt->order * sizeof(double));
}

void vector_unit(vector_t* pvt)
{
	int i;
	double* p;

	i = pvt->size * pvt->order;
	p = pvt->data;

	while (i--)
	{
		*p++ = 1.0;
	}
}

void vector_copy(vector_t* dest, const vector_t* src)
{
	XDL_ASSERT(dest != NULL && src != NULL);

	dest->size = src->size;
	dest->order = src->order;
	dest->data = (double*)xmem_realloc(dest->data, dest->size * dest->order * sizeof(double));
	xmem_copy((void*)dest->data, (void*)src->data, dest->size * dest->order * sizeof(double));
}

void vector_set_value(vector_t* pvt, int i, ...)
{
	int j;
	va_list arg;

	XDL_ASSERT(i >= 0 && i < pvt->size);

	va_start(arg, i);

	for (j = 0; j < pvt->order; j++)
	{
		(pvt->data)[i * pvt->order + j] = va_arg(arg, double);
	}

	va_end(arg);
}

void vector_get_value(vector_t* pvt, int i, ...)
{
	double* pd;
	int j;
	va_list arg;

	XDL_ASSERT(i >= 0 && i < pvt->size);

	va_start(arg, i);

	for (j = 0; j < pvt->order; j++)
	{
		pd = va_arg(arg, double*);
		*pd = (pvt->data)[i * pvt->order + j];
	}

	va_end(arg);
}

//x'= x * ShiftX
//y'= y * ShiftY
vector_t* vector_shift(vector_t vt, ...)
{
	vector_t* pvt;
	double* pb;
	int i,j;
	va_list arg;

	if (!vt.order)
		return NULL;

	pb = (double*)xmem_alloc(vt.order * sizeof(double));

	va_start(arg, vt);

	for (i = 0; i < vt.order; i++)
	{
		pb[i] = va_arg(arg, double);
	}

	va_end(arg);

	pvt = vector_clone(&vt);

	for (i = 0; i < pvt->size; i++)
	{
		for (j = 0; j < pvt->order; j++)
		{
			(pvt->data)[i * pvt->order + j] += pb[j];
		}
	}

	xmem_free(pb);

	return pvt;
}

//x'= x * cosα+ y * sinα
//y'= x * sinα+ y * cosα
vector_t* vector_rotate(vector_t vt, double ang)
{
	vector_t* pvt;
	matrix_t* pm;
	int i;

	pm = matrix_alloc(vt.order, vt.order);

	matrix_set_value(pm, 0, 0, cos(ang));
	matrix_set_value(pm, 0, 1, sin(ang));
	matrix_set_value(pm, 1, 0, -sin(ang));
	matrix_set_value(pm, 1, 1, cos(ang));

	for (i = 2; i < vt.order; i++)
	{
		matrix_set_value(pm, i, i, 1.0);
	}

	pvt = (vector_t*)matrix_mul(*((matrix_t*)&vt), *pm);
	matrix_free(pm);

	return pvt;
}

//x'= x * ScallX
//y'= y * ScallY
vector_t* vector_scale(vector_t vt, ...)
{
	vector_t* pvt;
	matrix_t* pm;
	double* pb;
	int i;
	va_list arg;

	pb = (double*)xmem_alloc(vt.order * sizeof(double));

	va_start(arg, vt);

	for (i = 0; i < vt.order; i++)
	{
		pb[i] = va_arg(arg, double);
	}

	va_end(arg);

	pm = matrix_alloc(vt.order, vt.order);

	for (i = 0; i < vt.order; i++)
	{
		matrix_set_value(pm, i, i, pb[i]);
	}

	xmem_free(pb);

	pvt = (vector_t*)matrix_mul(*((matrix_t*)&vt), *pm);
	matrix_free(pm);

	return pvt;
}

//x' = x + y * ShearX
//y' = y + x * ShearY
vector_t* vector_shear(vector_t vt, double sx, double sy)
{
	vector_t* pvt;
	matrix_t* pm;
	int i;

	pm = matrix_alloc(vt.order, vt.order);

	matrix_set_value(pm, 0, 0, 1.0);
	matrix_set_value(pm, 0, 1, sy);
	matrix_set_value(pm, 1, 0, sx);
	matrix_set_value(pm, 1, 1, 1.0);

	for (i = 2; i < vt.order; i++)
	{
		matrix_set_value(pm, i, i, 1.0);
	}

	pvt = (vector_t*)matrix_mul(*((matrix_t*)&vt), *pm);
	matrix_free(pm);

	return pvt;
}

vector_t* vector_trans(vector_t vt, matrix_t mt)
{
	return (vector_t*)matrix_mul(*((matrix_t*)&vt), mt);
}

void vector_parse(vector_t* pvt, const tchar_t* str, int len)
{
	const tchar_t* token;
	int i, j, n;

	if (is_null(str) || !len)
		return;

	if (len < 0)
		len = xslen(str);

	token = str;
	
	while (*token != _T('{') && *token != _T('\0'))
	{
		token++;
		len--;
	}

	if (*token == _T('\0'))
		return;

	token++; //skip '{'
	len--;

	for (i = 0; i < pvt->size && len; i++)
	{
		while (*token != _T('(') && *token != _T('}') && *token != _T('\0'))
		{
			token++;
			len--;
		}

		if (*token == _T('}') || *token == _T('\0'))
			break;

		token++; //skip '('
		len--;

		for (j = 0; j < pvt->order; j++)
		{
			n = 0;
			while (*token != _T(',') && *token != _T(')') && *token != _T('}') && *token != _T('\0'))
			{
				n++;
				token++;
				len--;
			}

			(pvt->data)[i * pvt->order + j] = xsntonum(token - n, n);

			if (*token == _T(')') || *token == _T('}') || *token == _T('\0'))
				break;

			token++; //skip ','
			len--;
		}

		if (*token == _T('}') || *token == _T('\0'))
			break;

		token++; //skip ')'
		len--;
	}
}

int vector_format(vector_t* pvt, tchar_t* buf, int max)
{
	int i, j, n;
	int total = 0;

	if (!pvt->size)
	{
		if (buf)
		{
			buf[0] = _T('\0');
		}
		return 0;
	}

	if (total + 1 > max)
		return total;

	if (buf)
	{
		buf[total] = _T('{');
	}
	total++;

	for (i = 0; i < pvt->size; i++)
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('(');
		}
		total++;

		for (j = 0; j < pvt->order; j++)
		{
			n = numtoxs((pvt->data)[i * pvt->order + j], ((buf) ? (buf + total) : NULL), NUM_LEN);
			if (total + n > max)
				return total;
			total += n;

			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = _T(',');
			}
			total++;
		}

		//reppace the last ','
		if (buf)
		{
			buf[total-1] = _T(')');
		}

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T(',');
		}
		total++;
	}
	
	//reppace the last ','
	if (buf)
	{
		buf[total-1] = _T('}');
		buf[total] = _T('\0');
	}

	return total;
}

#if defined(_DEBUG) || defined(DEBUG)
void test_vector()
{
	tchar_t* buf;
	int len;

	vector_t* pvt;

	pvt = vector_alloc(10, 1);
	vector_parse(pvt, _T("{(0),(1), (2),(3), (4) ,(5)(6), (7) ,(8),(9)}"), -1);
	len = vector_format(pvt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pvt, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pvt);

	pvt = vector_alloc(5,2);
	vector_parse(pvt, _T(" {(0,1) ,(2,3),(4, 5) ,(6, 7) ,(8,9)}"), -1);
	len = vector_format(pvt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pvt, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pvt);

	pvt = vector_alloc(4,3);
	vector_parse(pvt, _T(" {(0,1, 2), (3,4,5),(6,7), (8))}"), -1);
	len = vector_format(pvt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pvt, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);

	vector_t* pv = vector_shift(*pvt, (double)1, (double)2, (double)3);
	len = vector_format(pv, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pv, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pv);

	vector_free(pvt);

	pvt = vector_alloc(4,2);
	vector_parse(pvt, _T(" {(1,1) ,(-1,1),(-1, -1) ,(1, -1)}"), -1);
	len = vector_format(pvt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pvt, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);

	pv = vector_rotate(*pvt, XPI / 4);
	len = vector_format(pv, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pv, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pv);

	pv = vector_scale(*pvt, 2.0, 0.5);
	len = vector_format(pv, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pv, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pv);

	pv = vector_shear(*pvt, 1.0, 0.5);
	len = vector_format(pv, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pv, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pv);

	vector_free(pvt);

}
#endif