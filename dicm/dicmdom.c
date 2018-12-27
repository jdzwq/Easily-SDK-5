/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmdom.c | dicom document implement file

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

#include "dicmdom.h"
#include "dicmctx.h"
#include "dicmdoc.h"
#include "dicmtag.h"

typedef struct _DOM_PARSE_PARAM{
	union{
		dicm_context_file* ph;
		dicm_conext_command* pc;
		dicm_context_dataset* pd;
	};
	link_t_ptr dom;
	link_t_ptr plk;
	link_t_ptr nlk;
}DOM_PARSE_PARAM;

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
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t tvg[KEY_LEN] = { 0 };
	tchar_t tvn[KEY_LEN] = { 0 };
	tchar_t tvt[NUM_LEN] = { 0 };
	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	tchar_t tvr[3] = { 0 };

	sword_t l, h;
	const dicm_item_t* pitem;
	const dicm_group_t* pgroup;

	h = (sword_t)((tag & 0xFFFF0000) >> 16);
	l = (sword_t)(tag & 0x0000FFFF);

	pgroup = dicm_group_info(tag);
	if (pgroup)
		xscpy(tvg, pgroup->gn);
	else
		xsprintf(tvg, _T("Unknown%04X"), h);

	pitem = dicm_item_info(tag);
	if (pitem)
		xscpy(tvn, pitem->vn);
	else
		xsprintf(tvn, _T("Unknown%04X%04X"), h, l);

	xsprintf(tvx, _T("0x%04X%04X"), h, l);
	xsprintf(tvt, _T("(%04X,%04X)"), h, l);
	xsprintf(tvl, _T("%d"), vl);

	tvr[0] = (tchar_t)vr[0];
	tvr[1] = (tchar_t)vr[1];

	pa->nlk = insert_dom_node(pa->plk, LINK_LAST);
	set_dom_node_name(pa->nlk, tvn, -1);
	set_dom_node_attr(pa->nlk, _T("VR"), 2, tvr, 2);
	set_dom_node_attr(pa->nlk, _T("VL"), 2, tvl, -1);
	set_dom_node_attr(pa->nlk, _T("VX"), 2, tvx, -1);
	set_dom_node_attr(pa->nlk, _T("VT"), 2, tvt, -1);
	set_dom_node_attr(pa->nlk, _T("VG"), 2, tvg, -1);

	return 1;
}

static bool_t _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t* tval = NULL;
	dword_t n;
	bool_t b_da = 0;

	float fl;
	double fd;
	short ss;
	long sl;
	unsigned short us;
	unsigned long ul;

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

	set_dom_node_cdata(pa->nlk, b_da);
	set_dom_node_text(pa->nlk, tval, n);

	xmem_free(tval);

	return 1;
}

static bool_t _on_parse_sequence_begin(void* pp, dword_t tag, dword_t vl)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	pa->plk = pa->nlk;
	pa->nlk = NULL;

	return 1;
}

static bool_t _on_parse_sequence_item_begin(void* pp, dword_t tag, dword_t vl)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t tvn[KEY_LEN] = { 0 };
	tchar_t tvt[NUM_LEN] = { 0 };
	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	tchar_t tvr[3] = { 0 };

	sword_t l, h;
	const dicm_item_t* pitem;

	h = (sword_t)((tag & 0xFFFF0000) >> 16);
	l = (sword_t)(tag & 0x0000FFFF);

	pitem = dicm_item_info(tag);
	if (pitem)
		xscpy(tvn, pitem->vn);
	else
		xsprintf(tvn, _T("Unknown%04X%04X"), h, l);

	xsprintf(tvx, _T("0x%04X%04X"), h, l);
	xsprintf(tvt, _T("(%04X,%04X)"), h, l);
	xsprintf(tvl, _T("%d"), vl);

	pa->plk = insert_dom_node(pa->plk, LINK_LAST);
	pa->nlk = NULL;

	set_dom_node_name(pa->plk, tvn, -1);
	set_dom_node_attr(pa->plk, _T("VR"), 2, tvr, 2);
	set_dom_node_attr(pa->plk, _T("VL"), 2, tvl, -1);
	set_dom_node_attr(pa->plk, _T("VX"), 2, tvx, -1);
	set_dom_node_attr(pa->plk, _T("VT"), 2, tvt, -1);

	return 1;
}

