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


typedef bool_t(*PF_BIO_READ)(xhand_t, byte_t*, dword_t*);
typedef bool_t(*PF_BIO_WRITE)(xhand_t, const byte_t*, dword_t*);
typedef bool_t(*PF_BIO_FLUSH)(xhand_t);
typedef void(*PF_BIO_CLOSE)(xhand_t);
typedef bool_t(*PF_BIO_SETOPT)(xhand_t, int, void*, int);
typedef bool_t(*PF_BIO_GETOPT)(xhand_t, int, void*, int);
typedef unsigned short(*PF_BIO_ADDR)(xhand_t, tchar_t*);
typedef unsigned short(*PF_BIO_PEER)(xhand_t, tchar_t*);

typedef struct _if_bio_t{
	xhand_t		fd;

	PF_BIO_WRITE		pf_write;
	PF_BIO_FLUSH		pf_flush;
	PF_BIO_READ			pf_read;
	PF_BIO_CLOSE		pf_close;
	PF_BIO_SETOPT		pf_setopt;
	PF_BIO_GETOPT		pf_getopt;
	PF_BIO_ADDR			pf_addr;
	PF_BIO_PEER			pf_peer;
}if_bio_t;

typedef xhand_t(*PF_FIO_OPEN)(const secu_desc_t*, const tchar_t*, dword_t);
typedef bool_t(*PF_FIO_READ)(xhand_t, byte_t*, dword_t*);
typedef bool_t(*PF_FIO_WRITE)(xhand_t, const byte_t*, dword_t*);
typedef bool_t(*PF_FIO_READ_RANGE)(xhand_t, dword_t, dword_t, byte_t*, dword_t);
typedef bool_t(*PF_FIO_WRITE_RANGE)(xhand_t, dword_t, dword_t, const byte_t*, dword_t);
typedef bool_t(*PF_FIO_FLUSH)(xhand_t);
typedef void(*PF_FIO_CLOSE)(xhand_t);
typedef bool_t(*PF_FIO_SETOPT)(xhand_t, int, void*, int);
typedef bool_t(*PF_FIO_GETOPT)(xhand_t, int, void*, int);

typedef struct _if_fio_t{
	xhand_t		fd;

	PF_FIO_WRITE		pf_write;
	PF_FIO_FLUSH		pf_flush;
	PF_FIO_READ			pf_read;
	PF_FIO_READ_RANGE	pf_read_range;
	PF_FIO_WRITE_RANGE	pf_write_range;
	PF_FIO_CLOSE		pf_close;
	PF_FIO_SETOPT		pf_setopt;
	PF_FIO_GETOPT		pf_getopt;
}if_fio_t;

typedef void(*PF_LOG_TITLE)(stream_t, const tchar_t*, int);
typedef void(*PF_LOG_ERROR)(stream_t, const tchar_t*, const tchar_t*, int);
typedef void(*PF_LOG_DATA)(stream_t, const byte_t*, dword_t);
typedef void(*PF_LOG_XML)(stream_t, link_t_ptr);
typedef void(*PF_LOG_JSON)(stream_t, link_t_ptr);

typedef struct _if_log_t{
	stream_t log;

	PF_LOG_TITLE	pf_log_title;
	PF_LOG_ERROR	pf_log_error;
	PF_LOG_DATA		pf_log_data;
	PF_LOG_XML		pf_log_xml;
	PF_LOG_JSON		pf_log_json;
}if_log_t;

typedef bool_t(*PF_SEND_EVENT)(const tchar_t*, bool_t, link_t_ptr);
typedef bool_t(*PF_QUERY_EVENT)(const tchar_t*, bool_t , link_t_ptr);

typedef struct _if_post_t{
	tchar_t url[PATH_LEN + 1];

	PF_SEND_EVENT	pf_send_event;
	PF_QUERY_EVENT	pf_query_event;
}if_post_t;

typedef void(*PF_TRACK_ERROR)(void* param, const tchar_t* code, const tchar_t* text);

typedef struct _if_track_t{
	void* param;

	PF_TRACK_ERROR	pf_track_error;
}if_track_t;

#if defined(XDL_SUPPORT_VIEW)

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


typedef void(*PF_RECT_PT_TO_TM)(void*, xrect_t*);
typedef void(*PF_RECT_TM_TO_PT)(void*, xrect_t*);
typedef void(*PF_SIZE_PT_TO_TM)(void*, xsize_t*);
typedef void(*PF_SIZE_TM_TO_PT)(void*, xsize_t*);
typedef void(*PF_POINT_PT_TO_TM)(void*, xpoint_t*);
typedef void(*PF_POINT_TM_TO_PT)(void*, xpoint_t*);

