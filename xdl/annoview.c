/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc anno document

	@module	annoview.c | implement file

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

#include "annoview.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_VIEW

#define DEF_ANNO_SPAN		6

void calc_anno_arti_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	const tchar_t* type;
	xpoint_t* ppt;
	int n;

	n = get_anno_arti_xpoint(ilk, NULL, MAX_LONG);
	ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * n);
	get_anno_arti_xpoint(ilk, ppt, n);

	type = get_anno_arti_type_ptr(ilk);

	if (compare_text(type, -1, ATTR_ANNO_TYPE_TEXT, -1, 0) == 0)
	{
		pxr->fx = ppt[0].fx;
		pxr->fy = ppt[0].fy;
		pxr->fw = ppt[1].fx - ppt[0].fx;
		pxr->fh = ppt[1].fy - ppt[0].fy;
	}
	else if (compare_text(type, -1, ATTR_ANNO_TYPE_ANCHOR, -1, 0) == 0)
	{
		pxr->fx = ppt[0].fx - 1;
		pxr->fy = ppt[0].fy - DEF_ANNO_SPAN;
		pxr->fw = 2;
		pxr->fh = DEF_ANNO_SPAN;
	}
	else if (compare_text(type, -1, ATTR_ANNO_TYPE_CIRCLE, -1, 0) == 0)
	{
		pxr->fx = ppt[0].fx;
		pxr->fy = ppt[0].fy;
		pxr->fw = ppt[1].fx - ppt[0].fx;
		pxr->fh = ppt[1].fy - ppt[0].fy;
	}
	else if (compare_text(type, -1, ATTR_ANNO_TYPE_RANGE, -1, 0) == 0)
	{
		pxr->fx = (ppt[0].fx < ppt[1].fx) ? ppt[0].fx : ppt[1].fx;
		pxr->fy = (ppt[0].fy < ppt[1].fy) ? ppt[0].fy : ppt[1].fy;
		pxr->fw = (ppt[0].fx < ppt[1].fx) ? ppt[1].fx - ppt[0].fx : ppt[0].fx - ppt[1].fx;
		pxr->fh = (ppt[0].fy < ppt[1].fy) ? ppt[1].fy - ppt[0].fy : ppt[0].fy - ppt[1].fy;

		if (pxr->fw < 2)
		{
			pxr->fx -= 1;
			pxr->fw = 2;
		}
		if (pxr->fh < 2)
		{
			pxr->fy -= 1;
			pxr->fh = 2;
		}
	}
	else if (compare_text(type, -1, ATTR_ANNO_TYPE_ANGLE, -1, 0) == 0)
	{
		pxr->fx = pxr->fw = ppt[0].fx;
		pxr->fy = pxr->fh = ppt[0].fy;

		if (pxr->fx > ppt[1].fx)
			pxr->fx = ppt[1].fx;
		if (pxr->fw < ppt[1].fx)
			pxr->fw = ppt[1].fx;

		if (pxr->fx > ppt[2].fx)
			pxr->fx = ppt[2].fx;
		if (pxr->fw < ppt[2].fx)
			pxr->fw = ppt[2].fx;

		if (pxr->fy > ppt[1].fy)
			pxr->fy = ppt[1].fy;
		if (pxr->fh < ppt[1].fy)
			pxr->fh = ppt[1].fy;

		if (pxr->fy > ppt[2].fy)
			pxr->fy = ppt[2].fy;
		if (pxr->fh < ppt[2].fy)
			pxr->fh = ppt[2].fy;

		pxr->fw -= pxr->fx;
		pxr->fh -= pxr->fy;
	}

	xmem_free(ppt);
}

int calc_anno_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pplk, int* pind)
{
	link_t_ptr ilk;
	xrect_t xr;
	xpoint_t* lpt;
	int j, count;

	*pplk = NULL;
	*pind = -1;

	ilk = get_anno_next_arti(ptr, LINK_FIRST);
	while (ilk)
	{
		calc_anno_arti_rect(pbox, ptr, ilk, &xr);

		if (ft_in_rect(ppt, &xr))
		{
			*pplk = ilk;

			count = get_anno_arti_xpoint(ilk, NULL, MAX_LONG);
			lpt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * count);
			get_anno_arti_xpoint(ilk, lpt, count);

			for (j = 0; j < count; j++)
			{
				if (ft_inside(ppt->fx, ppt->fy, lpt[j].fx - (float)1.5, lpt[j].fy - (float)1.5, lpt[j].fx + (float)1.5, lpt[j].fy + (float)1.5))
				{
					*pind = j;
					xmem_free(lpt);
					return ANNO_HINT_SIZE;
				}
			}

			xmem_free(lpt);
			return ANNO_HINT_ARTI;
		}

		ilk = get_anno_next_arti(ptr, ilk);
	}

	return ANNO_HINT_NONE;
}

