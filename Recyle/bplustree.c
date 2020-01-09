/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc B+ tree document

	@module	bplustree.c | bplus tree implement file

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

#include "bplustree.h"
#include "xdlstd.h"
#include "xdlinit.h"

#define KEYBLK_SIZE		16

#define BPLUS_ENTITY_INDEX	1
#define BPLUS_ENTITY_DATA	2

static byte_t BPLUS_NULL_KEY[KEYBLK_SIZE] = { 0 };

#define IS_NULL_KEY(key)	((xmem_comp(key, KEYBLK_SIZE, BPLUS_NULL_KEY, KEYBLK_SIZE) == 0)? 1 : 0)

typedef struct _bplus_index_entity{
	integer_t ind;	//children file table index
	byte_t key[KEYBLK_SIZE]; //key token
#ifdef _DEBUG
	byte_t rev[796];
#endif
}bplus_index_entity;

typedef struct _bplus_index_t{
	link_t lkSibling;		// the sibling node link component also as self link component
	link_t lkChild;			// the child node link component 

	integer_t index;			//file table index
	int count;			//entities count
	bplus_index_entity* indices; //array for index keyset
}bplus_index_t;

typedef struct _bplus_data_entity{
	integer_t off;
	byte_t key[KEYBLK_SIZE];
#ifdef _DEBUG
	byte_t rev[796];
#endif
}bplus_data_entity;

typedef struct _bplus_data_t{
	link_t lkSibling;		// the sibling node link component also as self link component
	link_t lkChild;			// the child node link component 

	integer_t index;		//file table index
	int count;			//entities count
	bplus_data_entity* dataset;
}bplus_data_t;

typedef struct _bplus_tree_t{
	link_t lk;			// the self link component
	link_t lkIndices;	//index set
	link_t lkDataSet;	//data set

	link_t_ptr ft;		//file table
}bplus_tree_t;

/*restore bplus tree from self link ptr*/
#define BplusTreeFromLink(p) TypePtrFromLink(bplus_tree_t,p)
/*restore bplus index from self link ptr*/
#define BplusIndexFromLink(p) TypePtrFromSibling(bplus_index_t,p)
/*restore bplus index from child link ptr*/
#define BplusIndexFromChild(p) TypePtrFromChild(bplus_index_t,p)
/*restore bplus data from self link ptr*/
#define BplusDataFromLink(p) TypePtrFromSibling(bplus_data_t,p)
/*restore bplus data from child link ptr*/
#define BplusDataFromChild(p) TypePtrFromChild(bplus_data_t,p)

static int _insert_bplus_index(link_t_ptr ptr, link_t_ptr plk, link_t_ptr ilk, byte_t key[KEYBLK_SIZE]);
static link_t_ptr _split_bplus_index(link_t_ptr ptr, link_t_ptr ilk, byte_t key[KEYBLK_SIZE]);
static link_t_ptr _split_bplus_data(link_t_ptr ptr, link_t_ptr nlk, byte_t key[KEYBLK_SIZE]);
static link_t_ptr _insert_bplus_data(link_t_ptr ptr, link_t_ptr nlk, byte_t key[KEYBLK_SIZE]);
static bool_t _shift_bplus_data(link_t_ptr ptr, link_t_ptr nlk);

/*static void _key_gen(varval_t* pkey, unsigned char out[16])
{
	unsigned char keyblk[256];
	unsigned char padding[48];
	md5_context md5;
	int len;

	len = varval_to_utf8(pkey, keyblk, 256);
	
	xmem_set(padding, 0x36, 48);

	md5_starts(&md5);
	md5_update(&md5, keyblk, len);
	md5_update(&md5, padding, 48);
	md5_finish(&md5, out);
}*/

static void _key_gen(varval_t* pkey, unsigned char out[16])
{
	a_xsprintf((schar_t*)out, "%04d", pkey->long_one);
}

static bplus_index_t* _new_bplus_index()
{
	bplus_index_t* pbi;

	pbi = (bplus_index_t*)xmem_alloc(sizeof(bplus_index_t));
	pbi->lkSibling.tag = lkBplusIndex;
	pbi->lkSibling.next = NULL;
	pbi->lkSibling.prev = NULL;
	init_root_link(&pbi->lkChild);

	return pbi;
}

static bplus_index_t* _alloc_bplus_index(bplus_tree_t* pbt)
{
	bplus_index_t* pbi;
	int pos = 0;

	if (pbt->ft)
	{
		pos = alloc_file_table_block_index(pbt->ft, -1);
		if (pos == C_ERR)
			return NULL;

		set_file_table_block_tag(pbt->ft, pos, BPLUS_ENTITY_INDEX);
	}

	pbi = _new_bplus_index();

	if (!pbt->ft)
	{
		pbi->indices = (bplus_index_entity*)page_alloc(PAGE_SIZE);
		pbi->index = (integer_t)pbi->indices;
	}
	else
	{
		pbi->indices = (bplus_index_entity*)alloc_file_table_block_page(pbt->ft);
		pbi->index = pos;
	}

	return pbi;
}

static bplus_data_t* _new_bplus_data()
{
	bplus_data_t* pbn;

	pbn = (bplus_data_t*)xmem_alloc(sizeof(bplus_data_t));
	pbn->lkSibling.tag = lkBplusData;
	pbn->lkSibling.next = NULL;
	pbn->lkSibling.prev = NULL;
	pbn->lkChild.tag = lkBplusChild;

	return pbn;
}

