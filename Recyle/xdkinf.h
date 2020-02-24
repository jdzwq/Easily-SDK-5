/***********************************************************************
	Easily xdk v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdk interface document

	@module	xdkinf.h | xdk interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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

/*base64 encode/decode*/
typedef int(__cdecl *PF_BASE64_DECODE)(const tchar_t*, int, byte_t*, int);
typedef int(__cdecl *PF_BASE64_ENCODE)(const byte_t*, int, tchar_t*, int);

typedef struct _if_base64_t{
	PF_BASE64_DECODE	pf_base64_decode;
	PF_BASE64_ENCODE	pf_base64_encode;
}if_base64_t;

/*bar encode*/
typedef int(__cdecl *PF_PDF417_ENCODE)(const byte_t*, int, byte_t*, int, int*, int*);
typedef int(__cdecl *PF_CODE128_ENCODE)(const byte_t*, int, byte_t*, int);
typedef int(__cdecl *PF_QR_ENCODE)(const byte_t*, int, byte_t*, int, int*, int*);

typedef struct _if_barcode_t{
	PF_PDF417_ENCODE	pf_pdf417_encode;
	PF_CODE128_ENCODE	pf_code128_encode;
	PF_QR_ENCODE		pf_qr_encode;
}if_barcode_t;

/*jpg compress/decompress*/
typedef	int(__cdecl *PF_JPG_DECOMPRESS)(const byte_t*, int, byte_t*, int);
typedef	int(__cdecl *PF_JPG_COMPRESS)(const byte_t*, int, byte_t*, int);

typedef struct _if_jpg_t{
	PF_JPG_DECOMPRESS	pf_jpg_decompress;
	PF_JPG_COMPRESS		pf_jpg_compress;
}if_jpg_t;

/*png compress/decompress*/
typedef	int(__cdecl *PF_PNG_DECOMPRESS)(const byte_t*, int, byte_t*, int);
typedef	int(__cdecl *PF_PNG_COMPRESS)(const byte_t*, int, byte_t*, int);

typedef struct _if_png_t{
	PF_PNG_DECOMPRESS	pf_png_decompress;
	PF_PNG_COMPRESS		pf_png_compress;
}if_png_t;

/*encrypt*/
typedef	int(__cdecl *PF_MD5_BYTES)(const byte_t*, int, byte_t*);
typedef	int(__cdecl *PF_MD5_TOKEN)(const byte_t*, tchar_t*);
typedef	int(__cdecl *PF_SHA1_HMAC_BYTES)(const byte_t*, int, const byte_t*, int, byte_t*);
typedef int(__cdecl *PF_SHA2_HMAC_BYTES)(const byte_t*, int, const byte_t*, int, bool_t, byte_t*);
typedef int(__cdecl *PF_SHA4_HMAC_BYTES)(const byte_t*, int, const byte_t*, int, bool_t, byte_t*);

typedef struct _if_encrypt_t{
	PF_MD5_BYTES		pf_md5_bytes;
	PF_MD5_TOKEN		pf_md5_token;
	PF_SHA1_HMAC_BYTES	pf_sha1_hmac_bytes;
	PF_SHA2_HMAC_BYTES	pf_sha2_hmac_bytes;
	PF_SHA4_HMAC_BYTES	pf_sha4_hmac_bytes;
}if_encrypt_t;

/*zip/gzip compress/uncompress*/
typedef	bool_t(__cdecl *PF_ZLIB_COMPRESS_BYTES)(const byte_t*, unsigned long, byte_t*, unsigned long*);
typedef	bool_t(__cdecl *PF_ZLIB_UNCOMPRESS_BYTES)(const byte_t*, unsigned long, byte_t*, unsigned long*);
typedef	bool_t(__cdecl *PF_GZIP_COMPRESS_BYTES)(const byte_t*, unsigned long, byte_t*, unsigned long*);
typedef	bool_t(__cdecl *PF_GZIP_UNCOMPRESS_BYTES)(const byte_t*, unsigned long, byte_t*, unsigned long*);

typedef struct _if_zip_t{
	PF_ZLIB_COMPRESS_BYTES		pf_zlib_compress_bytes;
	PF_ZLIB_UNCOMPRESS_BYTES	pf_zlib_uncompress_bytes;
	PF_GZIP_COMPRESS_BYTES		pf_gzip_compress_bytes;
	PF_GZIP_UNCOMPRESS_BYTES	pf_gzip_uncompress_bytes;
}if_zip_t;

/*ssl*/
typedef ssl_sock_t(__cdecl *PF_SSL_ALLOC)(res_sock_t, pf_socket_write, pf_socket_read, bool_t);
typedef void(__cdecl *PF_SSL_FREE)(ssl_sock_t);
typedef int(__cdecl *PF_SSL_READ)(ssl_sock_t, byte_t*, dword_t);
typedef int(__cdecl *PF_SSL_WRITE)(ssl_sock_t, const byte_t*, dword_t);

typedef struct _if_ssl_t{
	PF_SSL_ALLOC	pf_ssl_alloc;
	PF_SSL_FREE		pf_ssl_free;
	PF_SSL_READ		pf_ssl_read;
	PF_SSL_WRITE	pf_ssl_write;
}if_ssl_t;

/*memory interface*/
typedef res_heap_t(__cdecl *PF_PROCESS_HEAP)(void);
typedef res_heap_t(__cdecl *PF_HEAP_CREATE)(void);
typedef void(__cdecl *PF_HEAP_DESTROY)(res_heap_t);
typedef void(__cdecl *PF_HEAP_CLEAN)(res_heap_t);
typedef void* (__cdecl *PF_HEAP_ALLOC)(res_heap_t, dword_t);
typedef void* (__cdecl *PF_HEAP_REALLOC)(res_heap_t, void*, dword_t);
typedef void(__cdecl *PF_HEAP_FREE)(res_heap_t, void*);
typedef void(__cdecl *PF_HEAP_ZERO)(res_heap_t, void*, dword_t);

typedef res_glob_t (__cdecl *PF_GLOB_ALLOC)(dword_t);
typedef res_glob_t (__cdecl *PF_GLOB_REALLOC)(res_glob_t, dword_t);
typedef void (__cdecl *PF_GLOB_FREE)(res_glob_t);
typedef dword_t (__cdecl *PF_GLOB_SIZE)(res_glob_t);
typedef void* (__cdecl *PF_GLOB_LOCK)(res_glob_t);
typedef bool_t (__cdecl *PF_GLOB_UNLOCK)(res_glob_t);

typedef void* (__cdecl *PF_PAGE_ALLOC)(dword_t);
typedef void* (__cdecl *PF_PAGE_REALLOC)(void*, dword_t);
typedef void(__cdecl *PF_PAGE_FREE)(void*);
typedef dword_t(__cdecl *PF_PAGE_SIZE)(void*);
typedef void* (__cdecl *PF_PAGE_LOCK)(void*);
typedef void(__cdecl *PF_PAGE_UNLOCK)(void*);
typedef bool_t(__cdecl *PF_PAGE_PROTECT)(void*);

typedef struct _if_memo_t{
	PF_PROCESS_HEAP		pf_process_heap;
	PF_HEAP_CREATE		pf_heap_create;
	PF_HEAP_DESTROY		pf_heap_destroy;
	PF_HEAP_CLEAN		pf_heap_clean;
	PF_HEAP_ALLOC		pf_heap_alloc;
	PF_HEAP_REALLOC		pf_heap_realloc;
	PF_HEAP_FREE		pf_heap_free;
	PF_HEAP_ZERO		pf_heap_zero;

	PF_GLOB_ALLOC		pf_glob_alloc;
	PF_GLOB_REALLOC		pf_glob_realloc;
	PF_GLOB_SIZE		pf_glob_size;
	PF_GLOB_FREE		pf_glob_free;
	PF_GLOB_LOCK		pf_glob_lock;
	PF_GLOB_UNLOCK		pf_glob_unlock;

	PF_PAGE_ALLOC		pf_page_alloc;
	PF_PAGE_REALLOC		pf_page_realloc;
	PF_PAGE_FREE		pf_page_free;
	PF_PAGE_SIZE		pf_page_size;
	PF_PAGE_LOCK		pf_page_lock;
	PF_PAGE_UNLOCK		pf_page_unlock;
	PF_PAGE_PROTECT		pf_page_protect;
}if_memo_t;

/*error interface*/
typedef int (__cdecl *PF_ERROR_GETINFO)(dword_t,tchar_t*, int);
typedef void (__cdecl *PF_ERROR_TRACE)(const tchar_t*);
typedef void(__cdecl *PF_ERROR_EXIT)(void);

typedef struct _if_error_t{
	PF_ERROR_GETINFO pf_error_get_info;
	PF_ERROR_TRACE	 pf_error_trace;
	PF_ERROR_EXIT	pf_error_exit;
}if_error_t;

