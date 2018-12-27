/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc time system call document

	@module	_if_date.c | time system call windows implement file

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

bool_t _mak_loc_date(xdate_t* pxd)
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

void _get_utc_date(xdate_t* pxd)
{
	SYSTEMTIME st = {0};

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

bool_t _mak_utc_date(xdate_t* pxd)
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

clock_t _get_times()
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	size_t dif1, dif2;

	st.wYear = 1970;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour =0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);
	dif1 = MAKESIZE(ft.dwLowDateTime, ft.dwHighDateTime);
	
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	dif2 = MAKESIZE(ft.dwLowDateTime, ft.dwHighDateTime);

	return (clock_t)((dif2 - dif1) / 10000);
}

clock_t _get_ticks()
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };
	size_t dif1, dif2;

	st.wYear = 1970;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);
	dif1 = MAKESIZE(ft.dwLowDateTime, ft.dwHighDateTime);

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	dif2 = MAKESIZE(ft.dwLowDateTime, ft.dwHighDateTime);

	return (clock_t)(dif2 - dif1);
}


#define PERNANOSEC_SINCE_1582	(0x01B21DD213814000)

stamp_t _get_timestamp()
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);

	return (stamp_t)MAKESIZE(ft.dwLowDateTime, ft.dwHighDateTime);
}

void _utc_date_from_times(xdate_t* pxd, stamp_t ts)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };

	st.wYear = 1970;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);

	ts *= 10000;
	ts += (stamp_t)MAKESIZE(ft.dwLowDateTime, ft.dwHighDateTime);
	ft.dwLowDateTime = GETSIZEL(ts);
	ft.dwHighDateTime = GETSIZEH(ts);

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

void _utc_date_from_ticks(xdate_t* pxd, stamp_t ts)
{
	return _utc_date_from_times(pxd, ts / 10000);
}

void _utc_date_from_timestamp(xdate_t* pxd, stamp_t ts)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };

	ft.dwLowDateTime = GETSIZEL(ts);
	ft.dwHighDateTime = GETSIZEH(ts);

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