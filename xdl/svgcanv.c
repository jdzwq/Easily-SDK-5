/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc svg canvas document

	@module	svgcanv.c | implement file

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
#include "svggdi.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdldoc.h"


#if defined(XDL_SUPPORT_GDI)

typedef struct _svg_canvas_t{
	canvas_head head;

	visual_t view;
}svg_canvas_t;

canvas_t create_svg_canvas(visual_t view)
{
	svg_canvas_t* pcanv;
	link_t_ptr svg;
	float htpermm, vtpermm;
	xrect_t vb;

	svg = svg_get_visual_doc(view);

	pcanv = (svg_canvas_t*)xmem_alloc(sizeof(svg_canvas_t));

	htpermm = PTPERMM;
	vtpermm = PTPERMM;

	vb.x = 0;
	vb.y = 0;
	vb.w = (int)(get_svg_width(svg) * htpermm);
	vb.h = (int)(get_svg_height(svg) * vtpermm);
	set_svg_viewbox(svg, &vb);

	pcanv->view = view;
	pcanv->head.tag = _CANVAS_PRINTER;

	return &pcanv->head;
}

void destroy_svg_canvas(canvas_t canv)
{
	svg_canvas_t* pcanv = TypePtrFromHead(svg_canvas_t, canv);

	XDL_ASSERT(canv && canv->tag == _CANVAS_PRINTER);

	xmem_free(pcanv);
}

visual_t svg_get_canvas_visual(canvas_t canv)
{
	svg_canvas_t* pcanv = TypePtrFromHead(svg_canvas_t, canv);

	XDL_ASSERT(canv && canv->tag == _CANVAS_PRINTER);

	return pcanv->view;
}

float svg_pt_per_in(canvas_t canv, bool_t horz)
{
	svg_canvas_t* pcanv = TypePtrFromHead(svg_canvas_t, canv);

	return (float)PDPERINCH;
}

float svg_pt_per_mm(canvas_t canv, bool_t horz)
{
	svg_canvas_t* pcanv = TypePtrFromHead(svg_canvas_t, canv);
	link_t_ptr svg;
	xrect_t vb;

	XDL_ASSERT(canv && canv->tag == _CANVAS_PRINTER);

	svg = svg_get_visual_doc(pcanv->view);

	get_svg_viewbox(svg, &vb);

	if (horz)
	{
		return (float)((float)vb.w / get_svg_width(svg));
	}
	else
	{
		return (float)((float)vb.h / get_svg_height(svg));
	}
}

float svg_pt_to_tm(canvas_t canv, int pt, bool_t horz)
{
	svg_canvas_t* pcanv = TypePtrFromHead(svg_canvas_t, canv);
	link_t_ptr svg;
	xrect_t vb;
	float htpermm, vtpermm;

	XDL_ASSERT(canv && canv->tag == _CANVAS_PRINTER);

	svg = svg_get_visual_doc(pcanv->view);

	get_svg_viewbox(svg, &vb);

	if (horz)
	{
		htpermm = (float)((float)vb.w / get_svg_width(svg));

		return (float)((float)pt / htpermm);
	}
	else
	{
		vtpermm = (float)((float)vb.h / get_svg_height(svg));

		return (float)((float)pt / vtpermm);
	}
}

int svg_tm_to_pt(canvas_t canv, float tm, bool_t horz)
{
	svg_canvas_t* pcanv = TypePtrFromHead(svg_canvas_t, canv);
	link_t_ptr svg;
	xrect_t vb;
	float htpermm, vtpermm;

	XDL_ASSERT(canv && canv->tag == _CANVAS_PRINTER);

	svg = svg_get_visual_doc(pcanv->view);

	get_svg_viewbox(svg, &vb);

	if (horz)
	{
		htpermm = (float)((float)vb.w / get_svg_width(svg));

		return (int)(tm * htpermm + 0.5);
	}
	else
	{
		vtpermm = (float)((float)vb.h / get_svg_height(svg));

		return (int)(tm * vtpermm + 0.5);
	}
}

