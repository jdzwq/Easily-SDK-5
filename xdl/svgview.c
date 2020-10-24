/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg document

	@module	svgview.c | implement file

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

#include "svgview.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdlgdi.h"
#include "xdldoc.h"


#if defined(XDL_SUPPORT_VIEW)


void draw_svg(const if_drawing_t* pif, const xrect_t* pbox, link_t_ptr ptr)
{
	link_t_ptr ilk;
	int count;
	xrect_t xr = { 0 };
	xsize_t xs = { 0 };
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
	bool_t sflag, lflag;

	xpoint_t* ppt;
	int i;
	bool_t b_round;
	int rx, ry;
	visual_t view;
	canvas_t canv;

	link_t_ptr st = NULL;

	int an, pn;
	tchar_t* aa = NULL;
	xpoint_t* pa = NULL;

	view = create_svg_visual(ptr);
	canv = create_svg_canvas(view);

	ilk = get_svg_first_child_node(ptr);
	while (ilk)
	{
		sz_name = get_svg_node_name_ptr(ilk);

		if (compare_text(sz_name, -1, SVG_NODE_LINE, -1, 1) == 0)
		{
			xmem_zero((void*)&xp, sizeof(xpen_t));
			read_line_from_svg_node(ilk, &xp, &pt[0], &pt[1]);
			svg_point_pt_to_tm(canv, &pt[0]);
			svg_point_pt_to_tm(canv, &pt[1]);

			pt[0].fx += pbox->fx;
			pt[0].fy += pbox->fy;
			pt[1].fx += pbox->fx;
			pt[1].fy += pbox->fy;
			(*pif->pf_draw_line)(pif->ctx, &xp, &pt[0], &pt[1]);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_RECT, -1, 1) == 0)
		{
			xmem_zero((void*)&xp, sizeof(xpen_t));
			xmem_zero((void*)&xb, sizeof(xbrush_t));
			b_round = svg_node_is_round(ilk);
			if (b_round)
				read_round_from_svg_node(ilk, &xp, &xb, &xr, &rx, &ry);
			else
				read_rect_from_svg_node(ilk, &xp, &xb, &xr);

			svg_rect_pt_to_tm(canv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			if (b_round)
				(*pif->pf_draw_round)(pif->ctx, &xp, &xb, &xr);
			else
				(*pif->pf_draw_rect)(pif->ctx, &xp, &xb, &xr);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_ELLIPSE, -1, 1) == 0)
		{
			xmem_zero((void*)&xp, sizeof(xpen_t));
			xmem_zero((void*)&xb, sizeof(xbrush_t));
			read_ellipse_from_svg_node(ilk, &xp, &xb, &xr);
			svg_rect_pt_to_tm(canv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			(*pif->pf_draw_ellipse)(pif->ctx, &xp, &xb, &xr);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_POLYLINE, -1, 1) == 0)
		{
			count = read_polyline_from_svg_node(ilk, NULL, NULL, MAX_LONG);
			if (count)
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				xmem_zero((void*)&xb, sizeof(xbrush_t));

				ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * count);
				read_polyline_from_svg_node(ilk, &xp, ppt, count);
				for (i = 0; i < count; i++)
				{
					svg_point_pt_to_tm(canv, &ppt[i]);

					ppt[i].fx += pbox->fx;
					ppt[i].fy += pbox->fy;
				}
				(*pif->pf_draw_polyline)(pif->ctx, &xp, ppt, count);

				xmem_free(ppt);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_POLYGON, -1, 1) == 0)
		{
			count = read_polygon_from_svg_node(ilk, NULL, NULL, NULL, MAX_LONG);
			if (count)
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				xmem_zero((void*)&xb, sizeof(xbrush_t));

				ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * count);
				read_polygon_from_svg_node(ilk, &xp, &xb, ppt, count);
				for (i = 0; i < count; i++)
				{
					svg_point_pt_to_tm(canv, &ppt[i]);

					ppt[i].fx += pbox->fx;
					ppt[i].fy += pbox->fy;
				}
				(*pif->pf_draw_polygon)(pif->ctx, &xp, &xb, ppt, count);

				xmem_free(ppt);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_TEXT, -1, 1) == 0)
		{
			xmem_zero((void*)&xf, sizeof(xfont_t));
			xmem_zero((void*)&xa, sizeof(xface_t));
			sz_text = read_text_from_svg_node(ilk, &xf, &xa, &xr);
			svg_rect_pt_to_tm(canv, &xr);

			xr.fx += pbox->fx;
			xr.fy += pbox->fy;
			if (xr.h)
			{
				(*pif->pf_draw_text)(pif->ctx, &xf, &xa, &xr, sz_text, -1);
			}
			else
			{
				pt[0].fx = xr.fx;
				pt[0].fy = xr.fy;
				(*pif->pf_text_out)(pif->ctx, &xf, &pt[0], sz_text, -1);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_PATH, -1, 1) == 0)
		{
			if (svg_node_is_pie(ilk))
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				xmem_zero((void*)&xb, sizeof(xbrush_t));
				read_pie_from_svg_node(ilk, &xp, &xb, RECTPOINT(&xr),&xr.w, &xr.h, &fang, &tang);

				svg_rect_pt_to_tm(canv, &xr);
				xr.fx += pbox->fx;
				xr.fy += pbox->fy;

				(*pif->pf_draw_pie)(pif->ctx, &xp, &xb, RECTPOINT(&xr), RECTSIZE(&xr), fang, tang);
			}
			else if (svg_node_is_arc(ilk))
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				read_arc_from_svg_node(ilk, &xp, &pt[0], &pt[1], &xs.w, &xs.h, &sflag, &lflag);

				svg_point_pt_to_tm(canv, &pt[0]);
				pt[0].fx += pbox->fx;
				pt[0].fy += pbox->fy;

				svg_point_pt_to_tm(canv, &pt[1]);
				pt[1].fx += pbox->fx;
				pt[1].fy += pbox->fy;

				svg_size_pt_to_tm(canv, &xs);

				(*pif->pf_draw_arc)(pif->ctx, &xp, &pt[0], &pt[1], &xs, sflag, lflag);
			}
			else
			{
				xmem_zero((void*)&xp, sizeof(xpen_t));
				xmem_zero((void*)&xb, sizeof(xbrush_t));
				read_path_from_svg_node(ilk, &xp, &xb, NULL, &an, NULL, &pn);

				svg_rect_pt_to_tm(canv, &xr);
				xr.fx += pbox->fx;
				xr.fy += pbox->fy;

				(*pif->pf_draw_path)(pif->ctx, &xp, &xb, aa, pa, pn);
			}
		}
		else if (compare_text(sz_name, -1, SVG_NODE_IMAGE, -1, 1) == 0)
		{
			xmem_zero((void*)&xi, sizeof(ximage_t));
			read_ximage_from_svg_node(ilk, &xi, &xr);

			svg_rect_pt_to_tm(canv, &xr);
			xr.fx += pbox->fx;
			xr.fy += pbox->fy;

			(*pif->pf_draw_image)(pif->ctx, &xi, &xr);
		}
		else if (compare_text(sz_name, -1, SVG_NODE_IMAGE, -1, 1) == 0)
		{
			xmem_zero((void*)&xi, sizeof(ximage_t));
			read_ximage_from_svg_node(ilk, &xi, &xr);

			svg_rect_pt_to_tm(canv, &xr);
			xr.fx += pbox->fx;
			xr.fy += pbox->fy;

			(*pif->pf_draw_image)(pif->ctx, &xi, &xr);
		}

		if (get_svg_first_child_node(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_node(st, (void*)ilk);

			ilk = get_svg_first_child_node(ilk);
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

	destroy_svg_canvas(canv);
	destroy_svg_visual(view);
}

#endif /*XDL_SUPPORT_VIEW*/
