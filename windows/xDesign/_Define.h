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

#define IDA_OBJECT_FRESH		2030
#define IDA_OBJECT_CHECK		2031
#define IDA_OBJECT_DELETE		2032

#define IDA_DATABASE_EXECUTE	2040
#define IDA_DATABASE_SCHEMA		2041
#define IDA_DATABASE_SELECT		2042

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

#define IDA_STYLE_FONT_NAME		5000
#define IDA_STYLE_FONT_SIZE		5001
#define IDA_STYLE_FONT_WEIGHT	5002
#define IDA_STYLE_TEXT_COLOR	5003
#define IDA_STYLE_PAINT_COLOR	5004
#define IDA_STYLE_DRAW_COLOR	5005
#define IDA_STYLE_TEXT_NEAR		5006
#define IDA_STYLE_TEXT_CENTER	5007
#define IDA_STYLE_TEXT_FAR		5008
#define IDA_STYLE_PROPER		5009

#define IDA_STYLE_ALIGN_NEAR	5100
#define IDA_STYLE_ALIGN_CENTER	5101
#define IDA_STYLE_ALIGN_FAR		5102
#define IDA_STYLE_SIZE_HORZ		5103
#define IDA_STYLE_SIZE_VERT		5104
#define IDA_STYLE_SIZE_WIDTH	5105
#define IDA_STYLE_SIZE_HEIGHT	5106
#define IDA_STYLE_GROUP			5107
#define IDA_STYLE_SENDBACK		5108
#define IDA_STYLE_FIELD_SHAPE	5109

#define IDA_APPEND_LABEL		5200
#define IDA_APPEND_TEXT			5201
#define IDA_APPEND_RICH			5202
#define IDA_APPEND_PHOTO		5204
#define IDA_APPEND_SHAPE		5205
#define IDA_APPEND_HREF			5206
#define IDA_APPEND_CHECK		5207
#define IDA_APPEND_MEMO			5208
#define IDA_APPEND_TAG			5209
#define IDA_APPEND_PAGENUM		5210
#define IDA_APPEND_TABLE		5211
#define IDA_APPEND_CODE			5212
#define IDA_APPEND_EMBED_GRID		5213
#define IDA_APPEND_EMBED_STATIS		5214
#define IDA_APPEND_EMBED_IMAGES		5215
#define IDA_APPEND_EMBED_FORM		5216

#define IDA_INSERT_COL			5210
#define IDA_INSERT_ROW			5211
#define IDA_DELETE_ROW			5212
#define IDA_ERASE_ROWS			5213
#define IDA_FRESH_ROWS			5214
#define IDA_UPDATE_ROWS			5215
#define IDA_FILL_CODE			5216
#define IDA_FILL_HELP			5217
#define IDA_IMPORT_COLS			5218
#define IDA_SELECT_COLS			5219

#define IDA_INSERT_GAX			5220
#define IDA_INSERT_YAX			5221
#define IDA_INSERT_XAX			5222
#define IDA_DELETE_XAX			5223
#define IDA_ERASE_XAXS			5224
#define IDA_IMPORT_XAXS			5225
#define IDA_UPDATE_XAXS			5226

#define IDA_ATTRIBUTES			5301
#define IDA_STYLESHEET			5302
#define IDA_DATASET				5303
#define IDA_RESULT				5304

#define IDA_APPEND_STATICBOX	5310
#define IDA_APPEND_EDITBOX		5311
#define IDA_APPEND_LISTBOX		5312
#define IDA_APPEND_NAVIBOX		5313
#define IDA_APPEND_SPINBOX		5314
#define IDA_APPEND_SLIDEBOX		5315
#define IDA_APPEND_RADIOBOX		5316
#define IDA_APPEND_CHECKBOX		5317
#define IDA_APPEND_DATEBOX		5318
#define IDA_APPEND_TIMEBOX		5319
#define IDA_APPEND_SHAPEBOX		5320
#define IDA_APPEND_PUSHBOX		5321
#define IDA_APPEND_USERBOX		5322

#define IDA_APPEND_GEOMAP		5330
#define IDA_APPEND_TRENDS		5331
#define IDA_APPEND_SCATTER		5332
#define IDA_APPEND_PANTO		5333
#define IDA_APPEND_DENSITY		5334
#define IDA_APPEND_COUNTER		5335

#define IDA_APPEND_PROCESS		5340
#define IDA_APPEND_JUDGE		5341
#define IDA_APPEND_DOCUMENT		5342
#define IDA_APPEND_SHEET		5343
#define IDA_APPEND_JOINT		5344

#define PANEL_CLASS_FORM		_T("FormPanel")
#define PANEL_CLASS_GRID		_T("GridPanel")
#define PANEL_CLASS_STATIS		_T("StatisPanel")
#define PANEL_CLASS_RICH		_T("RichPanel")
#define PANEL_CLASS_SCHEMA		_T("SchemaPanel")
#define PANEL_CLASS_TOPOG		_T("TopogPanel")
#define PANEL_CLASS_IMAGE		_T("ImagePanel")
#define PANEL_CLASS_DIALOG		_T("DialogPanel")
#define PANEL_CLASS_PANORAMA	_T("PanoramaPanel")
#define PANEL_CLASS_DIAGRAM		_T("DiagramPanel")
#define PANEL_CLASS_TAG			_T("TagPanel")
#define PANEL_CLASS_SQL			_T("SQLPanel")
#define PANEL_CLASS_XML			_T("XMLPanel")
#define PANEL_CLASS_JSON		_T("JsonPanel")


#define MAINFRAME_TOOLGROUP_PROJECT		_T("group_project")
#define MAINFRAME_TOOLGROUP_FILE		_T("group-file")
#define MAINFRAME_TOOLGROUP_SETTING		_T("group-setting")

#define MAINFRAME_TOOLGROUP_EDIT		_T("group-edit")
#define MAINFRAME_TOOLGROUP_STYLE		_T("group-style")
#define MAINFRAME_TOOLGROUP_ELEMENT		_T("group-element")

