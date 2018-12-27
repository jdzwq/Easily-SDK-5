/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc graph document

	@module	graphview.c | graph document implement file

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

#include "graphview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

#define GRAPH_MINFEED  (float)1.0

float calc_graph_page_width(const canvbox_t* pbox, link_t_ptr ptr, int page)
{
	link_t_ptr xlk, xlk_first, xlk_last;
	float yw, xw;

	yw = get_graph_yaxbar_width(ptr);
	xw = get_graph_xaxbar_width(ptr);

	calc_graph_xax_scope(pbox, ptr, page, &xlk_first, &xlk_last);
	xlk = xlk_first;
	while (xlk)
	{
		yw += xw;
		if (xlk == xlk_last)
			break;
		xlk = get_next_xax(ptr, xlk);
	}

	return yw;
}

int calc_graph_pages(const canvbox_t* pbox, link_t_ptr ptr)
{
	int xaxsperpage, xaxs;
	float pw,ph, yw, xw;

	pw = pbox->fw;
	ph = pbox->fh;

	yw = get_graph_yaxbar_width(ptr);
	xw = get_graph_xaxbar_width(ptr);

	xaxsperpage = (int)((pw - yw) / xw);
	if (xaxsperpage <= 0)
		return 1;

	xaxs = get_xax_count(ptr);	
	if (xaxs == 0)
		return 1;
	if (xaxs % xaxsperpage == 0)
		return xaxs / xaxsperpage;
	else
		return xaxs / xaxsperpage + 1;
}

void calc_graph_xax_scope(const canvbox_t* pbox, link_t_ptr ptr, int page, link_t_ptr* firstxax, link_t_ptr* lastxax)
{
	int xaxsperpage, pageindex;
	float pw,ph,yw, xw;
	link_t_ptr xlk;

	pageindex = page;
	pw = pbox->fw;
	ph = pbox->fh;
	yw = get_graph_yaxbar_width(ptr);
	xw = get_graph_xaxbar_width(ptr);

	xaxsperpage = (int)((pw - yw) / xw);
	if (xaxsperpage <= 0)
	{
		*firstxax = NULL;
		*lastxax = NULL;
		return;
	}

	*firstxax = get_xax_at(ptr, (pageindex - 1) * xaxsperpage);
	if (*firstxax == NULL)
	{
		*lastxax = NULL;
		return;
	}

	xlk = *firstxax;
	while (xlk && xaxsperpage--)
	{
		*lastxax = xlk;
		xlk = get_next_xax(ptr, xlk);
	}
}

int calc_graph_xax_page(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr xlk)
{
	int xaxsperpage, page, count;
	float pw,ph, yw, xw;
	link_t_ptr plk;

	if (xlk == NULL)
		return 0;

	pw = pbox->fw;
	ph = pbox->fh;
	yw = get_graph_yaxbar_width(ptr);
	xw = get_graph_xaxbar_width(ptr);

	xaxsperpage = (int)((pw - yw) / xw);
	if (xaxsperpage <= 0)
		return 0;

	page = 1;
	count = xaxsperpage;
	plk = get_next_xax(ptr, LINK_FIRST);
	while (plk && plk != xlk)
	{
		count--;
		if (count == 0)
		{
			page++;
			count = xaxsperpage;
		}
		plk = get_next_xax(ptr, plk);
	}
	return page;
}

void calc_graph_yax_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ylk, xrect_t* pxr)
{
	link_t_ptr plk;
	float yh, xh;

	xh = get_graph_xaxbar_height(ptr);
	yh = get_graph_yaxbar_height(ptr);

	pxr->fx = 0;
	pxr->fy = get_graph_title_height(ptr) + xh;
	
	plk = get_next_yax(ptr, LINK_FIRST);
	while (plk)
	{
		if (ylk == plk)
			break;

		pxr->fy += yh;
		plk = get_next_yax(ptr, plk);
	}

	if (plk)
	{
		pxr->fw = get_graph_yaxbar_width(ptr);
		pxr->fh = yh;
	}
	else
	{
		pxr->fw = 0;
		pxr->fh = 0;
	}
}

