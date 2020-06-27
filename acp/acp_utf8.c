/***********************************************************************
	Easily codepage

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc utf8 codepage document

	@module	acp.c | acp implement file

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

#include "acpdef.h"

int utf8_code_sequence(unsigned char b)
{
	if ((b & ~0x7F) == 0) {
		return 1;
	}
	//if ((b & 0xC0) != 0xC0) {
	//return 0;
	//}
	if ((b & 0xE0) == 0xC0) {
		return 2;
	}
	if ((b & 0xF0) == 0xE0) {
		return 3;
	}
	if ((b & 0xF8) == 0xF0) {
		return 4;
	}

	return 1;
}

int utf8_seek_unicode(unsigned char* src, unsigned short* dest)
{
	unsigned char b0, b1, b2, b3;
	int len;
	int c;

	len = utf8_code_sequence(*src);

	if (len == 3)
	{
		if (src[0] == 0xEF && src[1] == 0xBB && src[2] == 0xBF) //_UTF8 BOM
		{
			if (dest)
			{
				*dest = (unsigned short)DEFBOM;
			}
			return 1;
		}
	}

	if (len == 1)
	{
		// Handle 1-byte sequences (plain ASCII). 
		//UNICODE 00000000 - 0000007F.
		//UTF8 0xxxxxxx 
		b0 = src[0];
		c = (int)b0;
	}
	else if (len == 2)
	{
		// Handle 2-byte sequences.
		//UNICODE 00000080-000007FF
		//UTF8 110xxxxx 10xxxxxx 

		//((110xxxxx & 00011111) << 6 = 0000 0xxx xx00 0000
		//(10xxxxxx & 00111111)       = 0000 0000 00xx xxxx
		b0 = src[0];
		b1 = src[1];
		c = ((b0 & 0x1F) << 6) | (b1 & 0x3F);
		if (c < 0x80)
			c = ALT_CHAR;
	}
	else if (len == 3)
	{
		// Handle 3-byte sequences.
		//UNICODE 00000800-0000FFFF
		//UTF8 1110xxxx 10xxxxxx 10xxxxxx

		//(1110xxxx & 00001111) << 12 = xxxx 0000 0000 0000
		//(10xxxxxx & 00111111) << 6  = 0000 xxxx xx00 0000
		//(10xxxxxx & 00111111)       = 0000 0000 00xx xxxx
		b0 = src[0];
		b1 = src[1];
		b2 = src[2];
		c = ((b0 & 0xF) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
		if (c < 0x800)
			c = ALT_CHAR;
		else if (c >= 0xD800 && c <= 0xDFFF)
			c = ALT_CHAR;
	}
	else if (len == 4)
	{
		// Handle 4-byte sequences.
		//UNICODE 00010000-001FFFFF
		//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

		//(11110xxx & 00000111) << 18 = 000x xx00 0000 0000 0000 0000
		//(10xxxxxx & 00111111) << 12 = 0000 00xx xxxx 0000 0000 0000
		//(10xxxxxx & 00111111) << 6  = 0000 0000 0000 xxxx xx00 0000
		//(10xxxxxx & 00111111)       = 0000 0000 0000 0000 00xx xxxx
		b0 = src[0];
		b1 = src[1];
		b2 = src[2];
		b3 = src[3];
		c = ((b0 & 0x7) << 18) | ((b1 & 0x3F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
		if (c < 0x10000 || c > 0x10FFFF)
			c = ALT_CHAR;
	}
	else
	{
		// Handle other byte sequences (never valid).
		c = ALT_CHAR;
	}

	if (dest)
	{
		*dest = (unsigned short)c;
	}

	return 1;
}

int unicode_seek_utf8(unsigned short ch, unsigned char* dest)
{
	int c;
	int index = 0;

	if (ch == BIGBOM || ch == LITBOM)
	{
		if (dest)
		{
			dest[0] = 0xEF;
			dest[1] = 0xBB;
			dest[2] = 0xBF;
		}
		return 3;
	}

	c = (int)(ch);

	if (c >= 0x00 && c <= 0x7F)
	{
		// Handle 1-byte character (plain ASCII).
		//UNICODE 00000000 鈥?0000007F
		//UTF8 0xxxxxxx
		if (dest)
		{
			dest[index] = (unsigned char)c;
		}
		index++;
	}
	else if (c >= 0x80 && c <= 0x7FF)
	{
		// Handle 2-byte character.
		//UNICODE 00000080 鈥?000007FF
		//UTF8 110xxxxx 10xxxxxx 

		//xxxx xxxx xxxx xxxx & 0000 0111 1100 0000
		//0000 0xxx xx00 0000 >> 6
		//0000 0000 000x xxxx | 0000 0000 1100 0000
		//0000 0000 110x xxxx
		if (dest)
		{
			dest[index] = (unsigned char)(((c & 0x7C0) >> 6) | 0xC0);
		}
		index++;
		//xxxx xxxx xxxx xxxx & 0000 0000 0011 1111
		//0000 0000 00xx xxxx | 0000 0000 1000 0000
		//0000 0000 10xx xxxx
		if (dest)
		{
			dest[index] = (unsigned char)((c & 0x3F) | 0x80);
		}
		index++;
	}
	else if (c >= 0x800 && c <= 0xFFFF)
	{
		// Handle 3-byte character.
		//UNICODE 00000800 鈥?0000FFFF
		//UTF8 1110xxxx 10xxxxxx 10xxxxxx

		//xxxx xxxx xxxx xxxx & 1111 0000 0000 0000
		//xxxx 0000 0000 0000 >> 12
		//0000 0000 0000 xxxx | 0000 0000 1110 0000
		//0000 0000 1110 xxxx
		if (dest)
		{
			dest[index] = (unsigned char)(((c & 0xF000) >> 12) | 0xE0);
		}
		index++;

		//xxxx xxxx xxxx xxxx & 0000 1111 1100 0000
		//0000 xxxx xx00 0000 >> 6
		//0000 0000 00xx xxxx | 0000 0000 1000 0000
		//0000 0000 10xx xxxx
		if (dest)
		{
			dest[index] = (unsigned char)(((c & 0xFC0) >> 6) | 0x80);
		}
		index++;

		//xxxx xxxx xxxx xxxx & 0000 0000 0011 1111
		//0000 0000 00xx xxxx | 0000 0000 1000 0000
		//0000 0000 10xx xxxx
		if (dest)
		{
			dest[index] = (unsigned char)((c & 0x3F) | 0x80);
		}
		index++;
	}
	else if (c >= 0x10000 && c <= 0xFFFFF)
	{
		// Handle 4-bytes character.
		//UNICODE 00010000 鈥?001FFFFF
		//UTF8 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

		//xxxx xxxx xxxx xxxx xxxx & 1100 0000 0000 0000 0000
		//xx00 0000 0000 0000 0000 >> 18
		//0000 0000 0000 0000 00xx | 0000 0000 0000 1111 0000
		//0000 0000 0000 1111 00xx
		if (dest)
		{
			dest[index] = (unsigned char)(((c & 0xC0000) >> 18) | 0xF0);
		}
		index++;
		//xxxx xxxx xxxx xxxx xxxx & 0011 1111 0000 0000 0000
		//00xx xxxx 0000 0000 0000 >> 12
		//0000 0000 0000 00xx xxxx | 0000 0000 0000 1000 0000
		//0000 0000 0000 10xx xxxx
		if (dest)
		{
			dest[index] = (unsigned char)(((c & 0x3F000) >> 12) | 0x80);
		}
		index++;
		//xxxx xxxx xxxx xxxx xxxx & 0000 0000 1111 1100 0000
		//0000 0000 xxxx xx00 0000 >> 6
		//0000 0000 0000 00xx xxxx | 0000 0000 0000 1000 0000
		//0000 0000 0000 10xx xxxx
		if (dest)
		{
			dest[index] = (unsigned char)(((c & 0xFC0) >> 6) | 0x80);
		}
		index++;
		//xxxx xxxx xxxx xxxx xxxx & 0000 0000 0000 0011 1111
		//0000 0000 0000 00xx xxxx | 0000 0000 0000 1000 0000
		//0000 0000 0000 10xx xxxx
		if (dest)
		{
			dest[index] = (unsigned char)((c & 0x3F) | 0x80);
		}
		index++;
	}

	return index;
}
