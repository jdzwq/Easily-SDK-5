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
#include "varbytes.h"
#include "matrix.h"
#include "xdlimp.h"


typedef struct _vector_context{
	memobj_head head;

	int count;
	int dimen;
	void* data;
}vector_context;

vector_t vector_alloc(int count, int dimen)
{
	vector_context* pmv;
	int n;

	XDL_ASSERT(count >= 0 && dimen >= 0);

	pmv = (vector_context*)xmem_alloc(sizeof(vector_context));
	pmv->head.tag = MEM_VECTOR;
	PUT_THREEBYTE_LOC((pmv->head.len), 0, (sizeof(vector_context) - 4));

	pmv->count = count;
	pmv->dimen = dimen;
	n = pmv->count * pmv->dimen * sizeof(double);
	pmv->data = xmem_alloc(n);

	return (vector_t)&(pmv->head);
}

void vector_free(vector_t vec)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	
	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	if (pmv->data)
		xmem_free(pmv->data);

	xmem_free(pmv);
}

vector_t vector_clone(vector_t vec)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	vector_context* pnew;
	int n;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	pnew = (vector_context*)vector_alloc(pmv->count, pmv->dimen);

	n = pmv->count * pmv->dimen * sizeof(double);
	xmem_copy(pnew->data, pmv->data, n);

	return (vector_t)&(pnew->head);
}

void vector_reset(vector_t vec, int count, int dimen)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	int n;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	n = count * dimen * sizeof(double);
	pmv->data = xmem_realloc(pmv->data, n);
	xmem_zero(pmv->data, n);
	pmv->count = count;
	pmv->dimen = dimen;
}

void* vector_data(vector_t vec)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	return pmv->data;
}

void vector_attach(vector_t vec, void* data)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	if (pmv->data)
		xmem_free(pmv->data);

	pmv->data = data;
}

void* vector_detach(vector_t vec)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	void* d;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	d = pmv->data;
	pmv->data = NULL;

	return d;
}

void vector_zero(vector_t vec)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	int n;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	n = pmv->count * pmv->dimen * sizeof(double);
	xmem_zero(pmv->data, n);
}

void vector_unit(vector_t vec)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	double* pd;
	int i;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	pd = (double*)pmv->data;

	i = pmv->count * pmv->dimen;
	while (i--)
	{
		*pd++ = 1.0;
	}
}

void vector_copy(vector_t dst, vector_t src)
{
	vector_context* psrc = TypePtrFromHead(vector_context, src);
	vector_context* pdst = TypePtrFromHead(vector_context, dst);
	int n;

	XDL_ASSERT(psrc && psrc->head.tag == MEM_VECTOR && pdst && pdst->head.tag == MEM_VECTOR);

	n = psrc->count * psrc->dimen * sizeof(double);
	vector_reset(dst, psrc->count, psrc->dimen);
	xmem_copy(pdst->data, psrc->data, n);
}

int vector_get_count(vector_t vec)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	return pmv->count;
}

int vector_get_dimen(vector_t vec)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	return pmv->dimen;
}

void vector_set_value(vector_t vec, int i, ...)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	double* pd;
	int j;
	va_list arg;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	XDL_ASSERT(i >= 0 && i < pmv->count);

	va_start(arg, i);

	pd = (double*)pmv->data;

	for (j = 0; j < pmv->dimen; j++)
	{
		pd[i * pmv->dimen + j] = va_arg(arg, double);
	}

	va_end(arg);
}

void vector_get_value(vector_t vec, int i, ...)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	double *pd;
	int j;
	bool_t b = 0;
	double *pv;
	va_list arg;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	va_start(arg, i);

	if (i < 0 || i >= pmv->count)
		b = 1;

	pd = (double*)pmv->data;

	for (j = 0; j < pmv->dimen; j++)
	{
		pv = va_arg(arg, double*);
		if (b)
			*pv = MAXDBL;
		else
			*pv = pd[i * pmv->dimen + j];
	}

	va_end(arg);
}

//x'= x * ShiftX
//y'= y * ShiftY
vector_t vector_shift(vector_t vec, ...)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	double *pd;
	int i, j;
	bool_t b = 0;
	double *pb;
	vector_context* pnew;
	va_list arg;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	pb = (double*)xmem_alloc(pmv->dimen * sizeof(double));

	va_start(arg, vec);

	for (i = 0; i < pmv->dimen; i++)
	{
		pb[i] = va_arg(arg, double);
	}

	va_end(arg);

	pnew = (vector_context*)vector_clone(vec);

	pd = (double*)pnew->data;

	for (i = 0; i < pmv->count; i++)
	{
		for (j = 0; j < pmv->dimen; j++)
		{
			pd[i * pmv->dimen + j] += pb[j];
		}
	}

	xmem_free(pb);

	return (vector_t)&(pnew->head);
}

