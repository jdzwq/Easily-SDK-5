﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl initialize document

	@module	xdlinit.h | interface file

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

#ifndef _XDLINIT_H
#define	_XDLINIT_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_MEMO_DUMP
#include "dblink.h"
#endif

#define ERR_BUFF_SIZE		1024
#define ERR_ITEM_COUNT		4

#ifdef	__cplusplus
extern "C" {
#endif

	typedef struct _if_jump_t{
		jmp_buf*	if_buf;		//thread int jmp buffer
		int			if_size;	//jmp buffer array size
		int			if_index;  // top jmp buffer index
	}if_jump_t;

	typedef struct _if_dump_t{
		byte_t*		err_buf; //thread error buffer
		int			err_index; //error item count

		PF_TRACK_ERROR err_track; //error track callback function
		void* err_param; //error track callback param
	}if_dump_t;

	typedef struct _if_zone_t{
#ifdef XDK_SUPPORT_MEMO_HEAP
		res_heap_t	if_heap;
#endif
#ifdef XDL_SUPPORT_MEMO_DUMP
		link_t		if_dump;
		link_t		if_trak;
#endif
		vword_t	resv;
	}if_zone_t;

	typedef struct _xdl_mou_t{
		bool_t		if_ok;
		dword_t		if_opt;

		/*big Endian*/
		//bool_t		if_big;

#ifdef XDK_SUPPORT_MEMO
		if_memo_t if_memo;
#endif

#ifdef XDK_SUPPORT_THREAD
		/*master thread id*/
		pid_t		thread_id;
		/*thread local storage index*/
		tls_key_t	tls_thr_zero;
		tls_key_t	tls_thr_zone;
		tls_key_t	tls_thr_jump;
		tls_key_t	tls_thr_dump;
#else
		if_zone_t* pif_zone;
		if_jump_t* pif_jump;
		if_dump_t* pif_dump;
#endif /*XDK_SUPPORT_THREAD*/

#ifdef XDK_SUPPORT_ERROR
		if_error_t	if_error;
#endif

#ifdef XDK_SUPPORT_MBCS
		if_mbcs_t if_mbcs;
#endif 

#ifdef XDK_SUPPORT_ASYNC
		if_async_t if_async;
#endif 

#ifdef XDK_SUPPORT_DATE
		if_date_t	if_date;
#endif

#ifdef XDK_SUPPORT_THREAD
		if_thread_t if_thread;
#endif

#ifdef XDK_SUPPORT_TIMER
		if_timer_t if_timer;
#endif

#ifdef XDK_SUPPORT_SOCK
		if_socket_t	if_socket;
#endif

#ifdef XDK_SUPPORT_PIPE
		if_pipe_t	if_pipe;
#endif

#ifdef XDK_SUPPORT_SHARE
		if_share_t	if_share;
#endif

#ifdef XDK_SUPPORT_FILE
		if_file_t	if_file;
#endif

#ifdef XDK_SUPPORT_COMM
		if_comm_t	if_comm;
#endif

#ifdef XDK_SUPPORT_CONS
		if_cons_t	if_cons;
#endif

#ifdef XDK_SUPPORT_PROCESS
		if_process_t	if_process;
#endif

#ifdef XDL_SUPPORT_MEMO_DUMP
		/*for thread trace*/
		res_crit_t	dump_crit;
		link_t		dump_link;
#endif

	}xdl_mou_t;

	extern xdl_mou_t g_xdl_mou;

#ifdef XDK_SUPPORT_THREAD
#define THREAD_ZONE_INTERFACE ((g_xdl_mou.tls_thr_zone)? (if_zone_t*)(*(g_xdl_mou.if_thread.pf_thread_get_tls))(g_xdl_mou.tls_thr_zone) : NULL)
#define THREAD_JUMP_INTERFACE ((g_xdl_mou.tls_thr_jump)? (if_jump_t*)(*(g_xdl_mou.if_thread.pf_thread_get_tls))(g_xdl_mou.tls_thr_jump) : NULL)
#define THREAD_DUMP_INTERFACE ((g_xdl_mou.tls_thr_dump)? (if_dump_t*)(*(g_xdl_mou.if_thread.pf_thread_get_tls))(g_xdl_mou.tls_thr_dump) : NULL)
#else
#define THREAD_ZONE_INTERFACE (g_xdl_mou.pif_zone)
#define THREAD_JUMP_INTERFACE (g_xdl_mou.pif_jump)
#define THREAD_DUMP_INTERFACE (g_xdl_mou.pif_dump)
#endif

	EXP_API jmp_buf*	thread_jump_buff(void);

#define XDL_INITIALIZE_DRIVER			0x00000001
#define XDL_INITIALIZE_SERVER			0x00000002
#define XDL_INITIALIZE_CONSOLE			0x00000004

#define XDL_APARTMENT_PROCESS			0x00000000
#ifdef XDK_SUPPORT_THREAD
#define XDL_APARTMENT_THREAD			0x00010000
#endif

#define XDL_MOUNTED					((g_xdl_mou.if_ok)? 1 : 0)
//#define XDL_LITTLE					((g_xdl_mou.if_big)? 0 : 1)

#ifdef XDK_SUPPORT_MEMO
#define PROCESS_MEMO_INTERFACE		((g_xdl_mou.if_ok)? (if_memo_t*)(&g_xdl_mou.if_memo): NULL)
#endif

#ifdef XDK_SUPPORT_ERROR
#define PROCESS_ERROR_INTERFACE		((g_xdl_mou.if_ok)? (if_error_t*)(&g_xdl_mou.if_error) : NULL)
#endif

#ifdef XDK_SUPPORT_MBCS
#define PROCESS_MBCS_INTERFACE		((g_xdl_mou.if_ok)? (if_mbcs_t*)(&g_xdl_mou.if_mbcs): NULL)
#endif

#ifdef XDK_SUPPORT_ASYNC
#define PROCESS_ASYNC_INTERFACE		((g_xdl_mou.if_ok)? (if_async_t*)(&g_xdl_mou.if_async): NULL)
#endif

#ifdef XDK_SUPPORT_PROCESS
#define PROCESS_PROCESS_INTERFACE	((g_xdl_mou.if_ok)? (if_process_t*)(&g_xdl_mou.if_process): NULL)
#endif

#ifdef XDK_SUPPORT_THREAD
#define PROCESS_THREAD_INTERFACE	((g_xdl_mou.if_ok)? (if_thread_t*)(&g_xdl_mou.if_thread): NULL)
#endif

#ifdef XDK_SUPPORT_TIMER
#define PROCESS_TIMER_INTERFACE		((g_xdl_mou.if_ok)? (if_timer_t*)(&g_xdl_mou.if_timer): NULL)
#endif

#ifdef XDK_SUPPORT_DATE
#define PROCESS_DATE_INTERFACE		((g_xdl_mou.if_ok)? (if_date_t*)(&g_xdl_mou.if_date): NULL)
#endif

#ifdef XDK_SUPPORT_SOCK
#define PROCESS_SOCKET_INTERFACE	((g_xdl_mou.if_ok)? (if_socket_t*)(&g_xdl_mou.if_socket): NULL)
#endif

#ifdef XDK_SUPPORT_FILE
#define PROCESS_FILE_INTERFACE		((g_xdl_mou.if_ok)? (if_file_t*)(&g_xdl_mou.if_file): NULL)
#endif

#ifdef XDK_SUPPORT_SHARE
#define PROCESS_SHARE_INTERFACE		((g_xdl_mou.if_ok)? (if_share_t*)(&g_xdl_mou.if_share): NULL)
#endif

#ifdef XDK_SUPPORT_PIPE
#define PROCESS_PIPE_INTERFACE		((g_xdl_mou.if_ok)? (if_pipe_t*)(&g_xdl_mou.if_pipe): NULL)
#endif

#ifdef XDK_SUPPORT_COMM
#define PROCESS_COMM_INTERFACE		((g_xdl_mou.if_ok)? (if_comm_t*)(&g_xdl_mou.if_comm): NULL)
#endif

#ifdef XDK_SUPPORT_CONS
#define PROCESS_CONS_INTERFACE		((g_xdl_mou.if_ok)? (if_cons_t*)(&g_xdl_mou.if_cons): NULL)
#endif


	EXP_API	void	xdl_process_init(dword_t opt);

	EXP_API void	xdl_process_uninit();

	EXP_API void	xdl_process_clean();

#ifdef XDK_SUPPORT_THREAD

	EXP_API	void	xdl_thread_init(int master);

	EXP_API void	xdl_thread_uninit(int error);

	EXP_API void	xmem_dump();

	EXP_API void	thread_dump();

#endif /*XDK_SUPPORT_THREAD*/

#ifdef	__cplusplus
}
#endif

#endif	/* _XDLINIT_H */

