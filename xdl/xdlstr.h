/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdlstr document

	@module	xdlstr.h | str library interface file

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

#ifndef _XDLSTR_H
#define _XDLSTR_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API bool_t a_is_null(const schar_t* sz);
XDL_API bool_t w_is_null(const wchar_t* sz);

XDL_API schar_t* a_xsalloc(int len);
XDL_API wchar_t* w_xsalloc(int len);

XDL_API schar_t* a_xsrealloc(schar_t* str,int len);
XDL_API wchar_t* w_xsrealloc(wchar_t* str,int len);

XDL_API void a_xsfree(schar_t* str);
XDL_API void w_xsfree(wchar_t* str);

XDL_API void a_xszero(schar_t* str,int len);
XDL_API void w_xszero(wchar_t* str,int len);

XDL_API int a_xslen(const schar_t* str);
XDL_API int w_xslen(const wchar_t* str);

XDL_API const schar_t* a_xsnskip(const schar_t* sz, int max);
XDL_API const wchar_t* w_xsnskip(const wchar_t* sz, int max);

XDL_API const schar_t* a_xsskip(const schar_t* sz);
XDL_API const wchar_t* w_xsskip(const wchar_t* sz);

XDL_API bool_t a_is_numeric(const schar_t* sz);
XDL_API bool_t w_is_numeric(const wchar_t* sz);

XDL_API short a_xsntos(const schar_t* sz, int n);
XDL_API short w_xsntos(const wchar_t* sz, int n);

XDL_API short a_xstos(const schar_t* sz);
XDL_API short w_xstos(const wchar_t* sz);

XDL_API long a_xsntol(const schar_t* sz, int n);
XDL_API long w_xsntol(const wchar_t* sz, int n);

XDL_API long a_xstol(const schar_t* sz);
XDL_API long w_xstol(const wchar_t* sz);

XDL_API float a_xsntof(const schar_t* sz, int n);
XDL_API float w_xsntof(const wchar_t* sz, int n);

XDL_API float a_xstof(const schar_t* sz);
XDL_API float w_xstof(const wchar_t* sz);

XDL_API unsigned long a_hexntol(const schar_t* sz, int n);
XDL_API unsigned long w_hexntol(const wchar_t* sz, int n);

XDL_API unsigned long a_hextol(const schar_t* sz);
XDL_API unsigned long w_hextol(const wchar_t* sz);

XDL_API double a_xsntonum_dig(const schar_t* sz, int n, int digi);
XDL_API double w_xsntonum_dig(const wchar_t* sz, int n, int digi);

XDL_API double a_xsntonum(const schar_t* sz, int n);
XDL_API double w_xsntonum(const wchar_t* sz, int n);

XDL_API double a_xstonum(const schar_t* sz);
XDL_API double w_xstonum(const wchar_t* sz);

XDL_API long long a_xsntoll(const schar_t* sz, int n);
XDL_API long long w_xsntoll(const wchar_t* sz, int n);

XDL_API long long a_xstoll(const schar_t* sz);
XDL_API long long w_xstoll(const wchar_t* sz);

XDL_API int a_lltoxs(long long ll, schar_t* buf, int n);
XDL_API int w_lltoxs(long long ll, wchar_t* buf, int n);

XDL_API int a_stoxs(short s, schar_t* buf, int n);
XDL_API int w_stoxs(short s, wchar_t* buf, int n);

XDL_API int a_ustoxs(unsigned short s, schar_t* buf, int n);
XDL_API int w_ustoxs(unsigned short s, wchar_t* buf, int n);

XDL_API int a_ltoxs(long s, schar_t* buf, int n);
XDL_API int w_ltoxs(long s, wchar_t* buf, int n);

XDL_API int a_ultoxs(unsigned long s, schar_t* buf, int n);
XDL_API int w_ultoxs(unsigned long s, wchar_t* buf, int n);

XDL_API int a_ltohex(unsigned long s, schar_t type, schar_t* buf, int n);
XDL_API int w_ltohex(unsigned long s, wchar_t type, wchar_t* buf, int n);

XDL_API int a_ftoxs(float s, schar_t* buf, int n);
XDL_API int w_ftoxs(float s, wchar_t* buf, int n);

XDL_API int a_numtoxs_dig(double s, int digi, schar_t* buf, int n);
XDL_API int w_numtoxs_dig(double s, int digi, wchar_t* buf, int n);

XDL_API int a_numtoxs(double s, schar_t* buf, int n);
XDL_API int w_numtoxs(double s, wchar_t* buf, int n);

