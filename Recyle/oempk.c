/*
 *  Public Key abstraction layer
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include "oempk.h"
#include <stdlib.h>
#include <string.h>

#define INTERNAL_VALIDATE_RET( cond, ret )  do { } while( 0 )
#define INTERNAL_VALIDATE( cond )           do { } while( 0 )

/* Parameter validation macros based on platform_util.h */
#define PK_VALIDATE_RET( cond )    \
    INTERNAL_VALIDATE_RET( cond, ERR_PK_BAD_INPUT_DATA )
#define PK_VALIDATE( cond )        \
    INTERNAL_VALIDATE( cond )

/*
* Initialise a pk_context
*/
void pk_init(pk_context *ctx)
{
	PK_VALIDATE(ctx != NULL);

	ctx->pk_info = NULL;
	ctx->pk_ctx = NULL;
}

/*
* Free (the components of) a pk_context
*/
void pk_free(pk_context *ctx)
{
	if (ctx == NULL)
		return;

	if (ctx->pk_info != NULL)
		ctx->pk_info->ctx_free_func(ctx->pk_ctx);

	memset(ctx, 0, sizeof(pk_context));
}

#if defined(ECP_RESTARTABLE)
/*
* Initialize a restart context
*/
void pk_restart_init(pk_restart_ctx *ctx)
{
	PK_VALIDATE(ctx != NULL);
	ctx->pk_info = NULL;
	ctx->rs_ctx = NULL;
}

/*
* Free the components of a restart context
*/
void pk_restart_free(pk_restart_ctx *ctx)
{
	if (ctx == NULL || ctx->pk_info == NULL ||
		ctx->pk_info->rs_free_func == NULL)
	{
		return;
	}

	ctx->pk_info->rs_free_func(ctx->rs_ctx);

	ctx->pk_info = NULL;
	ctx->rs_ctx = NULL;
}
#endif /* ECDSA_C && ECP_RESTARTABLE */

/*
* Get pk_info structure from type
*/
const pk_info_t * pk_info_from_type(pk_type_t pk_type)
{
	switch (pk_type) {
	case PK_RSA:
		return(&rsa_info);
	case PK_ECKEY:
		return(&eckey_info);
	case PK_ECKEY_DH:
		return(&eckeydh_info);
	case PK_ECDSA:
		return(&ecdsa_info);
		/* PK_RSA_ALT omitted on purpose */
	default:
		return(NULL);
	}
}

/*
* Initialise context
*/
int pk_setup(pk_context *ctx, const pk_info_t *info)
{
	PK_VALIDATE_RET(ctx != NULL);
	if (info == NULL || ctx->pk_info != NULL)
		return(ERR_PK_BAD_INPUT_DATA);

	if ((ctx->pk_ctx = info->ctx_alloc_func()) == NULL)
		return(ERR_PK_ALLOC_FAILED);

	ctx->pk_info = info;

	return(0);
}

#if defined(PK_RSA_ALT_SUPPORT)
/*
* Initialize an RSA-alt context
*/
int pk_setup_rsa_alt(pk_context *ctx, void * key,
	pk_rsa_alt_decrypt_func decrypt_func,
	pk_rsa_alt_sign_func sign_func,
	pk_rsa_alt_key_len_func key_len_func)
{
	rsa_alt_context *rsa_alt;
	const pk_info_t *info = &rsa_alt_info;

	PK_VALIDATE_RET(ctx != NULL);
	if (ctx->pk_info != NULL)
		return(ERR_PK_BAD_INPUT_DATA);

	if ((ctx->pk_ctx = info->ctx_alloc_func()) == NULL)
		return(ERR_PK_ALLOC_FAILED);

	ctx->pk_info = info;

	rsa_alt = (rsa_alt_context *)ctx->pk_ctx;

	rsa_alt->key = key;
	rsa_alt->decrypt_func = decrypt_func;
	rsa_alt->sign_func = sign_func;
	rsa_alt->key_len_func = key_len_func;

	return(0);
}
#endif /* PK_RSA_ALT_SUPPORT */

