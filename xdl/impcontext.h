/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl display context document

	@module	impctx.h | display context interface file

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

#ifndef _IMPCONTEXT_H
#define _IMPCONTEXT_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_CONTEXT

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API void text_mm_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs);

XDL_API void text_mm_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

XDL_API float pt_per_mm(res_ctx_t rdc, bool_t horz);

XDL_API void text_pt_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

XDL_API void text_pt_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs);

XDL_API float cast_pt_to_mm(res_ctx_t rdc, long pt, bool_t horz);

XDL_API long cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz);

XDL_API int font_size(res_ctx_t rdc, long height);

XDL_API res_ctx_t create_display_context(void);

XDL_API res_ctx_t create_compatible_context(res_ctx_t rdc);

XDL_API void destroy_context(res_ctx_t rdc);

XDL_API void render_context(res_ctx_t src, long srcx, long srcy, res_ctx_t dst, long dstx, long dsty, long dstw, long dsth);

XDL_API void get_device_caps(res_ctx_t rdc, dev_cap_t* pcap);

#ifdef XDK_SUPPORT_CONTEXT_REGION
XDL_API res_rgn_t create_region(const tchar_t* shape, const xrect_t* pxr);

XDL_API void	delete_region(res_rgn_t rgn);

XDL_API bool_t	pt_in_region(res_rgn_t rgn, const xpoint_t* ppt);
#endif

#ifdef XDK_SUPPORT_CONTEXT_PRINTER

XDL_API res_ctx_t create_printer_context(const dev_prn_t* pmod);

XDL_API void destroy_printer_context(res_ctx_t rdc);

XDL_API bool_t default_printer_mode(dev_prn_t* pmod);

XDL_API bool_t setup_printer_mode(res_win_t wnd, dev_prn_t* pmod);

XDL_API void  begin_page(res_ctx_t rdc);

XDL_API void  end_page(res_ctx_t rdc);

XDL_API void  begin_doc(res_ctx_t rdc, const tchar_t* docname);

XDL_API void  end_doc(res_ctx_t rdc);

#endif //XDK_SUPPORT_CONTEXT_PRINTER

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*IMPCONTEXT_H*/