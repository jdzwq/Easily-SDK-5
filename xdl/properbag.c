/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc property bag document

	@module	properbag.c | implement file

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

#include "properbag.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

int write_style_attr(const tchar_t* org_style, int slen, const tchar_t* sz_key, int klen, const tchar_t* sz_val, int vlen, tchar_t* new_style, int max)
{
	link_t_ptr elk;
	int len;

	link_t_ptr st = NULL;

	if (is_null(sz_key) || !klen)
		return 0;

	st = create_string_table(0);
	string_table_parse_options(st, org_style, slen, OPT_ITEMFEED, OPT_LINEFEED);

	elk = get_string_entity(st, sz_key, klen);
	if (elk)
	{
		if (sz_val)
			set_string_entity_val(elk, sz_val, vlen);
		else
			delete_string_entity(st, elk);
	}
	else
	{
		if (sz_val)
			write_string_entity(st, sz_key, klen, sz_val, vlen);
	}

	len = string_table_format_options(st, new_style, max, OPT_ITEMFEED, OPT_LINEFEED);

	destroy_string_table(st);

	return len;
}

int read_style_attr(const tchar_t* style, int len, const tchar_t* sz_key, int klen, tchar_t* buf, int max)
{
	link_t_ptr elk;
	int rt = 0;

	link_t_ptr st = NULL;

	if (is_null(sz_key) || !klen)
		return 0;

	st = create_string_table(0);
	string_table_parse_options(st, style, len, OPT_ITEMFEED, OPT_LINEFEED);

	elk = get_string_entity(st, sz_key, klen);
	if (elk)
		rt = get_string_entity_val(elk, buf, max);

	destroy_string_table(st);

	return rt;
}


