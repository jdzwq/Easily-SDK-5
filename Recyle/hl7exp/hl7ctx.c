/***********************************************************************
	Easily HL7 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hl7 document

	@module	hl7ctx.c | hl7 implement file

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

#include "hl7ctx.h"
#include "hl7tag.h"

typedef enum{
	HL7_PARSE_END = 0,
	HL7_PARSE_TAG = 1,
	HL7_PARSE_VR = 2,
	HL7_PARSE_SIZE = 3,
	HL7_PARSE_DATA = 4,
	HL7_PARSE_SQ_BEGIN = 5,
	HL7_PARSE_SQ_ITEM_BEGIN = 6,
	HL7_PARSE_SQ_ITEM_END = 7,
	HL7_PARSE_SQ_END = 8
};

typedef enum{
	HL7_FORMAT_END = 0,
	HL7_FORMAT_TAG = 1,
	HL7_FORMAT_VR = 2,
	HL7_FORMAT_SIZE = 3,
	HL7_FORMAT_DATA = 4,
	HL7_FORMAT_SQ_BEGIN = 5,
	HL7_FORMAT_SQ_ITEM_BEGIN = 6,
	HL7_FORMAT_SQ_ITEM_END = 7,
	HL7_FORMAT_SQ_END = 8
};

hl7_context_file* hl7_alloc_head(void)
{
	hl7_context_file* ctx;

	ctx = (hl7_context_file*)xmem_alloc(sizeof(hl7_context_file));

	return ctx;
}

void hl7_free_head(hl7_context_file* ctx)
{
	xmem_free(ctx);
}

hl7_context_message* hl7_alloc_message(void)
{
	hl7_context_message* ctx;

	ctx = (hl7_context_message*)xmem_alloc(sizeof(hl7_context_message));

	return ctx;
}

void hl7_free_message(hl7_context_message* ctx)
{
	xmem_free(ctx);
}

void hl7_set_head_read(hl7_context_file* phl7, void* rp, PF_HL7_READ pf_read)
{
	phl7->rp = rp;
	phl7->pf_hl7_read = pf_read;
}

void hl7_set_head_write(hl7_context_file* phl7, void* wp, PF_HL7_WRITE pf_write)
{
	phl7->wp = wp;
	phl7->pf_hl7_write = pf_write;
}

void hl7_set_head_error(hl7_context_file* phl7, void* ep, PF_HL7_ERROR pf_error)
{
	phl7->ep = ep;
	phl7->pf_hl7_error = pf_error;
}

void hl7_set_message_read(hl7_context_message* phl7, void* rp, PF_HL7_READ pf_read)
{
	phl7->rp = rp;
	phl7->pf_hl7_read = pf_read;
}

void hl7_set_message_write(hl7_context_message* phl7, void* wp, PF_HL7_WRITE pf_write)
{
	phl7->wp = wp;
	phl7->pf_hl7_write = pf_write;
}

void hl7_set_message_error(hl7_context_message* phl7, void* ep, PF_HL7_ERROR pf_error)
{
	phl7->ep = ep;
	phl7->pf_hl7_error = pf_error;
}

void hl7_set_head_parse(hl7_context_file* phl7, void* pp, PF_HL7_PARSE_HEAD pf_parse_head)
{
	phl7->pp = pp;
	phl7->pf_hl7_parse_head = pf_parse_head;
}

void hl7_set_head_format(hl7_context_file* phl7, void* fp, PF_HL7_FORMAT_HEAD pf_format_head)
{
	phl7->fp = fp;
	phl7->pf_hl7_format_head = pf_format_head;
}

void hl7_set_message_parse(hl7_context_message* phl7, void* pp, PF_HL7_PARSE_ITEM pf_parse_item, PF_HL7_PARSE_DATA pf_parse_data, PF_HL7_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_HL7_PARSE_SEQUENCE_END pf_parse_sequence_end)
{
	phl7->pp = pp;

	phl7->pf_hl7_parse_item = pf_parse_item;
	phl7->pf_hl7_parse_data = pf_parse_data;
	phl7->pf_hl7_parse_sequence_begin = pf_parse_sequence_begin;
	phl7->pf_hl7_parse_sequence_end = pf_parse_sequence_end;
}

void hl7_set_message_format(hl7_context_message* phl7, void* fp, PF_HL7_FORMAT_ITEM pf_format_item, PF_HL7_FORMAT_DATA pf_format_data, PF_HL7_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_HL7_FORMAT_SEQUENCE_END pf_format_sequence_end)
{
	phl7->fp = fp;

	phl7->pf_hl7_format_item = pf_format_item;
	phl7->pf_hl7_format_data = pf_format_data;
	phl7->pf_hl7_format_sequence_begin = pf_format_sequence_begin;
	phl7->pf_hl7_format_sequence_end = pf_format_sequence_end;
}

bool_t hl7_parse_head(hl7_context_file* phl7)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	dword_t n;
	byte_t head[128];
	byte_t flag[4];
	bool_t rt;

	TRY_CATCH;

	//skip file head
	n = 128;
	(*phl7->pf_hl7_read)(phl7->rp, head, &n);
	if (n < 128)
	{
		raise_user_error(NULL, NULL);
	}

	//hl7 head
	n = 4;
	(*phl7->pf_hl7_read)(phl7->rp, flag, &n);
	if (n < 4)
	{
		raise_user_error(NULL, NULL);
	}

	if (a_xsnicmp((schar_t*)flag, HL7_VER, 4) != 0)
	{
		raise_user_error(_T("hl7_parse"), _T("read HL7 flag falied"));
	}

	if (phl7->pf_hl7_parse_head)
		rt = (*phl7->pf_hl7_parse_head)(phl7->pp, head, flag);
	else
		rt = 1;

	if (!rt)
	{
		raise_user_error(_T("hl7_parse"), _T("parse head breaked"));
	}

	END_CATCH;

	return 1;

ONERROR:

	if (phl7->pf_hl7_error)
	{
		get_last_error(errcode, errtext, ERR_LEN);
		(*phl7->pf_hl7_error)(phl7->ep, errtext);
	}

	return 0;
}


static void _dec_size(dword_t* p, int i, dword_t d)
{
	do{
		if (p[i] != 0xFFFFFFFF)
			p[i] -= d;
	} while (i--);
}

bool_t hl7_parse_message(hl7_context_message* phl7, dword_t bytes)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	byte_t tag[HL7_IID_SIZE] = { 0 };
	unsigned char vr[4] = { 0 };
	unsigned long vl = 0;
	unsigned char* vf = NULL;

	tchar_t tvr[4] = { 0 };
	dword_t n;
	bool_t b_sq = 0;
	int step;
	bool_t rt;
	
	dword_t size[256] = { 0 };
	int deep = 0;

	TRY_CATCH;

	//HL7 DATASET ENCODE (LitEndian)

	//	ITEM:{
	//		0-7: tag
	//		8-11: vr
	//		12-15: size
	//		16-xxxx: value{
	//			[data | {ITEM}...]
	//		}
	//	}

	size[deep] = bytes;

	step = HL7_PARSE_TAG;

	while (step != HL7_PARSE_END)
	{
		switch (step)
		{
		case HL7_PARSE_TAG:
			//element tag
			n = 8;
			if (!(*phl7->pf_hl7_read)(phl7->rp, tag, &n))
			{
				raise_user_error(NULL, NULL);
			}
			if (!n)
			{
				step = HL7_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);

			vr[0] = vr[1] = vr[2] = vr[3] = 0;

			step = HL7_PARSE_VR;
			break;
		case HL7_PARSE_VR:
			//element vr
			n = 4;
			if(!(*phl7->pf_hl7_read)(phl7->rp, vr, &n))
			{
				raise_user_error(_T("0"), _T("read hl7 vr failed"));
			}
			if (!n)
			{
				step = HL7_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);

#ifdef _UNICODE
			ascii_to_unicode((char*)vr, -1, tvr, 4);
#else
			xmem_copy((void*)tvr, (void*)vr, 4);
#endif
			b_sq = hl7_is_sequence(tvr);

			vl = 0;
			step = HL7_PARSE_SIZE;
			break;
		case HL7_PARSE_SIZE:
			//element size
			n = sizeof(long);
			if (!(*phl7->pf_hl7_read)(phl7->rp, (byte_t*)&vl, &n))
			{
				raise_user_error(_T("0"), _T("read hl7 size failed"));
			}
			if (!n)
			{
				step = HL7_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);

			if (vl > MAX_LONG)
			{
				raise_user_error(_T("0"), _T("invalid data size"));
			}

			if (phl7->pf_hl7_parse_item)
				rt = (*phl7->pf_hl7_parse_item)(phl7->pp, tag, vr, vl);
			else
				rt = 1;

			vf = NULL;
			step = (b_sq) ? HL7_PARSE_SQ_BEGIN : HL7_PARSE_DATA;
			break;
		case HL7_PARSE_DATA:
			//element value
			vf = (unsigned char*)xmem_alloc(vl + 2);
			n = vl;
			if(!(*phl7->pf_hl7_read)(phl7->rp, (byte_t*)vf, &n))
			{
				raise_user_error(_T("0"), _T("read hl7 value failed"));
			}
			_dec_size(size, deep, n);

			if (phl7->pf_hl7_parse_data)
				rt = (*phl7->pf_hl7_parse_data)(phl7->pp, tag, vr, vf, vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse hl7 breaked"));
			}

			xmem_free(vf);
			vf = NULL;

			if (!size[deep] && deep)
				step = HL7_PARSE_SQ_END;
			else if (!size[deep] && !deep)
				step = HL7_PARSE_END;
			else
				step = HL7_PARSE_TAG;
			break;
		case HL7_PARSE_SQ_BEGIN:
			if (phl7->pf_hl7_parse_sequence_begin)
				rt = (*phl7->pf_hl7_parse_sequence_begin)(phl7->pp, tag, vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse hl7 breaked"));
			}

			size[++deep] = vl;

			step = HL7_PARSE_TAG;
			break;
		case HL7_PARSE_SQ_END:
			if (phl7->pf_hl7_parse_sequence_end)
				rt = (*phl7->pf_hl7_parse_sequence_end)(phl7->pp, tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse hl7 breaked"));
			}

			deep--;

			step = HL7_PARSE_TAG;
			break;
		}
	}

	END_CATCH;

	return 1;

ONERROR:

	if (vf)
		xmem_free(vf);

	get_last_error(errcode, errtext, ERR_LEN);

	if (phl7->pf_hl7_error)
	{
		(*phl7->pf_hl7_error)(phl7->ep, errtext);
	}

	return xstos(errcode);
}

/*************************************************************************************************************/
bool_t hl7_format_head(hl7_context_file* phl7)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	dword_t n;
	byte_t flag[4] = { 0 };
	byte_t head[128] = { 0 };
	bool_t rt;

	TRY_CATCH;

	if (phl7->pf_hl7_format_head)
		rt = (*phl7->pf_hl7_format_head)(phl7->fp, head, flag);
	else
		rt = 1;

	if (!rt)
	{
		raise_user_error(_T("hl7_format"), _T("format head failed"));
	}

	n = 128;
	(*phl7->pf_hl7_write)(phl7->wp, head, &n);
	if (n < 128)
	{
		raise_user_error(_T("hl7_format"), _T("write hl7 head failed"));
	}

	n = 4;
	(*phl7->pf_hl7_write)(phl7->wp, HL7_VER, &n);
	if (n < 4)
	{
		raise_user_error(_T("hl7_format"), _T("write hl7 flat failed"));
	}

	END_CATCH;

	return 1;