/*
* Tell if a PK can do the operations of the given type
*/
int pk_can_do(const pk_context *ctx, pk_type_t type)
{
	/* A context with null pk_info is not set up yet and can't do anything.
	* For backward compatibility, also accept NULL instead of a context
	* pointer. */
	if (ctx == NULL || ctx->pk_info == NULL)
		return(0);

	return(ctx->pk_info->can_do(type));
}

/*
* Helper for pk_sign and pk_verify
*/
static int pk_hashlen_helper(md_type_t md_alg, size_t *hash_len)
{
	const md_info_t *md_info;

	if (*hash_len != 0)
		return(0);

	if ((md_info = md_info_from_type(md_alg)) == NULL)
		return(-1);

	*hash_len = md_get_size(md_info);
	return(0);
}

#if defined(ECP_RESTARTABLE)
/*
* Helper to set up a restart context if needed
*/
static int pk_restart_setup(pk_restart_ctx *ctx,
	const pk_info_t *info)
{
	/* Don't do anything if already set up or invalid */
	if (ctx == NULL || ctx->pk_info != NULL)
		return(0);

	/* Should never happen when we're called */
	if (info->rs_alloc_func == NULL || info->rs_free_func == NULL)
		return(ERR_PK_BAD_INPUT_DATA);

	if ((ctx->rs_ctx = info->rs_alloc_func()) == NULL)
		return(ERR_PK_ALLOC_FAILED);

	ctx->pk_info = info;

	return(0);
}
#endif /* ECDSA_C && ECP_RESTARTABLE */

/*
* Verify a signature (restartable)
*/
int pk_verify_restartable(pk_context *ctx,
	md_type_t md_alg,
	const unsigned char *hash, size_t hash_len,
	const unsigned char *sig, size_t sig_len,
	pk_restart_ctx *rs_ctx)
{
	PK_VALIDATE_RET(ctx != NULL);
	PK_VALIDATE_RET((md_alg == MD_NONE && hash_len == 0) ||
		hash != NULL);
	PK_VALIDATE_RET(sig != NULL);

	if (ctx->pk_info == NULL ||
		pk_hashlen_helper(md_alg, &hash_len) != 0)
		return(ERR_PK_BAD_INPUT_DATA);

#if defined(ECP_RESTARTABLE)
	/* optimization: use non-restartable version if restart disabled */
	if (rs_ctx != NULL &&
		ecp_restart_is_enabled() &&
		ctx->pk_info->verify_rs_func != NULL)
	{
		int ret;

		if ((ret = pk_restart_setup(rs_ctx, ctx->pk_info)) != 0)
			return(ret);

		ret = ctx->pk_info->verify_rs_func(ctx->pk_ctx,
			md_alg, hash, hash_len, sig, sig_len, rs_ctx->rs_ctx);

		if (ret != ERR_ECP_IN_PROGRESS)
			pk_restart_free(rs_ctx);

		return(ret);
	}
#else /* ECDSA_C && ECP_RESTARTABLE */
	(void)rs_ctx;
#endif /* ECDSA_C && ECP_RESTARTABLE */

	if (ctx->pk_info->verify_func == NULL)
		return(ERR_PK_TYPE_MISMATCH);

	return(ctx->pk_info->verify_func(ctx->pk_ctx, md_alg, hash, hash_len,
		sig, sig_len));
}

/*
* Verify a signature
*/
int pk_verify(pk_context *ctx, md_type_t md_alg,
	const unsigned char *hash, size_t hash_len,
	const unsigned char *sig, size_t sig_len)
{
	return(pk_verify_restartable(ctx, md_alg, hash, hash_len,
		sig, sig_len, NULL));
}

