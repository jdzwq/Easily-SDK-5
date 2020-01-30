/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc stringtable document

	@module	stringtable.c | implement file

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

#include "stringtable.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

typedef struct _string_table{
	link_t lk;			/* str table self link component*/
	link_t lkEntitys;	/* str table master root link ptr array*/
	int order;		/*sort rule 1: ascend,-1: descend, 0: none*/
}string_table_t;

typedef struct _string_entity_t{
	link_t lk;			/* entity self link component*/
	unsigned int mask; /* endtity runtime state*/
	tchar_t* key;		/* entity key, can not be null*/
	tchar_t* val;		/* entity value*/
	var_long data;			/* entiry data for runtime use*/
}string_entity_t;

/* restore str table struct ptr from link ptr*/
#define StringTableFromLink(p) TypePtrFromLink(string_table_t,p)
/* restore str entity struct ptr from link ptr*/
#define StringEntityFromLink(p) TypePtrFromLink(string_entity_t,p)


link_t_ptr create_string_table(int order)
{
	string_table_t* pht;

	pht = (string_table_t*)xmem_alloc(sizeof(string_table_t));
	pht->order = order;
	pht->lk.tag = lkStringTable;
	init_root_link(&pht->lkEntitys);

	return &(pht->lk);
}

void destroy_string_table(link_t_ptr ptr)
{
	string_table_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	clear_string_table(ptr);

	xmem_free(pht);
}

void clear_string_table(link_t_ptr ptr)
{
	string_table_t* pht;
	link_t_ptr plk;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	while (plk = get_string_next_entity(ptr, LINK_FIRST))
		delete_string_entity(ptr, plk);
}

bool_t is_string_table(link_t_ptr ptr)
{
	if (ptr)
		return (ptr->tag == lkStringTable) ? 1 : 0;
	else
		return 0;
}

bool_t is_string_entity(link_t_ptr ptr, link_t_ptr ilk)
{
	link_t_ptr plk;

	plk = get_string_next_entity(ptr, LINK_FIRST);
	while (plk)
	{
		if (plk == ilk)
			return 1;

		plk = get_string_next_entity(ptr, plk);
	}

	return 0;
}

int get_string_entity_count(link_t_ptr ptr)
{
	string_table_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	return get_link_count(&pht->lkEntitys);
}

link_t_ptr write_string_entity(link_t_ptr ptr, const tchar_t* key, int keylen, const tchar_t* val, int vallen)
{
	string_table_t* pht;
	string_entity_t* phe;
	link_t_ptr plk;
	int rt;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	if (is_null(key) || !keylen)
		return NULL;

	pht = StringTableFromLink(ptr);

	if (pht->order == ORDER_ASCEND)
		plk = get_string_prev_entity(ptr, LINK_LAST);
	else if (pht->order == ORDER_DESCEND)
		plk = get_string_next_entity(ptr, LINK_FIRST);
	else
		plk = get_string_next_entity(ptr, LINK_FIRST);

	while (plk)
	{
		phe = StringEntityFromLink(plk);

		rt = compare_text(phe->key, -1, key, keylen, 0);
		if (rt == 0)
		{
			set_string_entity_val(plk, val, vallen);
			return plk;
		}
		
		if (rt < 0 && (pht->order != ORDER_NONE))
			break;

		if (pht->order == ORDER_ASCEND)
			plk = get_string_prev_entity(ptr, plk);
		else if (pht->order == ORDER_DESCEND)
			plk = get_string_next_entity(ptr, plk);
		else
			plk = get_string_next_entity(ptr, plk);
	}

	phe = (string_entity_t*)xmem_alloc(sizeof(string_entity_t));
	phe->lk.tag = lkStringEntity;

	if (keylen < 0)
		keylen = xslen(key);

	phe->key = (tchar_t*)xmem_alloc((keylen + 1) * sizeof(tchar_t));
	xsncpy(phe->key, key, keylen);

	if (val && vallen)
	{
		if (vallen < 0)
			vallen = xslen(val);
		phe->val = (tchar_t*)xmem_alloc((vallen + 1) * sizeof(tchar_t));
		xsncpy(phe->val, val, vallen);
	}

	if (pht->order == ORDER_ASCEND)
		insert_link_after(&pht->lkEntitys, ((plk) ? plk : LINK_FIRST), &phe->lk);
	else if (pht->order == ORDER_DESCEND)
		insert_link_before(&pht->lkEntitys, ((plk) ? plk : LINK_LAST), &phe->lk);
	else
		insert_link_after(&pht->lkEntitys, LINK_LAST, &phe->lk);

	return &(phe->lk);
}

