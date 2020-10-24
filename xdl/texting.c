/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc coding document

	@module	coding.c | implement file

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

#include "texting.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDL_SUPPORT_VIEW)

typedef struct _FIXTEXT_SCAN{
	xrect_t xr;
}FIXTEXT_SCAN;

static int _fix_text_calc_rect(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	FIXTEXT_SCAN* ptt = (FIXTEXT_SCAN*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->xr.w = ptm->cur_x + ptm->cur_w - ptm->min_x;
		ptt->xr.h = ptm->cur_y + ptm->cur_h - ptm->min_y;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void calc_text_rect_raw(const if_drawing_t* piv, const xfont_t* pxf, const xface_t* pxa, const tchar_t* sz_text, int sz_len, xrect_t* pxr)
{
	FIXTEXT_SCAN tt = { 0 };
	if_measure_t im = { 0 };

	xmem_copy((void*)&tt.xr, (void*)pxr, sizeof(xrect_t));

	if (sz_len < 0)
		sz_len = xslen(sz_text);
	if (sz_text)
		sz_len++;

	(*piv->pf_get_measure)(piv->ctx, &im);

	scan_fix_text((tchar_t*)sz_text, sz_len, &im, pxf, pxa, pxr->x, pxr->y, pxr->w, MAX_LONG, 0, _fix_text_calc_rect, (void*)&tt);

	xmem_copy((void*)pxr, (void*)&tt.xr, sizeof(xrect_t));
}

void calc_text_rect(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const tchar_t* txt, int len, xrect_t* pxr)
{
	xrect_t xr = { 0 };
	visual_t visu;
	if_drawing_t iv = { 0 };

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &iv);

	calc_text_rect_raw(&iv, pxf, pxa, txt, len, &xr);

	(pif->pf_rect_pt_to_tm)(pif->ctx, &xr);

	xmem_copy((void*)pxr, (void*)&xr, sizeof(xrect_t));
}

typedef struct _VARTEXT_DRAW{
	const if_drawing_t* piv;
	int page;
}VARTEXT_DRAW;

static int _var_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	VARTEXT_DRAW* ptt = (VARTEXT_DRAW*)pp;
	xpoint_t pt;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		pt.x = ptm->cur_x;
		pt.y = ptm->cur_y;

		(*(ptt->piv->pf_text_out))(ptt->piv->ctx, pxf, &pt, cur_char, cur_count);
		break;
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void draw_var_text_raw(const if_drawing_t* piv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t data)
{
	VARTEXT_DRAW tt = { 0 };
	if_measure_t im = { 0 };

	tt.piv = piv;
	tt.page = 0;

	(*piv->pf_get_measure)(piv->ctx, &im);

	scan_var_text(data, &im, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 0, _var_text_calc_draw, (void*)&tt);
}

void draw_var_text(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, string_t var)
{
	xrect_t xr;
	visual_t visu;
	if_drawing_t iv = { 0 };

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	(*pif->pf_rect_tm_to_pt)(pif->ctx, &xr);

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &iv);

	draw_var_text_raw(&iv, pxf, pxa, &xr, var);
}

typedef struct _TAGTEXT_DRAW{
	const if_drawing_t* piv;
	int page;
}TAGTEXT_DRAW;

static int _tag_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TAGTEXT_DRAW* ptt = (TAGTEXT_DRAW*)pp;
	xpoint_t pt;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		pt.x = ptm->cur_x;
		pt.y = ptm->cur_y;

		(*(ptt->piv->pf_text_out))(ptt->piv->ctx, pxf, &pt, cur_char, cur_count);
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void draw_tag_text_raw(const if_drawing_t* piv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	TAGTEXT_DRAW tt = { 0 };
	if_measure_t im = { 0 };

	tt.piv = piv;
	tt.page = page;

	(*piv->pf_get_measure)(piv->ctx, &im);

	scan_tag_text(data, &im, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _tag_text_calc_draw, (void*)&tt);
}

void draw_tag_text(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag, int page)
{
	xrect_t xr;
	visual_t visu;
	if_drawing_t iv = { 0 };

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	(*pif->pf_rect_tm_to_pt)(pif->ctx, &xr);

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &iv);

	draw_tag_text_raw(&iv, pxf, pxa, &xr, tag, page);
}

typedef struct _TAGTEXT_PAGE{
	int pages;
}TAGTEXT_PAGE;

static int _tag_text_calc_pages(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	TAGTEXT_PAGE* ptt = (TAGTEXT_PAGE*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->pages = page;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

int calc_tag_pages_raw(const if_drawing_t* piv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data)
{
	TAGTEXT_PAGE tt = { 0 };
	if_measure_t im = { 0 };

	tt.pages = 1;

	(*piv->pf_get_measure)(piv->ctx, &im);

	scan_tag_text(data, &im, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _tag_text_calc_pages, (void*)&tt);

	return tt.pages;
}

int calc_tag_pages(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr tag)
{
	xrect_t xr;
	visual_t visu;
	if_drawing_t iv = { 0 };

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	(*pif->pf_rect_tm_to_pt)(pif->ctx, &xr);

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &iv);

	return calc_tag_pages_raw(&iv, pxf, pxa, &xr, tag);
}

typedef struct _MEMOTEXT_DRAW{
	const if_drawing_t* piv;
	int page;
}MEMOTEXT_DRAW;

static int _memo_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	MEMOTEXT_DRAW* ptt = (MEMOTEXT_DRAW*)pp;
	xpoint_t pt;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		pt.x = ptm->cur_x;
		pt.y = ptm->cur_y;

		(*(ptt->piv->pf_text_out))(ptt->piv->ctx, pxf, &pt, cur_char, cur_count);
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void draw_memo_text_raw(const if_drawing_t* piv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	MEMOTEXT_DRAW tt = { 0 };
	if_measure_t im = { 0 };

	tt.piv = piv;
	tt.page = page;

	(*piv->pf_get_measure)(piv->ctx, &im);

	scan_memo_text(data, &im, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _memo_text_calc_draw, (void*)&tt);
}

void draw_memo_text(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo, int page)
{
	xrect_t xr;
	visual_t visu;
	if_drawing_t iv = { 0 };

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	(*pif->pf_rect_tm_to_pt)(pif->ctx, &xr);

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &iv);

	draw_memo_text_raw(&iv, pxf, pxa, &xr, memo, page);
}

