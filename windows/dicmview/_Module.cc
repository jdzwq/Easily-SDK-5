/***********************************************************************
	Easily DICMView v1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc _Moudle document

	@module	_Moudle implement file

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

#include "_Module.h"
#include "_Define.h"
#include "_Frame.h"

// 全局变量: 
HINSTANCE	g_hInst = NULL;		// 当前实例
HWND		g_hMain = NULL;

tchar_t		g_szRunPath[PATH_LEN] = { 0 };

LINKPTR		g_ptrImages = NULL;
res_bmp_t	g_bmpThumb = NULL;

#ifdef _DEBUG
object_t	g_stmTrace = NULL;
#endif

int			g_indFace = 0;

// 此代码模块中包含的函数的前向声明: 
BOOL	InitInstance(HINSTANCE, int);
VOID	UnInitInstance(HINSTANCE);
VOID	LoadResource(VOID);
VOID	FreeResource(VOID);

static void STDCALL _error_track(void* param, const tchar_t* code, const tchar_t* text)
{
	object_t stm = (object_t)param;
	int len;
	dword_t dw;

	len = xslen(code);
	stream_write(stm, code, len, &dw);
	stream_write(stm, _T("\t"), 1, &dw);

	len = xslen(text);
	stream_write(stm, text, len, &dw);
	stream_write(stm, _T("\n"), 1, &dw);
}

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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; 

   tchar_t szPath[PATH_LEN] = { 0 };
   GetModuleFileName(NULL, szPath, PATH_LEN);

   split_path(szPath, g_szRunPath, NULL, NULL);

   xdl_process_init(XDL_APARTMENT_PROCESS | XDL_INITIALIZE_DESKTOP);

#ifdef _DEBUG
   xsprintf(szPath, _T("%s\\dicmiew-dump.txt"), g_szRunPath);
   XHANDLE xf = xfile_open(NULL, szPath, 1);
   if (xf)
   {
	   g_stmTrace = stream_alloc(xf);
	   stream_set_encode(g_stmTrace, _UTF8);
	   stream_write_utfbom(g_stmTrace, NULL);

	   trace_error((void*)g_stmTrace, _error_track);
   }
#endif

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

#ifdef _DEBUG
	if (g_stmTrace)
	{
		trace_error(NULL, NULL);

		XHANDLE xf = stream_fetch(g_stmTrace);
		xfile_close(xf);

		stream_free(g_stmTrace);
		g_stmTrace = NULL;
	}
#endif

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
	g_ptrImages = create_image_list();

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath,BMP_NEW);
	insert_image_item_from_file(g_ptrImages, BMP_NEW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_OPEN);
	insert_image_item_from_file(g_ptrImages, BMP_OPEN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CLOSE);
	insert_image_item_from_file(g_ptrImages, BMP_CLOSE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SAVE);
	insert_image_item_from_file(g_ptrImages, BMP_SAVE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SAVEAS);
	insert_image_item_from_file(g_ptrImages, BMP_SAVEAS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SCHEMA);
	insert_image_item_from_file(g_ptrImages, BMP_SCHEMA, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PRINT);
	insert_image_item_from_file(g_ptrImages, BMP_PRINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PRINTER);
	insert_image_item_from_file(g_ptrImages, BMP_PRINTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PREVIEW);
	insert_image_item_from_file(g_ptrImages, BMP_PREVIEW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FILE);
	insert_image_item_from_file(g_ptrImages, BMP_FILE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FIND);
	insert_image_item_from_file(g_ptrImages, BMP_FIND, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ICON);
	insert_image_item_from_file(g_ptrImages, BMP_ICON, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_EARTH);
	insert_image_item_from_file(g_ptrImages, BMP_EARTH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_RULER);
	insert_image_item_from_file(g_ptrImages, BMP_RULER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SEARCH);
	insert_image_item_from_file(g_ptrImages, BMP_SEARCH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ANCHOR);
	insert_image_item_from_file(g_ptrImages, BMP_ANCHOR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ANGLE);
	insert_image_item_from_file(g_ptrImages, BMP_ANGLE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALTER);
	insert_image_item_from_file(g_ptrImages, BMP_ALTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_STAR);
	insert_image_item_from_file(g_ptrImages, BMP_STAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_NOTE);
	insert_image_item_from_file(g_ptrImages, BMP_NOTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXT);
	insert_image_item_from_file(g_ptrImages, BMP_TEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_WINDOW);
	insert_image_item_from_file(g_ptrImages, BMP_WINDOW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ZOOMIN);
	insert_image_item_from_file(g_ptrImages, BMP_ZOOMIN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ZOOMOUT);
	insert_image_item_from_file(g_ptrImages, BMP_ZOOMOUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SELECTALL);
	insert_image_item_from_file(g_ptrImages, BMP_SELECTALL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DELETE);
	insert_image_item_from_file(g_ptrImages, BMP_DELETE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_COPY);
	insert_image_item_from_file(g_ptrImages, BMP_COPY, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CUT);
	insert_image_item_from_file(g_ptrImages, BMP_CUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_UNDO);
	insert_image_item_from_file(g_ptrImages, BMP_UNDO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PASTE);
	insert_image_item_from_file(g_ptrImages, BMP_PASTE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_EDIT);
	insert_image_item_from_file(g_ptrImages, BMP_EDIT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PROPER);
	insert_image_item_from_file(g_ptrImages, BMP_PROPER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FONTNAME);
	insert_image_item_from_file(g_ptrImages, BMP_FONTNAME, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FONTSIZE);
	insert_image_item_from_file(g_ptrImages, BMP_FONTSIZE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FONTCOLOR);
	insert_image_item_from_file(g_ptrImages, BMP_FONTCOLOR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FONTBOLD);
	insert_image_item_from_file(g_ptrImages, BMP_FONTBOLD, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXTNEAR);
	insert_image_item_from_file(g_ptrImages, BMP_TEXTNEAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXTCENTER);
	insert_image_item_from_file(g_ptrImages, BMP_TEXTCENTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXTFAR);
	insert_image_item_from_file(g_ptrImages, BMP_TEXTFAR, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNLEFT);
	insert_image_item_from_file(g_ptrImages, BMP_ALIGNLEFT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNRIGHT);
	insert_image_item_from_file(g_ptrImages, BMP_ALIGNRIGHT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNTOP);
	insert_image_item_from_file(g_ptrImages, BMP_ALIGNTOP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNBOTTOM);
	insert_image_item_from_file(g_ptrImages, BMP_ALIGNBOTTOM, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ALIGNCENTER);
	insert_image_item_from_file(g_ptrImages, BMP_ALIGNCENTER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEWIDTH);
	insert_image_item_from_file(g_ptrImages, BMP_SIZEWIDTH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEHEIGHT);
	insert_image_item_from_file(g_ptrImages, BMP_SIZEHEIGHT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEALL);
	insert_image_item_from_file(g_ptrImages, BMP_SIZEALL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEHORZ);
	insert_image_item_from_file(g_ptrImages, BMP_SIZEHORZ, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SIZEVERT);
	insert_image_item_from_file(g_ptrImages, BMP_SIZEVERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PAINT);
	insert_image_item_from_file(g_ptrImages, BMP_PAINT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_EXEC);
	insert_image_item_from_file(g_ptrImages, BMP_EXEC, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_UP);
	insert_image_item_from_file(g_ptrImages, BMP_UP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DOWN);
	insert_image_item_from_file(g_ptrImages, BMP_DOWN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_KEYBORD);
	insert_image_item_from_file(g_ptrImages, BMP_KEYBORD, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SUM);
	insert_image_item_from_file(g_ptrImages, BMP_SUM, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DRAW);
	insert_image_item_from_file(g_ptrImages, BMP_DRAW, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_GROUP);
	insert_image_item_from_file(g_ptrImages, BMP_GROUP, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_MACRO);
	insert_image_item_from_file(g_ptrImages, BMP_MACRO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SENDBACK);
	insert_image_item_from_file(g_ptrImages, BMP_SENDBACK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SHAPE);
	insert_image_item_from_file(g_ptrImages, BMP_SHAPE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_BORDER);
	insert_image_item_from_file(g_ptrImages, BMP_BORDER, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LABEL);
	insert_image_item_from_file(g_ptrImages, BMP_LABEL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TEXT);
	insert_image_item_from_file(g_ptrImages, BMP_TEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FRESH);
	insert_image_item_from_file(g_ptrImages, BMP_FRESH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CHECK);
	insert_image_item_from_file(g_ptrImages, BMP_CHECK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_MEMO);
	insert_image_item_from_file(g_ptrImages, BMP_MEMO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_GRID);
	insert_image_item_from_file(g_ptrImages, BMP_GRID, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_GRAPH);
	insert_image_item_from_file(g_ptrImages, BMP_GRAPH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_IMAGE);
	insert_image_item_from_file(g_ptrImages, BMP_IMAGE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PHOTO);
	insert_image_item_from_file(g_ptrImages, BMP_PHOTO, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_BOOK);
	insert_image_item_from_file(g_ptrImages, BMP_BOOK, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_INSERT);
	insert_image_item_from_file(g_ptrImages, BMP_INSERT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PLUS);
	insert_image_item_from_file(g_ptrImages, BMP_PLUS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_MINUS);
	insert_image_item_from_file(g_ptrImages, BMP_MINUS, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_ERASE);
	insert_image_item_from_file(g_ptrImages, BMP_ERASE, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_IMPORT);
	insert_image_item_from_file(g_ptrImages, BMP_IMPORT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_INPUT);
	insert_image_item_from_file(g_ptrImages, BMP_INPUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_OUTPUT);
	insert_image_item_from_file(g_ptrImages, BMP_OUTPUT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SQL);
	insert_image_item_from_file(g_ptrImages, BMP_SQL, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FETCH);
	insert_image_item_from_file(g_ptrImages, BMP_FETCH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_COMMIT);
	insert_image_item_from_file(g_ptrImages, BMP_COMMIT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_HELPC);
	insert_image_item_from_file(g_ptrImages, BMP_HELPC, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_HELPH);
	insert_image_item_from_file(g_ptrImages, BMP_HELPH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LOGIN);
	insert_image_item_from_file(g_ptrImages, BMP_LOGIN, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PERSON);
	insert_image_item_from_file(g_ptrImages, BMP_PERSON, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_FIRST);
	insert_image_item_from_file(g_ptrImages, BMP_FIRST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_NEXT);
	insert_image_item_from_file(g_ptrImages, BMP_NEXT, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PREV);
	insert_image_item_from_file(g_ptrImages, BMP_PREV, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LAST);
	insert_image_item_from_file(g_ptrImages, BMP_LAST, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_HERF);
	insert_image_item_from_file(g_ptrImages, BMP_HERF, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_RICH);
	insert_image_item_from_file(g_ptrImages, BMP_RICH, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_STATICBOX);
	insert_image_item_from_file(g_ptrImages, BMP_STATICBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_EDITBOX);
	insert_image_item_from_file(g_ptrImages, BMP_EDITBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_LISTBOX);
	insert_image_item_from_file(g_ptrImages, BMP_LISTBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_NAVIBOX);
	insert_image_item_from_file(g_ptrImages, BMP_NAVIBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SPINBOX);
	insert_image_item_from_file(g_ptrImages, BMP_SPINBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_SLIDEBOX);
	insert_image_item_from_file(g_ptrImages, BMP_SLIDEBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_RADIOBOX);
	insert_image_item_from_file(g_ptrImages, BMP_RADIOBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_CHECKBOX);
	insert_image_item_from_file(g_ptrImages, BMP_CHECKBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DATEBOX);
	insert_image_item_from_file(g_ptrImages, BMP_DATEBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_TIMEBOX);
	insert_image_item_from_file(g_ptrImages, BMP_TIMEBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_PUSHBOX);
	insert_image_item_from_file(g_ptrImages, BMP_PUSHBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_GROUPBOX);
	insert_image_item_from_file(g_ptrImages, BMP_GROUPBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_USERBOX);
	insert_image_item_from_file(g_ptrImages, BMP_USERBOX, NULL, path);

	xsprintf(path, _T("%s\\..\\image\\%s.bmp"), g_szRunPath, BMP_DIALOG);
	insert_image_item_from_file(g_ptrImages, BMP_DIALOG, NULL, path);

	LINKPTR ptr_string = create_string_table();
	string_table_parse_string(ptr_string, GDI_ATTR_RGB_OPTIONS, -1, OPT_ITEMFEED, OPT_LINEFEED);

	LINKPTR ent = get_string_next_entity(ptr_string, LINK_FIRST);
	while (ent)
	{
		insert_image_item_from_color(g_ptrImages, get_string_entity_val_ptr(ent), get_string_entity_key_ptr(ent));
		
		ent = get_string_next_entity(ptr_string, ent);
	}
	destroy_string_table(ptr_string);
}

VOID FreeResource()
{
	destroy_image_list(g_ptrImages);

	if (g_bmpThumb)
		delete_bitmap(g_bmpThumb);
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
