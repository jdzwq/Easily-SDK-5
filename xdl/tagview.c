/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tag view document

	@module	tagview.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#include "tagview.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdldoc.h"

#if defined(XDL_SUPPORT_VIEW) 

typedef struct _tag_scan_contet{
	bool_t paged;
	int page;

	link_t_ptr tag;
	link_t_ptr nlk;
	const tchar_t* text;
	int len, pos;
	int ind;
	tchar_t pch[CHS_LEN + 1];

	int point;

	PF_TEXT_SIZE pf_text_size;
	void* ctx;
	const xfont_t* pxf;
}tag_scan_contet;

#define TAGWORD_INDICATOR_NEXT_NODE		0
#define TAGWORD_INDICATOR_NEXT_WORD		1

bool_t call_tag_is_paging(void* ctx)
{
	return 0;
}

bool_t call_tag_break_page(void* ctx)
{
	return 0;
}

int call_tag_next_page(void* ctx)
{
	tag_scan_contet* pscan = (tag_scan_contet*)ctx;

	if (!pscan->page)
	{
		pscan->page = 1;
	}

	return pscan->page;
}


int call_tag_next_words(void* ctx, tchar_t** ppch, xsize_t* pse, bool_t* pins, bool_t* pdel, bool_t* psel, bool_t* patom)
{
	tag_scan_contet* pscan = (tag_scan_contet*)ctx;
	int n;
	xsize_t xs;

	if (pscan->ind == TAGWORD_INDICATOR_NEXT_NODE)
	{
		pscan->nlk = (pscan->nlk) ? get_tag_next_joint(pscan->tag, pscan->nlk) : get_tag_next_joint(pscan->tag, LINK_FIRST);
		if (!pscan->nlk)
		{
			*pins = 1;
			*pdel = 0;
			*psel = 0;
			*patom = 0;

			pse->w = 0;

			return 0;
		}

		pscan->text = get_tag_joint_text_ptr(pscan->nlk);
		pscan->len = xslen(pscan->text);
		pscan->pos = 0;
		pscan->point = 0;
		pscan->ind = TAGWORD_INDICATOR_NEXT_WORD;

		xszero(pscan->pch, CHS_LEN + 1);
	}

	if (pscan->ind == TAGWORD_INDICATOR_NEXT_WORD)
	{
		n = xschs(pscan->pch);
		pscan->pos += n;

		if (n) pscan->point++;

		n = xschs(pscan->text + pscan->pos);
		xsncpy(pscan->pch, pscan->text + pscan->pos, n);

		if (!get_dom_node_line_cator(pscan->nlk, pscan->point, &pse->w, &pse->h))
		{
			if (n == 1 && pscan->pch[0] == _T('\t'))
			{
				pse->w *= 4;
			}
			else if (n == 1 && IS_CONTROL_CHAR(pscan->pch[0]))
			{
				pse->w *= 1;
			}
			else
			{
				(*pscan->pf_text_size)(pscan->ctx, pscan->pxf, pscan->pch, n, &xs);

				if (xs.w)
					pse->w = xs.w;
				if (xs.h)
					pse->h = xs.h;
			}

			ins_dom_node_line_cator(pscan->nlk, pscan->point, pse->w, pse->h);
		}
	}

	*ppch = pscan->pch;

	if (pscan->pos + n == pscan->len)
	{
		pscan->ind = TAGWORD_INDICATOR_NEXT_NODE;
	}

	*pins = 1;
	*pdel = 1;
	*psel = 1;
	*patom = (is_tag_text_joint(pscan->nlk))? 0 : 2;

	return n;
}

