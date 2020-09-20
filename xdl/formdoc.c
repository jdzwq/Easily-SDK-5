/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc form document

	@module	formdoc.c | implement file

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

#include "formdoc.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

void default_form_attr(link_t_ptr ptr) 
{
	set_form_style(ptr, _T("fill-color:RGB(250,250,250);stroke-style:solid;stroke-color:RGB(198,198,198);stroke-width:1;"));

	set_form_width(ptr, DEF_PAPER_WIDTH); 
	set_form_height(ptr, DEF_PAPER_HEIGHT); 
}

void default_label_field_attr(link_t_ptr flk) 
{
	set_field_style(flk, _T("font-size:12;text-align:near;line-align:center;"));

	set_field_x(flk, 0); 
	set_field_y(flk, 0); 
	set_field_width(flk,DEF_TEXT_WIDTH); 
	set_field_height(flk,DEF_TEXT_HEIGHT); 

	set_field_visible(flk,1); 
	set_field_printable(flk,1); 
}

void default_code_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("font-size:12;text-align:near;line-align:center;"));

	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, DEF_TEXT_WIDTH);
	set_field_height(flk, DEF_TEXT_HEIGHT);

	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
}

void default_pagenum_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("font-size:10;text-align:center;line-align:center;"));

	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 15);
	set_field_height(flk, 6);

	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
}

void default_text_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("font-size:12;text-align:near;line-align:center;"));

	set_field_shape(flk, ATTR_SHAPE_BOTTOMLINE);

	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, DEF_TEXT_WIDTH);
	set_field_height(flk, DEF_TEXT_HEIGHT);

	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
	set_field_nullable(flk, 1);
	set_field_editor(flk, ATTR_EDITOR_FIREEDIT);
	set_field_data_type(flk, ATTR_DATA_TYPE_STRING);
}

void default_check_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("text-color:RGB(10,10,10);"));

	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 6);
	set_field_height(flk, 6);
	set_field_editable(flk, 1);
	set_field_nullable(flk, 1);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
}

void default_tag_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("font-size:12;text-align:near;line-align:near;text-wrap:word-break;"));

	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 60);
	set_field_height(flk, 40);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
	set_field_nullable(flk, 1);
}

void default_memo_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("font-size:12;text-align:near;line-align:near;text-wrap:word-break;"));

	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 80);
	set_field_height(flk, 60);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
	set_field_nullable(flk, 1);
}

void default_photo_field_attr(link_t_ptr flk)
{
	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 30);
	set_field_height(flk, 30);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 0);
	set_field_transparent(flk, 1);
}

void default_href_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("font-size:12;text-align:near;line-align:near;"));

	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 20);
	set_field_height(flk, 25);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
}

void default_shape_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("stroke-style:solid;stroke-color:RGB(168,168,168);stroke-width:1;"));

	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 30);
	set_field_height(flk, 20);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
}

void default_rich_field_attr(link_t_ptr flk)
{
	set_field_style(flk, _T("font-size:12;text-align:near;line-align:near;text-wrap:word-break;"));

	set_field_shape(flk, ATTR_SHAPE_MULTILINE);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 100);
	set_field_height(flk, 50);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
	set_field_nullable(flk, 1);
}

void default_table_field_attr(link_t_ptr flk)
{
	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 100);
	set_field_height(flk, 50);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
	set_field_ratio(flk, 0.5);
}

void default_grid_field_attr(link_t_ptr flk)
{
	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 100);
	set_field_height(flk, 50);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
}

void default_statis_field_attr(link_t_ptr flk)
{
	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 100);
	set_field_height(flk, 50);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
}

void default_form_field_attr(link_t_ptr flk)
{
	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 100);
	set_field_height(flk, 50);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
}

void default_images_field_attr(link_t_ptr flk)
{
	set_field_shape(flk, ATTR_SHAPE_RECT);
	set_field_x(flk, 0);
	set_field_y(flk, 0);
	set_field_width(flk, 150);
	set_field_height(flk, 50);
	set_field_visible(flk, 1);
	set_field_printable(flk, 1);
	set_field_editable(flk, 1);
}

