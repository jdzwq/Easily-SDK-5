/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssl document

	@module	netssl.h | interface file

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

#ifndef _NETSSL_H
#define _NETSSL_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_SOCK)

#define SSL_MAJOR_VERSION_3             3
#define SSL_MINOR_VERSION_0             0 //SSL v3.0
#define SSL_MINOR_VERSION_1             1 //TLS v1.0
#define SSL_MINOR_VERSION_2             2 //TLS v1.1
#define SSL_MINOR_VERSION_3             3 //TLS v1.2

typedef enum{
	SSLv30 = 0,
	TLSv10 = 1,
	TLSv11 = 2,
	TLSv12 = 3
}TLSVER;

#define SSL_MSG_CHANGE_CIPHER_SPEC     20
#define SSL_MSG_ALERT                  21
#define SSL_MSG_HANDSHAKE              22
#define SSL_MSG_APPLICATION_DATA       23

#define SSL_ALERT_CLOSE_NOTIFY          0
#define SSL_ALERT_WARNING               1
#define SSL_ALERT_FATAL                 2
#define SSL_ALERT_NO_CERTIFICATE       41

#define SSL_EXTENSION_SERVERNAME		0
#define SSL_EXTENSION_SIGNATUREANDHASHALGORITHM	13

#define SSL_CERTIFICATE_TYPE_RSA		1


#define SSL_HS_HELLO_REQUEST            0
#define SSL_HS_CLIENT_HELLO             1
#define SSL_HS_SERVER_HELLO             2
#define SSL_HS_CERTIFICATE             11
#define SSL_HS_SERVER_KEY_EXCHANGE     12
#define SSL_HS_CERTIFICATE_REQUEST     13
#define SSL_HS_SERVER_HELLO_DONE       14
#define SSL_HS_CERTIFICATE_VERIFY      15
#define SSL_HS_CLIENT_KEY_EXCHANGE     16
#define SSL_HS_FINISHED                20

/*
Official IANA names
*/
#define SSL_RSA_WITH_NULL_MD5                    0x01   /**< Weak! */
#define SSL_RSA_WITH_NULL_SHA                    0x02   /**< Weak! */

#define SSL_RSA_WITH_RC4_128_MD5                 0x04
#define SSL_RSA_WITH_RC4_128_SHA                 0x05
#define SSL_RSA_WITH_DES_CBC_SHA                 0x09   /**< Weak! Not in TLS 1.2 */

#define SSL_RSA_WITH_3DES_EDE_CBC_SHA            0x0A

#define SSL_DHE_RSA_WITH_DES_CBC_SHA             0x15   /**< Weak! Not in TLS 1.2 */
#define SSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA        0x16

#define SSL_PSK_WITH_NULL_SHA                    0x2C   /**< Weak! */
#define SSL_DHE_PSK_WITH_NULL_SHA                0x2D   /**< Weak! */
#define SSL_RSA_PSK_WITH_NULL_SHA                0x2E   /**< Weak! */
#define SSL_RSA_WITH_AES_128_CBC_SHA             0x2F

#define SSL_DHE_RSA_WITH_AES_128_CBC_SHA         0x33
#define SSL_RSA_WITH_AES_256_CBC_SHA             0x35
#define SSL_DHE_RSA_WITH_AES_256_CBC_SHA         0x39

#define SSL_RSA_WITH_NULL_SHA256                 0x3B   /**< Weak! */
#define SSL_RSA_WITH_AES_128_CBC_SHA256          0x3C   /**< TLS 1.2 */
#define SSL_RSA_WITH_AES_256_CBC_SHA256          0x3D   /**< TLS 1.2 */

#define SSL_RSA_WITH_CAMELLIA_128_CBC_SHA        0x41
#define SSL_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA    0x45

#define SSL_DHE_RSA_WITH_AES_128_CBC_SHA256      0x67   /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_AES_256_CBC_SHA256      0x6B   /**< TLS 1.2 */

#define SSL_RSA_WITH_CAMELLIA_256_CBC_SHA        0x84
#define SSL_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA    0x88

#define SSL_PSK_WITH_RC4_128_SHA                 0x8A
#define SSL_PSK_WITH_3DES_EDE_CBC_SHA            0x8B
#define SSL_PSK_WITH_AES_128_CBC_SHA             0x8C
#define SSL_PSK_WITH_AES_256_CBC_SHA             0x8D

