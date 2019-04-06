// xdc_frame_test.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "xdc_frame_test.h"

#define MAX_LOADSTRING 100

#define IDC_FORM	3000

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

res_win_t MainFrame_Create(const tchar_t* mname);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_XDC_FRAME_TEST, szWindowClass, MAX_LOADSTRING);

	xdl_process_init(XDL_APARTMENT_PROCESS | XDL_INITIALIZE_DESKTOP);

	MainFrame_Create(_T("Main"));

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_XDC_FRAME_TEST));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	xdl_process_uninit();

	return (int) msg.wParam;
}

#define MAINFRAME_TOOLBAR_HEIGHT	(float)25
#define MAINFRAME_TREEBAR_WIDTH		(float)50
#define MAINFRAME_TITLEBAR_HEIGHT	(float)8
#define MAINFRAME_STATUSBAR_HEIGHT	(float)8
#define MAINFRAME_CATEBAR_HEIGHT	(float)10

#define IDA_FILE_SAVE			2002
#define IDA_FILE_OPEN			2003

#define MAINFRAME_ACCEL_COUNT		2

accel_t	MAINFRAME_ACCEL[MAINFRAME_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('O'), IDA_FILE_OPEN,
	FVIRTKEY | FCONTROL, _T('S'), IDA_FILE_SAVE,
};

typedef struct tagMainFrameDelta{
	res_win_t hToolBar;
	res_win_t hTitleBar;
	res_win_t hResBar;
	res_win_t hDomBar;
	res_win_t hObjBar;
	res_win_t hCateBar;
	res_win_t hClientWnd;
	res_win_t hStatusBar;

	res_win_t hToolTip;
	res_win_t hKeyBox;
	res_win_t hNaviBox;

	bool_t bDirty;
	bool_t bMode;
}MainFrameDelta;

#define GETMAINFRAMEDELTA(widget) 			(MainFrameDelta*)widget_get_user_delta(widget)
#define SETMAINFRAMEDELTA(widget,ptd)		widget_set_user_delta(widget,(var_long)ptd)

int MainFrame_OnCreate(res_win_t widget, void* data)
{
	MainFrameDelta* pdt;

	widget_hand_create(widget);

	res_acl_t hac = create_accel_table(MAINFRAME_ACCEL, MAINFRAME_ACCEL_COUNT);

	widget_attach_accel(widget, hac);

	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_TOOLBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_TOP, 0, xs.cy);

	xs.fx = 0;
	xs.fy = MAINFRAME_STATUSBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_BOTTOM, 0, xs.cy);

	xs.fx = MAINFRAME_TREEBAR_WIDTH;
	xs.fy = 0;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WD_DOCK_LEFT | WD_DOCK_DYNA, xs.cx, 0);

	pdt = (MainFrameDelta*)xmem_alloc(sizeof(MainFrameDelta));
	SETMAINFRAMEDELTA(widget, pdt);

	xrect_t xr;
	widget_get_client_rect(widget, &xr);

	res_win_t hForm = formctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_OWNERNC | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);
	widget_set_user_id(hForm, IDC_FORM);
	widget_update_window(hForm);
	widget_update_client(hForm);
	widget_show(hForm, WD_SHOW_NORMAL);

	LINKPTR ptrForm = create_form_doc();
	formctrl_attach(hForm, ptrForm);

	widget_reset_paging(widget, xr.w, xr.h, 2048, 2048, 10, 10);

	return 0;
}

void MainFrame_OnDestroy(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hForm = widget_get_child(widget, IDC_FORM);
	if (widget_is_valid(hForm))
	{
		LINKPTR ptrForm = formctrl_detach(hForm);
		destroy_form_doc(ptrForm);

		widget_destroy(hForm);
	}

	res_acl_t hac = widget_get_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	xmem_free(pdt);

	widget_hand_destroy(widget);
}

int MainFrame_OnClose(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	widget_destroy(widget);

	PostQuitMessage(0);

	return 0;
}

void MainFrame_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr;
	widget_get_client_rect(widget, &xr);

	res_win_t hForm = widget_get_child(widget, IDC_FORM);
	if (widget_is_valid(hForm))
	{
		widget_move(hForm, RECTPOINT(&xr));
		widget_size(hForm, RECTSIZE(&xr));
	}
}

void MainFrame_OnScroll(res_win_t widget, bool_t bHorz, long nLine)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	widget_hand_scroll(widget, bHorz, nLine);
}

res_win_t MainFrame_Create(const tchar_t* mname)
{
	res_win_t widget;
	xrect_t xr = { 0 };

	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(MainFrame_OnCreate)
		EVENT_ON_DESTROY(MainFrame_OnDestroy)
		EVENT_ON_CLOSE(MainFrame_OnClose)

		EVENT_ON_SIZE(MainFrame_OnSize)
		EVENT_ON_SCROLL(MainFrame_OnScroll)

		EVENT_ON_NC_IMPLEMENT
		EVENT_ON_DOCKER_IMPLEMENT
		EVENT_ON_DOCKER_IMPLEMENT

	SUBPROC_END_DISPATH

	get_desktop_size(RECTSIZE(&xr));

	widget = widget_create(_T("TEST"), WD_STYLE_FRAME | WD_STYLE_DOCKER | WD_STYLE_MENUBAR | WD_STYLE_OWNERNC, &xr, NULL, &ev);

	if (!widget)
	{
		return 0;
	}

	widget_show(widget, WD_SHOW_NORMAL);
	widget_update_window(widget);
	widget_update_client(widget);

	return widget;
}


