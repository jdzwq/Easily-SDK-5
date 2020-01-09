/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hmac document

	@module	oemhmac.c | hmac third party implement file

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

#include "oembase64.h"
#include "xdlcode.h"
#include "xdlstr.h"

#ifdef XDL_SUPPORT_OEM

#include "base64\base64.h"

#pragma comment(lib,"base64.lib")

int a_base64_decode(const schar_t* src, int slen, byte_t* dest, int dlen)
{
	return _base64_decode(src, slen, dest, dlen);
}

int a_base64_encode(const byte_t* src, int slen, schar_t* dest, int dlen)
{
	return _base64_encode(src, slen, dest, dlen);
}

int w_base64_decode(const ucode_t* src, int slen, byte_t* dest, int dlen)
{
	schar_t* buf;
	int size;

	size = ucs_to_mbs(src, slen, NULL, MAX_LONG);
	buf = a_xsalloc(size + 1);
	ucs_to_mbs(src, slen, buf, size);

	size = _base64_decode(buf, size, dest, dlen);

	a_xsfree(buf);

	return size;
}

int w_base64_encode(const byte_t* src, int slen, ucode_t* dest, int dlen)
{
	return _w_base64_encode(src, slen, dest, dlen);
}



#endif //XDLOEM