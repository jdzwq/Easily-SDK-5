/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdl error function

	@module	xerr.c | xdl error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "firedate.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlctx.h"
#include "xdlutil.h"


int firedate_on_keydown(widget_t_ptr ptd,event_t_ptr pev)
{
	NMHDR hdr = {0};

	if((int)pev->wp == 13 || (int)pev->wp == 9)
	{
		hdr.hwndFrom = (HWND)ptd->self;
		hdr.idFrom = (UINT)ptd->id;
		hdr.code = WIDGET_NOTIFY_ACCEPT;

		SendMessage((HWND)ptd->owner,WM_NOTIFY,(WPARAM)ptd->id,(LPARAM)&hdr);
		return 0;
	}else if((int)pev->wp == 27)
	{
		hdr.hwndFrom = (HWND)ptd->self;
		hdr.idFrom = (UINT)ptd->id;
		hdr.code = WIDGET_NOTIFY_CANCEL;

		SendMessage((HWND)ptd->owner,WM_NOTIFY,(WPARAM)ptd->id,(LPARAM)&hdr);
		return 0;
	}

	return 0;
}

widget_t* firedate_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner)
{
	widget_t* pwd;
	
	pwd = widget_alloc(DATETIMEPICK_CLASS,wname,id,WIDGET_CHILD,x,y,width,height,owner);

	pwd->pf_on_keydown = (on_event)firedate_on_keydown;

	if(!widget_create(pwd))
	{
		widget_free(pwd);
		return NULL;
	}

	return pwd;
}

void firedate_destroy(widget_t* pwd)
{
	int count,i;
	wchar_t* data;

	if(!pwd)
		return;

	if(!IsWindow((HWND)pwd->self))
		return;

	count = (int)SendMessage((HWND)pwd->self,CB_GETCOUNT,0,0);
	for(i=0;i<count;i++)
	{
		data = (wchar_t*)SendMessage((HWND)pwd->self,CB_GETITEMDATA,(WPARAM)i,0);
		xmem_free(data);
	}

	widget_destroy(pwd);

	widget_free(pwd);
}


void firedate_set_datetime(widget_t* pwd,const xdate_t* pdt)
{
	SYSTEMTIME st = {0};

	assert(pwd);

	if(!IsWindow((HWND)pwd->self))
		return;

	st.wYear = pdt->year;
	st.wMonth = pdt->mon;
	st.wDay = pdt->day;
	st.wHour = pdt->hour;
	st.wMinute = pdt->min;
	st.wSecond = pdt->sec;

	SendMessage((HWND)pwd->self,DTM_SETSYSTEMTIME,0,(LPARAM)&st);
}

void firedate_get_datetime(widget_t* pwd,xdate_t* pdt)
{
	SYSTEMTIME st = {0};

	assert(pwd);

	if(!IsWindow((HWND)pwd->self))
		return;

	SendMessage((HWND)pwd->self,DTM_GETSYSTEMTIME,0,(LPARAM)&st);
	
	pdt->year = st.wYear;
	pdt->mon = st.wMonth;
	pdt->day = st.wDay;
	pdt->hour = st.wHour;
	pdt->min = st.wMinute;
	pdt->sec = st.wSecond;
}
