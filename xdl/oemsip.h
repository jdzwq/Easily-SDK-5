/**
* \file aes.h
*/
#ifndef OEMSIP_H
#define OEMSIP_H

#include "xdldef.h"
#include <stdint.h>

#ifdef XDL_SUPPORT_SIP

uint64_t siphash(const uint8_t *in, const size_t inlen, const uint8_t *k);

uint64_t siphash_nocase(const uint8_t *in, const size_t inlen, const uint8_t *k);

#ifdef __cplusplus
extern "C" {
#endif

	XDL_API void siphash64(const uint8_t *in, const size_t inlen, uint8_t out[8]);

#ifdef __cplusplus
}
#endif

#endif /*XDL_SUPPORT_SIP*/

#endif /* oemsip.h */

