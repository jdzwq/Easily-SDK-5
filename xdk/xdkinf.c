/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdk document

	@module	xdkinf.c | implement file

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

#include "xdkinf.h"
#include "xdkiml.h"

#ifdef XDK_SUPPORT_MEMO_HEAP
void xdk_impl_memo_heap(if_memo_t* pif)
{
	pif->pf_process_heap = _process_heapo;
	pif->pf_heap_create = _heapo_create;
	pif->pf_heap_destroy = _heapo_destroy;
	pif->pf_heap_clean = _heapo_clean;
	pif->pf_heap_alloc = _heapo_alloc;
	pif->pf_heap_realloc = _heapo_realloc;
	pif->pf_heap_free = _heapo_free;
	pif->pf_heap_zero = _heapo_zero;
}
#endif

#ifdef XDK_SUPPORT_MEMO_GLOB
void xdk_impl_memo_glob(if_memo_t* pif)
{
	pif->pf_glob_alloc = _globo_alloc;
	pif->pf_glob_free = _globo_free;
	pif->pf_glob_lock = _globo_lock;
	pif->pf_glob_realloc = _globo_realloc;
	pif->pf_glob_unlock = _globo_unlock;
	pif->pf_glob_size = _globo_size;
}
#endif

#ifdef XDK_SUPPORT_MEMO_LOCAL
void xdk_impl_memo_local(if_memo_t* pif)
{
	pif->pf_local_alloc = _local_alloc;
	pif->pf_local_realloc = _local_realloc;
	pif->pf_local_free = _local_free;
}
#endif

#ifdef XDK_SUPPORT_MEMO_PAGE
void xdk_impl_memo_page(if_memo_t* pif)
{
	pif->pf_page_alloc = _paged_alloc;
	pif->pf_page_realloc = _paged_realloc;
	pif->pf_page_free = _paged_free;
	pif->pf_page_lock = _paged_lock;
	pif->pf_page_size = _paged_size;
	pif->pf_page_unlock = _paged_unlock;
	pif->pf_page_protect = _paged_protect;
}
#endif

#ifdef XDK_SUPPORT_MEMO_CACHE
void xdk_impl_memo_cache(if_memo_t* pif)
{
	pif->pf_cache_open = _cache_open;
	pif->pf_cache_close = _cache_close;
	pif->pf_cache_write = _cache_write;
	pif->pf_cache_read = _cache_read;
}
#endif

#ifdef XDK_SUPPORT_ERROR
void xdk_impl_error(if_error_t* pif)
{
	pif->pf_error_text = _error_text;
	pif->pf_error_exit = _error_exit;
	pif->pf_error_debug = _error_debug;
	pif->pf_error_print = _error_print;
}
#endif

#ifdef XDK_SUPPORT_MBCS
void xdk_impl_mbcs(if_mbcs_t* pif)
{
	pif->pf_gbk_to_ucs = _gbk_to_ucs;
	pif->pf_ucs_to_gbk = _ucs_to_gbk;
	pif->pf_utf8_to_ucs = _utf8_to_ucs;
	pif->pf_ucs_to_utf8 = _ucs_to_utf8;
}
#endif

#ifdef XDK_SUPPORT_ASYNC
void xdk_impl_async(if_async_t* pif)
{
	pif->pf_async_alloc_lapp = _async_alloc_lapp;
	pif->pf_async_release_lapp = _async_release_lapp;
}
#endif

