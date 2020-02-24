/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc image box document

	@module	photoctrl.c | image control widnows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#include "xdcctrl.h"
#include "xdcinit.h"
#include "xdcutil.h"
#include "xdcwidg.h"
#include "xdcgdi.h"
#include "xdccanv.h"
#include "xdcres.h"
#include "xdcicon.h"
#include "xdcclip.h"


typedef struct _photo_delta_t{
	int opera;
	short org_x, org_y;
	res_bmp_t bmp;
	tchar_t type[RES_LEN];

	LINKPTR list;
	LINKPTR stack;
}photo_delta_t;


#define GETPHOTODELTA(ph) 	(photo_delta_t*)widget_get_user_delta(ph)
#define SETPHOTODELTA(ph,ptd) widget_set_user_delta(ph,(void*)ptd)

/*****************************************************************************/
int noti_photo_owner(res_win_t widget, unsigned long code, void* data, res_dc_t rdc)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	NOTICE_PHOTO nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.rdc = rdc;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}
/*****************************************************************************/
void _photoctrl_save(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd->bmp)
		return;

	if (!ptd->stack)
		ptd->stack = create_stack_table();

	push_stack_node(ptd->stack, (void*)ptd->bmp);
	ptd->bmp = NULL;
}

void _photoctrl_undo(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_bmp_t bmp;

	bmp = (ptd->stack) ? (res_bmp_t)pop_stack_node(ptd->stack) : NULL;

	if (!bmp)
		return;

	if (ptd->bmp)
		delete_bitmap(ptd->bmp);

	ptd->bmp = bmp;
}

void _photoctrl_clear(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_bmp_t bmp;

	if (ptd->stack)
	{
		while (bmp = (res_bmp_t)pop_stack_node(ptd->stack))
			delete_bitmap(bmp);

		destroy_stack_table(ptd->stack);
		ptd->stack = NULL;
	}
}

void _photoctrl_merge_draw(res_win_t widget, long x, long y)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	res_dc_t rdc, memdc;
	res_bmp_t obj, membm;
	xcolor_t xc = { 0 };
	xbrush_t xb = { 0 };
	xrect_t xr = { 0 };
	viewbox_t vb = { 0 };
	xsize_t xs;
	xpoint_t pt;

	widget_get_view_rect(widget, &vb);

	pt.fx = ZERO_WIDTH;
	pt.fy = ZERO_HEIGHT;
	widget_point_to_pt(widget, &pt);

	pt.x = x - pt.x;
	pt.y = y - pt.y;

	rdc = widget_client_dc(widget);

	memdc = create_compatible_context(rdc);

	get_bitmap_size(ptd->bmp, &xs.cx, &xs.cy);

	membm = create_compatible_bitmap(rdc, xs.cx, xs.cy);

	widget_release_dc(widget, rdc);

	obj = (res_bmp_t)select_object(memdc, (res_obj_t)membm);

	xr.w = xs.cx;
	xr.h = xs.cy;

	widget_get_xbrush(widget, &xb);
	draw_rect_raw(memdc, NULL, &xb, &xr);

	draw_bitmap_raw(memdc, ptd->bmp, &xr);

	noti_photo_owner(widget, NC_PHOTODRAWMASK, (void*)&pt, memdc);

	membm = (res_bmp_t)select_object(memdc, (res_obj_t)obj);

	delete_context(memdc);

	_photoctrl_save(widget);

	ptd->bmp = membm;
}

static void _photoctrl_reset_page(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	long pw, ph, fw, fh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	if (ptd->bmp)
	{
		get_bitmap_size(ptd->bmp, &xs.cx, &xs.cy);
		fw = xs.cx;
		fh = xs.cy;
	}
	else
	{
		fw = pw;
		fh = ph;
	}

	widget_reset_paging(widget, pw, ph, fw, fh, 10, 10);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

/*****************************************************************************/
int hand_photo_create(res_win_t widget, void* data)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	res_dc_t rdc;
	XHANDLE canv;

	ptd = (photo_delta_t*)xmem_alloc(sizeof(photo_delta_t));
	xmem_zero((void*)ptd, sizeof(photo_delta_t));

	rdc = widget_client_dc(widget);
	canv = create_display_canvas(rdc);
	widget_release_dc(widget, rdc);

	widget_attach_canvas(widget, canv);

	SETPHOTODELTA(widget, ptd);

	return 0;
}