void calc_graph_gax_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr glk, xrect_t* pxr)
{
	link_t_ptr plk;
	float th, yw, yh, xh, gw;
	int gn;

	gn = get_gax_count(ptr);
	if (!gn)
		return;

	th = get_graph_title_height(ptr);
	xh = get_graph_xaxbar_height(ptr);
	yw = get_graph_yaxbar_width(ptr);
	yh = get_graph_yaxbar_height(ptr);

	gw = (yw - yh) / gn;
	if (gw < 1)
		return;

	pxr->fx = 0;
	pxr->fy = th + xh + get_yax_count(ptr) * yh;

	plk = get_next_gax(ptr, LINK_FIRST);
	while (plk)
	{
		if (glk == plk)
			break;

		pxr->fx += gw;
		plk = get_next_gax(ptr, plk);
	}

	if (plk)
	{
		pxr->fw = gw;
		pxr->fh = yh;
	}
	else
	{
		pxr->fw = 0;
		pxr->fh = 0;
	}
}

int calc_graph_coor_rect(const canvbox_t* pbox, link_t_ptr ptr, int page, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr)
{
	link_t_ptr xax, yax, xlk_first, xlk_last;
	float th, xw, xh, yw, yh, xm, ym;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	th = get_graph_title_height(ptr);
	xw = get_graph_xaxbar_width(ptr);
	xh = get_graph_xaxbar_height(ptr);
	yw = get_graph_yaxbar_width(ptr);
	yh = get_graph_yaxbar_height(ptr);

	ym = th;
	xm = 0;

	if (!xlk && !ylk)
	{
		pxr->fx = xm;
		pxr->fw = yw;
		pxr->fy = ym;
		pxr->fh = xh;
		return 1;
	}

	if (ylk)
	{
		ym += xh;
		yax = get_next_yax(ptr, LINK_FIRST);
		while (yax)
		{
			if (yax == ylk)
				break;

			ym += yh;
			yax = get_next_yax(ptr, yax);
		}
		pxr->fy = ym;
		pxr->fh = yh;
	}
	else
	{
		pxr->fy = ym;
		pxr->fh = xh;
	}

	if (!xlk)
	{
		pxr->fx = xm;
		pxr->fw = yw;
		return 1;
	}

	calc_graph_xax_scope(pbox, ptr, page, &xlk_first, &xlk_last);

	xm += yw;

	xax = xlk_first;
	while (xax && xax != xlk)
	{
		xm += xw;

		xax = get_next_xax(ptr, xax);
		if (xax == xlk_last)
			break;
	}

	if (xax)
	{
		pxr->fx = xm;
		pxr->fw = xw;
		return 1;
	}

	xmem_zero((void*)pxr, sizeof(xrect_t));
	return 0;
}

void calc_graph_xax_rect(const canvbox_t* pbox, link_t_ptr ptr, int page, link_t_ptr xlk, xrect_t* pxr)
{
	calc_graph_coor_rect(pbox, ptr, page, xlk, NULL, pxr);

	pxr->fy = get_graph_title_height(ptr);
	pxr->fh = get_graph_xaxbar_height(ptr) + get_yax_count(ptr) * get_graph_yaxbar_height(ptr);
}

