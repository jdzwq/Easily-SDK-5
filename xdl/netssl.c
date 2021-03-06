﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@authen ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssl document

	@module	netssl.c | implement file

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
#include "nettcp.h"
#include "stream.h"
#include "bioinf.h"

#include "xdlimp.h"
#include "xdlstd.h"

#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK)


#define GET_THREEBYTE_LEN(buf,off)		((buf[off] << 16) | (buf[off+1] << 8) | buf[off+2])
#define PUT_THREEBYTE_LEN(buf,off,n)	{buf[off] = (byte_t)((n) >> 16);buf[off+1] = (byte_t)((n) >> 8);buf[off+2] = (byte_t)((n));}

static char dhm_G[] = "4";
static char dhm_P[] = "E4004C1F94182000103D883A448B3F802CE4B44A83301270002C20D0321CFD00" \
"11CCEF784C26A400F43DFB901BCA7538F2C6B176001CF5A0FD16D2C48B1D0C1C" \
"F6AC8E1DA6BCC3B4E1F96B0564965300FFA1D0B601EB2800F489AA512C4B248C" \
"01F76949A60BB7F00A40B1EAB64BDD48E8A700D60B7F1200FA8E77B0A979DABF";

typedef struct _ciphers_set{
	int cipher;
	int key_size;
	int mac_size;
	int iv_size;
	int iv_impl; //implicit iv
}ciphers_set;


static ciphers_set client_ciphers[] = {
	{ SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256, 16, 0, 12, 4 },
	{ SSL_ECDHE_RSA_WITH_AES_256_CBC_SHA, 32, 20, 16, 0 },
	{ SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256, 16, 32, 16, 0 },
	{ SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA, 16, 20, 16, 0 },
	{ SSL_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA, 24, 20, 8, 0 },
	{ SSL_DHE_RSA_WITH_AES_256_CBC_SHA256, 32, 32, 16, 0 },
	{ SSL_DHE_RSA_WITH_AES_256_CBC_SHA, 32, 20, 16, 0 },
	{ SSL_DHE_RSA_WITH_AES_128_CBC_SHA256, 16, 32, 16, 0 },
	{ SSL_DHE_RSA_WITH_AES_128_CBC_SHA, 16, 20, 16, 0 },
	{ SSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA, 24, 20, 8, 0 },
	{ SSL_RSA_WITH_AES_256_CBC_SHA256, 32, 32, 16, 0 },
	{ SSL_RSA_WITH_AES_128_CBC_SHA256, 16, 32, 16, 0 },
	{ SSL_RSA_WITH_AES_256_CBC_SHA, 32, 20, 16, 0 },
	{ SSL_RSA_WITH_AES_128_CBC_SHA, 16, 20, 16, 0 },
	{ SSL_RSA_WITH_3DES_EDE_CBC_SHA, 24, 20, 8, 0 },
	{ SSL_RSA_WITH_RC4_128_SHA, 16, 20, 0, 0 },
	{ SSL_RSA_WITH_RC4_128_MD5, 16, 16, 0, 0 },
};

static ciphers_set server_ciphers[] = {
	{ SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256, 16, 0, 12, 4 },
	{ SSL_ECDHE_RSA_WITH_AES_256_CBC_SHA, 32, 20, 16, 0 },
	{ SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256, 16, 32, 16, 0 },
	{ SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA, 16, 20, 16, 0 },
	/*{ SSL_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA, 24, 20, 8, 0 },
	{ SSL_DHE_RSA_WITH_AES_256_CBC_SHA256, 32, 32, 16, 0 },
	{ SSL_DHE_RSA_WITH_AES_256_CBC_SHA, 32, 20, 16, 0 },
	{ SSL_DHE_RSA_WITH_AES_128_CBC_SHA256, 16, 32, 16, 0 },
	{ SSL_DHE_RSA_WITH_AES_128_CBC_SHA, 16, 20, 16, 0 },
	{ SSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA, 24, 20, 8, 0 },
	{ SSL_RSA_WITH_AES_256_CBC_SHA256, 32, 32, 16, 0 },
	{ SSL_RSA_WITH_AES_128_CBC_SHA256, 16, 32, 16, 0 },
	{ SSL_RSA_WITH_AES_256_CBC_SHA, 32, 20, 16, 0 },
	{ SSL_RSA_WITH_AES_128_CBC_SHA, 16, 20, 16, 0 },
	{ SSL_RSA_WITH_3DES_EDE_CBC_SHA, 24, 20, 8, 0 },
	{ SSL_RSA_WITH_RC4_128_SHA, 16, 20, 0, 0 },
	{ SSL_RSA_WITH_RC4_128_MD5, 16, 16, 0, 0 },*/
};

#define IS_DHE_CIPHER(cipher) ((cipher == SSL_DHE_RSA_WITH_AES_256_CBC_SHA256 || \
								cipher == SSL_DHE_RSA_WITH_AES_256_CBC_SHA || \
								cipher == SSL_DHE_RSA_WITH_AES_128_CBC_SHA256 || \
								cipher == SSL_DHE_RSA_WITH_AES_128_CBC_SHA || \
								cipher == SSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA) ? 1 : 0)
#define IS_ECDHE_CIPHER(cipher) ((cipher == SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256 || \
								cipher == SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256 || \
								cipher == SSL_ECDHE_RSA_WITH_AES_256_CBC_SHA || \
								cipher == SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA || \
								cipher == SSL_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA)? 1 : 0)

#define IS_BLOCK_CIPHER(cipher) ((cipher == SSL_DHE_RSA_WITH_AES_256_CBC_SHA256 || \
								cipher == SSL_DHE_RSA_WITH_AES_256_CBC_SHA || \
								cipher == SSL_DHE_RSA_WITH_AES_128_CBC_SHA256 || \
								cipher == SSL_DHE_RSA_WITH_AES_128_CBC_SHA || \
								cipher == SSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA || \
								cipher == SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256 || \
								cipher == SSL_ECDHE_RSA_WITH_AES_256_CBC_SHA || \
								cipher == SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA || \
								cipher == SSL_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA || \
								cipher == SSL_RSA_WITH_AES_256_CBC_SHA256 || \
								cipher == SSL_RSA_WITH_AES_128_CBC_SHA256 || \
								cipher == SSL_RSA_WITH_AES_256_CBC_SHA || \
								cipher == SSL_RSA_WITH_AES_128_CBC_SHA || \
								cipher == SSL_RSA_WITH_3DES_EDE_CBC_SHA)? 1 : 0)
#define IS_STREAM_CIPHER(cipher) ((cipher == SSL_RSA_WITH_RC4_128_SHA || \
								cipher == SSL_RSA_WITH_RC4_128_MD5)? 1 : 0)
#define IS_AEAD_CIPHER(cipher) ((cipher == SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256)? 1 : 0)

#define IS_AES_CIPHER(cipher) ((cipher == SSL_DHE_RSA_WITH_AES_256_CBC_SHA256 || \
								cipher == SSL_DHE_RSA_WITH_AES_256_CBC_SHA || \
								cipher == SSL_DHE_RSA_WITH_AES_128_CBC_SHA256 || \
								cipher == SSL_DHE_RSA_WITH_AES_128_CBC_SHA || \
								cipher == SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256 || \
								cipher == SSL_ECDHE_RSA_WITH_AES_256_CBC_SHA || \
								cipher == SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA || \
								cipher == SSL_RSA_WITH_AES_256_CBC_SHA256 || \
								cipher == SSL_RSA_WITH_AES_128_CBC_SHA256 || \
								cipher == SSL_RSA_WITH_AES_256_CBC_SHA || \
								cipher == SSL_RSA_WITH_AES_128_CBC_SHA)? 1 : 0)
#define IS_DES_CIPHER(cipher) ((cipher == SSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA || \
								cipher == SSL_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA || \
								cipher == SSL_RSA_WITH_3DES_EDE_CBC_SHA)? 1 : 0)
#define IS_GCM_CIPHER(cipher) ((cipher == SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256)? 1 : 0)
#define IS_ARC_CIPHER(cipher) ((cipher == SSL_RSA_WITH_RC4_128_SHA || \
								cipher == SSL_RSA_WITH_RC4_128_MD5)? 1 : 0)

static char label_client_finished[] = "client finished";
static char label_server_finished[] = "server finished";
static char label_master_secret[] = "master secret";
static char label_key_expansion[] = "key expansion";

typedef struct _ssl_context{
	handle_head head;

	bio_interface* pif;

	//SecurityParameters
	int type;		//ConnectionEnd: { server, client }
	int alg_prf; //PRFAlgorithm: enum { tls_prf_sha256 } PRFAlgorithm;
	int alg_bulk_cipher;	//BulkCipherAlgorithm: { null, rc4, rc2, des, 3des, des40 }
	int cipher_type; //CipherType: { stream, block }
	int cipher; //the selected cipher
	int key_size; //the encrypt decrypt key size
	int mac_size; //hash length
	int iv_size;
	int iv_implicit;
	int exportable; //IsExportable: { true, false } 
	int alg_mac; //MACAlgorithm: enum { null, hmac_md5, hmac_sha1, hmac_sha256, hmac_sha384, hmac_sha512 }
	int compress_method; //CompressionMethod: { null, (0), (255) }
	int alg_hash; 
	int alg_sign;
	int ecp_group;
	int ecp_format;
	byte_t master_secret[SSL_MST_SIZE]; 
	byte_t rnd_srv[SSL_RND_SIZE]; //server_random
	byte_t rnd_cli[SSL_RND_SIZE]; //client_random

	//Generated by SecurityParameters
	byte_t iv_enc[SSL_MAX_IVC]; 
	byte_t iv_dec[SSL_MAX_IVC]; 
	byte_t mac_enc[SSL_MAX_MAC];
	byte_t mac_dec[SSL_MAX_MAC]; 
	dword_t ctx_enc[SSL_CTX_SIZE];
	dword_t ctx_dec[SSL_CTX_SIZE];

	//Session State
	int authen_client; //client is authenity
	int verify_server; //verify server mode
	int over; //handshake step
	int resumed; //session is resumed
	int compressed; //record is compressed
	int ses_size; //session id size
	byte_t ses_id[SSL_SES_SIZE]; //session id

	//Tools
	havege_state rng;
	md5_context md5;
	sha1_context sha1;
	sha2_context sha2; //TLS 1.2

	schar_t* host_cn;
	schar_t* peer_cn;

	x509_cert* crt_ca;
	x509_cert* crt_pe;
	x509_cert* crt_ow;
	rsa_context* rsa_ca;
	rsa_context* rsa_ow;
	dhm_context* dhm_ow;
	ecdh_context* ecdh_ow;

	//Record
	int major_ver;
	int minor_ver;
	int srv_major_ver;
	int srv_minor_ver;
	int cli_major_ver;
	int cli_minor_ver;

	byte_t* snd_pkg;
	int snd_msg_type;
	int snd_msg_len;
	int snd_msg_pop;
	byte_t* snd_ctr; //counter
	byte_t* snd_hdr;
	byte_t* snd_msg;

	byte_t* rcv_pkg;
	int rcv_msg_type;
	int rcv_msg_len;
	int rcv_msg_pop;
	byte_t* rcv_ctr; //counter
	byte_t* rcv_hdr;
	byte_t* rcv_msg;

	int snd_crypted; //record is crypted sending
	int rcv_crypted; //record is crypted recving
	int nb_zero;
}ssl_context;

/***********************************************************************************************************************************/
//hash(MAC_write_secret + pad_2 + hash(MAC_write_secret + pad_1 + seq_num + SSLCompressed.type + SSLCompressed.length + SSLCompressed.fragment));
//pad_1:  The character 0x36 repeated 48 times for MD5 or 40 times for SHA.
//pad_2 : The character 0x5c repeated 48 times for MD5 or 40 times for SHA.
//seq_num : The sequence number for this message.
//hash : Hashing algorithm derived from the cipher suite.

static void _ssl_mac_md5(byte_t *secret, byte_t *ctr, byte_t *msg, int len, int type, byte_t out[16])
{
	byte_t header[SSL_CTR_SIZE + 3] = { 0 };
	byte_t padding[48] = { 0 };
	md5_context md5 = { 0 };

	xmem_copy(header, ctr, SSL_CTR_SIZE);
	PUT_BYTE(header, SSL_CTR_SIZE, (byte_t)type);
	PUT_SWORD_NET(header, (SSL_CTR_SIZE + 1), (unsigned short)len);

	xmem_set(padding, 0x36, 48);
	md5_starts(&md5);
	md5_update(&md5, secret, 16);
	md5_update(&md5, padding, 48);
	md5_update(&md5, header, SSL_CTR_SIZE + 3);
	md5_update(&md5, msg, len);
	md5_finish(&md5, out);

	xmem_set(padding, 0x5C, 48);
	md5_starts(&md5);
	md5_update(&md5, secret, 16);
	md5_update(&md5, padding, 48);
	md5_update(&md5, out, 16);
	md5_finish(&md5, out);
}

static void _ssl_mac_sha1(byte_t *secret, byte_t *ctr, byte_t *buf, int len, int type, byte_t out[20])
{
	byte_t header[SSL_CTR_SIZE + 3] = { 0 };
	byte_t padding[40] = { 0 };
	sha1_context sha1 = { 0 };

	xmem_copy(header, ctr, SSL_CTR_SIZE);
	PUT_BYTE(header, SSL_CTR_SIZE, (byte_t)type);
	PUT_SWORD_NET(header, (SSL_CTR_SIZE + 1), (unsigned short)len);

	xmem_set(padding, 0x36, 40);
	sha1_starts(&sha1);
	sha1_update(&sha1, secret, 20);
	sha1_update(&sha1, padding, 40);
	sha1_update(&sha1, header, SSL_CTR_SIZE + 3);
	sha1_update(&sha1, buf, len);
	sha1_finish(&sha1, out);

	xmem_set(padding, 0x5C, 40);
	sha1_starts(&sha1);
	sha1_update(&sha1, secret, 20);
	sha1_update(&sha1, padding, 40);
	sha1_update(&sha1, out, 20);
	sha1_finish(&sha1, out);
}

//master_secret =
//MD5(pre_master_secret + SHA('A' + pre_master_secret + ClientHello.random + ServerHello.random)) +
//MD5(pre_master_secret + SHA('BB' + pre_master_secret + ClientHello.random + ServerHello.random)) +
//MD5(pre_master_secret + SHA('CCC' + pre_master_secret + ClientHello.random + ServerHello.random));
static void _ssl_prf0(byte_t *secret, int slen, byte_t *random, int rlen, byte_t *dstbuf, int dlen)
{
	int i, mul;
	byte_t padding[16] = { 0 };
	byte_t sha1sum[20] = { 0 };

	sha1_context sha1 = { 0 };
	md5_context md5 = { 0 };

	mul = dlen / 16;
	for (i = 0; i < mul; i++)
	{
		xmem_set((void*)padding, 'A' + i, 1 + i);

		sha1_starts(&sha1);
		sha1_update(&sha1, padding, 1 + i);
		sha1_update(&sha1, secret, slen);
		sha1_update(&sha1, random, rlen);
		sha1_finish(&sha1, sha1sum);

		md5_starts(&md5);
		md5_update(&md5, secret, slen);
		md5_update(&md5, sha1sum, 20);
		md5_finish(&md5, dstbuf + i * 16);
	}
}

/*
PRF1(secret, label, seed) = P_MD5(S1, label + seed) XOR P_SHA-1(S2, label + seed);
L_S = length in bytes of secret;
L_S1 = L_S2 = ceil(L_S / 2);

P_hash(secret, seed) = HMAC_hash(secret, A(1) + seed) + HMAC_hash(secret, A(2) + seed) + HMAC_hash(secret, A(3) + seed) + ...
A(0) = seed
A(i) = HMAC_hash(secret, A(i-1))
*/

static void _ssl_prf1(byte_t *secret, int slen, char *label, byte_t *random, int rlen, byte_t *dstbuf, int dlen)
{
	int nb, hs;
	int i, j, k;
	byte_t *S1, *S2;
	byte_t seed[128] = { 0 };
	byte_t hash[20] = { 0 };

	hs = (slen + 1) / 2;
	S1 = secret;
	S2 = secret + slen - hs;

	nb = a_xslen(label);
	xmem_copy(seed + 20, label, nb);
	xmem_copy(seed + 20 + nb, random, rlen);
	nb += rlen;

	// First compute P_md5(secret,label+random)[0..dlen]
	//the MD5 output length is 16 bytes
	//A(0)
	md5_hmac(S1, hs, seed + 20, nb, seed + 4);

	for (i = 0; i < dlen; i += 16)
	{
		md5_hmac(S1, hs, seed + 4, 16 + nb, hash);
		//A(i)
		md5_hmac(S1, hs, seed + 4, 16, seed + 4);

		k = (i + 16 > dlen) ? dlen % 16 : 16;

		for (j = 0; j < k; j++)
			dstbuf[i + j] = hash[j];
	}

	// XOR out with P_sha1(secret,label+random)[0..dlen]
	//A(0)
	sha1_hmac(S2, hs, seed + 20, nb, seed);

	for (i = 0; i < dlen; i += 20)
	{
		sha1_hmac(S2, hs, seed, 20 + nb, hash);
		//A(i)
		sha1_hmac(S2, hs, seed, 20, seed);

		k = (i + 20 > dlen) ? dlen % 20 : 20;

		for (j = 0; j < k; j++)
			dstbuf[i + j] = (byte_t)(dstbuf[i + j] ^ hash[j]);
	}
}

