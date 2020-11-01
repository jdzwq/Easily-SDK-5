/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdk interface document

	@module	xdkinf.h | interface file

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

#ifndef _XDKINF_H
#define	_XDKINF_H

#include "xdkdef.h"

#ifdef XDK_SUPPORT_MEMO
/*memory interface*/
#ifdef XDK_SUPPORT_MEMO_HEAP
typedef res_heap_t(*PF_PROCESS_HEAP)(void);
typedef res_heap_t(*PF_HEAP_CREATE)(void);
typedef void(*PF_HEAP_DESTROY)(res_heap_t);
typedef void(*PF_HEAP_CLEAN)(res_heap_t);
typedef void* (*PF_HEAP_ALLOC)(res_heap_t, dword_t);
typedef void* (*PF_HEAP_REALLOC)(res_heap_t, void*, dword_t);
typedef void(*PF_HEAP_FREE)(res_heap_t, void*);
typedef void(*PF_HEAP_ZERO)(res_heap_t, void*, dword_t);
#endif
#ifdef XDK_SUPPORT_MEMO_GLOB
typedef res_glob_t (*PF_GLOB_ALLOC)(dword_t);
typedef res_glob_t(*PF_GLOB_REALLOC)(res_glob_t, dword_t);
typedef void (*PF_GLOB_FREE)(res_glob_t);
typedef dword_t(*PF_GLOB_SIZE)(res_glob_t);
typedef void* (*PF_GLOB_LOCK)(res_glob_t);
typedef bool_t (*PF_GLOB_UNLOCK)(res_glob_t);
#endif
#ifdef XDK_SUPPORT_MEMO_LOCAL
typedef void* (*PF_LOCAL_ALLOC)(dword_t);
typedef void* (*PF_LOCAL_REALLOC)(void*, dword_t);
typedef void(*PF_LOCAL_FREE)(void*);
#endif
#ifdef XDK_SUPPORT_MEMO_PAGE
typedef void* (*PF_PAGE_ALLOC)(dword_t);
typedef void* (*PF_PAGE_REALLOC)(void*, dword_t);
typedef void(*PF_PAGE_FREE)(void*);
typedef dword_t(*PF_PAGE_SIZE)(void*);
typedef void* (*PF_PAGE_LOCK)(void*);
typedef void(*PF_PAGE_UNLOCK)(void*);
typedef bool_t(*PF_PAGE_PROTECT)(void*, bool_t);
#endif
#ifdef XDK_SUPPORT_MEMO_CACHE
typedef void*(*PF_CACHE_OPEN)(void);
typedef void(*PF_CACHE_CLOSE)(void*);
typedef bool_t(*PF_CACHE_WRITE)(void*, dword_t, dword_t, void*, dword_t, dword_t*);
typedef bool_t(*PF_CACHE_READ)(void*, dword_t, dword_t, void*, dword_t, dword_t*);
#endif

typedef struct _if_memo_t{
#ifdef XDK_SUPPORT_MEMO_HEAP
	PF_PROCESS_HEAP		pf_process_heap;
	PF_HEAP_CREATE		pf_heap_create;
	PF_HEAP_DESTROY		pf_heap_destroy;
	PF_HEAP_CLEAN		pf_heap_clean;
	PF_HEAP_ALLOC		pf_heap_alloc;
	PF_HEAP_REALLOC		pf_heap_realloc;
	PF_HEAP_FREE		pf_heap_free;
	PF_HEAP_ZERO		pf_heap_zero;
#endif
#ifdef XDK_SUPPORT_MEMO_GLOB
	PF_GLOB_ALLOC		pf_glob_alloc;
	PF_GLOB_REALLOC		pf_glob_realloc;
	PF_GLOB_SIZE		pf_glob_size;
	PF_GLOB_FREE		pf_glob_free;
	PF_GLOB_LOCK		pf_glob_lock;
	PF_GLOB_UNLOCK		pf_glob_unlock;
#endif
#ifdef XDK_SUPPORT_MEMO_LOCAL
	PF_LOCAL_ALLOC		pf_local_alloc;
	PF_LOCAL_REALLOC	pf_local_realloc;
	PF_LOCAL_FREE		pf_local_free;
#endif
#ifdef XDK_SUPPORT_MEMO_PAGE
	PF_PAGE_ALLOC		pf_page_alloc;
	PF_PAGE_REALLOC		pf_page_realloc;
	PF_PAGE_FREE		pf_page_free;
	PF_PAGE_SIZE		pf_page_size;
	PF_PAGE_LOCK		pf_page_lock;
	PF_PAGE_UNLOCK		pf_page_unlock;
	PF_PAGE_PROTECT		pf_page_protect;
#endif
#ifdef XDK_SUPPORT_MEMO_CACHE
	PF_CACHE_OPEN		pf_cache_open;
	PF_CACHE_CLOSE		pf_cache_close;
	PF_CACHE_WRITE		pf_cache_write;
	PF_CACHE_READ		pf_cache_read;
#endif
}if_memo_t;
#endif /*XDK_SUPPORT_MEMO*/