static bplus_data_t* _alloc_bplus_data(bplus_tree_t* pbt)
{
	bplus_data_t* pbn;
	int pos = 0;

	if (pbt->ft)
	{
		pos = alloc_file_table_block_index(pbt->ft, -1);
		if (pos == C_ERR)
			return NULL;

		set_file_table_block_tag(pbt->ft, pos, BPLUS_ENTITY_DATA);
	}

	pbn = _new_bplus_data();

	if (!pbt->ft)
	{
		pbn->dataset = (bplus_data_entity*)page_alloc(PAGE_SIZE);
		pbn->index = (integer_t)pbn->dataset;
	}
	else
	{
		pbn->dataset = (bplus_data_entity*)alloc_file_table_block_page(pbt->ft);;
		pbn->index = pos;
	}

	return pbn;
}

static bool_t _attach_bplus_index(bplus_tree_t* pbt, bplus_index_t* pbi)
{
	bplus_index_t* pbn;
	bplus_data_t* pbd;
	int max;

	XDL_ASSERT(pbt->ft != NULL);

	if (pbi->indices)
		return 1;

	if (!get_file_table_block_alloced(pbt->ft, pbi->index) || get_file_table_block_tag(pbt->ft, pbi->index) != BPLUS_ENTITY_INDEX)
		return 0;

	pbi->indices = (bplus_index_entity*)alloc_file_table_block_page(pbt->ft);

	if (!read_file_table_block(pbt->ft, pbi->index, (byte_t*)pbi->indices))
	{
		free_file_table_block_page(pbt->ft, pbi->indices);
		pbi->indices = NULL;
		return 0;
	}

	max = page_size(pbi->indices) / sizeof(bplus_index_entity);
	pbi->count = 0;
	while (pbi->count < max)
	{
		if (IS_NULL_KEY(pbi->indices[pbi->count].key) && pbi->indices[pbi->count].ind == 0)
			break;

		if (get_file_table_block_tag(pbt->ft, pbi->indices[pbi->count].ind) == BPLUS_ENTITY_INDEX)
		{
			pbn = _new_bplus_index(pbt);
			pbn->index = pbi->indices[pbi->count].ind;

			insert_link_after(&(pbi->lkChild), LINK_LAST, &(pbn->lkSibling));
		}
		else
		{
			pbd = _new_bplus_data(pbt);
			pbd->index = pbi->indices[pbi->count].ind;

			insert_link_after(&(pbi->lkChild), LINK_LAST, &(pbd->lkChild));
		}

		pbi->count++;
	}

	return 1;
}

static bool_t _attach_bplus_data(bplus_tree_t* pbt, bplus_data_t* pbn)
{
	int max;

	XDL_ASSERT(pbt->ft != NULL);

	if (pbn->dataset)
		return 1;

	if (!get_file_table_block_alloced(pbt->ft, pbn->index) || get_file_table_block_tag(pbt->ft, pbn->index) != BPLUS_ENTITY_DATA)
		return 0;

	pbn->dataset = (bplus_data_entity*)alloc_file_table_block_page(pbt->ft);

	if (!read_file_table_block(pbt->ft, pbn->index, (byte_t*)pbn->dataset))
	{
		free_file_table_block_page(pbt->ft, pbn->dataset);
		pbn->dataset = NULL;
		return 0;
	}

	max = page_size(pbn->dataset) / sizeof(bplus_data_entity);
	pbn->count = 0;
	while (pbn->count < max)
	{
		if (IS_NULL_KEY(pbn->dataset[pbn->count].key))
			break;

		pbn->count++;
	}

	return 1;
}

static bool_t _update_bplus_index(bplus_tree_t* pbt, bplus_index_t* pbi, bool_t b_save)
{
	int max;

	if (!pbi->indices)
		return 0;

	if (b_save)
	{
		max = page_size(pbi->indices) / sizeof(bplus_index_entity);
		xmem_zero((byte_t*)pbi->indices + pbi->count * sizeof(bplus_index_entity), (max - pbi->count)* sizeof(bplus_index_entity));

		return write_file_table_block(pbt->ft, pbi->index, (byte_t*)pbi->indices);
	}
	else
	{
		return read_file_table_block(pbt->ft, pbi->index, (byte_t*)pbi->indices);
	}
}

static bool_t _update_bplus_data(bplus_tree_t* pbt, bplus_data_t* pbd, bool_t b_save)
{
	int max;

	if (!pbd->dataset)
		return 0;

	if (b_save)
	{
		max = page_size(pbd->dataset) / sizeof(bplus_data_entity);
		xmem_zero((byte_t*)pbd->dataset + pbd->count * sizeof(bplus_data_entity), (max - pbd->count)* sizeof(bplus_data_entity));

		return write_file_table_block(pbt->ft, pbd->index, (byte_t*)pbd->dataset);
	}
	else
	{
		return read_file_table_block(pbt->ft, pbd->index, (byte_t*)pbd->dataset);
	}

	return 1;
}

static void _free_bplus_index(bplus_tree_t* pbt, bplus_index_t* pbi)
{
	if (pbt->ft)
	{
		free_file_table_block_index(pbt->ft, pbi->index);
		free_file_table_block_page(pbt->ft, pbi->indices);
	}
	else
	{
		page_free(pbi->indices);
	}

	xmem_free(pbi);
}

static void _free_bplus_data(bplus_tree_t* pbt, bplus_data_t* pbd)
{
	if (pbt->ft)
	{
		free_file_table_block_index(pbt->ft, pbd->index);
		free_file_table_block_page(pbt->ft, pbd->dataset);
	}
	else
	{
		page_free(pbd->dataset);
	}

	xmem_free(pbd);
}

static link_t_ptr _get_bplus_index_first_child(link_t_ptr ilk)
{
	bplus_index_t* pbn;

	pbn = BplusIndexFromLink(ilk);
	return get_first_link(&pbn->lkChild);
}

static link_t_ptr _get_bplus_index_last_child(link_t_ptr ilk)
{
	bplus_index_t* pbn;

	pbn = BplusIndexFromLink(ilk);
	return get_last_link(&pbn->lkChild);
}

static link_t_ptr _get_bplus_index_next_sibling(link_t_ptr ilk)
{
	return get_next_link(ilk);
}

