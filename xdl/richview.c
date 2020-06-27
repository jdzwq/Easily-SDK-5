/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc rich view document

	@module	richview.c | implement file

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

#include "richview.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#if defined(XDL_SUPPORT_VIEW)

typedef struct _RICHWORDOPERATOR{
	bool_t paged;
	int page;

	link_t_ptr rich;
	link_t_ptr nlk;

	const tchar_t* text;
	int len, pos;
	int ind;
	tchar_t pch[CHS_LEN + 1];
	int point;

	float permm;
	float indent;
	int place;

	PF_TEXT_SIZE pf_text_size;
	void* ctx;
	const xfont_t* pxf;
}RICHWORDOPERATOR;

#define RICHWORD_INDICATOR_NEXT_NODE	-4
#define RICHWORD_INDICATOR_NEXT_INDENT	-3
#define RICHWORD_INDICATOR_NEXT_ATOM	-2
#define RICHWORD_INDICATOR_NEXT_BREAK	-1
#define RICHWORD_INDICATOR_NEXT_WORD	0

bool_t call_rich_is_paging(void* param)
{
	RICHWORDOPERATOR* pscan = (RICHWORDOPERATOR*)param;

	return pscan->paged;
}

bool_t call_rich_break_page(void* param)
{
	RICHWORDOPERATOR* pscan = (RICHWORDOPERATOR*)param;
	page_cator_t cat = { 0 };
	int i, pages = 0;
	link_t_ptr nlk;
	bool_t done = 0;

	if (is_last_link(pscan->nlk) && pscan->ind == RICHWORD_INDICATOR_NEXT_NODE)
	{
		return 0;
	}

	done = 0;
	nlk = pscan->nlk;
	while (nlk)
	{
		pages = get_dom_node_page_cator_count(nlk);
		for (i = pages; i >=1; i--)
		{
			get_dom_node_page_cator(nlk, i, &cat);
			if (cat.page >= pscan->page)
			{
				del_dom_node_page_cator(nlk, i);
			}
			if (cat.page == pscan->page)
			{
				done = 1;
				break;
			}
		}
		if (done)
			break;

		nlk = get_rich_prev_anch(pscan->rich, nlk);
	}

	nlk = get_rich_next_anch(pscan->rich, pscan->nlk);
	while (nlk)
	{
		del_dom_node_page_cator(nlk, 0);

		nlk = get_rich_next_anch(pscan->rich, nlk);
	}

	cat.page = pscan->page;
	cat.indi = pscan->ind;
	cat.pos = pscan->pos;
	xscpy(cat.pch, pscan->pch);
	cat.point = pscan->point;

	pages = get_dom_node_page_cator_count(pscan->nlk) + 1;

	set_dom_node_page_cator(pscan->nlk, pages, &cat);

	return 1;
}

int call_rich_next_page(void* param)
{
	RICHWORDOPERATOR* pscan = (RICHWORDOPERATOR*)param;

	page_cator_t cat = { 0 };
	int pages;

	if (!pscan->page)
	{
		pscan->nlk = get_rich_next_anch(pscan->rich, LINK_FIRST);

		if (pscan->nlk)
		{
			pscan->indent = get_rich_anch_text_indent(pscan->nlk);
			pscan->text = NULL;
			pscan->len = (pscan->indent > 0) ? 1 : 0;
			pscan->pos = -1;
			xscpy(pscan->pch, _T("\t"));

			pscan->point = -1;

			pscan->ind = RICHWORD_INDICATOR_NEXT_INDENT;
		}
		else
		{
			pscan->text = NULL;
			pscan->len = 0;
			pscan->pos = 0;
			xszero(pscan->pch, CHS_LEN + 1);

			pscan->point = 0;

			pscan->ind = RICHWORD_INDICATOR_NEXT_NODE;
		}
	}
	else
	{
		while (pscan->nlk)
		{
			pages = 1;
			while(get_dom_node_page_cator(pscan->nlk, pages, &cat))
			{
				if (cat.page == pscan->page)
					break;

				pages++;
			}
			
			if (cat.page == pscan->page)
				break;

			pscan->nlk = get_rich_next_anch(pscan->rich, pscan->nlk);
		}

		if (!pscan->nlk)
			return 0;

		pscan->pos = cat.pos;
		pscan->ind = cat.indi;
		xscpy(pscan->pch, cat.pch);
		pscan->point = cat.point;

		if (pscan->ind == RICHWORD_INDICATOR_NEXT_INDENT)
		{
			pscan->indent = get_rich_anch_text_indent(pscan->nlk);
			pscan->text = NULL;
			pscan->len = (pscan->indent > 0) ? 1 : 0;
		}
		else if (pscan->ind == RICHWORD_INDICATOR_NEXT_ATOM || pscan->ind == RICHWORD_INDICATOR_NEXT_BREAK)
		{
			pscan->text = get_rich_anch_title_ptr(pscan->nlk);
			pscan->len = xslen(pscan->text);
		}
		else
		{
			pscan->text = get_rich_anch_text_ptr(pscan->nlk);
			pscan->len = xslen(pscan->text);
		}
	}
	
	pscan->page++;

	return pscan->page;
}

