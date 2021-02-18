/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc textor document

	@module	textor.c | implement file

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

#include "textor.h"
#include "xdcimp.h"

#define MAX_UNDO		7


typedef enum{
	COUNT_ALL = 0,
	COUNT_SEL = 1,
	COUNT_DEL = 2
}COUNT_HOW;

typedef struct _TEXTOR_CALC{
	bool_t done;

	void *from_obj,*to_obj;
	bool_t from_atom, to_atom;

	int client;

	int x, y;
	int width, height;
	xpoint_t from_pt, to_pt;

	bool_t paged;
	int page;
	int from_row, from_col, to_row, to_col;

	tchar_t* buf;
	int pos,max;

	int how;

	const drawing_interface* piv;
}TEXTOR_CALC;

static int _on_text_calc_paging(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char,int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col,  const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (page == ptt->page)
		{
			if (!ptt->done)
			{
				ptt->from_row = cur_row;
				ptt->from_col = cur_col;
				ptt->from_obj = object;
				ptt->from_atom = b_atom;

				ptt->done = 1;
			}

			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->to_obj = object;
			ptt->to_atom = b_atom;
		}
		break;
	case _SCANNER_STATE_END:
 		ptt->page = page;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_paging(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int* max_page, int* max_row, int* max_col, void** min_obj, bool_t* min_atom, void** max_obj, bool_t* max_atom)
{
	TEXTOR_CALC tt = { 0 };
	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);

	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_paging, (void*)&tt);

	if (max_page)
		*max_page = tt.page;
	if (max_row)
		*max_row = tt.to_row;
	if (max_col)
		*max_col = tt.to_col;
	if (min_obj)
		*min_obj = tt.from_obj;
	if (min_atom)
		*min_atom = tt.from_atom;
	if (max_obj)
		*max_obj = tt.to_obj;
	if (max_atom)
		*max_atom = tt.to_atom;
}

static int _on_text_calc_maxing(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (!ptt->done)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->from_obj = object;
			ptt->from_atom = b_atom;

			ptt->done = 1;
		}

		if (ptt->width < ptm->cur_x + ptm->cur_w - ptm->char_w)
			ptt->width = ptm->cur_x + ptm->cur_w - ptm->char_w;

		ptt->to_row = cur_row;
		ptt->to_col = cur_col;
		ptt->to_obj = object;
		ptt->to_atom = b_atom;
		ptt->max += cur_count;
		
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		ptt->height = ptm->cur_y + ptm->cur_h + ptm->line_h;

		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_maxing(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int* max_width, int* max_height, int* max_row, int* max_col, void** min_obj, bool_t* min_atom, void** max_obj, bool_t* max_atom)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_maxing, (void*)&tt);

	if (max_width)
		*max_width = tt.width - px;
	if (max_height)
		*max_height = tt.height - py;
	if (max_row)
		*max_row = tt.to_row;
	if (max_col)
		*max_col = tt.to_col;
	if (max_obj)
		*max_obj = tt.to_obj;
	if (max_atom)
		*max_atom = tt.to_atom;
	if (min_obj)
		*min_obj = tt.from_obj;
	if (min_atom)
		*min_atom = tt.from_atom;
}

static int _on_text_calc_line(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
		if (!ptt->done)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->from_obj = object;
			ptt->from_atom = b_atom;

			ptt->done = 1;
		}

		if (ptt->width < ptm->cur_x + ptm->cur_w - ptm->char_w)
			ptt->width = ptm->cur_x + ptm->cur_w - ptm->char_w;

		ptt->to_row = cur_row;
		ptt->to_col = cur_col;
		ptt->to_obj = object;
		ptt->to_atom = b_atom;
		ptt->max += cur_count;

		break;
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		ptt->height = ptm->cur_y + ptm->cur_h + ptm->line_h;

		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_line(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int* max_width, int* max_height, int* max_row, int* max_col, void** min_obj, bool_t* min_atom, void** max_obj, bool_t* max_atom)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_line, (void*)&tt);

	if (max_width)
		*max_width = tt.width - px;
	if (max_height)
		*max_height = tt.height - py;
	if (max_row)
		*max_row = tt.to_row;
	if (max_col)
		*max_col = tt.to_col;
	if (max_obj)
		*max_obj = tt.to_obj;
	if (max_atom)
		*max_atom = tt.to_atom;
	if (min_obj)
		*min_obj = tt.from_obj;
	if (min_atom)
		*min_atom = tt.from_atom;
}

static int _on_text_calc_object(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (ptt->done && ptt->to_obj != object)
		{
			return _SCANNER_OPERA_STOP;
		}
		if (!ptt->done && ptt->from_obj == object)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->done = 1;
			break;
		}
		if (ptt->done && ptt->to_obj == object)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			break;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_object(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, void* object, int* from_row, int* from_col, int* to_row, int* to_col)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.from_obj = tt.to_obj = object;
	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_object, (void*)&tt);

	if (from_row)
		*from_row = tt.from_row;
	if (from_col)
		*from_col = tt.from_col;
	if (to_row)
		*to_row = tt.to_row;
	if (to_col)
		*to_col = tt.to_col;
}

static int _on_text_calc_texting(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (ptt->done && ptt->to_obj != object)
		{
			return _SCANNER_OPERA_STOP;
		}
		if (ptt->done && ptt->to_obj == object && !b_sel)
		{
			return _SCANNER_OPERA_STOP;
		}
		if (!ptt->done && ptt->from_obj == object && b_sel)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->done = 1;
			break;
		}
		if (ptt->done && ptt->to_obj == object && b_sel)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			break;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_texting(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, void* object, int* from_row, int* from_col, int* to_row, int* to_col)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.from_obj = tt.to_obj = object;
	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_texting, (void*)&tt);

	if (from_row)
		*from_row = tt.from_row;
	if (from_col)
		*from_col = tt.from_col;
	if (to_row)
		*to_row = tt.to_row;
	if (to_col)
		*to_col = tt.to_col;
}

static int _on_text_calc_rect(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (ptt->done && ptt->to_obj != object)
		{
			return _SCANNER_OPERA_STOP;
		}
		if (!ptt->done && ptt->from_obj == object)
		{
			ptt->from_pt.x = ptm->cur_x;
			ptt->from_pt.y = ptm->cur_y;
			ptt->to_pt.x = ptm->cur_x + ptm->cur_w;
			ptt->to_pt.y = ptm->cur_y + ptm->cur_h;
			ptt->done = 1;
			break;
		}
		if (ptt->done && ptt->to_obj == object)
		{
			ptt->to_pt.x = ptm->cur_x + ptm->cur_w;
			ptt->to_pt.y = ptm->cur_y + ptm->cur_h;
			break;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_rect(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, void* object, xrect_t* pxr)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	tt.from_obj = tt.to_obj = object;
	tt.paged = ptd->paged;
	tt.page = ptd->page;

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_rect, (void*)&tt);

	if (pxr)
	{
		pxr->x = tt.from_pt.x;
		pxr->y = tt.from_pt.y;
		pxr->w = tt.to_pt.x - tt.from_pt.x;
		pxr->h = tt.to_pt.y - tt.from_pt.y;
	}
}

