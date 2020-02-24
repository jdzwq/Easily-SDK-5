/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc defination document

	@module	xdudef.h | interface file

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


#ifndef _XDUDEF_H
#define	_XDUDEF_H

#include <plf.h>

#if defined(_OS_WINDOWS)
#include "windows/_xdu_win.h"
#elif defined(_OS_MACOS)
#include "macos/_xdu_macos.h"
#elif defined(_OS_LINUX)
#include "linux/_xdu_linux.h"
#endif

#if defined(_USRDLL)
#if defined(_OS_WINDOWS)
#define XDU_API __declspec(dllexport)
#else
#define XDU_API __attribute__((visibility("default")))
#endif
#else
#define XDU_API extern
#endif

#define XPI			3.1415926535
//#define INCHPERTM	0.003937
#define INCHPERMM	0.03937
//#define TMPERINCH	254
#define MMPERINCH	25.4
//#define PTPERINCH	1440
//#define PTPERMM	56.693
#define MMPERLOGPT	0.1
#define MMPERPD		0.35
#define PDPERMM		2.835
#define PDPERINCH	72

#define ZERO_WIDTH				(float)0
#define ZERO_HEIGHT				(float)0


/*keyboard*/
#define KEY_SHIFT		0x10
#define KEY_CONTROL		0x11
#define KEY_ALT			0x12

#define KEY_BACK		8	//0x08
#define KEY_TAB			9	//0x09
#define KEY_ENTER		13	//0x0D
#define KEY_ESC			27	//0x1B
#define KEY_SPACE		32	//0x20
#define KEY_PAGEUP		33	//0x21
#define KEY_PAGEDOWN	34	//0x22
#define KEY_END			35	//0x23
#define KEY_HOME		36	//0x24
#define KEY_LEFT		37	//0x25
#define KEY_UP			38	//0x26
#define KEY_RIGHT		39	//0x27
#define KEY_DOWN		40	//0x28
#define KEY_INSERT		45	//0x2D
#define KEY_DELETE		46	//0x2E


/*widget class*/
#define XDUWIDGET		_T("XDUWIDGET")
/*widget store property*/
#define XDUSTRUCT		_T("XDUSTRUCT")
#define XDUDISPATCH		_T("XDUDISPATCH")
#define XDUSUBPROC		_T("XDUSUBPROC")
#define XDUUSERDELTA	_T("XDUUSERDELTA")
#define XDUCOREDELTA	_T("XDUCOREDELTA")
#define XDUSTYLE		_T("XDUSTYLE")
#define XDUACCEL		_T("XDUACCEL")
#define XDUOWNER		_T("XDUOWNER")
#define XDUUSERID		_T("XDUUSERID")
#define XDURESULT		_T("XDURESULT")
#define XDUGLRC			_T("XDUGLRC")

#define WIDGET_TITLE_SPAN		(float)10	//mm
#define WIDGET_MENU_SPAN		(float)7.5	//mm
#define WIDGET_SCROLL_SPAN		(float)5	//mm
#define WIDGET_ICON_SPAN		(float)3	//mm
#define WIDGET_FRAME_EDGE		(float)1.5	//mm
#define WIDGET_CHILD_EDGE		(float)0.5	//mm

/*widget alphablend level*/
#define ALPHA_SOLID			250
#define ALPHA_SOFT			128
#define ALPHA_TRANS			64

#define WM_NOTICE			(WM_EASYMSG_MIN + 1)
#define WM_SCROLL			(WM_EASYMSG_MIN + 2)

#define IDS_SUBCLASS_MIN		10
#define IDS_SUBCLASS_MAX		20

#define IDC_USERCTRL_MIN		100
#define IDC_USERCTRL_MAX		200

/*widget control identify*/
#define IDC_SELF			(IDC_USERCTRL_MIN - 1)
#define IDC_CHILD			(IDC_USERCTRL_MIN - 2)
#define IDC_PARENT			(IDC_USERCTRL_MIN - 3)
#define IDC_TIMER			(IDC_USERCTRL_MIN - 4)

/*widget states*/
#define WIDGET_NORMAL		0
#define WIDGET_HIDDEN		1
#define WIDGET_MINIMIZED	2
#define WIDGET_MAXIMIZED	3
#define WIDGET_FULLSCREEN	4
#define WIDGET_SHADED		5

