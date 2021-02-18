/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl file document

	@module	file.h | interface file

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

#ifndef _FILEINF_H
#define _FILEINF_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xfile_open: open file.
@INPUT const secu_desc_t* psd: the security struct pointer.
@INPUT const tchar_t* fname: the file path name, eg:
local file: "d:/somepath/somefile"
network file: "//127.0.0.1/sharedir/somepath/somefile"
http file: "http://127.0.0.1/virdir/somefile"
tftp file: "tftp://127.0.0.1/virdir/somefile"
oss file: "oss://127.0.0.1/virdir/somefile"
@INPUT dword_t fmode: the file open mode, can be FILE_OPEN_READ, FILE_OPEN_WRITE, FILE_OPEN_APPEND or combined.
@RETURN file_t: if succeeds return file object, fails return NULL.
*/
EXP_API file_t	xfile_open(const secu_desc_t* psd, const tchar_t* fname, dword_t fmode);

/*
@FUNCTION xfile_read: read data from file.
@INPUT file_t fh: the file object.
@OUTPUT byte_t* buf: the bytes buffer for reading.
@INPUT dword_t size: the request size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_read(file_t xf, byte_t* buf, dword_t size);

/*
@FUNCTION xfile_write: write data to file.
@INPUT file_t fh: the file object.
@INPUT const byte_t* buf: the data buffer for writing.
@INPUT dword_t size: the data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_write(file_t xf, const byte_t* buf, dword_t size);

/*
@FUNCTION xfile_flush: flush file, ensure data writing come to end.
@INPUT file_t fh: the file object.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
EXP_API bool_t		xfile_flush(file_t xf);

/*
@FUNCTION xfile_read_range: random read file data at the start position.
@INPUT file_t fh: the file object.
@INPUT dword_t hoff: the high value of 64bits position.
@INPUT dword_t loff: the low value of 64bits position.
@OUTPUT byte_t* buf: the data buffer for reading.
@INPUT dword_t size: the request size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_read_range(file_t xf, dword_t hoff, dword_t loff, byte_t* buf, dword_t size);

/*
@FUNCTION xfile_write_range: random write file data at the start position.
@INPUT file_t fh: the file object.
@INPUT dword_t hoff: the high value of 64bits position.
@INPUT dword_t loff: the low value of 64bits position.
@INPUT const byte_t* buf: the data for writing.
@INPUT dword_t size: the data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_write_range(file_t xf, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size);

/*
@FUNCTION xfile_close: close file and free file object.
@INPUT file_t fh: the file object.
@RETURN void: none.
*/
EXP_API void		xfile_close(file_t xf);

/*
@FUNCTION xfile_settime: set the file write time.
@INPUT file_t fh: the file object.
@INPUT const tchar_t* ftime: string GMT time, eg: "Wed, 4 Jul 2001 12:08:56"
@RETURN bool_t: return nonzero if supported.
*/
EXP_API bool_t		xfile_settime(file_t xf, const tchar_t* ftime);

/*
@FUNCTION xfile_setsince: set the file write since.
@INPUT file_t fh: the file object.
@INPUT int fs: file since tag, eg: FILE_SINCE_TIME, FILE_SINCE_ETAG.
@RETURN bool_t: return nonzero if supported.
*/
EXP_API bool_t		xfile_setsince(file_t xf, int fs);

/*
@FUNCTION xfile_info: get the file information.
@INPUT const secu_desc_t* psd: the security struct.
@INPUT const tchar_t* fname: the file path name.
@OUTPUT tchar_t* ftime: string buffer for returning file GMT write time, it can be NULL.
@OUTPUT tchar_t* fsize: string buffer for returning file size, it can be NULL.
@OUTPUT tchar_t* fetag: string buffer for returning file etag, it can be NULL.
@OUTPUT tchar_t* fencode: string buffer for return file encode, eg: "utf-8", "utf-16", "gb2312", if can be NULL.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode);

/*
@FUNCTION xfile_copy: copy source file to destination.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* srcfile: the source file path name.
@INPUT const tchar_t* destfile: the destination file path name.
@INPUT int fsince: the overwrite tag, with FILE_SINCE_TIME tag, if the destination file exists and file-write time new then source file, overwriting will discard and return nonezero.
with FILE_SINE_ETAG, if the destination file exists and file-etag equal to source file, overwriting will discard and return nonezero.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_copy(const secu_desc_t* psd, const tchar_t* srcfile, const tchar_t* destfile, int fsince);

/*
@FUNCTION xfile_delete: delete file by path name.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* fname: the file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_delete(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION xfile_rename: change the file path name.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* fname: the file path name.
@INPUT const tchar_t* nname: the new file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_rename(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* nname);

/*
@FUNCTION xfile_mkdir: make a directory.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* path: the path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_mkdir(const secu_desc_t* psd, const tchar_t* path);

/*
@FUNCTION xfile_rmdir: remove a directory, the directory must be empty.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* path: the path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_rmdir(const secu_desc_t* psd, const tchar_t* path);

#if defined(XDL_SUPPORT_DOC)
/*
@FUNCTION xfile_list: list a directory child items.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* path: the path name.
@INPUT link_t_ptr ptr: the list document link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_list(const secu_desc_t* psd, const tchar_t* path, link_t_ptr ptr);

/*
@FUNCTION xfile_tree: list a directory child and all sub child items.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* path: the path name.
@INPUT link_t_ptr ptr: the list document link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API void		xfile_tree(const secu_desc_t* psd, const tchar_t* path, link_t_ptr ptr);

#endif

/*
@FUNCTION xfile_dump: dump a directory child items.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* path: the path name.
@INPUT stream_t stm: the stream object.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t		xfile_dump(const secu_desc_t* psd, const tchar_t* path, stream_t stm);

#ifdef	__cplusplus
}
#endif


#endif /*FILE_H*/
