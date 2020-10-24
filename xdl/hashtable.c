/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hashtable document

	@module	hashtable.c | implement file

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

#include "hashtable.h"
#include "optparser.h"

#include "xdlimp.h"
#include "xdlstd.h"

typedef struct _hash_entity_t{
	link_t lk;				/* entity self link component*/
	unsigned short index;	/* entity key index*/
	tchar_t* wkey;			/* entity name, can not be null*/
	union{
		tchar_t* wval;		/* entity value*/
		void* vval;
	};
	dword_t vlen;	/* entity value bytes*/
	var_long data;		/* entiry data for runtime use*/
}hash_entity_t;

typedef struct _hash_table_t{
	link_t lk;			/* hash table self link component*/
	link_t* pp;		/* hash table master root link ptr array*/
	int size;			/* root link ptr array size, is prim value*/
}hash_table_t;

/* restore hash table struct ptr from link ptr*/
#define HashTableFromLink(p) TypePtrFromLink(hash_table_t,p) 
/* restore hash entity struct ptr from link ptr*/
#define HashEntityFromLink(p) TypePtrFromLink(hash_entity_t,p)

#define PRIM_TABLE_SIZE	6
static int PRIM_TABLE[PRIM_TABLE_SIZE] = { 3, 7, 13, 29, 53, 97 };

#define HASHINDEX(n,prim)	((int)(n % prim))
#define KEYINDEX(n)			((unsigned short)(n & 0x0000FFFF))

static int _next_prim(int prim)
{
	int i;

	for (i = 0; i < PRIM_TABLE_SIZE - 1; i++)
	{
		if (prim < PRIM_TABLE[i])
			break;
	}

	return PRIM_TABLE[i];
}

static dword_t hash_code(unsigned char* key,int keylen)
{
	dword_t sum,ge;
	
	if (!key)
		return 0;

	sum = 0;
	
	while(*key && keylen)
	{
		sum = (sum << 4) + (*key);
		if((ge = (sum & 0xF0000000))) 
		{
			sum = sum ^ (ge >> 24);
			sum = sum ^ ge;
		}
		key ++;
		keylen --;
	}
	return sum;
}

static int _insert_hash_entity(link_t_ptr root,link_t_ptr elk)
{
	hash_entity_t* pnew;
	hash_entity_t* phe;
	link_t_ptr plk;
	int count = 0;
	
	XDL_ASSERT(elk && elk->tag == lkHashEntity);
	
	pnew = HashEntityFromLink(elk);
	plk = get_first_link(root);
	while(plk)
	{
		phe = HashEntityFromLink(plk);
		if(phe->index >= pnew->index)
			break;/*find the prev link*/
		
		count++;
		plk = get_next_link(plk);
	}
	
	while (plk)
	{
		phe = HashEntityFromLink(plk);
		if (phe->index > pnew->index)
			break;

		if (compare_text(phe->wkey, -1, pnew->wkey, -1, 0) > 0)
			break;

		count++;
		plk = get_next_link(plk);
	}

	if(plk == NULL)
	{
		insert_link_after(root,LINK_LAST,elk);
	}else
	{
		insert_link_before(root,plk,elk);
	}

	while (elk)
	{
		count++;
		elk = get_next_link(elk);
	}

	return count;
}

static void _expand_hash_table(hash_table_t* pht)
{
	hash_entity_t* phe;
	link_t_ptr plk, pnew;
	int i, index, prim;
	dword_t code;

	prim = _next_prim(pht->size);
	if (prim == pht->size)
		return;

	pnew = (link_t*)xmem_alloc(prim * sizeof(link_t));
	for (i = 0; i < prim; i++)
		init_root_link(&(pnew[i]));

	for (i = 0; i<pht->size; i++)
	{
		while (plk = delete_link(&((pht->pp)[i]), LINK_FIRST))
		{
			phe = HashEntityFromLink(plk);

			code = hash_code((unsigned char*)phe->wkey, xslen(phe->wkey) * sizeof(tchar_t));
			index = HASHINDEX(code, prim);

			_insert_hash_entity(&(pnew[index]), plk);
		}
	}

	xmem_free(pht->pp);
	pht->pp = pnew;
	pht->size = prim;
}