#ifdef XDK_SUPPORT_THREAD
void xdk_impl_thread(if_thread_t* pif)
{
	pif->pf_thread_begin = _thread_begin;
	pif->pf_thread_end = _thread_end;
	pif->pf_thread_sleep = _thread_sleep;
	pif->pf_thread_get_id = _thread_get_id;
	pif->pf_thread_create_tls = _thread_create_tls;
	pif->pf_thread_destroy_tls = _thread_destroy_tls;
	pif->pf_thread_get_tls = _thread_get_tls;
	pif->pf_thread_set_tls = _thread_set_tls;
	pif->pf_thread_join = _thread_join;
}
#ifdef XDK_SUPPORT_THREAD_EVENT
void xdk_impl_thread_event(if_thread_t* pif)
{
	pif->pf_event_create = _event_create;
	pif->pf_event_destroy = _event_destroy;
	pif->pf_event_sign = _event_sign;
	pif->pf_event_wait = _event_wait;
}
#endif
#ifdef XDK_SUPPORT_THREAD_CRITI
void xdk_impl_thread_criti(if_thread_t* pif)
{
	pif->pf_criti_create = _criti_create;
	pif->pf_criti_destroy = _criti_destroy;
	pif->pf_criti_enter = _criti_enter;
	pif->pf_criti_leave = _criti_leave;
	pif->pf_criti_query = _criti_query;
}
#endif
#ifdef XDK_SUPPORT_THREAD_MUTEX
void xdk_impl_thread_mutex(if_thread_t* pif)
{
	pif->pf_mutex_create = _mutex_create;
	pif->pf_mutex_destroy = _mutex_destroy;
	pif->pf_mutex_open = _mutex_open;
	pif->pf_mutex_lock = _mutex_lock;
	pif->pf_mutex_unlock = _mutex_unlock;
}
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
void xdk_impl_thread_semap(if_thread_t* pif)
{
	pif->pf_semap_create = _semap_create;
	pif->pf_semap_destroy = _semap_destroy;
	pif->pf_semap_open = _semap_open;
	pif->pf_semap_lock = _semap_lock;
	pif->pf_semap_unlock = _semap_unlock;
}
#endif
#ifdef XDK_SUPPORT_THREAD_QUEUE
void xdk_impl_thread_queue(if_thread_t* pif)
{
	pif->pf_queue_create = _queue_create;
	pif->pf_queue_destroy = _queue_destroy;
	pif->pf_queue_wait = _queue_wait;
}
#endif
#endif /*XDK_SUPPORT_THREAD*/

#ifdef XDK_SUPPORT_TIMER
void xdk_impl_timer(if_timer_t* pif)
{
	pif->pf_create_timer_queue = _create_timer_queue;
	pif->pf_destroy_timer_queue = _destroy_timer_queue;
	pif->pf_create_timer = _create_timer;
	pif->pf_destroy_timer = _destroy_timer;
	pif->pf_alter_timer = _alter_timer;
}
#endif

#ifdef XDK_SUPPORT_FILE
void xdk_impl_file(if_file_t* pif)
{
	pif->pf_file_open = _file_open;
	pif->pf_file_close = _file_close;
	pif->pf_file_size = _file_size;
	pif->pf_file_read = _file_read;
	pif->pf_file_read_range = _file_read_range;
	pif->pf_file_write_range = _file_write_range;
	pif->pf_file_write = _file_write;
	pif->pf_file_flush = _file_flush;
	pif->pf_file_truncate = _file_truncate;
	pif->pf_file_delete = _file_delete;
	pif->pf_file_rename = _file_rename;
	pif->pf_file_info = _file_info;
	pif->pf_directory_open = _directory_open;
	pif->pf_file_settime = _file_settime;
	pif->pf_file_gettime = _file_gettime;
}
#ifdef XDK_SUPPORT_FILE_FIND
void xdk_impl_file_find(if_file_t* pif)
{
	pif->pf_file_find_first = _file_find_first;
	pif->pf_file_find_next = _file_find_next;
	pif->pf_file_find_close = _file_find_close;
}
#endif

#endif /*XDK_SUPPORT_FILE*/

#ifdef XDK_SUPPORT_SHARE
void xdk_impl_share(if_share_t* pif)
{
	pif->pf_share_srv = _share_srv;
	pif->pf_share_cli = _share_cli;
	pif->pf_share_close = _share_close;
	pif->pf_share_write = _share_write;
	pif->pf_share_read = _share_read;
	pif->pf_share_lock = _share_lock;
	pif->pf_share_unlock = _share_unlock;
}
#endif

#ifdef XDK_SUPPORT_PIPE
void xdk_impl_pipe(if_pipe_t* pif)
{
	pif->pf_pipe_srv = _pipe_srv;
	pif->pf_pipe_listen = _pipe_listen;
	pif->pf_pipe_stop = _pipe_stop;
	pif->pf_pipe_cli = _pipe_cli;
	pif->pf_pipe_close = _pipe_close;
	pif->pf_pipe_wait = _pipe_wait;
	pif->pf_pipe_read = _pipe_read;
	pif->pf_pipe_write = _pipe_write;
	pif->pf_pipe_flush = _pipe_flush;
}
#endif

#ifdef XDK_SUPPORT_COMM
void xdk_impl_comm(if_comm_t* pif)
{
	pif->pf_default_comm_mode = _default_comm_mode;
	pif->pf_set_comm_mode = _set_comm_mode;
	pif->pf_get_comm_mode = _get_comm_mode;
	pif->pf_comm_wait = _comm_wait;
	pif->pf_comm_open = _comm_open;
	pif->pf_comm_close = _comm_close;
	pif->pf_comm_read = _comm_read;
	pif->pf_comm_write = _comm_write;
	pif->pf_comm_flush = _comm_flush;
}
#endif

