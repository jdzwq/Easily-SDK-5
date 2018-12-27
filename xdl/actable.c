/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc Aho-Corasick automaton document

	@module	actable.c | ac table implement file

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

#include "actable.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#define AC_CHAR_INDEX(ch)		((ch >= _T('A') && ch <= _T('Z'))? (ch - 64) : 0)
#define AC_BASE_INC			127

typedef struct _ac_node_t{
	sword_t ind; /*node key index*/
	dword_t pre; /*prior state*/
}ac_node_t;

typedef struct _ac_leaf_t{
	bool_t leaf;
	union{
		dword_t fail; /*failed pointer*/
		var_long delta; /*user key delta*/
	};
}ac_leaf_t;

typedef struct _ac_table_t{
	link_t lk;
	
	dword_t	size; /*check table size*/
	dword_t* check; /*state check table*/

	dword_t	state; /*current state*/
	ac_node_t* base; /*ac node array*/
	ac_leaf_t* data; /*ac leaf array*/
}ac_table_t;

#define ACTableFromLink(p) TypePtrFromLink(ac_table_t,p)

link_t_ptr create_ac_table()
{
	ac_table_t* ptt;

	ptt = (ac_table_t*)xmem_alloc(sizeof(ac_table_t));
	ptt->lk.tag = lkACTable;

	ptt->base = (ac_node_t*)xmem_alloc((AC_BASE_INC + 1) * sizeof(ac_node_t));
	ptt->data = (ac_leaf_t*)xmem_alloc((AC_BASE_INC + 1) * sizeof(ac_leaf_t));

	return &ptt->lk;
}

void destroy_ac_table(link_t_ptr tt)
{
	ac_table_t* ptt = ACTableFromLink(tt);

	XDL_ASSERT(tt && tt->tag == lkACTable);

	if (ptt->check)
		xmem_free(ptt->check);

	if (ptt->base)
		xmem_free(ptt->base);

	if (ptt->data)
		xmem_free(ptt->data);

	xmem_free(ptt);
}

void insert_ac_table(link_t_ptr tt, const tchar_t* key, int len, var_long delta)
{
	ac_table_t* ptt = ACTableFromLink(tt);
	int i;
	sword_t ind;
	dword_t sta, pos;

	XDL_ASSERT(tt && tt->tag == lkACTable);

	if (len < 0)
		len = xslen(key);

	sta = 0;

	for (i = 0; i < len; i++)
	{
		ind = AC_CHAR_INDEX(key[i]);
		if (!ind)
			continue;

		pos = ptt->base[sta].ind + ind;
		if (pos >= ptt->size)
		{
			ptt->size = pos + 1;
			ptt->check = (dword_t*)xmem_realloc(ptt->check, ptt->size * sizeof(dword_t));
		}

		if (!ptt->check[pos])
		{// no collision
			if (ptt->state && !(ptt->state % AC_BASE_INC))
			{
				ptt->base = (ac_node_t*)xmem_realloc(ptt->base, (ptt->state + 1 + AC_BASE_INC) * sizeof(ac_node_t));
				ptt->data = (ac_leaf_t*)xmem_realloc(ptt->data, (ptt->state + 1 + AC_BASE_INC) * sizeof(ac_leaf_t));
			}
			ptt->state++;
			ptt->check[pos] = ptt->state;
			ptt->base[ptt->state].ind = ind;
			ptt->base[ptt->state].pre = sta; 
		}
		else if (ptt->base[ptt->check[pos]].pre != sta || ptt->base[ptt->check[pos]].ind != ind) //ptt->check[pos] <= sta
		{//resolve collision
			do
			{
				pos += ind;
				if (pos >= ptt->size)
				{
					ptt->size = pos + 1;
					ptt->check = (dword_t*)xmem_realloc(ptt->check, ptt->size * sizeof(dword_t));
				}
			} while (ptt->check[pos] && (ptt->base[ptt->check[pos]].pre != sta || ptt->base[ptt->check[pos]].ind != ind));

			if (!ptt->check[pos])
			{
				if (ptt->state && !(ptt->state % AC_BASE_INC))
				{
					ptt->base = (ac_node_t*)xmem_realloc(ptt->base, (ptt->state + 1 + AC_BASE_INC) * sizeof(ac_node_t));
					ptt->data = (ac_leaf_t*)xmem_realloc(ptt->data, (ptt->state + 1 + AC_BASE_INC) * sizeof(ac_leaf_t));
				}
				ptt->state++;
				ptt->check[pos] = ptt->state;
				ptt->base[ptt->state].ind = ind;
				ptt->base[ptt->state].pre = sta;
			}
		}

		sta = ptt->check[pos];
	}

	ptt->data[ptt->state].leaf = 1;
	ptt->data[ptt->state].delta = delta;
}

