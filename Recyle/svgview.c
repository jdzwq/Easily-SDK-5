/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc svg document

	@module	svgview.c | svg view implement file

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
#include "svgview.h"
#include "svgdoc.h"
#include "xdlstd.h"

#if defined(XDL_SUPPORT_VIEW) 

void draw_svg(const if_canvas_t* pcanv, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr ilk;
	link_t_ptr st = NULL;
	long count;
	xrect_t xr = { 0 };
	xpoint_t pt[2];
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xcolor_t xc = { 0 };
	ximage_t xi = { 0 };
	const tchar_t* sz_name;
	const tchar_t* sz_text;
	double fang, tang;
	xpoint_t* ppt;
	int i;
	bool_t b_round;
	xhand_t svgcanv;

	svgcanv = create_svg_canvas(ptr);

	ilk = get_svg_child_node(ptr);
	while (ilk)
	{
		sz_name = get_svg_node_name_ptr(ilk);

		if (compare_text(sz_name, -1, SVG_NODE_LINE, -1, 1) == 0)
		{
			default_xpen(&xp);
			read_line_from_svg_node(ilk, &xp, &pt[0], &pt[1]);
			svg_point_pt_to_tm(svgcanv, &pt[0]);
			svg_point_pt_to_tm(svgcanv, &pt[1]);

			pt[0].fx += pbox->fx;
			pt[0].fy += pbox->fy;
			pt[1].fx += pbox->fx;
			pt[1].fy += pbox->fy;
			(*pcanv->pf_draw_line)(pcanv->canvas, &xp, &pt[0], &pt[1]);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_RECT, -1, 1) == 0)
		{
			default_xpen(&xp);
			default_xbrush(&xb);
			b_round = read_rect_from_svg_node(ilk, &xp, &xb, &xr);
			svg_rect_pt_to_tm(svgcanv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			if (b_round)
				(*pcanv->pf_draw_round)(pcanv->canvas, &xp, &xb, &xr);
			else
				(*pcanv->pf_draw_rect)(pcanv->canvas, &xp, &xb, &xr);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_ELLIPSE, -1, 1) == 0)
		{
			default_xpen(&xp);
			default_xbrush(&xb);
			read_ellipse_from_svg_node(ilk, &xp, &xb, &xr);
			svg_rect_pt_to_tm(svgcanv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			(*pcanv->pf_draw_ellipse)(pcanv->canvas, &xp, &xb, &xr);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_POLYLINE, -1, 1) == 0)
		{
			count = read_polyline_from_svg_node(ilk, NULL, NULL, MAX_LONG);
			if (count)
			{
				default_xpen(&xp);
				default_xbrush(&xb);

				ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * count);
				read_polyline_from_svg_node(ilk, &xp, ppt, count);
				for (i = 0; i < count; i++)
				{
					svg_point_pt_to_tm(svgcanv, &ppt[i]);

					ppt[i].fx += pbox->fx;
					ppt[i].fy += pbox->fy;
				}
				(*pcanv->pf_draw_polyline)(pcanv->canvas, &xp, ppt, count);

				xmem_free(ppt);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_POLYGON, -1, 1) == 0)
		{
			count = read_polygon_from_svg_node(ilk, NULL, NULL, NULL, MAX_LONG);
			if (count)
			{
				default_xpen(&xp);
				default_xbrush(&xb);

				ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * count);
				read_polygon_from_svg_node(ilk, &xp, &xb, ppt, count);
				for (i = 0; i < count; i++)
				{
					svg_point_pt_to_tm(svgcanv, &ppt[i]);

					ppt[i].fx += pbox->fx;
					ppt[i].fy += pbox->fy;
				}
				(*pcanv->pf_draw_polygon)(pcanv->canvas, &xp, &xb, ppt, count);

				xmem_free(ppt);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_TEXT, -1, 1) == 0)
		{
			default_xfont(&xf);
			default_xface(&xa);
			sz_text = read_text_from_svg_node(ilk, &xf, &xa, &xr);
			svg_rect_pt_to_tm(svgcanv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			if (xr.h)
			{
				(*pcanv->pf_draw_text)(pcanv->canvas, &xf, &xa, &xr, sz_text, -1);
			}
			else
			{
				pt[0].fx = xr.fx;
				pt[0].fy = xr.fy;
				(*pcanv->pf_text_out)(pcanv->canvas, &xf, &pt[0], sz_text, -1);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_PATH, -1, 1) == 0)
		{
			if (svg_node_is_pie(ilk))
			{
				read_pie_from_svg_node(ilk, &xp, &xb, &xr, &fang, &tang);

				svg_rect_pt_to_tm(svgcanv, &xr);
				xr.fx += pbox->fx;
				xr.fy += pbox->fy;

				(*pcanv->pf_draw_pie)(pcanv->canvas, &xp, &xb, &xr, fang, tang);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_IMAGE, -1, 1) == 0)
		{
			read_image_from_svg_node(ilk, &xi, &xr);

			svg_rect_pt_to_tm(svgcanv, &xr);
			xr.fx += pbox->fx;
			xr.fy += pbox->fy;

			(*pcanv->pf_draw_image)(pcanv->canvas, &xi, &xr);
		}

		if (get_svg_child_node(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_node(st, (void*)ilk);

			ilk = get_svg_child_node(ilk);
			continue;

		}

		while (ilk)
		{
			ilk = get_svg_next_sibling_node(ilk);
			if (ilk)
				break;
			ilk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);

	destroy_svg_canvas(svgcanv);
}


#endif //XDLVIEW