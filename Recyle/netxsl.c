/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xsl document

	@module	netxsl.c | xsl implement file

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

#include "netxsl.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_CRYPT)

#define XSL_CTL_SIZE		8
#define XSL_HDR_SIZE		8
#define XSL_MSG_SIZE		6
#define XSL_RND_SIZE		32
#define XSL_CPH_SIZE		5
#define XSL_MST_SIZE		48
#define XSL_MAC_SIZE		20
#define XSL_PRF_SIZE		12

static char dhm_G[] = "4";
static char dhm_P[] = "E4004C1F94182000103D883A448B3F802CE4B44A83301270002C20D0321CFD00" \
"11CCEF784C26A400F43DFB901BCA7538F2C6B176001CF5A0FD16D2C48B1D0C1C" \
"F6AC8E1DA6BCC3B4E1F96B0564965300FFA1D0B601EB2800F489AA512C4B248C" \
"01F76949A60BB7F00A40B1EAB64BDD48E8A700D60B7F1200FA8E77B0A979DABF";

typedef struct _xsl_t{
	xhand_head head;

	int type;
	xhand_t tcp;
	int state;

	byte_t rnd_srv[XSL_RND_SIZE];
	byte_t rnd_cli[XSL_RND_SIZE];
	byte_t pre_mst[XSL_MST_SIZE];

	x509_cert* crt_ca;
	x509_cert* crt_pe;
	x509_cert* crt_ow;
	rsa_context* rsa;
	dhm_context* dhm;

	havege_state* rng;
	md5_context* md5;
	sha1_context* sha;

	byte_t mst_key[XSL_MST_SIZE];

	arc4_context* arc_enc;
	arc4_context* arc_dec;

	int ciph;
	int key_len;
	int mac_len;
	byte_t mac_enc[XSL_MAC_SIZE];
	byte_t mac_dec[XSL_MAC_SIZE]; 
}xsl_t;

/****************************************************************************************************/

static void _xsl_init(xsl_t* pxsl)
{
	dword_t ts;
	int i;
	byte_t* p;

	pxsl->rng = (havege_state*)xmem_alloc(sizeof(havege_state));
	havege_init(pxsl->rng);

	ts = (dword_t)get_times();

	if (pxsl->type == XSL_TYPE_CLIENT)
	{
		pxsl->ciph = XSL_RSA_RC4_128_SHA;

		PUT_DWORD_NET(pxsl->rnd_cli, 0, ts);
		p = pxsl->rnd_cli;
	}
	else
	{
		pxsl->ciph = XSL_RSA_RC4_128_MD5;

		PUT_DWORD_NET(pxsl->rnd_srv, 0, ts);
		p = pxsl->rnd_srv;
	}

	for (i = 4; i < XSL_RND_SIZE; i++)
	{
		*(p + i) = (byte_t)havege_rand(pxsl->rng);
	}

	pxsl->dhm = (dhm_context*)xmem_alloc(sizeof(dhm_context));

	if (pxsl->type == XSL_TYPE_SERVER)
	{
		mpi_read_string(&(pxsl->dhm->P), 16, dhm_P, -1);
		mpi_read_string(&(pxsl->dhm->G), 16, dhm_G, -1);
	}
	else
	{
		pxsl->pre_mst[0] = (unsigned char)XSL_MAJOR_VERSION_3;
		pxsl->pre_mst[1] = (unsigned char)XSL_MINOR_VERSION_1;

		for (i = 2; i < XSL_MST_SIZE; i++)
			pxsl->pre_mst[i] = (unsigned char)havege_rand(pxsl->rng);
	}

	pxsl->md5 = (md5_context*)xmem_alloc(sizeof(md5_context));
	md5_starts(pxsl->md5);

	pxsl->sha = (sha1_context*)xmem_alloc(sizeof(sha1_context));
	sha1_starts(pxsl->sha);

	pxsl->state = 0;
}

static void _xsl_uninit(xsl_t* pxsl)
{
	xmem_free(pxsl->rng);

	dhm_free(pxsl->dhm);
	xmem_free(pxsl->dhm);

	xmem_free(pxsl->md5);
	xmem_free(pxsl->sha);

	if (pxsl->arc_enc)
		xmem_free(pxsl->arc_enc);

	if (pxsl->arc_dec)
		xmem_free(pxsl->arc_dec);

	if (pxsl->rsa)
	{
		rsa_free(pxsl->rsa);
		xmem_free(pxsl->rsa);
	}
	if (pxsl->crt_ow)
	{
		x509_free(pxsl->crt_ow);
		xmem_free(pxsl->crt_ow);
	}
	if (pxsl->crt_pe)
	{
		x509_free(pxsl->crt_pe);
		xmem_free(pxsl->crt_pe);
	}
	if (pxsl->crt_ca)
	{
		x509_free(pxsl->crt_ca);
		xmem_free(pxsl->crt_ca);
	}
}

static void _xsl_hmac(unsigned char *secret, int slen, char *label, unsigned char *random, int rlen, unsigned char *dstbuf, int dlen)
{
	int nb, hs;
	int i, j, k;
	unsigned char *S1, *S2;
	unsigned char tmp[128] = { 0 };
	unsigned char h_i[20] = { 0 };

	hs = (slen + 1) / 2;
	S1 = secret;
	S2 = secret + slen - hs;

	nb = a_xslen(label);
	xmem_copy(tmp + 20, label, nb);
	xmem_copy(tmp + 20 + nb, random, rlen);
	nb += rlen;

	/*
	* First compute P_md5(secret,label+random)[0..dlen]
	*/
	md5_hmac(S1, hs, tmp + 20, nb, 4 + tmp);

	for (i = 0; i < dlen; i += 16)
	{
		md5_hmac(S1, hs, 4 + tmp, 16 + nb, h_i);
		md5_hmac(S1, hs, 4 + tmp, 16, 4 + tmp);

		k = (i + 16 > dlen) ? dlen % 16 : 16;

		for (j = 0; j < k; j++)
			dstbuf[i + j] = h_i[j];
	}

	/*
	* XOR out with P_sha1(secret,label+random)[0..dlen]
	*/
	sha1_hmac(S2, hs, tmp + 20, nb, tmp);

	for (i = 0; i < dlen; i += 20)
	{
		sha1_hmac(S2, hs, tmp, 20 + nb, h_i);
		sha1_hmac(S2, hs, tmp, 20, tmp);

		k = (i + 20 > dlen) ? dlen % 20 : 20;

		for (j = 0; j < k; j++)
			dstbuf[i + j] = (unsigned char)(dstbuf[i + j] ^ h_i[j]);
	}
}

