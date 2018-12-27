/***********************************************************************
	Easily DICMView v1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc DicomPanel document

	@module	DicomPanel implement file

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

#include "DicomPanel.h"

#include "_Define.h"
#include "_Module.h"
#include "_Frame.h"

#define IDC_DICOMPANEL_PHOTO		201
#define IDC_DICOMPANEL_PROPER		202
#define IDC_DICOMPANEL_WINDOW		203
#define IDC_DICOMPANEL_MENU			204

#define DICOMPANEL_TITLEITEM_WIDTH		(float)15
#define DICOMPANEL_TITLEITEM_HEIGHT		(float)10


typedef struct tagDicomPanelDelta{
	res_win_t hProper;
	res_win_t hPhoto;

	LINKPTR dicm;
	int nIndex;
	long nWindow;
	long nCenter;
	long nLevel;

	int nAnno;

	tchar_t szFile[PATH_LEN];
}DicomPanelDelta;

#define GETDICOMPANELDELTA(ph) 		(DicomPanelDelta*)widget_get_user_delta(ph)
#define SETDICOMPANELDELTA(ph,ptd)	widget_set_user_delta(ph,(void*)ptd)

#define DICOMPANEL_ACCEL_COUNT	5
accel_t	DICOMPANEL_ACCEL[DICOMPANEL_ACCEL_COUNT] = {
	FVIRTKEY | FCONTROL, _T('C'), IDA_EDIT_COPY,
	FVIRTKEY | FCONTROL, _T('X'), IDA_EDIT_CUT,
	FVIRTKEY | FCONTROL, _T('V'), IDA_EDIT_PASTE,
	FVIRTKEY, VK_DELETE, IDA_EDIT_DELETE,
	FVIRTKEY | FCONTROL, _T('Z'), IDA_EDIT_UNDO,
};

typedef struct _CT_WINDOW{
	tchar_t wname[RES_LEN];
	long window;
	long center;
}CT_WINDOW;

static CT_WINDOW ctwin[] = {
	{ _T("Ô­Ê¼´°"), 0, 0 },
	{ _T("Í·Â­Æ½É¨"), 90, 35 },
	{ _T("Í·Â­ÔöÇ¿"), 85, 40 },
	{ _T("Í·Â­¹Ç´°"), 1600, 450 },
	{ _T("¹Ø½Ú¹Ç´°"), 1600, 550 },
	{ _T("¹Ø½ÚÈí×éÖ¯"), 300, 40 },
	{ _T("±Çñ¼Èí×éÖ¯"), 350, 35 },
	{ _T("±Çñ¼¹Ç´°"), 2000, 450 },
	{ _T("ÈéÍ»"), 4000, 650 },
	{ _T("×µ¼äÅÌÈí×éÖ¯"), 350, 40 },
	{ _T("×µ¼äÅÌ¹Ç´°"), 2000, 450 },
	{ _T("·Î´°"), 1000, -650 },
	{ _T("×Ý¸ô´°"), 350, 40 },
	{ _T("·Î¸ß·Ö±æ"), 1500, -500 },
	{ _T("¸ÎÔà"), 200, 50 },
	{ _T("ÉöÔà"), 350, 40 },
};

void	DicomPanel_Switch(res_win_t widget);
bool_t	DicomPanel_GetDirty(res_win_t widget);
void	DicomPanel_SetDirty(res_win_t widget, bool_t bDirty);

bool_t	DicomPanel_SaveFile(res_win_t widget, const tchar_t* szFile);
bool_t	DicomPanel_OpenFile(res_win_t widget, const tchar_t* szFile);

void	DicomPanel_OnImageWheel(res_win_t photo, bool_t horz, long delta);

/*****************************************************************************************************/
bool_t DicomPanel_GetDirty(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	return photoctrl_get_dirty(pdt->hPhoto);
}

void DicomPanel_SetDirty(res_win_t widget, bool_t bDirty)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	photoctrl_set_dirty(pdt->hPhoto, bDirty);
}

void DicomPanel_Switch(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if(!DicomPanel_GetDirty(widget))
		return;

	dword_t rt = ShowMsg(MSGBTN_YES | MSGBTN_NO | MSGICO_TIP, _T("ÎÄ¼þÉÐÎ´±£´æ£¬ÊÇ·ñ±£´æÎÄ¼þ£¿"));

	switch (rt)
	{
	case MSGBTN_YES:
		widget_send_command(widget, 0, IDA_FILE_SAVE, NULL);
		break;
	case MSGBTN_NO:
		DicomPanel_SetDirty(widget, 0);
		break;
	}
}

void DicomPanel_UpdateProper(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	LINKPTR ptr_prop = properctrl_fetch(pdt->hProper);
	clear_proper_doc(ptr_prop);
	properctrl_redraw(pdt->hProper);

	if (!pdt->dicm)
		return;

	LINKPTR nlk = get_dom_first_child_node(pdt->dicm);
	while (nlk)
	{
		if (get_dom_node_cdata(nlk))
			write_proper(ptr_prop, get_dom_node_attr_ptr(nlk, _T("VG"),-1), -1, get_dom_node_name_ptr(nlk), -1, _T("..."), -1);
		else
			write_proper(ptr_prop, get_dom_node_attr_ptr(nlk, _T("VG"), -1), -1, get_dom_node_name_ptr(nlk), -1, get_dom_node_text_ptr(nlk), -1);

		nlk = get_dom_next_sibling_node(nlk);
	}

	properctrl_redraw(pdt->hProper);
}