link_t_ptr create_hash_table()
{
	hash_table_t* pht;
	int i;

	pht = (hash_table_t*)xmem_alloc(sizeof(hash_table_t));
	pht->lk.tag = lkHashTable;
	pht->lk.next = NULL;
	pht->lk.prev = NULL;
	pht->size = PRIM_TABLE[0];

	pht->pp = (link_t_ptr)xmem_alloc(pht->size * sizeof(link_t));
	for (i = 0; i<pht->size; i++)
		init_root_link(pht->pp + i);	/*initialize each master root link ptr*/

	return &(pht->lk);
}

void destroy_hash_table(link_t_ptr ptr)
{
	hash_table_t* pht;

	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	clear_hash_table(ptr);	/*free all hash eitity*/

	pht = HashTableFromLink(ptr);

	xmem_free(pht->pp);
	xmem_free(pht);
}

void clear_hash_table(link_t_ptr ptr)
{
	hash_table_t* pht;
	hash_entity_t* phe;
	link_t_ptr plk;
	int i;

	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	pht = HashTableFromLink(ptr);

	for (i = 0; i<pht->size; i++)
	{
		while (plk = delete_link(&(pht->pp[i]), LINK_FIRST))
		{
			phe = HashEntityFromLink(plk);

			xmem_free(phe->wkey);

			if (phe->wval)
				xmem_free(phe->wval);

			xmem_free(phe);
		}
	}
}

void copy_hash_table(link_t_ptr ptr_dest,link_t_ptr ptr_src)
{
	hash_table_t* pht;
	hash_entity_t* phe;
	link_t_ptr plk, elk;
	int i;
	
	XDL_ASSERT(ptr_dest && ptr_dest->tag == lkHashTable);
	XDL_ASSERT(ptr_src && ptr_src->tag == lkHashTable);

	pht = HashTableFromLink(ptr_src);
	for(i=0;i<pht->size;i++)
	{
		plk = get_first_link(&((pht->pp)[i]));
		while(plk != NULL)
		{
			phe = HashEntityFromLink(plk);

			elk = write_hash_bytes(ptr_dest, phe->wkey, -1, (byte_t*)phe->vval, phe->vlen);

			plk = get_next_link(plk);
		}
	}
}

int get_hash_entity_count(link_t_ptr ptr)
{
	hash_table_t* pht;
	link_t_ptr plk;
	int i,count = 0;

	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	pht = HashTableFromLink(ptr);

	for(i=0;i<pht->size;i++)
	{
		plk = get_first_link(&((pht->pp)[i]));
		while(plk != NULL)
		{
			count ++;
			plk = get_next_link(plk);
		}
	}

	return count;
}

link_t_ptr get_hash_entity_at(link_t_ptr ptr,int index)
{
	hash_table_t* pht;
	link_t_ptr plk;
	int i,count = 0;

	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	pht = HashTableFromLink(ptr);

	for(i=0;i<pht->size;i++)
	{
		plk = get_first_link(&((pht->pp)[i]));
		while(plk != NULL)
		{
			if(count == index)
				return plk;

			count ++;
			plk = get_next_link(plk);
		}
	}
	return NULL;
}

int get_hash_entity_index(link_t_ptr ptr,link_t_ptr elk)
{
	hash_table_t* pht;
	link_t_ptr plk;
	int i,index = 0;

	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	pht = HashTableFromLink(ptr);

	for(i=0;i<pht->size;i++)
	{
		plk = get_first_link(&((pht->pp)[i]));
		while(plk != NULL)
		{
			if(plk == elk)
				return index;

			index ++;
			plk = get_next_link(plk);
		}
	}
	return -1;
}

