/***********************************************************************
	Easily xdl v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc news document

	@module	newsdoc.c | news document implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
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

#include "newsdoc.h"
#include "xdlstd.h"


#ifdef XDL_SUPPORT_DOC

link_t_ptr create_news_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_NEWS,-1);

	return ptr;
}

void destroy_news_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_news_doc(link_t_ptr ptr)
{
	return  (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_NEWS, -1, 1) == 0) ? 1 : 0;
}

link_t_ptr get_news_title_node(link_t_ptr ptr)
{
	link_t_ptr nlk;
	const tchar_t* name;

	name = get_dom_node_name_ptr(ptr);

	if(compare_text(name,-1,DOC_NEWS,-1,1) != 0)
		return NULL;

	nlk = get_dom_first_child_node(ptr);
	while(nlk)
	{
		name = get_dom_node_name_ptr(nlk);
		if(compare_text(name,-1,DOC_NEWS_TITLE,-1,1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return nlk;
}

link_t_ptr get_news_text_node(link_t_ptr ptr)
{
	link_t_ptr nlk;
	const tchar_t* name;

	name = get_dom_node_name_ptr(ptr);

	if (compare_text(name, -1, DOC_NEWS, -1, 1) != 0)
		return NULL;

	nlk = get_dom_first_child_node(ptr);
	while (nlk)
	{
		name = get_dom_node_name_ptr(nlk);
		if (compare_text(name, -1, DOC_NEWS_TEXT, -1, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return nlk;
}


link_t_ptr get_news_attachment_node(link_t_ptr ptr)
{
	link_t_ptr nlk;
	const tchar_t* name;

	name = get_dom_node_name_ptr(ptr);

	if (compare_text(name, -1, DOC_NEWS, -1, 1) != 0)
		return NULL;

	nlk = get_dom_first_child_node(ptr);
	while (nlk)
	{
		name = get_dom_node_name_ptr(nlk);
		if (compare_text(name, -1, DOC_NEWS_ATTACHMENT, -1, 1) == 0)
			break;

		nlk = get_dom_next_sibling_node(nlk);
	}

	return nlk;
}

#endif //_NEWSDOC_H
