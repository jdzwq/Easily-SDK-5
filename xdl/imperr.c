/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc error document

	@module	imperr.c | implement file

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

#include "imperr.h"
#include "impmem.h"
#include "imperr.h"
#include "impassert.h"
#include "impconv.h"

#include "xdlinit.h"

void set_last_error(const tchar_t* errcode, const tchar_t* errtext, int len)
{
#ifdef XDK_SUPPORT_ERROR
	if_jump_t* pju;

	pju = THREAD_JUMP_INTERFACE;

	if (!pju)
		return;

	if (!pju->if_error)
		return;

	xmem_zero((void*)pju->if_error, 1024);

#if defined(_UNICODE) || defined(UNICODE)
	ucs_to_utf8(errcode, -1, pju->if_error, NUM_LEN);
	ucs_to_utf8(errtext, len, (pju->if_error + NUM_LEN), (1024 - NUM_LEN));
#else
	mbs_to_utf8(errcode, -1, pju->if_error, NUM_LEN);
	mbs_to_utf8(errtext, len, (pju->if_error + NUM_LEN), (1024 - NUM_LEN));
#endif

#endif
}

void get_last_error(tchar_t* code, tchar_t* text, int max)
{
#ifdef XDK_SUPPORT_ERROR
	if_jump_t* pju;

	pju = THREAD_JUMP_INTERFACE;

	if (!pju)
		return;

	if (code)
	{
#if defined(_UNICODE) || defined(UNICODE)
		utf8_to_ucs(pju->if_error, NUM_LEN, code, NUM_LEN);
#else
		utf8_to_mbs(pju->if_error, NUM_LEN, code, NUM_LEN);
#endif
	}

	if (text)
	{
#if defined(_UNICODE) || defined(UNICODE)
		utf8_to_ucs((pju->if_error + NUM_LEN), (1024 - NUM_LEN), text, max);
#else
		utf8_to_mbs((pju->if_error + NUM_LEN), (1024 - NUM_LEN), text, max);
#endif
	}
#endif
}

void set_system_error(const tchar_t* errcode)
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };

#ifdef XDK_SUPPORT_ERROR
	if_error_t* pie;

	pie = PROCESS_ERROR_INTERFACE;

	XDL_ASSERT(pie != NULL);

	(*pie->pf_error_text)(errtext, ERR_LEN);
#else
	xscpy(errtext, _T("unknown system error"));
#endif

	set_last_error(errcode, errtext, -1);
}

void trace_error(void* param, PF_ERROR_TRACE pf)
{
#ifdef XDK_SUPPORT_ERROR
	if_error_t* pie;

	pie = PROCESS_ERROR_INTERFACE;

	XDL_ASSERT(pie != NULL);

	pie->param = param;
	pie->pf_error_trace = pf;
#endif
}

void xdl_trace(const tchar_t* code, const tchar_t* info)
{
#ifdef XDK_SUPPORT_ERROR
	if_error_t* pie;

	pie = PROCESS_ERROR_INTERFACE;

	XDL_ASSERT(pie != NULL);

	if (pie->pf_error_trace)
	{
		(*pie->pf_error_trace)(pie->param, code, info);
	}
	else
	{
		(*pie->pf_error_debug)(code);
		(*pie->pf_error_debug)(_T(": "));
		(*pie->pf_error_debug)(info);
		(*pie->pf_error_debug)(_T("\n"));
	}
#endif
}

void xdl_trace_last()
{
#ifdef XDK_SUPPORT_ERROR
	if_jump_t* pju;

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	pju = THREAD_JUMP_INTERFACE;

	if (!pju)
		return;

	get_last_error(errcode, errtext, ERR_LEN);

	xdl_trace(errcode, errtext);
#endif
}
