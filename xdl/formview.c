/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc form document

	@module	formview.c | form document implement file

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
#include "formview.h"
#include "xdlview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

void calc_form_field_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr flk, xrect_t* pxr)
{
	pxr->fx = get_field_x(flk);
	pxr->fy = get_field_y(flk);
	pxr->fw = get_field_width(flk);
	pxr->fh = get_field_height(flk);
}

void calc_form_group_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr alk, xrect_t* pxr)
{
	link_t_ptr flk;
	xrect_t di;
	float x, y, w, h;
	int gid;

	gid = get_field_group(alk);

	calc_form_field_rect(pbox, ptr, alk, pxr);

	flk = get_next_field(ptr, LINK_FIRST);
	while (flk)
	{
		if ((!gid && get_field_selected(flk)) || (gid && gid == get_field_group(flk)))
		{
			calc_form_field_rect(pbox, ptr, flk, &di);

			x = pxr->fx;
			y = pxr->fy;
			w = pxr->fw;
			h = pxr->fh;

			if (x > di.fx)
			{
				pxr->fw += (x - di.fx);
				pxr->fx = di.fx;
			}
			if (y > di.fy)
			{
				pxr->fh += (y - di.fy);
				pxr->fy = di.fy;
			}
			if (x + w < di.fx + di.fw)
				pxr->fw = di.fx + di.fw - pxr->fx;
			if (y + h < di.fy + di.fh)
				pxr->fh = di.fy + di.fh - pxr->fy;
		}

		flk = get_next_field(ptr, flk);
	}
}

int calc_form_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pflk)
{
	link_t_ptr flk;
	int nHit;
	float xm, ym;
	xrect_t di;
	bool_t b_design;

	xm = ppt->fx;
	ym = ppt->fy;

	nHit = FORM_HINT_NONE;
	*pflk = NULL;

	b_design = form_is_design(ptr);

	flk = get_prev_field(ptr, LINK_LAST);
	while (flk)
	{
		calc_form_field_rect(pbox, ptr, flk, &di);

		if (ft_inside(xm, ym, di.fx, di.fy, di.fx + di.fw, di.fy + di.fh))
		{
			nHit = FORM_HINT_FIELD;
			*pflk = flk;

			if (b_design)
			{
				if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh / 2 - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh / 2 + DEF_SPLIT_FEED))
				{
					nHit = FORM_HINT_VERT_SPLIT;
					break;
				}
				else if (ft_inside(xm, ym, di.fx + di.fw / 2 - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw / 2 + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
				{
					nHit = FORM_HINT_HORZ_SPLIT;
					break;
				}
				else if (ft_inside(xm, ym, di.fx + di.fw - DEF_SPLIT_FEED, di.fy + di.fh - DEF_SPLIT_FEED, di.fx + di.fw + DEF_SPLIT_FEED, di.fy + di.fh + DEF_SPLIT_FEED))
				{
					nHit = FORM_HINT_CROSS_SPLIT;
					break;
				}
			}

			break;
		}
		flk = get_prev_field(ptr, flk);
	}

	return nHit;
}

