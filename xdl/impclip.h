/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc clipboard document

	@module	impclip.h | clipboard interface file

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

#ifndef _IMPCLIP_H
#define _IMPCLIP_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_CLIPBOARD

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION clipboard_open: open clipboard for operation.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	clipboard_open(void);

/*
@FUNCTION clipboard_close: close clipboard.
@RETURN void: none.
*/
XDL_API void	clipboard_close(void);

/*
@FUNCTION clipboard_clean: clean the clipboard content.
@RETURN void: none.
*/
XDL_API void	clipboard_clean();

/*
@FUNCTION clipboard_put: put the content into clipboard.
@INPUT int fmt: the data format, it can be mutiple characters(CB_FORMAT_MBS), unicode characers(CB_FORMAT_UCS), or binary bitmap(CB_FORMAT_DIB).
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	clipboard_put(int fmt, res_glob_t gb);

/*
@FUNCTION clipboard_get: get the content from clipboard.
@INPUT int fmt: the data format, it can be mutiple characters(CB_FORMAT_MBS), unicode characers(CB_FORMAT_UCS), or binary bitmap(CB_FORMAT_DIB).
@RETURN res_glob_t: if succeeds return global resource handle, fails return NULL.
*/
XDL_API res_glob_t clipboard_get(int fmt);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CLIPBOARD*/

#endif /*IMPCLIP_H*/