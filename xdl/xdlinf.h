/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc interface document

	@module	xdlinf.h | interface file

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


#ifndef _XDLINF_H
#define	_XDLINF_H

#include "xdldef.h"

typedef bool_t(*PF_CAN_ESCAPE)(void* p_obj);
typedef bool_t(*PF_WITH_EOF)(void* p_obj);

typedef int(*PF_READ_ESCAPE)(void* p_obj, int max, int pos, int encode, tchar_t* pch);
typedef int(*PF_WRITE_ESCAPE)(void* p_obj, int max, int pos, int encode, tchar_t ch);

typedef int(*PF_READ_CHAR)(void* p_obj, int max, int pos, int encode, tchar_t* pch);
typedef int(*PF_WRITE_CHAR)(void* p_obj, int max, int pos, int encode, const tchar_t* pch);

typedef int(*PF_WRITE_TOKEN)(void* p_obj, int max, int pos, int encode, const tchar_t* pch, int len);
typedef int(*PF_READ_TOKEN)(void* p_obj, int max, int pos, int encode, tchar_t* pch, int len);

typedef int(*PF_WRITE_INDENT)(void* p_obj, int max, int pos, int encode);
typedef int(*PF_WRITE_CARRIAGE)(void* p_obj, int max, int pos, int encode);

typedef void(*PF_SET_ENCODE)(void* p_obj, int encode);

typedef struct _if_operator_t{
	void* obj;
	dword_t max, pos;
	int encode;
	bool_t isdom;

	PF_SET_ENCODE		pf_set_encode;
	PF_READ_CHAR		pf_read_char;
	PF_WRITE_CHAR		pf_write_char;

	PF_READ_ESCAPE		pf_read_escape;
	PF_WRITE_ESCAPE		pf_write_escape;

	PF_READ_TOKEN		pf_read_token;
	PF_WRITE_TOKEN		pf_write_token;

	PF_WRITE_INDENT		pf_write_indent;
	PF_WRITE_CARRIAGE	pf_write_carriage;

	PF_CAN_ESCAPE		pf_can_escape;
	PF_WITH_EOF			pf_with_eof;
}if_operator_t;

typedef void(*PF_XML_WRITE_BEGIN)(void* pv);
typedef bool_t(*PF_XML_HEAD_WRITE_ATTR)(void* pv, const tchar_t* key, int klen, const tchar_t* val, int vlen);
typedef bool_t(*PF_XML_NODE_WRITE_BEGIN)(void* pv);
typedef bool_t(*PF_XML_NODE_WRITE_END)(void* pv);
typedef bool_t(*PF_XML_NODE_WRITE_NAME)(void* pv, const tchar_t* ns, int nslen, const tchar_t* na, int nalen);
typedef bool_t(*PF_XML_NODE_WRITE_ATTR)(void* pv, const tchar_t* key, int klen, const tchar_t* val, int vlen);
typedef bool_t(*PF_XML_NODE_WRITE_XMLNS)(void* pv, const tchar_t* key, int klen, const tchar_t* val, int vlen);
typedef bool_t(*PF_XML_NODE_WRITE_TEXT)(void* pv, bool_t b_cdata, const tchar_t* text, int len);
typedef void(*PF_XML_WRITE_END)(void* pv, int code);
typedef bool_t(*PF_XML_HAS_NODE)(void* pv);

typedef struct _xml_writer_t{
	PF_XML_WRITE_BEGIN		pf_write_begin;
	PF_XML_HEAD_WRITE_ATTR	pf_head_write_attr;
	PF_XML_NODE_WRITE_BEGIN	pf_node_write_begin;
	PF_XML_NODE_WRITE_NAME	pf_node_write_name;
	PF_XML_NODE_WRITE_ATTR	pf_node_write_attr;
	PF_XML_NODE_WRITE_XMLNS	pf_node_write_xmlns;
	PF_XML_NODE_WRITE_TEXT	pf_node_write_text;
	PF_XML_NODE_WRITE_END	pf_node_write_end;
	PF_XML_WRITE_END		pf_write_end;
	PF_XML_HAS_NODE			pf_has_node;
	void* obj;
}xml_writer_t;

