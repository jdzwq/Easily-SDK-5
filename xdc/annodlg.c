/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc anno dialog document

	@module	annodlg.c | anno dialog widnows implement file

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

#include "xdcdlg.h"
#include "handler.h"
#include "winnc.h"
#include "xdcctrl.h"
#include "xdcbox.h"
#include "xdcmenu.h"

#define IDC_ANNODLG_PHOTO			10
#define IDC_ANNODLG_ICONBOX			11
#define IDC_ANNODLG_MENU_FONT		12
#define IDC_ANNODLG_MENU_COLOR		13
#define IDC_ANNODLG_PUSHBOX_OK		20
#define IDC_ANNODLG_PUSHBOX_COMMIT	21
#define IDC_ANNODLG_PUSHBOX_COLOR	22
#define IDC_ANNODLG_PUSHBOX_FONT	23

#define IDA_ANNODLG_ICON_RECT		1001
#define IDA_ANNODLG_ICON_ELLIPSE	1002
#define IDA_ANNODLG_ICON_CROSS		1003
#define IDA_ANNODLG_ICON_STAR		1004
#define IDA_ANNODLG_ICON_DIAMOND	1005

#define ANNODLG_BUTTON_HEIGHT		(float)8 //tm
#define ANNODLG_BUTTON_WIDTH		(float)12 //tm

#define IS_ANNO_ICON(token) ((compare_text(token,-1,ATTR_ICON_RECT,-1,1) == 0 || compare_text(token,-1,ATTR_ICON_ELLIPSE,-1,1) == 0 || compare_text(token,-1,ATTR_ICON_CROSS,-1,1) == 0 || compare_text(token,-1,ATTR_ICON_STAR,-1,1) == 0 ||compare_text(token,-1,ATTR_ICON_DIAMOND,-1,1) == 0)? 1 : 0)

typedef struct _annodlg_delta_t{
	res_win_t photo;
	string_t varimg;
}annodlg_delta_t;

#define GETANNODLGDELTA(ph) 	(annodlg_delta_t*)widget_get_user_delta(ph)
#define SETANNODLGDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/************************************************************************************/
void annodlg_on_ok(res_win_t widget)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	byte_t* buf_zip = NULL;
	byte_t* buf_bmp = NULL;
	dword_t len_zip, len_bmp;
	tchar_t* buf_bas = NULL;
	int len_bas, tlen;

	photoctrl_commit(ptd->photo);

	len_bmp = photoctrl_get_bitmap(ptd->photo, NULL, MAX_LONG);
	buf_bmp = (byte_t*)xmem_alloc(len_bmp);
	photoctrl_get_bitmap(ptd->photo, buf_bmp, len_bmp);

	if (xsnicmp(GDI_ATTR_IMAGE_TYPE_JPG, varstr_ptr(ptd->varimg), xslen(GDI_ATTR_IMAGE_TYPE_JPG)) == 0)
	{
		len_zip = len_bmp;
		buf_zip = (byte_t*)xmem_alloc(len_bmp);
		len_zip = xjpg_compress(buf_bmp, len_bmp, buf_zip, len_zip);

		len_bas = xbas_encode(buf_zip, len_zip, NULL, MAX_LONG);
		tlen = xslen(GDI_ATTR_IMAGE_TYPE_JPG);

		buf_bas = xsalloc(len_bas + tlen + 1);
		xscpy(buf_bas, GDI_ATTR_IMAGE_TYPE_JPG);
		xbas_encode(buf_zip, len_zip, buf_bas + tlen, len_bas);

		xmem_free(buf_zip);

		varstr_cpy(ptd->varimg, buf_bas, tlen + len_bas);

		xmem_free(buf_bas);
	}
	if (xsnicmp(GDI_ATTR_IMAGE_TYPE_PNG, varstr_ptr(ptd->varimg), xslen(GDI_ATTR_IMAGE_TYPE_PNG)) == 0)
	{
		len_zip = len_bmp;
		buf_zip = (byte_t*)xmem_alloc(len_bmp);
		len_zip = xpng_compress(buf_bmp, len_bmp, buf_zip, len_zip);

		len_bas = xbas_encode(buf_zip, len_zip, NULL, MAX_LONG);
		tlen = xslen(GDI_ATTR_IMAGE_TYPE_PNG);

		buf_bas = xsalloc(len_bas + tlen + 1);
		xscpy(buf_bas, GDI_ATTR_IMAGE_TYPE_PNG);
		xbas_encode(buf_zip, len_zip, buf_bas + tlen, len_bas);

		xmem_free(buf_zip);

		varstr_cpy(ptd->varimg, buf_bas, tlen + len_bas);

		xmem_free(buf_bas);
	}
	if (xsnicmp(GDI_ATTR_IMAGE_TYPE_BMP, varstr_ptr(ptd->varimg), xslen(GDI_ATTR_IMAGE_TYPE_BMP)) == 0)
	{
		len_bas = xbas_encode(buf_bmp, len_bmp, NULL, MAX_LONG);
		tlen = xslen(GDI_ATTR_IMAGE_TYPE_BMP);

		buf_bas = xsalloc(len_bas + tlen + 1);
		xscpy(buf_bas, GDI_ATTR_IMAGE_TYPE_BMP);
		xbas_encode(buf_bmp, len_bmp, buf_bas + tlen, len_bas);

		varstr_cpy(ptd->varimg, buf_bas, tlen + len_bas);

		xmem_free(buf_bas);
	}

	xmem_free(buf_bmp);

	widget_close(widget, 1);
}