#ifdef XDK_SUPPORT_CONS
void xdk_impl_cons(if_cons_t* pif)
{
	pif->pf_cons_alloc = _cons_alloc;
	pif->pf_cons_free = _cons_free;
	pif->pf_cons_sigaction = _cons_sigaction;
	pif->pf_cons_read = _cons_read;
	pif->pf_cons_write = _cons_write;
	pif->pf_cons_flush = _cons_flush;
	pif->pf_cons_stdin = _cons_stdin;
	pif->pf_cons_stdout = _cons_stdout;
}
#endif

#ifdef XDK_SUPPORT_SOCK
void xdk_impl_socket(if_socket_t* pif)
{
	pif->pf_socket_startup = _socket_startup;
	pif->pf_socket_cleanup = _socket_cleanup;
	pif->pf_socket_tcp = _socket_tcp;
	pif->pf_socket_udp = _socket_udp;
	pif->pf_socket_icmp = _socket_icmp;
	pif->pf_socket_close = _socket_close;
	pif->pf_socket_wait = _socket_wait;
	pif->pf_socket_shutdown = _socket_shutdown;
	pif->pf_socket_connect = _socket_connect;
	pif->pf_socket_bind = _socket_bind;
	pif->pf_socket_sendto = _socket_sendto;
	pif->pf_socket_recvfrom = _socket_recvfrom;
	pif->pf_socket_recv = _socket_recv;
	pif->pf_socket_send = _socket_send;
	pif->pf_socket_setopt = _socket_setopt;
	pif->pf_socket_getopt = _socket_getopt;
	pif->pf_socket_set_linger = _socket_set_linger;
	pif->pf_socket_set_sndbuf = _socket_set_sndbuf;
	pif->pf_socket_set_rcvbuf = _socket_set_rcvbuf;
	pif->pf_socket_set_nonblk = _socket_set_nonblk;
	pif->pf_fill_addr = _fill_addr;
	pif->pf_conv_addr = _conv_addr;
	pif->pf_host_addr = _host_addr;
	pif->pf_socket_peer = _socket_peer;
	pif->pf_socket_addr = _socket_addr;
	pif->pf_socket_accept = _socket_accept;
	pif->pf_socket_listen = _socket_listen;
	pif->pf_socket_dupli = _socket_dupli;
	pif->pf_socket_share = _socket_share;

	pif->pf_socket_write = _socket_write;
	pif->pf_socket_read = _socket_read;

	pif->pf_socket_error = _socket_error;
}

#endif /*XDK_SUPPORT_SOCK*/

#ifdef XDK_SUPPORT_DATE
void xdk_impl_date(if_date_t* pif)
{
	pif->pf_get_loc_date = _get_loc_date;
	pif->pf_mak_loc_date = _mak_loc_date;
	pif->pf_get_utc_date = _get_utc_date;
	pif->pf_mak_utc_date = _mak_utc_date;
	pif->pf_get_times = _get_times;
	pif->pf_get_ticks = _get_ticks;
	pif->pf_get_timestamp = _get_timestamp;
	pif->pf_utc_date_from_times = _utc_date_from_times;
	pif->pf_utc_date_from_ticks = _utc_date_from_ticks;
	pif->pf_utc_date_from_timestamp = _utc_date_from_timestamp;
}
#endif

#ifdef XDK_SUPPORT_PROCESS
void xdk_impl_process(if_process_t* pif)
{
	pif->pf_free_library = _free_library;
	pif->pf_get_address = _get_address;
	pif->pf_load_library = _load_library;
	pif->pf_get_runpath = _get_runpath;
	pif->pf_create_process = _create_process;
	pif->pf_release_process = _release_process;
	pif->pf_process_waitrun = _process_waitrun;
	pif->pf_process_dupli = _process_dupli;
	pif->pf_process_alloc = _process_alloc;
	pif->pf_process_free = _process_free;
	pif->pf_process_write = _process_write;
	pif->pf_process_read = _process_read;
	pif->pf_release_handle = _release_handle;
	pif->pf_inherit_handle = _inherit_handle;
	pif->pf_read_profile = _read_profile;
	pif->pf_write_profile = _write_profile;
	pif->pf_get_envvar = _get_envvar;
	pif->pf_system_info = _system_info;
}
#endif

