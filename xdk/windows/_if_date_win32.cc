/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc time system call document

	@module	_if_date.c | windows implement file

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

#include "xdkiml.h"

#ifdef XDK_SUPPORT_DATE

void _get_loc_date(xdate_t* pxd)
{
	SYSTEMTIME st = {0};

	GetLocalTime(&st);

	pxd->year = st.wYear;
	pxd->mon = st.wMonth;
	pxd->day = st.wDay;
	pxd->hour = st.wHour;
	pxd->min = st.wMinute;
	pxd->sec = st.wSecond;
	pxd->millsec = st.wMilliseconds;

	pxd->wday = st.wDayOfWeek;
}

void _get_utc_date(xdate_t* pxd)
{
	SYSTEMTIME st = { 0 };

	GetSystemTime(&st);

	pxd->year = st.wYear;
	pxd->mon = st.wMonth;
	pxd->day = st.wDay;
	pxd->hour = st.wHour;
	pxd->min = st.wMinute;
	pxd->sec = st.wSecond;
	pxd->millsec = st.wMilliseconds;

	pxd->wday = st.wDayOfWeek;
}

bool_t _mak_utc_week(xdate_t* pxd)
{
	SYSTEMTIME st = {0};
	FILETIME ft = {0};

	st.wYear = pxd->year;
	st.wMonth = pxd->mon;
	st.wDay = pxd->day;
	st.wHour = pxd->hour;
	st.wMinute = pxd->min;
	st.wSecond = pxd->sec;
	st.wMilliseconds = pxd->millsec;

	SystemTimeToFileTime(&st,&ft);
	FileTimeToSystemTime(&ft,&st);
	
	pxd->wday = st.wDayOfWeek;

	return 1;
}

bool_t _mak_loc_week(xdate_t* pxd)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	FILETIME ft_utc = { 0 };

	st.wYear = pxd->year;
	st.wMonth = pxd->mon;
	st.wDay = pxd->day;
	st.wHour = pxd->hour;
	st.wMinute = pxd->min;
	st.wSecond = pxd->sec;
	st.wMilliseconds = pxd->millsec;

	SystemTimeToFileTime(&st, &ft);
	LocalFileTimeToFileTime(&ft, &ft_utc);
	FileTimeToSystemTime(&ft_utc, &st);

	pxd->wday = st.wDayOfWeek;

	return 1;
}

bool_t _loc_date_to_utc(xdate_t* pxd)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	FILETIME ft_utc = { 0 };

	st.wYear = pxd->year;
	st.wMonth = pxd->mon;
	st.wDay = pxd->day;
	st.wHour = pxd->hour;
	st.wMinute = pxd->min;
	st.wSecond = pxd->sec;
	st.wMilliseconds = pxd->millsec;

	SystemTimeToFileTime(&st, &ft);
	LocalFileTimeToFileTime(&ft, &ft_utc);
	FileTimeToSystemTime(&ft_utc, &st);

	pxd->year = st.wYear;
	pxd->mon = st.wMonth;
	pxd->day = st.wDay;
	pxd->hour = st.wHour;
	pxd->min = st.wMinute;
	pxd->sec = st.wSecond;
	pxd->millsec = st.wMilliseconds;

	pxd->wday = st.wDayOfWeek;

	return 1;
}

bool_t _utc_date_to_loc(xdate_t* pxd)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	FILETIME ft_loc = { 0 };

	st.wYear = pxd->year;
	st.wMonth = pxd->mon;
	st.wDay = pxd->day;
	st.wHour = pxd->hour;
	st.wMinute = pxd->min;
	st.wSecond = pxd->sec;
	st.wMilliseconds = pxd->millsec;

	SystemTimeToFileTime(&st, &ft);
	FileTimeToLocalFileTime(&ft, &ft_loc);
	FileTimeToSystemTime(&ft_loc, &st);

	pxd->year = st.wYear;
	pxd->mon = st.wMonth;
	pxd->day = st.wDay;
	pxd->hour = st.wHour;
	pxd->min = st.wMinute;
	pxd->sec = st.wSecond;
	pxd->millsec = st.wMilliseconds;

	pxd->wday = st.wDayOfWeek;

	return 1;
}