void DicomPanel_LoadAnno(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	xpoint_t pt[5] = { 0 };

	if (!pdt->dicm)
		return;

	LINKPTR ptr_anno = photoctrl_fetch(pdt->hPhoto);
	clear_anno_doc(ptr_anno);
	photoctrl_redraw(pdt->hPhoto);

	int n = load_dicm_text_annotation(pdt->dicm, NULL, MAX_LONG);
	if (n)
	{
		dicm_text_anno_t* pta = (dicm_text_anno_t*)xmem_alloc(n * sizeof(dicm_text_anno_t));
		load_dicm_text_annotation(pdt->dicm, pta, n);

		for (int i = 0; i < n; i++)
		{
			pt[0].x = pta[i].points[0].x;
			pt[0].y = pta[i].points[0].y;
			pt[1].x = pta[i].points[1].x;
			pt[1].y = pta[i].points[1].y;

			widget_viewbox_to_client(pdt->hPhoto, &(pt[0]));
			widget_viewbox_to_client(pdt->hPhoto, &(pt[1]));

			widget_point_to_tm(pdt->hPhoto, &(pt[0]));
			widget_point_to_tm(pdt->hPhoto, &(pt[1]));
			
			LINKPTR ilk = insert_anno_spot(ptr_anno, LINK_LAST);
			set_anno_spot_type(ilk, ATTR_ANNO_TYPE_TEXT);
			set_anno_spot_style(ilk, _T("font-size:9;font-weight:700;text-color:RGB(250,0,0);"));
			set_anno_spot_xpoint(ilk, pt, 2);
		}

		xmem_free(pta);
	}

	n = load_dicm_graphic_annotation(pdt->dicm, NULL, MAX_LONG);
	if (n)
	{
		dicm_graphic_anno_t* pga = (dicm_graphic_anno_t*)xmem_alloc(n * sizeof(dicm_graphic_anno_t));
		load_dicm_graphic_annotation(pdt->dicm, pga, n);

		for (int i = 0; i < n; i++)
		{
			pt[0].x = pga[i].points[0].x;
			pt[0].y = pga[i].points[0].y;
			pt[1].x = pga[i].points[1].x;
			pt[1].y = pga[i].points[1].y;

			widget_viewbox_to_client(pdt->hPhoto, &(pt[0]));
			widget_viewbox_to_client(pdt->hPhoto, &(pt[1]));

			widget_point_to_tm(pdt->hPhoto, &(pt[0]));
			widget_point_to_tm(pdt->hPhoto, &(pt[1]));

			LINKPTR ilk = insert_anno_spot(ptr_anno, LINK_LAST);
			set_anno_spot_type(ilk, ATTR_ANNO_TYPE_CIRCLE);
			set_anno_spot_style(ilk, _T("stroke-color:RGB(250,0,0);"));
			set_anno_spot_xpoint(ilk, pt, 2);
		}

		xmem_free(pga);
	}
}

void DicomPanel_SaveAnno(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	LINKPTR ptr_anno = photoctrl_fetch(pdt->hPhoto);
	int nGraph = 0;
	int nText = 0;

	LINKPTR ilk = get_anno_next_spot(ptr_anno, LINK_FIRST);
	while (ilk)
	{
		if (compare_text(get_anno_spot_type_ptr(ilk), -1, ATTR_ANNO_TYPE_TEXT, -1, 0) == 0)
			nText++;
		else if (compare_text(get_anno_spot_type_ptr(ilk), -1, ATTR_ANNO_TYPE_CIRCLE, -1, 0) == 0)
			nGraph++;

		ilk = get_anno_next_spot(ptr_anno, ilk);
	}

	if (nText)
	{
		dicm_text_anno_t* pta = (dicm_text_anno_t*)xmem_alloc(nText * sizeof(dicm_text_anno_t));

		nText = 0;
		ilk = get_anno_prev_spot(ptr_anno, LINK_LAST);
		while (ilk)
		{
			if (compare_text(get_anno_spot_type_ptr(ilk), -1, ATTR_ANNO_TYPE_TEXT, -1, 0) == 0)
			{
				get_anno_spot_xpoint(ilk, pta[nText].points, 2);

				widget_point_to_pt(pdt->hPhoto, &(pta[nText].points[0]));
				widget_point_to_pt(pdt->hPhoto, &(pta[nText].points[1]));

				widget_client_to_viewbox(pdt->hPhoto, &(pta[nText].points[0]));
				widget_client_to_viewbox(pdt->hPhoto, &(pta[nText].points[1]));

				xscpy(pta[nText].align, _T("LEFT"));
				xscpy(pta[nText].text, get_anno_spot_text_ptr(ilk));

				nText++;
			}

			ilk = get_anno_prev_spot(ptr_anno, ilk);
		}

		save_dicm_text_annotation(pdt->dicm, pta, nText);
		xmem_free(pta);
	}

	if (nGraph)
	{
		dicm_graphic_anno_t* ptg = (dicm_graphic_anno_t*)xmem_alloc(nGraph * sizeof(dicm_graphic_anno_t));

		nGraph = 0;
		ilk = get_anno_prev_spot(ptr_anno, LINK_LAST);
		while (ilk)
		{
			if (compare_text(get_anno_spot_type_ptr(ilk), -1, ATTR_ANNO_TYPE_CIRCLE, -1, 0) == 0)
			{
				get_anno_spot_xpoint(ilk, ptg[nGraph].points, 2);

				widget_point_to_pt(pdt->hPhoto, &(ptg[nGraph].points[0]));
				widget_point_to_pt(pdt->hPhoto, &(ptg[nGraph].points[1]));

				widget_client_to_viewbox(pdt->hPhoto, &(ptg[nGraph].points[0]));
				widget_client_to_viewbox(pdt->hPhoto, &(ptg[nGraph].points[1]));

				ptg[nGraph].count = 2;
				xscpy(ptg[nGraph].type, _T("CIRCLE"));

				nGraph++;
			}

			ilk = get_anno_prev_spot(ptr_anno, ilk);
		}

		save_dicm_graphic_annotation(pdt->dicm, ptg, nGraph);
		xmem_free(ptg);
	}
}

