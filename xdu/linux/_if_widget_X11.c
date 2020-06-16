/***********************************************************************
	Easily xdu v5.5

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

#include "xduiml.h"
#include "xduutil.h"

#ifdef XDU_SUPPORT_WIDGET

#define WIDGET_EVENTS   (KeyPressMask | KeyReleaseMask \
                        | ButtonPressMask | ButtonReleaseMask | Button1MotionMask | Button2MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask | ButtonMotionMask \
                        | EnterWindowMask |	LeaveWindowMask | PointerMotionMask| PointerMotionHintMask \
                        | KeymapStateMask | ExposureMask | VisibilityChangeMask | StructureNotifyMask | SubstructureNotifyMask | FocusChangeMask | PropertyChangeMask | ColormapChangeMask | OwnerGrabButtonMask)
#define WIDGET_CHILD_EVENTS (KeyPressMask | KeyReleaseMask \
						| ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | Button1MotionMask | Button2MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask \
						| EnterWindowMask |	LeaveWindowMask | PointerMotionMask| PointerMotionHintMask \
						| ExposureMask \
						| FocusChangeMask \
						| StructureNotifyMask | SubstructureNotifyMask)
#define WIDGET_POPUP_EVENTS (KeyPressMask | KeyReleaseMask \
						| ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | Button1MotionMask | Button2MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask \
						| EnterWindowMask |	LeaveWindowMask | PointerMotionMask| PointerMotionHintMask \
						| ExposureMask \
						| FocusChangeMask \
						| StructureNotifyMask | SubstructureNotifyMask)						
#define WIDGET_NORMAL_EVENTS (KeyPressMask | KeyReleaseMask \
						| ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | Button1MotionMask | Button2MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask \
						| EnterWindowMask |	LeaveWindowMask | PointerMotionMask| PointerMotionHintMask \
						| ExposureMask \
						| FocusChangeMask \
						| StructureNotifyMask | SubstructureNotifyMask \
						| VisibilityChangeMask \
						| KeymapStateMask)



#define WIDGET_TITLE_SPAN		(float)10	//mm
#define WIDGET_MENU_SPAN		(float)7.5	//mm
#define WIDGET_SCROLL_SPAN		(float)5	//mm
#define WIDGET_ICON_SPAN		(float)3	//mm
#define WIDGET_FRAME_EDGE		(float)1.5	//mm
#define WIDGET_CHILD_EDGE		(float)0.5	//mm

#define WIDGET_BORDER_WIDTH		2 //pt

#define HIWORD(dw)		(unsigned short)(((unsigned int)(dw) >> 16) & 0x0000FFFF)
#define LOWORD(dw)		(unsigned short)((unsigned int)(dw) & 0x0000FFFF)

#define IS_META_KEY(key)	(key == XK_Shift_L || key == XK_Shift_R || key == XK_Control_L || key == XK_Control_R ||key == XK_Caps_Lock || key == XK_Shift_Lock || key == XK_Meta_L || key == XK_Meta_R || key == XK_Alt_L || key == XK_Alt_R || key == XK_Super_L || key == XK_Super_R ||key == XK_Hyper_L || key == XK_Hyper_R)


X11_atoms_t  g_atoms = {0};
Window g_capture = (res_win_t)0;

typedef struct _X11_widget_t{
	res_win_t root;
	res_win_t parent;
	res_win_t owner;
	res_win_t self;

	uid_t uid;
	dword_t style;
	bool_t disable;

	long events;
	res_acl_t acl;
	XIC xic;

	int state;
	int result;

	res_ctx_t wgc;
	res_ctx_t cgc;

	xpoint_t pt;
	xsize_t st;

	scroll_t hs;
	scroll_t vs;

	xfont_t xf;
	xface_t xa;
	xbrush_t xb;
	xpen_t xp;
	xcolor_t msk;
	xcolor_t ico;
}X11_widget_t;


result_t XdcWidgetProc(res_win_t hWnd, unsigned int message, wparam_t wParam, lparam_t lParam);

static bool_t _WindowSetProper(res_win_t wt, Atom atom, const unsigned char* data, dword_t len)
{
    return (Success == XChangeProperty(g_display, wt, atom, XA_STRING, 8, PropModeReplace, data, len))? 1 : 0;
}

static bool_t _WindowGetProper(res_win_t wt, Atom atom, unsigned char* data, dword_t len)
{
    Atom type = 0;
    int format = 0;
    unsigned int nitems = 0, after = 0;
    unsigned char *prop = 0;
    
    if (Success != XGetWindowProperty(g_display, wt, atom, 0, 1024, False, XA_STRING, &type, &format, &nitems, &after, &prop))
        return 0;
    
    if(prop)
    {
        memcpy((void*)data,(void*)prop, len);
    }
    
    XFree(prop);
    
    return 1;
    
}

static bool_t _WindowDelProper(res_win_t wt, Atom atom)
{    
    return (Success == XDeleteProperty(g_display, wt, atom))? 1 : 0;
}

static X11_widget_t* GETXDUSTRUCT(res_win_t hWnd)
{
    byte_t bys[VOID_SIZE] = {0};
    
    _WindowGetProper(hWnd, g_atoms.xdu_struct, bys, VOID_SIZE);
    
    return (X11_widget_t*)GET_VAR_LONG_NET(bys);
}

static void SETXDUSTRUCT(res_win_t hWnd, X11_widget_t* p)
{
    byte_t bys[VOID_SIZE] = {0};
    
    PUT_VAR_LONG_NET(bys, p);
    
    _WindowSetProper(hWnd, g_atoms.xdu_struct, bys, VOID_SIZE);
}

static if_event_t* GETXDUDISPATCH(res_win_t hWnd)
{
    byte_t bys[VOID_SIZE] = {0};
    
    _WindowGetProper(hWnd, g_atoms.xdu_dispatch, bys, VOID_SIZE);
    
    return (if_event_t*)GET_VAR_LONG_NET(bys);
}

static void SETXDUDISPATCH(res_win_t hWnd, if_event_t* p)
{
    byte_t bys[VOID_SIZE] = {0};
    
    PUT_VAR_LONG_NET(bys, p);
    
    _WindowSetProper(hWnd, g_atoms.xdu_dispatch, bys, VOID_SIZE);
}

static if_subproc_t* GETXDUSUBPROC(res_win_t hWnd)
{
    byte_t bys[VOID_SIZE] = {0};
    
    _WindowGetProper(hWnd, g_atoms.xdu_subproc, bys, VOID_SIZE);
    
    return (if_subproc_t*)GET_VAR_LONG_NET(bys);
}

static void SETXDUSUBPROC(res_win_t hWnd, if_subproc_t* p)
{
    byte_t bys[VOID_SIZE] = {0};
    
    PUT_VAR_LONG_NET(bys, p);
    
    _WindowSetProper(hWnd, g_atoms.xdu_subproc, bys, VOID_SIZE);
}

static void* GETXDUCOREDELTA(res_win_t hWnd)
{
    byte_t bys[VOID_SIZE] = {0};
    
    _WindowGetProper(hWnd, g_atoms.xdu_core_delta, bys, VOID_SIZE);
    
    return (void*)GET_VAR_LONG_NET(bys);
    
}

static void SETXDUCOREDELTA(res_win_t hWnd, void* p)
{
    byte_t bys[VOID_SIZE] = {0};
    
    PUT_VAR_LONG_NET(bys, p);
    
    _WindowSetProper(hWnd, g_atoms.xdu_core_delta, bys, VOID_SIZE);
}

static void* GETXDUUSERDELTA(res_win_t hWnd)
{
    byte_t bys[VOID_SIZE] = {0};
    
    _WindowGetProper(hWnd, g_atoms.xdu_user_delta, bys, VOID_SIZE);
    
    return (void*)GET_VAR_LONG_NET(bys);
    
}

static void SETXDUUSERDELTA(res_win_t hWnd, void* p)
{
    byte_t bys[VOID_SIZE] = {0};
    
    PUT_VAR_LONG_NET(bys, p);
    
    _WindowSetProper(hWnd, g_atoms.xdu_user_delta, bys, VOID_SIZE);
}

/*******************************************************************************************/

