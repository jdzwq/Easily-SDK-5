/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget default handler document

	@module	handler.c | implement file

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

#include "handler.h"
#include "widgetex.h"
#include "widgetnc.h"
#include "splitor.h"
#include "docker.h"

/************************************default widget handler**************************************************/

void widgetex_hand_create(res_win_t wt)
{
	widget_struct_t* pwt;
	res_ctx_t rdc;

	pwt = (widget_struct_t*)xmem_alloc(sizeof(widget_struct_t));

	default_xfont(&pwt->xf);
	default_xface(&pwt->xa);
	default_xpen(&pwt->xp);
	default_xbrush(&pwt->xb);
	parse_xcolor(&pwt->msk, GDI_ATTR_RGB_WHITE);
	parse_xcolor(&pwt->ico, GDI_ATTR_RGB_GRAY);

	rdc = widget_client_ctx(wt);
	pwt->canv = create_display_canvas(rdc);
	widget_release_ctx(wt, rdc);

	SETWIDGSTRUCT(wt, pwt);
}

void widgetex_hand_destroy(res_win_t wt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	if (pwt)
	{
		destroy_display_canvas(pwt->canv);

		xmem_free(pwt);
	}

	SETWIDGSTRUCT(wt, 0);
}

bool_t widgetex_hand_scroll(res_win_t wt, bool_t bHorz, int nLine)
{
	int nCur, nMax;
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (bHorz)
	{
		nMax = (pwt->cb.w - pwt->vb.w);
		if (nMax < 0)
			nMax = 0;

		if (nLine < 0 && pwt->vb.x == 0)
			return 0;

		if (nLine > 0 && pwt->vb.x == nMax)
			return 0;

		nCur = (pwt->vb.x + nLine);

		if (nCur > nMax)
			nCur = nMax;
		if (nCur < 0)
			nCur = 0;

		pwt->vb.x = nCur;
	}
	else
	{
		nMax = (pwt->cb.h - pwt->vb.h);
		if (nMax < 0)
			nMax = 0;

		if (nLine < 0 && pwt->vb.y == 0)
			return 0;

		if (nLine > 0 && pwt->vb.y == nMax)
			return 0;

		nCur = (pwt->vb.y + nLine);

		if (nCur > nMax)
			nCur = nMax;
		if (nCur < 0)
			nCur = 0;

		pwt->vb.y = nCur;
	}

	widget_redraw(wt, NULL, 0);

	widget_reset_scroll(wt, bHorz);

	return 1;
}

/************************************default widget splitting dispatch**************************************************/

void widgetex_splitor_on_mousemove(res_win_t wt, dword_t dw, const xpoint_t* pxp)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_mouse_move(&pwt->splitor, dw, pxp);
	}
}

void widgetex_splitor_on_lbuttondown(res_win_t wt, const xpoint_t* pxp)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_lbutton_down(&pwt->splitor, pxp);
	}
}

void widgetex_splitor_on_lbuttonup(res_win_t wt, const xpoint_t* pxp)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_lbutton_up(&pwt->splitor, pxp);
	}
}

void widgetex_splitor_on_size(res_win_t wt, int code, const xsize_t* pxs)
{
	widget_struct_t* pwt;
	xrect_t xr;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		widget_get_client_rect(wt, &xr);
		hand_splitor_size(&pwt->splitor, &xr);
	}
}

void widgetex_splitor_on_paint(res_win_t wt, res_ctx_t rdc, const xrect_t* prt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_paint(&pwt->splitor, rdc);
	}
}

int widgetex_splitor_sub_mousemove(res_win_t wt, dword_t dw, const xpoint_t* pxp, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_mouse_move(&pwt->splitor, dw, pxp);
		return 1;
	}

	return 0;
}

int widgetex_splitor_sub_lbuttondown(res_win_t wt, const xpoint_t* pxp, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_lbutton_down(&pwt->splitor, pxp);
		return 1;
	}

	return 0;
}

int widgetex_splitor_sub_lbuttonup(res_win_t wt, const xpoint_t* pxp, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_lbutton_up(&pwt->splitor, pxp);

		return 1;
	}

	return 0;
}

int widgetex_splitor_sub_size(res_win_t wt, int code, const xsize_t* pxs, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;
	xrect_t xr;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		widget_get_client_rect(wt, &xr);
		hand_splitor_size(&pwt->splitor, &xr);

		return 1;
	}

	return 0;
}

int widgetex_splitor_sub_paint(res_win_t wt, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->splitor.split)
	{
		hand_splitor_paint(&pwt->splitor, rdc);

		return 1;
	}

	return 0;
}

/************************************default widget docking dispatch**************************************************/

void widgetex_docker_on_mousemove(res_win_t wt, dword_t dw, const xpoint_t* pxp)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_mouse_move(&pwt->docker, dw, pxp);
	}
}

void widgetex_docker_on_lbuttondown(res_win_t wt, const xpoint_t* pxp)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_lbutton_down(&pwt->docker, pxp);
	}
}

void widgetex_docker_on_lbuttonup(res_win_t wt, const xpoint_t* pxp)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_lbutton_up(&pwt->docker, pxp);
	}
}

void widgetex_docker_on_paint(res_win_t wt, res_ctx_t rdc, const xrect_t* prt)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_paint(&pwt->docker, rdc, prt);
	}
}

int widgetex_docker_sub_mousemove(res_win_t wt, dword_t dw, const xpoint_t* pxp, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_mouse_move(&pwt->docker, dw, pxp);
	}

	return 1;
}

int widgetex_docker_sub_lbuttondown(res_win_t wt, const xpoint_t* pxp, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_lbutton_down(&pwt->docker, pxp);
	}
	return 1;
}

int widgetex_docker_sub_lbuttonup(res_win_t wt, const xpoint_t* pxp, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_lbutton_up(&pwt->docker, pxp);
	}

	return 1;
}

int widgetex_docker_sub_paint(res_win_t wt, res_ctx_t rdc, const xrect_t* prt, uid_t sid, var_int delta)
{
	widget_struct_t* pwt;

	pwt = GETWIDGSTRUCT(wt);

	XDL_ASSERT(pwt != NULL);

	if (pwt->docker.widget)
	{
		hand_docker_paint(&pwt->docker, rdc, prt);
	}

	return 1;
}
