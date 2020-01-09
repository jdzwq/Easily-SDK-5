/*************************************************************
	EasySoft xdc v3.0

	(c) 2003-2006 EasySoft Corporation.  All Rights Reserved.

	@doc xdc.rtf: firelist control

	@module	firelist.c | firelist control implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2006.12
*************************************************************/

/************************************************************
*************************************************************/

#include "xdcdef.h"
#include "xdcutil.h"
#include "xdccanv.h"

typedef struct tagFireListDelta{
	HWND hFireWnd;
	HWND hEditorWnd;
	WNDPROC lpfEditorProc;
	int index;
}FireListDelta;

#define GETFIRELISTDELTA(hWnd) 	((FireListDelta*)GetWindowLong(hWnd,GWL_USERDATA))
#define SETFIRELISTDELTA(hWnd,ptd) SetWindowLong(hWnd,GWL_USERDATA,(LONG)ptd)

LRESULT CALLBACK FireListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void	LP_FireCommit(HWND hWnd);
void	LP_FireRollback(HWND hWnd);

int		LP_GetEditorText(HWND hWnd,wchar_t* buf,int max);
void	LP_SetEditorText(HWND hWnd,const wchar_t* text);
int		LP_EditorTextSize(HWND hWnd);

void	LP_SetEditorOptions(HWND hWnd,const wchar_t* token,const wchar_t* feed);
void	LP_ClearEditorOptions(HWND hWnd);

ATOM RegisterFireListClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)FireListProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);/*(COLOR_WINDOW+1);*/
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FIRELISTCLASS;

	return RegisterClass(&wcex);
}

LRESULT CALLBACK SubListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);

	switch(message)
	{
	case WM_KEYDOWN:
		if((int)wParam == 13 || (int)wParam == 9)
		{
			if(ptd)
			{
				LP_FireCommit(ptd->hFireWnd);
				return 0;
			}
		}else if((int)wParam == 27)
		{
			if(ptd)
			{
				LP_FireRollback(ptd->hFireWnd);
				return 0;
			}
		}
		break;
	}

	if(ptd && ptd->lpfEditorProc)
		return CallWindowProc(ptd->lpfEditorProc,ptd->hEditorWnd,message,wParam,lParam);
	else
		return DefWindowProc(hWnd,message,wParam,lParam);		
}

LRESULT CALLBACK FireListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	LPCREATESTRUCT lpcs;

	switch(message)
	{
	case WM_CREATE:
		lpcs = (LPCREATESTRUCT)lParam;
		ptd = (FireListDelta*)xmem_alloc(sizeof(FireListDelta));
		SETFIRELISTDELTA(hWnd,ptd);

		EnableWindow(hWnd,FALSE);
		ShowWindow(hWnd,SW_HIDE);

		ptd->index = CB_ERR;
		ptd->hFireWnd = hWnd;
		ptd->hEditorWnd = CreateWindowEx(0,UN("COMBOBOX"),NULL,lpcs->style | CBS_DROPDOWNLIST,lpcs->x,lpcs->y,lpcs->cx,lpcs->cy,lpcs->hwndParent,NULL,lpcs->hInstance,NULL);
		EnableWindow(ptd->hEditorWnd,FALSE);
		ptd->lpfEditorProc = (WNDPROC)GetWindowLong(ptd->hEditorWnd,GWL_WNDPROC);
		SetWindowLong(ptd->hEditorWnd,GWL_WNDPROC,(LONG)SubListProc);
		SETFIRELISTDELTA(ptd->hEditorWnd,ptd);

		EnableWindow(ptd->hEditorWnd,TRUE);
		BringWindowToTop(ptd->hEditorWnd);
		SetFocus(ptd->hEditorWnd);

		break;
	case WM_DESTROY:
		LP_ClearEditorOptions(hWnd);

		SetWindowLong(ptd->hEditorWnd,GWL_WNDPROC,(LONG)ptd->lpfEditorProc);
		DestroyWindow(ptd->hEditorWnd);

		xmem_free(ptd);
		
		SETFIRELISTDELTA(hWnd,0);
		break;
	case WM_SETFOCUS:
		SetFocus(ptd->hEditorWnd);
		return 0;
	case WM_FIRECOMMIT:
		LP_FireCommit(ptd->hFireWnd);
		return 0;
	case WM_FIREROLLBACK:
		LP_FireRollback(ptd->hFireWnd);
		return 0;
	case WM_SETOPTIONS:
		LP_SetEditorOptions(ptd->hFireWnd,(wchar_t*)lParam,(wchar_t*)wParam);
		return 0;
	case WM_GETTEXT:
		return LP_GetEditorText(hWnd,(wchar_t*)lParam,(int)wParam);
	case WM_SETTEXT:
		LP_SetEditorText(hWnd,(const wchar_t*)lParam);
		return 0;
	case WM_GETTEXTLENGTH:
		return LP_EditorTextSize(hWnd);
	case WM_SETFONT:
	case WM_GETFONT:
		return SendMessage(ptd->hEditorWnd,message,wParam,lParam);
	}

	return DefWindowProc(hWnd,message,wParam,lParam);	
}