void _widget_startup(int ver)
{
	g_atoms.net_active_window = XInternAtom (g_display, "_NET_ACTIVE_WINDOW", False);
    g_atoms.net_close_window = XInternAtom (g_display, "_NET_CLOSE_WINDOW", False);
    g_atoms.net_wm_action_close = XInternAtom (g_display, "_NET_WM_ACTION_CLOSE", False);
    g_atoms.net_wm_action_fullscreen = XInternAtom (g_display, "_NET_WM_ACTION_FULLSCREEN", False);
    g_atoms.net_wm_action_maximize_horz = XInternAtom (g_display, "_NET_WM_ACTION_MAXIMIZE_HORZ", False);
    g_atoms.net_wm_action_maximize_vert = XInternAtom (g_display, "_NET_WM_ACTION_MAXIMIZE_VERT", False);
    g_atoms.net_wm_action_minimize = XInternAtom (g_display, "_NET_WM_ACTION_MINIMIZE", False);
    g_atoms.net_wm_action_move = XInternAtom (g_display, "_NET_WM_ACTION_MOVE", False);
    g_atoms.net_wm_action_resize = XInternAtom (g_display, "_NET_WM_ACTION_RESIZE", False);
    g_atoms.net_wm_action_shade = XInternAtom (g_display, "_NET_WM_ACTION_SHADE", False);
    g_atoms.net_wm_allowed_actions = XInternAtom (g_display, "_NET_WM_ALLOWED_ACTIONS", False);
    g_atoms.net_wm_name = XInternAtom (g_display, "_NET_WM_NAME", False);
    g_atoms.net_wm_state = XInternAtom (g_display, "_NET_WM_STATE", False);
    g_atoms.net_wm_state_fullscreen = XInternAtom (g_display, "_NET_WM_STATE_FULLSCREEN", False);
    g_atoms.net_wm_state_hidden = XInternAtom (g_display, "_NET_WM_STATE_HIDDEN", False);
    g_atoms.net_wm_state_maximized_horz = XInternAtom (g_display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    g_atoms.net_wm_state_maximized_vert = XInternAtom (g_display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
    g_atoms.net_wm_state_modal = XInternAtom (g_display, "_NET_WM_STATE_MODAL", False);
    g_atoms.net_wm_state_shaded = XInternAtom (g_display, "_NET_WM_STATE_SHADED", False);
    g_atoms.net_wm_state_skip_pager = XInternAtom (g_display, "_NET_WM_STATE_SKIP_PAGER", False);
    g_atoms.net_wm_state_skip_taskbar = XInternAtom (g_display, "_NET_WM_STATE_SKIP_TASKBAR", False);
    g_atoms.net_wm_state_sticky = XInternAtom (g_display, "_NET_WM_STATE_STICKY", False);
    g_atoms.net_wm_window_type = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE", False);
    g_atoms.net_wm_window_type_combo = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_COMBO", False);
    g_atoms.net_wm_window_type_desktop = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    g_atoms.net_wm_window_type_dialog = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
    g_atoms.net_wm_window_type_dock = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_DOCK", False);
    g_atoms.net_wm_window_type_dnd = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_DND", False);
    g_atoms.net_wm_window_type_dropdown_menu = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", False);
    g_atoms.net_wm_window_type_menu = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_MENU", False);
    g_atoms.net_wm_window_type_normal = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
    g_atoms.net_wm_window_type_notification = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
    g_atoms.net_wm_window_type_popup_menu = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_POPUP_MENU", False);
    g_atoms.net_wm_window_type_splash = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_SPLASH", False);
    g_atoms.net_wm_window_type_tooltip = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_TOOLTIP", False);
    g_atoms.net_wm_window_type_toolbar = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
    g_atoms.net_wm_window_type_utility = XInternAtom (g_display, "_NET_WM_WINDOW_TYPE_UTILITY", False);
    g_atoms.net_wm_ping = XInternAtom (g_display, "_NET_WM_PING", False);
	g_atoms.wm_change_state = XInternAtom (g_display, "WM_CHANGE_STATE", False);
    g_atoms.wm_colormap_windows = XInternAtom (g_display, "WM_COLORMAP_WINDOWS", False);
    g_atoms.wm_delete_window = XInternAtom (g_display, "WM_DELETE_WINDOW", False);
    g_atoms.wm_hints = XInternAtom (g_display, "WM_HINTS", False);
    g_atoms.wm_name = XInternAtom (g_display, "WM_NAME", False);
    g_atoms.wm_normal_hints = XInternAtom (g_display, "WM_NORMAL_HINTS", False);
    g_atoms.wm_protocols = XInternAtom (g_display, "WM_PROTOCOLS", False);
    g_atoms.wm_state = XInternAtom (g_display, "WM_STATE", False);
    g_atoms.wm_take_focus = XInternAtom (g_display, "WM_TAKE_FOCUS", False);
    g_atoms.wm_transient_for = XInternAtom (g_display, "WM_TRANSIENT_FOR", False);

	g_atoms.wm_quit = XInternAtom (g_display, "WM_QUIT", False);
	g_atoms.wm_command = XInternAtom (g_display, "WM_COMMAND", False);
	g_atoms.wm_notice = XInternAtom (g_display, "WM_NOTICE", False);
	g_atoms.wm_input = XInternAtom (g_display, "WM_INPUT", False);
	g_atoms.wm_scroll = XInternAtom (g_display, "WM_SCROLL", False);

    g_atoms.xdu_struct = XInternAtom (g_display, "XDUSTRUCT", False);
    g_atoms.xdu_dispatch = XInternAtom (g_display, "XDUDISPATCH", False);
    g_atoms.xdu_subproc = XInternAtom (g_display, "XDUSUBPROC", False);
    g_atoms.xdu_user_delta = XInternAtom (g_display, "XDUUSERDELTA", False);
    g_atoms.xdu_core_delta = XInternAtom (g_display, "XDUCOREDELTA", False);
}

void _widget_cleanup()
{

}

res_win_t _widget_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent, if_event_t* pev)
{
    Window win, rot, par;
    int screen_num, screen_dep;
	int border_width = 0;
    XSetWindowAttributes attr = {0};
	Atom atom, atoms[2] = {0};
	XWMHints *hints = NULL;
	X11_widget_t* ps = NULL;
	if_event_t* pv = NULL;

    screen_num = DefaultScreen(g_display);
	screen_dep = DefaultDepth(g_display, screen_num);

	rot = RootWindow(g_display, screen_num);
	par = ((wparent)? wparent : rot);

	if(!par) return NULL;

	if(wstyle & WD_STYLE_TITLE)
	{
		attr.override_redirect = False;
		border_width = WIDGET_BORDER_WIDTH;
		attr.border_pixel = BlackPixel(g_display, screen_num);
		attr.background_pixel = WhitePixel(g_display,screen_num);
		attr.event_mask = WIDGET_NORMAL_EVENTS;
	}else
	{
		attr.override_redirect = True;

		if(wstyle & WD_STYLE_CHILD)
		{
			border_width = 0;
			attr.border_pixel = WhitePixel(g_display, screen_num);
			attr.background_pixel = BlackPixel(g_display,screen_num);
			attr.event_mask = WIDGET_CHILD_EVENTS;
		}else
		{
			border_width = WIDGET_BORDER_WIDTH;
			attr.border_pixel = BlackPixel(g_display, screen_num);
			attr.background_pixel = WhitePixel(g_display,screen_num);
			attr.event_mask = WIDGET_POPUP_EVENTS;
		}
	}

	win = XCreateWindow(g_display,
			par,
			pxr->x, pxr->y, (pxr->w - border_width), (pxr->h - border_width),
			border_width,
			screen_dep,
			InputOutput,
            CopyFromParent,
            (CWOverrideRedirect | CWBorderPixel | CWBackPixel | CWEventMask),
            &attr);

    if(!win) return NULL;

	XStoreName (g_display, win, wname);

	if(wstyle & WD_STYLE_TITLE)
	{
		atom = g_atoms.net_wm_window_type_normal;
	}else
	{
		if(wstyle & WD_STYLE_CHILD)
		{
			atom = g_atoms.net_wm_window_type_normal;
		}else
		{
			atom = g_atoms.net_wm_window_type_popup_menu;
		}
	}
    
    XChangeProperty(g_display, win, g_atoms.net_wm_window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *) &atom, 1);

	if(wstyle == WD_STYLE_FRAME)
	{
		atoms[0] = g_atoms.wm_take_focus;
		atoms[1] = g_atoms.wm_delete_window;
		XSetWMProtocols (g_display, win, atoms, 2);
	}else
	{
		atoms[0] = g_atoms.wm_take_focus;
		XSetWMProtocols (g_display, win, atoms, 1);
	}

	if(wstyle & WD_STYLE_NOACTIVE)
	{
		hints = XAllocWMHints();
		hints->flags |= InputHint;
		hints->input = False;
		XSetWMHints(g_display, win, hints);
		XFree(hints);
	}

	ps = (X11_widget_t*)calloc(1, sizeof(X11_widget_t));

	ps->root = rot;
	ps->parent = wparent;
	ps->self = win;
	ps->style = wstyle;
	ps->state = WS_SHOW_HIDE;
	ps->events = attr.event_mask;

	ps->wgc = _create_display_context(win);
	ps->cgc = _create_display_context(win);
    
	SETXDUSTRUCT(win, ps);

	if(pev)
	{
		pv = (if_event_t*)calloc(1, sizeof(if_event_t));
		memcpy((void*)pv, (void*)pev, sizeof(if_event_t));
		SETXDUDISPATCH(win, pv);
	}

	if(pev && pev->pf_on_create)
	{
		(*pev->pf_on_create)(win, pev->param);
	}

    return win;
}

