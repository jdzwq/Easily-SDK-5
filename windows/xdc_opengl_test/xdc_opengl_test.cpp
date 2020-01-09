// xdc_opengl_test.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "xdc_opengl_test.h"

#define MAX_LOADSTRING 100

void glInit(HWND hWnd);
void glPaint(HWND hWnd, HDC hDC);

PIXELFORMATDESCRIPTOR pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),  // size
	1,                          // version
	PFD_SUPPORT_OPENGL |        // OpenGL window
	PFD_DRAW_TO_WINDOW |        // render to window
	PFD_DOUBLEBUFFER,           // support double-buffering
	PFD_TYPE_RGBA,              // color type
	32,                         // prefered color depth
	0, 0, 0, 0, 0, 0,           // color bits (ignored)
	0,                          // no alpha buffer
	0,                          // alpha bits (ignored)
	0,                          // no accumulation buffer
	0, 0, 0, 0,                 // accum bits (ignored)
	16,                         // depth buffer
	0,                          // no stencil buffer
	0,                          // no auxiliary buffers
	PFD_MAIN_PLANE,             // main layer
	0,                          // reserved
	0, 0, 0,                    // no layer, visible, damage masks
};


// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	LoadString(hInstance, IDC_XDC_OPENGL_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_XDC_OPENGL_TEST));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_XDC_OPENGL_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_XDC_OPENGL_TEST);
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
   HWND hWnd = NULL;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   glInit(hWnd);

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

	switch (message)
	{
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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		glPaint(hWnd, ps.hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
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

void glInit(HWND hWnd)
{
	if (AllocConsole())
	{
		const GLubyte* Devise = glGetString(GL_RENDERER);    //����һ����Ⱦ����ʶ����ͨ���Ǹ�Ӳ��ƽ̨  
		const GLubyte* str = glGetString(GL_VERSION);
		_tprintf(_T("OpenGLʵ�ֵİ汾�ţ�%s\n"), str);
		_tprintf(_T("Ӳ��ƽ̨��%s\n"), Devise);
	}
}

void glPaint(HWND hWnd, HDC hDC)
{
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);

	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	/*GLuint nTexture = 0;
	glGenTextures(1, &nTexture);

	glBindTexture(GL_TEXTURE_2D, nTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();*/
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluOrtho2D(0, 300, 400, 0);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);

	//glEnable(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 200, 0, GL_RGB, GL_UNSIGNED_BYTE, "HELLO WORD");

	//glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	//glViewport(0, 0, 1024, 8096);
	glRectf(0.0f, 50.0f, 100.0f, 150.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(100.0f, 50.0f, 200.0f, 150.0f);
	glFlush();
	//glEnd();

	//glDisable(GL_TEXTURE_2D);

	wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);

	wglMakeCurrent(hDC, NULL);

	wglDeleteContext(hRC);
}