typedef struct _MEMOTEXT_PAGE{
	int pages;
}MEMOTEXT_PAGE;

static int _memo_text_calc_pages(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	MEMOTEXT_PAGE* ptt = (MEMOTEXT_PAGE*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->pages = page;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

int calc_memo_pages_raw(const if_drawing_t* piv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data)
{
	MEMOTEXT_PAGE tt = { 0 };
	if_measure_t im = { 0 };

	tt.pages = 1;

	(*piv->pf_get_measure)(piv->ctx, &im);

	scan_memo_text(data, &im, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _memo_text_calc_pages, (void*)&tt);

	return tt.pages;
}

int calc_memo_pages(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr memo)
{
	xrect_t xr;
	if_drawing_t iv = { 0 };

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	(*pif->pf_rect_tm_to_pt)(pif->ctx, &xr);

	return calc_memo_pages_raw(&iv, pxf, pxa, &xr, memo);
}

typedef struct _RICHTEXT_DRAW{
	const if_drawing_t* piv;
	int page;
}RICHTEXT_DRAW;

static int _rich_text_calc_draw(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	RICHTEXT_DRAW* ptt = (RICHTEXT_DRAW*)pp;
	xpoint_t pt;

	if (page < ptt->page)
		return _SCANNER_OPERA_PAGED;
	else if (page > ptt->page)
		return _SCANNER_OPERA_STOP;

	switch (scan)
	{
	case _SCANNER_STATE_WORDS:
		if (cur_char && *cur_char == _T('\n'))
			break;

		pt.x = ptm->cur_x;
		pt.y = ptm->cur_y;

		(*(ptt->piv->pf_text_out))(ptt->piv->ctx, pxf, &pt, cur_char, cur_count);
		break;
	case _SCANNER_STATE_NEWPAGE:
	case _SCANNER_STATE_END:
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

void draw_rich_text_raw(const if_drawing_t* piv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data, int page)
{
	RICHTEXT_DRAW tt = { 0 };
	if_measure_t im = { 0 };

	tt.piv = piv;
	tt.page = page;

	(*piv->pf_get_measure)(piv->ctx, &im);

	scan_rich_text(data, &im, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _rich_text_calc_draw, (void*)&tt);
}

void draw_rich_text(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich, int page)
{
	xrect_t xr;
	visual_t visu;
	if_drawing_t iv = { 0 };

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	(*pif->pf_rect_tm_to_pt)(pif->ctx, &xr);

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &iv);

	draw_rich_text_raw(&iv, pxf, pxa, &xr, rich, page);
}

typedef struct _RICHTEXT_PAGE{
	int pages;
}RICHTEXT_PAGE;

static int _rich_text_calc_pages(int scan, void* object, bool_t b_atom, bool_t b_ins, bool_t b_del, bool_t b_sel, const tchar_t* cur_char, int cur_count, tchar_t* ret_char, int page, int cur_row, int cur_col, const WORDPLACE* ptm, const xfont_t* pxf, const xface_t* pxa, void* pp)
{
	RICHTEXT_PAGE* ptt = (RICHTEXT_PAGE*)pp;

	switch (scan)
	{
	case _SCANNER_STATE_END:
		ptt->pages = page;
		return _SCANNER_OPERA_STOP;
	}

	return _SCANNER_OPERA_NEXT;
}

int calc_rich_pages_raw(const if_drawing_t* piv, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr data)
{
	RICHTEXT_PAGE tt = { 0 };
	if_measure_t im = { 0 };

	tt.pages = 1;

	(*piv->pf_get_measure)(piv->ctx, &im);

	scan_rich_text(data, &im, pxf, pxa, pxr->x, pxr->y, pxr->w, pxr->h, 1, _rich_text_calc_pages, (void*)&tt);

	return tt.pages;
}

int calc_rich_pages(const if_drawing_t* pif, const xfont_t* pxf, const xface_t* pxa, const xrect_t* pxr, link_t_ptr rich)
{
	xrect_t xr;
	visual_t visu;
	if_drawing_t iv = { 0 };

	xmem_copy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	(*pif->pf_rect_tm_to_pt)(pif->ctx, &xr);

	visu = (*pif->pf_get_visual_handle)(pif->ctx);
	(*pif->pf_get_visual_interface)(visu, &iv);

	return calc_rich_pages_raw(&iv, pxf, pxa, &xr, rich);
}


#endif /*XDL_SUPPORT_VIEW*/
