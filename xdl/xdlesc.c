/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc escape document

	@module	escape.c | implement file

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

#include "xdlesc.h"
#include "impmem.h"
#include "xdlstr.h"
#include "oemconv.h"


static tchar_t csv_esc[] = { _T('%'), _T('\t'), _T('\r'), _T('\n'), _T('\0') };

bool_t _is_csvesc(tchar_t ch)
{
	int i = 0;
	while (*(csv_esc + i) && *(csv_esc + i) != ch)
		i++;

	return (*(csv_esc + i)) ? 1 : 0;
}

int csv_char_encode(const tchar_t* val, int len, tchar_t* buf, int max)
{
	const tchar_t* token = val;
	int pos, total = 0;
	tchar_t num[3] = { 0 };

	if (len < 0)
		len = xslen(val);

	pos = 0;
	while (pos < len)
	{
		if (_is_csvesc(*token))
		{
			if (total + 3 > max)
				return total;

			if (buf)
			{
				buf[total] = _T('%');
				xsprintf(buf + total + 1, _T("%02X"), (long)(*token));
			}
			total += 3;
		}
		else
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = *token;
			}
			total++;
		}

		token++;
		pos++;
	}

	return total;
}

int csv_char_decode(const tchar_t* val, int len, tchar_t* buf, int max)
{
	const tchar_t* token = val;
	int pos, total = 0;
	tchar_t num[3] = { 0 };

	if (len < 0)
		len = xslen(val);

	pos = 0;
	while (pos < len)
	{
		if (*token == _T('%'))
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = (int)hexntol(token + 1, 2);
			}
			total++;
			pos += 3;
			token += 3;
		}
		else
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = *token;
			}
			total++;
			pos++;
			token++;
		}
	}

	return total;
}
/***************************************************************************************/
//static tchar_t url_esc[] = { _T(' '), _T('"'),_T('#'), _T('%'), _T('&'), _T('('), _T(')'), _T('+'), _T(','), _T('/'), _T(':'), _T(';'), _T('<'), _T('='), _T('>'), _T('?'), _T('@'), _T('\\'), _T('|'), _T('\0') };
//#define ASC_LEN	16
//static tchar_t url_esc[ASC_LEN] = { _T(' '), _T('"'), _T('#'), _T('%'), _T('('), _T(')'), _T('+'), _T(','), _T(':'), _T(';'), _T('<'), _T('>'), _T('?'), _T('@'), _T('\\'), _T('|'), _T('\0')  };

static tchar_t url_asc[] = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~");

static bool_t _is_urlesc(byte_t ch)
{
	if (ch >= ('A') && ch <= ('Z'))
		return 0;

	if (ch >= ('a') && ch <= ('z'))
		return 0;

	if (ch >= ('0') && ch <= ('9'))
		return 0;

	if (ch == ('-') || ch == ('_') || ch == ('.') || ch == ('~'))
		return 0;

	return 1;
}

dword_t url_byte_encode(const byte_t* val, dword_t len, byte_t* buf, dword_t max)
{
	const byte_t* token = val;
	dword_t pos, total = 0;
	schar_t num[3] = { 0 };

	pos = 0;
	while (pos < len)
	{
		if (_is_urlesc(*token))
		{
			if (total + 3 > max)
				return total;

			if (buf)
			{
				buf[total] = ('%');
				a_xsprintf((schar_t*)(buf + total + 1), ("%02X"), (long)(*token));
			}
			total += 3;
		}
		else
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = *token;
			}
			total++;
		}

		token++;
		pos++;
	}

	return total;
}

dword_t url_byte_decode(const byte_t* val, dword_t len, byte_t* buf, dword_t max)
{
	const byte_t* token = val;
	dword_t pos, total = 0;
	schar_t num[3] = { 0 };

	pos = 0;
	while (pos < len)
	{
		if (*token == ('%'))
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = (int)a_hexntol((schar_t*)(token + 1), 2);
			}
			total++;
			pos += 3;
			token += 3;
		}
		else
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = (int)(*token);
			}
			total++;
			pos++;
			token++;
		}
	}

	return total;
}

