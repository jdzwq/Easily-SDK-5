/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc document utility

	@module	docbag.c | bag for document implement file

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

#include "docbag.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlinit.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDK_SUPPORT_FILE)

bool_t save_xml_doc_to_file(link_t_ptr xml, const secu_desc_t* psd, const tchar_t* fname)
{
	file_t xf = NULL;
	byte_t* buf = NULL;
	dword_t size;

	TRY_CATCH;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	size = format_xml_doc_to_bytes(xml, NULL, MAX_LONG);

	buf = (byte_t*)xmem_alloc(size);

	format_xml_doc_to_bytes(xml, buf, size);

	if (!xfile_write(xf, buf, size))
	{
		raise_user_error(NULL, NULL);
	}
	
	xmem_free(buf);
	buf = NULL;

	xfile_close(xf);
	xf = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (buf)
		xmem_free(buf);

	if (xf)
		xfile_close(xf);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return 0;
}

bool_t load_xml_doc_from_file(link_t_ptr xml, const secu_desc_t* psd, const tchar_t* fname)
{
	file_t xf = NULL;
	byte_t* buf = NULL;

	tchar_t fsize[INT_LEN] = { 0 };
	dword_t size;

	TRY_CATCH;

	if (!xfile_info(psd, fname, NULL, fsize, NULL, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize))
	{
		raise_user_error(_T("0"), _T("not support huge size file"));
	}

	xf = xfile_open(psd, fname, FILE_OPEN_READ);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	size = xstol(fsize);
	buf = (byte_t*)xmem_alloc(size);

	if (!xfile_read(xf, buf, size))
	{
		raise_user_error(NULL, NULL);
	}

	if (!parse_xml_doc_from_bytes(xml, buf, size))
	{
		raise_user_error(NULL, NULL);
	}

	xmem_free(buf);
	buf = NULL;
	
	xfile_close(xf);
	xf = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (buf)
		xmem_free(buf);

	if (xf)
		xfile_close(xf);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return 0;
}

bool_t	save_dom_doc_to_file(link_t_ptr dom, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr xml;
	bool_t rt;

	xml = upcast_dom_to_xml(dom);
	rt = save_xml_doc_to_file(xml, psd, fname);
	downcast_xml_to_dom(xml);

	return rt;
}

bool_t	load_dom_doc_from_file(link_t_ptr dom, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr xml;
	bool_t rt;

	xml = upcast_dom_to_xml(dom);
	rt = load_xml_doc_from_file(xml, psd, fname);
	downcast_xml_to_dom(xml);

	return rt;
}

link_t_ptr	create_images_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta, ptr;

	meta = create_meta_doc();
	if (!load_dom_doc_from_file(meta, psd, fname))
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	if (compare_text(get_meta_doc_name_ptr(meta), -1, DOC_IMAGES, -1, 1) != 0)
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return ptr;
}

bool_t save_images_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta;
	bool_t rt;

	meta = create_meta_doc();

	attach_meta_body_node(meta, ptr);

	rt = save_dom_doc_to_file(meta, psd, fname);

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return rt;
}

link_t_ptr	create_form_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta,ptr;

	meta = create_meta_doc();
	if (!load_dom_doc_from_file(meta, psd, fname))
	{
		destroy_meta_doc(meta);
		return NULL;
	}
	
	if (compare_text(get_meta_doc_name_ptr(meta), -1, DOC_FORM, -1, 1) != 0)
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return ptr;
}

bool_t save_form_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta;
	bool_t rt;

	meta = create_meta_doc();

	attach_meta_body_node(meta, ptr);

	rt = save_dom_doc_to_file(meta, psd, fname);

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return rt;
}

link_t_ptr	create_grid_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta, ptr;

	meta = create_meta_doc();
	if (!load_dom_doc_from_file(meta, psd, fname))
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	if (compare_text(get_meta_doc_name_ptr(meta), -1, DOC_GRID, -1, 1) != 0)
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return ptr;
}

bool_t save_grid_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta;
	bool_t rt;

	meta = create_meta_doc();

	attach_meta_body_node(meta, ptr);

	rt = save_dom_doc_to_file(meta, psd, fname);

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return rt;
}

