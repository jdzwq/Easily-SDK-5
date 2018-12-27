/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdc language document

	@module	_xdc_ansi.h | xdc language interface file

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

#ifndef _XDCANSI_H
#define _XDCANSI_H

#ifdef LANG_CN

#ifdef TEXTOR_MENU_UNDO
#undef TEXTOR_MENU_UNDO
#define TEXTOR_MENU_UNDO			_T("撤销")
#endif
#ifdef TEXTOR_MENU_CUT
#undef TEXTOR_MENU_CUT
#define TEXTOR_MENU_CUT				_T("剪切")
#endif
#ifdef TEXTOR_MENU_PASTE
#undef TEXTOR_MENU_PASTE
#define TEXTOR_MENU_PASTE			_T("粘帖")
#endif
#ifdef TEXTOR_MENU_COPY
#undef TEXTOR_MENU_COPY
#define TEXTOR_MENU_COPY			_T("复制")
#endif

#ifdef MSGDLG_PUSHBOX_CLOSE
#undef MSGDLG_PUSHBOX_CLOSE
#define MSGDLG_PUSHBOX_CLOSE		_T("关闭")
#endif
#ifdef MSGDLG_PUSHBOX_OK
#undef MSGDLG_PUSHBOX_OK
#define MSGDLG_PUSHBOX_OK			_T("确定")
#endif
#ifdef MSGDLG_PUSHBOX_CANCEL
#undef MSGDLG_PUSHBOX_CANCEL
#define MSGDLG_PUSHBOX_CANCEL		_T("撤销")
#endif
#ifdef MSGDLG_PUSHBOX_YES
#undef MSGDLG_PUSHBOX_YES
#define MSGDLG_PUSHBOX_YES			_T("是")
#endif
#ifdef MSGDLG_PUSHBOX_NO
#undef MSGDLG_PUSHBOX_NO
#define MSGDLG_PUSHBOX_NO			_T("否")
#endif
#ifdef MSGDLG_PUSHBOX_KNOWN
#undef MSGDLG_PUSHBOX_KNOWN
#define MSGDLG_PUSHBOX_KNOWN		_T("下次不再提醒")
#endif

#ifdef MSGDLG_TITLE_TIP
#undef MSGDLG_TITLE_TIP
#define MSGDLG_TITLE_TIP		_T("提示")
#endif
#ifdef MSGDLG_TITLE_WRN
#undef MSGDLG_TITLE_WRN
#define MSGDLG_TITLE_WRN		_T("警告")
#endif
#ifdef MSGDLG_TITLE_ERR
#undef MSGDLG_TITLE_ERR
#define MSGDLG_TITLE_ERR		_T("错误")
#endif

#ifdef LISTDLG_PUSHBOX_OK
#undef LISTDLG_PUSHBOX_OK
#define LISTDLG_PUSHBOX_OK		_T("确定")
#endif

#ifdef ANNODLG_PUSHBOX_OK
#undef ANNODLG_PUSHBOX_OK
#define ANNODLG_PUSHBOX_OK		_T("确定")
#endif
#ifdef ANNODLG_PUSHBOX_COMMIT
#undef ANNODLG_PUSHBOX_COMMIT
#define ANNODLG_PUSHBOX_COMMIT	_T("提交")
#endif
#ifdef ANNODLG_PUSHBOX_FONTSIZE
#undef ANNODLG_PUSHBOX_FONTSIZE
#define ANNODLG_PUSHBOX_FONTSIZE	_T("字号")
#endif
#ifdef ANNODLG_PUSHBOX_FONTCOLOR
#undef ANNODLG_PUSHBOX_FONTCOLOR
#define ANNODLG_PUSHBOX_FONTCOLOR	_T("颜色")
#endif

#ifdef PREVIEWDLG_TREE_TITLE
#undef PREVIEWDLG_TREE_TITLE
#define PREVIEWDLG_TREE_TITLE		_T("文档列表")
#endif
#ifdef PREVIEWDLG_TREE_DEFITEM
#undef PREVIEWDLG_TREE_DEFITEM
#define PREVIEWDLG_TREE_DEFITEM		_T("SVG文档")
#endif
#ifdef PREVIEWDLG_STATUS_PAGEGUID
#undef PREVIEWDLG_STATUS_PAGEGUID
#define PREVIEWDLG_STATUS_PAGEGUID	_T("第%d页 共%d页")
#endif
#ifdef PREVIEWDLG_PUSHBOX_CLOSE
#undef PREVIEWDLG_PUSHBOX_CLOSE
#define PREVIEWDLG_PUSHBOX_CLOSE	_T("关闭窗口")
#endif
#ifdef PREVIEWDLG_PUSHBOX_PRINTCUR
#undef PREVIEWDLG_PUSHBOX_PRINTCUR
#define PREVIEWDLG_PUSHBOX_PRINTCUR	_T("打印当前")
#endif
#ifdef PREVIEWDLG_PUSHBOX_PRINTSEL
#undef PREVIEWDLG_PUSHBOX_PRINTSEL
#define PREVIEWDLG_PUSHBOX_PRINTSEL	_T("打印勾选")
#endif
#ifdef PREVIEWDLG_PUSHBOX_PRINTALL
#undef PREVIEWDLG_PUSHBOX_PRINTALL
#define PREVIEWDLG_PUSHBOX_PRINTALL	_T("打印全部")
#endif
#ifdef PREVIEWDLG_PUSHBOX_SETUP
#undef PREVIEWDLG_PUSHBOX_SETUP
#define PREVIEWDLG_PUSHBOX_SETUP	_T("打印设置")
#endif
#ifdef PREVIEWDLG_PUSHBOX_SAVEAS
#undef PREVIEWDLG_PUSHBOX_SAVEAS
#define PREVIEWDLG_PUSHBOX_SAVEAS	_T("另存文件")
#endif
#ifdef PREVIEWDLG_PUSHBOX_SAVE
#undef PREVIEWDLG_PUSHBOX_SAVE
#define PREVIEWDLG_PUSHBOX_SAVE		_T("保存文件")
#endif
#ifdef PREVIEWDLG_PUSHBOX_OPEN
#undef PREVIEWDLG_PUSHBOX_OPEN
#define PREVIEWDLG_PUSHBOX_OPEN		_T("打开文件")
#endif

#ifdef PROPERDLG_PUSHBOX_OK
#undef PROPERDLG_PUSHBOX_OK
#define PROPERDLG_PUSHBOX_OK		_T("确定")
#endif
#ifdef GRIDDLG_PUSHBOX_OK
#undef GRIDDLG_PUSHBOX_OK
#define GRIDDLG_PUSHBOX_OK			_T("确定")
#endif
#ifdef TABLEDLG_PUSHBOX_OK
#undef TABLEDLG_PUSHBOX_OK
#define TABLEDLG_PUSHBOX_OK			_T("确定")
#endif
#ifdef TEXTDLG_PUSHBOX_OK
#undef TEXTDLG_PUSHBOX_OK
#define TEXTDLG_PUSHBOX_OK			_T("确定")
#endif

#endif /*LANG_CN*/


#endif /*_XDCANSI_H*/
