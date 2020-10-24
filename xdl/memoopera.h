/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memo operator document

	@module	memoopera.h | interface file

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

#ifndef _MEMOOPERA_H
#define _MEMOOPERA_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

typedef struct _MEMOOBJECT{
	link_t_ptr txt, nlk;
	int len, pos;
	bool_t eof;
}MEMOOBJECT;

#ifdef	__cplusplus
extern "C" {
#endif

	LOC_API bool_t call_memo_can_escape(void* p_obj);

	LOC_API bool_t call_memo_with_eof(void* p_obj);

	LOC_API int call_memo_read_char(void* p_obj, int max, int pos, int encode, tchar_t* pch);

	LOC_API int call_memo_read_token(void* p_obj, int max, int pos, int encode, tchar_t* pch, int len);

	LOC_API int call_memo_write_char(void* p_obj, int max, int pos, int encode, const tchar_t* pch);

	LOC_API int call_memo_write_indent(void* p_obj, int max, int pos, int encode);

	LOC_API int call_memo_write_carriage(void* p_obj, int max, int pos, int encode);

	LOC_API int call_memo_write_token(void* p_obj, int max, int pos, int encode, const tchar_t* pch, int len);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_MEMOOPERA_H*/
