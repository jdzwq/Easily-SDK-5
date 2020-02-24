/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc png document

	@module	oempng.h | png third party wrap interface file

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

#ifndef _OEMPNG_H
#define	_OEMPNG_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_PNG

XDL_API	int xpng_decompress(const byte_t* png_buf, int png_len, byte_t* bmp_buf, int bmp_size);

XDL_API	int xpng_compress(const byte_t* bmp_buf, int bmp_len, byte_t* png_buf, int png_size);

#endif //XDLOEM

#endif	/* OEMPNG_H */

