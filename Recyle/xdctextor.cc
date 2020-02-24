/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc textor document

	@module	xdctextor.c | textor widnows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#include "xdctextor.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcclip.h"
#include "xdcgdi.h"
#include "xdccanv.h"

#define MAX_UNDO		7
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _TEXTOR_CALC{
	bool_t done;

	void *object,*org_object;
	int type, org_type;

	long x, y;
	long width, height;
	int from_row, from_col, to_row, to_col;
	xpoint_t from_pt, to_pt;
	tchar_t* buf;
	int total,max;

	res_dc_t rdc;
}TEXTOR_CALC;

int _var_text_calc_maxing(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _TEXTOR_SCAN_BEGIN:
		ptt->from_row = cur_row;
		ptt->from_col = cur_col;
		ptt->height = ptm->y;
		ptt->width = ptm->x;
		ptt->total = 0;
		break;
	case _TEXTOR_SCAN_END:
		ptt->to_row = cur_row;
		ptt->to_col = cur_col;
		ptt->height = ptm->y + ptm->cy + ptm->lh;
		ptt->total = cur_count;
		break;
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
		if (ptt->width < ptm->x + ptm->cx + ptm->lw)
			ptt->width = ptm->x + ptm->cx + ptm->lw;
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

void _textor_calc_maxing(textor_t* ptd, long pw, long ph, long* max_width, long* max_height, int* max_row, int* max_col, int* max_words)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	viewbox_t vb;
	res_dc_t rdc;

	vb.bx = 0;
	vb.by = 0;
	vb.bw = pw;
	vb.bh = ph;

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_maxing, (void*)&tt);

	widget_release_dc(ptd->widget, rdc);

	if (max_width)
		*max_width = tt.width - vb.bx;
	if (max_height)
		*max_height = tt.height - vb.by;
	if (max_row)
		*max_row = tt.to_row;
	if (max_col)
		*max_col = tt.to_col;
	if (max_words)
		*max_words = tt.total;
}

int _var_text_calc_position(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
		if (!ptt->done && ptt->object == object)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->done = 1;
			return _TEXTOR_OPERA_NEXT;
		}
		if (ptt->done && ptt->object == object)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			return _TEXTOR_OPERA_NEXT;
		}
		if (ptt->done && ptt->object != object)
		{
			return _TEXTOR_OPERA_STOP;
		}
		break;
	case _TEXTOR_SCAN_END:
		if (ptt->done && ptt->object == object)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

void _textor_calc_position(textor_t* ptd, void* object, int* from_row, int* from_col, int* to_row, int* to_col)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb;
	res_dc_t rdc;
	xfont_t xf;
	xface_t xa;

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	tt.object = object;
	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_position, (void*)&tt);
	widget_release_dc(ptd->widget, rdc);

	if (from_row)
		*from_row = tt.from_row;
	if (from_col)
		*from_col = tt.from_col;
	if (to_row)
		*to_row = tt.to_row;
	if (to_col)
		*to_col = tt.to_col;
}

int _var_text_calc_rect(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
		if (!ptt->done && ptt->object == object)
		{
			ptt->from_pt.x = ptm->x;
			ptt->from_pt.y = ptm->y;
			ptt->to_pt.x = ptm->x + ptm->cx;
			ptt->to_pt.y = ptm->y + ptm->cy;
			ptt->done = 1;
			return _TEXTOR_OPERA_NEXT;
		}
		if (ptt->done && ptt->object == object)
		{
			ptt->to_pt.x = ptm->x + ptm->cx;
			ptt->to_pt.y = ptm->y + ptm->cy;
			return _TEXTOR_OPERA_NEXT;
		}
		if (ptt->done && ptt->object != object)
		{
			return _TEXTOR_OPERA_STOP;
		}
		break;
	case _TEXTOR_SCAN_END:
		if (ptt->done && ptt->object == object)
		{
			ptt->to_pt.x = ptm->x + ptm->cx;
			ptt->to_pt.y = ptm->y + ptm->cy;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

void _textor_calc_rect(textor_t* ptd, void* object, xrect_t* pxr)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb;
	res_dc_t rdc;
	xfont_t xf;
	xface_t xa;

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	tt.object = object;
	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_rect, (void*)&tt);
	widget_release_dc(ptd->widget, rdc);

	if (pxr)
	{
		pxr->x = tt.from_pt.x;
		pxr->y = tt.from_pt.y;
		pxr->w = tt.to_pt.x - tt.from_pt.x;
		pxr->h = tt.to_pt.y - tt.from_pt.y;
	}
}

int _var_text_calc_hint(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
		if (ptt->y >= ptm->y && ptt->y < ptm->y + ptm->cy + ptm->lh && ptt->x < ptm->min_x + ptm->lw)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->object = object;
			ptt->type = type;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		else if (ptt->y >= ptm->y && ptt->y < ptm->y + ptm->cy + ptm->lh && ptt->x >= ptm->x && ptt->x < ptm->x + ptm->cx)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->object = object;
			ptt->type = type;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	/*case _TEXTOR_SCAN_CARRIAGE:
		if (ptt->y >= ptm->y && ptt->y < ptm->y + ptm->cy + ptm->lh)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->object = object;
			ptt->type = type;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		break;*/
	case _TEXTOR_SCAN_RETURN:
		if (ptt->y >= ptm->y && ptt->y < ptm->y + ptm->cy + ptm->lh)
		{
			ptt->from_row = cur_row + 1;
			ptt->from_col = 0;
			ptt->object = object;
			ptt->type = type;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	case _TEXTOR_SCAN_END:
		if (!ptt->done)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->object = object;
			ptt->type = type;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

void _textor_calc_hint(textor_t* ptd, long x, long y, int* row_at, int* col_at, void** pobj, int* ptype)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	res_dc_t rdc;

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	tt.x = x;
	tt.y = y;

	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_hint, (void*)&tt);
	widget_release_dc(ptd->widget, rdc);

	if (row_at)
		*row_at = tt.from_row;
	if (col_at)
		*col_at = tt.from_col;
	if (pobj)
		*pobj = tt.object;
	if (ptype)
		*ptype = tt.type;
}

