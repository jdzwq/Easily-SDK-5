/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dom document

	@module	domdoc.c | implement file

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

#include "domdoc.h"
#include "xmlattr.h"
#include "miscell.h"

#include "xdlimp.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_DOC

typedef struct _dom_node_t{
	link_t lkSibling;		// the sibling node link component also as self link component
	link_t lkChild;			// the child node link component 
	int count;				// child node count

	dword_t mask;			// runtime dom node mask
	var_long delta;			// runtime user value

	tchar_t* ns;			// xml namespace name
	tchar_t* name;			// node name
	tchar_t* text;			// node text

	link_t_ptr attr;		// hash table for node to properties 
	link_t_ptr xmlns;		// string table for namespace list
	
	link_t_ptr opti;		// runtime string table for options	

	int pages;				// runtime page guid
	page_cator_t* pcator;
	int points;				//runtime line guid
	dword_t* pointer;
}dom_node_t;

/*restore domnode from self link ptr*/
#define DomItemFromLink(p) TypePtrFromSibling(dom_node_t,p)
/*restore domnode from child link ptr*/
#define DomItemFromChild(p) TypePtrFromChild(dom_node_t,p)

typedef struct _token_param_t{
	const tchar_t* token;
	int len;
}token_param_t;

typedef struct _entity_param_t{
	const tchar_t *key,*val;
	int klen,vlen;
}entity_param_t;

dom_node_t* _alloc_dom_node()
{
	dom_node_t* pti;

	pti = (dom_node_t*)xmem_alloc(sizeof(dom_node_t));
	pti->lkSibling.tag = lkNode;
	pti->lkSibling.next = NULL;
	pti->lkSibling.prev = NULL;
	init_root_link(&pti->lkChild);

	pti->attr = create_hash_table();

	set_dom_node_name(&(pti->lkSibling),DOC_UNKNOWN,-1);

	return pti;
}

void _free_dom_node(dom_node_t* ptt)
{
	if (ptt->opti)
		destroy_string_table(ptt->opti);

	if (ptt->xmlns)
		destroy_string_table(ptt->xmlns);

	if (ptt->ns)
		xmem_free(ptt->ns);

	if (ptt->name)
		xmem_free(ptt->name);

	if (ptt->text)
		xmem_free(ptt->text);

	if (ptt->attr)
		destroy_hash_table(ptt->attr);

	if (ptt->pcator)
		xmem_free(ptt->pcator);

	if (ptt->pointer)
		xmem_free(ptt->pointer);

	xmem_free(ptt);
}

link_t_ptr create_dom_doc()
{
	dom_node_t* pti;

	pti = _alloc_dom_node();

	return &(pti->lkSibling);
}

void destroy_dom_doc(link_t_ptr ptr)
{
	XDL_ASSERT(is_dom_doc_node(ptr));

	delete_dom_node(ptr);
}


