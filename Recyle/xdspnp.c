/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pnp service document

	@module	xdspnp.c | pnp service implement file

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

#include "xdspnp.h"
#include "xdstcp.h"


/*****************************************************************************************************/
static void _pnp_bio_dispatch(unsigned short bind, unsigned short port, const tchar_t* addr, const byte_t* pack, dword_t size, void* param)
{
	pnp_listen_t* plis = (pnp_listen_t*)param;

	XDL_ASSERT(plis != NULL);

	if (plis->pf_dispatch)
	{
		(*plis->pf_dispatch)(bind, port, addr, pack, size, plis->pf_param);
	}
}

pnp_listen_t* xpnp_start_thread(int secu, unsigned short port, PF_PNPS_DISPATCH pf_dispatch, void* pf_param)
{
	pnp_listen_t* plis;

	plis = (pnp_listen_t*)xmem_alloc(sizeof(pnp_listen_t));
	
	plis->secu = secu;
	plis->pf_dispatch = pf_dispatch;
	plis->pf_param = (void*)pf_param;

	switch (plis->secu)
	{
	default:
		plis->lis_udp = xudp_start_thread(port, _pnp_bio_dispatch, (void*)plis);
		break;
	}

	if (!plis->lis_udp)
	{
		xmem_free(plis);
		return 0;
	}

	return plis;
}

pnp_listen_t* xpnp_start_process(int secu, unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	pnp_listen_t* plis;

	plis = (pnp_listen_t*)xmem_alloc(sizeof(pnp_listen_t));
	
	plis->secu = secu;

	switch (plis->secu)
	{
	default:
		plis->lis_udp = xudp_start_process(port, sz_module, sz_cmdline);
		break;
	}

	if (!plis->lis_udp)
	{
		xmem_free(plis);
		return 0;
	}

	return plis;
}

void xpnp_stop(pnp_listen_t* plis)
{
	switch (plis->secu)
	{
	default:
		if (plis->lis_udp)
			xudp_stop(plis->lis_udp);
		break;
	}

	xmem_free(plis);
}

