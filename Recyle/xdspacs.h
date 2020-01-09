/***********************************************************************
	Easily xds v5.0

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pacss service document

	@module	xdsinet.h | pacss service interface file

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

#ifndef _XDSPACS_H
#define _XDSPACS_H

#include "xdsdef.h"
#include "xdstcp.h"
#include "xdsssl.h"
#include "xdstls.h"

typedef void(*PF_PACSS_DISPATCH)(xhand_t pacs, void* param);

typedef struct _pacs_listen_t{
	int secu;
	union{
		tcp_listen_t* lis_tcp;
		ssl_listen_t* lis_ssl;
		tls_listen_t* lis_tls;
	};
	
	void *pf_param;
	PF_PACSS_DISPATCH pf_dispatch;
}pacs_listen_t;

#ifdef	__cplusplus
extern "C" {
#endif

	XDS_API pacs_listen_t*	xpacs_start_thread(int secu, unsigned short port, PF_PACSS_DISPATCH pf_dispatch, void* pf_param);

	XDS_API pacs_listen_t*	xpacs_start_process(int secu, unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

	XDS_API void xpacs_stop(pacs_listen_t* plis);

#ifdef	__cplusplus
}
#endif

typedef struct _pacss_block_t{
	int cbs;

	bool_t is_thread;
	xhand_t pacs;

	file_t log;
	PF_LOG_TITLE	pf_log_title;
	PF_LOG_ERROR	pf_log_error;
	PF_LOG_DATA		pf_log_data;
	PF_LOG_XML		pf_log_xml;

	tchar_t method[NUM_LEN];
	tchar_t site[RES_LEN];
	tchar_t path[PATH_LEN];

#ifdef _OS_64
	unsigned long long	wrecv;
#else
	unsigned long		wrecv;
#endif
	void*	precv;

}pacss_block_t;

typedef enum{
	PACSS_INVOKE_SUCCEED = 0,
	PACSS_INVOKE_WITHINFO = 1,
	PACSS_INVOKE_WITHERROR = 2,
	PACSS_INVOKE_PENDING = 100
}PACSS_INVOKE_STATE;

typedef int(STDCALL *PF_PACSS_INVOKE)(const pacss_block_t* pb);

#endif /*_XDPACS_H*/