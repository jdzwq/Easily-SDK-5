/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc binary tree document

	@module	binatree.c | implement file

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
(at your opbnon) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "binatree.h"
#include "dblink.h"
#include "varobj.h"
#include "stacktable.h"

#include "xdlimp.h"

typedef struct _bina_node_t{
	link_t lkSibling;	/*the self link component linked to parent*/
	link_t lkChild;		/*the child node link root*/
	
	variant_t key;		/*key indicate node sequence*/
	object_t val;		/*node value*/
	byte_t mask;		/*1st for left leaf, 2st for right leaf, 8st for color*/
}bina_node_t;

typedef struct _bina_tree_t{
	link_t lk;		/*the self link component*/

	int level;		/*baleance level*/

	link_t_ptr node; /*root node link*/
}bina_tree_t;

/*restore bina_node_t from self link ptr*/
#define BinaTreeFromLink(p) TypePtrFromLink(bina_tree_t,p)
/*restore bina_node_t from self link ptr*/
#define BinaNodeFromLink(p) TypePtrFromSibling(bina_node_t,p)
/*restore bina_node_t from child link ptr*/
#define BinaNodeFromChild(p) TypePtrFromChild(bina_node_t,p)

#define BINA_LEAF_L	0x01
#define BINA_LEAF_R	0x02

#define BINA_COLOR_B	0x00
#define BINA_COLOR_R	0x80

#define IS_BINA_RED_NODE(mask)		(mask & BINA_COLOR_R)
#define IS_BINA_LEFT_LEAF(mask)		((mask & 0x0F) == BINA_LEAF_L)
#define IS_BINA_RIGHT_LEAF(mask)	((mask & 0x0F) == BINA_LEAF_R)

#define SET_BINA_LEAF(mask, pos)	((mask & 0xF0) | pos)
#define SET_BINA_COLOR(mask, clr)	((mask & 0x0F) | clr)

static bina_node_t* _alloc_bina_node(variant_t key, byte_t clr, byte_t pos)
{
	bina_node_t* pbn;

	pbn = (bina_node_t*)xmem_alloc(sizeof(bina_node_t));
	pbn->lkSibling.tag = lkBinaNode;
	pbn->lkSibling.next = NULL;
	pbn->lkSibling.prev = NULL;
	init_root_link(&pbn->lkChild);

	pbn->mask = (byte_t)pos;
	pbn->mask |= clr;

	pbn->key = variant_alloc(VV_NULL);
	pbn->val = object_alloc(DEF_MBS);

	return pbn;
}

static void _free_bina_node(bina_node_t* pbn)
{
	variant_free(pbn->key);
	object_free(pbn->val);

	xmem_free(pbn);
}

static void _delete_bina_node(link_t_ptr ilk)
{
	link_t_ptr parent, child;
	bina_node_t* ptt;
	bina_node_t* pup;
	link_t_ptr stack = NULL;

	while (ilk)
	{
		child = get_bina_left_child_node(ilk);
		if (!child)
			child = get_bina_right_child_node(ilk);

		if (child)
		{
			if (!stack)
				stack = create_stack_table();

			push_stack_node(stack, (void*)ilk);

			ilk = child;
			continue;
		}

		parent = get_bina_parent_node(ilk);
		if (parent)
		{
			///delete node link from parent node
			pup = BinaNodeFromLink(parent);
			delete_link(&(pup->lkChild), ilk);
		}

		ptt = BinaNodeFromLink(ilk);
		_free_bina_node(ptt);

		ilk = (stack) ? (link_t_ptr)pop_stack_node(stack) : NULL;
	}

	if (stack)
		destroy_stack_table(stack);
}

static bool_t _is_bina_node_root(link_t_ptr ilk)
{
	bina_node_t* pbn;
	bool_t b;

	pbn = BinaNodeFromLink(ilk);

	b = (pbn->mask & 0x0F) ? 0 : 1;
	if (b)
	{
		XDL_ASSERT(get_bina_parent_node(ilk) == NULL);
	}

	return b;
}

