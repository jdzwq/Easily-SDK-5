/***********************************************************************
	Easily plf v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	platform.h | interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/


#ifndef _PLATFORM_H
#define	_PLATFORM_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32) || defined(WINCE)
#define _OS_WINDOWS
#elif defined(LINUX) || defined(__LINUX__) || defined(__linux__)
#define _OS_LINUX
#elif defined(APPLE) || defined (__APPLE__) || defined (__apple__)
#define _OS_MACOS
#else
#define _OS_UNKNOW
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__amd64__)
#define _OS_64
#else
#define _OS_32
#endif


#ifndef BYTE_ORDER

#define	LIT_ENDIAN	1234	/* least-significant byte first (vax, pc) */
#define	BIG_ENDIAN	4321	/* most-significant byte first (IBM, net) */
#define	PDP_ENDIAN	3412	/* LSB first in word, MSW first in int (pdp)*/

#if defined(_WIN32) || defined(_WIN64) || defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || \
   defined(vax) || defined(ns32000) || defined(sun386) || \
   defined(MIPSEL) || defined(_MIPSEL) || defined(BIT_ZERO_ON_RIGHT) || \
   defined(__alpha__) || defined(__alpha)
#define BYTE_ORDER    LIT_ENDIAN
#endif

#if defined(sel) || defined(pyr) || defined(mc68000) || defined(sparc) || \
    defined(is68k) || defined(tahoe) || defined(ibm032) || defined(ibm370) || \
    defined(MIPSEB) || defined(_MIPSEB) || defined(_IBMR2) || defined(DGUX) ||\
    defined(apollo) || defined(__convex__) || defined(_CRAY) || \
    defined(__hppa) || defined(__hp9000) || \
    defined(__hp9000s300) || defined(__hp9000s700) || \
    defined (BIT_ZERO_ON_LEFT) || defined(m68k) || defined(__sparc)
#define BYTE_ORDER	BIG_ENDIAN
#endif

#endif /* BYTE_ORDER */

#ifndef LIT_ENDIAN
#define LIT_ENDIAN	__LITTLE_ENDIAN
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN	__BIG_ENDIAN
#endif

#ifndef PDP_ENDIAN
#define PDP_ENDIAN	__PDP_ENDIAN
#endif


#if defined(_OS_WINDOWS)
#define STDCALL __stdcall
#else
#define STDCALL
#endif

#if defined(_UNICODE) || defined(UNICODE)
#define _T(x)      L ## x
#else
#define _T(x)      x
#endif

#if defined(_UNICODE) || defined(UNICODE)
typedef wchar_t			tchar_t;
#else
typedef char			tchar_t;
#endif

#ifndef schar_t
typedef char			schar_t;
#endif

#ifndef byte_t
typedef unsigned char	byte_t;
#endif

#ifndef bool_t
typedef unsigned int	bool_t;

#define BOOL_TRUE		((bool_t)1)
#define BOOL_FALSE		((bool_t)0)
#endif

#ifndef sword_t
typedef unsigned short	sword_t;
#endif

#ifndef dword_t
typedef unsigned int	dword_t;
#endif

#ifndef lword_t
typedef unsigned long long lword_t;
#endif

#ifdef _OS_64
typedef long long		var_long;
#else
typedef int				var_long;
#endif

#ifndef stamp_t
typedef long long		stamp_t;
#endif

#ifndef wait_t
typedef int				wait_t;

#define WAIT_ERR		((wait_t)-1)
#define WAIT_TMO		((wait_t)0)
#define WAIT_RET		((wait_t)1)
#endif


#define SWAPSWORD(n)			(((sword_t)(n) & 0x00FF) << 8) | ((sword_t)(n) & 0xFF00) >> 8))
#define SWAPDWORD(n)			(((dword_t)(n) & 0x0000FFFF) << 16) | ((dword_t)(n) & 0xFFFF0000) >> 16))

#define LIT_MAKESWORD(lc,hc)	((((sword_t)(hc) << 8) & 0xFF00) | ((sword_t)(lc) & 0x00FF))
#define LIT_GETHBYTE(sw)		(byte_t)(((sword_t)(sw) >> 8) & 0x00FF)
#define LIT_GETLBYTE(sw)		(byte_t)((sword_t)(sw) & 0x00FF)

#define BIG_MAKESWORD(lc,hc)	((((sword_t)(lc) << 8) & 0xFF00) | ((sword_t)(hc) & 0x00FF))
#define BIG_GETHBYTE(sw)		(byte_t)((sword_t)(sw) & 0x00FF)
#define BIG_GETLBYTE(sw)		(byte_t)(((sword_t)(sw) >> 8) & 0x00FF) 

#define LIT_MAKEDWORD(ls,hs)	((((dword_t)(hs) << 16) & 0xFFFF0000) | ((dword_t)(ls) & 0x0000FFFF))
#define LIT_GETHSWORD(dw)		(sword_t)(((dword_t)(dw) >> 16) & 0x0000FFFF)
#define LIT_GETLSWORD(dw)		(sword_t)((dword_t)(dw) & 0x0000FFFF)

