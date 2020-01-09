/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc virtual memory document

	@module	xdlvir.c | virtual memory implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#include "xdlvir.h"
#include "xdlinit.h"
#include "xdlmem.h"
#include "xdlerr.h"

typedef struct _page_t* page_t_ptr;

typedef struct _page_t{
	dword_t read_pos;
	dword_t write_pos;
	void* data;
	page_t_ptr next;
}page_t;

typedef struct _virtual_t{
	xhand_head	head;			//reserved for xhand_t

	dword_t		page_size;
	page_t_ptr	page_first;
	page_t_ptr	page_last;
}virtual_t;

xhand_t virtual_alloc(void)
{
	virtual_t* pxt;
	if_virmem_t* pif;
	
	pif = PROCESS_VIRMEM_INTERFACE;
	XDL_ASSERT(pif != NULL);

	pxt = (virtual_t*)xmem_alloc(sizeof(virtual_t));
	pxt->head.tag = _HANDLE_VIRTUAL;
	pxt->head.cbs = sizeof(virtual_t);

	pxt->page_size = (*pif->pf_page_size)();
	pxt->page_first = pxt->page_last = NULL;	

	return (xhand_t)pxt;
}

void virtual_free(xhand_t xs)
{
	virtual_t* pxt = (virtual_t*)xs;
	page_t *prev,*next;
	if_virmem_t* pif;

	XDL_ASSERT(xs && xs->tag == _HANDLE_VIRTUAL);

	pif = PROCESS_VIRMEM_INTERFACE;
	XDL_ASSERT(pif != NULL);

	prev = pxt->page_first;

	while(prev)
	{
		next = prev->next;

		(*pif->pf_page_free)(prev->data);

		xmem_free(prev);

		prev = next;
	}

	xmem_free(pxt);
}

dword_t virtual_pagesize(xhand_t xs)
{
	virtual_t* pxt = (virtual_t*)xs;
	if_virmem_t* pif;

	XDL_ASSERT(xs && xs->tag == _HANDLE_VIRTUAL);

	pif = PROCESS_VIRMEM_INTERFACE;
	XDL_ASSERT(pif != NULL);

	return pxt->page_size;
}

bool_t	virtual_write(xhand_t xs,const byte_t* buf,dword_t* pb)
{
	virtual_t* pxt = (virtual_t*)xs;
	page_t* pvp;
	dword_t len,bytes,total = 0;
	if_virmem_t* pif;

	XDL_ASSERT(xs && xs->tag == _HANDLE_VIRTUAL);

	pif = PROCESS_VIRMEM_INTERFACE;
	XDL_ASSERT(pif != NULL);
	
	len = *pb;

	if (!buf || !len)
	{
		return 1;
	}

	pvp = pxt->page_last;

	while(len)
	{
		if(!pvp)
		{
			pvp = (page_t*)xmem_alloc(sizeof(page_t));

			pvp->data = (*pif->pf_page_alloc)();
			(*pif->pf_page_lock)(pvp->data);

			pvp->read_pos = pvp->write_pos = 0;
			pvp->next = NULL;

			pxt->page_last = pvp;
			if(!pxt->page_first)
				pxt->page_first = pvp;
		}

		bytes = pxt->page_size - pvp->write_pos;
		bytes = (bytes < len)? bytes : len;
		memcpy((void*)((byte_t*)pvp->data + pvp->write_pos),buf,bytes);
		pvp->write_pos += bytes;

		len -= bytes;
		total += bytes;

		if(pvp->write_pos == pxt->page_size)
		{
			(*pif->pf_page_unlock)(pvp->data);
			pvp = pvp->next;
		}
	}

	*pb = total;

	return 1;
}

bool_t	virtual_read(xhand_t xs,byte_t* buf,dword_t* pb)
{
	virtual_t* pxt = (virtual_t*)xs;
	page_t* pvp;
	dword_t len,bytes,total = 0;
	if_virmem_t* pif;

	XDL_ASSERT(xs && xs->tag == _HANDLE_VIRTUAL);

	pif = PROCESS_VIRMEM_INTERFACE;
	XDL_ASSERT(pif != NULL);

	len = *pb;

	if (!buf || !len)
	{
		return 1;
	}

	pvp = pxt->page_first;

	while(len)
	{
		if(!pvp)
			break;

		bytes = pvp->write_pos - pvp->read_pos;
		
		bytes = (bytes < len)? bytes : len;
		memcpy((void*)((byte_t*)buf + total),(void*)((byte_t*)pvp->data + pvp->read_pos),bytes);
		
		pvp->read_pos += bytes;

		if(pvp->read_pos == pvp->write_pos)
		{
			(*pif->pf_page_free)(pvp->data);

			if (pxt->page_last == pxt->page_first)
				pxt->page_last = pvp->next;
			pxt->page_first = pvp->next;
			xmem_free(pvp);

			pvp = pxt->page_first;
		}

		len -= bytes;
		total += bytes;

		if (!bytes)
			break;
	}

	*pb = total;

	return 1;
}

bool_t	virtual_peek(xhand_t xs, byte_t* buf, dword_t len)
{
	virtual_t* pxt = (virtual_t*)xs;
	page_t* pvp;
	dword_t bytes, pos, total = 0;
	if_virmem_t* pif;

	XDL_ASSERT(xs && xs->tag == _HANDLE_VIRTUAL);

	pif = PROCESS_VIRMEM_INTERFACE;
	XDL_ASSERT(pif != NULL);

	if (!buf || !len)
	{
		return 1;
	}

	pvp = pxt->page_first;

	while (len)
	{
		if (!pvp)
			break;

		pos = pvp->read_pos;
		bytes = pvp->write_pos - pos;

		bytes = (bytes < len) ? bytes : len;
		memcpy((void*)((byte_t*)buf + total), (void*)((byte_t*)pvp->data + pvp->read_pos), bytes);

		pos += bytes;

		if (pos == pvp->write_pos)
		{
			pvp = pvp->next;
		}

		len -= bytes;
		total += bytes;

		if (!bytes)
			break;
	}

	return 1;
}

dword_t virtual_size(xhand_t xs)
{
	virtual_t* pxt = (virtual_t*)xs;
	page_t* pvp;
	dword_t total = 0;

	XDL_ASSERT(xs && xs->tag == _HANDLE_VIRTUAL);

	pvp = pxt->page_first;

	while(pvp)
	{
		total += (pvp->write_pos - pvp->read_pos);
		pvp = pvp->next;
	}

	return total;
}

void virtual_empty(xhand_t xs)
{
	virtual_t* pxt = (virtual_t*)xs;
	page_t *prev,*next;
	if_virmem_t* pif;

	XDL_ASSERT(xs && xs->tag == _HANDLE_VIRTUAL);

	pif = PROCESS_VIRMEM_INTERFACE;
	XDL_ASSERT(pif != NULL);

	prev = pxt->page_first;

	while(prev)
	{
		next = prev->next;

		(*pif->pf_page_free)(prev->data);

		xmem_free(prev);
		prev = next;
	}

	pxt->page_first = pxt->page_last = NULL;
}

void virtual_write_byte(xhand_t xs, byte_t c)
{
	byte_t* pb;
	dword_t one = 1;

	pb = &c;
	virtual_write(xs, pb, &one);
}

dword_t virtual_read_byte(xhand_t xs, byte_t* pb)
{
	dword_t one = 1;

	return virtual_read(xs, pb, &one);
}