link_t_ptr enum_dom_node(link_t_ptr ptr,CALLBACK_ENUMLINK pf,void* param)
{
	link_t_ptr nlk;
	link_t_ptr st = NULL;

	if(!pf)
		return NULL;

	nlk = get_dom_first_child_node(ptr);
	while(nlk)
	{
		if(!(*pf)(nlk,param))
			break;

		if(get_dom_first_child_node(nlk))
		{
			if(!st)
				st = create_stack_table();
			push_stack_node(st,(void*)nlk);
			nlk = get_dom_first_child_node(nlk);
			continue;
		}

		while(nlk)
		{
			nlk = get_dom_next_sibling_node(nlk);
			if(nlk)
				break;
			nlk = (st)? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}
	
	if(st)
		destroy_stack_table(st);

	return nlk;
}

link_t_ptr enum_dom_node_deep(link_t_ptr ptr, CALLBACK_ENUMLINK pf, void* param)
{
	link_t_ptr nlk;
	link_t_ptr st = NULL;
	bool_t b = 1;

	if (!pf)
		return NULL;

	nlk = get_dom_first_child_node(ptr);
	while (b && nlk)
	{
		if (get_dom_first_child_node(nlk))
		{
			if (!st)
				st = create_stack_table();
			push_stack_node(st, (void*)nlk);
			nlk = get_dom_first_child_node(nlk);
			continue;
		}

		if (!(*pf)(nlk, param))
			break;

		while (nlk)
		{
			nlk = get_dom_next_sibling_node(nlk);
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

bool_t is_dom_doc_node(link_t_ptr ptr)
{
	dom_node_t* pti;

	if (!ptr)
		return 0;

	if (ptr->tag != lkNode)
		return 0;

	pti = DomItemFromLink(ptr);

	return (bool_t)(pti->lkSibling.next == NULL && pti->lkSibling.prev == NULL);
}

static bool_t _compare_dom_node_proc(link_t_ptr plk, void* pv)
{
	return (plk == (link_t_ptr)pv)? 0 : 1;
}

bool_t is_dom_child_node(link_t_ptr ilk,link_t_ptr plk)
{
	if (!plk)
		return 0;

	return (enum_dom_node(ilk, (CALLBACK_ENUMLINK)_compare_dom_node_proc, (void*)plk)) ? 1 : 0;
}

bool_t is_dom_node_attr(link_t_ptr ilk,const tchar_t* key,int len)
{
	return (NULL == get_hash_entity(get_dom_node_attr_table(ilk),key,len) )? 0 : 1;
}

link_t_ptr get_dom_from_node(link_t_ptr ilk)
{
	link_t_ptr ptr;

	ptr = ilk;
	do{
		if(is_dom_doc_node(ptr))
			break;
		ptr = get_dom_parent_node(ptr);
	}while(ptr);

	return ptr;
}

link_t_ptr get_dom_node_attr_table(link_t_ptr ilk)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	return pti->attr;
}

link_t_ptr insert_dom_node_before(link_t_ptr ilk, link_t_ptr pos)
{
	dom_node_t* ptt;
	dom_node_t* pup;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = _alloc_dom_node();

	pup = DomItemFromLink(ilk);
	insert_link_before(&(pup->lkChild), pos, &(ptt->lkSibling));
	pup->count++;

	return &(ptt->lkSibling);
}

link_t_ptr insert_dom_node(link_t_ptr ilk,link_t_ptr pos)
{
	dom_node_t* ptt;
	dom_node_t* pup;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = _alloc_dom_node();

	pup = DomItemFromLink(ilk);
	insert_link_after(&(pup->lkChild),pos,&(ptt->lkSibling));
	pup->count ++;

	return &(ptt->lkSibling);
}

void delete_dom_node(link_t_ptr ilk)
{
	link_t_ptr parent,child;
	dom_node_t* ptt;
	dom_node_t* pup;
	link_t_ptr stack;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	stack = NULL;

	while(ilk)
	{
		child = get_dom_first_child_node(ilk);
		if(child)
		{
			if(!stack)
				stack = create_stack_table();

			push_stack_node(stack,(void*)ilk);

			ilk = child;
			continue;
		}

		parent = get_dom_parent_node(ilk);
		if(parent)
		{
			/* if exist, delete node link from parent node */
			pup = DomItemFromLink(parent);
			delete_link(&(pup->lkChild),ilk);
			pup->count --;
		}

		ptt = DomItemFromLink(ilk);
		_free_dom_node(ptt);

		ilk = (stack)? (link_t_ptr)pop_stack_node(stack) : NULL;
	}

	if(stack)
		destroy_stack_table(stack);
}

void delete_dom_child_nodes(link_t_ptr ilk)
{
	link_t_ptr plk;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	while(plk = get_dom_first_child_node(ilk))
		delete_dom_node(plk);
}

link_t_ptr get_dom_child_node_root(link_t_ptr ilk)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	return &pti->lkChild;
}

void attach_dom_node(link_t_ptr ilk,link_t_ptr pos,link_t_ptr plk)
{
	dom_node_t *ptt,*pup;

	XDL_ASSERT(ilk && ilk->tag == lkNode);
	XDL_ASSERT(plk && plk->tag == lkNode);

	ptt = DomItemFromLink(plk);
	pup = DomItemFromLink(ilk);
	insert_link_after(&(pup->lkChild), pos, &(ptt->lkSibling));
	pup->count ++;
}

link_t_ptr detach_dom_node(link_t_ptr ilk,link_t_ptr plk)
{
	dom_node_t *ptt, *pup;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pup = DomItemFromLink(ilk);
	plk = delete_link(&(pup->lkChild),plk);
	if (!plk)
		return NULL;

	pup->count--;

	ptt = DomItemFromLink(plk);
	ptt->lkSibling.next = NULL;
	ptt->lkSibling.prev = NULL;

	return plk;
}

void set_dom_node_name(link_t_ptr ilk,const tchar_t* sz_name,int len)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	if (len < 0)
		len = xslen(sz_name);

	if(len)
	{
		pti->name = (tchar_t*)xmem_realloc(pti->name,(len + 1) * sizeof(tchar_t));
		xsncpy(pti->name,sz_name,len);
	}else
	{
		xmem_free(pti->name);
		pti->name = NULL;
	}
}

const tchar_t* get_dom_node_name_ptr(link_t_ptr ilk)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	return pti->name;
}

