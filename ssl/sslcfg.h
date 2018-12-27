/**
 * \file config.h
 *
 * This set of compile-time options may be used to enable
 * or disable features selectively, and reduce the global
 * memory footprint.
 */
#ifndef SSL_CONFIG_H
#define SSL_CONFIG_H

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif

#include "../aes/aes.h"
#include "../arc/arc4.h"
#include "../des/des.h"
#include "../rsa/rsa.h"
#include "../md5/md5.h"
#include "../sha/sha1.h"
#include "../dhm/dhm.h"
#include "../x509/x509.h"
#include "../rand/havege.h"

/*
 * Uncomment if native integers are 8-bit wide.
 *
#define XYSSL_HAVE_INT8
 */

/*
 * Uncomment if native integers are 16-bit wide.
 *
#define XYSSL_HAVE_INT16
 */

/*
 * Uncomment if the compiler supports long long.
 *
 */
#if defined(HAVE_LONGLONG)
#define XYSSL_HAVE_LONGLONG
#endif

/*
 * Uncomment to enable the use of assembly code.
 */
#if defined(HAVE_ASM)
#define XYSSL_HAVE_ASM
#endif

/*
 * Uncomment if the CPU supports SSE2 (IA-32 specific).
 *
#define XYSSL_HAVE_SSE2
 */

/*
 * Enable all SSL/TLS debugging messages.
 */
#define XYSSL_DEBUG_MSG

/*
 * Enable the checkup functions (*_self_test).
 */
//#define XYSSL_SELF_TEST

/*
 * Enable the prime-number generation code.
 */
#if defined(GENPRIME)
#define XYSSL_GENPRIME
#endif

/*
 * Uncomment this macro to store the AES tables in ROM.
 *
 */
#if defined(AES_ROM_TABLES)
#define XYSSL_AES_ROM_TABLES
#endif

/*
 * Module:  library/aes.c
 * Caller:  library/ssl_tls.c
 *
 * This module enables the following ciphersuites:
 *      SSL_RSA_AES_128_SHA
 *      SSL_RSA_AES_256_SHA
 *      SSL_EDH_RSA_AES_256_SHA
 */
#define XYSSL_AES_C

/*
 * Module:  library/arc4.c
 * Caller:  library/ssl_tls.c
 *
 * This module enables the following ciphersuites:
 *      SSL_RSA_RC4_128_MD5
 *      SSL_RSA_RC4_128_SHA
 */
#if defined(ARC4_C)
#define XYSSL_ARC4_C
#endif

/*
 * Module:  library/base64.c
 * Caller:  library/x509parse.c
 *
 * This module is required for X.509 support.
 */
#define XYSSL_BASE64_C

/*
 * Module:  library/bignum.c
 * Caller:  library/dhm.c
 *          library/rsa.c
 *          library/ssl_tls.c
 *          library/x509parse.c
 *
 * This module is required for RSA and DHM support.
 */
#if defined(BIGNUM_C)
#define XYSSL_BIGNUM_C
#endif

/*
 * Module:  library/certs.c
 * Caller:
 *
 * This module is used for testing (ssl_client/server).
 */
#define XYSSL_CERTS_C

/*
 * Module:  library/debug.c
 * Caller:  library/ssl_cli.c
 *          library/ssl_srv.c
 *          library/ssl_tls.c
 *
 * This module provides debugging functions.
 */
#define XYSSL_DEBUG_C

/*
 * Module:  library/des.c
 * Caller:  library/ssl_tls.c
 *
 * This module enables the following ciphersuites:
 *      SSL_RSA_DES_168_SHA
 *      SSL_EDH_RSA_DES_168_SHA
 */
#if defined(DES_C)
#define XYSSL_DES_C
#endif

/*
 * Module:  library/dhm.c
 * Caller:  library/ssl_cli.c
 *          library/ssl_srv.c
 *
 * This module enables the following ciphersuites:
 *      SSL_EDH_RSA_DES_168_SHA
 *      SSL_EDH_RSA_AES_256_SHA
 */
