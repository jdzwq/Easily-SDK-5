/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@doc xdc document

	@module	xdcdef.h | defination interface file

	@devnote ����Ȩ 2003.01 - 2007.12	v3.0
	@devnote ����Ȩ 2008.01 - 2009.12	v3.5
	@devnote ����Ȩ 2012.01 -			v4.0
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

#define DEF_CTRL_FONT		L"font-style:normal;font-size:9;font-family:����;font-weight:400;"
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


/*�༭����֪ͨ�ṹ*/
typedef struct tagNMHDR_FIRE{
	HWND hwndFrom;		/*�༭������ľ��*/ 
    UINT idFrom;		/*�༭�������ID*/
    UINT code;			/*�༭����֪ͨ����*/
	int editor;			/*�༭��������*/
}NMHDR_FIRE;

/*����ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_GRID{
	HWND hwndFrom;		/*��������*/
    UINT idFrom;		/*�������ID*/
    UINT code;			/*�����֪ͨ����*/
	LINKPTR ptrGrid;	/*��������Ӽ�ָ��*/
	LINKPTR ptrRow;		/*����Ľ�����*/
	LINKPTR ptrCol;		/*����Ľ�����*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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

/*���ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_FORM{
	HWND hwndFrom;		/*������ľ��*/
    UINT idFrom;		/*�������ID*/
    UINT code;			/*����֪ͨ����*/
	LINKPTR ptrForm;	/*���ĵ�*/
	LINKPTR ptrField;	/*���Ľ����ֶ�*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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

/*���Կؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_PROP{
	HWND hwndFrom;		/*���Դ���ľ��*/
    UINT idFrom;		/*���Դ����ID*/
    UINT code;			/*���Ե�֪ͨ����*/
	LINKPTR ptrProper;	/*�����ĵ�*/
	LINKPTR ptrEntity;	/*���Եĵ�ǰ��*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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

/*���ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_TREE{
	HWND hwndFrom;		/*������ľ��*/ 
    UINT idFrom;		/*�������ID*/
    UINT code;			/*���ؼ���֪ͨ����*/
	LINKPTR ptrTree;	/*���ĵ�*/
	LINKPTR ptrItem;	/*��������Ŀ*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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

/*XML�ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_XML{
	HWND hwndFrom;		/*XML����ľ��*/
	UINT idFrom;		/*XML�����ID*/
	UINT code;			/*XML�ؼ���֪ͨ����*/
	LINKPTR ptrXml;	/*XML�ĵ�*/
	LINKPTR ptrNode;	/*XML������Ŀ*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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

/*�б�ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_LIST{
	HWND hwndFrom;		/*�б�����*/
    UINT idFrom;		/*�б���ID*/
    UINT code;			/*�б��֪ͨ����*/
	LINKPTR ptrList;	/*�б��ĵ�*/
	LINKPTR ptrItem;	/*�б�Ľ�����Ŀ*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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

/*��ǩ�ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_LABEL{
	HWND hwndFrom;		/*��ǩ������*/
	UINT idFrom;		/*��ǩ����ID*/
	UINT code;			/*��ǩ��֪ͨ����*/
	LINKPTR ptrLabel;	/*��ǩ�ĵ�*/
	LINKPTR ptrItem;	/*��ǩ�Ľ�����Ŀ*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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

/*���߿ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_TOOL{
	HWND hwndFrom;		/*���ߴ�����*/
    UINT idFrom;		/*���ߴ���ID*/
    UINT code;			/*���ߵ�֪ͨ����*/
	LINKPTR ptrTool;	/*�����ĵ�*/
	LINKPTR ptrItem;	/*���ߵĽ��㰴ť*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
}NMHDR_TOOL;

typedef enum{
	NC_TOOLLBCLK,
	NC_TOOLRBCLK,
	NC_TOOLDBCLK,

	NC_TOOLITEMCHANGING,
	NC_TOOLITEMCHANGED,

	NC_TOOL_NONE
}NMHDR_TOOL_CODE;

/*״̬�ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_STATUS{
	HWND hwndFrom;		/*״̬������*/
	UINT idFrom;		/*״̬����ID*/
	UINT code;			/*״̬��֪ͨ����*/
	LINKPTR ptrStatus;	/*״̬�ĵ�*/
	LINKPTR ptrItem;	/*״̬�Ľ��㰴ť*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
}NMHDR_STATUS;

typedef enum{
	NC_STATUSLBCLK,
	NC_STATUSRBCLK,
	NC_STATUSDBCLK,

	NC_STATUSITEMCHANGING,
	NC_STATUSITEMCHANGED,

	NC_STATUS_NONE
}NMHDR_STATUS_CODE;

/*�ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_STRING{
	HWND hwndFrom;		/*������*/
	UINT idFrom;		/*����ID*/
	UINT code;			/*֪ͨ����*/
	LINKPTR ptrStringTable;	/*�ĵ�*/
	LINKPTR ptrStringEntity;	/*����*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
}NMHDR_STRING;

typedef enum{
	NC_STRINGTABLELBCLK,
	NC_STRINGTABLERBCLK,
	NC_STRINGTABLEDBCLK,

	NC_STRINGENTITYCHANGING,
	NC_STRINGENTITYCHANGED,

	NC_STRINGTABLE_NONE
}NMHDR_STRING_CODE;

/*��ǩ�ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_TTTLE{
	HWND hwndFrom;		/*��ǩ������*/
    UINT idFrom;		/*��ǩ����ID*/
    UINT code;			/*��ǩ��֪ͨ����*/
	LINKPTR ptrTitle;	/*��ǩ�ĵ�*/
	LINKPTR ptrItem;		/*��ǩ�Ľ��㰴ť*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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

/*GRAPH�ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_GRAPH{
	HWND hwndFrom;		/*GRAPH������*/
    UINT idFrom;		/*GRAPH����ID*/
    UINT code;			/*GRAPH֪ͨ����*/
	LINKPTR ptrGraph;	/*GRAPH�ĵ�*/
	LINKPTR ptrXax;		/*GRAPH����x����*/
	LINKPTR ptrYax;		/*GRAPH����y����*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
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


/*����ؼ���֪ͨ�ṹ*/
typedef struct tagNMHDR_REPORT{
	HWND hwndFrom;		/*��������*/
    UINT idFrom;		/*������ID*/
    UINT code;			/*�����֪ͨ����*/
	LINKPTR ptrData;	/*����������ĵ�*/
	int retcode;		/*������*/
	void* data;			/*������ݣ���֪ͨ���붨��*/
}NMHDR_REPORT;