void DicomPanel_UpdatePhoto(res_win_t widget, bool_t reset)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (!pdt->dicm)
		return;

	dword_t pdg_len = get_dicm_pdg(pdt->dicm, NULL, MAX_LONG);
	byte_t* pdg_buf = (byte_t*)xmem_alloc(pdg_len);

	get_dicm_pdg(pdt->dicm, pdg_buf, pdg_len);

	if (pdg_len)
	{
		float xmm_pixel = dicm_get_pdg_xmm_pixel(pdg_buf);
		float ymm_pixel = dicm_get_pdg_ymm_pixel(pdg_buf);

		if ((long)xmm_pixel && (long)ymm_pixel)
		{
			object_t canv = widget_get_canvas(pdt->hPhoto);
			set_canvas_ratio(canv, (float)(1.0 / xmm_pixel), (float)(1.0 / ymm_pixel));
		}

		if (reset)
		{
			ctwin[0].window = (long)(dicm_get_pdg_win_width(pdg_buf));
			ctwin[0].center = (long)(dicm_get_pdg_win_center(pdg_buf));

			pdt->nIndex = 0;
			pdt->nWindow = (long)(dicm_get_pdg_win_width(pdg_buf));
			pdt->nCenter = (long)(dicm_get_pdg_win_center(pdg_buf));
			pdt->nLevel = (long)(dicm_get_pdg_win_center(pdg_buf));
		}
		else
		{
			dicm_set_pdg_win_width(pdg_buf, (float)pdt->nWindow);
			dicm_set_pdg_win_width(pdg_buf, (float)pdt->nLevel);
		}

		dword_t bmp_len = xpdg_revert(pdg_buf, pdg_len, NULL, MAX_LONG);
		byte_t* bmp_buf = (byte_t*)xmem_alloc(bmp_len);

		xpdg_revert(pdg_buf, pdg_len, bmp_buf, bmp_len);

		xmem_free(pdg_buf);

		photoctrl_set_bitmap(pdt->hPhoto, bmp_buf, bmp_len);

		xmem_free(bmp_buf);

		photoctrl_redraw(pdt->hPhoto);
	}
	else
	{
		ctwin[0].window = 0;
		ctwin[0].center = 0;

		pdt->nIndex = 0;
		pdt->nWindow = 0;
		pdt->nCenter = 0;
		pdt->nLevel = 0;

		photoctrl_set_bitmap(pdt->hPhoto, NULL, 0);
		photoctrl_redraw(pdt->hPhoto);
	}

	tchar_t szTitle[ERR_LEN];
	xsprintf(szTitle, _T("´°Î»£º%d"), pdt->nLevel);
	MainFrame_UpdateCenter(g_hMain, szTitle);

	xsprintf(szTitle, _T("%s£º%d,%d"), ctwin[pdt->nIndex].wname, ctwin[pdt->nIndex].window, ctwin[pdt->nIndex].center);
	MainFrame_UpdateWindow(g_hMain, szTitle);
}

bool_t DicomPanel_OpenFile(res_win_t widget, const tchar_t* szFile)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (pdt->dicm)
	{
		destroy_dom_doc(pdt->dicm);
		pdt->dicm = NULL;
	}
	
	if (!is_null(szFile))
	{
		pdt->dicm = create_dom_doc();
		if (!load_dicm_doc_from_file(pdt->dicm, NULL, szFile))
		{
			destroy_dom_doc(pdt->dicm);
			ShowMsg(MSGICO_ERR, _T("´ò¿ªDICOMÎÄ¼þÊ§°Ü!"));
			return 0;
		}
	}
	else
	{
		pdt->dicm = create_dicm_sop(SOP_CAP);
	}

	xscpy(pdt->szFile, szFile);

	DicomPanel_UpdatePhoto(widget, 1);

	DicomPanel_LoadAnno(widget);

	DicomPanel_UpdateProper(widget);

	return 1;
}

bool_t DicomPanel_SaveFile(res_win_t widget, const tchar_t* szFile)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	DicomPanel_SaveAnno(widget);

	bool_t rt = save_dicm_doc_to_file(pdt->dicm, NULL, szFile);

	if (!rt)
	{
		MessageBox(widget, _T("±£´æDICOMÎÄµµ´íÎó!"), _T("´íÎó"), MB_OK | MB_ICONEXCLAMATION);

		return 0;
	}

	DicomPanel_SetDirty(widget, 0);

	xscpy(pdt->szFile, szFile);

	tchar_t szName[RES_LEN + 1] = { 0 };

	split_file(pdt->szFile, NULL, szName);

	MainFrame_UpdatePanel(g_hMain, widget, szName);

	return 1;
}