void annodlg_on_commit(res_win_t widget)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);

	photoctrl_commit(ptd->photo);
}

void annodlg_on_show_color(res_win_t widget, const xrect_t* pxr)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	xpoint_t pt;

	if(!photoctrl_get_focus_spot(ptd->photo))
		return;

	pt.x = pxr->x;
	pt.y = pxr->y;
	color_menu(widget, IDC_ANNODLG_MENU_COLOR, &pt, WD_LAYOUT_RIGHTTOP);
}

void annodlg_on_select_color(res_win_t widget, const tchar_t* mid)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	link_t_ptr ilk;
	const tchar_t* org_style;
	tchar_t* new_style;
	int len;

	ilk = photoctrl_get_focus_spot(ptd->photo);
	if (!ilk)
		return;

	org_style = get_anno_spot_style_ptr(ilk);

	len = write_style_attr(org_style, -1, GDI_ATTR_FONT_COLOR, -1, mid, -1, NULL, MAX_LONG);
	new_style = xsalloc(len + 1);
	write_style_attr(org_style, -1, GDI_ATTR_FONT_COLOR, -1, mid, -1, new_style, len);

	set_anno_spot_style(ilk, new_style);

	xsfree(new_style);

	photoctrl_redraw(ptd->photo);
}

void annodlg_on_show_font(res_win_t widget, const xrect_t* pxr)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	xpoint_t pt;

	if (!photoctrl_get_focus_spot(ptd->photo))
		return;

	pt.x = pxr->x;
	pt.y = pxr->y;
	fontsize_menu(widget, IDC_ANNODLG_MENU_COLOR, &pt, WD_LAYOUT_RIGHTTOP);
}

void annodlg_on_select_font(res_win_t widget, const tchar_t* mid)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	link_t_ptr ilk;
	const tchar_t* org_style;
	tchar_t* new_style;
	int len;

	ilk = photoctrl_get_focus_spot(ptd->photo);
	if (!ilk)
		return;

	org_style = get_anno_spot_style_ptr(ilk);

	len = write_style_attr(org_style, -1, GDI_ATTR_FONT_SIZE, -1, mid, -1, NULL, MAX_LONG);
	new_style = xsalloc(len + 1);
	write_style_attr(org_style, -1, GDI_ATTR_FONT_SIZE, -1, mid, -1, new_style, len);

	set_anno_spot_style(ilk, new_style);

	xsfree(new_style);

	photoctrl_redraw(ptd->photo);
}

void annodlg_on_append_item(res_win_t widget, const tchar_t* mid)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	link_t_ptr ptr, ilk;
	xpoint_t pt[2] = { 0 };

	pt[0].x = 0;
	pt[0].y = 0;
	pt[1].x = 20;
	pt[2].y = 20;

	ptr = photoctrl_fetch(ptd->photo);
	ilk = insert_anno_spot(ptr, LINK_LAST);
	set_anno_spot_type(ilk, mid);
	set_anno_spot_xpoint(ilk, pt, 2);

	photoctrl_redraw(ptd->photo);
}