/*mbcs interface*/
typedef void(__cdecl *PF_GB2312_TO_UNICODE)(const schar_t*, ucode_t*);
typedef void(__cdecl *PF_UNICODE_TO_GB2312)(const ucode_t*, schar_t*);
typedef void(__cdecl *PF_UTF8_TO_UNICODE)(const schar_t*, ucode_t*);
typedef void(__cdecl *PF_UNICODE_TO_UTF8)(const ucode_t*, schar_t*);

typedef struct _if_mbcs_t{
	PF_GB2312_TO_UNICODE	pf_gb2312_to_unicode;
	PF_UNICODE_TO_GB2312	pf_unicode_to_gb2312;
	PF_UTF8_TO_UNICODE		pf_utf8_to_unicode;
	PF_UNICODE_TO_UTF8		pf_unicode_to_utf8;
}if_mbcs_t;

/*thread interface*/
typedef void (__cdecl *PF_THREAD_BEGIN)(PF_THREADFUNC, void*);
typedef void (__cdecl *PF_THREAD_END)(void);
typedef dword_t (__cdecl *PF_THREAD_CREATE_TLS)(void);
typedef void (__cdecl *PF_THREAD_DESTROY_TLS)(dword_t);
typedef void* (__cdecl *PF_THREAD_GET_TLS)(dword_t);
typedef void (__cdecl *PF_THREAD_SET_TLS)(dword_t,void*);
typedef dword_t (__cdecl * PF_THREAD_GET_ID)(void);
typedef void (__cdecl * PF_THREAD_SLEEP)(dword_t);

typedef res_even_t (__cdecl *PF_EVENT_CREATE)(const tchar_t*);
typedef res_even_t(__cdecl *PF_EVENT_OPEN)(const tchar_t*);
typedef void (__cdecl *PF_EVENT_DESTROY)(res_even_t);
typedef int (__cdecl *PF_EVENT_WAIT)(res_even_t, int);
typedef void (__cdecl *PF_EVENT_SIGN)(res_even_t, bool_t);

typedef res_crit_t (__cdecl *PF_CRITI_CREATE)(void);
typedef void (__cdecl *PF_CRITI_DESTROY)(res_crit_t);
typedef bool_t (__cdecl *PF_CRITI_QUERY)(res_crit_t);
typedef void (__cdecl *PF_CRITI_ENTER)(res_crit_t);
typedef void (__cdecl *PF_CRITI_LEAVE)(res_crit_t);

typedef res_mutx_t	(__cdecl *PF_MUTEX_CREATE)(const tchar_t*);
typedef void		(__cdecl *PF_MUTEX_DESTROY)(res_mutx_t);
typedef res_mutx_t	(__cdecl *PF_MUTEX_OPEN)(const tchar_t*);
typedef bool_t		(__cdecl *PF_MUTEX_LOCK)(res_mutx_t, int);
typedef void		(__cdecl *PF_MUTEX_UNLOCK)(res_mutx_t);

typedef struct _if_thread_t{
	PF_THREAD_BEGIN		pf_thread_begin;
	PF_THREAD_END		pf_thread_end;
	PF_THREAD_SLEEP		pf_thread_sleep;
	PF_THREAD_CREATE_TLS pf_thread_create_tls;
	PF_THREAD_DESTROY_TLS pf_thread_destroy_tls;
	PF_THREAD_GET_TLS	pf_thread_get_tls;
	PF_THREAD_SET_TLS	pf_thread_set_tls;
	PF_THREAD_GET_ID	pf_thread_get_id;

	PF_EVENT_CREATE		pf_event_create;
	PF_EVENT_OPEN		pf_event_open;
	PF_EVENT_DESTROY	pf_event_destroy;
	PF_EVENT_WAIT		pf_event_wait;
	PF_EVENT_SIGN		pf_event_sign;

	PF_CRITI_CREATE		pf_criti_create;
	PF_CRITI_DESTROY	pf_criti_destroy;
	PF_CRITI_QUERY		pf_criti_query;
	PF_CRITI_ENTER		pf_criti_enter;
	PF_CRITI_LEAVE		pf_criti_leave;

	PF_MUTEX_CREATE		pf_mutex_create;
	PF_MUTEX_DESTROY	pf_mutex_destroy;
	PF_MUTEX_OPEN		pf_mutex_open;
	PF_MUTEX_LOCK		pf_mutex_lock;
	PF_MUTEX_UNLOCK		pf_mutex_unlock;
}if_thread_t;

/*timer interface*/
typedef res_queue_t(__cdecl *PF_CREATE_TIMER_QUEUE)(void);
typedef void(__cdecl *PF_DESTROY_TIMER_QUEUE)(res_queue_t);
typedef res_timer_t(__cdecl *PF_CREATE_TIMER)(res_queue_t, dword_t, dword_t, overlap_t*);
typedef void(__cdecl *PF_DESTROY_TIMER)(res_queue_t, res_timer_t, res_even_t);
typedef bool_t(__cdecl *PF_ALTER_TIMER)(res_queue_t, res_timer_t, dword_t, dword_t);

typedef struct _if_timer_t{
	PF_CREATE_TIMER_QUEUE		pf_create_timer_queue;
	PF_DESTROY_TIMER_QUEUE		pf_destroy_timer_queue;
	PF_CREATE_TIMER				pf_create_timer;
	PF_DESTROY_TIMER			pf_destroy_timer;
	PF_ALTER_TIMER				pf_alter_timer;
}if_timer_t;

/*socket interface*/
typedef bool_t (__cdecl *PF_SOCKET_STARTUP)(void);
typedef void (__cdecl *PF_SOCKET_CLEANUP)(void);
typedef res_sock_t (__cdecl *PF_SOCKET_OPEN)(int, int, int);
typedef void (__cdecl *PF_SOCKET_SHUTDOWN)(res_sock_t, int);
typedef void (__cdecl *PF_SOCKET_CLOSE)(res_sock_t);
typedef bool_t(__cdecl *PF_SOCKET_WAIT)(res_sock_t, int);
typedef bool_t (__cdecl *PF_SOCKET_BIND)(res_sock_t, res_addr_t, int);
typedef bool_t (__cdecl *PF_SOCKET_CONNECT)(res_sock_t, res_addr_t, int);
typedef bool_t (__cdecl *PF_SOCKET_SEND)(res_sock_t, void*, dword_t, dword_t*, int);
typedef bool_t (__cdecl *PF_SOCKET_RECV)(res_sock_t, void*, dword_t, dword_t*, int);
typedef bool_t (__cdecl *PF_SOCKET_PEEK)(res_sock_t, void*, dword_t, dword_t*);
typedef bool_t (__cdecl *PF_SOCKET_SETOPT)(res_sock_t, int, int, const char*, int);
typedef bool_t (__cdecl *PF_SOCKET_GETOPT)(res_sock_t, int, int, char*, int*);
typedef bool_t (__cdecl *PF_HOST_ADDR)(const schar_t*, schar_t*);
typedef void (__cdecl *PF_FILL_ADDR)(net_addr_t*, int, short, const schar_t*);
typedef void(__cdecl *PF_SOCKET_ADDR)(net_addr_t*, schar_t*);
typedef void(__cdecl *PF_SOCKET_PEER)(res_sock_t, net_addr_t*);
typedef bool_t (__cdecl *PF_SOCKET_LISTEN)(res_sock_t, int);
typedef res_sock_t (__cdecl *PF_SOCKET_ACCEPT)(res_sock_t, res_addr_t, int*);
typedef dword_t (__cdecl *PF_SOCKET_SHARE_SIZE)(void);
typedef dword_t (__cdecl *PF_SOCKET_SHARE)(res_sock_t, dword_t, byte_t*);
typedef res_sock_t (__cdecl *PF_SOCKET_DUPLI)(const byte_t*, dword_t);
typedef int(__cdecl *PF_SOCKET_WRITE)(void* pso, unsigned char* buf, int len);
typedef int(__cdecl *PF_SOCKET_READ)(void* pso, unsigned char* buf, int len);