void DicomPanel_AdjustCenter(res_win_t widget, int plus)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (!pdt->dicm)
		return;

	long nMax = pdt->nCenter + pdt->nWindow / 2;
	long nMin = pdt->nCenter - pdt->nWindow / 2;

	if (plus > 0 && pdt->nLevel >= nMax)
		return;

	if (plus < 0 && pdt->nLevel <= nMin)
		return;

	int n = (nMax - nMin) / 255;
	if (!n)
		n = 1;

	if (plus > 0)
		pdt->nLevel += n;
	else
		pdt->nLevel -= n;

	if (pdt->nLevel > nMax)
		pdt->nLevel = nMax;
	if (pdt->nLevel < nMin)
		pdt->nLevel = nMin;

	DicomPanel_UpdatePhoto(widget, 0);
}

void DicomPanel_AdjustWindow(res_win_t widget, int plus)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (!pdt->dicm)
		return;

	int nSize = sizeof(ctwin) / sizeof(CT_WINDOW);

	if (plus > 0  && pdt->nIndex >= nSize - 1)
		return;
	if (plus < 0 && pdt->nIndex <= 0)
		return;

	pdt->nIndex += plus;

	if (pdt->nIndex > nSize - 1)
		pdt->nIndex = nSize - 1;
	if (pdt->nIndex < 0)
		pdt->nIndex = 0;

	pdt->nWindow = ctwin[pdt->nIndex].window;
	pdt->nCenter = ctwin[pdt->nIndex].center;
	pdt->nLevel = ctwin[pdt->nIndex].center;

	DicomPanel_UpdatePhoto(widget, 0);
}

void DicomPanel_AppendAnno(res_win_t widget, const tchar_t* mid, const xpoint_t* ppt)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	xpoint_t pt[4] = { 0 };

	DicomPanel_SetDirty(widget, 1);

	LINKPTR ptr_anno = photoctrl_fetch(pdt->hPhoto);

	if (compare_text(mid, -1, DICM_ANNO_TEXT, -1, 0) == 0)
	{
		pt[0].x = ppt->x;
		pt[0].y = ppt->y - 10;
		pt[1].x = ppt->x + 50;
		pt[1].y = ppt->y + 10;

		widget_point_to_tm(pdt->hPhoto, &(pt[0]));
		widget_point_to_tm(pdt->hPhoto, &(pt[1]));

		LINKPTR ilk = insert_anno_spot(ptr_anno, LINK_LAST);
		set_anno_spot_type(ilk, ATTR_ANNO_TYPE_TEXT);
		set_anno_spot_style(ilk, _T("font-size:9;font-weight:700;text-color:RGB(250,0,0);"));
		set_anno_spot_xpoint(ilk, pt, 2);
		set_anno_spot_text(ilk, _T("TEXT"));
	}
	else if (compare_text(mid, -1, DICM_ANNO_CIRCLE, -1, 0) == 0)
	{
		pt[0].x = ppt->x - 10;
		pt[0].y = ppt->y - 10;
		pt[1].x = ppt->x + 10;
		pt[1].y = ppt->y + 10;

		widget_point_to_tm(pdt->hPhoto, &(pt[0]));
		widget_point_to_tm(pdt->hPhoto, &(pt[1]));

		LINKPTR ilk = insert_anno_spot(ptr_anno, LINK_LAST);
		set_anno_spot_type(ilk, ATTR_ANNO_TYPE_CIRCLE);
		set_anno_spot_style(ilk, _T("stroke-color:RGB(250,0,0);"));
		set_anno_spot_xpoint(ilk, pt, 2);
	}
	else if (compare_text(mid, -1, DICM_ANNO_RANGE, -1, 0) == 0)
	{
		pt[0].x = ppt->x;
		pt[0].y = ppt->y;
		pt[1].x = ppt->x + 100;
		pt[1].y = ppt->y;

		widget_point_to_tm(pdt->hPhoto, &(pt[0]));
		widget_point_to_tm(pdt->hPhoto, &(pt[1]));

		LINKPTR ilk = insert_anno_spot(ptr_anno, LINK_LAST);
		set_anno_spot_type(ilk, ATTR_ANNO_TYPE_RANGE);
		set_anno_spot_style(ilk, _T("stroke-color:RGB(250,0,0);font-size:9;font-weight:700;text-color:RGB(250,0,0);"));
		set_anno_spot_xpoint(ilk, pt, 2);
	}
	else if (compare_text(mid, -1, DICM_ANNO_ANGLE, -1, 0) == 0)
	{
		pt[0].x = ppt->x;
		pt[0].y = ppt->y;
		pt[1].x = ppt->x + 50;
		pt[1].y = ppt->y;
		pt[2].x = ppt->x;
		pt[2].y = ppt->y - 50;

		widget_point_to_tm(pdt->hPhoto, &(pt[0]));
		widget_point_to_tm(pdt->hPhoto, &(pt[1]));
		widget_point_to_tm(pdt->hPhoto, &(pt[2]));

		LINKPTR ilk = insert_anno_spot(ptr_anno, LINK_LAST);
		set_anno_spot_type(ilk, ATTR_ANNO_TYPE_ANGLE);
		set_anno_spot_style(ilk, _T("stroke-color:RGB(250,0,0);font-size:9;font-weight:700;text-color:RGB(250,0,0);"));
		set_anno_spot_xpoint(ilk, pt, 3);
	}

	photoctrl_redraw(pdt->hPhoto);
}