/*���役�����Ծ����*/
typedef enum{NF_LEFT,NF_UP,NF_RIGHT,NF_DOWN,NF_END,NF_HOME,NF_PAGEUP,NF_PAGEDOWN}TabSkip;

//�༭������Ϣ����
typedef enum{
	EC_COMMIT = 1,
	EC_ROLLBACK = 2,
	EC_CLEAN = 3,
	EC_COMMAND = 4,
	EC_COPY = 10,
	EC_CUT = 11,
	EC_PASTE = 12
}_CommandCode;

/*����XDC�ؼ���֪ͨ���루code��*/
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
���壺ΪXDC�ؼ������ĵ�
WPARAM��������
LPARAM���ĵ������Ӽ�ָ��
���أ���
*/

#define WM_DETACHDATA		WM_EASYMSG_MIN + 2
/*
���壺ΪXDC�ؼ������ĵ�
WPARAM��������
LPARAM��������
���أ��ĵ������Ӽ�ָ��
*/

#define WM_FETCHDATA		WM_EASYMSG_MIN + 3
/*
���壺ȡXDC�ؼ����ĵ�
WPARAM��������
LPARAM��������
���أ��ĵ������Ӽ�ָ��
*/

#define WM_SETFIREFONT		WM_EASYMSG_MIN + 4
/*
���壺ΪXDC�ؼ����ñ༭��������
WPARAM��������
LPARAM��xfont_t �ṹָ��
���أ���
*/

#define WM_REDRAW			WM_EASYMSG_MIN + 5
/*
���壺֪ͨXDC�ؼ��ػ�
WPARAM��TRUE���ĵ���Ҫ�ؼ��㣬FALSE���ȶ������ؼ���
LPARAM��������
���أ���
*/

#define WM_TABSKIP			WM_EASYMSG_MIN + 6
/*
���壺֪ͨ������Ծ
WPARAM��������Ծ�ķ���ֵ
LPARAM��������
���أ���
*/

#define WM_KEYPREVIEW		WM_EASYMSG_MIN + 7
/*
���壺��Ԥ��
WPARAM��ͬWM_KEYDOWN
LPARAM��ͬWM_KEYDOWN
���أ�����ֵ�Ѵ�����ֵ��ԭ���ڴ���
*/

