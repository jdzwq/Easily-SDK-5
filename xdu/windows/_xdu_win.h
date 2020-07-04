/***********************************************************************
	Easily xdu v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdu windows definition document

	@module	_xdu_win.h | windows interface file

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

#ifndef _XDU_WIN_H
#define _XDU_WIN_H

#define XDU_SUPPORT_SHELL_DIALOG
#define XDU_SUPPORT_SHELL
#define XDU_SUPPORT_CLIPBOARD
#define XDU_SUPPORT_BLUT

#define XDU_SUPPORT_CONTEXT_BITMAP
#define XDU_SUPPORT_CONTEXT_PRINTER
#define XDU_SUPPORT_CONTEXT_REGION
//#define XDU_SUPPORT_CONTEXT_OPENGL
#if defined(_UNICODE) || defined(UNICODE)
#define XDU_SUPPORT_CONTEXT_GDIPLUS
#else
#define XDU_SUPPORT_CONTEXT_GDI
#endif
#define XDU_SUPPORT_CONTEXT

#define XDU_SUPPORT_WIDGET_NC
#define XDU_SUPPORT_WIDGET_REGION
#define XDU_SUPPORT_WIDGET

#if defined(WINCE)
#undef XDU_SUPPORT_CONTEXT_BITMAP_THUMB
#undef XDU_SUPPORT_CONTEXT_PRINTER
#undef XDU_SUPPORT_WIDGET_NC
#undef XDU_SUPPORT_WIDGET_REGION
#undef XDU_SUPPORT_SHELL
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT    0x0600

#ifdef WINVER
#undef WINVER
#endif

#if defined(WINCE) 
#define WINVER			_WIN32_WCE 
#else
#define WINVER			0x0501
#endif

#ifndef OEMRESOURCE 
#define OEMRESOURCE 
#endif

#ifndef STRICT
#define STRICT
#endif

#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <time.h>

#ifdef XDU_SUPPORT_CONTEXT
#include <ole2.h>
#include <olectl.h>
#endif
#ifdef XDU_SUPPORT_WIDGET
#include <commctrl.h>
#endif
#ifdef XDU_SUPPORT_SHELL
#include <ShellAPI.h>
#include <ShlObj.h>
#endif

#ifdef XDU_SUPPORT_BLUT
#include <BluetoothAPIs.h>
#include <Ws2bth.h>
#include <KS.H>
#endif

#ifdef XDU_SUPPORT_CONTEXT_OPENGL
#include <gl/GL.h>
#include <gl/GLU.h>
#endif

#ifndef uid_t
typedef unsigned int	uid_t;
#endif

#ifdef XDU_SUPPORT_CONTEXT
typedef struct _win32_context_t{
	HDC context;
	union
	{
		HBITMAP bitmap;
		HWND widget;
	}device;
	int type;
}win32_context_t;

typedef win32_context_t* res_ctx_t;
typedef COLORREF	res_clr_t;
typedef HFONT		res_font_t;
#ifdef XDU_SUPPORT_CONTEXT_BITMAP
typedef HBITMAP		res_bmp_t;
#endif
#ifdef XDU_SUPPORT_CONTEXT_REGION
typedef HRGN		res_rgn_t;
#endif
#ifdef XDU_SUPPORT_CONTEXT_OPENGL
typedef HGLRC		res_glc_t;
#endif
#endif

#ifdef XDU_SUPPORT_WIDGET
typedef MSG			msg_t;
typedef HACCEL		res_acl_t;
typedef HWND		res_win_t;
typedef WPARAM		uparam_t;
typedef LPARAM		lparam_t;
typedef LRESULT		result_t;
#endif

#ifdef XDU_SUPPORT_CLIPBOARD
#ifndef res_glob_t
typedef HANDLE		res_glob_t;
#endif
/*clipboard format*/
#define CB_FORMAT_MBS		CF_TEXT
#define CB_FORMAT_UCS		CF_UNICODETEXT
#define CB_FORMAT_DIB		CF_DIB

#ifdef _UNICODE
#define DEF_CB_FORMAT		CB_FORMAT_UCS
#else
#define DEF_CB_FORMAT		CB_FORMAT_MBS
#endif
#endif

#ifdef XDU_SUPPORT_WIDGET
#define WM_EASYMSG_MIN		WM_USER + 10
#define WM_EASYMSG_MAX		WM_USER  + 100


#ifdef XDU_SUPPORT_WIDGET_NC
/*widget nc hit test*/
#define HINT_NOWHERE	HTNOWHERE
#define HINT_CLIENT		HTCLIENT
#define HINT_MINIMIZE	HTMINBUTTON
#define HINT_MAXIMIZE	HTMAXBUTTON
#define HINT_RESTORE	HTSIZE
#define HINT_CLOSE		HTCLOSE
#define HINT_TITLE		HTCAPTION
#define HINT_TOP		HTTOP
#define HINT_TOPLEFT	HTTOPLEFT
#define HINT_TOPRIGHT	HTTOPRIGHT
#define HINT_LEFT		HTLEFT
#define HINT_RIGHT		HTRIGHT
#define HINT_BOTTOM		HTBOTTOM
#define HINT_LEFTBOTTOM	HTBOTTOMLEFT
#define HINT_RIGHTBOTTOM	HTBOTTOMRIGHT
#define HINT_BORDER		HTBORDER
#define HINT_ICON		HTHELP
#define HINT_MENUBAR	100
#define HINT_HSCROLL	101
#define HINT_VSCROLL	102
#define HINT_PAGEUP		103
#define HINT_PAGEDOWN	104
#define HINT_LINEUP		105
#define HINT_LINEDOWN	106
#define HINT_LINELEFT	107
#define HINT_LINERIGHT	108
#endif
#endif


#endif //_XDU_WIN_H