/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc B+ tree document

	@module	bplustree.c | implement file

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

#include "dblink.h"
#include "varobj.h"
#include "stacktable.h"
#include "filetable.h"

#include "xdlimp.h"


#define BPLUS_ENTITY_INDEX	1
#define BPLUS_ENTITY_DATA	0

#define BPLUS_PAGE_HEADER	16

/*
struct{
	byte_t[4] length; //total encoded length
	byte_t[vallen] val; //val encoded bytes
	byte_t[keylen] key; //key encoded bytes
}entity_encode
*/

typedef struct _bplus_index_entity{
	key32_t ind;	//children file table index
	key64_t key;	//key token
//#if defined(_DEBUG) || defined(DEBUG)
//	byte_t rev[888];
//#endif
}bplus_index_entity;

typedef struct _bplus_index_t{
	link_t lkSibling;		// the sibling node link component also as self link component
	link_t lkChild;			// the child node link component 

	key32_t index;		//file table index
	dword_t count;		//entities count
	bplus_index_entity* indices; //array for index keyset
	byte_t* guider;
	bool_t tabled; //if file table attached
}bplus_index_t;

typedef struct _bplus_data_entity{
	union{
		key32_t off;
		key64_t var;
	};
	union{
		key32_t len;
		key64_t val;
	};
	key64_t key;
//#if defined(_DEBUG) || defined(DEBUG)
//	byte_t rev[888];
//#endif
}bplus_data_entity;

typedef struct _bplus_data_t{
	link_t lkSibling;		// the sibling node link component also as self link component

	key32_t index;		// file table index
	dword_t count;			// entities count
	bplus_data_entity* dataset;
	byte_t* guider;
	bool_t tabled; //if file table attached
}bplus_data_t;

typedef struct _bplus_tree_t{
	link_t lk;				// the self link component

	link_t_ptr entity;		// root entity

	link_t_ptr ind_table;		//index file table
	link_t_ptr dat_table;		//data file table
}bplus_tree_t;

/*restore bplus tree from self link ptr*/
#define BplusTreeFromLink(p) TypePtrFromLink(bplus_tree_t,p)
/*restore bplus index from self link ptr*/
#define BplusIndexFromLink(p) TypePtrFromSibling(bplus_index_t,p)
/*restore bplus index from child link ptr*/
#define BplusIndexFromChild(p) TypePtrFromChild(bplus_index_t,p)
/*restore bplus data from self link ptr*/
#define BplusDataFromLink(p) TypePtrFromSibling(bplus_data_t,p)


static int _insert_bplus_index(link_t_ptr ptr, link_t_ptr plk, link_t_ptr ilk, key64_t* pkey);
static link_t_ptr _split_bplus_index(link_t_ptr ptr, link_t_ptr ilk, key64_t* pkey);
static link_t_ptr _split_bplus_data(link_t_ptr ptr, link_t_ptr nlk, key64_t* pkey);
static int _insert_bplus_data(link_t_ptr ptr, link_t_ptr plk, link_t_ptr* pnlk, key64_t* pkey);
static bool_t _shift_bplus_data(link_t_ptr ptr, link_t_ptr nlk);

#define _key_gen(var, pkey) variant_hash64(var, pkey)
#define _key_copy(pkey1, pkey2) (*pkey1 = *pkey2)
#define _key_zero(pkey)	(*pkey = 0)
#define _key_null(pkey) ((*pkey == 0) ? 1 : 0)

static int _key_comp(key64_t* pkey1, key64_t* pkey2)
{
	if (*pkey1 > *pkey2)
		return 1;
	else if (*pkey1 < *pkey2)
		return -1;
	else
		return 0;
}

static void _key_md5(variant_t var, key64_t* pkey)
{
	unsigned char keyblk[256];
	unsigned char padding[48];
	md5_context md5;
	int len;

	len = variant_encode(var, keyblk, 256);
	
	xmem_set(padding, 0x36, 48);

	md5_starts(&md5);
	md5_update(&md5, keyblk, len);
	md5_update(&md5, padding, 48);
	md5_finish(&md5, (unsigned char*)pkey);
}


static bool_t _lock_bplus_index(bplus_tree_t* pbt, bplus_index_t* pbi)
{
	res_file_t mh;
	byte_t* buff = NULL;

	XDL_ASSERT(!pbi->guider);

	pbi->guider = xmem_alloc(PAGE_SIZE);

	lock_file_table_block(pbt->ind_table, (dword_t)pbi->index, PAGE_SIZE, 0, &mh, (void**)&buff);
	if (buff)
	{
		xmem_copy((void*)pbi->guider, (void*)buff, PAGE_SIZE);
	}
	unlock_file_table_block(pbt->ind_table, (dword_t)pbi->index, PAGE_SIZE, 0, mh, (void*)buff);

	pbi->indices = (bplus_index_entity*)(pbi->guider + BPLUS_PAGE_HEADER);
	pbi->count = GET_SWORD_LOC(pbi->guider, 2);

	return 1;
}

static void _unlock_bplus_index(bplus_tree_t* pbt, bplus_index_t* pbi, bool_t b_save)
{
	res_file_t mh;
	byte_t* buff = NULL;

	XDL_ASSERT(pbi->guider != NULL);

	if (b_save)
	{
		PUT_SWORD_LOC(pbi->guider, 2, pbi->count);

		lock_file_table_block(pbt->ind_table, (dword_t)pbi->index, PAGE_SIZE, 1, &mh, (void**)&buff);
		xmem_copy((void*)buff, (void*)pbi->guider, (BPLUS_PAGE_HEADER + pbi->count * sizeof(bplus_index_entity)));
		unlock_file_table_block(pbt->ind_table, (dword_t)pbi->index, PAGE_SIZE, 1, mh, (void*)buff);
	}

	xmem_free(pbi->guider);
	pbi->guider = NULL;
	pbi->indices = NULL;
}

static bool_t _lock_bplus_data(bplus_tree_t* pbt, bplus_data_t* pbd)
{
	res_file_t mh;
	byte_t* buff = NULL;

	XDL_ASSERT(!pbd->guider);

	pbd->guider = xmem_alloc(PAGE_SIZE);

	lock_file_table_block(pbt->ind_table, (dword_t)pbd->index, PAGE_SIZE, 0, &mh, &buff);
	if (buff)
	{
		xmem_copy((void*)pbd->guider, (void*)buff, PAGE_SIZE);
	}
	unlock_file_table_block(pbt->ind_table, (dword_t)pbd->index, PAGE_SIZE, 0, mh, (void*)buff);

	pbd->dataset = (bplus_data_entity*)(pbd->guider + BPLUS_PAGE_HEADER);
	pbd->count = GET_SWORD_LOC(pbd->guider, 2);

	return 1;
}

