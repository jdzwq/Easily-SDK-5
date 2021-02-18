/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tcp service document

	@module	srvtcp.h | interface file

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

#ifndef _SRVTCP_H
#define _SRVTCP_H

#include "xdldef.h"

typedef void(*PF_TCPS_DISPATCH)(xhand_t tcp, void* param);

typedef struct _tcp_listen_t{
	res_file_t so;

	int act;
	
	res_crit_t cri;

	int res;
	res_thread_t* thr;

	bool_t is_thread;
	void* pf_param;
	union
	{
		PF_TCPS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};
	
}tcp_listen_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xtcp_start_thread: create tcp thread service routing.
@INPUT unsigned short port: the service port.
@INPUT PF_TCPS_DISPATCH pf_dispatch: the callback service dispatch function.
@INPUT void* param: the user parameter transback into dispath function.
@RETURN tcp_listen_t*: if succeeds return listen struct, fails return NULL.
*/
EXP_API tcp_listen_t*  xtcp_start_thread(unsigned short port, PF_TCPS_DISPATCH pf_dispatch, void* param);

/*
@FUNCTION xtcp_start_process: create tcp process service routing.
@INPUT unsigned short port: the service port.
@INPUT const tchar_t* sz_moudle: the process service moudle name.
@INPUT const tchar_t* sz_cmdline: the process execute command line.
@RETURN tcp_listen_t*: if succeeds return listen struct, fails return NULL.
*/
EXP_API tcp_listen_t*  xtcp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

/*
@FUNCTION xtcp_stop: stop the tcp service routing.
@INPUT tcp_listen_t* plis: the service listen struct.
@RETURN void: none.
*/
EXP_API void  xtcp_stop(tcp_listen_t* plis);

#ifdef	__cplusplus
}
#endif

typedef struct _tcps_block_t{
	int cbs;

	bool_t is_thread;
	xhand_t tcp;

	dword_t timo;
	tchar_t site[RES_LEN + 1];
	tchar_t path[PATH_LEN + 1];

	const trace_interface* ptk;
}tcps_block_t;

typedef enum{
	TCPS_INVOKE_SUCCEED = 0,
	TCPS_INVOKE_WITHINFO = 1,
	TCPS_INVOKE_WITHERROR = 2,
	TCPS_INVOKE_PENDING = 100
}TCPS_INVOKE_STATE;

typedef int(STDCALL *PF_TCPS_INVOKE)(const tcps_block_t* pb);


#endif /*_SRVTCP_H*/