/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdk interface document

	@module	xdkinf.h | xdk interface file

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
typedef void* (*PF_HEAP_ALLOC)(res_heap_t, size_t);
typedef void* (*PF_HEAP_REALLOC)(res_heap_t, void*, size_t);
typedef void(*PF_HEAP_FREE)(res_heap_t, void*);
typedef void(*PF_HEAP_ZERO)(res_heap_t, void*, size_t);
#endif
#ifdef XDK_SUPPORT_MEMO_GLOB
typedef res_glob_t (*PF_GLOB_ALLOC)(size_t);
typedef res_glob_t(*PF_GLOB_REALLOC)(res_glob_t, size_t);
typedef void (*PF_GLOB_FREE)(res_glob_t);
typedef size_t(*PF_GLOB_SIZE)(res_glob_t);
typedef void* (*PF_GLOB_LOCK)(res_glob_t);
typedef bool_t (*PF_GLOB_UNLOCK)(res_glob_t);
#endif
#ifdef XDK_SUPPORT_MEMO_LOCAL
typedef void* (*PF_LOCAL_ALLOC)(size_t);
typedef void* (*PF_LOCAL_REALLOC)(void*, size_t);
typedef void(*PF_LOCAL_FREE)(void*);
#endif
#ifdef XDK_SUPPORT_MEMO_PAGE
typedef void* (*PF_PAGE_ALLOC)(size_t);
typedef void* (*PF_PAGE_REALLOC)(void*, size_t);
typedef void(*PF_PAGE_FREE)(void*);
typedef size_t(*PF_PAGE_SIZE)(void*);
typedef void* (*PF_PAGE_LOCK)(void*);
typedef void(*PF_PAGE_UNLOCK)(void*);
typedef bool_t(*PF_PAGE_PROTECT)(void*, bool_t);
#endif
#ifdef XDK_SUPPORT_MEMO_CACHE
typedef void*(*PF_CACHE_OPEN)(void);
typedef void(*PF_CACHE_CLOSE)(void*);
typedef bool_t(*PF_CACHE_WRITE)(void*, size_t, void*, size_t, size_t*);
typedef bool_t(*PF_CACHE_READ)(void*, size_t, void*, size_t, size_t*);
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
typedef void(*PF_ASYNC_ALLOC_LAPP)(async_t*, int ms);
typedef void(*PF_ASYNC_RELEASE_LAPP)(async_t*);

typedef struct _if_async_t{
	PF_ASYNC_ALLOC_LAPP		pf_async_alloc_lapp;
	PF_ASYNC_RELEASE_LAPP	pf_async_release_lapp;
}if_async_t;
#endif