/****************************************************************************************/
dword_t xml_utf16lit_decode(const byte_t* src, tchar_t* dest)
{
	wchar_t pch[ESC_LEN + 1] = { 0 };
	dword_t pos = 0;
	int index = 0;

	while (index < ESC_LEN)
	{
		utf16lit_byte_to_ucs(src + pos, pch + index);
		pos += utf16_sequence(*(src + pos));

		if (*(pch + index) == L'\0')
			break;

		index++;

		if (!w_xsncmp(pch, W_LT, LT_LEN))
		{
			if (dest)
				*dest = _T('<');
			return pos;
		}
		else if (!w_xsncmp(pch, W_GT, GT_LEN))
		{
			if (dest)
				*dest = _T('>');
			return pos;
		}
		else if (!w_xsncmp(pch, W_AMP, AMP_LEN))
		{
			if (dest)
				*dest = _T('&');
			return pos;
		}
		else if (!w_xsncmp(pch, W_APOS, APOS_LEN))
		{
			if (dest)
				*dest = _T('\'');
			return pos;
		}
		else if (!w_xsncmp(pch, W_QUOT, QUOT_LEN))
		{
			if (dest)
				*dest = _T('\"');
			return pos;
		}
		/*else if (!w_xsncmp(pch, W_QDIV, QDIV_LEN))
		{
		if (dest)
		*dest = _T('/');
		return pos;
		}
		else if (!w_xsncmp(pch, W_PAGE, PAGE_LEN))
		{
		if (dest)
		*dest = _T('\f');
		return pos;
		}*/
	}

	if (dest)
	{
#ifdef _UNICODE
		xsncpy(dest, pch, index);
#else
		ucs_to_mbs(pch, index, dest, ESC_LEN);
#endif
	}

	return pos;
}

dword_t xml_utf16big_decode(const byte_t* src, tchar_t* dest)
{
	wchar_t pch[ESC_LEN + 1] = { 0 };
	dword_t pos = 0;
	int index = 0;

	while (index < ESC_LEN)
	{
		utf16big_byte_to_ucs(src + pos, pch + index);
		pos += utf16_sequence(*(src + pos));

		if (*(pch + index) == L'\0')
			break;

		index++;

		if (!w_xsncmp(pch, W_LT, LT_LEN))
		{
			if (dest)
				*dest = _T('<');
			return pos;
		}
		else if (!w_xsncmp(pch, W_GT, GT_LEN))
		{
			if (dest)
				*dest = _T('>');
			return pos;
		}
		else if (!w_xsncmp(pch, W_AMP, AMP_LEN))
		{
			if (dest)
				*dest = _T('&');
			return pos;
		}
		else if (!w_xsncmp(pch, W_APOS, APOS_LEN))
		{
			if (dest)
				*dest = _T('\'');
			return pos;
		}
		else if (!w_xsncmp(pch, W_QUOT, QUOT_LEN))
		{
			if (dest)
				*dest = _T('\"');
			return pos;
		}
		/*else if (!w_xsncmp(pch, W_QDIV, QDIV_LEN))
		{
		if (dest)
		*dest = _T('/');
		return pos;
		}
		else if (!w_xsncmp(pch, W_PAGE, PAGE_LEN))
		{
		if (dest)
		*dest = _T('\f');
		return pos;
		}*/
	}

	if (dest)
	{
#ifdef _UNICODE
		xsncpy(dest, pch, index);
#else
		ucs_to_mbs(pch, index, dest, ESC_LEN);
#endif
	}

	return pos;
}