void LP_FireCommit(HWND hWnd)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);	
	NMHDR_FIRE hdr;

	ptd->index = (int)SendMessage(ptd->hEditorWnd,CB_GETCURSEL,0,0);

	hdr.hwndFrom = hWnd;
	hdr.idFrom = GetWindowLong(hWnd,GWL_ID);
	hdr.code = NC_EDITORCOMMIT;
	//hdr.editor = edFireDate;
	SendMessage(GetParent(hWnd),WM_NOTIFY,(WPARAM)hdr.idFrom,(LPARAM)&hdr);
}

void LP_FireRollback(HWND hWnd)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	NMHDR_FIRE hdr;

	ptd->index = CB_ERR;

	hdr.hwndFrom = hWnd;
	hdr.idFrom = GetWindowLong(hWnd,GWL_ID);
	hdr.code = NC_EDITORROLLBACK;
	//hdr.editor = edFireDate;
	SendMessage(GetParent(hWnd),WM_NOTIFY,(WPARAM)hdr.idFrom,(LPARAM)&hdr);
}

int	LP_GetEditorText(HWND hWnd,wchar_t* buf,int max)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);

	ptd->index = (int)SendMessage(ptd->hEditorWnd,CB_GETCURSEL,0,0);
	if(ptd->index == CB_ERR)
		return 0;

	wchar_t* data = (wchar_t*)SendMessage((HWND)ptd->hEditorWnd,CB_GETITEMDATA,(WPARAM)ptd->index,0);
	int len = xslen(data);
	len = (len < max)? len : max;

	xsncpy(buf,data,len);
	return len;
}

void LP_SetEditorText(HWND hWnd,const wchar_t* text)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);
	
	int count = (int)SendMessage((HWND)ptd->hEditorWnd,CB_GETCOUNT,0,0);
	wchar_t* data;
	for(int i=0;i<count;i++)
	{
		data = (wchar_t*)SendMessage(ptd->hEditorWnd,CB_GETITEMDATA,(WPARAM)i,0);
		if(xscmp(data,text) == 0)
		{
			ptd->index = i;
			SendMessage(ptd->hEditorWnd,CB_SETCURSEL,(WPARAM)ptd->index,0);
			break;
		}
	}
}

int	LP_EditorTextSize(HWND hWnd)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);

	ptd->index = (int)SendMessage(ptd->hEditorWnd,CB_GETCURSEL,0,0);
	if(ptd->index == CB_ERR)
		return 0;

	wchar_t* data = (wchar_t*)SendMessage((HWND)ptd->hEditorWnd,CB_GETITEMDATA,(WPARAM)ptd->index,0);
	return xslen(data);
}

void LP_SetEditorOptions(HWND hWnd,const wchar_t* options,wchar_t* feed)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);

	wchar_t *key,*val;
	int klen,vlen;
	wchar_t* token;
	wchar_t *data,*label;
	int index;

	token = (wchar_t*)options;
	while(token = (wchar_t*)parse_options(token,-1,feed[0],feed[1],&key,&klen,&val,&vlen))
	{
		data = (wchar_t*)xmem_alloc((klen + 1) * sizeof(wchar_t));
		xsncpy(data,key,klen);

		label = (wchar_t*)xmem_alloc((vlen + 1) * sizeof(wchar_t));
		xsncpy(label,val,vlen);

		index = (int)SendMessage((HWND)ptd->hEditorWnd,CB_ADDSTRING,0,(LPARAM)label);
		SendMessage((HWND)ptd->hEditorWnd,CB_SETITEMDATA,(WPARAM)index,(LPARAM)data);

		xmem_free(label);
	}

}

void LP_ClearEditorOptions(HWND hWnd)
{
	FireListDelta* ptd = GETFIRELISTDELTA(hWnd);

	int count = (int)SendMessage((HWND)ptd->hEditorWnd,CB_GETCOUNT,0,0);
	wchar_t* data;
	for(int i=0;i<count;i++)
	{
		data = (wchar_t*)SendMessage((HWND)ptd->hEditorWnd,CB_GETITEMDATA,(WPARAM)i,0);
		xmem_free(data);
	}
}

