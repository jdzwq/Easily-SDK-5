﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc file etag document

	@module	filetag.h | interface file

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

#ifndef _FILETAG_H
#define _FILETAG_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif


EXP_API	void a_md5_token(const byte_t dig_buf[MD5_SIZE], schar_t str_buf[MD5_LEN + 1]);

EXP_API	void w_md5_token(const byte_t dig_buf[MD5_SIZE], wchar_t str_buf[MD5_LEN + 1]);

EXP_API void file_info_etag(const tchar_t* fname, const tchar_t* ftime, const tchar_t* fsize, tchar_t* buf);

#if defined(UNICODE) || defined(_UNICODE)
#define md5_token		w_md5_token
#else
#define md5_token		a_md5_token
#endif


#ifdef	__cplusplus
}
#endif


#endif /*_XDLETAG_H*/