void properbag_parse_stylesheet(link_t_ptr ptr,const tchar_t* str)
{	
	link_t_ptr ent;
	xpen_t xp = {0};
	xbrush_t xb = {0};
	xfont_t xf = {0};
	xface_t xa = {0};

	parse_xpen_from_style(&xp,str);
	
	ent = write_proper(ptr,PROPERTY_BAG_PENSTYLE,-1,GDI_ATTR_STROKE_STYLE,-1,xp.style,-1);
	set_entity_options(ent,GDI_ATTR_STROKE_STYLE_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);
	
	ent = write_proper(ptr,PROPERTY_BAG_PENSTYLE,-1,GDI_ATTR_STROKE_COLOR,-1,xp.color,-1);
	set_entity_options(ent,GDI_ATTR_RGB_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr,PROPERTY_BAG_PENSTYLE,-1,GDI_ATTR_STROKE_WIDTH,-1,xp.size,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr,PROPERTY_BAG_PENSTYLE,-1,GDI_ATTR_STROKE_OPACITY,-1,xp.opacity,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIREEDIT);

	parse_xbrush_from_style(&xb,str);

	ent = write_proper(ptr,PROPERTY_BAG_BRUSHSTYLE,-1,GDI_ATTR_FILL_STYLE,-1,xb.style,-1);
	set_entity_options(ent,GDI_ATTR_FILL_STYLE_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr,PROPERTY_BAG_BRUSHSTYLE,-1,GDI_ATTR_FILL_COLOR,-1,xb.color,-1);
	set_entity_options(ent,GDI_ATTR_RGB_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr,PROPERTY_BAG_BRUSHSTYLE,-1,GDI_ATTR_FILL_OPACITY,-1,xb.opacity,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIREEDIT);

	parse_xfont_from_style(&xf,str);

	ent = write_proper(ptr,PROPERTY_BAG_FONTSTYLE,-1,GDI_ATTR_FONT_FAMILY,-1,xf.family,-1);
	//set_entity_options(ent,GDI_ATTR_FONT_FAMILY_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr,PROPERTY_BAG_FONTSTYLE,-1,GDI_ATTR_FONT_STYLE,-1,xf.style,-1);
	set_entity_options(ent,GDI_ATTR_FONT_STYLE_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr,PROPERTY_BAG_FONTSTYLE,-1,GDI_ATTR_FONT_SIZE,-1,xf.size,-1);
	set_entity_options(ent,GDI_ATTR_FONT_SIZE_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr,PROPERTY_BAG_FONTSTYLE,-1,GDI_ATTR_FONT_WEIGHT,-1,xf.weight,-1);
	set_entity_options(ent,GDI_ATTR_FONT_WEIGHT_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr, PROPERTY_BAG_FONTSTYLE, -1, GDI_ATTR_FONT_COLOR, -1, xf.color, -1);
	set_entity_options(ent, GDI_ATTR_RGB_OPTIONS, -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);

	parse_xface_from_style(&xa, str);

	ent = write_proper(ptr,PROPERTY_BAG_TEXTSTYLE,-1,GDI_ATTR_TEXT_ALIGN,-1,xa.text_align,-1);
	set_entity_options(ent,GDI_ATTR_TEXT_ALIGN_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr,PROPERTY_BAG_TEXTSTYLE,-1,GDI_ATTR_LINE_ALIGN,-1,xa.line_align,-1);
	set_entity_options(ent,GDI_ATTR_TEXT_ALIGN_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr,PROPERTY_BAG_TEXTSTYLE,-1,GDI_ATTR_TEXT_WRAP,-1,xa.text_wrap,-1);
	set_entity_options(ent,GDI_ATTR_TEXT_WRAP_OPTIONS,-1);
	set_entity_editable(ent,1);
	set_entity_editor(ent,ATTR_EDITOR_FIRELIST);

	ent = write_proper(ptr, PROPERTY_BAG_TEXTSTYLE, -1, GDI_ATTR_LINE_HEIGHT, -1, xa.line_height, -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

int properbag_format_stylesheet(link_t_ptr ptr,tchar_t* buf,int len)
{
	xpen_t xp = {0};
	xbrush_t xb = {0};
	xfont_t xf = {0};
	xface_t xa = {0};
	int size = 0;

	read_proper(ptr,PROPERTY_BAG_PENSTYLE,-1,GDI_ATTR_STROKE_STYLE,-1,xp.style,RES_LEN);
	read_proper(ptr,PROPERTY_BAG_PENSTYLE,-1,GDI_ATTR_STROKE_COLOR,-1,xp.color,CLR_LEN);
	read_proper(ptr,PROPERTY_BAG_PENSTYLE,-1,GDI_ATTR_STROKE_WIDTH,-1,xp.size,NUM_LEN);
	read_proper(ptr,PROPERTY_BAG_PENSTYLE,-1,GDI_ATTR_STROKE_OPACITY,-1,xp.opacity,NUM_LEN);

	size += format_xpen_to_style(&xp,buf + size,len - size);

	read_proper(ptr,PROPERTY_BAG_BRUSHSTYLE,-1,GDI_ATTR_FILL_STYLE,-1,xb.style,RES_LEN);
	read_proper(ptr,PROPERTY_BAG_BRUSHSTYLE,-1,GDI_ATTR_FILL_COLOR,-1,xb.color,CLR_LEN);
	read_proper(ptr,PROPERTY_BAG_BRUSHSTYLE,-1,GDI_ATTR_FILL_OPACITY,-1,xb.opacity,NUM_LEN);

	size += format_xbrush_to_style(&xb,buf + size,len - size);

	read_proper(ptr,PROPERTY_BAG_FONTSTYLE,-1,GDI_ATTR_FONT_FAMILY,-1,xf.family,RES_LEN);
	read_proper(ptr,PROPERTY_BAG_FONTSTYLE,-1,GDI_ATTR_FONT_STYLE,-1,xf.style,RES_LEN);
	read_proper(ptr,PROPERTY_BAG_FONTSTYLE,-1,GDI_ATTR_FONT_WEIGHT,-1,xf.weight,NUM_LEN);
	read_proper(ptr,PROPERTY_BAG_FONTSTYLE,-1,GDI_ATTR_FONT_SIZE,-1,xf.size,NUM_LEN);
	read_proper(ptr, PROPERTY_BAG_FONTSTYLE, -1, GDI_ATTR_FONT_COLOR, -1, xf.color, CLR_LEN);

	size += format_xfont_to_style(&xf,buf + size,len - size);

	read_proper(ptr, PROPERTY_BAG_TEXTSTYLE, -1, GDI_ATTR_TEXT_ALIGN, -1, xa.text_align, RES_LEN);
	read_proper(ptr, PROPERTY_BAG_TEXTSTYLE, -1, GDI_ATTR_LINE_ALIGN, -1, xa.line_align, RES_LEN);
	read_proper(ptr, PROPERTY_BAG_TEXTSTYLE, -1, GDI_ATTR_TEXT_WRAP, -1, xa.text_wrap, RES_LEN);
	read_proper(ptr, PROPERTY_BAG_TEXTSTYLE, -1, GDI_ATTR_LINE_HEIGHT, -1, xa.line_height, INT_LEN);

	size += format_xface_to_style(&xa,buf + size,len - size);

	return size;
}

void properbag_write_images_attributes(link_t_ptr ptr, link_t_ptr imagelist)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_IMAGES, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_LAYER, -1, get_images_layer_ptr(imagelist), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_LAYER_OPTIONS, -1);

	ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_WIDTH, get_images_item_width(imagelist));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_HEIGHT, get_images_item_height(imagelist));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_ICON_SPAN, get_images_icon_span(imagelist));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

void properbag_read_images_attributes(link_t_ptr ptr, link_t_ptr imagelist)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_IMAGES, -1, 0) != 0)
		return;

	set_images_layer(imagelist, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_LAYER));

	set_images_item_width(imagelist, (float)get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_WIDTH));

	set_images_item_height(imagelist, (float)get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_HEIGHT));

	set_images_icon_span(imagelist, (float)get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_ICON_SPAN));
}

void properbag_write_images_item_attributes(link_t_ptr ptr, link_t_ptr imageitem)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_IMAGES_IMAGE, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ALT, -1, get_images_item_alt_ptr(imageitem), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_images_item_id_ptr(imageitem), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
}