static tchar_t* _field_calc_func(const tchar_t* token,void* parm)
{
	link_t_ptr ptr = (link_t_ptr)parm;
	link_t_ptr flk;
	int len;
	tchar_t* tmp;

	flk = get_field(ptr,token);
	if(flk == NULL)
		return NULL;
	
	tmp = (tchar_t*)get_field_text_ptr(flk);
	if(is_null(tmp))
		return NULL;

	len = xslen(tmp);
	tmp = (tchar_t*)xmem_alloc((len + 1) * sizeof(tchar_t));
	get_field_text(flk,tmp,len);

	return tmp;
}

typedef struct _field_compare_param_t{
	const tchar_t* attr;
	const tchar_t* type;
	int nocase;
}field_compare_param_t;

int _field_compare_func(link_t_ptr flk1, link_t_ptr flk2, void* parm)
{
	field_compare_param_t* pcp = (field_compare_param_t*)parm;

	return compare_data(get_dom_node_attr_ptr(flk1,pcp->attr,-1),get_dom_node_attr_ptr(flk2,pcp->attr,-1),pcp->type);
}

/******************************************************************************************************/
link_t_ptr create_form_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr,DOC_FORM,-1);

	default_form_attr(ptr);

	return ptr;
}

link_t_ptr get_form_fieldset(link_t_ptr ptr)
{
	if (is_form_doc(ptr))
		return ptr;
	else
		return NULL;
}

link_t_ptr form_from_field(link_t_ptr flk)
{
	link_t_ptr nlk;

	nlk = get_dom_parent_node(flk);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, DOC_FORM, -1, 0) == 0)
			break;
		nlk = get_dom_parent_node(nlk);
	}

	return nlk;
}

void destroy_form_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_form_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(get_form_fieldset(ptr));
}

bool_t is_form_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr),-1,DOC_FORM,-1,0) == 0)? 1 : 0;
}

bool_t is_form_field(link_t_ptr ptr,link_t_ptr plk)
{
	ptr = get_form_fieldset(ptr);

	return is_dom_child_node(ptr,plk);
}

void reset_form_doc(link_t_ptr ptr)
{
	link_t_ptr flk,sub;
	const tchar_t* cls;

	flk = get_next_field(ptr,LINK_FIRST);
	while(flk)
	{
		cls = get_field_class_ptr(flk);
		if (IS_DATA_FIELD(cls))
		{
			set_field_text(flk, NULL, 0);
		}
		else if (compare_text(cls, -1, DOC_FORM_GRID, -1, 0) == 0)
		{
			sub = get_field_embed_grid(flk);
			if (sub)
			{
				clear_grid_rowset(sub);
			}
		}
		else if (compare_text(cls, -1, DOC_FORM_STATIS, -1, 0) == 0)
		{
			sub = get_field_embed_statis(flk);
			if (sub)
			{
				clear_statis_xaxset(sub);
			}
		}
		else if (compare_text(cls, -1, DOC_FORM_IMAGES, -1, 0) == 0)
		{
			sub = get_field_embed_images(flk);
			if (sub)
			{
				clear_images_doc(sub);
			}
		}
		else if (compare_text(cls, -1, DOC_FORM_RICH, -1, 0) == 0)
		{
			sub = get_field_embed_rich(flk);
			if (sub)
			{
				reset_rich_doc(sub);
			}
		}
		else if (compare_text(cls, -1, DOC_FORM_FORM, -1, 0) == 0)
		{
			sub = get_field_embed_form(flk);
			if (sub)
			{
				reset_form_doc(sub);
			}
		}

		set_field_dirty(flk, 0);

		flk = get_next_field(ptr,flk);
	}
}

