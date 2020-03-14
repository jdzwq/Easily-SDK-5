﻿/**
* \file md5.h
*/
#ifndef MD5_H
#define MD5_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_CRYPT

/**
* \brief          MD5 context structure
*/
typedef struct
{
	unsigned long total[2];     /*!< number of bytes processed  */
	unsigned long state[4];     /*!< intermediate digest state  */
	unsigned char buffer[64];   /*!< data block being processed */

	unsigned char ipad[64];     /*!< HMAC: inner padding        */
	unsigned char opad[64];     /*!< HMAC: outer padding        */
}
md5_context;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* \brief          MD5 context setup
	*
	* \param ctx      context to be initialized
	*/
	EXP_API void md5_starts(md5_context *ctx);

	/**
	* \brief          MD5 process buffer
	*
	* \param ctx      MD5 context
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	*/
	EXP_API void md5_update(md5_context *ctx, unsigned char *input, int ilen);

	/**
	* \brief          MD5 final digest
	*
	* \param ctx      MD5 context
	* \param output   MD5 checksum result
	*/
	EXP_API void md5_finish(md5_context *ctx, unsigned char output[16]);

	/**
	* \brief          Output = MD5( input buffer )
	*
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	* \param output   MD5 checksum result
	*/
	EXP_API void md5(unsigned char *input, int ilen, unsigned char output[16]);

	/**
	* \brief          Output = MD5( file contents )
	*
	* \param path     input file name
	* \param output   MD5 checksum result
	*
	* \return         0 if successful, 1 if fopen failed,
	*                 or 2 if fread failed
	*/
	//EXP_API int md5_file(char *path, unsigned char output[16]);

	/**
	* \brief          MD5 HMAC context setup
	*
	* \param ctx      HMAC context to be initialized
	* \param key      HMAC secret key
	* \param keylen   length of the HMAC key
	*/
	EXP_API void md5_hmac_starts(md5_context *ctx, unsigned char *key, int keylen);

	/**
	* \brief          MD5 HMAC process buffer
	*
	* \param ctx      HMAC context
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	*/
	EXP_API void md5_hmac_update(md5_context *ctx, unsigned char *input, int ilen);

	/**
	* \brief          MD5 HMAC final digest
	*
	* \param ctx      HMAC context
	* \param output   MD5 HMAC checksum result
	*/
	EXP_API void md5_hmac_finish(md5_context *ctx, unsigned char output[16]);

	/**
	* \brief          Output = HMAC-MD5( hmac key, input buffer )
	*
	* \param key      HMAC secret key
	* \param keylen   length of the HMAC key
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	* \param output   HMAC-MD5 result
	*/
	EXP_API void md5_hmac(unsigned char *key, int keylen, unsigned char *input, int ilen, unsigned char output[16]);

#ifdef __cplusplus
}
#endif


#endif /*XDL_SUPPORT_CRYPT*/

#endif	/*OEMMD5_H */