void properbag_read_images_item_attributes(link_t_ptr ptr, link_t_ptr imageitem)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_IMAGES_IMAGE, -1, 0) != 0)
		return;

	set_images_item_alt(imageitem, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ALT));

	set_images_item_id(imageitem, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));
}

void properbag_write_form_attributes(link_t_ptr ptr, link_t_ptr form)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_FORM, -1);
	set_entity_editable(ent, 0);

	//Attributes
	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_form_name_ptr(form), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_form_id_ptr(form), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_form_title_ptr(form), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_form_width(form));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_form_height(form));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

void properbag_read_form_attributes(link_t_ptr ptr, link_t_ptr form)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_FORM, -1, 0) != 0)
		return;

	set_form_name(form, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_form_id(form, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_form_title(form, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_form_width(form, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_form_height(form, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));
}

void properbag_write_field_attributes(link_t_ptr ptr, link_t_ptr flk)
{
	const tchar_t* sz_class;
	link_t_ptr ent;

	sz_class = get_field_class_ptr(flk);

	//Attributes
	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, get_field_class_ptr(flk), -1);
	set_entity_editable(ent, 0);

	if (IS_DATA_FIELD(sz_class) || IS_EMBED_FIELD(sz_class))
	{
		ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_field_id_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	}

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_field_name_ptr(flk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_X, get_field_x(flk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_Y, get_field_y(flk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_field_width(flk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_field_height(flk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	if (compare_text(sz_class, -1, DOC_FORM_TABLE, -1, 1) == 0)
	{
		ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_RATIO, get_field_ratio(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_SHAPE, -1, get_field_shape_ptr(flk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_SHAPE_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_VISIBLE, get_field_visible(flk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_PRINTABLE, get_field_printable(flk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_WRAPABLE, get_field_wrapable(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, TF_OPTIONS, -1);
	}

	if (compare_text(sz_class, -1, DOC_FORM_LABEL, -1, 1) == 0)
	{
		ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_CODEBAR, -1, get_field_codebar_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, ATTR_CODEBAR_OPTIONS, -1);
	}

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0 || compare_text(sz_class, -1, DOC_FORM_CHECK, -1, 1) == 0)
	{
		ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_FIREABLE, get_field_fireable(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, TF_OPTIONS, -1);
	}

	if (IS_DATA_FIELD(sz_class) || IS_EMBED_FIELD(sz_class))
	{
		ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_EDITABLE, get_field_editable(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, TF_OPTIONS, -1);
	}

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_EDITOR, -1, get_field_editor_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, ATTR_EDITOR_OPTIONS, -1);
	}

	if (IS_EDITOR_FIELD(sz_class))
	{
		ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_TABORDER, get_field_taborder(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}

	if (compare_text(sz_class, -1, DOC_FORM_PHOTO, -1, 1) == 0)
	{
		ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_TRANSPARENT, get_field_transparent(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, TF_OPTIONS, -1);
	}

	if (compare_text(sz_class, -1, DOC_FORM_CHECK, -1, 1) == 0)
	{
		ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_VALUE, -1, get_field_value_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_GROUP, get_field_group(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_ZERONULL, get_field_zeronull(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, TF_OPTIONS, -1);
	}

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		ent = set_proper_boolean(ptr, PROPERTY_BAG_DATA, ATTR_NULLABLE, get_field_nullable(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, TF_OPTIONS, -1);

		ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_DATA_TYPE, -1, get_field_data_type_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, ATTR_DATA_TYPE_OPTIONS, -1);

		ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_LEN, get_field_data_len(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_DIG, get_field_data_dig(flk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_DATA_MIN, -1, get_field_data_min_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

		ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_DATA_MAX, -1, get_field_data_max_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

		ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_FORMAT, -1, get_field_format_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

		ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_CASE, -1, get_field_case_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, TF_OPTIONS, -1);

		ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_MACRO, -1, get_field_macro_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	}

	if (compare_text(sz_class, -1, DOC_FORM_LABEL, -1, 1) == 0 || IS_DATA_FIELD(sz_class))
	{
		ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_DATA, -1, get_field_text_ptr(flk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	}
}

void properbag_read_field_attributes(link_t_ptr ptr, link_t_ptr flk)
{
	const tchar_t* sz_class;

	sz_class = get_field_class_ptr(flk);

	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, sz_class, -1, 0) != 0)
		return;

	set_field_name(flk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	if (IS_DATA_FIELD(sz_class) || IS_EMBED_FIELD(sz_class))
	{
		set_field_id(flk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));
	}

	set_field_x(flk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_X));

	set_field_y(flk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_Y));

	set_field_width(flk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_field_height(flk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));

	if (compare_text(sz_class, -1, DOC_FORM_TABLE, -1, 1) == 0)
	{
		set_field_ratio(flk, get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_RATIO));
	}

	set_field_shape(flk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_SHAPE));

	set_field_visible(flk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_VISIBLE));

	set_field_printable(flk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_PRINTABLE));

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		set_field_wrapable(flk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_WRAPABLE));
	}

	if (compare_text(sz_class, -1, DOC_FORM_LABEL, -1, 1) == 0)
	{
		set_field_codebar(flk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_CODEBAR));
	}

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		set_field_fireable(flk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_FIREABLE));
	}

	if (IS_DATA_FIELD(sz_class) || IS_EMBED_FIELD(sz_class))
	{
		set_field_editable(flk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_EDITABLE));
	}

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		set_field_editor(flk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_EDITOR));
	}

	if (IS_EDITOR_FIELD(sz_class))
	{
		set_field_taborder(flk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_TABORDER));
	}

	if (compare_text(sz_class, -1, DOC_FORM_PHOTO, -1, 1) == 0)
	{
		set_field_transparent(flk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_TRANSPARENT));
	}

	if (compare_text(sz_class, -1, DOC_FORM_CHECK, -1, 1) == 0)
	{
		set_field_value(flk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_VALUE));

		set_field_group(flk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_GROUP));
	}

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		set_field_zeronull(flk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_ZERONULL));
	}

	if (compare_text(sz_class, -1, DOC_FORM_TEXT, -1, 1) == 0)
	{
		set_field_nullable(flk, get_proper_boolean(ptr, PROPERTY_BAG_DATA, ATTR_NULLABLE));

		set_field_data_type(flk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_DATA_TYPE));

		set_field_data_len(flk, get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_LEN));

		set_field_data_dig(flk, get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_DIG));

		set_field_data_min(flk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_DATA_MIN));

		set_field_data_max(flk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_DATA_MAX));

		set_field_format(flk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_FORMAT));

		set_field_case(flk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_CASE));

		set_field_macro(flk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_MACRO));
	}

	if (IS_DATA_FIELD(sz_class) || compare_text(sz_class, -1, DOC_FORM_LABEL, -1, 1) == 0)
	{
		set_field_text(flk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_DATA), -1);
	}
}


