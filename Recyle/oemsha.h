/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc sha document

	@module	oemsha.h | sha third party wrap interface file

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

#ifndef _OEMSHA_H
#define	_OEMSHA_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_SHA

#define XSHA1_LEN	20
#define XSHA2_LEN	32
#define XSHA4_LEN	64

XDL_API	int xsha1_hmac_bytes(const byte_t* key_buf, int key_len, const byte_t* src_buf, int src_len, byte_t* dig_buf);

XDL_API int xsha2_hmac_bytes(const byte_t* key_buf, int key_len, const byte_t* src_buf, int src_len, bool_t is224, byte_t* dig_buf);

XDL_API int xsha4_hmac_bytes(const byte_t* key_buf, int key_len, const byte_t* src_buf, int src_len, bool_t is384, byte_t* dig_buf);

#endif //XDLOEM

#endif	/* OEMSHA_H */

