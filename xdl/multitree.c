/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc multi tree document

	@module	multitree.c | multi tree implement file

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

#include "multitree.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

typedef struct _multi_node_t{
	link_t lkSibling;		// the sibling node link component also as self link component
	link_t lkChild;			// the child node link component 
	int count;				// child node count

	dword_t mask;			// runtime multi node mask
	var_long delta;			// runtime user value

	link_t_ptr attr;		// hash table for node to properties 
}multi_node_t;

/*restore multinode from self link ptr*/
#define MultiNodeFromLink(p) TypePtrFromSibling(multi_node_t,p)
/*restore multinode from child link ptr*/
#define MultiNodeFromChild(p) TypePtrFromChild(multi_node_t,p)

multi_node_t* _alloc_multi_node()
{
	multi_node_t* pti;

	pti = (multi_node_t*)xmem_alloc(sizeof(multi_node_t));
	pti->lkSibling.tag = lkMultiTree;
	pti->lkSibling.next = NULL;
	pti->lkSibling.prev = NULL;
	init_root_link(&pti->lkChild);

	pti->attr = create_hash_table();

	return pti;
}

void _free_multi_node(multi_node_t* ptt)
{
	if (ptt->attr)
		destroy_hash_table(ptt->attr);

	xmem_free(ptt);
}

link_t_ptr create_multi_tree()
{
	multi_node_t* pti;

	pti = _alloc_multi_node();

	return &(pti->lkSibling);
}

void destroy_multi_tree(link_t_ptr ptr)
{
	XDL_ASSERT(is_multi_tree(ptr));

	delete_multi_node(ptr);
}


link_t_ptr enum_multi_node(link_t_ptr ptr,CALLBACK_ENUMLINK pf,void* param)
{
	link_t_ptr nlk;
	link_t_ptr st = NULL;

	if(!pf)
		return NULL;

	nlk = get_multi_first_child_node(ptr);
	while(nlk)
	{
		if(!(*pf)(nlk,param))
			break;

		if(get_multi_first_child_node(nlk))
		{
			if(!st)
				st = create_stack_table();
			push_stack_node(st,(void*)nlk);
			nlk = get_multi_first_child_node(nlk);
			continue;
		}

		while(nlk)
		{
			nlk = get_multi_next_sibling_node(nlk);
			if(nlk)
				break;
			nlk = (st)? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}
	
	if(st)
		destroy_stack_table(st);

	return nlk;
}

link_t_ptr enum_multi_node_deep(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param)
{
	link_t_ptr nlk;
	link_t_ptr st = NULL;
	bool_t b = 1;

	if (!pf)
		return NULL;

	nlk = get_multi_first_child_node(ptr);
	while (b && nlk)
	{
		if (get_multi_first_child_node(nlk))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_multi_first_child_node(nlk);
			continue;
		}

		if (!(*pf)(nlk, param))
			break;

		while (nlk)
		{
			nlk = get_multi_next_sibling_node(nlk);
			if (nlk)
				break;

			nlk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
			if (nlk)
			{
				if (!(*pf)(nlk, param))
				{
					b = 0;
					break;
				}
			}
		}
	}

	if (st)
		destroy_stack_table(st);

	return nlk;
}

bool_t is_multi_tree(link_t_ptr ptr)
{
	multi_node_t* pti;

	if (!ptr)
		return 0;

	if (ptr->tag != lkMultiTree)
		return 0;

	pti = MultiNodeFromLink(ptr);

	return (bool_t)(pti->lkSibling.next == NULL && pti->lkSibling.prev == NULL);
}

static bool_t _compare_multi_node_proc(link_t_ptr plk, void* pv)
{
	return (plk == (link_t_ptr)pv)? 0 : 1;
}

bool_t is_multi_child_node(link_t_ptr ilk,link_t_ptr plk)
{
	if (!plk)
		return 0;

	return (enum_multi_node(ilk, (CALLBACK_ENUMLINK)_compare_multi_node_proc, (void*)plk)) ? 1 : 0;
}

link_t_ptr get_multi_node_attr_table(link_t_ptr ilk)
{
	multi_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	pti = MultiNodeFromLink(ilk);
	return pti->attr;
}

link_t_ptr insert_multi_node_before(link_t_ptr ilk, link_t_ptr pos)
{
	multi_node_t* ptt;
	multi_node_t* pup;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	ptt = _alloc_multi_node();

	pup = MultiNodeFromLink(ilk);
	insert_link_before(&(pup->lkChild), pos, &(ptt->lkSibling));
	pup->count++;

	return &(ptt->lkSibling);
}

link_t_ptr insert_multi_node(link_t_ptr ilk,link_t_ptr pos)
{
	multi_node_t* ptt;
	multi_node_t* pup;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	ptt = _alloc_multi_node();

	pup = MultiNodeFromLink(ilk);
	insert_link_after(&(pup->lkChild),pos,&(ptt->lkSibling));
	pup->count ++;

	return &(ptt->lkSibling);
}

