/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	xdsdef.h | interface file

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


#ifndef _XDSDEF_H
#define	_XDSDEF_H


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

#if defined(_OS_WINDOWS)
#if defined(_USRDLL)
#define EXP_API __declspec(dllexport)
#define LOC_API 
#else
#define EXP_API extern
#define LOC_API 
#endif
#else
#define EXP_API __attribute__((visibility("default"))) extern
#define LOC_API __attribute__((visibility("hidden")))
#endif


#if defined(_OS_WINDOWS)
#define STDCALL __stdcall
#else
#define STDCALL
#endif

#if defined(_OS_WINDOWS)
#include <tchar.h>
#else
#include <wchar.h>
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#ifndef _OS_WINDOWS
#if defined(_UNICODE) || defined(UNICODE)
#define _T(x)      L ## x
#else
#define _T(x)      x
#endif
#endif

#ifndef CHAR_BIT
#define CHAR_BIT  __CHAR_BIT__
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

/*
#if defined(_UNICODE) || defined(UNICODE)
#define _T(x)      L ## x
#else
#define _T(x)      x
#endif
*/

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

#define GET_THREEBYTE_BIG(buf,off)		(((unsigned int)(buf[off]) << 16) | ((unsigned int)(buf[off+1]) << 8) | (unsigned int)(buf[off+2]))
#define PUT_THREEBYTE_BIG(buf,off,n)	{buf[off] = (unsigned char)((n) >> 16);buf[off+1] = (unsigned char)((n) >> 8);buf[off+2] = (unsigned char)((n));}

#define GET_THREEBYTE_LIT(buf,off)		(((unsigned int)(buf[off+2]) << 16) | ((unsigned int)(buf[off+1]) << 8) | (unsigned int)(buf[off]))
#define PUT_THREEBYTE_LIT(buf,off,n)	{buf[off] = (unsigned char)((n));buf[off+1] = (unsigned char)((n) >> 8);buf[off+2] = (unsigned char)((n)>>16);}

#define PUT_BYTE(buf,off,n)			(buf[off] = (unsigned char)((n) & 0xFF))
#define PUT_SWORD_LIT(buf,off,n)	(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF))
#define PUT_DWORD_LIT(buf,off,n)	(buf[off] = (unsigned char) ((n) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+2] = (unsigned char) (((n) >> 16) & 0xFF), buf[off+3] = (unsigned char) (((n) >> 24) & 0xFF))
#define PUT_LWORD_LIT(buf,off,n)    (PUT_DWORD_LIT(buf,off,LIT_GETLDWORD(n)),PUT_DWORD_LIT(buf,(off+4),LIT_GETHDWORD(n)))
#define PUT_SWORD_BIG(buf,off,n)	(buf[off] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+1] = (unsigned char) ((n) & 0xFF))
#define PUT_DWORD_BIG(buf,off,n)	(buf[off] = (unsigned char) (((n) >> 24) & 0xFF), buf[off+1] = (unsigned char) (((n) >> 16) & 0xFF), buf[off+2] = (unsigned char) (((n) >> 8) & 0xFF), buf[off+3] = (unsigned char) ((n) & 0xFF))
#define PUT_LWORD_BIG(buf,off,n)    (PUT_DWORD_BIG(buf,off,BIG_GETLDWORD(n)),PUT_DWORD_BIG(buf,(off+4),BIG_GETHDWORD(n)))

#define GET_BYTE(buf,off)			((unsigned char)((buf[off]) & 0xFF))
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
#define PUT_THREEBYTE_NET	PUT_THREEBYTE_BIG
#define GET_THREEBYTE_NET	GET_THREEBYTE_BIG

#if BYTE_ORDER == BIG_ENDIAN
#define PUT_SWORD_LOC		PUT_SWORD_BIG
#define GET_SWORD_LOC		GET_SWORD_BIG
#define PUT_DWORD_LOC		PUT_DWORD_BIG
#define GET_DWORD_LOC		GET_DWORD_BIG
#define PUT_LWORD_LOC		PUT_LWORD_BIG
#define GET_LWORD_LOC		GET_LWORD_BIG
#define PUT_THREEBYTE_LOC	PUT_THREEBYTE_BIG
#define GET_THREEBYTE_LOC	GET_THREEBYTE_BIG
#else
#define PUT_SWORD_LOC		PUT_SWORD_LIT
#define GET_SWORD_LOC		GET_SWORD_LIT
#define PUT_DWORD_LOC		PUT_DWORD_LIT
#define GET_DWORD_LOC		GET_DWORD_LIT
#define PUT_LWORD_LOC		PUT_LWORD_LIT
#define GET_LWORD_LOC		GET_LWORD_LIT
#define PUT_THREEBYTE_LOC	PUT_THREEBYTE_LIT
#define GET_THREEBYTE_LOC	GET_THREEBYTE_LIT
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

/*define unicode prefix*/
#ifndef GBKBOM
#define GBKBOM		0xFFFF
#endif

