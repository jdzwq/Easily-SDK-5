/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc grid document

	@module	griddoc.c | implement file

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

#include "griddoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

typedef struct _calc_param_t{
	link_t_ptr ptr;
	link_t_ptr rlk;
}calc_param_t;

typedef struct _find_param_t{
	link_t_ptr ptr;
	link_t_ptr rlk;
	int nocase;
}find_param_t;

typedef struct _format_param_t{
	link_t_ptr dom;
	link_t_ptr nlk;
}format_param_t;

void default_grid_attr(link_t_ptr ptr)
{
	set_grid_width(ptr, DEF_PAPER_WIDTH);
	set_grid_height(ptr, DEF_PAPER_HEIGHT);

	set_grid_colbar_height(ptr, DEF_ITEM_HEIGHT);
	set_grid_rowbar_width(ptr,DEF_ICON_SPAN);
	set_grid_rowbar_height(ptr,DEF_ITEM_HEIGHT);

	set_grid_shape(ptr,ATTR_SHAPE_RECT);
	set_grid_showcheck(ptr, 1);
	set_grid_subfield(ptr, 1);
}

void default_col_attr(link_t_ptr clk) 
{
	set_col_style(clk, _T("font-size:10.5;text-align:center;line-align:center;"));

	set_col_width(clk,DEF_TEXT_WIDTH); 
	set_col_visible(clk,1); 
	set_col_printable(clk,1); 
	set_col_editable(clk,1); 
	set_col_editor(clk,ATTR_EDITOR_FIREEDIT); 
	set_col_data_type(clk,ATTR_DATA_TYPE_STRING); 
	set_col_nullable(clk,1); 
}

tchar_t* col_calc_func(const tchar_t* token,void* parm)
{
	calc_param_t* pcp = (calc_param_t*)parm;
	link_t_ptr clk;
	tchar_t* tmp;
	int len;

	clk = get_col(pcp->ptr,token);
	if(clk == NULL)
		return NULL;

	tmp = (tchar_t*)get_cell_text_ptr(pcp->rlk,clk);
	if(is_null(tmp))
		return NULL;

	len = xslen(tmp);
	tmp = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
	get_cell_text(pcp->rlk,clk,tmp,len);

	return tmp;
}

bool_t row_find_proc(const tchar_t* key,const tchar_t* sin,const tchar_t* val,void* parm)
{
	link_t_ptr clk;
	find_param_t* pfp;
	bool_t matched = 0;
	int vallen,rt;
	const tchar_t* str;
	const tchar_t* type;

	pfp = (find_param_t*)parm;
	
	if(is_null(key))
		return 0;

	clk = get_col(pfp->ptr,key);
	if(clk == NULL)
		return 0;
	
	type = get_col_data_type_ptr(clk);
	str = get_cell_text_ptr(pfp->rlk,clk);
	
	if(!xscmp(sin,_T(">")))
	{
		rt = compare_data(str,val,type);
		matched = (rt > 0)? 1 : 0;
	}else if(!xscmp(sin,_T(">=")))
	{
		rt = compare_data(str,val,type);
		matched = (rt >= 0)? 1 : 0;
	}else if(!xscmp(sin,_T("<")))
	{
		rt = compare_data(str,val,type);
		matched = (rt < 0)? 1 : 0;
	}else if(!xscmp(sin,_T("<=")))
	{
		rt = compare_data(str,val,type);
		matched = (rt <= 0)? 1 : 0;
	}else if(!xscmp(sin,_T("==")))
	{
		rt = compare_data(str,val,type);
		matched = (rt == 0)? 1 : 0;
	}
	else if (!xscmp(sin, _T("*=")))
	{
		matched = is_prefix(str, val);
	}
	else if (!xscmp(sin, _T("=*")))
	{
		matched = is_suffix(str, val);
	}
	else if (!xscmp(sin, _T("<>")))
	{
		rt = compare_data(str,val,type);
		matched = (rt != 0)? 1 : 0;
	}else if(!xscmp(sin,_T("=")))
	{
		if(str == NULL && val == NULL)
			matched = 1;
		else if(str == NULL && val)
			matched = 0;
		else if(str && val == NULL)
			matched = 1;
		else
		{
			vallen = (val)? xslen(val) : 0;
			matched = xsnicmp(str,val,vallen)? 0 : 1;
		}
	}
	return matched;
}

int _row_compare_func(link_t_ptr rlk1, link_t_ptr rlk2, void* parm)
{
	link_t_ptr clk = (link_t_ptr)parm;

	return compare_data(get_cell_text_ptr(rlk1,clk),get_cell_text_ptr(rlk2,clk),get_col_data_type_ptr(clk));
}

void _reset_grid_filter(link_t_ptr ptr)
{
	link_t_ptr rlk;
	
	rlk = get_next_row(ptr,LINK_FIRST);
	while(rlk)
	{
		set_row_hidden(rlk,0);
		rlk = get_next_row(ptr,rlk);
	}
}

link_t_ptr get_grid_colset(link_t_ptr ptr)
{
	link_t_ptr plk;

	plk = get_dom_first_child_node(ptr);
	while(plk)
	{
		if(0 == xscmp(get_dom_node_name_ptr(plk),DOC_GRID_COLSET))
			return plk;

		plk = get_dom_next_sibling_node(plk);
	}

	return NULL;
}

link_t_ptr get_grid_rowset(link_t_ptr ptr)
{
	link_t_ptr plk;

	plk = get_dom_first_child_node(ptr);
	while(plk)
	{
		if(0 == xscmp(get_dom_node_name_ptr(plk),DOC_GRID_ROWSET))
			return plk;

		plk = get_dom_next_sibling_node(plk);
	}

	return NULL;
}

void set_rowset_checked(link_t_ptr ptr, bool_t b)
{
	link_t_ptr plk;

	plk = get_grid_rowset(ptr);

	if (plk)
		set_dom_node_mask_check(plk, MSK_CHECKED, b);
}

