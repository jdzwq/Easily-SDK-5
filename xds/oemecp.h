﻿/**
 * \file ecp.h
 *
 * \brief This file provides an API for Elliptic Curves over GF(P) (ECP).
 *
 * The use of ECP in cryptography and TLS is defined in
 * <em>Standards for Efficient Cryptography Group (SECG): SEC1
 * Elliptic Curve Cryptography</em> and
 * <em>RFC-4492: Elliptic Curve Cryptography (ECC) Cipher Suites
 * for Transport Layer Security (TLS)</em>.
 *
 * <em>RFC-2409: The Internet Key Exchange (IKE)</em> defines ECP
 * group types.
 *
 */

/*
 *  Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved
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
 *  This file is part of Mbed TLS (https://tls.mbed.org)
 */

#ifndef ECP_H
#define ECP_H

#include "xdsdef.h"
#include "oemmpi.h"

/*
* ECP error codes
*/
#define ERR_ECP_BAD_INPUT_DATA                    -0x4F80  /**< Bad input parameters to function. */
#define ERR_ECP_BUFFER_TOO_SMALL                  -0x4F00  /**< The buffer is too small to write to. */
#define ERR_ECP_FEATURE_UNAVAILABLE               -0x4E80  /**< The requested feature is not available, for example, the requested curve is not supported. */
#define ERR_ECP_VERIFY_FAILED                     -0x4E00  /**< The signature is not valid. */
#define ERR_ECP_ALLOC_FAILED                      -0x4D80  /**< Memory allocation failed. */
#define ERR_ECP_RANDOM_FAILED                     -0x4D00  /**< Generation of random value, such as ephemeral key, failed. */
#define ERR_ECP_INVALID_KEY                       -0x4C80  /**< Invalid private or public key. */
#define ERR_ECP_SIG_LEN_MISMATCH                  -0x4C00  /**< The buffer contains a valid signature followed by more data. */

/* MBEDTLS_ERR_ECP_HW_ACCEL_FAILED is deprecated and should not be used. */
#define ERR_ECP_HW_ACCEL_FAILED                   -0x4B80  /**< The ECP hardware accelerator failed. */

#define ERR_ECP_IN_PROGRESS                       -0x4B00  /**< Operation in progress, call again with the same parameters to continue. */

/*
* Point formats, from RFC 4492's enum ECPointFormat
*/
#define ECP_PF_UNCOMPRESSED    0   /**< Uncompressed point format. */
#define ECP_PF_COMPRESSED      1   /**< Compressed point format. */

/*
* Some other constants from RFC 4492
*/
#define ECP_TLS_NAMED_CURVE    3   /**< The named_curve of ECCurveType. */

/**
* The maximum size of the groups, that is, of \c N and \c P.
*/
#define ECP_MAX_BITS     521   /**< The maximum size of groups, in bits. */

#define ECP_MAX_BYTES    ( ( ECP_MAX_BITS + 7 ) / 8 )
#define ECP_MAX_PT_LEN   ( 2 * ECP_MAX_BYTES + 1 )

/*
 * Maximum "window" size used for point multiplication.
 * Default: 6.
 * Minimum value: 2. Maximum value: 7.
 *
 * Result is an array of at most ( 1 << ( MBEDTLS_ECP_WINDOW_SIZE - 1 ) )
 * points used for point multiplication. This value is directly tied to EC
 * peak memory usage, so decreasing it by one should roughly cut memory usage
 * by two (if large curves are in use).
 *
 * Reduction in size may reduce speed, but larger curves are impacted first.
 * Sample performances (in ECDHE handshakes/s, with FIXED_POINT_OPTIM = 1):
 *      w-size:     6       5       4       3       2
 *      521       145     141     135     120      97
 *      384       214     209     198     177     146
 *      256       320     320     303     262     226
 *      224       475     475     453     398     342
 *      192       640     640     633     587     476
 */
#define ECP_WINDOW_SIZE    6   /**< The maximum window size used. */

/**
 * Domain-parameter identifiers: curve, subgroup, and generator.
 *
 * \note Only curves over prime fields are supported.
 *
 * \warning This library does not support validation of arbitrary domain
 * parameters. Therefore, only standardized domain parameters from trusted
 * sources should be used. See mbedtls_ecp_group_load().
 */
