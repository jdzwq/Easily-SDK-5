/*
 *  Elliptic curves over GF(p): generic functions
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
 * GECC = Guide to Elliptic Curve Cryptography - Hankerson, Menezes, Vanstone
 * FIPS 186-3 http://csrc.nist.gov/publications/fips/fips186-3/fips_186-3.pdf
 * RFC 4492 for the related TLS structures and constants
 * RFC 7748 for the Curve448 and Curve25519 curve definitions
 *
 * [Curve25519] http://cr.yp.to/ecdh/curve25519-20060209.pdf
 *
 * [2] CORON, Jean-S'ebastien. Resistance against differential power analysis
 *     for elliptic curve cryptosystems. In : Cryptographic Hardware and
 *     Embedded Systems. Springer Berlin Heidelberg, 1999. p. 292-302.
 *     <http://link.springer.com/chapter/10.1007/3-540-48059-5_25>
 *
 * [3] HEDABOU, Mustapha, PINEL, Pierre, et B'EN'ETEAU, Lucien. A comb method to
 *     render ECC resistant against Side Channel Attacks. IACR Cryptology
 *     ePrint Archive, 2004, vol. 2004, p. 342.
 *     <http://eprint.iacr.org/2004/342.pdf>
 */

#include <stddef.h>
#include <string.h>

#include "oemecp.h"

/*
* Initialize (the components of) a point
*/
void ecp_point_init(ecp_point *pt)
{
	mpi_init(&pt->X);
	mpi_init(&pt->Y);
	mpi_init(&pt->Z);
}

/*
* Unallocate (the components of) a point
*/
void ecp_point_free(ecp_point *pt)
{
	if (pt == NULL)
		return;

	mpi_free(&(pt->X));
	mpi_free(&(pt->Y));
	mpi_free(&(pt->Z));
}

/*
* Copy the contents of a point
*/
int ecp_copy(ecp_point *P, const ecp_point *Q)
{
	int ret;

	MPI_CHK(mpi_copy(&P->X, &Q->X));
	MPI_CHK(mpi_copy(&P->Y, &Q->Y));
	MPI_CHK(mpi_copy(&P->Z, &Q->Z));

cleanup:
	return(ret);
}

/*
* Set point to zero
*/
int ecp_set_zero(ecp_point *pt)
{
	int ret;

	MPI_CHK(mpi_lset(&pt->X, 1));
	MPI_CHK(mpi_lset(&pt->Y, 1));
	MPI_CHK(mpi_lset(&pt->Z, 0));

cleanup:
	return(ret);
}

/*
* Tell if a point is zero
*/
int ecp_is_zero(ecp_point *pt)
{
	return(mpi_cmp_int(&pt->Z, 0) == 0);
}

/*
* Compare two points lazily
*/
int ecp_point_cmp(const ecp_point *P, const ecp_point *Q)
{
	if (mpi_cmp_mpi(&P->X, &Q->X) == 0 &&
		mpi_cmp_mpi(&P->Y, &Q->Y) == 0 &&
		mpi_cmp_mpi(&P->Z, &Q->Z) == 0)
	{
		return(0);
	}

	return(-1);
}

/*
* Import a non-zero point from ASCII strings
*/
int ecp_point_read_string(ecp_point *P, int radix, const char *x, const char *y)
{
	int ret;

	MPI_CHK(mpi_read_string(&P->X, radix, x, strlen(x)));
	MPI_CHK(mpi_read_string(&P->Y, radix, y, strlen(y)));
	MPI_CHK(mpi_lset(&P->Z, 1));

cleanup:
	return(ret);
}

/*
* Curve types: internal for now, might be exposed later
*/
typedef enum
{
	ECP_TYPE_NONE = 0,
	ECP_TYPE_SHORT_WEIERSTRASS,    /* y^2 = x^3 + a x + b      */
	ECP_TYPE_MONTGOMERY,           /* y^2 = x^3 + a x^2 + x    */
} ecp_curve_type;

/*
* List of supported curves:
*  - internal ID
*  - TLS NamedCurve ID (RFC 4492 sec. 5.1.1, RFC 7071 sec. 2)
*  - size in bits
*  - readable name
*
* Curves are listed in order: largest curves first, and for a given size,
* fastest curves first. This provides the default order for the SSL module.
*
* Reminder: update profiles in x509_crt.c when adding a new curves!
*/
static const ecp_curve_info ecp_supported_curves[] =
{
	{ ECP_DP_SECP521R1, 25, 521, "secp521r1" },
	{ ECP_DP_BP512R1, 28, 512, "brainpoolP512r1" },
	{ ECP_DP_SECP384R1, 24, 384, "secp384r1" },
	{ ECP_DP_BP384R1, 27, 384, "brainpoolP384r1" },
	{ ECP_DP_SECP256R1, 23, 256, "secp256r1" },
	{ ECP_DP_SECP256K1, 22, 256, "secp256k1" },
	{ ECP_DP_BP256R1, 26, 256, "brainpoolP256r1" },
	{ ECP_DP_SECP224R1, 21, 224, "secp224r1" },
	{ ECP_DP_SECP224K1, 20, 224, "secp224k1" },
	{ ECP_DP_SECP192R1, 19, 192, "secp192r1" },
	{ ECP_DP_SECP192K1, 18, 192, "secp192k1" },
	{ ECP_DP_NONE, 0, 0, NULL },
};

#define ECP_NB_CURVES   (sizeof(ecp_supported_curves) / sizeof(ecp_supported_curves[0]))

static ecp_group_id ecp_supported_grp_id[ECP_NB_CURVES];

/*
* List of supported curves and associated info
*/
const ecp_curve_info *ecp_curve_list(void)
{
	return(ecp_supported_curves);
}

/*
* List of supported curves, group ID only
*/
const ecp_group_id *ecp_grp_id_list(void)
{
	static int init_done = 0;
	size_t i = 0;
	const ecp_curve_info *curve_info;

	if (!init_done)
	{
		for (curve_info = ecp_curve_list(); curve_info->grp_id != ECP_DP_NONE; curve_info++)
		{
			ecp_supported_grp_id[i++] = curve_info->grp_id;
		}
		ecp_supported_grp_id[i] = ECP_DP_NONE;

		init_done = 1;
	}

	return(ecp_supported_grp_id);
}

/*
* Get the curve info for the internal identifier
*/
const ecp_curve_info *ecp_curve_info_from_grp_id(ecp_group_id grp_id)
{
	const ecp_curve_info *curve_info;

	for (curve_info = ecp_curve_list(); curve_info->grp_id != ECP_DP_NONE; curve_info++)
	{
		if (curve_info->grp_id == grp_id)
			return(curve_info);
	}

	return(NULL);
}

/*
* Get the curve info from the TLS identifier
*/
const ecp_curve_info *ecp_curve_info_from_tls_id(uint16_t tls_id)
{
	const ecp_curve_info *curve_info;

	for (curve_info = ecp_curve_list(); curve_info->grp_id != ECP_DP_NONE; curve_info++)
	{
		if (curve_info->tls_id == tls_id)
			return(curve_info);
	}

	return(NULL);
}


/*
* Get the curve info from the name
*/
const ecp_curve_info *ecp_curve_info_from_name(const char *name)
{
	const ecp_curve_info *curve_info;

	if (name == NULL)
		return(NULL);

	for (curve_info = ecp_curve_list(); curve_info->grp_id != ECP_DP_NONE; curve_info++)
	{
		if (strcmp(curve_info->name, name) == 0)
			return(curve_info);
	}

	return(NULL);
}

/*
* Get the type of a curve
*/
static ecp_curve_type ecp_get_type(const ecp_group *grp)
{
	if (grp->G.X.p == NULL)
		return(ECP_TYPE_NONE);

	if (grp->G.Y.p == NULL)
		return(ECP_TYPE_MONTGOMERY);
	else
		return(ECP_TYPE_SHORT_WEIERSTRASS);
}

/*
* Initialize (the components of) a group
*/
void ecp_group_init(ecp_group *grp)
{
	grp->id = ECP_DP_NONE;
	mpi_init(&grp->P);
	mpi_init(&grp->A);
	mpi_init(&grp->B);
	ecp_point_init(&grp->G);
	mpi_init(&grp->N);
	grp->pbits = 0;
	grp->nbits = 0;
	grp->h = 0;
	grp->modp = NULL;
	grp->t_pre = NULL;
	grp->t_post = NULL;
	grp->t_data = NULL;
	grp->T = NULL;
	grp->T_size = 0;
}


/*
* Unallocate (the components of) a group
*/
void ecp_group_free(ecp_group *grp)
{
	size_t i;

	if (grp == NULL)
		return;

	if (grp->h != 1)
	{
		mpi_free(&grp->P);
		mpi_free(&grp->A);
		mpi_free(&grp->B);
		ecp_point_free(&grp->G);
		mpi_free(&grp->N);
	}

	if (grp->T != NULL)
	{
		for (i = 0; i < grp->T_size; i++) 
			ecp_point_free(&grp->T[i]);
		free(grp->T);
	}

	memset(grp, 0, sizeof(ecp_group));
}

/*
* Create an MPI from embedded constants
* (assumes len is an exact multiple of sizeof mpi_uint)
*/
static void ecp_mpi_load(mpi *X, const mpi_uint *p, size_t len)
{
	X->s = 1;
	X->n = len / sizeof(mpi_uint);
	X->p = (mpi_uint *)p;
}

/*
* Set an MPI to static value 1
*/
static void ecp_mpi_set1(mpi *X)
{
	static mpi_uint one[] = { 1 };
	X->s = 1;
	X->n = 1;
	X->p = one;
}

/*
* Make group available from embedded constants
*/
static int _ecp_group_load(ecp_group *grp,
	const mpi_uint *p, size_t plen,
	const mpi_uint *a, size_t alen,
	const mpi_uint *b, size_t blen,
	const mpi_uint *gx, size_t gxlen,
	const mpi_uint *gy, size_t gylen,
	const mpi_uint *n, size_t nlen)
{
	ecp_mpi_load(&grp->P, p, plen);
	if (a != NULL)
		ecp_mpi_load(&grp->A, a, alen);
	ecp_mpi_load(&grp->B, b, blen);
	ecp_mpi_load(&grp->N, n, nlen);

	ecp_mpi_load(&grp->G.X, gx, gxlen);
	ecp_mpi_load(&grp->G.Y, gy, gylen);
	ecp_mpi_set1(&grp->G.Z);

	grp->pbits = mpi_bitlen(&grp->P);
	grp->nbits = mpi_bitlen(&grp->N);

	grp->h = 1;

	return(0);
}

#if defined(_OS_32) || defined(_OS_WINDOWS)

#define BYTES_TO_T_UINT_4( a, b, c, d )                       \
    ( (mpi_uint) (a) <<  0 ) |                        \
    ( (mpi_uint) (b) <<  8 ) |                        \
    ( (mpi_uint) (c) << 16 ) |                        \
    ( (mpi_uint) (d) << 24 )

#define BYTES_TO_T_UINT_2( a, b )                   \
    BYTES_TO_T_UINT_4( a, b, 0, 0 )

#define BYTES_TO_T_UINT_8( a, b, c, d, e, f, g, h ) \
    BYTES_TO_T_UINT_4( a, b, c, d ),                \
    BYTES_TO_T_UINT_4( e, f, g, h )

#else /* 64-bits */

#define BYTES_TO_T_UINT_8( a, b, c, d, e, f, g, h ) \
    ( (mpi_uint) (a) <<  0 ) |                        \
    ( (mpi_uint) (b) <<  8 ) |                        \
    ( (mpi_uint) (c) << 16 ) |                        \
    ( (mpi_uint) (d) << 24 ) |                        \
    ( (mpi_uint) (e) << 32 ) |                        \
    ( (mpi_uint) (f) << 40 ) |                        \
    ( (mpi_uint) (g) << 48 ) |                        \
    ( (mpi_uint) (h) << 56 )

#define BYTES_TO_T_UINT_4( a, b, c, d )             \
    BYTES_TO_T_UINT_8( a, b, c, d, 0, 0, 0, 0 )

#define BYTES_TO_T_UINT_2( a, b )                   \
    BYTES_TO_T_UINT_8( a, b, 0, 0, 0, 0, 0, 0 )

#endif

static const mpi_uint secp192r1_p[] = {
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
};
static const mpi_uint secp192r1_b[] = {
	BYTES_TO_T_UINT_8(0xB1, 0xB9, 0x46, 0xC1, 0xEC, 0xDE, 0xB8, 0xFE),
	BYTES_TO_T_UINT_8(0x49, 0x30, 0x24, 0x72, 0xAB, 0xE9, 0xA7, 0x0F),
	BYTES_TO_T_UINT_8(0xE7, 0x80, 0x9C, 0xE5, 0x19, 0x05, 0x21, 0x64),
};
static const mpi_uint secp192r1_gx[] = {
	BYTES_TO_T_UINT_8(0x12, 0x10, 0xFF, 0x82, 0xFD, 0x0A, 0xFF, 0xF4),
	BYTES_TO_T_UINT_8(0x00, 0x88, 0xA1, 0x43, 0xEB, 0x20, 0xBF, 0x7C),
	BYTES_TO_T_UINT_8(0xF6, 0x90, 0x30, 0xB0, 0x0E, 0xA8, 0x8D, 0x18),
};
static const mpi_uint secp192r1_gy[] = {
	BYTES_TO_T_UINT_8(0x11, 0x48, 0x79, 0x1E, 0xA1, 0x77, 0xF9, 0x73),
	BYTES_TO_T_UINT_8(0xD5, 0xCD, 0x24, 0x6B, 0xED, 0x11, 0x10, 0x63),
	BYTES_TO_T_UINT_8(0x78, 0xDA, 0xC8, 0xFF, 0x95, 0x2B, 0x19, 0x07),
};
static const mpi_uint secp192r1_n[] = {
	BYTES_TO_T_UINT_8(0x31, 0x28, 0xD2, 0xB4, 0xB1, 0xC9, 0x6B, 0x14),
	BYTES_TO_T_UINT_8(0x36, 0xF8, 0xDE, 0x99, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
};

static const mpi_uint secp224r1_p[] = {
	BYTES_TO_T_UINT_8(0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),
	BYTES_TO_T_UINT_8(0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00),
};
static const mpi_uint secp224r1_b[] = {
	BYTES_TO_T_UINT_8(0xB4, 0xFF, 0x55, 0x23, 0x43, 0x39, 0x0B, 0x27),
	BYTES_TO_T_UINT_8(0xBA, 0xD8, 0xBF, 0xD7, 0xB7, 0xB0, 0x44, 0x50),
	BYTES_TO_T_UINT_8(0x56, 0x32, 0x41, 0xF5, 0xAB, 0xB3, 0x04, 0x0C),
	BYTES_TO_T_UINT_4(0x85, 0x0A, 0x05, 0xB4),
};
static const mpi_uint secp224r1_gx[] = {
	BYTES_TO_T_UINT_8(0x21, 0x1D, 0x5C, 0x11, 0xD6, 0x80, 0x32, 0x34),
	BYTES_TO_T_UINT_8(0x22, 0x11, 0xC2, 0x56, 0xD3, 0xC1, 0x03, 0x4A),
	BYTES_TO_T_UINT_8(0xB9, 0x90, 0x13, 0x32, 0x7F, 0xBF, 0xB4, 0x6B),
	BYTES_TO_T_UINT_4(0xBD, 0x0C, 0x0E, 0xB7),
};
static const mpi_uint secp224r1_gy[] = {
	BYTES_TO_T_UINT_8(0x34, 0x7E, 0x00, 0x85, 0x99, 0x81, 0xD5, 0x44),
	BYTES_TO_T_UINT_8(0x64, 0x47, 0x07, 0x5A, 0xA0, 0x75, 0x43, 0xCD),
	BYTES_TO_T_UINT_8(0xE6, 0xDF, 0x22, 0x4C, 0xFB, 0x23, 0xF7, 0xB5),
	BYTES_TO_T_UINT_4(0x88, 0x63, 0x37, 0xBD),
};
static const mpi_uint secp224r1_n[] = {
	BYTES_TO_T_UINT_8(0x3D, 0x2A, 0x5C, 0x5C, 0x45, 0x29, 0xDD, 0x13),
	BYTES_TO_T_UINT_8(0x3E, 0xF0, 0xB8, 0xE0, 0xA2, 0x16, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_4(0xFF, 0xFF, 0xFF, 0xFF),
};


static const mpi_uint secp256r1_p[] = {
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00),
	BYTES_TO_T_UINT_8(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),
	BYTES_TO_T_UINT_8(0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF),
};
static const mpi_uint secp256r1_b[] = {
	BYTES_TO_T_UINT_8(0x4B, 0x60, 0xD2, 0x27, 0x3E, 0x3C, 0xCE, 0x3B),
	BYTES_TO_T_UINT_8(0xF6, 0xB0, 0x53, 0xCC, 0xB0, 0x06, 0x1D, 0x65),
	BYTES_TO_T_UINT_8(0xBC, 0x86, 0x98, 0x76, 0x55, 0xBD, 0xEB, 0xB3),
	BYTES_TO_T_UINT_8(0xE7, 0x93, 0x3A, 0xAA, 0xD8, 0x35, 0xC6, 0x5A),
};
static const mpi_uint secp256r1_gx[] = {
	BYTES_TO_T_UINT_8(0x96, 0xC2, 0x98, 0xD8, 0x45, 0x39, 0xA1, 0xF4),
	BYTES_TO_T_UINT_8(0xA0, 0x33, 0xEB, 0x2D, 0x81, 0x7D, 0x03, 0x77),
	BYTES_TO_T_UINT_8(0xF2, 0x40, 0xA4, 0x63, 0xE5, 0xE6, 0xBC, 0xF8),
	BYTES_TO_T_UINT_8(0x47, 0x42, 0x2C, 0xE1, 0xF2, 0xD1, 0x17, 0x6B),
};
static const mpi_uint secp256r1_gy[] = {
	BYTES_TO_T_UINT_8(0xF5, 0x51, 0xBF, 0x37, 0x68, 0x40, 0xB6, 0xCB),
	BYTES_TO_T_UINT_8(0xCE, 0x5E, 0x31, 0x6B, 0x57, 0x33, 0xCE, 0x2B),
	BYTES_TO_T_UINT_8(0x16, 0x9E, 0x0F, 0x7C, 0x4A, 0xEB, 0xE7, 0x8E),
	BYTES_TO_T_UINT_8(0x9B, 0x7F, 0x1A, 0xFE, 0xE2, 0x42, 0xE3, 0x4F),
};
static const mpi_uint secp256r1_n[] = {
	BYTES_TO_T_UINT_8(0x51, 0x25, 0x63, 0xFC, 0xC2, 0xCA, 0xB9, 0xF3),
	BYTES_TO_T_UINT_8(0x84, 0x9E, 0x17, 0xA7, 0xAD, 0xFA, 0xE6, 0xBC),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF),
};

