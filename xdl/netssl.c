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
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_CRYPT)


#define GET_THREEBYTE_LEN(buf,off)	((buf[off] << 16) | (buf[off+1] << 8) | buf[off+2])
#define PUT_THREEBYTE_LEN(buf,off,n)	{buf[off] = (unsigned char)((n) >> 16);buf[off+1] = (unsigned char)((n) >> 8);buf[off+2] = (unsigned char)((n));}


static char dhm_G[] = "4";
static char dhm_P[] = "E4004C1F94182000103D883A448B3F802CE4B44A83301270002C20D0321CFD00" \
"11CCEF784C26A400F43DFB901BCA7538F2C6B176001CF5A0FD16D2C48B1D0C1C" \
"F6AC8E1DA6BCC3B4E1F96B0564965300FFA1D0B601EB2800F489AA512C4B248C" \
"01F76949A60BB7F00A40B1EAB64BDD48E8A700D60B7F1200FA8E77B0A979DABF";

static int _default_ciphers[] =
{
	SSL_EDH_RSA_AES_256_SHA,
	SSL_EDH_RSA_DES_168_SHA,
	SSL_RSA_AES_128_SHA,
	SSL_RSA_AES_256_SHA,
	SSL_RSA_DES_168_SHA,
	SSL_RSA_RC4_128_SHA,
	SSL_RSA_RC4_128_MD5,
	0
};

typedef struct _ssl_t{
	xhand_head head;

	int type;
	xhand_t tcp;

	int cli_auth;
	int srv_mode;

	byte_t rnd_srv[SSL_RND_SIZE];
	byte_t rnd_cli[SSL_RND_SIZE];
	byte_t pmsmst[SSL_BLK_SIZE];
	int pmslen;

	int* set_ciphers;
	schar_t* host_cn;
	schar_t* peer_cn;

	x509_cert* crt_ca;
	x509_cert* crt_pe;
	x509_cert* crt_ow;
	rsa_context* rsa_ow;
	dhm_context* dhm_ow;

	havege_state rng;
	md5_context md5;
	sha1_context sha1;

	int key_len;
	int mac_len;
	int min_len;
	int iv_len;
	byte_t iv_enc[SSL_IVC_SIZE]; 
	byte_t iv_dec[SSL_IVC_SIZE]; 
	byte_t mac_enc[SSL_MAC_SIZE];
	byte_t mac_dec[SSL_MAC_SIZE]; 
	dword_t ctx_enc[SSL_CTX_SIZE];
	dword_t ctx_dec[SSL_CTX_SIZE];

	int ses_over;
	bool_t ses_crypt;
	bool_t ses_resume;
	int ses_ciph;
	int ses_size;
	byte_t ses_id[SSL_SES_SIZE];
	byte_t ses_mst[SSL_MST_SIZE];

	int major_ver;
	int minor_ver;
	int max_major_ver;
	int max_minor_ver;

	byte_t* snd_pkg;
	int snd_msg_type;
	int snd_msg_len;
	int snd_msg_pop;
	byte_t* snd_ctr;
	byte_t* snd_hdr;
	byte_t* snd_msg;

	byte_t* rcv_pkg;
	int rcv_msg_type;
	int rcv_msg_len;
	int rcv_msg_pop;
	byte_t* rcv_ctr;
	byte_t* rcv_hdr;
	byte_t* rcv_msg;

	int nb_zero;
}ssl_t;

/***********************************************************************************************************************************/

static void _ssl_hmac(unsigned char *secret, int slen, char *label, unsigned char *random, int rlen, unsigned char *dstbuf, int dlen)
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

	// First compute P_md5(secret,label+random)[0..dlen]
	md5_hmac(S1, hs, tmp + 20, nb, 4 + tmp);

	for (i = 0; i < dlen; i += 16)
	{
		md5_hmac(S1, hs, 4 + tmp, 16 + nb, h_i);
		md5_hmac(S1, hs, 4 + tmp, 16, 4 + tmp);

		k = (i + 16 > dlen) ? dlen % 16 : 16;

		for (j = 0; j < k; j++)
			dstbuf[i + j] = h_i[j];
	}

	// XOR out with P_sha1(secret,label+random)[0..dlen]
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

static void _ssl_mac_md5(unsigned char *secret, unsigned char *ctr, unsigned char *msg, int len, int type, unsigned char out[16])
{
	unsigned char header[11];
	unsigned char padding[48];
	md5_context md5;

	xmem_copy(header, ctr, 8);
	PUT_BYTE(header, 8, (byte_t)type);
	PUT_SWORD_NET(header, 9, (unsigned short)len);

	xmem_set(padding, 0x36, 48);
	md5_starts(&md5);
	md5_update(&md5, secret, 16);
	md5_update(&md5, padding, 48);
	md5_update(&md5, header, 11);
	md5_update(&md5, msg, len);
	md5_finish(&md5, out);

	xmem_set(padding, 0x5C, 48);
	md5_starts(&md5);
	md5_update(&md5, secret, 16);
	md5_update(&md5, padding, 48);
	md5_update(&md5, out, 16);
	md5_finish(&md5, out);
}

static void _ssl_mac_sha1(unsigned char *secret, unsigned char *ctr, unsigned char *buf, int len, int type, unsigned char out[20])
{
	unsigned char header[11];
	unsigned char padding[40];
	sha1_context sha1;

	xmem_copy(header, ctr, 8);
	PUT_BYTE(header, 8, (byte_t)type);
	PUT_SWORD_NET(header, 9, (unsigned short)len);

	xmem_set(padding, 0x36, 40);
	sha1_starts(&sha1);
	sha1_update(&sha1, secret, 20);
	sha1_update(&sha1, padding, 40);
	sha1_update(&sha1, header, 11);
	sha1_update(&sha1, buf, len);
	sha1_finish(&sha1, out);

	xmem_set(padding, 0x5C, 40);
	sha1_starts(&sha1);
	sha1_update(&sha1, secret, 20);
	sha1_update(&sha1, padding, 40);
	sha1_update(&sha1, out, 20);
	sha1_finish(&sha1, out);
}

static void _ssl_init(ssl_t* pssl)
{
	havege_init(&pssl->rng);
	md5_starts(&pssl->md5);
	sha1_starts(&pssl->sha1);

	pssl->set_ciphers = _default_ciphers;
	pssl->ses_ciph = SSL_RSA_RC4_128_SHA;

	pssl->rcv_pkg = (unsigned char *)xmem_alloc(SSL_MAX_SIZE);
	pssl->rcv_ctr = pssl->rcv_pkg;
	pssl->rcv_hdr = pssl->rcv_pkg + SSL_CTR_SIZE;
	pssl->rcv_msg = pssl->rcv_pkg + SSL_CTR_SIZE + SSL_HDR_SIZE;

	pssl->snd_pkg = (unsigned char *)xmem_alloc(SSL_MAX_SIZE);
	pssl->snd_ctr = pssl->snd_pkg;
	pssl->snd_hdr = pssl->snd_pkg + SSL_CTR_SIZE;
	pssl->snd_msg = pssl->snd_pkg + SSL_CTR_SIZE + SSL_HDR_SIZE;
}

static void _ssl_uninit(ssl_t* pssl)
{
	if (pssl->rcv_pkg)
	{
		xmem_free(pssl->rcv_pkg);
	}
	if (pssl->snd_pkg)
	{
		xmem_free(pssl->snd_pkg);
	}
	if (pssl->rsa_ow)
	{
		rsa_free(pssl->rsa_ow);
		xmem_free(pssl->rsa_ow);
	}
	if (pssl->crt_ow)
	{
		x509_free(pssl->crt_ow);
		xmem_free(pssl->crt_ow);
	}
	if (pssl->crt_pe)
	{
		x509_free(pssl->crt_pe);
		xmem_free(pssl->crt_pe);
	}
	if (pssl->crt_ca)
	{
		x509_free(pssl->crt_ca);
		xmem_free(pssl->crt_ca);
	}
	if (pssl->dhm_ow)
	{
		dhm_free(pssl->dhm_ow);
		xmem_free(pssl->dhm_ow);
	}
}