/*widget command code*/
#define COMMAND_COLOR		1
#define COMMAND_TABSKIP		9
#define COMMAND_COMMIT		13
#define	COMMAND_ROLLBACK	27
#define COMMAND_UPDATE		11
#define COMMAND_CHANGE		12
#define COMMAND_COPY		20
#define COMMAND_CUT			21
#define COMMAND_PASTE		22
#define COMMAND_UNDO		23
#define COMMAND_QURYDRAG	40
#define COMMAND_QUERYDROP	41
#define COMMAND_QUERYINFO	42
#define COMMAND_FIND		43
#define COMMAND_REPLACE		44
#define COMMAND_RENAME		45
#define COMMAND_REMOVE		46

/*widget style*/
#define WD_STYLE_CHILD		0x00000001
#define WD_STYLE_EDITOR		0x00000002
#define WD_STYLE_DOCKER		0x00000004
#define WD_STYLE_HOTOVER	0x00000008

#define WD_STYLE_SIZEBOX	0x00000010
#define WD_STYLE_CLOSEBOX	0x00000020
#define WD_STYLE_HSCROLL	0x00000040
#define WD_STYLE_VSCROLL	0x00000080

#define WD_STYLE_TITLE		0x00000100
#define WD_STYLE_BORDER		0x00000200
#define WD_STYLE_OWNERNC	0x00000400
#define WD_STYLE_MENUBAR	0x00000800

#define WD_STYLE_PAGING		0x00001000
#define WD_STYLE_NOACTIVE	0x00002000
#define WD_STYLE_NOCHANGE	0x00004000
#define WD_STYLE_OPENGL		0x00008000

#define WD_STYLE_CONTROL	(WD_STYLE_CHILD | WD_STYLE_OWNERNC)
#define WD_STYLE_POPUP		(WD_STYLE_OWNERNC)
#define WD_STYLE_DIALOG		(WD_STYLE_TITLE | WD_STYLE_CLOSEBOX | WD_STYLE_BORDER | WD_STYLE_OWNERNC)
#define WD_STYLE_FRAME		(WD_STYLE_TITLE | WD_STYLE_CLOSEBOX | WD_STYLE_SIZEBOX | WD_STYLE_BORDER | WD_STYLE_OWNERNC)


/*widget show mode*/
#define WD_SHOW_NORMAL		0
#define WD_SHOW_HIDE		1
#define WD_SHOW_MAXIMIZE	2
#define WD_SHOW_MINIMIZE	3
#define WD_SHOW_FULLSCREEN	4
#define WD_SHOW_POPUPTOP	5

/*widget docking position*/
#define WD_DOCK_TOP			0x00000001
#define WD_DOCK_BOTTOM		0x00000002
#define WD_DOCK_LEFT		0x00000004
#define WD_DOCK_RIGHT		0x00000008
#define WD_DOCK_DYNA		0x00010000

/*pushbox style*/
#define WD_PUSHBOX_TEXT		0x00010000
#define WD_PUSHBOX_CHECK	0x00020000
#define	WD_PUSHBOX_ICON		0x00040000
#define WD_PUSHBOX_IMAGE	0x00080000

/*tab opera*/
#define WD_TAB_LEFT			0
#define WD_TAB_UP			1
#define WD_TAB_RIGHT		2
#define WD_TAB_DOWN			3
#define WD_TAB_END			4
#define	WD_TAB_HOME			5
#define WD_TAB_PAGEUP		6
#define WD_TAB_PAGEDOWN		7

/*widget opera state*/
#define WD_OPERA_NONE		0x00000000
#define WD_OPERA_CONTROL	0x00000001
#define WD_OPERA_SHIFT		0x00000002

/*widget layout position*/
#define WD_LAYOUT_LEFTTOP		1
#define WD_LAYOUT_RIGHTTOP		2
#define WD_LAYOUT_LEFTBOTTOM	3
#define WD_LAYOUT_RIGHTBOTTOM	4

//widget message button
#define MSGBTN_OK		0x00000001
#define MSGBTN_CANCEL	0x00000002
#define MSGBTN_YES		0x00000004
#define MSGBTN_NO		0x00000008
#define MSGBTN_KNOWN	0x00000010

//widget message icon
#define MSGICO_TIP		0x00010000
#define MSGICO_WRN		0x00020000
#define MSGICO_ERR		0x00040000