void hand_photo_destroy(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	if (ptd->bmp)
		delete_bitmap(ptd->bmp);

	_photoctrl_clear(widget);

	destroy_display_canvas(widget_detach_canvas(widget));

	SETPHOTODELTA(widget, 0);
	xmem_free(ptd);
}

void hand_photo_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	if (ptd->opera)
		widget_set_cursor(widget, CURS_HAND);
	else
		widget_set_cursor(widget, CURS_ARROW);
}

void hand_photo_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
		widget_set_capture(widget, 1);
	}
}

void hand_photo_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	widget_set_capture(widget, 0);

	if (ptd->opera)
	{
		if (ptd->bmp)
		{
			_photoctrl_merge_draw(widget, pxp->x, pxp->y);
		}
	}

	if (ptd->opera)
	{
		ptd->opera = 0;
		widget_invalid(widget, NULL, 0);
	}

	noti_photo_owner(widget, NC_PHOTOLBCLK, (void*)pxp, NULL);
}

void hand_photo_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	noti_photo_owner(widget, NC_PHOTODBCLK, (void*)pxp, NULL);
}

void hand_photo_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
}

void hand_photo_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	noti_photo_owner(widget, NC_PHOTORBCLK, (void*)pxp, NULL);
}

void hand_photo_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_photo_keydown(res_win_t widget, int key)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if ((key == _T('z') || key == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
	{
		widget_undo(widget);
	}else if ((key == _T('c') || key == _T('C')) && widget_key_state(widget, KEY_CONTROL))
	{
		widget_copy(widget);
	}
	else if ((key == _T('x') || key == _T('X')) && widget_key_state(widget, KEY_CONTROL))
	{
		widget_cut(widget);
	}
	else if ((key == _T('v') || key == _T('V')) && widget_key_state(widget, KEY_CONTROL))
	{
		widget_paste(widget);
	}
	else if (key == KEY_ESC)
	{
		ptd->opera = 0;
	}
}

void hand_photo_set_focus(res_win_t widget, res_win_t wt)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;
}

void hand_photo_kill_focus(res_win_t widget, res_win_t wt)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	if (widget_is_editor(widget))
	{
		widget_send_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, NULL);
	}
}

void hand_photo_copy(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	photoctrl_copy(widget);
}

void hand_photo_cut(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	photoctrl_cut(widget);
}

void hand_photo_paste(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	photoctrl_paste(widget);
}

void hand_photo_undo(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	photoctrl_undo(widget);
}

void hand_photo_command_color(res_win_t widget, void* data)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	widget_set_color_mode(widget, (clr_mod_t*)data);

	widget_update_window(widget);
	widget_update_client(widget);
}

void hand_photo_command_switch(res_win_t widget, bool_t bkey)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	dword_t ws;

	ws = widget_get_style(widget);

	if (bkey)
		ws |= (WD_STYLE_TOUCH);
	else
		ws &= ~(WD_STYLE_TOUCH);

	widget_set_style(widget, ws);

	widget_update_window(widget);
}

void hand_photo_size(res_win_t widget, int code, const xsize_t* prs)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	_photoctrl_reset_page(widget);

	widget_invalid(widget, NULL, 0);
}

void hand_photo_erase(res_win_t widget, res_dc_t dc)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
}

void hand_photo_paint(res_win_t widget, res_dc_t dc, const xrect_t* pxr)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_dc_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xrect_t xr;

	XHANDLE canv;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);

	rdc = begin_display_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	if (ptd->bmp)
	{
		widget_get_view_rect(widget, (viewbox_t*)&xr);

		draw_bitmap_raw(rdc, ptd->bmp, &xr);
	}

	end_display_paint(canv, dc, pxr);
}

