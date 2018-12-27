/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl linux definition document

	@module	_xdl_linux.h | linux definition interface file

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

#ifndef _XDK_LINUX_H
#define _XDK_LINUX_H

//#define XDK_SUPPORT_MEMO_HEAP
#define XDK_SUPPORT_MEMO_PAGE
#define XDK_SUPPORT_MEMO_GLOB
#define XDK_SUPPORT_MEMO_LOCAL
#define XDK_SUPPORT_MEMO_CACHE
#define XDK_SUPPORT_MEMO
#define XDK_SUPPORT_ERROR
#define XDK_SUPPORT_DATE
#define XDK_SUPPORT_MBCS
#define XDK_SUPPORT_THREAD_EVENT
#define XDK_SUPPORT_THREAD_CRITI
#define XDK_SUPPORT_THREAD_MUTEX
#define XDK_SUPPORT_THREAD_SEMAP
#define XDK_SUPPORT_THREAD_QUEUE
#define XDK_SUPPORT_THREAD
#define XDK_SUPPORT_PROCESS
#define XDK_SUPPORT_FILE_BLOCK
#define XDK_SUPPORT_FILE_FIND
#define XDK_SUPPORT_FILE
#define XDK_SUPPORT_SHARE
#define XDK_SUPPORT_PIPE
#define XDK_SUPPORT_CONS
#define XDK_SUPPORT_COMM
#define XDK_SUPPORT_SOCK
#define XDK_SUPPORT_TIMER
#define XDK_SUPPORT_SHELL

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <locale.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/epoll.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <termios.h>
#include <pthread.h> 

//linux

#if defined(_UNICODE) || defined(UNICODE)
#define _T(x)      L ## x
#else
#define _T(x)      x
#endif

#ifdef XDK_SUPPORT_SOCK
typedef struct sockaddr_in	net_addr_t;
typedef struct in_addr		sin_addr_t;
typedef void*				res_addr_t;
#endif

#ifdef XDK_SUPPORT_MEMO
#ifdef XDK_SUPPORT_MEMO_GLOB
typedef void*		res_glob_t;
#endif
#ifdef XDK_SUPPORT_MEMO_HEAP
typedef void*		res_heap_t;
#endif
#endif

#ifdef XDK_SUPPORT_FILE
typedef int         res_file_t;
#ifdef XDK_SUPPORT_FILE_FIND
typedef void*		res_find_t;
#endif
#endif

#ifdef XDK_SUPPORT_CONS
typedef void(*GNU_SIGNAL_HANDLER)(int sig);
#endif

#ifdef XDK_SUPPORT_THREAD
typedef unsigned long tls_key_t;

#ifdef XDK_SUPPORT_THREAD_EVENT
typedef void*		res_even_t;
#endif
#ifdef XDK_SUPPORT_THREAD_MUTEX
typedef void*		res_mutx_t;
#endif
#ifdef XDK_SUPPORT_THREAD_CRITI
typedef void*       res_crit_t;
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
typedef void*       res_sema_t;
#endif
typedef void*(*GNU_THREAD_PROC)(void* param);
#endif

#ifdef XDK_SUPPORT_PROCESS
typedef void*		res_modu_t;
#endif

#ifdef XDK_SUPPORT_TIMER
typedef void*		res_queue_t;
typedef void*		res_timer_t;
#endif

typedef int         res_hand_t;

#define INVALID_FILE	((int)(-1))

#ifdef XDK_SUPPORT_COMM
#define COMM_EVNET_RING		TIOCM_RNG
#define COMM_EVNET_RLSD		TIOCM_LE
#define COMM_EVNET_CTS		TIOCM_CTS
#define COMM_EVNET_DSR		TIOCM_DSR
#define COMM_EVNET_RXCHAR	0x1000
#define COMM_EVNET_TXEMPTY	0x2000
#define COMM_EVNET_BREAK	0x4000
#define COMM_EVNET_ERROR	-1
#define COMM_EVENT_IDLE		0x8000

#define COMM_ERROR_BREAK	-1
#define COMM_ERROR_DNS		-2
#define COMM_ERROR_FRAME	-3
#define COMM_ERROR_IOE		-4
#define COMM_ERROR_MODE		-5
#define COMM_ERROR_OOP		-6
#define COMM_ERROR_OVERRUN	-7
#define COMM_ERROR_PTO		-8
#define COMM_ERROR_RXOVER	-9
#define COMM_ERROR_RXPARITY	-10
#define COMM_ERROR_TXFULL	-11

#define COMM_STATUS_CTS_ON	0x0001
#define COMM_STATUS_DSR_ON	0x0002
#define COMM_STATUS_RING_ON	0x0004
#define COMM_STATUS_RLSD_ON	0x0008

#endif

#ifdef XDK_SUPPORT_SOCK
/*socket event*/
#ifndef FD_READ
#define FD_READ			0x00000001
#endif

#ifndef FD_WRITE
#define FD_WRITE		0x00000002
#endif

#ifndef FD_ACCEPT
#define FD_ACCEPT		0x00000004
#endif
#endif /*XDK_SUPPORT_SOCK*/

#define MAX_EVENT  3

typedef struct OVERLAPPED{
    union{
        struct timeval tv;
        struct timespec tp;
    };
    
    union{
        fd_set fd[MAX_EVENT];
        struct epoll_event ev;
    };
}OVERLAPPED, *LPOVERLAPPED;

#ifndef LPSIZE
typedef size_t*     LPSIZE;
#endif

#ifdef _UNICODE
#define _tstrcmp	wcscmp
#define _tstrncmp	wcsncmp
#define _tsprintf	swprintf
#define _tstrcat	wcscat
#define _tstrncat	wcsncat
#define _tstrcpy	wcscpy
#define _tstrstr	wcsstr
#define _tstrncpy	wcsncpy
#define _tstrlen	wcslen
#define _tstrtol	wcstol
#define _tstrtof	wcstof
#define _tstrnull(s)		(s == NULL || s[0] == L'\0')
#define _tprintf    wprintf
#else
#define _tstrcmp	strcmp
#define _tstrncmp	strncmp
#define _tstricmp	strcmp
#define _tstrnicmp	strncmp
#define _tsprintf	sprintf
#define _tstrcat	stecat
#define _tstrncat	strncat
#define _tstrcpy	strcpy
#define _tstrstr	strstr
#define _tstrncpy	strncpy
#define _tstrlen	strlen
#define _tstrtol	atol
#define _tstrtof	atof
#define _tstrnull(s)		(s == NULL || s[0] == '\0')
#define _tprintf    printf
#endif

#ifndef min
#define min(x, y) ({                            \
         __typeof__(x) _min1 = (x);                  \
         __typeof__(y) _min2 = (y);                  \
         (void) (&_min1 == &_min2);              \
         _min1 < _min2 ? _min1 : _min2; })
#endif

#ifndef max
#define max(x, y) ({                            \
         __typeof__(x) _max1 = (x);                  \
         __typeof__(y) _max2 = (y);                  \
         (void) (&_max1 == &_max2);              \
         _max1 > _max2 ? _max1 : _max2; })
#endif

#endif //_XDK_LINUX_H