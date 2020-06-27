/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memo view

	@module	memoview.c | implement file

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
#include "memoview.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"


#if defined(XDL_SUPPORT_VIEW) 

typedef struct _MEMOWORDOPERATOR{
	bool_t paged;
	int page;

	link_t_ptr memo;
	link_t_ptr nlk;
	const tchar_t* text;
	int len, pos;
	int ind;
	tchar_t pch[CHS_LEN + 1];

	int point;

	PF_TEXT_SIZE pf_text_size;
	void* ctx;
	const xfont_t* pxf;
}MEMOWORDOPERATOR;

//#define MEMOWORD_INDICATOR_NEXT_RETURN	-3
#define MEMOWORD_INDICATOR_NEXT_LINE	-2
#define MEMOWORD_INDICATOR_NEXT_INDENT	-1
#define MEMOWORD_INDICATOR_NEXT_WORD	0

bool_t call_memo_is_paging(void* param)
{
	MEMOWORDOPERATOR* pscan = (MEMOWORDOPERATOR*)param;

	return pscan->paged;
}

bool_t call_memo_break_page(void* param)
{
	MEMOWORDOPERATOR* pscan = (MEMOWORDOPERATOR*)param;
	page_cator_t cat = { 0 };
	int i, pages = 0;
	link_t_ptr nlk;
	bool_t done = 0;

	if (is_last_link(pscan->nlk) && pscan->ind == MEMOWORD_INDICATOR_NEXT_LINE)
	{
		return 0;
	}

	nlk = pscan->nlk;
	while (nlk)
	{
		pages = get_dom_node_page_cator_count(nlk);
		for (i = pages; i >= 1; i--)
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

		nlk = get_memo_prev_line(pscan->memo, nlk);
	}

	nlk = get_memo_next_line(pscan->memo, pscan->nlk);
	while (nlk)
	{
		del_dom_node_page_cator(nlk, 0);

		nlk = get_memo_next_line(pscan->memo, nlk);
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

int call_memo_next_page(void* param)
{
	MEMOWORDOPERATOR* pscan = (MEMOWORDOPERATOR*)param;

	page_cator_t cat = { 0 };
	int pages;

	if (!pscan->page)
	{
		pscan->nlk = get_memo_next_line(pscan->memo, LINK_FIRST);

		if (pscan->nlk)
		{
			pscan->text = NULL;
			pscan->len = get_memo_line_indent(pscan->nlk);
			pscan->pos = -1;
			xscpy(pscan->pch, _T("\t"));

			pscan->point = -1;

			pscan->ind = MEMOWORD_INDICATOR_NEXT_INDENT;
		}
		else
		{
			pscan->text = NULL;
			pscan->len = 0;
			pscan->pos = 0;
			xszero(pscan->pch, CHS_LEN + 1);

			pscan->point = 0;

			pscan->ind = MEMOWORD_INDICATOR_NEXT_LINE;
		}
	}
	else
	{
		while (pscan->nlk)
		{
			pages = 1;
			while (get_dom_node_page_cator(pscan->nlk, pages, &cat))
			{
				if (cat.page == pscan->page)
					break;

				pages++;
			}

			if (cat.page == pscan->page)
				break;

			pscan->nlk = get_memo_next_line(pscan->memo, pscan->nlk);
		}

		if (!pscan->nlk)
			return 0;

		pscan->pos = cat.pos;
		pscan->ind = cat.indi;
		xscpy(pscan->pch, cat.pch);
		pscan->point = cat.point;

		if (pscan->ind == MEMOWORD_INDICATOR_NEXT_INDENT)
		{
			pscan->text = NULL;
			pscan->len = get_memo_line_indent(pscan->nlk);
		}
		else
		{
			pscan->text = get_memo_line_text_ptr(pscan->nlk);
			pscan->len = xslen(pscan->text);
		}
	}

	pscan->page++;

	return pscan->page;
}


int call_memo_next_words(void* param, tchar_t** ppch, xsize_t* pse, bool_t* pins, bool_t* pdel, bool_t* psel, bool_t* patom)
{
	MEMOWORDOPERATOR* pscan = (MEMOWORDOPERATOR*)param;
	int n;
	xsize_t xs;

	if (pscan->ind == MEMOWORD_INDICATOR_NEXT_LINE)
	{
		pscan->nlk = (pscan->nlk) ? get_memo_next_line(pscan->memo, pscan->nlk) : NULL;
		if (!pscan->nlk)
		{
			*pins = 1;
			*pdel = 0;
			*psel = 0;
			*patom = 0;

			pse->cx = 0;

			return 0;
		}

		pscan->text = NULL;
		pscan->len = get_memo_line_indent(pscan->nlk);
		pscan->pos = -1;
		xscpy(pscan->pch, _T("\t"));

		pscan->point = -1;

		pscan->ind = MEMOWORD_INDICATOR_NEXT_INDENT;
	}
	
	if (pscan->ind == MEMOWORD_INDICATOR_NEXT_INDENT)
	{
		n = xslen(pscan->pch);
		pscan->pos += n;

		if (pscan->pos == pscan->len)
		{
			pscan->text = get_memo_line_text_ptr(pscan->nlk);
			pscan->len = xslen(pscan->text);
			pscan->pos = 0;
			xszero(pscan->pch, CHS_LEN + 1);

			pscan->point = 0;

			pscan->ind = MEMOWORD_INDICATOR_NEXT_WORD;
		}
		else
		{
			xscpy(pscan->pch, _T("\t"));
			*ppch = pscan->pch;
			n = 1;

			pse->cx *= 4;
		}
	}
	
	if (pscan->ind == MEMOWORD_INDICATOR_NEXT_WORD)
	{
		n = xschs(pscan->pch);
		pscan->pos += n;

		if (n) pscan->point++;

		if (pscan->pos == pscan->len)
		{
			xscpy(pscan->pch, _T("\n"));
			*ppch = pscan->pch;
			n = 1;

			pse->cx *= 2;

			pscan->ind = MEMOWORD_INDICATOR_NEXT_LINE;
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
		}
	}

	switch (pscan->ind)
	{
	case MEMOWORD_INDICATOR_NEXT_LINE:
		*pins = 1;
		*pdel = 1;
		*psel = 0;
		*patom = 0;
		break;
	case MEMOWORD_INDICATOR_NEXT_INDENT:
		*pins = 1;
		*pdel = 1;
		*psel = 0;
		*patom = 0;
		break;
	case MEMOWORD_INDICATOR_NEXT_WORD:
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

int call_memo_insert_words(void* param, tchar_t* pch, xsize_t* pse)
{
	MEMOWORDOPERATOR* pscan = (MEMOWORDOPERATOR*)param;
	int n;
	xsize_t xs = { 0 };
	link_t_ptr plk;

	if (!pscan->nlk)
	{
		pscan->nlk = insert_memo_line(pscan->memo, LINK_LAST);
		pscan->text = NULL;
		pscan->len = 0;
		pscan->pos = 0;

		pscan->point = -1;

		pscan->ind = MEMOWORD_INDICATOR_NEXT_INDENT;
	}

	switch (pscan->ind)
	{
	case MEMOWORD_INDICATOR_NEXT_INDENT:
		if (*pch == _T('\t'))
		{
			n = get_memo_line_indent(pscan->nlk);
			set_memo_line_indent(pscan->nlk, n + 1);
			pscan->len++;
			n = 1;
		}
		else
		{
			n = get_memo_line_indent(pscan->nlk) - pscan->pos;
			set_memo_line_indent(pscan->nlk, pscan->pos);

			if (*pch == _T('\n'))
			{
				cut_dom_node_line_cator(pscan->nlk, 0);

				plk = split_memo_line(pscan->nlk, 0);
				set_memo_line_indent(plk, n);
	
				pscan->len = pscan->pos;
				xszero(pscan->pch, CHS_LEN + 1);
				n = 1;
			}
			else
			{
				while (n--)
				{
					memo_line_text_ins_chars(pscan->nlk, 0, _T("\t"), 1);
					pscan->len--;

					ins_dom_node_line_cator(pscan->nlk, 0, 1, 1);
				}
				xszero(pscan->pch, CHS_LEN + 1);

				n = xschs(pch);
				memo_line_text_ins_chars(pscan->nlk, 0, pch, n);

				if (n == 1 && pscan->pch[0] == _T('\t'))
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

				ins_dom_node_line_cator(pscan->nlk, 0, xs.cx, xs.cy);
			}
		}
		break;
	case MEMOWORD_INDICATOR_NEXT_WORD:
		if (*pch == _T('\t') && !pscan->pos)
		{
			n = get_memo_line_indent(pscan->nlk);
			set_memo_line_indent(pscan->nlk, n + 1);
			pscan->len = n + 1;
			pscan->pos = n;
			xszero(pscan->pch, CHS_LEN + 1);
			pscan->ind = MEMOWORD_INDICATOR_NEXT_INDENT;
			n = 1;
			break;
		}
		else if (*pch == _T('\n'))
		{
			cut_dom_node_line_cator(pscan->nlk, pscan->point);

			split_memo_line(pscan->nlk, pscan->pos);

			pscan->len = pscan->pos;
			xszero(pscan->pch, CHS_LEN + 1);
			n = 1;
			break;
		}

		n = xschs(pch);
		pscan->text = memo_line_text_ins_chars(pscan->nlk, pscan->pos, pch, n);
		pscan->len += n;
		xszero(pscan->pch, CHS_LEN + 1);

		if (n == 1 && pscan->pch[0] == _T('\t'))
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
		break;
	case MEMOWORD_INDICATOR_NEXT_LINE:
		if (*pch == _T('\n'))
		{
			insert_memo_line(pscan->memo, pscan->nlk);

			xszero(pscan->pch, CHS_LEN + 1);
			pscan->ind = MEMOWORD_INDICATOR_NEXT_WORD;
			n = 1;
		}
		else if (*pch == _T('\t') && !pscan->pos)
		{
			n = get_memo_line_indent(pscan->nlk);
			set_memo_line_indent(pscan->nlk, n + 1);
			pscan->len = n + 1;
			pscan->pos = n;
			xszero(pscan->pch, CHS_LEN + 1);

			pscan->ind = MEMOWORD_INDICATOR_NEXT_INDENT;
			n = 1;
		}
		else
		{
			n = xschs(pch);
			pscan->text = memo_line_text_ins_chars(pscan->nlk, pscan->pos, pch, n);
			pscan->pos = pscan->len;
			pscan->len += n;

			if (n == 1 && pscan->pch[0] == _T('\t'))
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
			pscan->ind = MEMOWORD_INDICATOR_NEXT_WORD;
		}

		break;
	}
	
	return n;
}

int call_memo_delete_words(void* param)
{
	MEMOWORDOPERATOR* pscan = (MEMOWORDOPERATOR*)param;
	int n;
	link_t_ptr nxt;

	if (!pscan->nlk)
		return 0;

	switch (pscan->ind)
	{
	case MEMOWORD_INDICATOR_NEXT_LINE:
		nxt = get_memo_next_line(pscan->memo, pscan->nlk);
		if (!nxt)
		{
			nxt = pscan->nlk;
			if (get_memo_line_indent(nxt) == 0 && is_null(get_memo_line_text_ptr(nxt)))
			{
				pscan->nlk = get_memo_prev_line(pscan->memo, nxt);
				delete_memo_line(nxt);

				if (pscan->nlk)
				{
					pscan->text = get_memo_line_text_ptr(pscan->nlk);
					pscan->len = xslen(pscan->text);
					pscan->pos = pscan->len;
					pscan->point = get_dom_node_line_cator_count(pscan->nlk);
				}
				else
				{
					pscan->pos = 0;
					pscan->text = NULL;
					pscan->len = 0;
					pscan->point = 0;
				}

				xszero(pscan->pch, CHS_LEN + 1);
				pscan->ind = MEMOWORD_INDICATOR_NEXT_LINE;

				return 1;
			}
			else
			{
				return 0;
			}
		}

		pscan->point = get_dom_node_line_cator_count(pscan->nlk);
		pscan->pos = xslen(get_memo_line_text_ptr(pscan->nlk));
		pscan->nlk = merge_memo_line(nxt);

		pscan->text = get_memo_line_text_ptr(pscan->nlk);
		pscan->len = xslen(pscan->text);

		xszero(pscan->pch, CHS_LEN + 1);

		pscan->ind = MEMOWORD_INDICATOR_NEXT_WORD;
		return 1;
	case MEMOWORD_INDICATOR_NEXT_INDENT:
		n = get_memo_line_indent(pscan->nlk);
		set_memo_line_indent(pscan->nlk, n - 1);
		pscan->len--;
		xszero(pscan->pch, CHS_LEN + 1);
		n = 1;
		return 1;
	case MEMOWORD_INDICATOR_NEXT_WORD:
		n = xschs(pscan->text + pscan->pos);
		pscan->text = memo_line_text_del_chars(pscan->nlk, pscan->pos, n);
		pscan->len -= n;
		xszero(pscan->pch, CHS_LEN + 1);

		del_dom_node_line_cator(pscan->nlk, pscan->point);
		return n;
	}

	return 0;
}

void call_memo_cur_object(void* param, void** pobj)
{
	MEMOWORDOPERATOR* pscan = (MEMOWORDOPERATOR*)param;

	*pobj = (void*)pscan->nlk;
}

void scan_memo_text(link_t_ptr ptr, if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, int bx, int by, int bw, int bh, bool_t paged, PF_SCAN_TEXTOR_CALLBACK pf, void* pp)
{
	MEMOWORDOPERATOR ro = { 0 };
	if_wordscan_t it = { 0 };

	ro.memo = ptr;
	ro.pf_text_size = pif->pf_text_size;
	ro.ctx = pif->ctx;
	ro.pxf = pxf;

	it.param = (void*)&ro;
	it.pf_is_paging = call_memo_is_paging;
	it.pf_cur_object = call_memo_cur_object;
	it.pf_delete_word = call_memo_delete_words;
	it.pf_insert_word = call_memo_insert_words;
	it.pf_next_word = call_memo_next_words;

	if (paged)
	{
		ro.paged = paged;
		it.pf_next_page = call_memo_next_page;
		it.pf_break_page = call_memo_break_page;
	}
	else
	{
		call_memo_next_page((void*)&ro);
	}

	scan_object_text(pif, pxf, pxa, bx, by, bw, bh, &it, pf, pp);
}

#endif //