/***********************************************************************
	Easily xdl v5.0

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc slots service document

	@module	srvslots.h | interface file

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

#ifndef _SRVSLOT_H
#define _SRVSLOT_H

#include "xdldef.h"
#include "srvtcp.h"
#include "srvssl.h"
#include "srvssh.h"

typedef void(*PF_SLOTS_DISPATCH)(xhand_t slot, void* param);

typedef struct _slot_listen_t{
	int secu;
	union{
		tcp_listen_t* lis_tcp;
		ssl_listen_t* lis_ssl;
		ssh_listen_t* lis_ssh;
	};
	
	void *pf_param;
	PF_SLOTS_DISPATCH pf_dispatch;
}slot_listen_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xslot_start_thread: create slot thread service routing.
@INPUT int secu: the security type, it can be _SECU_TCP, _SECU_SSL.
@INPUT unsigned short port: the service port.
@INPUT PF_SLOTS_DISPATCH pf_dispatch: the callback service dispatch function.
@INPUT void* param: the user parameter transback into dispath function.
@RETURN slot_listen_t*: if succeeds return listen struct, fails return NULL.
*/
EXP_API slot_listen_t*	xslot_start_thread(int secu, unsigned short port, PF_SLOTS_DISPATCH pf_dispatch, void* pf_param);

/*
@FUNCTION xslot_start_process: create slot process service routing.
@INPUT int secu: the security type, it can be _SECU_TCP, _SECU_SSL.
@INPUT unsigned short port: the service port.
@INPUT const tchar_t* sz_moudle: the process service moudle name.
@INPUT const tchar_t* sz_cmdline: the process execute command line.
@RETURN slot_listen_t*: if succeeds return listen struct, fails return NULL.
*/
EXP_API slot_listen_t*	xslot_start_process(int secu, unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

/*
@FUNCTION xslot_stop: stop the slot service routing.
@INPUT slot_listen_t* plis: the service listen struct.
@RETURN void: none.
*/
EXP_API void xslot_stop(slot_listen_t* plis);

#ifdef	__cplusplus
}
#endif

typedef struct _slots_block_t{
	int cbs;

	bool_t is_thread;
	xhand_t slot;

	tchar_t site[RES_LEN + 1];
	tchar_t path[PATH_LEN + 1];

	void* hand;
	PF_TRACK_ERROR pf_track_eror;
}slots_block_t;

typedef enum{
	SLOTS_INVOKE_SUCCEED = 0,
	SLOTS_INVOKE_WITHINFO = 1,
	SLOTS_INVOKE_WITHERROR = 2,
	SLOTS_INVOKE_PENDING = 100
}SLOTS_INVOKE_STATE;

typedef int(STDCALL *PF_SLOTS_INVOKE)(const slots_block_t* pb);

#endif /*_XDSLOT_H*/