static const mpi_uint secp384r1_p[] = {
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00),
	BYTES_TO_T_UINT_8(0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
};
static const mpi_uint secp384r1_b[] = {
	BYTES_TO_T_UINT_8(0xEF, 0x2A, 0xEC, 0xD3, 0xED, 0xC8, 0x85, 0x2A),
	BYTES_TO_T_UINT_8(0x9D, 0xD1, 0x2E, 0x8A, 0x8D, 0x39, 0x56, 0xC6),
	BYTES_TO_T_UINT_8(0x5A, 0x87, 0x13, 0x50, 0x8F, 0x08, 0x14, 0x03),
	BYTES_TO_T_UINT_8(0x12, 0x41, 0x81, 0xFE, 0x6E, 0x9C, 0x1D, 0x18),
	BYTES_TO_T_UINT_8(0x19, 0x2D, 0xF8, 0xE3, 0x6B, 0x05, 0x8E, 0x98),
	BYTES_TO_T_UINT_8(0xE4, 0xE7, 0x3E, 0xE2, 0xA7, 0x2F, 0x31, 0xB3),
};
static const mpi_uint secp384r1_gx[] = {
	BYTES_TO_T_UINT_8(0xB7, 0x0A, 0x76, 0x72, 0x38, 0x5E, 0x54, 0x3A),
	BYTES_TO_T_UINT_8(0x6C, 0x29, 0x55, 0xBF, 0x5D, 0xF2, 0x02, 0x55),
	BYTES_TO_T_UINT_8(0x38, 0x2A, 0x54, 0x82, 0xE0, 0x41, 0xF7, 0x59),
	BYTES_TO_T_UINT_8(0x98, 0x9B, 0xA7, 0x8B, 0x62, 0x3B, 0x1D, 0x6E),
	BYTES_TO_T_UINT_8(0x74, 0xAD, 0x20, 0xF3, 0x1E, 0xC7, 0xB1, 0x8E),
	BYTES_TO_T_UINT_8(0x37, 0x05, 0x8B, 0xBE, 0x22, 0xCA, 0x87, 0xAA),
};
static const mpi_uint secp384r1_gy[] = {
	BYTES_TO_T_UINT_8(0x5F, 0x0E, 0xEA, 0x90, 0x7C, 0x1D, 0x43, 0x7A),
	BYTES_TO_T_UINT_8(0x9D, 0x81, 0x7E, 0x1D, 0xCE, 0xB1, 0x60, 0x0A),
	BYTES_TO_T_UINT_8(0xC0, 0xB8, 0xF0, 0xB5, 0x13, 0x31, 0xDA, 0xE9),
	BYTES_TO_T_UINT_8(0x7C, 0x14, 0x9A, 0x28, 0xBD, 0x1D, 0xF4, 0xF8),
	BYTES_TO_T_UINT_8(0x29, 0xDC, 0x92, 0x92, 0xBF, 0x98, 0x9E, 0x5D),
	BYTES_TO_T_UINT_8(0x6F, 0x2C, 0x26, 0x96, 0x4A, 0xDE, 0x17, 0x36),
};
static const mpi_uint secp384r1_n[] = {
	BYTES_TO_T_UINT_8(0x73, 0x29, 0xC5, 0xCC, 0x6A, 0x19, 0xEC, 0xEC),
	BYTES_TO_T_UINT_8(0x7A, 0xA7, 0xB0, 0x48, 0xB2, 0x0D, 0x1A, 0x58),
	BYTES_TO_T_UINT_8(0xDF, 0x2D, 0x37, 0xF4, 0x81, 0x4D, 0x63, 0xC7),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
};

