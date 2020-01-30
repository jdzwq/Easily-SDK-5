/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	xdcdef.h | interface file

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


#ifndef _XDCDEF_H
#define	_XDCDEF_H

#include <xdl.h>


#if defined(_USRDLL)
#if defined(_OS_WINDOWS)
#define XDC_API __declspec(dllexport)
#else
#define XDC_API __attribute__((visibility("default")))
#endif
#else
#define XDC_API extern
#endif

#define XDCKEYBOX			_T("XDCKEYBOX")

/*reserved subproc id*/
#define IDS_DIALOG			(IDS_SUBCLASS_MIN + 1)
#define IDS_DATEBOX			(IDS_SUBCLASS_MIN + 2)
#define IDS_TIMEBOX			(IDS_SUBCLASS_MIN + 3)
#define IDS_KEYBOX			(IDS_SUBCLASS_MIN + 4)
#define IDS_NUMBOX			(IDS_SUBCLASS_MIN + 5)
#define IDS_EDITBOX			(IDS_SUBCLASS_MIN + 6)
#define IDS_DROPBOX			(IDS_SUBCLASS_MIN + 7)
#define IDS_WORDSBOX		(IDS_SUBCLASS_MIN + 8)
#define IDS_CHECKBOX		(IDS_SUBCLASS_MIN + 9)

/*reserved control id*/
#define IDC_FIREEDIT		(IDC_USERCTRL_MIN + 1)
#define IDC_FIRELIST		(IDC_USERCTRL_MIN + 2)
#define IDC_FIREDATE		(IDC_USERCTRL_MIN + 3)
#define IDC_FIRETIME		(IDC_USERCTRL_MIN + 4)
#define IDC_FIRENUM			(IDC_USERCTRL_MIN + 5)
#define IDC_FIREWORDS		(IDC_USERCTRL_MIN + 6)
#define IDC_FIRECHECK		(IDC_USERCTRL_MIN + 7)
#define IDC_FIREGRID		(IDC_USERCTRL_MIN + 8)

#define IDC_EDITBOX			(IDC_USERCTRL_MIN + 11)
#define IDC_DATEBOX			(IDC_USERCTRL_MIN + 12)
#define IDC_TIMEBOX			(IDC_USERCTRL_MIN + 13)
#define IDC_DROPBOX			(IDC_USERCTRL_MIN + 14)
#define IDC_KEYBOX			(IDC_USERCTRL_MIN + 15)
#define IDC_NUMBOX			(IDC_USERCTRL_MIN + 16)
#define IDC_WORDSBOX		(IDC_USERCTRL_MIN + 17)
#define IDC_CHECKBOX		(IDC_USERCTRL_MIN + 18)
#define IDC_RICHBOX			(IDC_USERCTRL_MIN + 19)
#define IDC_MEMOBOX			(IDC_USERCTRL_MIN + 20)
#define IDC_TAGBOX			(IDC_USERCTRL_MIN + 21)
#define IDC_PHOTOBOX		(IDC_USERCTRL_MIN + 22)
#define IDC_IMAGESBOX		(IDC_USERCTRL_MIN + 23)
#define IDC_GRIDBOX			(IDC_USERCTRL_MIN + 24)
#define IDC_STATISBOX		(IDC_USERCTRL_MIN + 25)
#define IDC_TABLEBOX		(IDC_USERCTRL_MIN + 26)
#define IDC_FORMBOX			(IDC_USERCTRL_MIN + 27)
#define IDC_MENUBOX			(IDC_USERCTRL_MIN + 28)

#define IDC_EDITMENU		(IDC_USERCTRL_MIN + 29)
#define IDC_TOOLTIP			(IDC_USERCTRL_MIN + 30)
#define IDC_VERTBOX			(IDC_USERCTRL_MIN + 31)
#define IDC_HORZBOX			(IDC_USERCTRL_MIN + 32)

