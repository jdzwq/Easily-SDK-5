/***********************************************************************
	Easily xdl v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xdl virtual memory document

	@module	xdlvir.h | xdl virtual memory interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#ifndef _XDLVIR_H
#define _XDLVIR_H

#include "xdldef.h"

XDL_API xhand_t virtual_alloc(void);

XDL_API void virtual_free(xhand_t xs);

XDL_API dword_t virtual_pagesize(xhand_t xs);

XDL_API bool_t	virtual_write(xhand_t xs,const byte_t* buf,dword_t* pb);

XDL_API bool_t virtual_read(xhand_t xs, byte_t* buf, dword_t* pb);

XDL_API bool_t virtual_peek(xhand_t xs, byte_t* buf, dword_t max);

XDL_API dword_t virtual_size(xhand_t xs);

XDL_API void virtual_empty(xhand_t xs);

#endif /*_XDLVIR_H*/