dword_t xml_utf8_decode(const byte_t* src, tchar_t* dest)
{
	wchar_t pch[ESC_LEN + 1] = { 0 };
	dword_t pos = 0;
	int index = 0;

	while (index < ESC_LEN)
	{
		utf8_byte_to_ucs(src + pos, pch + index);
		pos += utf8_sequence(*(src + pos));

		if (*(pch + index) == L'\0')
			break;

		index++;

		if (!w_xsncmp(pch, W_LT, LT_LEN))
		{
			if (dest)
				*dest = _T('<');
			return pos;
		}
		else if (!w_xsncmp(pch, W_GT, GT_LEN))
		{
			if (dest)
				*dest = _T('>');
			return pos;
		}
		else if (!w_xsncmp(pch, W_AMP, AMP_LEN))
		{
			if (dest)
				*dest = _T('&');
			return pos;
		}
		else if (!w_xsncmp(pch, W_APOS, APOS_LEN))
		{
			if (dest)
				*dest = _T('\'');
			return pos;
		}
		else if (!w_xsncmp(pch, W_QUOT, QUOT_LEN))
		{
			if (dest)
				*dest = _T('\"');
			return pos;
		}
		/*else if (!w_xsncmp(pch, W_QDIV, QDIV_LEN))
		{
		if (dest)
		*dest = _T('/');
		return pos;
		}
		else if (!w_xsncmp(pch, W_PAGE, PAGE_LEN))
		{
		if (dest)
		*dest = _T('\f');
		return pos;
		}*/
	}

	if (dest)
	{
#ifdef _UNICODE
		xsncpy(dest, pch, index);
#else
		ucs_to_mbs(pch, index, dest, ESC_LEN);
#endif
	}

	return pos;
}

#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
dword_t xml_gb2312_decode(const byte_t* src, tchar_t* dest)
{
	wchar_t pch[ESC_LEN + 1] = { 0 };
	dword_t pos = 0;
	int index = 0;

	while (index < ESC_LEN)
	{
		gb2312_byte_to_ucs(src + pos, pch + index);
		pos += gb2312_sequence(*(src + pos));

		if (*(pch + index) == L'\0')
			break;

		index++;

		if (!w_xsncmp(pch, W_LT, LT_LEN))
		{
			if (dest)
				*dest = _T('<');
			return pos;
		}
		else if (!w_xsncmp(pch, W_GT, GT_LEN))
		{
			if (dest)
				*dest = _T('>');
			return pos;
		}
		else if (!w_xsncmp(pch, W_AMP, AMP_LEN))
		{
			if (dest)
				*dest = _T('&');
			return pos;
		}
		else if (!w_xsncmp(pch, W_APOS, APOS_LEN))
		{
			if (dest)
				*dest = _T('\'');
			return pos;
		}
		else if (!w_xsncmp(pch, W_QUOT, QUOT_LEN))
		{
			if (dest)
				*dest = _T('\"');
			return pos;
		}
		/*else if (!w_xsncmp(pch, W_QDIV, QDIV_LEN))
		{
		if (dest)
		*dest = _T('/');
		return pos;
		}
		else if (!w_xsncmp(pch, W_PAGE, PAGE_LEN))
		{
		if (dest)
		*dest = _T('\f');
		return pos;
		}*/
	}

	if (dest)
	{
#ifdef _UNICODE
		xsncpy(dest, pch, index);
#else
		ucs_to_mbs(pch, index, dest, ESC_LEN);
#endif
	}

	return pos;
}
#endif

dword_t xml_unn_decode(const byte_t* src, tchar_t* dest)
{
	wchar_t pch[ESC_LEN + 1] = { 0 };
	dword_t pos = 0;
	int index = 0;

	while (index < ESC_LEN)
	{
		unn_byte_to_ucs(src + pos, pch + index);
		pos += unn_sequence(*(src + pos));

		if (*(pch + index) == L'\0')
			break;

		index++;

		if (!w_xsncmp(pch, W_LT, LT_LEN))
		{
			if (dest)
				*dest = _T('<');
			return pos;
		}
		else if (!w_xsncmp(pch, W_GT, GT_LEN))
		{
			if (dest)
				*dest = _T('>');
			return pos;
		}
		else if (!w_xsncmp(pch, W_AMP, AMP_LEN))
		{
			if (dest)
				*dest = _T('&');
			return pos;
		}
		else if (!w_xsncmp(pch, W_APOS, APOS_LEN))
		{
			if (dest)
				*dest = _T('\'');
			return pos;
		}
		else if (!w_xsncmp(pch, W_QUOT, QUOT_LEN))
		{
			if (dest)
				*dest = _T('\"');
			return pos;
		}
		/*else if (!w_xsncmp(pch, W_QDIV, QDIV_LEN))
		{
		if (dest)
		*dest = _T('/');
		return pos;
		}
		else if (!w_xsncmp(pch, W_PAGE, PAGE_LEN))
		{
		if (dest)
		*dest = _T('\f');
		return pos;
		}*/
	}

	if (dest)
	{
#ifdef _UNICODE
		xsncpy(dest, pch, index);
#else
		ucs_to_mbs(pch, index, dest, ESC_LEN);
#endif
	}

	return pos;
}