link_t_ptr _hash_first_entity(hash_enum_t* pea)
{
	hash_table_t* pht;
	link_t_ptr plk;
	int i,count = 0;

	pht = HashTableFromLink(pea->hash);

	for(i=0;i<pht->size;i++)
	{
		plk = get_first_link(&((pht->pp)[i]));
		if(plk != NULL)
		{
			pea->index = i;
			pea->entity = plk;
			return pea->entity;
		}
	}
	return NULL;
}

link_t_ptr _hash_last_entity(hash_enum_t* pea)
{
	hash_table_t* pht;
	link_t_ptr plk;
	int i, count = 0;

	pht = HashTableFromLink(pea->hash);

	for (i = pht->size - 1; i >= 0; i--)
	{
		plk = get_last_link(&((pht->pp)[i]));
		if (plk != NULL)
		{
			pea->index = i;
			pea->entity = plk;
			return pea->entity;
		}
	}
	return NULL;
}

link_t_ptr get_hash_next_entity(hash_enum_t* pea)
{
	hash_table_t* pht;
	link_t_ptr plk;
	int i,count = 0;

	XDL_ASSERT(pea->hash && pea->hash->tag == lkHashTable);

	if(pea->entity == LINK_FIRST)
		return _hash_first_entity(pea);
	else if (pea->entity == LINK_LAST)
		return NULL;

	plk = get_next_link(pea->entity);
	if(plk)
	{
		pea->entity = plk;
		return pea->entity;
	}

	pht = HashTableFromLink(pea->hash);

	for(i=pea->index + 1;i<pht->size;i++)
	{
		plk = get_first_link(&((pht->pp)[i]));
		if(plk)
		{
			pea->index = i;
			pea->entity = plk;
			return pea->entity;
		}
	}
	return NULL;
}

link_t_ptr get_hash_prev_entity(hash_enum_t* pea)
{
	hash_table_t* pht;
	link_t_ptr plk;
	int i, count = 0;

	XDL_ASSERT(pea->hash && pea->hash->tag == lkHashTable);

	if (pea->entity == LINK_FIRST)
		return NULL;
	else if (pea->entity == LINK_LAST)
		return _hash_last_entity(pea);

	plk = get_prev_link(pea->entity);
	if (plk)
	{
		pea->entity = plk;
		return pea->entity;
	}

	pht = HashTableFromLink(pea->hash);

	for (i = pea->index - 1; i >= 0; i--)
	{
		plk = get_last_link(&((pht->pp)[i]));
		if (plk)
		{
			pea->index = i;
			pea->entity = plk;
			return pea->entity;
		}
	}
	return NULL;
}

