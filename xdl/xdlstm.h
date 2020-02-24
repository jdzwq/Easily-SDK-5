/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc sream document

	@module	stream.h | interface file

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

/*
@FUNCTION stream_alloc: alloc a stream object.
@INPUT xhand_t io: the io handle, it can be pnp, tcp, ssl, pipe, comm, cons, share, file, http, tftp handle.
@RETURN stream_t: the new stream object.
*/
XDL_API stream_t stream_alloc(xhand_t io);

/*
@FUNCTION stream_free: free the stream object.
@INPUT stream_t xs: the stream object.
@RETURN void: none.
*/
XDL_API xhand_t stream_free(stream_t xs);

/*
@FUNCTION stream_attach: attach a io handle and its read write function to stream object.
@INPUT stream_t xs: the stream object.
@INPUT xhand_t io: the io handle, it can be pnp, tcp, ssl, pipe, comm, cons, share, file, http, tftp handle.
@RETURN xhand_t: return the orginal io handle if exists, otherwise return NULL.
*/
XDL_API xhand_t stream_attach(stream_t xs, xhand_t io, PF_BIO_READ pf_read, PF_BIO_WRITE pf_write, PF_BIO_FLUSH pf_flush);

/*
@FUNCTION stream_fetch: get the io handle from stream object.
@INPUT stream_t xs: the stream object.
@RETURN xhand_t: return the io handle if exists, otherwise return NULL.
*/
XDL_API xhand_t stream_fetch(stream_t xs);

/*
@FUNCTION stream_detach: remove the io handle from stream object.
@INPUT stream_t xs: the stream object.
@RETURN xhand_t: return the io handle if exists, otherwise return NULL.
*/
XDL_API xhand_t stream_detach(stream_t xs);

/*
@FUNCTION stream_set_encode: set the stream object encode.
@INPUT stream_t xs: the stream object.
@INPUT int encode: the encode, it can be _UTF8, _UTF16_BIG, _UTF16_LIT.
@RETURN void: none.
*/
XDL_API void stream_set_encode(stream_t xs, int encode);

/*
@FUNCTION stream_get_encode: get the stream object encode.
@INPUT stream_t xs: the stream object.
@RETURN int: return encode, it can be _UTF8, _UTF16_BIG, _UTF16_LIT or zero.
*/
XDL_API int stream_get_encode(stream_t xs);

/*
@FUNCTION stream_set_mode: set the stream object read and write mode.
@INPUT stream_t xs: the stream object.
@INPUT int mode: read and write mode, it can be BYTES_OPERA, CHUNK_OPERA, LINE_OPERA, the default mode is BYTES_OPERA.
@RETURN void: none.
*/
XDL_API void stream_set_mode(stream_t xs, int mode);

/*
@FUNCTION stream_get_mode: get the stream object read and write mode.
@INPUT stream_t xs: the stream object.
@RETURN int: return read and write mode, it can be BYTES_OPERA, CHUNK_OPERA, LINE_OPERA.
*/
XDL_API int stream_get_mode(stream_t xs);

/*
@FUNCTION stream_set_size: set the stream object operating maximized size in bytes. 
@INPUT stream_t xs: the stream object.
@INPUT dw: the size in bytes for operating limit.
@RETURN void: none.
*/
XDL_API void stream_set_size(stream_t xs, dword_t dw);

/*
@FUNCTION stream_get_size: get the stream object operating maximized size in bytes.
@INPUT stream_t xs: the stream object.
@RETURN dword_t: return the operation limit size in bytes if setted, otherwise return zero.
*/
XDL_API dword_t stream_get_size(stream_t xs);

