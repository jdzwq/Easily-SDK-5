/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc topog document

	@module	topogview.c | implement file

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

#include "topogview.h"
#include "xdldoc.h"
#include "xdlimp.h"

#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW

void calc_topog_spot_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	pxr->fx = get_topog_spot_col(ilk) * get_topog_rx(ptr);
	pxr->fy = get_topog_spot_row(ilk) * get_topog_ry(ptr);
	pxr->fw = get_topog_rx(ptr);
	pxr->fh = get_topog_ry(ptr);
}

int calc_topog_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr, link_t_ptr* pilk, int* prow, int* pcol)
{
	link_t_ptr ilk;
	int nHit;
	float xm, ym;
	xrect_t di;

	xm = ppt->fx;
	ym = ppt->fy;

	nHit = TOPOG_HINT_NONE;
	*pilk = NULL;

	*prow = (int)(ym / get_topog_ry(ptr));
	*pcol = (int)(xm / get_topog_rx(ptr));

	if (*prow < 0 || *prow >= get_topog_rows(ptr))
	{
		*prow = -1;
	}

	if (*pcol < 0 || *pcol >= get_topog_cols(ptr))
	{
		*pcol = -1;
	}

	ilk = get_topog_prev_spot(ptr, LINK_LAST);
	while (ilk)
	{
		calc_topog_spot_rect(pbox, ptr, ilk, &di);

		if (ft_inside(xm, ym, di.fx, di.fy, di.fx + di.fw, di.fy + di.fh))
		{
			nHit = TOPOG_HINT_SPOT;
			*pilk = ilk;

			break;
		}
		ilk = get_topog_prev_spot(ptr, ilk);
	}

	return nHit;
}

void draw_topog(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr ilk;
	xrect_t xr;
	xsize_t xs;
	xbrush_t xb_dot, xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };

	bool_t b_design, b_print;
	const tchar_t *style;
	const tchar_t *type;

	matrix_t* mt = NULL;
	int rows,cols, i, j;
	float rx, ry;
	int dark;

	string_t vs = NULL;

	b_print = (pif->canvas->tag == _CANVAS_PRINTER)? 1 : 0;
	b_design = topog_is_design(ptr);

	style = get_topog_style_ptr(ptr);
	
	parse_xbrush_from_style(&xb, style);
	if (!b_print)
	{
		format_xcolor(&pif->clr_bkg, xb.color);
	}

	parse_xpen_from_style(&xp, style);
	xscpy(xp.color, xb.color);
	lighten_xpen(&xp, DEF_SOFT_LIGHTEN);

	xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
	lighten_xbrush(&xb_dot, DEF_SOFT_LIGHTEN);

	if (!b_print)
	{
		format_xcolor(&pif->clr_msk, xi.color);
	}
	else
	{
		xscpy(xi.color, _T(""));
	}

	rows = get_topog_rows(ptr);
	cols = get_topog_cols(ptr);

	mt = matrix_alloc(rows, cols);

	matrix_parse(mt, get_topog_matrix_ptr(ptr), -1);

	rx = get_topog_rx(ptr);
	ry = get_topog_ry(ptr);

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			xr.fx = pbox->fx + j * rx;
			xr.fy = pbox->fy + i * ry;
			xr.fw = rx;
			xr.fh = ry;

			ft_expand_rect(&xr, -0.5, -0.5);

			dark = (int)matrix_get_value(mt, i, j);
			if (dark)
			{
				xmem_copy((void*)&xb_dot, (void*)&xb, sizeof(xbrush_t));
				lighten_xbrush(&xb_dot, dark + DEF_SOFT_LIGHTEN);
				(*pif->pf_draw_rect)(pif->canvas, ((b_design)? &xp : NULL), &xb_dot, &xr);
			}
			else if (b_design)
			{
				(*pif->pf_draw_rect)(pif->canvas, &xp, NULL, &xr);
			}
		}
	}

	matrix_free(mt);

	vs = string_alloc();

	ilk = get_topog_next_spot(ptr, LINK_FIRST);
	while (ilk)
	{
		style = get_topog_spot_style_ptr(ilk);
		type = get_topog_spot_type_ptr(ilk);

		default_xfont(&xf);
		parse_xfont_from_style(&xf, style);

		default_xface(&xa);
		parse_xface_from_style(&xa, style);

		(*pif->pf_measure_metric)(pif->canvas, &xf, &xs);

		calc_topog_spot_rect(pbox, ptr, ilk, &xr);
		xr.fw = xs.fx;
		xr.fh = xs.fy;
		ft_offset_rect(&xr, pbox->fx, pbox->fy);

		if (compare_text(type, -1, ATTR_SPOT_TYPE_COLORBAR, -1, 0) == 0)
		{
			(*pif->pf_color_out)(pif->canvas, &xr, 1, get_topog_spot_title_ptr(ilk), -1);
		}
		else if (compare_text(type, -1, ATTR_SPOT_TYPE_ICON, -1, 0) == 0)
		{
			parse_xcolor(&xc, xf.color);
			(*pif->pf_draw_gizmo)(pif->canvas, &xc, &xr, get_topog_spot_title_ptr(ilk));
		}
		else if (compare_text(type, -1, ATTR_SPOT_TYPE_IMAGE, -1, 0) == 0)
		{
			parse_ximage_from_source(&xi, get_topog_spot_title_ptr(ilk));
			(*pif->pf_draw_image)(pif->canvas, &xi, &xr);
			xi.source = NULL;
		}
		else if (compare_text(type, -1, ATTR_SPOT_TYPE_TEXT, -1, 0) == 0)
		{
			string_cpy(vs, get_topog_spot_title_ptr(ilk), -1);
			(*pif->pf_draw_var_text)(pif->canvas, &xf, &xa, &xr, vs);
		}

		ilk = get_topog_next_spot(ptr, ilk);
	}

	string_free(vs);
}


#endif //XDLVIEW