typedef void(*PF_GET_MEASURE)(void*, if_measure_t*);

typedef void(*PF_DRAW_LINE)(void*, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_BEZIER)(void*, const xpen_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_CURVE)(void*, const xpen_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_ARC)(void*, const xpen_t*, const xpoint_t*, const xpoint_t*, const xsize_t*, bool_t, bool_t);
typedef void(*PF_DRAW_POLYLINE)(void*, const xpen_t*, const xpoint_t*, int);

typedef void(*PF_DRAW_RECT)(void*, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_ROUND)(void*, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_ELLIPSE)(void*, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_PIE)(void*, const xpen_t*, const xbrush_t*, const xpoint_t*, const xsize_t*, double, double);
typedef void(*PF_DRAW_FAN)(void*, const xpen_t*, const xbrush_t*, const xpoint_t*, const xsize_t*, double, double);
typedef void(*PF_CALC_FAN)(void*, const xpoint_t*, const xsize_t*, double, double, xpoint_t* pa, int n);
typedef void(*PF_DRAW_TRIANGLE)(void*, const xpen_t*, const xbrush_t*, const xrect_t*, const tchar_t*);
typedef void(*PF_DRAW_ARROW)(void*, const xpen_t*, const xbrush_t*, const xrect_t*, const xspan_t*, double);
typedef void(*PF_DRAW_POLYGON)(void*, const xpen_t*, const xbrush_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_EQUALGON)(void*, const xpen_t*, const xbrush_t*, const xpoint_t*, const xspan_t*, int);
typedef void(*PF_CALC_EQUALGON)(void*, const xpoint_t*, const xspan_t*, int, xpoint_t*);
typedef void(*PF_DRAW_PATH)(void*, const xpen_t*, const xbrush_t*, const tchar_t*, const xpoint_t*, int n);

typedef void(*PF_TEXT_RECT)(void*, const xfont_t*, const xface_t*, const tchar_t*, int, xrect_t* pxr);
typedef void(*PF_TEXT_SIZE)(void*, const xfont_t*, const tchar_t*, int, xsize_t* pps);
typedef void(*PF_TEXT_METRIC)(void*, const xfont_t*, xsize_t* pps);
typedef void(*PF_DRAW_TEXT)(void*, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int);
typedef void(*PF_TEXT_OUT)(void*, const xfont_t*, const xpoint_t*, const tchar_t*, int);
typedef void(*PF_MULTI_LINE)(void*, const xfont_t*, const xface_t*, const xpen_t*, const xrect_t*);

typedef void(*PF_COLOR_OUT)(void*, const xrect_t*, bool_t horz, const tchar_t*, int);
typedef void(*PF_DRAW_IMAGE)(void*, const ximage_t*, const xrect_t*);
typedef void(*PF_DRAW_ICON)(void*, const tchar_t*, const xrect_t*);
typedef void(*PF_DRAW_THUMB)(void*, const tchar_t*, const xrect_t*);
typedef void(*PF_DRAW_BITMAP)(void*, bitmap_t, const xpoint_t*);

typedef void(*PF_INCLIP_RECT)(void*, const xrect_t*);
typedef void(*PF_EXCLIP_RECT)(void*, const xrect_t*);
typedef void(*PF_GRADIENT_RECT)(void*, const xcolor_t*, const xcolor_t*, const tchar_t*, const xrect_t*);
typedef void(*PF_ALPHABLEND_RECT)(void*, const xcolor_t*, const xrect_t*, int);

typedef visual_t(*PF_GET_VISUAL)(void*);

typedef struct _if_drawing_t* if_drawing_ptr;

typedef void(*PF_GET_INTERFACE)(void*, if_drawing_ptr);

