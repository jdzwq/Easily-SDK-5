/**
 * \file arc4.h
 */
#ifndef ARC4_H
#define ARC4_H

#define ARC4_C

/**
 * \brief          ARC4 context structure
 */
typedef struct
{
    int x;                      /*!< permutation index */
    int y;                      /*!< permutation index */
    unsigned char m[256];       /*!< permutation table */
}
arc4_context;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          ARC4 key schedule
 *
 * \param ctx      ARC4 context to be initialized
 * \param key      the secret key
 * \param keylen   length of the key
 */
extern void arc4_setup( arc4_context *ctx, unsigned char *key, int keylen );

/**
 * \brief          ARC4 cipher function
 *
 * \param ctx      ARC4 context
 * \param length   input, output buffer length
 * \param input    input buffer to be processed
 * \param output   output buffer
 */
extern void arc4_crypt(arc4_context *ctx, int length, unsigned char *input, unsigned char* output);

/*
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
extern int arc4_self_test( int verbose );

#ifdef __cplusplus
}
#endif

#endif /* arc4.h */
