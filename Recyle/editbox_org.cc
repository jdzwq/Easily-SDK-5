/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc edit control document

	@module	editbox.c | edit control widnows implement file

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

#include "xdcbox.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcclip.h"
#include "xdceditor.h"
#include "xdcgdi.h"

typedef enum{EDIT_DRAG_FIRST = 1,EDIT_DRAG_LAST = 2}_EDITDRAG;

typedef struct _VAREDIT_UNDO{
	int cur_row, cur_col;
	int sel_row, sel_col;
	BOOL b_select;
	XHANDLE data;

	_VAREDIT_UNDO* next;
}VAREDIT_UNDO;

typedef struct _editbox_delta_t{
	XHANDLE data;

	int cur_row, cur_col;
	int sel_row, sel_col;
	bool_t b_select;

	int n_drag;

	VAREDIT_UNDO* ptu;

	res_dc_t rdc;
}editbox_delta_t;

#define GETEDITBOXDELTA(ph) 	(editbox_delta_t*)widget_get_user_delta(ph)
#define SETEDITBOXDELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

/***************************************************************************************/
typedef struct _VAREDITCALC{
	bool_t done;
	long x, y;
	long width, height;
	int from_row, from_col,to_row,to_col;
	xpoint_t from_pt, to_pt;
	tchar_t* buf;
	int total,max;

	res_dc_t rdc;
	long offx, offy;
}VAREDITCALC;

int _var_text_calc_maxing(int scan, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	VAREDITCALC* ptt = (VAREDITCALC*)pp;

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
		if (ptt->width < cur_width + pse->cx + line_width)
			ptt->width = cur_width + pse->cx + line_width;
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _editbox_calc_maxing(res_win_t widget, long* max_width, long* max_height, int* max_row, int* max_col)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	VAREDITCALC tt = { 0 };
	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_viewport(widget, &vb);
	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	
	scan_var_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _var_text_calc_maxing, (void*)&tt);

	if (max_width)
		*max_width = tt.width;
	if (max_height)
		*max_height = tt.height;
	if (max_row)
		*max_row = tt.to_row;
	if (max_col)
		*max_col = tt.to_col;
}

int _var_text_calc_hint(int scan, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	VAREDITCALC* ptt = (VAREDITCALC*)pp;

	switch (scan)
	{
	case _TEXT_SCAN_CHAR:
		if (ptt->y >= cur_height && ptt->y < cur_height + pse->cy + line_height && ptt->x >= cur_width && ptt->x < cur_width + pse->cx)
		{
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
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	case _TEXT_SCAN_END:
		if (!ptt->done)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			return _TEXT_OPERA_STOP;
		}
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _editbox_calc_hint(res_win_t widget, long x, long y, int* row_at, int* col_at)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	VAREDITCALC tt = { 0 };
	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_viewport(widget, &vb);
	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.x = x + vb.bx;
	tt.y = y + vb.by;
	scan_var_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _var_text_calc_hint, (void*)&tt);

	if (row_at)
		*row_at = tt.from_row;
	if (col_at)
		*col_at = tt.from_col;
}

int _var_text_calc_point(int scan, long line_width, long line_height,  const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	VAREDITCALC* ptt = (VAREDITCALC*)pp;

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

void _editbox_calc_point(res_win_t widget, int row, int col, xpoint_t* ppt1, xpoint_t* ppt2)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	VAREDITCALC tt = { 0 };
	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_viewport(widget, &vb);
	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.from_row = row;
	tt.from_col = col;
	scan_var_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _var_text_calc_point, (void*)&tt);

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

int _var_text_calc_rowcol(int scan, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	VAREDITCALC* ptt = (VAREDITCALC*)pp;

	switch (scan)
	{
	case _TEXT_SCAN_CHAR:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
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
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		else if (ptt->from_row == cur_row && ptt->from_col < 0)
		{
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
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->done = 1;
			return _TEXT_OPERA_STOP;
		}
		break;
	}

	return _TEXT_OPERA_NEXT;
}

void _editbox_calc_rowcol(res_win_t widget, int row, int col, int* prow, int* pcol)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	VAREDITCALC tt = { 0 };
	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_viewport(widget, &vb);
	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	if (col < 0)
	{
		row--;
	}
	if (row < 0)
	{
		*prow = 0;
		*pcol = -1;
		return;
	}

	tt.from_row = row;
	tt.from_col = col;
	scan_var_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _var_text_calc_rowcol, (void*)&tt);

	if (prow)
		*prow = tt.to_row;
	if (pcol)
		*pcol = tt.to_col;
}