int calc_graph_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, int page, link_t_ptr* pxlk, link_t_ptr* pylk, link_t_ptr* pglk)
{
	link_t_ptr glk, ylk, xlk, firstxlk, lastxlk;
	float pw, ph, th, yw, yh, xw, xh, yt, gw, h, w, mx, my, by;
	int gn, hint;

	pw = pbox->fw;
	ph = pbox->fh;

	th = get_graph_title_height(ptr);
	xw = get_graph_xaxbar_width(ptr);
	xh = get_graph_xaxbar_height(ptr);
	yw = get_graph_yaxbar_width(ptr);
	yh = get_graph_yaxbar_height(ptr);

	yt = xh + get_yax_count(ptr) * yh;
	gn = get_gax_count(ptr);
	gw = (gn) ? (float)((yw - yh) / gn) : 0;

	hint = GRAPH_HINT_NONE;
	*pxlk = NULL;
	*pylk = NULL;
	*pglk = NULL;

	mx = ppt->fx;
	my = ppt->fy;

	if (mx < 0 && my < 0)
		return hint;

	if (mx < yw && my < th)
	{
		hint = GRAPH_HINT_MENU;
		return hint;
	}

	if (mx >= yw && my < th)
	{
		hint = GRAPH_HINT_TITLE;
		return hint;
	}

	if (mx < yw - DEF_SPLIT_FEED && my < th + xh - DEF_SPLIT_FEED)
	{
		hint = GRAPH_HINT_NULBAR;
		return hint;
	}

	by = (ph - th * 2 - yt) / 2 + (th + yt);

	if (mx > yw && my > by - DEF_SPLIT_FEED &&  my < by + DEF_SPLIT_FEED)
	{
		hint = GRAPH_HINT_MIDD_SPLIT;
		return hint;
	}

	h = th + yt;
	w = 0;
	glk = get_next_gax(ptr, LINK_FIRST);
	while (glk)
	{
		if (mx > w && mx < w + gw && my > h && my < h + yh)
		{
			*pglk = glk;
			hint = GRAPH_HINT_GAXBAR;
			return hint;
		}
		w += gw;
		glk = get_next_gax(ptr, glk);
	}

	h = th + xh;

	ylk = get_next_yax(ptr, LINK_FIRST);
	while (ylk)
	{
		if (my > h + DEF_SPLIT_FEED && my < h + yh - DEF_SPLIT_FEED)
		{
			*pylk = ylk;
			if (mx < yw - DEF_SPLIT_FEED)
			{
				hint = GRAPH_HINT_YAXBAR;
				return hint;
			}
		}
		else if (my >= h + yh - DEF_SPLIT_FEED && my <= h + yh + DEF_SPLIT_FEED)
		{
			*pylk = ylk;
			if (mx < yw - DEF_SPLIT_FEED)
			{
				hint = GRAPH_HINT_HORZ_SPLIT;
				return hint;
			}
		}

		h += yh;
		ylk = get_next_yax(ptr, ylk);
	}

	calc_graph_xax_scope(pbox, ptr, page, &firstxlk, &lastxlk);

	w = yw;

	xlk = firstxlk;
	while (xlk)
	{
		if (mx > w + DEF_SPLIT_FEED && mx < w + xw - DEF_SPLIT_FEED)
		{
			*pxlk = xlk;

			if (my < th + xh - DEF_SPLIT_FEED && my > th - DEF_SPLIT_FEED)
				hint = GRAPH_HINT_XAXBAR;
			else if (my < h)
				hint = GRAPH_HINT_COOR;

			break;
		}
		else if (mx >= w + xw - DEF_SPLIT_FEED && mx <= w + xw + DEF_SPLIT_FEED && my < th + xh - DEF_SPLIT_FEED && my > th - DEF_SPLIT_FEED)
		{
			*pxlk = xlk;
			hint = GRAPH_HINT_VERT_SPLIT;

			break;
		}

		w += xw;

		if (xlk == lastxlk)
			break;

		xlk = get_next_xax(ptr, xlk);
	}

	if (hint == GRAPH_HINT_NONE)
	{
		*pxlk = *pylk = NULL;
	}

	return hint;
}

