/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc base document

	@module	oembas.c | implement file

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

#include "oembas.h"
#include "imperr.h"

const wchar_t w_base_table[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const schar_t a_base_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

const int base_index[128] = {
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 62,
	255, 255, 255, 63,
	52, 53, 54, 55,
	56, 57, 58, 59,
	60, 61, 255, 255,
	255, 255, 255, 255,
	255, 0, 1, 2,
	3, 4, 5, 6,
	7, 8, 9, 10,
	11, 12, 13, 14,
	15, 16, 17, 18,
	19, 20, 21, 22,
	23, 24, 25, 255,
	255, 255, 255, 255,
	255, 26, 27, 28,
	29, 30, 31, 32,
	33, 34, 35, 36,
	37, 38, 39, 40,
	41, 42, 43, 44,
	45, 46, 47, 48,
	49, 50, 51, 255,
	255, 255, 255, 255
};

dword_t a_xbas_decode(const schar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	int b1, b2, b3, b4;
	int tag, rk, i;
	dword_t count = 0;

	if (slen < 0)
		slen = (src) ? strlen(src) : 0;

	tag = 0;
	rk = slen / 4;

	for (i = 0; i < rk; i++)
	{
		if (count > dlen)
			return count;

		if (i == rk - 1)
		{
			if (src[i * 4 + 3] == '=' && src[i * 4 + 2] == '=')
				tag = 2;
			else if (src[i * 4 + 3] == '=')
				tag = 1;
			else
				tag = 0;
		}

		b1 = base_index[0x00FF & src[i * 4]];
		b2 = base_index[0x00FF & src[i * 4 + 1]];
		b3 = base_index[0x00FF & src[i * 4 + 2]];
		b4 = base_index[0x00FF & src[i * 4 + 3]];

		if (b1 == 255)
			continue;

		if (dest)
		{
			if (tag == 0)
			{
				dest[count] = (0xFC & (b1 << 2)) | (0x03 & (b2 >> 4));
				dest[count + 1] = (0xF0 & (b2 << 4)) | (0x0F & (b3 >> 2));
				dest[count + 2] = (0xC0 & (b3 << 6)) | (0x3F & b4);
			}
			else if (tag == 1)
			{
				dest[count] = (0xFC & (b1 << 2)) | (0x03 & (b2 >> 4));
				dest[count + 1] = (0xF0 & (b2 << 4)) | (0x0F & (b3 >> 2));
			}
			else if (tag == 2)
			{
				dest[count] = (0xFC & (b1 << 2)) | (0x03 & (b2 >> 4));
			}
		}

		if (tag == 0)
			count += 3;
		else if (tag == 1)
			count += 2;
		else if (tag == 2)
			count += 1;
	}

	return count;
}

int a_xbas_encode(const byte_t* src, dword_t slen, schar_t* dest, int dlen)
{
	int b1, b2, b3, b4;
	int max, rm, rk, i;
	int count = 0;

	if (slen < 0)
		slen = (src) ? (int)strlen((char*)src) : 0;

	max = (int)strlen(a_base_table) - 1;
	rm = slen % 3;
	rk = slen / 3;

	for (i = 0; i < rk; i++)
	{
		if (count > dlen)
			return count;

		b1 = 0x3F & (src[i * 3] >> 2);
		b2 = (0x30 & (src[i * 3] << 4)) | (0x0F & (src[i * 3 + 1] >> 4));
		b3 = (0x3C & (src[i * 3 + 1] << 2)) | (0x03 & (src[i * 3 + 2] >> 6));
		b4 = 0x3F & src[i * 3 + 2];

		if (dest)
		{
			dest[count] = a_base_table[b1];
			dest[count + 1] = a_base_table[b2];
			dest[count + 2] = a_base_table[b3];
			dest[count + 3] = a_base_table[b4];
		}

		count += 4;
	}

	if (rm == 2)
	{
		b1 = 0x3F & (src[i * 3] >> 2);
		b2 = (0x30 & (src[i * 3] << 4)) | (0x0F & (src[i * 3 + 1] >> 4));
		b3 = (0x3C & (src[i * 3 + 1] << 2));
		b4 = max;

		if (dest)
		{
			dest[count] = a_base_table[b1];
			dest[count + 1] = a_base_table[b2];
			dest[count + 2] = a_base_table[b3];
			dest[count + 3] = a_base_table[b4];
		}

		count += 4;
	}
	else if (rm == 1)
	{
		b1 = 0x3F & (src[i * 3] >> 2);
		b2 = (0x30 & (src[i * 3] << 4));
		b3 = max;
		b4 = max;

		if (dest)
		{
			dest[count] = a_base_table[b1];
			dest[count + 1] = a_base_table[b2];
			dest[count + 2] = a_base_table[b3];
			dest[count + 3] = a_base_table[b4];
		}

		count += 4;
	}

	return count;
}

dword_t w_xbas_decode(const wchar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	int b1, b2, b3, b4;
	int tag, rk, i;
	dword_t count = 0;

	if (slen < 0)
		slen = (src) ? wcslen(src) : 0;

	tag = 0;
	rk = slen / 4;

	for (i = 0; i < rk; i++)
	{
		if (count > dlen)
			return count;

		if (i == rk - 1)
		{
			if (src[i * 4 + 3] == L'=' && src[i * 4 + 2] == L'=')
				tag = 2;
			else if (src[i * 4 + 3] == L'=')
				tag = 1;
			else
				tag = 0;
		}

		b1 = base_index[0x00FF & src[i * 4]];
		b2 = base_index[0x00FF & src[i * 4 + 1]];
		b3 = base_index[0x00FF & src[i * 4 + 2]];
		b4 = base_index[0x00FF & src[i * 4 + 3]];

		if (b1 == 255)
			continue;

		if (dest)
		{
			if (tag == 0)
			{
				dest[count] = (0xFC & (b1 << 2)) | (0x03 & (b2 >> 4));
				dest[count + 1] = (0xF0 & (b2 << 4)) | (0x0F & (b3 >> 2));
				dest[count + 2] = (0xC0 & (b3 << 6)) | (0x3F & b4);
			}
			else if (tag == 1)
			{
				dest[count] = (0xFC & (b1 << 2)) | (0x03 & (b2 >> 4));
				dest[count + 1] = (0xF0 & (b2 << 4)) | (0x0F & (b3 >> 2));
			}
			else if (tag == 2)
			{
				dest[count] = (0xFC & (b1 << 2)) | (0x03 & (b2 >> 4));
			}
		}

		if (tag == 0)
			count += 3;
		else if (tag == 1)
			count += 2;
		else if (tag == 2)
			count += 1;
	}

	return count;
}

int w_xbas_encode(const byte_t* src, dword_t slen, wchar_t* dest, int dlen)
{
	int b1, b2, b3, b4;
	int max, rm, rk, i;
	int count = 0;

	if (slen < 0)
		slen = (src) ? (int)strlen((char*)src) : 0;

	max = (int)wcslen(w_base_table) - 1;
	rm = slen % 3;
	rk = slen / 3;

	for (i = 0; i < rk; i++)
	{
		if (count > dlen)
			return count;

		b1 = 0x3F & (src[i * 3] >> 2);
		b2 = (0x30 & (src[i * 3] << 4)) | (0x0F & (src[i * 3 + 1] >> 4));
		b3 = (0x3C & (src[i * 3 + 1] << 2)) | (0x03 & (src[i * 3 + 2] >> 6));
		b4 = 0x3F & src[i * 3 + 2];

		if (dest)
		{
			dest[count] = w_base_table[b1];
			dest[count + 1] = w_base_table[b2];
			dest[count + 2] = w_base_table[b3];
			dest[count + 3] = w_base_table[b4];
		}

		count += 4;
	}

	if (rm == 2)
	{
		b1 = 0x3F & (src[i * 3] >> 2);
		b2 = (0x30 & (src[i * 3] << 4)) | (0x0F & (src[i * 3 + 1] >> 4));
		b3 = (0x3C & (src[i * 3 + 1] << 2));
		b4 = max;

		if (dest)
		{
			dest[count] = w_base_table[b1];
			dest[count + 1] = w_base_table[b2];
			dest[count + 2] = w_base_table[b3];
			dest[count + 3] = w_base_table[b4];
		}

		count += 4;
	}
	else if (rm == 1)
	{
		b1 = 0x3F & (src[i * 3] >> 2);
		b2 = (0x30 & (src[i * 3] << 4));
		b3 = max;
		b4 = max;

		if (dest)
		{
			dest[count] = w_base_table[b1];
			dest[count + 1] = w_base_table[b2];
			dest[count + 2] = w_base_table[b3];
			dest[count + 3] = w_base_table[b4];
		}

		count += 4;
	}

	return count;
}