typedef bool_t(*PF_XML_HEAD_READ_BEGIN)(void* pv);
typedef bool_t(*PF_XML_HEAD_READ_ATTR)(void* pv, const tchar_t* key, const tchar_t* val);
typedef bool_t(*PF_XML_HEAD_READ_END)(void* pv);
typedef bool_t(*PF_XML_NODE_READ_BEGIN)(void* pv, int indent, bool_t b_parent, const tchar_t* ns, const tchar_t* nn);
typedef bool_t(*PF_XML_NODE_READ_CLOSE)(void* pv, int indent, bool_t b_parent);
typedef bool_t(*PF_XML_NODE_READ_END)(void* pv, int indent, bool_t b_parent, bool_t b_close, const tchar_t* ns, const tchar_t* nn);
typedef bool_t(*PF_XML_NODE_READ_XMLNS)(void* pv, const tchar_t* ns, const tchar_t* url);
typedef bool_t(*PF_XML_NODE_READ_ATTR)(void* pv, const tchar_t* key, const tchar_t* val);
typedef bool_t(*PF_XML_NODE_READ_TEXT)(void* pv, bool_t b_cdata, const tchar_t* text, int len);

typedef struct _xml_reader_t{
	PF_XML_HEAD_READ_BEGIN		pf_head_read_begin;
	PF_XML_HEAD_READ_ATTR		pf_head_read_attr;
	PF_XML_HEAD_READ_END		pf_head_read_end;
	PF_XML_NODE_READ_BEGIN		pf_node_read_begin;
	PF_XML_NODE_READ_CLOSE		pf_node_read_close;
	PF_XML_NODE_READ_XMLNS		pf_node_read_xmlns;
	PF_XML_NODE_READ_ATTR		pf_node_read_attr;
	PF_XML_NODE_READ_TEXT		pf_node_read_text;
	PF_XML_NODE_READ_END		pf_node_read_end;
	void* obj;
}xml_reader_t;

#define SOCK_OPTION_SNDBUF		1
#define SOCK_OPTION_RCVBUF		2
#define SOCK_OPTION_NONBLK		3
#define FILE_OPTION_TIME		10
#define FILE_OPTION_SINCE		11

typedef xhand_t(*PF_BIO_OPEN)(const secu_desc_t*, const tchar_t*, dword_t);
typedef bool_t(*PF_BIO_READ)(xhand_t, byte_t*, dword_t*);
typedef bool_t(*PF_BIO_WRITE)(xhand_t, const byte_t*, dword_t*);
typedef bool_t(*PF_BIO_FLUSH)(xhand_t);
typedef bool_t(*PF_BIO_READ_RANGE)(xhand_t, dword_t, dword_t, byte_t*, dword_t);
typedef bool_t(*PF_BIO_WRITE_RANGE)(xhand_t, dword_t, dword_t, const byte_t*, dword_t);
typedef void(*PF_BIO_CLOSE)(xhand_t);
typedef bool_t(*PF_BIO_SETOPT)(xhand_t, int, void*, int);

typedef struct _if_bio_t{
	xhand_t		bio;
	PF_BIO_WRITE	pf_write;
	PF_BIO_FLUSH	pf_flush;
	PF_BIO_READ		pf_read;
	PF_BIO_READ_RANGE	pf_read_range;
	PF_BIO_WRITE_RANGE	pf_write_range;
	PF_BIO_CLOSE		pf_close;
	PF_BIO_SETOPT	pf_setopt;
}if_bio_t;

typedef void(*PF_LOG_TITLE)(file_t, const tchar_t*, int);
typedef void(*PF_LOG_ERROR)(file_t, const tchar_t*, const tchar_t*, int);
typedef void(*PF_LOG_DATA)(file_t, const byte_t*, dword_t);
typedef void(*PF_LOG_XML)(file_t, link_t_ptr);
typedef void(*PF_LOG_JSON)(file_t, link_t_ptr);

typedef void(*PF_TRACK_ERROR)(void* hand, const tchar_t* code, const tchar_t* text);

typedef float(*PF_MEASURE_PIXEL)(void*, bool_t);
typedef void(*PF_MEASURE_RECT)(void*, const xfont_t*, const xface_t*, const tchar_t*, int, xrect_t* pxr);
typedef void(*PF_MEASURE_SIZE)(void*, const xfont_t*, const tchar_t*, int, xsize_t* pxs);
typedef void(*PF_MEASURE_FONT)(void*, const xfont_t*, xsize_t* pxs);

