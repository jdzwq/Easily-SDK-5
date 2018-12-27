/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc link list document

	@module	dblink.c | link list implement file

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

#include "dblink.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlinit.h"

void init_root_link(link_t_ptr root)
{
	XDL_ASSERT(root != LINK_FIRST && root != LINK_LAST);

	//initialize double link
	root->tag = lkRoot;
	root->next = root;
	root->prev = root;
}

bool_t is_empty_link(link_t_ptr root)
{
	XDL_ASSERT(root && root->tag == lkRoot);

	return (root->prev == root && root == root->next) ? 1 : 0;
}

bool_t is_root_link(link_t_ptr plk)
{
	XDL_ASSERT(plk != LINK_FIRST && plk != LINK_LAST);

	return (plk->tag == lkRoot)? 1 : 0;
}

bool_t is_first_link(link_t_ptr plk)
{
	XDL_ASSERT(plk != LINK_FIRST && plk != LINK_LAST);

	if (plk->tag == lkRoot)
		return 0;

	return (plk->prev->tag == lkRoot) ? 1 : 0;
}

bool_t is_last_link(link_t_ptr plk)
{
	XDL_ASSERT(plk != LINK_FIRST && plk != LINK_LAST);

	if (plk->tag == lkRoot)
		return 0;

	return (plk->next->tag == lkRoot) ? 1 : 0;
}

bool_t is_child_link(link_t_ptr root, link_t_ptr plk)
{
	link_t_ptr nlk;

	XDL_ASSERT(root && root->tag == lkRoot && root != plk);

	nlk = get_first_link(root);
	while (nlk)
	{
		if (nlk == plk)
			return 1;
		nlk = get_next_link(nlk);
	}

	return 0;
}

void insert_link_before(link_t_ptr root,link_t_ptr plk,link_t_ptr pnew)
{
	XDL_ASSERT(root && root->tag == lkRoot && root != plk && pnew && pnew->tag != lkRoot);

	if(plk == LINK_FIRST)
	{
		plk = root->next;	// will insert into first 
	}else if(plk == LINK_LAST)
	{
		plk = root;	// will insert into last 
	}

	XDL_ASSERT(plk != pnew);

	XDL_ASSERT(pnew->next == NULL && pnew->prev == NULL);

	//insert pnew into list before plk
	pnew->next = plk;
	pnew->prev = plk->prev;

	XDL_ASSERT(plk->prev != NULL);

	plk->prev->next = pnew;
	plk->prev = pnew;		
}

void insert_link_after(link_t_ptr root,link_t_ptr plk,link_t_ptr pnew)
{
	XDL_ASSERT(root && root->tag == lkRoot && root != plk && pnew && pnew->tag != lkRoot);

	if(plk == LINK_FIRST)
	{
		plk = root;	// will insert into first 
	}else if(plk == LINK_LAST)
	{
		plk = root->prev;	// will insert into last 
	}

	XDL_ASSERT(plk != pnew);

	XDL_ASSERT(pnew->next == NULL && pnew->prev == NULL);

	//insert pnew into list after plk
	pnew->prev = plk;
	pnew->next = plk->next;

	XDL_ASSERT(plk->next != NULL);

	plk->next->prev = pnew;
	plk->next = pnew;		
}

link_t_ptr delete_link(link_t_ptr root,link_t_ptr plk)
{	
	XDL_ASSERT(root != plk);

	if(plk == LINK_FIRST)
	{
		XDL_ASSERT(root && root->tag == lkRoot);
		if (root->next->tag == lkRoot)
			return NULL;

		plk = root->next;	// delete first link ptr 
	}else if(plk == LINK_LAST)
	{
		XDL_ASSERT(root && root->tag == lkRoot);
		if (root->prev->tag == lkRoot)
			return NULL;

		plk = root->prev;	// delete last link ptr 
	}
	else
	{
		XDL_ASSERT(plk && plk->tag != lkRoot);
	}

	//delete plk from list
	if (plk->prev)
		plk->prev->next = plk->next;
	if (plk->next)
		plk->next->prev = plk->prev;

	plk->next = plk->prev = NULL;
	return plk;
}

link_t_ptr get_link_at(link_t_ptr root,int index)
{
	link_t_ptr cur;

	XDL_ASSERT(root && root->tag == lkRoot && index >= 0);

	cur = root->next;

	while(cur->tag != lkRoot && index--)
		cur = cur->next;

	if(cur->tag == lkRoot)
		return NULL; // empty list
	else
		return cur;
}