int _var_text_calc_select(int scan, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	VAREDITCALC* ptt = (VAREDITCALC*)pp;

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

int _editbox_calc_select(res_win_t widget, int from_row, int from_col, int to_row, int to_col, tchar_t* buf, int max)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	VAREDITCALC tt = { 0 };
	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_viewport(widget, &vb);
	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.buf = buf;
	tt.max = max;
	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.to_row = to_row;
	tt.to_col = to_col;
	scan_var_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _var_text_calc_select, (void*)&tt);

	return tt.total;
}

int _var_text_calc_delete(int scan, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	VAREDITCALC* ptt = (VAREDITCALC*)pp;

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

void _editbox_calc_delete(res_win_t widget, int from_row, int from_col, int to_row, int to_col)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	VAREDITCALC tt = { 0 };
	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_viewport(widget, &vb);
	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.total = _editbox_calc_select(widget, from_row, from_col, to_row, to_col, NULL, MAX_LONG);
	scan_var_text(ptd->rdc, ptd->data , &xf, &xa, vb.bw, vb.bh, _var_text_calc_delete, (void*)&tt);
}

int _var_text_calc_insert(int scan, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	VAREDITCALC* ptt = (VAREDITCALC*)pp;

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

void _editbox_calc_insert(res_win_t widget, int row, int col, tchar_t* str)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	VAREDITCALC tt = { 0 };
	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_viewport(widget, &vb);
	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	tt.from_row = row;
	tt.from_col = col;
	tt.buf = str;
	tt.total = xslen(str);
	scan_var_text(ptd->rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _var_text_calc_insert, (void*)&tt);
}