void set_dom_node_text(link_t_ptr ilk,const tchar_t* sz_text,int len)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	if(len < 0)
		len = (sz_text)? xslen(sz_text) : 0;

	if(len)
	{
		pti->text = xsrealloc(pti->text, len + 1);
		xsncpy(pti->text,sz_text,len);
	}else
	{
		xmem_free(pti->text);
		pti->text =  NULL;
	}
}

const tchar_t* get_dom_node_text_ptr(link_t_ptr ilk)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	return pti->text;
}

int get_dom_node_text(link_t_ptr ilk,tchar_t* buf,int max)
{
	dom_node_t* pti;
	int len;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	
	len = xslen(pti->text);
	len = (len < max)? len : max;

	if (buf)
		xsncpy(buf,pti->text,len);

	return len;
}

void attach_dom_node_attr(link_t_ptr ilk, const tchar_t* key, tchar_t* val)
{
	link_t_ptr ent, tbl;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	tbl = get_dom_node_attr_table(ilk);

	XDL_ASSERT(tbl != NULL);

	ent = write_hash_attr(tbl, key, -1, NULL, 0);

	attach_hash_entity_val(ent, val);
}

tchar_t* detach_dom_node_attr(link_t_ptr ilk, const tchar_t* key)
{
	link_t_ptr ent, tbl;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	tbl = get_dom_node_attr_table(ilk);

	XDL_ASSERT(tbl != NULL);

	ent = get_hash_entity(tbl, key, -1);

	return (ent) ? detach_hash_entity_val(ent) : NULL;
}

void attach_dom_node_text(link_t_ptr ilk, tchar_t* sz_text)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	xmem_free(pti->text);
	pti->text = sz_text;
}

tchar_t* detach_dom_node_text(link_t_ptr ilk)
{
	dom_node_t* pti;
	tchar_t* str;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	str = pti->text;

	pti->text = NULL;

	return str;
}

int dom_node_text_get_chars(link_t_ptr ilk, int pos, tchar_t* pch, int n)
{
	dom_node_t* pti;
	int len;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	if (!pti->text)
		return 0;

	if (pch)
	{
		xszero(pch, n + 1);
	}

	len = xslen(pti->text);

	if (pos < 0 || pos >= len)
		return 0;

	n = (n < len - pos) ? n : (len - pos);

	if (pch)
	{
		xsncpy(pch, pti->text + pos, n);
	}

	return n;
}

const tchar_t* dom_node_text_set_chars(link_t_ptr ilk, int pos, const tchar_t* pch, int n)
{
	dom_node_t* pti;
	int len;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	len = xslen(pti->text);

	if (pos < 0)
		pos = len;

	if (pos > len)
		return pti->text;

	if (n < 0)
		n = xslen(pch);

	if (pos + n > len)
		pti->text = xsrealloc(pti->text, pos + n - len + 1);

	xsnset(pti->text, pos, pch, n);

	return pti->text;
}

