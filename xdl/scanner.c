/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc text scanner document

	@module	scanner.c | text scanner implement file

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

#include "scanner.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_VIEW

void scan_object_text(if_measure_t* pif, const xfont_t* pxf, const xface_t* pxa, long bx, long by, long bw, long bh, if_wordscan_t* pit, PF_SCAN_TEXTOR_CALLBACK pf, void* pp)
{
	float line_rati;
	bool_t b_wordbreak = 0;
	xsize_t xs = { 0 };
	xfont_t xf_atom = { 0 };

	tchar_t sch[4] = { 0 };
	tchar_t* pch = NULL;
	int chs;

	int ts, to;
	bool_t b_newline = 0;
	bool_t b_newpage = 0;
	bool_t b_cancel = 0;
	bool_t b_paging = 0;

	bool_t b_atom = 0;
	bool_t b_ins = 0;
	bool_t b_del = 0;
	bool_t b_sel = 0;

	void* obj = NULL;
	int row_at = 0;
	int col_at = 0;
	int page = 0;

	WORDPLACE tm = { 0 };

	b_wordbreak = (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0) ? 1 : 0;

	xmem_copy((void*)&xf_atom, (void*)pxf, sizeof(xfont_t));
	xscpy(xf_atom.weight, GDI_ATTR_FONT_WEIGHT_BOLD);

	if (is_null(pxa->line_height))
		line_rati = xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = xstof(pxa->line_height);

	if (line_rati < 1)
		line_rati = 1.0;

	(*pif->pf_text_metric)(pif->ctx, pxf, &xs);

	tm.char_w = (xs.cx + 4) / 4;
	tm.char_h = xs.cy;
	tm.line_h = (long)((float)xs.cy * (line_rati - 1.0));
	tm.min_x = bx;
	tm.min_y = by;
	tm.max_x = bx + bw;
	tm.max_y = by + bh;
	tm.cur_x = tm.min_x + tm.char_w;
	tm.cur_y = tm.min_y + tm.line_h;

	b_paging = (*pit->pf_is_paging)(pit->param);

	to = (*pf)(_SCANNER_STATE_BEGIN, NULL, 0, 0, 0, 0, NULL, 0, NULL, 0, -1, 0, &tm, pxf, pxa, pp);

	if (to != _SCANNER_OPERA_STOP)
	{
		if (b_paging)
			to = _SCANNER_OPERA_PAGED;
		else
			to = _SCANNER_OPERA_NEXT;
	}

	while (to != _SCANNER_OPERA_STOP)
	{
		switch (to)
		{
		case _SCANNER_OPERA_PAGED:
			while (to == _SCANNER_OPERA_PAGED)
			{
				page = (*pit->pf_next_page)(pit->param);
				if (!page)
				{
					to = _SCANNER_OPERA_STOP;
					break;
				}

				to = (*pf)(_SCANNER_STATE_CATOR, NULL, 0, 0, 0, 0, NULL, 0, NULL, page, 0, 0, &tm, pxf, pxa, pp);
			}	
			continue;

			break;
		case _SCANNER_OPERA_NEXT:
			if (b_newline)
			{
				b_newline = 0;
				break;
			}
			if (b_newpage)
			{
				b_newpage = 0;
				break;
			}

			obj = NULL;
			pch = NULL;
			chs = (*pit->pf_next_word)(pit->param, &pch, &xs, &b_ins, &b_del, &b_sel, &b_atom);
			if (!chs)
			{
				b_cancel = 1;
				break;
			}
			
			if (b_atom)
			{
				chs = xslen(pch);
			}

			(*pit->pf_cur_object)(pit->param, &obj);

			break;
		case _SCANNER_OPERA_INS:
			if ((*pit->pf_insert_word)(pit->param, sch))
			{
				to = _SCANNER_OPERA_NEXT;
			}
			else
			{
				to = _SCANNER_OPERA_STOP;
			}
			continue;
		case _SCANNER_OPERA_DEL:
			if((*pit->pf_delete_word)(pit->param))
			{
				to = _SCANNER_OPERA_NEXT;
			}
			else
			{
				to = _SCANNER_OPERA_STOP;
			}
			continue;
		}

		if (b_cancel)
		{
			b_cancel = 0;

			chs = 0;
			sch[0] = 0;
			ts = _SCANNER_STATE_END;
		}
		else if (pch && *pch == _T('\r'))
		{
			chs = 0;
			xsncpy(sch, _T("\r"), 1);

			ts = _SCANNER_STATE_NEWLINE;
		}
		else if (pch && *pch == _T('\v'))
		{
			chs = 0;
			xsncpy(sch, _T("\v"), 1);

			ts = _SCANNER_STATE_NEWPAGE;
		}
		else if (pch && *pch == _T('\n'))
		{
			tm.cur_w = tm.char_w * 2;
			tm.cur_h = tm.char_h;

			chs = 1;
			xsncpy(sch, _T("\n"), chs);

			ts = _SCANNER_STATE_LINEBREAK;
		}
		else if (pch && *pch == _T('\f'))
		{
			tm.cur_w = tm.char_w * 2;
			tm.cur_h = tm.char_h;

			if (b_paging)
			{
				chs = 1;
				xsncpy(sch, _T("\f"), chs);

				ts = _SCANNER_STATE_PAGEBREAK;
			}
			else
			{
				chs = 1;
				xsncpy(sch, _T("\f"), chs);

				ts = _SCANNER_STATE_LINEBREAK;
			}
		}
		else
		{
			if (!b_atom && pch && *pch == _T('\t'))
			{
				tm.cur_w = tm.char_w * 4;
				tm.cur_h = tm.char_h;
			}else if (!b_atom && pch && *pch == _T('\0'))
			{
				tm.cur_w = tm.char_w;
				tm.cur_h = tm.char_h;
			}
			else
			{
				if (!chs)
				{
					tm.cur_w = tm.char_w;
					tm.cur_h = tm.char_h;
				}
				else
				{
					//if (b_atom)
					//	(*pif->pf_text_size)(pif->ctx, &xf_atom, pch, chs, &xs);
					//else
					//	(*pif->pf_text_size)(pif->ctx, pxf, pch, chs, &xs);

					tm.cur_w = xs.cx;
					tm.cur_h = tm.char_h;
				}
			}

			ts = _SCANNER_STATE_WORDS;
		}

		sch[0] = 0;

		switch (ts)
		{
		case _SCANNER_STATE_WORDS:
		case _SCANNER_STATE_LINEBREAK:
		case _SCANNER_STATE_PAGEBREAK:
			if (b_atom)
				to = (*pf)(ts, obj, b_atom, b_ins, b_del, b_sel, pch, chs, sch, page, row_at, col_at, &tm, &xf_atom, pxa, pp);
			else
				to = (*pf)(ts, obj, b_atom, b_ins, b_del, b_sel, pch, chs, sch, page, row_at, col_at, &tm, pxf, pxa, pp);
			break;
		case _SCANNER_STATE_NEWLINE:
			to = (*pf)(ts, obj, 0, 0, 0, 0, NULL, chs, NULL, page, row_at, col_at, &tm, pxf, pxa, pp);
			break;
		case _SCANNER_STATE_NEWPAGE:
			to = (*pf)(ts, obj, 0, 0, 0, 0, NULL, chs, NULL, page, row_at, col_at, &tm, pxf, pxa, pp);
			break;
		case _SCANNER_STATE_END:
			to = (*pf)(ts, obj, b_atom, b_ins, b_del, b_sel, NULL, chs, sch, page, row_at, col_at, &tm, pxf, pxa, pp);
			if (to != _SCANNER_OPERA_INS)
				to = _SCANNER_OPERA_STOP;
			break;
		}

		if (to != _SCANNER_OPERA_NEXT)
			continue;

		switch (ts)
		{
		case _SCANNER_STATE_WORDS:
			tm.cur_x += tm.cur_w;
			tm.cur_w = 0;

			col_at++;

			if (b_paging && (tm.cur_x - tm.char_w > tm.min_x) && (tm.cur_x + tm.char_w > tm.max_x) && (tm.cur_y + 2 * (tm.cur_h + tm.line_h) > tm.max_y))
			{
				ts = _SCANNER_STATE_NEWPAGE;
				b_newpage = 1;
				xsncpy(sch, _T("\v"), 1);
				pch = sch;
			}
			else if (b_wordbreak && (tm.cur_x - tm.char_w > tm.min_x) && (tm.cur_x + tm.char_w > tm.max_x))
			{
				ts = _SCANNER_STATE_NEWLINE;
				b_newline = 1;
				xsncpy(sch, _T("\r"), 1);
				pch = sch;
			}
			break;
		case _SCANNER_STATE_LINEBREAK:
			tm.cur_x += tm.cur_w;
			tm.cur_w = 0;

			col_at++;

			if (b_paging && (tm.cur_y + 2 * (tm.cur_h + tm.line_h) > tm.max_y))
			{
				ts = _SCANNER_STATE_NEWPAGE;
				b_newpage = 1;
				xsncpy(sch, _T("\v"), 1);
				pch = sch;
			}
			else if (!is_null(pxa->text_wrap))
			{
				ts = _SCANNER_STATE_NEWLINE;
				b_newline = 1;
				xsncpy(sch, _T("\r"), 1);
				pch = sch;
			}
			break;
		case _SCANNER_STATE_PAGEBREAK:
			tm.cur_x += tm.cur_w;
			tm.cur_w = 0;

			col_at++;

			b_newpage = 1;
			xsncpy(sch, _T("\v"), 1);
			pch = sch;

			break;
		case _SCANNER_STATE_NEWLINE:
			tm.cur_y += (tm.cur_h + tm.line_h);
			tm.cur_x = tm.min_x + tm.char_w;
			tm.cur_h = 0;

			row_at++;
			col_at = 0;

			break;
		case _SCANNER_STATE_NEWPAGE:
			if ((*pit->pf_break_page)(pit->param))
			{
				page = (*pit->pf_next_page)(pit->param);
				
				tm.cur_x = tm.min_x + tm.char_w;
				tm.cur_y = tm.min_y + tm.line_h;

				row_at = 0;
				col_at = 0;

				to = (*pf)(_SCANNER_STATE_CATOR, NULL, 0, 0, 0, 0, NULL, 0, NULL, page, row_at, col_at, &tm, pxf, pxa, pp);
			}
			break;
		}
	}
}

#endif