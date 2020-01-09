/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc math document

	@module	xdlmath.c | math implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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

#include "xdlmath.h"

#ifdef XDL_SUPPORT_MATH

#include <math.h>
#include <time.h>

double fcos(double f)
{
	return cos(f);
}

double facos(double f)
{
	return acos(f);
}

double fsin(double f)
{
	return sin(f);
}

double fasin(double f)
{
	return asin(f);
}

double ftan(double f)
{
	return tan(f);
}

double fatan(double f)
{
	return atan(f);
}

void frand(int digs, int* pa, int max)
{
	int i, r = 1;

	while (digs--)
		r *= 10;

	srand(time(NULL));

	max = (max < r) ? max : r;
	for (i = 0; i < max; i++)
	{
		pa[i] = rand() % r;
	}
}

#endif /*XDL_SUPPORT_MATH*/