const tchar_t* dom_node_text_ins_chars(link_t_ptr ilk, int pos, const tchar_t* pch, int n)
{
	dom_node_t* pti;
	int len;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	len = xslen(pti->text);

	if (pos < 0)
		pos = len;

	if (pos > len)
		return pti->text;

	if (n < 0)
		n = xslen(pch);

	if (n)
	{
		pti->text = xsrealloc(pti->text, n + len + 1);
		xsnins(pti->text, pos, pch, n);
	}

	return pti->text;
}

const tchar_t* dom_node_text_del_chars(link_t_ptr ilk, int pos, int n)
{
	dom_node_t* pti;
	int len;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	len = xslen(pti->text);

	if (pos < 0 || pos >= len)
		return pti->text;

	xsndel(pti->text, pos, n);

	return pti->text;
}

const tchar_t* dom_node_attr_set_chars(link_t_ptr ilk, const tchar_t* attr, int pos, const tchar_t* pch, int n)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	return hash_attr_set_chars(pti->attr, attr, pos, pch, n);
}

const tchar_t* dom_node_attr_ins_chars(link_t_ptr ilk, const tchar_t* attr, int pos, const tchar_t* pch, int n)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	return hash_attr_ins_chars(pti->attr, attr, pos, pch, n);
}

const tchar_t* dom_node_attr_del_chars(link_t_ptr ilk, const tchar_t* attr, int pos, int n)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	return hash_attr_del_chars(pti->attr, attr, pos, n);
}

void set_dom_node_ns(link_t_ptr ilk, const tchar_t* sz_ns, int len)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	if (len < 0)
		len = (sz_ns) ? xslen(sz_ns) : 0;

	if (len)
	{
		pti->ns = (tchar_t*)xmem_realloc(pti->ns, (len + 1) * sizeof(tchar_t));
		xsncpy(pti->ns, sz_ns, len);
	}
	else
	{
		xmem_free(pti->ns);
		pti->ns = NULL;
	}
}

const tchar_t* get_dom_node_ns_ptr(link_t_ptr ilk)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	return pti->ns;
}

void set_dom_node_nsurl(link_t_ptr ilk, const tchar_t* sz_url, int len)
{
	dom_node_t* pti;
	tchar_t nkey[PATH_LEN + 1];

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	if (!pti->xmlns)
		pti->xmlns = create_string_table(0);

	if (is_null(pti->ns))
		xsprintf(nkey, _T("%s"), XMLNS);
	else
		xsprintf(nkey, _T("%s:%s"), XMLNS, pti->ns);

	write_string_entity(pti->xmlns, nkey, -1, sz_url, len);
}

const tchar_t* get_dom_node_nsurl_ptr(link_t_ptr ilk)
{
	dom_node_t* pti;
	tchar_t nkey[PATH_LEN + 1];

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	if (!pti->xmlns)
		return NULL;

	if (is_null(pti->ns))
		xsprintf(nkey, _T("%s"), XMLNS);
	else
		xsprintf(nkey, _T("%s:%s"), XMLNS, pti->ns);

	return get_string_entity_ptr(pti->xmlns, nkey, -1);
}

link_t_ptr get_dom_node_name_table(link_t_ptr ilk)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	return pti->xmlns;
}

void set_dom_node_xmlns(link_t_ptr ilk, const tchar_t* nsname, int klen, const tchar_t* nsurl, int vlen)
{
	dom_node_t* pti;
	link_t_ptr slk;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);

	if (is_null(nsname) || !klen)
		return;

	if (is_null(nsurl))
		vlen = 0;

	if (vlen < 0)
		vlen = xslen(nsurl);

	slk = (pti->xmlns)? get_string_entity(pti->xmlns, nsname, klen) : NULL;
	if (slk && !vlen)
	{
		delete_string_entity(pti->xmlns, slk);

		if (get_string_entity_count(pti->xmlns) == 0)
		{
			destroy_string_table(pti->xmlns);
			pti->xmlns = NULL;
		}
		return;
	}

	if (vlen)
	{
		if (!pti->xmlns)
			pti->xmlns = create_string_table(0);

		write_string_entity(pti->xmlns, nsname, klen, nsurl, vlen);
	}
}

int get_dom_node_xmlns(link_t_ptr ilk, const tchar_t* nsname, int len, tchar_t* buf, int max)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	if (!pti->xmlns)
		return 0;

	return read_string_entity(pti->xmlns, nsname, len, buf, max);
}


