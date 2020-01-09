/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc text control document

	@module	textctrl.c | text control widnows implement file

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

#include "xdcctrl.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcclip.h"
#include "xdceditor.h"
#include "xdcgdi.h"

typedef enum{TEXT_DRAG_FIRST = 1,TEXT_DRAG_LAST = 2}_TEXTDRAG;

typedef struct _TAGTEXT_UNDO{
	int cur_row, cur_col;
	int sel_row, sel_col;
	bool_t b_select;
	tchar_t* text;

	_TAGTEXT_UNDO* next;
}TAGTEXT_UNDO;

typedef struct _textctrl_delta_t{
	LINKPTR data;

	int cur_row, cur_col;
	int sel_row, sel_col;
	bool_t b_select;
	int n_drag;

	TAGTEXT_UNDO* ptu;

	res_dc_t rdc;
}textctrl_delta_t;

#define GETTEXTCTRLDELTA(ph) 	(textctrl_delta_t*)widget_get_user_delta(ph)
#define SETTEXTCTRLDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

/***************************************************************************************/
typedef struct _TAGTEXTCALC{
	bool_t done;
	long x, y;
	long width, height;
	int from_row, from_col,to_row,to_col;
	xpoint_t from_pt, to_pt;
	LINKPTR nlk;
	tchar_t* buf;
	int total,max;

	res_dc_t rdc;
	long offx, offy;
}TAGTEXTCALC;

int _tag_text_calc_maxing(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;

	switch (scan)
	{
	case _TEXT_SCAN_BEGIN:
		ptt->from_row = cur_row;
		ptt->from_col = cur_col;
		ptt->height = cur_height;
		ptt->width = cur_width;
		break;
	case _TEXT_SCAN_END:
		ptt->to_row = cur_row;
		ptt->to_col = cur_col;
		ptt->height = cur_height + pse->cy + line_height;
		break;
	case _TEXT_SCAN_CHAR:
	case _TEXT_SCAN_SOFT_BREAK:
	case _TEXT_SCAN_HARD_BREAK:
		if (ptt->width < cur_width + line_width)
			ptt->width = cur_width + line_width;
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _textctrl_calc_maxing(res_win_t widget, long* max_width, long* max_height, int* max_row, int* max_col)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;

	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	scan_tag_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_maxing, (void*)&tt);

	if (max_width)
		*max_width = tt.width;
	if (max_height)
		*max_height = tt.height;
	if (max_row)
		*max_row = tt.to_row;
	if (max_col)
		*max_col = tt.to_col;
}

