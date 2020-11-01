/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdu interface document

	@module	xduinf.h | interface file

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

#ifndef _XDUINF_H
#define	_XDUINF_H

#include "xdudef.h"

#ifdef XDU_SUPPORT_BLUT
typedef int(*PF_ENUM_BLUT)(dev_blt_t*, int);
typedef res_file_t(*PF_BLUT_OPEN)(const tchar_t*, int, dword_t);
typedef void(*PF_BLUT_CLOSE)(res_file_t);
typedef dword_t(*PF_BLUT_LISTEN)(res_file_t, async_t*);
typedef bool_t(*PF_BLUT_READ)(res_file_t, void*, dword_t, async_t*);
typedef bool_t(*PF_BLUT_WRITE)(res_file_t, void*, dword_t, async_t*);
typedef bool_t(*PF_BLUT_FLUSH)(res_file_t);

typedef struct _if_blut_t{
	PF_ENUM_BLUT	pf_enum_blut;
	PF_BLUT_LISTEN		pf_blut_listen;
	PF_BLUT_OPEN		pf_blut_open;
	PF_BLUT_CLOSE		pf_blut_close;
	PF_BLUT_READ		pf_blut_read;
	PF_BLUT_WRITE		pf_blut_write;
	PF_BLUT_FLUSH		pf_blut_flush;
}if_blut_t;
#endif

#ifdef XDU_SUPPORT_SHELL
/*shell interface*/
typedef bool_t(*PF_SHELL_GET_RUNPATH)(tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_CURPATH)(tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_APPPATH)(tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_DOCPATH)(tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_TMPPATH)(tchar_t*, int);
#ifdef XDU_SUPPORT_SHELL_DIALOG
typedef bool_t(*PF_SHELL_GET_FILENAME)(res_win_t, const tchar_t*, const tchar_t*, const tchar_t*, bool_t, tchar_t*, int, tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_PATHNAME)(res_win_t, const tchar_t*, bool_t, tchar_t*, int);
#endif

typedef struct _if_shell_t {
	PF_SHELL_GET_RUNPATH	pf_shell_get_runpath;
	PF_SHELL_GET_CURPATH	pf_shell_get_curpath;
	PF_SHELL_GET_APPPATH	pf_shell_get_apppath;
	PF_SHELL_GET_DOCPATH	pf_shell_get_docpath;
	PF_SHELL_GET_TMPPATH	pf_shell_get_tmppath;
#ifdef XDU_SUPPORT_SHELL_DIALOG
	PF_SHELL_GET_FILENAME	pf_shell_get_filename;
	PF_SHELL_GET_PATHNAME	pf_shell_get_pathname;
#endif
}if_shell_t;
#endif /*XDU_SUPPORT_SHELL*/

#ifdef XDU_SUPPORT_CONTEXT
/*context interface*/
typedef int(*PF_CONTEXT_STARTUP)(void);
typedef void(*PF_CONTEXT_CLEANUP)(void);
typedef visual_t(*PF_CREATE_DISPLAY_CONTEXT)(res_win_t);
typedef visual_t(*PF_CREATE_COMPATIBLE_CONTEXT)(visual_t, int, int);
typedef void(*PF_DESTROY_CONTEXT)(visual_t);
typedef void(*PF_RENDER_CONTEXT)(visual_t, int, int, visual_t, int, int, int, int);
typedef void(*PF_GET_DEVICE_CAPS)(visual_t, dev_cap_t*);
typedef float(*PF_PT_PER_MM)(visual_t, bool_t);
typedef void(*PF_TEXT_MM_SIZE)(visual_t, const xfont_t*, const tchar_t*, int, xsize_t*);
typedef void(*PF_TEXT_PT_SIZE)(visual_t, const xfont_t*, const tchar_t*, int, xsize_t*);
typedef void(*PF_TEXT_MM_METRIC)(visual_t, const xfont_t*, xsize_t*);
typedef void(*PF_TEXT_PT_METRIC)(visual_t, const xfont_t*, xsize_t*);
typedef void(*PF_CAST_PT_TO_MM)(visual_t, bool_t, xspan_t*);
typedef void(*PF_CAST_MM_TO_PT)(visual_t, bool_t, xspan_t*);

