/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdl error function

	@module	xerr.c | xdl error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "fireedit.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlctx.h"

int fireedit_on_keydown(widget_t_ptr ptd,event_t_ptr pev)
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

widget_t* fireedit_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner)
{
	widget_t* pwd;

	pwd = widget_alloc(UN("EDIT"),wname,id,WIDGET_CHILD,x,y,width,height,owner);
	
	pwd->pf_on_keydown = (on_event)fireedit_on_keydown;

	if(!widget_create(pwd))
	{
		widget_free(pwd);
		return NULL;
	}

	return pwd;
}

void fireedit_destroy(widget_t* pwd)
{
	if(!pwd)
		return;

	widget_destroy(pwd);

	widget_free(pwd);
}