typedef struct _if_measure_t{
	void* ctx; //visual_t or canvas_t

	PF_MEASURE_PIXEL	pf_measure_pixel;
	PF_MEASURE_RECT		pf_measure_rect;
	PF_MEASURE_SIZE		pf_measure_size;
	PF_MEASURE_FONT		pf_measure_font;

	xrect_t rect;
}if_measure_t;


typedef void(*PF_RECT_PT_TO_TM)(canvas_t, xrect_t*);
typedef void(*PF_RECT_TM_TO_PT)(canvas_t, xrect_t*);
typedef void(*PF_SIZE_PT_TO_TM)(canvas_t, xsize_t*);
typedef void(*PF_SIZE_TM_TO_PT)(canvas_t, xsize_t*);
typedef void(*PF_POINT_PT_TO_TM)(canvas_t, xpoint_t*);
typedef void(*PF_POINT_TM_TO_PT)(canvas_t, xpoint_t*);

typedef void(*PF_GET_MEASURE)(canvas_t, if_measure_t* pim);
typedef void(*PF_GET_MEASURE_RAW)(visual_t, if_measure_t* pim);

typedef void(*PF_DRAW_LINE)(canvas_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_LINE_RAW)(visual_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_BEZIER)(canvas_t, const xpen_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_BEZIER_RAW)(visual_t, const xpen_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_CURVE)(canvas_t, const xpen_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_CURVE_RAW)(visual_t, const xpen_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_ARC)(canvas_t, const xpen_t*, const xpoint_t*, const xpoint_t*, const xsize_t*, bool_t, bool_t);
typedef void(*PF_DRAW_ARC_RAW)(visual_t, const xpen_t*, const xpoint_t*, const xpoint_t*, const xsize_t*, bool_t, bool_t);
typedef void(*PF_DRAW_POLYLINE)(canvas_t, const xpen_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_POLYLINE_RAW)(visual_t, const xpen_t*, const xpoint_t*, int);

typedef void(*PF_DRAW_RECT)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_RECT_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_ROUND)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_ROUND_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_ELLIPSE)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_ELLIPSE_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_PIE)(canvas_t, const xpen_t*, const xbrush_t*, const xpoint_t*, const xsize_t*, double, double);
typedef void(*PF_DRAW_PIE_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xpoint_t*, const xsize_t*, double, double);
typedef void(*PF_DRAW_FAN)(canvas_t, const xpen_t*, const xbrush_t*, const xpoint_t*, const xsize_t*, double, double);
typedef void(*PF_DRAW_FAN_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xpoint_t*, const xsize_t*, double, double);
typedef void(*PF_CALC_FAN)(canvas_t, const xpoint_t*, const xsize_t*, double, double, xpoint_t* pa, int n);
typedef void(*PF_CALC_FAN_RAW)(visual_t, const xpoint_t*, const xsize_t*, double, double, xpoint_t* pa, int n);
typedef void(*PF_DRAW_TRIANGLE)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*, const tchar_t*);
typedef void(*PF_DRAW_TRIANGLE_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*, const tchar_t*);
typedef void(*PF_DRAW_ARROW)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*, const xspan_t*, double);
typedef void(*PF_DRAW_ARROW_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xrect_t*, const xspan_t*, double);
typedef void(*PF_DRAW_POLYGON)(canvas_t, const xpen_t*, const xbrush_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_POLYGON_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_EQUALGON)(canvas_t, const xpen_t*, const xbrush_t*, const xpoint_t*, const xspan_t*, int);
typedef void(*PF_DRAW_EQUALGON_RAW)(visual_t, const xpen_t*, const xbrush_t*, const xpoint_t*, const xspan_t*, int);
typedef void(*PF_CALC_EQUALGON)(canvas_t, const xpoint_t*, const xspan_t*, int, xpoint_t*);
typedef void(*PF_CALC_EQUALGON_RAW)(visual_t, const xpoint_t*, const xspan_t*, int, xpoint_t*);
typedef void(*PF_DRAW_PATH)(canvas_t, const xpen_t*, const xbrush_t*, const tchar_t*, const xpoint_t*, int n);
typedef void(*PF_DRAW_PATH_RAW)(visual_t, const xpen_t*, const xbrush_t*, const tchar_t*, const xpoint_t*, int n);

