/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc date document

	@module	impdate.c | implement file

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

#include "impdate.h"
#include "xdlinit.h"
#include "xdlimp.h"

#ifdef XDK_SUPPORT_DATE

void get_loc_date(xdate_t* pxd)
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_get_loc_date)(pxd);
}

void get_utc_date(xdate_t* pxd)
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_get_utc_date)(pxd);
}

bool_t mak_week_date(xdate_t* pxd)
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_mak_week_date)(pxd);
}

bool_t loc_date_to_utc(xdate_t* pxd)
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_loc_date_to_utc)(pxd);
}

bool_t utc_date_to_loc(xdate_t* pxd)
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_utc_date_to_loc)(pxd);
}

dword_t get_times()
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (*pif->pf_get_times)();
}

lword_t get_ticks()
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (lword_t)(*pif->pf_get_ticks)();
}

lword_t get_timestamp()
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	return (lword_t)(*pif->pf_get_timestamp)();
}

void utc_date_from_times(xdate_t* pxd, dword_t s)
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_utc_date_from_times)(pxd, s);
}

void utc_date_from_ticks(xdate_t* pxd, lword_t ts)
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_utc_date_from_ticks)(pxd, (clock_t)ts);
}

void utc_date_from_timestamp(xdate_t* pxd, lword_t ts)
{
	if_date_t* pif;

	pif = PROCESS_DATE_INTERFACE;

	XDL_ASSERT(pif != NULL);

	(*pif->pf_utc_date_from_timestamp)(pxd, (stamp_t)ts);
}

#else
#include <time.h>

void get_loc_date(xdate_t* pxd)
{
	time_t t;
	struct tm *pts;

	t = time(NULL);
	pts = localtime(&t);

	pxd->year = pts->tm_year + 1900;
	pxd->mon = pts->tm_mon + 1;
	pxd->day = pts->tm_mday;
	pxd->wday = pts->tm_wday;
	pxd->hour = pts->tm_hour;
	pxd->min = pts->tm_min;
	pxd->sec = pts->tm_sec;
}

bool_t mak_loc_date(xdate_t* pxd)
{
	struct tm ts;
	struct tm* pts;
	time_t t;

	ts.tm_year = pxd->year - 1900;
	ts.tm_mon = pxd->mon - 1;
	ts.tm_mday = pxd->day;
	ts.tm_hour = pxd->hour;
	ts.tm_min = pxd->min;
	ts.tm_sec = pxd->sec;

	t = mktime(&ts);
	if (t>=0)
	{
		pts = gmtime(&t);
		pxd->wday = pts->tm_wday;
		return 1;
	}

	return 0;
}

void get_utc_date(xdate_t* pxd)
{
	time_t t;
	struct tm *pts;

	t = time(NULL);
	pts = gmtime(&t);

	pxd->year = pts->tm_year + 1900;
	pxd->mon = pts->tm_mon + 1;
	pxd->day = pts->tm_mday;
	pxd->wday = pts->tm_wday;
	pxd->hour = pts->tm_hour;
	pxd->min = pts->tm_min;
	pxd->sec = pts->tm_sec;
}

bool_t mak_utc_date(xdate_t* pxd)
{
	struct tm ts;
	struct tm* pts;
	time_t t;

	ts.tm_year = pxd->year - 1900;
	ts.tm_mon = pxd->mon - 1;
	ts.tm_mday = pxd->day;
	ts.tm_hour = pxd->hour;
	ts.tm_min = pxd->min;
	ts.tm_sec = pxd->sec;

	t = mktime(&ts);
	if (t>=0)
	{
		pts = gmtime(&t);
		pxd->wday = pts->tm_wday;
		return 1;
	}

	return 0;
}

dword_t get_times()
{
	time_t t;

	t = time(NULL);

	return (dword_t)t;
}

dword_t get_ticks()
{
	clock_t c;

	c = clock();

	return (dword_t)c;
}
#endif //XDK_SUPPORT_DATE
