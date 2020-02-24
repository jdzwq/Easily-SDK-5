/***********************************************************************
	Easily xds v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tftp service document

	@module	xdstftp.c | tftp service implement file

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

#include "xdstftp.h"
#include "xdsudp.h"


/*****************************************************************************************************/

void xtftp_dispatch(unsigned short port, const tchar_t* addr, const byte_t* pack, PF_TFTP_DISPATCH pf_dispatch, void* pf_param)
{
	xhand_t tftp = NULL;
	
	tftp = xtftp_server(port, addr, pack);

	if (pf_dispatch)
	{
		(*pf_dispatch)((xhand_t)tftp, pf_param);
	}

	xtftp_close(tftp);
}

static void _tftp_udp_dispatch(unsigned short bind, unsigned short port, const tchar_t* addr, const byte_t* pack, void* param)
{
	tftp_listen_t* plis = (tftp_listen_t*)param;

	XDL_ASSERT(plis != NULL);

	xtftp_dispatch(port, addr, pack, plis->pf_dispatch, plis->pf_param);
}

tftp_listen_t* xtftp_start_thread(unsigned short port, PF_TFTP_DISPATCH pf_dispatch, void* pf_param)
{
	tftp_listen_t* plis;

	plis = (tftp_listen_t*)xmem_alloc(sizeof(tftp_listen_t));
	
	plis->pf_dispatch = pf_dispatch;
	plis->pf_param = (void*)pf_param;

	plis->lis_udp = xudp_start_thread(port, _tftp_udp_dispatch, (void*)plis);

	if (!plis->lis_udp)
	{
		xmem_free(plis);
		return 0;
	}

	return plis;
}

tftp_listen_t* xtftp_start_process(unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	tftp_listen_t* plis;

	plis = (tftp_listen_t*)xmem_alloc(sizeof(tftp_listen_t));

	plis->lis_udp = xudp_start_process(port, sz_module, sz_cmdline);

	if (!plis->lis_udp)
	{
		xmem_free(plis);
		return 0;
	}

	return plis;
}

void xtftp_stop(tftp_listen_t* plis)
{
	if (plis->lis_udp)
		xudp_stop(plis->lis_udp);

	xmem_free(plis);
}

