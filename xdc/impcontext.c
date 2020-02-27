/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc printer context document

	@module	impctx.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
***********************************************************************/

/**********************************************************************
This program is xmem_free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "impcontext.h"
#include "xdcinit.h"

#ifdef XDU_SUPPORT_CONTEXT

int font_size(res_ctx_t rdc, int height)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_font_size)(rdc, height);
}

float pt_per_mm(res_ctx_t rdc, bool_t horz)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_pt_per_mm)(rdc, horz);
}

float cast_pt_to_mm(res_ctx_t rdc, int pt, bool_t horz)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_cast_pt_to_mm)(rdc, pt, horz);
}

int cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_cast_mm_to_pt)(rdc, mm, horz);

}

void text_mm_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	if_context_t* pif;
	float fx, fy;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_text_mm_size)(rdc, pxf, txt, len, &fx, &fy);
	pxs->fx = fx;
	pxs->fy = fy;
}

void text_mm_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	if_context_t* pif;
	float fx, fy;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_text_mm_metric)(rdc, pxf, &fx, &fy);
	pxs->fx = fx;
	pxs->fy = fy;
}

void text_pt_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs)
{
	if_context_t* pif;
	int cx, cy;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_text_pt_metric)(rdc, pxf, &cx, &cy);
	pxs->cx = cx;
	pxs->cy = cy;
}

void text_pt_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs)
{
	if_context_t* pif;
	int cx, cy;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_text_pt_size)(rdc, pxf, txt, len, &cx, &cy);
	pxs->cx = cx;
	pxs->cy = cy;
}

/*************************************************************************************************/
res_ctx_t create_display_context()
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_display_context)();
}

res_ctx_t create_compatible_context(res_ctx_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_compatible_context)(rdc);
}

void destroy_context(res_ctx_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_destroy_context)(rdc);
}

void render_context(res_ctx_t src, int srcx, int srcy, res_ctx_t dst, int dstx, int dsty, int dstw, int dsth)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_render_context)(src, srcx, srcy, dst, dstx, dsty, dstw, dsth);
}

void get_device_caps(res_ctx_t rdc, dev_cap_t* pcap)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_get_device_caps)(rdc, pcap);
}

res_pmp_t select_pixmap(res_ctx_t rdc, res_pmp_t obj)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_select_pixmap)(rdc, obj);
}

res_pmp_t create_compatible_pixmap(res_ctx_t rdc, int cx, int cy)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_compatible_pixmap)(rdc, cx, cy);
}

void destroy_pixmap(res_pmp_t pmp)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_destroy_pixmap)(pmp);
}

#ifdef XDU_SUPPORT_CONTEXT_REGION
res_rgn_t create_region(const tchar_t* shape, const xrect_t* pxr)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_region)(shape, pxr);
}

void delete_region(res_rgn_t rgn)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_delete_region)(rgn);
}

bool_t pt_in_region(res_rgn_t rgn, const xpoint_t* ppt)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_pt_in_region)(rgn, ppt);
}
#endif

#ifdef XDU_SUPPORT_CONTEXT_PRINTER

res_ctx_t create_printer_context(const dev_prn_t* pmod)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_printer_context)(pmod);
}

void destroy_printer_context(res_ctx_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_destroy_printer_context)(rdc);
}

bool_t default_printer_mode(dev_prn_t* pmod)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_default_printer_mode)(pmod);
}

bool_t setup_printer_mode(res_win_t wnd, dev_prn_t* pmod)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_setup_printer_mode)(wnd, pmod);
}

void  begin_page(res_ctx_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_begin_page)(rdc);
}

void  end_page(res_ctx_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_end_page)(rdc);
}

void  begin_doc(res_ctx_t rdc, const tchar_t* docname)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_begin_doc)(rdc, docname);
}

void  end_doc(res_ctx_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_end_doc)(rdc);
}

#endif //XDU_SUPPORT_CONTEXT_PRINTER


#endif /*XDU_SUPPORT_CONTEXT*/