/*graphic interface*/
typedef void(*PF_GDI_DRAW_LINE)(visual_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_GDI_DRAW_ARC)(visual_t, const xpen_t*, const xpoint_t*, const xpoint_t*, const xsize_t*, bool_t, bool_t);
typedef void(*PF_GDI_DRAW_POLYLINE)(visual_t, const xpen_t*, const xpoint_t*, int);
typedef void(*PF_GDI_DRAW_BEZIER)(visual_t, const xpen_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_GDI_DRAW_CURVE)(visual_t, const xpen_t*, const xpoint_t*, int);
typedef void(*PF_GDI_DRAW_RECT)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_TRIANGLE)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*, const tchar_t*);
typedef void(*PF_GDI_DRAW_PATH)(visual_t, const xpen_t*, const xbrush_t*, const tchar_t*, const xpoint_t*);
typedef void(*PF_GDI_GRADIENT_RECT)(visual_t, const xcolor_t* xc_brim, const xcolor_t* xc_core, const tchar_t* gradient, const xrect_t*);
typedef void(*PF_GDI_ALPHABLEND_RECT)(visual_t, const xcolor_t*, const xrect_t*, int);
typedef void(*PF_GDI_DRAW_POLYGON)(visual_t, const xpen_t*, const xbrush_t*, const xpoint_t*, int);
typedef void(*PF_GDI_DRAW_ROUND)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_ELLIPSE)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_FAN)(visual_t, const xpen_t*, const xbrush_t*, const xpoint_t*, const xsize_t*, double, double);
typedef void(*PF_GDI_CALC_FAN)(visual_t, const xpoint_t*, const xsize_t*, double, double, xpoint_t*, int);
typedef void(*PF_GDI_DRAW_PIE)(visual_t, const xpen_t*, const xbrush_t*, const xpoint_t*, const xsize_t*, double, double);
typedef void(*PF_GDI_DRAW_ARROW)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*, const xspan_t*, double);
typedef void(*PF_GDI_DRAW_TEXT)(visual_t, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int);
typedef void(*PF_GDI_TEXT_OUT)(visual_t, const xfont_t*, const xpoint_t*, const tchar_t*, int);
typedef void(*PF_GDI_DRAW_PICTURE)(visual_t, const tchar_t*, const tchar_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_IMAGE)(visual_t, bitmap_t, const tchar_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_BITMAP)(visual_t, bitmap_t, const xpoint_t*);
typedef void(*PF_GDI_DRAW_ICON)(visual_t, const xcolor_t*, const xrect_t*, const tchar_t*);
typedef void(*PF_GDI_FILL_REGION)(visual_t, const xbrush_t*, res_rgn_t);

typedef void(*PF_GDI_EXCLIP_RECT)(visual_t, const xrect_t*);
typedef void(*PF_GDI_INCLIP_RECT)(visual_t, const xrect_t*);
typedef void(*PF_GDI_TEXT_RECT)(visual_t, const xfont_t*, const xface_t*, const tchar_t*, int, xrect_t*);
typedef void(*PF_GDI_TEXT_SIZE)(visual_t, const xfont_t*, const tchar_t*, int, xsize_t*);
typedef void(*PF_GDI_TEXT_METRIC)(visual_t, const xfont_t*, xsize_t*);

#ifdef XDU_SUPPORT_CONTEXT_REGION
typedef res_rgn_t(*PF_CREATE_REGION)(const tchar_t*, const xrect_t*);
typedef void(*PF_DELETE_REGION)(res_rgn_t);
typedef bool_t(*PF_PT_IN_REGION)(res_rgn_t, const xpoint_t*);
#endif

#ifdef XDU_SUPPORT_CONTEXT_BITMAP
/*bitmap interface*/
typedef void(*PF_DESTROY_BITMAP)(bitmap_t);
typedef void(*PF_GET_BITMAP_SIZE)(bitmap_t, int*, int*);

typedef bitmap_t(*PF_CREATE_CONTEXT_BITMAP)(visual_t);
typedef bitmap_t(*PF_CREATE_COLOR_BITMAP)(visual_t, const xcolor_t*, int, int);
typedef bitmap_t(*PF_CREATE_PATTERN_BITMAP)(visual_t, const xcolor_t*, const xcolor_t*, int, int, const tchar_t*);
typedef bitmap_t(*PF_CREATE_GRADIENT_BITMAP)(visual_t, const xcolor_t*, const xcolor_t*, int, int, const tchar_t*);
typedef bitmap_t(*PF_CREATE_CODE128_BITMAP)(visual_t, int, int, const unsigned char*, dword_t, const tchar_t*);
typedef bitmap_t(*PF_CREATE_PDF417_BITMAP)(visual_t, int, int, const unsigned char*, dword_t, int, int);
typedef bitmap_t(*PF_CREATE_QRCODE_BITMAP)(visual_t, int, int, const unsigned char*, dword_t, int, int);
typedef bitmap_t(*PF_CREATE_STORAGE_BITMAP)(visual_t, const tchar_t*);
typedef bitmap_t(*PF_LOAD_BITMAP_FROM_BYTES)(visual_t, const unsigned char*, dword_t);
typedef dword_t(*PF_SAVE_BITMAP_TO_BYTES)(visual_t, bitmap_t, unsigned char*, dword_t);
typedef dword_t(*PF_GET_BITMAP_BYTES)(bitmap_t);
typedef bitmap_t(*PF_LOAD_BITMAP_FROM_THUMB)(visual_t, const tchar_t*);
typedef bitmap_t(*PF_LOAD_BITMAP_FROM_ICON)(visual_t, const tchar_t*);
#endif

#ifdef XDU_SUPPORT_CONTEXT_PRINTER
/*printer interface*/
typedef bool_t(*PF_DEFAULT_PRINTER_MODE)(dev_prn_t*);
typedef bool_t(*PF_SETUP_PRINTER_MODE)(res_win_t, dev_prn_t*);
typedef visual_t(*PF_CREATE_PRINTER_CONTEXT)(const dev_prn_t*);
typedef void(*PF_DESTROY_PRINTER_CONTEXT)(visual_t);
typedef void(*PF_BEGIN_PAGE)(visual_t);
typedef void(*PF_END_PAGE)(visual_t);
typedef void(*PF_BEGIN_DOC)(visual_t, const tchar_t*);
typedef void(*PF_END_DOC)(visual_t);
#endif

