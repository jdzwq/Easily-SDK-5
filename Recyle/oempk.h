/**
 * \file pk.h
 *
 * \brief Public Key abstraction layer
 */
/*
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

#ifndef PK_H
#define PK_H

#include "xdsdef.h"
#include "oemmd.h"
#include "oemrsa.h"
#include "oemecp.h"

#define ERR_PK_ALLOC_FAILED        -0x3F80  /**< Memory allocation failed. */
#define ERR_PK_TYPE_MISMATCH       -0x3F00  /**< Type mismatch, eg attempt to encrypt with an ECDSA key */
#define ERR_PK_BAD_INPUT_DATA      -0x3E80  /**< Bad input parameters to function. */
#define ERR_PK_FILE_IO_ERROR       -0x3E00  /**< Read/write of file failed. */
#define ERR_PK_KEY_INVALID_VERSION -0x3D80  /**< Unsupported key version */
#define ERR_PK_KEY_INVALID_FORMAT  -0x3D00  /**< Invalid key tag or value. */
#define ERR_PK_UNKNOWN_PK_ALG      -0x3C80  /**< Key algorithm is unsupported (only RSA and EC are supported). */
#define ERR_PK_PASSWORD_REQUIRED   -0x3C00  /**< Private key password can't be empty. */
#define ERR_PK_PASSWORD_MISMATCH   -0x3B80  /**< Given private key password does not allow for correct decryption. */
#define ERR_PK_INVALID_PUBKEY      -0x3B00  /**< The pubkey tag or value is invalid (only RSA and EC are supported). */
#define ERR_PK_INVALID_ALG         -0x3A80  /**< The algorithm tag or value is invalid. */
#define ERR_PK_UNKNOWN_NAMED_CURVE -0x3A00  /**< Elliptic curve is unsupported (only NIST curves are supported). */
#define ERR_PK_FEATURE_UNAVAILABLE -0x3980  /**< Unavailable feature, e.g. RSA disabled for RSA key. */
#define ERR_PK_SIG_LEN_MISMATCH    -0x3900  /**< The buffer contains a valid signature followed by more data. */

/* ERR_PK_HW_ACCEL_FAILED is deprecated and should not be used. */
#define ERR_PK_HW_ACCEL_FAILED     -0x3880  /**< PK hardware accelerator failed. */

/**
* \def PK_RSA_ALT_SUPPORT
*
* Support external private RSA keys (eg from a HSM) in the PK layer.
*
* Comment this macro to disable support for external private RSA keys.
*/
#define PK_RSA_ALT_SUPPORT

#if defined(PK_RSA_ALT_SUPPORT)
/**
* \brief           Types for RSA-alt abstraction
*/
typedef int(*pk_rsa_alt_decrypt_func)(void *ctx, int mode, size_t *olen,
	const unsigned char *input, unsigned char *output,
	size_t output_max_len);
typedef int(*pk_rsa_alt_sign_func)(void *ctx,
	int(*f_rng)(void *, unsigned char *, size_t), void *p_rng,
	int mode, md_type_t md_alg, unsigned int hashlen,
	const unsigned char *hash, unsigned char *sig);
typedef size_t(*pk_rsa_alt_key_len_func)(void *ctx);
#endif

/**
* \brief          Public key types
*/
typedef enum {
	PK_NONE = 0,
	PK_RSA,
	PK_ECKEY,
	PK_ECKEY_DH,
	PK_ECDSA,
	PK_RSA_ALT,
	PK_RSASSA_PSS,
} pk_type_t;

/**
* \brief           Options for RSASSA-PSS signature verification.
*                  See \c rsa_rsassa_pss_verify_ext()
*/
typedef struct pk_rsassa_pss_options
{
	md_type_t mgf1_hash_id;
	int expected_salt_len;

} pk_rsassa_pss_options;

/**
* \brief           Types for interfacing with the debug module
*/
typedef enum
{
	PK_DEBUG_NONE = 0,
	PK_DEBUG_MPI,
	PK_DEBUG_ECP,
} pk_debug_type;

/**
* \brief           Item to send to the debug module
*/
typedef struct pk_debug_item
{
	pk_debug_type type;
	const char *name;
	void *value;
} pk_debug_item;

/** Maximum number of item send for debugging, plus 1 */
#define PK_DEBUG_MAX_ITEMS 3

