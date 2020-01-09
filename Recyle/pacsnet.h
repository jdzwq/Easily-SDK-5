/***********************************************************************
	Easily pacs web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc pacs defination document

	@module	pacsnet.h | pacs web api definition interface file

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

#ifndef _PACSNET_H
#define _PACSNET_H

#include "pacsdef.h"

#define _HANDLE_PACS	MIN_HANDLE_TYPE	+ 1

#define PDU_VER_LEN			2
#define PDU_AE_LEN			16

#define PDU_ASSOCIATE_RQ	0x01
#define PDU_ASSOCIATE_AC	0x02
#define PDU_ASSOCIATE_RJ	0x03
#define PDU_P_DATA_TF 		0x04
#define PDU_A_RELEASE_RQ	0x05
#define PDU_A_RELEASE_RP	0x06
#define PDU_A_ABORT_RQ		0x07

typedef enum{
	_PACS_TYPE_SCU = 1,
	_PACS_TYPE_SCP = 2
}PACS_TYPE;

#ifdef	__cplusplus
extern "C" {
#endif

	PACS_API xhand_t	pacs_scu(const tchar_t* dst_ae, const tchar_t* src_ae, const tchar_t* addr, const tchar_t* port);

	PACS_API xhand_t	pacs_scp(xhand_t bio);

	PACS_API void		pacs_close(xhand_t xpacs);

	PACS_API unsigned short	pacs_get_addr(xhand_t xhttp, tchar_t* addr);

	PACS_API bool_t		pacs_send(xhand_t xpacs, const byte_t* data, dword_t size);

	PACS_API bool_t		pacs_recv(xhand_t xpacs, byte_t* buf, dword_t *pb);


#ifdef	__cplusplus
}
#endif


#endif /*_H*/