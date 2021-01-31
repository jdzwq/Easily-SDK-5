/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl error document

	@module	imperr.h | interface file

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

#ifndef _IMPERR_H
#define _IMPERR_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION set_system_error: set system error.
@INPUT const tchar_t* errcode: the error code.
@RETURN void: none.
*/
EXP_API void set_system_error(const tchar_t* errcode);

/*
@FUNCTION set_last_error: set last error.
@INPUT const tchar_t* errcode: the error code.
@INPUT const tchar_t* errtext: the error text.
@INPUT int len: the error text length in characters, not include terminate character.
@RETURN void: none.
*/
EXP_API void set_last_error(const tchar_t* errcode, const tchar_t* errtext, int len);

/*
@FUNCTION get_last_error: get last error.
@INPUT const tchar_t* errcode: the error code.
@INPUT const tchar_t* errtext: the error text.
@INPUT int len: the error text length in characters, not include terminate character.
@RETURN void: none.
*/
EXP_API void get_last_error(tchar_t* code, tchar_t* text, int max);

/*
@FUNCTION trace_error: set error trace routing.
@INPUT void* param: the parameter will be transfered into error routing.
@INPUT PF_ERROR_TRACE pf: the error trace routing.
@RETURN void: none.
*/
EXP_API void trace_error(void* param, PF_ERROR_TRACE pf);

/*
@FUNCTION xdl_trace: set and trace error.
@INPUT const tchar_t* code: the error code.
@INPUT const tchar_t* info: the error text.
@RETURN void: none.
*/
EXP_API void xdl_trace(const tchar_t* code, const tchar_t* info);

/*
@FUNCTION xdl_trace_last: trace last error.
@RETURN void: none.
*/
EXP_API void xdl_trace_last(void);

#if defined(DEBUG) || defined(_DEBUG)
#define XDL_TRACE(code, token)	xdl_trace(code, token)
#define XDL_TRACE_LAST			xdl_trace_last()
#else
#define XDL_TRACE(code, token)	
#define XDL_TRACE_LAST
#endif

#ifdef	__cplusplus
}
#endif

#endif /*IMPERR_H*/