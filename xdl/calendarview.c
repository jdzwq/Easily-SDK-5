/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc calendar document

	@module	calendarview.c | implement file

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
#include "calendarview.h"
#include "xdlview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

#define CALENDAR_BAR_SPAN		((float)5)

void calc_calendar_daily_rect(link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	xdate_t dt;
	calendar_t ca = { 0 };
	float iw, ih, th;
	int i,j;
	bool_t b = 0;

	th = get_calendar_title_height(ptr);
	iw = get_calendar_width(ptr) / CALENDAR_COL;
	ih = (get_calendar_height(ptr) - th) / CALENDAR_ROW;

	parse_date(&dt, get_calendar_today_ptr(ptr));
	fill_calendar(&ca, &dt);

	parse_date(&dt, get_calendar_daily_name_ptr(ilk));

	for (i = 0; i < CALENDAR_ROW; i++)
	{
		for (j = 0; j < CALENDAR_COL; j++)
		{
			if (ca.calen_days[i][j] == dt.day)
			{
				b = 1;
				break;
			}
		}

		if (b)
			break;
	}

	if (b)
	{
		pxr->fx = j * iw;
		pxr->fy = th + i * ih;
		pxr->fw = iw;
		pxr->fh = ih;
	}
	else
	{
		xmem_set((void*)pxr, 0, sizeof(xrect_t));
	}
}

int calc_calendar_hint(link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	link_t_ptr ilk;
	int nHit;
	float pw, ph, ic, iw, ih, th;
	xrect_t xr;

	nHit = CALENDAR_HINT_NONE;
	*pilk = NULL;

	pw = get_calendar_width(ptr);
	ph = get_calendar_height(ptr);
	th = get_calendar_title_height(ptr);
	iw = get_calendar_width(ptr) / CALENDAR_COL;
	ih = (get_calendar_height(ptr) - th) / CALENDAR_ROW;
	ic = iw / 2;

	xr.fx = 3 * iw;
	xr.fy = th / 2;
	xr.fw = ic;
	xr.fh = CALENDAR_BAR_SPAN;

	if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
	{
		return CALENDAR_HINT_DAYBTN;
	}

	xr.fx += ic;
	if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
	{
		return CALENDAR_HINT_WEEKBTN;
	}

	xr.fx += ic;
	if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
	{
		return CALENDAR_HINT_MONTHBTN;
	}

	xr.fx += ic;
	if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
	{
		return CALENDAR_HINT_YEARBTN;
	}

	xr.fx = pw - 2 * ic;
	xr.fy = th / 2;
	xr.fw = ic / 2;
	xr.fh = CALENDAR_BAR_SPAN;

	if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
	{
		return CALENDAR_HINT_PREVBTN;
	}

	xr.fx += ic / 2;
	xr.fw = ic;
	if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
	{
		return CALENDAR_HINT_TODAYBTN;
	}

	xr.fx += ic;
	xr.fw = ic / 2;
	if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
	{
		return CALENDAR_HINT_NEXTBTN;
	}

	ilk = get_calendar_prev_daily(ptr, LINK_LAST);
	while (ilk)
	{
		calc_calendar_daily_rect(ptr, ilk, &xr);

		if (ft_inside(ppt->fx, ppt->fy, xr.fx, xr.fy, xr.fx + xr.fw, xr.fy + xr.fh))
		{
			*pilk = ilk;

			nHit = CALENDAR_HINT_DAILY;
			break;
		}
		ilk = get_calendar_prev_daily(ptr, ilk);
	}

	return nHit;
}