static void _set_bina_node_root(link_t_ptr ilk)
{
	bina_node_t* pbn;

	pbn = BinaNodeFromLink(ilk);

	pbn->mask = SET_BINA_LEAF(pbn->mask, 0);
}


static bool_t _is_bina_node_red(link_t_ptr ilk)
{
	bina_node_t* pbn;

	pbn = BinaNodeFromLink(ilk);

	return IS_BINA_RED_NODE(pbn->mask);
}

static void _set_bina_node_red(link_t_ptr ilk, bool_t b)
{
	bina_node_t* pbn;

	pbn = BinaNodeFromLink(ilk);

	pbn->mask = SET_BINA_COLOR(pbn->mask, ((b) ? BINA_COLOR_R : BINA_COLOR_B));
}

static link_t_ptr _delete_bina_left_child_link(link_t_ptr ilk)
{
	link_t_ptr nlk;
	bina_node_t* pbn;
	bina_node_t* ptt;

	pbn = BinaNodeFromLink(ilk);
	nlk = get_first_link(&(pbn->lkChild));
	if (!nlk)
		return NULL;

	ptt = BinaNodeFromLink(nlk);
	if (!IS_BINA_LEFT_LEAF(ptt->mask))
		return NULL;

	ptt->mask = SET_BINA_LEAF(ptt->mask, 0);
	return delete_link(&(pbn->lkChild), nlk);
}

static link_t_ptr _delete_bina_right_child_link(link_t_ptr ilk)
{
	link_t_ptr nlk;
	bina_node_t* pbn;
	bina_node_t* ptt;

	pbn = BinaNodeFromLink(ilk);
	nlk = get_last_link(&(pbn->lkChild));
	if (!nlk)
		return NULL;

	ptt = BinaNodeFromLink(nlk);
	if (!IS_BINA_RIGHT_LEAF(ptt->mask))
		return NULL;

	ptt->mask = SET_BINA_LEAF(ptt->mask, 0);
	return delete_link(&(pbn->lkChild), nlk);
}

static void _insert_bina_left_child_link(link_t_ptr ilk, link_t_ptr plk)
{
	link_t_ptr nlk;
	bina_node_t* pbn;
	bina_node_t* ptt;

	pbn = BinaNodeFromLink(ilk);
	nlk = get_first_link(&(pbn->lkChild));
	
	if (nlk)
	{
		ptt = BinaNodeFromLink(nlk);
		XDL_ASSERT(!IS_BINA_LEFT_LEAF(ptt->mask));
	}

	ptt = BinaNodeFromLink(plk);
	ptt->mask = SET_BINA_LEAF(ptt->mask, BINA_LEAF_L);
	insert_link_before(&(pbn->lkChild), LINK_FIRST, plk);
}

static void _insert_bina_right_child_link(link_t_ptr ilk, link_t_ptr plk)
{
	link_t_ptr nlk;
	bina_node_t* pbn;
	bina_node_t* ptt;

	pbn = BinaNodeFromLink(ilk);
	nlk = get_last_link(&(pbn->lkChild));

	if (nlk)
	{
		ptt = BinaNodeFromLink(nlk);
		XDL_ASSERT(!IS_BINA_RIGHT_LEAF(ptt->mask));
	}

	ptt = BinaNodeFromLink(plk);
	ptt->mask = SET_BINA_LEAF(ptt->mask, BINA_LEAF_R);
	insert_link_after(&(pbn->lkChild), LINK_LAST, plk);
}

