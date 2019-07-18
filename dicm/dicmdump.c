/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmdump.c | dicom document implement file

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

#include "dicmdump.h"
#include "dicmctx.h"
#include "dicmtag.h"

typedef struct _DUMP_PARSE_PARAM{
	union{
		dicm_context_file* ph;
		dicm_conext_command* pc;
		dicm_context_dataset* pd;
	};
	stream_t stm;
	int level;
}DUMP_PARSE_PARAM;

static bool_t _on_read(void* rp, byte_t* buf, dword_t* pb)
{
	stream_t stm = (stream_t)rp;

	return stream_read_bytes(stm, buf, pb);
}

static void _on_error(void* ep, const tchar_t* errtext)
{

}

static bool_t _on_parse_item(void* pp, dword_t tag, const byte_t* vr, dword_t vl)
{
	DUMP_PARSE_PARAM* pa = (DUMP_PARSE_PARAM*)pp;

	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	tchar_t tvr[3] = { 0 };

	sword_t l, h;
	int i;

	i = pa->level;
	while (i--)
	{
		stream_write(pa->stm, _T(" "), 1, NULL);
	}

	h = (sword_t)((tag & 0xFFFF0000) >> 16);
	l = (sword_t)(tag & 0x0000FFFF);

	xsprintf(tvx, _T("0x%04X%04X"), h, l);
	xsprintf(tvl, _T("%08X"), vl);

	if (vr[0])
		tvr[0] = vr[0];
	else
		tvr[0] = _T(' ');

	if (vr[1])
		tvr[1] = vr[1];
	else
		tvr[1] = _T(' ');

	stream_write(pa->stm, tvx, -1, NULL);

	stream_write(pa->stm, _T("\t"), 1, NULL);

	stream_write(pa->stm, tvr, -1, NULL);

	stream_write(pa->stm, _T("\t"), 1, NULL);

	stream_write(pa->stm, tvl, -1, NULL);

	stream_write(pa->stm, _T("\t"), 1, NULL);

	return 1;
}