typedef void(*PF_TEXT_RECT)(canvas_t, const xfont_t*, const xface_t*, const tchar_t*, int, xrect_t* pxr);
typedef void(*PF_TEXT_RECT_RAW)(visual_t, const xfont_t*, const xface_t*, const tchar_t*, int, xrect_t* pxr);
typedef void(*PF_TEXT_SIZE)(canvas_t, const xfont_t*, const tchar_t*, int, xsize_t* pps);
typedef void(*PF_TEXT_SIZE_RAW)(visual_t, const xfont_t*, const tchar_t*, int, xsize_t* pps);
typedef void(*PF_TEXT_METRIC)(canvas_t, const xfont_t*, xsize_t* pps);
typedef void(*PF_TEXT_METRIC_RAW)(visual_t, const xfont_t*, xsize_t* pps);
typedef void(*PF_DRAW_TEXT)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int);
typedef void(*PF_DRAW_TEXT_RAW)(visual_t, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int);
typedef void(*PF_TEXT_OUT)(canvas_t, const xfont_t*, const xpoint_t*, const tchar_t*, int);
typedef void(*PF_TEXT_OUT_RAW)(visual_t, const xfont_t*, const xpoint_t*, const tchar_t*, int);
typedef void(*PF_MULTI_LINE)(canvas_t, const xfont_t*, const xface_t*, const xpen_t*, const xrect_t*);
typedef void(*PF_MULTI_LINE_RAW)(visual_t, const xfont_t*, const xface_t*, const xpen_t*, const xrect_t*);

typedef void(*PF_COLOR_OUT)(canvas_t, const xrect_t*, bool_t horz, const tchar_t*, int);
typedef void(*PF_COLOR_OUT_RAW)(visual_t, const xrect_t*, bool_t horz, const tchar_t*, int);
typedef void(*PF_DRAW_IMAGE)(canvas_t, const ximage_t*, const xrect_t*);
typedef void(*PF_DRAW_IMAGE_RAW)(visual_t, const ximage_t*, const xrect_t*);
typedef void(*PF_DRAW_ICON)(canvas_t, const tchar_t*, const xrect_t*);
typedef void(*PF_DRAW_ICON_RAW)(visual_t, const tchar_t*, const xrect_t*);
typedef void(*PF_DRAW_THUMB)(canvas_t, const tchar_t*, const xrect_t*);
typedef void(*PF_DRAW_THUMB_RAW)(visual_t, const tchar_t*, const xrect_t*);
typedef void(*PF_DRAW_BITMAP)(canvas_t, bitmap_t, const xpoint_t*);
typedef void(*PF_DRAW_BITMAP_RAW)(visual_t, bitmap_t, const xpoint_t*);

typedef void(*PF_INCLIP_RECT_RAW)(visual_t, const xrect_t*);
typedef void(*PF_EXCLIP_RECT_RAW)(visual_t, const xrect_t*);
typedef void(*PF_GRADIENT_RECT_RAW)(visual_t, const xcolor_t*, const xcolor_t*, const tchar_t*, const xrect_t*);
typedef void(*PF_ALPHABLEND_RECT_RAW)(visual_t, const xcolor_t*, const xrect_t*, int);


