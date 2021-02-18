/***********************************************************************
	Easily xdl v5.0

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc udp service document

	@module	srvudp.h | interface file

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

#ifndef _SRVUDP_H
#define _SRVUDP_H

#include "xdldef.h"


typedef void(*PF_UDPS_DISPATCH)(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size, void* param);

typedef struct _udp_listen_t{
	short port;
	res_file_t so;

	int act;

	int res;
	res_thread_t* thr;

	bool_t is_thread;
	bool_t is_secu;
	void* pf_param;
	union
	{
		PF_UDPS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};
}udp_listen_t;


#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xudp_start_thread: create udp thread service routing.
@INPUT unsigned short port: the service port.
@INPUT PF_UDPS_DISPATCH pf_dispatch: the callback service dispatch function.
@INPUT void* param: the user parameter transback into dispath function.
@RETURN udp_listen_t*: if succeeds return listen struct, fails return NULL.
*/
EXP_API udp_listen_t*  xudp_start_thread(unsigned short port, PF_UDPS_DISPATCH pf_dispatch, void* param);

/*
@FUNCTION xudp_start_process: create udp process service routing.
@INPUT unsigned short port: the service port.
@INPUT const tchar_t* sz_moudle: the process service moudle name.
@INPUT const tchar_t* sz_cmdline: the process execute command line.
@RETURN udp_listen_t*: if succeeds return listen struct, fails return NULL.
*/
EXP_API udp_listen_t*  xudp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

/*
@FUNCTION xudp_stop: stop the udp service routing.
@INPUT udp_listen_t* plis: the service listen struct.
@RETURN void: none.
*/
EXP_API void  xudp_stop(udp_listen_t* plis);

#ifdef	__cplusplus
}
#endif

typedef struct _udps_block_t{
	int cbs;

	bool_t is_thread;

	unsigned short port;
	tchar_t addr[ADDR_LEN + 1];

	dword_t size;
	byte_t pack[UDP_PKG_SIZE];

	dword_t timo;
	tchar_t site[RES_LEN + 1];
	tchar_t path[PATH_LEN + 1];

	const trace_interface* ptk;
}udps_block_t;

typedef enum{
	UDPS_INVOKE_SUCCEED = 0,
	UDPS_INVOKE_WITHINFO = 1,
	UDPS_INVOKE_WITHERROR = 2,
	UDPS_INVOKE_PENDING = 100
}UDPS_INVOKE_STATE;

typedef int(STDCALL *PF_UDPS_INVOKE)(const udps_block_t* pb);

#endif /*_SRVUDP_H*/