int _var_text_calc_focus(int scan, long line_width, long line_height, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, long cur_width, long cur_height, xsize_t* pse, void* pp)
{
	VAREDITCALC* ptt = (VAREDITCALC*)pp;
	xrect_t xr_reg, xr_dot;
	xcolor_t xc;

	switch (scan)
	{
	case _TEXT_SCAN_CHAR:
	case _TEXT_SCAN_SOFT_BREAK:
	case _TEXT_SCAN_HARD_BREAK:
	case _TEXT_SCAN_END:
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

void _editbox_draw_focus(res_win_t widget, res_dc_t rdc)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	VAREDITCALC tt = { 0 };
	viewbox_t vb = { 0 };

	xfont_t xf = { 0 };
	xface_t xa = { 0 };

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

	scan_var_text(rdc, ptd->data, &xf, &xa, vb.bw, vb.bh, _var_text_calc_focus, (void*)&tt);
}

void _editbox_reset_caret(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xpoint_t pt1, pt2;

	_editbox_calc_point(widget, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	widget_show_caret(widget, pt1.x, pt1.y, 1);
}

void _editbox_reset_page(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xrect_t xr;
	long page_width, page_height;

	widget_get_client_rect(widget, &xr);

	_editbox_calc_maxing(widget, &page_width, &page_height, NULL, NULL);

	widget_reset_paging(widget, xr.w, xr.h, page_width, page_height, 0, 0);

	if (widget_can_horz_scroll(widget))
		widget_reset_horz_scroll(widget);

	if (widget_can_vert_scroll(widget))
		widget_reset_vert_scroll(widget);
}

void _editbox_reset_visible(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xpoint_t pt1, pt2;
	xrect_t xr;

	_editbox_calc_point(widget, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	xr.x = pt1.x;
	xr.y = pt1.y;
	xr.w = pt2.x - pt1.x;
	xr.h = pt2.y - pt1.y;

	widget_ensure_visible(widget, &xr);
}

void _editbox_done(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	VAREDIT_UNDO *pnew;

	pnew = (VAREDIT_UNDO*)xmem_alloc(sizeof(VAREDIT_UNDO));

	pnew->b_select = ptd->b_select;
	pnew->sel_row = ptd->sel_row;
	pnew->sel_col = ptd->sel_col;
	pnew->cur_row = ptd->cur_row;
	pnew->cur_col = ptd->cur_col;
	pnew->data = varstr_clone(ptd->data);

	pnew->next = ptd->ptu;
	ptd->ptu = pnew;
}

void _editbox_clean(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	VAREDIT_UNDO *next;

	while (ptd->ptu)
	{
		next = ptd->ptu->next;

		varstr_free(ptd->ptu->data);
		xmem_free(ptd->ptu);

		ptd->ptu = next;
	}
}

void _editbox_copy(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	res_glob_t gb;
	int len = 0;
	byte* buf;

	if (!ptd->b_select)
		return;

	len = _editbox_calc_select(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, NULL, MAX_LONG);
	if (!len)
	{
		return;
	}

	if (!clipboard_open())
		return;

	gb = system_alloc((len + 1)* sizeof(tchar_t));
	buf = (byte_t*)system_lock(gb);

	_editbox_calc_select(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, (tchar_t*)buf, len);

	system_unlock(gb);

	if (!clipboard_put(FMT_UCS, gb))
	{
		system_free(gb);
		clipboard_close();

		return;
	}

	clipboard_close();
}

void _editbox_cut(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (!ptd->b_select)
		return;

	_editbox_done(widget);

	_editbox_copy(widget);

	_editbox_calc_delete(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

	ptd->b_select = 0;
	ptd->cur_row = ptd->sel_row;
	ptd->cur_col = ptd->sel_col;

	_editbox_reset_page(widget);

	widget_invalid(widget, NULL, 0);

	_editbox_reset_caret(widget);
}

void _editbox_paste(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
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

	_editbox_done(widget);

	_editbox_calc_insert(widget, ptd->cur_row, ptd->cur_col, (tchar_t*)buf);
	_editbox_calc_rowcol(widget, ptd->cur_row, ptd->cur_col + len, &row, &col);

	system_unlock(gb);

	clipboard_clean();
	clipboard_close();

	ptd->b_select = 0;
	ptd->cur_row = row;
	ptd->cur_col = col;

	_editbox_reset_page(widget);

	widget_invalid(widget, NULL, 0);

	_editbox_reset_visible(widget);
	_editbox_reset_caret(widget);
}

void _editbox_undo(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	VAREDIT_UNDO *next;

	if (!ptd->ptu)
		return;

	next = ptd->ptu->next;

	varstr_free(ptd->data);

	ptd->data = ptd->ptu->data;
	ptd->b_select = ptd->ptu->b_select;
	ptd->sel_row = ptd->ptu->sel_row;
	ptd->sel_col = ptd->ptu->sel_col;
	ptd->cur_row = ptd->ptu->cur_row;
	ptd->cur_col = ptd->ptu->cur_col;

	xmem_free(ptd->ptu);
	ptd->ptu = next;

	_editbox_reset_page(widget);

	widget_invalid(widget, NULL, 0);

	_editbox_reset_visible(widget);
	_editbox_reset_caret(widget);
}


/********************************************************************************************/
int hand_editbox_create(res_win_t widget, void* data)
{
	editbox_delta_t* ptd;
	xhand_t canv;

	ptd = (editbox_delta_t*)xmem_alloc(sizeof(editbox_delta_t));
	memset((void*)ptd, 0, sizeof(editbox_delta_t));

	ptd->rdc = widget_client_dc(widget);
	canv = create_display_canvas(ptd->rdc);

	widget_set_canvas(widget, canv);

	ptd->data = varstr_alloc(0, 0);

	SETEDITBOXDELTA(widget, ptd);

	return 0;
}

void hand_editbox_destroy(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xhand_t canv;

	XDC_ASSERT(ptd != NULL);

	_editbox_clean(widget);

	canv = widget_get_canvas(widget);

	destroy_display_canvas(canv);

	widget_release_dc(widget, ptd->rdc);

	varstr_free(ptd->data);

	xmem_free(ptd);

	SETEDITBOXDELTA(widget, 0);
}

void hand_editbox_set_focus(res_win_t widget, res_win_t wt)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xfont_t xf;
	long h;

	if (!ptd->data)
		return;

	widget_get_xfont(widget, &xf);

	text_metric_raw(ptd->rdc, &xf, &h, NULL, NULL, NULL);

	widget_create_caret(widget, 2, h);
}

void hand_editbox_kill_focus(res_win_t widget, res_win_t wt)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (!ptd->data)
		return;

	widget_destroy_caret(widget);
}

void hand_editbox_keydown(res_win_t widget, int key)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	int row, col;
	int cid;
	tchar_t token[2] = { 0 };
	xface_t xa;

	if (!ptd->data)
		return;

	widget_get_xface(widget, &xa);

	row = ptd->cur_row;
	col = ptd->cur_col;

	switch (key)
	{
	case KEY_BACK:
		if (!ptd->b_select)
		{
			_editbox_calc_rowcol(widget, ptd->cur_row, ptd->cur_col - 1, &row, &col);
			if (col < 0)
				return;

			ptd->sel_row = row;
			ptd->sel_col = col;
			ptd->cur_row = row;
			ptd->cur_col = col;
		}

		_editbox_done(widget);

		_editbox_calc_delete(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;

		ptd->b_select = 0;
		_editbox_reset_page(widget);
		widget_invalid(widget, NULL, 0);

		_editbox_reset_visible(widget);
		_editbox_reset_caret(widget);

		cid = widget_get_user_id(widget);
		widget_post_command(widget, COMMAND_UPDATE, cid, NULL);
		break;
	case KEY_DELETE:
		_editbox_done(widget);

		_editbox_calc_delete(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;

		ptd->b_select = 0;
		_editbox_reset_page(widget);
		widget_invalid(widget, NULL, 0);

		_editbox_reset_visible(widget);
		_editbox_reset_caret(widget);

		cid = widget_get_user_id(widget);
		widget_post_command(widget, COMMAND_UPDATE, cid, NULL);
		break;
	case KEY_TAB:
		if (is_null(xa.text_wrap))
		{
			cid = widget_get_user_id(widget);
			widget_post_command(widget, COMMAND_COMMIT, cid, NULL);
		}
		break;
	case KEY_ENTER:
		if (is_null(xa.text_wrap))
		{
			cid = widget_get_user_id(widget);
			widget_post_command(widget, COMMAND_COMMIT, cid, NULL);
		}
		break;
	case KEY_ESC:
		if (ptd->b_select)
		{
			ptd->b_select = 0;
			widget_invalid(widget, NULL, 0);
		}

		if (is_null(xa.text_wrap))
		{
			cid = widget_get_user_id(widget);
			widget_post_command(widget, COMMAND_ROLLBACK, cid, NULL);
		}
		break;
	case KEY_LEFT:
		_editbox_calc_rowcol(widget, ptd->cur_row, ptd->cur_col - 1, &row, &col);
		if (col < 0)
			break;
		if (row != ptd->cur_row || col != ptd->cur_col)
		{
			if (widget_key_state(widget, KEY_SHIFT))
			{
				if (compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
					break;

				ptd->cur_row = row;
				ptd->cur_col = col;
				ptd->b_select = 1;
				widget_invalid(widget, NULL, 0);
			}
			else
			{
				ptd->cur_row = row;
				ptd->cur_col = col;
				ptd->sel_row = row;
				ptd->sel_col = col;
				if (ptd->b_select)
				{
					ptd->b_select = 0;
					widget_invalid(widget, NULL, 0);
				}
			}

			_editbox_reset_visible(widget);

			_editbox_reset_caret(widget);
		}
		else if (!widget_key_state(widget, KEY_SHIFT))
		{
			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(widget, NULL, 0);
			}
		}
		break;
	case KEY_RIGHT:
		_editbox_calc_rowcol(widget, ptd->cur_row, ptd->cur_col + 1, &row, &col);
		if (row != ptd->cur_row || col != ptd->cur_col)
		{
			if (widget_key_state(widget, KEY_SHIFT))
			{
				ptd->cur_row = row;
				ptd->cur_col = col;
				ptd->b_select = 1;
				widget_invalid(widget, NULL, 0);
			}
			else
			{
				ptd->cur_row = row;
				ptd->cur_col = col;
				ptd->sel_row = row;
				ptd->sel_col = col;
				if (ptd->b_select)
				{
					ptd->b_select = 0;
					widget_invalid(widget, NULL, 0);
				}
			}

			_editbox_reset_visible(widget);

			_editbox_reset_caret(widget);
		}
		else if (!widget_key_state(widget, KEY_SHIFT))
		{
			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(widget, NULL, 0);
			}
		}
		break;
	case KEY_UP:
		_editbox_calc_rowcol(widget, ptd->cur_row - 1, ptd->cur_col, &row, &col);
		if (col < 0)
			break;
		if (row != ptd->cur_row || col != ptd->cur_col)
		{
			if (widget_key_state(widget, KEY_SHIFT))
			{
				if (compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
					break;

				ptd->cur_row = row;
				ptd->cur_col = col;
				ptd->b_select = 1;
				widget_invalid(widget, NULL, 0);
			}
			else
			{
				ptd->cur_row = row;
				ptd->cur_col = col;
				ptd->sel_row = row;
				ptd->sel_col = col;
				if (ptd->b_select)
				{
					ptd->b_select = 0;
					widget_invalid(widget, NULL, 0);
				}
			}

			_editbox_reset_visible(widget);

			_editbox_reset_caret(widget);
		}
		else if (!widget_key_state(widget, KEY_SHIFT))
		{
			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(widget, NULL, 0);
			}
		}
		break;
	case KEY_DOWN:
		_editbox_calc_rowcol(widget, ptd->cur_row + 1, ptd->cur_col, &row, &col);
		if (row != ptd->cur_row || col != ptd->cur_col)
		{
			if (widget_key_state(widget, KEY_SHIFT))
			{
				ptd->cur_row = row;
				ptd->cur_col = col;
				ptd->b_select = 1;
				widget_invalid(widget, NULL, 0);
			}
			else
			{
				ptd->cur_row = row;
				ptd->cur_col = col;
				ptd->sel_row = row;
				ptd->sel_col = col;
				if (ptd->b_select)
				{
					ptd->b_select = 0;
					widget_invalid(widget, NULL, 0);
				}
			}

			_editbox_reset_visible(widget);

			_editbox_reset_caret(widget);
		}
		else if (!widget_key_state(widget, KEY_SHIFT))
		{
			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(widget, NULL, 0);
			}
		}
		break;
	case _T('c'):
	case _T('C'):
		if (widget_key_state(widget,KEY_CONTROL))
		{
			_editbox_copy(widget);
		}
		break;
	case _T('x'):
	case _T('X'):
		if (widget_key_state(widget,KEY_CONTROL))
		{
			_editbox_cut(widget);

			cid = widget_get_user_id(widget);
			widget_post_command(widget, COMMAND_UPDATE, cid, NULL);
		}
		break;
	case _T('v'):
	case _T('V'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_editbox_paste(widget);

			cid = widget_get_user_id(widget);
			widget_post_command(widget, COMMAND_UPDATE, cid, NULL);
		}
		break;
	case _T('z'):
	case _T('Z'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			_editbox_undo(widget);

			cid = widget_get_user_id(widget);
			widget_post_command(widget, COMMAND_UPDATE, cid, NULL);
		}
		break;
	}
}

void hand_editbox_char(res_win_t widget, tchar_t ch)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	int row, col;
	int cid;
	tchar_t token[2] = { 0 };
	xface_t xa = { 0 };

	if (!ptd->data)
		return;

	widget_get_xface(widget, &xa);

	if (ch == KEY_ENTER)
	{
		if (is_null(xa.text_wrap))
			return;
		ch = _T('\n');
	}
	else if (ch == KEY_TAB)
	{
		if (is_null(xa.text_wrap))
			return;
		ch = _T('\t');
	}
	else if (ch < 32)
		return;

	_editbox_done(widget);

	if (ptd->b_select)
	{
		_editbox_calc_delete(widget, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;
		ptd->b_select = 0;
	}

	token[0] = ch;

	_editbox_calc_insert(widget, ptd->cur_row, ptd->cur_col, token);
	_editbox_calc_rowcol(widget, ptd->cur_row, ptd->cur_col + 1, &row, &col);

	ptd->cur_row = row;
	ptd->cur_col = col;

	_editbox_reset_page(widget);
	widget_invalid(widget, NULL, 0);

	_editbox_reset_visible(widget);
	_editbox_reset_caret(widget);

	cid = widget_get_user_id(widget);
	widget_post_command(widget, COMMAND_UPDATE, cid, NULL);
}

void hand_editbox_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	
	int row = 0;
	int col = 0;
	xpoint_t pt1, pt2;

	if (!ptd->data)
		return;

	if (!widget_is_focus(widget))
		widget_set_focus(widget);

	if (ptd->b_select)
	{
		_editbox_calc_point(widget, ptd->sel_row, ptd->sel_col, &pt1, &pt2);
		if (is_inside(pxp->x, pxp->y, pt1.x - TEXT_FOCUS_SPAN, pt1.y - TEXT_FOCUS_SPAN, pt1.x + TEXT_FOCUS_SPAN, pt2.y + TEXT_FOCUS_SPAN))
		{
			widget_set_capture(widget, 1);
			ptd->n_drag = EDIT_DRAG_FIRST;
			return;
		}

		_editbox_calc_point(widget, ptd->cur_row, ptd->cur_col, &pt1, &pt2);
		if (is_inside(pxp->x, pxp->y, pt2.x - TEXT_FOCUS_SPAN, pt1.y - TEXT_FOCUS_SPAN, pt2.x + TEXT_FOCUS_SPAN, pt2.y + TEXT_FOCUS_SPAN))
		{
			widget_set_capture(widget, 1);
			ptd->n_drag = EDIT_DRAG_LAST;
			return;
		}

		ptd->b_select = 0;
		widget_invalid(widget, NULL, 0);
	}

	_editbox_calc_hint(widget, pxp->x, pxp->y, &row, &col);

	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->cur_row = row;
	ptd->cur_col = col;

	_editbox_reset_caret(widget);
}