/***************************************************************************************/
res_win_t photoctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	ev.pf_on_create = hand_photo_create;
	ev.pf_on_destroy = hand_photo_destroy;

	ev.pf_on_mouse_move = hand_photo_mouse_move;
	ev.pf_on_lbutton_down = hand_photo_lbutton_down;
	ev.pf_on_lbutton_up = hand_photo_lbutton_up;
	ev.pf_on_rbutton_down = hand_photo_rbutton_down;
	ev.pf_on_rbutton_up = hand_photo_rbutton_up;
	ev.pf_on_lbutton_dbclick = hand_photo_lbutton_dbclick;

	ev.pf_on_scroll = hand_photo_scroll;

	ev.pf_on_keydown = hand_photo_keydown;
	ev.pf_on_set_focus = hand_photo_set_focus;
	ev.pf_on_kill_focus = hand_photo_kill_focus;

	ev.pf_on_erase = hand_photo_erase;
	ev.pf_on_paint = hand_photo_paint;

	ev.pf_on_size = hand_photo_size;
	ev.pf_on_command_color = hand_photo_command_color;
	ev.pf_on_command_switch = hand_photo_command_switch;

	ev.pf_on_copy = hand_photo_copy;
	ev.pf_on_cut = hand_photo_cut;
	ev.pf_on_paste = hand_photo_paste;
	ev.pf_on_undo = hand_photo_undo;

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void photoctrl_set_data(res_win_t widget, const tchar_t* data)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_dc_t rdc;
	ximage_t xi = { 0 };
	byte_t* buf_zip = NULL;
	byte_t* buf_bmp = NULL;
	int len_zip,len_bmp;

	XDC_ASSERT(ptd != NULL);

	_photoctrl_clear(widget);

	if (ptd->bmp)
	{
		delete_bitmap(ptd->bmp);
		ptd->bmp = NULL;
	}

	rdc = widget_client_dc(widget);

	parse_ximage_from_source(&xi, data);

	if (compare_text(xi.type, -1, ATTR_IMAGE_TYPE_JPG, -1, 1) == 0)
	{
		len_zip = base64_decode(xi.source, -1, NULL, MAX_LONG);
		buf_zip = (byte_t*)xmem_alloc(len_zip);
		base64_decode(xi.source, -1, buf_zip, len_zip);

		len_bmp = xjpg_decompress(buf_zip, len_zip, NULL, MAX_LONG);
		buf_bmp = (byte_t*)xmem_alloc(len_bmp);
		xjpg_decompress(buf_zip, len_zip, buf_bmp, len_bmp);

		xmem_free(buf_zip);

		ptd->bmp = load_bitmap_from_bytes(rdc, buf_bmp, len_bmp);

		xmem_free(buf_bmp);
	}
	else if (compare_text(xi.type, -1, ATTR_IMAGE_TYPE_PNG, -1, 1) == 0)
	{
		len_zip = base64_decode(xi.source, -1, NULL, MAX_LONG);
		buf_zip = (byte_t*)xmem_alloc(len_zip);
		base64_decode(xi.source, -1, buf_zip, len_zip);

		len_bmp = xpng_decompress(buf_zip, len_zip, NULL, MAX_LONG);
		buf_bmp = (byte_t*)xmem_alloc(len_bmp);
		xpng_decompress(buf_zip, len_zip, buf_bmp, len_bmp);

		xmem_free(buf_zip);

		ptd->bmp = load_bitmap_from_bytes(rdc, buf_bmp, len_bmp);

		xmem_free(buf_bmp);
	}
	else if (compare_text(xi.type, -1, ATTR_IMAGE_TYPE_BMP, -1, 1) == 0)
	{
		len_bmp = base64_decode(xi.source, -1, NULL, MAX_LONG);
		buf_bmp = (byte_t*)xmem_alloc(len_bmp);
		base64_decode(xi.source, -1, buf_bmp, len_bmp);

		ptd->bmp = load_bitmap_from_bytes(rdc, buf_bmp, len_bmp);

		xmem_free(buf_bmp);
	}

	xscpy(ptd->type, xi.type);

	widget_release_dc(widget, rdc);

	_photoctrl_reset_page(widget);

	widget_invalid(widget, NULL, 0);
}

