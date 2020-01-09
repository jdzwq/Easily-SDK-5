/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc code bar document

	@module	oembase64.h | base64 third party wrap interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#ifndef _OEMBASE64_H
#define	_OEMBASE64_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_OEM

XDL_API int w_base64_decode(const ucode_t* src, int slen, byte_t* dest, int dlen);

XDL_API int a_base64_decode(const schar_t* src, int slen, byte_t* dest, int dlen);

XDL_API int w_base64_encode(const byte_t* src, int slen, ucode_t* dest, int dlen);

XDL_API int a_base64_encode(const byte_t* src, int slen, schar_t* dest, int dlen);

#ifdef _UNICODE
#define base64_decode		w_base64_decode
#define base64_encode		w_base64_encode
#else
#define base64_decode		a_base64_decode
#define base64_encode		a_base64_encode
#endif

#endif //XDLOEM

#endif	// OEMBASE64_H 