static void _ssl_derive_keys(ssl_t *pssl)
{
	unsigned char rndb[SSL_RND_SIZE * 2] = { 0 };
	unsigned char keyblk[256] = { 0 };

	unsigned char padding[16] = { 0 };
	unsigned char sha1sum[20] = { 0 };
	int i;
	byte_t *key_enc, *key_dec;

	sha1_context sha1 = { 0 };
	md5_context md5 = {0};

	if (!pssl->ses_resume)
	{
		xmem_copy((void*)rndb, (void*)pssl->rnd_cli, SSL_RND_SIZE);
		xmem_copy((void*)(rndb + SSL_RND_SIZE), (void*)pssl->rnd_srv, SSL_RND_SIZE);

		// generate session master secret
		if (pssl->minor_ver == SSL_MINOR_VERSION_0)
		{
			for (i = 0; i < 3; i++)
			{
				xmem_set((void*)padding, 'A' + i, 1 + i);

				sha1_starts(&sha1);
				sha1_update(&sha1, padding, 1 + i);
				sha1_update(&sha1, pssl->pmsmst, pssl->pmslen);
				sha1_update(&sha1, rndb, SSL_RND_SIZE * 2);
				sha1_finish(&sha1, sha1sum);

				md5_starts(&md5);
				md5_update(&md5, pssl->pmsmst, pssl->pmslen);
				md5_update(&md5, sha1sum, 20);
				md5_finish(&md5, pssl->ses_mst + i * 16);
			}
		}
		else
		{
			_ssl_hmac(pssl->pmsmst, pssl->pmslen, "master secret", rndb, SSL_RND_SIZE * 2, pssl->ses_mst, SSL_MST_SIZE);
		}

		xmem_zero(pssl->pmsmst, sizeof(pssl->pmsmst));
		pssl->pmslen = 0;
	}

	// swap the client and server random values.
	xmem_copy((void*)rndb, (void*)pssl->rnd_srv, SSL_RND_SIZE);
	xmem_copy((void*)(rndb + SSL_RND_SIZE), (void*)pssl->rnd_cli, SSL_RND_SIZE);

	xmem_zero(pssl->rnd_cli, sizeof(pssl->rnd_cli));
	xmem_zero(pssl->rnd_srv, sizeof(pssl->rnd_srv));

	// generate key block
	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_zero(&md5, sizeof(md5_context));
		xmem_zero(&sha1, sizeof(sha1_context));

		for (i = 0; i < 16; i++)
		{
			xmem_set(padding, 'A' + i, 1 + i);

			sha1_starts(&sha1);
			sha1_update(&sha1, padding, 1 + i);
			sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
			sha1_update(&sha1, rndb, SSL_RND_SIZE * 2);
			sha1_finish(&sha1, sha1sum);

			md5_starts(&md5);
			md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
			md5_update(&md5, sha1sum, 20);
			md5_finish(&md5, keyblk + i * 16);
		}
	}
	else
	{
		_ssl_hmac(pssl->ses_mst, SSL_MST_SIZE, "key expansion", rndb, SSL_RND_SIZE * 2, keyblk, 256);
	}

	switch (pssl->ses_ciph)
	{
	case SSL_RSA_RC4_128_MD5:
		pssl->key_len = 16; pssl->min_len = 16;
		pssl->iv_len = 0; pssl->mac_len = 16;
		break;

	case SSL_RSA_RC4_128_SHA:
		pssl->key_len = 16; pssl->min_len = 20;
		pssl->iv_len = 0; pssl->mac_len = 20;
		break;
	case SSL_RSA_DES_168_SHA:
	case SSL_EDH_RSA_DES_168_SHA:
		pssl->key_len = 24; pssl->min_len = 24;
		pssl->iv_len = 8; pssl->mac_len = 20;
		break;
	case SSL_RSA_AES_128_SHA:
		pssl->key_len = 16; pssl->min_len = 32;
		pssl->iv_len = 16; pssl->mac_len = 20;
		break;

	case SSL_RSA_AES_256_SHA:
	case SSL_EDH_RSA_AES_256_SHA:
		pssl->key_len = 32; pssl->min_len = 32;
		pssl->iv_len = 16; pssl->mac_len = 20;
		break;
	}

	if (pssl->type == SSL_TYPE_CLIENT)
	{
		xmem_copy(pssl->mac_enc, keyblk, pssl->mac_len);
		xmem_copy(pssl->mac_dec, keyblk + pssl->mac_len, pssl->mac_len);

		xmem_copy(pssl->iv_enc, keyblk + pssl->mac_len * 2 + pssl->key_len * 2, pssl->iv_len);
		xmem_copy(pssl->iv_dec, keyblk + pssl->mac_len * 2 + pssl->key_len * 2 + pssl->iv_len, pssl->iv_len);

		key_enc = keyblk + pssl->mac_len * 2;
		key_dec = keyblk + pssl->mac_len * 2 + pssl->key_len;
	}
	else
	{
		xmem_copy(pssl->mac_enc, keyblk + pssl->mac_len, pssl->mac_len);
		xmem_copy(pssl->mac_dec, keyblk, pssl->mac_len);

		xmem_copy(pssl->iv_enc, keyblk + pssl->mac_len * 2 + pssl->key_len * 2 + pssl->iv_len, pssl->iv_len);
		xmem_copy(pssl->iv_dec, keyblk + pssl->mac_len * 2 + pssl->key_len * 2, pssl->iv_len);

		key_enc = keyblk + pssl->mac_len * 2 + pssl->key_len;
		key_dec = keyblk + pssl->mac_len * 2;
	}

	switch (pssl->ses_ciph)
	{
	case SSL_RSA_RC4_128_MD5:
	case SSL_RSA_RC4_128_SHA:
		arc4_setup((arc4_context *)pssl->ctx_enc, key_enc, pssl->key_len);
		arc4_setup((arc4_context *)pssl->ctx_dec, key_dec, pssl->key_len);
		break;
	case SSL_RSA_DES_168_SHA:
	case SSL_EDH_RSA_DES_168_SHA:
		des3_set3key_enc((des3_context *)pssl->ctx_enc, key_enc);
		des3_set3key_dec((des3_context *)pssl->ctx_dec, key_dec);
		break;
	case SSL_RSA_AES_128_SHA:
		aes_setkey_enc((aes_context *)pssl->ctx_enc, key_enc, 128);
		aes_setkey_dec((aes_context *)pssl->ctx_dec, key_dec, 128);
		break;
	case SSL_RSA_AES_256_SHA:
	case SSL_EDH_RSA_AES_256_SHA:
		aes_setkey_enc((aes_context *)pssl->ctx_enc, key_enc, 256);
		aes_setkey_dec((aes_context *)pssl->ctx_dec, key_dec, 256);
		break;
	}
}

static int _ssl_check_rcv_msg(ssl_t* pssl)
{
	if (pssl->rcv_msg_len == 0)
	{
		pssl->nb_zero++;

		if (pssl->nb_zero > 3)
		{
			return C_ERR;
		}
	}
	else
	{
		pssl->nb_zero = 0;
	}

	return C_OK;
}

static int _ssl_encrypt_snd_msg(ssl_t *pssl)
{
	int i, padlen;
	unsigned char* mac_buf;

	mac_buf = pssl->snd_msg + pssl->snd_msg_len;

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		if (pssl->mac_len == 16)
		{
			_ssl_mac_md5(pssl->mac_enc, pssl->snd_ctr, pssl->snd_msg, pssl->snd_msg_len, pssl->snd_msg_type, mac_buf);
		}
		else if (pssl->mac_len == 20)
		{
			_ssl_mac_sha1(pssl->mac_enc, pssl->snd_ctr, pssl->snd_msg, pssl->snd_msg_len, pssl->snd_msg_type, mac_buf);
		}
		else
		{
			return C_ERR;
		}
	}
	else
	{
		if (pssl->mac_len == 16)
		{
			md5_hmac(pssl->mac_enc, 16, pssl->snd_ctr, pssl->snd_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE, mac_buf);
		}
		else if (pssl->mac_len == 20)
		{
			sha1_hmac(pssl->mac_enc, 20, pssl->snd_ctr, pssl->snd_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE, mac_buf);
		}
		else
		{
			return C_ERR;
		}
	}

	pssl->snd_msg_len += pssl->mac_len;

	//incre out message control bits
	for (i = 7; i >= 0; i--)
	{
		if (++pssl->snd_ctr[i] != 0)
			break;
	}

	if (pssl->iv_len == 0)
	{
		arc4_crypt((arc4_context *)pssl->ctx_enc, pssl->snd_msg_len, pssl->snd_msg, pssl->snd_msg);
	}
	else
	{
		padlen = pssl->iv_len - (pssl->snd_msg_len + 1) % pssl->iv_len;
		if (padlen == pssl->iv_len)
			padlen = 0;

		for (i = 0; i <= padlen; i++)
			pssl->snd_msg[pssl->snd_msg_len + i] = (unsigned char)(padlen);

		pssl->snd_msg_len += (padlen + 1);

		switch (pssl->iv_len)
		{
		case  8:
			des3_crypt_cbc((des3_context *)pssl->ctx_enc, DES_ENCRYPT, pssl->snd_msg_len, pssl->iv_enc, pssl->snd_msg, pssl->snd_msg);
			break;
		case 16:
			aes_crypt_cbc((aes_context *)pssl->ctx_enc, AES_ENCRYPT, pssl->snd_msg_len, pssl->iv_enc, pssl->snd_msg, pssl->snd_msg);
			break;
		}
	}

	return C_OK;
}

static int _ssl_decrypt_rcv_msg(ssl_t *pssl)
{
	int i, padlen;
	unsigned char* mac_buf;
	unsigned char mac_tmp[20];

	if (pssl->rcv_msg_len < pssl->min_len)
	{
		return C_ERR;
	}

	if (pssl->iv_len == 0)
	{
		padlen = 0;
		arc4_crypt((arc4_context *)pssl->ctx_dec, pssl->rcv_msg_len, pssl->rcv_msg, pssl->rcv_msg);
	}
	else
	{
		if (pssl->rcv_msg_len % pssl->iv_len != 0)
		{
			return C_ERR;
		}

		switch (pssl->iv_len)
		{
		case  8:
			des3_crypt_cbc((des3_context *)pssl->ctx_dec, DES_DECRYPT, pssl->rcv_msg_len, pssl->iv_dec, pssl->rcv_msg, pssl->rcv_msg);
			break;
		case 16:
			aes_crypt_cbc((aes_context *)pssl->ctx_dec, AES_DECRYPT, pssl->rcv_msg_len, pssl->iv_dec, pssl->rcv_msg, pssl->rcv_msg);
			break;
		}

		padlen = pssl->rcv_msg[pssl->rcv_msg_len - 1] + 1;

		if (pssl->minor_ver == SSL_MINOR_VERSION_0)
		{
			if (padlen > pssl->iv_len)
			{
				padlen = 0;
			}
		}
		else
		{
			for (i = 1; i <= padlen; i++)
			{
				if (pssl->rcv_msg[pssl->rcv_msg_len - i] != padlen - 1)
				{
					padlen = 0;
				}
			}
		}
	}

	pssl->rcv_msg_len -= (pssl->mac_len + padlen);
	PUT_SWORD_NET(pssl->rcv_hdr, 3, (unsigned short)pssl->rcv_msg_len);

	mac_buf = pssl->rcv_msg + pssl->rcv_msg_len;

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		if (pssl->mac_len == 16)
			_ssl_mac_md5(pssl->mac_dec, pssl->rcv_ctr, pssl->rcv_msg, pssl->rcv_msg_len, pssl->rcv_msg_type, mac_tmp);
		else
			_ssl_mac_sha1(pssl->mac_dec, pssl->rcv_ctr, pssl->rcv_msg, pssl->rcv_msg_len, pssl->rcv_msg_type, mac_tmp);
	}
	else
	{
		if (pssl->mac_len == 16)
			md5_hmac(pssl->mac_dec, 16, pssl->rcv_ctr, pssl->rcv_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE, mac_tmp);
		else
			sha1_hmac(pssl->mac_dec, 20, pssl->rcv_ctr, pssl->rcv_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE, mac_tmp);
	}

	if (xmem_comp(mac_tmp, pssl->mac_len, mac_buf, pssl->mac_len) != 0)
	{
		return C_ERR;
	}

	if (pssl->iv_len != 0 && padlen == 0)
	{
		return C_ERR;
	}

	//incres in message control bits
	for (i = 7; i >= 0; i--)
	{
		if (++pssl->rcv_ctr[i] != 0)
			break;
	}

	return C_OK;
}

