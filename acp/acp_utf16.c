/***********************************************************************
	Easily codepage

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc utf16 codepage document

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


#include "acp.h"

int utf16lit_code_sequence(unsigned char b)
{
	return 2;
}

//unicode:	0000 0000 yyyy yyyy yyxx xxxx xxxx
//utf16:	1101 10yy yyyy yyyy 1101 11xx xxxx xxxx
int utf16lit_seek_unicode(unsigned char* mbs, unsigned short* ucs)
{
	unsigned short ch, cl;

	ch = LIT_MAKESHORT(mbs[0], mbs[1]);

	if (ch > 0xDC00 && ch < 0xDFFF)
	{
		if (ucs)
		{
			cl = LIT_MAKESHORT(mbs[2], mbs[3]);

			*ucs = ((ch & ~0xD800) << 10) | (cl & ~0xDC00) + 0x10000;
		}
	}
	else if (ch == 0xFEFF)
	{
		if (ucs)
		{
			*ucs = ALT_CHAR;
		}
	}
	else
	{
		if (ucs)
		{
			*ucs = ch;
		}
	}

	return 1;
}

int unicode_seek_utf16lit(unsigned short ucs, unsigned char* mbs)
{
	if (ucs < 0 || ucs > 0x10FFFF)
		return 0;

	if (ucs < 0x10000)
	{
		if (mbs)
		{
			mbs[0] = LIT_GETLCHAR(ucs);
			mbs[1] = LIT_GETHCHAR(ucs);
		}
		return 2;
	}
	else
	{
		ucs = (unsigned short)((unsigned long)ucs - 0x10000);

		if (mbs)
		{
			mbs[0] = LIT_GETLCHAR(((ucs & 0xFFC00) >> 10) | 0xD800);
			mbs[1] = LIT_GETHCHAR(((ucs & 0xFFC00) >> 10) | 0xD800);
			mbs[2] = LIT_GETLCHAR((ucs & 0x3FF) | 0xDC00);
			mbs[3] = LIT_GETHCHAR((ucs & 0x3FF) | 0xDC00);;
		}
		return 4;
	}

	return 0;
}

int utf16big_code_sequence(unsigned char b)
{
	return 2;
}

int utf16big_seek_unicode(unsigned char* mbs, unsigned short* ucs)
{
	unsigned short ch, cl;

	ch = BIG_MAKESHORT(mbs[0], mbs[1]);

	if (ch > 0xDC00 && ch < 0xDFFF)
	{
		if (ucs)
		{
			cl = BIG_MAKESHORT(mbs[2], mbs[3]);

			*ucs = ((ch & ~0xD800) << 10) | (cl & ~0xDC00) + 0x10000;
		}
	}
	else if (ch == 0xFEFF)
	{
		if (ucs)
		{
			*ucs = ALT_CHAR;
		}
	}
	else
	{
		if (ucs)
		{
			*ucs = ch;
		}
	}

	return 1;
}

int unicode_seek_utf16big(unsigned short ucs, unsigned char* mbs)
{
	if (ucs < 0 || ucs > 0x10FFFF)
		return 0;

	if (ucs < 0x10000)
	{
		if (mbs)
		{
			mbs[0] = BIG_GETLCHAR(ucs);
			mbs[1] = BIG_GETHCHAR(ucs);
		}
		return 2;
	}
	else
	{
		ucs -= (unsigned short)((unsigned long)ucs - 0x10000);

		if (mbs)
		{
			mbs[0] = BIG_GETLCHAR(((ucs & 0xFFC00) >> 10) | 0xD800);
			mbs[1] = BIG_GETHCHAR(((ucs & 0xFFC00) >> 10) | 0xD800);
			mbs[2] = BIG_GETLCHAR((ucs & 0x3FF) | 0xDC00);
			mbs[3] = BIG_GETHCHAR((ucs & 0x3FF) | 0xDC00);;
		}
		return 4;
	}

	return 0;
}