int _var_text_calc_point(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _TEXTOR_SCAN_BEGIN:
		if (!ptt->from_row && !ptt->from_col)
		{
			ptt->from_pt.x = ptm->x;
			ptt->from_pt.y = ptm->y;
			ptt->to_pt.x = ptm->x;
			ptt->to_pt.y = ptm->y;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	case _TEXTOR_SCAN_END:
		if (!ptt->done)
		{
			ptt->from_pt.x = ptm->x;
			ptt->from_pt.y = ptm->y;
			ptt->to_pt.x = ptm->x + ptm->cx;
			ptt->to_pt.y = ptm->y + ptm->cy;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->from_pt.x = ptm->x;
			ptt->from_pt.y = ptm->y;
			ptt->to_pt.x = ptm->x + ptm->cx;
			ptt->to_pt.y = ptm->y + ptm->cy;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

void _textor_calc_point(textor_t* ptd, int row, int col, xpoint_t* ppt1, xpoint_t* ppt2)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	res_dc_t rdc;

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	tt.from_row = row;
	tt.from_col = col;

	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_point, (void*)&tt);
	widget_release_dc(ptd->widget, rdc);

	if (ppt1)
	{
		ppt1->x = tt.from_pt.x;
		ppt1->y = tt.from_pt.y;
	}
	if (ppt2)
	{
		ppt2->x = tt.to_pt.x;
		ppt2->y = tt.to_pt.y;
	}
}

int _var_text_calc_rowcol(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->object = object;
			ptt->type = type;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	case _TEXTOR_SCAN_RETURN:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->to_row = cur_row + 1;
			ptt->to_col = 0;
			ptt->object = object;
			ptt->type = type;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		else if (ptt->from_row == cur_row && ptt->from_col < 0)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col - 1;
			ptt->object = object;
			ptt->type = ptt->type;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		else if (ptt->from_row == cur_row && ptt->from_col > cur_col)
		{
			ptt->from_row = ptt->from_row + 1;
			ptt->from_col = ptt->from_col - cur_col ;
			return _TEXTOR_OPERA_NEXT;
		}
		break;
	case _TEXTOR_SCAN_END:
		if (!ptt->done)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->object = object;
			ptt->type = type;
			ptt->done = 1;
			return _TEXTOR_OPERA_STOP;
		}
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

void _textor_calc_rowcol(textor_t* ptd, int row, int col, int* prow, int* pcol, void** pobj, int* ptype)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	res_dc_t rdc;

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

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

	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_rowcol, (void*)&tt);
	widget_release_dc(ptd->widget, rdc);

	if (prow)
		*prow = tt.to_row;
	if (pcol)
		*pcol = tt.to_col;
	if (pobj)
		*pobj = tt.object;
	if (ptype)
		*ptype = tt.type;
}

int _var_text_calc_select(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;
	int n;

	switch(scan)
	{
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
		if (inside_rowcol(cur_row, cur_col, ptt->from_row, ptt->from_col, ptt->to_row, ptt->to_col) && b_sel)
		{
			n = xslen(ret_char);

			if (ptt->total + n > ptt->max)
				return _TEXTOR_OPERA_STOP;

			if (ptt->buf)
			{
				xsncpy(ptt->buf + ptt->total, ret_char, n);
			}
			ptt->total = ptt->total + n;
		}
		else if (compare_rowcol(cur_row, cur_col, ptt->to_row, ptt->to_col) > 0)
		{
			return _TEXTOR_OPERA_STOP;
		}
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

int _textor_calc_select(textor_t* ptd, int from_row, int from_col, int to_row, int to_col, tchar_t* buf, int max)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	res_dc_t rdc;

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	tt.buf = buf;
	tt.max = max;
	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.to_row = to_row;
	tt.to_col = to_col;

	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_select, (void*)&tt);
	widget_release_dc(ptd->widget, rdc);

	return tt.total;
}

int _var_text_calc_delete(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;
	int n;

	switch(scan)
	{
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
		if (cur_row == ptt->from_row && cur_col == ptt->from_col  && b_del)
		{
			if (ptt->total > 0)
			{
				n = xslen(ret_char);
				ptt->total = ptt->total - n;
				return _TEXTOR_OPERA_DEL;
			}
			else
			{
				return _TEXTOR_OPERA_STOP;
			}
		}
		else if (compare_rowcol(cur_row, cur_col, ptt->from_row, ptt->from_col) > 0)
		{
			return _TEXTOR_OPERA_STOP;
		}
		break;
	}
	
	return _TEXTOR_OPERA_NEXT;
}

bool_t _textor_calc_delete(textor_t* ptd, int from_row, int from_col, int to_row, int to_col)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	res_dc_t rdc;

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.total = _textor_calc_select(ptd, from_row, from_col, to_row, to_col, NULL, MAX_LONG);

	if (!tt.total)
		return 0;

	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_delete, (void*)&tt);
	widget_release_dc(ptd->widget, rdc);

	return 1;
}

