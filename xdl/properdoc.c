/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc property document

	@module	properdoc.c | implement file

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

#include "properdoc.h"
#include "domdoc.h"

#include "xdlimp.h"
#include "xdlstd.h"


#ifdef XDL_SUPPORT_DOC

void default_proper_attr(link_t_ptr ptr)
{
	set_proper_style(ptr, _T("font-size:10.5;text-align:near;line-align:center;"));
	set_proper_icon_span(ptr,DEF_ATTR_ICON_SPAN); 
	set_proper_item_span(ptr,25); 
	set_proper_item_height(ptr, DEF_ITEM_HEIGHT);

	set_proper_shape(ptr,ATTR_SHAPE_RECT); 
}

link_t_ptr create_proper_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_PROPER,-1);
	default_proper_attr(ptr);
	return ptr;
}

void destroy_proper_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

bool_t is_proper_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_PROPER,-1,0) == 0)? 1 : 0;
}

bool_t is_proper_section(link_t_ptr ptr, link_t_ptr slk)
{
	return is_dom_child_node(ptr, slk);
}

bool_t is_proper_entity(link_t_ptr ptr,link_t_ptr elk)
{
	link_t_ptr slk;

	slk = get_next_section(ptr, LINK_FIRST);
	while (slk)
	{
		if (is_dom_child_node(slk, elk))
			return 1;

		slk = get_next_section(ptr, slk);
	}

	return 0;
}

void clear_proper_doc(link_t_ptr ptr)
{
	link_t_ptr slk;

	while(slk = get_next_section(ptr,LINK_FIRST))
		delete_section(slk);
}

link_t_ptr insert_section(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr slk;

	slk = insert_dom_node(ptr,pos);
	set_dom_node_name(slk,DOC_PROPER_SECTION,-1);

	return slk;
}

void delete_section(link_t_ptr slk)
{
	link_t_ptr elk;

	while(elk = get_next_entity(slk,LINK_FIRST))
		delete_entity(elk);

	delete_dom_node(slk);
}

link_t_ptr proper_doc_from_section(link_t_ptr slk)
{
	return get_dom_parent_node(slk);
}

link_t_ptr get_section(link_t_ptr ptr,const tchar_t* szsec,int seclen)
{
	link_t_ptr slk;
	
	if(is_null(szsec) || !seclen)
		return NULL;
	
	if(seclen < 0)
		seclen = xslen(szsec);

	slk = get_next_section(ptr,LINK_FIRST);
	while(slk)
	{
		if(0 == compare_text(get_section_name_ptr(slk),-1,szsec,seclen,0))
			break;
		slk = get_next_section(ptr,slk);

	}
	return slk;
}

