/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdl macos definition document

	@module	_xdl_macos.h | macos definition interface file

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

#ifndef _XDK_MACOS_H
#define _XDK_MACOS_H

#define XDK_SUPPORT_MEMO_HEAP
#define XDK_SUPPORT_MEMO_PAGE
#define XDK_SUPPORT_MEMO_GLOB
#define XDK_SUPPORT_MEMO_LOCAL
#define XDK_SUPPORT_MEMO_CACHE
#define XDK_SUPPORT_MEMO
#define XDK_SUPPORT_ERROR
#define XDK_SUPPORT_DATE
#define XDK_SUPPORT_MBCS
#define XDK_SUPPORT_ASYNC
#define XDK_SUPPORT_THREAD_EVENT
#define XDK_SUPPORT_THREAD_CRITI
#define XDK_SUPPORT_THREAD_MUTEX
#define XDK_SUPPORT_THREAD_SEMAP
#define XDK_SUPPORT_THREAD_QUEUE
#define XDK_SUPPORT_THREAD
#define XDK_SUPPORT_PROCESS
#define XDK_SUPPORT_FILE_FIND
#define XDK_SUPPORT_FILE
#define XDK_SUPPORT_SHARE
#define XDK_SUPPORT_PIPE
#define XDK_SUPPORT_CONS
#define XDK_SUPPORT_COMM
#define XDK_SUPPORT_SOCK
#define XDK_SUPPORT_TIMER
#define XDK_SUPPORT_SHELL

#define XDK_SUPPORT_CONTEXT
#define XDK_SUPPORT_CONTEXT_REGION
#define XDK_SUPPORT_CONTEXT_BITMAP
#define XDK_SUPPORT_CONTEXT_GRAPHIC

#define XDK_SUPPORT_WIDGET

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <locale.h>

#include <malloc/malloc.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/event.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/ttycom.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/event.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <util.h>

#include <termios.h>
#include <pthread.h> 

//macos
#include <mach-o/dyld.h>

#ifdef XDK_SUPPORT_CONTEXT
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/Xft/Xft.h>
#endif

#if defined(_UNICODE) || defined(UNICODE)
#define _T(x)      L ## x
#else
#define _T(x)      x
#endif

#ifdef XDK_SUPPORT_SOCK
typedef struct sockaddr_in	net_addr_t;
typedef struct in_addr		sin_addr_t;
typedef void*				res_addr_t;
#endif

#ifdef XDK_SUPPORT_MEMO
#ifdef XDK_SUPPORT_MEMO_GLOB
typedef void*		res_glob_t;
#endif
#ifdef XDK_SUPPORT_MEMO_HEAP
typedef void*		res_heap_t;
#endif
#endif

#ifdef XDK_SUPPORT_FILE
typedef int         res_file_t;
#ifdef XDK_SUPPORT_FILE_FIND
typedef void*		res_find_t;
#endif
#endif

#define INVALID_FILE	((int)(-1))

#ifdef XDK_SUPPORT_CONS
typedef void(*MAC_SIGNAL_HANDLER)(int sig);
#endif

#ifdef XDK_SUPPORT_THREAD
typedef unsigned long tls_key_t;

#ifdef XDK_SUPPORT_THREAD_EVENT
typedef void*		res_even_t;
#endif
#ifdef XDK_SUPPORT_THREAD_MUTEX
typedef void*		res_mutx_t;
#endif
#ifdef XDK_SUPPORT_THREAD_CRITI
typedef void*       res_crit_t;
#endif
#ifdef XDK_SUPPORT_THREAD_SEMAP
typedef void*       res_sema_t;
#endif
#ifdef XDK_SUPPORT_THREAD_QUEUE
typedef int         res_queue_t;
#endif
typedef void*(*MAC_THREAD_PROC)(void* param);
#endif

#ifdef XDK_SUPPORT_PROCESS
typedef void*		res_modu_t;
#endif

#ifdef XDK_SUPPORT_TIMER
typedef void*		res_timer_t;
typedef void(*MAC_TIMER_PROC)(void* param, unsigned char wait);
#endif

typedef void*       res_hand_t;