static const mpi_uint secp521r1_p[] = {
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_2(0xFF, 0x01),
};
static const mpi_uint secp521r1_b[] = {
	BYTES_TO_T_UINT_8(0x00, 0x3F, 0x50, 0x6B, 0xD4, 0x1F, 0x45, 0xEF),
	BYTES_TO_T_UINT_8(0xF1, 0x34, 0x2C, 0x3D, 0x88, 0xDF, 0x73, 0x35),
	BYTES_TO_T_UINT_8(0x07, 0xBF, 0xB1, 0x3B, 0xBD, 0xC0, 0x52, 0x16),
	BYTES_TO_T_UINT_8(0x7B, 0x93, 0x7E, 0xEC, 0x51, 0x39, 0x19, 0x56),
	BYTES_TO_T_UINT_8(0xE1, 0x09, 0xF1, 0x8E, 0x91, 0x89, 0xB4, 0xB8),
	BYTES_TO_T_UINT_8(0xF3, 0x15, 0xB3, 0x99, 0x5B, 0x72, 0xDA, 0xA2),
	BYTES_TO_T_UINT_8(0xEE, 0x40, 0x85, 0xB6, 0xA0, 0x21, 0x9A, 0x92),
	BYTES_TO_T_UINT_8(0x1F, 0x9A, 0x1C, 0x8E, 0x61, 0xB9, 0x3E, 0x95),
	BYTES_TO_T_UINT_2(0x51, 0x00),
};
static const mpi_uint secp521r1_gx[] = {
	BYTES_TO_T_UINT_8(0x66, 0xBD, 0xE5, 0xC2, 0x31, 0x7E, 0x7E, 0xF9),
	BYTES_TO_T_UINT_8(0x9B, 0x42, 0x6A, 0x85, 0xC1, 0xB3, 0x48, 0x33),
	BYTES_TO_T_UINT_8(0xDE, 0xA8, 0xFF, 0xA2, 0x27, 0xC1, 0x1D, 0xFE),
	BYTES_TO_T_UINT_8(0x28, 0x59, 0xE7, 0xEF, 0x77, 0x5E, 0x4B, 0xA1),
	BYTES_TO_T_UINT_8(0xBA, 0x3D, 0x4D, 0x6B, 0x60, 0xAF, 0x28, 0xF8),
	BYTES_TO_T_UINT_8(0x21, 0xB5, 0x3F, 0x05, 0x39, 0x81, 0x64, 0x9C),
	BYTES_TO_T_UINT_8(0x42, 0xB4, 0x95, 0x23, 0x66, 0xCB, 0x3E, 0x9E),
	BYTES_TO_T_UINT_8(0xCD, 0xE9, 0x04, 0x04, 0xB7, 0x06, 0x8E, 0x85),
	BYTES_TO_T_UINT_2(0xC6, 0x00),
};
static const mpi_uint secp521r1_gy[] = {
	BYTES_TO_T_UINT_8(0x50, 0x66, 0xD1, 0x9F, 0x76, 0x94, 0xBE, 0x88),
	BYTES_TO_T_UINT_8(0x40, 0xC2, 0x72, 0xA2, 0x86, 0x70, 0x3C, 0x35),
	BYTES_TO_T_UINT_8(0x61, 0x07, 0xAD, 0x3F, 0x01, 0xB9, 0x50, 0xC5),
	BYTES_TO_T_UINT_8(0x40, 0x26, 0xF4, 0x5E, 0x99, 0x72, 0xEE, 0x97),
	BYTES_TO_T_UINT_8(0x2C, 0x66, 0x3E, 0x27, 0x17, 0xBD, 0xAF, 0x17),
	BYTES_TO_T_UINT_8(0x68, 0x44, 0x9B, 0x57, 0x49, 0x44, 0xF5, 0x98),
	BYTES_TO_T_UINT_8(0xD9, 0x1B, 0x7D, 0x2C, 0xB4, 0x5F, 0x8A, 0x5C),
	BYTES_TO_T_UINT_8(0x04, 0xC0, 0x3B, 0x9A, 0x78, 0x6A, 0x29, 0x39),
	BYTES_TO_T_UINT_2(0x18, 0x01),
};
static const mpi_uint secp521r1_n[] = {
	BYTES_TO_T_UINT_8(0x09, 0x64, 0x38, 0x91, 0x1E, 0xB7, 0x6F, 0xBB),
	BYTES_TO_T_UINT_8(0xAE, 0x47, 0x9C, 0x89, 0xB8, 0xC9, 0xB5, 0x3B),
	BYTES_TO_T_UINT_8(0xD0, 0xA5, 0x09, 0xF7, 0x48, 0x01, 0xCC, 0x7F),
	BYTES_TO_T_UINT_8(0x6B, 0x96, 0x2F, 0xBF, 0x83, 0x87, 0x86, 0x51),
	BYTES_TO_T_UINT_8(0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_2(0xFF, 0x01),
};

static const mpi_uint secp192k1_p[] = {
	BYTES_TO_T_UINT_8(0x37, 0xEE, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
};
static const mpi_uint secp192k1_a[] = {
	BYTES_TO_T_UINT_2(0x00, 0x00),
};
static const mpi_uint secp192k1_b[] = {
	BYTES_TO_T_UINT_2(0x03, 0x00),
};
static const mpi_uint secp192k1_gx[] = {
	BYTES_TO_T_UINT_8(0x7D, 0x6C, 0xE0, 0xEA, 0xB1, 0xD1, 0xA5, 0x1D),
	BYTES_TO_T_UINT_8(0x34, 0xF4, 0xB7, 0x80, 0x02, 0x7D, 0xB0, 0x26),
	BYTES_TO_T_UINT_8(0xAE, 0xE9, 0x57, 0xC0, 0x0E, 0xF1, 0x4F, 0xDB),
};
static const mpi_uint secp192k1_gy[] = {
	BYTES_TO_T_UINT_8(0x9D, 0x2F, 0x5E, 0xD9, 0x88, 0xAA, 0x82, 0x40),
	BYTES_TO_T_UINT_8(0x34, 0x86, 0xBE, 0x15, 0xD0, 0x63, 0x41, 0x84),
	BYTES_TO_T_UINT_8(0xA7, 0x28, 0x56, 0x9C, 0x6D, 0x2F, 0x2F, 0x9B),
};
static const mpi_uint secp192k1_n[] = {
	BYTES_TO_T_UINT_8(0x8D, 0xFD, 0xDE, 0x74, 0x6A, 0x46, 0x69, 0x0F),
	BYTES_TO_T_UINT_8(0x17, 0xFC, 0xF2, 0x26, 0xFE, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
};

static const mpi_uint secp224k1_p[] = {
	BYTES_TO_T_UINT_8(0x6D, 0xE5, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_4(0xFF, 0xFF, 0xFF, 0xFF),
};
static const mpi_uint secp224k1_a[] = {
	BYTES_TO_T_UINT_2(0x00, 0x00),
};
static const mpi_uint secp224k1_b[] = {
	BYTES_TO_T_UINT_2(0x05, 0x00),
};
static const mpi_uint secp224k1_gx[] = {
	BYTES_TO_T_UINT_8(0x5C, 0xA4, 0xB7, 0xB6, 0x0E, 0x65, 0x7E, 0x0F),
	BYTES_TO_T_UINT_8(0xA9, 0x75, 0x70, 0xE4, 0xE9, 0x67, 0xA4, 0x69),
	BYTES_TO_T_UINT_8(0xA1, 0x28, 0xFC, 0x30, 0xDF, 0x99, 0xF0, 0x4D),
	BYTES_TO_T_UINT_4(0x33, 0x5B, 0x45, 0xA1),
};
static const mpi_uint secp224k1_gy[] = {
	BYTES_TO_T_UINT_8(0xA5, 0x61, 0x6D, 0x55, 0xDB, 0x4B, 0xCA, 0xE2),
	BYTES_TO_T_UINT_8(0x59, 0xBD, 0xB0, 0xC0, 0xF7, 0x19, 0xE3, 0xF7),
	BYTES_TO_T_UINT_8(0xD6, 0xFB, 0xCA, 0x82, 0x42, 0x34, 0xBA, 0x7F),
	BYTES_TO_T_UINT_4(0xED, 0x9F, 0x08, 0x7E),
};
static const mpi_uint secp224k1_n[] = {
	BYTES_TO_T_UINT_8(0xF7, 0xB1, 0x9F, 0x76, 0x71, 0xA9, 0xF0, 0xCA),
	BYTES_TO_T_UINT_8(0x84, 0x61, 0xEC, 0xD2, 0xE8, 0xDC, 0x01, 0x00),
	BYTES_TO_T_UINT_8(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),
	BYTES_TO_T_UINT_8(0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00),
};

static const mpi_uint secp256k1_p[] = {
	BYTES_TO_T_UINT_8(0x2F, 0xFC, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
};
static const mpi_uint secp256k1_a[] = {
	BYTES_TO_T_UINT_2(0x00, 0x00),
};
static const mpi_uint secp256k1_b[] = {
	BYTES_TO_T_UINT_2(0x07, 0x00),
};
static const mpi_uint secp256k1_gx[] = {
	BYTES_TO_T_UINT_8(0x98, 0x17, 0xF8, 0x16, 0x5B, 0x81, 0xF2, 0x59),
	BYTES_TO_T_UINT_8(0xD9, 0x28, 0xCE, 0x2D, 0xDB, 0xFC, 0x9B, 0x02),
	BYTES_TO_T_UINT_8(0x07, 0x0B, 0x87, 0xCE, 0x95, 0x62, 0xA0, 0x55),
	BYTES_TO_T_UINT_8(0xAC, 0xBB, 0xDC, 0xF9, 0x7E, 0x66, 0xBE, 0x79),
};
static const mpi_uint secp256k1_gy[] = {
	BYTES_TO_T_UINT_8(0xB8, 0xD4, 0x10, 0xFB, 0x8F, 0xD0, 0x47, 0x9C),
	BYTES_TO_T_UINT_8(0x19, 0x54, 0x85, 0xA6, 0x48, 0xB4, 0x17, 0xFD),
	BYTES_TO_T_UINT_8(0xA8, 0x08, 0x11, 0x0E, 0xFC, 0xFB, 0xA4, 0x5D),
	BYTES_TO_T_UINT_8(0x65, 0xC4, 0xA3, 0x26, 0x77, 0xDA, 0x3A, 0x48),
};
static const mpi_uint secp256k1_n[] = {
	BYTES_TO_T_UINT_8(0x41, 0x41, 0x36, 0xD0, 0x8C, 0x5E, 0xD2, 0xBF),
	BYTES_TO_T_UINT_8(0x3B, 0xA0, 0x48, 0xAF, 0xE6, 0xDC, 0xAE, 0xBA),
	BYTES_TO_T_UINT_8(0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
	BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
};

static const mpi_uint brainpoolP256r1_p[] = {
	BYTES_TO_T_UINT_8(0x77, 0x53, 0x6E, 0x1F, 0x1D, 0x48, 0x13, 0x20),
	BYTES_TO_T_UINT_8(0x28, 0x20, 0x26, 0xD5, 0x23, 0xF6, 0x3B, 0x6E),
	BYTES_TO_T_UINT_8(0x72, 0x8D, 0x83, 0x9D, 0x90, 0x0A, 0x66, 0x3E),
	BYTES_TO_T_UINT_8(0xBC, 0xA9, 0xEE, 0xA1, 0xDB, 0x57, 0xFB, 0xA9),
};
static const mpi_uint brainpoolP256r1_a[] = {
	BYTES_TO_T_UINT_8(0xD9, 0xB5, 0x30, 0xF3, 0x44, 0x4B, 0x4A, 0xE9),
	BYTES_TO_T_UINT_8(0x6C, 0x5C, 0xDC, 0x26, 0xC1, 0x55, 0x80, 0xFB),
	BYTES_TO_T_UINT_8(0xE7, 0xFF, 0x7A, 0x41, 0x30, 0x75, 0xF6, 0xEE),
	BYTES_TO_T_UINT_8(0x57, 0x30, 0x2C, 0xFC, 0x75, 0x09, 0x5A, 0x7D),
};
static const mpi_uint brainpoolP256r1_b[] = {
	BYTES_TO_T_UINT_8(0xB6, 0x07, 0x8C, 0xFF, 0x18, 0xDC, 0xCC, 0x6B),
	BYTES_TO_T_UINT_8(0xCE, 0xE1, 0xF7, 0x5C, 0x29, 0x16, 0x84, 0x95),
	BYTES_TO_T_UINT_8(0xBF, 0x7C, 0xD7, 0xBB, 0xD9, 0xB5, 0x30, 0xF3),
	BYTES_TO_T_UINT_8(0x44, 0x4B, 0x4A, 0xE9, 0x6C, 0x5C, 0xDC, 0x26),
};
static const mpi_uint brainpoolP256r1_gx[] = {
	BYTES_TO_T_UINT_8(0x62, 0x32, 0xCE, 0x9A, 0xBD, 0x53, 0x44, 0x3A),
	BYTES_TO_T_UINT_8(0xC2, 0x23, 0xBD, 0xE3, 0xE1, 0x27, 0xDE, 0xB9),
	BYTES_TO_T_UINT_8(0xAF, 0xB7, 0x81, 0xFC, 0x2F, 0x48, 0x4B, 0x2C),
	BYTES_TO_T_UINT_8(0xCB, 0x57, 0x7E, 0xCB, 0xB9, 0xAE, 0xD2, 0x8B),
};
static const mpi_uint brainpoolP256r1_gy[] = {
	BYTES_TO_T_UINT_8(0x97, 0x69, 0x04, 0x2F, 0xC7, 0x54, 0x1D, 0x5C),
	BYTES_TO_T_UINT_8(0x54, 0x8E, 0xED, 0x2D, 0x13, 0x45, 0x77, 0xC2),
	BYTES_TO_T_UINT_8(0xC9, 0x1D, 0x61, 0x14, 0x1A, 0x46, 0xF8, 0x97),
	BYTES_TO_T_UINT_8(0xFD, 0xC4, 0xDA, 0xC3, 0x35, 0xF8, 0x7E, 0x54),
};
static const mpi_uint brainpoolP256r1_n[] = {
	BYTES_TO_T_UINT_8(0xA7, 0x56, 0x48, 0x97, 0x82, 0x0E, 0x1E, 0x90),
	BYTES_TO_T_UINT_8(0xF7, 0xA6, 0x61, 0xB5, 0xA3, 0x7A, 0x39, 0x8C),
	BYTES_TO_T_UINT_8(0x71, 0x8D, 0x83, 0x9D, 0x90, 0x0A, 0x66, 0x3E),
	BYTES_TO_T_UINT_8(0xBC, 0xA9, 0xEE, 0xA1, 0xDB, 0x57, 0xFB, 0xA9),
};

static const mpi_uint brainpoolP384r1_p[] = {
	BYTES_TO_T_UINT_8(0x53, 0xEC, 0x07, 0x31, 0x13, 0x00, 0x47, 0x87),
	BYTES_TO_T_UINT_8(0x71, 0x1A, 0x1D, 0x90, 0x29, 0xA7, 0xD3, 0xAC),
	BYTES_TO_T_UINT_8(0x23, 0x11, 0xB7, 0x7F, 0x19, 0xDA, 0xB1, 0x12),
	BYTES_TO_T_UINT_8(0xB4, 0x56, 0x54, 0xED, 0x09, 0x71, 0x2F, 0x15),
	BYTES_TO_T_UINT_8(0xDF, 0x41, 0xE6, 0x50, 0x7E, 0x6F, 0x5D, 0x0F),
	BYTES_TO_T_UINT_8(0x28, 0x6D, 0x38, 0xA3, 0x82, 0x1E, 0xB9, 0x8C),
};
static const mpi_uint brainpoolP384r1_a[] = {
	BYTES_TO_T_UINT_8(0x26, 0x28, 0xCE, 0x22, 0xDD, 0xC7, 0xA8, 0x04),
	BYTES_TO_T_UINT_8(0xEB, 0xD4, 0x3A, 0x50, 0x4A, 0x81, 0xA5, 0x8A),
	BYTES_TO_T_UINT_8(0x0F, 0xF9, 0x91, 0xBA, 0xEF, 0x65, 0x91, 0x13),
	BYTES_TO_T_UINT_8(0x87, 0x27, 0xB2, 0x4F, 0x8E, 0xA2, 0xBE, 0xC2),
	BYTES_TO_T_UINT_8(0xA0, 0xAF, 0x05, 0xCE, 0x0A, 0x08, 0x72, 0x3C),
	BYTES_TO_T_UINT_8(0x0C, 0x15, 0x8C, 0x3D, 0xC6, 0x82, 0xC3, 0x7B),
};
static const mpi_uint brainpoolP384r1_b[] = {
	BYTES_TO_T_UINT_8(0x11, 0x4C, 0x50, 0xFA, 0x96, 0x86, 0xB7, 0x3A),
	BYTES_TO_T_UINT_8(0x94, 0xC9, 0xDB, 0x95, 0x02, 0x39, 0xB4, 0x7C),
	BYTES_TO_T_UINT_8(0xD5, 0x62, 0xEB, 0x3E, 0xA5, 0x0E, 0x88, 0x2E),
	BYTES_TO_T_UINT_8(0xA6, 0xD2, 0xDC, 0x07, 0xE1, 0x7D, 0xB7, 0x2F),
	BYTES_TO_T_UINT_8(0x7C, 0x44, 0xF0, 0x16, 0x54, 0xB5, 0x39, 0x8B),
	BYTES_TO_T_UINT_8(0x26, 0x28, 0xCE, 0x22, 0xDD, 0xC7, 0xA8, 0x04),
};
static const mpi_uint brainpoolP384r1_gx[] = {
	BYTES_TO_T_UINT_8(0x1E, 0xAF, 0xD4, 0x47, 0xE2, 0xB2, 0x87, 0xEF),
	BYTES_TO_T_UINT_8(0xAA, 0x46, 0xD6, 0x36, 0x34, 0xE0, 0x26, 0xE8),
	BYTES_TO_T_UINT_8(0xE8, 0x10, 0xBD, 0x0C, 0xFE, 0xCA, 0x7F, 0xDB),
	BYTES_TO_T_UINT_8(0xE3, 0x4F, 0xF1, 0x7E, 0xE7, 0xA3, 0x47, 0x88),
	BYTES_TO_T_UINT_8(0x6B, 0x3F, 0xC1, 0xB7, 0x81, 0x3A, 0xA6, 0xA2),
	BYTES_TO_T_UINT_8(0xFF, 0x45, 0xCF, 0x68, 0xF0, 0x64, 0x1C, 0x1D),
};
static const mpi_uint brainpoolP384r1_gy[] = {
	BYTES_TO_T_UINT_8(0x15, 0x53, 0x3C, 0x26, 0x41, 0x03, 0x82, 0x42),
	BYTES_TO_T_UINT_8(0x11, 0x81, 0x91, 0x77, 0x21, 0x46, 0x46, 0x0E),
	BYTES_TO_T_UINT_8(0x28, 0x29, 0x91, 0xF9, 0x4F, 0x05, 0x9C, 0xE1),
	BYTES_TO_T_UINT_8(0x64, 0x58, 0xEC, 0xFE, 0x29, 0x0B, 0xB7, 0x62),
	BYTES_TO_T_UINT_8(0x52, 0xD5, 0xCF, 0x95, 0x8E, 0xEB, 0xB1, 0x5C),
	BYTES_TO_T_UINT_8(0xA4, 0xC2, 0xF9, 0x20, 0x75, 0x1D, 0xBE, 0x8A),
};
static const mpi_uint brainpoolP384r1_n[] = {
	BYTES_TO_T_UINT_8(0x65, 0x65, 0x04, 0xE9, 0x02, 0x32, 0x88, 0x3B),
	BYTES_TO_T_UINT_8(0x10, 0xC3, 0x7F, 0x6B, 0xAF, 0xB6, 0x3A, 0xCF),
	BYTES_TO_T_UINT_8(0xA7, 0x25, 0x04, 0xAC, 0x6C, 0x6E, 0x16, 0x1F),
	BYTES_TO_T_UINT_8(0xB3, 0x56, 0x54, 0xED, 0x09, 0x71, 0x2F, 0x15),
	BYTES_TO_T_UINT_8(0xDF, 0x41, 0xE6, 0x50, 0x7E, 0x6F, 0x5D, 0x0F),
	BYTES_TO_T_UINT_8(0x28, 0x6D, 0x38, 0xA3, 0x82, 0x1E, 0xB9, 0x8C),
};

static const mpi_uint brainpoolP512r1_p[] = {
	BYTES_TO_T_UINT_8(0xF3, 0x48, 0x3A, 0x58, 0x56, 0x60, 0xAA, 0x28),
	BYTES_TO_T_UINT_8(0x85, 0xC6, 0x82, 0x2D, 0x2F, 0xFF, 0x81, 0x28),
	BYTES_TO_T_UINT_8(0xE6, 0x80, 0xA3, 0xE6, 0x2A, 0xA1, 0xCD, 0xAE),
	BYTES_TO_T_UINT_8(0x42, 0x68, 0xC6, 0x9B, 0x00, 0x9B, 0x4D, 0x7D),
	BYTES_TO_T_UINT_8(0x71, 0x08, 0x33, 0x70, 0xCA, 0x9C, 0x63, 0xD6),
	BYTES_TO_T_UINT_8(0x0E, 0xD2, 0xC9, 0xB3, 0xB3, 0x8D, 0x30, 0xCB),
	BYTES_TO_T_UINT_8(0x07, 0xFC, 0xC9, 0x33, 0xAE, 0xE6, 0xD4, 0x3F),
	BYTES_TO_T_UINT_8(0x8B, 0xC4, 0xE9, 0xDB, 0xB8, 0x9D, 0xDD, 0xAA),
};
static const mpi_uint brainpoolP512r1_a[] = {
	BYTES_TO_T_UINT_8(0xCA, 0x94, 0xFC, 0x77, 0x4D, 0xAC, 0xC1, 0xE7),
	BYTES_TO_T_UINT_8(0xB9, 0xC7, 0xF2, 0x2B, 0xA7, 0x17, 0x11, 0x7F),
	BYTES_TO_T_UINT_8(0xB5, 0xC8, 0x9A, 0x8B, 0xC9, 0xF1, 0x2E, 0x0A),
	BYTES_TO_T_UINT_8(0xA1, 0x3A, 0x25, 0xA8, 0x5A, 0x5D, 0xED, 0x2D),
	BYTES_TO_T_UINT_8(0xBC, 0x63, 0x98, 0xEA, 0xCA, 0x41, 0x34, 0xA8),
	BYTES_TO_T_UINT_8(0x10, 0x16, 0xF9, 0x3D, 0x8D, 0xDD, 0xCB, 0x94),
	BYTES_TO_T_UINT_8(0xC5, 0x4C, 0x23, 0xAC, 0x45, 0x71, 0x32, 0xE2),
	BYTES_TO_T_UINT_8(0x89, 0x3B, 0x60, 0x8B, 0x31, 0xA3, 0x30, 0x78),
};
static const mpi_uint brainpoolP512r1_b[] = {
	BYTES_TO_T_UINT_8(0x23, 0xF7, 0x16, 0x80, 0x63, 0xBD, 0x09, 0x28),
	BYTES_TO_T_UINT_8(0xDD, 0xE5, 0xBA, 0x5E, 0xB7, 0x50, 0x40, 0x98),
	BYTES_TO_T_UINT_8(0x67, 0x3E, 0x08, 0xDC, 0xCA, 0x94, 0xFC, 0x77),
	BYTES_TO_T_UINT_8(0x4D, 0xAC, 0xC1, 0xE7, 0xB9, 0xC7, 0xF2, 0x2B),
	BYTES_TO_T_UINT_8(0xA7, 0x17, 0x11, 0x7F, 0xB5, 0xC8, 0x9A, 0x8B),
	BYTES_TO_T_UINT_8(0xC9, 0xF1, 0x2E, 0x0A, 0xA1, 0x3A, 0x25, 0xA8),
	BYTES_TO_T_UINT_8(0x5A, 0x5D, 0xED, 0x2D, 0xBC, 0x63, 0x98, 0xEA),
	BYTES_TO_T_UINT_8(0xCA, 0x41, 0x34, 0xA8, 0x10, 0x16, 0xF9, 0x3D),
};
static const mpi_uint brainpoolP512r1_gx[] = {
	BYTES_TO_T_UINT_8(0x22, 0xF8, 0xB9, 0xBC, 0x09, 0x22, 0x35, 0x8B),
	BYTES_TO_T_UINT_8(0x68, 0x5E, 0x6A, 0x40, 0x47, 0x50, 0x6D, 0x7C),
	BYTES_TO_T_UINT_8(0x5F, 0x7D, 0xB9, 0x93, 0x7B, 0x68, 0xD1, 0x50),
	BYTES_TO_T_UINT_8(0x8D, 0xD4, 0xD0, 0xE2, 0x78, 0x1F, 0x3B, 0xFF),
	BYTES_TO_T_UINT_8(0x8E, 0x09, 0xD0, 0xF4, 0xEE, 0x62, 0x3B, 0xB4),
	BYTES_TO_T_UINT_8(0xC1, 0x16, 0xD9, 0xB5, 0x70, 0x9F, 0xED, 0x85),
	BYTES_TO_T_UINT_8(0x93, 0x6A, 0x4C, 0x9C, 0x2E, 0x32, 0x21, 0x5A),
	BYTES_TO_T_UINT_8(0x64, 0xD9, 0x2E, 0xD8, 0xBD, 0xE4, 0xAE, 0x81),
};
static const mpi_uint brainpoolP512r1_gy[] = {
	BYTES_TO_T_UINT_8(0x92, 0x08, 0xD8, 0x3A, 0x0F, 0x1E, 0xCD, 0x78),
	BYTES_TO_T_UINT_8(0x06, 0x54, 0xF0, 0xA8, 0x2F, 0x2B, 0xCA, 0xD1),
	BYTES_TO_T_UINT_8(0xAE, 0x63, 0x27, 0x8A, 0xD8, 0x4B, 0xCA, 0x5B),
	BYTES_TO_T_UINT_8(0x5E, 0x48, 0x5F, 0x4A, 0x49, 0xDE, 0xDC, 0xB2),
	BYTES_TO_T_UINT_8(0x11, 0x81, 0x1F, 0x88, 0x5B, 0xC5, 0x00, 0xA0),
	BYTES_TO_T_UINT_8(0x1A, 0x7B, 0xA5, 0x24, 0x00, 0xF7, 0x09, 0xF2),
	BYTES_TO_T_UINT_8(0xFD, 0x22, 0x78, 0xCF, 0xA9, 0xBF, 0xEA, 0xC0),
	BYTES_TO_T_UINT_8(0xEC, 0x32, 0x63, 0x56, 0x5D, 0x38, 0xDE, 0x7D),
};
static const mpi_uint brainpoolP512r1_n[] = {
	BYTES_TO_T_UINT_8(0x69, 0x00, 0xA9, 0x9C, 0x82, 0x96, 0x87, 0xB5),
	BYTES_TO_T_UINT_8(0xDD, 0xDA, 0x5D, 0x08, 0x81, 0xD3, 0xB1, 0x1D),
	BYTES_TO_T_UINT_8(0x47, 0x10, 0xAC, 0x7F, 0x19, 0x61, 0x86, 0x41),
	BYTES_TO_T_UINT_8(0x19, 0x26, 0xA9, 0x4C, 0x41, 0x5C, 0x3E, 0x55),
	BYTES_TO_T_UINT_8(0x70, 0x08, 0x33, 0x70, 0xCA, 0x9C, 0x63, 0xD6),
	BYTES_TO_T_UINT_8(0x0E, 0xD2, 0xC9, 0xB3, 0xB3, 0x8D, 0x30, 0xCB),
	BYTES_TO_T_UINT_8(0x07, 0xFC, 0xC9, 0x33, 0xAE, 0xE6, 0xD4, 0x3F),
	BYTES_TO_T_UINT_8(0x8B, 0xC4, 0xE9, 0xDB, 0xB8, 0x9D, 0xDD, 0xAA),
};



static int ecp_use_curve25519(ecp_group *grp)
{
	int ret;

	/* Actually ( A + 2 ) / 4 */
	MPI_CHK(mpi_read_string(&grp->A, 16, "01DB42", 6));

	/* P = 2^255 - 19 */
	MPI_CHK(mpi_lset(&grp->P, 1));
	MPI_CHK(mpi_shift_l(&grp->P, 255));
	MPI_CHK(mpi_sub_int(&grp->P, &grp->P, 19));
	grp->pbits = mpi_bitlen(&grp->P);

	/* N = 2^252 + 27742317777372353535851937790883648493 */
	MPI_CHK(mpi_read_string(&grp->N, 16, "14DEF9DEA2F79CD65812631A5CF5D3ED", 32));
	MPI_CHK(mpi_set_bit(&grp->N, 252, 1));

	/* Y intentionally not set, since we use x/z coordinates.
	* This is used as a marker to identify Montgomery curves! */
	MPI_CHK(mpi_lset(&grp->G.X, 9));
	MPI_CHK(mpi_lset(&grp->G.Z, 1));
	mpi_free(&grp->G.Y);

	/* Actually, the required msb for private keys */
	grp->nbits = 254;

cleanup:
	if (ret != 0)
		ecp_group_free(grp);

	return(ret);
}

/*
* Specialized function for creating the Curve448 group
*/
static int ecp_use_curve448(ecp_group *grp)
{
	mpi Ns;
	int ret;

	mpi_init(&Ns);

	/* Actually ( A + 2 ) / 4 */
	MPI_CHK(mpi_read_string(&grp->A, 16, "98AA", 4));

	/* P = 2^448 - 2^224 - 1 */
	MPI_CHK(mpi_lset(&grp->P, 1));
	MPI_CHK(mpi_shift_l(&grp->P, 224));
	MPI_CHK(mpi_sub_int(&grp->P, &grp->P, 1));
	MPI_CHK(mpi_shift_l(&grp->P, 224));
	MPI_CHK(mpi_sub_int(&grp->P, &grp->P, 1));
	grp->pbits = mpi_bitlen(&grp->P);

	/* Y intentionally not set, since we use x/z coordinates.
	* This is used as a marker to identify Montgomery curves! */
	MPI_CHK(mpi_lset(&grp->G.X, 5));
	MPI_CHK(mpi_lset(&grp->G.Z, 1));
	mpi_free(&grp->G.Y);

	/* N = 2^446 - 13818066809895115352007386748515426880336692474882178609894547503885 */
	MPI_CHK(mpi_set_bit(&grp->N, 446, 1));
	MPI_CHK(mpi_read_string(&Ns, 16, "8335DC163BB124B65129C96FDE933D8D723A70AADC873D6D54A7BB0D", 56));
	MPI_CHK(mpi_sub_mpi(&grp->N, &grp->N, &Ns));

	/* Actually, the required msb for private keys */
	grp->nbits = 447;

cleanup:
	mpi_free(&Ns);
	if (ret != 0)
		ecp_group_free(grp);

	return(ret);
}


#define NIST_MODP( P )      grp->modp = ecp_mod_ ## P;

#define LOAD_GROUP_A( G )   _ecp_group_load( grp,            \
                            G ## _p,  sizeof( G ## _p  ),   \
                            G ## _a,  sizeof( G ## _a  ),   \
                            G ## _b,  sizeof( G ## _b  ),   \
                            G ## _gx, sizeof( G ## _gx ),   \
                            G ## _gy, sizeof( G ## _gy ),   \
                            G ## _n,  sizeof( G ## _n  ) )

#define LOAD_GROUP( G )     _ecp_group_load( grp,            \
                            G ## _p,  sizeof( G ## _p  ),   \
                            NULL,     0,                    \
                            G ## _b,  sizeof( G ## _b  ),   \
                            G ## _gx, sizeof( G ## _gx ),   \
                            G ## _gy, sizeof( G ## _gy ),   \
                            G ## _n,  sizeof( G ## _n  ) )

/* Add 64-bit chunks (dst += src) and update carry */
static void add64(mpi_uint *dst, mpi_uint *src, mpi_uint *carry)
{
	unsigned char i;
	mpi_uint c = 0;
	for (i = 0; i < 8 / sizeof(mpi_uint); i++, dst++, src++)
	{
		*dst += c;      c = (*dst < c);
		*dst += *src;   c += (*dst < *src);
	}
	*carry += c;
}

/* Add carry to a 64-bit chunk and update carry */
static void carry64(mpi_uint *dst, mpi_uint *carry)
{
	unsigned char i;
	for (i = 0; i < 8 / sizeof(mpi_uint); i++, dst++)
	{
		*dst += *carry;
		*carry = (*dst < *carry);
	}
}

#define WIDTH       8 / sizeof( mpi_uint )
#define A( i )      N->p + (i) * WIDTH
#define ADD( i )    add64( p, A( i ), &c )
#define NEXT        p += WIDTH; carry64( p, &c )
#define LAST        p += WIDTH; *p = c; while( ++p < end ) *p = 0

/*
* Fast quasi-reduction modulo p192 (FIPS 186-3 D.2.1)
*/
static int ecp_mod_p192(mpi *N)
{
	int ret;
	mpi_uint c = 0;
	mpi_uint *p, *end;

	/* Make sure we have enough blocks so that A(5) is legal */
	MPI_CHK(mpi_grow(N, 6 * WIDTH));

	p = N->p;
	end = p + N->n;

	ADD(3); ADD(5);             NEXT; // A0 += A3 + A5
	ADD(3); ADD(4); ADD(5);   NEXT; // A1 += A3 + A4 + A5
	ADD(4); ADD(5);             LAST; // A2 += A4 + A5

cleanup:
	return(ret);
}

#undef WIDTH
#undef A
#undef ADD
#undef NEXT
#undef LAST

#define LOAD32      cur = A( i );

#if defined(_OS_32) || defined(_OS_WINDOWS)  /* 32 bit */

#define MAX32       N->n
#define A( j )      N->p[j]
#define STORE32     N->p[i] = cur;

#else                               /* 64-bit */

#define MAX32       N->n * 2
#define A( j ) (j) % 2 ? (uint32_t)( N->p[(j)/2] >> 32 ) : \
                         (uint32_t)( N->p[(j)/2] )
#define STORE32                                   \
    if( i % 2 ) {                                 \
        N->p[i/2] &= 0x00000000FFFFFFFF;          \
        N->p[i/2] |= ((mpi_uint) cur) << 32;        \
	    } else {                                      \
        N->p[i/2] &= 0xFFFFFFFF00000000;          \
        N->p[i/2] |= (mpi_uint) cur;                \
	    }

#endif

/*
* Helpers for addition and subtraction of chunks, with signed carry.
*/
static void add32(uint32_t *dst, uint32_t src, signed char *carry)
{
	*dst += src;
	*carry += (*dst < src);
}

static void sub32(uint32_t *dst, uint32_t src, signed char *carry)
{
	*carry -= (*dst < src);
	*dst -= src;
}

#define ADD( j )    add32( &cur, A( j ), &c );
#define SUB( j )    sub32( &cur, A( j ), &c );

/*
* If the result is negative, we get it in the form
* c * 2^(bits + 32) + N, with c negative and N positive shorter than 'bits'
*/
static int fix_negative(mpi *N, signed char c, mpi *C, size_t bits)
{
	int ret;

	/* C = - c * 2^(bits + 32) */
#if !defined(_OS_64)
	((void)bits);
#else
	if (bits == 224)
		C->p[C->n - 1] = ((mpi_uint)-c) << 32;
	else
#endif
		C->p[C->n - 1] = (mpi_uint)-c;

	/* N = - ( C - N ) */
	MPI_CHK(mpi_sub_abs(N, C, N));
	N->s = -1;

cleanup:

	return(ret);
}

/*
* Helpers for the main 'loop'
* (see fix_negative for the motivation of C)
*/
#define INIT( b )                                                       \
    int ret;                                                            \
    signed char c = 0, cc;                                              \
    uint32_t cur;                                                       \
    size_t i = 0, bits = (b);                                           \
    mpi C;                                                      \
    mpi_uint Cp[ (b) / 8 / sizeof( mpi_uint) + 1 ];     \
                                                                        \
    C.s = 1;                                                            \
    C.n = (b) / 8 / sizeof( mpi_uint) + 1;                      \
    C.p = Cp;                                                           \
    memset( Cp, 0, C.n * sizeof( mpi_uint ) );                  \
                                                                        \
    MPI_CHK( mpi_grow( N, (b) * 2 / 8 /                 \
                                       sizeof( mpi_uint ) ) );  \
    LOAD32;

#define NEXT                    \
    STORE32; i++; LOAD32;       \
    cc = c; c = 0;              \
    if( cc < 0 )                \
        sub32( &cur, -cc, &c ); \
	    else                        \
        add32( &cur, cc, &c );  \

#define LAST                                    \
    STORE32; i++;                               \
    cur = c > 0 ? c : 0; STORE32;               \
    cur = 0; while( ++i < MAX32 ) { STORE32; }  \
    if( c < 0 ) fix_negative( N, c, &C, bits );


/*
* Fast quasi-reduction modulo p224 (FIPS 186-3 D.2.2)
*/
static int ecp_mod_p224(mpi *N)
{
	INIT(224);

	SUB(7); SUB(11);               NEXT; // A0 += -A7 - A11
	SUB(8); SUB(12);               NEXT; // A1 += -A8 - A12
	SUB(9); SUB(13);               NEXT; // A2 += -A9 - A13
	SUB(10); ADD(7); ADD(11);    NEXT; // A3 += -A10 + A7 + A11
	SUB(11); ADD(8); ADD(12);    NEXT; // A4 += -A11 + A8 + A12
	SUB(12); ADD(9); ADD(13);    NEXT; // A5 += -A12 + A9 + A13
	SUB(13); ADD(10);               LAST; // A6 += -A13 + A10

cleanup:
	return(ret);
}

/*
* Fast quasi-reduction modulo p256 (FIPS 186-3 D.2.3)
*/
static int ecp_mod_p256(mpi *N)
{
	INIT(256);

	ADD(8); ADD(9);
	SUB(11); SUB(12); SUB(13); SUB(14);             NEXT; // A0

	ADD(9); ADD(10);
	SUB(12); SUB(13); SUB(14); SUB(15);             NEXT; // A1

	ADD(10); ADD(11);
	SUB(13); SUB(14); SUB(15);                        NEXT; // A2

	ADD(11); ADD(11); ADD(12); ADD(12); ADD(13);
	SUB(15); SUB(8); SUB(9);                        NEXT; // A3

	ADD(12); ADD(12); ADD(13); ADD(13); ADD(14);
	SUB(9); SUB(10);                                   NEXT; // A4

	ADD(13); ADD(13); ADD(14); ADD(14); ADD(15);
	SUB(10); SUB(11);                                   NEXT; // A5

	ADD(14); ADD(14); ADD(15); ADD(15); ADD(14); ADD(13);
	SUB(8); SUB(9);                                   NEXT; // A6

	ADD(15); ADD(15); ADD(15); ADD(8);
	SUB(10); SUB(11); SUB(12); SUB(13);             LAST; // A7

cleanup:
	return(ret);
}

/*
* Fast quasi-reduction modulo p384 (FIPS 186-3 D.2.4)
*/
static int ecp_mod_p384(mpi *N)
{
	INIT(384);

	ADD(12); ADD(21); ADD(20);
	SUB(23);                                              NEXT; // A0

	ADD(13); ADD(22); ADD(23);
	SUB(12); SUB(20);                                   NEXT; // A2

	ADD(14); ADD(23);
	SUB(13); SUB(21);                                   NEXT; // A2

	ADD(15); ADD(12); ADD(20); ADD(21);
	SUB(14); SUB(22); SUB(23);                        NEXT; // A3

	ADD(21); ADD(21); ADD(16); ADD(13); ADD(12); ADD(20); ADD(22);
	SUB(15); SUB(23); SUB(23);                        NEXT; // A4

	ADD(22); ADD(22); ADD(17); ADD(14); ADD(13); ADD(21); ADD(23);
	SUB(16);                                              NEXT; // A5

	ADD(23); ADD(23); ADD(18); ADD(15); ADD(14); ADD(22);
	SUB(17);                                              NEXT; // A6

	ADD(19); ADD(16); ADD(15); ADD(23);
	SUB(18);                                              NEXT; // A7

	ADD(20); ADD(17); ADD(16);
	SUB(19);                                              NEXT; // A8

	ADD(21); ADD(18); ADD(17);
	SUB(20);                                              NEXT; // A9

	ADD(22); ADD(19); ADD(18);
	SUB(21);                                              NEXT; // A10

	ADD(23); ADD(20); ADD(19);
	SUB(22);                                              LAST; // A11

cleanup:
	return(ret);
}

#undef A
#undef LOAD32
#undef STORE32
#undef MAX32
#undef INIT
#undef NEXT
#undef LAST


/* Size of p521 in terms of mpi_uint */
#define P521_WIDTH      ( 521 / 8 / sizeof( mpi_uint ) + 1 )

/* Bits to keep in the most significant mpi_uint */
#define P521_MASK       0x01FF

/*
* Fast quasi-reduction modulo p521 (FIPS 186-3 D.2.5)
* Write N as A1 + 2^521 A0, return A0 + A1
*/
static int ecp_mod_p521(mpi *N)
{
	int ret;
	size_t i;
	mpi M;
	mpi_uint Mp[P521_WIDTH + 1];
	/* Worst case for the size of M is when mpi_uint is 16 bits:
	* we need to hold bits 513 to 1056, which is 34 limbs, that is
	* P521_WIDTH + 1. Otherwise P521_WIDTH is enough. */

	if (N->n < P521_WIDTH)
		return(0);

	/* M = A1 */
	M.s = 1;
	M.n = N->n - (P521_WIDTH - 1);
	if (M.n > P521_WIDTH + 1)
		M.n = P521_WIDTH + 1;
	M.p = Mp;
	memcpy(Mp, N->p + P521_WIDTH - 1, M.n * sizeof(mpi_uint));
	MPI_CHK(mpi_shift_r(&M, 521 % (8 * sizeof(mpi_uint))));

	/* N = A0 */
	N->p[P521_WIDTH - 1] &= P521_MASK;
	for (i = P521_WIDTH; i < N->n; i++)
		N->p[i] = 0;

	/* N = A0 + A1 */
	MPI_CHK(mpi_add_abs(N, N, &M));

cleanup:
	return(ret);
}

#undef P521_WIDTH
#undef P521_MASK


#define P255_WIDTH      ( 255 / 8 / sizeof( mpi_uint ) + 1 )

/*
* Fast quasi-reduction modulo p255 = 2^255 - 19
* Write N as A0 + 2^255 A1, return A0 + 19 * A1
*/
static int ecp_mod_p255(mpi *N)
{
	int ret;
	size_t i;
	mpi M;
	mpi_uint Mp[P255_WIDTH + 2];

	if (N->n < P255_WIDTH)
		return(0);

	/* M = A1 */
	M.s = 1;
	M.n = N->n - (P255_WIDTH - 1);
	if (M.n > P255_WIDTH + 1)
		return(ERR_ECP_BAD_INPUT_DATA);
	M.p = Mp;
	memset(Mp, 0, sizeof Mp);
	memcpy(Mp, N->p + P255_WIDTH - 1, M.n * sizeof(mpi_uint));
	MPI_CHK(mpi_shift_r(&M, 255 % (8 * sizeof(mpi_uint))));
	M.n++; /* Make room for multiplication by 19 */

	/* N = A0 */
	MPI_CHK(mpi_set_bit(N, 255, 0));
	for (i = P255_WIDTH; i < N->n; i++)
		N->p[i] = 0;

	/* N = A0 + 19 * A1 */
	MPI_CHK(mpi_mul_int(&M, &M, 19));
	MPI_CHK(mpi_add_abs(N, N, &M));

cleanup:
	return(ret);
}

#undef P255_WIDTH

/* Size of p448 in terms of mpi_uint */
#define P448_WIDTH      ( 448 / 8 / sizeof( mpi_uint ) )

/* Number of limbs fully occupied by 2^224 (max), and limbs used by it (min) */
#define DIV_ROUND_UP( X, Y ) ( ( ( X ) + ( Y ) - 1 ) / ( Y ) )
#define P224_WIDTH_MIN   ( 28 / sizeof( mpi_uint ) )
#define P224_WIDTH_MAX   DIV_ROUND_UP( 28, sizeof( mpi_uint ) )
#define P224_UNUSED_BITS ( ( P224_WIDTH_MAX * sizeof( mpi_uint ) * 8 ) - 224 )

/*
* Fast quasi-reduction modulo p448 = 2^448 - 2^224 - 1
* Write N as A0 + 2^448 A1 and A1 as B0 + 2^224 B1, and return
* A0 + A1 + B1 + (B0 + B1) * 2^224.  This is different to the reference
* implementation of Curve448, which uses its own special 56-bit limbs rather
* than a generic bignum library.  We could squeeze some extra speed out on
* 32-bit machines by splitting N up into 32-bit limbs and doing the
* arithmetic using the limbs directly as we do for the NIST primes above,
* but for 64-bit targets it should use half the number of operations if we do
* the reduction with 224-bit limbs, since mpi_add_mpi will then use 64-bit adds.
*/
static int ecp_mod_p448(mpi *N)
{
	int ret;
	size_t i;
	mpi M, Q;
	mpi_uint Mp[P448_WIDTH + 1], Qp[P448_WIDTH];

	if (N->n <= P448_WIDTH)
		return(0);

	/* M = A1 */
	M.s = 1;
	M.n = N->n - (P448_WIDTH);
	if (M.n > P448_WIDTH)
		/* Shouldn't be called with N larger than 2^896! */
		return(ERR_ECP_BAD_INPUT_DATA);
	M.p = Mp;
	memset(Mp, 0, sizeof(Mp));
	memcpy(Mp, N->p + P448_WIDTH, M.n * sizeof(mpi_uint));

	/* N = A0 */
	for (i = P448_WIDTH; i < N->n; i++)
		N->p[i] = 0;

	/* N += A1 */
	MPI_CHK(mpi_add_mpi(N, N, &M));

	/* Q = B1, N += B1 */
	Q = M;
	Q.p = Qp;
	memcpy(Qp, Mp, sizeof(Qp));
	MPI_CHK(mpi_shift_r(&Q, 224));
	MPI_CHK(mpi_add_mpi(N, N, &Q));

	/* M = (B0 + B1) * 2^224, N += M */
	if (sizeof(mpi_uint) > 4)
		Mp[P224_WIDTH_MIN] &= ((mpi_uint)-1) >> (P224_UNUSED_BITS);
	for (i = P224_WIDTH_MAX; i < M.n; ++i)
		Mp[i] = 0;
	MPI_CHK(mpi_add_mpi(&M, &M, &Q));
	M.n = P448_WIDTH + 1; /* Make room for shifted carry bit from the addition */
	MPI_CHK(mpi_shift_l(&M, 224));
	MPI_CHK(mpi_add_mpi(N, N, &M));

cleanup:
	return(ret);
}

#undef P448_WIDTH
#undef DIV_ROUND_UP
#undef P224_WIDTH_MIN
#undef P224_WIDTH_MAX
#undef P224_UNUSED_BITS

/*
* Fast quasi-reduction modulo P = 2^s - R,
* with R about 33 bits, used by the Koblitz curves.
*
* Write N as A0 + 2^224 A1, return A0 + R * A1.
* Actually do two passes, since R is big.
*/
#define P_KOBLITZ_MAX   ( 256 / 8 / sizeof( mpi_uint ) )  // Max limbs in P
#define P_KOBLITZ_R     ( 8 / sizeof( mpi_uint ) )        // Limbs in R

static int ecp_mod_koblitz(mpi *N, mpi_uint *Rp, size_t p_limbs,
	size_t adjust, size_t shift, mpi_uint mask)
{
	int ret;
	size_t i;
	mpi M, R;
	mpi_uint Mp[P_KOBLITZ_MAX + P_KOBLITZ_R + 1];

	if (N->n < p_limbs)
		return(0);

	/* Init R */
	R.s = 1;
	R.p = Rp;
	R.n = P_KOBLITZ_R;

	/* Common setup for M */
	M.s = 1;
	M.p = Mp;

	/* M = A1 */
	M.n = N->n - (p_limbs - adjust);
	if (M.n > p_limbs + adjust)
		M.n = p_limbs + adjust;
	memset(Mp, 0, sizeof Mp);
	memcpy(Mp, N->p + p_limbs - adjust, M.n * sizeof(mpi_uint));
	if (shift != 0)
		MPI_CHK(mpi_shift_r(&M, shift));
	M.n += R.n; /* Make room for multiplication by R */

	/* N = A0 */
	if (mask != 0)
		N->p[p_limbs - 1] &= mask;
	for (i = p_limbs; i < N->n; i++)
		N->p[i] = 0;

	/* N = A0 + R * A1 */
	MPI_CHK(mpi_mul_mpi(&M, &M, &R));
	MPI_CHK(mpi_add_abs(N, N, &M));

	/* Second pass */

	/* M = A1 */
	M.n = N->n - (p_limbs - adjust);
	if (M.n > p_limbs + adjust)
		M.n = p_limbs + adjust;
	memset(Mp, 0, sizeof Mp);
	memcpy(Mp, N->p + p_limbs - adjust, M.n * sizeof(mpi_uint));
	if (shift != 0)
		MPI_CHK(mpi_shift_r(&M, shift));
	M.n += R.n; /* Make room for multiplication by R */

	/* N = A0 */
	if (mask != 0)
		N->p[p_limbs - 1] &= mask;
	for (i = p_limbs; i < N->n; i++)
		N->p[i] = 0;

	/* N = A0 + R * A1 */
	MPI_CHK(mpi_mul_mpi(&M, &M, &R));
	MPI_CHK(mpi_add_abs(N, N, &M));

cleanup:
	return(ret);
}

/*
* Fast quasi-reduction modulo p192k1 = 2^192 - R,
* with R = 2^32 + 2^12 + 2^8 + 2^7 + 2^6 + 2^3 + 1 = 0x0100001119
*/
static int ecp_mod_p192k1(mpi *N)
{
	static mpi_uint Rp[] = {
		BYTES_TO_T_UINT_8(0xC9, 0x11, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00) };

	return(ecp_mod_koblitz(N, Rp, 192 / 8 / sizeof(mpi_uint), 0, 0, 0));
}

/*
* Fast quasi-reduction modulo p224k1 = 2^224 - R,
* with R = 2^32 + 2^12 + 2^11 + 2^9 + 2^7 + 2^4 + 2 + 1 = 0x0100001A93
*/
static int ecp_mod_p224k1(mpi *N)
{
	static mpi_uint Rp[] = {
		BYTES_TO_T_UINT_8(0x93, 0x1A, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00) };

#if defined(_OS_64)
	return(ecp_mod_koblitz(N, Rp, 4, 1, 32, 0xFFFFFFFF));
#else
	return(ecp_mod_koblitz(N, Rp, 224 / 8 / sizeof(mpi_uint), 0, 0, 0));
#endif
}

/*
* Fast quasi-reduction modulo p256k1 = 2^256 - R,
* with R = 2^32 + 2^9 + 2^8 + 2^7 + 2^6 + 2^4 + 1 = 0x01000003D1
*/
static int ecp_mod_p256k1(mpi *N)
{
	static mpi_uint Rp[] = {
		BYTES_TO_T_UINT_8(0xD1, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00) };
	return(ecp_mod_koblitz(N, Rp, 256 / 8 / sizeof(mpi_uint), 0, 0, 0));
}

/*
* Set a group using well-known domain parameters
*/
int ecp_group_load(ecp_group *grp, ecp_group_id id)
{
	ecp_group_free(grp);

	grp->id = id;

	switch (id)
	{
	case ECP_DP_SECP192R1:
		NIST_MODP(p192);
		return(LOAD_GROUP(secp192r1));

	case ECP_DP_SECP224R1:
		NIST_MODP(p224);
		return(LOAD_GROUP(secp224r1));

	case ECP_DP_SECP256R1:
		NIST_MODP(p256);
		return(LOAD_GROUP(secp256r1));

	case ECP_DP_SECP384R1:
		NIST_MODP(p384);
		return(LOAD_GROUP(secp384r1));

	case ECP_DP_SECP521R1:
		NIST_MODP(p521);
		return(LOAD_GROUP(secp521r1));

	case ECP_DP_SECP192K1:
		grp->modp = ecp_mod_p192k1;
		return(LOAD_GROUP_A(secp192k1));

	case ECP_DP_SECP224K1:
		grp->modp = ecp_mod_p224k1;
		return(LOAD_GROUP_A(secp224k1));

	case ECP_DP_SECP256K1:
		grp->modp = ecp_mod_p256k1;
		return(LOAD_GROUP_A(secp256k1));

	case ECP_DP_BP256R1:
		return(LOAD_GROUP_A(brainpoolP256r1));

	case ECP_DP_BP384R1:
		return(LOAD_GROUP_A(brainpoolP384r1));

	case ECP_DP_BP512R1:
		return(LOAD_GROUP_A(brainpoolP512r1));

	case ECP_DP_CURVE25519:
		grp->modp = ecp_mod_p255;
		return(ecp_use_curve25519(grp));

	case ECP_DP_CURVE448:
		grp->modp = ecp_mod_p448;
		return(ecp_use_curve448(grp));

	default:
		ecp_group_free(grp);
		return(ERR_ECP_FEATURE_UNAVAILABLE);
	}
}

/*
* Copy the contents of a group object
*/
int ecp_group_copy(ecp_group *dst, const ecp_group *src)
{
	return(ecp_group_load(dst, src->id));
}


/*
* Export a point into unsigned binary data (SEC1 2.3.3)
*/
int ecp_point_write_binary(const ecp_group *grp,
	const ecp_point *P,
	int format, size_t *olen,
	unsigned char *buf, size_t buflen)
{
	int ret = 0;
	size_t plen;

	/*
	* Common case: P == 0
	*/
	if (mpi_cmp_int(&P->Z, 0) == 0)
	{
		if (buflen < 1)
			return(ERR_ECP_BUFFER_TOO_SMALL);

		buf[0] = 0x00;
		*olen = 1;

		return(0);
	}

	plen = mpi_size(&grp->P);

	if (format == ECP_PF_UNCOMPRESSED)
	{
		*olen = 2 * plen + 1;

		if (buflen < *olen)
			return(ERR_ECP_BUFFER_TOO_SMALL);

		buf[0] = 0x04;
		MPI_CHK(mpi_write_binary(&P->X, buf + 1, plen));
		MPI_CHK(mpi_write_binary(&P->Y, buf + 1 + plen, plen));
	}
	else if (format == ECP_PF_COMPRESSED)
	{
		*olen = plen + 1;

		if (buflen < *olen)
			return(ERR_ECP_BUFFER_TOO_SMALL);

		buf[0] = 0x02 + mpi_get_bit(&P->Y, 0);
		MPI_CHK(mpi_write_binary(&P->X, buf + 1, plen));
	}

cleanup:
	return(ret);
}


/*
* Import a point from unsigned binary data (SEC1 2.3.4)
*/
int ecp_point_read_binary(const ecp_group *grp,
	ecp_point *pt,
	const unsigned char *buf, size_t ilen)
{
	int ret;
	size_t plen;

	if (ilen < 1)
		return(ERR_ECP_BAD_INPUT_DATA);

	if (buf[0] == 0x00)
	{
		if (ilen == 1)
			return(ecp_set_zero(pt));
		else
			return(ERR_ECP_BAD_INPUT_DATA);
	}

	plen = mpi_size(&grp->P);

	if (buf[0] != 0x04)
		return(ERR_ECP_FEATURE_UNAVAILABLE);

	if (ilen != 2 * plen + 1)
		return(ERR_ECP_BAD_INPUT_DATA);

	MPI_CHK(mpi_read_binary(&pt->X, buf + 1, plen));
	MPI_CHK(mpi_read_binary(&pt->Y, buf + 1 + plen, plen));
	MPI_CHK(mpi_lset(&pt->Z, 1));

cleanup:
	return(ret);
}

/*
* Import a point from a TLS ECPoint record (RFC 4492)
*      struct {
*          opaque point <1..2^8-1>;
*      } ECPoint;
*/
int ecp_tls_read_point(const ecp_group *grp,
	ecp_point *pt,
	const unsigned char **buf, size_t buf_len)
{
	unsigned char data_len;
	const unsigned char *buf_start;

	/*
	* We must have at least two bytes (1 for length, at least one for data)
	*/
	if (buf_len < 2)
		return(ERR_ECP_BAD_INPUT_DATA);

	data_len = *(*buf)++;
	if (data_len < 1 || data_len > buf_len - 1)
		return(ERR_ECP_BAD_INPUT_DATA);

	/*
	* Save buffer start for read_binary and update buf
	*/
	buf_start = *buf;
	*buf += data_len;

	return(ecp_point_read_binary(grp, pt, buf_start, data_len));
}

/*
* Export a point as a TLS ECPoint record (RFC 4492)
*      struct {
*          opaque point <1..2^8-1>;
*      } ECPoint;
*/
int ecp_tls_write_point(const ecp_group *grp, const ecp_point *pt,
	int format, size_t *olen,
	unsigned char *buf, size_t blen)
{
	int ret;

	/*
	* buffer length must be at least one, for our length byte
	*/
	if (blen < 1)
		return(ERR_ECP_BAD_INPUT_DATA);

	if ((ret = ecp_point_write_binary(grp, pt, format,
		olen, buf + 1, blen - 1)) != 0)
		return(ret);

	/*
	* write length to the first byte and update total length
	*/
	buf[0] = (unsigned char)*olen;
	++*olen;

	return(0);
}


/*
* Read a group id from an ECParameters record (RFC 4492) and convert it to
* ecp_group_id.
*/
int ecp_tls_read_group_id(ecp_group_id *grp,
	const unsigned char **buf, size_t len)
{
	uint16_t tls_id;
	const ecp_curve_info *curve_info;

	/*
	* We expect at least three bytes (see below)
	*/
	if (len < 3)
		return(ERR_ECP_BAD_INPUT_DATA);

	/*
	* First byte is curve_type; only named_curve is handled
	*/
	if (*(*buf)++ != ECP_TLS_NAMED_CURVE)
		return(ERR_ECP_BAD_INPUT_DATA);

	/*
	* Next two bytes are the namedcurve value
	*/
	tls_id = *(*buf)++;
	tls_id <<= 8;
	tls_id |= *(*buf)++;

	if ((curve_info = ecp_curve_info_from_tls_id(tls_id)) == NULL)
		return(ERR_ECP_FEATURE_UNAVAILABLE);

	*grp = curve_info->grp_id;

	return(0);
}

/*
* Set a group from an ECParameters record (RFC 4492)
*/
int ecp_tls_read_group(ecp_group *grp,
	const unsigned char **buf, size_t len)
{
	int ret;
	ecp_group_id grp_id;

	if ((ret = ecp_tls_read_group_id(&grp_id, buf, len)) != 0)
		return(ret);

	return(ecp_group_load(grp, grp_id));
}

/*
* Write the ECParameters record corresponding to a group (RFC 4492)
*/
int ecp_tls_write_group(const ecp_group *grp, size_t *olen,
	unsigned char *buf, size_t blen)
{
	const ecp_curve_info *curve_info;

	if ((curve_info = ecp_curve_info_from_grp_id(grp->id)) == NULL)
		return(ERR_ECP_BAD_INPUT_DATA);

	/*
	* We are going to write 3 bytes (see below)
	*/
	*olen = 3;
	if (blen < *olen)
		return(ERR_ECP_BUFFER_TOO_SMALL);

	/*
	* First byte is curve_type, always named_curve
	*/
	*buf++ = ECP_TLS_NAMED_CURVE;

	/*
	* Next two bytes are the namedcurve value
	*/
	buf[0] = curve_info->tls_id >> 8;
	buf[1] = curve_info->tls_id & 0xFF;

	return(0);
}

/*
* Wrapper around fast quasi-modp functions, with fall-back to mpi_mod_mpi.
* See the documentation of struct ecp_group.
*
* This function is in the critial loop for ecp_mul, so pay attention to perf.
*/
static int ecp_modp(mpi *N, const ecp_group *grp)
{
	int ret;

	if (grp->modp == NULL)
		return(mpi_mod_mpi(N, N, &grp->P));

	/* N->s < 0 is a much faster test, which fails only if N is 0 */
	if ((N->s < 0 && mpi_cmp_int(N, 0) != 0) ||
		mpi_bitlen(N) > 2 * grp->pbits)
	{
		return(ERR_ECP_BAD_INPUT_DATA);
	}

	MPI_CHK(grp->modp(N));

	/* N->s < 0 is a much faster test, which fails only if N is 0 */
	while (N->s < 0 && mpi_cmp_int(N, 0) != 0)
		MPI_CHK(mpi_add_mpi(N, N, &grp->P));

	while (mpi_cmp_mpi(N, &grp->P) >= 0)
		/* we known P, N and the result are positive */
		MPI_CHK(mpi_sub_abs(N, N, &grp->P));

cleanup:
	return(ret);
}

#if defined(ECP_SELF_TEST)
/*
* Counts of point addition and doubling, and field multiplications.
* Used to test resistance of point multiplication to simple timing attacks.
*/
static unsigned long add_count, dbl_count, mul_count;
#endif
/*
* Fast mod-p functions expect their argument to be in the 0..p^2 range.
*
* In order to guarantee that, we need to ensure that operands of
* mpi_mul_mpi are in the 0..p range. So, after each operation we will
* bring the result back to this range.
*
* The following macros are shortcuts for doing that.
*/

/*
* Reduce a mpi mod p in-place, general case, to use after mpi_mul_mpi
*/
#if defined(ECP_SELF_TEST)
#define INC_MUL_COUNT   mul_count++;
#else
#define INC_MUL_COUNT
#endif

#define MOD_MUL( N )                                                    \
    do                                                                  \
	    {                                                                   \
        MPI_CHK( ecp_modp( &(N), grp ) );                       \
        INC_MUL_COUNT                                                   \
	    } while( 0 )

/*
* Reduce a mpi mod p in-place, to use after mpi_sub_mpi
* N->s < 0 is a very fast test, which fails only if N is 0
*/
#define MOD_SUB( N )                                                    \
	    while( (N).s < 0 && mpi_cmp_int( &(N), 0 ) != 0 )           \
        MPI_CHK( mpi_add_mpi( &(N), &(N), &grp->P ) )

/*
* Reduce a mpi mod p in-place, to use after mpi_add_mpi and mpi_mul_int.
* We known P, N and the result are positive, so sub_abs is correct, and
* a bit faster.
*/
#define MOD_ADD( N )                                                    \
	    while( mpi_cmp_mpi( &(N), &grp->P ) >= 0 )                  \
        MPI_CHK( mpi_sub_abs( &(N), &(N), &grp->P ) )

/*
* For curves in short Weierstrass form, we do all the internal operations in
* Jacobian coordinates.
*
* For multiplication, we'll use a comb method with coutermeasueres against
* SPA, hence timing attacks.
*/

/*
* Normalize jacobian coordinates so that Z == 0 || Z == 1  (GECC 3.2.1)
* Cost: 1N := 1I + 3M + 1S
*/
static int ecp_normalize_jac(const ecp_group *grp, ecp_point *pt)
{
	int ret;
	mpi Zi, ZZi;

	if (mpi_cmp_int(&pt->Z, 0) == 0)
		return(0);

#if defined(ECP_NORMALIZE_JAC_ALT)
	if (internal_ecp_grp_capable(grp))
		return(internal_ecp_normalize_jac(grp, pt));
#endif /* MBEDTLS_ECP_NORMALIZE_JAC_ALT */

	mpi_init(&Zi); mpi_init(&ZZi);

	/*
	* X = X / Z^2  mod p
	*/
	MPI_CHK(mpi_inv_mod(&Zi, &pt->Z, &grp->P));
	MPI_CHK(mpi_mul_mpi(&ZZi, &Zi, &Zi)); MOD_MUL(ZZi);
	MPI_CHK(mpi_mul_mpi(&pt->X, &pt->X, &ZZi)); MOD_MUL(pt->X);

	/*
	* Y = Y / Z^3  mod p
	*/
	MPI_CHK(mpi_mul_mpi(&pt->Y, &pt->Y, &ZZi)); MOD_MUL(pt->Y);
	MPI_CHK(mpi_mul_mpi(&pt->Y, &pt->Y, &Zi)); MOD_MUL(pt->Y);

	/*
	* Z = 1
	*/
	MPI_CHK(mpi_lset(&pt->Z, 1));

cleanup:

	mpi_free(&Zi); mpi_free(&ZZi);

	return(ret);
}


/*
* Normalize jacobian coordinates of an array of (pointers to) points,
* using Montgomery's trick to perform only one inversion mod P.
* (See for example Cohen's "A Course in Computational Algebraic Number
* Theory", Algorithm 10.3.4.)
*
* Warning: fails (returning an error) if one of the points is zero!
* This should never happen, see choice of w in ecp_mul_comb().
*
* Cost: 1N(t) := 1I + (6t - 3)M + 1S
*/
static int ecp_normalize_jac_many(const ecp_group *grp,
	ecp_point *T[], size_t T_size)
{
	int ret;
	size_t i;
	mpi *c, u, Zi, ZZi;

	if (T_size < 2)
		return(ecp_normalize_jac(grp, *T));

#if defined(ECP_NORMALIZE_JAC_MANY_ALT)
	if (internal_ecp_grp_capable(grp))
		return(internal_ecp_normalize_jac_many(grp, T, T_size));
#endif

	if ((c = calloc(T_size, sizeof(mpi))) == NULL)
		return(ERR_ECP_ALLOC_FAILED);

	for (i = 0; i < T_size; i++)
		mpi_init(&c[i]);

	mpi_init(&u); mpi_init(&Zi); mpi_init(&ZZi);

	/*
	* c[i] = Z_0 * ... * Z_i
	*/
	MPI_CHK(mpi_copy(&c[0], &T[0]->Z));
	for (i = 1; i < T_size; i++)
	{
		MPI_CHK(mpi_mul_mpi(&c[i], &c[i - 1], &T[i]->Z));
		MOD_MUL(c[i]);
	}

	/*
	* u = 1 / (Z_0 * ... * Z_n) mod P
	*/
	MPI_CHK(mpi_inv_mod(&u, &c[T_size - 1], &grp->P));

	for (i = T_size - 1;; i--)
	{
		/*
		* Zi = 1 / Z_i mod p
		* u = 1 / (Z_0 * ... * Z_i) mod P
		*/
		if (i == 0) {
			MPI_CHK(mpi_copy(&Zi, &u));
		}
		else
		{
			MPI_CHK(mpi_mul_mpi(&Zi, &u, &c[i - 1])); MOD_MUL(Zi);
			MPI_CHK(mpi_mul_mpi(&u, &u, &T[i]->Z)); MOD_MUL(u);
		}

		/*
		* proceed as in normalize()
		*/
		MPI_CHK(mpi_mul_mpi(&ZZi, &Zi, &Zi)); MOD_MUL(ZZi);
		MPI_CHK(mpi_mul_mpi(&T[i]->X, &T[i]->X, &ZZi)); MOD_MUL(T[i]->X);
		MPI_CHK(mpi_mul_mpi(&T[i]->Y, &T[i]->Y, &ZZi)); MOD_MUL(T[i]->Y);
		MPI_CHK(mpi_mul_mpi(&T[i]->Y, &T[i]->Y, &Zi)); MOD_MUL(T[i]->Y);

		/*
		* Post-precessing: reclaim some memory by shrinking coordinates
		* - not storing Z (always 1)
		* - shrinking other coordinates, but still keeping the same number of
		*   limbs as P, as otherwise it will too likely be regrown too fast.
		*/
		MPI_CHK(mpi_shrink(&T[i]->X, grp->P.n));
		MPI_CHK(mpi_shrink(&T[i]->Y, grp->P.n));
		mpi_free(&T[i]->Z);

		if (i == 0)
			break;
	}

cleanup:

	mpi_free(&u); mpi_free(&Zi); mpi_free(&ZZi);
	for (i = 0; i < T_size; i++)
		mpi_free(&c[i]);
	free(c);

	return(ret);
}

/*
* Conditional point inversion: Q -> -Q = (Q.X, -Q.Y, Q.Z) without leak.
* "inv" must be 0 (don't invert) or 1 (invert) or the result will be invalid
*/
static int ecp_safe_invert_jac(const ecp_group *grp,
	ecp_point *Q,
	unsigned char inv)
{
	int ret;
	unsigned char nonzero;
	mpi mQY;

	mpi_init(&mQY);

	/* Use the fact that -Q.Y mod P = P - Q.Y unless Q.Y == 0 */
	MPI_CHK(mpi_sub_mpi(&mQY, &grp->P, &Q->Y));
	nonzero = mpi_cmp_int(&Q->Y, 0) != 0;
	MPI_CHK(mpi_safe_cond_assign(&Q->Y, &mQY, inv & nonzero));

cleanup:
	mpi_free(&mQY);

	return(ret);
}


/*
* Point doubling R = 2 P, Jacobian coordinates
*
* Based on http://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian.html#doubling-dbl-1998-cmo-2 .
*
* We follow the variable naming fairly closely. The formula variations that trade a MUL for a SQR
* (plus a few ADDs) aren't useful as our bignum implementation doesn't distinguish squaring.
*
* Standard optimizations are applied when curve parameter A is one of { 0, -3 }.
*
* Cost: 1D := 3M + 4S          (A ==  0)
*             4M + 4S          (A == -3)
*             3M + 6S + 1a     otherwise
*/
static int ecp_double_jac(const ecp_group *grp, ecp_point *R,
	const ecp_point *P)
{
	int ret;
	mpi M, S, T, U;

#if defined(ECP_SELF_TEST)
	dbl_count++;
#endif

#if defined(ECP_DOUBLE_JAC_ALT)
	if (internal_ecp_grp_capable(grp))
		return(internal_ecp_double_jac(grp, R, P));
#endif /* MBEDTLS_ECP_DOUBLE_JAC_ALT */

	mpi_init(&M); mpi_init(&S); mpi_init(&T); mpi_init(&U);

	/* Special case for A = -3 */
	if (grp->A.p == NULL)
	{
		/* M = 3(X + Z^2)(X - Z^2) */
		MPI_CHK(mpi_mul_mpi(&S, &P->Z, &P->Z)); MOD_MUL(S);
		MPI_CHK(mpi_add_mpi(&T, &P->X, &S)); MOD_ADD(T);
		MPI_CHK(mpi_sub_mpi(&U, &P->X, &S)); MOD_SUB(U);
		MPI_CHK(mpi_mul_mpi(&S, &T, &U)); MOD_MUL(S);
		MPI_CHK(mpi_mul_int(&M, &S, 3)); MOD_ADD(M);
	}
	else
	{
		/* M = 3.X^2 */
		MPI_CHK(mpi_mul_mpi(&S, &P->X, &P->X)); MOD_MUL(S);
		MPI_CHK(mpi_mul_int(&M, &S, 3)); MOD_ADD(M);

		/* Optimize away for "koblitz" curves with A = 0 */
		if (mpi_cmp_int(&grp->A, 0) != 0)
		{
			/* M += A.Z^4 */
			MPI_CHK(mpi_mul_mpi(&S, &P->Z, &P->Z)); MOD_MUL(S);
			MPI_CHK(mpi_mul_mpi(&T, &S, &S)); MOD_MUL(T);
			MPI_CHK(mpi_mul_mpi(&S, &T, &grp->A)); MOD_MUL(S);
			MPI_CHK(mpi_add_mpi(&M, &M, &S)); MOD_ADD(M);
		}
	}

	/* S = 4.X.Y^2 */
	MPI_CHK(mpi_mul_mpi(&T, &P->Y, &P->Y)); MOD_MUL(T);
	MPI_CHK(mpi_shift_l(&T, 1)); MOD_ADD(T);
	MPI_CHK(mpi_mul_mpi(&S, &P->X, &T)); MOD_MUL(S);
	MPI_CHK(mpi_shift_l(&S, 1)); MOD_ADD(S);

	/* U = 8.Y^4 */
	MPI_CHK(mpi_mul_mpi(&U, &T, &T)); MOD_MUL(U);
	MPI_CHK(mpi_shift_l(&U, 1)); MOD_ADD(U);

	/* T = M^2 - 2.S */
	MPI_CHK(mpi_mul_mpi(&T, &M, &M)); MOD_MUL(T);
	MPI_CHK(mpi_sub_mpi(&T, &T, &S)); MOD_SUB(T);
	MPI_CHK(mpi_sub_mpi(&T, &T, &S)); MOD_SUB(T);

	/* S = M(S - T) - U */
	MPI_CHK(mpi_sub_mpi(&S, &S, &T)); MOD_SUB(S);
	MPI_CHK(mpi_mul_mpi(&S, &S, &M)); MOD_MUL(S);
	MPI_CHK(mpi_sub_mpi(&S, &S, &U)); MOD_SUB(S);

	/* U = 2.Y.Z */
	MPI_CHK(mpi_mul_mpi(&U, &P->Y, &P->Z)); MOD_MUL(U);
	MPI_CHK(mpi_shift_l(&U, 1)); MOD_ADD(U);

	MPI_CHK(mpi_copy(&R->X, &T));
	MPI_CHK(mpi_copy(&R->Y, &S));
	MPI_CHK(mpi_copy(&R->Z, &U));

cleanup:
	mpi_free(&M); mpi_free(&S); mpi_free(&T); mpi_free(&U);

	return(ret);
}

/*
* Addition: R = P + Q, mixed affine-Jacobian coordinates (GECC 3.22)
*
* The coordinates of Q must be normalized (= affine),
* but those of P don't need to. R is not normalized.
*
* Special cases: (1) P or Q is zero, (2) R is zero, (3) P == Q.
* None of these cases can happen as intermediate step in ecp_mul_comb():
* - at each step, P, Q and R are multiples of the base point, the factor
*   being less than its order, so none of them is zero;
* - Q is an odd multiple of the base point, P an even multiple,
*   due to the choice of precomputed points in the modified comb method.
* So branches for these cases do not leak secret information.
*
* We accept Q->Z being unset (saving memory in tables) as meaning 1.
*
* Cost: 1A := 8M + 3S
*/
static int ecp_add_mixed(const ecp_group *grp, ecp_point *R,
	const ecp_point *P, const ecp_point *Q)
{
	int ret;
	mpi T1, T2, T3, T4, X, Y, Z;

#if defined(ECP_SELF_TEST)
	add_count++;
#endif

#if defined(ECP_ADD_MIXED_ALT)
	if (internal_ecp_grp_capable(grp))
		return(internal_ecp_add_mixed(grp, R, P, Q));
#endif /* MBEDTLS_ECP_ADD_MIXED_ALT */

	/*
	* Trivial cases: P == 0 or Q == 0 (case 1)
	*/
	if (mpi_cmp_int(&P->Z, 0) == 0)
		return(ecp_copy(R, Q));

	if (Q->Z.p != NULL && mpi_cmp_int(&Q->Z, 0) == 0)
		return(ecp_copy(R, P));

	/*
	* Make sure Q coordinates are normalized
	*/
	if (Q->Z.p != NULL && mpi_cmp_int(&Q->Z, 1) != 0)
		return(ERR_ECP_BAD_INPUT_DATA);

	mpi_init(&T1); mpi_init(&T2); mpi_init(&T3); mpi_init(&T4);
	mpi_init(&X); mpi_init(&Y); mpi_init(&Z);

	MPI_CHK(mpi_mul_mpi(&T1, &P->Z, &P->Z));  MOD_MUL(T1);
	MPI_CHK(mpi_mul_mpi(&T2, &T1, &P->Z));  MOD_MUL(T2);
	MPI_CHK(mpi_mul_mpi(&T1, &T1, &Q->X));  MOD_MUL(T1);
	MPI_CHK(mpi_mul_mpi(&T2, &T2, &Q->Y));  MOD_MUL(T2);
	MPI_CHK(mpi_sub_mpi(&T1, &T1, &P->X));  MOD_SUB(T1);
	MPI_CHK(mpi_sub_mpi(&T2, &T2, &P->Y));  MOD_SUB(T2);

	/* Special cases (2) and (3) */
	if (mpi_cmp_int(&T1, 0) == 0)
	{
		if (mpi_cmp_int(&T2, 0) == 0)
		{
			ret = ecp_double_jac(grp, R, P);
			goto cleanup;
		}
		else
		{
			ret = ecp_set_zero(R);
			goto cleanup;
		}
	}

	MPI_CHK(mpi_mul_mpi(&Z, &P->Z, &T1));  MOD_MUL(Z);
	MPI_CHK(mpi_mul_mpi(&T3, &T1, &T1));  MOD_MUL(T3);
	MPI_CHK(mpi_mul_mpi(&T4, &T3, &T1));  MOD_MUL(T4);
	MPI_CHK(mpi_mul_mpi(&T3, &T3, &P->X));  MOD_MUL(T3);
	MPI_CHK(mpi_mul_int(&T1, &T3, 2));  MOD_ADD(T1);
	MPI_CHK(mpi_mul_mpi(&X, &T2, &T2));  MOD_MUL(X);
	MPI_CHK(mpi_sub_mpi(&X, &X, &T1));  MOD_SUB(X);
	MPI_CHK(mpi_sub_mpi(&X, &X, &T4));  MOD_SUB(X);
	MPI_CHK(mpi_sub_mpi(&T3, &T3, &X));  MOD_SUB(T3);
	MPI_CHK(mpi_mul_mpi(&T3, &T3, &T2));  MOD_MUL(T3);
	MPI_CHK(mpi_mul_mpi(&T4, &T4, &P->Y));  MOD_MUL(T4);
	MPI_CHK(mpi_sub_mpi(&Y, &T3, &T4));  MOD_SUB(Y);

	MPI_CHK(mpi_copy(&R->X, &X));
	MPI_CHK(mpi_copy(&R->Y, &Y));
	MPI_CHK(mpi_copy(&R->Z, &Z));

cleanup:

	mpi_free(&T1); mpi_free(&T2); mpi_free(&T3); mpi_free(&T4);
	mpi_free(&X); mpi_free(&Y); mpi_free(&Z);

	return(ret);
}

/*
* Randomize jacobian coordinates:
* (X, Y, Z) -> (l^2 X, l^3 Y, l Z) for random l
* This is sort of the reverse operation of ecp_normalize_jac().
*
* This countermeasure was first suggested in [2].
*/
static int ecp_randomize_jac(const ecp_group *grp, ecp_point *pt,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	mpi l, ll;
	size_t p_size;
	int count = 0;

#if defined(ECP_RANDOMIZE_JAC_ALT)
	if (internal_ecp_grp_capable(grp))
		return(internal_ecp_randomize_jac(grp, pt, f_rng, p_rng));
#endif /* MBEDTLS_ECP_RANDOMIZE_JAC_ALT */

	p_size = (grp->pbits + 7) / 8;
	mpi_init(&l); mpi_init(&ll);

	/* Generate l such that 1 < l < p */
	do
	{
		MPI_CHK(mpi_fill_random(&l, p_size, f_rng, p_rng));

		while (mpi_cmp_mpi(&l, &grp->P) >= 0)
			MPI_CHK(mpi_shift_r(&l, 1));

		if (count++ > 10)
			return(ERR_ECP_RANDOM_FAILED);
	} while (mpi_cmp_int(&l, 1) <= 0);

	/* Z = l * Z */
	MPI_CHK(mpi_mul_mpi(&pt->Z, &pt->Z, &l)); MOD_MUL(pt->Z);

	/* X = l^2 * X */
	MPI_CHK(mpi_mul_mpi(&ll, &l, &l)); MOD_MUL(ll);
	MPI_CHK(mpi_mul_mpi(&pt->X, &pt->X, &ll)); MOD_MUL(pt->X);

	/* Y = l^3 * Y */
	MPI_CHK(mpi_mul_mpi(&ll, &ll, &l)); MOD_MUL(ll);
	MPI_CHK(mpi_mul_mpi(&pt->Y, &pt->Y, &ll)); MOD_MUL(pt->Y);

cleanup:
	mpi_free(&l); mpi_free(&ll);

	return(ret);
}

/* d = ceil( n / w ) */
#define COMB_MAX_D      ( ECP_MAX_BITS + 1 ) / 2

/* number of precomputed points */
#define COMB_MAX_PRE    ( 1 << ( ECP_WINDOW_SIZE - 1 ) )

/*
* Compute the representation of m that will be used with our comb method.
*
* The basic comb method is described in GECC 3.44 for example. We use a
* modified version that provides resistance to SPA by avoiding zero
* digits in the representation as in [3]. We modify the method further by
* requiring that all K_i be odd, which has the small cost that our
* representation uses one more K_i, due to carries, but saves on the size of
* the precomputed table.
*
* Summary of the comb method and its modifications:
*
* - The goal is to compute m*P for some w*d-bit integer m.
*
* - The basic comb method splits m into the w-bit integers
*   x[0] .. x[d-1] where x[i] consists of the bits in m whose
*   index has residue i modulo d, and computes m * P as
*   S[x[0]] + 2 * S[x[1]] + .. + 2^(d-1) S[x[d-1]], where
*   S[i_{w-1} .. i_0] := i_{w-1} 2^{(w-1)d} P + ... + i_1 2^d P + i_0 P.
*
* - If it happens that, say, x[i+1]=0 (=> S[x[i+1]]=0), one can replace the sum by
*    .. + 2^{i-1} S[x[i-1]] - 2^i S[x[i]] + 2^{i+1} S[x[i]] + 2^{i+2} S[x[i+2]] ..,
*   thereby successively converting it into a form where all summands
*   are nonzero, at the cost of negative summands. This is the basic idea of [3].
*
* - More generally, even if x[i+1] != 0, we can first transform the sum as
*   .. - 2^i S[x[i]] + 2^{i+1} ( S[x[i]] + S[x[i+1]] ) + 2^{i+2} S[x[i+2]] ..,
*   and then replace S[x[i]] + S[x[i+1]] = S[x[i] ^ x[i+1]] + 2 S[x[i] & x[i+1]].
*   Performing and iterating this procedure for those x[i] that are even
*   (keeping track of carry), we can transform the original sum into one of the form
*   S[x'[0]] +- 2 S[x'[1]] +- .. +- 2^{d-1} S[x'[d-1]] + 2^d S[x'[d]]
*   with all x'[i] odd. It is therefore only necessary to know S at odd indices,
*   which is why we are only computing half of it in the first place in
*   ecp_precompute_comb and accessing it with index abs(i) / 2 in ecp_select_comb.
*
* - For the sake of compactness, only the seven low-order bits of x[i]
*   are used to represent its absolute value (K_i in the paper), and the msb
*   of x[i] encodes the sign (s_i in the paper): it is set if and only if
*   if s_i == -1;
*
* Calling conventions:
* - x is an array of size d + 1
* - w is the size, ie number of teeth, of the comb, and must be between
*   2 and 7 (in practice, between 2 and MBEDTLS_ECP_WINDOW_SIZE)
* - m is the MPI, expected to be odd and such that bitlength(m) <= w * d
*   (the result will be incorrect if these assumptions are not satisfied)
*/
static void ecp_comb_recode_core(unsigned char x[], size_t d,
	unsigned char w, const mpi *m)
{
	size_t i, j;
	unsigned char c, cc, adjust;

	memset(x, 0, d + 1);

	/* First get the classical comb values (except for x_d = 0) */
	for (i = 0; i < d; i++)
		for (j = 0; j < w; j++)
			x[i] |= mpi_get_bit(m, i + d * j) << j;

	/* Now make sure x_1 .. x_d are odd */
	c = 0;
	for (i = 1; i <= d; i++)
	{
		/* Add carry and update it */
		cc = x[i] & c;
		x[i] = x[i] ^ c;
		c = cc;

		/* Adjust if needed, avoiding branches */
		adjust = 1 - (x[i] & 0x01);
		c |= x[i] & (x[i - 1] * adjust);
		x[i] = x[i] ^ (x[i - 1] * adjust);
		x[i - 1] |= adjust << 7;
	}
}

/*
* Precompute points for the adapted comb method
*
* Assumption: T must be able to hold 2^{w - 1} elements.
*
* Operation: If i = i_{w-1} ... i_1 is the binary representation of i,
*            sets T[i] = i_{w-1} 2^{(w-1)d} P + ... + i_1 2^d P + P.
*
* Cost: d(w-1) D + (2^{w-1} - 1) A + 1 N(w-1) + 1 N(2^{w-1} - 1)
*
* Note: Even comb values (those where P would be omitted from the
*       sum defining T[i] above) are not needed in our adaption
*       the comb method. See ecp_comb_recode_core().
*
* This function currently works in four steps:
* (1) [dbl]      Computation of intermediate T[i] for 2-power values of i
* (2) [norm_dbl] Normalization of coordinates of these T[i]
* (3) [add]      Computation of all T[i]
* (4) [norm_add] Normalization of all T[i]
*
* Step 1 can be interrupted but not the others; together with the final
* coordinate normalization they are the largest steps done at once, depending
* on the window size. Here are operation counts for P-256:
*
* step     (2)     (3)     (4)
* w = 5    142     165     208
* w = 4    136      77     160
* w = 3    130      33     136
* w = 2    124      11     124
*
* So if ECC operations are blocking for too long even with a low max_ops
* value, it's useful to set MBEDTLS_ECP_WINDOW_SIZE to a lower value in order
* to minimize maximum blocking time.
*/
static int ecp_precompute_comb(const ecp_group *grp,
	ecp_point T[], const ecp_point *P,
	unsigned char w, size_t d,
	ecp_restart_ctx *rs_ctx)
{
	int ret;
	unsigned char i;
	size_t j = 0;
	const unsigned char T_size = 1U << (w - 1);
	ecp_point *cur, *TT[COMB_MAX_PRE - 1];

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL)
	{
		if (rs_ctx->rsm->state == ecp_rsm_pre_dbl)
			goto dbl;
		if (rs_ctx->rsm->state == ecp_rsm_pre_norm_dbl)
			goto norm_dbl;
		if (rs_ctx->rsm->state == ecp_rsm_pre_add)
			goto add;
		if (rs_ctx->rsm->state == ecp_rsm_pre_norm_add)
			goto norm_add;
	}
#else
	(void)rs_ctx;
#endif

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL)
	{
		rs_ctx->rsm->state = ecp_rsm_pre_dbl;

		/* initial state for the loop */
		rs_ctx->rsm->i = 0;
	}

dbl:
#endif
	/*
	* Set T[0] = P and
	* T[2^{l-1}] = 2^{dl} P for l = 1 .. w-1 (this is not the final value)
	*/
	MPI_CHK(ecp_copy(&T[0], P));

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL && rs_ctx->rsm->i != 0)
		j = rs_ctx->rsm->i;
	else
#endif
		j = 0;

	for (; j < d * (w - 1); j++)
	{
		ECP_BUDGET(ECP_OPS_DBL);

		i = 1U << (j / d);
		cur = T + i;

		if (j % d == 0)
			MPI_CHK(ecp_copy(cur, T + (i >> 1)));

		MPI_CHK(ecp_double_jac(grp, cur, cur));
	}

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL)
		rs_ctx->rsm->state = ecp_rsm_pre_norm_dbl;

norm_dbl:
#endif
	/*
	* Normalize current elements in T. As T has holes,
	* use an auxiliary array of pointers to elements in T.
	*/
	j = 0;
	for (i = 1; i < T_size; i <<= 1)
		TT[j++] = T + i;

	ECP_BUDGET(ECP_OPS_INV + 6 * j - 2);

	MPI_CHK(ecp_normalize_jac_many(grp, TT, j));

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL)
		rs_ctx->rsm->state = ecp_rsm_pre_add;

add:
#endif
	/*
	* Compute the remaining ones using the minimal number of additions
	* Be careful to update T[2^l] only after using it!
	*/
	ECP_BUDGET((T_size - 1) * ECP_OPS_ADD);

	for (i = 1; i < T_size; i <<= 1)
	{
		j = i;
		while (j--)
			MPI_CHK(ecp_add_mixed(grp, &T[i + j], &T[j], &T[i]));
	}

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL)
		rs_ctx->rsm->state = ecp_rsm_pre_norm_add;

norm_add:
#endif
	/*
	* Normalize final elements in T. Even though there are no holes now, we
	* still need the auxiliary array for homogeneity with the previous
	* call. Also, skip T[0] which is already normalised, being a copy of P.
	*/
	for (j = 0; j + 1 < T_size; j++)
		TT[j] = T + j + 1;

	ECP_BUDGET(ECP_OPS_INV + 6 * j - 2);

	MPI_CHK(ecp_normalize_jac_many(grp, TT, j));

cleanup:
#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL &&
		ret == ERR_ECP_IN_PROGRESS)
	{
		if (rs_ctx->rsm->state == ecp_rsm_pre_dbl)
			rs_ctx->rsm->i = j;
	}
