/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc interface document

	@module	xdlinf.h | interface interface file

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


typedef xhand_t(*PF_BIO_OPEN)(const secu_desc_t*, const tchar_t*, dword_t);
typedef bool_t(*PF_BIO_READ)(xhand_t, byte_t*, dword_t*);
typedef bool_t(*PF_BIO_WRITE)(xhand_t, const byte_t*, dword_t*);
typedef bool_t(*PF_BIO_FLUSH)(xhand_t);
typedef bool_t(*PF_BIO_READ_RANGE)(xhand_t, dword_t, dword_t, byte_t*, dword_t);
typedef bool_t(*PF_BIO_WRITE_RANGE)(xhand_t, dword_t, dword_t, const byte_t*, dword_t);
typedef void(*PF_BIO_CLOSE)(xhand_t);
typedef void(*PF_BIO_SET_TIME)(xhand_t, const tchar_t*);
typedef void(*PF_BIO_SET_SINCE)(xhand_t, int);

typedef void(*PF_BIO_ENCRYPT)(byte_t* buf, dword_t size, void* pa);
typedef void(*PF_BIO_DECRYPT)(byte_t* buf, dword_t size, void* pa);

typedef void(*PF_BIO_SET_SNDBUF)(xhand_t, dword_t);
typedef void(*PF_BIO_SET_RDVBUF)(xhand_t, dword_t);
typedef void(*PF_BIO_SET_LINGER)(xhand_t, bool_t, int);
typedef unsigned short(*PF_BIO_PEER_PORT)(xhand_t, tchar_t*);

typedef struct _if_socket_bio{
	xhand_t		bio;
	PF_BIO_WRITE	pf_write;
	PF_BIO_FLUSH	pf_flush;
	PF_BIO_READ		pf_read;
	PF_BIO_ENCRYPT	pf_encrypt;
	PF_BIO_DECRYPT	pf_decrypt;

	PF_BIO_SET_SNDBUF	pf_set_sndbuf;
	PF_BIO_SET_RDVBUF	pf_set_rcvbuf;
	PF_BIO_SET_LINGER	pf_set_linger;

	PF_BIO_READ_RANGE	pf_read_range;
	PF_BIO_WRITE_RANGE	pf_write_range;
	PF_BIO_CLOSE		pf_close;
	PF_BIO_SET_TIME		pf_set_time;
	PF_BIO_SET_SINCE	pf_set_since;
	PF_BIO_PEER_PORT	pf_peer_port;
}if_bio_t;

typedef void(*PF_IMAGE_SIZE)(void*, const ximage_t*, xsize_t* pxs);
typedef void(*PF_TEXT_SIZE)(void*, const xfont_t*, const tchar_t*, int, xsize_t* pxs);
typedef void(*PF_TEXT_METRIC)(void*, const xfont_t*, xsize_t* pxs);

typedef struct _if_measure_t{
	void*	ctx;
	PF_TEXT_SIZE	pf_text_size;
	PF_TEXT_METRIC	pf_text_metric;
	PF_IMAGE_SIZE	pf_image_size;
}if_measure_t;

typedef int(*PF_CANVAS_TYPE)(canvas_t);
typedef void(*PF_DRAW_LINE)(canvas_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_3DLINE)(canvas_t, const xpen_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_3DRECT)(canvas_t, const xpen_t*, const xrect_t*);
typedef void(*PF_DRAW_POLYLINE)(canvas_t, const xpen_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_POLYGON)(canvas_t, const xpen_t*, const xbrush_t*, const xpoint_t*, int);
typedef void(*PF_DRAW_BEZIER)(canvas_t, const xpen_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*, const xpoint_t*);
typedef void(*PF_DRAW_RECT)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_GRADIENT_RECT)(canvas_t, const xgradi_t*, const xrect_t*);
typedef void(*PF_ALPHABLEND_RECT)(canvas_t, const xcolor_t*, const xrect_t*, int);
typedef void(*PF_DRAW_ROUND)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_ELLIPSE)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*);
typedef void(*PF_DRAW_PIE)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*, double, double);
typedef void(*PF_DRAW_ARC)(canvas_t, const xpen_t*, const xrect_t*, double, double);
typedef void(*PF_DRAW_ARROW)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*, float, double);
typedef void(*PF_DRAW_SHAPE)(canvas_t, const xpen_t*, const xbrush_t*, const xrect_t*, const tchar_t*);
typedef void(*PF_MULTI_LINE)(canvas_t, const xfont_t*, const xface_t*, const xpen_t*, const xrect_t*);
typedef void(*PF_DRAW_TEXT)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int);
typedef void(*PF_DRAW_PASSWORD)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int);
typedef void(*PF_TEXT_OUT)(canvas_t, const xfont_t*, const xpoint_t*, const tchar_t*, int);
typedef void(*PF_COLOR_OUT)(canvas_t, const xrect_t*, bool_t horz, const tchar_t*, int);
typedef void(*PF_DRAW_DATA)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, const tchar_t*, int, int, const tchar_t*, const tchar_t*, bool_t, bool_t);
typedef void(*PF_DRAW_IMAGE)(canvas_t, const ximage_t*, const xrect_t*);
typedef void(*PF_DRAW_ICON)(canvas_t, const xcolor_t*, const xrect_t*, const tchar_t* iname);
typedef void(*PF_DRAW_VAR_TEXT)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, string_t);