#define WM_STRPREVIEW		WM_EASYMSG_MIN + 8
/*
���壺�ı���Ԥ��
WPARAM��������
LPARAM���ı���
���أ�������
*/



#define WM_RESETEDITOR		WM_EASYMSG_MIN + 10
/*
���壺֪ͨ�༭���ύ�༭
WPARAM������ֵ�ύ����ֵȡ��
LPARAM��������
���أ���
*/

#define WM_GETEDITOR		WM_EASYMSG_MIN + 11
/*
���壺ȡ�༭���Ĵ�����
WPARAM��������
LPARAM��������
���أ�HWND���͵Ĵ�����
*/

#define WM_GETEDITORTEXT	WM_EASYMSG_MIN + 12
/*
���壺ȡ�༭�����ı�
WPARAM��������ָ��
LPARAM�����������ȣ��������ս��
���أ�ʵ���ı�����
*/

#define WM_SETEDITORTEXT	WM_EASYMSG_MIN + 13
/*
���壺���ñ༭�����ı�
WPARAM���ı�����
LPARAM���ı�ָ��
���أ���
*/

#define WM_EDITORTEXTSIZE	WM_EASYMSG_MIN + 14
/*
���壺ȡ�༭���ı�����
WPARAM��������
LPARAM��������
���أ��ı�����
*/

#define WM_GETIMAGELIST		WM_EASYMSG_MIN + 15
/*
���壺��ȡXDC�ؼ�λͼ�б�
WPARAM��������
LPARAM��������
���أ�λͼ�б�ָ��
*/

#define WM_SETIMAGELIST		WM_EASYMSG_MIN + 16
/*
���壺����XDC�ؼ�λͼ�б�
WPARAM��������
LPARAM��λͼ�б�ָ��
���أ���
*/

#define WM_ACCEPTDATA		WM_EASYMSG_MIN + 17
/*
���壺֪ͨXDC�ؼ��ύ��ȡ���༭
WPARAM��TRUE�ύ�༭�����ݣ�FALSEȡ���༭������
LPARAM��������
���أ���
*/

#define WM_VERIFYDATA		WM_EASYMSG_MIN + 18
/*
���壺֪ͨXDC�ؼ�У������
WPARAM��TRUEУ�����ݲ�����ʾ������Ϣ��FALSEֻУ����������Ϣ��ʾ
LPARAM��������
���أ���
*/

#define WM_SETLOCK			WM_EASYMSG_MIN + 19
/*
���壺���������XDC�ؼ�
WPARAM��TRUE�����ؼ�ʹ֮���ܱ��༭��FALSE�����ؼ�ʹ֮���Ա༭
LPARAM��������
���أ���
*/

#define WM_GETLOCK			WM_EASYMSG_MIN + 20
/*
���壺ȡXDC�ؼ�����״̬
WPARAM��������
LPARAM��������
���أ�TRUE�ؼ��������ܱ��༭��FALSE�ؼ������Ա༭
*/

#define WM_SETAUTOSKIP		WM_EASYMSG_MIN + 21
/*
���壺����XDC�ؼ������Զ���Ծ״̬
WPARAM��TRUE�ύ�༭�󽹵��Զ���Ծ��FALSE�ύ�༭�󽹵�פ��
LPARAM��������
���أ���
*/

#define WM_SETCURPAGE		WM_EASYMSG_MIN + 22
/*
���壺���õ�ǰҳ
WPARAM��ҳ������1��ʼ
LPARAM��������
���أ���
*/
#define WM_GETCURPAGE		WM_EASYMSG_MIN + 23
/*
���壺ȡ��ǰҳ
WPARAM��������
LPARAM��������
���أ�ҳ������1��ʼ
*/
#define WM_GETMAXPAGE		WM_EASYMSG_MIN + 24
/*
���壺ȡҳ����
WPARAM��������
LPARAM��������
���أ�ҳ����
*/

#define WM_MOVENEXTPAGE		WM_EASYMSG_MIN + 25
/*
���壺������һҳ
WPARAM��������
LPARAM��������
���أ�ҳ����
*/

#define WM_MOVEPREVPAGE		WM_EASYMSG_MIN + 26
/*
���壺������һҳ
WPARAM��������
LPARAM��������
���أ�ҳ����
*/

#define WM_MOVEFIRSTPAGE	WM_EASYMSG_MIN + 27
/*
���壺������ҳ
WPARAM��������
LPARAM��������
���أ�ҳ����
*/