void set_dom_node_mask(link_t_ptr ilk,dword_t mask)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	pti->mask = mask;
}

dword_t get_dom_node_mask(link_t_ptr ilk)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	return pti->mask;
}

void set_dom_node_mask_check(link_t_ptr ilk,dword_t ul,bool_t b)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	if(b)
		pti->mask |= ul;
	else
		pti->mask &= (~ul);
}

bool_t get_dom_node_mask_check(link_t_ptr ilk,dword_t ul)
{
	dom_node_t* pti;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	pti = DomItemFromLink(ilk);
	
	return (pti->mask & ul)? 1 : 0;
}

typedef struct _NODE_MASK_COUNT{
	dword_t mask;
	int count;
}NODE_MASK_COUNT;

static bool_t _count_check_node(link_t_ptr plk, void* pv)
{
	NODE_MASK_COUNT* pn = (NODE_MASK_COUNT*)pv;

	if (get_dom_node_mask_check(plk, pn->mask))
		pn->count++;

	return 1;
}

int get_dom_child_node_mask_check_count(link_t_ptr ilk,dword_t ul)
{
	NODE_MASK_COUNT nd = { 0 };

	nd.mask = ul;
	enum_dom_node(ilk, (CALLBACK_ENUMLINK)_count_check_node, (void*)&nd);

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

	if (get_dom_node_mask_check(plk, pn->mask) != pn->check)
		pn->count++;

	set_dom_node_mask_check(plk, pn->mask, pn->check);

	return 1;
}

int set_dom_child_node_mask_check(link_t_ptr ilk,dword_t ul, bool_t b)
{
	NODE_MASK_CHECK nd = { 0 };

	nd.mask = ul;
	nd.check = b;
	enum_dom_node(ilk, (CALLBACK_ENUMLINK)_reset_check_node, (void*)&nd);

	return nd.count;
}

var_long get_dom_node_delta(link_t_ptr ilk)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	return ptt->delta;
}

void set_dom_node_delta(link_t_ptr ilk,var_long delta)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	ptt->delta = delta;
}

void set_dom_node_options(link_t_ptr ilk,const tchar_t* token,int len)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	if(ptt->opti)
	{
		destroy_string_table(ptt->opti);
		ptt->opti = NULL;
	}

	if(is_null(token) || !len)
		return;

	ptt->opti = create_string_table(ORDER_ASCEND);
	string_table_parse_options(ptt->opti, token, len, OPT_ITEMFEED, OPT_LINEFEED);
}

int get_dom_node_options_len(link_t_ptr ilk)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	if(!ptt->opti)
		return 0;

	return string_table_format_options(ptt->opti,NULL,MAX_LONG, OPT_ITEMFEED, OPT_LINEFEED);
}

int get_dom_node_options(link_t_ptr ilk,tchar_t* buf,int max)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	if(!ptt->opti)
	{
		return 0;
	}

	return string_table_format_options(ptt->opti,buf,max, OPT_ITEMFEED, OPT_LINEFEED);
}

link_t_ptr get_dom_node_options_table(link_t_ptr ilk)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	return ptt->opti;
}

const tchar_t* get_dom_node_options_text_ptr(link_t_ptr ilk,const tchar_t* szKey,int klen)
{
	dom_node_t* ptt;
	link_t_ptr ent;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	if(is_null(szKey))
		return NULL;

	if (!ptt->opti)
		return NULL;

	ent = get_string_entity(ptt->opti,szKey,klen);
	
	return (ent) ? get_string_entity_val_ptr(ent) : NULL;
}

void set_dom_node_options_text(link_t_ptr ilk,const tchar_t* szKey,int klen,const tchar_t* szVal,int vlen)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	if(is_null(szKey))
		return ;

	if(!ptt->opti)
	{
		ptt->opti = create_string_table(ORDER_ASCEND);
	}

	write_string_entity(ptt->opti,szKey,klen,szVal,vlen);
}

int get_dom_child_node_count(link_t_ptr ilk)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	return ptt->count;
}