int xml_ucs_decode(const wchar_t* src, tchar_t* dest)
{
	if (!w_xsncmp(src, W_LT, LT_LEN))
	{
		if (dest)
			*dest = _T('<');
		return LT_LEN;
	}
	else if (!w_xsncmp(src, W_GT, GT_LEN))
	{
		if (dest)
			*dest = _T('>');
		return GT_LEN;
	}
	else if (!w_xsncmp(src, W_AMP, AMP_LEN))
	{
		if (dest)
			*dest = _T('&');
		return AMP_LEN;
	}
	else if (!w_xsncmp(src, W_APOS, APOS_LEN))
	{
		if (dest)
			*dest = _T('\'');
		return APOS_LEN;
	}
	else if (!w_xsncmp(src, W_QUOT, QUOT_LEN))
	{
		if (dest)
			*dest = _T('\"');
		return QUOT_LEN;
	}
	/*else if (!w_xsncmp(src, W_QDIV, QDIV_LEN))
	{
	if (dest)
	*dest = _T('/');
	return QDIV_LEN;
	}
	else if (!w_xsncmp(src, W_PAGE, PAGE_LEN))
	{
	if (dest)
	*dest = _T('\f');
	return PAGE_LEN;
	}*/

	if (dest)
	{
#ifdef _UNICODE
		dest[0] = src[0];
#else
		ucs_to_mbs(src, 1, dest, ESC_LEN);
#endif
	}

#ifdef _UNICODE
	return 1;
#else
	return ucs_sequence(*src);
#endif
}

int xml_mbs_decode(const schar_t* src, tchar_t* dest)
{
	if (!a_xsncmp(src, A_LT, LT_LEN))
	{
		if (dest)
			*dest = _T('<');
		return LT_LEN;
	}
	else if (!a_xsncmp(src, A_GT, GT_LEN))
	{
		if (dest)
			*dest = _T('>');
		return GT_LEN;
	}
	else if (!a_xsncmp(src, A_AMP, AMP_LEN))
	{
		if (dest)
			*dest = _T('&');
		return AMP_LEN;
	}
	else if (!a_xsncmp(src, A_APOS, APOS_LEN))
	{
		if (dest)
			*dest = _T('\'');
		return APOS_LEN;
	}
	else if (!a_xsncmp(src, A_QUOT, QUOT_LEN))
	{
		if (dest)
			*dest = _T('\"');
		return QUOT_LEN;
	}
	/*else if (!a_xsncmp(src, A_QDIV, QDIV_LEN))
	{
	if (dest)
	*dest = _T('/');
	return QDIV_LEN;
	}
	else if (!a_xsncmp(src, A_PAGE, PAGE_LEN))
	{
	if (dest)
	*dest = _T('\f');
	return PAGE_LEN;
	}*/

	if (dest)
	{
#ifdef _UNICODE
		mbs_byte_to_ucs(src, dest);
#else
		dest[0] = src[0];
#endif
	}

#ifdef _UNICODE
	return mbs_sequence(*src);
#else
	return 1;
#endif
}