void properbag_write_grid_attributes(link_t_ptr ptr, link_t_ptr grid)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_GRID, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_grid_name_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_grid_id_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_grid_title_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_grid_width(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_grid_height(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_TITLE_HEIGHT, get_grid_title_height(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_COLBAR_HEIGHT, get_grid_colbar_height(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_ROWBAR_WIDTH, get_grid_rowbar_width(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_ROWBAR_HEIGHT, get_grid_rowbar_height(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_SUBFIELD, get_grid_subfield(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_SHAPE, -1, get_grid_shape_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_SHAPE_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_SHOWSUM, get_grid_showsum(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_SHOWCHECK, get_grid_showcheck(grid));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_STEPDRAW, -1, get_grid_stepdraw_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_LAYER_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_UPDATE_TABLE, -1, get_grid_update_table_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_LIMIT_CLAUSE, -1, get_grid_limit_clause_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_FROM_CLAUSE, -1, get_grid_from_clause_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_WHERE_CLAUSE, -1, get_grid_where_clause_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_GROUP_CLAUSE, -1, get_grid_group_clause_ptr(grid), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
}

void properbag_read_grid_attributes(link_t_ptr ptr, link_t_ptr grid)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_GRID, -1, 0) != 0)
		return;

	set_grid_name(grid, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_grid_id(grid, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_grid_title(grid, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_grid_width(grid, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_grid_height(grid, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));

	set_grid_title_height(grid, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_TITLE_HEIGHT));

	set_grid_colbar_height(grid, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_COLBAR_HEIGHT));

	set_grid_rowbar_width(grid, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_ROWBAR_WIDTH));

	set_grid_rowbar_height(grid, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_ROWBAR_HEIGHT));

	set_grid_subfield(grid, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_SUBFIELD));

	set_grid_shape(grid, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_SHAPE));

	set_grid_showsum(grid, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_SHOWSUM));

	set_grid_showcheck(grid, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_SHOWCHECK));

	set_grid_stepdraw(grid, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_STEPDRAW));

	set_grid_update_table(grid, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_UPDATE_TABLE));

	set_grid_limit_clause(grid, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_LIMIT_CLAUSE));

	set_grid_from_clause(grid, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_FROM_CLAUSE));

	set_grid_where_clause(grid, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_WHERE_CLAUSE));

	set_grid_group_clause(grid, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_GROUP_CLAUSE));
}