/*
PRF2(secret, label, seed) = P_<hash>(secret, label + seed)
L_S = length in bytes of secret;
L_S1 = L_S2 = ceil(L_S / 2);

P_hash(secret, seed) = HMAC_hash(secret, A(1) + seed) + HMAC_hash(secret, A(2) + seed) + HMAC_hash(secret, A(3) + seed) + ...
A(0) = seed
A(i) = HMAC_hash(secret, A(i-1))
*/

static void _ssl_prf2(byte_t *secret, int slen, char *label, byte_t *random, int rlen, byte_t *dstbuf, int dlen)
{
	int nb, hs;
	int i, j, k;
	byte_t *S1;
	byte_t seed[128] = { 0 };
	byte_t hash[32] = { 0 };

	S1 = secret;
	hs = slen;

	nb = a_xslen(label);
	xmem_copy(seed + 32, label, nb);
	xmem_copy(seed + 32 + nb, random, rlen);
	nb += rlen;

	// compute P_sha256(secret,label+random)[0..dlen]
	//the sha256 output length is 32 bytes
	//A(0)
	sha2_hmac(S1, hs, seed + 32, nb, seed, 0);

	for (i = 0; i < dlen; i += 32)
	{
		sha2_hmac(S1, hs, seed, 32 + nb, hash, 0);
		//A(i)
		sha2_hmac(S1, hs, seed, 32, seed, 0);

		k = (i + 32 > dlen) ? dlen % 32 : 32;

		for (j = 0; j < k; j++)
			dstbuf[i + j] = hash[j];
	}
}

static void _ssl_init(ssl_context* pssl)
{
	//initialize tools
	havege_init(&pssl->rng);

	md5_starts(&pssl->md5);
	sha1_starts(&pssl->sha1);
	sha2_starts(&pssl->sha2, 0); //TLS 1.2

	//initialize session state
	pssl->over = 0; 
	pssl->resumed = 0;
	pssl->cipher = 0;
	pssl->compressed = 0;

	if (pssl->type == SSL_TYPE_CLIENT)
	{
		pssl->alg_hash = ALG_HASH_SHA1;
		pssl->alg_sign = ALG_SIGN_RSA;

		pssl->major_ver = pssl->cli_major_ver = SSL_MAJOR_VERSION_3;
		pssl->minor_ver = pssl->cli_minor_ver = SSL_MINOR_VERSION_3;
	}
	else
	{
		pssl->verify_server = SSL_VERIFY_NONE;
		pssl->authen_client = 0;

		pssl->major_ver = pssl->srv_major_ver = SSL_MAJOR_VERSION_3;
		pssl->minor_ver = pssl->srv_minor_ver = SSL_MINOR_VERSION_3;
	}

	//initialize records
	pssl->rcv_pkg = (byte_t *)xmem_alloc(SSL_MAX_SIZE);
	pssl->rcv_ctr = pssl->rcv_pkg;
	pssl->rcv_hdr = pssl->rcv_pkg + SSL_CTR_SIZE;
	pssl->rcv_msg = pssl->rcv_pkg + SSL_CTR_SIZE + SSL_HDR_SIZE;

	pssl->snd_pkg = (byte_t *)xmem_alloc(SSL_MAX_SIZE);
	pssl->snd_ctr = pssl->snd_pkg;
	pssl->snd_hdr = pssl->snd_pkg + SSL_CTR_SIZE;
	pssl->snd_msg = pssl->snd_pkg + SSL_CTR_SIZE + SSL_HDR_SIZE;

	pssl->rcv_crypted = 0;
	pssl->snd_crypted = 0;
}

static void _ssl_uninit(ssl_context* pssl)
{
	if (pssl->rcv_pkg)
	{
		xmem_free(pssl->rcv_pkg);
	}
	if (pssl->snd_pkg)
	{
		xmem_free(pssl->snd_pkg);
	}
	if (pssl->rsa_ca)
	{
		rsa_free(pssl->rsa_ca);
		xmem_free(pssl->rsa_ca);
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

	if (pssl->ecdh_ow)
	{
		ecdh_free(pssl->ecdh_ow);
		xmem_free(pssl->ecdh_ow);
	}
}

static bool_t _ssl_choose_cipher(ssl_context* pssl, int ciph)
{
	int i, n;
	ciphers_set* pcs;

	if (pssl->type == SSL_TYPE_CLIENT)
	{
		n = sizeof(client_ciphers) / sizeof(ciphers_set);
		pcs = client_ciphers;
	}
	else
	{
		n = sizeof(server_ciphers) / sizeof(ciphers_set);
		pcs = server_ciphers;
	}

	for (i = 0; i < n; i++)
	{
		if (ciph == pcs[i].cipher)
		{
			pssl->cipher = pcs[i].cipher;
			pssl->key_size = pcs[i].key_size;
			pssl->mac_size = pcs[i].mac_size;
			pssl->iv_size = pcs[i].iv_size;
			pssl->iv_implicit = pcs[i].iv_impl;

			return 1;
		}
	}

	set_last_error(_T("_ssl_choose_cipher"), _T("unknown cipher"), -1);

	return 0;
}

static void _ssl_derive_keys(ssl_context *pssl, byte_t* premaster, int prelen)
{
	byte_t rndb[SSL_RND_SIZE * 2] = { 0 };
	byte_t keyblk[SSL_BLK_SIZE] = { 0 };

	byte_t *key_enc, *key_dec;
	int iv_copy;

	//generating master security
	if (!pssl->resumed)
	{
		xmem_copy((void*)rndb, (void*)pssl->rnd_cli, SSL_RND_SIZE);
		xmem_copy((void*)(rndb + SSL_RND_SIZE), (void*)pssl->rnd_srv, SSL_RND_SIZE);

		if (pssl->minor_ver == SSL_MINOR_VERSION_0)
		{
			_ssl_prf0(premaster, prelen, rndb, SSL_RND_SIZE * 2, pssl->master_secret, SSL_MST_SIZE);
		}
		else if (pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			_ssl_prf2(premaster, prelen, label_master_secret, rndb, SSL_RND_SIZE * 2, pssl->master_secret, SSL_MST_SIZE);
		}
		else
		{
			_ssl_prf1(premaster, prelen, label_master_secret, rndb, SSL_RND_SIZE * 2, pssl->master_secret, SSL_MST_SIZE);
		}
	}

	// swap the client and server random values.
	xmem_copy((void*)rndb, (void*)pssl->rnd_srv, SSL_RND_SIZE);
	xmem_copy((void*)(rndb + SSL_RND_SIZE), (void*)pssl->rnd_cli, SSL_RND_SIZE);

	xmem_zero(pssl->rnd_cli, sizeof(pssl->rnd_cli));
	xmem_zero(pssl->rnd_srv, sizeof(pssl->rnd_srv));

	// generate key block
	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		//key_block =
		//MD5(master_secret + SHA(`A' + master_secret + ServerHello.random + ClientHello.random)) +
		//MD5(master_secret + SHA(`BB' + master_secret + ServerHello.random + ClientHello.random)) +
		//MD5(master_secret + SHA(`CCC' + master_secret + ServerHello.random + ClientHello.random)) + [...];
		_ssl_prf0(pssl->master_secret, SSL_MST_SIZE, rndb, SSL_RND_SIZE * 2, keyblk, SSL_BLK_SIZE);
	}
	else if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		//key_block = 
		//PRF(SecurityParameters.master_secret,
		//"key expansion",
		//SecurityParameters.server_random +
		//SecurityParameters.client_random);
		_ssl_prf2(pssl->master_secret, SSL_MST_SIZE, label_key_expansion, rndb, SSL_RND_SIZE * 2, keyblk, SSL_BLK_SIZE);
	}
	else
	{
		//key_block = 
		//PRF(SecurityParameters.master_secret,
		//"key expansion",
		//SecurityParameters.server_random +
		//SecurityParameters.client_random);
		_ssl_prf1(pssl->master_secret, SSL_MST_SIZE, label_key_expansion, rndb, SSL_RND_SIZE * 2, keyblk, SSL_BLK_SIZE);
	}

	if (IS_AEAD_CIPHER(pssl->cipher))
		iv_copy = (pssl->iv_implicit) ? pssl->iv_implicit : pssl->iv_size;
	else
		iv_copy = pssl->iv_size;

	//the key_block is partitioned as follows:
	//client_write_MAC_secret[SecurityParameters.hash_size]
	//server_write_MAC_secret[SecurityParameters.hash_size]
	//client_write_key[SecurityParameters.key_material_length]
	//server_write_key[SecurityParameters.key_material_length]
	//client_write_IV[SecurityParameters.IV_size]
	//server_write_IV[SecurityParameters.IV_size]
	if (pssl->type == SSL_TYPE_CLIENT)
	{
		//client_write_MAC_secret for client encrypting record
		xmem_copy(pssl->mac_enc, keyblk, pssl->mac_size);
		//server_write_MAC_secret for client decrypting record
		xmem_copy(pssl->mac_dec, keyblk + pssl->mac_size, pssl->mac_size);
		//client_write_key for client setup encrypting context
		key_enc = keyblk + pssl->mac_size * 2;
		//server_write_key for client setup decrypting context
		key_dec = keyblk + pssl->mac_size * 2 + pssl->key_size;
		//client_write_IV for client encrypting IV
		xmem_copy(pssl->iv_enc, keyblk + pssl->mac_size * 2 + pssl->key_size * 2, iv_copy);
		//server_write_IV for client decrypting IV
		xmem_copy(pssl->iv_dec, keyblk + pssl->mac_size * 2 + pssl->key_size * 2 + iv_copy, iv_copy);
	}
	else
	{
		//client_write_MAC_secret for server decrypting record
		xmem_copy(pssl->mac_dec, keyblk, pssl->mac_size);
		//server_write_MAC_secret for server encrypting record
		xmem_copy(pssl->mac_enc, keyblk + pssl->mac_size, pssl->mac_size);
		//client_write_key for server decrypting context
		key_dec = keyblk + pssl->mac_size * 2;
		//server_write_key for server encrypting context
		key_enc = keyblk + pssl->mac_size * 2 + pssl->key_size;	
		//client_write_IV for server decrypting IV
		xmem_copy(pssl->iv_dec, keyblk + pssl->mac_size * 2 + pssl->key_size * 2, iv_copy);
		//server_write_IV for server encrypting IV
		xmem_copy(pssl->iv_enc, keyblk + pssl->mac_size * 2 + pssl->key_size * 2 + iv_copy, iv_copy);
	}

	//initialize encrypt and decrypt context
	switch (pssl->cipher)
	{
	case SSL_RSA_WITH_RC4_128_MD5:
	case SSL_RSA_WITH_RC4_128_SHA:
		arc4_setup((arc4_context *)pssl->ctx_enc, key_enc, pssl->key_size); //the material size is bytes
		arc4_setup((arc4_context *)pssl->ctx_dec, key_dec, pssl->key_size); //the material size is bytes
		break;
	case SSL_RSA_WITH_3DES_EDE_CBC_SHA:
	case SSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA:
	case SSL_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA:
		des3_set3key_enc((des3_context *)pssl->ctx_enc, key_enc); //the material size is 24 bytes
		des3_set3key_dec((des3_context *)pssl->ctx_dec, key_dec); //the material size is 24 bytes
		break;
	case SSL_RSA_WITH_AES_128_CBC_SHA:
	case SSL_RSA_WITH_AES_256_CBC_SHA:
	case SSL_RSA_WITH_AES_128_CBC_SHA256:
	case SSL_RSA_WITH_AES_256_CBC_SHA256:
	case SSL_DHE_RSA_WITH_AES_128_CBC_SHA:
	case SSL_DHE_RSA_WITH_AES_256_CBC_SHA:
	case SSL_DHE_RSA_WITH_AES_256_CBC_SHA256:
	case SSL_DHE_RSA_WITH_AES_128_CBC_SHA256:
	case SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256:
	case SSL_ECDHE_RSA_WITH_AES_256_CBC_SHA:
	case SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA:
		aes_setkey_enc((aes_context *)pssl->ctx_enc, key_enc, (pssl->key_size * 8)); //the material size is bits
		aes_setkey_dec((aes_context *)pssl->ctx_dec, key_dec, (pssl->key_size * 8)); //the material size is bits
		break;
	case SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
		gcm_setkey((gcm_context *)pssl->ctx_enc, key_enc, (pssl->key_size * 8));
		gcm_setkey((gcm_context *)pssl->ctx_dec, key_dec, (pssl->key_size * 8));
		break;
	}
}

static int _ssl_check_rcv_msg(ssl_context* pssl)
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

static int _ssl_encrypt_snd_msg(ssl_context *pssl)
{
	int i, padlen;
	byte_t* mac_buf;
	byte_t iv_pre[16] = { 0 };
	int iv_copy;

	//byte_t gcm_add[13] = { 0 };
	//byte_t gcm_iv[12] = { 0 };
	//int addlen;
	int taglen;

	if (IS_BLOCK_CIPHER(pssl->cipher) || IS_STREAM_CIPHER(pssl->cipher))
	{
		mac_buf = pssl->snd_msg + pssl->snd_msg_len;

		if (pssl->minor_ver == SSL_MINOR_VERSION_0)
		{
			//The MAC is generated as :
			//hash(MAC_write_secret + pad_2 + hash(MAC_write_secret + pad_1 + seq_num + SSLCompressed.type + SSLCompressed.length + SSLCompressed.fragment));

			if (pssl->mac_size == 16)
			{
				_ssl_mac_md5(pssl->mac_enc, pssl->snd_ctr, pssl->snd_msg, pssl->snd_msg_len, pssl->snd_msg_type, mac_buf);
			}
			else if (pssl->mac_size == 20)
			{
				_ssl_mac_sha1(pssl->mac_enc, pssl->snd_ctr, pssl->snd_msg, pssl->snd_msg_len, pssl->snd_msg_type, mac_buf);
			}
			else
			{
				set_last_error(_T("_ssl_encrypt_snd_msg"), _T("unknown hmac function"), -1);

				return C_ERR;
			}
		}
		else
		{
			//The MAC is generated as:
			//HMAC_hash(MAC_write_secret, seq_num + TLSCompressed.type + TLSCompressed.version + TLSCompressed.length + TLSCompressed.fragment));

			if (pssl->mac_size == 16)
			{
				md5_hmac(pssl->mac_enc, pssl->mac_size, pssl->snd_ctr, pssl->snd_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE, mac_buf);
			}
			else if (pssl->mac_size == 20)
			{
				sha1_hmac(pssl->mac_enc, pssl->mac_size, pssl->snd_ctr, pssl->snd_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE, mac_buf);
			}
			else if (pssl->mac_size == 32) //TLS 1.2
			{
				sha2_hmac(pssl->mac_enc, pssl->mac_size, pssl->snd_ctr, pssl->snd_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE, mac_buf, 0);
			}
			else
			{
				set_last_error(_T("_ssl_encrypt_snd_msg"), _T("unknown hmac function"), -1);

				return C_ERR;
			}
		}

		pssl->snd_msg_len += pssl->mac_size;
	}

	if (IS_STREAM_CIPHER(pssl->cipher))
	{
		if (IS_ARC_CIPHER(pssl->cipher))
		{
			arc4_crypt((arc4_context *)pssl->ctx_enc, pssl->snd_msg_len, pssl->snd_msg, pssl->snd_msg);
		}
	}
	else
	{
		if (IS_BLOCK_CIPHER(pssl->cipher))
		{
			padlen = pssl->iv_size - (pssl->snd_msg_len + 1) % pssl->iv_size;
			if (padlen == pssl->iv_size)
				padlen = 0;

			padlen++;
			for (i = 0; i < padlen; i++)
				pssl->snd_msg[pssl->snd_msg_len + i] = (byte_t)(padlen - 1);

			pssl->snd_msg_len += (padlen);
		}

		// TLS 1.1~1.2
		if (pssl->minor_ver == SSL_MINOR_VERSION_2 || pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			/* 
			block-ciphered struct {
			opaque IV[CipherSpec.block_length];
			opaque content[TLSCompressed.length];
			opaque MAC[CipherSpec.hash_size];
			uint8 padding[GenericBlockCipher.padding_length];
			uint8 padding_length;
			} GenericBlockCipher;
			*/
			if (IS_AEAD_CIPHER(pssl->cipher))
			{
				if (pssl->iv_implicit)
				{
					xmem_copy((void*)(pssl->iv_enc + pssl->iv_implicit), (void*)pssl->snd_ctr, (pssl->iv_size - pssl->iv_implicit));
				}
				iv_copy = (pssl->iv_implicit) ? (pssl->iv_size - pssl->iv_implicit) : pssl->iv_size;
				xmem_copy((void*)(iv_pre), (void*)(pssl->iv_enc + pssl->iv_implicit), iv_copy);
			}
			else
			{
				iv_copy = pssl->iv_size;
				xmem_copy((void*)(iv_pre), (void*)(pssl->iv_enc), iv_copy);
			}
		}
		else
		{
			/*
			block-ciphered struct {
			opaque content[TLSCompressed.length];
			opaque MAC[CipherSpec.hash_size];
			uint8 padding[GenericBlockCipher.padding_length];
			uint8 padding_length;
			} GenericBlockCipher;
			*/
			iv_copy = 0;
			xmem_zero((void*)iv_pre, 16);
		}

		if (IS_DES_CIPHER(pssl->cipher))
		{
			des3_crypt_cbc((des3_context *)pssl->ctx_enc, DES_ENCRYPT, pssl->snd_msg_len, pssl->iv_enc, pssl->snd_msg, pssl->snd_msg);
		}
		else if (IS_AES_CIPHER(pssl->cipher))
		{
			aes_crypt_cbc((aes_context *)pssl->ctx_enc, AES_ENCRYPT, pssl->snd_msg_len, pssl->iv_enc, pssl->snd_msg, pssl->snd_msg);
		}
		else if (IS_GCM_CIPHER(pssl->cipher))
		{
			/*
			addlen = 0;
			xmem_copy((void*)gcm_add, pssl->snd_ctr, SSL_CTR_SIZE);
			addlen += SSL_CTR_SIZE;
			gcm_add[addlen] = pssl->snd_msg_type;
			gcm_add[addlen + 1] = (byte_t)(pssl->major_ver);
			gcm_add[addlen + 2] = (byte_t)(pssl->minor_ver);
			addlen += 3;
			PUT_SWORD_NET(gcm_add, addlen, (unsigned short)pssl->snd_msg_len);
			addlen += 2;

			xmem_copy((void*)gcm_iv, (void*)pssl->iv_enc, pssl->iv_size);
			*/

			taglen = 16;
			mac_buf = pssl->snd_msg + pssl->snd_msg_len;

			//if (C_OK != gcm_crypt_and_tag((gcm_context *)pssl->ctx_enc, AES_ENCRYPT, pssl->snd_msg_len, gcm_iv, pssl->iv_size, gcm_add, addlen, pssl->snd_msg, pssl->snd_msg, taglen, mac_buf))
			if (C_OK != gcm_crypt_and_tag((gcm_context *)pssl->ctx_enc, AES_ENCRYPT, pssl->snd_msg_len, pssl->iv_enc, pssl->iv_size, pssl->snd_ctr, (SSL_CTR_SIZE + SSL_HDR_SIZE), pssl->snd_msg, pssl->snd_msg, taglen, mac_buf))
			{
				set_last_error(_T("_ssl_encrypt_snd_msg"), _T("gcm_crypt_and_tag falied"), -1);

				return C_ERR;
			}
			pssl->snd_msg_len += taglen;
		}

		// TLS 1.1~1.2
		if (pssl->minor_ver == SSL_MINOR_VERSION_2 || pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			xmem_move((void*)(pssl->snd_msg), pssl->snd_msg_len, iv_copy);
			xmem_copy((void*)(pssl->snd_msg), (void*)(iv_pre), iv_copy);
			pssl->snd_msg_len += iv_copy;
		}
	}

	//reset message length
	PUT_SWORD_NET(pssl->snd_hdr, 3, (unsigned short)pssl->snd_msg_len);

	return C_OK;
}