//将正常串转换为转义串
dword_t xml_utf16lit_encode(tchar_t ch, byte_t* dest, dword_t max)
{
	if (ch == _T('<'))
	{
#ifdef _UNICODE
		return ucs_to_utf16lit(W_LT, LT_LEN, dest, max);
#else
		return mbs_to_utf16lit(A_LT, LT_LEN, dest, max);
#endif
	}
	else if (ch == _T('>'))
	{
#ifdef _UNICODE
		return ucs_to_utf16lit(W_GT, GT_LEN, dest, max);
#else
		return mbs_to_utf16lit(A_GT, GT_LEN, dest, max);
#endif
	}
	else if (ch == _T('&'))
	{
#ifdef _UNICODE
		return ucs_to_utf16lit(W_AMP, AMP_LEN, dest, max);
#else
		return mbs_to_utf16lit(A_AMP, AMP_LEN, dest, max);
#endif
	}
	else if (ch == _T('\''))
	{
#ifdef _UNICODE
		return ucs_to_utf16lit(W_APOS, APOS_LEN, dest, max);
#else
		return mbs_to_utf16lit(A_APOS, APOS_LEN, dest, max);
#endif
	}
	else if (ch == _T('\"'))
	{
#ifdef _UNICODE
		return ucs_to_utf16lit(W_QUOT, QUOT_LEN, dest, max);
#else
		return mbs_to_utf16lit(A_QUOT, QUOT_LEN, dest, max);
#endif
	}
	/*else if (ch == _T('/'))
	{
	#ifdef _UNICODE
	return ucs_to_utf16lit(W_QDIV, QDIV_LEN, dest, max);
	#else
	return ucs_to_utf16lit(A_QDIV, QDIV_LEN, dest, max);
	#endif
	}
	else if (ch == _T('\f'))
	{
	#ifdef _UNICODE
	return ucs_to_utf16lit(W_PAGE, PAGE_LEN, dest, max);
	#else
	return mbs_to_utf16lit(A_PAGE, PAGE_LEN, dest, max);
	#endif
	}*/

	return 0;
}

dword_t xml_utf16big_encode(tchar_t ch, byte_t* dest, dword_t max)
{
	if (ch == _T('<'))
	{
#ifdef _UNICODE
		return ucs_to_utf16big(W_LT, LT_LEN, dest, max);
#else
		return mbs_to_utf16big(A_LT, LT_LEN, dest, max);
#endif
	}
	else if (ch == _T('>'))
	{
#ifdef _UNICODE
		return ucs_to_utf16big(W_GT, GT_LEN, dest, max);
#else
		return mbs_to_utf16big(A_GT, GT_LEN, dest, max);
#endif
	}
	else if (ch == _T('&'))
	{
#ifdef _UNICODE
		return ucs_to_utf16big(W_AMP, AMP_LEN, dest, max);
#else
		return mbs_to_utf16big(A_AMP, AMP_LEN, dest, max);
#endif
	}
	else if (ch == _T('\''))
	{
#ifdef _UNICODE
		return ucs_to_utf16big(W_APOS, APOS_LEN, dest, max);
#else
		return mbs_to_utf16big(A_APOS, APOS_LEN, dest, max);
#endif
	}
	else if (ch == _T('\"'))
	{
#ifdef _UNICODE
		return ucs_to_utf16big(W_QUOT, QUOT_LEN, dest, max);
#else
		return mbs_to_utf16big(A_QUOT, QUOT_LEN, dest, max);
#endif
	}
	/*else if (ch == _T('/'))
	{
	#ifdef _UNICODE
	return ucs_to_utf16big(W_QDIV, QDIV_LEN, dest, max);
	#else
	return ucs_to_utf16big(A_QDIV, QDIV_LEN, dest, max);
	#endif
	}
	else if (ch == _T('\f'))
	{
	#ifdef _UNICODE
	return ucs_to_utf16big(W_PAGE, PAGE_LEN, dest, max);
	#else
	return mbs_to_utf16big(A_PAGE, PAGE_LEN, dest, max);
	#endif
	}*/

	return 0;
}