#define WM_MOVELASTPAGE		WM_EASYMSG_MIN + 28
/*
���壺����βҳ
WPARAM��������
LPARAM��������
���أ�ҳ����
*/

#define WM_FINDTEXT			WM_EASYMSG_MIN + 29
/*
���壺����ƥ����ִ�
WPARAM��������ʱָ����������ֵʱȫ�ֲ���
LPARAM�������ҵ��ִ�ָ��
���أ�
*/

#define WM_SETCURSEL		WM_EASYMSG_MIN + 32
/*
���壺����XDC����ѡ��
WPARAM��������
LPARAM������ָ��
���أ�������
*/

#define WM_GETCURSEL		WM_EASYMSG_MIN + 33
/*
���壺��ȡXDC����ѡ��
WPARAM��������
LPARAM��������
���أ�ѡ��ָ��
*/

#define WM_SETOWNER		WM_EASYMSG_MIN + 34
/*
���壺������������
WPARAM��������
LPARAM��������
���أ�������
*/

#define WM_GETOWNER		WM_EASYMSG_MIN + 35
/*
���壺��ȡ��������
WPARAM��������
LPARAM��������
���أ�������
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
���壺ȡ���Կؼ���ǰ��
WPARAM��������
LPARAM��������
���أ������Ӽ�ָ��
*/
#define PM_SETCURENT		WM_EASYMSG_MIN + 53
/*
���壺�������Կؼ���ǰ��
WPARAM��������
LPARAM�������Ӽ�ָ��
���أ���
*/
#define PM_REDRAWENT		WM_EASYMSG_MIN + 56
/*
���壺�ػ����Կؼ���ǰ��
WPARAM��������
LPARAM�������Ӽ�ָ��
���أ���
*/
#define PM_REDRAWSEC		WM_EASYMSG_MIN + 57
/*
���壺�ػ����Կؼ���ǰ��
WPARAM��������
LPARAM�������Ӽ�ָ��
���أ���
*/

#define TREECTRLCLASS		L"TreeCtrl"
ATOM RegisterTreeCtrlClass(HINSTANCE hInstance);
#define TR_GETCURITEM				WM_EASYMSG_MIN + 50
/*
���壺ȡ���ؼ�������Ŀ
WPARAM��������
LPARAM��������
���أ���Ŀ���Ӽ�ָ��
*/
#define TR_SETCURITEM				WM_EASYMSG_MIN + 51
/*
���壺���ý�����Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/
#define TR_REDRAWITEM				WM_EASYMSG_MIN + 52
/*
���壺�ػ����ؼ�ָ����Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/

#define GRIDCTRLCLASS		L"GridCtrl"
ATOM RegisterGridCtrlClass(HINSTANCE hInstance);
#define GM_GETCURROW		WM_EASYMSG_MIN + 50
/*
���壺ȡ����ؼ�������
WPARAM��������
LPARAM��������
���أ������Ӽ�ָ��
*/
#define GM_SETROWCOL		WM_EASYMSG_MIN + 51
/*
���壺��������ؼ������к���
WPARAM�������Ӽ�ָ���λ�ñ�ʶ��
LPARAM�������Ӽ�ָ���λ�ñ�ʶ��
���أ���
*/
#define GM_GETCURCOL		WM_EASYMSG_MIN + 52
/*
���壺ȡ����ؼ�������
WPARAM��������
LPARAM��������
���أ������Ӽ�ָ��
*/
#define GM_INSERTROW		WM_EASYMSG_MIN + 53
/*
���壺��ָ��λ�ú�������У��������������������
WPARAM��������
LPARAM�������Ӽ�ָ���λ�ñ�ʶ��
���أ��������Ӽ�ָ��
*/
#define GM_DELETEROW		WM_EASYMSG_MIN + 54
/*
���壺ɾ��������
WPARAM��������
LPARAM�������Ӽ�ָ��
���أ���
*/
#define GM_REDRAWROW		WM_EASYMSG_MIN + 55
/*
���壺�ػ���
WPARAM��TRUE���н��ؼ��㣬FALSE�������ؼ���
LPARAM�������Ӽ�ָ��
���أ���
*/
#define GM_REDRAWCOL		WM_EASYMSG_MIN + 56
/*
���壺�ػ���
WPARAM��������
LPARAM�������Ӽ�ָ��
���أ���
*/
#define GM_SETNAVCOL		WM_EASYMSG_MIN + 57
/*
���壺�������л��У����ڸ����ύ�༭�󽹵�����л�����һ��
WPARAM��TRUEָĩ��ʱ���Զ�������в��л������У�FALSE���������
LPARAM�������Ӽ�ָ��
���أ���
*/
#define GM_ENABLECHECK		WM_EASYMSG_MIN + 60
/*
���壺�����Ƿ����ѡѡ����
WPARAM��TRUE��ѡ���У�FALSE����ѡ����
LPARAM��������
���أ���
*/

