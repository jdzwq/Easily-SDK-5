/***********************************************************************
	Easily xdl v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc rednet defination document

	@module	radobj.h | rad network definition interface file

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


#ifndef _RADOBJ_H
#define	_RADOBJ_H

#include "xdldef.h"

#define MSGVER_SIZE		4
#define MSGHAN_SIZE		32

#define MSGVER_SENSOR		"\x00\x00\x01\x00"
#define MSGVER_DECTOR		"\x00\x01\x00\x00"
#define MSGVER_APPLICATION	"\x01\x00\x00\x00"
#define MSG_CONFIG		_T("config")

typedef struct _rad_hdr_t{
	byte_t ver[MSGVER_SIZE];
	sword_t qos;
	sword_t mid;
	tchar_t utc[UTC_LEN+1];
}rad_hdr_t;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API	dword_t radobj_write(object_t val, const rad_hdr_t* phr, const byte_t* msg, dword_t len);

	EXP_API dword_t radobj_read(object_t val, rad_hdr_t* phr, byte_t* msg, dword_t max);

#ifdef	__cplusplus
}
#endif


#endif	/* _RADOBJ_H */