link_t_ptr	create_graph_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta, ptr;

	meta = create_meta_doc();
	if (!load_dom_doc_from_file(meta, psd, fname))
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	if (compare_text(get_meta_doc_name_ptr(meta), -1, DOC_GRAPH, -1, 1) != 0)
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return ptr;
}

bool_t save_graph_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta;
	bool_t rt;

	meta = create_meta_doc();

	attach_meta_body_node(meta, ptr);

	rt = save_dom_doc_to_file(meta, psd, fname);

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return rt;
}

link_t_ptr	create_dialog_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta, ptr;

	meta = create_meta_doc();
	if (!load_dom_doc_from_file(meta, psd, fname))
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	if (compare_text(get_meta_doc_name_ptr(meta), -1, DOC_DIALOG, -1, 1) != 0)
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return ptr;
}

bool_t save_dialog_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta;
	bool_t rt;

	meta = create_meta_doc();

	attach_meta_body_node(meta, ptr);

	rt = save_dom_doc_to_file(meta, psd, fname);

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return rt;
}

link_t_ptr	create_chart_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta, ptr;

	meta = create_meta_doc();
	if (!load_dom_doc_from_file(meta, psd, fname))
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	if (compare_text(get_meta_doc_name_ptr(meta), -1, DOC_CHART, -1, 1) != 0)
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return ptr;
}

bool_t save_chart_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta;
	bool_t rt;

	meta = create_meta_doc();

	attach_meta_body_node(meta, ptr);

	rt = save_dom_doc_to_file(meta, psd, fname);

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return rt;
}

link_t_ptr	create_topog_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta, ptr;

	meta = create_meta_doc();
	if (!load_dom_doc_from_file(meta, psd, fname))
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	if (compare_text(get_meta_doc_name_ptr(meta), -1, DOC_TOPOG, -1, 1) != 0)
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return ptr;
}

bool_t save_topog_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta;
	bool_t rt;

	meta = create_meta_doc();

	attach_meta_body_node(meta, ptr);

	rt = save_dom_doc_to_file(meta, psd, fname);

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return rt;
}

link_t_ptr	create_rich_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta, ptr;

	meta = create_meta_doc();
	if (!load_dom_doc_from_file(meta, psd, fname))
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	if (compare_text(get_meta_doc_name_ptr(meta), -1, DOC_RICH, -1, 1) != 0)
	{
		destroy_meta_doc(meta);
		return NULL;
	}

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return ptr;
}

bool_t save_rich_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr meta;
	bool_t rt;

	meta = create_meta_doc();

	attach_meta_body_node(meta, ptr);

	rt = save_dom_doc_to_file(meta, psd, fname);

	ptr = detach_meta_body_node(meta);
	destroy_meta_doc(meta);

	return rt;
}

link_t_ptr	create_schema_doc_from_file(const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	if (!load_dom_doc_from_file(ptr, psd, fname))
	{
		destroy_dom_doc(ptr);
		return NULL;
	}

	if (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_SCHEMA, -1, 1) != 0)
	{
		destroy_dom_doc(ptr);
		return NULL;
	}

	return ptr;
}

bool_t save_schema_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	return save_dom_doc_to_file(ptr, psd, fname);
}

int save_text_to_file(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* buf, int len)
{
	file_t xf = NULL;
	byte_t* data = NULL;
	dword_t size;
	int skip;

	TRY_CATCH;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);

	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	skip = format_utfbom(DEF_MBS, NULL);

#ifdef _UNICODE
	size = ucs_to_mbs(buf, len, NULL, MAX_LONG);
#else
	size = mbs_to_mbs(buf, len, NULL, MAX_LONG);
#endif

	data = (byte_t*)xmem_alloc(skip + size);

	format_utfbom(DEF_MBS, data);

#ifdef _UNICODE
	size = ucs_to_mbs(buf, len, (schar_t*)(data + skip), size);
#else
	size = mbs_to_mbs(buf, len, (schar_t*)(data + skip), size);
#endif

	size += skip;

	if (!xfile_write(xf, data, size))
	{
		raise_user_error(NULL, NULL);
	}
	
	xfile_close(xf);
	xf = NULL;

	xmem_free(data);
	data = NULL;

	END_CATCH;

	return len;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (data)
		xmem_free(data);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return C_ERR;
}