void _widget_destroy(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	if_event_t* pv = GETXDUDISPATCH(wt);

	if(pv && pv->pf_on_destroy)
	{
		(*pv->pf_on_destroy)(wt);
	}

    XDestroyWindow(g_display, wt);

	if(ps)
	{
		if(ps->wgc) _destroy_context(ps->wgc);

		if(ps->cgc) _destroy_context(ps->cgc);

		free(ps);
	}

	if(pv)
	{
		free(pv);
	}
}

void _widget_close(res_win_t wt, int ret)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
    if_event_t* pv = GETXDUDISPATCH(wt);
	//XClientMessageEvent ev = {0};

	if(ps) ps->result = ret;
	
	if(pv && pv->pf_on_close)
	{
		if((*pv->pf_on_close)(wt))
			return;
	}

	_widget_destroy(wt);
    
    /*ev.type = ClientMessage;
    ev.window = wt;
    ev.message_type = g_atoms.net_close_window;
    ev.format = 32;
    ev.data.l[0] = CurrentTime;
    ev.data.l[1] = 0;
    ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
    
    XSendEvent (g_display, RootWindow(g_display, XDefaultScreen(g_display)), False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev);
    
    XFlush (g_display);*/
}

if_subproc_t* _widget_get_subproc(res_win_t wt, uid_t sid)
{
	return GETXDUSUBPROC(wt);
}

bool_t _widget_set_subproc(res_win_t wt, uid_t sid, if_subproc_t* sub)
{
	SETXDUSUBPROC(wt, sub);

	return 1;
}

void _widget_del_subproc(res_win_t wt, uid_t sid)
{
	SETXDUSUBPROC(wt, NULL);
}

bool_t _widget_set_subproc_delta(res_win_t wt, uid_t sid, var_long delta)
{
	if_subproc_t* psub;

	psub = GETXDUSUBPROC(wt);

	psub->delta = delta;
	return 1;
}

var_long _widget_get_subproc_delta(res_win_t wt, uid_t sid)
{
	if_subproc_t* psub;

	psub = GETXDUSUBPROC(wt);

	return psub->delta;
}

bool_t _widget_has_subproc(res_win_t wt)
{
	return (GETXDUSUBPROC(wt) == NULL) ? 0 : 1;
}

void _widget_set_core_delta(res_win_t wt, var_long pd)
{
	SETXDUCOREDELTA(wt, pd);
}

var_long _widget_get_core_delta(res_win_t wt)
{
	return GETXDUCOREDELTA(wt);
}

void _widget_set_user_delta(res_win_t wt, var_long pd)
{
	SETXDUUSERDELTA(wt, pd);
}

var_long _widget_get_user_delta(res_win_t wt)
{
	return GETXDUUSERDELTA(wt);
}

void _widget_set_style(res_win_t wt, dword_t ws)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(ps) ps->style = ws;
}

dword_t _widget_get_style(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	return (ps)? ps->style : 0;
}

void _widget_set_accel(res_win_t wt, res_acl_t acl)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(ps) ps->acl = acl;
}

res_acl_t _widget_get_accel(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	return (ps)? ps->acl : (res_acl_t)0;
}

void _widget_set_owner(res_win_t wt, res_win_t win)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(ps) ps->owner = win;
}

res_win_t _widget_get_owner(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	return (ps)? ps->owner : (res_win_t)0;
}

void _widget_set_user_id(res_win_t wt, uid_t uid)
{
    X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(ps) ps->uid = uid;
}

uid_t _widget_get_user_id(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

    return (ps)? ps->uid : 0;
}

void _widget_set_user_result(res_win_t wt, int rt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(ps) ps->result = rt;
}

int _widget_get_user_result(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

    return (ps)? ps->result : 0;
}

res_win_t _widget_get_child(res_win_t wt, uid_t uid)
{
    unsigned int i, n;
    Window Root, Parent, child;
    Window* Children;
	X11_widget_t* ps;

    if(XQueryTree(g_display, wt, &Root, &Parent, &Children, &n) != True)
        return (res_win_t)NULL;

    for(i=0;i<n;i++)
    {
        child = Children[i];
        ps = GETXDUSTRUCT(child);

        if(ps && ps->uid == uid)
            return child;
    }
    
    return (res_win_t)NULL;
}

res_win_t _widget_get_parent(res_win_t wt)
{
    unsigned int n = 0;
    Window Root = NULL, Parent = NULL;
    Window* Children = NULL;
    
    if(XQueryTree(g_display, wt, &Root, &Parent, &Children, &n) != True)
        return (res_win_t)NULL;
    
	if(Children)
		XFree(Children);

    return (res_win_t)Parent;
}

void _widget_set_user_prop(res_win_t wt, const tchar_t* pname,var_long val)
{
    unsigned char bys[VOID_SIZE] = {0};
    
    PUT_VAR_LONG_NET(bys, val);
    
    _WindowSetProper(wt, pname, bys, VOID_SIZE);
}

var_long _widget_get_user_prop(res_win_t wt, const tchar_t* pname)
{
    unsigned char bys[VOID_SIZE] = {0};
    
    _WindowGetProper(wt, pname, bys, VOID_SIZE);
    
    return (var_long)GET_VAR_LONG_NET(bys);
}

var_long _widget_del_user_prop(res_win_t wt, const tchar_t* pname)
{
    var_long rt;
    
    rt = _widget_get_user_prop(wt, pname);
    
    _WindowDelProper(wt, pname);
    
    return rt;
}

if_event_t* _widget_get_dispatch(res_win_t wt)
{
	return GETXDUDISPATCH(wt);
}

void _widget_calc_border(dword_t ws, border_t* pbd)
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

void _widget_adjust_size(dword_t ws, xsize_t* pxs)
{
	border_t bd = {0};

	_widget_calc_border(ws, &bd);

	pxs->cx += (2 * bd.edge + bd.vscroll);
	pxs->cy += (2 * bd.edge + bd.title + bd.menu + bd.hscroll);
}

bool_t _widget_is_maximized(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	return (ps->state == WS_SHOW_MAXIMIZE)? 1 : 0;
}

bool_t _widget_is_minimized(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	return (ps->state == WS_SHOW_MINIMIZE)? 1 : 0;
}

bool_t _widget_enum_child(res_win_t wt, PF_ENUM_WINDOW_PROC pf, var_long pv)
{
	unsigned int i, n;
    Window Root, Parent, child;
    Window* Children;

    if(XQueryTree(g_display, wt, &Root, &Parent, &Children, &n) != True)
        return 0;

    for(i=0;i<n;i++)
    {
        child = Children[i];
        
        if((*pf)(child, pv))
            break;
    }
    
    return 1;
}

res_ctx_t _widget_client_ctx(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	
	return (ps)? ps->cgc : NULL;
}

res_ctx_t _widget_window_ctx(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	
	return (ps)? ps->wgc : NULL;
}

void _widget_release_ctx(res_win_t wt, res_ctx_t dc)
{
	X11_context_t* ctx = (X11_context_t*)dc;

}

void _widget_get_client_rect(res_win_t wt, xrect_t* prt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	XWindowAttributes attr = {0};
	border_t bd = {0};

	XGetWindowAttributes(g_display, wt, &attr);

	if(ps)
	{
		_widget_calc_border(ps->style, &bd);
	}

	prt->x = 0;
	prt->y = 0;
	prt->w = attr.width - 2 * bd.edge - bd.vscroll;
	prt->h = attr.height - 2 * bd.edge - bd.title - bd.menu - bd.hscroll;
}

void _widget_get_window_rect(res_win_t wt, xrect_t* prt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	XWindowAttributes attr = {0};
	int dst_x = 0, dst_y = 0;
	Window rot, cld = NULL;

	XGetWindowAttributes(g_display, wt, &attr);

	rot = (ps)? ps->root : RootWindow(g_display, DefaultScreen(g_display));

	XTranslateCoordinates(g_display, wt, rot, attr.x, attr.y, &dst_x, &dst_y, &cld);

	prt->x = dst_x - attr.border_width;
	prt->y = dst_y - attr.border_width;
	prt->w = attr.width + 2 * attr.border_width;
	prt->h = attr.height + 2 * attr.border_width;
}

