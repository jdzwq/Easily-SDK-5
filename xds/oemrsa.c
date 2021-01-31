﻿/*
*  The RSA public-key cryptosystem
*
*  Copyright (C) 2006-2007  Christophe Devine
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License aint
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/*
*  RSA was designed by Ron Rivest, Adi Shamir and Len Adleman.
*
*  http://theory.lcs.mit.edu/~rivest/rsapaper.pdf
*  http://www.cacr.math.uwaterloo.ca/hac/about/chap8.pdf
*/

#include "oemrsa.h"
#include <stdlib.h>
#include <string.h>

/*
* Initialize an RSA context
*/
void rsa_init(rsa_context *ctx,
	int padding,
	int hash_id,
	int(*f_rng)(void *),
	void *p_rng)
{
	memset(ctx, 0, sizeof(rsa_context));

	ctx->padding = padding;
	ctx->hash_id = hash_id;

	ctx->f_rng = f_rng;
	ctx->p_rng = p_rng;
}

#if defined(GENPRIME)

/*
* Generate an RSA keypair
*/
int rsa_gen_key(rsa_context *ctx, int nbits, int exponent)
{
	int ret;
	mpi P1, Q1, H, G;

	if (ctx->f_rng == NULL || nbits < 128 || exponent < 3)
		return(ERR_RSA_BAD_INPUT_DATA);

	mpi_init(&P1);
	mpi_init(&Q1);
	mpi_init(&H);
	mpi_init(&G);

	/*
	* find primes P and Q with Q < P so that:
	* GCD( E, (P-1)*(Q-1) ) == 1
	*/
	MPI_CHK(mpi_lset(&ctx->E, exponent));

	do
	{
		MPI_CHK(mpi_gen_prime(&ctx->P, (nbits + 1) >> 1, 0,
			ctx->f_rng, ctx->p_rng));

		MPI_CHK(mpi_gen_prime(&ctx->Q, (nbits + 1) >> 1, 0,
			ctx->f_rng, ctx->p_rng));

		if (mpi_cmp_mpi(&ctx->P, &ctx->Q) < 0)
			mpi_swap(&ctx->P, &ctx->Q);

		if (mpi_cmp_mpi(&ctx->P, &ctx->Q) == 0)
			continue;

		MPI_CHK(mpi_mul_mpi(&ctx->N, &ctx->P, &ctx->Q));
		if (mpi_msb(&ctx->N) != nbits)
			continue;

		MPI_CHK(mpi_sub_int(&P1, &ctx->P, 1));
		MPI_CHK(mpi_sub_int(&Q1, &ctx->Q, 1));
		MPI_CHK(mpi_mul_mpi(&H, &P1, &Q1));
		MPI_CHK(mpi_gcd(&G, &ctx->E, &H));
	} while (mpi_cmp_int(&G, 1) != 0);

	/*
	* D  = E^-1 mod ((P-1)*(Q-1))
	* DP = D mod (P - 1)
	* DQ = D mod (Q - 1)
	* QP = Q^-1 mod P
	*/
	MPI_CHK(mpi_inv_mod(&ctx->D, &ctx->E, &H));
	MPI_CHK(mpi_mod_mpi(&ctx->DP, &ctx->D, &P1));
	MPI_CHK(mpi_mod_mpi(&ctx->DQ, &ctx->D, &Q1));
	MPI_CHK(mpi_inv_mod(&ctx->QP, &ctx->Q, &ctx->P));

	ctx->len = (mpi_msb(&ctx->N) + 7) >> 3;

cleanup:

	mpi_free(&G);
	mpi_free(&H);
	mpi_free(&Q1);
	mpi_free(&P1);

	if (ret != 0)
	{
		rsa_free(ctx);
		return(ERR_RSA_KEY_GEN_FAILED | ret);
	}

	return(0);
}

#endif

int rsa_size(rsa_context* ctx)
{
	return ctx->len;
}

