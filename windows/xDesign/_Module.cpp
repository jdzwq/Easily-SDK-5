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

	res_ctx_t ctx = create_display_context(NULL);
	
	g_bmpThumb = load_bitmap_from_thumb(ctx, path);

	destroy_context(ctx);

	//create image list
	g_imagelist = create_images_doc();

	/*xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath,GDI_ATTR_GIZMO_NEW);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_NEW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_OPEN);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_OPEN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_CLOSE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_CLOSE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SAVE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SAVE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SAVEAS);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SAVEAS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SCHEMA);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SCHEMA, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PRINT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PRINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PRINT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PRINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PREVIEW);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PREVIEW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SELECTALL);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SELECTALL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_DELETE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_DELETE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_COPY);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_COPY, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_CUT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_CUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_UNDO);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_UNDO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PASTE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PASTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_EDIT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_EDIT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PROPER);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PROPER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_FONTNAME);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_FONTNAME, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_FONTSIZE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_FONTSIZE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_FONTCOLOR);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_FONTCOLOR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_FONTWEIGHT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_FONTWEIGHT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_ALIGNNEAR);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_ALIGNNEAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_ALIGNCENTER);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_ALIGNCENTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_ALIGNFAR);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_ALIGNFAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_ARRANGELEFT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_ARRANGELEFT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_ARRANGERIGHT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_ARRANGERIGHT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNTOP);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNTOP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNBOTTOM);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNBOTTOM, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_ARRANGECENTER);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_ARRANGECENTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SIZEHORZ);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SIZEHORZ, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SIZEVERT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SIZEVERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEALL);
	insert_images_item_from_file(g_imagelist, BMP_SIZEALL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SPACEHORZ);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SPACEHORZ, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SPACEVERT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SPACEVERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PRINT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PRINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_EXECUTE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_EXECUTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_STOP);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_STOP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_UP);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_UP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_DOWN);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_DOWN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_KEYBOX);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_KEYBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SUM);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SUM, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_STYLE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_STYLE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_GROUP);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_GROUP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_NOTE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_NOTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_ORDER);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_ORDER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SHAPE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SHAPE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_BORDER);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_BORDER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_LABEL);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_LABEL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SINGLETEXT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SINGLETEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_FRESH);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_FRESH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_CHECK);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_CHECK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_MEMO);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_MEMO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_GRID);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_GRID, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_GRAPH);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_GRAPH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PHOTO);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PHOTO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PHOTO);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PHOTO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_BOOK);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_BOOK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_INSERT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_INSERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PLUS);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PLUS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_MINUS);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_MINUS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_REMOVE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_REMOVE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_IMPORT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_IMPORT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_INPUT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_INPUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_OUTPUT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_OUTPUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SQL);
	insert_images_item_from_file(g_imagelist, BMP_SQL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_FETCH);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_FETCH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_UPDATE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_UPDATE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_HELPC);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_HELPC, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_HELPP);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_HELPP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_LOGO);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_LOGO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_FIRST);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_FIRST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_NEXT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_NEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PREV);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PREV, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_LAST);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_LAST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_HERF);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_HERF, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_RICH);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_RICH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_STATIC);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_STATIC, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_EDIT);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_EDIT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_LIST);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_LIST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_NAVI);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_NAVI, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SPIN);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SPIN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SLIDE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SLIDE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_RADIO);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_RADIO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_CHECKBOX);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_CHECKBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_DATE);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_DATE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_TIME);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_TIME, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PUSH);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PUSH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_GROUPBOX);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_GROUPBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_USER);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_USER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_DIALOG);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_DIALOG, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_HERF);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_HERF, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PANORAMA);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PANORAMA, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_LOCATION);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_LOCATION, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_TRENDS);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_TRENDS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_PANTO);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_PANTO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_SCATTER);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_SCATTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_DENSITY);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_DENSITY, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, GDI_ATTR_GIZMO_COUNTER);
	insert_images_item_from_file(g_imagelist, GDI_ATTR_GIZMO_COUNTER, NULL, path);*/
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
