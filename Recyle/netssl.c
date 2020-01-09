/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssl document

	@module	netssl.c | ssl implement file

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

#include "netssl.h"
#include "xdlinit.h"
#include "xdlstd.h"

#if defined(XDL_SUPPORT_SSL) && defined(XDL_SUPPORT_SOCK)


typedef struct _ssl_t{
	xhand_head head;

	int type;
	res_sock_t so;

	ssl_sock_t ssl;
}ssl_t;

/****************************************************************************************************/

xhand_t xssl_cli(unsigned short port, const tchar_t* addr)
{
	net_addr_t sin;
	int n;
	bool_t rt;
	struct linger li = { 1,10 };

	res_sock_t so = 0;
	ssl_t* pso = NULL;

	TRY_CATCH;

	so = xsocket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!so)
	{
		raise_user_error(NULL, NULL);
	}

	xmem_zero((void*)&sin, sizeof(sin));

	fill_addr(&sin, port, addr);
	
	n = 5;
	rt = 0;
	while(n && !xsocket_connect(so,(res_addr_t)&sin,sizeof(sin)))
	{
		n--;
#if defined(_DEBUG) && defined(XDL_SUPPORT_THREAD)
		xthread_sleep(10);
#endif
	}

	if (!n)
	{
		raise_user_error(NULL, NULL);
	}

	xsocket_setopt(so, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(struct linger));

	pso = (ssl_t*)xmem_alloc(sizeof(ssl_t));
	pso->head.tag = _HANDLE_SSL;

	pso->so = so;
	pso->type = SSL_TYPE_CLIENT;

	pso->ssl = yssl_alloc(pso->so, xsocket_write, xsocket_read, 0);
	if (!pso->ssl)
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return (xhand_t)pso;
ONERROR:

	if (so)
		xsocket_close(so);

	if (pso)
		xmem_free(pso);

	XDL_TRACE_LAST;

	return NULL;
}

xhand_t xssl_srv(res_sock_t so)
{
	struct linger li = { 1, 10 };
	int zo = 0;

	ssl_t* pso = NULL;

	TRY_CATCH;

	if (!so)
	{
		raise_user_error(_T("xssl_srv"), _T("invalid socket handle"));
	}

	pso = (ssl_t*)xmem_alloc(sizeof(ssl_t));
	pso->head.tag = _HANDLE_SSL;

	pso->so = so;
	pso->type = SSL_TYPE_SERVER;

	xsocket_setopt(pso->so, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(struct linger));
	xsocket_setopt(pso->so, SOL_SOCKET, SO_SNDBUF, (const char*)&zo, sizeof(int));
	xsocket_setopt(pso->so, SOL_SOCKET, SO_RCVBUF, (const char*)&zo, sizeof(int));

	pso->ssl = yssl_alloc(pso->so, xsocket_write, xsocket_read, 1);
	if (!pso->ssl)
	{
		raise_user_error(NULL, NULL);
	}

	END_CATCH;

	return (xhand_t)pso;
ONERROR:

	if (pso)
		xmem_free(pso);

	XDL_TRACE_LAST;

	return NULL;
}

void  xssl_close(xhand_t ssl)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_CLIENT || pso->type == SSL_TYPE_SERVER);

	if (pso->type == SSL_TYPE_CLIENT)
	{
		//disable recive
		xsocket_shutdown(pso->so, 0);
	}
	else if (pso->type == SSL_TYPE_SERVER)
	{
		//disable send
		xsocket_shutdown(pso->so, 1);
		xsocket_wait(pso->so, 3);
	}

#if defined(_DEBUG) && defined(XDL_SUPPORT_THREAD)
	//wait data sended
	xthread_sleep(10);
#endif

	if (pso->type == SSL_TYPE_CLIENT)
	{
		xsocket_close(pso->so);
	}

	yssl_free(pso->ssl);

	xmem_free(pso);
}

res_sock_t xssl_socket(xhand_t ssl)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	return pso->so;
}

int xssl_type(xhand_t ssl)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	return pso->type;
}

bool_t xssl_write(xhand_t ssl, const byte_t* buf, dword_t* pb)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	if (C_ERR == yssl_write(pso->ssl, buf, *pb))
	{
		set_last_error(_T("xssl_write"), _T("ssl write failed"), -1);
		*pb = 0;
		return 0;
	}

	return 1;
}

bool_t xssl_read(xhand_t ssl, byte_t* buf, dword_t* pb)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	if (C_ERR == yssl_read(pso->ssl, buf, *pb))
	{
		set_last_error(_T("xssl_read"), _T("ssl read failed"), -1);
		*pb = 0;
		return 0;
	}

	return 1;
}

void xssl_set_ca(xhand_t ssl, const byte_t* sz_cert, dword_t len)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	yssl_set_ca(pso->ssl, sz_cert, len);
}

void xssl_set_cert(xhand_t ssl, const byte_t* sz_cert, dword_t len)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	yssl_set_cert(pso->ssl, sz_cert, len);
}

void xssl_set_rsa(xhand_t ssl, const byte_t* sz_rsa, dword_t rlen, const byte_t* sz_pwd, dword_t plen)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	yssl_set_rsa(pso->ssl, sz_rsa, rlen, sz_pwd, plen);
}

void xssl_set_dhm(xhand_t ssl, schar_t *dhm_P, schar_t *dhm_G)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	yssl_set_dhm(pso->ssl, dhm_P, dhm_G);
}

#endif //XDL_SUPPORT_SSL
