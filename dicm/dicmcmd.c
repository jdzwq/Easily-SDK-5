/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmcmd.c | dicom document implement file

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

#include "dicmcmd.h"
#include "dicmctx.h"

typedef struct _CMD_PARSE_PARAM{
	dicm_conext_command* pdcm;
	dicm_command_t* pcmd;
}CMD_PARSE_PARAM;

static bool_t _on_read(void* rp, byte_t* buf, dword_t* pb)
{
	stream_t stm = (stream_t)rp;

	return stream_read_bytes(stm, buf, pb);
}

static bool_t _on_write(void* fp, byte_t* buf, dword_t* pb)
{
	stream_t stm = (stream_t)fp;

	return stream_write_bytes(stm, buf, *pb);
}

static void _on_error(void* ep, const tchar_t* errtext)
{

}

static bool_t _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
{
	CMD_PARSE_PARAM* pa = (CMD_PARSE_PARAM*)pp;

	pa->pcmd->mask = 0;

	switch (tag)
	{
	case 0x00000000:
		pa->pcmd->mask |= MSK_GroupLength;
		xmem_copy((void*)&(pa->pcmd->GroupLength), (void*)vf, vl);
		break;
	case 0x00000002:
		pa->pcmd->mask |= MSK_AffectedSOPClassUID;
		xmem_copy((void*)(pa->pcmd->AffectedSOPClassUID), (void*)vf, vl);
		break;
	case 0x00000100:
		pa->pcmd->mask |= MSK_CommandField;
		xmem_copy((void*)&(pa->pcmd->CommandField), (void*)vf, vl);
		break;
	case 0x00000110:
		pa->pcmd->mask |= MSK_MessageID;
		xmem_copy((void*)&(pa->pcmd->MessageID), (void*)vf, vl);
		break;
	case 0x00000120:
		pa->pcmd->mask |= MSK_MessageIDBeingRespondedTo;
		xmem_copy((void*)&(pa->pcmd->MessageIDBeingRespondedTo), (void*)vf, vl);
		break;
	case 0x00000600:
		pa->pcmd->mask |= MSK_MoveDestination;
		xmem_copy((void*)(pa->pcmd->MoveDestination), (void*)vf, vl);
		break;
	case 0x00000700:
		pa->pcmd->mask |= MSK_Priority;
		xmem_copy((void*)&(pa->pcmd->Priority), (void*)vf, vl);
		break;
	case 0x00000800:
		pa->pcmd->mask |= MSK_DataSetType;
		xmem_copy((void*)&(pa->pcmd->DataSetType), (void*)vf, vl);
		break;
	case 0x00000900:
		pa->pcmd->mask |= MSK_Status;
		xmem_copy((void*)&(pa->pcmd->Status), (void*)vf, vl);
		break;
	case 0x00001020:
		pa->pcmd->mask |= MSK_NumberofRemainingSubOperations;
		xmem_copy((void*)&(pa->pcmd->NumberofRemainingSubOperations), (void*)vf, vl);
		break;
	case 0x00001021:
		pa->pcmd->mask |= MSK_NumberofCompletedSubOperations;
		xmem_copy((void*)&(pa->pcmd->NumberofCompletedSubOperations), (void*)vf, vl);
		break;
	case 0x00001022:
		pa->pcmd->mask |= MSK_NumberofFailedSubOperations;
		xmem_copy((void*)&(pa->pcmd->NumberofFailedSubOperations), (void*)vf, vl);
		break;
	case 0x00001023:
		pa->pcmd->mask |= MSK_NumberofWarningSubOperations;
		xmem_copy((void*)&(pa->pcmd->NumberofWarningSubOperations), (void*)vf, vl);
		break;
	}

	return 1;
}

bool_t parse_dicm_command_from_stream(dicm_command_t* pcmd, bool_t bigEndian, dword_t maxLength, stream_t stm)
{
	CMD_PARSE_PARAM pp = { 0 };
	bool_t b;

	pp.pdcm = dicm_alloc_command();
	pp.pdcm->b_BigEndian = bigEndian;
	pp.pdcm->n_maxLength = maxLength;
	pp.pcmd = pcmd;

	dicm_set_command_read(pp.pdcm, (void*)stm, _on_read);
	dicm_set_command_parse(pp.pdcm, (void*)&pp, NULL, _on_parse_data);

	b = dicm_parse_command(pp.pdcm);
	dicm_free_command(pp.pdcm);

	return b;
}

typedef struct _CMD_FORMAT_PARAM{
	dicm_conext_command* pdcm;
	dicm_command_t* pcmd;
	dword_t mask;
}CMD_FORMAT_PARAM;

