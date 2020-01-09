/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssl document

	@module	oemssl.c | ssl implement file

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

#include "oemssl.h"
#include "impmem.h"
#include "imperr.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_SSL

#include "../ssl/ssl.h"

#ifdef _OS_WINDOWS
#pragma comment(lib,"ssl.lib")
#endif

static char dhm_G[] = "4";
static char dhm_P[] = "E4004C1F94182000103D883A448B3F802CE4B44A83301270002C20D0321CFD00" \
"11CCEF784C26A400F43DFB901BCA7538F2C6B176001CF5A0FD16D2C48B1D0C1C" \
"F6AC8E1DA6BCC3B4E1F96B0564965300FFA1D0B601EB2800F489AA512C4B248C" \
"01F76949A60BB7F00A40B1EAB64BDD48E8A700D60B7F1200FA8E77B0A979DABF";


static void yssl_debug(void *ctx, int level, char *str)
{
	int len;
	tchar_t err[ERR_LEN + 1] = { 0 };

#ifdef _UNICODE
	len = a_xslen(str);
	len = mbs_to_ucs(str, len, err, ERR_LEN);
	set_last_error(_T("-1"), err, len);
#else
	len = a_xslen(str);
	len = (len < max) ? len : max;
	a_xsncpy(err, str, len);
	set_last_error(_T("-1"), err, len);
#endif
}

ssl_sock_t yssl_alloc(res_sock_t so, pf_socket_write pf_write, pf_socket_read pf_read, bool_t srv)
{
	int ret;
	havege_state* phs = NULL;
	ssl_session* pssn = NULL;
	ssl_context* pssl = NULL;
	res_sock_t* pw = NULL;
	res_sock_t* pr = NULL;

	phs = (havege_state*)xmem_alloc(sizeof(havege_state));
	havege_init(phs);
	
	pssl = (ssl_context*)xmem_alloc(sizeof(ssl_context));

	if ((ret = ssl_init(pssl)) != 0)
	{
		xmem_free(phs);
		return NULL;
	}

	if (srv)
	{
		ssl_set_endpoint(pssl, SSL_IS_SERVER);
		ssl_set_dh_param(pssl, dhm_P, dhm_G);
	}
	else
	{
		ssl_set_endpoint(pssl, SSL_IS_CLIENT);
	}

	ssl_set_authmode(pssl, SSL_VERIFY_NONE);

	ssl_set_rng(pssl, havege_rand, phs);
	ssl_set_dbg(pssl, yssl_debug, NULL);

	pw = (res_sock_t*)xmem_alloc(sizeof(res_sock_t));
	*pw = so;
	pr = (res_sock_t*)xmem_alloc(sizeof(res_sock_t));
	*pr = so;
	ssl_set_bio(pssl, pf_read, (void*)pr, pf_write, (void*)pw);

	ssl_set_ciphers(pssl, ssl_default_ciphers);
	
	pssn = (ssl_session*)xmem_alloc(sizeof(ssl_session));	
	ssl_set_session(pssl, 1, 600, pssn);

	return (ssl_sock_t)pssl;
}

void yssl_free(ssl_sock_t ssl)
{
	ssl_context* pssl = (ssl_context*)ssl;
	havege_state* phs = NULL;
	ssl_session* pssn = NULL;
	res_sock_t* pw = NULL;
	res_sock_t* pr = NULL;

	x509_cert* pcert_ca = NULL;
	x509_cert* pcert_owner = NULL;
	rsa_context* prsa = NULL;


	if (!pssl)
		return;

	phs = pssl->p_rng;
	pssn = pssl->session;
	pw = pssl->p_send;
	pr = pssl->p_recv;

	pcert_ca = pssl->ca_chain;
	if (pcert_ca)
	{
		x509_free(pcert_ca);
		xmem_free(pcert_ca);
	}
	pssl->ca_chain = NULL;

	pcert_owner = pssl->own_cert;
	if (pcert_owner)
	{
		x509_free(pcert_owner);
		xmem_free(pcert_owner);
	}
	pssl->own_cert = NULL;

	prsa = pssl->rsa_key;
	if (prsa)
	{
		rsa_free(prsa);
	}
	pssl->rsa_key = NULL;

	ssl_free(pssl);

	xmem_free(phs);
	xmem_free(pssn);
	xmem_free(pw);
	xmem_free(pr);

	xmem_free(pssl);
}

int yssl_read(ssl_sock_t ssl, byte_t* buf, dword_t max)
{
	ssl_context* pssl = (ssl_context*)ssl;
	int ret, len = 0;

	do
	{
		ret = ssl_read(pssl, buf, max);

		if (ret == ERR_NET_TRY_AGAIN)
			continue;

		if (ret == ERR_SSL_PEER_CLOSE_NOTIFY)
			return 0;

		if (ret < 0)
			return C_ERR;

	} while (0);

	return ret;
}

int yssl_write(ssl_sock_t ssl, const byte_t* buf, dword_t size)
{
	ssl_context* pssl = (ssl_context*)ssl;
	int ret, len = 0;

	do
	{
		ret = ssl_write(pssl, (byte_t*)buf, size);

		if (ret == ERR_NET_TRY_AGAIN)
			continue;

		if (ret == ERR_SSL_PEER_CLOSE_NOTIFY)
			return 0;

		if (ret < 0)
		{
			return C_ERR;
		}

	} while (0);

	return ret;
}

void yssl_set_ca(xhand_t ssl, const byte_t* sz_cert, dword_t len)
{
	ssl_context* pssl = (ssl_context*)ssl;

	if (pssl->ca_chain == NULL)
		pssl->ca_chain = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	x509parse_crt(pssl->ca_chain, (unsigned char*)sz_cert, (int)len);
}

void yssl_set_cert(xhand_t ssl, const byte_t* sz_cert, dword_t len)
{
	ssl_context* pssl = (ssl_context*)ssl;

	if (pssl->own_cert == NULL)
		pssl->own_cert = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	x509parse_crt(pssl->own_cert, (unsigned char*)sz_cert, (int)len);
}

void yssl_set_rsa(xhand_t ssl, const byte_t* sz_rsa, dword_t rlen, const byte_t* sz_pwd, dword_t plen)
{
	ssl_context* pssl = (ssl_context*)ssl;

	if (pssl->rsa_key == NULL)
		pssl->rsa_key = (rsa_context*)xmem_alloc(sizeof(rsa_context));

	x509parse_key(pssl->rsa_key, (unsigned char*)sz_rsa, (int)rlen, (unsigned char*)sz_pwd, (int)plen);
}

void yssl_set_dhm(xhand_t ssl, schar_t *dhm_P, schar_t *dhm_G)
{
	ssl_context* pssl = (ssl_context*)ssl;

	mpi_read_string(&(pssl->dhm_ctx.P), 16, dhm_P, -1);
	mpi_read_string(&(pssl->dhm_ctx.G), 16, dhm_G, -1);
}

#endif //XDLOEM