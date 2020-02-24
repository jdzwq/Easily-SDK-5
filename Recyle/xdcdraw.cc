/***********************************************************************
	Easily xdc v4.0

	(c) 2003-2015 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc draw document

	@module	xdcdraw.c | draw implement file

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

#include "xdcdraw.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdccanv.h"

void draw_line(xhand_t canv,const xpen_t* pxp,long x1, long y1, long x2, long y2)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xpoint_t pt[2];

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;
	point_tm_to_pt(canv, pt, 2);

	(*pif->pf_gdi_draw_line)(rdc,pxp,pt[0].x,pt[0].y,pt[1].x,pt[1].y);
}

void draw_3dline(xhand_t canv,const xpen_t* pxp,long x1, long y1, long x2, long y2)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xpoint_t pt[2];

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;
	point_tm_to_pt(canv, pt, 2);

	(*pif->pf_gdi_draw_3dline)(rdc, pxp, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
}

void draw_polyline(xhand_t canv,const xpen_t* pxp,const xpoint_t* ppt,int n)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xpoint_t* pt;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	pt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * n);
	memcpy((void*)pt, (void*)ppt, sizeof(xpoint_t) * n);

	point_tm_to_pt(canv, pt, n);
	
	(*pif->pf_gdi_draw_polyline)(rdc,pxp,pt,n);

	xmem_free(pt);
}

void draw_polygon(xhand_t canv,const xpen_t* pxp,const xpoint_t* ppt,int n)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xpoint_t* pt;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	pt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * n);
	memcpy((void*)pt, (void*)ppt, sizeof(xpoint_t) * n);

	point_tm_to_pt(canv, pt, n);

	(*pif->pf_gdi_draw_polygon)(rdc,pxp,pt,n);

	xmem_free(pt);
}

void fill_polygon(xhand_t canv,const xbrush_t* pxb,const xpoint_t* ppt,int n)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xpoint_t* pt;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	pt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t) * n);
	memcpy((void*)pt, (void*)ppt, sizeof(xpoint_t) * n);

	point_tm_to_pt(canv, pt, n);

	(*pif->pf_gdi_fill_polygon)(rdc,pxb,pt,n);

	xmem_free(pt);
}

void draw_3drect(xhand_t canv, const xpen_t* pxp, const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_3drect)(rdc, pxp, &xr);
}

void draw_rect(xhand_t canv,const xpen_t* pxp,const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_rect)(rdc,pxp,&xr);
}

void fill_rect(xhand_t canv,const xbrush_t* pxb,const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_fill_rect)(rdc,pxb,&xr);
}

void gradient_rect(xhand_t canv, const xgradi_t* pxg, const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_gradinet_rect)(rdc, pxg, &xr);
}

void draw_round(xhand_t canv,const xpen_t* pxp,const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_round)(rdc,pxp,&xr);
}

void fill_round(xhand_t canv,const xbrush_t* pxb,const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_fill_round)(rdc,pxb,&xr);
}

void draw_ellipse(xhand_t canv,const xpen_t* pxp,const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_ellipse)(rdc,pxp,&xr);
}

void fill_ellipse(xhand_t canv,const xbrush_t* pxb,const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_fill_ellipse)(rdc,pxb,&xr);
}

void draw_pie(xhand_t canv,const xpen_t* pxp,const xrect_t* pxr,double fang,double tang)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_pie)(rdc,pxp,&xr,fang,tang);
}

void fill_pie(xhand_t canv,const xbrush_t* pxb,const xrect_t* pxr,double fang,double tang)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_fill_pie)(rdc,pxb,&xr,fang,tang);
}

void draw_arrow(xhand_t canv,const xpen_t* pxp,const xrect_t* pxr,int alen,double arc)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_arrow)(rdc,pxp,&xr,alen,arc);
}

void fill_arrow(xhand_t canv,const xbrush_t* pxb,const xrect_t* pxr,int alen,double arc)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_fill_arrow)(rdc,pxb,&xr,alen,arc);
}

void draw_shape(xhand_t canv,const xpen_t* pxp,const xrect_t* pxr,const wchar_t* shape)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_shape)(rdc,pxp,&xr,shape);
}

void fill_shape(xhand_t canv,const xbrush_t* pxb,const xrect_t* pxr,const wchar_t* shape)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_fill_shape)(rdc,pxb,&xr,shape);
}

void draw_text(xhand_t canv,const xfont_t* pxf,const xface_t* pxa,const xrect_t* pxr,const wchar_t* txt,int len)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_text)(rdc,pxf,pxa,&xr,txt,len);
}

void calc_text(xhand_t canv, const xfont_t* pxf, const xface_t* pxa, const wchar_t* txt, int len, xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_calc_text)(rdc, pxf, pxa, txt, len, &xr);

	memcpy((void*)pxr, (void*)&xr, sizeof(xrect_t));
	rect_pt_to_tm(canv, pxr);
}

void draw_image(xhand_t canv,const ximage_t* pxi,const xrect_t* pxr)
{
	res_dc_t rdc = get_canvas_dc(canv);
	xrect_t xr;

	if_graphic_t *pif;

	pif = PROCESS_GRAPHIC_INTERFACE;

	memcpy((void*)&xr, (void*)pxr, sizeof(xrect_t));
	rect_tm_to_pt(canv, &xr);

	(*pif->pf_gdi_draw_image)(rdc,pxi,&xr);
}

void draw_insert(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xpen_t xp;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	center_rect(&xr, size, size);

	draw_line(canv, &xp, xr.x + xr.w / 2, xr.y, xr.x + xr.w / 2, xr.y + xr.h);
	draw_line(canv, &xp, xr.x, xr.y + xr.h / 2, xr.x + xr.w, xr.y + xr.y / 2);
}

void draw_delete(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xpen_t xp;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	center_rect(&xr, size, size);

	draw_line(canv, &xp, xr.x, xr.y, xr.x + xr.w, xr.y + xr.h);
	draw_line(canv, &xp, xr.x, xr.y + xr.h, xr.x + xr.w, xr.y);
}

void draw_close(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	center_rect(&xr, size, size);
	
	default_xfont(&xf);
	xscpy(xf.size, L"9");
	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	format_xcolor(pxc, xa.text_color);

	draw_text(canv, &xf, &xa, prt, L"×", -1);
}

void draw_sum(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	center_rect(&xr, size, size);

	default_xfont(&xf);
	xscpy(xf.size, L"9");
	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	format_xcolor(pxc, xa.text_color);

	draw_text(canv, &xf, &xa, prt, L"∑", -1);
}

void draw_check(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	center_rect(&xr, size, size);

	default_xfont(&xf);
	xscpy(xf.size, L"9");
	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	format_xcolor(pxc, xa.text_color);

	draw_text(canv, &xf, &xa, prt, L"√", -1);
}

void draw_expand(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	center_rect(&xr, size, size);

	default_xfont(&xf);
	xscpy(xf.size, L"9");
	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	format_xcolor(pxc, xa.text_color);

	draw_text(canv, &xf, &xa, prt, L"－", -1);
}

void draw_collapse(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	center_rect(&xr, size, size);

	default_xfont(&xf);
	xscpy(xf.size, L"9");
	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	format_xcolor(pxc, xa.text_color);

	draw_text(canv, &xf, &xa, prt, L"＋", -1);
}

void draw_up(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	center_rect(&xr, size, size);

	default_xfont(&xf);
	xscpy(xf.size, L"9");
	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	format_xcolor(pxc, xa.text_color);

	draw_text(canv, &xf, &xa, prt, L"︿", -1);
}

void draw_down(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xfont_t xf;
	xface_t xa;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	center_rect(&xr, size, size);

	default_xfont(&xf);
	xscpy(xf.size, L"9");
	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	format_xcolor(pxc, xa.text_color);

	draw_text(canv, &xf, &xa, prt, L"﹀", -1);
}

void draw_guider(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xpen_t xp;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	center_rect(&xr, size, size);

	xpoint_t pt[5];

	pt[0].x = xr.x;
	pt[0].y = xr.y;

	pt[1].x = xr.x + xr.w;
	pt[1].y = xr.y + xr.h / 2;

	pt[2].x = xr.x;
	pt[2].y = xr.y + xr.h;

	pt[3].x = xr.x + 4;
	pt[3].y = xr.y + xr.h / 2;

	pt[4].x = xr.x;
	pt[4].y = xr.y;

	draw_polygon(canv, &xp, pt, 5);
}

void draw_folder(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, int size)
{
	xpen_t xp;
	xrect_t xr;

	memcpy((void*)&xr, (void*)prt, sizeof(xrect_t));

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);

	center_rect(&xr, size, size);

	xpoint_t pt[4];

	pt[0].x = xr.x;
	pt[0].y = xr.y + xr.h / 4;

	pt[1].x = xr.x;
	pt[1].y = xr.y;

	pt[2].x = xr.x + xr.w / 2;
	pt[2].y = xr.y;

	pt[3].x = xr.x + 3 * xr.w / 4;
	pt[3].y = xr.y + xr.h / 4;

	draw_polyline(canv, &xp, pt, 4);

	xr.y += xr.h / 4;
	xr.h -= xr.h / 4;

	draw_rect(canv, &xp, &xr);
}

void draw_select(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, byte_t opa)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, L"%d", opa);
	xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DOTTED);

	draw_rect(canv, &xp, prt);
}

void draw_focus(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, byte_t opa)
{
	xpen_t xp;

	default_xpen(&xp);
	format_xcolor(pxc, xp.color);
	xsprintf(xp.opacity, L"%d", opa);

	draw_rect(canv, &xp, prt);
}

void draw_alpha(xhand_t canv, const xcolor_t* pxc, const xrect_t* prt, byte_t opa)
{
	xbrush_t xb;

	default_xbrush(&xb);
	format_xcolor(pxc, xb.color);
	xsprintf(xb.opacity, L"%d", opa);

	fill_rect(canv, &xb, prt);
}
