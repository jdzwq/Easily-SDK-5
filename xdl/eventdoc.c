/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc event document

	@module	eventdoc.c | implement file

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

#include "eventdoc.h"
#include "domdoc.h"

#include "xdlimp.h"
#include "xdlstd.h"


#ifdef XDL_SUPPORT_DOC

link_t_ptr create_event_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_EVENT, -1);

	return ptr;
}

void destroy_event_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_event_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_EVENT, -1, 0) == 0) ? 1 : 0;
}

void clear_event_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}


int	get_event_id(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_ID, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_id_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_ID, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_id(link_t_ptr ptr, const tchar_t* id, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_ID, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_ID, -1);
	}

	set_dom_node_text(nlk, id, len);
}

int	get_event_from(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_FROM, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_from_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_FROM, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_from(link_t_ptr ptr, const tchar_t* from, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_FROM, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_FROM, -1);
	}

	set_dom_node_text(nlk, from, len);
}

int	get_event_sender(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_SENDER, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_sender_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_SENDER, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_sender(link_t_ptr ptr, const tchar_t* sender, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_SENDER, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_SENDER, -1);
	}

	set_dom_node_text(nlk, sender, len);
}

int	get_event_send_time(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_SENDTIME, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_send_time_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_SENDTIME, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_send_time(link_t_ptr ptr, const tchar_t* send_time, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_SENDTIME, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_SENDTIME, -1);
	}

	set_dom_node_text(nlk, send_time, len);
}

int	get_event_code(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_CODE, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_code_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_CODE, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_code(link_t_ptr ptr, const tchar_t* code, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_CODE, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_CODE, -1);
	}

	set_dom_node_text(nlk, code, len);
}

int	get_event_describe(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_DESCRIBE, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_describe_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_DESCRIBE, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_describe(link_t_ptr ptr, const tchar_t* describe, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_DESCRIBE, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_DESCRIBE, -1);
	}

	set_dom_node_text(nlk, describe, len);
}

int	get_event_medium(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_MEDIUM, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_medium_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_MEDIUM, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_medium(link_t_ptr ptr, const tchar_t* medium, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_MEDIUM, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_MEDIUM, -1);
	}

	set_dom_node_text(nlk, medium, len);
}

int	get_event_content(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_CONTENT, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_content_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_CONTENT, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_content(link_t_ptr ptr, const tchar_t* content, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_CONTENT, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_CONTENT, -1);
		set_dom_node_cdata(nlk, 1);
	}

	set_dom_node_text(nlk, content, len);
}

int	get_event_to(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_TO, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_to_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_TO, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_to(link_t_ptr ptr, const tchar_t* to, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_TO, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_TO, -1);
	}

	set_dom_node_text(nlk, to, len);
}

int	get_event_receiptor(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTOR, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_receiptor_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTOR, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_receiptor(link_t_ptr ptr, const tchar_t* receiptor, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTOR, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_RECEIPTOR, -1);
	}

	set_dom_node_text(nlk, receiptor, len);
}

int	get_event_receipt_time(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTTIME, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_receipt_time_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTTIME, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_receipt_time(link_t_ptr ptr, const tchar_t* receipt_time, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTTIME, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_RECEIPTTIME, -1);
	}

	set_dom_node_text(nlk, receipt_time, len);
}

int	get_event_receipt_state(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTSTATE, -1);

	return (nlk) ? get_dom_node_text(nlk, buf, max) : 0;
}

const tchar_t*	get_event_receipt_state_ptr(link_t_ptr ptr)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTSTATE, -1);

	return (nlk) ? get_dom_node_text_ptr(nlk) : NULL;
}

void set_event_receipt_state(link_t_ptr ptr, const tchar_t* receipt_state, int len)
{
	link_t_ptr nlk;

	nlk = find_dom_node_by_name(ptr, 0, DOC_EVENT_RECEIPTSTATE, -1);
	if (!nlk)
	{
		nlk = insert_dom_node(ptr, LINK_FIRST);
		set_dom_node_name(nlk, DOC_EVENT_RECEIPTSTATE, -1);
	}

	set_dom_node_text(nlk, receipt_state, len);
}


#endif /*XDL_SUPPORT_DOC*/