typedef struct _if_context_t{
	PF_CONTEXT_STARTUP			pf_context_startup;
	PF_CONTEXT_CLEANUP			pf_context_cleanup;

	PF_CREATE_COMPATIBLE_CONTEXT	pf_create_compatible_context;
	PF_CREATE_DISPLAY_CONTEXT	pf_create_display_context;
	PF_DESTROY_CONTEXT			pf_destroy_context;
	PF_RENDER_CONTEXT			pf_render_context;

	PF_GET_DEVICE_CAPS			pf_get_device_caps;
	PF_TEXT_MM_SIZE		pf_text_mm_size;
	PF_TEXT_PT_SIZE		pf_text_pt_size;
	PF_TEXT_MM_METRIC	pf_text_mm_metric;
	PF_TEXT_PT_METRIC	pf_text_pt_metric;
	PF_CAST_PT_TO_MM	pf_cast_pt_to_mm;
	PF_CAST_MM_TO_PT	pf_cast_mm_to_pt;
	PF_PT_PER_MM		pf_pt_per_mm;

	PF_GDI_DRAW_LINE		pf_gdi_draw_line;
	PF_GDI_DRAW_POLYLINE	pf_gdi_draw_polyline;
	PF_GDI_DRAW_POLYGON		pf_gdi_draw_polygon;
	PF_GDI_DRAW_BEZIER		pf_gdi_draw_bezier;
	PF_GDI_DRAW_CURVE		pf_gdi_draw_curve;
	PF_GDI_DRAW_TRIANGLE	pf_gdi_draw_triangle;
	PF_GDI_DRAW_RECT		pf_gdi_draw_rect;
	PF_GDI_DRAW_PATH		pf_gdi_draw_path;
	PF_GDI_GRADIENT_RECT	pf_gdi_gradient_rect;
	PF_GDI_ALPHABLEND_RECT	pf_gdi_alphablend_rect;
	PF_GDI_DRAW_ROUND		pf_gdi_draw_round;
	PF_GDI_DRAW_ELLIPSE		pf_gdi_draw_ellipse;
	PF_GDI_DRAW_FAN			pf_gdi_draw_fan;
	PF_GDI_CALC_FAN			pf_gdi_calc_fan;
	PF_GDI_DRAW_PIE			pf_gdi_draw_pie;
	PF_GDI_DRAW_ARC			pf_gdi_draw_arc;
	PF_GDI_DRAW_ARROW		pf_gdi_draw_arrow;
	PF_GDI_DRAW_TEXT		pf_gdi_draw_text;
	PF_GDI_TEXT_OUT			pf_gdi_text_out;
	PF_GDI_FILL_REGION		pf_gdi_fill_region;
	PF_GDI_DRAW_PICTURE		pf_gdi_draw_picture;
	PF_GDI_DRAW_IMAGE		pf_gdi_draw_image;
	PF_GDI_DRAW_BITMAP		pf_gdi_draw_bitmap;
	PF_GDI_DRAW_ICON		pf_gdi_draw_icon;
	PF_GDI_TEXT_RECT		pf_gdi_text_rect;
	PF_GDI_TEXT_SIZE		pf_gdi_text_size;
	PF_GDI_TEXT_METRIC		pf_gdi_text_metric;
	PF_GDI_EXCLIP_RECT		pf_gdi_exclip_rect;
	PF_GDI_INCLIP_RECT		pf_gdi_inclip_rect;

#ifdef XDU_SUPPORT_CONTEXT_REGION
	PF_CREATE_REGION			pf_create_region;
	PF_DELETE_REGION			pf_delete_region;
	PF_PT_IN_REGION				pf_pt_in_region;
#endif

#ifdef XDU_SUPPORT_CONTEXT_BITMAP
	PF_DESTROY_BITMAP			pf_destroy_bitmap;
	PF_GET_BITMAP_SIZE			pf_get_bitmap_size;

	PF_CREATE_CONTEXT_BITMAP	pf_create_context_bitmap;
	PF_CREATE_COLOR_BITMAP		pf_create_color_bitmap;
	PF_CREATE_PATTERN_BITMAP	pf_create_pattern_bitmap;
	PF_CREATE_GRADIENT_BITMAP	pf_create_gradient_bitmap;
	PF_CREATE_CODE128_BITMAP	pf_create_code128_bitmap;
	PF_CREATE_PDF417_BITMAP		pf_create_pdf417_bitmap;
	PF_CREATE_QRCODE_BITMAP		pf_create_qrcode_bitmap;
	PF_CREATE_STORAGE_BITMAP	pf_create_storage_bitmap;
	PF_LOAD_BITMAP_FROM_BYTES	pf_load_bitmap_from_bytes;
	PF_SAVE_BITMAP_TO_BYTES		pf_save_bitmap_to_bytes;
	PF_GET_BITMAP_BYTES			pf_get_bitmap_bytes;

	PF_LOAD_BITMAP_FROM_THUMB	pf_load_bitmap_from_thumb;
	PF_LOAD_BITMAP_FROM_ICON	pf_load_bitmap_from_icon;
#endif

#ifdef XDU_SUPPORT_CONTEXT_PRINTER
	PF_SETUP_PRINTER_MODE		pf_setup_printer_mode;
	PF_DEFAULT_PRINTER_MODE		pf_default_printer_mode;
	PF_CREATE_PRINTER_CONTEXT	pf_create_printer_context;
	PF_DESTROY_PRINTER_CONTEXT	pf_destroy_printer_context;
	PF_BEGIN_PAGE				pf_begin_page;
	PF_END_PAGE					pf_end_page;
	PF_BEGIN_DOC				pf_begin_doc;
	PF_END_DOC					pf_end_doc;
#endif
}if_context_t;
#endif /*XDU_SUPPORT_CONTEXT*/


