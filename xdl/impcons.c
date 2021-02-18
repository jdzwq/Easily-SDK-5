/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc console document

	@module	impcons.c | implement file

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

#include "impcons.h"
#include "impmem.h"
#include "imperr.h"
#include "impassert.h"
#include "impasync.h"
#include "impstr.h"

#include "xdlstd.h"

#include "xdlinit.h"

#ifdef XDK_SUPPORT_CONS

typedef struct _cons_context{
	handle_head head;		//reserved for xhand_t

	res_file_t cons;
	tchar_t *cname;
}cons_context;

xhand_t xcons_alloc()
{
	cons_context* pst;
	if_cons_t* pif;
	res_file_t fd;
	tchar_t cname[PATH_LEN + 1] = { 0 };

	pif = PROCESS_CONS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	fd = (*pif->pf_cons_alloc)(cname, PATH_LEN);
	if (fd == INVALID_FILE)
	{
		set_system_error(_T("pf_cons_alloc"));
		return NULL;
	}

	pst = (cons_context*)xmem_alloc(sizeof(cons_context));
	pst->head.tag = _HANDLE_CONS;
	pst->cons = fd;
	pst->cname = xsclone(cname);

	return &pst->head;
}

const tchar_t* xcons_name(xhand_t con)
{
	cons_context* pst = TypePtrFromHead(cons_context, con);

	XDL_ASSERT(con && con->tag == _HANDLE_CONS);

	return pst->cname;
}

res_file_t xcons_stdin(xhand_t con)
{
	cons_context* pst = TypePtrFromHead(cons_context, con);
	if_cons_t* pif;

	XDL_ASSERT(con && con->tag == _HANDLE_CONS);

	pif = PROCESS_CONS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_cons_stdin)(pst->cons);
}

res_file_t xcons_stdout(xhand_t con)
{
	cons_context* pst = TypePtrFromHead(cons_context, con);
	if_cons_t* pif;

	XDL_ASSERT(con && con->tag == _HANDLE_CONS);

	pif = PROCESS_CONS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_cons_stdout)(pst->cons);
}

void xcons_free(xhand_t con)
{
	cons_context* pst = TypePtrFromHead(cons_context, con);
	if_cons_t* pif;

	pif = PROCESS_CONS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	XDL_ASSERT(con && con->tag == _HANDLE_CONS);

	if (pst->cons)
	{
		(*pif->pf_cons_free)(pst->cons);
	}
	
	if (pst->cname)
	{
		xsfree(pst->cname);
	}

	xmem_free(pst);
}

bool_t xcons_sigaction(xhand_t con, PF_SIGHANDLER pf)
{
	cons_context* pst = TypePtrFromHead(cons_context, con);
	if_cons_t* pif;

	pif = PROCESS_CONS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	XDL_ASSERT(con && con->tag == _HANDLE_CONS);

	return (*pif->pf_cons_sigaction)(pst->cons, pf);
}

bool_t xcons_write(xhand_t con, const byte_t* buf, dword_t* pb)
{
	cons_context* pst = TypePtrFromHead(cons_context, con);
	if_cons_t* pif;
	dword_t size;

	pif = PROCESS_CONS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	XDL_ASSERT(con && con->tag == _HANDLE_CONS);

	size = *pb;

	if (!(*pif->pf_cons_write)(pst->cons, (void*)buf, size, &size))
	{
		set_system_error(_T("pf_cons_write"));
		*pb = 0;

		return 0;
	}

	*pb = (dword_t)size;

	return 1;
}

bool_t xcons_flush(xhand_t con)
{
	cons_context* pst = TypePtrFromHead(cons_context, con);
	if_cons_t* pif;

	pif = PROCESS_CONS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	XDL_ASSERT(con && con->tag == _HANDLE_CONS);

	return (*pif->pf_cons_flush)(pst->cons);
}

bool_t xcons_read(xhand_t con, byte_t* buf, dword_t* pb)
{
	cons_context* pst = TypePtrFromHead(cons_context, con);
	if_cons_t* pif;
	dword_t size;

	pif = PROCESS_CONS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	XDL_ASSERT(con && con->tag == _HANDLE_CONS);

	size = *pb;

	if (!(*pif->pf_cons_read)(pst->cons, buf, size, &size))
	{
		set_system_error(_T("pf_cons_read"));
		*pb = 0;

		return 0;
	}

	*pb = (dword_t)size;

	return 1;
}

#endif //XDK_SUPPORT_CONS