void insert_link_at(link_t_ptr root, int index, link_t_ptr pnew)
{
	link_t_ptr plk;

	if (index == 0)
		insert_link_after(root, LINK_FIRST, pnew);
	else if (index < 0)
		insert_link_after(root, LINK_LAST, pnew);
	else
	{
		plk = get_link_at(root, index);
		insert_link_before(root, plk, pnew);
	}
}

link_t_ptr delete_link_at(link_t_ptr root, int index)
{
	link_t_ptr plk;

	plk = get_link_at(root, index);
	return delete_link(root, plk);
}

int get_link_index(link_t_ptr root,link_t_ptr plk)
{
	int count;

	XDL_ASSERT(root && root->tag == lkRoot && root != plk);

	if(plk == LINK_FIRST)
	{
		plk = root->next;	// delete first link ptr 
	}else if(plk == LINK_LAST)
	{
		plk = root->prev;	// delete last link ptr 
	}else
	{
		XDL_ASSERT(plk->tag != lkRoot);
	}

	count = 0;
	while(plk && plk->tag != lkRoot)
	{
		plk = plk->prev;
		count ++;
	}

	return count - 1;
}

link_t_ptr get_first_link(link_t_ptr root)
{
	XDL_ASSERT(root && root->tag == lkRoot);

	if(root->next->tag == lkRoot)
		return NULL;	// empty list 
	else
		return root->next;
}

link_t_ptr get_next_link(link_t_ptr plk)
{
	XDL_ASSERT(plk && plk->tag != lkRoot);

	if (!plk->prev || !plk->next)
		return NULL;

	if(plk->next->tag == lkRoot)
		return NULL;	// plk is tail or list is empty
	else
		return plk->next;
}

link_t_ptr get_prev_link(link_t_ptr plk)
{
	XDL_ASSERT(plk && plk->tag != lkRoot);

	if (!plk->prev || !plk->next)
		return NULL;

	if(plk->prev->tag == lkRoot)
		return NULL;	// plk is head or list is empty
	else
		return plk->prev;
}

link_t_ptr get_last_link(link_t_ptr root)
{
	XDL_ASSERT(root && root->tag == lkRoot);

	if(root->prev->tag == lkRoot)
		return NULL;	// empty list 
	else
		return root->prev;
}

link_t_ptr get_root_link(link_t_ptr plk)
{
	XDL_ASSERT(plk != LINK_FIRST && plk != LINK_LAST);

	if (!plk->prev || !plk->next)
		return NULL;

	while(plk->tag != lkRoot && plk->prev != NULL)
		plk = plk->prev;

	return plk;
}

void move_link(link_t_ptr root, link_t_ptr root_src)
{
	XDL_ASSERT(root && root->tag == lkRoot);
	XDL_ASSERT(root_src && root_src->tag == lkRoot);
	XDL_ASSERT(root->next == root && root == root->prev);

	root_src->next->prev = root;
	root_src->prev->next = root;
	root->next = root_src->next;
	root->prev = root_src->prev;

	root_src->next = root_src->prev = root_src;
}

void switch_link(link_t_ptr root, link_t_ptr plk1,link_t_ptr plk2)
{
	link_t_ptr prev1,prev2;

	XDL_ASSERT(plk1 && plk1->tag != lkRoot && plk2 && plk2->tag != lkRoot);
	
	if (plk1 == plk2)
		return;

	//delete then inster
	if(plk1 == get_prev_link(plk2))
	{
		delete_link(root,plk1);
		insert_link_after(root,plk2,plk1);
		return;
	}else if(plk2 == get_prev_link(plk1))
	{
		delete_link(root,plk2);
		insert_link_after(root,plk1,plk2);
		return;
	}

	prev1 = get_prev_link(plk1);
	prev2 = get_prev_link(plk2);

	delete_link(root,plk2);
	if(prev1 == NULL)
		insert_link_after(root,LINK_FIRST,plk2);
	else
		insert_link_after(root,prev1,plk2);

	delete_link(root,plk1);
	if(prev2 == NULL)
		insert_link_after(root,LINK_FIRST,plk1);
	else
		insert_link_after(root,prev2,plk1);
}

void switch_link_next(link_t_ptr root, link_t_ptr plk)
{
	link_t_ptr next;

	XDL_ASSERT(plk && plk->tag != lkRoot);

	XDL_ASSERT(root != NULL);

	next = get_next_link(plk);
	delete_link(root,plk);

	if(next == NULL)
		insert_link_after(root,LINK_LAST,plk);
	else
		insert_link_after(root,next,plk);
}

