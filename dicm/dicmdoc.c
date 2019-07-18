/***********************************************************************
	Easily DICOM 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc dicom document

	@module	dicmdoc.c | dicom document implement file

	@devnote 张文权 2018.01 - 2018.12	v1.0
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

#include "dicmdoc.h"
#include "dicmctx.h"
#include "dicmtag.h"
#include "dicmpdg.h"

link_t_ptr create_dicm_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_DICM, -1);

	return ptr;
}

void destroy_dicm_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

void clear_dicm_doc(link_t_ptr ptr)
{
	delete_dom_child_nodes(ptr);
}

bool_t is_dicm_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_DICM, -1, 0) == 0) ? 1 : 0;
}

bool_t is_dicm_item(link_t_ptr ptr, link_t_ptr ilk)
{
	if (!ilk)
		return 0;

	return is_dom_child_node(ptr, ilk);
}

link_t_ptr get_dicm_item(link_t_ptr ilk, const tchar_t* iname)
{
	return find_dom_node_by_name(ilk, 0, iname, -1);
}

link_t_ptr	get_dicm_item_by_tag(link_t_ptr ilk, const tchar_t* tag)
{
	return find_dom_node_by_attr(ilk, 0, _T("VX"), -1, tag, -1);
}

void delete_dicm_item(link_t_ptr ilk)
{
	delete_dom_node(ilk);
}

link_t_ptr insert_dicm_item(link_t_ptr ilk, link_t_ptr pos)
{
	link_t_ptr nlk;

	nlk = insert_dom_node(ilk, pos);

	return nlk;
}

link_t_ptr insert_dicm_item_by_tag(link_t_ptr ilk, const tchar_t* tag)
{
	link_t_ptr nlk;

	nlk = get_dicm_first_child_item(ilk);
	while (nlk)
	{
		if (compare_text(get_dicm_item_tag_ptr(nlk), -1, tag, -1, 0) == 0)
			return nlk;
		else if (compare_text(get_dicm_item_tag_ptr(nlk), -1, tag, -1, 0) > 0)
			break;

		nlk = get_dicm_next_sibling_item(nlk);
	}

	if (!nlk)
	{
		nlk = insert_dom_node(ilk, LINK_LAST);
	}
	else
	{
		nlk = insert_dom_node_before(ilk, nlk);
	}

	set_dom_node_attr(nlk, _T("VX"), -1, tag, -1);

	return nlk;
}

link_t_ptr get_dicm_first_child_item(link_t_ptr ilk)
{
	return get_dom_first_child_node(ilk);
}

link_t_ptr get_dicm_last_child_item(link_t_ptr ilk)
{
	return get_dom_last_child_node(ilk);
}

link_t_ptr get_dicm_parent_item(link_t_ptr ilk)
{
	return get_dom_parent_node(ilk);
}

link_t_ptr get_dicm_next_sibling_item(link_t_ptr ilk)
{
	return get_dom_next_sibling_node(ilk);
}

link_t_ptr get_dicm_prev_sibling_item(link_t_ptr ilk)
{
	return get_dom_prev_sibling_node(ilk);
}

int get_dicm_charset(link_t_ptr dom)
{
	link_t_ptr plk;
	int chs = DICM_CHARSET_ENG;

	plk = find_dom_node_by_name(dom, 0, _T("SpecificCharacterSet"), -1);
	if (plk)
	{
		if (compare_text(get_dom_node_text_ptr(plk), -1, _T("ISO_IR 192"), -1, 1) == 0)
			chs = DICM_CHARSET_UTF;
		else if (compare_text(get_dom_node_text_ptr(plk), -1, _T("GB18030"), -1, 1) == 0)
			chs = DICM_CHARSET_GBK;
		else
			chs = DICM_CHARSET_ENG;
	}

	return chs;
}

void clear_dicm_annotation(link_t_ptr dom)
{
	link_t_ptr plk;

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (plk)
	{
		delete_dom_node(plk);
	}
}

int load_dicm_text_annotation(link_t_ptr dom, dicm_text_anno_t* pta, int max)
{
	link_t_ptr plk, nlk, glk;
	int i = 0;

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (!plk)
		return 0;

	nlk = find_dom_node_by_name(plk, 1, _T("TextObjectSequence"), -1);
	if (!nlk)
		return 0;

	glk = get_dom_first_child_node(nlk);
	while (glk && i < max)
	{
		nlk = get_dom_first_child_node(glk);
		while (nlk)
		{
			if (pta)
			{
				if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("UnformattedTextValue"), -1, 1) == 0)
					xscpy(pta[i].text, get_dom_node_text_ptr(nlk));
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BoundingBoxTopLeftHandCorner"), -1, 1) == 0)
					parse_dicm_point(get_dom_node_text_ptr(nlk), -1, pta[i].points + 1, 1);
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BoundingBoxBottomRightHandCorner"), -1, 1) == 0)
					parse_dicm_point(get_dom_node_text_ptr(nlk), -1, pta[i].points + 2, 1);
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("AnchorPoint"), -1, 1) == 0)
					parse_dicm_point(get_dom_node_text_ptr(nlk), -1, pta[i].points, 1);
			}

			nlk = get_dom_next_sibling_node(nlk);
		}

		if (get_dom_child_node_count(glk))
			i++;

		glk = get_dom_next_sibling_node(glk);
	}

	return i;
}

void save_dicm_text_annotation(link_t_ptr dom, const dicm_text_anno_t* pta, int count)
{
	link_t_ptr plk, nlk, glk, ilk;
	int bytes;
	int i, chs;
	tchar_t token[1024];

	chs = get_dicm_charset(dom);

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (!plk)
	{
		plk = insert_dicm_item_by_tag(dom, _T("0x00700001"));
		set_dom_node_name(plk, _T("GraphicAnnotationSequence"), -1);
		set_dom_node_attr(plk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(plk, _T("VX"), -1, _T("0x00700001"), -1);
		set_dom_node_attr(plk, _T("VT"), -1, _T("(0070,0001)"), -1);
		set_dom_node_attr(plk, _T("VR"), -1, _T("SQ"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(plk, _T("VL"), -1, token, -1);
	}
	else
	{
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(plk, _T("VL"), -1, token, -1);
	}

	glk = find_dom_node_by_name(plk, 1, _T("TextObjectSequence"), -1);
	if (!glk)
	{
		nlk = find_dom_node_by_name(plk, 0, _T("StartOfItem"), -1);
		if (!nlk)
		{
			nlk = insert_dom_node(plk, LINK_FIRST);
			set_dom_node_name(nlk, _T("StartOfItem"), -1);
			set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE000"), -1);
			set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E000)"), -1);
			xsprintf(token, _T("%d"), 0xFFFFFFFF);
			set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
		}
		else
		{
			xsprintf(token, _T("%d"), 0xFFFFFFFF);
			set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
		}

		glk = insert_dicm_item_by_tag(nlk, _T("0x00700008"));
		set_dom_node_name(glk, _T("TextObjectSequence"), -1);
		set_dom_node_attr(glk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(glk, _T("VX"), -1, _T("0x00700008"), -1);
		set_dom_node_attr(glk, _T("VT"), -1, _T("(0070,0008)"), -1);
		set_dom_node_attr(glk, _T("VR"), -1, _T("SQ"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(glk, _T("VL"), -1, token, -1);
	}
	else
	{
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(glk, _T("VL"), -1, token, -1);

		//StartOfItem
		nlk = get_dom_parent_node(glk);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
	}

	ilk = find_dom_node_by_name(nlk, 0, _T("EndOfItems"), -1);
	if (!ilk)
	{
		ilk = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(ilk, _T("EndOfItems"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE00D"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E00D)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);
	}

	delete_dom_child_nodes(glk);

	for (i = 0; i < count; i++)
	{
		ilk = insert_dom_node(glk, LINK_LAST);
		set_dom_node_name(ilk, _T("StartOfItem"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE000"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E000)"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("BoundingBoxAnnotationUnits"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700003"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0003)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("PIXEL"), -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(_T("PIXEL"), -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(_T("PIXEL"), -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("AnchorPointAnnotationUnits"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700004"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0004)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("PIXEL"), -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(_T("PIXEL"), -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(_T("PIXEL"), -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("UnformattedTextValue"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700006"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0006)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("LO"), -1);
		set_dom_node_text(nlk, pta[i].text, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(pta[i].text, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(pta[i].text, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("BoundingBoxTopLeftHandCorner"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700010"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0010)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("DS"), -1);
		format_dicm_point(pta[i].points + 1, 1, token, 1024);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("BoundingBoxBottomRightHandCorner"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700011"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0011)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("DS"), -1);
		format_dicm_point(pta[i].points + 2, 1, token, 1024);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("BoundingBoxTextHorizJustification"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700012"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0012)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, pta[i].align, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(pta[i].align, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(pta[i].align, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("AnchorPoint"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700014"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0014)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("DS"), -1);
		format_dicm_point(pta[i].points, 1, token, 1024);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("AnchorPointVisibility"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700015"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0015)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("N"), -1);
		set_dom_node_attr(nlk, _T("VL"), -1, _T("1"), -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("EndOfItems"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE00D"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E00D)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
	}

	nlk = insert_dom_node(glk, LINK_LAST);
	set_dom_node_name(nlk, _T("EndOfSequence"), -1);
	set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE0DD"), -1);
	set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E0DD)"), -1);
	xsprintf(token, _T("%d"), 0);
	set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

	ilk = find_dom_node_by_name(plk, 0, _T("EndOfSequence"), -1);
	if (!ilk)
	{
		ilk = insert_dom_node(plk, LINK_LAST);
		set_dom_node_name(ilk, _T("EndOfSequence"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE0DD"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E0DD)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);
	}
}

int load_dicm_graphic_annotation(link_t_ptr dom, dicm_graphic_anno_t* pga, int max)
{
	link_t_ptr plk, nlk, glk;
	int i = 0;

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (!plk)
		return 0;

	nlk = find_dom_node_by_name(plk, 1, _T("GraphicObjectSequence"), -1);
	if (!nlk)
		return 0;

	glk = get_dom_first_child_node(nlk);
	while (glk && i < max)
	{
		nlk = get_dom_first_child_node(glk);
		while (nlk)
		{
			if (pga)
			{
				if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("NumberOfGraphicPoints"), -1, 1) == 0)
					pga[i].count = xstol(get_dom_node_text_ptr(nlk));
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("GraphicData"), -1, 1) == 0)
					pga[i].count = parse_dicm_point(get_dom_node_text_ptr(nlk), -1, pga[i].points, DICM_MAX_POINTS);
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("GraphicType"), -1, 1) == 0)
					xscpy(pga[i].type, get_dom_node_text_ptr(nlk));
				else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("GraphicFilled"), -1, 1) == 0)
					pga[i].fill = (compare_text(get_dom_node_text_ptr(nlk), -1, _T("Y"), 1, 1) == 0) ? 1 : 0;
			}

			nlk = get_dom_next_sibling_node(nlk);
		}

		if (get_dom_child_node_count(glk))
			i++;

		glk = get_dom_next_sibling_node(glk);
	}

	return i;
}

void save_dicm_graphic_annotation(link_t_ptr dom, const dicm_graphic_anno_t* pga, int count)
{
	link_t_ptr plk, nlk, glk, ilk;
	int bytes;
	int i, chs;
	tchar_t token[1024];

	chs = get_dicm_charset(dom);

	plk = find_dom_node_by_name(dom, 0, _T("GraphicAnnotationSequence"), -1);
	if (!plk)
	{
		plk = insert_dicm_item_by_tag(dom, _T("0x00700001"));
		set_dom_node_name(plk, _T("GraphicAnnotationSequence"), -1);
		set_dom_node_attr(plk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(plk, _T("VX"), -1, _T("0x00700001"), -1);
		set_dom_node_attr(plk, _T("VT"), -1, _T("(0070,0001)"), -1);
		set_dom_node_attr(plk, _T("VR"), -1, _T("SQ"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(plk, _T("VL"), -1, token, -1);
	}
	else
	{
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(plk, _T("VL"), -1, token, -1);
	}

	glk = find_dom_node_by_name(plk, 1, _T("GraphicObjectSequence"), -1);
	if (!glk)
	{
		nlk = find_dom_node_by_name(plk, 0, _T("StartOfItem"), -1);
		if (!nlk)
		{
			nlk = insert_dom_node(plk, LINK_FIRST);
			set_dom_node_name(nlk, _T("StartOfItem"), -1);
			set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE000"), -1);
			set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E000)"), -1);
			xsprintf(token, _T("%d"), 0xFFFFFFFF);
			set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
		}
		else
		{
			xsprintf(token, _T("%d"), 0xFFFFFFFF);
			set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
		}

		glk = insert_dicm_item_by_tag(nlk, _T("0x00700009"));
		set_dom_node_name(glk, _T("GraphicObjectSequence"), -1);
		set_dom_node_attr(glk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(glk, _T("VX"), -1, _T("0x00700009"), -1);
		set_dom_node_attr(glk, _T("VT"), -1, _T("(0070,0009)"), -1);
		set_dom_node_attr(glk, _T("VR"), -1, _T("SQ"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(glk, _T("VL"), -1, token, -1);
	}
	else
	{
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(glk, _T("VL"), -1, token, -1);

		//StartOfItem
		nlk = get_dom_parent_node(glk);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
	}

	ilk = find_dom_node_by_name(nlk, 0, _T("EndOfItems"), -1);
	if (!ilk)
	{
		ilk = insert_dom_node(nlk, LINK_LAST);
		set_dom_node_name(ilk, _T("EndOfItems"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE00D"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E00D)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);
	}

	delete_dom_child_nodes(glk);

	for (i = 0; i < count; i++)
	{
		ilk = insert_dom_node(glk, LINK_LAST);
		set_dom_node_name(ilk, _T("StartOfItem"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE000"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E000)"), -1);
		xsprintf(token, _T("%d"), 0xFFFFFFFF);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("GraphicAnnotationUnits"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700005"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0005)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("PIXEL"), -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(_T("PIXEL"), -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(_T("PIXEL"), -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("NumberOfGraphicPoints"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700021"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0021)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("IS"), -1);
		xsprintf(token, _T("%d"), pga[i].count);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("GraphicData"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700022"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0022)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("DS"), -1);
		format_dicm_point(pga[i].points, pga[i].count, token, 1024);
		set_dom_node_text(nlk, token, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(token, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(token, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("GraphicType"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700023"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0023)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("LO"), -1);
		set_dom_node_text(nlk, pga[i].type, -1);
		if (chs == DICM_CHARSET_UTF)
			bytes = ucs_to_utf8(pga[i].type, -1, NULL, MAX_LONG);
		else
			bytes = ucs_to_mbs(pga[i].type, -1, NULL, MAX_LONG);
		xsprintf(token, _T("%d"), bytes);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("GraphicFilled"), -1);
		set_dom_node_attr(nlk, _T("VG"), -1, _T("Annotation"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0x00700024"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(0070,0024)"), -1);
		set_dom_node_attr(nlk, _T("VR"), -1, _T("SH"), -1);
		set_dom_node_text(nlk, _T("N"), -1);
		set_dom_node_attr(nlk, _T("VL"), -1, _T("1"), -1);

		nlk = insert_dom_node(ilk, LINK_LAST);
		set_dom_node_name(nlk, _T("EndOfItems"), -1);
		set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE00D"), -1);
		set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E00D)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(nlk, _T("VL"), -1, token, -1);
	}

	nlk = insert_dom_node(glk, LINK_LAST);
	set_dom_node_name(nlk, _T("EndOfSequence"), -1);
	set_dom_node_attr(nlk, _T("VX"), -1, _T("0xFFFEE0DD"), -1);
	set_dom_node_attr(nlk, _T("VT"), -1, _T("(FFFE,E0DD)"), -1);
	xsprintf(token, _T("%d"), 0);
	set_dom_node_attr(nlk, _T("VL"), -1, token, -1);

	ilk = find_dom_node_by_name(plk, 0, _T("EndOfSequence"), -1);
	if (!ilk)
	{
		ilk = insert_dom_node(plk, LINK_LAST);
		set_dom_node_name(ilk, _T("EndOfSequence"), -1);
		set_dom_node_attr(ilk, _T("VX"), -1, _T("0xFFFEE0DD"), -1);
		set_dom_node_attr(ilk, _T("VT"), -1, _T("(FFFE,E0DD)"), -1);
		xsprintf(token, _T("%d"), 0);
		set_dom_node_attr(ilk, _T("VL"), -1, token, -1);
	}
}

dword_t get_dicm_pdg(link_t_ptr ptr, byte_t* buf, dword_t max)
{
	link_t_ptr nlk;
	const tchar_t* tmp;
	int len;
	byte_t* src = NULL;
	dword_t size = 0;

	pdg_file_t df = { 0 };
	pdg_info_t di = { 0 };
	pdg_bits_t bo = { 0 };

	nlk = get_dom_first_child_node(ptr);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Modality"), -1, 1) == 0) 
		{
			tmp = get_dom_node_text_ptr(nlk);
			ucs_to_mbs(tmp, -1, di.moda, 16);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("SamplesPerPixel"), -1, 1) == 0) //image colors
		{
			di.psize = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Rows"), -1, 1) == 0) //image rows
		{
			di.height = xstol(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Columns"), -1, 1) == 0) //image cols
		{
			di.width = xstol(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelSpacing"), -1, 1) == 0) //pixel space
		{
			tmp = get_dom_node_text_ptr(nlk);
			if (!is_null(tmp))
			{
				di.xmm_pixel = (long)(xstof(tmp) * 10000);
				while (*tmp != _T('/') && *tmp != _T('\\') && *tmp != _T('\0'))
					tmp++;
				if (*tmp != _T('\0'))
					tmp++;
				di.ymm_pixel = (long)(xstof(tmp) * 10000);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BitsAllocated"), -1, 1) == 0) //bit size
		{
			bo.bit_all = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BitsStored"), -1, 1) == 0) //bit valid
		{
			bo.bit_len = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("HighBit"), -1, 1) == 0) //bit high
		{
			bo.bit_pos = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelRepresentation"), -1, 1) == 0) //is sign
		{
			di.psign = xstos(get_dom_node_text_ptr(nlk));
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("WindowCenter"), -1, 1) == 0) //windows center
		{
			di.win_center = (long)(xstof(get_dom_node_text_ptr(nlk)) * 10000);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("WindowWidth"), -1, 1) == 0) //windows width
		{
			di.win_width = (long)(xstof(get_dom_node_text_ptr(nlk)) * 10000);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RescaleIntercept"), -1, 1) == 0) //rescale Intercept
		{
			di.win_inter = (long)(xstof(get_dom_node_text_ptr(nlk)) * 10000);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RescaleSlope"), -1, 1) == 0) //rescale Slope
		{
			di.win_slope = (long)(xstof(get_dom_node_text_ptr(nlk)) * 10000);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelData"), -1, 0) == 0) //image bytes
		{
			tmp = get_dom_node_text_ptr(nlk);
			len = xslen(tmp);
			size = xbas_decode(tmp, len, NULL, MAX_LONG);
			src = (byte_t*)xmem_alloc(size);
			xbas_decode(tmp, len, src, size);
		}

		nlk = get_dom_next_sibling_node(nlk);
	}

	di.psize *= (bo.bit_all / 8);
	df.flag = 0x4d434944;
	df.fsize = di.width * di.height * di.psize;
	df.offset = sizeof(pdg_file_t) + sizeof(pdg_info_t);

	if (buf)
		size = dicm_fill_pdg(buf, max, &df, &di, &bo, src, size);
	else
		size = df.fsize;

	xmem_free(src);

	return size;
}

void set_dicm_pdg(link_t_ptr ptr, const byte_t* buf, dword_t size)
{
	link_t_ptr nlk;
	tchar_t vf[NUM_LEN];
	tchar_t vl[INT_LEN];
	const tchar_t* vr;
	dword_t len;
	tchar_t* tmp;

	pdg_file_t df = { 0 };
	pdg_info_t di = { 0 };
	pdg_bits_t bo = { 0 };
	int samp;
	const byte_t* pbm = NULL;
	
	memcpy((void*)&df, (void*)buf, sizeof(pdg_file_t));
	memcpy((void*)&di, (void*)(buf + sizeof(pdg_file_t)), sizeof(pdg_info_t));
	pbm = buf + sizeof(pdg_file_t) + sizeof(pdg_info_t);
	size -= (sizeof(pdg_file_t) + sizeof(pdg_info_t));

	switch (di.psize)
	{
	case 1:
		samp = 1;
		bo.bit_all = 8;
		bo.bit_len = 8;
		bo.bit_pos = 7;
		break;
	case 2:
		samp = 1;
		bo.bit_all = 16;
		bo.bit_len = 16;
		bo.bit_pos = 15;
		break;
	case 3:
		samp = 3;
		bo.bit_all = 8;
		bo.bit_len = 8;
		bo.bit_pos = 7;
		break;
	}

	nlk = get_dom_first_child_node(ptr);
	while (nlk)
	{
		if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Modality"), -1, 0) == 0)
		{
			mbs_to_ucs(di.moda, 16, vf, 16);
			set_dom_node_text(nlk, vf, -1);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("SamplesPerPixel"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), samp);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Rows"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), di.height);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("Columns"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), di.width);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelSpacing"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%.4f/%.4f"), (double)di.xmm_pixel / 10000, (double)di.ymm_pixel / 10000);
			set_dom_node_text(nlk, vf, -1);

			xsprintf(vl, _T("%d"), xslen(vf));
			set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BitsAllocated"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), bo.bit_all);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("BitsStored"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%d"), bo.bit_len);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("HighBit"), -1, 0) == 0)
		{
			ustoxs(bo.bit_pos, vf, NUM_LEN);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelRepresentation"), -1, 0) == 0)
		{
			ustoxs(di.psign, vf, NUM_LEN);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("WindowCenter"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%f"), (double)di.win_center / 10000);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("WindowWidth"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%f"), (double)di.win_width / 10000);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RescaleIntercept"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%f"), (double)di.win_inter / 10000);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("RescaleSlope"), -1, 0) == 0)
		{
			xsprintf(vf, _T("%f"), (double)di.win_slope / 10000);
			set_dom_node_text(nlk, vf, -1);

			vr = get_dom_node_attr_ptr(nlk, _T("VR"), -1);
			if (!DICM_IS_NUM_VR(vr))
			{
				xsprintf(vl, _T("%d"), xslen(vf));
				set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
			}
		}
		else if (compare_text(get_dom_node_name_ptr(nlk), -1, _T("PixelData"), -1, 0) == 0)
		{
			len = xbas_encode(pbm, size, NULL, MAX_LONG);
			tmp = xsalloc(len + 1);
			xbas_encode(pbm, size, tmp, len);
			attach_dom_node_text(nlk, tmp);

			xsprintf(vl, _T("%d"), size);
			set_dom_node_attr(nlk, _T("VL"), -1, vl, -1);
		}

		nlk = get_dom_next_sibling_node(nlk);
	}

}

link_t_ptr create_dicm_sop(const tchar_t* sopuid)
{
	link_t_ptr dom, nlk;
	int i, n;
	sword_t l, h;
	const dicm_item_t* pitem;
	const dicm_group_t* pgroup;
	tchar_t tvg[KEY_LEN] = { 0 };
	tchar_t tvn[KEY_LEN] = { 0 };
	tchar_t tvt[NUM_LEN] = { 0 };
	tchar_t tvx[INT_LEN] = { 0 };
	tchar_t tvl[INT_LEN] = { 0 };
	int vl;

	dom = create_dom_doc();

	set_dom_node_name(dom, _T("DICM"), -1);

	n = sizeof(DICM_CAPTAGS) / sizeof(dword_t);
	for (i = 0; i < n; i++)
	{
		h = (sword_t)((DICM_CAPTAGS[i] & 0xFFFF0000) >> 16);
		l = (sword_t)(DICM_CAPTAGS[i] & 0x0000FFFF);

		pgroup = dicm_group_info(DICM_CAPTAGS[i]);
		if (pgroup)
			xscpy(tvg, pgroup->gn);
		else
			xsprintf(tvg, _T("Unknown%04X"), h);

		pitem = dicm_item_info(DICM_CAPTAGS[i]);
		if (pitem)
			xscpy(tvn, pitem->vn);
		else
			xsprintf(tvn, _T("Unknown%04X%04X"), h, l);

		xsprintf(tvx, _T("0x%04X%04X"), h, l);
		xsprintf(tvt, _T("(%04X,%04X)"), h, l);

		nlk = insert_dom_node(dom, LINK_LAST);
		set_dom_node_name(nlk, tvn, -1);
		set_dom_node_attr(nlk, _T("VR"), 2, pitem->vr, 2);
		set_dom_node_attr(nlk, _T("VX"), 2, tvx, -1);
		set_dom_node_attr(nlk, _T("VT"), 2, tvt, -1);
		set_dom_node_attr(nlk, _T("VG"), 2, tvg, -1);
		if (DICM_IS_NUM_VR(pitem->vr))
			vl = dicm_vr_size(pitem->vr);
		else
			vl = 0;

		if (compare_text(tvn, -1, _T("FileMetaInfoGroupLength"), -1, 0) == 0)
		{
			if (!vl)
			{
				vl = 3;
			}
			xsprintf(tvl, _T("%d"), vl);
			set_dom_node_attr(nlk, _T("VL"), 2, tvl, -1);
			set_dom_node_text(nlk, _T("196"), -1);
		}
		else if (compare_text(tvn, -1, _T("FileMetaInfoVersion"), -1, 0) == 0)
		{
			if (!vl)
			{
				vl = 2;
			}
			xsprintf(tvl, _T("%d"), vl);
			set_dom_node_attr(nlk, _T("VL"), 2, tvl, -1);
			set_dom_node_text(nlk, _T("AAE="), -1);	
			set_dom_node_cdata(nlk, 1);
		}
		else if (compare_text(tvn, -1, _T("MediaStorageSOPClassUID"), -1, 0) == 0)
		{
			if (!vl)
			{
				vl = xslen(_T("1.2.840.10008.5.1.4.1.1.7"));
			}
			xsprintf(tvl, _T("%d"), vl);
			set_dom_node_attr(nlk, _T("VL"), 2, tvl, -1);
			set_dom_node_text(nlk, _T("1.2.840.10008.5.1.4.1.1.7"), -1);
		}
		/*else if (compare_text(tvn, -1, _T("MediaStorageSOPInstanceUID"), -1, 0) == 0)
		{
		}*/
		else if (compare_text(tvn, -1, _T("TransferSyntaxUID"), -1, 0) == 0)
		{
			if (!vl)
			{
				vl = xslen(_T("1.2.840.10008.1.2.1"));
			}
			xsprintf(tvl, _T("%d"), vl);
			set_dom_node_attr(nlk, _T("VL"), 2, tvl, -1);
			set_dom_node_text(nlk, _T("1.2.840.10008.1.2.1"), -1);
		}
		else if (compare_text(tvn, -1, _T("ImplementationClassUID"), -1, 0) == 0)
		{
			if (!vl)
			{
				vl = xslen(_T("1.2.840.473.8013.2.3.1"));
			}
			xsprintf(tvl, _T("%d"), vl);
			set_dom_node_attr(nlk, _T("VL"), 2, tvl, -1);
			set_dom_node_text(nlk, _T("1.2.840.473.8013.2.3.1"), -1);
		}
		else if (compare_text(tvn, -1, _T("ImplementationVersionName"), -1, 0) == 0)
		{
			if (!vl)
			{
				vl = xslen(_T("XDLDICM50"));
			}
			xsprintf(tvl, _T("%d"), vl);
			set_dom_node_attr(nlk, _T("VL"), 2, tvl, -1);
			set_dom_node_text(nlk, _T("XDLDICM50"), -1);
		}
		/*else if (compare_text(tvn, -1, _T("SourceApplicationEntityTitle"), -1, 0) == 0)
		{
		}*/
		else if (compare_text(tvn, -1, _T("SOPClassUID"), -1, 0) == 0)
		{
			if (!vl)
			{
				vl = xslen(_T("1.2.840.10008.5.1.4.1.1.7"));
			}
			xsprintf(tvl, _T("%d"), vl);
			set_dom_node_attr(nlk, _T("VL"), 2, tvl, -1);
			set_dom_node_text(nlk, _T("1.2.840.10008.5.1.4.1.1.7"), -1);
		}
		/*
		else if (compare_text(tvn, -1, _T("SOPInstanceUID"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("StudyDate"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("SeriesDate"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("ContentDate"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("StudyTime"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("SeriesTime"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("ContentTime"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("AccessionNumber"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("Modality"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("PatientName"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("PatientID"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("PatientBirthDate"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("PatientSex"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("PatientAge"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("StudyInstanceUID"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("SeriesInstanceUID"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("SeriesNumber"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("InstanceNumber"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("SamplesPerPixel"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("PhotometricInterpretation"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("PlanarConfiguration"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("Rows"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("Columns"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("BitsAllocated"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("BitsStored"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("HighBit"), -1, 0) == 0)
		{
		}
		else if (compare_text(tvn, -1, _T("PixelRepresentation"), -1, 0) == 0)
		{
		}
		*/
	}

	return dom;
}

