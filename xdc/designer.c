/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc designer document

	@module	designer.c | implement file

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

#include "designer.h"
#include "xdcimp.h"


/*************************************************************************************************/

typedef struct _DESIGNER_CALC{
	const xpoint_t* ppt;

	link_t_ptr xlk;
	link_t_ptr ylk;
	bool_t focus;
	bool_t drag;
	bool_t sizew;
	bool_t sizeh;
	xrect_t rect;

	const if_drawing_t* piv;
}DESIGNER_CALC;

static int _on_hint_calc_focus(int state, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr, bool_t focus, bool_t drag, bool_t sizew, bool_t sizeh, void* pp)
{
	DESIGNER_CALC* ptt = (DESIGNER_CALC*)pp;
	
	switch (state)
	{
	case _HINTNER_STATE_ITEM:
		if (ft_in_rect(ptt->ppt, pxr))
		{
			ptt->xlk = xlk;
			ptt->ylk = ylk;
			xmem_copy((void*)&ptt->rect, (void*)pxr, sizeof(xrect_t));

			ptt->focus = focus;
			ptt->drag = drag;
			ptt->sizew = sizew;
			ptt->sizeh = sizeh;

			return _HINTNER_OPERA_STOP;
		}
		break;
	case _HINTNER_STATE_NONE:

		return _HINTNER_OPERA_STOP;
	}

	return _HINTNER_OPERA_NEXT;
}

void _designer_calc_focus(designer_t* ptd, const xpoint_t* ppt, link_t_ptr* p_xlk, link_t_ptr* p_ylk, xrect_t* p_rect, bool_t* p_focus, bool_t* p_drag, bool_t* p_sizew, bool_t* p_sizeh)
{
	DESIGNER_CALC tt = { 0 };
	xpoint_t pt;

	xmem_copy((void*)&pt, (void*)ppt, sizeof(xpoint_t));
	widget_point_to_tm(ptd->widget, &pt);

	tt.ppt = &pt;

	(*ptd->pf_hint_test)((void*)ptd->data, ptd->page, _on_hint_calc_focus, (void*)&tt);

	if (p_xlk)
		*p_xlk = tt.xlk;
	if (p_ylk)
		*p_ylk = tt.ylk;
	if (p_focus)
		*p_focus = tt.focus;
	if (p_drag)
		*p_drag = tt.drag;
	if (p_sizew)
		*p_sizew = tt.sizew;
	if (p_sizeh)
		*p_sizeh = tt.sizeh;
	if (p_rect)
	{
		xmem_copy((void*)p_rect, (void*)&tt.rect, sizeof(xrect_t));
		widget_rect_to_pt(ptd->widget, p_rect);
	}
}

bool_t hand_designer_mouse_move(designer_t* ptd, dword_t dw, const xpoint_t* pxp)
{


	return 0;
}

bool_t hand_designer_lbutton_down(designer_t* ptd, const xpoint_t* pxp)
{
	_designer_calc_focus(ptd, pxp, &ptd->xlk, &ptd->ylk, &ptd->rect, &ptd->focus, &ptd->drag, &ptd->sizew, &ptd->sizeh);

	return 0;
}

bool_t hand_designer_lbutton_up(designer_t* ptd, const xpoint_t* pxp)
{

	return 0;
}

void hand_designer_paint(designer_t* ptd, visual_t rdc)
{
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };

	if_drawing_t ifv = {0};
	const if_drawing_t* pif = NULL;

	widget_get_xpen(ptd->widget, &xp);

	pif = widget_get_canvas_interface(ptd->widget);

	parse_xcolor(&xc, xp.color);
	lighten_xcolor(&xc, DEF_SOFT_DARKEN);
	draw_ruler(pif, &xc, (const xrect_t*)&(pif->rect));

	get_visual_interface(rdc, &ifv);

	parse_xcolor(&xc, DEF_ENABLE_COLOR);
	draw_focus_raw(&ifv, &xc, &ptd->rect, ALPHA_SOLID);
}

