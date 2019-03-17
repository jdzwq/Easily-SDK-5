/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl oss document

	@module	netoss.h | xdl oss interface file

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

#ifndef _NETOSS_H
#define _NETOSS_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION oss_ioctl: the oss file io control.
@INPUT const tchar_t* io_op: the operation code, it can be "GET", "PUT", "DELETE".
@INPUT const tchar_t* url: the restful object url.
@INPUT const tchar_t* acc_id: the security id.
@INPUT const tchar_t* acc_key: the security key.
@INPUT const byte_t* in_data: the input data bytes buffer.
@INPUT dword_t in_size: the data size in bytes.
@OUTPUT tchar_t* out_code: the string buffer for outputing code.
@OUTPUT tchar_t* out_msg: the string buffer for outputing message.
@OUTPUT tchar_t* out_type: the string buffer for outputing http content type.
@OUTPUT tchar_t* out_size: the string buffer for outputing http content length.
@OUTPUT byte_t** out_buffer: the buffer for outputing data, it must be alloced by varbuf_alloc.
@OUTPUT dword_t* out_bytes: the data size in bytes outputed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t oss_ioctl(
	const tchar_t* io_op,
	const tchar_t* url,
	const tchar_t* acc_id,
	const tchar_t* acc_key,

	const byte_t* in_data, 
	dword_t in_size, 

	tchar_t* out_code,
	tchar_t* out_msg,

	tchar_t* out_type,
	tchar_t* out_size,

	byte_t** out_buffer,
	dword_t* out_bytes
	);


#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*NETOSS_H*/