/**********************************************************************************/
int hand_annodlg_create(res_win_t widget, void* data)
{
	annodlg_delta_t* ptd;

	xrect_t xr;
	xsize_t xs;
	res_win_t iconbox, pushbox;
	long nBar, nSplit;
	tchar_t icons[1024] = { 0 };

	byte_t* buf_zip = NULL;
	byte_t* buf_bmp = NULL;
	int len_zip, len_bmp;

	widget_hand_create(widget);

	ptd = (annodlg_delta_t*)xmem_alloc(sizeof(annodlg_delta_t));
	xmem_zero((void*)ptd, sizeof(annodlg_delta_t));

	ptd->varimg = (string_t)data;

	xs.fx = ZERO_WIDTH;
	xs.fy = DEF_TOUCH_SPAN;
	widget_size_to_pt(widget, &xs);
	nBar = xs.cy;

	widget_get_client_rect(widget, &xr);
	xr.h = nBar;

	iconbox = iconbox_create(widget, WD_STYLE_CONTROL, &xr);
	widget_set_owner(iconbox, widget);
	widget_set_user_id(iconbox, IDC_ANNODLG_ICONBOX);

	xsprintf(icons, _T("%d~%s;%d~%s;%d~%s;%d~%s;%d~%s;"),
		IDA_ANNODLG_ICON_RECT, ATTR_ICON_RECT,
		IDA_ANNODLG_ICON_ELLIPSE, ATTR_ICON_ELLIPSE,
		IDA_ANNODLG_ICON_CROSS, ATTR_ICON_CROSS,
		IDA_ANNODLG_ICON_STAR, ATTR_ICON_STAR,
		IDA_ANNODLG_ICON_DIAMOND, ATTR_ICON_DIAMOND);

	iconbox_set_options(iconbox, icons, -1);
	widget_show(iconbox, WD_SHOW_NORMAL);

	xs.fx = ANNODLG_BUTTON_WIDTH;
	xs.fy = ANNODLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.y += nBar;
	xr.h -= (xs.cy + nBar);

	ptd->photo = photoctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_owner(ptd->photo, widget);
	widget_set_user_id(ptd->photo, IDC_ANNODLG_PHOTO);

	if (ptd->varimg)
	{
		if (xsnicmp(GDI_ATTR_IMAGE_TYPE_JPG, varstr_ptr(ptd->varimg), xslen(GDI_ATTR_IMAGE_TYPE_JPG)) == 0)
		{
			len_zip = xbas_decode(varstr_ptr(ptd->varimg), varstr_len(ptd->varimg), NULL, MAX_LONG);
			buf_zip = (byte_t*)xmem_alloc(len_zip);
			xbas_decode(varstr_ptr(ptd->varimg), varstr_len(ptd->varimg), buf_zip, len_zip);

			len_bmp = xjpg_decompress(buf_zip, len_zip, NULL, MAX_LONG);
			buf_bmp = (byte_t*)xmem_alloc(len_bmp);
			xjpg_decompress(buf_zip, len_zip, buf_bmp, len_bmp);

			xmem_free(buf_zip);

			photoctrl_set_bitmap(ptd->photo, buf_bmp, len_bmp);

			xmem_free(buf_bmp);
		}
		if (xsnicmp(GDI_ATTR_IMAGE_TYPE_PNG, varstr_ptr(ptd->varimg), xslen(GDI_ATTR_IMAGE_TYPE_PNG)) == 0)
		{
			len_zip = xbas_decode(varstr_ptr(ptd->varimg), varstr_len(ptd->varimg), NULL, MAX_LONG);
			buf_zip = (byte_t*)xmem_alloc(len_zip);
			xbas_decode(varstr_ptr(ptd->varimg), varstr_len(ptd->varimg), buf_zip, len_zip);

			len_bmp = xpng_decompress(buf_zip, len_zip, NULL, MAX_LONG);
			buf_bmp = (byte_t*)xmem_alloc(len_bmp);
			xpng_decompress(buf_zip, len_zip, buf_bmp, len_bmp);

			xmem_free(buf_zip);

			photoctrl_set_bitmap(ptd->photo, buf_bmp, len_bmp);

			xmem_free(buf_bmp);
		}
		if (xsnicmp(GDI_ATTR_IMAGE_TYPE_BMP, varstr_ptr(ptd->varimg), xslen(GDI_ATTR_IMAGE_TYPE_BMP)) == 0)
		{
			len_bmp = xbas_decode(varstr_ptr(ptd->varimg), varstr_len(ptd->varimg), NULL, MAX_LONG);
			buf_bmp = (byte_t*)xmem_alloc(len_bmp);
			xbas_decode(varstr_ptr(ptd->varimg), varstr_len(ptd->varimg), buf_bmp, len_bmp);

			photoctrl_set_bitmap(ptd->photo, buf_bmp, len_bmp);

			xmem_free(buf_bmp);
		}
	}

	widget_show(ptd->photo, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;
	xr.x = xr.x + xr.w - xs.cx;
	xr.w = xs.cx;

	xs.fx = DEF_SPLIT_FEED;
	xs.fy = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);
	nSplit = xs.cx;

	pt_expand_rect(&xr, -xs.cx, -xs.cy);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_ANNODLG_PUSHBOX_OK);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, ANNODLG_PUSHBOX_OK, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x -= (xr.w + nSplit);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_ANNODLG_PUSHBOX_COMMIT);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, ANNODLG_PUSHBOX_COMMIT, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xs.fx = ANNODLG_BUTTON_WIDTH;
	xs.fy = ANNODLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;
	xr.w = xs.cx;

	pt_expand_rect(&xr, -xs.cx, -xs.cy);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_ANNODLG_PUSHBOX_FONT);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, ANNODLG_PUSHBOX_FONTSIZE, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	xr.x += (xr.w + nSplit);

	pushbox = pushbox_create(widget, WD_STYLE_CONTROL | WD_PUSHBOX_TEXT, &xr);
	widget_set_user_id(pushbox, IDC_ANNODLG_PUSHBOX_COLOR);
	widget_set_owner(pushbox, widget);
	pushbox_set_text(pushbox, ANNODLG_PUSHBOX_FONTCOLOR, -1);
	widget_show(pushbox, WD_SHOW_NORMAL);

	SETANNODLGDELTA(widget, ptd);

	widget_set_focus(ptd->photo);

	return 0;
}