#ifdef XDK_SUPPORT_ERROR
/*error interface*/
typedef int (*PF_ERROR_TEXT)(tchar_t*, int);
typedef void(*PF_ERROR_EXIT)(void);
typedef void(*PF_ERROR_DEBUG)(const tchar_t*);
typedef void(*PF_ERROR_PRINT)(const char*);
typedef void(STDCALL *PF_ERROR_TRACE)(void*, const tchar_t*, const tchar_t*);

typedef struct _if_error_t{
	PF_ERROR_TEXT	pf_error_text;
	PF_ERROR_EXIT	pf_error_exit;
	PF_ERROR_DEBUG	pf_error_debug;
	PF_ERROR_PRINT	pf_error_print;

	PF_ERROR_TRACE	 pf_error_trace;
	void* param;
}if_error_t;
#endif

#ifdef XDK_SUPPORT_MBCS
/*mbcs interface*/
typedef int(*PF_GBK_TO_UCS)(const schar_t*, int, wchar_t*, int);
typedef int(*PF_UCS_TO_GBK)(const wchar_t*, int, schar_t*, int);
typedef int(*PF_UTF8_TO_UCS)(const schar_t*, int, wchar_t*, int);
typedef int(*PF_UCS_TO_UTF8)(const wchar_t*, int, schar_t*, int);

typedef struct _if_mbcs_t{
	PF_GBK_TO_UCS	pf_gbk_to_ucs;
	PF_UCS_TO_GBK	pf_ucs_to_gbk;
	PF_UTF8_TO_UCS	pf_utf8_to_ucs;
	PF_UCS_TO_UTF8	pf_ucs_to_utf8;
}if_mbcs_t;
#endif

#ifdef XDK_SUPPORT_ASYNC
typedef async_t*(*PF_ASYNC_ALLOC_LAPP)(int, int, res_file_t);
typedef void(*PF_ASYNC_FREE_LAPP)(async_t*);

typedef struct _if_async_t{
	PF_ASYNC_ALLOC_LAPP	pf_async_alloc_lapp;
	PF_ASYNC_FREE_LAPP	pf_async_free_lapp;
}if_async_t;
#endif

