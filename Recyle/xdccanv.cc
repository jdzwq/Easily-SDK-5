/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc canvas document

	@module	xdccanv.c | canvas implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2014.12	v4.0
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

#include "xdccanv.h"
#include "xdcinit.h"
#include "xdcutil.h"

typedef struct _rdc_canvas_t{
	xhand_head head;	// reservered for xhand_t

	int dev;			// canvas type 
	res_dc_t dc;		// memory draw context 
	res_bmp_t bmp;		// memory view
	long ptxoff;		// view x-offset
	long ptyoff;		// view y-offset
	long ptwidth;		// view width
	long ptheight;		// view height
	float htpertm;		// horz points per millimetre
	float vtpertm;		// vert points per millimetre 
	long offsetx;		// canvas horz offset
	long offsety;		// canvas vert offset
	float scale;		// canvas scale
}rdc_canvas_t;


void screen_point_size(long* pw, long* ph)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_screen_size)(pw, ph);
}

xhand_t create_display_canvas(res_dc_t rdc)
{
	rdc_canvas_t* pcanv;
	if_context_t* pif;
	dev_cap_t dev = { 0 };

	pif = PROCESS_CONTEXT_INTERFACE;

	pcanv = (rdc_canvas_t*)xmem_alloc(sizeof(rdc_canvas_t));
	pcanv->head.tag = _HANDLE_CANVAS;
	pcanv->head.cbs = sizeof(rdc_canvas_t);

	pcanv->dev = CANV_DISPLAY;

	if (rdc)
		pcanv->dc = (*pif->pf_create_compatible_context)(rdc);
	else
		pcanv->dc = (*pif->pf_create_display_context)();

	(*pif->pf_get_device_caps)(rdc, &dev);

	pcanv->scale = 1.0;
	pcanv->htpertm = (float)((float)dev.horz_pixels * INCHPERTM);
	pcanv->vtpertm = (float)((float)dev.vert_pixels * INCHPERTM);
	pcanv->offsetx = 0;
	pcanv->offsety = 0;

	(*pif->pf_delete_context)(pcanv->dc);
	pcanv->dc = NULL;

	return (xhand_t)pcanv;
}

void destroy_display_canvas(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	xmem_free(pcanv);
}

res_dc_t begin_display_paint(xhand_t canv,res_dc_t rdc,long width,long height)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;
	res_bmp_t bmp;
	dev_cap_t dev = { 0 };
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	assert(pcanv);
	assert(!pcanv->dc);

	bmp = (*pif->pf_create_compatible_bitmap)(rdc, width, height);
	assert(bmp);

	pcanv->dc = (*pif->pf_create_compatible_context)(rdc);
	assert(pcanv->dc);

	pcanv->bmp = (res_bmp_t)(*pif->pf_select_object)(pcanv->dc,bmp);

	return pcanv->dc;
}

void end_display_paint(xhand_t canv,res_dc_t rdc,const xrect_t* pxr)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;
	res_bmp_t bmp;
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_render_context)(pcanv->dc, pxr->x, pxr->y, rdc, pxr->x, pxr->y, pxr->w, pxr->h);

	bmp = (res_bmp_t)(*pif->pf_select_object)(pcanv->dc, pcanv->bmp);
	(*pif->pf_delete_bitmap)(bmp);

	(*pif->pf_delete_context)(pcanv->dc);
	pcanv->dc = NULL;
}

res_dc_t create_printer_dc(const wchar_t* printer,const printer_devmode_t* pmod)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_printer_context)(printer, pmod);
}

xhand_t create_printer_canvas(res_dc_t rdc)
{
	rdc_canvas_t* pcanv;
	dev_cap_t dev = { 0 };

	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_get_device_caps)(rdc, &dev);
	pcanv = (rdc_canvas_t*)xmem_alloc(sizeof(rdc_canvas_t));
	pcanv->head.tag = _HANDLE_CANVAS;
	pcanv->head.cbs = sizeof(rdc_canvas_t);

	pcanv->dev = CANV_PRINTER;

	pcanv->dc = rdc;
	pcanv->scale = 1.0;
	pcanv->htpertm = (float)((float)dev.horz_pixels * INCHPERTM);
	pcanv->vtpertm = (float)((float)dev.vert_pixels * INCHPERTM);

	pcanv->offsetx = (long)((float)dev.horz_offset / pcanv->htpertm);
	pcanv->offsety = (long)((float)dev.vert_offset / pcanv->vtpertm);

	return (xhand_t)pcanv;
}


