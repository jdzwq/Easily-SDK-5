/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc sream document

	@module	xdlstm.h | stream interface file

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

#ifndef _XDLSTM_H
#define _XDLSTM_H

#include "xdldef.h"


typedef enum{
	BYTES_OPERA = 0,
	CHUNK_OPERA = 1,
	LINE_OPERA = 2
}STREAM_OPERA_MODE;

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API stream_t stream_alloc(xhand_t io);

XDL_API xhand_t stream_free(stream_t xs);

XDL_API xhand_t stream_attach(stream_t xs, xhand_t io, PF_BIO_READ pf_read, PF_BIO_WRITE pf_write, PF_BIO_FLUSH pf_flush);

XDL_API xhand_t stream_fetch(stream_t xs);

XDL_API xhand_t stream_detach(stream_t xs);

XDL_API void stream_set_crypt(stream_t xs, PF_BIO_ENCRYPT pf_encrypt, PF_BIO_DECRYPT pf_decrypt, void* pa);

XDL_API void stream_set_encode(stream_t xs, int encode);

XDL_API int stream_get_encode(stream_t xs);

XDL_API void stream_set_mode(stream_t xs, int mode);

XDL_API int stream_get_mode(stream_t xs);

XDL_API void stream_set_size(stream_t xs, dword_t dw);

XDL_API dword_t stream_get_size(stream_t xs);

XDL_API bool_t stream_read_chunk_size(stream_t xs, dword_t* pch);

XDL_API bool_t stream_write_chunk_size(stream_t xs, dword_t dw);

XDL_API bool_t stream_write(stream_t xs, const tchar_t* buf, int len, dword_t* pch);

XDL_API bool_t stream_flush(stream_t xs);

XDL_API bool_t stream_read(stream_t xs, tchar_t* buf, int len, dword_t* pch);

XDL_API bool_t stream_write_char(stream_t xs, const tchar_t* buf, dword_t* pch);

XDL_API bool_t stream_read_char(stream_t xs, tchar_t* buf, dword_t* pch);

XDL_API bool_t stream_read_escape(stream_t xs, tchar_t* buf,dword_t* pch);

XDL_API bool_t stream_write_escape(stream_t xs, tchar_t ch,dword_t* pch);

XDL_API bool_t stream_write_bytes(stream_t xs, const byte_t* buf, dword_t len);

XDL_API bool_t stream_read_bytes(stream_t xs, byte_t* buf, dword_t *pch);

XDL_API bool_t stream_write_sword_lit(stream_t xs, sword_t sw);

XDL_API bool_t stream_write_sword_big(stream_t xs, sword_t sw);

XDL_API bool_t stream_read_sword_lit(stream_t xs, sword_t* psw);

XDL_API bool_t stream_read_sword_big(stream_t xs, sword_t* psw);

XDL_API bool_t stream_write_dword_lit(stream_t xs, dword_t dw);

XDL_API bool_t stream_write_dword_big(stream_t xs, dword_t dw);

XDL_API bool_t stream_read_dword_lit(stream_t xs, dword_t* pdw);

XDL_API bool_t stream_read_dword_big(stream_t xs, dword_t* pdw);

XDL_API bool_t stream_read_line(stream_t xs, string_t vs, dword_t* pch);

XDL_API bool_t stream_write_line(stream_t xs, string_t vs, dword_t* pch);

XDL_API bool_t stream_write_utfbom(stream_t xs, dword_t* pch);

XDL_API bool_t stream_read_utfbom(stream_t xs, dword_t* pch);

XDL_API dword_t stream_printf(stream_t xs, const tchar_t* fmt, ...);

XDL_API bool_t stream_copy(stream_t xs_src, stream_t xs_dst);

XDL_API void stream_opera_reset(stream_t xs);


#ifdef	__cplusplus
}
#endif

#endif /*_XDLSTM_H*/