static link_t_ptr _get_bplus_index_prev_sibling(link_t_ptr ilk)
{
	return get_prev_link(ilk);
}

static link_t_ptr _get_bplus_index_parent(link_t_ptr ilk)
{
	bplus_index_t* pbn;
	link_t_ptr parent;

	XDL_ASSERT(ilk->tag == lkBplusIndex);

	parent = get_root_link(ilk);
	if (!parent)
		return NULL;

	pbn = BplusIndexFromChild(parent);

	return (pbn->lkSibling.tag == lkBplusIndex) ? &(pbn->lkSibling) : NULL;
}

static link_t_ptr _get_bplus_data_parent(link_t_ptr nlk)
{
	bplus_index_t* pbn;
	bplus_data_t* pbd;
	link_t_ptr ilk, parent;

	XDL_ASSERT(nlk->tag == lkBplusData);

	pbd = BplusDataFromLink(nlk);
	ilk = &(pbd->lkChild);

	parent = get_root_link(ilk);
	if (!parent)
		return NULL;

	pbn = BplusIndexFromChild(parent);

	return (pbn->lkSibling.tag == lkBplusIndex) ? &(pbn->lkSibling) : NULL;
}

static link_t_ptr _split_bplus_index(link_t_ptr ptr, link_t_ptr ilk, byte_t key[KEYBLK_SIZE])
{
	bplus_tree_t* pbt;
	bplus_index_t* pnew;
	bplus_index_t* pbi;
	bplus_index_t* pbn;
	bplus_data_t* pbd;

	link_t_ptr plk, llk;

	byte_t org[KEYBLK_SIZE];
	int n, max;

	pbt = BplusTreeFromLink(ptr);

	pbi = BplusIndexFromLink(ilk);

	//split at middle position
	max = page_size(pbi->indices) / sizeof(bplus_index_entity);

	llk = _get_bplus_index_first_child(ilk);
	n = 0;
	while (n < max / 2 && llk)
	{
		llk = _get_bplus_index_next_sibling(llk);
		n++;
	}
#ifdef _DEBUG
	XDL_ASSERT(llk != NULL);
#endif

	xmem_copy(org, pbi->indices[n].key, KEYBLK_SIZE);

	//alloc new index entity for holding left side keys
	pnew = _alloc_bplus_index(pbt);
	if (!pnew)
		return NULL;

	//remove middle key from original entity and insert into new entity
	if (llk->tag == lkBplusChild)
	{
		pbd = BplusDataFromChild(llk);
		llk = _get_bplus_index_prev_sibling(llk);
#ifdef _DEBUG
		XDL_ASSERT(ilk == _get_bplus_data_parent(&(pbd->lkSibling)));
#endif
		delete_link(&(pbi->lkChild), &(pbd->lkChild));
		//must insert at last
		pnew->indices[n].ind = pbd->index;
		insert_link_after(&(pnew->lkChild), LINK_LAST, &(pbd->lkChild));
		pnew->count = 1;
	}
	else
	{
		pbn = BplusIndexFromLink(llk);
		llk = _get_bplus_index_prev_sibling(llk);
#ifdef _DEBUG
		XDL_ASSERT(ilk == _get_bplus_index_parent(&(pbn->lkSibling)));
#endif
		delete_link(&(pbi->lkChild), &(pbn->lkSibling));
		//must insert at last
		pnew->indices[n].ind = pbn->index;
		insert_link_after(&(pnew->lkChild), LINK_LAST, &(pbn->lkSibling));
		pnew->count = 1;
	}

	while (n--)
	{
		xmem_copy(pnew->indices[n].key, pbi->indices[n].key, KEYBLK_SIZE);
		pnew->indices[n].ind = pbi->indices[n].ind;
		pnew->count++;

		//reset child entites parent
		if (llk->tag == lkBplusChild)
		{
			pbd = BplusDataFromChild(llk);
			llk = _get_bplus_index_prev_sibling(llk);
#ifdef _DEBUG
			XDL_ASSERT(ilk == _get_bplus_data_parent(&(pbd->lkSibling)));
#endif
			delete_link(&(pbi->lkChild), &(pbd->lkChild));
			insert_link_before(&(pnew->lkChild), LINK_FIRST, &(pbd->lkChild));
		}
		else
		{
			pbn = BplusIndexFromLink(llk);
			llk = _get_bplus_index_prev_sibling(llk);
#ifdef _DEBUG
			XDL_ASSERT(ilk == _get_bplus_index_parent(&(pbn->lkSibling)));
#endif
			delete_link(&(pbi->lkChild), &(pbn->lkSibling));
			insert_link_before(&(pnew->lkChild), LINK_FIRST, &(pbn->lkSibling));
		}
	}

	if (pbt->ft)
	{
		_update_bplus_index(pbt, pnew, 1);
	}

	//remove the middle position key, and rearrange right side keys
	pbi->count = 0;
	n = max / 2 + 1;
	while (n < max)
	{
		xmem_copy(pbi->indices[pbi->count].key, pbi->indices[n].key, KEYBLK_SIZE);
		pbi->indices[pbi->count].ind = pbi->indices[n].ind;
		pbi->count++;
		n++;
	}

	if (pbt->ft)
	{
		_update_bplus_index(pbt, pbi, 1);
	}

	//the middle position key insert into parent index entity
	plk = _get_bplus_index_parent(ilk);
	if (!plk)
	{
		//nlk is root index entity, then remove first
		delete_link(&(pbt->lkIndices), ilk);

		//alloc new root index entity
		pbn = _alloc_bplus_index(pbt);
		XDL_ASSERT(pbn != NULL);

		if (pbt->ft)
		{
			set_file_table_root(pbt->ft, pbn->index);
		}

		plk = &(pbn->lkSibling);
		insert_link_after(&(pbt->lkIndices), LINK_LAST, plk);

		//new index entity insert to new parent
		n = _insert_bplus_index(ptr, plk, &(pnew->lkSibling), org);
		XDL_ASSERT(n != C_ERR);

		//org nlk insert back to new parent
		insert_link_after(&(pbn->lkChild), &(pnew->lkSibling), &(pbi->lkSibling));
		xmem_zero(pbn->indices[n + 1].key, KEYBLK_SIZE);
		pbn->indices[n + 1].ind = pbi->index;
		pbn->count++;

		if (pbt->ft)
		{
			_update_bplus_index(pbt, pbn, 1);
		}
	}
	else
	{
		//new index entity insert to org parent
		_insert_bplus_index(ptr, plk, &(pnew->lkSibling), org);
	}

	return (xmem_comp(key, KEYBLK_SIZE, org, KEYBLK_SIZE) < 0) ? &(pnew->lkSibling) : ilk;
}