void hand_annodlg_destroy(res_win_t widget)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	res_win_t ctrl;

	XDL_ASSERT(ptd != NULL);

	ctrl = widget_get_child(widget, IDC_ANNODLG_ICONBOX);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PHOTO);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_OK);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_COMMIT);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_FONT);
	if (ctrl)
		widget_destroy(ctrl);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_COLOR);
	if (ctrl)
		widget_destroy(ctrl);

	xmem_free(ptd);

	SETANNODLGDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_annodlg_size(res_win_t widget, int code, const xsize_t* prs)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	xsize_t xs;
	xrect_t xr;
	res_win_t ctrl;
	int nBar, nSplit;

	xs.fx = ZERO_WIDTH;
	xs.fy = DEF_TOUCH_SPAN;
	widget_size_to_pt(widget, &xs);
	nBar = xs.cy;

	widget_get_client_rect(widget, &xr);
	xr.h = nBar;

	ctrl = widget_get_child(widget, IDC_ANNODLG_ICONBOX);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
	}

	xs.fx = ANNODLG_BUTTON_WIDTH;
	xs.fy = ANNODLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.y += nBar;
	xr.h -= (xs.cy + nBar);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PHOTO);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
	}

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;
	xr.x = xr.x + xr.w - xs.cx;
	xr.w = xs.cx;

	xs.fx = DEF_SPLIT_FEED;
	xs.fy = DEF_SPLIT_FEED;
	widget_size_to_pt(widget, &xs);
	nSplit = xs.cx;

	pt_expand_rect(&xr, -nSplit, -nSplit);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_OK);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
	}

	xr.x -= (xr.w + nSplit);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_COMMIT);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
	}

	xs.fx = ANNODLG_BUTTON_WIDTH;
	xs.fy = ANNODLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_client_rect(widget, &xr);
	xr.y = xr.y + xr.h - xs.cy;
	xr.h = xs.cy;
	xr.w = xs.cx;

	pt_expand_rect(&xr, -nSplit, -nSplit);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_FONT);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
	}

	xr.x += (xr.w + nSplit);

	ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_COLOR);
	if (widget_is_valid(ctrl))
	{
		widget_move(ctrl, RECTPOINT(&xr));
		widget_size(ctrl, RECTSIZE(&xr));
	}

	widget_update(widget, NULL, 0);
}