int read_string_entity(link_t_ptr ptr, const tchar_t* key, int keylen, tchar_t* buf, int max)
{
	link_t_ptr ent;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	ent = get_string_entity(ptr, key, keylen);
	if (ent)
	{
		return get_string_entity_val(ent, buf, max);
	}
	else
	{
		return 0;
	}
}

const tchar_t* get_string_entity_ptr(link_t_ptr ptr, const tchar_t* key, int keylen)
{
	link_t_ptr ent;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	ent = get_string_entity(ptr, key, keylen);
	if (ent)
	{
		return get_string_entity_val_ptr(ent);
	}
	else
	{
		return NULL;
	}
}

link_t_ptr	get_string_entity(link_t_ptr ptr, const tchar_t* key, int keylen)
{
	string_entity_t* phe;
	string_table_t* pht;
	link_t_ptr plk;
	int rt;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	if (is_null(key) || !keylen)
		return NULL;

	pht = StringTableFromLink(ptr);

	if (pht->order == ORDER_ASCEND)
		plk = get_string_prev_entity(ptr, LINK_LAST);
	else if (pht->order == ORDER_DESCEND)
		plk = get_string_next_entity(ptr, LINK_FIRST);
	else
		plk = get_string_next_entity(ptr, LINK_FIRST);

	while (plk)
	{
		phe = StringEntityFromLink(plk);

		rt = compare_text(phe->key, -1, key, keylen, 0);
		if (rt == 0)
			return plk;
		
		if (rt < 0 && (pht->order != ORDER_NONE))
			break;

		if (pht->order == ORDER_ASCEND)
			plk = get_string_prev_entity(ptr, plk);
		else if (pht->order == ORDER_DESCEND)
			plk = get_string_next_entity(ptr, plk);
		else
			plk = get_string_next_entity(ptr, plk);
	}

	return NULL;
}

link_t_ptr	find_string_entity(link_t_ptr ptr, const tchar_t* key, int keylen)
{
	string_entity_t* phe;
	string_table_t* pht;
	link_t_ptr plk;
	int rt;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	if (is_null(key) || !keylen)
		return NULL;

	pht = StringTableFromLink(ptr);

	plk = get_string_next_entity(ptr, LINK_FIRST);
	while (plk)
	{
		phe = StringEntityFromLink(plk);

		rt = compare_text(phe->key, -1, key, keylen, 1);
		if (rt == 0)
			return plk;

		plk = get_string_next_entity(ptr, plk);
	}

	return NULL;
}

link_t_ptr insert_string_entity(link_t_ptr ptr, link_t_ptr pos)
{
	string_table_t* pht;
	string_entity_t* phe;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);

	phe = (string_entity_t*)xmem_alloc(sizeof(string_entity_t));
	phe->lk.tag = lkStringEntity;

	insert_link_after(&pht->lkEntitys, pos, &phe->lk);

	return &(phe->lk);
}

void delete_string_entity(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr elk;
	string_entity_t* phe;
	string_table_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pst = StringTableFromLink(ptr);
	elk = delete_link(&pst->lkEntitys, pos);
	if (!elk)
		return;

	phe = StringEntityFromLink(elk);

	xmem_free(phe->key);

	if (phe->val)
		xmem_free(phe->val);

	xmem_free(phe);
}

const tchar_t* get_string_entity_key_ptr(link_t_ptr elk)
{
	string_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);
	return phe->key;
}

int get_string_entity_key(link_t_ptr elk, tchar_t* buf, int max)
{
	string_entity_t* phe;
	int len;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);

	len = xslen(phe->key);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, phe->key, len);
	}

	return len;
}

