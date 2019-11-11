/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc form bio document

	@module	formbio.c | implement file

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

#include "formbio.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"
#include "xdlinit.h"

#if defined(XDL_SUPPORT_DOC) && defined(XDL_SUPPORT_BIO)

bool_t load_field_object_from_file(link_t_ptr flk, const secu_desc_t* psd, const tchar_t* fname)
{
	link_t_ptr doc;
	const tchar_t* fclass;

	int bsize;
	tchar_t* base_buf = NULL;

	fclass = get_field_class_ptr(flk);

	if (compare_text(fclass, -1, DOC_FORM_GRID, -1, 0) == 0)
	{
		doc = create_grid_from_meta_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_grid(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_GRAPH, -1, 0) == 0)
	{
		doc = create_graph_from_meta_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_graph(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_RICH, -1, 0) == 0)
	{
		doc = create_rich_from_meta_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_rich(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_IMAGES, -1, 0) == 0)
	{
		doc = create_images_from_meta_file(psd, fname);
		if (!doc)
			return 0;

		set_field_embed_images(flk, doc);

		return 1;
	}
	else if (compare_text(fclass, -1, DOC_FORM_FORM, -1, 0) == 0)
	{
		doc = create_form_from_meta_file(psd, fname);
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



#endif //XDLDOC