#endif

	return(ret);
}


/*
* Select precomputed point: R = sign(i) * T[ abs(i) / 2 ]
*
* See ecp_comb_recode_core() for background
*/
static int ecp_select_comb(const ecp_group *grp, ecp_point *R,
	const ecp_point T[], unsigned char T_size,
	unsigned char i)
{
	int ret;
	unsigned char ii, j;

	/* Ignore the "sign" bit and scale down */
	ii = (i & 0x7Fu) >> 1;

	/* Read the whole table to thwart cache-based timing attacks */
	for (j = 0; j < T_size; j++)
	{
		MPI_CHK(mpi_safe_cond_assign(&R->X, &T[j].X, j == ii));
		MPI_CHK(mpi_safe_cond_assign(&R->Y, &T[j].Y, j == ii));
	}

	/* Safely invert result if i is "negative" */
	MPI_CHK(ecp_safe_invert_jac(grp, R, i >> 7));

cleanup:
	return(ret);
}

/*
* Core multiplication algorithm for the (modified) comb method.
* This part is actually common with the basic comb method (GECC 3.44)
*
* Cost: d A + d D + 1 R
*/
static int ecp_mul_comb_core(const ecp_group *grp, ecp_point *R,
	const ecp_point T[], unsigned char T_size,
	const unsigned char x[], size_t d,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng,
	ecp_restart_ctx *rs_ctx)
{
	int ret;
	ecp_point Txi;
	size_t i;

	ecp_point_init(&Txi);

#if !defined(ECP_RESTARTABLE)
	(void)rs_ctx;
#endif

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL &&
		rs_ctx->rsm->state != ecp_rsm_comb_core)
	{
		rs_ctx->rsm->i = 0;
		rs_ctx->rsm->state = ecp_rsm_comb_core;
	}

	/* new 'if' instead of nested for the sake of the 'else' branch */
	if (rs_ctx != NULL && rs_ctx->rsm != NULL && rs_ctx->rsm->i != 0)
	{
		/* restore current index (R already pointing to rs_ctx->rsm->R) */
		i = rs_ctx->rsm->i;
	}
	else