link_t_ptr	write_hash_attr(link_t_ptr ptr,const tchar_t* key,int keylen,const tchar_t* val,int vallen)
{
	hash_table_t* pht;
	hash_entity_t* phe;
	link_t_ptr plk;
	dword_t code;
	int count;

	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	if(is_null(key) || !keylen)
		return NULL;

	pht = HashTableFromLink(ptr);
	/*first to find entity with the key*/
	plk = get_hash_entity(ptr,key,keylen);
	if(plk != NULL)		/*if exist then to replace the entiry value*/
	{
		phe = HashEntityFromLink(plk);
		
		if(!is_null(val) && vallen)
		{
			if(vallen < 0)
				vallen = xslen(val);

			phe->wval = (tchar_t*)xmem_realloc(phe->wval,(vallen + 1) * sizeof(tchar_t));
			xsncpy(phe->wval,val,vallen);
			phe->vlen = (vallen + 1) * sizeof(tchar_t);
		}else if(phe->wval)
		{
			xmem_free(phe->wval);
			phe->wval = NULL;
			phe->vlen = 0;
		}
	}else	/*if not exist then to add new entity with key and value*/
	{	
		if(keylen == -1)
			keylen = xslen(key);

		code = hash_code((unsigned char*)key,keylen * sizeof(tchar_t));

		phe = (hash_entity_t*)xmem_alloc(sizeof(hash_entity_t));
		phe->lk.tag = lkHashEntity;
		
		phe->wkey = (tchar_t*)xmem_alloc((keylen + 1) * sizeof(tchar_t));
		xsncpy(phe->wkey,key,keylen);
		phe->index = KEYINDEX(code);
		
		if(!is_null(val) && vallen)
		{
			if(vallen < 0)
				vallen = xslen(val);
			phe->wval = (tchar_t*)xmem_alloc((vallen + 1) * sizeof(tchar_t));
			xsncpy(phe->wval,val,vallen);
			phe->vlen = (vallen + 1) * sizeof(tchar_t);
		}
		
		plk = &((pht->pp)[HASHINDEX(code,pht->size)]);
		
		/*insert entity to list by key's asc order*/
		count = _insert_hash_entity(plk,&phe->lk);
		if (count > pht->size)
		{
			_expand_hash_table(pht);
		}
	}

	return &(phe->lk);
}

int read_hash_attr(link_t_ptr ptr,const tchar_t* key,int keylen,tchar_t* buf,int max)
{
	link_t_ptr ent;

	ent = get_hash_entity(ptr,key,keylen);
	if (!ent)
	{
		if (buf)
			buf[0] = _T('\0');
		return 0;
	}
	else
	{
		return get_hash_entity_val(ent, buf, max);
	}
}

const tchar_t* get_hash_attr_ptr(link_t_ptr ptr,const tchar_t* key,int keylen)
{
	link_t_ptr ent;
	hash_entity_t* phe;

	ent = get_hash_entity(ptr,key,keylen);
	if(ent == NULL)
	{
		return NULL;
	}else
	{
		phe = HashEntityFromLink(ent);

		return phe->wval;
	}
}

int get_hash_attr_len(link_t_ptr ptr, const tchar_t* key, int keylen)
{
	link_t_ptr ent;
	hash_entity_t* phe;

	ent = get_hash_entity(ptr, key, keylen);
	if (ent == NULL)
	{
		return 0;
	}
	else
	{
		phe = HashEntityFromLink(ent);

		if (!phe->vlen)
			return 0;
		else
			return (phe->vlen) / sizeof(tchar_t) - 1;
	}
}

link_t_ptr get_hash_entity(link_t_ptr ptr,const tchar_t* key,int keylen)
{
	dword_t code;
	hash_entity_t* phe;
	hash_table_t* pht;
	link_t_ptr plk;
	int rt;

	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	if(is_null(key) || keylen == 0)
		return NULL;

	if (keylen < 0)
		keylen = xslen(key);

	pht = HashTableFromLink(ptr);

	/*first to calc master root link ptr array position by the key's hash code */
	code = hash_code((unsigned char*)key,keylen * sizeof(tchar_t));
	/*then to compare the entity key in the ordered list*/
	plk = get_first_link(&((pht->pp)[HASHINDEX(code,pht->size)]));
	while(plk != NULL)
	{
		phe = HashEntityFromLink(plk);
		if(phe->index == KEYINDEX(code))
		{
			rt = compare_text(phe->wkey, -1, key, keylen, 0);
			if (rt == 0)
				return plk;
			else if (rt > 0)
				return NULL;
		}
		else if (phe->index > KEYINDEX(code))
		{
			return NULL;
		}
	
		plk = get_next_link(plk);
	}

	return NULL;
}

void delete_hash_entity(link_t_ptr elk)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);
	/*delete link ptr from list*/
	delete_link(NULL,elk);

	xmem_free(phe->wkey);
	if(phe->wval)
		xmem_free(phe->wval);
	xmem_free(phe);
}