void svg_rect_tm_to_pt(canvas_t canv, xrect_t* pxr)
{
	int left, right, top, bottom;

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
	int cx, cy;

	cx = svg_tm_to_pt(canv, pxs->fw, 1) - svg_tm_to_pt(canv, 0, 1);
	cy = svg_tm_to_pt(canv, pxs->fh, 0) - svg_tm_to_pt(canv, 0, 0);

	pxs->w = cx;
	pxs->h = cy;
}

void svg_size_pt_to_tm(canvas_t canv, xsize_t* pxs)
{
	float cx, cy;

	cx = svg_pt_to_tm(canv, pxs->w, 1) - svg_pt_to_tm(canv, 0, 1);
	cy = svg_pt_to_tm(canv, pxs->h, 0) - svg_pt_to_tm(canv, 0, 0);

	pxs->fw = cx;
	pxs->fh = cy;
}

void svg_point_tm_to_pt(canvas_t canv, xpoint_t* ppt)
{
	int x, y;

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

void svg_span_tm_to_pt(canvas_t canv, xspan_t* ppn)
{
	ppn->r = svg_tm_to_pt(canv, ppn->fr, 1) - svg_tm_to_pt(canv, 0, 1);
}

void svg_span_pt_to_tm(canvas_t canv, xspan_t* ppn)
{
	ppn->fr = svg_pt_to_tm(canv, ppn->r, 1) - svg_pt_to_tm(canv, 0, 1);
}

typedef struct _svg_visual_t{
	visual_head head;

	link_t_ptr g;
}svg_visual_t;

visual_t create_svg_visual(link_t_ptr g)
{
	svg_visual_t* pview;
	link_t_ptr svg;
	xrect_t vb;

	pview = (svg_visual_t*)xmem_alloc(sizeof(svg_visual_t));

	pview->g = g;
	pview->head.tag = _VIEWING_SCRIPT;

	svg = svg_doc_from_node(g);
	get_svg_viewbox(svg, &vb);

	return &pview->head;
}

void destroy_svg_visual(visual_t view)
{
	svg_visual_t* pview = TypePtrFromHead(svg_visual_t, view);

	XDL_ASSERT(view && view->tag == _VIEWING_SCRIPT);

	xmem_free(pview);
}

link_t_ptr svg_get_visual_doc(visual_t view)
{
	svg_visual_t* pview = TypePtrFromHead(svg_visual_t, view);

	XDL_ASSERT(view && view->tag == _VIEWING_SCRIPT);

	return pview->g;
}

float svg_pt_to_tm_raw(visual_t view, int pt, bool_t horz)
{
	svg_visual_t* pview = TypePtrFromHead(svg_visual_t, view);

	xrect_t vb;
	float htpermm, vtpermm;

	XDL_ASSERT(view && view->tag == _VIEWING_SCRIPT);

	get_svg_viewbox(pview->g, &vb);

	if (horz)
	{
		htpermm = (float)((float)vb.w / get_svg_width(pview->g));

		return (float)((float)pt / htpermm);
	}
	else
	{
		vtpermm = (float)((float)vb.h / get_svg_height(pview->g));

		return (float)((float)pt / vtpermm);
	}
}

int svg_tm_to_pt_raw(visual_t view, float tm, bool_t horz)
{
	svg_visual_t* pview = TypePtrFromHead(svg_visual_t, view);

	xrect_t vb;
	float htpermm, vtpermm;

	XDL_ASSERT(view && view->tag == _VIEWING_SCRIPT);

	get_svg_viewbox(pview->g, &vb);

	if (horz)
	{
		htpermm = (float)((float)vb.w / get_svg_width(pview->g));

		return (int)(tm * htpermm + 0.5);
	}
	else
	{
		vtpermm = (float)((float)vb.h / get_svg_height(pview->g));

		return (int)(tm * vtpermm + 0.5);
	}
}


#endif /*XDL_SUPPORT_VIEW*/