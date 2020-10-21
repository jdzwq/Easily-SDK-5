/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc image editor document

	@module	modelctrl.c | implement file

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

#include "xdcctrl.h"
#include "xdcimp.h"
#include "xdcfire.h"
#include "xdcbox.h"

#define BITMAP_BYTES(buf)		GET_DWORD(buf, 2)
#define BITMAP_WIDTH(buf)		GET_DWORD(BITMAPINFOHEADERPTR(buf), 4)
#define BITMAP_HEIGHT(buf)		GET_DWORD(BITMAPINFOHEADERPTR(buf), 8)


typedef struct _model_delta_t{
	int org_x, org_y;
	int index;

	bitmap_t bmp;

	res_win_t editor;
	res_win_t hsc;
	res_win_t vsc;

	bool_t b_drag;
	bool_t b_size;
	bool_t b_lock;

	link_t_ptr anno;
	link_t_ptr arti;

	link_t_ptr stack;
}model_delta_t;


#define GETMODELDELTA(ph) 	(model_delta_t*)widget_get_user_delta(ph)
#define SETMODELDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

int noti_model_owner(res_win_t widget, unsigned int code, link_t_ptr arti, void* data, visual_t rdc);
/*******************************************************************************************************/
static void _modelctrl_done(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->anno);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->anno, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->anno, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->anno, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->anno, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _modelctrl_undo(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->anno);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_anno_doc(ptd->anno);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->anno, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->anno, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		modelctrl_redraw(widget);
	}
}

static void _modelctrl_discard(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _modelctrl_clean(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _modelctrl_copy(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	res_glob_t gb;
	dword_t len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->anno);

	if (!get_anno_arti_selected_count(ptd->anno))
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_anno_doc();
	ilk = get_anno_next_arti(ptd->anno, LINK_FIRST);
	while (ilk)
	{
		if (get_anno_arti_selected(ilk))
		{
			nlk = insert_anno_arti(dom, LINK_LAST);
			copy_dom_node(nlk, ilk);
		}

		ilk = get_anno_next_arti(ptd->anno, ilk);
	}

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, DEF_MBS);
#endif

	gb = gmem_alloc(len + sizeof(tchar_t));
	buf = (byte_t*)gmem_lock(gb);

#ifdef _UNICODE
	format_dom_doc_to_bytes(dom, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(dom, buf, len, DEF_MBS);
#endif

	gmem_unlock(gb);

	if (!clipboard_put(DEF_CB_FORMAT, gb))
	{
		gmem_free(gb);
		clipboard_close();

		destroy_anno_doc(dom);

		return 0;
	}

	clipboard_close();

	destroy_anno_doc(dom);
	return 1;
}

static bool_t _modelctrl_cut(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->anno);

	if (!_modelctrl_copy(widget))
		return 0;

	ilk = get_anno_next_arti(ptd->anno, LINK_FIRST);
	while (ilk)
	{
		nxt = get_anno_next_arti(ptd->anno, ilk);

		if (get_anno_arti_selected(ilk))
		{
			delete_anno_arti(ilk);
		}
		ilk = nxt;
	}

	modelctrl_redraw(widget);

	return 1;
}

