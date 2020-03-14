/**
* \file aes.h
*/
#ifndef OEMMUR_H
#define OEMMUR_H

#include "xdldef.h"
#include <stdint.h>

#ifdef XDL_SUPPORT_MUR

uint32_t MurmurHash3_x86_32(const void * key, int len, uint32_t seed);
void MurmurHash3_x86_128(const void * key, int len, uint32_t seed, void * out);
void MurmurHash3_x64_128(const void * key, int len, uint32_t seed, void * out);

#ifdef __cplusplus
extern "C" {
#endif

	EXP_API void murhash32(const uint8_t *in, const size_t inlen, uint8_t out[4]);

	EXP_API void murhash128(const uint8_t *in, const size_t inlen, uint8_t out[16]);

#ifdef __cplusplus
}
#endif

#endif /*XDL_SUPPORT_MUR*/

#endif /* oemmur.h */