#define SSL_DHE_PSK_WITH_RC4_128_SHA             0x8E
#define SSL_DHE_PSK_WITH_3DES_EDE_CBC_SHA        0x8F
#define SSL_DHE_PSK_WITH_AES_128_CBC_SHA         0x90
#define SSL_DHE_PSK_WITH_AES_256_CBC_SHA         0x91

#define SSL_RSA_PSK_WITH_RC4_128_SHA             0x92
#define SSL_RSA_PSK_WITH_3DES_EDE_CBC_SHA        0x93
#define SSL_RSA_PSK_WITH_AES_128_CBC_SHA         0x94
#define SSL_RSA_PSK_WITH_AES_256_CBC_SHA         0x95

#define SSL_RSA_WITH_AES_128_GCM_SHA256          0x9C   /**< TLS 1.2 */
#define SSL_RSA_WITH_AES_256_GCM_SHA384          0x9D   /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_AES_128_GCM_SHA256      0x9E   /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_AES_256_GCM_SHA384      0x9F   /**< TLS 1.2 */

#define SSL_PSK_WITH_AES_128_GCM_SHA256          0xA8   /**< TLS 1.2 */
#define SSL_PSK_WITH_AES_256_GCM_SHA384          0xA9   /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_AES_128_GCM_SHA256      0xAA   /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_AES_256_GCM_SHA384      0xAB   /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_AES_128_GCM_SHA256      0xAC   /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_AES_256_GCM_SHA384      0xAD   /**< TLS 1.2 */

#define SSL_PSK_WITH_AES_128_CBC_SHA256          0xAE
#define SSL_PSK_WITH_AES_256_CBC_SHA384          0xAF
#define SSL_PSK_WITH_NULL_SHA256                 0xB0   /**< Weak! */
#define SSL_PSK_WITH_NULL_SHA384                 0xB1   /**< Weak! */

#define SSL_DHE_PSK_WITH_AES_128_CBC_SHA256      0xB2
#define SSL_DHE_PSK_WITH_AES_256_CBC_SHA384      0xB3
#define SSL_DHE_PSK_WITH_NULL_SHA256             0xB4   /**< Weak! */
#define SSL_DHE_PSK_WITH_NULL_SHA384             0xB5   /**< Weak! */

#define SSL_RSA_PSK_WITH_AES_128_CBC_SHA256      0xB6
#define SSL_RSA_PSK_WITH_AES_256_CBC_SHA384      0xB7
#define SSL_RSA_PSK_WITH_NULL_SHA256             0xB8   /**< Weak! */
#define SSL_RSA_PSK_WITH_NULL_SHA384             0xB9   /**< Weak! */

#define SSL_RSA_WITH_CAMELLIA_128_CBC_SHA256     0xBA   /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA256 0xBE   /**< TLS 1.2 */

#define SSL_RSA_WITH_CAMELLIA_256_CBC_SHA256     0xC0   /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA256 0xC4   /**< TLS 1.2 */

#define SSL_ECDH_ECDSA_WITH_NULL_SHA             0xC001 /**< Weak! */
#define SSL_ECDH_ECDSA_WITH_RC4_128_SHA          0xC002 /**< Not in SSL3! */
#define SSL_ECDH_ECDSA_WITH_3DES_EDE_CBC_SHA     0xC003 /**< Not in SSL3! */
#define SSL_ECDH_ECDSA_WITH_AES_128_CBC_SHA      0xC004 /**< Not in SSL3! */
#define SSL_ECDH_ECDSA_WITH_AES_256_CBC_SHA      0xC005 /**< Not in SSL3! */

#define SSL_ECDHE_ECDSA_WITH_NULL_SHA            0xC006 /**< Weak! */
#define SSL_ECDHE_ECDSA_WITH_RC4_128_SHA         0xC007 /**< Not in SSL3! */
#define SSL_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA    0xC008 /**< Not in SSL3! */
#define SSL_ECDHE_ECDSA_WITH_AES_128_CBC_SHA     0xC009 /**< Not in SSL3! */
#define SSL_ECDHE_ECDSA_WITH_AES_256_CBC_SHA     0xC00A /**< Not in SSL3! */