XDL_API void a_xsntriml(schar_t* sz, int n);
XDL_API void w_xsntriml(wchar_t* sz, int n);

XDL_API void a_xstriml(schar_t* sz);
XDL_API void w_xstriml(wchar_t* sz);

XDL_API void a_xsntrimr(schar_t* sz, int n);
XDL_API void w_xsntrimr(wchar_t* sz, int n);

XDL_API void a_xstrimr(schar_t* sz);
XDL_API void w_xstrimr(wchar_t* sz);

XDL_API void a_xsntrim(schar_t* sz, int n);
XDL_API void w_xsntrim(wchar_t* sz, int n);

XDL_API void a_xstrim(schar_t* sz);
XDL_API void w_xstrim(wchar_t* sz);

XDL_API void a_xsnupr(schar_t* sz, int n);
XDL_API void w_xsnupr(wchar_t* sz, int n);

XDL_API void a_xsupr(schar_t* sz);
XDL_API void w_xsupr(wchar_t* sz);

XDL_API void a_xsnlwr(schar_t* sz, int n);
XDL_API void w_xsnlwr(wchar_t* sz, int n);

XDL_API void a_xslwr(schar_t* sz);
XDL_API void w_xslwr(wchar_t* sz);

XDL_API void a_xsndel(schar_t* str, int start,int len);
XDL_API void w_xsndel(wchar_t* str, int start, int len);

XDL_API void a_xsdel(schar_t* str, int start);
XDL_API void w_xsdel(wchar_t* str, int start);

XDL_API void a_xsnins(schar_t* str, int index, const schar_t* sub, int len);
XDL_API void w_xsnins(wchar_t* str, int index, const wchar_t* sub, int len);

XDL_API void a_xsins(schar_t* str, int index, const schar_t* sub);
XDL_API void w_xsins(wchar_t* str, int index, const wchar_t* sub);

XDL_API void a_xsnset(schar_t* str, int index, const schar_t* sub, int len);
XDL_API void w_xsnset(wchar_t* str, int index, const wchar_t* sub, int len);

XDL_API void a_xsset(schar_t* str, int index, const schar_t* sub);
XDL_API void w_xsset(wchar_t* str, int index, const wchar_t* sub);

XDL_API void a_xsncpy(schar_t* dest, const schar_t* src, int n);
XDL_API void w_xsncpy(wchar_t* dest, const wchar_t* src, int n);

XDL_API void a_xscpy(schar_t* dest, const schar_t* src);
XDL_API void w_xscpy(wchar_t* dest, const wchar_t* src);

XDL_API void a_xsncat(schar_t* dest, const schar_t* src,int n);
XDL_API void w_xsncat(wchar_t* dest, const wchar_t* src,int n);

XDL_API void a_xscat(schar_t* dest, const schar_t* src);
XDL_API void w_xscat(wchar_t* dest, const wchar_t* src);

XDL_API void a_xsnmid(schar_t* dest, const schar_t* src,int from,int len);
XDL_API void w_xsnmid(wchar_t* dest, const wchar_t* src,int from,int len);

XDL_API void a_xsmid(schar_t* dest, const schar_t* src, int from);
XDL_API void w_xsmid(wchar_t* dest, const wchar_t* src, int from);

XDL_API int a_xscmp(const schar_t* dest,const schar_t* src);
XDL_API int w_xscmp(const wchar_t* dest,const wchar_t* src);

XDL_API int a_xsncmp(const schar_t* dest,const schar_t* src,int n);
XDL_API int w_xsncmp(const wchar_t* dest,const wchar_t* src,int n);

XDL_API int a_xsicmp(const schar_t* dest,const schar_t* src);
XDL_API int w_xsicmp(const wchar_t* dest,const wchar_t* src);

XDL_API int a_xsnicmp(const schar_t* dest,const schar_t* src,int n);
XDL_API int w_xsnicmp(const wchar_t* dest,const wchar_t* src,int n);

XDL_API const schar_t* a_xsnstr(const schar_t* str,int len,const schar_t* sub);
XDL_API const wchar_t* w_xsnstr(const wchar_t* str,int len,const wchar_t* sub);

XDL_API const schar_t* a_xsstr(const schar_t* str, const schar_t* sub);
XDL_API const wchar_t* w_xsstr(const wchar_t* str, const wchar_t* sub);

XDL_API const schar_t* a_xsnistr(const schar_t* str,int len,const schar_t* sub);
XDL_API const wchar_t* w_xsnistr(const wchar_t* str,int len,const wchar_t* sub);

