/*************************************************************
	Easily xdc v4.0

	(c) 2003-2013 Easily Corporation.  All Rights Reserved.

	@doc xdc document

	@module	xdcutil.h | utility interface file

	@devnote 张文权 2003.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 -			v4.0
*************************************************************/


#ifndef _XDCUTIL_H
#define _XDCUTIL_H

#include "xdcdef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UINT PROPERCLIPFORMAT;

XDC_API void XdcInitControl();

XDC_API void XdcUnInitControl();

XDC_API void XdcTrackMenu(HWND hOwner, LINKPTR ptrMenu, POINT* lppt);

XDC_API HFONT XdcCreateFont(const xfont_t* pxf);

XDC_API void XdcEnsureVisible(HWND hWnd,const RECT* prt);

XDC_API void CenterRect(RECT* prt,int cx,int cy);
XDC_API void MergeRect(RECT* prt,const RECT* pr1,const RECT* pr2);

#ifdef __cplusplus
}
#endif

#endif