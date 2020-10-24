/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdcdlg document

	@module	xdcdlg.c | implement file

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
#include "xdcimp.h"
#include "xdcbox.h"
#include "xdcctrl.h"

res_win_t create_dialog(link_t_ptr ptr_dlg, res_win_t owner)
{
	res_win_t dlg,box = NULL;
	xrect_t xr = { 0 };
	xspan_t xn;
	link_t_ptr ilk;
	const tchar_t* type;
	visual_t rdc;
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)
		EVENT_ON_NC_IMPLEMENT
	EVENT_END_DISPATH
	
	dlg = widget_create(get_dialog_name_ptr(ptr_dlg), WD_STYLE_DIALOG, &xr, owner, &ev);

	XDL_ASSERT(dlg != NULL);

	widget_set_owner(dlg, owner);
	widget_set_user_id(dlg, xstol(get_dialog_id_ptr(ptr_dlg)));

	rdc = widget_client_ctx(owner);

	ilk = get_dialog_prev_item(ptr_dlg, LINK_LAST);
	while (ilk)
	{
		xn.fr = get_dialog_item_x(ilk);
		cast_mm_to_pt(rdc, 1, &xn);
		xr.x = xn.r;

		xn.fr = get_dialog_item_y(ilk);
		cast_mm_to_pt(rdc, 0, &xn);
		xr.y = xn.r;

		xn.fr = get_dialog_item_width(ilk);
		cast_mm_to_pt(rdc, 1, &xn);
		xr.w = xn.r;

		xn.fr = get_dialog_item_height(ilk);
		cast_mm_to_pt(rdc, 1, &xn);
		xr.h = xn.r;

		type = get_dialog_item_class_ptr(ilk);

		if (compare_text(type, -1, DOC_DIALOG_DATEBOX, -1, 1) == 0)
		{
			box = datebox_create(dlg, WD_STYLE_CONTROL, &xr);
		}
		else if (compare_text(type, -1, DOC_DIALOG_EDITBOX, -1, 1) == 0)
		{
			box = editbox_create(dlg, WD_STYLE_CONTROL, &xr);
			editbox_set_text(box, get_dialog_item_text_ptr(ilk));
		}
		else if (compare_text(type, -1, DOC_DIALOG_LISTBOX, -1, 1) == 0)
		{
			box = listbox_create(dlg, WD_STYLE_CONTROL, &xr);
			listbox_set_options(box, get_dialog_item_text_ptr(ilk), -1);
		}
		else if (compare_text(type, -1, DOC_DIALOG_PUSHBOX, -1, 1) == 0)
		{
			box = pushbox_create(dlg, WD_STYLE_CONTROL, &xr);
			pushbox_set_text(box, get_dialog_item_text_ptr(ilk), -1);
		}
		else if (compare_text(type, -1, DOC_DIALOG_RADIOBOX, -1, 1) == 0)
		{
			box = radiobox_create(dlg, WD_STYLE_CONTROL, &xr);
			radiobox_set_state(box, (bool_t)xstol(get_dialog_item_text_ptr(ilk)));
		}
		else if (compare_text(type, -1, DOC_DIALOG_CHECKBOX, -1, 1) == 0)
		{
			box = checkbox_create(dlg, WD_STYLE_CONTROL, &xr);
			radiobox_set_state(box, (bool_t)xstol(get_dialog_item_text_ptr(ilk)));
		}
		else if (compare_text(type, -1, DOC_DIALOG_SHAPEBOX, -1, 1) == 0)
		{
			box = shapebox_create(dlg, WD_STYLE_CONTROL, &xr);
			shapebox_set_shape(box, get_dialog_item_text_ptr(ilk));
		}
		else if (compare_text(type, -1, DOC_DIALOG_SLIDEBOX, -1, 1) == 0)
		{
			box = slidebox_create(dlg, WD_STYLE_CONTROL, &xr);
			slidebox_set_slide(box, xstol(get_dialog_item_text_ptr(ilk)));
		}
		else if (compare_text(type, -1, DOC_DIALOG_SPINBOX, -1, 1) == 0)
		{
			box = spinbox_create(dlg, WD_STYLE_CONTROL, &xr);
			spinbox_set_spin(box, xstol(get_dialog_item_text_ptr(ilk)));
		}
		else if (compare_text(type, -1, DOC_DIALOG_STATICBOX, -1, 1) == 0)
		{
			box = staticbox_create(dlg, WD_STYLE_CONTROL, &xr);
			staticbox_set_text(box, get_dialog_item_text_ptr(ilk));
		}
		else if (compare_text(type, -1, DOC_DIALOG_TIMEBOX, -1, 1) == 0)
		{
			box = timebox_create(dlg, WD_STYLE_CONTROL, &xr);
		}
		else if (compare_text(type, -1, DOC_DIALOG_NAVIBOX, -1, 1) == 0)
		{
			box = navibox_create(dlg, WD_STYLE_CONTROL, &xr);
		}
		else if (compare_text(type, -1, DOC_DIALOG_USERBOX, -1, 1) == 0)
		{
			if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_FORMCTRL, -1, 1) == 0)
				box = formctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_GRIDCTRL, -1, 1) == 0)
				box = gridctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_STATISCTRL, -1, 1) == 0)
				box = statisctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_TREECTRL, -1, 1) == 0)
				box = treectrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_LISTCTRL, -1, 1) == 0)
				box = listctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_LABELCTRL, -1, 1) == 0)
				box = labelctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_PROPERCTRL, -1, 1) == 0)
				box = properctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_STATUSCTRL, -1, 1) == 0)
				box = statusctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_TOOLCTRL, -1, 1) == 0)
				box = toolctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_TITLECTRL, -1, 1) == 0)
				box = titlectrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_TABLECTRL, -1, 1) == 0)
				box = tablectrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_NOTESCTRL, -1, 1) == 0)
				box = notesctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_RICHCTRL, -1, 1) == 0)
				box = richctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_MEMOCTRL, -1, 1) == 0)
				box = memoctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_TAGCTRL, -1, 1) == 0)
				box = tagctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_PHOTOCTRL, -1, 1) == 0)
				box = photoctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_IMAGESCTRL, -1, 1) == 0)
				box = imagesctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else if (compare_text(get_dialog_item_text_ptr(ilk), -1, ATTR_CONTROL_TOPOGCTRL, -1, 1) == 0)
				box = topogctrl_create(get_dialog_item_name_ptr(ilk), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, dlg);
			else
				box = NULL;
		}

		if (box)
		{
			widget_set_owner(box, dlg);
			widget_set_user_id(box, xstol(get_dialog_item_id_ptr(ilk)));
			widget_set_point(box, RECTPOINT(&xr));
			widget_show(box, WS_SHOW_NORMAL);
		}
		
		ilk = get_dialog_prev_item(ptr_dlg, ilk);
	}

	xn.fr = get_dialog_width(ptr_dlg);
	cast_mm_to_pt(rdc, 1, &xn);
	xr.w = xn.r;

	xn.fr = get_dialog_height(ptr_dlg);
	cast_mm_to_pt(rdc, 0, &xn);
	xr.h = xn.r;

	widget_release_ctx(owner, rdc);

	widget_adjust_size(WD_STYLE_DIALOG, RECTSIZE(&xr));
	widget_size(dlg, RECTSIZE(&xr));
	widget_update(dlg);
	widget_center_window(dlg, owner);

	return dlg;
}


