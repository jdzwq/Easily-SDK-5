#ifndef MD2_H
#define MD2_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_CRYPT

/**
* \brief          MD2 context structure
*/
typedef struct
{
	unsigned char cksum[16];    /*!< checksum of the data block */
	unsigned char state[48];    /*!< intermediate digest state  */
	unsigned char buffer[16];   /*!< data block being processed */

	unsigned char ipad[64];     /*!< HMAC: inner padding        */
	unsigned char opad[64];     /*!< HMAC: outer padding        */
	int left;                   /*!< amount of data in buffer   */
}
md2_context;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* \brief          MD2 context setup
	*
	* \param ctx      context to be initialized
	*/
	EXP_API void md2_starts(md2_context *ctx);

	/**
	* \brief          MD2 process buffer
	*
	* \param ctx      MD2 context
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	*/
	EXP_API void md2_update(md2_context *ctx, unsigned char *input, int ilen);

	/**
	* \brief          MD2 final digest
	*
	* \param ctx      MD2 context
	* \param output   MD2 checksum result
	*/
	EXP_API void md2_finish(md2_context *ctx, unsigned char output[16]);

	/**
	* \brief          Output = MD2( input buffer )
	*
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	* \param output   MD2 checksum result
	*/
	EXP_API void md2(unsigned char *input, int ilen, unsigned char output[16]);

	/**
	* \brief          Output = MD2( file contents )
	*
	* \param path     input file name
	* \param output   MD2 checksum result
	*
	* \return         0 if successful, 1 if fopen failed,
	*                 or 2 if fread failed
	*/
	//EXP_API int md2_file(char *path, unsigned char output[16]);

	/**
	* \brief          MD2 HMAC context setup
	*
	* \param ctx      HMAC context to be initialized
	* \param key      HMAC secret key
	* \param keylen   length of the HMAC key
	*/
	EXP_API void md2_hmac_starts(md2_context *ctx, unsigned char *key, int keylen);

	/**
	* \brief          MD2 HMAC process buffer
	*
	* \param ctx      HMAC context
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	*/
	EXP_API void md2_hmac_update(md2_context *ctx, unsigned char *input, int ilen);

	/**
	* \brief          MD2 HMAC final digest
	*
	* \param ctx      HMAC context
	* \param output   MD2 HMAC checksum result
	*/
	EXP_API void md2_hmac_finish(md2_context *ctx, unsigned char output[16]);

	/**
	* \brief          Output = HMAC-MD2( hmac key, input buffer )
	*
	* \param key      HMAC secret key
	* \param keylen   length of the HMAC key
	* \param input    buffer holding the  data
	* \param ilen     length of the input data
	* \param output   HMAC-MD2 result
	*/
	EXP_API void md2_hmac(unsigned char *key, int keylen,
		unsigned char *input, int ilen,
		unsigned char output[16]);


#ifdef __cplusplus
}
#endif

#endif

#endif /* md2.h */
