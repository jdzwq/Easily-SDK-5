/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc widget document

	@module	_if_widget.c | widget Xwidnows implement file

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

#include "xdkiml.h"
#include "xdkattr.h"
#include "xdkutil.h"

#ifdef XDK_SUPPORT_WIDGET

#define WIDGET_TITLE_SPAN		(float)10	//mm
#define WIDGET_MENU_SPAN		(float)7.5	//mm
#define WIDGET_SCROLL_SPAN		(float)5	//mm
#define WIDGET_ICON_SPAN		(float)3	//mm
#define WIDGET_FRAME_EDGE		(float)1.5	//mm
#define WIDGET_CHILD_EDGE		(float)0.5	//mm

#define HIWORD(dw)		(unsigned short)(((unsigned long)(dw) >> 16) & 0x0000FFFF)
#define LOWORD(dw)		(unsigned short)((unsigned long)(dw) & 0x0000FFFF)


#define GETXDKDISPATCH(hWnd)		(if_event_t*)(hWnd->ptr_if_event)
#define SETXDKDISPATCH(hWnd, ev)	(hWnd->ptr_if_event = (res_hand_t)ev)

#define GETXDKSUBPROC(hWnd)			(if_subproc_t*)(hWnd->ptr_if_subproc)
#define SETXDKSUBPROC(hWnd, lp)		(hWnd->ptr_if_subproc = (res_hand_t)lp)

#define GETXDKCOREDELTA(hWnd)		(void*)(hWnd->delta)
#define SETXDKCOREDELTA(hWnd, lp)	(hWnd->delta = (res_hand_t)lp)

#define GETXDKUSERDELTA(hWnd)		(void*)(hWnd->pdata)
#define SETXDKUSERDELTA(hWnd, lp)	(hWnd->pdata = (res_hand_t)lp)

#define GETXDKSTYLE(hWnd)			(u32_t)(hWnd->style)
#define SETXDKSTYLE(hWnd, dw)		(hWnd->style = (u32_t)dw)

#define GETXDKACCEL(hWnd)			(res_acl_t)(hWnd->accel)
#define SETXDKACCEL(hWnd, acl)		(hWnd->accel = (res_acl_t)acl)

#define GETXDKOWNER(hWnd)			(res_win_t)(hWnd->owner)
#define SETXDKOWNER(hWnd, win)		(hWnd->owner = (res_hand_t)win)

#define GETXDKUSER(hWnd)			(u32_t)(hWnd->uid)
#define SETXDKUSER(hWnd, dw)		(hWnd->uid = (u32_t)dw)

#define GETXDKRESULT(hWnd)			(u32_t)(hWnd->result)
#define SETXDKRESULT(hWnd, dw)		(hWnd->result = (u32_t)dw)

result_t XdcWidgetProc(res_win_t hWnd, unsigned int message, wparam_t wParam, lparam_t lParam);

static void _ClientRectToWindow(res_win_t hWnd, XRectangle* prt)
{

}

static void _ClientPointToWindow(res_win_t hWnd, XPoint* ppt)
{

}

static void _ScreenRectToWindow(res_win_t hWnd, XRectangle* prt)
{

}

static void _ScreenPointToWindow(res_win_t hWnd, XPoint* ppt)
{

}

static void _CenterRect(XRectangle* prt, long cx, long cy)
{

}

static u32_t _WindowStyle(u32_t wstyle)
{
	u32_t dw = 0;


	return dw;
}

/*******************************************************************************************/

Atom RegisterXdcWidgetClass(res_modu_t hInstance)
{
    return 0;
}