void switch_link_prev(link_t_ptr root, link_t_ptr plk)
{
	link_t_ptr prev;

	XDL_ASSERT(plk && plk->tag != lkRoot);

	XDL_ASSERT(root != NULL);

	prev = get_prev_link(plk);
	if(prev)
		prev = get_prev_link(prev);

	delete_link(root,plk);

	if(prev == NULL)
		insert_link_after(root,LINK_FIRST,plk);
	else
		insert_link_after(root,prev,plk);
}

void switch_link_last(link_t_ptr root, link_t_ptr plk)
{
	XDL_ASSERT(plk && plk->tag != lkRoot);

	XDL_ASSERT(root != NULL);

	delete_link(root,plk);
	insert_link_after(root,LINK_LAST,plk);
}

void switch_link_first(link_t_ptr root, link_t_ptr plk)
{
	XDL_ASSERT(plk && plk->tag != lkRoot);

	XDL_ASSERT(root != NULL);

	delete_link(root,plk);
	insert_link_after(root,LINK_FIRST,plk);
}

void switch_link_after(link_t_ptr root, link_t_ptr prev,link_t_ptr plk)
{
	XDL_ASSERT(plk && plk->tag != lkRoot);

	if (prev == plk)
		return;

	XDL_ASSERT(root != NULL);

	delete_link(root,plk);
	insert_link_after(root,prev,plk);
}

void switch_link_before(link_t_ptr root, link_t_ptr next,link_t_ptr plk)
{
	XDL_ASSERT(plk && plk->tag != lkRoot);

	if (next == plk)
		return;

	XDL_ASSERT(root != NULL);

	delete_link(root,plk);
	insert_link_before(root,next,plk);
}

int get_link_count(link_t_ptr root)
{
	link_t_ptr plk;
	int count = 0;

	XDL_ASSERT(root && root->tag == lkRoot);

	plk = get_first_link(root);
	while(plk)
	{
		count ++;
		plk = get_next_link(plk);
	}

	return count;
}

void push_link(link_t_ptr root,link_t_ptr ptr)
{
	XDL_ASSERT(root && root->tag == lkRoot && ptr && ptr->tag != lkRoot);

	insert_link_after(root,LINK_FIRST,ptr);
}

link_t_ptr pop_link(link_t_ptr root)
{
	XDL_ASSERT(root && root->tag == lkRoot);

	return delete_link(root,LINK_FIRST);
}

link_t_ptr peek_link(link_t_ptr root)
{
	XDL_ASSERT(root && root->tag == lkRoot);

	return get_first_link(root);
}

void bubble_sort_link(link_t_ptr root,CALLBACK_SORTLINK pf,int desc,void* parm)
{
	link_t_ptr prev,next;
	int tag = 1;
	int rt;

	XDL_ASSERT(root && root->tag == lkRoot);

	if (!pf)
		return;

	while(tag)
	{
		tag = 0;
		prev = get_first_link(root);
		while(prev)
		{
			next = get_next_link(prev);
			if(next == NULL)
				break;

			rt = (*pf)(prev,next,parm);
			if((rt > 0 && !desc) || (rt < 0 && desc))
			{
				switch_link_next(root,prev);
				tag = 1;
			}else
				prev = next;
		}
	}
}

int _div_link(link_t_ptr root1,link_t_ptr root2)
{
	int count,tag;

	tag = get_link_count(root1);
	count = tag / 2;
	while(count--)
	{
		insert_link_after(root2,LINK_FIRST,delete_link(root1,LINK_LAST));
	}
	return tag;
}

void _mrg_link(link_t_ptr root1,link_t_ptr root2,CALLBACK_SORTLINK pf,int desc,void* parm)
{
	link_t_ptr next1,next2;
	link_t_ptr plk;
	int rt;

	next1 = get_first_link(root1);
	next2 = get_first_link(root2);
	
	while(next1 != NULL && next2 != NULL)
	{
		rt = (*pf)(next1,next2,parm);
		if((rt > 0 && !desc) || (rt < 0 && desc))
		{
			plk = get_next_link(next2);
			insert_link_before(root1,next1,delete_link(root2,next2));
			next2 = plk;
		}else
		{
			next1 = get_next_link(next1);
		}
	}

	while(next2 != NULL)
	{
		plk = get_next_link(next2);
		insert_link_after(root1,LINK_LAST,delete_link(root2,next2));
		next2 = plk;
	}
	
}