/*widget cursor identify*/
#define CURSOR_ARROW		0
#define CURSOR_WAIT			1
#define CURSOR_SIZENS		2
#define CURSOR_SIZEWE		3
#define CURSOR_SIZEALL		4
#define CURSOR_HAND			5
#define CURSOR_HELP			6
#define CURSOR_DRAG			7
#define CURSOR_IBEAM		8

/*widget icon identify*/
#define ICON_APPLICATION	_T("application")
#define ICON_QUESTION		_T("question")
#define ICON_EXCLAMATION	_T("exclamation")
#define ICON_INFORMATION	_T("information")
#define ICON_WARING			_T("waring")
#define ICON_ERROR			_T("error")
#define ICON_HAND			_T("hand")
#define ICON_ASTERISK		_T("asterisk")



typedef struct _accel_t{
	unsigned char vir;
	unsigned short key;
	unsigned short cmd;
}accel_t;

typedef struct _dev_cap_t{
	int horz_res, vert_res;
	int horz_pixels, vert_pixels;
	int horz_feed, vert_feed;
	int horz_size, vert_size;
}dev_cap_t;

typedef struct _dev_prn_t{
	short paper_width, paper_height;
	short landscape;
}dev_prn_t;


typedef enum{ _RGB_COLOR, HSL_COLOR, _HEX_COLOR }CLRFMT;

#define RGB_GRAY(r,g,b) (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b)

typedef struct _yuv_color_t{
	unsigned char y, u, v;
}yuv_color_t;

typedef struct _xcolor_t{
	unsigned char r, g, b;
}xcolor_t;

typedef struct _clr_mod_t{
	xcolor_t clr_bkg;
	xcolor_t clr_frg;
	xcolor_t clr_txt;
	xcolor_t clr_msk;
	xcolor_t clr_ico;
}clr_mod_t;


typedef struct _xrect_t{
	union{
		int x;
		float fx;
	};
	union{
		int y;
		float fy;
	};
	union{
		int w;
		float fw;
	};
	union{
		int h;
		float fh;
	};
}xrect_t;

typedef struct _xpoint_t{
	union{
		int x;
		float fx;
	};
	union{
		int y;
		float fy;
	};
}xpoint_t;

typedef struct _xsize_t{
	union{
		int cx;
		float fx;
	};
	union{
		int cy;
		float fy;
	};
}xsize_t;

#define RECTPOINT(pxr)	((xpoint_t*)pxr)
#define RECTSIZE(pxr)	((xsize_t*)pxr + 1)

typedef struct _shadow_t{
	int offx;
	int offy;
}shadow_t;

typedef struct _adorn_t{
	int feed;
	int size;
}adorn_t;

typedef struct _xgradi_t{
	tchar_t type[64];
	tchar_t opacity[16];
	tchar_t brim_color[16];
	tchar_t core_color[16];
}xgradi_t;

typedef struct _xbrush_t{
	tchar_t style[64];
	tchar_t opacity[16];
	tchar_t color[16];
	shadow_t shadow;
}xbrush_t;

typedef struct _xpen_t{
	tchar_t style[64];
	tchar_t size[16];
	tchar_t opacity[16];
	tchar_t color[16];
	adorn_t adorn;
}xpen_t;

typedef struct _xfont_t{
	tchar_t style[64];
	tchar_t size[16];
	tchar_t weight[16];
	tchar_t family[64];
	tchar_t color[16];
}xfont_t;

typedef struct _xface_t{
	tchar_t text_wrap[64];
	tchar_t text_align[64];
	tchar_t line_align[64];
	tchar_t line_height[16];
}xface_t;

typedef struct _ximage_t{
	tchar_t style[64];
	tchar_t type[64];
	tchar_t color[16];

	const tchar_t* source;
}ximage_t;


typedef struct _border_t{
	int title;
	int edge;
	int hscroll;
	int vscroll;
	int menu;
	int icon;
}border_t;

typedef struct _scroll_t{
	int pos;
	int min;
	int max;
	int page;
	int track;
}scroll_t;

typedef struct _str_find_t{
	bool_t b_case;
	bool_t b_back;
	const tchar_t* sub_str;
	bool_t b_none;
}str_find_t;

typedef struct _str_replace_t
{
	bool_t b_case;
	bool_t b_back;
	const tchar_t* org_str;
	const tchar_t* new_str;
	bool_t b_none;
}str_replace_t;