ONERROR:

	if (phl7->pf_hl7_error)
	{
		get_last_error(errcode, errtext, ERR_LEN);
		(*phl7->pf_hl7_error)(phl7->ep, errtext);
	}

	return 0;
}


bool_t hl7_format_message(hl7_context_message* phl7, dword_t bytes)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	byte_t tag[HL7_IID_SIZE] = { 0 };
	unsigned char vr[4] = { 0 };
	byte_t* vf = NULL;
	dword_t vl = 0;
	bool_t b_sq = 0;

	tchar_t tvr[4] = { 0 };
	dword_t n;
	int rt;
	int step;

	dword_t size[256] = { 0 };
	int deep = 0;

	TRY_CATCH;

	//HL7 DATASET ENCODE (LitEndian)

	//	ITEM:{
	//		0-7: tag
	//		8-11: vr
	//		12-15: size
	//		16-xxxx: value{
	//			[data | {ITEM}...]
	//		}
	//	}

	size[deep] = bytes;

	//command tag
	n = 4;
	(*phl7->pf_hl7_write)(phl7->wp, tag, &n);
	if (!n)
	{
		raise_user_error(NULL, NULL);
	}
	_dec_size(size, deep, n);

	//command size
	n = sizeof(long);
	(*phl7->pf_hl7_write)(phl7->wp, (byte_t*)&vl, &n);
	if (!n)
	{
		raise_user_error(NULL, NULL);
	}
	_dec_size(size, deep, n);

	step = HL7_FORMAT_TAG;

	while (step != HL7_FORMAT_END)
	{
		switch (step)
		{
		case HL7_FORMAT_TAG:
			if (phl7->pf_hl7_format_item)
				rt = (*phl7->pf_hl7_format_item)(phl7->fp, tag, vr, &vl);
			else
				rt = 1;

			if (!rt)
			{
				step = HL7_FORMAT_END;
				break;
			}

			//item tag
			n = 8;
			(*phl7->pf_hl7_write)(phl7->wp, tag, &n);
			if (!n)
			{
				raise_user_error(_T("0"), _T("write hl7 item tag failed"));
			}
			_dec_size(size, deep, n);

			step = HL7_FORMAT_VR;
			break;
		case HL7_FORMAT_VR:
			n = 4;
			(*phl7->pf_hl7_write)(phl7->wp, vr, &n);
			if (!n)
			{
				raise_user_error(_T("0"), _T("write hl7 item vr failed"));
			}
			_dec_size(size, deep, n);

#ifdef _UNICODE
			ascii_to_unicode((char*)vr, -1, tvr, 4);
#else
			xmem_copy((void*)tvr, (void*)vr, 4);
#endif
			b_sq = hl7_is_sequence(tvr);

			step = HL7_FORMAT_SIZE;
			break;
		case HL7_FORMAT_SIZE:
			n = sizeof(long);
			(*phl7->pf_hl7_write)(phl7->wp, (byte_t*)&vl, &n);
			if (!n)
			{
				raise_user_error(_T("0"), _T("write hl7 item length failed"));
			}
			_dec_size(size, deep, n);

			step = (b_sq) ? HL7_FORMAT_SQ_BEGIN : HL7_FORMAT_DATA;
			break;
		case HL7_FORMAT_DATA:
			if (vl > MAX_LONG)
			{
				raise_user_error(_T("0"), _T("parse hl7 breaked"));
			}

			vf = (byte_t*)xmem_alloc(vl + 2);

			if (phl7->pf_hl7_format_data)
				rt = (*phl7->pf_hl7_format_data)(phl7->fp, tag, vr, vf, &vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format hl7 item breaked"));
			}

			n = vl;
			(*phl7->pf_hl7_write)(phl7->wp, vf, &n);
			if (n < vl)
			{
				raise_user_error(_T("0"), _T("write hl7 data failed"));
			}
			_dec_size(size, deep, n);

			xmem_free(vf);
			vf = NULL;

			if (!size[deep] && deep)
				step = HL7_FORMAT_SQ_END;
			else if (!size[deep] && !deep)
				step = HL7_FORMAT_END;
			else
				step = HL7_FORMAT_TAG;
			break;
		case HL7_FORMAT_SQ_BEGIN:
			if (phl7->pf_hl7_format_sequence_begin)
				rt = (*phl7->pf_hl7_format_sequence_begin)(phl7->fp, tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format hl7 sequence breaked"));
			}

			size[++deep] = vl;

			step = HL7_FORMAT_TAG;
			break;
		case HL7_FORMAT_SQ_END:
			if (phl7->pf_hl7_format_sequence_end)
				rt = (*phl7->pf_hl7_format_sequence_end)(phl7->fp, tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format hl7 sequence breaked"));
			}

			deep--;

			step = HL7_FORMAT_TAG;
			break;
		}
	}

	END_CATCH;

	return 1;

ONERROR:

	if (vf)
		xmem_free(vf);

	get_last_error(errcode, errtext, ERR_LEN);

	if (phl7->pf_hl7_error)
	{
		(*phl7->pf_hl7_error)(phl7->ep, errtext);
	}

	return xstos(errcode);
}