#ifdef XDK_SUPPORT_SHELL
void xdk_impl_shell(if_shell_t* pif)
{
	pif->pf_shell_get_curpath = _shell_get_curpath;
	pif->pf_shell_get_runpath = _shell_get_runpath;
	pif->pf_shell_get_docpath = _shell_get_docpath;
	pif->pf_shell_get_apppath = _shell_get_apppath;
	pif->pf_shell_get_tmppath = _shell_get_tmppath;
}

#ifdef XDK_SUPPORT_SHELL_DIALOG
void xdk_impl_shell_dialog(if_shell_t* pif)
{
	pif->pf_shell_get_filename = _shell_get_filename;
	pif->pf_shell_get_pathname = _shell_get_pathname;
}
#endif
#endif /*XDK_SUPPORT_SHELL*/


#ifdef XDK_SUPPORT_CONTEXT
void xdk_impl_context(if_context_t* pif)
{
	pif->pf_context_startup = _context_startup;
	pif->pf_context_cleanup = _context_cleanup;

	pif->pf_create_compatible_context = _create_compatible_context;
	pif->pf_create_display_context = _create_display_context;
	pif->pf_destroy_context = _destroy_context;
	pif->pf_get_device_caps = _get_device_caps;
	pif->pf_render_context = _render_context;

	pif->pf_select_pixmap = _select_pixmap;
	pif->pf_create_compatible_pixmap = _create_compatible_pixmap;
	pif->pf_destroy_pixmap = _destroy_pixmap;

	pif->pf_pt_per_mm = _pt_per_mm;;
	pif->pf_text_mm_metric = _text_mm_metric;
	pif->pf_text_pt_metric = _text_pt_metric;
	pif->pf_text_mm_size = _text_mm_size;
	pif->pf_text_pt_size = _text_pt_size;
	pif->pf_cast_mm_to_pt = _cast_mm_to_pt;
	pif->pf_cast_pt_to_mm = _cast_pt_to_mm;
	pif->pf_font_size = _font_size;
}

#ifdef XDK_SUPPORT_CONTEXT_BITMAP
void xdk_impl_context_bitmap(if_context_t* pif)
{
	pif->pf_destroy_bitmap = _destroy_bitmap;
	pif->pf_get_bitmap_size = _get_bitmap_size;

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
#if defined(XDK_SUPPORT_SHELL)
	pif->pf_load_bitmap_from_thumb = _load_bitmap_from_thumb;
	pif->pf_load_bitmap_from_icon = _load_bitmap_from_icon;
#endif
}
#endif

#ifdef XDK_SUPPORT_CONTEXT_REGION
void xdk_impl_context_region(if_context_t* pif)
{
	pif->pf_create_region = _create_region;
	pif->pf_delete_region = _delete_region;
	pif->pf_pt_in_region = _pt_in_region;
}
#endif

#ifdef XDK_SUPPORT_CONTEXT_PRINTER
void xdk_impl_context_printer(if_context_t* pif)
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