void _widget_client_to_screen(res_win_t wt, xpoint_t* ppt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	Window rot, cld = NULL;
	int dst_x = 0, dst_y = 0;

	rot = (ps)? ps->root : RootWindow(g_display, DefaultScreen(g_display));

	XTranslateCoordinates(g_display, wt, rot, ppt->x, ppt->y, &dst_x, &dst_y, &cld);

	ppt->x = dst_x;
	ppt->y = dst_y;
}

void _widget_screen_to_client(res_win_t wt, xpoint_t* ppt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	Window rot, cld = NULL;
	int dst_x = 0, dst_y = 0;

	rot = (ps)? ps->root : RootWindow(g_display, DefaultScreen(g_display));

	XTranslateCoordinates(g_display, rot, wt, ppt->x, ppt->y, &dst_x, &dst_y, &cld);

	ppt->x = dst_x;
	ppt->y = dst_y;
}

void _widget_client_to_window(res_win_t wt, xpoint_t* ppt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	border_t bd = {0};

	if(ps)
	{
		_widget_calc_border(ps->style, &bd);
	}

	ppt->x += bd.edge;
	ppt->y += (bd.edge + bd.title + bd.menu);
}

void _widget_window_to_client(res_win_t wt, xpoint_t* ppt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	border_t bd = {0};

	if(ps)
	{
		_widget_calc_border(ps->style, &bd);
	}

	ppt->x -= bd.edge;
	ppt->y -= (bd.edge + bd.title + bd.menu);
}

void _widget_center_window(res_win_t wt, res_win_t owner)
{
	
}

void _widget_set_cursor(res_win_t wt, int curs)
{
	Cursor cur;

	if(!curs)
	{
		XUndefineCursor(g_display, wt);
		return;
	}

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

	cur = XCreateFontCursor(g_display, XC_xterm);

	XDefineCursor(g_display, wt, cur);

	XFreeCursor(g_display, cur);
}

void _widget_set_capture(res_win_t wt, bool_t b)
{
	g_capture = (b)? wt : (res_win_t)0;
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
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	
}

void _widget_destroy_caret(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

}

void _widget_show_caret(res_win_t wt, int x, int y, bool_t b)
{

}

void _widget_set_focus(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	Window org = NULL;
	int rev = 0;

	if(ps && (ps->style & WD_STYLE_NOACTIVE))
		return;

	XGetInputFocus(g_display, &org, &rev);
	if(org == wt)
		return;
	
	XSetInputFocus(g_display, wt, RevertToParent, CurrentTime);
}

bool_t _widget_key_state(res_win_t wt, int key)
{
	XKeyEvent ke = {0};
	KeySym ks = 0;

	ks = XLookupKeysym(&ke, 0);

	switch (key)
	{
	case KEY_SHIFT:
		if(ke.state & ShiftMask)
			return 1;
		break;
	case KEY_CONTROL:
		if(ke.state & ControlMask)
			return 1;
		break;
	}

	return 0;
}


static int _tmp_error_handler(Display* dpy, XErrorEvent* pee)
{
	return 0;
}

bool_t _widget_is_valid(res_win_t wt)
{
	X11_widget_t* ps ;
	XErrorHandler pf_org;
	
	pf_org = XSetErrorHandler(_tmp_error_handler);

	ps = GETXDUSTRUCT(wt);

	XSetErrorHandler(pf_org);

	return (ps)? 1 : 0;
}

bool_t _widget_is_focus(res_win_t wt)
{
	Window fw = 0;
	int r = 0;

	XGetInputFocus(g_display, &fw, &r);

	return (wt == fw)? 1 : 0;
}

bool_t _widget_is_child(res_win_t wt)
{
    X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(!ps) return 0;

	return (ps->style & WD_STYLE_CHILD)? 1 : 0;
}

bool_t _widget_is_ownc(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(!ps) return 0;

	return (ps->style & WD_STYLE_OWNERNC) ? 1 : 0;
}

void _widget_move(res_win_t wt, const xpoint_t* ppt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	XMoveWindow(g_display, wt, ppt->x, ppt->y);
}

void _widget_size(res_win_t wt, const xsize_t* pxs)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	XResizeWindow(g_display, wt, pxs->cx, pxs->cy);
}

void _widget_take(res_win_t wt, int zor)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	XWindowChanges wc = {0};

	switch(zor)
	{
	case WS_TAKE_TOP:
		wc.stack_mode = Above;
		break;
	case WS_TAKE_BOTTOM:
		wc.stack_mode = Below;
		break;
	case WS_TAKE_TOPMOST:
		wc.stack_mode = TopIf;
		break;
	case WS_TAKE_NOTOPMOST:
		wc.stack_mode = Opposite;
		break;
	}

	XConfigureWindow(g_display, wt, CWStackMode, &wc);
}

void _widget_show(res_win_t wt, dword_t sw)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	XClientMessageEvent ev = {0};

	switch(sw)
	{
	case WS_SHOW_MINIMIZE:
		XMapWindow(g_display, wt);
		XIconifyWindow(g_display, wt, DefaultScreen(g_display));

		if(ps) ps->state = WS_SHOW_MINIMIZE;
		break;
	case WS_SHOW_MAXIMIZE:
		XMapWindow(g_display, wt);

		ev.type = ClientMessage;
		ev.serial = 0;
		ev.send_event = 1;
		ev.display = g_display;
		ev.window = wt;
		ev.message_type = g_atoms.net_wm_state;
		ev.format = 32;
		ev.data.l[0] = 1;
		ev.data.l[1] = g_atoms.net_wm_state_maximized_vert;
		ev.data.l[2] = g_atoms.net_wm_state_maximized_horz;
		ev.data.l[3] = 1;

		XSendEvent(g_display, RootWindow(g_display, DefaultScreen(g_display)), False, SubstructureRedirectMask | SubstructureNotifyMask, &ev);

		if(ps) ps->state = WS_SHOW_MAXIMIZE;
		break;
	case WS_SHOW_HIDE:
		XWithdrawWindow(g_display, wt, DefaultScreen(g_display));

		if(ps) ps->state = WS_SHOW_HIDE;
		break;
	default:
		XMapWindow(g_display, wt);

		if(ps) ps->state = WS_SHOW_NORMAL;
		break;
	}
}

void _widget_update(res_win_t wt)
{

}

void _widget_layout(res_win_t wt)
{
	XEvent ev = {0};

	ev.type = ResizeRequest;
    ev.xresizerequest.window = wt;

    XSendEvent(g_display,wt,False,ResizeRedirectMask,&ev);
}

void _widget_erase(res_win_t wt, const xrect_t* prt)
{
	if(!prt)
		XClearArea(g_display, wt, 0, 0, 0, 0, True);
	else
		XClearArea(g_display, wt, prt->x, prt->y, prt->w, prt->h, True);
}

void _widget_paint(res_win_t wt)
{
	XClearWindow(g_display, wt);

	XFlush(g_display);
}

void _widget_enable(res_win_t wt, bool_t b)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(ps) ps->disable = (b)? 0 : 1;
}

void _widget_post_notice(res_win_t wt, NOTICE* pnt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	XClientMessageEvent ev = {0};

	if(!ps) return;

    ev.type = ClientMessage;
	ev.serial = 0;
	ev.send_event = 1;
	ev.display = g_display;
    ev.window = wt;
    ev.message_type = g_atoms.wm_notice;
    ev.format = 32;
    ev.data.l[0] = pnt->id;
    ev.data.l[1] = pnt->code;
    ev.data.l[2] = (long)pnt;
	ev.data.l[3] = ev.data.l[4] = 0;
    
    XSendEvent (g_display, wt, False, SubstructureNotifyMask, (XEvent*)&ev);
}

int _widget_send_notice(res_win_t wt, NOTICE* pnt)
{
    X11_widget_t* ps = GETXDUSTRUCT(wt);
	if_event_t* pif;

	if(!ps) return 0;
	
	pif = GETXDUDISPATCH(wt);

	if(pif && pif->pf_on_notice)
	{
		(*pif->pf_on_notice)(wt, pnt);

		return 1;
	}

	return 0;
}

void _widget_post_command(res_win_t wt, int code, uid_t cid, var_long data)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);
	XClientMessageEvent ev = {0};

	if(!ps) return;

    ev.type = ClientMessage;
	ev.serial = 0;
	ev.send_event = 1;
	ev.display = g_display;
    ev.window = wt;
    ev.message_type = g_atoms.wm_command;
    ev.format = 32;
    ev.data.l[0] = cid;
    ev.data.l[1] = code;
    ev.data.l[2] = data;
	ev.data.l[3] = ev.data.l[4] = 0;
    
    XSendEvent (g_display, wt, False, SubstructureNotifyMask, (XEvent*)&ev);
}