int call_rich_next_words(void* param, tchar_t** ppch, xsize_t* pse, bool_t* pins, bool_t* pdel, bool_t* psel, bool_t* patom)
{
	RICHWORDOPERATOR* pscan = (RICHWORDOPERATOR*)param;
	int n;
	xsize_t xs = { 0 };

	if (pscan->ind == RICHWORD_INDICATOR_NEXT_NODE)
	{
		pscan->nlk = (pscan->nlk) ? get_rich_next_anch(pscan->rich, pscan->nlk) : NULL;
		if (!pscan->nlk)
		{
			*pins = 0;
			*pdel = 0;
			*psel = 0;
			*patom = 0;

			pse->cx = 0;

			return 0;
		}

		pscan->indent = get_rich_anch_text_indent(pscan->nlk);
		pscan->text = NULL;
		pscan->len = (pscan->indent > 0) ? 1 : 0;
		pscan->pos = -1;
		xscpy(pscan->pch, _T("\t"));

		pscan->point = -1;

		pscan->ind = RICHWORD_INDICATOR_NEXT_INDENT;
	}

	if (pscan->ind == RICHWORD_INDICATOR_NEXT_INDENT)
	{
		if (pscan->pos < 0)
		{
			pscan->place = (int)(get_rich_anch_text_place(pscan->nlk) * pscan->permm);
		}

		n = xslen(pscan->pch);
		pscan->pos += n;

		if (pscan->pos == pscan->len)
		{
			xszero(pscan->pch, CHS_LEN + 1);

			pscan->text = get_rich_anch_title_ptr(pscan->nlk);
			pscan->len = xslen(pscan->text);
			pscan->pos = 0;
			pscan->point = -1;

			pscan->ind = RICHWORD_INDICATOR_NEXT_ATOM;
		}
		else
		{
			xscpy(pscan->pch, _T("\t"));
			*ppch = pscan->pch;
			n = 1;

			pse->cx = (int)(pscan->indent * pscan->permm);
		}
	}

	if (pscan->ind == RICHWORD_INDICATOR_NEXT_ATOM)
	{
		n = xschs(pscan->pch);
		pscan->pos += n;

		if (pscan->pos == pscan->len)
		{
			pscan->len++;
			pscan->ind = RICHWORD_INDICATOR_NEXT_BREAK;
		}
		else
		{
			n = xschs(pscan->text + pscan->pos);
			xsncpy(pscan->pch, pscan->text + pscan->pos, n);

			*ppch = pscan->pch;

			if (n == 1 && pscan->pch[0] == _T('\t'))
			{
				pse->cx *= 4;
			}
			else if (n == 1 && IS_CONTROL_CHAR(pscan->pch[0]))
			{
				pse->cx *= 1;
			}
			else
			{
				(*pscan->pf_text_size)(pscan->ctx, pscan->pxf, pscan->pch, n, &xs);

				if (xs.cx)
					pse->cx = xs.cx;
				if (xs.cy)
					pse->cy = xs.cy;
			}

			if (pscan->place)
			{
				pscan->place -= pse->cx;
			}
		}
	}

	if (pscan->ind == RICHWORD_INDICATOR_NEXT_BREAK)
	{
		if (pscan->pos == pscan->len)
		{
			pscan->text = get_rich_anch_text_ptr(pscan->nlk);
			pscan->len = xslen(pscan->text);
			pscan->pos = 0;
			pscan->point = 0;

			xszero(pscan->pch, CHS_LEN + 1);
			pscan->ind = RICHWORD_INDICATOR_NEXT_WORD;
		}
		else
		{
			pscan->len--;
			pscan->pos = pscan->len;

			xscpy(pscan->pch, _T("\0"));
			n = 1;

			*ppch = pscan->pch;

			if (pscan->place)
			{
				pscan->place -= pse->cx;
			}
		}
	}

	if (pscan->ind == RICHWORD_INDICATOR_NEXT_WORD)
	{
		n = xschs(pscan->pch);
		pscan->pos += n;

		if (n) pscan->point++;

		if (pscan->pos == pscan->len)
		{
			if (get_rich_anch_lined(pscan->nlk))
				xscpy(pscan->pch, _T("\n"));
			else
				xscpy(pscan->pch, _T("\t"));

			n = 1;
			*ppch = pscan->pch;

			if (pscan->place > 0)
			{
				pse->cx = pscan->place;
			}

			pscan->place = 0;

			pscan->ind = RICHWORD_INDICATOR_NEXT_NODE;
		}
		else
		{
			n = xschs(pscan->text + pscan->pos);
			xsncpy(pscan->pch, pscan->text + pscan->pos, n);
			*ppch = pscan->pch;

			if (!get_dom_node_line_cator(pscan->nlk, pscan->point, &pse->cx, &pse->cy))
			{
				if (n == 1 && pscan->pch[0] == _T('\t'))
				{
					pse->cx *= 4;
				}
				else if (n == 1 && IS_CONTROL_CHAR(pscan->pch[0]))
				{
					pse->cx *= 1;
					pse->cy *= 1;
				}
				else
				{
					(*pscan->pf_text_size)(pscan->ctx, pscan->pxf, pscan->pch, n, &xs);

					if (xs.cx)
						pse->cx = xs.cx;
					if (xs.cy)
						pse->cy = xs.cy;
				}

				ins_dom_node_line_cator(pscan->nlk, pscan->point, pse->cx, pse->cy);
			}

			if (pscan->place)
			{
				pscan->place -= pse->cx;
			}
		}
	}

	switch (pscan->ind)
	{
	case RICHWORD_INDICATOR_NEXT_NODE:
		*pins = 1;
		*pdel = 0;
		*psel = 0;
		*patom = 0;
		break;
	case RICHWORD_INDICATOR_NEXT_INDENT:
		*pins = 0;
		*pdel = 0;
		*psel = 0;
		*patom = 0;
		break;
	case RICHWORD_INDICATOR_NEXT_ATOM:
		*pins = (get_rich_anch_fixed(pscan->nlk))? 0 : 1;
		*pdel = *pins;
		*psel = 0;
		*patom = 1;
		break;
	case RICHWORD_INDICATOR_NEXT_BREAK:
		*pins = (get_rich_anch_fixed(pscan->nlk)) ? 0 : 1;
		*pdel = 0;
		*psel = 0;
		*patom = 1;
		break;
	case RICHWORD_INDICATOR_NEXT_WORD:
		*pins = 1;
		*pdel = 1;
		*psel = 1;
		*patom = 0;
		break;
	default:
		*pins = 0;
		*pdel = 0;
		*psel = 0;
		*patom = 0;
		break;
	}

	return n;
}