void refresh_form_doc(link_t_ptr ptr)
{
	link_t_ptr flk, sub;
	const tchar_t* cls;

	flk = get_next_field(ptr, LINK_FIRST);
	while (flk)
	{
		cls = get_field_class_ptr(flk);
		if (IS_DATA_FIELD(cls))
		{
			set_field_dirty(flk, 0);
		}
		else if (compare_text(cls, -1, DOC_FORM_GRID, -1, 0) == 0)
		{
			sub = get_field_embed_grid(flk);
			if (sub)
			{
				refresh_grid_rowset(sub);
			}
			set_field_dirty(flk, 0);
		}
		else if (compare_text(cls, -1, DOC_FORM_STATIS, -1, 0) == 0)
		{
			sub = get_field_embed_statis(flk);
			if (sub)
			{
				refresh_statis_xaxset(sub);
			}
			set_field_dirty(flk, 0);
		}
		else if (compare_text(cls, -1, DOC_FORM_FORM, -1, 0) == 0)
		{
			sub = get_field_embed_form(flk);
			if (sub)
			{
				refresh_form_doc(sub);
			}
			set_field_dirty(flk, 0);
		}

		flk = get_next_field(ptr, flk);
	}
}

int get_field_count(link_t_ptr ptr)
{
	link_t_ptr flk;
	int count = 0;

	flk = get_next_field(ptr,LINK_FIRST);
	while(flk)
	{
		count ++;
		flk = get_next_field(ptr,flk);
	}
	return count;
}

int get_field_selected_count(link_t_ptr ptr)
{
	link_t_ptr flk;
	int count = 0;

	flk = get_next_field(ptr, LINK_FIRST);
	while (flk)
	{
		if (get_field_selected(flk))
			count++;
		flk = get_next_field(ptr, flk);
	}

	return count;
}

int get_update_field_count(link_t_ptr ptr)
{
	link_t_ptr flk;
	int count = 0;
	const tchar_t* cls;

	flk = get_next_field(ptr, LINK_FIRST);
	while (flk)
	{
		cls = get_field_class_ptr(flk);
		if (IS_DATA_FIELD(cls) || IS_EMBED_FIELD(cls))
		{
			count += get_field_dirty(flk);
		}

		flk = get_next_field(ptr, flk);
	}

	return count;
}

int get_field_max_group(link_t_ptr ptr)
{
	link_t_ptr flk;
	int gid = 0;

	flk = get_next_field(ptr, LINK_FIRST);
	while (flk)
	{
		if (gid < get_field_group(flk))
			gid = get_field_group(flk);

		flk = get_next_field(ptr, flk);
	}

	return gid;
}

int get_field_count_by_class(link_t_ptr ptr,const tchar_t* sz_class)
{
	link_t_ptr flk;
	int count = 0;

	flk = get_next_field(ptr,LINK_FIRST);
	while(flk)
	{
		if(compare_text(get_field_class_ptr(flk),-1,sz_class,-1,0) == 0)
			count ++;
		flk = get_next_field(ptr,flk);
	}
	return count;
}

