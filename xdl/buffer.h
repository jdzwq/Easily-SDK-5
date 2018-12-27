/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl buffer document

	@module	buffer.h | xdl buffer interface file

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

#ifndef _BUFFER_H
#define _BUFFER_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API byte_t** buffer_alloc(void);

	XDL_API void	buffer_free(byte_t** pp);

	XDL_API byte_t*	buffer_realloc(byte_t** pp, dword_t size);

	XDL_API byte_t** buffer_attach(byte_t* p);

	XDL_API byte_t* buffer_detach(byte_t** pp);


#ifdef	__cplusplus
}
#endif

#endif /*_BUFFER_H*/