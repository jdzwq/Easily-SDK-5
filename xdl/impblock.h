/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl block document

	@module	impblock.h | xdl block interface file

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

#ifndef _IMPBLOCK_H
#define _IMPBLOCK_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_FILE_BLOCK

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t xblock_open(const tchar_t* fname, dword_t fmode);

XDL_API void xblock_close(xhand_t block);

XDL_API bool_t xblock_size(xhand_t block, dword_t* ph, dword_t* pl);

XDL_API res_file_t xblock_handle(xhand_t block);

XDL_API bool_t xblock_write(xhand_t block, dword_t hoff, dword_t loff, const byte_t* data, dword_t size);

XDL_API bool_t xblock_read(xhand_t block, dword_t hoff, dword_t loff, byte_t* buf, dword_t size);

XDL_API bool_t xblock_truncate(xhand_t block, dword_t hoff, dword_t loff);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_FILE_BLOCK*/

#endif /*_IMPBLOCK_H*/