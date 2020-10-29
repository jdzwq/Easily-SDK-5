/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc service defination document

	@module	srvdef.h | service definition interface file

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


#ifndef _SRVDEF_H
#define _SRVDEF_H

#include <xdl.h>

#define XSERVICE_ROOT			_T("XSERVICE_ROOT")
#define XSERVICE_DATA			_T("XSERVICE_DATA")

#define XPORTM_WAIT_TIMO         (1000)
#define XPORTM_PIPE_NAME		_T("xportm")

#define XPORTD_PORT_TYPE_HTTP		_T("HTTP")
#define XPORTD_PORT_TYPE_TCP		_T("TCP")
#define XPORTD_PORT_TYPE_UDP		_T("UDP")
#define XPORTD_PORT_TYPE_PNP		_T("PNP")

#define IS_THREAD_MODE(mode)		(compare_text(mode,-1,_T("thread"),-1,1) == 0)


#define XTIMERD_STATE_STOPPED	0
#define XTIMERD_STATE_RUNNING	1
#define XTIMERD_STATE_PAUSED	2

#define XTIMERD_ATTR_SCHEDULE	_T("schedule")
#define XTIMERD_ATTR_MODE		_T("mode")
#define XTIMERD_ATTR_MODULE		_T("module")
#define XTIMERD_ATTR_TASK		_T("task")
#define XTIMERD_ATTR_FIRSTTIME	_T("first-time")
#define XTIMERD_ATTR_LASTTIME	_T("last-time")
#define XTIMERD_ATTR_MONTHHINT	_T("month-hint")
#define XTIMERD_ATTR_WEEKHINT	_T("week-hint")
#define XTIMERD_ATTR_DAYHINT	_T("day-hint")
#define XTIMERD_ATTR_HOURHINT	_T("hour-hint")
#define XTIMERD_ATTR_MINUTEHINT	_T("minute-hint")
#define XTIMERD_ATTR_SECONDHINT	_T("second-hint")

typedef struct _timer_hint{
	xdate_t fdate;
	xdate_t ldate;

	int n_mon;
	int* p_mon;
	int n_week;
	int* p_week;
	int n_day;
	int* p_day;
	int n_hour;
	int* p_hour;
	int n_min;
	int* p_min;
	int n_sec;
	int* p_sec;
}timer_hint;

#endif //_SRVDEF_H