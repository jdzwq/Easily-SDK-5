/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdl error function

	@module	xerr.c | xdl error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlwidget.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlctx.h"

LRESULT CALLBACK WidgetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	widget_t* ptd = NULL;
	int rt = 0;
	event_t ev = {0};

	ptd = (widget_t*)GetWindowLongPtr(hWnd,GWLP_USERDATA);

	ev.wnd = (handle_t)hWnd;
	ev.msg = message;
	ev.wp = (unsigned long)wParam;
	ev.lp = (void*)lParam;

	switch(message)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		break;
	case WM_KEYDOWN:
		if(ptd && ptd->pf_on_keydown)
			rt = (*ptd->pf_on_keydown)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_KEYUP:
		if(ptd && ptd->pf_on_keyup)
			rt = (*ptd->pf_on_keyup)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_CHAR:
		if(ptd && ptd->pf_on_char)
			rt = (*ptd->pf_on_char)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_LBUTTONDOWN:
		if(ptd && ptd->pf_on_left_button_down)
			rt = (*ptd->pf_on_left_button_down)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_LBUTTONUP:
		if(ptd && ptd->pf_on_left_button_up)
			rt = (*ptd->pf_on_left_button_up)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_LBUTTONDBLCLK:
		if(ptd && ptd->pf_on_left_button_click)
			rt = (*ptd->pf_on_left_button_click)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_RBUTTONDOWN:
		if(ptd && ptd->pf_on_right_button_down)
			rt = (*ptd->pf_on_right_button_down)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_RBUTTONUP:
		if(ptd && ptd->pf_on_right_button_up)
			rt = (*ptd->pf_on_right_button_up)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_RBUTTONDBLCLK:
		if(ptd && ptd->pf_on_right_button_click)
			rt = (*ptd->pf_on_right_button_click)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_MOUSEMOVE:
		if(ptd && ptd->pf_on_mouse_move)
			rt = (*ptd->pf_on_mouse_move)(ptd,&ev);

		if(rt)
			return rt;
		break;
	/*case WM_MOUSEWHEEL:
		if(ptd && ptd->pf_on_mouse_wheel)
			rt = (*ptd->pf_on_mouse_wheel)(ptd,&ev);

		if(rt)
			return rt;
		break;*/
	case WM_MOUSEHOVER:
		if(ptd && ptd->pf_on_mouse_hover)
			rt = (*ptd->pf_on_mouse_hover)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_MOUSELEAVE:
		if(ptd && ptd->pf_on_mouse_leave)
			rt = (*ptd->pf_on_mouse_leave)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_NCPAINT:
		if(ptd && ptd->pf_on_paint_window)
			rt = (*ptd->pf_on_paint_window)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_ERASEBKGND:
		if(ptd && ptd->pf_on_erase_client)
			rt = (*ptd->pf_on_erase_client)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_PAINT:
		if(ptd && ptd->pf_on_paint_client)
			rt = (*ptd->pf_on_paint_client)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_COMMAND:
		if(ptd && ptd->pf_on_command)
			rt = (*ptd->pf_on_command)(ptd,&ev);

		if(rt)
			return rt;
		break;
	case WM_NOTIFY:
		if(ptd && ptd->pf_on_notify)
			rt = (*ptd->pf_on_notify)(ptd,&ev);

		if(rt)
			return rt;
		break;
	}

	if(ptd && ptd->proc)
		return  CallWindowProc((WNDPROC)ptd->proc,(HWND)ptd->self,message,wParam,lParam);
	else
		return DefWindowProc(hWnd,message,wParam,lParam);	
}

ATOM RegisterWidgetClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
	wcex.lpfnWndProc	= (WNDPROC)WidgetProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WIDGETCLASS;

	return RegisterClass(&wcex);
}

bool_t widget_init(void)
{
	INITCOMMONCONTROLSEX sex;

	HINSTANCE hInst = GetModuleHandle(NULL);

	sex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	sex.dwICC = ICC_DATE_CLASSES ;
	InitCommonControlsEx(&sex);

	RegisterWidgetClass(hInst);

	return 1;
}

void widget_uninit(void)
{
	HINSTANCE hInst = GetModuleHandle(NULL);

	UnregisterClass(WIDGETCLASS,hInst);
}

widget_t* widget_alloc(const wchar_t* cname,const wchar_t* wname,unsigned long id,int type,int x,int y,int width,int height,handle_t owner)
{
	widget_t* pwd = (widget_t*)xmem_alloc(sizeof(widget_t));

	pwd->self = NULL;
	pwd->owner = owner;
	pwd->x = x;
	pwd->y = y;
	pwd->width = width;
	pwd->height = height;
	pwd->type = type;
	pwd->id = id;

	if(!w_is_null(cname))
	{
		pwd->cname = (wchar_t*)xmem_alloc(w_xslen(cname) * sizeof(wchar_t));
		w_xscpy(pwd->cname,cname);
	}

	if(!w_is_null(wname))
	{
		pwd->wname = (wchar_t*)xmem_alloc(w_xslen(wname) * sizeof(wchar_t));
		w_xscpy(pwd->wname,wname);
	}

	return pwd;
}

void widget_free(widget_t* pwd)
{
	assert(pwd);
	assert(pwd->self == NULL);

	if(pwd->cname)
		xmem_free(pwd->cname);
	if(pwd->wname)
		xmem_free(pwd->wname);

	xmem_free(pwd);
}

bool_t widget_create(widget_t* pwd)
{
	event_t ev;

	assert(pwd);
	assert(pwd->self == NULL);

	if(pwd->type == WIDGET_CHILD)
		pwd->style |= (WS_CHILD | WS_VISIBLE);
	else if(pwd->type == WIDGET_POPUP)
		pwd->style |= (WS_POPUP | WS_VISIBLE);

	pwd->self = (handle_t)CreateWindow(pwd->cname,NULL,pwd->style,pwd->x,pwd->y,pwd->width,pwd->height,(HWND)pwd->owner,NULL,NULL,(LPVOID)pwd);
		
	if(!IsWindow((HWND)pwd->self))
	{
		return 0;
	}

	if(pwd->pf_on_create)
	{
		ev.wnd = pwd->self;
		ev.msg = WM_CREATE;
		ev.wp = 0;
		ev.lp = (void*)pwd;

		(*pwd->pf_on_create)(pwd,&ev);
	}

	SetWindowLongPtr((HWND)pwd->self,GWLP_USERDATA,(LONG_PTR)pwd);
	SetWindowLongPtr((HWND)pwd->self,GWL_ID,(LONG_PTR)pwd->id);	
	pwd->proc = (void*)GetWindowLongPtr((HWND)pwd->self,GWL_WNDPROC);
	SetWindowLongPtr((HWND)pwd->self,GWL_WNDPROC,(LONG_PTR)WidgetProc);

	return 1;
}

void widget_destroy(widget_t* pwd)
{
	event_t ev;

	assert(pwd);

	if(!IsWindow((HWND)pwd->self))
		return;

	if(pwd->pf_on_destroy)
	{
		ev.wnd = pwd->self;
		ev.msg = WM_DESTROY;
		ev.wp = 0;
		ev.lp = 0;

		(*pwd->pf_on_destroy)(pwd,&ev);
	}
	
	DestroyWindow((HWND)pwd->self);

	pwd->self = NULL;
}

void widget_set_focus(widget_t* pwd)
{
	assert(pwd);

	SetFocus((HWND)pwd->self);
}