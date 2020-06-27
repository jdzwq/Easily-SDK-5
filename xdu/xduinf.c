/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdu document

	@module	xduinf.c | implement file

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

#include "xduinf.h"
#include "xduiml.h"


#ifdef XDU_SUPPORT_SHELL
void xdu_impl_shell(if_shell_t* pif)
{
	pif->pf_shell_get_curpath = _shell_get_curpath;
	pif->pf_shell_get_runpath = _shell_get_runpath;
	pif->pf_shell_get_docpath = _shell_get_docpath;
	pif->pf_shell_get_apppath = _shell_get_apppath;
	pif->pf_shell_get_tmppath = _shell_get_tmppath;
}

#ifdef XDU_SUPPORT_SHELL_DIALOG
void xdu_impl_shell_dialog(if_shell_t* pif)
{
	pif->pf_shell_get_filename = _shell_get_filename;
	pif->pf_shell_get_pathname = _shell_get_pathname;
}
#endif
#endif /*XDU_SUPPORT_SHELL*/


#ifdef XDU_SUPPORT_CONTEXT
void xdu_impl_context(if_context_t* pif)
{
	pif->pf_context_startup = _context_startup;
	pif->pf_context_cleanup = _context_cleanup;

	pif->pf_create_compatible_context = _create_compatible_context;
	pif->pf_create_display_context = _create_display_context;
	pif->pf_destroy_context = _destroy_context;
	pif->pf_get_device_caps = _get_device_caps;
	pif->pf_render_context = _render_context;

	pif->pf_pt_per_mm = _pt_per_mm;;
	pif->pf_text_mm_metric = _text_mm_metric;
	pif->pf_text_pt_metric = _text_pt_metric;
	pif->pf_text_mm_size = _text_mm_size;
	pif->pf_text_pt_size = _text_pt_size;
	pif->pf_cast_mm_to_pt = _cast_mm_to_pt;
	pif->pf_cast_pt_to_mm = _cast_pt_to_mm;
	pif->pf_font_size = _font_size;
}

#ifdef XDU_SUPPORT_CONTEXT_BITMAP
void xdu_impl_context_bitmap(if_context_t* pif)
{
	pif->pf_destroy_bitmap = _destroy_bitmap;
	pif->pf_get_bitmap_size = _get_bitmap_size;

	pif->pf_create_context_bitmap = _create_context_bitmap;
	pif->pf_create_color_bitmap = _create_color_bitmap;
	pif->pf_create_gradient_bitmap = _create_gradient_bitmap;
	pif->pf_create_pattern_bitmap = _create_pattern_bitmap;
	pif->pf_create_code128_bitmap = _create_code128_bitmap;
	pif->pf_create_pdf417_bitmap = _create_pdf417_bitmap;
	pif->pf_create_qrcode_bitmap = _create_qrcode_bitmap;
	pif->pf_create_storage_bitmap = _create_storage_bitmap;
	pif->pf_save_bitmap_to_bytes = _save_bitmap_to_bytes;
	pif->pf_load_bitmap_from_bytes = _load_bitmap_from_bytes;
	pif->pf_get_bitmap_bytes = _get_bitmap_bytes;
#if defined(XDU_SUPPORT_SHELL)
	pif->pf_load_bitmap_from_thumb = _load_bitmap_from_thumb;
	pif->pf_load_bitmap_from_icon = _load_bitmap_from_icon;
#endif
}
#endif

#ifdef XDU_SUPPORT_CONTEXT_REGION
void xdu_impl_context_region(if_context_t* pif)
{
	pif->pf_create_region = _create_region;
	pif->pf_delete_region = _delete_region;
	pif->pf_pt_in_region = _pt_in_region;
}
#endif

#ifdef XDU_SUPPORT_CONTEXT_PRINTER
void xdu_impl_context_printer(if_context_t* pif)
{
	pif->pf_create_printer_context = _create_printer_context;
	pif->pf_destroy_printer_context = _destroy_printer_context;
	pif->pf_setup_printer_mode = _setup_printer_mode;
	pif->pf_default_printer_mode = _default_printer_mode;
	pif->pf_begin_doc = _begin_doc;
	pif->pf_begin_page = _begin_page;
	pif->pf_end_page = _end_page;
	pif->pf_end_doc = _end_doc;
}
#endif