static bool_t _on_parse_sequence_item_end(void* pp, dword_t tag)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t tvn[KEY_LEN] = { 0 };
	tchar_t tvt[NUM_LEN] = { 0 };
	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	tchar_t tvr[3] = { 0 };

	sword_t l, h;
	const dicm_item_t* pitem;

	if (tag == 0xFFFEE00D)
	{
		h = (sword_t)((tag & 0xFFFF0000) >> 16);
		l = (sword_t)(tag & 0x0000FFFF);

		pitem = dicm_item_info(tag);
		if (pitem)
			xscpy(tvn, pitem->vn);
		else
			xsprintf(tvn, _T("Unknown%04X%04X"), h, l);

		xsprintf(tvx, _T("0x%04X%04X"), h, l);
		xsprintf(tvt, _T("(%04X,%04X)"), h, l);
		xsprintf(tvl, _T("%d"), 0);

		pa->nlk = insert_dom_node(pa->plk, LINK_LAST);
		set_dom_node_name(pa->nlk, tvn, -1);
		set_dom_node_attr(pa->nlk, _T("VR"), 2, tvr, 2);
		set_dom_node_attr(pa->nlk, _T("VL"), 2, tvl, -1);
		set_dom_node_attr(pa->nlk, _T("VX"), 2, tvx, -1);
		set_dom_node_attr(pa->nlk, _T("VT"), 2, tvt, -1);
	}

	pa->plk = get_dom_parent_node(pa->plk);
	pa->nlk = NULL;

	return 1;
}

static bool_t _on_parse_sequence_end(void* pp, dword_t tag)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t tvn[KEY_LEN] = { 0 };
	tchar_t tvt[NUM_LEN] = { 0 };
	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	tchar_t tvr[3] = { 0 };

	sword_t l, h;
	const dicm_item_t* pitem;

	if (tag == 0xFFFEE0DD)
	{
		h = (sword_t)((tag & 0xFFFF0000) >> 16);
		l = (sword_t)(tag & 0x0000FFFF);

		pitem = dicm_item_info(tag);
		if (pitem)
			xscpy(tvn, pitem->vn);
		else
			xsprintf(tvn, _T("Unknown%04X%04X"), h, l);

		xsprintf(tvx, _T("0x%04X%04X"), h, l);
		xsprintf(tvt, _T("(%04X,%04X)"), h, l);
		xsprintf(tvl, _T("%d"), 0);

		pa->nlk = insert_dom_node(pa->plk, LINK_LAST);
		set_dom_node_name(pa->nlk, tvn, -1);
		set_dom_node_attr(pa->nlk, _T("VR"), 2, tvr, 2);
		set_dom_node_attr(pa->nlk, _T("VL"), 2, tvl, -1);
		set_dom_node_attr(pa->nlk, _T("VX"), 2, tvx, -1);
		set_dom_node_attr(pa->nlk, _T("VT"), 2, tvt, -1);
	}

	pa->plk = get_dom_parent_node(pa->plk);
	pa->nlk = NULL;

	return 1;
}

