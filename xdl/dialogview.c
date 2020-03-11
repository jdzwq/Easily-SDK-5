/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dialog document

	@module	dialogview.c | implement file

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
#include "dialogview.h"
#include "xdlview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

#define DIALOG_TITLE_HEIGHT		(float)10

void calc_dialog_item_rect(link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	pxr->fx = get_dialog_item_x(ilk);
	pxr->fy = get_dialog_item_y(ilk);
	pxr->fw = get_dialog_item_width(ilk);
	pxr->fh = get_dialog_item_height(ilk);
}

int calc_dialog_hint(link_t_ptr ptr, const xpoint_t* ppt, link_t_ptr* pilk)
{
	link_t_ptr ilk;
	int nHit;
	float xm, ym;
	xrect_t di;

	xm = ppt->fx;
	ym = ppt->fy;

	nHit = DIALOG_HINT_NONE;
	*pilk = NULL;

	ilk = get_dialog_prev_item(ptr, LINK_LAST);
	while (ilk)
	{
		calc_dialog_item_rect(ptr, ilk, &di);

		if (ft_inside(xm, ym, di.fx, di.fy, di.fx + di.fw, di.fy + di.fh))
		{
			*pilk = ilk;

			if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh / 2 - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh / 2 + DEF_SPLIT_FEED))
			{
				nHit = DIALOG_HINT_VERT_SPLIT;
				break;
			}
			else if (ft_inside(xm, ym, di.fx + di.fw / 2 - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw / 2 + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
			{
				nHit = DIALOG_HINT_HORZ_SPLIT;
				break;
			}
			else if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
			{
				nHit = DIALOG_HINT_CROSS_SPLIT;
				break;
			}

			nHit = DIALOG_HINT_ITEM;
			break;
		}
		ilk = get_dialog_prev_item(ptr, ilk);
	}

	return nHit;
}

void draw_dialog(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr obj,ilk;
	xrect_t xr;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	canvbox_t cb = { 0 };

	const tchar_t* style;
	bool_t b_print;

	xdate_t dt;

	b_print = (pif->canvas->tag == _CANVAS_PRINTER) ? 1 : 0;

	default_xfont(&xf);
	default_xface(&xa);
	default_xpen(&xp);
	default_xbrush(&xb);

	style = get_dialog_style_ptr(ptr);

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

	xr.fx = pbox->fx;
	xr.fy = pbox->fy - DIALOG_TITLE_HEIGHT;
	xr.fw = pbox->fw;
	xr.fh = DIALOG_TITLE_HEIGHT;
	(*pif->pf_draw_shape)(pif->canvas, &xp, &xb, &xr, ATTR_SHAPE_RECT);

	xr.fx = pbox->fx;
	xr.fy = pbox->fy - DIALOG_TITLE_HEIGHT;
	xr.fw = DIALOG_TITLE_HEIGHT;
	xr.fh = DIALOG_TITLE_HEIGHT;

	parse_xcolor(&xc, xp.color);
	ft_center_rect(&xr, DEF_SMALL_ICON, DEF_SMALL_ICON);
	(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr, GDI_ICON_LOGO);

	xr.fx = pbox->fx + DIALOG_TITLE_HEIGHT;
	xr.fy = pbox->fy - DIALOG_TITLE_HEIGHT;
	xr.fw = pbox->fw - DIALOG_TITLE_HEIGHT;
	xr.fh = DIALOG_TITLE_HEIGHT;
	xscpy(xa.text_wrap, _T(""));
	(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, get_dialog_title_ptr(ptr), -1);

	xr.fx = pbox->fx + pbox->fw - 2 * DEF_SMALL_ICON;
	xr.fy = pbox->fy - DIALOG_TITLE_HEIGHT + DEF_SMALL_ICON;
	xr.fw = DEF_SMALL_ICON;
	xr.fh = DEF_SMALL_ICON;

	parse_xcolor(&xc, xp.color);
	ft_center_rect(&xr, DEF_SMALL_ICON, DEF_SMALL_ICON);
	(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr, GDI_ICON_CLOSE);

	xr.fx = pbox->fx;
	xr.fy = pbox->fy;
	xr.fw = pbox->fw;
	xr.fh = pbox->fh;

	(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr, ATTR_SHAPE_RECT);

	ilk = get_dialog_next_item(ptr, LINK_FIRST);
	while (ilk)
	{
		default_xfont(&xf);
		default_xface(&xa);
		
		style = get_dialog_item_style_ptr(ilk);

		calc_dialog_item_rect(ptr, ilk, &xr);
		ft_offset_rect(&xr, pbox->fx, pbox->fy);

		//(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr, ATTR_SHAPE_RECT);

		parse_xfont_from_style(&xf, style);
		parse_xface_from_style(&xa, style);
		if (!b_print)
		{
			format_xcolor(&pif->clr_txt, xf.color);
		}

		if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_SHAPEBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr, get_dialog_item_text_ptr(ilk));
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_STATICBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, get_dialog_item_text_ptr(ilk), -1);
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_EDITBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, get_dialog_item_text_ptr(ilk), -1);
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_PUSHBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			draw_pushbox(pif, &cb, &xf, get_dialog_item_text_ptr(ilk));
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_RADIOBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			draw_radiobox(pif, &cb, &xf, (bool_t)xstol(get_dialog_item_text_ptr(ilk)));
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_CHECKBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			draw_checkbox(pif, &cb, &xf, (bool_t)xstol(get_dialog_item_text_ptr(ilk)));
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_DATEBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			parse_date(&dt, get_dialog_item_text_ptr(ilk));
			draw_datebox(pif, &cb, &xf, &dt);
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_TIMEBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			parse_datetime(&dt, get_dialog_item_text_ptr(ilk));
			draw_timebox(pif, &cb, &xf, &dt);
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_LISTBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			obj = create_string_table(0);
			string_table_parse_options(obj, get_dialog_item_text_ptr(ilk), -1, OPT_ITEMFEED, OPT_LINEFEED);
			draw_listbox(pif, &cb, &xf, obj);
			destroy_string_table(obj);
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_SLIDEBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			draw_slidebox(pif, &cb, &xf, xstol(get_dialog_item_text_ptr(ilk)));
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_SPINBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			draw_spinbox(pif, &cb, &xf, xstol(get_dialog_item_text_ptr(ilk)));
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_NAVIBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			draw_navibox(pif, &cb, &xf, NULL);
		}
		else if (compare_text(get_dialog_item_class_ptr(ilk), -1, DOC_DIALOG_USERBOX, -1, 1) == 0)
		{
			cb.fx = xr.fx;
			cb.fy = xr.fy;
			cb.fw = xr.fw;
			cb.fh = xr.fh;

			(*pif->pf_draw_rect)(pif->canvas, &xp, &xb, &xr);
			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, get_dialog_item_text_ptr(ilk), -1);
		}

		ilk = get_dialog_next_item(ptr, ilk);
	}
}

#endif //XDLVIEW