#define BIG_MAKEDWORD(ls,hs)	((((dword_t)(ls) << 16) & 0xFFFF0000) | ((dword_t)(hs) & 0x0000FFFF))
#define BIG_GETHSWORD(dw)		(sword_t)((dword_t)(dw) & 0x0000FFFF)
#define BIG_GETLSWORD(dw)		(sword_t)(((dword_t)(dw) >> 16) & 0x0000FFFF)

#define LIT_MAKELWORD(lw,hw)	((((lword_t)(hw) << 32) & 0xFFFFFFFF00000000) | ((lword_t)(lw) & 0x00000000FFFFFFFF))
#define LIT_GETHDWORD(ll)		(dword_t)(((lword_t)(ll) >> 32) & 0x00000000FFFFFFFF)
#define LIT_GETLDWORD(ll)		(dword_t)((lword_t)(ll) & 0x00000000FFFFFFFF)

#define BIG_MAKELWORD(lw,hw)	((((lword_t)(lw) << 32) & 0xFFFFFFFF00000000) | (lword_t)(hw) & 0x00000000FFFFFFFF))
#define BIG_GETHDWORD(ll)		(dword_t)((lword_t)(ll) & 0x00000000FFFFFFFF)
#define BIG_GETLDWORD(ll)		(dword_t)(((lword_t)(ll) >> 32) & 0x00000000FFFFFFFF)

#if BYTE_ORDER == BIG_ENDIAN
#define MAKELWORD			BIG_MAKELWORD
#define GETLDWORD			BIG_GETLDWORD
#define GETHDWORD			BIG_GETHDWORD

#define MAKEDWORD			BIG_MAKEDWORD
#define GETLSWORD			BIG_GETLSWORD
#define GETHSWORD			BIG_GETHSWORD

#define MAKESWORD			BIG_MAKESWORD
#define GETLBYTE			BIG_GETLBYTE
#define GETHBYTE			BIG_GETHBYTE
#else
#define MAKELWORD			LIT_MAKELWORD
#define GETLDWORD			LIT_GETLDWORD
#define GETHDWORD			LIT_GETHDWORD

#define MAKEDWORD			LIT_MAKEDWORD
#define GETLSWORD			LIT_GETLSWORD
#define GETHSWORD			LIT_GETHSWORD

#define MAKESWORD			LIT_MAKESWORD
#define GETLBYTE			LIT_GETLBYTE
#define GETHBYTE			LIT_GETHBYTE
#endif

#define PUT_BYTE(buf,off,n)			(buf[off] = (unsigned char)((n) & 0xFF))
#define PUT_SWORD_LIT(buf,off,n)	(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF))
#define PUT_DWORD_LIT(buf,off,n)	(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+2] = (unsigned char) (((n) >> 16) & 0xFF), buf[off+3] = (unsigned char) (((n) >> 24) & 0xFF))
#define PUT_LWORD_LIT(buf,off,n)    (PUT_DWORD_LIT(buf,off,LIT_GETLDWORD(n)),PUT_DWORD_LIT(buf,(off+4),LIT_GETHDWORD(n)))
#define PUT_SWORD_BIG(buf,off,n)	(buf[off] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+1] = (unsigned char) ((n) & 0xFF))
#define PUT_DWORD_BIG(buf,off,n)	(buf[off] = (unsigned char) (((n) >> 24) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 16) & 0xFF), buf[off+2] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+3] = (unsigned char) ((n) & 0xFF))
#define PUT_LWORD_BIG(buf,off,n)    (PUT_DWORD_BIG(buf,off,BIG_GETLDWORD(n)),PUT_DWORD_BIG(buf,(off+4),BIG_GETHDWORD(n)))

#define GET_BYTE(buf,off)			((unsigned char)(buf[off] & 0xFF))
#define GET_SWORD_LIT(buf,off)		((((unsigned short)(buf[off + 1]) << 8) & 0xFF00) | ((unsigned short)(buf[off]) & 0x00FF))
#define GET_DWORD_LIT(buf,off)		((((unsigned int)(buf[off + 3]) << 24) & 0xFF000000) | (((unsigned int)(buf[off + 2]) << 16) & 0x00FF0000)  | (((unsigned int)(buf[off + 1]) << 8) & 0x0000FF00) | ((unsigned int)(buf[off]) & 0x000000FF))
#define GET_LWORD_LIT(buf,off)      LIT_MAKELWORD(GET_DWORD_LIT(buf,off),GET_DWORD_LIT(buf,(off + 4)))
#define GET_SWORD_BIG(buf,off)		((((unsigned short)(buf[off]) << 8) & 0xFF00) | ((unsigned short)(buf[off+1]) & 0x00FF))
#define GET_DWORD_BIG(buf,off)		((((unsigned int)(buf[off]) << 24) & 0xFF000000)  | (((unsigned int)(buf[off + 1]) << 16) & 0x00FF0000) | (((unsigned int)(buf[off + 2]) << 8) & 0x0000FF00) | ((unsigned int)(buf[off + 3]) & 0x000000FF))
#define GET_LWORD_BIG(buf,off)      (BIG_MAKELWORD(GET_DWORD_BIG(buf,off),GET_DWORD_BIG(buf,(off + 4)))