void xdu_impl_context_graphic(if_context_t* pif)
{
#ifdef XDU_SUPPORT_CONTEXT_GDI
	pif->pf_gdi_draw_arrow = _gdi_draw_arrow;
	pif->pf_gdi_draw_ellipse = _gdi_draw_ellipse;
	pif->pf_gdi_draw_line = _gdi_draw_line;
	pif->pf_gdi_draw_pie = _gdi_draw_pie;
	pif->pf_gdi_draw_arc = _gdi_draw_arc;
	pif->pf_gdi_draw_polygon = _gdi_draw_polygon;
	pif->pf_gdi_draw_polyline = _gdi_draw_polyline;
	pif->pf_gdi_draw_bezier = _gdi_draw_bezier;
	pif->pf_gdi_draw_curve = _gdi_draw_curve;
	pif->pf_gdi_draw_rect = _gdi_draw_rect;
	pif->pf_gdi_draw_round = _gdi_draw_round;
	pif->pf_gdi_draw_text = _gdi_draw_text;
	pif->pf_gdi_text_out = _gdi_text_out;
	pif->pf_gdi_text_size = _gdi_text_size;
	pif->pf_gdi_text_metric = _gdi_text_metric;
	pif->pf_gdi_gradient_rect = _gdi_gradient_rect;
	pif->pf_gdi_alphablend_rect = _gdi_alphablend_rect;
	pif->pf_gdi_exclip_rect = _gdi_exclip_rect;
	pif->pf_gdi_inclip_rect = _gdi_inclip_rect;
#ifdef XDU_SUPPORT_CONTEXT_BITMAP
	pif->pf_gdi_draw_bitmap = _gdi_draw_bitmap;
	pif->pf_gdi_draw_image = _gdi_draw_image;
#endif
#ifdef XDU_SUPPORT_CONTEXT_REGION
    pif->pf_gdi_fill_region = _gdi_fill_region;
#endif
#endif

#ifdef XDU_SUPPORT_CONTEXT_GDIPLUS
	pif->pf_gdi_draw_arrow = _gdiplus_draw_arrow;
	pif->pf_gdi_draw_ellipse = _gdiplus_draw_ellipse;
	pif->pf_gdi_draw_line = _gdiplus_draw_line;
	pif->pf_gdi_draw_pie = _gdiplus_draw_pie;
	pif->pf_gdi_draw_arc = _gdiplus_draw_arc;
	pif->pf_gdi_draw_polygon = _gdiplus_draw_polygon;
	pif->pf_gdi_draw_polyline = _gdiplus_draw_polyline;
	pif->pf_gdi_draw_bezier = _gdiplus_draw_bezier;
	pif->pf_gdi_draw_curve = _gdiplus_draw_curve;
	pif->pf_gdi_draw_rect = _gdiplus_draw_rect;
	pif->pf_gdi_draw_path = _gdiplus_draw_path;
	pif->pf_gdi_draw_round = _gdiplus_draw_round;
	pif->pf_gdi_draw_text = _gdiplus_draw_text;
	pif->pf_gdi_text_out = _gdiplus_text_out;
	pif->pf_gdi_text_size = _gdiplus_text_size;
	pif->pf_gdi_text_metric = _gdiplus_text_metric;
	pif->pf_gdi_gradient_rect = _gdiplus_gradient_rect;
	pif->pf_gdi_alphablend_rect = _gdiplus_alphablend_rect;
	pif->pf_gdi_exclip_rect = _gdiplus_exclip_rect;
	pif->pf_gdi_inclip_rect = _gdiplus_inclip_rect;
#ifdef XDU_SUPPORT_CONTEXT_BITMAP
	pif->pf_gdi_draw_bitmap = _gdiplus_draw_bitmap;
	pif->pf_gdi_draw_image = _gdiplus_draw_image;
#endif
#ifdef XDU_SUPPORT_CONTEXT_REGION
	pif->pf_gdi_fill_region = _gdiplus_fill_region;
#endif
#endif

#ifdef XDU_SUPPORT_CONTEXT_CAIRO
	pif->pf_gdi_draw_arrow = _cairo_draw_arrow;
	pif->pf_gdi_draw_ellipse = _cairo_draw_ellipse;
	pif->pf_gdi_draw_line = _cairo_draw_line;
	pif->pf_gdi_draw_pie = _cairo_draw_pie;
	pif->pf_gdi_draw_arc = _cairo_draw_arc;
	pif->pf_gdi_draw_polygon = _cairo_draw_polygon;
	pif->pf_gdi_draw_polyline = _cairo_draw_polyline;
	pif->pf_gdi_draw_bezier = _cairo_draw_bezier;
	pif->pf_gdi_draw_curve = _cairo_draw_curve;
	pif->pf_gdi_draw_rect = _cairo_draw_rect;
	pif->pf_gdi_draw_path = _cairo_draw_path;
	pif->pf_gdi_draw_round = _cairo_draw_round;
	pif->pf_gdi_draw_text = _cairo_draw_text;
	pif->pf_gdi_text_out = _cairo_text_out;
	pif->pf_gdi_text_size = _cairo_text_size;
	pif->pf_gdi_text_metric = _cairo_text_metric;
	pif->pf_gdi_gradient_rect = _cairo_gradient_rect;
	pif->pf_gdi_alphablend_rect = _cairo_alphablend_rect;
	pif->pf_gdi_exclip_rect = _cairo_exclip_rect;
	pif->pf_gdi_inclip_rect = _cairo_inclip_rect;
#ifdef XDU_SUPPORT_CONTEXT_BITMAP
	pif->pf_gdi_draw_bitmap = _cairo_draw_bitmap;
	pif->pf_gdi_draw_image = _cairo_draw_image;
#endif
#ifdef XDU_SUPPORT_CONTEXT_REGION
	pif->pf_gdi_fill_region = _cairo_fill_region;
#endif
#endif
}

