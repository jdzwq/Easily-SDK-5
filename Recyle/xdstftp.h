/***********************************************************************
	Easily xds v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tftps service document

	@module	xdstftp.h | tftps service interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#ifndef _XDSTFTP_H
#define _XDSTFTP_H

#include "xdsdef.h"
#include "xdsudp.h"

typedef void(*PF_TFTP_DISPATCH)(xhand_t tftp, void* param);

typedef struct _tftp_listen_t{
	udp_listen_t* lis_udp;

	void *pf_param;
	PF_TFTP_DISPATCH pf_dispatch;
}tftp_listen_t;

#ifdef	__cplusplus
extern "C" {
#endif

	XDS_API tftp_listen_t*	xtftp_start_thread(unsigned short port, PF_TFTP_DISPATCH pf_dispatch, void* pf_param);

	XDS_API tftp_listen_t*	xtftp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

	XDS_API void xtftp_stop(tftp_listen_t* plis);

	XDS_API void xtftp_dispatch(unsigned short port, const tchar_t* addr, const byte_t* pack, PF_TFTP_DISPATCH pf_dispatch, void* pf_param);


#ifdef	__cplusplus
}
#endif

typedef struct _tftps_block_t{
	int cbs;

	bool_t is_thread;
	xhand_t tftp;

	object_t log;
	PF_LOG_TITLE	pf_write_log_title;
	PF_LOG_ERROR	pf_write_log_error;
	PF_LOG_DATA	pf_write_log_data;
	PF_LOG_XML	pf_write_log_xml;

	tchar_t ctxid[NUM_LEN];
	tchar_t site[RES_LEN];
	tchar_t space[PATH_LEN];
	tchar_t path[PATH_LEN];
	tchar_t file[PATH_LEN];
	tchar_t param[PATH_LEN];

#ifdef _OS_64
	unsigned long long	wrecv;
#else
	unsigned long		wrecv;
#endif
	void*	precv;

}tftps_block_t;

typedef enum{
	TFTPS_INVOKE_SUCCEED = 0,
	TFTPS_INVOKE_WITHINFO = 1,
	TFTPS_INVOKE_WITHERROR = 2,
	TFTPS_INVOKE_PENDING = 100
}TFTPS_INVOKE_STATE;

typedef int(STDCALL *PF_TFTPS_INVOKE)(const tftps_block_t* pb);


#endif /*_XDSTFTP_H*/