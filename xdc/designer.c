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


void noti_designer_item_sizing(designer_t* ptd, link_t_ptr ilk, const xpoint_t* pxp)
{
	bool_t bHorz = 0;

	
	if (bHorz)
		widget_set_cursor(ptd->widget, CURSOR_SIZENS);
	else
		widget_set_cursor(ptd->widget, CURSOR_SIZEWE);

}

void noti_designer_item_sized(designer_t* ptd, const xpoint_t* pxp)
{
	
}

/*************************************************************************************************/

bool_t hand_designer_mouse_move(designer_t* ptd, dword_t dw, const xpoint_t* pxp)
{
	return 0;
}

bool_t hand_designer_lbutton_down(designer_t* ptd, const xpoint_t* pxp)
{

	return 0;
}

bool_t hand_designer_lbutton_up(designer_t* ptd, const xpoint_t* pxp)
{

	return 0;
}

void hand_designer_paint(designer_t* ptd, visual_t rdc)
{
	link_t_ptr ilk;
	link_t_ptr st = NULL;
	xrect_t xr;
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };;
	if_drawing_t ifv = {0};
	const if_drawing_t* pif = NULL;

	XDL_ASSERT(ptd != NULL);

	widget_get_xbrush(ptd->widget, &xb);

	widget_get_xpen(ptd->widget, &xp);

	widget_get_client_rect(ptd->widget, &xr);

	pif = widget_get_canvas_interface(ptd->widget);

	parse_xcolor(&xc, xp.color);
	lighten_xcolor(&xc, DEF_SOFT_DARKEN);
	draw_ruler(pif, &xc, (const xrect_t*)&(pif->rect));
}

