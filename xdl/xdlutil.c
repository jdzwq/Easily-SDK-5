/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc utility document

	@module	xdlutil.c | xdl utility implement file

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

#include "xdlutil.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

static tchar_t week_day[7][4] = { _T("Sun"), _T("Mon"), _T("Tue"), _T("Wed"), _T("Thu"), _T("Fri"), _T("Sat") };

static tchar_t year_mon[12][4] = { _T("Jan"), _T("Feb"), _T("Mar"), _T("Apr"), _T("May"), _T("Jun"), _T("Jul"), _T("Aug"), _T("Sep"), _T("Oct"), _T("Nov"), _T("Dec") };
static int mon_day[13] = { 29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static tchar_t chs_num[10][CHS_LEN + 1] = { _T("零"), _T("壹"), _T("贰"), _T("叁"), _T("肆"), _T("伍"), _T("陆"), _T("柒"), _T("捌"), _T("玖") };
#define CHSUNI_MAX	15
static tchar_t chs_uni[CHSUNI_MAX + 1][CHS_LEN + 1] = { _T("万"), _T("仟"), _T("佰"), _T("拾"), _T("亿"), _T("仟"), _T("佰"), _T("拾"), _T("万"), _T("仟"), _T("佰"), _T("拾"), _T("元"), _T("角"), _T("分"), _T("整") };


#ifdef _OS_WINDOWS
static tchar_t calen_week[CALENDAR_COL][UTF_LEN + 1] = { _T("日"), _T("一"), _T("二"), _T("三"), _T("四"), _T("五"), _T("六") };
#else
static tchar_t calen_week[CALENDAR_COL][UTF_LEN + 1] = { _T("Sun"), _T("Mon"), _T("Tue"), _T("Wed"), _T("Thu"), _T("Fri"), _T("Sat") };
#endif

#define IS_STRING_SPLIT(ch) (ch == _T(' ') || ch == _T('\t') || ch == _T('\n') || ch == _T(',') || ch == _T(';') || ch == _T('~') || ch == _T(':'))

void bytes_turn(byte_t* ba, int n)
{
	byte_t b;
	int i;

	for (i = 0; i < n / 2; i++)
	{
		b = ba[i];
		ba[i] = ba[n - 1 - i];
		ba[n - 1 - i] = b;
	}
}

int parse_charset(const tchar_t* enstr)
{
	if (xsnicmp(enstr, CHARSET_GB2312, xslen(CHARSET_GB2312)) == 0)
		return _GB2312;
	else if (xsnicmp(enstr, CHARSET_UTF8, xslen(CHARSET_UTF8)) == 0)
		return _UTF8;
	else if (xsnicmp(enstr, CHARSET_UTF16, xslen(CHARSET_UTF16)) == 0)
		return (DEF_BOM == BIGBOM) ? _UTF16_BIG : _UTF16_LIT;
	else
		return _UNKNOWN;
}

void format_charset(int encode, tchar_t* buf)
{
	switch (encode)
	{
	case _GB2312:
		xscpy(buf, CHARSET_GB2312);
		break;
	case _UTF8:
		xscpy(buf, CHARSET_UTF8);
		break;
	case _UTF16_BIG:
		xscpy(buf, CHARSET_UTF16);
		break;
	case _UTF16_LIT:
		xscpy(buf, CHARSET_UTF16);
		break;
	default:
		buf[0] = _T('\0');
	}
}


int compare_data(const tchar_t* szSrc, const tchar_t* szDes, const tchar_t* datatype)
{
	long nSrc, nDes;
	double dbSrc, dbDes;
	short shSrc, shDes;
	int rt;
	xdate_t md1 = { 0 };
	xdate_t md2 = { 0 };

	if (is_null(szSrc) && is_null(szDes))
		return 0;
	else if (is_null(szSrc))
		return -1;
	else if (is_null(szDes))
		return 1;

	if (xscmp(datatype, ATTR_DATA_TYPE_BOOLEAN) == 0)
	{
		shSrc = xstos(szSrc);
		shDes = xstos(szDes);
		if (shSrc > shDes)
			rt = 1;
		else if (shSrc < shDes)
			rt = -1;
		else
			rt = 0;
		return rt;
	}
	else if (xscmp(datatype, ATTR_DATA_TYPE_INTEGER) == 0)
	{
		nSrc = xstol(szSrc);
		nDes = xstol(szDes);
		if (nSrc > nDes)
			rt = 1;
		else if (nSrc < nDes)
			rt = -1;
		else
			rt = 0;
		return rt;
	}
	else if (xscmp(datatype, ATTR_DATA_TYPE_NUMERIC) == 0)
	{
		dbSrc = xstonum(szSrc);
		dbDes = xstonum(szDes);
		if (dbSrc > dbDes)
			rt = 1;
		else if (dbSrc < dbDes)
			rt = -1;
		else
			rt = 0;
		return rt;
	}
	else if (xscmp(datatype, ATTR_DATA_TYPE_DATE) == 0)
	{
		parse_date(&md1, szSrc);
		parse_date(&md2, szDes);
		return compare_date(&md1, &md2);
	}
	else if (xscmp(datatype, ATTR_DATA_TYPE_DATETIME) == 0)
	{
		parse_datetime(&md1, szSrc);
		parse_datetime(&md2, szDes);
		return compare_datetime(&md1, &md2);
	}
	else
	{
		return xscmp(szSrc, szDes);
	}
}

int verify_text(const tchar_t* str, const tchar_t* datatype, bool_t nullable, int len, const tchar_t* min, const tchar_t* max)
{

	if (is_null(str))
		return (nullable) ? veValid : veNull;

	if (xscmp(datatype, ATTR_DATA_TYPE_BOOLEAN) == 0)
	{
		if (xslen(str) > 1)
			return veTruncate;

		if (*str != _T('1') && *str != _T('0'))
			return veDatatype;
	}
	else if (xscmp(datatype, ATTR_DATA_TYPE_STRING) == 0)
	{
		if (len && xslen(str) > len)
			return veTruncate;
	}
	else if (xscmp(datatype, ATTR_DATA_TYPE_INTEGER) == 0 || xscmp(datatype, ATTR_DATA_TYPE_NUMERIC) == 0)
	{
		if (!is_numeric(str))
			return veDatatype;
	}
	else if (xscmp(datatype, ATTR_DATA_TYPE_DATETIME) == 0 || xscmp(datatype, ATTR_DATA_TYPE_DATE) == 0)
	{
		if (!is_datetime(str))
			return veDatatype;
	}

	//verify min and max value
	if (!is_null(min))
	{
		if (compare_data(min, str, datatype) > 0)
			return veOverflow;
	}

	if (!is_null(max))
	{
		if (compare_data(max, str, datatype) < 0)
			return veOverflow;
	}

	return veValid;
}

int parse_encode(const tchar_t* enstr)
{
	if (xsnicmp(enstr, _T("gb2312"), xslen(_T("gb2312"))) == 0)
		return _GB2312;
	else if (xsnicmp(enstr, _T("utf-8"), xslen(_T("utf-8"))) == 0)
		return _UTF8;
	else if (xsnicmp(enstr, _T("utf-16-lit"), xslen(_T("utf-16-lit"))) == 0)
		return _UTF16_LIT;
	else if (xsnicmp(enstr, _T("utf-16-big"), xslen(_T("utf-16-big"))) == 0)
		return _UTF16_BIG;
	else
		return _UNKNOWN;
}

void format_encode(int encode, tchar_t* buf)
{
	switch (encode)
	{
	case _GB2312:
		xscpy(buf, _T("gb2312"));
		break;
	case _UTF8:
		xscpy(buf, _T("utf-8"));
		break;
	case _UTF16_BIG:
		xscpy(buf, _T("utf-16-big"));
		break;
	case _UTF16_LIT:
		xscpy(buf, _T("utf-16-lit"));
		break;
	default:
		buf[0] = _T('\0');
	}
}

int parse_utfbom(const byte_t* buf, int len)
{
	if (len > 1 && buf[0] == 0xFF && buf[1] == 0xFE)
		return _UTF16_LIT;
	
	if (len > 1 && buf[0] == 0xFE && buf[1] == 0xFF)
		return _UTF16_BIG;
	
	if (len > 2 && buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF)
		return _UTF8;
	
	return _UNKNOWN;
}

int format_utfbom(int encode, byte_t* buf)
{
	if (encode == _UTF16_LIT)
	{
		if (buf)
		{
			buf[0] = 0xFF;
			buf[1] = 0xFE;
		}
		return 2;
	}
	else if (encode == _UTF16_BIG)
	{
		if (buf)
		{
			buf[0] = 0xFE;
			buf[1] = 0xFF;
		}
		return 2;
	}
	else if (encode == _UTF8)
	{
		if (buf)
		{
			buf[0] = 0xEF;
			buf[1] = 0xBB;
			buf[2] = 0xBF;
		}
		return 3;
	}

	return 0;
}

int skip_utfbom(const byte_t* buf)
{
	if (buf[0] == 0xFF && buf[1] == 0xFE)
		return 2;

	if (buf[0] == 0xFE && buf[1] == 0xFF)
		return 2;

	if (buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF)
		return 3;

	return 0;
}

bool_t is_ip(const tchar_t* addr)
{
	if (addr[0] >= _T('0') && addr[0] <= _T('9'))
	{
		if ((addr[1] >= _T('0') && addr[1] <= _T('9')) || addr[1] == _T('.'))
		{
			if ((addr[2] >= _T('0') && addr[2] <= _T('9')) || addr[2] == _T('.'))
				return 1;
		}
	}

	return 0;
}

void parse_long_range(tchar_t* sz_range, dword_t* phoff, dword_t* ploff, dword_t* psize, long long* ptotal)
{
	int len,step = 0;
	const tchar_t* token = sz_range;
	long long ll_from = 0;
	long long ll_to = 0;
	long long ll_total = 0;

	*phoff = *ploff = *psize = 0;

	while (*token != _T('\0'))
	{
		while ((*token < _T('0') || *token > _T('9')) && *token != _T('\0'))
			token++;

		len = 0;
		while (*token >= _T('0') && *token <= _T('9'))
		{
			token++;
			len++;
		}

		if (step == 0)
		{
			ll_from = xsntoll(token - len, len);
			step++;
		}
		else if (step == 1)
		{
			ll_to = xsntoll(token - len, len);
			step++;
		}
		else
		{
			ll_total = xsntoll(token - len, len);
			step++;
		}

		if (*token != _T('\0'))
			token++;
	}

	*phoff = GETHDWORD(ll_from);
	*ploff = GETLDWORD(ll_from);
	*psize = (dword_t)(ll_to - ll_from + 1);
	*ptotal = ll_total;
}

void format_long_range(tchar_t* sz_range, dword_t hoff, dword_t loff, dword_t size, long long total)
{
	tchar_t sz_from[NUM_LEN + 1] = { 0 };
	tchar_t sz_to[NUM_LEN + 1] = { 0 };
	tchar_t sz_total[NUM_LEN + 1] = { 0 };

	long long ll = 0;

	ll = MAKELONGLONG(loff, hoff);
	lltoxs(ll, sz_from, NUM_LEN);

	ll += (size - 1);
	lltoxs(ll, sz_to, NUM_LEN);

	lltoxs(total, sz_total, NUM_LEN);

	xsprintf(sz_range, _T("%s-%s/%s"), sz_from, sz_to, sz_total);
}

int format_longlong(unsigned long hl, unsigned long ll, tchar_t* buf)
{
	long long li;
	int len = 0;
	
	li = (((long long)hl) << 32) | (long long)ll;
	do
	{
		if (buf)
		{
			buf[len] = (int)(li % 10) + _T('0');
		}
		li /= 10;
		len++;
	} while (li);

	if (buf)
	{
		buf[len] = _T('\0');

		xsnrev(buf, len);
	}

	return len;
}

void parse_longlong(unsigned long* phl, unsigned long* pll, const tchar_t* str)
{
	long long li = 0;
	int len = 0;

	if (phl)
		*phl = 0;
	if (pll)
		*pll = 0;

	if (is_null(str))
		return;

	while (str[len] != _T('\0'))
	{
		li *= 10;
		li += str[len++] - _T('0');
	}

	if (phl)
		*phl = (unsigned long)((li & 0xFFFFFFFF00000000) >> 32);
	if (pll)
		*pll = (unsigned long)(li & 0x00000000FFFFFFFF);
}

bool_t is_zero_size(const tchar_t* fsize)
{
	dword_t hdw, ldw;

	parse_longlong(&hdw, &ldw, fsize);

	return (!hdw && !ldw)? 1 : 0;
}

bool_t is_huge_size(const tchar_t* fsize)
{
	dword_t hdw, ldw;

	parse_longlong(&hdw, &ldw, fsize);

	if (hdw)
		return 1;
	else if (ldw > MAX_LONG)
		return 1;

	return 0;
}

unsigned long parse_hexnum(const tchar_t* token, int len)
{
	unsigned long k = 0;
	long c = 0;
	int pos = 0;

	if (len < 0)
		len = xslen(token);

	if (is_null(token) || !len)
		return 0;

	if (token[0] == _T('0') && (token[1] == _T('x') || token[1] == _T('X')))
		pos += 2;

	while (pos < len)
	{
		k *= 16;

		if (token[pos] >= _T('a') && token[pos] <= _T('z'))
			c = (token[pos] - _T('a')) + 10;
		else if (token[pos] >= _T('A') && token[pos] <= _T('Z'))
			c = (token[pos] - _T('A')) + 10;
		else if (token[pos] >= _T('0') && token[pos] <= _T('9'))
			c = (token[pos] - _T('0'));
		else if (token[pos] == _T('\0'))
			break;
		else
			return 0;

		k += c;

		pos++;
	}

	return k;
}

int format_hexnum(unsigned long n, tchar_t* buf, int max)
{
	long c = 0;
	int pos = 0;

	do
	{
		c = n % 16;

		if (buf)
		{
			if (c >= 10)
				buf[pos] = _T('A') + (byte_t)(c - 10);
			else
				buf[pos] = _T('0') + (byte_t)c;
		}

		n /= 16;
		pos++;

		if (pos % 2)
			continue;
	} while (n && pos < max);

	if (buf)
	{
		buf[pos] = _T('\0');
		xsnrev(buf, pos);
	}

	return pos;
}

int fill_long(long ln, tchar_t* buf, int max)
{
	int len;
	bool_t b = 0;

	if (ln < 0)
	{
		b = 1;
		ln = 0 - ln;
		max--;
		buf[0] = _T('-');
		buf++;
	}

	len = ltoxs(ln, NULL, max);

	if (len < max)
	{
		len = max - len;
		ltoxs(ln, buf + len, max - len);
		while (len--)
		{
			(*buf) = _T('0');
			buf++;
		}

		if (b)
			max++;

		return max;
	}
	else
	{
		return ltoxs(ln, buf, max);
	}
}

int format_long_ex(long ln, const tchar_t* fmt, tchar_t* buf, int max)
{
	int n_split, b_negat;
	int n, len, total = 0;
	tchar_t sz_num[INT_LEN + 1] = { 0 };
	tchar_t* token;

	if (is_null(fmt))
		return ltoxs(ln, buf, max);

	n_split = 0;

	if (ln < 0)
		b_negat = 1;
	else
		b_negat = 0;

	if (b_negat && *fmt == _T('('))
	{
		b_negat++;
	}

	while (*fmt == _T('#'))
	{
		fmt++;
	}

	if (*fmt == _T(','))
	{
		fmt++;
		n_split = 0;
		while (*fmt == _T('#'))
		{
			n_split++;
			fmt++;
		}
	}

	while (*fmt >= _T('0') && *fmt <= _T('9'))
		fmt++;

	if (b_negat)
		ln = 0 - ln;

	ltoxs(ln, sz_num, INT_LEN);

	len = xslen(sz_num);

	if (len && n_split)
	{
		if (len % n_split)
			n = (len / n_split);
		else
			n = (len / n_split) - 1;
	}
	else
	{
		n = 0;
	}

	if (len + n + b_negat + xslen(fmt) > max)
		return 0;

	total = 0;

	if (b_negat == 1)
	{
		if (buf)
		{
			buf[total] = _T('-');
		}
		total++;
	}
	else if (b_negat == 2)
	{
		if (buf)
		{
			buf[total] = _T('(');
		}
		total++;
	}

	token = sz_num;
	while (*token != _T('\0'))
	{
		if (token > sz_num && n && n_split && !(len % n_split))
		{
			if (buf)
			{
				buf[total] = _T(',');
			}
			total++;
			n--;
		}

		if (buf)
		{
			buf[total] = *token;
		}
		total++;
		len--;

		token++;
	}

	if (b_negat == 2)
	{
		if (buf)
		{
			buf[total] = _T(')');
		}
		total++;
	}

	len = xslen(fmt);
	if (buf)
	{
		xsncpy(buf + total, fmt, len);
	}
	total += len;

	return total;
}

bool_t is_zero_numeric(double dbl, int scale)
{
	if (!scale || scale > MAX_DOUBLE_DIGI)
		scale = MAX_DOUBLE_DIGI;
	else if (scale < 0)
		scale = 0;

	while (scale--)
	{
		dbl *= 10;
	}

	return ((long)dbl) ? 0 : 1;
}

double parse_numeric(const tchar_t* token, int len)
{
	double f = 0;
	int b_negat = 0;
	int sz_len = 0;
	tchar_t sz_num[NUM_LEN + 1] = { 0 };

	if (len < 0)
		len = xslen(token);

	if (is_null(token) || !len)
		return 0;

	while (*token != _T('\0') && len && sz_len < NUM_LEN)
	{
		if (*token == _T('('))
		{
			b_negat = 1;
		}
		else if (*token == _T('+') || *token == _T('-') || (*token >= _T('0') && *token <= _T('9')) || *token == _T('.'))
		{
			sz_num[sz_len] = *token;
			sz_len++;
		}else if (*token != _T(','))
		{
			break;
		}

		token++;
		len--;
	}

	f = xsntonum(sz_num, sz_len);

	if (b_negat)
	{
		f = 0 - f;
	}

	if (*token == _T('%'))
		return f / 100;
	else if (xsncmp(token, _T("万"), xslen(_T("万"))) == 0)
		return f * 10000;
	else
		return f;
}

int format_numeric(double dbl, const tchar_t* fmt, tchar_t* buf, int max)
{
	int n_split, n_prec, b_percent, b_negat;
	int n, len, total = 0;
	tchar_t sz_num[NUM_LEN + 1] = { 0 };
	tchar_t* token;

	if (is_null(fmt))
		return numtoxs(dbl, buf, max);
	
	n_split = n_prec = b_percent = 0;

	if (dbl < 0)
		b_negat = 1;
	else
		b_negat = 0;

	if (*fmt == _T('('))
	{
		fmt++;

		if(b_negat)
			b_negat++;
	}

	while (*fmt == _T('#'))
		fmt++;

	if (*fmt == _T(','))
	{
		fmt++;
		n_split = 0;
		while (*fmt == _T('#'))
		{
			n_split++;
			fmt++;
		}
	}

	if (*fmt == _T('.'))
	{
		fmt++;
		n_prec = 0;
		while (*fmt == _T('#'))
		{
			n_prec++;
			fmt++;
		}
	}

	if (*fmt == _T('%'))
	{
		b_percent = 1;
		fmt++;
	}
	else if (xsncmp(fmt, _T("万"), xslen(_T("万"))) == 0)
	{
		b_percent = 2;
		fmt += xslen(_T("万"));
	}

	if (*fmt == _T(')'))
		fmt++;

	if (b_negat)
		dbl = 0 - dbl;

	if (b_percent == 1)
		dbl *= 100;
	else if (b_percent == 2)
		dbl /= 10000;

	numtoxs_dig(dbl, n_prec, sz_num, NUM_LEN);

	len = 0;
	token = sz_num;
	while (*token != _T('.') && *token != _T('\0'))
	{
		token++;
		len++;
	}

	if (n_prec)
	{
		if (*token == _T('.'))
		{
			token++;
		}
		else
		{
			*token = _T('.');
			*(token + 1) = _T('\0');
			token++;
		}

		while (n_prec--)
		{
			if (*token == _T('\0'))
			{
				*token = _T('0');
				*(token + 1) = _T('\0');
			}

			token++;
		}
	}
	else
	{
		*token = _T('\0');
	}

	n_prec = (int)(token - sz_num) - len;

	if (len && n_split)
	{
		if (len % n_split)
			n = (len / n_split);
		else
			n = (len / n_split) - 1;
	}
	else
	{
		n = 0;
	}

	if (len + n + n_prec + b_negat + b_percent + xslen(fmt) > max)
		return 0;
	
	total = 0;

	if (b_negat == 1)
	{
		if (buf)
		{
			buf[total] = _T('-');
		}
		total++;
	}
	else if (b_negat == 2)
	{
		if (buf)
		{
			buf[total] = _T('(');
		}
		total++;
	}

	token = sz_num;
	while (*token != _T('\0'))
	{
		if (token > sz_num && n && n_split && !(len % n_split))
		{
			if (buf)
			{
				buf[total] = _T(',');
			}
			total++;
			n--;
		}

		if (buf)
		{
			buf[total] = *token;
		}
		total++;
		len--;

		token++;
	}

	if (b_percent == 1)
	{
		if (buf)
		{
			buf[total] = _T('%');
		}
		total++;
	}
	else if (b_percent == 2)
	{
		if (buf)
		{
			xscpy(buf + total, _T("万"));
		}
		total += xslen(_T("万"));
	}

	if (b_negat == 2)
	{
		if (buf)
		{
			buf[total] = _T(')');
		}
		total++;
	}

	len = xslen(fmt);
	if (buf)
	{
		xsncpy(buf + total, fmt, len);
	}
	total += len;

	if (buf)
	{
		buf[total] = _T('\0');
	}

	return total;
}

int format_money_chs(double dbl, int n_span, tchar_t* buf, int max)
{
	tchar_t sz_num[NUM_LEN + 1] = { 0 };
	tchar_t* token;
	int step,n,count,i,len,pt,total = 0;
	bool_t b_zero;

	if (dbl <= 0)
		return 0;

	numtoxs_dig(dbl, 2, sz_num, NUM_LEN);

	len = 0;
	token = sz_num;
	while (*token != _T('\0') && *token != _T('.'))
	{
		token++;
		len++;
	}

	len = xslen(sz_num) - len;
	if (!len)
	{
		*token = _T('0');
		token++;
		*token = _T('0');
		token++;
		*token = _T('\0');
	}
	else if (len == 1)
	{
		*token = _T('0');
		token++;
		*token = _T('0');
		token++;
		*token = _T('\0');
	}
	else if (len == 2)
	{
		*token = *(token + 1);
		token++;
		*token = _T('0');
		token++;
		*token = _T('\0');
	}
	else if (len == 3)
	{
		*token = *(token + 1);
		token++;
		*token = *(token + 1);
		token++;
		*token = _T('\0');
	}
	
	len = (int)(token - sz_num);
	token = sz_num;

	while (len > CHSUNI_MAX)
	{
		token++;
		len--;
	}

	i = CHSUNI_MAX - len;
	step = 5;
	count = 0;
	while (i < CHSUNI_MAX)
	{
		b_zero = (count)? 0 : 1;
		count = 0;
		for (; i < step; i++)
		{
			n = (int)(*token - _T('0'));
			if (!n_span && b_zero && !n)
			{
				token++;
				continue;
			}

			b_zero = (n)? 0 : 1;
			if (n)
			{
				count++;
			}

			pt = xslen(chs_num[n]);
			if (total + pt > max)
				return total;

			if (buf)
			{
				xsncpy(buf + total, chs_num[n], pt);
			}
			total += pt;

			if (n_span)
			{
				if (total + n_span > max)
					return total;

				if (buf)
				{
					count = n_span;
					while (count--)
					{
						buf[total + count] = _T(' ');
					}
				}
				total += n_span;

				token++;
				continue;
			}

			if (n)
			{
				pt = xslen(chs_uni[i]);
				if (total + pt > max)
					return total;

				if (buf)
				{
					xsncpy(buf + total, chs_uni[i], pt);
				}
				total += pt;
			}

			token++;
		}

		if (!n_span && i < 13 && count && b_zero)
		{
			pt = xslen(chs_uni[i - 1]);

			if (buf)
			{
				xsncpy(buf + total - pt, chs_uni[i - 1], pt);
			}
		}
		
		if (!n_span && i == 13 && total)
		{
			pt = xslen(chs_uni[i - 1]);

			if (buf)
			{
				xsncpy(buf + total - pt, chs_uni[i - 1], pt);
			}

			count = 1;
		}

		if (!n_span && i == 15 && b_zero && total)
		{
			pt = xslen(chs_uni[i]);

			if (buf)
			{
				xsncpy(buf + total - pt, chs_uni[i], pt);
			}
		}

		switch (i)
		{
		case 5:
			step = 9;
			break;
		case 9:
			step = 13;
			break;
		case 13:
			step = 15;
			break;
		}

	}

	if (buf)
	{
		buf[total] = _T('\0');
	}

	return total;
}

int format_date(const xdate_t* pmd,tchar_t* buf)
{
	return xsprintf(buf,_T("%d-%02d-%02d"),pmd->year,pmd->mon,pmd->day);
}

int format_datetime(const xdate_t* pmd,tchar_t* buf)
{
	return xsprintf(buf,_T("%d-%02d-%02d %02d:%02d:%02d"),pmd->year,pmd->mon,pmd->day,pmd->hour,pmd->min,pmd->sec);
}

int format_time(const xdate_t* pmd, tchar_t* buf)
{
	return xsprintf(buf, _T("%02d:%02d:%02d"), pmd->hour, pmd->min, pmd->sec);
}

int format_ages(const xdate_t* bday, const xdate_t* tday, tchar_t* buf)
{
	if (bday->year < tday->year)
	{
		return xsprintf(buf, _T("%d%s"), tday->year - bday->year + 1, AGES_YEAR);
	}
	else if (bday->year == tday->year)
	{
		if (bday->mon < tday->mon)
		{
			return xsprintf(buf, _T("%d%s"), tday->mon - bday->mon + 1, AGES_MONTH);
		}
		else if (bday->mon == tday->mon)
		{
			if (bday->day <= tday->day)
			{
				return xsprintf(buf, _T("%d%s"), tday->day - bday->day + 1, AGES_DAY);
			}
		}
	}

	return 0;
}

void reset_date(xdate_t* pmd)
{
	xmem_zero((void*)pmd, sizeof(xdate_t));

	pmd->year = 1970;
	pmd->mon = 1;
	pmd->day = 1;
}

void parse_date(xdate_t* pmd, const tchar_t* text)
{
	tchar_t* tmp;
	int i;
	tchar_t token[5];

	pmd->year = 2000;
	pmd->mon = 1;
	pmd->day = 1;

	if (is_null(text))
		return;

	tmp = (tchar_t*)text;

	i = 0;
	token[i] = _T('\0');
	while (*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 4)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp++;
	}

	pmd->year = xstos(token);
	if (*tmp == _T('\0') || i == 0)
		return;

	while ((*tmp < _T('0') || *tmp > _T('9')) && *tmp != _T('\0'))
		tmp++;

	i = 0;
	token[i] = _T('\0');
	while (*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 2)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp++;
	}

	pmd->mon = xstos(token);
	if (*tmp == _T('\0') || i == 0)
		return;

	while ((*tmp < _T('0') || *tmp > _T('9')) && *tmp != _T('\0'))
		tmp++;

	i = 0;
	token[i] = _T('\0');
	while (*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 2)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp++;
	}

	pmd->day = xstos(token);
}