#ifdef XDK_SUPPORT_THREAD
/*thread interface*/
typedef void (*PF_THREAD_BEGIN)(res_hand_t*, PF_THREADFUNC, void*);
typedef void (*PF_THREAD_END)(void);
typedef void (*PF_THREAD_CREATE_TLS)(tls_key_t*);
typedef void(*PF_THREAD_DESTROY_TLS)(tls_key_t);
typedef void* (*PF_THREAD_GET_TLS)(tls_key_t);
typedef void(*PF_THREAD_SET_TLS)(tls_key_t, void*);
typedef pid_t (* PF_THREAD_GET_ID)(void);
typedef void (* PF_THREAD_SLEEP)(int);
typedef void(*PF_THREAD_JOIN)(res_hand_t);
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
typedef wait_t(*PF_MUTEX_LOCK)(res_mutx_t, int);
typedef void		(*PF_MUTEX_UNLOCK)(res_mutx_t);
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
typedef res_sema_t(*PF_SEMAP_CREATE)(const tchar_t*, int max);
typedef void(*PF_SEMAP_DESTROY)(const tchar_t*, res_sema_t);
typedef res_sema_t(*PF_SEMAP_OPEN)(const tchar_t*);
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
	PF_THREAD_CREATE_TLS pf_thread_create_tls;
	PF_THREAD_DESTROY_TLS pf_thread_destroy_tls;
	PF_THREAD_GET_TLS	pf_thread_get_tls;
	PF_THREAD_SET_TLS	pf_thread_set_tls;
	PF_THREAD_GET_ID	pf_thread_get_id;
	PF_THREAD_JOIN		pf_thread_join;
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
	PF_MUTEX_LOCK		pf_mutex_lock;
	PF_MUTEX_UNLOCK		pf_mutex_unlock;
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
	PF_SEMAP_CREATE		pf_semap_create;
	PF_SEMAP_DESTROY	pf_semap_destroy;
	PF_SEMAP_OPEN		pf_semap_open;
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
typedef res_hand_t(*PF_CREATE_TIMER_QUEUE)(void);
typedef void(*PF_DESTROY_TIMER_QUEUE)(res_hand_t);
typedef res_timer_t(*PF_CREATE_TIMER)(res_hand_t, clock_t, clock_t, PF_TIMERFUNC, void*);
typedef void(*PF_DESTROY_TIMER)(res_hand_t, res_timer_t);
typedef bool_t(*PF_ALTER_TIMER)(res_hand_t, res_timer_t, clock_t, clock_t);

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
typedef res_file_t (*PF_SOCKET_TCP)(int, u32_t);
typedef res_file_t(*PF_SOCKET_UDP)(int, u32_t);
typedef res_file_t(*PF_SOCKET_ICMP)(int, u32_t);
typedef void (*PF_SOCKET_SHUTDOWN)(res_file_t, int);
typedef void (*PF_SOCKET_CLOSE)(res_file_t);
typedef u32_t(*PF_SOCKET_WAIT)(res_file_t, u32_t, int);
typedef bool_t (*PF_SOCKET_BIND)(res_file_t, res_addr_t, int);
typedef bool_t (*PF_SOCKET_CONNECT)(res_file_t, res_addr_t, int);
typedef bool_t(*PF_SOCKET_SENDTO)(res_file_t, res_addr_t, int, void*, size_t, async_t*);
typedef bool_t(*PF_SOCKET_RECVFROM)(res_file_t, res_addr_t, int*, void*, size_t, async_t*);
typedef bool_t(*PF_SOCKET_SEND)(res_file_t, void*, size_t, async_t*);
typedef bool_t(*PF_SOCKET_RECV)(res_file_t, void*, size_t, async_t*);
typedef bool_t (*PF_SOCKET_SETOPT)(res_file_t, int, const char*, int);
typedef bool_t (*PF_SOCKET_GETOPT)(res_file_t, int, char*, int*);
typedef bool_t (*PF_SOCKET_SET_LINGER)(res_file_t, bool_t, int);
typedef bool_t (*PF_SOCKET_SET_SNDBUF)(res_file_t, int);
typedef bool_t (*PF_SOCKET_SET_RCVBUF)(res_file_t, int);
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
typedef bool_t(*PF_SOCKET_SHARE)(pid_t, res_file_t, res_file_t, void*, size_t);
typedef res_file_t(*PF_SOCKET_DUPLI)(res_file_t, u32_t, void*, size_t*);

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
typedef res_file_t (*PF_FILE_OPEN)(const tchar_t*, u32_t);
typedef void (*PF_FILE_CLOSE)(res_file_t);
typedef bool_t(*PF_FILE_SIZE)(res_file_t, u32_t*, u32_t*);
typedef bool_t(*PF_FILE_WRITE)(res_file_t, void*, size_t, async_t*);
typedef bool_t(*PF_FILE_FLUSH)(res_file_t);
typedef bool_t(*PF_FILE_READ)(res_file_t, void*, size_t, async_t*);
typedef bool_t(*PF_FILE_READ_RANGE)(res_file_t, u32_t, u32_t, void*, size_t);
typedef bool_t(*PF_FILE_WRITE_RANGE)(res_file_t, u32_t, u32_t, void*, size_t);
typedef bool_t(*PF_FILE_TRUNCATE)(res_file_t, u32_t, u32_t);
typedef bool_t (*PF_FILE_DELETE)(const tchar_t*);
typedef bool_t(*PF_FILE_RENAME)(const tchar_t*, const tchar_t*);
typedef bool_t (*PF_FILE_INFO)(const tchar_t*, file_info_t*);
typedef bool_t (*PF_FILE_SETTIME)(res_file_t fh, const xdate_t* pdt);
typedef bool_t(*PF_FILE_GETTIME)(res_file_t fh, xdate_t* pdt);
typedef bool_t(*PF_DIRECTORY_OPEN)(const tchar_t*, u32_t);
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
typedef res_file_t(*PF_SHARE_SRV)(const tchar_t*, const tchar_t*, size_t);
typedef void(*PF_SHARE_CLOSE)(const tchar_t*, res_file_t);
typedef res_file_t(*PF_SHARE_CLI)(const tchar_t*, size_t);
typedef bool_t(*PF_SHARE_WRITE)(res_file_t, size_t, void*, size_t, size_t*);
typedef bool_t(*PF_SHARE_READ)(res_file_t, size_t, void*, size_t, size_t*);
typedef void*(*PF_SHARE_LOCK)(res_file_t, size_t, size_t);
typedef void(*PF_SHARE_UNLOCK)(res_file_t, size_t, size_t, void*);

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
typedef res_file_t (*PF_PIPE_SRV)(const tchar_t*, u32_t);
typedef bool_t(*PF_PIPE_LISTEN)(res_file_t, async_t*);
typedef void(*PF_PIPE_STOP)(res_file_t);
typedef res_file_t (*PF_PIPE_CLI)(const tchar_t*, u32_t);
typedef void(*PF_PIPE_CLOSE)(const tchar_t*, res_file_t);
typedef bool_t(*PF_PIPE_WRITE)(res_file_t, void*, size_t, async_t*);
typedef bool_t(*PF_PIPE_FLUSH)(res_file_t);
typedef bool_t(*PF_PIPE_READ)(res_file_t, void*, size_t, async_t*);
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
typedef res_file_t(*PF_COMM_OPEN)(const tchar_t*, u32_t);
typedef void(*PF_COMM_CLOSE)(res_file_t);
typedef u32_t(*PF_COMM_WAIT)(res_file_t, async_t*);
typedef bool_t(*PF_COMM_READ)(res_file_t, void*, size_t, async_t*);
typedef bool_t(*PF_COMM_WRITE)(res_file_t, void*, size_t, async_t*);
typedef bool_t(*PF_COMM_FLUSH)(res_file_t);