int rsa_import_pubkey(rsa_context *ctx, const unsigned char *data, int len, int ne)
{
	const unsigned char *p = data;
	int i, bys;

	if (len < 4)
		return (ERR_RSA_BAD_INPUT_DATA);

	/*
	* ne=0 means E then N (the keys sent by the server).
	* ne=1 means N then E (the keys stored in a keyfile).
	*/

	if (!ne) {
		bys = 0;
		for (i = 0; i < 4; i++)
		{
			bys = (bys << 8) + *p;
			p++;
		}
		len -= 4;

		if (0 != mpi_read_binary(&(ctx->E), p, bys))
			return (ERR_RSA_KEY_GEN_FAILED);
		
		p += bys;
		len -= bys;
	}

	bys = 0;
	for (i = 0; i < 4; i++)
	{
		bys = (bys << 8) + *p;
		p++;
	}
	len -= 4;

	if (0 != mpi_read_binary(&(ctx->N), p, bys))
		return (ERR_RSA_KEY_GEN_FAILED);

	ctx->len = (mpi_msb(&ctx->N) + 7) >> 3;

	p += bys;
	len -= bys;

	if (ne) {
		bys = 0;
		for (i = 0; i < 4; i++)
		{
			bys = (bys << 8) + *p;
			p++;
		}
		len -= 4;

		if (0 != mpi_read_binary(&(ctx->E), p, bys))
			return (ERR_RSA_KEY_GEN_FAILED);

		p += bys;
		len -= bys;
	}

	return (0);
}

int rsa_pubkey_size(rsa_context* ctx)
{
	return 4 + mpi_size(&ctx->N) + 4 + mpi_size(&ctx->E);
}

int rsa_export_pubkey(rsa_context *ctx, unsigned char *data, int* olen, int ne)
{
	unsigned char* p = data;
	int bys;

	if (!ne)
	{
		bys = mpi_size(&ctx->E);
		*p++ = (unsigned char)((bys >> 24) & 0xFF);
		*p++ = (unsigned char)((bys >> 16) & 0xFF);
		*p++ = (unsigned char)((bys >> 8) & 0xFF);
		*p++ = (unsigned char)((bys) & 0xFF);

		mpi_write_binary(&ctx->E, p, bys);
		p += bys;
	}

	bys = mpi_size(&ctx->N);
	*p++ = (unsigned char)((bys >> 24) & 0xFF);
	*p++ = (unsigned char)((bys >> 16) & 0xFF);
	*p++ = (unsigned char)((bys >> 8) & 0xFF);
	*p++ = (unsigned char)((bys)& 0xFF);

	mpi_write_binary(&ctx->N, p, bys);
	p += bys;

	if (ne)
	{
		bys = mpi_size(&ctx->E);
		*p++ = (unsigned char)((bys >> 24) & 0xFF);
		*p++ = (unsigned char)((bys >> 16) & 0xFF);
		*p++ = (unsigned char)((bys >> 8) & 0xFF);
		*p++ = (unsigned char)((bys)& 0xFF);

		mpi_write_binary(&ctx->E, p, bys);
		p += bys;
	}

	*olen = (p - data);

	return (0);
}

/*
* Check a public RSA key
*/
int rsa_check_pubkey(rsa_context *ctx)
{
	if ((ctx->N.p[0] & 1) == 0 ||
		(ctx->E.p[0] & 1) == 0)
		return(ERR_RSA_KEY_CHECK_FAILED);

	if (mpi_msb(&ctx->N) < 128 ||
		mpi_msb(&ctx->N) > 4096)
		return(ERR_RSA_KEY_CHECK_FAILED);

	if (mpi_msb(&ctx->E) < 2 ||
		mpi_msb(&ctx->E) > 64)
		return(ERR_RSA_KEY_CHECK_FAILED);

	return(0);
}

