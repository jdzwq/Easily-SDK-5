/***********************************************************************
	Easily dcm

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dcmdom.c | dicom dom implement file

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

#include "dcmdom.h"
#include "dcmraw.h"
#include "dcmbmp.h"

typedef struct _DOM_PARSE_PARAM{
	dicm_context_t* pdcm;
	LINKPTR dom;
	LINKPTR plk;
	LINKPTR nlk;
}DOM_PARSE_PARAM;

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
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t tvg[NAME_LEN] = { 0 };
	tchar_t tvn[NAME_LEN] = { 0 };
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

	return 0;
}

static int __callback _on_parse_data(void* pp, dword_t tag, const byte_t* vr, const byte_t* vf, dword_t vl)
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

	set_dom_node_cdata(pa->nlk, b_da);
	set_dom_node_text(pa->nlk, tval, n);

	xmem_free(tval);

	return 0;
}

static int __callback _on_parse_sequence_begin(void* pp, dword_t tag, dword_t vl)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	pa->plk = pa->nlk;
	pa->nlk = NULL;

	return 0;
}

static int __callback _on_parse_sequence_item_begin(void* pp, dword_t tag, dword_t vl)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t tvn[NAME_LEN] = { 0 };
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

	return 0;
}

static int __callback _on_parse_sequence_item_end(void* pp, dword_t tag)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t tvn[NAME_LEN] = { 0 };
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

	return 0;
}

static int __callback _on_parse_sequence_end(void* pp, dword_t tag)
{
	DOM_PARSE_PARAM* pa = (DOM_PARSE_PARAM*)pp;

	tchar_t tvn[NAME_LEN] = { 0 };
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

	return 0;
}

bool_t dicm_to_dom(const secu_desc_t* psd, const tchar_t* fname, LINKPTR dom)
{
	XHANDLE xf = NULL;
	DOM_PARSE_PARAM pp = { 0 };
	dicm_context_t* pdcm = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, 0);
	if (!xf)
	{
		raise_user_error(_T("dicm_file_to_dom"), _T("open dicm file failed"));
	}
	
	set_dom_node_name(dom, _T("DICM"), -1);

	pdcm = dicm_alloc();

	pp.pdcm = pdcm;
	pp.dom = dom;
	pp.plk = dom;
	pp.nlk = NULL;

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

typedef struct _DOM_FORMAT_PARAM{
	dicm_context_t* pdcm;
	LINKPTR dom;
	LINKPTR plk;
	LINKPTR nlk;
}DOM_FORMAT_PARAM;

static bool_t __callback _on_write(void* fp, byte_t* buf, dword_t* pb)
{
	XHANDLE xf = (XHANDLE)fp;

	return xfile_write(xf, buf, pb);
}

static int __callback _on_format_item(void* fp, dword_t* ptag, byte_t* pvr, dword_t* pvl)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;
	tchar_t vr[3] = { 0 };

	if (!pa->nlk)
		pa->nlk = get_dom_first_child_node(pa->plk);
	else
		pa->nlk = get_dom_next_sibling_node(pa->nlk);

	if (!pa->nlk)
		return DICM_BREAK;

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
			*pvl = base64_decode(get_dom_node_text_ptr(pa->nlk), -1, NULL, MAX_LONG);
		}
		else if (pvr[0] == '\0' && pvr[1] == '\0')
		{
			*pvl = base64_decode(get_dom_node_text_ptr(pa->nlk), -1, NULL, MAX_LONG);
		}
	}

	return DICM_CONTINUE;
}

static int __callback _on_format_data(void* fp, dword_t* ptag, byte_t* pvr, byte_t* pvf, dword_t* pvl)
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
		return DICM_BREAK;

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
			base64_decode(tval, -1, pvf, *pvl);
		}
		else if (pvr[0] == '\0' && pvr[1] == '\0')
		{
			base64_decode(tval, -1, pvf, *pvl);
		}
		else if (pvr[0] == 'S' && pvr[1] == 'Q')
		{
			if (pvf)
				*pvf = 0;
		}
		else
		{
			if (IS_SENS_VR(pvr) && pa->pdcm->n_CharSet == DICM_CHARSET_UTF)
			{
				ucs_to_utf8(tval, -1, pvf, *pvl);
			}
			else
			{
				ucs_to_gb2312(tval, -1, pvf, *pvl);
			}
		}
	}

	return DICM_CONTINUE;
}

static int __callback _on_format_sequence_begin(void* fp, dword_t* ptag)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;

	pa->plk = pa->nlk;
	pa->nlk = NULL;

	return DICM_CONTINUE;
}

static int __callback _on_format_sequence_item_begin(void* fp, dword_t* ptag)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;

	pa->plk = pa->nlk;
	pa->nlk = NULL;

	return DICM_CONTINUE;
}

static int __callback _on_format_sequence_item_end(void* fp, dword_t* ptag)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;

	pa->nlk = pa->plk;
	pa->plk = get_dom_parent_node(pa->plk);

	return DICM_CONTINUE;
}

static int __callback _on_format_sequence_end(void* fp, dword_t* ptag)
{
	DOM_FORMAT_PARAM* pa = (DOM_FORMAT_PARAM*)fp;

	pa->nlk = pa->plk;
	pa->plk = get_dom_parent_node(pa->plk);

	return DICM_CONTINUE;
}

bool_t dicm_from_dom(const secu_desc_t* psd, const tchar_t* fname, LINKPTR dom)
{
	XHANDLE xf = NULL;
	DOM_FORMAT_PARAM fp = { 0 };
	dicm_context_t* pdcm = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);
	if (!xf)
	{
		raise_user_error(_T("dicm_dom_to_file"), _T("save dicm file failed"));
	}

	pdcm = dicm_alloc();

	fp.pdcm = pdcm;
	fp.dom = dom;
	fp.plk = dom;
	fp.nlk = NULL;

	dicm_set_write(pdcm, (void*)xf, _on_write);
	dicm_set_format(pdcm, (void*)&fp, _on_format_item, _on_format_data, _on_format_sequence_begin, _on_format_sequence_end, _on_format_sequence_item_begin, _on_format_sequence_item_end);

	dicm_format(pdcm);

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

int dicm_dom_charset(LINKPTR dom)
{
	LINKPTR plk;
	int chs = DICM_CHARSET_ENG;

	plk = find_dom_node_by_name(dom, 0, _T("SpecificCharacterSet"), -1);
	if (plk)
	{
		if (compare_text(get_dom_node_text_ptr(plk), -1, _T("ISO_IR 192"), -1, 1) == 0)
			chs = DICM_CHARSET_UTF;
		else if (compare_text(get_dom_node_text_ptr(plk), -1, _T("GB18030"), -1, 1) == 0)
			chs = DICM_CHARSET_GBK;
		else
			chs = DICM_CHARSET_ENG;
	}

	return chs;
}

LINKPTR dicm_dom_insert_node_by_tag(LINKPTR plk, const tchar_t* tag)
{
	LINKPTR nlk;

	nlk = get_dom_first_child_node(plk);
	while (nlk)
	{
		if (compare_text(get_dom_node_attr_ptr(nlk, _T("VX"), -1), -1, tag, -1, 0) == 0)
			return nlk;
		else if (compare_text(get_dom_node_attr_ptr(nlk, _T("VX"), -1), -1, tag, -1, 0) > 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	if (!nlk)
	{
		nlk = insert_dom_node(plk, LINK_LAST);
	}
	else
	{
		nlk = insert_dom_node_before(plk, nlk);
	}

	set_dom_node_attr(nlk, _T("VX"), -1, tag, -1);

	return nlk;
}

void dicm_dom_clear_annotation(LINKPTR dom)
{
	LINKPTR plk;

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (plk)
	{
		delete_dom_node(plk);
	}
}

int dicm_dom_load_text_annotation(LINKPTR dom, dicm_text_anno_t* pta, int max)
{
	LINKPTR plk, nlk, glk;
	int i = 0;

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (!plk)
		return 0;

	nlk = find_dom_node_by_name(plk, 1, _T("TextObjectSequence"), -1);
	if (!nlk)
		return 0;

	glk = get_dom_first_child_node(nlk);
	while (glk && i < max)
	{
		nlk = get_dom_first_child_node(glk);
		while (nlk)
		{
			if (pta)
			{
				if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("UnformattedTextValue"), -1, 1) == 0)
					xscpy(pta[i].text, get_dom_node_text_ptr(nlk));
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BoundingBoxTopLeftHandCorner"), -1, 1) == 0)
					dicm_parse_point(get_dom_node_text_ptr(nlk), -1, pta[i].points + 1, 1);
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BoundingBoxBottomRightHandCorner"), -1, 1) == 0)
					dicm_parse_point(get_dom_node_text_ptr(nlk), -1, pta[i].points + 2, 1);
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("AnchorPoint"), -1, 1) == 0)
					dicm_parse_point(get_dom_node_text_ptr(nlk), -1, pta[i].points, 1);
			}

			nlk = get_dom_next_sibling_node(nlk);
		}

		if (get_dom_child_node_count(glk))
			i++;

		glk = get_dom_next_sibling_node(glk);
	}

	return i;
}

void dicm_dom_save_text_annotation(LINKPTR dom, const dicm_text_anno_t* pta, int count)
{
	LINKPTR plk, nlk, glk, ilk;
	int bytes;
	int i, chs;
	tchar_t token[1024];

	chs = dicm_dom_charset(dom);

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (!plk)
	{
		plk = dicm_dom_insert_node_by_tag(dom, _T("0x00700001"));
		set_dom_node_name(plk, _T("GraphicAnnotationSequence"), -1);
		set_dom_node_attr(plk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(plk, _T("VX"), -1, _T("0x00700001"), -1);
		set_dom_node_attr(plk, _T("VT"), -1, _T("(0070,0001)"), -1);
		set_dom_node_attr(plk, _T("VR"), -1, _T("SQ"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(plk, _T("VL"), -1, token, -1);
	}
	else
	{
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(plk, _T("VL"), -1, token, -1);
	}

	glk = find_dom_node_by_name(plk, 1, _T("TextObjectSequence"), -1);
	if (!glk)
	{
		nlk = find_dom_node_by_name(plk, 0, _T("StartOfItem"), -1);
		if (!nlk)
		{
			nlk = insert_dom_node(plk, LINK_FIRST);
			set_dom_node_name(nlk, _T("StartOfItem"), -1);
			set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE000"), -1);
			set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E000)"), -1);
			xsprintf(token, _T("%d"), 0xFFFFFFFF);
			set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
		}
		else
		{
			xsprintf(token, _T("%d"), 0xFFFFFFFF);
			set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
		}

		glk = dicm_dom_insert_node_by_tag(nlk, _T("0x00700008"));
		set_dom_node_name(glk, _T("TextObjectSequence"), -1);
		set_dom_node_attr(glk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(glk, _T("VX"), -1, _T("0x00700008"), -1);
		set_dom_node_attr(glk, _T("VT"), -1, _T("(0070,0008)"), -1);
		set_dom_node_attr(glk, _T("VR"), -1, _T("SQ"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(glk, _T("VL"), -1, token, -1);
	}
	else
	{
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(glk, _T("VL"), -1, token, -1);

		//StartOfItem
		nlk = get_dom_parent_node(glk);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
	}

	ilk = find_dom_node_by_name(nlk, 0, _T("EndOfItems"), -1);
	if (!ilk)
	{
		ilk = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(ilk, _T("EndOfItems"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE00D"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E00D)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);
	}

	delete_dom_child_nodes(glk);

	for (i = 0; i < count; i++)
	{
		ilk = insert_dom_node(glk, LINK_LAST);
		set_dom_node_name(ilk, _T("StartOfItem"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE000"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E000)"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("BoundingBoxAnnotationUnits"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700003"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0003)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("PIXEL"), -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(_T("PIXEL"), -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(_T("PIXEL"), -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("AnchorPointAnnotationUnits"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700004"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0004)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("PIXEL"), -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(_T("PIXEL"), -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(_T("PIXEL"), -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("UnformattedTextValue"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700006"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0006)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("LO"), -1);
		set_dom_node_text(nlk, pta[i].text, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(pta[i].text, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(pta[i].text, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("BoundingBoxTopLeftHandCorner"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700010"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0010)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("DS"), -1);
		dicm_format_point(pta[i].points + 1, 1, token, 1024);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("BoundingBoxBottomRightHandCorner"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700011"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0011)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("DS"), -1);
		dicm_format_point(pta[i].points + 2, 1, token, 1024);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("BoundingBoxTextHorizJustification"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700012"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0012)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, pta[i].align, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(pta[i].align, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(pta[i].align, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("AnchorPoint"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700014"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0014)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("DS"), -1);
		dicm_format_point(pta[i].points, 1, token, 1024);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("AnchorPointVisibility"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700015"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0015)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("N"), -1);
		set_dom_node_attr(nlk, _T("VL"), -1, _T("1"), -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("EndOfItems"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE00D"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E00D)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
	}

	nlk = insert_dom_node(glk, LINK_LAST);
	set_dom_node_name(nlk, _T("EndOfSequence"), -1);
	set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE0DD"), -1);
	set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E0DD)"), -1);
	xsprintf(token, _T("%d"), 0);
	set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

	ilk = find_dom_node_by_name(plk, 0, _T("EndOfSequence"), -1);
	if (!ilk)
	{
		ilk = insert_dom_node(plk, LINK_LAST);
		set_dom_node_name(ilk, _T("EndOfSequence"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE0DD"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E0DD)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);
	}
}

int dicm_dom_load_graphic_annotation(LINKPTR dom, dicm_graphic_anno_t* pga, int max)
{
	LINKPTR plk, nlk, glk;
	int i = 0;

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (!plk)
		return 0;

	nlk = find_dom_node_by_name(plk, 1, _T("GraphicObjectSequence"), -1);
	if (!nlk)
		return 0;

	glk = get_dom_first_child_node(nlk);
	while (glk && i < max)
	{
		nlk = get_dom_first_child_node(glk);
		while (nlk)
		{
			if (pga)
			{
				if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("NumberOfGraphicPoints"), -1, 1) == 0)
					pga[i].count = xstol(get_dom_node_text_ptr(nlk));
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("GraphicData"), -1, 1) == 0)
					pga[i].count = dicm_parse_point(get_dom_node_text_ptr(nlk), -1, pga[i].points, DICM_MAX_POINTS);
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("GraphicType"), -1, 1) == 0)
					xscpy(pga[i].type, get_dom_node_text_ptr(nlk));
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("GraphicFilled"), -1, 1) == 0)
					pga[i].fill = (compare_text(get_dom_node_text_ptr(nlk), -1, _T("Y"), 1, 1) == 0) ? 1 : 0;
			}

			nlk = get_dom_next_sibling_node(nlk);
		}

		if (get_dom_child_node_count(glk))
			i++;

		glk = get_dom_next_sibling_node(glk);
	}

	return i;
}

void dicm_dom_save_graphic_annotation(LINKPTR dom, const dicm_graphic_anno_t* pga, int count)
{
	LINKPTR plk, nlk, glk, ilk;
	int bytes;
	int i, chs;
	tchar_t token[1024];

	chs = dicm_dom_charset(dom);

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (!plk)
	{
		plk = dicm_dom_insert_node_by_tag(dom, _T("0x00700001"));
		set_dom_node_name(plk, _T("GraphicAnnotationSequence"), -1);
		set_dom_node_attr(plk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(plk, _T("VX"), -1, _T("0x00700001"), -1);
		set_dom_node_attr(plk, _T("VT"), -1, _T("(0070,0001)"), -1);
		set_dom_node_attr(plk, _T("VR"), -1, _T("SQ"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(plk, _T("VL"), -1, token, -1);
	}
	else
	{
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(plk, _T("VL"), -1, token, -1);
	}
	
	glk = find_dom_node_by_name(plk, 1, _T("GraphicObjectSequence"), -1);
	if (!glk)
	{
		nlk = find_dom_node_by_name(plk, 0, _T("StartOfItem"), -1);
		if (!nlk)
		{
			nlk = insert_dom_node(plk, LINK_FIRST);
			set_dom_node_name(nlk, _T("StartOfItem"), -1);
			set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE000"), -1);
			set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E000)"), -1);
			xsprintf(token, _T("%d"), 0xFFFFFFFF);
			set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
		}
		else
		{
			xsprintf(token, _T("%d"), 0xFFFFFFFF);
			set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
		}

		glk = dicm_dom_insert_node_by_tag(nlk, _T("0x00700009"));
		set_dom_node_name(glk, _T("GraphicObjectSequence"), -1);
		set_dom_node_attr(glk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(glk, _T("VX"), -1, _T("0x00700009"), -1);
		set_dom_node_attr(glk, _T("VT"), -1, _T("(0070,0009)"), -1);
		set_dom_node_attr(glk, _T("VR"), -1, _T("SQ"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(glk, _T("VL"), -1, token, -1);
	}
	else
	{
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(glk, _T("VL"), -1, token, -1);

		//StartOfItem
		nlk = get_dom_parent_node(glk);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
	}

	ilk = find_dom_node_by_name(nlk, 0, _T("EndOfItems"), -1);
	if (!ilk)
	{
		ilk = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(ilk, _T("EndOfItems"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE00D"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E00D)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);
	}

	delete_dom_child_nodes(glk);

	for (i = 0; i < count; i++)
	{
		ilk = insert_dom_node(glk, LINK_LAST);
		set_dom_node_name(ilk, _T("StartOfItem"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE000"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E000)"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("GraphicAnnotationUnits"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700005"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0005)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("PIXEL"), -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(_T("PIXEL"), -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(_T("PIXEL"), -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("NumberOfGraphicPoints"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700021"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0021)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("IS"), -1);
		xsprintf(token, _T("%d"), pga[i].count);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("GraphicData"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700022"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0022)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("DS"), -1);
		dicm_format_point(pga[i].points, pga[i].count, token, 1024);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("GraphicType"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700023"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0023)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("LO"), -1);
		set_dom_node_text(nlk, pga[i].type, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(pga[i].type, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_gb2312(pga[i].type, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("GraphicFilled"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700024"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0024)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("N"), -1);
		set_dom_node_attr(nlk, _T("VL"), -1, _T("1"), -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("EndOfItems"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE00D"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E00D)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
	}

	nlk = insert_dom_node(glk, LINK_LAST);
	set_dom_node_name(nlk, _T("EndOfSequence"), -1);
	set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE0DD"), -1);
	set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E0DD)"), -1);
	xsprintf(token, _T("%d"), 0);
	set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

	ilk = find_dom_node_by_name(plk, 0, _T("EndOfSequence"), -1);
	if (!ilk)
	{
		ilk = insert_dom_node(plk, LINK_LAST);
		set_dom_node_name(ilk, _T("EndOfSequence"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE0DD"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E0DD)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);
	}
}

/*FileMetaInfoGroupLength VX = "0x00020000" VR = "UL"
FileMetaInfoVersion VX = "0x00020001" VR = "OB"
MediaStorageSOPClassUID VX = "0x00020002" VR = "UI"
MediaStorageSOPInstanceUID VX = "0x00020003" VR = "UI"
TransferSyntaxUID VX = "0x00020010" VR = "UI"
ImplementationClassUID VX = "0x00020012" VR = "UI"
ImplementationVersionName VX = "0x00020013" VR = "SH"
SourceApplicationEntityTitle VX = "0x00020016" VR = "AE"
SOPClassUID VX = "0x00080016" VR = "UI"
SOPInstanceUID VX = "0x00080018" VR = "UI"
StudyDate VX = "0x00080020" VR = "DA",
SeriesDate VX = "0x00080021" VR = "DA"
ContentDate VX = "0x00080023" VR = "DA"
StudyTime VX = "0x00080030" VR = "TM"
SeriesTime VX = "0x00080031" VR = "TM"
ContentTime VX = "0x00080033" VR = "TM"
AccessionNumber VX = "0x00080050" VR = "SH"
Modality VX = "0x00080060" VR = "CS"
PatientName VX = "0x00100010" VR = "PN"
PatientID VX = "0x00100020" VR = "LO"
PatientBirthDate VX = "0x00100030" VR = "DA"
PatientSex VX = "0x00100040" VR = "CS"
PatientAge VX = "0x00101010" VR = "AS"
StudyInstanceUID VX = "0x0020000D" VR = "UI"
SeriesInstanceUID VX = "0x0020000E" VR = "UI"
SeriesNumber VX = "0x00200011" VR = "IS"
InstanceNumber VX = "0x00200013" VR = "IS"
SamplesPerPixel VX = "0x00280002" VR = "US"
PhotometricInterpretation VX = "0x00280004" VR = "CS"
PlanarConfiguration VX = "0x00280006" VR = "US"
Rows VX = "0x00280010" VR = "US"
Columns VX = "0x00280011" VR = "US"
BitsAllocated VX = "0x00280100" VR = "US"
BitsStored VX = "0x00280101" VR = "US"
HighBit VX = "0x00280102" VR = "US"
PixelRepresentation VX = "0x00280103" VR = "US"
PixelData VX = "0x7FE00010" VR = "OW"*/