int _var_text_calc_insert(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;
	int n;

	switch (scan)
	{
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
	case _TEXTOR_SCAN_END:
		if (cur_row == ptt->from_row && cur_col == ptt->from_col && b_ins)
		{
			if (ptt->total)
			{
				n = wschs(ptt->buf + ptt->total - 1);
				xsncpy(ret_char, ptt->buf + ptt->total - 1, n);
				ptt->total = ptt->total - n;
				return _TEXTOR_OPERA_INS;
			}
			else
			{
				return _TEXTOR_OPERA_STOP;
			}
		}
		else if (compare_rowcol(cur_row, cur_col, ptt->from_row, ptt->from_col) > 0)
		{
			return _TEXTOR_OPERA_STOP;
		}
		break;
	case _TEXTOR_SCAN_RETURN:
		if (cur_row == ptt->from_row && cur_col == ptt->from_col)
		{
			ptt->from_row = cur_row + 1;
			ptt->from_col = 0;
		}
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

bool_t _textor_calc_insert(textor_t* ptd, int* prow, int* pcol, tchar_t* str, int len)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	res_dc_t rdc;

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	if (len < 0)
		len = xslen(str);

	tt.from_row = *prow;
	tt.from_col = *pcol;
	tt.buf = str;
	tt.total = len;

	rdc = widget_client_dc(ptd->widget);

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_insert, (void*)&tt);
	widget_release_dc(ptd->widget, rdc);

	*prow = tt.from_row;
	*pcol = tt.from_col;

	return (tt.total == len) ? 0 : 1;
}

int _var_text_calc_focus(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;
	xrect_t xr_reg, xr_dot;
	xcolor_t xc;

	switch (scan)
	{
	case _TEXTOR_SCAN_INDENT:
	case _TEXTOR_SCAN_WORDS:
	case _TEXTOR_SCAN_RETURN:
	case _TEXTOR_SCAN_END:
		if (compare_rowcol(ptt->from_row, ptt->from_col, ptt->to_row, ptt->to_col) > 0)
		{
			return _TEXTOR_OPERA_STOP;
		}

		if (!ptt->done && ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->from_pt.x = ptm->x;
			ptt->from_pt.y = ptm->y;
			ptt->done = 1;

			xr_dot.x = ptt->from_pt.x;
			xr_dot.y = ptt->from_pt.y - TEXT_FOCUS_SPAN;
			xr_dot.w = TEXT_FOCUS_SPAN;
			xr_dot.h = TEXT_FOCUS_SPAN;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			alpha_rect_raw(ptt->rdc, &xc, &xr_dot, ALPHA_SOLID);
		}

		if (ptt->done && scan == _TEXTOR_SCAN_RETURN)
		{
			ptt->to_pt.x = ptm->x + ptm->cx;
			ptt->to_pt.y = ptm->y + ptm->cy;

			xr_reg.x = ptt->from_pt.x;
			xr_reg.y = ptt->from_pt.y;
			xr_reg.w = ptt->to_pt.x - ptt->from_pt.x;
			xr_reg.h = ptt->to_pt.y - ptt->from_pt.y;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			alpha_rect_raw(ptt->rdc, &xc, &xr_reg, ALPHA_SOFT);

			ptt->from_pt.x = ptm->min_x + ptm->lw;
			ptt->from_pt.y = ptm->y + ptm->cy + ptm->lh;

			return _TEXTOR_OPERA_NEXT;
		}

		if (ptt->done && ptt->to_row == cur_row && ptt->to_col == cur_col)
		{
			ptt->to_pt.x = ptm->x + ptm->cx;
			ptt->to_pt.y = ptm->y + ptm->cy;

			xr_reg.x = ptt->from_pt.x;
			xr_reg.y = ptt->from_pt.y;
			xr_reg.w = ptt->to_pt.x - ptt->from_pt.x;
			xr_reg.h = ptt->to_pt.y - ptt->from_pt.y;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			alpha_rect_raw(ptt->rdc, &xc, &xr_reg, ALPHA_SOFT);
			
			xr_dot.x = ptt->to_pt.x - TEXT_FOCUS_SPAN;
			xr_dot.y = ptt->to_pt.y;
			xr_dot.w = TEXT_FOCUS_SPAN;
			xr_dot.h = TEXT_FOCUS_SPAN;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			alpha_rect_raw(ptt->rdc, &xc, &xr_dot, ALPHA_SOLID);
			return _TEXTOR_OPERA_STOP;
		}

		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

void _textor_draw_focus(textor_t* ptd, res_dc_t rdc)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb = { 0 };

	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	tt.from_row = ptd->sel_row;
	tt.from_col = ptd->sel_col;
	tt.to_row = ptd->cur_row;
	tt.to_col = ptd->cur_col;

	tt.rdc = rdc;

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_focus, (void*)&tt);
}

int _var_text_calc_paint(int scan, void* object, int type, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, tchar_t* ret_char, int cur_row, int cur_col, int cur_count, const TEXTMEASURE* ptm, const xfont_t* pxf,const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;
	xcolor_t xc = { 0 };
	xpoint_t pt1, pt2;
	xpen_t xp;

	switch (scan)
	{
	case _TEXTOR_SCAN_WORDS:
		if (cur_char && *cur_char == _T('\n'))
		{
			default_xpen(&xp);
			xscpy(xp.color, pxa->text_color);
			lighten_xpen(&xp, DEF_SOFT_LIGHTEN);

			pt1.x = ptm->x + ptm->cx;
			pt1.y = ptm->y + ptm->cy;
			pt2.x = pt1.x - 3;
			pt2.y = pt1.y;
			draw_line_raw(ptt->rdc, &xp, &pt1, &pt2);

			pt2.x = pt1.x;
			pt2.y = pt1.y - 4;
			draw_line_raw(ptt->rdc, &xp, &pt1, &pt2);
		}
		else
		{
			pt1.x = ptm->x;
			pt1.y = ptm->y;

			parse_xcolor(&xc, pxa->text_color);
			text_out_raw(ptt->rdc, pxf, &xc, &pt1, cur_char, -1);
		}
		break;
	}

	return _TEXTOR_OPERA_NEXT;
}