XDL_API const schar_t* a_xsistr(const schar_t* str, const schar_t* sub);
XDL_API const wchar_t* w_xsistr(const wchar_t* str, const wchar_t* sub);

XDL_API const schar_t* a_kmpnstr(const schar_t* str, int len, const schar_t* sub);
XDL_API const wchar_t* w_kmpnstr(const wchar_t* str, int len, const wchar_t* sub);

XDL_API const schar_t* a_kmpstr(const schar_t* str, const schar_t* sub);
XDL_API const wchar_t* w_kmpstr(const wchar_t* str, const wchar_t* sub);

XDL_API const schar_t* a_kmpnistr(const schar_t* str, int len, const schar_t* sub);
XDL_API const wchar_t* w_kmpnistr(const wchar_t* str, int len, const wchar_t* sub);

XDL_API const schar_t* a_kmpistr(const schar_t* str, const schar_t* sub);
XDL_API const wchar_t* w_kmpistr(const wchar_t* str, const wchar_t* sub);

XDL_API void a_xsnrev(schar_t* str, int len);
XDL_API void w_xsnrev(wchar_t* str, int len);

XDL_API void a_xsrev(schar_t* str);
XDL_API void w_xsrev(wchar_t* str);

XDL_API schar_t* a_xsnclone(const schar_t* str, int len);
XDL_API wchar_t* w_xsnclone(const wchar_t* str, int len);

XDL_API schar_t* a_xsclone(const schar_t* str);
XDL_API wchar_t* w_xsclone(const wchar_t* str);

XDL_API bool_t a_is_prefix(const schar_t* str, const schar_t* sub);
XDL_API bool_t w_is_prefix(const wchar_t* str, const wchar_t* sub);

XDL_API bool_t a_is_suffix(const schar_t* str, const schar_t* sub);
XDL_API bool_t w_is_suffix(const wchar_t* str, const wchar_t* sub);

XDL_API int a_xsprintf_arg(schar_t* buf,const schar_t* fmt,va_list* parg);
XDL_API int w_xsprintf_arg(wchar_t* buf,const wchar_t* fmt,va_list* parg);

XDL_API int a_xsprintf(schar_t* buf,const schar_t* fmt,...);
XDL_API int w_xsprintf(wchar_t* buf,const wchar_t* fmt,...);

XDL_API int a_xsappend(schar_t* buf, const schar_t* fmt, ...);
XDL_API int w_xsappend(wchar_t* buf, const wchar_t* fmt, ...);

XDL_API const schar_t* a_xscanf(const schar_t* str, const schar_t* fmt, ...);
XDL_API const wchar_t* w_xscanf(const wchar_t* str, const wchar_t* fmt, ...);

XDL_API const schar_t* a_xscanf_arg(const schar_t* str, const schar_t* fmt, va_list* parg);
XDL_API const wchar_t* w_xscanf_arg(const wchar_t* str, const wchar_t* fmt, va_list* parg);

#ifdef	__cplusplus
}
#endif

#if defined(_UNICODE) || defined(UNICODE)
#define xsalloc				w_xsalloc
#define xsrealloc			w_xsrealloc
#define xsfree				w_xsfree
#define xszero				w_xszero
#define is_null				w_is_null
#define is_numeric			w_is_numeric
#define xsntos				w_xsntos
#define xstos				w_xstos
#define xstous				(unsigned short)w_xstos
#define xsntol				w_xsntol
#define xstol				w_xstol
#define xstoul				(unsigned long)w_xstol
#define xsntof				w_xsntof
#define xstof				w_xstof
#define xsntonum_dig		w_xsntonum_dig
#define xsntonum			w_xsntonum
#define xstonum				w_xstonum
#define xsntoll				w_xsntoll
#define xstoll				w_xstoll
#define hexntol				w_hexntol
#define hextol				w_hextol
#define xsntriml			w_xsntriml
#define xstriml				w_xstriml
#define xsntriml			w_xsntriml
#define xstrimr				w_xstrimr
#define xsntrim				w_xsntrim
#define xstrim				w_xstrim
#define xsndel				w_xsndel
#define xsdel				w_xsdel
#define xsnins				w_xsnins
#define xsins				w_xsins
#define xsnset				w_xsnset
#define xsset				w_xsset
#define xslen				w_xslen
#define xscpy				w_xscpy
#define xsncpy				w_xsncpy
#define xscat				w_xscat
#define xsncat				w_xsncat
#define xsnmid				w_xsnmid
#define xsmid				w_xsmid
#define xscmp				w_xscmp
#define xsncmp				w_xsncmp
#define xsicmp				w_xsicmp
#define xsnicmp				w_xsnicmp
#define xsnupr				w_xsnupr
#define xsupr				w_xsupr
#define xsnlwr				w_xsnlwr
#define xslwr				w_xslwr
#define xsnrev				w_xsnrev
#define xsrev				w_xsrev
#define xsnstr				w_xsnstr
#define xsstr				w_xsstr
#define xsnistr				w_xsnistr
#define xsistr				w_xsistr
#define kmpnstr				w_kmpnstr
#define kmpstr				w_kmpstr
#define kmpnistr			w_kmpnistr
#define kmpistr				w_kmpistr
#define xsnclone			w_xsnclone
#define xsclone				w_xsclone
#define is_prefix			w_is_prefix
#define is_suffix			w_is_suffix
#define xsprintf			w_xsprintf
#define xsprintf_arg		w_xsprintf_arg
#define xsappend			w_xsappend
#define xscanf				w_xscanf
#define xscanf_arg			w_xscanf_arg