static bool_t _modelctrl_paste(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk;

	XDL_ASSERT(ptd && ptd->anno);

	if (!clipboard_open())
		return 0;

	gb = clipboard_get(DEF_CB_FORMAT);
	if (!gb)
	{
		clipboard_close();
		return 0;
	}

	len = gmem_size(gb);
	buf = (byte_t*)gmem_lock(gb);

	dom = create_dom_doc();
#ifdef _UNICODE
	if (!parse_dom_doc_from_bytes(dom, buf, len, DEF_UCS))
#else
	if (!parse_dom_doc_from_bytes(dom, buf, len, DEF_MBS))
#endif
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	if (!is_anno_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	while (nlk = get_anno_next_arti(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_anno_artiset(dom), nlk);
		attach_dom_node(get_anno_artiset(ptd->anno), LINK_LAST, nlk);
	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);
	return 1;
}

static void _modelctrl_arti_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	calc_anno_arti_rect(ptd->anno, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static bitmap_t _modelctrl_merge_anno(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	xcolor_t xc = { 0 };
	xbrush_t xb = { 0 };
	xrect_t xr = { 0 };
	xsize_t xs;
	link_t_ptr ilk;

	visual_t rdc, memdc;
	bitmap_t membm;
	if_visual_t* piv;

	if (!ptd->bmp && !ptd->anno)
		return NULL;

	rdc = widget_client_ctx(widget);

	get_bitmap_size(ptd->bmp, &xs.w, &xs.h);

	memdc = create_compatible_context(rdc, xs.w, xs.h);
	piv = create_visual_interface(memdc);

	xr.x = 0;
	xr.h = 0;
	xr.w = xs.w;
	xr.h = xs.h;

	widget_get_xbrush(widget, &xb);

	(*piv->pf_draw_rect_raw)(piv->visual, NULL, &xb, &xr);

	(*piv->pf_draw_bitmap_raw)(piv->visual, ptd->bmp, RECTPOINT(&xr));

	ilk = get_anno_next_arti(ptd->anno, LINK_FIRST);
	while (ilk)
	{
		_modelctrl_arti_rect(widget, ilk, &xr);

		//noti_model_owner(widget, NC_MODELANNODRAW, ilk, (void*)&xr, memdc);
	}

	destroy_visual_interface(piv);

	membm = create_context_bitmap(memdc);

	destroy_context(memdc);

	widget_release_ctx(widget, rdc);

	return membm;
}

static void _modelctrl_reset_page(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	int pw, ph, fw, fh;
	xrect_t xr;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	if (ptd->bmp)
	{

		get_bitmap_size(ptd->bmp, &fw, &fh);
	}
	else
	{
		fw = pw;
		fh = ph;
	}

	widget_reset_paging(widget, pw, ph, fw, fh, 20, 20);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}
/*******************************************************************************************************/
int noti_model_owner(res_win_t widget, unsigned int code, link_t_ptr arti, void* data, visual_t rdc)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	NOTICE_MODEL nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.anno = ptd->anno;
	nf.arti = arti;

	nf.data = data;
	nf.rdc = rdc;
	
	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_model_arti_drag(res_win_t widget, int x, int y)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->arti);

	widget_set_cursor(widget, CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_model_owner(widget, NC_MODELANNODRAG, ptd->arti, (void*)&pt, NULL);
}

void noti_model_arti_drop(res_win_t widget, int x, int y)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	xpoint_t pt;
	xrect_t xr,xr_org;
	xsize_t xs;
	xpoint_t* ppt;
	int i,count;

	XDL_ASSERT(ptd->arti);

	ptd->b_drag = 0;

	widget_set_cursor(widget, CURSOR_ARROW);

	xs.w = x - ptd->org_x;
	xs.h = y - ptd->org_y;

	if (!xs.w && !xs.h)
		return;

	_modelctrl_done(widget);

	_modelctrl_arti_rect(widget, ptd->arti, &xr_org);

	count = get_anno_arti_xpoint(ptd->arti, NULL, MAX_LONG);
	ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)*count);
	get_anno_arti_xpoint(ptd->arti, ppt, count);

	widget_size_to_tm(widget, &xs);

	for (i = 0; i < count; i++)
	{
		ppt[i].fx += xs.fw;
		ppt[i].fy += xs.fh;
	}
	
	set_anno_arti_xpoint(ptd->arti, ppt, count);
	xmem_free(ppt);

	_modelctrl_arti_rect(widget, ptd->arti, &xr);

	pt_merge_rect(&xr_org, &xr);
	pt_expand_rect(&xr_org, 100, 100);
	widget_erase(widget, &xr_org);

	pt.x = x;
	pt.y = y;
	noti_model_owner(widget, NC_MODELANNODROP, ptd->arti, (void*)&pt, NULL);
}