#if BYTE_ORDER == BIG_ENDIAN
#ifndef BIGBOM
#define BIGBOM		0xFEFF
#endif
#ifndef LITBOM
#define LITBOM		0xFFFE
#endif
#ifndef UTFBOM
#define UTFBOM		0xEFBBBF
#endif
#ifndef DEFBOM
#define DEFBOM		BIGBOM
#endif
#else
#ifndef BIGBOM
#define BIGBOM		0xFFFE
#endif
#ifndef LITBOM
#define LITBOM		0xFEFF
#endif
#ifndef UTFBOM
#define UTFBOM		0xBFBBEF
#endif
#ifndef DEFBOM
#define DEFBOM		LITBOM
#endif
#endif

#define PUT_ENCODE(buf, off, enc)	(buf[off] = (unsigned char)(enc >> 16), buf[off + 1] = (unsigned char)(enc >> 8), buf[off + 2] = (unsigned char)(enc))
#define GET_ENCODE(buf, off)		(int)((unsigned int)buf[off] << 16 | (unsigned int)buf[off + 1] << 8 | (unsigned int)buf[off + 2])

#define CHARSET_GB2312		_T("gb2312")
#define CHARSET_UTF8		_T("utf-8")
#define CHARSET_UTF16		_T("utf-16")

#define _UNKNOWN        0x0000
#define _GB2312         GBKBOM
#define _UTF8           UTFBOM
#define _UTF16_LIT      LITBOM
#define _UTF16_BIG      BIGBOM
#if BYTE_ORDER == BIG_ENDIAN
#define _UCS2           _UTF16_BIG
#else
#define _UCS2           _UTF16_LIT
#endif


#ifdef _OS_WINDOWS
#define DEF_MBS			_GB2312
#else
#define DEF_MBS			_UTF8
#endif

#if BYTE_ORDER == BIG_ENDIAN
#define DEF_UCS			_UTF16_BIG
#else
#define DEF_UCS			_UTF16_LIT
#endif

/*define max encode bytes*/
#ifdef _UNICODE
#define CHS_LEN		1
#else
#define CHS_LEN		3
#endif

#define UTF_LEN		3

#define IS_CONTROL_CHAR(ch)		(((int)ch >= 0 && (int)ch <= 31) || (int)ch == 127)

#define IS_NUM_FEED(ch)	 ((ch == _T(' ') || ch == _T('\t'))? 1 : 0)

/*define options string token item feed and line feed*/
#define OPT_ITEMFEED		_T('~')

#define OPT_LINEFEED		_T(';')

#define CSS_ITEMFEED		_T(':')

#define CSS_LINEFEED		_T(';')

#define TXT_ITEMFEED		_T('\t')

#define TXT_LINEFEED		_T('\r')

#define CSV_ITEMFEED		_T(',')

#define CSV_LINEFEED		_T('\n')

#define URL_ITEMFEED		_T('=')

#define URL_LINEFEED		_T('&')


#define MAXDBL      1.7976931348623158e+308
#define MINDBL      2.2250738585072014e-308
#define MAXFLT      3.402823466e+38F
#define MINFLT      1.175494351e-38F

#define IS_ZERO_FLOAT(f)	((-MINFLT <= f && f <= MINFLT)? 1 : 0)
#define IS_ZERO_DOUBLE(d)	((-MINDBL <= d && d <= MINDBL)? 1 : 0)
#define IS_VALID_FLOAT(f)	((-MAXFLT < f && f < MAXFLT)? 1 : 0)
#define IS_VALID_DOUBLE(d)	((-MAXDBL < d && d < MAXDBL)? 1 : 0)

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

/*define max numeric precision*/
#define MAX_DOUBLE_PREC	18
#define MAX_DOUBLE_DIGI	6
#define DEF_DOUBLE_PREC 8
#define MAX_FLOAT_PREC	12
#define MAX_FLOAT_DIGI	4
#define DEF_FLOAT_PREC	6


/*define some string token length*/
#define INT_LEN			16
#define NUM_LEN			48 
#define DATE_LEN		48
#define UTC_LEN			24
#define CLR_LEN			24
#define RES_LEN			64
#define KEY_LEN			128
#define BLK_LEN			32
#define META_LEN		256
#define UUID_LEN		36
#define PATH_LEN		1024
#define STYLE_LEN		2048
#define CSS_LEN			4096
#define MD5_LEN			32
#define HMAC_LEN		160
#define ETAG_LEN		64
#define ADDR_LEN		18
#define MAC_LEN			17
#define ERR_LEN			512

#define XPI			3.1415926535
//#define INCHPERTM	0.003937
#define INCHPERMM	0.03937f
//#define TMPERINCH	254
#define MMPERINCH	25.4f
//#define PTPERINCH	1440
//#define PTPERMM	56.693f
#define MMPERLOGPT	0.1f
#define MMPERPD		0.3527f
#define PDPERMM		2.835f
#define PDPERINCH	72
#define PTPERMM		3.779f
#define MMPERPT		0.2646f

