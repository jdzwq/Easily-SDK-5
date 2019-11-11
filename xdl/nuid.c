/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc nuid document

	@module	nuid.c | implement file

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

#include "nuid.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdloem.h"

void nuid_zero(nuid_t* pu)
{
	xmem_zero((void*)pu, sizeof(nuid_t));
}

void nuid_from_timestamp(nuid_t* pu, lword_t tms)
{
	dword_t r, nh, nl;

	Srand48((dword_t)(GETLDWORD(tms)));
	r = Lrand48() >> 16;
	nh = r | 0x0100;
	nl = Lrand48();

	pu->data1 = (dword_t)(tms & 0xffffffff);
	pu->data2 = (sword_t)((tms >> 32) & 0xffff);
	pu->data3 = (sword_t)(((tms >> 48) & 0x0ffff) | 0x1000);
	
	PUT_SWORD_LOC(pu->data4, 6, (sword_t)((r & 0x3fff) | 0x8000));
	PUT_SWORD_LOC(pu->data4, 4, (sword_t)(nh));
	PUT_SWORD_LOC(pu->data4, 0, (dword_t)(nl));
}

lword_t nuid_to_timestamp(nuid_t* pu)
{
	return (lword_t)(pu->data1) | ((lword_t)(pu->data2) << 32) | ((lword_t)(pu->data3 & 0x0fff) << 48);
}

void nuid_from_md5(nuid_t* pu, byte_t buf[16])
{
	pu->data1 = (dword_t)(buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3]);
	pu->data2 = (sword_t)(buf[4] << 8 | buf[5]);
	//pu->data3 = (sword_t)(((buf[6] & 0x0f) | 0x30) << 8 | buf[7]);
	pu->data3 = (sword_t)(buf[6] << 8 | buf[7]);

	//PUT_SWORD_LOC(pu->data4, 6, (sword_t)(((buf[8] & 0x3f) | 0x80) << 8 | buf[9]));
	PUT_SWORD_LOC(pu->data4, 6, (sword_t)(buf[8] << 8 | buf[9]));
	PUT_SWORD_LOC(pu->data4, 4, (sword_t)(buf[10] << 8 | buf[11]));
	PUT_DWORD_LOC(pu->data4, 0, (dword_t)(buf[12] << 24 | buf[13] << 16 | buf[14] << 8 | buf[15]));
}

void nuid_to_md5(nuid_t* pu, byte_t buf[16])
{
	buf[0] = (byte_t)(pu->data1 >> 24);
	buf[1] = (byte_t)(pu->data1 >> 16);
	buf[2] = (byte_t)(pu->data1 >> 8);
	buf[3] = (byte_t)(pu->data1);

	buf[4] = (byte_t)(pu->data2 >> 8);
	buf[5] = (byte_t)(pu->data2);

	buf[6] = (byte_t)(pu->data3 >> 8);
	buf[7] = (byte_t)(pu->data3);

	buf[8] = (byte_t)(GET_SWORD_LOC(pu->data4, 6) >> 8);
	buf[9] = (byte_t)(GET_SWORD_LOC(pu->data4, 6));
	buf[10] = (byte_t)(GET_SWORD_LOC(pu->data4, 4) >> 8);
	buf[11] = (byte_t)(GET_SWORD_LOC(pu->data4, 4));
	buf[12] = (byte_t)(GET_DWORD_LOC(pu->data4, 0) >> 24);
	buf[13] = (byte_t)(GET_DWORD_LOC(pu->data4, 0) >> 16);
	buf[14] = (byte_t)(GET_DWORD_LOC(pu->data4, 0) >> 8);
	buf[15] = (byte_t)(GET_DWORD_LOC(pu->data4, 0));
}

void nuid_parse_string(nuid_t* pu, const tchar_t* buf, int len)
{
	const tchar_t* token = buf;
	tchar_t* num;
	int n, k;
	unsigned short us;
	unsigned long ul;

	if (len < 0)
		len = xslen(buf);

	num = NULL;
	n = len - (token - buf);
	token = parse_string_token(token, n, _T('-'), &num, &k);
	if (k)
	{
		pu->data1 = parse_hexnum(num, k);
	}
	if (!token)
		return;

	num = NULL;
	n = len - (token - buf);
	token = parse_string_token(token, n, _T('-'), &num, &k);
	if (k)
	{
		pu->data2 = (unsigned short)parse_hexnum(num, k);
	}
	if (!token)
		return;

	num = NULL;
	n = len - (token - buf);
	token = parse_string_token(token, n, _T('-'), &num, &k);
	if (k)
	{
		pu->data3 = (unsigned short)parse_hexnum(num, k);
	}
	if (!token)
		return;

	num = NULL;
	n = len - (token - buf);
	token = parse_string_token(token, n, _T('-'), &num, &k);
	if (k)
	{
		us = (unsigned short)parse_hexnum(num, k);
		PUT_SWORD_LOC(pu->data4, 6, us);
	}
	if (!token)
		return;

	num = NULL;
	n = len - (token - buf);
	token = parse_string_token(token, n, _T('-'), &num, &k);
	if (k >= 4)
	{
		us = (unsigned short)parse_hexnum(num, 4);
		PUT_SWORD_LOC(pu->data4, 4, us);
		if (k >= 12)
		{
			ul = parse_hexnum(num + 4, 8);
			PUT_DWORD_LOC(pu->data4, 0, ul);
		}
	}
}

int nuid_format_string(nuid_t* pu, tchar_t* buf, int max)
{
	unsigned long a, b, c;

	a = GET_SWORD_LOC(pu->data4, 6);
	b = GET_SWORD_LOC(pu->data4, 4);
	c = GET_DWORD_LOC(pu->data4, 0);

	return xsprintf(buf, _T("%08x-%04x-%04x-%04x-%04x%08x"), pu->data1, pu->data2, pu->data3, a, b, c);
}

#if defined(_DEBUG) || defined(DEBUG)
void test_nuid(void)
{
	lword_t ts = get_timestamp();
	printf("timestamp: %llu\n", ts);

	xdate_t dt = { 0 };
	utc_date_from_timestamp(&dt, ts);
	printf("%d-%d-%d %d:%d:%d %d\n", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.millsec);

	nuid_t ui = { 0 };
	nuid_from_timestamp(&ui, ts);

	lword_t ms = nuid_to_timestamp(&ui);

	tchar_t us[50] = { 0 };
	int len = nuid_format_string(&ui, us, 50);

	wprintf(_T("%s\n"), us);

	nuid_zero(&ui);
	nuid_parse_string(&ui, us, len);

	wprintf(_T("%s\n"), us);

	ts = nuid_to_timestamp(&ui);
	printf("timestamp: %llu\n", ts);

	utc_date_from_timestamp(&dt, ts);
	printf("%d-%d-%d %d:%d:%d %d\n", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.millsec);

	ts = get_times();
	utc_date_from_times(&dt, ts);
	printf("%d-%d-%d %d:%d:%d %d\n", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec, dt.millsec);
}
#endif