void noti_model_arti_sizing(res_win_t widget, int x, int y)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->arti);

	widget_set_cursor(widget, CURSOR_SIZEALL);

	_modelctrl_arti_rect(widget, ptd->arti, &xr);

	noti_model_owner(widget, NC_MODELANNOSIZING, ptd->arti, (void*)&xr, NULL);
}

void noti_model_arti_sized(res_win_t widget, int x, int y)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	xrect_t xr_org,xr;
	xsize_t xs;
	int count;
	xpoint_t* ppt;

	XDL_ASSERT(ptd->arti);

	ptd->b_size = 0;

	widget_set_cursor(widget, CURSOR_ARROW);

	xs.w = x - ptd->org_x;
	xs.h = y - ptd->org_y;

	if (!xs.w && !xs.h)
	{
		ptd->index = -1;
		return;
	}

	_modelctrl_done(widget);

	_modelctrl_arti_rect(widget, ptd->arti, &xr_org);

	count = get_anno_arti_xpoint(ptd->arti, NULL, MAX_LONG);
	ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)*count);
	get_anno_arti_xpoint(ptd->arti, ppt, count);

	widget_size_to_tm(widget, &xs);

	ppt[ptd->index].fx += xs.fw;
	ppt[ptd->index].fy += xs.fh;

	set_anno_arti_xpoint(ptd->arti, ppt, count);
	xmem_free(ppt);

	ptd->index = -1;

	_modelctrl_arti_rect(widget, ptd->arti, &xr);

	pt_merge_rect(&xr_org, &xr);
	pt_expand_rect(&xr_org, 100, 100);
	widget_erase(widget, &xr_org);

	noti_model_owner(widget, NC_MODELANNOSIZED, ptd->arti, (void*)&xr, NULL);
}

bool_t noti_model_arti_changing(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->arti);

	if (noti_model_owner(widget, NC_MODELANNOCHANGING, ptd->arti, NULL, NULL))
		return 0;

	_modelctrl_arti_rect(widget, ptd->arti, &xr);

	ptd->arti = NULL;

	pt_expand_rect(&xr, 100, 100);

	widget_erase(widget, &xr);

	return 1;
}

void noti_model_arti_changed(res_win_t widget, link_t_ptr ilk)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->arti);

	ptd->arti = ilk;

	_modelctrl_arti_rect(widget, ptd->arti, &xr);

	pt_expand_rect(&xr, 100, 100);

	widget_erase(widget, &xr);

	noti_model_owner(widget, NC_MODELANNOCHANGED, ptd->arti, NULL, NULL);
}

void noti_model_begin_edit(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	xrect_t xr = { 0 };

	clr_mod_t ob = { 0 };

	XDL_ASSERT(ptd->arti);
	XDL_ASSERT(!ptd->editor);

	if (ptd->b_lock)
		return;

	widget_get_color_mode(widget, &ob);

	_modelctrl_arti_rect(widget, ptd->arti, &xr);

	if (noti_model_owner(widget, NC_MODELANNOEDITING, ptd->arti, NULL, NULL))
		return;

	ptd->editor = fireedit_create(widget, &xr);
	XDL_ASSERT(ptd->editor);
	widget_set_user_id(ptd->editor, IDC_FIREEDIT);
	widget_set_owner(ptd->editor, widget);

	widget_set_color_mode(ptd->editor, &ob);
	widget_show(ptd->editor, WS_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	editbox_set_text(ptd->editor, get_anno_arti_text_ptr(ptd->arti));
	editbox_selectall(ptd->editor);
	editbox_auto_size(ptd->editor, 1);
}

void noti_model_commit_edit(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	const tchar_t* text;
	res_win_t editctrl;
	xrect_t xr;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->arti);

	text = (tchar_t*)editbox_get_text_ptr(ptd->editor);

	if (!noti_model_owner(widget, NC_MODELANNOCOMMIT, ptd->arti, NULL, NULL))
	{
		_modelctrl_done(widget);

		set_anno_arti_text(ptd->arti, text);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);

	_modelctrl_arti_rect(widget, ptd->arti, &xr);

	pt_expand_rect(&xr, 100, 100);

	widget_erase(widget, &xr);

	widget_set_focus(widget);
}

