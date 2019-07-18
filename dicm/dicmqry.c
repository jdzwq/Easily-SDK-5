/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmqry.c | dicom document implement file

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

#include "dicmqry.h"
#include "dicmctx.h"
#include "dicmtag.h"

typedef struct _QRY_PARSE_PARAM{
	union{
		dicm_context_file* ph;
		dicm_conext_command* pc;
		dicm_context_dataset* pd;
	};
	link_t_ptr hash;
}QRY_PARSE_PARAM;

static bool_t _on_read(void* rp, byte_t* buf, dword_t* pb)
{
	stream_t stm = (stream_t)rp;

	return stream_read_bytes(stm, buf, pb);
}

static void _on_error(void* ep, const tchar_t* errtext)
{

}

static bool_t _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
{
	QRY_PARSE_PARAM* pa = (QRY_PARSE_PARAM*)pp;
	tchar_t* tval = NULL;
	dword_t n;
	const dicm_item_t* pdi;
	link_t_ptr ent;
	bool_t rt;

	float fl;
	double fd;
	short ss;
	long sl;
	unsigned short us;
	unsigned long ul;

	pdi = dicm_item_info(tag);
	if (!pdi)
		return 0;

	ent = get_hash_entity(pa->hash, pdi->vn, -1);
	if (!ent)
		return 0;

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
		if (DICM_IS_SENS_VR(vr) && pa->pd->n_CharSet == DICM_CHARSET_UTF)
		{
			n = utf8_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			utf8_to_ucs(vf, vl, tval, n);
		}
		else
		{
			n = mbs_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			mbs_to_ucs(vf, vl, tval, n);
		}
	}

	rt = (NULL == xsstr(get_hash_entity_val_ptr(ent), tval)) ? 1 : 0;

	xmem_free(tval);

	return rt;
}

bool_t query_dicm(const tchar_t* qry, dword_t op, stream_t stm)
{
	QRY_PARSE_PARAM pp = { 0 };
	bool_t b;

	if (op & DICM_OPERA_HEAD)
	{
		pp.ph = dicm_alloc_head();
		pp.hash = NULL;

		dicm_set_head_read(pp.ph, (void*)stm, _on_read);
		dicm_set_head_parse(pp.ph, (void*)&pp, NULL);

		b = dicm_parse_head(pp.ph);

		dicm_free_head(pp.ph);
	}
	else if (op & DICM_OPERA_COMMAND)
	{
		pp.pc = dicm_alloc_command();
		pp.hash = NULL;

		dicm_set_command_read(pp.pc, (void*)stm, _on_read);
		dicm_set_command_parse(pp.pc, (void*)&pp, NULL, NULL);

		b = dicm_parse_command(pp.pc);

		dicm_free_command(pp.pc);
	}
	else
	{
		b = 1;
	}

	if (!b)
		return 0;

	if (op & DICM_OPERA_DATASET)
	{
		pp.pd = dicm_alloc_dataset();
		pp.hash = create_hash_table();

		dicm_set_dataset_read(pp.pd, (void*)stm, _on_read);
		dicm_set_dataset_parse(pp.pd, (void*)&pp, NULL, _on_parse_data, NULL, NULL, NULL, NULL);

		b = dicm_parse_dataset(pp.pd);

		destroy_hash_table(pp.hash);
		dicm_free_dataset(pp.pd);
	}
	else
	{
		b = 1;
	}

	return b;
}
