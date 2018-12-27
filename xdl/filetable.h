/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc filetable document

	@module	filetable.h | filetable interface file

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

#ifndef _FILETABLE_H
#define _FILETABLE_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_file_table(const tchar_t* fname);

	XDL_API void destroy_file_table(link_t_ptr pt);

	XDL_API void set_file_table_guid(link_t_ptr pt, dword_t guid);

	XDL_API dword_t get_file_table_guid(link_t_ptr pt);

	XDL_API void set_file_table_root(link_t_ptr pt, int pos);

	XDL_API int get_file_table_root(link_t_ptr pt);

	XDL_API int alloc_file_table_block(link_t_ptr pt, dword_t size);

	XDL_API void free_file_table_block(link_t_ptr pt, int pos, dword_t size);

	XDL_API bool_t get_file_table_block_alloced(link_t_ptr pt, int pos);

	XDL_API bool_t read_file_table_block(link_t_ptr pt, int pos, byte_t* buf, dword_t size);

	XDL_API bool_t write_file_table_block(link_t_ptr pt, int pos, byte_t* buf, dword_t size);

#if defined(_DEBUG) || defined(DEBUG)
XDL_API void test_file_table(const tchar_t* fname);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_FILETABLE_H*/