void _textor_draw_text(textor_t* ptd, res_dc_t rdc)
{
	TEXTOR_CALC tt = { 0 };
	if_measure_t it = { 0 };

	viewbox_t vb = { 0 };

	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	widget_get_view_rect(ptd->widget, &vb);

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	tt.from_row = ptd->sel_row;
	tt.from_col = ptd->sel_col;
	tt.to_row = ptd->cur_row;
	tt.to_col = ptd->cur_col;

	tt.rdc = rdc;

	it.dc = (void*)rdc;
	it.pf_text_metric = (PF_TEXT_METRIC)text_metric_raw;
	it.pf_text_size = (PF_TEXT_SIZE)text_size_raw;

	(*ptd->pf_scan_text)(ptd->data, &it, &xf, &xa, vb.bx, vb.by, vb.bw, vb.bh, _var_text_calc_paint, (void*)&tt);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void _textor_reset_caret(textor_t* ptd)
{
	xpoint_t pt1, pt2;

	_textor_calc_point(ptd, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	widget_show_caret(ptd->widget, pt1.x, pt1.y, 1);
}

void _textor_reset_page(textor_t* ptd)
{
	xrect_t xr;
	long page_width, page_height;

	long line_feed;
	float line_height;
	xsize_t xs = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	res_dc_t rdc;

	widget_get_xfont(ptd->widget, &xf);
	widget_get_xface(ptd->widget, &xa);

	line_height = (float)xstof(xa.line_height);
	if (!line_height)
		line_height = 1.0;

	rdc = widget_client_dc(ptd->widget);
	text_metric_raw(rdc, &xf, &xs.cx, &xs.cy);
	widget_release_dc(ptd->widget, rdc);

	line_feed = (long)(xs.cy * line_height);

	widget_get_client_rect(ptd->widget, &xr);

	_textor_calc_maxing(ptd, xr.w, xr.h, &page_width, &page_height, NULL, NULL, NULL);

	if (page_width < xr.w)
		page_width = xr.w;
	if (page_height < xr.h)
		page_height = xr.h;

	widget_reset_paging(ptd->widget, xr.w, xr.h, page_width, page_height, line_feed, line_feed);

	widget_reset_scroll(ptd->widget, 1);

	widget_reset_scroll(ptd->widget, 0);
}

void _textor_ensure_visible(textor_t* ptd, int row, int col)
{
	xpoint_t pt1, pt2;
	xrect_t xr;
	xface_t xa = { 0 };

	_textor_calc_point(ptd, row, col, &pt1, &pt2);

	xr.x = pt1.x;
	xr.y = pt1.y;
	xr.w = pt2.x - pt1.x;
	xr.h = pt2.y - pt1.y;

	widget_get_xface(ptd->widget, &xa);

	if (is_null(xa.text_wrap))
		widget_ensure_visible(ptd->widget, &xr, 0);
	else
		widget_ensure_visible(ptd->widget, &xr, 1);
}

void _textor_done(textor_t* ptd)
{
	TEXTOR_UNDO *pnew,*pnxt;
	int count = 0;

	if (!ptd->max_undo)
		return;

	pnew = (TEXTOR_UNDO*)xmem_alloc(sizeof(TEXTOR_UNDO));

	pnew->b_select = ptd->b_select;
	pnew->sel_row = ptd->sel_row;
	pnew->sel_col = ptd->sel_col;
	pnew->cur_row = ptd->cur_row;
	pnew->cur_col = ptd->cur_col;

	pnew->len = (*ptd->pf_get_text)(ptd->data, NULL, MAX_LONG);
	pnew->text = xsalloc(pnew->len + 1);
	(*ptd->pf_get_text)(ptd->data, pnew->text, pnew->len);

	pnew->next = ptd->ptu;
	ptd->ptu = pnew;

	while (count ++ < ptd->max_undo && pnew)
	{
		pnxt = pnew;
		pnew = pnew->next;
	}

	if (pnew)
	{
		pnxt->next = NULL;
	}

	while (pnew)
	{
		pnxt = pnew->next;

		xsfree(pnew->text);
		xmem_free(pnew);

		pnew = pnxt;
	}
}

void _textor_discard(textor_t* ptd)
{
	TEXTOR_UNDO *prev;

	if (ptd->ptu)
	{
		prev = ptd->ptu;
		ptd->ptu = prev->next;
		
		xsfree(prev->text);
		xmem_free(prev);
	}
}

void _textor_clean(textor_t* ptd)
{
	TEXTOR_UNDO *next;

	while (ptd->ptu)
	{
		next = ptd->ptu->next;

		xsfree(ptd->ptu->text);
		xmem_free(ptd->ptu);

		ptd->ptu = next;
	}
}


/********************************************************************************************/
int hand_textor_create(textor_t* ptd, void* data)
{
	XDC_ASSERT(ptd);

	return 0;
}

void hand_textor_destroy(textor_t* ptd)
{
	_textor_clean(ptd);
}

void hand_textor_set_focus(textor_t* ptd, res_win_t wt)
{
	xfont_t xf;
	xsize_t xs;
	res_dc_t rdc;

	XDC_ASSERT(ptd && ptd->widget);

	widget_get_xfont(ptd->widget, &xf);

	rdc = widget_client_dc(ptd->widget);
	text_metric_raw(rdc, &xf, &xs.cx, &xs.cy);
	widget_release_dc(ptd->widget, rdc);

	widget_create_caret(ptd->widget, 2, xs.cy);
}

void hand_textor_kill_focus(textor_t* ptd, res_win_t wt)
{
	XDC_ASSERT(ptd && ptd->widget);

	widget_destroy_caret(ptd->widget);
}

int hand_textor_char(textor_t* ptd, tchar_t ch)
{
	int row, col;
	void* object = NULL;
	int type = 0;
	tchar_t token[2] = { 0 };
	xface_t xa = { 0 };

	XDC_ASSERT(ptd && ptd->widget);

	widget_get_xface(ptd->widget, &xa);

	if (ch == KEY_ENTER)
	{
		if (is_null(xa.text_wrap))
			return PRESS_IGNORE;

		ch = _T('\n');
	}
	else if (ch == KEY_TAB)
	{
		if (is_null(xa.text_wrap))
			return PRESS_IGNORE;

		ch = _T('\t');
	}
	else if (ch < 32)
		return PRESS_IGNORE;

	_textor_done(ptd);

	if (ptd->b_select)
	{
		_textor_calc_delete(ptd, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col);

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;
		ptd->b_select = 0;
	}

	token[0] = ch;

	_textor_calc_insert(ptd, &ptd->cur_row, &ptd->cur_col, token, 1);
	_textor_calc_rowcol(ptd, ptd->cur_row, ptd->cur_col + 1, &row, &col, &object, &type);

	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = object;
	ptd->type = type;

	_textor_reset_page(ptd);
	widget_invalid(ptd->widget, NULL, 0);

	_textor_ensure_visible(ptd, ptd->cur_row,ptd->cur_col);
	_textor_reset_caret(ptd);

	return PRESS_ACCEPT;
}

void hand_textor_lbutton_down(textor_t* ptd, const xpoint_t* pxp)
{
	int row = 0;
	int col = 0;
	void* object = NULL;
	int type = 0;
	xpoint_t pt1, pt2;

	XDC_ASSERT(ptd && ptd->widget);

	if (!widget_is_focus(ptd->widget))
		widget_set_focus(ptd->widget);

	widget_set_capture(ptd->widget, 1);

	if (ptd->b_select)
	{
		_textor_calc_point(ptd, ptd->sel_row, ptd->sel_col, &pt1, &pt2);
		if (pt_inside(pxp->x, pxp->y, pt1.x - TEXT_FOCUS_SPAN, pt1.y - TEXT_FOCUS_SPAN, pt1.x + TEXT_FOCUS_SPAN, pt2.y + TEXT_FOCUS_SPAN))
		{
			ptd->n_drag = _TEXTOR_DRAG_FIRST;
			return;
		}

		_textor_calc_point(ptd, ptd->cur_row, ptd->cur_col, &pt1, &pt2);
		if (pt_inside(pxp->x, pxp->y, pt2.x - TEXT_FOCUS_SPAN, pt1.y - TEXT_FOCUS_SPAN, pt2.x + TEXT_FOCUS_SPAN, pt2.y + TEXT_FOCUS_SPAN))
		{
			ptd->n_drag = _TEXTOR_DRAG_LAST;
			return;
		}

		ptd->b_select = 0;
		widget_invalid(ptd->widget, NULL, 0);
	}

	_textor_calc_hint(ptd, pxp->x, pxp->y, &row, &col, &object, &type);

	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = object;
	ptd->type;

	_textor_reset_caret(ptd);
}

void hand_textor_lbutton_up(textor_t* ptd, const xpoint_t* pxp)
{
	XDC_ASSERT(ptd && ptd->widget);

	widget_set_capture(ptd->widget, 0);

	if (ptd->n_drag)
	{
		ptd->n_drag = 0;
		widget_invalid(ptd->widget, NULL, 0);
		return;
	}
}

void hand_textor_lbutton_dbclick(textor_t* ptd, const xpoint_t* pxp)
{
	XDC_ASSERT(ptd && ptd->widget);

	if (ptd->object)
		hand_textor_selectobj(ptd);
	else
		hand_textor_selectcur(ptd);
}

void hand_textor_mousemove(textor_t* ptd, dword_t mk, const xpoint_t* ppt)
{
	int row, col;
	void* object = NULL;
	int type = 0;

	XDC_ASSERT(ptd && ptd->widget);

	if (ptd->n_drag == _TEXTOR_DRAG_FIRST)
	{
		_textor_calc_hint(ptd, ppt->x, ppt->y, &row, &col, NULL, NULL);
		if (row >= ptd->cur_row && col >= ptd->cur_col)
			return;

		ptd->sel_row = row;
		ptd->sel_col = col;

		widget_invalid(ptd->widget, NULL, 0);

		_textor_ensure_visible(ptd, ptd->sel_row, ptd->sel_col);
	}
	else if (ptd->n_drag == _TEXTOR_DRAG_LAST)
	{
		_textor_calc_hint(ptd, ppt->x, ppt->y, &row, &col, &object, &type);
		if (row <= ptd->sel_row && col <= ptd->sel_col)
			return;

		ptd->cur_row = row;
		ptd->cur_col = col;
		ptd->object = object;
		ptd->type = type;

		widget_invalid(ptd->widget, NULL, 0);

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	}
	else if (mk & MK_LBUTTON)
	{
		if (ptd->b_select)
		{
			_textor_calc_hint(ptd, ppt->x, ppt->y, &row, &col, &object, &type);

			if (compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) >= 0)
			{
				ptd->n_drag = _TEXTOR_DRAG_LAST;
			}
			else if (compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
			{
				ptd->n_drag = _TEXTOR_DRAG_FIRST;
			}
		}
		else
		{
			hand_textor_selectcur(ptd);
		}
	}
}

void hand_textor_size(textor_t* ptd, int code, const xsize_t* prs)
{
	XDC_ASSERT(ptd && ptd->widget);

	hand_textor_redraw(ptd);
}

void hand_textor_scroll(textor_t* ptd, bool_t bHorz, long nLine)
{
	XDC_ASSERT(ptd && ptd->widget);

	widget_hand_scroll(ptd->widget, bHorz, nLine);

	_textor_reset_caret(ptd);
}

void hand_textor_paint(textor_t* ptd, res_dc_t dc, const xrect_t* pxr)
{
	xrect_t xr;
	res_dc_t rdc;
	xhand_t canv;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };

	XDC_ASSERT(ptd && ptd->widget);

	widget_get_xbrush(ptd->widget, &xb);
	widget_get_xpen(ptd->widget, &xp);

	canv = widget_get_canvas(ptd->widget);

	XDC_ASSERT(canv);

	widget_get_client_rect(ptd->widget, &xr);

	rdc = begin_display_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	_textor_draw_text(ptd, rdc);

	if (ptd->b_select)
	{
		_textor_draw_focus(ptd, rdc);
	}

	end_display_paint(canv, dc, pxr);
}

void hand_textor_selectall(textor_t* ptd)
{
	int row, col, count;
	viewbox_t vb = { 0 };

	XDC_ASSERT(ptd && ptd->widget);

	widget_get_view_rect(ptd->widget, &vb);

	_textor_calc_maxing(ptd, vb.bw, vb.bh, NULL, NULL, &row, &col, &count);

	if (!count)
		return;

	ptd->sel_row = 0;
	ptd->sel_col = 0;
	ptd->cur_row = row;
	ptd->cur_col = col;

	ptd->b_select = 1;

	_textor_reset_caret(ptd);

	widget_invalid(ptd->widget, NULL, 0);
}

void hand_textor_selectcur(textor_t* ptd)
{
	XDC_ASSERT(ptd && ptd->widget);

	ptd->b_select = 1;

	widget_invalid(ptd->widget, NULL, 0);
}

void hand_textor_selectobj(textor_t* ptd)
{
	int from_row, from_col, to_row, to_col;

	XDC_ASSERT(ptd && ptd->widget);

	if (!ptd->object)
		return;

	_textor_calc_position(ptd, ptd->object, &from_row, &from_col, &to_row, &to_col);

	ptd->sel_row = from_row;
	ptd->sel_col = from_col;
	ptd->cur_row = to_row;
	ptd->cur_col = to_col;

	ptd->b_select = 1;

	_textor_reset_caret(ptd);

	widget_invalid(ptd->widget, NULL, 0);
}

void hand_textor_redraw(textor_t* ptd)
{
	int row, col;
	int type;
	viewbox_t vb = { 0 };

	XDC_ASSERT(ptd && ptd->widget);

	_textor_reset_page(ptd);

	widget_update_window(ptd->widget);

	widget_get_view_rect(ptd->widget, &vb);

	_textor_calc_maxing(ptd, vb.bw, vb.bh, NULL, NULL, &row, &col, NULL);

	if (!inside_rowcol(ptd->cur_row, ptd->cur_col, 0, 0, row, col))
	{
		ptd->sel_row = ptd->cur_row = row;
		ptd->sel_col = ptd->cur_col = col;
		ptd->b_select = 0;
	}

	_textor_calc_rowcol(ptd, ptd->cur_row, ptd->cur_col, &row, &col, &ptd->object, &type);

	widget_invalid(ptd->widget, NULL, 0);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);
}