static int _insert_bplus_index(link_t_ptr ptr, link_t_ptr plk, link_t_ptr ilk, byte_t key[KEYBLK_SIZE])
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_index_t* pbn;
	bplus_data_t* pbd;
	int n, max;
	link_t_ptr llk;

	pbt = BplusTreeFromLink(ptr);

	pbi = BplusIndexFromLink(plk);

	max = page_size(pbi->indices) / sizeof(bplus_index_entity);

	//split full index entity, and return the new or org index for key to insert
	if (pbi->count == max)
	{
		plk = _split_bplus_index(ptr, plk, key);
		if (!plk)
			return C_ERR;
	}

	//add the key to index entity
	pbi = BplusIndexFromLink(plk);
	llk = get_last_link(&(pbi->lkChild));
	n = pbi->count;
	while (n)
	{
		if (!IS_NULL_KEY(pbi->indices[n - 1].key) && xmem_comp(pbi->indices[n - 1].key, KEYBLK_SIZE, key, KEYBLK_SIZE) <= 0)
			break;

		xmem_copy(pbi->indices[n].key, pbi->indices[n - 1].key, KEYBLK_SIZE);
		pbi->indices[n].ind = pbi->indices[n - 1].ind;
		n--;

		llk = get_prev_link(llk);
	}

	xmem_copy(pbi->indices[n].key, key, KEYBLK_SIZE);
	if (ilk->tag == lkBplusChild)
	{
		pbd = BplusDataFromChild(ilk);
		pbi->indices[n].ind = pbd->index;
	}
	else
	{
		pbn = BplusIndexFromLink(ilk);
		pbi->indices[n].ind = pbn->index;
	}
	pbi->count++;
	insert_link_after(&(pbi->lkChild), ((llk) ? llk : LINK_FIRST), ilk);

	if (pbt->ft)
	{
		_update_bplus_index(pbt, pbi, 1);
	}

	return n;
}

static void _delete_bplus_index(link_t_ptr ptr, link_t_ptr ilk)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_index_t* pbp;

	link_t_ptr plk;
	int n;

	pbt = BplusTreeFromLink(ptr);

	pbi = BplusIndexFromLink(ilk);

	plk = _get_bplus_index_parent(ilk);
	if (plk)
	{
		pbp = BplusIndexFromLink(plk);
		n = pbp->count;

		ilk = _get_bplus_index_last_child(plk);
		while (ilk)
		{
			n--;
			if (ilk == &(pbi->lkSibling))
				break;

			ilk = _get_bplus_index_prev_sibling(ilk);
		}

#ifdef _DEBUG
		if (n < pbp->count)
		{
			XDL_ASSERT(pbp->indices[n].ind == pbi->index);
		}
#endif
		delete_link(&(pbp->lkChild), ilk);
		pbp->count--;

		while (n < pbp->count)
		{
			xmem_copy(pbp->indices[n].key, pbp->indices[n + 1].key, KEYBLK_SIZE);
			pbp->indices[n].ind = pbp->indices[n + 1].ind;
			n++;
		}

		if (pbt->ft)
		{
			_update_bplus_index(pbt, pbp, 1);
		}

		if (!pbp->count)
		{
			_delete_bplus_index(ptr, plk);
		}
	}
	else
	{
		delete_link(&(pbt->lkIndices), ilk);
	}

	_free_bplus_index(pbt, pbi);
}

static link_t_ptr _find_bplus_entity(link_t_ptr ptr, byte_t key[KEYBLK_SIZE])
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_data_t* pbd;

	link_t_ptr plk, ilk;
	int i;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	plk = get_first_link(&(pbt->lkIndices));
	while (plk)
	{
		pbi = BplusIndexFromLink(plk);
		if (pbt->ft)
		{
			_attach_bplus_index(pbt, pbi);
		}

		ilk = get_first_link(&(pbi->lkChild));
		for (i = 0; i < pbi->count; i++)
		{
			if (IS_NULL_KEY(pbi->indices[i].key) || xmem_comp(pbi->indices[i].key, KEYBLK_SIZE, key, KEYBLK_SIZE) > 0)
			{
				break;
			}

			ilk = get_next_link(ilk);
		}

		plk = ilk;
		if (plk && plk->tag == lkBplusChild)
		{
			pbd = BplusDataFromChild(plk);
			plk = &(pbd->lkSibling);
			if (pbt->ft)
			{
				_attach_bplus_data(pbt, pbd);
			}
			break;
		}
	}

	if (!plk)
	{
		plk = get_last_link(&(pbt->lkDataSet));
		if (plk)
		{
			pbd = BplusDataFromLink(plk);
			if (pbt->ft)
			{
				_attach_bplus_data(pbt, pbd);
			}
		}
	}

	return plk;
}

