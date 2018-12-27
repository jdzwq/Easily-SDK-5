/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc shell document

	@module	impshell.h | shell interface file

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

#ifndef _IMPSHELL_H
#define _IMPSHELL_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_SHELL

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API bool_t shell_get_curpath(tchar_t* pathbuf, int pathlen);

XDL_API bool_t shell_get_runpath(tchar_t* pathbuf, int pathlen);

XDL_API bool_t shell_get_docpath(tchar_t* pathbuf, int pathlen);

XDL_API bool_t shell_get_apppath(tchar_t* pathbuf, int pathlen);

XDL_API bool_t shell_get_tmppath(tchar_t* pathbuf, int pathlen);

#ifdef XDK_SUPPORT_SHELL_DIALOG
XDL_API bool_t shell_get_filename(res_win_t owner, const tchar_t* defpath, const tchar_t* filter, const tchar_t* defext, bool_t saveit, tchar_t* pathbuf, int pathlen, tchar_t* filebuf, int filelen);

XDL_API bool_t shell_get_pathname(res_win_t owner, const tchar_t* defpath, bool_t createit, tchar_t* pathbuf, int pathlen);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SHELL*/

#endif /*IMPSHELL_H*/