//color darkeness
#define DEF_SOFT_DARKEN		-3
#define DEF_MIDD_DARKEN		-6
#define DEF_HARD_DARKEN		-10
#define DEF_SOFT_LIGHTEN	3
#define DEF_MIDD_LIGHTEN	6
#define DEF_HARD_LIGHTEN	10

#define DEF_MIN_SHADOW		5
#define DEF_MAX_SHADOW		10

typedef struct _handle_head{
	byte_t tag; //handle object type
	byte_t lru[3]; //handle object counter
}handle_head;

/*visual type*/
#define _VISUAL_UNKNOWN		0x00
#define _VISUAL_DISPLAY		0x01
#define _VISUAL_PRINTER		0x02
#define _VISUAL_SCRIPT		0x03

typedef struct _handle_head	 *visual_t;
typedef struct _handle_head	 *bitmap_t;

typedef enum{ _RGB_COLOR, HSL_COLOR, _HEX_COLOR }CLRFMT;

#define RGB_GRAY(r,g,b) (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b)

typedef struct _yuv_color_t{
	unsigned char y, u, v;
}yuv_color_t;

typedef struct _xcolor_t{
	unsigned char r, g, b;
}xcolor_t;

typedef struct _clr_mod_t{
	xcolor_t clr_bkg;
	xcolor_t clr_frg;
	xcolor_t clr_txt;
	xcolor_t clr_msk;
	xcolor_t clr_ico;
}clr_mod_t;


typedef struct _xrect_t{
	union{
		int x;
		float fx;
	};
	union{
		int y;
		float fy;
	};
	union{
		int w;
		float fw;
	};
	union{
		int h;
		float fh;
	};
}xrect_t;

typedef struct _xpoint_t{
	union{
		int x;
		float fx;
	};
	union{
		int y;
		float fy;
	};
}xpoint_t;

typedef struct _xsize_t{
	union{
		int w;
		float fw;
	};
	union{
		int h;
		float fh;
	};
}xsize_t;

typedef struct _xspan_t{
	union{
		int r;
		float fr;
	};
}xspan_t;

#define RECTPOINT(pxr)	((xpoint_t*)pxr)
#define RECTSIZE(pxr)	((xsize_t*)pxr + 1)

typedef struct _border_t{
	int title;
	int edge;
	int hscroll;
	int vscroll;
	int menu;
	int icon;
}border_t;

typedef struct _scroll_t{
	int pos;
	int min;
	int max;
	int page;
	int track;
}scroll_t;

typedef struct _shadow_t{
	int offx;
	int offy;
}shadow_t;

typedef struct _adorn_t{
	int feed;
	int size;
}adorn_t;

typedef struct _xbrush_t{
	tchar_t style[RES_LEN + 1];
	tchar_t opacity[INT_LEN + 1];
	tchar_t color[CLR_LEN + 1];
	tchar_t linear[CLR_LEN + 1];
	tchar_t gradient[RES_LEN + 1];
	shadow_t shadow;
}xbrush_t;

typedef struct _xpen_t{
	tchar_t style[RES_LEN + 1];
	tchar_t size[INT_LEN + 1];
	tchar_t opacity[INT_LEN + 1];
	tchar_t color[CLR_LEN + 1];
	adorn_t adorn;
}xpen_t;

typedef struct _xfont_t{
	tchar_t style[RES_LEN + 1];
	tchar_t size[INT_LEN + 1];
	tchar_t weight[INT_LEN + 1];
	tchar_t family[RES_LEN + 1];
	tchar_t color[CLR_LEN + 1];
}xfont_t;

typedef struct _xface_t{
	tchar_t text_wrap[RES_LEN + 1];
	tchar_t text_align[RES_LEN + 1];
	tchar_t line_align[RES_LEN + 1];
	tchar_t line_height[INT_LEN + 1];
}xface_t;

typedef struct _ximage_t{
	tchar_t style[RES_LEN + 1];
	tchar_t type[RES_LEN + 1];
	tchar_t color[CLR_LEN + 1];

	const tchar_t* source;
}ximage_t;

#if defined(_UNICODE) || defined(UNICODE)
#define _tprintf    wprintf
#else
#define _tprintf    printf
#endif

#ifndef _OS_WINDOWS
#ifndef min
#define min(x, y) ({                        \
    __typeof__(x) _min1 = (x);              \
    __typeof__(y) _min2 = (y);              \
    (void) (&_min1 == &_min2);              \
    _min1 < _min2 ? _min1 : _min2; })
#endif
    
#ifndef max
#define max(x, y) ({                         \
    __typeof__(x) _max1 = (x);               \
    __typeof__(y) _max2 = (y);               \
    (void) (&_max1 == &_max2);               \
    _max1 > _max2 ? _max1 : _max2; })
#endif
#endif

#ifdef _OS_WINDOWS
#ifndef snprintf
#define snprintf	_snprintf
#endif
#endif

#endif	/* _XDSFORM_H */