void draw_anno(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr ilk;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr;
	xpoint_t pt1, pt2;
	xpoint_t* ppt;
	int i, n;
	double a1, a2;

	const tchar_t *style;
	const tchar_t* type;
	tchar_t token[RES_LEN];

	ilk = get_anno_next_arti(ptr, LINK_FIRST);
	while (ilk)
	{
		type = get_anno_arti_type_ptr(ilk);
		style = get_anno_arti_style_ptr(ilk);

		n = get_anno_arti_xpoint(ilk, NULL, MAX_LONG);
		ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)*n);
		get_anno_arti_xpoint(ilk, ppt, n);

		for (i = 0; i < n; i++)
		{
			ppt[i].fx += pbox->fx;
			ppt[i].fy += pbox->fy;
		}

		if (compare_text(type, -1, ATTR_ANNO_TYPE_TEXT, -1, 0) == 0)
		{
			default_xface(&xa);
			default_xfont(&xf);
			parse_xfont_from_style(&xf, style);
			parse_xface_from_style(&xa, style);

			xr.fx = ppt[0].fx;
			xr.fy = ppt[0].fy;
			xr.fw = ppt[1].fx - ppt[0].fx;
			xr.fh = ppt[1].fy - ppt[0].fy;

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, get_anno_arti_text_ptr(ilk), -1);
		}
		else if (compare_text(type, -1, ATTR_ANNO_TYPE_ICON, -1, 0) == 0)
		{
			default_xfont(&xf);
			parse_xfont_from_style(&xf, style);
			parse_xcolor(&xc, xf.color);

			xr.fx = ppt[0].fx;
			xr.fy = ppt[0].fy;
			xr.fw = ppt[1].fx - ppt[0].fx;
			xr.fh = ppt[1].fy - ppt[0].fy;

			(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr, get_anno_arti_text_ptr(ilk));
		}
		else if (compare_text(type, -1, ATTR_ANNO_TYPE_CIRCLE, -1, 0) == 0)
		{
			default_xpen(&xp);
			parse_xpen_from_style(&xp, style);

			xr.fx = ppt[0].fx;
			xr.fy = ppt[0].fy;
			xr.fw = ppt[1].fx - ppt[0].fx;
			xr.fh = ppt[1].fy - ppt[0].fy;

			(*pif->pf_draw_ellipse)(pif->canvas, &xp, NULL, &xr);
		}
		else if (compare_text(type, -1, ATTR_ANNO_TYPE_ANCHOR, -1, 0) == 0)
		{
			default_xpen(&xp);
			parse_xpen_from_style(&xp, style);
			default_xbrush(&xb);
			xscpy(xb.color, xp.color);

			xr.fx = ppt[0].fx - 1;
			xr.fy = ppt[0].fy - DEF_ANNO_SPAN;
			xr.fw = 2;
			xr.fh = 2;
			(*pif->pf_draw_ellipse)(pif->canvas, &xp, &xb, &xr);

			pt1.fx = ppt[0].fx;
			pt1.fy = ppt[0].fy;
			pt2.fx = ppt[0].fx;
			pt2.fy = ppt[0].fy - DEF_ANNO_SPAN + 2;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);
		}
		else if (compare_text(type, -1, ATTR_ANNO_TYPE_RANGE, -1, 0) == 0)
		{
			default_xpen(&xp);
			parse_xpen_from_style(&xp, style);
			default_xbrush(&xb);
			xscpy(xb.color, xp.color);

			xr.fx = ppt[0].fx - (float)0.5;
			xr.fy = ppt[0].fy - (float)0.5;
			xr.fw = 1;
			xr.fh = 1;
			(*pif->pf_draw_ellipse)(pif->canvas, &xp, &xb, &xr);

			xr.fx = ppt[1].fx - (float)0.5;
			xr.fy = ppt[1].fy - (float)0.5;
			xr.fw = 1;
			xr.fh = 1;
			(*pif->pf_draw_ellipse)(pif->canvas, &xp, &xb, &xr);

			xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

			pt1.fx = ppt[0].fx;
			pt1.fy = ppt[0].fy;
			pt2.fx = ppt[1].fx;
			pt2.fy = ppt[1].fy;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

			default_xfont(&xf);
			parse_xfont_from_style(&xf, style);
			xscpy(xf.style, GDI_ATTR_FONT_STYLE_ITALIC);

			a1 = (ppt[1].fx - ppt[0].fx);// *pdt->dblX;
			a2 = (ppt[1].fy - ppt[0].fy);// *pdt->dblY;

			calc_anno_arti_rect(pbox, ptr, ilk, &xr);
			ft_offset_rect(&xr, pbox->fx, pbox->fy);

			//pt1.fx = xr.fx + xr.fw / 2;
			//pt1.fy = xr.fy + xr.fh / 2 - 5;
			//xsprintf(token, _T("W: %.1fmm"), a1);
			//(*pif->pf_text_out)(pif->canvas, &xf, &pt1, token, -1);

			//pt1.fx = xr.fx + xr.fw / 2;
			//pt1.fy = xr.fy + xr.fh / 2;
			//xsprintf(token, _T("H: %.1fmm"), a2);
			//(*pif->pf_text_out)(pif->canvas, &xf, &pt1, token, -1);

			a1 = sqrt(a1 * a1 + a2 * a2);
			xsprintf(token, _T("%.1fmm"), a1);
			pt1.fx = xr.fx + xr.fw / 2;
			pt1.fy = xr.fy + xr.fh / 2 - 10;
			(*pif->pf_text_out)(pif->canvas, &xf, &pt1, token, -1);
		}
		else if (compare_text(type, -1, ATTR_ANNO_TYPE_ANGLE, -1, 0) == 0)
		{
			default_xpen(&xp);
			parse_xpen_from_style(&xp, style);
			xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);
			default_xbrush(&xb);
			xscpy(xb.color, xp.color);

			pt1.fx = ppt[0].fx;
			pt1.fy = ppt[0].fy;
			pt2.fx = ppt[1].fx;
			pt2.fy = ppt[1].fy;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

			pt1.fx = ppt[0].fx;
			pt1.fy = ppt[0].fy;
			pt2.fx = ppt[2].fx;
			pt2.fy = ppt[2].fy;
			(*pif->pf_draw_line)(pif->canvas, &xp, &pt1, &pt2);

			xr.fx = ppt[1].fx - (float)0.5;
			xr.fy = ppt[1].fy - (float)0.5;
			xr.fw = 1;
			xr.fh = 1;
			(*pif->pf_draw_ellipse)(pif->canvas, &xp, &xb, &xr);

			xr.fx = ppt[2].fx - (float)0.5;
			xr.fy = ppt[2].fy - (float)0.5;
			xr.fw = 1;
			xr.fh = 1;
			(*pif->pf_draw_ellipse)(pif->canvas, &xp, &xb, &xr);

			xr.fx = ppt[0].fx - 2;
			xr.fy = ppt[0].fy - 2;
			xr.fw = 4;
			xr.fh = 4;

			a1 = atan((double)(ppt[0].fy - ppt[1].fy) / (double)(ppt[1].fx - ppt[0].fx));
			a2 = atan((double)(ppt[0].fy - ppt[2].fy) / (double)(ppt[2].fx - ppt[0].fx));

			if (ppt[0].fy > ppt[1].fy && ppt[1].fx < ppt[0].fx)
				a1 = XPI + a1;
			if (ppt[0].fy < ppt[1].fy && ppt[1].fx < ppt[0].fx)
				a1 = -XPI + a1;

			if (ppt[0].fy > ppt[2].fy && ppt[2].fx < ppt[0].fx)
				a2 = XPI + a2;
			if (ppt[0].fy < ppt[2].fy && ppt[2].fx < ppt[0].fx)
				a2 = -XPI + a2;

			(*pif->pf_draw_arc)(pif->canvas, &xp, &ppt[0], 2, 2, a1, a2);

			default_xfont(&xf);
			parse_xfont_from_style(&xf, style);
			xscpy(xf.style, GDI_ATTR_FONT_STYLE_ITALIC);

			a1 = (a2 - a1) / XPI * 180;
			if (a1 < 0)
				a1 = 0 - a1;
			xsprintf(token, _T("%.1f°"), a1);

			calc_anno_arti_rect(pbox, ptr, ilk, &xr);
			ft_offset_rect(&xr, pbox->fx, pbox->fy);

			pt1.fx = xr.fx + xr.fw / 2;
			pt1.fy = xr.fy + xr.fh / 2 - 3;
			(*pif->pf_text_out)(pif->canvas, &xf, &pt1, token, -1);
		}
	
		xmem_free(ppt);

		ilk = get_anno_next_arti(ptr, ilk);
	}
}


#endif //XDLVIEW
