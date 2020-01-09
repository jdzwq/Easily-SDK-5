/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@doc xdc document

	@module	xdcdef.h | defination interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/

#ifndef _XDCDEF_H
#define _XDCDEF_H

#if defined(_USRDLL)
#define XDC_API __declspec(dllexport)
#else
#define XDC_API 
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT    0x0410

#ifdef WINVER
#undef WINVER
#endif

#define WINVER			0x0500

#ifndef OEMRESOURCE 
#define OEMRESOURCE 
#endif
#include <windows.h>
#include <ole2.h>
#include <olectl.h>
#include <commctrl.h>

#include <xdl.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STRICT
#define STRICT
#endif

#define PIPER				0.01745
#define FPI					3.14159

#define ALPHA_HIGH			68
#define ALPHA_MEDU			128
#define ALPHA_LOW			198

#define DROPBOX_STYLE_FONTFACE		1
#define DROPBOX_STYLE_RGBCOLOR		2
#define DROPBOX_STYLE_ALIGMENT		3

#define DEF_CTRL_FONT		L"font-style:normal;font-size:9;font-family:宋体;font-weight:400;"
#define DEF_CTRL_FACE		L"text-color:10.10.10;text-align:near;line-align:center;text-wrap:line-barek;"
#define DEF_CTRL_PEN		L"stroke-style:solid;stroke-color:112.128.144;stroke-width:1;stroke-opacity:1;"
#define DEF_CTRL_BRUSH		L"fill-style:solid;fill-color:250.250.250;linear-color:198.198.198;fill-mode:cross;fill-opacity:1;"
#define DEF_MENU_BRUSH		L"fill-style:solid;fill-color:0.128.128;linear-color:198.198.198;fill-mode:cross;fill-opacity:0.5;"
#define DEF_GRAY_BRUSH		L"fill-style:solid;fill-color:198.198.198;fill-mode:horz;fill-opacity:1;"
#define DEF_DARK_BRUSH		L"fill-style:solid;fill-color:128.128.128;fill-mode:horz;fill-opacity:1;"
#define DEF_WHITE_BRUSH		L"fill-style:solid;fill-color:239.239.239;fill-mode:horz;fill-opacity:1;"

#define DEF_FOCUS_COLOR		L"192.192.192"
#define DEF_ALARM_COLOR		L"178.34.34"
#define DEF_ENABLE_COLOR	L"47.79.79"
#define DEF_DISABLE_COLOR	L"128.128.128"
#define DEF_ALPHA_COLOR		L"152.185.158"

#define DEF_FOCUS_FEED		5

#define AUTOSKIP_HORZ		1
#define AUTOSKIP_VERT		2

#define IDC_USERCTRL		2000

#define IDC_FIREGRID		(IDC_USERCTRL + 1)
#define IDC_FIREEDIT		(IDC_USERCTRL + 2)
#define IDC_FIRELIST		(IDC_USERCTRL + 3)
#define IDC_FIREDATE		(IDC_USERCTRL + 4)
#define IDC_FIREMEMO		(IDC_USERCTRL + 5)
#define IDC_FIREDROP		(IDC_USERCTRL + 6)
#define IDC_FIREMENU		(IDC_USERCTRL + 7)

#define IDC_GRIDEDIT		(IDC_USERCTRL + 10)
#define IDC_REPORTCTRL		(IDC_USERCTRL + 11)
#define IDC_SCREENCTRL		(IDC_USERCTRL + 12)


/*编辑件的通知结构*/
typedef struct tagNMHDR_FIRE{
	HWND hwndFrom;		/*编辑件窗体的句柄*/ 
    UINT idFrom;		/*编辑件窗体的ID*/
    UINT code;			/*编辑件的通知代码*/
	int editor;			/*编辑件的类型*/
}NMHDR_FIRE;

