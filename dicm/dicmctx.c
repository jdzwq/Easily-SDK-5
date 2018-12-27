/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmctx.c | dicom implement file

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

#include "dicmctx.h"
#include "dicmtag.h"

typedef enum{
	DICM_PARSE_END = 0,
	DICM_PARSE_TAG = 1,
	DICM_PARSE_VR = 2,
	DICM_PARSE_SIZE = 3,
	DICM_PARSE_DATA = 4,
	DICM_PARSE_SQ_BEGIN = 5,
	DICM_PARSE_SQ_ITEM_BEGIN = 6,
	DICM_PARSE_SQ_ITEM_END = 7,
	DICM_PARSE_SQ_END = 8
};

typedef enum{
	DICM_FORMAT_END = 0,
	DICM_FORMAT_TAG = 1,
	DICM_FORMAT_VR = 2,
	DICM_FORMAT_SIZE = 3,
	DICM_FORMAT_DATA = 4,
	DICM_FORMAT_SQ_BEGIN = 5,
	DICM_FORMAT_SQ_ITEM_BEGIN = 6,
	DICM_FORMAT_SQ_ITEM_END = 7,
	DICM_FORMAT_SQ_END = 8
};


dicm_context_file* dicm_alloc_head(void)
{
	dicm_context_file* ctx;

	ctx = (dicm_context_file*)xmem_alloc(sizeof(dicm_context_file));

	return ctx;
}

void dicm_free_head(dicm_context_file* ctx)
{
	xmem_free(ctx);
}

dicm_conext_command* dicm_alloc_command(void)
{
	dicm_conext_command* ctx;

	ctx = (dicm_conext_command*)xmem_alloc(sizeof(dicm_conext_command));

	return ctx;
}

void dicm_free_command(dicm_conext_command* ctx)
{
	xmem_free(ctx);
}

dicm_context_dataset* dicm_alloc_dataset(void)
{
	dicm_context_dataset* ctx;

	ctx = (dicm_context_dataset*)xmem_alloc(sizeof(dicm_context_dataset));

	ctx->b_Implicit = 0;
	ctx->b_BigEndian = 0;
	ctx->n_CharSet = 0;

	return ctx;
}

void dicm_free_dataset(dicm_context_dataset* ctx)
{
	xmem_free(ctx);
}

void dicm_set_head_read(dicm_context_file* pdcm, void* rp, PF_DICM_READ pf_read)
{
	pdcm->rp = rp;
	pdcm->pf_dicm_read = pf_read;
}

void dicm_set_head_write(dicm_context_file* pdcm, void* wp, PF_DICM_WRITE pf_write)
{
	pdcm->wp = wp;
	pdcm->pf_dicm_write = pf_write;
}

void dicm_set_head_error(dicm_context_file* pdcm, void* ep, PF_DICM_ERROR pf_error)
{
	pdcm->ep = ep;
	pdcm->pf_dicm_error = pf_error;
}

void dicm_set_command_read(dicm_conext_command* pdcm, void* rp, PF_DICM_READ pf_read)
{
	pdcm->rp = rp;
	pdcm->pf_dicm_read = pf_read;
}

void dicm_set_command_write(dicm_conext_command* pdcm, void* wp, PF_DICM_WRITE pf_write)
{
	pdcm->wp = wp;
	pdcm->pf_dicm_write = pf_write;
}

void dicm_set_command_error(dicm_conext_command* pdcm, void* ep, PF_DICM_ERROR pf_error)
{
	pdcm->ep = ep;
	pdcm->pf_dicm_error = pf_error;
}

void dicm_set_dataset_read(dicm_context_dataset* pdcm, void* rp, PF_DICM_READ pf_read)
{
	pdcm->rp = rp;
	pdcm->pf_dicm_read = pf_read;
}

void dicm_set_dataset_write(dicm_context_dataset* pdcm, void* wp, PF_DICM_WRITE pf_write)
{
	pdcm->wp = wp;
	pdcm->pf_dicm_write = pf_write;
}

void dicm_set_dataset_error(dicm_context_dataset* pdcm, void* ep, PF_DICM_ERROR pf_error)
{
	pdcm->ep = ep;
	pdcm->pf_dicm_error = pf_error;
}