typedef enum
{
    ECP_DP_NONE = 0,       /*!< Curve not defined. */
    ECP_DP_SECP192R1,      /*!< Domain parameters for the 192-bit curve defined by FIPS 186-4 and SEC1. */
    ECP_DP_SECP224R1,      /*!< Domain parameters for the 224-bit curve defined by FIPS 186-4 and SEC1. */
    ECP_DP_SECP256R1,      /*!< Domain parameters for the 256-bit curve defined by FIPS 186-4 and SEC1. */
    ECP_DP_SECP384R1,      /*!< Domain parameters for the 384-bit curve defined by FIPS 186-4 and SEC1. */
    ECP_DP_SECP521R1,      /*!< Domain parameters for the 521-bit curve defined by FIPS 186-4 and SEC1. */
    ECP_DP_BP256R1,        /*!< Domain parameters for 256-bit Brainpool curve. */
    ECP_DP_BP384R1,        /*!< Domain parameters for 384-bit Brainpool curve. */
    ECP_DP_BP512R1,        /*!< Domain parameters for 512-bit Brainpool curve. */
    ECP_DP_CURVE25519,     /*!< Domain parameters for Curve25519. */
    ECP_DP_SECP192K1,      /*!< Domain parameters for 192-bit "Koblitz" curve. */
    ECP_DP_SECP224K1,      /*!< Domain parameters for 224-bit "Koblitz" curve. */
    ECP_DP_SECP256K1,      /*!< Domain parameters for 256-bit "Koblitz" curve. */
    ECP_DP_CURVE448,       /*!< Domain parameters for Curve448. */
} ecp_group_id;

typedef struct _ecp_point
{
	mpi X;          /*!< The X coordinate of the ECP point. */
	mpi Y;          /*!< The Y coordinate of the ECP point. */
	mpi Z;          /*!< The Z coordinate of the ECP point. */
}ecp_point;


/**
 * Curve information, for use by other modules.
 */
typedef struct _ecp_curve_info
{
    ecp_group_id grp_id;    /*!< An internal identifier. */
    uint16_t tls_id;                /*!< The TLS NamedCurve identifier. */
    uint16_t bit_size;              /*!< The curve size in bits. */
    const char *name;               /*!< A human-friendly name. */
}ecp_curve_info;

/**
 * \brief           The ECP group structure.
 *
 * We consider two types of curve equations:
 * <ul><li>Short Weierstrass: <code>y^2 = x^3 + A x + B mod P</code>
 * (SEC1 + RFC-4492)</li>
 * <li>Montgomery: <code>y^2 = x^3 + A x^2 + x mod P</code> (Curve25519,
 * Curve448)</li></ul>
 * In both cases, the generator (\p G) for a prime-order subgroup is fixed.
 *
 * For Short Weierstrass, this subgroup is the whole curve, and its
 * cardinality is denoted by \p N. Our code requires that \p N is an
 * odd prime as mbedtls_ecp_mul() requires an odd number, and
 * mbedtls_ecdsa_sign() requires that it is prime for blinding purposes.
 *
 * For Montgomery curves, we do not store \p A, but <code>(A + 2) / 4</code>,
 * which is the quantity used in the formulas. Additionally, \p nbits is
 * not the size of \p N but the required size for private keys.
 *
 * If \p modp is NULL, reduction modulo \p P is done using a generic algorithm.
 * Otherwise, \p modp must point to a function that takes an \p mbedtls_mpi in the
 * range of <code>0..2^(2*pbits)-1</code>, and transforms it in-place to an integer
 * which is congruent mod \p P to the given MPI, and is close enough to \p pbits
 * in size, so that it may be efficiently brought in the 0..P-1 range by a few
 * additions or subtractions. Therefore, it is only an approximative modular
 * reduction. It must return 0 on success and non-zero on failure.
 *
 * \note        Alternative implementations must keep the group IDs distinct. If
 *              two group structures have the same ID, then they must be
 *              identical.
 *
 */
