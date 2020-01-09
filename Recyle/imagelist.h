/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl imagelist document

	@module	imagelist.h | xdl image list interface file

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

#ifndef _IMAGELIST_H
#define _IMAGELIST_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_image_list(void);

	XDL_API void destroy_image_list(link_t_ptr il);

	XDL_API void clear_image_list(link_t_ptr il);

	XDL_API int get_image_list_count(link_t_ptr il);

	XDL_API int get_image(link_t_ptr il, const tchar_t* alt, tchar_t* src, int max);

	XDL_API const tchar_t* get_image_ptr(link_t_ptr il, const tchar_t* alt);

	XDL_API int get_image_len(link_t_ptr il, const tchar_t* alt);

	XDL_API void insert_image(link_t_ptr il, const tchar_t* alt, const tchar_t* src, int len);

	XDL_API void attach_image(link_t_ptr il, const tchar_t* alt, tchar_t* src);

	XDL_API tchar_t* detach_image(link_t_ptr il, const tchar_t* alt);

	XDL_API void delete_image(link_t_ptr il, const tchar_t* alt);

	XDL_API link_t_ptr get_image_hash_table(link_t_ptr il);

	XDL_API bool_t get_image_at(link_t_ptr il, int index, const tchar_t** palt, const tchar_t** psrc);

	XDL_API bool_t get_ximage(link_t_ptr il, const tchar_t* alt, ximage_t* pxi);

#ifdef	__cplusplus
}
#endif

#endif /*_IMAGELIST_H*/
