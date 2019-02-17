/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc words control document

	@module	wordsbox.c | words control widnows implement file

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

#include "xdcbox.h"
#include "handler.h"
#include "winnc.h"

typedef struct _words_delta_t{
	link_t_ptr words;
	link_t_ptr item;
	long bw, bh;
	int page;
}words_delta_t;

#define GETWORDSDELTA(ph) 	(words_delta_t*)widget_get_user_delta(ph)
#define SETWORDSDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)


#define WORDSBOX_MAX_ITEMS		9
#define WORDSBOX_GUID_SPAN		(float)6

/***************************************************************************************/
void _wordsbox_item_rect(res_win_t widget, link_t_ptr plk, xrect_t* pxr)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_wordsbox_item_rect(&im, &xf, ptd->words, ptd->page, plk, pxr);
	widget_rect_to_pt(widget, pxr);
}

void _wordsbox_reset_page(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };
	xrect_t xr;
	xsize_t xs;

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_wordsbox_size(&im, &xf, ptd->words, &xs);
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);

	widget_reset_paging(widget, xr.w, xr.h, xs.cx, xs.cy, 0, 0);
}

void _wordsbox_ensure_visible(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	xrect_t xr;

	if (!ptd->item)
		return;

	_wordsbox_item_rect(widget, ptd->item, &xr);

	widget_ensure_visible(widget, &xr, 1);
}

/*************************************************************************/

void noti_wordsbox_command(res_win_t widget, int code, var_long data)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	if (widget_has_subproc(widget))
		widget_post_command(widget, code, IDC_SELF, data);
	else
		widget_post_command(widget_get_owner(widget), code, widget_get_user_id(widget), data);
}

void wordsbox_on_item_changing(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->item);

	_wordsbox_item_rect(widget,ptd->item, &xr);

	ptd->item = NULL;

	widget_update(widget, &xr, 0);
}

void wordsbox_on_item_changed(res_win_t widget, link_t_ptr elk)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->item);

	ptd->item = elk;

	_wordsbox_item_rect(widget, ptd->item, &xr);
	
	widget_update(widget, &xr, 0);

	noti_wordsbox_command(widget, COMMAND_UPDATE, (var_long)NULL);
}

/********************************************************************************************/
int hand_words_create(res_win_t widget, void* data)
{
	words_delta_t* ptd;
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xsize_t xs;

	ptd = (words_delta_t*)xmem_alloc(sizeof(words_delta_t));

	widget_get_xfont(widget, &xf);

	rdc = widget_client_ctx(widget);
	text_metric_raw(rdc, &xf, &xs);
	widget_release_ctx(widget, rdc);

	ptd->bw = xs.cx;
	ptd->bh = xs.cy;

	ptd->page = 0;

	SETWORDSDELTA(widget, ptd);

	return 0;
}

void hand_words_destroy(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	xmem_free(ptd);

	SETWORDSDELTA(widget, 0);
}

void hand_words_keydown(res_win_t widget, int key)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	if (!ptd->words)
		return;

	switch (key)
	{
	case KEY_LEFT:
		wordsbox_tabskip(widget,WD_TAB_LEFT);
		break;
	case KEY_RIGHT:
		wordsbox_tabskip(widget,WD_TAB_RIGHT);
		break;
	case KEY_UP:
		wordsbox_tabskip(widget,WD_TAB_UP);
		break;
	case KEY_DOWN:
		wordsbox_tabskip(widget,WD_TAB_DOWN);
		break;
	case KEY_HOME:
		wordsbox_tabskip(widget,WD_TAB_HOME);
		break;
	case KEY_END:
		wordsbox_tabskip(widget,WD_TAB_END);
		break;
	case KEY_PAGEUP:
		wordsbox_tabskip(widget,WD_TAB_PAGEUP);
		break;
	case KEY_PAGEDOWN:
		wordsbox_tabskip(widget,WD_TAB_PAGEDOWN);
		break;
	}
}


void hand_words_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	if (!ptd->words)
		return;
}

void hand_words_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	if (!ptd->words)
		return;
}

void hand_words_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	link_t_ptr ilk = NULL;
	xpoint_t pt;

	if (!ptd->words)
		return;

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	calc_wordsbox_hint(&im, &xf, &pt, ptd->words, ptd->page, &ilk);

	if (ilk != ptd->item)
	{
		if (ptd->item)
			wordsbox_on_item_changing(widget);

		if (ilk)
			wordsbox_on_item_changed(widget, ilk);
	}

	noti_wordsbox_command(widget, COMMAND_SELECT, (var_long)NULL);
}

void hand_words_size(res_win_t widget, int code, const xsize_t* prs)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	if (!ptd->words)
		return;

	wordsbox_redraw(widget);
}

void hand_words_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	if (!ptd->words)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_words_erase(res_win_t widget, res_ctx_t rdc)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	
	if (!ptd->words)
		return;
}