#endif
	{
		/* Start with a non-zero point and randomize its coordinates */
		i = d;
		MPI_CHK(ecp_select_comb(grp, R, T, T_size, x[i]));
		MPI_CHK(mpi_lset(&R->Z, 1));
		if (f_rng != 0)
			MPI_CHK(ecp_randomize_jac(grp, R, f_rng, p_rng));
	}

	while (i != 0)
	{
		ECP_BUDGET(ECP_OPS_DBL + ECP_OPS_ADD);
		--i;

		MPI_CHK(ecp_double_jac(grp, R, R));
		MPI_CHK(ecp_select_comb(grp, &Txi, T, T_size, x[i]));
		MPI_CHK(ecp_add_mixed(grp, R, R, &Txi));
	}

cleanup:

	ecp_point_free(&Txi);

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL &&
		ret == ERR_ECP_IN_PROGRESS)
	{
		rs_ctx->rsm->i = i;
		/* no need to save R, already pointing to rs_ctx->rsm->R */
	}
#endif

	return(ret);
}

/*
* Recode the scalar to get constant-time comb multiplication
*
* As the actual scalar recoding needs an odd scalar as a starting point,
* this wrapper ensures that by replacing m by N - m if necessary, and
* informs the caller that the result of multiplication will be negated.
*
* This works because we only support large prime order for Short Weierstrass
* curves, so N is always odd hence either m or N - m is.
*
* See ecp_comb_recode_core() for background.
*/
static int ecp_comb_recode_scalar(const ecp_group *grp,
	const mpi *m,
	unsigned char k[COMB_MAX_D + 1],
	size_t d,
	unsigned char w,
	unsigned char *parity_trick)
{
	int ret;
	mpi M, mm;

	mpi_init(&M);
	mpi_init(&mm);

	/* N is always odd (see above), just make extra sure */
	if (mpi_get_bit(&grp->N, 0) != 1)
		return(ERR_ECP_BAD_INPUT_DATA);

	/* do we need the parity trick? */
	*parity_trick = (mpi_get_bit(m, 0) == 0);

	/* execute parity fix in constant time */
	MPI_CHK(mpi_copy(&M, m));
	MPI_CHK(mpi_sub_mpi(&mm, &grp->N, m));
	MPI_CHK(mpi_safe_cond_assign(&M, &mm, *parity_trick));

	/* actual scalar recoding */
	ecp_comb_recode_core(k, d, w, &M);

cleanup:
	mpi_free(&mm);
	mpi_free(&M);

	return(ret);
}