static dword_t _next_state(ac_table_t* ptt, dword_t sta, sword_t ind)
{
	dword_t pos;

	if (!ind)
		return 0;

	pos = ptt->base[sta].ind + ind;
	if (!ptt->check[pos])
	{
		return 0;
	}
	while (ptt->check[pos] && (ptt->base[ptt->check[pos]].pre != sta || ptt->base[ptt->check[pos]].ind != ind))
	{
		pos += ind;
	}

	if (pos >= ptt->size)
		return 0;

	return (ptt->check[pos]) ? ptt->check[pos] : 0;
}


var_long find_ac_table(link_t_ptr tt, const tchar_t* key, int len)
{
	ac_table_t* ptt = ACTableFromLink(tt);
	int i;
	sword_t ind;
	dword_t sta;

	XDL_ASSERT(tt && tt->tag == lkACTable);

	if (len < 0)
		len = xslen(key);

	sta = 0;

	for (i = 0; i < len; i++)
	{
		ind = AC_CHAR_INDEX(key[i]);
		if (!ind)
			continue;

		sta = _next_state(ptt, sta, ind);

		if (!sta)
			return 0;
	}

	return ptt->data[sta].delta;
}

typedef struct _ac_fail_t{
	sword_t ind; /*current char index*/
	dword_t sta; /*current state*/
}ac_fail_t;

void build_ac_table(link_t_ptr tt)
{
	ac_table_t* ptt = ACTableFromLink(tt);
	dword_t i, pre, sta;
	ac_fail_t *ptf = NULL;
	int k, n = 0;

	XDL_ASSERT(tt && tt->tag == lkACTable);

	for (i = 1; i < ptt->size; i++)
	{
		//terminal state
		sta = ptt->check[i];
		if (!ptt->data[sta].leaf)
			continue;

		//prior state
		sta = ptt->base[sta].pre;

		n = 0;
		//backtacking, get maximized sub key
		while (sta)
		{
			pre = ptt->base[sta].pre;
			if (!pre)
				break;

			if (!(n % 16))
			{
				ptf = (ac_fail_t*)xmem_realloc(ptf, (n + 16) * sizeof(ac_fail_t));
			}

			xmem_move((void*)ptf, n * sizeof(ac_fail_t), sizeof(ac_fail_t));
			ptf->ind = ptt->base[sta].ind;
			ptf->sta = sta;
			n++;

			//prior state
			sta = ptt->base[sta].pre;
		}

		//build failed pointer
		sta = 0;
		k = 0;
		while (k < n)
		{
			sta = _next_state(ptt, sta, ptf[k].ind);

			if (sta)
			{
				ptt->data[ptf[k].sta].fail = sta;
			}
			
			k++;
		}

		xmem_free(ptf);
		ptf = NULL;
	}
}

