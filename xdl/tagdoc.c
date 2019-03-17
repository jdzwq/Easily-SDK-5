/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tag text doc document

	@module	tagdoc.c | tag doc implement file

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

#include "tagdoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_tag_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_TAG, -1);

	return ptr;
}

void destroy_tag_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_tag_jointset(link_t_ptr ptr)
{
	return ptr;
}

bool_t is_tag_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_TAG, -1, 0) == 0) ? 1 : 0;
}

bool_t is_tag_joint(link_t_ptr ptr, link_t_ptr nlk)
{
	return is_dom_child_node(ptr, nlk);
}

void clear_tag_doc(link_t_ptr ptr)
{
	ptr = get_tag_jointset(ptr);

	delete_dom_child_nodes(ptr);
}

int get_tag_joint_count(link_t_ptr ptr)
{
	ptr = get_tag_jointset(ptr);

	return get_dom_child_node_count(ptr);
}

link_t_ptr insert_tag_joint(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr nlk;

	ptr = get_tag_jointset(ptr);

	nlk = insert_dom_node(ptr, pos);

	set_dom_node_name(nlk, DOC_TAG_TEXT, -1);

	return nlk;
}

void delete_tag_joint(link_t_ptr elk)
{
	delete_dom_node(elk);
}

link_t_ptr merge_tag_joint(link_t_ptr nlk)
{
	link_t_ptr pre;
	int n;
	tchar_t* buf;

	pre = get_prev_link(nlk);
	if (!pre)
		return NULL;

	n = xslen(get_tag_joint_text_ptr(pre)) + xslen(get_tag_joint_text_ptr(nlk));

	if (n)
	{
		buf = detach_dom_node_text(pre);
		buf = xsrealloc(buf, n + 1);
		xscat(buf, get_tag_joint_text_ptr(nlk));

		attach_dom_node_text(pre, buf);
	}

	delete_tag_joint(nlk);

	return pre;
}

link_t_ptr get_tag_joint(link_t_ptr ptr, const tchar_t* nname)
{
	link_t_ptr nlk;

	nlk = get_tag_next_joint(ptr, LINK_FIRST);
	while (nlk)
	{
		if (compare_text(get_tag_joint_name_ptr(nlk), -1, nname, -1, 0) == 0)
			return nlk;

		nlk = get_tag_next_joint(ptr, nlk);
	}

	return NULL;
}

link_t_ptr get_tag_next_joint(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_tag_jointset(ptr);

	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_tag_prev_joint(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_tag_jointset(ptr);

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_dom_prev_sibling_node(pos);
}

void clear_tag_joint_tag(link_t_ptr elk)
{
	set_tag_joint_name(elk, DOC_TAG_TEXT);
}


int format_tag_doc(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr ilk;
	int len, total = 0;
	const tchar_t *sz_name, *sz_text;

	ilk = get_tag_next_joint(ptr, LINK_FIRST);
	while (ilk)
	{
		if (!is_tag_text_joint(ilk))
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = _T('[');
			}
			total++;

			sz_name = get_tag_joint_name_ptr(ilk);
			len = xslen(sz_name);
			if (len + total > max)
				return total;

			if (buf)
			{
				xsncpy(buf + total, sz_name, len);
			}
			total += len;

			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = _T('~');
			}
			total++;
		}

		sz_text = get_tag_joint_text_ptr(ilk);
		len = xslen(sz_text);
		if (len + total > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, sz_text, len);
		}
		total += len;

		if (!is_tag_text_joint(ilk))
		{
			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = _T(']');
			}
			total++;
		}

		ilk = get_tag_next_joint(ptr, ilk);
	}

	return total;
}

bool_t parse_tag_doc(link_t_ptr ptr, const tchar_t* buf, int len)
{
	link_t_ptr ilk;
	const tchar_t *tk_name, *tk_text;
	int len_name, len_text, total = 0;

	if (len < 0)
		len = xslen(buf);

	clear_tag_doc(ptr);

	while (total < len)
	{
		if (*(buf + total) == _T('['))
		{
			total++;

			tk_name = buf + total;
			len_name = 0;

			while (*(buf + total) != _T('~') && *(buf + total) != _T(']') && *(buf + total) != _T('\0'))
			{
				total++;
				len_name++;
			}

			if (*(buf + total) == _T('~'))
			{
				total++;
			}
		}
		else
		{
			tk_name = NULL;
			len_name = 0;
		}

		tk_text = buf + total;
		len_text = 0;

		while (*(buf + total) != _T(']') && *(buf + total) != _T('[') && *(buf + total) != _T('\0'))
		{
			total++;
			len_text++;
		}

		if (*(buf + total) == _T(']'))
		{
			total++;
		}

		if (len_name || len_text)
		{
			ilk = insert_tag_joint(ptr, LINK_LAST);

			if (len_name)
				set_dom_node_name(ilk, tk_name, len_name);

			set_dom_node_text(ilk, tk_text, len_text);
		}

		if (*(buf + total) == _T('\0'))
			break;
	}

	return 1;
}

bool_t is_tag_text_reserve(tchar_t ch)
{
	return (ch == _T('[') || ch == _T(']') || ch == _T('~')) ? 1 : 0;
}

#endif //XDL_SUPPORT_DOC