int load_text_from_file(const secu_desc_t* psd, const tchar_t* fname, tchar_t* buf, int max)
{
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	dword_t dw, size;
	int skip, encode;

	file_t xf = NULL;
	byte_t* data = NULL;

	TRY_CATCH;

	if (!xfile_info(psd, fname, NULL, fsize, NULL, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize))
	{
		raise_user_error(_T("0"), _T("not support huge size file"));
	}

	xf = xfile_open(psd, fname, FILE_OPEN_READ);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	size = xstol(fsize);
	data = (byte_t*)xmem_alloc(size);

	if (!xfile_read(xf, data, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	encode = parse_utfbom(data, size);
	skip = skip_utfbom(data);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		dw = utf8_to_ucs(data + skip, size - skip, buf, max);
#else
		dw = utf8_to_mbs(data + skip, size - skip, buf, max);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		dw = utf16lit_to_ucs(data + skip, size - skip, buf, max);
#else
		dw = utf16lit_to_mbs(data + skip, size - skip, buf, max);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		dw = utf16big_to_ucs(data + skip, size - skip, buf, max);
#else
		dw = utf16big_to_mbs(data + skip, size - skip, buf, max);
#endif
		break;
	default:
#ifdef _UNICODE
		dw = mbs_to_ucs((schar_t*)(data + skip), size - skip, buf, max);
#else
		dw = mbs_to_mbs((schar_t*)(data + skip), size - skip, buf, max);
#endif
		break;
	}

	xmem_free(data);
	data = NULL;

	END_CATCH;

	return (int)dw;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (data)
		xmem_free(data);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return C_ERR;
}

bool_t load_field_object_from_file(link_t_ptr flk, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr doc;
	const tchar_t* fclass;

	int bsize;
	tchar_t* base_buf = NULL;

	fclass = get_field_class_ptr(flk);

	if (compare_text(fclass, -1, DOC_FORM_GRID, -1, 0) == 0)
	{
		doc = create_grid_doc_from_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_grid(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_GRAPH, -1, 0) == 0)
	{
		doc = create_graph_doc_from_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_graph(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_RICH, -1, 0) == 0)
	{
		doc = create_rich_doc_from_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_rich(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_IMAGES, -1, 0) == 0)
	{
		doc = create_images_doc_from_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_images(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_FORM, -1, 0) == 0)
	{
		doc = create_form_doc_from_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_form(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_PHOTO, -1, 0) == 0)
	{
		bsize = load_image_base64_from_file(psd, fname, NULL, MAX_LONG);
		
		base_buf = xsalloc(bsize + 1);

		load_image_base64_from_file(psd, fname, base_buf, bsize);

		attach_dom_node_text(flk, base_buf);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_MEMO, -1, 0) == 0 || compare_text(fclass, -1, DOC_FORM_TAG, -1, 0) == 0)
	{
		bsize = load_text_from_file(psd, fname, NULL, MAX_LONG);
		if (bsize == C_ERR)
			return 0;

		base_buf = xsalloc(bsize + 1);
		load_text_from_file(psd, fname, base_buf, bsize);

		attach_dom_node_text(flk, base_buf);

		return 1;
	}

	return 0;
}

int format_grid_colset_to_csv(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr clk;
	int len, size;
	const tchar_t* str;

	size = 0;
	if (size >= max)
		return size;

	clk = get_next_visible_col(ptr, LINK_FIRST);
	while (clk)
	{
		str = get_col_title_ptr(clk);
		len = xslen(str);
		if (size + len > max)
			return size;

		if (buf)
		{
			xsncpy(buf + size, str, len);
		}
		size += len;

		if (size + 1 > max)
			return size;

		if (buf)
		{
			buf[size] = TXT_ITEMFEED;
		}
		size += 1;

		clk = get_next_visible_col(ptr, clk);
	}

	if (buf)
	{
		if (size && buf[size - 1] == TXT_ITEMFEED)
			buf[size - 1] = TXT_LINEFEED;
	}

	if (size + 1 > max)
		return size;

	if (buf)
	{
		buf[size] = CSV_LINEFEED;
	}
	size += 1;

	return size;
}