#define SSL_ECDH_RSA_WITH_NULL_SHA               0xC00B /**< Weak! */
#define SSL_ECDH_RSA_WITH_RC4_128_SHA            0xC00C /**< Not in SSL3! */
#define SSL_ECDH_RSA_WITH_3DES_EDE_CBC_SHA       0xC00D /**< Not in SSL3! */
#define SSL_ECDH_RSA_WITH_AES_128_CBC_SHA        0xC00E /**< Not in SSL3! */
#define SSL_ECDH_RSA_WITH_AES_256_CBC_SHA        0xC00F /**< Not in SSL3! */

#define SSL_ECDHE_RSA_WITH_NULL_SHA              0xC010 /**< Weak! */
#define SSL_ECDHE_RSA_WITH_RC4_128_SHA           0xC011 /**< Not in SSL3! */
#define SSL_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA      0xC012 /**< Not in SSL3! */
#define SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA       0xC013 /**< Not in SSL3! */
#define SSL_ECDHE_RSA_WITH_AES_256_CBC_SHA       0xC014 /**< Not in SSL3! */

#define SSL_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256  0xC023 /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384  0xC024 /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_AES_128_CBC_SHA256   0xC025 /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_AES_256_CBC_SHA384   0xC026 /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256    0xC027 /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_AES_256_CBC_SHA384    0xC028 /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_AES_128_CBC_SHA256     0xC029 /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_AES_256_CBC_SHA384     0xC02A /**< TLS 1.2 */

#define SSL_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256  0xC02B /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384  0xC02C /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_AES_128_GCM_SHA256   0xC02D /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_AES_256_GCM_SHA384   0xC02E /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256    0xC02F /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_AES_256_GCM_SHA384    0xC030 /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_AES_128_GCM_SHA256     0xC031 /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_AES_256_GCM_SHA384     0xC032 /**< TLS 1.2 */

#define SSL_ECDHE_PSK_WITH_RC4_128_SHA           0xC033 /**< Not in SSL3! */
#define SSL_ECDHE_PSK_WITH_3DES_EDE_CBC_SHA      0xC034 /**< Not in SSL3! */
#define SSL_ECDHE_PSK_WITH_AES_128_CBC_SHA       0xC035 /**< Not in SSL3! */
#define SSL_ECDHE_PSK_WITH_AES_256_CBC_SHA       0xC036 /**< Not in SSL3! */
#define SSL_ECDHE_PSK_WITH_AES_128_CBC_SHA256    0xC037 /**< Not in SSL3! */
#define SSL_ECDHE_PSK_WITH_AES_256_CBC_SHA384    0xC038 /**< Not in SSL3! */
#define SSL_ECDHE_PSK_WITH_NULL_SHA              0xC039 /**< Weak! No SSL3! */
#define SSL_ECDHE_PSK_WITH_NULL_SHA256           0xC03A /**< Weak! No SSL3! */
#define SSL_ECDHE_PSK_WITH_NULL_SHA384           0xC03B /**< Weak! No SSL3! */

