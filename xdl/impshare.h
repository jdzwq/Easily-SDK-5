/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl share document

	@module	impshare.h | xdl block interface file

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

#ifndef _IMPSHARE_H
#define _IMPSHARE_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_SHARE

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t xshare_srv(const tchar_t* pname, const tchar_t* fpath, dword_t size);

XDL_API xhand_t xshare_cli(const tchar_t* bname, dword_t size);

XDL_API void xshare_close(xhand_t block);

XDL_API res_file_t xshare_handle(xhand_t block);

XDL_API bool_t xshare_write(xhand_t block, const byte_t* data, dword_t* pb);

XDL_API bool_t xshare_read(xhand_t block, byte_t* buf, dword_t* pb);

XDL_API void* xshare_lock(xhand_t block, dword_t offset, dword_t size);

XDL_API void xshare_unlock(xhand_t block, dword_t offset, dword_t size, void* p);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SHARE*/

#endif /*_IMPSHARE_H*/