int format_grid_rowset_to_csv(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr clk, rlk;
	int len, size;
	const tchar_t* str;

	size = 0;
	if (size >= max)
		return size;

	rlk = get_next_visible_row(ptr, LINK_FIRST);
	while (rlk)
	{
		clk = get_next_visible_col(ptr, LINK_FIRST);
		while (clk)
		{
			str = get_cell_text_ptr(rlk, clk);
			len = xslen(str);
			if (size + len > max)
				return size;

			if(buf)
			{
				get_cell_text(rlk, clk, buf + size, len);
			}
			size += len;

			if (size + 1 > max)
				return size;

			if (buf)
			{
				buf[size] = TXT_ITEMFEED;
			}
			size += 1;

			clk = get_next_visible_col(ptr, clk);
		}

		if (buf)
		{
			if (size && buf[size - 1] == TXT_ITEMFEED)
				buf[size - 1] = TXT_LINEFEED;
		}

		if (size + 1 > max)
			return size;

		if (buf)
		{
			buf[size] = CSV_LINEFEED;
		}
		size += 1;

		rlk = get_next_visible_row(ptr, rlk);
	}

	return size;
}

int parse_grid_colset_from_csv(link_t_ptr ptr, const tchar_t* flat, int len)
{
	link_t_ptr clk;
	tchar_t* token;
	tchar_t* prev;
	tchar_t* colname;
	int total = 0;

	if (len < 0)
		len = xslen(flat);

	clear_grid_rowset(ptr);
	clear_grid_colset(ptr);

	token = (tchar_t*)flat;
	while (total < len)
	{
		prev = token;
		while (*token != CSV_ITEMFEED && *token != CSV_LINEFEED && *token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0') && total < len)
		{
			token++;
			total++;
		}

		clk = insert_col(ptr, LINK_LAST);

		colname = xsnclone(prev, (int)(token - prev));
		set_col_name(clk, colname);
		set_col_title(clk, colname);
		xsfree(colname);

		if (*token == CSV_LINEFEED || *token == TXT_LINEFEED || *token == _T('\0') || total >= len)
			break;

		token++;
		total++;
	}

	while ((*token == CSV_LINEFEED || *token == TXT_LINEFEED) && total < len)
	{
		token++;
		total++;
	}

	return total;
}

int parse_grid_rowset_from_csv(link_t_ptr ptr, const tchar_t* flat, int len)
{
	link_t_ptr clk, rlk;
	tchar_t* token;
	tchar_t* prev;
	int total = 0;

	if (len <= 0)
		return 0;

	clear_grid_rowset(ptr);

	token = (tchar_t*)flat;
	while (total < len)
	{
		rlk = insert_row(ptr, LINK_LAST);

		clk = get_next_visible_col(ptr, LINK_FIRST);
		while (clk)
		{
			prev = token;
			while (*token != CSV_ITEMFEED && *token != CSV_LINEFEED && *token != TXT_ITEMFEED && *token != TXT_LINEFEED && *token != _T('\0') && total < len)
			{
				token++;
				total++;
			}

			set_cell_text(rlk, clk, prev, (int)(token - prev));

			if (*token == CSV_LINEFEED || *token == TXT_LINEFEED || *token == _T('\0') || total >= len)
				break;

			token++;
			total++;

			clk = get_next_visible_col(ptr, clk);
		}

		//skip linefeed
		while (*token != CSV_LINEFEED && *token != TXT_LINEFEED && *token != _T('\0') && total < len)
		{
			token++;
			total++;
		}

		while ((*token == CSV_LINEFEED || *token == TXT_LINEFEED) && total < len)
		{
			token++;
			total++;
		}
	}

	return total;
}

bool_t save_grid_doc_to_csv_file(link_t_ptr ptr, bool_t head, const secu_desc_t* psd, const tchar_t* fname)
{
	int skip, len;
	tchar_t* str;
	dword_t size;

	file_t xf = NULL;
	byte_t* buf = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	if (head)
		len = format_grid_colset_to_csv(ptr, NULL, MAX_LONG) + format_grid_rowset_to_csv(ptr, NULL, MAX_LONG);
	else
		len = format_grid_rowset_to_csv(ptr, NULL, MAX_LONG);

	str = xsalloc(len + 1);

	if (head)
	{
		skip = format_grid_rowset_to_csv(ptr, str, len);
	}
	else
	{
		skip = 0;
	}
	
	format_grid_rowset_to_csv(ptr, str + skip, len - skip);

	skip = format_utfbom(DEF_MBS, NULL);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, NULL, MAX_LONG);
#else
	size = mbs_to_mbs(str, len, NULL, MAX_LONG);
#endif

	buf = (byte_t*)xmem_alloc(skip + size);

	format_utfbom(DEF_MBS, buf);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, (schar_t*)(buf + skip), size);
