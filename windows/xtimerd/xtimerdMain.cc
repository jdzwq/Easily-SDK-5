#include "resource.h"
#include "xtimerd.h"

#define UWM_NOTIFY			WM_USER + 2

HWND hMain = NULL;
HMENU hMenu = NULL;

// Global Variables:
HINSTANCE hInst;
NOTIFYICONDATA	nid = { 0 };

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
VOID				ExitInstance(HINSTANCE);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void ShowIcon()
{
	nid.cbSize = sizeof(nid);
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
	xscpy(nid.szTip, _T(""));
	nid.uCallbackMessage = UWM_NOTIFY;
	nid.uID = 11;
	nid.hWnd = hMain;
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void HideIcon()
{
	nid.cbSize = sizeof(nid);
	nid.uID = 11;
	nid.hWnd = hMain;
	Shell_NotifyIcon(NIM_DELETE, &nid);
	DeleteObject(nid.hIcon);
}

BOOL ConsoleHandler(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_C_EVENT)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL IdleInstance()
{

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	// Initialize global strings
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	BOOL b_idle = TRUE;
	BOOL b_run = TRUE;

	while (b_run)
	{
		while (b_idle && !PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			if (!IdleInstance())
				b_idle = FALSE;
		}

		do
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				b_run = FALSE;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message != WM_TIMER)
			{
				b_idle = TRUE;
			}

		} while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE));
	}

	ExitInstance(hInstance);

	return msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;//LoadIcon(hInstance, (LPCTSTR)IDI_LARGE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCTSTR)IDC_XTIMERD;
	wcex.lpszClassName = _T("XTIMERDCLASS");
	wcex.hIconSm = 0;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
#ifdef _DEBUG
	TCHAR szFile[PATH_LEN + 1] = { 0 };
#endif

	proc_info_t pi = { 0 };

	hInst = hInstance;

	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_SERVER);

	hWnd = CreateWindow(_T("XTIMERDCLASS"), NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}

	hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_XTIMERD));

	ShowWindow(hWnd, SW_HIDE);

	UpdateWindow(hWnd);

	hMain = hWnd;

	ShowIcon();

	PostMessage(hMain, WM_COMMAND, MAKEWPARAM(ID_OPERA_START, 1), 0);

	return TRUE;
}

void ExitInstance(HINSTANCE hInstance)
{
	HideIcon();

	DestroyMenu(hMenu);
	hMenu = NULL;

	hMain = NULL;

	xdl_process_uninit();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			if (MessageBox(hWnd, _T("will exist xTimer!"), _T("Notify"), MB_YESNO | MB_ICONQUESTION) != IDYES)
				return 0;
			DestroyWindow(hWnd);
			break;
		case ID_OPERA_START:
			xtimerd_start();
			break;
		case ID_OPERA_STOP:
			xtimerd_stop();
			break;
		case ID_OPERA_CLEAN:
			xdl_process_clean();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		return 0;
	case UWM_NOTIFY:
		if ((UINT)lParam == WM_LBUTTONDOWN)
		{
			POINT pt;
			GetCursorPos(&pt);
			HMENU hSub = GetSubMenu(hMenu, 0);
			TrackPopupMenu(hSub, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
		}
		break;
	case WM_DESTROY:
		xtimerd_stop();

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