#define GM_ENABLEDRAG		WM_EASYMSG_MIN + 61
/*
���壺�����Ƿ�����Ϸ���
WPARAM��TRUEָ�����Ϸ��У�FALSE��ֹ�Ϸ���
LPARAM��������
���أ���
*/
#define GM_COPYROW			WM_EASYMSG_MIN + 62
/*
���壺����������
WPARAM��Դ�������Ӽ�ָ��
LPARAM��Դ�����Ӽ�ָ��
���أ���
*/

#define FORMCTRLCLASS		L"FormCtrl"
ATOM RegisterFormCtrlClass(HINSTANCE hInstance);
#define FM_GETCURFIELD				WM_EASYMSG_MIN + 50
/*
���壺ȡ���ؼ������ֶ�
WPARAM��������
LPARAM��������
���أ��ֶ����Ӽ�ָ��
*/
#define FM_SETCURFIELD				WM_EASYMSG_MIN + 51
/*
���壺���ñ��ؼ������ֶ�
WPARAM��������
LPARAM���ֶ����Ӽ�ָ��
���أ���
*/
#define FM_REDRAWFIELD				WM_EASYMSG_MIN + 52
/*
���壺�ػ�����ֶ�
WPARAM��TRUE���ֶν��ؼ��㣬FALSE�������ؼ���
LPARAM���ֶ����Ӽ�ָ��
���أ���
*/


#define LISTCTRLCLASS		L"ListCtrl"
ATOM RegisterListCtrlClass(HINSTANCE hInstance);
#define LM_GETCURITEM		WM_EASYMSG_MIN + 50
/*
���壺ȡ�б�ǰ������Ŀ
WPARAM��������
LPARAM��������
���أ���Ŀ���Ӽ�ָ��
*/
#define LM_SETCURITEM		WM_EASYMSG_MIN + 51
/*
���壺�����б�ǰ������Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/
#define LM_REDRAWITEM		WM_EASYMSG_MIN + 52
/*
���壺�ػ��б���Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/

#define LABELCTRLCLASS		L"LabelCtrl"
ATOM RegisterLabelCtrlClass(HINSTANCE hInstance);
#define LL_GETCURITEM		WM_EASYMSG_MIN + 50
/*
���壺ȡ��ǩ��ǰ������Ŀ
WPARAM��������
LPARAM��������
���أ���Ŀ���Ӽ�ָ��
*/
#define LL_SETCURITEM		WM_EASYMSG_MIN + 51
/*
���壺���ñ�ǩ��ǰ������Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/
#define LL_REDRAWITEM		WM_EASYMSG_MIN + 52
/*
���壺�ػ��ǩ��Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/

#define TOOLCTRLCLASS		L"ToolCtrl"
ATOM RegisterToolCtrlClass(HINSTANCE hInstance);
#define TB_GETCURITEM		WM_EASYMSG_MIN + 50
/*
���壺ȡ���������㰴ť
WPARAM��������
LPARAM��������
���أ���ť���Ӽ�ָ��
*/
#define TB_SETCURITEM		WM_EASYMSG_MIN + 51
/*
���壺���ù��������㰴ť
WPARAM��������
LPARAM����ť���Ӽ�ָ��
���أ���
*/
#define TB_REDRAWITEM		WM_EASYMSG_MIN + 52
/*
���壺�ػ湤������ť
WPARAM��������
LPARAM����ť���Ӽ�ָ��
���أ���
*/

#define STATUSCTRLCLASS		L"StatusCtrl"
ATOM RegisterStatusCtrlClass(HINSTANCE hInstance);
#define SB_GETCURITEM		WM_EASYMSG_MIN + 50
/*
���壺ȡ״̬�����㰴ť
WPARAM��������
LPARAM��������
���أ���ť���Ӽ�ָ��
*/
#define SB_SETCURITEM		WM_EASYMSG_MIN + 51
/*
���壺����״̬�����㰴ť
WPARAM��������
LPARAM����ť���Ӽ�ָ��
���أ���
*/
#define SB_REDRAWITEM		WM_EASYMSG_MIN + 52
/*
���壺�ػ�״̬����ť
WPARAM��������
LPARAM����ť���Ӽ�ָ��
���أ���
*/