void delete_hash_attr(link_t_ptr ptr,const tchar_t* key,int keylen)
{
	link_t_ptr elk;

	elk = get_hash_entity(ptr,key,keylen);
	if (elk)
	{
		delete_hash_entity(elk);
	}
}

const tchar_t* get_hash_entity_key_ptr(link_t_ptr elk)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);
	return (tchar_t*)phe->wkey;
}

const tchar_t* get_hash_entity_val_ptr(link_t_ptr elk)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);
	return (tchar_t*)phe->wval;
}

int get_hash_entity_key(link_t_ptr elk,tchar_t* buf,int max)
{
	hash_entity_t* phe;
	int len;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);
	len = xslen(phe->wkey);
	len = (len < max)? len : max;

	if (buf)
		xsncpy(buf,phe->wkey,len);

	return len;
}

int get_hash_entity_val(link_t_ptr elk,tchar_t* buf,int max)
{
	hash_entity_t* phe;
	int len;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);

	len = (phe->vlen)? ((phe->vlen / sizeof(tchar_t)) - 1) : 0;
	len = (len < max) ? len : max;

	if (buf)
		xsncpy(buf, phe->wval, len);

	return len;
}

void attach_hash_entity_val(link_t_ptr elk, tchar_t* val)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);

	if (phe->wval)
		xmem_free(phe->wval);

	if (is_null(val))
	{
		phe->wval = NULL;
		phe->vlen = 0;
	}
	else
	{
		phe->wval = val;
		phe->vlen = (xslen(val) + 1) * sizeof(tchar_t);
	}
}

tchar_t* detach_hash_entity_val(link_t_ptr elk)
{
	hash_entity_t* phe;
	tchar_t* val;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);
	val = phe->wval;
	phe->wval = NULL;
	phe->vlen = 0;

	return val;
}

link_t_ptr set_hash_attr_boolean(link_t_ptr ptr,const tchar_t* key,bool_t s)
{
	return write_hash_attr(ptr,key,-1,((s)? _T("1") : _T("0")),-1);
}

bool_t get_hash_attr_boolean(link_t_ptr ptr,const tchar_t* key)
{
	tchar_t token[2] = {0};

	read_hash_attr(ptr,key,-1,token,1);
	return xstol(token)? 1 : 0;
}

link_t_ptr set_hash_attr_integer(link_t_ptr ptr,const tchar_t* key,int s)
{
	tchar_t token[NUM_LEN + 1] = {0};

	xsprintf(token,_T("%d"),s);
	return write_hash_attr(ptr,key,-1,token,-1);
}

int get_hash_attr_integer(link_t_ptr ptr,const tchar_t* key)
{
	tchar_t token[NUM_LEN + 1] = {0};

	read_hash_attr(ptr,key,-1,token,NUM_LEN);
	return xstol(token);
}

link_t_ptr set_hash_attr_float(link_t_ptr ptr, const tchar_t* key, float s)
{
	tchar_t token[NUM_LEN + 1] = { 0 };

	ftoxs(s, token, NUM_LEN);
	return write_hash_attr(ptr, key, -1, token, -1);
}

float get_hash_attr_float(link_t_ptr ptr, const tchar_t* key)
{
	tchar_t token[NUM_LEN + 1] = { 0 };

	read_hash_attr(ptr, key, -1, token, NUM_LEN);

	return xstof(token);
}

link_t_ptr set_hash_attr_numeric(link_t_ptr ptr,const tchar_t* key,double s)
{
	tchar_t token[NUM_LEN + 1] = {0};

	numtoxs(s, token, NUM_LEN);

	return write_hash_attr(ptr,key,-1,token,-1);
}

double get_hash_attr_numeric(link_t_ptr ptr,const tchar_t* key)
{
	tchar_t token[NUM_LEN + 1] = {0};

	read_hash_attr(ptr,key,-1,token,NUM_LEN);

	return xstonum(token);
}