typedef struct _ecp_group
{
    ecp_group_id id;    /*!< An internal group identifier. */
    mpi P;              /*!< The prime modulus of the base field. */
    mpi A;              /*!< For Short Weierstrass: \p A in the equation. For Montgomery curves: <code>(A + 2) / 4</code>. */
    mpi B;              /*!< For Short Weierstrass: \p B in the equation. For Montgomery curves: unused. */
    ecp_point G;        /*!< The generator of the subgroup used. */
    mpi N;              /*!< The order of \p G. */
    size_t pbits;               /*!< The number of bits in \p P.*/
    size_t nbits;               /*!< For Short Weierstrass: The number of bits in \p P. For Montgomery curves: the number of bits in the private keys. */
    unsigned int h;             /*!< \internal 1 if the constants are static. */
    int (*modp)(mpi *); /*!< The function for fast pseudo-reduction mod \p P (see above).*/
    int (*t_pre)(ecp_point *, void *);  /*!< Unused. */
    int (*t_post)(ecp_point *, void *); /*!< Unused. */
    void *t_data;               /*!< Unused. */
    ecp_point *T;       /*!< Pre-computed points for ecp_mul_comb(). */
    size_t T_size;              /*!< The number of pre-computed points. */
}ecp_group;

/* We want to declare restartable versions of existing functions anyway */
typedef void ecp_restart_ctx;
#define ECP_BUDGET( ops )   /* no-op; for compatibility */
#define ECP_RS_ENTER( sub )     (void) rs_ctx;
#define ECP_RS_LEAVE( sub )     (void) rs_ctx;

#define ECP_FIXED_POINT_OPTIM  1   /**< Enable fixed-point speed-up. */

