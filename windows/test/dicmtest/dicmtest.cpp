// dicmtest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define SRCFILE		_T("D:\\Easily-app-5\\windows\\bin\\dicom-ct1")
#define XMLFILE		_T("D:\\Easily-app-5\\windows\\bin\\dicom-ct1.xml")
#define BMPFILE		_T("D:\\Easily-app-5\\windows\\bin\\dicom-ct1.bmp")
#define DCMFILE		_T("D:\\Easily-app-5\\windows\\bin\\dicom-ct1.dcm")

void _dicm_to_dom()
{
	XHANDLE xf = xfile_open(NULL, SRCFILE, 0);

	stream_t stm = stream_alloc(xf);

	LINKPTR dom = create_dicm_doc();

	parse_dicm_doc_from_stream(dom, stm);

	stream_free(stm);
	xfile_close(xf);

	save_dom_doc_to_file(dom, NULL, XMLFILE);

	destroy_dicm_doc(dom);
}

void _dom_to_dicm()
{
	LINKPTR dom = create_dicm_doc();

	load_dom_doc_from_file(dom, NULL, XMLFILE);

	XHANDLE xf = xfile_open(NULL, DCMFILE, 1);

	stream_t stm = stream_alloc(xf);

	format_dicm_doc_to_stream(dom, stm);

	stream_free(stm);
	xfile_close(xf);

	destroy_dicm_doc(dom);
}

void _dicm_to_bmp()
{
	XHANDLE xf = xfile_open(NULL, SRCFILE, 0);

	stream_t stm = stream_alloc(xf);

	byte_t** pbuf = xmem_alloc_buffer();

	dword_t dw = parse_dicm_pdg_from_stream(pbuf, stm);

	stream_free(stm);
	xfile_close(xf);

	dword_t len = xpdg_revert(*pbuf, dw, NULL, MAX_LONG);

	byte_t* buf = (byte_t*)xmem_alloc(len);
	xpdg_revert(*pbuf, dw, buf, len);

	xf = xfile_open(NULL, BMPFILE, 1);
	xfile_write(xf, buf, &len);

	xfile_close(xf);

	xmem_free_buffer(pbuf);
	xmem_free(buf);
}

void _bmp_to_dicm()
{
	tchar_t fsize[NUM_LEN];

	xfile_info(NULL, BMPFILE, NULL, fsize, NULL, NULL);
	dword_t size = xstol(fsize);
	byte_t* bmp_buf = (byte_t*)xmem_alloc(size);

	XHANDLE xf = xfile_open(NULL, BMPFILE, 0);
	xfile_read(xf, bmp_buf, &size);
	xfile_close(xf);

	dword_t len = xpdg_convert(bmp_buf, size, NULL, MAX_LONG);

	byte_t* pdg_buf = (byte_t*)xmem_alloc(len);
	xpdg_convert(bmp_buf, size, pdg_buf, len);

	xmem_free(bmp_buf);

	LINKPTR ptr = create_dicm_sop(SOP_CAP);
	set_dicm_pdg(ptr, pdg_buf, len);
	xmem_free(pdg_buf);

	xf = xfile_open(NULL, DCMFILE, 1);
	stream_t stm = stream_alloc(xf);

	format_dicm_doc_to_stream(ptr, stm);

	stream_free(stm);
	xfile_close(xf);

	destroy_dicm_doc(ptr);
}

void _dicm_cap()
{
	XHANDLE xf = xfile_open(NULL, DCMFILE, 0);

	stream_t stm = stream_alloc(xf);

	LINKPTR ptr = create_dicm_doc();

	parse_dicm_doc_from_stream(ptr, stm);

	stream_free(stm);

	xfile_close(xf);

	dword_t dw = get_dicm_pdg(ptr, NULL, MAX_LONG);
	byte_t* pdg_buf = (byte_t*)xmem_alloc(dw);
	get_dicm_pdg(ptr, pdg_buf, dw);

	dword_t len = xpdg_revert(pdg_buf, dw, NULL, MAX_LONG);
	byte_t* bmp_buf = (byte_t*)xmem_alloc(len);
	xpdg_revert(pdg_buf, dw, bmp_buf, len);

	xmem_free(pdg_buf);

	save_dom_doc_to_file(ptr, NULL, XMLFILE);

	destroy_dicm_doc(ptr);

	xf = xfile_open(NULL, BMPFILE, 1);
	xfile_write(xf, bmp_buf, &len);
	xfile_close(xf);

	xmem_free(bmp_buf);
}