link_t_ptr get_dom_first_child_node(link_t_ptr ilk)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	return get_first_link(&ptt->lkChild);
}

link_t_ptr get_dom_last_child_node(link_t_ptr ilk)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	return get_last_link(&ptt->lkChild);
}

link_t_ptr get_dom_parent_node(link_t_ptr ilk)
{
	dom_node_t* ptt;
	link_t_ptr parent;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	if(is_dom_doc_node(ilk))
		return NULL;

	parent = get_root_link(ilk);

	XDL_ASSERT(parent != NULL);

	ptt = DomItemFromChild(parent);
	return &(ptt->lkSibling);
}

link_t_ptr get_dom_next_sibling_node(link_t_ptr ilk)
{
	XDL_ASSERT(ilk && ilk->tag == lkNode);

	if(is_dom_doc_node(ilk))
		return NULL;
	else
		return get_next_link(ilk);
}

link_t_ptr get_dom_prev_sibling_node(link_t_ptr ilk)
{
	XDL_ASSERT(ilk && ilk->tag == lkNode);

	if(is_dom_doc_node(ilk))
		return NULL;
	else
		return get_prev_link(ilk);

}

int get_dom_node_level(link_t_ptr ilk)
{
	int level = 0;

	while (ilk = get_dom_parent_node(ilk))
		level++;

	return level;
}

link_t_ptr get_dom_child_node_at(link_t_ptr ilk,int index)
{
	link_t_ptr nlk;

	XDL_ASSERT(index >= 0);

	nlk = get_dom_first_child_node(ilk);
	while(nlk)
	{
		if(!index)
			break;

		nlk = get_dom_next_sibling_node(nlk);
		index --;
	}
	return nlk;
}

static bool_t _compare_dom_node_attr_proc(link_t_ptr plk, void* pv)
{
	entity_param_t* pp;

	XDL_ASSERT(pv != NULL);

	pp = (entity_param_t*)pv;

	return (compare_text(get_dom_node_attr_ptr(plk,pp->key,pp->klen),-1,pp->val,pp->vlen,1) == 0)? 0 : 1;
}

link_t_ptr find_dom_node_by_attr(link_t_ptr pos,bool_t deep,const tchar_t* key,int klen,const tchar_t* val,int vlen)
{
	entity_param_t ep = { 0 };
	link_t_ptr sub;

	if (deep)
	{
		ep.key = key;
		ep.klen = klen;
		ep.val = val;
		ep.vlen = vlen;

		return enum_dom_node(pos, (CALLBACK_ENUMLINK)_compare_dom_node_attr_proc, (void*)&ep);
	}
	else
	{
		sub = get_dom_first_child_node(pos);
		while (sub)
		{
			if (compare_text(get_dom_node_attr_ptr(sub, key, klen), -1, val, vlen, 1) == 0)
				break;

			sub = get_dom_next_sibling_node(sub);
		}

		return sub;
	}
}

static bool_t _compare_dom_node_name_proc(link_t_ptr plk, void* pv)
{
	token_param_t *pp;

	XDL_ASSERT(pv != NULL);

	pp = (token_param_t*)pv;

	return (compare_text(get_dom_node_name_ptr(plk),-1,pp->token,pp->len,1) == 0)? 0 : 1;
}

link_t_ptr find_dom_node_by_name(link_t_ptr pos,bool_t deep,const tchar_t* sz_name,int len)
{
	token_param_t tp = { 0 };
	link_t_ptr sub;

	if (deep)
	{
		tp.token = sz_name;
		tp.len = len;

		return enum_dom_node(pos, (CALLBACK_ENUMLINK)_compare_dom_node_name_proc, (void*)&tp);
	}
	else
	{
		sub = get_dom_first_child_node(pos);
		while (sub)
		{
			if (compare_text(get_dom_node_name_ptr(sub), -1, sz_name, len, 1) == 0)
				break;

			sub = get_dom_next_sibling_node(sub);
		}

		return sub;
	}
}

static bool_t _compare_dom_node_text_proc(link_t_ptr plk, void* pv)
{
	token_param_t *pp;

	XDL_ASSERT(pv != NULL);

	pp = (token_param_t*)pv;

	return (compare_text(get_dom_node_text_ptr(plk), -1, pp->token, pp->len,1) == 0) ? 0 : 1;
}

