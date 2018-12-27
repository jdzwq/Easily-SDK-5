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

	widget_show(hMsg, WD_SHOW_NORMAL);
	widget_set_focus(hMsg);

	return widget_do_modal(hMsg);
}

INT DoInput(const tchar_t* title, tchar_t* buf, int max)
{
	xrect_t xr = { 0 };

	res_win_t hInput = inputdlg_create(title, buf, max, g_hMain);

	widget_show(hInput, WD_SHOW_NORMAL);

	return (INT)widget_do_modal(hInput);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; 

   tchar_t szPath[PATH_LEN] = { 0 };
   GetModuleFileName(NULL, szPath, PATH_LEN);

   split_path(szPath, g_szRunPath, NULL, NULL);

   xdl_process_init(XDL_APARTMENT_PROCESS | XDL_INITIALIZE_DESKTOP);

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

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath,BMP_NEW);
	insert_images_item_from_file(g_imagelist, BMP_NEW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_OPEN);
	insert_images_item_from_file(g_imagelist, BMP_OPEN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CLOSE);
	insert_images_item_from_file(g_imagelist, BMP_CLOSE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SAVE);
	insert_images_item_from_file(g_imagelist, BMP_SAVE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SAVEAS);
	insert_images_item_from_file(g_imagelist, BMP_SAVEAS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SCHEMA);
	insert_images_item_from_file(g_imagelist, BMP_SCHEMA, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PRINT);
	insert_images_item_from_file(g_imagelist, BMP_PRINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PRINTER);
	insert_images_item_from_file(g_imagelist, BMP_PRINTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PREVIEW);
	insert_images_item_from_file(g_imagelist, BMP_PREVIEW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SELECTALL);
	insert_images_item_from_file(g_imagelist, BMP_SELECTALL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DELETE);
	insert_images_item_from_file(g_imagelist, BMP_DELETE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_COPY);
	insert_images_item_from_file(g_imagelist, BMP_COPY, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CUT);
	insert_images_item_from_file(g_imagelist, BMP_CUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_UNDO);
	insert_images_item_from_file(g_imagelist, BMP_UNDO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PASTE);
	insert_images_item_from_file(g_imagelist, BMP_PASTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_EDIT);
	insert_images_item_from_file(g_imagelist, BMP_EDIT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PROPER);
	insert_images_item_from_file(g_imagelist, BMP_PROPER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FONTNAME);
	insert_images_item_from_file(g_imagelist, BMP_FONTNAME, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FONTSIZE);
	insert_images_item_from_file(g_imagelist, BMP_FONTSIZE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FONTCOLOR);
	insert_images_item_from_file(g_imagelist, BMP_FONTCOLOR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FONTBOLD);
	insert_images_item_from_file(g_imagelist, BMP_FONTBOLD, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXTNEAR);
	insert_images_item_from_file(g_imagelist, BMP_TEXTNEAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXTCENTER);
	insert_images_item_from_file(g_imagelist, BMP_TEXTCENTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXTFAR);
	insert_images_item_from_file(g_imagelist, BMP_TEXTFAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNLEFT);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNLEFT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNRIGHT);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNRIGHT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNTOP);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNTOP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNBOTTOM);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNBOTTOM, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNCENTER);
	insert_images_item_from_file(g_imagelist, BMP_ALIGNCENTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEWIDTH);
	insert_images_item_from_file(g_imagelist, BMP_SIZEWIDTH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEHEIGHT);
	insert_images_item_from_file(g_imagelist, BMP_SIZEHEIGHT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEALL);
	insert_images_item_from_file(g_imagelist, BMP_SIZEALL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEHORZ);
	insert_images_item_from_file(g_imagelist, BMP_SIZEHORZ, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEVERT);
	insert_images_item_from_file(g_imagelist, BMP_SIZEVERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PAINT);
	insert_images_item_from_file(g_imagelist, BMP_PAINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_EXEC);
	insert_images_item_from_file(g_imagelist, BMP_EXEC, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_STOP);
	insert_images_item_from_file(g_imagelist, BMP_STOP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_UP);
	insert_images_item_from_file(g_imagelist, BMP_UP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DOWN);
	insert_images_item_from_file(g_imagelist, BMP_DOWN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_KEYBORD);
	insert_images_item_from_file(g_imagelist, BMP_KEYBORD, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SUM);
	insert_images_item_from_file(g_imagelist, BMP_SUM, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DRAW);
	insert_images_item_from_file(g_imagelist, BMP_DRAW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_GROUP);
	insert_images_item_from_file(g_imagelist, BMP_GROUP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_MACRO);
	insert_images_item_from_file(g_imagelist, BMP_MACRO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SENDBACK);
	insert_images_item_from_file(g_imagelist, BMP_SENDBACK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SHAPE);
	insert_images_item_from_file(g_imagelist, BMP_SHAPE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_BORDER);
	insert_images_item_from_file(g_imagelist, BMP_BORDER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LABEL);
	insert_images_item_from_file(g_imagelist, BMP_LABEL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXT);
	insert_images_item_from_file(g_imagelist, BMP_TEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FRESH);
	insert_images_item_from_file(g_imagelist, BMP_FRESH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CHECK);
	insert_images_item_from_file(g_imagelist, BMP_CHECK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_MEMO);
	insert_images_item_from_file(g_imagelist, BMP_MEMO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_GRID);
	insert_images_item_from_file(g_imagelist, BMP_GRID, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_GRAPH);
	insert_images_item_from_file(g_imagelist, BMP_GRAPH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_IMAGE);
	insert_images_item_from_file(g_imagelist, BMP_IMAGE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PHOTO);
	insert_images_item_from_file(g_imagelist, BMP_PHOTO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_BOOK);
	insert_images_item_from_file(g_imagelist, BMP_BOOK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_INSERT);
	insert_images_item_from_file(g_imagelist, BMP_INSERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PLUS);
	insert_images_item_from_file(g_imagelist, BMP_PLUS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_MINUS);
	insert_images_item_from_file(g_imagelist, BMP_MINUS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ERASE);
	insert_images_item_from_file(g_imagelist, BMP_ERASE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_IMPORT);
	insert_images_item_from_file(g_imagelist, BMP_IMPORT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_INPUT);
	insert_images_item_from_file(g_imagelist, BMP_INPUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_OUTPUT);
	insert_images_item_from_file(g_imagelist, BMP_OUTPUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SQL);
	insert_images_item_from_file(g_imagelist, BMP_SQL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FETCH);
	insert_images_item_from_file(g_imagelist, BMP_FETCH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_COMMIT);
	insert_images_item_from_file(g_imagelist, BMP_COMMIT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_HELPC);
	insert_images_item_from_file(g_imagelist, BMP_HELPC, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_HELPH);
	insert_images_item_from_file(g_imagelist, BMP_HELPH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LOGIN);
	insert_images_item_from_file(g_imagelist, BMP_LOGIN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FIRST);
	insert_images_item_from_file(g_imagelist, BMP_FIRST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_NEXT);
	insert_images_item_from_file(g_imagelist, BMP_NEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PREV);
	insert_images_item_from_file(g_imagelist, BMP_PREV, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LAST);
	insert_images_item_from_file(g_imagelist, BMP_LAST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_HERF);
	insert_images_item_from_file(g_imagelist, BMP_HERF, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_RICH);
	insert_images_item_from_file(g_imagelist, BMP_RICH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_STATICBOX);
	insert_images_item_from_file(g_imagelist, BMP_STATICBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_EDITBOX);
	insert_images_item_from_file(g_imagelist, BMP_EDITBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LISTBOX);
	insert_images_item_from_file(g_imagelist, BMP_LISTBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_NAVIBOX);
	insert_images_item_from_file(g_imagelist, BMP_NAVIBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SPINBOX);
	insert_images_item_from_file(g_imagelist, BMP_SPINBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SLIDEBOX);
	insert_images_item_from_file(g_imagelist, BMP_SLIDEBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_RADIOBOX);
	insert_images_item_from_file(g_imagelist, BMP_RADIOBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CHECKBOX);
	insert_images_item_from_file(g_imagelist, BMP_CHECKBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DATEBOX);
	insert_images_item_from_file(g_imagelist, BMP_DATEBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TIMEBOX);
	insert_images_item_from_file(g_imagelist, BMP_TIMEBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PUSHBOX);
	insert_images_item_from_file(g_imagelist, BMP_PUSHBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_GROUPBOX);
	insert_images_item_from_file(g_imagelist, BMP_GROUPBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_USERBOX);
	insert_images_item_from_file(g_imagelist, BMP_USERBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DIALOG);
	insert_images_item_from_file(g_imagelist, BMP_DIALOG, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ADDRBOX);
	insert_images_item_from_file(g_imagelist, BMP_ADDRBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CHART);
	insert_images_item_from_file(g_imagelist, BMP_CHART, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_MAPCHART);
	insert_images_item_from_file(g_imagelist, BMP_MAPCHART, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LINECHART);
	insert_images_item_from_file(g_imagelist, BMP_LINECHART, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_BARCHART);
	insert_images_item_from_file(g_imagelist, BMP_BARCHART, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PIECHART);
	insert_images_item_from_file(g_imagelist, BMP_PIECHART, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DOTCHART);
	insert_images_item_from_file(g_imagelist, BMP_DOTCHART, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_COUNTER);
	insert_images_item_from_file(g_imagelist, BMP_COUNTER, NULL, path);

	LINKPTR ptr_string = create_string_table();
	string_table_parse_options(ptr_string, GDI_ATTR_RGB_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	LINKPTR ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		insert_images_item_from_color(g_imagelist, get_string_entity_val_ptr(ent), get_string_entity_key_ptr(ent));
		
		ent = get_string_next_entity(ptr_string, ent);
	}
	destroy_string_table(ptr_string);

	cache_images_doc(g_imagelist);
}

VOID FreeResource()
{
	uncache_images_doc(g_imagelist);
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
