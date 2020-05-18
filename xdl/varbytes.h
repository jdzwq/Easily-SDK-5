/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl buffer document

	@module	varbytes.h | interface file

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

#ifndef _VARBYTES_H
#define _VARBYTES_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION bytes_alloc: alloc a buffer address, now the buffer has no body.
@RETURN byte_t**: if succeeds return buffer address, fails return NULL.
*/
EXP_API byte_t** bytes_alloc(void);

/*
@FUNCTION bytes_free: free buffer address and buffer body.
@INPUT byte_t** pp: the buffer address.
@RETURN void: none.
*/
EXP_API void	bytes_free(byte_t** pp);

/*
@FUNCTION bytes_realloc: realloc buffer body by size.
@INPUT byte_t** pp: the buffer address.
@INPUT dword_t size: the needed buffer size.
@RETURN byte_t*: if succeed return buffer, fails return NULL.
*/
EXP_API byte_t*	bytes_realloc(byte_t** pp, dword_t size);

/*
@FUNCTION bytes_attach: alloc a buffer address and attach a buffer body, the original buffer body will be freed.
@INPUT byte_t** pp: the buffer address.
@INPUT byte_t* p: the buffer body will be attached.
@INPUT dword_t size: the buffer size attached.
@RETURN void: none.
*/
EXP_API void bytes_attach(byte_t** pp, byte_t* p, dword_t size);

/*
@FUNCTION bytes_detach: detach buffer body and free buffer address.
@INPUT byte_t** pp: the buffer address.
@RETURN byte_t*: return buffer body.
*/
EXP_API byte_t* bytes_detach(byte_t** pp);

/*
@FUNCTION bytes_size: get buffer size.
@INPUT byte_t** pp: the buffer address.
@RETURN dword_t: return buffer size in bytes.
*/
EXP_API dword_t bytes_size(byte_t** pp);

/*
@FUNCTION bytes_copy: copy bytes from buffer.
@INPUT byte_t** pp: the buffer address.
@INPUT dword_t pos: the zero based position.
@OUTPUT byte_t* sub: the sub buffer for returning copyed bytes.
@INOUTPUT dword_t* plen: input the needed size and return the actual size copyed.
@RETURN void: none.
*/
EXP_API void bytes_copy(byte_t** pp, dword_t pos, byte_t* sub, dword_t* plen);

/*
@FUNCTION bytes_insert: insert token into buffer.
@INPUT byte_t** pp: the buffer address.
@INPUT dword_t pos: the zero based position.
@INPUT const byte_t* sub: the sub token to be inserted.
@INPUT dword_t len: the sub token size in bytes.
@RETURN void: none.
*/
EXP_API void bytes_insert(byte_t** pp, dword_t pos, const byte_t* sub, dword_t len);

/*
@FUNCTION bytes_delete: delete token from buffer.
@INPUT byte_t** pp: the buffer address.
@INPUT dword_t pos: the zero based position.
@INPUT dword_t len: the sub token size in bytes.
@RETURN void: none.
*/
EXP_API void bytes_delete(byte_t** pp, dword_t pos, dword_t len);

/*
@FUNCTION bytes_replace: replace token from buffer.
@INPUT byte_t** pp: the buffer address.
@INPUT dword_t pos: the zero based position.
@INPUT dword_t num: the bytes of token to be replaced.
@INPUT const byte_t* sub: the sub token to be inserted.
@INPUT dword_t len: the sub token size in bytes.
@RETURN void: none.
*/
EXP_API void bytes_replace(byte_t** pp, dword_t pos, dword_t num, const byte_t* sub, dword_t len);

#ifdef	__cplusplus
}
#endif

#endif /*_BUFFER_H*/