static int _find_data_by_key(bplus_data_entity* ple, int n1, int n2, byte_t key[KEYBLK_SIZE])
{
	int n;
	int rt;

	if (n1 > n2)
	{
		return -1;
	}

	n = (n1 + n2) / 2;
	rt = xmem_comp(ple[n].key, KEYBLK_SIZE, key, KEYBLK_SIZE);

	switch (rt)
	{
	case 1:
		return _find_data_by_key(ple, n1, n - 1, key);
	case -1:
		return _find_data_by_key(ple, n + 1, n2, key);
	default:
		return n;
	}
}

static link_t_ptr _split_bplus_data(link_t_ptr ptr, link_t_ptr nlk, byte_t key[KEYBLK_SIZE])
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_data_t* pnew;
	bplus_index_t* pbi;

	link_t_ptr plk;
	byte_t org[KEYBLK_SIZE];
	int n, max;

	pbt = BplusTreeFromLink(ptr);

	pbd = BplusDataFromLink(nlk);

	//split at middle position
	max = page_size(pbd->dataset) / sizeof(bplus_data_entity);
	n = max / 2;
	xmem_copy(org, pbd->dataset[n].key, KEYBLK_SIZE);

	//alloc new data entity for holding left side keys
	pnew = _alloc_bplus_data(pbt);
	if (!pnew)
		return NULL;

	insert_link_before(&(pbt->lkDataSet), nlk, &(pnew->lkSibling));

	while (n--)
	{
		xmem_copy(pnew->dataset[n].key, pbd->dataset[n].key, KEYBLK_SIZE);
		pnew->dataset[n].off = pbd->dataset[n].off;
		pnew->count++;
	}

	if (pbt->ft)
	{
		_update_bplus_data(pbt, pnew, 1);
	}

	//rearrange right side keys
	pbd->count = 0;
	n = max / 2;
	while (n < max)
	{
		xmem_copy(pbd->dataset[pbd->count].key, pbd->dataset[n].key, KEYBLK_SIZE);
		pbd->dataset[pbd->count].off = pbd->dataset[n].off;
		pbd->count++;
		n++;
	}

	if (pbt->ft)
	{
		_update_bplus_data(pbt, pbd, 1);
	}

	//get parent index entity
	plk = _get_bplus_data_parent(nlk);
	if (!plk)
	{
		//alloc new root index entity
		pbi = _alloc_bplus_index(pbt);
		XDL_ASSERT(pbi != NULL);

		if (pbt->ft)
		{
			set_file_table_root(pbt->ft, pbi->index);
		}

		plk = &pbi->lkSibling;
		insert_link_after(&(pbt->lkIndices), LINK_LAST, plk);

		//new index entity insert to new parent
		n = _insert_bplus_index(ptr, plk, &(pnew->lkChild), org);
		XDL_ASSERT(n != C_ERR);

		//org nlk insert to new parent
		insert_link_after(&(pbi->lkChild), &(pnew->lkChild), &(pbd->lkChild));
		xmem_zero(pbi->indices[n + 1].key, KEYBLK_SIZE);
		pbi->indices[n + 1].ind = pbd->index;
		pbi->count++;

		if (pbt->ft)
		{
			_update_bplus_index(pbt, pbi, 1);
		}
	}
	else
	{
		//the splited key add to indices entity
		_insert_bplus_index(ptr, plk, &(pnew->lkChild), org);
	}

	return (xmem_comp(key, KEYBLK_SIZE, org, KEYBLK_SIZE) < 0) ? &(pnew->lkSibling) : nlk;
}

static link_t_ptr _insert_bplus_data(link_t_ptr ptr, link_t_ptr nlk, byte_t key[KEYBLK_SIZE])
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_data_t* pnew;
	int n, max;

	pbt = BplusTreeFromLink(ptr);

	//new last data entity
	if (!nlk)
	{
		pnew = _alloc_bplus_data(pbt);
		if (!pnew)
			return NULL;

		nlk = &pnew->lkSibling;
		insert_link_after(&(pbt->lkDataSet), LINK_LAST, nlk);
	}

	pbd = BplusDataFromLink(nlk);
	max = page_size(pbd->dataset) / sizeof(bplus_data_entity);

	//split full data entity, and return the new or org data for key to insert
	if (pbd->count == max)
	{
		nlk = _split_bplus_data(ptr, nlk, key);
		if (!nlk)
			return NULL;
	}

	//add the key to data entity
	pbd = BplusDataFromLink(nlk);
	n = pbd->count;
	while (n)
	{
		if (xmem_comp(pbd->dataset[n - 1].key, KEYBLK_SIZE, key, KEYBLK_SIZE) <= 0)
			break;

		xmem_copy(pbd->dataset[n].key, pbd->dataset[n - 1].key, KEYBLK_SIZE);
		pbd->dataset[n].off = pbd->dataset[n - 1].off;
		n--;
	}

	xmem_copy(pbd->dataset[n].key, key, KEYBLK_SIZE);
	pbd->count++;

	if (pbt->ft)
	{
		_update_bplus_data(pbt, pbd, 1);
	}

	return nlk;
}

static bool_t _shift_bplus_data(link_t_ptr ptr, link_t_ptr nlk)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_data_t* pbn;
	bplus_index_t* pbi;

	link_t_ptr parent, plk;
	int n, max;

	pbt = BplusTreeFromLink(ptr);

	//get prior data entity
	plk = get_prev_link(nlk);
	if (!plk)
		return 0;

	pbn = BplusDataFromLink(nlk);
	if (pbn->count < 2)
		return 0;

	//if prior data is full and not point to the same parent index, then can not shift
	pbd = BplusDataFromLink(plk);
	max = page_size(pbd->dataset) / sizeof(bplus_data_entity);
	if (pbd->count == max)
		return 0;

	parent = _get_bplus_data_parent(plk);
	//must the same parent
	if (parent != _get_bplus_data_parent(nlk))
		return 0;

	//find parent entity position
	pbi = BplusIndexFromLink(parent);
	for (n = 0; n < pbi->count; n++)
	{
		if (pbi->indices[n].ind == pbd->index)
			break;
	}