/*
* Perform comb multiplication (for short Weierstrass curves)
* once the auxiliary table has been pre-computed.
*
* Scalar recoding may use a parity trick that makes us compute -m * P,
* if that is the case we'll need to recover m * P at the end.
*/
static int ecp_mul_comb_after_precomp(const ecp_group *grp,
	ecp_point *R,
	const mpi *m,
	const ecp_point *T,
	unsigned char T_size,
	unsigned char w,
	size_t d,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng,
	ecp_restart_ctx *rs_ctx)
{
	int ret;
	unsigned char parity_trick;
	unsigned char k[COMB_MAX_D + 1];
	ecp_point *RR = R;

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL)
	{
		RR = &rs_ctx->rsm->R;

		if (rs_ctx->rsm->state == ecp_rsm_final_norm)
			goto final_norm;
	}
#endif

	MPI_CHK(ecp_comb_recode_scalar(grp, m, k, d, w,
		&parity_trick));
	MPI_CHK(ecp_mul_comb_core(grp, RR, T, T_size, k, d,
		f_rng, p_rng, rs_ctx));
	MPI_CHK(ecp_safe_invert_jac(grp, RR, parity_trick));

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL)
		rs_ctx->rsm->state = ecp_rsm_final_norm;

final_norm:
#endif
	/*
	* Knowledge of the jacobian coordinates may leak the last few bits of the
	* scalar [1], and since our MPI implementation isn't constant-flow,
	* inversion (used for coordinate normalization) may leak the full value
	* of its input via side-channels [2].
	*
	* [1] https://eprint.iacr.org/2003/191
	* [2] https://eprint.iacr.org/2020/055
	*
	* Avoid the leak by randomizing coordinates before we normalize them.
	*/
	if (f_rng != 0)
		MPI_CHK(ecp_randomize_jac(grp, RR, f_rng, p_rng));

	ECP_BUDGET(ECP_OPS_INV);
	MPI_CHK(ecp_normalize_jac(grp, RR));

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL)
		MPI_CHK(ecp_copy(R, RR));
