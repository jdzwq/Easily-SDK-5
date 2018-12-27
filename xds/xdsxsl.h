/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xsl service document

	@module	xdsxsl.h | xsl interface file

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

#ifndef _XDSXSL_H
#define _XDSXSL_H

#include "xdsdef.h"

typedef void(*PF_XSLS_DISPATCH)(xhand_t xsl, void* param);

typedef struct _xsl_listen_t{
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
		PF_XSLS_DISPATCH pf_dispatch;
		const tchar_t* sz_module;
	};
}xsl_listen_t;

#ifdef	__cplusplus
extern "C" {
#endif

	XDS_API xsl_listen_t*  xxsl_start_thread(unsigned short port, PF_XSLS_DISPATCH pf_dispatch, void* param);

	XDS_API xsl_listen_t*  xxsl_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline);

	XDS_API void  xxsl_stop(xsl_listen_t* plis);

#ifdef	__cplusplus
}
#endif

#endif /*_XDSXSL_H*/