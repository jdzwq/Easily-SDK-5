/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl buffer document

	@module	varbytes.h | xdl variant bytes interface file

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
XDL_API byte_t** bytes_alloc(void);

/*
@FUNCTION bytes_free: free buffer address and buffer body.
@INPUT byte_t** pp: the buffer address.
@RETURN void: none.
*/
XDL_API void	bytes_free(byte_t** pp);

/*
@FUNCTION bytes_realloc: realloc buffer body by size.
@INPUT byte_t** pp: the buffer address.
@INPUT dword_t size: the needed buffer size.
@RETURN byte_t*: if succeed return buffer, fails return NULL.
*/
XDL_API byte_t*	bytes_realloc(byte_t** pp, dword_t size);

/*
@FUNCTION bytes_attach: alloc a buffer address and attach a buffer body.
@INPUT byte_t* p: the buffer body will be attached.
@RETURN byte_t**: if succeeds return buffer address, fails return NULL.
*/
XDL_API byte_t** bytes_attach(byte_t* p);

/*
@FUNCTION bytes_detach: detach buffer body and free buffer address.
@INPUT byte_t** pp: the buffer address.
@RETURN byte_t*: return buffer body.
*/
XDL_API byte_t* bytes_detach(byte_t** pp);


#ifdef	__cplusplus
}
#endif

#endif /*_BUFFER_H*/