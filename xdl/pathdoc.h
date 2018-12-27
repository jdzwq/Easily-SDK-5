/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc GDI path document

	@module	pathdoc.h | GDI path document interface file

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

#ifndef _PATHDOC_H
#define _PATHDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/**************************************serial enable attributes**********************************************/
#define set_path_node_name(nlk,val)							set_dom_node_name(nlk,val,-1)

#define get_path_node_name_ptr(nlk)							get_dom_node_name_ptr(nlk)
/******************************************runtime pathtion*************************************************/
#ifdef	__cplusplus
extern "C" {
#endif

	XDL_API link_t_ptr create_path_doc(void);

	XDL_API void destroy_path_doc(link_t_ptr ptr);

	XDL_API link_t_ptr get_path_nodeset(link_t_ptr ptr);

	XDL_API bool_t is_path_doc(link_t_ptr ptr);

	XDL_API void clear_path_doc(link_t_ptr ptr);

	XDL_API link_t_ptr insert_path_node(link_t_ptr ptr, link_t_ptr pos);

	XDL_API int get_path_node_count(link_t_ptr ptr);

	XDL_API void delete_path_node(link_t_ptr nlk);

	XDL_API link_t_ptr get_path_next_node(link_t_ptr ptr, link_t_ptr pos);

	XDL_API link_t_ptr get_path_prev_node(link_t_ptr ptr, link_t_ptr pos);

	XDL_API void set_path_move_node(link_t_ptr nlk, const xpoint_t* ppt);

	XDL_API void get_path_move_node(link_t_ptr nlk, xpoint_t* ppt);

	XDL_API void set_path_line_node(link_t_ptr nlk, const xpoint_t* ppt);

	XDL_API void get_path_line_node(link_t_ptr nlk, xpoint_t* ppt);

	XDL_API void set_path_arc_node(link_t_ptr nlk, const xsize_t* pps, bool_t ccw, const xpoint_t* ppt);

	XDL_API void get_path_arc_node(link_t_ptr nlk, xsize_t* pps, bool_t* pccw, xpoint_t* ppt);

	XDL_API void set_path_bezier_node(link_t_ptr nlk, const xpoint_t* ppt, int n);

	XDL_API int get_path_bezier_node(link_t_ptr nlk, xpoint_t* ppt);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*PATHDOC_H*/
