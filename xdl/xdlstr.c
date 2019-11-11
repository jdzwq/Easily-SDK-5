/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc string document

	@module	xdlstr.c | implement file

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

#include "xdlstr.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

schar_t* a_xsalloc(int len)
{
	return (schar_t*)xmem_alloc(len * sizeof(schar_t));
}

wchar_t* w_xsalloc(int len)
{
	return (wchar_t*)xmem_alloc(len * sizeof(wchar_t));
}

schar_t* a_xsrealloc(schar_t* str,int len)
{
	return (schar_t*)xmem_realloc(str, len * sizeof(schar_t));
}

wchar_t* w_xsrealloc(wchar_t* str,int len)
{
	return (wchar_t*)xmem_realloc(str, len * sizeof(wchar_t));
}

void a_xszero(schar_t* str,int len)
{
	xmem_zero((void*)str, len * sizeof(schar_t));
}

void w_xszero(wchar_t* str,int len)
{
	xmem_zero((void*)str, len * sizeof(wchar_t));
}

void a_xsfree(schar_t* str)
{
	xmem_free(str);
}

void w_xsfree(wchar_t* str)
{
	xmem_free(str);
}

bool_t a_is_null(const schar_t* sz)
{
	return ((!sz || (sz && *sz == '\0')) ? 1 : 0);
}

bool_t w_is_null(const wchar_t* sz)
{
	return ((!sz || (sz && *sz == L'\0')) ? 1 : 0);
}

bool_t a_is_numeric(const schar_t* sz)
{
	schar_t* token;
	int dot_count = 0;
	int blk_count = 0;

	token = (schar_t*)sz;
	while(*token != '\0' && (*token == ' ' || *token == '\t'))
		token ++;
	if(*token == '-' || *token == '+')
		token ++;

	while(*token != '\0')
	{
		switch(*token)
		{
		case '.':
			dot_count ++;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(blk_count || dot_count > 1)
				return 0;
			break;
		case ' ':
		case '\t':
			if(dot_count > 1)
				return 0;
			blk_count ++;
			break;
		default:
			return 0;
		}
		token ++;
	}
	return 1;
}

bool_t w_is_numeric(const wchar_t* sz)
{
	wchar_t* token;
	int dot_count = 0;
	int blk_count = 0;

	token = (wchar_t*)sz;
	while(*token != L'\0' && (*token == L' ' || *token == L'\t'))
		token ++;
	if(*token == L'-' || *token == L'+')
		token ++;

	while(*token != L'\0')
	{
		switch(*token)
		{
		case L'.':
			dot_count ++;
			break;
		case L'0':
		case L'1':
		case L'2':
		case L'3':
		case L'4':
		case L'5':
		case L'6':
		case L'7':
		case L'8':
		case L'9':
			if(blk_count || dot_count > 1)
				return 0;
			break;
		case L' ':
		case L'\t':
			if(dot_count > 1)
				return 0;
			blk_count ++;
			break;
		default:
			return 0;
		}
		token ++;
	}
	return 1;
}

const schar_t* a_xsnskip(const schar_t* sz, int n)
{
	while (n-- && (*sz == ' ' || *sz == '\t'))
		sz++;

	return sz;
}

const wchar_t* w_xsnskip(const wchar_t* sz, int n)
{
	while (n-- && (*sz == L' ' || *sz == L'\t'))
		sz++;

	return sz;
}

const schar_t* a_xsskip(const schar_t* sz)
{
	return a_xsnskip(sz, a_xslen(sz));
}

const wchar_t* w_xsskip(const wchar_t* sz)
{
	return w_xsnskip(sz, w_xslen(sz));
}

short a_xsntos(const schar_t* sz, int n)
{
	bool_t sign = 0;
	short num = 0;
	int len = 0;
	schar_t* token = NULL;

	if (!n)
		return 0;

	token = (schar_t*)a_xsnskip(sz, n);
	n -= (int)(token - sz);

	if (*token == '-')
	{
		token++;
		sign = 1;
		n--;
	}
	else if (*token == '+')
	{
		token++;
		n--;
	}

	while (*token >= '0' && *token <= '9' && n--)
	{
		num *= 10;
		num += (*token - '0');
		token++;
	}

	return (sign) ? -num : num;
}

short w_xsntos(const wchar_t* sz, int n)
{
	bool_t sign = 0;
	short num = 0;
	int len = 0;
	wchar_t* token = NULL;

	if (!n)
		return 0;

	token = (wchar_t*)w_xsnskip(sz, n);
	n -= (int)(token - sz);

	if (*token == L'-')
	{
		token++;
		sign = 1;
		n--;
	}
	else if (*token == L'+')
	{
		token++;
		n--;
	}

	while (*token >= L'0' && *token <= L'9' && n--)
	{
		num *= 10;
		num += (*token - L'0');
		token++;
	}

	return (sign) ? -num : num;
}

short a_xstos(const schar_t* sz)
{
	return a_xsntos(sz, a_xslen(sz));
}

short w_xstos(const wchar_t* sz)
{
	return w_xsntos(sz, w_xslen(sz));
}

int a_stoxs(short s, schar_t* buf, int n)
{
	int sign = 0;
	int len = 0;

	if (s < 0 && n)
	{
		sign = 1;
		s = 0 - s;
		if (buf)
		{
			buf[len] = '-';
		}
		len++;
	}

	while (len < n)
	{
		if (buf)
		{
			buf[len] = s % 10 + '0';
		}

		len++;
		s /= 10;

		if (!s)
			break;
	}

	if (buf)
	{
		if (sign)
			a_xsnrev(buf + 1, len - 1);
		else
			a_xsnrev(buf, len);

		buf[len] = '\0';
	}

	return len;
}

int a_ustoxs(unsigned short s, schar_t* buf, int n)
{
	int len = 0;

	while (len < n)
	{
		if (buf)
		{
			buf[len] = s % 10 + '0';
		}
		len++;
		s /= 10;

		if (!s)
			break;
	}

	if (buf)
	{
		a_xsnrev(buf,len);
		buf[len] = '\0';
	}

	return len;
}

int w_stoxs(short s, wchar_t* buf, int n)
{
	int sign = 0;
	int len = 0;

	if (s < 0 && n)
	{
		sign = 1;
		s = 0 - s;
		if (buf)
		{
			buf[len] = L'-';
		}
		len++;
	}

	while (len < n)
	{
		if (buf)
		{
			buf[len] = s % 10 + L'0';
		}

		len++;
		s /= 10;

		if (!s)
			break;
	}

	if (buf)
	{
		if (sign)
			w_xsnrev(buf + 1, len - 1);
		else
			w_xsnrev(buf, len);

		buf[len] = L'\0';
	}

	return len;
}

int w_ustoxs(unsigned short s, wchar_t* buf, int n)
{
	int len = 0;

	while (len < n)
	{
		if (buf)
		{
			buf[len] = s % 10 + L'0';
		}

		len++;
		s /= 10;

		if (!s)
			break;
	}

	if (buf)
	{
		w_xsnrev(buf, len);
		buf[len] = L'\0';
	}

	return len;
}

long a_xsntol(const schar_t* sz, int n)
{
	bool_t sign = 0;
	long num = 0;
	int len = 0;
	schar_t* token = NULL;

	if (!n)
		return 0;

	token = (schar_t*)a_xsnskip(sz, n);
	n -= (int)(token - sz);

	if (*token == '-')
	{
		token++;
		sign = 1;
		n--;
	}
	else if (*token == '+')
	{
		token++;
		n--;
	}

	while (*token >= '0' && *token <= '9' && n--)
	{
		num *= 10;
		num += (*token - '0');
		token++;
	}

	return (sign) ? -num : num;
}

long w_xsntol(const wchar_t* sz, int n)
{
	bool_t sign = 0;
	long num = 0;
	int len = 0;
	wchar_t* token = NULL;

	if (!n)
		return 0;

	token = (wchar_t*)w_xsnskip(sz, n);
	n -= (int)(token - sz);

	if (*token == L'-')
	{
		token++;
		sign = 1;
		n--;
	}
	else if (*token == L'+')
	{
		token++;
		n--;
	}

	while (*token >= L'0' && *token <= L'9' && n--)
	{
		num *= 10;
		num += (*token - L'0');
		token++;
	}

	return (sign) ? -num : num;
}

long a_xstol(const schar_t* sz)
{
	return a_xsntol(sz, a_xslen(sz));
}

long w_xstol(const wchar_t* sz)
{
	return w_xsntol(sz, w_xslen(sz));
}