int call_rich_insert_words(void* param, tchar_t* pch, xsize_t* pse)
{
	RICHWORDOPERATOR* pscan = (RICHWORDOPERATOR*)param;
	int n = 0;
	xsize_t xs = { 0 };

	if (!pscan->nlk)
		return 0;

	switch (pscan->ind)
	{
	case RICHWORD_INDICATOR_NEXT_INDENT:
		break;
	case RICHWORD_INDICATOR_NEXT_ATOM:
	case RICHWORD_INDICATOR_NEXT_BREAK:
		n = xschs(pch);
		pscan->text = rich_anch_title_ins_chars(pscan->nlk, pscan->pos, pch, n);
		pscan->len += n;

		xszero(pscan->pch, CHS_LEN + 1);
		break;
	case RICHWORD_INDICATOR_NEXT_NODE:
	case RICHWORD_INDICATOR_NEXT_WORD:
		n = xschs(pch);
		pscan->text = rich_anch_text_ins_chars(pscan->nlk, pscan->pos, pch, n);
		pscan->len += n;

		if (n == 1 && pch[0] == _T('\t'))
		{
			xs.cx = pse->cx * 4;
			xs.cy = pse->cy;
		}
		else if (n == 1 && IS_CONTROL_CHAR(pch[0]))
		{
			xs.cx = pse->cx;
			xs.cy = pse->cy;
		}
		else
		{
			(*pscan->pf_text_size)(pscan->ctx, pscan->pxf, pch, n, &xs);

			if (!xs.cx)
				xs.cx = pse->cx;
			if (!xs.cy)
				xs.cy = pse->cy;
		}

		ins_dom_node_line_cator(pscan->nlk, pscan->point, xs.cx, xs.cy);

		xszero(pscan->pch, CHS_LEN + 1);
		pscan->ind = RICHWORD_INDICATOR_NEXT_WORD;
		break;
	}

	return n;
}