#else
	size = mbs_to_mbs(str, len, (schar_t*)(buf + skip), size);
#endif

	xsfree(str);

	size += skip;
	if (!xfile_write(xf, buf, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	xmem_free(buf);
	buf = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (buf)
		xmem_free(buf);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return 0;
}

bool_t load_grid_doc_from_csv_file(link_t_ptr ptr, bool_t head, const secu_desc_t* psd, const tchar_t* fname)
{
	dword_t size;
	int skip,len;
	int encode = 0;
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	
	xhand_t xf = NULL;
	byte_t* buf = NULL;
	tchar_t* str = NULL;

	TRY_CATCH;

	if (!xfile_info(psd, fname, NULL, fsize, NULL, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize))
	{
		raise_user_error(_T("0"), _T("not support large file"));
	}

	xf = xfile_open(psd, fname, 0);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	size = xstol(fsize);
	buf = (byte_t*)xmem_alloc(size + 1);

	if (!xfile_read(xf, buf, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	encode = parse_utfbom(buf, size);
	skip = skip_utfbom(buf);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		len = utf8_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf8_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		len = utf16lit_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf16lit_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		len = utf16big_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf16big_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	default:
#ifdef _UNICODE
		len = mbs_to_ucs((schar_t*)(buf + skip), size - skip, NULL, MAX_LONG);
#else
		len = mbs_to_mbs((schar_t*)(buf + skip), size - skip, NULL, MAX_LONG);
#endif
		break;
	}

	str = xsalloc(len + 1);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		len = utf8_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf8_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		len = utf16lit_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf16lit_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		len = utf16big_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf16big_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	default:
#ifdef _UNICODE
		len = mbs_to_ucs((schar_t*)(buf + skip), size - skip, str, len);
#else
		len = mbs_to_mbs((schar_t*)(buf + skip), size - skip, str, len);
#endif
		break;
	}

	xmem_free(buf);
	buf = NULL;

	if (head)
		skip = parse_grid_colset_from_csv(ptr, str, len);
	else
		skip = 0;

	parse_grid_rowset_from_csv(ptr, str + skip, len - skip);

	xsfree(str);
	str = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (buf)
		xmem_free(buf);

	if (str)
		xsfree(str);

	return 0;
}

tchar_t* _parse_ini_line(tchar_t* str, tchar_t** pkey, int* pkeylen, tchar_t** pval, int* pvallen, int* prt)
{
	tchar_t* token = str;
	int sign = 0;

	*pkey = *pval = NULL;
	*pkeylen = *pvallen = 0;
	while ((*token == _T('\r') || *token == _T('\n') || *token == _T(' ') || *token == _T('\t')) && *token != _T('\0'))
		token++;
	if (*token == _T('\0'))
		return NULL;

	if (*token == _T(';'))
		*prt = 1;
	else if (*token == _T('['))
	{
		*prt = 2;
		token++;
		*pkey = token;
	}
	else
	{
		*prt = 3;
		*pkey = token;
	}

	while (*token != _T('\r') && *token != _T('\n') && *token != _T('\0'))
	{
		if (*token == _T('='))
		{
			sign = 1;
			token++;
			continue;
		}

		if (*prt == 2)
		{
			if (*token == _T(']'))
			{
				token++;
				break;
			}
			else
				*pkeylen = *pkeylen + 1;
		}

		else if (*prt == 3)
		{
			if (sign == 0)
				*pkeylen = *pkeylen + 1;
			else
			{
				if (*pval == NULL)
					*pval = token;
				*pvallen = *pvallen + 1;
			}
		}

		token++;
	}

	return token;
}

void parse_proper_doc_from_ini(link_t_ptr ptr, const tchar_t* str, int len)
{
	tchar_t *val, *key, *sec, *tmp, *buf;
	int vallen, keylen, seclen;
	int rt;

	buf = (tchar_t*)str;
	sec = NULL;
	seclen = 0;
	while (buf = _parse_ini_line(buf, &key, &keylen, &val, &vallen, &rt))
	{
		if (rt == 2 && keylen)
		{
			sec = key;
			seclen = keylen;
			tmp = sec + keylen;
			while (*tmp == _T(' ') || *tmp == _T('\t') || *tmp == _T('\0'))
			{
				if (tmp == key)
					break;
				seclen--;
				tmp--;
			}
		}
		else if (rt == 3 && keylen)
		{
			tmp = key + keylen;
			while (*tmp == _T(' ') || *tmp == _T('\t') || *tmp == _T('\0'))
			{
				if (tmp == key)
					break;
				keylen--;
				tmp--;
			}

			if (val && vallen)
			{
				tmp = val + vallen;
				while (*tmp == _T(' ') || *tmp == _T('\t') || *tmp == _T('\0'))
				{
					if (tmp == val)
						break;
					vallen--;
					tmp--;
				}
			}

			write_proper(ptr, sec, seclen, key, keylen, val, vallen);
		}
	}
}

int format_proper_doc_to_ini(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr slk, elk;
	int total;
	const tchar_t *sec, *ent, *val;
	int sec_len, ent_len, val_len;

	total = 0;

	sec = NULL;
	slk = get_next_section(ptr, LINK_FIRST);
	while (slk)
	{
		if (total + 1 > max)
			return total;

		if (buf)
		{
			buf[total] = _T('[');
		}
		total += 1;

		//[sec]\r\n
		sec = get_dom_node_attr_ptr(slk, ATTR_NAME, -1);
		sec_len = xslen(sec);

		if (total + sec_len > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, sec, sec_len);
		}
		total += sec_len;

		if (total + 3 > max)
			return total;

		if (buf)
		{
			xsncpy(buf + total, _T("]\r\n"), 3);
		}
		total += 3;

		ent = val = NULL;
		elk = get_next_entity(slk, LINK_FIRST);
		while (elk)
		{
			//ent=val\r\n
			ent = get_entity_name_ptr(elk);
			ent_len = xslen(ent);

			if (total + ent_len > max)
				return total;

			if (buf)
			{
				xsncpy(buf + total, ent, ent_len);
			}
			total += ent_len;

			if (total + 1 > max)
				return total;

			if (buf)
			{
				buf[total] = _T('=');
			}
			total += 1;

			val = get_entity_value_ptr(elk);
			val_len = xslen(val);

			if (total + val_len > max)
				return total;

			if (buf)
			{
				xsncpy(buf + total, val, val_len);
			}
			total += val_len;

			if (total + 2 > max)
				return total;

			if (buf)
			{
				xsncpy(buf + total, _T("\r\n"), 2);
			}
			total += 2;

			elk = get_next_entity(slk, elk);
		}

		slk = get_next_section(ptr, slk);
	}

	return total;
}