#ifdef XDU_SUPPORT_WIDGET

typedef int(STDCALL *PF_ENUM_WINDOW_PROC)(res_win_t widget, var_long pv);

/*define widget notify header*/
typedef struct _NOTICE{
	res_win_t widget;
	unsigned int id;
	unsigned int code;
}NOTICE, *LPNOTICE;

/*subclass widget event*/
typedef int(*SUB_ON_LBUTTON_DOWN)(res_win_t, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_LBUTTON_UP)(res_win_t, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_LBUTTON_DBCLICK)(res_win_t, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_RBUTTON_DOWN)(res_win_t, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_RBUTTON_UP)(res_win_t, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_MOUSE_MOVE)(res_win_t, unsigned int, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_MOUSE_HOVER)(res_win_t, unsigned int, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_MOUSE_LEAVE)(res_win_t, unsigned int, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_WHEEL)(res_win_t, bool_t, int, uid_t, var_long);
typedef int(*SUB_ON_SCROLL)(res_win_t, bool_t, int, uid_t, var_long);
typedef int(*SUB_ON_KEYDOWN)(res_win_t, int, uid_t, var_long);
typedef int(*SUB_ON_CHAR)(res_win_t, tchar_t, uid_t, var_long);
typedef int(*SUB_ON_SIZE)(res_win_t, int, const xsize_t*, uid_t, var_long);
typedef int(*SUB_ON_MOVE)(res_win_t, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_SHOW)(res_win_t, bool_t, uid_t, var_long);
typedef int(*SUB_ON_PAINT)(res_win_t, res_ctx_t, const xrect_t*, uid_t, var_long);
typedef int(*SUB_ON_ERASE)(res_win_t, res_ctx_t, uid_t, var_long);
typedef int(*SUB_ON_SET_FOCUS)(res_win_t, res_win_t, uid_t, var_long);
typedef int(*SUB_ON_KILL_FOCUS)(res_win_t, res_win_t, uid_t, var_long);
typedef int(*SUB_ON_ENABLE)(res_win_t, bool_t, uid_t, var_long);
typedef int(*SUB_ON_NOTICE)(res_win_t, NOTICE*, uid_t, var_long);
typedef int(*SUB_ON_MENU_COMMAND)(res_win_t, int, int, var_long, uid_t, var_long);
typedef int(*SUB_ON_PARENT_COMMAND)(res_win_t, int, var_long, uid_t, var_long);
typedef int(*SUB_ON_CHILD_COMMAND)(res_win_t, int, var_long, uid_t, var_long);
typedef int(*SUB_ON_SELF_COMMAND)(res_win_t, int, var_long, uid_t, var_long);
typedef int(*SUB_ON_COMMAND_FIND)(res_win_t, str_find_t*, uid_t, var_long);
typedef int(*SUB_ON_COMMAND_REPLACE)(res_win_t, str_replace_t*, uid_t, var_long);
typedef int(*SUB_ON_TIMER)(res_win_t, var_long, uid_t, var_long);
typedef int(*SUB_ON_COPY)(res_win_t, uid_t, var_long);
typedef int(*SUB_ON_CUT)(res_win_t, uid_t, var_long);
typedef int(*SUB_ON_PASTE)(res_win_t, uid_t, var_long);
typedef int(*SUB_ON_UNDO)(res_win_t, uid_t, var_long);

typedef int(*SUB_ON_SYSCLR_CLICK)(res_win_t, const xpoint_t*, uid_t, var_long);
typedef int(*SUB_ON_SYSLOG_CLICK)(res_win_t, const xpoint_t*, uid_t, var_long);

typedef int(*SUB_ON_CLOSE)(res_win_t, uid_t, var_long);
typedef void(*SUB_ON_SUBBING)(res_win_t, uid_t, var_long);
typedef void(*SUB_ON_UNSUBBING)(res_win_t, uid_t, var_long);