#ifdef _DEBUG
	if (n < pbi->count)
	{
		XDL_ASSERT(pbi->indices[n].ind == pbd->index);
	}
#endif

	//replace parent key
	xmem_copy(pbi->indices[n].key, pbn->dataset[1].key, KEYBLK_SIZE);

	if (pbt->ft)
	{
		_update_bplus_index(pbt, pbi, 1);
	}

	//add key to left data entity
	xmem_copy(pbd->dataset[pbd->count].key, pbn->dataset[0].key, KEYBLK_SIZE);
	pbd->dataset[pbd->count].off = pbn->dataset[0].off;
	pbd->count++;

	if (pbt->ft)
	{
		_update_bplus_data(pbt, pbd, 1);
	}

	//remove key from right data
	pbn->count--;
	for (n = 0; n < pbn->count; n++)
	{
		xmem_copy(pbn->dataset[n].key, pbn->dataset[n + 1].key, KEYBLK_SIZE);
		pbn->dataset[n].off = pbn->dataset[n + 1].off;
	}

	if (pbt->ft)
	{
		_update_bplus_data(pbt, pbn, 1);
	}

	return 1;
}

static void _pop_bplus_data(link_t_ptr ptr, link_t_ptr nlk, byte_t key_del[KEYBLK_SIZE], byte_t key_new[KEYBLK_SIZE])
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_index_t* pbi;
	link_t_ptr ilk, plk;
	int n;

	pbt = BplusTreeFromLink(ptr);
	pbd = BplusDataFromLink(nlk);

	plk = _get_bplus_data_parent(nlk);
	while (plk)
	{
		pbi = BplusIndexFromLink(plk);
		n = pbi->count;
		ilk = _get_bplus_index_last_child(plk);
		while (ilk)
		{
			if (n && xmem_comp(key_del, KEYBLK_SIZE, pbi->indices[n - 1].key, KEYBLK_SIZE) == 0)
			{
				xmem_copy(pbi->indices[n - 1].key, key_new, KEYBLK_SIZE);

				if (pbt->ft)
				{
					_update_bplus_index(pbt, pbi, 1);
				}
				return;
			}

			n--;
			ilk = _get_bplus_index_prev_sibling(ilk);
		}
		plk = _get_bplus_index_parent(plk);
	}
}

static void _delete_bplus_data(link_t_ptr ptr, link_t_ptr nlk)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_index_t* pbi;

	link_t_ptr plk, ilk;
	int n;

	pbt = BplusTreeFromLink(ptr);

	pbd = BplusDataFromLink(nlk);

	plk = _get_bplus_data_parent(nlk);
	if (plk)
	{
		pbi = BplusIndexFromLink(plk);
		n = pbi->count;
		ilk = _get_bplus_index_last_child(plk);
		while (ilk)
		{
			n--;
			if (ilk == &(pbd->lkChild))
				break;

			ilk = _get_bplus_index_prev_sibling(ilk);
		}
#ifdef _DEBUG
		if (n < pbi->count)
		{
			XDL_ASSERT(pbi->indices[n].ind == pbd->index);
		}
#endif
		delete_link(&(pbi->lkChild), ilk);
		pbi->count--;

		while (n < pbi->count)
		{
			xmem_copy(pbi->indices[n].key, pbi->indices[n + 1].key, KEYBLK_SIZE);
			pbi->indices[n].ind = pbi->indices[n + 1].ind;
			n++;
		}

		if (pbt->ft)
		{
			_update_bplus_index(pbt, pbi, 1);
		}

		if (!pbi->count)
		{
			_delete_bplus_index(ptr, plk);
		}
	}

	delete_link(&(pbt->lkDataSet), nlk);
	_free_bplus_data(pbt, pbd);
}

/****************************************************************************************************/

link_t_ptr create_bplus_tree()
{
	bplus_tree_t* pbt;

	pbt = (bplus_tree_t*)xmem_alloc(sizeof(bplus_tree_t));
	pbt->lk.tag = lkBplusTree;

	init_root_link(&(pbt->lkIndices));
	init_root_link(&(pbt->lkDataSet));

	return &(pbt->lk);
}

void attach_bplus_file_table(link_t_ptr ptr, link_t_ptr ft)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	if (!ft)
		return;

	pbt = BplusTreeFromLink(ptr);

	clear_bplus_tree(ptr);

	pbt->ft = ft;

	pbi = _new_bplus_index(pbt);
	pbi->index = get_file_table_root(pbt->ft);

	if (!_attach_bplus_index(pbt, pbi))
	{
		_free_bplus_index(pbt, pbi);
		return;
	}

	insert_link_after(&(pbt->lkIndices), LINK_LAST, &(pbi->lkSibling));
}

link_t_ptr detach_bplus_file_table(link_t_ptr ptr)
{
	bplus_tree_t* pbt;
	link_t_ptr ft;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	ft = pbt->ft;
	pbt->ft = NULL;

	return ft;
}

void clear_bplus_tree(link_t_ptr ptr)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_data_t* pbd;

	link_t_ptr ilk, parent, child;
	link_t_ptr stack;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	stack = NULL;

	ilk = get_first_link(&(pbt->lkIndices));
	while (ilk)
	{
		child = _get_bplus_index_first_child(ilk);
		if (child && child->tag == lkBplusIndex)
		{
			if (!stack)
				stack = create_stack_table();

			push_stack_node(stack, (void*)ilk);

			ilk = child;
			continue;
		}

		parent = _get_bplus_index_parent(ilk);
		if (parent)
		{
			pbi = BplusIndexFromLink(parent);
			delete_link(&(pbi->lkChild), ilk);
		}
		else
		{
			delete_link(&(pbt->lkIndices), ilk);
		}

		pbi = BplusIndexFromLink(ilk);
		_free_bplus_index(pbt, pbi);

		ilk = (stack) ? (link_t_ptr)pop_stack_node(stack) : NULL;
	}

	if (stack)
		destroy_stack_table(stack);

	while (ilk = delete_link(&pbt->lkDataSet, LINK_FIRST))
	{
		pbd = BplusDataFromLink(ilk);
		_free_bplus_data(pbt, pbd);
	}
}

