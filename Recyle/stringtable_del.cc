/*************************************************************
	Easily xdl v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	stringtable.cc | string table implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

/************************************************************
*************************************************************/

#include "stringtable.h"
#include "xdlstr.h"
#include "xdlmem.h"

typedef struct _string_table{
	link_t lk;			/* str table self link component*/
	link_t lkEntitys;		/* str table master root link ptr array*/
}string_table_t;

typedef struct _string_entity_t{
	link_t lk;			/* entity self link component*/
	unsigned long data;	/* entiry data for xmem_free use*/
	wchar_t* wkey; /* entity key, can not be null*/
	wchar_t* wval; /* entity value*/
}string_entity_t;

/* restore str table struct ptr from link ptr*/
#define StringTableFromLink(p) TypePtrFromLink(string_table_t,p)
/* restore str entity struct ptr from link ptr*/
#define StringEntityFromLink(p) TypePtrFromLink(string_entity_t,p)

int string_table_sort(link_t_ptr plk1,link_t_ptr plk2,void* param)
{
	string_entity_t *pse1,*pse2;

	pse1 = StringEntityFromLink(plk1);
	pse2 = StringEntityFromLink(plk2);
	return xscmp(pse1->wkey,pse2->wkey);
}

link_t_ptr create_string_table()
{
	string_table_t* pht;
	
	pht = (string_table_t*)xmem_alloc(sizeof(string_table_t));
	pht->lk.tag = lkStringTable;
	init_root_link(&pht->lkEntitys);

	return &(pht->lk);
}