typedef struct _if_drawing_t{
	int tag;

	void* ctx;

	PF_GET_MEASURE		pf_get_measure;

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

	//the visual only
	PF_INCLIP_RECT		pf_inclip_rect;
	PF_EXCLIP_RECT		pf_exclip_rect;
	PF_GRADIENT_RECT	pf_gradient_rect;
	PF_ALPHABLEND_RECT	pf_alphablend_rect;

	// the canvas only
	PF_RECT_PT_TO_TM	pf_rect_pt_to_tm;
	PF_RECT_TM_TO_PT	pf_rect_tm_to_pt;
	PF_SIZE_PT_TO_TM	pf_size_pt_to_tm;
	PF_SIZE_TM_TO_PT	pf_size_tm_to_pt;
	PF_POINT_PT_TO_TM	pf_point_pt_to_tm;
	PF_POINT_TM_TO_PT	pf_point_tm_to_pt;

	PF_GET_MEASURE		pf_get_visual_measure;
	PF_GET_INTERFACE	pf_get_visual_interface;
	PF_GET_VISUAL		pf_get_visual_handle;

	clr_mod_t mode;

	xrect_t rect;
}if_drawing_t;


typedef bool_t(STDCALL *PF_DB_PARSE_DSN)(const tchar_t*, tchar_t*, int, tchar_t*, int, tchar_t*, int, tchar_t*, int);
typedef xdb_t(STDCALL *PF_DB_OPEN)(const tchar_t*, const tchar_t*, const tchar_t*, const tchar_t*);
typedef xdb_t(STDCALL *PF_DB_OPEN_DSN)(const tchar_t*);
typedef void(STDCALL *PF_DB_CLOSE)(xdb_t);
typedef bool_t(STDCALL *PF_DB_EXEC)(xdb_t, const tchar_t*, int);
typedef bool_t(STDCALL *PF_DB_SELECT)(xdb_t, link_t_ptr, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_SCHEMA)(xdb_t, link_t_ptr, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_FETCH)(xdb_t, link_t_ptr);
typedef bool_t(STDCALL *PF_DB_UPDATE)(xdb_t, link_t_ptr);
typedef bool_t(STDCALL *PF_DB_DATETIME)(xdb_t, int, tchar_t*);
typedef int(STDCALL *PF_DB_ROWS)(xdb_t);
typedef int(STDCALL *PF_DB_ERROR)(xdb_t, tchar_t*, int);
typedef bool_t(STDCALL *PF_DB_CALL_FUNC)(xdb_t, link_t_ptr);
typedef bool_t(STDCALL *PF_DB_CALL_JSON)(xdb_t, const tchar_t*, link_t_ptr);
typedef bool_t(STDCALL *PF_DB_EXPORT)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_IMPORT)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_BATCH)(xdb_t, stream_t);
typedef bool_t(STDCALL *PF_DB_WRITE_BLOB)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_READ_BLOB)(xdb_t, stream_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_WRITE_CLOB)(xdb_t, string_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_READ_CLOB)(xdb_t, string_t, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_WRITE_XDOC)(xdb_t, link_t_ptr, const tchar_t*);
typedef bool_t(STDCALL *PF_DB_READ_XDOC)(xdb_t, link_t_ptr, const tchar_t*);

typedef int(*PF_DB_CALL_ARGV)(xdb_t, const tchar_t*, const tchar_t*, ...);

typedef struct _if_xdb_t{
	res_modu_t lib;
	xdb_t xdb;

	PF_DB_PARSE_DSN	pf_db_parse_dsn;
	PF_DB_OPEN_DSN	pf_db_open_dsn;
	PF_DB_OPEN		pf_db_open;
	PF_DB_CLOSE		pf_db_close;

	PF_DB_EXEC		pf_db_exec;
	PF_DB_SELECT	pf_db_select;
	PF_DB_SCHEMA	pf_db_schema;
	PF_DB_FETCH		pf_db_fetch;
	PF_DB_UPDATE	pf_db_update;
	PF_DB_CALL_FUNC	pf_db_call_func;
	PF_DB_CALL_JSON	pf_db_call_json;
	PF_DB_EXPORT	pf_db_export;
	PF_DB_IMPORT	pf_db_import;
	PF_DB_BATCH		pf_db_batch;
	PF_DB_WRITE_BLOB	pf_db_write_blob;
	PF_DB_READ_BLOB		pf_db_read_blob;
	PF_DB_WRITE_CLOB	pf_db_write_clob;
	PF_DB_READ_CLOB		pf_db_read_clob;
	PF_DB_WRITE_XDOC	pf_db_write_xdoc;
	PF_DB_READ_XDOC		pf_db_read_xdoc;

	PF_DB_DATETIME	pf_db_datetime;
	PF_DB_ROWS		pf_db_rows;
	PF_DB_ERROR		pf_db_error;

}if_xdb_t;

#endif /*XDL_SUPPORT_VIEW*/

#endif	/* _XDLINF_H */