int _tag_text_calc_hint(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;

	switch (scan)
	{
	case _TEXT_SCAN_CHAR:
		if (ptt->y >= cur_height && ptt->y < cur_height + pse->cy + line_height && ptt->x >= cur_width && ptt->x < cur_width + pse->cx)
		{
			ptt->nlk = cur_node;
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	case _TEXT_SCAN_HARD_BREAK:
	case _TEXT_SCAN_SOFT_BREAK:
		if (ptt->y >= cur_height && ptt->y < cur_height + pse->cy + line_height)
		{
			ptt->nlk = cur_node;
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	case _TEXT_SCAN_END:
		if (!ptt->done)
		{
			ptt->nlk = NULL;
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			return _TEXT_OPERA_STOP;
		}
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _textctrl_calc_hint(res_win_t widget, long x, long y, link_t_ptr* pilk, int* row_at, int* col_at)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;
	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.x = x + vb.bx;
	tt.y = y + vb.by;
	scan_tag_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_hint, (void*)&tt);

	if (pilk)
		*pilk = tt.nlk;
	if (row_at)
		*row_at = tt.from_row;
	if (col_at)
		*col_at = tt.from_col;
}

int _tag_text_calc_point(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;

	switch (scan)
	{
	case _TEXT_SCAN_BEGIN:
		if (!ptt->from_row && !ptt->from_col)
		{
			ptt->from_pt.x = cur_width;
			ptt->from_pt.y = cur_height;
			ptt->to_pt.x = cur_width;
			ptt->to_pt.y = cur_height;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	case _TEXT_SCAN_END:
		if (!ptt->done)
		{
			ptt->from_pt.x = cur_width;
			ptt->from_pt.y = cur_height;
			ptt->to_pt.x = cur_width + pse->cx;
			ptt->to_pt.y = cur_height + pse->cy;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	case _TEXT_SCAN_CHAR:
	case _TEXT_SCAN_SOFT_BREAK:
	case _TEXT_SCAN_HARD_BREAK:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->from_pt.x = cur_width;
			ptt->from_pt.y = cur_height;
			ptt->to_pt.x = cur_width + pse->cx;
			ptt->to_pt.y = cur_height + pse->cy;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _textctrl_calc_point(res_win_t widget, int row, int col, xpoint_t* ppt1, xpoint_t* ppt2)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;

	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.from_row = row;
	tt.from_col = col;
	scan_tag_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_point, (void*)&tt);

	if (ppt1)
	{
		ppt1->x = tt.from_pt.x - vb.bx;
		ppt1->y = tt.from_pt.y - vb.by;
	}
	if (ppt2)
	{
		ppt2->x = tt.to_pt.x - vb.bx;
		ppt2->y = tt.to_pt.y - vb.by;
	}
}

int _tag_text_calc_rowcol(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;

	switch (scan)
	{
	case _TEXT_SCAN_CHAR:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->nlk = cur_node;
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	case _TEXT_SCAN_HARD_BREAK:
	case _TEXT_SCAN_SOFT_BREAK:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->nlk = cur_node;
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		else if (ptt->from_row == cur_row && ptt->from_col < 0)
		{
			ptt->nlk = cur_node;
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		else if (ptt->from_row == cur_row && ptt->from_col > cur_col)
		{
			ptt->from_row = ptt->from_row + 1;
			ptt->from_col = ptt->from_col - cur_col - 1;
			return _TEXT_OPERA_NEXT;
		}
		break;
	case _TEXT_SCAN_END:
		if (!ptt->done)
		{
			ptt->nlk = cur_node;
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _textctrl_calc_rowcol(res_win_t widget, int row, int col, int* prow, int* pcol, LINKPTR* pnlk)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;

	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	if (col < 0)
	{
		row--;
	}
	if (row < 0)
	{
		pnlk = NULL;
		*prow = 0;
		*pcol = -1;
		return;
	}

	tt.from_row = row;
	tt.from_col = col;
	scan_tag_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_rowcol, (void*)&tt);

	if (prow)
		*prow = tt.to_row;
	if (pcol)
		*pcol = tt.to_col;
	if (pnlk)
		*pnlk = tt.nlk;
}

int _tag_text_calc_position(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;

	switch (scan)
	{
	case _TEXT_SCAN_NODE_BEGIN:
		if (!ptt->done && ptt->nlk == cur_node)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_NEXT;
		}
		break;
	case _TEXT_SCAN_NODE_END:
		if (ptt->done && ptt->nlk == cur_node)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			return _TEXT_OPERA_NEXT;
		}
		break;
	}
	
	return _TEXT_OPERA_NEXT;
}

void _textctrl_calc_position(res_win_t widget, LINKPTR nlk, int* from_row, int* from_col, int* to_row, int* to_col)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;

	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.nlk = nlk;
	scan_tag_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_position, (void*)&tt);

	if (from_row)
		*from_row = tt.from_row;
	if (from_col)
		*from_col = tt.from_col;
	if (to_row)
		*to_row = tt.to_row;
	if (to_col)
		*to_col = tt.to_col;
}

int _tag_text_calc_select(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;

	switch(scan)
	{
	case _TEXT_SCAN_CHAR:
	case _TEXT_SCAN_SOFT_BREAK:
	case _TEXT_SCAN_HARD_BREAK:
		if (inside_rowcol(cur_row, cur_col, ptt->from_row, ptt->from_col, ptt->to_row, ptt->to_col))
		{
			if (ptt->total + 1 > ptt->max)
				return _TEXT_OPERA_STOP;

			if (ptt->buf)
			{
				if (b_alter)
					ptt->buf[ptt->total] = _T(' ');
				else
					ptt->buf[ptt->total] = *cur_char;
			}
			ptt->total = ptt->total + 1;

			if (ptt->to_row == cur_row && ptt->to_col == cur_col)
				return _TEXT_OPERA_STOP;
			else
				return _TEXT_OPERA_NEXT;
		}
		break;
	}

	return _TEXT_OPERA_NEXT;
}

int _textctrl_calc_select(res_win_t widget, int from_row, int from_col, int to_row, int to_col, tchar_t* buf, int max)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;

	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return 0;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.buf = buf;
	tt.max = max;
	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.to_row = to_row;
	tt.to_col = to_col;
	scan_tag_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_select, (void*)&tt);

	return tt.total;
}

