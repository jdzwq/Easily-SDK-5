/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc canvas document

	@module	gdicanv.c | implement file

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

#include "gdicanv.h"
#include "xdcimp.h"

#if defined(XDU_SUPPORT_CONTEXT)

typedef struct _rdc_canvas_t{
	canvas_head head;

	res_ctx_t dc;		// memory draw context 

	float htpermm, vtpermm;
	float horz_feed, vert_feed;
	float horz_size, vert_size;
}rdc_canvas_t;

/*******************************************************************************************************************/
float pt_to_tm(canvas_t canv, int pt, bool_t horz)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t,canv);

	XDL_ASSERT(canv);

	if (horz)
		return (float)((float)pt / (float)(pcanv->htpermm) - pcanv->horz_feed);
	else
		return (float)((float)pt / (float)(pcanv->vtpermm) - pcanv->vert_feed);
}

int tm_to_pt(canvas_t canv, float tm, bool_t horz)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	if (horz)
		return (int)(((float)tm + (float)pcanv->horz_feed) * pcanv->htpermm + 0.5);
	else
		return (int)(((float)tm + (float)pcanv->vert_feed) * pcanv->vtpermm + 0.5);
}

void rect_tm_to_pt(canvas_t canv, xrect_t* pxr)
{
	int left, right, top, bottom;

	if (pxr->fw < 0)
	{
		left = tm_to_pt(canv, pxr->fx + pxr->fw, 1);
		right = tm_to_pt(canv, pxr->fx, 1);
	}
	else
	{
		left = tm_to_pt(canv, pxr->fx, 1);
		right = tm_to_pt(canv, pxr->fx + pxr->fw, 1);
	}

	if (pxr->fh < 0)
	{
		top = tm_to_pt(canv, pxr->fy + pxr->fh, 0);
		bottom = tm_to_pt(canv, pxr->fy, 0);
	}
	else
	{
		top = tm_to_pt(canv, pxr->fy, 0);
		bottom = tm_to_pt(canv, pxr->fy + pxr->fh, 0);
	}

	pxr->x = left;
	pxr->y = top;
	pxr->w = right - left;
	pxr->h = bottom - top;
}

void rect_pt_to_tm(canvas_t canv, xrect_t* pxr)
{
	float left, right, top, bottom;

	if (pxr->w < 0)
	{
		left = pt_to_tm(canv, pxr->x + pxr->w, 1);
		right = pt_to_tm(canv, pxr->x, 1);
	}
	else
	{
		left = pt_to_tm(canv, pxr->x, 1);
		right = pt_to_tm(canv, pxr->x + pxr->w, 1);
	}

	if (pxr->h < 0)
	{
		top = pt_to_tm(canv, pxr->y + pxr->h, 0);
		bottom = pt_to_tm(canv, pxr->y, 0);
	}
	else
	{
		top = pt_to_tm(canv, pxr->y, 0);
		bottom = pt_to_tm(canv, pxr->y + pxr->h, 0);
	}

	pxr->fx = left;
	pxr->fy = top;
	pxr->fw = right - left;
	pxr->fh = bottom - top;
}

void size_tm_to_pt(canvas_t canv, xsize_t* pxs)
{
	int cx, cy;

	cx = tm_to_pt(canv, pxs->fx, 1) - tm_to_pt(canv, 0, 1);
	cy = tm_to_pt(canv, pxs->fy, 0) - tm_to_pt(canv, 0, 0);

	pxs->cx = cx;
	pxs->cy = cy;
}

void size_pt_to_tm(canvas_t canv, xsize_t* pxs)
{
	float cx, cy;

	cx = pt_to_tm(canv, pxs->cx, 1) - pt_to_tm(canv, 0, 1);
	cy = pt_to_tm(canv, pxs->cy, 0) - pt_to_tm(canv, 0, 0);

	pxs->fx = cx;
	pxs->fy = cy;
}

void point_tm_to_pt(canvas_t canv, xpoint_t* ppt)
{
	int x, y;

	x = tm_to_pt(canv, ppt->fx, 1);
	y = tm_to_pt(canv, ppt->fy, 0);

	ppt->x = x;
	ppt->y = y;
}

void point_pt_to_tm(canvas_t canv, xpoint_t* ppt)
{
	float x, y;

	x = pt_to_tm(canv, ppt->x, 1);
	y = pt_to_tm(canv, ppt->y, 0);

	ppt->fx = x;
	ppt->fy = y;
}