#define SSL_RSA_WITH_ARIA_128_CBC_SHA256         0xC03C /**< TLS 1.2 */
#define SSL_RSA_WITH_ARIA_256_CBC_SHA384         0xC03D /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_ARIA_128_CBC_SHA256     0xC044 /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_ARIA_256_CBC_SHA384     0xC045 /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_ARIA_128_CBC_SHA256 0xC048 /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_ARIA_256_CBC_SHA384 0xC049 /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_ARIA_128_CBC_SHA256  0xC04A /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_ARIA_256_CBC_SHA384  0xC04B /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_ARIA_128_CBC_SHA256   0xC04C /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_ARIA_256_CBC_SHA384   0xC04D /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_ARIA_128_CBC_SHA256    0xC04E /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_ARIA_256_CBC_SHA384    0xC04F /**< TLS 1.2 */
#define SSL_RSA_WITH_ARIA_128_GCM_SHA256         0xC050 /**< TLS 1.2 */
#define SSL_RSA_WITH_ARIA_256_GCM_SHA384         0xC051 /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_ARIA_128_GCM_SHA256     0xC052 /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_ARIA_256_GCM_SHA384     0xC053 /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_ARIA_128_GCM_SHA256 0xC05C /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_ARIA_256_GCM_SHA384 0xC05D /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_ARIA_128_GCM_SHA256  0xC05E /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_ARIA_256_GCM_SHA384  0xC05F /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_ARIA_128_GCM_SHA256   0xC060 /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_ARIA_256_GCM_SHA384   0xC061 /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_ARIA_128_GCM_SHA256    0xC062 /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_ARIA_256_GCM_SHA384    0xC063 /**< TLS 1.2 */
#define SSL_PSK_WITH_ARIA_128_CBC_SHA256         0xC064 /**< TLS 1.2 */
#define SSL_PSK_WITH_ARIA_256_CBC_SHA384         0xC065 /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_ARIA_128_CBC_SHA256     0xC066 /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_ARIA_256_CBC_SHA384     0xC067 /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_ARIA_128_CBC_SHA256     0xC068 /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_ARIA_256_CBC_SHA384     0xC069 /**< TLS 1.2 */
#define SSL_PSK_WITH_ARIA_128_GCM_SHA256         0xC06A /**< TLS 1.2 */
#define SSL_PSK_WITH_ARIA_256_GCM_SHA384         0xC06B /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_ARIA_128_GCM_SHA256     0xC06C /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_ARIA_256_GCM_SHA384     0xC06D /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_ARIA_128_GCM_SHA256     0xC06E /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_ARIA_256_GCM_SHA384     0xC06F /**< TLS 1.2 */
#define SSL_ECDHE_PSK_WITH_ARIA_128_CBC_SHA256   0xC070 /**< TLS 1.2 */
#define SSL_ECDHE_PSK_WITH_ARIA_256_CBC_SHA384   0xC071 /**< TLS 1.2 */

#define SSL_ECDHE_ECDSA_WITH_CAMELLIA_128_CBC_SHA256 0xC072 /**< Not in SSL3! */
#define SSL_ECDHE_ECDSA_WITH_CAMELLIA_256_CBC_SHA384 0xC073 /**< Not in SSL3! */
#define SSL_ECDH_ECDSA_WITH_CAMELLIA_128_CBC_SHA256  0xC074 /**< Not in SSL3! */
#define SSL_ECDH_ECDSA_WITH_CAMELLIA_256_CBC_SHA384  0xC075 /**< Not in SSL3! */
#define SSL_ECDHE_RSA_WITH_CAMELLIA_128_CBC_SHA256   0xC076 /**< Not in SSL3! */
#define SSL_ECDHE_RSA_WITH_CAMELLIA_256_CBC_SHA384   0xC077 /**< Not in SSL3! */
#define SSL_ECDH_RSA_WITH_CAMELLIA_128_CBC_SHA256    0xC078 /**< Not in SSL3! */
#define SSL_ECDH_RSA_WITH_CAMELLIA_256_CBC_SHA384    0xC079 /**< Not in SSL3! */

#define SSL_RSA_WITH_CAMELLIA_128_GCM_SHA256         0xC07A /**< TLS 1.2 */
#define SSL_RSA_WITH_CAMELLIA_256_GCM_SHA384         0xC07B /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_CAMELLIA_128_GCM_SHA256     0xC07C /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_CAMELLIA_256_GCM_SHA384     0xC07D /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_CAMELLIA_128_GCM_SHA256 0xC086 /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_CAMELLIA_256_GCM_SHA384 0xC087 /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_CAMELLIA_128_GCM_SHA256  0xC088 /**< TLS 1.2 */
#define SSL_ECDH_ECDSA_WITH_CAMELLIA_256_GCM_SHA384  0xC089 /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_CAMELLIA_128_GCM_SHA256   0xC08A /**< TLS 1.2 */
#define SSL_ECDHE_RSA_WITH_CAMELLIA_256_GCM_SHA384   0xC08B /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_CAMELLIA_128_GCM_SHA256    0xC08C /**< TLS 1.2 */
#define SSL_ECDH_RSA_WITH_CAMELLIA_256_GCM_SHA384    0xC08D /**< TLS 1.2 */