void dicm_set_head_parse(dicm_context_file* pdcm, void* pp, PF_DICM_PARSE_HEAD pf_parse_head)
{
	pdcm->pp = pp;
	pdcm->pf_dicm_parse_head = pf_parse_head;
}

void dicm_set_head_format(dicm_context_file* pdcm, void* fp, PF_DICM_FORMAT_HEAD pf_format_head)
{
	pdcm->fp = fp;
	pdcm->pf_dicm_format_head = pf_format_head;
}

void dicm_set_command_parse(dicm_conext_command* pdcm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data)
{
	pdcm->pp = pp;
	pdcm->pf_dicm_parse_item = pf_parse_item;
	pdcm->pf_dicm_parse_data = pf_parse_data;
}

void dicm_set_command_format(dicm_conext_command* pdcm, void* fp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data)
{
	pdcm->fp = fp;
	pdcm->pf_dicm_format_item = pf_format_item;
	pdcm->pf_dicm_format_data = pf_format_data;
}

void dicm_set_dataset_parse(dicm_context_dataset* pdcm, void* pp, PF_DICM_PARSE_ITEM pf_parse_item, PF_DICM_PARSE_DATA pf_parse_data, PF_DICM_PARSE_SEQUENCE_BEGIN pf_parse_sequence_begin, PF_DICM_PARSE_SEQUENCE_END pf_parse_sequence_end, PF_DICM_PARSE_SEQUENCE_ITEM_BEGIN pf_parse_sequence_item_begin, PF_DICM_PARSE_SEQUENCE_ITEM_END pf_parse_sequence_item_end)
{
	pdcm->pp = pp;
	pdcm->pf_dicm_parse_item = pf_parse_item;
	pdcm->pf_dicm_parse_data = pf_parse_data;
	pdcm->pf_dicm_parse_sequence_begin = pf_parse_sequence_begin;
	pdcm->pf_dicm_parse_sequence_end = pf_parse_sequence_end;
	pdcm->pf_dicm_parse_sequence_item_begin = pf_parse_sequence_item_begin;
	pdcm->pf_dicm_parse_sequence_item_end = pf_parse_sequence_item_end;
}

void dicm_set_dataset_format(dicm_context_dataset* pdcm, void* fp, PF_DICM_FORMAT_ITEM pf_format_item, PF_DICM_FORMAT_DATA pf_format_data, PF_DICM_FORMAT_SEQUENCE_BEGIN pf_format_sequence_begin, PF_DICM_FORMAT_SEQUENCE_END pf_format_sequence_end, PF_DICM_FORMAT_SEQUENCE_ITEM_BEGIN pf_format_sequence_item_begin, PF_DICM_FORMAT_SEQUENCE_ITEM_END pf_format_sequence_item_end)
{
	pdcm->fp = fp;
	pdcm->pf_dicm_format_item = pf_format_item;
	pdcm->pf_dicm_format_data = pf_format_data;
	pdcm->pf_dicm_format_sequence_begin = pf_format_sequence_begin;
	pdcm->pf_dicm_format_sequence_end = pf_format_sequence_end;
	pdcm->pf_dicm_format_sequence_item_begin = pf_format_sequence_item_begin;
	pdcm->pf_dicm_format_sequence_item_end = pf_format_sequence_item_end;
}

bool_t dicm_parse_head(dicm_context_file* pdcm)
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
	(*pdcm->pf_dicm_read)(pdcm->rp, head, &n);
	if (n < 128)
	{
		raise_user_error(NULL, NULL);
	}

	//dicm head
	n = 4;
	(*pdcm->pf_dicm_read)(pdcm->rp, flag, &n);
	if (n < 4)
	{
		raise_user_error(NULL, NULL);
	}

	if (a_xsnicmp((schar_t*)flag, "DICM", 4) != 0)
	{
		raise_user_error(_T("dcm_parse"), _T("read DICM flag falied"));
	}

	if (pdcm->pf_dicm_parse_head)
		rt = (*pdcm->pf_dicm_parse_head)(pdcm->pp, head, flag);
	else
		rt = 1;

	if (!rt)
	{
		raise_user_error(_T("dcm_parse"), _T("parse head breaked"));
	}

	END_CATCH;

	return 1;

ONERROR:

	if (pdcm->pf_dicm_error)
	{
		get_last_error(errcode, errtext, ERR_LEN);
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return 0;
}

