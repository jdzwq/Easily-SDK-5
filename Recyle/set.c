/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc set document

	@module	set.c | xdl prime implement file

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
#include "xdlstd.h"

typedef enum{
	_SET_SET = 0,
	_SET_NUM = 1
}SET_TYPE;

typedef struct _set_ele_t* set_ele_t_ptr;

typedef struct _set_ele_t{
	int type;
	int size;
	union{
		double data;
		set_ele_t_ptr pset;
	};
}set_ele_t;

typedef struct _set_t{
	object_head		head;
	set_ele_t_ptr	pset;
}set_t;

static set_ele_t* _set_ele_alloc()
{
	set_ele_t* pvs;

	pvs = (set_ele_t*)xmem_alloc(sizeof(set_ele_t));

	pvs->type = _SET_SET;
	pvs->size = 0;
	pvs->pset = NULL;

	return pvs;
}

static void _set_ele_empty(set_ele_t* pset)
{
	int i;

	if (pset->type == _SET_SET)
	{
		for (i = 0; i < pset->size; i++)
		{
			_set_ele_empty(&(pset->pset[i]));
		}
		xmem_free(pset->pset);
	}

	xmem_zero((void*)pset, sizeof(set_ele_t));
	pset->type = _SET_SET;
}

static void _set_ele_free(set_ele_t* pv)
{
	_set_ele_empty(pv);

	xmem_free(pv);
}

static void _set_ele_copy(set_ele_t* pset, const set_ele_t* psrc)
{
	int i;

	if (psrc->type == _SET_SET)
	{
		pset->type = psrc->type;
		pset->size = psrc->size;
		pset->pset = (set_ele_t*)xmem_alloc(psrc->size * sizeof(set_ele_t));
		
		for (i = 0; i < psrc->size; i++)
		{
			_set_ele_copy(&(pset->pset[i]), &(psrc->pset[i]));
		}
	}
	else
	{
		pset->type = psrc->type;
		pset->size = 1;
		xmem_copy((void*)&pset->data, (void*)&psrc->data, sizeof(double));
	}
}

static int _set_ele_comp(const set_ele_t* pv1, const set_ele_t* pv2)
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
			rt = _set_ele_comp(&(pv1[i]), &(pv2[i]));
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

static int _set_ele_find(set_ele_t pa[], int min, int max, const set_ele_t* p)
{
	int mid;
	int rt;

	while (min <= max)
	{
		mid = (min + max) / 2;

		if (pa[mid].type > p->type)
			return _set_ele_find(pa, min, mid - 1, p);
		else if (pa[mid].type < p->type)
			return _set_ele_find(pa, mid + 1, max, p);

		if (p->type == _SET_SET)
		{
			if (pa[mid].size > p->size)
				return _set_ele_find(pa, min, mid - 1, p);
			else if (pa[mid].size < p->size)
				return _set_ele_find(pa, mid + 1, max, p);
			
			rt = _set_ele_comp(&pa[mid], p);
			if (rt > 0)
				return _set_ele_find(pa, min, mid - 1, p);
			else if (rt < 0)
				return _set_ele_find(pa, mid + 1, max, p);
			else
				return mid;
		}
		else
		{
			if(pa[mid].data > p->data)
				return _set_ele_find(pa, min, mid - 1, p);
			else if (pa[mid].data < p->data)
				return _set_ele_find(pa, mid + 1, max, p);
			else
				return mid;
		}
	}

	return max;
}

static void _set_ele_add(set_ele_t* pset, const set_ele_t* pv)
{
	int i, j;

	i = _set_ele_find(pset->pset, 0, pset->size - 1, pv);

	if (i >= 0 && _set_ele_comp(&(pset->pset[i]), pv) == 0)
		return;

	pset->pset = (set_ele_t*)xmem_realloc(pset->pset, sizeof(set_ele_t) * (pset->size + 1));
	for (j = pset->size; j > i + 1; j--)
	{
		pset->pset[j] = pset->pset[j - 1];
	}
	pset->size++;

	_set_ele_copy(&(pset->pset[i + 1]), pv);
}

static void _set_ele_del(set_ele_t* pset, const set_ele_t* pv)
{
	int i, j;

	i = _set_ele_find(pset->pset, 0, pset->size - 1, pv);
	if (i < 0)
		return;

	if (_set_ele_comp(&(pset->pset[i]), pv) != 0)
		return;
	
	_set_ele_empty(&(pset->pset[i]));

	for (j = i; j < pset->size - 1; j++)
	{
		pset->pset[j] = pset->pset[j + 1];
	}

	pset->pset = (set_ele_t*)xmem_realloc(pset->pset, sizeof(set_ele_t) * (pset->size - 1));
	pset->size--;
}

static bool_t _set_ele_in(set_ele_t* pset, const set_ele_t* pv)
{
	int i;

	i = _set_ele_find(pset->pset, 0, pset->size - 1, pv);

	return (i >= 0 && _set_ele_comp(&(pset->pset[i]), pv) == 0) ? 1 : 0;
}


static void _set_ele_get(set_ele_t* pset, int i, set_ele_t* pv)
{
	if (i < 0 || i >= pset->size)
		return;

	_set_ele_copy(pv, &(pset->pset[i]));
}