#define SSL_PSK_WITH_CAMELLIA_128_GCM_SHA256       0xC08E /**< TLS 1.2 */
#define SSL_PSK_WITH_CAMELLIA_256_GCM_SHA384       0xC08F /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_CAMELLIA_128_GCM_SHA256   0xC090 /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_CAMELLIA_256_GCM_SHA384   0xC091 /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_CAMELLIA_128_GCM_SHA256   0xC092 /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_CAMELLIA_256_GCM_SHA384   0xC093 /**< TLS 1.2 */

#define SSL_PSK_WITH_CAMELLIA_128_CBC_SHA256       0xC094
#define SSL_PSK_WITH_CAMELLIA_256_CBC_SHA384       0xC095
#define SSL_DHE_PSK_WITH_CAMELLIA_128_CBC_SHA256   0xC096
#define SSL_DHE_PSK_WITH_CAMELLIA_256_CBC_SHA384   0xC097
#define SSL_RSA_PSK_WITH_CAMELLIA_128_CBC_SHA256   0xC098
#define SSL_RSA_PSK_WITH_CAMELLIA_256_CBC_SHA384   0xC099
#define SSL_ECDHE_PSK_WITH_CAMELLIA_128_CBC_SHA256 0xC09A /**< Not in SSL3! */
#define SSL_ECDHE_PSK_WITH_CAMELLIA_256_CBC_SHA384 0xC09B /**< Not in SSL3! */

#define SSL_RSA_WITH_AES_128_CCM                0xC09C  /**< TLS 1.2 */
#define SSL_RSA_WITH_AES_256_CCM                0xC09D  /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_AES_128_CCM            0xC09E  /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_AES_256_CCM            0xC09F  /**< TLS 1.2 */
#define SSL_RSA_WITH_AES_128_CCM_8              0xC0A0  /**< TLS 1.2 */
#define SSL_RSA_WITH_AES_256_CCM_8              0xC0A1  /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_AES_128_CCM_8          0xC0A2  /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_AES_256_CCM_8          0xC0A3  /**< TLS 1.2 */
#define SSL_PSK_WITH_AES_128_CCM                0xC0A4  /**< TLS 1.2 */
#define SSL_PSK_WITH_AES_256_CCM                0xC0A5  /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_AES_128_CCM            0xC0A6  /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_AES_256_CCM            0xC0A7  /**< TLS 1.2 */
#define SSL_PSK_WITH_AES_128_CCM_8              0xC0A8  /**< TLS 1.2 */
#define SSL_PSK_WITH_AES_256_CCM_8              0xC0A9  /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_AES_128_CCM_8          0xC0AA  /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_AES_256_CCM_8          0xC0AB  /**< TLS 1.2 */
/* The last two are named with PSK_DHE in the RFC, which looks like a typo */

#define SSL_ECDHE_ECDSA_WITH_AES_128_CCM        0xC0AC  /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_AES_256_CCM        0xC0AD  /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_AES_128_CCM_8      0xC0AE  /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_AES_256_CCM_8      0xC0AF  /**< TLS 1.2 */

#define SSL_ECJPAKE_WITH_AES_128_CCM_8          0xC0FF  /**< experimental */

/* RFC 7905 */
#define SSL_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256   0xCCA8 /**< TLS 1.2 */
#define SSL_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 0xCCA9 /**< TLS 1.2 */
#define SSL_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256     0xCCAA /**< TLS 1.2 */
#define SSL_PSK_WITH_CHACHA20_POLY1305_SHA256         0xCCAB /**< TLS 1.2 */
#define SSL_ECDHE_PSK_WITH_CHACHA20_POLY1305_SHA256   0xCCAC /**< TLS 1.2 */
#define SSL_DHE_PSK_WITH_CHACHA20_POLY1305_SHA256     0xCCAD /**< TLS 1.2 */
#define SSL_RSA_PSK_WITH_CHACHA20_POLY1305_SHA256     0xCCAE /**< TLS 1.2 */

typedef enum {
	ALG_HASH_NONE = 0,
	ALG_HASH_MD5 = 1,
	ALG_HASH_SHA1 = 2,
	ALG_HASH_SHA224 = 3,
	ALG_HASH_SHA256 = 4,
	ALG_HASH_SHA384 = 5,
	ALG_HASH_SHA512 =6
}HashAlgorithm;