void _dicm_query()
{
	XHANDLE xf = xfile_open(NULL, SRCFILE, 0);

	stream_t stm = stream_alloc(xf);

	bool_t b = query_dicm(_T("SOPClassUID=[1.2.840.10008.5.1.4.1.1.1,1.2.840.10008.5.1.4.1.1.2]"), stm);

	stream_free(stm);
	xfile_close(xf);
}
/*
void _format_dicm_anno()
{
	LINKPTR dom = create_dom_doc();

	dicm_to_dom(NULL, SRCFILE, dom);

	dicm_graphic_anno_t ga[2] = { 0 };
	ga[0].count = 1;
	ga[0].points[0].x = 10;
	ga[0].points[0].y = 10;
	xscpy(ga[0].type, _T("POINT"));

	ga[1].count = 2;
	ga[1].points[0].x = 20;
	ga[1].points[0].y = 20;
	ga[1].points[1].x = 40;
	ga[1].points[1].y = 40;
	xscpy(ga[1].type, _T("CIRCLE"));

	dicm_dom_save_graphic_annotation(dom, ga, 2);

	dicm_text_anno_t ta[2] = { 0 };
	ta[0].points[0].x = 100;
	ta[0].points[0].y = 100;
	ta[0].points[1].x = 200;
	ta[0].points[1].y = 120;
	xscpy(ta[0].text, _T("TEXT1"));

	ta[1].points[0].x = 100;
	ta[1].points[0].y = 120;
	ta[1].points[1].x = 200;
	ta[1].points[1].y = 140;
	xscpy(ta[1].text, _T("TEXT2"));

	dicm_dom_save_text_annotation(dom, ta, 2);

	save_dom_doc_to_file(dom, NULL, DSTFILE);

	dicm_from_dom(NULL, DCMFILE, dom);

	destroy_dom_doc(dom);
}

void _dump_dicm()
{
	xhand_t std = xcons_stdout();

	xhand_t stm = stream_alloc(std);
	stream_set_encode(stm, _GB2312);

	dicm_to_stream(NULL, DCMFILE, stm);

	stream_free(stm);
}

void _dicm_bmp()
{
	load_bitmap_from_file()
}
*/

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(0);

	//_dicm_to_dom();

	//_dom_to_dicm();

	//_dicm_to_bmp();

	//_bmp_to_dicm();

	//_dicm_cap();

	_dicm_query();

	//getch();

	xdl_process_uninit();

	return 0;
}

/*
HDC hdc;
int ibits;//当前显示模式下每个像素所占字节数
WORD  wbitsCount;//位图中每个像素所占字节数。
DWORD dwpalettelsize = 0;//调色板大小
DWORD dwbmdibitsize, dwdibsize, dwwritten;
BITMAP bitmap;
BITMAPFILEHEADER bmfhdr;
BITMAPINFOHEADER bi;
LPBITMAPINFOHEADER lpbi;
HANDLE fh, fdib, hpal, holdpal = NULL;
CString work;
hdc = CreateDC("display", NULL, NULL, NULL);
ibits = ::GetDeviceCaps(hdc, BITSPIXEL)*GetDeviceCaps(hdc, PLANES);
if (ibits <= 1)
wbitsCount = 1;
if (ibits <= 4)
wbitsCount = 4;
if (ibits <= 8)
wbitsCount = 8;
if (ibits <= 16)
wbitsCount = 16;
if (ibits <= 24)
wbitsCount = 24;
else
wbitsCount = 32;
//以上代码中ibits返回的显示模式下，每个像素所占的位数，wbitsCount是位图上每个像素所占的位数。
if (wbitsCount <= 8)
dwpalettelsize = (1 << wbitsCount)*sizeof(RGBQUAD);
GetObject(hBitmap, sizeof(BITMAP), (void *)&bitmap);//得到BITMAP的信息，这是一个BITMAP结构。
bi.biSize = sizeof(BITMAPINFOHEADER);
bi.biWidth = bitmap.bmWidth;
bi.biHeight = bitmap.bmHeight;
bi.biPlanes = 1;
bi.biBitCount = wbitsCount;
bi.biClrImportant = 0;
bi.biClrUsed = 0;
bi.biCompression = BI_RGB;
bi.biSizeImage = 0;
bi.biYPelsPerMeter = 0;
bi.biXPelsPerMeter = 0;
dwbmdibitsize = ((bitmap.bmWidth*wbitsCount + 31) / 8)*bitmap.bmHeight;
fdib = GlobalAlloc(GHND, dwbmdibitsize + dwpalettelsize + sizeof(BITMAPINFOHEADER));
lpbi = (LPBITMAPINFOHEADER)::GlobalLock(fdib);
*lpbi = bi;//将bi中的数据写入分配的内存中。
//处理调色板
hpal = ::GetStockObject(DEFAULT_PALETTE);
if (hpal)
{
	hdc = ::GetDC(NULL);
	holdpal = ::SelectPalette(hdc, (HPALETTE)hpal, false);
	::RealizePalette(hdc);
}
GetDIBits(hdc, hBitmap, 0, (UINT)bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwpalettelsize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
if (holdpal)
{
	::SelectPalette(hdc, (HPALETTE)holdpal, true);
	::RealizePalette(hdc);
	::ReleaseDC(NULL, hdc);
}
//创建文件
fh = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
if (fh == INVALID_HANDLE_VALUE)
return FALSE;
bmfhdr.bfType = 0x4d42;
dwdibsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwbmdibitsize + dwpalettelsize;
bmfhdr.bfSize = dwdibsize;
bmfhdr.bfReserved1 = 0;
bmfhdr.bfReserved2 = 0;
bmfhdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwpalettelsize;
::WriteFile(fh, (LPSTR)&bmfhdr, sizeof(BITMAPFILEHEADER), &dwwritten, NULL);
::WriteFile(fh, (LPSTR)lpbi, dwdibsize, &dwwritten, NULL);
::GlobalUnlock(fdib);
::GlobalFree(fdib);
::CloseHandle(fh);
*/