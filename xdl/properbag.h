/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc property bag document

	@module	properbag.h | interface file

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

#ifndef _PROPERBAG_H
#define _PROPERBAG_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#ifdef	__cplusplus
extern "C" {
#endif

XDL_API int write_style_attr(const tchar_t* org_style, int slen, const tchar_t* sz_key, int klen, const tchar_t* sz_val, int vlen, tchar_t* new_style, int max);

XDL_API int read_style_attr(const tchar_t* style, int len, const tchar_t* sz_key, int klen, tchar_t* buf, int max);

XDL_API void properbag_parse_stylesheet(link_t_ptr ptr,const tchar_t* str);

XDL_API int properbag_format_stylesheet(link_t_ptr ptr,tchar_t* buf,int len);

XDL_API void properbag_write_images_attributes(link_t_ptr ptr, link_t_ptr imagelist);

XDL_API void properbag_read_images_attributes(link_t_ptr ptr, link_t_ptr imagelist);

XDL_API void properbag_write_images_item_attributes(link_t_ptr ptr, link_t_ptr imageitem);

XDL_API void properbag_read_images_item_attributes(link_t_ptr ptr, link_t_ptr imageitem);

XDL_API void properbag_write_form_attributes(link_t_ptr ptr, link_t_ptr form);

XDL_API void properbag_read_form_attributes(link_t_ptr ptr, link_t_ptr form);

XDL_API void properbag_write_field_attributes(link_t_ptr ptr, link_t_ptr flk);

XDL_API void properbag_read_field_attributes(link_t_ptr ptr, link_t_ptr flk);

XDL_API void properbag_write_grid_attributes(link_t_ptr ptr, link_t_ptr grid);

XDL_API void properbag_read_grid_attributes(link_t_ptr ptr, link_t_ptr grid);

XDL_API void properbag_write_col_attributes(link_t_ptr ptr, link_t_ptr clk);

XDL_API void properbag_read_col_attributes(link_t_ptr ptr, link_t_ptr clk);

XDL_API void properbag_write_graph_attributes(link_t_ptr ptr, link_t_ptr grid);

XDL_API void properbag_read_graph_attributes(link_t_ptr ptr, link_t_ptr grid);

XDL_API void properbag_write_gax_attributes(link_t_ptr ptr, link_t_ptr glk);

XDL_API void properbag_read_gax_attributes(link_t_ptr ptr, link_t_ptr glk);

XDL_API void properbag_write_yax_attributes(link_t_ptr ptr, link_t_ptr ylk);

XDL_API void properbag_read_yax_attributes(link_t_ptr ptr, link_t_ptr ylk);

XDL_API void properbag_write_topog_attributes(link_t_ptr ptr, link_t_ptr topog, int row, int col);

XDL_API void properbag_read_topog_attributes(link_t_ptr ptr, link_t_ptr topog);

XDL_API void properbag_write_topog_spot_attributes(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void properbag_read_topog_spot_attributes(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void properbag_write_rich_attributes(link_t_ptr ptr, link_t_ptr rich);

XDL_API void properbag_read_rich_attributes(link_t_ptr ptr, link_t_ptr rich);

XDL_API void properbag_write_rich_anch_attributes(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void properbag_read_rich_anch_attributes(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void properbag_write_dialog_attributes(link_t_ptr ptr, link_t_ptr dialog);

XDL_API void properbag_read_dialog_attributes(link_t_ptr ptr, link_t_ptr dialog);

XDL_API void properbag_write_dialog_item_attributes(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void properbag_read_dialog_item_attributes(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void properbag_write_chart_attributes(link_t_ptr ptr, link_t_ptr chart);

XDL_API void properbag_read_chart_attributes(link_t_ptr ptr, link_t_ptr chart);

XDL_API void properbag_write_chart_table_attributes(link_t_ptr ptr, link_t_ptr ilk);

XDL_API void properbag_read_chart_table_attributes(link_t_ptr ptr, link_t_ptr ilk);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //PROPERBAG_H