bool_t get_rowset_checked(link_t_ptr ptr)
{
	link_t_ptr plk;

	plk = get_grid_rowset(ptr);

	return (plk) ? get_dom_node_mask_check(plk, MSK_CHECKED) : 0;
}

link_t_ptr create_grid_doc(void)
{
	link_t_ptr ptr;
	link_t_ptr nlk;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_GRID,-1);

	nlk = insert_dom_node(ptr,LINK_LAST);
	set_dom_node_name(nlk,DOC_GRID_COLSET,-1);

	nlk = insert_dom_node(ptr,LINK_LAST);
	set_dom_node_name(nlk,DOC_GRID_ROWSET,-1);

	default_grid_attr(ptr);

	return ptr;
}

void destroy_grid_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_grid_doc(link_t_ptr ptr)
{
	clear_grid_rowset(ptr);
	clear_grid_colset(ptr);
}

void clear_grid_colset(link_t_ptr ptr)
{
	delete_dom_child_nodes(get_grid_colset(ptr));
}

void clear_grid_rowset(link_t_ptr ptr)
{
	link_t_ptr clk;

	clk = get_next_col(ptr, LINK_FIRST);
	while (clk)
	{
		set_col_sum_text(clk, NULL);
		clk = get_next_col(ptr, clk);
	}

	delete_dom_child_nodes(get_grid_rowset(ptr));
}

bool_t is_grid_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_GRID,-1,0) == 0)? 1 : 0;
}

bool_t is_grid_col(link_t_ptr ptr,link_t_ptr clk)
{
	return is_dom_child_node(get_grid_colset(ptr), clk);
}

bool_t is_grid_row(link_t_ptr ptr, link_t_ptr rlk)
{
	return is_dom_child_node(get_grid_rowset(ptr), rlk);
}

void merge_grid_colset(link_t_ptr ptr_dest,link_t_ptr ptr_src)
{
	link_t_ptr cols_dest, cols_src;

	XDL_ASSERT(is_grid_doc(ptr_dest) && is_grid_doc(ptr_src));

	cols_dest = get_grid_colset(ptr_dest);
	cols_src = get_grid_colset(ptr_src);

	merge_dom_child_nodes(cols_dest, cols_src);
}

void merge_grid_rowset(link_t_ptr ptr_dest,link_t_ptr ptr_src)
{
	link_t_ptr rows_dest,rows_src;

	XDL_ASSERT(is_grid_doc(ptr_dest) && is_grid_doc(ptr_src));

	rows_dest = get_grid_rowset(ptr_dest);
	rows_src = get_grid_rowset(ptr_src);
	
	merge_dom_child_nodes(rows_dest, rows_src);
}

link_t_ptr insert_col(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr slk,clk;

	slk = get_grid_colset(ptr);
	if(!slk)
		return NULL;

	clk = insert_dom_node(slk,pos);
	set_dom_node_name(clk,DOC_GRID_COL,-1);

	default_col_attr(clk);
	return clk;
}

int get_col_count(link_t_ptr ptr)
{
	link_t_ptr slk;

	slk = get_grid_colset(ptr);
	if(!slk)
		return 0;

	return get_dom_child_node_count(slk);
}

int get_col_selected_count(link_t_ptr ptr)
{
	link_t_ptr clk;
	int count = 0;

	clk = get_next_col(ptr, LINK_FIRST);
	while (clk)
	{
		if (get_col_selected(clk))
			count++;

		clk = get_next_visible_col(ptr, clk);
	}

	return count;
}

void delete_col(link_t_ptr clk)
{
	delete_dom_node(clk);
}

link_t_ptr get_next_col(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr slk;

	if(pos == LINK_FIRST)
	{
		slk = get_grid_colset(ptr);
		return (slk)? get_dom_first_child_node(slk) : NULL;
	}else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_prev_col(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr slk;

	if(pos == LINK_LAST)
	{
		slk = get_grid_colset(ptr);
		return (slk)? get_dom_last_child_node(slk) : NULL;
	}else if(pos == LINK_FIRST)
		return NULL;
	else
		return get_prev_link(pos);
}

link_t_ptr get_next_visible_col(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr clk;
	
	clk = get_next_col(ptr,pos);
	if(!clk)
		return NULL;
	else
		return (!grid_is_design(ptr) && !get_col_visible(clk)) ? NULL : clk;
}

link_t_ptr get_prev_visible_col(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr clk;
	
	clk = get_prev_col(ptr,pos);
	if(!clk)
		return NULL;
	else
		return (!grid_is_design(ptr) && !get_col_visible(clk)) ? NULL : clk;
}

link_t_ptr get_next_focusable_col(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr clk;

	clk = get_next_col(ptr, pos);
	while (clk)
	{
		if (get_col_visible(clk) && get_col_editable(clk))
			break;

		clk = get_next_col(ptr, clk);
	}

	return clk;
}

link_t_ptr get_prev_focusable_col(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr clk;

	clk = get_prev_col(ptr, pos);
	while (clk)
	{
		if (get_col_visible(clk) && get_col_editable(clk))
			break;

		clk = get_prev_col(ptr, clk);
	}

	return clk;
}

bool_t get_col_focusable(link_t_ptr clk)
{
	return (get_col_visible(clk) && get_col_editable(clk)) ? 1 : 0;
}

link_t_ptr get_col(link_t_ptr ptr,const tchar_t* cname)
{
	link_t_ptr clk;

	if(is_null(cname))
		return NULL;

	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		if(compare_text(cname,-1,get_col_name_ptr(clk),-1,0) == 0)
			return clk;
		clk = get_next_col(ptr,clk);
	}
	return NULL;
}