void  destroy_printer_canvas(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	assert(pcanv->dev == CANV_PRINTER);

	(*pif->pf_delete_context)(pcanv->dc);

	xmem_free(pcanv);
}

void  begin_page(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	assert(pcanv->dev == CANV_PRINTER);

	(*pif->pf_begin_page)(pcanv->dc);
}

void  end_page(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	assert(pcanv->dev == CANV_PRINTER);

	(*pif->pf_end_page)(pcanv->dc);
}

void  begin_doc(xhand_t canv,const wchar_t* docname)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	assert(pcanv->dev == CANV_PRINTER);

	(*pif->pf_begin_doc)(pcanv->dc,docname);
}

void  end_doc(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	assert(pcanv->dev == CANV_PRINTER);

	(*pif->pf_end_doc)(pcanv->dc);
}

float get_canvas_scale(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	assert(pcanv);

	return pcanv->scale;
}

void set_canvas_scale(xhand_t canv,float scale)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	assert(pcanv);

	pcanv->scale = scale;
}

long pt_to_tm(xhand_t canv, long pt, bool_t horz)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	if(horz)
		return (long)((float)pt / (float)(pcanv->htpertm * pcanv->scale))- pcanv->offsetx;
	else
		return (long)((float)pt / (float)(pcanv->vtpertm * pcanv->scale)) - pcanv->offsety;
}

long tm_to_pt(xhand_t canv, long tm, bool_t horz)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	if(horz)
		return (long)(((float)tm * pcanv->scale + (float)pcanv->offsetx) * pcanv->htpertm);
	else
		return (long)(((float)tm * pcanv->scale + (float)pcanv->offsety) * pcanv->vtpertm);
}

void rect_tm_to_pt(xhand_t canv, xrect_t* pxr)
{
	long left, right, top, bottom;

	if (pxr->w < 0)
	{
		left = tm_to_pt(canv, pxr->x + pxr->w, 1);
		right = tm_to_pt(canv, pxr->x, 1);
	}
	else
	{
		left = tm_to_pt(canv, pxr->x, 1);
		right = tm_to_pt(canv, pxr->x + pxr->w, 1);
	}

	if (pxr->h < 0)
	{
		top = tm_to_pt(canv, pxr->y + pxr->h, 0);
		bottom = tm_to_pt(canv, pxr->y, 0);
	}
	else
	{
		top = tm_to_pt(canv, pxr->y, 0);
		bottom = tm_to_pt(canv, pxr->y + pxr->h, 0);
	}

	pxr->x = left;
	pxr->y = top;
	pxr->w = right - left;
	pxr->h = bottom - top;
}

void rect_pt_to_tm(xhand_t canv, xrect_t* pxr)
{
	long left, right, top, bottom;

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

	pxr->x = left;
	pxr->y = top;
	pxr->w = right - left;
	pxr->h = bottom - top;
}

void point_tm_to_pt(xhand_t canv, xpoint_t* ppt,int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		ppt[i].x = tm_to_pt(canv, ppt[i].x, 1);
		ppt[i].y = tm_to_pt(canv, ppt[i].y, 0);
	}
}

void point_pt_to_tm(xhand_t canv, xpoint_t* ppt,int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		ppt[i].x = pt_to_tm(canv, ppt[i].x, 1);
		ppt[i].y = pt_to_tm(canv, ppt[i].y, 0);
	}
}

void set_canvas_horz_feed(xhand_t canv,long xoff)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	assert(pcanv && pcanv->dev != CANV_PRINTER);

	pcanv->offsetx = xoff;
}

long get_canvas_horz_feed(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	return pcanv->offsetx;
}

void set_canvas_vert_feed(xhand_t canv,long yoff)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	assert(pcanv && pcanv->dev != CANV_PRINTER);

	pcanv->offsety = yoff;
}

long get_canvas_vert_feed(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	return pcanv->offsety;
}

res_dc_t get_canvas_dc(xhand_t canv)
{
	rdc_canvas_t* pcanv = (rdc_canvas_t*)canv;

	return (res_dc_t)pcanv->dc;
}
