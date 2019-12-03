/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc icon document

	@module	icongdi.h | interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#ifndef _ICONGDI_H
#define _ICONGDI_H

#include "xdldef.h"

#if defined(XDK_SUPPORT_CONTEXT) && defined(XDK_SUPPORT_CONTEXT_GRAPHIC)

#ifdef	__cplusplus
extern "C" {
#endif

#define ICON_ALIGNCENTER	_T("alignCenter")
#define ICON_ALIGNFAR		_T("alignFar")
#define ICON_ALIGNNEAR		_T("alignNear")
#define ICON_APPEND			_T("append")
#define ICON_ARRANGECENTER	_T("arrangeCenter")
#define ICON_ARRANGELEFT	_T("arrangeLeft")
#define ICON_ARRANGERIGHT	_T("arrangeRight")
#define ICON_BACKGROUND		_T("background")
#define ICON_BEGIN			_T("begin")
#define ICON_BOOK			_T("book")
#define ICON_BORDER			_T("border")
#define ICON_CALENDAR		_T("calendar")
#define ICON_CHECK			_T("check")
#define ICON_CHECKBOX		_T("checkBox")
#define ICON_CHECKED		_T("checked")
#define ICON_CLEAR			_T("clear")
#define ICON_CLOSE			_T("close")
#define ICON_COLLAPSE		_T("collapse")
#define ICON_COPY			_T("copy")
#define ICON_COUNTER		_T("counter")
#define ICON_CUT			_T("cut")
#define ICON_DATE			_T("date")
#define ICON_DELETE			_T("delete")
#define ICON_DENSITY		_T("density")
#define ICON_DIAGRAM		_T("diagram")
#define ICON_DIALOG			_T("dialog")
#define ICON_DOC			_T("doc")
#define ICON_DOWN			_T("down")
#define ICON_EDIT			_T("edit")
#define ICON_END			_T("end")
#define ICON_EXECUTE		_T("execute")
#define ICON_EXPAND			_T("expand")
#define ICON_EXPORT			_T("export")
#define ICON_FETCH			_T("fetch")
#define ICON_FIND			_T("find")
#define ICON_FIRST			_T("first")
#define ICON_FIXED			_T("fixed")
#define ICON_FRESH			_T("fresh")
#define ICON_FOLDER			_T("folder")
#define ICON_FONTCOLOR		_T("fontColor")
#define ICON_FONTWEIGHT		_T("fontWeight")
#define ICON_FONTNAME		_T("fontName")
#define ICON_FONTSIZE		_T("fontSize")
#define ICON_FONTSTYLE		 _T("fontStyle")
#define ICON_FOREGROUND		_T("foreground")
#define ICON_GRAPH			_T("graph")
#define ICON_GRID			_T("grid")
#define ICON_GROUP			_T("group")
#define ICON_GUIDER			_T("guider")
#define ICON_HELPC			_T("helpc")
#define ICON_HELPP			_T("helpp")
#define ICON_HERF			_T("herf")
#define ICON_HOME			_T("home")
#define ICON_IMAGES			_T("images")
#define ICON_IMPORT			_T("import")
#define ICON_INPUT			_T("input")
#define ICON_INSERT			_T("insert")
#define ICON_JUDGE			_T("judge")
#define ICON_KEYBOX			_T("keyBox")
#define ICON_KEYBOXED		_T("keyBoxed")
#define ICON_LABEL			_T("label")
#define ICON_LAST			_T("last")
#define ICON_LIST			_T("list")
#define ICON_LOCATION		_T("location")
#define ICON_LOCK			_T("lock")
#define ICON_LOGO			_T("logo")
#define ICON_MAXIMIZE		_T("maximize")
#define ICON_MEMO			_T("memo")
#define ICON_MINIMIZE		_T("minimize")
#define ICON_MINUS			_T("minus")
#define ICON_MULTITEXT		_T("multiText")
#define ICON_NAVI			_T("navi")
#define ICON_NEW			_T("new")
#define ICON_NEXT			_T("next")
#define ICON_NOTE			_T("note")
#define ICON_NUMERIC		_T("numeric")
#define ICON_OPEN			_T("open")
#define ICON_ORDER			_T("order")
#define ICON_OUTPUT			_T("output")
#define ICON_PANTO			_T("panto")
#define ICON_PASTE			_T("paste")
#define ICON_PAUSE			_T("pause")
#define ICON_PHOTO			_T("photo")
#define ICON_PLUS			_T("plus")
#define ICON_PREV			_T("prev")
#define ICON_PREVIEW		_T("preview")
#define ICON_PRINT			_T("print")
#define ICON_PROCESS		_T("process")
#define ICON_PROPER			_T("proper")
#define ICON_PUSH			_T("push")
#define ICON_RADIO			_T("radio")
#define ICON_RADIOBOX		_T("radioBox")
#define ICON_RADIOED		_T("radioed")
#define ICON_REMOVE			_T("remove")
#define ICON_REDO			_T("redo")
#define ICON_RESTORE		_T("restore")
#define ICON_RICH			_T("rich")
#define ICON_SAVE			_T("save")
#define ICON_SAVEAS			_T("saveas")
#define ICON_SCATTER		_T("scatter")
#define ICON_SCHEMA			_T("schema")
#define ICON_SCREEN			_T("screen")
#define ICON_SELECTALL		_T("selectAll")
#define ICON_SELECTED		_T("selected")
#define ICON_SHAPE			_T("shape")
#define ICON_SINGLETEXT		_T("singleText")
#define ICON_SIZEHORZ		_T("sizeHorz")
#define ICON_SIZEVERT		_T("sizeVert")
#define ICON_SLIDE			_T("slide")
#define ICON_SPACEHORZ		_T("spaceHorz")
#define ICON_SPACEVERT		_T("spaceVert")
#define ICON_SPIN			_T("spin")
#define ICON_START			_T("start")
#define ICON_STATIC			_T("static")
#define ICON_STOP			_T("stop")
#define ICON_STYLE			_T("style")
#define ICON_SUM			_T("sum")
#define ICON_TABLE			_T("table")
#define ICON_TAG			_T("tag")
#define ICON_TIME			_T("time")
#define ICON_TOUCH			_T("touch")
#define ICON_TOUCHED		_T("touched")
#define ICON_TRENDS			_T("trends")
#define ICON_UNDO			_T("undo")
#define ICON_UNLOCK			_T("unlock")
#define ICON_UP				_T("up")
#define ICON_UPDATE			_T("update")
#define ICON_USER			_T("user")
#define ICON_ZOOMIN			_T("zoomIn")
#define ICON_ZOOMOUT		_T("zoomOut")

#define ICON_RECT			_T("rect")
#define ICON_ELLIPSE		_T("ellipse")
#define ICON_CROSS			_T("cross")
#define ICON_STAR			_T("star")
#define ICON_DIAMOND		_T("diamond")

typedef void(*PF_ICON_MAKE)(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

typedef struct _ICON_DRAW_TABLE{
	tchar_t icon_name[32];
	PF_ICON_MAKE icon_func;
}ICON_DRAW_TABLE;

XDL_API PF_ICON_MAKE find_icon_maker(const tchar_t* iname);

XDL_API void draw_logo_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void draw_close_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void draw_maximize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void draw_minimize_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

XDL_API void draw_restore_icon_raw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* prt);

#if defined(_DEBUG) || defined(DEBUG)

XDL_API void test_icon_draw(res_ctx_t rdc, const xcolor_t* pxc, const xrect_t* pxr);

#endif 


#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_CONTEXT*/

#endif /*ICONGDI_H*/