/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl commole document

	@module	impcomm.h | interface file

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

#ifndef _IMPCOMM_H
#define _IMPCOMM_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_COMM

#define COMM_BASE_TIMO	(3000) //millionsecond

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xcomm_default_mode: fill default comm port config.
@OUTPUT dev_com_t* pmod: dev_com_t struct used to fill config value.
@RETURN void: none.
*/
EXP_API void xcomm_default_mode(dev_com_t* pmod);

/*
@FUNCTION xcomm_set_mode: set comm port config.
@INPUT xhand_t com: comm io handle.
@INPUT const dev_com_t* pmod: dev_com_t struct used to set config value.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xcomm_set_mode(xhand_t com, const dev_com_t* pmod);

/*
@FUNCTION xcomm_get_mode: get comm port config.
@INPUT xhand_t com: comm io handle.
@OUTPUT dev_com_t* pmod: dev_com_t struct used to return config value.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xcomm_get_mode(xhand_t com, dev_com_t* pmod);

/*
@FUNCTION xcomm_open: open comm port.
@INPUT const tchar_t* fname: comm port name, eg: "COM1", "COM2"....
@INPUT dword_t fmode: file open mode, can be combined, eg: FILE_OPEN_WRITE, FILE_OPEN_READ.
@RETURN xhand_t: if succeeds return file handle, fails return NULL.
*/
EXP_API xhand_t xcomm_open(const tchar_t* fname, dword_t fmode);

/*
@FUNCTION xcomm_close: close comm port.
@INPUT xhand_t com: comm io handle.
@RETURN void: none.
*/
EXP_API void xcomm_close(xhand_t com);

/*
@FUNCTION xcomm_listen: wait comm event coming.
@INPUT xhand_t com: comm io handle.
@OUTPUT dword_t* pcb: integer variable for return error code, line state, data queue bytes.
@RETURN dword_t: event mask. if EV_ERR, EV_BAREK returned, pcb indicate the error code.
if EV_RING, EV_RLSD, EV_CTS, EV_DSR returned, pcb indicate the line state.
if EV_RXCHAR, EV_TXTEMPTY returned, pcb indicate data queue bytes.
*/
EXP_API dword_t xcomm_listen(xhand_t com, dword_t* pcb);

/*
@FUNCTION xcomm_write: write data to comm port.
@INPUT xhand_t com: comm io handle.
@INPUT const byte_t* buf: data buffer pointer.
@INOUTPUT dword_t* pb: integer variable indicate total bytes to write, and return actually bytes writed.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
EXP_API bool_t xcomm_write(xhand_t com, const byte_t* buf, dword_t* pb);

/*
@FUNCTION xcomm_flush: flush comm port, ensure data writing come to end.
@INPUT xhand_t com: comm io handle.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
EXP_API bool_t xcomm_flush(xhand_t com);

/*
@FUNCTION xcomm_read: read data from comm port.
@INPUT xhand_t com: comm io handle.
@OUTPUT byte_t* buf: data buffer pointer.
@INOUTPUT dword_t* pb: integer variable indicate total bytes to read, and return actually bytes readed.
@RETURN bool_t: if succeeds return nonezero, fails return zero.
*/
EXP_API bool_t xcomm_read(xhand_t com, byte_t* buf, dword_t* pb);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_COMM*/

#endif /*IMPCOMM_H*/