static int _on_text_calc_hint(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
		if (ptt->y >= ptm->cur_y && ptt->y < ptm->cur_y + ptm->cur_h + ptm->line_h && ptt->x < ptm->min_x + ptm->char_w)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->from_obj = object;
			ptt->from_atom = b_atom;
			ptt->done = 1;
			return _SCANNER_OPERA_STOP;
		}
		else if (ptt->y >= ptm->cur_y && ptt->y < ptm->cur_y + ptm->cur_h + ptm->line_h && ptt->x >= ptm->cur_x && ptt->x < ptm->cur_x + ptm->cur_w)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->from_obj = object;
			ptt->from_atom = b_atom;
			ptt->done = 1;
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (ptt->y >= ptm->cur_y && ptt->y < ptm->cur_y + ptm->cur_h + ptm->line_h && ptt->x >= ptm->cur_x)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->from_obj = object;
			ptt->from_atom = b_atom;
			ptt->done = 1;
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_NEWLINE:
		if (ptt->y >= ptm->cur_y && ptt->y < ptm->cur_y + ptm->cur_h + ptm->line_h)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col - 1;
			ptt->from_obj = object;
			ptt->from_atom = b_atom;
			ptt->done = 1;
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
		if (!ptt->done)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col - 1;
			ptt->from_obj = object;
			ptt->from_atom = b_atom;
		}
		return _SCANNER_OPERA_STOP;
	case _SCANNER_STATE_END:
		if (!ptt->done)
		{
			ptt->from_row = cur_row;
			ptt->from_col = cur_col;
			ptt->from_obj = object;
			ptt->from_atom = b_atom;
		}
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_hint(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int x, int y, int* row_at, int* col_at, void** pobj, bool_t* patom)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.x = x;
	tt.y = y;

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_hint, (void*)&tt);

	if (row_at)
		*row_at = tt.from_row;
	if (col_at)
		*col_at = tt.from_col;
	if (pobj)
		*pobj = tt.from_obj;
	if (patom)
		*patom = tt.from_atom;
}

static int _on_text_calc_point(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->from_pt.x = ptm->cur_x;
			ptt->from_pt.y = ptm->cur_y;
			ptt->to_pt.x = ptm->cur_x + ptm->cur_w;
			ptt->to_pt.y = ptm->cur_y + ptm->cur_h;
			ptt->done = 1;
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		if (!ptt->done)
		{
			ptt->from_pt.x = ptm->cur_x;
			ptt->from_pt.y = ptm->cur_y;
			ptt->to_pt.x = ptm->cur_x + ptm->cur_w;
			ptt->to_pt.y = ptm->cur_y + ptm->cur_h;
			ptt->done = 1;
		}
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_point(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int row, int col, xpoint_t* ppt1, xpoint_t* ppt2)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.from_row = row;
	tt.from_col = col;

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_point, (void*)&tt);

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

static int _on_text_calc_rowcol(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->to_obj = object;
			ptt->to_atom = b_atom;
			ptt->done = 1;
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_NEWLINE:
		if (ptt->from_row == cur_row && ptt->from_col < 0)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col - 1;
			ptt->to_obj = object;
			ptt->to_atom = b_atom;
			ptt->done = 1;
			return _SCANNER_OPERA_STOP;
		}
		else if (ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->from_row++;
			ptt->from_col = 0;
			break;
		}
		else if (ptt->from_row == cur_row && ptt->from_col > cur_col)
		{
			ptt->from_row++;
			ptt->from_col -= cur_col;
			break;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
		if (!ptt->done)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col - 1;
			ptt->to_obj = object;
			ptt->to_atom = b_atom;
			ptt->done = 1;
		}
		return _SCANNER_OPERA_STOP;
	case _SCANNER_STATE_END:
		if (!ptt->done)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->to_obj = object;
			ptt->to_atom = b_atom;
			ptt->done = 1;
		}
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_calc_rowcol(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int row, int col, int* prow, int* pcol, void** pobj, bool_t* patom)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	if (col < 0)
	{
		row--;
		col = -1;
	}

	if (row < 0)
	{
		if (prow)
			*prow = -1;
		if (pcol)
			*pcol = -1;
		return;
	}else if (row > ptd->max_row)
	{
		if (prow)
			*prow = -1;
		if (pcol)
			*pcol = -1;
		return;
	}

	tt.paged = ptd->paged;
	tt.page = ptd->page;
	tt.from_row = row;
	tt.from_col = col;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_rowcol, (void*)&tt);

	if (prow)
		*prow = tt.to_row;
	if (pcol)
		*pcol = tt.to_col;
	if (pobj)
		*pobj = tt.to_obj;
	if (patom)
		*patom = tt.to_atom;
}

static int _on_text_exec_select(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch(scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (inside_rowcol(cur_row, cur_col, ptt->from_row, ptt->from_col, ptt->to_row, ptt->to_col) && b_sel)
		{
			if (ptt->pos + cur_count > ptt->max)
				return _SCANNER_OPERA_STOP;

			if (ptt->buf)
			{
				xsncpy(ptt->buf + ptt->pos, cur_char, cur_count);
			}
			ptt->pos += cur_count;
		}
		else if (compare_rowcol(cur_row, cur_col, ptt->to_row, ptt->to_col) > 0)
		{
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

int _textor_exec_select(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int from_row, int from_col, int to_row, int to_col, tchar_t* buf, int max)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.buf = buf;
	tt.max = max;
	tt.pos = 0;

	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.to_row = to_row;
	tt.to_col = to_col;

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_exec_select, (void*)&tt);

	return tt.pos;
}

static int _on_text_calc_count(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (inside_rowcol(cur_row, cur_col, ptt->from_row, ptt->from_col, ptt->to_row, ptt->to_col))
		{
			if (ptt->how == COUNT_SEL && b_sel)
				ptt->max += cur_count;
			else if (ptt->how == COUNT_DEL && b_del)
				ptt->max += cur_count;
			else if (ptt->how == COUNT_ALL)
				ptt->max += cur_count;
		}
		else if (compare_rowcol(cur_row, cur_col, ptt->to_row, ptt->to_col) > 0)
		{
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

int _textor_calc_count(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int from_row, int from_col, int to_row, int to_col, int how)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.how = how;
	tt.max = 0;
	tt.pos = 0;

	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.to_row = to_row;
	tt.to_col = to_col;

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_count, (void*)&tt);

	return tt.max;
}

static int _on_text_exec_delete(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (cur_row == ptt->from_row && cur_col == ptt->from_col)
		{
			if (b_del)
			{
				if (ptt->pos < ptt->max)
				{
					ptt->pos += cur_count;
					return _SCANNER_OPERA_DEL;
				}
				else
				{
					ptt->to_row = cur_row;
					ptt->to_col = cur_col;
					ptt->to_obj = object;
					ptt->to_atom = b_atom;
					return _SCANNER_OPERA_STOP;
				}
			}
			else
			{
				if (ptt->pos >= ptt->max)
				{
					ptt->to_row = cur_row;
					ptt->to_col = cur_col;
					ptt->to_obj = object;
					ptt->to_atom = b_atom;
					return _SCANNER_OPERA_STOP;
				}
				else
				{
					ptt->from_col++;
					return _SCANNER_OPERA_NEXT;
				}
			}
		}
		else if (compare_rowcol(cur_row, cur_col, ptt->from_row, ptt->from_col) > 0)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->to_obj = object;
			ptt->to_atom = b_atom;
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_NEWLINE:
		if (cur_row == ptt->from_row && cur_col == ptt->from_col)
		{
			ptt->from_row++;
			ptt->from_col = 0;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
		ptt->to_row = cur_row;
		ptt->to_col = cur_col - 1;
		ptt->to_obj = object;
		ptt->to_atom = b_atom;
		return _SCANNER_OPERA_STOP;
	case _SCANNER_STATE_END:
		ptt->to_row = cur_row;
		ptt->to_col = cur_col;
		ptt->to_obj = object;
		ptt->to_atom = b_atom;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

bool_t _textor_exec_delete(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int from_row, int from_col, int to_row, int to_col, int* prow, int* pcol, void** pobj, bool_t* patom)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	tt.from_row = from_row;
	tt.from_col = from_col;
	tt.max = _textor_calc_count(ptd, px, py, pw, ph, paged, from_row, from_col, to_row, to_col, COUNT_DEL);
	tt.pos = 0;

	if (!tt.max)
		return 0;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_exec_delete, (void*)&tt);

	if (prow)
		*prow = tt.to_row;
	if (pcol)
		*pcol = tt.to_col;
	if (pobj)
		*pobj = tt.to_obj;
	if (patom)
		*patom = tt.to_atom;

	return (tt.pos)? 1 : 0;
}

static int _on_text_exec_insert(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;
	int n;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_END:
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (cur_row == ptt->from_row && cur_col == ptt->from_col)
		{
			if (b_ins)
			{
				if (ptt->pos < ptt->max && ptt->buf[ptt->pos] == _T('\r'))
				{
					ptt->pos++;
				}

				if (ptt->pos < ptt->max)
				{
					n = xschs(ptt->buf + ptt->pos);
					xsncpy(ret_char, ptt->buf + ptt->pos, n);
					ptt->pos += n;

					ptt->from_col++;

					ptt->to_row = cur_row;
					ptt->to_col = cur_col;
					ptt->to_obj = object;
					ptt->to_atom = b_atom;
					return _SCANNER_OPERA_INS;
				}
				else
				{
					ptt->to_row = cur_row;
					ptt->to_col = cur_col;
					ptt->to_obj = object;
					ptt->to_atom = b_atom;
					return _SCANNER_OPERA_STOP;
				}
			}
			else
			{
				ptt->from_col++;
				return _SCANNER_OPERA_NEXT;
			}
		}
		else if (compare_rowcol(cur_row, cur_col, ptt->from_row, ptt->from_col) > 0)
		{
			ptt->to_row = cur_row;
			ptt->to_col = cur_col;
			ptt->to_obj = object;
			ptt->to_atom = b_atom;
			return _SCANNER_OPERA_STOP;
		}
		break;
	case _SCANNER_STATE_NEWLINE:
		if (cur_row == ptt->from_row && cur_col <= ptt->from_col)
		{
			ptt->from_row = cur_row + 1;
			ptt->from_col -= cur_col;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
		ptt->page++;
		ptt->from_row = 0;
		ptt->from_col = 0;
		break;
	}

	return _SCANNER_OPERA_NEXT;
}

bool_t _textor_exec_insert(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, int* ppage, int* prow, int* pcol, void** pobj, bool_t* patom, tchar_t* str, int len)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	if (len < 0)
		len = xslen(str);

	if (!len)
		return 0;

	tt.paged = ptd->paged;
	tt.page = *ppage;
	tt.from_row = *prow;
	tt.from_col = *pcol;
	tt.buf = str;
	tt.max = len;
	tt.pos = 0;

	get_visual_interface(ptd->cdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_exec_insert, (void*)&tt);

	*ppage = tt.page;
	*prow = tt.to_row;
	*pcol = tt.to_col;
	if (pobj)
		*pobj = tt.to_obj;
	if (patom)
		*patom = tt.to_atom;

	return (tt.pos) ? 1 : 0;
}

