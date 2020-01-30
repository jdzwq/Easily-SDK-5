/**
* \file md4.h
*/
#ifndef MD4_H
#define MD4_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_CRYPT

/**
* \brief          MD4 context structure
*/
typedef struct
{
	unsigned long total[2];     /*!< number of bytes processed  */
	unsigned long state[4];     /*!< intermediate digest state  */
	unsigned char buffer[64];   /*!< data block being processed */

	unsigned char ipad[64];     /*!< HMAC: inner padding        */
	unsigned char opad[64];     /*!< HMAC: outer padding        */
}
md4_context;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* \brief          MD4 context setup
	*
	* \param ctx      context to be initialized
	*/
	XDL_API void md4_starts(md4_context *ctx);

	/**
	* \brief          MD4 process buffer
	*
	* \param ctx      MD4 context
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	*/
	XDL_API void md4_update(md4_context *ctx, unsigned char *input, int ilen);

	/**
	* \brief          MD4 final digest
	*
	* \param ctx      MD4 context
	* \param output   MD4 checksum result
	*/
	XDL_API void md4_finish(md4_context *ctx, unsigned char output[16]);

	/**
	* \brief          Output = MD4( input buffer )
	*
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	* \param output   MD4 checksum result
	*/
	XDL_API void md4(unsigned char *input, int ilen, unsigned char output[16]);

	/**
	* \brief          Output = MD4( file contents )
	*
	* \param path     input file name
	* \param output   MD4 checksum result
	*
	* \return         0 if successful, 1 if fopen failed,
	*                 or 2 if fread failed
	*/
	//XDL_API int md4_file(char *path, unsigned char output[16]);

	/**
	* \brief          MD4 HMAC context setup
	*
	* \param ctx      HMAC context to be initialized
	* \param key      HMAC secret key
	* \param keylen   length of the HMAC key
	*/
	XDL_API void md4_hmac_starts(md4_context *ctx, unsigned char *key, int keylen);

	/**
	* \brief          MD4 HMAC process buffer
	*
	* \param ctx      HMAC context
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	*/
	XDL_API void md4_hmac_update(md4_context *ctx, unsigned char *input, int ilen);

	/**
	* \brief          MD4 HMAC final digest
	*
	* \param ctx      HMAC context
	* \param output   MD4 HMAC checksum result
	*/
	XDL_API void md4_hmac_finish(md4_context *ctx, unsigned char output[16]);

	/**
	* \brief          Output = HMAC-MD4( hmac key, input buffer )
	*
	* \param key      HMAC secret key
	* \param keylen   length of the HMAC key
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	* \param output   HMAC-MD4 result
	*/
	XDL_API void md4_hmac(unsigned char *key, int keylen,
		unsigned char *input, int ilen,
		unsigned char output[16]);


#ifdef __cplusplus
}
#endif

#endif

#endif /* md4.h */