#ifdef XDU_SUPPORT_CLIPBOARD
/*clipboard interface*/
typedef bool_t(*PF_CLIPBOARD_OPEN)(void);
typedef void(*PF_CLIPBOARD_CLOSE)(void);
typedef void(*PF_CLIPBOARD_CLEAN)(void);
typedef bool_t(*PF_CLIPBOARD_PUT)(int, res_glob_t);
typedef res_glob_t(*PF_CLIPBOARD_GET)(int fmt);

typedef struct _if_clipboard_t{
	PF_CLIPBOARD_OPEN		pf_clipboard_open;
	PF_CLIPBOARD_CLOSE		pf_clipboard_close;
	PF_CLIPBOARD_CLEAN		pf_clipboard_clean;
	PF_CLIPBOARD_PUT		pf_clipboard_put;
	PF_CLIPBOARD_GET		pf_clipboard_get;
}if_clipboard_t;
#endif /*XDU_SUPPORT_CLIPBOARD*/

#ifdef XDU_SUPPORT_WIDGET
/*widget interface*/

typedef void(*PF_WIDGET_STARTUP)(int);
typedef void(*PF_WIDGET_CLEANUP)(void);
typedef res_win_t(*PF_WIDGET_CREATE)(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev);
typedef void(*PF_WIDGET_DESTROY)(res_win_t);
typedef void(*PF_WIDGET_CLOSE)(res_win_t, int);
typedef if_event_t* (*PF_WIDGET_GET_DISPATCH)(res_win_t);
typedef void(*PF_WIDGET_SET_STYLE)(res_win_t, dword_t);
typedef dword_t(*PF_WIDGET_GET_STYLE)(res_win_t);
typedef void(*PF_WIDGET_SET_ACCEL)(res_win_t, res_acl_t);
typedef res_acl_t(*PF_WIDGET_GET_ACCEL)(res_win_t);
typedef void(*PF_WIDGET_SET_OWNER)(res_win_t, res_win_t);
typedef res_win_t(*PF_WIDGET_GET_OWNER)(res_win_t);
typedef void(*PF_WIDGET_SET_USER_ID)(res_win_t, uid_t);
typedef uid_t(*PF_WIDGET_GET_USER_ID)(res_win_t);
typedef void(*PF_WIDGET_SET_CORE_DELTA)(res_win_t, var_long);
typedef var_long (*PF_WIDGET_GET_CORE_DELTA)(res_win_t);
typedef void(*PF_WIDGET_SET_USER_DELTA)(res_win_t, var_long);
typedef var_long (*PF_WIDGET_GET_USER_DELTA)(res_win_t);
typedef void(*PF_WIDGET_SET_USER_PROP)(res_win_t, const tchar_t*, var_long);
typedef var_long (*PF_WIDGET_GET_USER_PROP)(res_win_t, const tchar_t*);
typedef var_long (*PF_WIDGET_DEL_USER_PROP)(res_win_t, const tchar_t*);
typedef void(*PF_WIDGET_SET_USER_RESULT)(res_win_t, int);
typedef int(*PF_WIDGET_GET_USER_RESULT)(res_win_t);
typedef visual_t(*PF_WIDGET_CLIENT_DC)(res_win_t);
typedef visual_t(*PF_WIDGET_WINDOW_DC)(res_win_t);
typedef void(*PF_WIDGET_RELEASE_DC)(res_win_t, visual_t);
typedef res_win_t(*PF_WIDGET_GET_CHILD)(res_win_t, uid_t);
typedef res_win_t(*PF_WIDGET_GET_PARENT)(res_win_t);
typedef void(*PF_WIDGET_GET_CLIENT_RECT)(res_win_t, xrect_t*);
typedef void(*PF_WIDGET_GET_WINDOW_RECT)(res_win_t, xrect_t*);
typedef void(*PF_WIDGET_CLIENT_TO_SCREEN)(res_win_t, xpoint_t*);
typedef void(*PF_WIDGET_SCREEN_TO_CLIENT)(res_win_t, xpoint_t*);
typedef void(*PF_WIDGET_CLIENT_TO_WINDOW)(res_win_t, xpoint_t*);
typedef void(*PF_WIDGET_WINDOW_TO_CLIENT)(res_win_t, xpoint_t*);
typedef void(*PF_WIDGET_CENTER_WINDOW)(res_win_t, res_win_t);
typedef void(*PF_WIDGET_SET_CURSOR)(res_win_t, int);
typedef void(*PF_WIDGET_SET_CAPTURE)(res_win_t, bool_t);
typedef void(*PF_WIDGET_SET_FOCUS)(res_win_t);
typedef bool_t(*PF_WIDGET_KEY_STATE)(res_win_t, int);
typedef bool_t(*PF_WIDGET_IS_VALID)(res_win_t);
typedef bool_t(*PF_WIDGET_IS_CHILD)(res_win_t);
typedef bool_t(*PF_WIDGET_IS_FOCUS)(res_win_t);
typedef bool_t(*PF_WIDGET_IS_OWNC)(res_win_t);
typedef void(*PF_WIDGET_SIZE)(res_win_t, const xsize_t*);
typedef void(*PF_WIDGET_MOVE)(res_win_t, const xpoint_t*);
typedef void(*PF_WIDGET_TAKE)(res_win_t, int);
typedef void(*PF_WIDGET_SHOW)(res_win_t, dword_t);
typedef void(*PF_WIDGET_UPDATE)(res_win_t);
typedef void(*PF_WIDGET_LAYOUT)(res_win_t);
typedef void(*PF_WIDGET_PAINT)(res_win_t);
typedef void(*PF_WIDGET_ERASE)(res_win_t, const xrect_t*);
typedef void(*PF_WIDGET_ENABLE)(res_win_t, bool_t);
typedef void(*PF_WIDGET_ACTIVE)(res_win_t);
typedef void(*PF_WIDGET_CREATE_CARET)(res_win_t, int, int);
typedef void(*PF_WIDGET_DESTROY_CARET)(res_win_t);
typedef void(*PF_WIDGET_SHOW_CARET)(res_win_t, int, int, bool_t);