typedef struct _if_socket_t{
	PF_SOCKET_STARTUP		pf_socket_startup;
	PF_SOCKET_CLEANUP		pf_socket_cleanup;
	PF_SOCKET_OPEN			pf_socket_open;
	PF_SOCKET_CLOSE			pf_socket_close;
	PF_SOCKET_WAIT			pf_socket_wait;
	PF_SOCKET_SHUTDOWN		pf_socket_shutdown;
	PF_SOCKET_BIND			pf_socket_bind;
	PF_SOCKET_CONNECT		pf_socket_connect;
	PF_SOCKET_SEND			pf_socket_send;
	PF_SOCKET_RECV			pf_socket_recv;
	PF_SOCKET_PEEK			pf_socket_peek;
	PF_SOCKET_SETOPT		pf_socket_setopt;
	PF_SOCKET_GETOPT		pf_socket_getopt;
	PF_HOST_ADDR			pf_host_addr;
	PF_FILL_ADDR			pf_fill_addr;
	PF_SOCKET_PEER			pf_socket_peer;
	PF_SOCKET_ADDR			pf_socket_addr;
	PF_SOCKET_LISTEN		pf_socket_listen;
	PF_SOCKET_ACCEPT		pf_socket_accept;
	PF_SOCKET_DUPLI			pf_socket_dupli;
	PF_SOCKET_SHARE			pf_socket_share;
	PF_SOCKET_SHARE_SIZE	pf_socket_share_size;
	PF_SOCKET_WRITE			pf_socket_write;
	PF_SOCKET_READ			pf_socket_read;
}if_socket_t;

/*file interface*/
typedef res_file_t (__cdecl *PF_FILE_OPEN)(const tchar_t*, dword_t);
typedef void (__cdecl *PF_FILE_CLOSE)(res_file_t);
typedef bool_t (__cdecl *PF_FILE_WRITE)(res_file_t, void*, dword_t, dword_t*);
typedef bool_t (__cdecl *PF_FILE_READ)(res_file_t, void*, dword_t, dword_t*);
typedef bool_t(__cdecl *PF_FILE_PEEK)(res_file_t, void*, dword_t, dword_t*);
typedef bool_t (__cdecl *PF_FILE_READ_RANGE)(res_file_t, void*, dword_t, dword_t, dword_t, dword_t*);
typedef bool_t(__cdecl *PF_FILE_WRITE_RANGE)(res_file_t, void*, dword_t, dword_t, dword_t, dword_t*);
typedef bool_t (__cdecl *PF_FILE_DELETE)(const tchar_t*);
typedef bool_t (__cdecl *PF_FILE_INFO)(const tchar_t*, file_info_t*);
typedef res_find_t (__cdecl *PF_FILE_FIND_FIRST)(const tchar_t*, file_info_t*);
typedef bool_t (__cdecl *PF_FILE_FIND_NEXT)(res_find_t, file_info_t*);
typedef bool_t (__cdecl *PF_FILE_SETTIME)(res_file_t fh, const xdate_t* pdt);
typedef bool_t(__cdecl *PF_FILE_GETTIME)(res_file_t fh, xdate_t* pdt);
typedef bool_t(__cdecl *PF_DIRECTORY_OPEN)(const tchar_t*, dword_t);
typedef bool_t(__cdecl *PF_DIRECTORY_CREATE)(const tchar_t*);
typedef bool_t(__cdecl *PF_DIRECTORY_REMOVE)(const tchar_t*);

typedef struct _if_file_t{
	PF_FILE_OPEN		pf_file_open;
	PF_FILE_CLOSE		pf_file_close;
	PF_FILE_WRITE		pf_file_write;
	PF_FILE_READ		pf_file_read;
	PF_FILE_PEEK		pf_file_peek;
	PF_FILE_READ_RANGE	pf_file_read_range;
	PF_FILE_WRITE_RANGE	pf_file_write_range;
	PF_FILE_DELETE		pf_file_delete;
	PF_FILE_INFO		pf_file_info;
	PF_FILE_SETTIME		pf_file_settime;
	PF_FILE_GETTIME		pf_file_gettime;
	PF_FILE_FIND_FIRST	pf_file_find_first;
	PF_FILE_FIND_NEXT	pf_file_find_next;
	PF_DIRECTORY_OPEN	pf_directory_open;
	PF_DIRECTORY_CREATE	pf_directory_create;
	PF_DIRECTORY_REMOVE	pf_directory_remove;
}if_file_t;

/*pipe interface*/
typedef void (__cdecl *PF_PIPE_SRV)(const tchar_t*, res_pipe_t*, res_pipe_t*);
typedef void (__cdecl *PF_PIPE_CLI)(const tchar_t*, res_pipe_t*, res_pipe_t*);
typedef bool_t (__cdecl *PF_PIPE_WRITE)(res_pipe_t, void*, dword_t, dword_t*);
typedef bool_t (__cdecl *PF_PIPE_READ)(res_pipe_t, void*, dword_t, dword_t*);
typedef void (__cdecl *PF_PIPE_CLOSE)(res_pipe_t);
typedef void(__cdecl *PF_PIPE_STOP)(res_pipe_t);
typedef bool_t(__cdecl *PF_PIPE_WAIT)(const tchar_t*, dword_t);
typedef bool_t(__cdecl *PF_PIPE_FLUSH)(res_pipe_t);

typedef struct _if_pipe_t{
	PF_PIPE_SRV			pf_pipe_srv;
	PF_PIPE_CLI			pf_pipe_cli;
	PF_PIPE_CLOSE		pf_pipe_close;
	PF_PIPE_STOP		pf_pipe_stop;
	PF_PIPE_WRITE		pf_pipe_write;
	PF_PIPE_READ		pf_pipe_read;
	PF_PIPE_WAIT		pf_pipe_wait;
	PF_PIPE_FLUSH		pf_pipe_flush;
}if_pipe_t;

/*block interface*/
typedef res_file_t(__cdecl *PF_BLOCK_OPEN)(const tchar_t*, dword_t);
typedef res_file_t(__cdecl *PF_BLOCK_SHARE)(const tchar_t* fname, const tchar_t* fpath, dword_t* pb);
typedef void(__cdecl *PF_BLOCK_CLOSE)(res_file_t);
typedef bool_t(__cdecl *PF_BLOCK_WRITE)(res_file_t, dword_t, void*, dword_t, dword_t*);
typedef bool_t(__cdecl *PF_BLOCK_READ)(res_file_t, dword_t, void*, dword_t, dword_t*);

typedef struct _if_block_t{
	PF_BLOCK_OPEN		pf_block_open;
	PF_BLOCK_SHARE		pf_block_share;
	PF_BLOCK_CLOSE		pf_block_close;
	PF_BLOCK_WRITE		pf_block_write;
	PF_BLOCK_READ		pf_block_read;
}if_block_t;

/*console interface*/
typedef bool_t (__cdecl *PF_CONS_ALLOC)(void);
typedef void (__cdecl *PF_CONS_FREE)(void);
typedef bool_t (__cdecl *PF_CONS_WRITE)(void*, dword_t, dword_t*);
typedef bool_t (__cdecl *PF_CONS_READ)(void*, dword_t, dword_t*);
typedef bool_t (__cdecl *PF_CONS_STDOUT)(void);
typedef bool_t (__cdecl *PF_CONS_STDIN)(void);

typedef struct _if_cons_t{
	PF_CONS_ALLOC		pf_cons_alloc;
	PF_CONS_FREE		pf_cons_free;
	PF_CONS_WRITE		pf_cons_write;
	PF_CONS_READ		pf_cons_read;
	PF_CONS_STDOUT		pf_cons_stdout;
	PF_CONS_STDIN		pf_cons_stdin;
}if_cons_t;

/*date interface*/
typedef void (__cdecl *PF_GET_LOC_DATE)(xdate_t*);
typedef bool_t(__cdecl *PF_MAK_LOC_DATE)(xdate_t*);
typedef void (__cdecl *PF_GET_UTC_DATE)(xdate_t*);
typedef bool_t(__cdecl *PF_MAK_UTC_DATE)(xdate_t*);
typedef dword_t (__cdecl *PF_GET_TIMES)(void);
typedef dword_t (__cdecl *PF_GET_TICKS)(void);

typedef struct _if_date_t{
	PF_GET_LOC_DATE		pf_get_loc_date;
	PF_MAK_LOC_DATE		pf_mak_loc_date;
	PF_GET_UTC_DATE		pf_get_utc_date;
	PF_MAK_UTC_DATE		pf_mak_utc_date;
	PF_GET_TIMES		pf_get_times;
	PF_GET_TICKS		pf_get_ticks;
}if_date_t;