static int _on_text_calc_focus(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;
	xrect_t xr_reg, xr_dot;
	xcolor_t xc;

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
	case _SCANNER_STATE_LINEBREAK:
	case _SCANNER_STATE_PAGEBREAK:
		if (compare_rowcol(ptt->from_row, ptt->from_col, ptt->to_row, ptt->to_col) > 0)
		{
			return _SCANNER_OPERA_STOP;
		}

		if (!ptt->done && ptt->from_row == cur_row && ptt->from_col == cur_col)
		{
			ptt->from_pt.x = ptm->cur_x;
			ptt->from_pt.y = ptm->cur_y;
			ptt->done = 1;

			xr_dot.x = ptt->from_pt.x;
			xr_dot.y = ptt->from_pt.y - DEF_FOCUS_SPAN;
			xr_dot.w = DEF_FOCUS_SPAN;
			xr_dot.h = DEF_FOCUS_SPAN;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			(*(ptt->piv->pf_alphablend_rect))(ptt->piv->ctx, &xc, &xr_dot, ALPHA_SOLID);
		}

		if (ptt->done && ptt->to_row == cur_row && ptt->to_col == cur_col)
		{
			ptt->to_pt.x = ptm->cur_x + ptm->cur_w;
			ptt->to_pt.y = ptm->cur_y + ptm->cur_h;

			xr_reg.x = ptt->from_pt.x;
			xr_reg.y = ptt->from_pt.y;
			xr_reg.w = ptt->to_pt.x - ptt->from_pt.x;
			xr_reg.h = ptt->to_pt.y - ptt->from_pt.y;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			(*(ptt->piv->pf_alphablend_rect))(ptt->piv->ctx, &xc, &xr_reg, ALPHA_SOFT);
			
			xr_dot.x = ptt->to_pt.x - DEF_FOCUS_SPAN;
			xr_dot.y = ptt->to_pt.y;
			xr_dot.w = DEF_FOCUS_SPAN;
			xr_dot.h = DEF_FOCUS_SPAN;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			(*(ptt->piv->pf_alphablend_rect))(ptt->piv->ctx, &xc, &xr_dot, ALPHA_SOLID);
			return _SCANNER_OPERA_STOP;
		}

		break;
	case _SCANNER_STATE_NEWLINE:
		if (ptt->done)
		{
			ptt->to_pt.x = ptm->cur_x + ptm->cur_w;
			ptt->to_pt.y = ptm->cur_y + ptm->cur_h;

			xr_reg.x = ptt->from_pt.x;
			xr_reg.y = ptt->from_pt.y;
			xr_reg.w = ptt->to_pt.x - ptt->from_pt.x;
			xr_reg.h = ptt->to_pt.y - ptt->from_pt.y;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			(*(ptt->piv->pf_alphablend_rect))(ptt->piv->ctx, &xc, &xr_reg, ALPHA_SOFT);

			ptt->from_pt.x = ptm->min_x + ptm->char_w;
			ptt->from_pt.y = ptm->cur_y + ptm->cur_h + ptm->line_h;
		}
		break;
	case _SCANNER_STATE_NEWPAGE:
		if (ptt->done)
		{
			ptt->to_pt.x = ptm->cur_x + ptm->cur_w;
			ptt->to_pt.y = ptm->cur_y + ptm->cur_h;

			xr_reg.x = ptt->from_pt.x;
			xr_reg.y = ptt->from_pt.y;
			xr_reg.w = ptt->to_pt.x - ptt->from_pt.x;
			xr_reg.h = ptt->to_pt.y - ptt->from_pt.y;
			parse_xcolor(&xc, DEF_ALPHA_COLOR);
			(*(ptt->piv->pf_alphablend_rect))(ptt->piv->ctx, &xc, &xr_reg, ALPHA_SOFT);

			ptt->from_pt.x = ptm->min_x + ptm->char_w;
			ptt->from_pt.y = ptm->cur_y + ptm->cur_h + ptm->line_h;
		}
		return _SCANNER_OPERA_STOP;
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_draw_focus_raw(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, visual_t rdc)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	tt.from_row = ptd->sel_row;
	tt.from_col = ptd->sel_col;
	tt.to_row = ptd->cur_row;
	tt.to_col = ptd->cur_col;

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(rdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_focus, (void*)&tt);
}