void properbag_write_col_attributes(link_t_ptr ptr, link_t_ptr clk)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_GRID_COL, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_col_name_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_col_id_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_col_title_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_col_width(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = write_proper(ptr, PROPERTY_BAG_POSITION, -1, ATTR_ALIGNMENT, -1, get_col_alignment_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_ALIGNMENT_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_VISIBLE, get_col_visible(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_PRINTABLE, get_col_printable(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_WRAPABLE, get_col_wrapable(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_FIREABLE, get_col_fireable(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_SORTABLE, get_col_sortable(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_GROUPABLE, get_col_groupable(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_EDITABLE, get_col_editable(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_EDITOR, -1, get_col_editor_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_EDITOR_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_SUM_MODE, -1, get_col_sum_mode_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_SUM_MODE_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, GDI_ATTR_IMAGE, -1, get_col_image_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_ZERONULL, get_col_zeronull(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_DATA, ATTR_NULLABLE, get_col_nullable(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_DATA_TYPE, -1, get_col_data_type_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_DATA_TYPE_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_DATA_TYPE, -1, get_col_data_type_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_DATA_TYPE_OPTIONS, -1);

	ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_LEN, get_col_data_len(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_DIG, get_col_data_dig(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_DATA_MIN, -1, get_col_data_min_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_DATA_MAX, -1, get_col_data_max_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_FORMAT, -1, get_col_format_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_CASE, -1, get_col_case_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_MACRO, -1, get_col_macro_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_SOURCE, -1, ATTR_TABLE_NAME, -1, get_col_table_name_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_SOURCE, -1, ATTR_FIELD_NAME, -1, get_col_field_name_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_SOURCE, -1, ATTR_FIELD_TYPE, -1, get_col_field_type_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_FIELD_TYPE_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_SOURCE, -1, ATTR_FIELD_CAST, -1, get_col_field_cast_ptr(clk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_SOURCE, ATTR_UPDATEABLE, get_col_updateable(clk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);
}

void properbag_read_col_attributes(link_t_ptr ptr, link_t_ptr clk)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_GRID_COL, -1, 0) != 0)
		return;

	set_col_name(clk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_col_id(clk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_col_title(clk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_col_width(clk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_col_alignment(clk, get_proper_ptr(ptr, PROPERTY_BAG_POSITION, ATTR_ALIGNMENT));

	set_col_visible(clk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_VISIBLE));

	set_col_printable(clk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_PRINTABLE));

	set_col_wrapable(clk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_WRAPABLE));

	set_col_fireable(clk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_FIREABLE));

	set_col_sortable(clk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_SORTABLE));

	set_col_groupable(clk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_GROUPABLE));

	set_col_editable(clk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_EDITABLE));

	set_col_editor(clk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_EDITOR));

	set_col_sum_mode(clk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_SUM_MODE));

	set_col_image(clk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, GDI_ATTR_IMAGE));

	set_col_zeronull(clk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_ZERONULL));

	set_col_nullable(clk, get_proper_boolean(ptr, PROPERTY_BAG_DATA, ATTR_NULLABLE));

	set_col_data_type(clk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_DATA_TYPE));

	set_col_data_len(clk, get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_LEN));

	set_col_data_dig(clk, get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_DIG));

	set_col_data_min(clk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_DATA_MIN));

	set_col_data_max(clk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_DATA_MAX));

	set_col_format(clk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_FORMAT));

	set_col_case(clk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_CASE));

	set_col_macro(clk, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_MACRO));

	set_col_table_name(clk, get_proper_ptr(ptr, PROPERTY_BAG_SOURCE, ATTR_TABLE_NAME));

	set_col_field_name(clk, get_proper_ptr(ptr, PROPERTY_BAG_SOURCE, ATTR_FIELD_NAME));

	set_col_field_type(clk, get_proper_ptr(ptr, PROPERTY_BAG_SOURCE, ATTR_FIELD_TYPE));

	set_col_field_cast(clk, get_proper_ptr(ptr, PROPERTY_BAG_SOURCE, ATTR_FIELD_CAST));

	set_col_updateable(clk, get_proper_boolean(ptr, PROPERTY_BAG_SOURCE, ATTR_UPDATEABLE));
}

void properbag_write_graph_attributes(link_t_ptr ptr, link_t_ptr graph)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_GRAPH, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_graph_name_ptr(graph), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_graph_id_ptr(graph), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_graph_title_ptr(graph), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_graph_width(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_graph_height(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_TITLE_HEIGHT, get_graph_title_height(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_XAXBAR_WIDTH, get_graph_xaxbar_width(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_XAXBAR_HEIGHT, get_graph_xaxbar_height(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_YAXBAR_WIDTH, get_graph_yaxbar_width(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_YAXBAR_HEIGHT, get_graph_yaxbar_height(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_POSITION, ATTR_SHOWSUM, get_graph_showsum(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_XAXIS, -1, get_graph_xaxis_ptr(graph), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_XAXDT, -1, get_graph_xaxdt_ptr(graph), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_DATA_TYPE_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_DATA, -1, ATTR_XAXFMT, -1, get_graph_xaxfmt_ptr(graph), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_DATA, ATTR_XAXWRP, get_graph_xaxwrp(graph));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);
}

void properbag_read_graph_attributes(link_t_ptr ptr, link_t_ptr graph)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_GRAPH, -1, 0) != 0)
		return;

	set_graph_name(graph, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_graph_id(graph, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_graph_title(graph, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_graph_width(graph, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_graph_height(graph, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));

	set_graph_title_height(graph, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_TITLE_HEIGHT));

	set_graph_yaxbar_width(graph, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_YAXBAR_WIDTH));

	set_graph_yaxbar_height(graph, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_YAXBAR_HEIGHT));

	set_graph_xaxbar_width(graph, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_XAXBAR_WIDTH));

	set_graph_xaxbar_height(graph, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_XAXBAR_HEIGHT));

	set_graph_showsum(graph, get_proper_boolean(ptr, PROPERTY_BAG_POSITION, ATTR_SHOWSUM));

	set_graph_xaxis(graph, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_XAXIS));

	set_graph_xaxdt(graph, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_XAXDT));

	set_graph_xaxfmt(graph, get_proper_ptr(ptr, PROPERTY_BAG_DATA, ATTR_XAXFMT));

	set_graph_xaxwrp(graph, get_proper_boolean(ptr, PROPERTY_BAG_DATA, ATTR_XAXWRP));
}

