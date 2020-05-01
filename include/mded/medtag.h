/***********************************************************************
	Easily MDED 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc meded document

	@module	medtag.h | dicom interface file

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


#ifndef _MEDTAG_H
#define	_MEDTAG_H

#include "meddef.h"

typedef struct _med_vr_t{
	tchar_t vr[4];
	int min;
	int max;
}med_vr_t;

typedef struct _med_item_t{
	dword_t tag;
	tchar_t vr[4];
	tchar_t vn[64];
}med_item_t;

typedef struct _med_gruop_t{
	sword_t tag;
	tchar_t gn[64];
}med_group_t;

#define MED_IS_NUM_VR(ba)		((ba[0] == 'F' && ba[1] == 'L') || (ba[0] == 'F' && ba[1] == 'D') || (ba[0] == 'S' && ba[1] == 'S') || (ba[0] == 'S' && ba[1] == 'L') || (ba[0] == 'U' && ba[1] == 'S') || (ba[0] == 'U' && ba[1] == 'L'))


#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API const med_item_t* med_item_info(dword_t tag);

	EXP_API const med_group_t* med_group_info(dword_t tag);

	EXP_API bool_t med_vr_size(const tchar_t* pvr, int* pmin, int* pmax);

	EXP_API bool_t med_is_sequence(dword_t tag);

	EXP_API bool_t med_is_group(dword_t tag);

	EXP_API void med_bytes_turn(byte_t vr[2], byte_t* vf, dword_t vl);


#ifdef	__cplusplus
}
#endif


#endif	/* _MEDTAG_H */