static int _on_text_calc_paint(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const word_place_t* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TEXTOR_CALC* ptt = (TEXTOR_CALC*)pp;

	xpoint_t pt1, pt2;
	xrect_t xr;

	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };

	switch (scan)
	{
	case _SCANNER_STATE_CATOR:
		if (page < ptt->page)
			return _SCANNER_OPERA_PAGED;
		else if (page > ptt->page)
			return _SCANNER_OPERA_STOP;

		break;
	case _SCANNER_STATE_WORDS:
		if (ptm->cur_y > 0 && ptm->cur_y < ptt->client)
		{
			pt1.x = ptm->cur_x;
			pt1.y = ptm->cur_y;

			if (cur_char && *cur_char == _T('\0'))
				(*(ptt->piv->pf_text_out))(ptt->piv->ctx, pxf, &pt1, _T("."), 1);
			else
				(*(ptt->piv->pf_text_out))(ptt->piv->ctx, pxf, &pt1, cur_char, cur_count);
		}
		break;
	case _SCANNER_STATE_LINEBREAK:
		default_xpen(&xp);
		xscpy(xp.color, pxf->color);
		lighten_xpen(&xp, DEF_HARD_LIGHTEN);

		if (cur_char && *cur_char == _T('\f'))
		{
			xr.x = ptm->cur_x + ptm->cur_w - 3;
			xr.y = ptm->cur_y + ptm->cur_h - 3;
			xr.w = 3;
			xr.h = 3;
			(*(ptt->piv->pf_draw_rect))(ptt->piv->ctx, &xp, NULL, &xr);
		}
		else
		{
			pt1.x = ptm->cur_x + ptm->cur_w;
			pt1.y = ptm->cur_y + ptm->cur_h;
			pt2.x = pt1.x - 3;
			pt2.y = pt1.y;
			(*(ptt->piv->pf_draw_line))(ptt->piv->ctx, &xp, &pt1, &pt2);

			pt2.x = pt1.x;
			pt2.y = pt1.y - 4;
			(*(ptt->piv->pf_draw_line))(ptt->piv->ctx, &xp, &pt1, &pt2);
		}
		break;
	case _SCANNER_STATE_PAGEBREAK:
		default_xpen(&xp);
		xscpy(xp.color, pxf->color);
		lighten_xpen(&xp, DEF_HARD_LIGHTEN);

		default_xbrush(&xb);
		xscpy(xb.color, pxf->color);

		xr.x = ptm->cur_x + ptm->cur_w - 3;
		xr.y = ptm->cur_y + ptm->cur_h - 3;
		xr.w = 3;
		xr.h = 3;
		(*(ptt->piv->pf_draw_rect))(ptt->piv->ctx, &xp, &xb, &xr);
		break;
	case _SCANNER_STATE_NEWPAGE:
		return _SCANNER_OPERA_STOP;
	case _SCANNER_STATE_END:
		if (!b_ins)
		{
			default_xbrush(&xb);
			xscpy(xb.color, pxf->color);
			lighten_xbrush(&xb, DEF_HARD_LIGHTEN);

			xr.x = ptm->cur_x + ptm->cur_w - 3;
			xr.y = ptm->cur_y + ptm->cur_h - 3;
			xr.w = 3;
			xr.h = 3;
			(*(ptt->piv->pf_draw_rect))(ptt->piv->ctx, NULL, &xb, &xr);
		}
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void _textor_draw_text(textor_context* ptd, int px, int py, int pw, int ph, bool_t paged, visual_t rdc)
{
	TEXTOR_CALC tt = { 0 };

	drawing_interface iv = { 0 };
	measure_interface it = { 0 };
	const xfont_t* pxf;
	const xface_t* pxa;
	xrect_t xr;

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	widget_get_client_rect(ptd->widget, &xr);
	tt.client = xr.h;
	tt.from_row = ptd->sel_row;
	tt.from_col = ptd->sel_col;
	tt.to_row = ptd->cur_row;
	tt.to_col = ptd->cur_col;

	tt.paged = ptd->paged;
	tt.page = ptd->page;

	get_visual_interface(rdc, &iv);
	tt.piv = &iv;

	(*(tt.piv->pf_get_measure))(tt.piv->ctx, &it);

	(*ptd->pf_scan_text)(ptd->data, &it, pxf, pxa, px, py, pw, ph, paged, _on_text_calc_paint, (void*)&tt);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void _textor_reset_caret(textor_context* ptd)
{
	xpoint_t pt1, pt2;

	_textor_calc_point(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	widget_show_caret(ptd->widget, pt1.x, pt1.y, 1);
}

void _textor_reset_page(textor_context* ptd, bool_t repage)
{
	xrect_t xr;
	xsize_t xs;
	int pw, ph;
	int rows, cols;

	int line_feed;
	float line_height;

	const xfont_t* pxf;
	const xface_t* pxa;

	drawing_interface ifv = { 0 };

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);

	line_height = xstof(pxa->line_height);
	if (!line_height)
		line_height = 1.0;

	get_visual_interface(ptd->cdc, &ifv);

	(*ifv.pf_text_metric)(ifv.ctx, pxf, &xs);

	line_feed = (int)(xs.h * line_height);

	ptd->paged = (*ptd->pf_get_paging)(ptd->widget, &xs);

	if (ptd->paged)
	{
		if (repage)
		{
			_textor_calc_paging(ptd, 0, 0, xs.w, xs.h, ptd->paged, &ptd->pages, &rows, &cols, NULL, NULL, NULL, NULL);
			ptd->max_row = rows;
			ptd->max_col = cols;
		}

		pw = xs.w;
		ph = xs.h;
	}
	else
	{
		ptd->pages = 1;
		_textor_calc_maxing(ptd, 0, 0, xs.w, xs.h,ptd->paged, &pw, &ph, &rows, &cols, NULL, NULL, NULL, NULL);

		if (repage)
		{
			ptd->max_row = rows;
			ptd->max_col = cols;
		}

		if (pw < xs.w)
			pw = xs.w;
		if (ph < xs.h)
			ph = xs.h;
	}

	widget_get_client_rect(ptd->widget, &xr);

	widget_reset_paging(ptd->widget, xr.w, xr.h, pw, ph, line_feed, line_feed);

	widget_reset_scroll(ptd->widget, 1);

	widget_reset_scroll(ptd->widget, 0);
}

void _textor_ensure_visible(textor_context* ptd, int row, int col)
{
	xpoint_t pt1, pt2;
	xrect_t xr;
	const xface_t* pxa;

	pxa = widget_get_xface_ptr(ptd->widget);

	_textor_calc_point(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, row, col, &pt1, &pt2);

	xr.x = pt1.x - 2;
	xr.y = pt1.y;
	xr.w = pt2.x - pt1.x + 4;
	xr.h = pt2.y - pt1.y;

	if (is_null(pxa->text_wrap))
		widget_ensure_visible(ptd->widget, &xr, 0);
	else
		widget_ensure_visible(ptd->widget, &xr, 1);

	widget_get_view_rect(ptd->widget, &ptd->vb);
}

void _textor_done(textor_context* ptd)
{
	TEXTORUNDO *pnew,*pnxt;
	int count = 0;

	if (!ptd->max_undo)
		return;

	pnew = (TEXTORUNDO*)xmem_alloc(sizeof(TEXTORUNDO));

	pnew->b_select = ptd->b_select;
	pnew->sel_row = ptd->sel_row;
	pnew->sel_col = ptd->sel_col;
	pnew->cur_row = ptd->cur_row;
	pnew->cur_col = ptd->cur_col;
	pnew->page = ptd->page;

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

void _textor_discard(textor_context* ptd)
{
	TEXTORUNDO *prev;

	if (ptd->ptu)
	{
		prev = ptd->ptu;
		ptd->ptu = prev->next;
		
		xsfree(prev->text);
		xmem_free(prev);
	}
}

void _textor_clean(textor_context* ptd)
{
	TEXTORUNDO *next;

	while (ptd->ptu)
	{
		next = ptd->ptu->next;

		xsfree(ptd->ptu->text);
		xmem_free(ptd->ptu);

		ptd->ptu = next;
	}
}


/********************************************************************************************/

void hand_textor_set_focus(textor_context* ptd, res_win_t wt)
{
	xsize_t xs;
	float pm, lh;
	const xfont_t* pxf;
	const xface_t* pxa;

	XDL_ASSERT(ptd && ptd->widget);

	pxf = widget_get_xfont_ptr(ptd->widget);
	pxa = widget_get_xface_ptr(ptd->widget);
	lh = (pxa) ? xstof(pxa->line_height) : 1.0f;

	font_metric_by_pt(xstof(pxf->size), &pm, NULL);
	xs.fw = pm;
	xs.fh = pm * lh;
	widget_size_to_pt(ptd->widget, &xs);

	widget_create_caret(ptd->widget, 2, xs.h);
}

void hand_textor_kill_focus(textor_context* ptd, res_win_t wt)
{
	XDL_ASSERT(ptd && ptd->widget);

	widget_destroy_caret(ptd->widget);
}

int hand_textor_word(textor_context* ptd, tchar_t* pch)
{
	int row,col,page = 0;
	void* object = NULL;
	bool_t atom = 0;
	tchar_t token[CHS_LEN + 1] = { 0 };
	xface_t xa = { 0 };

	XDL_ASSERT(ptd && ptd->widget);

	widget_get_xface(ptd->widget, &xa);

	if (*pch == KEY_ENTER)
	{
		if (is_null(xa.text_wrap))
			return _TEXTOR_PRESS_IGNORE;

		xsncpy(token, _T("\n"), 1);
	}
	else if (*pch == KEY_TAB)
	{
		if (is_null(xa.text_wrap))
			return _TEXTOR_PRESS_IGNORE;

		xsncpy(token, _T("\t"), 1);
	}
	else
	{
		xsncpy(token, pch, CHS_LEN);
	}

	_textor_done(ptd);

	if (ptd->b_select)
	{
		_textor_exec_delete(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, &ptd->sel_row, &ptd->sel_col, NULL, NULL);

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;
		ptd->b_select = 0;
	}

	page = ptd->page;
	row = ptd->cur_row;
	col = ptd->cur_col;
	if (!_textor_exec_insert(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, &page, &row, &col, &object, &atom, token, xslen(token)))
		return _TEXTOR_PRESS_IGNORE;

	ptd->page = page;
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = object;
	ptd->atom = atom;

	_textor_reset_page(ptd, 1);
	widget_erase(ptd->widget, NULL);

	_textor_ensure_visible(ptd, ptd->cur_row,ptd->cur_col);
	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_ACCEPT;
}

void hand_textor_lbutton_down(textor_context* ptd, const xpoint_t* pxp)
{
	xpoint_t pt1, pt2;
	int row = 0;
	int col = 0;
	void* object = NULL;
	bool_t atom = 0;

	XDL_ASSERT(ptd && ptd->widget);

	if (!widget_is_focus(ptd->widget))
		widget_set_focus(ptd->widget);
			
	widget_set_capture(ptd->widget, 1);
	
	if (ptd->b_select)
	{
		_textor_calc_point(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, &pt1, &pt2);
		if (pt_inside(pxp->x, pxp->y, pt1.x - DEF_FOCUS_SPAN, pt1.y - DEF_FOCUS_SPAN, pt1.x + DEF_FOCUS_SPAN, pt2.y + DEF_FOCUS_SPAN))
		{
			ptd->n_drag = _TEXTOR_DRAG_FIRST;
			return;
		}

		_textor_calc_point(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col, &pt1, &pt2);
		if (pt_inside(pxp->x, pxp->y, pt2.x - DEF_FOCUS_SPAN, pt1.y - DEF_FOCUS_SPAN, pt2.x + DEF_FOCUS_SPAN, pt2.y + DEF_FOCUS_SPAN))
		{
			ptd->n_drag = _TEXTOR_DRAG_LAST;
			return;
		}

		ptd->b_select = 0;
		widget_erase(ptd->widget, NULL);
	}

	_textor_calc_hint(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, pxp->x, pxp->y, &row, &col, &object, &atom);

	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = object;
	ptd->atom = atom;

	_textor_reset_caret(ptd);
}

void hand_textor_lbutton_up(textor_context* ptd, const xpoint_t* pxp)
{
	XDL_ASSERT(ptd && ptd->widget);

	widget_set_capture(ptd->widget, 0);

	if (ptd->n_drag)
	{
		ptd->n_drag = 0;
		widget_erase(ptd->widget, NULL);
		return;
	}
}

void hand_textor_mousemove(textor_context* ptd, dword_t mk, const xpoint_t* ppt)
{
	int row, col;
	void* object = NULL;
	bool_t atom = 0;

	XDL_ASSERT(ptd && ptd->widget);

	if (ptd->n_drag == _TEXTOR_DRAG_FIRST)
	{
		_textor_calc_hint(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ppt->x, ppt->y, &row, &col, NULL, NULL);
		if (row >= ptd->cur_row && col >= ptd->cur_col)
			return;

		ptd->sel_row = row;
		ptd->sel_col = col;

		widget_erase(ptd->widget, NULL);

		_textor_ensure_visible(ptd, ptd->sel_row, ptd->sel_col);
	}
	else if (ptd->n_drag == _TEXTOR_DRAG_LAST)
	{
		_textor_calc_hint(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ppt->x, ppt->y, &row, &col, &object, &atom);
		if (row <= ptd->sel_row && col <= ptd->sel_col)
			return;

		ptd->cur_row = row;
		ptd->cur_col = col;
		ptd->object = object;
		ptd->atom = atom;

		widget_erase(ptd->widget, NULL);

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	}
	else if (mk & MS_WITH_LBUTTON)
	{
		if (ptd->b_select)
		{
			_textor_calc_hint(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ppt->x, ppt->y, &row, &col, NULL, NULL);

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

void hand_textor_size(textor_context* ptd, int code, const xsize_t* prs)
{
	XDL_ASSERT(ptd && ptd->widget);

	_textor_reset_page(ptd, 0);

	widget_get_view_rect(ptd->widget, &ptd->vb);

	widget_erase(ptd->widget, NULL);
}

bool_t hand_textor_scroll(textor_context* ptd, bool_t bHorz, int nLine)
{
	XDL_ASSERT(ptd && ptd->widget);

	if (!widget_hand_scroll(ptd->widget, bHorz, nLine))
		return 0;

	widget_get_view_rect(ptd->widget, &ptd->vb);

	_textor_reset_caret(ptd);

	return 1;
}

void hand_textor_paint(textor_context* ptd, visual_t dc, const xrect_t* pxr)
{
	xrect_t xr;
	visual_t rdc;
	canvas_t canv;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	
	const drawing_interface* pif = NULL;
	drawing_interface ifv = { 0 };

	XDL_ASSERT(ptd && ptd->widget);

	widget_get_xbrush(ptd->widget, &xb);
	widget_get_xpen(ptd->widget, &xp);

	canv = widget_get_canvas(ptd->widget);

	pif = widget_get_canvas_interface(ptd->widget);

	widget_get_client_rect(ptd->widget, &xr);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	get_visual_interface(rdc, &ifv);

	(*ifv.pf_draw_rect)(ifv.ctx, NULL, &xb, &xr);

	if (widget_can_paging(ptd->widget))
	{
		parse_xcolor(&xc, xp.color);
		lighten_xcolor(&xc, DEF_HARD_DARKEN);

		draw_corner(pif, &xc, (const xrect_t*)&(pif->rect));
	}

	_textor_draw_text(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, rdc);

	if (ptd->b_select)
	{
		_textor_draw_focus_raw(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, rdc);
	}

	end_canvas_paint(canv, dc, pxr);
}

void hand_textor_selectall(textor_context* ptd)
{
	int row, col;
	void* obj;
	bool_t atom;
	xsize_t xs;

	XDL_ASSERT(ptd && ptd->widget);

	(*ptd->pf_get_paging)(ptd->widget, &xs);

	_textor_calc_maxing(ptd, 0, 0, xs.w, xs.h, ptd->paged, NULL, NULL, &row, &col, NULL, NULL, &obj, &atom);

	if (!row && !col && !obj)
	{
		return;
	}

	ptd->sel_row = 0;
	ptd->sel_col = 0;
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = obj;
	ptd->atom = atom;

	ptd->b_select = 1;

	_textor_reset_caret(ptd);

	widget_erase(ptd->widget, NULL);
}

void hand_textor_selectline(textor_context* ptd)
{
	int row, col;
	void* obj;
	bool_t atom;
	xsize_t xs;

	XDL_ASSERT(ptd && ptd->widget);

	(*ptd->pf_get_paging)(ptd->widget, &xs);

	_textor_calc_line(ptd, 0, 0, xs.w, xs.h, ptd->paged, NULL, NULL, &row, &col, NULL, NULL, &obj, &atom);

	if (!row && !col && !obj)
	{
		return;
	}

	ptd->sel_row = 0;
	ptd->sel_col = 0;
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = obj;
	ptd->atom = atom;

	ptd->b_select = 1;

	_textor_reset_caret(ptd);

	widget_erase(ptd->widget, NULL);
}

void hand_textor_selectcur(textor_context* ptd)
{
	XDL_ASSERT(ptd && ptd->widget);

	ptd->b_select = 1;

	widget_erase(ptd->widget, NULL);
}

void hand_textor_selectobj(textor_context* ptd)
{
	int from_row, from_col, to_row, to_col;

	XDL_ASSERT(ptd && ptd->widget);

	if (!ptd->object)
		return;

	_textor_calc_texting(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->object, &from_row, &from_col, &to_row, &to_col);

	if (from_row == to_row && from_col == to_col)
	{
		return;
	}

	ptd->sel_row = from_row;
	ptd->sel_col = from_col;
	ptd->cur_row = to_row;
	ptd->cur_col = to_col;

	ptd->b_select = 1;

	_textor_reset_caret(ptd);

	widget_erase(ptd->widget, NULL);
}

void hand_textor_findobj(textor_context* ptd, void* obj)
{
	int from_row, from_col, to_row, to_col;

	XDL_ASSERT(ptd && ptd->widget);

	_textor_calc_object(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, obj, &from_row, &from_col, &to_row, &to_col);

	ptd->sel_row = from_row;
	ptd->sel_col = from_col;
	ptd->cur_row = from_row;
	ptd->cur_col = from_col;

	ptd->object = obj;
	ptd->b_select = 0;

	_textor_reset_caret(ptd);

	widget_erase(ptd->widget, NULL);
}

void hand_textor_redraw(textor_context* ptd)
{
	int row, col;

	XDL_ASSERT(ptd && ptd->widget);

	_textor_reset_page(ptd, 1);

	_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col, &row, &col, &ptd->object, &ptd->atom);

	ptd->cur_row = ptd->sel_row = row;
	ptd->cur_col = ptd->sel_col = col;
	ptd->b_select = 0;

	widget_erase(ptd->widget, NULL);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);
}

int hand_textor_back(textor_context* ptd)
{
	int sel_row, sel_col, cur_row, cur_col;
	void* object = NULL;
	bool_t atom = 0;

	XDL_ASSERT(ptd && ptd->widget);

	if (!ptd->b_select)
	{
		if (ptd->paged && ptd->page > 1 && !ptd->cur_row && !ptd->cur_col)
		{
			hand_textor_move_prev_page(ptd);
			
			cur_row = sel_row = ptd->max_row;
			cur_col = sel_col = ptd->max_col;
		}
		else
		{
			_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col - 1, &sel_row, &sel_col, NULL, NULL);
			if (sel_col < 0)
				return _TEXTOR_PRESS_IGNORE;

			cur_row = sel_row;
			cur_col = sel_col;
		}
	}
	else
	{
		sel_row = ptd->sel_row;
		sel_col = ptd->sel_col;
		cur_row = ptd->cur_row;
		cur_col = ptd->cur_col;
	}

	_textor_done(ptd);

	if (!_textor_exec_delete(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, sel_row, sel_col, cur_row, cur_col, &sel_row, &sel_col, &object, &atom))
	{
		_textor_discard(ptd);
		return _TEXTOR_PRESS_IGNORE;
	}

	ptd->cur_row = ptd->sel_row = sel_row;
	ptd->cur_col = ptd->sel_col = sel_col;
	ptd->object = object;
	ptd->atom = atom;

	ptd->b_select = 0;

	_textor_reset_page(ptd, 1);
	widget_erase(ptd->widget, NULL);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_ACCEPT;
}

int hand_textor_delete(textor_context* ptd)
{
	int sel_row, sel_col, cur_row, cur_col;
	void* object = NULL;
	bool_t atom = 0;

	XDL_ASSERT(ptd && ptd->widget);

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

	if (!_textor_exec_delete(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, sel_row, sel_col, cur_row, cur_col, &sel_row, &sel_col, &object, &atom))
	{
		_textor_discard(ptd);
		return _TEXTOR_PRESS_IGNORE;
	}

	ptd->cur_row = ptd->sel_row = sel_row;
	ptd->cur_col = ptd->sel_col = sel_col;
	ptd->object = object;
	ptd->atom = atom;

	ptd->b_select = 0;

	_textor_reset_page(ptd, 1);

	widget_erase(ptd->widget, NULL);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_ACCEPT;
}

int hand_textor_escape(textor_context* ptd)
{
	XDL_ASSERT(ptd && ptd->widget);

	if (ptd->b_select)
	{
		ptd->b_select = 0;
		widget_erase(ptd->widget, NULL);

		return _TEXTOR_PRESS_HANDLE;
	}

	return _TEXTOR_PRESS_IGNORE;
}

int hand_textor_left(textor_context* ptd)
{
	int row, col;
	void* object = NULL;
	bool_t atom = 0;
	bool_t redraw = 0;

	XDL_ASSERT(ptd && ptd->widget);

	if (ptd->b_select && !widget_key_state(ptd->widget, KEY_SHIFT))
	{
		_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, &row, &col, &object, &atom);

		ptd->cur_row = ptd->sel_row = row;
		ptd->cur_col = ptd->sel_col = col;
		ptd->object = object;
		ptd->atom = atom;
		ptd->b_select = 0;

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

		widget_erase(ptd->widget, NULL);

		_textor_reset_caret(ptd);

		return _TEXTOR_PRESS_HANDLE;
	}

	if (ptd->b_select != widget_key_state(ptd->widget, KEY_SHIFT))
	{
		ptd->b_select = widget_key_state(ptd->widget, KEY_SHIFT);
		redraw = 1;
	}

	if (ptd->b_select && !ptd->cur_row && !ptd->cur_col)
		return _TEXTOR_PRESS_IGNORE;

	if (!ptd->b_select && !ptd->cur_row && !ptd->cur_col && ptd->paged)
	{
		hand_textor_move_prev_page(ptd);
		return _TEXTOR_PRESS_HANDLE;
	}

	_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col - 1, &row, &col, &object, &atom);

	if (col < 0)
		return _TEXTOR_PRESS_IGNORE;

	if (row == ptd->cur_row && col == ptd->cur_col)
		return _TEXTOR_PRESS_IGNORE;

	if (ptd->b_select && compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
		return _TEXTOR_PRESS_IGNORE;

	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = object;
	ptd->atom = atom;

	if (!ptd->b_select)
	{
		ptd->sel_row = ptd->cur_row;
		ptd->sel_col = ptd->cur_col;
	}
	else
	{
		redraw = 1;
	}

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

	if (redraw)
		widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_HANDLE;
}

int hand_textor_right(textor_context* ptd)
{
	int row, col;
	void* object = NULL;
	bool_t atom = 0;
	bool_t redraw = 0;

	XDL_ASSERT(ptd && ptd->widget);

	if (ptd->b_select && !widget_key_state(ptd->widget, KEY_SHIFT))
	{
		_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col + 1, &row, &col, &object, &atom);

		ptd->cur_row = ptd->sel_row = row;
		ptd->cur_col = ptd->sel_col = col;
		ptd->object = object;
		ptd->atom = atom;
		ptd->b_select = 0;

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

		widget_erase(ptd->widget, NULL);

		_textor_reset_caret(ptd);

		return _TEXTOR_PRESS_HANDLE;
	}

	if (!ptd->b_select && widget_key_state(ptd->widget, KEY_SHIFT))
	{
		ptd->b_select = 1;

		widget_erase(ptd->widget, NULL);

		return _TEXTOR_PRESS_HANDLE;
	}

	if (ptd->b_select != widget_key_state(ptd->widget, KEY_SHIFT))
	{
		ptd->b_select = widget_key_state(ptd->widget, KEY_SHIFT);
		redraw = 1;
	}

	if (ptd->b_select && ptd->cur_row == ptd->max_row && ptd->cur_col >= ptd->max_col)
		return _TEXTOR_PRESS_IGNORE;

	if (!ptd->b_select && ptd->cur_row == ptd->max_row && ptd->cur_col >= ptd->max_col && ptd->paged)
	{
		hand_textor_move_next_page(ptd);
		return _TEXTOR_PRESS_HANDLE;
	}

	_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col + 1, &row, &col, &object, &atom);

	if (col < 0)
		return _TEXTOR_PRESS_IGNORE;

	if (row == ptd->cur_row && col == ptd->cur_col)
		return _TEXTOR_PRESS_IGNORE;

	if (ptd->b_select && compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
		return _TEXTOR_PRESS_IGNORE;

	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = object;
	ptd->atom = atom;

	if (!ptd->b_select)
	{
		ptd->sel_row = ptd->cur_row;
		ptd->sel_col = ptd->cur_col;
	}
	else
	{
		redraw = 1;
	}

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

	if (redraw)
		widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_HANDLE;
}

int hand_textor_up(textor_context* ptd)
{
	int row, col;
	void* object = NULL;
	bool_t atom = 0;
	bool_t redraw = 0;

	XDL_ASSERT(ptd && ptd->widget);

	if (ptd->b_select && !widget_key_state(ptd->widget, KEY_SHIFT))
	{
		_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, &row, &col, &object, &atom);

		ptd->cur_row = ptd->sel_row = row;
		ptd->cur_col = ptd->sel_col = col;
		ptd->object = object;
		ptd->atom = atom;
		ptd->b_select = 0;

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

		widget_erase(ptd->widget, NULL);

		_textor_reset_caret(ptd);

		return _TEXTOR_PRESS_HANDLE;
	}

	if (ptd->b_select != widget_key_state(ptd->widget, KEY_SHIFT))
	{
		ptd->b_select = widget_key_state(ptd->widget, KEY_SHIFT);
		redraw = 1;
	}

	if (ptd->b_select && !ptd->cur_row)
		return _TEXTOR_PRESS_IGNORE;

	if (!ptd->b_select && !ptd->cur_row)
	{
		hand_textor_move_prev_page(ptd);
		return _TEXTOR_PRESS_HANDLE;
	}

	_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, -1, &row, &col, &object, &atom);

	if (col < 0)
		return _TEXTOR_PRESS_IGNORE;

	if (row == ptd->cur_row && col == ptd->cur_col)
		return _TEXTOR_PRESS_IGNORE;

	if (ptd->b_select && compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
		return _TEXTOR_PRESS_IGNORE;

	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = object;
	ptd->atom = atom;

	if (!ptd->b_select)
	{
		ptd->sel_row = ptd->cur_row;
		ptd->sel_col = ptd->cur_col;
	}
	else
	{
		redraw = 1;
	}

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

	if (redraw)
		widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_HANDLE;
}

