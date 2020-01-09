/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc sream document

	@module	xdlstm.h | stream interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#ifndef _XDLSTM_H
#define _XDLSTM_H

#include "xdldef.h"

typedef bool_t(*PF_STREAM_WRITE)(xhand_t, const byte_t*, dword_t*);
typedef bool_t(*PF_STREAM_READ)(xhand_t, byte_t*, dword_t*);

typedef enum{
	BYTES_OPERA = 0,
	CHUNK_OPERA = 1,
	LINE_OPERA = 2
}STREAM_OPERA_MODE;

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API object_t stream_alloc(xhand_t io);

XDL_API xhand_t stream_free(object_t xs);

XDL_API xhand_t stream_attach(object_t xs, xhand_t io, PF_STREAM_READ pf_read, PF_STREAM_WRITE pf_write);

XDL_API xhand_t stream_fetch(object_t xs);

XDL_API xhand_t stream_detach(object_t xs);

XDL_API void stream_set_encode(object_t xs, int encode);

XDL_API int stream_get_encode(object_t xs);

XDL_API void stream_set_mode(object_t xs, int mode);

XDL_API int stream_get_mode(object_t xs);

XDL_API void stream_set_size(object_t xs, dword_t dw);

XDL_API dword_t stream_get_size(object_t xs);

XDL_API bool_t stream_read_chunk_size(object_t xs, dword_t* pb);

XDL_API bool_t stream_write_chunk_size(object_t xs, dword_t dw);

XDL_API bool_t stream_write(object_t xs, const tchar_t* buf, int len, dword_t* pb);

XDL_API bool_t stream_read(object_t xs, tchar_t* buf, int len, dword_t* pch);

XDL_API bool_t stream_write_char(object_t xs, const tchar_t* buf, dword_t* pb);

XDL_API bool_t stream_read_char(object_t xs, tchar_t* buf, dword_t* pb);

XDL_API bool_t stream_read_escape(object_t xs, tchar_t* buf,dword_t* pb);

XDL_API bool_t stream_write_escape(object_t xs, tchar_t ch,dword_t* pb);

XDL_API bool_t stream_write_bytes(object_t xs, const byte_t* buf, dword_t len,dword_t* pb);

XDL_API bool_t stream_read_bytes(object_t xs, byte_t* buf, dword_t len,dword_t* pb);

XDL_API bool_t stream_read_line(object_t xs, object_t vs, dword_t* pb);

XDL_API bool_t stream_write_line(object_t xs, object_t vs, dword_t* pb);

XDL_API bool_t stream_write_utfbom(object_t xs, dword_t* pb);

XDL_API bool_t stream_read_utfbom(object_t xs, dword_t* pb);

XDL_API dword_t stream_printf(object_t xs, const tchar_t* fmt, ...);

XDL_API bool_t stream_copy(object_t xs_src, object_t xs_dst);

XDL_API void stream_opera_reset(object_t xs);


#ifdef	__cplusplus
}
#endif

#endif /*_XDLSTM_H*/