unsigned long a_hexntol(const schar_t* token, int n)
{
	unsigned long k = 0;
	long c = 0;
	int pos = 0;

	if (!n)
		return 0;

	if (n > 1 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X'))
		pos += 2;

	while (pos < n)
	{
		k *= 16;

		if (token[pos] >= 'a' && token[pos] <= 'z')
			c = (token[pos] - 'a') + 10;
		else if (token[pos] >= 'A' && token[pos] <= 'Z')
			c = (token[pos] - 'A') + 10;
		else if (token[pos] >= '0' && token[pos] <= '9')
			c = (token[pos] - '0');
		else if (token[pos] == '\0')
			break;
		else
			return 0;

		k += c;

		pos++;
	}

	return k;
}

unsigned long w_hexntol(const wchar_t* token, int n)
{
	unsigned long k = 0;
	long c = 0;
	int pos = 0;

	if (!n)
		return 0;

	if (n > 1 && token[0] == L'0' && (token[1] == L'x' || token[1] == L'X'))
		pos += 2;

	while (pos < n)
	{
		k *= 16;

		if (token[pos] >= L'a' && token[pos] <= L'z')
			c = (token[pos] - L'a') + 10;
		else if (token[pos] >= L'A' && token[pos] <= L'Z')
			c = (token[pos] - L'A') + 10;
		else if (token[pos] >= L'0' && token[pos] <= L'9')
			c = (token[pos] - L'0');
		else if (token[pos] == '\0')
			break;
		else
			return 0;

		k += c;

		pos++;
	}

	return k;
}

unsigned long a_hextol(const schar_t* sz)
{
	return a_hexntol(sz, a_xslen(sz));
}

unsigned long w_hextol(const wchar_t* sz)
{
	return w_hexntol(sz, w_xslen(sz));
}

int a_ltoxs(long s, schar_t* buf, int n)
{
	int sign = 0;
	int len = 0;

	if (s < 0 && n)
	{
		sign = 1;
		s = 0 - s;
		if (buf)
		{
			buf[len] = '-';
		}
		len++;
	}

	while (len < n)
	{
		if (buf)
		{
			buf[len] = s % 10 + '0';
		}
		len++;
		s /= 10;
		if (!s)
			break;
	}

	if (buf)
	{
		if (sign)
			a_xsnrev(buf + 1, len - 1);
		else
			a_xsnrev(buf, len);

		buf[len] = '\0';
	}

	return len;
}

int a_ultoxs(unsigned long s, schar_t* buf, int n)
{
	int len = 0;

	while (len < n)
	{
		if (buf)
		{
			buf[len] = s % 10 + '0';
		}
		len++;
		s /= 10;
		if (!s)
			break;
	}

	if (buf)
	{
		a_xsnrev(buf, len);
		buf[len] = '\0';
	}

	return len;
}

int w_ltoxs(long s, wchar_t* buf, int n)
{
	int sign = 0;
	int len = 0;

	if (s < 0 && n)
	{
		sign = 1;
		s = 0 - s;
		if (buf)
		{
			buf[len] = L'-';
		}
		len++;
	}

	while (len < n)
	{
		if (buf)
		{
			buf[len] = s % 10 + L'0';
		}
		len++;
		s /= 10;
		if (!s)
			break;
	}

	if (buf)
	{
		if (sign)
			w_xsnrev(buf + 1, len - 1);
		else
			w_xsnrev(buf, len);

		buf[len] = L'\0';
	}

	return len;
}

int w_ultoxs(unsigned long s, wchar_t* buf, int n)
{
	int len = 0;

	while (len < n)
	{
		if (buf)
		{
			buf[len] = s % 10 + L'0';
		}
		len++;
		s /= 10;
		if (!s)
			break;
	}

	if (buf)
	{
		w_xsnrev(buf, len);
		buf[len] = L'\0';
	}

	return len;
}

int a_ltohex(unsigned long s, schar_t type, schar_t* buf, int n)
{
	int len = 0;
	int us;

	while (len < n)
	{
		us = s % 16;
		if (type == 'x')
		{
			if (buf)
			{
				buf[len] = (us < 10) ? (us + 48) : (us + 87);
			}
		}
		else
		{
			if (buf)
			{
				buf[len] = (us < 10) ? (us + 48) : (us + 55);
			}
		}

		s /= 16;
		len++;

		if (!s && !(len % 2))
			break;
	}

	if (buf)
	{
		a_xsnrev(buf,len);
		buf[len] = '\0';
	}

	return len;
}

int w_ltohex(unsigned long s, wchar_t type, wchar_t* buf, int n)
{
	int len = 0;
	int us;

	while (len < n)
	{
		us = s % 16;
		if (type == L'x')
		{
			if (buf)
			{
				buf[len] = (us < 10) ? (us + 48) : (us + 87);
			}
		}
		else
		{
			if (buf)
			{
				buf[len] = (us < 10) ? (us + 48) : (us + 55);
			}
		}

		s /= 16;
		len++;

		if (!s && !(len % 2))
			break;
	}

	if (buf)
	{
		w_xsnrev(buf, len);
		buf[len] = L'\0';
	}

	return len;
}

float a_xsntof(const schar_t* sz, int n)
{
	bool_t sign = 0;
	int prec = MAX_FLOAT_DIGI;
	int mul = 1;
	long num = 0;
	long dec = 0; 
	float f = 0;
	schar_t* token;

	if (!n)
		return 0;

	token = (schar_t*)a_xsnskip(sz, n);
	n -= (int)(token - sz);

	if(*token == '-')
	{
		token ++;
		sign = 1;
		n--;
	}
	else if (*token == '+')
	{
		token++;
		n--;
	}

	if(*token != '.')
	{
		while (*token >= '0' && *token <= '9' && n--)
		{
			num *= 10;
			num += (*token - '0');

			token++;
		}
	}

	if(*token == '.')
	{
		token ++; 
		n--;
		while(*token >= '0' && *token <= '9' && prec && n--)
		{
			dec *= 10;
			dec += (*token - '0');
			token ++;
			prec --;
			mul *= 10;
		}
	}

	f = (float)num + (float)dec / mul;

	return (sign)? -f : f;
}


float w_xsntof(const wchar_t* sz, int n)
{
	int sign = 0;
	int prec = MAX_FLOAT_DIGI;
	int mul = 1;
	long num = 0;
	long dec = 0; 
	float f = 0;
	wchar_t* token;

	if (!n)
		return 0;

	token = (wchar_t*)w_xsnskip(sz, n);
	n -= (int)(token - sz);

	if(*token == L'-')
	{
		token ++;
		sign = 1;
		n--;
	}
	else if (*token == L'+')
	{
		token++;
		n--;
	}

	if(*token != L'.')
	{
		while (*token >= L'0' && *token <= L'9' && n--)
		{
			num *= 10;
			num += (*token - L'0');

			token++;
		}
	}

	if(*token == L'.')
	{
		token ++; 
		n--;
		while(*token >= L'0' && *token <= L'9' && prec && n--)
		{
			dec *= 10;
			dec += (*token - L'0');
			token ++;
			prec --;
			mul *= 10;
		}
	}

	f = (float)num + (float)dec / mul;

	return (sign)? -f : f;
}

float a_xstof(const schar_t* sz)
{
	return a_xsntof(sz, a_xslen(sz));
}

float w_xstof(const wchar_t* sz)
{
	return w_xsntof(sz, w_xslen(sz));
}

int a_ftoxs(float f, schar_t* buf, int n)
{
	int sign = 0;
	int prec = MAX_FLOAT_DIGI;
	int dot,len = 0;
	unsigned long xi;
	int dig;
	float inc = 5.0;

	if (f < 0 && n)
	{
		sign = 1;
		f = 0 - f;
		if (buf)
		{
			buf[len] = '-';
		}
		len++;
	}

	dig = prec;
	do{
		inc /= 10;
	} while (dig--);

	f += inc;

	xi = (unsigned long)(f);
	f -= (float)xi;

	while (len < n)
	{
		if (buf)
		{
			buf[len] = xi % 10 + '0';
		}
		len++;
		xi /= 10;
		if (!xi)
			break;
	}

	if (buf)
	{
		if (sign)
			a_xsnrev(buf + 1, len - 1);
		else
			a_xsnrev(buf,len);

		buf[len] = '\0';
	}

	if (len >= n)
		return len;

	if (buf)
	{
		buf[len] = '.';
	}
	len++;

	dot = len;

	dig = prec;
	while (dig--)
		f *= 10.0;

	xi = (unsigned long)(f);

	dig = prec;
	while (dig && xi % 10 == 0)
	{
		xi /= 10;
		dig--;
	}

	if (!dig)
	{
		if (buf)
		{
			buf[len] = '\0';
		}
		len--;
	}

	while (dig-- && len < n)
	{
		if (buf)
		{
			buf[len] = xi % 10 + '0';
		}
		len++;
		xi /= 10;
	}

	if (buf)
	{
		a_xsnrev(buf + dot, len - dot);
		buf[len] = '\0';
	}

	return len;
}

int w_ftoxs(float f, wchar_t* buf, int n)
{
	int sign = 0;
	int prec = MAX_FLOAT_DIGI;
	int dot, len = 0;
	unsigned long xi;
	int dig;
	float inc = 5.0;

	if (f < 0 && n)
	{
		sign = 1;
		f = 0 - f;
		if (buf)
		{
			buf[len] = L'-';
		}
		len++;
	}

	dig = prec;
	do{
		inc /= 10;
	} while (dig--);

	f += inc;

	xi = (unsigned long)(f);
	f -= (float)xi;

	while (len < n)
	{
		if (buf)
		{
			buf[len] = xi % 10 + L'0';
		}
		len++;
		xi /= 10;
		if (!xi)
			break;
	}

	if (buf)
	{
		if (sign)
			w_xsnrev(buf + 1, len - 1);
		else
			w_xsnrev(buf,len);

		buf[len] = L'\0';
	}

	if (len >= n)
		return len;

	if (buf)
	{
		buf[len] = L'.';
	}
	len++;

	dot = len;

	dig = prec;
	while (dig--)
		f *= 10.0;

	xi = (unsigned long)(f);

	dig = prec;
	while (dig && xi % 10 == 0)
	{
		xi /= 10;
		dig--;
	}

	if (!dig)
	{
		if (buf)
		{
			buf[len] = L'\0';
		}
		len--;
	}

	while (dig--)
	{
		if (buf)
		{
			buf[len] = xi % 10 + L'0';
		}
		len++;
		xi /= 10;
	}

	if (buf)
	{
		w_xsnrev(buf + dot, len - dot);
		buf[len] = L'\0';
	}

	return len;
}

double a_xsntonum_dig(const schar_t* sz, int n, int prec)
{
	bool_t sign = 0;
	int mul = 1;
	long num = 0;
	long dec = 0;
	double f = 0;
	schar_t* token;

	if (!n)
		return 0;

	token = (schar_t*)a_xsnskip(sz, n);
	n -= (int)(token - sz);

	if (*token == '-')
	{
		token++;
		sign = 1;
		n--;
	}
	else if (*token == '+')
	{
		token++;
		n--;
	}
	
	if (*token != '.')
	{
		while (*token >= '0' && *token <= '9' && n--)
		{
			num *= 10;
			num += (*token - '0');
			token++;
		}
	}

	if (*token == '.')
	{
		token++;
		n--;
		while (*token >= '0' && *token <= '9' && prec && n--)
		{
			dec *= 10;
			dec += (*token - '0');
			token++;
			prec--;
			mul *= 10;
		}	
	}

	f = (double)num + (double)dec / mul;

	return (sign) ? -f : f;
}

double w_xsntonum_dig(const wchar_t* sz, int n, int prec)
{
	int sign = 0;
	int mul = 1;
	long num = 0;
	long dec = 0;
	double f = 0;
	wchar_t* token;

	if (!n)
		return 0;

	token = (wchar_t*)w_xsnskip(sz, n);
	n -= (int)(token - sz);

	if (*token == L'-')
	{
		token++;
		sign = 1;
		n--;
	}
	else if (*token == L'+')
	{
		token++;
		n--;
	}

	if (*token != L'.')
	{
		while (*token >= L'0' && *token <= L'9' && n--)
		{
			num *= 10;
			num += (*token - L'0');
			token++;
		}
	}

	if (*token == L'.')
	{
		token++;
		n--;
		while (*token >= L'0' && *token <= L'9' && prec && n--)
		{
			dec *= 10;
			dec += (*token - L'0');
			token++;
			prec--;
			mul *= 10;
		}
	}

	f = (double)num + (double)dec / mul;

	return (sign) ? -f : f;
}

double a_xstonum_dig(const schar_t* sz, int digi)
{
	return a_xsntonum_dig(sz, a_xslen(sz),digi);
}

double w_xstonum_dig(const wchar_t* sz,int digi)
{
	return w_xsntonum_dig(sz, w_xslen(sz),digi);
}

double a_xsntonum(const schar_t* sz, int n)
{
	return a_xsntonum_dig(sz, n, MAX_DOUBLE_DIGI);
}

double w_xsntonum(const wchar_t* sz, int n)
{
	return w_xsntonum_dig(sz, n, MAX_DOUBLE_DIGI);
}

double a_xstonum(const schar_t* sz)
{
	return a_xsntonum_dig(sz, a_xslen(sz), MAX_DOUBLE_DIGI);
}

double w_xstonum(const wchar_t* sz)
{
	return w_xsntonum_dig(sz, w_xslen(sz), MAX_DOUBLE_DIGI);
}

int a_numtoxs_dig(double f, int prec, schar_t* buf, int n)
{
	int sign = 0;
	int dot, len = 0;
	unsigned long xi;
	int dig;
	double inc = 5.0;

	if (!prec)
		prec = MAX_DOUBLE_DIGI;

	if (f < 0 && n)
	{
		sign = 1;
		f = 0 - f;
		if (buf)
		{
			buf[len] = '-';
		}
		len++;
	}

	dig = prec;
	do{
		inc /= 10;
	} while (dig--);

	f += inc;

	xi = (unsigned long)(f);
	f -= (double)xi;

	while (len < n)
	{
		if (buf)
		{
			buf[len] = xi % 10 + '0';
		}
		len++;
		xi /= 10;
		if (!xi)
			break;
	}

	if (buf)
	{
		if (sign)
			a_xsnrev(buf + 1, len - 1);
		else
			a_xsnrev(buf, len);

		buf[len] = '\0';
	}

	if (len >= n)
		return len;

	if (buf)
	{
		buf[len] = '.';
	}
	len++;

	dot = len;

	dig = prec;
	while (dig--)
		f *= 10.0;

	xi = (unsigned long)(f);

	dig = prec;
	while (dig && xi % 10 == 0)
	{
		xi /= 10;
		dig--;
	}

	if (!dig)
	{
		if (buf)
		{
			buf[len] = '\0';
		}
		len--;
	}
	while (dig-- && len < n)
	{
		if (buf)
		{
			buf[len] = xi % 10 + '0';
		}
		len++;
		xi /= 10;
	}

	if (buf)
	{
		a_xsnrev(buf + dot, len - dot);
		buf[len] = '\0';
	}

	return len;
}

int w_numtoxs_dig(double f, int prec, wchar_t* buf, int n)
{
	int sign = 0;
	int dot, len = 0;
	unsigned long xi;
	int dig;
	double inc = 5.0;

	if (!prec)
		prec = MAX_DOUBLE_DIGI;

	if (f < 0 && n)
	{
		sign = 1;
		f = 0 - f;
		if (buf)
		{
			buf[len] = L'-';
		}
		len++;
	}

	dig = prec;
	do{
		inc /= 10;
	} while (dig--);

	f += inc;

	xi = (unsigned long)(f);
	f -= (double)xi;

	while (len < n)
	{
		if (buf)
		{
			buf[len] = xi % 10 + L'0';
		}
		len++;
		xi /= 10;
		if (!xi)
			break;
	}

	if (buf)
	{
		if (sign)
			w_xsnrev(buf + 1, len - 1);
		else
			w_xsnrev(buf, len);

		buf[len] = L'\0';
	}

	if (len >= n)
		return len;

	if (buf)
	{
		buf[len] = L'.';
	}
	len++;

	dot = len;

	dig = prec;
	while (dig--)
		f *= 10.0;

	xi = (unsigned long)(f);

	dig = prec;
	while (dig && xi % 10 == 0)
	{
		xi /= 10;
		dig--;
	}

	if (!dig)
	{
		if (buf)
		{
			buf[len] = L'\0';
		}
		len--;
	}
	while (dig-- && len < n)
	{
		if (buf)
		{
			buf[len] = xi % 10 + L'0';
		}
		len++;
		xi /= 10;
	}

	if (buf)
	{
		w_xsnrev(buf + dot, len - dot);
		buf[len] = L'\0';
	}

	return len;
}

int a_numtoxs(double f, schar_t* buf, int n)
{
	return a_numtoxs_dig(f, MAX_DOUBLE_DIGI, buf, n);
}

int w_numtoxs(double f, wchar_t* buf, int n)
{
	return w_numtoxs_dig(f, MAX_DOUBLE_DIGI, buf, n);
}

long long a_xsntoll(const schar_t* sz, int len)
{
	long long li = 0;
	int pos;
	schar_t* token;
	bool_t sign = 0;

	if (!sz || !len)
		return 0;

	token = (schar_t*)a_xsnskip(sz, len);
	len -= (int)(token - sz);

	if (*token == '-')
	{
		token++;
		sign = 1;
		len--;
	}
	else if (*token == '+')
	{
		token++;
		len--;
	}

	pos = 0;
	while (*token >= '0' && *token <= '9' && len--)
	{
		li *= 10;
		li += sz[pos++] - '0';
	}

	return (sign) ? (0 - li) : li;
}

long long w_xsntoll(const wchar_t* sz, int len)
{
	long long li = 0;
	int pos;
	wchar_t* token;
	bool_t sign = 0;

	if (!sz || !len)
		return 0;

	token = (wchar_t*)w_xsnskip(sz, len);
	len -= (int)(token - sz);

	if (*token == L'-')
	{
		token++;
		sign = 1;
		len--;
	}
	else if (*token == L'+')
	{
		token++;
		len--;
	}

	pos = 0;
	while (*token >= L'0' && *token <= L'9' && len--)
	{
		li *= 10;
		li += sz[pos++] - L'0';
	}

	return (sign) ? (0 - li) : li;
}

long long a_xstoll(const schar_t* sz)
{
	return a_xsntoll(sz, a_xslen(sz));
}

long long w_xstoll(const wchar_t* sz)
{
	return w_xsntoll(sz, w_xslen(sz));
}

int a_lltoxs(long long ll, schar_t* buf, int n)
{
	int pos = 0;
	bool_t sign = 0;

	if (ll < 0 && n)
	{
		sign = 1;
		ll = 0 - ll;
		if (buf)
		{
			buf[pos] = '-';
		}
		pos++;
	}

	while (pos < n)
	{
		if (buf)
		{
			buf[pos] = (int)(ll % 10) + '0';
		}
		ll /= 10;
		pos++;

		if (!ll)
			break;
	}

	if (buf)
	{
		if (sign)
			a_xsnrev(buf + 1, pos - 1);
		else
			a_xsnrev(buf, pos);

		buf[pos] = '\0';
	}

	return pos;
}

int w_lltoxs(long long ll, wchar_t* buf, int n)
{
	int pos = 0;

	bool_t sign = 0;

	if (ll < 0 && n)
	{
		sign = 1;
		ll = 0 - ll;
		if (buf)
		{
			buf[pos] = L'-';
		}
		pos++;
	}

	while (pos < n)
	{
		if (buf)
		{
			buf[pos] = (int)(ll % 10) + L'0';
		}
		ll /= 10;
		pos++;

		if (!ll)
			break;
	}

	if (buf)
	{
		if (sign)
			w_xsnrev(buf + 1, pos - 1);
		else
			w_xsnrev(buf, pos);

		buf[pos] = L'\0';
	}

	return pos;
}

void a_xsntriml(schar_t* sz, int n)
{
	schar_t* token;
	int len;

	if (!sz || !n)
		return;

	len = n;
	token = sz;
	while ((*token == ' ' || *token == '\t') && len--)
		token++;

	if(token == sz)
		return;
	
	len = n;
	while(*token && len--)
		*(sz++) = *(token ++);

	*sz = '\0';
}

void w_xsntriml(wchar_t* sz, int n)
{
	wchar_t* token;
	int len;

	if (!sz && !n)
		return;

	len = n;
	token = sz;
	while((*token == L' ' || *token == L'\t') && len--)
		token ++;

	if(token == sz)
		return;
	
	len = n;
	while(*token && len--)
		*(sz++) = *(token ++);

	*sz = L'\0';
}

void a_xstriml(schar_t* sz)
{
	a_xsntriml(sz, a_xslen(sz));
}

void w_xstriml(wchar_t* sz)
{
	w_xsntriml(sz, w_xslen(sz));
}

void a_xsntrimr(schar_t* sz, int n)
{
	int len = 0;

	if (!n)
		return;

	sz += n;

	while ((*sz == ' ' || *sz == '\t' || *sz == '\0') && n--)
		sz--;

	*(sz + 1) = '\0';
}

void w_xsntrimr(wchar_t* sz, int n)
{
	int len = 0;

	if (!n)
		return;

	sz += n;

	while ((*sz == L' ' || *sz == L'\t' || *sz == L'\0') && n--)
		sz--;

	*(sz + 1) = L'\0';
}

void a_xstrimr(schar_t* sz)
{
	a_xsntrimr(sz, a_xslen(sz));
}

void w_xstrimr(wchar_t* sz)
{
	w_xsntrimr(sz, w_xslen(sz));
}

void a_xsntrim(schar_t* sz, int n)
{
	a_xsntrimr(sz, n);
	a_xsntriml(sz, n);
}

void w_xsntrim(wchar_t* sz, int n)
{
	w_xsntrimr(sz, n);
	w_xsntriml(sz, n);
}

void a_xstrim(schar_t* sz)
{
	a_xsntrim(sz, a_xslen(sz));
}

void w_xstrim(wchar_t* sz)
{
	w_xsntrim(sz, w_xslen(sz));
}

void a_xsndel(schar_t* str,int start,int len)
{
	schar_t* token;

	if(!str || !len)
		return;

	str += start;
	token = str + len;
	while(*token)
		*(str ++) = *(token++);

	*str = '\0';
}

void w_xsndel(wchar_t* str,int start,int len)
{
	wchar_t* token;

	if (!str || !len)
		return;

	str += start;
	token = str + len;
	while (*token)
		*(str++) = *(token++);

	*str = L'\0';
}

void a_xsdel(schar_t* str, int start)
{
	*(str + start) = '\0';
}

void w_xsdel(wchar_t* str, int start)
{
	*(str + start) = L'\0';
}

void a_xsnins(schar_t* str, int index, const schar_t* sub, int len)
{
	schar_t* token;
	int size;

	if (!str && !sub)
		return;

	size = a_xslen(str);
	str += size;

	token = str + len;
	*token = '\0';

	size -= index;
	while (size--)
	{
		*(--token) = *(--str);
	}

	str += len;
	while (len--)
	{
		*(--str) = *(sub + len);
	}
}

void w_xsnins(wchar_t* str, int index, const wchar_t* sub, int len)
{
	wchar_t* token;
	int size;

	if (!str && !sub)
		return;

	size = w_xslen(str);
	str += size;

	token = str + len;
	*token = L'\0';

	size -= index;
	while (size --)
	{
		*(--token) = *(--str);
	}
	
	str += len;
	while (len --)
	{
		*(--str) = *(sub + len);
	}
}

void a_xsins(schar_t* str, int index, const schar_t* sub)
{
	a_xsnins(str, index, sub, a_xslen(sub));
}

void w_xsins(wchar_t* str, int index, const wchar_t* sub)
{
	w_xsnins(str, index, sub, w_xslen(sub));
}

void a_xsnset(schar_t* str, int index, const schar_t* sub, int len)
{
	if (!str && !sub)
		return;

	str += index;
	while (len--)
	{
		*(str++) = *(sub++);
	}
}

void w_xsnset(wchar_t* str, int index, const wchar_t* sub, int len)
{
	if (!str && !sub)
		return;

	str += index;
	while (len--)
	{
		*(str++) = *(sub++);
	}
}

void a_xsset(schar_t* str, int index, const schar_t* sub)
{
	a_xsnset(str, index, sub, a_xslen(sub));
}

void w_xsset(wchar_t* str, int index, const wchar_t* sub)
{
	w_xsnset(str, index, sub, w_xslen(sub));
}

int a_xslen(const schar_t* str)
{
	schar_t* token;

	if(!str)
		return 0;

	token = (schar_t*)str;
	while(*token)
		token ++;

	return (int)(token - str);
}

int w_xslen(const wchar_t* str)
{
	wchar_t* token;

	if(!str)
		return 0;

	token = (wchar_t*)str;
	while(*token)
		token ++;

	return (int)(token - str);
}

void a_xsncpy(schar_t* dest,const schar_t* src,int n)
{
	if(!dest)
		return;

	if(!src)
	{
		*dest = '\0';
		return;
	}

	while(*src && n--)
		*(dest++) = *(src++);

	*dest = '\0';
}

void w_xsncpy(wchar_t* dest,const wchar_t* src,int n)
{
	if(!dest)
		return;

	if(!src)
	{
		*dest = L'\0';
		return;
	}

	while(*src && n--)
		*(dest++) = *(src++);

	*dest = L'\0';
}

void a_xscpy(schar_t* dest, const schar_t* src)
{
	a_xsncpy(dest, src, a_xslen(src));
}

void w_xscpy(wchar_t* dest, const wchar_t* src)
{
	w_xsncpy(dest, src, w_xslen(src));
}

void a_xsncat(schar_t* dest,const schar_t* src,int n)
{
	if(!dest || !src)
		return;

	while (*dest)
	{
		dest++;
	}

	while(*src && n--)
		*(dest++) = *(src++);

	*dest = '\0';
}

void w_xsncat(wchar_t* dest,const wchar_t* src,int n)
{
	if (!dest || !src)
		return;

	while (*dest)
	{
		dest++;
	}

	while (*src && n--)
		*(dest++) = *(src++);

	*dest = L'\0';
}

void a_xscat(schar_t* dest, const schar_t* src)
{
	a_xsncat(dest, src, a_xslen(src));
}

void w_xscat(wchar_t* dest, const wchar_t* src)
{
	w_xsncat(dest, src, w_xslen(src));
}

void a_xsnmid(schar_t* dest,const schar_t* src,int from,int len)
{
	if(!dest || !src || from < 0)
		return;

	len -= from;
	if (len < 0)
		return;

	while(*(src + from) && len--)
	{
		*dest = *(src + from);
		from ++;
	}

	*dest = '\0';
}

void w_xsnmid(wchar_t* dest,const wchar_t* src,int from,int len)
{
	if (!dest || !src)
		return;

	len -= from;
	if (len < 0)
		return;

	while (*(src + from) && len--)
	{
		*dest = *(src + from);
		from++;
	}

	*dest = L'\0';
}

void a_xsmid(schar_t* dest, const schar_t* src, int from)
{
	a_xsnmid(dest, src, from, a_xslen(src));
}

void w_xsmid(wchar_t* dest, const wchar_t* src, int from)
{
	w_xsnmid(dest, src, from, w_xslen(src));
}

int a_xscmp(const schar_t* dest,const schar_t* src)
{
	int rt = 0 ;

	if (a_is_null(dest) && a_is_null(src))
		return 0;
	else if (a_is_null(dest))
		return -1;
	else if (a_is_null(src))
		return 1;

	while(*dest || *src)
	{
		if(rt = *(dest++) - *(src++))
			break;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

int w_xscmp(const wchar_t* dest,const wchar_t* src)
{
	int rt = 0 ;

	if (w_is_null(dest) && w_is_null(src))
		return 0;
	else if (w_is_null(dest))
		return -1;
	else if (w_is_null(src))
		return 1;

	while(*dest || *src)
	{
		if(rt = *(dest++) - *(src++))
			break;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

int a_xsncmp(const schar_t* dest,const schar_t* src,int n)
{
	int rt = 0 ;

	if (a_is_null(dest) && a_is_null(src))
		return 0;
	else if (a_is_null(dest))
		return (n)? -1 : 0;
	else if (a_is_null(src))
		return (n)? 1 : 0;

	while(n && *src)
	{
		if(rt = *(dest++) - *(src++))
			break;
		n--;
	}

	if(rt < 0)
		return -1 ;
	else if(rt > 0)
		return 1 ;
	else
		return (n)? 1 : 0;
}

int w_xsncmp(const wchar_t* dest,const wchar_t* src,int n)
{
	int rt = 0 ;

	if (w_is_null(dest) && w_is_null(src))
		return 0;
	else if (w_is_null(dest))
		return (n) ? -1 : 0;
	else if (w_is_null(src))
		return (n) ? 1 : 0;

	while(n && *src)
	{
		if(rt = *(dest++) - *(src++))
			break;
		n--;
	}

	if (rt < 0)
		return -1;
	else if (rt > 0)
		return 1;
	else
		return (n) ? 1 : 0;
}

schar_t ach_upper(schar_t ch)
{
	if (ch >= 'a' && ch <= 'z')
		return ch + ('A' - 'a');
	else
		return ch;
}

schar_t ach_lower(schar_t ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return ch + ('a' - 'A');
	else
		return ch;
}

wchar_t wch_upper(wchar_t ch)
{
	if (ch >= L'a' && ch <= L'z')
		return ch + (L'A' - L'a');
	else
		return ch;
}

wchar_t wch_lower(wchar_t ch)
{
	if (ch >= L'A' && ch <= L'Z')
		return ch + (L'a' - L'A');
	else
		return ch;
}

int a_xsicmp(const schar_t* dest, const schar_t* src)
{
	int rt = 0;

	if (a_is_null(dest) && a_is_null(src))
		return 0;
	else if (a_is_null(dest))
		return -1;
	else if (a_is_null(src))
		return 1;

	while (*src || *dest)
	{
		if (rt = ach_upper(*dest) - ach_upper(*src))
			break;
		dest++;
		src++;
	}

	if (rt < 0)
		rt = -1;
	else if (rt > 0)
		rt = 1;

	return rt;
}

int w_xsicmp(const wchar_t* dest, const wchar_t* src)
{
	int rt = 0;

	if (w_is_null(dest) && w_is_null(src))
		return 0;
	else if (w_is_null(dest))
		return -1;
	else if (w_is_null(src))
		return 1;

	while (*src || *dest)
	{
		if (rt = wch_upper(*dest) - wch_upper(*src))
			break;
		dest++;
		src++;
	}

	if (rt < 0)
		rt = -1;
	else if (rt > 0)
		rt = 1;

	return rt;
}

int a_xsnicmp(const schar_t* dest, const schar_t* src, int n)
{
	int rt = 0;

	if (a_is_null(dest) && a_is_null(src))
		return 0;
	else if (a_is_null(dest))
		return (n)? -1 : 0;
	else if (a_is_null(src))
		return (n)? 1 : 0;

	while (n && *src)
	{
		if (rt = ach_upper(*dest) - ach_upper(*src))
			break;
		dest++;
		src++;
		n--;
	}

	if (rt < 0)
		return -1;
	else if (rt > 0)
		return 1;
	else
		return (n) ? 1 : 0;
}

int w_xsnicmp(const wchar_t* dest, const wchar_t* src, int n)
{
	int rt = 0;

	if (w_is_null(dest) && w_is_null(src))
		return 0;
	else if (w_is_null(dest))
		return (n) ? -1 : 0;
	else if (w_is_null(src))
		return (n) ? 1 : 0;

	while (n && *src)
	{
		if (rt = wch_upper(*dest) - wch_upper(*src))
			break;
		dest++;
		src++;
		n--;
	}

	if (rt < 0)
		return -1;
	else if (rt > 0)
		return 1;
	else
		return (n) ? 1 : 0;
}


void a_xsnupr(schar_t* sz, int n)
{
	if(!sz)
		return;

	while(*sz && n--)
	{
		if(*sz >= 'a' && *sz <= 'z') 
			*sz = *sz + ('A' - 'a'); 
		sz ++;
	}
}

void w_xsnupr(wchar_t* sz, int n)
{
	if(!sz)
		return;

	while(*sz && n--)
	{
		if(*sz >= L'a' && *sz <= L'z') 
			*sz = *sz + (L'A' - L'a'); 
		sz ++;
	}
}

void a_xsupr(schar_t* sz)
{
	a_xsnupr(sz, a_xslen(sz));
}

void w_xsupr(wchar_t* sz)
{
	w_xsnupr(sz, w_xslen(sz));
}

void a_xsnlwr(schar_t* sz, int n)
{
	if(!sz)
		return;

	while(*sz && n--)
	{
		if(*sz >= 'A' && *sz <= 'Z') 
			*sz = *sz + ('a' - 'A'); 
		sz ++;
	}
}

void w_xsnlwr(wchar_t* sz, int n)
{
	if(!sz)
		return;

	while(*sz && n--)
	{
		if(*sz >= L'A' && *sz <= L'Z') 
			*sz = *sz + (L'a' - L'A'); 
		sz ++;
	}
}

void a_xslwr(schar_t* sz)
{
	a_xsnlwr(sz, a_xslen(sz));
}

void w_xslwr(wchar_t* sz)
{
	w_xsnlwr(sz, w_xslen(sz));
}

void a_xsnrev(schar_t* str, int len)
{
	int i = 0;
	schar_t ch;

	len --;
	while (i < len)
	{
		ch = str[i];
		str[i] = str[len];
		str[len] = ch;

		i++;
		len--;
	}
}

void w_xsnrev(wchar_t* str, int len)
{
	int i = 0;
	wchar_t ch;

	len--;
	while (i < len)
	{
		ch = str[i];
		str[i] = str[len];
		str[len] = ch;

		i++;
		len--;
	}
}

void a_xsrev(schar_t* str)
{
	a_xsnrev(str, a_xslen(str));
}

void w_xsrev(wchar_t* str)
{
	w_xsnrev(str, w_xslen(str));
}

const schar_t* a_kmpnstr(const schar_t* str, int len, const schar_t* sub)
{
	int i, j, n_sub;
	int* kmp;

	if (a_is_null(str) || !len || a_is_null(sub))
		return NULL;

	n_sub = a_xslen(sub);

	if (n_sub > len)
		return NULL;

	kmp = (int*)xmem_alloc(n_sub * sizeof(int));

	kmp[0] = 0;
	kmp[1] = 0;

	i = 2;
	j = 0;
	for (i = 2; i < n_sub; i++)
	{
		while (1)
		{
			if (sub[i - 1] == sub[j])
			{
				j++;
				if (sub[i] == sub[j])
				{
					kmp[i] = kmp[j];
				}
				else
				{
					kmp[i] = j;
				}
				break;
			}
			else
			{
				if (j == 0)
				{
					kmp[i] = 0;
					break;
				}
				j = kmp[j];
			}
		}
	}

	j = 0;
	for (i = 0; i < len;)
	{
		if (str[i] == sub[j])
		{
			i++;
			j++;
		}
		else
		{
			j = kmp[j];
			if (j == 0)
				i++;
		}
		if (j == n_sub)
			break;
	}

	xmem_free(kmp);

	return (j == n_sub) ? str + (i - j) : NULL;
}

const wchar_t* w_kmpnstr(const wchar_t* str, int len, const wchar_t* sub)
{
	int i, j, n_sub;
	int* kmp;

	if (w_is_null(str) || !len || w_is_null(sub))
		return NULL;

	n_sub = w_xslen(sub);

	if (n_sub > len)
		return NULL;

	kmp = (int*)xmem_alloc(n_sub * sizeof(int));

	kmp[0] = 0;
	kmp[1] = 0;

	i = 2;
	j = 0;
	for (i = 2; i < n_sub; i++)
	{
		while (1)
		{
			if (sub[i - 1] == sub[j])
			{
				j++;
				if (sub[i] == sub[j])
				{
					kmp[i] = kmp[j];
				}
				else
				{
					kmp[i] = j;
				}
				break;
			}
			else
			{
				if (j == 0)
				{
					kmp[i] = 0;
					break;
				}
				j = kmp[j];
			}
		}
	}

	j = 0;
	for (i = 0; i < len;)
	{
		if (str[i] == sub[j])
		{
			i++;
			j++;
		}
		else
		{
			j = kmp[j];
			if (j == 0)
				i++;
		}
		if (j == n_sub)
			break;
	}

	xmem_free(kmp);

	return (j == n_sub) ? str + (i - j) : NULL;
}

const schar_t* a_kmpstr(const schar_t* str, const schar_t* sub)
{
	return a_kmpnstr(str, a_xslen(str), sub);
}

const wchar_t* w_kmpstr(const wchar_t* str, const wchar_t* sub)
{
	return w_kmpnstr(str, w_xslen(str), sub);
}

const schar_t* a_kmpnistr(const schar_t* str, int len, const schar_t* sub)
{
	int i, j, n_sub;
	int* kmp;

	if (a_is_null(str) || !len || a_is_null(sub))
		return NULL;

	n_sub = a_xslen(sub);

	if (n_sub > len)
		return NULL;

	kmp = (int*)xmem_alloc(n_sub * sizeof(int));

	kmp[0] = 0;
	kmp[1] = 0;

	i = 2;
	j = 0;
	for (i = 2; i < n_sub; i++)
	{
		while (1)
		{
			if (ach_upper(sub[i - 1]) == ach_upper(sub[j]))
			{
				j++;
				if (ach_upper(sub[i]) == ach_upper(sub[j]))
				{
					kmp[i] = kmp[j];
				}
				else
				{
					kmp[i] = j;
				}
				break;
			}
			else
			{
				if (j == 0)
				{
					kmp[i] = 0;
					break;
				}
				j = kmp[j];
			}
		}
	}

	j = 0;
	for (i = 0; i < len;)
	{
		if (ach_upper(str[i]) == ach_upper(sub[j]))
		{
			i++;
			j++;
		}
		else
		{
			j = kmp[j];
			if (j == 0)
				i++;
		}
		if (j == n_sub)
			break;
	}

	xmem_free(kmp);

	return (j == n_sub) ? str + (i - j) : NULL;
}

const wchar_t* w_kmpnistr(const wchar_t* str, int len, const wchar_t* sub)
{
	int i, j, n_sub;
	int* kmp;

	if (w_is_null(str) || !len || w_is_null(sub))
		return NULL;

	n_sub = w_xslen(sub);

	if (n_sub > len)
		return NULL;

	kmp = (int*)xmem_alloc(n_sub * sizeof(int));

	kmp[0] = 0;
	kmp[1] = 0;

	i = 2;
	j = 0;
	for (i = 2; i < n_sub; i++)
	{
		while (1)
		{
			if (wch_upper(sub[i - 1]) == wch_upper(sub[j]))
			{
				j++;
				if (wch_upper(sub[i]) == wch_upper(sub[j]))
				{
					kmp[i] = kmp[j];
				}
				else
				{
					kmp[i] = j;
				}
				break;
			}
			else
			{
				if (j == 0)
				{
					kmp[i] = 0;
					break;
				}
				j = kmp[j];
			}
		}
	}

	j = 0;
	for (i = 0; i < len;)
	{
		if (wch_upper(str[i]) == wch_upper(sub[j]))
		{
			i++;
			j++;
		}
		else
		{
			j = kmp[j];
			if (j == 0)
				i++;
		}
		if (j == n_sub)
			break;
	}

	xmem_free(kmp);

	return (j == n_sub) ? str + (i - j) : NULL;
}

const schar_t* a_kmpistr(const schar_t* str, const schar_t* sub)
{
	return a_kmpnistr(str, a_xslen(str), sub);
}

const wchar_t* w_kmpistr(const wchar_t* str, const wchar_t* sub)
{
	return w_kmpnistr(str, w_xslen(str), sub);
}

const schar_t* a_xsnstr(const schar_t* str,int len,const schar_t* sub)
{
	schar_t *tk = (schar_t *)str;
	schar_t *s1, *s2;

	if (a_is_null(sub))
		return((schar_t *)str);

	while(*tk && len)
	{
		s1 = tk;
		s2 = (schar_t *)sub;

		while (*s1 && *s2 && !(*s1-*s2) )
			s1++, s2++;

		if (!*s2)
			return tk;

		tk++;
		len--;
	}

	return NULL;
}

const wchar_t* w_xsnstr(const wchar_t* str,int len,const wchar_t* sub)
{
	wchar_t *tk = (wchar_t *)str;
	wchar_t *s1, *s2;

	if (w_is_null(sub))
		return((wchar_t *)str);

	while(*tk && len)
	{
		s1 = tk;
		s2 = (wchar_t *)sub;

		while (*s1 && *s2 && !(*s1-*s2) )
			s1++, s2++;

		if (!*s2)
			return tk;

		tk++;
		len--;
	}

	return NULL;
}

const schar_t* a_xsstr(const schar_t* str, const schar_t* sub)
{
	return a_xsnstr(str, a_xslen(str), sub);
}

const wchar_t* w_xsstr(const wchar_t* str, const wchar_t* sub)
{
	return w_xsnstr(str, w_xslen(str), sub);
}

const schar_t* a_xsnistr(const schar_t* str, int len, const schar_t* sub)
{
	schar_t *tk = (schar_t *)str;
	schar_t *s1, *s2;

	if (a_is_null(sub))
		return((schar_t *)str);

	while (*tk && len)
	{
		s1 = tk;
		s2 = (schar_t *)sub;

		while (*s1 && *s2 && !(ach_upper(*s1) - ach_upper(*s2)))
			s1++, s2++;

		if (!*s2)
			return tk;

		tk++;
		len--;
	}

	return NULL;
}


const wchar_t* w_xsnistr(const wchar_t* str, int len, const wchar_t* sub)
{
	wchar_t *tk = (wchar_t *)str;
	wchar_t *s1, *s2;

	if (w_is_null(sub))
		return((wchar_t *)str);

	while (*tk && len)
	{
		s1 = tk;
		s2 = (wchar_t *)sub;

		while (*s1 && *s2 && !(wch_upper(*s1) - wch_upper(*s2)))
			s1++, s2++;

		if (!*s2)
			return tk;

		tk++;
		len--;
	}

	return NULL;
}

const schar_t* a_xsistr(const schar_t* str, const schar_t* sub)
{
	return a_xsnistr(str, a_xslen(str), sub);
}

const wchar_t* w_xsistr(const wchar_t* str, const wchar_t* sub)
{
	return w_xsnistr(str, w_xslen(str), sub);
}

schar_t* a_xsnclone(const schar_t* str, int len)
{
	schar_t* token;

	if (a_is_null(str))
		return NULL;

	token = a_xsalloc(len + 1);

	xmem_copy((void*)token, (void*)str, len * sizeof(schar_t));
	return token;
}

wchar_t* w_xsnclone(const wchar_t* str, int len)
{
	wchar_t* token;

	if (w_is_null(str))
		return NULL;

	token = w_xsalloc(len + 1);

	xmem_copy((void*)token, (void*)str, len * sizeof(wchar_t));
	return token;
}

schar_t* a_xsclone(const schar_t* str)
{
	return a_xsnclone(str, a_xslen(str));
}

wchar_t* w_xsclone(const wchar_t* str)
{
	return w_xsnclone(str, w_xslen(str));
}

bool_t a_is_prefix(const schar_t* str, const schar_t* sub)
{
	if (a_is_null(sub))
		return 1;

	if (a_is_null(str))
		return 0;

	while (*str && *sub && *str == *sub)
	{
		str++;
		sub++;
	}

	return (*sub) ? 0 : 1;
}

bool_t w_is_prefix(const wchar_t* str, const wchar_t* sub)
{
	if (w_is_null(sub))
		return 1;

	if (w_is_null(str))
		return 0;

	while (*str && *sub && *str == *sub)
	{
		str++;
		sub++;
	}

	return (*sub) ? 0 : 1;
}

bool_t a_is_suffix(const schar_t* str, const schar_t* sub)
{
	int len_str,len_sub;

	if (a_is_null(sub))
		return 1;

	if (a_is_null(str))
		return 0;

	len_str = a_xslen(str);
	len_sub = a_xslen(sub);

	if (len_str < len_sub)
		return 0;

	str += (len_str - len_sub);
	while (*str && *sub && *str == *sub)
	{
		str++;
		sub++;
	}

	return (*sub) ? 0 : 1;
}

bool_t w_is_suffix(const wchar_t* str, const wchar_t* sub)
{
	int len_str, len_sub;

	if (w_is_null(sub))
		return 1;

	if (w_is_null(str))
		return 0;

	len_str = w_xslen(str);
	len_sub = w_xslen(sub);

	if (len_str < len_sub)
		return 0;

	str += (len_str - len_sub);
	while (*str && *sub && *str == *sub)
	{
		str++;
		sub++;
	}

	return (*sub) ? 0 : 1;
}

//%[flag] [width] [.precision] [{h | l | I64 | L}]type
//flags: -,+,' ',#,0
//width:
//precision
//type:c,C,d,i,o,u,x,X,e,E,f,g,s,S,T,t

typedef enum{
	XS_SKIP = 0,
	XS_FLAG = 1,
	XS_WIDTH = 2,
	XS_PREC = 3,
	XS_SIZE= 4,
	XS_TYPE = 5,
	XS_PROC = 6,
	XS_TERM = 7,
	XS_END = 8
}XF_STATUS;

typedef enum{
	XO_PAUSE = 0,
	XO_CONTINUE = 1
}XF_OPERA;

#define a_is_flag(ch)	((ch == '+' || ch == '#')? 1 : 0)
#define a_is_digit(ch)	((ch >= '0' && ch <= '9')? 1 : 0)
#define a_is_hex(ch)	(((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))? 1 : 0)
#define a_is_size(ch)	((*token == 'h' || *token == 'l')? 1 : 0)
#define a_is_type(ch)	((ch == 'c' || ch == 'd' || ch == 'u' || ch == 'x' || ch == 'X' || ch == 'f'|| ch == 's' || ch == 'S')? 1 : 0)

#define w_is_flag(ch)	((ch == L'+' || ch == L'#')? 1 : 0)
#define w_is_digit(ch)	((ch >= L'0' && ch <= L'9')? 1 : 0)
#define w_is_hex(ch)	(((ch >= L'0' && ch <= L'9') || (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z'))? 1 : 0)
#define w_is_size(ch)	((*token == L'h' || *token == L'l')? 1 : 0)
#define w_is_type(ch)	((ch == L'c' || ch == L'd' || ch == L'u' || ch == L'x' || ch == L'X' || ch == L'f'|| ch == L's' || ch == L'S')? 1 : 0)

int a_tk_printf(schar_t* buf,schar_t flag,int width,int prec,schar_t size,schar_t type,va_list* parg)
{
	schar_t ch;
	int len,pos;
	short s;
	unsigned short us;
	long i;
	long long li;
	unsigned long ui;
	double dbl;
	schar_t* sz;
	wchar_t* wz;

	switch(type)
	{
	case 'c':
		ch = va_arg(*parg,schar_t);
		if(buf)
		{
			*buf = ch;
			*(buf + 1) = '\0';
		}
		return 1;
	case 'd':
		if(size == 'h')
		{
			pos = 0;
			s = va_arg(*parg,short);
			if (flag == '+')
			{
				if (s < 0)
				{
					if (buf)
					{
						buf[pos] = '-';
					}
					s = 0 - s;
				}
				else
				{
					if (buf)
					{
						buf[pos] = '+';
					}
				}
				pos++;
			}
			else
			{
				if (s < 0)
				{
					if (buf)
					{
						buf[pos] = '-';
					}
					s = 0 - s;
					pos++;
				}
			}
			if (width)
			{
				len = width - a_stoxs(s, NULL, width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = '0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + a_stoxs(s, ((buf) ? buf + pos : NULL),width);
		}else if (size == 'l')
		{
			pos = 0;
			li = va_arg(*parg, long long);
			if (flag == '+')
			{
				if (li < 0)
				{
					if (buf)
					{
						buf[pos] = '-';
					}
					li = 0 - li;
				}
				else
				{
					if (buf)
					{
						buf[pos] = '+';
					}
				}
				pos++;
			}
			else
			{
				if (li < 0)
				{
					if (buf)
					{
						buf[pos] = '-';
					}
					li = 0 - li;
					pos++;
				}
			}
			if (width)
			{
				len = width - a_lltoxs(li, NULL, width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = '0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + a_lltoxs(li, ((buf) ? buf + pos : NULL), width);
		}
		else
		{
			pos = 0;
			i = va_arg(*parg,long);
			if (flag == '+')
			{
				if (i < 0)
				{
					if (buf)
					{
						buf[pos] = '-';
					}
					i = 0 - i;
				}
				else
				{
					if (buf)
					{
						buf[pos] = '+';
					}
				}
				pos++;
			}
			else
			{
				if (i < 0)
				{
					if (buf)
					{
						buf[pos] = '-';
					}
					i = 0 - i;
					pos++;
				}
			}
			if (width)
			{
				len = width - a_ltoxs(i, NULL,width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = '0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + a_ltoxs(i, ((buf) ? buf + pos : NULL),width);
		}
		break;
	case 'u':
		if (size == 'h')
		{
			pos = 0;
			us = va_arg(*parg, unsigned short);
			if (width)
			{
				len = width - a_ustoxs(us, NULL,width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = '0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + a_ustoxs(us, ((buf)? buf + pos : NULL),width);
		}
		else
		{
			pos = 0;
			ui = va_arg(*parg, unsigned long);
			if (width)
			{
				len = width - a_ultoxs(ui, NULL,width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = '0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + a_ultoxs(ui, ((buf) ? buf + pos : NULL),width);
		}
	case 'x':
	case 'X':
		ui = va_arg(*parg,unsigned long);
		pos = 0;
		if (flag == '#')
		{
			if (buf)
			{
				buf[0] = '0';
				buf[1] = type;
			}
			pos += 2;
		}
		if (width)
		{
			len = width - a_ltohex(ui, type, NULL,width);
			while (len > 0)
			{
				if (buf)
				{
					buf[pos] = '0';
				}
				pos++;
				len--;
			}
		}
		else
		{
			width = NUM_LEN;
		}
		return pos + a_ltohex(ui, type, ((buf) ? (buf + pos) : NULL),width);
	case 'f':
		dbl = va_arg(*parg, double);
		if (!width)
		{
			width = NUM_LEN;
		}
		if (!prec)
		{
			prec = MAX_DOUBLE_DIGI;
		}
		return a_numtoxs_dig(dbl, prec, buf, width);
	case 's':
		sz = va_arg(*parg,schar_t*);
		len = a_xslen(sz);

		if (!width)
			width = len;
		else
			width = (width < len) ? width : len;
		
		if (buf)
		{
			a_xsncpy(buf, sz, width);
		}

		return width;
	case 'S':
		wz = va_arg(*parg, wchar_t*);
		len = ucs_to_mbs(wz, -1, NULL, MAX_LONG);

		if (!width)
			width = len;
		else
			width = (width < len) ? width : len;

		if (buf)
		{
			ucs_to_mbs(wz, -1, buf, width);
			buf[width] = '\0';
		}

		return width;

	}

	return 0;
}


int w_tk_printf(wchar_t* buf,wchar_t flag,int width,int prec,wchar_t size,wchar_t type,va_list* parg)
{
	wchar_t ch;
	int len, pos;
	short s;
	unsigned short us;
	long i;
	long long li;
	unsigned long ui;
	double dbl;
	wchar_t* sz;
	schar_t* wz;

	switch (type)
	{
	case L'c':
		ch = va_arg(*parg, wchar_t);
		if (buf)
		{
			*buf = ch;
			*(buf + 1) = L'\0';
		}
		return 1;
	case L'd':
		if (size == L'h')
		{
			pos = 0;
			s = va_arg(*parg, short);
			if (flag == L'+')
			{
				if (s < 0)
				{
					if (buf)
					{
						buf[pos] = L'-';
					}
					s = 0 - s;
				}
				else
				{
					if (buf)
					{
						buf[pos] = L'+';
					}
				}
				pos++;
			}
			else
			{
				if (s < 0)
				{
					if (buf)
					{
						buf[pos] = L'-';
					}
					s = 0 - s;
					pos++;
				}
			}
			if (width)
			{
				len = width - w_stoxs(s, NULL, width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = L'0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + w_stoxs(s, ((buf) ? buf + pos : NULL), width);
		}
		else if (size == L'l')
		{
			pos = 0;
			li = va_arg(*parg, long long);
			if (flag == L'+')
			{
				if (li < 0)
				{
					if (buf)
					{
						buf[pos] = L'-';
					}
					li = 0 - li;
				}
				else
				{
					if (buf)
					{
						buf[pos] = L'+';
					}
				}
				pos++;
			}
			else
			{
				if (li < 0)
				{
					if (buf)
					{
						buf[pos] = L'-';
					}
					li = 0 - li;
					pos++;
				}
			}
			if (width)
			{
				len = width - w_lltoxs(li, NULL, width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = L'0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + w_lltoxs(li, ((buf) ? buf + pos : NULL), width);
		}
		else
		{
			pos = 0;
			i = va_arg(*parg, long);
			if (flag == L'+')
			{
				if (i < 0)
				{
					if (buf)
					{
						buf[pos] = L'-';
					}
					i = 0 - i;
				}
				else
				{
					if (buf)
					{
						buf[pos] = L'+';
					}
				}
				pos++;
			}
			else
			{
				if (i < 0)
				{
					if (buf)
					{
						buf[pos] = L'-';
					}
					i = 0 - i;
					pos++;
				}
			}
			if (width)
			{
				len = width - w_ltoxs(i, NULL, width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = L'0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + w_ltoxs(i, ((buf) ? buf + pos : NULL), width);
		}
		break;
	case L'u':
		if (size == L'h')
		{
			pos = 0;
			us = va_arg(*parg, unsigned short);
			if (width)
			{
				len = width - w_ustoxs(us, NULL, width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = L'0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + w_ustoxs(us, ((buf) ? buf + pos : NULL), width);
		}
		else
		{
			pos = 0;
			ui = va_arg(*parg, unsigned long);
			if (width)
			{
				len = width - w_ultoxs(ui, NULL, width);
				while (len > 0)
				{
					if (buf)
					{
						buf[pos] = L'0';
					}
					pos++;
					len--;
				}
			}
			else
			{
				width = NUM_LEN;
			}
			return pos + w_ultoxs(ui, ((buf) ? buf + pos : NULL), width);
		}
	case L'x':
	case L'X':
		ui = va_arg(*parg, unsigned long);
		pos = 0;
		if (flag == L'#')
		{
			if (buf)
			{
				buf[0] = L'0';
				buf[1] = type;
			}
			pos += 2;
		}
		if (width)
		{
			len = width - w_ltohex(ui, type, NULL, width);
			while (len > 0)
			{
				if (buf)
				{
					buf[pos] = L'0';
				}
				pos++;
				len--;
			}
		}
		else
		{
			width = NUM_LEN;
		}
		return pos + w_ltohex(ui, type, ((buf) ? (buf + pos) : NULL), width);
	case L'f':
		dbl = va_arg(*parg, double);
		if (!width)
		{
			width = NUM_LEN;
		}
		if (!prec)
		{
			prec = MAX_DOUBLE_DIGI;
		}
		return w_numtoxs_dig(dbl, prec, buf, width);
	case L's':
		sz = va_arg(*parg, wchar_t*);

		len = w_xslen(sz);

		if (!width)
			width = len;
		else
			width = (width < len) ? width : len;

		if (buf)
		{
			w_xsncpy(buf, sz, width);
		}
		return width;
	case L'S':
		wz = va_arg(*parg, schar_t*);
		len = mbs_to_ucs(wz, -1, NULL, MAX_LONG);

		if (!width)
			width = len;
		else
			width = (width < len) ? width : len;

		if (buf)
		{
			mbs_to_ucs(wz, -1, buf, width);
			buf[width] = L'\0';
		}
		return width;
	}
	return 0;
}

int a_xsprintf(schar_t* buf,const schar_t* fmt,...)
{
	int rt;
	va_list arg;
	
	va_start(arg,fmt);
	rt = a_xsprintf_arg(buf,fmt,&arg);
	va_end(arg);

	return rt;
}

int a_xsprintf_arg(schar_t* buf,const schar_t* fmt,va_list* parg)
{
	int total = 0;
	schar_t xf_flag = 0;
	int xf_width = 0;
	int xf_prec = 0;
	schar_t xf_size = 0;
	schar_t xf_type = 0;
	schar_t tk_width[NUM_LEN + 1],tk_prec[NUM_LEN + 1];
	int width_count = 0;
	int prec_count = 0;
	int tk_count = 0;


	XF_STATUS xs = XS_SKIP;
	XF_OPERA xo = XO_PAUSE;

	schar_t* token = (schar_t*)fmt;

	while(xs != XS_END)
	{
		switch(xs)
		{
		case XS_SKIP:
			if(*token == '%' && *(token + 1) != '%')
			{
				if(!tk_count)
				{
					xs = XS_FLAG;
					xo = XO_CONTINUE;
				}else
				{
					xs = XS_PROC;
					xo = XO_PAUSE;
				}
			}else if(*token == '\0')
			{
				xs = XS_PROC;
				xo = XO_PAUSE; 
			}else
			{
				xs = XS_SKIP;
				xo = XO_CONTINUE; 
			}
			break;
		case XS_FLAG:
			if(a_is_flag(*token)) 
			{
				xf_flag = *token;
				xs = XS_FLAG;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_WIDTH;
				xo = XO_PAUSE;
			}
			break;
		case XS_WIDTH:
			if(a_is_digit(*token))
			{
				tk_width[width_count ++] = *token;
				xs = XS_WIDTH;
				xo = XO_CONTINUE;
			}else if(*token == '.')
			{
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_PREC:
			if(a_is_digit(*token))
			{
				tk_prec[prec_count ++] = *token;
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_SIZE:
			if(a_is_size(*token))
			{
				xf_size = *token;
				xs = XS_TYPE;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_TYPE;
				xo = XO_PAUSE;
			}
			break;
		case XS_TYPE:
			if(a_is_type(*token))
			{
				xf_type = *token;
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}
			break;
		case XS_PROC:
			if(xf_type)
			{
				tk_width[width_count] = '\0';
				xf_width = a_xstol(tk_width);

				tk_prec[prec_count] = '\0';
				xf_prec = a_xstol(tk_prec);

				total += a_tk_printf((buf)? (buf + total) : NULL,xf_flag,xf_width,xf_prec,xf_size,xf_type,parg);

				if(*token == '\0')
					xs = XS_END;
				else
					xs = XS_SKIP;

				xo = XO_PAUSE;
			}else
			{
				if(buf)
				{
					a_xsncpy(buf + total, token - tk_count, tk_count);
				}
				total += tk_count;

				if(*token == '\0')
					xs = XS_END;
				else
					xs = XS_SKIP;

				xo = XO_PAUSE;
			}

			xf_flag = 0;
			xf_width = 0;
			xf_prec = 0;
			xf_size = 0;
			xf_type = 0;
			width_count = prec_count = tk_count = 0;

			break;
		}

		if(xo == XO_CONTINUE)
		{
			token ++;
			tk_count ++;
		}
	}

	return total;
}

int w_xsprintf(wchar_t* buf,const wchar_t* fmt,...)
{
	int rt;
	va_list arg;
	
	va_start(arg,fmt);
	rt = w_xsprintf_arg(buf,fmt,&arg);
	va_end(arg);

	return rt;
}

int w_xsprintf_arg(wchar_t* buf,const wchar_t* fmt,va_list* parg)
{
	int total = 0;
	wchar_t xf_flag = 0;
	int xf_width = 0;
	int xf_prec = 0;
	wchar_t xf_size = 0;
	wchar_t xf_type = 0;
	wchar_t tk_width[NUM_LEN + 1],tk_prec[NUM_LEN + 1];
	int width_count = 0;
	int prec_count = 0;
	int tk_count = 0;


	XF_STATUS xs = XS_SKIP;
	XF_OPERA xo = XO_PAUSE;

	wchar_t* token = (wchar_t*)fmt;

	while(xs != XS_END)
	{
		switch(xs)
		{
		case XS_SKIP:
			if(*token == L'%' && *(token + 1) != L'%')
			{
				if(!tk_count)
				{
					xs = XS_FLAG;
					xo = XO_CONTINUE;
				}else
				{
					xs = XS_PROC;
					xo = XO_PAUSE;
				}
			}else if(*token == L'\0')
			{
				xs = XS_PROC;
				xo = XO_PAUSE; 
			}else
			{
				xs = XS_SKIP;
				xo = XO_CONTINUE; 
			}
			break;
		case XS_FLAG:
			if(w_is_flag(*token)) 
			{
				xf_flag = *token;
				xs = XS_FLAG;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_WIDTH;
				xo = XO_PAUSE;
			}
			break;
		case XS_WIDTH:
			if(w_is_digit(*token))
			{
				tk_width[width_count ++] = *token;
				xs = XS_WIDTH;
				xo = XO_CONTINUE;
			}else if(*token == L'.')
			{
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_PREC:
			if(w_is_digit(*token))
			{
				tk_prec[prec_count ++] = *token;
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_SIZE:
			if(w_is_size(*token))
			{
				xf_size = *token;
				xs = XS_TYPE;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_TYPE;
				xo = XO_PAUSE;
			}
			break;
		case XS_TYPE:
			if(w_is_type(*token))
			{
				xf_type = *token;
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}
			break;
		case XS_PROC:
			if(xf_type)
			{
				tk_width[width_count] = L'\0';
				xf_width = w_xstol(tk_width);

				tk_prec[prec_count] = L'\0';
				xf_prec = w_xstol(tk_prec);

				total += w_tk_printf((buf)? (buf + total) : NULL,xf_flag,xf_width,xf_prec,xf_size,xf_type,parg);

				if(*token == L'\0')
					xs = XS_END;
				else
					xs = XS_SKIP;

				xo = XO_PAUSE;
			}else
			{
				if(buf)
				{
					w_xsncpy(buf + total, token - tk_count, tk_count);
				}
				total += tk_count;

				if(*token == L'\0')
					xs = XS_END;
				else
					xs = XS_SKIP;

				xo = XO_PAUSE;
			}

			xf_flag = 0;
			xf_width = 0;
			xf_prec = 0;
			xf_size = 0;
			xf_type = 0;
			width_count = prec_count = tk_count = 0;

			break;
		}

		if(xo == XO_CONTINUE)
		{
			token ++;
			tk_count ++;
		}
	}

	return total;
}

int a_test_numeric(const schar_t* token, int len)
{
	int pos = 0;

	if (!token)
		return 0;

	if (len < 0)
		len = a_xslen(token);

	while (pos < len)
	{
		if (*token == '+' || *token == '-' || *token == '.' || (*token >= '0' && *token <= '9'))
		{
			token++;
			pos++;
		}else
			break;
	}

	return pos;
}

int w_test_numeric(const wchar_t* token, int len)
{
	int pos = 0;

	if (!token)
		return 0;

	if (len < 0)
		len = w_xslen(token);

	while (pos < len)
	{
		if (*token == L'+' || *token == L'-' || *token == L'.' || (*token >= L'0' && *token <= L'9'))
		{
			token++;
			pos++;
		}
		else
			break;
	}

	return pos;
}

int a_test_hex(const schar_t* token, int len)
{
	int pos = 0;

	if (!token)
		return 0;

	while (pos < len)
	{
		if (*token == 'x' || *token == 'X' || (*token >= '0' && *token <= '9') || (*token >= 'A' && *token <= 'F') || (*token >= 'a' && *token <= 'f'))
		{
			token++;
			pos++;
		}else
			break;
	}

	return pos;
}

int w_test_hex(const wchar_t* token, int len)
{
	int pos = 0;

	if (!token)
		return 0;

	while (pos < len)
	{
		if (*token == L'x' || *token == L'X' || (*token >= L'0' && *token <= L'9') || (*token >= L'A' && *token <= L'F') || (*token >= L'a' && *token <= L'f'))
		{
			token++;
			pos++;
		}
		else
			break;
	}

	return pos;
}

const schar_t* a_tk_scanf(const schar_t* token, schar_t size, schar_t type, va_list* parg)
{
	int pos;
	schar_t* pch;
	short *ps;
	long *pi;
	long long *pli;
	unsigned short *pus;
	unsigned long *pui;
	double *pdb;

	switch (type)
	{
	case 'c':
		pch = va_arg(*parg, schar_t*);
		*pch = *token;
		return token + 1;
	case 'd':
		if (size == 'h')
		{
			pos = 0;
			ps = va_arg(*parg, short*);

			pos = a_test_numeric(token, -1);
			*ps = a_xsntos(token, pos);
			return token + pos;
		}
		else if (size == 'l')
		{
			pos = 0;
			pli = va_arg(*parg, long long*);

			pos = a_test_numeric(token, -1);
			*pli = a_xsntoll(token, pos);
			return token + pos;
		}
		else
		{
			pos = 0;
			pi = va_arg(*parg, long*);

			pos = a_test_numeric(token, -1);
			*pi = a_xsntol(token, pos);
			return token + pos;
		}
		break;
	case 'u':
		if (size == 'h')
		{
			pos = 0;
			pus = va_arg(*parg, unsigned short*);

			pos = a_test_numeric(token, -1);
			*pus = (unsigned short)a_xsntos(token, pos);
			return token + pos;
		}
		else
		{
			pos = 0;
			pui = va_arg(*parg, unsigned long*);

			pos = a_test_numeric(token, -1);
			*pui = (unsigned long)a_xsntol(token, pos);
			return token + pos;
		}
		break;
	case 'x':
	case 'X':
		pos = 0;
		pui = va_arg(*parg, unsigned long*);

		pos = a_test_hex(token, -1);
		*pui = a_hexntol(token, pos);
		return token + pos;
	case 'f':
		pos = 0;
		pdb = va_arg(*parg, double*);

		pos = a_test_numeric(token, -1);
		*pdb = a_xsntonum(token, pos);
		return token + pos;
	}

	return NULL;
}

const wchar_t* w_tk_scanf(const wchar_t* token, wchar_t size, wchar_t type, va_list* parg)
{
	int pos;
	wchar_t* pch;
	short *ps;
	long *pi;
	long long *pli;
	unsigned short *pus;
	unsigned long *pui;
	double *pdb;

	switch (type)
	{
	case L'c':
		pch = va_arg(*parg, wchar_t*);
		*pch = *token;
		return token + 1;
	case L'd':
		if (size == L'h')
		{
			pos = 0;
			ps = va_arg(*parg, short*);

			pos = w_test_numeric(token, -1);
			*ps = w_xsntos(token, pos);
			return token + pos;
		}
		else if (size == L'l')
		{
			pos = 0;
			pli = va_arg(*parg, long long*);

			pos = w_test_numeric(token, -1);
			*pli = w_xsntoll(token, pos);
			return token + pos;
		}
		else
		{
			pos = 0;
			pi = va_arg(*parg, long*);

			pos = w_test_numeric(token, -1);
			*pi = w_xsntol(token, pos);
			return token + pos;
		}
		break;
	case L'u':
		if (size == L'h')
		{
			pos = 0;
			pus = va_arg(*parg, unsigned short*);

			pos = w_test_numeric(token, -1);
			*pus = (unsigned short)w_xsntos(token, pos);
			return token + pos;
		}
		else
		{
			pos = 0;
			pui = va_arg(*parg, unsigned long*);

			pos = w_test_numeric(token, -1);
			*pui = (unsigned long)w_xsntol(token, pos);
			return token + pos;
		}
		break;
	case L'x':
	case L'X':
		pos = 0;
		pui = va_arg(*parg, unsigned long*);

		pos = w_test_hex(token, -1);
		*pui = w_hexntol(token, pos);
		return token + pos;
	case L'f':
		pos = 0;
		pdb = va_arg(*parg, double*);

		pos = w_test_numeric(token, -1);
		*pdb = w_xsntonum(token, pos);
		return token + pos;
	}

	return NULL;
}

const schar_t* a_xscanf_arg(const schar_t* str, const schar_t* fmt, va_list* parg)
{
	int total = 0;
	schar_t xf_flag = 0;
	int xf_width = 0;
	int xf_prec = 0;
	schar_t xf_size = 0;
	schar_t xf_type = 0;
	schar_t tk_width[NUM_LEN + 1], tk_prec[NUM_LEN + 1];
	int width_count = 0;
	int prec_count = 0;
	int tk_count = 0;

	XF_STATUS xs = XS_SKIP;
	XF_OPERA xo = XO_PAUSE;

	schar_t* token = (schar_t*)fmt;

	if (a_is_null(str))
		return NULL;

	while (xs != XS_END)
	{
		switch (xs)
		{
		case XS_SKIP:
			if (*token == '%' && *(token + 1) != '%')
			{
				if (!tk_count)
				{
					xs = XS_FLAG;
					xo = XO_CONTINUE;
				}
				else
				{
					xs = XS_PROC;
					xo = XO_PAUSE;
				}
			}
			else if (*token == '\0')
			{
				xs = XS_PROC;
				xo = XO_PAUSE;
			}
			else
			{
				xs = XS_SKIP;
				xo = XO_CONTINUE;
			}
			break;
		case XS_FLAG:
			if (a_is_flag(*token))
			{
				xf_flag = *token;
				xs = XS_FLAG;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_WIDTH;
				xo = XO_PAUSE;
			}
			break;
		case XS_WIDTH:
			if (a_is_digit(*token))
			{
				tk_width[width_count++] = *token;
				xs = XS_WIDTH;
				xo = XO_CONTINUE;
			}
			else if (*token == '.')
			{
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_PREC:
			if (a_is_digit(*token))
			{
				tk_prec[prec_count++] = *token;
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_SIZE:
			if (a_is_size(*token))
			{
				xf_size = *token;
				xs = XS_TYPE;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_TYPE;
				xo = XO_PAUSE;
			}
			break;
		case XS_TYPE:
			if (a_is_type(*token))
			{
				xf_type = *token;
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}
			break;
		case XS_PROC:
			if (xf_type)
			{
				tk_width[width_count] = '\0';
				xf_width = a_xstol(tk_width);

				tk_prec[prec_count] = '\0';
				xf_prec = a_xstol(tk_prec);

				str = a_tk_scanf(str, xf_size, xf_type, parg);

				if (*token == '\0')
					xs = XS_END;
				else
					xs = XS_SKIP;

				xo = XO_PAUSE;
			}
			else
			{
				while (*str && tk_count)
				{
					if (*str == *(token - tk_count))
					{
						str++;
						tk_count--;
					}
				}

				if (*token == '\0')
					xs = XS_END;
				else
					xs = XS_SKIP;

				xo = XO_PAUSE;
			}

			xf_flag = 0;
			xf_width = 0;
			xf_prec = 0;
			xf_size = 0;
			xf_type = 0;
			width_count = prec_count = tk_count = 0;

			break;
		}

		if (xo == XO_CONTINUE)
		{
			token++;
			tk_count++;
		}
	}

	return str;
}

const wchar_t* w_xscanf_arg(const wchar_t* str, const wchar_t* fmt, va_list* parg)
{
	int total = 0;
	wchar_t xf_flag = 0;
	int xf_width = 0;
	int xf_prec = 0;
	wchar_t xf_size = 0;
	wchar_t xf_type = 0;
	wchar_t tk_width[NUM_LEN + 1], tk_prec[NUM_LEN + 1];
	int width_count = 0;
	int prec_count = 0;
	int tk_count = 0;

	XF_STATUS xs = XS_SKIP;
	XF_OPERA xo = XO_PAUSE;

	wchar_t* token = (wchar_t*)fmt;

	if (w_is_null(str))
		return NULL;

	while (xs != XS_END)
	{
		switch (xs)
		{
		case XS_SKIP:
			if (*token == L'%' && *(token + 1) != L'%')
			{
				if (!tk_count)
				{
					xs = XS_FLAG;
					xo = XO_CONTINUE;
				}
				else
				{
					xs = XS_PROC;
					xo = XO_PAUSE;
				}
			}
			else if (*token == L'\0')
			{
				xs = XS_PROC;
				xo = XO_PAUSE;
			}
			else
			{
				xs = XS_SKIP;
				xo = XO_CONTINUE;
			}
			break;
		case XS_FLAG:
			if (w_is_flag(*token))
			{
				xf_flag = *token;
				xs = XS_FLAG;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_WIDTH;
				xo = XO_PAUSE;
			}
			break;
		case XS_WIDTH:
			if (w_is_digit(*token))
			{
				tk_width[width_count++] = *token;
				xs = XS_WIDTH;
				xo = XO_CONTINUE;
			}
			else if (*token == L'.')
			{
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_PREC:
			if (w_is_digit(*token))
			{
				tk_prec[prec_count++] = *token;
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_SIZE:
			if (w_is_size(*token))
			{
				xf_size = *token;
				xs = XS_TYPE;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_TYPE;
				xo = XO_PAUSE;
			}
			break;
		case XS_TYPE:
			if (w_is_type(*token))
			{
				xf_type = *token;
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}
			else
			{
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}
			break;
		case XS_PROC:
			if (xf_type)
			{
				tk_width[width_count] = L'\0';
				xf_width = w_xstol(tk_width);

				tk_prec[prec_count] = L'\0';
				xf_prec = w_xstol(tk_prec);

				str = w_tk_scanf(str, xf_size, xf_type, parg);

				if (*token == L'\0')
					xs = XS_END;
				else
					xs = XS_SKIP;

				xo = XO_PAUSE;
			}
			else
			{
				while (*str && tk_count)
				{
					if (*str == *(token - tk_count))
					{
						str++;
						tk_count--;
					}
				}

				if (*token == L'\0')
					xs = XS_END;
				else
					xs = XS_SKIP;

				xo = XO_PAUSE;
			}

			xf_flag = 0;
			xf_width = 0;
			xf_prec = 0;
			xf_size = 0;
			xf_type = 0;
			width_count = prec_count = tk_count = 0;

			break;
		}

		if (xo == XO_CONTINUE)
		{
			token++;
			tk_count++;
		}
	}

	return str;
}

const schar_t* a_xscanf(const schar_t* str, const schar_t* fmt, ...)
{
	const schar_t* token;
	va_list arg;

	va_start(arg, fmt);
	token = a_xscanf_arg(str, fmt, &arg);
	va_end(arg);

	return token;
}

const wchar_t* w_xscanf(const wchar_t* str, const wchar_t* fmt, ...)
{
	const wchar_t* token;
	va_list arg;

	va_start(arg, fmt);
	token = w_xscanf_arg(str, fmt, &arg);
	va_end(arg);

	return token;
}

int a_xsappend(schar_t* buf, const schar_t* fmt, ...)
{
	int len;
	va_list arg;

	len = a_xslen(buf);

	va_start(arg, fmt);
	len += a_xsprintf_arg(buf + len, fmt, &arg);
	va_end(arg);

	return len;
}

int w_xsappend(wchar_t* buf, const wchar_t* fmt, ...)
{
	int len;
	va_list arg;

	len = w_xslen(buf);

	va_start(arg, fmt);
	len += w_xsprintf_arg(buf + len, fmt, &arg);
	va_end(arg);

	return len;
}

