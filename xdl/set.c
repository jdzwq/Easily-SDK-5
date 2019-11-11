/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc set document

	@module	set.c | implement file

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

#include "set.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

set_t* set_alloc()
{
	set_t* pvs;

	pvs = (set_t*)xmem_alloc(sizeof(set_t));

	pvs->type = _SET_SET;
	pvs->size = 0;
	pvs->pset = NULL;

	return pvs;
}

void set_free(set_t* pv)
{
	set_empty(pv);

	xmem_free(pv);
}

void set_copy(set_t* pset, const set_t* psrc)
{
	int i;

	if (psrc->type == _SET_SET)
	{
		pset->type = psrc->type;
		pset->size = psrc->size;
		pset->pset = (set_t*)xmem_alloc(psrc->size * sizeof(set_t));
		
		for (i = 0; i < psrc->size; i++)
		{
			set_copy(&(pset->pset[i]), &(psrc->pset[i]));
		}
	}
	else
	{
		pset->type = psrc->type;
		pset->size = 1;
		xmem_copy((void*)&pset->data, (void*)&psrc->data, sizeof(double));
	}
}

void set_empty(set_t* pset)
{
	int i;

	if (pset->type == _SET_SET)
	{
		for (i = 0; i < pset->size; i++)
		{
			set_empty(&(pset->pset[i]));
		}
		xmem_free(pset->pset);
	}
	
	xmem_zero((void*)pset, sizeof(set_t));
	pset->type = _SET_SET;
}

int set_comp(const set_t* pv1, const set_t* pv2)
{
	int i;
	int rt;

	if (pv1->type < pv2->type)
		return -1;
	else if (pv1->type > pv2->type)
		return 1;

	if (pv1->type == _SET_SET)
	{
		if (pv1->size < pv2->size)
			return -1;
		else if (pv1->size > pv2->size)
			return 1;

		for (i = 0; i < pv1->size; i++)
		{
			rt = set_comp(&(pv1[i]), &(pv2[i]));
			if (rt)
				return rt;
		}
		return 0;
	}
	else
	{
		if (pv1->data == pv2->data)
			return 0;
		else if (pv1->data > pv2->data)
			return 1;
		else
			return -1;
	}
}

int set_find(set_t pa[], int min, int max, const set_t* p)
{
	int mid;
	int rt;

	while (min <= max)
	{
		mid = (min + max) / 2;

		if (pa[mid].type > p->type)
			return set_find(pa, min, mid - 1, p);
		else if (pa[mid].type < p->type)
			return set_find(pa, mid + 1, max, p);

		if (p->type == _SET_SET)
		{
			if (pa[mid].size > p->size)
				return set_find(pa, min, mid - 1, p);
			else if (pa[mid].size < p->size)
				return set_find(pa, mid + 1, max, p);
			
			rt = set_comp(&pa[mid], p);
			if (rt > 0)
				return set_find(pa, min, mid - 1, p);
			else if (rt < 0)
				return set_find(pa, mid + 1, max, p);
			else
				return mid;
		}
		else
		{
			if(pa[mid].data > p->data)
				return set_find(pa, min, mid - 1, p);
			else if (pa[mid].data < p->data)
				return set_find(pa, mid + 1, max, p);
			else
				return mid;
		}
	}

	return max;
}

void set_add(set_t* pset, const set_t* pv)
{
	int i, j;

	i = set_find(pset->pset, 0, pset->size - 1, pv);

	if (i >= 0 && set_comp(&(pset->pset[i]), pv) == 0)
		return;

	pset->pset = (set_t*)xmem_realloc(pset->pset, sizeof(set_t) * (pset->size + 1));
	for (j = pset->size; j > i + 1; j--)
	{
		pset->pset[j] = pset->pset[j - 1];
	}
	pset->size++;

	set_copy(&(pset->pset[i + 1]), pv);
}

void set_del(set_t* pset, const set_t* pv)
{
	int i, j;

	i = set_find(pset->pset, 0, pset->size - 1, pv);
	if (i < 0)
		return;

	if (set_comp(&(pset->pset[i]), pv) != 0)
		return;
	
	set_empty(&(pset->pset[i]));

	for (j = i; j < pset->size - 1; j++)
	{
		pset->pset[j] = pset->pset[j + 1];
	}

	pset->pset = (set_t*)xmem_realloc(pset->pset, sizeof(set_t) * (pset->size - 1));
	pset->size--;
}

