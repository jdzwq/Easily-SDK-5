/***********************************************************************
	Easily DICMView v1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc _Moudle interface document

	@module	_Moudle interface file

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

#pragma once

#include "_Define.h"

extern HINSTANCE	g_hInst;
extern HWND			g_hMain;
extern tchar_t		g_szRunPath[PATH_LEN];
extern LINKPTR		g_ptrImages;
extern res_bmp_t	g_bmpThumb;


typedef struct _FACE_STYLE{
	tchar_t title[10];
	tchar_t bkg[CLR_LEN + 1];
	tchar_t frg[CLR_LEN + 1];
	tchar_t txt[CLR_LEN + 1];
	tchar_t msk[CLR_LEN + 1];
}FACE_STYLE;

#define FACE_COUNT	6

static FACE_STYLE g_face[FACE_COUNT] = {
	{ _T("̫�պ�"), GDI_ATTR_RGB_HARDBLACK, GDI_ATTR_RGB_SOFTBLACK, GDI_ATTR_RGB_GRAY, GDI_ATTR_RGB_DARKWHITE },
	{ _T("������"), GDI_ATTR_RGB_LIGHTGREEN, GDI_ATTR_RGB_LIGHTCYAN, GDI_ATTR_RGB_DARKBLACK, GDI_ATTR_RGB_SOFTWHITE },
	{ _T("������"), GDI_ATTR_RGB_SOFTYELLOW, GDI_ATTR_RGB_SOFTGREEN, GDI_ATTR_RGB_DARKBLACK, GDI_ATTR_RGB_SOFTWHITE },
	{ _T("ϲ���"), GDI_ATTR_RGB_LIGHTRED, GDI_ATTR_RGB_SOFTGREEN, GDI_ATTR_RGB_DARKBLACK, GDI_ATTR_RGB_SOFTWHITE },
	{ _T("������"), GDI_ATTR_RGB_LIGHTPURPLE, GDI_ATTR_RGB_LIGHTCYAN, GDI_ATTR_RGB_DARKBLACK, GDI_ATTR_RGB_SOFTWHITE },
	{ _T("�����"), GDI_ATTR_RGB_LIGHTWHITE, GDI_ATTR_RGB_GRAY, GDI_ATTR_RGB_BLACK, GDI_ATTR_RGB_SOFTWHITE }
};


extern int g_indFace;

#define PERFERENCE_FACE		_T("FACE")

#define COLOR_TABLE_SIZE	17

static tchar_t COLOR_TABLE[COLOR_TABLE_SIZE][CLR_LEN] = {
	GDI_ATTR_RGB_WHITE,
	GDI_ATTR_RGB_GRAY,
	GDI_ATTR_RGB_RED,
	GDI_ATTR_RGB_PURPLE,
	GDI_ATTR_RGB_GREEN,
	GDI_ATTR_RGB_BLUE,
	GDI_ATTR_RGB_CYAN,
	GDI_ATTR_RGB_YELLOW,
	GDI_ATTR_RGB_BLACK,
	GDI_ATTR_RGB_SOFTWHITE,
	GDI_ATTR_RGB_SOFTGRAY,
	GDI_ATTR_RGB_SOFTPURPLE,
	GDI_ATTR_RGB_SOFTGREEN,
	GDI_ATTR_RGB_SOFTBLUE,
	GDI_ATTR_RGB_SOFTCYAN,
	GDI_ATTR_RGB_SOFTYELLOW,
	GDI_ATTR_RGB_SOFTBLACK
};


VOID	SavePreference(const tchar_t* sec, const tchar_t* key, const tchar_t* val);

VOID	LoadPreference(const tchar_t* sec, const tchar_t* key, tchar_t* val);

INT		ShowMsg( dword_t mid, const tchar_t* text);