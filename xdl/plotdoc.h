/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc plot document

	@module	plotdoc.h | interface file

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

#ifndef _PLOTDOC_H
#define _PLOTDOC_H

#include "xdldef.h"
#include "variant.h"

#ifdef XDL_SUPPORT_DOC


#ifdef	__cplusplus
extern "C" {
#endif

/**********************************************Functions**********************************************/

/*
@PLOTTION create_plot_doc: create a plot document.
@RETURN link_t_ptr: return the plot document link component.
*/
EXP_API link_t_ptr create_plot_doc(void);

/*
@PLOTTION destroy_plot_doc: destroy a plot document.
@INPUT link_t_ptr ptr: the plot document link component.
@RETURN void: none.
*/
EXP_API void destroy_plot_doc(link_t_ptr ptr);

/*
@PLOTTION clear_plot_doc: clear plot document, the parameter set will be emptied.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN void: node.
*/
EXP_API void clear_plot_doc(link_t_ptr ptr);

/*
@PLOTTION is_plot_doc: test is plot document.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN bool_t: return nonzero if being a plot, otherwise return zero.
*/
EXP_API bool_t is_plot_doc(link_t_ptr ptr);

/*
@PLOTTION get_plot_width: get plot width.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN float: return plot width.
*/
EXP_API float	get_plot_width(link_t_ptr ptr);

/*
@PLOTTION set_plot_width: set plot width.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT float width: the plot width.
@RETURN void: none.
*/
EXP_API void set_plot_width(link_t_ptr ptr, float width);

/*
@PLOTTION get_plot_height: get plot height.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN float: return plot height.
*/
EXP_API float	get_plot_height(link_t_ptr ptr);

/*
@PLOTTION set_plot_height: set plot height.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT float height: the plot height.
@RETURN void: none.
*/
EXP_API void set_plot_height(link_t_ptr ptr, float height);

/*
@PLOTTION get_plot_ruler: get plot ruler count.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN int: return the ruler count.
*/
EXP_API int	get_plot_ruler(link_t_ptr ptr);

/*
@PLOTTION set_plot_ruler: set plot ruler count.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT int n: the rulers.
@RETURN void: none.
*/
EXP_API void set_plot_ruler(link_t_ptr ptr, int n);

/*
@PLOTTION get_plot_type: get plot type.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning type.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_type(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION get_plot_type_ptr: get plot type string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_type_ptr(link_t_ptr ptr);

/*
@PLOTTION set_plot_type: set plot type.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* type: the plot type.
@INPUT int len: the type string length.
@RETURN void: none.
*/
EXP_API void set_plot_type(link_t_ptr ptr, const tchar_t* type, int len);

/*
@PLOTTION get_plot_title: get plot title.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning title.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_title(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION get_plot_title_ptr: get plot title string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_title_ptr(link_t_ptr ptr);

/*
@PLOTTION set_plot_title: set plot title.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* title: the plot title.
@INPUT int len: the title string length.
@RETURN void: none.
*/
EXP_API void set_plot_title(link_t_ptr ptr, const tchar_t* title, int len);

/*
@PLOTTION get_plot_style: get plot style.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning style.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_style(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION get_plot_style_ptr: get plot style string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_style_ptr(link_t_ptr ptr);

/*
@PLOTTION set_plot_style: set plot style.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* style: the plot style.
@INPUT int len: the style string length.
@RETURN void: none.
*/
EXP_API void set_plot_style(link_t_ptr ptr, const tchar_t* style, int len);

/*
@PLOTTION get_plot_y_stages: get plot y-stages string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning y-stages string array.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_y_stages(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_y_stages: set plot y-stages string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* y-stages: the plot y-stages string array.
@INPUT int len: the y-stages string length.
@RETURN void: none.
*/
EXP_API void set_plot_y_stages(link_t_ptr ptr, const tchar_t* y_stages, int len);

/*
@PLOTTION get_plot_y_stages_ptr: get plot y_stages string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_y_stages_ptr(link_t_ptr ptr);

/*
@PLOTTION get_plot_y_bases: get plot y-bases string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning y-bases string array.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_y_bases(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_y_bases: set plot y-bases string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* y-bases: the plot y-bases string array.
@INPUT int len: the y-bases string length.
@RETURN void: none.
*/
EXP_API void set_plot_y_bases(link_t_ptr ptr, const tchar_t* y_bases, int len);

/*
@PLOTTION get_plot_y_bases_ptr: get plot y_bases string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_y_bases_ptr(link_t_ptr ptr);

/*
@PLOTTION get_plot_y_steps: get plot y-steps string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning y-steps string array.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_y_steps(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_y_steps: set plot y-steps string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* y-steps: the plot y-steps string array.
@INPUT int len: the y-steps string length.
@RETURN void: none.
*/
EXP_API void set_plot_y_steps(link_t_ptr ptr, const tchar_t* y_steps, int len);

/*
@PLOTTION get_plot_y_steps_ptr: get plot y_steps string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_y_steps_ptr(link_t_ptr ptr);

/*
@PLOTTION get_plot_y_labels: get plot y-labels string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning y-labels string array.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_y_labels(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_y_labels: set plot y-labels string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* y-labels: the plot y-labels string array.
@INPUT int len: the y-labels string length.
@RETURN void: none.
*/
EXP_API void set_plot_y_labels(link_t_ptr ptr, const tchar_t* y_labels, int len);

/*
@PLOTTION get_plot_y_labels_ptr: get plot y_labels string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_y_labels_ptr(link_t_ptr ptr);

/*
@PLOTTION get_plot_y_colors: get plot y-colors string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning y-colors string array.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_y_colors(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_y_colors: set plot y-colors string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* y-colors: the plot y-colors string array.
@INPUT int len: the y-colors string length.
@RETURN void: none.
*/
EXP_API void set_plot_y_colors(link_t_ptr ptr, const tchar_t* y_colors, int len);

/*
@PLOTTION get_plot_y_colors_ptr: get plot y_colors string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_y_colors_ptr(link_t_ptr ptr);

/*
@PLOTTION get_plot_y_shapes: get plot y-shapes string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning y-shapes string array.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_y_shapes(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_y_shapes: set plot y-shapes string array.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* y-shapes: the plot y-shapes string array.
@INPUT int len: the y-shapes string length.
@RETURN void: none.
*/
EXP_API void set_plot_y_shapes(link_t_ptr ptr, const tchar_t* y_shapes, int len);

/*
@PLOTTION get_plot_y_shapes_ptr: get plot y_shapes string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_y_shapes_ptr(link_t_ptr ptr);

/*
@PLOTTION get_plot_x_labels: get plot x-labels string arrax.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning x-labels string arrax.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_x_labels(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_x_labels: set plot x-labels string arrax.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* x-labels: the plot x-labels string arrax.
@INPUT int len: the x-labels string length.
@RETURN void: none.
*/
EXP_API void set_plot_x_labels(link_t_ptr ptr, const tchar_t* x_labels, int len);

/*
@PLOTTION get_plot_x_labels_ptr: get plot x_labels string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_x_labels_ptr(link_t_ptr ptr);

/*
@PLOTTION get_plot_x_colors: get plot x-colors string arrax.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning x-colors string arrax.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_x_colors(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_x_colors: set plot x-colors string arrax.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* x-colors: the plot x-colors string arrax.
@INPUT int len: the x-colors string length.
@RETURN void: none.
*/
EXP_API void set_plot_x_colors(link_t_ptr ptr, const tchar_t* x_colors, int len);

/*
@PLOTTION get_plot_x_colors_ptr: get plot x_colors string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_x_colors_ptr(link_t_ptr ptr);

/*
@PLOTTION get_plot_rows: get plot matrix rows.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN int: return matrix rows.
*/
EXP_API int	get_plot_rows(link_t_ptr ptr);

/*
@PLOTTION set_plot_rows: set plot matrix rows.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT int rows: the matrix rows.
@RETURN void: none.
*/
EXP_API void set_plot_rows(link_t_ptr ptr, int rows);

/*
@PLOTTION get_plot_cols: get plot matrix cols.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN int: return matrix cols.
*/
EXP_API int	get_plot_cols(link_t_ptr ptr);

/*
@PLOTTION set_plot_cols: set plot matrix cols.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT int cols: the matrix cols.
@RETURN void: none.
*/
EXP_API void set_plot_cols(link_t_ptr ptr, int cols);

/*
@PLOTTION get_plot_matrix: get plot matrix string arrax.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT tchar_t* buf: the characters buffer for returning matrix string arrax.
@INPUT int max: the buffer size.
@RETURN int: return characters copied.
*/
EXP_API int	get_plot_matrix(link_t_ptr ptr, tchar_t* buf, int max);

/*
@PLOTTION set_plot_matrix: set plot matrix string arrax.
@INPUT link_t_ptr ptr: the plot link component.
@INPUT const tchar_t* matrix: the plot matrix string arrax.
@INPUT int len: the matrix string length.
@RETURN void: none.
*/
EXP_API void set_plot_matrix(link_t_ptr ptr, const tchar_t* matrix, int len);

/*
@PLOTTION get_plot_matrix_ptr: get plot matrix string token.
@INPUT link_t_ptr ptr: the plot link component.
@RETURN const tchar_t*: return string pointer.
*/
EXP_API const tchar_t*	get_plot_matrix_ptr(link_t_ptr ptr);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*PLOTDOC_H*/
