/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl inet document

	@module	inet.h | xdl http interface file

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

#ifndef _XDLINET_H
#define _XDLINET_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API xhand_t inet_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode);

	XDL_API void inet_close_file(xhand_t inet);

	XDL_API bool_t inet_read_file(xhand_t inet, byte_t* buf, dword_t* pb);

	XDL_API bool_t inet_read_file_range(xhand_t inet, dword_t hoff, dword_t loff, byte_t* buf, dword_t* pb);

	XDL_API bool_t inet_write_file(xhand_t inet, const byte_t* buf, dword_t* pb);

	XDL_API bool_t inet_write_file_range(xhand_t inet, dword_t hoff, dword_t loff, const byte_t* buf, dword_t* pb);

	XDL_API void inet_set_filetime(xhand_t inet, const tchar_t* ftime);

	XDL_API void inet_set_filesince(xhand_t inet, int since);

	XDL_API bool_t inet_delete_file(const secu_desc_t* psd, const tchar_t* fname);

	XDL_API bool_t inet_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa);

	XDL_API bool_t inet_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*INET_H*/