link_t_ptr find_dom_node_by_text(link_t_ptr pos, bool_t deep, const tchar_t* sz_text, int len)
{
	token_param_t tp = { 0 };
	link_t_ptr sub;

	if (deep)
	{
		tp.token = sz_text;
		tp.len = len;

		return enum_dom_node(pos, (CALLBACK_ENUMLINK)_compare_dom_node_text_proc, (void*)&tp);
	}
	else
	{
		sub = get_dom_first_child_node(pos);
		while (sub)
		{
			if (compare_text(get_dom_node_text_ptr(sub), -1, sz_text, len, 1) == 0)
				break;

			sub = get_dom_next_sibling_node(sub);
		}

		return sub;
	}
}

int _sorting_dom_node_name_proc(link_t_ptr plk1, link_t_ptr plk2, void* param)
{
	return compare_text(get_dom_node_name_ptr(plk1), -1, get_dom_node_name_ptr(plk2), -1, 0);
}

void sort_dom_child_node_by_name(link_t_ptr ilk,int desc)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	bubble_sort_link(&ptt->lkChild, (CALLBACK_SORTLINK)_sorting_dom_node_name_proc, desc, NULL);
}

int _sorting_dom_node_text_proc(link_t_ptr plk1, link_t_ptr plk2, void* param)
{
	return compare_text(get_dom_node_text_ptr(plk1), -1, get_dom_node_text_ptr(plk2), -1, 0);
}

void sort_dom_child_node_by_text(link_t_ptr ilk,int desc)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	bubble_sort_link(&ptt->lkChild, (CALLBACK_SORTLINK)_sorting_dom_node_text_proc, desc, NULL);
}

int _sorting_dom_node_attr_proc(link_t_ptr plk1, link_t_ptr plk2, void* param)
{
	return compare_text(get_dom_node_attr_ptr(plk1, (const tchar_t*)param, -1), -1, get_dom_node_attr_ptr(plk2, (const tchar_t*)param, -1), -1, 0);
}

void sort_dom_child_node_by_attr(link_t_ptr ilk, const tchar_t* attr,int desc)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);
	bubble_sort_link(&ptt->lkChild, (CALLBACK_SORTLINK)_sorting_dom_node_attr_proc, desc, (void*)attr);
}

void merge_dom_child_nodes(link_t_ptr dst, link_t_ptr src)
{
	link_t_ptr plk;

	while (plk = detach_dom_node(src, LINK_FIRST))
		attach_dom_node(dst, LINK_LAST, plk);
}

void copy_dom_node(link_t_ptr dst, link_t_ptr src)
{
	link_t_ptr st_src = NULL;
	link_t_ptr st_dst = NULL;
	link_t_ptr nlk, ilk, plk;

	copy_hash_table(get_dom_node_attr_table(dst), get_dom_node_attr_table(src));
	set_dom_node_name(dst, get_dom_node_name_ptr(src), -1);
	set_dom_node_text(dst, get_dom_node_text_ptr(src), -1);

	plk = dst;

	nlk = get_dom_first_child_node(src);
	while (nlk)
	{
		ilk = insert_dom_node(plk, LINK_LAST);

		copy_hash_table(get_dom_node_attr_table(ilk), get_dom_node_attr_table(nlk));
		set_dom_node_name(ilk, get_dom_node_name_ptr(nlk), -1);
		set_dom_node_text(ilk, get_dom_node_text_ptr(nlk), -1);

		if (get_dom_first_child_node(nlk))
		{
			if (!st_src)
				st_src = create_stack_table();
			push_stack_node(st_src, (void*)nlk);
			nlk = get_dom_first_child_node(nlk);

			if (!st_dst)
				st_dst = create_stack_table();
			push_stack_node(st_dst, plk);
			plk = ilk;

			continue;
		}

		while (nlk)
		{
			nlk = get_dom_next_sibling_node(nlk);
			if (nlk)
				break;
			nlk = (st_src) ? (link_t_ptr)pop_stack_node(st_src) : NULL;
			plk = (st_dst) ? (link_t_ptr)pop_stack_node(st_dst) : NULL;
		}
	}

	if (st_src)
		destroy_stack_table(st_src);

	if (st_dst)
		destroy_stack_table(st_dst);
}