int hand_textor_back(textor_t* ptd)
{
	int sel_row, sel_col, cur_row, cur_col;
	void* object = NULL;
	int type = 0;

	XDC_ASSERT(ptd && ptd->widget);

	if (!ptd->b_select)
	{
		_textor_calc_rowcol(ptd, ptd->cur_row, ptd->cur_col - 1, &sel_row, &sel_col, NULL, NULL);
		if (sel_col < 0)
			return PRESS_IGNORE;

		cur_row = sel_row;
		cur_col = sel_col;
	}
	else
	{
		sel_row = ptd->sel_row;
		sel_col = ptd->sel_col;
		cur_row = ptd->cur_row;
		cur_col = ptd->cur_col;
	}

	_textor_done(ptd);

	if (!_textor_calc_delete(ptd, sel_row, sel_col, cur_row, cur_col))
	{
		_textor_discard(ptd);
		return PRESS_IGNORE;
	}

	_textor_calc_rowcol(ptd, sel_row, sel_col, &cur_row, &cur_col, &object, &type);

	ptd->sel_row = cur_row;
	ptd->sel_col = cur_col;
	ptd->cur_row = cur_row;
	ptd->cur_col = cur_col;
	ptd->object = object;
	ptd->type = type;

	ptd->b_select = 0;
	_textor_reset_page(ptd);
	widget_invalid(ptd->widget, NULL, 0);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return PRESS_ACCEPT;
}

