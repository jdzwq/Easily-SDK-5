/***********************************************************************
	Easily HL7 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hl7 document

	@module	hl7ctx.h | hl7 interface file

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


#ifndef _HL7CTX_H
#define	_HL7CTX_H

#include "hl7def.h"

/*hl7 raw def*/
typedef bool_t(*PF_HL7_READ)(void* rp, byte_t* buf, dword_t* pb);
typedef bool_t(*PF_HL7_WRITE)(void* wp, byte_t* buf, dword_t* pb);
typedef void(*PF_HL7_ERROR)(void* ep, const tchar_t* errtext);

typedef bool_t(*PF_HL7_PARSE_HEAD)(void* pp, const byte_t head[128], const byte_t flag[4]);
typedef bool_t(*PF_HL7_FORMAT_HEAD)(void* fp, byte_t head[128], byte_t flag[4]);

typedef bool_t(*PF_HL7_PARSE_ITEM)(void* pp, const byte_t* tag, const byte_t* vr, dword_t vl);
typedef bool_t(*PF_HL7_PARSE_DATA)(void* pp, const byte_t* tag, const byte_t* vr, const byte_t* vf, dword_t vl);
typedef bool_t(*PF_HL7_PARSE_SEQUENCE_BEGIN)(void* pp, const byte_t* tag, dword_t vl);
typedef bool_t(*PF_HL7_PARSE_SEQUENCE_END)(void* pp, const byte_t* tag);

typedef bool_t(*PF_HL7_FORMAT_ITEM)(void* fp, byte_t* tag, byte_t* pvr, dword_t* pvl);
typedef bool_t(*PF_HL7_FORMAT_DATA)(void* fp, byte_t* tag, byte_t* pvr, byte_t* pvf, dword_t* pvl);
typedef bool_t(*PF_HL7_FORMAT_SEQUENCE_BEGIN)(void* fp, byte_t* tag);
typedef bool_t(*PF_HL7_FORMAT_SEQUENCE_END)(void* fp, byte_t* tag);

#define HL7_VER			"HL73"

typedef struct _hl7_context_file{
	int op;

	void*	rp;
	PF_HL7_READ		pf_hl7_read;

	void* wp;
	PF_HL7_WRITE		pf_hl7_write;

	void* ep;
	PF_HL7_ERROR		pf_hl7_error;

	void* pp;
	PF_HL7_PARSE_HEAD	pf_hl7_parse_head;

	void* fp;
	PF_HL7_FORMAT_HEAD	pf_hl7_format_head;
}hl7_context_file;

typedef struct _hl7_context_message{
	int op;

	void*	rp;
	PF_HL7_READ		pf_hl7_read;

	void* wp;
	PF_HL7_WRITE		pf_hl7_write;

	void* ep;
	PF_HL7_ERROR		pf_hl7_error;

	void* pp;
	PF_HL7_PARSE_ITEM		pf_hl7_parse_item;
	PF_HL7_PARSE_DATA		pf_hl7_parse_data;
	PF_HL7_PARSE_SEQUENCE_BEGIN	pf_hl7_parse_sequence_begin;
	PF_HL7_PARSE_SEQUENCE_END		pf_hl7_parse_sequence_end;

	void* fp;
	PF_HL7_FORMAT_ITEM		pf_hl7_format_item;
	PF_HL7_FORMAT_DATA		pf_hl7_format_data;
	PF_HL7_FORMAT_SEQUENCE_BEGIN	pf_hl7_format_sequence_begin;
	PF_HL7_FORMAT_SEQUENCE_END		pf_hl7_format_sequence_end;
}hl7_context_message;

#ifdef	__cplusplus
extern "C" {
#endif

	HL7_API hl7_context_file* hl7_alloc_head(void);

	HL7_API void hl7_free_head(hl7_context_file* ctx);

	HL7_API void hl7_set_head_read(hl7_context_file* pdcm, void* rp, PF_HL7_READ pf_read);

	HL7_API void hl7_set_head_write(hl7_context_file* pdcm, void* wp, PF_HL7_WRITE pf_write);

	HL7_API void hl7_set_head_error(hl7_context_file* pdcm, void* ep, PF_HL7_ERROR pf_error);

	HL7_API void hl7_set_head_parse(hl7_context_file* pdcm, void* pp, PF_HL7_PARSE_HEAD pf_parse_head);

	HL7_API void hl7_set_head_format(hl7_context_file* pdcm, void* fp, PF_HL7_FORMAT_HEAD pf_format_head);

	HL7_API bool_t hl7_parse_head(hl7_context_file* pdcm);

	HL7_API bool_t hl7_format_head(hl7_context_file* pdcm);

	HL7_API hl7_context_message* hl7_alloc_message(void);

	HL7_API void hl7_free_message(hl7_context_message* ctx);

	HL7_API void hl7_set_message_read(hl7_context_message* pdcm, void* rp, PF_HL7_READ pf_read);

	HL7_API void hl7_set_message_write(hl7_context_message* pdcm, void* wp, PF_HL7_WRITE pf_write);

	HL7_API void hl7_set_message_error(hl7_context_message* pdcm, void* ep, PF_HL7_ERROR pf_error);

	HL7_API void hl7_set_message_parse(hl7_context_message* pdcm, void* pp, PF_HL7_PARSE_ITEM pf_parse_item, PF_HL7_PARSE_DATA pf_parse_data, PF_HL7_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_HL7_PARSE_SEQUENCE_END pf_parse_sequence_end);

	HL7_API void hl7_set_message_format(hl7_context_message* pdcm, void* pp, PF_HL7_FORMAT_ITEM pf_format_item, PF_HL7_FORMAT_DATA pf_format_data, PF_HL7_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_HL7_FORMAT_SEQUENCE_END pf_format_sequence_end);

	HL7_API bool_t hl7_parse_message(hl7_context_message* pdcm, dword_t size);

	HL7_API bool_t hl7_format_message(hl7_context_message* pdcm, dword_t size);

#ifdef	__cplusplus
}
#endif


#endif	/* _HL7CTX_H */


