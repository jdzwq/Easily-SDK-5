/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc rich text doc document

	@module	richdoc.c | implement file

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

#include "richdoc.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_rich_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_RICH, -1);

	set_rich_width(ptr, PAPER_A4_WIDTH);
	set_rich_height(ptr, PAPER_A4_HEIGHT);

	return ptr;
}

void destroy_rich_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_rich_anchset(link_t_ptr ptr)
{
	return ptr;
}

bool_t is_rich_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_RICH, -1, 0) == 0) ? 1 : 0;
}

bool_t is_rich_anch(link_t_ptr ptr, link_t_ptr nlk)
{
	return is_dom_child_node(ptr, nlk);
}

void clear_rich_doc(link_t_ptr ptr)
{
	ptr = get_rich_anchset(ptr);

	delete_dom_child_nodes(ptr);
}

int get_rich_anch_count(link_t_ptr ptr)
{
	ptr = get_rich_anchset(ptr);

	return get_dom_child_node_count(ptr);
}

void reset_rich_doc(link_t_ptr ptr)
{
	link_t_ptr nlk;

	if (get_rich_dynamic(ptr))
	{
		clear_rich_doc(ptr);
	}
	else
	{
		nlk = get_rich_next_anch(ptr, LINK_FIRST);
		while (nlk)
		{
			set_rich_anch_text(nlk, NULL, 0);

			nlk = get_rich_next_anch(ptr, nlk);
		}
	}
}

link_t_ptr insert_rich_anch(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr nlk;

	ptr = get_rich_anchset(ptr);

	nlk = insert_dom_node(ptr, pos);
	set_dom_node_name(nlk, DOC_RICH_NODE, -1);

	return nlk;
}

void delete_rich_anch(link_t_ptr elk)
{
	delete_dom_node(elk);
}

link_t_ptr get_rich_anch(link_t_ptr ptr, const tchar_t* nname)
{
	link_t_ptr nlk;

	nlk = get_rich_next_anch(ptr, LINK_FIRST);
	while (nlk)
	{
		if (compare_text(get_rich_anch_name_ptr(nlk), -1, nname, -1, 0) == 0)
			return nlk;

		nlk = get_rich_next_anch(ptr, nlk);
	}

	return NULL;
}

link_t_ptr get_rich_next_anch(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_rich_anchset(ptr);

	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_rich_prev_anch(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_rich_anchset(ptr);

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_dom_prev_sibling_node(pos);
}

int format_rich_doc(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr ilk;
	int len, total = 0;
	const tchar_t *sz_name, *sz_title, *sz_text;

	ilk = get_rich_next_anch(ptr, LINK_FIRST);
	while (ilk)
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('^');
		}
		total++;

		sz_name = get_rich_anch_name_ptr(ilk);
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
			buf[total] = _T('|');
		}
		total++;

		sz_title = get_rich_anch_title_ptr(ilk);
		len = xslen(sz_title);
		if (len + total > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, sz_title, len);
		}
		total += len;

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('|');
		}
		total++;

		if (total + 2 > max)
			return total;

		if (buf)
		{
			if (get_rich_anch_fixed(ilk))
				buf[total] = _T('1');
			else
				buf[total] = _T('0');

			if (get_rich_anch_lined(ilk))
				buf[total+1] = _T('1');
			else
				buf[total+1] = _T('0');
		}
		total += 2;

		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('~');
		}
		total ++;

		sz_text = get_rich_anch_text_ptr(ilk);
		len = xslen(sz_text);
		if (len + total > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, sz_text, len);
		}
		total += len;

		ilk = get_rich_next_anch(ptr, ilk);
	}

	return total;
}

bool_t parse_rich_doc(link_t_ptr ptr, const tchar_t* buf, int len)
{
	link_t_ptr ilk;
	const tchar_t *tk_name, *tk_title, *tk_text;
	int len_name, len_title, len_text, total = 0;
	bool_t bfix = 0;
	bool_t blin = 0;

	if (len < 0)
		len = xslen(buf);

	clear_rich_doc(ptr);

	while (total < len)
	{
		while (*(buf + total) != _T('^') && *(buf + total) != _T('\0'))
		{
			total++;
		}

		if (*(buf + total) == _T('^'))
		{
			total++;
		}

		tk_name = buf + total;
		len_name = 0;
		while (*(tk_name + len_name) != _T('|') && *(tk_name + len_name) != _T('\0'))
		{
			len_name++;
		}
		total += len_name;

		if (*(buf + total) == _T('|'))
		{
			total++;
		}

		tk_title = buf + total;
		len_title = 0;
		while (*(tk_title + len_title) != _T('|') && *(tk_title + len_title) != _T('~') && *(tk_title + len_title) != _T('\0'))
		{
			len_title++;
		}
		total += len_title;

		if (*(buf + total) == _T('|'))
		{
			total++;
		}

		if (*(buf + total) != _T('~') && *(tk_title + len_title) != _T('\0'))
		{
			bfix = (*(buf + total) == _T('1')) ? 1 : 0;
			blin = (*(buf + total + 1) == _T('1')) ? 1 : 0;
		}
		else
		{
			bfix = 0;
			blin = 0;
		}

		while (*(buf + total) != _T('~'))
		{
			total++;
		}

		if (*(buf + total) == _T('~'))
		{
			total++;
		}

		tk_text = buf + total;
		len_text = 0;
		while (*(tk_text + len_text) != _T('^') && *(tk_text + len_text) != _T('\0'))
		{
			len_text++;
		}
		total += len_text;

		if (len_name)
		{
			ilk = insert_rich_anch(ptr, LINK_LAST);

			set_dom_node_attr(ilk, ATTR_NAME, -1, tk_name, len_name);
			set_dom_node_attr(ilk, ATTR_TITLE, -1, tk_title, len_title);
			set_dom_node_text(ilk, tk_text, len_text);
			if (bfix)
			{
				set_rich_anch_fixed(ilk, 1);
			}
			if (blin)
			{
				set_rich_anch_lined(ilk, 1);
			}
		}

		if (*(buf + total) == _T('\0'))
			break;
	}

	return 1;
}

bool_t is_rich_text_reserve(tchar_t ch)
{
	return (ch == _T('^') || ch == _T('|') || ch == _T('~')) ? 1 : 0;
}


#endif //XDL_SUPPORT_DOC