link_t_ptr insert_field(link_t_ptr ptr,const tchar_t* sz_class)
{
	link_t_ptr doc,flk = NULL;

	if (compare_text(sz_class, -1, DOC_FORM_SHAPE, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_shape_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_LABEL, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_label_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_PAGENUM, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_pagenum_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_text_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_TAG, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_tag_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_MEMO, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_memo_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_CHECK, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_check_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_HREF, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_href_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_CODE, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_code_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_PHOTO, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_photo_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_TABLE, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_table_field_attr(flk);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_RICH, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_rich_field_attr(flk);

		doc = create_rich_doc();
		attach_dom_node(flk, LINK_LAST, doc);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_GRID, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_grid_field_attr(flk);

		doc = create_grid_doc();
		attach_dom_node(flk, LINK_LAST, doc);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_STATIS, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_statis_field_attr(flk);

		doc = create_statis_doc();
		attach_dom_node(flk, LINK_LAST, doc);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_IMAGES, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_images_field_attr(flk);

		doc = create_images_doc();
		attach_dom_node(flk, LINK_LAST, doc);
	}
	else if (compare_text(sz_class, -1, DOC_FORM_FORM, -1, 0) == 0)
	{
		flk = insert_dom_node(get_form_fieldset(ptr), LINK_LAST);
		set_dom_node_name(flk, sz_class, -1);
		default_form_field_attr(flk);

		doc = create_form_doc();
		attach_dom_node(flk, LINK_LAST, doc);
	}

	return flk;
}

void delete_field(link_t_ptr flk)
{
	delete_dom_node(flk);
}

link_t_ptr get_next_field(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr set;

	set = get_form_fieldset(ptr);

	if(pos == LINK_FIRST)
		return get_dom_first_child_node(set);
	else if(pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_prev_field(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr set;

	set = get_form_fieldset(ptr);

	if(pos == LINK_FIRST)
		return NULL;
	else if(pos == LINK_LAST)
		return get_dom_last_child_node(set);
	else
		return get_dom_prev_sibling_node(pos);
}

link_t_ptr get_next_visible_field(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr flk;
	
	flk = get_next_field(ptr,pos);
	while(flk)
	{
		if(get_field_visible(flk))
			break;
		flk = get_next_field(ptr,flk);
	}
	return flk;
}

link_t_ptr get_prev_visible_field(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr flk;
	
	flk = get_prev_field(ptr,pos);
	while(flk)
	{
		if(get_field_visible(flk))
			break;
		flk = get_prev_field(ptr,flk);
	}
	return flk;
}

link_t_ptr get_next_focusable_field(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr flk;
	const tchar_t* cls;

	flk = get_next_field(ptr,pos);
	while(flk)
	{
		cls = get_field_class_ptr(flk);

		if(get_field_visible(flk) && get_field_editable(flk))
			break;

		flk = get_next_field(ptr,flk);
	}

	return flk;
}

link_t_ptr get_prev_focusable_field(link_t_ptr ptr,link_t_ptr pos)
{
	link_t_ptr flk;
	const tchar_t* cls;

	flk = get_prev_field(ptr,pos);
	while(flk)
	{
		cls = get_field_class_ptr(flk);

		if (get_field_visible(flk) && get_field_editable(flk))
			break;

		flk = get_prev_field(ptr,flk);
	}

	return flk;
}

bool_t get_field_focusable(link_t_ptr flk)
{
	return (get_field_visible(flk) && get_field_editable(flk)) ? 1 : 0;
}

link_t_ptr get_field(link_t_ptr ptr,const tchar_t* fname)
{
	link_t_ptr flk;

	if(is_null(fname))
		return NULL;

	flk = get_next_field(ptr,LINK_FIRST);
	while(flk)
	{
		if(compare_text(get_field_name_ptr(flk),-1,fname,-1,0) == 0)
			return flk;

		flk = get_next_field(ptr,flk);
	}
	return NULL;
}

link_t_ptr get_field_by_id(link_t_ptr ptr, const tchar_t* fid)
{
	link_t_ptr flk;

	if (is_null(fid))
		return NULL;

	flk = get_next_field(ptr, LINK_FIRST);
	while (flk)
	{
		if (compare_text(get_field_id_ptr(flk), -1, fid, -1, 0) == 0)
			return flk;

		flk = get_next_field(ptr, flk);
	}
	return NULL;
}

void attach_field_text(link_t_ptr flk, tchar_t* val)
{
	attach_dom_node_text(flk, val);
}

tchar_t* detach_field_text(link_t_ptr flk)
{
	return detach_dom_node_text(flk);
}

const tchar_t* get_field_text_ptr(link_t_ptr flk)
{
	return get_dom_node_text_ptr(flk);
}

int get_field_text(link_t_ptr flk, tchar_t* buf, int max)
{
	return get_dom_node_text(flk, buf, max);
}

void set_field_text(link_t_ptr flk,const tchar_t* sz_text, int len)
{
	const tchar_t* cls;
	const tchar_t* type;
	const tchar_t* scase;
	tchar_t* tmp;
	tchar_t sz_date[DATE_LEN + 1] = { 0 };
	xdate_t md = { 0 };

	if (len < 0)
		len = xslen(sz_text);

	cls = get_field_class_ptr(flk);
	if(compare_text(cls,-1,DOC_FORM_TEXT,-1,0) == 0)
	{
		if(!len)
		{
			set_dom_node_text(flk,NULL,0);
			return;
		}
		
		type = get_field_data_type_ptr(flk);
		if(compare_text(type,-1,ATTR_DATA_TYPE_STRING,-1,0) == 0)
		{
			tmp = xsnclone(sz_text, len);

			scase = get_field_case_ptr(flk);
			if(compare_text(scase,-1,ATTR_CASE_LOWER,-1,0) == 0)
			{
				xslwr(tmp);
			}else if(compare_text(scase,-1,ATTR_CASE_UPPER,-1,0) == 0)
			{
				xsupr(tmp);
			}

			attach_dom_node_text(flk,tmp);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_BOOLEAN, -1, 0) == 0)
		{
			set_field_boolean(flk, (bool_t)xsntos(sz_text, len));
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_INTEGER, -1, 0) == 0)
		{
			set_field_integer(flk, xsntol(sz_text, len));
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_NUMERIC, -1, 0) == 0)
		{
			set_field_numeric(flk, xsntonum(sz_text, len));
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_DATE, -1, 0) == 0)
		{
			xsncpy(sz_date, sz_text, DATE_LEN);
			parse_date(&md, sz_date);
			if (verify_datetime(&md))
				set_field_date(flk, &md);
			else
				set_dom_node_text(flk, NULL, 0);
		}
		else if (compare_text(type, -1, ATTR_DATA_TYPE_DATETIME, -1, 0) == 0)
		{
			xsncpy(sz_date, sz_text, DATE_LEN);
			parse_datetime(&md, sz_date);
			if (verify_datetime(&md))
				set_field_datetime(flk, &md);
			else
				set_dom_node_text(flk, NULL, 0);
		}
	}
	else if (compare_text(cls, -1, DOC_FORM_CHECK, -1, 0) == 0)
	{
		set_dom_node_text(flk, sz_text, len);
	}
	else if (compare_text(cls, -1, DOC_FORM_LABEL, -1, 0) == 0)
	{
		set_dom_node_text(flk,sz_text, len);
	}
	else if (compare_text(cls, -1, DOC_FORM_TAG, -1, 0) == 0)
	{
		set_dom_node_text(flk, sz_text, len);
	}
	else if (compare_text(cls, -1, DOC_FORM_MEMO, -1, 0) == 0)
	{
		set_dom_node_text(flk, sz_text, len);
	}
	else if (compare_text(cls, -1, DOC_FORM_CODE, -1, 0) == 0)
	{
		set_dom_node_text(flk, sz_text, len);
	}
	else if (compare_text(cls, -1, DOC_FORM_PHOTO, -1, 0) == 0)
	{
		set_dom_node_text(flk, sz_text, len);
	}
	else if (compare_text(cls, -1, DOC_FORM_HREF, -1, 0) == 0)
	{
		set_dom_node_text(flk, sz_text, len);
	}
	else if (compare_text(cls, -1, DOC_FORM_TABLE, -1, 0) == 0)
	{
		set_dom_node_text(flk, sz_text, len);
	}
}

