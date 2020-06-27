/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc statis document

	@module	statisdoc.c | implement file

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

#include "statisdoc.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_statis_attr(link_t_ptr ptr) 
{
	set_statis_style(ptr, _T("font-size:10.5;text-align:center;line-align:center;"));

	set_statis_width(ptr, DEF_PAPER_WIDTH);
	set_statis_height(ptr, DEF_PAPER_HEIGHT);

	set_statis_title_height(ptr,DEF_TEXT_HEIGHT); 

	set_statis_xaxbar_width(ptr,25); 
	set_statis_xaxbar_height(ptr, 8);
	set_statis_yaxbar_height(ptr, DEF_ITEM_HEIGHT);
	set_statis_yaxbar_width(ptr, 25);
}

void default_gax_attr(link_t_ptr glk)
{
	set_gax_midd(glk, 0);
	set_gax_step(glk, 1);
	set_gax_statis_type(glk, ATTR_STATIS_TYPE_LINE);
}

void default_yax_attr(link_t_ptr ylk) 
{
	set_yax_color(ylk, GDI_ATTR_RGB_SOFTWHITE);
}

link_t_ptr get_statis_xaxset(link_t_ptr ptr)
{
	link_t_ptr plk;

	plk = get_dom_first_child_node(ptr);
	while(plk)
	{
		if(0 == xscmp(get_dom_node_name_ptr(plk),DOC_STATIS_XAXSET))
			return plk;

		plk = get_dom_next_sibling_node(plk);
	}
	
	return NULL;
}

link_t_ptr get_statis_yaxset(link_t_ptr ptr)
{
	link_t_ptr plk;

	plk = get_dom_first_child_node(ptr);
	while(plk)
	{
		if(0 == xscmp(get_dom_node_name_ptr(plk),DOC_STATIS_YAXSET))
			return plk;

		plk = get_dom_next_sibling_node(plk);
	}
	
	return NULL;
}

link_t_ptr get_statis_gaxset(link_t_ptr ptr)
{
	link_t_ptr plk;

	plk = get_dom_first_child_node(ptr);
	while (plk)
	{
		if (0 == xscmp(get_dom_node_name_ptr(plk), DOC_STATIS_GAXSET))
			return plk;

		plk = get_dom_next_sibling_node(plk);
	}

	return NULL;
}

link_t_ptr create_statis_doc()
{
	link_t_ptr ptr, glk_set,ylk_set,xlk_set;

	ptr = create_dom_doc();

	set_dom_node_name(ptr,DOC_STATIS,-1);
	default_statis_attr(ptr);

	glk_set = insert_dom_node(ptr, LINK_LAST);
	set_dom_node_name(glk_set, DOC_STATIS_GAXSET, -1);

	ylk_set = insert_dom_node(ptr,LINK_LAST);
	set_dom_node_name(ylk_set,DOC_STATIS_YAXSET,-1);

	xlk_set = insert_dom_node(ptr,LINK_LAST);
	set_dom_node_name(xlk_set,DOC_STATIS_XAXSET,-1);

	return ptr;
}

void destroy_statis_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_statis_doc(link_t_ptr ptr)
{
	clear_statis_gaxset(ptr);
	clear_statis_yaxset(ptr);
	clear_statis_xaxset(ptr);
}

void clear_statis_gaxset(link_t_ptr ptr)
{
	delete_dom_child_nodes(get_statis_gaxset(ptr));
}

void clear_statis_yaxset(link_t_ptr ptr)
{
	delete_dom_child_nodes(get_statis_yaxset(ptr));
}

void clear_statis_xaxset(link_t_ptr ptr)
{
	delete_dom_child_nodes(get_statis_xaxset(ptr));
}