static void _xsl_derive(xsl_t *pxsl)
{
	unsigned char rndb[XSL_RND_SIZE * 2] = { 0 };
	unsigned char keyblk[256] = { 0 };

	xmem_copy((void*)rndb, (void*)pxsl->rnd_cli, XSL_RND_SIZE);
	xmem_copy((void*)(rndb + XSL_RND_SIZE), (void*)pxsl->rnd_srv, XSL_RND_SIZE);

	/* generate session master secret*/
	_xsl_hmac(pxsl->pre_mst, XSL_MST_SIZE, XSL_STR_SECRET, rndb, XSL_RND_SIZE * 2, pxsl->mst_key, XSL_MST_SIZE);

	/* Swap the client and server random values.*/
	xmem_copy((void*)rndb, (void*)pxsl->rnd_srv, XSL_RND_SIZE);
	xmem_copy((void*)(rndb + XSL_RND_SIZE), (void*)pxsl->rnd_cli, XSL_RND_SIZE);

	/* generate key expansion*/
	_xsl_hmac(pxsl->mst_key, XSL_MST_SIZE, XSL_STR_EXPANSION, rndb, XSL_RND_SIZE * 2, keyblk, 256);

	xmem_zero(pxsl->rnd_cli, sizeof(pxsl->rnd_cli));
	xmem_zero(pxsl->rnd_srv, sizeof(pxsl->rnd_srv));
	xmem_zero(pxsl->pre_mst, sizeof(pxsl->pre_mst));

	switch (pxsl->ciph)
	{
	case XSL_RSA_RC4_128_MD5:
		pxsl->key_len = 16;
		pxsl->mac_len = 16;
		break;

	case XSL_RSA_RC4_128_SHA:
		pxsl->key_len = 16;
		pxsl->mac_len = 20;
		break;
	}

	if (pxsl->type == XSL_TYPE_CLIENT)
	{
		xmem_copy(pxsl->mac_enc, keyblk, pxsl->mac_len);
		xmem_copy(pxsl->mac_dec, keyblk + pxsl->mac_len, pxsl->mac_len);

		pxsl->arc_enc = (arc4_context*)xmem_alloc(sizeof(arc4_context));
		arc4_setup(pxsl->arc_enc, keyblk + pxsl->mac_len * 2, pxsl->key_len);
		pxsl->arc_dec = (arc4_context*)xmem_alloc(sizeof(arc4_context)); 
		arc4_setup(pxsl->arc_dec, keyblk + pxsl->mac_len * 2 + pxsl->key_len, pxsl->key_len);
	}
	else
	{
		xmem_copy(pxsl->mac_enc, keyblk + pxsl->mac_len, pxsl->mac_len);
		xmem_copy(pxsl->mac_dec, keyblk, pxsl->mac_len);

		pxsl->arc_enc = (arc4_context*)xmem_alloc(sizeof(arc4_context));;
		arc4_setup(pxsl->arc_enc, keyblk + pxsl->mac_len * 2 + pxsl->key_len, pxsl->key_len);
		pxsl->arc_dec = (arc4_context*)xmem_alloc(sizeof(arc4_context));;
		arc4_setup(pxsl->arc_dec, keyblk + pxsl->mac_len * 2, pxsl->key_len);
	}
}

static void _arc4_encrypt(byte_t* buf, dword_t dw, void* pa)
{
	arc4_context* arc = (arc4_context*)pa;

	arc4_crypt(arc, dw, buf, buf);
}

static void _arc4_decrypt(byte_t* buf, dword_t dw, void* pa)
{
	arc4_context* arc = (arc4_context*)pa;

	arc4_crypt(arc, dw, buf, buf);
}