#endif

cleanup:
	return(ret);
}

/*
* Pick window size based on curve size and whether we optimize for base point
*/
static unsigned char ecp_pick_window_size(const ecp_group *grp,
	unsigned char p_eq_g)
{
	unsigned char w;

	/*
	* Minimize the number of multiplications, that is minimize
	* 10 * d * w + 18 * 2^(w-1) + 11 * d + 7 * w, with d = ceil( nbits / w )
	* (see costs of the various parts, with 1S = 1M)
	*/
	w = grp->nbits >= 384 ? 5 : 4;

	/*
	* If P == G, pre-compute a bit more, since this may be re-used later.
	* Just adding one avoids upping the cost of the first mul too much,
	* and the memory cost too.
	*/
	if (p_eq_g)
		w++;

	/*
	* Make sure w is within bounds.
	* (The last test is useful only for very small curves in the test suite.)
	*/
	if (w > ECP_WINDOW_SIZE)
		w = ECP_WINDOW_SIZE;
	if (w >= grp->nbits)
		w = 2;

	return(w);
}


/*
* Multiplication using the comb method - for curves in short Weierstrass form
*
* This function is mainly responsible for administrative work:
* - managing the restart context if enabled
* - managing the table of precomputed points (passed between the below two
*   functions): allocation, computation, ownership tranfer, freeing.
*
* It delegates the actual arithmetic work to:
*      ecp_precompute_comb() and ecp_mul_comb_with_precomp()
*
* See comments on ecp_comb_recode_core() regarding the computation strategy.
*/
static int ecp_mul_comb(ecp_group *grp, ecp_point *R,
	const mpi *m, const ecp_point *P,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng,
	ecp_restart_ctx *rs_ctx)
{
	int ret;
	unsigned char w, p_eq_g, i;
	size_t d;
	unsigned char T_size, T_ok;
	ecp_point *T;

	ECP_RS_ENTER(rsm);

	/* Is P the base point ? */
#if ECP_FIXED_POINT_OPTIM == 1
	p_eq_g = (mpi_cmp_mpi(&P->Y, &grp->G.Y) == 0 &&
		mpi_cmp_mpi(&P->X, &grp->G.X) == 0);
#else
	p_eq_g = 0;
#endif

	/* Pick window size and deduce related sizes */
	w = ecp_pick_window_size(grp, p_eq_g);
	T_size = 1U << (w - 1);
	d = (grp->nbits + w - 1) / w;

	/* Pre-computed table: do we have it already for the base point? */
	if (p_eq_g && grp->T != NULL)
	{
		/* second pointer to the same table, will be deleted on exit */
		T = grp->T;
		T_ok = 1;
	}
	else
#if defined(ECP_RESTARTABLE)
		/* Pre-computed table: do we have one in progress? complete? */
		if (rs_ctx != NULL && rs_ctx->rsm != NULL && rs_ctx->rsm->T != NULL)
		{
			/* transfer ownership of T from rsm to local function */
			T = rs_ctx->rsm->T;
			rs_ctx->rsm->T = NULL;
			rs_ctx->rsm->T_size = 0;

			/* This effectively jumps to the call to mul_comb_after_precomp() */
			T_ok = rs_ctx->rsm->state >= ecp_rsm_comb_core;
		}
		else
#endif
			/* Allocate table if we didn't have any */
		{
			T = calloc(T_size, sizeof(ecp_point));
			if (T == NULL)
			{
				ret = ERR_ECP_ALLOC_FAILED;
				goto cleanup;
			}

			for (i = 0; i < T_size; i++)
				ecp_point_init(&T[i]);

			T_ok = 0;
		}

	/* Compute table (or finish computing it) if not done already */
	if (!T_ok)
	{
		MPI_CHK(ecp_precompute_comb(grp, T, P, w, d, rs_ctx));

		if (p_eq_g)
		{
			/* almost transfer ownership of T to the group, but keep a copy of
			* the pointer to use for calling the next function more easily */
			grp->T = T;
			grp->T_size = T_size;
		}
	}

	/* Actual comb multiplication using precomputed points */
	MPI_CHK(ecp_mul_comb_after_precomp(grp, R, m,
		T, T_size, w, d,
		f_rng, p_rng, rs_ctx));

cleanup:

	/* does T belong to the group? */
	if (T == grp->T)
		T = NULL;

	/* does T belong to the restart context? */
#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->rsm != NULL && ret == ERR_ECP_IN_PROGRESS && T != NULL)
	{
		/* transfer ownership of T from local function to rsm */
		rs_ctx->rsm->T_size = T_size;
		rs_ctx->rsm->T = T;
		T = NULL;
	}
#endif

	/* did T belong to us? then let's destroy it! */
	if (T != NULL)
	{
		for (i = 0; i < T_size; i++)
			ecp_point_free(&T[i]);
		free(T);
	}

	/* don't free R while in progress in case R == P */
#if defined(ECP_RESTARTABLE)
	if (ret != ERR_ECP_IN_PROGRESS)
#endif
		/* prevent caller from using invalid value */
		if (ret != 0)
			ecp_point_free(R);

	ECP_RS_LEAVE(rsm);

	return(ret);
}

/*
* For Montgomery curves, we do all the internal arithmetic in projective
* coordinates. Import/export of points uses only the x coordinates, which is
* internaly represented as X / Z.
*
* For scalar multiplication, we'll use a Montgomery ladder.
*/

/*
* Normalize Montgomery x/z coordinates: X = X/Z, Z = 1
* Cost: 1M + 1I
*/
static int ecp_normalize_mxz(const ecp_group *grp, ecp_point *P)
{
	int ret;

#if defined(ECP_NORMALIZE_MXZ_ALT)
	if (internal_ecp_grp_capable(grp))
		return(internal_ecp_normalize_mxz(grp, P));
#endif /* MBEDTLS_ECP_NORMALIZE_MXZ_ALT */

	MPI_CHK(mpi_inv_mod(&P->Z, &P->Z, &grp->P));
	MPI_CHK(mpi_mul_mpi(&P->X, &P->X, &P->Z)); MOD_MUL(P->X);
	MPI_CHK(mpi_lset(&P->Z, 1));

cleanup:
	return(ret);
}

