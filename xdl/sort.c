/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc sorting document

	@module	sort.c | implement file

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

#include "sort.h"

void bubble_xsort(xsort_t* pxs, int count)
{
	int i,tag = 1;
	xsort_t x = { 0 };

	while (tag)
	{
		tag = 0;
		count--;
		for (i = 0; i < count; i++)
		{
			if (pxs[i].fact > pxs[i + 1].fact)
			{
				x.fact = pxs[i + 1].fact;
				x.data = pxs[i + 1].data;

				pxs[i + 1].fact = pxs[i].fact;
				pxs[i + 1].data = pxs[i].data;

				pxs[i].fact = x.fact;
				pxs[i].data = x.data;

				tag = 1;
			}
		}
	}
}
