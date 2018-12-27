/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc error document

	@module	imperr.c | error implement file

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
#include "xdlinit.h"
#include "xdlimp.h"
#include "xdlstr.h"


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

void set_last_error(const tchar_t* errcode, const tchar_t* errtext, int len)
{
#ifdef XDK_SUPPORT_ERROR
	if_jump_t* pju;

	pju = THREAD_JUMP_INTERFACE;
	
	if (!pju) 
		return;

	if (len < 0)
		len = xslen(errtext);

	len = (len < ERR_LEN) ? len : ERR_LEN;

	xsncpy(pju->errcode,errcode,NUM_LEN);
	xsncpy(pju->errtext, errtext, len);
#endif
}

void get_last_error(tchar_t* code, tchar_t* text, int max)
{
#ifdef XDK_SUPPORT_ERROR
	if_jump_t* pju;
	int len;

	pju = THREAD_JUMP_INTERFACE;
	
	if (!pju) 
		return;

	if (code)
	{
		xsncpy(code, pju->errcode, NUM_LEN);
	}

	if (text)
	{
		len = xslen(pju->errtext);
		len = (len < max) ? len : max;
		xsncpy(text, pju->errtext, len);
	}
#endif
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
	tchar_t errcode[NUM_LEN + 1];
	tchar_t errtext[ERR_LEN + 1];

	get_last_error(errcode, errtext, ERR_LEN);

	xdl_trace(errcode, errtext);
}