link_t_ptr get_col_by_id(link_t_ptr ptr, const tchar_t* cid)
{
	link_t_ptr clk;

	if (is_null(cid))
		return NULL;

	clk = get_next_col(ptr, LINK_FIRST);
	while (clk)
	{
		if (compare_text(cid, -1, get_col_id_ptr(clk), -1, 0) == 0)
			return clk;
		clk = get_next_col(ptr, clk);
	}
	return NULL;
}

link_t_ptr get_col_at(link_t_ptr ptr,int index)
{
	link_t_ptr clk;

	clk = get_next_col(ptr,LINK_FIRST);
	while(clk && index --)
	{
		clk = get_next_col(ptr,clk);
	}

	return clk;
}

bool_t get_col_integrity(link_t_ptr clk)
{
	bool_t updateable;
	const tchar_t* fieldtype;
	const tchar_t* fieldname;
	
	updateable = get_col_updateable(clk);
	fieldtype = get_col_field_type_ptr(clk);
	fieldname = get_col_field_name_ptr(clk);

	if(updateable == 0  || is_null(fieldname) || is_null(fieldtype) || compare_text(fieldtype,-1,ATTR_FIELD_TYPE_CALC,-1,0) == 0)
		return 0;
	else
		return 1;
}

bool_t get_grid_updateable(link_t_ptr ptr)
{
	link_t_ptr clk;

	clk = get_next_col(ptr, LINK_FIRST);
	while (clk)
	{
		if (get_col_integrity(clk))
		{
			return 1;
		}
		clk = get_next_col(ptr, clk);
	}

	return 0;
}
int get_row_count(link_t_ptr ptr)
{
	link_t_ptr slk;

	slk = get_grid_rowset(ptr);
	if(!slk)
		return 0;

	return get_dom_child_node_count(slk);
}

int get_row_checked_count(link_t_ptr ptr)
{
	link_t_ptr rlk;
	int count = 0;

	rlk = get_next_visible_row(ptr, LINK_FIRST);
	while (rlk)
	{
		if (get_row_checked(rlk))
			count++;

		rlk = get_next_visible_row(ptr, rlk);
	}

	return count;
}

link_t_ptr insert_row(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr slk,rlk;

	slk = get_grid_rowset(ptr);
	if(!slk)
		return NULL;

	rlk = insert_dom_node(slk,pos);
	set_dom_node_name(rlk,DOC_GRID_ROW,-1);
	return rlk;
}

void delete_row(link_t_ptr rlk)
{
	delete_dom_node(rlk);
}

link_t_ptr get_next_row(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr slk;

	if(pos == LINK_FIRST)
	{
		slk = get_grid_rowset(ptr);
		return (slk)? get_dom_first_child_node(slk) : NULL;
	}else if(pos == LINK_LAST)
		return NULL;
	else
		return get_next_link(pos);
}

link_t_ptr get_prev_row(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr slk;

	if(pos == LINK_LAST)
	{
		slk = get_grid_rowset(ptr);
		return (slk)? get_dom_last_child_node(slk) : NULL;
	}else if(pos == LINK_FIRST)
		return NULL;
	else
		return get_prev_link(pos);
}

void set_row_dirty(link_t_ptr rlk)
{
	int state;
	state = get_row_state(rlk);
	if(state == dsNewClean)
		set_row_state(rlk,dsNewDirty);
	else if(state == dsClean)
		set_row_state(rlk,dsDirty);
}

void set_row_clean(link_t_ptr rlk)
{
	int state;
	state = get_row_state(rlk);
	if(state == dsNewDirty)
		set_row_state(rlk,dsNewClean);
	else if(state == dsDirty)
		set_row_state(rlk,dsClean);
}

link_t_ptr set_row_delete(link_t_ptr rlk)
{
	switch(get_row_state(rlk))
	{
	case dsNewClean:
	case dsNewDirty:
		delete_row(rlk);
		return NULL;
	case dsClean:
	case dsDirty:
		set_row_state(rlk,dsDelete);
		return rlk;
	default:
		return rlk;
	}
}

void set_cell_dirty(link_t_ptr rlk,link_t_ptr clk,bool_t b)
{
	link_t_ptr ent;
	var_long msk;

	const tchar_t* sz_col;

	sz_col = get_col_name_ptr(clk);
	if(is_null(sz_col))
		return;

	ent = get_hash_entity(get_dom_node_attr_table(rlk),sz_col,-1);
	if (!ent)
		return;

	msk = get_hash_entity_delta(ent);
	if (b)
		msk |= dsDirty;
	else
		msk &= (~dsDirty);

	set_hash_entity_delta(ent,msk);
}

bool_t get_cell_dirty(link_t_ptr rlk,link_t_ptr clk)
{
	link_t_ptr ent;
	const tchar_t* sz_col;
	var_long msk;

	sz_col = get_col_name_ptr(clk);
	if(is_null(sz_col))
		return dsClean;

	ent = get_hash_entity(get_dom_node_attr_table(rlk),sz_col,-1);
	if(!ent)
		return dsClean;

	msk = get_hash_entity_delta(ent);

	return (msk & dsDirty)? 1 : 0;
}

const tchar_t* get_row_storage_ptr(link_t_ptr rlk, const tchar_t* key)
{
	tchar_t cname[KEY_LEN + 1] = { 0 };

	xsprintf(cname, _T("row_%s"), key);

	return get_dom_node_options_text_ptr(rlk, cname, -1);
}

void set_row_storage(link_t_ptr rlk, const tchar_t* key, const tchar_t* val)
{
	tchar_t cname[KEY_LEN + 1] = { 0 };

	xsprintf(cname, _T("row_%s"), key);

	set_dom_node_options_text(rlk, cname, -1, val, -1);
}

const tchar_t* get_cell_storage_ptr(link_t_ptr rlk, link_t_ptr clk, const tchar_t* key)
{
	tchar_t cname[KEY_LEN + 1] = { 0 };

	xsprintf(cname, _T("%s_%s"), get_col_name_ptr(clk), key);

	return get_dom_node_options_text_ptr(rlk, cname, -1);
}

