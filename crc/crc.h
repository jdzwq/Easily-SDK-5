#ifndef CRC_H
#define CRC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	uint16_t crc16(const unsigned char *s, uint16_t l);
	uint32_t crc32(uint32_t crc, const unsigned char *s, uint32_t l);
	uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l);

#ifdef __cplusplus
}
#endif

#if defined(SELF_TEST)
void crc16_test()
void crc32_test()
void crc64_test()
#endif

#endif
