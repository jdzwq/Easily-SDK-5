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
	pif->pf_async_init = _async_init;
	pif->pf_async_uninit = _async_uninit;
}
#endif

#ifdef XDK_SUPPORT_THREAD
void xdk_impl_thread(if_thread_t* pif)
{
	pif->pf_thread_begin = _thread_begin;
	pif->pf_thread_end = _thread_end;
	pif->pf_thread_sleep = _thread_sleep;
	pif->pf_thread_yield = _thread_yield;
	pif->pf_thread_get_id = _thread_get_id;
	pif->pf_thread_create_tls = _thread_create_tls;
	pif->pf_thread_destroy_tls = _thread_destroy_tls;
	pif->pf_thread_get_tls = _thread_get_tls;
	pif->pf_thread_set_tls = _thread_set_tls;
	pif->pf_thread_join = _thread_join;
    pif->pf_thread_safe = _thread_safe;
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
	pif->pf_mutex_close = _mutex_close;
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
	pif->pf_semap_close = _semap_close;
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
	pif->pf_comm_listen = _comm_listen;
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
	pif->pf_socket_get_nonblk = _socket_get_nonblk;
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
	pif->pf_get_utc_date = _get_utc_date;
	pif->pf_mak_loc_week = _mak_loc_week;
	pif->pf_mak_utc_week = _mak_utc_week;
	pif->pf_loc_date_to_utc = _loc_date_to_utc;
	pif->pf_utc_date_to_loc = _utc_date_to_loc;
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
    pif->pf_process_safe = _process_safe;
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