/*
* Randomize projective x/z coordinates:
* (X, Z) -> (l X, l Z) for random l
* This is sort of the reverse operation of ecp_normalize_mxz().
*
* This countermeasure was first suggested in [2].
* Cost: 2M
*/
static int ecp_randomize_mxz(const ecp_group *grp, ecp_point *P,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	mpi l;
	size_t p_size;
	int count = 0;

#if defined(ECP_RANDOMIZE_MXZ_ALT)
	if (internal_ecp_grp_capable(grp))
		return(internal_ecp_randomize_mxz(grp, P, f_rng, p_rng);
#endif /* MBEDTLS_ECP_RANDOMIZE_MXZ_ALT */

	p_size = (grp->pbits + 7) / 8;
	mpi_init(&l);

	/* Generate l such that 1 < l < p */
	do
	{
		MPI_CHK(mpi_fill_random(&l, p_size, f_rng, p_rng));

		while (mpi_cmp_mpi(&l, &grp->P) >= 0)
			MPI_CHK(mpi_shift_r(&l, 1));

		if (count++ > 10)
			return(ERR_ECP_RANDOM_FAILED);
	} while (mpi_cmp_int(&l, 1) <= 0);

	MPI_CHK(mpi_mul_mpi(&P->X, &P->X, &l)); MOD_MUL(P->X);
	MPI_CHK(mpi_mul_mpi(&P->Z, &P->Z, &l)); MOD_MUL(P->Z);

cleanup:
	mpi_free(&l);

	return(ret);
}


/*
* Double-and-add: R = 2P, S = P + Q, with d = X(P - Q),
* for Montgomery curves in x/z coordinates.
*
* http://www.hyperelliptic.org/EFD/g1p/auto-code/montgom/xz/ladder/mladd-1987-m.op3
* with
* d =  X1
* P = (X2, Z2)
* Q = (X3, Z3)
* R = (X4, Z4)
* S = (X5, Z5)
* and eliminating temporary variables tO, ..., t4.
*
* Cost: 5M + 4S
*/
static int ecp_double_add_mxz(const ecp_group *grp,
	ecp_point *R, ecp_point *S,
	const ecp_point *P, const ecp_point *Q,
	const mpi *d)
{
	int ret;
	mpi A, AA, B, BB, E, C, D, DA, CB;

#if defined(ECP_DOUBLE_ADD_MXZ_ALT)
	if (internal_ecp_grp_capable(grp))
		return(internal_ecp_double_add_mxz(grp, R, S, P, Q, d));
#endif /* MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT */

	mpi_init(&A); mpi_init(&AA); mpi_init(&B);
	mpi_init(&BB); mpi_init(&E); mpi_init(&C);
	mpi_init(&D); mpi_init(&DA); mpi_init(&CB);

	MPI_CHK(mpi_add_mpi(&A, &P->X, &P->Z)); MOD_ADD(A);
	MPI_CHK(mpi_mul_mpi(&AA, &A, &A)); MOD_MUL(AA);
	MPI_CHK(mpi_sub_mpi(&B, &P->X, &P->Z)); MOD_SUB(B);
	MPI_CHK(mpi_mul_mpi(&BB, &B, &B)); MOD_MUL(BB);
	MPI_CHK(mpi_sub_mpi(&E, &AA, &BB)); MOD_SUB(E);
	MPI_CHK(mpi_add_mpi(&C, &Q->X, &Q->Z)); MOD_ADD(C);
	MPI_CHK(mpi_sub_mpi(&D, &Q->X, &Q->Z)); MOD_SUB(D);
	MPI_CHK(mpi_mul_mpi(&DA, &D, &A)); MOD_MUL(DA);
	MPI_CHK(mpi_mul_mpi(&CB, &C, &B)); MOD_MUL(CB);
	MPI_CHK(mpi_add_mpi(&S->X, &DA, &CB)); MOD_MUL(S->X);
	MPI_CHK(mpi_mul_mpi(&S->X, &S->X, &S->X)); MOD_MUL(S->X);
	MPI_CHK(mpi_sub_mpi(&S->Z, &DA, &CB)); MOD_SUB(S->Z);
	MPI_CHK(mpi_mul_mpi(&S->Z, &S->Z, &S->Z)); MOD_MUL(S->Z);
	MPI_CHK(mpi_mul_mpi(&S->Z, d, &S->Z)); MOD_MUL(S->Z);
	MPI_CHK(mpi_mul_mpi(&R->X, &AA, &BB)); MOD_MUL(R->X);
	MPI_CHK(mpi_mul_mpi(&R->Z, &grp->A, &E)); MOD_MUL(R->Z);
	MPI_CHK(mpi_add_mpi(&R->Z, &BB, &R->Z)); MOD_ADD(R->Z);
	MPI_CHK(mpi_mul_mpi(&R->Z, &E, &R->Z)); MOD_MUL(R->Z);

cleanup:
	mpi_free(&A); mpi_free(&AA); mpi_free(&B);
	mpi_free(&BB); mpi_free(&E); mpi_free(&C);
	mpi_free(&D); mpi_free(&DA); mpi_free(&CB);

	return(ret);
}

/*
* Multiplication with Montgomery ladder in x/z coordinates,
* for curves in Montgomery form
*/
static int ecp_mul_mxz(ecp_group *grp, ecp_point *R,
	const mpi *m, const ecp_point *P,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	int ret;
	size_t i;
	unsigned char b;
	ecp_point RP;
	mpi PX;

	ecp_point_init(&RP); mpi_init(&PX);

	/* Save PX and read from P before writing to R, in case P == R */
	MPI_CHK(mpi_copy(&PX, &P->X));
	MPI_CHK(ecp_copy(&RP, P));

	/* Set R to zero in modified x/z coordinates */
	MPI_CHK(mpi_lset(&R->X, 1));
	MPI_CHK(mpi_lset(&R->Z, 0));
	mpi_free(&R->Y);

	/* RP.X might be sligtly larger than P, so reduce it */
	MOD_ADD(RP.X);

	/* Randomize coordinates of the starting point */
	if (f_rng != NULL)
		MPI_CHK(ecp_randomize_mxz(grp, &RP, f_rng, p_rng));

	/* Loop invariant: R = result so far, RP = R + P */
	i = mpi_bitlen(m); /* one past the (zero-based) most significant bit */
	while (i-- > 0)
	{
		b = mpi_get_bit(m, i);
		/*
		*  if (b) R = 2R + P else R = 2R,
		* which is:
		*  if (b) double_add( RP, R, RP, R )
		*  else   double_add( R, RP, R, RP )
		* but using safe conditional swaps to avoid leaks
		*/
		MPI_CHK(mpi_safe_cond_swap(&R->X, &RP.X, b));
		MPI_CHK(mpi_safe_cond_swap(&R->Z, &RP.Z, b));
		MPI_CHK(ecp_double_add_mxz(grp, R, &RP, R, &RP, &PX));
		MPI_CHK(mpi_safe_cond_swap(&R->X, &RP.X, b));
		MPI_CHK(mpi_safe_cond_swap(&R->Z, &RP.Z, b));
	}

	/*
	* Knowledge of the projective coordinates may leak the last few bits of the
	* scalar [1], and since our MPI implementation isn't constant-flow,
	* inversion (used for coordinate normalization) may leak the full value
	* of its input via side-channels [2].
	*
	* [1] https://eprint.iacr.org/2003/191
	* [2] https://eprint.iacr.org/2020/055
	*
	* Avoid the leak by randomizing coordinates before we normalize them.
	*/
	if (f_rng != NULL)
		MPI_CHK(ecp_randomize_mxz(grp, R, f_rng, p_rng));

	MPI_CHK(ecp_normalize_mxz(grp, R));

cleanup:
	ecp_point_free(&RP); mpi_free(&PX);

	return(ret);
}

/*
* Check that an affine point is valid as a public key,
* short weierstrass curves (SEC1 3.2.3.1)
*/
static int ecp_check_pubkey_sw(const ecp_group *grp, const ecp_point *pt)
{
	int ret;
	mpi YY, RHS;

	/* pt coordinates must be normalized for our checks */
	if (mpi_cmp_int(&pt->X, 0) < 0 ||
		mpi_cmp_int(&pt->Y, 0) < 0 ||
		mpi_cmp_mpi(&pt->X, &grp->P) >= 0 ||
		mpi_cmp_mpi(&pt->Y, &grp->P) >= 0)
		return(ERR_ECP_INVALID_KEY);

	mpi_init(&YY); mpi_init(&RHS);

	/*
	* YY = Y^2
	* RHS = X (X^2 + A) + B = X^3 + A X + B
	*/
	MPI_CHK(mpi_mul_mpi(&YY, &pt->Y, &pt->Y));  MOD_MUL(YY);
	MPI_CHK(mpi_mul_mpi(&RHS, &pt->X, &pt->X));  MOD_MUL(RHS);

	/* Special case for A = -3 */
	if (grp->A.p == NULL)
	{
		MPI_CHK(mpi_sub_int(&RHS, &RHS, 3));  MOD_SUB(RHS);
	}
	else
	{
		MPI_CHK(mpi_add_mpi(&RHS, &RHS, &grp->A));  MOD_ADD(RHS);
	}

	MPI_CHK(mpi_mul_mpi(&RHS, &RHS, &pt->X));  MOD_MUL(RHS);
	MPI_CHK(mpi_add_mpi(&RHS, &RHS, &grp->B));  MOD_ADD(RHS);

	if (mpi_cmp_mpi(&YY, &RHS) != 0)
		ret = ERR_ECP_INVALID_KEY;

cleanup:

	mpi_free(&YY); mpi_free(&RHS);

	return(ret);
}

/*
* Check validity of a public key for Montgomery curves with x-only schemes
*/
static int ecp_check_pubkey_mx(const ecp_group *grp, const ecp_point *pt)
{
	/* [Curve25519 p. 5] Just check X is the correct number of bytes */
	/* Allow any public value, if it's too big then we'll just reduce it mod p
	* (RFC 7748 sec. 5 para. 3). */
	if (mpi_size(&pt->X) > (grp->nbits + 7) / 8)
		return(ERR_ECP_INVALID_KEY);

	return(0);
}

/*
* Check that a point is valid as a public key
*/
int ecp_check_pubkey(const ecp_group *grp,
	const ecp_point *pt)
{

	/* Must use affine coordinates */
	if (mpi_cmp_int(&pt->Z, 1) != 0)
		return(ERR_ECP_INVALID_KEY);

	if (ecp_get_type(grp) == ECP_TYPE_MONTGOMERY)
		return(ecp_check_pubkey_mx(grp, pt));

	if (ecp_get_type(grp) == ECP_TYPE_SHORT_WEIERSTRASS)
		return(ecp_check_pubkey_sw(grp, pt));

	return(ERR_ECP_BAD_INPUT_DATA);
}

/*
* Check that an mpi is valid as a private key
*/
int ecp_check_privkey(const ecp_group *grp,
	const mpi *d)
{
	if (ecp_get_type(grp) == ECP_TYPE_MONTGOMERY)
	{
		/* see RFC 7748 sec. 5 para. 5 */
		if (mpi_get_bit(d, 0) != 0 ||
			mpi_get_bit(d, 1) != 0 ||
			mpi_bitlen(d) - 1 != grp->nbits) /* mpi_bitlen is one-based! */
			return(ERR_ECP_INVALID_KEY);

		/* see [Curve25519] page 5 */
		if (grp->nbits == 254 && mpi_get_bit(d, 2) != 0)
			return(ERR_ECP_INVALID_KEY);

		return(0);
	}

	if (ecp_get_type(grp) == ECP_TYPE_SHORT_WEIERSTRASS)
	{
		/* see SEC1 3.2 */
		if (mpi_cmp_int(d, 1) < 0 ||
			mpi_cmp_mpi(d, &grp->N) >= 0)
			return(ERR_ECP_INVALID_KEY);
		else
			return(0);
	}

	return(ERR_ECP_BAD_INPUT_DATA);
}


/*
* Generate a private key
*/
int ecp_gen_privkey(const ecp_group *grp,
	mpi *d,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	int ret = ERR_ECP_BAD_INPUT_DATA;
	size_t n_size;

	n_size = (grp->nbits + 7) / 8;

	if (ecp_get_type(grp) == ECP_TYPE_MONTGOMERY)
	{
		/* [M225] page 5 */
		size_t b;

		do {
			MPI_CHK(mpi_fill_random(d, n_size, f_rng, p_rng));
		} while (mpi_bitlen(d) == 0);

		/* Make sure the most significant bit is nbits */
		b = mpi_bitlen(d) - 1; /* mpi_bitlen is one-based */
		if (b > grp->nbits)
			MPI_CHK(mpi_shift_r(d, b - grp->nbits));
		else
			MPI_CHK(mpi_set_bit(d, grp->nbits, 1));

		/* Make sure the last two bits are unset for Curve448, three bits for
		Curve25519 */
		MPI_CHK(mpi_set_bit(d, 0, 0));
		MPI_CHK(mpi_set_bit(d, 1, 0));
		if (grp->nbits == 254)
		{
			MPI_CHK(mpi_set_bit(d, 2, 0));
		}
	}

	if (ecp_get_type(grp) == ECP_TYPE_SHORT_WEIERSTRASS)
	{
		/* SEC1 3.2.1: Generate d such that 1 <= n < N */
		int count = 0;
		unsigned cmp = 0;

		/*
		* Match the procedure given in RFC 6979 (deterministic ECDSA):
		* - use the same byte ordering;
		* - keep the leftmost nbits bits of the generated octet string;
		* - try until result is in the desired range.
		* This also avoids any biais, which is especially important for ECDSA.
		*/
		do
		{
			MPI_CHK(mpi_fill_random(d, n_size, f_rng, p_rng));
			MPI_CHK(mpi_shift_r(d, 8 * n_size - grp->nbits));

			/*
			* Each try has at worst a probability 1/2 of failing (the msb has
			* a probability 1/2 of being 0, and then the result will be < N),
			* so after 30 tries failure probability is a most 2**(-30).
			*
			* For most curves, 1 try is enough with overwhelming probability,
			* since N starts with a lot of 1s in binary, but some curves
			* such as secp224k1 are actually very close to the worst case.
			*/
			if (++count > 30)
				return(ERR_ECP_RANDOM_FAILED);

			ret = mpi_lt_mpi_ct(d, &grp->N, &cmp);
			if (ret != 0)
			{
				goto cleanup;
			}
		} while (mpi_cmp_int(d, 1) < 0 || cmp != 1);
	}

cleanup:
	return(ret);
}

/*
 * Restartable multiplication R = m * P
 */
int ecp_mul_restartable( ecp_group *grp, ecp_point *R,
             const mpi *m, const ecp_point *P,
             int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
             ecp_restart_ctx *rs_ctx )
{
    int ret = ERR_ECP_BAD_INPUT_DATA;
#if defined(ECP_INTERNAL_ALT)
    char is_grp_capable = 0;
#endif

#if defined(ECP_RESTARTABLE)
    /* reset ops count for this call if top-level */
    if( rs_ctx != NULL && rs_ctx->depth++ == 0 )
        rs_ctx->ops_done = 0;
#endif

#if defined(ECP_INTERNAL_ALT)
    if( ( is_grp_capable = internal_ecp_grp_capable( grp ) ) )
        MPI_CHK( internal_ecp_init( grp ) );
#endif /* ECP_INTERNAL_ALT */

#if defined(ECP_RESTARTABLE)
    /* skip argument check when restarting */
    if( rs_ctx == NULL || rs_ctx->rsm == NULL )
#endif
    {
        /* check_privkey is free */
        ECP_BUDGET( ECP_OPS_CHK );

        /* Common sanity checks */
        MPI_CHK( ecp_check_privkey( grp, m ) );
        MPI_CHK( ecp_check_pubkey( grp, P ) );
    }

    ret = ERR_ECP_BAD_INPUT_DATA;

    if( ecp_get_type( grp ) == ECP_TYPE_MONTGOMERY )
        MPI_CHK( ecp_mul_mxz( grp, R, m, P, f_rng, p_rng ) );

    if( ecp_get_type( grp ) == ECP_TYPE_SHORT_WEIERSTRASS )
        MPI_CHK( ecp_mul_comb( grp, R, m, P, f_rng, p_rng, rs_ctx ) );

cleanup:

#if defined(ECP_INTERNAL_ALT)
    if( is_grp_capable )
        internal_ecp_free( grp );
#endif /* ECP_INTERNAL_ALT */

#if defined(ECP_RESTARTABLE)
    if( rs_ctx != NULL )
        rs_ctx->depth--;
#endif

    return( ret );
}

/*
* Multiplication R = m * P
*/
int ecp_mul(ecp_group *grp, ecp_point *R,
	const mpi *m, const ecp_point *P,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	return(ecp_mul_restartable(grp, R, m, P, f_rng, p_rng, NULL));
}

/*
* R = m * P with shortcuts for m == 1 and m == -1
* NOT constant-time - ONLY for short Weierstrass!
*/
static int ecp_mul_shortcuts(ecp_group *grp,
	ecp_point *R,
	const mpi *m,
	const ecp_point *P,
	ecp_restart_ctx *rs_ctx)
{
	int ret;

	if (mpi_cmp_int(m, 1) == 0)
	{
		MPI_CHK(ecp_copy(R, P));
	}
	else if (mpi_cmp_int(m, -1) == 0)
	{
		MPI_CHK(ecp_copy(R, P));
		if (mpi_cmp_int(&R->Y, 0) != 0)
			MPI_CHK(mpi_sub_mpi(&R->Y, &grp->P, &R->Y));
	}
	else
	{
		MPI_CHK(ecp_mul_restartable(grp, R, m, P,
			NULL, NULL, rs_ctx));
	}

cleanup:
	return(ret);
}

/*
* Restartable linear combination
* NOT constant-time
*/
int ecp_muladd_restartable(
	ecp_group *grp, ecp_point *R,
	const mpi *m, const ecp_point *P,
	const mpi *n, const ecp_point *Q,
	ecp_restart_ctx *rs_ctx)
{
	int ret;
	ecp_point mP;
	ecp_point *pmP = &mP;
	ecp_point *pR = R;
#if defined(ECP_INTERNAL_ALT)
	char is_grp_capable = 0;
#endif

	if (ecp_get_type(grp) != ECP_TYPE_SHORT_WEIERSTRASS)
		return(ERR_ECP_FEATURE_UNAVAILABLE);

	ecp_point_init(&mP);

	ECP_RS_ENTER(ma);

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->ma != NULL)
	{
		/* redirect intermediate results to restart context */
		pmP = &rs_ctx->ma->mP;
		pR = &rs_ctx->ma->R;

		/* jump to next operation */
		if (rs_ctx->ma->state == ecp_rsma_mul2)
			goto mul2;
		if (rs_ctx->ma->state == ecp_rsma_add)
			goto add;
		if (rs_ctx->ma->state == ecp_rsma_norm)
			goto norm;
	}
#endif /* ECP_RESTARTABLE */

	MPI_CHK(ecp_mul_shortcuts(grp, pmP, m, P, rs_ctx));
#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->ma != NULL)
		rs_ctx->ma->state = ecp_rsma_mul2;

mul2:
#endif
	MPI_CHK(ecp_mul_shortcuts(grp, pR, n, Q, rs_ctx));

#if defined(ECP_INTERNAL_ALT)
	if ((is_grp_capable = internal_ecp_grp_capable(grp)))
		MPI_CHK(internal_ecp_init(grp));
#endif /* ECP_INTERNAL_ALT */

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->ma != NULL)
		rs_ctx->ma->state = ecp_rsma_add;

add:
#endif
	ECP_BUDGET(ECP_OPS_ADD);
	MPI_CHK(ecp_add_mixed(grp, pR, pmP, pR));
#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->ma != NULL)
		rs_ctx->ma->state = ecp_rsma_norm;

norm:
#endif
	ECP_BUDGET(ECP_OPS_INV);
	MPI_CHK(ecp_normalize_jac(grp, pR));

#if defined(ECP_RESTARTABLE)
	if (rs_ctx != NULL && rs_ctx->ma != NULL)
		MPI_CHK(ecp_copy(R, pR));
#endif

cleanup:
#if defined(ECP_INTERNAL_ALT)
	if (is_grp_capable)
		internal_ecp_free(grp);
#endif /* ECP_INTERNAL_ALT */

	ecp_point_free(&mP);

	ECP_RS_LEAVE(ma);

	return(ret);
}

/*
* Linear combination
* NOT constant-time
*/
int ecp_muladd(ecp_group *grp, ecp_point *R,
	const mpi *m, const ecp_point *P,
	const mpi *n, const ecp_point *Q)
{
	return(ecp_muladd_restartable(grp, R, m, P, n, Q, NULL));
}

/*
* Generate a keypair with configurable base point
*/
int ecp_gen_keypair_base(ecp_group *grp,
	const ecp_point *G,
	mpi *d, ecp_point *Q,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	int ret;

	MPI_CHK(ecp_gen_privkey(grp, d, f_rng, p_rng));
	MPI_CHK(ecp_mul(grp, Q, d, G, f_rng, p_rng));

cleanup:
	return(ret);
}

/*
* Generate key pair, wrapper for conventional base point
*/
int ecp_gen_keypair(ecp_group *grp,
	mpi *d, ecp_point *Q,
	int(*f_rng)(void *, unsigned char *, size_t),
	void *p_rng)
{
	return(ecp_gen_keypair_base(grp, &grp->G, d, Q, f_rng, p_rng));
}

/*
* Generate a keypair, prettier wrapper
*/
int ecp_gen_key(ecp_group_id grp_id, ecp_keypair *key,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;

	if ((ret = ecp_group_load(&key->grp, grp_id)) != 0)
		return(ret);

	return(ecp_gen_keypair(&key->grp, &key->d, &key->Q, f_rng, p_rng));
}

/*
* Check a public-private key pair
*/
int ecp_check_pub_priv(const ecp_keypair *pub, const ecp_keypair *prv)
{
	int ret;
	ecp_point Q;
	ecp_group grp;

	if (pub->grp.id == ECP_DP_NONE ||
		pub->grp.id != prv->grp.id ||
		mpi_cmp_mpi(&pub->Q.X, &prv->Q.X) ||
		mpi_cmp_mpi(&pub->Q.Y, &prv->Q.Y) ||
		mpi_cmp_mpi(&pub->Q.Z, &prv->Q.Z))
	{
		return(ERR_ECP_BAD_INPUT_DATA);
	}

	ecp_point_init(&Q);
	ecp_group_init(&grp);

	/* ecp_mul() needs a non-const group... */
	ecp_group_copy(&grp, &prv->grp);

	/* Also checks d is valid */
	MPI_CHK(ecp_mul(&grp, &Q, &prv->d, &prv->grp.G, NULL, NULL));

	if (mpi_cmp_mpi(&Q.X, &prv->Q.X) ||
		mpi_cmp_mpi(&Q.Y, &prv->Q.Y) ||
		mpi_cmp_mpi(&Q.Z, &prv->Q.Z))
	{
		ret = ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

cleanup:
	ecp_point_free(&Q);
	ecp_group_free(&grp);

	return(ret);
}

#if defined(ECP_SELF_TEST)
/*
* Checkup routine
*/
int ecp_self_test(int verbose)
{
	int ret;
	size_t i;
	ecp_group grp;
	ecp_point R, P;
	mpi m;
	unsigned long add_c_prev, dbl_c_prev, mul_c_prev;
	/* exponents especially adapted for secp192r1 */
	const char *exponents[] =
	{
		"000000000000000000000000000000000000000000000001", /* one */
		"FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22830", /* N - 1 */
		"5EA6F389A38B8BC81E767753B15AA5569E1782E30ABE7D25", /* random */
		"400000000000000000000000000000000000000000000000", /* one and zeros */
		"7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", /* all ones */
		"555555555555555555555555555555555555555555555555", /* 101010... */
	};

	ecp_group_init(&grp);
	ecp_point_init(&R);
	ecp_point_init(&P);
	mpi_init(&m);

	/* Use secp192r1 if available, or any available curve */
#if defined(ECP_DP_SECP192R1_ENABLED)
	MPI_CHK(ecp_group_load(&grp, ECP_DP_SECP192R1));
#else
	MPI_CHK(ecp_group_load(&grp, ecp_curve_list()->grp_id));
#endif

	if (verbose != 0)
		printf("  ECP test #1 (constant op_count, base point G): ");

	/* Do a dummy multiplication first to trigger precomputation */
	MPI_CHK(mpi_lset(&m, 2));
	MPI_CHK(ecp_mul(&grp, &P, &m, &grp.G, NULL, NULL));

	add_count = 0;
	dbl_count = 0;
	mul_count = 0;
	MPI_CHK(mpi_read_string(&m, 16, exponents[0], 48));
	MPI_CHK(ecp_mul(&grp, &R, &m, &grp.G, NULL, NULL));

	for (i = 1; i < sizeof(exponents) / sizeof(exponents[0]); i++)
	{
		add_c_prev = add_count;
		dbl_c_prev = dbl_count;
		mul_c_prev = mul_count;
		add_count = 0;
		dbl_count = 0;
		mul_count = 0;

		MPI_CHK(mpi_read_string(&m, 16, exponents[i], 48));
		MPI_CHK(ecp_mul(&grp, &R, &m, &grp.G, NULL, NULL));

		if (add_count != add_c_prev ||
			dbl_count != dbl_c_prev ||
			mul_count != mul_c_prev)
		{
			if (verbose != 0)
				printf("failed (%u)\n", (unsigned int)i);

			ret = 1;
			goto cleanup;
		}
	}

	if (verbose != 0)
		printf("passed\n");

	if (verbose != 0)
		printf("  ECP test #2 (constant op_count, other point): ");
	/* We computed P = 2G last time, use it */

	add_count = 0;
	dbl_count = 0;
	mul_count = 0;
	MPI_CHK(mpi_read_string(&m, 16, exponents[0], 48));
	MPI_CHK(ecp_mul(&grp, &R, &m, &P, NULL, NULL));

	for (i = 1; i < sizeof(exponents) / sizeof(exponents[0]); i++)
	{
		add_c_prev = add_count;
		dbl_c_prev = dbl_count;
		mul_c_prev = mul_count;
		add_count = 0;
		dbl_count = 0;
		mul_count = 0;

		MPI_CHK(mpi_read_string(&m, 16, exponents[i], 48));
		MPI_CHK(ecp_mul(&grp, &R, &m, &P, NULL, NULL));

		if (add_count != add_c_prev ||
			dbl_count != dbl_c_prev ||
			mul_count != mul_c_prev)
		{
			if (verbose != 0)
				printf("failed (%u)\n", (unsigned int)i);

			ret = 1;
			goto cleanup;
		}
	}

	if (verbose != 0)
		printf("passed\n");

cleanup:

	if (ret < 0 && verbose != 0)
		printf("Unexpected error, return code = %08X\n", ret);

	ecp_group_free(&grp);
	ecp_point_free(&R);
	ecp_point_free(&P);
	mpi_free(&m);

	if (verbose != 0)
		printf("\n");

	return(ret);
}
#endif