#if defined(DHM_C)
#define XYSSL_DHM_C
#endif

/*
 * Module:  library/havege.c
 * Caller:
 *
 * This module enables the HAVEGE random number generator.
 */
#if defined(HAVEGE_C)
#define XYSSL_HAVEGE_C
#endif

/*
 * Module:  library/md2.c
 * Caller:  library/x509parse.c
 *
 * Uncomment to enable support for (rare) MD2-signed X.509 certs.
 *
#define XYSSL_MD2_C
 */

/*
 * Module:  library/md4.c
 * Caller:  library/x509parse.c
 *
 * Uncomment to enable support for (rare) MD4-signed X.509 certs.
 *
#define XYSSL_MD4_C
 */

/*
 * Module:  library/md5.c
 * Caller:  library/ssl_tls.c
 *          library/x509parse.c
 *
 * This module is required for SSL/TLS and X.509.
 */
#if defined(MD5_C)
#define XYSSL_MD5_C
#endif

/*
 * Module:  library/net.c
 * Caller:
 *
 * This module provides TCP/IP networking routines.
 */
#define XYSSL_NET_C

/*
 * Module:  library/padlock.c
 * Caller:  library/aes.c
 *
 * This modules adds support for the VIA PadLock on x86.
 */
#if defined(PADLOCK_C)
#define XYSSL_PADLOCK_C
#endif

/*
 * Module:  library/rsa.c
 * Caller:  library/ssl_cli.c
 *          library/ssl_srv.c
 *          library/ssl_tls.c
 *          library/x509.c
 *
 * This module is required for SSL/TLS and MD5-signed certificates.
 */
#if defined(RSA_C)
#define XYSSL_RSA_C
#endif

/*
 * Module:  library/sha1.c
 * Caller:  library/ssl_cli.c
 *          library/ssl_srv.c
 *          library/ssl_tls.c
 *          library/x509parse.c
 *
 * This module is required for SSL/TLS and SHA1-signed certificates.
 */
#if defined(SHA1_C)
#define XYSSL_SHA1_C
#endif

/*
 * Module:  library/sha2.c
 * Caller:
 *
 * This module adds support for SHA-224 and SHA-256.
 */
#if defined(SHA2_C)
#define XYSSL_SHA2_C
#endif

/*
 * Module:  library/sha4.c
 * Caller:
 *
 * This module adds support for SHA-384 and SHA-512.
 */
#if defined(SHA4_C)
#define XYSSL_SHA4_C
#endif

/*
 * Module:  library/ssl_cli.c
 * Caller:
 *
 * This module is required for SSL/TLS client support.
 */
#define XYSSL_SSL_CLI_C

/*
 * Module:  library/ssl_srv.c
 * Caller:
 *
 * This module is required for SSL/TLS server support.
 */
#define XYSSL_SSL_SRV_C

/*
 * Module:  library/ssl_tls.c
 * Caller:  library/ssl_cli.c
 *          library/ssl_srv.c
 *
 * This module is required for SSL/TLS.
 */
#define XYSSL_SSL_TLS_C

/*
 * Module:  library/timing.c
 * Caller:  library/havege.c
 *
 * This module is used by the HAVEGE random number generator.
 */
#if defined(TIMING_C)
#define XYSSL_TIMING_C
#endif

/*
 * Module:  library/x509parse.c
 * Caller:  library/ssl_cli.c
 *          library/ssl_srv.c
 *          library/ssl_tls.c
 *
 * This module is required for X.509 certificate parsing.
 */
#if defined(X509_C)
#define XYSSL_X509_PARSE_C
#endif

/*
 * Module:  library/x509_write.c
 * Caller:
 *
 * This module is required for X.509 certificate writing.
 */
#if defined(X509_C)
#define XYSSL_X509_WRITE_C
#endif

#endif /* config.h */