static int _ssl_decrypt_rcv_msg(ssl_context *pssl)
{
	int i, n, padlen = 0;
	byte_t* mac_buf;
	byte_t mac_tmp[32];
	int iv_copy;

	//byte_t gcm_add[13] = { 0 };
	//byte_t gcm_iv[12] = { 0 };
	//int addlen;
	int taglen = 16;

	if (IS_AEAD_CIPHER(pssl->cipher))
		iv_copy = (pssl->iv_implicit) ? (pssl->iv_size - pssl->iv_implicit) : pssl->iv_size;
	else
		iv_copy = pssl->iv_size;

	if (IS_BLOCK_CIPHER(pssl->cipher) || IS_STREAM_CIPHER(pssl->cipher))
		n = (pssl->minor_ver == SSL_MINOR_VERSION_2 || pssl->minor_ver == SSL_MINOR_VERSION_3) ? (pssl->mac_size + iv_copy) : pssl->mac_size;
	else
		n = (pssl->minor_ver == SSL_MINOR_VERSION_2 || pssl->minor_ver == SSL_MINOR_VERSION_3) ? (taglen + iv_copy) : taglen;

	if (pssl->rcv_msg_len < n)
	{
		set_last_error(_T("_ssl_decrypt_rcv_msg"), _T("message length to small"), -1);

		return C_ERR;
	}

	if (IS_STREAM_CIPHER(pssl->cipher))
	{
		if (IS_ARC_CIPHER(pssl->cipher))
		{
			arc4_crypt((arc4_context *)pssl->ctx_dec, pssl->rcv_msg_len, pssl->rcv_msg, pssl->rcv_msg);
		}
	}
	else
	{
		if (pssl->minor_ver == SSL_MINOR_VERSION_2 || pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			if (IS_AEAD_CIPHER(pssl->cipher))
				xmem_copy((void*)(pssl->iv_dec + pssl->iv_implicit), (void*)pssl->rcv_msg, iv_copy);
			else
				xmem_copy((void*)(pssl->iv_dec), (void*)pssl->rcv_msg, iv_copy);

			xmem_move((void*)(pssl->rcv_msg + iv_copy), pssl->rcv_msg_len - iv_copy, 0 - iv_copy);

			pssl->rcv_msg_len -= iv_copy;
		}

		if (IS_BLOCK_CIPHER(pssl->cipher))
		{
			if (pssl->rcv_msg_len % pssl->iv_size != 0)
			{
				set_last_error(_T("_ssl_decrypt_rcv_msg"), _T("message length not multiple of IV size"), -1);

				return C_ERR;
			}
		}

		if (IS_DES_CIPHER(pssl->cipher))
		{
			des3_crypt_cbc((des3_context *)pssl->ctx_dec, DES_DECRYPT, pssl->rcv_msg_len, pssl->iv_dec, pssl->rcv_msg, pssl->rcv_msg);
		}
		else if (IS_AES_CIPHER(pssl->cipher))
		{
			aes_crypt_cbc((aes_context *)pssl->ctx_dec, AES_DECRYPT, pssl->rcv_msg_len, pssl->iv_dec, pssl->rcv_msg, pssl->rcv_msg);
		}
		else if (IS_GCM_CIPHER(pssl->cipher))
		{
			taglen = 16;
			pssl->rcv_msg_len -= taglen;
			mac_buf = pssl->rcv_msg + pssl->rcv_msg_len;

			//reset message length
			PUT_SWORD_NET(pssl->rcv_hdr, 3, (unsigned short)pssl->rcv_msg_len);

			/*
			addlen = 0;
			xmem_copy((void*)gcm_add, pssl->rcv_ctr, SSL_CTR_SIZE);
			addlen += SSL_CTR_SIZE;
			gcm_add[addlen] = pssl->rcv_msg_type;
			gcm_add[addlen + 1] = (byte_t)(pssl->major_ver);
			gcm_add[addlen + 2] = (byte_t)(pssl->minor_ver);
			addlen += 3;
			PUT_SWORD_NET(gcm_add, addlen, (unsigned short)(pssl->rcv_msg_len));
			addlen += 2;

			xmem_copy((void*)gcm_iv, (void*)pssl->iv_dec, pssl->iv_size);
			*/

			//if (C_OK != gcm_crypt_and_tag((gcm_context *)pssl->ctx_dec, AES_DECRYPT, pssl->rcv_msg_len, gcm_iv, pssl->iv_size, gcm_add, addlen, pssl->rcv_msg, pssl->rcv_msg, taglen, mac_tmp))
			if (C_OK != gcm_crypt_and_tag((gcm_context *)pssl->ctx_dec, AES_DECRYPT, pssl->rcv_msg_len, pssl->iv_dec, pssl->iv_size, pssl->rcv_ctr, (SSL_CTR_SIZE + SSL_HDR_SIZE), pssl->rcv_msg, pssl->rcv_msg, taglen, mac_tmp))
			{
				set_last_error(_T("_ssl_decrypt_rcv_msg"), _T("gcm_crypt_and_tag falied"), -1);

				return C_ERR;
			}

			if (xmem_comp((void*)mac_tmp, taglen, (void*)mac_buf, taglen) != 0)
			{
				set_last_error(_T("_ssl_decrypt_rcv_msg"), _T("gcm tag checked falied"), -1);

				return C_ERR;
			}
		}

		if (IS_BLOCK_CIPHER(pssl->cipher))
		{
			padlen = pssl->rcv_msg[pssl->rcv_msg_len - 1] + 1;

			if (pssl->minor_ver == SSL_MINOR_VERSION_0)
			{
				if (padlen > pssl->iv_size)
				{
					padlen = 0;
				}
			}
			else
			{
				for (i = 1; i <= padlen; i++)
				{
					if (pssl->rcv_msg[pssl->rcv_msg_len - i] != (padlen - 1))
					{
						padlen = 0;
					}
				}
			}

			if (pssl->iv_size != 0 && padlen == 0)
			{
				set_last_error(_T("_ssl_decrypt_rcv_msg"), _T("invalid message pading length"), -1);

				return C_ERR;
			}
		}
		else
		{
			padlen = 0;
		}

		pssl->rcv_msg_len -= padlen;
	}

	if (IS_BLOCK_CIPHER(pssl->cipher) || IS_STREAM_CIPHER(pssl->cipher))
	{
		pssl->rcv_msg_len -= pssl->mac_size;
		mac_buf = pssl->rcv_msg + pssl->rcv_msg_len;

		//reset message length
		PUT_SWORD_NET(pssl->rcv_hdr, 3, (unsigned short)pssl->rcv_msg_len);

		if (pssl->minor_ver == SSL_MINOR_VERSION_0)
		{
			if (pssl->mac_size == 16)
				_ssl_mac_md5(pssl->mac_dec, pssl->rcv_ctr, pssl->rcv_msg, pssl->rcv_msg_len, pssl->rcv_msg_type, mac_tmp);
			else if (pssl->mac_size == 20)
				_ssl_mac_sha1(pssl->mac_dec, pssl->rcv_ctr, pssl->rcv_msg, pssl->rcv_msg_len, pssl->rcv_msg_type, mac_tmp);
		}
		else
		{
			if (pssl->mac_size == 16)
				md5_hmac(pssl->mac_dec, pssl->mac_size, pssl->rcv_ctr, (pssl->rcv_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE), mac_tmp);
			else if (pssl->mac_size == 20)
				sha1_hmac(pssl->mac_dec, pssl->mac_size, pssl->rcv_ctr, (pssl->rcv_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE), mac_tmp);
			else if (pssl->mac_size == 32) //TLS 1.2
				sha2_hmac(pssl->mac_dec, pssl->mac_size, pssl->rcv_ctr, (pssl->rcv_msg_len + SSL_CTR_SIZE + SSL_HDR_SIZE), mac_tmp, 0);
		}

		if (xmem_comp((void*)mac_tmp, pssl->mac_size, (void*)mac_buf, pssl->mac_size) != 0)
		{
			set_last_error(_T("_ssl_decrypt_rcv_msg"), _T("message signature hash not matched"), -1);

			return C_ERR;
		}
	}

	return C_OK;
}

static int _ssl_write_snd_msg(ssl_context *pssl)
{
	dword_t dw;
	int i, haslen;
	byte_t* token;
	int total;

	//ssl header: 5 bytes
	//0: message type
	//1-2: version
	//3-4: package message length
	PUT_BYTE(pssl->snd_hdr, 0, (byte_t)(pssl->snd_msg_type));
	PUT_BYTE(pssl->snd_hdr, 1, (byte_t)(pssl->major_ver));
	PUT_BYTE(pssl->snd_hdr, 2, (byte_t)(pssl->minor_ver));
	PUT_SWORD_NET(pssl->snd_hdr, 3, (unsigned short)(pssl->snd_msg_len));

	if (pssl->snd_msg_type == SSL_MSG_HANDSHAKE)
	{
		token = pssl->snd_msg;
		total = pssl->snd_msg_len;
		while (total)
		{
			haslen = GET_THREEBYTE_LEN(token, 1);

			md5_update(&pssl->md5, token, SSL_HSH_SIZE + haslen);
			sha1_update(&pssl->sha1, token, SSL_HSH_SIZE + haslen);
			sha2_update(&pssl->sha2, token, SSL_HSH_SIZE + haslen);

			total -= (SSL_HSH_SIZE + haslen);
			token += (SSL_HSH_SIZE + haslen);
		}
	}

	if (pssl->snd_crypted)
	{
		if (C_OK != _ssl_encrypt_snd_msg(pssl))
		{
			set_last_error(_T("_ssl_write_snd_msg"), _T("encrypt message block failed"), -1);
			return C_ERR;
		}

		//incre send message control bits
		for (i = SSL_CTR_SIZE - 1; i >= 0; i--)
		{
			if (++pssl->snd_ctr[i] != 0)
				break;
		}
	}

	dw = SSL_HDR_SIZE + pssl->snd_msg_len;

	if (!(*pssl->pif->pf_write)(pssl->pif->fd, pssl->snd_hdr, &dw))
	{
		set_last_error(_T("_ssl_write_snd_msg"), _T("write message block failed"), -1);
		return C_ERR;
	}

	pssl->snd_msg_pop = 0;

	return C_OK;
}

static bool_t _ssl_write_data(ssl_context* pssl, byte_t* buf, int* need)
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

static bool_t _ssl_flush_data(ssl_context* pssl)
{
	if (!pssl->snd_msg_pop)
		return 1;

	pssl->snd_msg_len = pssl->snd_msg_pop;
	pssl->snd_msg_type = SSL_MSG_APPLICATION_DATA;

	return (C_OK == _ssl_write_snd_msg(pssl)) ? 1 : 0;
}

static bool_t _ssl_write_close(ssl_context* pssl)
{
	pssl->over = -1;

	pssl->snd_msg_type = SSL_MSG_ALERT;
	pssl->snd_msg_len = 2;
	pssl->snd_msg[0] = SSL_ALERT_WARNING;
	pssl->snd_msg[1] = SSL_ALERT_CLOSE_NOTIFY;

	return (C_OK == _ssl_write_snd_msg(pssl)) ? 1 : 0;
}

static int _ssl_read_rcv_msg(ssl_context *pssl)
{
	dword_t dw;
	int i, haslen;
	byte_t* token;
	int total;

	dw = SSL_HDR_SIZE;
	if (!(pssl->pif->pf_read)(pssl->pif->fd, pssl->rcv_hdr, &dw))
	{
		set_last_error(_T("_ssl_read_rcv_msg"), _T("read message head failed"), -1);
		return C_ERR;
	}
    
    if(!dw)
    {
		xmem_zero((void*)pssl->rcv_hdr, SSL_HDR_SIZE);
		return C_OK;
    }

	if (pssl->rcv_hdr[1] != SSL_MAJOR_VERSION_3)
	{
		set_last_error(_T("_ssl_read_rcv_msg"), _T("major version mismatch"), -1);
		return C_ERR;
	}

	if (pssl->rcv_hdr[2] > SSL_MINOR_VERSION_3)
	{
		set_last_error(_T("_ssl_read_rcv_msg"), _T("minor version mismatch"), -1);
		return C_ERR;
	}

	pssl->rcv_msg_type = GET_BYTE(pssl->rcv_hdr, 0);
	pssl->rcv_msg_len = GET_SWORD_NET(pssl->rcv_hdr, 3);

	if (pssl->rcv_msg_len < 1 || pssl->rcv_msg_len > SSL_MAX_SIZE)
	{
		set_last_error(_T("_ssl_read_rcv_msg"), _T("invalid message block length"), -1);
		return C_ERR;
	}

	dw = pssl->rcv_msg_len;
	if (!(*pssl->pif->pf_read)(pssl->pif->fd, pssl->rcv_msg, &dw))
	{
		set_last_error(_T("_ssl_read_rcv_msg"), _T("read message block failed"), -1);
		return C_ERR;
	}

	if (pssl->rcv_crypted)
	{
		if (C_OK != _ssl_decrypt_rcv_msg(pssl))
		{
			set_last_error(_T("_ssl_read_rcv_msg"), _T("decrypt message block failed"), -1);
			return C_ERR;
		}

		//incre recv message control bits
		for (i = SSL_CTR_SIZE - 1; i >= 0; i--)
		{
			if (++pssl->rcv_ctr[i] != 0)
				break;
		}
	}

	if (pssl->rcv_msg_type == SSL_MSG_HANDSHAKE)
	{
		total = pssl->rcv_msg_len;
		token = pssl->rcv_msg;
		while (total)
		{
			haslen = GET_THREEBYTE_LEN(token, 1);

			md5_update(&pssl->md5, token, SSL_HSH_SIZE + haslen);
			sha1_update(&pssl->sha1, token, SSL_HSH_SIZE + haslen);
			sha2_update(&pssl->sha2, token, SSL_HSH_SIZE + haslen);

			total -= (SSL_HSH_SIZE + haslen);
			token += (SSL_HSH_SIZE + haslen);
		}
	}
	else if (pssl->rcv_msg_type == SSL_MSG_ALERT)
	{
		if (pssl->rcv_msg[0] == SSL_ALERT_FATAL)
		{
			set_last_error(_T("_ssl_read_rcv_msg"), _T("fatal alert message"), -1);
			return C_ERR;
		}

		if (pssl->rcv_msg[0] == SSL_ALERT_WARNING && pssl->rcv_msg[1] == SSL_ALERT_CLOSE_NOTIFY)
		{
			pssl->over = -1;
			pssl->rcv_msg_len = 0;
		}
	}

	pssl->rcv_msg_pop = 0;

	return C_OK;
}

