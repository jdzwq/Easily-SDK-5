/**
 * \file md.c
 *
 * \brief Generic message digest wrapper for mbed TLS
 *
 * \author Adriaan de Jong <dejong@fox-it.com>
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

#include "oemmd.h"
#include <stdlib.h>
#include <string.h>

/*
* Reminder: update profiles in x509_crt.c when adding a new hash!
*/
static const int supported_digests[] = {

#if defined(SHA512_C)
	MD_SHA512,
	MD_SHA384,
#endif

#if defined(SHA256_C)
	MD_SHA256,
	MD_SHA224,
#endif

#if defined(SHA1_C)
	MD_SHA1,
#endif

#if defined(RIPEMD160_C)
	MD_RIPEMD160,
#endif

#if defined(MD5_C)
	MD_MD5,
#endif

#if defined(MD4_C)
	MD_MD4,
#endif

#if defined(MD2_C)
	MD_MD2,
#endif

	MD_NONE
};

const int *md_list(void)
{
	return(supported_digests);
}

const md_info_t *md_info_from_string(const char *md_name)
{
	if (NULL == md_name)
		return(NULL);

	/* Get the appropriate digest information */
#if defined(MD2_C)
	if (!strcmp("MD2", md_name))
		return md_info_from_type(MD_MD2);
#endif
#if defined(MD4_C)
	if (!strcmp("MD4", md_name))
		return md_info_from_type(MD_MD4);
#endif
#if defined(MD5_C)
	if (!strcmp("MD5", md_name))
		return md_info_from_type(MD_MD5);
#endif
#if defined(RIPEMD160_C)
	if (!strcmp("RIPEMD160", md_name))
		return md_info_from_type(MD_RIPEMD160);
#endif
#if defined(SHA1_C)
	if (!strcmp("SHA1", md_name) || !strcmp("SHA", md_name))
		return md_info_from_type(MD_SHA1);
#endif
#if defined(SHA256_C)
	if (!strcmp("SHA224", md_name))
		return md_info_from_type(MD_SHA224);
	if (!strcmp("SHA256", md_name))
		return md_info_from_type(MD_SHA256);
#endif
#if defined(SHA512_C)
	if (!strcmp("SHA384", md_name))
		return md_info_from_type(MD_SHA384);
	if (!strcmp("SHA512", md_name))
		return md_info_from_type(MD_SHA512);
#endif
	return(NULL);
}

const md_info_t *md_info_from_type(md_type_t md_type)
{
	switch (md_type)
	{
#if defined(MD2_C)
	case MD_MD2:
		return(&md2_info);
#endif
#if defined(MD4_C)
	case MD_MD4:
		return(&md4_info);
#endif
#if defined(MD5_C)
	case MD_MD5:
		return(&md5_info);
#endif
#if defined(RIPEMD160_C)
	case MD_RIPEMD160:
		return(&ripemd160_info);
#endif
#if defined(SHA1_C)
	case MD_SHA1:
		return(&sha1_info);
#endif
#if defined(SHA256_C)
	case MD_SHA224:
		return(&sha224_info);
	case MD_SHA256:
		return(&sha256_info);
#endif
#if defined(SHA512_C)
	case MD_SHA384:
		return(&sha384_info);
	case MD_SHA512:
		return(&sha512_info);
#endif
	default:
		return(NULL);
	}
}

void md_init(md_context_t *ctx)
{
	memset(ctx, 0, sizeof(md_context_t));
}

void md_free(md_context_t *ctx)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return;

	if (ctx->md_ctx != NULL)
		ctx->md_info->ctx_free_func(ctx->md_ctx);

	if (ctx->hmac_ctx != NULL)
	{
		memset(ctx->hmac_ctx, 0, 2 * ctx->md_info->block_size);
		free(ctx->hmac_ctx);
	}

	memset(ctx, 0, sizeof(md_context_t));
}

int md_clone(md_context_t *dst,
	const md_context_t *src)
{
	if (dst == NULL || dst->md_info == NULL ||
		src == NULL || src->md_info == NULL ||
		dst->md_info != src->md_info)
	{
		return(ERR_MD_BAD_INPUT_DATA);
	}

	dst->md_info->clone_func(dst->md_ctx, src->md_ctx);

	return(0);
}

#if ! defined(DEPRECATED_REMOVED)
int md_init_ctx(md_context_t *ctx, const md_info_t *md_info)
{
	return md_setup(ctx, md_info, 1);
}
#endif

int md_setup(md_context_t *ctx, const md_info_t *md_info, int hmac)
{
	if (md_info == NULL || ctx == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	if ((ctx->md_ctx = md_info->ctx_alloc_func()) == NULL)
		return(ERR_MD_ALLOC_FAILED);

	if (hmac != 0)
	{
		ctx->hmac_ctx = calloc(2, md_info->block_size);
		if (ctx->hmac_ctx == NULL)
		{
			md_info->ctx_free_func(ctx->md_ctx);
			return(ERR_MD_ALLOC_FAILED);
		}
	}

	ctx->md_info = md_info;

	return(0);
}

int md_starts(md_context_t *ctx)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	return(ctx->md_info->starts_func(ctx->md_ctx));
}