static void _unlock_bplus_data(bplus_tree_t* pbt, bplus_data_t* pbd, bool_t b_save)
{
	res_file_t mh;
	byte_t* buff = NULL;

	XDL_ASSERT(pbd->guider != NULL);

	if (b_save)
	{
		PUT_SWORD_LOC(pbd->guider, 2, pbd->count);

		lock_file_table_block(pbt->ind_table, (dword_t)pbd->index, PAGE_SIZE, 1, &mh, &buff);
		xmem_copy((void*)buff, (void*)pbd->guider, (BPLUS_PAGE_HEADER + pbd->count * sizeof(bplus_data_entity)));
		unlock_file_table_block(pbt->ind_table, (dword_t)pbd->index, PAGE_SIZE, 1, mh, (void*)buff);
	}

	xmem_free(pbd->guider);
	pbd->guider = NULL;
	pbd->dataset = NULL;
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

static bplus_data_t* _new_bplus_data()
{
	bplus_data_t* pbn;

	pbn = (bplus_data_t*)xmem_alloc(sizeof(bplus_data_t));
	pbn->lkSibling.tag = lkBplusData;
	pbn->lkSibling.next = NULL;
	pbn->lkSibling.prev = NULL;

	return pbn;
}

static bool_t _alloc_bplus_index(bplus_tree_t* pbt, bplus_index_t* pbi)
{
	XDL_ASSERT(pbi->guider == NULL);

	if (pbt->ind_table)
	{
		XDL_ASSERT(pbi->index == 0);
		pbi->index = alloc_file_table_block(pbt->ind_table, PAGE_SIZE);

		if (!(pbi->index)) return 0;

		XDL_ASSERT(_lock_bplus_index(pbt, pbi));
	}
	else
	{
		XDL_ASSERT(pbi->guider == NULL);
		pbi->guider = (byte_t*)xmem_alloc(PAGE_SIZE);
		pbi->indices = (bplus_index_entity*)(pbi->guider + BPLUS_PAGE_HEADER);
	}

	PUT_SWORD_LOC(pbi->guider, 0, BPLUS_ENTITY_INDEX);
	PUT_SWORD_LOC(pbi->guider, 2, 0);

	if (pbt->ind_table)
	{
		pbi->tabled = 1;
		pbi->count = 0;
		_unlock_bplus_index(pbt, pbi, 1);
	}
	else
	{
		pbi->tabled = 0;
		pbi->count = 0;
		pbi->index = (key32_t)pbi->indices;
	}

	return 1;
}

static bool_t _attach_bplus_index(bplus_tree_t* pbt, bplus_index_t* pbi)
{
	bplus_index_t* pbn;
	bplus_data_t* pbd;
	int tag = -1;
	dword_t m;

	res_file_t mh;
	byte_t* guider = NULL;

	XDL_ASSERT(pbt->ind_table != NULL);

	if (pbi->tabled)
		return 1;

	XDL_ASSERT(_lock_bplus_index(pbt, pbi));

	m = pbi->count;
	pbi->count = 0;
	while (pbi->count < m)
	{
		if (tag < 0)
		{
			//test the child node is index or data entity
			lock_file_table_block(pbt->ind_table, (dword_t)(pbi->indices[pbi->count].ind), PAGE_SIZE, 0, &mh, (void**)&guider);
			if (!guider)
				break;

			tag = GET_SWORD_LOC(guider, 0);

			unlock_file_table_block(pbt->ind_table, (dword_t)(pbi->indices[pbi->count].ind), PAGE_SIZE, 0, mh, (void*)guider);
			guider = NULL;
		}

		if (tag == BPLUS_ENTITY_INDEX)
		{
			pbn = _new_bplus_index(pbt);

			pbn->index = pbi->indices[pbi->count].ind;

			insert_link_after(&(pbi->lkChild), LINK_LAST, &(pbn->lkSibling));
			pbi->count++;
		}
		else if (tag == BPLUS_ENTITY_DATA)
		{
			pbd = _new_bplus_data(pbt);

			pbd->index = (dword_t)(pbi->indices[pbi->count].ind);

			insert_link_after(&(pbi->lkChild), LINK_LAST, &(pbd->lkSibling));
			pbi->count++;
		}
	}

	_unlock_bplus_index(pbt, pbi, 0);

	pbi->tabled = 1;

	return 1;
}

static void _free_bplus_index(bplus_tree_t* pbt, bplus_index_t* pbi)
{
	if (pbt->ind_table)
	{
		free_file_table_block(pbt->ind_table, (dword_t)pbi->index, PAGE_SIZE);
	}
	else
	{
		xmem_free(pbi->guider);
	}

	xmem_free(pbi);
}

static bool_t _alloc_bplus_data(bplus_tree_t* pbt, bplus_data_t* pbn)
{
	if (pbt->ind_table)
	{
		XDL_ASSERT(pbn->index == 0);
		pbn->index = alloc_file_table_block(pbt->ind_table, PAGE_SIZE);
		if (!(pbn->index)) return 0;
	
		XDL_ASSERT(_lock_bplus_data(pbt, pbn));
	}
	else
	{
		XDL_ASSERT(pbn->guider == NULL);
		pbn->guider = (byte_t*)xmem_alloc(PAGE_SIZE);
		pbn->dataset = (bplus_data_entity*)(pbn->guider + BPLUS_PAGE_HEADER);
	}

	PUT_SWORD_LOC(pbn->guider, 0, BPLUS_ENTITY_DATA);
	PUT_SWORD_LOC(pbn->guider, 2, 0);

	if (pbt->ind_table)
	{
		pbn->tabled = 1;
		pbn->count = 0;
		_unlock_bplus_data(pbt, pbn, 1);
	}
	else
	{
		pbn->tabled = 0;
		pbn->count = 0;
		pbn->index = (key32_t)pbn->dataset;
	}

	return 1;
}

static bool_t _attach_bplus_data(bplus_tree_t* pbt, bplus_data_t* pbn)
{

	XDL_ASSERT(pbt->ind_table != NULL);

	if (pbn->tabled)
		return 1;

	XDL_ASSERT(_lock_bplus_data(pbt, pbn));

	_unlock_bplus_data(pbt, pbn, 0);

	pbn->tabled = 1;

	return 1;
}

static void _free_bplus_data(bplus_tree_t* pbt, bplus_data_t* pbd)
{
	XDL_ASSERT(pbd->count == 0);

	if (pbt->ind_table)
	{
		free_file_table_block(pbt->ind_table, (dword_t)pbd->index, PAGE_SIZE);
	}
	else
	{
		xmem_free(pbd->guider);
	}

	xmem_free(pbd);
}

static link_t_ptr _get_bplus_index_first_child(link_t_ptr ilk)
{
	bplus_index_t* pbn;

	if (ilk->tag != lkBplusIndex)
		return NULL;

	pbn = BplusIndexFromLink(ilk);
	return get_first_link(&pbn->lkChild);
}

static link_t_ptr _get_bplus_index_last_child(link_t_ptr ilk)
{
	bplus_index_t* pbn;

	if (ilk->tag != lkBplusIndex)
		return NULL;

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
	link_t_ptr parent;

	XDL_ASSERT(nlk->tag == lkBplusData);

	parent = get_root_link(nlk);
	if (!parent)
		return NULL;

	pbn = BplusIndexFromChild(parent);

	return (pbn->lkSibling.tag == lkBplusIndex) ? &(pbn->lkSibling) : NULL;
}

static link_t_ptr _split_bplus_index(link_t_ptr ptr, link_t_ptr ilk, key64_t* pkey)
{
	bplus_tree_t* pbt;
	bplus_index_t* pnew;
	bplus_index_t* pbi;
	bplus_index_t* pbn;
	bplus_data_t* pbd;

	link_t_ptr plk, llk;

	key64_t org;
	int n, m;


	pbt = BplusTreeFromLink(ptr);

	pbi = BplusIndexFromLink(ilk);

	//split at middle position
	m = (PAGE_SIZE - BPLUS_PAGE_HEADER) / sizeof(bplus_index_entity);

	llk = _get_bplus_index_first_child(ilk);
	n = 0;
	while (n < m / 2 && llk)
	{
		llk = _get_bplus_index_next_sibling(llk);
		n++;
	}
#if defined(_DEBUG) || defined(DEBUG)
	XDL_ASSERT(llk != NULL);
#endif

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_index(pbt, pbi));
	}

	_key_copy(&org, &(pbi->indices[n].key));

	//alloc new index entity for holding left side keys
	pnew = _new_bplus_index();
	XDL_ASSERT(_alloc_bplus_index(pbt, pnew));

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_index(pbt, pnew));
	}

	//remove middle key from original entity and insert into new entity
	if (llk->tag == lkBplusData)
	{
		pbd = BplusDataFromLink(llk);
		llk = _get_bplus_index_prev_sibling(llk);
#if defined(_DEBUG) || defined(DEBUG)
		XDL_ASSERT(pbd->index == pbi->indices[n].ind);
		XDL_ASSERT(ilk == _get_bplus_data_parent(&(pbd->lkSibling)));
#endif
		delete_link(&(pbi->lkChild), &(pbd->lkSibling));
		//must insert at last
		_key_zero(&(pnew->indices[n].key));
		pnew->indices[n].ind = pbd->index;
		insert_link_after(&(pnew->lkChild), LINK_LAST, &(pbd->lkSibling));
		pnew->count = 1;
	}
	else if (llk->tag == lkBplusIndex)
	{
		pbn = BplusIndexFromLink(llk);
		llk = _get_bplus_index_prev_sibling(llk);
#if defined(_DEBUG) || defined(DEBUG)
		XDL_ASSERT(pbn->index == pbi->indices[n].ind);
		XDL_ASSERT(ilk == _get_bplus_index_parent(&(pbn->lkSibling)));
#endif
		delete_link(&(pbi->lkChild), &(pbn->lkSibling));
		//must insert at last
		_key_zero(&(pnew->indices[n].key));
		pnew->indices[n].ind = pbn->index;
		insert_link_after(&(pnew->lkChild), LINK_LAST, &(pbn->lkSibling));
		pnew->count = 1;
	}
	else
	{
		XDL_ASSERT(0);
	}

	while (n--)
	{
		_key_copy(&(pnew->indices[n].key), &(pbi->indices[n].key));
		pnew->indices[n].ind = pbi->indices[n].ind;
		pnew->count++;

		//reset child entites parent
		if (llk->tag == lkBplusData)
		{
			pbd = BplusDataFromLink(llk);
			llk = _get_bplus_index_prev_sibling(llk);
#if defined(_DEBUG) || defined(DEBUG)
			XDL_ASSERT(pbd->index == pbi->indices[n].ind);
			XDL_ASSERT(ilk == _get_bplus_data_parent(&(pbd->lkSibling)));
#endif
			delete_link(&(pbi->lkChild), &(pbd->lkSibling));
			insert_link_before(&(pnew->lkChild), LINK_FIRST, &(pbd->lkSibling));
		}
		else if (llk->tag == lkBplusIndex)
		{
			pbn = BplusIndexFromLink(llk);
			llk = _get_bplus_index_prev_sibling(llk);
#if defined(_DEBUG) || defined(DEBUG)
			XDL_ASSERT(pbn->index == pbi->indices[n].ind);
			XDL_ASSERT(ilk == _get_bplus_index_parent(&(pbn->lkSibling)));
#endif
			delete_link(&(pbi->lkChild), &(pbn->lkSibling));
			insert_link_before(&(pnew->lkChild), LINK_FIRST, &(pbn->lkSibling));
		}
		else
		{
			XDL_ASSERT(0);
		}
	}

	if (pbt->ind_table)
	{
		_unlock_bplus_index(pbt, pnew, 1);
	}

	//remove the middle position key, and rearrange right side keys
	pbi->count = 0;
	n = m / 2 + 1;
	while (n < m)
	{
		_key_copy(&(pbi->indices[pbi->count].key), &(pbi->indices[n].key));
		pbi->indices[pbi->count].ind = pbi->indices[n].ind;
		pbi->count++;
		n++;
	}

	if (pbt->ind_table)
	{
		_unlock_bplus_index(pbt, pbi, 1);
	}

	//the middle position key insert into parent index entity
	plk = _get_bplus_index_parent(ilk);
	if (!plk)
	{
		//alloc new root index entity
		pbn = _new_bplus_index();
		XDL_ASSERT(_alloc_bplus_index(pbt, pbn));

		plk = pbt->entity = &(pbn->lkSibling);

		if (pbt->ind_table)
		{
			set_file_table_root(pbt->ind_table, (dword_t)pbn->index);
		}

		//new index entity insert to new parent
		n = _insert_bplus_index(ptr, plk, &(pnew->lkSibling), &org);
		XDL_ASSERT(n != C_ERR);

		if (pbt->ind_table)
		{
			XDL_ASSERT(_lock_bplus_index(pbt, pbn));
		}

		//org nlk insert back to new parent
		insert_link_after(&(pbn->lkChild), &(pnew->lkSibling), &(pbi->lkSibling));
		_key_zero(&(pbn->indices[n + 1].key));
		pbn->indices[n + 1].ind = pbi->index;
		pbn->count++;

		if (pbt->ind_table)
		{
			_unlock_bplus_index(pbt, pbn, 1);
		}
	}
	else
	{
		//new index entity insert to org parent
		_insert_bplus_index(ptr, plk, &(pnew->lkSibling), &org);
	}

	return (_key_comp(pkey, &org) < 0) ? &(pnew->lkSibling) : ilk;
}

