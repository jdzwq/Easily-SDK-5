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
#include "xdloem.h"
#include "xdlstd.h"

vector_t* vector_alloc(sword_t size)
{
	vector_t* pvt;

	pvt = (vector_t*)xmem_alloc(sizeof(vector_t));

	pvt->size = size;

	pvt->data = (double*)xmem_alloc(size * sizeof(double));

	return pvt;
}

void vector_free(vector_t* pvt)
{
	xmem_free(pvt->data);
	xmem_free(pvt);
}

void vector_zero(vector_t* pvt)
{
	xmem_zero(pvt->data, pvt->size * sizeof(double));
}

void vector_set_value(vector_t* pvt, sword_t i, double db)
{
	(pvt->data)[i] = db;
}

double vector_get_value(vector_t* pvt, sword_t i)
{
	return (pvt->data)[i];
}

void vector_parse(vector_t* pvt, const tchar_t* str, int len)
{
	const tchar_t* token;
	sword_t j, n;

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

	for (j = 0; j < pvt->size; j++)
	{
		n = 0;
		while (*token != _T(' ') && *token != _T('\0'))
		{
			n++;
			token++;
		}

		(pvt->data)[j] = xsntonum(token - n, n);

		if (*token == _T(' '))
			token++;

		if (*token == _T(']') || *token == _T('\0'))
			break;
	}
}

int vector_format(vector_t* pvt, tchar_t* buf, int max)
{
	sword_t j, n;
	int total = 0;

	if (total + 1 > max)
		return total;

	if (buf)
	{
		xsncat(buf + total, _T("["), 1);
	}
	total++;

	for (j = 0; j < pvt->size; j++)
	{
		n = numtoxs((pvt->data)[j], ((buf) ? (buf + total) : NULL), NUM_LEN);
		if (total + n > max)
			return total;

		total += n;

		if (total + 1 > max)
			return total;

		if (buf)
		{
			xsncat(buf + total, _T(" "), 1);
		}

		total++;
	}

	if (pvt->size)
	{
		if (buf)
		{
			buf[total - 1] = _T(']');
		}
	}
	else
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			xsncat(buf + total, _T("]"), 1);
		}

		total++;
	}

	return total;
}

#if defined(_DEBUG) || defined(DEBUG)
void test_vector()
{
	tchar_t* buf;
	int len;

	vector_t* pvt = vector_alloc(10);

	vector_parse(pvt, _T(" [0 1 2 3 4 5 6 7 8 9] "), -1);

	len = vector_format(pvt, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	vector_format(pvt, buf, len);

	_tprintf(_T("%s\n"), buf);

	xsfree(buf);

	vector_free(pvt);

}
#endif