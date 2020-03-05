/***********************************************************************
	Easily xDesign v3.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xDesign document

	@module	xDesign implement file

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

#include "stdafx.h"
#include "_Module.h"
#include "_Define.h"
#include "_Frame.h"

// 全局变量: 
HINSTANCE	g_hInst = NULL;		// 当前实例
HWND		g_hMain = NULL;

tchar_t		g_szRunPath[PATH_LEN] = { 0 };

LINKPTR		g_imagelist = NULL;
res_bmp_t	g_bmpThumb = NULL;

int			g_indFace = 0;

// 此代码模块中包含的函数的前向声明: 
BOOL	InitInstance(HINSTANCE, int);
VOID	UnInitInstance(HINSTANCE);
VOID	LoadResource(VOID);
VOID	FreeResource(VOID);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	
	HWND hClient;
	HACCEL hClientAccel, hMainAccel;
	MSG msg;

	hMainAccel = widget_get_accel(g_hMain);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_KEYDOWN)
		{
			hClient = MainFrame_GetActivePanel(g_hMain);
			if (hClient)
			{
				hClientAccel = widget_get_accel(hClient);
				if (hClientAccel)
				{
					if (TranslateAccelerator(hClient, hClientAccel, &msg))
						continue;
				}
			}

			if (hMainAccel)
			{
				if (TranslateAccelerator(g_hMain, hMainAccel, &msg))
					continue;
			}
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnInitInstance(hInstance);

	return 0;
}

INT ShowMsg(dword_t mid, const tchar_t* text)
{
	res_win_t hMsg = msgdlg_create(text, mid, g_hMain);

	widget_show(hMsg, WS_SHOW_NORMAL);
	widget_set_focus(hMsg);

	return widget_do_modal(hMsg);
}

INT DoInput(const tchar_t* title, tchar_t* buf, int max)
{
	xrect_t xr = { 0 };

	res_win_t hInput = inputdlg_create(title, buf, max, g_hMain);

	widget_show(hInput, WS_SHOW_NORMAL);

	return (INT)widget_do_modal(hInput);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; 

   tchar_t szPath[PATH_LEN] = { 0 };
   GetModuleFileName(NULL, szPath, PATH_LEN);

   split_path(szPath, g_szRunPath, NULL, NULL);

   xdl_process_init(XDL_APARTMENT_PROCESS);

   xdc_process_init();

   LoadResource();

   tchar_t szFace[INT_LEN] = { 0 };

   LoadPreference(_T("Default"), PERFERENCE_FACE, szFace);

   g_indFace = xstol(szFace);

   g_hMain = MainFrame_Create(_T("MainFrame"));

   return TRUE;
}

VOID UnInitInstance(HINSTANCE hInstance)
{
	FreeResource();

	tchar_t szFace[INT_LEN] = { 0 };
	ltoxs(g_indFace, szFace, INT_LEN);

	SavePreference(_T("Default"), PERFERENCE_FACE, szFace);

	xdc_process_uninit();

	xdl_process_uninit();
}

VOID LoadResource()
{
	tchar_t path[PATH_LEN];

	GetModuleFileName(NULL, path, PATH_LEN);

	HDC hDC = GetDC(NULL);
	
	g_bmpThumb = load_bitmap_from_thumb(hDC, path);

	ReleaseDC(NULL, hDC);

	//create image list
	g_imagelist = create_images_doc();

	/*xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath,GDI_ICON_NEW);
	insert_images_item_from_file(g_imagelist, GDI_ICON_NEW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_OPEN);
	insert_images_item_from_file(g_imagelist, GDI_ICON_OPEN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_CLOSE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_CLOSE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SAVE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SAVE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SAVEAS);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SAVEAS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SCHEMA);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SCHEMA, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PRINT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PRINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PRINT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PRINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PREVIEW);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PREVIEW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SELECTALL);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SELECTALL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_DELETE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_DELETE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_COPY);
	insert_images_item_from_file(g_imagelist, GDI_ICON_COPY, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_CUT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_CUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_UNDO);
	insert_images_item_from_file(g_imagelist, GDI_ICON_UNDO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PASTE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PASTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_EDIT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_EDIT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PROPER);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PROPER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_FONTNAME);
	insert_images_item_from_file(g_imagelist, GDI_ICON_FONTNAME, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_FONTSIZE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_FONTSIZE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_FONTCOLOR);
	insert_images_item_from_file(g_imagelist, GDI_ICON_FONTCOLOR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_FONTWEIGHT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_FONTWEIGHT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_ALIGNNEAR);
	insert_images_item_from_file(g_imagelist, GDI_ICON_ALIGNNEAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_ALIGNCENTER);
	insert_images_item_from_file(g_imagelist, GDI_ICON_ALIGNCENTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_ALIGNFAR);
	insert_images_item_from_file(g_imagelist, GDI_ICON_ALIGNFAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_ARRANGELEFT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_ARRANGELEFT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_ARRANGERIGHT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_ARRANGERIGHT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNTOP);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNTOP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNBOTTOM);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNBOTTOM, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_ARRANGECENTER);
	insert_images_item_from_file(g_imagelist, GDI_ICON_ARRANGECENTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SIZEHORZ);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SIZEHORZ, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SIZEVERT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SIZEVERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEALL);
	insert_images_item_from_file(g_imagelist, BMP_SIZEALL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SPACEHORZ);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SPACEHORZ, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SPACEVERT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SPACEVERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PRINT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PRINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_EXECUTE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_EXECUTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_STOP);
	insert_images_item_from_file(g_imagelist, GDI_ICON_STOP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_UP);
	insert_images_item_from_file(g_imagelist, GDI_ICON_UP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_DOWN);
	insert_images_item_from_file(g_imagelist, GDI_ICON_DOWN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_KEYBOX);
	insert_images_item_from_file(g_imagelist, GDI_ICON_KEYBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SUM);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SUM, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_STYLE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_STYLE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_GROUP);
	insert_images_item_from_file(g_imagelist, GDI_ICON_GROUP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_NOTE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_NOTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_ORDER);
	insert_images_item_from_file(g_imagelist, GDI_ICON_ORDER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SHAPE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SHAPE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_BORDER);
	insert_images_item_from_file(g_imagelist, GDI_ICON_BORDER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_LABEL);
	insert_images_item_from_file(g_imagelist, GDI_ICON_LABEL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SINGLETEXT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SINGLETEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_FRESH);
	insert_images_item_from_file(g_imagelist, GDI_ICON_FRESH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_CHECK);
	insert_images_item_from_file(g_imagelist, GDI_ICON_CHECK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_MEMO);
	insert_images_item_from_file(g_imagelist, GDI_ICON_MEMO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_GRID);
	insert_images_item_from_file(g_imagelist, GDI_ICON_GRID, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_GRAPH);
	insert_images_item_from_file(g_imagelist, GDI_ICON_GRAPH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PHOTO);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PHOTO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PHOTO);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PHOTO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_BOOK);
	insert_images_item_from_file(g_imagelist, GDI_ICON_BOOK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_INSERT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_INSERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PLUS);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PLUS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_MINUS);
	insert_images_item_from_file(g_imagelist, GDI_ICON_MINUS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_REMOVE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_REMOVE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_IMPORT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_IMPORT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_INPUT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_INPUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_OUTPUT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_OUTPUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SQL);
	insert_images_item_from_file(g_imagelist, BMP_SQL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_FETCH);
	insert_images_item_from_file(g_imagelist, GDI_ICON_FETCH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_UPDATE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_UPDATE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_HELPC);
	insert_images_item_from_file(g_imagelist, GDI_ICON_HELPC, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_HELPP);
	insert_images_item_from_file(g_imagelist, GDI_ICON_HELPP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_LOGO);
	insert_images_item_from_file(g_imagelist, GDI_ICON_LOGO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_FIRST);
	insert_images_item_from_file(g_imagelist, GDI_ICON_FIRST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_NEXT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_NEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PREV);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PREV, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_LAST);
	insert_images_item_from_file(g_imagelist, GDI_ICON_LAST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_HERF);
	insert_images_item_from_file(g_imagelist, GDI_ICON_HERF, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_RICH);
	insert_images_item_from_file(g_imagelist, GDI_ICON_RICH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_STATIC);
	insert_images_item_from_file(g_imagelist, GDI_ICON_STATIC, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_EDIT);
	insert_images_item_from_file(g_imagelist, GDI_ICON_EDIT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_LIST);
	insert_images_item_from_file(g_imagelist, GDI_ICON_LIST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_NAVI);
	insert_images_item_from_file(g_imagelist, GDI_ICON_NAVI, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SPIN);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SPIN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SLIDE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SLIDE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_RADIO);
	insert_images_item_from_file(g_imagelist, GDI_ICON_RADIO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_CHECKBOX);
	insert_images_item_from_file(g_imagelist, GDI_ICON_CHECKBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_DATE);
	insert_images_item_from_file(g_imagelist, GDI_ICON_DATE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_TIME);
	insert_images_item_from_file(g_imagelist, GDI_ICON_TIME, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PUSH);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PUSH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_GROUPBOX);
	insert_images_item_from_file(g_imagelist, GDI_ICON_GROUPBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_USER);
	insert_images_item_from_file(g_imagelist, GDI_ICON_USER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_DIALOG);
	insert_images_item_from_file(g_imagelist, GDI_ICON_DIALOG, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_HERF);
	insert_images_item_from_file(g_imagelist, GDI_ICON_HERF, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PANORAMA);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PANORAMA, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_LOCATION);
	insert_images_item_from_file(g_imagelist, GDI_ICON_LOCATION, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_TRENDS);
	insert_images_item_from_file(g_imagelist, GDI_ICON_TRENDS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_PANTO);
	insert_images_item_from_file(g_imagelist, GDI_ICON_PANTO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_SCATTER);
	insert_images_item_from_file(g_imagelist, GDI_ICON_SCATTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_DENSITY);
	insert_images_item_from_file(g_imagelist, GDI_ICON_DENSITY, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ICON_COUNTER);
	insert_images_item_from_file(g_imagelist, GDI_ICON_COUNTER, NULL, path);*/

	LINKPTR ptr_string = create_string_table(0);
	string_table_parse_options(ptr_string, GDI_ATTR_RGB_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	LINKPTR ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		insert_images_item_from_color(g_imagelist, get_string_entity_val_ptr(ent), get_string_entity_key_ptr(ent));
		
		ent = get_string_next_entity(ptr_string, ent);
	}
	destroy_string_table(ptr_string);
}

VOID FreeResource()
{
	destroy_images_doc(g_imagelist);

	if (g_bmpThumb)
		destroy_bitmap(g_bmpThumb);
}

VOID LoadPreference(const tchar_t* sec, const tchar_t* key, tchar_t* val)
{
	tchar_t sz_file[PATH_LEN] = { 0 };

	xsprintf(sz_file, _T("%s\\xProfile.ini"), g_szRunPath);

	read_profile(sz_file, sec, key, val, RES_LEN);
}

VOID SavePreference(const tchar_t* sec, const tchar_t* key, const tchar_t* val)
{
	tchar_t sz_file[PATH_LEN] = { 0 };

	xsprintf(sz_file, _T("%s\\xProfile.ini"), g_szRunPath);

	write_profile(sz_file, sec, key, val);
}