static int _insert_bplus_index(link_t_ptr ptr, link_t_ptr plk, link_t_ptr ilk, key64_t* pkey)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_index_t* pbn;
	bplus_data_t* pbd;
	int n, m;
	link_t_ptr llk;

	pbt = BplusTreeFromLink(ptr);
	pbi = BplusIndexFromLink(plk);

	n = pbi->count;
	m = (PAGE_SIZE - BPLUS_PAGE_HEADER) / sizeof(bplus_index_entity);
	//split full index entity, and return the new or org index for key to insert
	if (n == m)
	{
		plk = _split_bplus_index(ptr, plk, pkey);
	}

	//add the key to index entity
	pbi = BplusIndexFromLink(plk);

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_index(pbt, pbi));
	}

	llk = get_last_link(&(pbi->lkChild));
	n = pbi->count;
	while (n)
	{
		if (!_key_null(&(pbi->indices[n - 1].key)) && _key_comp(&(pbi->indices[n - 1].key), pkey) <= 0)
			break;

		_key_copy(&(pbi->indices[n].key), &(pbi->indices[n - 1].key));
		pbi->indices[n].ind = pbi->indices[n - 1].ind;
		n--;

		llk = get_prev_link(llk);
	}

	_key_copy(&(pbi->indices[n].key), pkey);

	if (ilk->tag == lkBplusData)
	{
		pbd = BplusDataFromLink(ilk);
		pbi->indices[n].ind = pbd->index;
	}
	else if (ilk->tag == lkBplusIndex)
	{
		pbn = BplusIndexFromLink(ilk);
		pbi->indices[n].ind = pbn->index;
	}
	else
	{
		XDL_ASSERT(0);
	}
	pbi->count++;
	insert_link_after(&(pbi->lkChild), ((llk) ? llk : LINK_FIRST), ilk);

	if (pbt->ind_table)
	{
		_unlock_bplus_index(pbt, pbi, 1);
	}

	return n;
}

static void _delete_bplus_index(link_t_ptr ptr, link_t_ptr ilk)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_index_t* pbp;

	link_t_ptr plk;
	dword_t n;

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

#if defined(_DEBUG) || defined(DEBUG)
		XDL_ASSERT(pbp->indices[n].ind == pbi->index);
#endif

		delete_link(&(pbp->lkChild), ilk);
		pbp->count--;

		_free_bplus_index(pbt, pbi);

		if (pbt->ind_table)
		{
			XDL_ASSERT(_lock_bplus_index(pbt, pbp));
		}

		_key_zero(&(pbp->indices[n].key));
		pbp->indices[n].ind = 0;

		while (n < pbp->count)
		{
			_key_copy(&(pbp->indices[n].key), &(pbp->indices[n + 1].key));
			pbp->indices[n].ind = pbp->indices[n + 1].ind;
			n++;
		}

		if (pbt->ind_table)
		{
			_unlock_bplus_index(pbt, pbp, 1);
		}

		if (!pbp->count)
		{
			_delete_bplus_index(ptr, plk);
		}
	}
	else
	{
		//delete root entity
		XDL_ASSERT(pbt->entity == ilk);
		pbt->entity = NULL;

		if (pbt->ind_table)
		{
			XDL_ASSERT(pbi->index == get_file_table_root(pbt->ind_table));
			set_file_table_root(pbt->ind_table, 0);
		}

		_free_bplus_index(pbt, pbi);
	}
}

