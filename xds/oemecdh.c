/*
 *  Elliptic curve Diffie-Hellman
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

/*
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 * RFC 4492
 */

#include "oemecdh.h"

#if defined(ECDH_LEGACY_CONTEXT)
typedef ecdh_context ecdh_context_mbed;
#endif

static ecp_group_id ecdh_grp_id(
	const ecdh_context *ctx)
{
#if defined(ECDH_LEGACY_CONTEXT)
	return(ctx->grp.id);
#else
	return(ctx->grp_id);
#endif
}

/*
* Generate public key (restartable version)
*
* Note: this internal function relies on its caller preserving the value of
* the output parameter 'd' across continuation calls. This would not be
* acceptable for a public function but is OK here as we control call sites.
*/
static int ecdh_gen_public_restartable(ecp_group *grp,
	mpi *d, ecp_point *Q,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng,
	ecp_restart_ctx *rs_ctx)
{
	int ret;

	/* If multiplication is in progress, we already generated a privkey */
#if defined(ECP_RESTARTABLE)
	if (rs_ctx == NULL || rs_ctx->rsm == NULL)
#endif
		
	MPI_CHK(ecp_gen_privkey(grp, d, f_rng, p_rng));

	MPI_CHK(ecp_mul_restartable(grp, Q, d, &grp->G, f_rng, p_rng, rs_ctx));

cleanup:
	return(ret);
}

/*
* Generate public key
*/
int ecdh_gen_public(ecp_group *grp, mpi *d, ecp_point *Q,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	return(ecdh_gen_public_restartable(grp, d, Q, f_rng, p_rng, NULL));
}

