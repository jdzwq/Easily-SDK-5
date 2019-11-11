/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssl service document

	@module	xdsssl.h | interface file

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

#ifndef _XDSSSL_H
#define _XDSSSL_H

#include "xdsdef.h"

typedef void(*PF_SSLS_DISPATCH)(xhand_t ssl, void* param);

typedef struct _ssl_listen_t{
	res_file_t so;

	int act;

	int res;
#ifdef XDK_SUPPORT_THREAD_QUEUE
	res_hand_t epo;
#endif

	bool_t is_thread;
	void* pf_param;
	union
	{
		PF_SSLS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};
}ssl_listen_t;

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xssl_start_thread: create ssl thread service routing.
@INPUT unsigned short port: the service port.
@INPUT PF_SSLS_DISPATCH pf_dispatch: the callback service dispatch function.
@INPUT void* param: the user parameter transback into dispath function.
@RETURN ssl_listen_t*: if succeeds return listen struct, fails return NULL.
*/
XDS_API ssl_listen_t*  xssl_start_thread(unsigned short port, PF_SSLS_DISPATCH pf_dispatch, void* param);

/*
@FUNCTION xssl_start_process: create ssl process service routing.
@INPUT unsigned short port: the service port.
@INPUT const tchar_t* sz_moudle: the process service moudle name.
@INPUT const tchar_t* sz_cmdline: the process execute command line.
@RETURN ssl_listen_t*: if succeeds return listen struct, fails return NULL.
*/
XDS_API ssl_listen_t*  xssl_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

/*
@FUNCTION xssl_stop: stop the ssl service routing.
@INPUT ssl_listen_t* plis: the service listen struct.
@RETURN void: none.
*/
XDS_API void  xssl_stop(ssl_listen_t* plis);

#ifdef	__cplusplus
}
#endif

#endif /*_XDSSSL_H*/