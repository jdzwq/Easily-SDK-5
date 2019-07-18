/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmctx.h | dicom interface file

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


#ifndef _DICMCTX_H
#define	_DICMCTX_H

#include "dicmdef.h"

/*dicom raw def*/
typedef bool_t(*PF_DICM_READ)(void* rp, byte_t* buf, dword_t* pb);
typedef bool_t(*PF_DICM_WRITE)(void* wp, byte_t* buf, dword_t* pb);
typedef void(*PF_DICM_ERROR)(void* ep, const tchar_t* errtext);

typedef bool_t(*PF_DICM_PARSE_HEAD)(void* pp, const byte_t head[128], const byte_t flag[4]);
typedef bool_t(*PF_DICM_FORMAT_HEAD)(void* fp, byte_t head[128], byte_t flag[4]);

typedef bool_t(*PF_DICM_PARSE_ITEM)(void* pp, dword_t tag, const byte_t* vr, dword_t vl);
typedef bool_t(*PF_DICM_PARSE_DATA)(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl);
typedef bool_t(*PF_DICM_PARSE_SEQUENCE_BEGIN)(void* pp, dword_t tag, dword_t vl);
typedef bool_t(*PF_DICM_PARSE_SEQUENCE_END)(void* pp, dword_t tag);
typedef bool_t(*PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN)(void* pp, dword_t tag, dword_t vl);
typedef bool_t(*PF_DICM_PARSE_SEQUENCE_ITEM_END)(void* pp, dword_t tag);

typedef bool_t(*PF_DICM_FORMAT_ITEM)(void* fp, dword_t* ptag, byte_t* pvr, dword_t* pvl);
typedef bool_t(*PF_DICM_FORMAT_DATA)(void* fp, dword_t* ptag, byte_t* pvr, byte_t* pvf, dword_t* pvl);
typedef bool_t(*PF_DICM_FORMAT_SEQUENCE_BEGIN)(void* fp, dword_t* ptag);
typedef bool_t(*PF_DICM_FORMAT_SEQUENCE_END)(void* fp, dword_t* ptag);
typedef bool_t(*PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN)(void* fp, dword_t* ptag);
typedef bool_t(*PF_DICM_FORMAT_SEQUENCE_ITEM_END)(void* fp, dword_t* ptag);

#define DICM_OPERA_HEAD		0x00000001
#define DICM_OPERA_COMMAND	0x00000002
#define DICM_OPERA_DATASET	0x00000004

typedef struct _dicm_context_file{
	int op;

	void*	rp;
	PF_DICM_READ		pf_dicm_read;

	void* wp;
	PF_DICM_WRITE		pf_dicm_write;

	void* ep;
	PF_DICM_ERROR		pf_dicm_error;

	void* pp;
	PF_DICM_PARSE_HEAD	pf_dicm_parse_head;

	void* fp;
	PF_DICM_FORMAT_HEAD	pf_dicm_format_head;
}dicm_context_file;

typedef struct _dicm_conext_command{
	int op;

	void*	rp;
	PF_DICM_READ		pf_dicm_read;

	void* wp;
	PF_DICM_WRITE		pf_dicm_write;

	void* ep;
	PF_DICM_ERROR		pf_dicm_error;

	void* pp;
	PF_DICM_PARSE_ITEM		pf_dicm_parse_item;
	PF_DICM_PARSE_DATA		pf_dicm_parse_data;

	void* fp;
	PF_DICM_FORMAT_ITEM		pf_dicm_format_item;
	PF_DICM_FORMAT_DATA		pf_dicm_format_data;

	bool_t b_BigEndian;
	dword_t n_maxLength;
}dicm_conext_command;

typedef struct _dicm_context_dataset{
	int op;

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
	dword_t n_maxLength;
}dicm_context_dataset;

#define DICM_CMD_TAG_SIZE		4
#define DICM_CMD_LEN_SIZE		4

#ifdef	__cplusplus
extern "C" {
#endif

	DCM_API dicm_context_file* dicm_alloc_head(void);

	DCM_API void dicm_free_head(dicm_context_file* ctx);

	DCM_API void dicm_set_head_read(dicm_context_file* pdcm, void* rp, PF_DICM_READ pf_read);

	DCM_API void dicm_set_head_write(dicm_context_file* pdcm, void* wp, PF_DICM_WRITE pf_write);

	DCM_API void dicm_set_head_error(dicm_context_file* pdcm, void* ep, PF_DICM_ERROR pf_error);

	DCM_API void dicm_set_head_parse(dicm_context_file* pdcm, void* pp, PF_DICM_PARSE_HEAD pf_parse_head);

	DCM_API void dicm_set_head_format(dicm_context_file* pdcm, void* fp, PF_DICM_FORMAT_HEAD pf_format_head);

	DCM_API bool_t dicm_parse_head(dicm_context_file* pdcm);

	DCM_API bool_t dicm_format_head(dicm_context_file* pdcm);

	/*********************************************************************************************************/

	DCM_API dicm_conext_command* dicm_alloc_command(void);

	DCM_API void dicm_free_command(dicm_conext_command* ctx);

	DCM_API void dicm_set_command_read(dicm_conext_command* pdcm, void* rp, PF_DICM_READ pf_read);

	DCM_API void dicm_set_command_write(dicm_conext_command* pdcm, void* wp, PF_DICM_WRITE pf_write);

	DCM_API void dicm_set_command_error(dicm_conext_command* pdcm, void* ep, PF_DICM_ERROR pf_error);

	DCM_API void dicm_set_command_parse(dicm_conext_command* pdcm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data);

	DCM_API void dicm_set_command_format(dicm_conext_command* pdcm, void* fp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data);

	DCM_API bool_t dicm_parse_command(dicm_conext_command* pdcm);

	DCM_API bool_t dicm_format_command(dicm_conext_command* pdcm);

	/*************************************************************************************************************/

	DCM_API dicm_context_dataset* dicm_alloc_dataset(void);

	DCM_API void dicm_free_dataset(dicm_context_dataset* ctx);

	DCM_API void dicm_set_dataset_read(dicm_context_dataset* pdcm, void* rp, PF_DICM_READ pf_read);

	DCM_API void dicm_set_dataset_write(dicm_context_dataset* pdcm, void* wp, PF_DICM_WRITE pf_write);

	DCM_API void dicm_set_dataset_error(dicm_context_dataset* pdcm, void* ep, PF_DICM_ERROR pf_error);

	DCM_API void dicm_set_dataset_parse(dicm_context_dataset* pdcm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data, PF_DICM_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_DICM_PARSE_SEQUENCE_END pf_parse_sequence_end, PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN pf_parse_sequence_item_begin, PF_DICM_PARSE_SEQUENCE_ITEM_END pf_parse_sequence_item_end);

	DCM_API void dicm_set_dataset_format(dicm_context_dataset* pdcm, void* pp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data, PF_DICM_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_DICM_FORMAT_SEQUENCE_END pf_format_sequence_end, PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN pf_format_sequence_item_begin, PF_DICM_FORMAT_SEQUENCE_ITEM_END pf_format_sequence_item_end);

	DCM_API bool_t dicm_parse_dataset(dicm_context_dataset* pdcm);

	DCM_API bool_t dicm_format_dataset(dicm_context_dataset* pdcm);

#ifdef	__cplusplus
}
#endif


#endif	/* _DICMCTX_H */


