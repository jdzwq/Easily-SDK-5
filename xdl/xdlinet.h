/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl inet document

	@module	inet.h | interface file

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

/*
@FUNCTION xinet_open_file: open file.
@INPUT const secu_desc_t* psd: the security struct pointer.
@INPUT const tchar_t* fname: the file path name, eg:
http file: "http://127.0.0.1/virpath/somefile"
http file: "tftp://127.0.0.1/virpath/somefile"
@INPUT dword_t fmode: the file open mode, can be FILE_OPEN_READ, FILE_OPEN_WRITE, FILE_OPEN_APPEND or combined.
@RETURN file_t: if succeeds return file handle, fails return NULL.
*/
EXP_API xhand_t xinet_open_file(const secu_desc_t* psd, const tchar_t* fname, dword_t mode);

/*
@FUNCTION xinet_close_file: close file and free file object.
@INPUT xhand_t inet: the file handle.
@RETURN void: none.
*/
EXP_API void xinet_close_file(xhand_t inet);

/*
@FUNCTION xinet_read_file: read data from file.
@INPUT xhand_t inet: the file handle.
@OUTPUT byte_t* buf: the bytes buffer for reading.
@INOUTPUT dword_t* pb: the reading request size in bytes, and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xinet_read_file(xhand_t inet, byte_t* buf, dword_t* pb);

/*
@FUNCTION xinet_write_file: write data to file.
@INPUT xhand_t inet: the file handle.
@INPUT const byte_t* buf: the data buffer for writing.
@INOUTPUT dword_t* pb: the writing request size in bytes, and return the bytes writed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xinet_write_file(xhand_t inet, const byte_t* buf, dword_t* pb);

/*
@FUNCTION xinet_write_file_range: random write file data at the start position.
@INPUT xhand_t inet: the file object.
@INPUT dword_t hoff: the high value of 64bits position.
@INPUT dword_t loff: the low value of 64bits position.
@INPUT const byte_t* buf: the data for writing.
@INPUT dword_t size: the data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xinet_write_file_range(xhand_t inet, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size);

/*
@FUNCTION xinet_read_file_range: random read file data at the start position.
@INPUT xhand_t inet: the file handle.
@INPUT dword_t hoff: the high value of 64bits position.
@INPUT dword_t loff: the low value of 64bits position.
@OUTPUT byte_t* buf: the data buffer for reading.
@INPUT dword_t size: the request size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xinet_read_file_range(xhand_t inet, dword_t hoff, dword_t loff, byte_t* buf, dword_t size);

/*
@FUNCTION xinet_setopt: set the file options.
@INPUT xhand_t inet: the file handle.
@INPUT int oid: the option id, eg: FILE_OPTION_TIME, FILE_OPTION_SINCE.
@INPUT void* opt: the option value pointer
@INPUT int len: the value length in bytes, string value must be a zero terminated token and set len to zero.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xinet_setopt(xhand_t inet, int oid, void* opt, int len);

/*
@FUNCTION xinet_set_filetime: set the file write time.
@INPUT xhand_t inet: the file handle.
@INPUT const tchar_t* ftime: string GMT time, eg: "Wed, 4 Jul 2001 12:08:56"
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API void xinet_set_filetime(xhand_t inet, const tchar_t* ftime);

/*
@FUNCTION xinet_set_filesince: set the file write since.
@INPUT xhand_t inet: the file handle.
@INPUT int since: file since tag, eg: FILE_SINCE_TIME, FILE_SINCE_ETAG.
@RETURN void: none.
*/
EXP_API void xinet_set_filesince(xhand_t inet, int since);

/*
@FUNCTION xinet_delete_file: delete file by path name.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* fname: the file path name.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xinet_delete_file(const secu_desc_t* psd, const tchar_t* fname);

/*
@FUNCTION xinet_list_file: lis files one by one in the directory.
@INPUT const secu_desc_t* psd: the security struct for writing destination file.
@INPUT const tchar_t* pname: the path name.
@INPUT CALLBACK_LISTFILE pf: the callback function for enuming file entity.
@INPUT void* pa: the param transfer into CALLBACK_LISTFILE function.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xinet_list_file(const secu_desc_t* psd, const tchar_t* path, CALLBACK_LISTFILE pf, void* pa);

/*
@FUNCTION xinet_file_info: get the file information.
@INPUT const secu_desc_t* psd: the security struct.
@INPUT const tchar_t* fname: the file path name.
@OUTPUT tchar_t* ftime: string buffer for returning file GMT write time, it can be NULL.
@OUTPUT tchar_t* fsize: string buffer for returning file size, it can be NULL.
@OUTPUT tchar_t* fetag: string buffer for returning file etag, it can be NULL.
@OUTPUT tchar_t* fencode: string buffer for return file encode, eg: "utf-8", "utf-16", "gb2312", if can be NULL.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xinet_file_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*_INTERNET_H*/