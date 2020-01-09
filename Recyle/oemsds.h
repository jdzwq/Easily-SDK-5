/* SDSLib 2.0 -- A C dynamic strings library
*
* Copyright (c) 2006-2015, Salvatore Sanfilippo <antirez at gmail dot com>
* Copyright (c) 2015, Oran Agra
* Copyright (c) 2015, Redis Labs, Inc
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

#ifndef SDS_H
#define SDS_H

#include "xdldef.h"
#include <stdint.h>

#ifdef XDL_SUPPORT_SDS

#define SDS_MAX_PREALLOC (1024*1024)

typedef char *sds;

/* Note: sdshdr5 is never used, we just access the flags byte directly.
* However is here to document the layout of type 5 SDS strings. */

#if defined(__GNUC__)
struct __attribute__((__packed__)) sdshdr5{
	unsigned char flags; /* 3 lsb of type, and 5 msb of string length */
	char buf[];
};
struct __attribute__((__packed__)) sdshdr8{
	uint8_t len; /* used */
	uint8_t alloc; /* excluding the header and null terminator */
	unsigned char flags; /* 3 lsb of type, 5 unused bits */
	char buf[];
};
struct __attribute__((__packed__)) sdshdr16{
	uint16_t len; /* used */
	uint16_t alloc; /* excluding the header and null terminator */
	unsigned char flags; /* 3 lsb of type, 5 unused bits */
	char buf[];
};
struct __attribute__((__packed__)) sdshdr32{
	uint32_t len; /* used */
	uint32_t alloc; /* excluding the header and null terminator */
	unsigned char flags; /* 3 lsb of type, 5 unused bits */
	char buf[];
};
struct __attribute__((__packed__)) sdshdr64{
	uint64_t len; /* used */
	uint64_t alloc; /* excluding the header and null terminator */
	unsigned char flags; /* 3 lsb of type, 5 unused bits */
	char buf[];
};
#else
#pragma pack (push,1) 
struct sdshdr5{
	unsigned char flags; /* 3 lsb of type, and 5 msb of string length */
	char buf[];
};
struct sdshdr8{
	uint8_t len; /* used */
	uint8_t alloc; /* excluding the header and null terminator */
	unsigned char flags; /* 3 lsb of type, 5 unused bits */
	char buf[];
};
struct sdshdr16{
	uint16_t len; /* used */
	uint16_t alloc; /* excluding the header and null terminator */
	unsigned char flags; /* 3 lsb of type, 5 unused bits */
	char buf[];
};
struct sdshdr32{
	uint32_t len; /* used */
	uint32_t alloc; /* excluding the header and null terminator */
	unsigned char flags; /* 3 lsb of type, 5 unused bits */
	char buf[];
};
struct sdshdr64{
	uint64_t len; /* used */
	uint64_t alloc; /* excluding the header and null terminator */
	unsigned char flags; /* 3 lsb of type, 5 unused bits */
	char buf[];
};
#pragma pack(pop)
#endif

#define SDS_TYPE_5  0
#define SDS_TYPE_8  1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_64 4
#define SDS_TYPE_MASK 7
#define SDS_TYPE_BITS 3
#define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
#define SDS_HDR(T,s) ((struct sdshdr##T *)((s)-(sizeof(struct sdshdr##T))))
#define SDS_TYPE_5_LEN(f) ((f)>>SDS_TYPE_BITS)

size_t sdslen(const sds s);

size_t sdsavail(const sds s);

void sdssetlen(sds s, size_t newlen);

void sdsinclen(sds s, size_t inc);

size_t sdsalloc(const sds s);

void sdssetalloc(sds s, size_t newlen);

int sdsHdrSize(char type);

char sdsReqType(size_t string_size);

#ifdef __cplusplus
extern "C" {
#endif

	extern sds sdsnewlen(const void *init, size_t initlen);

	extern sds sdsnew(const char *init);

	extern sds sdsempty(void);

	extern sds sdsdup(const sds s);

	extern void sdsfree(sds s);

	extern sds sdsgrowzero(sds s, size_t len);

	extern sds sdscatlen(sds s, const void *t, size_t len);

	extern sds sdscat(sds s, const char *t);

	extern sds sdscatsds(sds s, const sds t);

	extern sds sdscpylen(sds s, const char *t, size_t len);

	extern sds sdscpy(sds s, const char *t);

	extern sds sdscatvprintf(sds s, const char *fmt, va_list ap);

#ifdef __GNUC__
	extern sds sdscatprintf(sds s, const char *fmt, ...)
		__attribute__((format(printf, 2, 3)));
#else
	extern sds sdscatprintf(sds s, const char *fmt, ...);
#endif

	extern sds sdscatfmt(sds s, char const *fmt, ...);

	extern sds sdstrim(sds s, const char *cset);

	extern void sdsrange(sds s, int start, int end);

	extern void sdsupdatelen(sds s);

	extern void sdsclear(sds s);

	extern int sdscmp(const sds s1, const sds s2);

	extern sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);

	extern void sdsfreesplitres(sds *tokens, int count);

	extern void sdstolower(sds s);

	extern void sdstoupper(sds s);

	extern sds sdsfromlonglong(long long value);

	extern sds sdscatrepr(sds s, const char *p, size_t len);

	extern sds *sdssplitargs(const char *line, int *argc);

	extern sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);

	extern sds sdsjoin(char **argv, int argc, char *sep);

	extern sds sdsjoinsds(sds *argv, int argc, const char *sep, size_t seplen);

	/* Low level functions exposed to the user API */
	extern sds sdsMakeRoomFor(sds s, size_t addlen);

	extern void sdsIncrLen(sds s, int incr);

	extern sds sdsRemoveFreeSpace(sds s);

	extern size_t sdsAllocSize(sds s);

	extern void *sdsAllocPtr(sds s);


#ifdef __cplusplus
}
#endif

#endif

#endif /*SDS_H*/