dword_t xml_utf8_encode(tchar_t ch, byte_t* dest, dword_t max)
{
	if (ch == _T('<'))
	{
#ifdef _UNICODE
		return ucs_to_utf8(W_LT, LT_LEN, dest, max);
#else
		return mbs_to_utf8(A_LT, LT_LEN, dest, max);
#endif
	}
	else if (ch == _T('>'))
	{
#ifdef _UNICODE
		return ucs_to_utf8(W_GT, GT_LEN, dest, max);
#else
		return mbs_to_utf8(A_GT, GT_LEN, dest, max);
#endif
	}
	else if (ch == _T('&'))
	{
#ifdef _UNICODE
		return ucs_to_utf8(W_AMP, AMP_LEN, dest, max);
#else
		return mbs_to_utf8(A_AMP, AMP_LEN, dest, max);
#endif
	}
	else if (ch == _T('\''))
	{
#ifdef _UNICODE
		return ucs_to_utf8(W_APOS, APOS_LEN, dest, max);
#else
		return mbs_to_utf8(A_APOS, APOS_LEN, dest, max);
#endif
	}
	else if (ch == _T('\"'))
	{
#ifdef _UNICODE
		return ucs_to_utf8(W_QUOT, QUOT_LEN, dest, max);
#else
		return mbs_to_utf8(A_QUOT, QUOT_LEN, dest, max);
#endif
	}
	/*else if (ch == _T('/'))
	{
	#ifdef _UNICODE
	return ucs_to_utf8(W_QDIV, QDIV_LEN, dest, max);
	#else
	return ucs_to_utf8(A_QDIV, QDIV_LEN, dest, max);
	#endif
	}
	else if (ch == _T('\f'))
	{
	#ifdef _UNICODE
	return ucs_to_utf8(W_PAGE, PAGE_LEN, dest, max);
	#else
	return mbs_to_utf8(A_PAGE, PAGE_LEN, dest, max);
	#endif
	}*/

	return 0;
}

#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
dword_t xml_gb2312_encode(tchar_t ch, byte_t* dest, dword_t max)
{
	if (ch == _T('<'))
	{
#ifdef _UNICODE
		return ucs_to_gb2312(W_LT, LT_LEN, dest, max);
#else
		return mbs_to_gb2312(A_LT, LT_LEN, dest, max);
#endif
	}
	else if (ch == _T('>'))
	{
#ifdef _UNICODE
		return ucs_to_gb2312(W_GT, GT_LEN, dest, max);
#else
		return mbs_to_gb2312(A_GT, GT_LEN, dest, max);
#endif
	}
	else if (ch == _T('&'))
	{
#ifdef _UNICODE
		return ucs_to_gb2312(W_AMP, AMP_LEN, dest, max);
#else
		return mbs_to_gb2312(A_AMP, AMP_LEN, dest, max);
#endif
	}
	else if (ch == _T('\''))
	{
#ifdef _UNICODE
		return ucs_to_gb2312(W_APOS, APOS_LEN, dest, max);
#else
		return mbs_to_gb2312(A_APOS, APOS_LEN, dest, max);
#endif
	}
	else if (ch == _T('\"'))
	{
#ifdef _UNICODE
		return ucs_to_gb2312(W_QUOT, QUOT_LEN, dest, max);
#else
		return mbs_to_gb2312(A_QUOT, QUOT_LEN, dest, max);
#endif
	}
	/*else if (ch == _T('/'))
	{
	#ifdef _UNICODE
	return ucs_to_gb2312(W_QDIV, QDIV_LEN, dest, max);
	#else
	return ucs_to_gb2312(A_QDIV, QDIV_LEN, dest, max);
	#endif
	}
	else if (ch == _T('\f'))
	{
	#ifdef _UNICODE
	return ucs_to_gb2312(W_PAGE, PAGE_LEN, dest, max);
	#else
	return mbs_to_gb2312(A_PAGE, PAGE_LEN, dest, max);
	#endif
	}*/

	return 0;
}
#endif