bool_t parse_dicm_doc_from_stream(link_t_ptr ptr, dword_t op, stream_t stm)
{
	DOM_PARSE_PARAM pp = { 0 };
	bool_t b;

	if (op & DICM_OPERA_HEAD)
	{
		pp.ph = dicm_alloc_head();
		pp.dom = ptr;
		pp.plk = ptr;
		pp.nlk = NULL;

		dicm_set_head_read(pp.ph, (void*)stm, _on_read);
		dicm_set_head_parse(pp.ph, (void*)&pp, NULL);

		b = dicm_parse_head(pp.ph);

		dicm_free_head(pp.ph);
	}
	else if (op & DICM_OPERA_COMMAND)
	{
		pp.pc = dicm_alloc_command();
		pp.dom = ptr;
		pp.plk = ptr;
		pp.nlk = NULL;

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

	if (b & DICM_OPERA_DATASET)
	{
		pp.pd = dicm_alloc_dataset();
		pp.dom = ptr;
		pp.plk = ptr;
		pp.nlk = NULL;

		dicm_set_dataset_read(pp.pd, (void*)stm, _on_read);
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

typedef struct _DOM_FORMAT_PARAM{
	union{
		dicm_context_file* ph;
		dicm_conext_command* pc;
		dicm_context_dataset* pd;
	};
	link_t_ptr dom;
	link_t_ptr plk;
	link_t_ptr nlk;
}DOM_FORMAT_PARAM;

static bool_t _on_write(void* fp, byte_t* buf, dword_t* pb)
{
	stream_t stm = (stream_t)fp;

	return stream_write_bytes(stm, buf, *pb);
}

static bool_t _on_format_item(void* fp, dword_t* ptag, byte_t* pvr, dword_t* pvl)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;
	tchar_t vr[3] = { 0 };

	if (!pa->nlk)
		pa->nlk = get_dom_first_child_node(pa->plk);
	else
		pa->nlk = get_dom_next_sibling_node(pa->nlk);

	if (!pa->nlk)
		return 0;

	*ptag = hextol(get_dom_node_attr_ptr(pa->nlk, _T("VX"), -1));
	get_dom_node_attr(pa->nlk, _T("VR"), -1, vr, 2);
	pvr[0] = (byte_t)vr[0];
	pvr[1] = (byte_t)vr[1];
	*pvl = xstol(get_dom_node_attr_ptr(pa->nlk, _T("VL"), -1));

	if (!(*pvl) && !get_dom_first_child_node(pa->nlk))
	{
		if (pvr[0] == 'F' && pvr[1] == 'L') //4
		{
			*pvl = sizeof(float);
		}
		else if (pvr[0] == 'F' && pvr[1] == 'D') //8
		{
			*pvl = sizeof(double);
		}
		else if (pvr[0] == 'S' && pvr[1] == 'S') //2
		{
			*pvl = sizeof(short);
		}
		else if (pvr[0] == 'S' && pvr[1] == 'L') //4
		{
			*pvl = sizeof(long);
		}
		else if (pvr[0] == 'U' && pvr[1] == 'S') //2
		{
			*pvl = sizeof(unsigned short);
		}
		else if (pvr[0] == 'U' && pvr[1] == 'L') //4
		{
			*pvl = sizeof(unsigned long);
		}
		else if ((pvr[0] == 'O' && pvr[1] == 'B') || (pvr[0] == 'O' && pvr[1] == 'W'))
		{
			*pvl = xbas_decode(get_dom_node_text_ptr(pa->nlk), -1, NULL, MAX_LONG);
		}
		else if (pvr[0] == '\0' && pvr[1] == '\0')
		{
			*pvl = xbas_decode(get_dom_node_text_ptr(pa->nlk), -1, NULL, MAX_LONG);
		}
		else
		{
			if (get_dicm_charset(pa->dom) == DICM_CHARSET_UTF)
				*pvl = ucs_to_utf8(get_dom_node_text_ptr(pa->nlk), -1, NULL, MAX_LONG);
			else
				*pvl = ucs_to_mbs(get_dom_node_text_ptr(pa->nlk), -1, NULL, MAX_LONG);
		}
	}

	return 1;
}

static bool_t _on_format_data(void* fp, dword_t* ptag, byte_t* pvr, byte_t* pvf, dword_t* pvl)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;
	const tchar_t* tval;
	float fl;
	double fd;
	short ss;
	long sl;
	unsigned short us;
	unsigned long ul;

	if (!pa->nlk)
		return 0;

	if (*pvl)
	{
		tval = get_dom_node_text_ptr(pa->nlk);

		if (pvr[0] == 'F' && pvr[1] == 'L') //4
		{
			fl = xstof(tval);
			memcpy((void*)pvf, (void*)&fl, sizeof(float));
		}
		else if (pvr[0] == 'F' && pvr[1] == 'D') //8
		{
			fd = xstonum(tval);
			memcpy((void*)pvf, (void*)&fd, sizeof(double));
		}
		else if (pvr[0] == 'S' && pvr[1] == 'S') //2
		{
			ss = xstos(tval);
			memcpy((void*)pvf, (void*)&ss, sizeof(short));
		}
		else if (pvr[0] == 'S' && pvr[1] == 'L') //4
		{
			sl = xstol(tval);
			memcpy((void*)pvf, (void*)&sl, sizeof(long));
		}
		else if (pvr[0] == 'U' && pvr[1] == 'S') //2
		{
			us = xstous(tval);
			memcpy((void*)pvf, (void*)&us, sizeof(unsigned short));
		}
		else if (pvr[0] == 'U' && pvr[1] == 'L') //4
		{
			ul = xstoul(tval);
			memcpy((void*)pvf, (void*)&ul, sizeof(unsigned long));
		}
		else if ((pvr[0] == 'O' && pvr[1] == 'B') || (pvr[0] == 'O' && pvr[1] == 'W'))
		{
			xbas_decode(tval, -1, pvf, *pvl);
		}
		else if (pvr[0] == '\0' && pvr[1] == '\0')
		{
			xbas_decode(tval, -1, pvf, *pvl);
		}
		else if (pvr[0] == 'S' && pvr[1] == 'Q')
		{
			if (pvf)
				*pvf = 0;
		}
		else
		{
			if (DICM_IS_SENS_VR(pvr) && pa->pd->n_CharSet == DICM_CHARSET_UTF)
			{
				ucs_to_utf8(tval, -1, pvf, *pvl);
			}
			else
			{
				ucs_to_mbs(tval, -1, pvf, *pvl);
			}
		}
	}

	return 1;
}

static bool_t _on_format_sequence_begin(void* fp, dword_t* ptag)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;

	pa->plk = pa->nlk;
	pa->nlk = NULL;

	return 1;
}