#ifdef XDK_SUPPORT_THREAD
/*thread interface*/
typedef void (*PF_THREAD_BEGIN)(res_thread_t*, PF_THREADFUNC, void*);
typedef void (*PF_THREAD_END)(void);
typedef void (*PF_THREAD_CREATE_TLS)(tls_key_t*);
typedef void(*PF_THREAD_DESTROY_TLS)(tls_key_t);
typedef void* (*PF_THREAD_GET_TLS)(tls_key_t);
typedef void(*PF_THREAD_SET_TLS)(tls_key_t, void*);
typedef pid_t (* PF_THREAD_GET_ID)(void);
typedef void (* PF_THREAD_SLEEP)(int);
typedef void(*PF_THREAD_YIELD)(void);
typedef void(*PF_THREAD_JOIN)(res_thread_t);
typedef void (*PF_THREAD_SAFE)(void);
#ifdef XDK_SUPPORT_THREAD_EVENT
typedef res_even_t (*PF_EVENT_CREATE)(void);
typedef void (*PF_EVENT_DESTROY)(res_even_t);
typedef wait_t (*PF_EVENT_WAIT)(res_even_t, int);
typedef void (*PF_EVENT_SIGN)(res_even_t, bool_t);
#endif
#ifdef XDK_SUPPORT_THREAD_CRITI
typedef res_crit_t (*PF_CRITI_CREATE)(void);
typedef void (*PF_CRITI_DESTROY)(res_crit_t);
typedef bool_t (*PF_CRITI_QUERY)(res_crit_t);
typedef void (*PF_CRITI_ENTER)(res_crit_t);
typedef void (*PF_CRITI_LEAVE)(res_crit_t);
#endif
#ifdef XDK_SUPPORT_THREAD_MUTEX
typedef res_mutx_t	(*PF_MUTEX_CREATE)(const tchar_t*);
typedef void		(*PF_MUTEX_DESTROY)(const tchar_t*, res_mutx_t);
typedef res_mutx_t	(*PF_MUTEX_OPEN)(const tchar_t*);
typedef void		(*PF_MUTEX_CLOSE)(res_mutx_t);
typedef wait_t(*PF_MUTEX_LOCK)(res_mutx_t, int);
typedef void		(*PF_MUTEX_UNLOCK)(res_mutx_t);
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
typedef res_sema_t(*PF_SEMAP_CREATE)(const tchar_t*, int max);
typedef void(*PF_SEMAP_DESTROY)(const tchar_t*, res_sema_t);
typedef res_sema_t(*PF_SEMAP_OPEN)(const tchar_t*);
typedef void(*PF_SEMAP_CLOSE)(res_sema_t);
typedef wait_t(*PF_SEMAP_LOCK)(res_sema_t, int);
typedef void(*PF_SEMAP_UNLOCK)(res_sema_t);
#endif
#ifdef XDK_SUPPORT_THREAD_QUEUE
typedef res_queue_t(*PF_QUEUE_CREATE)(res_queue_t, res_file_t, int);
typedef void(*PF_QUEUE_DESTROY)(res_queue_t);
typedef wait_t(*PF_QUEUE_WAIT)(res_queue_t, int);
#endif
typedef struct _if_thread_t{
	PF_THREAD_BEGIN		pf_thread_begin;
	PF_THREAD_END		pf_thread_end;
	PF_THREAD_SLEEP		pf_thread_sleep;
	PF_THREAD_YIELD		pf_thread_yield;
	PF_THREAD_CREATE_TLS pf_thread_create_tls;
	PF_THREAD_DESTROY_TLS pf_thread_destroy_tls;
	PF_THREAD_GET_TLS	pf_thread_get_tls;
	PF_THREAD_SET_TLS	pf_thread_set_tls;
	PF_THREAD_GET_ID	pf_thread_get_id;
	PF_THREAD_JOIN		pf_thread_join;
    PF_THREAD_SAFE      pf_thread_safe;
#ifdef XDK_SUPPORT_THREAD_EVENT
	PF_EVENT_CREATE		pf_event_create;
	PF_EVENT_DESTROY	pf_event_destroy;
	PF_EVENT_WAIT		pf_event_wait;
	PF_EVENT_SIGN		pf_event_sign;
#endif
#ifdef XDK_SUPPORT_THREAD_CRITI
	PF_CRITI_CREATE		pf_criti_create;
	PF_CRITI_DESTROY	pf_criti_destroy;
	PF_CRITI_QUERY		pf_criti_query;
	PF_CRITI_ENTER		pf_criti_enter;
	PF_CRITI_LEAVE		pf_criti_leave;
#endif
#ifdef XDK_SUPPORT_THREAD_MUTEX
	PF_MUTEX_CREATE		pf_mutex_create;
	PF_MUTEX_DESTROY	pf_mutex_destroy;
	PF_MUTEX_OPEN		pf_mutex_open;
	PF_MUTEX_CLOSE		pf_mutex_close;
	PF_MUTEX_LOCK		pf_mutex_lock;
	PF_MUTEX_UNLOCK		pf_mutex_unlock;
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
	PF_SEMAP_CREATE		pf_semap_create;
	PF_SEMAP_DESTROY	pf_semap_destroy;
	PF_SEMAP_OPEN		pf_semap_open;
	PF_SEMAP_CLOSE		pf_semap_close;
	PF_SEMAP_LOCK		pf_semap_lock;
	PF_SEMAP_UNLOCK		pf_semap_unlock;
#endif
#ifdef XDK_SUPPORT_THREAD_QUEUE
	PF_QUEUE_CREATE		pf_queue_create;
	PF_QUEUE_DESTROY	pf_queue_destroy;
	PF_QUEUE_WAIT		pf_queue_wait;
#endif
}if_thread_t;
#endif /*XDK_SUPP0RT_THREAD*/

#ifdef XDK_SUPPORT_TIMER
/*timer interface*/
typedef res_queue_t(*PF_CREATE_TIMER_QUEUE)(void);
typedef void(*PF_DESTROY_TIMER_QUEUE)(res_queue_t);
typedef res_timer_t(*PF_CREATE_TIMER)(res_queue_t, dword_t, dword_t, PF_TIMERFUNC, void*);
typedef void(*PF_DESTROY_TIMER)(res_queue_t, res_timer_t);
typedef bool_t(*PF_ALTER_TIMER)(res_queue_t, res_timer_t, dword_t, dword_t);

typedef struct _if_timer_t{
	PF_CREATE_TIMER_QUEUE		pf_create_timer_queue;
	PF_DESTROY_TIMER_QUEUE		pf_destroy_timer_queue;
	PF_CREATE_TIMER				pf_create_timer;
	PF_DESTROY_TIMER			pf_destroy_timer;
	PF_ALTER_TIMER				pf_alter_timer;
}if_timer_t;
#endif /*XDK_SUPPORT_TIMER*/