dword_t _get_times()
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	lword_t dif1, dif2;
	double dif;

	st.wYear = 1970;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour =0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);
	dif1 = MAKELWORD(ft.dwLowDateTime, ft.dwHighDateTime) / 10000;
	
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	dif2 = MAKELWORD(ft.dwLowDateTime, ft.dwHighDateTime) / 10000;

	dif = (double)(dif2 - dif1) / 1000.0;

	return (dword_t)(dif);
}

clock_t _get_ticks()
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	lword_t dif1, dif2;
	double dif;

	st.wYear = 1970;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);
	dif1 = MAKELWORD(ft.dwLowDateTime, ft.dwHighDateTime);

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	dif2 = MAKELWORD(ft.dwLowDateTime, ft.dwHighDateTime);

	dif = (double)(dif2 - dif1) / 10000000.0;

	return (clock_t)(dif * CLOCKS_PER_SEC);
}


#define PERNANOSEC_SINCE_1582	(0x01B21DD213814000)

stamp_t _get_timestamp()
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);

	return (stamp_t)(MAKELWORD(ft.dwLowDateTime, ft.dwHighDateTime) / 10000);
}

void _utc_date_from_times(xdate_t* pxd, dword_t s)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	lword_t ss;

	st.wYear = 1970;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);

	ss = (lword_t)s * 10000000;
	ss += MAKELWORD(ft.dwLowDateTime, ft.dwHighDateTime);

	ft.dwLowDateTime = GETLDWORD(ss);
	ft.dwHighDateTime = GETHDWORD(ss);

	ZeroMemory((void*)&st, sizeof(SYSTEMTIME));
	FileTimeToSystemTime(&ft, &st);

	pxd->year = st.wYear;
	pxd->mon = st.wMonth;
	pxd->day = st.wDay;
	pxd->hour = st.wHour;
	pxd->min = st.wMinute;
	pxd->sec = st.wSecond;
	pxd->millsec = st.wMilliseconds;

	pxd->wday = st.wDayOfWeek;
}

void _utc_date_from_ticks(xdate_t* pxd, clock_t ts)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	double fs;
	lword_t ss;

	st.wYear = 1970;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);

	fs = ((double)ts / (double)CLOCKS_PER_SEC) * 10000000.0;
	ss = (lword_t)fs + MAKELWORD(ft.dwLowDateTime, ft.dwHighDateTime);

	ft.dwLowDateTime = GETLDWORD(ss);
	ft.dwHighDateTime = GETHDWORD(ss);

	FileTimeToSystemTime(&ft, &st);

	pxd->year = st.wYear;
	pxd->mon = st.wMonth;
	pxd->day = st.wDay;
	pxd->hour = st.wHour;
	pxd->min = st.wMinute;
	pxd->sec = st.wSecond;
	pxd->millsec = st.wMilliseconds;

	pxd->wday = st.wDayOfWeek;
}

void _utc_date_from_timestamp(xdate_t* pxd, stamp_t ts)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	lword_t ss;

	ss = (lword_t)ts * 10000;
	ft.dwLowDateTime = GETLDWORD(ss);
	ft.dwHighDateTime = GETHDWORD(ss);

	FileTimeToSystemTime(&ft, &st);

	pxd->year = st.wYear;
	pxd->mon = st.wMonth;
	pxd->day = st.wDay;
	pxd->hour = st.wHour;
	pxd->min = st.wMinute;
	pxd->sec = st.wSecond;
	pxd->millsec = st.wMilliseconds;

	pxd->wday = st.wDayOfWeek;
}

#endif //XDK_SUPPORT_DATE