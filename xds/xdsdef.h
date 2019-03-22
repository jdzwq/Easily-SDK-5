/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	xdsdef.h | definition interface file

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


#ifndef _XDSDEF_H
#define	_XDSDEF_H

#include <xdl.h>

#if defined(_OS_WINDOWS) && defined(_USRDLL)
#define XDS_API __declspec(dllexport)
#else
#define XDS_API extern
#endif

typedef void(*PF_LOG_TITLE)(file_t, const tchar_t*, int);
typedef void(*PF_LOG_ERROR)(file_t, const tchar_t*, const tchar_t*, int);
typedef void(*PF_LOG_DATA)(file_t, const byte_t*, dword_t);
typedef void(*PF_LOG_XML)(file_t, link_t_ptr);
typedef void(*PF_LOG_JSON)(file_t, link_t_ptr);


#endif	/* _XDSDEF_H */