#ifdef XDK_SUPPORT_SOCK
/*socket interface*/
typedef bool_t (*PF_SOCKET_STARTUP)(void);
typedef void (*PF_SOCKET_CLEANUP)(void);
typedef res_file_t (*PF_SOCKET_TCP)(int, dword_t);
typedef res_file_t(*PF_SOCKET_UDP)(int, dword_t);
typedef res_file_t(*PF_SOCKET_ICMP)(int, dword_t);
typedef void (*PF_SOCKET_SHUTDOWN)(res_file_t, int);
typedef void (*PF_SOCKET_CLOSE)(res_file_t);
typedef dword_t(*PF_SOCKET_WAIT)(res_file_t, dword_t, int);
typedef bool_t (*PF_SOCKET_BIND)(res_file_t, res_addr_t, int);
typedef bool_t (*PF_SOCKET_CONNECT)(res_file_t, res_addr_t, int);
typedef bool_t(*PF_SOCKET_SENDTO)(res_file_t, res_addr_t, int, void*, dword_t, async_t*);
typedef bool_t(*PF_SOCKET_RECVFROM)(res_file_t, res_addr_t, int*, void*, dword_t, async_t*);
typedef bool_t(*PF_SOCKET_SEND)(res_file_t, void*, dword_t, async_t*);
typedef bool_t(*PF_SOCKET_RECV)(res_file_t, void*, dword_t, async_t*);
typedef bool_t (*PF_SOCKET_SETOPT)(res_file_t, int, const char*, int);
typedef bool_t (*PF_SOCKET_GETOPT)(res_file_t, int, char*, int*);
typedef bool_t (*PF_SOCKET_SET_LINGER)(res_file_t, bool_t, int);
typedef bool_t (*PF_SOCKET_SET_SNDBUF)(res_file_t, int);
typedef bool_t (*PF_SOCKET_SET_RCVBUF)(res_file_t, int);
typedef bool_t (*PF_SOCKET_SET_NONBLK)(res_file_t, bool_t);
typedef bool_t(*PF_SOCKET_GET_NONBLK)(res_file_t);
typedef bool_t (*PF_HOST_ADDR)(const schar_t*, schar_t*);
typedef void (*PF_FILL_ADDR)(net_addr_t*, unsigned short, const schar_t*);
typedef void(*PF_CONV_ADDR)(net_addr_t*, unsigned short*, schar_t*);
typedef void(*PF_SOCKET_ADDR)(res_file_t, net_addr_t*);
typedef void(*PF_SOCKET_PEER)(res_file_t, net_addr_t*);
typedef bool_t (*PF_SOCKET_LISTEN)(res_file_t, int);
typedef res_file_t (*PF_SOCKET_ACCEPT)(res_file_t, res_addr_t, int*, async_t*);
typedef int(*PF_SOCKET_WRITE)(void* pso, unsigned char* buf, int len);
typedef int(*PF_SOCKET_READ)(void* pso, unsigned char* buf, int len);
typedef int(*PF_SOCKET_ERROR)(tchar_t*, int);
typedef bool_t(*PF_SOCKET_SHARE)(pid_t, res_file_t, res_file_t, void*, dword_t);
typedef res_file_t(*PF_SOCKET_DUPLI)(res_file_t, void*, dword_t*);

typedef struct _if_socket_t{
	PF_SOCKET_STARTUP		pf_socket_startup;
	PF_SOCKET_CLEANUP		pf_socket_cleanup;
	PF_SOCKET_TCP			pf_socket_tcp;
	PF_SOCKET_UDP			pf_socket_udp;
	PF_SOCKET_ICMP			pf_socket_icmp;
	PF_SOCKET_CLOSE			pf_socket_close;
	PF_SOCKET_WAIT			pf_socket_wait;
	PF_SOCKET_SHUTDOWN		pf_socket_shutdown;
	PF_SOCKET_BIND			pf_socket_bind;
	PF_SOCKET_CONNECT		pf_socket_connect;
	PF_SOCKET_SENDTO		pf_socket_sendto;
	PF_SOCKET_RECVFROM		pf_socket_recvfrom;
	PF_SOCKET_SEND			pf_socket_send;
	PF_SOCKET_RECV			pf_socket_recv;
	PF_SOCKET_SETOPT		pf_socket_setopt;
	PF_SOCKET_GETOPT		pf_socket_getopt;
	PF_SOCKET_SET_LINGER	pf_socket_set_linger;
	PF_SOCKET_SET_SNDBUF	pf_socket_set_sndbuf;
	PF_SOCKET_SET_RCVBUF	pf_socket_set_rcvbuf;
	PF_SOCKET_SET_NONBLK	pf_socket_set_nonblk;
	PF_SOCKET_GET_NONBLK	pf_socket_get_nonblk;
	PF_HOST_ADDR			pf_host_addr;
	PF_FILL_ADDR			pf_fill_addr;
	PF_CONV_ADDR			pf_conv_addr;
	PF_SOCKET_ADDR			pf_socket_addr;
	PF_SOCKET_PEER			pf_socket_peer;
	PF_SOCKET_LISTEN		pf_socket_listen;
	PF_SOCKET_ACCEPT		pf_socket_accept;
	PF_SOCKET_WRITE			pf_socket_write;
	PF_SOCKET_READ			pf_socket_read;
	PF_SOCKET_ERROR			pf_socket_error;
	PF_SOCKET_DUPLI			pf_socket_dupli;
	PF_SOCKET_SHARE			pf_socket_share;
}if_socket_t;
#endif /*XDK_SUPPORT_SOCK*/

