// xdl_bmp_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define RGB_GRAY(r,g,b) (0.299 * r + 0.587 * g + 0.114 * b)

void test_gray()
{
	byte_t n1 = RGB_GRAY(255, 0, 255);
	byte_t n2 = RGB_GRAY(36, 0, 36);
}

void test_conv_gray()
{
	tchar_t fsize[INT_LEN] = { 0 };

	xfile_info(NULL, _T("pngtest.bmp"), NULL, fsize, NULL, NULL);

	dword_t dw = xstol(fsize);
	byte_t* buf = (byte_t*)xmem_alloc(dw);

	xhand_t xf = xfile_open(NULL, _T("person.bmp"), 0);

	xfile_read(xf, buf, &dw);

	xfile_close(xf);

	dword_t n = xbmp_convgray(buf, dw, NULL, MAX_LONG);
	byte_t* dst = (byte_t*)xmem_alloc(n);

	xbmp_convgray(buf, dw, dst, n);

	xmem_free(buf);

	xf = xfile_open(NULL, _T("D:\\Easily-app\\windows\\bin\\pngtest_dst.bmp"), FILE_OPEN_CREATE);
	xfile_write(xf, dst, &n);

	xmem_free(dst);
	xfile_close(xf);
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(XDL_PROCESS_APARTMENT);

	//test_conv_gray();

	test_gray();

	xdl_process_uninit();

	getch();

	return 0;
}

