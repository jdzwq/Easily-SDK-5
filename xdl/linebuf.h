/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc linear buffer document

	@module	linebuf.h | interface file

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

#ifndef _LINEBUF_H
#define _LINEBUF_H

#include "xdldef.h"

typedef struct _LINE_BUFFER* LINEBUFFERPTR;
typedef struct _LINE_BUFFER{
	union{
		struct{
			sword_t readed;
			sword_t writed;
			byte_t* data;
		};
		struct{
			sword_t frag;
			sword_t resv;
			dword_t flag;
		};
	};
	
	LINEBUFFERPTR prev;
	LINEBUFFERPTR next;
}LINE_BUFFER;

#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API void init_line_buffer(LINEBUFFERPTR root, sword_t frag);

	XDL_API LINEBUFFERPTR alloc_line_buffer(LINEBUFFERPTR root);

	XDL_API void free_line_buffer(LINEBUFFERPTR root, LINEBUFFERPTR link);

	XDL_API LINEBUFFERPTR first_line_buffer(LINEBUFFERPTR root);

	XDL_API LINEBUFFERPTR last_line_buffer(LINEBUFFERPTR root);

	XDL_API LINEBUFFERPTR next_line_buffer(LINEBUFFERPTR link);

	XDL_API LINEBUFFERPTR prev_line_buffer(LINEBUFFERPTR link);

	XDL_API dword_t read_line_buffer(LINEBUFFERPTR root, byte_t* buf, dword_t max);

	XDL_API dword_t write_line_buffer(LINEBUFFERPTR root, const byte_t* buf, dword_t len);

#ifdef	__cplusplus
}
#endif

#endif /*LINEBUF_H*/