typedef void(*PF_WIDGET_SET_TITLE)(res_win_t, const tchar_t*);
typedef int(*PF_WIDGET_GET_TITLE)(res_win_t, tchar_t*, int);
typedef void(*PF_WIDGET_CALC_BORDER)(dword_t, border_t*);
typedef void(*PF_WIDGET_ADJUST_SIZE)(dword_t, xsize_t*);
typedef bool_t(*PF_WIDGET_ENUM_CHILD)(res_win_t, PF_ENUM_WINDOW_PROC, var_long);
typedef void(*PF_WIDGET_MENU_ITEM_RECT)(res_win_t, int, xrect_t*);
typedef bool_t(*PF_WIDGET_IS_MAXIMIZED)(res_win_t);
typedef bool_t(*PF_WIDGET_IS_MINIMIZED)(res_win_t);

typedef if_subproc_t* (*PF_WIDGET_GET_SUBPROC)(res_win_t, uid_t);
typedef bool_t(*PF_WIDGET_SET_SUBPROC)(res_win_t, uid_t, if_subproc_t*);
typedef void(*PF_WIDGET_DEL_SUBPROC)(res_win_t, uid_t);
typedef bool_t(*PF_WIDGET_SET_SUBPROC_DELTA)(res_win_t, uid_t, var_long);
typedef var_long (*PF_WIDGET_GET_SUBPROC_DELTA)(res_win_t, uid_t);
typedef bool_t(*PF_WIDGET_HAS_SUBPROC)(res_win_t);

typedef void(*PF_WIDGET_POST_CHAR)(res_win_t, tchar_t);
typedef void(*PF_WIDGET_POST_KEY)(res_win_t, int);
typedef void(*PF_WIDGET_POST_NOTICE)(res_win_t, NOTICE*);
typedef int(*PF_WIDGET_SEND_NOTICE)(res_win_t, NOTICE*);
typedef void(*PF_WIDGET_POST_COMMAND)(res_win_t, int, uid_t, var_long);
typedef int(*PF_WIDGET_SEND_COMMAND)(res_win_t, int, uid_t, var_long);

typedef var_long(*PF_WIDGET_SET_TIMER)(res_win_t, int);
typedef void(*PF_WIDGET_KILL_TIMER)(res_win_t, var_long);

