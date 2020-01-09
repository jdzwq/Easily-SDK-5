/***********************************************************************
	Easily xdl v5.0

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc udp service document

	@module	xdsudp.h | udp interface file

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

#ifndef _XDSUDP_H
#define _XDSUDP_H

#include "xdsdef.h"


typedef void(*PF_UDPS_DISPATCH)(unsigned short bind, unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size, void* param);

typedef struct _udp_listen_t{
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
		PF_UDPS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};
}udp_listen_t;


#ifdef	__cplusplus
extern "C" {
#endif

	XDS_API udp_listen_t*  xudp_start_thread(unsigned short port, PF_UDPS_DISPATCH pf_dispatch, void* param);

	XDS_API udp_listen_t*  xudp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

	XDS_API void  xudp_stop(udp_listen_t* plis);

#ifdef	__cplusplus
}
#endif

#endif /*_XDSUDP_H*/