bool_t load_proper_doc_from_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	dword_t size;
	int len;
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	int encode, skip;

	xhand_t xf = NULL;
	byte_t* buf = NULL;
	tchar_t* str = NULL;

	TRY_CATCH;

	if (!xfile_info(psd, fname, NULL, fsize, NULL, NULL))
	{
		raise_user_error(NULL, NULL);
	}

	if (is_huge_size(fsize))
	{
		raise_user_error(_T("0"), _T("not support large file"));
	}

	xf = xfile_open(psd, fname, FILE_OPEN_READ);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	size = xstol(fsize);
	buf = (byte_t*)xmem_alloc(size + 1);

	if (!xfile_read(xf, buf, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	encode = parse_utfbom(buf, size);
	skip = skip_utfbom(buf);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		len = utf8_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf8_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		len = utf16lit_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf16lit_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		len = utf16big_to_ucs(buf + skip, size - skip, NULL, MAX_LONG);
#else
		len = utf16big_to_mbs(buf + skip, size - skip, NULL, MAX_LONG);
#endif
		break;
	default:
#ifdef _UNICODE
		len = mbs_to_ucs((schar_t*)(buf + skip), size - skip, NULL, MAX_LONG);
#else
		len = mbs_to_mbs((schar_t*)(buf + skip), size - skip, NULL, MAX_LONG);
#endif
		break;
	}

	str = xsalloc(len + 1);

	switch (encode)
	{
	case _UTF8:
#ifdef _UNICODE
		len = utf8_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf8_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	case _UTF16_LIT:
#ifdef _UNICODE
		len = utf16lit_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf16lit_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	case _UTF16_BIG:
#ifdef _UNICODE
		len = utf16big_to_ucs(buf + skip, size - skip, str, len);
#else
		len = utf16big_to_mbs(buf + skip, size - skip, str, len);
#endif
		break;
	default:
#ifdef _UNICODE
		len = mbs_to_ucs((schar_t*)(buf + skip), size - skip, str, len);
#else
		len = mbs_to_mbs((schar_t*)(buf + skip), size - skip, str, len);
#endif
		break;
	}

	xmem_free(buf);
	buf = NULL;

	parse_proper_doc_from_ini(ptr, str, len);

	xsfree(str); 
	str = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (buf)
		xmem_free(buf);

	if (str)
		xsfree(str);

#if defined(_DEBUG) || defined(DEBUG)
	XDL_TRACE_LAST;
#endif

	return 0;
}