void delete_multi_node(link_t_ptr ilk)
{
	link_t_ptr parent,child;
	multi_node_t* ptt;
	multi_node_t* pup;
	link_t_ptr stack;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	stack = NULL;

	while(ilk)
	{
		child = get_multi_first_child_node(ilk);
		if(child)
		{
			if(!stack)
				stack = create_stack_table();

			push_stack_node(stack,(void*)ilk);

			ilk = child;
			continue;
		}

		parent = get_multi_parent_node(ilk);
		if(parent)
		{
			/* if exist, delete node link from parent node */
			pup = MultiNodeFromLink(parent);
			delete_link(&(pup->lkChild),ilk);
			pup->count --;
		}

		ptt = MultiNodeFromLink(ilk);
		_free_multi_node(ptt);

		ilk = (stack)? (link_t_ptr)pop_stack_node(stack) : NULL;
	}

	if(stack)
		destroy_stack_table(stack);
}

void delete_multi_child_nodes(link_t_ptr ilk)
{
	link_t_ptr plk;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	while(plk = get_multi_first_child_node(ilk))
		delete_multi_node(plk);
}

void set_multi_node_mask(link_t_ptr ilk,dword_t mask)
{
	multi_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	pti = MultiNodeFromLink(ilk);
	pti->mask = mask;
}

dword_t get_multi_node_mask(link_t_ptr ilk)
{
	multi_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	pti = MultiNodeFromLink(ilk);
	return pti->mask;
}

void set_multi_node_mask_check(link_t_ptr ilk,dword_t ul,bool_t b)
{
	multi_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	pti = MultiNodeFromLink(ilk);
	if(b)
		pti->mask |= ul;
	else
		pti->mask &= (~ul);
}

bool_t get_multi_node_mask_check(link_t_ptr ilk,dword_t ul)
{
	multi_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	pti = MultiNodeFromLink(ilk);
	
	return (pti->mask & ul)? 1 : 0;
}

typedef struct _NODE_MASK_COUNT{
	dword_t mask;
	int count;
}NODE_MASK_COUNT;

static bool_t _count_check_node(link_t_ptr plk, void* pv)
{
	NODE_MASK_COUNT* pn = (NODE_MASK_COUNT*)pv;

	if (get_multi_node_mask_check(plk, pn->mask))
		pn->count++;

	return 1;
}

int get_multi_child_node_mask_check_count(link_t_ptr ilk,dword_t ul)
{
	NODE_MASK_COUNT nd = { 0 };

	nd.mask = ul;
	enum_multi_node(ilk, (CALLBACK_ENUMLINK)_count_check_node, (void*)&nd);

	return nd.count;
}

typedef struct _NODE_MASK_CHECK{
	dword_t mask;
	bool_t check;
	int count;
}NODE_MASK_CHECK;

static bool_t _reset_check_node(link_t_ptr plk, void* pv)
{
	NODE_MASK_CHECK* pn = (NODE_MASK_CHECK*)pv;

	if (get_multi_node_mask_check(plk, pn->mask) != pn->check)
		pn->count++;

	set_multi_node_mask_check(plk, pn->mask, pn->check);

	return 1;
}

int set_multi_child_node_mask_check(link_t_ptr ilk,dword_t ul, bool_t b)
{
	NODE_MASK_CHECK nd = { 0 };

	nd.mask = ul;
	nd.check = b;
	enum_multi_node(ilk, (CALLBACK_ENUMLINK)_reset_check_node, (void*)&nd);

	return nd.count;
}

var_long get_multi_node_delta(link_t_ptr ilk)
{
	multi_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	ptt = MultiNodeFromLink(ilk);

	return ptt->delta;
}

void set_multi_node_delta(link_t_ptr ilk,var_long delta)
{
	multi_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	ptt = MultiNodeFromLink(ilk);

	ptt->delta = delta;
}

int get_multi_child_node_count(link_t_ptr ilk)
{
	multi_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	ptt = MultiNodeFromLink(ilk);
	return ptt->count;
}

link_t_ptr get_multi_first_child_node(link_t_ptr ilk)
{
	multi_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	ptt = MultiNodeFromLink(ilk);
	return get_first_link(&ptt->lkChild);
}

link_t_ptr get_multi_last_child_node(link_t_ptr ilk)
{
	multi_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	ptt = MultiNodeFromLink(ilk);
	return get_last_link(&ptt->lkChild);
}

link_t_ptr get_multi_parent_node(link_t_ptr ilk)
{
	multi_node_t* ptt;
	link_t_ptr parent;

	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	if(is_multi_tree(ilk))
		return NULL;

	parent = get_root_link(ilk);

	XDL_ASSERT(parent != NULL);

	ptt = MultiNodeFromChild(parent);
	return &(ptt->lkSibling);
}

link_t_ptr get_multi_next_sibling_node(link_t_ptr ilk)
{
	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	if(is_multi_tree(ilk))
		return NULL;
	else
		return get_next_link(ilk);
}

link_t_ptr get_multi_prev_sibling_node(link_t_ptr ilk)
{
	XDL_ASSERT(ilk && ilk->tag == lkMultiTree);

	if(is_multi_tree(ilk))
		return NULL;
	else
		return get_prev_link(ilk);

}
