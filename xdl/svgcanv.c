/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg canvas document

	@module	svgcanv.c | svg canvas implement file

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

#include "svgcanv.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlview.h"

#if defined(XDK_SUPPORT_CONTEXT)

typedef struct _svg_canvas_t{
	res_ctx_t rdc;
	int dev;			// canvas type 
	link_t_ptr g;		// svg document 
}svg_canvas_t;


canvas_t create_svg_canvas(link_t_ptr svg)
{
	svg_canvas_t* pcanv;
	float htpermm, vtpermm;
	xrect_t vb;

	pcanv = (svg_canvas_t*)xmem_alloc(sizeof(svg_canvas_t));

	pcanv->rdc = create_display_context();

	htpermm = pt_per_mm(pcanv->rdc, 1);
	vtpermm = pt_per_mm(pcanv->rdc, 0);

	vb.x = 0;
	vb.y = 0;
	vb.w = (long)(get_svg_width(svg) * htpermm);
	vb.h = (long)(get_svg_height(svg) * vtpermm);
	set_svg_viewbox(svg, &vb);

	pcanv->dev = _CANV_PRINTER;
	pcanv->g = svg;

	return (canvas_t)pcanv;
}

void destroy_svg_canvas(canvas_t canv)
{
	svg_canvas_t* pcanv = (svg_canvas_t*)canv;

	destroy_context(pcanv->rdc);

	xmem_free(pcanv);
}

res_ctx_t svg_get_canvas_ctx(canvas_t canv)
{
	svg_canvas_t* pcanv = (svg_canvas_t*)canv;

	return pcanv->rdc;
}

link_t_ptr svg_get_canvas_doc(canvas_t canv)
{
	svg_canvas_t* pcanv = (svg_canvas_t*)canv;

	return pcanv->g;
}

int svg_get_canvas_type(canvas_t canv)
{
	svg_canvas_t* pcanv = (svg_canvas_t*)canv;

	return pcanv->dev;
}

float svg_pt_to_tm(canvas_t canv, long pt, bool_t horz)
{
	svg_canvas_t* pcanv = (svg_canvas_t*)canv;
	xrect_t vb;
	float htpermm, vtpermm;

	get_svg_viewbox(pcanv->g, &vb);

	if (horz)
	{
		htpermm = (float)((float)vb.w / get_svg_width(pcanv->g));

		return (float)((float)pt / htpermm);
	}
	else
	{
		vtpermm = (float)((float)vb.h / get_svg_height(pcanv->g));

		return (float)((float)pt / vtpermm);
	}
}

long svg_tm_to_pt(canvas_t canv, float tm, bool_t horz)
{
	svg_canvas_t* pcanv = (svg_canvas_t*)canv;
	xrect_t vb;
	float htpermm, vtpermm;

	get_svg_viewbox(pcanv->g, &vb);

	if (horz)
	{
		htpermm = (float)((float)vb.w / get_svg_width(pcanv->g));

		return (long)(tm * htpermm);
	}
	else
	{
		vtpermm = (float)((float)vb.h / get_svg_height(pcanv->g));

		return (long)(tm * vtpermm);
	}
}

void svg_rect_tm_to_pt(canvas_t canv, xrect_t* pxr)
{
	long left, right, top, bottom;

	if (pxr->fw < 0)
	{
		left = svg_tm_to_pt(canv, pxr->fx + pxr->fw, 1);
		right = svg_tm_to_pt(canv, pxr->fx, 1);
	}
	else
	{
		left = svg_tm_to_pt(canv, pxr->fx, 1);
		right = svg_tm_to_pt(canv, pxr->fx + pxr->fw, 1);
	}

	if (pxr->fh < 0)
	{
		top = svg_tm_to_pt(canv, pxr->fy + pxr->fh, 0);
		bottom = svg_tm_to_pt(canv, pxr->fy, 0);
	}
	else
	{
		top = svg_tm_to_pt(canv, pxr->fy, 0);
		bottom = svg_tm_to_pt(canv, pxr->fy + pxr->fh, 0);
	}

	pxr->x = left;
	pxr->y = top;
	pxr->w = right - left;
	pxr->h = bottom - top;
}

void svg_rect_pt_to_tm(canvas_t canv, xrect_t* pxr)
{
	float left, right, top, bottom;

	if (pxr->w < 0)
	{
		left = svg_pt_to_tm(canv, pxr->x + pxr->w, 1);
		right = svg_pt_to_tm(canv, pxr->x, 1);
	}
	else
	{
		left = svg_pt_to_tm(canv, pxr->x, 1);
		right = svg_pt_to_tm(canv, pxr->x + pxr->w, 1);
	}

	if (pxr->h < 0)
	{
		top = svg_pt_to_tm(canv, pxr->y + pxr->h, 0);
		bottom = svg_pt_to_tm(canv, pxr->y, 0);
	}
	else
	{
		top = svg_pt_to_tm(canv, pxr->y, 0);
		bottom = svg_pt_to_tm(canv, pxr->y + pxr->h, 0);
	}

	pxr->fx = left;
	pxr->fy = top;
	pxr->fw = right - left;
	pxr->fh = bottom - top;
}

void svg_size_tm_to_pt(canvas_t canv, xsize_t* pxs)
{
	long cx, cy;

	cx = svg_tm_to_pt(canv, pxs->fx, 1) - svg_tm_to_pt(canv, 0, 1);
	cy = svg_tm_to_pt(canv, pxs->fy, 0) - svg_tm_to_pt(canv, 0, 0);

	pxs->cx = cx;
	pxs->cy = cy;
}

void svg_size_pt_to_tm(canvas_t canv, xsize_t* pxs)
{
	float cx, cy;

	cx = svg_pt_to_tm(canv, pxs->cx, 1) - svg_pt_to_tm(canv, 0, 1);
	cy = svg_pt_to_tm(canv, pxs->cy, 0) - svg_pt_to_tm(canv, 0, 0);

	pxs->fx = cx;
	pxs->fy = cy;
}

void svg_point_tm_to_pt(canvas_t canv, xpoint_t* ppt)
{
	long x, y;

	x = svg_tm_to_pt(canv, ppt->fx, 1);
	y = svg_tm_to_pt(canv, ppt->fy, 0);

	ppt->x = x;
	ppt->y = y;
}

void svg_point_pt_to_tm(canvas_t canv, xpoint_t* ppt)
{
	float x, y;

	x = svg_pt_to_tm(canv, ppt->x, 1);
	y = svg_pt_to_tm(canv, ppt->y, 0);

	ppt->fx = x;
	ppt->fy = y;
}

void svg_get_canvas_measure(canvas_t canv, if_measure_t* pif)
{
	pif->ctx = (void*)canv;
	pif->pf_text_metric = (PF_TEXT_METRIC)svg_text_metric;
	pif->pf_text_size = (PF_TEXT_SIZE)svg_text_size;;
}

#endif /*XDL_SUPPORT_VIEW*/