void parse_datetime(xdate_t* pmd, const tchar_t* text)
{
	tchar_t* tmp;
	int i;
	tchar_t token[5];

	pmd->year = 2000;
	pmd->mon = 1;
	pmd->day = 1;
	pmd->hour = pmd->min = pmd->sec = 0;

	if(is_null(text))
		return ;

	tmp = (tchar_t*)text;

	i = 0;
	token[i] = _T('\0');
	while(*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 4)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp ++;
	}

	pmd->year = xstos(token);
	if(*tmp == _T('\0') || i == 0)
		return ;

	while((*tmp < _T('0') || *tmp > _T('9')) && *tmp != _T('\0'))
		tmp ++;

	i = 0;
	token[i] = _T('\0');
	while(*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 2)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp ++;
	}

	pmd->mon = xstos(token);
	if(*tmp == _T('\0') || i == 0)
		return ;

	while((*tmp < _T('0') || *tmp > _T('9')) && *tmp != _T('\0'))
		tmp ++;

	i = 0;
	token[i] = _T('\0');
	while(*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 2)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp ++;
	}

	pmd->day = xstos(token);
	if (*tmp == _T('\0') || i == 0)
		return ;

	while((*tmp < _T('0') || *tmp > _T('9')) && *tmp != _T('\0'))
		tmp ++;

	i = 0;
	token[i] = _T('\0');
	while(*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 2)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp ++;
	}

	pmd->hour = xstos(token);
	if(*tmp == _T('\0') || i == 0)
		return ;

	while((*tmp < _T('0') || *tmp > _T('9')) && *tmp != _T('\0'))
		tmp ++;

	i = 0;
	token[i] = _T('\0');
	while(*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 2)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp ++;
	}

	pmd->min = xstos(token);
	if(*tmp == _T('\0') || i == 0)
		return ;

	while((*tmp < _T('0') || *tmp > _T('9')) && *tmp != _T('\0'))
		tmp ++;

	i = 0;
	token[i] = _T('\0');
	while(*tmp != _T('\0') && *tmp >= _T('0') && *tmp <= _T('9') && i < 2)
	{
		token[i++] = *tmp;
		token[i] = _T('\0');
		tmp ++;
	}

	pmd->sec = xstos(token);
}