void noti_model_rollback_edit(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->arti);

	noti_model_owner(widget, NC_MODELANNOROLLBACK, ptd->arti, NULL, NULL);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_model_reset_editor(res_win_t widget, bool_t bCommit)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_model_commit_edit(widget);
		else
			noti_model_rollback_edit(widget);
	}
}
/*****************************************************************************/
int hand_model_create(res_win_t widget, void* data)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	widget_hand_create(widget);

	ptd = (model_delta_t*)xmem_alloc(sizeof(model_delta_t));
	xmem_zero((void*)ptd, sizeof(model_delta_t));

	SETMODELDELTA(widget, ptd);

	ptd->bmp = NULL;
	ptd->anno = NULL;
	ptd->stack = create_stack_table();

	return 0;
}

void hand_model_destroy(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_model_reset_editor(widget, 0);

	if (widget_is_valid(ptd->hsc))
		widget_destroy(ptd->hsc);

	if (widget_is_valid(ptd->vsc))
		widget_destroy(ptd->vsc);

	if (ptd->bmp)
		destroy_bitmap(ptd->bmp);

	_modelctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETMODELDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_model_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	int hint;
	int ind = -1;
	link_t_ptr ilk = NULL;
	xpoint_t pt;

	if (!ptd)
		return;

	if (ptd->b_drag)
	{
		noti_model_arti_drag(widget, pxp->x, pxp->y);
	}
	else if (ptd->b_size)
	{
		noti_model_arti_sizing(widget, pxp->x, pxp->y);
	}
	else
	{
		pt.x = pxp->x;
		pt.y = pxp->y;
		widget_point_to_tm(widget, &pt);

		hint = calc_anno_hint(&pt, ptd->anno, &ilk, &ind);

		if (hint == ANNO_HINT_SIZE && ilk == ptd->arti)
		{
			widget_set_cursor(widget, CURSOR_SIZEALL);
		}
		else if (hint == ANNO_HINT_ARTI && ilk == ptd->arti)
		{
			widget_set_cursor(widget, CURSOR_HAND);
		}
	}
}

void hand_model_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	int hint;
	int ind = -1;
	link_t_ptr ilk = NULL;
	xpoint_t pt;

	if (!ptd)
		return;

	noti_model_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	hint = calc_anno_hint(&pt, ptd->anno, &ilk, &ind);

	if (hint == ANNO_HINT_SIZE && ilk == ptd->arti)
	{
		ptd->org_x = pxp->x;
		ptd->org_y = pxp->y;

		ptd->b_size = 1;
		ptd->index = ind;

		widget_set_cursor(widget, CURSOR_SIZEALL);
	}
	else if (hint == ANNO_HINT_ARTI && ilk == ptd->arti)
	{
		ptd->org_x = pxp->x;
		ptd->org_y = pxp->y;

		ptd->b_drag = 1;

		widget_set_cursor(widget, CURSOR_HAND);
	}
}

void hand_model_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	link_t_ptr ilk = NULL;
	int ind = -1;
	int hint;
	xpoint_t pt;
	bool_t bRe;

	if (!ptd)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}

	if (ptd->b_size)
	{
		noti_model_arti_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag)
	{
		noti_model_arti_drop(widget, pxp->x, pxp->y);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	hint = calc_anno_hint(&pt, ptd->anno, &ilk, &ind);

	bRe = (ilk == ptd->arti) ? 1 : 0;

	if (ptd->arti && !bRe)
	{
		if (!noti_model_arti_changing(widget))
			bRe = 1;
	}
	
	if (ilk && !bRe)
	{
		noti_model_arti_changed(widget, ilk);
	}

	noti_model_owner(widget, NC_MODELLBCLK, ptd->arti, (void*)pxp, NULL);
}