#ifdef XDK_SUPPORT_COMM
#define COMM_EVNET_RING		TIOCM_RNG
#define COMM_EVNET_RLSD		TIOCM_LE
#define COMM_EVNET_CTS		TIOCM_CTS
#define COMM_EVNET_DSR		TIOCM_DSR
#define COMM_EVNET_RXCHAR	0x1000
#define COMM_EVNET_TXEMPTY	0x2000
#define COMM_EVNET_BREAK	0x4000
#define COMM_EVNET_ERROR	-1
#define COMM_EVENT_IDLE		0x8000

#define COMM_ERROR_BREAK	-1
#define COMM_ERROR_DNS		-2
#define COMM_ERROR_FRAME	-3
#define COMM_ERROR_IOE		-4
#define COMM_ERROR_MODE		-5
#define COMM_ERROR_OOP		-6
#define COMM_ERROR_OVERRUN	-7
#define COMM_ERROR_PTO		-8
#define COMM_ERROR_RXOVER	-9
#define COMM_ERROR_RXPARITY	-10
#define COMM_ERROR_TXFULL	-11

#define COMM_STATUS_CTS_ON	0x0001
#define COMM_STATUS_DSR_ON	0x0002
#define COMM_STATUS_RING_ON	0x0004
#define COMM_STATUS_RLSD_ON	0x0008

#endif

#ifdef XDK_SUPPORT_SOCK
/*socket event*/
#ifndef FD_READ
#define FD_READ			0x00000001
#endif

#ifndef FD_WRITE
#define FD_WRITE		0x00000002
#endif

#ifndef FD_ACCEPT
#define FD_ACCEPT		0x00000004
#endif
#endif /*XDK_SUPPORT_SOCK*/

#define MAX_EVENT  3

typedef struct OVERLAPPED{
    union{
        struct timeval tv;
        struct timespec tp;
    };
    
    union{
        struct fd_set fd[MAX_EVENT];
        struct kevent ev[MAX_EVENT];
    };
}OVERLAPPED, *LPOVERLAPPED;

#ifndef LPSIZE
typedef size_t*     LPSIZE;
#endif

#ifdef XDK_SUPPORT_CONTEXT
typedef struct _X11_suface_t{
    Drawable device;
    GC context;
    int memo;
}X11_suface_t;

extern Display*     g_display;

typedef X11_suface_t* res_ctx_t;
typedef Pixmap      res_pmp_t;
typedef Colormap    res_clr_t;
typedef Font		res_font_t;
#ifdef XDK_SUPPORT_CONTEXT_BITMAP
typedef XImage*		res_bmp_t;
#endif
#ifdef XDK_SUPPORT_CONTEXT_REGION
typedef Region		res_rgn_t;
#endif
#endif

#ifdef XDK_SUPPORT_WIDGET
typedef XEvent      msg_t;
typedef unsigned long	res_acl_t;
typedef unsigned long	wparam_t;
typedef void*       lparam_t;
typedef int         result_t;
typedef Window      res_win_t;

typedef struct _X11_create_struct_t{
    void*      param;
    res_modu_t  inst;
    res_win_t   parent;
    int         cy;
    int         cx;
    int         y;
    int         x;
    const char*     wname;
    const char*     wclass;
    unsigned long   stylex;
}X11_create_struct_t;


/*windows message*/

#define WM_NCCREATE                     0x0081
#define WM_NCDESTROY                    0x0082
#define WM_NCCALCSIZE                   0x0083
#define WM_NCHITTEST                    0x0084
#define WM_NCPAINT                      0x0085
#define WM_NCACTIVATE                   0x0086
#define WM_GETDLGCODE                   0x0087
#define WM_NCMOUSEMOVE                  0x00A0
#define WM_NCLBUTTONDOWN                0x00A1
#define WM_NCLBUTTONUP                  0x00A2
#define WM_NCLBUTTONDBLCLK              0x00A3
#define WM_NCRBUTTONDOWN                0x00A4
#define WM_NCRBUTTONUP                  0x00A5
#define WM_NCRBUTTONDBLCLK              0x00A6
#define WM_NCMBUTTONDOWN                0x00A7
#define WM_NCMBUTTONUP                  0x00A8
#define WM_NCMBUTTONDBLCLK              0x00A9

