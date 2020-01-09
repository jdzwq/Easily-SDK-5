/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl tcp document

	@module	xdltcp.c | xdl http implement file

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

#include "xdltcp.h"
#include "xdlstd.h"
#include "xdldoc.h"


#ifdef XDL_SUPPORT_SOCK

typedef struct _xtcp_t{
	xhand_head head;		//head for xhand_t

	int type;
	bool_t secu;
	xhand_t socket;
	object_t send_stream;
	object_t recv_stream;

	tchar_t addr[RES_LEN + 1];
	tchar_t port[NUM_LEN + 1];
}xtcp_t;

#ifdef XDL_SUPPORT_SOCK_SRV

typedef struct _xtcp_listen_t{
	xhand_head head;			//reserved for xhand_t

	int type;
	xhand_t socket;
	object_t send_stream;
	object_t recv_stream;

	void *pf_param;
	PF_XTCP_ACCEPT pf_accept;
}xtcp_listen_t;

#endif /*XDL_SUPPORT_SOCK_SRV*/

/////////////////////////////////////////////////////////////////////////////////////////////


xhand_t  xtcp_open(bool_t secu, const tchar_t* addr, const tchar_t* port)
{
	xtcp_t* ptcp;

	if (is_null(addr) || is_null(port))
	{
		set_last_error(_T("xtcp_open"), _T("addr or port is empty"), -1);
		return 0;
	}

	ptcp = (xtcp_t*)xmem_alloc(sizeof(xtcp_t));
	ptcp->head.tag = _HANDLE_TCP;
	ptcp->head.cbs = sizeof(xtcp_t);

	ptcp->type = _XTCP_TYPE_CLI;
	ptcp->secu = secu;

	if (is_ip(addr))
	{
		xscpy(ptcp->addr, addr);
	}
	else
	{
		host_addr(addr, ptcp->addr);
	}

	xscpy(ptcp->port, port);

	ptcp->socket = xsocket_cli(ptcp->secu, xstos(ptcp->port), ptcp->addr);
	if(!ptcp->socket)
	{
		xmem_free(ptcp);
		return 0;
	}

	ptcp->send_stream = stream_alloc(ptcp->socket);
	ptcp->recv_stream = stream_alloc(ptcp->socket);

	return (xhand_t)ptcp;
}

void xtcp_close(xhand_t xtcp)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->type == _XTCP_TYPE_CLI);

	if (ptcp->send_stream)
	{
		stream_free(ptcp->send_stream);
	}

	if (ptcp->recv_stream)
	{
		stream_free(ptcp->recv_stream);
	}

	if (ptcp->socket)
		xsocket_close(ptcp->socket);

	xmem_free(ptcp);
}

object_t xtcp_get_send_stream(xhand_t xtcp)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);

	return ptcp->send_stream;
}

object_t xtcp_get_recv_stream(xhand_t xtcp)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);

	return ptcp->recv_stream;
}

int xtcp_type(xhand_t xtcp)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);

	return ptcp->type;
}


int	xtcp_get_addr(xhand_t xtcp, tchar_t* buf, int max)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;
	int len;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);

	len = xslen(ptcp->addr);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, ptcp->addr, len);
	}

	return len;
}

bool_t xtcp_send(xhand_t xtcp, const byte_t* data, dword_t len)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;
	dword_t dw = 0;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->send_stream != NULL);

	return stream_write_bytes(ptcp->send_stream, data, len, &dw);
}

bool_t xtcp_recv(xhand_t xtcp,byte_t* buf,dword_t max)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;
	dword_t dw = 0;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->recv_stream != NULL);

	return stream_read_bytes(ptcp->recv_stream, buf, max, &dw);
}

bool_t xtcp_send_full(xhand_t xtcp, const byte_t* buf, dword_t len)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;
	dword_t n_size = 0;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->send_stream != NULL);

	if (!len)
		return 1;
	
	n_size = len;
	if (!stream_write_chunk_size(ptcp->send_stream, n_size))
		return 0;

	stream_write_bytes(ptcp->send_stream, buf, len, &n_size);

	return stream_write_chunk_size(ptcp->send_stream, 0);
}