int DicomPanel_DrawFace(res_win_t widget, res_ctx_t rdc, const xrect_t* pxr)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);
	xfont_t xf;
	xface_t xa;
	xrect_t xr;
	tchar_t token[PATH_LEN];

	if (!pdt->dicm)
		return 1;

	dicm_summary_t sum = { 0 };
	
	get_dicm_summary(pdt->dicm, &sum);
	
	if (is_null(sum.StudyID))
	{
		return 1;
	}

	default_xfont(&xf);
	default_xface(&xa);

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	xr.x = pxr->x;
	xr.y = pxr->y;
	xr.w = pxr->w;
	xr.h = 20;
	xsprintf(token, _T("%s %s %s"), sum.PatientName, sum.PatientSex, sum.PatientAge);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xr.y += xr.h;
	xsprintf(token, _T("%s %s"), sum.StudyID, sum.StudyDate);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xr.y += xr.h;
	xsprintf(token, _T("%s WL:%s WW:%s"), sum.Modality, sum.WindowCenter, sum.WindowWidth);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xr.x = pxr->x;
	xr.y = pxr->y + pxr->h - 20;
	xr.w = pxr->w;
	xr.h = 20;
	xsprintf(token, _T("Tilt:%s"), sum.GantryDetectorTilt);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xr.y -= xr.h;
	xsprintf(token, _T("%smm"), sum.SliceThickness);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xr.y -= xr.h;
	xsprintf(token, _T("KVP:%s mA:%s"), sum.KVP, sum.XRayTubeCurrent);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xr.y -= xr.h;
	xsprintf(token, _T("%s"), sum.ManufacturersModelName);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_FAR);

	xr.x = pxr->x;
	xr.y = pxr->y;
	xr.w = pxr->w;
	xr.h = 20;
	xsprintf(token, _T("%s"), sum.InstitutionName);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);
	
	xr.y += xr.h;
	xsprintf(token, _T("%s"), sum.Manufacturer);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xr.x = pxr->x;
	xr.y = pxr->y + pxr->h - 20;
	xr.w = pxr->w;
	xr.h = 20;
	xsprintf(token, _T("DFOV:mm"));
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	xr.y -= xr.h;
	xsprintf(token, _T("EXPO:%sms"), sum.ExposureTime);
	draw_text_raw(rdc, &xf, &xa, &xr, token, -1);

	return 0;
}
/***************************************************************************************************************/
void DicomPanel_OnSelectAll(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	photoctrl_redraw(pdt->hPhoto);
}

void DicomPanel_OnDelete(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	photoctrl_redraw(pdt->hPhoto);
}

void DicomPanel_OnCopy(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	widget_copy(pdt->hPhoto);
}

void DicomPanel_OnCut(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	widget_cut(pdt->hPhoto);
}

void DicomPanel_OnPaste(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	widget_paste(pdt->hPhoto);
}

void DicomPanel_OnWindowType(res_win_t widget, const xpoint_t* ppt)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	xrect_t xr = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_DICOMPANEL_WINDOW);
	widget_set_owner(hMenu, widget);

	clr_mod_t clr;
	widget_get_color_mode(widget, &clr);

	widget_set_color_mode(hMenu, &clr);

	LINKPTR ptrMenu = create_menu_doc();
	set_menu_images(ptrMenu, g_ptrImages);
	LINKPTR mlk;

	int n = sizeof(ctwin) / sizeof(CT_WINDOW);
	for (int i = 0; i < n; i++)
	{
		mlk = insert_menu_item(ptrMenu, LINK_LAST);
		set_menu_item_iid(mlk, i + 1);
		set_menu_item_title(mlk, ctwin[i].wname);
		set_menu_item_image(mlk, BMP_GRID);
	}
	
	menubox_set_data(hMenu, ptrMenu);

	menubox_layout(hMenu, ppt, WD_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void DicomPanel_OnAttach(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	tchar_t szPath[PATH_LEN] = { 0 };
	tchar_t szFile[PATH_LEN] = { 0 };
	tchar_t szExt[10] = { 0 };

	const tchar_t* szFilter = _T("JPG File(*.jpg)\0*.jpg\0PNG File(*.png)\0*.png\0Bitmap File(*.bmp)\0*.bmp\0");
	xscpy(szExt, _T("jpg"));

	shell_get_curpath(szPath, PATH_LEN);

	if (!shell_get_filename(widget, szPath, szFilter, szExt, 0, szPath, PATH_LEN, szFile, PATH_LEN))
		return;

	xscat(szPath, _T("\\"));
	xscat(szPath, szFile);

	dword_t size = load_bitmap_bytes_from_file(NULL, szPath, NULL, MAX_LONG);

	byte_t* data = (byte_t*)xmem_alloc(size);

	load_bitmap_bytes_from_file(NULL, szPath, data, size);

	dword_t pdg_len = xpdg_convert(data, size, NULL, MAX_LONG);
	byte_t* pdg_buf = (byte_t*)xmem_alloc(pdg_len);

	xpdg_convert(data, size, pdg_buf, pdg_len);

	set_dicm_pdg(pdt->dicm, pdg_buf, pdg_len);

	xmem_free(pdg_buf);

	photoctrl_set_bitmap(pdt->hPhoto, data, size);

	xmem_free(data);

	photoctrl_redraw(pdt->hPhoto);
}

void DicomPanel_OnDetach(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	photoctrl_redraw(pdt->hPhoto);
}

void DicomPanel_Photo_OnRBClick(res_win_t widget, NOTICE_PHOTO* pnf)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (!is_null(pdt->szFile))
		return;

	xpoint_t* ppt = (xpoint_t*)pnf->data;

	xrect_t xr = { 0 };

	res_win_t hMenu = menubox_create(widget, WD_STYLE_POPUP, &xr);
	widget_set_user_id(hMenu, IDC_DICOMPANEL_MENU);
	widget_set_owner(hMenu, widget);

	clr_mod_t clr;
	widget_get_color_mode(widget, &clr);

	widget_set_color_mode(hMenu, &clr);

	LINKPTR ptrMenu = create_menu_doc();
	set_menu_images(ptrMenu, g_ptrImages);
	LINKPTR mlk;

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_ATTACH);
	set_menu_item_title(mlk, _T("¸½¼ÓÍ¼Ïñ"));
	set_menu_item_image(mlk, BMP_PLUS);

	mlk = insert_menu_item(ptrMenu, LINK_LAST);
	set_menu_item_iid(mlk, IDA_EDIT_DETACH);
	set_menu_item_title(mlk, _T("³·ÀëÍ¼Ïñ"));
	set_menu_item_image(mlk, BMP_MINUS);

	menubox_set_data(hMenu, ptrMenu);

	xpoint_t xp;
	xp.x = ppt->x;
	xp.y = ppt->y;

	widget_client_to_screen(pdt->hPhoto, &xp);

	menubox_layout(hMenu, &xp, WD_LAYOUT_RIGHTBOTTOM);

	widget_do_trace(hMenu);

	destroy_menu_doc(ptrMenu);
}