void get_dicm_summary(link_t_ptr ptr, dicm_summary_t* pds)
{
	link_t_ptr plk;

	plk = get_dom_first_child_node(ptr);
	while (plk)
	{
		if (compare_text(get_dom_node_name_ptr(plk), -1, _T("PatientID"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->PatientID, RES_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("PatientName"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->PatientName, RES_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("PatientSex"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->PatientSex, INT_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("PatientAge"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->PatientAge, INT_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("StudyInstanceUID"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->StudyInstanceUID, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("StudyID"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->StudyID, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("StudyDate"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->StudyDate, DATE_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("SeriesInstanceUID"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->SeriesInstanceUID, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("SeriesDate"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->SeriesDate, DATE_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("SeriesNumber"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->SeriesNumber, INT_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("Modality"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->Modality, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("Manufacturer"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->Manufacturer, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("InstitutionName"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->InstitutionName, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("StationName"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->StationName, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("ManufacturersModelName"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->ManufacturersModelName, KEY_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("SliceThickness"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->SliceThickness, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("KVP"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->KVP, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("GantryDetectorTilt"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->GantryDetectorTilt, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("XRayTubeCurrent"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->XRayTubeCurrent, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("ExposureTime"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->ExposureTime, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("WindowCenter"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->WindowCenter, NUM_LEN);
		}
		else if (compare_text(get_dom_node_name_ptr(plk), -1, _T("WindowWidth"), -1, 1) == 0)
		{
			get_dom_node_text(plk, pds->WindowWidth, NUM_LEN);
		}

		plk = get_dom_next_sibling_node(plk);
	}
}