#ifdef XDK_SUPPORT_FILE
/*file interface*/
typedef res_file_t (*PF_FILE_OPEN)(const tchar_t*, dword_t);
typedef void (*PF_FILE_CLOSE)(res_file_t);
typedef bool_t(*PF_FILE_SIZE)(res_file_t, dword_t*, dword_t*);
typedef bool_t(*PF_FILE_WRITE)(res_file_t, void*, dword_t, async_t*);
typedef bool_t(*PF_FILE_FLUSH)(res_file_t);
typedef bool_t(*PF_FILE_READ)(res_file_t, void*, dword_t, async_t*);
typedef bool_t(*PF_FILE_READ_RANGE)(res_file_t, dword_t, dword_t, void*, dword_t);
typedef bool_t(*PF_FILE_WRITE_RANGE)(res_file_t, dword_t, dword_t, void*, dword_t);
typedef bool_t(*PF_FILE_TRUNCATE)(res_file_t, dword_t, dword_t);
typedef bool_t (*PF_FILE_DELETE)(const tchar_t*);
typedef bool_t(*PF_FILE_RENAME)(const tchar_t*, const tchar_t*);
typedef bool_t (*PF_FILE_INFO)(const tchar_t*, file_info_t*);
typedef bool_t (*PF_FILE_SETTIME)(res_file_t fh, const xdate_t* pdt);
typedef bool_t(*PF_FILE_GETTIME)(res_file_t fh, xdate_t* pdt);
typedef bool_t(*PF_DIRECTORY_OPEN)(const tchar_t*, dword_t);
typedef bool_t(*PF_DIRECTORY_CREATE)(const tchar_t*);
typedef bool_t(*PF_DIRECTORY_REMOVE)(const tchar_t*);

#ifdef XDK_SUPPORT_FILE_FIND
typedef res_find_t(*PF_FILE_FIND_FIRST)(const tchar_t*, file_info_t*);
typedef bool_t(*PF_FILE_FIND_NEXT)(res_find_t, file_info_t*);
typedef void (*PF_FILE_FIND_CLOSE)(res_find_t);
#endif

typedef struct _if_file_t{
	PF_FILE_OPEN		pf_file_open;
	PF_FILE_CLOSE		pf_file_close;
	PF_FILE_WRITE		pf_file_write;
	PF_FILE_SIZE		pf_file_size;
	PF_FILE_FLUSH		pf_file_flush;
	PF_FILE_READ		pf_file_read;
	PF_FILE_READ_RANGE	pf_file_read_range;
	PF_FILE_WRITE_RANGE	pf_file_write_range;
	PF_FILE_TRUNCATE	pf_file_truncate;
	PF_FILE_DELETE		pf_file_delete;
	PF_FILE_RENAME		pf_file_rename;
	PF_FILE_INFO		pf_file_info;
	PF_FILE_SETTIME		pf_file_settime;
	PF_FILE_GETTIME		pf_file_gettime;
	PF_DIRECTORY_OPEN	pf_directory_open;
	PF_DIRECTORY_CREATE	pf_directory_create;
	PF_DIRECTORY_REMOVE	pf_directory_remove;

#ifdef XDK_SUPPORT_FILE_FIND
	PF_FILE_FIND_FIRST	pf_file_find_first;
	PF_FILE_FIND_NEXT	pf_file_find_next;
	PF_FILE_FIND_CLOSE	pf_file_find_close;
#endif

}if_file_t;
#endif /*XDK_SUPPORT_FILE*/