void DicomPanel_Photo_OnLBClick(res_win_t widget, NOTICE_PHOTO* pnf)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	switch (pdt->nAnno)
	{
	case IDA_ANNOTA_RANGE:
		DicomPanel_AppendAnno(widget, DICM_ANNO_RANGE, (xpoint_t*)pnf->data);
		break;
	case IDA_ANNOTA_ANGLE:
		DicomPanel_AppendAnno(widget, DICM_ANNO_ANGLE, (xpoint_t*)pnf->data);
		break;
	case IDA_ANNOTA_REGION:
		DicomPanel_AppendAnno(widget, DICM_ANNO_CIRCLE, (xpoint_t*)pnf->data);
		break;
	case IDA_ANNOTA_TEXT:
		DicomPanel_AppendAnno(widget, DICM_ANNO_TEXT, (xpoint_t*)pnf->data);
		break;

	}
	pdt->nAnno = 0;
}

void DicomPanel_OnSave(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (!DicomPanel_GetDirty(widget))
		return;

	tchar_t szFile[PATH_LEN] = { 0 };

	if (is_null(pdt->szFile))
	{
		tchar_t szPath[PATH_LEN] = { 0 };

		shell_get_curpath(szPath, PATH_LEN);

		if (!shell_get_filename(widget, szPath, _T("dicom file(*.dcm)\0*.dcm\0dicom file(*.*)\0*.*\0"), _T(""), 1, szPath, PATH_LEN, szFile, PATH_LEN))
			return;

		xscat(szPath, _T("\\"));
		xscat(szPath, szFile);
		xscpy(szFile, szPath);
	}
	else
	{
		xscpy(szFile, pdt->szFile);
	}

	DicomPanel_SaveFile(widget, szFile);
}

void DicomPanel_OnSaveAs(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);
}

void DicomPanel_OnPrint(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);
}

void DicomPanel_OnPreview(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

}

/***********************************************************************************************/

int DicomPanel_OnCreate(res_win_t widget, void* data)
{
	DicomPanelDelta* pdt = (DicomPanelDelta*)xmem_alloc(sizeof(DicomPanelDelta));
	xrect_t xr;

	widget_hand_create(widget);

	SETDICOMPANELDELTA(widget, pdt);

	res_acl_t hac = create_accel_table(DICOMPANEL_ACCEL, DICOMPANEL_ACCEL_COUNT);
	widget_attach_accel(widget, hac);

	LINKPTR ptrSplit = create_split_doc();

	split_item(ptrSplit, 0);
	set_split_item_ratio(ptrSplit, _T("80%"));

	LINKPTR ilkDicom = get_split_first_child_item(ptrSplit);
	LINKPTR ilkProper = get_split_last_child_item(ptrSplit);

	widget_get_client_rect(widget, &xr);
	pdt->hPhoto = photoctrl_create(_T("DicomPanel"), WD_STYLE_CONTROL | WD_STYLE_OWNERNC | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL | WD_STYLE_PAGING | WD_STYLE_NOCHANGE, &xr, widget);

	widget_set_user_id(pdt->hPhoto, IDC_DICOMPANEL_PHOTO);
	widget_set_owner(pdt->hPhoto, widget);

	LINKPTR ptrAnno = create_anno_doc();
	photoctrl_attach(pdt->hPhoto, ptrAnno);

	if_event_t* pev = widget_get_dispatch(pdt->hPhoto);
	pev->pf_on_wheel = DicomPanel_OnImageWheel;

	clr_mod_t clr = { 0 };
	parse_xcolor(&clr.clr_bkg, GDI_ATTR_RGB_BLACK);
	parse_xcolor(&clr.clr_frg, GDI_ATTR_RGB_SOFTBLACK);
	parse_xcolor(&clr.clr_txt, GDI_ATTR_RGB_GRAY);
	parse_xcolor(&clr.clr_msk, GDI_ATTR_RGB_DARKWHITE);
	widget_set_color_mode(pdt->hPhoto, &clr);

	set_split_item_delta(ilkDicom, pdt->hPhoto);
	widget_show(pdt->hPhoto, WD_SHOW_NORMAL);

	widget_get_client_rect(widget, &xr);
	pdt->hProper = properctrl_create(_T("DicomProper"), WD_STYLE_CONTROL | WD_STYLE_OWNERNC | WD_STYLE_VSCROLL | WD_STYLE_NOCHANGE, &xr, widget);
	widget_set_user_id(pdt->hProper, IDC_DICOMPANEL_PROPER);
	widget_set_owner(pdt->hProper, widget);
	widget_set_color_mode(pdt->hProper, &clr);

	set_split_item_delta(ilkProper, pdt->hProper);
	widget_show(pdt->hProper, WD_SHOW_NORMAL);

	LINKPTR ptrProper = create_proper_doc();
	properctrl_attach(pdt->hProper, ptrProper);

	widget_attach_splitor(widget, ptrSplit);

	if (!DicomPanel_OpenFile(widget, (tchar_t*)data))
		return -1;

	photoctrl_redraw(pdt->hPhoto);

	return 0;
}