/*moudle interface*/
typedef res_modu_t(__cdecl *PF_LOAD_LIBRARY)(const tchar_t*);
typedef void (__cdecl *PF_FREE_LIBRARY)(res_modu_t);
typedef void* (__cdecl *PF_GET_ADDRESS)(res_modu_t, const tchar_t*);
typedef void (__cdecl *PF_GET_RUNPATH)(res_modu_t, tchar_t*, int);
typedef bool_t (__cdecl *PF_CREATE_PROCESS)(const tchar_t*, const tchar_t*, bool_t, proc_info_t*);
typedef void (__cdecl *PF_RELEASE_PROCESS)(proc_info_t*);
typedef res_void_t (__cdecl *PF_PROCESS_DUPLI)(res_proc_t, res_void_t);
typedef void* (__cdecl *PF_PROCESS_ALLOC)(res_proc_t, dword_t);
typedef void (__cdecl *PF_PROCESS_FREE)(res_proc_t, void*);
typedef bool_t (__cdecl *PF_PROCESS_WRITE)(res_proc_t, void*, const void*, dword_t);
typedef bool_t (__cdecl *PF_PROCESS_READ)(res_proc_t, void*, void*, dword_t);
typedef void(__cdecl *PF_RELEASE_HANDLE)(res_void_t);
typedef void(__cdecl *PF_WAITFOR_HANDLE)(res_void_t, dword_t);
typedef bool_t(__cdecl *PF_INHERIT_HANDLE)(res_void_t, bool_t);
typedef void(__cdecl *PF_WRITE_PROFILE)(const tchar_t*, const tchar_t*, const tchar_t*, const tchar_t*);
typedef void(__cdecl *PF_READ_PROFILE)(const tchar_t*, const tchar_t*, const tchar_t*, tchar_t*, int);

typedef struct _if_module_t{
	PF_LOAD_LIBRARY		pf_load_library;
	PF_FREE_LIBRARY		pf_free_library;
	PF_GET_ADDRESS		pf_get_address;
	PF_GET_RUNPATH		pf_get_runpath;
	PF_CREATE_PROCESS	pf_create_process;
	PF_RELEASE_PROCESS	pf_release_process;
	PF_PROCESS_DUPLI	pf_process_dupli;
	PF_PROCESS_ALLOC	pf_process_alloc;
	PF_PROCESS_FREE		pf_process_free;
	PF_PROCESS_WRITE	pf_process_write;
	PF_PROCESS_READ		pf_process_read;
	PF_RELEASE_HANDLE	pf_release_handle;
	PF_WAITFOR_HANDLE	pf_waitfor_handle;
	PF_INHERIT_HANDLE	pf_inherit_handle;
	PF_WRITE_PROFILE	pf_write_profile;
	PF_READ_PROFILE		pf_read_profile;
}if_module_t;

/*context interface*/
typedef res_ctx_t(__cdecl *PF_CREATE_DISPLAY_CONTEXT)(void);
typedef res_ctx_t(__cdecl *PF_CREATE_COMPATIBLE_CONTEXT)(res_ctx_t);
typedef void(__cdecl *PF_DESTROY_CONTEXT)(res_ctx_t);
typedef void(__cdecl *PF_GET_DEVICE_CAPS)(res_ctx_t, dev_cap_t*);
typedef res_obj_t(__cdecl *PF_SELECT_OBJECT)(res_ctx_t, res_obj_t);
typedef void(__cdecl *PF_RENDER_CONTEXT)(res_ctx_t, long, long, res_ctx_t, long, long, long, long);
typedef res_bmp_t(__cdecl *PF_CREATE_COMPATIBLE_BITMAP)(res_ctx_t, long, long);
typedef void(__cdecl *PF_DELETE_BITMAP)(res_bmp_t);
typedef void(__cdecl *PF_GET_BITMAP_SIZE)(res_bmp_t, long*, long*);

typedef float(__cdecl *PF_PT_PER_MM)(res_ctx_t, bool_t);
typedef void(__cdecl *PF_TEXT_MM_SIZE)(res_ctx_t, const xfont_t*, const tchar_t*, int, float*, float*);
typedef void(__cdecl *PF_TEXT_PT_SIZE)(res_ctx_t, const xfont_t*, const tchar_t*, int, long*, long*);
typedef void(__cdecl *PF_TEXT_MM_METRIC)(res_ctx_t, const xfont_t*, float*, float*);
typedef void(__cdecl *PF_TEXT_PT_METRIC)(res_ctx_t, const xfont_t*, long*, long*);
typedef float(__cdecl *PF_CAST_PT_TO_MM)(res_ctx_t, long, bool_t);
typedef long(__cdecl *PF_CAST_MM_TO_PT)(res_ctx_t, float, bool_t);
typedef int(__cdecl *PF_FONT_SIZE)(res_ctx_t, long);

typedef struct _if_context_t{
	PF_CREATE_COMPATIBLE_CONTEXT	pf_create_compatible_context;
	PF_CREATE_DISPLAY_CONTEXT	pf_create_display_context;
	PF_DESTROY_CONTEXT			pf_destroy_context;
	PF_GET_DEVICE_CAPS			pf_get_device_caps;
	PF_SELECT_OBJECT			pf_select_object;
	PF_RENDER_CONTEXT			pf_render_context;
	PF_CREATE_COMPATIBLE_BITMAP	pf_create_compatible_bitmap;
	PF_DELETE_BITMAP			pf_delete_bitmap;
	PF_GET_BITMAP_SIZE			pf_get_bitmap_size;

	PF_TEXT_MM_SIZE		pf_text_mm_size;
	PF_TEXT_PT_SIZE		pf_text_pt_size;
	PF_TEXT_MM_METRIC	pf_text_mm_metric;
	PF_TEXT_PT_METRIC	pf_text_pt_metric;
	PF_CAST_PT_TO_MM	pf_cast_pt_to_mm;
	PF_CAST_MM_TO_PT	pf_cast_mm_to_pt;
	PF_PT_PER_MM		pf_pt_per_mm;
	PF_FONT_SIZE		pf_font_size;
}if_context_t;

/*bitmap interface*/
typedef res_bmp_t(__cdecl *PF_CREATE_COLOR_BITMAP)(res_ctx_t, const xcolor_t*, long, long);
typedef res_bmp_t(__cdecl *PF_CREATE_PATTERN_BITMAP)(res_ctx_t, const xcolor_t*, const xcolor_t*, long, long, const tchar_t*);
typedef res_bmp_t(__cdecl *PF_CREATE_GRADIENT_BITMAP)(res_ctx_t, const xcolor_t*, const xcolor_t*, long, long, const tchar_t*);
typedef res_bmp_t(__cdecl *PF_CREATE_CODE128_BITMAP)(res_ctx_t, long, long, const byte_t*, dword_t, const tchar_t*);
typedef res_bmp_t(__cdecl *PF_CREATE_PDF417_BITMAP)(res_ctx_t, long, long, const byte_t*, dword_t, int, int);
typedef res_bmp_t(__cdecl *PF_CREATE_QRCODE_BITMAP)(res_ctx_t, long, long, const byte_t*, dword_t, int, int);
typedef res_bmp_t(__cdecl *PF_LOAD_BITMAP_FROM_FILE)(res_ctx_t, const tchar_t*);
typedef res_bmp_t(__cdecl *PF_LOAD_BITMAP_FROM_BYTES)(res_ctx_t, const byte_t*, dword_t);
typedef dword_t(__cdecl *PF_SAVE_BITMAP_TO_BYTES)(res_ctx_t, res_bmp_t, byte_t*, dword_t);
typedef dword_t(__cdecl *PF_GET_BITMAP_BYTES)(res_bmp_t);
typedef res_bmp_t(__cdecl *PF_LOAD_BITMAP_FROM_THUMB)(res_ctx_t, const tchar_t*);
typedef res_bmp_t(__cdecl *PF_LOAD_BITMAP_FROM_ICON)(res_ctx_t, const tchar_t*);

typedef struct _if_bitmap_t{
	PF_CREATE_COLOR_BITMAP		pf_create_color_bitmap;
	PF_CREATE_PATTERN_BITMAP	pf_create_pattern_bitmap;
	PF_CREATE_GRADIENT_BITMAP	pf_create_gradient_bitmap;
	PF_CREATE_CODE128_BITMAP	pf_create_code128_bitmap;
	PF_CREATE_PDF417_BITMAP		pf_create_pdf417_bitmap;
	PF_CREATE_QRCODE_BITMAP		pf_create_qrcode_bitmap;
	PF_LOAD_BITMAP_FROM_FILE	pf_load_bitmap_from_file;
	PF_LOAD_BITMAP_FROM_BYTES	pf_load_bitmap_from_bytes;
	PF_SAVE_BITMAP_TO_BYTES		pf_save_bitmap_to_bytes;
	PF_GET_BITMAP_BYTES			pf_get_bitmap_bytes;

	PF_LOAD_BITMAP_FROM_THUMB	pf_load_bitmap_from_thumb;
	PF_LOAD_BITMAP_FROM_ICON	pf_load_bitmap_from_icon;
}if_bitmap_t;

