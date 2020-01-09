#ifndef _SHA1_H
#define _SHA1_H

#ifdef __cplusplus
extern "C" {
#endif

	extern void hmac_sha1(unsigned char *key, int key_length, unsigned char *data, int data_length, unsigned char *digest);

#ifdef __cplusplus
}
#endif

#endif