void destroy_bplus_tree(link_t_ptr ptr)
{
	bplus_tree_t* pbt;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	clear_bplus_tree(ptr);

	xmem_free(pbt);
}

bool_t is_bplus_tree(link_t_ptr ptr)
{
	if (!ptr)
		return 0;

	return (ptr->tag == lkBplusTree) ? 1 : 0;
}

link_t_ptr find_bplus_entity(link_t_ptr ptr, varval_t var)
{
	byte_t key[KEYBLK_SIZE];

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	_key_gen(&var, key);

	return _find_bplus_entity(ptr, key);
}

int get_bplus_entity_key_count(link_t_ptr nlk)
{
	bplus_data_t* pbd;

	XDL_ASSERT(nlk && nlk->tag == lkBplusData);

	pbd = BplusDataFromLink(nlk);

	return pbd->count;
}

dword_t get_bplus_entity_key(link_t_ptr nlk, int pos, byte_t* buf, dword_t max)
{
	bplus_data_t* pbd;

	XDL_ASSERT(nlk && nlk->tag == lkBplusData);

	pbd = BplusDataFromLink(nlk);

	if (pos < 0 || pos >= pbd->count)
		return 0;

	max = (max < KEYBLK_SIZE) ? max : KEYBLK_SIZE;

	if (buf)
	{
		xmem_copy(buf, pbd->dataset[pos].key, max);
	}

	return max;
}

link_t_ptr insert_bplus_entity(link_t_ptr ptr, varval_t var)
{
	bplus_data_t* pbd;
	link_t_ptr nlk;
	int n;
	byte_t key[KEYBLK_SIZE];

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	_key_gen(&var, key);

	nlk = _find_bplus_entity(ptr, key);
	if (nlk)
	{
		pbd = BplusDataFromLink(nlk);

		n = _find_data_by_key(pbd->dataset, 0, pbd->count - 1, key);
		if (n >= 0)
		{
			return nlk;
		}

		if (pbd->count > 1 && xmem_comp(pbd->dataset[1].key, KEYBLK_SIZE, key, KEYBLK_SIZE) < 0)
		{
			_shift_bplus_data(ptr, nlk);
		}
	}

	return _insert_bplus_data(ptr, nlk, key);
}

bool_t delete_bplus_entity(link_t_ptr ptr, varval_t var)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	link_t_ptr nlk, nxt;
	int i, n, max;
	byte_t key[KEYBLK_SIZE];

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	_key_gen(&var, key);

	nlk = _find_bplus_entity(ptr, key);

	if (!nlk)
		return 0;

	pbt = BplusTreeFromLink(ptr);
	pbd = BplusDataFromLink(nlk);

	n = _find_data_by_key(pbd->dataset, 0, pbd->count - 1, key);
	if (n < 0)
		return 0;
	
	max = page_size(pbd->dataset) / sizeof(bplus_data_entity);

	if (!n && pbd->count > 1)
	{
		_pop_bplus_data(ptr, nlk, pbd->dataset[n].key, pbd->dataset[n + 1].key);
	}

	//remove key
	pbd->count--;
	for (i = n; i < pbd->count; i++)
	{
		xmem_copy(pbd->dataset[i].key, pbd->dataset[i + 1].key, KEYBLK_SIZE);
		pbd->dataset[i].off = pbd->dataset[i + 1].off;
	}

	if (pbt->ft)
	{
		_update_bplus_data(pbt, pbd, 1);
	}

	//shift one key from rightside entity
	nxt = get_next_link(nlk);
	if (nxt)
	{
		_shift_bplus_data(ptr, nxt);
	}

	//if data entity is empty then remove it
	pbd = BplusDataFromLink(nlk);
	if (!pbd->count)
	{
		_delete_bplus_data(ptr, nlk);
	}

	return 1;
}

void enum_bplus_entity(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_data_t* pbd;

	link_t_ptr ilk, child;
	link_t_ptr stack;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	stack = NULL;

	ilk = get_first_link(&(pbt->lkIndices));
	while (ilk)
	{
		if (ilk->tag == lkBplusIndex)
		{
			pbi = BplusIndexFromLink(ilk);
			if (pbt->ft)
			{
				_attach_bplus_index(pbt, pbi);
			}
		}
		else if (ilk->tag == lkBplusChild)
		{
			pbd = BplusDataFromChild(ilk);
			if (pbt->ft)
			{
				_attach_bplus_data(pbt, pbd);
			}

			if (!(*pf)(&(pbd->lkSibling), param))
				goto _CLEAN;
		}

		child = (ilk->tag == lkBplusIndex)? _get_bplus_index_first_child(ilk) : NULL;
		if (child)
		{
			if (!stack)
				stack = create_stack_table();

			push_stack_node(stack, (void*)ilk);

			ilk = child;
			continue;
		}

		while (ilk)
		{
			ilk = _get_bplus_index_next_sibling(ilk);
			if (ilk)
				break;
			ilk = (stack) ? (link_t_ptr)pop_stack_node(stack) : NULL;
		}
	}

_CLEAN:

	if (stack)
		destroy_stack_table(stack);
}

