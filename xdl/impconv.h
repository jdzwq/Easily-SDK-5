/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc code convert document

	@module	impconv.h | interface file

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

#ifndef _IMPCONV_H
#define _IMPCONV_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

	EXP_API bool_t is_ascii(byte_t b);

	EXP_API int ascii_byte_to_unicode(char b, wchar_t* c);

	EXP_API int unicode_byte_to_ascii(wchar_t c, char* b);

	EXP_API int ascii_to_unicode(const char* src, int len, wchar_t* dst, int max);

	EXP_API int unicode_to_ascii(const wchar_t* src, int len, char* dst, int max);

	EXP_API int gb2312_index(const byte_t* src);

	EXP_API int gbk_index(const byte_t* src);

	EXP_API int ucs_index(wchar_t wc);

#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	EXP_API dword_t gb2312_sequence(byte_t b);

	EXP_API int gb2312_byte_to_ucs(const byte_t* src, wchar_t* dest);

	EXP_API int gb2312_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen);

	EXP_API dword_t gb2312_byte_to_utf8(const byte_t* src, byte_t* dest);

	EXP_API dword_t gb2312_to_utf8(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t gb2312_byte_to_utf16lit(const byte_t* src, byte_t* dest);

	EXP_API dword_t gb2312_to_utf16lit(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t gb2312_byte_to_utf16big(const byte_t* src, byte_t* dest);

	EXP_API dword_t gb2312_to_utf16big(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t gb2312_byte_to_unn(const byte_t* src, byte_t* dest);

	EXP_API dword_t gb2312_to_unn(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API int gb2312_byte_to_mbs(const byte_t* src, schar_t* dest);

	EXP_API int gb2312_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen);
#endif

	EXP_API dword_t utf8_sequence(byte_t b);

	EXP_API int utf8_byte_to_ucs(const byte_t* src, wchar_t* dest);

	EXP_API int utf8_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen);

	EXP_API dword_t utf8_byte_to_utf16lit(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf8_to_utf16lit(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t utf8_byte_to_utf16big(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf8_to_utf16big(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t utf8_byte_to_unn(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf8_to_unn(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API int utf8_byte_to_mbs(const byte_t* src, schar_t* dest);

	EXP_API int utf8_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen);

#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	EXP_API dword_t utf8_byte_to_gb2312(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf8_to_gb2312(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);
#endif

	EXP_API dword_t utf16_sequence(byte_t b);

	EXP_API int utf16lit_byte_to_ucs(const byte_t* src, wchar_t* dest);

	EXP_API int utf16lit_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen);

	EXP_API dword_t utf16lit_byte_to_utf8(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf16lit_to_utf8(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t utf16lit_byte_to_unn(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf16lit_to_unn(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API int utf16lit_byte_to_mbs(const byte_t* src, schar_t* dest);

	EXP_API int utf16lit_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen);

#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	EXP_API dword_t utf16lit_byte_to_gb2312(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf16lit_to_gb2312(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);
#endif

	EXP_API int utf16big_byte_to_ucs(const byte_t* src, wchar_t* dest);

	EXP_API int utf16big_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen);

	EXP_API dword_t utf16big_byte_to_utf8(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf16big_to_utf8(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t utf16big_byte_to_unn(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf16big_to_unn(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API int utf16big_byte_to_mbs(const byte_t* src, schar_t* dest);

	EXP_API int utf16big_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen);

#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	EXP_API dword_t utf16big_byte_to_gb2312(const byte_t* src, byte_t* dest);

	EXP_API dword_t utf16big_to_gb2312(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);
#endif

	EXP_API dword_t ucs_sequence(wchar_t b);

	EXP_API dword_t ucs_byte_to_utf8(wchar_t ch, byte_t* dest);

	EXP_API dword_t ucs_to_utf8(const wchar_t* src, int slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t ucs_byte_to_utf16lit(wchar_t ch, byte_t* dest);

	EXP_API dword_t ucs_to_utf16lit(const wchar_t* src, int slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t ucs_byte_to_utf16big(wchar_t ch, byte_t* dest);

	EXP_API dword_t ucs_to_utf16big(const wchar_t* src, int slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t ucs_byte_to_unn(wchar_t ch, byte_t* dest);

	EXP_API dword_t ucs_to_unn(const wchar_t* src, int slen, byte_t* dest, dword_t dlen);

	EXP_API int ucs_byte_to_mbs(wchar_t ch, schar_t* buf);

	EXP_API int ucs_to_mbs(const wchar_t* src, int slen, schar_t* dest, int dlen);

#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	EXP_API dword_t ucs_byte_to_gb2312(wchar_t ch, byte_t* dest);

	EXP_API dword_t ucs_to_gb2312(const wchar_t* src, int slen, byte_t* dest, dword_t dlen);
#endif

	EXP_API dword_t mbs_sequence(schar_t b);

	EXP_API int mbs_byte_to_ucs(const schar_t* src, wchar_t* dest);

	EXP_API int mbs_to_ucs(const schar_t* src, int slen, wchar_t* dest, int dlen);

	EXP_API dword_t mbs_byte_to_utf8(const schar_t* src, byte_t* dest);

	EXP_API dword_t mbs_to_utf8(const schar_t* src, int slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t mbs_byte_to_utf16lit(const schar_t* src, byte_t* dest);

	EXP_API dword_t mbs_to_utf16lit(const schar_t* src, int slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t mbs_byte_to_utf16big(const schar_t* src, byte_t* dest);

	EXP_API dword_t mbs_to_utf16big(const schar_t* src, int slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t mbs_byte_to_unn(const schar_t* src, byte_t* dest);

	EXP_API dword_t mbs_to_unn(const schar_t* src, int slen, byte_t* dest, dword_t dlen);

	EXP_API int mbs_byte_to_mbs(const schar_t* src, schar_t* dest);

	EXP_API int mbs_to_mbs(const schar_t* src, int slen, schar_t* dest, int dlen);

#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	EXP_API dword_t mbs_byte_to_gb2312(const schar_t* src, byte_t* dest);

	EXP_API dword_t mbs_to_gb2312(const schar_t* src, int slen, byte_t* dest, dword_t dlen);
#endif

	EXP_API dword_t unn_sequence(byte_t b);

	EXP_API int unn_byte_to_ucs(const byte_t* src, wchar_t* dest);

	EXP_API int unn_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen);

	EXP_API dword_t unn_byte_to_utf8(const byte_t* src, byte_t* dest);

	EXP_API dword_t unn_to_utf8(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t unn_byte_to_utf16lit(const byte_t* src, byte_t* dest);

	EXP_API dword_t unn_to_utf16lit(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API dword_t unn_byte_to_utf16big(const byte_t* src, byte_t* dest);

	EXP_API dword_t unn_to_utf16big(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);

	EXP_API int unn_byte_to_mbs(const byte_t* src, schar_t* dest);

	EXP_API int unn_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen);

#if defined(XDL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	EXP_API dword_t unn_byte_to_gb2312(const byte_t* src, byte_t* dest);

	EXP_API dword_t unn_to_gb2312(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*_XDLCONV_H*/
