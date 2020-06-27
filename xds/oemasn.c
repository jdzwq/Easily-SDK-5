/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc asn1 document

	@module	oemasn.c | implement file

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

#include "oemasn.h"


/******************************************************************************************************************/

dword_t xasn_get_tag(byte_t *buf, int* ptag)
{
	*ptag = *(buf);

	return 1;
}

dword_t xasn_set_tag(byte_t* buf, int tag)
{
	if (buf)
	{
		*buf = (byte_t)tag;
	}

	return 1;
}

dword_t xasn_get_len(byte_t* buf, int *plen)
{
	if ((buf[0] & 0x80) == 0)
	{
		*plen = buf[0];
		return 1;
	}
	else
	{
		switch ((*buf) & 0x7F)
		{
		case 1:
			*plen = buf[1];
			return 2;
		case 2:
			*plen = (buf[1] << 8) | buf[2];
			return 3;
		default:
			*plen = 0;
			return 0;
		}
	}
}

dword_t xasn_set_len(byte_t* buf, int len)
{
	if (len < 0)
		return 0;

	if (len <= 127)
	{
		if (buf)
		{
			buf[0] = (byte_t)len;
		}
		return 1;
	}
	else if (len <= 256)
	{
		if (buf)
		{
			buf[0] = 0x81;
			buf[1] = (byte_t)len;
		}
		return 2;
	}
	else
	{
		if (buf)
		{
			buf[0] = 0x82;
			buf[1] = (byte_t)((len & 0xFF00) >> 8);
			buf[2] = (byte_t)(len & 0x00FF);
		}
		return 3;
	}
}

dword_t xasn_get_bool(byte_t* buf, bool_t *pval)
{
	*pval = (buf[0]) ? 1 : 0;

	return 1;
}

dword_t xasn_set_bool(byte_t* buf, bool_t val)
{
	if (buf)
	{
		buf[0] = (val) ? 0xFF : 0x00;
	}

	return 1;
}