typedef struct _if_comm_t{
	PF_DEFAULT_COMM_MODE	pf_default_comm_mode;
	PF_GET_COMM_MODE		pf_get_comm_mode;
	PF_SET_COMM_MODE		pf_set_comm_mode;
	PF_COMM_WAIT		pf_comm_wait;
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
typedef bool_t(*PF_CONS_WRITE)(res_file_t, void*, size_t, size_t*);
typedef bool_t(*PF_CONS_FLUSH)(res_file_t);
typedef bool_t(*PF_CONS_READ)(res_file_t, void*, size_t, size_t*);
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
typedef bool_t(*PF_MAK_LOC_DATE)(xdate_t*);
typedef void (*PF_GET_UTC_DATE)(xdate_t*);
typedef bool_t(*PF_MAK_UTC_DATE)(xdate_t*);
typedef clock_t(*PF_GET_TIMES)(void);
typedef clock_t(*PF_GET_TICKS)(void);
typedef stamp_t(*PF_GET_TIMESTAMP)(void);
typedef void(*PF_UTC_DATE_FROM_TIMES)(xdate_t*, clock_t);
typedef void(*PF_UTC_DATE_FROM_TICKS)(xdate_t*, clock_t);
typedef void(*PF_UTC_DATE_FROM_TIMESTAMP)(xdate_t*, stamp_t);

typedef struct _if_date_t{
	PF_GET_LOC_DATE		pf_get_loc_date;
	PF_MAK_LOC_DATE		pf_mak_loc_date;
	PF_GET_UTC_DATE		pf_get_utc_date;
	PF_MAK_UTC_DATE		pf_mak_utc_date;
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
typedef void(*PF_PROCESS_WAITRUN)(res_hand_t);
typedef res_hand_t (*PF_PROCESS_DUPLI)(res_hand_t, res_hand_t);
typedef void* (*PF_PROCESS_ALLOC)(res_hand_t, size_t);
typedef void (*PF_PROCESS_FREE)(res_hand_t, void*);
typedef bool_t(*PF_PROCESS_WRITE)(res_hand_t, void*, void*, size_t);
typedef bool_t(*PF_PROCESS_READ)(res_hand_t, void*, void*, size_t);
typedef void(*PF_RELEASE_HANDLE)(res_hand_t);
typedef bool_t(*PF_INHERIT_HANDLE)(res_hand_t, bool_t);
typedef void(*PF_WRITE_PROFILE)(const tchar_t*, const tchar_t*, const tchar_t*, const tchar_t*);
typedef void(*PF_READ_PROFILE)(const tchar_t*, const tchar_t*, const tchar_t*, tchar_t*, int);
typedef void(*PF_SYSTEM_INFO)(sys_info_t*);

typedef struct _if_process_t{
	PF_LOAD_LIBRARY		pf_load_library;
	PF_FREE_LIBRARY		pf_free_library;
	PF_GET_ADDRESS		pf_get_address;
	PF_GET_RUNPATH		pf_get_runpath;
	PF_CREATE_PROCESS	pf_create_process;
	PF_RELEASE_PROCESS	pf_release_process;
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
	PF_SYSTEM_INFO		pf_system_info;
}if_process_t;
#endif /*XDK_SUPPORT_PROCESS*/

#ifdef XDK_SUPPORT_SHELL
/*shell interface*/
typedef bool_t(*PF_SHELL_GET_RUNPATH)(tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_CURPATH)(tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_APPPATH)(tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_DOCPATH)(tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_TMPPATH)(tchar_t*, int);
#ifdef XDK_SUPPORT_SHELL_DIALOG
typedef bool_t(*PF_SHELL_GET_FILENAME)(res_win_t, const tchar_t*, const tchar_t*, const tchar_t*, bool_t, tchar_t*, int, tchar_t*, int);
typedef bool_t(*PF_SHELL_GET_PATHNAME)(res_win_t, const tchar_t*, bool_t, tchar_t*, int);
#endif

typedef struct _if_shell_t {
	PF_SHELL_GET_RUNPATH	pf_shell_get_runpath;
	PF_SHELL_GET_CURPATH	pf_shell_get_curpath;
	PF_SHELL_GET_APPPATH	pf_shell_get_apppath;
	PF_SHELL_GET_DOCPATH	pf_shell_get_docpath;
	PF_SHELL_GET_TMPPATH	pf_shell_get_tmppath;
#ifdef XDK_SUPPORT_SHELL_DIALOG
	PF_SHELL_GET_FILENAME	pf_shell_get_filename;
	PF_SHELL_GET_PATHNAME	pf_shell_get_pathname;
#endif
}if_shell_t;
#endif /*XDK_SUPPORT_SHELL*/

#ifdef XDK_SUPPORT_CONTEXT
/*context interface*/
typedef int(*PF_CONTEXT_STARTUP)(void);
typedef void(*PF_CONTEXT_CLEANUP)(void);
typedef res_ctx_t(*PF_CREATE_DISPLAY_CONTEXT)(void);
typedef res_ctx_t(*PF_CREATE_COMPATIBLE_CONTEXT)(res_ctx_t);
typedef void(*PF_DESTROY_CONTEXT)(res_ctx_t);
typedef void(*PF_GET_DEVICE_CAPS)(res_ctx_t, dev_cap_t*);
typedef void(*PF_RENDER_CONTEXT)(res_ctx_t, long, long, res_ctx_t, long, long, long, long);

typedef float(*PF_PT_PER_MM)(res_ctx_t, bool_t);
typedef void(*PF_TEXT_MM_SIZE)(res_ctx_t, const xfont_t*, const tchar_t*, int, float*, float*);
typedef void(*PF_TEXT_PT_SIZE)(res_ctx_t, const xfont_t*, const tchar_t*, int, long*, long*);
typedef void(*PF_TEXT_MM_METRIC)(res_ctx_t, const xfont_t*, float*, float*);
typedef void(*PF_TEXT_PT_METRIC)(res_ctx_t, const xfont_t*, long*, long*);
typedef float(*PF_CAST_PT_TO_MM)(res_ctx_t, long, bool_t);
typedef long(*PF_CAST_MM_TO_PT)(res_ctx_t, float, bool_t);
typedef int(*PF_FONT_SIZE)(res_ctx_t, long);

#ifdef XDK_SUPPORT_CONTEXT_REGION
typedef res_rgn_t(*PF_CREATE_REGION)(const tchar_t*, const xrect_t*);
typedef void(*PF_DELETE_REGION)(res_rgn_t);
typedef bool_t(*PF_PT_IN_REGION)(res_rgn_t, const xpoint_t*);
#endif

#ifdef XDK_SUPPORT_CONTEXT_BITMAP
/*bitmap interface*/
typedef res_bmp_t(*PF_SELECT_COMPATIBLE_BITMAP)(res_ctx_t, res_bmp_t);
typedef res_bmp_t(*PF_CREATE_COMPATIBLE_BITMAP)(res_ctx_t, long, long);
typedef void(*PF_DESTROY_BITMAP)(res_bmp_t);
typedef void(*PF_GET_BITMAP_SIZE)(res_bmp_t, long*, long*);

typedef res_bmp_t(*PF_CREATE_COLOR_BITMAP)(res_ctx_t, const xcolor_t*, long, long);
typedef res_bmp_t(*PF_CREATE_PATTERN_BITMAP)(res_ctx_t, const xcolor_t*, const xcolor_t*, long, long, const tchar_t*);
typedef res_bmp_t(*PF_CREATE_GRADIENT_BITMAP)(res_ctx_t, const xcolor_t*, const xcolor_t*, long, long, const tchar_t*);
typedef res_bmp_t(*PF_CREATE_CODE128_BITMAP)(res_ctx_t, long, long, const unsigned char*, size_t, const tchar_t*);
typedef res_bmp_t(*PF_CREATE_PDF417_BITMAP)(res_ctx_t, long, long, const unsigned char*, size_t, int, int);
typedef res_bmp_t(*PF_CREATE_QRCODE_BITMAP)(res_ctx_t, long, long, const unsigned char*, size_t, int, int);
typedef res_bmp_t(*PF_CREATE_STORAGE_BITMAP)(res_ctx_t, const tchar_t*);
typedef res_bmp_t(*PF_LOAD_BITMAP_FROM_BYTES)(res_ctx_t, const unsigned char*, size_t);
typedef size_t(*PF_SAVE_BITMAP_TO_BYTES)(res_ctx_t, res_bmp_t, unsigned char*, size_t);
typedef size_t(*PF_GET_BITMAP_BYTES)(res_bmp_t);
typedef res_bmp_t(*PF_LOAD_BITMAP_FROM_THUMB)(res_ctx_t, const tchar_t*);
typedef res_bmp_t(*PF_LOAD_BITMAP_FROM_ICON)(res_ctx_t, const tchar_t*);
#endif

#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC
/*graphic interface*/
typedef void(*PF_GDI_DRAW_LINE)(res_ctx_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_GDI_DRAW_3DLINE)(res_ctx_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_GDI_DRAW_3DRECT)(res_ctx_t, const xpen_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_POLYLINE)(res_ctx_t, const xpen_t*, const xpoint_t*, int);
typedef void(*PF_GDI_DRAW_POLYGON)(res_ctx_t, const xpen_t*, const xbrush_t*, const xpoint_t*, int);
typedef void(*PF_GDI_DRAW_BEZIER)(res_ctx_t, const xpen_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_GDI_DRAW_PATH)(res_ctx_t, const xpen_t*, const tchar_t*, int);
typedef void(*PF_GDI_DRAW_RECT)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_GDI_GRADIENT_RECT)(res_ctx_t, const xgradi_t*, const xrect_t*);
typedef void(*PF_GDI_ALPHABLEND_RECT)(res_ctx_t, const xcolor_t*, const xrect_t*, int);
typedef void(*PF_GDI_DRAW_ROUND)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_ELLIPSE)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_PIE)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*, double, double);
typedef void(*PF_GDI_DRAW_ARC)(res_ctx_t, const xpen_t*, const xrect_t*, double, double);
typedef void(*PF_GDI_DRAW_ARROW)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*, int, double);
typedef void(*PF_GDI_DRAW_TEXT)(res_ctx_t, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int);
typedef void(*PF_GDI_TEXT_OUT)(res_ctx_t, const xfont_t*, const xpoint_t*, const tchar_t*, int);
typedef void(*PF_GDI_DRAW_IMAGE)(res_ctx_t, res_bmp_t, const tchar_t*, const xrect_t*);
typedef void(*PF_GDI_DRAW_BITMAP)(res_ctx_t, res_bmp_t, const xrect_t*);
typedef void(*PF_GDI_DRAW_ICON)(res_ctx_t, const xcolor_t*, const xrect_t*, const tchar_t*);
typedef void(*PF_GDI_FILL_REGION)(res_ctx_t, const xbrush_t*, res_rgn_t);

