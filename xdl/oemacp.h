/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl code page document

	@module	oemacp.h | interface file

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

#ifndef _OEMACP_H
#define _OEMACP_H

#include "xdldef.h"


#ifdef	__cplusplus
extern "C" {
#endif

#ifdef GPL_SUPPORT_ACP

	XDL_API acp_t create_ucs_codepage(void);

	XDL_API acp_t create_gbk_codepage(void);

	XDL_API void destroy_codepage(acp_t acp);

	XDL_API bool_t acp_get_code(acp_t acp, sword_t code, sword_t* pval);

#endif /*GPL_SUPPORT_ACP*/


XDL_API int w_help_code(const wchar_t* src, int len, wchar_t* buf, int max);

XDL_API int a_help_code(const schar_t* src, int len, schar_t* buf, int max);

#ifdef _UNICODE
#define help_code			w_help_code
#else
#define help_code			a_help_code
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_OEMACP_H*/
