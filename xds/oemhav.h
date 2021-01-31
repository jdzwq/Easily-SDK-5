/**
* \file havege.h
*/
#ifndef HAVEGE_H
#define HAVEGE_H

#include "xdsdef.h"

#define HAVEGE_COLLECT_SIZE 1024

/**
* \brief          HAVEGE state structure
*/
typedef struct
{
	int PT1, PT2, offset[2];
	int pool[HAVEGE_COLLECT_SIZE];
	int WALK[8192];
}
havege_state;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* \brief          HAVEGE initialization
	*
	* \param hs       HAVEGE state to be initialized
	*/
	EXP_API void havege_init(havege_state *hs);

	/**
	* \brief          HAVEGE rand function
	*
	* \param rng_st   points to an HAVEGE state
	*
	* \return         A random int
	*/
	EXP_API int havege_rand(void *p_rng);

	/**
	* \brief          HAVEGE rand function
	*
	* \param p_rng    A HAVEGE state
	* \param output   Buffer to fill
	* \param len      Length of buffer
	*
	* \return         0
	*/
	EXP_API int havege_rand_bytes(void *p_rng, unsigned char *output, size_t len);

#ifdef __cplusplus
}
#endif


#endif /* havege.h */


