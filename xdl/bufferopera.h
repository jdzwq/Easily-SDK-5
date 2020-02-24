/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc buffer operator document

	@module	bufferopera.h | interface file

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

#ifndef BUFFEROPERA_H
#define BUFFEROPERA_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API bool_t call_buffer_can_escape(void* p_obj);

	XDL_API bool_t call_buffer_with_eof(void* p_obj);

	XDL_API int call_buffer_read_escape(void* p_obj, int max, int pos, int encode, tchar_t* pch);

	XDL_API int call_buffer_write_escape(void* p_obj, int max, int pos, int encode, tchar_t ch);

	XDL_API int call_buffer_read_char(void* p_obj, int max, int pos, int encode, tchar_t* pch);

	XDL_API int call_buffer_read_token(void* p_obj, int max, int pos, int encode, tchar_t* pch, int len);

	XDL_API int call_buffer_write_char(void* p_obj, int max, int pos, int encode, const tchar_t* pch);

	XDL_API int call_buffer_write_token(void* p_obj, int max, int pos, int encode, const tchar_t* pch, int len);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*BUFFEROPERA_H*/
