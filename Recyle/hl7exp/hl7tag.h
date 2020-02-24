/***********************************************************************
	Easily HL7 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hl7 document

	@module	hl7tag.h | hl7 interface file

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


#ifndef _HL7TAG_H
#define	_HL7TAG_H

#include "hl7def.h"

typedef struct _hl7_vr_t{
	tchar_t vr[4];
	int vl;
}hl7_vr_t;

typedef struct _hl7_item_t{
	tchar_t tag[8];
	tchar_t vr[4];
	tchar_t vn[64];
}hl7_item_t;


#define HL7_VR_SIZE		3
#define HL7_IID_SIZE	6

#ifdef	__cplusplus
extern "C" {
#endif

	HL7_API bool_t hl7_is_sequence(tchar_t vr[HL7_VR_SIZE]);

	HL7_API const hl7_item_t* hl7_item_info(tchar_t tag[HL7_IID_SIZE]);

	HL7_API dword_t hl7_vr_size(const tchar_t* pvr);

#ifdef	__cplusplus
}
#endif


#endif	/* _HL7CTX_H */