static int _find_bplus_data(bplus_data_entity* ple, int n1, int n2, key64_t* pkey)
{
	int n;
	int rt;

	if (n1 > n2)
	{
		return -1;
	}

	n = (n1 + n2) / 2;
	rt = _key_comp(&(ple[n].key), pkey);

	switch (rt)
	{
	case 1:
		return _find_bplus_data(ple, n1, n - 1, pkey);
	case -1:
		return _find_bplus_data(ple, n + 1, n2, pkey);
	default:
		return n;
	}
}

static link_t_ptr _split_bplus_data(link_t_ptr ptr, link_t_ptr nlk, key64_t* pkey)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_data_t* pnew;
	bplus_index_t* pbi;

	link_t_ptr plk;
	key64_t org;
	int n, m;

	pbt = BplusTreeFromLink(ptr);
	pbd = BplusDataFromLink(nlk);

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbd));
	}

	//split at middle position
	m = (PAGE_SIZE - BPLUS_PAGE_HEADER) / sizeof(bplus_data_entity);
	n = m / 2;
	_key_copy(&org, &(pbd->dataset[n].key));

	//alloc new data entity for holding left side keys
	pnew = _new_bplus_data();
	XDL_ASSERT(_alloc_bplus_data(pbt, pnew));

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pnew));
	}

	while (n--)
	{
		_key_copy(&(pnew->dataset[n].key), &(pbd->dataset[n].key));
		if (pbt->dat_table)
		{
			pnew->dataset[n].off = pbd->dataset[n].off;
			pnew->dataset[n].len = pbd->dataset[n].len;
		}
		else
		{
			pnew->dataset[n].var = pbd->dataset[n].var;
			pnew->dataset[n].val = pbd->dataset[n].val;
		}
		pnew->count++;
	}

	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pnew, 1);
	}

	//rearrange right side keys
	pbd->count = 0;
	n = m / 2;
	while (n < m)
	{
		_key_copy(&(pbd->dataset[pbd->count].key), &(pbd->dataset[n].key));
		if (pbt->dat_table)
		{
			pbd->dataset[pbd->count].off = pbd->dataset[n].off;
			pbd->dataset[pbd->count].len = pbd->dataset[n].len;
		}
		else
		{
			pbd->dataset[pbd->count].var = pbd->dataset[n].var;
			pbd->dataset[pbd->count].val = pbd->dataset[n].val;
		}
		pbd->count++;
		n++;
	}

	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pbd, 1);
	}

	//get parent index entity
	plk = _get_bplus_data_parent(nlk);
	if (!plk)
	{
		//alloc new root index entity
		pbi = _new_bplus_index();
		XDL_ASSERT(_alloc_bplus_index(pbt, pbi));

		plk = pbt->entity = &pbi->lkSibling;

		if (pbt->ind_table)
		{
			set_file_table_root(pbt->ind_table, (dword_t)pbi->index);
		}

		//new index entity insert to new parent
		n = _insert_bplus_index(ptr, plk, &(pnew->lkSibling), &org);
		XDL_ASSERT(n != C_ERR);

		if (pbt->ind_table)
		{
			XDL_ASSERT(_lock_bplus_index(pbt, pbi));
		}

		//org nlk insert to new parent
		insert_link_after(&(pbi->lkChild), &(pnew->lkSibling), &(pbd->lkSibling));
		_key_zero(&(pbi->indices[n + 1].key));
		pbi->indices[n + 1].ind = pbd->index;
		pbi->count++;

		if (pbt->ind_table)
		{
			_unlock_bplus_index(pbt, pbi, 1);
		}
	}
	else
	{
		//the splited key add to indices entity
		_insert_bplus_index(ptr, plk, &(pnew->lkSibling), &org);
	}

	return (_key_comp(pkey, &org) < 0) ? &(pnew->lkSibling) : nlk;
}

static int _insert_bplus_data(link_t_ptr ptr, link_t_ptr plk, link_t_ptr* pnlk, key64_t* pkey)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_data_t* pnew;
	bplus_index_t* pbi;
	link_t_ptr nlk = *pnlk;
	int n, m;
	bool_t b_ins = 0;

	pbt = BplusTreeFromLink(ptr);

	if (!plk && !nlk)
	{
		//new data entity as root node
		pnew = _new_bplus_data();
		XDL_ASSERT(_alloc_bplus_data(pbt, pnew));

		nlk = pbt->entity = &pnew->lkSibling;

		if (pbt->ind_table)
		{
			set_file_table_root(pbt->ind_table, (dword_t)pnew->index);
		}
	}
	else if (plk && !nlk)
	{
		//new data entity
		pbi = BplusIndexFromLink(plk);
		n = pbi->count;
		m = (PAGE_SIZE - BPLUS_PAGE_HEADER) / sizeof(bplus_index_entity);
		if (n == m)
		{
			plk = _split_bplus_index(ptr, plk, pkey);
		}
		
		pbi = BplusIndexFromLink(plk);

		pnew = _new_bplus_data();
		XDL_ASSERT(_alloc_bplus_data(pbt, pnew));

		nlk = &pnew->lkSibling;

		if (pbt->ind_table)
		{
			XDL_ASSERT(_lock_bplus_index(pbt, pbi));
		}

		insert_link_after(&(pbi->lkChild), LINK_LAST, nlk);
		_key_zero(&(pbi->indices[pbi->count].key));
		pbi->indices[pbi->count].ind = pnew->index;
		pbi->count++;

		if (pbt->ind_table)
		{
			_unlock_bplus_index(pbt, pbi, 1);
		}
	}
	else if (nlk)
	{
		//load data from persist file
		pbd = BplusDataFromLink(nlk);
		if (pbt->ind_table)
		{
			XDL_ASSERT(_attach_bplus_data(pbt, pbd));
		}
	}

	pbd = BplusDataFromLink(nlk);
	n = pbd->count;
	m = (PAGE_SIZE - BPLUS_PAGE_HEADER) / sizeof(bplus_data_entity);
	//split full data entity, and return the new or org data for key to insert
	if (n == m)
	{
		nlk = _split_bplus_data(ptr, nlk, pkey);
		XDL_ASSERT(nlk != NULL);
	}

	//add the key to data entity
	pbd = BplusDataFromLink(nlk);

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbd));
	}

	n = pbd->count;
	while (n)
	{
		if (_key_comp(&(pbd->dataset[n - 1].key), pkey) <= 0)
			break;

		_key_copy(&(pbd->dataset[n].key), &(pbd->dataset[n - 1].key));
		if (pbt->dat_table)
		{
			pbd->dataset[n].off = pbd->dataset[n - 1].off;
			pbd->dataset[n].len = pbd->dataset[n - 1].len;
		}
		else
		{
			pbd->dataset[n].var = pbd->dataset[n - 1].var;
			pbd->dataset[n].val = pbd->dataset[n - 1].val;
		}
		n--;
	}

	_key_copy(&(pbd->dataset[n].key), pkey);
	if (pbt->dat_table)
	{
		pbd->dataset[n].off = 0;
		pbd->dataset[n].len = 0;
	}
	else
	{
		pbd->dataset[n].var = 0;
		pbd->dataset[n].val = 0;
	}
	pbd->count++;

	//save node to persist file
	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pbd, 1);
	}

	*pnlk = nlk;
	return n;
}