int format_gmttime(const xdate_t* pdt, tchar_t* buf)
{
	return xsprintf(buf, _T("%s, %02d %s %04d %02d:%02d:%02d GMT"), week_day[pdt->wday], pdt->day, year_mon[pdt->mon-1], pdt->year, pdt->hour, pdt->min, pdt->sec);
}

void parse_gmttime(xdate_t* pdt, const tchar_t* str)
{
	tchar_t* token = (tchar_t*)str;
	tchar_t* key;
	int i, klen;

	pdt->year = 2000;
	pdt->mon = 1;
	pdt->day = 1;
	pdt->hour = 0;
	pdt->min = 0;
	pdt->sec = 0;
	pdt->wday = 0;

	if (is_null(str))
		return;

	key = token;
	klen = 0;
	while (*token != _T('\0') && *token != _T(',') && *token != _T(' '))
	{
		token++;
		klen++;
	}

	for (i = 0; i < 7; i++)
	{
		if (compare_text(week_day[i], -1, key, klen, 1) == 0)
		{
			pdt->wday = i;
			break;
		}
	}

	while (*token != _T('\0') &&( *token == _T(',') || *token == _T(' ')))
		token++;

	key = token;
	klen = 0;
	while (*token != _T('\0') && *token != _T(',') && *token != _T(' '))
	{
		token++;
		klen++;
	}

	pdt->day = xstos(key);

	while (*token != _T('\0') && (*token == _T(',') || *token == _T(' ')))
		token++;

	key = token;
	klen = 0;
	while (*token != _T('\0') && *token != _T(',') && *token != _T(' '))
	{
		token++;
		klen++;
	}

	for (i = 0; i < 12; i++)
	{
		if (compare_text(year_mon[i], -1, key, klen, 1) == 0)
		{
			pdt->mon = i + 1;
			break;
		}
	}

	while (*token != _T('\0') && (*token == _T(',') || *token == _T(' ')))
		token++;

	key = token;
	klen = 0;
	while (*token != _T('\0') && *token != _T(',') && *token != _T(' '))
	{
		token++;
		klen++;
	}

	pdt->year = xstos(key);

	while (*token != _T('\0') && (*token == _T(',') || *token == _T(' ')))
		token++;

	key = token;
	klen = 0;
	while (*token != _T('\0') && *token != _T(':'))
	{
		token++;
		klen++;
	}

	pdt->hour = xstos(key);

	if (*token == _T(':'))
		token++;

	key = token;
	klen = 0;
	while (*token != _T('\0') && *token != _T(':'))
	{
		token++;
		klen++;
	}

	pdt->min = xstos(key);

	if (*token == _T(':'))
		token++;

	key = token;
	klen = 0;
	while (*token != _T('\0') && *token != _T(':'))
	{
		token++;
		klen++;
	}

	pdt->sec = xstos(key);
}