void DicomPanel_OnDestroy(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	res_acl_t hac = widget_detach_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	link_t_ptr split = widget_detach_splitor(widget);
	if (split)
		destroy_split_doc(split);

	if (widget_is_valid(pdt->hPhoto))
	{
		LINKPTR ptrAnno = photoctrl_detach(pdt->hPhoto);
		if (ptrAnno)
			destroy_anno_doc(ptrAnno);

		widget_destroy(pdt->hPhoto);
	}

	if (widget_is_valid(pdt->hProper))
	{
		LINKPTR ptrProper = properctrl_detach(pdt->hProper);
		if (ptrProper)
			destroy_proper_doc(ptrProper);

		widget_destroy(pdt->hProper);
	}

	if (pdt->dicm)
		destroy_dom_doc(pdt->dicm);

	xmem_free(pdt);

	widget_hand_destroy(widget);
}

int DicomPanel_OnClose(res_win_t widget)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	DicomPanel_Switch(widget);

	return (DicomPanel_GetDirty(widget)) ? 1 : 0;
}

void DicomPanel_OnSetFocus(res_win_t widget, res_win_t hOrg)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (!pdt)
		return;

	if (widget_is_valid(pdt->hPhoto))
	{
		widget_set_focus(pdt->hPhoto);
	}
}

void DicomPanel_OnShow(res_win_t widget, bool_t bShow)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (bShow)
	{
		LINKPTR ptrTool = create_tool_doc();
		LINKPTR glk, ilk;

		tchar_t token[NUM_LEN + 1];

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_EDIT);
		set_tool_group_title(glk, _T("±à¼­"));
		set_tool_group_item_width(glk, MAINFRAME_TOOLGROUP_MINWIDTH);
		set_tool_group_item_height(glk, MAINFRAME_TOOLGROUP_HEIGHT);
		set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_SELECTALL);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("È«Ñ¡"));
		set_tool_item_image(ilk, BMP_SELECTALL);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_DELETE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("É¾³ý"));
		set_tool_item_image(ilk, BMP_DELETE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_COPY);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("¿½±´"));
		set_tool_item_image(ilk, BMP_COPY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_CUT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("¼ôÇÐ"));
		set_tool_item_image(ilk, BMP_CUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_PASTE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("Õ³Ìù"));
		set_tool_item_image(ilk, BMP_PASTE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_EDIT_UNDO);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("³·Ïú"));
		set_tool_item_image(ilk, BMP_UNDO);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_MASK);
		set_tool_group_title(glk, _T("²âÁ¿Óë±ê×¢"));
		set_tool_group_item_width(glk, MAINFRAME_TOOLGROUP_MAXWIDTH);
		set_tool_group_item_height(glk, MAINFRAME_TOOLGROUP_HEIGHT);
		//set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_ANNOTA_ANGLE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("½Ç¶È²âÁ¿"));
		set_tool_item_image(ilk, BMP_ANGLE);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_ANNOTA_RANGE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("¾àÀë²âÁ¿"));
		set_tool_item_image(ilk, BMP_RULER);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_ANNOTA_REGION);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("ÇøÓò±ê×¢"));
		set_tool_item_image(ilk, BMP_STAR);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_ANNOTA_TEXT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("ÎÄ±¾±ê×¢"));
		set_tool_item_image(ilk, BMP_NOTE);

		glk = insert_tool_group(ptrTool, LINK_LAST);
		set_tool_group_name(glk, MAINFRAME_TOOLGROUP_GRAY);
		set_tool_group_title(glk, _T("´°Î»Óë´°¿í"));
		set_tool_group_item_width(glk, MAINFRAME_TOOLGROUP_MAXWIDTH);
		set_tool_group_item_height(glk, MAINFRAME_TOOLGROUP_HEIGHT);
		//set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_WINDOW_TYPE);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("Ñ¡Ôñ´°ÐÍ"));
		set_tool_item_image(ilk, BMP_WINDOW);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_WINDOW_ZOOMOUT);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("Ì§¸ß´°Î»"));
		set_tool_item_image(ilk, BMP_ZOOMOUT);

		ilk = insert_tool_group_item(glk, LINK_LAST);
		xsprintf(token, _T("%d"), IDA_WINDOW_ZOOMIN);
		set_tool_item_id(ilk, token);
		set_tool_item_title(ilk, _T("½µµÍ´°Î»"));
		set_tool_item_image(ilk, BMP_ZOOMIN);

		MainFrame_MergeTool(g_hMain, ptrTool);

		destroy_tool_doc(ptrTool);
	}
	else
	{
		MainFrame_ClearTool(g_hMain);
	}
}

