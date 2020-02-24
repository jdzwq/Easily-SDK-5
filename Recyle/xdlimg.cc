/*************************************************************
	EasySoft xdc v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc imagelist

	@module	imagelist.c | imagelist implement file

	@devnote ’≈Œƒ»® 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlimg.h"
#include "xdlmem.h"
#include "xdlstr.h"
#include "xdlfile.h"

handle_t load_image_from_file(const wchar_t* filename,int width,int height)
{
	HANDLE handle;
	IPicture* p = NULL;
	IStream* s = NULL;
	HGLOBAL hb = NULL;
	void* buf = NULL;
	size_t size;
	handle_t fh;
	
	if(is_null(filename))
		return 0;
	
	fh = xfile_open(filename,UN("r"));
	if(fh == NULL)
		return 0;

	size = xfile_size(fh);
	if(size < 0)
	{
		xfile_close(fh);
		return 0;
	}

	hb = GlobalAlloc(GHND,size);
	buf = GlobalLock(hb);
	xfile_read(fh,buf,size);
	xfile_close(fh);

	if(!hb)
		return 0;
	
	CreateStreamOnHGlobal((HGLOBAL)buf,FALSE,&s);
	if(s == NULL)
	{
		GlobalUnlock(hb);
		GlobalFree(hb);
		return 0;
	}

	OleLoadPicture(s,0,FALSE,IID_IPicture,(void**)&p);
	s->Release();
	GlobalUnlock(hb);
	GlobalFree(hb);
	
	if(p == NULL)
		return NULL;

	p->get_Handle((OLE_HANDLE *)&handle);

	if(width > 0 && height> 0)
		handle = (HANDLE)CopyImage(handle,IMAGE_BITMAP,width,height,LR_CREATEDIBSECTION);
	else
		handle = (HANDLE)CopyImage(handle,IMAGE_BITMAP,width,height,LR_CREATEDIBSECTION | LR_COPYRETURNORG);

	p->Release();

	return (handle_t)handle;
}

handle_t load_image_from_res(const wchar_t* resname,int width,int height)
{
	return (handle_t)LoadImage(NULL,MAKEINTRESOURCE(resname),IMAGE_BITMAP,width,height,LR_CREATEDIBSECTION);
}



void free_image(handle_t ih)
{
	DeleteObject((HBITMAP)ih);
}

