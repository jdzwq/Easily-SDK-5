/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc common control document

	@module	xdcctrl.h | control interface file

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

#ifndef _XDCCTRL_H
#define _XDCCTRL_H

#include "xdcdef.h"

typedef struct _EDITDELTA{
	union{
		link_t_ptr data;
		link_t_ptr menu;
	};
	union{
		link_t_ptr item;
		const tchar_t* text;
	};
}EDITDELTA;


#ifdef	__cplusplus
extern "C" {
#endif

/*************************************svg control*******************************************************/
typedef struct _NOTICE_SVG{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr svg;
	void* data;

	int ret;
}NOTICE_SVG;

typedef enum{
	NC_SVGLBCLK,
	NC_SVGRBCLK,
	NC_SVGDBCLK,
}SVG_NOTICE_CODE;

XDC_API res_win_t svgctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	svgctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr svgctrl_detach(res_win_t widget);

XDC_API link_t_ptr svgctrl_fetch(res_win_t widget);

XDC_API void	svgctrl_redraw(res_win_t widget);

/**********************************************form control******************************************************************/

typedef struct _NOTICE_FORM{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr form;
	link_t_ptr field;
	void* data;

	xpoint_t pt;

	int ret;
}NOTICE_FORM;

typedef enum{

	NC_FORMLBCLK,
	NC_FORMRBCLK,
	NC_FORMDBCLK,

	NC_FIELDHOVER,

	NC_FIELDCHANGING,
	NC_FIELDCHANGED,
	NC_FIELDSELECTED,

	NC_FIELDEDITING,
	NC_FIELDUPDATE,
	NC_FIELDCOMMIT,
	NC_FIELDROLLBACK,

	NC_FIELDCALCED,
	NC_FORMCALCED,

	NC_FIELDDRAG,
	NC_FIELDDROP,
	NC_FIELDSIZING,
	NC_FIELDSIZED,

	NC_FIELDGRID,
	NC_FIELDGRAPH,
	NC_FIELDFORM,
	NC_FIELDPHOTO,
	NC_FIELDIMAGES,
	NC_FIELDTAG,
	NC_FIELDRICH,
	NC_FIELDMEMO
}FORM_NOTICE_CODE;

XDC_API res_win_t formctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void formctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API res_win_t formctrl_get_editor(res_win_t widget);

XDC_API link_t_ptr formctrl_detach(res_win_t widget);

XDC_API link_t_ptr formctrl_fetch(res_win_t widget);

XDC_API bool_t formctrl_verify(res_win_t widget, bool_t bAlarm);

XDC_API void formctrl_accept(res_win_t widget, bool_t bAccept);

XDC_API bool_t formctrl_is_update(res_win_t widget);

XDC_API void formctrl_redraw_field(res_win_t widget, link_t_ptr flk, bool_t bCalc);

XDC_API void formctrl_redraw(res_win_t widget, bool_t bCalc);

XDC_API bool_t formctrl_get_dirty(res_win_t widget);

XDC_API void formctrl_set_dirty(res_win_t widget, bool_t bDirty);

XDC_API bool_t formctrl_set_focus_field(res_win_t widget, link_t_ptr flk);

XDC_API link_t_ptr formctrl_get_focus_field(res_win_t widget);

XDC_API bool_t formctrl_set_field_text(res_win_t widget, link_t_ptr flk, const tchar_t* text);

XDC_API void formctrl_get_field_rect(res_win_t widget, link_t_ptr flk, xrect_t* pxr);

XDC_API bool_t formctrl_get_lock(res_win_t widget);

XDC_API void formctrl_set_lock(res_win_t widget, bool_t bLock);

XDC_API void formctrl_tabskip(res_win_t widget, int nSkip);

XDC_API void formctrl_move_to_page(res_win_t widget, int page);

XDC_API void formctrl_move_first_page(res_win_t widget);

XDC_API void formctrl_move_last_page(res_win_t widget);

XDC_API void formctrl_move_next_page(res_win_t widget);

XDC_API void formctrl_move_prev_page(res_win_t widget);

XDC_API int formctrl_get_cur_page(res_win_t widget);

XDC_API int formctrl_get_max_page(res_win_t widget);

/***********************************grid control*******************************************************************/
typedef struct _NOTICE_GRID{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr grid;
	link_t_ptr row;
	link_t_ptr col;
	void* data;

	int ret;
}NOTICE_GRID;


typedef enum{
	NC_GRIDLBCLK,
	NC_GRIDRBCLK,
	NC_GRIDDBCLK,

	NC_COLHOVER,
	NC_COLCHANGING,
	NC_COLCHANGED,
	NC_COLSELECTED,

	NC_CELLSETFOCUS,
	NC_CELLKILLFOCUS,

	NC_CELLEDITING,
	NC_CELLUPDATE,
	NC_CELLCOMMIT,
	NC_CELLROLLBACK,

	NC_COLCALCED,
	NC_ROWCALCED,
	NC_GRIDCALCED,

	NC_ROWCHANGING,
	NC_ROWCHANGED,
	NC_ROWCHECKED,
	NC_ROWINSERT,
	NC_ROWDELETE,

	NC_ROWSIZING,
	NC_ROWSIZED,
	NC_ROWDRAG,
	NC_ROWDROP,
	NC_COLSIZING,
	NC_COLSIZED,
	NC_COLDRAG,
	NC_COLDROP
}GRID_NOTICE_CODE;

XDC_API res_win_t gridctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	gridctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr gridctrl_detach(res_win_t widget);

XDC_API link_t_ptr gridctrl_fetch(res_win_t widget);

XDC_API bool_t	gridctrl_verify(res_win_t widget, bool_t bAlarm);

XDC_API void	gridctrl_accept(res_win_t widget, bool_t bAccept);

XDC_API bool_t	gridctrl_is_update(res_win_t widget);

XDC_API void	gridctrl_tabskip(res_win_t widget, int dir);

XDC_API void	gridctrl_auto_insert(res_win_t widget, bool_t bAuto);

XDC_API void	gridctrl_set_fixed(res_win_t widget, link_t_ptr clk);

XDC_API link_t_ptr	gridctrl_get_fixed(res_win_t widget);

XDC_API bool_t	gridctrl_set_cell_text(res_win_t widget, link_t_ptr rlk, link_t_ptr clk, const tchar_t* szText);

XDC_API bool_t	gridctrl_copy_row(res_win_t widget, link_t_ptr srcGrid, link_t_ptr srcRow);

XDC_API bool_t	gridctrl_delete_row(res_win_t widget, link_t_ptr rlk);

XDC_API link_t_ptr gridctrl_insert_row(res_win_t widget, link_t_ptr pre);

XDC_API void	gridctrl_redraw(res_win_t widget, bool_t bCalc);

XDC_API void	gridctrl_redraw_row(res_win_t widget, link_t_ptr rlk, bool_t bCalc);

XDC_API void	gridctrl_redraw_col(res_win_t widget, link_t_ptr clk, bool_t bCalc);

XDC_API void	gridctrl_get_cell_rect(res_win_t widget, link_t_ptr rlk, link_t_ptr clk, xrect_t* pxr);

XDC_API bool_t	gridctrl_set_focus_cell(res_win_t widget, link_t_ptr rlk, link_t_ptr clk);

XDC_API link_t_ptr gridctrl_get_focus_row(res_win_t widget);

XDC_API link_t_ptr gridctrl_get_focus_col(res_win_t widget);

XDC_API void	gridctrl_move_first_page(res_win_t widget);

XDC_API void	gridctrl_move_prev_page(res_win_t widget);

XDC_API void	gridctrl_move_next_page(res_win_t widget);

XDC_API void	gridctrl_move_last_page(res_win_t widget);

XDC_API void	gridctrl_move_to_page(res_win_t widget, int page);

XDC_API int		gridctrl_get_cur_page(res_win_t widget);

XDC_API int		gridctrl_get_max_page(res_win_t widget);

XDC_API bool_t	gridctrl_get_lock(res_win_t widget);

XDC_API void	gridctrl_set_lock(res_win_t widget, bool_t bLock);

XDC_API bool_t	gridctrl_get_dirty(res_win_t widget);

XDC_API void	gridctrl_set_dirty(res_win_t widget, bool_t bDirty);

XDC_API void	gridctrl_find(res_win_t widget, link_t_ptr pos, const tchar_t* token);

XDC_API void	gridctrl_filter(res_win_t widget, const tchar_t* token);

XDC_API void	gridctrl_popup_size(res_win_t widget, xsize_t* pse);

/**********************************graph control*******************************************************************/

typedef struct _NOTICE_GRAPH{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr graph;
	link_t_ptr xax;
	link_t_ptr yax;
	link_t_ptr gax;
	void* data;

	int ret;
}NOTICE_GRAPH;

typedef enum{
	NC_GRAPHLBCLK,
	NC_GRAPHRBCLK,
	NC_GRAPHDBCLK,

	NC_GAXCHANGING,
	NC_GAXCHANGED,

	NC_YAXHOVER,
	NC_YAXCHANGING,
	NC_YAXCHANGED,
	NC_YAXSELECTED,

	NC_COORSETFOCUS,
	NC_COORKILLFOCUS,

	NC_COOREDITING,
	NC_COORUPDATE,
	NC_COORCOMMIT,
	NC_COORROLLBACK,

	NC_XAXCHANGING,
	NC_XAXCHANGED,
	NC_XAXCHECKED,
	NC_XAXDELETE,
	NC_XAXINSERT,

	NC_XAXSIZING,
	NC_XAXSIZED,
	NC_YAXSIZING,
	NC_YAXSIZED,

	NC_XAXDRAG,
	NC_XAXDROP,
	NC_YAXDRAG,
	NC_YAXDROP,

	NC_GRAPHCALCED,
	NC_YAXCALCED,
	NC_XAXCALCED
}GRAPH_NOTICE_CODE;

XDC_API res_win_t graphctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	graphctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr graphctrl_detach(res_win_t widget);

XDC_API link_t_ptr graphctrl_fetch(res_win_t widget);

XDC_API void	graphctrl_accept(res_win_t widget, bool_t bAccept);

XDC_API bool_t	graphctrl_is_update(res_win_t widget);

XDC_API void	graphctrl_auto_insert(res_win_t widget, bool_t bAuto);

XDC_API bool_t	graphctrl_delete_xax(res_win_t widget, link_t_ptr xlk);

XDC_API link_t_ptr graphctrl_insert_xax(res_win_t widget, link_t_ptr pre);

XDC_API void	graphctrl_tabskip(res_win_t widget, int nf);

XDC_API void	graphctrl_redraw(res_win_t widget, bool_t bCalc);

XDC_API void	graphctrl_redraw_xax(res_win_t widget, link_t_ptr xlk, bool_t bCalc);

XDC_API void	graphctrl_redraw_yax(res_win_t widget, link_t_ptr ylk, bool_t bCalc);

XDC_API link_t_ptr graphctrl_get_focus_xax(res_win_t widget);

XDC_API link_t_ptr graphctrl_get_focus_yax(res_win_t widget);

XDC_API link_t_ptr graphctrl_get_focus_gax(res_win_t widget);

XDC_API bool_t	graphctrl_set_focus_coor(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk);

XDC_API void	graphctrl_get_coor_rect(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr);

XDC_API bool_t	graphctrl_set_coor_text(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, const tchar_t* szText);

XDC_API void	graphctrl_move_first_page(res_win_t widget);

XDC_API void	graphctrl_move_prev_page(res_win_t widget);

XDC_API void	graphctrl_move_next_page(res_win_t widget);

XDC_API void	graphctrl_move_last_page(res_win_t widget);

XDC_API void	graphctrl_move_to_page(res_win_t widget, int page);

XDC_API int		graphctrl_get_cur_page(res_win_t widget);

XDC_API int		graphctrl_get_max_page(res_win_t widget);

XDC_API bool_t	graphctrl_get_lock(res_win_t widget);

XDC_API void	graphctrl_set_lock(res_win_t widget, bool_t bLock);

XDC_API bool_t	graphctrl_get_dirty(res_win_t widget);

XDC_API void	graphctrl_set_dirty(res_win_t widget, bool_t bDirty);

/***********************************dialog control*******************************************************************/
typedef struct _NOTICE_DIALOG{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr dialog;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_DIALOG;

typedef enum{

	NC_DIALOGLBCLK,
	NC_DIALOGRBCLK,
	NC_DIALOGDBCLK,

	NC_DIALOGITEMHOVER,

	NC_DIALOGITEMCHANGING,
	NC_DIALOGITEMCHANGED,
	NC_DIALOGITEMSELECTED,

	NC_DIALOGITEMCALCED,
	NC_DIALOGCALCED,

	NC_DIALOGITEMDRAG,
	NC_DIALOGITEMDROP,
	NC_DIALOGITEMSIZING,
	NC_DIALOGITEMSIZED,
}DIALOG_NOTICE_CODE;

XDC_API res_win_t dialogctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void dialogctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr dialogctrl_detach(res_win_t widget);

XDC_API link_t_ptr dialogctrl_fetch(res_win_t widget);

XDC_API void dialogctrl_redraw_item(res_win_t widget, link_t_ptr flk);

XDC_API void dialogctrl_redraw(res_win_t widget);

XDC_API bool_t dialogctrl_set_focus_item(res_win_t widget, link_t_ptr flk);

XDC_API link_t_ptr dialogctrl_get_focus_item(res_win_t widget);

XDC_API void dialogctrl_get_item_rect(res_win_t widget, link_t_ptr flk, xrect_t* pxr);

XDC_API void dialogctrl_tabskip(res_win_t widget, int nSkip);

XDC_API bool_t dialogctrl_get_dirty(res_win_t widget);

XDC_API void dialogctrl_set_dirty(res_win_t widget, bool_t bDirty);

/***********************************chart control*******************************************************************/
typedef struct _NOTICE_CHART{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr chart;
	link_t_ptr table;
	void* data;

	int ret;
}NOTICE_CHART;

typedef enum{

	NC_CHARTLBCLK,
	NC_CHARTRBCLK,
	NC_CHARTDBCLK,

	NC_CHARTTABLEHOVER,

	NC_CHARTTABLECHANGING,
	NC_CHARTTABLECHANGED,
	NC_CHARTTABLESELECTED,

	NC_CHARTTABLECALCED,
	NC_CHARTCALCED,

	NC_CHARTTABLEDRAG,
	NC_CHARTTABLEDROP,
	NC_CHARTTABLESIZING,
	NC_CHARTTABLESIZED,
}CHART_NOTICE_CODE;

XDC_API res_win_t chartctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void chartctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr chartctrl_detach(res_win_t widget);

XDC_API link_t_ptr chartctrl_fetch(res_win_t widget);

XDC_API void chartctrl_redraw_table(res_win_t widget, link_t_ptr flk);

XDC_API void chartctrl_redraw(res_win_t widget);

XDC_API bool_t chartctrl_set_focus_table(res_win_t widget, link_t_ptr flk);

XDC_API link_t_ptr chartctrl_get_focus_table(res_win_t widget);

XDC_API void chartctrl_get_table_rect(res_win_t widget, link_t_ptr flk, xrect_t* pxr);

XDC_API void chartctrl_tabskip(res_win_t widget, int nSkip);

XDC_API bool_t chartctrl_get_dirty(res_win_t widget);

XDC_API void chartctrl_set_dirty(res_win_t widget, bool_t bDirty);

/********************************image list control************************************************************/
typedef struct _NOTICE_IMAGES{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr images;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_IMAGES;

typedef enum{

	NC_IMAGESLBCLK,
	NC_IMAGESRBCLK,
	NC_IMAGESDBCLK,

	NC_IMAGEITEMHOVER,

	NC_IMAGEITEMDRAG,
	NC_IMAGEITEMDROP,

	NC_IMAGEITEMCHANGING,
	NC_IMAGEITEMCHANGED,
	NC_IMAGEITEMCHECKED,

	NC_IMAGEITEMEDITING,
	NC_IMAGEITEMUPDATE,
	NC_IMAGEITEMCOMMIT,
	NC_IMAGEITEMROLLBACK,

	NC_IMAGESCALCED,
	NC_IMAGEITEMCALCED
}IMAGES_NOTICE_CODE;

XDC_API res_win_t imagesctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	imagesctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr	imagesctrl_detach(res_win_t widget);

XDC_API link_t_ptr	imagesctrl_fetch(res_win_t widget);

XDC_API void	imagesctrl_redraw(res_win_t widget);

XDC_API void	imagesctrl_tabskip(res_win_t widget, int nSkip);

XDC_API void	imagesctrl_redraw_item(res_win_t widget, link_t_ptr plk);

XDC_API bool_t	imagesctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr	imagesctrl_get_focus_item(res_win_t widget);

XDC_API bool_t imagesctrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* szText);

XDC_API void	imagesctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

XDC_API void	imagesctrl_popup_size(res_win_t widget, xsize_t* pse);

XDC_API bool_t	imagesctrl_get_lock(res_win_t widget);

XDC_API void	imagesctrl_set_lock(res_win_t widget, bool_t bLock);

/*********************************topog control**************************************************************/

typedef struct _NOTICE_TOPOG{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr topog;
	link_t_ptr spot;
	void* data;

	int row;
	int col;

	int ret;
}NOTICE_TOPOG;

typedef enum{
	NC_TOPOGLBCLK,
	NC_TOPOGRBCLK,
	NC_TOPOGDBCLK,

	NC_TOPOGSPOTHOVER,
	NC_TOPOGSPOTCHANGING,
	NC_TOPOGSPOTCHANGED,
	NC_TOPOGSPOTSELECTED,

	NC_TOPOGSPOTDRAG,
	NC_TOPOGSPOTDROP,

	NC_TOPOGCALCED,
	NC_TOPOGSPOTCALCED
}TOPOG_NOTICE_CODE;

XDC_API res_win_t topogctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void topogctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr topogctrl_detach(res_win_t widget);

XDC_API link_t_ptr topogctrl_fetch(res_win_t widget);

XDC_API void topogctrl_redraw(res_win_t widget);

XDC_API void topogctrl_tabskip(res_win_t widget, int nSkip);

XDC_API void topogctrl_redraw_spot(res_win_t widget, link_t_ptr plk);

XDC_API bool_t topogctrl_set_focus_spot(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr topogctrl_get_focus_spot(res_win_t widget);

XDC_API void topogctrl_get_focus_dot(res_win_t widget, int* prow, int* pcol);

XDC_API void topogctrl_get_spot_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

XDC_API bool_t topogctrl_get_lock(res_win_t widget);

XDC_API void topogctrl_set_lock(res_win_t widget, bool_t bLock);

XDC_API bool_t topogctrl_get_dirty(res_win_t widget);

XDC_API void topogctrl_set_dirty(res_win_t widget, bool_t bDirty);

XDC_API bool_t topogctrl_set_bitmap(res_win_t widget, res_bmp_t bmp);

/********************************bitmap control***************************************************************/
typedef struct _NOTICE_PHOTO{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr anno;
	link_t_ptr spot;

	res_ctx_t rdc;
	void* data;

	int ret;
}NOTICE_PHOTO;

typedef enum{
	NC_PHOTOLBCLK,
	NC_PHOTORBCLK,
	NC_PHOTODBCLK,

	NC_PHOTOANNOCHANGING,
	NC_PHOTOANNOCHANGED,
	NC_PHOTOANNOSIZING,
	NC_PHOTOANNOSIZED,
	NC_PHOTOANNODRAG,
	NC_PHOTOANNODROP,

	NC_PHOTOANNOEDITING,
	NC_PHOTOANNOUPDATE,
	NC_PHOTOANNOCOMMIT,
	NC_PHOTOANNOROLLBACK,

	NC_PHOTOFACEDRAW
}PHOTO_NOTICE_CODE;

XDC_API res_win_t photoctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	photoctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr photoctrl_detach(res_win_t widget);

XDC_API link_t_ptr photoctrl_fetch(res_win_t widget);

XDC_API void photoctrl_set_bitmap(res_win_t widget, const byte_t* data, dword_t size);

XDC_API dword_t photoctrl_get_bitmap(res_win_t widget, byte_t* buf, dword_t max);

XDC_API void photoctrl_commit(res_win_t widget);

XDC_API link_t_ptr photoctrl_get_focus_spot(res_win_t widget);

XDC_API void photoctrl_redraw(res_win_t widget);

XDC_API bool_t photoctrl_get_lock(res_win_t widget);

XDC_API void photoctrl_set_lock(res_win_t widget, bool_t bLock);

XDC_API bool_t photoctrl_get_dirty(res_win_t widget);

XDC_API void photoctrl_set_dirty(res_win_t widget, bool_t bDirty);

/***********************************table control*******************************************************************/
typedef struct _NOTICE_TABLE{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr table;
	link_t_ptr item;
	bool_t onkey;
	void* data;

	int ret;
}NOTICE_TABLE;

typedef enum{

	NC_TABLELBCLK,
	NC_TABLERBCLK,
	NC_TABLEDBCLK,

	NC_TABLEITEMINSERT,
	NC_TABLEITEMDELETE,

	NC_TABLEITEMCHANGING,
	NC_TABLEITEMCHANGED,

	NC_TABLEITEMEDITING,
	NC_TABLEITEMUPDATE,
	NC_TABLEITEMCOMMIT,
	NC_TABLEITEMROLLBACK,

	NC_TABLECALCED,
	NC_TABLEITEMCALCED
}TABLE_NOTICE_CODE;

XDC_API res_win_t tablectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	tablectrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr tablectrl_detach(res_win_t widget);

XDC_API link_t_ptr tablectrl_fetch(res_win_t widget);

XDC_API void	tablectrl_accept(res_win_t widget, bool_t bAccept);

XDC_API bool_t	tablectrl_is_dirty(res_win_t widget);

XDC_API void	tablectrl_tabskip(res_win_t widget, int dir);

XDC_API void	tablectrl_auto_insert(res_win_t widget, bool_t bAuto);

XDC_API bool_t	tablectrl_delete_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr tablectrl_insert_item(res_win_t widget, link_t_ptr pos);

XDC_API void	tablectrl_redraw(res_win_t widget);

XDC_API void	tablectrl_redraw_item(res_win_t widget, link_t_ptr ilk);

XDC_API bool_t	tablectrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr tablectrl_get_focus_item(res_win_t widget);

XDC_API void	tablectrl_set_item_key_text(res_win_t widget, link_t_ptr ilk, const tchar_t* key);

XDC_API void	tablectrl_set_item_val_text(res_win_t widget, link_t_ptr ilk, const tchar_t* val);

XDC_API void	tablectrl_set_ratio(res_win_t widget, float r);

XDC_API float	tablectrl_get_ratio(res_win_t widget);

XDC_API bool_t	tablectrl_get_lock(res_win_t widget);

XDC_API void	tablectrl_set_lock(res_win_t widget, bool_t bLock);

/*********************************label control**************************************************************/

typedef struct _NOTICE_LABEL{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr label;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_LABEL;


typedef enum{

	NC_LABELLBCLK,
	NC_LABELRBCLK,
	NC_LABELDBCLK,

	NC_LABELITEMHOVER,

	NC_LABELITEMCHANGING,
	NC_LABELITEMCHANGED,

	NC_LABELITEMDRAG,
	NC_LABELITEMDROP,

	NC_LABELCALCED,
	NC_LABELITEMCALCED
}LABEL_NOTICE_CODE;

XDC_API res_win_t labelctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	labelctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr labelctrl_detach(res_win_t widget);

XDC_API link_t_ptr labelctrl_fetch(res_win_t widget);

XDC_API void	labelctrl_redraw(res_win_t widget);

XDC_API void	labelctrl_tabskip(res_win_t widget, int nSkip);

XDC_API void	labelctrl_redraw_item(res_win_t widget, link_t_ptr plk);

XDC_API bool_t	labelctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr labelctrl_get_focus_item(res_win_t widget);

XDC_API void	labelctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

XDC_API void	labelctrl_move_first_page(res_win_t widget);

XDC_API void	labelctrl_move_prev_page(res_win_t widget);

XDC_API void	labelctrl_move_next_page(res_win_t widget);

XDC_API void	labelctrl_move_last_page(res_win_t widget);

XDC_API void	labelctrl_move_to_page(res_win_t widget, int page);

XDC_API int		labelctrl_get_cur_page(res_win_t widget);

XDC_API int		labelctrl_get_max_page(res_win_t widget);

XDC_API void	labelctrl_popup_size(res_win_t widget, xsize_t* pse);

/********************************list control************************************************************/
typedef struct _NOTICE_LIST{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr list;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_LIST;

typedef enum{

	NC_LISTLBCLK,
	NC_LISTRBCLK,
	NC_LISTDBCLK,

	NC_LISTITEMHOVER,

	NC_LISTITEMCHANGING,
	NC_LISTITEMCHANGED,
	NC_LISTITEMCHECKED,

	NC_LISTITEMEDITING,
	NC_LISTITEMUPDATE,
	NC_LISTITEMCOMMIT,
	NC_LISTITEMROLLBACK,

	NC_LISTITEMEXPAND,
	NC_LISTITEMCOLLAPSE,
	NC_LISTITEMDRAG,
	NC_LISTITEMDROP,

	NC_LISTCALCED,
	NC_LISTITEMCALCED
}LIST_NOTICE_CODE;

XDC_API res_win_t listctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	listctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr	listctrl_detach(res_win_t widget);

XDC_API link_t_ptr	listctrl_fetch(res_win_t widget);

XDC_API void	listctrl_accept(res_win_t widget, bool_t bAccept);

XDC_API void	listctrl_redraw(res_win_t widget);

XDC_API void	listctrl_tabskip(res_win_t widget, int nSkip);

XDC_API void	listctrl_redraw_item(res_win_t widget, link_t_ptr plk);

XDC_API bool_t	listctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr	listctrl_get_focus_item(res_win_t widget);

XDC_API bool_t listctrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* szText);

XDC_API void	listctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

XDC_API void	listctrl_find(res_win_t widget, link_t_ptr pos, const tchar_t* token);

XDC_API void	listctrl_popup_size(res_win_t widget, xsize_t* pse);

XDC_API bool_t	listctrl_get_lock(res_win_t widget);

XDC_API void	listctrl_set_lock(res_win_t widget, bool_t bLock);

/**************************proper control*************************************************************/
typedef struct _NOTICE_PROPER{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr proper;
	link_t_ptr section;
	link_t_ptr entity;
	void* data;

	int ret;
}NOTICE_PROPER;

typedef enum{

	NC_PROPERLBCLK,
	NC_PROPERRBCLK,
	NC_PROPERDBCLK,

	NC_ENTITYHOVER,

	NC_ENTITYCHANGING,
	NC_ENTITYCHANGED,

	NC_ENTITYEDITING,
	NC_ENTITYUPDATE,
	NC_ENTITYCOMMIT,
	NC_ENTITYROLLBACK,

	NC_PROPERCALCED,
	NC_SECTIONCALCED,
	NC_ENTITYCALCED
}PROPER_NOTICE_CODE;

XDC_API res_win_t properctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	properctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr properctrl_detach(res_win_t widget);

XDC_API link_t_ptr properctrl_fetch(res_win_t widget);

XDC_API void	properctrl_accept(res_win_t widget, bool_t bAccept);

XDC_API void	properctrl_redraw(res_win_t widget);

XDC_API void	properctrl_redraw_entity(res_win_t widget, link_t_ptr elk);

XDC_API void	properctrl_redraw_section(res_win_t widget, link_t_ptr slk);

XDC_API bool_t	properctrl_set_focus_entity(res_win_t widget, link_t_ptr elk);

XDC_API link_t_ptr properctrl_get_focus_entity(res_win_t widget);

XDC_API void	properctrl_get_entity_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

XDC_API void	properctrl_tabskip(res_win_t widget, int dir);

XDC_API bool_t	properctrl_set_entity_value(res_win_t widget, link_t_ptr elk, const tchar_t* token);

XDC_API bool_t	properctrl_get_lock(res_win_t widget);

XDC_API void	properctrl_set_lock(res_win_t widget, bool_t bLock);

/***************************************status control******************************************************************/
typedef struct _NOTICE_STATUS{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr status;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_STATUS;

typedef enum{

	NC_STATUSLBCLK,
	NC_STATUSRBCLK,
	NC_STATUSDBCLK,

	NC_STATUSITEMHOVER,

	NC_STATUSCALCED,
	NC_STATUSITEMCALCED
}STATUS_NOTICE_CODE;

XDC_API res_win_t statusctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	statusctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr statusctrl_detach(res_win_t widget);

XDC_API link_t_ptr statusctrl_fetch(res_win_t widget);

XDC_API void	statusctrl_redraw(res_win_t widget);

XDC_API void	statusctrl_redraw_title(res_win_t widget);

XDC_API void	statusctrl_redraw_item(res_win_t widget, link_t_ptr plk);

XDC_API bool_t	statusctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr statusctrl_get_focus_item(res_win_t widget);

XDC_API void	statusctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

XDC_API void	statusctrl_tabskip(res_win_t widget, int nSkip);

XDC_API void	statusctrl_show_step(res_win_t widget, bool_t b_show);

XDC_API void	statusctrl_step_it(res_win_t widget, int steps, const tchar_t* sz_step);

/*******************************message control************************************************************/
typedef struct _NOTICE_NOTES{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr arch;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_NOTES;

typedef enum{
	NC_NOTESLBCLK,
	NC_NOTESRBCLK,
	NC_NOTESDBCLK,

	NC_NOTESITEMHOVER,

	NC_NOTESITEMCHANGING,
	NC_NOTESITEMCHANGED,
	NC_NOTESITEMDELETE,

	NC_NOTESCALCED,
	NC_NOTESITEMCALCED
}NOTES_NOTICE_CODE;

XDC_API res_win_t notesctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	notesctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr notesctrl_detach(res_win_t widget);

XDC_API link_t_ptr notesctrl_fetch(res_win_t widget);

XDC_API void	notesctrl_redraw(res_win_t widget);

XDC_API void	notesctrl_redraw_item(res_win_t widget, link_t_ptr elk);

XDC_API bool_t	notesctrl_set_focus_item(res_win_t widget, link_t_ptr ent);

XDC_API link_t_ptr notesctrl_get_focus_item(res_win_t widget);

XDC_API void	notesctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

XDC_API void	notesctrl_tabskip(res_win_t widget, int nSkip);

/*************************************title control***********************************************************/

typedef struct _NOTICE_TITLE{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr title;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_TITLE;

typedef enum{

	NC_TITLELBCLK,
	NC_TITLERBCLK,
	NC_TITLEDBCLK,

	NC_TITLEITEMHOVER,

	NC_TITLEITEMCHANGING,
	NC_TITLEITEMCHANGED,

	NC_TITLEITEMINSERT,
	NC_TITLEITEMDELETE,

	NC_TITLECALCED,
	NC_TITLEITEMCALCED
}TITLE_NOTICE_CODE;

XDC_API res_win_t titlectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	titlectrl_auto_focus(res_win_t widget, bool_t b);

XDC_API void	titlectrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr titlectrl_detach(res_win_t widget);

XDC_API link_t_ptr titlectrl_fetch(res_win_t widget);

XDC_API void	titlectrl_redraw(res_win_t widget);

XDC_API void	titlectrl_tabskip(res_win_t widget, int nSkip);

XDC_API void	titlectrl_redraw_item(res_win_t widget, link_t_ptr plk);

XDC_API link_t_ptr titlectrl_insert_item(res_win_t widget, link_t_ptr pos);

XDC_API bool_t	titlectrl_delete_item(res_win_t widget, link_t_ptr plk);

XDC_API bool_t	titlectrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr titlectrl_get_focus_item(res_win_t widget);

XDC_API void	titlectrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*************************************title control******************************************************/
typedef struct _NOTICE_TOOL{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr tool;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_TOOL;

typedef enum{

	NC_TOOLLBCLK,
	NC_TOOLRBCLK,
	NC_TOOLDBCLK,

	NC_TOOLITEMHOVER,

	NC_TOOLCALCED,
	NC_TOOLITEMCALCED
}TOOL_NOTICE_CODE;

XDC_API res_win_t toolctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	toolctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr toolctrl_detach(res_win_t widget);

XDC_API link_t_ptr toolctrl_fetch(res_win_t widget);

XDC_API void	toolctrl_redraw(res_win_t widget);

XDC_API void	toolctrl_tabskip(res_win_t widget, int nSkip);

XDC_API void	toolctrl_redraw_item(res_win_t widget, link_t_ptr ilk);

XDC_API bool_t	toolctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr toolctrl_get_focus_item(res_win_t widget);

XDC_API void	toolctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*************************************tree control****************************************************/
typedef struct _NOTICE_TREE{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr tree;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_TREE;

typedef enum{

	NC_TREELBCLK,
	NC_TREERBCLK,
	NC_TREEDBCLK,

	NC_TREEITEMHOVER,

	NC_TREEITEMCHANGING,
	NC_TREEITEMCHANGED,
	NC_TREEITEMCHECKED,

	NC_TREEITEMEDITING,
	NC_TREEITEMUPDATE,
	NC_TREEITEMCOMMIT,
	NC_TREEITEMROLLBACK,

	NC_TREEITEMEXPAND,
	NC_TREEITEMCOLLAPSE,

	NC_TREEITEMDRAG,
	NC_TREEITEMDROP,

	NC_TREECALCED,
	NC_TREEITEMCALCED
}TREE_NOTICE_CODE;

XDC_API res_win_t treectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	treectrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr treectrl_detach(res_win_t widget);

XDC_API link_t_ptr treectrl_fetch(res_win_t widget);

XDC_API void	treectrl_accept(res_win_t widget, bool_t bAccept);

XDC_API void	treectrl_redraw(res_win_t widget);

XDC_API void	treectrl_redraw_item(res_win_t widget, link_t_ptr ilk);

XDC_API bool_t	treectrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

XDC_API link_t_ptr treectrl_get_focus_item(res_win_t widget);

XDC_API bool_t treectrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* szText);

XDC_API void	treectrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

XDC_API void	treectrl_tabskip(res_win_t widget, int nSkip);

XDC_API void	treectrl_find(res_win_t widget, link_t_ptr pos, const tchar_t* token);

XDC_API void	treectrl_popup_size(res_win_t widget, xsize_t* pse);

XDC_API bool_t	treectrl_get_lock(res_win_t widget);

XDC_API void	treectrl_set_lock(res_win_t widget, bool_t bLock);

/**************************************rich control**************************************************/
typedef struct _NOTICE_RICH{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr rich;
	link_t_ptr node;
	void* data;

	int ret;
}NOTICE_RICH;

typedef enum{
	NC_RICHLBCLK,
	NC_RICHRBCLK,
	NC_RICHDBCLK,

	NC_RICHATOMEDITING,
	NC_RICHATOMUPDATE,
	NC_RICHATOMCOMMIT,
	NC_RICHATOMROLLBACK,

	NC_RICHNODEUPDATE,
	NC_RICHNODECHANGED,

	NC_RICHCALCED,
	NC_RICHNODECALCED
}RICH_NOTICE_CODE;

XDC_API res_win_t richctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	richctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr richctrl_detach(res_win_t widget);

XDC_API link_t_ptr richctrl_fetch(res_win_t widget);

XDC_API void	richctrl_redraw(res_win_t widget);

XDC_API link_t_ptr richctrl_get_focus_node(res_win_t widget);

XDC_API void richctrl_set_focus_node(res_win_t widget, link_t_ptr nlk);

XDC_API void richctrl_delete_node(res_win_t widget, link_t_ptr nlk);

XDC_API link_t_ptr richctrl_insert_node(res_win_t widget, link_t_ptr pos);

XDC_API void richctrl_set_node_text(res_win_t widget, link_t_ptr nlk, const tchar_t* token);

XDC_API void richctrl_get_node_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr);

XDC_API void richctrl_select_cur(res_win_t widget);

XDC_API void richctrl_select_all(res_win_t widget);

XDC_API int richctrl_get_selected_text(res_win_t widget, tchar_t* buf, int max);

XDC_API bool_t richctrl_get_dirty(res_win_t widget);

XDC_API void richctrl_set_dirty(res_win_t widget, bool_t bDirty);

XDC_API void richctrl_move_to_page(res_win_t widget, int page);

XDC_API void richctrl_move_first_page(res_win_t widget);

XDC_API void richctrl_move_last_page(res_win_t widget);

XDC_API void richctrl_move_next_page(res_win_t widget);

XDC_API void richctrl_move_prev_page(res_win_t widget);

XDC_API int richctrl_get_cur_page(res_win_t widget);

XDC_API int richctrl_get_max_page(res_win_t widget);

XDC_API bool_t richctrl_get_lock(res_win_t widget);

XDC_API void richctrl_set_lock(res_win_t widget, bool_t bLock);

/**************************************memo control**************************************************/
typedef struct _NOTICE_MEMO{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr memo;
	link_t_ptr line;
	void* data;

	int ret;
}NOTICE_MEMO;

typedef enum{
	NC_MEMOLBCLK,
	NC_MEMORBCLK,
	NC_MEMODBCLK,

	NC_MEMOLINEUPDATE,
	NC_MEMOLINECHANGED,

	NC_MEMOCALCED,
	NC_MEMOLINECALCED
}MEMO_NOTICE_CODE;

XDC_API res_win_t memoctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	memoctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr memoctrl_detach(res_win_t widget);

XDC_API link_t_ptr memoctrl_fetch(res_win_t widget);

XDC_API void	memoctrl_redraw(res_win_t widget);

XDC_API link_t_ptr memoctrl_get_focus_line(res_win_t widget);

XDC_API void memoctrl_set_focus_line(res_win_t widget, link_t_ptr nlk);

XDC_API void memoctrl_delete_line(res_win_t widget, link_t_ptr nlk);

XDC_API void memoctrl_set_line_text(res_win_t widget, link_t_ptr nlk, const tchar_t* token);

XDC_API void memoctrl_get_line_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr);

XDC_API void memoctrl_selec_cur(res_win_t widget);

XDC_API void memoctrl_select_all(res_win_t widget);

XDC_API int memoctrl_get_selected_text(res_win_t widget, tchar_t* buf, int max);

XDC_API bool_t memoctrl_get_dirty(res_win_t widget);

XDC_API void memoctrl_set_dirty(res_win_t widget, bool_t bDirty);

XDC_API void memoctrl_move_to_page(res_win_t widget, int page);

XDC_API void memoctrl_move_first_page(res_win_t widget);

XDC_API void memoctrl_move_last_page(res_win_t widget);

XDC_API void memoctrl_move_next_page(res_win_t widget);

XDC_API void memoctrl_move_prev_page(res_win_t widget);

XDC_API int memoctrl_get_cur_page(res_win_t widget);

XDC_API int memoctrl_get_max_page(res_win_t widget);

XDC_API bool_t memoctrl_get_lock(res_win_t widget);

XDC_API void memoctrl_set_lock(res_win_t widget, bool_t bLock);

/**************************************tag control**************************************************/
typedef struct _NOTICE_TAG{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr tag;
	link_t_ptr node;
	void* data;

	int ret;
}NOTICE_TAG;

typedef enum{
	NC_TAGLBCLK,
	NC_TAGRBCLK,
	NC_TAGDBCLK,

	NC_TAGNODEEDITING,
	NC_TAGNODEUPDATE,
	NC_TAGNODECHANGED,
	NC_TAGNODECOMMIT,
	NC_TAGNODEROLLBACK,

	NC_TAGCALCED,
	NC_TAGNODECALCED
}TAG_NOTICE_CODE;

XDC_API res_win_t tagctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

XDC_API void	tagctrl_attach(res_win_t widget, link_t_ptr ptr);

XDC_API link_t_ptr tagctrl_detach(res_win_t widget);

XDC_API link_t_ptr tagctrl_fetch(res_win_t widget);

XDC_API void	tagctrl_redraw(res_win_t widget);

XDC_API link_t_ptr tagctrl_get_focus_node(res_win_t widget);

XDC_API void tagctrl_set_focus_node(res_win_t widget, link_t_ptr nlk);

XDC_API void tagctrl_delete_node(res_win_t widget, link_t_ptr nlk);

XDC_API void tagctrl_set_node_text(res_win_t widget, link_t_ptr nlk, const tchar_t* token);

XDC_API void tagctrl_get_node_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr);

XDC_API void tagctrl_select_all(res_win_t widget);

XDC_API void tagctrl_select_cur(res_win_t widget);

XDC_API int tagctrl_get_selected_text(res_win_t widget, tchar_t* buf, int max);

XDC_API void tagctrl_setup_tag(res_win_t widget, const tchar_t* tname);

XDC_API void tagctrl_clean_tag(res_win_t widget);

XDC_API bool_t tagctrl_get_dirty(res_win_t widget);

XDC_API void tagctrl_set_dirty(res_win_t widget, bool_t bDirty);

XDC_API void tagctrl_move_to_page(res_win_t widget, int page);

XDC_API void tagctrl_move_first_page(res_win_t widget);

XDC_API void tagctrl_move_last_page(res_win_t widget);

XDC_API void tagctrl_move_next_page(res_win_t widget);

XDC_API void tagctrl_move_prev_page(res_win_t widget);

XDC_API int tagctrl_get_cur_page(res_win_t widget);

XDC_API int tagctrl_get_max_page(res_win_t widget);

XDC_API bool_t tagctrl_get_lock(res_win_t widget);

XDC_API void tagctrl_set_lock(res_win_t widget, bool_t bLock);


#ifdef	__cplusplus
}
#endif

#endif /*XDCCTRL_H*/