int photoctrl_get_data(res_win_t widget, tchar_t* buf, int max)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	int len,tlen;
	res_dc_t rdc;
	byte_t* buf_zip = NULL;
	byte_t* buf_bmp = NULL;
	int len_zip, len_bmp;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->bmp)
		return 0;

	len_bmp = get_bitmap_bytes(ptd->bmp);
	buf_bmp = (byte_t*)xmem_alloc(len_bmp);

	rdc = widget_client_dc(widget);

	save_bitmap_to_bytes(rdc, ptd->bmp, buf_bmp, len_bmp);

	widget_release_dc(widget, rdc);

	if (compare_text(ptd->type, -1, ATTR_IMAGE_TYPE_JPG, -1, 1) == 0)
	{
		len_zip = len_bmp;
		buf_zip = (byte_t*)xmem_alloc(len_bmp);
		len_zip = xjpg_compress(buf_bmp, len_bmp, buf_zip, len_zip);

		xmem_free(buf_bmp);

		len = base64_encode(buf_zip, len_zip, NULL, MAX_LONG);
		tlen = xslen(ptd->type);

		max = (max < len + tlen) ? max : (len + tlen);

		if (buf)
		{
			xscpy(buf, ptd->type);
			base64_encode(buf_zip, len_zip, buf + tlen, max - tlen);
		}
		
		xmem_free(buf_zip);
	}
	else if (compare_text(ptd->type, -1, ATTR_IMAGE_TYPE_PNG, -1, 1) == 0)
	{
		len_zip = len_bmp;
		buf_zip = (byte_t*)xmem_alloc(len_bmp);
		len_zip = xpng_compress(buf_bmp, len_bmp, buf_zip, len_zip);

		xmem_free(buf_bmp);

		len = base64_encode(buf_zip, len_zip, NULL, MAX_LONG);
		tlen = xslen(ptd->type);

		max = (max < len + tlen) ? max : (len + tlen);

		if (buf)
		{
			xscpy(buf, ptd->type);
			base64_encode(buf_zip, len_zip, buf + tlen, max - tlen);
		}

		xmem_free(buf_zip);
	}
	else if (compare_text(ptd->type, -1, ATTR_IMAGE_TYPE_BMP, -1, 1) == 0)
	{
		len = base64_encode(buf_bmp, len_bmp, NULL, MAX_LONG);
		tlen = xslen(ptd->type);

		max = (max < len + tlen) ? max : (len + tlen);

		if (buf)
		{
			xscpy(buf, ptd->type);
			base64_encode(buf_bmp, len_bmp, buf + tlen, max - tlen);
		}

		xmem_free(buf_bmp);
	}
	else
	{
		xmem_free(buf_bmp);
		max = 0;
	}

	return max;
}

void photoctrl_set_opera(res_win_t widget, int opera)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDC_ASSERT(ptd != NULL);

	ptd->opera = opera;
}

int photoctrl_get_opera(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDC_ASSERT(ptd != NULL);

	return ptd->opera;
}

bool_t photoctrl_can_undo(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	void* p;

	XDC_ASSERT(ptd != NULL);

	p = (ptd->stack) ? peek_stack_node(ptd->stack) : NULL;

	return (p) ? 1 : 0;
}

void photoctrl_undo(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDC_ASSERT(ptd != NULL);

	_photoctrl_undo(widget);
	ptd->opera = 0;
	widget_invalid(widget, NULL, 0);
}

bool_t photoctrl_copy(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_dc_t rdc;
	res_glob_t gb;
	int len;
	byte_t* buf;

	XDC_ASSERT(ptd != NULL);

	if (!ptd->bmp)
		return 0;

	if (!clipboard_open())
		return 0;

	len = get_bitmap_bytes(ptd->bmp);

	gb = system_alloc(len);
	buf = (byte_t*)system_lock(gb);

	rdc = widget_client_dc(widget);
	len = save_bitmap_to_bytes(rdc, ptd->bmp, buf, len);
	widget_release_dc(widget, rdc);

	system_unlock(gb);

	if (!clipboard_put(FMT_DIB, gb))
	{
		system_free(gb);
		clipboard_close();

		return 0;
	}

	clipboard_close();
	return 1;
}

bool_t photoctrl_cut(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDC_ASSERT(ptd != NULL);

	if (!ptd->bmp)
		return 0;

	if (!photoctrl_copy(widget))
		return 0;

	_photoctrl_save(widget);

	widget_invalid(widget, NULL, 0);

	return 1;
}

bool_t photoctrl_paste(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_dc_t rdc;
	res_glob_t gb;
	int len;
	byte_t* buf;

	XDC_ASSERT(ptd != NULL);

	if (!clipboard_open())
		return 0;

	gb = clipboard_get(FMT_DIB);
	if (!gb)
	{
		clipboard_close();
		return 0;
	}

	_photoctrl_save(widget);

	len = system_size(gb);
	buf = (byte_t*)system_lock(gb);

	rdc = widget_client_dc(widget);
	ptd->bmp = load_bitmap_from_bytes(rdc, buf, len);
	widget_release_dc(widget, rdc);

	system_unlock(gb);
	clipboard_clean();
	clipboard_close();

	widget_invalid(widget, NULL, 0);

	return 1;
}
