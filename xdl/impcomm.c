/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc comm document

	@module	impcomm.c | comport implement file

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

#include "impcomm.h"
#include "xdlinit.h"
#include "xdlimp.h"

#ifdef XDK_SUPPORT_COMM

typedef struct _comport_t{
	xhand_head head;		//reserved for xhand_t

	res_file_t comm;
	async_t over;
}comport_t;

void xcomm_default_mode(dev_com_t* pmod)
{
	if_comm_t* pif;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_default_comm_mode)(pmod);
}

bool_t xcomm_set_mode(xhand_t com, const dev_com_t* pmod)
{
	comport_t* pst = (comport_t*)com;
	if_comm_t* pif;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_set_comm_mode)(pst->comm, pmod);
}

bool_t xcomm_get_mode(xhand_t com, dev_com_t* pmod)
{
	comport_t* pst = (comport_t*)com;
	if_comm_t* pif;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_get_comm_mode)(pst->comm, pmod);
}

xhand_t xcomm_open(const tchar_t* pname, dword_t fmode)
{
	comport_t* pst;
	if_comm_t* pif;
	res_file_t fh;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	fh = (*pif->pf_comm_open)(pname, fmode);
	if (fh == INVALID_FILE)
	{
		set_system_error(_T("pf_comm_open"));
		return NULL;
	}

	pst = (comport_t*)xmem_alloc(sizeof(comport_t));
	pst->head.tag = _HANDLE_COMM;
	pst->comm = fh;

	if (fmode & FILE_OPEN_OVERLAP)
	{
		async_alloc_lapp(&pst->over);
	}

	return (xhand_t)pst;
}

void xcomm_close(xhand_t com)
{
	comport_t* pst = (comport_t*)com;
	if_comm_t* pif;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_comm_close)(pst->comm);

	async_free_lapp(&pst->over);

	xmem_free(pst);
}

dword_t xcomm_listen(xhand_t com, dword_t* pcb)
{
	comport_t* pst = (comport_t*)com;
	if_comm_t* pif;
	dword_t even;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	even = (*pif->pf_comm_listen)(pst->comm, &pst->over);

	if (pcb) *pcb = (dword_t)pst->over.size;

	return even;
}

bool_t xcomm_write(xhand_t com, const byte_t* buf, dword_t* pcb)
{
	comport_t* pst = (comport_t*)com;
	if_comm_t* pif;
	dword_t len;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	XDL_ASSERT(com && com->tag == _HANDLE_COMM);

	len = *pcb;

	if (!(*pif->pf_comm_write)(pst->comm, (void*)buf, len, &pst->over))
	{
		set_system_error(_T("pf_comm_write"));
		*pcb = 0;

		return 0;
	}

	if (pcb) *pcb = (dword_t)pst->over.size;

	return 1;
}

bool_t xcomm_flush(xhand_t com)
{
	comport_t* pst = (comport_t*)com;
	if_comm_t* pif;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	XDL_ASSERT(com && com->tag == _HANDLE_COMM);

	return (*pif->pf_comm_flush)(pst->comm);
}

bool_t xcomm_read(xhand_t com, byte_t* buf, dword_t* pcb)
{
	comport_t* pst = (comport_t*)com;
	if_comm_t* pif;
	dword_t len;

	pif = PROCESS_COMM_INTERFACE;

	XDL_ASSERT(pif != NULL);

	XDL_ASSERT(com && com->tag == _HANDLE_COMM);

	len = *pcb;

	if (!(*pif->pf_comm_read)(pst->comm, buf, len, &pst->over))
	{
		set_system_error(_T("pf_comm_read"));
		*pcb = 0;

		return 0;
	}

	if (pcb) *pcb = (dword_t)pst->over.size;

	return 1;
}

#endif //XDK_SUPPORT_COMM
