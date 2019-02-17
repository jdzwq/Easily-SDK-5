/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl pipe document

	@module	imppipe.h | xdl pipe interface file

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

#ifndef _IMPPIPE_H
#define _IMPPIPE_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_PIPE

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xpipe_srv: create a pipe server.
@INPUT const tchar_t* pname: pipe server name.
@INPUT dword_t fmode: the file open mode, can be FILE_OPEN_READ, FILE_OPEN_WRITE, or combined.
@RETURN xhand_t: if succeeds return pipe handle, fails return NULL.
*/
XDL_API xhand_t xpipe_srv(const tchar_t* pname, dword_t fmode);

/*
@FUNCTION xpipe_cli: create a pipe client, and connect to named pipe server.
@INPUT const tchar_t* pname: pipe server name.
@INPUT dword_t fmode: the file open mode, can be FILE_OPEN_READ, FILE_OPEN_WRITE, or combined.
@RETURN xhand_t: if succeeds return pipe handle, fails return NULL.
*/
XDL_API xhand_t xpipe_cli(const tchar_t* pname, dword_t fmode);

/*
@FUNCTION xpipe_listen: the pipe server used to wiat named pipe connecting.
@INPUT xhand_t pip: server pipe handle.
@INPUT int ms: millisecond for timeout, it can be negative, if so, the waiting will be infinity.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xpipe_listen(xhand_t pip, int ms);

/*
@FUNCTION xpipe_stop: the pipe server used to stop listen.
@INPUT xhand_t pip: server pipe handle.
@RETURN void: none.
*/
XDL_API void xpipe_stop(xhand_t pip);

/*
@FUNCTION xpipe_free: free pipe.
@INPUT xhand_t pip: server or client pipe handle.
@RETURN void: none.
*/
XDL_API void xpipe_free(xhand_t pip);

/*
@FUNCTION xpipe_attach: attach a system resource handle.
@INPUT res_file_t hp: system pipe resource handle.
@RETURN xhand_t: pipe handle.
*/
XDL_API xhand_t xpipe_attach(res_file_t hp);

/*
@FUNCTION xpipe_detach: detach system resource handle, then free pipe handle.
@INPUT xhand_t pip: pipe handle.
@RETURN res_file_t: system resource handle if exists, else return NULL.
*/
XDL_API res_file_t xpipe_detach(xhand_t pip);

/*
@FUNCTION xpipe_handle: get system resource handle attached.
@INPUT xhand_t pip: pipe handle.
@RETURN res_file_t: system resource handle if exists, else return NULL.
*/
XDL_API res_file_t xpipe_handle(xhand_t pip);

/*
@FUNCTION xpipe_wait: the pipe client used to connect pipe server.
@INPUT const tchar_t* pname: server pipe name.
@INPUT int ms: millisecond for timeout, it can be negative, if so, the waiting will be infinity.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xpipe_wait(const tchar_t* pname, int ms);

/*
@FUNCTION xpipe_write: write data to pipe.
@INPUT xhand_t pip: pipe handle.
@INPUT const byte_t* buf: data buffer pointer.
@INOUTPUT dword_t* pb: integer variable indicate total bytes to write, and return actually bytes writed.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
XDL_API bool_t xpipe_write(xhand_t pip, const byte_t* data, dword_t* pb);

/*
@FUNCTION xpipe_flush: flush pipe, ensure data writing come to end.
@INPUT xhand_t pip: pipe handle.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
XDL_API bool_t xpipe_flush(xhand_t pip);

/*
@FUNCTION xpipe_read: read data from pipe.
@INPUT xhand_t pip: pipe handle.
@OUTPUT byte_t* buf: data buffer pointer.
@INOUTPUT dword_t* pb: integer variable indicate total bytes to read, and return actually bytes readed.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
XDL_API bool_t xpipe_read(xhand_t pip, byte_t* buf, dword_t* pb);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_PIPE*/

#endif /*IMPPIPE_H*/