#ifdef XDK_SUPPORT_SHARE
typedef res_file_t(*PF_SHARE_SRV)(const tchar_t*, const tchar_t*, dword_t, dword_t, dword_t);
typedef void(*PF_SHARE_CLOSE)(const tchar_t*, res_file_t);
typedef res_file_t(*PF_SHARE_CLI)(const tchar_t*, dword_t);
typedef bool_t(*PF_SHARE_WRITE)(res_file_t, dword_t, void*, dword_t, dword_t*);
typedef bool_t(*PF_SHARE_READ)(res_file_t, dword_t, void*, dword_t, dword_t*);
typedef void*(*PF_SHARE_LOCK)(res_file_t, dword_t, dword_t);
typedef void(*PF_SHARE_UNLOCK)(res_file_t, dword_t, dword_t, void*);

typedef struct _if_share_t{
	PF_SHARE_SRV		pf_share_srv;
	PF_SHARE_CLI		pf_share_cli;
	PF_SHARE_CLOSE		pf_share_close;
	PF_SHARE_WRITE		pf_share_write;
	PF_SHARE_READ		pf_share_read;
	PF_SHARE_LOCK		pf_share_lock;
	PF_SHARE_UNLOCK		pf_share_unlock;
}if_share_t;
#endif 

#ifdef XDK_SUPPORT_PIPE
/*pipe interface*/
typedef res_file_t (*PF_PIPE_SRV)(const tchar_t*, dword_t);
typedef bool_t(*PF_PIPE_LISTEN)(res_file_t, async_t*);
typedef void(*PF_PIPE_STOP)(res_file_t);
typedef res_file_t (*PF_PIPE_CLI)(const tchar_t*, dword_t);
typedef void(*PF_PIPE_CLOSE)(const tchar_t*, res_file_t);
typedef bool_t(*PF_PIPE_WRITE)(res_file_t, void*, dword_t, async_t*);
typedef bool_t(*PF_PIPE_FLUSH)(res_file_t);
typedef bool_t(*PF_PIPE_READ)(res_file_t, void*, dword_t, async_t*);
typedef wait_t(*PF_PIPE_WAIT)(const tchar_t*, int);

typedef struct _if_pipe_t{
	PF_PIPE_SRV			pf_pipe_srv;
	PF_PIPE_CLI			pf_pipe_cli;
	PF_PIPE_CLOSE		pf_pipe_close;
	PF_PIPE_LISTEN		pf_pipe_listen;
	PF_PIPE_STOP		pf_pipe_stop;
	PF_PIPE_WRITE		pf_pipe_write;
	PF_PIPE_FLUSH		pf_pipe_flush;
	PF_PIPE_READ		pf_pipe_read;
	PF_PIPE_WAIT		pf_pipe_wait;
}if_pipe_t;
#endif /*XDK_SUPPORT_PIPE*/

#ifdef XDK_SUPPORT_COMM
typedef void(*PF_DEFAULT_COMM_MODE)(dev_com_t*);
typedef bool_t(*PF_GET_COMM_MODE)(res_file_t, dev_com_t*);
typedef bool_t(*PF_SET_COMM_MODE)(res_file_t, const dev_com_t*);
typedef res_file_t(*PF_COMM_OPEN)(const tchar_t*, dword_t);
typedef void(*PF_COMM_CLOSE)(res_file_t);
typedef dword_t(*PF_COMM_LISTEN)(res_file_t, async_t*);
typedef bool_t(*PF_COMM_READ)(res_file_t, void*, dword_t, async_t*);
typedef bool_t(*PF_COMM_WRITE)(res_file_t, void*, dword_t, async_t*);
typedef bool_t(*PF_COMM_FLUSH)(res_file_t);

typedef struct _if_comm_t{
	PF_DEFAULT_COMM_MODE	pf_default_comm_mode;
	PF_GET_COMM_MODE		pf_get_comm_mode;
	PF_SET_COMM_MODE		pf_set_comm_mode;
	PF_COMM_LISTEN		pf_comm_listen;
	PF_COMM_OPEN		pf_comm_open;
	PF_COMM_CLOSE		pf_comm_close;
	PF_COMM_READ		pf_comm_read;
	PF_COMM_WRITE		pf_comm_write;
	PF_COMM_FLUSH		pf_comm_flush;
}if_comm_t;
#endif

#ifdef XDK_SUPPORT_CONS
/*console interface*/
typedef res_file_t (*PF_CONS_ALLOC)(tchar_t*, int);
typedef void (*PF_CONS_FREE)(res_file_t);
typedef bool_t(*PF_CONS_SIGACTION)(res_file_t, PF_SIGHANDLER);
typedef bool_t(*PF_CONS_WRITE)(res_file_t, void*, dword_t, dword_t*);
typedef bool_t(*PF_CONS_FLUSH)(res_file_t);
typedef bool_t(*PF_CONS_READ)(res_file_t, void*, dword_t, dword_t*);
typedef res_file_t (*PF_CONS_STDOUT)(res_file_t);
typedef res_file_t (*PF_CONS_STDIN)(res_file_t);

