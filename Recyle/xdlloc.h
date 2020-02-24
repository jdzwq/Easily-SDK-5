/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdl local document

	@module	xdlloc.h | xdl local interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#ifndef _XDLLOC_H
#define _XDLLOC_H

#include "xdldef.h"

XDL_API int loc_to_gb2312(const byte_t* src, int len, byte_t* buf, int max);

XDL_API int loc_to_utf8(const byte_t* src, int len, byte_t* buf, int max);

XDL_API int loc_to_utf16(const byte_t* src, int len, byte_t* buf, int max);

XDL_API int gb2312_to_loc(const byte_t* src, int len, byte_t* buf, int max);

XDL_API int utf8_to_loc(const byte_t* src, int len, byte_t* buf, int max);

XDL_API int utf16_to_loc(const byte_t* src, int len, byte_t* buf, int max);

#endif /*_XDLLOC_H*/