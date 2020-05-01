/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dcmctx.h | dicom interface file

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


#ifndef _DCMCTX_H
#define	_DCMCTX_H

#include "dcmdef.h"

#define DCM_HEAD_SIZE		128

#define DCM_OPERA_FILEHEAD	0x00000001
#define DCM_OPERA_COMMAND	0x00000002
#define DCM_OPERA_DATASET	0x00000004

/*dicom raw def*/
typedef bool_t(*PF_DCM_READ)(void* rp, byte_t* buf, dword_t* pb);
typedef bool_t(*PF_DCM_WRITE)(void* wp, byte_t* buf, dword_t* pb);
typedef void(*PF_DCM_ERROR)(void* ep, const tchar_t* errtext);

typedef bool_t(*PF_DCM_PARSE_FILEHEAD)(void* pp, const byte_t* head, const byte_t* flag);
typedef bool_t(*PF_DCM_FORMAT_FILEHEAD)(void* fp, byte_t* head, byte_t* flag);

typedef bool_t(*PF_DCM_PARSE_ITEM)(void* pp, dword_t tag, const byte_t* vr, dword_t vl);
typedef bool_t(*PF_DCM_PARSE_DATA)(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl);
typedef bool_t(*PF_DCM_PARSE_SEQUENCE_BEGIN)(void* pp, dword_t tag, dword_t vl);
typedef bool_t(*PF_DCM_PARSE_SEQUENCE_END)(void* pp, dword_t tag);
typedef bool_t(*PF_DCM_PARSE_SEQUENCE_ITEM_BEGIN)(void* pp, dword_t tag, dword_t vl);
typedef bool_t(*PF_DCM_PARSE_SEQUENCE_ITEM_END)(void* pp, dword_t tag);

typedef bool_t(*PF_DCM_FORMAT_ITEM)(void* fp, dword_t* ptag, byte_t* pvr, dword_t* pvl);
typedef bool_t(*PF_DCM_FORMAT_DATA)(void* fp, dword_t* ptag, byte_t* pvr, byte_t* pvf, dword_t* pvl);
typedef bool_t(*PF_DCM_FORMAT_SEQUENCE_BEGIN)(void* fp, dword_t* ptag);
typedef bool_t(*PF_DCM_FORMAT_SEQUENCE_END)(void* fp, dword_t* ptag);
typedef bool_t(*PF_DCM_FORMAT_SEQUENCE_ITEM_BEGIN)(void* fp, dword_t* ptag);
typedef bool_t(*PF_DCM_FORMAT_SEQUENCE_ITEM_END)(void* fp, dword_t* ptag);

typedef struct _dcm_context_file{
	int op;

	void*	rp;
	PF_DCM_READ		pf_dcm_read;

	void* wp;
	PF_DCM_WRITE		pf_dcm_write;

	void* ep;
	PF_DCM_ERROR		pf_dcm_error;

	void* pp;
	PF_DCM_PARSE_FILEHEAD	pf_dcm_parse_head;

	void* fp;
	PF_DCM_FORMAT_FILEHEAD	pf_dcm_format_head;
}dcm_context_file;

typedef struct _dcm_conext_command{
	int op;

	void*	rp;
	PF_DCM_READ		pf_dcm_read;

	void* wp;
	PF_DCM_WRITE		pf_dcm_write;

	void* ep;
	PF_DCM_ERROR		pf_dcm_error;

	void* pp;
	PF_DCM_PARSE_ITEM		pf_dcm_parse_item;
	PF_DCM_PARSE_DATA		pf_dcm_parse_data;

	void* fp;
	PF_DCM_FORMAT_ITEM		pf_dcm_format_item;
	PF_DCM_FORMAT_DATA		pf_dcm_format_data;

	bool_t b_BigEndian;
	dword_t n_maxLength;
}dcm_conext_command;

typedef struct _dcm_context_dataset{
	int op;

	void*	rp;
	PF_DCM_READ		pf_dcm_read;

	void* wp;
	PF_DCM_WRITE		pf_dcm_write;

	void* ep;
	PF_DCM_ERROR		pf_dcm_error;

	void* pp;
	PF_DCM_PARSE_ITEM		pf_dcm_parse_item;
	PF_DCM_PARSE_DATA		pf_dcm_parse_data;
	PF_DCM_PARSE_SEQUENCE_BEGIN	pf_dcm_parse_sequence_begin;
	PF_DCM_PARSE_SEQUENCE_END		pf_dcm_parse_sequence_end;
	PF_DCM_PARSE_SEQUENCE_ITEM_BEGIN	pf_dcm_parse_sequence_item_begin;
	PF_DCM_PARSE_SEQUENCE_ITEM_END		pf_dcm_parse_sequence_item_end;

	void* fp;
	PF_DCM_FORMAT_ITEM		pf_dcm_format_item;
	PF_DCM_FORMAT_DATA		pf_dcm_format_data;
	PF_DCM_FORMAT_SEQUENCE_BEGIN	pf_dcm_format_sequence_begin;
	PF_DCM_FORMAT_SEQUENCE_END		pf_dcm_format_sequence_end;
	PF_DCM_FORMAT_SEQUENCE_ITEM_BEGIN	pf_dcm_format_sequence_item_begin;
	PF_DCM_FORMAT_SEQUENCE_ITEM_END	pf_dcm_format_sequence_item_end;

	bool_t b_Implicit;
	bool_t b_BigEndian;
	int n_CharSet;
	dword_t n_maxLength;
}dcm_context_dataset;

