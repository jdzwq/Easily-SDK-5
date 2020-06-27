/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pipe document

	@module	imppipe.c | implement file

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

#include "imppipe.h"
#include "xdlinit.h"
#include "xdlimp.h"

#include "xdlstd.h"

#ifdef XDK_SUPPORT_PIPE

typedef struct _pipe_t{
	xhand_head head;

	res_file_t pipe;
	bool_t b_srv;
	tchar_t* pname;

	async_t* pov;
}pipe_t;

xhand_t xpipe_srv(const tchar_t* pname, dword_t fmode)
{
	res_file_t pd;
	pipe_t* ppi;
	if_pipe_t* pif;

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	pd = (*pif->pf_pipe_srv)(pname, fmode);

	if (pd == INVALID_FILE)
	{
		set_system_error(_T("pf_pipe_srv"));
		return NULL;
	}

	ppi = (pipe_t*)xmem_alloc(sizeof(pipe_t));
	ppi->head.tag = _HANDLE_PIPE;
	ppi->pipe = pd;
	ppi->b_srv = 1;
	ppi->pname = xsclone(pname);

	ppi->pov = async_alloc_lapp(((fmode & FILE_OPEN_OVERLAP) ? ASYNC_EVENT : ASYNC_BLOCK), PIPE_BASE_TIMO, INVALID_FILE);

	return &ppi->head;
}

bool_t xpipe_listen(xhand_t pip)
{
	pipe_t* ppi = TypePtrFromHead(pipe_t, pip);
	if_pipe_t* pif;
    bool_t rt;

	XDL_ASSERT(pip && pip->tag == _HANDLE_PIPE);

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

    rt = (*pif->pf_pipe_listen)(ppi->pipe, ppi->pov);
    
    return rt;
}

void xpipe_stop(xhand_t pip)
{
	pipe_t* ppi = TypePtrFromHead(pipe_t, pip);
	if_pipe_t* pif;

	XDL_ASSERT(pip && pip->tag == _HANDLE_PIPE);

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_pipe_stop)(ppi->pipe);
}

xhand_t xpipe_cli(const tchar_t* pname, dword_t fmode)
{
	res_file_t pd;
	pipe_t* ppi;
	if_pipe_t* pif;

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	pd = (*pif->pf_pipe_cli)(pname, fmode);

	if (pd == INVALID_FILE)
	{
		set_system_error(_T("pf_pipe_cli"));
		return NULL;
	}

	ppi = (pipe_t*)xmem_alloc(sizeof(pipe_t));
	ppi->head.tag = _HANDLE_PIPE;
	ppi->pipe = pd;
	ppi->b_srv = 0;
	ppi->pname = xsclone(pname);

	ppi->pov = async_alloc_lapp(((fmode & FILE_OPEN_OVERLAP) ? ASYNC_EVENT : ASYNC_BLOCK), PIPE_BASE_TIMO, INVALID_FILE);

	return &ppi->head;
}

xhand_t xpipe_attach(res_file_t hp)
{
	pipe_t* ppi;
	if_pipe_t* pif;

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	ppi = (pipe_t*)xmem_alloc(sizeof(pipe_t));
	ppi->head.tag = _HANDLE_PIPE;
	ppi->pipe = hp;

	ppi->pov = async_alloc_lapp(ASYNC_BLOCK, PIPE_BASE_TIMO, INVALID_FILE);

	return &ppi->head;
}

res_file_t xpipe_detach(xhand_t pip)
{
	pipe_t* ppi = TypePtrFromHead(pipe_t, pip);
	res_file_t hp;

	XDL_ASSERT(pip && pip->tag == _HANDLE_PIPE);

	hp = ppi->pipe;

	if (ppi->pov)
	{
		async_free_lapp(ppi->pov);
	}

	xmem_free(ppi);

	return hp;
}

res_file_t xpipe_handle(xhand_t pip)
{
	pipe_t* ppi = TypePtrFromHead(pipe_t, pip);

	XDL_ASSERT(pip && pip->tag == _HANDLE_PIPE);

	return ppi->pipe;
}

bool_t xpipe_wait(const tchar_t* pname, int ms)
{
	if_pipe_t* pif;

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_pipe_wait)(pname, ms);
}

bool_t xpipe_flush(xhand_t pip)
{
	pipe_t* ppi = TypePtrFromHead(pipe_t, pip);
	if_pipe_t* pif;

	XDL_ASSERT(pip && pip->tag == _HANDLE_PIPE);

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (ppi->pipe)
		return (*pif->pf_pipe_flush)(ppi->pipe);
	else
		return 0;
}

void xpipe_free(xhand_t pip)
{
	pipe_t* ppi = TypePtrFromHead(pipe_t, pip);
	if_pipe_t* pif;

	XDL_ASSERT(pip && pip->tag == _HANDLE_PIPE);

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (ppi->b_srv && ppi->pipe)
	{
		(*pif->pf_pipe_stop)(ppi->pipe);
	}
	
	if (ppi->pipe)
	{
        if(ppi->b_srv)
            (*pif->pf_pipe_close)(ppi->pname, ppi->pipe);
        else
            (*pif->pf_pipe_close)(NULL, ppi->pipe);
	}

	if (ppi->pov)
	{
		async_free_lapp(ppi->pov);
	}

	xsfree(ppi->pname);
	xmem_free(ppi);
}

bool_t xpipe_write(xhand_t pip, const byte_t* buf, dword_t* pcb)
{
	pipe_t* ppt = (pipe_t*)pip;
	if_pipe_t* pif;
	dword_t size, pos = 0;;

	XDL_ASSERT(pip && pip->tag == _HANDLE_PIPE);

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pcb;

	while (pos < size)
	{
		ppt->pov->size = 0;
		if (!(*pif->pf_pipe_write)(ppt->pipe, (void*)(buf + pos), size - pos, ppt->pov))
		{
			set_system_error(_T("pf_pipe_write"));

			*pcb = (dword_t)pos;
			return 0;
		}

		if (!(ppt->pov->size)) break;

		pos += (ppt->pov->size);
	}

	*pcb = (dword_t)pos;
	return 1;
}

bool_t xpipe_read(xhand_t pip, byte_t* buf, dword_t* pcb)
{
	pipe_t* ppt = (pipe_t*)pip;
	if_pipe_t* pif;
	dword_t size, pos = 0;

	XDL_ASSERT(pip && pip->tag == _HANDLE_PIPE);

	pif = PROCESS_PIPE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	size = *pcb;

	while (pos < size)
	{
		ppt->pov->size = 0;
		if (!(*pif->pf_pipe_read)(ppt->pipe, (void*)(buf + pos), size - pos, ppt->pov))
		{
			set_system_error(_T("pf_pipe_read"));

			*pcb = pos;
			return 0;
		}

		if (!(ppt->pov->size)) break;

		pos += (ppt->pov->size);
	}

	*pcb = pos;
	return 1;
}

#endif //XDK_SUPPORT_PIPE