void set_string_entity_key(link_t_ptr elk, const tchar_t* key, int len)
{
	string_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);

	if (len < 0)
		len = xslen(key);
	if (!len)
		return;

	phe->key = (tchar_t*)xmem_realloc(phe->key, (len + 1) * sizeof(tchar_t));
	xsncpy(phe->key, key, len);
}

const tchar_t* get_string_entity_val_ptr(link_t_ptr elk)
{
	string_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);
	return phe->val;
}

int get_string_entity_val(link_t_ptr elk, tchar_t* buf, int max)
{
	string_entity_t* phe;
	int len;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);

	len = xslen(phe->val);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, phe->val, len);
	}

	return len;
}

void set_string_entity_val(link_t_ptr elk, const tchar_t* val, int len)
{
	string_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);

	if (!is_null(val) && len)
	{
		if (len < 0)
			len = xslen(val);

		phe->val = (tchar_t*)xmem_realloc(phe->val, (len + 1) * sizeof(tchar_t));
		xsncpy(phe->val, val, len);
	}
	else if (phe->val)
	{
		xmem_free(phe->val);
		phe->val = NULL;
	}
}

void set_string_entity_delta(link_t_ptr elk, var_long data)
{
	string_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);
	phe->data = data;
}

var_long get_string_entity_delta(link_t_ptr elk)
{
	string_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);
	return phe->data;
}

void set_string_entity_mask(link_t_ptr elk, dword_t mask)
{
	string_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);
	phe->mask = mask;
}

dword_t get_string_entity_mask(link_t_ptr elk)
{
	string_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);
	return phe->mask;
}

link_t_ptr get_string_next_entity(link_t_ptr ptr, link_t_ptr pos)
{
	string_table_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pst = StringTableFromLink(ptr);
	if (pos == LINK_FIRST)
		return get_first_link(&pst->lkEntitys);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_string_prev_entity(link_t_ptr ptr, link_t_ptr pos)
{
	string_table_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pst = StringTableFromLink(ptr);
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_last_link(&pst->lkEntitys);
	else
		return get_prev_link(pos);
}

link_t_ptr get_string_entity_at(link_t_ptr ptr, int index)
{
	link_t_ptr ilk;

	if (index < 0)
		return NULL;

	ilk = get_string_next_entity(ptr, LINK_FIRST);
	while (ilk && index--)
		ilk = get_string_next_entity(ptr, ilk);

	return ilk;
}

int get_string_updated_entity_count(link_t_ptr ptr)
{
	link_t_ptr ilk;
	int count = 0;

	ilk = get_string_next_entity(ptr, LINK_FIRST);
	while (ilk)
	{
		count += get_string_entity_dirty(ilk);

		ilk = get_string_next_entity(ptr, ilk);
	}

	return count;
}

int get_string_entity_index(link_t_ptr ptr, link_t_ptr tlk)
{
	link_t_ptr plk;
	int index = -1;

	plk = get_string_next_entity(ptr, LINK_FIRST);
	while (plk)
	{
		index++;
		if (plk == tlk)
			break;
		plk = get_string_next_entity(ptr, tlk);
	}

	return index;
}

void string_table_parse_attrset(link_t_ptr ptr, const tchar_t* attrset, int len)
{
	tchar_t* token;
	tchar_t *key, *val;
	int keylen, vallen;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	clear_string_table(ptr);

	if (is_null(attrset) || !len)
		return;

	if (len < 0)
		len = xslen(attrset);

	token = (tchar_t*)attrset;
	while (*token && len)
	{
		key = token;
		while (*token != _T(' ') && *token != _T('\t') && *token != _T('=') && *token != _T('\0'))
		{
			token++;
			len--;
		}
		keylen = (int)(token - key);

		if (token == _T('\0'))
			break;

		//skip ' ','='
		while (*token != _T('\'') && *token != _T('\"') && *token != _T('\0'))
		{
			token++;
			len--;
		}

		if (token == _T('\0'))
			break;

		//skip '\'','\"'
		token++;
		len--;

		val = token;
		while (*token != _T('\'') && *token != _T('\"') && *token != _T('\0'))
		{
			token++;
			len--;
		}
		vallen = (int)(token - val);

		write_string_entity(ptr, key, keylen, val, vallen);

		//skip ' ','\'','\"'
		while (*token == _T('\'') || *token == _T('\"') || *token == _T(' ') || *token == _T('\t'))
		{
			token++;
			len--;
		}

		if (*token == _T('\0'))
			break;
	}
}

int string_table_format_attrset(link_t_ptr ptr, tchar_t* buf, int max)
{
	string_table_t* pht;
	string_entity_t* phe;
	link_t_ptr plk;
	int total, len;

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	total = 0;

	plk = get_string_next_entity(ptr, LINK_FIRST);
	while (plk)
	{
		phe = StringEntityFromLink(plk);

		len = xslen(phe->key) + ((phe->val) ? xslen(phe->val) : 0) + 4; /*include two quaute and one '=' and one ' '*/
		if (total + len > max)
			return total;

		if (buf)
		{
			xsprintf(buf + total, _T("%s=%c%s%c "), phe->key, _T('\"'), ((phe->val) ? phe->val : _T("")), _T('\"'));
		}
		total += len;

		plk = get_string_next_entity(ptr, plk);
	}
	return total;
}

int string_table_format_key_tokens(link_t_ptr ptr, tchar_t* buf, int max, tchar_t feed)
{
	string_table_t* pht;
	string_entity_t* phe;
	link_t_ptr plk;
	int total, len;
	tchar_t pch[2] = { 0 };

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pch[0] = feed;

	pht = StringTableFromLink(ptr);
	total = 0;

	plk = get_string_next_entity(ptr, LINK_FIRST);
	while (plk)
	{
		phe = StringEntityFromLink(plk);

		len = xslen(phe->key);
		if (total + len > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, phe->key, len);
		}
		total += len;

		if (total + 1 > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, pch, 1);
		}
		total += 1;

		plk = get_string_next_entity(ptr, plk);
	}

	return total;
}