typedef void(*PF_GDI_EXCLIP_RECT)(res_ctx_t, const xrect_t*);
typedef void(*PF_GDI_INCLIP_RECT)(res_ctx_t, const xrect_t*);

typedef void(*PF_GDI_TEXT_SIZE)(res_ctx_t, const xfont_t*, const tchar_t*, int, xsize_t*);
typedef void(*PF_GDI_TEXT_METRIC)(res_ctx_t, const xfont_t*, xsize_t*);
#endif

#ifdef XDK_SUPPORT_CONTEXT_PRINTER
/*printer interface*/
typedef bool_t(*PF_DEFAULT_PRINTER_MODE)(dev_prn_t*);
typedef bool_t(*PF_SETUP_PRINTER_MODE)(res_win_t, dev_prn_t*);
typedef res_ctx_t(*PF_CREATE_PRINTER_CONTEXT)(const dev_prn_t*);
typedef void(*PF_DESTROY_PRINTER_CONTEXT)(res_ctx_t);
typedef void(*PF_BEGIN_PAGE)(res_ctx_t);
typedef void(*PF_END_PAGE)(res_ctx_t);
typedef void(*PF_BEGIN_DOC)(res_ctx_t, const tchar_t*);
typedef void(*PF_END_DOC)(res_ctx_t);
#endif