/*
* Check a private RSA key
*/
int rsa_check_privkey(rsa_context *ctx)
{
	int ret;
	mpi PQ, DE, P1, Q1, H, I, G;

	if ((ret = rsa_check_pubkey(ctx)) != 0)
		return(ret);

	mpi_init(&PQ);
	mpi_init(&DE);
	mpi_init(&P1);
	mpi_init(&Q1);
	mpi_init(&H);
	mpi_init(&I);
	mpi_init(&G);

	MPI_CHK(mpi_mul_mpi(&PQ, &ctx->P, &ctx->Q));
	MPI_CHK(mpi_mul_mpi(&DE, &ctx->D, &ctx->E));
	MPI_CHK(mpi_sub_int(&P1, &ctx->P, 1));
	MPI_CHK(mpi_sub_int(&Q1, &ctx->Q, 1));
	MPI_CHK(mpi_mul_mpi(&H, &P1, &Q1));
	MPI_CHK(mpi_mod_mpi(&I, &DE, &H));
	MPI_CHK(mpi_gcd(&G, &ctx->E, &H));

	if (mpi_cmp_mpi(&PQ, &ctx->N) == 0 &&
		mpi_cmp_int(&I, 1) == 0 &&
		mpi_cmp_int(&G, 1) == 0)
	{
		mpi_free(&G);
		mpi_free(&I);
		mpi_free(&H);
		mpi_free(&Q1);
		mpi_free(&P1);
		mpi_free(&DE);
		mpi_free(&PQ);
		return(0);
	}

cleanup:

	mpi_free(&G);
	mpi_free(&I);
	mpi_free(&H);
	mpi_free(&Q1);
	mpi_free(&P1);
	mpi_free(&DE);
	mpi_free(&PQ);
	return(ERR_RSA_KEY_CHECK_FAILED | ret);
}

/*
* Do an RSA public key operation
*/
int rsa_public(rsa_context *ctx,
	unsigned char *input,
	unsigned char *output)
{
	int ret, olen;
	mpi T;

	mpi_init(&T);

	MPI_CHK(mpi_read_binary(&T, input, ctx->len));

	if (mpi_cmp_mpi(&T, &ctx->N) >= 0)
	{
		mpi_free(&T);
		return(ERR_RSA_BAD_INPUT_DATA);
	}

	olen = ctx->len;
	MPI_CHK(mpi_exp_mod(&T, &T, &ctx->E, &ctx->N, &ctx->RN));
	MPI_CHK(mpi_write_binary(&T, output, olen));

cleanup:

	mpi_free(&T);

	if (ret != 0)
		return(ERR_RSA_PUBLIC_FAILED | ret);

	return(0);
}

/*
* Do an RSA private key operation
*/
int rsa_private(rsa_context *ctx,
	unsigned char *input,
	unsigned char *output)
{
	int ret, olen;
	mpi T, T1, T2;

	mpi_init(&T);
	mpi_init(&T1);
	mpi_init(&T2);

	MPI_CHK(mpi_read_binary(&T, input, ctx->len));

	if (mpi_cmp_mpi(&T, &ctx->N) >= 0)
	{
		mpi_free(&T);
		return(ERR_RSA_BAD_INPUT_DATA);
	}

#if 0
	MPI_CHK(mpi_exp_mod(&T, &T, &ctx->D, &ctx->N, &ctx->RN));
#else
	/*
	* faster decryption using the CRT
	*
	* T1 = input ^ dP mod P
	* T2 = input ^ dQ mod Q
	*/
	MPI_CHK(mpi_exp_mod(&T1, &T, &ctx->DP, &ctx->P, &ctx->RP));
	MPI_CHK(mpi_exp_mod(&T2, &T, &ctx->DQ, &ctx->Q, &ctx->RQ));

	/*
	* T = (T1 - T2) * (Q^-1 mod P) mod P
	*/
	MPI_CHK(mpi_sub_mpi(&T, &T1, &T2));
	MPI_CHK(mpi_mul_mpi(&T1, &T, &ctx->QP));
	MPI_CHK(mpi_mod_mpi(&T, &T1, &ctx->P));

	/*
	* output = T2 + T * Q
	*/
	MPI_CHK(mpi_mul_mpi(&T1, &T, &ctx->Q));
	MPI_CHK(mpi_add_mpi(&T, &T2, &T1));
#endif

	olen = ctx->len;
	MPI_CHK(mpi_write_binary(&T, output, olen));

cleanup:

	mpi_free(&T);
	mpi_free(&T1);
	mpi_free(&T2);

	if (ret != 0)
		return(ERR_RSA_PRIVATE_FAILED | ret);

	return(0);
}