typedef enum {
	ALG_SIGN_NONE = 0,
	ALG_SIGN_RSA = 1,
	ALG_SIGN_DSA = 2,
	ALG_SIG_ECDSA = 3
}SignatureAlgorithm;

#define SSL_VERIFY_NONE             0
#define SSL_VERIFY_OPTIONAL         1
#define SSL_VERIFY_REQUIRED         2

#define SSL_RSA_PUBLIC				0
#define SSL_RSA_PRIVATE				1

typedef enum
{
	SSL_HANDSHAKE_ERROR = -1,
	SSL_HELLO_REQUEST = 0,
	SSL_CLIENT_HELLO = 1,
	SSL_SERVER_HELLO = 2,
	SSL_SERVER_CERTIFICATE = 3,
	SSL_SERVER_KEY_EXCHANGE = 4,
	SSL_CERTIFICATE_REQUEST = 5,
	SSL_SERVER_HELLO_DONE = 6,
	SSL_CLIENT_CERTIFICATE = 7,
	SSL_CLIENT_KEY_EXCHANGE = 8,
	SSL_CERTIFICATE_VERIFY = 9,
	SSL_CLIENT_CHANGE_CIPHER_SPEC = 10,
	SSL_CLIENT_FINISHED = 11,
	SSL_SERVER_CHANGE_CIPHER_SPEC = 12,
	SSL_SERVER_FINISHED = 13,
	SSL_HANDSHAKE_OVER = 14
}handshake_states;

#define SSL_TYPE_CLIENT		0
#define SSL_TYPE_SERVER		1
#define SSL_TYPE_LISTEN		2

#define SSL_CTR_SIZE		8	//control byte size
#define SSL_HDR_SIZE		5	//message header size
#define SSL_HSH_SIZE		4	//handshake header size
#define SSL_RND_SIZE		32	//radom byte size
#define SSL_MST_SIZE		48	//master scret byte size
#define SSL_SES_SIZE		32	//session id size
#define SSL_CTX_SIZE		128	//encrypt/decrypt context size
#define SSL_BLK_SIZE		256	//key block size
#define SSL_FIN_SIZE		12 //finished hash size
#define SSL_MAX_MAC			64	//mac byte size(16, 20 , 32, 64)
#define SSL_MAX_IVC			16	//iv code size(8, 16)

#define SSL_PKG_SIZE		16384
#define SSL_MAX_SIZE		(SSL_PKG_SIZE + 512)

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xssl_cli: create a SSL client.
@INPUT unsigned short port: the network port to connect.
@INPUT const tchar_t* addr: the network address to connect.
@RETURN xhand_t: if succeeds return SSL client handle, fails return NULL.
*/
EXP_API xhand_t xssl_cli(unsigned short port, const tchar_t* addr);

/*
@FUNCTION xssl_srv: create a SSL server.
@INPUT res_file_t so: the network io resource handle, it must be a socket resource handle.
@RETURN xhand_t: if succeeds return SSL server handle, fails return NULL.
*/
EXP_API xhand_t xssl_srv(res_file_t so);

/*
@FUNCTION xssl_socket: get socket resource handle.
@INPUT xhand_t ssl: the SSL handle.
@RETURN res_file_t: return the socket resource handle.
*/
EXP_API res_file_t xssl_socket(xhand_t ssl);

/*
@FUNCTION xssl_type: get socket type, it can be _XSSL_TYPE_CLI, _XSSL_TYPE_SRV.
@INPUT xhand_t ssl: the SSL handle.
@RETURN int: return the socket type.
*/
EXP_API int  xssl_type(xhand_t ssl);

/*
@FUNCTION xssl_close: close SSL handle.
@INPUT xhand_t ssl: the SSL handle.
@RETURN void: none.
*/
EXP_API void  xssl_close(xhand_t ssl);

