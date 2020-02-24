/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc stacktable document

	@module	stacktable.c | stacktable implement file

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

#include "stacktable.h"
#include "xdlstd.h"

typedef struct _stack_table_t{
	link_t lk;
	link_t lkChilds;
}stack_table_t;

typedef struct _stack_node_t{
	link_t lk;
	void* data;
}stack_node_t;

#define StackTableFromLink(p)		TypePtrFromLink(stack_table_t,p)
#define StackNodeFromLink(p)		TypePtrFromLink(stack_node_t,p)

link_t_ptr create_stack_table(void)
{
	stack_table_t* pst;

	pst = (stack_table_t*)xmem_alloc(sizeof(stack_table_t));
	pst->lk.tag = lkStackTable;
	pst->lk.next = NULL;
	pst->lk.prev = NULL;
	init_root_link(&pst->lkChilds);

	return &pst->lk;
}

void destroy_stack_table(link_t_ptr ptr)
{
	stack_table_t* pst;

	XDL_ASSERT(ptr && ptr->tag == lkStackTable);

	pst = StackTableFromLink(ptr);

	clear_stack_table(ptr);
	xmem_free(pst);
}

void clear_stack_table(link_t_ptr ptr)
{
	stack_table_t* pst;
	stack_node_t* psi;
	link_t_ptr ilk;

	XDL_ASSERT(ptr && ptr->tag == lkStackTable);

	pst = StackTableFromLink(ptr);
	while(ilk = delete_link(&pst->lkChilds,LINK_FIRST))
	{
		psi = StackNodeFromLink(ilk);
		xmem_free(psi);
	}
}

void push_stack_node(link_t_ptr ptr,void* data)
{
	stack_table_t* pst;
	stack_node_t* psi;

	XDL_ASSERT(ptr && ptr->tag == lkStackTable);

	pst = StackTableFromLink(ptr);
	psi = (stack_node_t*)xmem_alloc(sizeof(stack_node_t));
	psi->lk.tag = lkStackNode;
	psi->lk.next = NULL;
	psi->lk.prev = NULL;
	psi->data = data;
	insert_link_after(&pst->lkChilds,LINK_FIRST,&psi->lk);
}

void* pop_stack_node(link_t_ptr ptr)
{
	void* data;
	stack_table_t* pst;
	stack_node_t* psi;
	link_t_ptr ilk;

	XDL_ASSERT(ptr && ptr->tag == lkStackTable);

	pst = StackTableFromLink(ptr);
	ilk = delete_link(&pst->lkChilds,LINK_FIRST);
	if(!ilk)
		return 0;

	psi = StackNodeFromLink(ilk);
	data = psi->data;
	xmem_free(psi);

	return data;
}

void* peek_stack_node(link_t_ptr ptr)
{
	stack_table_t* pst;
	stack_node_t* psi;
	link_t_ptr ilk;

	XDL_ASSERT(ptr && ptr->tag == lkStackTable);

	pst = StackTableFromLink(ptr);
	ilk = get_first_link(&pst->lkChilds);
	if(!ilk)
		return 0;
	psi = StackNodeFromLink(ilk);
	return psi->data;
}
