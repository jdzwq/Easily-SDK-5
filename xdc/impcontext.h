/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdc display context document

	@module	impctx.h | interface file

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

#include "xdcdef.h"

#ifdef XDU_SUPPORT_CONTEXT

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION create_display_context: create display device context.
@INPUT wt: the widget, if NULL created for screen.
@RETURN res_ctx_t: if succeeds return device context, fails return NULL.
*/
EXP_API res_ctx_t create_display_context(res_win_t wt);

/*
@FUNCTION create_compatible_context: create memory context compatiable with device context.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT width: the memory context width in pixel.
@INPUT height: the memory context height in pixel.
@RETURN res_ctx_t: if succeeds return memory context, fails return NULL.
*/
EXP_API res_ctx_t create_compatible_context(res_ctx_t rdc, int width, int height);

/*
@FUNCTION destroy_context: destroy device or memory context.
@INPUT res_ctx_t rdc: device or memory context resource handle.
@RETURN void: none.
*/
EXP_API void destroy_context(res_ctx_t rdc);

/*
@FUNCTION render_context: render source memory context bitmap into destination device context.
@INPUT res_ctx_t src: source memory context resource handle.
@INPUT int srcx: source bitmap start x coordinate.
@INPUT int srcy: source bitmap start y coordinate.
@INPUT res_ctx_t dst: destination device context resource handle.
@INPUT int dstx: destination bitmap start x coordinate.
@INPUT int dsty: destination bitmap start y coordinate.
@INPUT int dstw: destination bitmap width.
@INPUT int dsth: destination bitmap height.
@RETURN void: none.
*/
EXP_API void render_context(res_ctx_t src, int srcx, int srcy, res_ctx_t dst, int dstx, int dsty, int dstw, int dsth);

/*
@FUNCTION get_device_caps: get device context capabilities.
@INPUT res_ctx_t rdc: device or memory context resource handle.
@OUTPUT dev_cap_t* pcap: device capabilities struct for returning information.
@RETURN void: none.
*/
EXP_API void get_device_caps(res_ctx_t rdc, dev_cap_t* pcap);

/*
@FUNCTION text_mm_metric: get character millimeter metrics according to the font.
@INPUT res_ctx_t rdc: device or memory context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@OUTPUT xsize_t* pxs: the size struct for returning character millimeter metrics.
@RETURN void: none.
*/
EXP_API void text_mm_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs);

/*
@FUNCTION text_pt_metric: get character points metrics according to the font.
@INPUT res_ctx_t rdc: device or memory context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@OUTPUT xsize_t* pxs: the size struct for returning character millimeter metrics.
@RETURN void: none.
*/
EXP_API void text_pt_metric(res_ctx_t rdc, const xfont_t* pxf, xsize_t* pxs);

/*
@FUNCTION text_mm_size: get text millimeter metrics according to the font.
@INPUT res_ctx_t rdc: device or memory context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const tchar_t* txt: the string token.
@INPUT int len: the string length in characters, not include terminate character.
@OUTPUT xsize_t* pxs: the size struct for returning text millimeter metrics.
@RETURN void: none.
*/
EXP_API void text_mm_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

/*
@FUNCTION text_pt_size: get text points metrics according to the font.
@INPUT res_ctx_t rdc: device or memory context resource handle.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const tchar_t* txt: the string token.
@INPUT int len: the string length in characters, not include terminate character.
@OUTPUT xsize_t* pxs: the size struct for returning text points metrics.
@RETURN void: none.
*/
EXP_API void text_pt_size(res_ctx_t rdc, const xfont_t* pxf, const tchar_t* txt, int len, xsize_t* pxs);

/*
@FUNCTION pt_per_mm: calc points per millimeter.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT bool_t horz: nonzero for horizonal calcing, zero for vertical calcing.
@RETURN float: float millimeter value.
*/
EXP_API float pt_per_mm(res_ctx_t rdc, bool_t horz);