int format_datetime_ex(const xdate_t* pxd, const tchar_t* fmt, tchar_t* buf, int max)
{
	int y_count, m_count, d_count, h_count, i_count, s_count;
	const tchar_t* tk_at;
	int tk_len, total = 0;

	if (is_null(fmt))
		return format_datetime(pxd, buf);

	while (*fmt != _T('\0'))
	{
		tk_at = fmt;
		tk_len = 0;
		while (*fmt != _T('\0') && *fmt != _T('y') && *fmt != _T('Y') && *fmt != _T('m') && *fmt != _T('M') && *fmt != _T('d') && *fmt != _T('D') && *fmt != _T('h') && *fmt != _T('H') && *fmt != _T('i') && *fmt != _T('I') && *fmt != _T('s') && *fmt != _T('S'))
		{
			tk_len++;
			fmt++;
		}

		if (total + tk_len > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, tk_at, tk_len);
		}
		total += tk_len;

		y_count = 0;
		while (*fmt == _T('y') || *fmt == _T('Y'))
		{
			y_count++;
			fmt++;
		}

		if (total + y_count > max)
			return total;

		if (buf && y_count)
		{
			y_count = xsprintf(buf + total, _T("%04d"), pxd->year);
		}
		total += y_count;

		m_count = 0;
		while (*fmt == _T('m') || *fmt == _T('M'))
		{
			m_count++;
			fmt++;
		}

		if (total + m_count > max)
			return total;

		if (buf && m_count)
		{
			m_count = xsprintf(buf + total, _T("%02d"), pxd->mon);
		}
		total += m_count;

		d_count = 0;
		while (*fmt == _T('d') || *fmt == _T('D'))
		{
			d_count++;
			fmt++;
		}

		if (total + d_count > max)
			return total;

		if (buf && d_count)
		{
			d_count = xsprintf(buf + total, _T("%02d"), pxd->day);
		}
		total += d_count;

		h_count = 0;
		while (*fmt == _T('h') || *fmt == _T('H'))
		{
			h_count++;
			fmt++;
		}

		if (total + h_count > max)
			return total;

		if (buf && h_count)
		{
			h_count = xsprintf(buf + total, _T("%02d"), pxd->hour);
		}
		total += h_count;

		i_count = 0;
		while (*fmt == _T('i') || *fmt == _T('I'))
		{
			i_count++;
			fmt++;
		}

		if (total + i_count > max)
			return total;

		if (buf && i_count)
		{
			i_count = xsprintf(buf + total, _T("%02d"), pxd->min);
		}
		total += i_count;

		s_count = 0;
		while (*fmt == _T('s') || *fmt == _T('S'))
		{
			s_count++;
			fmt++;
		}

		if (total + s_count > max)
			return total;

		if (buf && s_count)
		{
			s_count = xsprintf(buf + total, _T("%02d"), pxd->sec);
		}
		total += s_count;
	}

	return total;
}

void parse_datetime_ex(xdate_t* pxd, const tchar_t* fmt, const tchar_t* str)
{
	const tchar_t* tkat;
	int tklen;
	tchar_t ch;

	if (is_null(fmt) || is_null(str))
	{
		parse_datetime(pxd, str);
		return;
	}

	while (*fmt != _T('\0'))
	{
		while (*fmt != _T('\0') && *fmt != _T('y') && *fmt != _T('Y') && *fmt != _T('m') && *fmt != _T('M') && *fmt != _T('d') && *fmt != _T('D') && *fmt != _T('h') && *fmt != _T('H') && *fmt != _T('i') && *fmt != _T('I') && *fmt != _T('s') && *fmt != _T('S'))
		{
			fmt++;
		}

		while (*str != _T('\0') && (*str < _T('0') || *str > _T('9')))
		{
			str++;
		}

		tkat = str;
		tklen = 0;
		while (*str >= _T('0') && *str <= _T('9'))
		{
			str++;
			tklen++;
		}

		if (*fmt == _T('y') || *fmt == _T('Y'))
		{
			pxd->year = xsntos(tkat, tklen);
			if (tklen < 4)
				pxd->year += 2000;
		}
		else if (*fmt == _T('m') || *fmt == _T('M'))
		{
			pxd->mon = xsntos(tkat, tklen);
		}
		else if (*fmt == _T('d') || *fmt == _T('D'))
		{
			pxd->day = xsntos(tkat, tklen);
		}
		else if (*fmt == _T('h') || *fmt == _T('H'))
		{
			pxd->hour = xsntos(tkat, tklen);
		}
		else if (*fmt == _T('i') || *fmt == _T('I'))
		{
			pxd->min = xsntos(tkat, tklen);
		}
		else if (*fmt == _T('s') || *fmt == _T('S'))
		{
			pxd->sec = xsntos(tkat, tklen);
		}

		ch = *fmt;
		fmt++;
		while (ch == *fmt)
			fmt++;

		if (*str == _T('\0'))
			break;
	}
}

int format_password(const tchar_t* sz, tchar_t* buf, int max)
{
	int len;

	len = xslen(sz);
	len = (len < max) ? len : max;
	max = len;

	while (len--)
	{
		if (buf)
		{
			buf[len] = _T('*');
		}
	}
	
	if (buf)
		buf[max] = _T('\0');

	return max;
}

const tchar_t* parse_attrset_token(const tchar_t* attrset, int len, tchar_t** keyptr, int* keylen, tchar_t** valptr, int* vallen)
{
	const tchar_t* token;
	int total = 0;

	*keyptr = *valptr = NULL;
	*keylen = *vallen = 0;

	if (is_null(attrset) || !len)
		return NULL;

	if (len < 0)
		len = xslen(attrset);

	token = attrset;

	*keyptr = (tchar_t*)token;
	while (*token != _T(' ') && *token != _T('\t') && *token != _T('=') && total < len)
	{
		token++;
		total++;
	}
	*keylen = (int)(token - *keyptr);

	if (total == len)
		return token;

	//skip ' ','='
	while (*token != _T('\'') && *token != _T('\"') && total < len)
	{
		token++;
		total++;
	}

	if (total == len)
		return token;

	//skip '\'','\"'
	token++;
	total++;

	*valptr = (tchar_t*)token;
	while (*token != _T('\'') && *token != _T('\"') && total < len)
	{
		token++;
		total++;
	}
	*vallen = (int)(token - *valptr);

	//skip ' ','\'','\"'
	while (*token == _T('\'') || *token == _T('\"') || *token == _T(' ') || *token == _T('\t'))
	{
		token++;
		total++;
	}

	return token;
}

int parse_attrset_token_count(const tchar_t* attrset, int len)
{
	int count = 0;
	tchar_t *key, *val;
	const tchar_t* term;
	int klen, vlen;

	if (is_null(attrset) || !len)
		return 0;

	if (len < 0)
		len = xslen(attrset);

	term = attrset + len;
	while (attrset = parse_attrset_token(attrset, (int)(term - attrset), &key, &klen, &val, &vlen))
	{
		count++;
	}

	return count;
}

const tchar_t* parse_options_token(const tchar_t* options, int len, tchar_t itemfeed, tchar_t linefeed, tchar_t** keyptr, int* keylen, tchar_t** valptr, int* vallen)
{
	const tchar_t* token;
	int total = 0;

	*keyptr = *valptr = NULL;
	*keylen = *vallen = 0;

	if (is_null(options) || !len)
		return NULL;

	if (len < 0)
		len = xslen(options);

	token = options;
	/*skip blank*/
	while ((*token == _T(' ') || *token == _T('\t')) && total < len)
	{
		token++;
		total++;
	}

	*keyptr = (tchar_t*)token;
	while (*token != itemfeed && *token != _T('\0') && total < len)
	{
		token++;
		total++;
	}
	*keylen = (int)(token - *keyptr);

	if (total == len)
		return token;

	if (*token == itemfeed)
	{
		token++; /*skip item feed*/
		total++;
	}

	/*skip blank*/
	while ((*token == _T(' ') || *token == _T('\t')) && total < len)
	{
		token++;
		total++;
	}

	if (total == len)
		return token;

	*valptr = (tchar_t*)token;
	while (*token != linefeed && *token != _T('\0') && total < len)
	{
		token++;
		total++;
	}
	*vallen = (int)(token - *valptr);

	while (*token == linefeed || *token == _T('\n'))
	{
		token++;
		total++;
	}

	return token;
}

int parse_options_token_count(const tchar_t* options,int len,tchar_t itemfeed,tchar_t linefeed)
{
	int count = 0;
	tchar_t *key,*val;
	const tchar_t* term;
	int klen,vlen;

	if (is_null(options) || !len)
		return 0;

	if (len < 0)
		len = xslen(options);

	term = options + len;
	while(options = parse_options_token(options,(int)(term - options),itemfeed,linefeed,&key,&klen,&val,&vlen))
	{
		count ++;
	}

	return count;
}

int get_options_value(const tchar_t* options, int len, tchar_t itemfeed, tchar_t linefeed, const tchar_t* pkey, tchar_t* buf, int max)
{
	int count = 0;
	tchar_t *key, *val;
	const tchar_t* term;
	int klen, vlen;

	if (is_null(options) || !len)
		return 0;

	if (len < 0)
		len = xslen(options);

	term = options + len;
	while (options = parse_options_token(options, (int)(term - options), itemfeed, linefeed, &key, &klen, &val, &vlen))
	{
		if (compare_text(key, klen, pkey, -1, 0) == 0)
		{
			max = (max < vlen) ? max : vlen;
			if (buf)
			{
				xsncpy(buf, val, max);
			}
			return max;
		}
	}

	return 0;
}

const tchar_t* parse_string_token(const tchar_t* tokens,int len, tchar_t itemfeed, tchar_t** pkey, int* pkeylen)
{
	const tchar_t* token;
	int total = 0;

	*pkey = NULL;
	*pkeylen = 0;

	if(is_null(tokens) || !len)
		return NULL;

	if(len < 0)
		len = xslen(tokens);

	token = tokens;

	*pkey = (tchar_t*)token;
	while((itemfeed && *token != itemfeed) || (!itemfeed && IS_STRING_SPLIT(*token)))
	{
		token ++;
		total++;

		if (total >= len)
			break;
	}
	*pkeylen = (int)(token - *pkey);

	if(total == len)
		return token;

	token ++; //skip item feed
	total++;

	return token;
}

int parse_string_token_count(const tchar_t* tokens, int len, tchar_t itemfeed)
{
	int count = 0;
	tchar_t *key;
	int klen;
	const tchar_t* term;

	if (is_null(tokens) || !len)
		return 0;

	if (len < 0)
		len = xslen(tokens);

	term = tokens + len;
	while(tokens = parse_string_token(tokens,(int)(term - tokens),itemfeed,&key,&klen))
	{
		count ++;
	}

	return count;
}

int parse_string_array_count(const tchar_t* tokens)
{
	int count = 0;
	int len;

	len = xslen(tokens);
	while (len)
	{
		count++;
		tokens += (len + 1);
		len = xslen(tokens);
	}

	return count;
}

int parse_string_array(const tchar_t* tokens, tchar_t** sa, int n)
{
	int len, i = 0;

	len = xslen(tokens);
	while (len && i < n)
	{
		sa[i++] = xsclone(tokens);
		tokens += (len + 1);
		len = xslen(tokens);
	}

	return i;
}

int format_string_array(const tchar_t** sa, int n, tchar_t* tokens, int max)
{
	int len, i, total = 0;

	for (i = 0; i < n; i++)
	{
		len = xslen(sa[i]);
		if (total + len + 1 > max)
			break;

		if (tokens)
		{
			xsncpy(tokens, sa[i], len);
			tokens[len] = _T('\0');
			tokens += (len + 1);
		}
		total += (len + 1);
	}

	if (total + 1 > max)
		return total;

	if (tokens)
	{
		tokens[0] = _T('\0');
	}
	total++;

	return total;
}

const tchar_t* parse_param_name(const tchar_t* param, int len, tchar_t itemdot, int* plen)
{
	const tchar_t* token;
	int total = 0;
	int tag = 0;

	*plen = 0;

	if (is_null(param) || !len)
		return NULL;

	if (len < 0)
		len = xslen(param);

	token = param;

	while (total < len)
	{
		if (*token == _T('\'') || *token == _T('\"'))
			tag = (tag == 0) ? 1 : 0;

		if (tag == 0 && *token == itemdot)
			break;

		token++;
		total++;
	}

	if (total == len)
		return NULL;

	if (*token == itemdot)
	{//skip ':'
		token++;
		total++;
	}

	while (*(token + *plen) != _T(' ') && total < len)
	{
		total++;

		*plen = *plen + 1;
	}

	return token;
}

