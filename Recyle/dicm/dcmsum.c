/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom bmp document

	@module	dcmsum.c | dicom bmp implement file

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

#include "dcmsum.h"
#include "dcmraw.h"

typedef struct _SUM_PARSE_PARAM{
	dicm_context_t* pdcm;
	dicm_summary_t* psum;
}SUM_PARSE_PARAM;

static bool_t __callback _on_read(void* rp, byte_t* buf, dword_t* pb)
{
	XHANDLE xf = (XHANDLE)rp;

	return xfile_read(xf, buf, pb);
}

static void __callback _on_error(void* ep, const tchar_t* errtext)
{

}

static int __callback _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
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
		if (IS_SENS_VR(vr) && pa->pdcm->n_CharSet == DICM_CHARSET_UTF)
		{
			n = utf8_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			utf8_to_ucs(vf, vl, tval, n);
		}
		else
		{
			n = gb2312_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			gb2312_to_ucs(vf, vl, tval, n);
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
		xsncpy(pa->psum->Modality, tval, NAME_LEN);
		break;
	case 0x00080070:
		xsncpy(pa->psum->Manufacturer, tval, NAME_LEN);
		break;
	case 0x00080080:
		xsncpy(pa->psum->InstitutionName, tval, NAME_LEN);
		break;
	case 0x00081090:
		xsncpy(pa->psum->ManufacturersModelName, tval, NAME_LEN);
		break;
	case 0x00081010:
		xsncpy(pa->psum->StationName, tval, NAME_LEN);
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

	return 0;
}

dicm_summary_t* dicm_summary_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	XHANDLE xf = NULL;
	SUM_PARSE_PARAM pp = { 0 };
	dicm_context_t* pdcm = NULL;
	dicm_summary_t* psum = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, 0);
	if (!xf)
	{
		raise_user_error(_T("dicm_file_to_image"), _T("open dicm file failed"));
	}

	pdcm = dicm_alloc();
	psum = (dicm_summary_t*)xmem_alloc(sizeof(dicm_summary_t));
	
	pp.pdcm = pdcm;
	pp.psum = psum;

	dicm_set_read(pdcm, (void*)xf, _on_read);
	dicm_set_parse(pdcm, (void*)&pp, NULL, _on_parse_data, NULL, NULL, NULL, NULL);

	dicm_parse(pdcm);

	xfile_close(xf);
	xf = NULL;

	dicm_free(pdcm);
	pdcm = NULL;

	if (is_null(psum->PatientID))
	{
		raise_user_error(_T("dicm_file_to_summary"), _T("parse dicm file failed"));
	}

	END_CATCH;

	return psum;

ONERROR:

	if (xf)
		xfile_close(xf);

	if (pdcm)
		dicm_free(pdcm);

	if (psum)
		xmem_free(psum);

	return NULL;
}

dicm_summary_t* dicm_summary_from_dom(LINKPTR dom)
{
	LINKPTR plk;
	dicm_summary_t* pds;

	pds = (dicm_summary_t*)xmem_alloc(sizeof(dicm_summary_t));

	plk = get_dom_first_child_node(dom);
	while (plk)
	{
		if (compare_text(get_dom_node_name_ptr(plk), -1, _T("PatientID"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->PatientID, RES_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("PatientName"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->PatientName, RES_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("PatientSex"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->PatientSex, INT_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("PatientAge"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->PatientAge, INT_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("StudyInstanceUID"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->StudyInstanceUID, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("StudyID"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->StudyID, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("StudyDate"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->StudyDate, DATE_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("SeriesInstanceUID"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->SeriesInstanceUID, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("SeriesDate"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->SeriesDate, DATE_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("SeriesNumber"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->SeriesNumber, INT_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("Modality"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->Modality, NAME_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("Manufacturer"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->Manufacturer, NAME_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("InstitutionName"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->InstitutionName, NAME_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("StationName"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->StationName, NAME_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("ManufacturersModelName"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->ManufacturersModelName, NAME_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("SliceThickness"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->SliceThickness, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("KVP"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->KVP, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("GantryDetectorTilt"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->GantryDetectorTilt, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("XRayTubeCurrent"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->XRayTubeCurrent, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("ExposureTime"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->ExposureTime, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("WindowCenter"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->WindowCenter, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("WindowWidth"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->WindowWidth, NUM_LEN);
		}

		plk = get_dom_next_sibling_node(plk);
	}

	if (is_null(pds->PatientID))
	{
		xmem_free(pds);
		return NULL;
	}

	return pds;
}

void dicm_summary_free(dicm_summary_t* pds)
{
	xmem_free(pds);
}
