/***********************************************************************
	Easily codepage

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc utf16 codepage document

	@module	acp.c | acp implement file

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


#include "acpdef.h"

typedef struct _codeguid_t{
	sword_t val;
}codeguid_t;

typedef struct _pageguid_t{
	codeguid_t* page_base;
}pageguid_t;

typedef struct _codepage_t{
	acp_head	head;
	int			page_size;
	int			max_pages;
	pageguid_t*	page_array;
}codepage_t;

codepage_t* _acp_alloc(wchar_t code_base)
{
	codepage_t* pxt;

	pxt = (codepage_t*)pmem_alloc(PAGE_SIZE);

	pxt->page_size = pmem_size(pxt);
	pxt->max_pages = (pxt->page_size - sizeof(codepage_t)) / sizeof(pageguid_t);
	pxt->page_array = (pageguid_t*)(pxt + 1);

	//pmem_lock(pxt);

	pxt->head.base = code_base;

	return pxt;
}

void _acp_free(codepage_t* pxt)
{
	pageguid_t* pg;
	int i;

	for (i = 0; i < pxt->max_pages; i++)
	{
		pg = (pageguid_t*)(pxt->page_array + i);

		if (pg->page_base)
			pmem_free(pg->page_base);
	}

	//pmem_unlock(pxt);
	pmem_free(pxt);
}

void _acp_protect(codepage_t* pxt)
{
	pageguid_t* pg;
	int i;

	for (i = 0; i < pxt->max_pages; i++)
	{
		pg = (pageguid_t*)(pxt->page_array + i);

		if (pg->page_base)
			pmem_protect(pg->page_base, 1);
	}
}

bool_t _acp_set_code(codepage_t* pxt, sword_t code, sword_t val)
{
	pageguid_t* pg;
	int index, ents, offset;

	ents = pxt->page_size / sizeof(codeguid_t);
	index = (int)(code - pxt->head.base) / ents;
	offset = (int)(code - pxt->head.base) % ents;

	if (index < 0 || index >= pxt->max_pages)
		return 0;

	pg = pxt->page_array + index;
	if (!pg->page_base)
	{
		pg->page_base = (codeguid_t*)pmem_alloc(PAGE_SIZE);
	}

	pg->page_base[offset].val = val;

	return 1;
}

bool_t acp_get_code(acp_t acp, sword_t code, sword_t* pval)
{
	codepage_t* pxt = TypePtrFromHead(codepage_t, acp);

	pageguid_t* pg;
	int index, ents, offset;

	ents = pxt->page_size / sizeof(codeguid_t);
	index = (int)(code - pxt->head.base) / ents;
	offset = (int)(code - pxt->head.base) % ents;
	if (offset < 0)
		return 0;

	if (index < 0 || index >= pxt->max_pages)
		return 0;

	pg = pxt->page_array + index;
	if (!pg->page_base)
	{
		return 0;
	}

	pmem_lock(pg->page_base);

	if (pval)
		*pval = pg->page_base[offset].val;
	else
		*pval = 0;

	return (*pval) ? 1 : 0;
}

acp_t create_ucs_codepage()
{
	int i, size;
	unsigned short c, v;
	codepage_t* pcp;

	pcp = _acp_alloc(_ACP_UCSMIN);
	size = unicode_gb2312_size();
	for (i = 0; i < size; i++)
	{
		c = v = 0;
		unicode_gb2312_code(i, &c, &v, NULL);
		_acp_set_code(pcp, c, v);
	}

	_acp_protect(pcp);

	return &pcp->head;
}

acp_t create_gbk_codepage()
{
	int i, size;
	unsigned short c, v;
	codepage_t* pcp;

	pcp = _acp_alloc(_ACP_GBKMIN);
	size = gb2312_unicode_size();
	for (i = 0; i < size; i++)
	{
		c = v = 0;
		gb2312_unicode_code(i, &c, &v, NULL);
		_acp_set_code(pcp, c, v);
	}
	_acp_protect(pcp);

	return &pcp->head;
}

void destroy_codepage(acp_t acp)
{
	codepage_t* pxt = TypePtrFromHead(codepage_t, acp);

	_acp_free(pxt);
}

/*******************************************************************************************/

int w_help_code(const wchar_t* src, int len, wchar_t* buf, int max)
{
	int count = 0;
	int i = 0;

	if (len < 0)
	{
		len = 0;
		while (src && *(src + len))
			len++;
	}

	while (i < len && count < max)
	{
		count += unicode_seek_help(*(src + i), ((buf) ? buf + count : NULL));
		i++;
	}

	return count;
}

int a_help_code(const schar_t* src, int len, schar_t* buf, int max)
{
	int seq, count = 0;
	int i = 0;

	if (len < 0)
	{
		len = 0;
		while (src && *(src + len))
			len++;
	}

	while (i < len && count < max)
	{
		seq = gb2312_code_sequence(*(byte_t*)(src + i));
		if (!seq)
			break;

		count += gb2312_seek_help((byte_t*)(src + i), ((buf) ? buf + count : NULL));
		i += seq;
	}

	return count;
}