void draw_form_page(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr, int page)
{
	link_t_ptr flk,obj;
	xrect_t xr;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };
	canvbox_t cb;

	bool_t b_design, b_print;
	const tchar_t *sz_class,*sz_text, *sz_shape, *style, *type, *fldfmt;
	bool_t zeronull, wrapable;

	link_t_ptr imagelist;
	tchar_t sz_token[PATH_LEN];

	b_design = form_is_design(ptr);
	b_print = ((*pif->pf_canvas_type)(pif->canvas) == _CANV_PRINTER)? 1 : 0;

	imagelist = get_form_images(ptr);

	if (b_design)
		flk = get_next_field(ptr, LINK_FIRST);
	else
		flk = get_next_visible_field(ptr, LINK_FIRST);

	while (flk)
	{
		if (b_print && !get_field_printable(flk))
		{
			goto skip;
		}

		default_xfont(&xf);
		default_xface(&xa);
		memset((void*)&xi, 0, sizeof(ximage_t));

		default_xpen(&xp);

		style = get_field_style_ptr(flk);

		calc_form_field_rect(pbox, ptr, flk, &xr);
		ft_offset_rect(&xr, pbox->fx, pbox->fy);

		sz_shape = get_field_shape_ptr(flk);
		if (!is_null(sz_shape))
		{
			parse_xpen_from_style(&xp, style);
			if (!b_print)
			{
				format_xcolor(&pif->clr_frg, xp.color);
			}

			(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr, sz_shape);
		}
		else if (b_design)
		{
			parse_xpen_from_style(&xp, style);
			xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);
			if (!b_print)
			{
				format_xcolor(&pif->clr_frg, xp.color);
			}

			(*pif->pf_draw_shape)(pif->canvas, &xp, NULL, &xr, ATTR_SHAPE_RECT);
		}

		sz_class = get_field_class_ptr(flk);

		if (compare_text(sz_class, -1, DOC_FORM_PAGENUM, -1, 0) == 0)
		{
			xsprintf(sz_token, PAGENUM_GUID, page);
			parse_xfont_from_style(&xf, style);
			parse_xface_from_style(&xa, style);
			if (!b_print)
			{
				format_xcolor(&pif->clr_txt, xf.color);
			}

			(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz_token, -1);
		}
		else if (compare_text(sz_class, -1, DOC_FORM_LABEL, -1, 0) == 0)
		{
			if (!b_print)
			{
				format_xcolor(&pif->clr_msk, xi.color);
			}
			else
			{
				xscpy(xi.color, _T(""));
			}

			if (compare_text(get_field_codebar_ptr(flk),-1,ATTR_CODEBAR_IMAGE,-1,0) == 0)
			{
				if (imagelist)
				{
					get_ximage(imagelist, get_field_text_ptr(flk), &xi);

					(*pif->pf_draw_image)(pif->canvas, &xi, &xr);
				}
			}
			else if (compare_text(get_field_codebar_ptr(flk), -1, ATTR_CODEBAR_CODE128, -1, 0) == 0)
			{
				xscpy(xi.color, _T(""));
				
				xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_CODE128);
				xi.source = get_field_text_ptr(flk);

				(*pif->pf_draw_image)(pif->canvas, &xi, &xr);
			}
			else if (compare_text(get_field_codebar_ptr(flk), -1, ATTR_CODEBAR_PDF417, -1, 0) == 0)
			{
				xscpy(xi.color, _T(""));

				xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_PDF417);
				xi.source = get_field_text_ptr(flk);

				(*pif->pf_draw_image)(pif->canvas, &xi, &xr);
			}
			else if (compare_text(get_field_codebar_ptr(flk), -1, ATTR_CODEBAR_QRCODE, -1, 0) == 0)
			{
				xscpy(xi.color, _T(""));

				xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_QRCODE);
				xi.source = get_field_text_ptr(flk);

				(*pif->pf_draw_image)(pif->canvas, &xi, &xr);
			}
			else
			{
				parse_xfont_from_style(&xf, style);
				parse_xface_from_style(&xa, style);
				if (!b_print)
				{
					format_xcolor(&pif->clr_txt, xf.color);
				}

				(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, get_field_text_ptr(flk), -1);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 0) == 0)
		{
			parse_xfont_from_style(&xf, style);
			parse_xface_from_style(&xa, style);
			if (!b_print)
			{
				format_xcolor(&pif->clr_txt, xf.color);
			}

			type = get_field_data_type_ptr(flk);
			fldfmt = get_field_format_ptr(flk);
			zeronull = get_field_zeronull(flk);
			wrapable = get_field_wrapable(flk);

			if (get_field_password(flk))
			{
				(*pif->pf_draw_password)(pif->canvas, &xf, &xa, &xr, get_field_text_ptr(flk), -1);
			}
			else
			{
				sz_text = get_field_options_text_ptr(flk);
				(*pif->pf_draw_data)(pif->canvas, &xf, &xa, &xr, sz_text, -1, get_field_data_dig(flk), type, fldfmt, zeronull, wrapable);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_CHECK, -1, 0) == 0)
		{
			sz_text = get_field_text_ptr(flk);

			if (!is_null(sz_text))
			{
				parse_xfont_from_style(&xf, style);
				if (!b_print)
				{
					format_xcolor(&pif->clr_txt, xf.color);
				}

				parse_xcolor(&xc, xf.color);
				ft_center_rect(&xr, DEF_SMALL_ICON, DEF_SMALL_ICON);
				(*pif->pf_draw_icon)(pif->canvas, &xc, &xr, ATTR_ICON_SELECTED);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_PHOTO, -1, 0) == 0)
		{
			if (!b_print && get_field_transparent(flk))
			{
				format_xcolor(&pif->clr_msk, xi.color);
			}
			else
			{
				xscpy(xi.color, _T(""));
			}

			parse_ximage_from_source(&xi, get_field_text_ptr(flk));

			(*pif->pf_draw_image)(pif->canvas, &xi, &xr);
		}
		else if (compare_text(sz_class, -1, DOC_FORM_HREF, -1, 0) == 0)
		{
			sz_text = get_field_text_ptr(flk);
			if (!is_null(sz_text))
			{
				xi.source = sz_text;
				xscpy(xi.type, GDI_ATTR_IMAGE_TYPE_THUMB);
				(*pif->pf_draw_image)(pif->canvas, &xi, &xr);

				parse_xfont_from_style(&xf, style);
				parse_xface_from_style(&xa, style);
				xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_FAR);
				xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
				if (!b_print)
				{
					format_xcolor(&pif->clr_txt, xf.color);
				}

				split_path(sz_text, NULL, sz_token, NULL);
				(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr, sz_token, -1);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_TABLE, -1, 0) == 0)
		{
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
			lighten_xbrush(&xb, DEF_SOFT_DARKEN);

			sz_text = get_field_text_ptr(flk);
			if (!is_null(sz_text))
			{
				cb.fx = xr.fx;
				cb.fy = xr.fy;
				cb.fw = xr.fw;
				cb.fh = xr.fh;

				obj = create_string_table(0);
				string_table_parse_options(obj, sz_text, -1, OPT_ITEMFEED, OPT_LINEFEED);
				draw_table(pif, &xf, &xa, &xp, &xb, &cb, obj, get_field_ratio(flk));
				destroy_string_table(obj);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_MEMO, -1, 0) == 0)
		{
			parse_xfont_from_style(&xf, style);
			parse_xface_from_style(&xa, style);
			if (!b_print)
			{
				format_xcolor(&pif->clr_txt, xf.color);
			}

			if (compare_text(sz_shape, -1, ATTR_SHAPE_MULTILINE, -1, 0) == 0)
			{
				(*pif->pf_multi_line)(pif->canvas, &xf, &xa, &xp, &xr);
			}

			sz_text = get_field_text_ptr(flk);
			if (!is_null(sz_text))
			{
				obj = create_memo_doc();
				parse_memo_doc(obj, sz_text, -1);
				(*pif->pf_draw_memo_text)(pif->canvas, &xf, &xa, &xr, obj, page);
				destroy_memo_doc(obj);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_TAG, -1, 0) == 0)
		{
			parse_xfont_from_style(&xf, style);
			parse_xface_from_style(&xa, style);
			if (!b_print)
			{
				format_xcolor(&pif->clr_txt, xf.color);
			}

			if (compare_text(sz_shape, -1, ATTR_SHAPE_MULTILINE, -1, 0) == 0)
			{
				(*pif->pf_multi_line)(pif->canvas, &xf, &xa, &xp, &xr);
			}

			sz_text = get_field_text_ptr(flk);
			if (!is_null(sz_text))
			{
				obj = create_tag_doc();
				parse_tag_doc(obj, sz_text, -1);
				(*pif->pf_draw_tag_text)(pif->canvas, &xf, &xa, &xr, obj, page);
				destroy_tag_doc(obj);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_RICH, -1, 0) == 0)
		{
			parse_xfont_from_style(&xf, style);
			parse_xface_from_style(&xa, style);
			if (!b_print)
			{
				format_xcolor(&pif->clr_txt, xf.color);
			}

			if (compare_text(sz_shape, -1, ATTR_SHAPE_MULTILINE, -1, 0) == 0)
			{
				(*pif->pf_multi_line)(pif->canvas, &xf, &xa, &xp, &xr);
			}

			obj = get_field_embed_rich(flk);
			if (obj)
			{
				(*pif->pf_draw_rich_text)(pif->canvas, &xf, &xa, &xr, obj, page);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_GRID, -1, 0) == 0)
		{
			obj = get_field_embed_grid(flk);
			if (obj)
			{
				cb.fx = xr.fx;
				cb.fy = xr.fy;
				cb.fw = xr.fw;
				cb.fh = xr.fh;
				
				draw_grid_page(pif, &cb, obj, page);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_GRAPH, -1, 0) == 0)
		{
			obj = get_field_embed_graph(flk);
			if (obj)
			{
				cb.fx = xr.fx;
				cb.fy = xr.fy;
				cb.fw = xr.fw;
				cb.fh = xr.fh;

				draw_graph_page(pif, &cb, obj, page);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_IMAGES, -1, 0) == 0)
		{
			obj = get_field_embed_images(flk);
			if (obj)
			{
				cb.fx = xr.fx;
				cb.fy = xr.fy;
				cb.fw = xr.fw;
				cb.fh = xr.fh;

				draw_images(pif, &cb, obj);
			}
		}
		else if (compare_text(sz_class, -1, DOC_FORM_FORM, -1, 0) == 0)
		{
			obj = get_field_embed_form(flk);
			if (obj)
			{
				cb.fx = xr.fx;
				cb.fy = xr.fy;
				cb.fw = xr.fw;
				cb.fh = xr.fh;

				draw_form_page(pif, &cb, obj, page);
			}
		}

	skip:
		if (b_design)
			flk = get_next_field(ptr, flk);
		else
			flk = get_next_visible_field(ptr, flk);
	}
}


#endif //XDLVIEW