int hand_textor_delete(textor_t* ptd)
{
	int sel_row, sel_col, cur_row, cur_col;
	void* object = NULL;
	int type = 0;

	XDC_ASSERT(ptd && ptd->widget);

	if (!ptd->b_select)
	{
		sel_row = ptd->cur_row;
		sel_col = ptd->cur_col;
		cur_row = ptd->cur_row;
		cur_col = ptd->cur_col;
	}
	else
	{
		sel_row = ptd->sel_row;
		sel_col = ptd->sel_col;
		cur_row = ptd->cur_row;
		cur_col = ptd->cur_col;
	}

	_textor_done(ptd);

	if (!_textor_calc_delete(ptd, sel_row, sel_col, cur_row, cur_col))
	{
		_textor_discard(ptd);
		return PRESS_IGNORE;
	}

	_textor_calc_rowcol(ptd, sel_row, sel_col, &cur_row, &cur_col, &object, &type);

	ptd->sel_row = cur_row;
	ptd->sel_col = cur_col;
	ptd->cur_row = cur_row;
	ptd->cur_col = cur_col;
	ptd->object = object;
	ptd->type = type;

	ptd->b_select = 0;
	_textor_reset_page(ptd);
	widget_invalid(ptd->widget, NULL, 0);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return PRESS_ACCEPT;
}

int hand_textor_escape(textor_t* ptd)
{
	XDC_ASSERT(ptd && ptd->widget);

	if (ptd->b_select)
	{
		ptd->b_select = 0;
		widget_invalid(ptd->widget, NULL, 0);

		return PRESS_HANDLE;
	}

	return PRESS_IGNORE;
}