typedef struct _if_visual_t{
	visual_t visual;

	PF_GET_MEASURE_RAW		pf_get_measure_raw;

	PF_DRAW_LINE_RAW		pf_draw_line_raw;
	PF_DRAW_BEZIER_RAW		pf_draw_bezier_raw;
	PF_DRAW_CURVE_RAW		pf_draw_curve_raw;
	PF_DRAW_ARC_RAW			pf_draw_arc_raw;
	PF_DRAW_POLYLINE_RAW	pf_draw_polyline_raw;

	PF_DRAW_RECT_RAW		pf_draw_rect_raw;
	PF_DRAW_TRIANGLE_RAW	pf_draw_triangle_raw;
	PF_DRAW_ROUND_RAW		pf_draw_round_raw;
	PF_DRAW_ELLIPSE_RAW		pf_draw_ellipse_raw;
	PF_DRAW_PIE_RAW			pf_draw_pie_raw;
	PF_DRAW_FAN_RAW			pf_draw_fan_raw;
	PF_CALC_FAN_RAW			pf_calc_fan_raw;
	PF_DRAW_ARROW_RAW		pf_draw_arrow_raw;
	PF_DRAW_POLYGON_RAW		pf_draw_polygon_raw;
	PF_DRAW_EQUALGON_RAW	pf_draw_equalgon_raw;
	PF_CALC_EQUALGON_RAW	pf_calc_equalgon_raw;
	PF_DRAW_PATH_RAW		pf_draw_path_raw;

	PF_TEXT_RECT_RAW		pf_text_rect_raw;
	PF_TEXT_SIZE_RAW		pf_text_size_raw;
	PF_TEXT_METRIC_RAW		pf_text_metric_raw;
	PF_DRAW_TEXT_RAW		pf_draw_text_raw;
	PF_TEXT_OUT_RAW			pf_text_out_raw;
	PF_MULTI_LINE_RAW		pf_multi_line_raw;

	PF_COLOR_OUT_RAW		pf_color_out_raw;
	PF_DRAW_ICON_RAW		pf_draw_icon_raw;
	PF_DRAW_THUMB_RAW		pf_draw_thumb_raw;
	PF_DRAW_IMAGE_RAW		pf_draw_image_raw;
	PF_DRAW_BITMAP_RAW		pf_draw_bitmap_raw;

	PF_INCLIP_RECT_RAW		pf_inclip_rect_raw;
	PF_EXCLIP_RECT_RAW		pf_exclip_rect_raw;
	PF_GRADIENT_RECT_RAW	pf_gradient_rect_raw;
	PF_ALPHABLEND_RECT_RAW	pf_alphablend_rect_raw;

	viewbox_t rect;
}if_visual_t;

typedef visual_t(*PF_GET_CANVAS_VIEWING)(canvas_t);
typedef void(*PF_GET_VIEWING_INTERFACE)(canvas_t, if_visual_t*);

typedef struct _if_canvas_t{
	canvas_t canvas;

	PF_GET_VIEWING_INTERFACE pf_get_visual_interface;
	PF_GET_CANVAS_VIEWING	pf_get_canvas_visual;

	PF_GET_MEASURE		pf_get_measure;

	PF_RECT_PT_TO_TM	pf_rect_pt_to_tm;
	PF_RECT_TM_TO_PT	pf_rect_tm_to_pt;
	PF_SIZE_PT_TO_TM	pf_size_pt_to_tm;
	PF_SIZE_TM_TO_PT	pf_size_tm_to_pt;
	PF_POINT_PT_TO_TM	pf_point_pt_to_tm;
	PF_POINT_TM_TO_PT	pf_point_tm_to_pt;

	PF_DRAW_LINE		pf_draw_line;
	PF_DRAW_BEZIER		pf_draw_bezier;
	PF_DRAW_CURVE		pf_draw_curve;
	PF_DRAW_ARC			pf_draw_arc;
	PF_DRAW_POLYLINE	pf_draw_polyline;

	PF_DRAW_RECT		pf_draw_rect;
	PF_DRAW_TRIANGLE	pf_draw_triangle;
	PF_DRAW_ROUND		pf_draw_round;
	PF_DRAW_ELLIPSE		pf_draw_ellipse;
	PF_DRAW_PIE			pf_draw_pie;
	PF_DRAW_FAN			pf_draw_fan;
	PF_CALC_FAN			pf_calc_fan;
	PF_DRAW_ARROW		pf_draw_arrow;
	PF_DRAW_POLYGON		pf_draw_polygon;
	PF_DRAW_EQUALGON	pf_draw_equalgon;
	PF_CALC_EQUALGON	pf_calc_equalgon;
	PF_DRAW_PATH		pf_draw_path;

	PF_TEXT_RECT		pf_text_rect;
	PF_TEXT_SIZE		pf_text_size;
	PF_TEXT_METRIC		pf_text_metric;
	PF_DRAW_TEXT		pf_draw_text;
	PF_TEXT_OUT			pf_text_out;
	PF_MULTI_LINE		pf_multi_line;

	PF_COLOR_OUT		pf_color_out;
	PF_DRAW_ICON		pf_draw_icon;
	PF_DRAW_THUMB		pf_draw_thumb;
	PF_DRAW_IMAGE		pf_draw_image;
	PF_DRAW_BITMAP		pf_draw_bitmap;

	xcolor_t clr_bkg;
	xcolor_t clr_frg;
	xcolor_t clr_txt;
	xcolor_t clr_ico;
	xcolor_t clr_msk;

	canvbox_t rect;
}if_canvas_t;



#endif	/* _XDLINF_H */