bool_t xtcp_recv_full(xhand_t xtcp, byte_t** pbuf, dword_t* plen)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;
	dword_t nlen, npos, nbys;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->recv_stream != NULL);

	nlen = npos = 0;

	do{
		if (!stream_read_chunk_size(ptcp->recv_stream, &nbys))
			return 0;

		if (!nbys)
			break;

		nlen += nbys;
		xmem_realloc_buffer(pbuf, nlen);

		if (!stream_read_bytes(ptcp->recv_stream, *pbuf + npos, nbys, &nbys))
			break;

		npos += nbys;
	} while (nbys);

	*plen = npos;
	return (!nbys) ? 1 : 0;
}

bool_t xtcp_send_xml(xhand_t xtcp,link_t_ptr xml)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->send_stream != NULL);

	return format_xml_doc_to_stream(xml, ptcp->send_stream);
}

bool_t xtcp_recv_xml(xhand_t xtcp,link_t_ptr xml)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->recv_stream != NULL);

	return parse_xml_doc_from_stream(xml, ptcp->recv_stream);
}

bool_t xtcp_send_var(xhand_t xtcp, object_t var)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->send_stream != NULL);

	return stream_write_line(ptcp->send_stream, var, NULL);
}

bool_t xtcp_recv_var(xhand_t xtcp, object_t var)
{
	xtcp_t* ptcp = (xtcp_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(ptcp->recv_stream != NULL);

	return stream_read_line(ptcp->recv_stream, var, NULL);
}

/*****************************************************************************************************/
#ifdef XDL_SUPPORT_SOCK_SRV

void xtcp_accept(xhand_t socket, PF_XTCP_ACCEPT pf_accept, void* pf_param)
{
	xtcp_t* ptcp = NULL;

	ptcp = (xtcp_t*)xmem_alloc(sizeof(xtcp_t));
	ptcp->head.tag = _HANDLE_TCP;
	ptcp->head.cbs = sizeof(xtcp_t);
	ptcp->type = _XTCP_TYPE_SRV;

	ptcp->socket = socket;
	ptcp->send_stream = stream_alloc(ptcp->socket);
	ptcp->recv_stream = stream_alloc(ptcp->socket);
	xsocket_peer(socket, ptcp->addr);

	if (pf_accept)
	{
		(*pf_accept)((xhand_t)ptcp, pf_param);
	}

	stream_free(ptcp->send_stream);
	stream_free(ptcp->recv_stream);

	xmem_free(ptcp);
}

static int _tcp_socket_accept(xhand_t socket, void* param)
{
	xtcp_listen_t* plis = (xtcp_listen_t*)param;

	XDL_ASSERT(plis != NULL);
	
	xtcp_accept(socket, plis->pf_accept, plis->pf_param);

	return 0;
}

xhand_t xtcp_start_thread(bool_t secu, short port, PF_XTCP_ACCEPT pf_accept, void* pf_param)
{
	xtcp_listen_t* plis;

	plis = (xtcp_listen_t*)xmem_alloc(sizeof(xtcp_listen_t));
	plis->head.tag = _HANDLE_TCP;
	plis->head.cbs = sizeof(xtcp_listen_t);

	plis->type = _XTCP_TYPE_LIS;

	plis->pf_accept = pf_accept;
	plis->pf_param = (void*)pf_param;

	plis->socket = xsocket_thread_srv(secu, port, _tcp_socket_accept, (void*)plis);
	
	if (!plis->socket)
	{
		xmem_free(plis);
		return 0;
	}

	return (xhand_t)plis;
}

xhand_t xtcp_start_process(bool_t secu, short port, const tchar_t* sz_module, tchar_t* sz_cmdline)
{
	xtcp_listen_t* plis;

	plis = (xtcp_listen_t*)xmem_alloc(sizeof(xtcp_listen_t));
	plis->head.tag = _HANDLE_TCP;
	plis->head.cbs = sizeof(xtcp_listen_t);

	plis->type = _XTCP_TYPE_LIS;

	plis->socket = xsocket_process_srv(secu, port, sz_module, sz_cmdline);

	if (!plis->socket)
	{
		xmem_free(plis);
		return 0;
	}

	return (xhand_t)plis;
}

void xtcp_stop(xhand_t xtcp)
{
	xtcp_listen_t* plis = (xtcp_listen_t*)xtcp;

	XDL_ASSERT(xtcp && xtcp->tag == _HANDLE_TCP);
	XDL_ASSERT(plis->type == _XTCP_TYPE_LIS);

	if (plis->socket)
		xsocket_stop(plis->socket);

	xmem_free(plis);
}

#endif /*XDL_SUPPORT_SOCK_SRV*/

#endif /*XDL_SUPPORT_SOCK*/