void draw_graph_page(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr, int page)
{
	link_t_ptr xlk_first, xlk_last, ylk, xlk, xlk_pre, glk;
	float px, py, pw, ph;
	float th, yw, yh, xw, xh, yt, gw;
	float tmpx, midy, incy, cury, prey;
	float pred, nxtd, dby;
	float x1, x2, y1, y2;
	double middnum, stepnum;
	double coornum, total;
	int gn, i, rsteps, psteps, dalen, dadig, maxdig;
	bool_t b_design, b_print, b_sum;
	const tchar_t *gtype, *lcap, *style, *color;
	const tchar_t *xaxtype, *xaxfmt;
	bool_t zeronull, xaxwrp;

	tchar_t token[NUM_LEN + 1] = { 0 };
	xpen_t xp_shape, xp = { 0 };
	xbrush_t xb_shape, xb_bar, xb = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };
	xrect_t xr_bar, xr = { 0 };
	xpoint_t pt[2];

	b_design = graph_is_design(ptr);
	b_print = ((*pif->pf_canvas_type)(pif->canvas) == _CANV_PRINTER)? 1 : 0;

	default_xfont(&xf);
	default_xface(&xa);

	style = get_graph_style_ptr(ptr);

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	th = get_graph_title_height(ptr);
	xh = get_graph_xaxbar_height(ptr);
	xw = get_graph_xaxbar_width(ptr);
	yh = get_graph_yaxbar_height(ptr);
	yw = get_graph_yaxbar_width(ptr);

	yt = xh + yh * get_yax_count(ptr);
	gn = get_gax_count(ptr);
	gw = (gn) ? (float)((yw - yh)/ gn) : 0;

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
	memcpy((void*)&xb_bar, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_bar, DEF_SOFT_DARKEN);

	parse_xface_from_style(&xa, style);

	parse_xfont_from_style(&xf, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_txt, xf.color);
	}

	if (!b_print)
	{
		format_xcolor(&pif->clr_msk, xi.color);
	}

	b_sum = get_graph_showsum(ptr);
	xaxtype = get_graph_xaxdt_ptr(ptr);
	xaxfmt = get_graph_xaxfmt_ptr(ptr);
	xaxwrp = get_graph_xaxwrp(ptr);

	//draw title
	xr.fx = px;
	xr.fy = py;
	xr.fw = pw;
	xr.fh = th;

	xscpy(xa.text_align, ATTR_ALIGNMENT_NEAR);
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, get_graph_title_ptr(ptr), -1);

	//draw frame
	//top line
	pt[0].fx = px;
	pt[0].fy = th + py;
	pt[1].fx = pw + px;
	pt[1].fy = th + py;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

	//sum line
	pt[0].fx = px;
	pt[0].fy = th + yt + py;
	pt[1].fx = pw + px;
	pt[1].fy = th + yt + py;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

	//vert line
	pt[0].fx = yw + px;
	pt[0].fy = th + py;
	pt[1].fx = yw + px;
	pt[1].fy = ph - th + py;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

	//bottom line
	pt[0].fx = px;
	pt[0].fy = ph - th + py;
	pt[1].fx = pw + px;
	pt[1].fy = ph - th + py;
	(*pif->pf_draw_line)(pif->canvas, &xp, &pt[0], &pt[1]);

	if (ph <= th + yt)
		return;

	//draw yax bar
	xr.fy = th + xh + py;
	xr.fx = px;
	xr.fw = yw;
	xr.fh = yh;
	ylk = get_next_yax(ptr, LINK_FIRST);
	while (ylk)
	{
		lcap = get_yax_line_cap_ptr(ylk);

		color = get_yax_color_ptr(ylk);
		parse_xcolor(&xc, color);

		default_xpen(&xp_shape);
		format_xcolor(&xc, xp_shape.color);

		default_xbrush(&xb_shape);
		format_xcolor(&xc, xb_shape.color);

		xr_bar.fx = xr.fx;
		xr_bar.fy = xr.fy;
		xr_bar.fw = yh;
		xr_bar.fh = yh;

		ft_center_rect(&xr_bar, DEF_SMALL_ICON, DEF_SMALL_ICON);

		if (compare_text(lcap, -1, ATTR_LINE_CAP_RECT, -1, 0) == 0)
			(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_bar, ATTR_ICON_RECT);
		else if (compare_text(lcap, -1, ATTR_LINE_CAP_ELLIPSE, -1, 0) == 0)
			(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_bar, ATTR_ICON_ELLIPSE);
		else if (compare_text(lcap, -1, ATTR_LINE_CAP_CROSS, -1, 0) == 0)
			(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_bar, ATTR_ICON_CROSS);
		else if (compare_text(lcap, -1, ATTR_LINE_CAP_STAR, -1, 0) == 0)
			(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_bar, ATTR_ICON_STAR);
		else if (compare_text(lcap, -1, ATTR_LINE_CAP_DIAMOND, -1, 0) == 0)
			(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_bar, ATTR_ICON_DIAMOND);
		else
			(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_bar, ATTR_ICON_RECT);

		xr_bar.fx = xr.fx + yh;
		xr_bar.fy = xr.fy;
		xr_bar.fw = yw - yh;
		xr_bar.fh = yh;

		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_bar, get_yax_title_ptr(ylk), -1);

		xr.fy += yh;
		ylk = get_next_yax(ptr, ylk);
	}

	//draw sum icon
	if (b_sum)
	{
		xr.fx = yw - yh + px;
		xr.fy = th + yt + py;
		xr.fw = yh;
		xr.fh = yh;
		ft_center_rect(&xr, DEF_SMALL_ICON, DEF_SMALL_ICON);

		parse_xcolor(&xc, xf.color);
		(*pif->pf_draw_icon)(pif->canvas, &xc, &xr, ATTR_ICON_SUM);
	}

	//draw xax bar and coor
	calc_graph_xax_scope(pbox, ptr, page, &xlk_first, &xlk_last);

	x1 = x2 = yw + px;
	xlk = xlk_first;
	while (xlk)
	{
		x1 = x2;
		x2 += xw;

		xr.fx = x1;
		xr.fy = py + th;
		xr.fw = xw;
		xr.fh = xh;

		(*pif->pf_draw_rect)(pif->canvas, &xp, &xb_bar, &xr);

		xscpy(xa.text_align, ATTR_ALIGNMENT_CENTER);
		(*pif->pf_draw_data)(pif->canvas, &xf, &xa, &xr, get_xax_text_ptr(xlk), -1, 0, xaxtype, xaxfmt, 1, xaxwrp);

		maxdig = 0;

		y1 = y2 = th + xh + py;
		total = 0;
		ylk = get_next_yax(ptr, LINK_FIRST);
		while (ylk)
		{
			zeronull = get_yax_zeronull(ylk);
			dalen = get_yax_data_len(ylk);
			dadig = get_yax_data_dig(ylk);
			if (maxdig < dadig)
			{
				maxdig = dadig;
			}

			y1 = y2;
			y2 = y1 + yh;

			xr.fx = x1 + GRAPH_MINFEED;
			xr.fy = y1;
			xr.fw = x2 - x1 - 2 * GRAPH_MINFEED;
			xr.fh = y2 - y1;

			coornum = get_coor_numeric(xlk, ylk);

			if (b_sum)
			{
				total += coornum;
			}

			if (zeronull && is_zero_numeric(coornum, dadig))
			{
				xscpy(token, _T(""));
			}
			else
			{
				numtoxs_dig(coornum, dadig, token, NUM_LEN);
			}

			xscpy(xa.text_align, ATTR_ALIGNMENT_FAR);
			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, token, -1);

			ylk = get_next_yax(ptr, ylk);
		}

		//draw sum coor
		if (b_sum)
		{
			y1 = y2;
			y2 = y1 + yh;

			xr.fx = x1 + GRAPH_MINFEED;
			xr.fy = y1;
			xr.fw = x2 - x1 - 2 * GRAPH_MINFEED;
			xr.fh = y2 - y1;

			if (zeronull && is_zero_numeric(total, maxdig))
			{
				xscpy(token, _T(""));
			}
			else
			{
				numtoxs_dig(total, maxdig, token, NUM_LEN);
			}

			xscpy(xa.text_align, ATTR_ALIGNMENT_FAR);
			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, token, -1);
		}

		if (xlk == xlk_last)
			break;
		xlk = get_next_xax(ptr, xlk);
	}

	//calc base line
	//midy = (ph - th * 2 - yt - 3 * yh) / 2 + (th + yt + 3 * yh / 2) + py;
	midy = (ph + yt) / 2 + py;
	incy = (ph - th * 2 - yt - 3 * yh) / 10;

	//draw gaxbar
	xr.fx = px;
	xr.fy = th + yt + py;
	xr.fw = gw;
	xr.fh = yh;

	xscpy(xa.text_align, ATTR_ALIGNMENT_CENTER);

	glk = get_next_gax(ptr, LINK_FIRST);
	while (glk)
	{
		//gax title
		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, get_gax_title_ptr(glk), -1);

		gtype = get_gax_graph_type_ptr(glk);

		stepnum = get_gax_step(glk);
		middnum = get_gax_midd(glk);

		//gax ruler
		pt[0].fx = yw + px;
		pt[0].fy = midy;
		pt[1].fx = px + pw;
		pt[1].fy = midy;

		xr_bar.fx = xr.fx;
		xr_bar.fy = midy - yh / 2;
		xr_bar.fw = xr.fw;
		xr_bar.fh = yh;

		numtoxs_dig(middnum, 1, token, NUM_LEN);

		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_bar, token, -1);

		//midd line
		if (compare_text(gtype, -1, ATTR_GRAPH_TYPE_PIE, -1, 0) != 0)
		{
			memcpy((void*)&xp_shape, (void*)&xp, sizeof(xpen_t));
			xscpy(xp_shape.style, GDI_ATTR_STROKE_STYLE_DASHED);

			pt[0].fx = yw + px;
			pt[0].fy = midy;
			pt[1].fx = px + pw;
			pt[1].fy = midy;
			(*pif->pf_draw_line)(pif->canvas, &xp_shape, &pt[0], &pt[1]);
		}

		for (i = 1; i <= 5; i++)
		{
			xr_bar.fx = xr.fx;
			xr_bar.fy = midy - incy * i - yh / 2;
			xr_bar.fw = xr.fw;
			xr_bar.fh = yh;

			numtoxs_dig(middnum + i * 10 * stepnum, 1, token, NUM_LEN);

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_bar, token, -1);

			pt[0].fx = yw - 2 * GRAPH_MINFEED + px;
			pt[0].fy = midy - incy * i;
			pt[1].fx = yw + px;
			pt[1].fy = midy - incy * i;;
			(*pif->pf_draw_line)(pif->canvas, &xp_shape, &pt[0], &pt[1]);

			xr_bar.fx = xr.fx;
			xr_bar.fy = midy + incy * i - yh / 2;
			xr_bar.fw = xr.fw;
			xr_bar.fh = yh;

			numtoxs_dig(middnum - i * 10 * stepnum, 1, token, NUM_LEN);

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_bar, token, -1);

			pt[0].fx = yw - 2 * GRAPH_MINFEED + px;
			pt[0].fy = midy + incy * i;
			pt[1].fx = yw + px;
			pt[1].fy = midy + incy * i;;
			(*pif->pf_draw_line)(pif->canvas, &xp_shape, &pt[0], &pt[1]);
		}

		xr.fx += gw;
		glk = get_next_gax(ptr, glk);
	}

	//draw graph
	x1 = x2 = yw + px;
	xlk = xlk_first;
	while (xlk)
	{
		x1 = x2;
		x2 += xw;

		rsteps = get_yax_count(ptr);
		tmpx = (rsteps)? ((xw - 2 * GRAPH_MINFEED) / rsteps) : 0;

		total = 0;
		ylk = get_next_yax(ptr, LINK_FIRST);
		while (ylk)
		{
			glk = get_yax_gax(ptr, ylk);

			if (glk && compare_text(get_gax_graph_type_ptr(glk), -1, ATTR_GRAPH_TYPE_PIE, -1, 0) == 0)
			{
				coornum = get_coor_numeric(xlk, ylk);

				total += coornum;
			}

			ylk = get_next_yax(ptr, ylk);
		}

		rsteps = psteps = 0;
		nxtd = pred = 0;

		ylk = get_next_yax(ptr, LINK_FIRST);
		while (ylk)
		{
			glk = get_yax_gax(ptr, ylk);
			if (!glk)
			{
				ylk = get_next_yax(ptr, ylk);
				continue;
			}

			gtype = get_gax_graph_type_ptr(glk);
			stepnum = get_gax_step(glk);
			middnum = get_gax_midd(glk);

			color = get_yax_color_ptr(ylk);
			parse_xcolor(&xc, color);

			default_xbrush(&xb_shape);
			format_xcolor(&xc, xb_shape.color);

			default_xpen(&xp_shape);
			format_xcolor(&xc, xp_shape.color);

			lcap = get_yax_line_cap_ptr(ylk);

			coornum = get_coor_numeric(xlk, ylk);

			if (compare_text(gtype, -1, ATTR_GRAPH_TYPE_PIE, -1, 0) == 0)
				cury = th + yt + yh + xw + py;
			else
				cury = midy - (long)(((coornum - (float)middnum) / stepnum * (float)incy / 10));

			if (compare_text(gtype, -1, ATTR_GRAPH_TYPE_LINE, -1, 0) == 0)
			{
				xr.fx = (x1 + x2) / 2 - DEF_SMALL_ICON / 2;
				xr.fy = cury - DEF_SMALL_ICON / 2;
				xr.fw = DEF_SMALL_ICON;
				xr.fh = DEF_SMALL_ICON;

				if (compare_text(lcap, -1, ATTR_LINE_CAP_RECT, -1, 0) == 0)
					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr, ATTR_ICON_RECT);
				else if (compare_text(lcap, -1, ATTR_LINE_CAP_ELLIPSE, -1, 0) == 0)
					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr, ATTR_ICON_ELLIPSE);
				else if (compare_text(lcap, -1, ATTR_LINE_CAP_CROSS, -1, 0) == 0)
					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr, ATTR_ICON_CROSS);
				else if (compare_text(lcap, -1, ATTR_LINE_CAP_STAR, -1, 0) == 0)
					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr, ATTR_ICON_STAR);
				else if (compare_text(lcap, -1, ATTR_LINE_CAP_DIAMOND, -1, 0) == 0)
					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr, ATTR_ICON_DIAMOND);
				else
					(*pif->pf_draw_icon)(pif->canvas, &xc, &xr, ATTR_ICON_ELLIPSE);

				if (xlk != xlk_first)
				{
					xlk_pre = get_prev_xax(ptr, xlk);
					prey = midy - (long)(((get_coor_numeric(xlk_pre, ylk) - (float)middnum) / stepnum * (float)incy / 10));

					pt[0].fx = (x1 + x2) / 2 - xw;
					pt[0].fy = prey;
					pt[1].fx = (x1 + x2) / 2;
					pt[1].fy = cury;
					(*pif->pf_draw_line)(pif->canvas, &xp_shape, &pt[0], &pt[1]);
				}
			}
			else if (compare_text(gtype, -1, ATTR_GRAPH_TYPE_RECT, -1, 0) == 0)
			{
				//xr.fy = cury;
				//xr.fh = ph - th - (cury - py);
				xr.fx = x1 + rsteps * tmpx + GRAPH_MINFEED;
				xr.fw = tmpx;
				if (cury > midy)
				{
					xr.fy = midy;
					xr.fh = cury - midy;
				}
				else
				{
					xr.fy = cury;
					xr.fh = midy - cury;
				}

				if ((long)cury == (long)midy)
				{
					pt[0].fx = xr.fx;
					pt[0].fy = cury;
					pt[1].fx = xr.fx + xr.fw;
					pt[1].fy = cury;
					(*pif->pf_draw_line)(pif->canvas, &xp_shape, &pt[0], &pt[1]);
				}else
				{
					(*pif->pf_draw_rect)(pif->canvas, &xp_shape, &xb_shape, &xr);
				}

				rsteps++;
			}
			else if (compare_text(gtype, -1, ATTR_GRAPH_TYPE_PIE, -1, 0) == 0)
			{
				if (ylk == get_prev_yax(ptr, LINK_LAST))
				{
					xr.fx = x1 + GRAPH_MINFEED;
					xr.fw = xw - 2 * GRAPH_MINFEED;
					xr.fy = cury - xw + GRAPH_MINFEED;
					xr.fh = xw - 2 * GRAPH_MINFEED;

					(*pif->pf_draw_ellipse)(pif->canvas, &xp, NULL, &xr);
				}

				pred = nxtd;

				if ((int)total == 0)
					dby = 0;
				else
					dby = (float)(coornum / total);

				nxtd = pred + (float)(2 * XPI *  dby);
				if (pred != nxtd)
				{
					xr.fx = x1 + GRAPH_MINFEED;
					xr.fw = xw - 2 * GRAPH_MINFEED;
					xr.fy = cury - xw + GRAPH_MINFEED;
					xr.fh = xw - 2 * GRAPH_MINFEED;

					(*pif->pf_draw_pie)(pif->canvas, &xp_shape, &xb_shape, &xr, pred, nxtd);
				}

				if (ylk == get_prev_yax(ptr, LINK_LAST))
				{
					xr.fx = x1 + xw / 4;
					xr.fw = xw / 2;
					xr.fy = cury - xw * 3 / 4;
					xr.fh = xw / 2;

					(*pif->pf_draw_ellipse)(pif->canvas, &xp, &xb, &xr);
				}

				xr.fx = x1 + GRAPH_MINFEED;
				xr.fw = 2 * GRAPH_MINFEED;
				xr.fy = cury + psteps * yh;
				xr.fh = yh;
				(*pif->pf_draw_rect)(pif->canvas, NULL, &xb_shape, &xr);

				xr.fx = x1 + 2 * GRAPH_MINFEED;
				xr.fw = xw - 2 * GRAPH_MINFEED;
				xr.fy = cury + psteps * yh;
				xr.fh = yh;
				xsprintf(token, _T("%.2f%c"), dby * 100, _T('%'));
				xscpy(xa.text_align, ATTR_ALIGNMENT_CENTER);
				(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, token, -1);

				psteps++;
			}

			ylk = get_next_yax(ptr, ylk);
		}

		if (xlk == xlk_last)
			break;
		xlk = get_next_xax(ptr, xlk);
	}
}

#endif //XDLVIEW