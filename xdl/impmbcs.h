/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl mbcs document

	@module	impmbcs.h | xdl mbcs interface file

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

#ifndef _IMPMBCS_H
#define _IMPMBCS_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_MBCS

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API int gbk_code_sequence(byte_t b);

	XDL_API int gbk_byte_to_ucs(const byte_t* src, wchar_t* dest);

	XDL_API int gbk_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen);

	XDL_API int ucs_byte_to_gbk(wchar_t ch, byte_t* dest);

	XDL_API int ucs_to_gbk(const wchar_t* src, int slen, byte_t* dest, dword_t dlen);

	XDL_API int utf_code_sequence(byte_t b);

	XDL_API int utf_byte_to_ucs(const byte_t* src, wchar_t* dest);

	XDL_API int utf_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen);

	XDL_API int ucs_byte_to_utf(wchar_t ch, byte_t* dest);

	XDL_API int ucs_to_utf(const wchar_t* src, int slen, byte_t* dest, dword_t dlen);


#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_MBCS*/

#endif /*IMPMBCS_H*/
