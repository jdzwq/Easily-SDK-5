/***********************************************************************
	Easily xdc v4.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc device context document

	@module	if_context_win.c | device context windows implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 -			v4.5
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

#include "_if_xdc.h"

res_dc_t __cdecl _create_display_context(void)
{
	return CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
}

res_dc_t __cdecl _create_compatible_context(res_dc_t rdc)
{
	HDC hdc;

	hdc = CreateCompatibleDC(rdc);
	if (hdc)
		SetBkMode(hdc, TRANSPARENT);

	return hdc;
}

void __cdecl _delete_context(res_dc_t rdc)
{
	DeleteDC(rdc);
}

void __cdecl _get_device_caps(res_dc_t rdc, dev_cap_t* pcap)
{
	pcap->horz_res = GetDeviceCaps(rdc, HORZRES);
	pcap->vert_res = GetDeviceCaps(rdc, VERTRES);

	pcap->horz_pixels = GetDeviceCaps(rdc, LOGPIXELSX);
	pcap->vert_pixels = GetDeviceCaps(rdc, LOGPIXELSY);

	pcap->horz_size = GetDeviceCaps(rdc, PHYSICALWIDTH);
	pcap->vert_size = GetDeviceCaps(rdc, PHYSICALHEIGHT);

	pcap->horz_feed = GetDeviceCaps(rdc, PHYSICALOFFSETX);
	pcap->vert_feed = GetDeviceCaps(rdc, PHYSICALOFFSETY);
}

res_obj_t __cdecl _select_object(res_dc_t rdc, res_obj_t obj)
{
	return SelectObject(rdc, obj);
}

void __cdecl _render_context(res_dc_t src, long srcx, long srcy, res_dc_t dst, long dstx, long dsty, long dstw, long dsth)
{
	BitBlt(dst, dstx, dsty, dstw, dsth, src, srcx, srcy, SRCCOPY);
}

#ifdef XDC_SUPPORT_CONTEXT_PRINTER

res_dc_t __cdecl _create_printer_context(const dev_mod_t* pmod)
{
	HDC hDC;
	HANDLE hPrinter;
	DEVMODE* pdev;
	long size;
	tchar_t szPrinter[PATH_LEN] = {0};
	tchar_t* tmp;
	
	if(pmod && xslen(pmod->devname))
	{
		_tcscpy(szPrinter,pmod->devname);
	}else
	{
		GetProfileString(_T("windows"),_T("device"),_T(""),szPrinter, PATH_LEN);
		tmp = _tcsstr(szPrinter,_T(","));
		if(tmp != NULL)
			*tmp = _T('\0');
	}

	if(!OpenPrinter(szPrinter,&hPrinter,NULL))
		return 0;

	size = DocumentProperties(NULL,hPrinter,szPrinter,NULL,NULL,0);
	if(size < 0)
	{
		ClosePrinter(hPrinter);
		return 0;
	}

	pdev = (DEVMODE*)GlobalLock(GlobalAlloc(GHND,size));
	pdev->dmSize = sizeof(DEVMODE);

	size = DocumentProperties(NULL,hPrinter,szPrinter,pdev,NULL,DM_OUT_BUFFER);
	if(size < 0)
	{
		ClosePrinter(hPrinter);
		GlobalUnlock(pdev);
		GlobalFree(pdev);
		return 0;
	}

	size = 0;
	if (pmod && pmod->paper_width > 0 && pmod->paper_height > 0)
	{
		pdev->dmFields |= (DM_PAPERSIZE | DM_PAPERWIDTH | DM_PAPERLENGTH);
		pdev->dmPaperSize = DMPAPER_USER;
		pdev->dmPaperWidth = pmod->paper_width;
		pdev->dmPaperLength = pmod->paper_height;
		size = 1;
	}

	if(pmod && pmod->b_landscape)
	{
		pdev->dmFields |= DM_ORIENTATION;
		pdev->dmOrientation = DMORIENT_LANDSCAPE;
		size = 1;
	}
	
	if(size)
	{
		size = DocumentProperties(NULL,hPrinter,szPrinter,pdev,pdev,DM_OUT_BUFFER | DM_IN_BUFFER);
	}

	ClosePrinter(hPrinter);

	hDC = CreateDC(_T("WINSPOOL"),szPrinter,NULL,pdev);

	GlobalUnlock(pdev);
	GlobalFree(pdev);

	return hDC;
}

void __cdecl _begin_page(res_dc_t rdc)
{
	StartPage(rdc);
}

void __cdecl _end_page(res_dc_t rdc)
{
	EndPage(rdc);
}

void __cdecl _begin_doc(res_dc_t rdc,const tchar_t* docname)
{
	DOCINFO df = {0};

	df.cbSize = sizeof(df);
	df.fwType = 0;
	df.lpszDatatype = NULL;
	df.lpszDocName = (LPTSTR)docname;
	df.lpszOutput = NULL;

	StartDoc(rdc,&df);
}

void __cdecl _end_doc(res_dc_t rdc)
{
	EndDoc(rdc);
}

bool_t __cdecl _default_printer_mode(dev_mod_t* pmod)
{
	TCHAR szPrinter[MAX_PATH];
	PRINTER_DEFAULTS pdf;
	HANDLE hPrinter;
	DEVMODE* pdev = NULL;

	TCHAR* tmp;
	long size;

	_tcscpy(szPrinter, _T(""));
	GetProfileString(_T("windows"), _T("device"), _T(""), szPrinter, MAX_PATH);

	tmp = _tcsstr(szPrinter, _T(","));
	if (tmp != NULL)
		*tmp = '\0';

	pdf.pDatatype = NULL;
	pdf.pDevMode = NULL;
	pdf.DesiredAccess = PRINTER_ACCESS_USE;
	if (!OpenPrinter(szPrinter, &hPrinter, NULL))
		return 0;

	size = DocumentProperties(NULL, hPrinter, szPrinter, NULL, NULL, 0);
	if (size < 0)
	{
		ClosePrinter(hPrinter);
		return 0;
	}

	pdev = (DEVMODE*)GlobalLock(GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, size));
	pdev->dmSize = sizeof(DEVMODE);//(WORD)size;

	size = DocumentProperties(NULL, hPrinter, szPrinter, pdev, NULL, DM_OUT_BUFFER);

	pmod->b_landscape = (pdev->dmOrientation == DMORIENT_LANDSCAPE) ? 1 : 0;
	pmod->paper_width = pdev->dmPaperWidth;
	pmod->paper_height = pdev->dmPaperLength;

	ClosePrinter(hPrinter);

	GlobalUnlock(pdev);
	GlobalFree(pdev);

	return 1;
}

bool_t __cdecl _setup_printer_mode(res_win_t wnd, dev_mod_t* pmod)
{
	TCHAR szPrinter[MAX_PATH];
	PRINTER_DEFAULTS pdf;
	HANDLE hPrinter;
	DEVMODE* pdev = NULL;

	TCHAR* tmp;
	long size;

	if (pmod->devname[0] != _T('\0'))
	{
		_tcscpy(szPrinter, (TCHAR*)pmod->devname);
	}
	else
	{
		_tcscpy(szPrinter, _T(""));
		GetProfileString(_T("windows"), _T("device"), _T(""), szPrinter, MAX_PATH);

		tmp = _tcsstr(szPrinter, _T(","));
		if (tmp != NULL)
			*tmp = '\0';
	}

	pdf.pDatatype = NULL;
	pdf.pDevMode = NULL;
	pdf.DesiredAccess = PRINTER_ACCESS_USE;
	if (!OpenPrinter(szPrinter, &hPrinter, NULL))
		return 0;

	size = DocumentProperties(wnd, hPrinter, szPrinter, NULL, NULL, 0);
	if (size < 0)
	{
		ClosePrinter(hPrinter);
		return 0;
	}

	pdev = (DEVMODE*)GlobalLock(GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, size));
	pdev->dmSize = sizeof(DEVMODE);//(WORD)size;

	size = DocumentProperties(wnd, hPrinter, szPrinter, pdev, pdev, DM_IN_PROMPT | DM_IN_BUFFER | DM_OUT_BUFFER);
	
	pmod->b_landscape = (pdev->dmOrientation == DMORIENT_LANDSCAPE) ? 1 : 0;
	pmod->paper_width = pdev->dmPaperWidth;
	pmod->paper_height = pdev->dmPaperLength;

	ClosePrinter(hPrinter);

	GlobalUnlock(pdev);
	GlobalFree(pdev);

	return 1;
}

#endif //XDC_SUPPORT_CONTEXT_PRINTER