int call_rich_delete_words(void* param)
{
	RICHWORDOPERATOR* pscan = (RICHWORDOPERATOR*)param;
	int n = 0;

	if (!pscan->nlk)
		return 0;

	switch (pscan->ind)
	{
	case RICHWORD_INDICATOR_NEXT_NODE:
		break;
	case RICHWORD_INDICATOR_NEXT_INDENT:
		break;
	case RICHWORD_INDICATOR_NEXT_ATOM:
	case RICHWORD_INDICATOR_NEXT_BREAK:
		n = xschs(pscan->text + pscan->pos);
		pscan->text = rich_anch_title_del_chars(pscan->nlk, pscan->pos, n);
		pscan->len -= n;

		xszero(pscan->pch, CHS_LEN + 1);
		break;
	case RICHWORD_INDICATOR_NEXT_WORD:
		n = xschs(pscan->text + pscan->pos);
		pscan->text = rich_anch_text_del_chars(pscan->nlk, pscan->pos, n);
		pscan->len -= n;

		del_dom_node_line_cator(pscan->nlk, pscan->point);

		xszero(pscan->pch, CHS_LEN + 1);
		break;
	}

	return n;
}

void call_rich_cur_object(void* param, void** pobj)
{
	RICHWORDOPERATOR* pscan = (RICHWORDOPERATOR*)param;

	*pobj = (void*)pscan->nlk;
}

void scan_rich_text(link_t_ptr ptr, if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, int bx, int by, int bw, int bh, bool_t paged, PF_SCAN_TEXTOR_CALLBACK pf, void* pp)
{
	RICHWORDOPERATOR ro = { 0 };
	if_wordscan_t it = { 0 };

	ro.rich = ptr;
	ro.pf_text_size = pif->pf_text_size;
	ro.ctx = pif->ctx;
	ro.pxf = pxf;
	ro.permm = (*pif->pf_mm_points)(pif->ctx, 1);

	it.param = (void*)&ro;
	it.pf_is_paging = call_rich_is_paging;
	it.pf_cur_object = call_rich_cur_object;
	it.pf_delete_word = call_rich_delete_words;
	it.pf_insert_word = call_rich_insert_words;
	it.pf_next_word = call_rich_next_words;

	if (paged)
	{
		ro.paged = paged;
		it.pf_next_page = call_rich_next_page;
		it.pf_break_page = call_rich_break_page;
	}
	else
	{
		call_rich_next_page((void*)&ro);
	}
	
	scan_object_text(pif, pxf, pxa, bx, by, bw, bh, &it, pf, pp);
}


#endif //XDL_SUPPORT_VIEW