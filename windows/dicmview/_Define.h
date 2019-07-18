/***********************************************************************
	Easily DICMView v1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc _Define interface document

	@module	_Define interface file

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

#include <xdl.h>
#include <xdc.h>
#include <dicm.h>

#define IDA_PROJECT_CREATE		1000
#define IDA_PROJECT_CLOSE		1001
#define IDA_PROJECT_SAVE		1002
#define IDA_PROJECT_OPEN		1003
#define IDA_CONFIG_RDS			1010
#define IDA_CONFIG_DOC			1011
#define IDA_CONFIG_TEC			1012

#define IDA_FILE_CREATE			2000
#define IDA_FILE_SAVEAS			2001
#define IDA_FILE_SAVE			2002
#define IDA_FILE_OPEN			2003
#define IDA_FILE_PRINT			2004
#define IDA_FILE_PREVIEW		2005
#define IDA_FILE_APPEND			2006
#define IDA_FILE_REMOVE			2007
#define IDA_FILE_SHOW			2008
#define IDA_FILE_SYNC			2009
#define IDA_FILE_SCHEMA			2010
#define IDA_FILE_EXPORT			2011
#define IDA_FILE_IMPORT			2012
#define IDA_FILE_SORT			2013
#define IDA_FILE_EXEC			2014

#define IDA_ELEMENT_INSERT		2020
#define IDA_ELEMENT_REMOVE		2021
#define IDA_ELEMENT_EDIT		2022
#define IDA_ELEMENT_IMPORT		2024
#define IDA_ELEMENT_EXPORT		2025
#define IDA_ELEMENT_DROP		2026

#define IDA_OBJECT_FRESH		2030
#define IDA_OBJECT_CHECK		2031
#define IDA_OBJECT_DELETE		2032

#define IDA_EDIT_SELECTALL		4000
#define IDA_EDIT_DELETE			4001
#define IDA_EDIT_COPY			4002
#define IDA_EDIT_CUT			4003
#define IDA_EDIT_PASTE			4004
#define IDA_EDIT_UNDO			4005
#define IDA_EDIT_INSERT			4006
#define IDA_EDIT_ATTACH			4007
#define IDA_EDIT_DETACH			4008
#define IDA_EDIT_EDITEX			4009

#define IDA_WINDOW_TYPE			5000
#define IDA_WINDOW_ZOOMIN		5001
#define IDA_WINDOW_ZOOMOUT		5002
#define IDA_ANNOTA_ANGLE		5010
#define IDA_ANNOTA_RANGE		5011
#define IDA_ANNOTA_REGION		5012
#define IDA_ANNOTA_TEXT			5013

#define MAINFRAME_TOOLGROUP_HEIGHT		(float)7
#define MAINFRAME_TOOLGROUP_MAXWIDTH	(float)18
#define MAINFRAME_TOOLGROUP_MINWIDTH	(float)7

#define MAINFRAME_TOOLGROUP_PROJECT		_T("group_project")
#define MAINFRAME_TOOLGROUP_FILE		_T("group-file")
#define MAINFRAME_TOOLGROUP_SETTING		_T("group-setting")

#define MAINFRAME_TOOLGROUP_EDIT		_T("group-edit")
#define MAINFRAME_TOOLGROUP_STYLE		_T("group-style")
#define MAINFRAME_TOOLGROUP_ELEMENT		_T("group-element")

#define MAINFRAME_TOOLGROUP_MASK		_T("group-mask")
#define MAINFRAME_TOOLGROUP_GRAY		_T("group-gray")

#define BMP_NEW				_T("new")
#define BMP_OPEN			_T("open")
#define BMP_FILE			_T("file")
#define BMP_SAVE			_T("save")
#define BMP_SAVEAS			_T("saveas")
#define BMP_SAVEALL			_T("saveall")
#define BMP_SCHEMA			_T("schema")
#define BMP_PRINT			_T("print")
#define BMP_PREVIEW			_T("preview")
#define BMP_CLOSE			_T("close")
#define BMP_FIND			_T("find")
#define BMP_ICON			_T("icon")
#define BMP_EARTH			_T("earth")
#define BMP_RULER			_T("ruler")
#define BMP_SEARCH			_T("search")
#define BMP_ANCHOR			_T("anchor")
#define BMP_ANGLE			_T("angle")
#define BMP_ALTER			_T("flag")
#define BMP_STAR			_T("star")
#define BMP_NOTE			_T("note")
#define BMP_TEXT			_T("text")
#define BMP_WINDOW			_T("window")
#define BMP_ZOOMIN			_T("zoomin")
#define BMP_ZOOMOUT			_T("zoomout")

#define BMP_SELECTALL		_T("selectall")
#define BMP_DELETE			_T("delete")
#define BMP_COPY			_T("copy")
#define BMP_CUT				_T("cut")
#define BMP_UNDO			_T("undo")
#define BMP_PASTE			_T("paste")
#define BMP_FONTNAME		_T("fontname")
#define BMP_FONTSIZE		_T("fontsize")
#define BMP_FONTCOLOR		_T("fontcolor")
#define BMP_FONTBOLD		_T("fontbold")
#define BMP_TEXTNEAR		_T("textnear")
#define BMP_TEXTCENTER		_T("textcenter")
#define BMP_TEXTFAR			_T("textfar")
#define BMP_ALIGNLEFT		_T("alignleft")
#define BMP_ALIGNRIGHT		_T("alignright")
#define BMP_ALIGNTOP		_T("aligntop")
#define BMP_ALIGNBOTTOM		_T("alignbottom")
#define BMP_ALIGNCENTER		_T("aligncenter")
#define BMP_SIZEWIDTH		_T("sizewidth")
#define BMP_SIZEHEIGHT		_T("sizeheight")
#define BMP_SIZEALL			_T("sizeall")
#define BMP_SIZEHORZ		_T("sizehorz")
#define BMP_SIZEVERT		_T("sizevert")
#define BMP_PAINT			_T("paint")
#define BMP_PREVIEW			_T("preview")
#define BMP_DRAW			_T("draw")
#define BMP_GROUP			_T("group")
#define BMP_SENDBACK		_T("sendback")
#define BMP_PROPER			_T("proper")
#define BMP_MACRO			_T("macro")
#define BMP_BORDER			_T("border")
#define BMP_FRESH			_T("fresh")
#define BMP_EXEC			_T("exec")
#define BMP_UP				_T("up")
#define BMP_DOWN			_T("down")
#define BMP_KEYBORD			_T("keybord")
#define BMP_SUM				_T("sum")

#define BMP_SHAPE			_T("shape")
#define BMP_LABEL			_T("label")
#define BMP_TEXT			_T("text")
#define BMP_MEMO			_T("memo")
#define BMP_GRID			_T("grid")
#define BMP_GRAPH			_T("graph")
#define BMP_IMAGE			_T("image")
#define BMP_PHOTO			_T("photo")
#define BMP_CHECK			_T("check")
#define BMP_EDIT			_T("edit")
#define BMP_RICH			_T("rich")
#define BMP_HERF			_T("herf")
#define BMP_FORM			_T("new")
#define BMP_PERSON			_T("person")

#define BMP_STATICBOX		_T("staticbox")
#define BMP_EDITBOX			_T("editbox")
#define BMP_LISTBOX			_T("listbox")
#define BMP_NAVIBOX			_T("navibox")
#define BMP_SPINBOX			_T("spinbox")
#define BMP_SLIDEBOX		_T("slidebox")
#define BMP_RADIOBOX		_T("radiobox")
#define BMP_DATEBOX			_T("datebox")
#define BMP_TIMEBOX			_T("timebox")
#define BMP_PUSHBOX			_T("pushbox")
#define BMP_GROUPBOX		_T("groupbox")
#define BMP_USERBOX			_T("userbox")
#define BMP_CHECKBOX		_T("checkbox")
#define BMP_DIALOG			_T("dialog")

#define BMP_BOOK			_T("book")
#define BMP_INSERT			_T("insert")
#define BMP_PLUS			_T("plus")
#define BMP_MINUS			_T("minus")
#define BMP_ERASE			_T("erase")
#define BMP_IMPORT			_T("import")
#define BMP_EXPORT			_T("export")
#define BMP_INPUT			_T("input")
#define BMP_OUTPUT			_T("output")
#define BMP_HELPC			_T("helpc")
#define BMP_HELPH			_T("helph")
#define BMP_SQL				_T("sql")
#define BMP_FETCH			_T("fetch")
#define BMP_COMMIT			_T("commit")

#define BMP_FIRST			_T("first")
#define BMP_LAST			_T("last")
#define BMP_PREV			_T("prev")
#define BMP_NEXT			_T("next")
#define BMP_PRINTER			_T("printer")

#define BMP_LOGIN			_T("login")