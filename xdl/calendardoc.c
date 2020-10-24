/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc calendar document

	@module	calendardoc.c | implement file

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

#include "calendardoc.h"
#include "domdoc.h"

#include "xdlimp.h"
#include "xdlstd.h"


#ifdef XDL_SUPPORT_DOC

void default_calendar_attr(link_t_ptr ptr)
{
	set_calendar_style(ptr, _T("font-size:10;text-align:near;line-align:center;"));

	set_calendar_width(ptr, 150);
	set_calendar_height(ptr, 100);
	set_calendar_title_height(ptr, 16);
}

void default_calendar_daily_attr(link_t_ptr ilk)
{
	set_calendar_daily_style(ilk, _T("font-size:10.5;text-align:near;line-align:center;stroke-color:RGB(168,168,168);"));
}


link_t_ptr create_calendar_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_CALENDAR,-1);
	default_calendar_attr(ptr);

	return ptr;
}

void destroy_calendar_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_calendar_dailyset(link_t_ptr ptr)
{
	return ptr;
}

void clear_calendar_doc(link_t_ptr ptr)
{
	ptr = get_calendar_dailyset(ptr);

	delete_dom_child_nodes(ptr);
}

bool_t is_calendar_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_CALENDAR,-1,0) == 0)? 1 : 0;
}

bool_t is_calendar_daily(link_t_ptr ptr,link_t_ptr ilk)
{
	ptr = get_calendar_dailyset(ptr);

	return is_dom_child_node(ptr,ilk);
}

link_t_ptr insert_calendar_daily(link_t_ptr ptr,const tchar_t* sz_today)
{
	link_t_ptr ilk;
	xdate_t dt1, dt2;
	tchar_t sz_date[DATE_LEN];

	ptr = get_calendar_dailyset(ptr);

	parse_date(&dt1, sz_today);

	ilk = get_calendar_next_daily(ptr, LINK_FIRST);
	while (ilk)
	{
		parse_date(&dt2, get_dom_node_name_ptr(ilk));

		if (compare_date(&dt1, &dt2) > 0)
			break;

		ilk = get_calendar_next_daily(ptr, ilk);
	}

	ilk = insert_dom_node(ptr,((ilk)? ilk : LINK_LAST));
	format_date(&dt1, sz_date);
	set_calendar_daily_name(ilk, sz_date);

	default_calendar_daily_attr(ilk);

	return ilk;
}

void delete_calendar_daily(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

link_t_ptr get_calendar_daily(link_t_ptr ptr, const tchar_t* sz_today)
{
	link_t_ptr tlk;
	xdate_t dt1, dt2;

	parse_date(&dt1, sz_today);

	ptr = get_calendar_dailyset(ptr);
	tlk = get_calendar_next_daily(ptr, LINK_FIRST);
	while (tlk)
	{
		parse_date(&dt2, get_calendar_daily_name_ptr(tlk));

		if (compare_date(&dt1, &dt2) == 0)
			return tlk;

		tlk = get_calendar_next_daily(ptr, tlk);
	}

	return NULL;
}


int get_calendar_daily_count(link_t_ptr ptr)
{
	ptr = get_calendar_dailyset(ptr);

	return get_dom_child_node_count(ptr);
}

int get_calendar_daily_count_by_today(link_t_ptr ptr, const tchar_t* sz_today)
{
	link_t_ptr ilk;
	int count = 0;
	xdate_t dt1, dt2;

	parse_date(&dt1, sz_today);

	ptr = get_calendar_dailyset(ptr);
	ilk = get_calendar_next_daily(ptr, LINK_FIRST);
	while (ilk)
	{
		parse_date(&dt2, get_dom_node_name_ptr(ilk));

		if (compare_date(&dt1, &dt2) == 0)
			count++;

		ilk = get_calendar_next_daily(ptr, ilk);
	}

	return count;
}

int get_calendar_daily_selected_count(link_t_ptr ptr)
{
	link_t_ptr flk;
	int count = 0;

	flk = get_calendar_next_daily(ptr, LINK_FIRST);
	while (flk)
	{
		if (get_calendar_daily_selected(flk))
			count++;
		flk = get_calendar_next_daily(ptr, flk);
	}

	return count;
}

link_t_ptr get_calendar_next_daily(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_calendar_dailyset(ptr);

	if(pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_calendar_prev_daily(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_calendar_dailyset(ptr);

	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_prev_link(pos);
}



#endif //XDLDOC