/*graphic interface*/
typedef void(__cdecl *PF_GDI_STARTUP)(int osv);
typedef void(__cdecl *PF_GDI_CLEANUP)(void);
typedef void(__cdecl *PF_GDI_DRAW_LINE)(res_ctx_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(__cdecl *PF_GDI_DRAW_3DLINE)(res_ctx_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(__cdecl *PF_GDI_DRAW_3DRECT)(res_ctx_t, const xpen_t*, const xrect_t*);
typedef void(__cdecl *PF_GDI_DRAW_POLYLINE)(res_ctx_t, const xpen_t*, const xpoint_t*, int);
typedef void(__cdecl *PF_GDI_DRAW_POLYGON)(res_ctx_t, const xpen_t*, const xbrush_t*, const xpoint_t*, int);
typedef void(__cdecl *PF_GDI_DRAW_RECT)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(__cdecl *PF_GDI_GRADIENT_RECT)(res_ctx_t, const xgradi_t*, const xrect_t*);
typedef void(__cdecl *PF_GDI_ALPHA_RECT)(res_ctx_t, const xcolor_t*, const xrect_t*, int);
typedef void(__cdecl *PF_GDI_DRAW_ROUND)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(__cdecl *PF_GDI_DRAW_ELLIPSE)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(__cdecl *PF_GDI_DRAW_PIE)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*, double, double);
typedef void(__cdecl *PF_GDI_DRAW_ARROW)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*, int, double);
typedef void(__cdecl *PF_GDI_DRAW_SHAPE)(res_ctx_t, const xpen_t*, const xbrush_t*, const xrect_t*, const tchar_t*);
typedef void(__cdecl *PF_GDI_MULTI_LINE)(res_ctx_t, const xfont_t*, const xface_t*, const xpen_t*, const xrect_t*);
typedef void(__cdecl *PF_GDI_DRAW_TEXT)(res_ctx_t, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int);
typedef void(__cdecl *PF_GDI_TEXT_OUT)(res_ctx_t, const xfont_t*, const xpoint_t*, const tchar_t*, int);
//typedef void(__cdecl *PF_GDI_DRAW_TAG_TEXT)(res_ctx_t, const xfont_t*, const xface_t*, const xrect_t*, LINKPTR);
//typedef void(__cdecl *PF_GDI_DRAW_VAR_TEXT)(res_ctx_t, const xfont_t*, const xface_t*, const xrect_t*, XHANDLE);
typedef void(__cdecl *PF_GDI_DRAW_IMAGE)(res_ctx_t, const ximage_t*, const xrect_t*);
typedef void(__cdecl *PF_GDI_DRAW_BITMAP)(res_ctx_t, res_bmp_t, const xrect_t*);
typedef void(__cdecl *PF_GDI_DRAW_ICON)(res_ctx_t, const xcolor_t*, const xrect_t*, const tchar_t*);
typedef void(__cdecl *PF_GDI_FILL_REGION)(res_ctx_t, const xbrush_t*, res_rgn_t);

typedef void(__cdecl *PF_GDI_EXCLIP_RECT)(res_ctx_t, const xrect_t*);
typedef void(__cdecl *PF_GDI_INCLIP_RECT)(res_ctx_t, const xrect_t*);

typedef void(__cdecl *PF_GDI_TEXT_SIZE)(res_ctx_t, const xfont_t*, const tchar_t*, int, xsize_t*);
typedef void(__cdecl *PF_GDI_TEXT_METRIC)(res_ctx_t, const xfont_t*, xsize_t*);
typedef void(__cdecl *PF_GDI_IMAGE_SIZE)(res_ctx_t, const ximage_t*, xsize_t*);

typedef struct _if_graphic_t{
	PF_GDI_STARTUP			pf_gdi_startup;
	PF_GDI_CLEANUP			pf_gdi_cleanup;
	PF_GDI_DRAW_LINE		pf_gdi_draw_line;
	PF_GDI_DRAW_3DLINE		pf_gdi_draw_3dline;
	PF_GDI_DRAW_POLYLINE	pf_gdi_draw_polyline;
	PF_GDI_DRAW_POLYGON		pf_gdi_draw_polygon;
	PF_GDI_DRAW_3DRECT		pf_gdi_draw_3drect;
	PF_GDI_DRAW_RECT		pf_gdi_draw_rect;
	PF_GDI_GRADIENT_RECT	pf_gdi_gradinet_rect;
	PF_GDI_ALPHA_RECT		pf_gdi_alpha_rect;
	PF_GDI_DRAW_ROUND		pf_gdi_draw_round;
	PF_GDI_DRAW_ELLIPSE		pf_gdi_draw_ellipse;
	PF_GDI_DRAW_PIE			pf_gdi_draw_pie;
	PF_GDI_DRAW_ARROW		pf_gdi_draw_arrow;
	PF_GDI_DRAW_SHAPE		pf_gdi_draw_shape;
	PF_GDI_MULTI_LINE		pf_gdi_multi_line;
	PF_GDI_DRAW_TEXT		pf_gdi_draw_text;
	PF_GDI_TEXT_OUT			pf_gdi_text_out;
	PF_GDI_FILL_REGION		pf_gdi_fill_region;
	PF_GDI_DRAW_IMAGE		pf_gdi_draw_image;
	PF_GDI_DRAW_BITMAP		pf_gdi_draw_bitmap;
	PF_GDI_DRAW_ICON		pf_gdi_draw_icon;
	//PF_GDI_DRAW_TAG_TEXT	pf_gdi_draw_tag_text;
	//PF_GDI_DRAW_VAR_TEXT	pf_gdi_draw_var_text;
	PF_GDI_TEXT_SIZE		pf_gdi_text_size;
	PF_GDI_TEXT_METRIC		pf_gdi_text_metric;
	PF_GDI_IMAGE_SIZE		pf_gdi_image_size;
	PF_GDI_EXCLIP_RECT		pf_gdi_exclip_rect;
	PF_GDI_INCLIP_RECT		pf_gdi_inclip_rect;
}if_graphic_t;

/*shell interface*/
typedef bool_t(__cdecl *PF_SHELL_GET_FILENAME)(res_win_t, const tchar_t*, const tchar_t*, const tchar_t*, bool_t, tchar_t*, int, tchar_t*, int);
typedef bool_t(__cdecl *PF_SHELL_GET_PATHNAME)(res_win_t, const tchar_t*, bool_t, tchar_t*, int);
typedef bool_t(__cdecl *PF_SHELL_GET_RUNPATH)(tchar_t*, int);
typedef bool_t(__cdecl *PF_SHELL_GET_CURPATH)(tchar_t*, int);
typedef bool_t(__cdecl *PF_SHELL_GET_APPPATH)(tchar_t*, int);
typedef bool_t(__cdecl *PF_SHELL_GET_DOCPATH)(tchar_t*, int);
typedef bool_t(__cdecl *PF_SHELL_GET_TMPPATH)(tchar_t*, int);

typedef struct _if_shell_t{
	PF_SHELL_GET_FILENAME	pf_shell_get_filename;
	PF_SHELL_GET_PATHNAME	pf_shell_get_pathname;
	PF_SHELL_GET_RUNPATH	pf_shell_get_runpath;
	PF_SHELL_GET_CURPATH	pf_shell_get_curpath;
	PF_SHELL_GET_APPPATH	pf_shell_get_apppath;
	PF_SHELL_GET_DOCPATH	pf_shell_get_docpath;
	PF_SHELL_GET_TMPPATH	pf_shell_get_tmppath;
}if_shell_t;

/*resource interface*/
typedef void(__cdecl *PF_GET_SCREEN_SIZE)(xsize_t*);
typedef void(__cdecl *PF_GET_DESKTOP_SIZE)(xsize_t*);
typedef void(__cdecl *PF_SCREEN_SIZE_TO_PT)(xsize_t*);
typedef void(__cdecl *PF_SCREEN_SIZE_TO_TM)(xsize_t*);
typedef res_acl_t(__cdecl *PF_CREATE_ACCEL_TABLE)(const accel_t*, int);
typedef void(__cdecl *PF_DESTROY_ACCEL_TABLE)(res_acl_t);
typedef res_rgn_t(__cdecl *PF_CREATE_REGION)(const tchar_t*, const xrect_t*);
typedef void(__cdecl *PF_DELETE_REGION)(res_rgn_t);
typedef bool_t(__cdecl *PF_PT_IN_REGION)(res_rgn_t, const xpoint_t*);

typedef struct _if_resource_t{
	PF_GET_SCREEN_SIZE		pf_get_screen_size;
	PF_GET_DESKTOP_SIZE		pf_get_desktop_size;
	PF_SCREEN_SIZE_TO_PT	pf_screen_size_to_pt;
	PF_SCREEN_SIZE_TO_TM	pf_screen_size_to_tm;
	PF_CREATE_ACCEL_TABLE	pf_create_accel_table;
	PF_DESTROY_ACCEL_TABLE	pf_destroy_accel_table;
	PF_CREATE_REGION		pf_create_region;
	PF_DELETE_REGION		pf_delete_region;
	PF_PT_IN_REGION			pf_pt_in_region;
}if_resource_t;

