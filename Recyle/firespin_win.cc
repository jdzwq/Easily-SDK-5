/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	firespin.cc | date control implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

/************************************************************
*************************************************************/

#include "windows\xdcdef.h"
#include "windows\xdcutil.h"
 
typedef struct tagFireSpinDelta{
	HWND hOwner;
	HWND hEdit;
	HFONT hFont;
}FireSpinDelta;

#define GETFIRESPINDELTA(hWnd) 	((FireSpinDelta*)GetWindowLongPtr(hWnd,GWLP_USERDATA))
#define SETFIRESPINDELTA(hWnd,ptd) SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)ptd)

LRESULT CALLBACK FireSpinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int FireSpin_GetEditorText(HWND hWnd,wchar_t* buf,int max);
void FireSpin_SetEditorText(HWND hWnd,const wchar_t* text);
int FireSpin_EditorTextSize(HWND hWnd);

ATOM RegisterFireSpinClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireSpinProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);/*(COLOR_WINDOW+1);*/
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIRESPINCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK SubSpinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfOrgProc = (WNDPROC)GetWindowLongPtr(hWnd,GWLP_USERDATA);

	switch(message)
	{
	case WM_KEYDOWN:
		if(SendMessage(GetParent(hWnd),WM_KEYPREVIEW,wParam,lParam))
			return 0;
		break;
	default:
		break;
	}

	if(lpfOrgProc)
		return CallWindowProc(lpfOrgProc,hWnd,message,wParam,lParam);
	else
		return DefWindowProc(hWnd,message,wParam,lParam);			
}

LRESULT CALLBACK FireSpinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireSpinDelta* ptd = GETFIRESPINDELTA(hWnd);
	LPCREATESTRUCT lpcs;
	WNDPROC lpfOrgProc;
	LPNMHDR lphdr;
	RECT rt;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireSpinDelta*)xmem_alloc(sizeof(FireSpinDelta));
		memset((void*)ptd,0,sizeof(FireSpinDelta));

		GetClientRect(hWnd,&rt);

		ptd->hEdit = CreateWindowEx(0,UPDOWN_CLASS,NULL,lpcs->style  | UDS_ALIGNRIGHT | UDS_AUTOBUDDY | UDS_SETBUDDYINT | WS_DISABLED,lpcs->x,lpcs->y,lpcs->cx,lpcs->cy,hWnd,NULL,lpcs->hInstance,NULL);
		lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC);
		SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)SubSpinProc);
		SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)lpfOrgProc);

		EnableWindow(ptd->hEdit,TRUE);
		SetFocus(ptd->hEdit);

		SETFIRESPINDELTA(hWnd,ptd);
		break;
	case WM_DESTROY:
		SETFIRESPINDELTA(hWnd,0);

		if(IsWindow(ptd->hEdit))
		{
			EnableWindow(ptd->hEdit,FALSE);
			lpfOrgProc = (WNDPROC)GetWindowLongPtr(ptd->hEdit,GWLP_USERDATA);
			SetWindowLongPtr(ptd->hEdit,GWLP_WNDPROC,(LONG_PTR)lpfOrgProc);
			SetWindowLongPtr(ptd->hEdit,GWLP_USERDATA,(LONG_PTR)NULL);
			DestroyWindow(ptd->hEdit);
		}

		if(ptd->hFont)
			DeleteObject(ptd->hFont);

		xmem_free(ptd);
		break;
	case WM_SETOWNER:
		if(ptd)
		{
			ptd->hOwner = (HWND)lParam;
		}
		return 0;
	case WM_GETOWNER:
		if(ptd)
			return (LRESULT)ptd->hOwner;
		else
			return 0;
	case WM_NOTIFY:
		if(ptd)
		{
			lphdr = (LPNMHDR)lParam;
			if(lphdr->hwndFrom == ptd->hEdit && lphdr->code == DTN_CLOSEUP)
			{
				if(IsWindow(ptd->hOwner))
					SendMessage(ptd->hOwner,WM_COMMAND,MAKEWPARAM(EC_COMMIT,0),(LPARAM)hWnd);
				else
					SetFocus(ptd->hEdit);
			}else if(lphdr->hwndFrom == ptd->hEdit && lphdr->code == NM_KILLFOCUS)
			{
				if(IsWindow(ptd->hOwner))
					SendMessage(ptd->hOwner,WM_COMMAND,MAKEWPARAM(EC_COMMIT,0),(LPARAM)hWnd);
				else
					SetFocus(ptd->hEdit);
			}
		}
		break;
	case WM_SETFIREFONT:
		if(ptd->hFont)
			DeleteObject(ptd->hFont);
		ptd->hFont = (HFONT)XdcCreateFont((const xfont_t*)lParam);
		SendMessage(ptd->hEdit,WM_SETFONT,(WPARAM)ptd->hFont,MAKELPARAM(1,0));
		return 0;
	case WM_RESETEDITOR:
		SendMessage(ptd->hOwner,WM_COMMAND,MAKEWPARAM(wParam,0),(LPARAM)hWnd);
		return 0;
	case WM_GETEDITORTEXT:
		return FireSpin_GetEditorText(hWnd,(wchar_t*)lParam,(int)wParam);
	case WM_SETEDITORTEXT:
		FireSpin_SetEditorText(hWnd,(const wchar_t*)lParam);
		return 0;
	case WM_EDITORTEXTSIZE:
		return FireSpin_EditorTextSize(hWnd);
	case WM_KEYPREVIEW:
		if(wParam == 13 || wParam == 9) /*enter key*/
		{
			SendMessage(ptd->hOwner,WM_COMMAND,MAKEWPARAM(EC_COMMIT,0),(LPARAM)hWnd);
			return 1;
		}
		else if(wParam == 27) /*escape key*/
		{
			SendMessage(ptd->hOwner,WM_COMMAND,MAKEWPARAM(EC_ROLLBACK,0),(LPARAM)hWnd);
			return 1;
		}
		return 0;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);	
}

int FireSpin_GetEditorText(HWND hWnd,wchar_t* buf,int max)
{
	FireSpinDelta* ptd = GETFIRESPINDELTA(hWnd);
	
	return GetWindowText(ptd->hEdit,buf,max + 1);
}

void FireSpin_SetEditorText(HWND hWnd,const wchar_t* text)
{
	FireSpinDelta* ptd = GETFIRESPINDELTA(hWnd);
	
	SetWindowText(ptd->hEdit,(LPCTSTR)text);

	SendMessage(ptd->hEdit,EM_SETSEL,0,(LPARAM)-1);
}

int FireSpin_EditorTextSize(HWND hWnd)
{
	FireSpinDelta* ptd = GETFIRESPINDELTA(hWnd);
	
	return GetWindowTextLength(ptd->hEdit);
}
