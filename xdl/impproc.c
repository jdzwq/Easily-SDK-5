/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc process document

	@module	impproc.c | implement file

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

#include "impproc.h"
#include "xdlinit.h"
#include "xdlimp.h"

#ifdef XDK_SUPPORT_PROCESS

res_modu_t load_library(const tchar_t* lname)
{
	if_process_t* pif;
	res_modu_t rmd;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	rmd = (*pif->pf_load_library)(lname);
	
	if(!rmd)
	{
		set_system_error(_T("pf_load_library"));
	}

	return rmd;
}

void free_library(res_modu_t lib)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_free_library)(lib);
}

void* get_address(res_modu_t lib, const schar_t* fname)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_get_address)(lib,fname);
}

void get_runpath(res_modu_t ins, tchar_t* buf, int max)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_get_runpath)(ins, buf, max);
}

bool_t create_process(const tchar_t* exename, const tchar_t* cmdline, int share, proc_info_t* ppi)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_create_process)(exename, cmdline, share, ppi))
	{
		set_system_error(_T("pf_create_process"));
		return 0;
	}

	return 1;
}

void process_waitrun(res_hand_t ph)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_process_waitrun)(ph);
}

void release_handle(res_hand_t hh)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_release_handle)(hh);
}

bool_t inherit_handle(res_hand_t hh, bool_t b)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_inherit_handle)(hh, b))
	{
		set_system_error(_T("pf_ibherit_handle"));
		return 0;
	}

	return 1;
}

void release_process(proc_info_t* ppi)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_release_process)(ppi);
}

res_hand_t process_dupli(res_hand_t ph, res_hand_t vh)
{
	if_process_t* pif;
	res_hand_t rv;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	rv = (*pif->pf_process_dupli)(ph, vh);
	if (!rv)
	{
		set_system_error(_T("pf_process_dupli"));
	}

	return rv;
}

void* process_alloc(res_hand_t ph, dword_t dw)
{
	if_process_t* pif;
	void* p;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	p = (*pif->pf_process_alloc)(ph, dw);

	if (!p)
	{
		set_system_error(_T("pf_process_alloc"));
	}

	return p;
}

void process_free(res_hand_t ph, void* p)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_process_free)(ph, p);
}

bool_t process_write(res_hand_t ph, void* p, void* data, dword_t dw)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_process_write)(ph, p, data, dw))
	{
		set_system_error(_T("pf_process_write"));
		return 0;
	}

	return 1;
}

bool_t process_read(res_hand_t ph, void* p, void* data, dword_t dw)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	if (!(*pif->pf_process_read)(ph, p, data, dw))
	{
		set_system_error(_T("pf_process_read"));
		return 0;
	}

	return 1;
}

void read_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, tchar_t* buf, int max)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_read_profile)(fname, sec, key, buf, max);
}

void write_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, const tchar_t* val)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_write_profile)(fname, sec, key, val);
}

void system_info(sys_info_t* psi)
{
	if_process_t* pif;

	pif = PROCESS_PROCESS_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_system_info)(psi);
}

#endif //XDK_SUPPORT_PROCESS