#define IDC_MSGDLG			(IDC_USERCTRL_MIN + 40)
#define IDC_PROEPRDLG		(IDC_USERCTRL_MIN + 41)
#define IDC_TABLEDLG		(IDC_USERCTRL_MIN + 42)
#define IDC_PROPERDLG		(IDC_USERCTRL_MIN + 43)
#define IDC_LISTDLG			(IDC_USERCTRL_MIN + 44)
#define IDC_ANNODLG			(IDC_USERCTRL_MIN + 45)
#define IDC_INPUTDLG		(IDC_USERCTRL_MIN + 46)
#define IDC_TEXTDLG			(IDC_USERCTRL_MIN + 47)
#define IDC_PREVIEWDLG		(IDC_USERCTRL_MIN + 48)
#define IDC_GRIDDLG			(IDC_USERCTRL_MIN + 49)

#define IS_VISIBLE_CHAR(ch) (((nChar >= _T('0') && nChar <= _T('9')) || (nChar >= _T('A') && nChar <= _T('Z')) || (nChar >= _T('a') && nChar <= _T('z')) || (nChar == _T('-')) || (nChar == _T('.')))? 1 : 0)
#define IS_ASCII_CHAR(ch) ((ch >= 32 && ch < 127)? 1 : 0)

#define DEF_TIPTIME		1500

#define DEF_INNER_FEED		(long)-1
#define DEF_OUTER_FEED		(long)1
#define DEF_FOCUS_SPAN		(long)5

typedef struct _PAGE_CALC{
	long total_width;
	long total_height;
	long page_width;
	long page_height;
	long line_width;
	long line_height;
}PAGE_CALC;


typedef struct _DOCKINFO{
	dword_t style;
	long cx, cy;
}DOCKINFO;

typedef struct _docker_t{
	res_win_t widget;
	DOCKINFO dock[4];
	long x, y;
	int ind;
	bool_t drag;
}docker_t;

typedef struct _splitor_t{
	res_win_t widget;
	link_t_ptr split;
	link_t_ptr item;
	long x, y;
}splitor_t;

typedef struct _widget_struct_t{
	int result;

	xpoint_t pt;
	xrect_t vb;
	xrect_t cb;
	xsize_t sc;

	link_t_ptr menu;
	canvas_t canv;

	union{
		splitor_t splitor;
		docker_t docker;
	};

	xfont_t xf;
	xface_t xa;
	xbrush_t xb;
	xpen_t xp;
	xcolor_t msk;
	xcolor_t ico;
}widget_struct_t;

#define GETWIDGSTRUCT(wt)			(widget_struct_t*)widget_get_core_delta(wt)
#define SETWIDGSTRUCT(wt, lp)		widget_set_core_delta(wt, (var_int)lp)



