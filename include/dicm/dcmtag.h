/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dcmtag.h | dicom interface file

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


#ifndef _DCMTAG_H
#define	_DCMTAG_H

#include "dcmdef.h"

typedef struct _dcm_vr_t{
	tchar_t vr[4];
	sword_t vl;
}dcm_vr_t;

typedef struct _dcm_item_t{
	dword_t tag;
	tchar_t vr[4];
	tchar_t vn[64];
}dcm_item_t;

typedef struct _dcm_gruop_t{
	sword_t tag;
	tchar_t gn[64];
}dcm_group_t;

#define DCM_IS_SENS_VR(ba)		((ba[0] == 'S' && ba[1] == 'H') || (ba[0] == 'L' && ba[1] == 'O') || (ba[0] == 'U' && ba[1] == 'C') || (ba[0] == 'S' && ba[1] == 'T') || (ba[0] == 'L' && ba[1] == 'T') || (ba[0] == 'U' && ba[1] == 'T') || (ba[0] == 'P' && ba[1] == 'N'))
#define DCM_IS_NUM_VR(ba)		((ba[0] == 'F' && ba[1] == 'L') || (ba[0] == 'F' && ba[1] == 'D') || (ba[0] == 'S' && ba[1] == 'S') || (ba[0] == 'S' && ba[1] == 'L') || (ba[0] == 'U' && ba[1] == 'S') || (ba[0] == 'U' && ba[1] == 'L'))
#define DCM_IS_META_TAG(n)		(((n & 0xFFFF0000) == 0x00020000)? 1 : 0)

#define DCM_CAPTAGS_SIZE		37


#ifdef	__cplusplus
extern "C" {
#endif

	extern dword_t DCM_CAPTAGS[DCM_CAPTAGS_SIZE];

	EXP_API const dcm_item_t* dcm_item_info(dword_t tag);

	EXP_API const dcm_group_t* dcm_group_info(dword_t tag);

	EXP_API dword_t dcm_vr_size(const tchar_t* pvr);

	EXP_API bool_t dcm_is_sequence(dword_t tag);

	EXP_API bool_t dcm_is_group(dword_t tag);

	EXP_API void dcm_bytes_turn(byte_t vr[2], byte_t* vf, dword_t vl);

	EXP_API bool_t sop_is_implicit(const byte_t* syn);

	EXP_API bool_t sop_is_bigendian(const byte_t* syn);


#ifdef	__cplusplus
}
#endif


#endif	/* _DCMTAG_H */


