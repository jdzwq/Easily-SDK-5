/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc md5 document

	@module	oemmd5.h | md5 third party wrap interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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

#ifndef _OEMMD5_H
#define	_OEMMD5_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_MD5

#define XMD5_RESULT_LEN		16
#define XMD5_FORMAT_LEN		32

XDL_API	int xmd5_bytes(const byte_t* src_buf, int src_len, byte_t* enc_buf);

XDL_API void a_xmd5_token(const byte_t* enc, schar_t* buf);

XDL_API void w_xmd5_token(const byte_t* enc, ucode_t* buf);

#ifdef _UNICODE
#define xmd5_token		w_xmd5_token
#else
#define xmd5_token		a_xmd5_token
#endif

#endif //XDLOEM

#endif	/* OEMMD5_H */