int _tag_text_calc_delete(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;

	switch(scan)
	{
	case _TEXT_SCAN_CHAR:
	case _TEXT_SCAN_SOFT_BREAK:
	case _TEXT_SCAN_HARD_BREAK:
		if (cur_row == ptt->from_row && cur_col == ptt->from_col)
		{
			if (!ptt->total)
				return _TEXT_OPERA_STOP;

			ptt->total = ptt->total - 1;
			return _TEXT_OPERA_DEL;
		}
		break;
	}
	
	return _TEXT_OPERA_NEXT;
}

void _textctrl_calc_delete(res_win_t widget, int from_row, int from_col, int to_row, int to_col)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;

	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.total = _textctrl_calc_select(widget, from_row, from_col, to_row, to_col, NULL, MAX_LONG);
	scan_tag_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_delete, (void*)&tt);
}

int _tag_text_calc_insert(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;

	switch (scan)
	{
	case _TEXT_SCAN_CHAR:
	case _TEXT_SCAN_SOFT_BREAK:
	case _TEXT_SCAN_HARD_BREAK:
	case _TEXT_SCAN_END:
		if (cur_row == ptt->from_row && cur_col == ptt->from_col)
		{
			if (ptt->total)
			{
				*ret_char = ptt->buf[ptt->total - 1];
				ptt->total = ptt->total - 1;
				return _TEXT_OPERA_INS;
			}
			else
			{
				return _TEXT_OPERA_STOP;
			}
		}
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _textctrl_calc_insert(res_win_t widget, int row, int col, tchar_t* str)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;

	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.from_row = row;
	tt.from_col = col;
	tt.buf = str;
	tt.total = xslen(str);
	scan_tag_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_insert, (void*)&tt);
}