void properbag_write_gax_attributes(link_t_ptr ptr, link_t_ptr glk)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_GRAPH_GAX, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_gax_name_ptr(glk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_gax_title_ptr(glk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_GRAPH_TYPE, -1, get_gax_graph_type_ptr(glk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_GRAPH_TYPE_OPTIONS, -1);

	ent = set_proper_numeric(ptr, PROPERTY_BAG_BEHAVE, ATTR_GAX_STEP, get_gax_step(glk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_numeric(ptr, PROPERTY_BAG_BEHAVE, ATTR_GAX_MIDD, get_gax_midd(glk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

void properbag_read_gax_attributes(link_t_ptr ptr, link_t_ptr glk)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_GRAPH_GAX, -1, 0) != 0)
		return;

	set_gax_name(glk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_gax_title(glk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_gax_graph_type(glk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_GRAPH_TYPE));

	set_gax_step(glk, get_proper_numeric(ptr, PROPERTY_BAG_BEHAVE, ATTR_GAX_STEP));

	set_gax_midd(glk, get_proper_numeric(ptr, PROPERTY_BAG_BEHAVE, ATTR_GAX_MIDD));
}

void properbag_write_yax_attributes(link_t_ptr ptr, link_t_ptr ylk)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_GRAPH_YAX, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_yax_name_ptr(ylk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_yax_id_ptr(ylk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_yax_title_ptr(ylk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_GROUP, -1, get_yax_group_ptr(ylk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_SORTABLE, get_yax_sortable(ylk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_LINE_CAP, -1, get_yax_line_cap_ptr(ylk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_LINE_CAP_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_COLOR, -1, get_yax_color_ptr(ylk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, GDI_ATTR_RGB_OPTIONS, -1);

	ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_LEN, get_yax_data_len(ylk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_DIG, get_yax_data_dig(ylk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_DATA, ATTR_ZERONULL, get_yax_zeronull(ylk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);
}

void properbag_read_yax_attributes(link_t_ptr ptr, link_t_ptr ylk)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_GRAPH_YAX, -1, 0) != 0)
		return;

	set_yax_name(ylk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_yax_id(ylk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_yax_title(ylk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_yax_group(ylk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_GROUP));

	set_yax_sortable(ylk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_SORTABLE));

	set_yax_line_cap(ylk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_LINE_CAP));

	set_yax_color(ylk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_COLOR));

	set_yax_data_len(ylk, get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_LEN));

	set_yax_data_dig(ylk, get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_DATA_DIG));

	set_yax_zeronull(ylk, get_proper_boolean(ptr, PROPERTY_BAG_DATA, ATTR_ZERONULL));
}

void properbag_write_topog_attributes(link_t_ptr ptr, link_t_ptr topog, int row, int col)
{
	link_t_ptr ent;
	int val;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_TOPOG, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_topog_name_ptr(topog), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_topog_id_ptr(topog), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_ROWS, get_topog_rows(topog));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_COLS, get_topog_cols(topog));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RX, get_topog_rx(topog));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RY, get_topog_ry(topog));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	if (row >= 0 && col >= 0)
	{
		ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_ROW, row);
		set_entity_editable(ent, 0);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_COL, col);
		set_entity_editable(ent, 0);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		val = (int)get_topog_matrix_value(topog, row, col);
		ent = set_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_CHECK, val);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	}
}

void properbag_read_topog_attributes(link_t_ptr ptr, link_t_ptr topog)
{
	link_t_ptr sec;
	int row, col;
	int val;

	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_TOPOG, -1, 0) != 0)
		return;

	set_topog_name(topog, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_topog_id(topog, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_topog_rows(topog, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_ROWS));

	set_topog_cols(topog, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_COLS));

	set_topog_rx(topog, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RX));

	set_topog_ry(topog, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RY));

	sec = get_section(ptr, PROPERTY_BAG_DATA, -1);
	if (sec)
	{
		row = get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_ROW);
		col = get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_COL);
		val = get_proper_integer(ptr, PROPERTY_BAG_DATA, ATTR_CHECK);
		set_topog_matrix_value(topog, row, col, val);
	}
}

void properbag_write_topog_spot_attributes(link_t_ptr ptr, link_t_ptr ilk)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_TOPOG_SPOT, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TYPE, -1, get_topog_spot_type_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, ATTR_SPOT_TYPE_OPTIONS, -1);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_topog_spot_name_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_topog_spot_id_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_topog_spot_title_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_integer(ptr, PROPERTY_BAG_POSITION, ATTR_ROW, get_topog_spot_row(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_integer(ptr, PROPERTY_BAG_POSITION, ATTR_COL, get_topog_spot_col(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

void properbag_read_topog_spot_attributes(link_t_ptr ptr, link_t_ptr ilk)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_TOPOG_SPOT, -1, 0) != 0)
		return;

	set_topog_spot_type(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TYPE));

	set_topog_spot_name(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_topog_spot_id(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_topog_spot_title(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_topog_spot_row(ilk, get_proper_integer(ptr, PROPERTY_BAG_POSITION, ATTR_ROW));

	set_topog_spot_col(ilk, get_proper_integer(ptr, PROPERTY_BAG_POSITION, ATTR_COL));
}

void properbag_write_rich_attributes(link_t_ptr ptr, link_t_ptr rich)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_RICH, -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_rich_name_ptr(rich), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_rich_id_ptr(rich), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_rich_title_ptr(rich), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_IDENTIFY, ATTR_DYNAMIC, get_rich_dynamic(rich));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_rich_width(rich));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_rich_height(rich));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