typedef struct _if_subproc_t{
	SUB_ON_SUBBING		sub_on_subbing;
	SUB_ON_UNSUBBING	sub_on_unsubbing;
	SUB_ON_CLOSE		sub_on_close;

	SUB_ON_LBUTTON_DOWN	sub_on_lbutton_down;
	SUB_ON_LBUTTON_UP	sub_on_lbutton_up;
	SUB_ON_LBUTTON_DBCLICK	sub_on_lbutton_dbclick;
	SUB_ON_RBUTTON_DOWN	sub_on_rbutton_down;
	SUB_ON_RBUTTON_UP	sub_on_rbutton_up;
	SUB_ON_MOUSE_MOVE	sub_on_mouse_move;
	SUB_ON_MOUSE_HOVER	sub_on_mouse_hover;
	SUB_ON_MOUSE_LEAVE	sub_on_mouse_leave;
	SUB_ON_WHEEL		sub_on_wheel;
	SUB_ON_SCROLL		sub_on_scroll;
	SUB_ON_KEYDOWN		sub_on_keydown;
	SUB_ON_CHAR			sub_on_char;
	SUB_ON_SIZE			sub_on_size;
	SUB_ON_MOVE			sub_on_move;
	SUB_ON_SHOW			sub_on_show;
	SUB_ON_ENABLE		sub_on_enable;
	SUB_ON_SET_FOCUS	sub_on_set_focus;
	SUB_ON_KILL_FOCUS	sub_on_kill_focus;
	SUB_ON_PAINT		sub_on_paint;
	SUB_ON_ERASE		sub_on_erase;
	SUB_ON_NOTICE		sub_on_notice;
	SUB_ON_MENU_COMMAND		sub_on_menu_command;
	SUB_ON_PARENT_COMMAND	sub_on_parent_command;
	SUB_ON_CHILD_COMMAND	sub_on_child_command;
	SUB_ON_SELF_COMMAND		sub_on_self_command;
	SUB_ON_COMMAND_FIND		sub_on_command_find;
	SUB_ON_COMMAND_REPLACE	sub_on_command_replace;

	SUB_ON_SYSCLR_CLICK		sub_on_sysclr_click;
	SUB_ON_SYSLOG_CLICK		sub_on_syslog_click;

	SUB_ON_TIMER		sub_on_timer;
	SUB_ON_COPY			sub_on_copy;
	SUB_ON_CUT			sub_on_cut;
	SUB_ON_PASTE		sub_on_paste;
	SUB_ON_UNDO			sub_on_undo;

	void* proc;
	uid_t sid;
	var_long delta;
}if_subproc_t;

typedef void(*PF_ON_NCPAINT)(res_win_t, res_ctx_t);
typedef void(*PF_ON_NCCALCSIZE)(res_win_t, xrect_t*);
typedef int(*PF_ON_NCHITTEST)(res_win_t, const xpoint_t*);
typedef int(*PF_ON_NCCALCSCROLL)(res_win_t, bool_t, const xpoint_t*);

typedef int(*PF_ON_CREATE)(res_win_t, void*);
typedef int(*PF_ON_CLOSE)(res_win_t);
typedef void(*PF_ON_DESTROY)(res_win_t);
typedef void(*PF_ON_LBUTTON_DOWN)(res_win_t, const xpoint_t*);
typedef void(*PF_ON_LBUTTON_UP)(res_win_t, const xpoint_t*);
typedef void(*PF_ON_LBUTTON_DBCLICK)(res_win_t, const xpoint_t*);
typedef void(*PF_ON_RBUTTON_DOWN)(res_win_t, const xpoint_t*);
typedef void(*PF_ON_RBUTTON_UP)(res_win_t, const xpoint_t*);
typedef void(*PF_ON_MOUSE_MOVE)(res_win_t, dword_t, const xpoint_t*);
typedef void(*PF_ON_MOUSE_HOVER)(res_win_t, dword_t, const xpoint_t*);
typedef void(*PF_ON_MOUSE_LEAVE)(res_win_t, dword_t, const xpoint_t*);
typedef void(*PF_ON_WHEEL)(res_win_t, bool_t, int);
typedef void(*PF_ON_SCROLL)(res_win_t, bool_t, int);
typedef void(*PF_ON_KEYDOWN)(res_win_t, int);
typedef void(*PF_ON_CHAR)(res_win_t, tchar_t);
typedef void(*PF_ON_SIZE)(res_win_t, int, const xsize_t*);
typedef void(*PF_ON_MOVE)(res_win_t, const xpoint_t*);
typedef void(*PF_ON_SHOW)(res_win_t, bool_t);
typedef void(*PF_ON_ACTIVATE)(res_win_t, int);
typedef void(*PF_ON_PAINT)(res_win_t, res_ctx_t, const xrect_t*);
typedef void(*PF_ON_ERASE)(res_win_t, res_ctx_t);
typedef void(*PF_ON_ENABLE)(res_win_t, bool_t);
typedef void(*PF_ON_SET_FOCUS)(res_win_t, res_win_t);
typedef void(*PF_ON_KILL_FOCUS)(res_win_t, res_win_t);
typedef void(*PF_ON_NOTICE)(res_win_t, NOTICE*);
typedef void(*PF_ON_MENU_COMMAND)(res_win_t, int, int, var_long);
typedef void(*PF_ON_PARENT_COMMAND)(res_win_t, int, var_long);
typedef void(*PF_ON_CHILD_COMMAND)(res_win_t, int, var_long);
typedef void(*PF_ON_SELF_COMMAND)(res_win_t, int, var_long);
typedef void(*PF_ON_COMMAND_FIND)(res_win_t, str_find_t*);
typedef void(*PF_ON_COMMAND_REPLACE)(res_win_t, str_replace_t*);
typedef void(*PF_ON_SYSCMD_CLICK)(res_win_t, const xpoint_t*);
typedef void(*PF_ON_TIMER)(res_win_t, var_long);
typedef void(*PF_ON_COPY)(res_win_t);
typedef void(*PF_ON_CUT)(res_win_t);
typedef void(*PF_ON_PASTE)(res_win_t);
typedef void(*PF_ON_UNDO)(res_win_t);