int string_table_format_val_tokens(link_t_ptr ptr, tchar_t* buf, int max, tchar_t feed)
{
	string_table_t* pht;
	string_entity_t* phe;
	link_t_ptr plk;
	int total, len;
	tchar_t pch[2] = { 0 };

	XDL_ASSERT(ptr && ptr->tag == lkStringTable);

	pch[0] = feed;

	pht = StringTableFromLink(ptr);
	total = 0;

	plk = get_string_next_entity(ptr, LINK_FIRST);
	while (plk)
	{
		phe = StringEntityFromLink(plk);

		len = xslen(phe->val);
		if (total + len > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, phe->val, len);
		}
		total += len;

		if (total + 1 > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, pch, 1);
		}
		total += 1;

		plk = get_string_next_entity(ptr, plk);
	}

	return total;
}

typedef struct _ST_FORMAT_PARAM{
	link_t_ptr ptr;
	link_t_ptr ent;
}ST_FORMAT_PARAM;

static bool_t _on_parse_token(void* pp, const tchar_t* key, int klen, const tchar_t* val, int vlen)
{
	link_t_ptr st = (link_t_ptr)pp;

	write_string_entity(st, key, klen, val, vlen);

	return 1;
}

static bool_t _on_format_token(void* fp, const tchar_t** pkey, int* pklen, const tchar_t** pval, int* pvlen)
{
	ST_FORMAT_PARAM* pfp = (ST_FORMAT_PARAM*)fp;
	link_t_ptr ent;

	ent = get_string_next_entity(pfp->ptr, pfp->ent);

	if (!ent)
		return 0;

	pfp->ent = ent;

	*pkey = get_string_entity_key_ptr(ent);
	*pklen = xslen(*pkey);

	*pval = get_string_entity_val_ptr(ent);
	*pvlen = xslen(*pval);

	return 1;
}

int string_table_parse_options(link_t_ptr ptr, const tchar_t* str, int len, tchar_t itemfeed, tchar_t linefeed)
{
	return parse_options(str, len, itemfeed, linefeed, (void*)ptr, _on_parse_token);
}

int string_table_format_options(link_t_ptr ptr, tchar_t* buf, int max, tchar_t itemfeed, tchar_t linefeed)
{
	ST_FORMAT_PARAM fp = { 0 };

	fp.ptr = ptr;
	fp.ent = LINK_FIRST;

	return format_options(buf, max, itemfeed, linefeed, (void*)&fp, _on_format_token);
}