static void _rotate_bina_node_left(link_t_ptr ilk)
{
	link_t_ptr plk, rlk, llk;
	bina_node_t* pbn;

	//remove right child from current node
	rlk = _delete_bina_right_child_link(ilk);
	if (!rlk)
		return;

	//remove right sub left child, insert into current right
	llk = _delete_bina_left_child_link(rlk);
	if (llk)
		_insert_bina_right_child_link(ilk, llk);

	plk = get_bina_parent_node(ilk);
	if (plk)
	{
		pbn = BinaNodeFromLink(ilk);

		if (IS_BINA_LEFT_LEAF(pbn->mask))
		{
			_delete_bina_left_child_link(plk);
			_insert_bina_left_child_link(plk, rlk);
		}
		else if (IS_BINA_RIGHT_LEAF(pbn->mask))
		{
			_delete_bina_right_child_link(plk);
			_insert_bina_right_child_link(plk, rlk);
		}
		else
		{
			XDL_ASSERT(0);
		}
	}
	else
	{
		_set_bina_node_root(rlk);
	}

	//current as right child's sub left child node
	_insert_bina_left_child_link(rlk, ilk);
}

static void _rotate_bina_node_right(link_t_ptr ilk)
{
	link_t_ptr plk, rlk, llk;
	bina_node_t* pbn;

	//remove left child from current node
	llk = _delete_bina_left_child_link(ilk);
	if (!llk)
		return;

	//remove left sub right child, insert into current left
	rlk = _delete_bina_right_child_link(llk);
	if (rlk)
		_insert_bina_left_child_link(ilk, rlk);

	plk = get_bina_parent_node(ilk);
	if (plk)
	{
		pbn = BinaNodeFromLink(ilk);

		if (IS_BINA_LEFT_LEAF(pbn->mask))
		{
			_delete_bina_left_child_link(plk);
			_insert_bina_left_child_link(plk, llk);
		}
		else if (IS_BINA_RIGHT_LEAF(pbn->mask))
		{
			_delete_bina_right_child_link(plk);
			_insert_bina_right_child_link(plk, llk);
		}
		else
		{
			XDL_ASSERT(0);
		}
	}
	else
	{
		_set_bina_node_root(llk);
	}

	//current as left child's sub right child node
	_insert_bina_right_child_link(llk, ilk);
}

static void _adjust_bina_node(link_t_ptr ilk, link_t_ptr* proot)
{
	link_t_ptr glk, plk, nlk;

	while (ilk)
	{
		//parent node
		plk = get_bina_parent_node(ilk);
		if (!plk)
		{
			//current node is root
			_set_bina_node_red(ilk, 0);
			_set_bina_node_root(ilk);
			*proot = ilk;
			break;
		}

		if (!_is_bina_node_red(plk))
			break;

		if (get_bina_right_child_node(plk) == ilk)
		{
			//case 1: parent node is read and current is right child node
			//then left rotate parent node
			//final let parent node as current for continue

			//left rotate parent
			_rotate_bina_node_left(plk);

			//now current is upon
			if (_is_bina_node_root(ilk))
			{
				_set_bina_node_red(ilk, 0);
				*proot = ilk;
			}

			//reset current node
			ilk = plk;
			continue;
		}

		//grand father node
		glk = get_bina_parent_node(plk);
		if (glk)
		{
			if (get_bina_right_child_node(glk) == plk)
			{
				//case 2: parent node and parent's brother node is red, 
				//then set parent and parent's brother to black and set grandfather to red
				//final let grandfather as current for continue

				_set_bina_node_red(plk, 0);
				_set_bina_node_red(glk, 1);

				nlk = get_bina_left_child_node(glk);
				if (nlk)
					_set_bina_node_red(nlk, 0);
				
				//reset current node
				ilk = glk;
				continue;
			}
			else
			{
				//case 3: parent node is read and current is left child node
				//then set parent to black and set grandfather to red and right rotate grandfather node
				//final let parent node as current for continue

				_set_bina_node_red(plk, 0);
				_set_bina_node_red(glk, 1);

				//right rotate grandfather
				_rotate_bina_node_right(glk);

				//now parent is upon
				if (_is_bina_node_root(plk))
				{
					_set_bina_node_red(plk, 0);
					*proot = plk;
				}

				//reset current node
				nlk = get_bina_right_child_node(glk);
				if (nlk && _is_bina_node_red(nlk))
				{
					ilk = nlk;
					continue;
				}

				nlk = get_bina_left_child_node(glk);
				if (nlk && _is_bina_node_red(nlk))
				{
					ilk = nlk;
					continue;
				}
			}
			//not need adjust
			break;
		}
		else
		{
			//case 4: parent is root, then set parent to black
			_set_bina_node_red(plk, 0);
			_set_bina_node_root(plk);
			*proot = plk;
		}

		//not need to adjust
		break;
	}
}