typedef void(*PF_WIDGET_SCROLL)(res_win_t, bool_t, int);
typedef void(*PF_WIDGET_GET_SCROLLINFO)(res_win_t, bool_t, scroll_t*);
typedef void(*PF_WIDGET_SET_SCROLLINFO)(res_win_t, bool_t, const scroll_t*);
typedef bool_t(*PF_WIDGET_HAS_STRUCT)(res_win_t);
typedef void(*PF_WIDGET_SET_XFONT)(res_win_t,const xfont_t*);
typedef void(*PF_WIDGET_GET_XFONT)(res_win_t, xfont_t*);
typedef const xfont_t*(*PF_WIDGET_GET_XFONT_PTR)(res_win_t);
typedef void(*PF_WIDGET_SET_XFACE)(res_win_t, const xface_t*);
typedef void(*PF_WIDGET_GET_XFACE)(res_win_t, xface_t*);
typedef const xface_t*(*PF_WIDGET_GET_XFACE_PTR)(res_win_t);
typedef void(*PF_WIDGET_SET_XBRUSH)(res_win_t, const xbrush_t*);
typedef void(*PF_WIDGET_GET_XBRUSH)(res_win_t, xbrush_t*);
typedef const xbrush_t*(*PF_WIDGET_GET_XBRUSH_PTR)(res_win_t);
typedef void(*PF_WIDGET_SET_XPEN)(res_win_t, const xpen_t*);
typedef void(*PF_WIDGET_GET_XPEN)(res_win_t, xpen_t*);
typedef const xpen_t*(*PF_WIDGET_GET_XPEN_PTR)(res_win_t);
typedef void(*PF_WIDGET_SET_MASK)(res_win_t, const xcolor_t*);
typedef void(*PF_WIDGET_GET_MASK)(res_win_t, xcolor_t*);
typedef const xcolor_t*(*PF_WIDGET_GET_MASK_PTR)(res_win_t);
typedef void(*PF_WIDGET_SET_ICONIC)(res_win_t, const xcolor_t*);
typedef void(*PF_WIDGET_GET_ICONIC)(res_win_t, xcolor_t*);
typedef const xcolor_t*(*PF_WIDGET_GET_ICONIC_PTR)(res_win_t);
typedef void(*PF_WIDGET_SET_COLOR_MODE)(res_win_t, const clr_mod_t*);
typedef void(*PF_WIDGET_GET_COLOR_MODE)(res_win_t, clr_mod_t*);
typedef void(*PF_WIDGET_SET_POINT)(res_win_t, const xpoint_t*);
typedef void(*PF_WIDGET_GET_POINT)(res_win_t, xpoint_t*);
typedef void(*PF_WIDGET_SET_SIZE)(res_win_t, const xsize_t*);
typedef void(*PF_WIDGET_GET_SIZE)(res_win_t, xsize_t*);

typedef int(*PF_WIDGET_DO_NORMAL)(res_win_t);
typedef int(*PF_WIDGET_DO_MODAL)(res_win_t);
typedef void(*PF_WIDGET_DO_TRACE)(res_win_t);

typedef void(*PF_SEND_QUIT_MESSAGE)(int);
typedef void(*PF_MESSAGE_FETCH)(msg_t*, res_win_t);
typedef bool_t(*PF_MESSAGE_PEEK)(msg_t*);
typedef bool_t(*PF_MESSAGE_TRANSLATE)(const msg_t*);
typedef result_t(*PF_MESSAGE_DISPATCH)(const msg_t*);
typedef bool_t(*PF_MESSAGE_IS_QUIT)(const msg_t*);
typedef void(*PF_MESSAGE_POSITION)(xpoint_t*);

typedef void(*PF_GET_SCREEN_SIZE)(xsize_t*);
typedef void(*PF_GET_DESKTOP_SIZE)(xsize_t*);
typedef void(*PF_SCREEN_SIZE_TO_PT)(xsize_t*);
typedef void(*PF_SCREEN_SIZE_TO_TM)(xsize_t*);
typedef res_acl_t(*PF_CREATE_ACCEL_TABLE)(const accel_t*, int);
typedef void(*PF_DESTROY_ACCEL_TABLE)(res_acl_t);

typedef void(*PF_WIDGET_TRACK_MOUSE)(res_win_t, dword_t);
typedef void(*PF_WIDGET_SET_ALPHA)(res_win_t, unsigned char);
typedef unsigned char(*PF_WIDGET_GET_ALPHA)(res_win_t);

#ifdef XDU_SUPPORT_WIDGET_REGION
typedef void(*PF_WIDGET_SET_REGION)(res_win_t, res_rgn_t);
#endif

#ifdef XDU_SUPPORT_CONTEXT_OPENGL
typedef res_glc_t(*PF_WIDGET_GET_GLCTX)(res_win_t);
#endif