int parse_param_name_count(const tchar_t* param, int len, tchar_t itemdot)
{
	int count = 0;
	int plen;
	const tchar_t* term;

	if (is_null(param) || !len)
		return 0;

	if (len < 0)
		len = xslen(param);

	term = param + len;
	while (param = parse_param_name(param, (int)(term - param), itemdot, &plen))
	{
		count++;
	}

	return count;
}

int max_mon_days(int year, int mon)
{
	if (year < MIN_YEAR || year > MAX_YEAR)
		return 0;

	if (mon < 1 || mon > 12)
		return 0;

	if (year % 4 == 0 && mon == 2)
	{
		return mon_day[0];
	}
	else
	{
		return mon_day[mon];
	}
}

int max_year_days(int year)
{
	if (year < MIN_YEAR || year > MAX_YEAR)
		return 0;

	if (year % 4 == 0)
	{
		return 366;
	}
	else
	{
		return 365;
	}
}

int diff_years(const xdate_t* pdt1, const xdate_t* pdt2)
{
	return pdt2->year - pdt1->year;
}

int diff_months(const xdate_t* pdt1, const xdate_t* pdt2)
{
	return (pdt2->year - pdt1->year) * 12 + (pdt2->mon - pdt1->mon);
}

int diff_days(const xdate_t* pdt1, const xdate_t* pdt2)
{
	int rt;
	int min_year, max_year, min_mon, max_mon, min_day, max_day;
	int n,days = 0;

	rt = compare_date(pdt1, pdt2);
	if (!rt)
		return 0;

	if (rt < 0)
	{
		min_year = pdt1->year;
		max_year = pdt2->year;
		min_mon = pdt1->mon;
		max_mon = pdt2->mon;
		min_day = pdt1->day;
		max_day = pdt2->day;
	}
	else
	{
		min_year = pdt2->year;
		max_year = pdt1->year;
		min_mon = pdt2->mon;
		max_mon = pdt1->mon;
		min_day = pdt2->day;
		max_day = pdt1->day;
	}

	for (n = 1; n < min_mon; n++)
	{
		days -= max_mon_days(min_year, n);
	}

	days -= min_day;

	for (n = min_year; n < max_year; n++)
	{
		days += max_year_days(n);
	}

	for (n = 1; n < max_mon; n++)
	{
		days += max_mon_days(max_year, n);
	}

	days += max_day;

	return (rt < 0)? days : -days;
}

void plus_years(xdate_t* pdt, int years)
{
	pdt->year += years;

	if (pdt->year % 4 != 0 && pdt->mon == 2 && pdt->day == mon_day[0])
	{
		pdt->day = mon_day[pdt->mon];
	}
}

void plus_months(xdate_t* pdt, int months)
{
	if (months < 0)
	{
		pdt->year += (pdt->mon - 12 + months) / 12;
		pdt->mon = 12 + (pdt->mon - 12 + months) % 12;
	}
	else
	{
		pdt->year += (pdt->mon + months) / 12;
		pdt->mon = (pdt->mon + months) % 12;
		if (!pdt->mon)
		{
			pdt->year--;
			pdt->mon = 12;
		}
	}

	if (pdt->day > mon_day[pdt->mon])
		pdt->day = mon_day[pdt->mon]; 
}

void plus_days(xdate_t* pdt, int days)
{
	while (days)
	{
		if (days > 0)
		{
			if (pdt->day + 1 > mon_day[pdt->mon] || (pdt->year % 4 == 0 && pdt->mon == 2 && pdt->day + 1 > mon_day[0]))
			{
				pdt->mon++;
				if (pdt->mon > 12)
				{
					pdt->year++;
					pdt->mon = 1;
				}
				pdt->day = 1;
			}
			else
			{
				pdt->day++;
			}
			days--;
		}
		else
		{
			if (pdt->day - 1 < 1)
			{
				pdt->mon--;
				if (pdt->mon < 1)
				{
					pdt->year--;
					pdt->mon = 12;
				}
				pdt->day = (pdt->year % 4 == 0 && pdt->mon == 2)? mon_day[0] : mon_day[pdt->mon];
			}
			else
			{
				pdt->day--;
			}
			days++;
		}
	}
}

void plus_weeks(xdate_t* pdt, int weeks)
{
	plus_days(pdt, weeks * 7);
}

void plus_hours(xdate_t* pdt, int hours)
{
	int days;
	
	hours += pdt->hour;

	days = hours / 24;
	hours = hours % 24;

	if (days)
	{
		plus_days(pdt, days);
	}

	pdt->hour = hours;
}

void plus_minutes(xdate_t* pdt, int minutes)
{
	int hours;

	minutes += pdt->min;

	hours = minutes / 60;
	minutes = minutes % 60;

	if (hours)
	{
		plus_hours(pdt, hours);
	}

	pdt->min = minutes;
}

void plus_seconds(xdate_t* pdt, int seconds)
{
	int minutes;

	seconds += pdt->sec;

	minutes = seconds / 60;
	seconds = seconds % 60;

	if (minutes)
	{
		plus_minutes(pdt, minutes);
	}

	pdt->sec = seconds;
}

void get_min_date(xdate_t* pdt)
{
	parse_datetime(pdt, SYS_MINDATE);
}

#ifdef XDK_SUPPORT_DATE
void default_calendar(calendar_t* pca)
{
	int i, max_day, wday, weeks;

	max_day = max_mon_days(1970, 1);

	for (i = 0; i < CALENDAR_COL; i++)
	{
		xscpy(pca->calen_week[i], calen_week[i]);
	}

	xmem_zero((void*)pca->calen_days, CALENDAR_ROW * CALENDAR_COL * sizeof(int));

	wday = 4;
	weeks = 0;
	for (i = 1; i <= max_day; i++)
	{
		pca->calen_days[weeks][wday] = i;
		wday++;

		if (wday > 6)
		{
			wday = 0;
			weeks++;
		}
	}
}

void fill_calendar(calendar_t* pca, const xdate_t* pdt)
{
	int i, max_day, weeks;
	xdate_t xd = { 0 };

	xd.year = pdt->year;
	xd.mon = pdt->mon;
	xd.day = 1;

	mak_loc_date(&xd);
	max_day = max_mon_days(xd.year, xd.mon);

	for (i = 0; i < CALENDAR_COL; i++)
	{
		xscpy(pca->calen_week[i], calen_week[i]);
	}

	xmem_zero((void*)pca->calen_days, CALENDAR_ROW * CALENDAR_COL * sizeof(int));

	weeks = 0;
	for (i = 1; i <= max_day; i++)
	{
		pca->calen_days[weeks][xd.wday] = i;
		xd.wday++;

		if (xd.wday > 6)
		{
			xd.wday = 0;
			weeks++;
		}
	}
}
#endif

bool_t verify_datetime(const xdate_t* pdt)
{
	if (pdt->year < MIN_YEAR || pdt->year > MAX_YEAR)
		return 0;

	if (pdt->mon < 1 || pdt->mon > 12)
		return 0;

	if (pdt->year % 4 == 0 && pdt->mon == 2)
	{
		if (pdt->day < 1 || pdt->day > mon_day[0])
			return 0;
	}
	else
	{
		if (pdt->day < 1 || pdt->day > mon_day[pdt->mon])
			return 0;
	}

	if (pdt->hour < 0 || pdt->hour > 23)
		return 0;

	if (pdt->min < 0 || pdt->min > 59)
		return 0;

	if (pdt->sec < 0 || pdt->min > 59)
		return 0;

	return 1;
}

bool_t is_datetime(const tchar_t* token)
{
	xdate_t xd = { 0 };

	parse_datetime(&xd, token);

	return verify_datetime(&xd);
}

int compare_datetime(const xdate_t* pmd1, const xdate_t* pmd2)
{
	if (!pmd1 && !pmd2)
		return 0;
	else if (pmd1 && !pmd2)
		return 1;
	else if (!pmd1 && pmd2)
		return -1;

	if(pmd1->year > pmd2->year)
		return 1;
	else if(pmd1->year < pmd2->year)
		return -1;
	else if(pmd1->mon > pmd2->mon)
		return 1;
	else if(pmd1->mon < pmd2->mon)
		return -1;
	else if(pmd1->day > pmd2->day)
		return 1;
	else if(pmd1->day < pmd2->day)
		return -1;
	else if(pmd1->hour > pmd2->hour)
		return 1;
	else if(pmd1->hour < pmd2->hour)
		return -1;
	else if(pmd1->min > pmd2->min)
		return 1;
	else if(pmd1->min < pmd2->min)
		return -1;
	else if(pmd1->sec > pmd2->sec)
		return 1;
	else if(pmd1->sec < pmd2->sec)
		return -1;
	else 
		return 0;
}

int compare_date(const xdate_t* pmd1, const xdate_t* pmd2)
{
	if (!pmd1 && !pmd2)
		return 0;
	else if (pmd1 && !pmd2)
		return 1;
	else if (!pmd1 && pmd2)
		return -1;

	if (pmd1->year > pmd2->year)
		return 1;
	else if (pmd1->year < pmd2->year)
		return -1;
	else if (pmd1->mon > pmd2->mon)
		return 1;
	else if (pmd1->mon < pmd2->mon)
		return -1;
	else if (pmd1->day > pmd2->day)
		return 1;
	else if (pmd1->day < pmd2->day)
		return -1;
	else
		return 0;
}

int compare_time(const xdate_t* pmt1, const xdate_t* pmt2)
{
	if (!pmt1 && !pmt2)
		return 0;
	else if (pmt1 && !pmt2)
		return 1;
	else if (!pmt1 && pmt2)
		return -1;

	if (pmt1->hour > pmt2->hour)
		return 1;
	else if (pmt1->hour < pmt2->hour)
		return -1;
	else if (pmt1->min > pmt2->min)
		return 1;
	else if (pmt1->min < pmt2->min)
		return -1;
	else if (pmt1->sec > pmt2->sec)
		return 1;
	else if (pmt1->sec < pmt2->sec)
		return -1;
	else
		return 0;
}

int split_line(const tchar_t* token, int len)
{
	int tklen = 0, total = 0;
	bool_t glt = 0;
	const tchar_t* tkcur = token;

	if (len < 0)
		len = xslen(token);

	if (!len)
		return 0;

	while (*tkcur != _T('\r') && *tkcur != _T('\n') && *tkcur != _T('\0') && total < len)
	{
		if (*tkcur == _T('\'') || *tkcur == _T('\"'))
		{
			if (glt)
				glt = 0;
			else
				glt = 1;
		}

		tklen++;
		tkcur++;
		total++;

		if (glt)
		{
			while (*tkcur == _T('\r') || *tkcur == _T('\n'))
			{
				tklen++;
				tkcur++;
				total++;
			}
		}
	}

	return total;
}