/*******************************************************************************************/

link_t_ptr create_bina_tree(int level)
{
	bina_tree_t* pbt;

	pbt = (bina_tree_t*)xmem_alloc(sizeof(bina_tree_t));
	pbt->lk.tag = lkBinaTree;

	pbt->level = level;

	return &(pbt->lk);
}

void destroy_bina_tree(link_t_ptr ptr)
{
	bina_tree_t* pbt;

	XDL_ASSERT(ptr && ptr->tag == lkBinaTree);

	pbt = BinaTreeFromLink(ptr);

	_delete_bina_node(pbt->node);

	xmem_free(pbt);
}

link_t_ptr get_bina_left_child_node(link_t_ptr ilk)
{
	bina_node_t* pbn;
	link_t_ptr nlk;

	XDL_ASSERT(ilk && ilk->tag == lkBinaNode);

	pbn = BinaNodeFromLink(ilk);

	nlk = get_first_link(&pbn->lkChild);
	if (!nlk)
		return NULL;

	pbn = BinaNodeFromLink(nlk);
	return (IS_BINA_LEFT_LEAF(pbn->mask)) ? nlk : NULL;
}

link_t_ptr get_bina_right_child_node(link_t_ptr ilk)
{
	bina_node_t* pbn;
	link_t_ptr nlk;

	XDL_ASSERT(ilk && ilk->tag == lkBinaNode);

	pbn = BinaNodeFromLink(ilk);

	nlk = get_last_link(&pbn->lkChild);
	if (!nlk)
		return NULL;

	pbn = BinaNodeFromLink(nlk);
	return (IS_BINA_RIGHT_LEAF(pbn->mask)) ? nlk : NULL;
}

link_t_ptr get_bina_parent_node(link_t_ptr ilk)
{
	bina_node_t* pbn;
	link_t_ptr root;

	XDL_ASSERT(ilk && ilk->tag == lkBinaNode);

	root = get_root_link(ilk);

	if (!root)
		return NULL;

	pbn = BinaNodeFromChild(root);

	return &(pbn->lkSibling);
}

bool_t is_bina_leaf(link_t_ptr ilk)
{
	return (ilk && !get_bina_left_child_node(ilk) && !get_bina_right_child_node(ilk)) ? 1 : 0;
}

link_t_ptr traver_bina_tree_preorder(link_t_ptr ptr,CALLBACK_ENUMLINK pf,void* param)
{
	bina_tree_t* pbt;

	link_t_ptr nlk,nlk_left,nlk_right;
	bina_node_t* pbn;
	link_t_ptr st = NULL;

	XDL_ASSERT(ptr && ptr->tag == lkBinaTree);

	pbt = BinaTreeFromLink(ptr);

	if(!pf)
		return NULL;

	nlk = pbt->node;
	while(nlk)
	{
		pbn = BinaNodeFromLink(nlk);
		//traverse inorder node
		if (!(*pf)(nlk, param))
			break;

		nlk_left = get_bina_left_child_node(nlk);
		if (nlk_left && !is_bina_leaf(nlk_left))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_bina_left_child_node(nlk);
			continue;
		}

		//traverse preorder leaf
		if (nlk_left && !(*pf)(nlk_left, param))
			break;

		nlk_right = get_bina_right_child_node(nlk);
		if (nlk_right && !is_bina_leaf(nlk_right))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_bina_right_child_node(nlk);
			continue;
		}

		//traverse postorder leaf
		if (nlk_right && !(*pf)(nlk_right, param))
			break;

		while (nlk)
		{
			pbn = BinaNodeFromLink(nlk);

			nlk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
			if (!nlk)
				break;

			if (IS_BINA_LEFT_LEAF(pbn->mask))
			{
				nlk = get_bina_right_child_node(nlk);
				if (nlk)
					break;
			}
		}
	}
	
	if(st)
		destroy_stack_table(st);

	return nlk;
}