void set_cell_storage(link_t_ptr rlk, link_t_ptr clk, const tchar_t* key, const tchar_t* val)
{
	tchar_t cname[KEY_LEN + 1] = { 0 };

	xsprintf(cname, _T("%s_%s"), get_col_name_ptr(clk), key);

	set_dom_node_options_text(rlk, cname, -1, val, -1);
}

const tchar_t* get_cell_prim_ptr(link_t_ptr rlk,link_t_ptr clk)
{
	const tchar_t* cname;

	cname = get_col_name_ptr(clk);
	if(is_null(cname))
		return NULL;
	
	return get_dom_node_options_text_ptr(rlk,cname,-1);
}

void set_cell_prim(link_t_ptr rlk, link_t_ptr clk, const tchar_t *sz, int len)
{
	const tchar_t* cname;

	cname = get_col_name_ptr(clk);
	if(is_null(cname))
		return;

	set_dom_node_options_text(rlk,cname,-1,sz,len);
}

void attach_cell_text(link_t_ptr rlk, link_t_ptr clk, tchar_t* val)
{
	attach_dom_node_attr(rlk, get_col_name_ptr(clk), val);
}

tchar_t* detach_cell_text(link_t_ptr rlk, link_t_ptr clk)
{
	return detach_dom_node_attr(rlk, get_col_name_ptr(clk));
}

const tchar_t* get_cell_text_ptr(link_t_ptr rlk,link_t_ptr clk)
{
	return get_dom_node_attr_ptr(rlk,get_col_name_ptr(clk),-1);
}

int get_cell_text(link_t_ptr rlk,link_t_ptr clk,tchar_t* buf,int max)
{
	return get_dom_node_attr(rlk,get_col_name_ptr(clk),-1,buf,max);
}