int _ssl_write_snd_msg(ssl_t *pssl)
{
	dword_t dw;
	stream_t stm = NULL;
	int haslen;

	TRY_CATCH;

	//ssl header: 5 bytes
	//0: message type
	//1-2: version
	//3-4: package message length
	PUT_BYTE(pssl->snd_hdr, 0, (byte_t)pssl->snd_msg_type);
	PUT_BYTE(pssl->snd_hdr, 1, (byte_t)pssl->major_ver);
	PUT_BYTE(pssl->snd_hdr, 2, (byte_t)pssl->minor_ver);
	PUT_SWORD_NET(pssl->snd_hdr, 3, (unsigned short)pssl->snd_msg_len);

	if (pssl->snd_msg_type == SSL_MSG_HANDSHAKE)
	{
		haslen = GET_THREEBYTE_LEN(pssl->snd_msg, 1);
		md5_update(&pssl->md5, pssl->snd_msg, SSL_HSH_SIZE + haslen);
		sha1_update(&pssl->sha1, pssl->snd_msg, SSL_HSH_SIZE + haslen);
	}

	if (pssl->ses_crypt != 0)
	{
		if (C_OK != _ssl_encrypt_snd_msg(pssl))
		{
			raise_user_error(NULL, NULL);
		}
		//reset message length
		PUT_SWORD_NET(pssl->snd_hdr, 3, (unsigned short)pssl->snd_msg_len);
	}

	stm = stream_alloc(pssl->tcp);

	dw = SSL_HDR_SIZE + pssl->snd_msg_len;

	if (!stream_write_bytes(stm, pssl->snd_hdr, dw))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	pssl->snd_msg_pop = 0;

	END_CATCH;

	return C_OK;
ONERROR:

	if (stm)
		stream_free(stm);

	return C_ERR;
}

static bool_t _ssl_write_data(ssl_t* pssl, byte_t* buf, int* need)
{
	if (!(*need))
		return 1;

	*need = (*need + pssl->snd_msg_pop < SSL_PKG_SIZE) ? (*need) : (SSL_PKG_SIZE - pssl->snd_msg_pop);

	xmem_copy(pssl->snd_msg + pssl->snd_msg_pop, buf, *need);
	pssl->snd_msg_pop += (*need);

	if (pssl->snd_msg_pop == SSL_PKG_SIZE)
	{
		pssl->snd_msg_len = pssl->snd_msg_pop;
		pssl->snd_msg_type = SSL_MSG_APPLICATION_DATA;

		if (C_OK != _ssl_write_snd_msg(pssl))
			return 0;
	}

	return 1;
}

static bool_t _ssl_flush_data(ssl_t* pssl)
{
	if (!pssl->snd_msg_pop)
		return 1;

	pssl->snd_msg_len = pssl->snd_msg_pop;
	pssl->snd_msg_type = SSL_MSG_APPLICATION_DATA;

	return (C_OK == _ssl_write_snd_msg(pssl)) ? 1 : 0;
}

static bool_t _ssl_write_close(ssl_t* pssl)
{
	pssl->ses_over = -1;

	pssl->snd_msg_type = SSL_MSG_ALERT;
	pssl->snd_msg_len = 2;
	pssl->snd_msg[0] = SSL_ALERT_WARNING;
	pssl->snd_msg[1] = SSL_ALERT_CLOSE_NOTIFY;

	pssl->ses_crypt = 0;

	return (C_OK == _ssl_write_snd_msg(pssl)) ? 1 : 0;
}