static bool_t _on_format_item(void* fp, dword_t* ptag, byte_t* pvr, dword_t* pvl)
{
	CMD_FORMAT_PARAM* pa = (CMD_FORMAT_PARAM*)fp;

	if (pa->mask & MSK_GroupLength)
	{
		*ptag = 0x00000000;
		pvr[0] = 'U';
		pvr[1] = 'L';
		*pvl = 4;

		pa->mask &= ~MSK_GroupLength;
		return 1;
	}
	else if (pa->mask & MSK_AffectedSOPClassUID)
	{
		*ptag = 0x00000002;
		pvr[0] = 'U';
		pvr[1] = 'I';
		*pvl = a_xslen((schar_t*)pa->pcmd->AffectedSOPClassUID);

		pa->mask &= ~MSK_AffectedSOPClassUID;
		return 1;
	}
	else if (pa->mask & MSK_CommandField)
	{
		*ptag = 0x00000100;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_CommandField;
		return 1;
	}
	else if (pa->mask & MSK_MessageID)
	{
		*ptag = 0x00000110;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_MessageID;
		return 1;
	}
	else if (pa->mask & MSK_MessageIDBeingRespondedTo)
	{
		*ptag = 0x00000120;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_MessageIDBeingRespondedTo;
		return 1;
	}
	else if (pa->mask & MSK_MoveDestination)
	{
		*ptag = 0x00000600;
		pvr[0] = 'A';
		pvr[1] = '2';
		*pvl = a_xslen((schar_t*)pa->pcmd->MoveDestination);

		pa->mask &= ~MSK_MoveDestination;
		return 1;
	}
	else if (pa->mask & MSK_Priority)
	{
		*ptag = 0x00000700;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_Priority;
		return 1;
	}
	else if (pa->mask & MSK_DataSetType)
	{
		*ptag = 0x00000800;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_DataSetType;
		return 1;
	}
	else if (pa->mask & MSK_Status)
	{
		*ptag = 0x00000900;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_Status;
		return 1;
	}
	else if (pa->mask & MSK_NumberofRemainingSubOperations)
	{
		*ptag = 0x00001020;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_NumberofRemainingSubOperations;
		return 1;
	}
	else if (pa->mask & MSK_NumberofCompletedSubOperations)
	{
		*ptag = 0x00001021;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_NumberofFailedSubOperations;
		return 1;
	}
	else if (pa->mask & MSK_NumberofCompletedSubOperations)
	{
		*ptag = 0x00001022;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_NumberofFailedSubOperations;
		return 1;
	}
	else if (pa->mask & MSK_NumberofWarningSubOperations)
	{
		*ptag = 0x00001023;
		pvr[0] = 'U';
		pvr[1] = 'S';
		*pvl = 2;

		pa->mask &= ~MSK_NumberofWarningSubOperations;
		return 1;
	}

	return 0;
}

static bool_t _on_format_data(void* fp, dword_t* ptag, byte_t* pvr, byte_t* pvf, dword_t* pvl)
{
	CMD_FORMAT_PARAM* pa = (CMD_FORMAT_PARAM*)fp;

	switch (*ptag)
	{
	case 0x00000000:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->GroupLength), *pvl);
		}
		return 1;
	case 0x00000002:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)(pa->pcmd->AffectedSOPClassUID), *pvl);
		}
		return 1;
	case 0x00000100:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->CommandField), *pvl);
		}
		return 1;
	case 0x00000110:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->MessageID), *pvl);
		}
		return 1;
	case 0x00000120:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->MessageIDBeingRespondedTo), *pvl);
		}
		return 1;
	case 0x00000600:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)(pa->pcmd->MoveDestination), *pvl);
		}
		return 1;
	case 0x00000700:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->Priority), *pvl);
		}
		return 1;
	case 0x00000800:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->DataSetType), *pvl);
		}
		return 1;
	case 0x00000900:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->Status), *pvl);
		}
		return 1;
	case 0x00001020:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->NumberofRemainingSubOperations), *pvl);
		}
		return 1;
	case 0x00001021:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->NumberofCompletedSubOperations), *pvl);
		}
		return 1;
	case 0x00001022:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->NumberofFailedSubOperations), *pvl);
		}
		return 1;
	case 0x00001023:
		if (*pvl)
		{
			xmem_copy((void*)pvf, (void*)&(pa->pcmd->NumberofWarningSubOperations), *pvl);
		}
		return 1;
	}

	return 0;
}

bool_t format_dicm_command_to_stream(dicm_command_t* pcmd, bool_t bigEndian, dword_t maxLength, stream_t stm)
{
	CMD_FORMAT_PARAM fp = { 0 };
	bool_t b;

	fp.pdcm = dicm_alloc_command();
	fp.pdcm->b_BigEndian = bigEndian;
	fp.pdcm->n_maxLength = maxLength;
	fp.pcmd = pcmd;
	fp.mask = pcmd->mask;

	dicm_set_command_write(fp.pdcm, (void*)stm, _on_write);
	dicm_set_command_format(fp.pdcm, (void*)&fp, _on_format_item, _on_format_data);

	b = dicm_format_command(fp.pdcm);
	dicm_free_command(fp.pdcm);

	return b;
}