/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc jpg document

	@module	oemjpg.h | interface file

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

#ifndef _OEMJPG_H
#define	_OEMJPG_H

#include "xdldef.h"

#ifdef GPL_SUPPORT_JPG

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API	dword_t xjpg_decompress(const byte_t* jpg_buf, dword_t jpg_len, byte_t* bmp_buf, dword_t bmp_size);

	EXP_API	dword_t xjpg_compress(const byte_t* bmp_buf, dword_t bmp_len, byte_t* jpg_buf, dword_t jpg_size);

#ifdef	__cplusplus
}
#endif

#endif /*GPL_SUPPORT_JPG*/

#endif	/* OEMJPG_H */