int _tag_text_calc_focus(int scan, link_t_ptr cur_node, bool_t b_alter, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	TAGTEXTCALC* ptt = (TAGTEXTCALC*)pp;
	xrect_t xr_reg, xr_dot;
	xcolor_t xc;

	switch (scan)
	{
	case _TEXT_SCAN_CHAR:
	case _TEXT_SCAN_SOFT_BREAK:
	case _TEXT_SCAN_HARD_BREAK:
		if (compare_rowcol(ptt->from_row, ptt->from_col, ptt->to_row, ptt->to_col) > 0)
		{
			return _TEXT_OPERA_STOP;
		}

		if (!ptt->done && ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->from_pt.x = cur_width - ptt->offx;
			ptt->from_pt.y = cur_height - ptt->offy;
			ptt->done = 1;

			xr_dot.x = ptt->from_pt.x;
			xr_dot.y = ptt->from_pt.y - TEXT_FOCUS_SPAN;
			xr_dot.w = TEXT_FOCUS_SPAN;
			xr_dot.h = TEXT_FOCUS_SPAN;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			draw_alpha_raw(ptt->rdc, &xc, &xr_dot, ALPHA_SOLID);
		}

		if (ptt->done && ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->from_pt.x = cur_width - ptt->offx;
			ptt->from_pt.y = cur_height - ptt->offy;
		}

		if (ptt->done && ptt->to_row == cur_row && ptt->to_col == cur_col)
		{
			ptt->to_pt.x = cur_width + pse->cx - ptt->offx;
			ptt->to_pt.y = cur_height + pse->cy - ptt->offy;

			xr_reg.x = ptt->from_pt.x;
			xr_reg.y = ptt->from_pt.y;
			xr_reg.w = ptt->to_pt.x - ptt->from_pt.x;
			xr_reg.h = ptt->to_pt.y - ptt->from_pt.y;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			draw_alpha_raw(ptt->rdc, &xc, &xr_reg, ALPHA_SOFT);

			xr_dot.x = ptt->to_pt.x - TEXT_FOCUS_SPAN;
			xr_dot.y = ptt->to_pt.y;
			xr_dot.w = TEXT_FOCUS_SPAN;
			xr_dot.h = TEXT_FOCUS_SPAN;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			draw_alpha_raw(ptt->rdc, &xc, &xr_dot, ALPHA_SOLID);
			return _TEXT_OPERA_STOP;
		}

		if (ptt->done && (scan == _TEXT_SCAN_SOFT_BREAK || scan == _TEXT_SCAN_HARD_BREAK))
		{
			ptt->to_pt.x = cur_width + pse->cx - ptt->offx;
			ptt->to_pt.y = cur_height + pse->cy - ptt->offy;

			xr_reg.x = ptt->from_pt.x;
			xr_reg.y = ptt->from_pt.y;
			xr_reg.w = ptt->to_pt.x - ptt->from_pt.x;
			xr_reg.h = ptt->to_pt.y - ptt->from_pt.y;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			draw_alpha_raw(ptt->rdc, &xc, &xr_reg, ALPHA_SOFT);

			ptt->from_row = ptt->from_row + 1;
			ptt->from_col = 0;

			return _TEXT_OPERA_NEXT;
		}
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _textctrl_draw_focus(res_win_t widget, res_dc_t rdc)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	
	TAGTEXTCALC tt = { 0 };
	viewbox_t vb;

	xfont_t xf;
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_viewport(widget, &vb);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.from_row = ptd->sel_row;
	tt.from_col = ptd->sel_col;
	tt.to_row = ptd->cur_row;
	tt.to_col = ptd->cur_col;

	tt.rdc = rdc;
	tt.offx = vb.bx;
	tt.offy = vb.by;

	scan_tag_text(rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _tag_text_calc_focus, (void*)&tt);
}

void _textctrl_reset_caret(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	xpoint_t pt1, pt2;

	_textctrl_calc_point(widget, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	widget_show_caret(widget, pt1.x, pt1.y, 1);
}

void _textctrl_reset_page(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	xrect_t xr;
	long page_width, page_height;

	widget_get_client_rect(widget, &xr);

	_textctrl_calc_maxing(widget, &page_width, &page_height, NULL, NULL);

	widget_reset_paging(widget, xr.w, xr.h, page_width, page_height, 0, 0);

	if (widget_can_horz_scroll(widget))
		widget_reset_horz_scroll(widget);

	if (widget_can_vert_scroll(widget))
		widget_reset_vert_scroll(widget);
}

void _textctrl_reset_visible(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	xpoint_t pt1, pt2;
	xrect_t xr;

	_textctrl_calc_point(widget, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	xr.x = pt1.x;
	xr.y = pt1.y;
	xr.w = pt2.x - pt1.x;
	xr.h = pt2.y - pt1.y;

	widget_ensure_visible(widget, &xr);
}

void _textctrl_done(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	TAGTEXT_UNDO *pnew;
	int len;

	if (!ptd->data)
		return;

	pnew = (TAGTEXT_UNDO*)xmem_alloc(sizeof(TAGTEXT_UNDO));

	pnew->b_select = ptd->b_select;
	pnew->sel_row = ptd->sel_row;
	pnew->sel_col = ptd->sel_col;
	pnew->cur_row = ptd->cur_row;
	pnew->cur_col = ptd->cur_col;

	len = format_tag_text(ptd->data, NULL, MAX_LONG);
	pnew->text = xsalloc(len + 1);
	format_tag_text(ptd->data, pnew->text, len);

	pnew->next = ptd->ptu;
	ptd->ptu = pnew;
}

void _textctrl_clean(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	TAGTEXT_UNDO *next;

	if (!ptd->data)
		return;

	while (ptd->ptu)
	{
		next = ptd->ptu->next;

		xsfree(ptd->ptu->text);
		xmem_free(ptd->ptu);

		ptd->ptu = next;
	}
}

void _textctrl_copy(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	res_glob_t gb;
	int len = 0;
	byte* buf;

	if (!ptd->b_select)
		return;

	len = _textctrl_calc_select(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, NULL, MAX_LONG);
	if (!len)
	{
		return;
	}

	if (!clipboard_open())
		return;

	gb = system_alloc((len + 1)* sizeof(tchar_t));
	buf = (byte_t*)system_lock(gb);

	_textctrl_calc_select(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, (tchar_t*)buf, len);

	system_unlock(gb);

	if (!clipboard_put(FMT_UCS, gb))
	{
		system_free(gb);
		clipboard_close();

		return;
	}

	clipboard_close();
}

void _textctrl_cut(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	if (!ptd->b_select)
		return;

	_textctrl_done(widget);

	_textctrl_copy(widget);

	_textctrl_calc_delete(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

	ptd->b_select = 0;
	ptd->cur_row = ptd->sel_row;
	ptd->cur_col = ptd->sel_col;

	_textctrl_reset_page(widget);
	widget_invalid(widget, NULL, 0);
	_textctrl_reset_visible(widget);
	_textctrl_reset_caret(widget);
}

void _textctrl_paste(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	res_glob_t gb;
	byte* buf;
	int row, col;
	int len;

	if (!clipboard_open())
		return;

	gb = clipboard_get(FMT_UCS);
	if (!gb)
	{
		clipboard_close();
		return;
	}

	buf = (byte_t*)system_lock(gb);
	len = xslen((tchar_t*)buf);
	if (!len)
	{
		clipboard_close();
		return;
	}

	_textctrl_done(widget);

	_textctrl_calc_insert(widget, ptd->cur_row, ptd->cur_col, (tchar_t*)buf);
	_textctrl_calc_rowcol(widget, ptd->cur_row, ptd->cur_col + len, &row, &col, NULL);

	system_unlock(gb);

	clipboard_clean();
	clipboard_close();

	ptd->b_select = 0;
	ptd->cur_row = row;
	ptd->cur_col = col;

	_textctrl_reset_page(widget);
	widget_invalid(widget, NULL, 0);
	_textctrl_reset_visible(widget);
	_textctrl_reset_caret(widget);
}

void _textctrl_undo(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	TAGTEXT_UNDO *next;

	if (!ptd->data)
		return;

	if (!ptd->ptu)
		return;

	next = ptd->ptu->next;

	parse_tag_text(ptd->data, ptd->ptu->text, -1);
	xsfree(ptd->ptu->text);

	ptd->b_select = ptd->ptu->b_select;
	ptd->sel_row = ptd->ptu->sel_row;
	ptd->sel_col = ptd->ptu->sel_col;
	ptd->cur_row = ptd->ptu->cur_row;
	ptd->cur_col = ptd->ptu->cur_col;

	xmem_free(ptd->ptu);
	ptd->ptu = next;

	_textctrl_reset_page(widget);
	widget_invalid(widget, NULL, 0);
	_textctrl_reset_visible(widget);
	_textctrl_reset_caret(widget);
}

/********************************************************************************************/
void hand_textctrl_create(res_win_t widget, void* data)
{
	textctrl_delta_t* ptd;
	xhand_t canv;

	ptd = (textctrl_delta_t*)xmem_alloc(sizeof(textctrl_delta_t));
	memset((void*)ptd, 0, sizeof(textctrl_delta_t));

	ptd->rdc = widget_client_dc(widget);
	canv = create_display_canvas(ptd->rdc);
	widget_set_canvas(widget, canv);

	ptd->data = create_tag_doc();

	SETTEXTCTRLDELTA(widget, ptd);
}

void hand_textctrl_destroy(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	_textctrl_clean(widget);

	destroy_display_canvas(widget_get_canvas(widget));

	widget_release_dc(widget, ptd->rdc);

	destroy_tag_doc(ptd->data);

	xmem_free(ptd);

	SETTEXTCTRLDELTA(widget, 0);
}

void hand_textctrl_set_focus(res_win_t widget, res_win_t wt)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	xfont_t xf;
	long h;

	if (!ptd->data)
		return;

	widget_get_xfont(widget, &xf);

	text_metric_raw(ptd->rdc, &xf, &h, NULL, NULL, NULL);

	widget_create_caret(widget, 2, h);
}

void hand_textctrl_kill_focus(res_win_t widget, res_win_t wt)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	if (!ptd->data)
		return;

	widget_destroy_caret(widget);
}

void hand_textctrl_keydown(res_win_t widget, int key)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	int row, col;
	LINKPTR ilk;
	tchar_t token[2] = { 0 };

	if (!ptd->data)
		return;

	row = ptd->cur_row;
	col = ptd->cur_col;

	switch (key)
	{
	case KEY_BACK:
		if (!ptd->b_select)
		{
			_textctrl_calc_rowcol(widget, ptd->cur_row, ptd->cur_col - 1, &row, &col, NULL);
			if (col < 0)
				break;

			ptd->sel_row = row;
			ptd->sel_col = col;
			ptd->cur_row = row;
			ptd->cur_col = col;
		}

		_textctrl_done(widget);

		_textctrl_calc_delete(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;

		ptd->b_select = 0;
		_textctrl_reset_page(widget);
		widget_invalid(widget, NULL, 0);
		_textctrl_reset_visible(widget);
		_textctrl_reset_caret(widget);

		widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);
		break;
	case KEY_DELETE:
		_textctrl_done(widget);

		_textctrl_calc_delete(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;

		ptd->b_select = 0;
		_textctrl_reset_page(widget);
		widget_invalid(widget, NULL, 0);
		_textctrl_reset_visible(widget);
		_textctrl_reset_caret(widget);

		widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);
		break;
	case KEY_TAB:
		break;
	case KEY_ENTER:
		break;
	case KEY_ESC:
		break;
	case KEY_LEFT:
		_textctrl_calc_rowcol(widget, ptd->cur_row, ptd->cur_col - 1, &row, &col, &ilk);
		if (col < 0)
			break;
		if (row != ptd->cur_row || col != ptd->cur_col)
		{
			ptd->cur_row = row;
			ptd->cur_col = col;

			_textctrl_reset_visible(widget);
			_textctrl_reset_caret(widget);

			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(widget, NULL, 0);
			}
		}
		break;
	case KEY_UP:
		_textctrl_calc_rowcol(widget, ptd->cur_row - 1, ptd->cur_col, &row, &col, &ilk);
		if (col < 0)
			break;
		if (row != ptd->cur_row || col != ptd->cur_col)
		{
			ptd->cur_row = row;
			ptd->cur_col = col;

			_textctrl_reset_visible(widget);
			_textctrl_reset_caret(widget);

			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(widget, NULL, 0);
			}
		}
		break;
	case KEY_RIGHT:
		_textctrl_calc_rowcol(widget, ptd->cur_row, ptd->cur_col + 1, &row, &col, &ilk);
		if (row != ptd->cur_row || col != ptd->cur_col)
		{
			ptd->cur_row = row;
			ptd->cur_col = col;

			_textctrl_reset_visible(widget);
			_textctrl_reset_caret(widget);

			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(widget, NULL, 0);
			}
		}
		break;
	case KEY_DOWN:
		_textctrl_calc_rowcol(widget, ptd->cur_row + 1, ptd->cur_col, &row, &col, &ilk);
		if (row != ptd->cur_row || col != ptd->cur_col)
		{
			ptd->cur_row = row;
			ptd->cur_col = col;

			_textctrl_reset_visible(widget);
			_textctrl_reset_caret(widget);

			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(widget, NULL, 0);
			}
		}
	case _T('c'):
	case _T('C'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_textctrl_copy(widget);
		}
		break;
	case _T('x'):
	case _T('X'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_textctrl_cut(widget);

			widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);
		}
		break;
	case _T('v'):
	case _T('V'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_textctrl_paste(widget);

			widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);
		}
		break;
	case _T('z'):
	case _T('Z'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_textctrl_undo(widget);

			widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);
		}
		break;
	}
}