/*
* Verify a signature with options
*/
int pk_verify_ext(pk_type_t type, const void *options,
	pk_context *ctx, md_type_t md_alg,
	const unsigned char *hash, size_t hash_len,
	const unsigned char *sig, size_t sig_len)
{
	PK_VALIDATE_RET(ctx != NULL);
	PK_VALIDATE_RET((md_alg == MD_NONE && hash_len == 0) ||
		hash != NULL);
	PK_VALIDATE_RET(sig != NULL);

	if (ctx->pk_info == NULL)
		return(ERR_PK_BAD_INPUT_DATA);

	if (!pk_can_do(ctx, type))
		return(ERR_PK_TYPE_MISMATCH);

	if (type == PK_RSASSA_PSS)
	{
#if defined(PKCS1_V21)
		int ret;
		const pk_rsassa_pss_options *pss_opts;

#if SIZE_MAX > UINT_MAX
		if (md_alg == MD_NONE && UINT_MAX < hash_len)
			return(ERR_PK_BAD_INPUT_DATA);
#endif /* SIZE_MAX > UINT_MAX */

		if (options == NULL)
			return(ERR_PK_BAD_INPUT_DATA);

		pss_opts = (const pk_rsassa_pss_options *)options;

		if (sig_len < pk_get_len(ctx))
			return(ERR_RSA_VERIFY_FAILED);

		ret = rsa_rsassa_pss_verify_ext(pk_rsa(*ctx),
			NULL, NULL, RSA_PUBLIC,
			md_alg, (unsigned int)hash_len, hash,
			pss_opts->mgf1_hash_id,
			pss_opts->expected_salt_len,
			sig);
		if (ret != 0)
			return(ret);

		if (sig_len > pk_get_len(ctx))
			return(ERR_PK_SIG_LEN_MISMATCH);

		return(0);
#else
		return(ERR_PK_FEATURE_UNAVAILABLE);
#endif /* RSA_C && PKCS1_V21 */
	}

	/* General case: no options */
	if (options != NULL)
		return(ERR_PK_BAD_INPUT_DATA);

	return(pk_verify(ctx, md_alg, hash, hash_len, sig, sig_len));
}

/*
* Make a signature (restartable)
*/
int pk_sign_restartable(pk_context *ctx,
	md_type_t md_alg,
	const unsigned char *hash, size_t hash_len,
	unsigned char *sig, size_t *sig_len,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng,
	pk_restart_ctx *rs_ctx)
{
	PK_VALIDATE_RET(ctx != NULL);
	PK_VALIDATE_RET((md_alg == MD_NONE && hash_len == 0) ||
		hash != NULL);
	PK_VALIDATE_RET(sig != NULL);

	if (ctx->pk_info == NULL ||
		pk_hashlen_helper(md_alg, &hash_len) != 0)
		return(ERR_PK_BAD_INPUT_DATA);

#if defined(ECP_RESTARTABLE)
	/* optimization: use non-restartable version if restart disabled */
	if (rs_ctx != NULL &&
		ecp_restart_is_enabled() &&
		ctx->pk_info->sign_rs_func != NULL)
	{
		int ret;

		if ((ret = pk_restart_setup(rs_ctx, ctx->pk_info)) != 0)
			return(ret);

		ret = ctx->pk_info->sign_rs_func(ctx->pk_ctx, md_alg,
			hash, hash_len, sig, sig_len, f_rng, p_rng, rs_ctx->rs_ctx);

		if (ret != ERR_ECP_IN_PROGRESS)
			pk_restart_free(rs_ctx);

		return(ret);
	}
#else /* ECDSA_C && ECP_RESTARTABLE */
	(void)rs_ctx;
#endif /* ECDSA_C && ECP_RESTARTABLE */

	if (ctx->pk_info->sign_func == NULL)
		return(ERR_PK_TYPE_MISMATCH);

	return(ctx->pk_info->sign_func(ctx->pk_ctx, md_alg, hash, hash_len,
		sig, sig_len, f_rng, p_rng));
}

