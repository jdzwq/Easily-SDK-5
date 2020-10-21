/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc wordstable document

	@module	wordstable.c | implement file

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

#include "wordstable.h"
#include "xdlimp.h"

#include "xdlstd.h"

typedef struct _words_table{
	link_t lk;			/* str table self link component*/
	link_t lkItems;	/* str table master root link ptr array*/
	int order;		/*sort rule 1: aorder,-1: deorder, 0: none*/
}words_table_t;

typedef struct _words_item_t{
	link_t lk;			/* item self link component*/
	tchar_t* text;		/* item text*/
	var_long data;		/* entiry data for runtime use*/
}words_item_t;

/* restore str table struct ptr from link ptr*/
#define WordsTableFromLink(p) TypePtrFromLink(words_table_t,p)
/* restore str item struct ptr from link ptr*/
#define WordsItemFromLink(p) TypePtrFromLink(words_item_t,p)


link_t_ptr create_words_table(int order)
{
	words_table_t* pht;

	pht = (words_table_t*)xmem_alloc(sizeof(words_table_t));
	pht->order = order;
	pht->lk.tag = lkWordsTable;
	init_root_link(&pht->lkItems);

	return &(pht->lk);
}

void destroy_words_table(link_t_ptr ptr)
{
	words_table_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pht = WordsTableFromLink(ptr);
	clear_words_table(ptr);

	xmem_free(pht);
}

void clear_words_table(link_t_ptr ptr)
{
	words_table_t* pht;
	link_t_ptr plk;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pht = WordsTableFromLink(ptr);
	while (plk = get_words_next_item(ptr, LINK_FIRST))
		delete_words_item(ptr, plk);
}

bool_t is_words_table(link_t_ptr ptr)
{
	if (ptr)
		return (ptr->tag == lkWordsTable) ? 1 : 0;
	else
		return 0;
}

bool_t is_words_item(link_t_ptr ptr, link_t_ptr ilk)
{
	words_table_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pht = WordsTableFromLink(ptr);

	return (&pht->lkItems == get_root_link(ilk)) ? 1 : 0;
}

int get_words_item_count(link_t_ptr ptr)
{
	words_table_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pht = WordsTableFromLink(ptr);
	return get_link_count(&pht->lkItems);
}

link_t_ptr insert_words_item(link_t_ptr ptr, const tchar_t* val, int len)
{
	words_table_t* pht;
	words_item_t* phe;
	link_t_ptr plk;
	int rt;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pht = WordsTableFromLink(ptr);

	if (pht->order == ORDER_ASCEND)
		plk = get_words_prev_item(ptr, LINK_LAST);
	else if (pht->order == ORDER_DESCEND)
		plk = get_words_next_item(ptr, LINK_FIRST);
	else
		plk = get_words_next_item(ptr, LINK_FIRST);

	while (plk)
	{
		phe = WordsItemFromLink(plk);

		rt = compare_text(phe->text, -1, val, -1, 1);
		if (rt == 0)
			return plk;
		else if (rt < 0 && (pht->order != ORDER_NONE))
			break;

		if (pht->order == ORDER_ASCEND)
			plk = get_words_prev_item(ptr, plk);
		else if (pht->order == ORDER_DESCEND)
			plk = get_words_next_item(ptr, plk);
		else
			plk = get_words_next_item(ptr, plk);
	}

	phe = (words_item_t*)xmem_alloc(sizeof(words_item_t));
	phe->lk.tag = lkWordsItem;
	phe->text = xsnclone(val, len);

	if (pht->order == ORDER_ASCEND)
		insert_link_after(&pht->lkItems, ((plk)? plk : LINK_FIRST), &phe->lk);
	else if (pht->order == ORDER_DESCEND)
		insert_link_before(&pht->lkItems, ((plk) ? plk : LINK_LAST), &phe->lk);
	else
		insert_link_after(&pht->lkItems, LINK_LAST, &phe->lk);

	return &(phe->lk);
}

link_t_ptr get_words_item(link_t_ptr ptr, const tchar_t* val, int len)
{
	words_table_t* pht;
	words_item_t* phe;
	link_t_ptr plk;
	int rt;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pht = WordsTableFromLink(ptr);

	if (pht->order == ORDER_ASCEND)
		plk = get_words_prev_item(ptr, LINK_LAST);
	else if (pht->order == ORDER_DESCEND)
		plk = get_words_next_item(ptr, LINK_FIRST);
	else
		plk = get_words_next_item(ptr, LINK_FIRST);

	while (plk)
	{
		phe = WordsItemFromLink(plk);

		rt = compare_text(phe->text, -1, val, -1, 1);
		if (rt == 0)
			return plk;
		else if (rt < 0 && (pht->order != ORDER_NONE))
			break;

		if (pht->order == ORDER_ASCEND)
			plk = get_words_prev_item(ptr, plk);
		else if (pht->order == ORDER_DESCEND)
			plk = get_words_next_item(ptr, plk);
		else
			plk = get_words_next_item(ptr, plk);
	}

	return NULL;
}