void hand_textctrl_char(res_win_t widget, tchar_t ch)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	int row, col;
	tchar_t token[2] = { 0 };

	if (!ptd->data)
		return;

	_textctrl_done(widget);

	if (ch == KEY_ENTER)
	{
		ch = _T('\n');
	}
	else if (ch == KEY_TAB)
	{
		ch = _T('\t');
	}
	else if (ch < 32)
		return;

	if (ptd->b_select)
	{
		_textctrl_calc_delete(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;
		ptd->b_select = 0;
	}

	token[0] = ch;

	_textctrl_calc_insert(widget, ptd->cur_row, ptd->cur_col, token);
	_textctrl_calc_rowcol(widget, ptd->cur_row, ptd->cur_col + 1, &row, &col, NULL);

	ptd->cur_row = row;
	ptd->cur_col = col;

	_textctrl_reset_page(widget);

	widget_invalid(widget, NULL, 0);

	_textctrl_reset_visible(widget);
	_textctrl_reset_caret(widget);

	widget_post_command(widget, COMMAND_UPDATE, widget_get_user_id(widget), NULL);
}

void hand_textctrl_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	
	if (!ptd->data)
		return;

	LINKPTR ilk = NULL;
	int row = 0;
	int col = 0;
	xpoint_t pt1, pt2;

	if (!widget_is_focus(widget))
		widget_set_focus(widget);

	if (ptd->b_select)
	{
		_textctrl_calc_point(widget, ptd->sel_row, ptd->sel_col, &pt1, &pt2);
		if (is_inside(pxp->x, pxp->y, pt1.x - TEXT_FOCUS_SPAN, pt1.y - TEXT_FOCUS_SPAN, pt1.x + TEXT_FOCUS_SPAN, pt2.y + TEXT_FOCUS_SPAN))
		{
			widget_set_capture(widget, 1);
			ptd->n_drag = TEXT_DRAG_FIRST;
			return;
		}

		_textctrl_calc_point(widget, ptd->cur_row, ptd->cur_col, &pt1, &pt2);
		if (is_inside(pxp->x, pxp->y, pt2.x - TEXT_FOCUS_SPAN, pt1.y - TEXT_FOCUS_SPAN, pt2.x + TEXT_FOCUS_SPAN, pt2.y + TEXT_FOCUS_SPAN))
		{
			widget_set_capture(widget, 1);
			ptd->n_drag = TEXT_DRAG_LAST;
			return;
		}
	}

	_textctrl_calc_hint(widget, pxp->x, pxp->y, &ilk, &row, &col);

	ptd->b_select = 0;
	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->cur_row = row;
	ptd->cur_col = col;

	_textctrl_reset_caret(widget);
}