int md_update(md_context_t *ctx, const unsigned char *input, size_t ilen)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	return(ctx->md_info->update_func(ctx->md_ctx, input, ilen));
}

int md_finish(md_context_t *ctx, unsigned char *output)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	return(ctx->md_info->finish_func(ctx->md_ctx, output));
}

int md(const md_info_t *md_info, const unsigned char *input, size_t ilen,
	unsigned char *output)
{
	if (md_info == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	return(md_info->digest_func(input, ilen, output));
}

int md_hmac_starts(md_context_t *ctx, const unsigned char *key, size_t keylen)
{
	int ret;
	unsigned char sum[MD_MAX_SIZE];
	unsigned char *ipad, *opad;
	size_t i;

	if (ctx == NULL || ctx->md_info == NULL || ctx->hmac_ctx == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	if (keylen > (size_t)ctx->md_info->block_size)
	{
		if ((ret = ctx->md_info->starts_func(ctx->md_ctx)) != 0)
			goto cleanup;
		if ((ret = ctx->md_info->update_func(ctx->md_ctx, key, keylen)) != 0)
			goto cleanup;
		if ((ret = ctx->md_info->finish_func(ctx->md_ctx, sum)) != 0)
			goto cleanup;

		keylen = ctx->md_info->size;
		key = sum;
	}

	ipad = (unsigned char *)ctx->hmac_ctx;
	opad = (unsigned char *)ctx->hmac_ctx + ctx->md_info->block_size;

	memset(ipad, 0x36, ctx->md_info->block_size);
	memset(opad, 0x5C, ctx->md_info->block_size);

	for (i = 0; i < keylen; i++)
	{
		ipad[i] = (unsigned char)(ipad[i] ^ key[i]);
		opad[i] = (unsigned char)(opad[i] ^ key[i]);
	}

	if ((ret = ctx->md_info->starts_func(ctx->md_ctx)) != 0)
		goto cleanup;
	if ((ret = ctx->md_info->update_func(ctx->md_ctx, ipad,
		ctx->md_info->block_size)) != 0)
		goto cleanup;

cleanup:
	memset(sum, 0, sizeof(sum));

	return(ret);
}

int md_hmac_update(md_context_t *ctx, const unsigned char *input, size_t ilen)
{
	if (ctx == NULL || ctx->md_info == NULL || ctx->hmac_ctx == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	return(ctx->md_info->update_func(ctx->md_ctx, input, ilen));
}

int md_hmac_finish(md_context_t *ctx, unsigned char *output)
{
	int ret;
	unsigned char tmp[MD_MAX_SIZE];
	unsigned char *opad;

	if (ctx == NULL || ctx->md_info == NULL || ctx->hmac_ctx == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	opad = (unsigned char *)ctx->hmac_ctx + ctx->md_info->block_size;

	if ((ret = ctx->md_info->finish_func(ctx->md_ctx, tmp)) != 0)
		return(ret);
	if ((ret = ctx->md_info->starts_func(ctx->md_ctx)) != 0)
		return(ret);
	if ((ret = ctx->md_info->update_func(ctx->md_ctx, opad,
		ctx->md_info->block_size)) != 0)
		return(ret);
	if ((ret = ctx->md_info->update_func(ctx->md_ctx, tmp,
		ctx->md_info->size)) != 0)
		return(ret);
	return(ctx->md_info->finish_func(ctx->md_ctx, output));
}

int md_hmac_reset(md_context_t *ctx)
{
	int ret;
	unsigned char *ipad;

	if (ctx == NULL || ctx->md_info == NULL || ctx->hmac_ctx == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	ipad = (unsigned char *)ctx->hmac_ctx;

	if ((ret = ctx->md_info->starts_func(ctx->md_ctx)) != 0)
		return(ret);
	return(ctx->md_info->update_func(ctx->md_ctx, ipad,
		ctx->md_info->block_size));
}

int md_hmac(const md_info_t *md_info,
	const unsigned char *key, size_t keylen,
	const unsigned char *input, size_t ilen,
	unsigned char *output)
{
	md_context_t ctx;
	int ret;

	if (md_info == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	md_init(&ctx);

	if ((ret = md_setup(&ctx, md_info, 1)) != 0)
		goto cleanup;

	if ((ret = md_hmac_starts(&ctx, key, keylen)) != 0)
		goto cleanup;
	if ((ret = md_hmac_update(&ctx, input, ilen)) != 0)
		goto cleanup;
	if ((ret = md_hmac_finish(&ctx, output)) != 0)
		goto cleanup;

cleanup:
	md_free(&ctx);

	return(ret);
}

int md_process(md_context_t *ctx, const unsigned char *data)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return(ERR_MD_BAD_INPUT_DATA);

	return(ctx->md_info->process_func(ctx->md_ctx, data));
}

unsigned char md_get_size(const md_info_t *md_info)
{
	if (md_info == NULL)
		return(0);

	return md_info->size;
}

md_type_t md_get_type(const md_info_t *md_info)
{
	if (md_info == NULL)
		return(MD_NONE);

	return md_info->type;
}

const char *md_get_name(const md_info_t *md_info)
{
	if (md_info == NULL)
		return(NULL);

	return md_info->name;
}