/*******************************************************************************************/
canvas_t create_display_canvas(res_ctx_t rdc)
{
	rdc_canvas_t* pcanv;
	dev_cap_t cap = { 0 };

	pcanv = (rdc_canvas_t*)xmem_alloc(sizeof(rdc_canvas_t));

	if (rdc)
		pcanv->dc = create_compatible_context(rdc, 1, 1);
	else
		pcanv->dc = create_display_context(NULL);

	get_device_caps(pcanv->dc, &cap);

	pcanv->htpermm = (float)((float)cap.horz_pixels * INCHPERMM);
	pcanv->vtpermm = (float)((float)cap.vert_pixels * INCHPERMM);
	pcanv->horz_size = (float)((float)cap.horz_res / (float)cap.horz_pixels / INCHPERMM);
	pcanv->vert_size = (float)((float)cap.vert_res / (float)cap.vert_pixels / INCHPERMM);
	pcanv->horz_feed = 0.0;
	pcanv->vert_feed = 0.0;

	destroy_context(pcanv->dc);
	pcanv->dc = NULL;

	pcanv->head.tag = _CANVAS_DISPLAY;

	return &pcanv->head;
}

void destroy_display_canvas(canvas_t canv)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv && canv->tag == _CANVAS_DISPLAY);

	xmem_free(pcanv);
}

void set_canvas_ratio(canvas_t canv, float htpermm, float vtpermm)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	pcanv->htpermm = htpermm;
	pcanv->vtpermm = vtpermm;
}

float get_canvas_horz_size(canvas_t canv)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	return pcanv->horz_size - 2 * pcanv->horz_feed;
}

float get_canvas_vert_size(canvas_t canv)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	return pcanv->vert_size - 2 * pcanv->vert_feed;
}

void set_canvas_horz_feed(canvas_t canv, float cx)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	pcanv->horz_feed = cx;
}

float get_canvas_horz_feed(canvas_t canv)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	return pcanv->horz_feed;
}

void set_canvas_vert_feed(canvas_t canv, float cx)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	pcanv->vert_feed = cx;
}

float get_canvas_vert_feed(canvas_t canv)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	return pcanv->vert_feed;
}

res_ctx_t get_canvas_ctx(canvas_t canv)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	return (res_ctx_t)pcanv->dc;
}

res_ctx_t begin_canvas_paint(canvas_t canv, res_ctx_t rdc, int width, int height)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	dev_cap_t dev = { 0 };

	XDL_ASSERT(canv);

	pcanv->dc = create_compatible_context(rdc, width, height);

	XDL_ASSERT(pcanv->dc != NULL);

	return pcanv->dc;
}

void end_canvas_paint(canvas_t canv, res_ctx_t rdc, const xrect_t* pxr)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	render_context(pcanv->dc, pxr->x, pxr->y, rdc, pxr->x, pxr->y, pxr->w, pxr->h);

	destroy_context(pcanv->dc);
	pcanv->dc = NULL;
}

void get_canvas_measure(canvas_t canv, if_measure_t* pif)
{
	pif->ctx = (void*)canv;
	pif->pf_text_metric = (PF_TEXT_METRIC)text_metric;
	pif->pf_text_size = (PF_TEXT_SIZE)text_size;
}

#ifdef XDU_SUPPORT_CONTEXT_PRINTER

canvas_t create_printer_canvas(res_ctx_t rdc)
{
	rdc_canvas_t* pcanv;
	dev_cap_t cap = { 0 };

	pcanv = (rdc_canvas_t*)xmem_alloc(sizeof(rdc_canvas_t));

	get_device_caps(rdc, &cap);

	pcanv->dc = rdc;
	pcanv->htpermm = (float)((float)cap.horz_pixels * INCHPERMM);
	pcanv->vtpermm = (float)((float)cap.vert_pixels * INCHPERMM);
	pcanv->horz_size = (float)((float)cap.horz_size / (float)cap.horz_pixels / INCHPERMM);
	pcanv->vert_size = (float)((float)cap.vert_size / (float)cap.vert_pixels / INCHPERMM);
	pcanv->horz_feed = (float)((float)cap.horz_feed / (float)cap.horz_pixels / INCHPERMM);
	pcanv->vert_feed = (float)((float)cap.vert_feed / (float)cap.horz_pixels / INCHPERMM);

	pcanv->head.tag = _CANVAS_PRINTER;

	return &pcanv->head;
}

void  destroy_printer_canvas(canvas_t canv)
{
	rdc_canvas_t* pcanv = TypePtrFromHead(rdc_canvas_t, canv);

	XDL_ASSERT(canv);

	xmem_free(pcanv);
}

#endif //#ifdef XDU_SUPPORT_CONTEXT_PRINTER

#endif //#ifdef XDC_SUPPORT_CONTXT