result_t XdcWidgetProc(res_win_t hWnd, unsigned int message, wparam_t wParam, lparam_t lParam)
{
	X11_create_struct_t* lpcs;
	if_event_t* pev;
	u32_t ds;

	switch (message)
	{
	case WM_NCPAINT:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_nc_paint)
		{
			return 0;
		}
		break;
	case WM_NCCALCSIZE:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_nc_calcsize)
		{
            return 0;
		}
		break;
	case WM_NCHITTEST:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_nc_hittest)
		{
            
		}
		break;
	case WM_NCLBUTTONUP:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_nc_hittest)
		{

		}
		break;
	case WM_NCRBUTTONUP:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		break;
	case WM_NCACTIVATE:
		ds = GETXDKSTYLE(hWnd);
		if (!(ds & WD_STYLE_OWNERNC))
			break;

		break;

	case WM_CREATE:
		lpcs = (X11_create_struct_t*)lParam;
		if (lpcs->param)
		{
			if_event_t* pv = (if_event_t*)lpcs->param;
			if (pv)
			{
                if_event_t* pev = NULL;
				SETXDKDISPATCH(hWnd, pev);
			}
		}

		pev = GETXDKDISPATCH(hWnd);

		if (pev && pev->pf_on_create)
		{
			if ((*pev->pf_on_create)(hWnd, pev->param))
				return -1;
		}
		break;
	case WM_DESTROY:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_destroy)
		{
			(*pev->pf_on_destroy)(hWnd);
		}
		break;
	case WM_CLOSE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_close)
		{
			if ((*pev->pf_on_close)(hWnd))
				return 0;
		}
		break;
	case WM_LBUTTONDBLCLK:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_lbutton_dbclick)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_lbutton_dbclick)(hWnd, &xp);
		}
		break;
	case WM_LBUTTONDOWN:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_lbutton_down)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_lbutton_down)(hWnd, &xp);
		}
		break;
	case WM_LBUTTONUP:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_lbutton_up)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_lbutton_up)(hWnd, &xp);
		}
		break;
	case WM_RBUTTONDOWN:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_rbutton_down)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_rbutton_down)(hWnd, &xp);
		}
		break;
	case WM_RBUTTONUP:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_rbutton_up)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_rbutton_up)(hWnd, &xp);
		}
		break;
	case WM_MOUSEMOVE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_mouse_move)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_mouse_move)(hWnd, (u32_t)wParam, &xp);
		}
		break;
	case WM_MOUSEHOVER:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_mouse_hover)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_mouse_hover)(hWnd, (u32_t)wParam, &xp);
		}
		break;
	case WM_MOUSELEAVE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_mouse_leave)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_mouse_leave)(hWnd, (u32_t)wParam, &xp);
		}
		break;
	case WM_MOVE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_move)
		{
			xpoint_t xp;

			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			(*pev->pf_on_move)(hWnd, &xp);
			return 0;
		}
		break;
	case WM_SIZE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_size)
		{
			xsize_t xs;
			
			xs.cx = (long)(short)LOWORD(lParam);
			xs.cy = (long)(short)HIWORD(lParam);

			(*pev->pf_on_size)(hWnd, (int)wParam, &xs);

			return 0;
		}
		break;
	case WM_SHOWWINDOW:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_show && !lParam)
		{
			(*pev->pf_on_show)(hWnd, (bool_t)wParam);
			return 0;
		}
		break;
	case WM_HSCROLL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_scroll)
		{
			scroll_t scr = { 0 };
			_widget_get_scroll_info(hWnd, 1, &scr);
			
			switch (LOWORD(wParam))
			{
			case SB_LEFT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, -scr.pos);
				break;
			case SB_RIGHT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.max - scr.pos);
				break;
			case SB_LINELEFT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, -scr.min);
				break;
			case SB_LINERIGHT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.min);
				break;
			case SB_PAGELEFT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, -scr.page);
				break;
			case SB_PAGERIGHT:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.page);
				break;
			case SB_THUMBPOSITION:
				(*pev->pf_on_scroll)(hWnd, (bool_t)1, scr.track - scr.pos);
				break;
			case SB_ENDSCROLL:
				/*if (pev->pf_on_scrollinfo)
				{
					scroll_t sc = { 0 };
					(*pev->pf_on_scrollinfo)(hWnd, (bool_t)1, &sc);

					scr.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
					scr.nPos = sc.pos;
					scr.nMin = sc.min;
					scr.nMax = sc.max;
					scr.nPage = sc.page;
					SetScrollInfo(hWnd, SB_HORZ, &scr, TRUE);
				}*/
				break;
			}
			return 0;
		}
		break;
	case WM_VSCROLL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_scroll)
		{
			scroll_t scr = { 0 };
			_widget_get_scroll_info(hWnd, 0, &scr);

			switch (LOWORD(wParam))
			{
			case SB_TOP:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, -scr.pos);
				break;
			case SB_BOTTOM:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.max - scr.pos);
				break;
			case SB_LINEUP:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, -scr.min);
				break;
			case SB_LINEDOWN:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.min);
				break;
			case SB_PAGEUP:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, -scr.page);
				break;
			case SB_PAGEDOWN:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.page);
				break;
			case SB_THUMBPOSITION:
				(*pev->pf_on_scroll)(hWnd, (bool_t)0, scr.track - scr.pos);
				break;
			case SB_ENDSCROLL:
				/*if (pev->pf_on_scrollinfo)
				{
					scroll_t sc = { 0 };
					(*pev->pf_on_scrollinfo)(hWnd, (bool_t)0, &sc);

					scr.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
					scr.nPos = sc.pos;
					scr.nMin = sc.min;
					scr.nMax = sc.max;
					scr.nPage = sc.page;
					SetScrollInfo(hWnd, SB_VERT, &scr, TRUE);
				}*/
				break;
			}
			return 0;
		}
		break;
	case WM_SCROLL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_scroll)
		{
			(*pev->pf_on_scroll)(hWnd, (bool_t)wParam, (long)lParam);
		}
		return 0;
	case WM_MOUSEWHEEL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_wheel)
		{

		}
		else if (pev && pev->pf_on_scroll)
		{

		}
		break;
	case WM_MOUSEHWHEEL:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_wheel)
		{

		}
		else if (pev && pev->pf_on_scroll)
		{

		}
		break;
	case WM_ACTIVATE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_activate)
		{
			(*pev->pf_on_activate)(hWnd, (int)LOWORD(wParam));
		}
		break;
	case WM_MOUSEACTIVATE:
		ds = GETXDKSTYLE(hWnd);
		if (ds & WD_STYLE_NOACTIVE)
			return MA_NOACTIVATE;
		break;
	case WM_ERASEBKGND:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_erase)
		{
			(*pev->pf_on_erase)(hWnd, (res_ctx_t)wParam);

			return 0;
		}
		break;
	case WM_PAINT:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_paint)
		{
		}
		break;
	case WM_KEYDOWN:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_keydown)
		{
			(*pev->pf_on_keydown)(hWnd, (int)wParam);
			return 0;
		}
		break;
	case WM_CHAR:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_char)
		{
			(*pev->pf_on_char)(hWnd, (tchar_t)wParam);
			return 0;
		}
		break;
	case WM_ENABLE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_enable)
		{
			(*pev->pf_on_enable)(hWnd, (bool_t)wParam);
			return 0;
		}
		break;
	case WM_SETFOCUS:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_set_focus)
		{
			(*pev->pf_on_set_focus)(hWnd, (res_win_t)wParam);
			return 0;
		}
		break;
	case WM_KILLFOCUS:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_kill_focus)
		{
			(*pev->pf_on_kill_focus)(hWnd, (res_win_t)wParam);
			return 0;
		}
		break;
	case WM_COMMAND:
		pev = GETXDKDISPATCH(hWnd);
		if (pev)
		{
			if (LOWORD(wParam) == IDC_PARENT)
			{
				if (HIWORD(wParam) == COMMAND_FIND)
				{
					if (pev->pf_on_command_find)
					{
						(*pev->pf_on_command_find)(hWnd, (str_find_t*)lParam);
						return 0;
					}
				}
				else if (HIWORD(wParam) == COMMAND_REPLACE)
				{
					if (pev->pf_on_command_replace)
					{
						(*pev->pf_on_command_replace)(hWnd, (str_replace_t*)lParam);
						return 0;
					}
				}
				else if (pev->pf_on_parent_command)
				{
					(*pev->pf_on_parent_command)(hWnd, (int)(short)HIWORD(wParam), (void*)lParam);
					return 0;
				}
			}
			else if (LOWORD(wParam) == IDC_CHILD)
			{
				if (pev->pf_on_child_command)
				{
					(*pev->pf_on_child_command)(hWnd, (int)(short)HIWORD(wParam), (void*)lParam);
					return 0;
				}
			}
			else if (LOWORD(wParam) == IDC_SELF)
			{
				if (pev->pf_on_self_command)
				{
					(*pev->pf_on_self_command)(hWnd, (int)(short)HIWORD(wParam), (void*)lParam);
					return 0;
				}
			}
			else
			{
				if (pev->pf_on_menu_command)
				{
					(*pev->pf_on_menu_command)(hWnd, (int)(short)HIWORD(wParam), (uid_t)(short)LOWORD(wParam), (void*)lParam);
					return 0;
				}
			}
		}
		break;
	case WM_SYSCOMMAND:
		pev = GETXDKDISPATCH(hWnd);
		if ((wParam & 0xFFF0) == SC_CONTEXTHELP && pev && pev->pf_on_syscmd_click)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);
			(*pev->pf_on_syscmd_click)(hWnd, &xp);
			return 0;
		}
		break;
	case WM_TIMER:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_timer)
		{
			(*pev->pf_on_timer)(hWnd, (var_long)(wParam));
			return 0;
		}
		break;
	case WM_COPY:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_copy)
		{
			(*pev->pf_on_copy)(hWnd);
			return 0;
		}
		break;
	case WM_CUT:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_cut)
		{
			(*pev->pf_on_cut)(hWnd);
			return 0;
		}
		break;
	case WM_PASTE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_paste)
		{
			(*pev->pf_on_paste)(hWnd);
			return 0;
		}
		break;
	case WM_UNDO:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_undo)
		{
			(*pev->pf_on_undo)(hWnd);
			return 0;
		}
		break;
	case WM_NOTICE:
		pev = GETXDKDISPATCH(hWnd);
		if (pev && pev->pf_on_notice)
		{
			(*pev->pf_on_notice)(hWnd, (NOTICE*)lParam);
		}
		return 0;
	}

	return 0;
}

