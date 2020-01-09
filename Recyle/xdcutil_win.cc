/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@module	xdcutil.cc | xdc utility implement file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

/************************************************************
*************************************************************/

#include "windows\xdcdef.h"
#include "windows\xdcutil.h"

#define WS_EX_LAYERED	0x80000
#define LWA_ALPHA		0x2
#define LWA_COLORKEY	0x1

typedef BOOL (WINAPI *pf_SetLayeredWindowAttributes)(HWND hWnd,COLORREF clrKey,BYTE bAlpha,DWORD dwFlags);

UINT PROPERCLIPFORMAT = 0;
HINSTANCE USER32_LIB = NULL;

void XdcInitControl()
{
	INITCOMMONCONTROLSEX sex;

	HINSTANCE hInst = GetModuleHandle(NULL);

	sex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	sex.dwICC = ICC_DATE_CLASSES | ICC_UPDOWN_CLASS ;
	InitCommonControlsEx(&sex);

	RegisterFireEditClass(hInst);
	RegisterFireListClass(hInst);
	RegisterFireDropClass(hInst);
	RegisterFireMemoClass(hInst);
	RegisterFireDateClass(hInst);
	RegisterFireSpinClass(hInst);
	RegisterFireMenuClass(hInst);
	RegisterDropListClass(hInst);
	
	RegisterProperCtrlClass(hInst);
	RegisterTreeCtrlClass(hInst);
	RegisterToolCtrlClass(hInst);
	RegisterTitleCtrlClass(hInst);
	RegisterListCtrlClass(hInst);
	RegisterLabelCtrlClass(hInst);
	RegisterGridCtrlClass(hInst);
	RegisterFormCtrlClass(hInst);
	RegisterGraphCtrlClass(hInst);
	RegisterStringCtrlClass(hInst);
	RegisterSplitCtrlClass(hInst);
	RegisterStatusCtrlClass(hInst);
	
	/*RegisterFireGridClass(hInst);
	RegisterFilterGridClass(hInst);
	
	RegisterScreenCtrlClass(hInst);
	RegisterInputCtrlClass(hInst);

	RegisterFireReportClass(hInst);
	RegisterReportCtrlClass(hInst);*/

	USER32_LIB = LoadLibrary(L"user32.dll");

	initial_canvas();
}

void XdcUnInitControl()
{
	HINSTANCE hInst = GetModuleHandle(NULL);

	uninitial_canvas();

	UnregisterClass(FIREEDITCLASS,hInst);
	UnregisterClass(FIRELISTCLASS,hInst);
	UnregisterClass(FIREDATECLASS,hInst);
	UnregisterClass(FIRESPINCLASS,hInst);
	UnregisterClass(FIREMENUCLASS,hInst);
	UnregisterClass(DROPLISTCLASS,hInst);
	
	UnregisterClass(PROPERCTRLCLASS,hInst);
	UnregisterClass(TREECTRLCLASS,hInst);
	UnregisterClass(TOOLCTRLCLASS,hInst);
	UnregisterClass(TITLECTRLCLASS,hInst);
	UnregisterClass(LISTCTRLCLASS,hInst);
	UnregisterClass(LABELCTRLCLASS, hInst);
	UnregisterClass(GRIDCTRLCLASS,hInst);
	UnregisterClass(FORMCTRLCLASS,hInst);
	UnregisterClass(GRAPHCTRLCLASS,hInst);
	UnregisterClass(STRINGCTRLCLASS, hInst);
	UnregisterClass(SPLITCTRLCLASS, hInst);
	UnregisterClass(STATUSCTRLCLASS, hInst);

	/*UnregisterClass(FIREGRIDCLASS,hInst);	
	UnregisterClass(FILTERGRIDCLASS,hInst);
	
	UnregisterClass(PANELCTRLCLASS,hInst);
	
	UnregisterClass(SCREENCTRLCLASS,hInst);
	UnregisterClass(INPUTCTRLCLASS,hInst);
	UnregisterClass(EVENTCTRLCLASS,hInst);
	UnregisterClass(EVENTPOPUPCLASS,hInst);
	UnregisterClass(CUBECTRLCLASS,hInst);

	UnregisterClass(REPORTCTRLCLASS,hInst);
	UnregisterClass(FIREREPORTCLASS,hInst);*/

	if(USER32_LIB)
		FreeLibrary(USER32_LIB);

	initial_canvas();
}