static bool_t _shift_bplus_data(link_t_ptr ptr, link_t_ptr nlk)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_data_t* pbn;
	bplus_index_t* pbi;

	link_t_ptr parent, plk;
	dword_t n, m;

	pbt = BplusTreeFromLink(ptr);

	pbn = BplusDataFromLink(nlk);
	n = pbn->count;
	if (n < 2) return 0;

	//get prior data entity
	plk = get_prev_link(nlk);
	if (!plk) return 0;

#if defined(_DEBUG) || defined(DEBUG)
	XDL_ASSERT(plk->tag == lkBplusData);
#endif

	//if prior data is full and not point to the same parent index, then can not shift
	pbd = BplusDataFromLink(plk);
	//data must be ready
	if (pbt->ind_table)
	{
		XDL_ASSERT(_attach_bplus_data(pbt, pbd));
	}
	n = pbd->count;
	m = (PAGE_SIZE - BPLUS_PAGE_HEADER) / sizeof(bplus_data_entity);
	if (n == m) return 0;

	//must be the same parent
	parent = _get_bplus_data_parent(plk);
	XDL_ASSERT(parent == _get_bplus_data_parent(nlk));

	//find parent entity position
	pbi = BplusIndexFromLink(parent);

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_index(pbt, pbi));
	}

	for (n = 0; n < pbi->count; n++)
	{
		if (pbi->indices[n].ind == pbd->index)
			break;
	}

#if defined(_DEBUG) || defined(DEBUG)
	if (n < pbi->count)
	{
		XDL_ASSERT(pbi->indices[n].ind == pbd->index);
	}
#endif

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbn));
	}

	//replace parent key
	_key_copy(&(pbi->indices[n].key), &(pbn->dataset[1].key));

	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pbn, 0);
	}

	if (pbt->ind_table)
	{
		_unlock_bplus_index(pbt, pbi, 1);
	}

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbd));
	}

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbn));
	}

	//add key to left data entity
	_key_copy(&(pbd->dataset[pbd->count].key), &(pbn->dataset[0].key));
	if (pbt->dat_table)
	{
		pbd->dataset[pbd->count].off = pbn->dataset[0].off;
		pbd->dataset[pbd->count].len = pbn->dataset[0].len;
	}
	else
	{
		pbd->dataset[pbd->count].var = pbn->dataset[0].var;
		pbd->dataset[pbd->count].val = pbn->dataset[0].val;
	}
	pbd->count++;

	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pbd, 1);
	}

	//remove key from right data
	pbn->count--;
	for (n = 0; n < pbn->count; n++)
	{
		_key_copy(&(pbn->dataset[n].key), &(pbn->dataset[n + 1].key));
		if (pbt->dat_table)
		{
			pbn->dataset[n].off = pbn->dataset[n + 1].off;
			pbn->dataset[n].len = pbn->dataset[n + 1].len;
		}
		else
		{
			pbn->dataset[n].var = pbn->dataset[n + 1].var;
			pbn->dataset[n].val = pbn->dataset[n + 1].val;
		}
	}

	_key_zero(&(pbn->dataset[pbn->count].key));
	if (pbt->dat_table)
	{
		pbn->dataset[pbn->count].off = 0;
		pbn->dataset[pbn->count].len = 0;
	}
	else
	{
		pbn->dataset[pbn->count].var = 0;
		pbn->dataset[pbn->count].val = 0;
	}

	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pbn, 1);
	}

	return 1;
}

static void _pop_bplus_data(link_t_ptr ptr, link_t_ptr nlk, key64_t* pkey_del, key64_t* pkey_new)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	link_t_ptr ilk, plk;
	int n, rt;

	pbt = BplusTreeFromLink(ptr);

	plk = _get_bplus_data_parent(nlk);
	while (plk)
	{
		pbi = BplusIndexFromLink(plk);

		if (pbt->ind_table)
		{
			XDL_ASSERT(_lock_bplus_index(pbt, pbi));
		}

		n = pbi->count;
		ilk = _get_bplus_index_last_child(plk);
		while (ilk)
		{
			n--;
			if (ilk == nlk)
				break;

			ilk = _get_bplus_index_prev_sibling(ilk);
		}

		while (n && ilk)
		{
			rt = _key_comp(pkey_del, &(pbi->indices[n - 1].key));
			if (rt == 0)
			{
				_key_copy(&(pbi->indices[n - 1].key), pkey_new);

				if (pbt->ind_table)
				{
					_unlock_bplus_index(pbt, pbi, 1);
				}

				return;
			}
			else if (rt < 0)
				break;

			n--;
			ilk = _get_bplus_index_prev_sibling(ilk);
		}

		if (pbt->ind_table)
		{
			_unlock_bplus_index(pbt, pbi, 0);
		}

		nlk = plk;
		plk = _get_bplus_index_parent(plk);
	}
}

static void _delete_bplus_data(link_t_ptr ptr, link_t_ptr nlk)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	bplus_index_t* pbi;

	link_t_ptr plk, ilk;
	dword_t n, ind;

	pbt = BplusTreeFromLink(ptr);

	pbd = BplusDataFromLink(nlk);

	plk = _get_bplus_data_parent(nlk);
	if (plk)
	{
		pbi = BplusIndexFromLink(plk);
		if (pbt->ind_table)
		{
			XDL_ASSERT(_lock_bplus_index(pbt, pbi));
		}

		n = pbi->count;
		ilk = _get_bplus_index_last_child(plk);
		while (ilk)
		{
			n--;
			if (ilk == &(pbd->lkSibling))
				break;

			ilk = _get_bplus_index_prev_sibling(ilk);
		}

		delete_link(&(pbi->lkChild), ilk);
		pbi->count--;

		//will assert valid position
		ind = pbd->index;

		_free_bplus_data(pbt, pbd);

#if defined(_DEBUG) || defined(DEBUG)
		XDL_ASSERT(pbi->indices[n].ind == ind);
#endif

		_key_zero(&(pbi->indices[n].key));
		pbi->indices[n].ind = 0;

		while (n < pbi->count)
		{
			_key_copy(&(pbi->indices[n].key), &(pbi->indices[n + 1].key));
			pbi->indices[n].ind = pbi->indices[n + 1].ind;
			n++;
		}

		if (pbt->ind_table)
		{
			_unlock_bplus_index(pbt, pbi, 1);
		}

		if (!pbi->count)
		{
			_delete_bplus_index(ptr, plk);
		}
	}
	else
	{
#if defined(_DEBUG) || defined(DEBUG)
		XDL_ASSERT(pbt->entity == nlk);
#endif
		pbt->entity = NULL;

		if (pbt->ind_table)
		{
			set_file_table_root(pbt->ind_table, 0);
		}

		_free_bplus_data(pbt, pbd);
	}
}

static link_t_ptr _find_bplus_entity(link_t_ptr ptr, link_t_ptr *pplk, key64_t* pkey)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_data_t* pbd;

	link_t_ptr plk, ilk;
	dword_t i;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	*pplk = NULL;
	plk = pbt->entity;
	while (plk)
	{
		if (plk->tag == lkBplusIndex)
		{
			pbi = BplusIndexFromLink(plk);
			//let index entity data ready
			if (pbt->ind_table)
			{
				XDL_ASSERT(_attach_bplus_index(pbt, pbi));
			}

			if (pbt->ind_table)
			{
				XDL_ASSERT(_lock_bplus_index(pbt, pbi));
			}

			ilk = get_first_link(&(pbi->lkChild));
			for (i = 0; i < pbi->count; i++)
			{
				if (_key_null(&(pbi->indices[i].key)) || _key_comp(&(pbi->indices[i].key), pkey) > 0)
				{
					break;
				}

				ilk = get_next_link(ilk);
			}

			if (pbt->ind_table)
			{
				_unlock_bplus_index(pbt, pbi, 0);
			}

			*pplk = plk;
			plk = ilk;
		}
		else if (plk->tag == lkBplusData)
		{
			pbd = BplusDataFromLink(plk);
			//let data entity data ready
			if (pbt->ind_table)
			{
				XDL_ASSERT(_attach_bplus_data(pbt, pbd));
			}
			break;
		}
		else
		{
			XDL_ASSERT(0);
		}
	}

	return plk;
}