link_t_ptr write_hash_bytes(link_t_ptr ptr, const tchar_t* key, int keylen, const byte_t* val, dword_t vallen)
{
	hash_entity_t* phe;
	link_t_ptr elk;

	elk = write_hash_attr(ptr, key, keylen, NULL, 0);
	if (!elk)
		return NULL;

	phe = HashEntityFromLink(elk);

	phe->vval = xmem_realloc(phe->vval, vallen);
	xmem_copy(phe->vval, (void*)val, vallen);
	phe->vlen = vallen;

	return elk;
}

dword_t read_hash_bytes(link_t_ptr ptr, const tchar_t* key, int keylen, byte_t* buf, dword_t max)
{
	hash_entity_t* phe;
	link_t_ptr ent;

	ent = get_hash_entity(ptr, key, keylen);
	if (!ent)
		return 0;

	phe = HashEntityFromLink(ent);

	max = (max < phe->vlen) ? max : phe->vlen;
	if (buf)
	{
		xmem_copy((void*)buf, phe->vval, max);
	}

	return max;
}

const byte_t* get_hash_bytes_ptr(link_t_ptr ptr, const tchar_t* key, int keylen)
{
	hash_entity_t* phe;
	link_t_ptr ent;

	ent = get_hash_entity(ptr, key, keylen);
	if (!ent)
		return 0;

	phe = HashEntityFromLink(ent);

	return (byte_t*)phe->vval;
}

dword_t get_hash_bytes_size(link_t_ptr ptr, const tchar_t* key, int keylen)
{
	link_t_ptr ent;
	hash_entity_t* phe;

	ent = get_hash_entity(ptr, key, keylen);
	if (ent == NULL)
	{
		return 0;
	}
	else
	{
		phe = HashEntityFromLink(ent);

		return phe->vlen;
	}
}

void attach_hash_entity_bytes(link_t_ptr elk, byte_t* val, dword_t len)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);

	if (phe->vval)
		xmem_free(phe->vval);

	phe->vval = (void*)val;
	phe->vlen = len;
}

byte_t* detach_hash_entity_bytes(link_t_ptr elk)
{
	hash_entity_t* phe;
	byte_t* val;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);

	val = (byte_t*)phe->wval;
	phe->vval = NULL;
	phe->vlen = 0;

	return val;
}

const byte_t* get_hash_entity_bytes_ptr(link_t_ptr elk)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);

	return (byte_t*)phe->vlen;
}

dword_t get_hash_entity_bytes_size(link_t_ptr elk)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);

	return phe->vlen;
}

void set_hash_entity_delta(link_t_ptr elk,var_long data)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);
	phe->data = data;
}

var_long get_hash_entity_delta(link_t_ptr elk)
{
	hash_entity_t* phe;

	XDL_ASSERT(elk && elk->tag == lkHashEntity);

	phe = HashEntityFromLink(elk);
	return phe->data;
}

int enum_hash_entity(link_t_ptr ptr,CALLBACK_ENUMLINK pf,void* pv)
{
	hash_table_t* pht;
	hash_entity_t* phe;
	link_t_ptr plk;
	int i,count;
	
	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	pht = HashTableFromLink(ptr);
	count = 0;
	for(i=0;i<pht->size;i++)
	{
		plk = get_first_link(&((pht->pp)[i]));
		while(plk != NULL)
		{
			phe = HashEntityFromLink(plk);
			count ++;

			if(pf == NULL)
			{
				plk = get_next_link(plk);
				continue;
			}
			
			if(!(*pf)(plk,pv))
				return -1;

			plk = get_next_link(plk);
		}
	}
	return count;
}