/*printer interface*/
typedef bool_t(__cdecl *PF_DEFAULT_PRINTER_MODE)(dev_mod_t*);
typedef bool_t(__cdecl *PF_SETUP_PRINTER_MODE)(res_win_t, dev_mod_t*);
typedef res_ctx_t(__cdecl *PF_CREATE_PRINTER_CONTEXT)(const dev_mod_t*);
typedef void(__cdecl *PF_DESTROY_PRINTER_CONTEXT)(res_ctx_t);
typedef void(__cdecl *PF_BEGIN_PAGE)(res_ctx_t);
typedef void(__cdecl *PF_END_PAGE)(res_ctx_t);
typedef void(__cdecl *PF_BEGIN_DOC)(res_ctx_t, const tchar_t*);
typedef void(__cdecl *PF_END_DOC)(res_ctx_t);

typedef struct _if_printer_t{
	PF_SETUP_PRINTER_MODE		pf_setup_printer_mode;
	PF_DEFAULT_PRINTER_MODE		pf_default_printer_mode;
	PF_CREATE_PRINTER_CONTEXT	pf_create_printer_context;
	PF_DESTROY_PRINTER_CONTEXT	pf_destroy_printer_context;
	PF_BEGIN_PAGE				pf_begin_page;
	PF_END_PAGE					pf_end_page;
	PF_BEGIN_DOC				pf_begin_doc;
	PF_END_DOC					pf_end_doc;
}if_printer_t;

/*clipboard interface*/
typedef bool_t(__cdecl *PF_CLIPBOARD_OPEN)(void);
typedef void(__cdecl *PF_CLIPBOARD_CLOSE)(void);
typedef void(__cdecl *PF_CLIPBOARD_CLEAN)(void);
typedef bool_t(__cdecl *PF_CLIPBOARD_PUT)(int, res_glob_t);
typedef res_glob_t(__cdecl *PF_CLIPBOARD_GET)(int fmt);

typedef struct _if_clipboard_t{
	PF_CLIPBOARD_OPEN		pf_clipboard_open;
	PF_CLIPBOARD_CLOSE		pf_clipboard_close;
	PF_CLIPBOARD_CLEAN		pf_clipboard_clean;
	PF_CLIPBOARD_PUT		pf_clipboard_put;
	PF_CLIPBOARD_GET		pf_clipboard_get;
}if_clipboard_t;

/*subclass widget event*/
typedef int(__cdecl *SUB_ON_LBUTTON_DOWN)(res_win_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_LBUTTON_UP)(res_win_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_LBUTTON_DBCLICK)(res_win_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_RBUTTON_DOWN)(res_win_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_RBUTTON_UP)(res_win_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_MOUSE_MOVE)(res_win_t, dword_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_MOUSE_HOVER)(res_win_t, dword_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_MOUSE_LEAVE)(res_win_t, dword_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_SCROLL)(res_win_t, bool_t, long, dword_t, void*);
typedef int(__cdecl *SUB_ON_KEYDOWN)(res_win_t, int, dword_t, void*);
typedef int(__cdecl *SUB_ON_CHAR)(res_win_t, tchar_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_SIZE)(res_win_t, int, const xsize_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_MOVE)(res_win_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_SHOW)(res_win_t, bool_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_PAINT)(res_win_t, res_ctx_t, const xrect_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_ERASE)(res_win_t, res_ctx_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_SET_FOCUS)(res_win_t, res_win_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_KILL_FOCUS)(res_win_t, res_win_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_ENABLE)(res_win_t, bool_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_NOTICE)(res_win_t, NOTICE*, dword_t, void*);
typedef int(__cdecl *SUB_ON_MENU_COMMAND)(res_win_t, int, int, void*, dword_t, void*);
typedef int(__cdecl *SUB_ON_PARENT_COMMAND)(res_win_t, int, void*, dword_t, void*);
typedef int(__cdecl *SUB_ON_CHILD_COMMAND)(res_win_t, int, void*, dword_t, void*);
typedef int(__cdecl *SUB_ON_SELF_COMMAND)(res_win_t, int, void*, dword_t, void*);
typedef int(__cdecl *SUB_ON_COMMAND_FIND)(res_win_t, str_find_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_COMMAND_REPLACE)(res_win_t, str_replace_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_TIMER)(res_win_t, long_ptr, dword_t, void*);
typedef int(__cdecl *SUB_ON_COPY)(res_win_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_CUT)(res_win_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_PASTE)(res_win_t, dword_t, void*);
typedef int(__cdecl *SUB_ON_UNDO)(res_win_t, dword_t, void*);

typedef int(__cdecl *SUB_ON_SYSCLR_CLICK)(res_win_t, const xpoint_t*, dword_t, void*);
typedef int(__cdecl *SUB_ON_SYSLOG_CLICK)(res_win_t, const xpoint_t*, dword_t, void*);

typedef int(__cdecl *SUB_ON_CLOSE)(res_win_t, dword_t, void*);
typedef void(__cdecl *SUB_ON_DESTROY)(res_win_t, dword_t, void*);

typedef void(__cdecl *SUB_ON_SUBING)(res_win_t, dword_t, void*);

typedef struct _if_subproc_t{
	void* delta;

	SUB_ON_SUBING		sub_on_subing;
	SUB_ON_DESTROY		sub_on_destroy;
	SUB_ON_CLOSE		sub_on_close;

	SUB_ON_LBUTTON_DOWN	sub_on_lbutton_down;
	SUB_ON_LBUTTON_UP	sub_on_lbutton_up;
	SUB_ON_LBUTTON_DBCLICK	sub_on_lbutton_dbclick;
	SUB_ON_RBUTTON_DOWN	sub_on_rbutton_down;
	SUB_ON_RBUTTON_UP	sub_on_rbutton_up;
	SUB_ON_MOUSE_MOVE	sub_on_mouse_move;
	SUB_ON_MOUSE_HOVER	sub_on_mouse_hover;
	SUB_ON_MOUSE_LEAVE	sub_on_mouse_leave;
	SUB_ON_SCROLL		sub_on_scroll;
	SUB_ON_KEYDOWN		sub_on_keydown;
	SUB_ON_CHAR			sub_on_char;
	SUB_ON_SIZE			sub_on_size;
	SUB_ON_MOVE			sub_on_move;
	SUB_ON_SHOW			sub_on_show;
	SUB_ON_ENABLE		sub_on_enable;
	SUB_ON_SET_FOCUS	sub_on_set_focus;
	SUB_ON_KILL_FOCUS	sub_on_kill_focus;
	SUB_ON_PAINT		sub_on_paint;
	SUB_ON_ERASE		sub_on_erase;
	SUB_ON_NOTICE		sub_on_notice;
	SUB_ON_MENU_COMMAND		sub_on_menu_command;
	SUB_ON_PARENT_COMMAND	sub_on_parent_command;
	SUB_ON_CHILD_COMMAND	sub_on_child_command;
	SUB_ON_SELF_COMMAND		sub_on_self_command;
	SUB_ON_COMMAND_FIND		sub_on_command_find;
	SUB_ON_COMMAND_REPLACE	sub_on_command_replace;

	SUB_ON_SYSCLR_CLICK		sub_on_sysclr_click;
	SUB_ON_SYSLOG_CLICK		sub_on_syslog_click;

	SUB_ON_TIMER		sub_on_timer;
	SUB_ON_COPY			sub_on_copy;
	SUB_ON_CUT			sub_on_cut;
	SUB_ON_PASTE		sub_on_paste;
	SUB_ON_UNDO			sub_on_undo;

	void* proc;
	dword_t sid;
}if_subproc_t;