int _widget_send_command(res_win_t wt, int code, uid_t cid, var_long data)
{
    X11_widget_t* ps = GETXDUSTRUCT(wt);
	if_event_t* pif;

	if(!ps) return 0;

	pif = GETXDUDISPATCH(wt);

	switch(cid)
	{
	case IDC_PARENT:
		if(pif && pif->pf_on_parent_command)
		{
			(*pif->pf_on_parent_command)(wt, code, data);
			return 1;
		}
		break;
	case IDC_CHILD:
		if(pif && pif->pf_on_child_command)
		{
			(*pif->pf_on_child_command)(wt, code, data);
			return 1;
		}
		break;
	case IDC_SELF:
		if(pif && pif->pf_on_self_command)
		{
			(*pif->pf_on_self_command)(wt, code, data);
			return 1;
		}
		break;
	default:
		if(pif && pif->pf_on_menu_command)
		{
			(*pif->pf_on_menu_command)(wt, cid, code, data);
			return 1;
		}
		break;
	}

	return 0;
}

void _widget_post_char(res_win_t wt, tchar_t ch)
{
	XKeyEvent ev = {0};
	KeySym key;

	Window fw = 0;
	int fs = 0;

	if(!wt)
	{
		XGetInputFocus(g_display, &fw, &fs);
		wt = fw;
	}

	if(ch < 0x20)
		key = ((KeySym)ch) | 0xFF00;
	else
		key = ((KeySym)ch) & 0x00FF;

    ev.type = KeyPress;
	ev.serial = 0;
	ev.send_event = 1;
	ev.display = g_display;
    ev.window = (wt)? wt : InputFocus;
    ev.root = DefaultRootWindow(g_display);
	ev.subwindow = 0;
    ev.time = CurrentTime;
    ev.x = 0;
    ev.y = 0;
    ev.x_root = 0;
	ev.y_root = 0;
	ev.state = 0;
	ev.keycode = XKeysymToKeycode(g_display, key);
	ev.same_screen = 1;
    
    XSendEvent (g_display, wt, False, NoEventMask, (XEvent*)&ev);

	ev.type = KeyRelease;
	ev.serial = 0;
	ev.send_event = 1;
	ev.display = g_display;
    ev.window = (wt)? wt : InputFocus;
    ev.root = DefaultRootWindow(g_display);
	ev.subwindow = 0;
    ev.time = CurrentTime;
    ev.x = 0;
    ev.y = 0;
    ev.x_root = 0;
	ev.y_root = 0;
	ev.state = 0;
	ev.keycode = XKeysymToKeycode(g_display, key);
	ev.same_screen = 1;

	XSendEvent (g_display, wt, False, NoEventMask, (XEvent*)&ev);
}

void _widget_post_key(res_win_t wt, int key)
{
	XKeyEvent ev = {0};
	Window fw = 0;
	int fs = 0;

	if(!wt)
	{
		XGetInputFocus(g_display, &fw, &fs);
		wt = fw;
	}

    ev.type = KeyPress;
	ev.serial = 0;
	ev.send_event = 1;
	ev.display = g_display;
    ev.window = (wt)? wt : InputFocus;
    ev.root = DefaultRootWindow(g_display);
	ev.subwindow = 0;
    ev.time = CurrentTime;
    ev.x = 0;
    ev.y = 0;
    ev.x_root = 0;
	ev.y_root = 0;
	ev.state = 0;
	ev.keycode = XKeysymToKeycode(g_display, key);
	ev.same_screen = 1;
    
    XSendEvent (g_display, wt, False, NoEventMask, (XEvent*)&ev);

	ev.type = KeyRelease;
	ev.serial = 0;
	ev.send_event = 1;
	ev.display = g_display;
    ev.window = (wt)? wt : InputFocus;
    ev.root = DefaultRootWindow(g_display);
	ev.subwindow = 0;
    ev.time = CurrentTime;
    ev.x = 0;
    ev.y = 0;
    ev.x_root = 0;
	ev.y_root = 0;
	ev.state = 0;
	ev.keycode = XKeysymToKeycode(g_display, key);
	ev.same_screen = 1;

	XSendEvent (g_display, wt, False, NoEventMask, (XEvent*)&ev);
}

void _widget_set_title(res_win_t wt, const tchar_t* token)
{

}

int _widget_get_title(res_win_t wt, tchar_t* buf, int max)
{
	return 0;
}

void _widget_active(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	XClientMessageEvent ev = {0};

    ev.type = ClientMessage;
	ev.serial = 0;
	ev.send_event = 1;
	ev.display = g_display;
    ev.window = wt;
    ev.message_type = g_atoms.net_active_window;
    ev.format = 32;
    ev.data.l[0] = 1;
    ev.data.l[1] = CurrentTime;
    ev.data.l[2] = 0;
	ev.data.l[3] = 0;
	ev.data.l[4] = 0;
    
    XSendEvent (g_display, wt, False, SubstructureNotifyMask, (XEvent*)&ev);
}

void _widget_scroll(res_win_t wt, bool_t horz, int line)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	XClientMessageEvent ev = {0};

    ev.type = ClientMessage;
	ev.serial = 0;
	ev.send_event = 1;
	ev.display = g_display;
    ev.window = wt;
    ev.message_type = g_atoms.wm_scroll;
    ev.format = 32;
    ev.data.l[0] = (horz)? 1 : 0;
    ev.data.l[1] = line;
    ev.data.l[2] = 0;
	ev.data.l[3] = 0;
	ev.data.l[4] = 0;
    
    XSendEvent (g_display, wt, False, SubstructureNotifyMask, (XEvent*)&ev);
}

void _widget_get_scroll_info(res_win_t wt, bool_t horz, scroll_t* psl)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(!ps) return;

	if(horz)
		memcpy((void*)psl, (void*)&(ps->hs), sizeof(scroll_t));
	else
		memcpy((void*)psl, (void*)&(ps->vs), sizeof(scroll_t));
}

void _widget_set_scroll_info(res_win_t wt, bool_t horz, const scroll_t* psl)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

	if(!ps) return;

	if(horz)
		memcpy((void*)&(ps->hs), (void*)psl, sizeof(scroll_t));
	else
		memcpy((void*)&(ps->vs), (void*)psl, sizeof(scroll_t));
}

bool_t _widget_has_struct(res_win_t wt)
{
	X11_widget_t* ps = GETXDUSTRUCT(wt);

    return (ps)? 1 : 0;
}

void  _widget_set_xfont(res_win_t wt, const xfont_t* pxf)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)&pws->xf, (void*)pxf, sizeof(xfont_t));
	}
}

void _widget_get_xfont(res_win_t wt, xfont_t* pxf)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)pxf, (void*)&pws->xf, sizeof(xfont_t));
	}
}

const xfont_t* _widget_get_xfont_ptr(res_win_t wt)
{
    X11_widget_t* pws = GETXDUSTRUCT(wt);

	return (pws)? &pws->xf : NULL;
}

void _widget_set_xface(res_win_t wt, const xface_t* pxa)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)&pws->xa, (void*)pxa, sizeof(xface_t));
	}
}

void _widget_get_xface(res_win_t wt, xface_t* pxa)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)pxa, (void*)&pws->xa, sizeof(xface_t));
	}
}

const xface_t* _widget_get_xface_ptr(res_win_t wt)
{
    X11_widget_t* pws = GETXDUSTRUCT(wt);

	return (pws)? &pws->xa : NULL;
}

void _widget_set_xbrush(res_win_t wt, const xbrush_t* pxb)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)&pws->xb, (void*)pxb, sizeof(xbrush_t));
	}
}

void _widget_get_xbrush(res_win_t wt, xbrush_t* pxb)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)pxb, (void*)&pws->xb, sizeof(xbrush_t));
	}
}

const xbrush_t* _widget_get_xbrush_ptr(res_win_t wt)
{
    X11_widget_t* pws = GETXDUSTRUCT(wt);

	return (pws)? &pws->xb : NULL;
}

void _widget_set_xpen(res_win_t wt, const xpen_t* pxp)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)&pws->xp, (void*)pxp, sizeof(xpen_t));
	}
}

void _widget_get_xpen(res_win_t wt, xpen_t* pxp)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)pxp, (void*)&pws->xp, sizeof(xpen_t));
	}
}

const xpen_t* _widget_get_xpen_ptr(res_win_t wt)
{
    X11_widget_t* pws = GETXDUSTRUCT(wt);

	return (pws)? &pws->xp : NULL;
}

void _widget_set_mask(res_win_t wt, const xcolor_t* pxc)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)&pws->msk, (void*)pxc, sizeof(xcolor_t));
	}
}

void _widget_get_mask(res_win_t wt, xcolor_t* pxc)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)pxc, (void*)&pws->msk, sizeof(xcolor_t));
	}
}

const xcolor_t* _widget_get_mask_ptr(res_win_t wt)
{
    X11_widget_t* pws = GETXDUSTRUCT(wt);

	return (pws)? &pws->msk : NULL;
}

