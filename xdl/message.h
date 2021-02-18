/***********************************************************************
	Easily xdl v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc message defination document

	@module	varmsg.h | message definition interface file

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


#ifndef _MESSAGE_H
#define	_MESSAGE_H

#include "xdldef.h"

#define MSGVER_SIZE		4
#define MSGHAN_SIZE		32

#define MSGVER_SENSOR	0x00000100
#define MSGVER_DECTOR	0x00010000
#define MSGVER_VENDOR	0x01000000
#define MSGID_CONFIG	0x00FFFFFF

typedef struct _msg_hdr_t{
	dword_t ver;
	byte_t qos;
	dword_t seq;
	lword_t utc;
}msg_hdr_t;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API message_t message_alloc(void);

	EXP_API void message_free(message_t msg);

	EXP_API dword_t message_size(message_t msg);

	EXP_API void message_borrow(message_t msg, byte_t* buf);

	EXP_API byte_t* message_revert(message_t msg);

	EXP_API void message_copy(message_t dst, message_t src);

	EXP_API	dword_t message_write(message_t msg, const msg_hdr_t* phr, const byte_t* buf, dword_t len);

	EXP_API dword_t message_read(message_t msg, msg_hdr_t* phr, byte_t* buf, dword_t max);

	EXP_API dword_t message_encode(message_t msg, byte_t* buf, dword_t max);

	EXP_API dword_t message_decode(message_t msg, const byte_t* data);

#if defined(XDL_SUPPORT_TEST)
	EXP_API void test_message(void);
#endif

#ifdef	__cplusplus
}
#endif


#endif	/* _RADOBJ_H */