const tchar_t* hash_attr_set_chars(link_t_ptr ptr, const tchar_t* key, int pos, const tchar_t* pch, int n)
{
	hash_entity_t* phe;
	link_t_ptr plk;
	int len;

	if (is_null(key))
		return NULL;

	plk = get_hash_entity(ptr, key, -1);
	if (!plk)
		plk = write_hash_attr(ptr, key, -1, NULL, 0);

	phe = HashEntityFromLink(plk);
	
	len = xslen(phe->wval);

	if (pos < 0)
		pos = len;

	if (pos > len)
		return phe->wval;

	if (n < 0)
		n = xslen(pch);

	if (pos + n > len)
		phe->wval = xsrealloc(phe->wval, pos + n - len + 1);

	xsnset(phe->wval, pos, pch, n);

	return phe->wval;
}

const tchar_t* hash_attr_ins_chars(link_t_ptr ptr, const tchar_t* key, int pos, const tchar_t* pch, int n)
{
	hash_entity_t* phe;
	link_t_ptr plk;
	int len;

	if (is_null(key))
		return NULL;

	plk = get_hash_entity(ptr, key, -1);
	if (!plk)
		plk = write_hash_attr(ptr, key, -1, NULL, 0);

	phe = HashEntityFromLink(plk);

	len = xslen(phe->wval);

	if (pos < 0)
		pos = len;

	if (pos > len)
		return phe->wval;

	if (n < 0)
		n = xslen(pch);

	if (n)
	{
		phe->wval = xsrealloc(phe->wval, n + len + 1);
		xsnins(phe->wval, pos, pch, n);
	}

	return phe->wval;
}

const tchar_t* hash_attr_del_chars(link_t_ptr ptr, const tchar_t* key, int pos, int n)
{
	hash_entity_t* phe;
	link_t_ptr plk;
	int len;

	if (is_null(key))
		return NULL;

	plk = get_hash_entity(ptr, key, -1);
	if (!plk)
		return NULL;

	phe = HashEntityFromLink(plk);

	len = xslen(phe->wval);

	if (pos < 0 || pos >= len)
		return phe->wval;

	xsndel(phe->wval, pos, n);

	return phe->wval;
}

void hash_table_parse_attrset(link_t_ptr ptr, const tchar_t* attrs, int len)
{
	tchar_t *key, *val;
	int keylen, vallen;
	int n, total = 0;

	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	clear_hash_table(ptr);

	if (len < 0)
		len = xslen(attrs);

	if (is_null(attrs) || !len)
		return;

	while (n = parse_attrset_token((attrs + total), (len - total), &key, &keylen, &val, &vallen))
	{
		total += n;

		write_hash_attr(ptr, key, keylen, val, vallen);
	}
}

int hash_table_format_attrset(link_t_ptr ptr,tchar_t* buf,int max)
{
	hash_table_t* pht;
	hash_entity_t* phe;
	link_t_ptr plk;
	int i,total,len;
	
	XDL_ASSERT(ptr && ptr->tag == lkHashTable);

	pht = HashTableFromLink(ptr);
	total = 0;

	for(i=0;i<pht->size;i++)
	{
		plk = get_first_link(&((pht->pp)[i]));
		while(plk != NULL)
		{
			phe = HashEntityFromLink(plk);

			len = xslen(phe->wkey) + ((phe->wval)? xslen(phe->wval) : 0) + 4; /*include asign and two quator and blank*/
			if(total + len > max)
				return total;

			if(buf)
				xsprintf(buf + total,_T("%s%c%c%s%c "),phe->wkey,_T('='),_T('\"'),((phe->wval)? phe->wval : _T("")),_T('\"'));
			total += len;

			plk = get_next_link(plk);
		}
	}

	return total;
}