bool_t split_xmlns(tchar_t* str, int* kat, int* klen, int* vat, int* vlen)
{
	tchar_t* token = str;

	while (*token != _T(':') && *token != _T('\0'))
	{
		token++;
	}

	if (*token == _T(':'))
	{
		*kat = 0;
		*klen = (int)(token - str);

		*vat = (int)(token - str) + 1;
		*vlen = -1;

		return 1;
	}
	else
	{
		*kat = *klen = 0;
		*vat = 0;
		*vlen = -1;

		return 0;
	}
}

const tchar_t* skip_xmlns(const tchar_t* str, int slen)
{
	tchar_t* token;

	if (slen < 0)
		slen = xslen(str);

	if (!slen)
		return NULL;

	token = (tchar_t*)(str + slen);

	while (*token != _T(':') && token != str)
	{
		token--;
	}

	if (*token == _T(':'))
	{
		token++;
	}

	return token;
}

int trim_xmlns(tchar_t* str, int slen)
{
	tchar_t* token;
	int len;

	if (slen < 0)
		slen = xslen(str);

	if (!slen)
		return 0;

	token = (tchar_t*)(str + slen);

	while (*token != _T(':') && token != str)
	{
		token--;
	}

	if (*token == _T(':'))
	{
		token++;
	}

	len = (int)(token - str);
	xsndel(str, 0, len);

	return slen - len;
}

int compare_nons(const tchar_t* src, int srclen, const tchar_t* dest, int destlen)
{
	const tchar_t *no_src, *no_dest;

	if (srclen < 0)
		srclen = xslen(src);
	no_src = skip_xmlns(src, srclen);
	srclen -= (int)(no_src - src);

	if (destlen < 0)
		destlen = xslen(dest);
	no_dest = skip_xmlns(dest, destlen);
	destlen -= (int)(no_dest - dest);

	return compare_text(no_src, srclen, no_dest, destlen, 1);
}

//compare to text token by len and case options
int compare_text(const tchar_t* src,int srclen,const tchar_t* dest,int destlen,int nocase)
{
	int cmplen;
	int rt;

	if(is_null(src) && is_null(dest))
		return 0;

	if(srclen == -1)
		srclen = xslen(src);
	if(destlen == -1)
		destlen = xslen(dest);

	cmplen = (srclen < destlen)? srclen : destlen;

	if(nocase)
		rt = xsnicmp(src,dest,cmplen);
	else
		rt = xsncmp(src,dest,cmplen);

	if(rt == 0 && srclen < destlen)
		rt = -1;
	else if(rt == 0 && srclen > destlen)
		rt = 1;
	
	return rt;
}

int compare_numeric(const tchar_t* szSrc, const tchar_t* szDes, int digi)
{
	double db1, db2;

	db1 = xstonum(szSrc);
	db2 = xstonum(szDes);

	if (db1 == db2)
		return 0;
	else if (db1 > db2)
		return 1;
	else
		return -1;
}


long mul_div_long(long m1, long m2, long d)
{
	return (long)((double)(m1 * m2) / (double)d);
}

short mul_div_short(short m1, short m2, short d)
{
	return (short)((float)(m1 * m2) / (float)d);
}

static int _next_book_mark(tchar_t* book_mark)
{
	int t1,t2,t3;

	if (!book_mark)
		return 0;

	if(xslen(book_mark) == 0)
	{
		book_mark[0] = 42;
		book_mark[1] = 42;
		book_mark[2] = 42;
		return 1;
	}
	
	t1 = (int)book_mark[0];
	t2 = (int)book_mark[1];
	t3 = (int)book_mark[2];

	t3 ++;
	if(t3 == 49)
	{
		t3 = 42;
		if(t2 < 91)
			t2 += 7;
		else
		{
			t2 = 42;
			if(t1 < 91)
				t1 += 7;
			else
				return 0;
		}
	}

	book_mark[0] = t1;
	book_mark[1] = t2;
	book_mark[2] = t3;

	return 1;
}

int parse_proto(const tchar_t* file)
{
	if(!file)
		return -1;

	if(xsnicmp(file,_T("http:"),xslen(_T("http:"))) == 0)
	{
		return _PROTO_HTTP;
	}else if(xsnicmp(file,_T("tftp:"),xslen(_T("tftp:"))) == 0)
	{
		return _PROTO_TFTP;
	}
	else if (xsnicmp(file, _T("oss:"), xslen(_T("oss:"))) == 0)
	{
		return _PROTO_OSS;
	}
	else if (xsnicmp(file, _T("\\\\"), xslen(_T("\\\\"))) == 0)
	{
		return _PROTO_NFS;
	}else
	{
		if(xsstr(file,_T(":")))
			return _PROTO_LOC;
		else
			return _PROTO_CUR;
	}
}

void parse_url(const tchar_t* url,tchar_t** proat,int* prolen,tchar_t** addrat,int* addrlen,tchar_t** portat,int* portlen,tchar_t** objat,int* objlen,tchar_t** qryat,int* qrylen)
{
	tchar_t* token = (tchar_t*)url;

	*proat = *addrat = *portat = *objat = *qryat = NULL;
	*prolen = *addrlen = *portlen = *objlen = *qrylen = 0;

	if(!token)
		return ;

	/*skip http://*/
	if(token = (tchar_t*)xsstr(url,_T("://")))
	{
		*proat = (tchar_t*)url;
		*prolen = (int)(token - url);
		token += xslen(_T("://"));
	}else
	{
		token = (tchar_t*)url;
	}
	
	/*get www.aaa.bbb*/
	*addrat = token;
	while (*token != _T('\0') && *token != _T(':') && *token != _T('/') && *token != _T('\\'))
	{
		*addrlen = *addrlen + 1;
		token ++;
	}
	if(*token == _T('\0'))
		return;
	
	if(*token == _T(':'))
	{
		/*skip ':'*/
		token ++;
		/*get 80*/
		*portat = token;
		while (*token != _T('\0') && *token != _T('/') && *token != _T('\\'))
		{
			*portlen = *portlen + 1;
			token ++;
		}
		if(*token == _T('\0'))
			return;
	}

	/*get somfile.html*/
	*objat = token;
	while(*token != _T('\0') && *token != _T('?'))
	{
		*objlen = *objlen + 1;
		token ++;
	}
	if(*token == _T('\0'))
		return;

	/*skip '?'*/
	token ++;
	/*skip blank*/
	while((*token == _T(' ') || *token == _T('\t')) && *token != _T('\0'))
		token ++;

	/*get key=val...*/
	*qryat = token;
	while(*token != _T('\0'))
	{
		*qrylen = *qrylen + 1;
		token ++;
	}
}

int printf_path(tchar_t* fpath, const tchar_t* strfmt, ...)
{
	tchar_t* tname;
	int tlen, total = 0;
	const tchar_t* tk;
	tchar_t *str;
	int slen;

	va_list arg;

	va_start(arg, strfmt);

	tk = strfmt;
	while (tk && *tk != _T('\0'))
	{
		if (*tk == _T('$') && *(tk + 1) == _T('('))
		{
			tk++;
			tname = tk;
			tlen = 0;
			while (*tk != _T(')'))
			{
				tk++;
				tlen++;
			}

			str = va_arg(arg, tchar_t*);
			slen = xslen(str);

			if (fpath)
			{
				xsncpy(fpath + total, str, slen);
			}
			total += slen;

			if (*tk == _T(')'))
			{
				tk++;
			}
		}
		else
		{
			if (fpath)
			{
				fpath[total] = *tk;
			}
			total++;
			
			tk++;
		}
	}

	if (fpath)
	{
		fpath[total] = _T('\0');
	}

	va_end(arg);

	return total;
}

void split_path(const tchar_t* pathfile, tchar_t* path, tchar_t* file, tchar_t* ext)
{
	const tchar_t* token;
	int extlen = 0;

	if (is_null(pathfile))
		return;

	token = pathfile + xslen(pathfile);
	while (token != pathfile && *token != _T('\\') && *token != _T('/'))
	{
		token--;

		if (*token == _T('.'))
		{
			if (ext)
				xscpy(ext, token + 1);
			extlen = xslen(token);
			break;
		}
	}

	while (token != pathfile && *token != _T('\\') && *token != _T('/'))
	{
		token--;
	}

	if (file)
	{
		if (*token == _T('\\') || *token == _T('/'))
			xsncpy(file, token + 1, xslen(token + 1) - extlen);
		else
			xsncpy(file, token, xslen(token) - extlen);
	}

	if (path)
	{
		xsncpy(path, pathfile, (int)(token - pathfile));
	}
}

void split_file(const tchar_t* pathfile, tchar_t* path, tchar_t* file)
{
	const tchar_t* token;
	int extlen = 0;

	if (is_null(pathfile))
		return;

	token = pathfile + xslen(pathfile);
	
	while (token != pathfile && *token != _T('\\') && *token != _T('/'))
	{
		token--;
	}

	if (file)
	{
		if (*token == _T('\\') || *token == _T('/'))
			xsncpy(file, token + 1, xslen(token + 1) - extlen);
		else
			xsncpy(file, token, xslen(token) - extlen);
	}

	if (path)
	{
		xsncpy(path, pathfile, (int)(token - pathfile));
	}
}

/************************************************************************************************/
void parse_xpen_from_style(xpen_t* pxp, const tchar_t* style)
{
	const tchar_t* token;
	tchar_t *key, *val;
	int klen, vlen;

	token = style;
	while (token)
	{
		token = parse_options_token(token, -1, CSS_ITEMFEED, CSS_LINEFEED, &key, &klen, &val, &vlen);

		if (compare_text(GDI_ATTR_STROKE_STYLE, -1, key, klen, 1) == 0)
			xsncpy(pxp->style, val, vlen);
		else if (compare_text(GDI_ATTR_STROKE_COLOR, -1, key, klen, 1) == 0)
			xsncpy(pxp->color, val, vlen);
		else if (compare_text(GDI_ATTR_STROKE_WIDTH, -1, key, klen, 1) == 0)
			xsncpy(pxp->size, val, vlen);
		else if (compare_text(GDI_ATTR_STROKE_OPACITY, -1, key, klen, 1) == 0)
			xsncpy(pxp->opacity, val, vlen);
	}
}

int format_xpen_to_style(const xpen_t* pxp, tchar_t* buf, int max)
{
	int len, total = 0;

	if (!is_null(pxp->style))
	{
		len = xslen(GDI_ATTR_STROKE_STYLE) + xslen(pxp->style) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STROKE_STYLE, CSS_ITEMFEED, pxp->style, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxp->size))
	{
		len = xslen(GDI_ATTR_STROKE_WIDTH) + xslen(pxp->size) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STROKE_WIDTH, CSS_ITEMFEED, pxp->size, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxp->color))
	{
		len = xslen(GDI_ATTR_STROKE_COLOR) + xslen(pxp->color) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STROKE_COLOR, CSS_ITEMFEED, pxp->color, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxp->opacity))
	{
		len = xslen(GDI_ATTR_STROKE_OPACITY) + xslen(pxp->opacity) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_STROKE_OPACITY, CSS_ITEMFEED, pxp->opacity, CSS_LINEFEED);
		}
		total += len;
	}

	return total;
}

