/*************************************************************
	EasySoft xdc v4.0

	(c) 2003-2007 EasySoft Corporation.  All Rights Reserved.

	@doc imagelist

	@module	imagelist.c | imagelist implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2007.12
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlimg.h"
#include "xdlfile.h"
#include "xdlstr.h"
#include "xdlutil.h"
#include "xdlmem.h"
#include "xdlcode.h"

void free_image(res_bmp_t ih)
{
	DeleteObject((HBITMAP)ih);
}

res_bmp_t load_image_from_bytes(const byte_t* pb, int bytes, int width, int height)
{
	HANDLE handle;
	IPicture* ip = NULL;
	IStream* is = NULL;

	CreateStreamOnHGlobal((HGLOBAL)pb, FALSE, &is);
	if (is == NULL)
	{
		return 0;
	}

	OleLoadPicture(is, 0, FALSE, IID_IPicture, (void**)&ip);
	is->Release();

	if (ip == NULL)
		return NULL;

	ip->get_Handle((OLE_HANDLE *)&handle);

	if (width > 0 && height> 0)
		handle = (HANDLE)CopyImage((HANDLE)handle, IMAGE_BITMAP, width, height, LR_CREATEDIBSECTION);
	else
		handle = (HANDLE)CopyImage((HANDLE)handle, IMAGE_BITMAP, width, height, LR_CREATEDIBSECTION | LR_COPYRETURNORG);

	ip->Release();

	return (res_bmp_t)handle;
}

int get_image_bytes(res_bmp_t rb)
{
	BITMAP bmp;
	WORD cClrBits;
	DWORD dwClrUsed;
	DWORD dwSizeImage;
	DWORD dwTotal;

	if (!GetObject(rb, sizeof(BITMAP), (LPSTR)&bmp))
		return 0;

	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else
		cClrBits = 32;

	if (cClrBits < 24)
		dwClrUsed = (1 << cClrBits);
	else
		dwClrUsed = 0;

	dwSizeImage = ((bmp.bmWidth * cClrBits + 31) & ~31) / 8 * bmp.bmHeight;

	dwTotal = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwClrUsed * sizeof(RGBQUAD) + dwSizeImage);

	return dwTotal;
}

res_bmp_t load_image_from_bytes(const byte_t* pb, int bytes)
{
	HBITMAP rb;
	PBITMAPINFO pbmi;
	BITMAPFILEHEADER bfh;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD dwOffset;
	HDC hDC;

	if (bytes < sizeof(BITMAPFILEHEADER))
		return 0;

	dwOffset = 0;
	memcpy((void*)&bfh, (void*)(pb + dwOffset), sizeof(BITMAPFILEHEADER));
	dwOffset += sizeof(BITMAPFILEHEADER);

	if (bfh.bfType != 0x4d42)
		return 0;

	if (bytes < (int)bfh.bfSize)
		return 0;

	pbmi = (PBITMAPINFO)(pb + dwOffset);
	pbih = (PBITMAPINFOHEADER)pbmi;
	lpBits = (LPBYTE)(pb + bfh.bfOffBits);

	hDC = GetDC(NULL);
	rb = CreateDIBitmap(hDC, pbih, CBM_INIT, lpBits, pbmi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hDC);

	return (res_bmp_t)rb;
}

int save_image_to_bytes(res_bmp_t rb, byte_t* buf, int max)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;
	BITMAPFILEHEADER bfh;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD dwTotal;
	HDC hDC;

	if (!GetObject(rb, sizeof(BITMAP), (LPSTR)&bmp))
		return 0;

	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else 
		cClrBits = 32;

	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)xmem_alloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
	else
		pbmi = (PBITMAPINFO)xmem_alloc(sizeof(BITMAPINFOHEADER));

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
	else
		pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 * pbmi->bmiHeader.biHeight;
	pbmi->bmiHeader.biClrImportant = 0;

	pbih = (PBITMAPINFOHEADER)pbmi;

	bfh.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
	bfh.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD));

	if (pbih->biSizeImage > (DWORD)max)
	{
		xmem_free(pbmi);
		return 0;
	}

	dwTotal = 0;
	memcpy((void*)(buf + dwTotal), (void*)&bfh, sizeof(BITMAPFILEHEADER));
	dwTotal += sizeof(BITMAPFILEHEADER);
	
	memcpy((void*)(buf + dwTotal), (void*)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD));
	dwTotal += sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD);

	lpBits = (LPBYTE)(buf + dwTotal);
	dwTotal += pbih->biSizeImage;

	hDC = GetDC(NULL);
	if (!GetDIBits(hDC, rb, 0, (WORD)pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS))
	{
		ReleaseDC(NULL, hDC);
		xmem_free(pbmi);
		return 0;
	}
	ReleaseDC(NULL, hDC);

	xmem_free(pbmi);

	return dwTotal;
}

res_bmp_t load_image_from_file(const wchar_t* filename)
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
	
	fh = xfile_open(filename,L"r");
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

	xfile_read(fh,(byte_t*)buf,size);
	xfile_close(fh);

	if(!hb)
		return 0;
	
	CreateStreamOnHGlobal(hb,FALSE,&s);
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

	handle = (HANDLE)CopyImage(handle,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION | LR_COPYRETURNORG);

	p->Release();

	return (res_bmp_t)handle;
}

typedef struct _bitmap_filehead_t
{
	sword_t type;		//bitmap file type
	dword_t size;		//bitmap file size
	sword_t reserved1;
	sword_t reserved2;
	dword_t offset;		//bitmap data offset from file header
}bitmap_filehead_t;		//14 bytes

typedef struct _bitmap_infohead_t{
	dword_t size;		//struct size
	dword_t width;			//bitmap point width 
	dword_t height;		//bitmap point height
	sword_t planes;		//number of planes for the target device, set to 1 
	sword_t bitcount;	//the number of bits-per-pixel. 1:is monochrome; 4:maximum of 16 colors; 8:maximum of 256 colors; 16:maximum of 2^16 colors; 24~; 32~;
	dword_t compression; //type of compression.0: uncompressed format; 1: RLE format for bitmaps with 8 bpp; 2:RLE format for bitmaps with 4 bpp. 
	dword_t imagesize;	// the size, in bytes, of the image
	dword_t horzpixels;	//the horizontal resolution, in pixels-per-meter
	dword_t vertpixels;	//the vertical resolution, in pixels-per-meter
	dword_t clrused;	// the number of color indexes in the color table  that are actually used by the bitmap
	dword_t clrimportant;//the number of color indexes that are required for displaying the bitmap
}bitmap_info_t;			//40 bytes

typedef struct _bitmap_rgbquad_t{
	byte_t blue;		//blue lighten(0-255) 
	byte_t green;		//green lighten(0-255) 
	byte_t red;			//red lighten(0-255) 
	byte_t reserved;	//set to zero
}bitmap_rgbquad_t;