void hand_words_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	res_ctx_t rdc;
	xrect_t xr;
	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb = { 0 };

	xfont_t xf;
	xbrush_t xb;
	xpen_t xp;
	xcolor_t xc;

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);

	pif = create_canvas_interface(canv);

	parse_xcolor(&pif->clr_bkg, xb.color);
	parse_xcolor(&pif->clr_frg, xp.color);
	parse_xcolor(&pif->clr_txt, xf.color);
	widget_get_xcolor(widget, &pif->clr_msk);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	draw_wordsbox(pif, &cb, &xf, ptd->words, ptd->page);

	if (ptd->item)
	{
		_wordsbox_item_rect(widget, ptd->item, &xr);

		pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

		parse_xcolor(&xc, DEF_FOCUS_COLOR);
		draw_focus_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	end_canvas_paint(canv, dc, pxr);

	destroy_canvas_interface(pif);

}

/************************************************************************************************/
res_win_t wordsbox_create(res_win_t widget, dword_t style, const xrect_t* pxr)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_words_create)
		EVENT_ON_DESTROY(hand_words_destroy)
		EVENT_ON_ERASE(hand_words_erase)
		EVENT_ON_PAINT(hand_words_paint)
		EVENT_ON_SIZE(hand_words_size)
		EVENT_ON_SCROLL(hand_words_scroll)
		EVENT_ON_KEYDOWN(hand_words_keydown)
		EVENT_ON_MOUSE_MOVE(hand_words_mouse_move)
		EVENT_ON_LBUTTON_DOWN(hand_words_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_words_lbutton_up)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(NULL, style, pxr, widget, &ev);
}

void wordsbox_set_data(res_win_t widget, link_t_ptr ptr)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->words = ptr;
	ptd->item = NULL;

	ptd->page = 1;

	wordsbox_redraw(widget);
}

link_t_ptr wordsbox_get_data(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->words;
}

void wordsbox_redraw(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	link_t_ptr ilk;
	bool_t b_valid;
	int pages;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	b_valid = 0;
	ilk = get_words_next_visible_item(ptd->words, LINK_FIRST);
	while (ilk)
	{
		if (ilk == ptd->item)
			b_valid = 1;

		ilk = get_words_next_visible_item(ptd->words, ilk);
	}

	if (!b_valid)
	{
		ptd->item = NULL;
	}

	pages = calc_wordsbox_pages(ptd->words);
	if (ptd->page > pages)
		ptd->page = pages;

	_wordsbox_reset_page(widget);

	widget_update_window(widget);

	widget_update(widget, NULL, 0);
}

bool_t wordsbox_set_focus_item(res_win_t widget, link_t_ptr ent)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	bool_t bRe;
	int page;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return 0;

	if (ent)
	{
#ifdef _DEBUG
		XDL_ASSERT(is_words_item(ptd->words, ent));
#endif
	}

	bRe = (ent == ptd->item) ? 1 : 0;

	if (!bRe && ptd->item)
	{
		wordsbox_on_item_changing(widget);
	}

	if (!bRe && ent)
	{
		wordsbox_on_item_changed(widget, ent);

		_wordsbox_ensure_visible(widget);
	}

	if (ptd->item)
	{
		page = calc_wordsbox_item_page(ptd->words, ptd->item);
		if (page != ptd->page)
		{
			wordsbox_move_to_page(widget, page);
		}
	}

	return 1;
}

link_t_ptr wordsbox_get_focus_item(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return NULL;

	return ptd->item;
}

void wordsbox_get_item_rect(res_win_t widget, link_t_ptr elk, xrect_t* prt)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_words_item(ptd->words, elk));
#endif

	_wordsbox_item_rect(widget, elk, prt);
}

void wordsbox_tabskip(res_win_t widget, int nSkip)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	link_t_ptr plk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	switch (nSkip)
	{
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (ptd->item)
			plk = get_words_next_visible_item(ptd->words, ptd->item);
		else
			plk = get_words_next_visible_item(ptd->words, LINK_FIRST);

		if (plk)
			wordsbox_set_focus_item(widget, plk);
		break;
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (ptd->item)
			plk = get_words_prev_visible_item(ptd->words, ptd->item);
		else
			plk = get_words_prev_visible_item(ptd->words, LINK_LAST);

		if (plk)
			wordsbox_set_focus_item(widget, plk);
		break;
	case WD_TAB_HOME:
		wordsbox_move_first_page(widget);
		break;
	case WD_TAB_END:
		wordsbox_move_last_page(widget);
		break;
	case WD_TAB_PAGEUP:
		wordsbox_move_prev_page(widget);
		break;
	case WD_TAB_PAGEDOWN:
		wordsbox_move_next_page(widget);
		break;
	}
}