#define EVENT_BEGIN_DISPATH(pv)			{if_event_t* pev = pv;
#define EVENT_ON_NCPAINT(proc)			pev->pf_on_nc_paint = proc;
#define EVENT_ON_NCCALCSIZE(proc)		pev->pf_on_nc_calcsize = proc;
#define EVENT_ON_NCHITTEST(proc)		pev->pf_on_nc_hittest = proc;
#define EVENT_ON_CREATE(proc)			pev->pf_on_create = proc;
#define EVENT_ON_DESTROY(proc)			pev->pf_on_destroy = proc;
#define EVENT_ON_CLOSE(proc)			pev->pf_on_close = proc;
#define EVENT_ON_LBUTTON_DOWN(proc)		pev->pf_on_lbutton_down = proc;
#define EVENT_ON_LBUTTON_UP(proc)		pev->pf_on_lbutton_up = proc;
#define EVENT_ON_LBUTTON_DBCLICK(proc)	pev->pf_on_lbutton_dbclick = proc;
#define EVENT_ON_RBUTTON_DOWN(proc)		pev->pf_on_rbutton_down = proc;
#define EVENT_ON_RBUTTON_UP(proc)		pev->pf_on_rbutton_up = proc;
#define EVENT_ON_MOUSE_MOVE(proc)		pev->pf_on_mouse_move = proc;
#define EVENT_ON_MOUSE_HOVER(proc)		pev->pf_on_mouse_hover = proc;
#define EVENT_ON_MOUSE_LEAVE(proc)		pev->pf_on_mouse_leave = proc;
#define EVENT_ON_WHEEL(proc)			pev->pf_on_wheel = proc;
#define EVENT_ON_SCROLL(proc)			pev->pf_on_scroll = proc;
#define EVENT_ON_KEYDOWN(proc)			pev->pf_on_keydown = proc;
#define EVENT_ON_CHAR(proc)				pev->pf_on_char = proc;
#define EVENT_ON_SIZE(proc)				pev->pf_on_size = proc;
#define EVENT_ON_MOVE(proc)				pev->pf_on_move = proc;
#define EVENT_ON_SHOW(proc)				pev->pf_on_show = proc;
#define EVENT_ON_ENABLE(proc)			pev->pf_on_enable = proc;
#define EVENT_ON_SET_FOCUS(proc)		pev->pf_on_set_focus = proc;
#define EVENT_ON_KILL_FOCUS(proc)		pev->pf_on_kill_focus = proc;
#define EVENT_ON_PAINT(proc)			pev->pf_on_paint = proc;
#define EVENT_ON_ERASE(proc)			pev->pf_on_erase = proc;
#define EVENT_ON_NOTICE(proc)			pev->pf_on_notice = proc;
#define EVENT_ON_MENU_COMMAND(proc)		pev->pf_on_menu_command = proc;
#define EVENT_ON_PARENT_COMMAND(proc)	pev->pf_on_parent_command = proc;
#define EVENT_ON_CHILD_COMMAND(proc)	pev->pf_on_child_command = proc;
#define EVENT_ON_COMMAND_FIND(proc)		pev->pf_on_command_find = proc;
#define EVENT_ON_COMMAND_REPLACE(proc)	pev->pf_on_command_replace = proc;
#define EVENT_ON_SELF_COMMAND(proc)		pev->pf_on_self_command = proc;
#define EVENT_ON_SYSCMD_CLICK(proc)		pev->pf_on_syscmd_click = proc;
#define EVENT_ON_TIMER(proc)			pev->pf_on_timer = proc;
#define EVENT_ON_COPY(proc)				pev->pf_on_copy = proc;
#define EVENT_ON_CUT(proc)				pev->pf_on_cut = proc;
#define EVENT_ON_PASTE(proc)			pev->pf_on_paste = proc;
#define EVENT_ON_UNDO(proc)				pev->pf_on_undo = proc;
#define EVENT_ON_NC_IMPLEMENT			{pev->pf_on_nc_paint = widgetnc_on_paint;pev->pf_on_nc_calcsize = widgetnc_on_calcsize;pev->pf_on_nc_hittest = widgetnc_on_hittest;pev->pf_on_nc_calcscroll = widgetnc_on_calcscroll;}
#define EVENT_ON_SPLITOR_IMPLEMENT		{pev->pf_on_mouse_move = widgetex_splitor_on_mousemove;pev->pf_on_lbutton_down=widgetex_splitor_on_lbuttondown;pev->pf_on_lbutton_up=widgetex_splitor_on_lbuttonup;pev->pf_on_size=widgetex_splitor_on_size;pev->pf_on_paint=widgetex_splitor_on_paint;}
#define EVENT_ON_DOCKER_IMPLEMENT		{pev->pf_on_mouse_move = widgetex_docker_on_mousemove;pev->pf_on_lbutton_down=widgetex_docker_on_lbuttondown;pev->pf_on_lbutton_up=widgetex_docker_on_lbuttonup;pev->pf_on_paint=widgetex_docker_on_paint;}
#define EVENT_END_DISPATH				}