void hand_textctrl_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	
	if (!ptd->data)
		return;

	if (ptd->n_drag)
	{
		widget_set_capture(widget, 0);

		ptd->n_drag = 0;

		widget_invalid(widget, NULL, 0);
	}
}

void hand_textctrl_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	if (!ptd->data)
		return;

	ptd->b_select = 1;

	widget_invalid(widget, NULL, 0);
}

void hand_textctrl_mousemove(res_win_t widget, UINT mk, long x, long y)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	LINKPTR ilk;
	int row, col;

	if (!ptd->data)
		return;

	if (ptd->n_drag == TEXT_DRAG_FIRST)
	{
		_textctrl_calc_hint(widget, x, y, &ilk, &row, &col);
		if (row >= ptd->cur_row && col >= ptd->cur_col)
			return;

		ptd->sel_row = row;
		ptd->sel_col = col;

		widget_invalid(widget, NULL, 0);
	}
	else if (ptd->n_drag == TEXT_DRAG_LAST)
	{
		_textctrl_calc_hint(widget, x, y, &ilk, &row, &col);
		if (row <= ptd->sel_row && col <= ptd->sel_col)
			return;

		ptd->cur_row = row;
		ptd->cur_col = col;

		widget_invalid(widget, NULL, 0);
	}
	else if (mk & MK_LBUTTON)
	{
		_textctrl_calc_hint(widget, x, y, &ilk, &row, &col);

		if (row < ptd->cur_row || (row == ptd->cur_row && col < ptd->cur_col))
		{
			ptd->sel_row = row;
			ptd->sel_col = col;

			ptd->b_select = 1;
			widget_invalid(widget, NULL, 0);
			_textctrl_reset_caret(widget);
		}else if (row > ptd->cur_row || (row == ptd->cur_row && col > ptd->cur_col))
		{
			ptd->cur_row = row;
			ptd->cur_col = col;

			ptd->b_select = 1;
			widget_invalid(widget, NULL, 0);
			_textctrl_reset_caret(widget);
		}
	}
}