void wordsbox_move_prev_page(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	nCurPage = ptd->page;

	if (nCurPage > 1)
	{
		nCurPage--;
		ptd->page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

void wordsbox_move_next_page(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	int nCurPage, nMaxPage;
	xsize_t xs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	widget_get_canv_size(widget, &xs);

	nCurPage = ptd->page;
	nMaxPage = calc_wordsbox_pages(ptd->words);

	if (nCurPage < nMaxPage)
	{
		nCurPage++;
		ptd->page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

void wordsbox_move_first_page(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	int nCurPage;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	nCurPage = ptd->page;

	if (nCurPage != 1)
	{
		nCurPage = 1;
		ptd->page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

void wordsbox_move_last_page(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	int nCurPage, nMaxPage;
	xsize_t xs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	widget_get_canv_size(widget, &xs);

	nCurPage = ptd->page;
	nMaxPage = calc_wordsbox_pages(ptd->words);

	if (nCurPage != nMaxPage)
	{
		nCurPage = nMaxPage;
		ptd->page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

void wordsbox_move_to_page(res_win_t widget, int page)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	int nCurPage, nMaxPage;
	xsize_t xs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	widget_get_canv_size(widget, &xs);

	nCurPage = ptd->page;
	nMaxPage = calc_wordsbox_pages(ptd->words);

	if (page > 0 && page != nCurPage && page <= nMaxPage)
	{
		nCurPage = page;
		ptd->page = nCurPage;

		widget_update(widget, NULL, 0);
	}
}

int wordsbox_get_max_page(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	xsize_t xs;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return 0;

	widget_get_canv_size(widget, &xs);

	return calc_wordsbox_pages(ptd->words);
}

int wordsbox_get_page(res_win_t widget)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return 0;

	return ptd->page;
}

void wordsbox_find(res_win_t widget, link_t_ptr pos, const tchar_t* token)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	link_t_ptr elk;
	int tlen;
	const tchar_t* text;
	tchar_t* help;
	int hlen;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	if (is_null(token))
	{
		wordsbox_set_focus_item(widget, NULL);
		return;
	}

	tlen = xslen(token);
	help = xsalloc(tlen + 1);

	if (pos == LINK_FIRST)
		elk = get_words_next_visible_item(ptd->words, LINK_FIRST);
	if (pos == LINK_LAST)
		elk = NULL;
	else
		elk = get_words_next_visible_item(ptd->words, pos);

	while (elk)
	{
		text = get_words_item_text_ptr(elk);

		if (xsnicmp(text, token, tlen) == 0)
			break;

		hlen = xslen(text);
		if (hlen)
		{
			help_code(text, hlen, help, tlen);
			if (xsnicmp(help, token, tlen) == 0)
				break;
		}

		elk = get_words_next_visible_item(ptd->words, elk);
	}

	xsfree(help);

	wordsbox_set_focus_item(widget, elk);
}

void wordsbox_filter(res_win_t widget, const tchar_t* token)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	link_t_ptr elk;
	int tlen;
	const tchar_t* text;

	bool_t b_redraw, b_hidden;
	tchar_t* help;
	int hlen;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return;

	wordsbox_set_focus_item(widget, NULL);

	tlen = xslen(token);
	help = xsalloc(tlen + 1);

	b_redraw = 0;
	elk = get_words_next_item(ptd->words, LINK_FIRST);
	while (elk)
	{
		b_hidden = 1;

		if (is_null(token))
		{
			b_hidden = 0;
		}
		else
		{
			text = get_words_item_text_ptr(elk);

			if (xsnicmp(text, token, tlen) == 0)
				b_hidden = 0;

			hlen = xslen(text);
			if (hlen)
			{
				help_code(text, hlen, help, tlen);
				if (xsnicmp(help, token, tlen) == 0)
					b_hidden = 0;
			}
		}

		if (b_hidden != get_words_item_hidden(elk))
		{
			set_words_item_hidden(elk, b_hidden);
			b_redraw = 1;
		}

		elk = get_words_next_item(ptd->words, elk);
	}

	xsfree(help);

	if (b_redraw)
		wordsbox_redraw(widget);
}

link_t_ptr wordsbox_seek(res_win_t widget, int index)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	link_t_ptr elk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->words)
		return NULL;

	elk = calc_wordsbox_item(ptd->words, ptd->page, index);

	wordsbox_set_focus_item(widget, elk);

	return elk;
}

void wordsbox_popup_size(res_win_t widget, xsize_t* pxs)
{
	words_delta_t* ptd = GETWORDSDELTA(widget);
	if_measure_t im = { 0 };
	xfont_t xf = { 0 };

	XDL_ASSERT(ptd != NULL);

	widget_get_xfont(widget, &xf);

	im.ctx = widget_get_canvas(widget);
	im.pf_text_metric = (PF_TEXT_METRIC)text_metric;
	im.pf_text_size = (PF_TEXT_SIZE)text_size;

	calc_wordsbox_size(&im, &xf, ptd->words, pxs);

	widget_size_to_pt(widget, pxs);

	widget_adjust_size(widget_get_style(widget), pxs);
}