void properbag_read_rich_attributes(link_t_ptr ptr, link_t_ptr rich)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_RICH, -1, 0) != 0)
		return;

	set_rich_name(rich, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_rich_id(rich, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_rich_title(rich, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_rich_dynamic(rich, get_proper_boolean(ptr, PROPERTY_BAG_IDENTIFY, ATTR_DYNAMIC));

	set_rich_width(rich, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_rich_height(rich, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));
}

void properbag_write_rich_anch_attributes(link_t_ptr ptr, link_t_ptr ilk)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, _T("rich-node"), -1);
	set_entity_editable(ent, 0);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_rich_anch_name_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_rich_anch_title_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_TEXT_INDENT, get_rich_anch_text_indent(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_TEXT_PLACE, get_rich_anch_text_place(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_FIXED, get_rich_anch_fixed(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);

	ent = set_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_LINED, get_rich_anch_lined(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	set_entity_options(ent, TF_OPTIONS, -1);
}

void properbag_read_rich_anch_attributes(link_t_ptr ptr, link_t_ptr ilk)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, _T("rich-node"), -1, 0) != 0)
		return;

	set_rich_anch_name(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_rich_anch_title(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_rich_anch_text_indent(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_TEXT_INDENT));

	set_rich_anch_text_place(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_TEXT_PLACE));

	set_rich_anch_fixed(ilk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_FIXED));

	set_rich_anch_lined(ilk, get_proper_boolean(ptr, PROPERTY_BAG_BEHAVE, ATTR_LINED));
}

void properbag_write_dialog_attributes(link_t_ptr ptr, link_t_ptr dialog)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_DIALOG, -1);
	set_entity_editable(ent, 0);

	//Attributes
	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_dialog_name_ptr(dialog), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_dialog_id_ptr(dialog), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_TITLE, -1, get_dialog_title_ptr(dialog), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_dialog_width(dialog));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_dialog_height(dialog));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

void properbag_read_dialog_attributes(link_t_ptr ptr, link_t_ptr dialog)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_DIALOG, -1, 0) != 0)
		return;

	set_dialog_name(dialog, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_dialog_id(dialog, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_dialog_title(dialog, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_TITLE));

	set_dialog_width(dialog, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_dialog_height(dialog, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));
}