#endif /*XDU_SUPPORT_CONTEXT*/

#ifdef XDU_SUPPORT_CLIPBOARD
void xdu_impl_clipboard(if_clipboard_t* pif)
{
	pif->pf_clipboard_open = _clipboard_open;
	pif->pf_clipboard_close = _clipboard_close;
	pif->pf_clipboard_clean = _clipboard_clean;
	pif->pf_clipboard_get = _clipboard_get;
	pif->pf_clipboard_put = _clipboard_put;
}
#endif

#ifdef XDU_SUPPORT_WIDGET
void xdu_impl_widget(if_widget_t* pif)
{

	pif->pf_widget_startup = _widget_startup;
	pif->pf_widget_cleanup = _widget_cleanup;
	pif->pf_widget_create = _widget_create;
	pif->pf_widget_destroy = _widget_destroy;
	pif->pf_widget_close = _widget_close;
	pif->pf_widget_get_dispatch = _widget_get_dispatch;
	pif->pf_widget_get_style = _widget_get_style;
	pif->pf_widget_set_style = _widget_set_style;
	pif->pf_widget_get_accel = _widget_get_accel;
	pif->pf_widget_set_accel = _widget_set_accel;
	pif->pf_widget_get_owner = _widget_get_owner;
	pif->pf_widget_set_owner = _widget_set_owner;
	pif->pf_widget_get_core_delta = _widget_get_core_delta;
	pif->pf_widget_set_core_delta = _widget_set_core_delta;
	pif->pf_widget_get_user_delta = _widget_get_user_delta;
	pif->pf_widget_set_user_delta = _widget_set_user_delta;
	pif->pf_widget_get_user_id = _widget_get_user_id;
	pif->pf_widget_set_user_id = _widget_set_user_id;
	pif->pf_widget_get_child = _widget_get_child;
	pif->pf_widget_get_parent = _widget_get_parent;
	pif->pf_widget_get_user_prop = _widget_get_user_prop;
	pif->pf_widget_set_user_prop = _widget_set_user_prop;
	pif->pf_widget_del_user_prop = _widget_del_user_prop;
	pif->pf_widget_get_user_result = _widget_get_user_result;
	pif->pf_widget_set_user_result = _widget_set_user_result;
	pif->pf_widget_client_ctx = _widget_client_ctx;
	pif->pf_widget_window_ctx = _widget_window_ctx;
	pif->pf_widget_release_ctx = _widget_release_ctx;
	pif->pf_widget_center_window = _widget_center_window;
	pif->pf_widget_screen_to_client = _widget_screen_to_client;
	pif->pf_widget_client_to_screen = _widget_client_to_screen;
	pif->pf_widget_window_to_client = _widget_window_to_client;
	pif->pf_widget_client_to_window = _widget_client_to_window;
	pif->pf_widget_get_client_rect = _widget_get_client_rect;
	pif->pf_widget_get_window_rect = _widget_get_window_rect;
	pif->pf_widget_is_child = _widget_is_child;
	pif->pf_widget_is_valid = _widget_is_valid;
	pif->pf_widget_is_focus = _widget_is_focus;
	pif->pf_widget_is_ownc = _widget_is_ownc;
	pif->pf_widget_key_state = _widget_key_state;
	pif->pf_widget_size = _widget_size;
	pif->pf_widget_move = _widget_move;
	pif->pf_widget_take = _widget_take;
	pif->pf_widget_show = _widget_show;
	pif->pf_widget_update = _widget_update;
	pif->pf_widget_layout = _widget_layout;
	pif->pf_widget_paint = _widget_paint;
	pif->pf_widget_erase = _widget_erase;
	pif->pf_widget_set_capture = _widget_set_capture;
	pif->pf_widget_set_cursor = _widget_set_cursor;
	pif->pf_widget_set_focus = _widget_set_focus;
	pif->pf_widget_set_title = _widget_set_title;
	pif->pf_widget_get_title = _widget_get_title;
	pif->pf_widget_create_caret = _widget_create_caret;
	pif->pf_widget_destroy_caret = _widget_destroy_caret;
	pif->pf_widget_show_caret = _widget_show_caret;

	pif->pf_widget_enable = _widget_enable;
	pif->pf_widget_active = _widget_active;

	pif->pf_widget_post_char = _widget_post_char;
	pif->pf_widget_post_key = _widget_post_key;
	pif->pf_widget_post_notice = _widget_post_notice;
	pif->pf_widget_send_notice = _widget_send_notice;
	pif->pf_widget_post_command = _widget_post_command;
	pif->pf_widget_send_command = _widget_send_command;

	pif->pf_widget_set_timer = _widget_set_timer;
	pif->pf_widget_kill_timer = _widget_kill_timer;
	pif->pf_widget_adjust_size = _widget_adjust_size;
	pif->pf_widget_calc_border = _widget_calc_border;

	pif->pf_widget_enum_child = _widget_enum_child;

	pif->pf_widget_is_maximized = _widget_is_maximized;
	pif->pf_widget_is_minimized = _widget_is_minimized;

	pif->pf_widget_get_subproc = _widget_get_subproc;
	pif->pf_widget_set_subproc = _widget_set_subproc;
	pif->pf_widget_del_subproc = _widget_del_subproc;
	pif->pf_widget_get_subproc_delta = _widget_get_subproc_delta;
	pif->pf_widget_set_subproc_delta = _widget_set_subproc_delta;
	pif->pf_widget_has_subproc = _widget_has_subproc;

	pif->pf_widget_scroll = _widget_scroll;
	pif->pf_widget_set_scroll_info = _widget_set_scroll_info;
	pif->pf_widget_get_scroll_info = _widget_get_scroll_info;

	pif->pf_widget_has_struct = _widget_has_struct;
	pif->pf_widget_set_xfont = _widget_set_xfont;
	pif->pf_widget_get_xfont = _widget_get_xfont;
	pif->pf_widget_get_xfont_ptr = _widget_get_xfont_ptr;
	pif->pf_widget_set_xface = _widget_set_xface;
	pif->pf_widget_get_xface = _widget_get_xface;
	pif->pf_widget_get_xface_ptr = _widget_get_xface_ptr;
	pif->pf_widget_set_xbrush = _widget_set_xbrush;
	pif->pf_widget_get_xbrush = _widget_get_xbrush;
	pif->pf_widget_get_xbrush_ptr = _widget_get_xbrush_ptr;
	pif->pf_widget_set_xpen = _widget_set_xpen;
	pif->pf_widget_get_xpen = _widget_get_xpen;
	pif->pf_widget_get_xpen_ptr = _widget_get_xpen_ptr;
	pif->pf_widget_set_mask = _widget_set_mask;
	pif->pf_widget_get_mask = _widget_get_mask;
	pif->pf_widget_get_mask_ptr = _widget_get_mask_ptr;
	pif->pf_widget_set_iconic = _widget_set_iconic;
	pif->pf_widget_get_iconic = _widget_get_iconic;
	pif->pf_widget_get_iconic_ptr = _widget_get_iconic_ptr;
	pif->pf_widget_set_color_mode = _widget_set_color_mode;
	pif->pf_widget_get_color_mode = _widget_get_color_mode;
	pif->pf_widget_set_point = _widget_set_point;
	pif->pf_widget_get_point = _widget_get_point;
	pif->pf_widget_set_size = _widget_set_size;
	pif->pf_widget_get_size = _widget_get_size;

	pif->pf_widget_do_normal = _widget_do_normal;
	pif->pf_widget_do_modal = _widget_do_modal;
	pif->pf_widget_do_trace = _widget_do_trace;

	pif->pf_send_quit_message = _send_quit_message;
	pif->pf_message_fetch = _message_fetch;
    pif->pf_message_peek = _message_peek;
	pif->pf_message_translate = _message_translate;
	pif->pf_message_dispatch = _message_dispatch;
	pif->pf_message_is_quit = _message_is_quit;
	pif->pf_message_position = _message_position;

	pif->pf_get_screen_size = _get_screen_size;
	pif->pf_get_desktop_size = _get_desktop_size;
	pif->pf_screen_size_to_pt = _screen_size_to_pt;
	pif->pf_screen_size_to_tm = _screen_size_to_tm;

	pif->pf_create_accel_table = _create_accel_table;
	pif->pf_destroy_accel_table = _destroy_accel_table;

	pif->pf_widget_track_mouse = _widget_track_mouse;
	pif->pf_widget_set_alpha = _widget_set_alpha;
	pif->pf_widget_get_alpha = _widget_get_alpha;

#ifdef XDU_SUPPORT_WIDGET_REGION
	pif->pf_widget_set_region = _widget_set_region;
#endif

#ifdef XDU_SUPPORT_CONTEXT_OPENGL
	pif->pf_widget_get_glctx = _widget_get_glctx;
#endif
}


#endif /*XDU_SUPPORT_WIDGET*/
