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

#define DICM_HEAD_SIZE		128

#define DICM_OPERA_FILEHEAD	0x00000001
#define DICM_OPERA_COMMAND	0x00000002
#define DICM_OPERA_DATASET	0x00000004

/*dicom raw def*/
typedef bool_t(*PF_DICM_READ)(void* rp, byte_t* buf, dword_t* pb);
typedef bool_t(*PF_DICM_WRITE)(void* wp, byte_t* buf, dword_t* pb);
typedef void(*PF_DICM_ERROR)(void* ep, const tchar_t* errtext);

typedef bool_t(*PF_DICM_PARSE_FILEHEAD)(void* pp, const byte_t* head, const byte_t* flag);
typedef bool_t(*PF_DICM_FORMAT_FILEHEAD)(void* fp, byte_t* head, byte_t* flag);

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

typedef struct _dicm_context_file{
	int op;

	void*	rp;
	PF_DICM_READ		pf_dicm_read;

	void* wp;
	PF_DICM_WRITE		pf_dicm_write;

	void* ep;
	PF_DICM_ERROR		pf_dicm_error;

	void* pp;
	PF_DICM_PARSE_FILEHEAD	pf_dicm_parse_head;

	void* fp;
	PF_DICM_FORMAT_FILEHEAD	pf_dicm_format_head;
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

	EXP_API dicm_context_file* dicm_alloc_filehead(void);

	EXP_API void dicm_free_filehead(dicm_context_file* ctx);

	EXP_API void dicm_set_filehead_read(dicm_context_file* pdicm, void* rp, PF_DICM_READ pf_read);

	EXP_API void dicm_set_filehead_write(dicm_context_file* pdicm, void* wp, PF_DICM_WRITE pf_write);

	EXP_API void dicm_set_filehead_error(dicm_context_file* pdicm, void* ep, PF_DICM_ERROR pf_error);

	EXP_API void dicm_set_filehead_parse(dicm_context_file* pdicm, void* pp, PF_DICM_PARSE_FILEHEAD pf_parse_head);

	EXP_API void dicm_set_filehead_format(dicm_context_file* pdicm, void* fp, PF_DICM_FORMAT_FILEHEAD pf_format_head);

	EXP_API bool_t dicm_parse_filehead(dicm_context_file* pdicm);

	EXP_API bool_t dicm_format_filehead(dicm_context_file* pdicm);

	/*********************************************************************************************************/

	EXP_API dicm_conext_command* dicm_alloc_command(void);

	EXP_API void dicm_free_command(dicm_conext_command* ctx);

	EXP_API void dicm_set_command_read(dicm_conext_command* pdicm, void* rp, PF_DICM_READ pf_read);

	EXP_API void dicm_set_command_write(dicm_conext_command* pdicm, void* wp, PF_DICM_WRITE pf_write);

	EXP_API void dicm_set_command_error(dicm_conext_command* pdicm, void* ep, PF_DICM_ERROR pf_error);

	EXP_API void dicm_set_command_parse(dicm_conext_command* pdicm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data);

	EXP_API void dicm_set_command_format(dicm_conext_command* pdicm, void* fp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data);

	EXP_API bool_t dicm_parse_command(dicm_conext_command* pdicm);

	EXP_API bool_t dicm_format_command(dicm_conext_command* pdicm);

	/*************************************************************************************************************/

	EXP_API dicm_context_dataset* dicm_alloc_dataset(void);

	EXP_API void dicm_free_dataset(dicm_context_dataset* ctx);

	EXP_API void dicm_set_dataset_read(dicm_context_dataset* pdicm, void* rp, PF_DICM_READ pf_read);

	EXP_API void dicm_set_dataset_write(dicm_context_dataset* pdicm, void* wp, PF_DICM_WRITE pf_write);

	EXP_API void dicm_set_dataset_error(dicm_context_dataset* pdicm, void* ep, PF_DICM_ERROR pf_error);

	EXP_API void dicm_set_dataset_parse(dicm_context_dataset* pdicm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data, PF_DICM_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_DICM_PARSE_SEQUENCE_END pf_parse_sequence_end, PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN pf_parse_sequence_item_begin, PF_DICM_PARSE_SEQUENCE_ITEM_END pf_parse_sequence_item_end);

	EXP_API void dicm_set_dataset_format(dicm_context_dataset* pdicm, void* pp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data, PF_DICM_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_DICM_FORMAT_SEQUENCE_END pf_format_sequence_end, PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN pf_format_sequence_item_begin, PF_DICM_FORMAT_SEQUENCE_ITEM_END pf_format_sequence_item_end);

	EXP_API bool_t dicm_parse_dataset(dicm_context_dataset* pdicm);

	EXP_API bool_t dicm_format_dataset(dicm_context_dataset* pdicm);

#ifdef	__cplusplus
}
#endif


#endif	/* _DICMCTX_H */