#define TITLECTRLCLASS		L"TitleCtrl"
ATOM RegisterTitleCtrlClass(HINSTANCE hInstance);
#define TT_GETCURITEM		WM_EASYMSG_MIN + 50
/*
���壺ȡ������������Ŀ
WPARAM��������
LPARAM��������
���أ���Ŀ���Ӽ�ָ��
*/
#define TT_SETCURITEM		WM_EASYMSG_MIN + 51
/*
���壺���ñ�����������Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/
#define TT_INSERTITEM		WM_EASYMSG_MIN + 52
/*
���壺������������Ŀ
WPARAM���û�����
LPARAM��λ����Ŀ���Ӽ�ָ��
���أ����������Ӽ�ָ�룬���ɹ�������ֵ
*/
#define TT_DELETEITEM		WM_EASYMSG_MIN + 53
/*
���壺ɾ����������Ŀ
WPARAM���û�����
LPARAM����Ŀ���Ӽ�ָ��
���أ�����ֵ�ɹ�
*/
#define TT_REDRAWITEM		WM_EASYMSG_MIN + 54
/*
���壺�ػ��������Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/

#define GRAPHCTRLCLASS		L"GraphCtrl"
ATOM RegisterGraphCtrlClass(HINSTANCE hInstance);

#define GH_GETCURYAX		WM_EASYMSG_MIN + 50
/*
���壺ȡcube�ؼ�����Y��
WPARAM��������
LPARAM��������
���أ����Ӽ�ָ��
*/
#define GH_GETCURXAX		WM_EASYMSG_MIN + 51
/*
���壺ȡcube�ؼ�����x��
WPARAM��������
LPARAM��������
���أ����Ӽ�ָ��
*/
#define GH_SETXAXYAX		WM_EASYMSG_MIN + 52
/*
���壺ȡcube�ؼ�����x��
WPARAM��������
LPARAM��������
���أ����Ӽ�ָ��
*/
#define GH_INSERTXAX		WM_EASYMSG_MIN + 53
/*
���壺����X��
WPARAM��������
LPARAM��ǰ��X�����Ӽ�ָ��
���أ����Ӽ�ָ��
*/
#define GH_DELETEXAX		WM_EASYMSG_MIN + 54
/*
���壺ɾ��X��
WPARAM��������
LPARAM��ָ����X�����Ӽ�ָ��
���أ���
*/
#define GH_REDRAWXAX		WM_EASYMSG_MIN + 55
/*
���壺�ػ�X��
WPARAM��������
LPARAM��X�����Ӽ�ָ��
���أ���
*/

#define STRINGCTRLCLASS		L"StringCtrl"
ATOM RegisterStringCtrlClass(HINSTANCE hInstance);
#define ST_GETCURENTITY		WM_EASYMSG_MIN + 50
/*
���壺ȡ��ǩ��ǰ������Ŀ
WPARAM��������
LPARAM��������
���أ���Ŀ���Ӽ�ָ��
*/
#define ST_SETCURENTITY		WM_EASYMSG_MIN + 51
/*
���壺���ñ�ǩ��ǰ������Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/
#define ST_REDRAWENTITY		WM_EASYMSG_MIN + 52
/*
���壺�ػ��ǩ��Ŀ
WPARAM��������
LPARAM����Ŀ���Ӽ�ָ��
���أ���
*/

#define XMLCTRLCLASS		L"XmlCtrl"
ATOM RegisterXmlCtrlClass(HINSTANCE hInstance);
#define XM_GETCURNODE		WM_EASYMSG_MIN + 50
/*
���壺ȡ���������㰴ť
WPARAM��������
LPARAM��������
���أ���ť���Ӽ�ָ��
*/
#define XM_SETCURNODE		WM_EASYMSG_MIN + 51
/*
���壺���ù��������㰴ť
WPARAM��������
LPARAM����ť���Ӽ�ָ��
���أ���
*/
#define XM_REDRAWNODE		WM_EASYMSG_MIN + 52
/*
���壺�ػ湤������ť
WPARAM��������
LPARAM����ť���Ӽ�ָ��
���أ���
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