typedef struct _if_context_t{
	PF_CONTEXT_STARTUP			pf_context_startup;
	PF_CONTEXT_CLEANUP			pf_context_cleanup;

	PF_CREATE_COMPATIBLE_CONTEXT	pf_create_compatible_context;
	PF_CREATE_DISPLAY_CONTEXT	pf_create_display_context;
	PF_DESTROY_CONTEXT			pf_destroy_context;
	PF_GET_DEVICE_CAPS			pf_get_device_caps;
	PF_RENDER_CONTEXT			pf_render_context;

	PF_TEXT_MM_SIZE		pf_text_mm_size;
	PF_TEXT_PT_SIZE		pf_text_pt_size;
	PF_TEXT_MM_METRIC	pf_text_mm_metric;
	PF_TEXT_PT_METRIC	pf_text_pt_metric;
	PF_CAST_PT_TO_MM	pf_cast_pt_to_mm;
	PF_CAST_MM_TO_PT	pf_cast_mm_to_pt;
	PF_PT_PER_MM		pf_pt_per_mm;
	PF_FONT_SIZE		pf_font_size;

#ifdef XDK_SUPPORT_CONTEXT_REGION
	PF_CREATE_REGION			pf_create_region;
	PF_DELETE_REGION			pf_delete_region;
	PF_PT_IN_REGION				pf_pt_in_region;
#endif

#ifdef XDK_SUPPORT_CONTEXT_BITMAP
	PF_SELECT_COMPATIBLE_BITMAP	pf_select_compatible_bitmap;
	PF_CREATE_COMPATIBLE_BITMAP	pf_create_compatible_bitmap;
	PF_DESTROY_BITMAP			pf_destroy_bitmap;
	PF_GET_BITMAP_SIZE			pf_get_bitmap_size;

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

#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC
	PF_GDI_DRAW_LINE		pf_gdi_draw_line;
	PF_GDI_DRAW_3DLINE		pf_gdi_draw_3dline;
	PF_GDI_DRAW_POLYLINE	pf_gdi_draw_polyline;
	PF_GDI_DRAW_POLYGON		pf_gdi_draw_polygon;
	PF_GDI_DRAW_BEZIER		pf_gdi_draw_bezier;
	PF_GDI_DRAW_PATH		pf_gdi_draw_path;
	PF_GDI_DRAW_3DRECT		pf_gdi_draw_3drect;
	PF_GDI_DRAW_RECT		pf_gdi_draw_rect;
	PF_GDI_GRADIENT_RECT	pf_gdi_gradient_rect;
	PF_GDI_ALPHABLEND_RECT	pf_gdi_alphablend_rect;
	PF_GDI_DRAW_ROUND		pf_gdi_draw_round;
	PF_GDI_DRAW_ELLIPSE		pf_gdi_draw_ellipse;
	PF_GDI_DRAW_PIE			pf_gdi_draw_pie;
	PF_GDI_DRAW_ARC			pf_gdi_draw_arc;
	PF_GDI_DRAW_ARROW		pf_gdi_draw_arrow;
	PF_GDI_DRAW_TEXT		pf_gdi_draw_text;
	PF_GDI_TEXT_OUT			pf_gdi_text_out;
	PF_GDI_FILL_REGION		pf_gdi_fill_region;
	PF_GDI_DRAW_IMAGE		pf_gdi_draw_image;
	PF_GDI_DRAW_BITMAP		pf_gdi_draw_bitmap;
	PF_GDI_DRAW_ICON		pf_gdi_draw_icon;
	PF_GDI_TEXT_SIZE		pf_gdi_text_size;
	PF_GDI_TEXT_METRIC		pf_gdi_text_metric;
	PF_GDI_EXCLIP_RECT		pf_gdi_exclip_rect;
	PF_GDI_INCLIP_RECT		pf_gdi_inclip_rect;
#endif

#ifdef XDK_SUPPORT_CONTEXT_PRINTER
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
#endif /*XDK_SUPPORT_CONTEXT*/


#ifdef XDK_SUPPORT_CLIPBOARD
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
#endif /*XDK_SUPPORT_CLIPBOARD*/

#ifdef XDK_SUPPORT_WIDGET
/*widget interface*/
typedef void(*PF_WIDGET_STARTUP)(int);
typedef void(*PF_WIDGET_CLEANUP)(void);
typedef res_win_t(*PF_WIDGET_CREATE)(const tchar_t* wname, u32_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev);
typedef void(*PF_WIDGET_DESTROY)(res_win_t);
typedef void(*PF_WIDGET_CLOSE)(res_win_t, int);
typedef if_event_t* (*PF_WIDGET_GET_DISPATCH)(res_win_t);
typedef void(*PF_WIDGET_SET_STYLE)(res_win_t, u32_t);
typedef u32_t(*PF_WIDGET_GET_STYLE)(res_win_t);
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
typedef res_ctx_t(*PF_WIDGET_CLIENT_DC)(res_win_t);
typedef res_ctx_t(*PF_WIDGET_WINDOW_DC)(res_win_t);
typedef void(*PF_WIDGET_RELEASE_DC)(res_win_t, res_ctx_t);
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
typedef void(*PF_WIDGET_SET_IMM)(res_win_t, bool_t);
typedef bool_t(*PF_WIDGET_GET_IMM)(res_win_t);
typedef void(*PF_WIDGET_SET_FOCUS)(res_win_t);
typedef bool_t(*PF_WIDGET_KEY_STATE)(res_win_t, int);
typedef bool_t(*PF_WIDGET_IS_VALID)(res_win_t);
typedef bool_t(*PF_WIDGET_IS_CHILD)(res_win_t);
typedef bool_t(*PF_WIDGET_IS_FOCUS)(res_win_t);
typedef bool_t(*PF_WIDGET_IS_OWNC)(res_win_t);
typedef void(*PF_WIDGET_POST_CHAR)(res_win_t, tchar_t);
typedef void(*PF_WIDGET_POST_KEY)(res_win_t, int);
typedef void(*PF_WIDGET_POST_QUIT)(res_win_t);
typedef void(*PF_WIDGET_SIZE)(res_win_t, const xsize_t*);
typedef void(*PF_WIDGET_MOVE)(res_win_t, const xpoint_t*);
typedef void(*PF_WIDGET_TAKE)(res_win_t, int);
typedef void(*PF_WIDGET_SHOW)(res_win_t, u32_t);
typedef void(*PF_WIDGET_UPDATE_CLIENT)(res_win_t);
typedef void(*PF_WIDGET_UPDATE_WINDOW)(res_win_t);
typedef void(*PF_WIDGET_PAINT)(res_win_t);
typedef void(*PF_WIDGET_UPDATE)(res_win_t, const xrect_t*, bool_t);
typedef void(*PF_WIDGET_ENABLE)(res_win_t, bool_t);
typedef void(*PF_WIDGET_CREATE_CARET)(res_win_t, int, int);
typedef void(*PF_WIDGET_DESTROY_CARET)(res_win_t);
typedef void(*PF_WIDGET_SHOW_CARET)(res_win_t, int, int, bool_t);

typedef void(*PF_WIDGET_SET_TITLE)(res_win_t, const tchar_t*);
typedef int(*PF_WIDGET_GET_TITLE)(res_win_t, tchar_t*, int);
typedef void(*PF_WIDGET_CALC_BORDER)(u32_t, border_t*);
typedef void(*PF_WIDGET_ADJUST_SIZE)(u32_t, xsize_t*);
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

typedef void(*PF_WIDGET_COPY)(res_win_t);
typedef void(*PF_WIDGET_CUT)(res_win_t);
typedef void(*PF_WIDGET_PASTE)(res_win_t);
typedef void(*PF_WIDGET_UNDO)(res_win_t);

typedef void(*PF_WIDGET_POST_MESSAGE)(res_win_t, int, var_long, var_long);
typedef int(*PF_WIDGET_SEND_MESSAGE)(res_win_t, int, var_long, var_long);
typedef void(*PF_WIDGET_POST_COMMAND)(res_win_t, int, uid_t, var_long);
typedef int(*PF_WIDGET_SEND_COMMAND)(res_win_t, int, uid_t, var_long);

typedef var_long(*PF_WIDGET_SET_TIMER)(res_win_t, int);
typedef void(*PF_WIDGET_KILL_TIMER)(res_win_t, var_long);

typedef void(*PF_WIDGET_GET_SCROLLINFO)(res_win_t, bool_t, scroll_t*);
typedef void(*PF_WIDGET_SET_SCROLLINFO)(res_win_t, bool_t, const scroll_t*);

typedef int(*PF_WIDGET_DO_MODAL)(res_win_t);
typedef void(*PF_WIDGET_DO_TRACE)(res_win_t);

typedef void(*PF_GET_SCREEN_SIZE)(xsize_t*);
typedef void(*PF_GET_DESKTOP_SIZE)(xsize_t*);
typedef void(*PF_SCREEN_SIZE_TO_PT)(xsize_t*);
typedef void(*PF_SCREEN_SIZE_TO_TM)(xsize_t*);
typedef res_acl_t(*PF_CREATE_ACCEL_TABLE)(const accel_t*, int);
typedef void(*PF_DESTROY_ACCEL_TABLE)(res_acl_t);

#ifdef XDK_SUPPORT_WIDGET_EX
typedef void(*PF_WIDGET_TRACK_MOUSE)(res_win_t, u32_t);
typedef void(*PF_WIDGET_SET_REGION)(res_win_t, res_rgn_t);
typedef void(*PF_WIDGET_SET_ALPHA)(res_win_t, unsigned char);
typedef unsigned char(*PF_WIDGET_GET_ALPHA)(res_win_t);
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
	PF_WIDGET_SET_IMM			pf_widget_set_imm;
	PF_WIDGET_GET_IMM			pf_widget_get_imm;
	PF_WIDGET_SET_FOCUS			pf_widget_set_focus;
	PF_WIDGET_KEY_STATE			pf_widget_key_state;
	PF_WIDGET_IS_VALID			pf_widget_is_valid;
	PF_WIDGET_IS_CHILD			pf_widget_is_child;
	PF_WIDGET_IS_FOCUS			pf_widget_is_focus;
	PF_WIDGET_IS_OWNC			pf_widget_is_ownc;
	PF_WIDGET_POST_CHAR			pf_widget_post_char;
	PF_WIDGET_POST_KEY			pf_widget_post_key;
	PF_WIDGET_POST_QUIT			pf_widget_post_quit;
	PF_WIDGET_SIZE				pf_widget_size;
	PF_WIDGET_MOVE				pf_widget_move;
	PF_WIDGET_TAKE				pf_widget_take;
	PF_WIDGET_SHOW				pf_widget_show;
	PF_WIDGET_UPDATE_CLIENT		pf_widget_update_client;
	PF_WIDGET_UPDATE_WINDOW		pf_widget_update_window;
	PF_WIDGET_PAINT				pf_widget_paint;
	PF_WIDGET_UPDATE			pf_widget_update;
	PF_WIDGET_ENABLE			pf_widget_enable;
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

	PF_WIDGET_COPY				pf_widget_copy;
	PF_WIDGET_CUT				pf_widget_cut;
	PF_WIDGET_PASTE				pf_widget_paste;
	PF_WIDGET_UNDO				pf_widget_undo;

	PF_WIDGET_POST_MESSAGE		pf_widget_post_message;
	PF_WIDGET_SEND_MESSAGE		pf_widget_send_message;
	PF_WIDGET_POST_COMMAND		pf_widget_post_command;
	PF_WIDGET_SEND_COMMAND		pf_widget_send_command;

	PF_WIDGET_GET_SCROLLINFO	pf_widget_get_scroll_info;
	PF_WIDGET_SET_SCROLLINFO	pf_widget_set_scroll_info;

	PF_WIDGET_DO_MODAL			pf_widget_do_modal;
	PF_WIDGET_DO_TRACE			pf_widget_do_trace;

	PF_GET_SCREEN_SIZE			pf_get_screen_size;
	PF_GET_DESKTOP_SIZE			pf_get_desktop_size;
	PF_SCREEN_SIZE_TO_PT		pf_screen_size_to_pt;
	PF_SCREEN_SIZE_TO_TM		pf_screen_size_to_tm;
	PF_CREATE_ACCEL_TABLE		pf_create_accel_table;
	PF_DESTROY_ACCEL_TABLE		pf_destroy_accel_table;
#ifdef XDK_SUPPORT_WIDGET_EX
	PF_WIDGET_TRACK_MOUSE		pf_widget_track_mouse;
	PF_WIDGET_SET_ALPHA			pf_widget_set_alpha;
	PF_WIDGET_GET_ALPHA			pf_widget_get_alpha;
	PF_WIDGET_SET_REGION		pf_widget_set_region;
#endif
}if_widget_t;
#endif /*XDK_SUPPORT_WIDGET*/

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef XDK_SUPPORT_MEMO
#ifdef XDK_SUPPORT_MEMO_HEAP
	XDK_API void xdk_impl_memo_heap(if_memo_t* pif);