link_t_ptr traver_bina_tree_inorder(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param)
{
	bina_tree_t* pbt;

	link_t_ptr nlk, nlk_left, nlk_right;
	bina_node_t* pbn;
	link_t_ptr st = NULL;

	XDL_ASSERT(ptr && ptr->tag == lkBinaTree);

	pbt = BinaTreeFromLink(ptr);

	if (!pf)
		return NULL;

	nlk = pbt->node;
	while (nlk)
	{
		nlk_left = get_bina_left_child_node(nlk);
		if (nlk_left && !is_bina_leaf(nlk_left))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_bina_left_child_node(nlk);
			continue;
		}

		//traverse preorder leaf
		if (nlk_left && !(*pf)(nlk_left, param))
			break;

		//traverse inorder node
		if (!(*pf)(nlk, param))
			break;

		nlk_right = get_bina_right_child_node(nlk);
		if (nlk_right && !is_bina_leaf(nlk_right))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_bina_right_child_node(nlk);
			continue;
		}

		//traverse postorder leaf
		if (nlk_right && !(*pf)(nlk_right, param))
			break;

		while (nlk)
		{
			pbn = BinaNodeFromLink(nlk);

			nlk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
			if (!nlk)
				break;

			if (IS_BINA_LEFT_LEAF(pbn->mask))
			{
				//traverse inorder node
				if (!(*pf)(nlk, param))
					break;

				nlk = get_bina_right_child_node(nlk);
				if (nlk)
					break;
			}
		}
	}

	if (st)
		destroy_stack_table(st);

	return nlk;
}

link_t_ptr traver_bina_tree_postorder(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param)
{
	bina_tree_t* pbt;

	link_t_ptr nlk, nlk_left, nlk_right;
	bina_node_t* pbn;
	link_t_ptr st = NULL;

	XDL_ASSERT(ptr && ptr->tag == lkBinaTree);

	pbt = BinaTreeFromLink(ptr);

	if (!pf)
		return NULL;

	nlk = pbt->node;
	while (nlk)
	{
		nlk_left = get_bina_left_child_node(nlk);
		if (nlk_left && !is_bina_leaf(nlk_left))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_bina_left_child_node(nlk);
			continue;
		}

		//traverse preorder leaf
		if (nlk_left && !(*pf)(nlk_left, param))
			break;

		nlk_right = get_bina_right_child_node(nlk);
		if (nlk_right && !is_bina_leaf(nlk_right))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_bina_right_child_node(nlk);
			continue;
		}

		//traverse postorder leaf
		if (nlk_right && !(*pf)(nlk_right, param))
			break;

		//traverse inorder node
		if (!(*pf)(nlk, param))
			break;

		while (nlk)
		{
			pbn = BinaNodeFromLink(nlk);

			nlk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
			if (!nlk)
				break;

			if (IS_BINA_LEFT_LEAF(pbn->mask))
			{
				if (!st)
					st = create_stack_table();
				push_stack_node(st, (void*)nlk);

				nlk = get_bina_right_child_node(nlk);
				if (nlk)
					break;
			}
			else if (IS_BINA_RIGHT_LEAF(pbn->mask))
			{
				//traverse inorder node
				if (!(*pf)(nlk, param))
					break;
			}
		}
	}

	if (st)
		destroy_stack_table(st);

	return nlk;
}