typedef struct _if_cons_t{
	PF_CONS_ALLOC		pf_cons_alloc;
	PF_CONS_FREE		pf_cons_free;
	PF_CONS_SIGACTION	pf_cons_sigaction;
	PF_CONS_WRITE		pf_cons_write;
	PF_CONS_FLUSH		pf_cons_flush;
	PF_CONS_READ		pf_cons_read;
	PF_CONS_STDOUT		pf_cons_stdout;
	PF_CONS_STDIN		pf_cons_stdin;
}if_cons_t;
#endif /*XDK_SUPPORT_CONS*/

#ifdef XDK_SUPPORT_DATE
/*date interface*/
typedef void (*PF_GET_LOC_DATE)(xdate_t*);
typedef void (*PF_GET_UTC_DATE)(xdate_t*);
typedef bool_t(*PF_MAK_LOC_WEEK)(xdate_t*);
typedef bool_t(*PF_MAK_UTC_WEEK)(xdate_t*);
typedef bool_t(*PF_LOC_DATE_TO_UTC)(xdate_t*);
typedef bool_t(*PF_UTC_DATE_TO_LOC)(xdate_t*);
typedef dword_t(*PF_GET_TIMES)(void);
typedef clock_t(*PF_GET_TICKS)(void);
typedef stamp_t(*PF_GET_TIMESTAMP)(void);
typedef void(*PF_UTC_DATE_FROM_TIMES)(xdate_t*, dword_t);
typedef void(*PF_UTC_DATE_FROM_TICKS)(xdate_t*, clock_t);
typedef void(*PF_UTC_DATE_FROM_TIMESTAMP)(xdate_t*, stamp_t);

typedef struct _if_date_t{
	PF_GET_LOC_DATE		pf_get_loc_date;
	PF_GET_UTC_DATE		pf_get_utc_date;
	PF_MAK_LOC_WEEK		pf_mak_loc_week;
	PF_MAK_UTC_WEEK		pf_mak_utc_week;
	PF_LOC_DATE_TO_UTC	pf_loc_date_to_utc;
	PF_UTC_DATE_TO_LOC	pf_utc_date_to_loc;
	PF_GET_TIMES		pf_get_times;
	PF_GET_TICKS		pf_get_ticks;
	PF_GET_TIMESTAMP	pf_get_timestamp;

	PF_UTC_DATE_FROM_TIMES	pf_utc_date_from_times;
	PF_UTC_DATE_FROM_TICKS	pf_utc_date_from_ticks;
	PF_UTC_DATE_FROM_TIMESTAMP pf_utc_date_from_timestamp;
}if_date_t;
#endif /*XDK_SUPPORT_DATE*/

#ifdef XDK_SUPPORT_PROCESS
/*moudle interface*/
typedef res_modu_t(*PF_LOAD_LIBRARY)(const tchar_t*);
typedef void (*PF_FREE_LIBRARY)(res_modu_t);
typedef void* (*PF_GET_ADDRESS)(res_modu_t, const schar_t*);
typedef void (*PF_GET_RUNPATH)(res_modu_t, tchar_t*, int);
typedef bool_t (*PF_CREATE_PROCESS)(const tchar_t*, const tchar_t*, int, proc_info_t*);
typedef void (*PF_RELEASE_PROCESS)(proc_info_t*);
typedef void(*PF_PROCESS_WAITRUN)(res_proc_t);
typedef res_file_t(*PF_PROCESS_DUPLI)(res_proc_t, res_file_t);
typedef void* (*PF_PROCESS_ALLOC)(res_proc_t, dword_t);
typedef void(*PF_PROCESS_FREE)(res_proc_t, void*);
typedef bool_t(*PF_PROCESS_WRITE)(res_proc_t, void*, void*, dword_t);
typedef bool_t(*PF_PROCESS_READ)(res_proc_t, void*, void*, dword_t);
typedef void (*PF_PROCESS_SAFE)(void);
typedef void(*PF_RELEASE_HANDLE)(res_file_t);
typedef bool_t(*PF_INHERIT_HANDLE)(res_file_t, bool_t);
typedef void(*PF_WRITE_PROFILE)(const tchar_t*, const tchar_t*, const tchar_t*, const tchar_t*);
typedef void(*PF_READ_PROFILE)(const tchar_t*, const tchar_t*, const tchar_t*, tchar_t*, int);
typedef int(*PF_GET_ENVVAR)(const tchar_t*, tchar_t*, int);
typedef void(*PF_SYSTEM_INFO)(sys_info_t*);