bool_t dicm_parse_command(dicm_conext_command* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	unsigned short vt[2] = { 0 };
	unsigned char vr[2] = { 0 };
	unsigned long vl = 0;
	unsigned char* vf = NULL;

	unsigned long ul = 0;
	unsigned short us = 0;
	unsigned char rev[4] = { 0 };

	dword_t n, total;
	dword_t tag;
	int i, step;
	bool_t rt;
	const dicm_item_t* pdi;

	TRY_CATCH;

	//DICOM COMMAND SET DEFAULT TRANSFER SYNTAX (LitEndian)

	//command set:{
	//	1-4: tag
	//	5-8: size
	//	9-xxx: value

	//group tag
	total = DICM_CMD_TAG_SIZE + DICM_CMD_LEN_SIZE + 4;

	step = DICM_PARSE_TAG;

	while (step != DICM_PARSE_END)
	{
		switch (step)
		{
		case DICM_PARSE_TAG:
			//group tag
			n = 2;
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&(vt[0]), &n))
			{
				raise_user_error(NULL, NULL);
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			total -= n;

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&(vt[0]), n);
			}

			//element tag
			n = 2;
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&(vt[1]), &n))
			{
				raise_user_error(NULL, NULL);
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			total -= n;

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&(vt[1]), n);
			}

			tag = MAKEDWORD(vt[1], vt[0]);

			pdi = dicm_item_info(tag);
			if (pdi)
			{
				vr[0] = (byte_t)pdi->vr[0];
				vr[1] = (byte_t)pdi->vr[1];
			}
			else
			{
				vr[0] = vr[1] = 0;
			}

			vl = 0;
			vf = NULL;

			step = DICM_PARSE_SIZE;
			break;
		case DICM_PARSE_SIZE:
			//element size
			ul = 0;
			n = sizeof(long);
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&ul, &n))
			{
				raise_user_error(NULL, NULL);
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			total -= n;

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&ul, n);
			}

			vl = ul;
			if (vl > ((pdcm->n_maxLength) ? pdcm->n_maxLength : MAX_LONG))
			{
				raise_user_error(_T("0"), _T("invalid data size"));
			}

			if (pdcm->pf_dicm_parse_item)
				rt = (*pdcm->pf_dicm_parse_item)(pdcm->pp, tag, vr, vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse dicm breaked"));
			}

			step = DICM_PARSE_DATA;
			break;
		case DICM_PARSE_DATA:
			//element value
			vf = (unsigned char*)xmem_alloc(vl + 2);
			n = vl;
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)vf, &n))
			{
				raise_user_error(NULL, NULL);
			}

			if (vr[0] == 'O' && vr[1] == 'W')
			{
				if (pdcm->b_BigEndian)
				{
					for (i = 0; i < (int)(n / 2); i += 2)
					{
						bytes_turn(vf + i, 2);
					}
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'D') //8
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 8);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}

			if (pdcm->pf_dicm_parse_data)
				rt = (*pdcm->pf_dicm_parse_data)(pdcm->pp, tag, vr, vf, vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse dicm breaked"));
			}

			//group size
			if (tag == 0x00000000)
				total = *((long*)vf);
			else
				total -= n;

			xmem_free(vf);
			vf = NULL;

			step = (total) ? DICM_PARSE_TAG : DICM_PARSE_END;
			break;
		}
	}

	END_CATCH;

	return 1;

ONERROR:

	if (vf)
		xmem_free(vf);

	get_last_error(errcode, errtext, ERR_LEN);

	if (pdcm->pf_dicm_error)
	{
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return xstos(errcode);
}

static void _dec_size(dword_t* p, int i, dword_t d)
{
	do{
		if (p[i] != 0xFFFFFFFF)
			p[i] -= d;
	} while (i--);
}

