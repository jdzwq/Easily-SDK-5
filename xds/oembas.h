/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl base64 document

	@module	oembas.h | interface file

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

#ifndef _OEMBAS_H
#define _OEMBAS_H

#include "xdsdef.h"


#ifdef	__cplusplus
extern "C" {
#endif

EXP_API dword_t a_xbas_decode(const schar_t* src, int slen, byte_t* dest, dword_t dlen);

EXP_API dword_t w_xbas_decode(const wchar_t* src, int slen, byte_t* dest, dword_t dlen);

EXP_API int a_xbas_encode(const byte_t* src, dword_t slen, schar_t* dest, int dlen);

EXP_API int w_xbas_encode(const byte_t* src, dword_t slen, wchar_t* dest, int dlen);

#if defined(UNICODE) || defined(_UNICODE)
#define xbas_decode		w_xbas_decode
#define xbas_encode		w_xbas_encode
#else
#define xbas_decode		a_xbas_decode
#define xbas_encode		a_xbas_encode
#endif

#ifdef	__cplusplus
}
#endif


#endif /*OEMBAS_H*/
