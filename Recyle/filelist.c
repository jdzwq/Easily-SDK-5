/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc file list document

	@module	filelist.c | file list document implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#include "filelist.h"
#include "xdlstd.h"

typedef struct _file_item_t{
	link_t lkSibling;		// the sibling item link component also as self link component
	link_t lkChild;			// the child item link component 
	int count;				// child item count

	dword_t mask;			// runtime file item mask
	void* delta;			// runtime user value

	file_info_t info; //file info
}file_item_t;

/*restore fileitem from self link ptr*/
#define FileItemFromLink(p) TypePtrFromSibling(file_item_t,p)
/*restore fileitem from child link ptr*/
#define FileItemFromChild(p) TypePtrFromChild(file_item_t,p)

file_item_t* _alloc_file_item()
{
	file_item_t* pti;

	pti = (file_item_t*)xmem_alloc(sizeof(file_item_t));
	pti->lkSibling.tag = lkFile;
	pti->lkSibling.next = NULL;
	pti->lkSibling.prev = NULL;
	init_root_link(&pti->lkChild);

	return pti;
}

void _free_file_item(file_item_t* ptt)
{
	xmem_free(ptt);
}

link_t_ptr create_file_list()
{
	file_item_t* pti;

	pti = _alloc_file_item();

	return &(pti->lkSibling);
}

void destroy_file_list(link_t_ptr ptr)
{
	delete_file_item(ptr);
}

link_t_ptr enum_file_item(link_t_ptr ptr, PF_LINKENUM pf, void* param)
{
	link_t_ptr nlk;
	link_t_ptr st = NULL;

	if (!pf)
		return NULL;

	nlk = get_file_first_child_item(ptr);
	while (nlk)
	{
		if (!(*pf)(nlk, param))
			break;

		if (get_file_first_child_item(nlk))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_file_first_child_item(nlk);
			continue;
		}

		while (nlk)
		{
			nlk = get_file_next_sibling_item(nlk);
			if (nlk)
				break;
			nlk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);

	return nlk;
}

link_t_ptr insert_file_item_before(link_t_ptr ilk, link_t_ptr pos)
{
	file_item_t* ptt;
	file_item_t* pup;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = _alloc_file_item();

	pup = FileItemFromLink(ilk);
	insert_link_before(&(pup->lkChild), pos, &(ptt->lkSibling));
	pup->count++;

	return &(ptt->lkSibling);
}

link_t_ptr insert_file_item(link_t_ptr ilk, link_t_ptr pos)
{
	file_item_t* ptt;
	file_item_t* pup;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = _alloc_file_item();

	pup = FileItemFromLink(ilk);
	insert_link_after(&(pup->lkChild), pos, &(ptt->lkSibling));
	pup->count++;

	return &(ptt->lkSibling);
}

void delete_file_item(link_t_ptr ilk)
{
	link_t_ptr parent, child;
	file_item_t* ptt;
	file_item_t* pup;
	link_t_ptr stack;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	stack = NULL;

	while (ilk)
	{
		child = get_file_first_child_item(ilk);
		if (child)
		{
			if (!stack)
				stack = create_stack_table();

			push_stack_node(stack, (void*)ilk);

			ilk = child;
			continue;
		}

		parent = get_file_parent_item(ilk);
		if (parent)
		{
			/* if exist, delete item link from parent item */
			pup = FileItemFromLink(parent);
			delete_link(&(pup->lkChild), ilk);
			pup->count--;
		}

		ptt = FileItemFromLink(ilk);
		_free_file_item(ptt);

		ilk = (stack) ? (link_t_ptr)pop_stack_node(stack) : NULL;
	}

	if (stack)
		destroy_stack_table(stack);
}

void delete_file_child_items(link_t_ptr ilk)
{
	link_t_ptr plk;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	while (plk = get_file_first_child_item(ilk))
		delete_file_item(plk);
}

void set_file_item_mask(link_t_ptr ilk, dword_t mask)
{
	file_item_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	pti = FileItemFromLink(ilk);
	pti->mask = mask;
}

dword_t get_file_item_mask(link_t_ptr ilk)
{
	file_item_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	pti = FileItemFromLink(ilk);
	return pti->mask;
}

void set_file_item_mask_check(link_t_ptr ilk, dword_t ul, bool_t b)
{
	file_item_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	pti = FileItemFromLink(ilk);
	if (b)
		pti->mask |= ul;
	else
		pti->mask &= (~ul);
}

bool_t get_file_item_mask_check(link_t_ptr ilk, dword_t ul)
{
	file_item_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	pti = FileItemFromLink(ilk);

	return (pti->mask & ul) ? 1 : 0;
}

typedef struct _ITEM_MASK_COUNT{
	dword_t mask;
	int count;
}ITEM_MASK_COUNT;