#define WM_NULL                         0x0000
#define WM_CREATE                       0x0001
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005
#define WM_ACTIVATE                     0x0006
#define WM_SETFOCUS                     0x0007
#define WM_KILLFOCUS                    0x0008
#define WM_ENABLE                       0x000A
#define WM_SETREDRAW                    0x000B
#define WM_SETTEXT                      0x000C
#define WM_GETTEXT                      0x000D
#define WM_GETTEXTLENGTH                0x000E
#define WM_PAINT                        0x000F
#define WM_CLOSE                        0x0010
#define WM_QUIT                         0x0012
#define WM_ERASEBKGND                   0x0014
#define WM_SYSCOLORCHANGE               0x0015
#define WM_SHOWWINDOW                   0x0018
#define WM_WININICHANGE                 0x001A
#define WM_DEVMODECHANGE                0x001B
#define WM_ACTIVATEAPP                  0x001C
#define WM_FONTCHANGE                   0x001D
#define WM_TIMECHANGE                   0x001E
#define WM_CANCELMODE                   0x001F
#define WM_SETCURSOR                    0x0020
#define WM_MOUSEACTIVATE                0x0021
#define WM_CHILDACTIVATE                0x0022
#define WM_QUEUESYNC                    0x0023
#define WM_GETMINMAXINFO                0x0024
#define WM_PAINTICON                    0x0026
#define WM_ICONERASEBKGND               0x0027
#define WM_NEXTDLGCTL                   0x0028
#define WM_SPOOLERSTATUS                0x002A
#define WM_DRAWITEM                     0x002B
#define WM_MEASUREITEM                  0x002C
#define WM_DELETEITEM                   0x002D
#define WM_VKEYTOITEM                   0x002E
#define WM_CHARTOITEM                   0x002F
#define WM_SETFONT                      0x0030
#define WM_GETFONT                      0x0031
#define WM_SETHOTKEY                    0x0032
#define WM_GETHOTKEY                    0x0033
#define WM_QUERYDRAGICON                0x0037
#define WM_COMPAREITEM                  0x0039
#define WM_COMPACTING                   0x0041
#define WM_COMMNOTIFY                   0x0044
#define WM_WINDOWPOSCHANGING            0x0046
#define WM_WINDOWPOSCHANGED             0x0047
#define WM_POWER                        0x0048
#define WM_COPYDATA                     0x004A
#define WM_CANCELJOURNAL                0x004B
#define WM_NOTIFY                       0x004E
#define WM_INPUTLANGCHANGEREQUEST       0x0050
#define WM_INPUTLANGCHANGE              0x0051
#define WM_TCARD                        0x0052
#define WM_HELP                         0x0053
#define WM_USERCHANGED                  0x0054
#define WM_NOTIFYFORMAT                 0x0055
#define WM_CONTEXTMENU                  0x007B
#define WM_STYLECHANGING                0x007C
#define WM_STYLECHANGED                 0x007D
#define WM_DISPLAYCHANGE                0x007E
#define WM_GETICON                      0x007F
#define WM_SETICON                      0x0080
#define WM_KEYFIRST                     0x0100
#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_CHAR                         0x0102
#define WM_DEADCHAR                     0x0103
#define WM_SYSKEYDOWN                   0x0104
#define WM_SYSKEYUP                     0x0105
#define WM_SYSCHAR                      0x0106
#define WM_SYSDEADCHAR                  0x0107
#define WM_IME_STARTCOMPOSITION         0x010D
#define WM_IME_ENDCOMPOSITION           0x010E
#define WM_IME_COMPOSITION              0x010F
#define WM_IME_KEYLAST                  0x010F
#define WM_INITDIALOG                   0x0110
#define WM_COMMAND                      0x0111
#define WM_SYSCOMMAND                   0x0112
#define WM_TIMER                        0x0113
#define WM_HSCROLL                      0x0114
#define WM_VSCROLL                      0x0115
#define WM_INITMENU                     0x0116
#define WM_INITMENUPOPUP                0x0117
#define WM_MENUSELECT                   0x011F
#define WM_MENUCHAR                     0x0120
#define WM_ENTERIDLE                    0x0121
#define WM_MENURBUTTONUP                0x0122
#define WM_MENUDRAG                     0x0123
#define WM_MENUGETOBJECT                0x0124
#define WM_UNINITMENUPOPUP              0x0125
#define WM_MENUCOMMAND                  0x0126
#define WM_CTLCOLORMSGBOX               0x0132
#define WM_CTLCOLOREDIT                 0x0133
#define WM_CTLCOLORLISTBOX              0x0134
#define WM_CTLCOLORBTN                  0x0135
#define WM_CTLCOLORDLG                  0x0136
#define WM_CTLCOLORSCROLLBAR            0x0137
#define WM_CTLCOLORSTATIC               0x0138
#define MN_GETHMENU                     0x01E1
#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#define WM_MOUSEWHEEL                   0x020A
#define WM_XBUTTONDOWN                  0x020B
#define WM_XBUTTONUP                    0x020C
#define WM_XBUTTONDBLCLK                0x020D
#define WM_MOUSEHWHEEL                  0x020E
#define WM_MDICREATE                    0x0220
#define WM_MDIDESTROY                   0x0221
#define WM_MDIACTIVATE                  0x0222
#define WM_MDIRESTORE                   0x0223
#define WM_MDINEXT                      0x0224
#define WM_MDIMAXIMIZE                  0x0225
#define WM_MDITILE                      0x0226
#define WM_MDICASCADE                   0x0227
#define WM_MDIICONARRANGE               0x0228
#define WM_MDIGETACTIVE                 0x0229
#define WM_MDISETMENU                   0x0230
#define WM_ENTERSIZEMOVE                0x0231
#define WM_EXITSIZEMOVE                 0x0232
#define WM_DROPFILES                    0x0233
#define WM_MDIREFRESHMENU               0x0234
#define WM_TOUCH                        0x0240
#define WM_IME_SETCONTEXT               0x0281
#define WM_IME_NOTIFY                   0x0282
#define WM_IME_CONTROL                  0x0283
#define WM_IME_COMPOSITIONFULL          0x0284
#define WM_IME_SELECT                   0x0285
#define WM_IME_CHAR                     0x0286
#define WM_MOUSEHOVER                   0x02A1
#define WM_MOUSELEAVE                   0x02A3
#define WM_CUT                          0x0300
#define WM_COPY                         0x0301
#define WM_PASTE                        0x0302
#define WM_CLEAR                        0x0303
#define WM_UNDO                         0x0304
#define WM_RENDERFORMAT                 0x0305
#define WM_RENDERALLFORMATS             0x0306
#define WM_DESTROYCLIPBOARD             0x0307
#define WM_DRAWCLIPBOARD                0x0308
#define WM_PAINTCLIPBOARD               0x0309
#define WM_VSCROLLCLIPBOARD             0x030A
#define WM_SIZECLIPBOARD                0x030B
#define WM_ASKCBFORMATNAME              0x030C
#define WM_CHANGECBCHAIN                0x030D
#define WM_HSCROLLCLIPBOARD             0x030E
#define WM_QUERYNEWPALETTE              0x030F
#define WM_PALETTEISCHANGING            0x0310
#define WM_PALETTECHANGED               0x0311
#define WM_HOTKEY                       0x0312

