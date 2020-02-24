/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom stream document

	@module	dcmstm.c | dicom stmsole implement file

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

#include "dcmstm.h"
#include "dcmraw.h"
#include "dcmbmp.h"

typedef struct _CON_PARSE_PARAM{
	dicm_context_t* pdcm;
	xhand_t stm;
	int level;
}CON_PARSE_PARAM;

static bool_t __callback _on_read(void* rp, byte_t* buf, dword_t* pb)
{
	XHANDLE xf = (XHANDLE)rp;

	return xfile_read(xf, buf, pb);
}

static void __callback _on_error(void* ep, const tchar_t* errtext)
{

}

static int __callback _on_parse_item(void* pp, dword_t tag, const byte_t* vr, dword_t vl)
{
	CON_PARSE_PARAM* pa = (CON_PARSE_PARAM*)pp;

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

	return 0;
}

static int __callback _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
{
	CON_PARSE_PARAM* pa = (CON_PARSE_PARAM*)pp;

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
		return 0;
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
		n = base64_encode(vf, vl, NULL, MAX_LONG);
		tval = xsalloc(n + 1);
		base64_encode(vf, vl, tval, n);
		b_da = 1;
	}
	else if (vr[0] == '\0' && vr[1] == '\0')
	{
		n = base64_encode(vf, vl, NULL, MAX_LONG);
		tval = xsalloc(n + 1);
		base64_encode(vf, vl, tval, n);
		b_da = 1;
	}
	else if (vr[0] == 'S' && vr[1] == 'Q')
	{
		tval = NULL;
		n = 0;
	}
	else
	{
		if (IS_SENS_VR(vr) && pa->pdcm->n_CharSet == DICM_CHARSET_UTF)
		{
			n = utf8_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			utf8_to_ucs(vf, vl, tval, n);
		}
		else
		{
			n = gb2312_to_ucs(vf, vl, NULL, MAX_LONG);
			tval = xsalloc(n + 1);
			gb2312_to_ucs(vf, vl, tval, n);
		}
	}

	stream_write(pa->stm, tval, n, NULL);

	xmem_free(tval);

	stream_write(pa->stm, _T("\n"), 1, NULL);

	return 0;
}

static int __callback _on_parse_sequence_begin(void* pp, dword_t tag, dword_t vl)
{
	CON_PARSE_PARAM* pa = (CON_PARSE_PARAM*)pp;

	pa->level++;

	stream_write(pa->stm, _T("\n"), 1, NULL);

	return 0;
}

static int __callback _on_parse_sequence_item_begin(void* pp, dword_t tag, dword_t vl)
{
	CON_PARSE_PARAM* pa = (CON_PARSE_PARAM*)pp;

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

	return 0;
}

static int __callback _on_parse_sequence_item_end(void* pp, dword_t tag)
{
	CON_PARSE_PARAM* pa = (CON_PARSE_PARAM*)pp;

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

	return 0;
}

static int __callback _on_parse_sequence_end(void* pp, dword_t tag)
{
	CON_PARSE_PARAM* pa = (CON_PARSE_PARAM*)pp;

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

	return 0;
}

bool_t dicm_to_stream(const secu_desc_t* psd, const tchar_t* fname, xhand_t stm)
{
	XHANDLE xf = NULL;
	CON_PARSE_PARAM pp = { 0 };
	dicm_context_t* pdcm = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, 0);
	if (!xf)
	{
		raise_user_error(_T("dicm_to_stream"), _T("open dicm file failed"));
	}
	
	pdcm = dicm_alloc();

	pp.pdcm = pdcm;
	pp.stm = stm;

	dicm_set_read(pdcm, (void*)xf, _on_read);
	dicm_set_parse(pdcm, (void*)&pp, _on_parse_item, _on_parse_data, _on_parse_sequence_begin, _on_parse_sequence_end, _on_parse_sequence_item_begin, _on_parse_sequence_item_end);

	dicm_parse(pdcm);

	dicm_free(pdcm);
	pdcm = NULL;

	xfile_close(xf);
	xf = NULL;

	END_CATCH;

	return 1;

ONERROR:

	if (xf)
		xfile_close(xf);

	if (pdcm)
		dicm_free(pdcm);

	return 0;
}

