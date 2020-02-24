/***********************************************************************
	Easily xdl codepage

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc codepage document

	@module	acp.h | codepage interface file

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

#ifndef _ACP_H
#define _ACP_H

#define LIT_MAKESHORT(lc,hc)	((((unsigned short)(hc) << 8) & 0xFF00) | ((unsigned short)(lc) & 0x00FF))
#define LIT_GETHCHAR(sw)		(unsigned char)(((unsigned short)(sw) >> 8) & 0x00FF)
#define LIT_GETLCHAR(sw)		(unsigned char)((unsigned short)(sw) & 0x00FF)

#define BIG_MAKESHORT(lc,hc)	((((unsigned short)(lc) << 8) & 0xFF00) | ((unsigned short)(hc) & 0x00FF))
#define BIG_GETHCHAR(sw)		(unsigned char)((unsigned short)(sw) & 0x00FF)
#define BIG_GETLCHAR(sw)		(unsigned char)(((unsigned short)(sw) >> 8) & 0x00FF) 

#if defined(_WIN32) || defined(_WIN64) || defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || \
   defined(vax) || defined(ns32000) || defined(sun386) || \
   defined(MIPSEL) || defined(_MIPSEL) || defined(BIT_ZERO_ON_RIGHT) || \
   defined(__alpha__) || defined(__alpha)
#define ACP_BYTE_ORDER    1234
#endif

#if defined(sel) || defined(pyr) || defined(mc68000) || defined(sparc) || \
    defined(is68k) || defined(tahoe) || defined(ibm032) || defined(ibm370) || \
    defined(MIPSEB) || defined(_MIPSEB) || defined(_IBMR2) || defined(DGUX) ||\
    defined(apollo) || defined(__convex__) || defined(_CRAY) || \
    defined(__hppa) || defined(__hp9000) || \
    defined(__hp9000s300) || defined(__hp9000s700) || \
    defined (BIT_ZERO_ON_LEFT) || defined(m68k) || defined(__sparc)
#define ACP_BYTE_ORDER	4321
#endif

#if ACP_BYTE_ORDER == 4321
#define MAKESHORT			BIG_MAKESHORT
#define GETLCHAR			BIG_GETLCHAR
#define GETHCHAR			BIG_GETHCHAR
#else
#define MAKESHORT			LIT_MAKESHORT
#define GETLCHAR			LIT_GETLCHAR
#define GETHCHAR			LIT_GETHCHAR
#endif

#define ALT_CHAR	0x20

#if ACP_BYTE_ORDER == 4321
#define BIGBOM		0xFEFF
#define LITBOM		0xFFFE
#define DEFBOM		BIGBOM
#else
#define BIGBOM		0xFFFE
#define LITBOM		0xFEFF
#define DEFBOM		LITBOM
#endif

#ifdef __cplusplus
extern "C" {
#endif

	extern int utf16lit_code_sequence(unsigned char b);

	extern int utf16lit_seek_unicode(unsigned char* mbs, unsigned short* ucs);

	extern int unicode_seek_utf16lit(unsigned short ucs, unsigned char* mbs);

	extern int utf16big_code_sequence(unsigned char b);

	extern int utf16big_seek_unicode(unsigned char* mbs, unsigned short* ucs);

	extern int unicode_seek_utf16big(unsigned short ucs, unsigned char* mbs);

	extern int utf8_code_sequence(unsigned char b);

	extern int utf8_seek_unicode(unsigned char* src, unsigned short* dest);

	extern int unicode_seek_utf8(unsigned short ch, unsigned char* dest);

	extern int gb2312_code_sequence(unsigned char b);

	extern int unicode_seek_gb2312(unsigned short ucs, unsigned char* mbs);

	extern int gb2312_seek_unicode(unsigned char* mbs, unsigned short* ucs);

	/********************************************************************************************************/
	extern int unicode_gb2312_size(void);

	extern void unicode_gb2312_code(int index, unsigned short* code, unsigned short* val, unsigned short* key);

	extern int gb2312_unicode_size(void);

	extern void gb2312_unicode_code(int index, unsigned short* code, unsigned short* val, unsigned short* key);

	extern int unicode_seek_help(unsigned short ucs, unsigned short* hlp);

	extern int gb2312_seek_help(const unsigned char* mbs, unsigned char* hlp);


#ifdef __cplusplus
}
#endif

#endif /*_ACP_H*/
