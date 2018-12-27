/*
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MPL as stated above or under the terms of the GNU
 * Library General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Library general Public License for more
 * details.
 */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define __INCLUDE_CODE128LIBIMP_H__
#include "code128libimp.h"
#undef __INCLUDE_CODE128LIBIMP_H__
#include "code128lib.h"

void code128init(code128param* p)
{
	memset((void*)p, 0, sizeof(code128param));
}

void code128free(code128param* p)
{
	if (p->outBytes)
		free(p->outBytes);
	p->outBytes = NULL;
}

void code128exec(code128param* p)
{
	char *token,*text;
	int i, j;
	int check; //chek sum
	int* pci;
	int len, index;

	if (!p->inText || !p->lenText)
	{
		p->error = -1;
		return;
	}
	
	text = p->inText;
	len = p->lenText;
	pci = (int*)calloc((len + 3),  sizeof(int));

	i = 0;
	pci[i] = 104; //CODE Begin
	i++;

	check = pci[i];
	j = len;
	while (j--)
	{
		token = Code128;
		index = 0;
		while (*text != *token && *token)
		{
			token++;
			index++;
		}
		if (!*token)
		{
			free(pci);
			p->error = -1;
			return;
		}

		pci[i] = index;
		check += pci[i] * i;

		i++;
		text++;
	}
	pci[i] = check % 103;
	pci[i + 1] = 106; //code end

	len += 3;
	p->lenBytes = len * 6 + 1;
	p->outBytes = (char*)calloc((p->lenBytes + 1),  sizeof(char));
	p->codeColumns = len;

	for (i = 0; i<len; i++)
	{
		strcat(p->outBytes, (char*)Bar128[pci[i]]);
	}

	free(pci);
}