#define WM_EASYMSG_MIN		(0x0400 + 10)
#define WM_EASYMSG_MAX		(0x0400 + 100)

/*mouse button state*/
#define MS_WITH_LBUTTON		0x0001
#define MS_WITH_RBUTTON		0x0002
#define MS_WITH_MBUTTON		0x0010
#define MS_WITH_CONTROL		0x0008
#define MS_WITH_SHIFT		0x0004

/*mouse track state*/
#define MS_TRACK_HOVER		0x00000001
#define	MS_TRACK_LEAVE		0x00000002
#define MS_TRACK_CANCEL		0x80000000

/*widget size mode*/
#define WD_SIZE_RESTORE		0
#define WD_SIZE_MINIMIZED	1
#define WD_SIZE_MAXIMIZED	2
#define WD_SIZE_LAYOUT		9

/*widget activate mode*/
#define WD_ACTIVE_NONE		0
#define WD_ACTIVE_OTHER		1
#define WD_ACTIVE_CLICK		2

/*widget position mode*/
#define WD_ZORDER_TOP		(0)
#define WD_ZORDER_BOTTOM	(1)
#define WD_ZORDER_TOPMOST	(-1)
#define WD_ZORDER_NOTOPMOST	(-2)

#define MA_ACTIVATE         1
#define MA_ACTIVATEANDEAT   2
#define MA_NOACTIVATE       3
#define MA_NOACTIVATEANDEAT 4