void _widget_set_iconic(res_win_t wt, const xcolor_t* pxc)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)&pws->ico, (void*)pxc, sizeof(xcolor_t));
	}
}

void _widget_get_iconic(res_win_t wt, xcolor_t* pxc)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)pxc, (void*)&pws->ico, sizeof(xcolor_t));
	}
}

const xcolor_t* _widget_get_iconic_ptr(res_win_t wt)
{
    X11_widget_t* pws = GETXDUSTRUCT(wt);

	return (pws)? &pws->ico : NULL;
}

void _widget_set_point(res_win_t wt, const xpoint_t* ppt)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)&pws->pt, (void*)ppt, sizeof(xpoint_t));
	}
}

void _widget_get_point(res_win_t wt, xpoint_t* ppt)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)ppt, (void*)&pws->pt, sizeof(xpoint_t));
	}
}

void _widget_set_size(res_win_t wt, const xsize_t* pst)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)&pws->st, (void*)pst, sizeof(xsize_t));
	}
}

void _widget_get_size(res_win_t wt, xsize_t* pst)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);

	if (pws)
	{
		memcpy((void*)pst, (void*)&pws->st, sizeof(xsize_t));
	}
}

static int STDCALL _widget_set_child_color_mode(res_win_t wt, var_long pv)
{
	dword_t dw;

	if (_widget_has_struct(wt))
	{
		dw = _widget_get_style(wt);
		if (dw & WD_STYLE_NOCHANGE)
			return 1;

		_widget_set_color_mode(wt, (const clr_mod_t*)pv);
	}

	return 1;
}

void _widget_set_color_mode(res_win_t wt, const clr_mod_t* pclr)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);
	dword_t dw = (pws)? pws->style : 0;

	if (!pws)
		return;

	format_xcolor(&pclr->clr_bkg, pws->xb.color);

	format_xcolor(&pclr->clr_frg, pws->xp.color);

	format_xcolor(&pclr->clr_txt, pws->xf.color);

	memcpy((void*)&pws->msk, (void*)&pclr->clr_msk, sizeof(xcolor_t));

	memcpy((void*)&pws->ico, (void*)&pclr->clr_ico, sizeof(xcolor_t));

	if (_widget_has_subproc(wt))
	{
		_widget_send_command(wt, COMMAND_COLOR, IDC_SELF, (var_long)pclr);
	}

	if (dw & WD_STYLE_NOCHANGE)
		return;

	_widget_enum_child(wt, _widget_set_child_color_mode, (var_long)pclr);
}

void _widget_get_color_mode(res_win_t wt, clr_mod_t* pclr)
{
	X11_widget_t* pws = GETXDUSTRUCT(wt);
	dword_t dw = (pws) ? pws->style : 0;

	if (!pws)
		return;

	parse_xcolor(&pclr->clr_bkg, pws->xb.color);

	parse_xcolor(&pclr->clr_frg, pws->xp.color);

	parse_xcolor(&pclr->clr_txt, pws->xf.color);

	memcpy((void*)&pclr->clr_msk, (void*)&pws->msk, sizeof(xcolor_t));

	memcpy((void*)&pclr->clr_ico, (void*)&pws->ico, sizeof(xcolor_t));
}


/*******************************************************************************************/
void _message_quit(int code)
{

}

bool_t _message_translate(const msg_t* pmsg)
{
	X11_widget_t* pwt;
	if_event_t* pif;
	accel_t* pac;

	char keystr[5] = {0};
	KeySym keysys = 0;
	int i, keys, state = 0;
	char ch = 0;
	char* pch = NULL;
	char kch = 0;

	if(pmsg->type == KeyPress)
	{
		if(!_widget_is_valid(pmsg->xkey.window))
			return 0;
	
		if(XFilterEvent(pmsg, pmsg->xkey.window))
			return 0;

		pwt = GETXDUSTRUCT(pmsg->xkey.window);
		pif = GETXDUDISPATCH(pmsg->xkey.window);

		keys = XLookupString(&(pmsg->xkey), keystr, 4, &keysys, NULL);
		if(!keys)
		{
			/*if(!IsFunctionKey(keysys) && !IsMiscFunctionKey(keysys) && !IsCursorKey(keysys))
			{
				return 1;
			}*/
			return 0;
		}

		if(pmsg->xkey.state & ControlMask) state |= KEY_CONTROL;
		if(pmsg->xkey.state & Mod1Mask) state |= KEY_ALT;

		switch(keys)
		{
		case 1:
			ch = keystr[0];
			keysys = XLookupKeysym(&(pmsg->xkey), 0);
			break;
		case 2:
			ch = keystr[1];
			keysys = XLookupKeysym(&(pmsg->xkey), 1);
			break;
		case 3:
			ch = keystr[2];
			keysys = XLookupKeysym(&(pmsg->xkey), 2);
			break;
		default:
			ch = keystr[3];
			keysys = XLookupKeysym(&(pmsg->xkey), 3);
			break;
		}

		pch = XKeysymToString(keysys);
		kch = (pch)? *pch : 0;

		pac = (accel_t*)(pwt->acl);
		i = 0;
		while(pac && pac[i].vir != 0 && pac[i].key != 0)
		{
			if(pac[i].vir == state && pac[i].key == kch)
			{
				if(pif && pif->pf_on_menu_command)
				{
					(*pif->pf_on_menu_command)(pmsg->xkey.window, pac[i].cmd, 1, 0);
				}

				return 1;
			}

			i++;
		}

		if(state) 
		{
			return 1;
		}

		if(ch)
		{
			if(pif && pif->pf_on_char)
			{
				(*pif->pf_on_char)(pmsg->xkey.window, ch);
			}
		}
	}else if(pmsg->type == KeyRelease)
	{
		if(XFilterEvent(pmsg, pmsg->xkey.window))
			return 0;
	}

    return 0;
}