static bool_t _set_bplus_entity_val(bplus_tree_t* pbt, bplus_data_t* pbd, int n, variant_t var, object_t val)
{
	byte_t* buf = NULL;
	dword_t dw, total = 0;
	dword_t off;
	dword_t dw_val;
	sword_t sw_key;

	res_file_t mh;

	if (pbt->dat_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbd));

		if (pbd->dataset[n].off)
		{
			free_file_table_block(pbt->dat_table, (dword_t)(pbd->dataset[n].off), (dword_t)(pbd->dataset[n].len));

			pbd->dataset[n].off = 0;
			pbd->dataset[n].len = 0;
		}

		sw_key = (sword_t)variant_encode(var, NULL, MAX_LONG);
		dw_val = object_encode(val, NULL, MAX_LONG);
		total = 4 + dw_val + sw_key;

		off = alloc_file_table_block(pbt->dat_table, total);
		XDL_ASSERT(off > 0);

		pbd->dataset[n].off = off;
		pbd->dataset[n].len = total;

		_unlock_bplus_data(pbt, pbd, 1);

		buf = NULL;
		lock_file_table_block(pbt->dat_table, off, total, 1, &mh, (void**)&buf);
		XDL_ASSERT(buf != NULL);

		total = 0;
		//preset the total length
		PUT_DWORD_LOC(buf, 0, (dw_val + sw_key));
		total += 4;

		//key variant
		dw = variant_encode(var, (buf + total), sw_key);
		total += dw;

		//value object
		dw = object_encode(val, (buf + total), dw_val);
		total += dw;

		unlock_file_table_block(pbt->dat_table, off, total, 1, mh, buf);
	}
	else
	{
		if (pbd->dataset[n].var)
			variant_copy((variant_t)(pbd->dataset[n].var), var);
		else
			pbd->dataset[n].var = (key64_t)variant_clone(var);

		if (pbd->dataset[n].val)
			object_copy((object_t)(pbd->dataset[n].val), val);
		else
			pbd->dataset[n].val = (key64_t)object_clone(val);
	}

	return 1;
}

static bool_t _get_bplus_entity_val(bplus_tree_t* pbt, bplus_data_t* pbd, int n, variant_t var, object_t val)
{
	byte_t* buf = NULL;
	dword_t dw, total = 0;
	dword_t off;

	res_file_t mh;

	if (pbt->dat_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbd));

		off = (dword_t)(pbd->dataset[n].off);
		total = (dword_t)(pbd->dataset[n].len);

		_unlock_bplus_data(pbt, pbd, 0);

		XDL_ASSERT(off > 0 && total > 0);

		buf = NULL;
		lock_file_table_block(pbt->dat_table, off, total, 0, &mh, (void**)&buf);
		if (!buf)
		{
			return 0;
		}

		dw = GET_DWORD_LOC(buf, 0);
		if ((total - 4) != dw)
		{
			unlock_file_table_block(pbt->dat_table, off, total, 0, mh, buf);
			return 0;
		}
		total = 4;

		dw = variant_decode(var, (buf + total));
		total += dw;

		dw = object_decode(val, (buf + total));
		total += dw;

		unlock_file_table_block(pbt->dat_table, off, total, 0, mh, buf);
	}
	else
	{
		if (var && pbd->dataset[n].var)
		{
			variant_copy(var, (variant_t)(pbd->dataset[n].var));
		}

		if (val && pbd->dataset[n].val)
		{
			object_copy(val, (object_t)(pbd->dataset[n].val));
		}
	}

	return 1;
}

static void _enum_bplus_entity(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_data_t* pbd;

	link_t_ptr ilk, child;
	link_t_ptr stack;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	stack = NULL;

	ilk = pbt->entity;
	while (ilk)
	{
		if (ilk->tag == lkBplusIndex)
		{
			pbi = BplusIndexFromLink(ilk);
			if (pbt->ind_table)
			{
				_attach_bplus_index(pbt, pbi);
			}
		}
		else if (ilk->tag == lkBplusData)
		{
			pbd = BplusDataFromLink(ilk);
			if (pbt->ind_table)
			{
				_attach_bplus_data(pbt, pbd);
			}

			if (!(*pf)(&(pbd->lkSibling), param))
				goto _CLEAN;
		}
		else
		{
			XDL_ASSERT(0);
		}

		child = (ilk->tag == lkBplusIndex) ? _get_bplus_index_first_child(ilk) : NULL;
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

static bool_t _attach_bplus_index_table(link_t_ptr ptr, link_t_ptr ft)
{
	bplus_tree_t* pbt;
	bplus_index_t* pbi;
	bplus_data_t* pbd;
	int index;
	int tag = -1;

	res_file_t mh;
	byte_t* guider = NULL;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	pbt->ind_table = ft;

	index = get_file_table_root(pbt->ind_table);
	if (!index) return 1;

	lock_file_table_block(pbt->ind_table, index, PAGE_SIZE, 0, &mh, (void**)&guider);
	if (!guider)
		return 0;

	tag = GET_SWORD_LOC(guider, 0);

	unlock_file_table_block(pbt->ind_table, index, PAGE_SIZE, 0, mh, (void*)guider);
	
	if (tag == BPLUS_ENTITY_INDEX)
	{
		pbi = _new_bplus_index(pbt);
		pbi->index = index;

		if (!_attach_bplus_index(pbt, pbi))
		{
			_free_bplus_index(pbt, pbi);
			return 0;
		}

		pbt->entity = &(pbi->lkSibling);
	}
	else if (tag == BPLUS_ENTITY_DATA)
	{
		pbd = _new_bplus_data(pbt);
		pbd->index = index;

		if (!_attach_bplus_data(pbt, pbd))
		{
			_free_bplus_data(pbt, pbd);
			return 0;
		}

		pbt->entity = &(pbd->lkSibling);
	}
	else
	{
		XDL_ASSERT(0);
	}

	return 1;
}

static bool_t _attach_bplus_data_table(link_t_ptr ptr, link_t_ptr ft)
{
	bplus_tree_t* pbt;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	pbt->dat_table = ft;

	return 1;
}

/****************************************************************************************************/

link_t_ptr create_bplus_tree()
{
	bplus_tree_t* pbt;

	pbt = (bplus_tree_t*)xmem_alloc(sizeof(bplus_tree_t));
	pbt->lk.tag = lkBplusTree;

	pbt->entity = NULL;

	return &(pbt->lk);
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

	ilk = pbt->entity;
	while (ilk)
	{
		child = (ilk->tag == lkBplusIndex)? _get_bplus_index_first_child(ilk) : NULL;
		if (child)
		{
			if (!stack)
				stack = create_stack_table();

			push_stack_node(stack, (void*)ilk);

			ilk = child;
			continue;
		}

		parent = (ilk->tag == lkBplusIndex) ? _get_bplus_index_parent(ilk) : _get_bplus_data_parent(ilk);
		if (parent)
		{
			pbi = BplusIndexFromLink(parent);
			delete_link(&(pbi->lkChild), ilk);
		}
		else
		{
			pbt->entity = NULL;
		}

		if (ilk->tag == lkBplusIndex)
		{
			pbi = BplusIndexFromLink(ilk);

			if (pbt->ind_table)
			{
				XDL_ASSERT(pbi->guider == NULL && pbi->indices == NULL);
			}

			while (pbi->count)
			{
				pbi->count--;

				if (pbi->indices)
				{
					pbi->indices[pbi->count].ind = 0;
					_key_zero(&(pbi->indices[pbi->count].key));
				}
			}

			if (pbi->guider)
				xmem_free(pbi->guider);

			xmem_free(pbi);
		}
		else if (ilk->tag == lkBplusData)
		{
			pbd = BplusDataFromLink(ilk);

			if (pbt->ind_table)
			{
				XDL_ASSERT(pbd->guider == NULL && pbd->dataset == NULL);
			}

			while(pbd->count)
			{
				pbd->count--;

				if (pbd->dataset)
				{
					if (!pbt->ind_table && pbd->dataset[pbd->count].var)
						variant_free((variant_t)(pbd->dataset[pbd->count].var));
					pbd->dataset[pbd->count].var = 0;

					if (!pbt->ind_table && pbd->dataset[pbd->count].val)
						object_free((object_t)(pbd->dataset[pbd->count].val));
					pbd->dataset[pbd->count].val = 0;

					_key_zero(&(pbd->dataset[pbd->count].key));
				}
			}
			
			if (pbd->guider)
				xmem_free(pbd->guider);

			xmem_free(pbd);
		}
		else
		{
			XDL_ASSERT(0);
		}

		ilk = (stack) ? (link_t_ptr)pop_stack_node(stack) : NULL;
	}

	if (stack)
		destroy_stack_table(stack);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_ASSERT(pbt->entity == NULL);
#endif
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

bool_t find_bplus_entity(link_t_ptr ptr, variant_t var, object_t val)
{
	key64_t key = { 0 };
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	link_t_ptr nlk, plk = NULL;
	int n;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);
	
	_key_gen(var, &key);

	nlk = _find_bplus_entity(ptr, &plk, &key);
	if (!nlk)
		return 0;

	pbd = BplusDataFromLink(nlk);

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbd));
	}

	n = _find_bplus_data(pbd->dataset, 0, pbd->count - 1, &key);

	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pbd, 0);
	}

	if (n < 0)
		return 0;

	return _get_bplus_entity_val(pbt, pbd, n, NULL, val);
}

