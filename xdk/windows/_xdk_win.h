/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl windows definition document

	@module	_xdl_win.h | windows interface file

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

#ifndef _XDK_WIN_H
#define _XDK_WIN_H

#define XDK_SUPPORT_MEMO_HEAP
#define XDK_SUPPORT_MEMO_PAGE
#define XDK_SUPPORT_MEMO_GLOB
#define XDK_SUPPORT_MEMO_LOCAL
#define XDK_SUPPORT_MEMO_CACHE
#define XDK_SUPPORT_MEMO
#define XDK_SUPPORT_ERROR
#define XDK_SUPPORT_DATE
#define XDK_SUPPORT_MBCS
#define XDK_SUPPORT_ASYNC
#define XDK_SUPPORT_THREAD_EVENT
#define XDK_SUPPORT_THREAD_CRITI
#define XDK_SUPPORT_THREAD_MUTEX
#define XDK_SUPPORT_THREAD_SEMAP
#define XDK_SUPPORT_THREAD_QUEUE
#define XDK_SUPPORT_THREAD
#define XDK_SUPPORT_PROCESS
#define XDK_SUPPORT_FILE_FIND
#define XDK_SUPPORT_FILE
#define XDK_SUPPORT_SHARE
#define XDK_SUPPORT_PIPE
#define XDK_SUPPORT_CONS
#define XDK_SUPPORT_COMM
#define XDK_SUPPORT_SOCK
#define XDK_SUPPORT_TIMER

#ifndef _WIN64
#define XDK_SUPPORT_BLUT
#endif

#if defined(WINCE)
#undef XDK_SUPPORT_THREAD_MUTEX
#undef XDK_SUPPORT_MEMO_PAGE
#undef XDK_SUPPORT_ACP
#undef XDK_SUPPORT_PIPE
#undef XDK_SUPPORT_CONS
#undef XDK_SUPPORT_SHARE
#undef XDK_SUPPORT_MEMO_CACHE
#undef XDK_SUPPORT_PROCESS
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT    0x0600

#ifdef WINVER
#undef WINVER
#endif

#if defined(WINCE) 
#define WINVER			_WIN32_WCE 
#else
#define WINVER			0x0501
#endif

#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <time.h>

#ifdef XDK_SUPPORT_SOCK
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif
#include <windows.h>
//#include <WinSock.h>
#include <windowsx.h>

#ifdef XDK_SUPPORT_BLUT
#include <BluetoothAPIs.h>
#include <Ws2bth.h>
#include <KS.H>
#endif

//#if defined(_UNICODE) || defined(UNICODE)
//#define _T(x)      L ## x
//#else
//#define _T(x)      x
//#endif

#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )

typedef HANDLE		res_queue_t;

#ifdef XDK_SUPPORT_SOCK
typedef struct sockaddr_in	net_addr_t;
typedef struct in_addr		sin_addr_t;
typedef SOCKADDR*	res_addr_t;
#endif

#ifdef XDK_SUPPORT_MEMO
#ifdef XDK_SUPPORT_MEMO_GLOB
typedef HGLOBAL		res_glob_t;
#endif
#ifdef XDK_SUPPORT_MEMO_HEAP
typedef HANDLE		res_heap_t;
#endif
#endif

#ifdef XDK_SUPPORT_FILE
typedef HANDLE		res_file_t;
#ifdef XDK_SUPPORT_FILE_FIND
typedef HANDLE		res_find_t;
#endif
#endif

#ifdef XDK_SUPPORT_CONS
typedef BOOL(WINAPI *WIN_SIGNAL_HANDLER)(DWORD sig);
#endif

#ifdef XDK_SUPPORT_THREAD
typedef HANDLE		res_thread_t;
typedef DWORD		tls_key_t;
#ifdef XDK_SUPPORT_THREAD_EVENT
typedef HANDLE		res_even_t;
#endif
#ifdef XDK_SUPPORT_THREAD_MUTEX
typedef HANDLE		res_mutx_t;
#endif
#ifdef XDK_SUPPORT_THREAD_CRITI
typedef CRITICAL_SECTION* res_crit_t;
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
typedef HANDLE		res_sema_t;
#endif

typedef unsigned (__stdcall *WIN_THREAD_PROC)(void* param);
#endif

#ifdef XDK_SUPPORT_PROCESS
typedef HMODULE		res_modu_t;
typedef HANDLE		res_proc_t;
#endif

#ifdef XDK_SUPPORT_TIMER
typedef HANDLE		res_timer_t;
typedef void(__stdcall *WIN_TIMER_PROC)(void* param, unsigned char wait);
#endif

#define INVALID_FILE	((HANDLE)(LONG_PTR)-1)

#ifndef pid_t
typedef unsigned int	pid_t;
#endif

#ifdef XDK_SUPPORT_COMM
#define COMM_EVNET_RING		EV_RING
#define COMM_EVNET_RLSD		EV_RLSD
#define COMM_EVNET_CTS		EV_CTS
#define COMM_EVNET_DSR		EV_DSR
#define COMM_EVNET_RXCHAR	EV_RXCHAR
#define COMM_EVNET_TXEMPTY	EV_TXEMPTY
#define COMM_EVNET_BREAK	EV_BREAK
#define COMM_EVNET_ERROR	EV_ERR
#define COMM_EVENT_IDLE		0x8000

#define COMM_ERROR_BREAK	CE_BREAK
#define COMM_ERROR_DNS		CE_DNS
#define COMM_ERROR_FRAME	CE_FRAME
#define COMM_ERROR_IOE		CE_IOE
#define COMM_ERROR_MODE		CE_MODE
#define COMM_ERROR_OOP		CE_OOP
#define COMM_ERROR_OVERRUN	CE_OVERRUN
#define COMM_ERROR_PTO		CE_PTO
#define COMM_ERROR_RXOVER	CE_RXOVER
#define COMM_ERROR_RXPARITY	CE_RXPARITY
#define COMM_ERROR_TXFULL	CE_TXFULL

#define COMM_STATUS_CTS_ON	MS_CTS_ON
#define COMM_STATUS_DSR_ON	MS_DSR_ON
#define COMM_STATUS_RING_ON	MS_RING_ON
#define COMM_STATUS_RLSD_ON	MS_RLSD_ON

#endif

#ifdef XDK_SUPPORT_SOCK
/*socket event*/
#ifndef FD_READ
#define FD_READ			(1 << 0)
#endif

#ifndef FD_WRITE
#define FD_WRITE		(1 << 1)
#endif

#ifndef FD_ACCEPT
#define FD_ACCEPT		(1 << 3)
#endif

#ifndef FD_CONNECT
#define FD_CONNECT		(1 << 4)
#endif

#ifndef FD_CLOSE
#define FD_CLOSE		(1 << 5)
#endif
#endif /*XDK_SUPPORT_SOCK*/


#endif //_XDK_WIN_H