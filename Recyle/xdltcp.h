/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl tcp document

	@module	xdltcp.h | xdl http interface file

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

#ifndef _XDLTCP_H
#define _XDLTCP_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_SOCK

typedef enum{
	_XTCP_TYPE_LIS = 0,
	_XTCP_TYPE_CLI = 1,
	_XTCP_TYPE_SRV = 2
}XTCP_TYPE;

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API xhand_t		xtcp_open(bool_t secu, const tchar_t* addr, const tchar_t* port);

XDL_API void		xtcp_close(xhand_t xtcp);

XDL_API object_t	xtcp_get_send_stream(xhand_t xtcp);

XDL_API object_t	xtcp_get_recv_stream(xhand_t xtcp);

XDL_API int			xtcp_type(xhand_t xtcp);

XDL_API int			xtcp_get_addr(xhand_t xtcp, tchar_t* buf, int max);

XDL_API bool_t		xtcp_send(xhand_t xtcp, const byte_t* data, dword_t len);

XDL_API bool_t		xtcp_recv(xhand_t xtcp, byte_t* buf, dword_t max);

XDL_API bool_t		xtcp_send_chunk(xhand_t xtcp, const byte_t* buf, dword_t len);

XDL_API bool_t		xtcp_recv_full(xhand_t xtcp, byte_t** pbuf, dword_t* plen);

XDL_API bool_t		xtcp_send_xml(xhand_t xtcp,link_t_ptr xml);

XDL_API bool_t		xtcp_recv_xml(xhand_t xtcp,link_t_ptr xml);

XDL_API bool_t		xtcp_send_var(xhand_t xtcp, object_t var);

XDL_API bool_t		xtcp_recv_var(xhand_t xtcp, object_t var);

/*********************************************************************************************/
#ifdef XDL_SUPPORT_SOCK_SRV

typedef struct _xtcp_block_t{
	int cbs;

	bool_t is_thread;
	xhand_t xh;

	object_t log;
	PF_WRITE_LOG_TITLE				pf_write_log_title;
	PF_WRITE_LOG_ERROR				pf_write_log_error;
	PF_WRITE_LOG_DATA				pf_write_log_data;
	PF_WRITE_LOG_XML				pf_write_log_xml;

	tchar_t path[PATH_LEN];
	tchar_t param[PATH_LEN];

#ifdef _OS_64
	unsigned long long	wrecv;
#else
	unsigned long		wrecv;
#endif
	void*	precv;

}xtcp_block_t;

typedef enum{
	XTCP_INVOKE_SUCCEED = 0,
	XTCP_INVOKE_WITHINFO = 1,
	XTCP_INVOKE_WITHERROR = 2,
	XTCP_INVOKE_PENDING = 100
}XTCP_INVOKE_STATE;

typedef int(_stdcall *PF_XTCP_INVOKE)(const xtcp_block_t* pb);

typedef void(*PF_XTCP_ACCEPT)(xhand_t xtcp, void* param);

XDL_API xhand_t		xtcp_start_thread(bool_t secu, short port, PF_XTCP_ACCEPT pf_accept, void* pf_param);

XDL_API xhand_t		xtcp_start_process(bool_t secu, short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

XDL_API void		xtcp_stop(xhand_t xtcp);

XDL_API void		xtcp_accept(xhand_t sok, PF_XTCP_ACCEPT pf_accept, void* pf_param);


#endif /*XDL_SUPPORT_SOCK_SRV*/

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_SOCK*/

#endif /*XDLTCP_H*/