void hand_editbox_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (!ptd->data)
		return;

	if (ptd->n_drag)
	{
		widget_set_capture(widget, 0);

		ptd->n_drag = 0;
		widget_invalid(widget, NULL, 0);
		return;
	}
}

void hand_editbox_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (!ptd->data)
		return;

	ptd->b_select = 1;

	widget_invalid(widget, NULL, 0);
}

void hand_editbox_mousemove(res_win_t widget, dword_t mk, const xpoint_t* ppt)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	int row, col;

	if (!ptd->data)
		return;

	if (ptd->n_drag == EDIT_DRAG_FIRST)
	{
		_editbox_calc_hint(widget, ppt->x, ppt->y, &row, &col);
		if (row >= ptd->cur_row && col >= ptd->cur_col)
			return;

		ptd->sel_row = row;
		ptd->sel_col = col;

		widget_invalid(widget, NULL, 0);
	}
	else if (ptd->n_drag == EDIT_DRAG_LAST)
	{
		_editbox_calc_hint(widget, ppt->x, ppt->y, &row, &col);
		if (row <= ptd->sel_row && col <= ptd->sel_col)
			return;

		ptd->cur_row = row;
		ptd->cur_col = col;

		widget_invalid(widget, NULL, 0);
	}
	else if (mk & MK_LBUTTON)
	{
		_editbox_calc_hint(widget, ppt->x, ppt->y, &row, &col);

		if (row < ptd->cur_row || (row == ptd->cur_row && col < ptd->cur_col))
		{
			ptd->sel_row = row;
			ptd->sel_col = col;

			ptd->b_select = 1;
			widget_invalid(widget, NULL, 0);
			_editbox_reset_caret(widget);
		}else if (row > ptd->cur_row || (row == ptd->cur_row && col > ptd->cur_col))
		{
			ptd->cur_row = row;
			ptd->cur_col = col;

			ptd->b_select = 1;
			widget_invalid(widget, NULL, 0);
			_editbox_reset_caret(widget);
		}
	}
}