void hand_model_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	if (ptd->arti && !ptd->b_lock)
	{
		noti_model_begin_edit(widget);
	}

	noti_model_owner(widget, NC_MODELDBCLK, ptd->arti, (void*)pxp, NULL);
}

void hand_model_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	noti_model_reset_editor(widget, 1);
}

void hand_model_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	noti_model_owner(widget, NC_MODELRBCLK, ptd->arti, (void*)pxp, NULL);
}

void hand_model_scroll(res_win_t widget, bool_t bHorz, int nLine)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	if (!ptd)
		return;

	noti_model_reset_editor(widget, 1);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_model_wheel(res_win_t widget, bool_t bHorz, int nDelta)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	scroll_t scr = { 0 };
	int nLine;
	res_win_t win;

	if (!ptd)
		return;

	noti_model_reset_editor(widget, 1);

	widget_get_scroll_info(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (widget_hand_scroll(widget, bHorz, nLine))
	{
		if (!bHorz && !(widget_get_style(widget) & WD_STYLE_VSCROLL))
		{
			if (!widget_is_valid(ptd->vsc))
			{
				ptd->vsc = show_vertbox(widget);
			}
		}

		if (bHorz && !(widget_get_style(widget) & WD_STYLE_HSCROLL))
		{
			if (!widget_is_valid(ptd->hsc))
			{
				ptd->hsc = show_horzbox(widget);
			}
		}

		return;
	}

	win = widget_get_parent(widget);

	if (widget_is_valid(win))
	{
		widget_scroll(win, bHorz, nLine);
	}
}

void hand_model_keydown(res_win_t widget, dword_t ks, int key)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	if (key == KEY_ENTER)
	{
		noti_model_begin_edit(widget);
	}
	else if ((key == _T('z') || key == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
	{
		_modelctrl_undo(widget);
	}else if ((key == _T('c') || key == _T('C')) && widget_key_state(widget, KEY_CONTROL))
	{
		_modelctrl_copy(widget);
	}
	else if ((key == _T('x') || key == _T('X')) && widget_key_state(widget, KEY_CONTROL))
	{
		_modelctrl_done(widget);

		if (!_modelctrl_cut(widget))
		{
			_modelctrl_discard(widget);
		}
	}
	else if ((key == _T('v') || key == _T('V')) && widget_key_state(widget, KEY_CONTROL))
	{
		_modelctrl_done(widget);

		if (!_modelctrl_paste(widget))
		{
			_modelctrl_discard(widget);
		}
	}
}

void hand_model_set_focus(res_win_t widget, res_win_t wt)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	if (!ptd)
		return;
}

void hand_model_kill_focus(res_win_t widget, res_win_t wt)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	if (!ptd)
		return;

	if (widget_is_editor(widget))
	{
		if (modelctrl_get_dirty(widget))
			widget_send_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		else
			widget_send_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, (var_long)NULL);
	}
}

void hand_model_child_command(res_win_t widget, int code, var_long data)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_model_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_model_rollback_edit(widget);
		break;
	}
}

void hand_model_size(res_win_t widget, int code, const xsize_t* prs)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	_modelctrl_reset_page(widget);

	widget_erase(widget, NULL);
}

