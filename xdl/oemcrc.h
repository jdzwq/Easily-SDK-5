#ifndef CRC_H
#define CRC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_CRYPT

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	XDL_API uint16_t crc16(const unsigned char *s, uint16_t l);

	XDL_API uint32_t crc32(uint32_t crc, const unsigned char *s, uint32_t l);

	XDL_API uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l);

#ifdef __cplusplus
}
#endif

#endif

#endif