void hand_editbox_size(res_win_t widget, int code, const xsize_t* prs)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (!ptd->data)
		return;

	editbox_redraw(widget);
}

void hand_editbox_vert_scroll(res_win_t widget, const scroll_t* psc)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

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

	_editbox_reset_caret(widget);
}

void hand_editbox_horz_scroll(res_win_t widget, const scroll_t* psc)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	long text_width;
	xfont_t xf = { 0 };

	if (!ptd->data)
		return;

	widget_get_xfont(widget, &xf);

	text_metric_raw(ptd->rdc, &xf, NULL, NULL, NULL, &text_width);

	widget_hand_horz_scroll(widget, psc, (short)text_width, (short)(2 * text_width));

	_editbox_reset_caret(widget);
}

void hand_editbox_vert_wheel(res_win_t widget, dword_t dw, short delta)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

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

	_editbox_reset_caret(widget);
}

void hand_editbox_horz_wheel(res_win_t widget, dword_t dw, short delta)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	long text_width;
	xfont_t xf = { 0 };

	if (!ptd->data)
		return;

	widget_get_xfont(widget, &xf);

	text_metric_raw(ptd->rdc, &xf, NULL, NULL, NULL, &text_width);

	widget_hand_horz_wheel(widget, delta, (short)text_width);

	_editbox_reset_caret(widget);
}