void hand_textctrl_size(res_win_t widget, int code, const xsize_t* prs)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	if (!ptd->data)
		return;

	textctrl_redraw(widget);
}

void hand_textctrl_vert_scroll(res_win_t widget, const scroll_t* psc)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	if (!ptd->data)
		return;

	short line_feed;
	float line_height;
	long text_height;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	line_height = (float)xstof(xa.line_height);
	if (!line_height)
		line_height = 1.0;

	text_metric_raw(ptd->rdc, &xf, &text_height, NULL, NULL, NULL);
	line_feed = (short)(text_height * line_height);

	widget_hand_vert_scroll(widget, psc, line_feed, 2 * line_feed);
}


void hand_textctrl_horz_scroll(res_win_t widget, const scroll_t* psc)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	long text_width;
	xfont_t xf = { 0 };

	if (!ptd->data)
		return;

	widget_get_xfont(widget, &xf);

	text_metric_raw(ptd->rdc, &xf, NULL, NULL, NULL, &text_width);

	widget_hand_horz_scroll(widget, psc, (short)text_width, (short)(2 * text_width));
}

void hand_textctrl_vert_wheel(res_win_t widget, dword_t dw, short delta)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	short line_feed;
	float line_height;
	long text_height;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	if (!ptd->data)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	line_height = (float)xstof(xa.line_height);
	if (!line_height)
		line_height = 1.0;

	text_metric_raw(ptd->rdc, &xf, &text_height, NULL, NULL, NULL);
	line_feed = (short)(text_height * line_height);

	widget_hand_vert_wheel(widget, delta, line_feed);
}