result_t _message_dispatch(const msg_t* pmsg)
{
	X11_widget_t* pwt;
	if_event_t* pif;
	
	switch(pmsg->type)
	{
		case KeymapNotify:
            XRefreshKeyboardMapping(&pmsg->xmapping);
            break;
		case KeyPress:
			if(!_widget_is_valid(pmsg->xkey.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xkey.window);
			pif = GETXDUDISPATCH(pmsg->xkey.window);
			if(pwt->disable)
				break;

			if(pif && pif->pf_on_keydown)
			{
				dword_t state = 0;
				if(pmsg->xkey.state & ShiftMask) state |= KS_WITH_SHIFT;
				if(pmsg->xkey.state & ControlMask) state |= KS_WITH_CONTROL;

				KeySym key;
				key = XLookupKeysym(&(pmsg->xkey), 0);
				(*pif->pf_on_keydown)(pmsg->xkey.window, state, key);
			}
			break;
		case KeyRelease:
			if(!_widget_is_valid(pmsg->xkey.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xkey.window);
			pif = GETXDUDISPATCH(pmsg->xkey.window);
			if(pwt->disable)
				break;

			if(pif && pif->pf_on_keyup)
			{
				dword_t state = 0;
				if(pmsg->xkey.state & ShiftMask) state |= KS_WITH_SHIFT;
				if(pmsg->xkey.state & ControlMask) state |= KS_WITH_CONTROL;

				KeySym key;
				key = XLookupKeysym(&(pmsg->xkey), 0);
				(*pif->pf_on_keyup)(pmsg->xkey.window, state, key);
			}
			break;
		case ButtonPress:
			if(!_widget_is_valid(pmsg->xbutton.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xbutton.window);
			pif = GETXDUDISPATCH(pmsg->xbutton.window);
			if(pwt->disable)
				break;

			if(pmsg->xbutton.button == Button1)
			{
				if (pif && pif->pf_on_lbutton_down)
				{
					xpoint_t xp;
					xp.x = pmsg->xbutton.x;
					xp.y = pmsg->xbutton.y;

					(*pif->pf_on_lbutton_down)(pmsg->xbutton.window, &xp);
				}
			}else if(pmsg->xbutton.button == Button3)
			{
				if (pif && pif->pf_on_rbutton_down)
				{
					xpoint_t xp;
					xp.x = pmsg->xbutton.x;
					xp.y = pmsg->xbutton.y;

					(*pif->pf_on_rbutton_down)(pmsg->xbutton.window, &xp);
				}
			}
			break;
		case ButtonRelease:
			if(!_widget_is_valid(pmsg->xbutton.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xbutton.window);
			pif = GETXDUDISPATCH(pmsg->xbutton.window);
			if(pwt->disable)
				break;

			if(pmsg->xbutton.button == Button1)
			{
				if (pif && pif->pf_on_lbutton_up)
				{
					xpoint_t xp;
					xp.x = pmsg->xbutton.x;
					xp.y = pmsg->xbutton.y;

					(*pif->pf_on_lbutton_up)(pmsg->xbutton.window, &xp);
				}
			}else if(pmsg->xbutton.button == Button3)
			{
				if (pif && pif->pf_on_rbutton_up)
				{
					xpoint_t xp;
					xp.x = pmsg->xbutton.x;
					xp.y = pmsg->xbutton.y;

					(*pif->pf_on_rbutton_up)(pmsg->xbutton.window, &xp);
				}
			}else if(pmsg->xbutton.button == Button4)
			{
				if (pif && pif->pf_on_wheel)
				{
					(*pif->pf_on_wheel)(pmsg->xbutton.window, 0, pwt->vs.min);
				}
			}else if(pmsg->xbutton.button == Button5)
			{
				if (pif && pif->pf_on_wheel)
				{
					(*pif->pf_on_wheel)(pmsg->xbutton.window, 0, -pwt->vs.min);
				}
			}
			break;
		case MotionNotify:
			if(!_widget_is_valid(pmsg->xmotion.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xmotion.window);
			pif = GETXDUDISPATCH(pmsg->xmotion.window);
			if(pwt->disable)
				break;

			if (pif && pif->pf_on_mouse_move)
			{
				dword_t state = 0;
				if(pmsg->xmotion.state & ShiftMask) state |= KS_WITH_SHIFT;
				if(pmsg->xmotion.state & ControlMask) state |= KS_WITH_CONTROL;
				if(pmsg->xmotion.state & Button1Mask) state |= MS_WITH_LBUTTON;
				if(pmsg->xmotion.state & Button3Mask) state |= MS_WITH_RBUTTON;

				xpoint_t xp;
				xp.x = pmsg->xmotion.x;
				xp.y = pmsg->xmotion.y;

				(*pif->pf_on_mouse_move)(pmsg->xmotion.window, state, &xp);
			}
			break;
		case EnterNotify:
			if(!_widget_is_valid(pmsg->xcrossing.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xcrossing.window);
			pif = GETXDUDISPATCH(pmsg->xcrossing.window);
			if(pwt->disable)
				break;

			if (pif && pif->pf_on_mouse_hover)
			{
				dword_t state = 0;
				if(pmsg->xcrossing.state & ShiftMask) state |= KS_WITH_SHIFT;
				if(pmsg->xcrossing.state & ControlMask) state |= KS_WITH_CONTROL;
				if(pmsg->xcrossing.state & Button1Mask) state |= MS_WITH_LBUTTON;
				if(pmsg->xcrossing.state & Button3Mask) state |= MS_WITH_RBUTTON;

				xpoint_t xp;
				xp.x = pmsg->xcrossing.x;
				xp.y = pmsg->xcrossing.y;

				(*pif->pf_on_mouse_hover)(pmsg->xcrossing.window, state, &xp);
			}
			break;
		case LeaveNotify:
			if(!_widget_is_valid(pmsg->xcrossing.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xcrossing.window);
			pif = GETXDUDISPATCH(pmsg->xcrossing.window);
			if(pwt->disable)
				break;

			if (pif && pif->pf_on_mouse_leave)
			{
				dword_t state = 0;
				if(pmsg->xcrossing.state & ShiftMask) state |= KS_WITH_SHIFT;
				if(pmsg->xcrossing.state & ControlMask) state |= KS_WITH_CONTROL;
				if(pmsg->xcrossing.state & Mod1Mask) state |= MS_WITH_LBUTTON;
				if(pmsg->xcrossing.state & Mod2Mask) state |= MS_WITH_RBUTTON;

				xpoint_t xp;
				xp.x = pmsg->xcrossing.x;
				xp.y = pmsg->xcrossing.y;

				(*pif->pf_on_mouse_leave)(pmsg->xcrossing.window, state, &xp);
			}
			break;
		case Expose:
			if(!_widget_is_valid(pmsg->xexpose.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xexpose.window);
			pif = GETXDUDISPATCH(pmsg->xexpose.window);
			if(pif && pif->pf_on_paint)
			{
				xrect_t xr = {0};
				xr.x = pmsg->xexpose.x;
				xr.y = pmsg->xexpose.y;
				xr.w = pmsg->xexpose.width;
				xr.h = pmsg->xexpose.height;

				(*pif->pf_on_paint)(pmsg->xexpose.window, pwt->wgc, &xr);
			}
			break;
		case FocusIn:
			if(!_widget_is_valid(pmsg->xfocus.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xfocus.window);
			pif = GETXDUDISPATCH(pmsg->xfocus.window);
			if(pwt->disable)
				break;

			if (pif && pif->pf_on_set_focus)
			{
				XIMStyles *styles;

				if(!g_xim) break;

				if(XGetIMValues(g_xim, XNQueryInputStyle, &styles, NULL) != NULL)
					break;

				pwt->xic = XCreateIC(g_xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, pmsg->xfocus.window, NULL);
				if(!pwt->xic)
				{
					XFree(styles);
					break;
				}

				XSetICFocus(pwt->xic);
				XFree(styles);

				(*pif->pf_on_set_focus)(pmsg->xfocus.window, pmsg->xfocus.window);
			}
			break;
		case FocusOut:
			if(!_widget_is_valid(pmsg->xfocus.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xfocus.window);
			pif = GETXDUDISPATCH(pmsg->xfocus.window);
			if(pwt->disable)
				break;

			if(!pwt->xic) break;

			XUnsetICFocus(pwt->xic);

			XDestroyIC(pwt->xic);
			pwt->xic = (XIC)0;
			
			if (pif && pif->pf_on_kill_focus)
			{
				(*pif->pf_on_kill_focus)(pmsg->xfocus.window, pmsg->xfocus.window);
			}
			break;
		case ConfigureNotify:
			if(!_widget_is_valid(pmsg->xconfigure.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xconfigure.window);
			pif = GETXDUDISPATCH(pmsg->xconfigure.window);

			if(pwt->pt.x != pmsg->xconfigure.x || pwt->pt.y != pmsg->xconfigure.y)
			{
				if(pif && pif->pf_on_move)
				{
					xpoint_t xp;
					xp.x = pmsg->xconfigure.x;
					xp.y = pmsg->xconfigure.y;

					(*pif->pf_on_move)(pmsg->xconfigure.window, &xp);
				}
				pwt->pt.x = pmsg->xconfigure.x;
				pwt->pt.y = pmsg->xconfigure.y;
			}

			if(pwt->st.cx != pmsg->xconfigure.width || pwt->st.cy != pmsg->xconfigure.height)
			{
				if(pif && pif->pf_on_size)
				{
					xsize_t st;
					st.cx = pmsg->xconfigure.width;
					st.cy = pmsg->xconfigure.height;

					(*pif->pf_on_size)(pmsg->xconfigure.window, WS_SIZE_RESTORE, &st);
				}
				pwt->st.cx = pmsg->xconfigure.width;
				pwt->st.cy = pmsg->xconfigure.height;
			}
			break;
		case MapNotify:
			if(!_widget_is_valid(pmsg->xmap.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xmap.window);
			pif = GETXDUDISPATCH(pmsg->xmap.window);
			if(pif && pif->pf_on_show)
			{
				(*pif->pf_on_show)(pmsg->xmap.window, 1);
			}
			break;
		case UnmapNotify:
			if(!_widget_is_valid(pmsg->xunmap.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xunmap.window);
			pif = GETXDUDISPATCH(pmsg->xunmap.window);
			if(pif && pif->pf_on_show)
			{
				(*pif->pf_on_show)(pmsg->xunmap.window, 0);
			}
			break;
		case CreateNotify:
			break;
		case DestroyNotify:
			break;
		case ClientMessage:
			if(!_widget_is_valid(pmsg->xclient.window))
				break;
			pwt = GETXDUSTRUCT(pmsg->xclient.window);
			pif = GETXDUDISPATCH(pmsg->xclient.window);
			if(pwt->disable)
				break;

			if(pmsg->xclient.message_type == g_atoms.wm_command)
			{
				if(pmsg->xclient.data.l[0] == IDC_PARENT)
				{
					if(pif && pif->pf_on_parent_command)
					{
						(*pif->pf_on_parent_command)(pmsg->xclient.window, (int)(pmsg->xclient.data.l[1]), (var_long)(pmsg->xclient.data.l[2]));
					}
				}else if(pmsg->xclient.data.l[0] == IDC_CHILD)
				{
					if(pif && pif->pf_on_child_command)
					{
						(*pif->pf_on_child_command)(pmsg->xclient.window, (int)(pmsg->xclient.data.l[1]), (var_long)(pmsg->xclient.data.l[2]));
					}
				}else if(pmsg->xclient.data.l[0] == IDC_SELF)
				{
					if(pif && pif->pf_on_self_command)
					{
						(*pif->pf_on_self_command)(pmsg->xclient.window, (int)(pmsg->xclient.data.l[1]), (var_long)(pmsg->xclient.data.l[2]));
					}
				}else
				{
					if(pif && pif->pf_on_menu_command)
					{
						(*pif->pf_on_menu_command)(pmsg->xclient.window, (int)(pmsg->xclient.data.l[1]), (int)(pmsg->xclient.data.l[0]), (var_long)(pmsg->xclient.data.l[2]));
					}
				}
				break;
			}

			if(pmsg->xclient.message_type == g_atoms.wm_notice)
			{
					if(pif && pif->pf_on_notice)
					{
						(*pif->pf_on_notice)(pmsg->xclient.window, (NOTICE*)(pmsg->xclient.data.l[2]));
					}
			}

			if(pmsg->xclient.message_type == g_atoms.wm_scroll)
			{
				if(pmsg->xclient.data.l[0] == 1)
				{
					if(pif && pif->pf_on_scroll)
					{
						(*pif->pf_on_scroll)(pmsg->xclient.window, (bool_t)1, (int)(pmsg->xclient.data.l[1]));
					}
				}else
				{
					if(pif && pif->pf_on_scroll)
					{
						(*pif->pf_on_scroll)(pmsg->xclient.window, (bool_t)0, (int)(pmsg->xclient.data.l[1]));
					}
				}
				break;
			}

			if(pmsg->xclient.message_type == g_atoms.net_active_window)
			{
				if(pif && pif->pf_on_activate)
				{
					(*pif->pf_on_activate)(pmsg->xclient.window, 1);
				}
				break;
			}

			if(pmsg->xclient.message_type == g_atoms.wm_protocols && pmsg->xclient.data.l[0] == g_atoms.wm_take_focus)
			{
				_widget_set_focus(pmsg->xclient.window);
				break;
			}

			if(pmsg->xclient.message_type == g_atoms.wm_protocols && pmsg->xclient.data.l[0] == g_atoms.wm_delete_window)
			{
				_widget_close(pmsg->xclient.window, 0);
				break;
			}
			break;
	}

	return 0;
}

void _message_fetch(msg_t* pmsg, res_win_t wt)
{
	XWindowAttributes attr = {0};
	Bool rt;
	int x, y;
	Window cld;

    if(wt)
	{
		XGetWindowAttributes(g_display, wt, &attr);
		XWindowEvent(g_display, wt, attr.your_event_mask, pmsg);

		return;
	}

    XNextEvent(g_display, pmsg);

	if(pmsg->type == ButtonPress || pmsg->type == ButtonRelease)
	{
		if(g_capture && g_capture != pmsg->xbutton.window)
		{
			x = pmsg->xbutton.x;
			y = pmsg->xbutton.y;

			XTranslateCoordinates(g_display, pmsg->xbutton.window, RootWindow(g_display, DefaultScreen(g_display)), pmsg->xbutton.x, pmsg->xbutton.y, &x, &y, &cld);

			pmsg->xbutton.window = g_capture;

			XTranslateCoordinates(g_display, RootWindow(g_display, DefaultScreen(g_display)), pmsg->xbutton.window, x, y, &(pmsg->xbutton.x), &(pmsg->xbutton.y), &cld);
		}
	}

	if(pmsg->type == MotionNotify)
	{
		if(g_capture && g_capture != pmsg->xmotion.window)
		{
			x = pmsg->xmotion.x;
			y = pmsg->xmotion.y;

			XTranslateCoordinates(g_display, pmsg->xmotion.window, RootWindow(g_display, DefaultScreen(g_display)), pmsg->xmotion.x, pmsg->xmotion.y, &x, &y, &cld);

			pmsg->xmotion.window = g_capture;

			XTranslateCoordinates(g_display, RootWindow(g_display, DefaultScreen(g_display)), pmsg->xmotion.window, x, y, &(pmsg->xmotion.x), &(pmsg->xmotion.y), &cld);
		}
	}

	if(g_capture && (pmsg->type == EnterNotify || pmsg->type == LeaveNotify))
	{
		x = pmsg->xcrossing.x;
		y = pmsg->xcrossing.y;

		XTranslateCoordinates(g_display, pmsg->xcrossing.window, RootWindow(g_display, DefaultScreen(g_display)), pmsg->xcrossing.x, pmsg->xcrossing.y, &x, &y, &cld);

		pmsg->type = MotionNotify;
		pmsg->xmotion.type = MotionNotify;
		pmsg->xmotion.send_event = pmsg->xcrossing.send_event;
		pmsg->xmotion.display = pmsg->xcrossing.display;
		pmsg->xmotion.window = g_capture;
		pmsg->xmotion.root = pmsg->xcrossing.root;
		pmsg->xmotion.subwindow = 0;
		pmsg->xmotion.time = pmsg->xcrossing.time;
		pmsg->xmotion.x_root = pmsg->xcrossing.x_root;
		pmsg->xmotion.y_root = pmsg->xcrossing.y_root;
		pmsg->xmotion.state = pmsg->xcrossing.state;
		pmsg->xmotion.is_hint = 0;
		pmsg->xmotion.same_screen = pmsg->xcrossing.same_screen;
	
		XTranslateCoordinates(g_display, RootWindow(g_display, DefaultScreen(g_display)), pmsg->xmotion.window, x, y, &(pmsg->xmotion.x), &(pmsg->xmotion.y), &cld);
	}
}

bool_t _message_peek(msg_t* pmsg)
{
	if(!XPending(g_display))
		return 0;

    XPeekEvent(g_display, pmsg);
	
	return 1;
}

void _message_position(xpoint_t* ppt)
{

}

int	_widget_do_normal(res_win_t wt)
{
	msg_t msg = {0};
    
	while(_widget_is_valid(wt))
    {
		_message_fetch(&msg, (res_win_t)0);
		
		if(_message_translate(&msg))
			continue;

		_message_dispatch(&msg);
    }

	return 0;
}

int	_widget_do_modal(res_win_t wt)
{
	msg_t msg = {0};
	

	return 0;
}

void _widget_do_trace(res_win_t wt)
{
	msg_t msg = {0};
    
	XSync(g_display, True);

	_widget_set_capture(wt, 1);

	while(_widget_is_valid(wt))
    {
		_message_fetch(&msg, (res_win_t)0);
		
		_message_dispatch(&msg);
    }

	_widget_set_capture(wt, 0);
}

/*********************************************************************************************************/
                          
void _get_screen_size(xsize_t* pxs)
{
	pxs->cx = DisplayWidth(g_display, DefaultScreen(g_display));
    pxs->cy = DisplayHeight(g_display, DefaultScreen(g_display));
}

void _get_desktop_size(xsize_t* pxs)
{
	pxs->cx = DisplayWidth(g_display, DefaultScreen(g_display));
    pxs->cy = DisplayHeight(g_display, DefaultScreen(g_display));
}

void _screen_size_to_tm(xsize_t* pxs)
{
	double dx, dy;

	dx = (double)DisplayWidthMM(g_display, DefaultScreen(g_display)) / (double)DisplayWidth(g_display, DefaultScreen(g_display));
	dx = (double)DisplayHeightMM(g_display, DefaultScreen(g_display)) / (double)DisplayHeight(g_display, DefaultScreen(g_display));

	pxs->fx = (float)((double)pxs->cx * dx);
	pxs->fy = (float)((double)pxs->cy * dy);
}

void _screen_size_to_pt(xsize_t* pxs)
{
	double dx, dy;

	dx = (double)DisplayWidth(g_display, DefaultScreen(g_display)) / (double)DisplayWidthMM(g_display, DefaultScreen(g_display));
	dx = (double)DisplayHeight(g_display, DefaultScreen(g_display)) / (double)DisplayHeightMM(g_display, DefaultScreen(g_display));

	pxs->cx = (int)((double)pxs->fx * dx);
	pxs->cy = (int)((double)pxs->fx * dy);
}

res_acl_t _create_accel_table(const accel_t* pac, int n)
{
	accel_t* pnew;

	pnew = (accel_t*)calloc(n + 1, sizeof(accel_t));
	memcpy((void*)pnew, (void*)pac, n * sizeof(accel_t));

	pnew[n].vir = 0;
	pnew[n].key = 0;
	pnew[n].cmd = 0;

	return (res_acl_t)pnew;
}

void _destroy_accel_table(res_acl_t hac)
{
	accel_t* pac = (accel_t*)hac;

	free(pac);
}

#ifdef XDU_SUPPORT_WIDGET_EX
void _widget_track_mouse(res_win_t wt, dword_t mask)
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
#endif //XDU_SUPPORT_WIDGET_EX



#endif //XDU_SUPPORT_WIDGET
