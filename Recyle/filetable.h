﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc filetable document

	@module	filetable.h | interface file

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

#define FILETABLE_SHARE		0x01
#define FILETABLE_DIRECT	0x02

#define BLOCK_SIZE_512		512
#define BLOCK_SIZE_1024		1024
#define BLOCK_SIZE_2048		2048
#define BLOCK_SIZE_4096		4096

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_file_table: create a file table.
@INPUT const tchar_t* fname: the file path name.
@INPUT int block: the block allocing size, must be BLOCK_SIZE_512, BLOCK_SIZE_1024, BLOCK_SIZE_2048, BLOCK_SIZE_4096.
@INPUT dword_t mask: the mask can be FILETABLE_SHARE, FILETABLE_DIRECT or combined.
@RETURN link_t_ptr: return the file table link component.
*/
EXP_API link_t_ptr create_file_table(const tchar_t* fname, int block, dword_t mask);

/*
@FUNCTION destroy_file_table: destroy a file table.
@INPUT link_t_ptr pt: the file table link component.
@RETURN void: none.
*/
EXP_API void destroy_file_table(link_t_ptr pt);

/*
@FUNCTION get_file_table_block: get the file table block type.
@INPUT link_t_ptr pt: the file table link component.
@RETURN dword_t: return the block type.
*/
EXP_API int get_file_table_block(link_t_ptr pt);

/*
@FUNCTION get_file_table_mask: get the file table mask.
@INPUT link_t_ptr pt: the file table link component.
@RETURN dword_t: return the mask.
*/
EXP_API dword_t get_file_table_mask(link_t_ptr pt);

/*
@FUNCTION get_file_table_block: get the file table block type.
@INPUT link_t_ptr pt: the file table link component.
@RETURN dword_t: return the block type.
*/
EXP_API int get_file_table_block(link_t_ptr pt);

/*
@FUNCTION set_file_table_root: set the file table root block index.
@INPUT link_t_ptr pt: the file table link component.
@INPUT dword_t pos: the zero based index.
@RETURN bool_t: return nonzero for alloced, otherwise return zero.
*/
EXP_API bool_t set_file_table_root(link_t_ptr pt, dword_t pos);

/*
@FUNCTION get_file_table_root: get the file table root block index.
@INPUT link_t_ptr pt: the file table link component.
@RETURN dword_t: return the root block index.
*/
EXP_API dword_t get_file_table_root(link_t_ptr pt);

/*
@FUNCTION alloc_file_table_block: alloc a file block.
@INPUT link_t_ptr pt: the file table link component.
@INPUT dword_t size: the size needed.
@RETURN dword_t: if succeeds return the block index.
*/
EXP_API dword_t alloc_file_table_block(link_t_ptr pt, dword_t size);

/*
@FUNCTION free_file_table_block: free a file block.
@INPUT link_t_ptr pt: the file table link component.
@INPUT dword_t pos: the file block index.
@INPUT dword_t size: the size of file block.
@RETURN void: none.
*/
EXP_API void free_file_table_block(link_t_ptr pt, dword_t pos, dword_t size);

/*
@FUNCTION get_file_table_block_alloced: test a file block is alloced by index.
@INPUT link_t_ptr pt: the file table link component.
@INPUT dword_t pos: the file block index.
@RETURN bool_t: return nonzero for alloced, otherwise return zero.
*/
EXP_API bool_t get_file_table_block_alloced(link_t_ptr pt, dword_t pos);

/*
@FUNCTION read_file_table_block: read a file block into buffer.
@INPUT link_t_ptr pt: the file table link component.
@INPUT dword_t pos: the file block index.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t size: the buffer size.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t read_file_table_block(link_t_ptr pt, dword_t pos, byte_t* buf, dword_t size);

/*
@FUNCTION write_file_table_block: write a file block from buffer.
@INPUT link_t_ptr pt: the file table link component.
@INPUT dword_t pos: the file block index.
@INPUT byte_t* buf: the bytes buffer.
@INPUT dword_t size: the buffer size.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t write_file_table_block(link_t_ptr pt, dword_t pos, byte_t* buf, dword_t size);

EXP_API void* lock_file_table_block(link_t_ptr pt, dword_t pos, dword_t size, bool_t write, res_file_t* pmh);

EXP_API void unlock_file_table_block(link_t_ptr pt, dword_t pos, dword_t size, res_file_t mh, void* pbuf);


#if defined(XDL_SUPPORT_TEST)
EXP_API void test_file_table_alloc(const tchar_t* fname, bool_t share);

EXP_API void test_file_table_write(const tchar_t* fname, bool_t share);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_FILETABLE_H*/