#define SUBPROC_BEGIN_DISPATH(pv)			{if_subproc_t* pev = pv;
#define SUBPROC_ON_SUBBING(proc)			pev->sub_on_subbing = proc;
#define SUBPROC_ON_UNSUBBING(proc)			pev->sub_on_unsubbing = proc;
#define SUBPROC_ON_CLOSE(proc)				pev->sub_on_close = proc;
#define SUBPROC_ON_LBUTTON_DOWN(proc)		pev->sub_on_lbutton_down = proc;
#define SUBPROC_ON_LBUTTON_UP(proc)			pev->sub_on_lbutton_up = proc;
#define SUBPROC_ON_LBUTTON_DBCLICK(proc)	pev->sub_on_lbutton_dbclick = proc;
#define SUBPROC_ON_RBUTTON_DOWN(proc)		pev->sub_on_rbutton_down = proc;
#define SUBPROC_ON_RBUTTON_UP(proc)			pev->sub_on_rbutton_up = proc;
#define SUBPROC_ON_MOUSE_MOVE(proc)			pev->sub_on_mouse_move = proc;
#define SUBPROC_ON_MOUSE_HOVER(proc)		pev->sub_on_mouse_hover = proc;
#define SUBPROC_ON_MOUSE_LEAVE(proc)		pev->sub_on_mouse_leave = proc;
#define SUBPROC_ON_WHEEL(proc)				pev->sub_on_wheel = proc;
#define SUBPROC_ON_SCROLL(proc)				pev->sub_on_scroll = proc;
#define SUBPROC_ON_KEYDOWN(proc)			pev->sub_on_keydown = proc;
#define SUBPROC_ON_CHAR(proc)				pev->sub_on_char = proc;
#define SUBPROC_ON_SIZE(proc)				pev->sub_on_size = proc;
#define SUBPROC_ON_MOVE(proc)				pev->sub_on_move = proc;
#define SUBPROC_ON_SHOW(proc)				pev->sub_on_show = proc;
#define SUBPROC_ON_ENABLE(proc)				pev->sub_on_enable = proc;
#define SUBPROC_ON_SET_FOCUS(proc)			pev->sub_on_set_focus = proc;
#define SUBPROC_ON_KILL_FOCUS(proc)			pev->sub_on_kill_focus = proc;
#define SUBPROC_ON_PAINT(proc)				pev->sub_on_paint = proc;
#define SUBPROC_ON_ERASE(proc)				pev->sub_on_erase = proc;
#define SUBPROC_ON_NOTICE(proc)				pev->sub_on_notice = proc;
#define SUBPROC_ON_MENU_COMMAND(proc)		pev->sub_on_menu_command = proc;
#define SUBPROC_ON_PARENT_COMMAND(proc)		pev->sub_on_parent_command = proc;
#define SUBPROC_ON_CHILD_COMMAND(proc)		pev->sub_on_child_command = proc;
#define SUBPROC_ON_SELF_COMMAND(proc)		pev->sub_on_self_command = proc;
#define SUBPROC_ON_COMMAND_FIND(proc)		pev->sub_on_command_find = proc;
#define SUBPROC_ON_COMMAND_REPLACE(proc)	pev->sub_on_command_replace = proc;
#define SUBPROC_ON_SELF_COMMAND(proc)		pev->sub_on_self_command = proc;
#define SUNPROC_ON_SYSCMD_CLICK(proc)		pev->sub_on_syscmd_click = proc;
#define SUBPROC_ON_TIMER(proc)				pev->sub_on_timer = proc;
#define SUBPROC_ON_SPLITOR_IMPLEMENT		{pev->sub_on_mouse_move = widgetex_splitor_sub_mousemove;pev->sub_on_lbutton_down=widgetex_splitor_sub_lbuttondown;pev->sub_on_lbutton_up=widgetex_splitor_sub_lbuttonup;pev->sub_on_size=widgetex_splitor_sub_size;pev->sub_on_paint=widgetex_splitor_sub_paint;}
#define SUBPROC_ON_DOCKER_IMPLEMENT			{pev->sub_on_mouse_move = widgetex_docker_sub_mousemove;pev->sub_on_lbutton_down=widgetex_docker_sub_lbuttondown;pev->sub_on_lbutton_up=widgetex_docker_sub_lbuttonup;pev->sub_on_paint=widgetex_docker_sub_paint;}
#define SUBPROC_ON_DIALOG_IMPLEMENT			{pev->sub_on_paint=sub_dialog_on_paint;pev->sub_on_size=sub_dialog_on_size;}
#define SUBPROC_END_DISPATH					}