#endif
#ifdef XDK_SUPPORT_MEMO_GLOB
	XDK_API void xdk_impl_memo_glob(if_memo_t* pif);
#endif
#ifdef XDK_SUPPORT_MEMO_GLOB
	XDK_API void xdk_impl_memo_local(if_memo_t* pif);
#endif
#ifdef XDK_SUPPORT_MEMO_PAGE
	XDK_API void xdk_impl_memo_page(if_memo_t* pif);
#endif
#ifdef XDK_SUPPORT_MEMO_CACHE
	XDK_API void xdk_impl_memo_cache(if_memo_t* pif);
#endif
#endif /*XDK_SUPPORT_MEMO*/

#ifdef XDK_SUPPORT_ERROR
	XDK_API void xdk_impl_error(if_error_t* pif);
#endif

#ifdef XDK_SUPPORT_MBCS
	XDK_API void xdk_impl_mbcs(if_mbcs_t* pif);
#endif

#ifdef XDK_SUPPORT_ASYNC
	XDK_API void xdk_impl_async(if_async_t* pif);
#endif

#ifdef XDK_SUPPORT_THREAD
	XDK_API void xdk_impl_thread(if_thread_t* pif);
#ifdef XDK_SUPPORT_THREAD_EVENT
	XDK_API void xdk_impl_thread_event(if_thread_t* pif);
