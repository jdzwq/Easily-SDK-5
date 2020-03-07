/***********************************************************************
	Easily xdu v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdu linux definition document

	@module	_xdl_linux.h | linux definition interface file

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

#ifndef _XDU_LINUX_H
#define _XDU_LINUX_H

#define XDU_SUPPORT_SHELL
#define XDU_SUPPORT_CONTEXT
#define XDU_SUPPORT_CONTEXT_REGION
#define XDU_SUPPORT_CONTEXT_BITMAP
#define XDU_SUPPORT_CONTEXT_GRAPHIC

#define XDU_SUPPORT_WIDGET

#ifdef XDU_SUPPORT_CONTEXT
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include <X11/cursorfont.h>
#endif


#ifdef XDU_SUPPORT_CONTEXT
typedef struct _X11_atoms_t{
    Atom net_active_window;
    Atom net_close_window;
    Atom net_wm_action_close;
    Atom net_wm_action_fullscreen;
    Atom net_wm_action_maximize_horz;
    Atom net_wm_action_maximize_vert;
    Atom net_wm_action_minimize;
    Atom net_wm_action_move;
    Atom net_wm_action_resize;
    Atom net_wm_action_shade;
    Atom net_wm_allowed_actions;
    Atom net_wm_name;
    Atom net_wm_state;
    Atom net_wm_state_fullscreen;
    Atom net_wm_state_hidden;
    Atom net_wm_state_maximized_horz;
    Atom net_wm_state_maximized_vert;
    Atom net_wm_state_modal;
    Atom net_wm_state_shaded;
    Atom net_wm_state_skip_pager;
    Atom net_wm_state_skip_taskbar;
    Atom net_wm_state_sticky;
    Atom net_wm_window_type;
    Atom net_wm_window_type_combo;
    Atom net_wm_window_type_desktop;
    Atom net_wm_window_type_dialog;
    Atom net_wm_window_type_dropdown_menu;
    Atom net_wm_window_type_dnd;
    Atom net_wm_window_type_dock;
    Atom net_wm_window_type_menu;
    Atom net_wm_window_type_normal;
    Atom net_wm_window_type_notification;
    Atom net_wm_window_type_popup_menu;
    Atom net_wm_window_type_splash;
    Atom net_wm_window_type_toolbar;
    Atom net_wm_window_type_tooltip;
    Atom net_wm_window_type_utility;
    Atom net_wm_ping;
    Atom wm_change_state;
    Atom wm_colormap_windows;
    Atom wm_delete_window;
    Atom wm_hints;
    Atom wm_name;
    Atom wm_normal_hints;
    Atom wm_protocols;
    Atom wm_state;
    Atom wm_take_focus;
    Atom wm_transient_for;

    Atom wm_quit;
    Atom wm_message;
    Atom wm_notice;
    Atom wm_input;
    Atom wm_scroll;
    
    Atom xdu_struct;
    Atom xdu_dispatch;
    Atom xdu_subproc;
    Atom xdu_user_delta;
    Atom xdu_core_delta;

}X11_atoms_t;

extern X11_atoms_t  g_atoms;

#define XRGB(ch) (unsigned short)((double)ch * 65535.0 / 256.0)

typedef struct _X11_context_t{
    int type;
    GC context;
    Drawable device;
    int width;
    int height;
    Visual* visual;
    Colormap color;
    unsigned int depth;
}X11_context_t;

extern Display*     g_display;

typedef X11_context_t* res_ctx_t;
typedef Colormap    res_clr_t;
typedef Font		res_font_t;
#ifdef XDU_SUPPORT_CONTEXT_BITMAP
typedef XImage*		res_bmp_t;
#endif
#ifdef XDU_SUPPORT_CONTEXT_REGION
typedef Region		res_rgn_t;
#endif

#endif /*XDU_SUPPORT_CONTEXT*/

#ifdef XDU_SUPPORT_WIDGET
extern Window       g_capture;

typedef XEvent      msg_t;
typedef unsigned long	res_acl_t;
typedef unsigned int	wparam_t;
typedef unsigned long   lparam_t;
typedef int         result_t;
typedef Window      res_win_t;

#ifdef XDU_SUPPORT_WIDGET_NC
/*widget nc hit test*/
#define HINT_NOWHERE	0
#define HINT_TITLE		2
#define HINT_CLIENT		1
#define HINT_RESTORE	4
#define HINT_MINIMIZE	8
#define HINT_MAXIMIZE	9
#define HINT_LEFT		10
#define HINT_RIGHT		11
#define HINT_TOP		12
#define HINT_TOPLEFT	13
#define HINT_TOPRIGHT	14
#define HINT_BOTTOM		15
#define HINT_LEFTBOTTOM	16
#define HINT_RIGHTBOTTOM	17
#define HINT_BORDER		18
#define HINT_CLOSE		20
#define HINT_ICON		21
#define HINT_MENUBAR	100
#define HINT_HSCROLL	101
#define HINT_VSCROLL	102
#define HINT_PAGEUP		103
#define HINT_PAGEDOWN	104
#endif
#endif /*XDU_SUPPORT_WIDGET*/


#endif //_XDU_LINUX_H