typedef struct _pk_info_t
{
	/** Public key type */
	pk_type_t type;

	/** Type name */
	const char *name;

	/** Get key size in bits */
	size_t(*get_bitlen)(const void *);

	/** Tell if the context implements this type (e.g. ECKEY can do ECDSA) */
	int(*can_do)(pk_type_t type);

	/** Verify signature */
	int(*verify_func)(void *ctx, md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		const unsigned char *sig, size_t sig_len);

	/** Make signature */
	int(*sign_func)(void *ctx, md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		unsigned char *sig, size_t *sig_len,
		int(*f_rng)(void *, unsigned char *, size_t),
		void *p_rng);

#if defined(ECP_RESTARTABLE)
	/** Verify signature (restartable) */
	int(*verify_rs_func)(void *ctx, md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		const unsigned char *sig, size_t sig_len,
		void *rs_ctx);

	/** Make signature (restartable) */
	int(*sign_rs_func)(void *ctx, md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		unsigned char *sig, size_t *sig_len,
		int(*f_rng)(void *, unsigned char *, size_t),
		void *p_rng, void *rs_ctx);
#endif /* ECDSA_C && ECP_RESTARTABLE */

	/** Decrypt message */
	int(*decrypt_func)(void *ctx, const unsigned char *input, size_t ilen,
		unsigned char *output, size_t *olen, size_t osize,
		int(*f_rng)(void *, unsigned char *, size_t),
		void *p_rng);

	/** Encrypt message */
	int(*encrypt_func)(void *ctx, const unsigned char *input, size_t ilen,
		unsigned char *output, size_t *olen, size_t osize,
		int(*f_rng)(void *, unsigned char *, size_t),
		void *p_rng);

	/** Check public-private key pair */
	int(*check_pair_func)(const void *pub, const void *prv);

	/** Allocate a new context */
	void * (*ctx_alloc_func)(void);

	/** Free the given context */
	void(*ctx_free_func)(void *ctx);

#if defined(ECP_RESTARTABLE)
	/** Allocate the restart context */
	void * (*rs_alloc_func)(void);

	/** Free the restart context */
	void(*rs_free_func)(void *rs_ctx);
#endif /* ECDSA_C && ECP_RESTARTABLE */

	/** Interface with the debug module */
	void(*debug_func)(const void *ctx, pk_debug_item *items);

}pk_info_t;

#if defined(PK_RSA_ALT_SUPPORT)
/* Container for RSA-alt */
typedef struct
{
	void *key;
	pk_rsa_alt_decrypt_func decrypt_func;
	pk_rsa_alt_sign_func sign_func;
	pk_rsa_alt_key_len_func key_len_func;
} rsa_alt_context;
#endif

/**
* \brief           Public key container
*/
typedef struct pk_context
{
	const pk_info_t *   pk_info; /**< Public key information         */
	void *                      pk_ctx;  /**< Underlying public key context  */
} pk_context;

#if defined(ECP_RESTARTABLE)
/**
* \brief           Context for resuming operations
*/
typedef struct
{
	const pk_info_t *   pk_info; /**< Public key information         */
	void *                      rs_ctx;  /**< Underlying restart context     */
} pk_restart_ctx;
#else /* ECDSA_C && ECP_RESTARTABLE */
/* Now we can declare functions that take a pointer to that */
typedef void pk_restart_ctx;
#endif /* ECDSA_C && ECP_RESTARTABLE */

