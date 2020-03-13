/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	xdkdef.h | interface file

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


#ifndef _XDKDEF_H
#define	_XDKDEF_H

#include <xdp.h>

#if defined(_OS_WINDOWS)
#include "windows/_xdk_win.h"
#elif defined(_OS_MACOS)
#include "macos/_xdk_macos.h"
#elif defined(_OS_LINUX)
#include "linux/_xdk_linux.h"
#endif


#ifndef INVALID_SIZE
#define INVALID_SIZE	((size_t)-1)
#endif


#define ZERO_WIDTH				(float)0
#define ZERO_HEIGHT				(float)0

#define FILE_OPEN_READ		0x00000000
#define FILE_OPEN_WRITE		0x00000001
#define FILE_OPEN_CREATE	0x00000002
#define FILE_OPEN_APPEND	0x00000004
#define FILE_OPEN_OVERLAP	0x00001000
#define FILE_OPEN_RANDOM	0x00002000

/*thread function*/
#if defined(_OS_WINDOWS)
#define PF_THREADFUNC	WIN_THREAD_PROC
#elif defined(_OS_MACOS)
#define PF_THREADFUNC	MAC_THREAD_PROC
#elif defined(_OS_LINUX)
#define PF_THREADFUNC	GNU_THREAD_PROC
#endif

/*signal handler*/
#if defined(_OS_WINDOWS)
#define PF_SIGHANDLER	WIN_SIGNAL_HANDLER
#elif defined(_OS_MACOS)
#define PF_SIGHANDLER	MAC_SIGNAL_HANDLER
#elif defined(_OS_LINUX)
#define PF_SIGHANDLER	GNU_SIGNAL_HANDLER
#endif

/*timer function*/
#if defined(_OS_WINDOWS)
#define PF_TIMERFUNC	WIN_TIMER_PROC
#elif defined(_OS_MACOS)
#define PF_TIMERFUNC	MAC_TIMER_PROC
#elif defined(_OS_LINUX)
#define PF_TIMERFUNC	GNU_TIMER_PROC
#endif

typedef struct async_t{
	int type;		/*the async type, can be ASYNC_BLOCK, ASYNC_EVENT, ASYNC_QUEUE*/
	dword_t timo;		/*the timeout value in millisecond*/
	dword_t size;	/*async operation data bytes*/

	void* lapp;		/*inner overlapped struct*/
#ifdef XDK_SUPPORT_THREAD_QUEUE
	res_queue_t port;	/*inner port resource handle*/
#endif
}async_t;

/*async type*/
#define ASYNC_BLOCK		0
#define ASYNC_EVENT		1
#define ASYNC_QUEUE		2

/*define process share mode*/
#define SHARE_NONE		0
#define SHARE_PIPE		1
#define SHARE_SOCK		2


typedef struct _xdate_t{
	unsigned short year;
	unsigned short mon;
	unsigned short day;
	unsigned short hour;
	unsigned short min;
	unsigned short sec;
	unsigned short millsec;

	unsigned short wday;
}xdate_t;

#define PROCESSOR_TYPE_386		386
#define PROCESSOR_TYPE_486		486
#define PROCESSOR_TYPE_PENTIUM	586
#define PROCESSOR_TYPE_IA64		2200
#define PROCESSOR_TYPE_X8664	8664

typedef struct _sys_info_t{
	tchar_t architecture[INT_LEN];
	int processor_type;
	int processor_number;
	int page_size;
	int page_gran;
}sys_info_t;

typedef struct _proc_info_t{
	res_proc_t process_handle;
	res_thread_t thread_handle;

	pid_t process_id;
	pid_t thread_id;

	res_file_t pip_read;
	res_file_t pip_write;
	res_file_t std_read;
	res_file_t std_write;
}proc_info_t;

typedef struct _file_info_t {
	bool_t	is_dir;

	union{
		struct{
			dword_t low_size;
			dword_t high_size;
		};
		lword_t file_size;
	};

	xdate_t create_time;
	xdate_t access_time;
	xdate_t write_time;

	tchar_t file_etag[ETAG_LEN];
	tchar_t file_name[META_LEN];
}file_info_t;

typedef struct _dev_com_t{
	short baud_rate;	/*the device baud rate, eg:9600, 14400, 19200...*/
	short parity;		/*parity checking, eg: 1-ODDPARITY, 2-EVENPARITY*/
	short byte_size;	/*numbers of bits in a byte, eg: 8*/
	short stop_bits;	/*numbers of stop bits used, eg: 0-ONESTOPBIT, 1-ONE5STOPBIT, 2-TWOSTOPBIT*/
	short timeout;		/*timeout value*/
	tchar_t devname[INT_LEN];
}dev_com_t;


#endif	/* _XDKDEF_H */