bool_t get_dom_node_page_cator(link_t_ptr ilk, int page, page_cator_t* pca)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	if (!ptt->pages)
		return 0;

	if (page > ptt->pages)
		return 0;

	if (pca)
	{
		xmem_copy((void*)pca, (void*)(&(ptt->pcator[page - 1])), sizeof(page_cator_t));
	}

	return 1;
}

void set_dom_node_page_cator(link_t_ptr ilk, int page, const page_cator_t* pca)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	if (!page)
		return;

	if (page > ptt->pages)
	{
		ptt->pcator = (page_cator_t*)xmem_realloc(ptt->pcator, page * sizeof(page_cator_t));
		ptt->pages = page;
	}

	xmem_copy((void*)(&(ptt->pcator[page - 1])), (void*)pca, sizeof(page_cator_t));
}

void del_dom_node_page_cator(link_t_ptr ilk, int page)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	if (!page)
	{
		xmem_free(ptt->pcator);
		ptt->pcator = NULL;
		ptt->pages = 0;
		return;
	}

	if (page <= ptt->pages)
	{
		ptt->pcator = (page_cator_t*)xmem_realloc(ptt->pcator, (page - 1) * sizeof(page_cator_t));
		ptt->pages = page - 1;
	}
}

int get_dom_node_page_cator_count(link_t_ptr ilk)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	return ptt->pages;
}

bool_t get_dom_node_line_cator(link_t_ptr ilk, int point, int* pcx, int* pcy)
{
	dom_node_t* ptt;
	dword_t dw;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	if (point < 0 || point > ptt->points - 1)
		return 0;

	dw = ptt->pointer[point];

	*pcx = dw & 0x0000FFFF;
	*pcy = (dw >> 16) & 0x0000FFFF;

	return 1;
}

void set_dom_node_line_cator(link_t_ptr ilk, int point, int cx, int cy)
{
	dom_node_t* ptt;
	dword_t dw;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	XDL_ASSERT(point >= 0 && point <= ptt->points - 1);

	dw = ((cy << 16) & 0xFFFF0000) | (cx & 0x0000FFFF);
	ptt->pointer[point] = dw;
}

void ins_dom_node_line_cator(link_t_ptr ilk, int point, int cx, int cy)
{
	dom_node_t* ptt;
	int i;
	dword_t dw;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	XDL_ASSERT(point >= 0 && point <= ptt->points);

	dw = ((cy << 16) & 0xFFFF0000) | (cx & 0x0000FFFF);

	ptt->points++;
	ptt->pointer = (dword_t*)xmem_realloc(ptt->pointer, ptt->points * sizeof(dword_t));

	for (i = ptt->points - 1; i > point; i--)
	{
		ptt->pointer[i] = ptt->pointer[i - 1];
	}

	ptt->pointer[point] = dw;
}

void del_dom_node_line_cator(link_t_ptr ilk, int point)
{
	dom_node_t* ptt;
	int i;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	XDL_ASSERT(point >= 0 && point <= ptt->points - 1);

	for (i = point; i < ptt->points - 1; i++)
	{
		ptt->pointer[i] = ptt->pointer[i + 1];
	}

	ptt->points--;
	ptt->pointer = (dword_t*)xmem_realloc(ptt->pointer, ptt->points * sizeof(dword_t));
}

void cut_dom_node_line_cator(link_t_ptr ilk, int point)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	XDL_ASSERT(point >= 0 && point <= ptt->points);

	if (point < ptt->points)
	{
		ptt->points = point;
		ptt->pointer = (dword_t*)xmem_realloc(ptt->pointer, ptt->points * sizeof(dword_t));
	}
}

int get_dom_node_line_cator_count(link_t_ptr ilk)
{
	dom_node_t* ptt;

	XDL_ASSERT(ilk && ilk->tag == lkNode);

	ptt = DomItemFromLink(ilk);

	return ptt->points;
}


#endif /*#XDL_SUPPORT_DOC*/