void delete_words_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;
	words_item_t* phe;
	words_table_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pst = WordsTableFromLink(ptr);
	ilk = delete_link(&pst->lkItems, pos);
	if (!ilk)
		return;

	phe = WordsItemFromLink(ilk);

	xmem_free(phe->text);

	xmem_free(phe);
}

const tchar_t* get_words_item_text_ptr(link_t_ptr ilk)
{
	words_item_t* phe;

	XDL_ASSERT(ilk && ilk->tag == lkWordsItem);

	phe = WordsItemFromLink(ilk);

	return phe->text;
}

void set_words_item_delta(link_t_ptr ilk, var_long data)
{
	words_item_t* phe;

	XDL_ASSERT(ilk && ilk->tag == lkWordsItem);

	phe = WordsItemFromLink(ilk);
	phe->data = data;
}

var_long get_words_item_delta(link_t_ptr ilk)
{
	words_item_t* phe;

	XDL_ASSERT(ilk && ilk->tag == lkWordsItem);

	phe = WordsItemFromLink(ilk);
	return phe->data;
}

link_t_ptr get_words_next_item(link_t_ptr ptr, link_t_ptr pos)
{
	words_table_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pst = WordsTableFromLink(ptr);
	if (pos == LINK_FIRST)
		return get_first_link(&pst->lkItems);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_words_prev_item(link_t_ptr ptr, link_t_ptr pos)
{
	words_table_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pst = WordsTableFromLink(ptr);
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_last_link(&pst->lkItems);
	else
		return get_prev_link(pos);
}

link_t_ptr get_words_item_at(link_t_ptr ptr, int index)
{
	link_t_ptr ilk;

	if (index < 0)
		return NULL;

	ilk = get_words_next_item(ptr, LINK_FIRST);
	while (ilk && index--)
		ilk = get_words_next_item(ptr, ilk);

	return ilk;
}

int get_words_item_index(link_t_ptr ptr, link_t_ptr tlk)
{
	link_t_ptr plk;
	int index = -1;

	plk = get_words_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		index++;
		if (plk == tlk)
			break;
		plk = get_words_next_item(ptr, tlk);
	}

	return index;
}

link_t_ptr get_words_next_visible_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		ilk = get_words_next_item(ptr, LINK_FIRST);
	else if (pos == LINK_LAST)
		return NULL;
	else
		ilk = get_words_next_item(ptr, pos);

	while (ilk)
	{
		if (!get_words_item_hidden(ilk))
		{
			break;
		}

		ilk = get_words_next_item(ptr, ilk);
	}

	return ilk;
}

link_t_ptr get_words_prev_visible_item(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		ilk = get_words_prev_item(ptr, LINK_LAST);
	else
		ilk = get_words_prev_item(ptr, pos);

	while (ilk)
	{
		if (!get_words_item_hidden(ilk))
		{
			break;
		}

		ilk = get_words_prev_item(ptr, ilk);
	}

	return ilk;
}

int get_words_visible_item_count(link_t_ptr ptr)
{
	link_t_ptr ilk;
	int count = 0;

	ilk = get_words_next_visible_item(ptr, LINK_FIRST);
	while (ilk)
	{
		count++;
		ilk = get_words_next_visible_item(ptr, ilk);
	}

	return count;
}

void words_table_parse_tokens(link_t_ptr ptr, const tchar_t* tokens, int len, tchar_t feed)
{
	tchar_t *key;
	int keylen;
	link_t_ptr ilk;
	int n, total = 0;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	clear_words_table(ptr);

	if (len < 0)
		len = xslen(tokens);

	if (is_null(tokens) || !len)
		return;

	while (n = parse_string_token((tokens + total), (len - total), feed, &key, &keylen))
	{
		total += n;

		if (keylen)
		{
			ilk = insert_words_item(ptr, key, keylen);
		}
	}
}

int words_table_format_tokens(link_t_ptr ptr, tchar_t* buf, int max, tchar_t feed)
{
	words_table_t* pht;
	words_item_t* phe;
	link_t_ptr plk;
	int total, len;

	XDL_ASSERT(ptr && ptr->tag == lkWordsTable);

	pht = WordsTableFromLink(ptr);
	total = 0;

	plk = get_words_next_item(ptr, LINK_FIRST);
	while (plk)
	{
		phe = WordsItemFromLink(plk);

		len = xslen(phe->text) + 1; /*include itemfeed*/
		if (total + len > max)
			return total;

		if (buf)
		{
			xsprintf(buf + total, _T("%s%c"), phe->text, feed);
		}
		total += len;

		plk = get_words_next_item(ptr, plk);
	}
	return total;
}