bool_t save_proper_doc_to_ini_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	int len;
	dword_t size;
	int skip;

	xhand_t xf = NULL;
	byte_t* buf = NULL;
	tchar_t* str = NULL;

	TRY_CATCH;

	xf = xfile_open(psd, fname, FILE_OPEN_CREATE);
	if (!xf)
	{
		raise_user_error(NULL, NULL);
	}

	len = format_proper_doc_to_ini(ptr, NULL, MAX_LONG);

	str = xsalloc(len + 1);
	format_proper_doc_to_ini(ptr, str, len);

	skip = format_utfbom(DEF_MBS, NULL);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, NULL, MAX_LONG);
#else
	size = mbs_to_mbs(str, len, NULL, MAX_LONG);
#endif

	buf = (byte_t*)xmem_alloc(skip + size);

	format_utfbom(_UTF8, buf);

#ifdef _UNICODE
	size = ucs_to_mbs(str, len, (schar_t*)(buf + skip), size);
#else
	size = mbs_to_mbs(str, len, (schar_t*)(buf + skip), size);
#endif

	xsfree(str);
	str = NULL;

	size += skip;

	if (!xfile_write(xf, buf, size))
	{
		raise_user_error(NULL, NULL);
	}

	xfile_close(xf);
	xf = NULL;

	xmem_free(buf);
	buf = NULL;

	END_CATCH;

	return 1;
ONERROR:

	if (xf)
		xfile_close(xf);

	if (buf)
		xmem_free(buf);

	if (str)
		xsfree(str);

	return 0;
}

bool_t load_memo_doc_from_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;

	len = load_text_from_file(psd, fname, NULL, MAX_LONG);
	if (len == C_ERR)
		return 0;
	
	buf = xsalloc(len + 1);

	load_text_from_file(psd, fname, buf, len);

	rt = parse_memo_doc(ptr, buf, len);

	xsfree(buf);

	return rt;
}

bool_t save_memo_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;

	len = format_memo_doc(ptr, NULL, MAX_LONG);

	buf = xsalloc(len + 1);

	format_memo_doc(ptr, buf, len);

	rt = save_text_to_file(psd, fname, buf, len);

	xsfree(buf);

	return rt;
}

bool_t load_tag_doc_from_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;

	len = load_text_from_file(psd, fname, NULL, MAX_LONG);
	if (len == C_ERR)
		return 0;

	buf = xsalloc(len + 1);

	load_text_from_file(psd, fname, buf, len);

	rt = parse_tag_doc(ptr, buf, len);

	xsfree(buf);

	return rt;
}

bool_t save_tag_doc_to_file(link_t_ptr ptr, const secu_desc_t* psd, const tchar_t* fname)
{
	tchar_t* buf;
	int len;
	bool_t rt;

	len = format_tag_doc(ptr, NULL, MAX_LONG);

	buf = xsalloc(len + 1);

	format_tag_doc(ptr, buf, len);

	rt = save_text_to_file(psd, fname, buf, len);

	xsfree(buf);

	return rt;
}

#endif //XDL_SUPPORT_DOC
