/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc document set

	@module	taskdoc.h | interface file

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

#ifndef _TASKDOC_H
#define _TASKDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**********************************************************************************************************/
#define get_task_descriptoin_ptr(ptr)			get_dom_node_attr_ptr(ilk,ATTR_DESCRIPTION,-1)

#define set_task_description(ptr,token)			set_dom_node_attr(ilk,ATTR_DESCRIPTION,-1,token,-1)

#define get_task_next_execute_time_ptr(ptr)		get_dom_node_attr_ptr(ilk,ATTR_NEXTEXETIME,-1)

#define set_task_next_execute(ptr,token)		set_dom_node_attr(ilk,ATTR_NEXTEXETIME,-1,token,-1)

#define get_task_last_execute_time_ptr(ptr)		get_dom_node_attr_ptr(ilk,ATTR_LASTEXETIME,-1)

#define set_task_last_execute(ptr,token)		set_dom_node_attr(ilk,ATTR_LASTEXETIME,-1,token,-1)

#define get_task_schedule_effective_date_ptr(ilk)		get_dom_node_attr_ptr(ilk,ATTR_EFFECTDATE,-1)

#define set_task_schedule_effective_date(ilk,token)		set_dom_node_attr(ilk,ATTR_EFFECTDATE,-1,token,-1)

#define get_task_schedule_expiry_date_ptr(ilk)			get_dom_node_attr_ptr(ilk,ATTR_EXPIRYDATE,-1)

#define set_task_schedule_expiry_date(ilk,token)		set_dom_node_attr(ilk,ATTR_EXPIRYDATE,-1,token,-1)

#define get_task_schedule_start_time_ptr(ilk)			get_dom_node_attr_ptr(ilk,ATTR_STARTTIME,-1)

#define set_task_schedule_start_time(ilk,token)			set_dom_node_attr(ilk,ATTR_STARTTIME,-1,token,-1)

#define get_task_schedule_interval_months(ilk)			get_dom_node_attr_integer(ilk,ATTR_INTERMONTHS)

#define set_task_schedule_interval_months(ilk,n)		set_dom_node_attr_integer(ilk,ATTR_INTERMONTHS,n)

#define get_task_schedule_interval_weeks(ilk)			get_dom_node_attr_integer(ilk,ATTR_INTERWEEKS)

#define set_task_schedule_interval_weeks(ilk,n)			set_dom_node_attr_integer(ilk,ATTR_INTERWEEKS,n)

#define get_task_schedule_interval_days(ilk)			get_dom_node_attr_integer(ilk,ATTR_INTERDAYS)

#define set_task_schedule_interval_days(ilk,n)			set_dom_node_attr_integer(ilk,ATTR_INTERDAYS,n)

#define get_task_schedule_interval_hours(ilk)			get_dom_node_attr_integer(ilk,ATTR_INTERHOURS)

#define set_task_schedule_interval_hours(ilk,n)			set_dom_node_attr_integer(ilk,ATTR_INTERHOURS,n)

#define get_task_schedule_interval_minutes(ilk)			get_dom_node_attr_integer(ilk,ATTR_INTERMINUTES)

#define set_task_schedule_interval_minutes(ilk,n)		set_dom_node_attr_integer(ilk,ATTR_INTERMINUTES,n)

#define get_task_schedule_interval_seconds(ilk)			get_dom_node_attr_integer(ilk,ATTR_INTERSECONDS)

#define set_task_schedule_interval_seconds(ilk,n)		set_dom_node_attr_integer(ilk,ATTR_INTERSECONDS,n)

#define get_task_schedule_interval_counts(ilk)			get_dom_node_attr_integer(ilk,ATTR_INTERCOUNTS)

#define set_task_schedule_interval_counts(ilk,n)		set_dom_node_attr_integer(ilk,ATTR_INTERCOUNTS,n)

#define get_task_step_name_ptr(ilk)						get_dom_node_attr_ptr(ilk,ATTR_NAME,-1)

#define set_task_step_name(ilk,token)					set_dom_node_attr(ilk,ATTR_NAME,-1,token,-1)

#define get_task_step_turned_to_ptr(ilk)				get_dom_node_attr_ptr(ilk,ATTR_TURNEDTO,-1)

#define set_task_step_turned_to(ilk,token)				set_dom_node_attr(ilk,ATTR_TURNEDTO,-1,token,-1)

#define get_task_step_commandline_ptr(ilk)				get_dom_node_text_ptr(ilk)

#define get_task_step_commandline(ilk, buf, max)		get_dom_node_text(ilk, buf, max)

#define set_task_step_commandline(ilk, token, len)		set_dom_node_text(ilk, token, len)
/***********************************************************************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

XDL_API link_t_ptr create_task_doc(void);

XDL_API void destroy_task_doc(link_t_ptr ptr);

XDL_API bool_t is_task_doc(link_t_ptr ptr);

XDL_API void clear_task_doc(link_t_ptr ptr);

XDL_API link_t_ptr get_task_next_schedule_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr get_task_prev_schedule_node(link_t_ptr ptr, link_t_ptr pos);

XDL_API link_t_ptr insert_task_schedule_node(link_t_ptr slk, link_t_ptr pos);

XDL_API void delete_task_schedule_node(link_t_ptr ilk);

XDL_API link_t_ptr get_task_next_step_node(link_t_ptr slk, link_t_ptr pos);

XDL_API link_t_ptr get_task_prev_step_node(link_t_ptr slk, link_t_ptr pos);

XDL_API link_t_ptr insert_task_step_node(link_t_ptr slk, link_t_ptr pos);

XDL_API void delete_task_step_node(link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif	/*_TASKDOC_H*/