#define DEF_CTRL_BRUSH		_T("fill-style:solid;fill-color:RGB(250,250,250);fill-opacity:250;")
#define DEF_CTRL_PEN		_T("stroke-style:solid;stroke-color:RGB(168,168,168);stroke-width:1;stroke-opacity:250;")
#define DEF_CTRL_FONT		_T("font-style:normal;font-size:9;font-family:Arial;font-weight:400;")
#define DEF_CTRL_FACE		_T("text-color:RGB(10,10,10);text-align:near;line-align:center;text-wrap:line-barek;")

#define DEF_ALARM_COLOR		_T("RGB(178,34,34)")
#define DEF_ENABLE_COLOR	_T("RGB(36,36,36)")
#define DEF_DISABLE_COLOR	_T("RGB(198,198,198)")
#define DEF_ALPHA_COLOR		_T("RGB(152,185,158)")

#define TEXT_STYLE_YELLOW	_T("text-color:RGB(160,82,45)")
#define TEXT_STYLE_PURPLE	_T("text-color:RGB(139,0,139);")
#define TEXT_STYLE_BLUE		_T("text-color:RGB(0,0,139);")
#define TEXT_STYLE_GREEN	_T("text-color:RGB(0,158,0);")
#define TEXT_STYLE_CYAN		_T("text-color:RGB(47,79,79);")
#define TEXT_STYLE_RED		_T("text-color:RGB(198,0,0);")
#define TEXT_STYLE_ORANGE	_T("text-color:RGB(250,127,0);")
#define TEXT_STYLE_SLATE	_T("text-color:RGB(108,123,139);")
#define TEXT_STYLE_GRAY		_T("text-color:RGB(168,168,168);")

#define TEXTOR_MENU_UNDO			_T("Cancel")
#define TEXTOR_MENU_CUT				_T("Cut")
#define TEXTOR_MENU_PASTE			_T("Paste")
#define TEXTOR_MENU_COPY			_T("Copy")

#define MSGDLG_PUSHBOX_CLOSE		_T("Close")
#define MSGDLG_PUSHBOX_OK			_T("OK")
#define MSGDLG_PUSHBOX_CANCEL		_T("Cancel")
#define MSGDLG_PUSHBOX_YES			_T("Yes")
#define MSGDLG_PUSHBOX_NO			_T("No")
#define MSGDLG_PUSHBOX_KNOWN		_T("No Tip Next")

#define MSGDLG_TITLE_TIP			_T("Tip")
#define MSGDLG_TITLE_WRN			_T("Warning")
#define MSGDLG_TITLE_ERR			_T("Error")

#define LISTDLG_PUSHBOX_OK			_T("OK")

#define ANNODLG_PUSHBOX_OK			_T("OK")
#define ANNODLG_PUSHBOX_COMMIT		_T("Commit")
#define ANNODLG_PUSHBOX_FONTSIZE	_T("FontSize")
#define ANNODLG_PUSHBOX_FONTCOLOR	_T("FontColor")

#define PREVIEWDLG_TREE_TITLE		_T("Title")
#define PREVIEWDLG_TREE_DEFITEM		_T("SVG Item")
#define PREVIEWDLG_STATUS_PAGEGUID	_T("Page%d %dPages")
#define PREVIEWDLG_PUSHBOX_CLOSE	_T("Close")
#define PREVIEWDLG_PUSHBOX_PRINTCUR	_T("Print Current")
#define PREVIEWDLG_PUSHBOX_PRINTSEL	_T("Print Selected")
#define PREVIEWDLG_PUSHBOX_PRINTALL	_T("Print All")
#define PREVIEWDLG_PUSHBOX_SETUP	_T("Printer Setup")
#define PREVIEWDLG_PUSHBOX_SAVEAS	_T("Save As")
#define PREVIEWDLG_PUSHBOX_SAVE		_T("Save")
#define PREVIEWDLG_PUSHBOX_OPEN		_T("Open")

#define PROPERDLG_PUSHBOX_OK		_T("OK")
#define GRIDDLG_PUSHBOX_OK			_T("OK")
#define TABLEDLG_PUSHBOX_OK			_T("OK")
#define TEXTDLG_PUSHBOX_OK			_T("OK")

#ifndef LAN_CN
#define LAN_CN
#endif

#ifdef _OS_WINDOWS
#include "lang\_xdc_ansi.h"
#endif

#endif	/* _XDCDEF_H */