void set_cell_text(link_t_ptr rlk, link_t_ptr clk, const tchar_t *sz, int len)
{
	const tchar_t* cname;
	const tchar_t* type;
	const tchar_t* scase;
	tchar_t* tmp;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	xdate_t md = { 0 };
	link_t_ptr elk;

	if (len < 0)
		len = xslen(sz);

	cname = get_col_name_ptr(clk);
	if (is_null(cname))
	{
		return;
	}

	if(!len)
	{
		set_dom_node_attr(rlk,cname,-1,NULL,-1);
		return;
	}

	type = get_col_data_type_ptr(clk);
	if (compare_text(type, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
	{
		set_cell_boolean(rlk, clk, (bool_t)xsntos(sz,len));
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
	{
		set_cell_integer(rlk, clk, xsntol(sz,len));
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
	{
		set_cell_numeric(rlk, clk, xsntonum(sz,len));
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0)
	{
		xsncpy(sz_date, sz, len);
		parse_date(&md, sz_date);
		if (verify_datetime(&md))
			set_cell_date(rlk, clk, &md);
		else
			set_dom_node_attr(rlk, cname, -1, NULL, -1);
	}
	else if (compare_text(type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
	{
		xsncpy(sz_date, sz, len);
		parse_datetime(&md, sz_date);
		if (verify_datetime(&md))
			set_cell_datetime(rlk, clk, &md);
		else
			set_dom_node_attr(rlk, cname, -1, NULL, -1);
	}
	else if(compare_text(type,-1,ATTR_DATA_TYPE_STRING,-1,0) == 0)
	{
		elk = set_dom_node_attr(rlk, cname, -1, NULL, -1);

		tmp = xsnclone(sz, len);
		scase = get_col_case_ptr(clk);
		if(compare_text(scase,-1,ATTR_CASE_LOWER,-1,0) == 0)
		{
			xslwr(tmp);
		}else if(compare_text(scase,-1,ATTR_CASE_UPPER,-1,0) == 0)
		{
			xsupr(tmp);
		}
		attach_hash_entity_val(elk, tmp);
	}
}

void set_cell_boolean(link_t_ptr rlk,link_t_ptr clk,bool_t b)
{
	set_dom_node_attr(rlk,get_col_name_ptr(clk),-1,((b)? _T("1") : _T("0")),-1);
}

bool_t get_cell_boolean(link_t_ptr rlk,link_t_ptr clk)
{
	return (compare_text(get_cell_text_ptr(rlk, clk), -1, _T("1"), -1, 0) == 0) ? 1 : 0;
}

void set_cell_integer(link_t_ptr rlk,link_t_ptr clk,int n)
{
	tchar_t token[NUM_LEN  + 1];

	xsprintf(token,_T("%d"),n);
	set_dom_node_attr(rlk,get_col_name_ptr(clk),-1,token,-1);
}

int get_cell_integer(link_t_ptr rlk,link_t_ptr clk)
{
	const tchar_t* token;
	
	token = get_cell_text_ptr(rlk,clk);
	return (is_null(token))? 0 : xstol(token);
}

void set_cell_numeric(link_t_ptr rlk,link_t_ptr clk,double n)
{
	tchar_t token[NUM_LEN + 1] = { 0 };

	numtoxs_dig(n, get_col_data_dig(clk), token, NUM_LEN);
	set_dom_node_attr(rlk,get_col_name_ptr(clk),-1,token,-1);
}

double get_cell_numeric(link_t_ptr rlk,link_t_ptr clk)
{
	const tchar_t* token;
	
	token = get_cell_text_ptr(rlk,clk);

	return (is_null(token))? 0 : xstonum(token);
}

void set_cell_datetime(link_t_ptr rlk,link_t_ptr clk,const xdate_t* pdt)
{
	tchar_t token[DATE_LEN + 1] = { 0 };

	format_datetime(pdt,token);

	set_dom_node_attr(rlk,get_col_name_ptr(clk),-1,token,-1);
}

void get_cell_datetime(link_t_ptr rlk,link_t_ptr clk,xdate_t* pdt)
{
	parse_datetime(pdt,get_cell_text_ptr(rlk,clk));
}

void set_cell_date(link_t_ptr rlk,link_t_ptr clk,const xdate_t* pdt)
{
	tchar_t token[DATE_LEN + 1] = { 0 };

	format_date(pdt,token);

	set_dom_node_attr(rlk,get_col_name_ptr(clk),-1,token,-1);
}

void get_cell_date(link_t_ptr rlk,link_t_ptr clk,xdate_t* pdt)
{
	parse_date(pdt,get_cell_text_ptr(rlk,clk));
}

const tchar_t* get_cell_options_text_ptr(link_t_ptr rlk,link_t_ptr clk)
{
	const tchar_t *sz_text, *sz_opt;

	sz_text = get_cell_text_ptr(rlk, clk);
	
	sz_opt = get_dom_node_options_text_ptr(clk,sz_text,-1);
	return (is_null(sz_opt)) ? sz_text : sz_opt;
}

int get_visible_row_count(link_t_ptr ptr)
{
	link_t_ptr rlk;
	int count = 0;

	rlk = get_next_row(ptr,LINK_FIRST);
	while(rlk)
	{
		if(get_row_visible(rlk))
			count ++;
		rlk = get_next_row(ptr,rlk);
	}
	return count;
}

int get_update_row_count(link_t_ptr ptr)
{
	link_t_ptr rlk;
	int ds;
	int count = 0;

	rlk = get_next_row(ptr,LINK_FIRST);
	while(rlk)
	{
		ds = get_row_state(rlk);
		if(ds == dsNewDirty || ds == dsDirty || ds == dsDelete)
			count ++;
		rlk = get_next_row(ptr,rlk);
	}
	return count;
}

link_t_ptr get_next_visible_row(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr rlk;

	rlk = get_next_row(ptr,pos);
	while(rlk)
	{
		if(get_row_visible(rlk))
			return rlk;
		rlk = get_next_row(ptr,rlk);
	}
	return NULL;
}

link_t_ptr get_prev_visible_row(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr rlk;

	rlk = get_prev_row(ptr,pos);
	while(rlk)
	{
		if(get_row_visible(rlk))
			return rlk;
		rlk = get_prev_row(ptr,rlk);
	}
	return NULL;
}

link_t_ptr get_visible_row_at(link_t_ptr ptr,int index)
{
	link_t_ptr rlk;

	XDL_ASSERT(index >= 0);

	rlk = get_next_visible_row(ptr,LINK_FIRST);
	while(rlk && index --)
		rlk = get_next_visible_row(ptr,rlk);
	return rlk;
}

int calc_grid_row(link_t_ptr ptr,link_t_ptr rlk)
{
	link_t_ptr clk;
	link_t_ptr macro;
	tchar_t* tmp;
	calc_param_t cp;
	int count = 0;

	cp.ptr = ptr;
	cp.rlk = rlk;

	clk = get_next_col(ptr,LINK_FIRST);
	while(clk)
	{
		tmp = (tchar_t*)get_col_macro_ptr(clk);
		if(!is_null(tmp))
		{
			macro = create_multi_tree();

			if(macro_parse(macro,tmp,-1))
			{
				tmp = macro_calc(macro,(macro_var_fetch)col_calc_func,(void*)&cp);
				if(compare_data(tmp,get_cell_text_ptr(rlk,clk),get_col_data_type_ptr(clk)))
				{
					set_cell_text(rlk,clk,tmp,-1);
					set_cell_dirty(rlk,clk,1);
					
					if(get_col_integrity(clk))
						count ++;
				}
				
				if(tmp)
					xmem_free(tmp);
			}

			destroy_multi_tree(macro);
		}
		clk = get_next_col(ptr,clk);
	}

	if(count)
		set_row_dirty(rlk);

	return count;
}

int calc_grid_rowset(link_t_ptr ptr)
{
	link_t_ptr rlk;
	int count = 0;

	rlk = get_next_visible_row(ptr,LINK_FIRST);
	while(rlk)
	{
		if(calc_grid_row(ptr,rlk))
			count ++;
		rlk = get_next_visible_row(ptr,rlk);
	}
	return count;
}

int sum_grid_col(link_t_ptr ptr,link_t_ptr clk)
{
	link_t_ptr rlk;
	int intRt;
	double dbRt;
	int count;
	const tchar_t* str;
	const tchar_t* type;
	const tchar_t* mode;
	tchar_t buf[NUM_LEN + 1] = { 0 };

	mode = get_col_sum_mode_ptr(clk);
	if(is_null(mode))
		return  0;

	set_col_sum_text(clk, NULL);

	type = get_col_data_type_ptr(clk);
	count = 0;
	intRt = 0;
	dbRt = 0;
	rlk = get_next_visible_row(ptr,LINK_FIRST);
	while(rlk)
	{
		if(get_row_state(rlk) == dsNewClean || get_row_skipped(rlk))
		{
			rlk = get_next_visible_row(ptr,rlk);
			continue;
		}
		
		count++;

		str = get_cell_text_ptr(rlk,clk);
		if(is_null(str))
		{
			rlk = get_next_visible_row(ptr,rlk);
			continue;
		}

		if(compare_text(type,-1,ATTR_DATA_TYPE_INTEGER,-1,0) == 0)
			intRt += xstol(str);
		else if(compare_text(type,-1,ATTR_DATA_TYPE_NUMERIC,-1,0) == 0)
			dbRt += xstonum(str);
		
		rlk = get_next_visible_row(ptr,rlk);
	}
	
	if(!count)
		return 0;

	buf[0] = _T('\0');
	if(compare_text(mode,-1,ATTR_SUM_MODE_COUNT,-1,0) == 0)
	{
		xsprintf(buf,_T("%d"),count);
	}else if(compare_text(mode,-1,ATTR_SUM_MODE_SUM,-1,0) == 0)
	{
		if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
			xsprintf(buf, _T("%d"), intRt);
		else if (compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
			numtoxs_dig(dbRt, get_col_data_dig(clk), buf, NUM_LEN);
	}else if(compare_text(mode,-1,ATTR_SUM_MODE_AVG,-1,0) == 0)
	{
		if(compare_text(type,-1,ATTR_DATA_TYPE_INTEGER,-1,0) == 0)
			xsprintf(buf,_T("%.2f"),(double)intRt / (double)count);
		else if(compare_text(type,-1,ATTR_DATA_TYPE_NUMERIC,-1,0) == 0)
			numtoxs_dig(dbRt / (double)count, get_col_data_dig(clk) + 2,buf, NUM_LEN);
	}

	set_col_sum_text(clk,buf);
	return count;
}

int sum_grid_colset(link_t_ptr ptr)
{
	link_t_ptr clk;
	int count = 0;

	clk = get_next_visible_col(ptr,LINK_FIRST);
	while(clk)
	{
		if(sum_grid_col(ptr,clk))
			count ++;

		clk = get_next_visible_col(ptr,clk);
	}
	return count;
}

void set_col_sum_text(link_t_ptr clk,const tchar_t* token)
{
	set_dom_node_text(clk, token, -1);
}

const tchar_t* get_col_sum_text_ptr(link_t_ptr clk)
{
	return get_dom_node_text_ptr(clk);
}

void set_cell_grouped(link_t_ptr rlk, link_t_ptr clk, bool_t b)
{

}

bool_t get_cell_grouped(link_t_ptr rlk, link_t_ptr clk)
{
    return 0;
}

link_t_ptr group_grid_col(link_t_ptr ptr, link_t_ptr clk, link_t_ptr rlk_from, link_t_ptr rlk_to)
{
	link_t_ptr rlk;
	const tchar_t* str = NULL;

	if (!get_col_groupable(clk))
		return LINK_LAST;
	
	if (rlk_from == LINK_FIRST)
		rlk_from = get_next_visible_row(ptr, LINK_FIRST);

	if (!rlk_from)
		return LINK_LAST;

	str = get_cell_text_ptr(rlk, clk);

	rlk = get_next_visible_row(ptr, rlk_from);
	while (rlk)
	{
		if (compare_text(str, -1, get_cell_text_ptr(rlk, clk), -1, 0) == 0)
		{
			set_cell_grouped(rlk, clk, 1);
			if (rlk == rlk_to)
				return rlk;
		}
		else
		{
			set_cell_grouped(rlk, clk, 0);
			return rlk;
		}
		
		rlk = get_next_visible_row(ptr, rlk);
	}

	return LINK_LAST;
}

int group_grid_colset(link_t_ptr ptr)
{
	link_t_ptr rlk, clk;
	link_t_ptr rlk_from, rlk_to = LINK_LAST;
	int count = 0;

	rlk = get_next_visible_row(ptr, LINK_FIRST);
	while (rlk)
	{
		clk = get_next_visible_col(ptr, LINK_FIRST);
		while (clk)
		{
			if (!get_col_groupable(clk))
				break;

			rlk_from = rlk;
			rlk_to = group_grid_col(ptr, clk, rlk_from, rlk_to);
			count++;

			clk = get_next_visible_col(ptr, clk);
		}
		rlk = (rlk_to == LINK_LAST)? NULL : rlk_to;
	}
	
	return count;
}

void clear_col_datadef(link_t_ptr clk)
{
	set_col_data_type(clk, ATTR_DATA_TYPE_STRING);
	set_dom_node_attr(clk, ATTR_DATA_LEN, -1, NULL, 0);
	set_dom_node_attr(clk, ATTR_DATA_DIG, -1, NULL, 0);
}

void refresh_grid_rowset(link_t_ptr ptr)
{
	link_t_ptr clk,rlk,next;
	int state;

	rlk = get_next_row(ptr,LINK_FIRST);
	while(rlk)
	{
		next = get_next_row(ptr,rlk);
		state = get_row_state(rlk);
		if (state == dsNewClean || state == dsDelete)
		{
			delete_row(rlk);
		}else
		{
			clk = get_next_col(ptr,LINK_FIRST);
			while(clk)
			{
				set_cell_dirty(rlk,clk,0);

				if(compare_text(get_col_field_type_ptr(clk),-1,ATTR_FIELD_TYPE_PRIM,-1,0) == 0)
					set_cell_prim(rlk,clk,get_cell_text_ptr(rlk,clk),-1);
				
				clk = get_next_col(ptr,clk);
			}

			set_row_state(rlk,dsClean);
		}
		rlk = next;
	}
}

void filter_grid_rowset(link_t_ptr ptr,const tchar_t* szFilter)
{
	link_t_ptr rlk;
	link_t_ptr maco;
	bool_t matched;
	find_param_t fp = {0};

	if(is_null(szFilter))	
	{
		_reset_grid_filter(ptr);
		return ;
	}
	
	maco = create_multi_tree();

	expr_parse(maco,szFilter);
	
	rlk = get_next_row(ptr,LINK_FIRST);
	while(rlk)
	{
		fp.ptr = ptr;
		fp.rlk = rlk;
		fp.nocase = 1;

		matched = expr_exec(maco,(expr_compare_ptr)row_find_proc,&fp);
		set_row_hidden(rlk,((matched)? 0 : 1));

		rlk = get_next_row(ptr,rlk);
	}

	destroy_multi_tree(maco);
}

int verify_grid_cell(link_t_ptr rlk,link_t_ptr clk)
{
	return verify_text(get_cell_text_ptr(rlk, clk), get_col_data_type_ptr(clk),get_col_nullable(clk), get_col_data_len(clk), get_col_data_min_ptr(clk), get_col_data_max_ptr(clk));
}

int verify_grid_doc(link_t_ptr ptr,link_t_ptr* prlk,link_t_ptr* pclk)
{
	link_t_ptr rlk,clk;
	int rs, rt;

	if(prlk)
		*prlk = NULL;
	if(pclk)
		*pclk = NULL;

	rlk = get_next_visible_row(ptr,LINK_FIRST);
	while(rlk)
	{
		rs = get_row_state(rlk);
		if(rs == dsNewClean || rs == dsClean)
		{
			rlk = get_next_visible_row(ptr,rlk);
			continue;
		}

		clk = get_next_focusable_col(ptr,LINK_FIRST);
		while(clk)
		{
			if (!get_col_integrity(clk))
			{
				clk = get_next_focusable_col(ptr, clk);
				continue;
			}

			rt = verify_grid_cell(rlk, clk);
			if(veValid != rt)
			{
				if (prlk)
					*prlk = rlk;
				if (pclk)
					*pclk = clk;

				return rt;
			}

			clk = get_next_focusable_col(ptr, clk);
		}

		rlk = get_next_visible_row(ptr,rlk);
	}

	return veValid;
}

int format_grid_verify(link_t_ptr ptr, link_t_ptr rlk, link_t_ptr clk, int code, tchar_t* buf, int max)
{
	int len;

	switch (code)
	{
	case veNull:
		len = xsprintf(buf, _T("[%s]: (null) null able checked failed"), get_col_title_ptr(clk));
		break;
	case veDatatype:
		len = xsprintf(buf, _T("[%s]: %s data type checked failed"), get_col_title_ptr(clk), get_cell_text_ptr(rlk, clk));
		break;
	case veOverflow:
		len = xsprintf(buf, _T("[%s]: %s data checked overflow"), get_col_title_ptr(clk), get_cell_text_ptr(rlk, clk));
		break;
	case veTruncate:
		len = xsprintf(buf, _T("[%s]: %s data will be truncated"), get_col_title_ptr(clk), get_cell_text_ptr(rlk, clk));
		break;
	case veCodeSystem:
		len = xsprintf(buf, _T("[%s]: %s data not in codeSystem"), get_col_title_ptr(clk), get_cell_text_ptr(rlk, clk));
		break;
	default:
		len = xsprintf(buf, _T("[%s]: %s data value checked failed"), get_col_title_ptr(clk), get_cell_text_ptr(rlk, clk));
		break;
	}

	return len;
}

int make_grid_options(link_t_ptr ptr,const tchar_t* keycol,const tchar_t*  valcol,tchar_t* buf,int max)
{
	link_t_ptr keyclk,valclk;
	link_t_ptr rlk;
	const tchar_t *key,*val;
	int total,len;

	keyclk = get_col(ptr,keycol);
	valclk = get_col(ptr,valcol);

	if(keyclk == NULL || valclk == NULL)
		return 0;

	total = 0;
	rlk = get_next_visible_row(ptr,LINK_FIRST);
	while(rlk)
	{
		if(is_null(get_cell_text_ptr(rlk,keyclk)))
		{
			rlk = get_next_visible_row(ptr,rlk);
			continue;
		}

		key = get_cell_text_ptr(rlk,keyclk);
		val = get_cell_text_ptr(rlk,valclk);

		len = xslen(key) + ((val)? xslen(val) : 0) + 2 ; 
		if(total + len > max)
			return total;

		if (buf)
		{
			xsprintf(buf + total, _T("%s%c%s%c"), key, OPT_ITEMFEED, ((val) ? val : _T("")), OPT_LINEFEED);
		}
		total += len;

		rlk = get_next_visible_row(ptr,rlk);
	}

	return total;
}

void sort_grid_col(link_t_ptr ptr,link_t_ptr clk)
{
	link_t_ptr nlk,rlk;
	bool_t decs;

	rlk = get_next_row(ptr,LINK_FIRST);
	if(!rlk)
		return;

	decs = (get_dom_node_mask(clk) & MSK_SORTED)? 1 : 0;

	nlk = get_grid_rowset(ptr);

	heap_sort_link(get_dom_child_node_root(nlk),(CALLBACK_SORTLINK)_row_compare_func,decs,(void*)clk);

	if(decs)
		set_dom_node_mask(clk,(get_dom_node_mask(clk) & ~MSK_SORTED));
	else
		set_dom_node_mask(clk,(get_dom_node_mask(clk) | MSK_SORTED));
}

link_t_ptr find_grid_row(link_t_ptr ptr,const tchar_t* token,link_t_ptr cur,bool_t next)
{
	link_t_ptr rlk;
	link_t_ptr tree;
	find_param_t fp = {0};

	tree = create_multi_tree();

	expr_parse(tree,token);

	if(next)
		rlk = get_next_visible_row(ptr,cur);
	else
		rlk = get_prev_visible_row(ptr,cur);

	while(rlk)
	{
		fp.ptr = ptr;
		fp.rlk = rlk;
		fp.nocase = 0;

		if(expr_exec(tree,(expr_compare_ptr)row_find_proc,&fp))
			break;

		if(next)
			rlk = get_next_visible_row(ptr,rlk);
		else
			rlk = get_prev_visible_row(ptr,rlk);
	}

	destroy_multi_tree(tree);

	return rlk;
}

void copy_grid_row(link_t_ptr ptr_dest,link_t_ptr rlk_dest,link_t_ptr ptr_src,link_t_ptr rlk_src)
{
	link_t_ptr clk_dest,clk_src;
	const tchar_t* cname;
	const tchar_t* token;
	const tchar_t* type;

	clk_dest = get_next_col(ptr_dest,LINK_FIRST);
	while(clk_dest)
	{
		cname = get_col_name_ptr(clk_dest);
		clk_src = get_col(ptr_src,cname);
		if(clk_src)
		{
			token = get_cell_text_ptr(rlk_src,clk_src);
			type = get_col_data_type_ptr(clk_dest);

			if (compare_data(get_cell_text_ptr(rlk_dest, clk_dest), token, type) != 0)
			{
				set_cell_text(rlk_dest, clk_dest, token, -1);
				set_cell_dirty(rlk_dest, clk_dest, 1);

				if (get_col_integrity(clk_dest))
				{
					set_row_dirty(rlk_dest);
				}
			}
		}

		clk_dest = get_next_col(ptr_dest,clk_dest);
	}
}

void copy_grid_rowset(link_t_ptr ptr_dest, link_t_ptr ptr_src)
{
	link_t_ptr rlk_dest, rlk_src;

	rlk_src = get_next_row(ptr_src, LINK_FIRST);
	while (rlk_src)
	{
		rlk_dest = insert_row(ptr_dest, LINK_LAST);
		set_row_state(rlk_dest, get_row_state(rlk_src));

		copy_grid_row(ptr_dest, rlk_dest, ptr_src, rlk_src);

		rlk_src = get_next_row(ptr_src, rlk_src);
	}
}

void update_grid_rowset(link_t_ptr ptr_dest, link_t_ptr ptr_src)
{
	link_t_ptr elk, st = NULL;
	link_t_ptr clk,rlk_dest,rlk_src;
	const tchar_t* cname;
	string_t vs = NULL;

	vs = string_alloc();

	st = create_string_table(0);

	clk = get_next_col(ptr_dest, LINK_FIRST);
	while (clk)
	{
		cname = get_col_name_ptr(clk);
		if (!get_col(ptr_src, cname))
		{
			clk = get_next_col(ptr_dest, clk);
			continue;
		}

		if (compare_text(get_col_field_type_ptr(clk), -1, ATTR_FIELD_TYPE_PRIM, -1, 0) == 0)
		{
			elk = write_string_entity(st, cname, -1, NULL, 0);
		}

		clk = get_next_col(ptr_dest, clk);
	}
	
	rlk_src = get_next_row(ptr_src, LINK_FIRST);
	while (rlk_src)
	{
		string_empty(vs);

		elk = get_string_next_entity(st, LINK_FIRST);
		while (elk)
		{
			clk = get_col(ptr_src, get_string_entity_key_ptr(elk));

			string_cat(vs, get_string_entity_key_ptr(elk), -1);
			string_cat(vs, _T(" == "), -1);

			string_cat(vs, get_cell_text_ptr(rlk_src, clk), -1);
			
			if (!is_last_link(elk))
			{
				string_cat(vs, _T(" AND "), -1);
			}

			elk = get_string_next_entity(st, elk);
		}

		rlk_dest = find_grid_row(ptr_dest, string_ptr(vs), LINK_FIRST, 1);
		if (!rlk_dest)
		{
			rlk_dest = insert_row(ptr_dest, LINK_LAST);
			set_row_state(rlk_dest, dsNewClean);
		}

		copy_grid_row(ptr_dest, rlk_dest, ptr_src, rlk_src);

		rlk_src = get_next_row(ptr_src, rlk_src);
	}

	destroy_string_table(st);
	string_free(vs);
}

void copy_grid_colsch(link_t_ptr ptr_dest, link_t_ptr ptr_src)
{
	link_t_ptr clk_dest, clk_src;
	const tchar_t* cname;

	clk_dest = get_next_col(ptr_dest, LINK_FIRST);
	while (clk_dest)
	{
		cname = get_col_name_ptr(clk_dest);
		clk_src = get_col(ptr_src, cname);
		if (clk_src)
		{
			set_col_data_type(clk_dest, get_col_data_type_ptr(clk_src));

			set_col_data_len(clk_dest, get_col_data_len(clk_src));

			set_col_data_dig(clk_dest, get_col_data_dig(clk_src));

			set_col_field_cast(clk_dest, get_col_field_cast_ptr(clk_src));
		}

		clk_dest = get_next_col(ptr_dest, clk_dest);
	}
}

link_t_ptr create_group_grid(link_t_ptr ptr, const tchar_t* scol, const tchar_t* dcol, const tchar_t* ncol)
{
	link_t_ptr grid;
	link_t_ptr rlk, sclk, dclk, nclk;
	link_t_ptr clk_fix, clk_new, rlk_new;
	string_t vs;

	sclk = get_col(ptr, scol);
	if (!sclk)
		return NULL;

	dclk = get_col(ptr, dcol);
	if (!dclk)
		return NULL;

	nclk = get_col(ptr, ncol);
	if (!nclk)
		return NULL;

	grid = create_grid_doc();
	clk_fix = insert_col(grid, LINK_LAST);
	set_col_name(clk_fix, scol);

	vs = string_alloc();

	rlk = get_next_visible_row(ptr, LINK_FIRST);
	while (rlk)
	{
		string_printf(vs, _T("%s == %s"), scol, get_cell_text_ptr(rlk, sclk));

		rlk_new = find_grid_row(grid, string_ptr(vs), LINK_FIRST, 1);
		if (!rlk_new)
		{
			rlk_new = insert_row(grid, LINK_LAST);
			set_cell_text(rlk_new, clk_fix, get_cell_text_ptr(rlk, sclk), -1);
		}

		clk_new = get_col(grid, get_cell_text_ptr(rlk, dclk));
		if (!clk_new)
		{
			clk_new = insert_col(grid, LINK_LAST);
			set_col_name(clk_new, get_cell_text_ptr(rlk, dclk));
			set_col_data_type(clk_new, get_col_data_type_ptr(nclk));
			set_col_data_dig(clk_new, get_col_data_dig(nclk));
			set_col_data_len(clk_new, get_col_data_len(nclk));
		}

		set_cell_numeric(rlk_new, clk_new, get_cell_numeric(rlk_new, clk_new) + get_cell_numeric(rlk, nclk));

		rlk = get_next_visible_row(ptr, rlk);
	}

	string_free(vs);

	return grid;
}

#endif //XDLDOC