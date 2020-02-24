﻿/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pacs service document

	@module	xdspacs.c | pacs service implement file

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

#include "xdspacs.h"
#include "xdstcp.h"


/*****************************************************************************************************/

static void _pacs_bio_dispatch(xhand_t bio, void* param)
{
	pacs_listen_t* plis = (pacs_listen_t*)param;
	xhand_t pacs = NULL;

	XDL_ASSERT(plis != NULL);

	pacs = xpacs_scp(bio);

	if (plis->pf_dispatch)
	{
		(*plis->pf_dispatch)((xhand_t)pacs, plis->pf_param);
	}

	xpacs_close(pacs);
}

pacs_listen_t* xpacs_start_thread(int secu, unsigned short port, PF_PACSS_DISPATCH pf_dispatch, void* pf_param)
{
	pacs_listen_t* plis;

	plis = (pacs_listen_t*)xmem_alloc(sizeof(pacs_listen_t));
	
	plis->secu = secu;
	plis->pf_dispatch = pf_dispatch;
	plis->pf_param = (void*)pf_param;

	switch (plis->secu)
	{
	case _SECU_SSL:
		plis->lis_ssl = xssl_start_thread(port, _pacs_bio_dispatch, (void*)plis);
		break;
	case _SECU_TLS:
		plis->lis_tls = xtls_start_thread(port, _pacs_bio_dispatch, (void*)plis);
		break;
	default:
		plis->lis_tcp = xtcp_start_thread(port, _pacs_bio_dispatch, (void*)plis);
		break;
	}

	if (!plis->lis_tcp)
	{
		xmem_free(plis);
		return 0;
	}

	return plis;
}

pacs_listen_t* xpacs_start_process(int secu, unsigned short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	pacs_listen_t* plis;

	plis = (pacs_listen_t*)xmem_alloc(sizeof(pacs_listen_t));
	
	plis->secu = secu;

	switch (plis->secu)
	{
	case _SECU_SSL:
		plis->lis_ssl = xssl_start_process(port, sz_module, sz_cmdline);
		break;
	case _SECU_TLS:
		plis->lis_tls = xtls_start_process(port, sz_module, sz_cmdline);
		break;
	default:
		plis->lis_tcp = xtcp_start_process(port, sz_module, sz_cmdline);
		break;
	}

	if (!plis->lis_tcp)
	{
		xmem_free(plis);
		return 0;
	}

	return plis;
}

void xpacs_stop(pacs_listen_t* plis)
{
	switch (plis->secu)
	{
	case _SECU_SSL:
		if (plis->lis_ssl)
			xssl_stop(plis->lis_ssl);
		break;
	case _SECU_TLS:
		if (plis->lis_tls)
			xtls_stop(plis->lis_tls);
		break;
	default:
		if (plis->lis_tcp)
			xtcp_stop(plis->lis_tcp);
		break;
	}

	xmem_free(plis);
}