#endif
#ifdef XDK_SUPPORT_THREAD_CRITI
	XDK_API void xdk_impl_thread_criti(if_thread_t* pif);
#endif
#ifdef XDK_SUPPORT_THREAD_MUTEX
	XDK_API void xdk_impl_thread_mutex(if_thread_t* pif);
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
	XDK_API void xdk_impl_thread_semap(if_thread_t* pif);
#endif
#ifdef XDK_SUPPORT_THREAD_QUEUE
	XDK_API void xdk_impl_thread_queue(if_thread_t* pif);
#endif
#endif /*XDK_SUPPORT_THREAD*/

#ifdef XDK_SUPPORT_TIMER
	XDK_API void xdk_impl_timer(if_timer_t* pif);
#endif

#ifdef XDK_SUPPORT_FILE
	XDK_API void xdk_impl_file(if_file_t* pif);
#ifdef XDK_SUPPORT_FILE_FIND
	XDK_API void xdk_impl_file_find(if_file_t* pif);
#endif
#endif /*XDK_SUPPORT_FILE*/

#ifdef XDK_SUPPORT_SHARE
	XDK_API void xdk_impl_share(if_share_t* pif);
#endif

#ifdef XDK_SUPPORT_PIPE
	XDK_API void xdk_impl_pipe(if_pipe_t* pif);