static bool_t _on_format_sequence_item_begin(void* fp, dword_t* ptag)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;

	pa->plk = pa->nlk;
	pa->nlk = NULL;

	return 1;
}

static bool_t _on_format_sequence_item_end(void* fp, dword_t* ptag)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;

	pa->nlk = pa->plk;
	pa->plk = get_dom_parent_node(pa->plk);

	return 1;
}

static bool_t _on_format_sequence_end(void* fp, dword_t* ptag)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;

	pa->nlk = pa->plk;
	pa->plk = get_dom_parent_node(pa->plk);

	return 1;
}

bool_t format_dicm_doc_to_stream(link_t_ptr ptr, dword_t op, stream_t stm)
{
	DOM_FORMAT_PARAM fp = { 0 };
	bool_t b;

	if (op & DICM_OPERA_HEAD)
	{
		fp.ph = dicm_alloc_head();
		fp.dom = ptr;
		fp.plk = ptr;
		fp.nlk = NULL;

		dicm_set_head_write(fp.ph, (void*)stm, _on_write);
		dicm_set_head_format(fp.ph, (void*)&fp, NULL);

		b = dicm_format_head(fp.ph);

		dicm_free_head(fp.ph);
	}
	else if (op & DICM_OPERA_COMMAND)
	{
		fp.pc = dicm_alloc_command();
		fp.dom = ptr;
		fp.plk = ptr;
		fp.nlk = NULL;

		dicm_set_command_write(fp.pc, (void*)stm, _on_write);
		dicm_set_command_format(fp.pc, (void*)&fp, NULL, NULL);

		b = dicm_format_command(fp.pc);

		dicm_free_command(fp.pc);
	}
	else
	{
		b = 1;
	}

	if (!b)
		return 0;

	if (op & DICM_OPERA_DATASET)
	{
		fp.pd = dicm_alloc_dataset();
		fp.dom = ptr;
		fp.plk = ptr;
		fp.nlk = NULL;

		dicm_set_dataset_write(fp.pd, (void*)stm, _on_write);
		dicm_set_dataset_format(fp.pd, (void*)&fp, _on_format_item, _on_format_data, _on_format_sequence_begin, _on_format_sequence_end, _on_format_sequence_item_begin, _on_format_sequence_item_end);

		b = dicm_format_dataset(fp.pd);

		dicm_free_dataset(fp.pd);
	}
	else
	{
		b = 1;
	}
	
	return b;
}