typedef int(__cdecl *PF_ON_CLOSE)(res_win_t);
typedef int(__cdecl *PF_ON_CREATE)(res_win_t, void*);
typedef void(__cdecl *PF_ON_DESTROY)(res_win_t);
typedef void(__cdecl *PF_ON_LBUTTON_DOWN)(res_win_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_LBUTTON_UP)(res_win_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_LBUTTON_DBCLICK)(res_win_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_RBUTTON_DOWN)(res_win_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_RBUTTON_UP)(res_win_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_MOUSE_MOVE)(res_win_t, dword_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_MOUSE_HOVER)(res_win_t, dword_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_MOUSE_LEAVE)(res_win_t, dword_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_SCROLL)(res_win_t, bool_t, long);
typedef void(__cdecl *PF_ON_KEYDOWN)(res_win_t, int);
typedef void(__cdecl *PF_ON_CHAR)(res_win_t, tchar_t);
typedef void(__cdecl *PF_ON_SIZE)(res_win_t, int, const xsize_t*);
typedef void(__cdecl *PF_ON_MOVE)(res_win_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_SHOW)(res_win_t, bool_t);
typedef void(__cdecl *PF_ON_ACTIVATE)(res_win_t, int);
typedef void(__cdecl *PF_ON_PAINT)(res_win_t, res_ctx_t, const xrect_t*);
typedef void(__cdecl *PF_ON_ERASE)(res_win_t, res_ctx_t);
typedef void(__cdecl *PF_ON_ENABLE)(res_win_t, bool_t);
typedef void(__cdecl *PF_ON_SET_FOCUS)(res_win_t, res_win_t);
typedef void(__cdecl *PF_ON_KILL_FOCUS)(res_win_t, res_win_t);
typedef void(__cdecl *PF_ON_NOTICE)(res_win_t, NOTICE*);
typedef void(__cdecl *PF_ON_MENU_COMMAND)(res_win_t, int, int, void*);
typedef void(__cdecl *PF_ON_PARENT_COMMAND)(res_win_t, int, void*);
typedef void(__cdecl *PF_ON_CHILD_COMMAND)(res_win_t, int, void*);
typedef void(__cdecl *PF_ON_SELF_COMMAND)(res_win_t, int, void*);
typedef void(__cdecl *PF_ON_COMMAND_FIND)(res_win_t, str_find_t*);
typedef void(__cdecl *PF_ON_COMMAND_REPLACE)(res_win_t, str_replace_t*);
typedef void(__cdecl *PF_ON_SYSCMD_CLICK)(res_win_t, const xpoint_t*);
typedef void(__cdecl *PF_ON_TIMER)(res_win_t, long_ptr);
typedef void(__cdecl *PF_ON_COPY)(res_win_t);
typedef void(__cdecl *PF_ON_CUT)(res_win_t);
typedef void(__cdecl *PF_ON_PASTE)(res_win_t);
typedef void(__cdecl *PF_ON_UNDO)(res_win_t);

/*widget event*/
typedef struct _if_event_t{
	PF_ON_CLOSE			pf_on_close;
	PF_ON_CREATE		pf_on_create;
	PF_ON_DESTROY		pf_on_destroy;
	PF_ON_LBUTTON_DOWN	pf_on_lbutton_down;
	PF_ON_LBUTTON_UP	pf_on_lbutton_up;
	PF_ON_LBUTTON_DBCLICK	pf_on_lbutton_dbclick;
	PF_ON_RBUTTON_DOWN	pf_on_rbutton_down;
	PF_ON_RBUTTON_UP	pf_on_rbutton_up;
	PF_ON_MOUSE_MOVE	pf_on_mouse_move;
	PF_ON_MOUSE_HOVER	pf_on_mouse_hover;
	PF_ON_MOUSE_LEAVE	pf_on_mouse_leave;
	PF_ON_SCROLL		pf_on_scroll;
	PF_ON_KEYDOWN		pf_on_keydown;
	PF_ON_CHAR			pf_on_char;
	PF_ON_SIZE			pf_on_size;
	PF_ON_MOVE			pf_on_move;
	PF_ON_SHOW			pf_on_show;
	PF_ON_ACTIVATE		pf_on_activate;
	PF_ON_ENABLE		pf_on_enable;
	PF_ON_SET_FOCUS		pf_on_set_focus;
	PF_ON_KILL_FOCUS	pf_on_kill_focus;
	PF_ON_PAINT			pf_on_paint;
	PF_ON_ERASE			pf_on_erase;
	PF_ON_NOTICE		pf_on_notice;
	PF_ON_MENU_COMMAND		pf_on_menu_command;
	PF_ON_PARENT_COMMAND	pf_on_parent_command;
	PF_ON_CHILD_COMMAND		pf_on_child_command;
	PF_ON_SELF_COMMAND		pf_on_self_command;
	PF_ON_COMMAND_FIND		pf_on_command_find;
	PF_ON_COMMAND_REPLACE	pf_on_command_replace;

	PF_ON_SYSCMD_CLICK		pf_on_syscmd_click;

	PF_ON_TIMER			pf_on_timer;
	PF_ON_COPY			pf_on_copy;
	PF_ON_CUT			pf_on_cut;
	PF_ON_PASTE			pf_on_paste;
	PF_ON_UNDO			pf_on_undo;

	dword_t style;
	void* param;
}if_event_t;

/*widget interface*/
typedef void(__cdecl *PF_WIDGET_STARTUP)(int);
typedef void(__cdecl *PF_WIDGET_CLEANUP)(void);
typedef res_win_t(__cdecl *PF_WIDGET_CREATE)(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev);
typedef void(__cdecl *PF_WIDGET_DESTROY)(res_win_t);
typedef void(__cdecl *PF_WIDGET_CLOSE)(res_win_t, int);
typedef if_event_t* (__cdecl *PF_WIDGET_GET_DISPATCH)(res_win_t);
//typedef win_struct_t* (__cdecl *PF_WIDGET_GET_STRUCT)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_HAS_STRUCT)(res_win_t);
typedef void(__cdecl *PF_WIDGET_SET_STYLE)(res_win_t, dword_t);
typedef dword_t(__cdecl *PF_WIDGET_GET_STYLE)(res_win_t);
typedef void(__cdecl *PF_WIDGET_SET_USER_ID)(res_win_t, dword_t);
typedef dword_t(__cdecl *PF_WIDGET_GET_USER_ID)(res_win_t);
typedef void(__cdecl *PF_WIDGET_SET_USER_DELTA)(res_win_t, void*);
typedef void* (__cdecl *PF_WIDGET_GET_USER_DELTA)(res_win_t);
typedef void(__cdecl *PF_WIDGET_SET_USER_PROP)(res_win_t, const tchar_t*, void*);
typedef void* (__cdecl *PF_WIDGET_GET_USER_PROP)(res_win_t, const tchar_t*);
typedef void* (__cdecl *PF_WIDGET_DEL_USER_PROP)(res_win_t, const tchar_t*);
typedef void(__cdecl *PF_WIDGET_SET_USER_RESULT)(res_win_t, int);
typedef int(__cdecl *PF_WIDGET_GET_USER_RESULT)(res_win_t);
typedef res_ctx_t(__cdecl *PF_WIDGET_CLIENT_DC)(res_win_t);
typedef res_ctx_t(__cdecl *PF_WIDGET_WINDOW_DC)(res_win_t);
typedef void(__cdecl *PF_WIDGET_RELEASE_DC)(res_win_t, res_ctx_t);
typedef res_win_t(__cdecl *PF_WIDGET_GET_CHILD)(res_win_t, dword_t);
typedef res_win_t(__cdecl *PF_WIDGET_GET_PARENT)(res_win_t);
typedef void(__cdecl *PF_WIDGET_GET_CLIENT_RECT)(res_win_t, xrect_t*);
typedef void(__cdecl *PF_WIDGET_GET_WINDOW_RECT)(res_win_t, xrect_t*);
typedef void(__cdecl *PF_WIDGET_CLIENT_TO_SCREEN)(res_win_t, xpoint_t*);
typedef void(__cdecl *PF_WIDGET_SCREEN_TO_CLIENT)(res_win_t, xpoint_t*);
typedef void(__cdecl *PF_WIDGET_CLIENT_TO_WINDOW)(res_win_t, xpoint_t*);
typedef void(__cdecl *PF_WIDGET_WINDOW_TO_CLIENT)(res_win_t, xpoint_t*);
typedef void(__cdecl *PF_WIDGET_CENTER_WINDOW)(res_win_t, res_win_t);
typedef void(__cdecl *PF_WIDGET_SET_CURSOR)(res_win_t, int);
typedef void(__cdecl *PF_WIDGET_SET_CAPTURE)(res_win_t, bool_t);
typedef void(__cdecl *PF_WIDGET_SET_IMM)(res_win_t, bool_t);
typedef bool_t(__cdecl *PF_WIDGET_GET_IMM)(res_win_t);
typedef void(__cdecl *PF_WIDGET_SET_FOCUS)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_KEY_STATE)(res_win_t, byte_t);
typedef bool_t(__cdecl *PF_WIDGET_IS_VALID)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_IS_CHILD)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_IS_FOCUS)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_CAN_FOCUS)(res_win_t);
typedef void(__cdecl *PF_WIDGET_POST_CHAR)(res_win_t, tchar_t);
typedef void(__cdecl *PF_WIDGET_POST_KEY)(res_win_t, int);
typedef void(__cdecl *PF_WIDGET_POST_QUIT)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_CAN_CLOSE)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_CAN_SIZE)(res_win_t);
typedef void(__cdecl *PF_WIDGET_RESET_SCROLL)(res_win_t, bool_t);
typedef bool_t(__cdecl *PF_WIDGET_CAN_PAGING)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_HAS_BORDER)(res_win_t);
typedef void(__cdecl *PF_WIDGET_SIZE)(res_win_t, const xsize_t*);
typedef void(__cdecl *PF_WIDGET_MOVE)(res_win_t, const xpoint_t*);
typedef void(__cdecl *PF_WIDGET_TAKE)(res_win_t, int);
typedef void(__cdecl *PF_WIDGET_SHOW)(res_win_t, dword_t);
typedef void(__cdecl *PF_WIDGET_UPDATE_CLIENT)(res_win_t);
typedef void(__cdecl *PF_WIDGET_UPDATE_WINDOW)(res_win_t);
typedef void(__cdecl *PF_WIDGET_INVALID)(res_win_t, const xrect_t*, bool_t);
typedef void(__cdecl *PF_WIDGET_UPDATE)(res_win_t, const xrect_t*, bool_t);
typedef void(__cdecl *PF_WIDGET_ENABLE)(res_win_t, bool_t);
typedef void(__cdecl *PF_WIDGET_CREATE_CARET)(res_win_t, int, int);
typedef void(__cdecl *PF_WIDGET_DESTROY_CARET)(res_win_t);
typedef void(__cdecl *PF_WIDGET_SHOW_CARET)(res_win_t, int, int, bool_t);