void parse_xbrush_from_style(xbrush_t* pxb, const tchar_t* style)
{
	const tchar_t* token;
	tchar_t *key, *val;
	int klen, vlen;

	token = style;
	while (token)
	{
		token = parse_options_token(token, -1, CSS_ITEMFEED, CSS_LINEFEED, &key, &klen, &val, &vlen);

		if (compare_text(GDI_ATTR_FILL_STYLE, -1, key, klen, 1) == 0)
			xsncpy(pxb->style, val, vlen);
		else if (compare_text(GDI_ATTR_FILL_COLOR, -1, key, klen, 1) == 0)
			xsncpy(pxb->color, val, vlen);
		else if (compare_text(GDI_ATTR_FILL_OPACITY, -1, key, klen, 1) == 0)
			xsncpy(pxb->opacity, val, vlen);
	}
}

int format_xbrush_to_style(const xbrush_t* pxb, tchar_t* buf, int max)
{
	int len, total = 0;

	if (!is_null(pxb->style))
	{
		len = xslen(GDI_ATTR_FILL_STYLE) + xslen(pxb->style) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FILL_STYLE, CSS_ITEMFEED, pxb->style, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxb->color))
	{
		len = xslen(GDI_ATTR_FILL_COLOR) + xslen(pxb->color) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FILL_COLOR, CSS_ITEMFEED, pxb->color, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pxb->opacity))
	{
		len = xslen(GDI_ATTR_FILL_OPACITY) + xslen(pxb->opacity) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FILL_OPACITY, CSS_ITEMFEED, pxb->opacity, CSS_LINEFEED);
		}
		total += len;
	}

	return total;
}

void parse_xfont_from_style(xfont_t* pxf, const tchar_t* style)
{
	const tchar_t* token;
	tchar_t *key, *val;
	int klen, vlen;

	token = style;
	while (token)
	{
		token = parse_options_token(token, -1, CSS_ITEMFEED, CSS_LINEFEED, &key, &klen, &val, &vlen);

		if (compare_text(GDI_ATTR_FONT_STYLE, -1, key, klen, 1) == 0)
			xsncpy(pxf->style, val, vlen);
		else if (compare_text(GDI_ATTR_FONT_SIZE, -1, key, klen, 1) == 0)
			xsncpy(pxf->size, val, vlen);
		else if (compare_text(GDI_ATTR_FONT_WEIGHT, -1, key, klen, 1) == 0)
			xsncpy(pxf->weight, val, vlen);
		else if (compare_text(GDI_ATTR_FONT_FAMILY, -1, key, klen, 1) == 0)
			xsncpy(pxf->family, val, vlen);
		else if (compare_text(GDI_ATTR_FONT_COLOR, -1, key, klen, 1) == 0)
			xsncpy(pxf->color, val, vlen);
	}
}

int format_xfont_to_style(const xfont_t* pfont, tchar_t* buf, int max)
{
	int len, total = 0;

	if (!is_null(pfont->family))
	{
		len = xslen(GDI_ATTR_FONT_FAMILY) + xslen(pfont->family) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_FAMILY, CSS_ITEMFEED, pfont->family, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pfont->style))
	{
		len = xslen(GDI_ATTR_FONT_STYLE) + xslen(pfont->style) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_STYLE, CSS_ITEMFEED, pfont->style, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pfont->size))
	{
		len = xslen(GDI_ATTR_FONT_SIZE) + xslen(pfont->size) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_SIZE, CSS_ITEMFEED, pfont->size, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pfont->weight))
	{
		len = xslen(GDI_ATTR_FONT_WEIGHT) + xslen(pfont->weight) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_WEIGHT, CSS_ITEMFEED, pfont->weight, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(pfont->color))
	{
		len = xslen(GDI_ATTR_FONT_COLOR) + xslen(pfont->color) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_FONT_COLOR, CSS_ITEMFEED, pfont->color, CSS_LINEFEED);
		}
		total += len;
	}

	return total;
}

void parse_xface_from_style(xface_t* ptt, const tchar_t* style)
{
	const tchar_t* token;
	tchar_t *key, *val;
	int klen, vlen;

	token = style;
	while (token)
	{
		token = parse_options_token(token, -1, CSS_ITEMFEED, CSS_LINEFEED, &key, &klen, &val, &vlen);

		if (compare_text(GDI_ATTR_TEXT_ALIGN, -1, key, klen, 1) == 0)
			xsncpy(ptt->text_align, val, vlen);
		else if (compare_text(GDI_ATTR_LINE_ALIGN, -1, key, klen, 1) == 0)
			xsncpy(ptt->line_align, val, vlen);
		else if (compare_text(GDI_ATTR_TEXT_WRAP, -1, key, klen, 1) == 0)
			xsncpy(ptt->text_wrap, val, vlen);
		else if (compare_text(GDI_ATTR_LINE_HEIGHT, -1, key, klen, 1) == 0)
			xsncpy(ptt->line_height, val, vlen);
	}
}

int format_xface_to_style(const xface_t* ptt, tchar_t* buf, int max)
{
	int len, total = 0;

	if (!is_null(ptt->text_align))
	{
		len = xslen(GDI_ATTR_TEXT_ALIGN) + xslen(ptt->text_align) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_TEXT_ALIGN, CSS_ITEMFEED, ptt->text_align, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(ptt->line_align))
	{
		len = xslen(GDI_ATTR_LINE_ALIGN) + xslen(ptt->line_align) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_LINE_ALIGN, CSS_ITEMFEED, ptt->line_align, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(ptt->text_wrap))
	{
		len = xslen(GDI_ATTR_TEXT_WRAP) + xslen(ptt->text_wrap) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_TEXT_WRAP, CSS_ITEMFEED, ptt->text_wrap, CSS_LINEFEED);
		}
		total += len;
	}

	if (!is_null(ptt->line_height))
	{
		len = xslen(GDI_ATTR_LINE_HEIGHT) + xslen(ptt->line_height) + 2;
		if (total + len > max)
			return -1;
		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), GDI_ATTR_LINE_HEIGHT, CSS_ITEMFEED, ptt->line_height, CSS_LINEFEED);
		}
		total += len;
	}

	return total;
}

void parse_ximage_from_source(ximage_t* pxi, const tchar_t* token)
{
	if (compare_text(token, xslen(GDI_ATTR_IMAGE_TYPE_JPG), GDI_ATTR_IMAGE_TYPE_JPG, -1, 0) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_JPG);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_JPG);
	}
	else if (compare_text(token, xslen(GDI_ATTR_IMAGE_TYPE_PNG), GDI_ATTR_IMAGE_TYPE_PNG, -1, 0) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_PNG);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_PNG);
	}
	else if (compare_text(token, xslen(GDI_ATTR_IMAGE_TYPE_BMP), GDI_ATTR_IMAGE_TYPE_BMP, -1, 0) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_BMP);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_BMP);
	}
	else if (compare_text(token, xslen(GDI_ATTR_IMAGE_TYPE_COLOR), GDI_ATTR_IMAGE_TYPE_COLOR, -1, 0) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_COLOR);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_COLOR);
	}
	else if (compare_text(token, xslen(GDI_ATTR_IMAGE_TYPE_CODE128), GDI_ATTR_IMAGE_TYPE_CODE128, -1, 0) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_CODE128);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_CODE128);
	}
	else if (compare_text(token, xslen(GDI_ATTR_IMAGE_TYPE_PDF417), GDI_ATTR_IMAGE_TYPE_PDF417, -1, 0) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_PDF417);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_PDF417);
	}
	else if (compare_text(token, xslen(GDI_ATTR_IMAGE_TYPE_QRCODE), GDI_ATTR_IMAGE_TYPE_QRCODE, -1, 0) == 0)
	{
		xscpy(pxi->type, GDI_ATTR_IMAGE_TYPE_QRCODE);
		pxi->source = token + xslen(GDI_ATTR_IMAGE_TYPE_QRCODE);
	}
	else
	{
		xscpy(pxi->type, _T(""));
		pxi->source = NULL;
	}
}

int format_ximage_to_source(const ximage_t* pxi, tchar_t* buf, int len)
{
	int size;

	size = xslen(pxi->type);

	if (size > len)
		return size;

	if (buf)
	{
		xscpy(buf, pxi->type);
	}

	size += xslen(pxi->source);

	if (size > len)
		return size;

	if (buf)
	{
		xscpy(buf + xslen(pxi->type), pxi->source);
	}

	return size;
}

bool_t inside_rowcol(int row, int col, int from_row, int from_col, int to_row, int to_col)
{
	if (row < from_row || (row == from_row && col < from_col))
		return 0;

	if (row > to_row || (row == to_row && col > to_col))
		return 0;

	return 1;
}

int compare_rowcol(int from_row, int from_col, int to_row, int to_col)
{
	if (from_row == to_row && from_col == to_col)
		return 0;
	else if (from_row == to_row && from_col < to_col)
		return -1;
	else if (from_row == to_row && from_col > to_col)
		return 1;
	else if (from_row < to_row)
		return -1;
	else
		return 1;
}

bool_t pt_inside(long x, long y, long x1, long y1, long x2, long y2)
{
	long minx, maxx, miny, maxy;

	minx = (x1 < x2) ? x1 : x2;
	maxx = (x1 > x2) ? x1 : x2;
	miny = (y1 < y2) ? y1 : y2;
	maxy = (y1 > y2) ? y1 : y2;

	return (x >= minx && x <= maxx && y >= miny && y <= maxy) ? 1 : 0;
}

bool_t ft_inside(float x, float y, float x1, float y1, float x2, float y2)
{
	float minx, maxx, miny, maxy;

	minx = (x1 < x2) ? x1 : x2;
	maxx = (x1 > x2) ? x1 : x2;
	miny = (y1 < y2) ? y1 : y2;
	maxy = (y1 > y2) ? y1 : y2;

	return (x >= minx && x <= maxx && y >= miny && y <= maxy) ? 1 : 0;
}

bool_t pt_in_rect(const xpoint_t* ppt, const xrect_t* pxr)
{
	return pt_inside(ppt->x, ppt->y, pxr->x, pxr->y, pxr->x + pxr->w, pxr->y + pxr->h);
}


bool_t ft_in_rect(const xpoint_t* ppt, const xrect_t* pxr)
{
	return ft_inside(ppt->fx, ppt->fy, pxr->fx, pxr->fy, pxr->fx + pxr->fw, pxr->fy + pxr->fh);
}

void ft_offset_point(xpoint_t* ppt, float cx, float cy)
{
	ppt->fx += cx;
	ppt->fy += cy;
}

void pt_offset_point(xpoint_t* ppt, long cx, long cy)
{
	ppt->x += cx;
	ppt->y += cy;
}

void ft_center_rect(xrect_t* pxr, float cx, float cy)
{
	pxr->fx += (pxr->fw - cx) / 2;
	pxr->fw = cx;
	pxr->fy += (pxr->fh - cy) / 2;
	pxr->fh = cy;
}

void pt_center_rect(xrect_t* pxr, long cx, long cy)
{
	pxr->x += (pxr->w - cx) / 2;
	pxr->w = cx;
	pxr->y += (pxr->h - cy) / 2;
	pxr->h = cy;
}

void pt_expand_rect(xrect_t* pxr, long cx, long cy)
{
	pxr->x -= cx;
	pxr->w += cx * 2;
	pxr->y -= cy;
	pxr->h += cy * 2;
}

void ft_expand_rect(xrect_t* pxr, float cx, float cy)
{
	pxr->fx -= cx;
	pxr->fw += cx * 2;
	pxr->fy -= cy;
	pxr->fh += cy * 2;
}