int hand_textor_down(textor_context* ptd)
{
	int row, col;
	void* object = NULL;
	bool_t atom = 0;
	bool_t redraw = 0;

	XDL_ASSERT(ptd && ptd->widget);

	if (ptd->b_select && !widget_key_state(ptd->widget, KEY_SHIFT))
	{
		_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col + 1, &row, &col, &object, &atom);

		ptd->cur_row = ptd->sel_row = row;
		ptd->cur_col = ptd->sel_col = col;
		ptd->object = object;
		ptd->atom = atom;
		ptd->b_select = 0;

		_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

		widget_erase(ptd->widget, NULL);

		_textor_reset_caret(ptd);

		return _TEXTOR_PRESS_HANDLE;
	}

	if (!ptd->b_select && widget_key_state(ptd->widget, KEY_SHIFT))
	{
		ptd->b_select = 1;

		widget_erase(ptd->widget, NULL);

		return _TEXTOR_PRESS_HANDLE;
	}

	if (ptd->b_select != widget_key_state(ptd->widget, KEY_SHIFT))
	{
		ptd->b_select = widget_key_state(ptd->widget, KEY_SHIFT);
		redraw = 1;
	}

	if (ptd->b_select && ptd->cur_row == ptd->max_row)
		return _TEXTOR_PRESS_IGNORE;

	if (!ptd->b_select && ptd->cur_row == ptd->max_row)
	{
		hand_textor_move_next_page(ptd);
		return _TEXTOR_PRESS_HANDLE;
	}

	_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row + 1, 0, &row, &col, &object, &atom);

	if (col < 0)
		return _TEXTOR_PRESS_IGNORE;

	if (row == ptd->cur_row && col == ptd->cur_col)
		return _TEXTOR_PRESS_IGNORE;

	if (ptd->b_select && compare_rowcol(row, col, ptd->sel_row, ptd->sel_col) < 0)
		return _TEXTOR_PRESS_IGNORE;

	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->object = object;
	ptd->atom = atom;

	if (!ptd->b_select)
	{
		ptd->sel_row = ptd->cur_row;
		ptd->sel_col = ptd->cur_col;
	}
	else
	{
		redraw = 1;
	}

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

	if (redraw)
		widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_HANDLE;
}