void draw_calendar(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr ilk;
	xrect_t xr;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	canvbox_t cb = { 0 };

	const tchar_t* style;
	const tchar_t* daily;
	bool_t b_print;

	float pw, ph, ic, iw, ih, th;
	tchar_t sz_token[DATE_LEN];

	xdate_t dt;
	calendar_t ca = { 0 };
	int i, j;

	pw = get_calendar_width(ptr);
	ph = get_calendar_height(ptr);
	th = get_calendar_title_height(ptr);
	iw = get_calendar_width(ptr) / CALENDAR_COL;
	ih = (get_calendar_height(ptr) - th) / CALENDAR_ROW;
	ic = iw / 2;

	b_print = (pif->canvas->tag == _CANVAS_PRINTER) ? 1 : 0;

	default_xfont(&xf);
	default_xface(&xa);
	default_xpen(&xp);
	default_xbrush(&xb);

	style = get_calendar_style_ptr(ptr);

	parse_xfont_from_style(&xf, style);
	parse_xface_from_style(&xa, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_txt, xf.color);
	}

	parse_xpen_from_style(&xp, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_frg, xp.color);
	}

	parse_xbrush_from_style(&xb, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_bkg, xb.color);
	}

	if (!b_print)
	{
		xmem_copy((void*)&xc, &pif->clr_ico, sizeof(xcolor_t));
	}
	else
	{
		parse_xcolor(&xc, xp.color);
	}

	//title
	ltoxs(xstol(xf.size) + 2, xf.size, INT_LEN);

	xr.fx = 0;
	xr.fy = 0;
	xr.fw = 3 * iw;
	xr.fh = ih / 2;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);
	
	parse_date(&dt, get_calendar_today_ptr(ptr));
	xsprintf(sz_token, _T("%d年%02d月"), dt.year, dt.mon);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz_token, -1);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	//daily bar
	ltoxs(xstol(xf.size) - 2, xf.size, INT_LEN);

	xr.fx = 3 * iw;
	xr.fy = th / 2 - CALENDAR_BAR_SPAN;
	xr.fw = ic;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("日"), -1);

	xr.fx += ic;
	(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("周"), -1);

	xr.fx += ic;
	(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("月"), -1);

	xr.fx += ic;
	(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("年"), -1);

	xr.fx = pw - 2 * ic;
	xr.fy = th / 2 - CALENDAR_BAR_SPAN;
	xr.fw = ic / 2;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
	ft_center_rect(&xr, DEF_SMALL_ICON, DEF_SMALL_ICON);
	(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr, GDI_ICON_PREV);

	xr.fx = pw - ic / 2;
	xr.fy = th / 2 - CALENDAR_BAR_SPAN;
	xr.fw = ic / 2;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
	ft_center_rect(&xr, DEF_SMALL_ICON, DEF_SMALL_ICON);
	(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr, GDI_ICON_NEXT);

	xr.fx = pw - ic / 2 * 3;
	xr.fy = th / 2 - CALENDAR_BAR_SPAN;
	xr.fw = ic;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("今天"), -1);

	//week bar
	xr.fx = 0;
	xr.fy = th - CALENDAR_BAR_SPAN;
	xr.fw = iw;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("周日"), -1);

	xr.fx += iw;
	xr.fy = th - CALENDAR_BAR_SPAN;
	xr.fw = iw;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("周一"), -1);

	xr.fx += iw;
	xr.fy = th - CALENDAR_BAR_SPAN;
	xr.fw = iw;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("周二"), -1);

	xr.fx += iw;
	xr.fy = th - CALENDAR_BAR_SPAN;
	xr.fw = iw;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("周三"), -1);

	xr.fx += iw;
	xr.fy = th - CALENDAR_BAR_SPAN;
	xr.fw = iw;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("周四"), -1);

	xr.fx += iw;
	xr.fy = th - CALENDAR_BAR_SPAN;
	xr.fw = iw;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("周五"), -1);

	xr.fx += iw;
	xr.fy = th - CALENDAR_BAR_SPAN;
	xr.fw = iw;
	xr.fh = CALENDAR_BAR_SPAN;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, _T("周六"), -1);

	xr.fx = 0;
	xr.fy = th;
	xr.fw = pw;
	xr.fh = ph - th;
	ft_offset_rect(&xr, pbox->fx, pbox->fy);

	//(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);

	parse_date(&dt, get_calendar_today_ptr(ptr));
	fill_calendar(&ca, &dt);

	ltoxs(xstol(xf.size) - 2, xf.size, INT_LEN);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);

	lighten_xbrush(&xb, DEF_SOFT_DARKEN);

	for (i = 0; i < CALENDAR_ROW; i++)
	{
		for (j = 0; j < CALENDAR_COL; j++)
		{
			xr.fx = j * iw;
			xr.fy = th + i * ih;
			xr.fw = iw;
			xr.fh = ih;
			ft_offset_rect(&xr, pbox->fx, pbox->fy);

			if (!j || j == 6)
				(*pif->pf_draw_rect)(pif->canvas, &xp, &xb, &xr);
			else
				(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);

			if (ca.calen_days[i][j])
			{
				xr.fx = j * iw;
				xr.fy = th + i * ih;
				xr.fw = iw;
				xr.fh = CALENDAR_BAR_SPAN;
				ft_offset_rect(&xr, pbox->fx, pbox->fy);

				xsprintf(sz_token, _T("%d日"), ca.calen_days[i][j]);
				(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz_token, -1);
			}
		}
	}
}

#endif //XDLVIEW