void DicomPanel_OnMenuCommand(res_win_t widget, int code, int cid, void* data)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);
	void* pv = NULL;

	switch (cid)
	{
	case IDA_FILE_SAVE:
		DicomPanel_OnSave(widget);
		break;
	case IDA_FILE_SAVEAS:
		DicomPanel_OnSaveAs(widget);
		break;
	case IDA_FILE_PRINT:
		DicomPanel_OnPrint(widget);
		break;
	case IDA_FILE_PREVIEW:
		DicomPanel_OnPreview(widget);
		break;

	case IDA_EDIT_SELECTALL:
		DicomPanel_OnSelectAll(widget);
		break;
	case IDA_EDIT_DELETE:
		DicomPanel_OnDelete(widget);
		break;
	case IDA_EDIT_COPY:
		DicomPanel_OnCopy(widget);
		break;
	case IDA_EDIT_CUT:
		DicomPanel_OnCut(widget);
		break;
	case IDA_EDIT_PASTE:
		DicomPanel_OnPaste(widget);
		break;
	case IDA_EDIT_ATTACH:
		DicomPanel_OnAttach(widget);
		break;
	case IDA_EDIT_DETACH:
		DicomPanel_OnDetach(widget);
		break;
	case IDA_WINDOW_TYPE:
		DicomPanel_OnWindowType(widget, (xpoint_t*)data);
		break;
	case IDA_WINDOW_ZOOMOUT:
		DicomPanel_AdjustCenter(widget, 1);
		break;
	case IDA_WINDOW_ZOOMIN:
		DicomPanel_AdjustCenter(widget, -1);
		break;

	case IDA_ANNOTA_ANGLE:
	case IDA_ANNOTA_RANGE:
	case IDA_ANNOTA_REGION:
	case IDA_ANNOTA_TEXT:
		pdt->nAnno = cid;
		break;

	case IDC_DICOMPANEL_MENU:
		widget_destroy((res_win_t)data);
		if (code)
		{
			widget_post_command(widget, 0, code, NULL);
		}
		break;
	case IDC_DICOMPANEL_WINDOW:
		widget_destroy((res_win_t)data);
		if (code)
		{
			DicomPanel_AdjustWindow(widget, code - 1 - pdt->nIndex);
		}
		break;
	}
	
}

void DicomPanel_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	if (phdr->id == IDC_DICOMPANEL_PHOTO)
	{
		NOTICE_PHOTO* pnf = (NOTICE_PHOTO*)phdr;
		switch (pnf->code)
		{
		case NC_PHOTOLBCLK:
			DicomPanel_Photo_OnLBClick(widget, pnf);
			break;
		case NC_PHOTORBCLK:
			DicomPanel_Photo_OnRBClick(widget, pnf);
			break;
		case NC_PHOTOFACEDRAW:
			pnf->ret = DicomPanel_DrawFace(widget, pnf->rdc, (xrect_t*)pnf->data);
			break;
		case NC_PHOTOANNOEDITING:
			if (compare_text(get_anno_spot_type_ptr(pnf->spot), -1, ATTR_ANNO_TYPE_TEXT, -1, 0) != 0)
			{
				pnf->ret = 1;
			}
			break;
		}
	}
	else if (phdr->id == IDC_DICOMPANEL_PROPER)
	{
		NOTICE_PROPER* pnp = (NOTICE_PROPER*)phdr;
		switch (pnp->code)
		{
		case NC_PROPERCALCED:
			break;
		case NC_ENTITYCOMMIT:
			break;
		case NC_ENTITYUPDATE:
			break;

		}
	}
}


void DicomPanel_OnKeydown(res_win_t widget, int nKey)
{
	DicomPanelDelta* pdt = GETDICOMPANELDELTA(widget);

	int nSize = sizeof(ctwin) / sizeof(CT_WINDOW);

	switch (nKey)
	{
	case KEY_PAGEUP:
		DicomPanel_AdjustWindow(widget, -1);
		break;
	case KEY_PAGEDOWN:
		DicomPanel_AdjustWindow(widget, 1);
		break;
	case KEY_HOME:
		DicomPanel_AdjustWindow(widget, -nSize); 
		break;
	case KEY_END:
		DicomPanel_AdjustWindow(widget, nSize);
		break;
	}
}

void DicomPanel_OnImageWheel(res_win_t photo, bool_t horz, long delta)
{
	res_win_t widget = widget_get_owner(photo);
	
	if (horz)
		DicomPanel_AdjustWindow(widget, (delta < 0) ? -1 : 1);
	else
		DicomPanel_AdjustCenter(widget, (delta < 0) ? -1 : 1);
}
/*********************************************************************************************************/
res_win_t DicomPanel_Create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, const tchar_t* fpath)
{
	if_event_t ev = { 0 };

	ev.param = (void*)fpath;

	EVENT_BEGIN_DISPATH(&ev)

	EVENT_ON_CREATE(DicomPanel_OnCreate)
	EVENT_ON_DESTROY(DicomPanel_OnDestroy)
	EVENT_ON_CLOSE(DicomPanel_OnClose)
	EVENT_ON_SHOW(DicomPanel_OnShow)

	EVENT_ON_KEYDOWN(DicomPanel_OnKeydown)

	EVENT_ON_NOTICE(DicomPanel_OnNotice)
	EVENT_ON_MENU_COMMAND(DicomPanel_OnMenuCommand)

	EVENT_ON_SPLITOR_IMPLEMENT
	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, g_hMain, &ev);
}