void pt_offset_rect(xrect_t* pxr, long cx, long cy)
{
	pxr->x += cx;
	pxr->y += cy;
}

void ft_offset_rect(xrect_t* pxr, float cx, float cy)
{
	pxr->fx += cx;
	pxr->fy += cy;
}

void ft_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt)
{
	float left, top, right, bottom;

	left = (pxr->fx < pxr_nxt->fx) ? pxr->fx : pxr_nxt->fx;
	top = (pxr->fy < pxr_nxt->fy) ? pxr->fy : pxr_nxt->fy;
	right = (pxr->fx + pxr->fw > pxr_nxt->fx + pxr_nxt->fw) ? (pxr->fx + pxr->fw) : (pxr_nxt->fx + pxr_nxt->fw);
	bottom = (pxr->fy + pxr->fh > pxr_nxt->fy + pxr_nxt->fh) ? (pxr->fy + pxr->fh) : (pxr_nxt->fy + pxr_nxt->fh);

	pxr->fx = left;
	pxr->fy = top;
	pxr->fw = right - left;
	pxr->fh = bottom - top;
}

void pt_merge_rect(xrect_t* pxr, const xrect_t* pxr_nxt)
{
	long left, top, right, bottom;

	left = (pxr->x < pxr_nxt->x) ? pxr->x : pxr_nxt->x;
	top = (pxr->y < pxr_nxt->y) ? pxr->y : pxr_nxt->y;
	right = (pxr->x + pxr->w > pxr_nxt->x + pxr_nxt->w) ? (pxr->x + pxr->w) : (pxr_nxt->x + pxr_nxt->w);
	bottom = (pxr->y + pxr->h > pxr_nxt->y + pxr_nxt->h) ? (pxr->y + pxr->h) : (pxr_nxt->y + pxr_nxt->h);

	pxr->x = left;
	pxr->y = top;
	pxr->w = right - left;
	pxr->h = bottom - top;
}

bool_t ft_clip_rect(xrect_t* pxr, const xrect_t* pxrClp)
{
	if (pxr->fx == pxrClp->fx && pxr->fy == pxrClp->fy)
	{
		if (pxr->fh == pxrClp->fh && pxr->fw > pxrClp->fw)
		{
			pxr->fx += pxrClp->fw;
			return 1;
		}
		else if (pxr->fw == pxrClp->fw && pxr->fh > pxrClp->fh)
		{
			pxr->fy += pxrClp->fh;
			return 1;
		}
	}
	else if (pxr->fx + pxr->fw == pxrClp->fx + pxrClp->fw && pxr->fy + pxr->fh == pxrClp->fy + pxrClp->fh)
	{
		if (pxr->fh == pxrClp->fh && pxr->fw > pxrClp->fw)
		{
			pxr->fw -= pxrClp->fw;
			return 1;
		}
		else if (pxr->fw == pxrClp->fw && pxr->fh > pxrClp->fh)
		{
			pxr->fh -= pxrClp->fh;
			return 1;
		}
	}

	return 0;
}

bool_t pt_clip_rect(xrect_t* pxr, const xrect_t* pxrClp)
{
	if (pxr->x == pxrClp->x && pxr->y == pxrClp->y)
	{
		if (pxr->h == pxrClp->h && pxr->w > pxrClp->w)
		{
			pxr->x += pxrClp->w;
			return 1;
		}
		else if (pxr->w == pxrClp->w && pxr->h > pxrClp->h)
		{
			pxr->y += pxrClp->h;
			return 1;
		}
	}
	else if (pxr->x + pxr->w == pxrClp->x + pxrClp->w && pxr->y + pxr->h == pxrClp->y + pxrClp->h)
	{
		if (pxr->h == pxrClp->h && pxr->w > pxrClp->w)
		{
			pxr->w -= pxrClp->w;
			return 1;
		}
		else if (pxr->w == pxrClp->w && pxr->h > pxrClp->h)
		{
			pxr->h -= pxrClp->h;
			return 1;
		}
	}

	return 0;
}

void ft_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub)
{
	if (pxr->fx < pxr_sub->fx)
	{
		pxr->fx = pxr_sub->fx;
		pxr->fw -= (pxr_sub->fx - pxr->fx);
	}
	if (pxr->fy > pxr_sub->fy)
	{
		pxr->fy = pxr_sub->fy;
		pxr->fh += (pxr->fy - pxr_sub->fy);
	}
}

void pt_inter_rect(xrect_t* pxr, const xrect_t* pxr_sub)
{
	if (pxr->x < pxr_sub->x)
	{
		pxr->x = pxr_sub->x;
		pxr->w -= (pxr_sub->x - pxr->x);
	}
	if (pxr->y > pxr_sub->y)
	{
		pxr->y = pxr_sub->y;
		pxr->h += (pxr->y - pxr_sub->y);
	}
}

void ft_cell_rect(xrect_t* pxr, int rows, int cols, int index)
{
	int row, col;

	row = index / cols;
	col = index % cols;

	pxr->fx += pxr->fw * col / cols;
	pxr->fw = pxr->fw / cols;
	pxr->fy += pxr->fh * row / rows;
	pxr->fh = pxr->fh / rows;
}

void pt_cell_rect(xrect_t* pxr, int rows, int cols, int index)
{
	int row, col;

	row = index / cols;
	col = index % cols;

	pxr->x += pxr->w * col / cols;
	pxr->w = pxr->w / cols;
	pxr->y += pxr->h * row / rows;
	pxr->h = pxr->h / rows;
}

void empty_rect(xrect_t* pxr)
{
	xmem_zero((void*)pxr, sizeof(xrect_t));
}

bool_t rect_is_empty(const xrect_t* pxr)
{
	return (!pxr->w || !pxr->h) ? 1 : 0;
}

int ft_parse_points_from_token(xpoint_t* ppt, int max, const tchar_t* token, int len)
{
	tchar_t *key = NULL;
	tchar_t *val = NULL;
	int klen = 0;
	int vlen = 0;
	int count = 0;

	while (token = parse_options_token(token, len, _T(' '), _T(','), &key, &klen, &val, &vlen))
	{
		if (ppt)
		{
			ppt[count].fx = xsntof(key, klen);
			ppt[count].fy = xsntof(val, vlen);
		}
		count++;
	}

	return count;
}

int ft_format_points_to_token(const xpoint_t* ppt, int n, tchar_t* buf, int max)
{
	int i;
	int len, total = 0;

	for (i = 0; i < n; i++)
	{
		len = xsprintf(NULL, _T("%.1f %.1f,"), ppt[i].fx, ppt[i].fy);
		if (len + total > max)
			return total;

		if (buf)
		{
			len = xsprintf(buf + total, _T("%.1f %.1f,"), ppt[i].fx, ppt[i].fy);
		}
		total += len;
	}

	if (total && buf)
	{
		buf[total-1] = _T('\0');
	}

	return total;
}

int pt_parse_points_from_token(xpoint_t* ppt, int max, const tchar_t* token, int len)
{
	tchar_t *key = NULL;
	tchar_t *val = NULL;
	int klen = 0;
	int vlen = 0;
	int count = 0;

	while (token = parse_options_token(token, len, _T(' '), _T(','), &key, &klen, &val, &vlen))
	{
		if (ppt)
		{
			ppt[count].x = xsntol(key, klen);
			ppt[count].y = xsntol(val, vlen);
		}
		count++;
	}

	return count;
}

int pt_format_points_to_token(const xpoint_t* ppt, int n, tchar_t* buf, int max)
{
	int i;
	int len, total = 0;

	for (i = 0; i < n; i++)
	{
		len = xsprintf(NULL, _T("%d %d,"), ppt[i].x, ppt[i].y);
		if (len + total > max)
			return total;

		if (buf)
		{
			len = xsprintf(buf + total, _T("%d %d,"), ppt[i].x, ppt[i].y);
		}
		total += len;
	}

	if (total && buf)
	{
		buf[total-1] = _T('\0');
	}

	return total;
}

int parse_dicm_point(const tchar_t* token, int len, xpoint_t* ppt, int max)
{
	int i, n;

	if (len < 0)
		len = xslen(token);

	if (is_null(token) || !len)
		return 0;

	i = 0;
	while (*token && len && i < max)
	{
		n = 0;
		while (*token != _T('/') && *token != _T('\\') && *token != _T('\0') && n < len)
		{
			token++;
			n++;
		}
		if (ppt)
		{
			ppt[i].x = xsntol(token - n, n);
		}

		if (*token == _T('/') || *token == _T('\\'))
		{
			token++;
			n++;
		}
		len -= n;

		n = 0;
		while (*token != _T('/') && *token != _T('\\') && *token != _T('\0') && n < len)
		{
			token++;
			n++;
		}
		if (ppt)
		{
			ppt[i].y = xsntol(token - n, n);
		}

		if (*token == _T('/') || *token == _T('\\'))
		{
			token++;
			n++;
		}
		len -= n;

		i++;
	}

	return i;
}

int format_dicm_point(const xpoint_t* ppt, int count, tchar_t* buf, int max)
{
	int i, n, total = 0;;

	for (i = 0; i < count; i++)
	{
		n = xsprintf(((buf) ? (buf + total) : NULL), _T("%d/%d/"), ppt[i].x, ppt[i].y);

		if (total + n > max)
			break;

		total += n;
	}

	if (total)
	{
		buf[total - 1] = _T('\0'); //last /
		total--;
	}

	return total;
}

#ifdef XDL_SUPPORT_CRYPT
void a_md5_token(const byte_t dig_buf[MD5_SIZE], schar_t str_buf[MD5_LEN])
{
	int i;
	for (i = 0; i < MD5_SIZE; i++)
	{
		a_xsprintf(&(str_buf[i * 2]), "%02x", dig_buf[i] & 0x00FF);
	}
}

void w_md5_token(const byte_t dig_buf[MD5_SIZE], wchar_t str_buf[MD5_LEN])
{
	int i;
	for (i = 0; i < MD5_SIZE; i++)
	{
		w_xsprintf(&(str_buf[i * 2]), L"%02x", dig_buf[i] & 0x00FF);
	}
}

void file_info_etag(const tchar_t* fname, const tchar_t* ftime, const tchar_t* fsize, tchar_t* buf)
{
	byte_t *in_buf;
	byte_t enc_buf[ETAG_LEN + 1] = { 0 };
	int len, total = 0;

#ifdef _UNICODE
	len = ucs_to_utf8(fname, -1, NULL, MAX_LONG) + ucs_to_utf8(ftime, -1, NULL, MAX_LONG) + ucs_to_utf8(fsize, -1, NULL, MAX_LONG);
#else
	len = mbs_to_utf8(fname, -1, NULL, MAX_LONG) + mbs_to_utf8(ftime, -1, NULL, MAX_LONG) + mbs_to_utf8(fsize, -1, NULL, MAX_LONG);
#endif

	in_buf = (byte_t*)xmem_alloc(len + 1);

#ifdef _UNICODE
	len = ucs_to_utf8(fname, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(fname, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(ftime, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(ftime, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(fsize, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(fsize, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

	md5(in_buf, total, enc_buf);

	xmem_free(in_buf);

	buf[0] = _T('\"');
	md5_token(enc_buf, buf + 1);
	total = xslen(buf);
	buf[total] = _T('\"');
	buf[total + 1] = _T('\0');
}
#endif