bool_t dicm_parse_dataset(dicm_context_dataset* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	unsigned short vt[2] = { 0 };
	unsigned char vr[2] = { 0 };
	unsigned long vl = 0;
	unsigned char* vf = NULL;

	unsigned long ul = 0;
	unsigned short us = 0;
	unsigned char rev[4] = { 0 };

	dword_t n;
	dword_t tag;
	bool_t b_sq = 0;
	int i, step;
	bool_t rt;
	
	dword_t size[256] = { 0 };
	int deep = 0;

	TRY_CATCH;

	//DICOM DATA SET DEFAULT TRANSFER SYNTAX (LitEndian)

	//implicit dataset:{
	//	1-4: tag
	//	5-8: size
	//	9-xxx: value

	//explicit dataset:{
	//	1-4: tag
	//	5-6: vr
	//	7-10: size
	//	11-xxx: value

	size[deep] = 0xFFFFFFFF;
	step = DICM_PARSE_TAG;

	while (step != DICM_PARSE_END)
	{
		switch (step)
		{
		case DICM_PARSE_TAG:
			//group tag
			n = 2;
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&(vt[0]), &n))
			{
				raise_user_error(NULL, NULL);
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&(vt[0]), n);
			}

			//element tag
			n = 2;
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&(vt[1]), &n))
			{
				raise_user_error(NULL, NULL);
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&(vt[1]), n);
			}

			tag = MAKEDWORD(vt[1], vt[0]);

			if (tag == 0xFFFEE000 || tag == 0xFFFEE00D || tag == 0xFFFEE0DD)
			{
				step = DICM_PARSE_SIZE;
			}
			else
			{
				if (pdcm->b_Implicit && !DICM_IS_META_TAG(tag))
				{
					b_sq = dicm_is_sequence(tag);
					step = DICM_PARSE_SIZE;
				}
				else
				{
					step = DICM_PARSE_VR;
				}
			}

			vr[0] = vr[1] = 0;
			vl = 0;
			vf = NULL;
			break;
		case DICM_PARSE_VR:
			//element vr
			n = sizeof(short);
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)vr, &n))
			{
				raise_user_error(_T("0"), _T("read dicm vr failed"));
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);
			
			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)vr, n);
			}

			b_sq = (vr[0] == 'S' && vr[1] == 'Q') ? 1 : 0;

			if (!pdcm->b_Implicit || DICM_IS_META_TAG(tag))
			{
				if ((vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'O' && vr[1] == 'W') || (vr[0] == 'S' && vr[1] == 'Q'))
				{
					//skip 2 bytes
					n = sizeof(short);
					if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)rev, &n))
					{
						raise_user_error(_T("0"), _T("read dicm size failed"));
					}
					if (!n)
					{
						step = DICM_PARSE_END;
						break;
					}
					_dec_size(size, deep, n);

					step = DICM_PARSE_SIZE;
				}
				else
				{
					//short size
					us = 0;
					n = sizeof(short);
					if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&us, &n))
					{
						raise_user_error(_T("0"), _T("read dicm size failed"));
					}
					if (!n)
					{
						step = DICM_PARSE_END;
						break;
					}
					_dec_size(size, deep, n);

					if (pdcm->b_BigEndian)
					{
						bytes_turn((byte_t*)&us, n);
					}

					vl = (dword_t)us;
					size[++deep] = 0xFFFFFFFF;

					if (pdcm->pf_dicm_parse_item)
						rt = (*pdcm->pf_dicm_parse_item)(pdcm->pp, tag, vr, vl);
					else
						rt = 1;

					if (!rt)
					{
						raise_user_error(_T("1"), _T("parse dicm breaked"));
					}

					step = DICM_PARSE_DATA;
				}
			}
			else
			{
				step = DICM_PARSE_SIZE;
			}
			break;
		case DICM_PARSE_SIZE:
			//element size
			ul = 0;
			n = sizeof(long);
			if (!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)&ul, &n))
			{
				raise_user_error(_T("0"), _T("read dicm size failed"));
			}
			if (!n)
			{
				step = DICM_PARSE_END;
				break;
			}
			_dec_size(size, deep, n);

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&ul, n);
			}

			vl = ul;
			if (vl > ((pdcm->n_maxLength) ? pdcm->n_maxLength : MAX_LONG))
			{
				raise_user_error(_T("0"), _T("invalid data size"));
			}

			if (tag != 0xFFFEE00D && tag != 0xFFFEE0DD)
			{
				if (b_sq || tag == 0xFFFEE000)
					size[++deep] = vl;
				else
					size[++deep] = 0xFFFFFFFF;
			}

			if (tag == 0xFFFEE000)
			{
				step = DICM_PARSE_SQ_ITEM_BEGIN;
				break;
			}
			else if (tag == 0xFFFEE00D)
			{
				step = DICM_PARSE_SQ_ITEM_END;
				break;
			}
			else if (tag == 0xFFFEE0DD)
			{
				step = DICM_PARSE_SQ_END;
				break;
			}
			else
			{
				if (pdcm->pf_dicm_parse_item)
					rt = (*pdcm->pf_dicm_parse_item)(pdcm->pp, tag, vr, vl);
				else
					rt = 1;

				if (!rt)
				{
					raise_user_error(_T("1"), _T("parse dicm breaked"));
				}

				if (b_sq)
				{
					step = DICM_PARSE_SQ_BEGIN;
				}
				else
				{
					step = DICM_PARSE_DATA;
				}
			}
			break;
		case DICM_PARSE_DATA:
			//element value
			vf = (unsigned char*)xmem_alloc(vl + 2);
			n = vl;
			if(!(*pdcm->pf_dicm_read)(pdcm->rp, (byte_t*)vf, &n))
			{
				raise_user_error(_T("0"), _T("read dicm value failed"));
			}
			_dec_size(size, deep, n);

			if (vr[0] == 'O' && vr[1] == 'W')
			{
				if (pdcm->b_BigEndian)
				{
					for (i = 0; i < (int)(n / 2); i += 2)
					{
						bytes_turn(vf + i, 2);
					}
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'D') //8
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 8);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}

			if (tag == 0x00020010)
			{
				if (a_xscmp((schar_t*)vf, "1.2.840.10008.1.2.1") == 0)
				{
					pdcm->b_Implicit = 0;
					pdcm->b_BigEndian = 0;
				}
				else if (a_xscmp((schar_t*)vf, "1.2.840.10008.1.2.2") == 0)
				{
					pdcm->b_Implicit = 0;
					pdcm->b_BigEndian = 1;
				}
				else if (a_xscmp((schar_t*)vf, "1.2.840.10008.1.2") == 0)
				{
					pdcm->b_Implicit = 1;
					pdcm->b_BigEndian = 0;
				}
			}
			else if (tag == 0x00080005)
			{
				if (a_xsnicmp((schar_t*)vf, "ISO_IR 192", 10) == 0)
					pdcm->n_CharSet = DICM_CHARSET_UTF;
				else if (a_xsnicmp((schar_t*)vf, "GB18030", 7) == 0)
					pdcm->n_CharSet = DICM_CHARSET_GBK;
				else
					pdcm->n_CharSet = DICM_CHARSET_ENG;
			}

			if (pdcm->pf_dicm_parse_data)
				rt = (*pdcm->pf_dicm_parse_data)(pdcm->pp, tag, vr, vf, vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse dicm breaked"));
			}

			xmem_free(vf);
			vf = NULL;

			deep--;

			if (!size[deep])
				step = DICM_PARSE_SQ_ITEM_END;
			else
				step = DICM_PARSE_TAG;
			break;
		case DICM_PARSE_SQ_BEGIN:
			if (pdcm->pf_dicm_parse_sequence_begin)
				rt = (*pdcm->pf_dicm_parse_sequence_begin)(pdcm->pp, tag, vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse dicm breaked"));
			}

			step = DICM_PARSE_TAG;
			break;
		case DICM_PARSE_SQ_ITEM_BEGIN:
			if (pdcm->pf_dicm_parse_sequence_item_begin)
				rt = (*pdcm->pf_dicm_parse_sequence_item_begin)(pdcm->pp, tag, vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse dicm breaked"));
			}

			step = DICM_PARSE_TAG;
			break;
		case DICM_PARSE_SQ_ITEM_END:
			if (pdcm->pf_dicm_parse_sequence_item_end)
				rt = (*pdcm->pf_dicm_parse_sequence_item_end)(pdcm->pp, tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse dicm breaked"));
			}

			deep--;

			if (size[deep])
				step = DICM_PARSE_TAG;
			else
				step = DICM_PARSE_SQ_END;
			break;
		case DICM_PARSE_SQ_END:
			if (pdcm->pf_dicm_parse_sequence_end)
				rt = (*pdcm->pf_dicm_parse_sequence_end)(pdcm->pp, tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("parse dicm breaked"));
			}

			deep--;

			step = DICM_PARSE_TAG;
			break;
		}
	}

	END_CATCH;

	return 1;

