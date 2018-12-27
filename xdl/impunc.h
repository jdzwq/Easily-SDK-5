/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc file document

	@module	impunc.h | unc file interface file

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

#ifndef _IMPUNC_H
#define _IMPUNC_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_FILE

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API bool_t xunc_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode);

	XDL_API int xunc_file_encode(const secu_desc_t* psd, const tchar_t* fname);

	XDL_API res_find_t xunc_find_first(const secu_desc_t* psd, const tchar_t* path, file_info_t* pfi);

	XDL_API bool_t xunc_find_next(res_find_t fd, file_info_t* pfi);

	XDL_API xhand_t xunc_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode);

	XDL_API bool_t xunc_read_file(xhand_t fh, byte_t* buf, dword_t* pb);

	XDL_API bool_t xunc_write_file(xhand_t fh, const byte_t* buf, dword_t* pb);

	XDL_API bool_t xunc_flush_file(xhand_t fh);

	XDL_API bool_t xunc_read_file_range(xhand_t fh, dword_t hoff, dword_t loff, byte_t* buf, dword_t* pb);

	XDL_API bool_t xunc_write_file_range(xhand_t fh, dword_t hoff, dword_t loff, const byte_t* buf, dword_t* pb);

	XDL_API void xunc_close_file(xhand_t fh);

	XDL_API bool_t xunc_set_filetime(xhand_t fh, const tchar_t* ftime);

	XDL_API bool_t xunc_get_filetime(xhand_t fh, tchar_t* ftime);

	XDL_API bool_t xunc_delete_file(const secu_desc_t* psd, const tchar_t* fname);

	XDL_API bool_t xunc_rename_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* nname);

	XDL_API bool_t xunc_open_directory(const secu_desc_t* psd, const tchar_t* path, dword_t mode);

	XDL_API bool_t xunc_create_directory(const secu_desc_t* psd, const tchar_t* pname);

	XDL_API bool_t xunc_remove_directory(const secu_desc_t* psd, const tchar_t* pname);

	XDL_API bool_t xunc_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_FILE*/

#endif /*XFILE_H*/
