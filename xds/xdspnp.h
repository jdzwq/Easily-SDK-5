/***********************************************************************
	Easily xds v5.0

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pnp service document

	@module	xdspnp.h | pnp interface file

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

#ifndef _XDSPNP_H
#define _XDSPNP_H

#include "xdsdef.h"


typedef void(*PF_PNPS_DISPATCH)(unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size, void* param);

typedef struct _pnp_listen_t{
	short port;
	res_file_t so;

	int act;
	int res;
#ifdef XDK_SUPPORT_THREAD_QUEUE
	res_hand_t epo;
#endif

	bool_t is_thread;
	bool_t is_secu;
	void* pf_param;
	union
	{
		PF_PNPS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};
}pnp_listen_t;


#ifdef	__cplusplus
extern "C" {
#endif

	XDS_API pnp_listen_t*  xpnp_start_thread(unsigned short port, PF_PNPS_DISPATCH pf_dispatch, void* param);

	XDS_API pnp_listen_t*  xpnp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

	XDS_API void  xpnp_stop(pnp_listen_t* plis);

#ifdef	__cplusplus
}
#endif

typedef struct _pnps_block_t{
	int cbs;

	bool_t is_thread;

	unsigned short port;
	tchar_t addr[ADDR_LEN];

	dword_t size;
	byte_t pack[PNP_PKG_SIZE];

	file_t log;
	PF_LOG_TITLE	pf_log_title;
	PF_LOG_ERROR	pf_log_error;
	PF_LOG_DATA		pf_log_data;
	PF_LOG_XML		pf_log_xml;

	tchar_t path[PATH_LEN];
}pnps_block_t;

typedef enum{
	PNPS_INVOKE_SUCCEED = 0,
	PNPS_INVOKE_WITHINFO = 1,
	PNPS_INVOKE_WITHERROR = 2,
	PNPS_INVOKE_PENDING = 100
}PNPS_INVOKE_STATE;

typedef int(STDCALL *PF_PNPS_INVOKE)(const pnps_block_t* pb);

#endif /*_XDSPNP_H*/