void hand_annodlg_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	xrect_t xr;
	res_win_t ctrl;

	if (!ptd->varimg)
		return;

	if (cid == IDC_ANNODLG_PUSHBOX_OK)
	{
		annodlg_on_ok(widget);
	}
	else if (cid == IDC_ANNODLG_PUSHBOX_COMMIT)
	{
		annodlg_on_commit(widget);
	}
	else if (cid == IDC_ANNODLG_PUSHBOX_FONT)
	{
		ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_FONT);
		widget_get_window_rect(ctrl, &xr);
		annodlg_on_show_font(widget, &xr);
	}
	else if (cid == IDC_ANNODLG_PUSHBOX_COLOR)
	{
		ctrl = widget_get_child(widget, IDC_ANNODLG_PUSHBOX_FONT);
		widget_get_window_rect(ctrl, &xr);
		annodlg_on_show_color(widget, &xr);
	}
	else if (cid == IDC_ANNODLG_ICONBOX)
	{
		switch (code)
		{
		case IDA_ANNODLG_ICON_CROSS:
			annodlg_on_append_item(widget, ATTR_ICON_CROSS);
			break;
		case IDA_ANNODLG_ICON_DIAMOND:
			annodlg_on_append_item(widget, ATTR_ICON_DIAMOND);
			break;
		case IDA_ANNODLG_ICON_ELLIPSE:
			annodlg_on_append_item(widget, ATTR_ICON_ELLIPSE);
			break;
		case IDA_ANNODLG_ICON_RECT:
			annodlg_on_append_item(widget, ATTR_ICON_RECT);
			break;
		case IDA_ANNODLG_ICON_STAR:
			annodlg_on_append_item(widget, ATTR_ICON_STAR);
			break;
		}
	}else if (cid == IDC_ANNODLG_MENU_COLOR)
	{
		//annodlg_on_select_color(widget, code);

		widget_close((res_win_t)data, 1);
	}
	else if (cid == IDC_ANNODLG_MENU_FONT)
	{
		//annodlg_on_select_font(widget, code);

		widget_close((res_win_t)data, 1);
	}
}

void hand_annodlg_notice(res_win_t widget, NOTICE* pnt)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);

	NOTICE_PHOTO* pnp;

	if (!ptd->photo)
		return;

	if (pnt->id == IDC_ANNODLG_PHOTO)
	{
		pnp = (NOTICE_PHOTO*)pnt;
		switch (pnp->code)
		{
		case NC_PHOTOFACEDRAW:
			break;
		}
	}
	
}

void hand_annodlg_erase(res_win_t widget, res_ctx_t dc)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
}

void hand_annodlg_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	annodlg_delta_t* ptd = GETANNODLGDELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xgradi_t xg = { 0 };
	xrect_t xr,xr_bar;
	xsize_t xs;
	canvas_t canv;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	xs.fx = ANNODLG_BUTTON_WIDTH;
	xs.fy = ANNODLG_BUTTON_HEIGHT;
	widget_size_to_pt(widget, &xs);

	xr_bar.x = xr.x;
	xr_bar.y = xr.y + xr.h - xs.cy;
	xr_bar.w = xr.w;
	xr_bar.h = xs.cy;

	xscpy(xg.brim_color, xb.color);
	xscpy(xg.core_color, xb.color);
	lighten_xgradi(&xg, DEF_MIDD_DARKEN);
	xscpy(xg.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	gradient_rect_raw(rdc, &xg, &xr_bar);

	end_canvas_paint(canv, dc, pxr);
}


/***************************************************************************************/
res_win_t annodlg_create(const tchar_t* title, string_t var, res_win_t owner)
{
	if_event_t ev = { 0 };
	clr_mod_t clr = { 0 };
	xrect_t xr = { 0 };
	xsize_t xs = { 0 };
	res_win_t dlg;

	ev.param = (void*)var;

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_annodlg_create)
		EVENT_ON_DESTROY(hand_annodlg_destroy)

		EVENT_ON_ERASE(hand_annodlg_erase)
		EVENT_ON_PAINT(hand_annodlg_paint)

		EVENT_ON_SIZE(hand_annodlg_size)

		EVENT_ON_NOTICE(hand_annodlg_notice)
		EVENT_ON_MENU_COMMAND(hand_annodlg_menu_command)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	dlg = widget_create(NULL, WD_STYLE_DIALOG, &xr, owner, &ev);	
	if (!dlg)
		return NULL;

	widget_set_owner(dlg, owner);
	widget_set_user_id(dlg, IDC_ANNODLG);
	widget_set_title(dlg, title);

	get_desktop_size(&xs);

	xs.cx = xs.cx / 3;
	xs.cy = xs.cy / 3;
	widget_adjust_size(WD_STYLE_DIALOG, &xs);

	widget_size(dlg, &xs);
	widget_center_window(dlg, owner);

	if (widget_is_valid(owner))
	{
		widget_get_color_mode(owner, &clr);
		widget_set_color_mode(dlg, &clr);
	}

	return dlg;
}