int hash_table_compare_attr(const tchar_t* key,const tchar_t* sin,const tchar_t* val,void* parm)
{
	link_t_ptr ptr;
	link_t_ptr ent;
	const tchar_t* str;
	int rt,vallen;

	ptr = (link_t_ptr)parm;
	
	if(is_null(key))
		return 0;

	ent = get_hash_entity(ptr,key,-1);
	if(ent == NULL)
		return 0;
	
	str = get_hash_entity_val_ptr(ent);
	
	if(!xscmp(sin,_T(">")))
	{
		rt = xscmp(str,val);
		rt = (rt > 0)? 1 : 0;
	}else if(!xscmp(sin,_T(">=")))
	{
		rt = xscmp(str,val);
		rt = (rt >= 0)? 1 : 0;
	}else if(!xscmp(sin,_T("<")))
	{
		rt = xscmp(str,val);
		rt = (rt < 0)? 1 : 0;
	}else if(!xscmp(sin,_T("<=")))
	{
		rt = xscmp(str,val);
		rt = (rt <= 0)? 1 : 0;
	}else if(!xscmp(sin,_T("==")))
	{
		rt = xscmp(str,val);
		rt = (rt == 0)? 1 : 0;
	}else if(!xscmp(sin,_T("<>")))
	{
		rt = xscmp(str,val);
		rt = (rt != 0)? 1 : 0;
	}else if(!xscmp(sin,_T("=")))
	{
		if(str == NULL && val == NULL)
			rt = 1;
		else if(str == NULL && val)
			rt = 0;
		else if(str && val == NULL)
			rt = 1;
		else
		{
			vallen = (val)? xslen(val) : 0;
			if(!xsncmp(str,val,vallen))
				rt = 1;
			else
				rt = 0;
		}
	}
	return rt;
}

static bool_t _on_parse_token(void* pp, const tchar_t* key, int klen, const tchar_t* val, int vlen)
{
	link_t_ptr ht = (link_t_ptr)pp;

	write_hash_attr(ht, key, klen, val, vlen);

	return 1;
}

static bool_t _on_format_token(void* fp, const tchar_t** pkey, int* pklen, const tchar_t** pval, int* pvlen)
{
	hash_enum_t* phe = (hash_enum_t*)fp;
	link_t_ptr ent;

	ent = get_hash_next_entity(phe);
	if (!ent)
		return 0;

	*pkey = get_hash_entity_key_ptr(ent);
	*pklen = xslen(*pkey);

	*pval = get_hash_entity_val_ptr(ent);
	*pvlen = xslen(*pval);

	return 1;
}

int hash_table_parse_options(link_t_ptr ptr, const tchar_t* str, int len, tchar_t itemfeed, tchar_t linefeed)
{
	return parse_options(str, len, itemfeed, linefeed, (void*)ptr, _on_parse_token);
}

int hash_table_format_options(link_t_ptr ptr, tchar_t* buf, int max, tchar_t itemfeed, tchar_t linefeed)
{
	hash_enum_t he = { 0 };
	he.hash = ptr;
	he.entity = LINK_FIRST;

	return format_options(buf, max, itemfeed, linefeed, (void*)&he, _on_format_token);
}


#if defined(_DEBUG) || defined(DEBUG)
void test_hash_table()
{
	uint32_t i,total = 0;
	int min,max,count, zero = 0;
	link_t_ptr ptr;
	hash_table_t* pht;

	ptr = create_hash_table();

	for (i = 0x4E00; i <= 0x9FA5; i++)
	{
		write_hash_attr(ptr, (tchar_t*)&i, 1, NULL, 0);
	}

	pht = HashTableFromLink(ptr);
	min = pht->size;
	max = 0;

	for (i = 0; i < pht->size; i++)
	{
		count = get_link_count(&((pht->pp)[i]));
		printf("entity size is:%d\n", count);
		total += count;
		if (!count)
			zero++;
		if (min > count)
			min = count;
		if (max < count)
			max = count;
	}

	printf("table size is:%d\n", pht->size);
	printf("zero eintities is:%d\n", zero);
	printf("max eintities is:%d\n", max);
	printf("min eintities is:%d\n", min);
	printf("total entities is:%d\n", total);

	destroy_hash_table(ptr);
}
#endif