#define PUT_SWORD_NET		PUT_SWORD_BIG
#define GET_SWORD_NET		GET_SWORD_BIG
#define PUT_DWORD_NET		PUT_DWORD_BIG
#define GET_DWORD_NET		GET_DWORD_BIG
#define PUT_LWORD_NET		PUT_LWORD_BIG
#define GET_LWORD_NET		GET_LWORD_BIG

#if BYTE_ORDER == BIG_ENDIAN
#define PUT_SWORD_LOC		PUT_SWORD_BIG
#define GET_SWORD_LOC		GET_SWORD_BIG
#define PUT_DWORD_LOC		PUT_DWORD_BIG
#define GET_DWORD_LOC		GET_DWORD_BIG
#define PUT_LWORD_LOC		PUT_LWORD_BIG
#define GET_LWORD_LOC		GET_LWORD_BIG
#else
#define PUT_SWORD_LOC		PUT_SWORD_LIT
#define GET_SWORD_LOC		GET_SWORD_LIT
#define PUT_DWORD_LOC		PUT_DWORD_LIT
#define GET_DWORD_LOC		GET_DWORD_LIT
#define PUT_LWORD_LOC		PUT_LWORD_LIT
#define GET_LWORD_LOC		GET_LWORD_LIT
#endif

#ifdef _OS_64
#define LIT_MAKESIZE(lw,hw)		((((size_t)(hw) << 32) & 0xFFFFFFFF00000000) | ((size_t)(lw) & 0x00000000FFFFFFFF))
#define LIT_GETSIZEH(ll)		(unsigned int)(((size_t)(ll) >> 32) & 0x00000000FFFFFFFF)
#define LIT_GETSIZEL(ll)		(unsigned int)((size_t)(ll) & 0x00000000FFFFFFFF)

#define BIG_MAKESIZE(lw,hw)		((((size_t)(lw) << 32) & 0xFFFFFFFF00000000) | (size_t)(hw) & 0x00000000FFFFFFFF))
#define BIG_GETSIZEH(ll)		(unsigned int)((size_t)(ll) & 0x00000000FFFFFFFF)
#define BIG_GETSIZEL(ll)		(unsigned int)(((size_t)(ll) >> 32) & 0x00000000FFFFFFFF)

#if BYTE_ORDER == BIG_ENDIAN
#define MAKESIZE			BIG_MAKESIZE
#define GETSIZEH			BIG_GETSIZEH
#define GETSIZEL			BIG_GETSIZEL
#else
#define MAKESIZE			LIT_MAKESIZE
#define GETSIZEH			LIT_GETSIZEH
#define GETSIZEL			LIT_GETSIZEL
#endif

#else
#define MAKESIZE(l,h)		((size_t)l)
#define GETSIZEH(ll)		((unsigned int)0)
#define GETSIZEL(ll)		((unsigned int)(ll))
#endif /*_OS_64*/

#ifdef _OS_64
#define PUT_VAR_LONG_NET(buf,p)     PUT_LWORD_NET(buf,0,(var_long)p)
#define GET_VAR_LONG_NET(buf)       (var_long)GET_LWORD_NET(buf,0)
#else
#define PUT_VAR_LONG_NET(buf,p)     PUT_DWORD_NET(buf,0,(var_long)p)
#define GET_VAR_LONG_NET(buf)       (var_long)GET_DWORD_NET(buf,0)
#endif

#ifdef _OS_64
#define VOID_SIZE       8
#else
#define VOID_SIZE       4
#endif

#ifdef _OS_64
#define PAGE_INDI		8
#else
#define PAGE_INDI		4
#endif

#ifndef PAGE_SHIFT

#if PAGE_INDI == 4
#define PAGE_SHIFT    12
#elif PAGE_INDI == 8
#define PAGE_SHIFT    13
#elif PAGE_INDI == 16
#define PAGE_SHIFT    14
#elif PAGE_INDI == 32
#define PAGE_SHIFT    15
#else
#define PAGE_SHIFT    16
#endif

#define PAGE_SIZE	(1 << PAGE_SHIFT)
#define PAGE_MASK	(~((1 << PAGE_SHIFT) - 1))

#endif /*PAGE_SHIFT*/

#ifndef PAGE_GRAN
#define PAGE_GRAN       (64 * 1024)
#endif

#ifndef PAGE_SPACE
#define PAGE_SPACE       (PAGE_GRAN * 1024)
#endif

/*define max integer value*/
#define MAX_LONG        2147483647		//0x7fffffff
#define MIN_LONG		-2147483648		//0x80000000
#define MAX_SHORT       32767			//0x7fff
#define MIN_SHORT		-32768			//0x8000
#define MAX_CHAR		127				//0x7f
#define MIN_CHAR		-128			//0x80
#define ALT_CHAR		0x20

#define MAX_DWORD		4294967295		//0xffffffff
#define MAX_WORD		65535			//0xffff
#define MAX_BYTE		255				//0xff

#define MIN_YEAR		1901
#define MAX_YEAR		2038


#endif	/* _PLATFORM_H */