#define DCM_CMD_TAG_SIZE		4
#define DCM_CMD_LEN_SIZE		4

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API dcm_context_file* dcm_alloc_filehead(void);

	EXP_API void dcm_free_filehead(dcm_context_file* ctx);

	EXP_API void dcm_set_filehead_read(dcm_context_file* pdcm, void* rp, PF_DCM_READ pf_read);

	EXP_API void dcm_set_filehead_write(dcm_context_file* pdcm, void* wp, PF_DCM_WRITE pf_write);

	EXP_API void dcm_set_filehead_error(dcm_context_file* pdcm, void* ep, PF_DCM_ERROR pf_error);

	EXP_API void dcm_set_filehead_parse(dcm_context_file* pdcm, void* pp, PF_DCM_PARSE_FILEHEAD pf_parse_head);

	EXP_API void dcm_set_filehead_format(dcm_context_file* pdcm, void* fp, PF_DCM_FORMAT_FILEHEAD pf_format_head);

	EXP_API bool_t dcm_parse_filehead(dcm_context_file* pdcm);

	EXP_API bool_t dcm_format_filehead(dcm_context_file* pdcm);

	/*********************************************************************************************************/

	EXP_API dcm_conext_command* dcm_alloc_command(void);

	EXP_API void dcm_free_command(dcm_conext_command* ctx);

	EXP_API void dcm_set_command_read(dcm_conext_command* pdcm, void* rp, PF_DCM_READ pf_read);

	EXP_API void dcm_set_command_write(dcm_conext_command* pdcm, void* wp, PF_DCM_WRITE pf_write);

	EXP_API void dcm_set_command_error(dcm_conext_command* pdcm, void* ep, PF_DCM_ERROR pf_error);

	EXP_API void dcm_set_command_parse(dcm_conext_command* pdcm, void* pp, PF_DCM_PARSE_ITEM pf_parse_item, PF_DCM_PARSE_DATA pf_parse_data);

	EXP_API void dcm_set_command_format(dcm_conext_command* pdcm, void* fp, PF_DCM_FORMAT_ITEM pf_format_item, PF_DCM_FORMAT_DATA pf_format_data);

	EXP_API bool_t dcm_parse_command(dcm_conext_command* pdcm);

	EXP_API bool_t dcm_format_command(dcm_conext_command* pdcm);

	/*************************************************************************************************************/

	EXP_API dcm_context_dataset* dcm_alloc_dataset(void);

	EXP_API void dcm_free_dataset(dcm_context_dataset* ctx);

	EXP_API void dcm_set_dataset_read(dcm_context_dataset* pdcm, void* rp, PF_DCM_READ pf_read);

	EXP_API void dcm_set_dataset_write(dcm_context_dataset* pdcm, void* wp, PF_DCM_WRITE pf_write);

	EXP_API void dcm_set_dataset_error(dcm_context_dataset* pdcm, void* ep, PF_DCM_ERROR pf_error);

	EXP_API void dcm_set_dataset_parse(dcm_context_dataset* pdcm, void* pp, PF_DCM_PARSE_ITEM pf_parse_item, PF_DCM_PARSE_DATA pf_parse_data, PF_DCM_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_DCM_PARSE_SEQUENCE_END pf_parse_sequence_end, PF_DCM_PARSE_SEQUENCE_ITEM_BEGIN pf_parse_sequence_item_begin, PF_DCM_PARSE_SEQUENCE_ITEM_END pf_parse_sequence_item_end);

	EXP_API void dcm_set_dataset_format(dcm_context_dataset* pdcm, void* pp, PF_DCM_FORMAT_ITEM pf_format_item, PF_DCM_FORMAT_DATA pf_format_data, PF_DCM_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_DCM_FORMAT_SEQUENCE_END pf_format_sequence_end, PF_DCM_FORMAT_SEQUENCE_ITEM_BEGIN pf_format_sequence_item_begin, PF_DCM_FORMAT_SEQUENCE_ITEM_END pf_format_sequence_item_end);

	EXP_API bool_t dcm_parse_dataset(dcm_context_dataset* pdcm);

	EXP_API bool_t dcm_format_dataset(dcm_context_dataset* pdcm);

#ifdef	__cplusplus
}
#endif


#endif	/* _DCMCTX_H */


