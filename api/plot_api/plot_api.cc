/***********************************************************************
	Easily plot web api

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc plot web api document

	@module	plot_api.cc | plot web api implement file

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

#include "plot_api.h"

#define CONENTYPE_IS_JSON(token)	((compare_text(token, xslen(HTTP_HEADER_CONTENTTYPE_APPJSON), HTTP_HEADER_CONTENTTYPE_APPJSON, -1, 1) == 0)? 1 : 0)
#define CONENTYPE_IS_UNKNOWN(token)	((is_null(token) || compare_text(token, 3, _T("*/*"), -1, 1) == 0)? 1 : 0)

#define DEF_COLOR_TABLE		_T("LightSlateGray,CornflowerBlue,DarkSalmon,ForestGreen,Indigo,LightSteelBlue,Orange,PapayaWhip")
#define DEF_PLOT_STYLE		_T("font-size:10;stroke-width:1;fill-color:WhiteSmoke;stroke-color:DimGray;fill-style:gradient;gradient:vert;")

typedef struct _plot_block_t{
	secu_desc_t sd;

	tchar_t path[PATH_LEN + 1];
}plot_block_t;

bool_t _invoke_ploting(const https_block_t* pb, plot_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };
	tchar_t sz_method[RES_LEN + 1] = { 0 };
	tchar_t sz_encoding[RES_LEN + 1] = { 0 };
	bool_t b_json = 0;

	tchar_t sz_name[RES_LEN + 1] = { 0 };
	tchar_t sz_file[PATH_LEN + 1] = { 0 };
	tchar_t sz_token[STYLE_LEN + 1] = { 0 };

	link_t_ptr ptr_xml = NULL;
	link_t_ptr ptr_json = NULL;

	link_t_ptr ptr_org = NULL;
	link_t_ptr ptr_plot;
	link_t_ptr ptr_svg = NULL;

	bool_t rt;

	TRY_CATCH;

	xhttp_get_url_method(pb->http, sz_method, RES_LEN);

	if (compare_text(sz_method, -1, HTTP_METHOD_GET, -1, 1) == 0)
	{
		ptr_plot = create_plot_doc();

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_TYPE, -1, sz_token, RES_LEN);
		if (!is_null(sz_token))
		{
			set_plot_type(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_WIDTH, -1, sz_token, NUM_LEN);
		if (!is_null(sz_token))
		{
			set_plot_width(ptr_plot, xstof(sz_token));
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_HEIGHT, -1, sz_token, NUM_LEN);
		if (!is_null(sz_token))
		{
			set_plot_height(ptr_plot, xstof(sz_token));
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_STYLE, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_style(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_RULER, -1, sz_token, INT_LEN);
		if (!is_null(sz_token))
		{
			set_plot_ruler(ptr_plot, xstol(sz_token));
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_Y_STAGES, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_y_stages_token(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_Y_BASES, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_y_bases_token(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_Y_STEPS, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_y_steps_token(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_Y_LABELS, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_y_labels_token(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_Y_COLORS, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_y_colors_token(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_X_LABELS, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_x_labels_token(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_PLOT_Y_COLORS, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_y_colors_token(ptr_plot, sz_token, -1);
		}

		xhttp_get_url_query_entity(pb->http, DOC_MATRIX_ROWS, -1, sz_token, INT_LEN);
		if (!is_null(sz_token))
		{
			set_plot_matrix_rows(ptr_plot, xstol(sz_token));
		}

		xhttp_get_url_query_entity(pb->http, DOC_MATRIX_COLS, -1, sz_token, INT_LEN);
		if (!is_null(sz_token))
		{
			set_plot_matrix_cols(ptr_plot, xstol(sz_token));
		}

		xhttp_get_url_query_entity(pb->http, DOC_MATRIX_DATA, -1, sz_token, STYLE_LEN);
		if (!is_null(sz_token))
		{
			set_plot_matrix_data(ptr_plot, sz_token, -1);
		}
	}
	else
	{
		xhttp_get_request_accept_type(pb->http, sz_encoding, RES_LEN);
		if (CONENTYPE_IS_UNKNOWN(sz_encoding))
		{
			xhttp_get_request_content_type(pb->http, sz_encoding, RES_LEN);
		}
		b_json = CONENTYPE_IS_JSON(sz_encoding);

		if (b_json)
		{
			ptr_json = create_json_doc();

			rt = xhttp_recv_json(pb->http, ptr_json);
			if (!rt)
			{
				get_last_error(sz_code, sz_error, ERR_LEN);
			}

			if (pb->plg)
			{
				(*pb->plg->pf_log_title)(pb->plg->log, _T("[PLOT]"), -1);

				(*pb->plg->pf_log_json)(pb->plg->log, ptr_json);
			}

			if (!rt)
			{
				raise_user_error(sz_code, sz_error);
			}
		}
		else
		{
			ptr_xml = create_xml_doc();

			rt = xhttp_recv_xml(pb->http, ptr_xml);
			if (!rt)
			{
				get_last_error(sz_code, sz_error, ERR_LEN);
			}

			if (pb->plg)
			{
				(*pb->plg->pf_log_title)(pb->plg->log, _T("[PLOT]"), -1);

				(*pb->plg->pf_log_xml)(pb->plg->log, ptr_xml);
			}

			if (!rt)
			{
				raise_user_error(sz_code, sz_error);
			}
		}

		if (b_json)
		{
			ptr_plot = ptr_json;
		}
		else
		{
			ptr_plot = get_xml_dom_node(ptr_xml);
		}
	}

	xsprintf(sz_file, _T("%s/%s"), pd->path, pb->object);
	split_file(sz_file, NULL, sz_name);

	if (!is_null(sz_name))
	{
		ptr_org = create_plot_from_meta_file(&pd->sd, sz_file);

		if (!ptr_org)
		{
			raise_user_error(_T("-1"), _T("load object falied"));
		}

		merge_plot_doc(ptr_plot, ptr_org);
		destroy_plot_doc(ptr_org);
		ptr_org = NULL;
	}

	get_plot_type(ptr_plot, sz_name, RES_LEN);
	if (is_null(sz_name))
	{
		raise_user_error(_T("-1"), _T("unknown plot type"));
	}

	if (is_zero_float(get_plot_width(ptr_plot)))
		set_plot_width(ptr_plot, DEF_PLOT_WIDTH);
	if (is_zero_float(get_plot_height(ptr_plot)))
		set_plot_height(ptr_plot, DEF_PLOT_HEIGHT);

	if (is_null(get_plot_style_ptr(ptr_plot)))
	{
		set_plot_style(ptr_plot, DEF_PLOT_STYLE, -1);
	}

	if (!get_plot_y_colors_token(ptr_plot, NULL, MAX_LONG))
	{
		set_plot_y_colors_token(ptr_plot, DEF_COLOR_TABLE, -1);
	}

	if (!get_plot_x_colors_token(ptr_plot, NULL, MAX_LONG))
	{
		set_plot_x_colors_token(ptr_plot, DEF_COLOR_TABLE, -1);
	}

	ptr_svg = create_svg_doc();

	svg_print_plot(ptr_svg, ptr_plot);

	if (compare_text(sz_method, -1, HTTP_METHOD_GET, -1, 1) == 0)
	{
		destroy_plot_doc(ptr_plot);
		ptr_plot = NULL;
	}
	else
	{
		if (b_json)
		{
			destroy_json_doc(ptr_json);
			ptr_json = NULL;
		}
		else
		{
			destroy_xml_doc(ptr_xml);
			ptr_xml = NULL;
		}
		ptr_plot = NULL;
	}

	xhttp_set_response_code(pb->http, HTTP_CODE_200);
	xhttp_set_response_message(pb->http, HTTP_CODE_200_TEXT, -1);

	xhttp_set_response_header(pb->http, HTTP_HEADER_CONTENTTYPE, -1, HTTP_HEADER_CONTENTTYPE_IMAGESVG, -1);

	ptr_xml = upcast_dom_to_xml(ptr_svg);

	rt = xhttp_send_xml(pb->http, ptr_xml);

	ptr_svg = downcast_xml_to_dom(ptr_xml);
	ptr_xml = NULL;

	destroy_svg_doc(ptr_svg);
	ptr_svg = NULL;

	END_CATCH;

	return 1;

ONERROR:

	get_last_error(sz_code, sz_error, ERR_LEN);

	xhttp_send_error(pb->http, NULL, NULL, sz_code, sz_error, -1);

	if (compare_text(sz_method, -1, HTTP_METHOD_GET, -1, 1) == 0)
	{
		if (ptr_plot)
			destroy_plot_doc(ptr_plot);
	}
	else
	{
		if (ptr_json)
			destroy_json_doc(ptr_json);

		if (ptr_xml)
			destroy_xml_doc(ptr_xml);
	}

	if (ptr_svg)
		destroy_svg_doc(ptr_svg);

	if (ptr_org)
		destroy_plot_doc(ptr_org);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->log, _T("[PLOT]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->log, sz_code, sz_error, -1);
	}

	return 0;
}

void _invoke_error(const https_block_t* pb, plot_block_t* pd)
{
	tchar_t sz_code[NUM_LEN + 1] = { 0 };
	tchar_t sz_error[ERR_LEN + 1] = { 0 };

	byte_t** d_recv = NULL;
	dword_t n_size = 0;

	get_last_error(sz_code, sz_error, ERR_LEN);

	d_recv = bytes_alloc();

	xhttp_recv_full(pb->http, d_recv, &n_size);

	bytes_free(d_recv);
	d_recv = NULL;

	xhttp_send_error(pb->http, HTTP_CODE_500, HTTP_CODE_500_TEXT, sz_code, sz_error, -1);

	if (pb->plg)
	{
		(*pb->plg->pf_log_title)(pb->plg->log, _T("[PLOT: 错误]"), -1);

		(*pb->plg->pf_log_error)(pb->plg->log, sz_code, sz_error, -1);
	}
}

/**********************************************************************************************/

int STDCALL https_invoke(const tchar_t* method, const https_block_t* pb)
{
	plot_block_t* pd = NULL;

	tchar_t token[PATH_LEN + 1] = { 0 };

	bool_t rt = 1;

	link_t_ptr ptr_prop = NULL;

	TRY_CATCH;

	if (xhttp_need_expect(pb->http))
	{
		xhttp_send_continue(pb->http);
	}

	pd = (plot_block_t*)xmem_alloc(sizeof(plot_block_t));

	ptr_prop = create_proper_doc();

	xsprintf(token, _T("%s/plot.ini"), pb->path);

	if (!load_proper_from_ini_file(ptr_prop, NULL, token))
	{
		raise_user_error(_T("-1"), _T("load loc config falied\n"));
	}

	read_proper(ptr_prop, _T("PLOT"), -1, _T("LOCATION"), -1, token, PATH_LEN);
	read_proper(ptr_prop, _T("PLOT"), -1, _T("PUBLICKEY"), -1, pd->sd.scr_uid, KEY_LEN);
	read_proper(ptr_prop, _T("PLOT"), -1, _T("PRIVATEKEY"), -1, pd->sd.scr_key, KEY_LEN);

	destroy_proper_doc(ptr_prop);
	ptr_prop = NULL;

	printf_path(pd->path, token);

	rt = _invoke_ploting(pb, pd);

	xmem_free(pd);
	pd = NULL;

	END_CATCH;

	return (rt) ? HTTPS_INVOKE_SUCCEED : HTTPS_INVOKE_WITHINFO;

ONERROR:

	_invoke_error(pb, pd);

	if (ptr_prop)
		destroy_proper_doc(ptr_prop);

	xmem_free(pd);

	return HTTPS_INVOKE_WITHINFO;
}