/*网格控件的通知结构*/
typedef struct tagNMHDR_GRID{
	HWND hwndFrom;		/*网格窗体句柄*/
    UINT idFrom;		/*网格窗体的ID*/
    UINT code;			/*网格的通知代码*/
	LINKPTR ptrGrid;	/*网格的连接件指针*/
	LINKPTR ptrRow;		/*网格的焦点行*/
	LINKPTR ptrCol;		/*网格的焦点列*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_GRID;

typedef enum{
	NC_GRIDLBCLK,
	NC_GRIDRBCLK,
	NC_GRIDDBCLK,

	NC_ROWCHANGING,
	NC_ROWCHANGED,
	NC_COLCHANGING,
	NC_COLCHANGED,
	NC_CELLSETFOCUS,
	NC_CELLKILLFOCUS,

	NC_ROWCHECK,
	NC_COLCHECK,
	
	NC_CELLEDITING,
	NC_CELLEDITED,
	NC_CELLUPDATE,
	NC_CELLCOMMIT,
	NC_CELLROLLBACK,

	NC_ROWCALCING,
	NC_GRIDCALCED,

	NC_INSERTROW,
	NC_DELETEROW,

	NC_ROWSIZING,
	NC_ROWSIZED,
	NC_ROWDRAG,
	NC_ROWDROP,
	NC_COLSIZING,
	NC_COLSIZED,
	NC_COLDRAG,
	NC_COLDROP,
	
	NC_GRID_NONE
}NMHDR_GRID_CODE;

/*表单控件的通知结构*/
typedef struct tagNMHDR_FORM{
	HWND hwndFrom;		/*表单窗体的句柄*/
    UINT idFrom;		/*表单窗体的ID*/
    UINT code;			/*表单的通知代码*/
	LINKPTR ptrForm;	/*表单文档*/
	LINKPTR ptrField;	/*表单的焦点字段*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_FORM;

typedef enum{
	NC_FORMLBCLK,
	NC_FORMRBCLK,
	NC_FORMDBCLK,

	NC_FIELDCHANGING,
	NC_FIELDCHANGED,

	NC_FIELDEDITING,
	NC_FIELDEDITED,
	NC_FIELDUPDATE,
	NC_FIELDCOMMIT,
	NC_FIELDROLLBACK,

	NC_FORMCALCING,
	NC_FORMCALCED,

	NC_FIELDDRAG,
	NC_FIELDDROP,
	NC_FIELDSIZING,
	NC_FIELDSIZED
}NMHDR_FORM_CODE;

/*属性控件的通知结构*/
typedef struct tagNMHDR_PROP{
	HWND hwndFrom;		/*属性窗体的句柄*/
    UINT idFrom;		/*属性窗体的ID*/
    UINT code;			/*属性的通知代码*/
	LINKPTR ptrProper;	/*属性文档*/
	LINKPTR ptrEntity;	/*属性的当前节*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_PROP;

typedef enum{
	NC_PROPERLBCLK,
	NC_PROPERRBCLK,
	NC_PROPERDBCLK,

	NC_ENTITYCHANGING,
	NC_ENTITYCHANGED,

	NC_ENTITYEDITING,
	NC_ENTITYEDITED,
	NC_ENTITYUPDATE,
	NC_ENTITYCOMMIT,
	NC_ENTITYROLLBACK,
	
	NC_PROPER_NONE
}NMHDR_PROPER_CODE;

/*树控件的通知结构*/
typedef struct tagNMHDR_TREE{
	HWND hwndFrom;		/*树窗体的句柄*/ 
    UINT idFrom;		/*树窗体的ID*/
    UINT code;			/*树控件的通知代码*/
	LINKPTR ptrTree;	/*树文档*/
	LINKPTR ptrItem;	/*树焦点条目*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_TREE;

typedef enum{
	NC_TREELBCLK,
	NC_TREERBCLK,
	NC_TREEDBCLK,

	NC_TREEITEMCHANGING,
	NC_TREEITEMCHANGED,

	NC_TREEITEMEDITING,
	NC_TREEITEMEDITED,
	NC_TREEITEMUPDATE,
	NC_TREEITEMCOMMIT,
	NC_TREEITEMROLLBACK,

	NC_TREEITEMCHECKED,

	NC_TREEITEMEXPAND,
	NC_TREEITEMCOLLAPSE,

	NC_TREEITEMDRAG,
	NC_TREEITEMDROP,

	NC_TREE_NONE
}NMHDR_TREE_CODE;

/*XML控件的通知结构*/
typedef struct tagNMHDR_XML{
	HWND hwndFrom;		/*XML窗体的句柄*/
	UINT idFrom;		/*XML窗体的ID*/
	UINT code;			/*XML控件的通知代码*/
	LINKPTR ptrXml;	/*XML文档*/
	LINKPTR ptrNode;	/*XML焦点条目*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_XML;

typedef enum{
	NC_XMLLBCLK,
	NC_XMLRBCLK,
	NC_XMLDBCLK,

	NC_XMLNODECHANGING,
	NC_XMLNODECHANGED,

	NC_XMLNODEEXPAND,
	NC_XMLNODECOLLAPSE,

	NC_XML_NONE
}NMHDR_XML_CODE;

/*列表控件的通知结构*/
typedef struct tagNMHDR_LIST{
	HWND hwndFrom;		/*列表窗体句柄*/
    UINT idFrom;		/*列表窗体ID*/
    UINT code;			/*列表的通知代码*/
	LINKPTR ptrList;	/*列表文档*/
	LINKPTR ptrItem;	/*列表的焦点条目*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_LIST;

typedef enum{
	NC_LISTLBCLK,
	NC_LISTRBCLK,
	NC_LISTDBCLK,

	NC_LISTITEMCHANGING,
	NC_LISTITEMCHANGED,

	NC_LISTITEMEDITING,
	NC_LISTITEMEDITED,
	NC_LISTITEMUPDATE,
	NC_LISTITEMCOMMIT,
	NC_LISTITEMROLLBACK,

	NC_LISTITEMCHECKED,

	NC_LISTITEMEXPAND,
	NC_LISTITEMCOLLAPSE,
	NC_LISTITEMDRAG,
	NC_LISTITEMDROP,

	NC_LIST_NONE
}NMHDR_LIST_CODE;

/*标签控件的通知结构*/
typedef struct tagNMHDR_LABEL{
	HWND hwndFrom;		/*标签窗体句柄*/
	UINT idFrom;		/*标签窗体ID*/
	UINT code;			/*标签的通知代码*/
	LINKPTR ptrLabel;	/*标签文档*/
	LINKPTR ptrItem;	/*标签的焦点条目*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_LABEL;

typedef enum{
	NC_LABELLBCLK,
	NC_LABELRBCLK,
	NC_LABELDBCLK,

	NC_LABELITEMCHANGING,
	NC_LABELITEMCHANGED,

	NC_LABELITEMCHECKED,

	NC_LABELITEMDRAG,
	NC_LABELITEMDROP,

	NC_LABEL_NONE
}NMHDR_LABEL_CODE;

/*工具控件的通知结构*/
typedef struct tagNMHDR_TOOL{
	HWND hwndFrom;		/*工具窗体句柄*/
    UINT idFrom;		/*工具窗体ID*/
    UINT code;			/*工具的通知代码*/
	LINKPTR ptrTool;	/*工具文档*/
	LINKPTR ptrItem;	/*工具的焦点按钮*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_TOOL;

typedef enum{
	NC_TOOLLBCLK,
	NC_TOOLRBCLK,
	NC_TOOLDBCLK,

	NC_TOOLITEMCHANGING,
	NC_TOOLITEMCHANGED,

	NC_TOOL_NONE
}NMHDR_TOOL_CODE;

/*状态控件的通知结构*/
typedef struct tagNMHDR_STATUS{
	HWND hwndFrom;		/*状态窗体句柄*/
	UINT idFrom;		/*状态窗体ID*/
	UINT code;			/*状态的通知代码*/
	LINKPTR ptrStatus;	/*状态文档*/
	LINKPTR ptrItem;	/*状态的焦点按钮*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_STATUS;

typedef enum{
	NC_STATUSLBCLK,
	NC_STATUSRBCLK,
	NC_STATUSDBCLK,

	NC_STATUSITEMCHANGING,
	NC_STATUSITEMCHANGED,

	NC_STATUS_NONE
}NMHDR_STATUS_CODE;

/*控件的通知结构*/
typedef struct tagNMHDR_STRING{
	HWND hwndFrom;		/*窗体句柄*/
	UINT idFrom;		/*窗体ID*/
	UINT code;			/*通知代码*/
	LINKPTR ptrStringTable;	/*文档*/
	LINKPTR ptrStringEntity;	/*焦点*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_STRING;

typedef enum{
	NC_STRINGTABLELBCLK,
	NC_STRINGTABLERBCLK,
	NC_STRINGTABLEDBCLK,

	NC_STRINGENTITYCHANGING,
	NC_STRINGENTITYCHANGED,

	NC_STRINGTABLE_NONE
}NMHDR_STRING_CODE;

/*标签控件的通知结构*/
typedef struct tagNMHDR_TTTLE{
	HWND hwndFrom;		/*标签窗体句柄*/
    UINT idFrom;		/*标签窗体ID*/
    UINT code;			/*标签的通知代码*/
	LINKPTR ptrTitle;	/*标签文档*/
	LINKPTR ptrItem;		/*标签的焦点按钮*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_TITLE;

typedef enum{
	NC_TITLELBCLK,
	NC_TITLERBCLK,
	NC_TITLEDBCLK,

	NC_TITLEITEMCHANGING,
	NC_TITLEITEMCHANGED,

	NC_TITLEITEMINSERT,
	NC_TITLEITEMDELETE,

	NC_TITLE_NONE
}NMHDR_TITLE_CODE;

/*GRAPH控件的通知结构*/
typedef struct tagNMHDR_GRAPH{
	HWND hwndFrom;		/*GRAPH窗体句柄*/
    UINT idFrom;		/*GRAPH窗体ID*/
    UINT code;			/*GRAPH通知代码*/
	LINKPTR ptrGraph;	/*GRAPH文档*/
	LINKPTR ptrXax;		/*GRAPH焦点x轴线*/
	LINKPTR ptrYax;		/*GRAPH焦点y轴线*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_GRAPH;

typedef enum{
	NC_GRAPHLBCLK,
	NC_GRAPHRBCLK,
	NC_GRAPHDBCLK,

	NC_XAXCHANGING,
	NC_XAXCHANGED,
	NC_YAXCHANGING,
	NC_YAXCHANGED,

	NC_SPOTSETFOCUS,
	NC_SPOTKILLFOCUS,

	NC_SPOTEDITING,
	NC_SPOTEDITED,
	NC_SPOTUPDATE,
	NC_SPOTCOMMIT,
	NC_SPOTROLLBACK,

	NC_DELETEXAX,
	NC_INSERTXAX,

	NC_XAXCHECK,
	NC_YAXCHECK,

	NC_XAXSIZING,
	NC_XAXSIZED,
	NC_YAXSIZING,
	NC_YAXSIZED,

	NC_XAXDRAG,
	NC_XAXDROP,
	NC_YAXDRAG,
	NC_YAXDROP,

	NC_GRAPH_NONE
}NMHDR_GRAPH_CODE;


/*报表控件的通知结构*/
typedef struct tagNMHDR_REPORT{
	HWND hwndFrom;		/*报表窗体句柄*/
    UINT idFrom;		/*报表窗体ID*/
    UINT code;			/*报表的通知代码*/
	LINKPTR ptrData;	/*报表的数据文档*/
	int retcode;		/*返回码*/
	void* data;			/*相关数据，由通知代码定义*/
}NMHDR_REPORT;

/*定义焦点的跳跃方向*/
typedef enum{NF_LEFT,NF_UP,NF_RIGHT,NF_DOWN,NF_END,NF_HOME,NF_PAGEUP,NF_PAGEDOWN}TabSkip;

//编辑件的消息代码
typedef enum{
	EC_COMMIT = 1,
	EC_ROLLBACK = 2,
	EC_CLEAN = 3,
	EC_COMMAND = 4,
	EC_COPY = 10,
	EC_CUT = 11,
	EC_PASTE = 12
}_CommandCode;

/*定义XDC控件的通知代码（code）*/
typedef enum{
	NC_FIREGRIDSHOW = 100,
	NC_FIREGRIDCOMMIT = 101,
	NC_FIREGRIDROLLBACK = 102,
	NC_FIREMENUSHOW = 103,
	NC_FIREMENUHIDE = 104
}_NotifyCode;

#define WM_EASYMSG_MIN		WM_USER + 10
#define WM_EASYMSG_MAX		WM_USER  + 100

#define WM_ATTACHDATA		WM_EASYMSG_MIN + 1
/*
含义：为XDC控件附加文档
WPARAM：无意义
LPARAM：文档的连接件指针
返回：无
*/

#define WM_DETACHDATA		WM_EASYMSG_MIN + 2
/*
含义：为XDC控件撤离文档
WPARAM：无意义
LPARAM：无意义
返回：文档的连接件指针
*/

#define WM_FETCHDATA		WM_EASYMSG_MIN + 3
/*
含义：取XDC控件的文档
WPARAM：无意义
LPARAM：无意义
返回：文档的连接件指针
*/

#define WM_SETFIREFONT		WM_EASYMSG_MIN + 4
/*
含义：为XDC控件设置编辑件的字体
WPARAM：无意义
LPARAM：xfont_t 结构指针
返回：无
*/

#define WM_REDRAW			WM_EASYMSG_MIN + 5
/*
含义：通知XDC控件重绘
WPARAM：TRUE则文档需要重计算，FALSE则稳定不需重计算
LPARAM：无意义
返回：无
*/

#define WM_TABSKIP			WM_EASYMSG_MIN + 6
/*
含义：通知焦点跳跃
WPARAM：整形跳跃的方向值
LPARAM：无意义
返回：无
*/

#define WM_KEYPREVIEW		WM_EASYMSG_MIN + 7
/*
含义：键预览
WPARAM：同WM_KEYDOWN
LPARAM：同WM_KEYDOWN
返回：非零值已处理，零值由原窗口处理
*/

#define WM_STRPREVIEW		WM_EASYMSG_MIN + 8
/*
含义：文本串预览
WPARAM：无意义
LPARAM：文本串
返回：无意义
*/



#define WM_RESETEDITOR		WM_EASYMSG_MIN + 10
/*
含义：通知编辑件提交编辑
WPARAM：非零值提交，零值取消
LPARAM：无意义
返回：无
*/

#define WM_GETEDITOR		WM_EASYMSG_MIN + 11
/*
含义：取编辑件的窗体句柄
WPARAM：无意义
LPARAM：无意义
返回：HWND类型的窗体句柄
*/

#define WM_GETEDITORTEXT	WM_EASYMSG_MIN + 12
/*
含义：取编辑件的文本
WPARAM：缓冲区指针
LPARAM：缓冲区长度，不包括终结符
返回：实际文本长度
*/

#define WM_SETEDITORTEXT	WM_EASYMSG_MIN + 13
/*
含义：设置编辑件的文本
WPARAM：文本长度
LPARAM：文本指针
返回：无
*/

#define WM_EDITORTEXTSIZE	WM_EASYMSG_MIN + 14
/*
含义：取编辑件文本长度
WPARAM：无意义
LPARAM：无意义
返回：文本长度
*/

#define WM_GETIMAGELIST		WM_EASYMSG_MIN + 15
/*
含义：获取XDC控件位图列表
WPARAM：无意义
LPARAM：无意义
返回：位图列表指针
*/

#define WM_SETIMAGELIST		WM_EASYMSG_MIN + 16
/*
含义：设置XDC控件位图列表
WPARAM：无意义
LPARAM：位图列表指针
返回：无
*/

#define WM_ACCEPTDATA		WM_EASYMSG_MIN + 17
/*
含义：通知XDC控件提交或取消编辑
WPARAM：TRUE提交编辑件数据，FALSE取消编辑件数据
LPARAM：无意义
返回：无
*/

#define WM_VERIFYDATA		WM_EASYMSG_MIN + 18
/*
含义：通知XDC控件校验数据
WPARAM：TRUE校验数据并且提示错误信息，FALSE只校验数据无信息提示
LPARAM：无意义
返回：无
*/

#define WM_SETLOCK			WM_EASYMSG_MIN + 19
/*
含义：锁定或解锁XDC控件
WPARAM：TRUE锁定控件使之不能被编辑，FALSE解锁控件使之可以编辑
LPARAM：无意义
返回：无
*/

#define WM_GETLOCK			WM_EASYMSG_MIN + 20
/*
含义：取XDC控件锁定状态
WPARAM：无意义
LPARAM：无意义
返回：TRUE控件锁定不能被编辑，FALSE控件锁可以编辑
*/

#define WM_SETAUTOSKIP		WM_EASYMSG_MIN + 21
/*
含义：设置XDC控件焦点自动跳跃状态
WPARAM：TRUE提交编辑后焦点自动跳跃，FALSE提交编辑后焦点驻留
LPARAM：无意义
返回：无
*/

#define WM_SETCURPAGE		WM_EASYMSG_MIN + 22
/*
含义：设置当前页
WPARAM：页索引，1开始
LPARAM：无意义
返回：无
*/
#define WM_GETCURPAGE		WM_EASYMSG_MIN + 23
/*
含义：取当前页
WPARAM：无意义
LPARAM：无意义
返回：页索引，1开始
*/
#define WM_GETMAXPAGE		WM_EASYMSG_MIN + 24
/*
含义：取页计数
WPARAM：无意义
LPARAM：无意义
返回：页计数
*/

#define WM_MOVENEXTPAGE		WM_EASYMSG_MIN + 25
/*
含义：移至下一页
WPARAM：无意义
LPARAM：无意义
返回：页计数
*/

#define WM_MOVEPREVPAGE		WM_EASYMSG_MIN + 26
/*
含义：移至上一页
WPARAM：无意义
LPARAM：无意义
返回：页计数
*/

#define WM_MOVEFIRSTPAGE	WM_EASYMSG_MIN + 27
/*
含义：移至首页
WPARAM：无意义
LPARAM：无意义
返回：页计数
*/

#define WM_MOVELASTPAGE		WM_EASYMSG_MIN + 28
/*
含义：移至尾页
WPARAM：无意义
LPARAM：无意义
返回：页计数
*/

#define WM_FINDTEXT			WM_EASYMSG_MIN + 29
/*
含义：查找匹配的字串
WPARAM：网格窗体时指明列名，空值时全局查找
LPARAM：待查找的字串指针
返回：
*/

#define WM_SETCURSEL		WM_EASYMSG_MIN + 32
/*
含义：设置XDC下拉选项
WPARAM：无意义
LPARAM：焦点指针
返回：无意义
*/

#define WM_GETCURSEL		WM_EASYMSG_MIN + 33
/*
含义：获取XDC下拉选项
WPARAM：无意义
LPARAM：无意义
返回：选项指针
*/

#define WM_SETOWNER		WM_EASYMSG_MIN + 34
/*
含义：设置宿主窗体
WPARAM：无意义
LPARAM：窗体句柄
返回：无意义
*/

#define WM_GETOWNER		WM_EASYMSG_MIN + 35
/*
含义：获取宿主窗体
WPARAM：无意义
LPARAM：无意义
返回：窗体句柄
*/

#define DROPLISTCLASS	L"DropList"
ATOM RegisterDropListClass(HINSTANCE hInstance);

#define DROPGRIDCLASS	L"DropGrid"
ATOM RegisterDropGridClass(HINSTANCE hInstance);

#define FIREEDITCLASS	L"FireEdit"
ATOM RegisterFireEditClass(HINSTANCE hInstance);

#define FIREMEMOCLASS	L"FireMemo"
ATOM RegisterFireMemoClass(HINSTANCE hInstance);

#define FIREDROPCLASS	L"FireDrop"
ATOM RegisterFireDropClass(HINSTANCE hInstance);

#define FIRELISTCLASS	L"FireList"
ATOM RegisterFireListClass(HINSTANCE hInstance);

#define FIREGRIDCLASS	L"FireGrid"
ATOM RegisterFireGridClass(HINSTANCE hInstance);

#define FIREDATECLASS	L"FireDate"
ATOM RegisterFireDateClass(HINSTANCE hInstance);

#define FIRESPINCLASS	L"FireSpin"
ATOM RegisterFireSpinClass(HINSTANCE hInstance);

#define FIREMENUCLASS	L"FireMenu"
ATOM RegisterFireMenuClass(HINSTANCE hInstance);

#define FILTERGRIDCLASS	L"FilterGrid"
ATOM RegisterFilterGridClass(HINSTANCE hInstance);
#define FT_FILTERGRID		WM_EASYMSG_MIN + 40
#define FT_GETSELECTROW		WM_EASYMSG_MIN + 41
#define FT_SETSELECTROW		WM_EASYMSG_MIN + 42
#define FT_SETFILTERCOL		WM_EASYMSG_MIN + 43
#define FT_GETFILTERCOL		WM_EASYMSG_MIN + 44
#define FT_GETFILTERGRID	WM_EASYMSG_MIN + 45
#define FT_SETFILTERGRID	WM_EASYMSG_MIN + 46

#define INPUTCTRLCLASS	L"InputCtrl"
ATOM RegisterInputCtrlClass(HINSTANCE hInstance);
#define IT_SETTITLE			WM_EASYMSG_MIN + 40
#define IT_GETTITLE			WM_EASYMSG_MIN + 41

#define PROPERCTRLCLASS	L"ProperCtrl"
ATOM RegisterProperCtrlClass(HINSTANCE hInstance);
#define PM_GETCURENT		WM_EASYMSG_MIN + 52
/*
含义：取属性控件当前节
WPARAM：无意义
LPARAM：无意义
返回：节连接件指针
*/
#define PM_SETCURENT		WM_EASYMSG_MIN + 53
/*
含义：设置属性控件当前节
WPARAM：无意义
LPARAM：节连接件指针
返回：无
*/
#define PM_REDRAWENT		WM_EASYMSG_MIN + 56
/*
含义：重绘属性控件当前节
WPARAM：无意义
LPARAM：节连接件指针
返回：无
*/
#define PM_REDRAWSEC		WM_EASYMSG_MIN + 57
/*
含义：重绘属性控件当前段
WPARAM：无意义
LPARAM：段连接件指针
返回：无
*/

#define TREECTRLCLASS		L"TreeCtrl"
ATOM RegisterTreeCtrlClass(HINSTANCE hInstance);
#define TR_GETCURITEM				WM_EASYMSG_MIN + 50
/*
含义：取树控件焦点条目
WPARAM：无意义
LPARAM：无意义
返回：条目连接件指针
*/
#define TR_SETCURITEM				WM_EASYMSG_MIN + 51
/*
含义：设置焦点条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/
#define TR_REDRAWITEM				WM_EASYMSG_MIN + 52
/*
含义：重绘树控件指定条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/

#define GRIDCTRLCLASS		L"GridCtrl"
ATOM RegisterGridCtrlClass(HINSTANCE hInstance);
#define GM_GETCURROW		WM_EASYMSG_MIN + 50
/*
含义：取网格控件焦点行
WPARAM：无意义
LPARAM：无意义
返回：行连接件指针
*/
#define GM_SETROWCOL		WM_EASYMSG_MIN + 51
/*
含义：设置网格控件焦点行和列
WPARAM：行连接件指针或位置标识符
LPARAM：列连接件指针或位置标识符
返回：无
*/
#define GM_GETCURCOL		WM_EASYMSG_MIN + 52
/*
含义：取网格控件焦点列
WPARAM：无意义
LPARAM：无意义
返回：列连接件指针
*/
#define GM_INSERTROW		WM_EASYMSG_MIN + 53
/*
含义：在指定位置后插入新行，锁定的网格不能添加新行
WPARAM：无意义
LPARAM：行连接件指针或位置标识符
返回：新行连接件指针
*/
#define GM_DELETEROW		WM_EASYMSG_MIN + 54
/*
含义：删除焦点行
WPARAM：无意义
LPARAM：行连接件指针
返回：无
*/
#define GM_REDRAWROW		WM_EASYMSG_MIN + 55
/*
含义：重绘行
WPARAM：TRUE该行将重计算，FALSE不进行重计算
LPARAM：行连接件指针
返回：无
*/
#define GM_REDRAWCOL		WM_EASYMSG_MIN + 56
/*
含义：重绘列
WPARAM：无意义
LPARAM：列连接件指针
返回：无
*/
#define GM_SETNAVCOL		WM_EASYMSG_MIN + 57
/*
含义：设置行切换列，即在该列提交编辑后焦点可以切换至下一行
WPARAM：TRUE指末行时，自动添加新行并切换至该行，FALSE不添加新行
LPARAM：列连接件指针
返回：无
*/
#define GM_ENABLECHECK		WM_EASYMSG_MIN + 60
/*
含义：设置是否可以选选择行
WPARAM：TRUE可选择行，FALSE不能选择行
LPARAM：无意义
返回：无
*/

#define GM_ENABLEDRAG		WM_EASYMSG_MIN + 61
/*
含义：设置是否可以拖放列
WPARAM：TRUE指可以拖放列，FALSE禁止拖放列
LPARAM：无意义
返回：无
*/
#define GM_COPYROW			WM_EASYMSG_MIN + 62
/*
含义：拷贝行数据
WPARAM：源网格连接件指针
LPARAM：源行连接件指针
返回：无
*/

#define FORMCTRLCLASS		L"FormCtrl"
ATOM RegisterFormCtrlClass(HINSTANCE hInstance);
#define FM_GETCURFIELD				WM_EASYMSG_MIN + 50
/*
含义：取表单控件焦点字段
WPARAM：无意义
LPARAM：无意义
返回：字段连接件指针
*/
#define FM_SETCURFIELD				WM_EASYMSG_MIN + 51
/*
含义：设置表单控件焦点字段
WPARAM：无意义
LPARAM：字段连接件指针
返回：无
*/
#define FM_REDRAWFIELD				WM_EASYMSG_MIN + 52
/*
含义：重绘表单的字段
WPARAM：TRUE该字段将重计算，FALSE不进行重计算
LPARAM：字段连接件指针
返回：无
*/


#define LISTCTRLCLASS		L"ListCtrl"
ATOM RegisterListCtrlClass(HINSTANCE hInstance);
#define LM_GETCURITEM		WM_EASYMSG_MIN + 50
/*
含义：取列表当前焦点条目
WPARAM：无意义
LPARAM：无意义
返回：条目连接件指针
*/
#define LM_SETCURITEM		WM_EASYMSG_MIN + 51
/*
含义：设置列表当前焦点条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/
#define LM_REDRAWITEM		WM_EASYMSG_MIN + 52
/*
含义：重绘列表条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/

#define LABELCTRLCLASS		L"LabelCtrl"
ATOM RegisterLabelCtrlClass(HINSTANCE hInstance);
#define LL_GETCURITEM		WM_EASYMSG_MIN + 50
/*
含义：取标签当前焦点条目
WPARAM：无意义
LPARAM：无意义
返回：条目连接件指针
*/
#define LL_SETCURITEM		WM_EASYMSG_MIN + 51
/*
含义：设置标签当前焦点条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/
#define LL_REDRAWITEM		WM_EASYMSG_MIN + 52
/*
含义：重绘标签条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/

#define TOOLCTRLCLASS		L"ToolCtrl"
ATOM RegisterToolCtrlClass(HINSTANCE hInstance);
#define TB_GETCURITEM		WM_EASYMSG_MIN + 50
/*
含义：取工具栏焦点按钮
WPARAM：无意义
LPARAM：无意义
返回：按钮连接件指针
*/
#define TB_SETCURITEM		WM_EASYMSG_MIN + 51
/*
含义：设置工具栏焦点按钮
WPARAM：无意义
LPARAM：按钮连接件指针
返回：无
*/
#define TB_REDRAWITEM		WM_EASYMSG_MIN + 52
/*
含义：重绘工具栏按钮
WPARAM：无意义
LPARAM：按钮连接件指针
返回：无
*/

#define STATUSCTRLCLASS		L"StatusCtrl"
ATOM RegisterStatusCtrlClass(HINSTANCE hInstance);
#define SB_GETCURITEM		WM_EASYMSG_MIN + 50
/*
含义：取状态栏焦点按钮
WPARAM：无意义
LPARAM：无意义
返回：按钮连接件指针
*/
#define SB_SETCURITEM		WM_EASYMSG_MIN + 51
/*
含义：设置状态栏焦点按钮
WPARAM：无意义
LPARAM：按钮连接件指针
返回：无
*/
#define SB_REDRAWITEM		WM_EASYMSG_MIN + 52
/*
含义：重绘状态栏按钮
WPARAM：无意义
LPARAM：按钮连接件指针
返回：无
*/

#define TITLECTRLCLASS		L"TitleCtrl"
ATOM RegisterTitleCtrlClass(HINSTANCE hInstance);
#define TT_GETCURITEM		WM_EASYMSG_MIN + 50
/*
含义：取标题栏焦点条目
WPARAM：无意义
LPARAM：无意义
返回：条目连接件指针
*/
#define TT_SETCURITEM		WM_EASYMSG_MIN + 51
/*
含义：设置标题栏焦点条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/
#define TT_INSERTITEM		WM_EASYMSG_MIN + 52
/*
含义：新增标题栏条目
WPARAM：用户参数
LPARAM：位置条目连接件指针
返回：新增的连接件指针，不成功返回零值
*/
#define TT_DELETEITEM		WM_EASYMSG_MIN + 53
/*
含义：删除标题栏条目
WPARAM：用户参数
LPARAM：条目连接件指针
返回：非零值成功
*/
#define TT_REDRAWITEM		WM_EASYMSG_MIN + 54
/*
含义：重绘标题栏条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/

#define GRAPHCTRLCLASS		L"GraphCtrl"
ATOM RegisterGraphCtrlClass(HINSTANCE hInstance);

#define GH_GETCURYAX		WM_EASYMSG_MIN + 50
/*
含义：取cube控件焦点Y轴
WPARAM：无意义
LPARAM：无意义
返回：连接件指针
*/
#define GH_GETCURXAX		WM_EASYMSG_MIN + 51
/*
含义：取cube控件焦点x轴
WPARAM：无意义
LPARAM：无意义
返回：连接件指针
*/
#define GH_SETXAXYAX		WM_EASYMSG_MIN + 52
/*
含义：取cube控件焦点x轴
WPARAM：无意义
LPARAM：无意义
返回：连接件指针
*/
#define GH_INSERTXAX		WM_EASYMSG_MIN + 53
/*
含义：插入X轴
WPARAM：无意义
LPARAM：前驱X轴连接件指针
返回：连接件指针
*/
#define GH_DELETEXAX		WM_EASYMSG_MIN + 54
/*
含义：删除X轴
WPARAM：无意义
LPARAM：指定德X轴连接件指针
返回：无
*/
#define GH_REDRAWXAX		WM_EASYMSG_MIN + 55
/*
含义：重绘X轴
WPARAM：无意义
LPARAM：X轴连接件指针
返回：无
*/

#define STRINGCTRLCLASS		L"StringCtrl"
ATOM RegisterStringCtrlClass(HINSTANCE hInstance);
#define ST_GETCURENTITY		WM_EASYMSG_MIN + 50
/*
含义：取标签当前焦点条目
WPARAM：无意义
LPARAM：无意义
返回：条目连接件指针
*/
#define ST_SETCURENTITY		WM_EASYMSG_MIN + 51
/*
含义：设置标签当前焦点条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/
#define ST_REDRAWENTITY		WM_EASYMSG_MIN + 52
/*
含义：重绘标签条目
WPARAM：无意义
LPARAM：条目连接件指针
返回：无
*/

#define XMLCTRLCLASS		L"XmlCtrl"
ATOM RegisterXmlCtrlClass(HINSTANCE hInstance);
#define XM_GETCURNODE		WM_EASYMSG_MIN + 50
/*
含义：取工具栏焦点按钮
WPARAM：无意义
LPARAM：无意义
返回：按钮连接件指针
*/
#define XM_SETCURNODE		WM_EASYMSG_MIN + 51
/*
含义：设置工具栏焦点按钮
WPARAM：无意义
LPARAM：按钮连接件指针
返回：无
*/
#define XM_REDRAWNODE		WM_EASYMSG_MIN + 52
/*
含义：重绘工具栏按钮
WPARAM：无意义
LPARAM：按钮连接件指针
返回：无
*/

#define SPLITCTRLCLASS		L"SplitCtrlClass"
ATOM RegisterSplitCtrlClass(HINSTANCE hInstance);

#define REPORTCTRLCLASS		L"ReportCtrl"
ATOM RegisterReportCtrlClass(HINSTANCE hInstance);

#define RP_MOVEFIRSTPAGE	WM_EASYMSG_MIN + 60
#define RP_MOVELASTPAGE		WM_EASYMSG_MIN + 61
#define RP_MOVENEXTPAGE		WM_EASYMSG_MIN + 62
#define RP_MOVEPREVPAGE		WM_EASYMSG_MIN + 63
#define RP_SETPAINTSCALE	WM_EASYMSG_MIN + 64
#define RP_GETPAINTSCALE	WM_EASYMSG_MIN + 65
#define RP_RESETPRINTER		WM_EASYMSG_MIN + 66
#define RP_GETCURPAGE		WM_EASYMSG_MIN + 67
#define RP_SETCURPAGE		WM_EASYMSG_MIN + 68
#define RP_GETMAXPAGE		WM_EASYMSG_MIN + 69
#define RP_SETSHOWRULER		WM_EASYMSG_MIN + 70
#define RP_GETPAPERWIDTH	WM_EASYMSG_MIN + 71
#define RP_GETPAPERHEIGHT	WM_EASYMSG_MIN + 72

#define GUID_PAGES			L"#pages"
#define GUID_PAGE			L"#page"


#ifdef __cplusplus
}
#endif

#endif