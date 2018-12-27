/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmsum.c | dicom document implement file

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

#include "dicmsum.h"
#include "dicmctx.h"
#include "dicmtag.h"

typedef struct _SUM_PARSE_PARAM{
	union{
		dicm_context_file* ph;
		dicm_conext_command* pc;
		dicm_context_dataset* pd;
	};
	dicm_summary_t* psum;
}SUM_PARSE_PARAM;

static bool_t _on_read(void* rp, byte_t* buf, dword_t* pb)
{
	stream_t stm = (stream_t)rp;

	return stream_read_bytes(stm, buf, pb);
}

static void _on_error(void* ep, const tchar_t* errtext)
{

}

static bool_t _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
{
	SUM_PARSE_PARAM* pa = (SUM_PARSE_PARAM*)pp;
	tchar_t* tval = NULL;
	dword_t n;

	float fl;
	double fd;
	short ss;
	long sl;
	unsigned short us;
	unsigned long ul;

	if (vr[0] == 'F' && vr[1] == 'L') //4
	{
		memcpy((void*)&fl, (void*)vf, sizeof(float));
		tval = xsalloc(NUM_LEN);
		n = ftoxs(fl, tval, NUM_LEN);
	}
	else if (vr[0] == 'F' && vr[1] == 'D') //8
	{
		memcpy((void*)&fd, (void*)vf, sizeof(double));
		tval = xsalloc(NUM_LEN);
		n = numtoxs(fd, tval, NUM_LEN);
	}
	else if (vr[0] == 'S' && vr[1] == 'S') //2
	{
		memcpy((void*)&ss, (void*)vf, sizeof(short));
		tval = xsalloc(NUM_LEN);
		n = stoxs(ss, tval, NUM_LEN);
	}
	else if (vr[0] == 'S' && vr[1] == 'L') //4
	{
		memcpy((void*)&sl, (void*)vf, sizeof(long));
		tval = xsalloc(NUM_LEN);
		n = ltoxs(sl, tval, NUM_LEN);
	}
	else if (vr[0] == 'U' && vr[1] == 'S') //2
	{
		memcpy((void*)&us, (void*)vf, sizeof(unsigned short));
		tval = xsalloc(NUM_LEN);
		n = ustoxs(us, tval, NUM_LEN);
	}
	else if (vr[0] == 'U' && vr[1] == 'L') //4
	{
		memcpy((void*)&ul, (void*)vf, sizeof(unsigned long));
		tval = xsalloc(NUM_LEN);
		n = ultoxs(ul, tval, NUM_LEN);
	}
	else if ((vr[0] == '\0' && vr[1] == '\0') || (vr[0] == 'O' && vr[1] == 'W') || (vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'S' && vr[1] == 'Q'))
	{
		tval = NULL;
	}
	else
	{
		if (DICM_IS_SENS_VR(vr) && pa->pd->n_CharSet == DICM_CHARSET_UTF)
		{
			n = utf8_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			utf8_to_ucs(vf, vl, tval, n);
		}
		else
		{
			n = mbs_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			mbs_to_ucs(vf, vl, tval, n);
		}
	}

	switch (tag)
	{
	case 0x00080020:
		xsncpy(pa->psum->StudyDate, tval, DATE_LEN);
		break;
	case 0x00080021:
		xsncpy(pa->psum->SeriesDate, tval, DATE_LEN);
		break;
	case 0x00080060:
		xsncpy(pa->psum->Modality, tval, KEY_LEN);
		break;
	case 0x00080070:
		xsncpy(pa->psum->Manufacturer, tval, KEY_LEN);
		break;
	case 0x00080080:
		xsncpy(pa->psum->InstitutionName, tval, KEY_LEN);
		break;
	case 0x00081090:
		xsncpy(pa->psum->ManufacturersModelName, tval, KEY_LEN);
		break;
	case 0x00081010:
		xsncpy(pa->psum->StationName, tval, KEY_LEN);
		break;

	case 0x00100010:
		xsncpy(pa->psum->PatientName, tval, RES_LEN);
		break;
	case 0x00100020:
		xsncpy(pa->psum->PatientID, tval, RES_LEN);
		break;
	case 0x00100040:
		xsncpy(pa->psum->PatientSex, tval, INT_LEN);
		break;
	case 0x00101010:
		xsncpy(pa->psum->PatientAge, tval, INT_LEN);
		break;

	case 0x00180050:
		xsncpy(pa->psum->SliceThickness, tval, NUM_LEN);
		break;
	case 0x00180060:
		xsncpy(pa->psum->KVP, tval, NUM_LEN);
		break;
	case 0x00181120:
		xsncpy(pa->psum->GantryDetectorTilt, tval, NUM_LEN);
		break;
	case 0x00181150:
		xsncpy(pa->psum->ExposureTime, tval, NUM_LEN);
		break;
	case 0x00181151:
		xsncpy(pa->psum->XRayTubeCurrent, tval, NUM_LEN);
		break;

	case 0x0020000D:
		xsncpy(pa->psum->StudyInstanceUID, tval, KEY_LEN);
		break;
	case 0x0020000E:
		xsncpy(pa->psum->SeriesInstanceUID, tval, KEY_LEN);
		break;
	case 0x00200010:
		xsncpy(pa->psum->StudyID, tval, KEY_LEN);
		break;
	case 0x00200011:
		xsncpy(pa->psum->SeriesNumber, tval, INT_LEN);
		break;

	case 0x00281050:
		xsncpy(pa->psum->WindowCenter, tval, NUM_LEN);
		break;
	case 0x00281051:
		xsncpy(pa->psum->WindowWidth, tval, NUM_LEN);
		break;

	}

	xmem_free(tval);

	return 1;
}

bool_t parse_dicm_summary_from_stream(dicm_summary_t* psum, dword_t op, stream_t stm)
{
	SUM_PARSE_PARAM pp = { 0 };
	bool_t b;

	if (op & DICM_OPERA_HEAD)
	{
		pp.ph = dicm_alloc_head();
		pp.psum = psum;

		dicm_set_head_read(pp.ph, (void*)stm, _on_read);
		dicm_set_head_parse(pp.ph, (void*)&pp, NULL);

		b = dicm_parse_head(pp.ph);
		dicm_free_head(pp.ph);
	}
	else if (op & DICM_OPERA_COMMAND)
	{
		pp.pc = dicm_alloc_command();
		pp.psum = psum;

		dicm_set_command_read(pp.pc, (void*)stm, _on_read);
		dicm_set_command_parse(pp.pc, (void*)&pp, NULL, NULL);

		b = dicm_parse_command(pp.pc);
		dicm_free_command(pp.pc);
	}
	else
	{
		b = 1;
	}

	if (!b)
		return 0;

	if (op & DICM_OPERA_DATASET)
	{
		pp.pd = dicm_alloc_dataset();
		pp.psum = psum;

		dicm_set_dataset_read(pp.pd, (void*)stm, _on_read);
		dicm_set_dataset_parse(pp.pd, (void*)&pp, NULL, _on_parse_data, NULL, NULL, NULL, NULL);

		b = dicm_parse_dataset(pp.pd);
		dicm_free_dataset(pp.pd);
	}
	else
	{
		b = 1;
	}

	return b;
}
