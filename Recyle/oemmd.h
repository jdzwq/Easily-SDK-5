 /**
 * \file md.h
 *
 * \brief This file contains the generic message-digest wrapper.
 *
 * \author Adriaan de Jong <dejong@fox-it.com>
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

#ifndef MD_H
#define MD_H

#include "xdsdef.h"

#define ERR_MD_FEATURE_UNAVAILABLE                -0x5080  /**< The selected feature is not available. */
#define ERR_MD_BAD_INPUT_DATA                     -0x5100  /**< Bad input parameters to function. */
#define ERR_MD_ALLOC_FAILED                       -0x5180  /**< Failed to allocate memory. */
#define ERR_MD_FILE_IO_ERROR                      -0x5200  /**< Opening or reading of file failed. */

/* ERR_MD_HW_ACCEL_FAILED is deprecated and should not be used. */
#define ERR_MD_HW_ACCEL_FAILED                    -0x5280  /**< MD hardware accelerator failed. */


#define MD_C
//#define MD2_C
//#define MD4_C
#define MD5_C
#define SHA1_C
#define SHA256_C
#define SHA512_C

#ifdef __cplusplus
extern "C" {
#endif


	/**
	* \brief     Supported message digests.
	*
	* \warning   MD2, MD4, MD5 and SHA-1 are considered weak message digests and
	*            their use constitutes a security risk. We recommend considering
	*            stronger message digests instead.
	*
	*/
	typedef enum{
		MD_NONE = 0,    /**< None. */
		MD_MD2,       /**< The MD2 message digest. */
		MD_MD4,       /**< The MD4 message digest. */
		MD_MD5,       /**< The MD5 message digest. */
		MD_SHA1,      /**< The SHA-1 message digest. */
		MD_SHA224,    /**< The SHA-224 message digest. */
		MD_SHA256,    /**< The SHA-256 message digest. */
		MD_SHA384,    /**< The SHA-384 message digest. */
		MD_SHA512,    /**< The SHA-512 message digest. */
		MD_RIPEMD160, /**< The RIPEMD-160 message digest. */
	}md_type_t;

	/**
	* Message digest information.
	* Allows message digest functions to be called in a generic way.
	*/
	typedef struct _md_info_t
	{
		/** Digest identifier */
		md_type_t type;

		/** Name of the message digest */
		const char * name;

		/** Output length of the digest function in bytes */
		int size;

		/** Block length of the digest function in bytes */
		int block_size;

		/** Digest initialisation function */
		int(*starts_func)(void *ctx);

		/** Digest update function */
		int(*update_func)(void *ctx, const unsigned char *input, size_t ilen);

		/** Digest finalisation function */
		int(*finish_func)(void *ctx, unsigned char *output);

		/** Generic digest function */
		int(*digest_func)(const unsigned char *input, size_t ilen,
			unsigned char *output);

		/** Allocate a new context */
		void * (*ctx_alloc_func)(void);

		/** Free the given context */
		void(*ctx_free_func)(void *ctx);

		/** Clone state from a context */
		void(*clone_func)(void *dst, const void *src);

		/** Internal use only */
		int(*process_func)(void *ctx, const unsigned char *input);
	}md_info_t;

#if defined(MD2_C)
	extern const md_info_t md2_info;
#endif
#if defined(MD4_C)
	extern const md_info_t md4_info;
#endif
#if defined(MD5_C)
	extern const md_info_t md5_info;
#endif
#if defined(RIPEMD160_C)
	extern const md_info_t ripemd160_info;
#endif
#if defined(SHA1_C)
	extern const md_info_t sha1_info;
#endif
#if defined(SHA256_C)
	extern const md_info_t sha224_info;
	extern const md_info_t sha256_info;
#endif
#if defined(SHA512_C)
	extern const md_info_t sha384_info;
	extern const md_info_t sha512_info;
#endif

#if defined(SHA512_C)
#define MD_MAX_SIZE         64  /* longest known is SHA512 */
#else
#define MD_MAX_SIZE         32  /* longest known is SHA256 or less */
#endif

	/**
	* The generic message-digest context.
	*/
	typedef struct md_context_t
	{
		/** Information about the associated message digest. */
		const md_info_t *md_info;

		/** The digest-specific context. */
		void *md_ctx;

		/** The HMAC part of the context. */
		void *hmac_ctx;
	} md_context_t;

	/**
	* \brief           This function returns the list of digests supported by the
	*                  generic digest module.
	*
	* \return          A statically allocated array of digests. Each element
	*                  in the returned list is an integer belonging to the
	*                  message-digest enumeration #md_type_t.
	*                  The last entry is 0.
	*/
	const int *md_list(void);

	/**
	* \brief           This function returns the message-digest information
	*                  associated with the given digest name.
	*
	* \param md_name   The name of the digest to search for.
	*
	* \return          The message-digest information associated with \p md_name.
	* \return          NULL if the associated message-digest information is not found.
	*/
	const md_info_t *md_info_from_string(const char *md_name);

	/**
	* \brief           This function returns the message-digest information
	*                  associated with the given digest type.
	*
	* \param md_type   The type of digest to search for.
	*
	* \return          The message-digest information associated with \p md_type.
	* \return          NULL if the associated message-digest information is not found.
	*/
	const md_info_t *md_info_from_type(md_type_t md_type);

	/**
	* \brief           This function initializes a message-digest context without
	*                  binding it to a particular message-digest algorithm.
	*
	*                  This function should always be called first. It prepares the
	*                  context for md_setup() for binding it to a
	*                  message-digest algorithm.
	*/
	void md_init(md_context_t *ctx);

	/**
	* \brief           This function clears the internal structure of \p ctx and
	*                  frees any embedded internal structure, but does not free
	*                  \p ctx itself.
	*
	*                  If you have called md_setup() on \p ctx, you must
	*                  call md_free() when you are no longer using the
	*                  context.
	*                  Calling this function if you have previously
	*                  called md_init() and nothing else is optional.
	*                  You must not call this function if you have not called
	*                  md_init().
	*/
	void md_free(md_context_t *ctx);

#if ! defined(DEPRECATED_REMOVED)
#if defined(DEPRECATED_WARNING)
#define DEPRECATED    __attribute__((deprecated))
#else
#define DEPRECATED
#endif
	/**
	* \brief           This function selects the message digest algorithm to use,
	*                  and allocates internal structures.
	*
	*                  It should be called after md_init() or md_free().
	*                  Makes it necessary to call md_free() later.
	*
	* \deprecated      Superseded by md_setup() in 2.0.0
	*
	* \param ctx       The context to set up.
	* \param md_info   The information structure of the message-digest algorithm
	*                  to use.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	* \return          #ERR_MD_ALLOC_FAILED on memory-allocation failure.
	*/
	int md_init_ctx(md_context_t *ctx, const md_info_t *md_info) DEPRECATED;
#undef DEPRECATED
#endif /* DEPRECATED_REMOVED */

	/**
	* \brief           This function selects the message digest algorithm to use,
	*                  and allocates internal structures.
	*
	*                  It should be called after md_init() or
	*                  md_free(). Makes it necessary to call
	*                  md_free() later.
	*
	* \param ctx       The context to set up.
	* \param md_info   The information structure of the message-digest algorithm
	*                  to use.
	* \param hmac      Defines if HMAC is used. 0: HMAC is not used (saves some memory),
	*                  or non-zero: HMAC is used with this context.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	* \return          #ERR_MD_ALLOC_FAILED on memory-allocation failure.
	*/
	int md_setup(md_context_t *ctx, const md_info_t *md_info, int hmac);

	/**
	* \brief           This function clones the state of an message-digest
	*                  context.
	*
	* \note            You must call md_setup() on \c dst before calling
	*                  this function.
	*
	* \note            The two contexts must have the same type,
	*                  for example, both are SHA-256.
	*
	* \warning         This function clones the message-digest state, not the
	*                  HMAC state.
	*
	* \param dst       The destination context.
	* \param src       The context to be cloned.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification failure.
	*/
	int md_clone(md_context_t *dst,
		const md_context_t *src);

	/**
	* \brief           This function extracts the message-digest size from the
	*                  message-digest information structure.
	*
	* \param md_info   The information structure of the message-digest algorithm
	*                  to use.
	*
	* \return          The size of the message-digest output in Bytes.
	*/
	unsigned char md_get_size(const md_info_t *md_info);

	/**
	* \brief           This function extracts the message-digest type from the
	*                  message-digest information structure.
	*
	* \param md_info   The information structure of the message-digest algorithm
	*                  to use.
	*
	* \return          The type of the message digest.
	*/
	md_type_t md_get_type(const md_info_t *md_info);

	/**
	* \brief           This function extracts the message-digest name from the
	*                  message-digest information structure.
	*
	* \param md_info   The information structure of the message-digest algorithm
	*                  to use.
	*
	* \return          The name of the message digest.
	*/
	const char *md_get_name(const md_info_t *md_info);

	/**
	* \brief           This function starts a message-digest computation.
	*
	*                  You must call this function after setting up the context
	*                  with md_setup(), and before passing data with
	*                  md_update().
	*
	* \param ctx       The generic message-digest context.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	*/
	int md_starts(md_context_t *ctx);

	/**
	* \brief           This function feeds an input buffer into an ongoing
	*                  message-digest computation.
	*
	*                  You must call md_starts() before calling this
	*                  function. You may call this function multiple times.
	*                  Afterwards, call md_finish().
	*
	* \param ctx       The generic message-digest context.
	* \param input     The buffer holding the input data.
	* \param ilen      The length of the input data.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	*/
	int md_update(md_context_t *ctx, const unsigned char *input, size_t ilen);

	/**
	* \brief           This function finishes the digest operation,
	*                  and writes the result to the output buffer.
	*
	*                  Call this function after a call to md_starts(),
	*                  followed by any number of calls to md_update().
	*                  Afterwards, you may either clear the context with
	*                  md_free(), or call md_starts() to reuse
	*                  the context for another digest operation with the same
	*                  algorithm.
	*
	* \param ctx       The generic message-digest context.
	* \param output    The buffer for the generic message-digest checksum result.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	*/
	int md_finish(md_context_t *ctx, unsigned char *output);

	/**
	* \brief          This function calculates the message-digest of a buffer,
	*                 with respect to a configurable message-digest algorithm
	*                 in a single call.
	*
	*                 The result is calculated as
	*                 Output = message_digest(input buffer).
	*
	* \param md_info  The information structure of the message-digest algorithm
	*                 to use.
	* \param input    The buffer holding the data.
	* \param ilen     The length of the input data.
	* \param output   The generic message-digest checksum result.
	*
	* \return         \c 0 on success.
	* \return         #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                 failure.
	*/
	int md(const md_info_t *md_info, const unsigned char *input, size_t ilen,
		unsigned char *output);

	/**
	* \brief           This function sets the HMAC key and prepares to
	*                  authenticate a new message.
	*
	*                  Call this function after md_setup(), to use
	*                  the MD context for an HMAC calculation, then call
	*                  md_hmac_update() to provide the input data, and
	*                  md_hmac_finish() to get the HMAC value.
	*
	* \param ctx       The message digest context containing an embedded HMAC
	*                  context.
	* \param key       The HMAC secret key.
	* \param keylen    The length of the HMAC key in Bytes.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	*/
	int md_hmac_starts(md_context_t *ctx, const unsigned char *key,
		size_t keylen);

	/**
	* \brief           This function feeds an input buffer into an ongoing HMAC
	*                  computation.
	*
	*                  Call md_hmac_starts() or md_hmac_reset()
	*                  before calling this function.
	*                  You may call this function multiple times to pass the
	*                  input piecewise.
	*                  Afterwards, call md_hmac_finish().
	*
	* \param ctx       The message digest context containing an embedded HMAC
	*                  context.
	* \param input     The buffer holding the input data.
	* \param ilen      The length of the input data.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	*/
	int md_hmac_update(md_context_t *ctx, const unsigned char *input,
		size_t ilen);

	/**
	* \brief           This function finishes the HMAC operation, and writes
	*                  the result to the output buffer.
	*
	*                  Call this function after md_hmac_starts() and
	*                  md_hmac_update() to get the HMAC value. Afterwards
	*                  you may either call md_free() to clear the context,
	*                  or call md_hmac_reset() to reuse the context with
	*                  the same HMAC key.
	*
	* \param ctx       The message digest context containing an embedded HMAC
	*                  context.
	* \param output    The generic HMAC checksum result.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	*/
	int md_hmac_finish(md_context_t *ctx, unsigned char *output);

	/**
	* \brief           This function prepares to authenticate a new message with
	*                  the same key as the previous HMAC operation.
	*
	*                  You may call this function after md_hmac_finish().
	*                  Afterwards call md_hmac_update() to pass the new
	*                  input.
	*
	* \param ctx       The message digest context containing an embedded HMAC
	*                  context.
	*
	* \return          \c 0 on success.
	* \return          #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                  failure.
	*/
	int md_hmac_reset(md_context_t *ctx);

	/**
	* \brief          This function calculates the full generic HMAC
	*                 on the input buffer with the provided key.
	*
	*                 The function allocates the context, performs the
	*                 calculation, and frees the context.
	*
	*                 The HMAC result is calculated as
	*                 output = generic HMAC(hmac key, input buffer).
	*
	* \param md_info  The information structure of the message-digest algorithm
	*                 to use.
	* \param key      The HMAC secret key.
	* \param keylen   The length of the HMAC secret key in Bytes.
	* \param input    The buffer holding the input data.
	* \param ilen     The length of the input data.
	* \param output   The generic HMAC result.
	*
	* \return         \c 0 on success.
	* \return         #ERR_MD_BAD_INPUT_DATA on parameter-verification
	*                 failure.
	*/
	int md_hmac(const md_info_t *md_info, const unsigned char *key, size_t keylen,
		const unsigned char *input, size_t ilen,
		unsigned char *output);

	/* Internal use */
	int md_process(md_context_t *ctx, const unsigned char *data);

#ifdef __cplusplus
}
#endif

#endif /* md.h */