/**
* \brief    The ECP key-pair structure.
*
* A generic key-pair that may be used for ECDSA and fixed ECDH, for example.
*
* \note    Members are deliberately in the same order as in the
*          ::mbedtls_ecdsa_context structure.
*/
typedef struct ecp_keypair
{
	ecp_group grp;      /*!<  Elliptic curve and base point     */
	mpi d;              /*!<  our secret value                  */
	ecp_point Q;        /*!<  our public value                  */
}
ecp_keypair;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* \brief           This function initializes a point as zero.
	*
	* \param pt        The point to initialize.
	*/
	EXP_API void ecp_point_init(ecp_point *pt);

	/**
	* \brief           This function frees the components of a point.
	*
	* \param pt        The point to free.
	*/
	EXP_API void ecp_point_free(ecp_point *pt);

	/**
	* \brief           This function copies the contents of point \p Q into
	*                  point \p P.
	*
	* \param P         The destination point. This must be initialized.
	* \param Q         The source point. This must be initialized.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
	* \return          Another negative error code for other kinds of failure.
	*/
	EXP_API int ecp_copy(ecp_point *P, const ecp_point *Q);

	/**
	* \brief           This function sets a point to the point at infinity.
	*
	* \param pt        The point to set. This must be initialized.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_set_zero(ecp_point *pt);

	/**
	* \brief           This function checks if a point is the point at infinity.
	*
	* \param pt        The point to test. This must be initialized.
	*
	* \return          \c 1 if the point is zero.
	* \return          \c 0 if the point is non-zero.
	* \return          A negative error code on failure.
	*/
	EXP_API int ecp_is_zero(ecp_point *pt);

	/**
	* \brief           This function compares two points.
	*
	* \note            This assumes that the points are normalized. Otherwise,
	*                  they may compare as "not equal" even if they are.
	*
	* \param P         The first point to compare. This must be initialized.
	* \param Q         The second point to compare. This must be initialized.
	*
	* \return          \c 0 if the points are equal.
	* \return          -1 if the points are not equal.
	*/
	EXP_API int ecp_point_cmp(const ecp_point *P, const ecp_point *Q);

	/**
	* \brief           This function imports a non-zero point from two ASCII
	*                  strings.
	*
	* \param P         The destination point. This must be initialized.
	* \param radix     The numeric base of the input.
	* \param x         The first affine coordinate, as a null-terminated string.
	* \param y         The second affine coordinate, as a null-terminated string.
	*
	* \return          \c 0 on success.
	* \return          An \c -1 error code on failure.
	*/
	EXP_API int ecp_point_read_string(ecp_point *P, int radix, const char *x, const char *y);

	/**
	* \brief           This function retrieves the information defined in
	*                  mbedtls_ecp_curve_info() for all supported curves in order
	*                  of preference.
	*
	* \return          A statically allocated array. The last entry is 0.
	*/
	EXP_API const ecp_curve_info *ecp_curve_list(void);

	/**
	* \brief           This function retrieves the list of internal group
	*                  identifiers of all supported curves in the order of
	*                  preference.
	*
	* \return          A statically allocated array,
	*                  terminated with MBEDTLS_ECP_DP_NONE.
	*/
	EXP_API const ecp_group_id *ecp_grp_id_list(void);

	/**
	* \brief           This function retrieves curve information from an internal
	*                  group identifier.
	*
	* \param grp_id    An \c MBEDTLS_ECP_DP_XXX value.
	*
	* \return          The associated curve information on success.
	* \return          NULL on failure.
	*/
	EXP_API const ecp_curve_info *ecp_curve_info_from_grp_id(ecp_group_id grp_id);

	/**
	* \brief           This function retrieves curve information from a TLS
	*                  NamedCurve value.
	*
	* \param tls_id    An \c MBEDTLS_ECP_DP_XXX value.
	*
	* \return          The associated curve information on success.
	* \return          NULL on failure.
	*/
	EXP_API const ecp_curve_info *ecp_curve_info_from_tls_id(uint16_t tls_id);

	/**
	* \brief           This function retrieves curve information from a
	*                  human-readable name.
	*
	* \param name      The human-readable name.
	*
	* \return          The associated curve information on success.
	* \return          NULL on failure.
	*/
	EXP_API const ecp_curve_info *ecp_curve_info_from_name(const char *name);

	/**
	* \brief           This function initializes an ECP group context
	*                  without loading any domain parameters.
	*
	* \note            After this function is called, domain parameters
	*                  for various ECP groups can be loaded through the
	*                  mbedtls_ecp_group_load() or mbedtls_ecp_tls_read_group()
	*                  functions.
	*/
	EXP_API void ecp_group_init(ecp_group *grp);

	/**
	* \brief           This function frees the components of an ECP group.
	*
	* \param grp       The group to free. This may be \c NULL, in which
	*                  case this function returns immediately. If it is not
	*                  \c NULL, it must point to an initialized ECP group.
	*/
	EXP_API void ecp_group_free(ecp_group *grp);

	/**
	* \brief           This function copies the contents of group \p src into
	*                  group \p dst.
	*
	* \param dst       The destination group. This must be initialized.
	* \param src       The source group. This must be initialized.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_group_copy(ecp_group *dst, const ecp_group *src);

	/**
	* \brief           This function sets up an ECP group context
	*                  from a standardized set of domain parameters.
	*
	* \note            The index should be a value of the NamedCurve enum,
	*                  as defined in <em>RFC-4492: Elliptic Curve Cryptography
	*                  (ECC) Cipher Suites for Transport Layer Security (TLS)</em>,
	*                  usually in the form of an \c MBEDTLS_ECP_DP_XXX macro.
	*
	* \param grp       The group context to setup. This must be initialized.
	* \param id        The identifier of the domain parameter set to load.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE if \p id doesn't
	*                  correspond to a known group.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_group_load(ecp_group *grp, ecp_group_id id);


	/**
	* \brief           This function exports a point into unsigned binary data.
	*
	* \param grp       The group to which the point should belong.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param P         The point to export. This must be initialized.
	* \param format    The point format. This must be either
	*                  #MBEDTLS_ECP_PF_COMPRESSED or #MBEDTLS_ECP_PF_UNCOMPRESSED.
	* \param olen      The address at which to store the length of
	*                  the output in Bytes. This must not be \c NULL.
	* \param buf       The output buffer. This must be a writable buffer
	*                  of length \p buflen Bytes.
	* \param buflen    The length of the output buffer \p buf in Bytes.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL if the output buffer
	*                  is too small to hold the point.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_point_write_binary(const ecp_group *grp, const ecp_point *P,
		int format, size_t *olen,
		unsigned char *buf, size_t buflen);

	/**
	* \brief           This function imports a point from unsigned binary data.
	*
	* \note            This function does not check that the point actually
	*                  belongs to the given group, see mbedtls_ecp_check_pubkey()
	*                  for that.
	*
	* \param grp       The group to which the point should belong.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param P         The destination context to import the point to.
	*                  This must be initialized.
	* \param buf       The input buffer. This must be a readable buffer
	*                  of length \p ilen Bytes.
	* \param ilen      The length of the input buffer \p buf in Bytes.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if the input is invalid.
	* \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
	* \return          #MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE if the point format
	*                  is not implemented.
	*/
	EXP_API int ecp_point_read_binary(const ecp_group *grp,
		ecp_point *P,
		const unsigned char *buf, size_t ilen);

	/**
	* \brief           This function imports a point from a TLS ECPoint record.
	*
	* \note            On function return, \p *buf is updated to point immediately
	*                  after the ECPoint record.
	*
	* \param grp       The ECP group to use.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param pt        The destination point.
	* \param buf       The address of the pointer to the start of the input buffer.
	* \param len       The length of the buffer.
	*
	* \return          \c 0 on success.
	* \return          An \c MBEDTLS_ERR_MPI_XXX error code on initialization
	*                  failure.
	* \return          #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if input is invalid.
	*/
	EXP_API int ecp_tls_read_point(const ecp_group *grp,
		ecp_point *pt,
		const unsigned char **buf, size_t len);

	/**
	* \brief           This function exports a point as a TLS ECPoint record
	*                  defined in RFC 4492, Section 5.4.
	*
	* \param grp       The ECP group to use.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param pt        The point to be exported. This must be initialized.
	* \param format    The point format to use. This must be either
	*                  #MBEDTLS_ECP_PF_COMPRESSED or #MBEDTLS_ECP_PF_UNCOMPRESSED.
	* \param olen      The address at which to store the length in Bytes
	*                  of the data written.
	* \param buf       The target buffer. This must be a writable buffer of
	*                  length \p blen Bytes.
	* \param blen      The length of the target buffer \p buf in Bytes.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if the input is invalid.
	* \return          #MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL if the target buffer
	*                  is too small to hold the exported point.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_tls_write_point(const ecp_group *grp,
		const ecp_point *pt,
		int format, size_t *olen,
		unsigned char *buf, size_t blen);

	/**
	* \brief           This function extracts an elliptic curve group ID from a
	*                  TLS ECParameters record as defined in RFC 4492, Section 5.4.
	*
	* \note            The read pointer \p buf is updated to point right after
	*                  the ECParameters record on exit.
	*
	* \param grp       The address at which to store the group id.
	*                  This must not be \c NULL.
	* \param buf       The address of the pointer to the start of the input buffer.
	* \param len       The length of the input buffer \c *buf in Bytes.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if input is invalid.
	* \return          #MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE if the group is not
	*                  recognized.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_tls_read_group_id(ecp_group_id *grp,
		const unsigned char **buf,
		size_t len);

	/**
	* \brief           This function sets up an ECP group context from a TLS
	*                  ECParameters record as defined in RFC 4492, Section 5.4.
	*
	* \note            The read pointer \p buf is updated to point right after
	*                  the ECParameters record on exit.
	*
	* \param grp       The group context to setup. This must be initialized.
	* \param buf       The address of the pointer to the start of the input buffer.
	* \param len       The length of the input buffer \c *buf in Bytes.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if input is invalid.
	* \return          #MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE if the group is not
	*                  recognized.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_tls_read_group(ecp_group *grp,
		const unsigned char **buf, size_t len);

	/**
	* \brief           This function exports an elliptic curve as a TLS
	*                  ECParameters record as defined in RFC 4492, Section 5.4.
	*
	* \param grp       The ECP group to be exported.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param olen      The address at which to store the number of Bytes written.
	*                  This must not be \c NULL.
	* \param buf       The buffer to write to. This must be a writable buffer
	*                  of length \p blen Bytes.
	* \param blen      The length of the output buffer \p buf in Bytes.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL if the output
	*                  buffer is too small to hold the exported group.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_tls_write_group(const ecp_group *grp,
		size_t *olen,
		unsigned char *buf, size_t blen);

	/**
	* \brief           This function performs a scalar multiplication of a point
	*                  by an integer: \p R = \p m * \p P.
	*
	*                  It is not thread-safe to use same group in multiple threads.
	*
	* \note            To prevent timing attacks, this function
	*                  executes the exact same sequence of base-field
	*                  operations for any valid \p m. It avoids any if-branch or
	*                  array index depending on the value of \p m.
	*
	* \note            If \p f_rng is not NULL, it is used to randomize
	*                  intermediate results to prevent potential timing attacks
	*                  targeting these results. We recommend always providing
	*                  a non-NULL \p f_rng. The overhead is negligible.
	*
	* \param grp       The ECP group to use.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param R         The point in which to store the result of the calculation.
	*                  This must be initialized.
	* \param m         The integer by which to multiply. This must be initialized.
	* \param P         The point to multiply. This must be initialized.
	* \param f_rng     The RNG function. This may be \c NULL if randomization
	*                  of intermediate results isn't desired (discouraged).
	* \param p_rng     The RNG context to be passed to \p p_rng.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_INVALID_KEY if \p m is not a valid private
	*                  key, or \p P is not a valid public key.
	* \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_mul(ecp_group *grp, ecp_point *R,
		const mpi *m, const ecp_point *P,
		int(*f_rng)(void *, unsigned char *, size_t), void *p_rng);

	/**
	* \brief           This function performs multiplication of a point by
	*                  an integer: \p R = \p m * \p P in a restartable way.
	*
	* \see             mbedtls_ecp_mul()
	*
	* \note            This function does the same as \c mbedtls_ecp_mul(), but
	*                  it can return early and restart according to the limit set
	*                  with \c mbedtls_ecp_set_max_ops() to reduce blocking.
	*
	* \param grp       The ECP group to use.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param R         The point in which to store the result of the calculation.
	*                  This must be initialized.
	* \param m         The integer by which to multiply. This must be initialized.
	* \param P         The point to multiply. This must be initialized.
	* \param f_rng     The RNG function. This may be \c NULL if randomization
	*                  of intermediate results isn't desired (discouraged).
	* \param p_rng     The RNG context to be passed to \p p_rng.
	* \param rs_ctx    The restart context (NULL disables restart).
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_INVALID_KEY if \p m is not a valid private
	*                  key, or \p P is not a valid public key.
	* \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
	* \return          #MBEDTLS_ERR_ECP_IN_PROGRESS if maximum number of
	*                  operations was reached: see \c mbedtls_ecp_set_max_ops().
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_mul_restartable(ecp_group *grp, ecp_point *R,
		const mpi *m, const ecp_point *P,
		int(*f_rng)(void *, unsigned char *, size_t), void *p_rng,
		ecp_restart_ctx *rs_ctx);

	/**
	* \brief           This function performs multiplication and addition of two
	*                  points by integers: \p R = \p m * \p P + \p n * \p Q in a
	*                  restartable way.
	*
	* \see             \c mbedtls_ecp_muladd()
	*
	* \note            This function works the same as \c mbedtls_ecp_muladd(),
	*                  but it can return early and restart according to the limit
	*                  set with \c mbedtls_ecp_set_max_ops() to reduce blocking.
	*
	* \param grp       The ECP group to use.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param R         The point in which to store the result of the calculation.
	*                  This must be initialized.
	* \param m         The integer by which to multiply \p P.
	*                  This must be initialized.
	* \param P         The point to multiply by \p m. This must be initialized.
	* \param n         The integer by which to multiply \p Q.
	*                  This must be initialized.
	* \param Q         The point to be multiplied by \p n.
	*                  This must be initialized.
	* \param rs_ctx    The restart context (NULL disables restart).
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_INVALID_KEY if \p m or \p n are not
	*                  valid private keys, or \p P or \p Q are not valid public
	*                  keys.
	* \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
	* \return          #MBEDTLS_ERR_ECP_IN_PROGRESS if maximum number of
	*                  operations was reached: see \c mbedtls_ecp_set_max_ops().
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_muladd_restartable(
		ecp_group *grp, ecp_point *R,
		const mpi *m, const ecp_point *P,
		const mpi *n, const ecp_point *Q,
		ecp_restart_ctx *rs_ctx);


	/**
	* \brief           This function performs multiplication and addition of two
	*                  points by integers: \p R = \p m * \p P + \p n * \p Q
	*
	*                  It is not thread-safe to use same group in multiple threads.
	*
	* \note            In contrast to mbedtls_ecp_mul(), this function does not
	*                  guarantee a constant execution flow and timing.
	*
	* \param grp       The ECP group to use.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param R         The point in which to store the result of the calculation.
	*                  This must be initialized.
	* \param m         The integer by which to multiply \p P.
	*                  This must be initialized.
	* \param P         The point to multiply by \p m. This must be initialized.
	* \param n         The integer by which to multiply \p Q.
	*                  This must be initialized.
	* \param Q         The point to be multiplied by \p n.
	*                  This must be initialized.
	*
	* \return          \c 0 on success.
	* \return          #MBEDTLS_ERR_ECP_INVALID_KEY if \p m or \p n are not
	*                  valid private keys, or \p P or \p Q are not valid public
	*                  keys.
	* \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_muladd(ecp_group *grp, ecp_point *R,
		const mpi *m, const ecp_point *P,
		const mpi *n, const ecp_point *Q);

	/**
	* \brief           This function checks that a point is a valid public key
	*                  on this curve.
	*
	*                  It only checks that the point is non-zero, has
	*                  valid coordinates and lies on the curve. It does not verify
	*                  that it is indeed a multiple of \p G. This additional
	*                  check is computationally more expensive, is not required
	*                  by standards, and should not be necessary if the group
	*                  used has a small cofactor. In particular, it is useless for
	*                  the NIST groups which all have a cofactor of 1.
	*
	* \note            This function uses bare components rather than an
	*                  ::mbedtls_ecp_keypair structure, to ease use with other
	*                  structures, such as ::mbedtls_ecdh_context or
	*                  ::mbedtls_ecdsa_context.
	*
	* \param grp       The ECP group the point should belong to.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param pt        The point to check. This must be initialized.
	*
	* \return          \c 0 if the point is a valid public key.
	* \return          #MBEDTLS_ERR_ECP_INVALID_KEY if the point is not
	*                  a valid public key for the given curve.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_check_pubkey(const ecp_group *grp,
		const ecp_point *pt);

	/**
	* \brief           This function checks that an \p mbedtls_mpi is a
	*                  valid private key for this curve.
	*
	* \note            This function uses bare components rather than an
	*                  ::mbedtls_ecp_keypair structure to ease use with other
	*                  structures, such as ::mbedtls_ecdh_context or
	*                  ::mbedtls_ecdsa_context.
	*
	* \param grp       The ECP group the private key should belong to.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param d         The integer to check. This must be initialized.
	*
	* \return          \c 0 if the point is a valid private key.
	* \return          #MBEDTLS_ERR_ECP_INVALID_KEY if the point is not a valid
	*                  private key for the given curve.
	* \return          Another negative error code on other kinds of failure.
	*/
	EXP_API int ecp_check_privkey(const ecp_group *grp,
		const mpi *d);

	/**
	* \brief           This function generates a private key.
	*
	* \param grp       The ECP group to generate a private key for.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param d         The destination MPI (secret part). This must be initialized.
	* \param f_rng     The RNG function. This must not be \c NULL.
	* \param p_rng     The RNG parameter to be passed to \p f_rng. This may be
	*                  \c NULL if \p f_rng doesn't need a context argument.
	*
	* \return          \c 0 on success.
	* \return          An \c MBEDTLS_ERR_ECP_XXX or \c MBEDTLS_MPI_XXX error code
	*                  on failure.
	*/
	EXP_API int ecp_gen_privkey(const ecp_group *grp,
		mpi *d,
		int(*f_rng)(void *, unsigned char *, size_t),
		void *p_rng);

	/**
	* \brief           This function generates a keypair with a configurable base
	*                  point.
	*
	* \note            This function uses bare components rather than an
	*                  ::mbedtls_ecp_keypair structure to ease use with other
	*                  structures, such as ::mbedtls_ecdh_context or
	*                  ::mbedtls_ecdsa_context.
	*
	* \param grp       The ECP group to generate a key pair for.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param G         The base point to use. This must be initialized
	*                  and belong to \p grp. It replaces the default base
	*                  point \c grp->G used by mbedtls_ecp_gen_keypair().
	* \param d         The destination MPI (secret part).
	*                  This must be initialized.
	* \param Q         The destination point (public part).
	*                  This must be initialized.
	* \param f_rng     The RNG function. This must not be \c NULL.
	* \param p_rng     The RNG context to be passed to \p f_rng. This may
	*                  be \c NULL if \p f_rng doesn't need a context argument.
	*
	* \return          \c 0 on success.
	* \return          An \c MBEDTLS_ERR_ECP_XXX or \c MBEDTLS_MPI_XXX error code
	*                  on failure.
	*/
	EXP_API int ecp_gen_keypair_base(ecp_group *grp,
		const ecp_point *G,
		mpi *d, ecp_point *Q,
		int(*f_rng)(void *, unsigned char *, size_t),
		void *p_rng);

	/**
	* \brief           This function generates an ECP keypair.
	*
	* \note            This function uses bare components rather than an
	*                  ::mbedtls_ecp_keypair structure to ease use with other
	*                  structures, such as ::mbedtls_ecdh_context or
	*                  ::mbedtls_ecdsa_context.
	*
	* \param grp       The ECP group to generate a key pair for.
	*                  This must be initialized and have group parameters
	*                  set, for example through mbedtls_ecp_group_load().
	* \param d         The destination MPI (secret part).
	*                  This must be initialized.
	* \param Q         The destination point (public part).
	*                  This must be initialized.
	* \param f_rng     The RNG function. This must not be \c NULL.
	* \param p_rng     The RNG context to be passed to \p f_rng. This may
	*                  be \c NULL if \p f_rng doesn't need a context argument.
	*
	* \return          \c 0 on success.
	* \return          An \c MBEDTLS_ERR_ECP_XXX or \c MBEDTLS_MPI_XXX error code
	*                  on failure.
	*/
	EXP_API int ecp_gen_keypair(ecp_group *grp, mpi *d,
		ecp_point *Q,
		int(*f_rng)(void *, unsigned char *, size_t),
		void *p_rng);

	/**
	* \brief           This function generates an ECP key.
	*
	* \param grp_id    The ECP group identifier.
	* \param key       The destination key. This must be initialized.
	* \param f_rng     The RNG function to use. This must not be \c NULL.
	* \param p_rng     The RNG context to be passed to \p f_rng. This may
	*                  be \c NULL if \p f_rng doesn't need a context argument.
	*
	* \return          \c 0 on success.
	* \return          An \c MBEDTLS_ERR_ECP_XXX or \c MBEDTLS_MPI_XXX error code
	*                  on failure.
	*/
	EXP_API int ecp_gen_key(ecp_group_id grp_id, ecp_keypair *key,
		int(*f_rng)(void *, unsigned char *, size_t),
		void *p_rng);

	/**
	* \brief           This function checks that the keypair objects
	*                  \p pub and \p prv have the same group and the
	*                  same public point, and that the private key in
	*                  \p prv is consistent with the public key.
	*
	* \param pub       The keypair structure holding the public key. This
	*                  must be initialized. If it contains a private key, that
	*                  part is ignored.
	* \param prv       The keypair structure holding the full keypair.
	*                  This must be initialized.
	*
	* \return          \c 0 on success, meaning that the keys are valid and match.
	* \return          #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if the keys are invalid or do not match.
	* \return          An \c MBEDTLS_ERR_ECP_XXX or an \c MBEDTLS_ERR_MPI_XXX
	*                  error code on calculation failure.
	*/
	EXP_API int ecp_check_pub_priv(const ecp_keypair *pub,
		const ecp_keypair *prv);

#if defined(DEBUG) || defined(_DEBUG)
#define ECP_SELF_TEST
	/**
	* \brief          The ECP checkup routine.
	*
	* \return         \c 0 on success.
	* \return         \c 1 on failure.
	*/
	EXP_API int ecp_self_test(int verbose);
#endif

#ifdef __cplusplus
}
#endif


#endif