static bool_t _count_check_item(link_t_ptr plk, void* pv)
{
	ITEM_MASK_COUNT* pn = (ITEM_MASK_COUNT*)pv;

	if (get_file_item_mask_check(plk, pn->mask))
		pn->count++;

	return 1;
}

int get_file_child_item_mask_check_count(link_t_ptr ilk, dword_t ul)
{
	ITEM_MASK_COUNT nd = { 0 };

	nd.mask = ul;
	enum_file_item(ilk, (PF_LINKENUM)_count_check_item, (void*)&nd);

	return nd.count;
}

typedef struct _ITEM_MASK_CHECK{
	dword_t mask;
	bool_t check;
	int count;
}ITEM_MASK_CHECK;

static bool_t _reset_check_item(link_t_ptr plk, void* pv)
{
	ITEM_MASK_CHECK* pn = (ITEM_MASK_CHECK*)pv;

	if (get_file_item_mask_check(plk, pn->mask) != pn->check)
		pn->count++;

	set_file_item_mask_check(plk, pn->mask, pn->check);

	return 1;
}

int set_file_child_item_mask_check(link_t_ptr ilk, dword_t ul, bool_t b)
{
	ITEM_MASK_CHECK nd = { 0 };

	nd.mask = ul;
	nd.check = b;
	enum_file_item(ilk, (PF_LINKENUM)_reset_check_item, (void*)&nd);

	return nd.count;
}

void* get_file_item_delta(link_t_ptr ilk)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);

	return ptt->delta;
}

void set_file_item_delta(link_t_ptr ilk, void* delta)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);

	ptt->delta = delta;
}

int get_file_child_item_count(link_t_ptr ilk)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);
	return ptt->count;
}

link_t_ptr get_file_first_child_item(link_t_ptr ilk)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);
	return get_first_link(&ptt->lkChild);
}

link_t_ptr get_file_last_child_item(link_t_ptr ilk)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);
	return get_last_link(&ptt->lkChild);
}

link_t_ptr get_file_parent_item(link_t_ptr ilk)
{
	file_item_t* ptt;
	link_t_ptr parent;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	parent = get_root_link(ilk);

	XDL_ASSERT(parent != NULL);

	ptt = FileItemFromChild(parent);

	if (ptt->lkSibling.next == NULL && ptt->lkSibling.prev == NULL)
		return NULL;

	return &(ptt->lkSibling);
}

link_t_ptr get_file_next_sibling_item(link_t_ptr ilk)
{
	XDL_ASSERT(ilk && ilk->tag == lkFile);

	return get_next_link(ilk);
}

link_t_ptr get_file_prev_sibling_item(link_t_ptr ilk)
{
	XDL_ASSERT(ilk && ilk->tag == lkFile);

	return get_prev_link(ilk);
}

link_t_ptr get_file_child_item_at(link_t_ptr ilk, int index)
{
	link_t_ptr nlk;

	XDL_ASSERT(index >= 0);

	nlk = get_file_first_child_item(ilk);
	while (nlk)
	{
		if (!index)
			break;

		nlk = get_file_next_sibling_item(nlk);
		index--;
	}
	return nlk;
}

int _sorting_file_item_name_proc(link_t_ptr plk1, link_t_ptr plk2, void* param)
{
	file_item_t *ptt1, *ptt2;

	ptt1 = FileItemFromLink(plk1);
	ptt2 = FileItemFromLink(plk2);

	return compare_text(ptt1->info.file_name, -1, ptt2->info.file_name, -1, 0);
}

void sort_file_child_item_by_name(link_t_ptr ilk, int desc)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);
	bubble_sort_link(&ptt->lkChild, (PF_LINKSORT)_sorting_file_item_name_proc, desc, NULL);
}

int _sorting_file_item_time_proc(link_t_ptr plk1, link_t_ptr plk2, void* param)
{
	file_item_t *ptt1, *ptt2;

	ptt1 = FileItemFromLink(plk1);
	ptt2 = FileItemFromLink(plk2);

	return compare_datetime(&ptt1->info.write_time, &ptt2->info.write_time);
}

void sort_file_child_item_by_time(link_t_ptr ilk, int desc)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);
	bubble_sort_link(&ptt->lkChild, (PF_LINKSORT)_sorting_file_item_time_proc, desc, NULL);
}

void set_file_item_info(link_t_ptr ilk, const file_info_t* pfi)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);

	xmem_copy((void*)&ptt->info, (void*)pfi, sizeof(file_info_t));
}

void get_file_item_info(link_t_ptr ilk, file_info_t* pfi)
{
	file_item_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkFile);

	ptt = FileItemFromLink(ilk);

	xmem_copy((void*)pfi, (void*)&ptt->info, sizeof(file_info_t));
}