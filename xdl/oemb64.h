/**
* \file base64.h
*/
#ifndef BASE64_H
#define BASE64_H


#include "xdldef.h"

#ifdef XDL_SUPPORT_CRYPT

#define ERR_BASE64_BUFFER_TOO_SMALL               -0x0010
#define ERR_BASE64_INVALID_CHARACTER              -0x0012

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* \brief          Encode a buffer into base64 format
	*
	* \param dst      destination buffer
	* \param dlen     size of the buffer
	* \param src      source buffer
	* \param slen     amount of data to be encoded
	*
	* \return         0 if successful, or ERR_BASE64_BUFFER_TOO_SMALL.
	*                 *dlen is always updated to reflect the amount
	*                 of data that has (or would have) been written.
	*
	* \note           Call this function with *dlen = 0 to obtain the
	*                 required buffer size in *dlen
	*/
	EXP_API int base64_encode(unsigned char *dst, int *dlen,
		unsigned char *src, int  slen);

	/**
	* \brief          Decode a base64-formatted buffer
	*
	* \param dst      destination buffer
	* \param dlen     size of the buffer
	* \param src      source buffer
	* \param slen     amount of data to be decoded
	*
	* \return         0 if successful, ERR_BASE64_BUFFER_TOO_SMALL, or
	*                 ERR_BASE64_INVALID_DATA if the input data is not
	*                 correct. *dlen is always updated to reflect the amount
	*                 of data that has (or would have) been written.
	*
	* \note           Call this function with *dlen = 0 to obtain the
	*                 required buffer size in *dlen
	*/
	EXP_API int base64_decode(unsigned char *dst, int *dlen,
		unsigned char *src, int  slen);


#ifdef __cplusplus
}
#endif

#endif

#endif /* base64.h */

