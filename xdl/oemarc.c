﻿/*
*  An implementation of the ARCFOUR algorithm
*
*  Copyright (C) 2006-2007  Christophe Devine
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License aint
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/*
*  The ARCFOUR algorithm was publicly disclosed on 94/09.
*
*  http://groups.google.com/group/sci.crypt/msg/10a300c9d21afca0
*/

#include "oemarc.h"

#ifdef XDL_SUPPORT_CRYPT


/*
* ARC4 key schedule
*/
void arc4_setup(arc4_context *ctx, unsigned char *key, int keylen)
{
	int i, j, k, a;
	unsigned char *m;

	ctx->x = 0;
	ctx->y = 0;
	m = ctx->m;

	for (i = 0; i < 256; i++)
		m[i] = (unsigned char)i;

	j = k = 0;

	for (i = 0; i < 256; i++, k++)
	{
		if (k >= keylen) k = 0;

		a = m[i];
		j = (j + a + key[k]) & 0xFF;
		m[i] = m[j];
		m[j] = (unsigned char)a;
	}
}

/*
* ARC4 cipher function
*/
void arc4_crypt(arc4_context *ctx, int length, unsigned char *input, unsigned char* output)
{
	int i, x, y, a, b;
	unsigned char *m;

	x = ctx->x;
	y = ctx->y;
	m = ctx->m;

	for (i = 0; i < length; i++)
	{
		x = (x + 1) & 0xFF; a = m[x];
		y = (y + a) & 0xFF; b = m[y];

		m[x] = (unsigned char)b;
		m[y] = (unsigned char)a;

		output[i] = (unsigned char)(input[i] ^ m[(unsigned char)(a + b)]);
	}

	ctx->x = x;
	ctx->y = y;
}

#endif //XDL_SUPPORT_CRYPT