typedef struct _if_widget_t{

	PF_WIDGET_STARTUP		pf_widget_startup;
	PF_WIDGET_CLEANUP		pf_widget_cleanup;

	PF_WIDGET_CREATE			pf_widget_create;
	PF_WIDGET_DESTROY			pf_widget_destroy;
	PF_WIDGET_CLOSE				pf_widget_close;
	PF_WIDGET_GET_DISPATCH		pf_widget_get_dispatch;
	PF_WIDGET_SET_STYLE			pf_widget_set_style;
	PF_WIDGET_GET_STYLE			pf_widget_get_style;
	PF_WIDGET_SET_ACCEL			pf_widget_set_accel;
	PF_WIDGET_GET_ACCEL			pf_widget_get_accel;
	PF_WIDGET_SET_OWNER			pf_widget_set_owner;
	PF_WIDGET_GET_OWNER			pf_widget_get_owner;
	PF_WIDGET_SET_CORE_DELTA	pf_widget_set_core_delta;
	PF_WIDGET_GET_CORE_DELTA	pf_widget_get_core_delta;
	PF_WIDGET_SET_USER_DELTA	pf_widget_set_user_delta;
	PF_WIDGET_GET_USER_DELTA	pf_widget_get_user_delta;
	PF_WIDGET_SET_USER_ID		pf_widget_set_user_id;
	PF_WIDGET_GET_USER_ID		pf_widget_get_user_id;
	PF_WIDGET_SET_USER_PROP		pf_widget_set_user_prop;
	PF_WIDGET_GET_USER_PROP		pf_widget_get_user_prop;
	PF_WIDGET_DEL_USER_PROP		pf_widget_del_user_prop;
	PF_WIDGET_SET_USER_RESULT	pf_widget_set_user_result;
	PF_WIDGET_GET_USER_RESULT	pf_widget_get_user_result;
	PF_WIDGET_CLIENT_DC			pf_widget_client_ctx;
	PF_WIDGET_WINDOW_DC			pf_widget_window_ctx;
	PF_WIDGET_RELEASE_DC		pf_widget_release_ctx;
	PF_WIDGET_GET_CHILD			pf_widget_get_child;
	PF_WIDGET_GET_PARENT		pf_widget_get_parent;
	PF_WIDGET_GET_CLIENT_RECT	pf_widget_get_client_rect;
	PF_WIDGET_GET_WINDOW_RECT	pf_widget_get_window_rect;
	PF_WIDGET_CLIENT_TO_SCREEN	pf_widget_client_to_screen;
	PF_WIDGET_SCREEN_TO_CLIENT	pf_widget_screen_to_client;
	PF_WIDGET_CLIENT_TO_WINDOW	pf_widget_client_to_window;
	PF_WIDGET_WINDOW_TO_CLIENT	pf_widget_window_to_client;
	PF_WIDGET_CENTER_WINDOW		pf_widget_center_window;
	PF_WIDGET_SET_CURSOR		pf_widget_set_cursor;
	PF_WIDGET_SET_CAPTURE		pf_widget_set_capture;
	PF_WIDGET_SET_FOCUS			pf_widget_set_focus;
	PF_WIDGET_KEY_STATE			pf_widget_key_state;
	PF_WIDGET_IS_VALID			pf_widget_is_valid;
	PF_WIDGET_IS_CHILD			pf_widget_is_child;
	PF_WIDGET_IS_FOCUS			pf_widget_is_focus;
	PF_WIDGET_IS_OWNC			pf_widget_is_ownc;
	PF_WIDGET_SIZE				pf_widget_size;
	PF_WIDGET_MOVE				pf_widget_move;
	PF_WIDGET_TAKE				pf_widget_take;
	PF_WIDGET_SHOW				pf_widget_show;
	PF_WIDGET_UPDATE			pf_widget_update;
	PF_WIDGET_LAYOUT			pf_widget_layout;
	PF_WIDGET_PAINT				pf_widget_paint;
	PF_WIDGET_ERASE				pf_widget_erase;
	PF_WIDGET_ENABLE			pf_widget_enable;
	PF_WIDGET_ACTIVE			pf_widget_active;
	PF_WIDGET_CREATE_CARET		pf_widget_create_caret;
	PF_WIDGET_DESTROY_CARET		pf_widget_destroy_caret;
	PF_WIDGET_SHOW_CARET		pf_widget_show_caret;
	PF_WIDGET_SET_TITLE			pf_widget_set_title;
	PF_WIDGET_GET_TITLE			pf_widget_get_title;
	PF_WIDGET_SET_TIMER			pf_widget_set_timer;
	PF_WIDGET_KILL_TIMER		pf_widget_kill_timer;
	PF_WIDGET_CALC_BORDER		pf_widget_calc_border;
	PF_WIDGET_ADJUST_SIZE		pf_widget_adjust_size;
	PF_WIDGET_ENUM_CHILD		pf_widget_enum_child;

	PF_WIDGET_IS_MAXIMIZED		pf_widget_is_maximized;
	PF_WIDGET_IS_MINIMIZED		pf_widget_is_minimized;

	PF_WIDGET_SET_SUBPROC		pf_widget_set_subproc;
	PF_WIDGET_GET_SUBPROC		pf_widget_get_subproc;
	PF_WIDGET_DEL_SUBPROC		pf_widget_del_subproc;
	PF_WIDGET_SET_SUBPROC_DELTA		pf_widget_set_subproc_delta;
	PF_WIDGET_GET_SUBPROC_DELTA		pf_widget_get_subproc_delta;
	PF_WIDGET_HAS_SUBPROC		pf_widget_has_subproc;

	PF_WIDGET_POST_CHAR			pf_widget_post_char;
	PF_WIDGET_POST_KEY			pf_widget_post_key;
	PF_WIDGET_POST_NOTICE		pf_widget_post_notice;
	PF_WIDGET_SEND_NOTICE		pf_widget_send_notice;
	PF_WIDGET_POST_COMMAND		pf_widget_post_command;
	PF_WIDGET_SEND_COMMAND		pf_widget_send_command;

	PF_WIDGET_SCROLL			pf_widget_scroll;
	PF_WIDGET_GET_SCROLLINFO	pf_widget_get_scroll_info;
	PF_WIDGET_SET_SCROLLINFO	pf_widget_set_scroll_info;

	PF_WIDGET_HAS_STRUCT		pf_widget_has_struct;
	PF_WIDGET_SET_XFONT			pf_widget_set_xfont;
	PF_WIDGET_GET_XFONT			pf_widget_get_xfont;
	PF_WIDGET_GET_XFONT_PTR		pf_widget_get_xfont_ptr;
	PF_WIDGET_SET_XFACE			pf_widget_set_xface;
	PF_WIDGET_GET_XFACE			pf_widget_get_xface;
	PF_WIDGET_GET_XFACE_PTR		pf_widget_get_xface_ptr;
	PF_WIDGET_SET_XBRUSH		pf_widget_set_xbrush;
	PF_WIDGET_GET_XBRUSH		pf_widget_get_xbrush;
	PF_WIDGET_GET_XBRUSH_PTR	pf_widget_get_xbrush_ptr;
	PF_WIDGET_SET_XPEN			pf_widget_set_xpen;
	PF_WIDGET_GET_XPEN			pf_widget_get_xpen;
	PF_WIDGET_GET_XPEN_PTR		pf_widget_get_xpen_ptr;
	PF_WIDGET_SET_MASK			pf_widget_set_mask;
	PF_WIDGET_GET_MASK			pf_widget_get_mask;
	PF_WIDGET_GET_MASK_PTR		pf_widget_get_mask_ptr;
	PF_WIDGET_SET_ICONIC		pf_widget_set_iconic;
	PF_WIDGET_GET_ICONIC		pf_widget_get_iconic;
	PF_WIDGET_GET_ICONIC_PTR	pf_widget_get_iconic_ptr;
	PF_WIDGET_SET_COLOR_MODE	pf_widget_set_color_mode;
	PF_WIDGET_GET_COLOR_MODE	pf_widget_get_color_mode;
	PF_WIDGET_SET_POINT			pf_widget_set_point;
	PF_WIDGET_GET_POINT			pf_widget_get_point;
	PF_WIDGET_SET_SIZE			pf_widget_set_size;
	PF_WIDGET_GET_SIZE			pf_widget_get_size;

	PF_WIDGET_DO_NORMAL			pf_widget_do_normal;
	PF_WIDGET_DO_MODAL			pf_widget_do_modal;
	PF_WIDGET_DO_TRACE			pf_widget_do_trace;

	PF_SEND_QUIT_MESSAGE		pf_send_quit_message;
	PF_MESSAGE_FETCH			pf_message_fetch;
    PF_MESSAGE_PEEK         	pf_message_peek;
	PF_MESSAGE_TRANSLATE		pf_message_translate;
	PF_MESSAGE_DISPATCH			pf_message_dispatch;
	PF_MESSAGE_IS_QUIT			pf_message_is_quit;
	PF_MESSAGE_POSITION			pf_message_position;

	PF_GET_SCREEN_SIZE			pf_get_screen_size;
	PF_GET_DESKTOP_SIZE			pf_get_desktop_size;
	PF_SCREEN_SIZE_TO_PT		pf_screen_size_to_pt;
	PF_SCREEN_SIZE_TO_TM		pf_screen_size_to_tm;
	PF_CREATE_ACCEL_TABLE		pf_create_accel_table;
	PF_DESTROY_ACCEL_TABLE		pf_destroy_accel_table;

	PF_WIDGET_TRACK_MOUSE		pf_widget_track_mouse;
	PF_WIDGET_SET_ALPHA			pf_widget_set_alpha;
	PF_WIDGET_GET_ALPHA			pf_widget_get_alpha;

#ifdef XDU_SUPPORT_WIDGET_REGION
	PF_WIDGET_SET_REGION		pf_widget_set_region;
#endif

#ifdef XDU_SUPPORT_CONTEXT_OPENGL
	PF_WIDGET_GET_GLCTX			pf_widget_get_glctx;
#endif
}if_widget_t;
#endif /*XDU_SUPPORT_WIDGET*/


