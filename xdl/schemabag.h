/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc schema bag document

	@module	schemabag.h | schema bag document interface file

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

#ifndef _SCHEMABAG_H
#define _SCHEMABAG_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION export_grid_schema: export grid col set defination to a schema document.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr sch: the schema link component.
@RETURN void: none.
*/
XDL_API void export_grid_schema(link_t_ptr ptr, link_t_ptr sch);

/*
@FUNCTION import_grid_schema: import grid col set defination from a schema document.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr sch: the schema link component.
@RETURN void: none.
*/
XDL_API void import_grid_schema(link_t_ptr ptr, link_t_ptr sch);

/*
@FUNCTION export_grid_data: export grid row set to dom document using schema defination.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr sch: the schema link component.
@OUTPUT link_t_ptr dom: the dom link component.
@RETURN void: none.
*/
XDL_API void export_grid_data(link_t_ptr ptr, link_t_ptr sch, link_t_ptr dom);

/*
@FUNCTION import_grid_data: import grid row set from dom document using schema defination.
@INPUT link_t_ptr ptr: the grid link component.
@INPUT link_t_ptr sch: the schema link component.
@OUTPUT link_t_ptr dom: the dom link component.
@RETURN void: none.
*/
XDL_API void import_grid_data(link_t_ptr ptr, link_t_ptr sch, link_t_ptr dom);

/*
@FUNCTION export_graph_schema: export graph yax set defination to a schema document.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr sch: the schema link component.
@RETURN void: none.
*/
XDL_API void export_graph_schema(link_t_ptr ptr, link_t_ptr sch);

/*
@FUNCTION export_graph_data: export graph xax set to dom document using schema defination.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr sch: the schema link component.
@OUTPUT link_t_ptr dom: the dom link component.
@RETURN void: none.
*/
XDL_API void export_graph_data(link_t_ptr ptr, link_t_ptr sch, link_t_ptr dom);

/*
@FUNCTION import_graph_data: import graph xax set from dom document using schema defination.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr sch: the schema link component.
@OUTPUT link_t_ptr dom: the dom link component.
@RETURN void: none.
*/
XDL_API void import_graph_data(link_t_ptr ptr, link_t_ptr sch, link_t_ptr dom);

/*
@FUNCTION export_form_schema: export form field set defination to a schema document.
@INPUT link_t_ptr ptr: the graph link component.
@INPUT link_t_ptr sch: the schema link component.
@RETURN void: none.
*/
XDL_API void export_form_schema(link_t_ptr ptr, link_t_ptr sch);

/*
@FUNCTION export_form_data: export form field data to dom document using schema defination.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr sch: the schema link component.
@OUTPUT link_t_ptr dom: the dom link component.
@RETURN void: none.
*/
XDL_API void export_form_data(link_t_ptr ptr, link_t_ptr sch, link_t_ptr dom);

/*
@FUNCTION import_form_data: import form field data from dom document using schema defination.
@INPUT link_t_ptr ptr: the form link component.
@INPUT link_t_ptr sch: the schema link component.
@OUTPUT link_t_ptr dom: the dom link component.
@RETURN void: none.
*/
XDL_API void import_form_data(link_t_ptr ptr, link_t_ptr sch, link_t_ptr dom);

/*
@FUNCTION export_rich_schema: export rich anchor set defination to a schema document.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT link_t_ptr sch: the schema link component.
@RETURN void: none.
*/
XDL_API void export_rich_schema(link_t_ptr ptr, link_t_ptr sch);

/*
@FUNCTION export_rich_data: export rich anchor set to dom document using schema defination.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT link_t_ptr sch: the schema link component.
@OUTPUT link_t_ptr dom: the dom link component.
@RETURN void: none.
*/
XDL_API void export_rich_data(link_t_ptr ptr, link_t_ptr sch, link_t_ptr dom);

/*
@FUNCTION import_rich_data: import rich anchor set from dom document using schema defination.
@INPUT link_t_ptr ptr: the rich link component.
@INPUT link_t_ptr sch: the schema link component.
@OUTPUT link_t_ptr dom: the dom link component.
@RETURN void: none.
*/
XDL_API void import_rich_data(link_t_ptr ptr, link_t_ptr sch, link_t_ptr dom);

//XDL_API void export_dom_node(link_t_ptr node, link_t_ptr sch, link_t_ptr dom);

//XDL_API void import_dom_node(link_t_ptr node, link_t_ptr sch, link_t_ptr dom);

#ifdef	__cplusplus
}
#endif

#endif //XDLDOC

#endif //SCHEMABAG_H