void properbag_write_dialog_item_attributes(link_t_ptr ptr, link_t_ptr ilk)
{
	link_t_ptr ent;
	const tchar_t* sz_class;

	sz_class = get_dialog_item_class_ptr(ilk);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, sz_class, -1);
	set_entity_editable(ent, 0);

	//Attributes
	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_dialog_item_name_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_dialog_item_id_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_DATA, -1, get_dialog_item_text_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	if (compare_text(sz_class, -1, DOC_DIALOG_SHAPEBOX, -1, 0) == 0)
	{
		set_entity_options(ent, ATTR_SHAPE_OPTIONS, -1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	}
	else if (compare_text(sz_class, -1, DOC_DIALOG_USERBOX, -1, 0) == 0)
	{
		set_entity_options(ent, ATTR_CONTROL_OPTIONS, -1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
	}
	else
	{
		set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);
	}

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_X, get_dialog_item_x(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_Y, get_dialog_item_y(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_dialog_item_width(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_dialog_item_height(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

void properbag_read_dialog_item_attributes(link_t_ptr ptr, link_t_ptr ilk)
{
	set_dialog_item_name(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_dialog_item_id(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_dialog_item_text(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_DATA), -1);

	set_dialog_item_x(ilk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_X));

	set_dialog_item_y(ilk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_Y));

	set_dialog_item_width(ilk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_dialog_item_height(ilk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));
}

void properbag_write_chart_attributes(link_t_ptr ptr, link_t_ptr chart)
{
	link_t_ptr ent;

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, DOC_CHART, -1);
	set_entity_editable(ent, 0);

	//Attributes
	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_chart_name_ptr(chart), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_chart_id_ptr(chart), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_chart_width(chart));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_chart_height(chart));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
}

void properbag_read_chart_attributes(link_t_ptr ptr, link_t_ptr chart)
{
	if (compare_text(get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_CLASS), -1, DOC_CHART, -1, 0) != 0)
		return;

	set_chart_name(chart, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_chart_id(chart, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_chart_width(chart, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_chart_height(chart, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));
}

void properbag_write_chart_table_attributes(link_t_ptr ptr, link_t_ptr ilk)
{
	link_t_ptr ent;
	const tchar_t* sz_class;

	sz_class = get_chart_table_class_ptr(ilk);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_CLASS, -1, sz_class, -1);
	set_entity_editable(ent, 0);

	//Attributes
	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_NAME, -1, get_chart_table_name_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = write_proper(ptr, PROPERTY_BAG_IDENTIFY, -1, ATTR_ID, -1, get_chart_table_id_ptr(ilk), -1);
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIREEDIT);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_X, get_chart_table_x(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_Y, get_chart_table_y(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH, get_chart_table_width(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	ent = set_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT, get_chart_table_height(ilk));
	set_entity_editable(ent, 1);
	set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

	if (compare_text(sz_class, -1, DOC_CHART_MAP, -1, 0) == 0)
	{
		ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_ROWS, get_chart_map_table_rows(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_COLS, get_chart_map_table_cols(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RX, get_chart_map_table_rx(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RY, get_chart_map_table_ry(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}

	if (compare_text(sz_class, -1, DOC_CHART_DOT, -1, 0) == 0)
	{
		ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_ROWS, get_chart_dot_table_rows(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_COLS, get_chart_dot_table_cols(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RX, get_chart_dot_table_rx(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RY, get_chart_dot_table_ry(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}

	if (compare_text(sz_class, -1, DOC_CHART_COUNTER, -1, 0) == 0)
	{
		ent = write_proper(ptr, PROPERTY_BAG_BEHAVE, -1, ATTR_LAYER, -1, get_chart_counter_table_layer_ptr(ilk), -1);
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRELIST);
		set_entity_options(ent, ATTR_LAYER_OPTIONS, -1);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_SIZE, get_chart_counter_table_size(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}

	if (compare_text(sz_class, -1, DOC_CHART_LINE, -1, 0) == 0)
	{
		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_SIZE, get_chart_line_table_size(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_BASE, get_chart_line_table_base(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_SPAN, get_chart_line_table_span(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}

	if (compare_text(sz_class, -1, DOC_CHART_BAR, -1, 0) == 0)
	{
		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_SIZE, get_chart_bar_table_size(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_BASE, get_chart_bar_table_base(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);

		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_SPAN, get_chart_bar_table_span(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}

	if (compare_text(sz_class, -1, DOC_CHART_PIE, -1, 0) == 0)
	{
		ent = set_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_SIZE, get_chart_bar_table_size(ilk));
		set_entity_editable(ent, 1);
		set_entity_editor(ent, ATTR_EDITOR_FIRENUM);
	}
}

void properbag_read_chart_table_attributes(link_t_ptr ptr, link_t_ptr ilk)
{
	const tchar_t* sz_class;

	sz_class = get_chart_table_class_ptr(ilk);

	set_chart_table_name(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_NAME));

	set_chart_table_id(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_ID));

	set_chart_table_text(ilk, get_proper_ptr(ptr, PROPERTY_BAG_IDENTIFY, ATTR_DATA), -1);

	set_chart_table_x(ilk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_X));

	set_chart_table_y(ilk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_Y));

	set_chart_table_width(ilk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_WIDTH));

	set_chart_table_height(ilk, (float)get_proper_float(ptr, PROPERTY_BAG_POSITION, ATTR_HEIGHT));

	if (compare_text(sz_class, -1, DOC_CHART_MAP, -1, 0) == 0)
	{
		set_chart_map_table_rows(ilk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_ROWS));

		set_chart_map_table_cols(ilk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_COLS));

		set_chart_map_table_rx(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RX));

		set_chart_map_table_ry(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RY));
	}

	if (compare_text(sz_class, -1, DOC_CHART_DOT, -1, 0) == 0)
	{
		set_chart_dot_table_rows(ilk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_ROWS));

		set_chart_dot_table_cols(ilk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_COLS));

		set_chart_dot_table_rx(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RX));

		set_chart_dot_table_ry(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_RY));
	}

	if (compare_text(sz_class, -1, DOC_CHART_COUNTER, -1, 0) == 0)
	{
		set_chart_counter_table_layer(ilk, get_proper_ptr(ptr, PROPERTY_BAG_BEHAVE, ATTR_LAYER));

		set_chart_counter_table_size(ilk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_SIZE));
	}

	if (compare_text(sz_class, -1, DOC_CHART_LINE, -1, 0) == 0)
	{
		set_chart_line_table_size(ilk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_SIZE));

		set_chart_line_table_base(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_BASE));

		set_chart_line_table_span(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_SPAN));
	}

	if (compare_text(sz_class, -1, DOC_CHART_BAR, -1, 0) == 0)
	{
		set_chart_bar_table_size(ilk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_SIZE));

		set_chart_bar_table_base(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_BASE));

		set_chart_bar_table_span(ilk, get_proper_float(ptr, PROPERTY_BAG_BEHAVE, ATTR_SPAN));
	}

	if (compare_text(sz_class, -1, DOC_CHART_PIE, -1, 0) == 0)
	{
		set_chart_bar_table_size(ilk, get_proper_integer(ptr, PROPERTY_BAG_BEHAVE, ATTR_SIZE));
	}
}

#endif //XDLDOC