static const tchar_t* _set_ele_parse(set_ele_t** ppv, dword_t* psize, const tchar_t* token, int len)
{
	int i, size, index, total = 0;
	set_ele_t* pv = NULL;
	set_ele_t* pa = NULL;
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
			token = _set_ele_parse(&pa, &size, token, len - total);
			total += (token - tmp);
			b = 1;
			break;
		case _T(','):
		case _T('}'):
			index++;
			pv = (set_ele_t*)xmem_realloc(pv, sizeof(set_ele_t) * index);
			if (b)
			{
				pv[index - 1].type = _SET_SET;
				pv[index - 1].size = size;
				pv[index - 1].pset = pa;
			}
			else
			{
				pv[index - 1].type = _SET_NUM;
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

static int _set_ele_format(const set_ele_t* ppv, dword_t size, tchar_t* buf, int max)
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
			total += _set_ele_format(ppv[i].pset, ppv[i].size, (buf) ? (buf + total) : NULL, max - total);
		}
		else
		{
			total += xsprintf((buf) ? (buf + total) : NULL, _T("%.f"), ppv[i].data);
		}

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T(',');
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

/***************************************************************************************/
object_t set_alloc(void)
{
	set_t* pt;

	pt = (set_t*)xmem_alloc(sizeof(set_t));
	pt->head.obj = _OBJECT_SET;

	pt->pset = _set_ele_alloc();

	return &pt->head;
}

void set_free(object_t set)
{
	set_t* pt = (set_t*)set;

	XDL_ASSERT(set && set->obj == _OBJECT_SET);

	_set_ele_free(pt->pset);
	xmem_free(pt);
}

void set_copy(object_t dst, object_t src)
{
	set_t* pdst = (set_t*)dst;
	set_t* psrc = (set_t*)src;

	XDL_ASSERT(dst && dst->obj == _OBJECT_SET);
	XDL_ASSERT(src && src->obj == _OBJECT_SET);

	_set_ele_copy(pdst->pset, psrc->pset);
}

int set_comp(object_t s1, object_t s2)
{
	set_t* ps1 = (set_t*)s1;
	set_t* ps2 = (set_t*)s2;

	XDL_ASSERT(s1 && s1->obj == _OBJECT_SET);
	XDL_ASSERT(s2 && s2->obj == _OBJECT_SET);

	return _set_ele_comp(ps1->pset, ps2->pset);
}

void set_empty(object_t set)
{
	set_t* pt = (set_t*)set;

	XDL_ASSERT(set && set->obj == _OBJECT_SET);

	_set_ele_empty(pt->pset);
}

void set_add(object_t set, object_t sub)
{
	set_t* pset = (set_t*)set;
	set_t* psub = (set_t*)sub;

	XDL_ASSERT(set && set->obj == _OBJECT_SET);
	XDL_ASSERT(sub && sub->obj == _OBJECT_SET);

	_set_ele_add(pset->pset, psub->pset);
}

void set_del(object_t set, object_t sub)
{
	set_t* pset = (set_t*)set;
	set_t* psub = (set_t*)sub;

	XDL_ASSERT(set && set->obj == _OBJECT_SET);
	XDL_ASSERT(sub && sub->obj == _OBJECT_SET);

	_set_ele_del(pset->pset, psub->pset);
}

bool_t set_in(object_t set, object_t sub)
{
	set_t* pset = (set_t*)set;
	set_t* psub = (set_t*)sub;

	XDL_ASSERT(set && set->obj == _OBJECT_SET);
	XDL_ASSERT(sub && sub->obj == _OBJECT_SET);

	return _set_ele_in(pset->pset, psub->pset);
}

void set_get(object_t set, int idex, object_t sub)
{
	set_t* pset = (set_t*)set;
	set_t* psub = (set_t*)sub;

	XDL_ASSERT(set && set->obj == _OBJECT_SET);
	XDL_ASSERT(sub && sub->obj == _OBJECT_SET);

	_set_ele_get(pset->pset, idex, psub->pset);
}

void set_parse(object_t set, const tchar_t* token, int len)
{
	set_t* pt = (set_t*)set;
	int size = 0;

	XDL_ASSERT(set && set->obj == _OBJECT_SET);

	_set_ele_empty(pt->pset);

	_set_ele_parse(&(pt->pset->pset), &size, token, len);
	pt->pset->size = size;
}

int set_format(object_t set, tchar_t* buf, int max)
{
	set_t* pt = (set_t*)set;

	XDL_ASSERT(set && set->obj == _OBJECT_SET);

	if (pt->pset->type == _SET_SET)
		return _set_ele_format(pt->pset->pset, pt->pset->size, buf, max);
	else
		return _set_ele_format(pt->pset, 1, buf, max);
}

#ifdef _DEBUG
void test_set()
{
	tchar_t num[NUM_LEN];
	object_t set;
	object_t sub;
	int i;

	set = set_alloc();
	sub = 

	for (int i = 0; i < 10; i++)
	{
		xsprintf(num, _T("%d"), i);

		ve.type = _SET_NUM;
		ve.size = 1;
		ve.data = (double)i;

		_set_ele_add(pset, &ve);

		_set_ele_empty(&ve);
	}

	int len = _set_ele_format(pset, NULL, MAX_LONG);
	tchar_t* buf = xsalloc(len + 1);
	set_format(pset, buf, len);

	_tprintf(_T("%s\n"), buf);

	xsfree(buf);

	_set_ele_empty(pset);

	_set_ele_parse(pset, _T("{1,2,3,{1,2}, {1,2,3},{1,2,{1,2,3,4}}}"), -1);

	len = _set_ele_format(pset, NULL, MAX_LONG);
	buf = xsalloc(len + 1);
	set_format(pset, buf, len);

	_tprintf(_T("%s\n"), buf);

	xsfree(buf);

	for (int i = 0; i < pset->size; i++)
	{
		_set_ele_get(pset, i, &ve);

		len = set_format(&ve, NULL, MAX_LONG);
		buf = xsalloc(len + 1);
		_set_ele_format(&ve, buf, len);

		_tprintf(_T("%s\n"), buf);

		xsfree(buf);

		_set_ele_empty(&ve);
	}

	_set_ele_free(pset);
}
#endif