void merge_sort_link(link_t_ptr root,CALLBACK_SORTLINK pf,int desc,void* parm)
{
	link_t lk;
	
	init_root_link(&lk);

	if(_div_link(root,&lk) > 1)
	{
		merge_sort_link(root,pf,desc,parm);
		merge_sort_link(&lk,pf,desc,parm);
		_mrg_link(root,&lk,pf,desc,parm);
	}
}

void _adjust_insert(link_t_ptr* pa, int n, CALLBACK_SORTLINK pf, void* parm)
{
	while(n > 1 && (*pf)(pa[0],pa[n/2],parm) > 0)
	{
		pa[n] = pa[n/2];

		n /= 2;
	}
	pa[n] = pa[0];
}

void _adjust_delete(link_t_ptr* pa,int n,CALLBACK_SORTLINK pf,void* parm)
{
	int i;

	i = 2;
	while(i <= n)
	{
		if(i < n && (*pf)(pa[i],pa[i+1],parm) < 0)
			i ++;
		if((*pf)(pa[0],pa[i],parm) >= 0)
			break;
		pa[i/2] = pa[i];
		i *= 2;
	}
	pa[i/2] = pa[0];
}

void heap_sort_link(link_t_ptr root,CALLBACK_SORTLINK pf,int desc,void* parm)
{
	link_t_ptr* pa;
	int count,i;
	link_t_ptr plk;

	count = get_link_count(root);
	if(count < 2)
		return;

	//alloc heap for sorting link  nodes
	pa = (link_t_ptr*)xmem_alloc((count + 1) * sizeof(link_t_ptr));
	
	//insert into heap
	i = 1;
	while(plk = delete_link(root,LINK_FIRST))
	{
		pa[0] = plk;
		_adjust_insert(pa,i++,pf,parm);
	}

	//delete from heap,then order insert into link list
	while(count)
	{
		plk = pa[1];
		insert_link_after(root,((desc)? LINK_LAST : LINK_FIRST),plk);

		pa[0] = pa[count--];
		_adjust_delete(pa,count,pf,parm);
	}

	//free heap
	xmem_free(pa);
}

int _part_sort_link(link_t_ptr* pa,int i,int j,CALLBACK_SORTLINK pf,void* parm)
{
	link_t_ptr base,tmp;
	int n;

	if(i == j)
		return 0;

	n = i++;
	base = pa[n];
	do{
		while(pa[i] != LINK_LAST && (*pf)(pa[i],base,parm) < 0)
			i++;

		while(pa[j] != LINK_FIRST && (*pf)(pa[j],base,parm) > 0)
			j--;

		if(i < j)
		{
			tmp = pa[i];
			pa[i] = pa[j];
			pa[j] = tmp;
		}
	}while(i < j);

	pa[n]= pa[j];
	pa[j] = base;

	return j;
}

void _quick_sort_link(link_t_ptr* pa, int i, int j, CALLBACK_SORTLINK pf, void* parm)
{
	int n;

	n = _part_sort_link(pa,i,j,pf,parm);
	if(n>i)
		_quick_sort_link(pa,i,n-1,pf,parm);
	if(n && n<j)
		_quick_sort_link(pa,n+1,j,pf,parm);
}

void quick_sort_link(link_t_ptr root, CALLBACK_SORTLINK pf, int desc, void* parm)
{
	link_t_ptr* pa;
	int count,i;

	//alloc array for storing link point
	count = get_link_count(root);
	pa = (link_t_ptr*)xmem_alloc((count + 2) * sizeof(link_t_ptr));
	pa[0] = LINK_FIRST;

	for(i=1;i<=count;i++)
		pa[i] = delete_link(root,LINK_FIRST);

	pa[count + 1] = LINK_LAST;
	//sort array
	_quick_sort_link(pa,1,count,pf,parm);
	
	//reinsert into link list
	for(i=1;i<=count;i++)
		insert_link_after(root,((desc)? LINK_FIRST : LINK_LAST),pa[i]);

	xmem_free(pa);
}

void enum_link(link_t_ptr root,CALLBACK_ENUMLINK pf,void* parm)
{
	link_t_ptr plk;

	if (!pf)
		return;

	plk = get_first_link(root);
	while(plk)
	{
		if(!(*pf)(plk,parm))
			break;

		plk = get_next_link(plk);
	}
}