typedef void(*PF_DRAW_TAG_TEXT)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, link_t_ptr, int);
typedef int(*PF_CALC_TAG_PAGES)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, link_t_ptr);
typedef void(*PF_DRAW_RICH_TEXT)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, link_t_ptr, int);
typedef int(*PF_CALC_RICH_PAGES)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, link_t_ptr);
typedef void(*PF_DRAW_MEMO_TEXT)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, link_t_ptr, int);
typedef int(*PF_CALC_MEMO_PAGES)(canvas_t, const xfont_t*, const xface_t*, const xrect_t*, link_t_ptr);

typedef void(*PF_MEASURE_RECT)(canvas_t, const xfont_t*, const xface_t*, const tchar_t*, int, xrect_t* ppr);
typedef void(*PF_MEASURE_SIZE)(canvas_t, const xfont_t*, const tchar_t*, int, xsize_t* pps);
typedef void(*PF_MEASURE_METRIC)(canvas_t, const xfont_t*, xsize_t* pps);

typedef struct _if_canvas_t{
	canvas_t canvas;

	PF_CANVAS_TYPE		pf_canvas_type;

	PF_DRAW_LINE		pf_draw_line;
	PF_DRAW_3DLINE		pf_draw_3dline;
	PF_DRAW_POLYLINE	pf_draw_polyline;
	PF_DRAW_POLYGON		pf_draw_polygon;
	PF_DRAW_BEZIER		pf_draw_bezier;
	PF_DRAW_3DRECT		pf_draw_3drect;
	PF_DRAW_RECT		pf_draw_rect;
	PF_GRADIENT_RECT	pf_gradient_rect;
	PF_ALPHABLEND_RECT	pf_alphablend_rect;
	PF_DRAW_ROUND		pf_draw_round;
	PF_DRAW_ELLIPSE		pf_draw_ellipse;
	PF_DRAW_PIE			pf_draw_pie;
	PF_DRAW_ARC			pf_draw_arc;
	PF_DRAW_ARROW		pf_draw_arrow;
	PF_DRAW_SHAPE		pf_draw_shape;
	PF_MULTI_LINE		pf_multi_line;
	PF_DRAW_TEXT		pf_draw_text;
	PF_TEXT_OUT			pf_text_out;
	PF_COLOR_OUT		pf_color_out;
	PF_DRAW_PASSWORD	pf_draw_password;
	PF_DRAW_DATA		pf_draw_data;
	PF_DRAW_VAR_TEXT	pf_draw_var_text;

	PF_DRAW_TAG_TEXT	pf_draw_tag_text;
	PF_CALC_TAG_PAGES	pf_calc_tag_pages;
	PF_DRAW_RICH_TEXT	pf_draw_rich_text;
	PF_CALC_RICH_PAGES	pf_calc_rich_pages;
	PF_DRAW_MEMO_TEXT	pf_draw_memo_text;
	PF_CALC_MEMO_PAGES	pf_calc_memo_pages;
	PF_DRAW_IMAGE		pf_draw_image;
	PF_DRAW_ICON		pf_draw_icon;

	PF_MEASURE_RECT		pf_measure_rect;
	PF_MEASURE_SIZE		pf_measure_size;
	PF_MEASURE_METRIC	pf_measure_metric;

	xcolor_t clr_bkg;
	xcolor_t clr_frg;
	xcolor_t clr_txt;
	xcolor_t clr_msk;
}if_canvas_t;


#endif	/* _XDLINF_H */