link_t_ptr get_next_section(link_t_ptr ptr,link_t_ptr pos)
{
	if(pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_prev_section(link_t_ptr ptr,link_t_ptr pos)
{
	if(pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else if(pos == LINK_FIRST)
		return NULL;
	else
		return get_prev_link(pos);
}

link_t_ptr insert_entity(link_t_ptr slk,link_t_ptr pos)
{
	link_t_ptr elk;

	elk = insert_dom_node(slk,pos);
	set_dom_node_name(elk,DOC_PROPER_ENTITY,-1);
	return elk;
}

void delete_entity(link_t_ptr elk)
{
	delete_dom_node(elk);
}

link_t_ptr section_from_entity(link_t_ptr elk)
{
	return get_dom_parent_node(elk);
}

link_t_ptr get_entity(link_t_ptr slk,const tchar_t* szent,int entlen)
{
	link_t_ptr elk;

	if(is_null(szent) || !entlen)
		return NULL;
	
	if(entlen < 0)
		entlen = xslen(szent);

	elk = get_next_entity(slk,LINK_FIRST);
	while(elk)
	{
		if(0 == compare_text(get_entity_name_ptr(elk),-1,szent,entlen,0))
			break;

		elk = get_next_entity(slk,elk);
	}
	return elk;
}

link_t_ptr get_next_entity(link_t_ptr slk,link_t_ptr pos)
{
	if(pos == LINK_FIRST)
		return get_dom_first_child_node(slk);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_prev_entity(link_t_ptr slk,link_t_ptr pos)
{
	if(pos == LINK_LAST)
		return get_dom_last_child_node(slk);
	else if(pos == LINK_FIRST)
		return NULL;
	else
		return get_prev_link(pos);
}

link_t_ptr	write_proper(link_t_ptr ptr,const tchar_t* szSec, int seclen, const tchar_t* szEnt, int entlen,const tchar_t* szVal, int vallen)
{
	link_t_ptr slk;
	link_t_ptr elk;
	
	if(is_null(szSec) || seclen == 0 || is_null(szEnt) || entlen == 0)
		return NULL;

	slk = get_section(ptr,szSec,seclen);
	if(!slk)	/*if no section with same name, then add new section*/
	{
		slk = insert_section(ptr,LINK_LAST);
		set_section_name(slk,szSec,seclen);

		elk = insert_entity(slk,LINK_LAST);
		set_entity_name(elk,szEnt,entlen);
		set_entity_value(elk,szVal,vallen);

		return elk;
	}else	/*else the section with same name already in properdata*/
	{
		elk = get_entity(slk,szEnt,entlen);
		if(!elk)	/*if no eitity with same name, then add new*/
		{
			elk = insert_entity(slk,LINK_LAST);
			set_entity_name(elk,szEnt,entlen);
			set_entity_value(elk,szVal,vallen);
		}else	/*else replace the entity value*/
		{
			set_entity_value(elk,szVal,vallen);
		}
		return elk;
	}
}

int	read_proper(link_t_ptr ptr,const tchar_t* szSec,int seclen,const tchar_t* szEnt,int entlen,tchar_t* buf,int max)
{
	link_t_ptr slk,elk;

	slk = get_section(ptr,szSec,seclen);
	if(!slk)
		return 0;

	elk = get_entity(slk,szEnt,entlen);
	if(!elk)
		return 0;

	return get_entity_value(elk,buf,max);
}

const tchar_t* get_entity_options_text_ptr(link_t_ptr ent)
{
	const tchar_t *sz_text, *sz_opt;

	sz_text = get_entity_value_ptr(ent);

	sz_opt = get_dom_node_options_text_ptr(ent, sz_text, -1);
	return (is_null(sz_opt)) ? sz_text : sz_opt;
}

const tchar_t* get_proper_ptr(link_t_ptr ptr,const tchar_t* szSec,const tchar_t* szEnt)
{
	link_t_ptr slk,elk;

	slk = get_section(ptr,szSec,-1);
	if(!slk)
		return NULL;

	elk = get_entity(slk,szEnt,-1);
	if(!elk)
		return NULL;

	return get_entity_value_ptr(elk);
}

link_t_ptr set_proper_boolean(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent, bool_t b)
{
	return write_proper(ptr, sec, -1, ent, -1, ((b)? _T("1") : _T("0")), -1);
}

bool_t get_proper_boolean(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent)
{
	return (compare_text(get_proper_ptr(ptr, sec, ent), -1, _T("1"), -1, 0) == 0) ? 1 : 0;
}

link_t_ptr set_proper_integer(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent,int n)
{
	tchar_t token[NUM_LEN + 1];

	xsprintf(token,_T("%d"),n);
	return write_proper(ptr,sec,-1,ent,-1,token,-1);
}

int get_proper_integer(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent)
{
	return xstol(get_proper_ptr(ptr,sec,ent));
}

link_t_ptr set_proper_float(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent, float n)
{
	tchar_t token[NUM_LEN + 1] = { 0 };

	ftoxs(n, token, NUM_LEN);
	return write_proper(ptr, sec, -1, ent, -1, token, -1);
}

float get_proper_float(link_t_ptr ptr, const tchar_t* sec, const tchar_t* ent)
{
	return xstof(get_proper_ptr(ptr, sec, ent));
}

link_t_ptr set_proper_numeric(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent,double n)
{
	tchar_t token[NUM_LEN + 1] = { 0 };

	numtoxs(n, token, NUM_LEN); 
	return write_proper(ptr,sec,-1,ent,-1,token,-1);
}

double get_proper_numeric(link_t_ptr ptr,const tchar_t* sec,const tchar_t* ent)
{
	return xstonum(get_proper_ptr(ptr,sec,ent));
}

#endif //XDLDOC