bool_t insert_bplus_entity(link_t_ptr ptr, variant_t var, object_t val)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	link_t_ptr nlk, plk = NULL;
	key64_t key = { 0 };
	key64_t org = { 0 };
	int n, m;

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	pbt = BplusTreeFromLink(ptr);

	_key_gen(var, &key);

	nlk = _find_bplus_entity(ptr, &plk, &key);
	if (nlk)
	{
		pbd = BplusDataFromLink(nlk);

		if (pbt->ind_table)
		{
			XDL_ASSERT(_lock_bplus_data(pbt, pbd));
		}

		n = _find_bplus_data(pbd->dataset, 0, pbd->count - 1, &key);
		m = pbd->count;
		_key_copy(&org, &(pbd->dataset[1].key));

		if (pbt->ind_table)
		{
			_unlock_bplus_data(pbt, pbd, 0);
		}

		if (n >= 0)
		{
			return _set_bplus_entity_val(pbt, pbd, n, var, val);
		}
		else
		{
			if (m > 1 && _key_comp(&org, &key) < 0)
			{
				_shift_bplus_data(ptr, nlk);
			}
		}
	}

	n = _insert_bplus_data(ptr, plk, &nlk, &key);
	XDL_ASSERT(n >= 0);

	pbd = BplusDataFromLink(nlk);

	return _set_bplus_entity_val(pbt, pbd, n, var, val);
}

bool_t delete_bplus_entity(link_t_ptr ptr, variant_t var)
{
	bplus_tree_t* pbt;
	bplus_data_t* pbd;
	link_t_ptr nlk, plk = NULL;
	dword_t i;
	int n;
	key64_t key = { 0 };
	key64_t key_del = { 0 };
	key64_t key_new = { 0 };

	XDL_ASSERT(ptr && ptr->tag == lkBplusTree);

	_key_gen(var, &key);

	nlk = _find_bplus_entity(ptr, &plk, &key);

	if (!nlk)
		return 0;

	pbt = BplusTreeFromLink(ptr);
	pbd = BplusDataFromLink(nlk);

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbd));
	}

	n = _find_bplus_data(pbd->dataset, 0, pbd->count - 1, &key);
	if (!n && pbd->count > 1)
	{
		_key_copy(&key_del, &(pbd->dataset[n].key));
		_key_copy(&key_new, &(pbd->dataset[n + 1].key));
	}

	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pbd, 0);
	}

	if (n < 0)
		return 0;

	if (!n && pbd->count > 1)
	{
		_pop_bplus_data(ptr, nlk, &key_del, &key_new);
	}

	if (pbt->ind_table)
	{
		XDL_ASSERT(_lock_bplus_data(pbt, pbd));
	}

	_key_zero(&(pbd->dataset[n].key));
	if (pbt->dat_table)
	{
		if (pbd->dataset[n].off)
		{
			free_file_table_block(pbt->dat_table, (dword_t)(pbd->dataset[n].off), (dword_t)(pbd->dataset[n].len));
			pbd->dataset[n].off = 0;
			pbd->dataset[n].len = 0;
		}
	}
	else
	{
		if (pbd->dataset[n].var)
		{
			variant_free((variant_t)(pbd->dataset[n].var));
			pbd->dataset[n].var = 0;
		}

		if (pbd->dataset[n].val)
		{
			object_free((object_t)(pbd->dataset[n].val));
			pbd->dataset[n].val = 0;
		}
	}

	//remove key
	pbd->count--;
	for (i = n; i < pbd->count; i++)
	{
		_key_copy(&(pbd->dataset[i].key), &(pbd->dataset[i + 1].key));
		if (pbt->dat_table)
		{
			pbd->dataset[i].off = pbd->dataset[i + 1].off;
			pbd->dataset[i].len = pbd->dataset[i + 1].len;
		}
		else
		{
			pbd->dataset[i].var = pbd->dataset[i + 1].var;
			pbd->dataset[i].val = pbd->dataset[i + 1].val;
		}
	}

	_key_zero(&(pbd->dataset[pbd->count].key));
	if (pbt->dat_table)
	{
		pbd->dataset[pbd->count].off = 0;
		pbd->dataset[pbd->count].len = 0;
	}
	else
	{
		pbd->dataset[pbd->count].var = 0;
		pbd->dataset[pbd->count].val = 0;
	}

	if (pbt->ind_table)
	{
		_unlock_bplus_data(pbt, pbd, 1);
	}

	//shift one key from rightside entity
	plk = get_next_link(nlk);
	if (plk)
	{
		_shift_bplus_data(ptr, plk);
	}

	//if data entity is empty then remove it
	pbd = BplusDataFromLink(nlk);
	if (!pbd->count)
	{
		_delete_bplus_data(ptr, nlk);
	}

	return 1;
}

typedef struct _bplus_enum_param{
	link_t_ptr ptr;
	variant_t key;
	object_t val;
	CALLBACK_ENUMBPLUSENTITY pf;
	void* pa;
}bplus_enum_param;

static bool_t _enum_data_node(link_t_ptr nlk, void* pa)
{
	bplus_enum_param* pp = (bplus_enum_param*)pa;

	bplus_tree_t* pbt = BplusTreeFromLink(pp->ptr);
	bplus_data_t* pbd = BplusDataFromLink(nlk);

	dword_t i;

	for (i = 0; i < pbd->count; i++)
	{
		variant_to_null(pp->key, VV_NULL);
		object_empty(pp->val);

		_get_bplus_entity_val(pbt, pbd, i, pp->key, pp->val);

		if (!(*(pp->pf))(pp->key, pp->val, pp->pa))
			return 0;
	}

	return 1;
}