ONERROR:

	if (vf)
		xmem_free(vf);

	get_last_error(errcode, errtext, ERR_LEN);

	if (pdcm->pf_dicm_error)
	{
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return xstos(errcode);
}

/*************************************************************************************************************/
bool_t dicm_format_head(dicm_context_file* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	dword_t n;
	byte_t head[128] = { 0 };
	byte_t flag[4] = { _T('D'),_T('I'),_T('C'),_T('M') };
	bool_t rt;

	TRY_CATCH;

	if (pdcm->pf_dicm_format_head)
		rt = (*pdcm->pf_dicm_format_head)(pdcm->fp, head, flag);
	else
		rt = 1;

	if (!rt)
	{
		raise_user_error(_T("dcm_format"), _T("format head failed"));
	}

	n = 128;
	(*pdcm->pf_dicm_write)(pdcm->wp, head, &n);
	if (n < 128)
	{
		raise_user_error(_T("dcm_format"), _T("write dicm head failed"));
	}

	n = 4;
	(*pdcm->pf_dicm_write)(pdcm->wp, flag, &n);
	if (n < 4)
	{
		raise_user_error(_T("dcm_format"), _T("write dicm flat failed"));
	}

	END_CATCH;

	return 1;

ONERROR:

	if (pdcm->pf_dicm_error)
	{
		get_last_error(errcode, errtext, ERR_LEN);
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return 0;
}

bool_t dicm_format_command(dicm_conext_command* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	dword_t tag;
	sword_t vt[2] = { 0 };
	byte_t vr[3] = { 0 };
	byte_t* vf = NULL;
	dword_t vl = 0;
	bool_t b_sq = 0;

	dword_t n;
	int i, rt;
	int step;

	dword_t ul;
	dword_t size = DICM_CMD_TAG_SIZE + DICM_CMD_LEN_SIZE + 4; //group tag, size, value

	//DICOM COMMAND SET DEFAULT TRANSFER SYNTAX (LitEndian)

	//command set:{
	//	1-4: tag
	//	5-8: size
	//	9-xxx: value
	//	}

	TRY_CATCH;

	step = DICM_FORMAT_TAG;

	while (step != DICM_FORMAT_END)
	{
		switch (step)
		{
		case DICM_FORMAT_TAG:
			if (pdcm->pf_dicm_format_item)
				rt = (*pdcm->pf_dicm_format_item)(pdcm->fp, &tag, vr, &vl);
			else
				rt = 1;

			if (!rt)
			{
				step = DICM_FORMAT_END;
				break;
			}

			vt[0] = GETHSWORD(tag);
			vt[1] = GETLSWORD(tag);

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&(vt[0]), 2);
				bytes_turn((byte_t*)&(vt[1]), 2);
			}

			n = 2;
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&(vt[0]), &n);
			if (!n)
			{
				raise_user_error(NULL, NULL);
			}
			size -= n;

			n = 2;
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&(vt[1]), &n);
			if (!n)
			{
				raise_user_error(NULL, NULL);
			}
			size -= n;

			step = DICM_FORMAT_SIZE;
			break;
		case DICM_FORMAT_SIZE:
			ul = vl;
			n = sizeof(long);
			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&ul, n);
			}
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&ul, &n);
			if (!n)
			{
				raise_user_error(NULL, NULL);
			}
			size -= n;

			step = DICM_FORMAT_DATA;
			break;
		case DICM_FORMAT_DATA:
			if (vl > ((pdcm->n_maxLength)? pdcm->n_maxLength : MAX_LONG))
			{
				raise_user_error(_T("0"), _T("parse dicm breaked"));
			}

			vf = (byte_t*)xmem_alloc(vl + 2);

			if (pdcm->pf_dicm_format_data)
				rt = (*pdcm->pf_dicm_format_data)(pdcm->fp, &tag, vr, vf, &vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format dicm item breaked"));
			}

			if (vr[0] == 'O' && vr[1] == 'W')
			{
				if (pdcm->b_BigEndian)
				{
					for (i = 0; i < (int)(vl / 2); i += 2)
					{
						bytes_turn(vf + i, 2);
					}
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'D') //8
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 8);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}

			n = vl;
			(*pdcm->pf_dicm_write)(pdcm->wp, vf, &n);
			if (n < vl)
			{
				raise_user_error(_T("0"), _T("write dicm data failed"));
			}
			size -= n;

			//group size
			if (tag == 0x00000000)
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
				size = *((long*)vf);
			}

			xmem_free(vf);
			vf = NULL;

			step = (size) ? DICM_FORMAT_TAG : DICM_FORMAT_END;
			break;
		}
	}

	END_CATCH;

	return 1;