int hand_textor_left(textor_t* ptd)
{
	int row, col;
	void* object = NULL;
	int type = 0;

	XDC_ASSERT(ptd && ptd->widget);

	if (ptd->b_select)
	{
		row = ptd->sel_row;
		col = ptd->sel_col;
	}
	else
	{
		_textor_calc_rowcol(ptd, ptd->cur_row, ptd->cur_col - 1, &row, &col, &object, &type);
	}

	if (col < 0)
		return PRESS_IGNORE;

	if (row != ptd->cur_row || col != ptd->cur_col)
	{
		if (widget_key_state(ptd->widget, KEY_SHIFT))
		{
			if (compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
				return PRESS_IGNORE;

			ptd->cur_row = row;
			ptd->cur_col = col;
			ptd->object = object;
			ptd->type = type;
			ptd->b_select = 1;
			widget_invalid(ptd->widget, NULL, 0);
		}
		else
		{
			ptd->cur_row = row;
			ptd->cur_col = col;
			ptd->sel_row = row;
			ptd->sel_col = col;
			ptd->object = object;
			ptd->type = type;
			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(ptd->widget, NULL, 0);
			}
		}

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

		_textor_reset_caret(ptd);
	}
	else if (!widget_key_state(ptd->widget, KEY_SHIFT))
	{
		if (ptd->b_select)
		{
			ptd->b_select = 0;
			widget_invalid(ptd->widget, NULL, 0);
		}
	}

	return PRESS_HANDLE;
}

int hand_textor_right(textor_t* ptd)
{
	int row, col;
	void* object = NULL;
	int type = 0;

	XDC_ASSERT(ptd && ptd->widget);

	_textor_calc_rowcol(ptd, ptd->cur_row, ptd->cur_col + 1, &row, &col, &object, &type);

	if (row != ptd->cur_row || col != ptd->cur_col)
	{
		if (widget_key_state(ptd->widget, KEY_SHIFT))
		{
			ptd->cur_row = row;
			ptd->cur_col = col;
			ptd->object = object;
			ptd->type = type;
			ptd->b_select = 1;
			widget_invalid(ptd->widget, NULL, 0);
		}
		else
		{
			ptd->cur_row = row;
			ptd->cur_col = col;
			ptd->sel_row = row;
			ptd->sel_col = col;
			ptd->object = object;
			ptd->type = type;
			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(ptd->widget, NULL, 0);
			}
		}

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

		_textor_reset_caret(ptd);
	}
	else if (!widget_key_state(ptd->widget, KEY_SHIFT))
	{
		if (ptd->b_select)
		{
			ptd->b_select = 0;
			widget_invalid(ptd->widget, NULL, 0);
		}
	}

	return PRESS_HANDLE;
}

int hand_textor_up(textor_t* ptd)
{
	int row, col;
	void* object = NULL;
	int type = 0;

	XDC_ASSERT(ptd && ptd->widget);

	_textor_calc_rowcol(ptd, ptd->cur_row - 1, ptd->cur_col, &row, &col, &object, &type);
	if (col < 0)
		return PRESS_IGNORE;

	if (row != ptd->cur_row || col != ptd->cur_col)
	{
		if (widget_key_state(ptd->widget, KEY_SHIFT))
		{
			if (compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
				return PRESS_IGNORE;

			ptd->cur_row = row;
			ptd->cur_col = col;
			ptd->object = object;
			ptd->type = type;
			ptd->b_select = 1;
			widget_invalid(ptd->widget, NULL, 0);
		}
		else
		{
			ptd->cur_row = row;
			ptd->cur_col = col;
			ptd->sel_row = row;
			ptd->sel_col = col;
			ptd->object = object;
			ptd->type = type;
			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(ptd->widget, NULL, 0);
			}
		}

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

		_textor_reset_caret(ptd);
	}
	else if (!widget_key_state(ptd->widget, KEY_SHIFT))
	{
		if (ptd->b_select)
		{
			ptd->b_select = 0;
			widget_invalid(ptd->widget, NULL, 0);
		}
	}

	return PRESS_HANDLE;
}

int hand_textor_down(textor_t* ptd)
{
	int row, col;
	void* object = NULL;
	int type = 0;

	XDC_ASSERT(ptd && ptd->widget);

	_textor_calc_rowcol(ptd, ptd->cur_row + 1, ptd->cur_col, &row, &col, &object, &type);

	if (row != ptd->cur_row || col != ptd->cur_col)
	{
		if (widget_key_state(ptd->widget, KEY_SHIFT))
		{
			ptd->cur_row = row;
			ptd->cur_col = col;
			ptd->object = object;
			ptd->type = type;
			ptd->b_select = 1;
			widget_invalid(ptd->widget, NULL, 0);
		}
		else
		{
			ptd->cur_row = row;
			ptd->cur_col = col;
			ptd->sel_row = row;
			ptd->sel_col = col;
			ptd->object = object;
			ptd->type = type;
			if (ptd->b_select)
			{
				ptd->b_select = 0;
				widget_invalid(ptd->widget, NULL, 0);
			}
		}

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

		_textor_reset_caret(ptd);
	}
	else if (!widget_key_state(ptd->widget, KEY_SHIFT))
	{
		if (ptd->b_select)
		{
			ptd->b_select = 0;
			widget_invalid(ptd->widget, NULL, 0);
		}
	}

	return PRESS_HANDLE;
}

