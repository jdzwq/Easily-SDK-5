/* hyperloglog.c - Redis HyperLogLog probabilistic cardinality approximation.
* This file implements the algorithm and the exported Redis commands.
*
* Copyright (c) 2014, Salvatore Sanfilippo <antirez at gmail dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of Redis nor the names of its contributors may be used
*     to endorse or promote products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef HLL_H
#define HLL_H

#include "xdldef.h"
#include <stdint.h>

#ifdef XDL_SUPPORT_HLL

#ifdef __cplusplus
extern "C" {
#endif

	/* ========================= HyperLogLog algorithm  ========================= */

	/* Our hash function is MurmurHash2, 64 bit version.
	* It was modified for Redis in order to provide the same result in
	* big and little endian archs (endian neutral). */
	XDL_API uint64_t MurmurHash64A(const void * key, int len, unsigned int seed);

	/* Given a string element to add to the HyperLogLog, returns the length
	* of the pattern 000..1 of the element hash. As a side effect 'regp' is
	* set to the register index this element hashes to. */
	XDL_API int hllPatLen(unsigned char *ele, size_t elesize, int *regp);

	/* ================== Dense representation implementation  ================== */

	/* "Add" the element in the dense hyperloglog data structure.
	* Actually nothing is added, but the max 0 pattern counter of the subset
	* the element beints to is incremented if needed.
	*
	* 'registers' is expected to have room for HLL_REGISTERS plus an
	* additional byte on the right. This requirement is met by sds strings
	* automatically since they are implicitly null terminated.
	*
	* The function always succeed, however if as a result of the operation
	* the approximated cardinality changed, 1 is returned. Otherwise 0
	* is returned. */
	XDL_API int hllDenseAdd(uint8_t *registers, unsigned char *ele, size_t elesize);

	/* Compute SUM(2^-reg) in the dense representation.
	* PE is an array with a pre-computer table of values 2^-reg indexed by reg.
	* As a side effect the integer pointed by 'ezp' is set to the number
	* of zero registers. */
	XDL_API double hllDenseSum(uint8_t *registers, double *PE, int *ezp);


#ifdef __cplusplus
}
#endif

#endif

#endif /*HLL_H*/