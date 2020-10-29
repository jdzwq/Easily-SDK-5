/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmsum.h | dicom document interface file

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


#ifndef _DICMSUM_H
#define	_DICMSUM_H

#include "dicmdef.h"

typedef struct _dicm_summary_t{
	tchar_t PatientID[RES_LEN + 1];
	tchar_t PatientName[RES_LEN + 1];
	tchar_t PatientSex[INT_LEN + 1];
	tchar_t PatientAge[INT_LEN + 1];

	tchar_t StudyInstanceUID[KEY_LEN + 1];
	tchar_t StudyID[KEY_LEN + 1];
	tchar_t StudyDate[DATE_LEN + 1];

	tchar_t SeriesInstanceUID[KEY_LEN + 1];
	tchar_t SeriesDate[DATE_LEN + 1];
	tchar_t SeriesNumber[NUM_LEN + 1];

	tchar_t AcquisitionDate[DATE_LEN + 1];

	tchar_t Modality[KEY_LEN + 1];
	tchar_t Manufacturer[KEY_LEN + 1];
	tchar_t InstitutionName[KEY_LEN + 1];
	tchar_t StationName[KEY_LEN + 1];
	tchar_t ManufacturersModelName[KEY_LEN + 1];

	tchar_t SliceThickness[NUM_LEN + 1];
	tchar_t KVP[NUM_LEN + 1];
	tchar_t GantryDetectorTilt[NUM_LEN + 1];
	tchar_t ExposureTime[NUM_LEN + 1];
	tchar_t XRayTubeCurrent[NUM_LEN + 1];

	tchar_t WindowCenter[NUM_LEN + 1];
	tchar_t WindowWidth[NUM_LEN + 1];
}dicm_summary_t;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API bool_t parse_dicm_summary_from_stream(dicm_summary_t* pds, dword_t op, stream_t stm);


#ifdef	__cplusplus
}
#endif

#endif	/* _DICMSUM_H */