/*
@FUNCTION xssl_write: write SSL data.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* data: the data buffer.
@INOUTPUT dword_t* pb: indicate the bytes to write and return the bytes writed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_write(xhand_t ssl, const byte_t* data, dword_t* pb);

/*
@FUNCTION xssl_flush: ensure write SSL data compeleted.
@INPUT xhand_t ssl: the SSL handle.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_flush(xhand_t ssl);

/*
@FUNCTION xssl_read: read SSL data.
@INPUT xhand_t ssl: the SSL handle.
@OUTPUT byte_t* data: the data buffer.
@INOUTPUT dword_t* pb: indicate the bytes to read and return the bytes readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_read(xhand_t ssl, byte_t* data, dword_t* pb);

/*
@FUNCTION xssl_setopt: set the socket options.
@INPUT xhand_t ssl: the ssl handle.
@INPUT int oid: the option id, eg: SOCK_OPTION_SNDBUF, SOCK_OPTION_RCVBUF, SOCK_OPTION_NONBLK.
@INPUT void* opt: the option value pointer
@INPUT int len: the value length in bytes, string value must be a zero terminated token and set len to zero.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_setopt(xhand_t ssl, int oid, void* opt, int len);

/*
@FUNCTION xssl_addr_port: get SSL local address and port.
@INPUT xhand_t ssl: the SSL handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the local port.
*/
EXP_API unsigned short xssl_addr_port(xhand_t ssl, tchar_t* addr);

/*
@FUNCTION xssl_peer_port: get SSL remote address and port.
@INPUT xhand_t ssl: the SSL handle.
@OUTPUT tchar_t* addr: the string buffer.
@RETURN unsigned short: return the remote port.
*/
EXP_API unsigned short xssl_peer_port(xhand_t ssl, tchar_t* addr);

/*
@FUNCTION xssl_set_host: set SSL host name.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const tchar_t* host_cn: the host name.
@RETURN void: none.
*/
EXP_API void xssl_set_host(xhand_t ssl, const tchar_t* host_cn);

/*
@FUNCTION xssl_set_peer: set SSL peer name.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const tchar_t* host_cn: the peer name.
@RETURN void: none.
*/
EXP_API void xssl_set_peer(xhand_t ssl, const tchar_t* peer_cn);

/*
@FUNCTION xssl_set_ca: set SSL root certificate.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* sz_cert: the cerificate bytes buffer.
@INPUT dword_t clen: the cert data size in bytes.
@INPUT const byte_t* sz_rsa: the rsa key bytes buffer.
@INPUT dword_t rlen: the rsa data size in bytes.
@INPUT const byte_t* sz_pwd: the password key bytes buffer.
@INPUT dword_t plen: the password data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_set_ca(xhand_t ssl, const byte_t* sz_cert, dword_t clen, const byte_t* sz_rsa, dword_t rlen, const tchar_t* sz_pwd, int plen);

/*
@FUNCTION xssl_set_cert: set SSL owner certificate.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* sz_cert: the cerificate bytes buffer.
@INPUT dword_t clen: the cert data size in bytes.
@INPUT const byte_t* sz_rsa: the rsa key bytes buffer.
@INPUT dword_t rlen: the rsa data size in bytes.
@INPUT const byte_t* sz_pwd: the password key bytes buffer.
@INPUT dword_t plen: the password data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_set_cert(xhand_t ssl, const byte_t* sz_cert, dword_t clen, const byte_t* sz_rsa, dword_t rlen, const tchar_t* sz_pwd, int plen);

/*
@FUNCTION xssl_set_dhm: set SSL dhm key.
@INPUT xhand_t ssl: the SSL handle.
@INPUT const byte_t* dhm_P: the dhm P value bytes buffer.
@INPUT const byte_t* dhm_G: the dhm G value bytes buffer.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xssl_set_dhm(xhand_t ssl, const byte_t *dhm_P, const byte_t *dhm_G);

/*
@FUNCTION xssl_set_auth: set SSL authorization mode.
@INPUT xhand_t ssl: the SSL handle.
@INPUT int srv_verify: the certify verify mode, it can be SSL_VERIFY_NONE, SSL_VERIFY_OPTIONAL, SSL_VERIFY_REQUIRED.
@RETURN void: none.
*/
EXP_API void xssl_set_verify(xhand_t ssl, int srv_verify);

/*
@FUNCTION xssl_set_version: set SSL/TLS maximized version
@INPUT xhand_t ssl: the SSL handle.
@INPUT int cli_ver: the client maximized version it can be SSLv30, TLSv10, TLSv11, TLSv12.
@RETURN void: none.
*/
EXP_API void xssl_set_version(xhand_t ssl, int cli_ver);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/