/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc date document

	@module	impdate.h | interface file

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

#ifndef _IMPDATE_H
#define _IMPDATE_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_DATE

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION get_loc_date: get locale datetime.
@OUTPUT xdate_t* pxd: the datetime struct for returning.
@RETURN void: none.
*/
EXP_API void get_loc_date(xdate_t* pxd);

/*
@FUNCTION get_utc_date: get UTC datetime.
@OUTPUT xdate_t* pxd: the datetime struct for returning.
@RETURN void: none.
*/
EXP_API void get_utc_date(xdate_t* pxd);

/*
@FUNCTION mak_week_date: fill days of week according to year, month, day, or and hour, minut, second member.
@INOUTPUT xdate_t* pxd: the datetime struct for returning valid datetime, wday will be filled if member is valid.
@RETURN bool_t: if succeeds retur nonzero, fails return zero.
*/
EXP_API bool_t mak_week_date(xdate_t* pxd);

/*
@FUNCTION loc_date_to_utc: convert local date to utc date.
@INOUTPUT xdate_t* pxd: the datetime struct of locale date.
@RETURN bool_t: if succeeds retur nonzero, fails return zero.
*/
EXP_API bool_t loc_date_to_utc(xdate_t* pxd);

/*
@FUNCTION utc_date_to_loc: convert utc date to local date.
@INOUTPUT xdate_t* pxd: the datetime struct of utc date.
@RETURN bool_t: if succeeds retur nonzero, fails return zero.
*/
EXP_API bool_t utc_date_to_loc(xdate_t* pxd);

/*
@FUNCTION get_times: get seconds elapse since 1970-01-01 00:00:00.
@RETURN dword_t: if succeeds retur seconds, fails return zero.
*/
EXP_API dword_t get_times(void);

/*
@FUNCTION get_ticks: get system ticks elapse since 1970-01-01 00:00:00.
@RETURN lword_t: if succeeds retur ticks, fails return zero.
*/
EXP_API lword_t get_ticks(void);

/*
@FUNCTION get_timestamp: get system timestamp.
@RETURN lword_t: if succeeds retur timestamp, fails return zero.
*/
EXP_API lword_t get_timestamp(void);

/*
@FUNCTION utc_date_from_times: convert time in seconds to UTC datetime.
@OUTPUT xdate_t* pxd: datetime struct.
@INPUT dword_t s: time second value.
@RETURN void: none.
*/
EXP_API void utc_date_from_times(xdate_t* pxd, dword_t s);

/*
@FUNCTION utc_date_from_ticks: convert ticks to UTC datetime.
@OUTPUT xdate_t* pxd: datetime struct.
@INPUT lword_t ts: tick value.
@RETURN void: none.
*/
EXP_API void utc_date_from_ticks(xdate_t* pxd, lword_t ts);

/*
@FUNCTION utc_date_from_ticks: convert timestamp to UTC datetime.
@OUTPUT xdate_t* pxd: datetime struct.
@INPUT lword_t ts: timestamp value.
@RETURN void: none.
*/
EXP_API void utc_date_from_timestamp(xdate_t* pxd, lword_t ts);


#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_DATE*/

#endif /*_IMPDATE_H*/
