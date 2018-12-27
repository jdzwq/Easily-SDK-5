/***********************************************************************
	Easily xDesign v3.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc xDesign interface document

	@module	xDesign interface file

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

res_win_t MainFrame_Create(const tchar_t* mname);

LINKPTR MainFrame_GetProject(res_win_t widget);

res_win_t MainFrame_GetActivePanel(res_win_t widget);

void MainFrame_UpdatePanel(res_win_t widget, res_win_t hPanel, const tchar_t* sz_title);

void MainFrame_MergeTool(res_win_t widget, LINKPTR subTool);

void MainFrame_ClearTool(res_win_t widget);

typedef struct _DOMMAIN_ITEM{
	tchar_t Name[KEY_LEN + 1];
	tchar_t Title[RES_LEN + 1];
	tchar_t DataType[RES_LEN + 1];
	tchar_t DataLen[NUM_LEN + 1];
	tchar_t DataDig[NUM_LEN + 1];
	tchar_t CodeSystem[RES_LEN + 1];
}DOMAINITEM, *LPDOMAINITEM;

typedef struct _DROPDOMAIN{
	DOMAINITEM dm;
	xpoint_t xp;
}DROPDOMAIN;

typedef struct _QUERYOBJECT{
	tchar_t szDoc[RES_LEN + 1];
	LINKPTR ptrDoc;
}QUERYOBJECT;

typedef struct _META_DATA{
	tchar_t Author[RES_LEN + 1];
	tchar_t Company[RES_LEN + 1];
}METADATA;