void destroy_string_table(link_t_ptr ptr)
{
	string_table_t* pht;

	assert(ptr);
	assert(ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	clear_string_table(ptr);

	xmem_free(pht);
}

void clear_string_table(link_t_ptr ptr)
{
	string_table_t* pht;
	link_t_ptr plk;

	assert(ptr);
	assert(ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	while(plk = get_string_next_entity(ptr,LINK_FIRST))
		delete_string_entity(ptr,plk);
}

void sort_string_table(link_t_ptr ptr,int desc)
{
	string_table_t* pht;

	assert(ptr);
	assert(ptr->tag == lkStringTable);
	pht = StringTableFromLink(ptr);
	bubble_sort_link(&pht->lkEntitys,(link_sort_call)string_table_sort,desc,NULL);
}

int get_string_entity_count(link_t_ptr ptr)
{
	string_table_t* pht;

	assert(ptr);
	assert(ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	return get_link_count(&pht->lkEntitys);
}

link_t_ptr write_string_entity_attr(link_t_ptr ptr,const wchar_t* key,int keylen,const wchar_t* val,int vallen)
{
	string_table_t* pht;
	string_entity_t* phe;
	link_t_ptr plk;

	assert(ptr);
	assert(ptr->tag == lkStringTable);

	if(is_null(key) || !keylen)
		return NULL;

	pht = StringTableFromLink(ptr);
	//first to find entity with the key
	plk = get_string_entity(ptr,key,keylen);
	if(plk != NULL)		//if exist then to replace the entiry value
	{
		phe = StringEntityFromLink(plk);
		
		if(!is_null(val) && vallen)
		{
			if(vallen < 0)
				vallen = xslen(val);
			phe->wval = (wchar_t*)xmem_realloc(phe->wval,(vallen + 1) * sizeof(wchar_t));
			xsncpy(phe->wval,val,vallen);
		}else if(phe->wval != NULL)
		{
			xmem_free(phe->wval);
			phe->wval = NULL;
		}
	}else	//if not exist then to add new entity with key and value
	{	
		phe = (string_entity_t*)xmem_alloc(sizeof(string_entity_t));
		phe->lk.tag = lkStringEntity;
		
		if(keylen < 0)
			keylen = xslen(key);
		
		phe->wkey = (wchar_t*)xmem_alloc((keylen + 1) * sizeof(wchar_t));
		xsncpy(phe->wkey,key,keylen);
		
		if(val && vallen)
		{
			if(vallen < 0)
				vallen = xslen(val);
			phe->wval = (wchar_t*)xmem_alloc((vallen + 1) * sizeof(wchar_t));
			xsncpy(phe->wval,val,vallen);
		}
		
		insert_link_after(&pht->lkEntitys,LINK_LAST,&phe->lk);
	}

	return &(phe->lk);
}

int read_string_entity_attr(link_t_ptr ptr,const wchar_t* key,int keylen,wchar_t* buf,int max)
{
	link_t_ptr ent;

	assert(ptr);
	assert(ptr->tag == lkStringTable);
	assert(buf && max >= 0);

	ent = get_string_entity(ptr,key,keylen);
	if(ent)
	{
		return get_string_entity_val(ent,buf,max);
	}else
	{
		buf[0] = L'\0';
		return 0;
	}
}

const wchar_t* get_string_entity_attr_ptr(link_t_ptr ptr,const wchar_t* key,int keylen)
{
	link_t_ptr ent;

	assert(ptr);
	assert(ptr->tag == lkStringTable);

	ent = get_string_entity(ptr,key,keylen);
	if(ent)
	{
		return get_string_entity_val_ptr(ent);
	}else
	{
		return NULL;
	}
}

link_t_ptr	get_string_entity(link_t_ptr ptr,const wchar_t* key,int keylen)
{
	int rt;
	string_entity_t* phe;
	string_table_t* pht;
	link_t_ptr plk;

	assert(ptr);
	assert(ptr->tag == lkStringTable);

	if(is_null(key) || !keylen)
		return NULL;

	pht = StringTableFromLink(ptr);

	if(keylen < 0)
		keylen = xslen(key);

	/*then to compare the entity key in the ordered list*/
	plk = get_string_next_entity(ptr,LINK_FIRST);
	while(plk)
	{
		phe = StringEntityFromLink(plk);
		rt = xsncmp(phe->wkey,key,keylen);
		if(rt == 0 && (int)xslen(phe->wkey) == keylen)
			return plk;
	
		plk = get_string_next_entity(ptr,plk);
	}

	return NULL;
}

void delete_string_entity(link_t_ptr ptr,link_t_ptr elk)
{
	string_entity_t* phe;
	string_table_t* pst;

	assert(ptr && ptr->tag == lkStringTable);
	assert(elk && elk->tag == lkStringEntity);

	pst = StringTableFromLink(ptr);
	elk = delete_link(&pst->lkEntitys,elk);
	assert(elk);

	phe = StringEntityFromLink(elk);

	xmem_free(phe->wkey);
	if(phe->wval)
		xmem_free(phe->wval);
	xmem_free(phe);
}

const wchar_t* get_string_entity_key_ptr(link_t_ptr elk)
{
	string_entity_t* phe;

	assert(elk);
	assert(elk->tag == lkStringEntity);
	phe = StringEntityFromLink(elk);
	return phe->wkey;
}

const wchar_t* get_string_entity_val_ptr(link_t_ptr elk)
{
	string_entity_t* phe;

	assert(elk);
	assert(elk->tag == lkStringEntity);
	phe = StringEntityFromLink(elk);
	return phe->wval;
}

int get_string_entity_val_len(link_t_ptr elk)
{
	string_entity_t* phe;

	assert(elk);
	assert(elk->tag == lkStringEntity);
	phe = StringEntityFromLink(elk);
	if(phe->wval)
		return xslen(phe->wval);
	else
		return 0;
}

int get_string_entity_key(link_t_ptr elk,wchar_t* buf,int max)
{
	string_entity_t* phe;
	int len;

	assert(elk);
	assert(elk->tag == lkStringEntity);
	assert(buf && max >= 0);

	phe = StringEntityFromLink(elk);
	len = xslen(phe->wkey);
	len = (len < max)? len : max;
	xsncpy(buf,phe->wkey,len);
	buf[len] = L'\0';

	return len;
}

int get_string_entity_val(link_t_ptr elk,wchar_t* buf,int max)
{
	string_entity_t* phe;
	int len;

	assert(elk);
	assert(elk->tag == lkStringEntity);
	assert(buf && max >= 0);

	phe = StringEntityFromLink(elk);
	if(phe->wval == NULL)
	{
		buf[0] = L'\0';
		return 0;
	}else
	{
		len = xslen(phe->wval);
		len = (len < max)? len : max;
		xsncpy(buf,phe->wval,len);
		buf[len] = L'\0';
		return len;
	}
}

void set_string_entity_val(link_t_ptr elk,const wchar_t* val,int len)
{
	string_entity_t* phe;

	assert(elk);
	assert(elk->tag == lkStringEntity);
	
	phe = StringEntityFromLink(elk);
	
	if(!is_null(val) && len)
	{
		if(len < 0)
			len = xslen(val);
		phe->wval = (wchar_t*)xmem_alloc((len + 1) * sizeof(wchar_t));
		xsncpy(phe->wval,val,len);
	}else if(phe->wval)
	{
		xmem_free(phe->wval);
		phe->wval = NULL;
	}
}

void set_string_entity_data(link_t_ptr elk,unsigned long data)
{
	string_entity_t* phe;

	assert(elk);
	assert(elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);
	phe->data = data;
}

unsigned long get_string_entity_data(link_t_ptr elk)
{
	string_entity_t* phe;

	assert(elk);
	assert(elk->tag == lkStringEntity);

	phe = StringEntityFromLink(elk);
	return phe->data;
}

link_t_ptr get_string_next_entity(link_t_ptr ptr,link_t_ptr pos)
{
	string_table_t* pst;
	assert(ptr && ptr->tag == lkStringTable);
	pst = StringTableFromLink(ptr);
	if(pos == LINK_FIRST)
		return get_first_link(&pst->lkEntitys);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_string_prev_entity(link_t_ptr ptr,link_t_ptr pos)
{
	string_table_t* pst;
	assert(ptr && ptr->tag == lkStringTable);
	pst = StringTableFromLink(ptr);
	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_last_link(&pst->lkEntitys);
	else
		return get_prev_link(pos);
}

link_t_ptr get_string_entity_at(link_t_ptr ptr, int index)
{
	link_t_ptr ilk;

	if (index < 0)
		return NULL;

	ilk = get_string_next_entity(ptr, LINK_FIRST);
	while (ilk && index--)
		ilk = get_string_next_entity(ptr, ilk);

	return ilk;
}

int get_string_entity_index(link_t_ptr ptr, link_t_ptr tlk)
{
	link_t_ptr plk;
	int index = -1;

	plk = get_string_next_entity(ptr, LINK_FIRST);
	while (plk)
	{
		index++;
		if (plk == tlk)
			break;
		plk = get_string_next_entity(ptr, tlk);
	}

	return index;
}

void string_table_parse_options(link_t_ptr ptr,const wchar_t* options,int len, wchar_t itemfeed,wchar_t linefeed)
{
	wchar_t* token;
	wchar_t *key,*val;
	int keylen,vallen;

	assert(ptr && ptr->tag == lkStringTable);

	clear_string_table(ptr);

	if(is_null(options) || !len)
		return;

	if(len < 0)
		len = xslen(options);

	token = (wchar_t*)options;
	while(*token && len)
	{
		key = token;
		while(*token != itemfeed && *token != L'\0')
		{
			token ++;
			len --;
		}
		keylen = (int)(token - key);

		if(token == L'\0')
			break;

		token ++; /*skip item feed*/
		len --;
		
		val = token;
		while(*token != linefeed && *token != L'\0')
		{
			token ++;
			len --;
		}
		vallen = (int)(token - val);

		write_string_entity_attr(ptr,key,keylen,val,vallen);

		if(*token == L'\0')
			break;

		token ++; /*skip line feed*/
		len --;
	}
}

int string_table_format_options_length(link_t_ptr ptr,wchar_t itemfeed,wchar_t linefeed)
{
	return string_table_format_options(ptr,NULL,MAX_LONG,itemfeed,linefeed);
}

int string_table_format_options(link_t_ptr ptr,wchar_t* buf,int max,wchar_t itemfeed,wchar_t linefeed)
{
	string_table_t* pht;
	string_entity_t* phe;
	link_t_ptr plk;
	int total,len;
	
	assert(ptr);
	assert(ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	total = 0;
	
	plk = get_string_next_entity(ptr,LINK_FIRST);
	while(plk)
	{
		phe = StringEntityFromLink(plk);
		
		len = xslen(phe->wkey) + ((phe->wval)? xslen(phe->wval) : 0) + 2; /*include itemfeed and linefeed*/
		if(total + len > max)
			return -1;
		if(buf)
			xsprintf(buf + total,L"%s%c%s%c",phe->wkey,itemfeed,((phe->wval)? phe->wval : L""),linefeed);
		total += len;
		
		plk = get_string_next_entity(ptr,plk);
	}
	return total;
}

void string_table_parse_attrset(link_t_ptr ptr,const wchar_t* attrset,int len)
{
	wchar_t* token;
	wchar_t *key,*val;
	int keylen,vallen;

	assert(ptr && ptr->tag == lkStringTable);

	clear_string_table(ptr);

	if(is_null(attrset) || !len)
		return;

	if(len < 0)
		len = xslen(attrset);

	token = (wchar_t*)attrset;
	while(*token && len)
	{
		key = token;
		while(*token != L' ' && *token != L'\t' && *token != L'=' && *token != L'\0')
		{
			token ++;
			len --;
		}
		keylen = (int)(token - key);

		if(token == L'\0')
			break;

		//skip ' ','='
		while(*token != L'\'' && *token != L'\"' && *token != L'\0')
		{
			token ++;
			len --;
		}

		if(token == L'\0')
			break;

		//skip '\'','\"'
		token ++;
		len --;
		
		val = token;
		while(*token != L'\'' && *token != L'\"' && *token != L'\0')
		{
			token ++;
			len --;
		}
		vallen = (int)(token - val);

		write_string_entity_attr(ptr,key,keylen,val,vallen);

		//skip ' ','\'','\"'
		while(*token == L'\'' || *token == L'\"' || *token == L' ' || *token == L'\t')
		{
			token ++;
			len --;
		}

		if(*token == L'\0')
			break;
	}
}

int string_table_format_attrset_length(link_t_ptr ptr)
{
	return string_table_format_attrset(ptr,NULL,MAX_LONG);
}

int string_table_format_attrset(link_t_ptr ptr,wchar_t* buf,int max)
{
	string_table_t* pht;
	string_entity_t* phe;
	link_t_ptr plk;
	int total,len;
	
	assert(ptr);
	assert(ptr->tag == lkStringTable);

	pht = StringTableFromLink(ptr);
	total = 0;
	
	plk = get_string_next_entity(ptr,LINK_FIRST);
	while(plk)
	{
		phe = StringEntityFromLink(plk);
		
		len = xslen(phe->wkey) + ((phe->wval)? xslen(phe->wval) : 0) + 5; /*include two quaute and one '=' and one ' '*/
		if(total + len > max)
			return -1;
		if(buf)
			xsprintf(buf + total,L"%s=%c%s%c ",phe->wkey,L'\"',((phe->wval)? phe->wval : L""),L'\"');
		total += len;
		
		plk = get_string_next_entity(ptr,plk);
	}
	return total;
}