void hand_editbox_command(res_win_t widget, int code, int cid, void* data)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
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

void hand_editbox_erase(res_win_t widget, res_dc_t dc)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	if (!ptd->data)
		return;
}

void hand_editbox_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	xrect_t xr,xr_text;
	res_dc_t rdc;
	viewbox_t vb;
	xhand_t canv;

	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xbrush_t xb = { 0 };

	if (!ptd->data)
		return;

	widget_get_client_rect(widget, &xr);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);
	widget_get_xbrush(widget, &xb);

	canv = widget_get_canvas(widget);

	rdc = begin_display_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_viewport(widget, &vb);

	xr_text.x = -vb.bx;
	xr_text.y = -vb.by;
	xr_text.w = vb.bw;
	xr_text.h = vb.bh;

	if (ptd->data)
	{
		draw_var_text_raw(rdc, &xf, &xa, &xr_text, ptd->data);
	}

	if (ptd->b_select)
	{
		_editbox_draw_focus(widget, rdc);
	}

	end_display_paint(canv, dc, pxr);
}

/************************************************************************************************/
res_win_t editbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	default_xfont(&ev.xf);
	default_xface(&ev.xa);
	default_xpen(&ev.xp);
	default_xbrush(&ev.xb);

	ev.pf_on_create = hand_editbox_create;
	ev.pf_on_destroy = hand_editbox_destroy;
	ev.pf_on_set_focus = hand_editbox_set_focus;
	ev.pf_on_kill_focus = hand_editbox_kill_focus;
	ev.pf_on_command = hand_editbox_command;

	ev.pf_on_erase = hand_editbox_erase;
	ev.pf_on_paint = hand_editbox_paint;

	ev.pf_on_size = hand_editbox_size;

	ev.pf_on_horz_scroll = hand_editbox_horz_scroll;
	ev.pf_on_horz_wheel = hand_editbox_horz_wheel;
	ev.pf_on_vert_scroll = hand_editbox_vert_scroll;
	ev.pf_on_vert_wheel = hand_editbox_vert_wheel;

	ev.pf_on_keydown = hand_editbox_keydown;
	ev.pf_on_char = hand_editbox_char;

	ev.pf_on_mouse_move = hand_editbox_mousemove;
	ev.pf_on_lbutton_down = hand_editbox_lbutton_down;
	ev.pf_on_lbutton_up = hand_editbox_lbutton_up;
	ev.pf_on_lbutton_dbclick = hand_editbox_lbutton_dbclick;

	return widget_create(NULL, style, pxr, widget, &ev);
}

