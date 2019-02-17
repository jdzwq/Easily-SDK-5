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

/*
@FUNCTION shell_get_curpath: get current path.
@OUTPUT tchar_t* pathbuf: the buffer for returning path.
@INPUT int pathlen: length of the buffer in characters, not include terminated character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t shell_get_curpath(tchar_t* pathbuf, int pathlen);

/*
@FUNCTION shell_get_runpath: get module runing path.
@OUTPUT tchar_t* pathbuf: the buffer for returning path.
@INPUT int pathlen: length of the buffer in characters, not include terminated character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t shell_get_runpath(tchar_t* pathbuf, int pathlen);

/*
@FUNCTION shell_get_docpath: get default system document path.
@OUTPUT tchar_t* pathbuf: the buffer for returning path.
@INPUT int pathlen: length of the buffer in characters, not include terminated character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t shell_get_docpath(tchar_t* pathbuf, int pathlen);

/*
@FUNCTION shell_get_apppath: get default system application path.
@OUTPUT tchar_t* pathbuf: the buffer for returning path.
@INPUT int pathlen: length of the buffer in characters, not include terminated character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t shell_get_apppath(tchar_t* pathbuf, int pathlen);

/*
@FUNCTION shell_get_tmppath: get default system template path.
@OUTPUT tchar_t* pathbuf: the buffer for returning path.
@INPUT int pathlen: length of the buffer in characters, not include terminated character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t shell_get_tmppath(tchar_t* pathbuf, int pathlen);

#ifdef XDK_SUPPORT_SHELL_DIALOG
/*
@FUNCTION shell_get_filename: use system file explorer to select single or multiple file name.
@INPUT res_win_t owner: the file explorer dialog owner window.
@INPUT const tchar_t* defpath: the default path opened by file explorer.
@INPUT const tchar_t* filter: the files filter list, eg: "JPG File(*.jpg)\0*.jpg\0PNG File(*.png)\0*.png\0Bitmap File(*.bmp)\0*.bmp\0".
@INPUT const tchar_t* defext: the file name default extension.
@INPUT saveit: if nonzero indicate select a filename for saving, zero indicate select some file name for opening.
@OUTPUT tchar_t* pathbuf: string buffer for return path name.
@INPUT pathlen: the path buffer length in characters, not include terminate character.
@OUTPUT tchar_t* filebuf: string buffer for return file name, if multiple file name returned, every file name seperated by '\0', the duplicate '\0\0' indicate end.
@INPUT filelen: the path buffer length in characters, not include terminate character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t shell_get_filename(res_win_t owner, const tchar_t* defpath, const tchar_t* filter, const tchar_t* defext, bool_t saveit, tchar_t* pathbuf, int pathlen, tchar_t* filebuf, int filelen);

/*
@FUNCTION shell_get_pathname: use system file explorer to select a path name.
@INPUT res_win_t owner: the file explorer dialog owner window.
@INPUT const tchar_t* defpath: the default path opened by file explorer.
@INPUT createit: if nonzero indicate select a pathame for creating, zero indicate select one path name exists.
@OUTPUT tchar_t* pathbuf: string buffer for return path name.
@INPUT pathlen: the path buffer length in characters, not include terminate character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t shell_get_pathname(res_win_t owner, const tchar_t* defpath, bool_t createit, tchar_t* pathbuf, int pathlen);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SHELL*/

#endif /*IMPSHELL_H*/