typedef void(__cdecl *PF_WIDGET_SET_TITLE)(res_win_t, const tchar_t*);
typedef int(__cdecl *PF_WIDGET_GET_TITLE)(res_win_t, tchar_t*, int);
typedef void(__cdecl *PF_WIDGET_ADJUST_SIZE)(dword_t, xsize_t*);
typedef bool_t(__cdecl *PF_WIDGET_ENUM_CHILD)(res_win_t, PF_ENUM_WINDOW_PROC, long_ptr);
typedef void(__cdecl *PF_WIDGET_MENU_ITEM_RECT)(res_win_t, int, xrect_t*);

typedef bool_t(__cdecl *PF_WIDGET_IS_MAXIMIZED)(res_win_t);
typedef bool_t(__cdecl *PF_WIDGET_IS_MINIMIZED)(res_win_t);
typedef void(__cdecl *PF_WIDGET_SET_ALPHA)(res_win_t, byte_t);
typedef byte_t(__cdecl *PF_WIDGET_GET_ALPHA)(res_win_t);
typedef void(__cdecl *PF_WIDGET_TRACK_MOUSE)(res_win_t, dword_t);
typedef void(__cdecl *PF_WIDGET_SET_REGION)(res_win_t, res_rgn_t);

//typedef if_subproc_t* (__cdecl *PF_WIDGET_GET_SUBPROC)(res_win_t, dword_t);
//typedef bool_t(__cdecl *PF_WIDGET_SET_SUBPROC)(res_win_t, dword_t, if_subproc_t*);
typedef void(__cdecl *PF_WIDGET_DEL_SUBPROC)(res_win_t, dword_t);
typedef bool_t(__cdecl *PF_WIDGET_SET_SUBPROC_DELTA)(res_win_t, dword_t, void*);
typedef void* (__cdecl *PF_WIDGET_GET_SUBPROC_DELTA)(res_win_t, dword_t);
typedef bool_t(__cdecl *PF_WIDGET_HAS_SUBPROC)(res_win_t);

typedef void(__cdecl *PF_WIDGET_COPY)(res_win_t);
typedef void(__cdecl *PF_WIDGET_CUT)(res_win_t);
typedef void(__cdecl *PF_WIDGET_PASTE)(res_win_t);
typedef void(__cdecl *PF_WIDGET_UNDO)(res_win_t);

//typedef int(__cdecl *PF_WIDGET_SEND_NOTICE)(res_win_t, NOTICE*);
typedef void(__cdecl *PF_WIDGET_POST_COMMAND)(res_win_t, int, int, void*);
typedef int(__cdecl *PF_WIDGET_SEND_COMMAND)(res_win_t, int, int, void*);

typedef long_ptr(__cdecl *PF_WIDGET_SET_TIMER)(res_win_t, dword_t);
typedef void(__cdecl *PF_WIDGET_KILL_TIMER)(res_win_t, long_ptr);

typedef int(__cdecl *PF_WIDGET_DO_MODAL)(res_win_t);
typedef void(__cdecl *PF_WIDGET_DO_TRACE)(res_win_t);

typedef struct _if_widget_t{
	PF_WIDGET_STARTUP		pf_widget_startup;
	PF_WIDGET_CLEANUP		pf_widget_cleanup;

	PF_WIDGET_CREATE			pf_widget_create;
	PF_WIDGET_DESTROY			pf_widget_destroy;
	PF_WIDGET_CLOSE				pf_widget_close;
	PF_WIDGET_HAS_STRUCT		pf_widget_has_struct;
	//PF_WIDGET_GET_STRUCT		pf_widget_get_struct;
	PF_WIDGET_GET_DISPATCH		pf_widget_get_dispatch;
	PF_WIDGET_SET_STYLE			pf_widget_set_style;
	PF_WIDGET_GET_STYLE			pf_widget_get_style;
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
	PF_WIDGET_CAN_FOCUS			pf_widget_can_focus;
	PF_WIDGET_POST_CHAR			pf_widget_post_char;
	PF_WIDGET_POST_KEY			pf_widget_post_key;
	PF_WIDGET_POST_QUIT			pf_widget_post_quit;
	PF_WIDGET_CAN_CLOSE			pf_widget_can_close;
	PF_WIDGET_CAN_SIZE			pf_widget_can_size;
	PF_WIDGET_RESET_SCROLL		pf_widget_reset_scroll;
	PF_WIDGET_CAN_PAGING		pf_widget_can_paging;
	PF_WIDGET_HAS_BORDER		pf_widget_has_border;
	PF_WIDGET_SIZE				pf_widget_size;
	PF_WIDGET_MOVE				pf_widget_move;
	PF_WIDGET_TAKE				pf_widget_take;
	PF_WIDGET_SHOW				pf_widget_show;
	PF_WIDGET_UPDATE_CLIENT		pf_widget_update_client;
	PF_WIDGET_UPDATE_WINDOW		pf_widget_update_window;
	PF_WIDGET_INVALID			pf_widget_invalid;
	PF_WIDGET_UPDATE			pf_widget_update;
	PF_WIDGET_ENABLE			pf_widget_enable;
	PF_WIDGET_CREATE_CARET		pf_widget_create_caret;
	PF_WIDGET_DESTROY_CARET		pf_widget_destroy_caret;
	PF_WIDGET_SHOW_CARET		pf_widget_show_caret;
	PF_WIDGET_SET_TITLE			pf_widget_set_title;
	PF_WIDGET_GET_TITLE			pf_widget_get_title;
	PF_WIDGET_SET_TIMER			pf_widget_set_timer;
	PF_WIDGET_KILL_TIMER		pf_widget_kill_timer;
	PF_WIDGET_ADJUST_SIZE		pf_widget_adjust_size;
	PF_WIDGET_ENUM_CHILD		pf_widget_enum_child;

	PF_WIDGET_MENU_ITEM_RECT	pf_widget_menu_item_rect;

	PF_WIDGET_IS_MAXIMIZED		pf_widget_is_maximized;
	PF_WIDGET_IS_MINIMIZED		pf_widget_is_minimized;
	PF_WIDGET_SET_ALPHA			pf_widget_set_alpha;
	PF_WIDGET_GET_ALPHA			pf_widget_get_alpha;
	PF_WIDGET_TRACK_MOUSE		pf_widget_track_mouse;
	PF_WIDGET_SET_REGION		pf_widget_set_region;

	//PF_WIDGET_SET_SUBPROC		pf_widget_set_subproc;
	//PF_WIDGET_GET_SUBPROC		pf_widget_get_subproc;
	//PF_WIDGET_DEL_SUBPROC		pf_widget_del_subproc;
	//PF_WIDGET_SET_SUBPROC_DELTA		pf_widget_set_subproc_delta;
	//PF_WIDGET_GET_SUBPROC_DELTA		pf_widget_get_subproc_delta;
	//PF_WIDGET_HAS_SUBPROC		pf_widget_has_subproc;

	PF_WIDGET_COPY				pf_widget_copy;
	PF_WIDGET_CUT				pf_widget_cut;
	PF_WIDGET_PASTE				pf_widget_paste;
	PF_WIDGET_UNDO				pf_widget_undo;

	//PF_WIDGET_SEND_NOTICE		pf_widget_send_notice;
	PF_WIDGET_POST_COMMAND		pf_widget_post_command;
	PF_WIDGET_SEND_COMMAND		pf_widget_send_command;

	PF_WIDGET_DO_MODAL			pf_widget_do_modal;
	PF_WIDGET_DO_TRACE			pf_widget_do_trace;
}if_widget_t;


#endif	/* _XDKINIT_H */

