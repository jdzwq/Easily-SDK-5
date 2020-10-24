/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc document set

	@module	taskdoc.c | implement file

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

#include "taskdoc.h"
#include "domdoc.h"

#include "xdlimp.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_task_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_TASK, -1);

	return ptr;
}

void destroy_task_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_task_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_TASK, -1, 0) == 0) ? 1 : 0;
}

void clear_task_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

link_t_ptr insert_task_schedule_node(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr plk;

	plk = insert_dom_node(ptr, pos);
	set_dom_node_name(plk, DOC_TASK_SCHEDULE, -1);

	return plk;
}

void delete_task_schedule_node(link_t_ptr slk)
{
	delete_dom_node(slk);
}

link_t_ptr get_task_next_schedule_node(link_t_ptr ptr, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_task_prev_schedule_node(link_t_ptr ptr, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_prev_link(pos);
}

link_t_ptr insert_task_step_node(link_t_ptr slk, link_t_ptr pos)
{
	link_t_ptr plk;

	plk = insert_dom_node(slk, pos);
	set_dom_node_name(plk, DOC_TASK_STEP, -1);

	return plk;
}

void delete_task_step_node(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

link_t_ptr get_task_next_step_node(link_t_ptr slk, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return get_dom_first_child_node(slk);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_task_prev_step_node(link_t_ptr slk, link_t_ptr pos)
{
	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(slk);
	else
		return get_prev_link(pos);
}



#endif //XDLDOC