/*
* Add the message padding, then do an RSA operation
*/
int rsa_pkcs1_encrypt(rsa_context *ctx,
	int mode, int  ilen,
	unsigned char *input,
	unsigned char *output)
{
	int nb_pad, olen;
	unsigned char *p = output;

	olen = ctx->len;

	switch (ctx->padding)
	{
	case RSA_PKCS_V15:

		if (ilen < 0 || olen < ilen + 11)
			return(ERR_RSA_BAD_INPUT_DATA);

		nb_pad = olen - 3 - ilen;

		*p++ = 0;
		*p++ = RSA_CRYPT;

		while (nb_pad-- > 0)
		{
			do {
				*p = (unsigned char)rand();
			} while (*p == 0);
			p++;
		}
		*p++ = 0;
		memcpy(p, input, ilen);
		break;

	default:

		return(ERR_RSA_INVALID_PADDING);
	}

	return((mode == RSA_PUBLIC)
		? rsa_public(ctx, output, output)
		: rsa_private(ctx, output, output));
}

/*
* Do an RSA operation, then remove the message padding
*/
int rsa_pkcs1_decrypt(rsa_context *ctx,
	int mode, int *olen,
	unsigned char *input,
	unsigned char *output)
{
	int ret, ilen;
	unsigned char *p;
	unsigned char buf[512];

	ilen = ctx->len;

	if (ilen < 16 || ilen >(int) sizeof(buf))
		return(ERR_RSA_BAD_INPUT_DATA);

	ret = (mode == RSA_PUBLIC)
		? rsa_public(ctx, input, buf)
		: rsa_private(ctx, input, buf);

	if (ret != 0)
		return(ret);

	p = buf;

	switch (ctx->padding)
	{
	case RSA_PKCS_V15:

		if (*p++ != 0 || *p++ != RSA_CRYPT)
			return(ERR_RSA_INVALID_PADDING);

		while (*p != 0)
		{
			if (p >= buf + ilen - 1)
				return(ERR_RSA_INVALID_PADDING);
			p++;
		}
		p++;
		break;

	default:

		return(ERR_RSA_INVALID_PADDING);
	}

	*olen = ilen - (int)(p - buf);
	memcpy(output, p, *olen);

	return(0);
}

/*
* Do an RSA operation to sign the message digest
*/
int rsa_pkcs1_sign(rsa_context *ctx,
	int mode,
	int hash_id,
	int hashlen,
	unsigned char *hash,
	unsigned char *sig)
{
	int nb_pad, olen;
	unsigned char *p = sig;

	olen = ctx->len;

	switch (ctx->padding)
	{
	case RSA_PKCS_V15:

		switch (hash_id)
		{
		case RSA_RAW:
			nb_pad = olen - 3 - hashlen;
			break;

		case RSA_MD2:
		case RSA_MD4:
		case RSA_MD5:
			nb_pad = olen - 3 - 34;
			break;

		case RSA_SHA1:
			nb_pad = olen - 3 - 35;
			break;

		default:
			return(ERR_RSA_BAD_INPUT_DATA);
		}

		if (nb_pad < 8)
			return(ERR_RSA_BAD_INPUT_DATA);

		*p++ = 0;
		*p++ = RSA_SIGN;
		memset(p, 0xFF, nb_pad);
		p += nb_pad;
		*p++ = 0;
		break;

	default:

		return(ERR_RSA_INVALID_PADDING);
	}

	switch (hash_id)
	{
	case RSA_RAW:
		memcpy(p, hash, hashlen);
		break;

	case RSA_MD2:
		memcpy(p, ASN1_HASH_MDX, 18);
		memcpy(p + 18, hash, 16);
		p[13] = 2; break;

	case RSA_MD4:
		memcpy(p, ASN1_HASH_MDX, 18);
		memcpy(p + 18, hash, 16);
		p[13] = 4; break;

	case RSA_MD5:
		memcpy(p, ASN1_HASH_MDX, 18);
		memcpy(p + 18, hash, 16);
		p[13] = 5; break;

	case RSA_SHA1:
		memcpy(p, ASN1_HASH_SHA1, 15);
		memcpy(p + 15, hash, 20);
		break;

	default:
		return(ERR_RSA_BAD_INPUT_DATA);
	}

	return((mode == RSA_PUBLIC)
		? rsa_public(ctx, sig, sig)
		: rsa_private(ctx, sig, sig));
}

