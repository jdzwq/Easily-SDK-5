/***********************************************************************
	Easily xdl v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc queue defination document

	@module	queue.h | queue definition interface file

	@devnote 张文权 2018.01 - 2018.12	v1.0
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


#ifndef _QUEUE_H
#define	_QUEUE_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API queue_t queue_alloc(void);

	EXP_API void queue_free(queue_t que);

	EXP_API dword_t queue_size(queue_t que);

	EXP_API void queue_copy(queue_t dst, queue_t src);

	EXP_API	dword_t queue_write(queue_t que, message_t msg);

	EXP_API dword_t queue_read(queue_t que, message_t msg);

	EXP_API dword_t queue_peek(queue_t que, message_t msg);

	EXP_API dword_t queue_encode(queue_t que, byte_t* buf, dword_t max);

	EXP_API dword_t queue_decode(queue_t que, const byte_t* data);

#if defined(XDL_SUPPORT_TEST)
	EXP_API void test_queue(void);
#endif

#ifdef	__cplusplus
}
#endif


#endif	/* _QUEUE_H */