/*widget event*/
typedef struct _if_event_t{
	PF_ON_NCPAINT		pf_on_nc_paint;
	PF_ON_NCCALCSIZE	pf_on_nc_calcsize;
	PF_ON_NCHITTEST		pf_on_nc_hittest;
	PF_ON_NCCALCSCROLL	pf_on_nc_calcscroll;

	PF_ON_CREATE		pf_on_create;
	PF_ON_CLOSE			pf_on_close;
	PF_ON_DESTROY		pf_on_destroy;
	PF_ON_LBUTTON_DOWN	pf_on_lbutton_down;
	PF_ON_LBUTTON_UP	pf_on_lbutton_up;
	PF_ON_LBUTTON_DBCLICK	pf_on_lbutton_dbclick;
	PF_ON_RBUTTON_DOWN	pf_on_rbutton_down;
	PF_ON_RBUTTON_UP	pf_on_rbutton_up;
	PF_ON_MOUSE_MOVE	pf_on_mouse_move;
	PF_ON_MOUSE_HOVER	pf_on_mouse_hover;
	PF_ON_MOUSE_LEAVE	pf_on_mouse_leave;
	PF_ON_WHEEL			pf_on_wheel;
	PF_ON_SCROLL		pf_on_scroll;
	PF_ON_KEYDOWN		pf_on_keydown;
	PF_ON_CHAR			pf_on_char;
	PF_ON_SIZE			pf_on_size;
	PF_ON_MOVE			pf_on_move;
	PF_ON_SHOW			pf_on_show;
	PF_ON_ACTIVATE		pf_on_activate;
	PF_ON_ENABLE		pf_on_enable;
	PF_ON_SET_FOCUS		pf_on_set_focus;
	PF_ON_KILL_FOCUS	pf_on_kill_focus;
	PF_ON_PAINT			pf_on_paint;
	PF_ON_ERASE			pf_on_erase;
	PF_ON_NOTICE		pf_on_notice;
	PF_ON_MENU_COMMAND		pf_on_menu_command;
	PF_ON_PARENT_COMMAND	pf_on_parent_command;
	PF_ON_CHILD_COMMAND		pf_on_child_command;
	PF_ON_SELF_COMMAND		pf_on_self_command;
	PF_ON_COMMAND_FIND		pf_on_command_find;
	PF_ON_COMMAND_REPLACE	pf_on_command_replace;

	PF_ON_SYSCMD_CLICK		pf_on_syscmd_click;

	PF_ON_TIMER			pf_on_timer;
	PF_ON_COPY			pf_on_copy;
	PF_ON_CUT			pf_on_cut;
	PF_ON_PASTE			pf_on_paste;
	PF_ON_UNDO			pf_on_undo;

	void* param;
}if_event_t;

#endif /*XDU_SUPPORT_WIDGET*/

#endif	/* _XDUDEF_H */