/*
@FUNCTION cast_pt_to_mm: mapping points to millimeter.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT int pt: the span value in points.
@INPUT bool_t horz: nonzero for horizonal mapping, zero for vertical mapping.
@RETURN float: float millimeter value.
*/
EXP_API float cast_pt_to_mm(res_ctx_t rdc, int pt, bool_t horz);

/*
@FUNCTION cast_mm_to_pt: mapping millimeter to points.
@INPUT res_ctx_t rdc: device context resource handle.
@INPUT float mm: the span value in millimeter.
@INPUT bool_t horz: nonzero for horizonal mapping, zero for vertical mapping.
@RETURN float: float millimeter value.
*/
EXP_API int cast_mm_to_pt(res_ctx_t rdc, float mm, bool_t horz);

#ifdef XDU_SUPPORT_CONTEXT_PRINTER

/*
@FUNCTION create_printer_context: create printer device context.
@INPUT const dev_prn_t* pmod: the printer device mode.
@RETURN res_ctx_t: if succeeds return device context, fails return NULL.
*/
EXP_API res_ctx_t create_printer_context(const dev_prn_t* pmod);

/*
@FUNCTION destroy_printer_context: destroy printer device context.
@INPUT res_ctx_t rdc: the printer device context.
@RETURN void: none.
*/
EXP_API void destroy_printer_context(res_ctx_t rdc);

/*
@FUNCTION default_printer_mode: get default printer device mode.
@OUTPUT dev_prn_t* pmod: for returning device mode.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t default_printer_mode(dev_prn_t* pmod);

/*
@FUNCTION setup_printer_mode: show printer setup dialog and set printer mode.
@INPUT res_win_t wnd: the printer setup dialog owner window.
@OUTPUT dev_prn_t* pmod: for returning device mode.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t setup_printer_mode(res_win_t wnd, dev_prn_t* pmod);

/*
@FUNCTION begin_page: start printing a page.
@INPUT res_ctx_t rdc: the printer device context.
@RETURN void: none.
*/
EXP_API void  begin_page(res_ctx_t rdc);

/*
@FUNCTION end_page: end printing a page.
@INPUT res_ctx_t rdc: the printer device context.
@RETURN void: none.
*/
EXP_API void  end_page(res_ctx_t rdc);

/*
@FUNCTION begin_doc: start printing a document.
@INPUT res_ctx_t rdc: the printer device context.
@INPUT const tchar_t* docname: the document title.
@RETURN void: none.
*/
EXP_API void  begin_doc(res_ctx_t rdc, const tchar_t* docname);

/*
@FUNCTION end_doc: end printing a document.
@INPUT res_ctx_t rdc: the printer device context.
@RETURN void: none.
*/
EXP_API void  end_doc(res_ctx_t rdc);

#endif //XDU_SUPPORT_CONTEXT_PRINTER

#ifdef XDU_SUPPORT_CONTEXT_REGION
/*
@FUNCTION create_region: create region object.
@INPUT const tchar_t* shape: the shape type, eg: ATTR_SHAPE_*.
@INPUT const xrect_t* pxr: the region rect.
@RETURN res_rgn_t: if succeeds return region resource handle, fails return NULL.
*/
EXP_API res_rgn_t create_region(const tchar_t* shape, const xrect_t* pxr);

/*
@FUNCTION delete_region: free region object.
@INPUT res_rgn_t rgn: the region resource handle.
@RETURN void: none.
*/
EXP_API void	delete_region(res_rgn_t rgn);

/*
@FUNCTION pt_in_region: test if the point inside the region.
@INPUT res_rgn_t rgn: the region resource handle.
@INPUT const xpoint_t* ppt: the point to test.
@RETURN bool_t: if inside return nonzero, otherwise return zero.
*/
EXP_API bool_t	pt_in_region(res_rgn_t rgn, const xpoint_t* ppt);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*XDU_SUPPORT_CONTEXT*/

#endif /*IMPCONTEXT_H*/