int hand_textor_undo(textor_t* ptd)
{
	TEXTOR_UNDO *next;
	int row, col;
	void* object = NULL;
	int type;

	if (!ptd->ptu)
		return PRESS_IGNORE;

	next = ptd->ptu->next;

	ptd->b_select = ptd->ptu->b_select;
	ptd->sel_row = ptd->ptu->sel_row;
	ptd->sel_col = ptd->ptu->sel_col;
	ptd->cur_row = ptd->ptu->cur_row;
	ptd->cur_col = ptd->ptu->cur_col;
	(*ptd->pf_set_text)(ptd->data, ptd->ptu->text, ptd->ptu->len);

	_textor_calc_rowcol(ptd, ptd->sel_row, ptd->sel_col, &row, &col, &object, &type);
	ptd->object = object;

	xsfree(ptd->ptu->text);
	xmem_free(ptd->ptu);
	ptd->ptu = next;

	_textor_reset_page(ptd);

	widget_invalid(ptd->widget, NULL, 0);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return PRESS_ACCEPT;
}

int hand_textor_copy(textor_t* ptd)
{
	res_glob_t gb;
	int len = 0;
	byte* buf;

	if (!ptd->b_select)
		return PRESS_IGNORE;

	len = _textor_calc_select(ptd, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, NULL, MAX_LONG);
	if (!len)
	{
		return PRESS_IGNORE;
	}

	if (!clipboard_open())
		return PRESS_IGNORE;

	gb = system_alloc((len + 1)* sizeof(tchar_t));
	buf = (byte_t*)system_lock(gb);

	_textor_calc_select(ptd, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, (tchar_t*)buf, len);

	system_unlock(gb);

	if (!clipboard_put(FMT_UCS, gb))
	{
		system_free(gb);
		clipboard_close();

		return PRESS_IGNORE;
	}

	clipboard_close();

	return PRESS_HANDLE;
}

int hand_textor_cut(textor_t* ptd)
{
	int row, col;
	void* object = NULL;
	int type = 0;

	if (!ptd->b_select)
		return PRESS_IGNORE;

	_textor_done(ptd);

	hand_textor_copy(ptd);

	if (!_textor_calc_delete(ptd, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col))
	{
		_textor_discard(ptd);
		return PRESS_IGNORE;
	}

	_textor_calc_rowcol(ptd, ptd->sel_row, ptd->sel_col, &row, &col, &object, &type);

	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->object = object;
	ptd->type = type;

	ptd->b_select = 0;

	_textor_reset_page(ptd);

	widget_invalid(ptd->widget, NULL, 0);

	_textor_reset_caret(ptd);

	return PRESS_ACCEPT;
}

int hand_textor_paste(textor_t* ptd)
{
	res_glob_t gb;
	byte* buf;
	int len;
	int row, col;
	void* object = NULL;
	int type = 0;

	if (!clipboard_open())
		return PRESS_IGNORE;

	gb = clipboard_get(FMT_UCS);
	if (!gb)
	{
		clipboard_close();
		return PRESS_IGNORE;
	}

	buf = (byte_t*)system_lock(gb);
	len = xslen((tchar_t*)buf);
	if (!len)
	{
		clipboard_close();
		return PRESS_IGNORE;
	}

	_textor_done(ptd);

	if (ptd->b_select)
	{
		if (!_textor_calc_delete(ptd, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col))
		{
			_textor_discard(ptd);

			clipboard_close();
			return PRESS_IGNORE;
		}

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;
	}

	if (!_textor_calc_insert(ptd, &ptd->cur_row, &ptd->cur_col, (tchar_t*)buf, len))
	{
		_textor_discard(ptd);

		clipboard_close();
		return PRESS_IGNORE;
	}

	_textor_calc_rowcol(ptd, ptd->cur_row, ptd->cur_col + len, &row, &col, &object, &type);
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->object = object;
	ptd->type = type;

	system_unlock(gb);

	clipboard_clean();
	clipboard_close();

	ptd->b_select = 0;

	_textor_reset_page(ptd);

	widget_invalid(ptd->widget, NULL, 0);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return PRESS_ACCEPT;
}

void hand_textor_clean(textor_t* ptd)
{
	_textor_clean(ptd);
}

void hand_textor_done(textor_t* ptd)
{
	_textor_done(ptd);
}

void hand_textor_discard(textor_t* ptd)
{
	_textor_discard(ptd);
}

int hand_textor_selected_text(textor_t* ptd, tchar_t* buf, int max)
{
	if (!ptd->b_select)
		return 0;

	return _textor_calc_select(ptd, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, buf, max);
}

int hand_textor_replace_text(textor_t* ptd, const tchar_t* token, int len)
{
	int row, col;
	void* object = NULL;
	int type = 0;

	if (!ptd->b_select)
		return PRESS_IGNORE;

	_textor_done(ptd);

	if (!_textor_calc_delete(ptd, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col))
	{
		_textor_discard(ptd);

		return PRESS_IGNORE;
	}

	ptd->cur_row = ptd->sel_row;
	ptd->cur_col = ptd->sel_col;

	if (len < 0)
		len = xslen(token);

	if (!_textor_calc_insert(ptd, &ptd->cur_row, &ptd->cur_col, (tchar_t*)token, len))
	{
		_textor_discard(ptd);

		return PRESS_IGNORE;
	}

	_textor_calc_rowcol(ptd, ptd->cur_row, ptd->cur_col + len, &row, &col, &object, &type);
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->object = object;
	ptd->type = type;

	ptd->b_select = 0;

	_textor_reset_page(ptd);

	widget_invalid(ptd->widget, NULL, 0);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return PRESS_ACCEPT;
}

void get_object_rect(textor_t* ptd, void* object, xrect_t* pxr)
{
	_textor_calc_rect(ptd, object, pxr);
}

void get_cursor_point(textor_t* ptd, xpoint_t* ppt)
{
	xpoint_t pt1, pt2;

	_textor_calc_point(ptd, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	ppt->x = pt2.x;
	ppt->y = pt2.y;
}

bool_t textor_is_select(textor_t* ptd)
{
	return ptd->b_select;
}