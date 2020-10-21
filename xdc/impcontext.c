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

float pt_per_mm(visual_t rdc, bool_t horz)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_pt_per_mm)(rdc, horz);
}

float pt_per_in(visual_t rdc, bool_t horz)
{
	return (float)PDPERINCH;
}

void cast_pt_to_mm(visual_t rdc, bool_t horz, xspan_t* pxn)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_cast_pt_to_mm)(rdc, horz, pxn);
}

void cast_mm_to_pt(visual_t rdc, bool_t horz, xspan_t* pxn)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_cast_mm_to_pt)(rdc, horz, pxn);

}

/*************************************************************************************************/

visual_t create_display_context(res_win_t wt)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_display_context)(wt);
}

visual_t create_compatible_context(visual_t rdc, int width, int height)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_compatible_context)(rdc, width, height);
}

void destroy_context(visual_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_destroy_context)(rdc);
}

void render_context(visual_t src, int srcx, int srcy, visual_t dst, int dstx, int dsty, int dstw, int dsth)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_render_context)(src, srcx, srcy, dst, dstx, dsty, dstw, dsth);
}

void get_device_caps(visual_t rdc, dev_cap_t* pcap)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_get_device_caps)(rdc, pcap);
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

visual_t create_printer_context(const dev_prn_t* pmod)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	return (*pif->pf_create_printer_context)(pmod);
}

void destroy_printer_context(visual_t rdc)
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

void  begin_page(visual_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_begin_page)(rdc);
}

void  end_page(visual_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_end_page)(rdc);
}

void  begin_doc(visual_t rdc, const tchar_t* docname)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_begin_doc)(rdc, docname);
}

void  end_doc(visual_t rdc)
{
	if_context_t* pif;

	pif = PROCESS_CONTEXT_INTERFACE;

	(*pif->pf_end_doc)(rdc);
}

#endif //XDU_SUPPORT_CONTEXT_PRINTER


#endif /*XDU_SUPPORT_CONTEXT*/