dword_t xml_unn_encode(tchar_t ch, byte_t* dest, dword_t max)
{
	if (ch == _T('<'))
	{
#ifdef _UNICODE
		return ucs_to_unn(W_LT, LT_LEN, dest, max);
#else
		return mbs_to_unn(A_LT, LT_LEN, dest, max);
#endif
	}
	else if (ch == _T('>'))
	{
#ifdef _UNICODE
		return ucs_to_unn(W_GT, GT_LEN, dest, max);
#else
		return mbs_to_unn(A_GT, GT_LEN, dest, max);
#endif
	}
	else if (ch == _T('&'))
	{
#ifdef _UNICODE
		return ucs_to_unn(W_AMP, AMP_LEN, dest, max);
#else
		return mbs_to_unn(A_AMP, AMP_LEN, dest, max);
#endif
	}
	else if (ch == _T('\''))
	{
#ifdef _UNICODE
		return ucs_to_unn(W_APOS, APOS_LEN, dest, max);
#else
		return mbs_to_unn(A_APOS, APOS_LEN, dest, max);
#endif
	}
	else if (ch == _T('\"'))
	{
#ifdef _UNICODE
		return ucs_to_unn(W_QUOT, QUOT_LEN, dest, max);
#else
		return mbs_to_unn(A_QUOT, QUOT_LEN, dest, max);
#endif
	}
	/*else if (ch == _T('/'))
	{
	#ifdef _UNICODE
	return ucs_to_unn(W_QDIV, QDIV_LEN, dest, max);
	#else
	return ucs_to_unn(A_QDIV, QDIV_LEN, dest, max);
	#endif
	}
	else if (ch == _T('\f'))
	{
	#ifdef _UNICODE
	return ucs_to_unn(W_PAGE, PAGE_LEN, dest, max);
	#else
	return mbs_to_unn(A_PAGE, PAGE_LEN, dest, max);
	#endif
	}*/

	return 0;
}

int xml_mbs_encode(tchar_t ch, schar_t* dest, int max)
{
	if (ch == _T('<'))
	{
		if (dest)
			a_xsncpy(dest, A_LT, max);
		return LT_LEN;
	}
	else if (ch == _T('>'))
	{
		if (dest)
			a_xsncpy(dest, A_GT, max);
		return GT_LEN;
	}
	else if (ch == _T('&'))
	{
		if (dest)
			a_xsncpy(dest, A_AMP, max);
		return AMP_LEN;
	}
	else if (ch == _T('\''))
	{
		if (dest)
			a_xsncpy(dest, A_APOS, max);
		return APOS_LEN;
	}
	else if (ch == _T('\"'))
	{
		if (dest)
			a_xsncpy(dest, A_QUOT, max);
		return QUOT_LEN;
	}
	/*else if (ch == _T('/'))
	{
	if (dest)
	a_xsncpy(dest, A_QDIV, max);
	return QDIV_LEN;
	}
	else if (ch == _T('\f'))
	{
	if (dest)
	a_xsncpy(dest, A_PAGE, max);
	return PAGE_LEN;
	}*/

	return 0;
}

int xml_ucs_encode(tchar_t ch, wchar_t* dest, int max)
{
	if (ch == _T('<'))
	{
		if (dest)
			w_xsncpy(dest, W_LT, max);
		return LT_LEN;
	}
	else if (ch == _T('>'))
	{
		if (dest)
			w_xsncpy(dest, W_GT, max);
		return GT_LEN;
	}
	else if (ch == _T('&'))
	{
		if (dest)
			w_xsncpy(dest, W_AMP, max);
		return AMP_LEN;
	}
	else if (ch == _T('\''))
	{
		if (dest)
			w_xsncpy(dest, W_APOS, max);
		return APOS_LEN;
	}
	else if (ch == _T('\"'))
	{
		if (dest)
			w_xsncpy(dest, W_QUOT, max);
		return QUOT_LEN;
	}
	/*else if (ch == _T('/'))
	{
	if (dest)
	w_xsncpy(dest, W_QDIV, max);
	return QDIV_LEN;
	}
	else if (ch == _T('\f'))
	{
	if (dest)
	w_xsncpy(dest, W_PAGE, max);
	return PAGE_LEN;
	}*/

	return 0;
}