void merge_statis_yaxset(link_t_ptr ptr_dest, link_t_ptr ptr_src)
{
	link_t_ptr yaxs_dest, yaxs_src;

	XDL_ASSERT(is_statis_doc(ptr_dest) && is_statis_doc(ptr_src));

	yaxs_dest = get_statis_yaxset(ptr_dest);
	yaxs_src = get_statis_yaxset(ptr_src);

	merge_dom_child_nodes(yaxs_dest, yaxs_src);
}

void merge_statis_xaxset(link_t_ptr ptr_dest, link_t_ptr ptr_src)
{
	link_t_ptr xaxs_dest, xaxs_src;

	XDL_ASSERT(is_statis_doc(ptr_dest) && is_statis_doc(ptr_src));

	xaxs_dest = get_statis_xaxset(ptr_dest);
	xaxs_src = get_statis_xaxset(ptr_src);

	merge_dom_child_nodes(xaxs_dest, xaxs_src);
}

bool_t is_statis_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_STATIS,-1,0) == 0)? 1 : 0;
}

bool_t is_statis_gax(link_t_ptr ptr, link_t_ptr glk)
{
	return is_dom_child_node(ptr, glk);
}

bool_t is_statis_xax(link_t_ptr ptr,link_t_ptr xlk)
{
	return is_dom_child_node(ptr,xlk);
}

bool_t is_statis_yax(link_t_ptr ptr,link_t_ptr ylk)
{
	return is_dom_child_node(ptr,ylk);
}

link_t_ptr insert_gax(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr slk, glk;

	slk = get_statis_gaxset(ptr);
	if (!slk)
		return NULL;

	glk = insert_dom_node(slk, pos);
	set_dom_node_name(glk, DOC_STATIS_GAX, -1);
	default_gax_attr(glk);

	return glk;
}

void delete_gax(link_t_ptr glk)
{
	delete_dom_node(glk);
}

int get_gax_count(link_t_ptr ptr)
{
	link_t_ptr slk;

	slk = get_statis_gaxset(ptr);
	if (!slk)
		return 0;

	return get_dom_child_node_count(slk);
}

