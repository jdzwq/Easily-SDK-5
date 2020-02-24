/*************************************************************
	EasySoft xdr v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdr error function

	@module	xerr.c | xdr error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlctx.h"
#include "xdlmem.h"
#include "xdlstr.h"


typedef struct _dispaly_context_t{
	int ct;
	handle_t hdc;
}display_context_t;

context_t* create_dispaly_context(const wchar_t* devname)
{
	display_context_t *pdev;
	
	pdev = (display_context_t*)xmem_alloc(sizeof(display_context_t));
	pdev->ct = CONTEXT_DISPLAY;
	pdev->hdc = (handle_t)CreateDC(NULL,((devname)? devname : TEXT("DISPALY")),NULL,NULL);
	assert(pdev->hdc);

	return (context_t*)pdev;
}

void destroy_dispaly_context(context_t* pct)
{
	display_context_t *pdev;
	
	assert(pct->ct == CONTEXT_DISPLAY);

	pdev = (display_context_t*)pct;
	if(pdev->hdc)
		DeleteDC((HDC)pdev->hdc);

	xmem_free(pdev);
}


/**************************************************************************************************/

typedef struct _printer_context_t{
	int ct;
	handle_t hdc;
}printer_context_t;

context_t* create_printer_context(const wchar_t* devname)
{
	printer_context_t *pdc;
	HDC hDC;
	TCHAR* tmp;
	HANDLE hPrinter;
	DEVMODE* pdev;
	long size;
	wchar_t szPrinter[50] = {0};

	if(devname && w_xslen(devname))
	{
		w_xscpy(szPrinter,devname);
	}else
	{
		GetProfileString(UN("windows"),UN("device"),UN(""),szPrinter,49);
		tmp = w_xsstr(szPrinter,UN(","));
		if(tmp != NULL)
			*tmp = UN('\0');
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

	/*size = 0;
	if(width > 0 && height > 0)
	{
		pdev->dmFields |= (DM_PAPERSIZE | DM_PAPERWIDTH | DM_PAPERLENGTH);
		pdev->dmPaperSize = DMPAPER_USER;
		pdev->dmPaperWidth = width * 10;
		pdev->dmPaperLength = height * 10;
		size = 1;
	}
	if(orient)
	{
		pdev->dmFields |= DM_ORIENTATION;
		pdev->dmOrientation = orient;
		size = 1;
	}
	
	if(size)
		size = DocumentProperties(NULL,hPrinter,szPrinter,pdev,pdev,DM_OUT_BUFFER | DM_IN_BUFFER);
	*/

	ClosePrinter(hPrinter);

	hDC = CreateDC(UN("WINSPOOL"),szPrinter,NULL,pdev);
	GlobalUnlock(pdev);
	GlobalFree(pdev);

	pdc = (printer_context_t*)xmem_alloc(sizeof(printer_context_t));
	pdc->ct = CONTEXT_PRINTER;
	pdc->hdc = (handle_t)hDC;

	return (context_t*)pdev;
}

void destroy_printer_context(context_t* pct)
{
	printer_context_t *pdev;
	
	assert(pct->ct == CONTEXT_PRINTER);

	pdev = (printer_context_t*)pct;
	if(pdev->hdc)
		DeleteDC((HDC)pdev->hdc);

	xmem_free(pdev);
}

/***********************************************************************************************************/
int mm_to_pt(const context_t* pct,float mm,bool_t horz)
{
	HDC hDC = get_windows_dc(pct);

	switch(GetMapMode(hDC))
	{
	case MM_HIENGLISH:
		return (int)(mm * INCHPERMM * 1000.0);
	case MM_LOENGLISH:
		return (int)(mm * INCHPERMM * 100.0);
	case MM_HIMETRIC:
		return (int)(mm * 100.0);
	case MM_LOMETRIC:
		return (int)(mm * 10.0);
	case MM_TWIPS:
		return (int)(mm * INCHPERMM * PTPERINCH);
	default:
		return (int)(GetDeviceCaps(hDC,HORZRES) / GetDeviceCaps(hDC,HORZSIZE) * mm);
	}
}

float pt_to_mm(const context_t* pct,int pt,bool_t horz)
{
	HDC hDC = get_windows_dc(pct);

	switch(GetMapMode(hDC))
	{
	case MM_HIENGLISH:
		return (float)((pt / 1000.0) / MMPERINCH);
	case MM_LOENGLISH:
		return (float)((pt / 100.0) / MMPERINCH);
	case MM_HIMETRIC:
		return (float)(pt / 100.0);
	case MM_LOMETRIC:
		return (float)(pt / 10.0);
	case MM_TWIPS:
		return (float)((pt / 1440) / MMPERINCH);
	default:
		return (float)(pt / (GetDeviceCaps(hDC,HORZRES) / GetDeviceCaps(hDC,HORZSIZE)));
	}
}