#endif

#ifdef XDK_SUPPORT_COMM
	XDK_API void xdk_impl_comm(if_comm_t* pif);
#endif

#ifdef XDK_SUPPORT_CONS
	XDK_API void xdk_impl_cons(if_cons_t* pif);
#endif

#ifdef XDK_SUPPORT_SOCK
	XDK_API void xdk_impl_socket(if_socket_t* pif);
#endif

#ifdef XDK_SUPPORT_DATE
	XDK_API void xdk_impl_date(if_date_t* pif);
#endif

#ifdef XDK_SUPPORT_PROCESS
	XDK_API void xdk_impl_process(if_process_t* pif);
#endif

#ifdef XDK_SUPPORT_SHELL
	XDK_API void xdk_impl_shell(if_shell_t* pif);
#ifdef XDK_SUPPORT_SHELL_DIALOG
	XDK_API void xdk_impl_shell_dialog(if_shell_t* pif);
#endif
#endif /*XDK_SUPPORT_SHELL*/

#ifdef XDK_SUPPORT_CONTEXT
	XDK_API void xdk_impl_context(if_context_t* pif);
#ifdef XDK_SUPPORT_CONTEXT_REGION
	XDK_API void xdk_impl_context_region(if_context_t* pif);
#endif
#ifdef XDK_SUPPORT_CONTEXT_BITMAP
	XDK_API void xdk_impl_context_bitmap(if_context_t* pif);
#endif
#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC
	XDK_API void xdk_impl_context_graphic(if_context_t* pif);
#endif
#ifdef XDK_SUPPORT_CONTEXT_PRINTER
	XDK_API void xdk_impl_context_printer(if_context_t* pif);
#endif
#endif /*XDK_SUPPORT_CONTEXT*/

#ifdef XDK_SUPPORT_CLIPBOARD
	XDK_API void xdk_impl_clipboard(if_clipboard_t* pif);
#endif

#ifdef XDK_SUPPORT_WIDGET
	XDK_API void xdk_impl_widget(if_widget_t* pif);
#ifdef XDK_SUPPORT_WIDGET_EX
	XDK_API void xdk_impl_widget_ex(if_widget_t* pif);
#endif
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _XDKINF_H */

