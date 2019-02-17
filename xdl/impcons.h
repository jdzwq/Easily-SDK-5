/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl console document

	@module	impcons.h | xdl console interface file

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

#ifndef _IMPCONS_H
#define _IMPCONS_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_CONS

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xcons_alloc: create a console.
@RETURN xhand_t: if succeeds return console handle, fails return NULL.
*/
XDL_API xhand_t xcons_alloc(void);

/*
@FUNCTION xcons_name: get the console name.
@RETURN const tchar_t*: if succeeds return console name, fails return NULL.
*/
XDL_API const tchar_t* xcons_name(xhand_t con);

/*
@FUNCTION xcons_stdin: get the console input system resource handle, the STDIN hanle used to read data from console.
@INPUT xhand_t con: console handle.
@RETURN res_file_t: if succeeds return system resource handle, fails return INVALID_FILE.
*/
XDL_API res_file_t xcons_stdin(xhand_t con);

/*
@FUNCTION xcons_stdout: get the console output system resource handle, the STDOUT hanle used to write data to console.
@INPUT xhand_t con: console handle.
@RETURN res_file_t: if succeeds return system resource handle, fails return NULL.
*/
XDL_API res_file_t xcons_stdout(xhand_t con);

/*
@FUNCTION xcons_free: free the console.
@INPUT xhand_t con: console handle.
@RETURN void: none.
*/
XDL_API void xcons_free(xhand_t con);

/*
@FUNCTION xcons_sigaction: install a signal monitor function into console.
@INPUT xhand_t con: console handle.
@INPUT PF_SIGHANDLER pf: the signal fonction.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xcons_sigaction(xhand_t con, PF_SIGHANDLER pf);

/*
@FUNCTION xcons_write: write data to console.
@INPUT xhand_t con: console handle.
@INPUT const byte_t* buf: data buffer pointer.
@INOUTPUT dword_t* pb: integer variable indicate total bytes to write, and return actually bytes writed.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
XDL_API bool_t xcons_write(xhand_t con, const byte_t* buf, dword_t* pb);

/*
@FUNCTION xcons_flush: flush console, ensure data writing come to end.
@INPUT xhand_t con: console handle.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
XDL_API bool_t xcons_flush(xhand_t con);

/*
@FUNCTION xcons_read: read data from console.
@INPUT xhand_t con: console handle.
@OUTPUT byte_t* buf: data buffer pointer.
@INOUTPUT dword_t* pb: integer variable indicate total bytes to read, and return actually bytes readed.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
XDL_API bool_t xcons_read(xhand_t con, byte_t* buf, dword_t* pb);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONS*/

#endif /*IMPCONS_H*/