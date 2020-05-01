/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmcmd.h | dicom document interface file

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


#ifndef _DCMCMD_H
#define	_DCMCMD_H

#include "dcmdef.h"

#define MSK_GroupLength				0x00000001
#define MSK_AffectedSOPClassUID		0x00000002
#define MSK_CommandField			0x00000004
#define MSK_MessageID				0x00000008
#define MSK_MessageIDBeingRespondedTo	0x00000010
#define MSK_MoveDestination			0x00000020
#define MSK_Priority				0x00000040
#define MSK_DataSetType				0x00000080
#define MSK_Status					0x00000100
#define MSK_NumberofRemainingSubOperations	0x00000200
#define MSK_NumberofCompletedSubOperations	0x00000400
#define MSK_NumberofFailedSubOperations		0x00000800
#define MSK_NumberofWarningSubOperations	0x00001000

typedef struct _dcm_command_t{
	dword_t mask;

	dword_t GroupLength;
	byte_t AffectedSOPClassUID[NUM_LEN];
	sword_t CommandField;
	sword_t MessageID;
	sword_t MessageIDBeingRespondedTo;
	byte_t MoveDestination[RES_LEN];
	sword_t Priority;
	sword_t DataSetType;
	sword_t Status;
	sword_t NumberofRemainingSubOperations;
	sword_t NumberofCompletedSubOperations;
	sword_t NumberofFailedSubOperations;
	sword_t NumberofWarningSubOperations;
}dcm_command_t;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API bool_t parse_dicm_command_from_stream(dcm_command_t* pcmd, bool_t bigEndian, dword_t maxLength, stream_t stm);

	EXP_API bool_t format_dicm_command_to_stream(dcm_command_t* pcmd, bool_t bigEndian, dword_t maxLength, stream_t stm);

#ifdef	__cplusplus
}
#endif

#endif	/* _DCMCMD_H */