//Secondary Capture Image Storage
//SOPClassUID 1.2.840.10008.5.1.4.1.1.7
const dword_t SCTAGS[] = {
	0x00020000,
	0x00020001,
	0x00020002,
	0x00020003,
	0x00020010,
	0x00020012,
	0x00020013,
	0x00020016,

	0x00080016,
	0x00080018,
	0x00080020,
	0x00080021,
	0x00080023,
	0x00080030,
	0x00080031,
	0x00080033,
	0x00080050,
	0x00080060,

	0x00100010,
	0x00100020,
	0x00100030,
	0x00100040,
	0x00101010,

	0x0020000D,
	0x0020000E,
	0x00200011,
	0x00200013,

	0x00280002,
	0x00280004,
	0x00280006,
	0x00280010,
	0x00280011,
	0x00280100,
	0x00280101,
	0x00280102,
	0x00280103,

	0x7FE00010
};

void dicm_dom_sc_image(LINKPTR dom)
{
	LINKPTR nlk;
	int i, n;
	sword_t l, h;
	const dicm_item_t* pitem;
	const dicm_group_t* pgroup;
	tchar_t tvg[NAME_LEN] = { 0 };
	tchar_t tvn[NAME_LEN] = { 0 };
	tchar_t tvt[NUM_LEN] = { 0 };
	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };

	set_dom_node_name(dom, _T("DICM"), -1);

	n = sizeof(SCTAGS) / sizeof(dword_t);
	for (i = 0; i < n; i++)
	{
		h = (sword_t)((SCTAGS[i] & 0xFFFF0000) >> 16);
		l = (sword_t)(SCTAGS[i] & 0x0000FFFF);

		pgroup = dicm_group_info(SCTAGS[i]);
		if (pgroup)
			xscpy(tvg, pgroup->gn);
		else
			xsprintf(tvg, _T("Unknown%04X"), h);

		pitem = dicm_item_info(SCTAGS[i]);
		if (pitem)
			xscpy(tvn, pitem->vn);
		else
			xsprintf(tvn, _T("Unknown%04X%04X"), h, l);

		xsprintf(tvx, _T("0x%04X%04X"), h, l);
		xsprintf(tvt, _T("(%04X,%04X)"), h, l);

		nlk = insert_dom_node(dom, LINK_LAST);
		set_dom_node_name(nlk, tvn, -1);
		set_dom_node_attr(nlk, _T("VR"), 2, pitem->vr, 2);
		set_dom_node_attr(nlk, _T("VL"), 2, NULL, -1);
		set_dom_node_attr(nlk, _T("VX"), 2, tvx, -1);
		set_dom_node_attr(nlk, _T("VT"), 2, tvt, -1);
		set_dom_node_attr(nlk, _T("VG"), 2, tvg, -1);

		if (compare_text(tvn, -1, _T("FileMetaInfoGroupLength"), -1, 0) == 0)
		{
			set_dom_node_text(nlk, _T("196"), -1);
		}
		else if (compare_text(tvn, -1, _T("FileMetaInfoVersion"), -1, 0) == 0)
		{
			set_dom_node_cdata(nlk, 1);
			set_dom_node_text(nlk, _T("AAE="), -1);
		}
		else if (compare_text(tvn, -1, _T("MediaStorageSOPClassUID"), -1, 0) == 0)
		{
			set_dom_node_text(nlk, _T("1.2.840.10008.5.1.4.1.1.7"), -1);
		}
		else if (compare_text(tvn, -1, _T("TransferSyntaxUID"), -1, 0) == 0)
		{
			set_dom_node_text(nlk, _T("1.2.840.10008.1.2.1"), -1);
		}
		else if (compare_text(tvn, -1, _T("ImplementationClassUID"), -1, 0) == 0)
		{
			set_dom_node_text(nlk, _T("1.2.840.473.8013.2.3.1"), -1);
		}
		else if (compare_text(tvn, -1, _T("ImplementationVersionName"), -1, 0) == 0)
		{
			set_dom_node_text(nlk, _T("DICOMLIB231"), -1);
		}
		else if (compare_text(tvn, -1, _T("SOPClassUID"), -1, 0) == 0)
		{
			set_dom_node_text(nlk, _T("1.2.840.10008.5.1.4.1.1.7"), -1);
		}
		else if (compare_text(tvn, -1, _T("SOPClassUID"), -1, 0) == 0)
		{
			set_dom_node_text(nlk, _T("1.2.840.10008.5.1.4.1.1.7"), -1);
		}

	}
}