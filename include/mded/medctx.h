/***********************************************************************
	Easily MDED 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc meded document

	@module	medctx.h | meded interface file

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


#ifndef _MEDCTX_H
#define	_MEDCTX_H

#include "meddef.h"

#define MED_HEAD_SIZE		128
#define MED_OPERA_FILEHEAD	0x00000001
#define MED_OPERA_DATASET	0x00000004

typedef bool_t(*PF_MED_READ)(void* rp, byte_t* buf, dword_t* pb);
typedef bool_t(*PF_MED_WRITE)(void* wp, byte_t* buf, dword_t* pb);
typedef void(*PF_MED_ERROR)(void* pp, const tchar_t* errtext);

typedef bool_t(*PF_MED_DECODE_FILEHEAD)(void* dp, const byte_t head[MED_HEAD_SIZE]);
typedef bool_t(*PF_MED_ENCODE_FILEHEAD)(void* ep, byte_t head[MED_HEAD_SIZE]);

typedef bool_t(*PF_MED_DECODE_HEAD)(void* pp, const byte_t* flag, dword_t level);
typedef bool_t(*PF_MED_DECODE_ITEM)(void* dp, dword_t tag, const byte_t* vr, dword_t vl);
typedef bool_t(*PF_MED_DECODE_DATA)(void* dp, dword_t tag, const byte_t* vr, dword_t vl, const byte_t* vf);
typedef bool_t(*PF_MED_DECODE_SEQUENCE_BEGIN)(void* dp, dword_t tag, dword_t vl);
typedef bool_t(*PF_MED_DECODE_SEQUENCE_END)(void* dp, dword_t tag);
typedef bool_t(*PF_MED_DECODE_SEQUENCE_ITEM_BEGIN)(void* dp, dword_t tag, dword_t vl);
typedef bool_t(*PF_MED_DECODE_SEQUENCE_ITEM_END)(void* dp, dword_t tag);

typedef bool_t(*PF_MED_ENCODE_HEAD)(void* fp, byte_t* flag, dword_t* level);
typedef bool_t(*PF_MED_ENCODE_ITEM)(void* ep, dword_t* ptag, byte_t* pvr, dword_t* pvl);
typedef bool_t(*PF_MED_ENCODE_DATA)(void* ep, dword_t* ptag, byte_t* pvr, dword_t* pvl, byte_t* pvf);
typedef bool_t(*PF_MED_ENCODE_SEQUENCE_BEGIN)(void* ep, dword_t* ptag);
typedef bool_t(*PF_MED_ENCODE_SEQUENCE_END)(void* ep, dword_t* ptag);
typedef bool_t(*PF_MED_ENCODE_SEQUENCE_ITEM_BEGIN)(void* ep, dword_t* ptag);
typedef bool_t(*PF_MED_ENCODE_SEQUENCE_ITEM_END)(void* ep, dword_t* ptag);

typedef struct _med_context_file{
	int op;

	void* rp;
	PF_MED_READ		pf_med_read;

	void* wp;
	PF_MED_WRITE		pf_med_write;

	void* pp;
	PF_MED_ERROR		pf_med_error;

	void* dp;
	PF_MED_DECODE_FILEHEAD	pf_med_decode_head;

	void* ep;
	PF_MED_ENCODE_FILEHEAD	pf_med_encode_head;
}med_context_file;

typedef struct _med_context_dataset{
	int op;

	void* rp;
	PF_MED_READ		pf_med_read;

	void* wp;
	PF_MED_WRITE		pf_med_write;

	void* pp;
	PF_MED_ERROR		pf_med_error;

	void* dp;
	PF_MED_DECODE_HEAD		pf_med_decode_head;
	PF_MED_DECODE_ITEM		pf_med_decode_item;
	PF_MED_DECODE_DATA		pf_med_decode_data;
	PF_MED_DECODE_SEQUENCE_BEGIN		pf_med_decode_sequence_begin;
	PF_MED_DECODE_SEQUENCE_END			pf_med_decode_sequence_end;
	PF_MED_DECODE_SEQUENCE_ITEM_BEGIN	pf_med_decode_sequence_item_begin;
	PF_MED_DECODE_SEQUENCE_ITEM_END		pf_med_decode_sequence_item_end;

	void* ep;
	PF_MED_ENCODE_HEAD		pf_med_encode_head;
	PF_MED_ENCODE_ITEM		pf_med_encode_item;
	PF_MED_ENCODE_DATA		pf_med_encode_data;
	PF_MED_ENCODE_SEQUENCE_BEGIN		pf_med_encode_sequence_begin;
	PF_MED_ENCODE_SEQUENCE_END			pf_med_encode_sequence_end;
	PF_MED_ENCODE_SEQUENCE_ITEM_BEGIN	pf_med_encode_sequence_item_begin;
	PF_MED_ENCODE_SEQUENCE_ITEM_END		pf_med_encode_sequence_item_end;
}med_context_dataset;

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API med_context_file* med_alloc_filehead(void);

	EXP_API void med_free_filehead(med_context_file* ctx);

	EXP_API void med_set_filehead_read(med_context_file* pctx, void* rp, PF_MED_READ pf_read);

	EXP_API void med_set_filehead_write(med_context_file* pctx, void* wp, PF_MED_WRITE pf_write);

	EXP_API void med_set_filehead_error(med_context_file* pctx, void* pp, PF_MED_ERROR pf_error);

	EXP_API void med_set_filehead_decode(med_context_file* pctx, void* dp, PF_MED_DECODE_FILEHEAD pf_decode_head);

	EXP_API void med_set_filehead_encode(med_context_file* pctx, void* ep, PF_MED_ENCODE_FILEHEAD pf_encode_head);

	EXP_API bool_t med_decode_filehead(med_context_file* pctx);

	EXP_API bool_t med_encode_filehead(med_context_file* pctx);

	/*************************************************************************************************************/

	EXP_API med_context_dataset* med_alloc_dataset(void);

	EXP_API void med_free_dataset(med_context_dataset* ctx);

	EXP_API void med_set_dataset_read(med_context_dataset* pctx, void* rp, PF_MED_READ pf_read);

	EXP_API void med_set_dataset_write(med_context_dataset* pctx, void* wp, PF_MED_WRITE pf_write);

	EXP_API void med_set_dataset_error(med_context_dataset* pctx, void* pp, PF_MED_ERROR pf_error);

	EXP_API void med_set_dataset_decode(med_context_dataset* pctx, void* dp, PF_MED_DECODE_HEAD pf_decode_head, PF_MED_DECODE_ITEM pf_decode_item, PF_MED_DECODE_DATA pf_decode_data, PF_MED_DECODE_SEQUENCE_BEGIN pf_decode_sequence_begin, PF_MED_DECODE_SEQUENCE_END pf_decode_sequence_end, PF_MED_DECODE_SEQUENCE_ITEM_BEGIN pf_decode_sequence_item_begin, PF_MED_DECODE_SEQUENCE_ITEM_END pf_decode_sequence_item_end);

	EXP_API void med_set_dataset_encode(med_context_dataset* pctx, void* ep, PF_MED_ENCODE_HEAD pf_encode_head, PF_MED_ENCODE_ITEM pf_encode_item, PF_MED_ENCODE_DATA pf_encode_data, PF_MED_ENCODE_SEQUENCE_BEGIN pf_encode_sequence_begin, PF_MED_ENCODE_SEQUENCE_END pf_encode_sequence_end, PF_MED_ENCODE_SEQUENCE_ITEM_BEGIN pf_encode_sequence_item_begin, PF_MED_ENCODE_SEQUENCE_ITEM_END pf_encode_sequence_item_end);

	EXP_API bool_t med_decode_dataset(med_context_dataset* pctx);

	EXP_API bool_t med_encode_dataset(med_context_dataset* pctx);

#ifdef	__cplusplus
}
#endif


#endif	/* _MEDCTX_H */


