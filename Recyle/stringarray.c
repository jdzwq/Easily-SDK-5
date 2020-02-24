/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc stringarray document

	@module	stringarray.c | stringarray implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#include "stringarray.h"
#include "xdllink.h"
#include "xdlstr.h"
#include "xdlmem.h"
#include "xdlerr.h"
#include "xdlutil.h"

typedef struct _string_array{
	link_t lk;			/* str table self link component*/
	link_t lkItems;	/* str table master root link ptr array*/
}string_array_t;

typedef struct _string_item_t{
	link_t lk;			/* item self link component*/
	void* data;			/* entiry data for runtime use*/
	tchar_t* text;		/* item text*/
}string_item_t;

/* restore str table struct ptr from link ptr*/
#define StringArrayFromLink(p) TypePtrFromLink(string_array_t,p)
/* restore str item struct ptr from link ptr*/
#define StringItemFromLink(p) TypePtrFromLink(string_item_t,p)

int string_array_sort(link_t_ptr plk1, link_t_ptr plk2, void* param)
{
	string_item_t *pse1, *pse2;

	pse1 = StringItemFromLink(plk1);
	pse2 = StringItemFromLink(plk2);

	return xsicmp(pse1->text, pse2->text);
}

link_t_ptr create_string_array()
{
	string_array_t* pht;

	pht = (string_array_t*)xmem_alloc(sizeof(string_array_t));
	pht->lk.tag = lkStringArray;
	init_root_link(&pht->lkItems);

	return &(pht->lk);
}

void destroy_string_array(link_t_ptr ptr)
{
	string_array_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pht = StringArrayFromLink(ptr);
	clear_string_array(ptr);

	xmem_free(pht);
}

void clear_string_array(link_t_ptr ptr)
{
	string_array_t* pht;
	link_t_ptr plk;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pht = StringArrayFromLink(ptr);
	while (plk = get_string_next_item(ptr, LINK_FIRST))
		delete_string_item(ptr, plk);
}

bool_t is_string_array(link_t_ptr ptr)
{
	if (ptr)
		return (ptr->tag == lkStringArray) ? 1 : 0;
	else
		return 0;
}

bool_t is_string_item(link_t_ptr ptr, link_t_ptr ilk)
{
	link_t_ptr plk;

	plk = get_string_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		if (plk == ilk)
			return 1;

		plk = get_string_next_item(ptr, plk);
	}

	return 0;
}

void sort_string_array(link_t_ptr ptr, int desc)
{
	string_array_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pht = StringArrayFromLink(ptr);
	bubble_sort_link(&pht->lkItems, (PF_LINKSORT)string_array_sort, desc, NULL);
}

int get_string_item_count(link_t_ptr ptr)
{
	string_array_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pht = StringArrayFromLink(ptr);
	return get_link_count(&pht->lkItems);
}

link_t_ptr insert_string_item(link_t_ptr ptr, link_t_ptr pos)
{
	string_array_t* pht;
	string_item_t* phe;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pht = StringArrayFromLink(ptr);

	phe = (string_item_t*)xmem_alloc(sizeof(string_item_t));
	phe->lk.tag = lkStringItem;

	insert_link_after(&pht->lkItems, pos, &phe->lk);

	return &(phe->lk);
}

void delete_string_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;
	string_item_t* phe;
	string_array_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pst = StringArrayFromLink(ptr);
	ilk = delete_link(&pst->lkItems, pos);
	if (!ilk)
		return;

	phe = StringItemFromLink(ilk);

	xmem_free(phe->text);

	xmem_free(phe);
}

const tchar_t* get_string_item_text_ptr(link_t_ptr ilk)
{
	string_item_t* phe;

	XDL_ASSERT(ilk && ilk->tag == lkStringItem);

	phe = StringItemFromLink(ilk);

	return phe->text;
}

void set_string_item_text(link_t_ptr ilk, const tchar_t* val, int len)
{
	string_item_t* phe;

	XDL_ASSERT(ilk && ilk->tag == lkStringItem);

	phe = StringItemFromLink(ilk);

	if (!is_null(val) && len)
	{
		if (len < 0)
			len = xslen(val);

		phe->text = (tchar_t*)xmem_realloc(phe->text, (len + 1) * sizeof(tchar_t));
		xsncpy(phe->text, val, len);
	}
	else if (phe->text)
	{
		xmem_free(phe->text);
		phe->text = NULL;
	}
}

void set_string_item_delta(link_t_ptr ilk, void* data)
{
	string_item_t* phe;

	XDL_ASSERT(ilk && ilk->tag == lkStringItem);

	phe = StringItemFromLink(ilk);
	phe->data = data;
}

void* get_string_item_delta(link_t_ptr ilk)
{
	string_item_t* phe;

	XDL_ASSERT(ilk && ilk->tag == lkStringItem);

	phe = StringItemFromLink(ilk);
	return phe->data;
}

link_t_ptr get_string_next_item(link_t_ptr ptr, link_t_ptr pos)
{
	string_array_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pst = StringArrayFromLink(ptr);
	if (pos == LINK_FIRST)
		return get_first_link(&pst->lkItems);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_string_prev_item(link_t_ptr ptr, link_t_ptr pos)
{
	string_array_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pst = StringArrayFromLink(ptr);
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_last_link(&pst->lkItems);
	else
		return get_prev_link(pos);
}

link_t_ptr get_string_item_at(link_t_ptr ptr, int index)
{
	link_t_ptr ilk;

	if (index < 0)
		return NULL;

	ilk = get_string_next_item(ptr, LINK_FIRST);
	while (ilk && index--)
		ilk = get_string_next_item(ptr, ilk);

	return ilk;
}

int get_string_item_index(link_t_ptr ptr, link_t_ptr tlk)
{
	link_t_ptr plk;
	int index = -1;

	plk = get_string_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		index++;
		if (plk == tlk)
			break;
		plk = get_string_next_item(ptr, tlk);
	}

	return index;
}

void string_array_parse_tokens(link_t_ptr ptr, const tchar_t* tokens, int len, tchar_t feed)
{
	const tchar_t* tmp;
	tchar_t *key;
	int keylen;
	link_t_ptr ilk;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	clear_string_array(ptr);

	if (is_null(tokens) || !len)
		return;

	if (len < 0)
		len = xslen(tokens);

	while (*tokens && len)
	{
		key = NULL;
		keylen = 0;

		tmp = parse_string_token(tokens, len, feed, &key, &keylen);

		if (keylen)
		{
			ilk = insert_string_item(ptr, LINK_LAST);
			set_string_item_text(ilk, key, keylen);
		}

		if (!tmp)
			break;

		len -= (int)(tmp - tokens);
		tokens = tmp;
	}
}

int string_array_format_tokens(link_t_ptr ptr, tchar_t* buf, int max, tchar_t feed)
{
	string_array_t* pht;
	string_item_t* phe;
	link_t_ptr plk;
	int total, len;

	XDL_ASSERT(ptr && ptr->tag == lkStringArray);

	pht = StringArrayFromLink(ptr);
	total = 0;

	plk = get_string_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		phe = StringItemFromLink(plk);

		len = xslen(phe->text) + 1; /*include itemfeed*/
		if (total + len > max)
			return total;

		if (buf)
		{
			xsprintf(buf + total, _T("%s%c"), phe->text, feed);
		}
		total += len;

		plk = get_string_next_item(ptr, plk);
	}
	return total;
}
