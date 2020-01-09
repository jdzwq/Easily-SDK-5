/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdl error function

	@module	xerr.c | xdl error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "firememo.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlctx.h"

int firememo_on_keydown(widget_t_ptr ptd,event_t_ptr pev)
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

widget_t* firememo_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner)
{
	widget_t* pwd;

	pwd = widget_alloc(UN("EDIT"),wname,id,WIDGET_CHILD,x,y,width,height,owner);
	pwd->style |= (ES_MULTILINE | ES_WANTRETURN);
	
	pwd->pf_on_keydown = (on_event)firememo_on_keydown;

	if(!widget_create(pwd))
	{
		widget_free(pwd);
		return NULL;
	}

	return pwd;
}

void firememo_destroy(widget_t* pwd)
{
	if(!pwd)
		return;

	widget_destroy(pwd);

	widget_free(pwd);
}


void firememo_set_text(widget_t* pwd,const wchar_t* text,int len)
{
	wchar_t * token;

	assert(pwd);

	if(!IsWindow((HWND)pwd->self))
		return;

	if(len < 0)
	{
		len = xslen(text);
		token = (wchar_t*)xmem_alloc((len + 1) * sizeof(wchar_t));
		xsncpy(token,text,len);

		SendMessage((HWND)pwd->self,WM_SETTEXT,0,(LPARAM)token);

		xmem_free(token);
	}else
		SendMessage((HWND)pwd->self,WM_SETTEXT,0,(LPARAM)text);
}

int firememo_get_text(widget_t* pwd,wchar_t* buf,int max)
{
	assert(pwd);

	if(!IsWindow((HWND)pwd->self))
		return 0;

	return (int)SendMessage((HWND)pwd->self,WM_GETTEXT,(WPARAM)(max + 1),(LPARAM)buf);
}