link_t_ptr get_next_gax(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr slk;

	if (pos == LINK_FIRST)
	{
		slk = get_statis_gaxset(ptr);
		return (slk) ? get_dom_first_child_node(slk) : NULL;
	}
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_prev_gax(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr slk;

	if (pos == LINK_LAST)
	{
		slk = get_statis_gaxset(ptr);
		return (slk) ? get_dom_last_child_node(slk) : NULL;
	}
	else if (pos == LINK_FIRST)
		return NULL;
	else
		return get_prev_link(pos);
}

link_t_ptr get_gax(link_t_ptr ptr, const tchar_t *gname)
{
	link_t_ptr glk;

	if (is_null(gname))
		return NULL;

	glk = get_next_gax(ptr, LINK_FIRST);
	while (glk)
	{
		if (compare_text(get_gax_name_ptr(glk), -1, gname, -1, 0) == 0)
			return glk;
		glk = get_next_gax(ptr, glk);
	}

	return NULL;
}

link_t_ptr insert_yax(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr slk,ylk;

	slk = get_statis_yaxset(ptr);
	if(!slk)
		return NULL;

	ylk = insert_dom_node(slk,pos);
	set_dom_node_name(ylk,DOC_STATIS_YAX,-1);
	default_yax_attr(ylk);

	return ylk;
}

void delete_yax(link_t_ptr ylk)
{
	delete_dom_node(ylk);
}

int get_yax_count(link_t_ptr ptr)
{
	link_t_ptr slk;

	slk = get_statis_yaxset(ptr);
	if(!slk)
		return 0;

	return get_dom_child_node_count(slk);
}

int get_yax_selected_count(link_t_ptr ptr)
{
	link_t_ptr ylk;
	int count = 0;

	ylk = get_next_yax(ptr, LINK_FIRST);
	while (ylk)
	{
		if (get_yax_selected(ylk))
			count++;

		ylk = get_next_yax(ptr, ylk);
	}

	return count;
}

link_t_ptr get_prev_yax(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr slk;

	if (pos == LINK_LAST)
	{
		slk = get_statis_yaxset(ptr);
		return (slk) ? get_dom_last_child_node(slk) : NULL;
	}
	else if (pos == LINK_FIRST)
		return NULL;
	else
		return get_prev_link(pos);
}

link_t_ptr get_next_yax(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr slk;

	if (pos == LINK_FIRST)
	{
		slk = get_statis_yaxset(ptr);
		return (slk) ? get_dom_first_child_node(slk) : NULL;
	}
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_yax_at(link_t_ptr ptr, int index)
{
	link_t_ptr ylk;

	ylk = get_next_yax(ptr, LINK_FIRST);
	while (ylk && index--)
	{
		ylk = get_next_yax(ptr, ylk);
	}

	return ylk;
}

link_t_ptr get_yax(link_t_ptr ptr,const tchar_t *yname)
{
	link_t_ptr ylk;

	if(is_null(yname))
		return NULL;

	ylk = get_next_yax(ptr,LINK_FIRST);
	while(ylk)
	{
		if(compare_text(get_yax_name_ptr(ylk),-1,yname,-1,0) == 0)
			return ylk;
		ylk = get_next_yax(ptr,ylk);
	}
	return NULL;
}

link_t_ptr get_yax_by_id(link_t_ptr ptr, const tchar_t* cid)
{
	link_t_ptr ylk;

	if (is_null(cid))
		return NULL;

	ylk = get_next_yax(ptr, LINK_FIRST);
	while (ylk)
	{
		if (compare_text(get_yax_id_ptr(ylk), -1, cid, -1, 0) == 0)
			return ylk;
		ylk = get_next_yax(ptr, ylk);
	}
	return NULL;
}

link_t_ptr get_yax_gax(link_t_ptr ptr, link_t_ptr ylk)
{
	const tchar_t* gname;
	link_t_ptr glk;

	gname = get_yax_group_ptr(ylk);
	glk = get_gax(ptr, gname);

	return glk;
}

link_t_ptr insert_xax(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr slk, xlk;

	slk = get_statis_xaxset(ptr);
	if (!slk)
		return NULL;

	xlk = insert_dom_node(slk, pos);
	set_dom_node_name(xlk, DOC_STATIS_XAX, -1);

	return xlk;
}

void delete_xax(link_t_ptr xlk)
{
	delete_dom_node(xlk);
}


link_t_ptr get_next_xax(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr slk;

	if (pos == LINK_FIRST)
	{
		slk = get_statis_xaxset(ptr);
		return (slk) ? get_dom_first_child_node(slk) : NULL;
	}
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_prev_xax(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr slk;

	if (pos == LINK_LAST)
	{
		slk = get_statis_xaxset(ptr);
		return (slk) ? get_dom_last_child_node(slk) : NULL;
	}
	else if (pos == LINK_FIRST)
		return NULL;
	else
		return get_prev_link(pos);
}

int get_xax_count(link_t_ptr ptr)
{
	link_t_ptr slk;

	slk = get_statis_xaxset(ptr);
	if (!slk)
		return 0;

	return get_dom_child_node_count(slk);
}

int get_xax_checked_count(link_t_ptr ptr)
{
	link_t_ptr xlk;
	int count = 0;

	xlk = get_next_xax(ptr, LINK_FIRST);
	while (xlk)
	{
		if (get_xax_checked(xlk))
			count++;

		xlk = get_next_xax(ptr, xlk);
	}

	return count;
}

link_t_ptr get_xax_at(link_t_ptr ptr, int pos)
{
	link_t_ptr xlk;

	xlk = get_next_xax(ptr, LINK_FIRST);
	while (xlk && pos--)
		xlk = get_next_xax(ptr, xlk);
	return xlk;
}

link_t_ptr get_xax(link_t_ptr ptr, const tchar_t* xname)
{
	link_t_ptr xlk;
	
	xlk = get_next_xax(ptr, LINK_FIRST);
	while (xlk)
	{
		if (compare_text(get_xax_name_ptr(xlk), -1, xname, -1, 0) == 0)
			return xlk;

		xlk = get_next_xax(ptr, xlk);
	}

	return NULL;
}

int get_update_xax_count(link_t_ptr ptr)
{
	link_t_ptr xlk;
	int ds;
	int count = 0;

	xlk = get_next_xax(ptr, LINK_FIRST);
	while (xlk)
	{
		ds = get_xax_state(xlk);
		if (ds == dsNewDirty || ds == dsDirty || ds == dsDelete)
			count++;
		xlk = get_next_xax(ptr, xlk);
	}
	return count;
}

void refresh_statis_xaxset(link_t_ptr ptr)
{
	link_t_ptr ylk, xlk, next;
	int state;

	xlk = get_next_xax(ptr, LINK_FIRST);
	while (xlk)
	{
		next = get_next_xax(ptr, xlk);

		state = get_xax_state(xlk);
		if (state == dsNewClean || state == dsDelete)
		{
			delete_xax(xlk);
		}
		else
		{
			ylk = get_next_yax(ptr, LINK_FIRST);
			while (ylk)
			{
				set_coor_dirty(xlk, ylk, 0);

				ylk = get_next_yax(ptr, ylk);
			}

			set_xax_state(xlk, dsClean);
		}
		xlk = next;
	}
}

const tchar_t* get_coor_text_ptr(link_t_ptr xlk,link_t_ptr ylk)
{
	link_t_ptr ent;

	ent = get_hash_entity(get_xax_attr_table(xlk),get_yax_name_ptr(ylk),-1);
	return (ent)? get_hash_entity_val_ptr(ent) : NULL;
}

int get_coor_text(link_t_ptr xlk,link_t_ptr ylk,tchar_t* buf,int max)
{
	return read_hash_attr(get_xax_attr_table(xlk),get_yax_name_ptr(ylk),-1,buf,max);
}

void set_coor_text(link_t_ptr xlk, link_t_ptr ylk, const tchar_t *sz, int len)
{
	const tchar_t* sz_name;

	sz_name = get_yax_name_ptr(ylk);
	if(is_null(sz_name))
		return;

	if (len < 0)
		len = xslen(sz);

	if(!len)
		write_hash_attr(get_xax_attr_table(xlk),sz_name,-1,NULL,0);
	else
		set_coor_numeric(xlk,ylk,xsntonum(sz,len));
}

void set_coor_numeric(link_t_ptr xlk,link_t_ptr ylk,double dbl)
{
	tchar_t token[NUM_LEN + 1] = { 0 };
	const tchar_t* sz_name;

	sz_name = get_yax_name_ptr(ylk);
	if(is_null(sz_name))
		return;

	numtoxs_dig(dbl, get_yax_data_dig(ylk), token, NUM_LEN);

	write_hash_attr(get_xax_attr_table(xlk),sz_name,-1,token,-1);
}

double get_coor_numeric(link_t_ptr xlk,link_t_ptr ylk)
{
	const tchar_t* token;

	token = get_coor_text_ptr(xlk,ylk);

	return (token)? xstonum(token) : 0;
}

const tchar_t* get_coor_storage_ptr(link_t_ptr xlk, link_t_ptr ylk, const tchar_t* key)
{
	tchar_t cname[KEY_LEN + 1] = { 0 };

	xsprintf(cname, _T("%s_%s"), get_yax_name_ptr(ylk), key);

	return get_dom_node_options_text_ptr(xlk, cname, -1);
}

void set_coor_storage(link_t_ptr xlk, link_t_ptr ylk, const tchar_t* key, const tchar_t* val)
{
	tchar_t cname[KEY_LEN + 1] = { 0 };

	xsprintf(cname, _T("%s_%s"), get_yax_name_ptr(ylk), key);

	set_dom_node_options_text(xlk, cname, -1, val, -1);
}

const tchar_t* get_xax_storage_ptr(link_t_ptr xlk, const tchar_t* key)
{
	tchar_t cname[KEY_LEN + 1] = { 0 };

	xsprintf(cname, _T("xax_%s"), key);

	return get_dom_node_options_text_ptr(xlk, cname, -1);
}

void set_xax_storage(link_t_ptr xlk, const tchar_t* key, const tchar_t* val)
{
	tchar_t cname[KEY_LEN + 1] = { 0 };

	xsprintf(cname, _T("xax_%s"), key);

	set_dom_node_options_text(xlk, cname, -1, val, -1);
}

void set_xax_dirty(link_t_ptr xlk)
{
	int state;
	state = get_xax_state(xlk);
	if (state == dsNewClean)
		set_xax_state(xlk, dsNewDirty);
	else if (state == dsClean)
		set_xax_state(xlk, dsDirty);
}

void set_xax_clean(link_t_ptr xlk)
{
	int state;
	state = get_xax_state(xlk);
	if (state == dsNewDirty)
		set_xax_state(xlk, dsNewClean);
	else if (state == dsDirty)
		set_xax_state(xlk, dsClean);
}

link_t_ptr set_xax_delete(link_t_ptr xlk)
{
	switch (get_xax_state(xlk))
	{
	case dsNewClean:
	case dsNewDirty:
		delete_xax(xlk);
		return NULL;
	case dsClean:
	case dsDirty:
		set_xax_state(xlk, dsDelete);
		return xlk;
	default:
		return xlk;
	}
}

void set_coor_dirty(link_t_ptr xlk, link_t_ptr ylk, bool_t b)
{
	link_t_ptr ent;
	const tchar_t* sz_yax;
	dword_t msk;

	sz_yax = get_yax_name_ptr(ylk);
	if (is_null(sz_yax))
		return;

	ent = get_hash_entity(get_dom_node_attr_table(xlk), sz_yax, -1);
	if (!ent)
		return;

	msk = get_hash_entity_delta(ent);
	if (b)
		msk |= dsDirty;
	else
		msk &= (~dsDirty);

	set_hash_entity_delta(ent, msk);
}

bool_t get_coor_dirty(link_t_ptr xlk, link_t_ptr ylk)
{
	link_t_ptr ent;
	const tchar_t* sz_yax;
	dword_t msk;

	sz_yax = get_yax_name_ptr(ylk);
	if (is_null(sz_yax))
		return 0;

	ent = get_hash_entity(get_dom_node_attr_table(xlk), sz_yax, -1);
	if (!ent)
		return 0;

	msk = get_hash_entity_delta(ent);

	return (msk & dsDirty) ? 1 : 0;
}


void calc_statis_gax_base(link_t_ptr ptr, link_t_ptr glk, float* pmidd, float* pstep)
{
	double f_min, f_max;
	link_t_ptr xlk, ylk;

	f_min = 0;
	f_max = 0;

	xlk = get_next_xax(ptr, LINK_FIRST);
	while (xlk)
	{
		ylk = get_next_yax(ptr, LINK_FIRST);
		while (ylk)
		{
			if (get_yax_gax(ptr, ylk) == glk)
			{
				if (f_min > get_coor_numeric(xlk, ylk))
					f_min = get_coor_numeric(xlk, ylk);

				if (f_max < get_coor_numeric(xlk, ylk))
					f_max = get_coor_numeric(xlk, ylk);
			}

			ylk = get_next_yax(ptr, ylk);
		}

		xlk = get_next_xax(ptr, xlk);
	}

	if (pmidd)
	{
		*pmidd = (float)(int)((f_min + f_max) / 20) * 10;
	}

	if (pstep)
	{
		*pstep = (float)(int)(f_max - f_min) / 100;
		if ((int)(f_max - f_min) % 100)
			*pstep = (float)(*pstep + 1.0);
		if (!(int)(*pstep))
			*pstep = (float)1.0;
	}
}

#endif //XDLDOC