#define stoxs				w_stoxs
#define ustoxs				w_ustoxs
#define ltoxs				w_ltoxs
#define ultoxs				w_ultoxs
#define ftoxs				w_ftoxs
#define lltoxs				w_lltoxs
#define ltohex				w_ltohex
#define numtoxs_dig			w_numtoxs_dig
#define numtoxs				w_numtoxs

#define xschs(pch)			(((pch) && *(pch))? ucs_sequence(*(pch)) : 0)

#else

#define xsalloc				a_xsalloc
#define xsrealloc			a_xsrealloc
#define xsfree				a_xsfree
#define xszero				a_xszero
#define is_null				a_is_null
#define is_numeric			a_is_numeric
#define xsntos				a_xsntos
#define xstos				a_xstos
#define xstous				(unsigned short)a_xstos
#define xsntol				a_xsntol
#define xstol				a_xstol
#define xstoul				(unsigned long)w_xstol
#define xsntof				a_xsntof
#define xstof				a_xstof
#define xsntonum_dig		a_xsntonum_dig
#define xstonum				a_xstonum
#define xsntonum			a_xsntonum
#define xstoll				a_xstoll
#define xsntoll				a_xsntoll
#define hexntol				a_hexntol
#define hextol				a_hextol
#define xsntriml			a_xsntriml
#define xstriml				a_xstriml
#define xsntrimr			a_xsntrimr
#define xstrimr				a_xstrimr
#define xsntrim				a_xsntrim
#define xstrim				a_xstrim
#define xsndel				a_xsndel
#define xsdel				a_xsdel
#define xsnins				a_xsnins
#define xsins				a_xsins
#define xsnset				a_xsnset
#define xsset				a_xsset
#define xslen				a_xslen
#define xscpy				a_xscpy
#define xsncpy				a_xsncpy
#define xscat				a_xscat
#define xsncat				a_xsncat
#define xsnmid				a_xsnmid
#define xsmid				a_xsmid
#define xscmp				a_xscmp
#define xsncmp				a_xsncmp
#define xsicmp				a_xsicmp
#define xsnicmp				a_xsnicmp
#define xsupr				a_xsupr
#define xsnupr				a_xsnupr
#define xslwr				a_xslwr
#define xsnlwr				a_xsnlwr
#define xsrev				a_xsrev
#define xsnrev				a_xsnrev
#define xsnstr				a_xsnstr
#define xsstr				a_xsstr
#define xsnistr				a_xsnistr
#define xsistr				a_xsistr
#define kmpnstr				a_kmpnstr
#define kmpstr				a_kmpstr
#define kmpnistr			a_kmpnistr
#define kmpistr				a_kmpistr
#define xsclone				a_xsclone
#define xsnclone			a_xsnclone
#define is_prefix			a_is_prefix
#define is_suffix			a_is_suffix

#define xsprintf			a_xsprintf
#define xsprintf_arg		a_xsprintf_arg
#define xsappend			a_xsappend
#define xscanf				a_xscanf
#define xscanf_arg			a_xscanf_arg

#define stoxs				a_stoxs
#define ustoxs				a_ustoxs
#define ltoxs				a_ltoxs
#define ultoxs				a_ultoxs
#define ftoxs				a_ftoxs
#define lltoxs				a_lltoxs
#define ltohex				a_ltohex
#define numtoxs_dig			a_numtoxs_dig
#define numtoxs				a_numtoxs

#define xschs(pch)			(((pch) && *(pch))? mbs_sequence(*(pch)) : 0)

#endif /*UNICODE*/


#endif /*_STRLIB_H*/