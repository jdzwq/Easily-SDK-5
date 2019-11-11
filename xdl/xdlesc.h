/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl escape document

	@module	escape.h | interface file

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

#ifndef XDLESC_H
#define XDLESC_H

#include "xdldef.h"

//定义转义符
#define CH_ESC		_T('&')

#define CH_LT		_T('<')
#define A_LT		"lt;"	// < 
#define W_LT		L"lt;"	// < 
#ifdef _UNICODE
#define LT			W_LT
#else
#define LT			A_LT
#endif
#define LT_LEN		3

#define CH_GT		_T('>')
#define A_GT		"gt;"	// > 
#define W_GT		L"gt;"	// > 
#ifdef _UNICODE
#define GT			W_GT
#else
#define GT			A_GT
#endif
#define GT_LEN		3

#define CH_AMP		_T('&')
#define A_AMP		"amp;"	// & 
#define W_AMP		L"amp;"	// & 
#ifdef _UNICODE
#define AMP			W_AMP
#else
#define AMP			A_AMP
#endif
#define AMP_LEN		4

#define CH_APOS		_T('\'')
#define A_APOS		"apos;"	// ' 
#define W_APOS		L"apos;"	// ' 
#ifdef _UNICODE
#define APOS		W_APOS
#else
#define APOS		A_APOS
#endif
#define APOS_LEN	5

#define CH_QUOT		_T('\"')
#define A_QUOT		"quot;"	// " 
#define W_QUOT		L"quot;"	// " 
#ifdef _UNICODE
#define QUOT		W_QUOT
#else
#define QUOT		A_QUOT
#endif
#define QUOT_LEN	5

/****************************
#define CH_SPAC		_T(' ')
#define A_SPAC		"nbsp;" //  
#define W_SPAC		L"nbsp;" //  
#define SPAC_LEN	5		

#define CH_QDIV		_T('/')
#define A_QDIV		"div;" // / 
#define W_QDIV		L"div;" // / 
#define QDIV_LEN	4		

#define CH_PAGE		_T('\f')
#define A_PAGE		"page;"	// 
#define W_PAGE		L"page;"	// 
#ifdef _UNICODE
#define PAGE		W_PAGE
#else
#define PAGE		A_PAGE
#endif
#define PAGE_LEN	5

#define CH_CARR		_T('\r')
#define A_CARR		"carr;"	// 
#define W_CARR		L"carr;"	// 
#ifdef _UNICODE
#define CARR		W_CARR
#else
#define CARR		A_CARR
#endif
#define CARR_LEN	5
*********************************/

#define ESC_LEN		6

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API bool_t is_escape(const tchar_t* esc, tchar_t ch);

XDL_API int encode_escape(const tchar_t* esc, const tchar_t* val, int len, tchar_t* buf, int max);

XDL_API int decode_escape(const tchar_t* val, int len, tchar_t* buf, int max);

/**************************************************************************************/
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
XDL_API dword_t gb2312_decode_escape(const byte_t* src, tchar_t* dest);

XDL_API dword_t gb2312_encode_escape(tchar_t ch, byte_t* dest, dword_t max);
#endif

XDL_API dword_t utf8_decode_escape(const byte_t* src, tchar_t* dest);

XDL_API dword_t utf8_encode_escape(tchar_t ch, byte_t* dest, dword_t max);

XDL_API dword_t utf16lit_decode_escape(const byte_t* src, tchar_t* dest);

XDL_API dword_t utf16lit_encode_escape(tchar_t ch, byte_t* dest, dword_t max);

XDL_API dword_t utf16big_decode_escape(const byte_t* src, tchar_t* dest);

XDL_API dword_t utf16big_encode_escape(tchar_t ch, byte_t* dest, dword_t max);

XDL_API dword_t unn_decode_escape(const byte_t* src, tchar_t* dest);

XDL_API dword_t unn_encode_escape(tchar_t ch, byte_t* dest, dword_t max);

XDL_API int ucs_decode_escape(const wchar_t* src, tchar_t* dest);

XDL_API int ucs_encode_escape(tchar_t ch, wchar_t* dest, int max);

XDL_API int mbs_decode_escape(const schar_t* src, tchar_t* dest);

XDL_API int mbs_encode_escape(tchar_t ch, schar_t* dest, int max);

#ifdef	__cplusplus
}
#endif

#endif /*ESCAPE_H*/