/*
* Compute shared secret (SEC1 3.3.1)
*/
static int ecdh_compute_shared_restartable(ecp_group *grp,
	mpi *z,
	const ecp_point *Q, const mpi *d,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng,
	ecp_restart_ctx *rs_ctx)
{
	int ret;
	ecp_point P;

	ecp_point_init(&P);

	MPI_CHK(ecp_mul_restartable(grp, &P, d, Q, f_rng, p_rng, rs_ctx));

	if (ecp_is_zero(&P))
	{
		ret = ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	MPI_CHK(mpi_copy(z, &P.X));

cleanup:
	ecp_point_free(&P);

	return(ret);
}

/*
* Compute shared secret (SEC1 3.3.1)
*/
int ecdh_compute_shared(ecp_group *grp, mpi *z,
	const ecp_point *Q, const mpi *d,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	return(ecdh_compute_shared_restartable(grp, z, Q, d, f_rng, p_rng, NULL));
}

static void ecdh_init_internal(ecdh_context_mbed *ctx)
{
	ecp_group_init(&ctx->grp);
	mpi_init(&ctx->d);
	ecp_point_init(&ctx->Q);
	ecp_point_init(&ctx->Qp);
	mpi_init(&ctx->z);

#if defined(ECP_RESTARTABLE)
	ecp_restart_init(&ctx->rs);
#endif
}

/*
* Initialize context
*/
void ecdh_init(ecdh_context *ctx)
{
#if defined(ECDH_LEGACY_CONTEXT)
	ecdh_init_internal(ctx);
	ecp_point_init(&ctx->Vi);
	ecp_point_init(&ctx->Vf);
	mpi_init(&ctx->_d);
#else
	memset(ctx, 0, sizeof(ecdh_context));

	ctx->var = ECDH_VARIANT_NONE;
#endif
	ctx->point_format = ECP_PF_UNCOMPRESSED;
#if defined(ECP_RESTARTABLE)
	ctx->restart_enabled = 0;
#endif
}

static int ecdh_setup_internal(ecdh_context_mbed *ctx,
	ecp_group_id grp_id)
{
	int ret;

	ret = ecp_group_load(&ctx->grp, grp_id);
	if (ret != 0)
	{
		return(ERR_ECP_FEATURE_UNAVAILABLE);
	}

	return(0);
}

/*
* Setup context
*/
int ecdh_setup(ecdh_context *ctx, ecp_group_id grp_id)
{
#if defined(ECDH_LEGACY_CONTEXT)
	return(ecdh_setup_internal(ctx, grp_id));
#else
	switch (grp_id)
	{
	default:
		ctx->point_format = ECP_PF_UNCOMPRESSED;
		ctx->var = ECDH_VARIANT_MBEDTLS_2_0;
		ctx->grp_id = grp_id;
		ecdh_init_internal(&ctx->ctx.mbed_ecdh);
		return(ecdh_setup_internal(&ctx->ctx.mbed_ecdh, grp_id));
	}
#endif
}

static void ecdh_free_internal(ecdh_context_mbed *ctx)
{
	ecp_group_free(&ctx->grp);
	mpi_free(&ctx->d);
	ecp_point_free(&ctx->Q);
	ecp_point_free(&ctx->Qp);
	mpi_free(&ctx->z);

#if defined(ECP_RESTARTABLE)
	ecp_restart_free(&ctx->rs);
#endif
}

#if defined(ECP_RESTARTABLE)
/*
* Enable restartable operations for context
*/
void ecdh_enable_restart(ecdh_context *ctx)
{
	ctx->restart_enabled = 1;
}
#endif

/*
* Free context
*/
void ecdh_free(ecdh_context *ctx)
{
	if (ctx == NULL)
		return;

#if defined(ECDH_LEGACY_CONTEXT)
	ecp_point_free(&ctx->Vi);
	ecp_point_free(&ctx->Vf);
	mpi_free(&ctx->_d);
	ecdh_free_internal(ctx);
#else
	switch (ctx->var)
	{
	case ECDH_VARIANT_MBEDTLS_2_0:
		ecdh_free_internal(&ctx->ctx.mbed_ecdh);
		break;
	default:
		break;
	}

	ctx->point_format = ECP_PF_UNCOMPRESSED;
	ctx->var = ECDH_VARIANT_NONE;
	ctx->grp_id = ECP_DP_NONE;
#endif
}

static int ecdh_make_params_internal(ecdh_context_mbed *ctx,
	size_t *olen, int point_format,
	unsigned char *buf, size_t blen,
	int(*f_rng)(void *,
	unsigned char *,
	size_t),
	void *p_rng,
	int restart_enabled)
{
	int ret;
	size_t grp_len, pt_len;
#if defined(ECP_RESTARTABLE)
	ecp_restart_ctx *rs_ctx = NULL;
#endif

	if (ctx->grp.pbits == 0)
		return(ERR_ECP_BAD_INPUT_DATA);

#if defined(ECP_RESTARTABLE)
	if (restart_enabled)
		rs_ctx = &ctx->rs;
#else
	(void)restart_enabled;
#endif


#if defined(ECP_RESTARTABLE)
	if ((ret = ecdh_gen_public_restartable(&ctx->grp, &ctx->d, &ctx->Q,
		f_rng, p_rng, rs_ctx)) != 0)
		return(ret);
#else
	if ((ret = ecdh_gen_public(&ctx->grp, &ctx->d, &ctx->Q,
		f_rng, p_rng)) != 0)
		return(ret);
#endif /* MBEDTLS_ECP_RESTARTABLE */

	if ((ret = ecp_tls_write_group(&ctx->grp, &grp_len, buf,
		blen)) != 0)
		return(ret);

	buf += grp_len;
	blen -= grp_len;

	if ((ret = ecp_tls_write_point(&ctx->grp, &ctx->Q, point_format,
		&pt_len, buf, blen)) != 0)
		return(ret);

	*olen = grp_len + pt_len;
	return(0);
}

/*
* Setup and write the ServerKeyExhange parameters (RFC 4492)
*      struct {
*          ECParameters    curve_params;
*          ECPoint         public;
*      } ServerECDHParams;
*/
int ecdh_make_params(ecdh_context *ctx, size_t *olen,
	unsigned char *buf, size_t blen,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	int restart_enabled = 0;

#if defined(ECP_RESTARTABLE)
	restart_enabled = ctx->restart_enabled;
#else
	(void)restart_enabled;
#endif

#if defined(ECDH_LEGACY_CONTEXT)
	return(ecdh_make_params_internal(ctx, olen, ctx->point_format, buf, blen,
		f_rng, p_rng, restart_enabled));
#else
	switch (ctx->var)
	{
	case ECDH_VARIANT_MBEDTLS_2_0:
		return(ecdh_make_params_internal(&ctx->ctx.mbed_ecdh, olen,
			ctx->point_format, buf, blen,
			f_rng, p_rng,
			restart_enabled));
	default:
		return ERR_ECP_BAD_INPUT_DATA;
	}
#endif
}

static int ecdh_read_params_internal(ecdh_context_mbed *ctx,
	const unsigned char **buf,
	const unsigned char *end)
{
	return(ecp_tls_read_point(&ctx->grp, &ctx->Qp, buf,
		end - *buf));
}

/*
* Read the ServerKeyExhange parameters (RFC 4492)
*      struct {
*          ECParameters    curve_params;
*          ECPoint         public;
*      } ServerECDHParams;
*/
int ecdh_read_params(ecdh_context *ctx,
	const unsigned char **buf,
	const unsigned char *end)
{
	int ret;
	ecp_group_id grp_id;

	if ((ret = ecp_tls_read_group_id(&grp_id, buf, end - *buf))
		!= 0)
		return(ret);

	if ((ret = ecdh_setup(ctx, grp_id)) != 0)
		return(ret);

#if defined(ECDH_LEGACY_CONTEXT)
	return(ecdh_read_params_internal(ctx, buf, end));
#else
	switch (ctx->var)
	{
	case ECDH_VARIANT_MBEDTLS_2_0:
		return(ecdh_read_params_internal(&ctx->ctx.mbed_ecdh,
			buf, end));
	default:
		return ERR_ECP_BAD_INPUT_DATA;
	}
#endif
}

static int ecdh_get_params_internal(ecdh_context_mbed *ctx,
	const ecp_keypair *key,
	ecdh_side side)
{
	int ret;

	/* If it's not our key, just import the public part as Qp */
	if (side == ECDH_THEIRS)
		return(ecp_copy(&ctx->Qp, &key->Q));

	/* Our key: import public (as Q) and private parts */
	if (side != ECDH_OURS)
		return(ERR_ECP_BAD_INPUT_DATA);

	if ((ret = ecp_copy(&ctx->Q, &key->Q)) != 0 ||
		(ret = mpi_copy(&ctx->d, &key->d)) != 0)
		return(ret);

	return(0);
}

/*
* Get parameters from a keypair
*/
int ecdh_get_params(ecdh_context *ctx,
	const ecp_keypair *key,
	ecdh_side side)
{
	int ret;

	if (ecdh_grp_id(ctx) == ECP_DP_NONE)
	{
		/* This is the first call to get_params(). Set up the context
		* for use with the group. */
		if ((ret = ecdh_setup(ctx, key->grp.id)) != 0)
			return(ret);
	}
	else
	{
		/* This is not the first call to get_params(). Check that the
		* current key's group is the same as the context's, which was set
		* from the first key's group. */
		if (ecdh_grp_id(ctx) != key->grp.id)
			return(ERR_ECP_BAD_INPUT_DATA);
	}

#if defined(ECDH_LEGACY_CONTEXT)
	return(ecdh_get_params_internal(ctx, key, side));
#else
	switch (ctx->var)
	{
	case ECDH_VARIANT_MBEDTLS_2_0:
		return(ecdh_get_params_internal(&ctx->ctx.mbed_ecdh,
			key, side));
	default:
		return ERR_ECP_BAD_INPUT_DATA;
	}
#endif
}

static int ecdh_make_public_internal(ecdh_context_mbed *ctx,
	size_t *olen, int point_format,
	unsigned char *buf, size_t blen,
	int(*f_rng)(void *,
	unsigned char *,
	size_t),
	void *p_rng,
	int restart_enabled)
{
	int ret;
#if defined(ECP_RESTARTABLE)
	ecp_restart_ctx *rs_ctx = NULL;
#endif

	if (ctx->grp.pbits == 0)
		return(ERR_ECP_BAD_INPUT_DATA);

#if defined(ECP_RESTARTABLE)
	if (restart_enabled)
		rs_ctx = &ctx->rs;
#else
	(void)restart_enabled;
#endif

#if defined(ECP_RESTARTABLE)
	if ((ret = ecdh_gen_public_restartable(&ctx->grp, &ctx->d, &ctx->Q,
		f_rng, p_rng, rs_ctx)) != 0)
		return(ret);
#else
	if ((ret = ecdh_gen_public(&ctx->grp, &ctx->d, &ctx->Q,
		f_rng, p_rng)) != 0)
		return(ret);
#endif /* MBEDTLS_ECP_RESTARTABLE */

	return ecp_tls_write_point(&ctx->grp, &ctx->Q, point_format, olen, buf, blen);
}


/*
* Setup and export the client public value
*/
int ecdh_make_public(ecdh_context *ctx, size_t *olen,
	unsigned char *buf, size_t blen,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	int restart_enabled = 0;

#if defined(ECP_RESTARTABLE)
	restart_enabled = ctx->restart_enabled;
#endif

#if defined(ECDH_LEGACY_CONTEXT)
	return(ecdh_make_public_internal(ctx, olen, ctx->point_format, buf, blen,
		f_rng, p_rng, restart_enabled));
#else
	switch (ctx->var)
	{
	case ECDH_VARIANT_MBEDTLS_2_0:
		return(ecdh_make_public_internal(&ctx->ctx.mbed_ecdh, olen,
			ctx->point_format, buf, blen,
			f_rng, p_rng,
			restart_enabled));
	default:
		return ERR_ECP_BAD_INPUT_DATA;
	}
#endif
}


static int ecdh_read_public_internal(ecdh_context_mbed *ctx,
	const unsigned char *buf, size_t blen)
{
	int ret;
	const unsigned char *p = buf;

	if ((ret = ecp_tls_read_point(&ctx->grp, &ctx->Qp, &p,
		blen)) != 0)
		return(ret);

	if ((size_t)(p - buf) != blen)
		return(ERR_ECP_BAD_INPUT_DATA);

	return(0);
}

/*
* Parse and import the client's public value
*/
int ecdh_read_public(ecdh_context *ctx,
	const unsigned char *buf, size_t blen)
{
#if defined(ECDH_LEGACY_CONTEXT)
	return(ecdh_read_public_internal(ctx, buf, blen));
#else
	switch (ctx->var)
	{
	case ECDH_VARIANT_MBEDTLS_2_0:
		return(ecdh_read_public_internal(&ctx->ctx.mbed_ecdh,
			buf, blen));
	default:
		return ERR_ECP_BAD_INPUT_DATA;
	}
#endif
}

static int ecdh_calc_secret_internal(ecdh_context_mbed *ctx,
	size_t *olen, unsigned char *buf,
	size_t blen,
	int(*f_rng)(void *,
	unsigned char *,
	size_t),
	void *p_rng,
	int restart_enabled)
{
	int ret;
#if defined(ECP_RESTARTABLE)
	ecp_restart_ctx *rs_ctx = NULL;
#endif

	if (ctx == NULL || ctx->grp.pbits == 0)
		return(ERR_ECP_BAD_INPUT_DATA);

#if defined(ECP_RESTARTABLE)
	if (restart_enabled)
		rs_ctx = &ctx->rs;
#else
	(void)restart_enabled;
#endif

#if defined(ECP_RESTARTABLE)
	if ((ret = ecdh_compute_shared_restartable(&ctx->grp, &ctx->z, &ctx->Qp,
		&ctx->d, f_rng, p_rng,
		rs_ctx)) != 0)
	{
		return(ret);
	}
#else
	if ((ret = ecdh_compute_shared(&ctx->grp, &ctx->z, &ctx->Qp,
		&ctx->d, f_rng, p_rng)) != 0)
	{
		return(ret);
	}
#endif /* MBEDTLS_ECP_RESTARTABLE */

	if (mpi_size(&ctx->z) > blen)
		return(ERR_ECP_BAD_INPUT_DATA);

	*olen = ctx->grp.pbits / 8 + ((ctx->grp.pbits % 8) != 0);
	return mpi_write_binary(&ctx->z, buf, *olen);
}

/*
* Derive and export the shared secret
*/
int ecdh_calc_secret(ecdh_context *ctx, size_t *olen,
	unsigned char *buf, size_t blen,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	int restart_enabled = 0;

#if defined(ECP_RESTARTABLE)
	restart_enabled = ctx->restart_enabled;
#endif

#if defined(ECDH_LEGACY_CONTEXT)
	return(ecdh_calc_secret_internal(ctx, olen, buf, blen, f_rng, p_rng,
		restart_enabled));
#else
	switch (ctx->var)
	{
	case ECDH_VARIANT_MBEDTLS_2_0:
		return(ecdh_calc_secret_internal(&ctx->ctx.mbed_ecdh, olen, buf,
			blen, f_rng, p_rng,
			restart_enabled));
	default:
		return(ERR_ECP_BAD_INPUT_DATA);
	}
#endif
}