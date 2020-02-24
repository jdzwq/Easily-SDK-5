/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc xdl error function

	@module	xerr.c | xdl error implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "firelist.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlctx.h"
#include "xdlutil.h"


int firelist_on_keydown(widget_t_ptr ptd,event_t_ptr pev)
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

widget_t* firelist_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner)
{
	widget_t* pwd;
	
	pwd = widget_alloc(UN("COMBOBOX"),wname,id,WIDGET_CHILD,x,y,width,height,owner);
	pwd->style |= CBS_DROPDOWNLIST;

	pwd->pf_on_keydown = (on_event)firelist_on_keydown;

	if(!widget_create(pwd))
	{
		widget_free(pwd);
		return NULL;
	}

	return pwd;
}

void firelist_destroy(widget_t* pwd)
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


void firelist_set_options(widget_t* pwd,const wchar_t* options,int len, wchar_t itemfeed, wchar_t linefeed)
{
	wchar_t *key,*val;
	int klen,vlen;
	wchar_t* token;
	wchar_t *data,*label;
	int index;

	assert(pwd);

	if(!IsWindow((HWND)pwd->self))
		return;

	token = (wchar_t*)options;
	while(token = (wchar_t*)parse_options(token,-1,itemfeed,linefeed,&key,&klen,&val,&vlen))
	{
		data = (wchar_t*)xmem_alloc((klen + 1) * sizeof(wchar_t));
		xsncpy(data,key,klen);

		label = (wchar_t*)xmem_alloc((vlen + 1) * sizeof(wchar_t));
		xsncpy(label,val,vlen);

		index = (int)SendMessage((HWND)pwd->self,CB_ADDSTRING,0,(LPARAM)label);
		SendMessage((HWND)pwd->self,CB_SETITEMDATA,(WPARAM)index,(LPARAM)data);

		xmem_free(label);
	}
}
