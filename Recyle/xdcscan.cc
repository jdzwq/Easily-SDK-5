/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text scanner document

	@module	xdcscan.c | text scanner implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
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

#include "xdcscan.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcgdi.h"

void scan_fix_text(res_dc_t rdc, const tchar_t* sz_text, int sz_len, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp)
{
	long text_height, char_width;
	int row_at, col_at;
	float line_rati;
	tchar_t sch[2] = { 0 };
	int sz_at, n_total = 0;
	int ts, to;
	xsize_t xs;
	xcolor_t xc = { 0 };

	TEXTMEASURE tm = { 0 };

	if (sz_len < 0)
		sz_len = xslen(sz_text);
	sz_at = 0;

	parse_xcolor(&xc, pxa->text_color);

	if (is_null(pxa->line_height))
		line_rati = (float)xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = (float)xstof(pxa->line_height);

	text_metric_raw(rdc, pxf, &text_height, NULL, &char_width, NULL);

	tm.lh = (long)((float)text_height * (line_rati - 1.0));
	tm.lw = char_width;

	tm.min_x = pbox->x;
	tm.max_x = pbox->x + pbox->w;
	tm.min_y = pbox->y;
	tm.max_y = pbox->y + pbox->h;
	tm.x = pbox->x + tm.lw;
	tm.y = pbox->y + tm.lh;

	to = (*pf)(_TEXTOR_SCAN_BEGIN, NULL, 0, 0, 0, 0, NULL, NULL, 0, -1, n_total, &tm, pxf, &xc, pp);

	row_at = col_at = 0;

	while (to != _TEXTOR_OPERA_STOP)
	{
		if (sz_at == sz_len)
		{
			sch[0] = 0;
			ts = _TEXTOR_SCAN_END;
		}
		else
		{
			if (sz_text[sz_at] == _T('\t'))
			{
				tm.cx = char_width * 4;
				tm.cy = text_height;

				if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
				{
					sch[0] = 0;
					ts = _TEXTOR_SCAN_RETURN;
				}
				else
				{
					n_total++;
					sch[0] = sz_text[sz_at];
					ts = _TEXTOR_SCAN_WORDS;
				}
			}
			else if (sz_text[sz_at] == _T('\n'))
			{
				tm.cx = char_width;
				tm.cy = text_height;

				n_total++;

				if (is_null(pxa->text_wrap))
				{
					sch[0] = _T('\n');
					ts = _TEXTOR_SCAN_WORDS;
				}
				else
				{
					sch[0] = _T('\n');
					ts = _TEXTOR_SCAN_CARRIAGE;
				}
			}
			else
			{
				text_size_raw(rdc, pxf, sz_text + sz_at, 1, &xs);
				tm.cx = xs.cx;
				tm.cy = xs.cy;
				if (tm.cy > text_height)
				{
					text_height = tm.cy;
				}

				if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
				{
					sch[0] = 0;
					ts = _TEXTOR_SCAN_RETURN;
				}
				else
				{
					n_total++;
					sch[0] = sz_text[sz_at];
					ts = _TEXTOR_SCAN_WORDS;
				}
			}
		}

		switch (ts)
		{
		case _TEXTOR_SCAN_RETURN:
			to = (*pf)(ts, NULL, 0, 0, 0, 0, NULL, NULL, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_WORDS:
			to = (*pf)(ts, NULL, 0, 0, 0, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_CARRIAGE:
			to = (*pf)(ts, NULL, 0, 0, 0, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_END:
			to = (*pf)(ts, NULL, 0, 0, 0, 0, NULL, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		}

		if (to != _TEXTOR_OPERA_NEXT)
			continue;

		switch (ts)
		{
		case _TEXTOR_SCAN_RETURN:
			row_at++;
			col_at = 0;
			tm.y += (tm.cy + tm.lh);
			tm.x = pbox->x + tm.lw;
			tm.cy = 0;
			break;
		case _TEXTOR_SCAN_WORDS:
			sz_at++;
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_CARRIAGE:
			sz_at++;
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_END:
			to = _TEXTOR_OPERA_STOP;
			break;
		}
	}
}

void scan_var_text(res_dc_t rdc, void* data, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp)
{
	long text_height, char_width;
	int row_at, col_at;
	float line_rati;
	const tchar_t* sz_text;
	tchar_t sch[2] = { 0 };
	int sz_at, sz_len, n_total = 0;
	int ts, to;
	bool_t b_newline = 0;
	xsize_t xs;
	xcolor_t xc = { 0 };

	xhand_t vs;

	TEXTMEASURE tm = { 0 };

	vs = (xhand_t)data;

	sz_text = varstr_ptr(vs);
	sz_len = varstr_len(vs);
	sz_at = 0;

	parse_xcolor(&xc, pxa->text_color);

	if (is_null(pxa->line_height))
		line_rati = (float)xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = (float)xstof(pxa->line_height);

	text_metric_raw(rdc, pxf, &text_height, NULL, &char_width, NULL);

	tm.lh = (long)((float)text_height * (line_rati - 1.0));
	tm.lw = char_width;

	tm.x = pbox->x + tm.lw;
	tm.y = pbox->y + tm.lh;
	tm.min_x = pbox->x;
	tm.max_x = pbox->x + pbox->w;
	tm.min_y = pbox->y;
	tm.max_y = pbox->y + pbox->h;

	to = (*pf)(_TEXTOR_SCAN_BEGIN, NULL, 0, 0, 0, 0, NULL, NULL, 0, -1, n_total, &tm, pxf, &xc, pp);

	row_at = col_at = 0;

	while (to != _TEXTOR_OPERA_STOP)
	{
		switch (to)
		{
		case _TEXTOR_OPERA_NEXT:
			break;
		case _TEXTOR_OPERA_INS:
			sz_len = varstr_ins_char(vs, sz_at, sch[0]);

			to = _TEXTOR_OPERA_NEXT;
			continue;

			break;
		case _TEXTOR_OPERA_DEL:
			sz_len = varstr_del_char(vs, sz_at);
			
			to = _TEXTOR_OPERA_NEXT;
			continue;

			break;
		}

		if (b_newline)
		{
			sch[0] = 0;
			ts = _TEXTOR_SCAN_RETURN;

			b_newline = 0;
		}
		else if (sz_at == sz_len)
		{
			sch[0] = 0;
			ts = _TEXTOR_SCAN_END;
		}
		else
		{
			if (sz_text[sz_at] == _T('\t'))
			{
				tm.cx = char_width * 4;
				tm.cy = text_height;

				if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
				{
					sch[0] = 0;
					ts = _TEXTOR_SCAN_RETURN;
				}
				else
				{
					n_total++;
					sch[0] = sz_text[sz_at];
					ts = _TEXTOR_SCAN_WORDS;
				}
			}
			else if (sz_text[sz_at] == _T('\n'))
			{
				tm.cx = char_width;
				tm.cy = text_height;

				n_total++;

				if (is_null(pxa->text_wrap))
				{
					sch[0] = _T('\n');
					ts = _TEXTOR_SCAN_WORDS;
				}
				else
				{
					b_newline = 1;

					sch[0] = _T('\n');
					ts = _TEXTOR_SCAN_CARRIAGE;
				}
			}
			else
			{
				text_size_raw(rdc, pxf, sz_text + sz_at, 1, &xs);
				tm.cx = xs.cx;
				tm.cy = xs.cy;
				if (tm.cy > text_height)
				{
					text_height = tm.cy;
				}

				if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
				{
					sch[0] = 0;
					ts = _TEXTOR_SCAN_RETURN;
				}
				else
				{
					n_total++;
					sch[0] = sz_text[sz_at];
					ts = _TEXTOR_SCAN_WORDS;
				}
			}
		}

		switch (ts)
		{
		case _TEXTOR_SCAN_RETURN:
			to = (*pf)(ts, NULL, 0, 0, 0, 0, NULL, NULL, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_WORDS:
			to = (*pf)(ts, NULL, 0, 1, 1, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_CARRIAGE:
			to = (*pf)(ts, NULL, 0, 1, 1, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_END:
			to = (*pf)(ts, NULL, 0, 1, 0, 0, NULL, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		}

		if (to != _TEXTOR_OPERA_NEXT)
			continue;

		switch (ts)
		{
		case _TEXTOR_SCAN_RETURN:
			row_at++;
			col_at = 0;
			tm.y += (tm.cy + tm.lh);
			tm.x = pbox->x + tm.lw;
			tm.cy = 0;
			break;
		case _TEXTOR_SCAN_WORDS:
			sz_at++;
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_CARRIAGE:
			sz_at++;
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_END:
			to = _TEXTOR_OPERA_STOP;
			break;
		}
	}
}


void scan_tag_text(res_dc_t rdc, void* data, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp)
{
	long text_height, char_width;
	int row_at, col_at;
	float line_rati;
	const tchar_t* sz_text;
	tchar_t sch[2] = { 0 };
	int sz_at, sz_len, n_total = 0;
	int ts, to;
	xsize_t xs;

	xfont_t xf_alter = { 0 };
	xcolor_t xc,xc_alter = { 0 };

	link_t_ptr tag, nxt, nlk = NULL;
	bool_t b_newtag = 0;
	bool_t b_alter = 0;

	TEXTMEASURE tm = { 0 };

	tag = (link_t_ptr)data;

	memcpy((void*)&xf_alter, (void*)pxf, sizeof(xfont_t));
	xscpy(xf_alter.style, GDI_ATTR_FONT_STYLE_UNDERLINE);

	parse_xcolor(&xc, pxa->text_color);
	parse_xcolor(&xc_alter, pxa->text_color);
	lighten_xcolor(&xc_alter, DEF_SOFT_LIGHTEN);
	
	if (is_null(pxa->line_height))
		line_rati = (float)xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = (float)xstof(pxa->line_height);

	text_metric_raw(rdc, pxf, &text_height, NULL, &char_width, NULL);

	tm.lh = (long)((float)text_height * (line_rati - 1.0));
	tm.lw = char_width;

	tm.x = pbox->x + tm.lw;
	tm.y = pbox->y + tm.lh;
	tm.min_x = pbox->x;
	tm.max_x = pbox->x + pbox->w;
	tm.min_y = pbox->y;
	tm.max_y = pbox->y + pbox->h;

	to = (*pf)(_TEXTOR_SCAN_BEGIN, NULL, 0, 0, 0, 0, NULL, NULL, 0, -1, n_total, &tm, pxf, &xc, pp);

	row_at = col_at = 0;
	b_newtag = 1;

	while (to != _TEXTOR_OPERA_STOP)
	{
		switch (to)
		{
		case _TEXTOR_OPERA_NEXT:
			if (b_newtag)
			{
				nlk = (nlk) ? get_tag_next_node(tag, nlk) : get_tag_next_node(tag, LINK_FIRST);
				if (nlk)
				{
					if (is_tag_text_node(nlk))
					{
						sz_text = get_tag_node_text_ptr(nlk);
						b_alter = 0;
					}
					else
					{
						sz_text = get_tag_node_text_ptr(nlk);
						if (is_null(sz_text))
						{
							sz_text = get_tag_node_alter_ptr(nlk);
						}
						b_alter = 1;
					}

					sz_len = xslen(sz_text);
					sz_at = 0;
				}
				else
				{
					b_alter = 0;
					sz_text = NULL;
					sz_len = sz_at = 0;
				}
			}
			break;
		case _TEXTOR_OPERA_INS:
			if (!nlk)
			{
				nlk = insert_tag_node(tag, LINK_LAST);
				sz_text = get_tag_node_text_ptr(nlk);
				sz_len = xslen(sz_text);
				sz_at = 0;
			}

			if (!b_alter)
			{
				sz_text = tag_node_text_ins_char(nlk, sz_at, sch[0]);
				sz_len++;
			}

			to = _TEXTOR_OPERA_NEXT;
			continue;

			break;
		case _TEXTOR_OPERA_DEL:
			if (b_alter)
			{
				sz_text = get_tag_node_text_ptr(nlk);
				if (!is_null(sz_text))
				{
					set_tag_node_text(nlk, NULL, 0);
					sz_text = get_tag_node_alter_ptr(nlk);
					sz_len = xslen(sz_text);
				}
				else
				{
					sz_text = NULL;
					sz_len = 0;
				}
			}
			else
			{
				sz_text = tag_node_text_del_char(nlk, sz_at);
				sz_len--;
			}

			if (is_null(sz_text))
			{
				nxt = nlk;
				nlk = get_tag_prev_node(tag, nlk);

				delete_tag_node(nxt);

				sz_len = 0;
				sz_at = 0;
			}

			to = _TEXTOR_OPERA_NEXT;
			continue;

			break;
		}

		if (b_newtag)
		{
			if (nlk)
			{
				sch[0] = 0;
				ts = _TEXTOR_SCAN_OBJECT_BEGIN;
			}
			else
			{
				sch[0] = 0;
				ts = _TEXTOR_SCAN_END;
			}
			b_newtag = 0;
		}
		else if (b_alter)
		{
			text_size_raw(rdc, pxf, sz_text, sz_len, &xs);
			tm.cx = xs.cx;
			tm.cy = xs.cy;
			if (tm.cy > text_height)
			{
				text_height = tm.cy;
			}

			if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
			{
				sch[0] = 0;
				ts = _TEXTOR_SCAN_RETURN;
			}
			else
			{
				n_total++;
				sch[0] = _T(' ');
				ts = _TEXTOR_SCAN_WORDS;
			}
		}
		else if (sz_at == sz_len)
		{
			sch[0] = 0;
			ts = _TEXTOR_SCAN_OBJECT_END;
		}
		else
		{
			if (sz_text[sz_at] == _T('\t'))
			{
				tm.cx = char_width * 4;
				tm.cy = text_height;

				if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
				{
					sch[0] = 0;
					ts = _TEXTOR_SCAN_RETURN;
				}
				else
				{
					n_total++;
					sch[0] = sz_text[sz_at];
					ts = _TEXTOR_SCAN_WORDS;
				}
			}
			else if (sz_text[sz_at] == _T('\n'))
			{
				tm.cx = char_width;
				tm.cy = text_height;

				n_total++;

				if (is_null(pxa->text_wrap))
				{
					sch[0] = _T('\n');
					ts = _TEXTOR_SCAN_WORDS;
				}
				else
				{
					sch[0] = _T('\n');
					ts = _TEXTOR_SCAN_CARRIAGE;
				}
			}
			else
			{
				text_size_raw(rdc, pxf, sz_text + sz_at, 1, &xs);
				tm.cx = xs.cx;
				tm.cy = xs.cy;
				if (tm.cy > text_height)
				{
					text_height = tm.cy;
				}

				if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
				{
					sch[0] = 0;
					ts = _TEXTOR_SCAN_RETURN;
				}
				else
				{
					n_total++;
					sch[0] = sz_text[sz_at];
					ts = _TEXTOR_SCAN_WORDS;
				}
			}
		}

		switch (ts)
		{
		case _TEXTOR_SCAN_RETURN:
			to = (*pf)(ts, nlk, 0, 0, 0, 0, NULL, NULL, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_OBJECT_BEGIN:
			to = (*pf)(ts, nlk, 0, 0, 0, 0, NULL, NULL, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_WORDS:
			if (b_alter)
				to = (*pf)(ts, nlk, 0, 1, 1, 1, sz_text + sz_at, sch, row_at, col_at, n_total, &tm, &xf_alter, &xc_alter, pp);
			else
				to = (*pf)(ts, nlk, 0, 1, 1, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_CARRIAGE:
			to = (*pf)(ts, nlk, 0, 1, 1, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_OBJECT_END:
			to = (*pf)(ts, nlk, 0, 1, 0, 0, NULL, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_END:
			to = (*pf)(ts, NULL, 0, 1, 0, 0, NULL, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		}

		if (to != _TEXTOR_OPERA_NEXT)
			continue;

		switch (ts)
		{
		case _TEXTOR_SCAN_RETURN:
			row_at++;
			col_at = 0;
			tm.y += (tm.cy + tm.lh);
			tm.x = pbox->x + tm.lw;
			tm.cy = 0;
			break;
		case _TEXTOR_SCAN_OBJECT_BEGIN:
			b_newtag = 0;
			break;
		case _TEXTOR_SCAN_WORDS:
			if (b_alter)
			{
				sz_at += sz_len;
				b_alter = 0;
			}
			else
			{
				sz_at++;
			}
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_CARRIAGE:
			sz_at++;
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_OBJECT_END:
			b_alter = 0;
			b_newtag = 1;
			break;
		case _TEXTOR_SCAN_END:
			to = _TEXTOR_OPERA_STOP;
			break;
		}
	}
}

void scan_text_doc(res_dc_t rdc, void* data, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pbox, PF_SCAN_TEXT_CALLBACK pf, void* pp)
{
	long text_height, char_width;
	int row_at, col_at;
	float line_rati;
	const tchar_t* sz_text;
	tchar_t sch[2] = { 0 };
	int sz_ind, sz_at, sz_len, n_tabs, n_total = 0;
	int ts, to;
	xsize_t xs;

	xcolor_t xc = { 0 };

	link_t_ptr txt, nxt, nlk = NULL;
	int n_indent = 0;
	bool_t b_newline = 0;
	bool_t b_return = 0;

	TEXTMEASURE tm = { 0 };

	txt = (link_t_ptr)data;

	parse_xcolor(&xc, pxa->text_color);

	if (is_null(pxa->line_height))
		line_rati = (float)xstof(DEF_GDI_TEXT_LINE_HEIGHT);
	else
		line_rati = (float)xstof(pxa->line_height);

	text_metric_raw(rdc, pxf, &text_height, NULL, &char_width, NULL);

	tm.lh = (long)((float)text_height * (line_rati - 1.0));
	tm.lw = char_width;

	tm.x = pbox->x + tm.lw;
	tm.y = pbox->y + tm.lh;
	tm.min_x = pbox->x;
	tm.max_x = pbox->x + pbox->w;
	tm.min_y = pbox->y;
	tm.max_y = pbox->y + pbox->h;

	to = (*pf)(_TEXTOR_SCAN_BEGIN, NULL, 0, 0, 0, 0, NULL, NULL, 0, -1, n_total, &tm, pxf, &xc, pp);

	row_at = col_at = 0;
	b_newline = 1;

	while (to != _TEXTOR_OPERA_STOP)
	{
		switch (to)
		{
		case _TEXTOR_OPERA_NEXT:
			if (b_newline)
			{
				nlk = (nlk) ? get_text_next_visible_line(txt, nlk) : get_text_next_visible_line(txt, LINK_FIRST);
				if (nlk)
				{
					sz_text = get_text_line_text_ptr(nlk);
					sz_len = xslen(sz_text);
					sz_at = 0;

					n_indent = get_text_line_indent(nlk);

					if (nlk != get_text_next_visible_line(txt, LINK_FIRST))
					{
						b_return = 1;
					}
				}
				else
				{
					sz_text = NULL;
					sz_len = sz_at = 0;

					n_indent = 0;
				}
			}
			break;
		case _TEXTOR_OPERA_INS:
			if (!nlk)
			{
				nlk = insert_text_line(txt, LINK_LAST);

				sz_text = get_text_line_text_ptr(nlk);
				sz_len = xslen(sz_text);
				sz_at = 0;

				n_indent = get_text_line_indent(nlk);
			}

			if (sch[0] == _T('\n'))
			{
				nxt = insert_text_line(txt, nlk);
				if (sz_len)
				{
					n_tabs = 0;
					while (*(sz_text + sz_at + n_tabs) == _T('\t'))
					{
						n_tabs++;
					}
					set_text_line_indent(nxt, n_tabs);
					set_text_line_text(nxt, sz_text + sz_at + n_tabs, sz_len - sz_at - n_tabs);
				}

				while (sz_at < sz_len)
				{
					sz_text = text_line_text_del_char(nlk, sz_at);
					sz_len--;
				}
			}
			else if (sch[0] == _T('\t') && !sz_at)
			{
				set_text_line_indent(nlk, get_text_line_indent(nlk) + 1);
				n_indent++;
			}
			else
			{
				if (n_indent)
				{
					set_text_line_indent(nlk, get_text_line_indent(nlk) - n_indent);
					n_indent = 0;
				}

				sz_text = text_line_text_ins_char(nlk, sz_at, sch[0]);
				sz_len++;
			}

			to = _TEXTOR_OPERA_NEXT;
			continue;

			break;
		case _TEXTOR_OPERA_DEL:
			if (n_indent)
			{
				set_text_line_indent(nlk, get_text_line_indent(nlk) - 1);
				n_indent--;
			}
			else if (sz_at == sz_len)
			{
				nxt = get_text_next_visible_line(txt, nlk);
				if (nlk && nxt)
				{
					sz_ind = sz_at;
					sz_at = sz_len;

					n_indent = get_text_line_indent(nxt);
					while (n_indent)
					{
						text_line_text_ins_char(nlk, sz_ind, _T('\t'));
						sz_ind++;
						n_indent--;
					}

					sz_text = get_text_line_text_ptr(nxt);
					while (sz_text && *sz_text != _T('\0'))
					{
						text_line_text_ins_char(nlk, sz_ind, *sz_text);
						sz_ind++;
						sz_text++;
					}
					sz_len = sz_ind;
					sz_text = get_text_line_text_ptr(nlk);

					delete_text_line(nxt);
				}
				else if (nxt)
				{
					nlk = nxt;
					sz_text = get_text_line_text_ptr(nlk);
					sz_len = xslen(sz_text);
					sz_at = 0;

					n_indent = get_text_line_indent(nlk);
				}
			}
			else
			{
				sz_text = text_line_text_del_char(nlk, sz_at);
				sz_len--;

				if (is_null(sz_text))
				{
					nxt = nlk;
					nlk = get_text_prev_visible_line(txt, nlk);

					delete_text_line(nxt);

					sz_text = (nlk)? get_text_line_text_ptr(nlk) : NULL;
					sz_len = xslen(sz_text);
					sz_at = sz_len;
				}
				else
				{
					while (*sz_text == _T('\t'))
					{
						sz_text = text_line_text_del_char(nlk, 0);
						sz_len--;

						set_text_line_indent(nlk, get_text_line_indent(nlk) + 1);
						n_indent++;

						if (!sz_len)
							break;
					}
				}
			}

			to = _TEXTOR_OPERA_NEXT;
			continue;

			break;
		}

		if (b_newline && b_return)
		{
			sch[0] = 0;
			ts = _TEXTOR_SCAN_RETURN;

			b_newline = 0;
		}
		else if (b_newline || b_return)
		{
			if (nlk)
			{
				sch[0] = 0;
				ts = _TEXTOR_SCAN_OBJECT_BEGIN;
			}
			else
			{
				sch[0] = 0;
				ts = _TEXTOR_SCAN_END;
			}

			b_return = 0;
			b_newline = 0;
		}
		else if (n_indent)
		{
			tm.cx = char_width * 2;
			tm.cy = text_height;

			n_total++;

			sch[0] = _T('\t');
			ts = _TEXTOR_SCAN_INDENT;
		}
		else
		{
			if (sz_at == sz_len && get_text_next_visible_line(txt, nlk) != NULL)
			{
				tm.cx = char_width;
				tm.cy = text_height;

				n_total++;

				sch[0] = _T('\n');
				ts = _TEXTOR_SCAN_CARRIAGE;
			}
			else if (sz_at >= sz_len)
			{
				sch[0] = 0;
				ts = _TEXTOR_SCAN_OBJECT_END;
			}
			else
			{
				if (sz_text[sz_at] == _T('\t'))
				{
					tm.cx = char_width * 4;
					tm.cy = text_height;

					if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
					{
						sch[0] = 0;
						ts = _TEXTOR_SCAN_RETURN;
					}
					else
					{
						n_total++;
						sch[0] = sz_text[sz_at];
						ts = _TEXTOR_SCAN_WORDS;
					}
				}
				else
				{
					text_size_raw(rdc, pxf, sz_text + sz_at, 1, &xs);
					tm.cx = xs.cx;
					tm.cy = xs.cy;
					if (tm.cy > text_height)
					{
						text_height = tm.cy;
					}

					if (compare_text(pxa->text_wrap, -1, GDI_ATTR_TEXT_WRAP_WORDBREAK, -1, 0) == 0 && tm.x > tm.min_x + tm.lw && tm.x + tm.cx > tm.max_x - tm.lw)
					{
						sch[0] = 0;
						ts = _TEXTOR_SCAN_RETURN;
					}
					else
					{
						n_total++;
						sch[0] = sz_text[sz_at];
						ts = _TEXTOR_SCAN_WORDS;
					}
				}	
			}
		}

		switch (ts)
		{
		case _TEXTOR_SCAN_RETURN:
			to = (*pf)(ts, nlk, 0, 0, 0, 0, NULL, NULL, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_OBJECT_BEGIN:
			to = (*pf)(ts, nlk, 0, 0, 0, 0, NULL, NULL, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_INDENT:
			to = (*pf)(ts, nlk, 0, 1, 1, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_WORDS:
			to = (*pf)(ts, nlk, 0, 1, 1, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_CARRIAGE:
			to = (*pf)(ts, nlk, 0, 1, 1, 1, sch, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_OBJECT_END:
			to = (*pf)(ts, nlk, 0, 1, 0, 0, NULL, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		case _TEXTOR_SCAN_END:
			to = (*pf)(ts, NULL, 0, 1, 0, 0, NULL, sch, row_at, col_at, n_total, &tm, pxf, &xc, pp);
			break;
		}

		if (to != _TEXTOR_OPERA_NEXT)
			continue;

		switch (ts)
		{
		case _TEXTOR_SCAN_RETURN:
			row_at++;
			col_at = 0;
			tm.y += (tm.cy + tm.lh);
			tm.x = pbox->x + tm.lw;
			tm.cy = 0;
			break;
		case _TEXTOR_SCAN_OBJECT_BEGIN:
			b_newline = 0;
			break;
		case _TEXTOR_SCAN_INDENT:
			n_indent--;
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_WORDS:
			sz_at++;
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_CARRIAGE:
			sz_at++;
			col_at++;
			tm.x += tm.cx;
			tm.cx = 0;
			break;
		case _TEXTOR_SCAN_OBJECT_END:
			b_newline = 1;
			break;
		case _TEXTOR_SCAN_END:
			to = _TEXTOR_OPERA_STOP;
			break;
		}
	}
}