void hand_textctrl_horz_wheel(res_win_t widget, dword_t dw, short delta)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	long text_width;
	xfont_t xf = { 0 };

	if (!ptd->data)
		return;

	widget_get_xfont(widget, &xf);

	text_metric_raw(ptd->rdc, &xf, NULL, NULL, NULL, &text_width);

	widget_hand_horz_wheel(widget, delta, (short)text_width);
}

void hand_textctrl_command(res_win_t widget, int code, int cid, void* data)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	xbrush_t xb;
	xpen_t xp;
	xface_t xa;
	xcolor_t* pxc;

	if (cid == IDC_PARENT)
	{
		if (code == COMMAND_COLOR)
		{
			pxc = (xcolor_t*)data;

			widget_get_xbrush(widget, &xb);
			format_xcolor(&pxc[0], xb.color);
			widget_set_xbrush(widget, &xb);

			widget_get_xpen(widget, &xp);
			format_xcolor(&pxc[1], xp.color);
			widget_set_xpen(widget, &xp);

			widget_get_xface(widget, &xa);
			format_xcolor(&pxc[2], xa.text_color);
			widget_set_xface(widget, &xa);
		}
	}
}


void hand_textctrl_erase(res_win_t widget, res_dc_t dc)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	if (!ptd->data)
		return;
}

void hand_textctrl_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	res_dc_t rdc;
	xrect_t xr,xr_text;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xbrush_t xb = { 0 };
	viewbox_t vb;

	xhand_t canv;
	if_canvas_t* pif;

	if (!ptd->data)
		return;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);
	
	widget_get_client_rect(widget, &xr);

	rdc = begin_display_paint(pif->canvas, dc, xr.w, xr.h);

	xscpy(xb.color, GDI_ATTR_RGB_SOFTWHITE);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_viewport(widget, &vb);

	xr_text.x = - vb.bx;
	xr_text.y = - vb.by;
	xr_text.w = vb.bw;
	xr_text.h = vb.bh;

	if (ptd->data)
	{
		draw_tag_text(pif->canvas, &xf, &xa, &xr_text, ptd->data);
	}

	if (ptd->b_select)
	{
		_textctrl_draw_focus(widget, rdc);
	}

	end_display_paint(pif->canvas, dc, &xr);
	destroy_canvas_interface(pif);
}

/************************************************************************************************/
res_win_t textctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_textctrl_create;
	ev.pf_on_destroy = hand_textctrl_destroy;

	ev.pf_on_erase = hand_textctrl_erase;
	ev.pf_on_paint = hand_textctrl_paint;

	ev.pf_on_size = hand_textctrl_size;

	ev.pf_on_set_focus = hand_textctrl_set_focus;
	ev.pf_on_kill_focus = hand_textctrl_kill_focus;

	ev.pf_on_horz_scroll = hand_textctrl_horz_scroll;
	ev.pf_on_horz_wheel = hand_textctrl_horz_wheel;
	ev.pf_on_vert_scroll = hand_textctrl_vert_scroll;
	ev.pf_on_vert_wheel = hand_textctrl_vert_wheel;

	ev.pf_on_keydown = hand_textctrl_keydown;
	ev.pf_on_char = hand_textctrl_char;

	ev.pf_on_lbutton_down = hand_textctrl_lbutton_down;
	ev.pf_on_lbutton_up = hand_textctrl_lbutton_up;
	ev.pf_on_lbutton_dbclick = hand_textctrl_lbutton_dbclick;

	ev.pf_on_command = hand_textctrl_command;

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void textctrl_set_text(res_win_t widget, const tchar_t* token)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	parse_tag_text(ptd->data, token, -1);

	textctrl_redraw(widget);
}

int textctrl_get_text(res_win_t widget, tchar_t* buf, int max)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);

	if (!ptd->data)
		return 0;

	return format_tag_text(ptd->data, buf, max);
}

void textctrl_redraw(res_win_t widget)
{
	textctrl_delta_t* ptd = GETTEXTCTRLDELTA(widget);
	int row, col;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->data)
		return;

	_textctrl_calc_maxing(widget, NULL, NULL, &row, &col);
	ptd->sel_row = ptd->cur_row = row;
	ptd->sel_col = ptd->cur_col = col;

	_textctrl_reset_page(widget);
	widget_invalid(widget, NULL, 0);
	_textctrl_reset_visible(widget);
	_textctrl_reset_caret(widget);
}