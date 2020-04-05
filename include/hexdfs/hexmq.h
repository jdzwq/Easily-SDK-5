/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc hexdfs defination document

	@module	hexmq.h | hex message queue definition interface file

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


#ifndef _HEXMQ_H
#define	_HEXMQ_H

#include "hexdef.h"

/*the message queue format*/
/* struct message queue
{
	fixed byte[4]: the message total size(network order), not include this 4 bytes.
	fixed byte[2]: the message handler information size(network order), not include this 2 bytes.
	option byte[0~MAX_SHORT]: the message handler information.
	fixed byte[2]: the message header size(network order), not include this 2 bytes.
	option byte[0~MAX_SHORT]: the message header.
	fixed byte[4]: the message element size(network order), not in this 4 bytes;
	option byte[0~MAX_INT]: the message element.
}*/

#define MQHEADER_SIZE		96
#define TIMESTAMP_SIZE		24 //UTC format: 1970-01-01T00:00:00.000Z
#define LONGITUDE_SIZE		10 //degree format: 180:30:30
#define LATITUDE_SIZE		10 //degree format: 180:30:30
#define ALTITUDE_SIZE		16 //metre format: 100.00
#define IDENTIFY_SIZE		36 //uuid format: 00000000-0000-0000-0000-000000000000

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif


#endif	/* _HEXITER_H */