int _ssl_read_rcv_msg(ssl_t *pssl)
{
	stream_t stm = NULL;
	dword_t dw;
	int haslen;

	TRY_CATCH;

	stm = stream_alloc(pssl->tcp);

	dw = SSL_HDR_SIZE;
	if (!stream_read_bytes(stm, pssl->rcv_hdr, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	if (pssl->rcv_hdr[1] != SSL_MAJOR_VERSION_3)
	{
		raise_user_error(_T("0"), _T("major version mismatch"));
	}

	if (pssl->rcv_hdr[2] != SSL_MINOR_VERSION_0 && pssl->rcv_hdr[2] != SSL_MINOR_VERSION_1)
	{
		raise_user_error(_T("0"), _T("minor version mismatch"));
	}

	pssl->rcv_msg_type = GET_BYTE(pssl->rcv_hdr, 0);
	pssl->rcv_msg_len = GET_SWORD_NET(pssl->rcv_hdr, 3);

	if (pssl->rcv_msg_len < 1 || pssl->rcv_msg_len > SSL_PKG_SIZE)
	{
		raise_user_error(_T("0"), _T("invalid message block length"));
	}

	dw = pssl->rcv_msg_len;
	if (!stream_read_bytes(stm, pssl->rcv_msg, &dw))
	{
		raise_user_error(NULL, NULL);
	}

	stream_free(stm);
	stm = NULL;

	if (pssl->rcv_msg_type == SSL_MSG_ALERT)
	{
		pssl->ses_crypt = 0;

		if (pssl->rcv_msg[0] == SSL_ALERT_FATAL)
		{
			raise_user_error(_T("0"), _T("fatal alert message"));
		}

		if (pssl->rcv_msg[0] == SSL_ALERT_WARNING && pssl->rcv_msg[1] == SSL_ALERT_CLOSE_NOTIFY)
		{
			pssl->ses_over = -1;
			pssl->rcv_msg_len = 0;
		}
	}

	if (pssl->ses_crypt)
	{
		if (C_OK != _ssl_decrypt_rcv_msg(pssl))
		{
			raise_user_error(NULL, NULL);
		}
	}

	if (pssl->rcv_msg_type == SSL_MSG_HANDSHAKE)
	{
		haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);
		md5_update(&pssl->md5, pssl->rcv_msg, SSL_HSH_SIZE + haslen);
		sha1_update(&pssl->sha1, pssl->rcv_msg, SSL_HSH_SIZE + haslen);
	}

	pssl->rcv_msg_pop = 0;

	END_CATCH;

	return C_OK;
ONERROR:

	if (stm)
		stream_free(stm);

	return C_ERR;
}

static bool_t _ssl_read_data(ssl_t* pssl, byte_t* buf, int* need)
{
	if (!(*need))
		return 1;

	if (pssl->rcv_msg_pop == pssl->rcv_msg_len)
	{
		if (C_OK != _ssl_read_rcv_msg(pssl))
		{
			return 0;
		}
		
		if (pssl->rcv_msg_len && pssl->rcv_msg_type != SSL_MSG_APPLICATION_DATA)
		{
			set_last_error(_T("0"), _T("not application data"), -1);
			return 0;
		}
	}

	*need = (*need + pssl->rcv_msg_pop < pssl->rcv_msg_len) ? (*need) : (pssl->rcv_msg_len - pssl->rcv_msg_pop);
	
	xmem_copy(buf, pssl->rcv_msg + pssl->rcv_msg_pop, *need);
	pssl->rcv_msg_pop += (*need);

	return 1;
}

/*********************************************************************************************************/

static handshake_states _ssl_write_client_hello(ssl_t *pssl)
{
	int i, n, len;
	int msglen = SSL_HSH_SIZE;
	dword_t t;

	pssl->major_ver = SSL_MAJOR_VERSION_3;
	pssl->minor_ver = SSL_MINOR_VERSION_0;

	pssl->max_major_ver = SSL_MAJOR_VERSION_3;
	pssl->max_minor_ver = SSL_MINOR_VERSION_1;

	//gen client random bytes
	t = get_times();
	PUT_DWORD_NET(pssl->rnd_cli, 0, t);

	for (i = 4; i < SSL_RND_SIZE; i++)
	{
		pssl->rnd_cli[i] = (byte_t)havege_rand(&pssl->rng);
	}

	//0:	handshake type
	//1~3:	handshake length
	//4~5:	highest version supported
	//6~9:	current UNIX time
	//10~37:	random bytes

	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)pssl->max_major_ver);
	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)pssl->max_minor_ver);
	
	xmem_copy(pssl->snd_msg + msglen, pssl->rnd_cli, SSL_RND_SIZE);
	msglen += SSL_RND_SIZE;
	
	//38: session id length
	//39~39+n: session id
	//40+n~41+n: cipherlist length
	//42+n: cipherlist
	//compression alg. (0)
	//extensions (unused)

	n = pssl->ses_size;
	if (n < 16 || n > 32 || pssl->ses_resume == 0)
		n = 0;

	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)n);

	xmem_copy(pssl->snd_msg + msglen, pssl->ses_id, n);
	msglen += n;

	for (n = 0; pssl->set_ciphers[n] != 0; n++);

	PUT_SWORD_NET(pssl->snd_msg, msglen, n * 2);
	msglen += 2;

	for (i = 0; i < n; i++)
	{
		PUT_SWORD_NET(pssl->snd_msg, msglen, pssl->set_ciphers[i]);
		msglen += 2;
	}

	PUT_BYTE(pssl->snd_msg, msglen++, 1);
	PUT_BYTE(pssl->snd_msg, msglen++, 0);

	if (pssl->host_cn != NULL)
	{
		len = a_xslen(pssl->host_cn);
		// Extensions length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(len + 9));
		msglen += 2;

		// Extension type
		PUT_SWORD_NET(pssl->snd_msg, msglen, 0);
		msglen += 2;

		// Extension length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(len + 5));
		msglen += 2;

		// Server name list length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(len + 3));
		msglen += 2;

		// Name type
		PUT_BYTE(pssl->snd_msg, msglen, 0);
		msglen++;

		// Hostname length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(len));
		msglen += 2;

		// Hostname
		xmem_copy(pssl->snd_msg + msglen, pssl->host_cn, len);
		msglen += len;
	}

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake type
	PUT_BYTE(pssl->snd_msg, 0, (byte_t)SSL_HS_CLIENT_HELLO);
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_SERVER_HELLO : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_parse_server_hello(ssl_t *pssl)
{
	dword_t t;
	int ciph, i;
	int msglen, haslen, seslen, extlen;

	//0:	handshake type
	//1~3:	handshake length
	//4~5:	protocol version
	//6~9:	UNIX time()
	//10~37:` random bytes

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_SERVER_HELLO)
	{
		set_last_error(_T("0"), _T("not handshake message"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg[4] != SSL_MAJOR_VERSION_3 || (pssl->rcv_msg[5] != SSL_MINOR_VERSION_0 && pssl->rcv_msg[5] != SSL_MINOR_VERSION_1))
	{
		set_last_error(_T("0"), _T("handshake hello message version checked failed"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_len < 38)
	{
		set_last_error(_T("0"), _T("handshake hello message block too short"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);
	
	msglen = SSL_HSH_SIZE;
	pssl->minor_ver = GET_BYTE(pssl->rcv_msg, msglen + 1);
	msglen += 2;

	t = GET_DWORD_NET(pssl->rcv_msg, msglen);
	xmem_copy(pssl->rnd_srv, pssl->rcv_msg + msglen, SSL_RND_SIZE);
	msglen += SSL_RND_SIZE;

	//38:	session id length
	//39~38+n:	session id
	//39+n~40+n:	chosen cipher
	//41+n~41+n:	chosen compression alg.
	//42+n~43+n:	extensions length
	//44+n~44+n+m:	extensions

	seslen = GET_BYTE(pssl->rcv_msg, msglen);
	msglen++;

	ciph = GET_SWORD_NET(pssl->rcv_msg, msglen + seslen);

	if (pssl->ses_resume == 0 || pssl->ses_ciph != ciph || a_xslen(pssl->ses_id) != seslen || xmem_comp(pssl->ses_id, seslen, pssl->rcv_msg + msglen, seslen) != 0)
	{
		pssl->ses_resume = 0;
		pssl->ses_ciph = ciph;
		xmem_copy(pssl->ses_id, pssl->rcv_msg + msglen, seslen);
		pssl->ses_size = seslen;
	}
	else
	{
		_ssl_derive_keys(pssl);
	}

	msglen += seslen;
	//skip chosen cipher
	msglen += 2;
	//skip compression alg
	msglen++;

	if (haslen + SSL_HSH_SIZE > msglen)
	{
		extlen = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;

		//skip extension
		msglen += extlen;
	}

	if (haslen + SSL_HSH_SIZE != msglen)
	{
		set_last_error(_T("0"), _T("invalid server hello message session block"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	i = 0;
	while (1)
	{
		if (pssl->set_ciphers[i] == 0)
		{
			set_last_error(_T("0"), _T("invalid server hello message cipher"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (pssl->set_ciphers[i++] == pssl->ses_ciph)
			break;
	}

	return SSL_SERVER_CERTIFICATE;
}

handshake_states _ssl_parse_server_certificate(ssl_t *pssl)
{
	int  ret, n;
	int msglen, haslen, crtlen;

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	//0:	handshake type
	//1~3:	handshake length
	//4~6:	length of all certs
	//7~9:	length of cert. 1
	//10~n-1:	peer certificate
	//n~n+2:	length of cert. 2
	//n+3~...:	upper level cert, etc.

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_CERTIFICATE)
	{
		set_last_error(_T("0"), _T("invalid certificate message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);

	msglen = SSL_HSH_SIZE;
	crtlen = GET_THREEBYTE_LEN(pssl->rcv_msg, msglen);
	msglen += 3;

	if (haslen != 3 + crtlen)
	{
		set_last_error(_T("0"), _T("invalid certificate block size"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	pssl->crt_pe = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	while (crtlen)
	{
		n = GET_THREEBYTE_LEN(pssl->rcv_msg, msglen);
		msglen += 3;
		crtlen -= 3;

		if (n < 128 || n > crtlen)
		{
			set_last_error(_T("0"), _T("invalid certificate size"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != x509parse_crt(pssl->crt_pe, pssl->rcv_msg + msglen, n))
		{
			set_last_error(_T("0"), _T("invalid certificate context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		msglen += n;
		crtlen -= n;
	}

	if (pssl->srv_mode != SSL_VERIFY_NONE)
	{
		if (pssl->crt_ca == NULL)
		{
			set_last_error(_T("0"), _T("CA chian empty"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (pssl->srv_mode != SSL_VERIFY_REQUIRED)
		{
			return SSL_SERVER_KEY_EXCHANGE;
		}

		if (C_OK != x509parse_verify(pssl->crt_pe, pssl->crt_ca, pssl->peer_cn, &ret))
		{
			set_last_error(_T("0"), _T("certificate verify failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}

	return SSL_SERVER_KEY_EXCHANGE;
}

static int _ssl_parse_server_key_exchange(ssl_t *pssl)
{
	int haslen, n;
	unsigned char *p, *end;
	unsigned char hash[36];
	md5_context md5;
	sha1_context sha1;

	if (pssl->ses_ciph != SSL_EDH_RSA_DES_168_SHA && pssl->ses_ciph != SSL_EDH_RSA_AES_256_SHA)
	{
		return SSL_CERTIFICATE_REQUEST;
	}

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	//0:	handshake type
	//1~3:	handshake length
	//4~n:	dhm parameters
	//4+n~5+n:	rsa key length
	//6+n:` rsa key hash

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_SERVER_KEY_EXCHANGE)
	{
		set_last_error(_T("0"), _T("invalid server key exchange message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);

	p = pssl->rcv_msg + SSL_HSH_SIZE;
	end = p + haslen;

	if (!pssl->dhm_ow)
	{
		pssl->dhm_ow = (dhm_context*)xmem_alloc(sizeof(dhm_context));
	}

	if (C_OK != dhm_read_params(pssl->dhm_ow, &p, end) != 0)
	{
		set_last_error(_T("0"), _T("invalid server key exchange message context"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->dhm_ow->len < 64 || pssl->dhm_ow->len > 256)
	{
		set_last_error(_T("0"), _T("invalid server key exchange message context length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if ((int)(end - p) != pssl->crt_pe->rsa.len)
	{
		set_last_error(_T("0"), _T("invalid server key exchange message context rsa key"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	n = haslen - (end - p) - 2;

	md5_starts(&md5);
	md5_update(&md5, pssl->rnd_cli, SSL_RND_SIZE);
	md5_update(&md5, pssl->rnd_srv, SSL_RND_SIZE);
	md5_update(&md5, pssl->rcv_msg + SSL_HSH_SIZE, n);
	md5_finish(&md5, hash);

	sha1_starts(&sha1);
	sha1_update(&sha1, pssl->rnd_cli, SSL_RND_SIZE);
	sha1_update(&sha1, pssl->rnd_srv, SSL_RND_SIZE);
	sha1_update(&sha1, pssl->rcv_msg + SSL_HSH_SIZE, n);
	sha1_finish(&sha1, hash + 16);

	if (C_OK != rsa_pkcs1_verify(&pssl->crt_pe->rsa, RSA_PUBLIC, RSA_RAW, 36, hash, p))
	{
		set_last_error(_T("0"), _T("invalid server key exchange message context verify"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return SSL_CERTIFICATE_REQUEST;
}

static int _ssl_parse_server_certificate_request(ssl_t *pssl)
{
	int n, crtlen, msglen = SSL_HSH_SIZE;

	//0:	handshake type
	//1~3:	handshake length
	//4 :	cert type count
	//5~n-1:	cert types
	//n~n+1:	length of all DNs
	//n+2~n+3:	length of DN 1
	//n+4:	Distinguished Name #1
	// ...  length of DN 2, etc.

	if (C_OK != _ssl_read_rcv_msg(pssl))
		return SSL_HANDSHAKE_ERROR;

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE)
	{
		set_last_error(_T("0"), _T("invalid certificate request message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	pssl->cli_auth = (pssl->rcv_msg[0] == SSL_HS_CERTIFICATE_REQUEST) ? 1 : 0;

	if (pssl->cli_auth == 0)
	{
		return SSL_SERVER_HELLO_DONE;
	}

	/*pssl->crt_ca = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	//skip cert type count
	n = GET_BYTE(pssl->rcv_msg, msglen);
	msglen++;
	//skip cert types
	msglen += n;

	//all certs length
	crtlen = GET_SWORD_NET(pssl->rcv_msg, msglen);
	msglen += 2;

	while (crtlen)
	{
		n = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;
		crtlen -= 2;

		if (n > crtlen)
		{
			set_last_error(_T("0"), _T("invalid certificate size"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != x509parse_crt(pssl->crt_ca, pssl->rcv_msg + msglen, n))
		{
			set_last_error(_T("0"), _T("invalid certificate context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		msglen += n;
		crtlen -= n;
	}*/

	return SSL_SERVER_HELLO_DONE;
}

static int _ssl_parse_server_hello_done(ssl_t *pssl)
{
	//0:	handshake type
	//1~3:	handshake length

	if (pssl->cli_auth != 0)
	{
		if (C_OK != _ssl_read_rcv_msg(pssl))
		{
			return SSL_HANDSHAKE_ERROR;
		}

		if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE)
		{
			set_last_error(_T("0"), _T("not handshake message"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}

	if (pssl->rcv_msg[0] != SSL_HS_SERVER_HELLO_DONE)
	{
		set_last_error(_T("0"), _T("invalid server hello message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->cli_auth) ? SSL_CLIENT_CERTIFICATE : SSL_CLIENT_KEY_EXCHANGE;
}

static handshake_states _ssl_write_client_certificate(ssl_t *pssl)
{
	int n, lenpos, crtlen, msglen = SSL_HSH_SIZE;
	x509_cert *crt;

	XDL_ASSERT(pssl->cli_auth != 0);

	if (pssl->crt_ow == NULL && pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		pssl->snd_msg_len = 2;
		pssl->snd_msg_type = SSL_MSG_ALERT;
		pssl->snd_msg[0] = SSL_ALERT_WARNING;
		pssl->snd_msg[1] = SSL_ALERT_NO_CERTIFICATE;

		return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_CLIENT_KEY_EXCHANGE : SSL_HANDSHAKE_ERROR;
	}

	//0: handshake type
	//1-3: handshake length
	//4-6: length of all certs
	//7-9: length of cert
	//10~n-1: peer certificate
	//n~n+2: length of cert
	//n+3: upper level cert, etc.

	//skip all certs length
	lenpos = msglen;
	msglen += 3;

	crtlen = 0;
	crt = pssl->crt_ow;
	while (crt != NULL && crt->next != NULL)
	{
		n = crt->raw.len;
		if (msglen + 3 + n > SSL_PKG_SIZE)
		{
			set_last_error(_T("0"), _T("message package overwrited"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		//peer cert length
		PUT_THREEBYTE_LEN(pssl->snd_msg, msglen, n);
		msglen += 3;
		crtlen += 3;

		//peer cert data
		xmem_copy(pssl->snd_msg + msglen, crt->raw.p, n);
		msglen += n;
		crtlen += n;

		crt = crt->next;
	}

	//all cert length
	PUT_THREEBYTE_LEN(pssl->snd_msg, lenpos, crtlen);
	
	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_CERTIFICATE;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_CLIENT_KEY_EXCHANGE : SSL_HANDSHAKE_ERROR;
}

static int _ssl_write_client_key_exchange(ssl_t *pssl)
{
	int n, msglen = SSL_HSH_SIZE;
	
	if (pssl->ses_ciph == SSL_EDH_RSA_DES_168_SHA || pssl->ses_ciph == SSL_EDH_RSA_AES_256_SHA)
	{
		n = pssl->dhm_ow->len;

		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)n);
		msglen += 2;

		if (C_OK != dhm_make_public(pssl->dhm_ow, 256, pssl->snd_msg + msglen, &n, havege_rand, &pssl->rng))
		{
			return SSL_HANDSHAKE_ERROR;
		}
		msglen += n;

		pssl->pmslen = pssl->dhm_ow->len;

		if (C_OK != dhm_calc_secret(pssl->dhm_ow, pssl->pmsmst, &pssl->pmslen))
		{
			return SSL_HANDSHAKE_ERROR;
		}
	}
	else
	{
		pssl->pmsmst[0] = (unsigned char)pssl->max_major_ver;
		pssl->pmsmst[1] = (unsigned char)pssl->max_minor_ver;
		pssl->pmslen = SSL_MST_SIZE;

		for (n = 2; n < pssl->pmslen; n++)
			pssl->pmsmst[n] = (unsigned char)havege_rand(&pssl->rng);

		n = pssl->crt_pe->rsa.len;

		if (pssl->minor_ver != SSL_MINOR_VERSION_0)
		{
			PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)n);
			msglen += 2;
		}

		if (C_OK != rsa_pkcs1_encrypt(&pssl->crt_pe->rsa, RSA_PUBLIC, pssl->pmslen, pssl->pmsmst, pssl->snd_msg + msglen))
		{
			return SSL_HANDSHAKE_ERROR;
		}
		msglen += n;
	}

	_ssl_derive_keys(pssl);

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_CLIENT_KEY_EXCHANGE;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->cli_auth) ? SSL_CERTIFICATE_VERIFY : SSL_CLIENT_CHANGE_CIPHER_SPEC;
}

static int _ssl_write_client_certificate_verify(ssl_t *pssl)
{
	int n, msglen = SSL_HSH_SIZE;
	unsigned char hash[36];
	md5_context md5;
	sha1_context sha1;
	unsigned char pad_1[48];
	unsigned char pad_2[48];

	XDL_ASSERT(pssl->cli_auth != 0);
	
	if (pssl->rsa_ow == NULL)
	{
		set_last_error(_T("0"), _T("no private key"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(pad_1, 0x36, SSL_MST_SIZE);
		xmem_set(pad_2, 0x5C, SSL_MST_SIZE);

		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, pad_1, SSL_MST_SIZE);
		md5_finish(&md5, hash);

		md5_starts(&md5);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, pad_2, SSL_MST_SIZE);
		md5_update(&md5, hash, 16);
		md5_finish(&md5, hash);

		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, pad_1, 40);
		sha1_finish(&sha1, hash + 16);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, pad_2, 40);
		sha1_update(&sha1, hash + 16, 20);
		sha1_finish(&sha1, hash + 16);
	}
	else
	{
		md5_finish(&md5, hash);
		sha1_finish(&sha1, hash + 16);
	}

	n = pssl->rsa_ow->len;
	PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)n);
	msglen += 2;

	if (C_OK != rsa_pkcs1_sign(pssl->rsa_ow, RSA_PRIVATE, RSA_RAW, 36, hash, pssl->snd_msg + msglen))
	{
		return SSL_HANDSHAKE_ERROR;
	}
	msglen += n;

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_CERTIFICATE_VERIFY;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_CLIENT_CHANGE_CIPHER_SPEC : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_write_client_change_cipher_spec(ssl_t *pssl)
{
	pssl->snd_msg_type = SSL_MSG_CHANGE_CIPHER_SPEC;
	pssl->snd_msg_len = 1;
	pssl->snd_msg[0] = 1;

	pssl->ses_crypt = 0;

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_CLIENT_FINISHED : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_write_client_finished(ssl_t *pssl)
{
	int msglen = SSL_HSH_SIZE;
	md5_context  md5;
	sha1_context sha1;
	unsigned char padbuf[48];
	unsigned char md5sum[16];
	unsigned char sha1sum[20];
	unsigned char* mac_buf;

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));

	mac_buf = pssl->snd_msg + msglen;

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(padbuf, 0x36, 48);

		md5_update(&md5, "CLNT", 4);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_finish(&md5, md5sum);

		sha1_update(&sha1, "CLNT", 4);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_finish(&sha1, sha1sum);

		xmem_set(padbuf, 0x5C, 48);

		md5_starts(&md5);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_update(&md5, md5sum, 16);
		md5_finish(&md5, mac_buf);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_update(&sha1, sha1sum, 20);
		sha1_finish(&sha1, mac_buf + 16);

		msglen += 36;
	}
	else
	{
		md5_finish(&md5, padbuf);
		sha1_finish(&sha1, padbuf + 16);

		_ssl_hmac(pssl->ses_mst, SSL_MST_SIZE, "client finished", padbuf, 36, mac_buf, 12);

		msglen += 12;
	}

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_FINISHED;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	pssl->ses_crypt = 1;

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->ses_resume) ? SSL_HANDSHAKE_OVER : SSL_SERVER_CHANGE_CIPHER_SPEC;
}

static handshake_states _ssl_parse_server_change_cipher_spec(ssl_t *pssl)
{
	pssl->ses_crypt = 0;

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_type != SSL_MSG_CHANGE_CIPHER_SPEC)
	{
		set_last_error(_T("0"), _T("invalid change cipher spec message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_len != 1 || pssl->rcv_msg[0] != 1)
	{
		set_last_error(_T("0"), _T("invalid change cipher spec message context"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return SSL_SERVER_FINISHED;
}

static handshake_states _ssl_parse_server_finished(ssl_t *pssl)
{
	int hash_len;
	md5_context  md5;
	sha1_context sha1;
	unsigned char padbuf[48] = { 0 };
	unsigned char md5sum[16] = { 0 };
	unsigned char sha1sum[20] = { 0 };
	unsigned char mac_buf[36] = { 0 };

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(padbuf, 0x36, 48);

		md5_update(&md5, "SRVR", 4);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_finish(&md5, md5sum);

		sha1_update(&sha1, "SRVR", 4);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_finish(&sha1, sha1sum);

		xmem_set(padbuf, 0x5C, 48);

		md5_starts(&md5);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_update(&md5, md5sum, 16);
		md5_finish(&md5, mac_buf);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_update(&sha1, sha1sum, 20);
		sha1_finish(&sha1, mac_buf + 16);

		hash_len = 36;
	}
	else
	{
		md5_finish(&md5, padbuf);
		sha1_finish(&sha1, padbuf + 16);

		_ssl_hmac(pssl->ses_mst, SSL_MST_SIZE, "server finished", padbuf, 36, mac_buf, 12);

		hash_len = 12;
	}

	pssl->ses_crypt = 1;

	if (C_OK != _ssl_read_rcv_msg(pssl))
		return SSL_HANDSHAKE_ERROR;

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_FINISHED)
	{
		set_last_error(_T("0"), _T("invalid finished message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (xmem_comp(pssl->rcv_msg + SSL_HSH_SIZE, hash_len, mac_buf, hash_len) != 0)
	{
		set_last_error(_T("0"), _T("invalid finished message hash"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->ses_resume) ? SSL_CLIENT_CHANGE_CIPHER_SPEC : SSL_HANDSHAKE_OVER;
}

static bool_t _ssl_handshake_client(ssl_t *pssl)
{
	handshake_states state = SSL_HELLO_REQUEST;

	while (state != SSL_HANDSHAKE_OVER)
	{
		switch (state)
		{
		case SSL_HELLO_REQUEST:
			state = SSL_CLIENT_HELLO;
			break;

			/*
			*  ==>   ClientHello
			*/
		case SSL_CLIENT_HELLO:
			state = _ssl_write_client_hello(pssl);
			break;

			/*
			*  <==   ServerHello
			*        Certificate
			*      ( ServerKeyExchange  )
			*      ( CertificateRequest )
			*        ServerHelloDone
			*/
		case SSL_SERVER_HELLO:
			state = _ssl_parse_server_hello(pssl);
			break;

		case SSL_SERVER_CERTIFICATE:
			state = _ssl_parse_server_certificate(pssl);
			break;

		case SSL_SERVER_KEY_EXCHANGE:
			state = _ssl_parse_server_key_exchange(pssl);
			break;

		case SSL_CERTIFICATE_REQUEST:
			state = _ssl_parse_server_certificate_request(pssl);
			break;

		case SSL_SERVER_HELLO_DONE:
			state = _ssl_parse_server_hello_done(pssl);
			break;

			/*
			*  ==> ( Certificate/Alert  )
			*        ClientKeyExchange
			*      ( CertificateVerify  )
			*        ChangeCipherSpec
			*        Finished
			*/
		case SSL_CLIENT_CERTIFICATE:
			state = _ssl_write_client_certificate(pssl);
			break;

		case SSL_CLIENT_KEY_EXCHANGE:
			state = _ssl_write_client_key_exchange(pssl);
			break;

		case SSL_CERTIFICATE_VERIFY:
			state = _ssl_write_client_certificate_verify(pssl);
			break;

		case SSL_CLIENT_CHANGE_CIPHER_SPEC:
			state = _ssl_write_client_change_cipher_spec(pssl);
			break;

		case SSL_CLIENT_FINISHED:
			state = _ssl_write_client_finished(pssl);
			break;

			/*
			*  <==   ChangeCipherSpec
			*        Finished
			*/
		case SSL_SERVER_CHANGE_CIPHER_SPEC:
			state = _ssl_parse_server_change_cipher_spec(pssl);
			break;

		case SSL_SERVER_FINISHED:
			state = _ssl_parse_server_finished(pssl);
			break;
		}
		
		if (state == SSL_HANDSHAKE_ERROR)
			break;
	}

	pssl->ses_over = (state == SSL_HANDSHAKE_OVER) ? 1 : -1;

	return (pssl->ses_over == 1)? 1 : 0;
}

/************************************************************************************************/

static handshake_states _ssl_parse_client_hello(ssl_t *pssl)
{
	int msglen, haslen, seslen, ciphlen, complen;
	int i, j;
	byte_t* ciph_buf;

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	// client hello message
	//0:	handshake type
	//1~3:	handshake length
	//4~5:	protocol version
	//6~9:	UNIX time()
	//10~37:	random bytes
	//38:	session id length
	//39~38+n: session id
	//39+n~40+n:	cipherlist length
	//41+n:	cipherlist
	//..:	compression alg.
	//..:	extensions

	if (pssl->rcv_msg[0] != SSL_HS_CLIENT_HELLO || pssl->rcv_msg[4] != SSL_MAJOR_VERSION_3)
	{
		set_last_error(_T("0"), _T("invalid client hello message"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);
	if (pssl->rcv_msg_len != SSL_HSH_SIZE + haslen)
	{
		set_last_error(_T("0"), _T("invalid client hello message length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	msglen = SSL_HSH_SIZE;

	pssl->major_ver = SSL_MAJOR_VERSION_3;
	pssl->minor_ver = (pssl->rcv_msg[5] <= SSL_MINOR_VERSION_1)? pssl->rcv_msg[5] : SSL_MINOR_VERSION_1;

	pssl->max_major_ver = pssl->rcv_msg[4];
	pssl->max_minor_ver = pssl->rcv_msg[5];
	msglen += 2;

	xmem_copy(pssl->rnd_cli, pssl->rcv_msg + msglen, SSL_RND_SIZE);
	msglen += SSL_RND_SIZE;

	seslen = GET_BYTE(pssl->rcv_msg, msglen);
	msglen++;

	if (seslen < 0 || seslen > 32)
	{
		set_last_error(_T("0"), _T("invalid client hello session id length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	xmem_copy(pssl->ses_id, pssl->rcv_msg + msglen, seslen);
	msglen += seslen;

	ciphlen = GET_SWORD_NET(pssl->rcv_msg, msglen);
	msglen += 2;

	if (ciphlen < 2 || ciphlen > 256 || (ciphlen % 2) != 0)
	{
		set_last_error(_T("0"), _T("invalid client hello session cipher list length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	ciph_buf = pssl->rcv_msg + msglen;
	for (i = 0; pssl->set_ciphers[i] != 0; i++)
	{
		for (j = 0; j < ciphlen; j += 2)
		{
			if (GET_SWORD_NET(ciph_buf, j) == pssl->set_ciphers[i])
				break;
		}

		if (j < ciphlen)
			break;
	}
	msglen += ciphlen;

	if (pssl->set_ciphers[i] == 0)
	{
		set_last_error(_T("0"), _T("invalid client hello session cipher chosen"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	pssl->ses_ciph = pssl->set_ciphers[i];

	complen = GET_BYTE(pssl->rcv_msg, msglen);
	msglen++;

	if (complen < 1 || complen > 16)
	{
		set_last_error(_T("0"), _T("invalid client hello session compress length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return SSL_SERVER_HELLO;
}

static handshake_states _ssl_write_server_hello(ssl_t *pssl)
{
	int i, msglen = SSL_HSH_SIZE;
	dword_t t;

	//gen server random bits
	t = get_times();
	PUT_DWORD_NET(pssl->rnd_srv, 0, t);

	for (i = 4; i < SSL_RND_SIZE; i++)
	{
		pssl->rnd_srv[i] = (byte_t)havege_rand(&pssl->rng);
	}

	//gen server session id
	pssl->ses_size = 32;
	for (i = 0; i < pssl->ses_size; i++)
	{
		pssl->ses_id[i] = (byte_t)havege_rand(&pssl->rng);
	}

	//0  handshake type
	//1~3   handshake length
	//4~5   protocol version
	//6~9   UNIX time()
	//10~37   random bytes

	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)pssl->major_ver);
	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)pssl->minor_ver);

	xmem_copy(pssl->snd_msg + msglen, pssl->rnd_srv, SSL_RND_SIZE);
	msglen += SSL_RND_SIZE;

	//38:	session id length
	//39~38+n:	session id
	//39+n~40+n:	chosen cipher
	//41+n~41+n:	chosen compression alg.
	
	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)pssl->ses_size);
	xmem_copy(pssl->snd_msg + msglen, pssl->ses_id, pssl->ses_size);
	msglen += pssl->ses_size;

	PUT_SWORD_NET(pssl->snd_msg, msglen, pssl->ses_ciph);
	msglen += 2;
	
	//SSL_COMPRESS_NULL
	PUT_BYTE(pssl->snd_msg, msglen++, 0);

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	PUT_BYTE(pssl->snd_msg, 0, (byte_t)SSL_HS_SERVER_HELLO);
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_SERVER_CERTIFICATE : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_write_server_certificate(ssl_t *pssl)
{
	int n, lenpos, crtlen, msglen = SSL_HSH_SIZE;
	x509_cert *crt;

	if (pssl->crt_ow == NULL)
	{
		set_last_error(_T("0"), _T("empty server certificate"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	//0: handshake type
	//1-3: handshake length
	//4-6: length of all certs
	//7-9: length of cert
	//10~n-1: peer certificate
	//n~n+2: length of cert
	//n+3: upper level cert, etc.

	//skip all certs length
	lenpos = msglen;
	msglen += 3;

	crtlen = 0;
	crt = pssl->crt_ow;
	while (crt != NULL && crt->next != NULL)
	{
		n = crt->raw.len;
		if (msglen + 3 + n > SSL_PKG_SIZE)
		{
			set_last_error(_T("0"), _T("message package overwrited"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		//peer cert length
		PUT_THREEBYTE_LEN(pssl->snd_msg, msglen, n);
		msglen += 3;
		crtlen += 3;

		//peer cert data
		xmem_copy(pssl->snd_msg + msglen, crt->raw.p, n);
		msglen += n;
		crtlen += n;

		crt = crt->next;
	}

	//reset all cert length
	PUT_THREEBYTE_LEN(pssl->snd_msg, lenpos, crtlen);

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_CERTIFICATE;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_SERVER_KEY_EXCHANGE : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_write_server_key_exchange(ssl_t *pssl)
{
	int n, msglen = SSL_HSH_SIZE;
	unsigned char hash[36] = { 0 };
	md5_context md5;
	sha1_context sha1;

	if (pssl->ses_ciph != SSL_EDH_RSA_DES_168_SHA && pssl->ses_ciph != SSL_EDH_RSA_AES_256_SHA)
	{
		return SSL_CERTIFICATE_REQUEST;
	}

	if (!pssl->dhm_ow)
	{
		pssl->dhm_ow = (dhm_context*)xmem_alloc(sizeof(dhm_context));
		mpi_read_string(&(pssl->dhm_ow->P), 16, dhm_P, -1);
		mpi_read_string(&(pssl->dhm_ow->G), 16, dhm_G, -1);
	}

	//0:	handshake type
	//1~3:	handshake length
	//4~n:	dhm parameters
	//4+n~5+n:	rsa key length
	//6+n:` rsa key sign

	// Ephemeral DH parameters:
	n = 0;
	if (C_OK != dhm_make_params(pssl->dhm_ow, 256, pssl->snd_msg + msglen, &n, havege_rand, &pssl->rng))
	{
		set_last_error(_T("0"), _T("make dhm params faild"), -1);
		return SSL_HANDSHAKE_ERROR;
	}
	msglen += n;

	/*
	* digitally-signed struct {
	*     opaque md5_hash[16];
	*     opaque sha_hash[20];
	* };
	*
	* md5_hash
	*     MD5(ClientHello.random + ServerHello.random
	*                            + ServerParams);
	* sha_hash
	*     SHA(ClientHello.random + ServerHello.random
	*                            + ServerParams);
	*/
	md5_starts(&md5);
	md5_update(&md5, pssl->rnd_cli, SSL_RND_SIZE);
	md5_update(&md5, pssl->rnd_srv, SSL_RND_SIZE);
	md5_update(&md5, pssl->snd_msg + SSL_HSH_SIZE, n);
	md5_finish(&md5, hash);

	sha1_starts(&sha1);
	sha1_update(&sha1, pssl->rnd_cli, SSL_RND_SIZE);
	sha1_update(&sha1, pssl->rnd_srv, SSL_RND_SIZE);
	sha1_update(&sha1, pssl->snd_msg + SSL_HSH_SIZE, n);
	sha1_finish(&sha1, hash + 16);

	PUT_SWORD_NET(pssl->snd_msg, msglen, pssl->rsa_ow->len);
	msglen += 2;
	
	if(C_OK != rsa_pkcs1_sign(pssl->rsa_ow, RSA_PRIVATE, RSA_RAW, 36, hash, pssl->snd_msg + msglen))
	{
		set_last_error(_T("0"), _T("rsa pkcs1 sign failed"), -1);
		return SSL_HANDSHAKE_ERROR;
	}
	msglen += pssl->rsa_ow->len;

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_SERVER_KEY_EXCHANGE;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->srv_mode == SSL_VERIFY_NONE) ? SSL_SERVER_HELLO_DONE : SSL_CERTIFICATE_REQUEST;
}

static handshake_states _ssl_write_server_certificate_request(ssl_t *pssl)
{
	int n, lenpos, crtlen, msglen = SSL_HSH_SIZE;
	x509_cert *crt;

	XDL_ASSERT(pssl->srv_mode != SSL_VERIFY_NONE);

	//0:	handshake type
	//1~3:	handshake length
	//4 :	cert type count
	//5~n-1:	cert types
	//n~n+1:	length of all DNs
	//n+2~n+3:	length of DN 1
	//n+4:	Distinguished Name #1
	// ...  length of DN 2, etc.

	//cert type count
	PUT_BYTE(pssl->snd_msg, msglen++, 1);
	//cert type
	PUT_BYTE(pssl->snd_msg, msglen++, 1);
	
	//skip all cert length 
	lenpos = msglen;
	msglen += 2;

	crtlen = 0;
	crt = pssl->crt_ca;
	while (crt != NULL && crt->next != NULL)
	{
		n = crt->subject_raw.len;
		PUT_SWORD_NET(pssl->snd_msg, msglen, n);
		msglen += 2;
		crtlen += 2;

		xmem_copy(pssl->snd_msg + msglen, crt->subject_raw.p, n);
		msglen += n;
		crtlen += n;
		
		crt = crt->next;
	}

	//reset all cert length
	PUT_SWORD_NET(pssl->snd_msg, lenpos, crtlen);

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	pssl->snd_msg[0] = SSL_HS_CERTIFICATE_REQUEST;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_SERVER_HELLO_DONE : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_write_server_hello_done(ssl_t *pssl)
{
	pssl->snd_msg_len = SSL_HSH_SIZE;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	pssl->snd_msg[0] = SSL_HS_SERVER_HELLO_DONE;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, 0);

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->srv_mode == SSL_VERIFY_NONE) ? SSL_CLIENT_KEY_EXCHANGE : SSL_CLIENT_CERTIFICATE;
}

static handshake_states _ssl_parse_client_certificate(ssl_t *pssl)
{
	int msglen, haslen, crtlen;
	int n, ret;

	XDL_ASSERT(pssl->srv_mode != SSL_VERIFY_NONE);
	
	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		if (pssl->rcv_msg_len == 2 && pssl->rcv_msg_type == SSL_MSG_ALERT && pssl->rcv_msg[0] == SSL_ALERT_WARNING  && pssl->rcv_msg[1] == SSL_ALERT_NO_CERTIFICATE)
		{
			if (pssl->srv_mode == SSL_VERIFY_OPTIONAL)
			{
				return SSL_CLIENT_KEY_EXCHANGE;
			}
			else
			{
				set_last_error(_T("0"), _T("client has no certificate"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
	}
	else
	{
		if (GET_THREEBYTE_LEN(pssl->rcv_msg, 1) == 3 && pssl->rcv_msg_type == SSL_MSG_HANDSHAKE  && pssl->rcv_msg[0] == SSL_HS_CERTIFICATE  && xmem_comp(pssl->rcv_msg + SSL_HSH_SIZE, 3, "\0\0\0", 3) == 0)
		{
			if (pssl->srv_mode == SSL_VERIFY_REQUIRED)
			{
				set_last_error(_T("0"), _T("client has no certificate"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
			else
			{
				return SSL_CLIENT_KEY_EXCHANGE;
			}
		}
	}

	//0:	handshake type
	//1~3:	handshake length
	//4~6:	length of all certs
	//7~9:	length of cert. 1
	//10~n-1:	peer certificate
	//n~n+2:	length of cert. 2
	//n+3~...:	upper level cert, etc.

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_CERTIFICATE)
	{
		set_last_error(_T("0"), _T("invalid certificate message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	msglen = SSL_HSH_SIZE;
	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);

	crtlen = GET_THREEBYTE_LEN(pssl->rcv_msg, msglen);
	msglen += 3;

	if (haslen != 3 + crtlen)
	{
		set_last_error(_T("0"), _T("invalid certificate block size"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	pssl->crt_pe = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	while (crtlen)
	{
		n = GET_THREEBYTE_LEN(pssl->rcv_msg, msglen);
		msglen += 3;
		crtlen -= 3;

		if (n < 128 || n > crtlen)
		{
			set_last_error(_T("0"), _T("invalid certificate size"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != x509parse_crt(pssl->crt_pe, pssl->rcv_msg + msglen, n))
		{
			set_last_error(_T("0"), _T("invalid certificate context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		msglen += n;
		crtlen -= n;
	}

	if (pssl->srv_mode != SSL_VERIFY_NONE)
	{
		if (pssl->crt_ca == NULL)
		{
			set_last_error(_T("0"), _T("CA chian empty"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != x509parse_verify(pssl->crt_pe, pssl->crt_ca, pssl->peer_cn, &ret))
		{
			if (pssl->srv_mode == SSL_VERIFY_REQUIRED)
			{
				set_last_error(_T("0"), _T("certificate verify failed"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
	}

	return SSL_CLIENT_KEY_EXCHANGE;
}

static handshake_states _ssl_parse_client_key_exchange(ssl_t *pssl)
{
	int msglen, haslen, i, n;

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_CLIENT_KEY_EXCHANGE)
	{
		set_last_error(_T("0"), _T("invalid client key exchange message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);
	msglen = SSL_HSH_SIZE;

	if (pssl->ses_ciph == SSL_EDH_RSA_DES_168_SHA || pssl->ses_ciph == SSL_EDH_RSA_AES_256_SHA)
	{
		//Receive G^Y mod P, premaster = (G^Y)^X mod P
		n = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;

		if (n < 1 || n > pssl->dhm_ow->len || n + 2 != haslen)
		{
			set_last_error(_T("0"), _T("invalid client key exchange length"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != dhm_read_public(pssl->dhm_ow, pssl->rcv_msg + msglen, n))
		{
			set_last_error(_T("0"), _T("invalid client key exchange context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		pssl->pmslen = pssl->dhm_ow->len;

		if (C_OK != dhm_calc_secret(pssl->dhm_ow, pssl->pmsmst, &pssl->pmslen))
		{
			set_last_error(_T("0"), _T("create premaster failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}
	else
	{
		// Decrypt the premaster using own private RSA key
		pssl->pmslen = 48;

		if (pssl->minor_ver != SSL_MINOR_VERSION_0)
		{
			n = GET_SWORD_NET(pssl->rcv_msg, msglen);
			msglen += 2;

			if (n != pssl->rsa_ow->len)
			{
				set_last_error(_T("0"), _T("invalid client key exchange key length"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
		else
		{
			n = pssl->rsa_ow->len;
		}

		if (haslen != 2 + n)
		{
			set_last_error(_T("0"), _T("invalid client key exchange key length"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != rsa_pkcs1_decrypt(pssl->rsa_ow, RSA_PRIVATE, &pssl->pmslen, pssl->rcv_msg + msglen, pssl->pmsmst))
		{
			set_last_error(_T("0"), _T("decrypt client key exchange key failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (pssl->pmslen != 48)// || pssl->pmsmst[0] != pssl->max_major_ver || pssl->pmsmst[1] != pssl->max_minor_ver)
		{
			/*
			* Protection against Bleichenbacher's attack:
			* invalid PKCS#1 v1.5 padding must not cause
			* the connection to end immediately; instead,
			* send a bad_record_mac later in the handshake.
			*/
			pssl->pmslen = 48;

			for (i = 0; i < pssl->pmslen; i++)
				pssl->pmsmst[i] = (unsigned char)havege_rand(&pssl->rng);
		}
	}

	_ssl_derive_keys(pssl);

	return (pssl->crt_pe) ? SSL_CERTIFICATE_VERIFY : SSL_CLIENT_CHANGE_CIPHER_SPEC;
}

static handshake_states _ssl_parse_client_certificate_verify(ssl_t *pssl)
{
	int msglen, haslen, n;
	md5_context md5;
	sha1_context sha1;
	unsigned char pad_1[48];
	unsigned char pad_2[48];
	unsigned char hash[36];

	XDL_ASSERT(pssl->crt_pe != NULL);

	memcpy(&md5, &pssl->md5, sizeof(md5_context));
	memcpy(&sha1, &pssl->sha1, sizeof(sha1_context));

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		memset(pad_1, 0x36, 48);
		memset(pad_2, 0x5C, 48);

		md5_update(&md5, pssl->ses_mst, 48);
		md5_update(&md5, pad_1, 48);
		md5_finish(&md5, hash);

		md5_starts(&md5);
		md5_update(&md5, pssl->ses_mst, 48);
		md5_update(&md5, pad_2, 48);
		md5_update(&md5, hash, 16);
		md5_finish(&md5, hash);

		sha1_update(&sha1, pssl->ses_mst, 48);
		sha1_update(&sha1, pad_1, 40);
		sha1_finish(&sha1, hash + 16);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->ses_mst, 48);
		sha1_update(&sha1, pad_2, 40);
		sha1_update(&sha1, hash + 16, 20);
		sha1_finish(&sha1, hash + 16);
	}
	else 
	{
		md5_finish(&md5, hash);
		sha1_finish(&sha1, hash + 16);
	}

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_CERTIFICATE_VERIFY)
	{
		set_last_error(_T("0"), _T("invalid certificate verify message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);
	msglen = SSL_HSH_SIZE;

	n = GET_SWORD_NET(pssl->rcv_msg, msglen);
	msglen += 2;

	if (n + 2 != haslen || n != pssl->crt_pe->rsa.len)
	{
		set_last_error(_T("0"), _T("invalid certificate verify message length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if(C_OK != rsa_pkcs1_verify(&pssl->crt_pe->rsa, RSA_PUBLIC, RSA_RAW, 36, hash, pssl->rcv_msg + msglen))
	{
		set_last_error(_T("0"), _T("invalid certificate verify message context"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return SSL_CLIENT_CHANGE_CIPHER_SPEC;
}

static handshake_states _ssl_parse_client_change_cipher_spec(ssl_t *pssl)
{
	pssl->ses_crypt = 0;

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_type != SSL_MSG_CHANGE_CIPHER_SPEC)
	{
		set_last_error(_T("0"), _T("invalid change cipher spec message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_len != 1 || pssl->rcv_msg[0] != 1)
	{
		set_last_error(_T("0"), _T("invalid change cipher spec message context"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return SSL_CLIENT_FINISHED;
}

static handshake_states _ssl_parse_client_finished(ssl_t *pssl)
{
	int hash_len;
	md5_context  md5;
	sha1_context sha1;
	unsigned char padbuf[48] = { 0 };
	unsigned char md5sum[16] = { 0 };
	unsigned char sha1sum[20] = { 0 };
	unsigned char mac_buf[36] = { 0 };

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(padbuf, 0x36, 48);

		md5_update(&md5, "CLNT", 4);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_finish(&md5, md5sum);

		sha1_update(&sha1, "CLNT", 4);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_finish(&sha1, sha1sum);

		xmem_set(padbuf, 0x5C, 48);

		md5_starts(&md5);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_update(&md5, md5sum, 16);
		md5_finish(&md5, mac_buf);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_update(&sha1, sha1sum, 20);
		sha1_finish(&sha1, mac_buf + 16);

		hash_len = 36;
	}
	else
	{
		md5_finish(&md5, padbuf);
		sha1_finish(&sha1, padbuf + 16);

		_ssl_hmac(pssl->ses_mst, SSL_MST_SIZE, "client finished", padbuf, 36, mac_buf, 12);

		hash_len = 12;
	}

	pssl->ses_crypt = 1;

	if (C_OK != _ssl_read_rcv_msg(pssl))
		return SSL_HANDSHAKE_ERROR;

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_FINISHED)
	{
		set_last_error(_T("0"), _T("invalid finished message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (xmem_comp(pssl->rcv_msg + SSL_HSH_SIZE, hash_len, mac_buf, hash_len) != 0)
	{
		set_last_error(_T("0"), _T("invalid finished message hash"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->ses_resume) ? SSL_HANDSHAKE_OVER : SSL_SERVER_CHANGE_CIPHER_SPEC;
}

static handshake_states _ssl_write_server_change_cipher_spec(ssl_t *pssl)
{
	pssl->snd_msg_type = SSL_MSG_CHANGE_CIPHER_SPEC;
	pssl->snd_msg_len = 1;
	pssl->snd_msg[0] = 1;

	pssl->ses_crypt = 0;

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_SERVER_FINISHED : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_write_server_finished(ssl_t *pssl)
{
	int msglen = SSL_HSH_SIZE;
	md5_context  md5;
	sha1_context sha1;
	unsigned char padbuf[48];
	unsigned char md5sum[16];
	unsigned char sha1sum[20];
	unsigned char* mac_buf;

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));

	mac_buf = pssl->snd_msg + msglen;

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(padbuf, 0x36, 48);

		md5_update(&md5, "SRVR", 4);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_finish(&md5, md5sum);

		sha1_update(&sha1, "SRVR", 4);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_finish(&sha1, sha1sum);

		xmem_set(padbuf, 0x5C, 48);

		md5_starts(&md5);
		md5_update(&md5, pssl->ses_mst, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_update(&md5, md5sum, 16);
		md5_finish(&md5, mac_buf);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->ses_mst, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_update(&sha1, sha1sum, 20);
		sha1_finish(&sha1, mac_buf + 16);

		msglen += 36;
	}
	else
	{
		md5_finish(&md5, padbuf);
		sha1_finish(&sha1, padbuf + 16);

		_ssl_hmac(pssl->ses_mst, SSL_MST_SIZE, "server finished", padbuf, 36, mac_buf, 12);

		msglen += 12;
	}

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_FINISHED;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	pssl->ses_crypt = 1;

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->ses_resume) ? SSL_CLIENT_CHANGE_CIPHER_SPEC : SSL_HANDSHAKE_OVER;
}

static bool_t _ssl_handshake_server(ssl_t *pssl)
{
	handshake_states state = SSL_HELLO_REQUEST;

	while (state != SSL_HANDSHAKE_OVER)
	{
		switch (state)
		{
		case SSL_HELLO_REQUEST:
			state = SSL_CLIENT_HELLO;
			break;

			/*
			*  <==   ClientHello
			*/
		case SSL_CLIENT_HELLO:
			state = _ssl_parse_client_hello(pssl);
			break;

			/*
			*  ==>   ServerHello
			*        Certificate
			*      ( ServerKeyExchange  )
			*      ( CertificateRequest )
			*        ServerHelloDone
			*/
		case SSL_SERVER_HELLO:
			state = _ssl_write_server_hello(pssl);
			break;

		case SSL_SERVER_CERTIFICATE:
			state = _ssl_write_server_certificate(pssl);
			break;

		case SSL_SERVER_KEY_EXCHANGE:
			state = _ssl_write_server_key_exchange(pssl);
			break;

		case SSL_CERTIFICATE_REQUEST:
			state = _ssl_write_server_certificate_request(pssl);
			break;

		case SSL_SERVER_HELLO_DONE:
			state = _ssl_write_server_hello_done(pssl);
			break;

			/*
			*  <== ( Certificate/Alert  )
			*        ClientKeyExchange
			*      ( CertificateVerify  )
			*        ChangeCipherSpec
			*        Finished
			*/
		case SSL_CLIENT_CERTIFICATE:
			state = _ssl_parse_client_certificate(pssl);
			break;

		case SSL_CLIENT_KEY_EXCHANGE:
			state = _ssl_parse_client_key_exchange(pssl);
			break;

		case SSL_CERTIFICATE_VERIFY:
			state = _ssl_parse_client_certificate_verify(pssl);
			break;

		case SSL_CLIENT_CHANGE_CIPHER_SPEC:
			state = _ssl_parse_client_change_cipher_spec(pssl);
			break;

		case SSL_CLIENT_FINISHED:
			state = _ssl_parse_client_finished(pssl);
			break;

			/*
			*  ==>   ChangeCipherSpec
			*        Finished
			*/
		case SSL_SERVER_CHANGE_CIPHER_SPEC:
			state = _ssl_write_server_change_cipher_spec(pssl);
			break;

		case SSL_SERVER_FINISHED:
			state = _ssl_write_server_finished(pssl);
			break;
		}

		if (state == SSL_HANDSHAKE_ERROR)
			break;
	}
		
	pssl->ses_over = (state == SSL_HANDSHAKE_OVER) ? 1 : -1;

	return (pssl->ses_over == 1)? 1 : 0;
}

/*********************************************************************************************************/

xhand_t xssl_cli(unsigned short port, const tchar_t* addr)
{
	ssl_t* pso;
	xhand_t tcp;
	
	tcp = xtcp_cli(port, addr);
	if (!tcp)
		return NULL;

	pso = (ssl_t*)xmem_alloc(sizeof(ssl_t));
	pso->head.tag = _HANDLE_SSL;

	pso->tcp = tcp;
	pso->type = SSL_TYPE_CLIENT;

	_ssl_init(pso);

	return (xhand_t)pso;
}

xhand_t xssl_srv(res_file_t so)
{
	ssl_t* pso;
	xhand_t tcp;

	tcp = xtcp_srv(so);
	if (!tcp)
		return NULL;

	pso = (ssl_t*)xmem_alloc(sizeof(ssl_t));
	pso->head.tag = _HANDLE_SSL;

	pso->tcp = tcp;
	pso->type = SSL_TYPE_SERVER;

	_ssl_init(pso);

	return (xhand_t)pso;
}

void  xssl_close(xhand_t ssl)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_CLIENT || pso->type == SSL_TYPE_SERVER);

	if (pso->snd_msg_pop)
	{
		_ssl_flush_data(pso);
	}

	_ssl_write_close(pso);

	xtcp_close(pso->tcp);

	_ssl_uninit(pso);

	xmem_free(pso);
}

res_file_t xssl_socket(xhand_t ssl)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	return (pso->tcp) ? xtcp_socket(pso->tcp) : INVALID_FILE;
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
	int bys, pos;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	if (!pso->ses_over)
	{
		if (pso->type == SSL_TYPE_CLIENT && !_ssl_handshake_client(pso))
		{
			return 0;
		}

		if (pso->type == SSL_TYPE_SERVER && !_ssl_handshake_server(pso))
		{
			return 0;
		}

		//clear handshake
		pso->rcv_msg_type = 0;
		pso->rcv_msg_len = 0;
		pso->rcv_msg_pop = 0;

		pso->snd_msg_type = 0;
		pso->snd_msg_len = 0;
		pso->snd_msg_pop = 0;
	}

	if (pso->ses_over < 0)
	{
		*pb = 0;
		return 1;
	}

	pos = 0;
	while (pos < (int)(*pb))
	{
		bys = *pb - pos;
		if (!_ssl_write_data(pso, buf + pos, &bys))
			break;

		if (!bys)
			break;

		pos += bys;
	}

	return (pos == (int)(*pb)) ? 1 : 0;
}

bool_t xssl_flush(xhand_t ssl)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	return _ssl_flush_data(pso);
}

bool_t xssl_read(xhand_t ssl, byte_t* buf, dword_t* pb)
{
	ssl_t* pso = (ssl_t*)ssl;
	int bys, pos;
	bool_t rt = 1;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	if (!pso->ses_over)
	{
		if (pso->type == SSL_TYPE_CLIENT && !_ssl_handshake_client(pso))
		{
			return 0;
		}

		if (pso->type == SSL_TYPE_SERVER && !_ssl_handshake_server(pso))
		{
			return 0;
		}

		//clear handshake
		pso->rcv_msg_type = 0;
		pso->rcv_msg_len = 0;
		pso->rcv_msg_pop = 0;

		pso->snd_msg_type = 0;
		pso->snd_msg_len = 0;
		pso->snd_msg_pop = 0;
	}

	if (pso->ses_over < 0)
	{
		*pb = 0;
		return 1;
	}

	pos = 0;
	while (pos < (int)(*pb))
	{
		bys = *pb - pos;
		rt = _ssl_read_data(pso, buf + pos, &bys);
		if (!rt)
			break;

		if (!bys)
			break;

		pos += bys;
	}

	*pb = pos;

	return rt;
}

void xssl_setopt(xhand_t ssl, int oid, void* opt, int len)
{
	switch (oid)
	{
	case SOCKET_OPTION_SNDBUF:
		xsocket_set_sndbuf(xssl_socket(ssl), *(int*)opt);
		break;
	case SOCKET_OPTION_RCVBUF:
		xsocket_set_rcvbuf(xssl_socket(ssl), *(int*)opt);
		break;
	case SOCKET_OPTION_NONBLK:
		xsocket_set_nonblk(xssl_socket(ssl), *(bool_t*)opt);
		break;
	}
}

unsigned short xssl_addr_port(xhand_t ssl, tchar_t* addr)
{
	ssl_t* pso = (ssl_t*)ssl;
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	xsocket_addr(xssl_socket(ssl), &na);
	conv_addr(&na, &port, addr);

	return port;
}

unsigned short xssl_peer_port(xhand_t ssl, tchar_t* addr)
{
	ssl_t* pso = (ssl_t*)ssl;
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	xsocket_peer(xssl_socket(ssl), &na);
	conv_addr(&na, &port, addr);

	return port;
}

void xssl_set_host(xhand_t ssl, const tchar_t* host_cn)
{
	ssl_t* pso = (ssl_t*)ssl;
	int len;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_CLIENT);

#ifdef _UNICODE
	len = ucs_to_mbs(host_cn, -1, NULL, MAX_LONG);
	pso->host_cn = a_xsalloc(len + 1);
	ucs_to_mbs(host_cn, -1, pso->host_cn, len);
#else
	len = a_xslen(host_cn);
	pso->host_cn = a_xsnclone(host_cn, len);
#endif
}

void xssl_set_peer(xhand_t ssl, const tchar_t* peer_cn)
{
	ssl_t* pso = (ssl_t*)ssl;
	int len;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_SERVER);

#ifdef _UNICODE
	len = ucs_to_mbs(peer_cn, -1, NULL, MAX_LONG);
	pso->peer_cn = a_xsalloc(len + 1);
	ucs_to_mbs(peer_cn, -1, pso->peer_cn, len);
#else
	len = a_xslen(peer_cn);
	pso->peer_cn = a_xsnclone(peer_cn, len);
#endif
}

bool_t xssl_set_ca(xhand_t ssl, const byte_t* sz_cert, dword_t clen)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_SERVER);

	if (!pso->crt_ca)
		pso->crt_ca = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	if (C_OK != x509parse_crt(pso->crt_ca, sz_cert, clen))
	{
		return 0;
	}

	return 1;
}

bool_t xssl_set_cert(xhand_t ssl, const byte_t* sz_cert, dword_t clen)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_CLIENT || pso->type == SSL_TYPE_SERVER);

	if (!pso->crt_ow)
		pso->crt_ow = (x509_cert*)xmem_alloc(sizeof(x509_cert));

	if (C_OK != x509parse_crt(pso->crt_ow, sz_cert, clen))
	{
		return 0;
	}

	return 1;
}

bool_t xssl_set_rsa(xhand_t ssl, const byte_t* sz_rsa, dword_t rlen, const byte_t* sz_pwd, dword_t plen)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_CLIENT || pso->type == SSL_TYPE_SERVER);

	if (!pso->rsa_ow)
		pso->rsa_ow = (rsa_context*)xmem_alloc(sizeof(rsa_context));

	if (C_OK != x509parse_key(pso->rsa_ow, sz_rsa, rlen, sz_pwd, plen))
	{
		return 0;
	}

	return 1;
}

bool_t xssl_set_dhm(xhand_t ssl, const byte_t *dhm_p, const byte_t *dhm_g)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_SERVER);

	if (dhm_p)
		mpi_read_string(&(pso->dhm_ow->P), 16, dhm_p, -1);
	if (dhm_g)
		mpi_read_string(&(pso->dhm_ow->G), 16, dhm_p, -1);

	return 1;
}

void xssl_set_auth(xhand_t ssl, int authmode)
{
	ssl_t* pso = (ssl_t*)ssl;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pso->type == SSL_TYPE_SERVER);

	pso->srv_mode = authmode;
}

#endif //XDK_SUPPORT_SOCK
