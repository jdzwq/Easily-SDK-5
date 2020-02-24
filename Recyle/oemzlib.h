/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc md5 document

	@module	oemzlib.h | zlib third party wrap interface file

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

#ifndef _OEMZLIB_H
#define	_OEMZLIB_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_ZLIB

XDL_API	bool_t xzlib_compress_bytes(const byte_t* src_buf, unsigned long src_len, byte_t* zip_buf, unsigned long* zip_len);

XDL_API	bool_t xzlib_uncompress_bytes(const byte_t* zip_buf, unsigned long zip_len, byte_t* dst_buf, unsigned long* dst_len);

XDL_API	bool_t xgzip_compress_bytes(const byte_t* src_buf, unsigned long src_len, byte_t* zip_buf, unsigned long* zip_len);

XDL_API	bool_t xgzip_uncompress_bytes(const byte_t* zip_buf, unsigned long zip_len, byte_t* dst_buf, unsigned long* dst_len);

#endif //XDLOEM

#endif	/* OEMZLIB_H */