link_t_ptr traver_bina_tree_levelorder(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param)
{
	bina_tree_t* pbt;
	bina_node_t* pbn;
	link_t_ptr nlk, nlk_left, nlk_right;
	link_t_ptr st = NULL;

	XDL_ASSERT(ptr && ptr->tag == lkBinaTree);

	pbt = BinaTreeFromLink(ptr);

	if (!pf)
		return NULL;

	nlk = pbt->node;

	if (nlk)
	{
		if (!st)
			st = create_stack_table();
		push_stack_node(st, (void*)nlk);
	}
	
	while (1)
	{
		nlk = (st) ? (link_t_ptr)pick_stack_node(st) : NULL;
		if (!nlk)
			break;

		pbn = BinaNodeFromLink(nlk);

		if (!(*pf)(nlk, param))
			break;

		nlk_left = get_bina_left_child_node(nlk);
		if (nlk_left)
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk_left);
		}

		nlk_right = get_bina_right_child_node(nlk);
		if (nlk_right)
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk_right);
		}
	}

	if (st)
		destroy_stack_table(st);

	return nlk;
}

link_t_ptr find_bina_node(link_t_ptr ptr, variant_t key, object_t val)
{
	bina_tree_t* pbt;

	link_t_ptr nlk;
	bina_node_t* pbn;
	int rt;

	XDL_ASSERT(ptr && ptr->tag == lkBinaTree);

	pbt = BinaTreeFromLink(ptr);

	nlk = pbt->node;
	while (nlk)
	{
		pbn = BinaNodeFromLink(nlk);

		rt = variant_comp(key, pbn->key);

		if (!rt)
		{
			if (val)
			{
				object_copy(val, pbn->val);
			}
			break;
		}

		if(rt < 0)
			nlk = get_bina_left_child_node(nlk);
		else
			nlk = get_bina_right_child_node(nlk);
	}

	return nlk;
}

link_t_ptr insert_bina_node(link_t_ptr ptr, variant_t key, object_t val)
{
	bina_tree_t* pbt;

	link_t_ptr nlk, plk;
	bina_node_t* pbn;
	bina_node_t* ptt;
	int rt;

	XDL_ASSERT(ptr && ptr->tag == lkBinaTree);

	pbt = BinaTreeFromLink(ptr);

	if (!pbt->node)
	{
		pbn = _alloc_bina_node(key, BINA_COLOR_B, 0);
		pbt->node = &(pbn->lkSibling);
		return pbt->node;
	}

	plk = nlk = pbt->node;
	while (nlk)
	{
		pbn = BinaNodeFromLink(nlk);

		rt = variant_comp(key, pbn->key);

		if (!rt)
			break;

		if (rt < 0)
		{
			plk = nlk;
			nlk = get_bina_left_child_node(nlk);
		}
		else
		{
			plk = nlk;
			nlk = get_bina_right_child_node(nlk);
		}
	}

	if (!nlk)
	{
		pbn = BinaNodeFromLink(plk);

		rt = variant_comp(key, pbn->key);

		if (rt < 0)
		{
			ptt = _alloc_bina_node(key, BINA_COLOR_R, BINA_LEAF_L);
			nlk = &(ptt->lkSibling);
			insert_link_before(&(pbn->lkChild), LINK_FIRST, nlk);
		}
		else
		{
			ptt = _alloc_bina_node(key, BINA_COLOR_R, BINA_LEAF_R);
			nlk = &(ptt->lkSibling);
			insert_link_after(&(pbn->lkChild), LINK_LAST, nlk);
		}

		if (pbt->level)
		{
			_adjust_bina_node(nlk, &(pbt->node));
		}
	}

	pbn = BinaNodeFromLink(nlk);

	if (val)
		object_copy(pbn->val, val);
	else
		object_empty(pbn->val);
	
	return nlk;
}

void attach_bina_leaf_object(link_t_ptr nlk, object_t val)
{
	bina_node_t* pbn;

	XDL_ASSERT(is_bina_leaf(nlk));

	XDL_ASSERT(val != NULL);

	pbn = BinaNodeFromLink(nlk);

	object_free(pbn->val);

	pbn->val = val;
}

object_t detach_bina_leaf_object(link_t_ptr nlk)
{
	bina_node_t* pbn;
	object_t val;

	XDL_ASSERT(is_bina_leaf(nlk));

	pbn = BinaNodeFromLink(nlk);

	val = pbn->val;
	pbn->val = object_alloc(DEF_MBS);

	return val;
}

