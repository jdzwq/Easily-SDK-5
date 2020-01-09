/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom defination document

	@module	dcmdef.h | dicom definition interface file

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


#ifndef _DCMDEF_H
#define	_DCMDEF_H

#include <xdl.h>

#if defined(_OS_WINDOWS) && defined(_USRDLL)
#define DCM_API __declspec(dllexport)
#else
#define DCM_API extern
#endif

typedef struct _dicm_vr_t{
	tchar_t vr[3];
	int vl;
}dicm_vr_t;

typedef struct _dicm_item_t{
	dword_t tag;
	tchar_t vr[3];
	tchar_t vn[100];
}dicm_item_t;

typedef struct _dicm_gruop_t{
	sword_t tag;
	tchar_t gn[100];
}dicm_group_t;

/*dicom raw def*/
typedef bool_t(__callback *PF_DICM_READ)(void* rp, byte_t* buf, dword_t* pb);
typedef bool_t(__callback *PF_DICM_WRITE)(void* wp, byte_t* buf, dword_t* pb);
typedef void(__callback *PF_DICM_ERROR)(void* ep, const tchar_t* errtext);

typedef int(__callback *PF_DICM_PARSE_ITEM)(void* pp, dword_t tag, const byte_t* vr, dword_t vl);
typedef int(__callback *PF_DICM_PARSE_DATA)(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl);
typedef int(__callback *PF_DICM_PARSE_SEQUENCE_BEGIN)(void* pp, dword_t tag, dword_t vl);
typedef int(__callback *PF_DICM_PARSE_SEQUENCE_END)(void* pp, dword_t tag);
typedef int(__callback *PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN)(void* pp, dword_t tag, dword_t vl);
typedef int(__callback *PF_DICM_PARSE_SEQUENCE_ITEM_END)(void* pp, dword_t tag);

typedef int(__callback *PF_DICM_FORMAT_ITEM)(void* fp, dword_t* ptag, byte_t* pvr, dword_t* pvl);
typedef int(__callback *PF_DICM_FORMAT_DATA)(void* fp, dword_t* ptag, byte_t* pvr, byte_t* pvf, dword_t* pvl);
typedef int(__callback *PF_DICM_FORMAT_SEQUENCE_BEGIN)(void* fp, dword_t* ptag);
typedef int(__callback *PF_DICM_FORMAT_SEQUENCE_END)(void* fp, dword_t* ptag);
typedef int(__callback *PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN)(void* fp, dword_t* ptag);
typedef int(__callback *PF_DICM_FORMAT_SEQUENCE_ITEM_END)(void* fp, dword_t* ptag);

typedef struct _dicm_context_t{
	void*	rp;
	PF_DICM_READ		pf_dicm_read;

	void* wp;
	PF_DICM_WRITE		pf_dicm_write;

	void* ep;
	PF_DICM_ERROR		pf_dicm_error;

	void* pp;
	PF_DICM_PARSE_ITEM		pf_dicm_parse_item;
	PF_DICM_PARSE_DATA		pf_dicm_parse_data;
	PF_DICM_PARSE_SEQUENCE_BEGIN	pf_dicm_parse_sequence_begin;
	PF_DICM_PARSE_SEQUENCE_END		pf_dicm_parse_sequence_end;
	PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN	pf_dicm_parse_sequence_item_begin;
	PF_DICM_PARSE_SEQUENCE_ITEM_END		pf_dicm_parse_sequence_item_end;

	void* fp;
	PF_DICM_FORMAT_ITEM		pf_dicm_format_item;
	PF_DICM_FORMAT_DATA		pf_dicm_format_data;
	PF_DICM_FORMAT_SEQUENCE_BEGIN	pf_dicm_format_sequence_begin;
	PF_DICM_FORMAT_SEQUENCE_END		pf_dicm_format_sequence_end;
	PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN	pf_dicm_format_sequence_item_begin;
	PF_DICM_FORMAT_SEQUENCE_ITEM_END	pf_dicm_format_sequence_item_end;

	bool_t b_Implicit;
	bool_t b_BigEndian;
	int n_CharSet;
}dicm_context_t;

