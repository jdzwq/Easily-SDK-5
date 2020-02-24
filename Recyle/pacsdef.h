/***********************************************************************
	Easily pacs

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc pacs defination document

	@module	pacsdef.h | definition interface file

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


#ifndef _PACSDEF_H
#define	_PACSDEF_H

#include <xdl.h>
#include <dicm.h>

#if defined(_OS_WINDOWS) && defined(_USRDLL)
#define PACS_API __declspec(dllexport)
#else
#define PACS_API extern
#endif

#define PDU_VER_LEN			2
#define PDU_AE_LEN			16

#define PDU_ASSOCIATE_RQ	0x01
#define PDU_ASSOCIATE_AC	0x02
#define PDU_ASSOCIATE_RJ	0x03
#define PDU_P_DATA_TF 		0x04
#define PDU_A_RELEASE_RQ	0x05
#define PDU_A_RELEASE_RP	0x06
#define PDU_A_ABORT_RQ		0x07

#endif	/* _PACSDEF_H */