bool_t delete_bina_node(link_t_ptr ptr, variant_t key)
{
	bina_tree_t* pbt;

	link_t_ptr plk, llk, rlk, ilk;
	bina_node_t* pbn;

	XDL_ASSERT(ptr && ptr->tag == lkBinaTree);

	pbt = BinaTreeFromLink(ptr);

	ilk = find_bina_node(ptr, key, NULL);
	if (!ilk)
		return 0;

	plk = get_bina_parent_node(ilk);
	if (plk)
	{
		//remove from parent
		pbn = BinaNodeFromLink(plk);
		delete_link(&(pbn->lkChild), ilk);
	}
	//remove left child
	llk = _delete_bina_left_child_link(ilk);
	//remove right child
	rlk = _delete_bina_right_child_link(ilk);

	pbn = BinaNodeFromLink(ilk);
	if (IS_BINA_LEFT_LEAF(pbn->mask))
	{
		if (llk && rlk)
		{
			//left child insert as parent left child
			_insert_bina_left_child_link(plk, llk);

			//right child insert into left child maximized right
			while (get_bina_right_child_node(llk))
			{
				llk = get_bina_right_child_node(llk);
			}
			_insert_bina_right_child_link(llk, rlk);

			if (pbt->level)
			{
				_set_bina_node_red(rlk, 1);
				_adjust_bina_node(rlk, &(pbt->node));
			}
		}
		else if (llk)
		{
			//left child insert as parent left child
			_insert_bina_left_child_link(plk, llk);
		}
		else if (rlk)
		{
			//right child insert as parent left child
			_insert_bina_left_child_link(plk, rlk);
		}
	}
	else if (IS_BINA_RIGHT_LEAF(pbn->mask))
	{
		if (rlk && llk)
		{
			//right child insert as parent right child
			_insert_bina_right_child_link(plk, rlk);

			//left child insert into right child minimized left
			while (get_bina_left_child_node(rlk))
			{
				rlk = get_bina_left_child_node(rlk);
			}
			_insert_bina_left_child_link(rlk, llk);

			if (pbt->level)
			{
				_set_bina_node_red(llk, 1);
				_adjust_bina_node(llk, &(pbt->node));
			}
		}
		else if (rlk)
		{
			//right child insert as parent right child
			_insert_bina_right_child_link(plk, rlk);
		}
		else if (llk)
		{
			//left child insert as parent right child
			_insert_bina_right_child_link(plk, llk);
		}
	}
	else
	{
		if (llk && rlk)
		{
			pbt->node = llk;
			//right child insert into left child maximized right
			while (get_bina_right_child_node(llk))
			{
				llk = get_bina_right_child_node(llk);
			}
			_insert_bina_right_child_link(llk, rlk);

			if (pbt->level)
			{
				_set_bina_node_red(rlk, 1);
				_adjust_bina_node(rlk, &(pbt->node));
			}
		}
		else if (llk)
		{
			pbt->node = llk;
		}
		else if (rlk)
		{
			pbt->node = rlk;
		}
		else
		{
			pbt->node = NULL;
		}
	}

	_free_bina_node(pbn);

	return 1;
}

#if defined(XDL_SUPPORT_TEST)
#include <time.h>

static bool_t print_node(link_t_ptr nlk, void* pa)
{
	bina_node_t* pbn;
	tchar_t buf[1024];

	pbn = BinaNodeFromLink(nlk);

	variant_to_string(pbn->key, buf, 100);

	_tprintf(_T("%s-%c "), buf, (IS_BINA_RED_NODE(pbn->mask) ? _T('R') : _T('B')));

	return 1;
}

static int calc_bina_step(link_t_ptr ilk)
{
	int n = 0;

	while (ilk)
	{
		ilk = get_bina_parent_node(ilk);
		n++;
	}

	return n;
}