int hand_textor_home(textor_context* ptd)
{
	XDL_ASSERT(ptd && ptd->widget);

	hand_textor_move_first_page(ptd);

	ptd->cur_row = ptd->sel_row = 0;
	ptd->cur_col = ptd->sel_col = 0;

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_HANDLE;
}

int hand_textor_end(textor_context* ptd)
{
	xsize_t xs;

	XDL_ASSERT(ptd && ptd->widget);

	hand_textor_move_last_page(ptd);

	(*ptd->pf_get_paging)(ptd->widget, &xs);
	_textor_calc_maxing(ptd, 0, 0, xs.w, xs.h, ptd->paged, NULL, NULL, &ptd->max_row, &ptd->max_col, &ptd->object, &ptd->atom, NULL, NULL);

	ptd->cur_row = ptd->sel_row = ptd->max_row;
	ptd->cur_col = ptd->sel_col = ptd->max_col + 1;

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_HANDLE;
}

int hand_textor_move_to_page(textor_context* ptd, int page)
{
	xsize_t xs;
	
	if (page < 1 || page > ptd->pages)
		return ptd->page;

	ptd->page = page;
	
	(*ptd->pf_get_paging)(ptd->widget, &xs);

	_textor_calc_maxing(ptd, 0, 0, xs.w, xs.h,  ptd->paged, NULL, NULL,&ptd->max_row, &ptd->max_col, &ptd->object, &ptd->atom, NULL, NULL);

	ptd->sel_row = ptd->cur_row = 0;
	ptd->sel_col = ptd->cur_col = 0;

	ptd->b_select = 0;

	widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return ptd->page;
}