static bool_t _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
{
	DUMP_PARSE_PARAM* pa = (DUMP_PARSE_PARAM*)pp;

	tchar_t* tval = NULL;
	dword_t n;
	bool_t b_da = 0;

	float fl;
	double fd;
	short ss;
	long sl;
	unsigned short us;
	unsigned long ul;

	if (tag == 0x7FE00010)
	{
		stream_write(pa->stm, _T("...\n"), -1, NULL);
		return 1;
	}

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
	else if ((vr[0] == 'O' && vr[1] == 'B') || (vr[0] == 'O' && vr[1] == 'W'))
	{
		n = xbas_encode(vf, vl, NULL, MAX_LONG);
		tval = xsalloc(n + 1);
		xbas_encode(vf, vl, tval, n);
		b_da = 1;
	}
	else if (vr[0] == '\0' && vr[1] == '\0')
	{
		n = xbas_encode(vf, vl, NULL, MAX_LONG);
		tval = xsalloc(n + 1);
		xbas_encode(vf, vl, tval, n);
		b_da = 1;
	}
	else if (vr[0] == 'S' && vr[1] == 'Q')
	{
		tval = NULL;
		n = 0;
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

	stream_write(pa->stm, tval, n, NULL);

	xmem_free(tval);

	stream_write(pa->stm, _T("\n"), 1, NULL);

	return 1;
}

static bool_t _on_parse_sequence_begin(void* pp, dword_t tag, dword_t vl)
{
	DUMP_PARSE_PARAM* pa = (DUMP_PARSE_PARAM*)pp;

	pa->level++;

	stream_write(pa->stm, _T("\n"), 1, NULL);

	return 1;
}

static bool_t _on_parse_sequence_item_begin(void* pp, dword_t tag, dword_t vl)
{
	DUMP_PARSE_PARAM* pa = (DUMP_PARSE_PARAM*)pp;

	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	tchar_t tvr[3] = { 0 };

	sword_t l, h;

	int i;

	pa->level++;

	i = pa->level;
	while (i--)
	{
		stream_write(pa->stm, _T(" "), 1, NULL);
	}

	h = (sword_t)((tag & 0xFFFF0000) >> 16);
	l = (sword_t)(tag & 0x0000FFFF);

	xsprintf(tvx, _T("0x%04X%04X"), h, l);
	xsprintf(tvl, _T("%08X"), vl);

	tvr[0] = _T(' ');
	tvr[1] = _T(' ');

	stream_write(pa->stm, tvx, -1, NULL);

	stream_write(pa->stm, _T("\t"), 1, NULL);

	stream_write(pa->stm, tvr, -1, NULL);

	stream_write(pa->stm, _T("\t"), 1, NULL);

	stream_write(pa->stm, tvl, -1, NULL);

	stream_write(pa->stm, _T("\n"), 1, NULL);

	return 1;
}

static bool_t _on_parse_sequence_item_end(void* pp, dword_t tag)
{
	DUMP_PARSE_PARAM* pa = (DUMP_PARSE_PARAM*)pp;

	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	tchar_t tvr[3] = { 0 };

	sword_t l, h;

	int i;

	if (tag == 0xFFFEE00D)
	{
		i = pa->level;
		while (i--)
		{
			stream_write(pa->stm, _T(" "), 1, NULL);
		}

		h = (sword_t)((tag & 0xFFFF0000) >> 16);
		l = (sword_t)(tag & 0x0000FFFF);

		xsprintf(tvx, _T("0x%04X%04X"), h, l);
		xsprintf(tvl, _T("%08X"), 0);

		tvr[0] = _T(' ');
		tvr[1] = _T(' ');

		stream_write(pa->stm, tvx, -1, NULL);

		stream_write(pa->stm, _T("\t"), 1, NULL);

		stream_write(pa->stm, tvr, -1, NULL);

		stream_write(pa->stm, _T("\t"), 1, NULL);

		stream_write(pa->stm, tvl, -1, NULL);

		stream_write(pa->stm, _T("\n"), 1, NULL);
	}

	pa->level--;

	return 1;
}

static bool_t _on_parse_sequence_end(void* pp, dword_t tag)
{
	DUMP_PARSE_PARAM* pa = (DUMP_PARSE_PARAM*)pp;

	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	tchar_t tvr[3] = { 0 };

	sword_t l, h;
	int i;

	if (tag == 0xFFFEE0DD)
	{
		i = pa->level;
		while (i--)
		{
			stream_write(pa->stm, _T(" "), 1, NULL);
		}

		h = (sword_t)((tag & 0xFFFF0000) >> 16);
		l = (sword_t)(tag & 0x0000FFFF);

		xsprintf(tvx, _T("0x%04X%04X"), h, l);
		xsprintf(tvl, _T("%08X"), 0);

		tvr[0] = _T(' ');
		tvr[1] = _T(' ');

		stream_write(pa->stm, tvx, -1, NULL);

		stream_write(pa->stm, _T("\t"), 1, NULL);

		stream_write(pa->stm, tvr, -1, NULL);

		stream_write(pa->stm, _T("\t"), 1, NULL);

		stream_write(pa->stm, tvl, -1, NULL);

		stream_write(pa->stm, _T("\n"), 1, NULL);
	}

	pa->level--;

	return 1;
}

bool_t dump_dicm(stream_t dst_stm, dword_t op, stream_t src_stm)
{
	DUMP_PARSE_PARAM pp = { 0 };
	bool_t b;

	if (op & DICM_OPERA_HEAD)
	{ 
		pp.ph = dicm_alloc_head();
		pp.stm = dst_stm;

		dicm_set_head_read(pp.ph, (void*)src_stm, _on_read);
		dicm_set_head_parse(pp.ph, (void*)&pp, NULL);

		b = dicm_parse_head(pp.ph);
		dicm_free_head(pp.ph);
	}
	else if (op & DICM_OPERA_COMMAND)
	{
		pp.pc = dicm_alloc_command();
		pp.stm = dst_stm;

		dicm_set_command_read(pp.pc, (void*)src_stm, _on_read);
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
		pp.stm = dst_stm;

		dicm_set_dataset_read(pp.pd, (void*)src_stm, _on_read);
		dicm_set_dataset_parse(pp.pd, (void*)&pp, _on_parse_item, _on_parse_data, _on_parse_sequence_begin, _on_parse_sequence_end, _on_parse_sequence_item_begin, _on_parse_sequence_item_end);

		b = dicm_parse_dataset(pp.pd);

		dicm_free_dataset(pp.pd);
	}
	else
	{
		b = 1;
	}

	return b;
}