#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC
void xdk_impl_context_graphic(if_context_t* pif)
{
#if defined(UNICODE) || defined(_UNICODE)
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
	pif->pf_gdi_fill_region = _gdiplus_fill_region;
	pif->pf_gdi_text_out = _gdiplus_text_out;
	pif->pf_gdi_text_size = _gdiplus_text_size;
	pif->pf_gdi_text_metric = _gdiplus_text_metric;
	pif->pf_gdi_gradient_rect = _gdiplus_gradient_rect;
	pif->pf_gdi_alphablend_rect = _gdiplus_alphablend_rect;
	pif->pf_gdi_exclip_rect = _gdiplus_exclip_rect;
	pif->pf_gdi_inclip_rect = _gdiplus_inclip_rect;
	pif->pf_gdi_draw_bitmap = _gdiplus_draw_bitmap;
	pif->pf_gdi_draw_image = _gdiplus_draw_image;
#else
	pif->pf_gdi_draw_arrow = _gdi_draw_arrow;
	pif->pf_gdi_draw_ellipse = _gdi_draw_ellipse;
	pif->pf_gdi_draw_line = _gdi_draw_line;
	pif->pf_gdi_draw_pie = _gdi_draw_pie;
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
#ifdef XDK_SUPPORT_CONTEXT_BITMAP
	pif->pf_gdi_draw_bitmap = _gdi_draw_bitmap;
	pif->pf_gdi_draw_image = _gdi_draw_image;
#endif
#ifdef XDK_SUPPORT_CONTEXT_REGION
    pif->pf_gdi_fill_region = _gdi_fill_region;
#endif
#endif
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#ifdef XDK_SUPPORT_CLIPBOARD
void xdk_impl_clipboard(if_clipboard_t* pif)
{
	pif->pf_clipboard_open = _clipboard_open;
	pif->pf_clipboard_close = _clipboard_close;
	pif->pf_clipboard_clean = _clipboard_clean;
	pif->pf_clipboard_get = _clipboard_get;
	pif->pf_clipboard_put = _clipboard_put;
}
#endif

#ifdef XDK_SUPPORT_WIDGET
void xdk_impl_widget(if_widget_t* pif)
{
	pif->pf_fetch_message = _fetch_message;
    pif->pf_peek_message = _peek_message;
	pif->pf_translate_message = _translate_message;
	pif->pf_dispatch_message = _dispatch_message;
	pif->pf_translate_accelerator = _translate_accelerator;
	pif->pf_message_position = _message_position;

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
	pif->pf_widget_resize = _widget_resize;
	pif->pf_widget_paint = _widget_paint;
	pif->pf_widget_redraw = _widget_redraw;
	pif->pf_widget_post_char = _widget_post_char;
	pif->pf_widget_post_key = _widget_post_key;
	pif->pf_widget_set_capture = _widget_set_capture;
	pif->pf_widget_set_imm = _widget_set_imm;
	pif->pf_widget_get_imm = _widget_get_imm;
	pif->pf_widget_set_cursor = _widget_set_cursor;
	pif->pf_widget_set_focus = _widget_set_focus;
	pif->pf_widget_set_title = _widget_set_title;
	pif->pf_widget_get_title = _widget_get_title;
	pif->pf_widget_create_caret = _widget_create_caret;
	pif->pf_widget_destroy_caret = _widget_destroy_caret;
	pif->pf_widget_show_caret = _widget_show_caret;

	pif->pf_widget_enable = _widget_enable;

	pif->pf_widget_post_message = _widget_post_message;
	pif->pf_widget_send_message = _widget_send_message;
	pif->pf_widget_post_command = _widget_post_command;
	pif->pf_widget_send_command = _widget_send_command;

	pif->pf_widget_set_timer = _widget_set_timer;
	pif->pf_widget_kill_timer = _widget_kill_timer;
	pif->pf_widget_adjust_size = _widget_adjust_size;
	pif->pf_widget_calc_border = _widget_calc_border;

	pif->pf_widget_copy = _widget_copy;
	pif->pf_widget_cut = _widget_cut;
	pif->pf_widget_paste = _widget_paste;
	pif->pf_widget_undo = _widget_undo;
	pif->pf_widget_enum_child = _widget_enum_child;

	pif->pf_widget_is_maximized = _widget_is_maximized;
	pif->pf_widget_is_minimized = _widget_is_minimized;

	pif->pf_widget_get_subproc = _widget_get_subproc;
	pif->pf_widget_set_subproc = _widget_set_subproc;
	pif->pf_widget_del_subproc = _widget_del_subproc;
	pif->pf_widget_get_subproc_delta = _widget_get_subproc_delta;
	pif->pf_widget_set_subproc_delta = _widget_set_subproc_delta;
	pif->pf_widget_has_subproc = _widget_has_subproc;
	pif->pf_widget_set_scroll_info = _widget_set_scroll_info;
	pif->pf_widget_get_scroll_info = _widget_get_scroll_info;

	pif->pf_widget_do_modal = _widget_do_modal;
	pif->pf_widget_do_trace = _widget_do_trace;

	pif->pf_get_screen_size = _get_screen_size;
	pif->pf_get_desktop_size = _get_desktop_size;
	pif->pf_screen_size_to_pt = _screen_size_to_pt;
	pif->pf_screen_size_to_tm = _screen_size_to_tm;

	pif->pf_create_accel_table = _create_accel_table;
	pif->pf_destroy_accel_table = _destroy_accel_table;

#ifdef XDK_SUPPORT_CONTEXT_OPENGL
	pif->pf_widget_get_glctx = _widget_get_glctx;
#endif
}

#ifdef XDK_SUPPORT_WIDGET_EX
void xdk_impl_widget_ex(if_widget_t* pif)
{
	pif->pf_widget_track_mouse = _widget_track_mouse;
	pif->pf_widget_set_region = _widget_set_region;
	pif->pf_widget_set_alpha = _widget_set_alpha;
	pif->pf_widget_get_alpha = _widget_get_alpha;
}
#endif

#endif /*XDK_SUPPORT_WIDGET*/
