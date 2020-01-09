/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc variant stream

	@module	xdlstm.c | variant stream implement file

	@devnote 张文权 2003.01 - 2007.12 v3.0
			 张文权 2008.01 - 2009.12 v4.0
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlstm.h"
#include "xdlmem.h"
#include "xdlerr.h"
#include "xdlstr.h"

typedef struct _page_t* page_t_ptr;

typedef struct _page_t{
	size_t read_pos;
	size_t write_pos;
	void* data;
	page_t_ptr next;
}page_t;

typedef struct _stream_t{
	size_t		page_size;
	page_t_ptr	page_first;
	page_t_ptr	page_last;
}stream_t;

handle_t stream_alloc(void)
{
	stream_t* pxt;
	page_t* pag;
	SYSTEM_INFO si;
	
	GetSystemInfo(&si);
	pxt = (stream_t*)xmem_alloc(sizeof(stream_t));
	pxt->page_size = si.dwPageSize;

	pag = (page_t*)xmem_alloc(sizeof(page_t));
	pag->data = VirtualAlloc(NULL,pxt->page_size,MEM_RESERVE,PAGE_NOACCESS);
	VirtualAlloc(pag->data,pxt->page_size,MEM_COMMIT,PAGE_READWRITE);
	pag->read_pos = pag->write_pos = 0;
	pag->next = NULL;

	pxt->page_first = pxt->page_last = pag;	

	return (handle_t)pxt;
}

void stream_free(handle_t xs)
{
	stream_t* pxt;
	page_t *prev,*next;
	
	assert(xs);
	pxt = (stream_t*)xs;
	prev = pxt->page_first;

	while(prev)
	{
		next = prev->next;

		VirtualFree(prev->data,(DWORD)pxt->page_size,MEM_DECOMMIT);
		VirtualFree(prev->data,0,MEM_RELEASE);

		xmem_free(prev);

		prev = next;
	}

	xmem_free(pxt);
}

size_t	stream_write(handle_t xs,const byte_t* buf,size_t len)
{
	stream_t* pxt;
	page_t* pvp;
	size_t bytes,total = 0;

	assert(xs);
	assert(len >= 0);
	pxt = (stream_t*)xs;

	pvp = pxt->page_last;

	while(len)
	{
		if(!pvp)
		{
			pvp = (page_t*)xmem_alloc(sizeof(page_t));
			pvp->data = VirtualAlloc(NULL,pxt->page_size,MEM_RESERVE,PAGE_NOACCESS);
			VirtualAlloc(pvp->data,pxt->page_size,MEM_COMMIT,PAGE_READWRITE);
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
			pvp = pvp->next;
		}
	}

	return total;
}

size_t	stream_read(handle_t xs,byte_t* buf,size_t len)
{
	stream_t* pxt;
	page_t* pvp;
	size_t bytes,total = 0;
	
	assert(xs);
	assert(buf && len >= 0);
	pxt = (stream_t*)xs;

	pvp = pxt->page_first;

	while(len)
	{
		if(!pvp)
			break;

		bytes = pxt->page_size - pvp->read_pos;
		if(!bytes)
			break;

		bytes = (bytes < len)? bytes : len;
		memcpy((void*)((byte_t*)buf + total),(void*)((byte_t*)pvp->data + pvp->read_pos),bytes);
		
		pvp->read_pos += bytes;

		if(pvp->read_pos == pxt->page_size)
		{
			VirtualFree(pvp->data,pxt->page_size,MEM_DECOMMIT);
			VirtualFree(pvp->data,0,MEM_RELEASE);
			pxt->page_first = pvp->next;
			xmem_free(pvp);
		}

		len -= bytes;
		total += bytes;
	}

	return total;
}

size_t stream_size(handle_t xs)
{
	stream_t* pxt;
	page_t* pvp;
	size_t total = 0;

	assert(xs);
	pxt = (stream_t*)xs;
	pvp = pxt->page_first;

	while(pvp)
	{
		total += (pvp->write_pos - pvp->read_pos);
		pvp = pvp->next;
	}

	return total;
}

void stream_empty(handle_t xs)
{
	stream_t* pxt;
	page_t *prev,*next;

	assert(xs);
	pxt = (stream_t*)xs;
	prev = pxt->page_first;

	while(prev)
	{
		next = prev->next;

		VirtualFree(prev->data,(DWORD)pxt->page_size,MEM_DECOMMIT);
		VirtualFree(prev->data,0,MEM_RELEASE);

		xmem_free(prev);
		prev = next;
	}

	pxt->page_first = pxt->page_last = NULL;
}
