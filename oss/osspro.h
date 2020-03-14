/***********************************************************************
	Easily oss

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc oss function document

	@module	osspro.h | oss interface file

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

#ifndef _OSSPRO_H
#define _OSSPRO_H

#include "ossdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API xhand_t oss_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode);

	EXP_API void oss_close_file(xhand_t inet);

	EXP_API bool_t oss_read_file(xhand_t inet, byte_t* buf, dword_t* pb);

	EXP_API bool_t oss_write_file(xhand_t inet, const byte_t* buf, dword_t* pb);

	EXP_API bool_t oss_delete_file(const secu_desc_t* psd, const tchar_t* fname);

	EXP_API bool_t oss_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa);

	EXP_API bool_t oss_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode);

	EXP_API bool_t oss_list(const secu_desc_t* psd, const tchar_t* path, link_t_ptr ptr);

#ifdef	__cplusplus
}
#endif

#endif