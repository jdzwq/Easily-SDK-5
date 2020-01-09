// xdl_file_test.cpp : 定义控制台应用程序的入口点。
//

#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif


void test_share_read()
{
	DWORD dw = 8192;
	//XHANDLE gb = xshare_share(_T("tlscli"), _T("tlscli.crt"), dw);

	BYTE buf[8192] = { 0 };

	//XHANDLE cb = xshare_open(_T("tlscli"), 8192);

	dw = 8192;
	//xshare_read(cb, buf, &dw);

	printf("%s", (char*)buf);

	//xshare_close(cb);

	//xshare_close(gb);
}

void test_share_write()
{
	BYTE buf[8192] = { 0 };
	for (int i=0; i < 8192; i++)
		buf[i] = (BYTE)i;

	//XHANDLE cb = xshare_open(_T("tlscli"), 8192);

	DWORD dw = 8192;
	//xshare_write(cb, buf, &dw);

	//XHANDLE gb = xshare_open(_T("tlscli"), 8192);
	//xshare_read(cb, buf, &dw);

	//xshare_close(cb);

	//xshare_close(gb);
}

void test_block_read()
{
	DWORD dw = 8192;

	//XHANDLE gb = xblock_open(_T("D:\\Easily-app-5\\windows\\bin\\phone.jpg"), FILE_OPEN_READ);

	BYTE buf[20480] = { 0 };

	//xblock_read(gb, 0, 0, buf, 20480);

	//xblock_close(gb);
}

void test_block_write()
{
	DWORD dw = 8192;

	//XHANDLE gb = xblock_open(_T("D:\\Easily-app-5\\windows\\bin\\phone.jpg"), FILE_OPEN_CREATE);

	BYTE buf[20480] = { 0 };

	//xblock_write(gb, 0, 0, buf, 20480);

	//xblock_close(gb);
}

void test_block_append()
{
	//XHANDLE gb = xblock_open(_T("D:\\Easily-app-5\\windows\\bin\\phone.jpg"), FILE_OPEN_READ);

	BYTE buf[1024] = { 0 };

	//xblock_read(gb, 0, 0, buf, 1024);

	//xblock_close(gb);

	//gb = xblock_open(_T("D:\\Easily-app-5\\windows\\bin\\phone.jpg"), FILE_OPEN_APPEND);

	//xblock_write(gb, 0, 0, buf, 1024);

	//xblock_close(gb);
}

void test_block_truncate()
{
	//XHANDLE gb = xblock_open(_T("D:\\Easily-app-5\\windows\\bin\\phone.jpg"), FILE_OPEN_CREATE);

	//xblock_truncate(gb, 0, 1024);

	//xblock_close(gb);
}

void test_tftp_write()
{
	tchar_t fsize[INT_LEN] = { 0 };

	xfile_info(NULL, _T("./body.bmp"), NULL, fsize, NULL, NULL);

	dword_t dw = xstol(fsize);
	byte_t* buf = (byte_t*)xmem_alloc(dw);

	file_t f_loc = xfile_open(NULL, _T("./body.bmp"), FILE_OPEN_READ);
	xfile_read(f_loc, buf, dw);

	xfile_close(f_loc);

	file_t f_rmt = xfile_open(NULL, _T("tftp://127.0.0.1/body.bmp"), FILE_OPEN_CREATE);

	xfile_write(f_rmt, buf, dw);

	xfile_close(f_rmt);

	xmem_free(buf);
}

void test_tftp_read()
{
	tchar_t ftime[DATE_LEN] = { 0 };
	tchar_t fsize[NUM_LEN] = { 0 };

	xfile_info(NULL, _T("tftp://172.16.190.190/body.bmp"), ftime, fsize, NULL, NULL);

	file_t f_loc = xfile_open(NULL, _T("./body2.bmp"), FILE_OPEN_CREATE);

	file_t f_rmt = xfile_open(NULL, _T("tftp://172.16.190.190/body.bmp"), FILE_OPEN_READ);

	dword_t dw = xstol(fsize);
	byte_t* buf = (byte_t*)xmem_alloc(dw);

	xfile_read(f_rmt, buf, dw);

	xfile_write(f_loc, buf, dw);

	xfile_close(f_rmt);
	xfile_close(f_loc);

	xmem_free(buf);
}

void test_tftp_delete()
{
	xfile_delete(NULL, _T("tftp://127.0.0.1/body.bmp"));
}

void test_tftp_list()
{
	LINKPTR ptr = create_list_doc();

	xfile_list(NULL, _T("tftp://127.0.0.1/*.bmp"), ptr);

	destroy_list_doc(ptr);
}

void test_load()
{
	tchar_t sz_file[PATH_LEN];
	byte_t* file_buf;
	dword_t size;
	tchar_t* base_buf;
	int tlen, len;
	tchar_t sz_type[RES_LEN];

	xsprintf(sz_file, _T("body.jpg"));

	size = load_image_bytes_from_file(NULL, sz_file, NULL, NULL, MAX_LONG);
	if (!size)
		return;

	file_buf = (byte_t*)xmem_alloc(size);

	size = load_image_bytes_from_file(NULL, sz_file, sz_type, file_buf, size);
	if (!size)
	{
		xmem_free(file_buf);
		return;
	}

	len = size;
	size = xbas_encode(file_buf, len, NULL, MAX_LONG);
	tlen = xslen(GDI_ATTR_IMAGE_TYPE_JPG);

	string_t file_data = string_alloc();

	base_buf = string_ensure_buf(file_data, tlen + size);
	xsncpy(base_buf, GDI_ATTR_IMAGE_TYPE_JPG, tlen);
	xbas_encode(file_buf, len, base_buf + tlen, size);

	xmem_free(file_buf);

	string_free(file_data);
}


int main()
{
	xdl_process_init(XDL_APARTMENT_THREAD);

	//test_share_read();

	//test_share_write();

	//test_tftp_write();

	test_tftp_read();

	//test_tftp_delete();

	//test_tftp_list();

	//test_block_read();

	//test_block_write();

	//test_block_append();

	//test_block_truncate();

	//test_load();

	xdl_process_uninit();

	getch();

	return 0;
}