ONERROR:

	if (vf)
		xmem_free(vf);

	get_last_error(errcode, errtext, ERR_LEN);

	if (pdcm->pf_dicm_error)
	{
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return xstos(errcode);
}

bool_t dicm_format_dataset(dicm_context_dataset* pdcm)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	dword_t tag;
	sword_t vt[2] = { 0 };
	byte_t vr[3] = { 0 };
	byte_t* vf = NULL;
	dword_t vl = 0;
	bool_t b_sq = 0;

	dword_t n;
	int i, rt;
	int step;

	byte_t ub[2];
	dword_t ul;
	sword_t us;

	dword_t size[256] = { 0 };
	int deep = 0;

	TRY_CATCH;

	//DICOM DATASET SET DEFAULT TRANSFER SYNTAX (LitEndian)

	size[deep] = 0xFFFFFFFF;
	step = DICM_FORMAT_TAG;

	while (step != DICM_FORMAT_END)
	{
		switch (step)
		{
		case DICM_FORMAT_TAG:
			if (pdcm->pf_dicm_format_item)
				rt = (*pdcm->pf_dicm_format_item)(pdcm->fp, &tag, vr, &vl);
			else
				rt = 1;

			if (!rt)
			{
				step = DICM_FORMAT_END;
				break;
			}

			vt[0] = GETHSWORD(tag);
			vt[1] = GETLSWORD(tag);

			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&(vt[0]), 2);
				bytes_turn((byte_t*)&(vt[1]), 2);
			}

			n = 2;
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&(vt[0]), &n);
			if (!n)
			{
				raise_user_error(_T("0"), _T("write dicm item tag failed"));
			}
			_dec_size(size, deep, n);

			n = 2;
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&(vt[1]), &n);
			if (!n)
			{
				raise_user_error(_T("0"), _T("write dicm item tag failed"));
			}
			_dec_size(size, deep, n);

			if (tag == 0xFFFEE000 || tag == 0xFFFEE00D || tag == 0xFFFEE0DD)
			{
				step = DICM_FORMAT_SIZE;
			}
			else
			{
				if (pdcm->b_Implicit && !DICM_IS_META_TAG(tag))
				{
					b_sq = dicm_is_sequence(tag);
					step = DICM_FORMAT_SIZE;
				}
				else
				{
					step = DICM_FORMAT_VR;
				}
			}
			break;
		case DICM_FORMAT_VR:
			memcpy((void*)ub, (void*)vr, 2);
			n = sizeof(short);
			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)ub, n);
			}
			(*pdcm->pf_dicm_write)(pdcm->wp, ub, &n);
			if (!n)
			{
				raise_user_error(_T("0"), _T("write dicm item vr failed"));
			}
			_dec_size(size, deep, n);

			b_sq = (vr[0] == 'S' && vr[1] == 'Q') ? 1 : 0;

			if ((vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'O' && vr[1] == 'W') || (vr[0] == 'S' && vr[1] == 'Q'))
			{
				us = 0;
				n = sizeof(short);
				(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&us, &n);
				if (!n)
				{
					raise_user_error(_T("0"), _T("write dicm item length failed"));
				}
				_dec_size(size, deep, n);

				step = DICM_FORMAT_SIZE;
			}
			else /*VR with short size*/
			{
				us = (sword_t)vl;
				n = sizeof(short);
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)&us, n);
				}
				(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&us, &n);
				if (!n)
				{
					raise_user_error(_T("0"), _T("write dicm item length failed"));
				}
				_dec_size(size, deep, n);

				size[++deep] = 0xFFFFFFFF;

				step = DICM_FORMAT_DATA;
			}
			break;
		case DICM_FORMAT_SIZE:
			ul = vl;
			n = sizeof(long);
			if (pdcm->b_BigEndian)
			{
				bytes_turn((byte_t*)&ul, n);
			}
			(*pdcm->pf_dicm_write)(pdcm->wp, (byte_t*)&ul, &n);
			if (!n)
			{
				raise_user_error(_T("0"), _T("write dicm item length failed"));
			}
			_dec_size(size, deep, n);

			if (tag != 0xFFFEE00D && tag != 0xFFFEE0DD)
			{
				if ((vr[0] == 'S' && vr[1] == 'Q') || tag == 0xFFFEE000)
					size[++deep] = vl;
				else
					size[++deep] = 0xFFFFFFFF;
			}

			if (tag == 0xFFFEE000)
			{
				step = DICM_FORMAT_SQ_ITEM_BEGIN;
				break;
			}
			else if (tag == 0xFFFEE00D)
			{
				step = DICM_FORMAT_SQ_ITEM_END;
				break;
			}
			else if (tag == 0xFFFEE0DD)
			{
				step = DICM_FORMAT_SQ_END;
				break;
			}
			else
			{
				if (b_sq)
					step = DICM_FORMAT_SQ_BEGIN;
				else
					step = DICM_FORMAT_DATA;
			}
			break;
		case DICM_FORMAT_DATA:
			if (vl > ((pdcm->n_maxLength) ? pdcm->n_maxLength : MAX_LONG))
			{
				raise_user_error(_T("0"), _T("parse dicm breaked"));
			}

			vf = (byte_t*)xmem_alloc(vl + 2);

			if (pdcm->pf_dicm_format_data)
				rt = (*pdcm->pf_dicm_format_data)(pdcm->fp, &tag, vr, vf, &vl);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format dicm item breaked"));
			}

			if (vr[0] == 'O' && vr[1] == 'W')
			{
				if (pdcm->b_BigEndian)
				{
					for (i = 0; i < (int)(vl / 2); i += 2)
					{
						bytes_turn(vf + i, 2);
					}
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'F' && vr[1] == 'D') //8
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 8);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'S' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'S') //2
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 2);
				}
			}
			else if (vr[0] == 'U' && vr[1] == 'L') //4
			{
				if (pdcm->b_BigEndian)
				{
					bytes_turn((byte_t*)vf, 4);
				}
			}

			n = vl;
			(*pdcm->pf_dicm_write)(pdcm->wp, vf, &n);
			if (n < vl)
			{
				raise_user_error(_T("0"), _T("write dicm data failed"));
			}
			_dec_size(size, deep, n);

			xmem_free(vf);
			vf = NULL;

			deep--;

			if (!size[deep])
				step = DICM_FORMAT_SQ_ITEM_END;
			else
				step = DICM_FORMAT_TAG;
			break;
		case DICM_FORMAT_SQ_BEGIN:
			if (pdcm->pf_dicm_format_sequence_begin)
				rt = (*pdcm->pf_dicm_format_sequence_begin)(pdcm->fp, &tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format dicm sequence breaked"));
			}
			step = DICM_FORMAT_TAG;
			break;
		case DICM_FORMAT_SQ_ITEM_BEGIN:
			if (pdcm->pf_dicm_format_sequence_item_begin)
				rt = (*pdcm->pf_dicm_format_sequence_item_begin)(pdcm->fp, &tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format dicm sequence breaked"));
			}
			step = DICM_FORMAT_TAG;
			break;
		case DICM_FORMAT_SQ_ITEM_END:
			if (pdcm->pf_dicm_format_sequence_item_end)
				rt = (*pdcm->pf_dicm_format_sequence_item_end)(pdcm->fp, &tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format dicm sequence breaked"));
			}

			deep--;

			if (size[deep])
				step = DICM_FORMAT_TAG;
			else
				step = DICM_FORMAT_SQ_END;
			break;
		case DICM_FORMAT_SQ_END:
			if (pdcm->pf_dicm_format_sequence_end)
				rt = (*pdcm->pf_dicm_format_sequence_end)(pdcm->fp, &tag);
			else
				rt = 1;

			if (!rt)
			{
				raise_user_error(_T("1"), _T("format dicm sequence breaked"));
			}

			deep--;

			step = DICM_FORMAT_TAG;
			break;
		}
	}

	END_CATCH;

	return 1;

ONERROR:

	if (vf)
		xmem_free(vf);

	get_last_error(errcode, errtext, ERR_LEN);

	if (pdcm->pf_dicm_error)
	{
		(*pdcm->pf_dicm_error)(pdcm->ep, errtext);
	}

	return xstos(errcode);
}