void set_field_boolean(link_t_ptr flk,bool_t b)
{
	set_dom_node_text(flk,((b)? _T("1") : _T("0")),-1);
}

bool_t get_field_boolean(link_t_ptr flk)
{
	return (compare_text(get_field_text_ptr(flk), -1, _T("1"), -1, 0) == 0) ? 1 : 0;
}

void set_field_integer(link_t_ptr flk,int n)
{
	tchar_t token[NUM_LEN + 1];

	xsprintf(token,_T("%d"),n);
	set_dom_node_text(flk,token,-1);
}

int get_field_integer(link_t_ptr flk)
{
	return xstol(get_field_text_ptr(flk));
}

void set_field_numeric(link_t_ptr flk,double dbl)
{
	tchar_t token[NUM_LEN + 1] = { 0 };

	numtoxs_dig(dbl, get_field_data_dig(flk), token, NUM_LEN);
	set_dom_node_text(flk,token,-1);
}

double get_field_numeric(link_t_ptr flk)
{
	return xstonum(get_field_text_ptr(flk));
}

void set_field_date(link_t_ptr flk, const xdate_t* pdt)
{
	tchar_t token[DATE_LEN + 1] = { 0 };

	format_date(pdt,token);
	
	set_dom_node_text(flk,token,-1);
}