#define SB_LINEUP           0
#define SB_LINELEFT         0
#define SB_LINEDOWN         1
#define SB_LINERIGHT        1
#define SB_PAGEUP           2
#define SB_PAGELEFT         2
#define SB_PAGEDOWN         3
#define SB_PAGERIGHT        3
#define SB_THUMBPOSITION    4
#define SB_THUMBTRACK       5
#define SB_TOP              6
#define SB_LEFT             6
#define SB_BOTTOM           7
#define SB_RIGHT            7
#define SB_ENDSCROLL        8

#define SC_SIZE         0xF000
#define SC_MOVE         0xF010
#define SC_MINIMIZE     0xF020
#define SC_MAXIMIZE     0xF030
#define SC_NEXTWINDOW   0xF040
#define SC_PREVWINDOW   0xF050
#define SC_CLOSE        0xF060
#define SC_VSCROLL      0xF070
#define SC_HSCROLL      0xF080
#define SC_MOUSEMENU    0xF090
#define SC_KEYMENU      0xF100
#define SC_ARRANGE      0xF110
#define SC_RESTORE      0xF120
#define SC_TASKLIST     0xF130
#define SC_SCREENSAVE   0xF140
#define SC_HOTKEY       0xF150
#define SC_DEFAULT      0xF160
#define SC_MONITORPOWER 0xF170
#define SC_CONTEXTHELP  0xF180
#define SC_SEPARATOR    0xF00F

#ifdef XDK_SUPPORT_WIDGET_NC
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
#endif /*XDK_SUPPORT_WIDGET*/

#ifdef _UNICODE
#define _tstrcmp	wcscmp
#define _tstrncmp	wcsncmp
#define _tsprintf	swprintf
#define _tstrcat	wcscat
#define _tstrncat	wcsncat
#define _tstrcpy	wcscpy
#define _tstrstr	wcsstr
#define _tstrncpy	wcsncpy
#define _tstrlen	wcslen
#define _tstrtol	wcstol
#define _tstrtof	wcstof
#define _tstrnull(s)		(s == NULL || s[0] == L'\0')
#define _tprintf    wprintf
#define _tsscanf	swscanf
#else
#define _tstrcmp	strcmp
#define _tstrncmp	strncmp
#define _tstricmp	strcmp
#define _tstrnicmp	strncmp
#define _tsprintf	sprintf
#define _tstrcat	stecat
#define _tstrncat	strncat
#define _tstrcpy	strcpy
#define _tstrstr	strstr
#define _tstrncpy	strncpy
#define _tstrlen	strlen
#define _tstrtol	atol
#define _tstrtof	atof
#define _tstrnull(s)		(s == NULL || s[0] == '\0')
#define _tprintf    printf
#define _tsscanf	sscanf
#endif

#ifndef min
#define min(x, y) ({                            \
         __typeof__(x) _min1 = (x);                  \
         __typeof__(y) _min2 = (y);                  \
         (void) (&_min1 == &_min2);              \
         _min1 < _min2 ? _min1 : _min2; })
#endif

#ifndef max
#define max(x, y) ({                            \
         __typeof__(x) _max1 = (x);                  \
         __typeof__(y) _max2 = (y);                  \
         (void) (&_max1 == &_max2);              \
         _max1 > _max2 ? _max1 : _max2; })
#endif

#endif //_XDK_MACOS_H