bool_t set_in(set_t* pset, const set_t* pv)
{
	int i;

	i = set_find(pset->pset, 0, pset->size - 1, pv);

	return (i >= 0 && set_comp(&(pset->pset[i]), pv) == 0) ? 1 : 0;
}


void set_get(set_t* pset, int i, set_t* pv)
{
	if (i < 0 || i >= pset->size)
		return;

	set_copy(pv, &(pset->pset[i]));
}

static const tchar_t* _set_parse(set_t** ppv, dword_t* psize, const tchar_t* token, int len)
{
	int i, size, index, total = 0;
	set_t* pv = NULL;
	set_t* pa = NULL;
	bool_t b = 0;
	const tchar_t* tmp;

	if (len < 0)
		len = xslen(token);

	while (*token != _T('{') && *token != _T('\0') && total < len)
	{
		total++;
		token++;
	}
	if (*token == _T('{'))
	{
		total++;
		token++;
	}

	index = 0;
	i = 0;
	while (total <= len)
	{
		switch (*token)
		{
		case _T('{'):
			pa = NULL;
			size = 0;
			tmp = token;
			token = _set_parse(&pa, &size, token, len - total);
			total += (token - tmp);
			b = 1;
			break;
		case _T(' '):
		case _T('\t'):
		case _T('}'):
			index++;
			pv = (set_t*)xmem_realloc(pv, sizeof(set_t) * index);
			if (b)
			{
				pv[index - 1].type = _SET_SET;
				pv[index - 1].size = size;
				pv[index - 1].pset = pa;
			}
			else
			{
				pv[index - 1].type = _SET_ELE;
				pv[index - 1].size = 1;
				pv[index - 1].data = xsntonum(token - i, i);
			}
			b = 0;

			if (*token == _T('}'))
			{
				token++;
				goto RET;
			}

			total++;
			token++;
			i = 0;
			break;
		default:
			total++;
			token++;
			i++;
			break;
		}
	}

RET:

	*psize = index;
	*ppv = pv;

	return token;
}

static int _set_format(const set_t* ppv, dword_t size, tchar_t* buf, int max)
{
	dword_t i;
	int total = 0;

	if (size)
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('{');
		}
		total += 1;
	}

	for (i = 0; i < size; i++)
	{
		if (ppv[i].type == _SET_SET)
		{
			total += _set_format(ppv[i].pset, ppv[i].size, (buf) ? (buf + total) : NULL, max - total);
		}
		else
		{
			total += xsprintf((buf) ? (buf + total) : NULL, _T("%.f"), ppv[i].data);
		}

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T(' ');
		}
		total += 1;
	}

	if (size)
	{
		if (buf)
		{
			buf[total - 1] = _T('}');
		}
	}

	return total;
}

void set_parse(set_t* pset, const tchar_t* token, int len)
{
	int size = 0;

	set_empty(pset);

	_set_parse(&(pset->pset), &size, token, len);
	pset->size = size;
}

int set_format(const set_t* pset, tchar_t* buf, int max)
{
	if (pset->type == _SET_SET)
		return _set_format(pset->pset, pset->size, buf, max);
	else
		return _set_format(pset, 1, buf, max);
}

#if defined(_DEBUG) || defined(DEBUG)
void test_set()
{
	tchar_t num[NUM_LEN];

	set_t* pset = set_alloc();
	set_t ve;

	for (int i = 0; i < 10; i++)
	{
		xsprintf(num, _T("%d"), i);

		ve.type = _SET_ELE;
		ve.size = 1;
		ve.data = (double)i;

		set_add(pset, &ve);

		set_empty(&ve);
	}

	int len = set_format(pset, NULL, MAX_LONG);
	tchar_t* buf = xsalloc(len + 1);
	set_format(pset, buf, len);

	_tprintf(_T("%s\n"), buf);

	xsfree(buf);

	set_empty(pset);

	set_parse(pset, _T("{1 2 3 {1 2} {1 2 3} {1 2 {1 2 3 4}}}"), -1);

	len = set_format(pset, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	set_format(pset, buf, len);

	_tprintf(_T("%s\n"), buf);

	xsfree(buf);

	for (int i = 0; i < pset->size; i++)
	{
		set_get(pset, i, &ve);

		len = set_format(&ve, NULL, MAX_LONG);
		buf = xsalloc(len + 1);
		set_format(&ve, buf, len);

		_tprintf(_T("%s\n"), buf);

		xsfree(buf);

		set_empty(&ve);
	}

	set_free(pset);
}
#endif