int call_tag_insert_words(void* ctx, tchar_t* pch, xsize_t* pse)
{
	tag_scan_contet* pscan = (tag_scan_contet*)ctx;
	int n = 0;
	xsize_t xs = { 0 };

	if (!pscan->nlk)
	{
		pscan->nlk = get_tag_prev_joint(pscan->tag, LINK_LAST);
		if (!(pscan->nlk && is_tag_text_joint(pscan->nlk)))
		{
			pscan->nlk = insert_tag_joint(pscan->tag, LINK_LAST);
			pscan->text = NULL;
			pscan->len = 0;
			pscan->pos = 0;

			pscan->point = 0;
		}
		else
		{
			pscan->text = get_tag_joint_text_ptr(pscan->nlk);
			pscan->len = xslen(pscan->text);
			pscan->pos = pscan->len;

			pscan->point = get_dom_node_line_cator_count(pscan->nlk);
		}
		pscan->ind = TAGWORD_INDICATOR_NEXT_WORD;
	}

	switch (pscan->ind)
	{
	case TAGWORD_INDICATOR_NEXT_NODE:
	case TAGWORD_INDICATOR_NEXT_WORD:
		n = xschs(pch);
		pscan->text = tag_joint_text_ins_chars(pscan->nlk, pscan->pos, pch, n);
		pscan->len += n;

		if (n == 1 && pscan->pch[0] == _T('\t'))
		{
			xs.w = pse->w * 4;
			xs.h = pse->h;
		}
		else if (n == 1 && IS_CONTROL_CHAR(pch[0]))
		{
			xs.w = pse->w;
			xs.h = pse->h;
		}
		else
		{
			(*pscan->pf_text_size)(pscan->ctx, pscan->pxf, pch, n, &xs);

			if (!xs.w)
				xs.w = pse->w;
			if (!xs.h)
				xs.h = pse->h;
		}

		ins_dom_node_line_cator(pscan->nlk, pscan->point, xs.w, xs.h);

		xszero(pscan->pch, CHS_LEN + 1);
		pscan->ind = TAGWORD_INDICATOR_NEXT_WORD;
		break;
	}

	return n;
}

int call_tag_delete_words(void* ctx)
{
	tag_scan_contet* pscan = (tag_scan_contet*)ctx;
	int n = 0;
	link_t_ptr nlk;

	if (!pscan->nlk)
		return 0;

	switch (pscan->ind)
	{
	case TAGWORD_INDICATOR_NEXT_NODE:
	case TAGWORD_INDICATOR_NEXT_WORD:
		n = xschs(pscan->text + pscan->pos);
		pscan->text = tag_joint_text_del_chars(pscan->nlk, pscan->pos, n);
		pscan->len -= n;

		xszero(pscan->pch, CHS_LEN + 1);

		del_dom_node_line_cator(pscan->nlk, pscan->point);

		if (!pscan->len)
		{
			nlk = pscan->nlk;
			pscan->nlk = get_tag_prev_joint(pscan->tag, nlk);
			delete_tag_joint(nlk);

			if (pscan->nlk)
			{
				pscan->text = get_tag_joint_text_ptr(pscan->nlk);
				pscan->len = xslen(pscan->text);
				pscan->pos = pscan->len;
				pscan->point = get_dom_node_line_cator_count(pscan->nlk);

				pscan->ind = TAGWORD_INDICATOR_NEXT_WORD;
			}
			else
			{
				pscan->text = NULL;
				pscan->len = 0;
				pscan->pos = 0;
				pscan->point = 0;

				pscan->ind = TAGWORD_INDICATOR_NEXT_NODE;
			}
		}
		break;
	}

	return n;
}

void call_tag_cur_object(void* ctx, void** pobj)
{
	tag_scan_contet* pscan = (tag_scan_contet*)ctx;

	*pobj = (void*)pscan->nlk;
}

void scan_tag_text(link_t_ptr ptr, const measure_interface* pif, const xfont_t* pxf, const xface_t* pxa, int bx, int by, int bw, int bh, bool_t paged, PF_SCAN_TEXTOR_CALLBACK pf, void* pp)
{
	tag_scan_contet ro = { 0 };
	wordscan_interface it = { 0 };

	ro.tag = ptr;
	ro.pf_text_size = pif->pf_measure_size;
	ro.ctx = pif->ctx;
	ro.pxf = pxf;

	it.ctx = (void*)&ro;
	it.pf_is_paging = call_tag_is_paging;
	it.pf_cur_object = call_tag_cur_object;
	it.pf_delete_word = call_tag_delete_words;
	it.pf_insert_word = call_tag_insert_words;
	it.pf_next_word = call_tag_next_words;

	if (paged)
	{
		ro.paged = paged;
		it.pf_next_page = call_tag_next_page;
		it.pf_break_page = call_tag_break_page;
	}
	else
	{
		call_tag_next_page((void*)&ro);
	}
	
	scan_object_text(pif, pxf, pxa, bx, by, bw, bh, &it, pf, pp);
}

#endif //XDL_SUPPORT_VIEW