#ifdef __cplusplus
extern "C" {
#endif

	extern const pk_info_t rsa_info;

	extern const pk_info_t eckey_info;
	extern const pk_info_t eckeydh_info;

	extern const pk_info_t ecdsa_info;

#if defined(PK_RSA_ALT_SUPPORT)
	extern const pk_info_t rsa_alt_info;
#endif

	/**
	* Quick access to an RSA context inside a PK context.
	*
	* \warning You must make sure the PK context actually holds an RSA context
	* before using this function!
	*/
	static rsa_context *pk_rsa(const pk_context pk)
	{
		return((rsa_context *)(pk).pk_ctx);
	}

	/**
	* Quick access to an EC context inside a PK context.
	*
	* \warning You must make sure the PK context actually holds an EC context
	* before using this function!
	*/
	static ecp_keypair *pk_ec(const pk_context pk)
	{
		return((ecp_keypair *)(pk).pk_ctx);
	}

	/**
	* \brief           Return information associated with the given PK type
	*
	* \param pk_type   PK type to search for.
	*
	* \return          The PK info associated with the type or NULL if not found.
	*/
	const pk_info_t *pk_info_from_type(pk_type_t pk_type);

	/**
	* \brief           Initialize a #pk_context (as NONE).
	*
	* \param ctx       The context to initialize.
	*                  This must not be \c NULL.
	*/
	void pk_init(pk_context *ctx);

	/**
	* \brief           Free the components of a #pk_context.
	*
	* \param ctx       The context to clear. It must have been initialized.
	*                  If this is \c NULL, this function does nothing.
	*/
	void pk_free(pk_context *ctx);

#if defined(ECP_RESTARTABLE)
	/**
	* \brief           Initialize a restart context
	*
	* \param ctx       The context to initialize.
	*                  This must not be \c NULL.
	*/
	void pk_restart_init(pk_restart_ctx *ctx);

	/**
	* \brief           Free the components of a restart context
	*
	* \param ctx       The context to clear. It must have been initialized.
	*                  If this is \c NULL, this function does nothing.
	*/
	void pk_restart_free(pk_restart_ctx *ctx);
#endif /* ECDSA_C && ECP_RESTARTABLE */

	/**
	* \brief           Initialize a PK context with the information given
	*                  and allocates the type-specific PK subcontext.
	*
	* \param ctx       Context to initialize. It must not have been set
	*                  up yet (type #PK_NONE).
	* \param info      Information to use
	*
	* \return          0 on success,
	*                  ERR_PK_BAD_INPUT_DATA on invalid input,
	*                  ERR_PK_ALLOC_FAILED on allocation failure.
	*
	* \note            For contexts holding an RSA-alt key, use
	*                  \c pk_setup_rsa_alt() instead.
	*/
	int pk_setup(pk_context *ctx, const pk_info_t *info);

#if defined(PK_RSA_ALT_SUPPORT)
	/**
	* \brief           Initialize an RSA-alt context
	*
	* \param ctx       Context to initialize. It must not have been set
	*                  up yet (type #PK_NONE).
	* \param key       RSA key pointer
	* \param decrypt_func  Decryption function
	* \param sign_func     Signing function
	* \param key_len_func  Function returning key length in bytes
	*
	* \return          0 on success, or ERR_PK_BAD_INPUT_DATA if the
	*                  context wasn't already initialized as RSA_ALT.
	*
	* \note            This function replaces \c pk_setup() for RSA-alt.
	*/
	int pk_setup_rsa_alt(pk_context *ctx, void * key,
		pk_rsa_alt_decrypt_func decrypt_func,
		pk_rsa_alt_sign_func sign_func,
		pk_rsa_alt_key_len_func key_len_func);
#endif

	/**
	* \brief           Get the size in bits of the underlying key
	*
	* \param ctx       The context to query. It must have been initialized.
	*
	* \return          Key size in bits, or 0 on error
	*/
	size_t pk_get_bitlen(const pk_context *ctx);

	/**
	* \brief           Get the length in bytes of the underlying key
	*
	* \param ctx       The context to query. It must have been initialized.
	*
	* \return          Key length in bytes, or 0 on error
	*/
	static size_t pk_get_len(const pk_context *ctx)
	{
		return((pk_get_bitlen(ctx) + 7) / 8);
	}

	/**
	* \brief           Tell if a context can do the operation given by type
	*
	* \param ctx       The context to query. It must have been initialized.
	* \param type      The desired type.
	*
	* \return          1 if the context can do operations on the given type.
	* \return          0 if the context cannot do the operations on the given
	*                  type. This is always the case for a context that has
	*                  been initialized but not set up, or that has been
	*                  cleared with pk_free().
	*/
	int pk_can_do(const pk_context *ctx, pk_type_t type);

	/**
	* \brief           Verify signature (including padding if relevant).
	*
	* \param ctx       The PK context to use. It must have been set up.
	* \param md_alg    Hash algorithm used (see notes)
	* \param hash      Hash of the message to sign
	* \param hash_len  Hash length or 0 (see notes)
	* \param sig       Signature to verify
	* \param sig_len   Signature length
	*
	* \return          0 on success (signature is valid),
	*                  #ERR_PK_SIG_LEN_MISMATCH if there is a valid
	*                  signature in sig but its length is less than \p siglen,
	*                  or a specific error code.
	*
	* \note            For RSA keys, the default padding type is PKCS#1 v1.5.
	*                  Use \c pk_verify_ext( PK_RSASSA_PSS, ... )
	*                  to verify RSASSA_PSS signatures.
	*
	* \note            If hash_len is 0, then the length associated with md_alg
	*                  is used instead, or an error returned if it is invalid.
	*
	* \note            md_alg may be MD_NONE, only if hash_len != 0
	*/
	int pk_verify(pk_context *ctx, md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		const unsigned char *sig, size_t sig_len);

	/**
	* \brief           Restartable version of \c pk_verify()
	*
	* \note            Performs the same job as \c pk_verify(), but can
	*                  return early and restart according to the limit set with
	*                  \c ecp_set_max_ops() to reduce blocking for ECC
	*                  operations. For RSA, same as \c pk_verify().
	*
	* \param ctx       The PK context to use. It must have been set up.
	* \param md_alg    Hash algorithm used (see notes)
	* \param hash      Hash of the message to sign
	* \param hash_len  Hash length or 0 (see notes)
	* \param sig       Signature to verify
	* \param sig_len   Signature length
	* \param rs_ctx    Restart context (NULL to disable restart)
	*
	* \return          See \c pk_verify(), or
	* \return          #ERR_ECP_IN_PROGRESS if maximum number of
	*                  operations was reached: see \c ecp_set_max_ops().
	*/
	int pk_verify_restartable(pk_context *ctx,
		md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		const unsigned char *sig, size_t sig_len,
		pk_restart_ctx *rs_ctx);

	/**
	* \brief           Verify signature, with options.
	*                  (Includes verification of the padding depending on type.)
	*
	* \param type      Signature type (inc. possible padding type) to verify
	* \param options   Pointer to type-specific options, or NULL
	* \param ctx       The PK context to use. It must have been set up.
	* \param md_alg    Hash algorithm used (see notes)
	* \param hash      Hash of the message to sign
	* \param hash_len  Hash length or 0 (see notes)
	* \param sig       Signature to verify
	* \param sig_len   Signature length
	*
	* \return          0 on success (signature is valid),
	*                  #ERR_PK_TYPE_MISMATCH if the PK context can't be
	*                  used for this type of signatures,
	*                  #ERR_PK_SIG_LEN_MISMATCH if there is a valid
	*                  signature in sig but its length is less than \p siglen,
	*                  or a specific error code.
	*
	* \note            If hash_len is 0, then the length associated with md_alg
	*                  is used instead, or an error returned if it is invalid.
	*
	* \note            md_alg may be MD_NONE, only if hash_len != 0
	*
	* \note            If type is PK_RSASSA_PSS, then options must point
	*                  to a pk_rsassa_pss_options structure,
	*                  otherwise it must be NULL.
	*/
	int pk_verify_ext(pk_type_t type, const void *options,
		pk_context *ctx, md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		const unsigned char *sig, size_t sig_len);

	/**
	* \brief           Make signature, including padding if relevant.
	*
	* \param ctx       The PK context to use. It must have been set up
	*                  with a private key.
	* \param md_alg    Hash algorithm used (see notes)
	* \param hash      Hash of the message to sign
	* \param hash_len  Hash length or 0 (see notes)
	* \param sig       Place to write the signature
	* \param sig_len   Number of bytes written
	* \param f_rng     RNG function
	* \param p_rng     RNG parameter
	*
	* \return          0 on success, or a specific error code.
	*
	* \note            For RSA keys, the default padding type is PKCS#1 v1.5.
	*                  There is no interface in the PK module to make RSASSA-PSS
	*                  signatures yet.
	*
	* \note            If hash_len is 0, then the length associated with md_alg
	*                  is used instead, or an error returned if it is invalid.
	*
	* \note            For RSA, md_alg may be MD_NONE if hash_len != 0.
	*                  For ECDSA, md_alg may never be MD_NONE.
	*
	* \note            In order to ensure enough space for the signature, the
	*                  \p sig buffer size must be of at least
	*                  `max(ECDSA_MAX_LEN, MPI_MAX_SIZE)` bytes.
	*/
	int pk_sign(pk_context *ctx, md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		unsigned char *sig, size_t *sig_len,
		int(*f_rng)(void *, unsigned char *, size_t), void *p_rng);

	/**
	* \brief           Restartable version of \c pk_sign()
	*
	* \note            Performs the same job as \c pk_sign(), but can
	*                  return early and restart according to the limit set with
	*                  \c ecp_set_max_ops() to reduce blocking for ECC
	*                  operations. For RSA, same as \c pk_sign().
	*
	* \note            In order to ensure enough space for the signature, the
	*                  \p sig buffer size must be of at least
	*                  `max(ECDSA_MAX_LEN, MPI_MAX_SIZE)` bytes.
	*
	* \param ctx       The PK context to use. It must have been set up
	*                  with a private key.
	* \param md_alg    Hash algorithm used (see notes)
	* \param hash      Hash of the message to sign
	* \param hash_len  Hash length or 0 (see notes)
	* \param sig       Place to write the signature
	* \param sig_len   Number of bytes written
	* \param f_rng     RNG function
	* \param p_rng     RNG parameter
	* \param rs_ctx    Restart context (NULL to disable restart)
	*
	* \return          See \c pk_sign(), or
	* \return          #ERR_ECP_IN_PROGRESS if maximum number of
	*                  operations was reached: see \c ecp_set_max_ops().
	*/
	int pk_sign_restartable(pk_context *ctx,
		md_type_t md_alg,
		const unsigned char *hash, size_t hash_len,
		unsigned char *sig, size_t *sig_len,
		int(*f_rng)(void *, unsigned char *, size_t), void *p_rng,
		pk_restart_ctx *rs_ctx);

	/**
	* \brief           Decrypt message (including padding if relevant).
	*
	* \param ctx       The PK context to use. It must have been set up
	*                  with a private key.
	* \param input     Input to decrypt
	* \param ilen      Input size
	* \param output    Decrypted output
	* \param olen      Decrypted message length
	* \param osize     Size of the output buffer
	* \param f_rng     RNG function
	* \param p_rng     RNG parameter
	*
	* \note            For RSA keys, the default padding type is PKCS#1 v1.5.
	*
	* \return          0 on success, or a specific error code.
	*/
	int pk_decrypt(pk_context *ctx,
		const unsigned char *input, size_t ilen,
		unsigned char *output, size_t *olen, size_t osize,
		int(*f_rng)(void *, unsigned char *, size_t), void *p_rng);

	/**
	* \brief           Encrypt message (including padding if relevant).
	*
	* \param ctx       The PK context to use. It must have been set up.
	* \param input     Message to encrypt
	* \param ilen      Message size
	* \param output    Encrypted output
	* \param olen      Encrypted output length
	* \param osize     Size of the output buffer
	* \param f_rng     RNG function
	* \param p_rng     RNG parameter
	*
	* \note            For RSA keys, the default padding type is PKCS#1 v1.5.
	*
	* \return          0 on success, or a specific error code.
	*/
	int pk_encrypt(pk_context *ctx,
		const unsigned char *input, size_t ilen,
		unsigned char *output, size_t *olen, size_t osize,
		int(*f_rng)(void *, unsigned char *, size_t), void *p_rng);

	/**
	* \brief           Check if a public-private pair of keys matches.
	*
	* \param pub       Context holding a public key.
	* \param prv       Context holding a private (and public) key.
	*
	* \return          0 on success or ERR_PK_BAD_INPUT_DATA
	*/
	int pk_check_pair(const pk_context *pub, const pk_context *prv);

	/**
	* \brief           Export debug information
	*
	* \param ctx       The PK context to use. It must have been initialized.
	* \param items     Place to write debug items
	*
	* \return          0 on success or ERR_PK_BAD_INPUT_DATA
	*/
	int pk_debug(const pk_context *ctx, pk_debug_item *items);

	/**
	* \brief           Access the type name
	*
	* \param ctx       The PK context to use. It must have been initialized.
	*
	* \return          Type name on success, or "invalid PK"
	*/
	const char * pk_get_name(const pk_context *ctx);

	/**
	* \brief           Get the key type
	*
	* \param ctx       The PK context to use. It must have been initialized.
	*
	* \return          Type on success.
	* \return          #PK_NONE for a context that has not been set up.
	*/
	pk_type_t pk_get_type(const pk_context *ctx);

	/** \ingroup pk_module */
	/**
	* \brief           Parse a private key in PEM or DER format
	*
	* \param ctx       The PK context to fill. It must have been initialized
	*                  but not set up.
	* \param key       Input buffer to parse.
	*                  The buffer must contain the input exactly, with no
	*                  extra trailing material. For PEM, the buffer must
	*                  contain a null-terminated string.
	* \param keylen    Size of \b key in bytes.
	*                  For PEM data, this includes the terminating null byte,
	*                  so \p keylen must be equal to `strlen(key) + 1`.
	* \param pwd       Optional password for decryption.
	*                  Pass \c NULL if expecting a non-encrypted key.
	*                  Pass a string of \p pwdlen bytes if expecting an encrypted
	*                  key; a non-encrypted key will also be accepted.
	*                  The empty password is not supported.
	* \param pwdlen    Size of the password in bytes.
	*                  Ignored if \p pwd is \c NULL.
	*
	* \note            On entry, ctx must be empty, either freshly initialised
	*                  with pk_init() or reset with pk_free(). If you need a
	*                  specific key type, check the result with pk_can_do().
	*
	* \note            The key is also checked for correctness.
	*
	* \return          0 if successful, or a specific PK or PEM error code
	*/
	int pk_parse_key(pk_context *ctx,
		const unsigned char *key, size_t keylen,
		const unsigned char *pwd, size_t pwdlen);

	/** \ingroup pk_module */
	/**
	* \brief           Parse a public key in PEM or DER format
	*
	* \param ctx       The PK context to fill. It must have been initialized
	*                  but not set up.
	* \param key       Input buffer to parse.
	*                  The buffer must contain the input exactly, with no
	*                  extra trailing material. For PEM, the buffer must
	*                  contain a null-terminated string.
	* \param keylen    Size of \b key in bytes.
	*                  For PEM data, this includes the terminating null byte,
	*                  so \p keylen must be equal to `strlen(key) + 1`.
	*
	* \note            On entry, ctx must be empty, either freshly initialised
	*                  with pk_init() or reset with pk_free(). If you need a
	*                  specific key type, check the result with pk_can_do().
	*
	* \note            The key is also checked for correctness.
	*
	* \return          0 if successful, or a specific PK or PEM error code
	*/
	int pk_parse_public_key(pk_context *ctx,
		const unsigned char *key, size_t keylen);

	/**
	* \brief           Write a private key to a PKCS#1 or SEC1 DER structure
	*                  Note: data is written at the end of the buffer! Use the
	*                        return value to determine where you should start
	*                        using the buffer
	*
	* \param ctx       PK context which must contain a valid private key.
	* \param buf       buffer to write to
	* \param size      size of the buffer
	*
	* \return          length of data written if successful, or a specific
	*                  error code
	*/
	int pk_write_key_der(pk_context *ctx, unsigned char *buf, size_t size);

	/**
	* \brief           Write a public key to a SubjectPublicKeyInfo DER structure
	*                  Note: data is written at the end of the buffer! Use the
	*                        return value to determine where you should start
	*                        using the buffer
	*
	* \param ctx       PK context which must contain a valid public or private key.
	* \param buf       buffer to write to
	* \param size      size of the buffer
	*
	* \return          length of data written if successful, or a specific
	*                  error code
	*/
	int pk_write_pubkey_der(pk_context *ctx, unsigned char *buf, size_t size);

	/**
	* \brief           Write a public key to a PEM string
	*
	* \param ctx       PK context which must contain a valid public or private key.
	* \param buf       Buffer to write to. The output includes a
	*                  terminating null byte.
	* \param size      Size of the buffer in bytes.
	*
	* \return          0 if successful, or a specific error code
	*/
	int pk_write_pubkey_pem(pk_context *ctx, unsigned char *buf, size_t size);

	/**
	* \brief           Write a private key to a PKCS#1 or SEC1 PEM string
	*
	* \param ctx       PK context which must contain a valid private key.
	* \param buf       Buffer to write to. The output includes a
	*                  terminating null byte.
	* \param size      Size of the buffer in bytes.
	*
	* \return          0 if successful, or a specific error code
	*/
	int pk_write_key_pem(pk_context *ctx, unsigned char *buf, size_t size);

	/*
	* WARNING: Low-level functions. You probably do not want to use these unless
	*          you are certain you do ;)
	*/

	/**
	* \brief           Parse a SubjectPublicKeyInfo DER structure
	*
	* \param p         the position in the ASN.1 data
	* \param end       end of the buffer
	* \param pk        The PK context to fill. It must have been initialized
	*                  but not set up.
	*
	* \return          0 if successful, or a specific PK error code
	*/
	int pk_parse_subpubkey(unsigned char **p, const unsigned char *end,
		pk_context *pk);

	/**
	* \brief           Write a subjectPublicKey to ASN.1 data
	*                  Note: function works backwards in data buffer
	*
	* \param p         reference to current position pointer
	* \param start     start of the buffer (for bounds-checking)
	* \param key       PK context which must contain a valid public or private key.
	*
	* \return          the length written or a negative error code
	*/
	int pk_write_pubkey(unsigned char **p, unsigned char *start,
		const pk_context *key);

#ifdef __cplusplus
}
#endif

#endif /* md.h */