//x'= x * cosα+ y * sinα
//y'= x * sinα+ y * cosα
vector_t vector_rotate(vector_t vec, double ang)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	int i;
	matrix_t mat;
	vector_t pnew;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	mat = matrix_alloc(pmv->dimen, pmv->dimen);

	matrix_set_value(mat, 0, 0, cos(ang));
	matrix_set_value(mat, 0, 1, sin(ang));
	matrix_set_value(mat, 1, 0, -sin(ang));
	matrix_set_value(mat, 1, 1, cos(ang));

	for (i = 2; i < pmv->dimen; i++)
	{
		matrix_set_value(mat, i, i, 1.0);
	}

	pnew = (vector_t)matrix_mul((matrix_t)vec, mat);

	matrix_free(mat);

	return pnew;
}

//x'= x * ScallX
//y'= y * ScallY
vector_t vector_scale(vector_t vec, ...)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	int i;
	bool_t b = 0;
	double *pb;
	vector_t pnew;
	matrix_t mat;
	va_list arg;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	pb = (double*)xmem_alloc(pmv->dimen * sizeof(double));

	va_start(arg, vec);

	for (i = 0; i < pmv->dimen; i++)
	{
		pb[i] = va_arg(arg, double);
	}

	va_end(arg);

	mat = matrix_alloc(pmv->dimen, pmv->dimen);

	for (i = 0; i < pmv->dimen; i++)
	{
		matrix_set_value(mat, i, i, pb[i]);
	}

	xmem_free(pb);

	pnew = (vector_t)matrix_mul((matrix_t)vec, mat);

	matrix_free(mat);

	return pnew;
}

//x' = x + y * ShearX
//y' = y + x * ShearY
vector_t vector_shear(vector_t vec, double sx, double sy)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	int i;
	matrix_t mat;
	vector_t pnew;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	mat = matrix_alloc(pmv->dimen, pmv->dimen);

	matrix_set_value(mat, 0, 0, 1.0);
	matrix_set_value(mat, 0, 1, sy);
	matrix_set_value(mat, 1, 0, sx);
	matrix_set_value(mat, 1, 1, 1.0);

	for (i = 2; i < pmv->dimen; i++)
	{
		matrix_set_value(mat, i, i, 1.0);
	}

	pnew = (vector_t)matrix_mul((matrix_t)vec, mat);

	matrix_free(mat);

	return pnew;
}

vector_t vector_trans(vector_t vec, matrix_t mat)
{
	return (vector_t)matrix_mul((matrix_t)vec, mat);
}

void vector_parse(vector_t vec, const tchar_t* str, int len)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	double *pd;
	int i, j, n;
	const tchar_t* token;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	pd = (double*)pmv->data;

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

	for (i = 0; i < pmv->count && len; i++)
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

		for (j = 0; j < pmv->dimen; j++)
		{
			n = 0;
			while (*token != _T(',') && *token != _T(')') && *token != _T('}') && *token != _T('\0'))
			{
				n++;
				token++;
				len--;
			}

			pd[i * pmv->dimen + j] = xsntonum(token - n, n);

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

int vector_format(vector_t vec, tchar_t* buf, int max)
{
	vector_context* pmv = TypePtrFromHead(vector_context, vec);
	double *pd;
	int i, j, n;
	int total = 0;

	XDL_ASSERT(pmv && pmv->head.tag == MEM_VECTOR);

	pd = (double*)pmv->data;

	if (!pmv->count)
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

	for (i = 0; i < pmv->count; i++)
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('(');
		}
		total++;

		for (j = 0; j < pmv->dimen; j++)
		{
			n = numtoxs(pd[i * pmv->dimen + j], ((buf) ? (buf + total) : NULL), NUM_LEN);
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

dword_t vector_encode(vector_t vec, int encode, byte_t* buf, dword_t max)
{
	dword_t n = 0;
	tchar_t* str;
	int len;

	XDL_ASSERT(vec != NULL);

	len = vector_format(vec, NULL, MAX_LONG);
	str = xsalloc(len + 1);
	vector_format(vec, str, len);

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

void vector_decode(vector_t vec, int encode, const byte_t* buf, dword_t len)
{
	int n;
	tchar_t* str;

	if (!buf || !len)
	{
		if (vec)
		{
			vector_zero(vec);
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

	if (vec)
	{
		vector_parse(vec, str, n);
	}

	xsfree(str);
}

#if defined(XDL_SUPPORT_TEST)
void test_vector()
{
	tchar_t* buf;
	int len;

	vector_t pvt;

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

	vector_t pv = vector_shift(pvt, (double)1, (double)2, (double)3);
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

	pv = vector_rotate(pvt, XPI / 4);
	len = vector_format(pv, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pv, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pv);

	pv = vector_scale(pvt, 2.0, 0.5);
	len = vector_format(pv, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pv, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pv);

	pv = vector_shear(pvt, 1.0, 0.5);
	len = vector_format(pv, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pv, buf, len);
	_tprintf(_T("%s\n"), buf);
	xsfree(buf);
	vector_free(pv);

	vector_free(pvt);

}
#endif