void enum_ac_table(link_t_ptr tt, PF_ENUM_AC_TABLE pf, void* pa)
{
	ac_table_t* ptt = ACTableFromLink(tt);
	dword_t i, sta;
	tchar_t ch;
	tchar_t *key = NULL;
	int n = 0;
	bool_t rt;

	XDL_ASSERT(tt && tt->tag == lkACTable);

	for (i = 1; i < ptt->size; i++)
	{
		//terminal state
		sta = ptt->check[i];
		if (!ptt->data[sta].leaf)
			continue;

		n = 0;
		//backtacking
		while (sta)
		{
			ch = (tchar_t)(ptt->base[sta].ind + 64);
			
			if (!(n % 32))
			{
				key = xsrealloc(key, n + 32);
			}

			xsnins(key, 0, &ch, 1);
			n++;

			//prior state
			sta = ptt->base[sta].pre;
		}

		if (pf)
			rt = (*pf)(key, n, ptt->data[ptt->check[i]].delta, pa);
		else
			rt = 1;

		xsfree(key);
		key = NULL;

		if (!rt)
			break;
	}
}

#if defined(_DEBUG) || defined(DEBUG)

void trace_ac_table(link_t_ptr tt, const tchar_t* key, int len)
{
	ac_table_t* ptt = ACTableFromLink(tt);
	int i, j;
	sword_t ind;
	dword_t sta, nxt;
	tchar_t *sub = NULL;
	int k = 0;

	XDL_ASSERT(tt && tt->tag == lkACTable);

	if (len < 0)
		len = xslen(key);

	sub = xsalloc(len + 1);

	_tprintf(_T("%s:\t"), key);

	//KMP match
	sta = 0;
	i = 0;
	j = i;
	while (i < len)
	{
		ind = (j < len)? AC_CHAR_INDEX(key[j]) : 0;

		nxt = _next_state(ptt, sta, ind);
		if (!nxt)
		{
			sta = (ptt->data[sta].leaf)? 0 : ptt->data[sta].fail;

			if (sta)
			{
				nxt = _next_state(ptt, sta, ind);
			}
		}

		if (nxt)
		{
			j++;
		}
		else
		{
			i++;
			j = i;
		}

		if (nxt)
		{
			sub[k++] = (tchar_t)(ind + 64);
			if (ptt->data[nxt].leaf)
			{
				_tprintf(_T("%s;"), sub);
				xszero(sub, len + 1);
				k = 0;
			}
		}

		sta = nxt;
	}

	xsfree(sub);

	_tprintf(_T("\n"));
}

static bool_t printf_ac_node(const tchar_t* key, int len, var_long delta, void* p)
{
	_tprintf(_T("%s\t%d\n"), key, (int)delta);

	return 1;
}

void test_ac_table()
{
	link_t_ptr tt;

	tt = create_ac_table();
	
	//test case 1
	insert_ac_table(tt, _T("AP"), -1, 1);
	insert_ac_table(tt, _T("BA"), -1, 2);
	insert_ac_table(tt, _T("ABC"), -1, 3);
	insert_ac_table(tt, _T("BCAP"), -1, 4);

	//test case 2
	/*insert_ac_table(tt, _T("POOL"), -1, 5);
	insert_ac_table(tt, _T("PRIZE"), -1, 6);
	insert_ac_table(tt, _T("PREVIEW"), -1, 7);
	insert_ac_table(tt, _T("PREPARE"), -1, 8);
	insert_ac_table(tt, _T("PRODUCE"), -1, 9);
	insert_ac_table(tt, _T("PROGRESS"), -1, 10);*/

	build_ac_table(tt);

	//test case 1
	//trace_ac_table(tt, _T("AP"), -1);
	//trace_ac_table(tt, _T("BA"), -1);
	//trace_ac_table(tt, _T("ABC"), -1);
	//trace_ac_table(tt, _T("BCAP"), -1);
	trace_ac_table(tt, _T("ABCAPBABC"), -1);

	//test case 2
	/*trace_ac_table(tt, _T("POOL"), -1);
	trace_ac_table(tt, _T("PRIZE"), -1);
	trace_ac_table(tt, _T("PREVIEW"), -1);
	trace_ac_table(tt, _T("PREPARE"), -1);
	trace_ac_table(tt, _T("PRODUCE"), -1);
	trace_ac_table(tt, _T("PROGRESS"), -1);*/

	enum_ac_table(tt, printf_ac_node, NULL);

	destroy_ac_table(tt);
}
#endif