/*
* Make a signature
*/
int pk_sign(pk_context *ctx, md_type_t md_alg,
	const unsigned char *hash, size_t hash_len,
	unsigned char *sig, size_t *sig_len,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	return(pk_sign_restartable(ctx, md_alg, hash, hash_len,
		sig, sig_len, f_rng, p_rng, NULL));
}

/*
* Decrypt message
*/
int pk_decrypt(pk_context *ctx,
	const unsigned char *input, size_t ilen,
	unsigned char *output, size_t *olen, size_t osize,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	PK_VALIDATE_RET(ctx != NULL);
	PK_VALIDATE_RET(input != NULL || ilen == 0);
	PK_VALIDATE_RET(output != NULL || osize == 0);
	PK_VALIDATE_RET(olen != NULL);

	if (ctx->pk_info == NULL)
		return(ERR_PK_BAD_INPUT_DATA);

	if (ctx->pk_info->decrypt_func == NULL)
		return(ERR_PK_TYPE_MISMATCH);

	return(ctx->pk_info->decrypt_func(ctx->pk_ctx, input, ilen,
		output, olen, osize, f_rng, p_rng));
}

/*
* Encrypt message
*/
int pk_encrypt(pk_context *ctx,
	const unsigned char *input, size_t ilen,
	unsigned char *output, size_t *olen, size_t osize,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	PK_VALIDATE_RET(ctx != NULL);
	PK_VALIDATE_RET(input != NULL || ilen == 0);
	PK_VALIDATE_RET(output != NULL || osize == 0);
	PK_VALIDATE_RET(olen != NULL);

	if (ctx->pk_info == NULL)
		return(ERR_PK_BAD_INPUT_DATA);

	if (ctx->pk_info->encrypt_func == NULL)
		return(ERR_PK_TYPE_MISMATCH);

	return(ctx->pk_info->encrypt_func(ctx->pk_ctx, input, ilen,
		output, olen, osize, f_rng, p_rng));
}

/*
* Check public-private key pair
*/
int pk_check_pair(const pk_context *pub, const pk_context *prv)
{
	PK_VALIDATE_RET(pub != NULL);
	PK_VALIDATE_RET(prv != NULL);

	if (pub->pk_info == NULL ||
		prv->pk_info == NULL ||
		prv->pk_info->check_pair_func == NULL)
	{
		return(ERR_PK_BAD_INPUT_DATA);
	}

	if (prv->pk_info->type == PK_RSA_ALT)
	{
		if (pub->pk_info->type != PK_RSA)
			return(ERR_PK_TYPE_MISMATCH);
	}
	else
	{
		if (pub->pk_info != prv->pk_info)
			return(ERR_PK_TYPE_MISMATCH);
	}

	return(prv->pk_info->check_pair_func(pub->pk_ctx, prv->pk_ctx));
}

/*
* Get key size in bits
*/
size_t pk_get_bitlen(const pk_context *ctx)
{
	/* For backward compatibility, accept NULL or a context that
	* isn't set up yet, and return a fake value that should be safe. */
	if (ctx == NULL || ctx->pk_info == NULL)
		return(0);

	return(ctx->pk_info->get_bitlen(ctx->pk_ctx));
}

/*
* Export debug information
*/
int pk_debug(const pk_context *ctx, pk_debug_item *items)
{
	PK_VALIDATE_RET(ctx != NULL);
	if (ctx->pk_info == NULL)
		return(ERR_PK_BAD_INPUT_DATA);

	if (ctx->pk_info->debug_func == NULL)
		return(ERR_PK_TYPE_MISMATCH);

	ctx->pk_info->debug_func(ctx->pk_ctx, items);
	return(0);
}

/*
* Access the PK type name
*/
const char *pk_get_name(const pk_context *ctx)
{
	if (ctx == NULL || ctx->pk_info == NULL)
		return("invalid PK");

	return(ctx->pk_info->name);
}

/*
* Access the PK type
*/
pk_type_t pk_get_type(const pk_context *ctx)
{
	if (ctx == NULL || ctx->pk_info == NULL)
		return(PK_NONE);

	return(ctx->pk_info->type);
}