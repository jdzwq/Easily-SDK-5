// xdc_userctrl_test.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "xdc_userctrl_test.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#define IDC_USERCTRL		3000
res_win_t w_user = NULL;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_XDC_USERCTRL_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	xdl_process_init(XDL_APARTMENT_PROCESS | XDL_INITIALIZE_DESKTOP);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_XDC_USERCTRL_TEST));

	// ����Ϣѭ��: 
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



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_XDC_USERCTRL_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_XDC_USERCTRL_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

VOID userctrl_OnCalc(res_win_t win, PAGE_CALC* ppc)
{
	ppc->total_height = 4096;
	ppc->total_width = 800;
	ppc->line_height = 10;
	ppc->line_width = 10;
}

VOID userctrl_OnDraw(res_win_t win, HDC hDC)
{
	TextOut(hDC, 0, 0, _T("Hello World!"), -1);
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	xrect_t xr = { 0 };
	LPNOTICE lpn;

	switch (message)
	{
	case WM_CREATE:
		w_user = userctrl_create(_T("ser"), WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, hWnd);
		widget_set_user_id(w_user, IDC_USERCTRL);
		widget_set_owner(w_user, hWnd);
		BringWindowToTop(w_user);
		widget_show(w_user, WD_SHOW_NORMAL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_NOTICE:
		lpn = (NOTICE*)lParam;
		if (lpn->id == IDC_USERCTRL)
		{
			NOTICE_USER* lpu = (NOTICE_USER*)lpn;

			switch (lpu->code)
			{
			case NC_USERCALC:
				userctrl_OnCalc(lpu->widget, (PAGE_CALC*)lpu->data);
				break;
			case NC_USERDRAW:
				userctrl_OnDraw(lpu->widget, (HDC)lpu->data);
				break;
			}
		}
		break;
	case WM_SIZE:
		if (widget_is_valid(w_user))
		{
			GetClientRect(hWnd, &rt);
			xr.x = rt.left;
			xr.y = rt.right;
			xr.w = rt.right - rt.left;
			xr.h = rt.bottom - rt.top;

			widget_move(w_user, RECTPOINT(&xr));
			widget_size(w_user, RECTSIZE(&xr));
			widget_update(w_user);
			widget_redraw(w_user, NULL, 0);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		if (widget_is_valid(w_user))
		{
			widget_destroy(w_user);
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