/*
@FUNCTION stream_read_chunk_size: read chunk size from stream.
@INPUT stream_t xs: the stream object.
@OUTPUT dword_t* pch: for return chunk size.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_chunk_size(stream_t xs, dword_t* pch);

/*
@FUNCTION stream_write_chunk_size: write chunk size to stream.
@INPUT stream_t xs: the stream object.
@INPUT dword_t dw: the chunk size.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_chunk_size(stream_t xs, dword_t dw);

/*
@FUNCTION stream_write: write string token to stream.
@INPUT stream_t xs: the stream object.
@INPUT const tchar_t* buf: the string buffer.
@INPUT int len: the string token length in characters.
@OUTPUT dword_t* pch: for return the size of data writed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write(stream_t xs, const tchar_t* buf, int len, dword_t* pch);

/*
@FUNCTION stream_flush: ensure io handle data writing completed.
@INPUT stream_t xs: the stream object.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_flush(stream_t xs);

/*
@FUNCTION stream_read: read string token from stream.
@INPUT stream_t xs: the stream object.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int len: the string buffer length in characters, not include terminate character.
@OUTPUT dword_t* pch: for return the size of data readed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read(stream_t xs, tchar_t* buf, int len, dword_t* pch);

/*
@FUNCTION stream_write_char: write multiple character into stream.
@INPUT stream_t xs: the stream object.
@INPUT const tchar_t* buf: the multiple character.
@OUTPUT dword_t* pch: for return the size of data wried in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_char(stream_t xs, const tchar_t* buf, dword_t* pch);

/*
@FUNCTION stream_read_char: read a multiple character from stream.
@INPUT stream_t xs: the stream object.
@OUTPUT tchar_t* buf: the multiple character buffer.
@OUTPUT dword_t* pch: for return the size of data readed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_char(stream_t xs, tchar_t* buf, dword_t* pch);

/*
@FUNCTION stream_read_escape: read a escape character from stream.
@INPUT stream_t xs: the stream object.
@OUTPUT tchar_t* buf: the character buffer.
@OUTPUT dword_t* pch: for return the size of data readed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_escape(stream_t xs, tchar_t* buf, dword_t* pch);

/*
@FUNCTION stream_write_escape: write a escape character from stream.
@INPUT stream_t xs: the stream object.
@INPUT tchar_t ch: the escape character.
@OUTPUT dword_t* pch: for return the size of data readed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_escape(stream_t xs, tchar_t ch, dword_t* pch);

/*
@FUNCTION stream_write_bytes: write some bytes into stream.
@INPUT stream_t xs: the stream object.
@INPUT const byte_t* buf: the bytes buffer.
@INPUT dword_t len: the bytes to write.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_bytes(stream_t xs, const byte_t* buf, dword_t len);

/*
@FUNCTION stream_read_bytes: read some bytes from stream.
@INPUT stream_t xs: the stream object.
@OUTPUT byte_t* buf: the bytes buffer.
@INOUTPUT dword_t* pch: the bytes want to read, and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_bytes(stream_t xs, byte_t* buf, dword_t *pch);

/*
@FUNCTION stream_write_sword_lit: write a unsigned short data into stream using Little Endian.
@INPUT stream_t xs: the stream object.
@INPUT sword_t sw: the unsigned short data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_sword_lit(stream_t xs, sword_t sw);

/*
@FUNCTION stream_write_sword_big: write a unsigned short data into stream using Big Endian.
@INPUT stream_t xs: the stream object.
@INPUT sword_t sw: the unsigned short data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_sword_big(stream_t xs, sword_t sw);

/*
@FUNCTION stream_read_sword_lit: read a unsigned short data from stream using Little Endian.
@INPUT stream_t xs: the stream object.
@OUTPUT sword_t* psw: for return unsigned short data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_sword_lit(stream_t xs, sword_t* psw);

/*
@FUNCTION stream_read_sword_big: read a unsigned short data from stream using Big Endian.
@INPUT stream_t xs: the stream object.
@OUTPUT sword_t* psw: for return unsigned short data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_sword_big(stream_t xs, sword_t* psw);

/*
@FUNCTION stream_write_dword_lit: write a unsigned int data into stream using Little Endian.
@INPUT stream_t xs: the stream object.
@INPUT dword_t dw: the unsigned int data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_dword_lit(stream_t xs, dword_t dw);

/*
@FUNCTION stream_write_dword_big: write a unsigned int data into stream using Big Endian.
@INPUT stream_t xs: the stream object.
@INPUT dword_t dw: the unsigned int data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_dword_big(stream_t xs, dword_t dw);

/*
@FUNCTION stream_read_dword_lit: read a unsigned int data from stream using Little Endian.
@INPUT stream_t xs: the stream object.
@OUTPUT dword_t* pdw: for return unsigned short data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_dword_lit(stream_t xs, dword_t* pdw);

/*
@FUNCTION stream_read_dword_big: read a unsigned int data from stream using Big Endian.
@INPUT stream_t xs: the stream object.
@OUTPUT dword_t* pdw: for return unsigned int data.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_dword_big(stream_t xs, dword_t* pdw);

/*
@FUNCTION stream_read_line: read one line string token into string object from stream.
@INPUT stream_t xs: the stream object.
@INPUT string_t vs: the string object
@OUTPUT dword_t* pch: for return data readed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_line(stream_t xs, string_t vs, dword_t* pch);

/*
@FUNCTION stream_write_line: write one line string token into stream from string object.
@INPUT stream_t xs: the stream object.
@INPUT string_t vs: the string object
@OUTPUT dword_t* pch: for return data writed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_line(stream_t xs, string_t vs, dword_t* pch);

/*
@FUNCTION stream_write_utfbom: write a BOM into stream, according to the encode setted.
@INPUT stream_t xs: the stream object.
@OUTPUT dword_t* pch: for return data writed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_write_utfbom(stream_t xs, dword_t* pch);

/*
@FUNCTION stream_read_utfbom: read a BOM from stream, according to the encode setted.
@INPUT stream_t xs: the stream object.
@OUTPUT dword_t* pch: for return data readed in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_read_utfbom(stream_t xs, dword_t* pch);

/*
@FUNCTION stream_printf: printf string token into stream.
@INPUT stream_t xs: the stream object.
@INPUT const tchar_t* fmt: the string format token.
@INPUT ...: the variable parameters.
@RETURN dword_t: if succeeds return the data writed in bytes, fails return zero.
*/
XDL_API dword_t stream_printf(stream_t xs, const tchar_t* fmt, ...);

/*
@FUNCTION stream_copy: copy source stream to destination stream.
@INPUT stream_t xs_src: the source stream object.
@INPUT stream_t xs_dst: the destination stream object.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t stream_copy(stream_t xs_src, stream_t xs_dst);

/*
@FUNCTION stream_opera_reset: clear the stream read and write counter.
@INPUT stream_t xs: the stream object.
@RETURN void: none.
*/
XDL_API void stream_opera_reset(stream_t xs);


#ifdef	__cplusplus
}
#endif

#endif /*_STREAM_H*/