int hand_textor_move_first_page(textor_context* ptd)
{
	xsize_t xs;
	
	if (ptd->page == 1)
		return ptd->page;

	ptd->page = 1;

	(*ptd->pf_get_paging)(ptd->widget, &xs);

	_textor_calc_maxing(ptd, 0, 0, xs.w, xs.h, ptd->paged, NULL, NULL, &ptd->max_row, &ptd->max_col, &ptd->object, &ptd->atom, NULL, NULL);

	ptd->sel_row = ptd->cur_row = 0;
	ptd->sel_col = ptd->cur_col = 0;

	ptd->b_select = 0;

	widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return ptd->page;
}

int hand_textor_move_prev_page(textor_context* ptd)
{
	xsize_t xs;

	if (ptd->page == 1)
		return ptd->page;

	ptd->page --;

	(*ptd->pf_get_paging)(ptd->widget, &xs);

	_textor_calc_maxing(ptd, 0, 0, xs.w, xs.h, ptd->paged, NULL, NULL, &ptd->max_row, &ptd->max_col, NULL, NULL, &ptd->object, &ptd->atom);

	ptd->sel_row = ptd->cur_row = ptd->max_row;
	ptd->sel_col = ptd->cur_col = ptd->max_col;

	ptd->b_select = 0;

	widget_erase(ptd->widget, NULL);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);

	_textor_reset_caret(ptd);

	return ptd->page;
}

int hand_textor_move_next_page(textor_context* ptd)
{
	xsize_t xs;

	if (ptd->page == ptd->pages)
		return ptd->page;

	ptd->page++;

	(*ptd->pf_get_paging)(ptd->widget, &xs);

	_textor_calc_maxing(ptd, 0, 0, xs.w, xs.h, ptd->paged, NULL, NULL, &ptd->max_row, &ptd->max_col, &ptd->object, &ptd->atom, NULL, NULL);

	ptd->sel_row = ptd->cur_row = 0;
	ptd->sel_col = ptd->cur_col = 0;

	ptd->b_select = 0;

	widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return ptd->page;
}

