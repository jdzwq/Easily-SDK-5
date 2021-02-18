/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc time system call document

	@module	_if_date.c | time system call macos implement file

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

#include <time.h>

void _get_loc_date(xdate_t* pxd)
{
	time_t timep;
	struct tm *p;

	time(&timep);
	p = localtime(&timep);

	pxd->year = 1900 + p->tm_year;
	pxd->mon = 1 + p->tm_mon;
	pxd->day = p->tm_mday;
	pxd->hour = p->tm_hour;
	pxd->min = p->tm_min;
	pxd->sec = p->tm_sec;
	pxd->millsec = 0;

	pxd->wday = p->tm_wday;
}

void _get_utc_date(xdate_t* pxd)
{
	time_t timep;
	struct tm *p;

	time(&timep);
	p = gmtime(&timep);

	pxd->year = 1900 + p->tm_year;
	pxd->mon = 1 + p->tm_mon;
	pxd->day = p->tm_mday;
	pxd->hour = p->tm_hour;
	pxd->min = p->tm_min;
	pxd->sec = p->tm_sec;
	pxd->millsec = 0;

	pxd->wday = p->tm_wday;
}

bool_t _mak_utc_week(xdate_t* pxd)
{
	time_t timep;
	struct tm t = { 0 };
	struct tm *p;

	t.tm_year = pxd->year - 1900;
	t.tm_mon = pxd->mon - 1;
	t.tm_mday = pxd->day;
	t.tm_hour = pxd->hour;
	t.tm_min = pxd->min;
	t.tm_sec = pxd->sec;

	timep = mktime(&t);
	p = gmtime(&timep);

	pxd->wday = p->tm_wday;

	return 1;
}

bool_t _mak_loc_week(xdate_t* pxd)
{
	time_t timep;
	struct tm t = { 0 };
	struct tm *p;

	t.tm_year = pxd->year - 1900;
	t.tm_mon = pxd->mon - 1;
	t.tm_mday = pxd->day;
	t.tm_hour = pxd->hour;
	t.tm_min = pxd->min;
	t.tm_sec = pxd->sec;

	timep = mktime(&t);
	p = gmtime(&timep);

	pxd->wday = p->tm_wday;

	return 1;
}

bool_t _loc_date_to_utc(xdate_t* pxd)
{
	time_t timep;
	struct tm t = { 0 };
	struct tm *p;

	t.tm_year = pxd->year - 1900;
	t.tm_mon = pxd->mon - 1;
	t.tm_mday = pxd->day;
	t.tm_hour = pxd->hour;
	t.tm_min = pxd->min;
	t.tm_sec = pxd->sec;

	timep = mktime(&t);
	p = localtime(&timep);

	pxd->year = 1900 + p->tm_year;
	pxd->mon = 1 + p->tm_mon;
	pxd->day = p->tm_mday;
	pxd->hour = p->tm_hour;
	pxd->min = p->tm_min;
	pxd->sec = p->tm_sec;
	pxd->millsec = 0;

	pxd->wday = p->tm_wday;

	return 1;
}

bool_t _utc_date_to_loc(xdate_t* pxd)
{
	return 0;
}

dword_t _get_times()
{
	struct tm t = { 0 };
	time_t time1,time2;

	t.tm_year = 70;
	t.tm_mon = 0;
	t.tm_mday = 1;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;

	time1 = mktime(&t);
	time2 = time(NULL);

	return (lword_t)(difftime(time1, time2));
}

void _utc_date_from_times(xdate_t* pxd, dword_t s)
{
    struct tm t = { 0 };
	time_t time1;
	struct tm *p;

	t.tm_year = 70;
	t.tm_mon = 0;
	t.tm_mday = 1;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;

	time1 = mktime(&t) + s;

	p = gmtime(&time1);

	pxd->year = 1900 + p->tm_year;
	pxd->mon = 1 + p->tm_mon;
	pxd->day = p->tm_mday;
	pxd->hour = p->tm_hour;
	pxd->min = p->tm_min;
	pxd->sec = p->tm_sec;
	pxd->millsec = 0;

	pxd->wday = p->tm_wday;
}

clock_t _get_ticks()
{
	struct tm t = { 0 };
	time_t time1,time2;

	t.tm_year = 70;
	t.tm_mon = 0;
	t.tm_mday = 1;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;

	time1 = mktime(&t);
	time2 = time(NULL);

	return (clock_t)(difftime(time1, time2) * CLOCKS_PER_SEC);
}

void _utc_date_from_ticks(xdate_t* pxd, clock_t ts)
{
   struct tm t = { 0 };
	time_t time1;
	struct tm *p;

	t.tm_year = 70;
	t.tm_mon = 0;
	t.tm_mday = 1;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;

	time1 = mktime(&t) + ts / CLOCKS_PER_SEC;

	p = gmtime(&time1);

	pxd->year = 1900 + p->tm_year;
	pxd->mon = 1 + p->tm_mon;
	pxd->day = p->tm_mday;
	pxd->hour = p->tm_hour;
	pxd->min = p->tm_min;
	pxd->sec = p->tm_sec;
	pxd->millsec = 0;

	pxd->wday = p->tm_wday;
}

stamp_t _get_timestamp()
{
    struct timeval t = {0};
    
    gettimeofday (&t, NULL);
    
    return (stamp_t)(t.tv_sec * 1000 + t.tv_usec / 1000);
}

void _utc_date_from_timestamp(xdate_t* pxd, stamp_t ts)
{
    struct tm t = { 0 };
	time_t time1;
	struct tm *p;

	t.tm_year = 70;
	t.tm_mon = 0;
	t.tm_mday = 1;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;

	time1 = mktime(&t) + (dword_t)(ts / 1000);

	p = localtime(&time1);

	pxd->year = 1900 + p->tm_year;
	pxd->mon = 1 + p->tm_mon;
	pxd->day = p->tm_mday;
	pxd->hour = p->tm_hour;
	pxd->min = p->tm_min;
	pxd->sec = p->tm_sec;
	pxd->millsec = (unsigned short)(ts % 1000);

	pxd->wday = p->tm_wday;
}

#endif //XDK_SUPPORT_DATE