static bool_t _ssl_read_data(ssl_context* pssl, byte_t* buf, int* need)
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
			set_last_error(_T("_ssl_read_data"), _T("not application data"), -1);
			return 0;
		}
	}

	*need = (*need + pssl->rcv_msg_pop < pssl->rcv_msg_len) ? (*need) : (pssl->rcv_msg_len - pssl->rcv_msg_pop);
	
	xmem_copy(buf, pssl->rcv_msg + pssl->rcv_msg_pop, *need);
	pssl->rcv_msg_pop += (*need);

	return 1;
}

/***************************************client routing************************************************************/

static handshake_states _ssl_write_client_hello(ssl_context *pssl)
{
	/*
	struct {
		ProtocolVersion client_version;
		struct {
              uint32 gmt_unix_time;
              opaque random_bytes[28];
           } Random;
		SessionID session_id;
		CipherSuite cipher_suites<2..2 ^ 16 - 1>;
		CompressionMethod compression_methods<1..2 ^ 8 - 1>;
		TLS1.2-> select (extensions_present) {
			case false:
			struct {};
			case true:
			Extension extensions<0..2^16-1>;
			};
	}Client Hello
	*/

	int i, n, len, pos, extlen;
	int msglen = SSL_HSH_SIZE;
	dword_t t;

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

	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)(pssl->cli_major_ver));
	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)(pssl->cli_minor_ver));
	
	xmem_copy(pssl->snd_msg + msglen, pssl->rnd_cli, SSL_RND_SIZE);
	msglen += SSL_RND_SIZE;
	
	//38: session id length (n)
	//39~39+n: session id
	//40+n~41+n: cipherlist length
	//42+n: cipherlist
	//compression alg. (0)
	//extensions (unused)

	n = pssl->ses_size;
	if (n < 16 || n > 32 || pssl->resumed == 0)
		n = 0;

	//sension id length
	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)n);

	//sension id
	xmem_copy(pssl->snd_msg + msglen, pssl->ses_id, n);
	msglen += n;

	n = sizeof(client_ciphers) / sizeof(ciphers_set);

	//cipher list length
	PUT_SWORD_NET(pssl->snd_msg, msglen, n * 2);
	msglen += 2;

	for (i = 0; i < n; i++)
	{
		//cipher
		PUT_SWORD_NET(pssl->snd_msg, msglen, client_ciphers[i].cipher);
		msglen += 2;
	}

	//compression methods length
	PUT_BYTE(pssl->snd_msg, msglen++, 1);

	//compression method (null)
	PUT_BYTE(pssl->snd_msg, msglen++, 0);

	//position for reseting extension length
	pos = msglen;
	extlen = 0;

	// preset Extensions length to zero
	PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(0));
	msglen += 2;

	if (!a_is_null(pssl->host_cn))
	{
		len = a_xslen(pssl->host_cn);

		// Extension type: Server name(0)
		PUT_SWORD_NET(pssl->snd_msg, msglen, SSL_EXTENSION_SERVERNAME);
		msglen += 2;
		extlen += 2;

		// Server name extension length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(len + 5));
		msglen += 2;
		extlen += 2;

		// Server name list count length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(len + 3));
		msglen += 2;
		extlen += 2;

		// Host name type
		PUT_BYTE(pssl->snd_msg, msglen, 0);
		msglen++;
		extlen++;

		// Host name length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(len));
		msglen += 2;
		extlen += 2;

		// Host name
		xmem_copy(pssl->snd_msg + msglen, pssl->host_cn, len);
		msglen += len;
		extlen += len;
	}
	
	if (pssl->minor_ver == SSL_MINOR_VERSION_3) //TLS 1.2
	{
		/*struct {
			NamedCurve elliptic_curve_list<1..2 ^ 16 - 1>
		} EllipticCurveList;
		*/
		// Extension type: supported_groups (10)
		PUT_SWORD_NET(pssl->snd_msg, msglen, SSL_EXTENSION_SUPPORTEDGROUPS);
		msglen += 2;
		extlen += 2;

		// supported_groups extension length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(6));
		msglen += 2;
		extlen += 2;

		// Elliptic curve list length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(4));
		msglen += 2;
		extlen += 2;

		// Supported Group: secp256r1 (0x0017)
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(23));
		msglen += 2;
		extlen += 2;

		// Supported Group: secp384r1 (0x0018)
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(24));
		msglen += 2;
		extlen += 2;

		/*struct {
			ECPointFormat ec_point_format_list<1..2 ^ 8 - 1>
		} ECPointFormatList;
		*/
		// Extension type: ec_point_formats (11)
		PUT_SWORD_NET(pssl->snd_msg, msglen, SSL_EXTENSION_ECPOINTFORMATS);
		msglen += 2;
		extlen += 2;

		// ec_point_format extension length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(2));
		msglen += 2;
		extlen += 2;

		//EC point formats Length: 1
		PUT_BYTE(pssl->snd_msg, msglen, (1));
		msglen++;
		extlen++;

		//EC point format: uncompressed (0)
		PUT_BYTE(pssl->snd_msg, msglen, (0));
		msglen++;
		extlen++;

		/*
		struct {
			ExtensionType extension_type;
			opaque extension_data<0..2^16-1>;
		} Extension;
		struct {
			HashAlgorithm hash;
			SignatureAlgorithm signature;
		} SignatureAndHashAlgorithm;
		*/

		// Extension type: Signature and Hash algorithm(13)
		PUT_SWORD_NET(pssl->snd_msg, msglen, SSL_EXTENSION_SIGNATUREANDHASHALGORITHM);
		msglen += 2;
		extlen += 2;

		// Signature and Hash extension length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(4));
		msglen += 2;
		extlen += 2;

		// Signature and Hash algorithm list count length
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(2));
		msglen += 2;
		extlen += 2;

		//HashAlgorithm
		PUT_BYTE(pssl->snd_msg, msglen, ALG_HASH_SHA1);
		msglen++;
		extlen++;

		//SignatureAlgorithm
		PUT_BYTE(pssl->snd_msg, msglen, ALG_SIGN_RSA);
		msglen++;
		extlen++;
	}

	if (!extlen && pssl->cli_minor_ver < SSL_MINOR_VERSION_3)
	{
		//remove the extension length if empty
		msglen -= 2;
	}
	else
	{
		// reset extension length
		PUT_SWORD_NET(pssl->snd_msg, pos, (unsigned short)(extlen));
	}

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake type
	PUT_BYTE(pssl->snd_msg, 0, (byte_t)SSL_HS_CLIENT_HELLO);
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_SERVER_HELLO : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_parse_server_hello(ssl_context *pssl)
{
	/*
	struct {
		ProtocolVersion server_version;
		struct {
              uint32 gmt_unix_time;
              opaque random_bytes[28];
           } Random;
		SessionID session_id;
		CipherSuite cipher_suite;
		CompressionMethod compression_method;
		TLS1.2-> select (extensions_present) {
			case false:
			struct {};
			case true:
			Extension extensions<0..2^16-1>;
			};
	} ServerHello;
	*/

	dword_t t;
	int ciph;
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
		set_last_error(_T("_ssl_parse_server_hello"), _T("not handshake message"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg[4] != SSL_MAJOR_VERSION_3 || pssl->rcv_msg[5] > SSL_MINOR_VERSION_3)
	{
		set_last_error(_T("_ssl_parse_server_hello"), _T("handshake hello message version checked failed"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_len < 38)
	{
		set_last_error(_T("_ssl_parse_server_hello"), _T("handshake hello message block too short"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);
	
	msglen = SSL_HSH_SIZE;
	pssl->srv_major_ver = GET_BYTE(pssl->rcv_msg, msglen);
	pssl->srv_minor_ver = GET_BYTE(pssl->rcv_msg, msglen + 1);
	msglen += 2;

	if (pssl->cli_major_ver != pssl->srv_major_ver)
	{
		set_last_error(_T("_ssl_parse_server_hello"), _T("handshake major version mistach"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	pssl->major_ver = pssl->cli_major_ver;
	pssl->minor_ver = (pssl->cli_minor_ver < pssl->srv_minor_ver) ? pssl->cli_minor_ver : pssl->srv_minor_ver;

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

	if (pssl->resumed == 0 || pssl->cipher != ciph || a_xslen(pssl->ses_id) != seslen || xmem_comp(pssl->ses_id, seslen, pssl->rcv_msg + msglen, seslen) != 0)
	{
		pssl->resumed = 0;
		xmem_copy(pssl->ses_id, pssl->rcv_msg + msglen, seslen);
		pssl->ses_size = seslen;

		if (!_ssl_choose_cipher(pssl, ciph))
		{
			set_last_error(_T("_ssl_parse_server_hello"), _T("unknown cipher type"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}

	//session id copyed
	msglen += seslen;

	//cipher choosed
	msglen += 2;

	//skip compression alg
	msglen++;

	//has no extension
	if (msglen == haslen + SSL_HSH_SIZE)
		return SSL_SERVER_CERTIFICATE;
	
	//extension length
	extlen = GET_SWORD_NET(pssl->rcv_msg, msglen);
	msglen += 2;

	//skip extension
	msglen += extlen;

	if (haslen + SSL_HSH_SIZE != msglen)
	{
		set_last_error(_T("_ssl_parse_server_hello"), _T("invalid server hello message session block"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	//if multiply handshake message
	if (pssl->rcv_msg_len > msglen)
	{
		xmem_move((pssl->rcv_msg + msglen), (pssl->rcv_msg_len - msglen), -msglen);
		pssl->rcv_msg_len -= msglen;
	}

	return SSL_SERVER_CERTIFICATE;
}

handshake_states _ssl_parse_server_certificate(ssl_context *pssl)
{
	/*
	opaque ASN.1Cert<1..2 ^ 24 - 1>;

	struct {
		ASN.1Cert certificate_list<0..2 ^ 24 - 1>;
	} Certificate;
	*/

	int  ret, n;
	int msglen, haslen, crtlen;

	if (pssl->rcv_msg[0] != SSL_HS_CERTIFICATE)
	{
		if (C_OK != _ssl_read_rcv_msg(pssl))
		{
			return SSL_HANDSHAKE_ERROR;
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
		set_last_error(_T("_ssl_parse_server_certificate"), _T("invalid certificate message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);

	msglen = SSL_HSH_SIZE;
	crtlen = GET_THREEBYTE_LEN(pssl->rcv_msg, msglen);
	msglen += 3;

	if (haslen != 3 + crtlen)
	{
		set_last_error(_T("_ssl_parse_server_certificate"), _T("invalid certificate block size"), -1);
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
			set_last_error(_T("_ssl_parse_server_certificate"), _T("invalid certificate size"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != x509parse_crt(pssl->crt_pe, pssl->rcv_msg + msglen, n))
		{
			set_last_error(_T("_ssl_parse_server_certificate"), _T("invalid certificate context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		msglen += n;
		crtlen -= n;
	}

	//if multiply handshake message
	if (pssl->rcv_msg_len > msglen)
	{
		xmem_move((pssl->rcv_msg + msglen), (pssl->rcv_msg_len - msglen), -msglen);
		pssl->rcv_msg_len -= msglen;
	}

	if (pssl->verify_server != SSL_VERIFY_NONE)
	{
		if (pssl->crt_ca == NULL)
		{
			set_last_error(_T("_ssl_parse_server_certificate"), _T("CA chian empty"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (pssl->verify_server != SSL_VERIFY_REQUIRED)
		{
			return SSL_SERVER_KEY_EXCHANGE;
		}

		if (C_OK != x509parse_verify(pssl->crt_pe, pssl->crt_ca, pssl->peer_cn, &ret))
		{
			set_last_error(_T("_ssl_parse_server_certificate"), _T("certificate verify failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}

	return (IS_DHE_CIPHER(pssl->cipher) || IS_ECDHE_CIPHER(pssl->cipher)) ? SSL_SERVER_KEY_EXCHANGE : SSL_CERTIFICATE_REQUEST;
}

static int _ssl_parse_server_key_exchange(ssl_context *pssl)
{
	/* TLS 1.0/1.1
	struct {
		select (KeyExchangeAlgorithm) {
		case diffie_hellman:
			ServerDHParams params;
			Signature signed_params;
		case rsa:
			ServerRSAParams params;
			Signature signed_params;
		};
	} ServerKeyExchange;
	*/
	/* TLS1.2
	struct {
	select (KeyExchangeAlgorithm) {
		case dh_anon:
			ServerDHParams params;
		case dhe_dss:
		case dhe_rsa:
			ServerDHParams params;
			digitally-signed struct {
				opaque client_random[32];
				opaque server_random[32];
			ServerDHParams params;
			} signed_params;
		case rsa:
		case dh_dss:
		case dh_rsa:
			struct {} ;
		};
	} ServerKeyExchange;
	*/
	int n, haslen, msglen;
	byte_t *p, *end;
	byte_t hash[36];
	md5_context md5;
	sha1_context sha1;
	sha2_context sha2;
	int alg_hash, alg_sign;

	if (pssl->rcv_msg[0] != SSL_HS_SERVER_KEY_EXCHANGE)
	{
		if (C_OK != _ssl_read_rcv_msg(pssl))
		{
			return SSL_HANDSHAKE_ERROR;
		}
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

	msglen = SSL_HSH_SIZE;

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);

	p = pssl->rcv_msg + SSL_HSH_SIZE;
	end = p + haslen;

	if (IS_DHE_CIPHER(pssl->cipher))
	{
		if (!pssl->dhm_ow)
		{
			pssl->dhm_ow = (dhm_context*)xmem_alloc(sizeof(dhm_context));
		}

		if (C_OK != dhm_read_params(pssl->dhm_ow, &p, end))
		{
			set_last_error(_T("_ssl_parse_server_key_exchange"), _T("server key exchange read dhm params error"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (pssl->dhm_ow->len < 64 || pssl->dhm_ow->len > 256)
		{
			set_last_error(_T("_ssl_parse_server_key_exchange"), _T("invalid server key exchange message context length"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}
	else if (IS_ECDHE_CIPHER(pssl->cipher))
	{
		if (!pssl->ecdh_ow)
		{
			pssl->ecdh_ow = (ecdh_context*)xmem_alloc(sizeof(ecdh_context));
			
			ecdh_init(pssl->ecdh_ow);

			if (C_OK != ecdh_read_params(pssl->ecdh_ow, (const unsigned char **)&p, end))
			{
				set_last_error(_T("_ssl_parse_server_key_exchange"), _T("server key exchange read ecdh params error"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
	}
	else
	{
		set_last_error(_T("_ssl_parse_server_key_exchange"), _T("invalid server key exchange cipher"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		alg_hash = *p;
		p++;
		msglen++;

		alg_sign = *p;
		p++;
		msglen++;

		n = GET_SWORD_NET(p, 0);
		p += 2;
		msglen += 2;
	}
	else
	{
		n = GET_SWORD_NET(p, 0);
		p += 2;
		msglen += 2;
	}

	if ((int)(end - p) != pssl->crt_pe->rsa.len)
	{
		set_last_error(_T("_ssl_parse_server_key_exchange"), _T("invalid server key exchange message context rsa key"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		n = haslen - (end - p) - 2 - 2;

		if (alg_hash == RSA_SHA256)
		{
			sha2_starts(&sha2, 0);
			sha2_update(&sha2, pssl->rnd_cli, SSL_RND_SIZE);
			sha2_update(&sha2, pssl->rnd_srv, SSL_RND_SIZE);
			sha2_update(&sha2, pssl->rcv_msg + SSL_HSH_SIZE, n);
			sha2_finish(&sha2, hash);

			if (C_OK != rsa_pkcs1_verify(&pssl->crt_pe->rsa, RSA_PUBLIC, RSA_SHA256, 32, hash, p))
			{
				set_last_error(_T("_ssl_parse_server_key_exchange"), _T("invalid server key exchange message context verify"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
		else
		{
			sha1_starts(&sha1);
			sha1_update(&sha1, pssl->rnd_cli, SSL_RND_SIZE);
			sha1_update(&sha1, pssl->rnd_srv, SSL_RND_SIZE);
			sha1_update(&sha1, pssl->rcv_msg + SSL_HSH_SIZE, n);
			sha1_finish(&sha1, hash);

			if (C_OK != rsa_pkcs1_verify(&pssl->crt_pe->rsa, RSA_PUBLIC, RSA_SHA1, 20, hash, p))
			{
				set_last_error(_T("_ssl_parse_server_key_exchange"), _T("invalid server key exchange message context verify"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
	}
	else
	{
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
			set_last_error(_T("_ssl_parse_server_key_exchange"), _T("invalid server key exchange message context verify"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}

	msglen += (n + pssl->crt_pe->rsa.len);

	//if multiply handshake message
	if (pssl->rcv_msg_len > msglen)
	{
		xmem_move((pssl->rcv_msg + msglen), (pssl->rcv_msg_len - msglen), -msglen);
		pssl->rcv_msg_len -= msglen;
	}

	return SSL_CERTIFICATE_REQUEST;
}

static int _ssl_parse_server_certificate_request(ssl_context *pssl)
{
	/*
	enum { rsa_sign(1), dss_sign(2), rsa_fixed_dh(3), dss_fixed_dh(4), (255) } ClientCertificateType;

	opaque DistinguishedName<1..2^16-1>;

	struct {
		ClientCertificateType certificate_types<1..2^8-1>;
		DistinguishedName certificate_authorities<3..2^16-1>;
	} CertificateRequest;
	*/
	/* TLS1.2
	struct {
        ClientCertificateType certificate_types<1..2^8-1>;
        SignatureAndHashAlgorithm
            supported_signature_algorithms<2^16-1>;
        DistinguishedName certificate_authorities<0..2^16-1>;
      } CertificateRequest;
	*/
	int haslen, msglen = SSL_HSH_SIZE;
	int n, crttype, dsnlen, alg_hash, alg_sign;

	//0:	handshake type
	//1~3:	handshake length
	//4 :	cert type count
	//5~n-1:	cert types
	//n~n+1:	length of all DNs
	//n+2~n+3:	length of DN 1
	//n+4:	Distinguished Name #1
	// ...  length of DN 2, etc.

	if (pssl->rcv_msg[0] != SSL_HS_CERTIFICATE_REQUEST)
	{
		if (C_OK != _ssl_read_rcv_msg(pssl))
			return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE)
	{
		set_last_error(_T("_ssl_parse_server_certificate_request"), _T("invalid certificate request message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	pssl->authen_client = (pssl->rcv_msg[0] == SSL_HS_CERTIFICATE_REQUEST) ? 1 : 0;

	if (pssl->authen_client == 0)
	{
		return SSL_SERVER_HELLO_DONE;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);

	//certificate_types count
	n = GET_BYTE(pssl->rcv_msg, msglen);
	msglen++;

	//certificate_types
	while (n--)
	{
		crttype = GET_BYTE(pssl->rcv_msg, msglen);
		msglen++;
	}

	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		//SignatureAndHashAlgorithm length
		n = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;

		while (n)
		{
			alg_hash = GET_BYTE(pssl->rcv_msg, msglen);
			msglen++;

			alg_sign = GET_BYTE(pssl->rcv_msg, msglen);
			msglen++;

			n -= 2;
		}
	}

	//all DistinguishedName length
	dsnlen = GET_SWORD_NET(pssl->rcv_msg, msglen);
	msglen += 2;

	while (dsnlen)
	{
		n = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;
		dsnlen -= 2;

		if (n > dsnlen)
		{
			set_last_error(_T("_ssl_parse_server_certificate_request"), _T("invalid certificate size"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		msglen += n;
		dsnlen -= n;
	}

	//if multiply handshake message
	if (pssl->rcv_msg_len > msglen)
	{
		xmem_move((pssl->rcv_msg + msglen), (pssl->rcv_msg_len - msglen), -msglen);
		pssl->rcv_msg_len -= msglen;
	}

	return SSL_SERVER_HELLO_DONE;
}

static int _ssl_parse_server_hello_done(ssl_context *pssl)
{
	/*
	struct { } ServerHelloDone;
	*/

	//0:	handshake type
	//1~3:	handshake length

	if (pssl->authen_client != 0)
	{
		if (pssl->rcv_msg[0] != SSL_HS_SERVER_HELLO_DONE)
		{
			if (C_OK != _ssl_read_rcv_msg(pssl))
			{
				return SSL_HANDSHAKE_ERROR;
			}
		}

		if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE)
		{
			set_last_error(_T("_ssl_parse_server_hello_done"), _T("not handshake message"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}

	if (pssl->rcv_msg[0] != SSL_HS_SERVER_HELLO_DONE)
	{
		set_last_error(_T("_ssl_parse_server_hello_done"), _T("invalid server hello message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->authen_client) ? SSL_CLIENT_CERTIFICATE : SSL_CLIENT_KEY_EXCHANGE;
}

static handshake_states _ssl_write_client_certificate(ssl_context *pssl)
{
	/*
	opaque ASN.1Cert<1..2 ^ 24 - 1>;

	struct {
		ASN.1Cert certificate_list<0..2 ^ 24 - 1>;
	} Certificate;
	*/

	int n, lenpos, crtlen, msglen = SSL_HSH_SIZE;
	x509_cert *crt;

	XDL_ASSERT(pssl->authen_client != 0);

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

	//preset certs length
	PUT_THREEBYTE_LEN(pssl->snd_msg, msglen, 0);
	lenpos = msglen;
	msglen += 3;

	crtlen = 0;
	crt = pssl->crt_ow;
	while (crt != NULL && crt->next != NULL)
	{
		n = crt->raw.len;
		if (msglen + 3 + n > SSL_PKG_SIZE)
		{
			set_last_error(_T("_ssl_write_client_certificate"), _T("message package overwrited"), -1);
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

	//reset certs length
	PUT_THREEBYTE_LEN(pssl->snd_msg, lenpos, crtlen);
	
	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_CERTIFICATE;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_CLIENT_KEY_EXCHANGE : SSL_HANDSHAKE_ERROR;
}

static int _ssl_write_client_key_exchange(ssl_context *pssl)
{
	/* TLS 1.0/1.1
	struct {
		select (KeyExchangeAlgorithm) {
		case rsa: 
			EncryptedPreMasterSecret;
		case diffie_hellman: 
			ClientDiffieHellmanPublic;
		} exchange_keys;
	} ClientKeyExchange;
	*/
	/* TLS 1.2
	struct {
		select (KeyExchangeAlgorithm) {
		case rsa:
			EncryptedPreMasterSecret;
		case dhe_dss:
		case dhe_rsa:
		case dh_dss:
		case dh_rsa:
		case dh_anon:
			ClientDiffieHellmanPublic;
		} exchange_keys;
	} ClientKeyExchange;
	*/

	int pos, n, msglen = SSL_HSH_SIZE;
	byte_t premaster[SSL_BLK_SIZE] = {0};
	int prelen = SSL_MST_SIZE;
	size_t m;

	//0:	handshake type
	//1~3:	handshake length
	//4~n:	dhm parameters
	//4+n~5+n:	rsa key length
	//6+n:` rsa key hash

	if (IS_DHE_CIPHER(pssl->cipher))
	{
		/*
		struct {
			select (PublicValueEncoding) {
			case implicit: struct { };
			case explicit: opaque dh_Yc<1..2^16-1>;
			} dh_public;
		} ClientDiffieHellmanPublic;
		*/

		n = pssl->dhm_ow->len;

		pos = msglen;
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)n);
		msglen += 2;

		m = mpi_size(&(pssl->dhm_ow->P)); //256
		if (C_OK != dhm_make_public(pssl->dhm_ow, (int)m, pssl->snd_msg + msglen, &n, havege_rand, &pssl->rng))
		{
			set_last_error(_T("_ssl_write_client_key_exchange"), _T("make public dhm error"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		msglen += n;

		//if n changed, reset key len
		PUT_SWORD_NET(pssl->snd_msg, pos, (unsigned short)n);

		prelen = pssl->dhm_ow->len;

		if (C_OK != dhm_calc_secret(pssl->dhm_ow, premaster, &prelen))
		{
			set_last_error(_T("_ssl_write_client_key_exchange"), _T("cacl dhm secret error"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}
	else if (IS_ECDHE_CIPHER(pssl->cipher))
	{
		m = 0;
		if (C_OK != ecdh_make_public(pssl->ecdh_ow, &m, pssl->snd_msg + msglen, 4096, havege_rand_bytes, &pssl->rng))
		{
			set_last_error(_T("_ssl_write_client_key_exchange"), _T("make public ecdh error"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		msglen += (int)m;

		m = 0;
		if (C_OK != ecdh_calc_secret(pssl->ecdh_ow, &m, premaster, 256, havege_rand_bytes, &pssl->rng))
		{
			set_last_error(_T("_ssl_write_client_key_exchange"), _T("cacl ecdh secret error"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		prelen = (int)m;
	}
	else
	{
		/*
		struct {
			ProtocolVersion client_version;
			opaque random[46];
		} PreMasterSecret;

		public-key-encrypted PreMasterSecret pre_master_secret;
		*/

		premaster[0] = (byte_t)pssl->major_ver;
		premaster[1] = (byte_t)pssl->minor_ver;
		prelen = SSL_MST_SIZE;

		for (n = 2; n < prelen; n++)
			premaster[n] = (byte_t)havege_rand(&pssl->rng);

		n = pssl->crt_pe->rsa.len;

		if (pssl->minor_ver != SSL_MINOR_VERSION_0)
		{
			PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)n);
			msglen += 2;
		}

		if (C_OK != rsa_pkcs1_encrypt(&pssl->crt_pe->rsa, RSA_PUBLIC, prelen, premaster, pssl->snd_msg + msglen))
		{
			set_last_error(_T("_ssl_write_client_key_exchange"), _T("rsa encrypt pre master secret error"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		msglen += n;
	}

	_ssl_derive_keys(pssl, premaster, prelen);

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

	return (pssl->authen_client) ? SSL_CERTIFICATE_VERIFY : SSL_CLIENT_CHANGE_CIPHER_SPEC;
}

static int _ssl_write_client_certificate_verify(ssl_context *pssl)
{
	/*
	struct {
		Signature signature;
	} CertificateVerify;
	*/

	int n, msglen = SSL_HSH_SIZE;
	byte_t hash[36];
	md5_context md5;
	sha1_context sha1;
	sha2_context sha2; //TLS1.2
	byte_t pad_1[48];
	byte_t pad_2[48];

	XDL_ASSERT(pssl->authen_client != 0);
	
	if (pssl->rsa_ow == NULL)
	{
		set_last_error(_T("_ssl_write_client_certificate_verify"), _T("no private key"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));
	xmem_copy(&sha2, &pssl->sha2, sizeof(sha2_context));

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(pad_1, 0x36, SSL_MST_SIZE);
		xmem_set(pad_2, 0x5C, SSL_MST_SIZE);

		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, pad_1, SSL_MST_SIZE);
		md5_finish(&md5, hash);

		md5_starts(&md5);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, pad_2, SSL_MST_SIZE);
		md5_update(&md5, hash, 16);
		md5_finish(&md5, hash);

		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, pad_1, 40);
		sha1_finish(&sha1, hash + 16);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, pad_2, 40);
		sha1_update(&sha1, hash + 16, 20);
		sha1_finish(&sha1, hash + 16);
	}
	else
	{
		if (pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			sha2_finish(&sha2, hash);
		}
		else
		{
			md5_finish(&md5, hash);
			sha1_finish(&sha1, hash + 16);
		}
	}

	//0:	handshake type
	//1~3:	handshake length
	//4~5:	rsa key length
	//6~n:  rsa key hash

	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		PUT_BYTE(pssl->snd_msg, msglen, ALG_HASH_SHA256);
		msglen++;

		PUT_BYTE(pssl->snd_msg, msglen, ALG_SIGN_RSA);
		msglen++;

		n = pssl->rsa_ow->len;
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)n);
		msglen += 2;

		if (C_OK != rsa_pkcs1_sign(pssl->rsa_ow, RSA_PRIVATE, RSA_RAW, 32, hash, pssl->snd_msg + msglen))
		{
			set_last_error(_T("_ssl_write_client_certificate_verify"), _T("rsa signature hash error"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}
	else
	{
		n = pssl->rsa_ow->len;
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)n);
		msglen += 2;

		if (C_OK != rsa_pkcs1_sign(pssl->rsa_ow, RSA_PRIVATE, RSA_RAW, 36, hash, pssl->snd_msg + msglen))
		{
			set_last_error(_T("_ssl_write_client_certificate_verify"), _T("rsa signature hash error"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
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

static handshake_states _ssl_write_client_change_cipher_spec(ssl_context *pssl)
{
	int i;
	/*
	struct {
		enum { change_cipher_spec(1), (255) } type;
	} ChangeCipherSpec;
	*/

	pssl->snd_msg_type = SSL_MSG_CHANGE_CIPHER_SPEC;
	pssl->snd_msg_len = 1;
	pssl->snd_msg[0] = 1;

	//clear send message control bits
	for (i = SSL_CTR_SIZE - 1; i >= 0; i--)
	{
		pssl->snd_ctr[i] = 0;
	}

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	//after send change cipher all record must be crypted sending
	pssl->snd_crypted = 1;

	return SSL_CLIENT_FINISHED;
}

static handshake_states _ssl_write_client_finished(ssl_context *pssl)
{
	/*
	struct {
		opaque verify_data[12];
	} Finished;
	verify_data = PRF(master_secret, finished_label, MD5(handshake_messages) + SHA-1(handshake_messages)) [0..11];
	*/

	int msglen = SSL_HSH_SIZE;
	md5_context  md5;
	sha1_context sha1;
	sha2_context sha2;
	byte_t padbuf[48] = {0};
	byte_t md5sum[16] = {0};
	byte_t sha1sum[20] = {0};
	byte_t* mac_buf;

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));
	xmem_copy(&sha2, &pssl->sha2, sizeof(sha2_context));

	//0:	handshake type
	//1~3:	handshake length
	//4~16:	verify data

	mac_buf = pssl->snd_msg + msglen;

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(padbuf, 0x36, 48);

		md5_update(&md5, "CLNT", 4);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_finish(&md5, md5sum);

		sha1_update(&sha1, "CLNT", 4);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_finish(&sha1, sha1sum);

		xmem_set(padbuf, 0x5C, 48);

		md5_starts(&md5);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_update(&md5, md5sum, 16);
		md5_finish(&md5, mac_buf);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_update(&sha1, sha1sum, 20);
		sha1_finish(&sha1, mac_buf + 16);

		msglen += 36;
	}
	else
	{
		if (pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			sha2_finish(&sha2, padbuf); //32 bytes

			_ssl_prf2(pssl->master_secret, SSL_MST_SIZE, label_client_finished, padbuf, 32, mac_buf, 12);
		}
		else
		{
			md5_finish(&md5, padbuf); //16 bytes
			sha1_finish(&sha1, padbuf + 16); //20 bytes

			_ssl_prf1(pssl->master_secret, SSL_MST_SIZE, label_client_finished, padbuf, 36, mac_buf, 12);
		}

		msglen += 12;
	}

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_FINISHED;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->resumed) ? SSL_HANDSHAKE_OVER : SSL_SERVER_CHANGE_CIPHER_SPEC;
}

static handshake_states _ssl_parse_server_change_cipher_spec(ssl_context *pssl)
{
	int i;
	/*
	struct {
	enum { change_cipher_spec(1), (255) } type;
	} ChangeCipherSpec;
	*/

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_type != SSL_MSG_CHANGE_CIPHER_SPEC)
	{
		set_last_error(_T("_ssl_parse_server_change_cipher_spec"), _T("invalid change cipher spec message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_len != 1 || pssl->rcv_msg[0] != 1)
	{
		set_last_error(_T("_ssl_parse_server_change_cipher_spec"), _T("invalid change cipher spec message context"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	//clear recv message control bits
	for (i = SSL_CTR_SIZE - 1; i >= 0; i--)
	{
		pssl->rcv_ctr[i] = 0;
	}

	//after recv change cipher all record must be crypted recving
	pssl->rcv_crypted = 1;

	return SSL_SERVER_FINISHED;
}

static handshake_states _ssl_parse_server_finished(ssl_context *pssl)
{
	/*
	struct {
	opaque verify_data[12];
	} Finished;
	verify_data = PRF(master_secret, finished_label, MD5(handshake_messages) + SHA-1(handshake_messages)) [0..11];
	*/

	int hash_len;
	md5_context  md5;
	sha1_context sha1;
	sha2_context sha2;
	byte_t padbuf[48] = { 0 };
	byte_t md5sum[16] = { 0 };
	byte_t sha1sum[20] = { 0 };
	byte_t mac_buf[36] = { 0 };

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));
	xmem_copy(&sha2, &pssl->sha2, sizeof(sha2_context));

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(padbuf, 0x36, 48);

		md5_update(&md5, "SRVR", 4);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_finish(&md5, md5sum);

		sha1_update(&sha1, "SRVR", 4);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_finish(&sha1, sha1sum);

		xmem_set(padbuf, 0x5C, 48);

		md5_starts(&md5);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_update(&md5, md5sum, 16);
		md5_finish(&md5, mac_buf);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_update(&sha1, sha1sum, 20);
		sha1_finish(&sha1, mac_buf + 16);

		hash_len = 36;
	}
	else
	{
		if (pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			sha2_finish(&sha2, padbuf);

			_ssl_prf2(pssl->master_secret, SSL_MST_SIZE, label_server_finished, padbuf, 32, mac_buf, 12);
		}
		else
		{
			md5_finish(&md5, padbuf);
			sha1_finish(&sha1, padbuf + 16);

			_ssl_prf1(pssl->master_secret, SSL_MST_SIZE, label_server_finished, padbuf, 36, mac_buf, 12);
		}

		hash_len = 12;
	}

	if (C_OK != _ssl_read_rcv_msg(pssl))
		return SSL_HANDSHAKE_ERROR;

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_FINISHED)
	{
		set_last_error(_T("_ssl_parse_server_finished"), _T("invalid finished message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (xmem_comp(pssl->rcv_msg + SSL_HSH_SIZE, hash_len, mac_buf, hash_len) != 0)
	{
		set_last_error(_T("_ssl_parse_server_finished"), _T("invalid finished message hash"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->resumed) ? SSL_CLIENT_CHANGE_CIPHER_SPEC : SSL_HANDSHAKE_OVER;
}

static bool_t _ssl_handshake_client(ssl_context *pssl)
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

	pssl->over = (state == SSL_HANDSHAKE_OVER) ? 1 : -1;

	return (pssl->over == 1)? 1 : 0;
}

/***************************************server routing************************************************************/

static handshake_states _ssl_parse_client_hello(ssl_context *pssl)
{
	/*
	struct {
		ProtocolVersion client_version;
		struct {
		uint32 gmt_unix_time;
		opaque random_bytes[28];
		} Random;
		SessionID session_id;
		CipherSuite cipher_suites<2..2 ^ 16 - 1>;
		CompressionMethod compression_methods<1..2 ^ 8 - 1>;
	}Client Hello
	*/

	int msglen, haslen, seslen, ciphlen, complen, comped, extlen, lstlen, grplen;
	int i, j, n;
	int ciph;
	int type;
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

	if (pssl->rcv_msg[0] != SSL_HS_CLIENT_HELLO)
	{
		set_last_error(_T("_ssl_parse_client_hello"), _T("invalid client hello message"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);
	if (pssl->rcv_msg_len != SSL_HSH_SIZE + haslen)
	{
		set_last_error(_T("_ssl_parse_client_hello"), _T("invalid client hello message length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	msglen = SSL_HSH_SIZE;

	pssl->cli_major_ver = pssl->rcv_msg[msglen];
	pssl->cli_minor_ver = pssl->rcv_msg[msglen + 1];
	msglen += 2;

	if (pssl->srv_major_ver != pssl->cli_major_ver)
	{
		set_last_error(_T("_ssl_parse_client_hello"), _T("handshake major version mistech"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->cli_minor_ver < SSL_MINOR_VERSION_1)
	{
		set_last_error(_T("_ssl_parse_client_hello"), _T("handshake minor version unsupported"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	pssl->major_ver = pssl->cli_major_ver;
	pssl->minor_ver = (pssl->srv_minor_ver < pssl->cli_minor_ver) ? pssl->srv_minor_ver : pssl->cli_minor_ver;

	xmem_copy(pssl->rnd_cli, pssl->rcv_msg + msglen, SSL_RND_SIZE);
	msglen += SSL_RND_SIZE;

	seslen = GET_BYTE(pssl->rcv_msg, msglen);
	msglen++;

	if (seslen < 0 || seslen > 32)
	{
		set_last_error(_T("_ssl_parse_client_hello"), _T("invalid client hello session id length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	xmem_copy(pssl->ses_id, pssl->rcv_msg + msglen, seslen);
	msglen += seslen;

	ciphlen = GET_SWORD_NET(pssl->rcv_msg, msglen);
	msglen += 2;

	if (ciphlen < 2 || ciphlen > 256 || (ciphlen % 2) != 0)
	{
		set_last_error(_T("_ssl_parse_client_hello"), _T("invalid client hello session cipher list length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	ciph_buf = pssl->rcv_msg + msglen;
	ciph = 0;
	n = sizeof(server_ciphers) / sizeof(ciphers_set);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < ciphlen; j += 2)
		{
			ciph = GET_SWORD_NET(ciph_buf, j);
			if (ciph == server_ciphers[i].cipher)
				break;
		}

		if (j < ciphlen)
			break;
	}
	msglen += ciphlen;

	if (!_ssl_choose_cipher(pssl, ciph))
	{
		set_last_error(_T("_ssl_parse_client_hello"), _T("unknown cipher type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	complen = GET_BYTE(pssl->rcv_msg, msglen);
	msglen++;

	comped = GET_BYTE(pssl->rcv_msg, msglen);
	msglen++;

	if (complen < 1 || complen > 16)
	{
		set_last_error(_T("_ssl_parse_client_hello"), _T("invalid client hello session compress length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	//has no extension
	if (msglen == haslen + SSL_HSH_SIZE)
		return SSL_SERVER_HELLO;

	//extension length
	extlen = GET_SWORD_NET(pssl->rcv_msg, msglen);
	msglen += 2;

	while (extlen)
	{
		//extension type
		type = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;
		extlen -= 2;

		//extension list length
		lstlen = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;
		extlen -= 2;

		switch (type)
		{
		case SSL_EXTENSION_SERVERNAME:
			//skip server name
			msglen += lstlen;
			extlen -= lstlen;
			break;
		case SSL_EXTENSION_SUPPORTEDGROUPS:
			//Elliptic curve list length
			grplen = GET_SWORD_NET(pssl->rcv_msg, msglen);
			msglen += 2;
			extlen -= 2;
			lstlen -= 2;

			while (grplen)
			{
				if (!pssl->ecp_group)
					pssl->ecp_group = GET_SWORD_NET(pssl->rcv_msg, msglen);
				msglen += 2;
				extlen -= 2;
				lstlen -= 2;
				grplen -= 2;
			}
			msglen += lstlen;
			extlen -= lstlen;
			break;
		case SSL_EXTENSION_ECPOINTFORMATS:
			//Elliptic curve list length
			grplen = GET_BYTE(pssl->rcv_msg, msglen);
			msglen ++;
			extlen --;
			lstlen--;

			if (!pssl->ecp_format)
				pssl->ecp_format = GET_BYTE(pssl->rcv_msg, msglen);
			msglen += lstlen;
			extlen -= lstlen;
			break;
		case SSL_EXTENSION_SIGNATUREANDHASHALGORITHM:
			//Algorithm list length
			grplen = GET_SWORD_NET(pssl->rcv_msg, msglen);
			msglen += 2;
			extlen -= 2;
			lstlen -= 2;

			while (grplen)
			{
				if (!pssl->alg_hash)
					pssl->alg_hash = GET_BYTE(pssl->rcv_msg, msglen);
				msglen++;
				extlen--;
				lstlen--;
				grplen--;

				if (!pssl->alg_sign)
					pssl->alg_sign = GET_BYTE(pssl->rcv_msg, msglen);
				msglen++;
				extlen--;
				lstlen--;
				grplen--;
			}
			break;
		default:
			//skip 
			msglen += lstlen;
			extlen -= lstlen;
			break;
		}

		if (extlen < 0)
		{
			set_last_error(_T("_ssl_parse_client_hello"), _T("invalid client hello extension"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}

	return SSL_SERVER_HELLO;
}

static handshake_states _ssl_write_server_hello(ssl_context *pssl)
{
	/*
	struct {
		ProtocolVersion server_version;
		struct {
		uint32 gmt_unix_time;
		opaque random_bytes[28];
		} Random;
		SessionID session_id;
		CipherSuite cipher_suite;
		CompressionMethod compression_method;

		//tls 1.2
		struct {
		ExtensionType extension_type;
		opaque extension_data<0..2^16-1>;
		} Extension;
	} ServerHello;
	*/

	int i, extlen, msglen = SSL_HSH_SIZE;
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

	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)(pssl->major_ver));
	PUT_BYTE(pssl->snd_msg, msglen++, (byte_t)(pssl->minor_ver));

	xmem_copy(pssl->snd_msg + msglen, pssl->rnd_srv, SSL_RND_SIZE);
	msglen += SSL_RND_SIZE;

	//38:	session id length
	//39~38+n:	session id
	//39+n~40+n:	chosen cipher
	//41+n~41+n:	chosen compression alg.
	
	PUT_BYTE(pssl->snd_msg, msglen, (byte_t)(pssl->ses_size));
	msglen++;
	xmem_copy(pssl->snd_msg + msglen, pssl->ses_id, pssl->ses_size);
	msglen += pssl->ses_size;

	PUT_SWORD_NET(pssl->snd_msg, msglen, pssl->cipher);
	msglen += 2;

	//SSL_COMPRESS_NULL
	PUT_BYTE(pssl->snd_msg, msglen, 0);
	msglen++;

	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		// preset Extensions length to zero
		PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(0));
		msglen += 2;
		extlen = 0;

		/*if (pssl->ecp_group)
		{
			//struct {
			//NamedCurve elliptic_curve_list<1..2 ^ 16 - 1>
			//} EllipticCurveList;
			
			// Extension type: supported_groups (10)
			PUT_SWORD_NET(pssl->snd_msg, msglen, SSL_EXTENSION_SUPPORTEDGROUPS);
			msglen += 2;
			extlen += 2;

			// supported_groups length
			PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(4));
			msglen += 2;
			extlen += 2;

			// list length
			PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(2));
			msglen += 2;
			extlen += 2;

			// Supported Group: secp256r1 (0x0017)
			PUT_SWORD_NET(pssl->snd_msg, msglen, (unsigned short)(pssl->ecp_group));
			msglen += 2;
			extlen += 2;

		}*/

		if (extlen)
		{
			//reset Extensions length
			PUT_SWORD_NET(pssl->snd_msg, (msglen - extlen - 2), (unsigned short)(extlen));
		}
		else
		{
			//remove Extensions length
			msglen -= 2;
		}
	}

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	PUT_BYTE(pssl->snd_msg, 0, (byte_t)SSL_HS_SERVER_HELLO);
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	return (C_OK == _ssl_write_snd_msg(pssl)) ? SSL_SERVER_CERTIFICATE : SSL_HANDSHAKE_ERROR;
}

static handshake_states _ssl_write_server_certificate(ssl_context *pssl)
{
	/*
	opaque ASN.1Cert<1..2 ^ 24 - 1>;

	struct {
		ASN.1Cert certificate_list<0..2 ^ 24 - 1>;
	} Certificate;
	*/

	int n, lenpos, crtlen, msglen = SSL_HSH_SIZE;
	x509_cert *crt;

	if (pssl->crt_ow == NULL)
	{
		set_last_error(_T("_ssl_write_server_certificate"), _T("empty server certificate"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	//0: handshake type
	//1-3: handshake length
	//4-6: length of all certs
	//7-9: length of cert
	//10~n-1: peer certificate
	//n~n+2: length of cert
	//n+3: upper level cert, etc.

	//preset certs length to zero
	PUT_THREEBYTE_LEN(pssl->snd_msg, msglen, 0);
	lenpos = msglen;
	msglen += 3;

	crtlen = 0;
	crt = pssl->crt_ow;
	while (crt != NULL && crt->next != NULL)
	{
		n = crt->raw.len;
		if (msglen + 3 + n > SSL_PKG_SIZE)
		{
			set_last_error(_T("_ssl_write_server_certificate"), _T("message package overwrited"), -1);
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

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (IS_DHE_CIPHER(pssl->cipher) || IS_ECDHE_CIPHER(pssl->cipher))
		return SSL_SERVER_KEY_EXCHANGE;
	else
		return (pssl->verify_server == SSL_VERIFY_NONE) ? SSL_SERVER_HELLO_DONE : SSL_CERTIFICATE_REQUEST;
}

static handshake_states _ssl_write_server_key_exchange(ssl_context *pssl)
{
	/* TLS 1.0/1.1
	struct {
		select (KeyExchangeAlgorithm) {
		case diffie_hellman:
			ServerDHParams params;
			Signature signed_params;
		case rsa:
			ServerRSAParams params;
			Signature signed_params;
		};
	} ServerKeyExchange;
	*/
	/* TLS 1.2
	struct {
		select (KeyExchangeAlgorithm) {
		case dh_anon:
			ServerDHParams params;
		case dhe_dss:
		case dhe_rsa:
			ServerDHParams params;
			digitally-signed struct {
				opaque client_random[32];
				opaque server_random[32];
				ServerDHParams params;
			} signed_params;
		case rsa:
		case dh_dss:
		case dh_rsa:
		struct {} ;
		};
	  } ServerKeyExchange;
	*/

	int n, msglen = SSL_HSH_SIZE;
	size_t m;
	byte_t hash[36] = { 0 };
	md5_context md5;
	sha1_context sha1;
	sha2_context sha2;
	ecp_curve_info* pec;

	//0:	handshake type
	//1~3:	handshake length
	//4~n:	dhm parameters
	//4+n~5+n:	rsa key length
	//6+n:` rsa key sign

	if (IS_DHE_CIPHER(pssl->cipher))
	{
		if (!pssl->dhm_ow)
		{
			pssl->dhm_ow = (dhm_context*)xmem_alloc(sizeof(dhm_context));
			mpi_read_string(&(pssl->dhm_ow->P), 16, dhm_P, -1);
			mpi_read_string(&(pssl->dhm_ow->G), 16, dhm_G, -1);
		}

		// Ephemeral DH parameters:
		n = 0;
		m = mpi_size(&(pssl->dhm_ow->P)); //256
		if (C_OK != dhm_make_params(pssl->dhm_ow, (int)m, pssl->snd_msg + msglen, &n, havege_rand, &pssl->rng))
		{
			set_last_error(_T("_ssl_write_server_key_exchange"), _T("make dhm params faild"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		msglen += n;
	}
	else if(IS_ECDHE_CIPHER(pssl->cipher))
	{
		if (!pssl->ecdh_ow)
		{
			pssl->ecdh_ow = (ecdh_context*)xmem_alloc(sizeof(ecdh_context));
			pec = ecp_curve_info_from_tls_id(TLS_ECP_GROUP_SECP256R1);
			if (!pec)
			{
				set_last_error(_T("_ssl_write_server_key_exchange"), _T("unknow ecdh group id"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
			ecdh_setup(pssl->ecdh_ow, pec->grp_id);
		}

		//
		m = 0;
		if (C_OK != ecdh_make_params(pssl->ecdh_ow, &m, pssl->snd_msg + msglen, 4096, havege_rand_bytes, &pssl->rng))
		{
			set_last_error(_T("_ssl_write_server_key_exchange"), _T("make ecdh params faild"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		n = (int)m;
		msglen += n;
	}

	/* TLS 1.0/1.1
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
	/* TLS 1.2
	struct {
		SignatureAndHashAlgorithm algorithm;
		opaque signature<0..2^16-1>;
	} DigitallySigned;
	*/
	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		/*sha2_starts(&sha2, 0);
		sha2_update(&sha2, pssl->rnd_cli, SSL_RND_SIZE);
		sha2_update(&sha2, pssl->rnd_srv, SSL_RND_SIZE);
		sha2_update(&sha2, pssl->snd_msg + SSL_HSH_SIZE, n);
		sha2_finish(&sha2, hash);

		PUT_BYTE(pssl->snd_msg, msglen, ALG_HASH_SHA256);
		msglen++;*/

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->rnd_cli, SSL_RND_SIZE);
		sha1_update(&sha1, pssl->rnd_srv, SSL_RND_SIZE);
		sha1_update(&sha1, pssl->snd_msg + SSL_HSH_SIZE, n);
		sha1_finish(&sha1, hash);

		PUT_BYTE(pssl->snd_msg, msglen, ALG_HASH_SHA1);
		msglen++;

		PUT_BYTE(pssl->snd_msg, msglen, ALG_SIGN_RSA);
		msglen++;

		PUT_SWORD_NET(pssl->snd_msg, msglen, pssl->rsa_ow->len);
		msglen += 2;

		//if (C_OK != rsa_pkcs1_sign(pssl->rsa_ow, RSA_PRIVATE, RSA_RAW, 32, hash, pssl->snd_msg + msglen))
		if (C_OK != rsa_pkcs1_sign(pssl->rsa_ow, RSA_PRIVATE, RSA_SHA1, 20, hash, pssl->snd_msg + msglen))
		{
			set_last_error(_T("_ssl_write_server_key_exchange"), _T("rsa pkcs1 sign failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		msglen += (pssl->rsa_ow->len);
	}
	else
	{
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

		if (C_OK != rsa_pkcs1_sign(pssl->rsa_ow, RSA_PRIVATE, RSA_RAW, 36, hash, pssl->snd_msg + msglen))
		{
			set_last_error(_T("_ssl_write_server_key_exchange"), _T("rsa pkcs1 sign failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		msglen += (pssl->rsa_ow->len);
	}

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

	return (pssl->verify_server == SSL_VERIFY_NONE) ? SSL_SERVER_HELLO_DONE : SSL_CERTIFICATE_REQUEST;
}

static handshake_states _ssl_write_server_certificate_request(ssl_context *pssl)
{
	/*
	enum { rsa_sign(1), dss_sign(2), rsa_fixed_dh(3), dss_fixed_dh(4), (255) } ClientCertificateType;

	opaque DistinguishedName<1..2^16-1>;

	struct {
		ClientCertificateType certificate_types<1..2^8-1>;
		DistinguishedName certificate_authorities<3..2^16-1>;
	} CertificateRequest;
	*/
	/* TLS 1.2
	struct {
		ClientCertificateType certificate_types<1..2^8-1>; enum {rsa_sign(1), dss_sign(2), rsa_fixed_dh(3), dss_fixed_dh(4), rsa_ephemeral_dh_RESERVED(5), dss_ephemeral_dh_RESERVED(6), fortezza_dms_RESERVED(20), (255)}
		SignatureAndHashAlgorithm; supported_signature_algorithms<2^16-1>;
		DistinguishedName certificate_authorities<0..2^16-1>;
	} CertificateRequest;
	*/

	int n, lenpos, crtlen, msglen = SSL_HSH_SIZE;
	x509_cert *crt;

	XDL_ASSERT(pssl->verify_server != SSL_VERIFY_NONE);

	//0:	handshake type
	//1~3:	handshake length
	//4 :	cert type count
	//5~n-1:	cert types
	//n~n+1:	length of all DNs
	//n+2~n+3:	length of DN 1
	//n+4:	Distinguished Name #1
	// ...  length of DN 2, etc.

	//cert type count
	PUT_BYTE(pssl->snd_msg, msglen, 1);
	msglen++;

	//cert type
	PUT_BYTE(pssl->snd_msg, msglen, SSL_CERTIFICATE_TYPE_RSA);
	msglen++;

	//SignatureAndHashAlgorithm
	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		// SignatureAndHashAlgorithm length
		PUT_SWORD_NET(pssl->snd_msg, msglen, 2);
		msglen += 2;

		//HashAlgorithm
		PUT_BYTE(pssl->snd_msg, msglen, ALG_HASH_SHA256);
		msglen++;

		//SignatureAlgorithm
		PUT_BYTE(pssl->snd_msg, msglen, ALG_SIGN_RSA);
		msglen++;
	}
	
	//preset all cert length
	PUT_SWORD_NET(pssl->snd_msg, msglen, 0);
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

static handshake_states _ssl_write_server_hello_done(ssl_context *pssl)
{
	/*
	struct { } ServerHelloDone;
	*/

	pssl->snd_msg_len = SSL_HSH_SIZE;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	pssl->snd_msg[0] = SSL_HS_SERVER_HELLO_DONE;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, 0);

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->verify_server == SSL_VERIFY_NONE) ? SSL_CLIENT_KEY_EXCHANGE : SSL_CLIENT_CERTIFICATE;
}

static handshake_states _ssl_parse_client_certificate(ssl_context *pssl)
{
	/*
	opaque ASN.1Cert<1..2 ^ 24 - 1>;

	struct {
		ASN.1Cert certificate_list<0..2 ^ 24 - 1>;
	} Certificate;
	*/

	int msglen, haslen, crtlen;
	int n, ret;

	XDL_ASSERT(pssl->verify_server != SSL_VERIFY_NONE);
	
	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		if (pssl->rcv_msg_len == 2 && pssl->rcv_msg_type == SSL_MSG_ALERT && pssl->rcv_msg[0] == SSL_ALERT_WARNING  && pssl->rcv_msg[1] == SSL_ALERT_NO_CERTIFICATE)
		{
			if (pssl->verify_server == SSL_VERIFY_OPTIONAL)
			{
				return SSL_CLIENT_KEY_EXCHANGE;
			}
			else
			{
				set_last_error(_T("_ssl_parse_client_certificate"), _T("client has no certificate"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
	}
	else
	{
		if (GET_THREEBYTE_LEN(pssl->rcv_msg, 1) == 3 && pssl->rcv_msg_type == SSL_MSG_HANDSHAKE  && pssl->rcv_msg[0] == SSL_HS_CERTIFICATE  && xmem_comp(pssl->rcv_msg + SSL_HSH_SIZE, 3, "\0\0\0", 3) == 0)
		{
			if (pssl->verify_server == SSL_VERIFY_REQUIRED)
			{
				set_last_error(_T("_ssl_parse_client_certificate"), _T("client has no certificate"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
			else
			{
				pssl->verify_server = SSL_VERIFY_NONE;
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
		set_last_error(_T("_ssl_parse_client_certificate"), _T("invalid certificate message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	msglen = SSL_HSH_SIZE;
	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);

	crtlen = GET_THREEBYTE_LEN(pssl->rcv_msg, msglen);
	msglen += 3;

	if (haslen != 3 + crtlen)
	{
		set_last_error(_T("_ssl_parse_client_certificate"), _T("invalid certificate block size"), -1);
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
			set_last_error(_T("_ssl_parse_client_certificate"), _T("invalid certificate size"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != x509parse_crt(pssl->crt_pe, pssl->rcv_msg + msglen, n))
		{
			set_last_error(_T("_ssl_parse_client_certificate"), _T("invalid certificate context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		msglen += n;
		crtlen -= n;
	}

	//if multiply handshake message
	if (pssl->rcv_msg_len > msglen)
	{
		xmem_move((pssl->rcv_msg + msglen), (pssl->rcv_msg_len - msglen), -msglen);
		pssl->rcv_msg_len -= msglen;
	}

	if (pssl->verify_server != SSL_VERIFY_NONE)
	{
		if (pssl->crt_ca == NULL)
		{
			set_last_error(_T("_ssl_parse_client_certificate"), _T("CA chian empty"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != x509parse_verify(pssl->crt_pe, pssl->crt_ca, pssl->peer_cn, &ret))
		{
			if (pssl->verify_server == SSL_VERIFY_REQUIRED)
			{
				set_last_error(_T("_ssl_parse_client_certificate"), _T("certificate verify failed"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
	}

	return SSL_CLIENT_KEY_EXCHANGE;
}

static handshake_states _ssl_parse_client_key_exchange(ssl_context *pssl)
{
	/* TLS 1.0/1.1
	struct {
		select (KeyExchangeAlgorithm) {
		case rsa: 
			EncryptedPreMasterSecret;
		case diffie_hellman: 
			ClientDiffieHellmanPublic;
		} exchange_keys;
	} ClientKeyExchange;
	*/
	/* TLS 1.2
	struct {
		select (KeyExchangeAlgorithm) {
		case rsa:
			EncryptedPreMasterSecret;
		case dhe_dss:
		case dhe_rsa:
		case dh_dss:
		case dh_rsa:
		case dh_anon:
			ClientDiffieHellmanPublic;
		} exchange_keys;
	} ClientKeyExchange;
	*/
	int haslen, i, n, msglen = SSL_HSH_SIZE;
	byte_t premaster[SSL_BLK_SIZE] = { 0 };
	int prelen = SSL_MST_SIZE;
	size_t m;

	if (pssl->rcv_msg[0] != SSL_HS_CLIENT_KEY_EXCHANGE)
	{
		if (C_OK != _ssl_read_rcv_msg(pssl))
		{
			return SSL_HANDSHAKE_ERROR;
		}
	}

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_CLIENT_KEY_EXCHANGE)
	{
		set_last_error(_T("_ssl_parse_client_key_exchange"), _T("invalid client key exchange message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);

	if (IS_DHE_CIPHER(pssl->cipher))
	{
		//Receive G^Y mod P, premaster = (G^Y)^X mod P
		n = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;

		//if (n < 1 || n > pssl->dhm_ow->len || n + 2 != haslen) //key size maybe changed
		if (n < 1 || n + 2 != haslen)
		{
			set_last_error(_T("_ssl_parse_client_key_exchange"), _T("invalid client key exchange length"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (C_OK != dhm_read_public(pssl->dhm_ow, pssl->rcv_msg + msglen, n))
		{
			set_last_error(_T("_ssl_parse_client_key_exchange"), _T("invalid client key exchange context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		prelen = pssl->dhm_ow->len;

		if (C_OK != dhm_calc_secret(pssl->dhm_ow, premaster, &prelen))
		{
			set_last_error(_T("_ssl_parse_client_key_exchange"), _T("create premaster failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		msglen += n;
	}
	else if (IS_ECDHE_CIPHER(pssl->cipher))
	{
		/*n = GET_SWORD_NET(pssl->rcv_msg, msglen);
		msglen += 2;

		//if (n < 1 || n > pssl->dhm_ow->len || n + 2 != haslen) //key size maybe changed
		if (n < 1 || n + 2 != haslen)
		{
			set_last_error(_T("_ssl_parse_client_key_exchange"), _T("invalid client key exchange length"), -1);
			return SSL_HANDSHAKE_ERROR;
		}*/

		if (C_OK != ecdh_read_public(pssl->ecdh_ow, pssl->rcv_msg + msglen, haslen))
		{
			set_last_error(_T("_ssl_parse_client_key_exchange"), _T("invalid client key exchange context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		m = 0;
		if (C_OK != ecdh_calc_secret(pssl->ecdh_ow, &m, premaster, 256, havege_rand_bytes, &pssl->rng))
		{
			set_last_error(_T("_ssl_parse_client_key_exchange"), _T("create premaster failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
		prelen = (int)m;

		msglen += haslen;
	}
	else
	{
		// Decrypt the premaster using own private RSA key
		prelen = SSL_MST_SIZE;

		if (pssl->minor_ver == SSL_MINOR_VERSION_0)
		{
			n = pssl->rsa_ow->len;

			if (haslen != n)
			{
				set_last_error(_T("_ssl_parse_client_key_exchange"), _T("invalid client key exchange key length"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}
		else
		{
			n = GET_SWORD_NET(pssl->rcv_msg, msglen);
			msglen += 2;

			if (n != pssl->rsa_ow->len)
			{
				set_last_error(_T("_ssl_parse_client_key_exchange"), _T("invalid client key exchange key length"), -1);
				return SSL_HANDSHAKE_ERROR;
			}
		}

		if (C_OK != rsa_pkcs1_decrypt(pssl->rsa_ow, RSA_PRIVATE, &prelen, pssl->rcv_msg + msglen, premaster))
		{
			set_last_error(_T("_ssl_parse_client_key_exchange"), _T("decrypt client key exchange key failed"), -1);
			return SSL_HANDSHAKE_ERROR;
		}

		if (prelen != SSL_MST_SIZE)// || premaster[0] != pssl->major_ver || premaster[1] != pssl->minor_ver)
		{
			/*
			* Protection against Bleichenbacher's attack:
			* invalid PKCS#1 v1.5 padding must not cause
			* the connection to end immediately; instead,
			* send a bad_record_mac later in the handshake.
			*/
			for (i = 0; i < prelen; i++)
				premaster[i] = (byte_t)havege_rand(&pssl->rng);
		}

		msglen += n;
	}

	//if multiply handshake message
	if (pssl->rcv_msg_len > msglen)
	{
		xmem_move((pssl->rcv_msg + msglen), (pssl->rcv_msg_len - msglen), -msglen);
		pssl->rcv_msg_len -= msglen;
	}

	_ssl_derive_keys(pssl, premaster, prelen);

	return (pssl->crt_pe) ? SSL_CERTIFICATE_VERIFY : SSL_CLIENT_CHANGE_CIPHER_SPEC;
}

static handshake_states _ssl_parse_client_certificate_verify(ssl_context *pssl)
{
	/*
	struct {
		Signature signature;
	} CertificateVerify;
	*/

	int msglen, haslen, n;
	md5_context md5;
	sha1_context sha1;
	sha2_context sha2;
	byte_t pad_1[48];
	byte_t pad_2[48];
	byte_t hash[36];
	int alg_hash, alg_sign;

	XDL_ASSERT(pssl->crt_pe != NULL);

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));
	xmem_copy(&sha2, &pssl->sha2, sizeof(sha2_context));

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		memset(pad_1, 0x36, 48);
		memset(pad_2, 0x5C, 48);

		md5_update(&md5, pssl->master_secret, 48);
		md5_update(&md5, pad_1, 48);
		md5_finish(&md5, hash);

		md5_starts(&md5);
		md5_update(&md5, pssl->master_secret, 48);
		md5_update(&md5, pad_2, 48);
		md5_update(&md5, hash, 16);
		md5_finish(&md5, hash);

		sha1_update(&sha1, pssl->master_secret, 48);
		sha1_update(&sha1, pad_1, 40);
		sha1_finish(&sha1, hash + 16);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->master_secret, 48);
		sha1_update(&sha1, pad_2, 40);
		sha1_update(&sha1, hash + 16, 20);
		sha1_finish(&sha1, hash + 16);
	}
	else 
	{
		if (pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			sha2_finish(&sha2, hash);
		}
		else
		{
			md5_finish(&md5, hash);
			sha1_finish(&sha1, hash + 16);
		}
	}

	if (pssl->rcv_msg[0] != SSL_HS_CERTIFICATE_VERIFY)
	{
		if (C_OK != _ssl_read_rcv_msg(pssl))
		{
			return SSL_HANDSHAKE_ERROR;
		}
	}

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_CERTIFICATE_VERIFY)
	{
		set_last_error(_T("_ssl_parse_client_certificate_verify"), _T("invalid certificate verify message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	haslen = GET_THREEBYTE_LEN(pssl->rcv_msg, 1);
	msglen = SSL_HSH_SIZE;

	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		alg_hash = GET_BYTE(pssl->rcv_msg, msglen);
		msglen++;

		alg_sign = GET_BYTE(pssl->rcv_msg, msglen);
		msglen++;
	}

	n = GET_SWORD_NET(pssl->rcv_msg, msglen);
	msglen += 2;

	if (n != pssl->crt_pe->rsa.len)
	{
		set_last_error(_T("_ssl_parse_client_certificate_verify"), _T("invalid certificate verify message length"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->minor_ver == SSL_MINOR_VERSION_3)
	{
		if (C_OK != rsa_pkcs1_verify(&pssl->crt_pe->rsa, RSA_PUBLIC, RSA_RAW, 32, hash, pssl->rcv_msg + msglen))
		{
			set_last_error(_T("_ssl_parse_client_certificate_verify"), _T("invalid certificate verify message context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}
	else
	{
		if (C_OK != rsa_pkcs1_verify(&pssl->crt_pe->rsa, RSA_PUBLIC, RSA_RAW, 36, hash, pssl->rcv_msg + msglen))
		{
			set_last_error(_T("_ssl_parse_client_certificate_verify"), _T("invalid certificate verify message context"), -1);
			return SSL_HANDSHAKE_ERROR;
		}
	}

	msglen += n;

	//if multiply handshake message
	if (pssl->rcv_msg_len > msglen)
	{
		xmem_move((pssl->rcv_msg + msglen), (pssl->rcv_msg_len - msglen), -msglen);
		pssl->rcv_msg_len -= msglen;
	}
	
	return SSL_CLIENT_CHANGE_CIPHER_SPEC;
}

static handshake_states _ssl_parse_client_change_cipher_spec(ssl_context *pssl)
{
	int i;
	/*
	struct {
		enum { change_cipher_spec(1), (255) } type;
	} ChangeCipherSpec;
	*/

	if (C_OK != _ssl_read_rcv_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_type != SSL_MSG_CHANGE_CIPHER_SPEC)
	{
		set_last_error(_T("_ssl_parse_client_change_cipher_spec"), _T("invalid change cipher spec message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (pssl->rcv_msg_len != 1 || pssl->rcv_msg[0] != 1)
	{
		set_last_error(_T("_ssl_parse_client_change_cipher_spec"), _T("invalid change cipher spec message context"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	//clear recv message control bits
	for (i = SSL_CTR_SIZE - 1; i >= 0; i--)
	{
		pssl->rcv_ctr[i] = 0;
	}

	//after read change cipher all record must be crypted recving
	pssl->rcv_crypted = 1;

	return SSL_CLIENT_FINISHED;
}

static handshake_states _ssl_parse_client_finished(ssl_context *pssl)
{
	/*
	struct {
		opaque verify_data[12];
	} Finished;
	verify_data = PRF(master_secret, finished_label, MD5(handshake_messages) + SHA-1(handshake_messages)) [0..11];
	*/

	int hash_len;
	md5_context  md5;
	sha1_context sha1;
	sha2_context sha2;
	byte_t padbuf[48] = { 0 };
	byte_t md5sum[16] = { 0 };
	byte_t sha1sum[20] = { 0 };
	byte_t mac_buf[36] = { 0 };

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));
	xmem_copy(&sha2, &pssl->sha2, sizeof(sha2_context));

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		xmem_set(padbuf, 0x36, 48);

		md5_update(&md5, "CLNT", 4);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_finish(&md5, md5sum);

		sha1_update(&sha1, "CLNT", 4);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_finish(&sha1, sha1sum);

		xmem_set(padbuf, 0x5C, 48);

		md5_starts(&md5);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_update(&md5, md5sum, 16);
		md5_finish(&md5, mac_buf);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_update(&sha1, sha1sum, 20);
		sha1_finish(&sha1, mac_buf + 16);

		hash_len = 36;
	}
	else
	{
		if (pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			sha2_finish(&sha2, padbuf);
			_ssl_prf2(pssl->master_secret, SSL_MST_SIZE, label_client_finished, padbuf, 32, mac_buf, 12);
		}
		else
		{
			md5_finish(&md5, padbuf);
			sha1_finish(&sha1, padbuf + 16);
			_ssl_prf1(pssl->master_secret, SSL_MST_SIZE, label_client_finished, padbuf, 36, mac_buf, 12);
		}

		hash_len = 12;
	}

	if (C_OK != _ssl_read_rcv_msg(pssl))
		return SSL_HANDSHAKE_ERROR;

	if (pssl->rcv_msg_type != SSL_MSG_HANDSHAKE || pssl->rcv_msg[0] != SSL_HS_FINISHED)
	{
		set_last_error(_T("_ssl_parse_client_finished"), _T("invalid finished message type"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	if (xmem_comp(pssl->rcv_msg + SSL_HSH_SIZE, hash_len, mac_buf, hash_len) != 0)
	{
		set_last_error(_T("_ssl_parse_client_finished"), _T("invalid finished message hash"), -1);
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->resumed) ? SSL_HANDSHAKE_OVER : SSL_SERVER_CHANGE_CIPHER_SPEC;
}

static handshake_states _ssl_write_server_change_cipher_spec(ssl_context *pssl)
{
	int i;
	/*
	struct {
		enum { change_cipher_spec(1), (255) } type;
	} ChangeCipherSpec;
	*/

	pssl->snd_msg_type = SSL_MSG_CHANGE_CIPHER_SPEC;
	pssl->snd_msg_len = 1;
	pssl->snd_msg[0] = 1;

	//clear send message control bits
	for (i = SSL_CTR_SIZE - 1; i >= 0; i--)
	{
		pssl->snd_ctr[i] = 0;
	}

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	//after write change cipher all record must be crypted sending
	pssl->snd_crypted = 1;

	return SSL_SERVER_FINISHED;
}

static handshake_states _ssl_write_server_finished(ssl_context *pssl)
{
	/*
	struct {
		opaque verify_data[12];
	} Finished;
	verify_data = PRF(master_secret, finished_label, MD5(handshake_messages) + SHA-1(handshake_messages)) [0..11];
	*/

	int msglen = SSL_HSH_SIZE;
	md5_context  md5;
	sha1_context sha1;
	sha2_context sha2;
	byte_t padbuf[48] = { 0 };
	byte_t md5sum[16] = { 0 };
	byte_t sha1sum[20] = { 0 };
	byte_t* mac_buf;

	xmem_copy(&md5, &pssl->md5, sizeof(md5_context));
	xmem_copy(&sha1, &pssl->sha1, sizeof(sha1_context));
	xmem_copy(&sha2, &pssl->sha2, sizeof(sha2_context));

	mac_buf = pssl->snd_msg + msglen;

	if (pssl->minor_ver == SSL_MINOR_VERSION_0)
	{
		//struct {
		//	opaque md5_hash[16];
		//	opaque sha_hash[20];
		//} Finished;
		//md5_hash:  MD5(master_secret + pad2 + MD5(handshake_messages + Sender + master_secret + pad1));
		//sha_hash:  SHA(master_secret + pad2 + SHA(handshake_messages + Sender + master_secret + pad1));
		//pad_1:  The character 0x36 repeated 48 times for MD5 or 40 times for SHA.
		//pad_2 : The character 0x5c repeated 48 times for MD5 or 40 times for SHA.
		//enum { client(0x434C4E54), server(0x53525652) } Sender;

		xmem_set(padbuf, 0x36, 48);

		md5_update(&md5, "SRVR", 4);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_finish(&md5, md5sum);

		sha1_update(&sha1, "SRVR", 4);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_finish(&sha1, sha1sum);

		xmem_set(padbuf, 0x5C, 48);

		md5_starts(&md5);
		md5_update(&md5, pssl->master_secret, SSL_MST_SIZE);
		md5_update(&md5, padbuf, 48);
		md5_update(&md5, md5sum, 16);
		md5_finish(&md5, mac_buf);

		sha1_starts(&sha1);
		sha1_update(&sha1, pssl->master_secret, SSL_MST_SIZE);
		sha1_update(&sha1, padbuf, 40);
		sha1_update(&sha1, sha1sum, 20);
		sha1_finish(&sha1, mac_buf + 16);

		msglen += 36;
	}
	else
	{
		if (pssl->minor_ver == SSL_MINOR_VERSION_3)
		{
			sha2_finish(&sha2, padbuf);
			_ssl_prf2(pssl->master_secret, SSL_MST_SIZE, label_server_finished, padbuf, 32, mac_buf, 12);
		}
		else
		{
			md5_finish(&md5, padbuf);
			sha1_finish(&sha1, padbuf + 16);
			_ssl_prf1(pssl->master_secret, SSL_MST_SIZE, label_server_finished, padbuf, 36, mac_buf, 12);
		}

		msglen += 12;
	}

	pssl->snd_msg_len = msglen;
	pssl->snd_msg_type = SSL_MSG_HANDSHAKE;
	//handshake code
	pssl->snd_msg[0] = SSL_HS_FINISHED;
	//handshake length
	PUT_THREEBYTE_LEN(pssl->snd_msg, 1, msglen - SSL_HSH_SIZE);

	if (C_OK != _ssl_write_snd_msg(pssl))
	{
		return SSL_HANDSHAKE_ERROR;
	}

	return (pssl->resumed) ? SSL_CLIENT_CHANGE_CIPHER_SPEC : SSL_HANDSHAKE_OVER;
}

static bool_t _ssl_handshake_server(ssl_context *pssl)
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
		
	pssl->over = (state == SSL_HANDSHAKE_OVER) ? 1 : -1;

	return (pssl->over == 1)? 1 : 0;
}

/*********************************************************************************************************/

xhand_t xssl_cli(unsigned short port, const tchar_t* addr)
{
	ssl_context* pssl;
	xhand_t tcp;
	
	tcp = xtcp_cli(port, addr);
	if (!tcp)
		return NULL;

	pssl = (ssl_context*)xmem_alloc(sizeof(ssl_context));
	pssl->head.tag = _HANDLE_SSL;

	pssl->type = SSL_TYPE_CLIENT;

	_ssl_init(pssl);

	pssl->pif = (bio_interface*)xmem_alloc(sizeof(bio_interface));

	get_bio_interface(tcp, pssl->pif);

	return &pssl->head;
}

xhand_t xssl_srv(res_file_t so)
{
	ssl_context* pssl;
	xhand_t tcp;

	tcp = xtcp_srv(so);
	if (!tcp)
		return NULL;

	pssl = (ssl_context*)xmem_alloc(sizeof(ssl_context));
	pssl->head.tag = _HANDLE_SSL;

	pssl->type = SSL_TYPE_SERVER;

	_ssl_init(pssl);

	pssl->pif = (bio_interface*)xmem_alloc(sizeof(bio_interface));

	get_bio_interface(tcp, pssl->pif);

	return &pssl->head;
}

void  xssl_close(xhand_t ssl)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pssl->type == SSL_TYPE_CLIENT || pssl->type == SSL_TYPE_SERVER);

	if (pssl->snd_msg_pop)
	{
		_ssl_flush_data(pssl);
	}

	if(pssl->over == 1)
	{
		_ssl_write_close(pssl);
	}

	if (pssl->pif)
		xtcp_close(pssl->pif->fd);

	_ssl_uninit(pssl);

	if (pssl->pif)
		xmem_free(pssl->pif);

	xmem_free(pssl);
}

res_file_t xssl_socket(xhand_t ssl)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	return (pssl->pif->fd) ? xtcp_socket(pssl->pif->fd) : INVALID_FILE;
}

int xssl_type(xhand_t ssl)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	return pssl->type;
}

bool_t xssl_write(xhand_t ssl, const byte_t* buf, dword_t* pb)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);
	int bys, pos;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	if (!pssl->over)
	{
		if (pssl->type == SSL_TYPE_CLIENT && !_ssl_handshake_client(pssl))
		{
			_ssl_write_close(pssl);
			return 0;
		}

		if (pssl->type == SSL_TYPE_SERVER && !_ssl_handshake_server(pssl))
		{
			_ssl_write_close(pssl);
			return 0;
		}

		//clear handshake
		pssl->rcv_msg_type = 0;
		pssl->rcv_msg_len = 0;
		pssl->rcv_msg_pop = 0;

		pssl->snd_msg_type = 0;
		pssl->snd_msg_len = 0;
		pssl->snd_msg_pop = 0;
	}

	if (pssl->over < 0)
	{
		*pb = 0;
		return 1;
	}

	pos = 0;
	while (pos < (int)(*pb))
	{
		bys = *pb - pos;
		if (!_ssl_write_data(pssl, buf + pos, &bys))
			break;

		if (!bys)
			break;

		pos += bys;
	}

	return (pos == (int)(*pb)) ? 1 : 0;
}

bool_t xssl_flush(xhand_t ssl)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	return _ssl_flush_data(pssl);
}

bool_t xssl_read(xhand_t ssl, byte_t* buf, dword_t* pb)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);
	int bys, pos;
	bool_t rt = 1;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	if (!pssl->over)
	{
		if (pssl->type == SSL_TYPE_CLIENT && !_ssl_handshake_client(pssl))
		{
			_ssl_write_close(pssl);
			return 0;
		}

		if (pssl->type == SSL_TYPE_SERVER && !_ssl_handshake_server(pssl))
		{
			_ssl_write_close(pssl);
			return 0;
		}

		//clear handshake
		pssl->rcv_msg_type = 0;
		pssl->rcv_msg_len = 0;
		pssl->rcv_msg_pop = 0;

		pssl->snd_msg_type = 0;
		pssl->snd_msg_len = 0;
		pssl->snd_msg_pop = 0;
	}

	if (pssl->over < 0)
	{
		*pb = 0;
		return 1;
	}

	pos = 0;
	while (pos < (int)(*pb))
	{
		bys = *pb - pos;
		rt = _ssl_read_data(pssl, buf + pos, &bys);
		if (!rt)
			break;

		if (!bys)
			break;

		pos += bys;
	}

	*pb = pos;

	return rt;
}

unsigned short xssl_addr_port(xhand_t ssl, tchar_t* addr)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	socket_addr(xssl_socket(ssl), &na);
	conv_addr(&na, &port, addr);

	return port;
}

unsigned short xssl_peer_port(xhand_t ssl, tchar_t* addr)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	socket_peer(xssl_socket(ssl), &na);
	conv_addr(&na, &port, addr);

	return port;
}

bool_t xssl_setopt(xhand_t ssl, int oid, void* opt, int len)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	switch (oid)
	{
	case SOCK_OPTION_SNDBUF:
		socket_set_sndbuf(xssl_socket(ssl), *(int*)opt);
		return 1;
	case SOCK_OPTION_RCVBUF:
		socket_set_rcvbuf(xssl_socket(ssl), *(int*)opt);
		return 1;
	case SOCK_OPTION_NONBLK:
		socket_set_nonblk(xssl_socket(ssl), *(bool_t*)opt);
		return 1;
	}

	return 0;
}

void xssl_set_host(xhand_t ssl, const tchar_t* host_cn)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);
	int len;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pssl->type == SSL_TYPE_CLIENT);

#ifdef _UNICODE
	len = ucs_to_mbs(host_cn, -1, NULL, MAX_LONG);
	pssl->host_cn = a_xsalloc(len + 1);
	ucs_to_mbs(host_cn, -1, pssl->host_cn, len);
#else
	len = a_xslen(host_cn);
	pssl->host_cn = a_xsnclone(host_cn, len);
#endif
}

void xssl_set_peer(xhand_t ssl, const tchar_t* peer_cn)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);
	int len;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pssl->type == SSL_TYPE_SERVER);

#ifdef _UNICODE
	len = ucs_to_mbs(peer_cn, -1, NULL, MAX_LONG);
	pssl->peer_cn = a_xsalloc(len + 1);
	ucs_to_mbs(peer_cn, -1, pssl->peer_cn, len);
#else
	len = a_xslen(peer_cn);
	pssl->peer_cn = a_xsnclone(peer_cn, len);
#endif
}

bool_t xssl_set_ca(xhand_t ssl, const byte_t* sz_cert, dword_t clen, const byte_t* sz_rsa, dword_t rlen, const tchar_t* sz_pwd, int len)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);
	byte_t buf_pwd[RES_LEN + 1] = { 0 };
	dword_t dw;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pssl->type == SSL_TYPE_SERVER);

	if (clen)
	{
		if (!pssl->crt_ca)
			pssl->crt_ca = (x509_cert*)xmem_alloc(sizeof(x509_cert));

		if (C_OK != x509parse_crt(pssl->crt_ca, sz_cert, clen))
		{
			x509_free(pssl->crt_ca);
			xmem_free(pssl->crt_ca);
			pssl->crt_ca = NULL;

			return 0;
		}
	}

	if (rlen)
	{
		if (len < 0)
			len = xslen(sz_pwd);

#ifdef _UNICODE
		dw = ucs_to_utf8(sz_pwd, len, buf_pwd, RES_LEN);
#else
		dw = mbs_to_utf8(sz_pwd, len, buf_pwd, RES_LEN);
#endif

		if (!pssl->rsa_ca)
			pssl->rsa_ca = (rsa_context*)xmem_alloc(sizeof(rsa_context));

		if (C_OK != x509parse_key(pssl->rsa_ca, sz_rsa, rlen, buf_pwd, dw))
		{
			rsa_free(pssl->rsa_ca);
			xmem_free(pssl->rsa_ca);
			pssl->rsa_ca = NULL;

			return 0;
		}
	}

	return 1;
}

bool_t xssl_set_cert(xhand_t ssl, const byte_t* sz_cert, dword_t clen, const byte_t* sz_rsa, dword_t rlen, const tchar_t* sz_pwd, int len)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);
	byte_t buf_pwd[RES_LEN + 1] = { 0 };
	dword_t dw;

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pssl->type == SSL_TYPE_CLIENT || pssl->type == SSL_TYPE_SERVER);

	if (clen)
	{
		if (!pssl->crt_ow)
			pssl->crt_ow = (x509_cert*)xmem_alloc(sizeof(x509_cert));

		if (C_OK != x509parse_crt(pssl->crt_ow, sz_cert, clen))
		{
			x509_free(pssl->crt_ow);
			xmem_free(pssl->crt_ow);
			pssl->crt_ow = NULL;

			return 0;
		}
	}

	if (rlen)
	{
		if (len < 0)
			len = xslen(sz_pwd);

#ifdef _UNICODE
		dw = ucs_to_utf8(sz_pwd, len, buf_pwd, RES_LEN);
#else
		dw = (len < RES_LEN) ? len : RES_LEN;
		a_xsncpy((schar_t*)buf_pwd, sz_pwd, dw);
#endif

		if (!pssl->rsa_ow)
			pssl->rsa_ow = (rsa_context*)xmem_alloc(sizeof(rsa_context));

		if (C_OK != x509parse_key(pssl->rsa_ow, sz_rsa, rlen, buf_pwd, dw))
		{
			rsa_free(pssl->rsa_ow);
			xmem_free(pssl->rsa_ow);
			pssl->rsa_ow = NULL;

			return 0;
		}
	}

	return 1;
}

bool_t xssl_set_dhm(xhand_t ssl, const byte_t *dhm_p, const byte_t *dhm_g)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pssl->type == SSL_TYPE_SERVER);

	if (dhm_p)
		mpi_read_string(&(pssl->dhm_ow->P), 16, dhm_p, -1);
	if (dhm_g)
		mpi_read_string(&(pssl->dhm_ow->G), 16, dhm_p, -1);

	return 1;
}

void xssl_set_verify(xhand_t ssl, int srv_verify)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pssl->type == SSL_TYPE_SERVER);

	pssl->verify_server = srv_verify;
}

void xssl_set_version(xhand_t ssl, int cli_ver)
{
	ssl_context* pssl = TypePtrFromHead(ssl_context, ssl);

	XDL_ASSERT(ssl && ssl->tag == _HANDLE_SSL);

	XDL_ASSERT(pssl->type == SSL_TYPE_CLIENT);

	pssl->cli_minor_ver = cli_ver;
}

#endif //XDK_SUPPORT_SOCK
