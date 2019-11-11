/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc file document

	@module	impunc.h | interface file

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

#ifndef _IMPUNCF_H
#define _IMPUNCF_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_FILE

#if defined(DEBUG) || defined(_DEBUG)
#define FILE_BASE_TIMO	(-1)
#else
#define FILE_BASE_TIMO	(3000)
#endif

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xuncf_file_info: get the file information.
@INPUT const secu_desc_t* psd: the security struct.
@INPUT const tchar_t* fname: the file path name.
@OUTPUT tchar_t* ftime: string buffer for returning file GMT write time, it can be NULL.
@OUTPUT tchar_t* fsize: string buffer for returning file size, it can be NULL.
@OUTPUT tchar_t* fetag: string buffer for returning file etag, it can be NULL.
@OUTPUT tchar_t* fencode: string buffer for return file encode, eg: "utf-8", "utf-16", "gb2312", if can be NULL.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode);

/*
@FUNCTION xuncf_file_encode: get the file encode.
@INPUT const secu_desc_t* psd: the security struct.
@INPUT const tchar_t* fname: the file path name.
@RETURN int: if succeeds return _UTF8, _UTF16_LIT, _UTF16_BIG, _GB2312, fails return zero.
*/
XDL_API int xuncf_file_encode(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION xuncf_find_first: find first file in the path.
@INPUT const secu_desc_t* psd: the security struct.
@INPUT const tchar_t* path: the file path name.
@OUTPUT file_info_t* pfi: fill the file or path information.
@RETURN res_find_t: if succeeds return find handle, fails return NULL.
*/
XDL_API res_find_t xuncf_find_first(const secu_desc_t* psd, const tchar_t* path, file_info_t* pfi);

/*
@FUNCTION xuncf_find_next: find first file in the path.
@INPUT res_find_t fd: the find handle.
@OUTPUT file_info_t* pfi: fill the file or path information.
@RETURN bool_t: if succeeds nonzero, fails return zero.
*/
XDL_API bool_t xuncf_find_next(res_find_t fd, file_info_t* pfi);

/*
@FUNCTION xuncf_find_close: close find handle.
@INPUT res_find_t fd: the find handle.
@RETURN void: none.
*/
XDL_API void xuncf_find_close(res_find_t fd);

/*
@FUNCTION xuncf_open_file: open file.
@INPUT const secu_desc_t* psd: the security struct pointer.
@INPUT const tchar_t* fname: the file path name, eg:
local file: "d:/somepath/somefile"
network file: "127.0.0.1//share/somepath/somefile"
@INPUT dword_t fmode: the file open mode, can be FILE_OPEN_READ, FILE_OPEN_WRITE, FILE_OPEN_APPEND or combined.
@RETURN file_t: if succeeds return file handle, fails return NULL.
*/
XDL_API xhand_t xuncf_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode);

/*
@FUNCTION xuncf_read_file: read data from file.
@INPUT xhand_t fh: the file handle.
@OUTPUT byte_t* buf: the bytes buffer for reading.
@INOUTPUT dword_t* pb: the reading request size in bytes, and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_read_file(xhand_t fh, byte_t* buf, dword_t* pb);

/*
@FUNCTION xuncf_write_file: write data to file.
@INPUT xhand_t fh: the file handle.
@INPUT const byte_t* buf: the data buffer for writing.
@INOUTPUT dword_t* pb: the writing request size in bytes, and return the bytes writed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_write_file(xhand_t fh, const byte_t* buf, dword_t* pb);

/*
@FUNCTION xuncf_flush_file: flush file, ensure data writing come to end.
@INPUT xhand_t fh: the file handle.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
XDL_API bool_t xuncf_flush_file(xhand_t fh);

/*
@FUNCTION xuncf_read_file_range: random read file data at the start position.
@INPUT xhand_t fh: the file handle.
@INPUT dword_t hoff: the high value of 64bits position.
@INPUT dword_t loff: the low value of 64bits position.
@OUTPUT byte_t* buf: the data buffer for reading.
@INPUT dword_t size: the request size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_read_file_range(xhand_t fh, dword_t hoff, dword_t loff, byte_t* buf, dword_t size);

/*
@FUNCTION xuncf_write_file_range: random write file data at the start position.
@INPUT xhand_t fh: the file object.
@INPUT dword_t hoff: the high value of 64bits position.
@INPUT dword_t loff: the low value of 64bits position.
@INPUT const byte_t* buf: the data for writing.
@INPUT dword_t size: the data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_write_file_range(xhand_t fh, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size);

/*
@FUNCTION xuncf_setopt: set the file options.
@INPUT xhand_t fh: the file handle.
@INPUT int oid: the option id, eg: FILE_OPTION_TIME, FILE_OPTION_SINCE.
@INPUT void* opt: the option value pointer
@INPUT int len: the value length in bytes, string value must be a zero terminated token and set len to zero.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API void xuncf_setopt(xhand_t fh, int oid, void* opt, int len);

/*
@FUNCTION xuncf_truncate: truncate file content at the position.
@INPUT xhand_t fh: the file handle.
@INPUT dword_t hoff: the high value of 64bits position.
@INPUT dword_t loff: the low value of 64bits position.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_truncate(xhand_t fh, dword_t hoff, dword_t loff);

/*
@FUNCTION xuncf_close_file: close file and free file object.
@INPUT xhand_t fh: the file handle.
@RETURN void: none.
*/
XDL_API void xuncf_close_file(xhand_t fh);

/*
@FUNCTION xuncf_file_size: get file size.
@INPUT xhand_t fh: the file handle.
@OUTPUT dword_t* ph: the high value of 64bits file size.
@OUTPUT dword_t* ph: the low value of 64bits file size.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_file_size(xhand_t fh, dword_t* ph, dword_t* pl);

/*
@FUNCTION xuncf_set_filetime: set the file write time.
@INPUT xhand_t fh: the file handle.
@INPUT const tchar_t* ftime: string GMT time, eg: "Wed, 4 Jul 2001 12:08:56"
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_set_filetime(xhand_t fh, const tchar_t* ftime);

/*
@FUNCTION xuncf_get_filetime: get the file write time.
@INPUT xhand_t fh: the file handle.
@OUTPUT tchar_t* ftime: buffer for returning string GMT time, eg: "Wed, 4 Jul 2001 12:08:56"
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_get_filetime(xhand_t fh, tchar_t* ftime);

/*
@FUNCTION xuncf_delete_file: delete file by path name.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* fname: the file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_delete_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION xuncf_rename_file: change the file path name.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* fname: the file path name.
@INPUT const tchar_t* nname: the new file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_rename_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* nname);

/*
@FUNCTION xuncf_open_directory: open directory gradually, the open mode with FILE_OPEN_WRITE, the directory will be created if not exists.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* path: the path name.
@INPUT dword_t fmode: the file open mode, can be FILE_OPEN_READ, FILE_OPEN_WRITE or combined.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_open_directory(const secu_desc_t* psd, const tchar_t* path, dword_t mode);

/*
@FUNCTION xuncf_create_directory: create directory.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* pname: the path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_create_directory(const secu_desc_t* psd, const tchar_t* pname);

/*
@FUNCTION xuncf_remove_directory: remove directory.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* pname: the path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_remove_directory(const secu_desc_t* psd, const tchar_t* pname);

/*
@FUNCTION xuncf_list_file: lis files one by one in the directory.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* pname: the path name.
@INPUT CALLBACK_LISTFILE pf: the callback function for enuming file entity.
@INPUT void* pa: the param transfer into CALLBACK_LISTFILE function.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xuncf_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_FILE*/

#endif /*XFILE_H*/