result_t XdcSubclassProc(res_win_t hWnd, unsigned int message, wparam_t wParam, lparam_t lParam, uid_t uIdSubclass, var_long dwRefData)
{
	if_subproc_t* pev;

	pev = GETXDKSUBPROC(hWnd);

	switch (message)
	{
	case WM_LBUTTONDBLCLK:
		if (pev && pev->sub_on_lbutton_dbclick)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_lbutton_dbclick)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_LBUTTONDOWN:
		if (pev && pev->sub_on_lbutton_down)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_lbutton_down)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_LBUTTONUP:
		if (pev && pev->sub_on_lbutton_up)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_lbutton_up)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_RBUTTONDOWN:
		if (pev && pev->sub_on_rbutton_down)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_rbutton_down)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_RBUTTONUP:
		if (pev && pev->sub_on_rbutton_up)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_rbutton_up)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOUSEMOVE:
		if (pev && pev->sub_on_mouse_move)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_mouse_move)(hWnd, (u32_t)wParam, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOUSEHOVER:
		if (pev && pev->sub_on_mouse_hover)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_mouse_hover)(hWnd, (u32_t)wParam, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOUSELEAVE:
		if (pev && pev->sub_on_mouse_leave)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_mouse_leave)(hWnd, (u32_t)wParam, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_SIZE:
		if (pev && pev->sub_on_size)
		{
			xsize_t xs = { 0 };
			xs.cx = (long)(short)LOWORD(lParam);
			xs.cy = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_size)(hWnd, (int)wParam, &xs, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOVE:
		if (pev && pev->sub_on_move)
		{
			xpoint_t xp = { 0 };

			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);

			if ((*pev->sub_on_move)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_SHOWWINDOW:
		if (pev && pev->sub_on_show)
		{
			if ((*pev->sub_on_show)(hWnd, (bool_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_SCROLL:
		if (pev && pev->sub_on_scroll)
		{
			if ((*pev->sub_on_scroll)(hWnd, (bool_t)wParam, (long)lParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_MOUSEWHEEL:
		if (pev && pev->sub_on_wheel)
		{

		}
		else if (pev && pev->sub_on_scroll)
		{

		}
		break;
	case WM_MOUSEHWHEEL:
		if (pev && pev->sub_on_wheel)
		{

		}
		else if (pev && pev->sub_on_scroll)
		{

		}
		break;
	case WM_ERASEBKGND:
		if (pev && pev->sub_on_erase)
		{
			if ((*pev->sub_on_erase)(hWnd, (res_ctx_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_PAINT:
		if (pev && pev->sub_on_paint)
		{

		}
		break;
	case WM_KEYDOWN:
		if (pev && pev->sub_on_keydown)
		{
			if ((*pev->sub_on_keydown)(hWnd, (int)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_CHAR:
		if (pev && pev->sub_on_char)
		{
			if ((*pev->sub_on_char)(hWnd, (tchar_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_ENABLE:
		if (pev && pev->sub_on_enable)
		{
			if ((*pev->sub_on_enable)(hWnd, (bool_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_SETFOCUS:
		if (pev && pev->sub_on_set_focus)
		{
			if ((*pev->sub_on_set_focus)(hWnd, (res_win_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_KILLFOCUS:
		if (pev && pev->sub_on_kill_focus)
		{
			if ((*pev->sub_on_kill_focus)(hWnd, (res_win_t)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_NOTICE:
		if (pev && pev->sub_on_notice)
		{
			if ((*pev->sub_on_notice)(hWnd, (NOTICE*)lParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_COMMAND:
		if (pev)
		{
			if (LOWORD(wParam) == IDC_PARENT)
			{
				if (HIWORD(wParam) == COMMAND_FIND)
				{
					if (pev->sub_on_command_find)
					{
						if ((*pev->sub_on_command_find)(hWnd, (str_find_t*)lParam, (uid_t)uIdSubclass, pev->delta))
							return 0;
					}
				}
				else if (HIWORD(wParam) == COMMAND_REPLACE)
				{
					if (pev->sub_on_command_replace)
					{
						if ((*pev->sub_on_command_replace)(hWnd, (str_replace_t*)lParam, (uid_t)uIdSubclass, pev->delta))
							return 0;
					}
				}
				else if (pev->sub_on_parent_command)
				{
					if ((*pev->sub_on_parent_command)(hWnd, (int)HIWORD(wParam), (void*)lParam, (uid_t)uIdSubclass, pev->delta))
						return 0;
				}
			}
			else if (LOWORD(wParam) == IDC_CHILD)
			{
				if (pev->sub_on_child_command)
				{
					if ((*pev->sub_on_child_command)(hWnd, (int)HIWORD(wParam), (void*)lParam, (uid_t)uIdSubclass, pev->delta))
						return 0;
				}
			}
			else if (LOWORD(wParam) == IDC_SELF)
			{
				if (pev->sub_on_self_command)
				{
					if ((*pev->sub_on_self_command)(hWnd, (int)HIWORD(wParam), (void*)lParam, (uid_t)uIdSubclass, pev->delta))
						return 0;
				}
			}
			else 
			{
				if (pev->sub_on_menu_command)
				{
					if ((*pev->sub_on_menu_command)(hWnd, (int)HIWORD(wParam), (int)LOWORD(wParam), (void*)lParam, (uid_t)uIdSubclass, pev->delta))
						return 0;
				}
			}
		}
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_MOUSEMENU && pev && pev->sub_on_sysclr_click)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);
			if ((*pev->sub_on_sysclr_click)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		else if ((wParam & 0xFFF0) == SC_CONTEXTHELP && pev && pev->sub_on_syslog_click)
		{
			xpoint_t xp;
			xp.x = (long)(short)LOWORD(lParam);
			xp.y = (long)(short)HIWORD(lParam);
			if ((*pev->sub_on_syslog_click)(hWnd, &xp, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_TIMER:
		if (pev && pev->sub_on_timer)
		{
			if ((*pev->sub_on_timer)(hWnd, (var_long)wParam, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_COPY:
		if (pev && pev->sub_on_copy)
		{
			if ((*pev->sub_on_copy)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_CUT:
		if (pev && pev->sub_on_cut)
		{
			if ((*pev->sub_on_cut)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_PASTE:
		if (pev && pev->sub_on_paste)
		{
			if ((*pev->sub_on_paste)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_UNDO:
		if (pev && pev->sub_on_undo)
		{
			if ((*pev->sub_on_undo)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 0;
		}
		break;
	case WM_CLOSE:
		if (pev && pev->sub_on_close)
		{
			if ((*pev->sub_on_close)(hWnd, (uid_t)uIdSubclass, pev->delta))
				return 1;
		}
		break;
	case WM_DESTROY:
		if (pev && pev->sub_on_destroy)
		{
			(*pev->sub_on_destroy)(hWnd, (uid_t)uIdSubclass, pev->delta);
		}
		break;
	}

    return 0;
}

void _widget_startup(int ver)
{

}

void _widget_cleanup()
{

}


res_win_t _widget_create(const tchar_t* wname, u32_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev)
{
	return (res_win_t)NULL;
}

void _widget_destroy(res_win_t wt)
{

}

void _widget_close(res_win_t wt, int ret)
{
	SETXDKRESULT(wt, ret);
}

if_subproc_t* _widget_get_subproc(res_win_t wt, uid_t sid)
{
	return GETXDKSUBPROC(wt);
}

bool_t _widget_set_subproc(res_win_t wt, uid_t sid, if_subproc_t* sub)
{

	return 1;
}

void _widget_del_subproc(res_win_t wt, uid_t sid)
{
	SETXDKSUBPROC(wt, NULL);
}

bool_t _widget_set_subproc_delta(res_win_t wt, uid_t sid, var_long delta)
{
	if_subproc_t* psub;

	psub = GETXDKSUBPROC(wt);

	psub->delta = delta;
	return 1;
}

var_long _widget_get_subproc_delta(res_win_t wt, uid_t sid)
{
	if_subproc_t* psub;

	psub = GETXDKSUBPROC(wt);

	return psub->delta;
}

bool_t _widget_has_subproc(res_win_t wt)
{
	return (GETXDKSUBPROC(wt) == NULL) ? 0 : 1;
}

void _widget_set_style(res_win_t wt, u32_t ws)
{
	SETXDKSTYLE(wt, ws);
}

u32_t _widget_get_style(res_win_t wt)
{
	return GETXDKSTYLE(wt);
}

void _widget_set_accel(res_win_t wt, res_acl_t acl)
{
	SETXDKACCEL(wt, acl);
}

res_acl_t _widget_get_accel(res_win_t wt)
{
	return GETXDKACCEL(wt);
}

void _widget_set_owner(res_win_t wt, res_win_t win)
{
	SETXDKOWNER(wt, win);
}

res_win_t _widget_get_owner(res_win_t wt)
{
	return GETXDKOWNER(wt);
}

void _widget_set_core_delta(res_win_t wt, var_long pd)
{
	SETXDKCOREDELTA(wt, pd);
}

var_long _widget_get_core_delta(res_win_t wt)
{
	return GETXDKCOREDELTA(wt);
}

void _widget_set_user_delta(res_win_t wt, var_long pd)
{
	SETXDKUSERDELTA(wt, pd);
}

var_long _widget_get_user_delta(res_win_t wt)
{
	return GETXDKUSERDELTA(wt);
}

void _widget_set_user_id(res_win_t wt, uid_t uid)
{
    SETXDKUSER(wt, uid);
}

uid_t _widget_get_user_id(res_win_t wt)
{
    return GETXDKUSER(wt);
}

void _widget_set_user_result(res_win_t wt, int rt)
{
	SETXDKRESULT(wt, rt);
}

int _widget_get_user_result(res_win_t wt)
{
	return (int)GETXDKRESULT(wt);
}

res_win_t _widget_get_child(res_win_t wt, uid_t uid)
{
	return NULL;
}

res_win_t _widget_get_parent(res_win_t wt)
{
	return (res_win_t)NULL;
}

void _widget_set_user_prop(res_win_t wt, const tchar_t* pname,var_long pval)
{

}

var_long _widget_get_user_prop(res_win_t wt, const tchar_t* pname)
{
    return NULL;
}

var_long _widget_del_user_prop(res_win_t wt, const tchar_t* pname)
{
    return NULL;
}

if_event_t* _widget_get_dispatch(res_win_t wt)
{
	return GETXDKDISPATCH(wt);
}

void _widget_calc_border(u32_t ws, border_t* pbd)
{
	xsize_t xs;

	pbd->edge = pbd->title = pbd->hscroll = pbd->vscroll = pbd->menu = pbd->icon = 0;

	if (ws & WD_STYLE_TITLE)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_TITLE_SPAN;
		_screen_size_to_pt(&xs);

		pbd->title = xs.cy;
	}

	if (ws & WD_STYLE_BORDER)
	{
		xs.fx = ZERO_WIDTH;
		if (ws & WD_STYLE_CHILD)
			xs.fy = WIDGET_CHILD_EDGE;
		else
			xs.fy = WIDGET_FRAME_EDGE;
		_screen_size_to_pt(&xs);

		pbd->edge = xs.cy;
	}

	if (ws & WD_STYLE_HSCROLL)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_SCROLL_SPAN;
		_screen_size_to_pt(&xs);

		pbd->hscroll = xs.cy;
	}

	if (ws & WD_STYLE_VSCROLL)
	{
		xs.fx = WIDGET_SCROLL_SPAN;
		xs.fy = ZERO_HEIGHT;
		_screen_size_to_pt(&xs);

		pbd->vscroll = xs.cx;
	}

	if (ws & WD_STYLE_MENUBAR)
	{
		xs.fx = ZERO_WIDTH;
		xs.fy = WIDGET_MENU_SPAN;
		_screen_size_to_pt(&xs);

		pbd->menu = xs.cy;
	}

	xs.fx = ZERO_WIDTH;
	xs.fy = WIDGET_ICON_SPAN;
	_screen_size_to_pt(&xs);
	pbd->icon = xs.cy;
}

void _widget_adjust_size(u32_t ws, xsize_t* pxs)
{
	pxs->cx += 2;
	pxs->cy += 2;
}

bool_t _widget_is_maximized(res_win_t wt)
{
	return 0;
}

bool_t _widget_is_minimized(res_win_t wt)
{
	return 0;
}

bool_t _widget_enum_child(res_win_t widget, PF_ENUM_WINDOW_PROC pf, var_long pv)
{
	return 1;
}


res_ctx_t _widget_client_ctx(res_win_t wt)
{
	return NULL;
}

res_ctx_t _widget_window_ctx(res_win_t wt)
{
	return NULL;
}

void _widget_release_ctx(res_win_t wt, res_ctx_t dc)
{

}

void _widget_get_client_rect(res_win_t wt, xrect_t* prt)
{

}

void _widget_get_window_rect(res_win_t wt, xrect_t* prt)
{

}

void _widget_client_to_screen(res_win_t wt, xpoint_t* ppt)
{
	XPoint pt;

	pt.x = ppt->x;
	pt.y = ppt->y;

	ppt->x = pt.x;
	ppt->y = pt.y;
}

void _widget_screen_to_client(res_win_t wt, xpoint_t* ppt)
{
	XPoint pt;

	pt.x = ppt->x;
	pt.y = ppt->y;


	ppt->x = pt.x;
	ppt->y = pt.y;
}

void _widget_client_to_window(res_win_t wt, xpoint_t* ppt)
{

}

void _widget_window_to_client(res_win_t wt, xpoint_t* ppt)
{

}

void _widget_center_window(res_win_t wt, res_win_t owner)
{
	
}

void _widget_set_cursor(res_win_t wt, int curs)
{
	switch (curs)
	{
	case CURSOR_SIZENS:
		break;
	case CURSOR_SIZEWE:
		break;
	case CURSOR_SIZEALL:
		break;
	case CURSOR_HAND:
		break;
	case CURSOR_HELP:
		break;
	case CURSOR_DRAG:
		break;
	case CURSOR_ARROW:
		break;
	case CURSOR_IBEAM:
		break;
	default:
		break;
	}
}

void _widget_set_capture(res_win_t wt, bool_t b)
{

}


var_long _widget_set_timer(res_win_t wt, int ms)
{
    return 0;
}

void _widget_kill_timer(res_win_t wt, var_long tid)
{

}

void _widget_create_caret(res_win_t wt, int w, int h)
{

}

void _widget_destroy_caret(res_win_t wt)
{

}

void _widget_show_caret(res_win_t wt, int x, int y, bool_t b)
{

}

void _widget_set_imm(res_win_t wt, bool_t b)
{

}

bool_t _widget_get_imm(res_win_t wt)
{
    return 0;
}

void _widget_set_focus(res_win_t wt)
{

}

bool_t _widget_key_state(res_win_t wt, int key)
{
	int ks;

	switch (key)
	{
	case KEY_SHIFT:
		break;
	case KEY_CONTROL:
		break;
	default:
		ks = 0;
	}

	return (ks & 0x8000) ? 1 : 0;
}

bool_t _widget_is_valid(res_win_t wt)
{
	return 0;
}

bool_t _widget_is_focus(res_win_t wt)
{
	return 0;
}

bool_t _widget_is_child(res_win_t wt)
{
    return 0;
}

bool_t _widget_is_ownc(res_win_t wt)
{
	return (GETXDKSTYLE(wt) & WD_STYLE_OWNERNC) ? 1 : 0;
}

void _widget_post_char(res_win_t wt, tchar_t ch)
{

}

void _widget_post_key(res_win_t wt, int key)
{

}

void _widget_move(res_win_t wt, const xpoint_t* ppt)
{

}

void _widget_size(res_win_t wt, const xsize_t* pxs)
{

}

void _widget_take(res_win_t wt, int zor)
{

}

void _widget_show(res_win_t wt, u32_t sw)
{
	
}

void _widget_invalid(res_win_t wt, const xrect_t* prt, bool_t b_erase)
{

}

void _widget_update(res_win_t wt, const xrect_t* prt, bool_t b_erase)
{

}

void _widget_update_window(res_win_t wt)
{

}

void _widget_update_client(res_win_t wt)
{

}

void _widget_enable(res_win_t wt, bool_t b)
{

}

void _widget_post_message(res_win_t wt, int msg, var_long wp, var_long lp)
{

}

int _widget_send_message(res_win_t wt, int msg, var_long wp, var_long lp)
{
    return 0;
}

void _widget_post_command(res_win_t wt, int code, uid_t cid, var_long data)
{

}

int _widget_send_command(res_win_t wt, int code, uid_t cid, var_long data)
{
    return 0;
}

void _widget_set_title(res_win_t wt, const tchar_t* token)
{

}

int _widget_get_title(res_win_t wt, tchar_t* buf, int max)
{
	return 0;
}

void _widget_get_scroll_info(res_win_t wt, bool_t horz, scroll_t* psl)
{

}

void _widget_set_scroll_info(res_win_t wt, bool_t horz, const scroll_t* psl)
{

}

void _widget_copy(res_win_t wt)
{

}

void _widget_cut(res_win_t wt)
{

}

void _widget_paste(res_win_t wt)
{

}

void _widget_undo(res_win_t wt)
{

}


//////////////////////////////////////////////////////////////////////////////////

int _widget_do_modal(res_win_t hWnd)
{
    return 0;
}

void _widget_do_trace(res_win_t hWnd)
{

}

/*********************************************************************************************************/
void _get_screen_size(xsize_t* pxs)
{

}

void _get_desktop_size(xsize_t* pxs)
{

}

void _screen_size_to_tm(xsize_t* pxs)
{

}

void _screen_size_to_pt(xsize_t* pxs)
{

}

res_acl_t _create_accel_table(const accel_t* pac, int n)
{
	return (res_acl_t)0;
}

void _destroy_accel_table(res_acl_t hac)
{

}

#ifdef XDK_SUPPORT_WIDGET_EX
void _widget_track_mouse(res_win_t wt, u32_t mask)
{

}

void _widget_set_region(res_win_t wt, res_rgn_t rgn)
{

}

void _widget_set_alpha(res_win_t wt, unsigned char b)
{

}

unsigned char _widget_get_alpha(res_win_t wt)
{
	return (unsigned char)0;
}
#endif /*XDK_SUPPORT_EX*/

#endif //XDK_SUPPORT_WIDGET