typedef struct _if_process_t{
	PF_LOAD_LIBRARY		pf_load_library;
	PF_FREE_LIBRARY		pf_free_library;
	PF_GET_ADDRESS		pf_get_address;
	PF_GET_RUNPATH		pf_get_runpath;
	PF_CREATE_PROCESS	pf_create_process;
	PF_RELEASE_PROCESS	pf_release_process;
    PF_PROCESS_SAFE     pf_process_safe;
	PF_PROCESS_WAITRUN	pf_process_waitrun;
	PF_PROCESS_DUPLI	pf_process_dupli;
	PF_PROCESS_ALLOC	pf_process_alloc;
	PF_PROCESS_FREE		pf_process_free;
	PF_PROCESS_WRITE	pf_process_write;
	PF_PROCESS_READ		pf_process_read;
	PF_RELEASE_HANDLE	pf_release_handle;
	PF_INHERIT_HANDLE	pf_inherit_handle;
	PF_WRITE_PROFILE	pf_write_profile;
	PF_READ_PROFILE		pf_read_profile;
	PF_GET_ENVVAR		pf_get_envvar;
	PF_SYSTEM_INFO		pf_system_info;
}if_process_t;
#endif /*XDK_SUPPORT_PROCESS*/


#ifdef	__cplusplus
extern "C" {
#endif

#ifdef XDK_SUPPORT_MEMO
#ifdef XDK_SUPPORT_MEMO_HEAP
	EXP_API void xdk_impl_memo_heap(if_memo_t* pif);
#endif
#ifdef XDK_SUPPORT_MEMO_GLOB
	EXP_API void xdk_impl_memo_glob(if_memo_t* pif);
#endif
#ifdef XDK_SUPPORT_MEMO_GLOB
	EXP_API void xdk_impl_memo_local(if_memo_t* pif);
#endif
#ifdef XDK_SUPPORT_MEMO_PAGE
	EXP_API void xdk_impl_memo_page(if_memo_t* pif);
#endif
#ifdef XDK_SUPPORT_MEMO_CACHE
	EXP_API void xdk_impl_memo_cache(if_memo_t* pif);
#endif
#endif /*XDK_SUPPORT_MEMO*/

#ifdef XDK_SUPPORT_ERROR
	EXP_API void xdk_impl_error(if_error_t* pif);
#endif

#ifdef XDK_SUPPORT_MBCS
	EXP_API void xdk_impl_mbcs(if_mbcs_t* pif);
#endif

#ifdef XDK_SUPPORT_ASYNC
	EXP_API void xdk_impl_async(if_async_t* pif);
#endif

#ifdef XDK_SUPPORT_THREAD
	EXP_API void xdk_impl_thread(if_thread_t* pif);
#ifdef XDK_SUPPORT_THREAD_EVENT
	EXP_API void xdk_impl_thread_event(if_thread_t* pif);
#endif
#ifdef XDK_SUPPORT_THREAD_CRITI
	EXP_API void xdk_impl_thread_criti(if_thread_t* pif);
#endif
#ifdef XDK_SUPPORT_THREAD_MUTEX
	EXP_API void xdk_impl_thread_mutex(if_thread_t* pif);
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
	EXP_API void xdk_impl_thread_semap(if_thread_t* pif);
#endif
#ifdef XDK_SUPPORT_THREAD_QUEUE
	EXP_API void xdk_impl_thread_queue(if_thread_t* pif);
#endif
#endif /*XDK_SUPPORT_THREAD*/

#ifdef XDK_SUPPORT_TIMER
	EXP_API void xdk_impl_timer(if_timer_t* pif);
#endif

#ifdef XDK_SUPPORT_FILE
	EXP_API void xdk_impl_file(if_file_t* pif);
#ifdef XDK_SUPPORT_FILE_FIND
	EXP_API void xdk_impl_file_find(if_file_t* pif);
#endif
#endif /*XDK_SUPPORT_FILE*/

#ifdef XDK_SUPPORT_SHARE
	EXP_API void xdk_impl_share(if_share_t* pif);
#endif

#ifdef XDK_SUPPORT_PIPE
	EXP_API void xdk_impl_pipe(if_pipe_t* pif);
#endif

#ifdef XDK_SUPPORT_COMM
	EXP_API void xdk_impl_comm(if_comm_t* pif);
#endif

#ifdef XDK_SUPPORT_CONS
	EXP_API void xdk_impl_cons(if_cons_t* pif);
#endif

#ifdef XDK_SUPPORT_SOCK
	EXP_API void xdk_impl_socket(if_socket_t* pif);
#endif

#ifdef XDK_SUPPORT_DATE
	EXP_API void xdk_impl_date(if_date_t* pif);
#endif

#ifdef XDK_SUPPORT_PROCESS
	EXP_API void xdk_impl_process(if_process_t* pif);
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* _XDKINF_H */