/*void test_bina_tree()
{
	link_t_ptr ptr = create_bina_tree(0);

	link_t_ptr nlk;

	variant_t v = { 0 };
	v.vv = VV_INT;

	object_t val = object_alloc();

	v.int_one = 0;
	object_set_variant(val, v);
	nlk = insert_bina_node(ptr, v, val);

	v.int_one = 1;
	object_set_variant(val, v);
	nlk = insert_bina_node(ptr, v, val);

	v.int_one = 3;
	object_set_variant(val, v);
	nlk = insert_bina_node(ptr, v, val);

	v.int_one = 5;
	object_set_variant(val, v);
	nlk = insert_bina_node(ptr, v, val);

	v.int_one = 2;
	object_set_variant(val, v);
	nlk = insert_bina_node(ptr, v, val);

	v.int_one = 4;
	object_set_variant(val, v);
	nlk = insert_bina_node(ptr, v, val);

	v.int_one = 6;
	object_set_variant(val, v);
	nlk = insert_bina_node(ptr, v, val);

	v.int_one = 7;
	object_set_variant(val, v);
	nlk = insert_bina_node(ptr, v, val);

	traver_bina_tree_preorder(ptr, print_node, NULL);

	_tprintf(_T("\n"));

	traver_bina_tree_inorder(ptr, print_node, NULL);

	_tprintf(_T("\n"));

	traver_bina_tree_postorder(ptr, print_node, NULL);

	_tprintf(_T("\n"));

	traver_bina_tree_levelorder(ptr, print_node, NULL);

	destroy_bina_tree(ptr);

	object_free(val);
}*/

void _test_bina_tree(int level)
{
	int sum_0 = 0;
	int sum_1 = 0;
	int count_0 = 0;
	int count_1 = 0;
	float max_0 = 0;
	float max_1 = 0;
	int rnd;

	variant_t key = variant_alloc(VV_INT);

	object_t val = object_alloc(_UTF8);

	int j = (level)? 100 : 1;
	while (j--)
	{
		link_t_ptr ptr_0 = create_bina_tree(0);
		link_t_ptr ptr_1 = create_bina_tree(1);

		Srand48(time(NULL));

		link_t_ptr nlk;
		int total_0 = 0;
		int total_1 = 0;
		float max;
		int n0, n1, n = 1000;

		for (int i = 0; i < n; i++)
		{
			rnd =  (level)? (Lrand48() % n) : i;
			variant_set_int(key, rnd);

			object_set_variant(val, key);

			insert_bina_node(ptr_0, key, val);

			insert_bina_node(ptr_1, key, val);
		}

		n0 = n1 = 0;
		for (int i = 0; i < n; i++)
		{
			variant_set_int(key, i);

			nlk = find_bina_node(ptr_0, key, val);
			if (nlk)
			{
				n0++;
				total_0 += calc_bina_step(nlk);
			}

			nlk = find_bina_node(ptr_1, key, val);
			if (nlk)
			{
				n1++;
				total_1 += calc_bina_step(nlk);
			}
		}

		max = (float)total_0 / n0;
		max_0 = (max_0 < max) ? max : max_0;

		max = (float)total_1 / n1;
		max_1 = (max_1 < max) ? max : max_1;

		_tprintf(_T("nm total step %d, total node %d, aveage step %.4f\n"), total_0, n0, (float)total_0 / n0);
		_tprintf(_T("bd total step %d, total node %d, aveage step %.4f\n"), total_1, n1, (float)total_1 / n1);

		destroy_bina_tree(ptr_0);
		destroy_bina_tree(ptr_1);

		sum_0 += total_0;
		sum_1 += total_1;
		count_0 += n0;
		count_1 += n1;

		//Sleep(100);
	}

	_tprintf(_T("nm total step %d, count node %d, max step %.2f, aveage step %.4f\n"), sum_0, count_0, max_0, (float)sum_0 / count_0);
	_tprintf(_T("bd total step %d, count node %d, max step %.2f, aveage step %.4f\n"), sum_1, count_1, max_1, (float)sum_1 / count_1);

	variant_free(key);
	object_free(val);
}

void test_bina_tree()
{
	_test_bina_tree(0);

	//_test_bina_tree(1);
}

#endif