#define DICM_CONTINUE	0
#define DICM_BREAK		1
#define DICM_ERROR		-1

#define DICM_CHARSET_ENG	0
#define DICM_CHARSET_UTF	1
#define DICM_CHARSET_GBK	2

/*dicom bmp def*/

typedef struct _dicm_image_t{
	tchar_t dcm_mod[16]; /*dicom modality*/

	float win_width;	/*windows width*/
	float win_center;	/*windows center*/
	float win_inter;	/*rescale intercept*/
	float win_slope;	/*rescale slope*/

	float min_width;	/*windows min width*/
	float max_width;	/*windows max width*/

	float xmm_pixel;	/*x pixel spaceing*/
	float ymm_pixel;	/*x pixel spaceing*/

	sword_t bmp_clr;	/*image colors*/
	dword_t bmp_cols;	/*image cols*/
	dword_t bmp_rows;	/*image rows*/

	sword_t bit_sig;	/*byte is sign*/
	sword_t bit_all;	/*byte bit alloced*/
	sword_t bit_len;	/*byte bit storage*/
	sword_t bit_pos;	/*byte bit position*/

	dword_t bytes;
	byte_t* pixels;
}dicm_image_t;

#define IS_SENS_VR(ba)		((ba[0] == 'S' && ba[1] == 'H') || (ba[0] == 'L' && ba[1] == 'O') || (ba[0] == 'U' && ba[1] == 'C') || (ba[0] == 'S' && ba[1] == 'T') || (ba[0] == 'L' && ba[1] == 'T') || (ba[0] == 'U' && ba[1] == 'T') || (ba[0] == 'P' && ba[1] == 'N'))
#define IS_NUM_VR(ba)		((ba[0] == 'F' && ba[1] == 'L') || (ba[0] == 'F' && ba[1] == 'D') || (ba[0] == 'S' && ba[1] == 'S') || (ba[0] == 'S' && ba[1] == 'L') || (ba[0] == 'U' && ba[1] == 'S') || (ba[0] == 'U' && ba[1] == 'L'))
#define IS_META_TAG(n)		(((n & 0xFFFF0000) == 0x00020000)? 1 : 0)

#define DICM_MAX_POINTS		16

typedef struct _dicm_graphic_anno_t{
	tchar_t type[RES_LEN];
	bool_t fill;
	int count;
	xpoint_t points[DICM_MAX_POINTS];
}dicm_graphic_anno_t;

typedef struct _dicm_text_anno_t{
	tchar_t text[RES_LEN];
	tchar_t align[INT_LEN];
	xpoint_t points[3];
}dicm_text_anno_t;

typedef struct _dicm_summary_t{
	tchar_t PatientID[RES_LEN];
	tchar_t PatientName[RES_LEN];
	tchar_t PatientSex[INT_LEN];
	tchar_t PatientAge[INT_LEN];

	tchar_t StudyInstanceUID[KEY_LEN];
	tchar_t StudyID[KEY_LEN];
	tchar_t StudyDate[DATE_LEN];

	tchar_t SeriesInstanceUID[KEY_LEN];
	tchar_t SeriesDate[DATE_LEN];
	tchar_t SeriesNumber[NUM_LEN];

	tchar_t AcquisitionDate[DATE_LEN];

	tchar_t Modality[NAME_LEN];
	tchar_t Manufacturer[NAME_LEN];
	tchar_t InstitutionName[NAME_LEN];
	tchar_t StationName[NAME_LEN];
	tchar_t ManufacturersModelName[NAME_LEN];

	tchar_t SliceThickness[NUM_LEN];
	tchar_t KVP[NUM_LEN];
	tchar_t GantryDetectorTilt[NUM_LEN];
	tchar_t ExposureTime[NUM_LEN];
	tchar_t XRayTubeCurrent[NUM_LEN];

	tchar_t WindowCenter[NUM_LEN];
	tchar_t WindowWidth[NUM_LEN];
}dicm_summary_t;

#endif	/* _DCMDEF_H */

