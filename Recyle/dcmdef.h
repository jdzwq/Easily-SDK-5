/*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the MPL as stated above or under the terms of the GNU
* Library General Public License as published by the Free Software Foundation;
* either version 2 of the License, or any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Library general Public License for more
* details.
*/

#ifndef _DCMDEF_H
#define	_DCMDEF_H

typedef struct _dicm_vr_t{
	unsigned char vr[3];
	int vl;
}dicm_vr_t;

typedef struct _dicm_item_t{
	unsigned long tag;
	unsigned char vr[3];
	unsigned char vn[100];
}dicm_item_t;

typedef struct _dicm_gruop_t{
	unsigned short tag;
	unsigned char gn[100];
}dicm_group_t;

/*dicom raw def*/
typedef int(__stdcall *PF_DICM_READ)(void* rp, unsigned char* buf, unsigned long* pb);
typedef int(__stdcall *PF_DICM_WRITE)(void* wp, unsigned char* buf, unsigned long* pb);
typedef void(__stdcall *PF_DICM_ERROR)(void* ep, const char* errtext);

typedef int(__stdcall *PF_DICM_PARSE_ITEM)(void* pp, unsigned long tag, const unsigned char* vr, unsigned long vl);
typedef int(__stdcall *PF_DICM_PARSE_DATA)(void* pp, unsigned long tag, const unsigned char* vr, const unsigned char* vf, unsigned long vl);
typedef int(__stdcall *PF_DICM_PARSE_SEQUENCE_BEGIN)(void* pp, unsigned long tag, unsigned long vl);
typedef int(__stdcall *PF_DICM_PARSE_SEQUENCE_END)(void* pp, unsigned long tag);
typedef int(__stdcall *PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN)(void* pp, unsigned long tag, unsigned long vl);
typedef int(__stdcall *PF_DICM_PARSE_SEQUENCE_ITEM_END)(void* pp, unsigned long tag);

typedef int(__stdcall *PF_DICM_FORMAT_ITEM)(void* fp, unsigned long* ptag, unsigned char* pvr, unsigned long* pvl);
typedef int(__stdcall *PF_DICM_FORMAT_DATA)(void* fp, unsigned long* ptag, unsigned char* pvr, unsigned char* pvf, unsigned long* pvl);
typedef int(__stdcall *PF_DICM_FORMAT_SEQUENCE_BEGIN)(void* fp, unsigned long* ptag);
typedef int(__stdcall *PF_DICM_FORMAT_SEQUENCE_END)(void* fp, unsigned long* ptag);
typedef int(__stdcall *PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN)(void* fp, unsigned long* ptag);
typedef int(__stdcall *PF_DICM_FORMAT_SEQUENCE_ITEM_END)(void* fp, unsigned long* ptag);

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

	int b_Implicit;
	int b_BigEndian;
	int n_CharSet;
}dicm_context_t;

#define DICM_CONTINUE	0
#define DICM_BREAK		1
#define DICM_ERROR		-1

#define DICM_CHARSET_ENG	0
#define DICM_CHARSET_UTF	1
#define DICM_CHARSET_GBK	2

#define IS_SENS_VR(ba)		((ba[0] == 'S' && ba[1] == 'H') || (ba[0] == 'L' && ba[1] == 'O') || (ba[0] == 'U' && ba[1] == 'C') || (ba[0] == 'S' && ba[1] == 'T') || (ba[0] == 'L' && ba[1] == 'T') || (ba[0] == 'U' && ba[1] == 'T') || (ba[0] == 'P' && ba[1] == 'N'))
#define IS_NUM_VR(ba)		((ba[0] == 'F' && ba[1] == 'L') || (ba[0] == 'F' && ba[1] == 'D') || (ba[0] == 'S' && ba[1] == 'S') || (ba[0] == 'S' && ba[1] == 'L') || (ba[0] == 'U' && ba[1] == 'S') || (ba[0] == 'U' && ba[1] == 'L'))
#define IS_META_TAG(n)		(((n & 0xFFFF0000) == 0x00020000)? 1 : 0)

#define DICM_MAX_POINTS		16

#endif	/* _DCMDEF_H */