HFONT XdcCreateFont(const xfont_t* pxf)
{
	LOGFONT lf;
	HFONT hf;
	HDC hDC;

	assert(pxf);

	hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	hDC = GetDC(NULL);

	GetObject(hf,sizeof(LOGFONT),(void*)&lf);
	lf.lfHeight = -MulDiv(xstos(pxf->size), GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = xstos(pxf->weight);

	ReleaseDC(NULL,hDC);

	if(xsicmp(pxf->style,GDI_ATTR_FONT_STYLE_ITALIC) == 0)
	{
		lf.lfItalic = (BYTE)TRUE;
	}else if(xsicmp(pxf->style,GDI_ATTR_FONT_STYLE_UNDERLINE) == 0)
	{
		lf.lfUnderline = (BYTE)TRUE;
	}else if(xsicmp(pxf->style,GDI_ATTR_FONT_STYLE_STRIKOUT) == 0)
	{
		lf.lfStrikeOut = (BYTE)TRUE;
	}
	xscpy(lf.lfFaceName,pxf->family);

	return CreateFontIndirect(&lf);
}

void XdcEnsureVisible(HWND hWnd,const RECT* prt)
{
	int nHorzPos,nVertPos;
	RECT rt;
	SCROLLINFO si;

	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	if(GetScrollInfo(hWnd,SB_HORZ,&si))
		nHorzPos = si.nPos;
	else
		nHorzPos = 0;

	memset((void*)&si,0,sizeof(si));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	if(GetScrollInfo(hWnd,SB_VERT,&si))
		nVertPos = si.nPos;
	else
		nVertPos = 0;

	GetClientRect(hWnd,&rt);
	
	if(prt->left < rt.left)
	{
		SendMessage(hWnd,WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,nHorzPos + prt->left - rt.left ),(LPARAM)0);
		SendMessage(hWnd,WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,0),(LPARAM)0);
	}else if(prt->right > rt.right)
	{
		SendMessage(hWnd,WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,nHorzPos + prt->right - rt.right),(LPARAM)0);
		SendMessage(hWnd,WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,0),(LPARAM)0);
	}
	
	if(prt->top < rt.top)
	{
		SendMessage(hWnd,WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,nVertPos + prt->top - rt.top),(LPARAM)0);
		SendMessage(hWnd,WM_VSCROLL,MAKEWPARAM(SB_ENDSCROLL,0),(LPARAM)0);
	}else if(prt->bottom > rt.bottom)
	{
		SendMessage(hWnd,WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,nVertPos + prt->bottom - rt.bottom),(LPARAM)0);
		SendMessage(hWnd,WM_VSCROLL,MAKEWPARAM(SB_ENDSCROLL,0),(LPARAM)0);
	}
}

void XdcMergeRect(RECT* prt,const RECT* pr1,const RECT* pr2)
{
	prt->left = (pr1->left < pr2->left)? pr1->left : pr2->left;
	prt->top = (pr1->top < pr2->top)? pr1->top : pr2->top;
	prt->right = (pr1->right > pr2->right)? pr1->right : pr2->right;
	prt->bottom = (pr1->bottom > pr2->bottom)? pr1->bottom : pr2->bottom;
}

void XdcCenterRect(RECT* prt,int cx,int cy)
{
	if(prt->right - prt->left > cx)
	{
		prt->left = (prt->right + prt->left - cx) / 2;
		prt->right = prt->left + cx;
	}

	if(prt->bottom - prt->top > cy)
	{
		prt->top = (prt->bottom + prt->top - cy) / 2;
		prt->bottom = prt->top + cy;
	}
}

void XdcTrackMenu(HWND hOwner, LINKPTR ptrMenu, POINT* lppt)
{
	HWND hMenu;
	RECT rt;

	rt.left = rt.right = lppt->x;
	rt.top = rt.bottom = lppt->y;

	hMenu = CreateWindow(FIREMENUCLASS, NULL, WS_POPUP, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, hOwner, NULL, NULL, NULL);
	SetWindowLong(hMenu, GWL_ID, (LONG)IDC_FIREDROP);
	SendMessage(hMenu, WM_SETOWNER, 0, (LPARAM)hOwner);
	SendMessage(hMenu, WM_ATTACHDATA, (WPARAM)0, (LPARAM)ptrMenu);
	BringWindowToTop(hMenu);
}