void get_field_date(link_t_ptr flk,xdate_t* pdt)
{
	parse_date(pdt,get_field_text_ptr(flk));
}

void set_field_datetime(link_t_ptr flk,const xdate_t* pdt)
{
	tchar_t token[DATE_LEN + 1] = { 0 };

	format_datetime(pdt,token);

	set_dom_node_text(flk,token,-1);
}

void get_field_datetime(link_t_ptr flk,xdate_t* pdt)
{
	parse_datetime(pdt,get_field_text_ptr(flk));
}

link_t_ptr get_field_embed_rich(link_t_ptr flk)
{
	const tchar_t *cls;

	cls = get_field_class_ptr(flk);

	return (compare_text(cls, -1, DOC_FORM_RICH, -1, 0) == 0) ? get_dom_first_child_node(flk) : NULL;
}

void set_field_embed_rich(link_t_ptr flk, link_t_ptr doc)
{
	const tchar_t *cls;
	link_t_ptr plk;

	cls = get_field_class_ptr(flk);

	if (compare_text(cls, -1, DOC_FORM_RICH, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
			destroy_dom_doc(plk);

		attach_dom_node(flk, LINK_LAST, doc);
	}
}

link_t_ptr get_field_embed_grid(link_t_ptr flk)
{
	const tchar_t *cls;

	cls = get_field_class_ptr(flk);
	
	return (compare_text(cls, -1, DOC_FORM_GRID, -1, 0) == 0) ? get_dom_first_child_node(flk) : NULL;
}

void set_field_embed_grid(link_t_ptr flk, link_t_ptr doc)
{
	const tchar_t *cls;
	link_t_ptr plk;

	cls = get_field_class_ptr(flk);

	if (compare_text(cls, -1, DOC_FORM_GRID, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
			destroy_dom_doc(plk);

		attach_dom_node(flk, LINK_LAST, doc);
	}
}

link_t_ptr get_field_embed_statis(link_t_ptr flk)
{
	const tchar_t *cls;

	cls = get_field_class_ptr(flk);

	return (compare_text(cls, -1, DOC_FORM_STATIS, -1, 0) == 0) ? get_dom_first_child_node(flk) : NULL;
}

void set_field_embed_statis(link_t_ptr flk, link_t_ptr doc)
{
	const tchar_t *cls;
	link_t_ptr plk;

	cls = get_field_class_ptr(flk);

	if (compare_text(cls, -1, DOC_FORM_STATIS, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
			destroy_dom_doc(plk);

		attach_dom_node(flk, LINK_LAST, doc);
	}
}

link_t_ptr get_field_embed_images(link_t_ptr flk)
{
	const tchar_t *cls;

	cls = get_field_class_ptr(flk);

	return (compare_text(cls, -1, DOC_FORM_IMAGES, -1, 0) == 0) ? get_dom_first_child_node(flk) : NULL;
}

void set_field_embed_images(link_t_ptr flk, link_t_ptr doc)
{
	const tchar_t *cls;
	link_t_ptr plk;

	cls = get_field_class_ptr(flk);

	if (compare_text(cls, -1, DOC_FORM_IMAGES, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
			destroy_dom_doc(plk);

		attach_dom_node(flk, LINK_LAST, doc);
	}
}

link_t_ptr get_field_embed_form(link_t_ptr flk)
{
	const tchar_t *cls;

	cls = get_field_class_ptr(flk);

	return (compare_text(cls, -1, DOC_FORM_FORM, -1, 0) == 0) ? get_dom_first_child_node(flk) : NULL;
}

void set_field_embed_form(link_t_ptr flk, link_t_ptr doc)
{
	const tchar_t *cls;
	link_t_ptr plk;

	cls = get_field_class_ptr(flk);

	if (compare_text(cls, -1, DOC_FORM_FORM, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
			destroy_dom_doc(plk);

		attach_dom_node(flk, LINK_LAST, doc);
	}
}

void clear_field_embed(link_t_ptr flk)
{
	const tchar_t *cls;
	link_t_ptr plk;

	cls = get_field_class_ptr(flk);

	if (compare_text(cls, -1, DOC_FORM_GRID, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
		{
			clear_grid_rowset(plk);
			clear_grid_colset(plk);
			attach_dom_node(flk, LINK_FIRST, plk);
		}
	}
	else if (compare_text(cls, -1, DOC_FORM_STATIS, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
		{
			clear_statis_xaxset(plk);
			clear_statis_yaxset(plk);
			attach_dom_node(flk, LINK_FIRST, plk);
		}
	}
	else if (compare_text(cls, -1, DOC_FORM_IMAGES, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
		{
			clear_images_doc(plk);
			attach_dom_node(flk, LINK_FIRST, plk);
		}
	}
	else if (compare_text(cls, -1, DOC_FORM_RICH, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
		{
			clear_rich_doc(plk);
			attach_dom_node(flk, LINK_FIRST, plk);
		}
	}
	else if (compare_text(cls, -1, DOC_FORM_FORM, -1, 0) == 0)
	{
		plk = detach_dom_node(flk, LINK_FIRST);
		if (plk)
		{
			clear_form_doc(plk);
			attach_dom_node(flk, LINK_FIRST, plk);
		}
	}
}

void clear_field_datadef(link_t_ptr flk)
{
	if (compare_text(get_field_class_ptr(flk), -1, DOC_FORM_TEXT, -1, 0) != 0)
		return;

	set_field_data_type(flk, ATTR_DATA_TYPE_STRING);
	set_dom_node_attr(flk, ATTR_DATA_LEN, -1, NULL, 0);
	set_dom_node_attr(flk, ATTR_DATA_DIG, -1, NULL, 0);
}

const tchar_t* get_field_options_text_ptr(link_t_ptr flk)
{
	const tchar_t *sz_text, *sz_opt;

	sz_text = get_field_text_ptr(flk);

	sz_opt = get_dom_node_options_text_ptr(flk, sz_text, -1);
	return (is_null(sz_opt)) ? sz_text : sz_opt;
}

int calc_form_field(link_t_ptr ptr,link_t_ptr flk)
{
	link_t_ptr sub,macro;
	tchar_t* tmp;
	const tchar_t* cls;
	int ds = 0;

	cls = get_field_class_ptr(flk);
	if(compare_text(cls,-1,DOC_FORM_TEXT,-1,0) == 0)
	{
		tmp = (tchar_t*)get_field_macro_ptr(flk);
		if(!is_null(tmp))
		{
			macro = create_multi_tree();

			if(macro_parse(macro,tmp,-1))
			{
				tmp = macro_calc(macro,(macro_var_fetch)_field_calc_func,(void*)ptr);
				if(compare_data(tmp,get_field_text_ptr(flk),get_field_data_type_ptr(flk)) != 0) /*text is changed*/
				{
					set_field_text(flk,tmp, -1);
					ds = 1;
				}
				if(tmp)
					xmem_free(tmp);
			}

			destroy_multi_tree(macro);
		}
	}else if(compare_text(cls,-1,DOC_FORM_GRID,-1,0) == 0)
	{
		sub = get_field_embed_grid(flk);
		if (sub)
		{
			ds = (calc_grid_rowset(sub)) ? 1 : 0;

			if (get_grid_showsum(sub))
				sum_grid_colset(sub);
		}
	}
	else if (compare_text(cls, -1, DOC_FORM_FORM, -1, 0) == 0)
	{
		sub = get_field_embed_form(flk);
		if (sub)
		{
			ds = calc_form_doc(sub);
		}
	}
	
	return ds;
}

int calc_form_doc(link_t_ptr ptr)
{
	link_t_ptr flk;
	int count = 0;

	flk = get_next_field(ptr,LINK_FIRST);
	while(flk)
	{
		if(calc_form_field(ptr,flk))
			count ++;
		
		flk = get_next_field(ptr,flk);
	}
	return count;
}

int verify_form_field(link_t_ptr ptr,link_t_ptr flk)
{
	const tchar_t* cls;
	link_t_ptr obj;

	cls = get_field_class_ptr(flk);
	if (compare_text(cls, -1, DOC_FORM_TEXT, -1, 0) == 0)
	{
		return verify_text(get_field_text_ptr(flk),get_field_data_type_ptr(flk),  get_field_nullable(flk), get_field_data_len(flk), get_field_data_min_ptr(flk), get_field_data_max_ptr(flk));
	}
	else if (compare_text(cls, -1, DOC_FORM_GRID, -1, 0) == 0)
	{
		obj = get_field_embed_grid(flk);
		if (obj)
			return verify_grid_doc(obj, NULL, NULL);
	}
	else if (compare_text(cls, -1, DOC_FORM_FORM, -1, 0) == 0)
	{
		obj = get_field_embed_form(flk);
		if (obj)
			return verify_form_doc(obj, NULL);
	}

	return veValid;
}

int verify_form_doc(link_t_ptr ptr,link_t_ptr* pflk)
{
	link_t_ptr flk;
	int rt;

	if(pflk)
		*pflk = NULL;

	flk = get_next_focusable_field(ptr,LINK_FIRST);
	while(flk)
	{
		rt = verify_form_field(ptr, flk);
		if(rt != veValid)
		{
			if(pflk)
				*pflk = flk;
			return rt;
		}
		flk = get_next_focusable_field(ptr,flk);
	}
	return veValid;
}

int format_form_verify(link_t_ptr ptr, link_t_ptr flk, int code, tchar_t* buf, int max)
{
	int len;

	switch (code)
	{
	case veNull:
		len = xsprintf(buf, _T("[%s]: (null) null able checked failed"), get_field_name_ptr(flk));
		break;
	case veDatatype:
		len = xsprintf(buf, _T("[%s]: %s data type checked failed"), get_field_name_ptr(flk), get_field_text_ptr(flk));
		break;
	case veOverflow:
		len = xsprintf(buf, _T("[%s]: %s data checked overflow"), get_field_name_ptr(flk), get_field_text_ptr(flk));
		break;
	case veTruncate:
		len = xsprintf(buf, _T("[%s]: %s data will be truncated"), get_field_name_ptr(flk), get_field_text_ptr(flk));
		break;
	case veCodeSystem:
		len = xsprintf(buf, _T("[%s]: %s data not in codeSystem"), get_field_name_ptr(flk), get_field_text_ptr(flk));
		break;
	default:
		len = xsprintf(buf, _T("[%s]: %s data value checked failed"), get_field_name_ptr(flk), get_field_text_ptr(flk));
		break;
	}

	return len;
}

void reset_form_taborder(link_t_ptr ptr)
{
	field_compare_param_t cp;
	link_t_ptr nlk, root;

	link_t_ptr flk;
	int taborder = 0;
	const tchar_t* cls;

	cp.attr = ATTR_TABORDER;
	cp.type = ATTR_DATA_TYPE_INTEGER;
	cp.nocase = 0;

	nlk = get_form_fieldset(ptr);
	root = get_dom_child_node_root(nlk);

	bubble_sort_link(root, (CALLBACK_SORTLINK)_field_compare_func, 0, (void*)&cp);

	flk = get_next_field(ptr,LINK_FIRST);
	while(flk)
	{
		cls = get_field_class_ptr(flk);
		if (IS_EDITOR_FIELD(cls))
		{
			taborder += 10;
			set_field_taborder(flk,taborder);
		}
		flk = get_next_field(ptr,flk);
	}
}


#endif //XDLDOC