/*
* Do an RSA operation and check the message digest
*/
int rsa_pkcs1_verify(rsa_context *ctx,
	int mode,
	int hash_id,
	int hashlen,
	unsigned char *hash,
	unsigned char *sig)
{
	int ret, len, siglen;
	unsigned char *p, c;
	unsigned char buf[512];

	siglen = ctx->len;

	if (siglen < 16 || siglen >(int) sizeof(buf))
		return(ERR_RSA_BAD_INPUT_DATA);

	ret = (mode == RSA_PUBLIC)
		? rsa_public(ctx, sig, buf)
		: rsa_private(ctx, sig, buf);

	if (ret != 0)
		return(ret);

	p = buf;

	switch (ctx->padding)
	{
	case RSA_PKCS_V15:

		if (*p++ != 0 || *p++ != RSA_SIGN)
			return(ERR_RSA_INVALID_PADDING);

		while (*p != 0)
		{
			if (p >= buf + siglen - 1 || *p != 0xFF)
				return(ERR_RSA_INVALID_PADDING);
			p++;
		}
		p++;
		break;

	default:

		return(ERR_RSA_INVALID_PADDING);
	}

	len = siglen - (int)(p - buf);

	if (len == 34)
	{
		c = p[13];
		p[13] = 0;

		if (memcmp(p, ASN1_HASH_MDX, 18) != 0)
			return(ERR_RSA_VERIFY_FAILED);

		if ((c == 2 && hash_id == RSA_MD2) ||
			(c == 4 && hash_id == RSA_MD4) ||
			(c == 5 && hash_id == RSA_MD5))
		{
			if (memcmp(p + 18, hash, 16) == 0)
				return(0);
			else
				return(ERR_RSA_VERIFY_FAILED);
		}
	}

	if (len == 35 && hash_id == RSA_SHA1)
	{
		if (memcmp(p, ASN1_HASH_SHA1, 15) != 0)
			return(ERR_RSA_VERIFY_FAILED);

		if (memcmp(p + 15, hash, 20) == 0)
			return(0);
		else
			return(ERR_RSA_VERIFY_FAILED);
	}

	if (len == hashlen && hash_id == RSA_RAW)
	{
		if (memcmp(p, hash, hashlen) == 0)
			return(0);
		else
			return(ERR_RSA_VERIFY_FAILED);
	}

	while (len >= hashlen)
	{
		if (memcmp(p, hash, hashlen) == 0)
			return(0);
		p++;
		len--;
	} 

	return(ERR_RSA_INVALID_PADDING);
}

/*
* Free the components of an RSA key
*/
void rsa_free(rsa_context *ctx)
{
	mpi_free(&ctx->RQ);
	mpi_free(&ctx->RP);
	mpi_free(&ctx->RN);
	mpi_free(&ctx->QP);
	mpi_free(&ctx->DQ);
	mpi_free(&ctx->DP);
	mpi_free(&ctx->Q);
	mpi_free(&ctx->P);
	mpi_free(&ctx->D);
	mpi_free(&ctx->E);
	mpi_free(&ctx->N);
}

