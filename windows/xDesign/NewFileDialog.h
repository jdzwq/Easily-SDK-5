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

#define NEWFILEDLG_RET_NEWFORM		1
#define NEWFILEDLG_RET_NEWGRID		2
#define NEWFILEDLG_RET_NEWGRAPH		3
#define NEWFILEDLG_RET_NEWRICH		4
#define NEWFILEDLG_RET_NEWSCHEMA	5
#define NEWFILEDLG_RET_NEWTOPOG		6
#define NEWFILEDLG_RET_NEWIMAGES	7
#define NEWFILEDLG_RET_NEWDIALOG	8
#define NEWFILEDLG_RET_TMPL			10

typedef struct _NEWFILEDLG_PARAM{
	bool_t bMode;
	tchar_t szPath[PATH_LEN];
}NEWFILEDLG_PARAM;

res_win_t NewFileDlg_Create(const tchar_t* title, NEWFILEDLG_PARAM* ppd);