int hand_textor_move_last_page(textor_context* ptd)
{
	xsize_t xs;

	if (ptd->page == ptd->pages)
		return ptd->page;

	ptd->page = ptd->pages;

	(*ptd->pf_get_paging)(ptd->widget, &xs);

	_textor_calc_maxing(ptd, 0, 0, xs.w, xs.h, ptd->paged, NULL, NULL, &ptd->max_row, &ptd->max_col, &ptd->object, &ptd->atom, NULL, NULL);

	ptd->sel_row = ptd->cur_row = 0;
	ptd->sel_col = ptd->cur_col = 0;

	ptd->b_select = 0;

	widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return ptd->page;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int hand_textor_undo(textor_context* ptd)
{
	TEXTORUNDO *next;

	if (!ptd->ptu)
		return _TEXTOR_PRESS_IGNORE;

	next = ptd->ptu->next;

	ptd->b_select = ptd->ptu->b_select;
	ptd->sel_row = ptd->ptu->sel_row;
	ptd->sel_col = ptd->ptu->sel_col;
	ptd->cur_row = ptd->ptu->cur_row;
	ptd->cur_col = ptd->ptu->cur_col;
	ptd->page = ptd->ptu->page;
	(*ptd->pf_set_text)(ptd->data, ptd->ptu->text, ptd->ptu->len);

	_textor_reset_page(ptd, 1);

	_textor_calc_rowcol(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col, NULL, NULL, &ptd->object, &ptd->atom);

	xsfree(ptd->ptu->text);
	xmem_free(ptd->ptu);
	ptd->ptu = next;

	widget_erase(ptd->widget, NULL);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_ACCEPT;
}

int hand_textor_copy(textor_context* ptd)
{
	res_glob_t gb;
	int len = 0;
	tchar_t* buf;

	if (!ptd->b_select)
		return _TEXTOR_PRESS_IGNORE;

	len = _textor_exec_select(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, NULL, MAX_LONG);
	if (!len)
	{
		return _TEXTOR_PRESS_IGNORE;
	}

	if (!clipboard_open())
		return _TEXTOR_PRESS_IGNORE;

	gb = gmem_alloc((len + 1)* sizeof(tchar_t));
	buf = (tchar_t*)gmem_lock(gb);

	_textor_exec_select(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, (tchar_t*)buf, len);

	gmem_unlock(gb);
	if (!clipboard_put(DEF_CB_FORMAT, gb))
	{
		gmem_free(gb);
		clipboard_close();

		return _TEXTOR_PRESS_IGNORE;
	}

	clipboard_close();

	return _TEXTOR_PRESS_HANDLE;
}

int hand_textor_cut(textor_context* ptd)
{
	void* object = NULL;
	bool_t atom = 0;

	if (!ptd->b_select)
		return _TEXTOR_PRESS_IGNORE;

	if (hand_textor_copy(ptd) != _TEXTOR_PRESS_HANDLE)
		return _TEXTOR_PRESS_IGNORE;

	_textor_done(ptd);

	if (!_textor_exec_delete(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, &ptd->sel_row, &ptd->sel_col, &object, &atom))
	{
		_textor_discard(ptd);
		return _TEXTOR_PRESS_IGNORE;
	}

	ptd->cur_row = ptd->sel_row;
	ptd->cur_col = ptd->sel_col;
	ptd->object = object;
	ptd->atom = atom;

	ptd->b_select = 0;

	_textor_reset_page(ptd, 1);

	widget_erase(ptd->widget, NULL);

	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_ACCEPT;
}

int hand_textor_paste(textor_context* ptd)
{
	res_glob_t gb;
	tchar_t* buf;
	int len;
	int row, col, page;
	void* object = NULL;
	bool_t atom = 0;

	if (!clipboard_open())
		return _TEXTOR_PRESS_IGNORE;

	gb = clipboard_get(DEF_CB_FORMAT);
	if (!gb)
	{
		clipboard_close();
		return _TEXTOR_PRESS_IGNORE;
	}

	buf = (tchar_t*)gmem_lock(gb);
	len = xslen(buf);
	if (!len)
	{
		clipboard_close();
		return _TEXTOR_PRESS_IGNORE;
	}

	_textor_done(ptd);

	if (ptd->b_select)
	{
		if (!_textor_exec_delete(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, &ptd->sel_row, &ptd->sel_col, NULL, NULL))
		{
			_textor_discard(ptd);

			clipboard_close();
			return _TEXTOR_PRESS_IGNORE;
		}

		ptd->cur_row = ptd->sel_row;
		ptd->cur_col = ptd->sel_col;
	}

	page = ptd->page;
	row = ptd->cur_row;
	col = ptd->cur_col;

	if (!_textor_exec_insert(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, &page, &row, &col, &object, &atom, (tchar_t*)buf, len))
	{
		_textor_discard(ptd);

		clipboard_close();
		return _TEXTOR_PRESS_IGNORE;
	}

	ptd->page = page;
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->object = object;
	ptd->atom = atom;

	gmem_unlock(gb);

	clipboard_close();

	ptd->b_select = 0;

	_textor_reset_page(ptd, 1);

	widget_erase(ptd->widget, NULL);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_ACCEPT;
}

void hand_textor_clean(textor_context* ptd)
{
	_textor_clean(ptd);
}

void hand_textor_done(textor_context* ptd)
{
	_textor_done(ptd);
}

void hand_textor_discard(textor_context* ptd)
{
	_textor_discard(ptd);
}

int hand_textor_selected_text(textor_context* ptd, tchar_t* buf, int max)
{
	if (!ptd->b_select)
		return 0;

	return _textor_exec_select(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, buf, max);
}

int hand_textor_replace_text(textor_context* ptd, const tchar_t* token, int len)
{
	int row, col, page;
	void* object = NULL;
	bool_t atom = 0;

	if (!ptd->b_select)
		return _TEXTOR_PRESS_IGNORE;

	_textor_done(ptd);

	if (!_textor_exec_delete(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->sel_row, ptd->sel_col, ptd->cur_row, ptd->cur_col, &ptd->sel_row, &ptd->sel_col, NULL, NULL))
	{
		_textor_discard(ptd);

		return _TEXTOR_PRESS_IGNORE;
	}

	ptd->cur_row = ptd->sel_row;
	ptd->cur_col = ptd->sel_col;

	if (len < 0)
		len = xslen(token);

	page = ptd->page;
	row = ptd->cur_row;
	col = ptd->cur_col;

	if (!_textor_exec_insert(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, &page, &row, &col, &object, &atom, (tchar_t*)token, len))
	{
		_textor_discard(ptd);

		return _TEXTOR_PRESS_IGNORE;
	}

	ptd->page = page;
	ptd->cur_row = row;
	ptd->cur_col = col;
	ptd->sel_row = row;
	ptd->sel_col = col;
	ptd->object = object;
	ptd->atom = atom;

	ptd->b_select = 0;

	_textor_reset_page(ptd, 1);

	widget_erase(ptd->widget, NULL);

	_textor_ensure_visible(ptd, ptd->cur_row, ptd->cur_col);
	_textor_reset_caret(ptd);

	return _TEXTOR_PRESS_ACCEPT;
}

void textor_object_rect(textor_context* ptd, void* object, xrect_t* pxr)
{
	_textor_calc_rect(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, object, pxr);
}

void textor_cursor_point(textor_context* ptd, xpoint_t* ppt)
{
	xpoint_t pt1, pt2;

	_textor_calc_point(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	ppt->x = pt1.x;
	ppt->y = pt2.y;
}

void textor_cursor_rect(textor_context* ptd, xrect_t* pxr)
{
	xpoint_t pt1, pt2;

	_textor_calc_point(ptd, ptd->vb.px, ptd->vb.py, ptd->vb.pw, ptd->vb.ph, ptd->paged, ptd->cur_row, ptd->cur_col, &pt1, &pt2);

	pxr->x = pt1.x;
	pxr->y = pt1.y;
	pxr->w = pt2.x - pt1.x;
	pxr->h = pt2.y - pt1.y;
}

bool_t textor_is_select(textor_context* ptd)
{
	return ptd->b_select;
}

bool_t textor_at_atom(textor_context* ptd)
{
	return ptd->atom;
}

int textor_cur_page(textor_context* ptd)
{
	return ptd->page;
}

int textor_max_page(textor_context* ptd)
{
	return ptd->pages;
}