/***************************************************************************************************/
static dword_t _xsl_format_client_hello(xsl_t *pxsl, stream_t stm)
{
	dword_t total = 0;
	byte_t num[2];
	byte_t* buf = NULL;
	unsigned short tag;
	unsigned short len;

	x509_cert* crt = NULL;

	TRY_CATCH;

	tag = XSL_TAG_RANDOM;
	while (tag)
	{
		//tag
		if (stm)
		{
			PUT_SWORD_NET(num, 0, tag);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		switch (tag)
		{
		case XSL_TAG_RANDOM:
			len = XSL_RND_SIZE;
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);
				xmem_copy((void*)buf, (void*)pxsl->rnd_cli, len);
			}
			tag = XSL_TAG_CIPHER;
			break;
		case XSL_TAG_CIPHER:
			len = sizeof(short);
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);
				PUT_SWORD_NET(buf, 0, pxsl->ciph);
			}
			crt = pxsl->crt_ow;
			tag = (crt && crt->next)? XSL_TAG_CERTIFICATE : 0;
			break;
		case XSL_TAG_CERTIFICATE:
			len = crt->raw.len;
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);
				xmem_copy((void*)buf, (void*)crt->raw.p, len);
			}
			crt = crt->next;
			tag = (crt && crt->next) ? XSL_TAG_CERTIFICATE : 0;
			break;
		}

		//tag length
		if (stm)
		{
			PUT_SWORD_NET(num, 0, len);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		//tag value
		if (stm)
		{
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += len;

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	END_CATCH;

	return total;
ONERROR:

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static dword_t _xsl_format_client_exchange(xsl_t *pxsl, stream_t stm)
{
	dword_t total = 0;
	byte_t num[2];
	byte_t* buf = NULL;
	unsigned short tag;
	unsigned short len;

	rsa_context* rsa;

	TRY_CATCH;

	tag = XSL_TAG_RSACRYPT;
	while (tag)
	{
		//tag
		if (stm)
		{
			PUT_SWORD_NET(num, 0, tag);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		switch (tag)
		{
		case XSL_TAG_RSACRYPT:
			//use server public key for encrypt
			rsa = (pxsl->crt_pe)? &(pxsl->crt_pe->rsa) : NULL;
			len = (rsa)? rsa->len : 0;
			if (!len)
			{
				raise_user_error(_T("_xsl_format_client_exchange"), _T("invalid server public key"));
			}
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);
				if (C_OK != rsa_pkcs1_encrypt(rsa, XSL_RSA_PUBLIC, XSL_MST_SIZE, pxsl->pre_mst, buf))
				{
					raise_user_error(_T("_xsl_format_client_exchange"), _T("rsa encrypt failed"));
				}
			}
			tag = 0;
			break;

		}

		//tag length
		if (stm)
		{
			PUT_SWORD_NET(num, 0, len);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		//tag value
		if (stm)
		{
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += len;

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	if (stm)
	{
		//derive client key
		_xsl_derive(pxsl);
	}

	END_CATCH;

	return total;
ONERROR:

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static dword_t _xsl_format_client_finished(xsl_t *pxsl, stream_t stm)
{
	dword_t total = 0;
	byte_t num[2];
	byte_t* buf = NULL;
	unsigned short tag;
	unsigned short len;

	byte_t hash[MD5_SIZE + SHA1_SIZE] = { 0 };
	byte_t hmac[32] = { 0 };

	md5_context* xsl_md5 = NULL;
	sha1_context* xsl_sha = NULL;

	TRY_CATCH;

	if (stm)
	{
		stream_set_crypt(stm, _arc4_encrypt, NULL, (void*)pxsl->arc_enc);
	}

	tag = XSL_TAG_PROFILE;
	while (tag)
	{
		//tag
		if (stm)
		{
			PUT_SWORD_NET(num, 0, tag);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		switch (tag)
		{
		case XSL_TAG_PROFILE:
			if (stm)
			{
				//finish handshake read
				xsl_md5 = (md5_context*)xmem_clone((void*)pxsl->md5, sizeof(md5_context));
				xsl_sha = (sha1_context*)xmem_clone((void*)pxsl->sha, sizeof(sha1_context));

				md5_finish(xsl_md5, hash);
				sha1_finish(xsl_sha, hash + MD5_SIZE);

				xmem_free(xsl_md5);
				xsl_md5 = NULL;
				xmem_free(xsl_sha);
				xsl_sha = NULL;
			}
			//make client profile
			len = XSL_PRF_SIZE;
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);

				_xsl_hmac(pxsl->mst_key, XSL_MST_SIZE, XSL_STR_FINISHED, hash, MD5_SIZE + SHA1_SIZE, buf, len);

				if (pxsl->mac_len == MD5_SIZE)
				{
					md5_hmac(pxsl->mac_enc, pxsl->mac_len, buf, len, hmac);
				}
				else if (pxsl->mac_len == SHA1_SIZE)
				{
					sha1_hmac(pxsl->mac_enc, pxsl->mac_len, buf, len, hmac);
				}
			}
			tag = XSL_TAG_RSASIGN;
			break;
		case XSL_TAG_RSASIGN:
			//use client private key for sign
			len = (pxsl->rsa) ? pxsl->rsa->len : 0;
			if (!len)
			{
				raise_user_error(_T("_xsl_format_client_finished"), _T("invalid client private key"));
			}
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);

				if (C_OK != rsa_pkcs1_sign(pxsl->rsa, XSL_RSA_PRIVATE, 0, pxsl->mac_len, hmac, buf))
				{
					raise_user_error(_T("_xsl_format_client_finished"), _T("rsa sign failed"));
				}
			}
			tag = 0;
			break;
		}

		//tag length
		if (stm)
		{
			PUT_SWORD_NET(num, 0, len);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		//tag value
		if (stm)
		{
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += len;

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	if (stm)
	{
		stream_set_crypt(stm, NULL, NULL, NULL);
	}

	END_CATCH;

	return total;
ONERROR:

	if (stm)
	{
		stream_set_crypt(stm, NULL, NULL, NULL);
	}

	if (buf)
		xmem_free(buf);

	if (xsl_md5)
		xmem_free(xsl_md5);

	if (xsl_sha)
		xmem_free(xsl_sha);

	XDL_TRACE_LAST;

	return 0;
}

static dword_t _xsl_format_client_error(xsl_t *pxsl, stream_t stm)
{
	dword_t total = 0;
	byte_t num[2];
	byte_t* buf = NULL;
	unsigned short tag;
	unsigned short len;

	TRY_CATCH;

	tag = XSL_TAG_ERROR;
	while (tag)
	{
		//tag
		if (stm)
		{
			PUT_SWORD_NET(num, 0, tag);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		switch (tag)
		{
		case XSL_TAG_ERROR:
			buf = a_xsclone("client error");
			len = a_xslen(buf);

			tag = 0;
			break;
		}

		//tag length
		if (stm)
		{
			PUT_SWORD_NET(num, 0, len);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		//tag value
		if (stm)
		{
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += len;

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	END_CATCH;

	return total;
ONERROR:

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

bool_t _xsl_write_client_handshake(xsl_t* pxsl, int hs)
{
	byte_t hdr[XSL_HDR_SIZE] = { 0 };

	dword_t dw = 0;
	stream_t stm = NULL;
	
	switch (hs)
	{
	case XSL_HS_CLIENT_HELLO:
		dw = _xsl_format_client_hello(pxsl, stm);
		break;
	case XSL_HS_CLIENT_EXCHANGE:
		dw = _xsl_format_client_exchange(pxsl, NULL);
		break;
	case XSL_HS_FINISHED:
		dw = _xsl_format_client_finished(pxsl, NULL);
		break;
	case XSL_HS_ERROR:
		dw = _xsl_format_client_error(pxsl, NULL);
		break;
	}

	if (!dw)
		return 0;

	//header block
	PUT_SWORD_NET(hdr, 0, hs);
	PUT_BYTE(hdr, 2, XSL_MAJOR_VERSION_3);
	PUT_BYTE(hdr, 3, XSL_MINOR_VERSION_1);
	PUT_DWORD_NET(hdr, 4, dw);

	stm = stream_alloc(pxsl->tcp);

	if (!stream_write_bytes(stm, hdr, XSL_HDR_SIZE))
	{
		stream_free(stm);
		return 0;
	}

	switch (hs)
	{
	case XSL_HS_CLIENT_HELLO:
		dw = _xsl_format_client_hello(pxsl, stm);
		break;
	case XSL_HS_CLIENT_EXCHANGE:
		dw = _xsl_format_client_exchange(pxsl, stm);
		break;
	case XSL_HS_FINISHED:
		dw = _xsl_format_client_finished(pxsl, stm);
		break;
	case XSL_HS_ERROR:
		dw = _xsl_format_client_error(pxsl, stm);
		break;
	default:
		dw = 0;
		break;
	}

	stream_free(stm);

	return (dw) ? 1 : 0;
}

static bool_t _xsl_parse_server_hello(xsl_t *pxsl, stream_t stm, dword_t total)
{
	byte_t num[2];
	byte_t* buf = NULL;
	dword_t len;
	unsigned short tag;

	tchar_t errinfo[ERR_LEN + 1] = { 0 };

	TRY_CATCH;

	while (total)
	{
		//tag
		len = 2;
		if(!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		md5_update(pxsl->md5, num, 2);
		sha1_update(pxsl->sha, num, 2);

		tag = GET_SWORD_NET(num, 0);

		//tag value length
		len = 2;
		if(!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		md5_update(pxsl->md5, num, 2);
		sha1_update(pxsl->sha, num, 2);

		len = GET_SWORD_NET(num, 0);

		//tag value
		buf = (byte_t*)xmem_alloc(len);
		if(!stream_read_bytes(stm, buf, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= len;

		md5_update(pxsl->md5, buf, len);
		sha1_update(pxsl->sha, buf, len);

		switch (tag)
		{
		case XSL_TAG_RANDOM: 
			if (len > XSL_RND_SIZE)
			{
				raise_user_error(_T("_xsl_parse_server_hello"), _T("invalidate random bytes"));
			}
			xmem_copy((void*)(pxsl->rnd_srv), (void*)buf, len);
			break;
		case XSL_TAG_CERTIFICATE:
			if (!pxsl->crt_pe)
			{
				pxsl->crt_pe = (x509_cert*)xmem_alloc(sizeof(x509_cert));
			}
			if (!len)
			{
				raise_user_error(_T("_xsl_parse_server_hello"), _T("parse server cert failed"));
			}
			if (C_OK != x509parse_crt(pxsl->crt_pe, buf, len))
			{
				raise_user_error(_T("_xsl_parse_server_hello"), _T("parse server cert failed"));
			}
			break;
		case XSL_TAG_ERROR:
#ifdef _UNICODE
			mbs_to_ucs(buf, len, errinfo, ERR_LEN);
#else
			len = (len < ERR_LEN)? len : ERR_LEN;
			a_xsncpy(errinfo, buf, len);
#endif
			raise_user_error(_T("_xsl_parse_server_hello"), errinfo);
			break;
		}

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _xsl_parse_server_exchange(xsl_t *pxsl, stream_t stm, dword_t total)
{
	byte_t num[2];
	byte_t* buf = NULL;
	dword_t len;
	unsigned short tag;

	md5_context* md5 = NULL;
	sha1_context* sha = NULL;

	byte_t hash[MD5_SIZE + SHA1_SIZE] = { 0 };
	byte_t *ps, *pe;

	tchar_t errinfo[ERR_LEN + 1] = { 0 };

	rsa_context* rsa;

	TRY_CATCH;

	while (total)
	{
		//tag
		len = 2;
		if(!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		md5_update(pxsl->md5, num, 2);
		sha1_update(pxsl->sha, num, 2);

		tag = GET_SWORD_NET(num, 0);

		//tag value length
		len = 2;
		if(!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		md5_update(pxsl->md5, num, 2);
		sha1_update(pxsl->sha, num, 2);

		len = GET_SWORD_NET(num, 0);

		//tag value
		buf = (byte_t*)xmem_alloc(len);
		if(!stream_read_bytes(stm, buf, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= len;

		md5_update(pxsl->md5, buf, len);
		sha1_update(pxsl->sha, buf, len);

		switch (tag)
		{
		case XSL_TAG_DHMPARAM: 
			ps = buf;
			pe = buf + len;
			dhm_read_params(pxsl->dhm, &ps, pe);
			
			md5 = (md5_context*)xmem_alloc(sizeof(md5_context));
			md5_starts(md5);
			md5_update(md5, pxsl->rnd_cli, XSL_RND_SIZE);
			md5_update(md5, pxsl->rnd_srv, XSL_RND_SIZE);
			md5_update(md5, buf, len);
			md5_finish(md5, hash);
			xmem_free(md5);
			md5 = NULL;

			sha = (sha1_context*)xmem_alloc(sizeof(sha1_context));
			sha1_starts(sha);
			sha1_update(sha, pxsl->rnd_cli, XSL_RND_SIZE);
			sha1_update(sha, pxsl->rnd_srv, XSL_RND_SIZE);
			sha1_update(sha, buf, len);
			sha1_finish(sha, hash + MD5_SIZE);
			xmem_free(sha);
			sha = NULL;

			break;
		case XSL_TAG_RSASIGN: 
			//use server public key to verify
			rsa = (pxsl->crt_pe)? &(pxsl->crt_pe->rsa) : NULL;
			if (!rsa)
			{
				raise_user_error(_T("_xsl_parse_server_key_exchange"), _T("invalid server public key"));
			}
			if (len != rsa->len)
			{
				raise_user_error(_T("_xsl_parse_server_key_exchange"), _T("invalid rsa sign bytes"));
			}
			if (C_OK != rsa_pkcs1_verify(rsa, XSL_RSA_PUBLIC, 0, MD5_SIZE + SHA1_SIZE, hash, buf))
			{
				raise_user_error(_T("_xsl_parse_server_key_exchange"), _T("rsa verify failed"));
			}

			break;
		case XSL_TAG_ERROR:
#ifdef _UNICODE
			mbs_to_ucs(buf, len, errinfo, ERR_LEN);
#else
			len = (len < ERR_LEN) ? len : ERR_LEN;
			a_xsncpy(errinfo, buf, len);
#endif
			raise_user_error(_T("_xsl_parse_server_key_exchange"), errinfo);
			break;
		}

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	if (buf)
		xmem_free(buf);

	if (md5)
		xmem_free(md5);

	if (sha)
		xmem_free(sha);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _xsl_parse_server_finished(xsl_t *pxsl, stream_t stm, dword_t total)
{
	byte_t num[2];
	byte_t* buf = NULL;
	dword_t len;
	unsigned short tag;

	byte_t hash[MD5_SIZE + SHA1_SIZE] = { 0 };
	byte_t prof[XSL_PRF_SIZE] = { 0 };
	byte_t hmac[32] = { 0 };

	tchar_t errinfo[ERR_LEN + 1] = { 0 };
	rsa_context* rsa;

	TRY_CATCH;

	stream_set_crypt(stm, NULL, _arc4_decrypt, (void*)pxsl->arc_dec);

	while (total)
	{
		//tag
		len = 2;
		if (!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		tag = GET_SWORD_NET(num, 0);

		//tag value length
		len = 2;
		if (!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		len = GET_SWORD_NET(num, 0);

		//tag value
		buf = (byte_t*)xmem_alloc(len);
		if (!stream_read_bytes(stm, buf, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= len;

		switch (tag)
		{
		case XSL_TAG_PROFILE:
			if (len != XSL_PRF_SIZE)
			{
				raise_user_error(NULL, NULL);
			}

			//finish handshake read
			md5_finish(pxsl->md5, hash);
			sha1_finish(pxsl->sha, hash + MD5_SIZE);

			//make client profile
			_xsl_hmac(pxsl->mst_key, XSL_MST_SIZE, XSL_STR_FINISHED, hash, MD5_SIZE + SHA1_SIZE, prof, XSL_PRF_SIZE);

			//compare server profile
			if (xmem_comp(buf, len, prof, XSL_PRF_SIZE) != 0)
			{
				raise_user_error(NULL, NULL);
			}

			if (pxsl->mac_len == MD5_SIZE)
			{
				md5_hmac(pxsl->mac_dec, pxsl->mac_len, buf, len, hmac);
			}
			else if (pxsl->mac_len == SHA1_SIZE)
			{
				sha1_hmac(pxsl->mac_dec, pxsl->mac_len, buf, len, hmac);
			}

			break;
		case XSL_TAG_RSASIGN:
			//use server public key to verify
			rsa = (pxsl->crt_pe)? &(pxsl->crt_pe->rsa) : NULL;
			if (!rsa)
			{
				raise_user_error(_T("_xsl_parse_server_finished"), _T("invalid server public key"));
			}
			if (C_OK != rsa_pkcs1_verify(rsa, XSL_RSA_PUBLIC, 0, pxsl->mac_len, hmac, buf))
			{
				raise_user_error(_T("_xsl_parse_server_finished"), _T("rsa verify failed"));
			}

			break;
		case XSL_TAG_ERROR:
#ifdef _UNICODE
			mbs_to_ucs(buf, len, errinfo, ERR_LEN);
#else
			len = (len < ERR_LEN) ? len : ERR_LEN;
			a_xsncpy(errinfo, buf, len);
#endif
			raise_user_error(_T("_xsl_parse_server_finished"), errinfo);
			break;

		}

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	stream_set_crypt(stm, NULL, NULL, NULL);

	END_CATCH;

	return 1;
ONERROR:

	stream_set_crypt(stm, NULL, NULL, NULL);

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _xsl_read_server_handshake(xsl_t *pxsl, int hs)
{
	byte_t hdr[XSL_HDR_SIZE] = { 0 };

	dword_t total = 0;
	dword_t len;

	int hs_type;
	int max_ver, min_ver;

	stream_t stm = NULL;
	bool_t rt;

	TRY_CATCH;

	stm = stream_alloc(pxsl->tcp);

	//hdr
	len = XSL_HDR_SIZE;
	rt = stream_read_bytes(stm, hdr, &len);
	if (!rt)
	{
		raise_user_error(NULL, NULL);
	}

	hs_type = GET_SWORD_NET(hdr, 0);
	max_ver = GET_BYTE(hdr, 2);
	min_ver = GET_BYTE(hdr, 3);
	total = GET_DWORD_NET(hdr, 4);

	if (hs_type != hs)
	{
		raise_user_error(_T("_xsl_read_server_handshake"), _T("invalid handshake"));
	}

	switch (hs)
	{
	case XSL_HS_SERVER_HELLO:
		rt = _xsl_parse_server_hello(pxsl, stm, total);
		break;
	case XSL_HS_SERVER_EXCHANGE:
		rt = _xsl_parse_server_exchange(pxsl, stm, total);
		break;
	case XSL_HS_FINISHED:
		rt = _xsl_parse_server_finished(pxsl, stm, total);
		break;
	default:
		rt = 0;
		break;
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return rt;
ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}


/***************************************************************************************************/
bool_t _xsl_handshake_client(xsl_t *pxsl)
{
	bool_t ret = 1;

	while (pxsl->state != XSL_HANDSHAKE_OVER && pxsl->state != XSL_HANDSHAKE_ERROR)
	{
		switch (pxsl->state)
		{
		case XSL_HELLO_REQUEST:
			pxsl->state = XSL_CLIENT_HELLO;
			break;

			/*
			*  ==>   ClientHello
			*/
		case XSL_CLIENT_HELLO:
			ret = _xsl_write_client_handshake(pxsl, XSL_HS_CLIENT_HELLO);
			pxsl->state = (ret) ? XSL_SERVER_HELLO : XSL_HANDSHAKE_ERROR;
			break;

			/*
			*  <==   ServerHello
			*        Certificate
			*      ( ServerKeyExchange  )
			*      ( CertificateRequest )
			*        ServerHelloDone
			*/
		case XSL_SERVER_HELLO:
			ret = _xsl_read_server_handshake(pxsl, XSL_HS_SERVER_HELLO);
			pxsl->state = (ret) ? XSL_SERVER_EXCHANGE : XSL_HANDSHAKE_ERROR;
			break;

		case XSL_SERVER_EXCHANGE:
			ret = _xsl_read_server_handshake(pxsl, XSL_HS_SERVER_EXCHANGE);
			pxsl->state = (ret) ? XSL_CLIENT_EXCHANGE : XSL_HANDSHAKE_ERROR;
			break;

			/*
			*  ==> ( Certificate/Alert  )
			*        ClientKeyExchange
			*      ( CertificateVerify  )
			*        ChangeCipherSpec
			*        Finished
			*/

		case XSL_CLIENT_EXCHANGE:
			ret = _xsl_write_client_handshake(pxsl, XSL_HS_CLIENT_EXCHANGE);
			pxsl->state = (ret) ? XSL_CLIENT_FINISHED : XSL_HANDSHAKE_ERROR;
			break;

		case XSL_CLIENT_FINISHED:
			ret = _xsl_write_client_handshake(pxsl, XSL_HS_FINISHED);
			pxsl->state = (ret) ? XSL_SERVER_FINISHED : XSL_HANDSHAKE_ERROR;
			break;

			/*
			*        Finished
			*/

		case XSL_SERVER_FINISHED:
			ret = _xsl_read_server_handshake(pxsl, XSL_HS_FINISHED);
			pxsl->state = (ret) ? XSL_HANDSHAKE_OVER : XSL_HANDSHAKE_ERROR;
			break;
		}
	}

	return (pxsl->state == XSL_HANDSHAKE_OVER) ? 1 : 0;
}

/****************************************************************************************************/
static bool_t _xsl_parse_client_hello(xsl_t *pxsl, stream_t stm, dword_t total)
{
	byte_t num[2];
	byte_t* buf = NULL;
	dword_t len;
	unsigned short tag;

	tchar_t errinfo[ERR_LEN + 1] = { 0 };
	int flag = 0;

	TRY_CATCH;

	while (total)
	{
		//tag
		len = 2;
		if(!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		tag = GET_SWORD_NET(num, 0);

		//tag value length
		len = 2;
		if(!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		len = GET_SWORD_NET(num, 0);

		//tag value
		buf = (byte_t*)xmem_alloc(len);
		if(!stream_read_bytes(stm, buf, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= len;

		switch (tag)
		{
		case XSL_TAG_RANDOM: 
			if (len > XSL_RND_SIZE)
			{
				raise_user_error(_T("_xsl_parse_client_hello"), _T("invalid random bytes"));
			}
			xmem_copy((void*)(pxsl->rnd_cli), (void*)buf, len);
			break;
		case XSL_TAG_CIPHER: 
			if (len == sizeof(short))
				pxsl->ciph = GET_SWORD_NET(buf, 0);
			else if (len == sizeof(long))
				pxsl->ciph = GET_DWORD_NET(buf, 0);
			else
			{
				raise_user_error(_T("_xsl_parse_client_hello"), _T("invalid cipher type"));
			}
			break;
		case XSL_TAG_CERTIFICATE:
			if (!pxsl->crt_pe)
			{
				pxsl->crt_pe = (x509_cert*)xmem_alloc(sizeof(x509_cert));
			}
			if (!len)
			{
				raise_user_error(_T("_xsl_parse_client_hello"), _T("parse client cert failed"));
			}
			if (C_OK != x509parse_crt(pxsl->crt_pe, buf, len))
			{
				raise_user_error(_T("_xsl_parse_client_hello"), _T("parse client cert failed"));
			}
			/*if (!pxsl->crt_ca)
			{
				raise_user_error(_T("_xsl_parse_client_hello"), _T("invalid server ca cert"));
			}
			if (!xcert_verify_x509(pxsl->crt_pe, pxsl->crt_ca, NULL, &flag))
			{
				raise_user_error(_T("_xsl_parse_client_hello"), _T("verify calient cert failed"));
			}*/
			break;
		case XSL_TAG_ERROR:
#ifdef _UNICODE
			mbs_to_ucs(buf, len, errinfo, ERR_LEN);
#else
			len = (len < ERR_LEN) ? len : ERR_LEN;
			a_xsncpy(errinfo, buf, len);
#endif
			raise_user_error(_T("_xsl_parse_client_hello"), errinfo);
			break;
		}

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	END_CATCH;

	return 1;
ONERROR:

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _xsl_parse_client_exchange(xsl_t *pxsl, stream_t stm, dword_t total)
{
	byte_t num[2];
	byte_t* buf = NULL;
	dword_t len;
	unsigned short tag;

	tchar_t errinfo[ERR_LEN + 1] = { 0 };
	dword_t dw = 0;

	TRY_CATCH;

	while (total)
	{
		//tag
		len = 2;
		if(!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		tag = GET_SWORD_NET(num, 0);

		//tag value length
		len = 2;
		if(!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		len = GET_SWORD_NET(num, 0);

		//tag value
		buf = (byte_t*)xmem_alloc(len);
		if(!stream_read_bytes(stm, buf, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= len;

		switch (tag)
		{
		case XSL_TAG_RSACRYPT: 
			//use server private key to decrypt
			if (!pxsl->rsa)
			{
				raise_user_error(_T("_xsl_parse_client_keyexchange"), _T("invalid server private key"));
			}
			if (len != pxsl->rsa->len)
			{
				raise_user_error(_T("_xsl_parse_client_keyexchange"), _T("invalid server private key"));
			}

			if (C_OK != rsa_pkcs1_decrypt(pxsl->rsa, XSL_RSA_PRIVATE, &dw, buf, pxsl->pre_mst))
			{
				raise_user_error(_T("_xsl_parse_client_keyexchange"), _T("rsa decrypt failed"));
			}

			if (dw != XSL_MST_SIZE)
			{
				raise_user_error(_T("_xsl_parse_client_keyexchange"), _T("invalid master random"));
			}

			break;
		case XSL_TAG_ERROR:
#ifdef _UNICODE
			mbs_to_ucs(buf, len, errinfo, ERR_LEN);
#else
			len = (len < ERR_LEN) ? len : ERR_LEN;
			a_xsncpy(errinfo, buf, len);
#endif
			raise_user_error(_T("_xsl_parse_client_keyexchange"), errinfo);
			break;
		}

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	///derive server keys
	_xsl_derive(pxsl);

	END_CATCH;

	return 1;
ONERROR:

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _xsl_parse_client_finished(xsl_t *pxsl, stream_t stm, dword_t total)
{
	byte_t num[2];
	byte_t* buf = NULL;
	dword_t len;
	unsigned short tag;

	byte_t prof[XSL_PRF_SIZE] = { 0 };
	byte_t hmac[32] = { 0 };
	byte_t hash[MD5_SIZE + SHA1_SIZE] = { 0 };

	tchar_t errinfo[ERR_LEN + 1] = { 0 };
	md5_context* xsl_md5 = NULL;
	sha1_context* xsl_sha = NULL;

	rsa_context* rsa;

	TRY_CATCH;

	stream_set_crypt(stm, NULL, _arc4_decrypt, (void*)pxsl->arc_dec);

	while (total)
	{
		//tag
		len = 2;
		if (!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		tag = GET_SWORD_NET(num, 0);

		//tag value length
		len = 2;
		if (!stream_read_bytes(stm, (void*)num, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= 2;

		len = GET_SWORD_NET(num, 0);

		//tag value
		buf = (byte_t*)xmem_alloc(len);
		if (!stream_read_bytes(stm, buf, &len))
		{
			raise_user_error(NULL, NULL);
		}
		total -= len;

		switch (tag)
		{
		case XSL_TAG_PROFILE:
			if (len != XSL_PRF_SIZE)
			{
				raise_user_error(_T("_xsl_parse_client_finished"), _T("invalid profile size"));
			}

			//finish handshake read
			xsl_md5 = (md5_context*)xmem_clone((void*)pxsl->md5, sizeof(md5_context));
			xsl_sha = (sha1_context*)xmem_clone((void*)pxsl->sha, sizeof(sha1_context));

			md5_finish(xsl_md5, hash);
			sha1_finish(xsl_sha, hash + MD5_SIZE);

			xmem_free(xsl_md5);
			xsl_md5 = NULL;
			xmem_free(xsl_sha);
			xsl_sha = NULL;

			//make server profile
			_xsl_hmac(pxsl->mst_key, XSL_MST_SIZE, XSL_STR_FINISHED, hash, MD5_SIZE + SHA1_SIZE, prof, XSL_PRF_SIZE);

			//compare server profile
			if (xmem_comp(buf, len, prof, XSL_PRF_SIZE) != 0)
			{
				raise_user_error(_T("_xsl_parse_client_finished"), _T("invalid profile"));
			}

			if (pxsl->mac_len == MD5_SIZE)
			{
				md5_hmac(pxsl->mac_dec, pxsl->mac_len, buf, len, hmac);
			}
			else if (pxsl->mac_len == SHA1_SIZE)
			{
				sha1_hmac(pxsl->mac_dec, pxsl->mac_len, buf, len, hmac);
			}

			break;
		case XSL_TAG_RSASIGN:
			//use client rsa public key to verify
			rsa = (pxsl->crt_pe)? &(pxsl->crt_pe->rsa) : NULL;
			if (!rsa)
			{
				raise_user_error(_T("_xsl_parse_client_finished"), _T("invalid client public key"));
			}

			if (C_OK != rsa_pkcs1_verify(rsa, XSL_RSA_PUBLIC, 0, pxsl->mac_len, hmac, buf))
			{
				raise_user_error(_T("_xsl_parse_client_finished"), _T("rsa verify failed"));
			}
			break;
		case XSL_TAG_ERROR:
#ifdef _UNICODE
			mbs_to_ucs(buf, len, errinfo, ERR_LEN);
#else
			len = (len < ERR_LEN) ? len : ERR_LEN;
			a_xsncpy(errinfo, buf, len);
#endif
			raise_user_error(_T("_xsl_parse_client_finished"), errinfo);
			break;
		}

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	stream_set_crypt(stm, NULL, NULL, NULL);

	END_CATCH;

	return 1;
ONERROR:

	stream_set_crypt(stm, NULL, NULL, NULL);

	if (buf)
		xmem_free(buf);

	if (xsl_md5)
		xmem_free(xsl_md5);

	if (xsl_sha)
		xmem_free(xsl_sha);

	XDL_TRACE_LAST;

	return 0;
}

static bool_t _xsl_read_client_handshake(xsl_t *pxsl, int hs)
{
	byte_t hdr[XSL_HDR_SIZE] = { 0 };

	dword_t total = 0;
	dword_t len;

	int hs_type;
	int max_ver, min_ver;

	stream_t stm = NULL;
	bool_t rt;

	TRY_CATCH;

	stm = stream_alloc(pxsl->tcp);

	//hdr
	len = XSL_HDR_SIZE;
	rt = stream_read_bytes(stm, hdr, &len);
	if (!rt)
	{
		raise_user_error(NULL, NULL);
	}

	hs_type = GET_SWORD_NET(hdr, 0);
	max_ver = GET_BYTE(hdr, 2);
	min_ver = GET_BYTE(hdr, 3);
	total = GET_DWORD_NET(hdr, 4);

	if (hs_type != hs)
	{
		raise_user_error(_T("_xsl_read_client_handshake"), _T("invalid handshake"));
	}

	switch (hs)
	{
	case XSL_HS_CLIENT_HELLO:
		rt = _xsl_parse_client_hello(pxsl, stm, total);
		break;
	case XSL_HS_CLIENT_EXCHANGE:
		rt = _xsl_parse_client_exchange(pxsl, stm, total);
		break;
	case XSL_HS_FINISHED:
		rt = _xsl_parse_client_finished(pxsl, stm, total);
		break;
	default:
		rt = 0;
		break;
	}

	stream_free(stm);
	stm = NULL;

	END_CATCH;

	return rt;
ONERROR:

	if (stm)
		stream_free(stm);

	XDL_TRACE_LAST;

	return 0;
}

static dword_t _xsl_format_server_hello(xsl_t *pxsl, stream_t stm)
{
	dword_t total = 0;
	byte_t num[2];
	byte_t* buf = NULL;
	unsigned short tag;
	unsigned short len;

	x509_cert* crt;

	TRY_CATCH;

	tag = XSL_TAG_RANDOM;
	while (tag)
	{
		//tag
		if (stm)
		{
			PUT_SWORD_NET(num, 0, tag);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}

			md5_update(pxsl->md5, num, 2);
			sha1_update(pxsl->sha, num, 2);
		}
		total += 2;

		switch (tag)
		{
		case XSL_TAG_RANDOM:
			len = XSL_RND_SIZE;
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);
				xmem_copy((void*)buf, (void*)pxsl->rnd_srv, len);
			}
			crt = pxsl->crt_ow;
			tag = (crt && crt->next) ? XSL_TAG_CERTIFICATE : 0;
			break;
		case XSL_TAG_CERTIFICATE:
			len = crt->raw.len;
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);
				xmem_copy((void*)buf, (void*)crt->raw.p, len);
			}
			crt = crt->next;
			tag = (crt && crt->next) ? XSL_TAG_CERTIFICATE : 0;
			break;
		}

		//tag length
		if (stm)
		{
			PUT_SWORD_NET(num, 0, len);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}

			md5_update(pxsl->md5, num, 2);
			sha1_update(pxsl->sha, num, 2);
		}
		total += 2;

		//tag value
		if (stm)
		{
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}

			md5_update(pxsl->md5, buf, len);
			sha1_update(pxsl->sha, buf, len);
		}
		total += len;

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	END_CATCH;

	return total;
ONERROR:

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static dword_t _xsl_format_server_exchange(xsl_t *pxsl, stream_t stm)
{
	dword_t total = 0;
	byte_t num[2];
	byte_t* buf = NULL;
	unsigned short tag;
	unsigned short len;

	dword_t dw = 0;
	byte_t hash[MD5_SIZE + SHA1_SIZE] = { 0 };

	md5_context* md5 = NULL;
	sha1_context* sha = NULL;

	TRY_CATCH;

	tag = XSL_TAG_DHMPARAM;
	while (tag)
	{
		//tag
		if (stm)
		{
			PUT_SWORD_NET(num, 0, tag);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}

			md5_update(pxsl->md5, num, 2);
			sha1_update(pxsl->sha, num, 2);
		}
		total += 2;

		switch (tag)
		{
		case XSL_TAG_DHMPARAM:
			buf = (byte_t*)xmem_alloc(1024);
			dhm_make_params(pxsl->dhm, 1024, buf, &dw, havege_rand, pxsl->rng);
			len = dw;

			if (stm)
			{
				md5 = (md5_context*)xmem_alloc(sizeof(md5_context));
				md5_starts(md5);
				md5_update(md5, pxsl->rnd_cli, XSL_RND_SIZE);
				md5_update(md5, pxsl->rnd_srv, XSL_RND_SIZE);
				md5_update(md5, buf, dw);
				md5_finish(md5, hash);
				xmem_free(md5);
				md5 = NULL;

				sha = (sha1_context*)xmem_alloc(sizeof(sha1_context));
				sha1_starts(sha);
				sha1_update(sha, pxsl->rnd_cli, XSL_RND_SIZE);
				sha1_update(sha, pxsl->rnd_srv, XSL_RND_SIZE);
				sha1_update(sha, buf, dw);
				sha1_finish(sha, hash + MD5_SIZE);
				xmem_free(sha);
				sha = NULL;
			}
			tag = XSL_TAG_RSASIGN;
			break;
		case XSL_TAG_RSASIGN:
			//use server private key for sign
			len = (pxsl->rsa) ? (pxsl->rsa->len) : 0;
			if (!len)
			{
				raise_user_error(_T("_xsl_format_server_exchange"), _T("invalid server private key"));
			}
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);
				if (C_OK != rsa_pkcs1_sign(pxsl->rsa, XSL_RSA_PRIVATE, 0, MD5_SIZE + SHA1_SIZE, hash, buf))
				{
					raise_user_error(_T("_xsl_format_server_exchange"), _T("rsa sign failed"));
				}
			}
			tag = 0;
			break;

		}

		//tag length
		if (stm)
		{
			PUT_SWORD_NET(num, 0, len);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}

			md5_update(pxsl->md5, num, 2);
			sha1_update(pxsl->sha, num, 2);
		}
		total += 2;

		//tag value
		if (stm)
		{
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}

			md5_update(pxsl->md5, buf, len);
			sha1_update(pxsl->sha, buf, len);
		}
		total += len;

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	END_CATCH;

	return total;
ONERROR:

	if (buf)
		xmem_free(buf);

	if (md5)
		xmem_free(md5);

	if (sha)
		xmem_free(sha);

	XDL_TRACE_LAST;

	return 0;
}

static dword_t _xsl_format_server_finished(xsl_t *pxsl, stream_t stm)
{
	dword_t total = 0;
	byte_t num[2];
	byte_t* buf = NULL;
	unsigned short tag;
	unsigned short len;

	byte_t prof[XSL_PRF_SIZE] = { 0 };
	byte_t hmac[32] = { 0 };
	byte_t hash[MD5_SIZE + SHA1_SIZE] = { 0 };

	TRY_CATCH;

	if (stm)
	{
		stream_set_crypt(stm, _arc4_encrypt, NULL, (void*)pxsl->arc_enc);
	}

	tag = XSL_TAG_PROFILE;
	while (tag)
	{
		//tag
		if (stm)
		{
			PUT_SWORD_NET(num, 0, tag);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		switch (tag)
		{
		case XSL_TAG_PROFILE:
			if (stm)
			{
				//finish handshake read
				md5_finish(pxsl->md5, hash);
				sha1_finish(pxsl->sha, hash + MD5_SIZE);
			}
			//make server profile
			len = XSL_PRF_SIZE;
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);

				_xsl_hmac(pxsl->mst_key, XSL_MST_SIZE, XSL_STR_FINISHED, hash, MD5_SIZE + SHA1_SIZE, buf, len);

				if (pxsl->mac_len == MD5_SIZE)
					md5_hmac(pxsl->mac_enc, pxsl->mac_len, buf, len, hmac);
				else if (pxsl->mac_len == SHA1_SIZE)
					sha1_hmac(pxsl->mac_enc, pxsl->mac_len, buf, len, hmac);
			}
			tag = XSL_TAG_RSASIGN;
			break;
		case XSL_TAG_RSASIGN:
			//use server private key for sign
			len = (pxsl->rsa) ? (pxsl->rsa->len) : 0;
			if (!len)
			{
				raise_user_error(_T("_xsl_format_server_finished"), _T("invalid server private key"));
			}
			if (stm)
			{
				buf = (byte_t*)xmem_alloc(len);

				if (C_OK != rsa_pkcs1_sign(pxsl->rsa, XSL_RSA_PRIVATE, 0, pxsl->mac_len, hmac, buf))
				{
					raise_user_error(_T("_xsl_format_server_finished"), _T("rsa sign failed"));
				}
			}
			tag = 0;
			break;
		}


		//tag length
		if (stm)
		{
			PUT_SWORD_NET(num, 0, len);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		//tag value
		if (stm)
		{
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += len;

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	if (stm)
	{
		stream_set_crypt(stm, NULL, NULL, NULL);
	}

	END_CATCH;

	return total;
ONERROR:

	if (stm)
	{
		stream_set_crypt(stm, NULL, NULL, NULL);
	}

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

static dword_t _xsl_format_server_error(xsl_t *pxsl, stream_t stm)
{
	dword_t total = 0;
	byte_t num[2];
	byte_t* buf = NULL;
	unsigned short tag;
	unsigned short len;

	TRY_CATCH;

	tag = XSL_TAG_ERROR;
	while (tag)
	{
		//tag
		if (stm)
		{
			PUT_SWORD_NET(num, 0, tag);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		switch (tag)
		{
		case XSL_TAG_ERROR:
			buf = a_xsclone("server error");
			len = a_xslen(buf);

			tag = 0;
			break;
		}

		//tag length
		if (stm)
		{
			PUT_SWORD_NET(num, 0, len);
			if (!stream_write_bytes(stm, num, 2))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += 2;

		//tag value
		if (stm)
		{
			if (!stream_write_bytes(stm, buf, len))
			{
				raise_user_error(NULL, NULL);
			}
		}
		total += len;

		if (buf)
		{
			xmem_free(buf);
			buf = NULL;
		}
	}

	END_CATCH;

	return total;
ONERROR:

	if (buf)
		xmem_free(buf);

	XDL_TRACE_LAST;

	return 0;
}

bool_t _xsl_write_server_handshake(xsl_t* pxsl, int hs)
{
	byte_t hdr[XSL_HDR_SIZE] = { 0 };

	dword_t dw = 0;
	stream_t stm = NULL;

	switch (hs)
	{
	case XSL_HS_SERVER_HELLO:
		dw = _xsl_format_server_hello(pxsl, NULL);
		break;
	case XSL_HS_SERVER_EXCHANGE:
		dw = _xsl_format_server_exchange(pxsl, NULL);
		break;
	case XSL_HS_FINISHED:
		dw = _xsl_format_server_finished(pxsl, NULL);
		break;
	case XSL_HS_ERROR:
		dw = _xsl_format_server_error(pxsl, NULL);
		break;
	default:
		dw = 0;
		break;
	}

	if (!dw)
		return 0;

	/*header block*/
	PUT_SWORD_NET(hdr, 0, hs);
	PUT_BYTE(hdr, 2, XSL_MAJOR_VERSION_3);
	PUT_BYTE(hdr, 3, XSL_MINOR_VERSION_1);
	PUT_DWORD_NET(hdr, 4, dw);

	stm = stream_alloc(pxsl->tcp);

	if (!stream_write_bytes(stm, hdr, XSL_HDR_SIZE))
	{
		stream_free(stm);
		return 0;
	}

	switch (hs)
	{
	case XSL_HS_SERVER_HELLO:
		dw = _xsl_format_server_hello(pxsl, stm);
		break;
	case XSL_HS_SERVER_EXCHANGE:
		dw = _xsl_format_server_exchange(pxsl, stm);
		break;
	case XSL_HS_FINISHED:
		dw = _xsl_format_server_finished(pxsl, stm);
		break;
	case XSL_HS_ERROR:
		dw = _xsl_format_server_error(pxsl, stm);
		break;
	default:
		dw = 0;
		break;
	}

	stream_free(stm);

	return (dw) ? 1 : 0;
}

bool_t _xsl_handshake_server(xsl_t *pxsl)
{
	bool_t ret = 1;

	while (pxsl->state != XSL_HANDSHAKE_OVER && pxsl->state != XSL_HANDSHAKE_ERROR)
	{
		switch (pxsl->state)
		{
		case XSL_HELLO_REQUEST:
			pxsl->state = XSL_CLIENT_HELLO;
			break;

			/*
			*  <==   ClientHello
			*/
		case XSL_CLIENT_HELLO:
			ret = _xsl_read_client_handshake(pxsl, XSL_HS_CLIENT_HELLO);
			pxsl->state = (ret) ? XSL_SERVER_HELLO : XSL_HANDSHAKE_ERROR;
			break;

			/*
			*  ==>   ServerHello
			*        Certificate
			*      ( ServerKeyExchange  )
			*      ( CertificateRequest )
			*        ServerHelloDone
			*/
		case XSL_SERVER_HELLO:
			ret = _xsl_write_server_handshake(pxsl, XSL_HS_SERVER_HELLO);
			pxsl->state = (ret) ? XSL_SERVER_EXCHANGE : XSL_HANDSHAKE_ERROR;
			break;

		case XSL_SERVER_EXCHANGE:
			ret = _xsl_write_server_handshake(pxsl, XSL_HS_SERVER_EXCHANGE);
			pxsl->state = (ret) ? XSL_CLIENT_EXCHANGE : XSL_HANDSHAKE_ERROR;
			break;

			/*
			*  <== ( Certificate/Alert  )
			*        ClientKeyExchange
			*      ( CertificateVerify  )
			*        ChangeCipherSpec
			*        Finished
			*/

		case XSL_CLIENT_EXCHANGE:
			ret = _xsl_read_client_handshake(pxsl, XSL_HS_CLIENT_EXCHANGE);
			pxsl->state = (ret) ? XSL_CLIENT_FINISHED : XSL_HANDSHAKE_ERROR;
			break;

		case XSL_CLIENT_FINISHED:
			ret = _xsl_read_client_handshake(pxsl, XSL_HS_FINISHED);
			pxsl->state = (ret) ? XSL_SERVER_FINISHED : XSL_HANDSHAKE_ERROR;
			break;

			/*
			*        Finished
			*/

		case XSL_SERVER_FINISHED:
			ret = _xsl_write_server_handshake(pxsl, XSL_HS_FINISHED);
			pxsl->state = (ret) ? XSL_HANDSHAKE_OVER : XSL_HANDSHAKE_ERROR;
			break;
		}
	}

	return (pxsl->state == XSL_HANDSHAKE_OVER) ? 1 : 0;
}

bool_t _xsl_write_data(xsl_t* pxsl, const byte_t* buf, dword_t *pb)
{
	stream_t stm = NULL;
	bool_t rt;

	if (!(*pb))
		return 1;

	stm = stream_alloc(pxsl->tcp);

	stream_set_crypt(stm, _arc4_encrypt, NULL, (void*)pxsl->arc_enc);

	rt = stream_write_bytes(stm, buf, *pb);
	
	stream_free(stm);

	return rt;
}

bool_t _xsl_read_data(xsl_t* pxsl, byte_t* buf, dword_t *pb)
{
	stream_t stm = NULL;
	bool_t rt;

	if (!(*pb))
		return 1;

	stm = stream_alloc(pxsl->tcp);

	stream_set_crypt(stm, NULL, _arc4_decrypt, (void*)pxsl->arc_dec);

	rt = stream_read_bytes(stm, buf, pb);
	
	stream_free(stm);

	return rt;
}

/*********************************************************************************************************/

xhand_t xxsl_cli(unsigned short port, const tchar_t* addr)
{
	xsl_t* pso;
	xhand_t tcp;
	
	tcp = xtcp_cli(port, addr);
	if (!tcp)
		return NULL;

	pso = (xsl_t*)xmem_alloc(sizeof(xsl_t));
	pso->head.tag = _HANDLE_XSL;

	pso->tcp = tcp;
	pso->type = XSL_TYPE_CLIENT;

	_xsl_init(pso);

	return (xhand_t)pso;
}

xhand_t xxsl_srv(res_file_t so)
{
	xsl_t* pso;
	xhand_t tcp;

	tcp = xtcp_srv(so);
	if (!tcp)
		return NULL;

	pso = (xsl_t*)xmem_alloc(sizeof(xsl_t));
	pso->head.tag = _HANDLE_XSL;

	pso->tcp = tcp;
	pso->type = XSL_TYPE_SERVER;

	_xsl_init(pso);

	return (xhand_t)pso;
}

void  xxsl_close(xhand_t xsl)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	XDL_ASSERT(pso->type == XSL_TYPE_CLIENT || pso->type == XSL_TYPE_SERVER);

	xtcp_close(pso->tcp);

	_xsl_uninit(pso);

	xmem_free(pso);
}

res_file_t xxsl_socket(xhand_t xsl)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	return (pso->tcp)? xtcp_socket(pso->tcp) : 0;
}

int xxsl_type(xhand_t xsl)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	return pso->type;
}

unsigned short xxsl_addr_port(xhand_t xsl, tchar_t* addr)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	return xtcp_addr_port(pso->tcp, addr);
}

unsigned short xxsl_peer_port(xhand_t xsl, tchar_t* addr)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	return xtcp_peer_port(pso->tcp, addr);
}

bool_t xxsl_write(xhand_t xsl, const byte_t* buf, dword_t* pb)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	if (pso->type == XSL_TYPE_CLIENT && !_xsl_handshake_client(pso))
	{
		return 0;
	}

	if (pso->type == XSL_TYPE_SERVER && !_xsl_handshake_server(pso))
	{
		return 0;
	}

	return _xsl_write_data(pso, buf, pb);
}

bool_t xxsl_read(xhand_t xsl, byte_t* buf, dword_t* pb)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	if (pso->type == XSL_TYPE_CLIENT && !_xsl_handshake_client(pso))
	{
		return 0;
	}

	if (pso->type == XSL_TYPE_SERVER && !_xsl_handshake_server(pso))
	{
		return 0;
	}

	return _xsl_read_data(pso, buf, pb);
}

void xxsl_set_send_buff(xhand_t xsl, dword_t dw)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	xtcp_set_send_buff(pso->tcp, dw);
}

void xxsl_set_recv_buff(xhand_t xsl, dword_t dw)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	xtcp_set_recv_buff(pso->tcp, dw);
}

void xxsl_set_linger(xhand_t xsl, bool_t b_wait, int n_sec)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	xtcp_set_linger(pso->tcp, b_wait, n_sec);
}

bool_t xxsl_set_ca(xhand_t xsl, const byte_t* sz_cert, dword_t clen)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	XDL_ASSERT(pso->type == XSL_TYPE_CLIENT || pso->type == XSL_TYPE_SERVER);

	if (!pso->crt_ca)
		pso->crt_ca = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	if (!x509parse_crt(pso->crt_ca, sz_cert, clen))
	{
		return 0;
	}

	return 1;
}

bool_t xxsl_set_cert(xhand_t xsl, const byte_t* sz_cert, dword_t clen)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	XDL_ASSERT(pso->type == XSL_TYPE_CLIENT || pso->type == XSL_TYPE_SERVER);

	if (!pso->crt_ow)
		pso->crt_ow = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	if (!x509parse_crt(pso->crt_ow, sz_cert, clen))
	{
		return 0;
	}

	return 1;
}

bool_t xxsl_set_rsa(xhand_t xsl, const byte_t* sz_rsa, dword_t rlen, const byte_t* sz_pwd, dword_t plen)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	XDL_ASSERT(pso->type == XSL_TYPE_CLIENT || pso->type == XSL_TYPE_SERVER);

	if (!pso->rsa)
		pso->rsa = (rsa_context*)xmem_alloc(sizeof(rsa_context));

	if (!x509parse_key(pso->rsa, sz_rsa, rlen, sz_pwd, plen))
	{
		return 0;
	}

	return 1;
}

bool_t xxsl_set_dhm(xhand_t xsl, const byte_t *dhm_P, const byte_t *dhm_G)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	XDL_ASSERT(pso->type == XSL_TYPE_CLIENT || pso->type == XSL_TYPE_SERVER);

	if (!pso->dhm)
		pso->dhm = (dhm_context*)xmem_alloc(sizeof(dhm_context));

	if (dhm_P)
		mpi_read_string(&(pso->dhm->P), 16, dhm_P, -1);
	if (dhm_G)
		mpi_read_string(&(pso->dhm->G), 16, dhm_G, -1);

	return 1;
}

bool_t xxsl_set_ciph(xhand_t xsl, int ciph)
{
	xsl_t* pso = (xsl_t*)xsl;

	XDL_ASSERT(xsl && xsl->tag == _HANDLE_XSL);

	XDL_ASSERT(pso->type == XSL_TYPE_CLIENT || pso->type == XSL_TYPE_SERVER);

	if (ciph != XSL_RSA_RC4_128_SHA && ciph != XSL_RSA_RC4_128_MD5)
		return 0;

	pso->ciph = ciph;

	return 1;
}

#endif //XDK_SUPPORT_SOCK