void hand_model_paint(res_win_t widget, visual_t dc, const xrect_t* pxr)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	visual_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr;

	canvas_t canv;
	if_canvas_t* pif;
	if_visual_t* piv;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);
	widget_get_canv_rect(widget, &pif->rect);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	piv = create_visual_interface(rdc);

	(*piv->pf_draw_rect_raw)(piv->visual, NULL, &xb, &xr);

	widget_get_view_rect(widget, (viewbox_t*)&xr);

	if (ptd->bmp)
	{
		(*piv->pf_draw_bitmap_raw)(piv->visual, ptd->bmp, RECTPOINT(&xr));
	}

	noti_model_owner(widget, NC_MODELFACEDRAW, NULL, (void*)&xr, rdc);

	draw_anno(pif, ptd->anno);

	if (ptd->arti)
	{
		_modelctrl_arti_rect(widget, ptd->arti, &xr);
		pt_expand_rect(&xr, 1, 1);

		parse_xcolor(&xc, DEF_ALARM_COLOR);
		draw_select_raw(piv, &xc, &xr, ALPHA_SOLID);
	}

	destroy_visual_interface(piv);

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t modelctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_model_create)
		EVENT_ON_DESTROY(hand_model_destroy)

		EVENT_ON_PAINT(hand_model_paint)

		EVENT_ON_SIZE(hand_model_size)

		EVENT_ON_SCROLL(hand_model_scroll)
		EVENT_ON_WHEEL(hand_model_wheel)

		EVENT_ON_KEYDOWN(hand_model_keydown)

		EVENT_ON_MOUSE_MOVE(hand_model_mouse_move)
		EVENT_ON_LBUTTON_DBCLICK(hand_model_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_model_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_model_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_model_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_model_rbutton_up)

		EVENT_ON_CHILD_COMMAND(hand_model_child_command)

		EVENT_ON_SET_FOCUS(hand_model_set_focus)
		EVENT_ON_KILL_FOCUS(hand_model_kill_focus)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void modelctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	_modelctrl_clean(widget);

	ptd->anno = ptr;
	ptd->arti = NULL;

	modelctrl_redraw(widget);
}

link_t_ptr modelctrl_detach(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	link_t_ptr ptr;

	_modelctrl_clean(widget);

	ptr = ptd->anno;

	ptd->anno = NULL;
	ptd->arti = NULL;

	modelctrl_redraw(widget);

	return ptr;
}

link_t_ptr modelctrl_fetch(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	return ptd->anno;
}

void modelctrl_set_object(res_win_t widget, const byte_t* data, dword_t size)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	visual_t rdc;

	XDL_ASSERT(ptd != NULL);

	noti_model_reset_editor(widget, 0);

	rdc = widget_client_ctx(widget);

	if (ptd->bmp)
	{
		destroy_bitmap(ptd->bmp);
		ptd->bmp = NULL;
	}

	ptd->bmp = load_bitmap_from_bytes(rdc, data, size);

	widget_release_ctx(widget, rdc);

	_modelctrl_reset_page(widget);

	widget_erase(widget, NULL);
}

dword_t modelctrl_get_object(res_win_t widget, byte_t* buf, dword_t max)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	dword_t len_bmp;
	visual_t rdc;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->bmp)
		return 0;

	len_bmp = get_bitmap_bytes(ptd->bmp);
	if (!buf)
		return len_bmp;

	rdc = widget_client_ctx(widget);

	save_bitmap_to_bytes(rdc, ptd->bmp, buf, max);

	widget_release_ctx(widget, rdc);

	return max;
}

void modelctrl_redraw(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_model_reset_editor(widget, 0);

	widget_erase(widget, NULL);
}

void modelctrl_set_lock(res_win_t widget,bool_t bLock)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t modelctrl_get_lock(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

bool_t modelctrl_set_focus_arti(res_win_t widget, link_t_ptr ilk)
{
	model_delta_t* ptd = GETMODELDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->arti)
		return 0;

	if (ilk == LINK_FIRST)
		ilk = get_anno_next_arti(ptd->anno, LINK_FIRST);
	else if (ilk == LINK_LAST)
		ilk = get_anno_prev_arti(ptd->anno, LINK_LAST);

	bRe = (ilk == ptd->arti) ? 1 : 0;

	if (!bRe && ptd->arti)
	{
		if (!noti_model_arti_changing(widget))
			return 0;
	}

	if (!bRe && ilk)
	{
		noti_model_arti_changed(widget, ilk);
	}

	return 1;
}

link_t_ptr modelctrl_get_focus_arti(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->arti;
}

bool_t modelctrl_get_dirty(res_win_t widget)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return peek_stack_node(ptd->stack, -1) ? 1 : 0;
}

void modelctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	model_delta_t* ptd = GETMODELDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->anno)
		return;

	if (bDirty)
		_modelctrl_done(widget);
	else
		_modelctrl_clean(widget);
}

