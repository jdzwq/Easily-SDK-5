/***********************************************************************
	Easily DICMView v1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc _Frame interface document

	@module	_Frame interface file

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

res_win_t MainFrame_Create(const tchar_t* mname);

res_win_t MainFrame_GetActivePanel(res_win_t widget);

void MainFrame_UpdatePanel(res_win_t widget, res_win_t hPanel, const tchar_t* sz_title);

void MainFrame_MergeTool(res_win_t widget, LINKPTR subTool);

void MainFrame_ClearTool(res_win_t widget);

void MainFrame_UpdateWindow(res_win_t widget, const tchar_t* szWindow);

void MainFrame_UpdateCenter(res_win_t widget, const tchar_t* szCenter);