int sub_dialog_on_paint(res_win_t widget, visual_t dc, const xrect_t* pxr, uid_t sid, var_long delta)
{
	visual_t rdc;
	xrect_t xr;

	xbrush_t xb = { 0 };

	canvas_t canv;
	const if_drawing_t* pif = NULL;
	if_drawing_t ifv = {0};

	widget_get_xbrush(widget, &xb);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);
	if (canv)
	{
		pif = widget_get_canvas_interface(widget);

		rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);
	}
	else
	{
		rdc = dc;
	}

	get_visual_interface(rdc, &ifv);

	(*ifv.pf_draw_rect)(ifv.ctx, NULL, &xb, &xr);

	if (canv)
	{
		end_canvas_paint(canv, dc, pxr);
		
	}

	return 1;
}

static int STDCALL _widget_set_child_point(res_win_t widget, var_long pv)
{
	xpoint_t pt;

	if (widget_has_struct(widget))
	{
		widget_get_point(widget, &pt);

		widget_move(widget, &pt);
	}

	return 1;
}

int sub_dialog_on_size(res_win_t widget, int code, const xsize_t* pxs, uid_t sid, var_long delta)
{
	if (code != WS_SIZE_MINIMIZED)
	{
		widget_enum_child(widget, _widget_set_child_point, (var_long)0);

		widget_erase(widget, NULL);

		return 1;
	}
	else
	{
		return 0;
	}
}