void update_bplus_file_table(link_t_ptr ptr, bool_t b_save)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_data_t* pbd;

	link_t_ptr ilk, child;
	link_t_ptr stack;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	if (!pbt->ft)
		return;

	stack = NULL;

	ilk = get_first_link(&(pbt->lkIndices));
	while (ilk)
	{
		if (ilk->tag == lkBplusIndex)
		{
			pbi = BplusIndexFromLink(ilk);
			if (!_update_bplus_index(pbt, pbi, b_save))
				goto _CLEAN;
		}
		else if (ilk->tag == lkBplusChild)
		{
			pbd = BplusDataFromChild(ilk);
			if (!_update_bplus_data(pbt, pbd, b_save))
				goto _CLEAN;
		}

		child = (ilk->tag == lkBplusIndex)? _get_bplus_index_first_child(ilk) : NULL;
		if (child)
		{
			if (!stack)
				stack = create_stack_table();

			push_stack_node(stack, (void*)ilk);

			ilk = child;
			continue;
		}

		while (ilk)
		{
			ilk = _get_bplus_index_next_sibling(ilk);
			if (ilk)
				break;
			ilk = (stack) ? (link_t_ptr)pop_stack_node(stack) : NULL;
		}
	}

_CLEAN:

	if (stack)
		destroy_stack_table(stack);
}

#ifdef _DEBUG
#include <time.h>
#include "oemrand.h"

static bool_t print_node(link_t_ptr nlk, void* pa)
{
	bplus_data_t* pbd = BplusDataFromLink(nlk);
	bplus_index_t* pbi;
	int n;
	byte_t* org = (byte_t*)pa;

	_tprintf(_T("{"));
	for (n = 0; n < pbd->count; n++)
	{
		printf("%s ", pbd->dataset[n].key);

		XDL_ASSERT(xmem_comp(pbd->dataset[n].key, KEYBLK_SIZE, org, KEYBLK_SIZE) > 0);

		xmem_copy(org, pbd->dataset[n].key, KEYBLK_SIZE);
	}
	_tprintf(_T("}"));

	link_t_ptr plk = _get_bplus_data_parent(nlk);
	while (plk)
	{
		pbi = BplusIndexFromLink(plk);

		_tprintf(_T("("));
		for (int i = 0; i < pbi->count; i++)
		{
			if (pbd->index == pbi->indices[i].ind)
				_tprintf(_T("["));

			if (pbd->index == pbi->indices[i].ind)
				printf("%s] ", pbi->indices[i].key);
			else
				printf("%s ", pbi->indices[i].key);
		}
		_tprintf(_T(")"));

		plk = _get_bplus_index_parent(plk);
	}

	_tprintf(_T("\n"));

	return 1;
}


void test_bplus_tree()
{
	/*link_t_ptr ptr = create_bplus_tree();
	link_t_ptr nlk;

	//tchar_t numset[] = _T("27 16 48 40 48 32 33 48 48 11 31 29 33 47 11 9 1 46 42 8 35 15 1 15 2 33 2 32 49 36 37 38 0 50 51 80 70 83 91 85 96 82 43 20 24 11 12 46 32 11 46 22 11 45 9 17 39 0 44 30 14 18 21 75 71 65 61 83 77 97 66 78 60 63 91 81 92 84");
	tchar_t numset[] = _T("136 82 188 91 130 20 27 152 84 182 42 5 29 78 180 32 173 107 10 191 164 83 139 1 99 100 87 172 181 46 69 35 5 8 69 170 24 118 9 79 126 6 5 174 56 171 44 56 175 124 1 73 133 144 115 34 172 160 108 198 54 119 133 74 193 76 172 188 176 29 51 76 64 9 57 132 37 125 91 196 91 111 184 199 64 114 58 150 14 38 7");// 23 167 51 151 138 121 194 9 175 1");
	
	varval_t v = { 0 };
	v.vv = VV_LONG;

	tchar_t* key;
	int len;
	const tchar_t *token = numset;
	while (token = parse_string_token(token, -1, _T(' '), &key, &len))
	{
		v.long_one = xsntol(key, len);
		_tprintf(_T("INS %d: "), v.long_one);

		if (v.long_one == 181)
			v.long_one *= 1;
		nlk = insert_bplus_entity(ptr, v, NULL);

		_tprintf(_T("("));
		bplus_data_t* pbd = BplusDataFromLink(nlk);
		for (int i = 0; i < pbd->count; i++)
		{
			_tprintf(_T("%d "), pbd->dataset[i].key.long_one);
		}
		_tprintf(_T(")\n"));
	}

	destroy_bplus_tree(ptr);*/

	byte_t org[KEYBLK_SIZE];

	int j = 100;
	while (j--)
	{
		link_t_ptr ptr = create_bplus_tree();

		//link_t_ptr pt_file = create_file_table(_T("mytable"), FILE_OPEN_APPEND);

		//attach_bplus_file_table(ptr, pt_file);

		srand48(time(NULL));

		link_t_ptr ent;
		varval_t v = { 0 };

		int n = 100;

		v.vv = VV_LONG;
		for (int i = 0; i < n; i++)
		{
			v.long_one = lrand48() % n;
			_tprintf(_T("%04d "), v.long_one);

			ent = find_bplus_entity(ptr, v);

			ent = insert_bplus_entity(ptr, v);
		}

		_tprintf(_T("\n"));

		xmem_zero(org, KEYBLK_SIZE);
		enum_bplus_entity(ptr, print_node, (void*)org);

		v.vv = VV_LONG;
		for (int i = 0; i < n; i++)
		{
			v.long_one = lrand48() % n;
			_tprintf(_T("%04d "), v.long_one);

			delete_bplus_entity(ptr, v);
		}

		_tprintf(_T("\n"));

		xmem_zero(org, KEYBLK_SIZE);
		enum_bplus_entity(ptr, print_node, (void*)org);

		//update_bplus_tree(ptr, 1);

		//pt_file = detach_bplus_file_table(ptr);

		//destroy_file_table(pt_file);

		destroy_bplus_tree(ptr);

		Sleep(100);
	}
}
#endif