#ifdef	__cplusplus
extern "C" {
#endif

#ifdef XDU_SUPPORT_BLUT
	EXP_API void xdu_impl_blut(if_blut_t* pif);
#endif

#ifdef XDU_SUPPORT_SHELL
	EXP_API void xdu_impl_shell(if_shell_t* pif);
#ifdef XDU_SUPPORT_SHELL_DIALOG
	EXP_API void xdu_impl_shell_dialog(if_shell_t* pif);
#endif
#endif /*XDU_SUPPORT_SHELL*/

#ifdef XDU_SUPPORT_CONTEXT
	EXP_API void xdu_impl_context(if_context_t* pif);
	EXP_API void xdu_impl_context_graphic(if_context_t* pif);
#ifdef XDU_SUPPORT_CONTEXT_REGION
	EXP_API void xdu_impl_context_region(if_context_t* pif);
#endif
#ifdef XDU_SUPPORT_CONTEXT_BITMAP
	EXP_API void xdu_impl_context_bitmap(if_context_t* pif);
#endif
#ifdef XDU_SUPPORT_CONTEXT_PRINTER
	EXP_API void xdu_impl_context_printer(if_context_t* pif);
#endif
#endif /*XDU_SUPPORT_CONTEXT*/

#ifdef XDU_SUPPORT_CLIPBOARD
	EXP_API void xdu_impl_clipboard(if_clipboard_t* pif);
#endif

#ifdef XDU_SUPPORT_WIDGET
	EXP_API void xdu_impl_widget(if_widget_t* pif);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _XDUINF_H */