void enum_bplus_entity(link_t_ptr ptr, CALLBACK_ENUMBPLUSENTITY pf, void* param)
{
	bplus_enum_param bp = { 0 };

	bp.ptr = ptr;
	bp.key = variant_alloc(VV_NULL);
	bp.val = object_alloc();
	bp.pf = pf;
	bp.pa = param;

	_enum_bplus_entity(ptr, _enum_data_node, (void*)&bp);

	variant_free(bp.key);
	object_free(bp.val);
}

link_t_ptr create_bplus_file_table(link_t_ptr index_table, link_t_ptr data_table)
{
	link_t_ptr ptr;
	
	ptr = create_bplus_tree();

	if (!_attach_bplus_index_table(ptr, index_table))
	{
		destroy_bplus_tree(ptr);
		return NULL;
	}

	if (!_attach_bplus_data_table(ptr, data_table))
	{
		destroy_bplus_tree(ptr);
		return NULL;
	}

	return ptr;
}

#if defined(XDL_SUPPORT_TEST)
#include <time.h>

static bool_t print_node(link_t_ptr nlk, void* pa)
{
	bplus_tree_t* pbt = (bplus_tree_t*)pa;
	bplus_data_t* pbd = BplusDataFromLink(nlk);
	bplus_index_t* pbi;
	dword_t i,n;
	key64_t key;
	key64_t org;

	variant_t var = variant_alloc(VV_NULL);
	object_t val = object_alloc();
	_key_zero(&org);

	_tprintf(_T("{"));
	for (n = 0; n < pbd->count; n++)
	{
		_get_bplus_entity_val(pbt, pbd, n, var, val);
		_key_gen(var, &key);

		printf("%lu ", key);

		XDL_ASSERT(key > 0);
		XDL_ASSERT(_key_comp(&key, &org) > 0);
	
		_key_copy(&org, &key);
		variant_to_null(var, VV_NULL);
	}
	_tprintf(_T("}"));


	key64_t* pkey;
	link_t_ptr plk = _get_bplus_data_parent(nlk);
	while (plk)
	{
		pbi = BplusIndexFromLink(plk);

		if (pbt->ind_table)
		{
			_lock_bplus_index(pbt, pbi);
		}

		_tprintf(_T("("));
		for (i = 0; i < pbi->count; i++)
		{
			pkey = &(pbi->indices[i].key);

			if (pbd->index == pbi->indices[i].ind)
				_tprintf(_T("["));

			if (pbd->index == pbi->indices[i].ind)
				printf("%lu] ", *pkey);
			else
				printf("%lu ", *pkey);
		}
		_tprintf(_T(")"));

		if (pbt->ind_table)
		{
			_unlock_bplus_index(pbt, pbi, 0);
		}

		plk = _get_bplus_index_parent(plk);
	}

	variant_free(var);
	object_free(val);

	_tprintf(_T("\n"));

	return 1;
}

void test_bplus_tree_none_table()
{
	//tchar_t numset[] = _T("27 16 48 40 48 32 33 48 48 11 31 29 33 47 11 9 1 46 42 8 35 15 1 15 2 33 2 32 49 36 37 38 0 50 51 80 70 83 91 85 96 82 43 20 24 11 12 46 32 11 46 22 11 45 9 17 39 0 44 30 14 18 21 75 71 65 61 83 77 97 66 78 60 63 91 81 92 84");
	tchar_t numset[] = _T("136 82 188 91 130 20 27 152 84 182 42 5 29 78 180 32 173 107 10 191 164 83 139 1 99 100 87 172 181 46 69 35 5 8 69 170 24 118 9 79 126 6 5 174 56 171 44 56 175 124 1 73 133 144 115 34 172 160 108 198 54 119 133 74 193 76 172 188 176 29 51 76 64 9 57 132 37 125 91 196 91 111 184 199 64 114 58 150 14 38 7");// 23 167 51 151 138 121 194 9 175 1");

	link_t_ptr ptr = create_bplus_tree();

	variant_t v = variant_alloc(VV_INT);

	object_t val = object_alloc();

	key64_t org = { 0 };

	tchar_t* key;
	int len;
	int n, total = 0;
	while (n = parse_string_token((numset + total), -1, _T(' '), &key, &len))
	{
		total += n;
		n = xsntol(key, len);
		variant_set_int(v, n);
		_tprintf(_T("INS %d: "),n);

		object_set_variant(val, v);
		insert_bplus_entity(ptr, v, val);
	}

	_key_zero(&org);

	_enum_bplus_entity(ptr, print_node, (void*)BplusTreeFromLink(ptr));

	destroy_bplus_tree(ptr);

	variant_free(v);
	object_free(val);

	///////////////////////////////////
	ptr = create_bplus_tree();

	v = variant_alloc(VV_STRING_UTF8);
	val = object_alloc();
	variant_t v2 = variant_alloc(VV_STRING_UTF8);

	tchar_t str[NUM_LEN + 1];
	int i;
	bool_t rt;
	n = 100000;
	for (i = 0; i < n; i++)
	{
		xsprintf(str, _T("key%d"), i);
		variant_from_string(v, str, -1);
		object_set_variant(val, v);

		insert_bplus_entity(ptr, v, val);
	}

	for (i = 0; i < n; i++)
	{
		xsprintf(str, _T("key%d"), i);
		variant_from_string(v, str, -1);
		object_set_variant(val, v);

		rt = find_bplus_entity(ptr, v, val);
		if (!rt)
		{
			_tprintf(_T("missing %s\n"), str);
		}
		else
		{
			object_get_variant(val, v2);
			XDL_ASSERT(variant_comp(v, v2) == 0);
		}
	}

	_tprintf(_T("end\n"), str);

	variant_free(v2);
	variant_free(v);
	object_free(val);
	destroy_bplus_tree(ptr);
}

void test_bplus_tree_file_table(const tchar_t* tname, dword_t tmask)
{
	tchar_t iname[PATH_LEN + 1] = { 0 };
	tchar_t dname[PATH_LEN + 1] = { 0 };

	xsprintf(iname, _T("%s.ind"), tname);
	xsprintf(dname, _T("%s.dat"), tname);

	int i, j = 10;
	while (j--)
	{
		link_t_ptr pt_index = create_file_table(iname, BLOCK_SIZE_4096, tmask);
		link_t_ptr pt_data = create_file_table(dname, BLOCK_SIZE_512, tmask);

		link_t_ptr ptr = create_bplus_file_table(pt_index, pt_data);

		Srand48((int)time(NULL));

		variant_t v = variant_alloc(VV_INT);

		object_t val = object_alloc();

		int n = 1000;
		int m;

		for (i = 0; i < n; i++)
		{
			while ((m = Lrand48() % n) == 0);
			//_tprintf(_T("04d "), m);
			variant_set_int(v, m);

			object_set_variant(val, v);
			insert_bplus_entity(ptr, v, val);
		}

		_tprintf(_T("\n"));

		_enum_bplus_entity(ptr, print_node, (void*)BplusTreeFromLink(ptr));

		Srand48((int)time(NULL));

		for (i = 0; i < n; i++)
		{
			while ((m = Lrand48() % n) == 0);
			//_tprintf(_T("04d "), m);
			variant_set_int(v, m);

			delete_bplus_entity(ptr, v);
		}

		_tprintf(_T("\n"));

		variant_free(v);
		object_free(val);

		_enum_bplus_entity(ptr, print_node, (void*)BplusTreeFromLink(ptr));

		destroy_file_table(pt_index);

		destroy_file_table(pt_data);

		destroy_bplus_tree(ptr);
	}
}
#endif