void editbox_redraw(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	int row, col;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->data)
		return;

	_editbox_calc_maxing(widget, NULL, NULL, &row, &col);
	ptd->sel_row = ptd->cur_row = row;
	ptd->sel_col = ptd->cur_col = col;

	_editbox_reset_page(widget);
	widget_invalid(widget, NULL, 0);	
	_editbox_reset_visible(widget);
	_editbox_reset_caret(widget);
}

void editbox_selectall(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	int row, col;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->data)
		return;

	_editbox_calc_maxing(widget, NULL, NULL, &row, &col);

	ptd->sel_row = 0;
	ptd->sel_col = 0;
	ptd->cur_row = row;
	ptd->cur_col = col;

	if (varstr_len(ptd->data))
		ptd->b_select = 1;
	else
		ptd->b_select = 0;

	_editbox_reset_caret(widget);

	widget_invalid(widget, NULL, 0);
}

void editbox_set_text(res_win_t widget, const tchar_t* text)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->data)
		return ;

	varstr_cpy(ptd->data, text, -1);

	editbox_redraw(widget);
}

int editbox_get_text(res_win_t widget, tchar_t* buf, int max)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);
	int len;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->data)
		return 0;

	len = varstr_len(ptd->data);
	if (buf)
	{
		len = (len < max) ? len : max;
		xsncpy(buf, varstr_ptr(ptd->data), len);
	}

	return len;
}

const tchar_t* editbox_get_text_ptr(res_win_t widget)
{
	editbox_delta_t* ptd = GETEDITBOXDELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->data)
		return NULL;

	return varstr_ptr(ptd->data);
}